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
			"li x5, 212\n"
			"faddi.pi f2, f0, 151\n"
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
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
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
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
			"li x0, 0x320\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x6\n" 
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
			"la x12, rand_int_32\n"
			"flw.ps f21, 352(x12)\n"
			"flw.ps f20, 0(x12)\n"
			"fsll.pi f27, f21, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 288(x26)\n"
			"flw.ps f20, 608(x26)\n"
			"fmul.s f20, f17, f20, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 608(x18)\n"
			"flw.ps f11, 768(x18)\n"
			"fsgnjx.ps f8, f19, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f6, 256(x25)\n"
			"fcvt.ps.pw f4, f6, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 288(x10)\n"
			"fround.ps f15, f6, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m4, x27, 0x65\n" 
			"li x5, 0x80340148\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_int_32\n"
			"flw.ps f27, 640(x18)\n"
			"faddi.pi f1, f27, 0x130\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 512(x25)\n"
			"flw.ps f18, 224(x25)\n"
			"fmin.s f27, f19, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 576(x9)\n"
			"fltu.pi f24, f4, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 576(x12)\n"
			"fle.s x25, f20, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 288(x27)\n"
			"flw.ps f17, 256(x27)\n"
			"fle.pi f21, f1, f17\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 96(x10)\n"
			"flw.ps f19, 192(x10)\n"
			"fmaxu.pi f20, f27, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 288(x10)\n"
			"fsrli.pi f27, f1, 0x5\n" 
			"li x27, 0xf0a3665c3387e5bb\n"
			"fcvt.s.lu f6, x27, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 352(x19)\n"
			"fclass.ps f21, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 640(x17)\n"
			"flw.ps f17, 896(x17)\n"
			"fnmadd.s f18, f31, f17, f17, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x17, 0x200000004760029\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x17, x17, x7\n"
			"csrw tensor_reduce, x17\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_909191_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xf9 << TENSOR_IMA_SCP_LOC_B) | (0xb5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f24, 320(x9)\n"
			"flw.ps f27, 576(x9)\n"
			"fxor.pi f6, f24, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 224(x25)\n"
			"fsgnj.ps f4, f11, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 288(x12)\n"
			"flw.ps f20, 992(x12)\n"
			"fadd.ps f15, f21, f20, rne\n" 
			"li x5, 0x173d\n"
			"csrw tensor_mask, x5\n"	
			"li x26, 0xdf74750fad19511\n"
			"fcvt.s.w f21, x26, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 224(x19)\n"
			"flw.ps f19, 800(x19)\n"
			"fcmovm.ps f31, f6, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 352(x15)\n"
			"flw.ps f1, 736(x15)\n"
			"fsgnjx.s f17, f8, f1\n" 
			"li x9, 0x9c7ee1001941b74a\n"
			"fcvt.s.w f6, x9, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 448(x10)\n"
			"flw.ps f24, 32(x10)\n"
			"fsub.ps f18, f15, f24, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f31, 160(x26)\n"
			"flw.ps f1, 160(x26)\n"
			"fmin.pi f1, f31, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f15, 896(x19)\n"
			"fsatu8.pi f31, f15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 160(x10)\n"
			"flw.ps f6, 544(x10)\n"
			"fle.ps f13, f31, f6\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_0:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 192(x15)\n"
			"flog.ps f20, f4\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 512(x18)\n"
			"flw.ps f21, 384(x18)\n"
			"fmulhu.pi f9, f19, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 256(x12)\n"
			"fandi.pi f4, f19, 0xff\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"fsetm.pi m5, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 96(x10)\n"
			"fexp.ps f27, f24\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x18, 0x160000000307000b\n"
			"csrw tensor_reduce, x18\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_430172_HID_0:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0xa\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f24, 512(x10)\n"
			"fltu.pi f24, f24, f24\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 416(x19)\n"
			"fcvt.pwu.ps f15, f18, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 992(x25)\n"
			"flw.ps f27, 576(x25)\n"
			"flw.ps f15, 256(x25)\n"
			"fnmsub.ps f9, f1, f27, f15, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 256(x15)\n"
			"fcvt.pw.ps f1, f20, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 256(x27)\n"
			"fltu.pi f13, f20, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 512(x9)\n"
			"flw.ps f24, 672(x9)\n"
			"fsgnjx.s f11, f20, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 672(x12)\n"
			"flw.ps f11, 320(x12)\n"
			"fadd.ps f21, f31, f11, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 640(x19)\n"
			"flw.ps f8, 704(x19)\n"
			"flt.ps f18, f20, f8\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 512(x9)\n"
			"flw.ps f15, 960(x9)\n"
			"fand.pi f6, f9, f15\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 160(x18)\n"
			"flw.ps f13, 96(x18)\n"
			"fdivu.pi f8, f18, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 128(x15)\n"
			"flw.ps f11, 576(x15)\n"
			"fsgnj.s f19, f17, f11\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 32(x10)\n"
			"fexp.ps f27, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 672(x26)\n"
			"fnot.pi f27, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 160(x27)\n"
			"flw.ps f1, 192(x27)\n"
			"fmulh.pi f4, f13, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 512(x19)\n"
			"fsin.ps f15, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 800(x17)\n"
			"faddi.pi f6, f31, 0xe\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_996812_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 0xe48cf9dfdeb410b1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8fa38ad41b111dd1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x25, rand_int_32\n"
			"flw.ps f19, 320(x25)\n"
			"fsrai.pi f15, f19, 0x7\n" 
			"li x15, 0x7197246e4186d741\n"
			"fcvt.s.wu f24, x15, rdn\n" 
			"li x26, 0xcab56e4b1716cce8\n"
			"fcvt.s.w f17, x26, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 736(x12)\n"
			"flw.ps f9, 704(x12)\n"
			"flw.ps f18, 640(x12)\n"
			"fnmsub.ps f8, f4, f9, f18, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 320(x9)\n"
			"flw.ps f27, 704(x9)\n"
			"fadd.pi f21, f6, f27\n" 
			"mova.x.m x12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f31, 896(x18)\n"
			"flw.ps f15, 128(x18)\n"
			"feq.pi f8, f31, f15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 704(x10)\n"
			"flw.ps f19, 192(x10)\n"
			"flw.ps f11, 160(x10)\n"
			"fcmov.ps f13, f6, f19, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_0:\n"
			"masknot m0, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x27\n" 
			"li x5, 0xf2c9\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 736(x17)\n"
			"flw.ps f18, 32(x17)\n"
			"flw.ps f1, 704(x17)\n"
			"fnmsub.ps f24, f8, f18, f1, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 416(x12)\n"
			"fcvt.wu.s x27, f6, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 480(x12)\n"
			"flw.ps f18, 320(x12)\n"
			"fminu.pi f8, f19, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 32(x12)\n"
			"fclass.s x27, f18\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 320(x27)\n"
			"flw.ps f20, 32(x27)\n"
			"feq.s x25, f19, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 736(x10)\n"
			"fround.ps f1, f6, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_745754_HID_0:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x22 << TENSOR_IMA_SCP_LOC_B) | (0xf2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"fbci.pi f9, 0x1bce8\n" 
			"fbci.pi f1, 0x54a02\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 128(x26)\n"
			"flw.ps f27, 608(x26)\n"
			"fadd.ps f13, f1, f27, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 736(x12)\n"
			"flw.ps f8, 672(x12)\n"
			"fmul.s f1, f21, f8, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 544(x15)\n"
			"flw.ps f6, 992(x15)\n"
			"flw.ps f27, 896(x15)\n"
			"fmadd.ps f8, f1, f6, f27, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 672(x25)\n"
			"flw.ps f20, 320(x25)\n"
			"fsgnjx.ps f24, f4, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 576(x27)\n"
			"flw.ps f17, 448(x27)\n"
			"flw.ps f11, 384(x27)\n"
			"fmsub.ps f6, f20, f17, f11, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 224(x26)\n"
			"flw.ps f11, 416(x26)\n"
			"fsgnjn.s f4, f13, f11\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 480(x25)\n"
			"fsgnjx.ps f13, f6, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 704(x25)\n"
			"flw.ps f17, 416(x25)\n"
			"fsgnj.s f21, f11, f17\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 96(x12)\n"
			"flw.ps f15, 768(x12)\n"
			"fmax.ps f1, f9, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 384(x27)\n"
			"fpackreph.pi f27, f8\n" 
			"li x5, 0x7827\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_0:\n"
			"flem.ps m0, f20, f9\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 960(x26)\n"
			"flw.ps f20, 608(x26)\n"
			"fsub.ps f21, f11, f20, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_0:\n"
			"mov.m.x m0, x10, 0x76\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8aa7725f2a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 992(x25)\n"
			"flw.ps f13, 96(x25)\n"
			"fnmsub.ps f21, f6, f13, f13, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"flw.ps f18, 704(x19)\n"
			"fsgnjx.ps f20, f18, f18\n" 
			"li x12, 0x9a4506c2aac9b479\n"
			"fcvt.s.wu f31, x12, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 256(x19)\n"
			"flw.ps f24, 192(x19)\n"
			"fsgnjn.ps f8, f1, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 576(x12)\n"
			"flw.ps f11, 544(x12)\n"
			"fand.pi f17, f21, f11\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 96(x12)\n"
			"faddi.pi f6, f17, 0x137\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_0:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8e920ded1d\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 608(x26)\n"
			"fcvt.wu.s x18, f9, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 224(x26)\n"
			"frsq.ps f13, f11\n" 
			"LBL_SEQID_5_FP_TRANS_ID_9_HID_0:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 288(x26)\n"
			"frcp.ps f20, f4\n" 
			"LBL_SEQID_5_FP_TRANS_ID_10_HID_0:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 960(x25)\n"
			"flog.ps f27, f24\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 192(x25)\n"
			"flw.ps f6, 768(x25)\n"
			"fsub.pi f11, f1, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 768(x25)\n"
			"flw.ps f15, 320(x25)\n"
			"flw.ps f1, 320(x25)\n"
			"fcmov.ps f27, f20, f15, f1\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 768(x25)\n"
			"fpackrepb.pi f6, f15\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 384(x17)\n"
			"fsrai.pi f15, f21, 0x8\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_0:\n"
			"fltm.pi m4, f19, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 256(x10)\n"
			"fsin.ps f15, f21\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xb1b2\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x6a << TENSOR_FMA16_SCP_LOC_B) | (0xd7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"LBL_HPM_3_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 704(x6)\n"
			"fcvt.w.s x16, f17, rmm\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f3, 960(x7)\n"
			"fcvt.ps.f16 f16, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 128(x27)\n"
			"fclass.s x9, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 320(x27)\n"
			"flw.ps f26, 704(x27)\n"
			"fcmovm.ps f16, f25, f26\n" 
			"fbci.ps f3, 0x84b7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"maskor m2, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f25, 608(x9)\n"
			"flw.ps f17, 832(x9)\n"
			"fxor.pi f27, f25, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 608(x9)\n"
			"flw.ps f16, 384(x9)\n"
			"fmin.pi f26, f12, f16\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 160(x13)\n"
			"flw.ps f23, 864(x13)\n"
			"fsgnj.ps f29, f17, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 736(x25)\n"
			"fcvt.ps.pw f12, f22, rup\n" 
			"maskpopcz x6, m4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 64(x13)\n"
			"flw.ps f0, 416(x13)\n"
			"fnmadd.ps f23, f25, f25, f0, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskxor m3, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f6, 192(x13)\n"
			"fsra.pi f28, f6, f6\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 640(x9)\n"
			"flw.ps f17, 448(x9)\n"
			"fsra.pi f29, f16, f17\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 960(x13)\n"
			"flw.ps f20, 512(x13)\n"
			"fsgnjx.s f0, f16, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f17, 576(x9)\n"
			"flw.ps f23, 736(x9)\n"
			"fsra.pi f12, f17, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_1:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 768(x15)\n"
			"fexp.ps f23, f28\n" 
			"li x6, 0xd00970c9ea8f2e78\n"
			"fcvt.s.w f25, x6, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_1:\n"
			"fltm.pi m3, f20, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x25, 0x8c8b687f061ebf15\n"
			"fcvt.s.lu f16, x25, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 352(x9)\n"
			"flw.ps f9, 736(x9)\n"
			"fdiv.s f26, f25, f9, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 576(x9)\n"
			"flw.ps f20, 416(x9)\n"
			"fsub.pi f30, f12, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 0(x25)\n"
			"flw.ps f29, 672(x25)\n"
			"fmsub.s f23, f28, f28, f29, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f30, 704(x27)\n"
			"flw.ps f6, 0(x27)\n"
			"fsra.pi f28, f30, f6\n" 
			"maskpopcz x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x18, rand_int_32\n"
			"flw.ps f30, 800(x18)\n"
			"flw.ps f26, 736(x18)\n"
			"fltu.pi f17, f30, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 480(x15)\n"
			"flw.ps f12, 192(x15)\n"
			"flw.ps f20, 960(x15)\n"
			"fmsub.s f29, f27, f12, f20, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 992(x6)\n"
			"flw.ps f25, 512(x6)\n"
			"fand.pi f22, f26, f25\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 544(x7)\n"
			"fcvt.wu.s x27, f25, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"feqm.ps m0, f0, f26\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f9, 608(x15)\n"
			"fsra.pi f28, f9, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"li x27, 0x9d394a8bd842ba0e\n"
			"fcvt.s.w f29, x27, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 160(x11)\n"
			"fsgnjn.ps f12, f22, f22\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"masknot m3, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"maskand m0, m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_1:\n"
			"maskor m3, m4, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf6ac986ae6\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f23, 416(x7)\n"
			"flw.ps f3, 96(x7)\n"
			"for.pi f26, f23, f3\n" 
			"mova.x.m x27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_1:\n"
			"feqm.ps m0, f30, f22\n" 
			"li x5, 0x803401B0\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 864(x7)\n"
			"fswizz.ps f12, f12, 0x1e\n" 
			"LBL_SEQID_2_FP_TRANS_ID_9_HID_1:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 96(x15)\n"
			"flog.ps f17, f0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 960(x25)\n"
			"flw.ps f30, 832(x25)\n"
			"fsub.ps f0, f29, f30, rtz\n" 
			"li x13, 0x15ba41c43df88ec2\n"
			"fcvt.s.w f20, x13, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 64(x5)\n"
			"flw.ps f0, 640(x5)\n"
			"fmax.ps f22, f25, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 320(x11)\n"
			"flw.ps f22, 608(x11)\n"
			"fsgnj.ps f26, f20, f22\n" 
			"la x27, rand_int_32\n"
			"flw.ps f0, 928(x27)\n"
			"fsrli.pi f30, f0, 0xc\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 320(x13)\n"
			"flw.ps f28, 384(x13)\n"
			"fdiv.pi f20, f20, f28\n" 
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 576(x13)\n"
			"flw.ps f29, 288(x13)\n"
			"fsgnjx.ps f16, f30, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 96(x7)\n"
			"frsq.ps f28, f26\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 416(x5)\n"
			"flt.pi f22, f9, f9\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_1:\n"
			"mov.m.x m2, x16, 0x3f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_1:\n"
			"fltm.ps m3, f16, f17\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_1:\n"
			"fsetm.pi m2, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 192(x15)\n"
			"flw.ps f25, 672(x15)\n"
			"fsgnjx.s f17, f6, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 672(x11)\n"
			"fpackrepb.pi f12, f23\n" 
			"fbci.ps f30, 0xfb91\n" 
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_1:\n"
			"maskxor m2, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_14_HID_1:\n"
			"fsetm.pi m4, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 800(x27)\n"
			"flw.ps f20, 544(x27)\n"
			"fsgnjn.ps f20, f0, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 160(x18)\n"
			"fcvt.pwu.ps f9, f9, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_15_HID_1:\n"
			"mov.m.x m3, x27, 0x68\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f9, 864(x15)\n"
			"flw.ps f3, 64(x15)\n"
			"fminu.pi f23, f9, f3\n" 
			"LBL_SEQID_3_FP_TRANS_ID_16_HID_1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 256(x6)\n"
			"flog.ps f6, f17\n" 
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
			"la x9, rand_int_32\n"
			"flw.ps f20, 288(x9)\n"
			"flw.ps f6, 32(x9)\n"
			"for.pi f12, f20, f6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 0(x6)\n"
			"flw.ps f26, 864(x6)\n"
			"for.pi f25, f29, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 992(x9)\n"
			"flw.ps f20, 0(x9)\n"
			"fmulh.pi f27, f27, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 896(x9)\n"
			"flw.ps f12, 864(x9)\n"
			"fdiv.ps f17, f16, f12, rtz\n" 
			"maskpopc x18, m2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 480(x16)\n"
			"flw.ps f6, 832(x16)\n"
			"fsra.pi f23, f3, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 224(x27)\n"
			"flw.ps f0, 736(x27)\n"
			"fmul.ps f22, f25, f0, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 256(x13)\n"
			"flw.ps f30, 960(x13)\n"
			"fsll.pi f17, f12, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 480(x5)\n"
			"flw.ps f23, 448(x5)\n"
			"fsub.ps f23, f9, f23, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 512(x6)\n"
			"ffrc.ps f20, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f28, 96(x25)\n"
			"fpackreph.pi f26, f28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 832(x13)\n"
			"fcvt.ps.pw f25, f3, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 992(x15)\n"
			"fclass.s x13, f28\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 160(x16)\n"
			"fsrai.pi f9, f27, 0xc\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 736(x13)\n"
			"faddi.pi f20, f26, 0xc6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 288(x13)\n"
			"flw.ps f23, 64(x13)\n"
			"fsra.pi f20, f0, f23\n" 
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
			"fbci.pi f29, 0x526f0\n" 
			"li x7, 0x6c8f7e20e8547299\n"
			"fcvt.s.wu f17, x7, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 96(x18)\n"
			"flw.ps f17, 768(x18)\n"
			"flw.ps f29, 896(x18)\n"
			"fnmsub.s f20, f23, f17, f29, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 320(x25)\n"
			"flw.ps f20, 352(x25)\n"
			"flt.s x27, f17, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 320(x13)\n"
			"flw.ps f9, 256(x13)\n"
			"fdivu.pi f30, f28, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 992(x6)\n"
			"fsin.ps f9, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 160(x13)\n"
			"fcvt.wu.s x15, f17, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 896(x16)\n"
			"flw.ps f12, 960(x16)\n"
			"fmul.s f16, f25, f12, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 192(x15)\n"
			"flw.ps f16, 64(x15)\n"
			"fle.ps f30, f22, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 448(x9)\n"
			"flw.ps f27, 768(x9)\n"
			"fmul.s f3, f17, f27, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 832(x6)\n"
			"fpackrepb.pi f22, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 576(x25)\n"
			"flw.ps f28, 320(x25)\n"
			"fsgnjx.ps f27, f30, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 992(x6)\n"
			"flw.ps f9, 992(x6)\n"
			"flt.s x16, f30, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_17_HID_1:\n"
			"feqm.ps m4, f12, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f28, 544(x25)\n"
			"fltu.pi f6, f28, f28\n" 
			"maskpopc x15, m0\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"LBL_HPM_3_CORE_FTRANS_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_FTRANS_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"fbci.ps f10, 0x1d8a4\n" 
			"li x5, 0x3691379f36ad3493\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3056c3bfbb10fde5\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 64(x16)\n"
			"flw.ps f1, 32(x16)\n"
			"fmin.ps f18, f10, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 736(x11)\n"
			"frsq.ps f30, f18\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 992(x5)\n"
			"flw.ps f24, 352(x5)\n"
			"for.pi f3, f22, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskxor m7, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f10, 256(x28)\n"
			"flw.ps f1, 64(x28)\n"
			"fsll.pi f1, f10, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 384(x11)\n"
			"fsin.ps f14, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 992(x6)\n"
			"flw.ps f24, 960(x6)\n"
			"fltu.pi f22, f10, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 768(x22)\n"
			"flw.ps f22, 800(x22)\n"
			"fsgnjn.s f18, f4, f22\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 736(x5)\n"
			"flw.ps f30, 224(x5)\n"
			"fsub.pi f4, f14, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 928(x25)\n"
			"flw.ps f17, 160(x25)\n"
			"fadd.s f1, f1, f17, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 672(x28)\n"
			"flw.ps f3, 384(x28)\n"
			"fsgnjn.s f3, f18, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 480(x23)\n"
			"flw.ps f17, 288(x23)\n"
			"fsgnj.ps f1, f27, f17\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x22, 0x21ac74fe87730af4\n"
			"fcvt.s.w f17, x22, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f23, 448(x16)\n"
			"fcvt.ps.pw f14, f23, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 192(x19)\n"
			"flw.ps f4, 192(x19)\n"
			"fle.ps f10, f14, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_71703_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xf1 << TENSOR_FMA32_SCP_LOC_B) | (0x3c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f30, 224(x23)\n"
			"fpackrepb.pi f19, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 864(x25)\n"
			"fsub.s f19, f6, f6, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"flem.ps m0, f21, f6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe67f70fab3\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f30, 576(x22)\n"
			"flw.ps f19, 96(x22)\n"
			"fmin.pi f3, f30, f19\n" 
			"li x16, 0xc5cac768b9e35821\n"
			"fcvt.s.w f29, x16, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 64(x28)\n"
			"frsq.ps f21, f30\n" 
			"li x5, 0x88a00c6bc5133670\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xf8905da00ebe90a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"maskor m0, m1, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f3, 704(x11)\n"
			"fsatu8.pi f19, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 736(x23)\n"
			"flw.ps f21, 512(x23)\n"
			"fmadd.ps f24, f17, f17, f21, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 736(x25)\n"
			"flw.ps f19, 160(x25)\n"
			"fminu.pi f21, f27, f19\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 448(x11)\n"
			"flw.ps f1, 384(x11)\n"
			"feq.pi f3, f21, f1\n" 
			"la x28, rand_int_32\n"
			"flw.ps f30, 512(x28)\n"
			"flw.ps f21, 224(x28)\n"
			"fdivu.pi f29, f30, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 256(x28)\n"
			"flw.ps f22, 736(x28)\n"
			"fadd.s f21, f17, f22, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_2:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 256(x22)\n"
			"flog.ps f4, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"mova.m.x x17\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 960(x25)\n"
			"ffrc.ps f4, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_981373_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_718251_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x19, 0x160000000307000b\n"
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
			"li x5, 0xa0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"mova.x.m x23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 64(x11)\n"
			"fsin.ps f1, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 768(x19)\n"
			"flw.ps f24, 960(x19)\n"
			"flw.ps f1, 960(x19)\n"
			"fmsub.ps f29, f27, f24, f1, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f29, 896(x19)\n"
			"fsrli.pi f4, f29, 0x9\n" 
			"mova.x.m x19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_2:\n"
			"maskand m6, m1, m6\n" 
			"li x5, 0x80340108\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"fltm.pi m7, f23, f24\n" 
			"li x5, 0x80340140\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 768(x23)\n"
			"flw.ps f30, 576(x23)\n"
			"fcmovm.ps f17, f24, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 384(x6)\n"
			"flw.ps f27, 32(x6)\n"
			"fmul.s f29, f1, f27, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 192(x28)\n"
			"fadd.ps f21, f23, f23, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 32(x6)\n"
			"flw.ps f21, 864(x6)\n"
			"fsgnjx.s f23, f14, f21\n" 
			"li x23, 0x32e9fb32bdf4f108\n"
			"fcvt.s.w f3, x23, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 608(x17)\n"
			"flw.ps f27, 544(x17)\n"
			"fadd.s f17, f22, f27, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 256(x22)\n"
			"flw.ps f14, 96(x22)\n"
			"fmul.s f14, f1, f14, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 288(x5)\n"
			"flw.ps f1, 512(x5)\n"
			"flt.pi f24, f17, f1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 288(x22)\n"
			"flw.ps f14, 288(x22)\n"
			"fcmov.ps f4, f10, f14, f10\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xf\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 0(x23)\n"
			"flw.ps f22, 960(x23)\n"
			"fsgnj.ps f3, f10, f22\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_2:\n"
			"fsetm.pi m6, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 512(x17)\n"
			"fnot.pi f27, f30\n" 
			"maskpopc x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 192(x25)\n"
			"flw.ps f24, 288(x25)\n"
			"flw.ps f4, 32(x25)\n"
			"fmadd.s f10, f17, f24, f4, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_2:\n"
			"fltm.pi m1, f23, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 768(x16)\n"
			"flw.ps f17, 960(x16)\n"
			"flw.ps f21, 544(x16)\n"
			"fmadd.s f21, f3, f17, f21, rne\n" 
			"li x5, 0x6bcdce44e00bc585\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x62d355ba91c11151\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 64(x28)\n"
			"flw.ps f18, 256(x28)\n"
			"fsgnjx.s f19, f24, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_2:\n"
			"fltm.ps m6, f14, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 544(x5)\n"
			"flw.ps f10, 736(x5)\n"
			"fcmovm.ps f21, f29, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f21, 800(x22)\n"
			"fsrai.pi f14, f21, 0xb\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 864(x5)\n"
			"flw.ps f14, 704(x5)\n"
			"fsgnj.ps f1, f19, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 0(x23)\n"
			"flw.ps f23, 672(x23)\n"
			"feq.pi f22, f21, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 96(x5)\n"
			"flw.ps f14, 288(x5)\n"
			"fsgnj.ps f3, f10, f14\n" 
			"fbci.ps f17, 0x68e51\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 416(x23)\n"
			"flw.ps f4, 768(x23)\n"
			"flt.s x19, f30, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"maskpopcz x11, m1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 288(x6)\n"
			"flw.ps f10, 288(x6)\n"
			"flw.ps f18, 736(x6)\n"
			"fnmadd.s f19, f22, f10, f18, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 512(x19)\n"
			"flw.ps f24, 544(x19)\n"
			"fle.s x17, f18, f24\n" 
			"mova.x.m x23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 704(x28)\n"
			"flw.ps f10, 864(x28)\n"
			"flw.ps f19, 448(x28)\n"
			"fnmadd.s f10, f21, f10, f19, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_2:\n"
			"maskor m6, m0, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 416(x19)\n"
			"flw.ps f23, 384(x19)\n"
			"flt.s x23, f24, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 640(x19)\n"
			"flw.ps f22, 864(x19)\n"
			"fmul.ps f6, f23, f22, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 832(x17)\n"
			"flw.ps f10, 320(x17)\n"
			"fsub.s f10, f27, f10, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 512(x17)\n"
			"fcvt.f16.ps f3, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 320(x11)\n"
			"flw.ps f10, 640(x11)\n"
			"fcmovm.ps f17, f19, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 416(x25)\n"
			"flw.ps f29, 992(x25)\n"
			"fcmov.ps f24, f3, f29, f3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 64(x5)\n"
			"fmulhu.pi f6, f22, f22\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 576(x16)\n"
			"flw.ps f24, 800(x16)\n"
			"fsgnjn.s f19, f19, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 960(x5)\n"
			"flw.ps f19, 320(x5)\n"
			"fmul.pi f24, f27, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 608(x28)\n"
			"flw.ps f22, 960(x28)\n"
			"feq.pi f10, f6, f22\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 480(x11)\n"
			"flw.ps f17, 672(x11)\n"
			"fmul.ps f29, f21, f17, rne\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_2:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 384(x23)\n"
			"flog.ps f17, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 576(x19)\n"
			"fltu.pi f10, f30, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 928(x17)\n"
			"flw.ps f24, 128(x17)\n"
			"fmax.pi f18, f21, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 32(x6)\n"
			"flw.ps f3, 544(x6)\n"
			"fsgnjx.s f4, f27, f3\n" 
			"li x5, 0xfa360647d409564f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x14b57f07cab8963c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_2:\n"
			"fsetm.pi m6, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f21, 832(x28)\n"
			"fcvt.ps.pwu f3, f21, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x17, m6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f17, 768(x11)\n"
			"flw.ps f14, 32(x11)\n"
			"fsrl.pi f4, f17, f14\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_2:\n"
			"fltm.ps m1, f24, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_FP_TRANS_ID_14_HID_2:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 608(x17)\n"
			"frcp.ps f17, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 448(x25)\n"
			"flw.ps f22, 192(x25)\n"
			"fle.pi f29, f14, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 768(x6)\n"
			"fswizz.ps f1, f23, 0x15\n" 
			"LBL_SEQID_5_FP_TRANS_ID_15_HID_2:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 672(x6)\n"
			"frcp.ps f4, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 32(x28)\n"
			"flw.ps f4, 896(x28)\n"
			"fsgnjn.s f19, f29, f4\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_2:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 768(x6)\n"
			"flw.ps f30, 800(x6)\n"
			"fadd.s f23, f25, f30, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 160(x18)\n"
			"flw.ps f8, 544(x18)\n"
			"fsub.pi f17, f19, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 96(x10)\n"
			"flw.ps f30, 352(x10)\n"
			"fcmov.ps f16, f17, f30, f30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 704(x16)\n"
			"flw.ps f23, 896(x16)\n"
			"fmin.ps f18, f27, f23\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 960(x6)\n"
			"fround.ps f7, f30, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 512(x6)\n"
			"fclass.ps f20, f27\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 768(x28)\n"
			"flw.ps f16, 256(x28)\n"
			"fsub.s f4, f4, f16, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 256(x23)\n"
			"fcvt.wu.s x25, f18, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_3:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 704(x28)\n"
			"frcp.ps f23, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 480(x28)\n"
			"flw.ps f10, 832(x28)\n"
			"fmax.ps f18, f24, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 0(x10)\n"
			"flw.ps f27, 128(x10)\n"
			"fsgnj.ps f17, f0, f27\n" 
			"li x23, 0xe189563b5bb963f7\n"
			"fcvt.s.w f15, x23, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 448(x16)\n"
			"fnot.pi f4, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 416(x26)\n"
			"fnmsub.ps f7, f19, f19, f19, rtz\n" 
			"fbci.pi f10, 0x6b6b1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 800(x6)\n"
			"fclass.s x10, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f23, 96(x26)\n"
			"fcvt.ps.pwu f18, f23, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 32(x26)\n"
			"flw.ps f20, 544(x26)\n"
			"fxor.pi f24, f23, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f30, 640(x18)\n"
			"fsat8.pi f20, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 0(x28)\n"
			"flw.ps f18, 832(x28)\n"
			"fmin.ps f20, f25, f18\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 864(x10)\n"
			"flw.ps f16, 416(x10)\n"
			"fadd.pi f18, f20, f16\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 704(x5)\n"
			"fcvt.wu.s x18, f25, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_3:\n"
			"fsetm.pi m5, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 352(x16)\n"
			"flw.ps f0, 672(x16)\n"
			"flw.ps f30, 192(x16)\n"
			"fnmsub.s f27, f16, f0, f30, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 288(x18)\n"
			"flw.ps f24, 256(x18)\n"
			"fmul.s f23, f0, f24, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 384(x6)\n"
			"flw.ps f24, 800(x6)\n"
			"flt.pi f7, f0, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 160(x7)\n"
			"flw.ps f4, 672(x7)\n"
			"flw.ps f17, 192(x7)\n"
			"fnmsub.s f25, f16, f4, f17, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 800(x5)\n"
			"faddi.pi f0, f27, 0xc0\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 128(x10)\n"
			"flw.ps f23, 928(x10)\n"
			"fsgnj.ps f27, f8, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 736(x7)\n"
			"fcvt.ps.pwu f25, f10, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 640(x22)\n"
			"fsatu8.pi f15, f7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_3:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 896(x6)\n"
			"flog.ps f7, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f15, 288(x6)\n"
			"faddi.pi f20, f15, 0x14c\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 128(x18)\n"
			"fclass.ps f0, f23\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 960(x5)\n"
			"flw.ps f15, 832(x5)\n"
			"fminu.pi f7, f20, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 704(x5)\n"
			"fsatu8.pi f4, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 448(x7)\n"
			"flw.ps f27, 672(x7)\n"
			"fle.pi f18, f25, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 512(x25)\n"
			"ffrc.ps f10, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 0(x26)\n"
			"flw.ps f7, 864(x26)\n"
			"flw.ps f24, 864(x26)\n"
			"fnmadd.s f10, f4, f7, f24, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_3:\n"
			"mov.m.x m3, x10, 0x29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x10, 0xdd1e930806fe2352\n"
			"fcvt.s.lu f30, x10, rmm\n" 
			"fbci.ps f17, 0x67bcd\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 768(x28)\n"
			"flw.ps f7, 832(x28)\n"
			"fle.s x22, f27, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 480(x18)\n"
			"flw.ps f23, 672(x18)\n"
			"fsgnjx.s f0, f4, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 864(x28)\n"
			"flw.ps f15, 832(x28)\n"
			"fmul.ps f23, f19, f15, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 320(x16)\n"
			"flw.ps f23, 992(x16)\n"
			"fcmovm.ps f17, f16, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 352(x25)\n"
			"fclass.ps f20, f0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 704(x16)\n"
			"flw.ps f10, 512(x16)\n"
			"flw.ps f23, 352(x16)\n"
			"fmadd.ps f27, f25, f10, f23, rtz\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 448(x7)\n"
			"flw.ps f17, 704(x7)\n"
			"flw.ps f10, 544(x7)\n"
			"fmsub.s f0, f16, f17, f10, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 256(x23)\n"
			"flw.ps f0, 960(x23)\n"
			"fmin.s f18, f30, f0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 320(x25)\n"
			"fsatu8.pi f20, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 608(x16)\n"
			"fpackrepb.pi f15, f20\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f16, 832(x18)\n"
			"fcvt.ps.f16 f7, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 416(x7)\n"
			"flw.ps f7, 640(x7)\n"
			"fmin.ps f27, f23, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 736(x26)\n"
			"fswizz.ps f0, f15, 0x15\n" 
			"li x6, 0x39af611c866e13d8\n"
			"fcvt.s.w f15, x6, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 416(x7)\n"
			"ffrc.ps f18, f10\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 832(x5)\n"
			"flw.ps f7, 672(x5)\n"
			"flw.ps f20, 384(x5)\n"
			"fmadd.s f27, f17, f7, f20, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 672(x25)\n"
			"flw.ps f4, 160(x25)\n"
			"fadd.ps f18, f27, f4, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f16, 448(x16)\n"
			"flw.ps f7, 352(x16)\n"
			"fsra.pi f0, f16, f7\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_3:\n"
			"fsetm.pi m0, f19\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 128(x22)\n"
			"flw.ps f20, 416(x22)\n"
			"fmin.s f15, f0, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 256(x28)\n"
			"flw.ps f17, 160(x28)\n"
			"fxor.pi f27, f18, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f18, 288(x26)\n"
			"flw.ps f15, 992(x26)\n"
			"for.pi f0, f18, f15\n" 
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
			"li x16, 0x1ca951e19009c6ee\n"
			"fcvt.s.w f7, x16, rtz\n" 
			"fbci.pi f20, 0x4bdc8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 352(x5)\n"
			"flw.ps f19, 416(x5)\n"
			"fle.pi f30, f30, f19\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_3:\n"
			"fsetm.pi m5, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 960(x28)\n"
			"flw.ps f0, 512(x28)\n"
			"flt.ps f19, f19, f0\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 96(x16)\n"
			"flw.ps f8, 768(x16)\n"
			"fmaxu.pi f7, f17, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 640(x6)\n"
			"fsgnjx.ps f18, f24, f24\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 832(x16)\n"
			"flw.ps f10, 192(x16)\n"
			"fle.pi f18, f4, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 192(x10)\n"
			"flw.ps f27, 928(x10)\n"
			"feq.ps f25, f7, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 576(x18)\n"
			"flw.ps f4, 384(x18)\n"
			"fmul.ps f10, f17, f4, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 448(x28)\n"
			"fpackrepb.pi f23, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 32(x7)\n"
			"flw.ps f4, 640(x7)\n"
			"fadd.s f19, f30, f4, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f30, 320(x25)\n"
			"fsat8.pi f19, f30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 128(x16)\n"
			"flw.ps f0, 384(x16)\n"
			"flw.ps f23, 608(x16)\n"
			"fmadd.ps f4, f24, f0, f23, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 192(x16)\n"
			"fandi.pi f10, f18, 0x19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 416(x18)\n"
			"flw.ps f30, 704(x18)\n"
			"fdiv.ps f15, f25, f30, rdn\n" 
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
			"LBL_SEQID_5_FP_TRANS_ID_6_HID_3:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 256(x16)\n"
			"fexp.ps f4, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 896(x7)\n"
			"fnot.pi f27, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 640(x16)\n"
			"fround.ps f7, f18, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 160(x18)\n"
			"flw.ps f30, 640(x18)\n"
			"fsgnjx.s f24, f18, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 640(x25)\n"
			"flw.ps f19, 224(x25)\n"
			"fdiv.ps f24, f20, f19, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 960(x10)\n"
			"fsll.pi f17, f19, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 448(x25)\n"
			"flw.ps f24, 672(x25)\n"
			"fltu.pi f0, f10, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 192(x23)\n"
			"flw.ps f8, 256(x23)\n"
			"flw.ps f15, 416(x23)\n"
			"fmsub.s f18, f20, f8, f15, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 256(x16)\n"
			"fcvt.f16.ps f23, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 960(x6)\n"
			"flw.ps f27, 512(x6)\n"
			"fmax.ps f0, f0, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 0(x25)\n"
			"fsgnjn.ps f7, f27, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 384(x10)\n"
			"fcvt.f16.ps f27, f10\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_3:\n"
			"maskxor m7, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_3:\n"
			"fltm.ps m7, f25, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f15, 96(x18)\n"
			"flw.ps f30, 96(x18)\n"
			"fsub.pi f19, f15, f30\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 832(x25)\n"
			"flw.ps f8, 640(x25)\n"
			"fmin.pi f19, f27, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x18, rand_int_32\n"
			"flw.ps f29, 608(x18)\n"
			"flw.ps f21, 96(x18)\n"
			"fsra.pi f24, f29, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"mov.m.x m4, x6, 0x54\n" 
			"li x5, 0x803401C0\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_int_32\n"
			"flw.ps f29, 64(x15)\n"
			"flw.ps f15, 288(x15)\n"
			"fsub.pi f1, f29, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 256(x12)\n"
			"flw.ps f31, 864(x12)\n"
			"flw.ps f20, 832(x12)\n"
			"fmadd.s f31, f23, f31, f20, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 928(x26)\n"
			"flw.ps f24, 864(x26)\n"
			"flw.ps f15, 800(x26)\n"
			"fcmov.ps f21, f20, f24, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f30, 864(x12)\n"
			"flw.ps f21, 736(x12)\n"
			"fminu.pi f21, f30, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f30, 512(x18)\n"
			"flw.ps f21, 192(x18)\n"
			"fmulh.pi f15, f30, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 832(x12)\n"
			"fsrai.pi f27, f31, 0xf\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 640(x27)\n"
			"flw.ps f24, 960(x27)\n"
			"fmulh.pi f21, f15, f24\n" 
			"maskpopc x13, m7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 736(x6)\n"
			"flw.ps f27, 512(x6)\n"
			"fle.ps f24, f15, f27\n" 
			"li x5, 0xbcc0\n"
			"csrw tensor_mask, x5\n"	
			"la x18, rand_int_32\n"
			"flw.ps f14, 736(x18)\n"
			"fsat8.pi f20, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 448(x23)\n"
			"fle.s x12, f30, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 576(x14)\n"
			"fcvt.ps.pwu f15, f6, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 544(x17)\n"
			"fclass.ps f27, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 448(x6)\n"
			"fcvt.w.s x15, f13, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_986537_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbd << TENSOR_IMA_SCP_LOC_B) | (0x20 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 512(x23)\n"
			"flw.ps f18, 544(x23)\n"
			"fsgnjx.s f2, f30, f18\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 704(x14)\n"
			"flw.ps f24, 768(x14)\n"
			"fsgnjn.s f6, f21, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 96(x13)\n"
			"flw.ps f13, 128(x13)\n"
			"flw.ps f15, 64(x13)\n"
			"fmadd.s f13, f23, f13, f15, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 0(x13)\n"
			"flw.ps f20, 736(x13)\n"
			"flw.ps f24, 288(x13)\n"
			"fnmadd.ps f9, f31, f20, f24, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 896(x23)\n"
			"flw.ps f24, 992(x23)\n"
			"flw.ps f27, 448(x23)\n"
			"fmsub.ps f15, f29, f24, f27, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 32(x26)\n"
			"flw.ps f9, 928(x26)\n"
			"feq.s x6, f1, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 736(x13)\n"
			"flw.ps f23, 608(x13)\n"
			"flw.ps f6, 544(x13)\n"
			"fmadd.ps f20, f9, f23, f6, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 480(x26)\n"
			"fsin.ps f21, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 384(x6)\n"
			"flw.ps f27, 896(x6)\n"
			"fmin.s f15, f9, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 832(x17)\n"
			"fmin.ps f18, f1, f1\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 448(x15)\n"
			"fpackreph.pi f20, f29\n" 
			"li x5, 0xc4e706a3b70f515b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2d1994e77cf92927\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 832(x12)\n"
			"flw.ps f20, 384(x12)\n"
			"fadd.s f30, f23, f20, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 864(x23)\n"
			"flw.ps f1, 864(x23)\n"
			"feq.s x12, f29, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 192(x15)\n"
			"flw.ps f14, 928(x15)\n"
			"flt.ps f9, f6, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 608(x27)\n"
			"flw.ps f1, 192(x27)\n"
			"fdiv.s f9, f20, f1, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_int_32\n"
			"flw.ps f14, 640(x14)\n"
			"flw.ps f31, 800(x14)\n"
			"fsub.pi f6, f14, f31\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x15, 0x160000000307000b\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xcc << TENSOR_FMA32_SCP_LOC_B) | (0x60 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 160(x13)\n"
			"fclass.ps f30, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f18, 640(x23)\n"
			"flw.ps f9, 736(x23)\n"
			"fmul.pi f14, f18, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 64(x26)\n"
			"fcvt.w.s x15, f1, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f2, 128(x12)\n"
			"fcvt.ps.pw f23, f2, rtz\n" 
			"li x5, 0xe94e10ff76ff3c3e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd63a890eef48a975\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f20, 128(x13)\n"
			"flw.ps f6, 320(x13)\n"
			"flt.pi f6, f20, f6\n" 
			"fbci.pi f13, 0x738f1\n" 
			"li x5, 0xfa211ef754e27921\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xadf2eb906424d83\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 32(x23)\n"
			"flw.ps f15, 480(x23)\n"
			"fsgnjx.s f23, f24, f15\n" 
			"fbci.ps f21, 0x60eab\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 800(x23)\n"
			"fcvt.ps.pwu f13, f20, rup\n" 
			"li x12, 0x2a13d176ed093d5b\n"
			"fcvt.s.w f29, x12, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 160(x23)\n"
			"flw.ps f9, 352(x23)\n"
			"fsub.pi f18, f6, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_1_HID_4:\n"
			"mov.m.x m1, x15, 0x6e\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_int_32\n"
			"flw.ps f29, 992(x13)\n"
			"faddi.pi f13, f29, 0xd\n" 
			"li x5, 0xa21\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f13, 0xee58\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 928(x18)\n"
			"flw.ps f31, 992(x18)\n"
			"feq.pi f23, f15, f31\n" 
			"maskpopc x23, m1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 832(x17)\n"
			"flw.ps f15, 160(x17)\n"
			"fmin.ps f30, f24, f15\n" 
			"LBL_SEQID_3_M0_WRITE_ID_2_HID_4:\n"
			"fltm.ps m6, f1, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_4:\n"
			"fltm.pi m4, f21, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 576(x18)\n"
			"flw.ps f27, 480(x18)\n"
			"fmul.s f6, f18, f27, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 512(x6)\n"
			"flw.ps f9, 608(x6)\n"
			"fmulh.pi f23, f27, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 768(x15)\n"
			"flw.ps f23, 960(x15)\n"
			"fmulh.pi f31, f9, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 320(x6)\n"
			"flw.ps f21, 256(x6)\n"
			"fadd.pi f1, f31, f21\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_4:\n"
			"fltm.ps m4, f1, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x18\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 608(x23)\n"
			"flw.ps f1, 544(x23)\n"
			"fadd.ps f30, f18, f1, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 224(x17)\n"
			"flw.ps f20, 64(x17)\n"
			"fmin.s f9, f23, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_4:\n"
			"fltm.ps m7, f13, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 928(x27)\n"
			"frsq.ps f18, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 256(x12)\n"
			"flw.ps f31, 160(x12)\n"
			"fsgnjn.ps f27, f18, f31\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 352(x12)\n"
			"fswizz.ps f30, f21, 0x1a\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 416(x23)\n"
			"fsgnjn.s f14, f27, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f21, 512(x14)\n"
			"flw.ps f15, 544(x14)\n"
			"fxor.pi f1, f21, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 320(x12)\n"
			"fcvt.wu.s x6, f1, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_4:\n"
			"maskand m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f15, 32(x6)\n"
			"flw.ps f27, 768(x6)\n"
			"fsrl.pi f14, f15, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 0(x18)\n"
			"flw.ps f27, 544(x18)\n"
			"flw.ps f23, 768(x18)\n"
			"fmadd.ps f27, f30, f27, f23, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_4:\n"
			"fsetm.pi m1, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f23, 704(x27)\n"
			"flw.ps f21, 288(x27)\n"
			"fmin.pi f14, f23, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 32(x18)\n"
			"flw.ps f27, 640(x18)\n"
			"feq.pi f31, f18, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 640(x6)\n"
			"fcvt.f16.ps f13, f23\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 256(x12)\n"
			"flw.ps f27, 672(x12)\n"
			"flt.s x17, f20, f27\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_4:\n"
			"feqm.ps m6, f21, f13\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xde1e2187dd\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 128(x17)\n"
			"flw.ps f14, 768(x17)\n"
			"flw.ps f21, 128(x17)\n"
			"fnmsub.ps f31, f27, f14, f21, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 160(x15)\n"
			"fswizz.ps f9, f27, 0x18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 480(x18)\n"
			"fpackreph.pi f31, f18\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 576(x14)\n"
			"flw.ps f9, 768(x14)\n"
			"fmax.s f13, f13, f9\n" 
			"li x5, 0x1fee42f82d1d045e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa09b3464cc27508d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 160(x13)\n"
			"flw.ps f15, 672(x13)\n"
			"flt.s x13, f1, f15\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
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
			"mova.x.m x27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 480(x15)\n"
			"flw.ps f1, 640(x15)\n"
			"fmax.ps f21, f18, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 704(x15)\n"
			"flw.ps f9, 768(x15)\n"
			"fsub.s f6, f30, f9, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 32(x15)\n"
			"flw.ps f18, 96(x15)\n"
			"flt.s x18, f13, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 320(x6)\n"
			"fclass.s x18, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_4:\n"
			"fltm.ps m7, f27, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 736(x6)\n"
			"fcvt.lu.s x26, f30, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 832(x18)\n"
			"flw.ps f29, 64(x18)\n"
			"fsub.ps f21, f31, f29, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 960(x26)\n"
			"flw.ps f2, 128(x26)\n"
			"fle.s x17, f20, f2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 640(x27)\n"
			"fswizz.ps f30, f1, 0xc\n" 
			"la x27, rand_int_32\n"
			"flw.ps f24, 128(x27)\n"
			"flw.ps f21, 672(x27)\n"
			"frem.pi f6, f24, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f18, 512(x6)\n"
			"fsrli.pi f30, f18, 0x9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 128(x26)\n"
			"fcvt.ps.pwu f21, f30, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 320(x6)\n"
			"flw.ps f18, 96(x6)\n"
			"flt.s x27, f9, f18\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 640(x12)\n"
			"flw.ps f20, 0(x12)\n"
			"fmulhu.pi f15, f31, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 576(x23)\n"
			"fcvt.ps.pw f13, f14, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_736967_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xa0 << TENSOR_FMA32_SCP_LOC_B) | (0xb5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
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
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_5\n"
			"li x5, 0x6\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 480(x7)\n"
			"ffrc.ps f25, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 704(x20)\n"
			"flw.ps f18, 32(x20)\n"
			"fsgnjx.s f15, f24, f18\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 672(x7)\n"
			"flw.ps f14, 960(x7)\n"
			"fsll.pi f19, f18, f14\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 288(x13)\n"
			"flw.ps f25, 896(x13)\n"
			"flw.ps f1, 384(x13)\n"
			"fcmov.ps f27, f21, f25, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 896(x14)\n"
			"flw.ps f24, 96(x14)\n"
			"fnmsub.s f24, f15, f24, f24, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 224(x7)\n"
			"fsrai.pi f0, f24, 0x0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"masknot m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f1, 480(x22)\n"
			"flw.ps f5, 384(x22)\n"
			"flt.pi f21, f1, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 672(x9)\n"
			"fsin.ps f10, f19\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 128(x10)\n"
			"flw.ps f13, 256(x10)\n"
			"fcmovm.ps f0, f8, f13\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 256(x13)\n"
			"fdiv.ps f19, f28, f28, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"fsetm.pi m0, f0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x20, 0x5d1fd7112dfc3fca\n"
			"fcvt.s.l f26, x20, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 128(x26)\n"
			"flw.ps f15, 864(x26)\n"
			"fsgnjn.ps f13, f24, f15\n" 
			"li x14, 0x7110a1b24bb6497c\n"
			"fcvt.s.wu f25, x14, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 960(x9)\n"
			"faddi.pi f8, f19, 0x87\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f0, 256(x26)\n"
			"faddi.pi f26, f0, 0x78\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 736(x12)\n"
			"flw.ps f15, 192(x12)\n"
			"fmul.pi f5, f26, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"mov.m.x m7, x7, 0x30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_16\n"
			"flw.ps f28, 64(x15)\n"
			"fcvt.ps.f16 f13, f28\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 800(x15)\n"
			"fpackrepb.pi f15, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 960(x20)\n"
			"flw.ps f5, 256(x20)\n"
			"fsgnjn.ps f13, f1, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 896(x25)\n"
			"fcvt.f16.ps f0, f5\n" 
			"fbci.ps f28, 0x56b0c\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 672(x22)\n"
			"frsq.ps f18, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 544(x20)\n"
			"flw.ps f1, 832(x20)\n"
			"fsll.pi f18, f8, f1\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 32(x13)\n"
			"flw.ps f10, 480(x13)\n"
			"fsrl.pi f8, f25, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 864(x20)\n"
			"flw.ps f26, 96(x20)\n"
			"fsgnj.s f21, f5, f26\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 416(x7)\n"
			"flw.ps f13, 512(x7)\n"
			"fsrl.pi f10, f25, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f15, 320(x20)\n"
			"flw.ps f21, 352(x20)\n"
			"feq.pi f26, f15, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 992(x20)\n"
			"flw.ps f5, 832(x20)\n"
			"feq.pi f28, f21, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 64(x14)\n"
			"fcvt.w.s x25, f25, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 96(x14)\n"
			"fcvt.pwu.ps f0, f25, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_5:\n"
			"fsetm.pi m7, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f28, 448(x25)\n"
			"fnot.pi f5, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 928(x13)\n"
			"flw.ps f24, 960(x13)\n"
			"fsgnj.s f15, f1, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 832(x15)\n"
			"fclass.s x9, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 672(x7)\n"
			"flw.ps f19, 224(x7)\n"
			"fnmsub.s f8, f13, f19, f19, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_5:\n"
			"fltm.pi m3, f15, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x785f83b44c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 224(x7)\n"
			"ffrc.ps f18, f1\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 480(x26)\n"
			"fpackrepb.pi f26, f28\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 736(x26)\n"
			"fsat8.pi f8, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 256(x7)\n"
			"frsq.ps f18, f13\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 608(x13)\n"
			"flw.ps f27, 32(x13)\n"
			"fsub.pi f14, f10, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f15, 928(x12)\n"
			"fcvt.ps.pwu f13, f15, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 448(x20)\n"
			"fsrai.pi f10, f0, 0x9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 800(x9)\n"
			"flw.ps f25, 704(x9)\n"
			"fmaxu.pi f26, f27, f25\n" 
			"maskpopc x7, m7\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f28, 512(x7)\n"
			"fsat8.pi f18, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 544(x15)\n"
			"flw.ps f21, 192(x15)\n"
			"fmin.s f18, f8, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 992(x14)\n"
			"flw.ps f21, 704(x14)\n"
			"flw.ps f10, 64(x14)\n"
			"fmadd.s f13, f24, f21, f10, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 704(x15)\n"
			"flw.ps f25, 832(x15)\n"
			"fxor.pi f18, f26, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 768(x13)\n"
			"flw.ps f0, 160(x13)\n"
			"fsgnjx.s f24, f28, f0\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_5:\n"
			"maskxor m7, m7, m3\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_int_32\n"
			"flw.ps f13, 736(x7)\n"
			"flw.ps f19, 992(x7)\n"
			"feq.pi f10, f13, f19\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_5:\n"
			"fltm.ps m3, f25, f24\n" 
			"li x5, 0x80340100\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 448(x13)\n"
			"flw.ps f26, 896(x13)\n"
			"fsgnj.ps f14, f15, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 768(x15)\n"
			"fcvt.pw.ps f24, f5, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 960(x9)\n"
			"fcvt.pwu.ps f5, f15, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 736(x14)\n"
			"flw.ps f24, 256(x14)\n"
			"fmul.s f18, f5, f24, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 704(x25)\n"
			"fsin.ps f18, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 896(x15)\n"
			"flw.ps f21, 512(x15)\n"
			"fle.pi f21, f18, f21\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_5:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 832(x9)\n"
			"frcp.ps f27, f1\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_5:\n"
			"feqm.ps m4, f0, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 256(x9)\n"
			"flw.ps f26, 0(x9)\n"
			"fmin.ps f5, f21, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 256(x26)\n"
			"flw.ps f19, 160(x26)\n"
			"feq.s x25, f27, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 800(x20)\n"
			"flw.ps f25, 352(x20)\n"
			"flw.ps f28, 448(x20)\n"
			"fmsub.ps f8, f18, f25, f28, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 608(x13)\n"
			"frsq.ps f24, f0\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_5:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 960(x13)\n"
			"frcp.ps f1, f5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 224(x20)\n"
			"fclass.ps f25, f25\n" 
			"mova.x.m x10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 96(x20)\n"
			"fsgnj.s f14, f14, f14\n" 
			"mova.x.m x10\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 608(x12)\n"
			"flw.ps f1, 480(x12)\n"
			"fsgnj.ps f8, f5, f1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 672(x10)\n"
			"flw.ps f24, 640(x10)\n"
			"fmin.s f15, f0, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 992(x22)\n"
			"fpackrepb.pi f25, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 384(x9)\n"
			"flw.ps f18, 96(x9)\n"
			"flt.ps f27, f24, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 448(x9)\n"
			"flw.ps f25, 768(x9)\n"
			"fmulhu.pi f13, f28, f25\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 800(x22)\n"
			"flw.ps f1, 352(x22)\n"
			"flt.pi f14, f24, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 896(x20)\n"
			"fcvt.wu.s x22, f19, rtz\n" 
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
			"fbci.ps f27, 0x61c44\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 288(x26)\n"
			"flw.ps f18, 992(x26)\n"
			"fmax.pi f0, f15, f18\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 576(x22)\n"
			"fcvt.w.s x10, f19, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 0(x14)\n"
			"flw.ps f18, 320(x14)\n"
			"fmin.pi f19, f21, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 384(x12)\n"
			"flw.ps f14, 928(x12)\n"
			"fmul.s f26, f27, f14, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 320(x20)\n"
			"flw.ps f14, 320(x20)\n"
			"fle.s x15, f25, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f8, 608(x7)\n"
			"flw.ps f18, 992(x7)\n"
			"fand.pi f26, f8, f18\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 704(x10)\n"
			"fcvt.ps.pw f13, f8, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 384(x10)\n"
			"flw.ps f0, 352(x10)\n"
			"fsgnjn.ps f13, f1, f0\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_5:\n"
			"mov.m.x m0, x7, 0x7a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 320(x15)\n"
			"flw.ps f8, 416(x15)\n"
			"flw.ps f21, 896(x15)\n"
			"fmadd.s f19, f14, f8, f21, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 320(x26)\n"
			"flw.ps f15, 384(x26)\n"
			"fsgnjx.ps f13, f21, f15\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 256(x15)\n"
			"flw.ps f13, 736(x15)\n"
			"fand.pi f13, f15, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 192(x25)\n"
			"flw.ps f10, 608(x25)\n"
			"fsgnjx.ps f19, f27, f10\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 544(x14)\n"
			"flw.ps f19, 384(x14)\n"
			"fsub.pi f15, f28, f19\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 640(x10)\n"
			"flw.ps f28, 224(x10)\n"
			"fadd.s f15, f21, f28, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"flw.ps f21, 608(x15)\n"
			"fcvt.pw.ps f21, f21, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 992(x26)\n"
			"flw.ps f1, 352(x26)\n"
			"fle.pi f6, f10, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 928(x27)\n"
			"fround.ps f6, f28, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 960(x17)\n"
			"fsat8.pi f11, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 224(x11)\n"
			"fclass.s x27, f21\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 384(x18)\n"
			"flw.ps f29, 896(x18)\n"
			"fmul.ps f10, f27, f29, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 864(x27)\n"
			"flw.ps f17, 480(x27)\n"
			"fle.ps f12, f8, f17\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 32(x11)\n"
			"flw.ps f31, 480(x11)\n"
			"fsgnj.s f27, f9, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 64(x20)\n"
			"fcvt.pw.ps f31, f14, rup\n" 
			"li x5, 0xf63b\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 320(x12)\n"
			"fcvt.w.s x15, f31, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 448(x17)\n"
			"flw.ps f1, 352(x17)\n"
			"frem.pi f4, f29, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 672(x17)\n"
			"fswizz.ps f29, f28, 0x1c\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 736(x27)\n"
			"flw.ps f14, 736(x27)\n"
			"flw.ps f4, 640(x27)\n"
			"fcmov.ps f8, f12, f14, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 320(x23)\n"
			"fandi.pi f17, f31, 0x7c\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 320(x14)\n"
			"flw.ps f1, 640(x14)\n"
			"flw.ps f12, 928(x14)\n"
			"fnmsub.s f1, f31, f1, f12, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 64(x26)\n"
			"flw.ps f28, 32(x26)\n"
			"flw.ps f27, 800(x26)\n"
			"fmsub.ps f31, f11, f28, f27, rne\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 480(x26)\n"
			"flw.ps f9, 224(x26)\n"
			"flw.ps f10, 160(x26)\n"
			"fnmsub.ps f17, f12, f9, f10, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 32(x12)\n"
			"flw.ps f8, 608(x12)\n"
			"flw.ps f17, 544(x12)\n"
			"fnmsub.s f9, f9, f8, f17, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 480(x18)\n"
			"faddi.pi f8, f11, 0xe5\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 288(x23)\n"
			"flw.ps f10, 320(x23)\n"
			"fmin.pi f11, f29, f10\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 864(x26)\n"
			"flw.ps f11, 640(x26)\n"
			"flt.pi f1, f1, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 256(x23)\n"
			"flw.ps f29, 576(x23)\n"
			"for.pi f17, f8, f29\n" 
			"fbci.pi f8, 0x111bd\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 320(x20)\n"
			"flw.ps f10, 512(x20)\n"
			"fle.pi f27, f23, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 288(x20)\n"
			"fcvt.w.s x14, f29, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 800(x15)\n"
			"flw.ps f28, 416(x15)\n"
			"flt.s x14, f14, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 32(x18)\n"
			"fswizz.ps f9, f6, 0x7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 64(x27)\n"
			"flw.ps f8, 800(x27)\n"
			"fmax.pi f10, f21, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 480(x12)\n"
			"ffrc.ps f11, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 608(x10)\n"
			"flw.ps f23, 736(x10)\n"
			"flw.ps f21, 0(x10)\n"
			"fcmov.ps f10, f6, f23, f21\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 672(x23)\n"
			"fsrli.pi f9, f11, 0x7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 96(x18)\n"
			"flw.ps f1, 896(x18)\n"
			"feq.s x11, f6, f1\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_88961_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_285278_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x23, 0x160000000307000b\n"
			"csrw tensor_reduce, x23\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x23 << TENSOR_IMA_SCP_LOC_B) | (0x4d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_6:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"li x17, 0x6f38a981756e9505\n"
			"fcvt.s.w f27, x17, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 384(x18)\n"
			"fcvt.wu.s x27, f6, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 96(x10)\n"
			"fnot.pi f1, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_0_HID_6:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5b1a6aaf7e\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_1_HID_6:\n"
			"fsetm.pi m2, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 512(x20)\n"
			"fclass.s x26, f11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 128(x18)\n"
			"fslli.pi f28, f21, 0x7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 64(x27)\n"
			"faddi.pi f29, f21, 0x1b7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 896(x20)\n"
			"fcvt.pwu.ps f4, f28, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 640(x12)\n"
			"flw.ps f6, 416(x12)\n"
			"feq.ps f11, f10, f6\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 608(x18)\n"
			"flw.ps f8, 128(x18)\n"
			"for.pi f27, f9, f8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 576(x12)\n"
			"flw.ps f8, 416(x12)\n"
			"fand.pi f9, f21, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 640(x23)\n"
			"flw.ps f9, 288(x23)\n"
			"flt.s x10, f1, f9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 544(x18)\n"
			"fclass.ps f1, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_6:\n"
			"maskxor m7, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f4, 544(x23)\n"
			"flw.ps f17, 64(x23)\n"
			"fadd.pi f29, f4, f17\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xa0c1\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_81320_HID_6:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x2b0e\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x55 << TENSOR_IMA_SCP_LOC_B) | (0x63 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_3_FP_TRANS_ID_3_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 256(x23)\n"
			"flog.ps f21, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_6:\n"
			"flem.ps m2, f23, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 288(x10)\n"
			"flw.ps f9, 384(x10)\n"
			"fsub.s f8, f23, f9, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 544(x18)\n"
			"fcvt.wu.s x27, f8, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 288(x20)\n"
			"fcvt.ps.pwu f17, f8, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_6:\n"
			"fltm.pi m0, f29, f6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x17\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 256(x12)\n"
			"flw.ps f28, 672(x12)\n"
			"fmul.pi f12, f27, f28\n" 
			"li x17, 0xf67166855408da33\n"
			"fcvt.s.wu f23, x17, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 768(x11)\n"
			"fround.ps f29, f27, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 864(x20)\n"
			"fcvt.w.s x20, f23, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 128(x23)\n"
			"flw.ps f4, 992(x23)\n"
			"fmul.ps f4, f28, f4, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 512(x15)\n"
			"fmul.ps f21, f8, f8, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 192(x15)\n"
			"flw.ps f17, 896(x15)\n"
			"fmul.s f14, f21, f17, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f29, 480(x27)\n"
			"fcvt.ps.pwu f29, f29, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 544(x26)\n"
			"fmulhu.pi f14, f11, f11\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_354343_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa1 << TENSOR_FMA16_SCP_LOC_B) | (0x5b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
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
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"maskpopc x17, m2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 320(x12)\n"
			"fltu.pi f1, f14, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 192(x11)\n"
			"flw.ps f21, 544(x11)\n"
			"fmin.pi f9, f4, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f12, 448(x12)\n"
			"flw.ps f11, 960(x12)\n"
			"frem.pi f27, f12, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 960(x23)\n"
			"flw.ps f8, 928(x23)\n"
			"fxor.pi f31, f4, f8\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 672(x15)\n"
			"flw.ps f21, 288(x15)\n"
			"fsub.s f31, f1, f21, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 192(x23)\n"
			"fround.ps f12, f31, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 768(x11)\n"
			"flw.ps f17, 256(x11)\n"
			"fsgnjn.ps f29, f4, f17\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_6:\n"
			"flem.ps m7, f29, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_6:\n"
			"masknot m0, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x87bd\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f17, 128(x23)\n"
			"fsat8.pi f27, f17\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_6:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 512(x26)\n"
			"flog.ps f6, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 640(x11)\n"
			"fsat8.pi f8, f31\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_6:\n"
			"feqm.ps m2, f29, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_6:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 288(x15)\n"
			"frcp.ps f23, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 864(x20)\n"
			"fcvt.f16.ps f1, f11\n" 
			"li x5, 0x8fcf\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_173565_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_746792_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_378635_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
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
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_6:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 384(x26)\n"
			"fexp.ps f1, f28\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 192(x20)\n"
			"flw.ps f27, 416(x20)\n"
			"for.pi f10, f10, f27\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_6:\n"
			"feqm.ps m5, f29, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 352(x11)\n"
			"fcvt.pwu.ps f10, f17, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 0(x26)\n"
			"fclass.ps f21, f27\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 672(x11)\n"
			"flw.ps f21, 448(x11)\n"
			"fmin.s f9, f9, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_int_32\n"
			"flw.ps f10, 960(x26)\n"
			"fandi.pi f31, f10, 0x65\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_6:\n"
			"flem.ps m5, f8, f10\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 960(x23)\n"
			"fround.ps f17, f28, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 704(x14)\n"
			"flw.ps f1, 0(x14)\n"
			"fle.ps f28, f31, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 32(x15)\n"
			"flw.ps f11, 224(x15)\n"
			"fmul.s f29, f8, f11, rtz\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_6:\n"
			"mov.m.x m2, x27, 0x35\n" 
			"li x5, 0x803401E8\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 320(x11)\n"
			"flw.ps f14, 64(x11)\n"
			"fsgnjx.ps f9, f11, f14\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 64(x15)\n"
			"flw.ps f10, 928(x15)\n"
			"fminu.pi f9, f31, f10\n" 
			"li x5, 0xf930e73d17453f4e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd479c1dccfdff2d1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_6:\n"
			"fsetm.pi m0, f10\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 896(x17)\n"
			"flw.ps f27, 128(x17)\n"
			"feq.ps f8, f31, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xde << TENSOR_FMA16_SCP_LOC_B) | (0xc4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xf3 << TENSOR_IMA_SCP_LOC_B) | (0xbf << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_2_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf6 << TENSOR_FMA16_SCP_LOC_B) | (0x4b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 480(x15)\n"
			"fexp.ps f27, f27\n" 
			"fbci.pi f6, 0x2dcdb\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 512(x23)\n"
			"flw.ps f15, 928(x23)\n"
			"flt.ps f16, f0, f15\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 0(x25)\n"
			"flw.ps f28, 0(x25)\n"
			"fmulh.pi f27, f2, f28\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 992(x27)\n"
			"flw.ps f24, 704(x27)\n"
			"frem.pi f28, f12, f24\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 928(x27)\n"
			"flw.ps f24, 352(x27)\n"
			"fadd.pi f20, f19, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 864(x10)\n"
			"fcvt.f16.ps f20, f6\n" 
			"li x23, 0xddf9ccf54274b1\n"
			"fcvt.s.lu f9, x23, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f24, 32(x27)\n"
			"flw.ps f16, 800(x27)\n"
			"fltu.pi f3, f24, f16\n" 
			"la x10, rand_int_32\n"
			"flw.ps f24, 608(x10)\n"
			"fsrli.pi f28, f24, 0x8\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 416(x16)\n"
			"fsrai.pi f6, f2, 0x4\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 832(x23)\n"
			"flw.ps f2, 672(x23)\n"
			"fsgnjn.s f15, f24, f2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 32(x19)\n"
			"flw.ps f27, 640(x19)\n"
			"fmulh.pi f15, f28, f27\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 704(x11)\n"
			"fandi.pi f19, f16, 0x70\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 64(x11)\n"
			"flw.ps f0, 256(x11)\n"
			"fmin.s f9, f16, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"maskxor m2, m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f19, 608(x16)\n"
			"flw.ps f20, 352(x16)\n"
			"fmaxu.pi f0, f19, f20\n" 
			"li x14, 0xb17a0c6b597e6751\n"
			"fcvt.s.wu f28, x14, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 256(x16)\n"
			"fswizz.ps f15, f19, 0x16\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_7:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 384(x23)\n"
			"fexp.ps f9, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 608(x19)\n"
			"fcvt.ps.pwu f8, f19, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 896(x15)\n"
			"flw.ps f20, 288(x15)\n"
			"feq.pi f6, f8, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 896(x28)\n"
			"flw.ps f28, 0(x28)\n"
			"fsgnj.s f12, f3, f28\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 864(x14)\n"
			"flw.ps f24, 544(x14)\n"
			"flw.ps f19, 640(x14)\n"
			"fnmsub.ps f12, f3, f24, f19, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 64(x15)\n"
			"flw.ps f15, 992(x15)\n"
			"fsgnjx.ps f3, f27, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 352(x10)\n"
			"fpackrepb.pi f0, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 288(x14)\n"
			"ffrc.ps f9, f24\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 64(x10)\n"
			"fsrli.pi f3, f23, 0x7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 640(x23)\n"
			"flw.ps f8, 384(x23)\n"
			"feq.ps f2, f12, f8\n" 
			"mova.x.m x20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 640(x11)\n"
			"flw.ps f20, 288(x11)\n"
			"fsub.pi f0, f9, f20\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 768(x11)\n"
			"flw.ps f15, 160(x11)\n"
			"flw.ps f30, 0(x11)\n"
			"fmsub.s f23, f9, f15, f30, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 576(x28)\n"
			"fcvt.f16.ps f9, f6\n" 
			"maskpopcz x23, m7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 224(x10)\n"
			"flw.ps f30, 480(x10)\n"
			"flw.ps f27, 960(x10)\n"
			"fnmsub.ps f20, f16, f30, f27, rtz\n" 
			"li x27, 0xe156d1e38dbeaa8f\n"
			"fcvt.s.wu f2, x27, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 768(x19)\n"
			"flw.ps f15, 0(x19)\n"
			"feq.ps f15, f27, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 352(x28)\n"
			"fandi.pi f23, f16, 0x178\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 640(x27)\n"
			"flw.ps f19, 864(x27)\n"
			"fsgnjn.s f3, f27, f19\n" 
			"li x25, 0x39d7a9dad106664a\n"
			"fcvt.s.w f2, x25, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_7:\n"
			"flem.ps m6, f28, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_7:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 768(x20)\n"
			"flog.ps f15, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 512(x23)\n"
			"ffrc.ps f16, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 64(x19)\n"
			"fcvt.w.s x19, f2, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 0(x25)\n"
			"fcvt.w.s x19, f9, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 608(x11)\n"
			"flw.ps f12, 768(x11)\n"
			"feq.pi f24, f28, f12\n" 
			"mova.x.m x20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 32(x10)\n"
			"fcvt.lu.s x10, f3, rdn\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_7:\n"
			"maskxor m7, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 736(x28)\n"
			"fswizz.ps f15, f30, 0x6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 256(x20)\n"
			"fswizz.ps f12, f16, 0x8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 64(x10)\n"
			"flw.ps f20, 960(x10)\n"
			"fmax.s f16, f9, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 384(x27)\n"
			"flw.ps f6, 416(x27)\n"
			"for.pi f9, f19, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 0(x11)\n"
			"flw.ps f28, 768(x11)\n"
			"fmin.s f6, f15, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 992(x28)\n"
			"fcvt.f16.ps f3, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 512(x19)\n"
			"flw.ps f0, 736(x19)\n"
			"flw.ps f12, 576(x19)\n"
			"fnmadd.ps f19, f28, f0, f12, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 96(x25)\n"
			"fandi.pi f2, f3, 0xd8\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 288(x16)\n"
			"flw.ps f9, 160(x16)\n"
			"fxor.pi f8, f2, f9\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 320(x25)\n"
			"flw.ps f28, 32(x25)\n"
			"fcmovm.ps f3, f19, f28\n" 
			"fbci.ps f20, 0x2a3d0\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 192(x15)\n"
			"flw.ps f9, 512(x15)\n"
			"fmulhu.pi f20, f28, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 800(x19)\n"
			"flw.ps f20, 352(x19)\n"
			"fmax.pi f28, f30, f20\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 416(x19)\n"
			"flw.ps f15, 288(x19)\n"
			"fsgnj.ps f0, f0, f15\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 416(x15)\n"
			"flw.ps f23, 448(x15)\n"
			"fadd.pi f20, f9, f23\n" 
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
			"li x28, 0xebcb0b16f4b8876b\n"
			"fcvt.s.w f23, x28, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f23, 576(x16)\n"
			"fpackrepb.pi f0, f23\n" 
			"fbci.pi f24, 0x29729\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 128(x14)\n"
			"flw.ps f16, 320(x14)\n"
			"fsgnjn.ps f12, f9, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f16, 160(x16)\n"
			"flw.ps f28, 352(x16)\n"
			"feq.pi f6, f16, f28\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 736(x28)\n"
			"flw.ps f20, 192(x28)\n"
			"fltu.pi f3, f9, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 448(x16)\n"
			"fclass.s x27, f12\n" 
			"maskpopcz x28, m6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 928(x25)\n"
			"fmax.s f15, f16, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 672(x15)\n"
			"flw.ps f30, 928(x15)\n"
			"fadd.s f30, f2, f30, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f12, 672(x16)\n"
			"faddi.pi f15, f12, 0x1d4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 0(x16)\n"
			"fcvt.ps.pw f6, f27, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 96(x23)\n"
			"flw.ps f12, 864(x23)\n"
			"fsgnjx.ps f6, f19, f12\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 64(x27)\n"
			"flw.ps f8, 448(x27)\n"
			"fmax.pi f3, f12, f8\n" 
			"li x11, 0x80577c2aa5995576\n"
			"fcvt.s.w f15, x11, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 672(x25)\n"
			"flw.ps f30, 864(x25)\n"
			"fmin.ps f8, f27, f30\n" 
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
			"masknot m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 928(x20)\n"
			"flw.ps f30, 640(x20)\n"
			"fsub.ps f15, f0, f30, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 672(x11)\n"
			"flw.ps f16, 192(x11)\n"
			"fle.ps f12, f30, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 32(x14)\n"
			"flw.ps f27, 576(x14)\n"
			"fmsub.s f2, f0, f27, f27, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 896(x28)\n"
			"flw.ps f3, 0(x28)\n"
			"flw.ps f27, 672(x28)\n"
			"fmadd.s f27, f9, f3, f27, rmm\n" 
			"fbci.ps f19, 0x5129a\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_7:\n"
			"maskand m2, m2, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x85a5c4df2c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_7:\n"
			"flem.ps m2, f2, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 96(x20)\n"
			"flw.ps f6, 512(x20)\n"
			"flw.ps f23, 160(x20)\n"
			"fmsub.ps f6, f2, f6, f23, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 96(x23)\n"
			"fclass.s x20, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 512(x28)\n"
			"flw.ps f19, 512(x28)\n"
			"fsrl.pi f8, f15, f19\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 544(x11)\n"
			"flw.ps f28, 64(x11)\n"
			"fminu.pi f20, f3, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 544(x25)\n"
			"flw.ps f2, 288(x25)\n"
			"fmul.ps f20, f27, f2, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 64(x10)\n"
			"fswizz.ps f20, f24, 0x4\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_7:\n"
			"fltm.ps m7, f8, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 576(x28)\n"
			"flw.ps f6, 416(x28)\n"
			"fmax.ps f9, f3, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 32(x10)\n"
			"flw.ps f5, 384(x10)\n"
			"fmin.ps f22, f8, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 448(x11)\n"
			"fand.pi f14, f27, f27\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 288(x7)\n"
			"flw.ps f3, 32(x7)\n"
			"fmul.pi f19, f18, f3\n" 
			"li x5, 0xdab6\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f25, 736(x17)\n"
			"faddi.pi f22, f25, 0x1aa\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 768(x10)\n"
			"flw.ps f18, 928(x10)\n"
			"fadd.s f21, f17, f18, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 32(x11)\n"
			"flw.ps f3, 736(x11)\n"
			"flt.ps f28, f7, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 416(x11)\n"
			"fmulh.pi f14, f25, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"maskor m4, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 128(x13)\n"
			"flw.ps f7, 544(x13)\n"
			"fle.ps f21, f21, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 288(x17)\n"
			"fpackrepb.pi f14, f27\n" 
			"fbci.ps f22, 0x5a396\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 416(x5)\n"
			"flw.ps f4, 640(x5)\n"
			"fsgnjn.ps f5, f18, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 832(x12)\n"
			"flw.ps f21, 96(x12)\n"
			"fmulhu.pi f24, f27, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 384(x17)\n"
			"frsq.ps f22, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 128(x12)\n"
			"flw.ps f28, 704(x12)\n"
			"flw.ps f14, 544(x12)\n"
			"fmadd.ps f8, f8, f28, f14, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 96(x11)\n"
			"flw.ps f31, 416(x11)\n"
			"fcmovm.ps f4, f19, f31\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0xb6e6\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 160(x13)\n"
			"frsq.ps f3, f24\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 96(x20)\n"
			"flw.ps f27, 352(x20)\n"
			"flw.ps f18, 384(x20)\n"
			"fmsub.s f5, f31, f27, f18, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 896(x10)\n"
			"flw.ps f19, 384(x10)\n"
			"fadd.ps f28, f3, f19, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 992(x11)\n"
			"flw.ps f24, 448(x11)\n"
			"fsll.pi f28, f5, f24\n" 
			"la x7, rand_int_32\n"
			"flw.ps f3, 896(x7)\n"
			"flw.ps f27, 448(x7)\n"
			"fminu.pi f24, f3, f27\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 896(x10)\n"
			"fandi.pi f3, f3, 0xee\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 640(x13)\n"
			"fcvt.ps.pw f21, f22, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 896(x10)\n"
			"fsqrt.s f5, f24, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 32(x12)\n"
			"flw.ps f21, 160(x12)\n"
			"fadd.ps f14, f8, f21, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_8:\n"
			"fltm.pi m5, f21, f8\n" 
			"li x5, 0x80340120\n"
			"ld x5, 0(x5)\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 320(x12)\n"
			"flw.ps f24, 608(x12)\n"
			"flw.ps f25, 832(x12)\n"
			"fmadd.s f5, f21, f24, f25, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"maskor m4, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 224(x22)\n"
			"fclass.s x20, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 672(x20)\n"
			"flw.ps f31, 160(x20)\n"
			"fsra.pi f5, f25, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 192(x7)\n"
			"flw.ps f22, 640(x7)\n"
			"flw.ps f14, 512(x7)\n"
			"fnmadd.s f19, f31, f22, f14, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 800(x10)\n"
			"flw.ps f19, 64(x10)\n"
			"fltu.pi f31, f27, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_23608_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x43 << TENSOR_IMA_SCP_LOC_B) | (0x56 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x22, 0x160000000307000b\n"
			"csrw tensor_reduce, x22\n" 
			"csrwi tensor_wait, 9\n" 
			"li x5, 0xd940f9930445f80e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8796cb2a7101a328\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_58789_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
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
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xf8 << TENSOR_FMA32_SCP_LOC_B) | (0x79 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f18, 480(x13)\n"
			"fpackrepb.pi f8, f18\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 512(x23)\n"
			"flw.ps f24, 448(x23)\n"
			"flt.ps f3, f19, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 0(x17)\n"
			"flw.ps f7, 320(x17)\n"
			"fsub.s f24, f18, f7, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 128(x22)\n"
			"flw.ps f21, 32(x22)\n"
			"fmaxu.pi f22, f4, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 128(x22)\n"
			"flw.ps f14, 928(x22)\n"
			"fmin.s f18, f22, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 672(x12)\n"
			"flw.ps f7, 672(x12)\n"
			"flw.ps f31, 992(x12)\n"
			"fnmadd.ps f7, f22, f7, f31, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 192(x12)\n"
			"flw.ps f21, 96(x12)\n"
			"flt.pi f28, f3, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 352(x17)\n"
			"fpackrepb.pi f25, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 128(x12)\n"
			"fsrli.pi f17, f3, 0xf\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 416(x7)\n"
			"flw.ps f8, 672(x7)\n"
			"flw.ps f5, 576(x7)\n"
			"fmadd.s f18, f22, f8, f5, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 384(x23)\n"
			"fround.ps f31, f14, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 608(x7)\n"
			"flw.ps f24, 352(x7)\n"
			"fnmadd.ps f18, f22, f24, f24, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 768(x7)\n"
			"flw.ps f31, 864(x7)\n"
			"flw.ps f27, 544(x7)\n"
			"fnmadd.s f8, f14, f31, f27, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 288(x17)\n"
			"fcvt.ps.pw f17, f28, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 96(x11)\n"
			"flw.ps f14, 512(x11)\n"
			"fcmovm.ps f3, f17, f14\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 832(x13)\n"
			"fround.ps f25, f27, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe4 << TENSOR_FMA16_SCP_LOC_B) | (0xaf << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 864(x7)\n"
			"flw.ps f21, 64(x7)\n"
			"flt.ps f7, f7, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 160(x17)\n"
			"flw.ps f24, 448(x17)\n"
			"feq.s x7, f28, f24\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 608(x28)\n"
			"flw.ps f7, 448(x28)\n"
			"flw.ps f27, 896(x28)\n"
			"fnmadd.s f7, f5, f7, f27, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 992(x13)\n"
			"flw.ps f27, 128(x13)\n"
			"flw.ps f18, 128(x13)\n"
			"fnmadd.ps f31, f25, f27, f18, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 416(x10)\n"
			"flw.ps f28, 320(x10)\n"
			"fle.pi f5, f4, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 192(x17)\n"
			"fswizz.ps f19, f28, 0x4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 672(x11)\n"
			"fsin.ps f17, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 736(x5)\n"
			"flw.ps f7, 768(x5)\n"
			"flw.ps f24, 288(x5)\n"
			"fmsub.s f22, f25, f7, f24, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_8:\n"
			"maskand m4, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 640(x5)\n"
			"fsub.ps f18, f5, f5, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 192(x13)\n"
			"fltu.pi f18, f22, f22\n" 
			"li x5, 0x30f3cb61e04b0020\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6848d12463659d86\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f8, 192(x13)\n"
			"fsat8.pi f31, f8\n" 
			"li x5, 0x8ee1\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 512(x7)\n"
			"fclass.ps f19, f17\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 992(x7)\n"
			"flw.ps f31, 672(x7)\n"
			"fmul.pi f19, f28, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 0(x17)\n"
			"flw.ps f19, 256(x17)\n"
			"flw.ps f22, 768(x17)\n"
			"fmadd.s f17, f17, f19, f22, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 288(x11)\n"
			"flw.ps f18, 736(x11)\n"
			"fle.ps f3, f14, f18\n" 
			"li x5, 0x6e8e\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_4_FP_TRANS_ID_4_HID_8:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 960(x11)\n"
			"fexp.ps f25, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 832(x11)\n"
			"flw.ps f18, 160(x11)\n"
			"fmadd.ps f28, f17, f17, f18, rup\n" 
			"fbci.ps f25, 0x14844\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 640(x28)\n"
			"flw.ps f27, 704(x28)\n"
			"flt.ps f8, f18, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 416(x12)\n"
			"fsat8.pi f27, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 736(x12)\n"
			"fpackreph.pi f7, f14\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 960(x10)\n"
			"flw.ps f22, 928(x10)\n"
			"feq.pi f22, f19, f22\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 928(x7)\n"
			"fpackreph.pi f8, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 416(x13)\n"
			"flw.ps f18, 160(x13)\n"
			"flw.ps f5, 96(x13)\n"
			"fmadd.ps f7, f19, f18, f5, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 832(x13)\n"
			"fcvt.pwu.ps f8, f17, rtz\n" 
			"li x10, 0x65e557cd10ce5cdb\n"
			"fcvt.s.w f4, x10, rmm\n" 
			"fbci.ps f21, 0x3c5cb\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 736(x23)\n"
			"fandi.pi f21, f4, 0x10f\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 160(x20)\n"
			"fsatu8.pi f24, f31\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 384(x11)\n"
			"flw.ps f14, 832(x11)\n"
			"fsub.pi f19, f21, f14\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_8:\n"
			"fsetm.pi m5, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x70 << TENSOR_FMA32_SCP_LOC_B) | (0xfb << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 864(x28)\n"
			"frsq.ps f24, f19\n" 
			"li x5, 0xec198189c192e626\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x1c4aa1798eba14ce\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 576(x7)\n"
			"flw.ps f21, 640(x7)\n"
			"fsgnjx.ps f8, f8, f21\n" 
			"li x5, 0x96cb\n"
			"csrw tensor_mask, x5\n"	
			"li x13, 0xf71d490c19517626\n"
			"fcvt.s.l f24, x13, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 32(x11)\n"
			"faddi.pi f5, f31, 0x1b2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 512(x12)\n"
			"fcvt.w.s x22, f5, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 544(x28)\n"
			"flw.ps f18, 864(x28)\n"
			"feq.ps f27, f14, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 736(x12)\n"
			"flw.ps f27, 64(x12)\n"
			"flt.ps f27, f4, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 736(x13)\n"
			"flw.ps f31, 192(x13)\n"
			"fsgnjn.s f7, f22, f31\n" 
			"li x5, 0x15f5bac7130a12ae\n"
			"fcvt.s.wu f21, x5, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 256(x7)\n"
			"flw.ps f27, 320(x7)\n"
			"fmax.pi f31, f21, f27\n" 
			"LBL_SEQID_5_M0_WRITE_ID_6_HID_8:\n"
			"maskxor m6, m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f24, 832(x5)\n"
			"flw.ps f18, 128(x5)\n"
			"fxor.pi f28, f24, f18\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_8:\n"
			"flem.ps m6, f24, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f22, 352(x17)\n"
			"flw.ps f28, 192(x17)\n"
			"fmul.pi f7, f22, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 832(x13)\n"
			"flw.ps f3, 160(x13)\n"
			"fsgnjn.s f27, f22, f3\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_8:\n"
			"fltm.ps m5, f28, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_293852_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x2c\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbd << TENSOR_IMA_SCP_LOC_B) | (0xf0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb5 << TENSOR_FMA32_SCP_LOC_B) | (0x70 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
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
			"LBL_KERNEL_HID_9:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_9\n"
			"li x5, 0x6\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f17, 64(x7)\n"
			"flw.ps f13, 672(x7)\n"
			"fdiv.pi f23, f17, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 256(x23)\n"
			"flw.ps f10, 832(x23)\n"
			"fmulh.pi f29, f11, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f25, 768(x23)\n"
			"fandi.pi f31, f25, 0x184\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 832(x7)\n"
			"fcvt.lu.s x22, f23, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 448(x20)\n"
			"flw.ps f29, 608(x20)\n"
			"fmin.s f14, f2, f29\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 480(x23)\n"
			"flw.ps f4, 672(x23)\n"
			"fsrl.pi f5, f5, f4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 704(x20)\n"
			"flw.ps f10, 352(x20)\n"
			"fmul.pi f5, f17, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 160(x20)\n"
			"fclass.s x27, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 352(x27)\n"
			"flw.ps f14, 896(x27)\n"
			"fmul.ps f25, f15, f14, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 256(x17)\n"
			"fround.ps f11, f13, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 352(x7)\n"
			"flw.ps f5, 992(x7)\n"
			"fsub.ps f3, f23, f5, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 608(x20)\n"
			"frsq.ps f4, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskor m2, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 352(x20)\n"
			"flw.ps f23, 0(x20)\n"
			"fmax.ps f13, f14, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"maskxor m0, m7, m3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f23, 864(x17)\n"
			"flw.ps f29, 416(x17)\n"
			"fmul.pi f2, f23, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f5, 32(x27)\n"
			"fslli.pi f13, f5, 0x0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 480(x28)\n"
			"flw.ps f5, 608(x28)\n"
			"fmsub.ps f13, f15, f5, f5, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 704(x20)\n"
			"flw.ps f29, 256(x20)\n"
			"fadd.pi f14, f31, f29\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 320(x7)\n"
			"fcvt.ps.pwu f17, f31, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 704(x23)\n"
			"fpackreph.pi f17, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 640(x26)\n"
			"flw.ps f23, 960(x26)\n"
			"fmul.s f13, f26, f23, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"maskor m7, m3, m7\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 640(x28)\n"
			"flw.ps f17, 928(x28)\n"
			"fsgnjn.ps f10, f3, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"feqm.ps m2, f9, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f3, 32(x17)\n"
			"flw.ps f29, 576(x17)\n"
			"fmin.pi f13, f3, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 192(x27)\n"
			"flw.ps f9, 608(x27)\n"
			"flw.ps f3, 384(x27)\n"
			"fnmsub.ps f26, f17, f9, f3, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 320(x26)\n"
			"fandi.pi f15, f4, 0x2f\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 608(x22)\n"
			"fcvt.lu.s x20, f31, rtz\n" 
			"mova.x.m x9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 864(x26)\n"
			"ffrc.ps f31, f15\n" 
			"maskpopc x7, m2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_9:\n"
			"fltm.pi m3, f5, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 96(x7)\n"
			"flw.ps f29, 864(x7)\n"
			"fle.s x7, f13, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_9:\n"
			"maskxor m0, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f3, 128(x23)\n"
			"flw.ps f31, 992(x23)\n"
			"fand.pi f15, f3, f31\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f11, 384(x26)\n"
			"fcvt.ps.f16 f5, f11\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_9:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 576(x26)\n"
			"fexp.ps f9, f23\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 608(x20)\n"
			"flw.ps f14, 896(x20)\n"
			"flw.ps f13, 288(x20)\n"
			"fnmsub.ps f9, f4, f14, f13, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 64(x22)\n"
			"flw.ps f13, 128(x22)\n"
			"fsub.ps f13, f4, f13, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"fsetm.pi m0, f2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x330b71c5c7\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 320(x28)\n"
			"flw.ps f25, 576(x28)\n"
			"fmax.s f15, f14, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 448(x20)\n"
			"flw.ps f26, 672(x20)\n"
			"fsgnjn.s f29, f15, f26\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 832(x27)\n"
			"flw.ps f10, 480(x27)\n"
			"fmul.ps f26, f26, f10, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_9:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f10, 0x2c77c\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 832(x19)\n"
			"flw.ps f23, 576(x19)\n"
			"fsgnj.s f26, f4, f23\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 608(x22)\n"
			"flw.ps f3, 544(x22)\n"
			"flt.pi f15, f15, f3\n" 
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 544(x17)\n"
			"flw.ps f25, 416(x17)\n"
			"fnmadd.s f29, f31, f25, f25, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_9:\n"
			"fsetm.pi m3, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_9:\n"
			"masknot m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 192(x22)\n"
			"flw.ps f13, 32(x22)\n"
			"flt.ps f2, f26, f13\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 864(x19)\n"
			"flw.ps f29, 384(x19)\n"
			"flw.ps f17, 992(x19)\n"
			"fnmadd.s f3, f25, f29, f17, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_9:\n"
			"feqm.ps m7, f10, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 544(x9)\n"
			"fcvt.wu.s x7, f17, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_9:\n"
			"mov.m.x m3, x27, 0x30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x28, 0xadd952a5c6ee2ebe\n"
			"fcvt.s.w f4, x28, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 768(x23)\n"
			"fclass.s x19, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 288(x28)\n"
			"flw.ps f3, 128(x28)\n"
			"fsgnjn.s f2, f17, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 736(x27)\n"
			"flw.ps f25, 992(x27)\n"
			"fsgnj.s f10, f4, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 352(x17)\n"
			"flw.ps f3, 160(x17)\n"
			"fcmovm.ps f11, f26, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 992(x22)\n"
			"fcvt.wu.s x23, f3, rtz\n" 
			"LBL_SEQID_3_FP_TRANS_ID_13_HID_9:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 480(x7)\n"
			"frcp.ps f23, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 32(x22)\n"
			"ffrc.ps f29, f31\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 544(x19)\n"
			"flw.ps f26, 480(x19)\n"
			"flw.ps f9, 160(x19)\n"
			"fnmadd.ps f13, f5, f26, f9, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 288(x28)\n"
			"fcvt.ps.pw f2, f15, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 128(x22)\n"
			"flw.ps f4, 704(x22)\n"
			"fle.ps f29, f2, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 64(x23)\n"
			"fnot.pi f29, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 448(x26)\n"
			"fnot.pi f17, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f15, 320(x9)\n"
			"flw.ps f3, 992(x9)\n"
			"fsra.pi f2, f15, f3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 608(x28)\n"
			"fxor.pi f10, f4, f4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 512(x20)\n"
			"flw.ps f10, 32(x20)\n"
			"fmul.pi f25, f31, f10\n" 
			"mova.x.m x28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 480(x9)\n"
			"flw.ps f9, 448(x9)\n"
			"flw.ps f26, 416(x9)\n"
			"fmadd.s f5, f5, f9, f26, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 672(x7)\n"
			"flw.ps f3, 352(x7)\n"
			"fltu.pi f4, f26, f3\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_9:\n"
			"maskxor m0, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x22, m3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 192(x23)\n"
			"flw.ps f17, 0(x23)\n"
			"fadd.s f9, f10, f17, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 992(x9)\n"
			"fpackrepb.pi f10, f10\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 480(x27)\n"
			"flw.ps f3, 768(x27)\n"
			"flt.pi f9, f15, f3\n" 
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 800(x27)\n"
			"flw.ps f13, 512(x27)\n"
			"flt.s x26, f2, f13\n" 
			"fbci.ps f15, 0x7d90b\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 320(x9)\n"
			"flw.ps f3, 192(x9)\n"
			"fxor.pi f2, f4, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 192(x17)\n"
			"flw.ps f9, 256(x17)\n"
			"fmulhu.pi f11, f25, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 128(x9)\n"
			"fcvt.pw.ps f3, f15, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 928(x28)\n"
			"flw.ps f15, 928(x28)\n"
			"flw.ps f2, 160(x28)\n"
			"fcmov.ps f14, f17, f15, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 928(x26)\n"
			"flw.ps f23, 640(x26)\n"
			"for.pi f11, f13, f23\n" 
			"li x23, 0x9e202f160390e3e\n"
			"fcvt.s.l f23, x23, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 576(x17)\n"
			"fcvt.w.s x19, f3, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 672(x26)\n"
			"flw.ps f11, 928(x26)\n"
			"fmax.pi f31, f9, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 384(x28)\n"
			"flw.ps f31, 32(x28)\n"
			"fadd.ps f23, f9, f31, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 736(x26)\n"
			"fswizz.ps f26, f5, 0xf\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 544(x27)\n"
			"flw.ps f14, 224(x27)\n"
			"feq.ps f29, f15, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 0(x26)\n"
			"flw.ps f26, 512(x26)\n"
			"fsgnj.ps f10, f2, f26\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 384(x28)\n"
			"flw.ps f23, 192(x28)\n"
			"fadd.pi f9, f29, f23\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 384(x27)\n"
			"flw.ps f31, 416(x27)\n"
			"fle.pi f4, f23, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f28, 672(x7)\n"
			"flw.ps f3, 256(x7)\n"
			"fsra.pi f25, f28, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 480(x26)\n"
			"flw.ps f28, 544(x26)\n"
			"fcmovm.ps f5, f27, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"masknot m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 896(x26)\n"
			"flw.ps f24, 672(x26)\n"
			"fle.s x18, f13, f24\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 928(x13)\n"
			"flw.ps f25, 960(x13)\n"
			"feq.pi f22, f14, f25\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 576(x17)\n"
			"flw.ps f13, 192(x17)\n"
			"fle.pi f28, f31, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 416(x26)\n"
			"flw.ps f29, 288(x26)\n"
			"flw.ps f14, 96(x26)\n"
			"fmadd.s f14, f28, f29, f14, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 416(x22)\n"
			"flw.ps f2, 192(x22)\n"
			"fmul.ps f2, f22, f2, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 544(x18)\n"
			"fpackreph.pi f16, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 192(x17)\n"
			"flw.ps f31, 64(x17)\n"
			"fle.s x22, f16, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 64(x10)\n"
			"fcvt.f16.ps f4, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 736(x17)\n"
			"flw.ps f31, 928(x17)\n"
			"feq.ps f9, f16, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 0(x11)\n"
			"flw.ps f31, 512(x11)\n"
			"fmin.ps f31, f27, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 128(x15)\n"
			"flw.ps f22, 128(x15)\n"
			"fmin.ps f13, f3, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 288(x5)\n"
			"flog.ps f3, f9\n" 
			"li x5, 0x2b1a\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_10:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 416(x5)\n"
			"flog.ps f29, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x7, 0x200000000760000\n"
			"csrr x9, mhartid\n"
			"srli x9, x9, 4\n"
			"slli x9, x9, 6\n"
			"add x7, x7, x9\n"
			"csrw tensor_reduce, x7\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xf\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"fltm.pi m6, f4, f2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xdc2bb5b618\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x18, m6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fbci.ps f3, 0x374a2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 352(x26)\n"
			"flw.ps f3, 704(x26)\n"
			"flw.ps f14, 224(x26)\n"
			"fmadd.s f25, f21, f3, f14, rmm\n" 
			"li x22, 0x9b063476145453cd\n"
			"fcvt.s.wu f21, x22, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"maskor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x1f336ec754\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 992(x15)\n"
			"fclass.s x26, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 672(x18)\n"
			"flw.ps f24, 192(x18)\n"
			"fsgnjn.s f4, f13, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_10:\n"
			"feqm.ps m4, f27, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 224(x10)\n"
			"flw.ps f24, 672(x10)\n"
			"flw.ps f5, 0(x10)\n"
			"fmadd.s f24, f9, f24, f5, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_10:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 928(x22)\n"
			"flog.ps f4, f24\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 704(x18)\n"
			"fcvt.ps.pw f4, f2, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f3, 224(x10)\n"
			"fsatu8.pi f13, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_10:\n"
			"maskand m2, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9b84e28c38\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 640(x15)\n"
			"fcvt.pwu.ps f16, f31, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 0(x5)\n"
			"flw.ps f21, 832(x5)\n"
			"fdiv.ps f16, f22, f21, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x17, 0x160000000307000b\n"
			"csrw tensor_reduce, x17\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 352(x22)\n"
			"flw.ps f13, 960(x22)\n"
			"flw.ps f14, 64(x22)\n"
			"fnmadd.ps f27, f28, f13, f14, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f13, 512(x15)\n"
			"flw.ps f3, 352(x15)\n"
			"fsub.pi f28, f13, f3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f31, 896(x26)\n"
			"fpackreph.pi f21, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 704(x17)\n"
			"flw.ps f31, 704(x17)\n"
			"fsll.pi f4, f5, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f16, 960(x17)\n"
			"flw.ps f22, 0(x17)\n"
			"fmulhu.pi f25, f16, f22\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_10:\n"
			"mov.m.x m0, x7, 0x5d\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f21, 0(x5)\n"
			"flw.ps f22, 224(x5)\n"
			"fminu.pi f25, f21, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 608(x17)\n"
			"fcvt.wu.s x5, f28, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 960(x5)\n"
			"flw.ps f28, 448(x5)\n"
			"fminu.pi f22, f9, f28\n" 
			"li x10, 0x31c27e7950c7168c\n"
			"fcvt.s.w f25, x10, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 992(x17)\n"
			"flw.ps f25, 32(x17)\n"
			"fsra.pi f24, f31, f25\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 192(x11)\n"
			"flw.ps f21, 320(x11)\n"
			"fsgnj.s f27, f3, f21\n" 
			"fbci.ps f4, 0x40901\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 352(x22)\n"
			"flw.ps f16, 0(x22)\n"
			"fle.ps f4, f14, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_int_32\n"
			"flw.ps f2, 384(x18)\n"
			"fslli.pi f14, f2, 0x4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 640(x17)\n"
			"flw.ps f29, 384(x17)\n"
			"flw.ps f2, 992(x17)\n"
			"fmsub.ps f4, f13, f29, f2, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f28, 192(x17)\n"
			"flw.ps f21, 384(x17)\n"
			"frem.pi f13, f28, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 864(x17)\n"
			"flw.ps f28, 576(x17)\n"
			"fmax.pi f2, f27, f28\n" 
			"li x13, 0x5abc67cb638cd4bd\n"
			"fcvt.s.wu f29, x13, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 640(x18)\n"
			"fnot.pi f21, f14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 704(x11)\n"
			"flw.ps f16, 288(x11)\n"
			"flw.ps f25, 32(x11)\n"
			"fcmov.ps f3, f31, f16, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_10:\n"
			"mov.m.x m4, x18, 0x29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 672(x17)\n"
			"fcvt.wu.s x7, f24, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 128(x15)\n"
			"flw.ps f5, 544(x15)\n"
			"fsgnjn.s f13, f21, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f13, 928(x17)\n"
			"fsatu8.pi f21, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_int_32\n"
			"flw.ps f4, 576(x15)\n"
			"flw.ps f5, 576(x15)\n"
			"fmin.pi f28, f4, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 896(x11)\n"
			"flw.ps f25, 256(x11)\n"
			"fsub.s f24, f29, f25, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 448(x13)\n"
			"flw.ps f24, 224(x13)\n"
			"fsll.pi f28, f2, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 640(x13)\n"
			"flw.ps f3, 704(x13)\n"
			"fmin.s f2, f22, f3\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 576(x18)\n"
			"flw.ps f31, 192(x18)\n"
			"flt.ps f5, f22, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 416(x17)\n"
			"flw.ps f9, 832(x17)\n"
			"fsgnjx.s f13, f31, f9\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 128(x17)\n"
			"fsub.pi f28, f5, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 416(x18)\n"
			"flw.ps f14, 896(x18)\n"
			"fsgnjn.s f13, f3, f14\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 512(x5)\n"
			"frsq.ps f22, f24\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 544(x18)\n"
			"flw.ps f25, 160(x18)\n"
			"for.pi f9, f21, f25\n" 
			"li x7, 0xe89ab15ca1d82d3b\n"
			"fcvt.s.wu f21, x7, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 416(x26)\n"
			"flw.ps f14, 352(x26)\n"
			"flw.ps f9, 288(x26)\n"
			"fnmadd.ps f27, f3, f14, f9, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_10:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f31, 832(x11)\n"
			"flw.ps f25, 736(x11)\n"
			"fand.pi f2, f31, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_10:\n"
			"maskxor m2, m4, m4\n" 
			"li x5, 0x80340178\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f3, 224(x10)\n"
			"fsrli.pi f14, f3, 0x1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 128(x15)\n"
			"flw.ps f16, 480(x15)\n"
			"fle.ps f9, f3, f16\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 448(x5)\n"
			"flw.ps f25, 128(x5)\n"
			"fminu.pi f31, f5, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 288(x13)\n"
			"fnot.pi f31, f22\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 32(x18)\n"
			"flw.ps f27, 352(x18)\n"
			"flw.ps f13, 256(x18)\n"
			"fnmsub.ps f16, f29, f27, f13, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x18, 0x20ec6213a0365ae2\n"
			"fcvt.s.w f29, x18, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 352(x11)\n"
			"flw.ps f21, 704(x11)\n"
			"fsgnjn.ps f21, f29, f21\n" 
			"la x15, rand_int_32\n"
			"flw.ps f3, 832(x15)\n"
			"fsat8.pi f31, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 672(x7)\n"
			"flw.ps f9, 608(x7)\n"
			"fsgnj.ps f14, f24, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 192(x26)\n"
			"flw.ps f14, 448(x26)\n"
			"fmin.pi f14, f13, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 512(x6)\n"
			"fcvt.wu.s x26, f2, rup\n" 
			"li x5, 0x40719145ae921d8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x50340a9d20ab942\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 640(x18)\n"
			"flw.ps f2, 160(x18)\n"
			"flw.ps f31, 544(x18)\n"
			"fmadd.s f2, f13, f2, f31, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 32(x7)\n"
			"flw.ps f9, 224(x7)\n"
			"flt.pi f29, f25, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_10:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 896(x13)\n"
			"flw.ps f2, 32(x13)\n"
			"fle.s x6, f31, f2\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_10:\n"
			"flem.ps m2, f27, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x18, m6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 192(x22)\n"
			"flw.ps f21, 928(x22)\n"
			"flt.ps f24, f22, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 64(x13)\n"
			"fadd.s f28, f31, f31, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 512(x13)\n"
			"fround.ps f28, f28, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 160(x10)\n"
			"flw.ps f21, 864(x10)\n"
			"flt.ps f16, f31, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 320(x26)\n"
			"flw.ps f28, 736(x26)\n"
			"flw.ps f29, 288(x26)\n"
			"fmsub.s f2, f22, f28, f29, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 768(x5)\n"
			"flw.ps f13, 64(x5)\n"
			"fmax.ps f13, f22, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 0(x26)\n"
			"flw.ps f3, 288(x26)\n"
			"fsub.ps f27, f5, f3, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"flem.ps m1, f2, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f22, 768(x18)\n"
			"flw.ps f3, 896(x18)\n"
			"flt.pi f9, f22, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 160(x12)\n"
			"frsq.ps f15, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 224(x20)\n"
			"fcvt.l.s x19, f14, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 352(x26)\n"
			"fcvt.f16.ps f17, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"flem.ps m0, f9, f25\n" 
			"li x5, 0x80340188\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 416(x26)\n"
			"flw.ps f16, 800(x26)\n"
			"feq.ps f2, f21, f16\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 736(x27)\n"
			"flw.ps f9, 800(x27)\n"
			"fsub.s f22, f21, f9, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 448(x26)\n"
			"fsat8.pi f9, f14\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 800(x13)\n"
			"flw.ps f17, 256(x13)\n"
			"fmin.ps f29, f2, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 480(x18)\n"
			"fsgnjx.s f3, f25, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 768(x19)\n"
			"flw.ps f9, 672(x19)\n"
			"fmadd.s f25, f2, f9, f9, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 416(x13)\n"
			"flw.ps f9, 512(x13)\n"
			"fadd.s f25, f22, f9, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f29, 704(x16)\n"
			"flw.ps f21, 352(x16)\n"
			"fand.pi f28, f29, f21\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 768(x16)\n"
			"fandi.pi f20, f28, 0xf5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 672(x18)\n"
			"fminu.pi f25, f19, f19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 928(x18)\n"
			"flw.ps f2, 832(x18)\n"
			"fadd.ps f4, f22, f2, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 320(x19)\n"
			"flw.ps f11, 640(x19)\n"
			"fsgnjn.s f22, f14, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 288(x16)\n"
			"flw.ps f9, 608(x16)\n"
			"for.pi f11, f2, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_11:\n"
			"fltm.ps m0, f29, f22\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf455f91f04\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f21, 480(x11)\n"
			"faddi.pi f25, f21, 0x105\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 192(x20)\n"
			"flw.ps f17, 480(x20)\n"
			"flt.s x27, f9, f17\n" 
			"fbci.ps f15, 0x53978\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"feqm.ps m5, f4, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"maskor m7, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 480(x18)\n"
			"fsin.ps f25, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"maskand m5, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 128(x11)\n"
			"flw.ps f19, 288(x11)\n"
			"flw.ps f2, 448(x11)\n"
			"fnmsub.ps f20, f28, f19, f2, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 192(x27)\n"
			"flw.ps f19, 416(x27)\n"
			"flw.ps f9, 416(x27)\n"
			"fnmadd.ps f15, f29, f19, f9, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 608(x18)\n"
			"flw.ps f29, 480(x18)\n"
			"fmadd.ps f25, f22, f22, f29, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 32(x20)\n"
			"flw.ps f14, 96(x20)\n"
			"fsgnjn.ps f21, f3, f14\n" 
			"li x27, 0x2705fbe4f9aa4095\n"
			"fcvt.s.w f3, x27, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f29, 512(x12)\n"
			"fnot.pi f25, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"fltm.pi m1, f3, f20\n" 
			"li x5, 0x80340138\n"
			"ld x5, 0(x5)\n"
			"li x20, 0x35a1174048926952\n"
			"fcvt.s.wu f17, x20, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 928(x22)\n"
			"fnot.pi f17, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 0(x12)\n"
			"flw.ps f11, 768(x12)\n"
			"flw.ps f16, 352(x12)\n"
			"fmsub.ps f17, f3, f11, f16, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 64(x13)\n"
			"flw.ps f2, 448(x13)\n"
			"feq.ps f15, f25, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_11:\n"
			"fsetm.pi m5, f29\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_int_32\n"
			"flw.ps f28, 672(x22)\n"
			"flw.ps f4, 384(x22)\n"
			"fmin.pi f2, f28, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 576(x19)\n"
			"flw.ps f21, 960(x19)\n"
			"fsgnj.s f14, f3, f21\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 192(x16)\n"
			"fnot.pi f28, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 704(x16)\n"
			"flw.ps f14, 576(x16)\n"
			"fsub.s f17, f25, f14, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 864(x9)\n"
			"flw.ps f20, 896(x9)\n"
			"fadd.ps f2, f25, f20, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f15, 736(x16)\n"
			"faddi.pi f3, f15, 0x18b\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 992(x22)\n"
			"fclass.ps f11, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 128(x13)\n"
			"flw.ps f3, 576(x13)\n"
			"flw.ps f22, 32(x13)\n"
			"fmsub.s f11, f25, f3, f22, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f16, 640(x19)\n"
			"fandi.pi f21, f16, 0x1e9\n" 
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
			"fbci.pi f11, 0x55720\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_11:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 288(x18)\n"
			"flw.ps f29, 256(x18)\n"
			"fmin.s f16, f19, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 352(x26)\n"
			"flw.ps f15, 832(x26)\n"
			"flt.s x9, f2, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 768(x26)\n"
			"fcvt.ps.pwu f20, f9, rne\n" 
			"maskpopcz x18, m0\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 896(x26)\n"
			"flw.ps f3, 352(x26)\n"
			"fmulh.pi f21, f29, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 192(x11)\n"
			"fsat8.pi f9, f9\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 416(x22)\n"
			"flw.ps f16, 64(x22)\n"
			"feq.ps f2, f20, f16\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 736(x12)\n"
			"flw.ps f28, 704(x12)\n"
			"fsub.pi f20, f29, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 480(x12)\n"
			"fadd.ps f4, f19, f19, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 544(x26)\n"
			"flw.ps f29, 160(x26)\n"
			"fadd.pi f25, f17, f29\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 864(x26)\n"
			"fcvt.ps.pw f2, f9, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 544(x18)\n"
			"flw.ps f15, 544(x18)\n"
			"fdiv.s f25, f9, f15, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 736(x27)\n"
			"fandi.pi f19, f2, 0x4d\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 768(x16)\n"
			"fsub.pi f19, f28, f28\n" 
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
			"flw.ps f21, 864(x11)\n"
			"fcvt.ps.pw f16, f21, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 768(x27)\n"
			"frsq.ps f28, f3\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_11:\n"
			"feqm.ps m1, f19, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_11:\n"
			"maskand m1, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x18, m5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 224(x20)\n"
			"flw.ps f14, 128(x20)\n"
			"flw.ps f19, 224(x20)\n"
			"fnmsub.ps f29, f16, f14, f19, rmm\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f3, 544(x19)\n"
			"fcvt.ps.f16 f28, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 768(x19)\n"
			"fsin.ps f28, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 96(x26)\n"
			"flw.ps f16, 992(x26)\n"
			"flw.ps f3, 960(x26)\n"
			"fmadd.ps f16, f9, f16, f3, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 288(x12)\n"
			"flw.ps f21, 480(x12)\n"
			"fmax.s f3, f20, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 992(x19)\n"
			"flw.ps f21, 544(x19)\n"
			"fsgnjn.s f9, f17, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 608(x12)\n"
			"flw.ps f3, 928(x12)\n"
			"fdivu.pi f14, f21, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 96(x26)\n"
			"flw.ps f21, 160(x26)\n"
			"flw.ps f11, 800(x26)\n"
			"fmsub.s f29, f29, f21, f11, rup\n" 
			"mova.x.m x19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 32(x13)\n"
			"flw.ps f9, 288(x13)\n"
			"fcmovm.ps f22, f11, f9\n" 
			"mova.x.m x12\n" 
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 448(x20)\n"
			"ffrc.ps f17, f29\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 512(x27)\n"
			"fand.pi f15, f17, f17\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 768(x27)\n"
			"flw.ps f29, 928(x27)\n"
			"flt.pi f4, f15, f29\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 672(x12)\n"
			"flw.ps f29, 704(x12)\n"
			"fmax.pi f15, f9, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 288(x18)\n"
			"flw.ps f17, 768(x18)\n"
			"fremu.pi f4, f21, f17\n" 
			"fbci.pi f29, 0x60b5d\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 768(x18)\n"
			"fcvt.w.s x22, f14, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_11:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 800(x20)\n"
			"flw.ps f25, 64(x20)\n"
			"feq.s x22, f20, f25\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_11:\n"
			"maskxor m7, m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_11:\n"
			"fltm.pi m7, f16, f11\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4045a5d326\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f16, 288(x12)\n"
			"fltu.pi f28, f16, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 64(x9)\n"
			"fslli.pi f3, f21, 0xe\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 832(x13)\n"
			"fmulh.pi f21, f2, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 64(x16)\n"
			"fcmovm.ps f3, f4, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 512(x18)\n"
			"fswizz.ps f21, f11, 0x15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 864(x16)\n"
			"fcvt.f16.ps f11, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 288(x7)\n"
			"flw.ps f17, 96(x7)\n"
			"fsll.pi f29, f27, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 320(x9)\n"
			"flw.ps f25, 736(x9)\n"
			"feq.s x28, f27, f25\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 704(x16)\n"
			"flw.ps f17, 96(x16)\n"
			"flw.ps f27, 576(x16)\n"
			"fcmov.ps f10, f3, f17, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_12:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 608(x27)\n"
			"flog.ps f17, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 288(x20)\n"
			"flw.ps f17, 832(x20)\n"
			"fremu.pi f19, f25, f17\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 320(x27)\n"
			"flw.ps f19, 640(x27)\n"
			"fxor.pi f19, f3, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"maskor m0, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f6, 224(x20)\n"
			"flw.ps f25, 576(x20)\n"
			"fle.pi f6, f6, f25\n" 
			"la x15, rand_int_32\n"
			"flw.ps f30, 416(x15)\n"
			"flw.ps f29, 640(x15)\n"
			"flt.pi f28, f30, f29\n" 
			"li x5, 0x3185\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f3, 640(x20)\n"
			"flw.ps f29, 736(x20)\n"
			"fminu.pi f2, f3, f29\n" 
			"li x5, 0x64dde9d8e8fd69a2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3dffdefaa3c6a3b2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x27, rand_int_32\n"
			"flw.ps f6, 704(x27)\n"
			"flw.ps f14, 128(x27)\n"
			"fle.pi f6, f6, f14\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 864(x20)\n"
			"flw.ps f17, 800(x20)\n"
			"fsub.ps f30, f25, f17, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 832(x25)\n"
			"flw.ps f14, 288(x25)\n"
			"flw.ps f29, 608(x25)\n"
			"fcmov.ps f14, f2, f14, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 96(x25)\n"
			"flw.ps f10, 832(x25)\n"
			"flw.ps f15, 736(x25)\n"
			"fnmadd.s f25, f17, f10, f15, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_12:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 416(x15)\n"
			"fexp.ps f2, f17\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x1 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa0 << TENSOR_IMA_SCP_LOC_B) | (0x1d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"li x7, 0xfd52150ae556ed93\n"
			"fcvt.s.w f19, x7, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"maskxor m1, m5, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9c9a5de32b\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f28, 288(x9)\n"
			"fcvt.ps.pwu f15, f28, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 512(x9)\n"
			"flw.ps f13, 544(x9)\n"
			"fmul.ps f1, f15, f13, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 672(x25)\n"
			"flw.ps f25, 800(x25)\n"
			"fand.pi f29, f15, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f13, 896(x25)\n"
			"flw.ps f29, 64(x25)\n"
			"fsub.pi f6, f13, f29\n" 
			"la x15, rand_int_32\n"
			"flw.ps f6, 320(x15)\n"
			"fpackreph.pi f11, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 672(x19)\n"
			"fsatu8.pi f29, f3\n" 
			"li x28, 0x11b124c17d6d7956\n"
			"fcvt.s.lu f2, x28, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 416(x15)\n"
			"flw.ps f25, 288(x15)\n"
			"fcmovm.ps f14, f15, f25\n" 
			"fbci.pi f25, 0xaa72\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 0(x16)\n"
			"flw.ps f30, 384(x16)\n"
			"fdiv.pi f3, f1, f30\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 32(x20)\n"
			"flw.ps f2, 864(x20)\n"
			"fdiv.pi f10, f1, f2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 832(x15)\n"
			"fandi.pi f15, f17, 0x19b\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 192(x25)\n"
			"flw.ps f14, 992(x25)\n"
			"fle.s x28, f15, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 352(x9)\n"
			"flw.ps f28, 224(x9)\n"
			"fmax.ps f19, f11, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_53463_HID_12:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x20, 0x160000000307000b\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x68 << TENSOR_FMA16_SCP_LOC_B) | (0x88 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x15 << TENSOR_FMA16_SCP_LOC_B) | (0x7c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f2, 992(x19)\n"
			"flw.ps f19, 896(x19)\n"
			"fsll.pi f19, f2, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 960(x16)\n"
			"flw.ps f29, 576(x16)\n"
			"flw.ps f13, 128(x16)\n"
			"fmadd.ps f19, f28, f29, f13, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 992(x15)\n"
			"flw.ps f17, 768(x15)\n"
			"fmulhu.pi f30, f27, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 160(x19)\n"
			"feq.s x7, f25, f25\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 992(x20)\n"
			"fsrli.pi f1, f13, 0xc\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 256(x19)\n"
			"fcvt.ps.pwu f29, f13, rmm\n" 
			"li x5, 0xaa94\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 800(x15)\n"
			"flw.ps f11, 224(x15)\n"
			"fsgnj.s f6, f3, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 160(x28)\n"
			"flw.ps f14, 416(x28)\n"
			"fmul.s f2, f28, f14, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 800(x15)\n"
			"flw.ps f30, 416(x15)\n"
			"fsgnjn.s f25, f13, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 96(x19)\n"
			"flw.ps f15, 928(x19)\n"
			"flt.s x20, f13, f15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 32(x15)\n"
			"fcvt.wu.s x17, f10, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 640(x28)\n"
			"flw.ps f3, 992(x28)\n"
			"fxor.pi f2, f29, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 384(x19)\n"
			"fsgnjn.s f13, f11, f11\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"maskand m5, m3, m3\n" 
			"li x5, 0x803401B0\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 512(x15)\n"
			"flw.ps f13, 416(x15)\n"
			"fmax.s f10, f17, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 480(x20)\n"
			"flw.ps f6, 864(x20)\n"
			"fsgnjn.ps f29, f1, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7b << TENSOR_IMA_SCP_LOC_B) | (0xdf << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x25, rand_int_32\n"
			"flw.ps f19, 256(x25)\n"
			"flw.ps f6, 640(x25)\n"
			"flt.pi f10, f19, f6\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 704(x16)\n"
			"fcvt.wu.s x15, f10, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 352(x28)\n"
			"flw.ps f1, 320(x28)\n"
			"fsgnjn.ps f29, f14, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 160(x16)\n"
			"fsgnj.ps f17, f6, f6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 64(x19)\n"
			"flw.ps f11, 256(x19)\n"
			"fadd.s f19, f30, f11, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 96(x19)\n"
			"flw.ps f13, 416(x19)\n"
			"flw.ps f27, 384(x19)\n"
			"fmsub.ps f10, f14, f13, f27, rtz\n" 
			"li x17, 0x7fa81e5fdce087e9\n"
			"fcvt.s.w f1, x17, rne\n" 
			"LBL_SEQID_3_FP_TRANS_ID_5_HID_12:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 288(x27)\n"
			"fexp.ps f6, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 928(x28)\n"
			"fcvt.f16.ps f19, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 224(x28)\n"
			"flw.ps f13, 800(x28)\n"
			"fsgnjn.s f19, f15, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 800(x17)\n"
			"flw.ps f19, 448(x17)\n"
			"flt.ps f17, f3, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 704(x20)\n"
			"flw.ps f30, 992(x20)\n"
			"fcmovm.ps f11, f3, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 576(x27)\n"
			"flw.ps f19, 416(x27)\n"
			"fmulh.pi f1, f10, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 480(x15)\n"
			"flw.ps f11, 416(x15)\n"
			"fadd.ps f15, f19, f11, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 320(x9)\n"
			"flw.ps f3, 736(x9)\n"
			"fadd.pi f27, f13, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 96(x7)\n"
			"flw.ps f25, 576(x7)\n"
			"flw.ps f3, 608(x7)\n"
			"fmsub.s f15, f28, f25, f3, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x37d19e69bbd0a0bd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5e2d385901b72cf7\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 224(x16)\n"
			"fsin.ps f27, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 352(x15)\n"
			"fcvt.pwu.ps f2, f17, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 224(x20)\n"
			"flw.ps f2, 224(x20)\n"
			"feq.pi f10, f17, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 256(x25)\n"
			"flw.ps f27, 384(x25)\n"
			"fsgnjx.ps f14, f3, f27\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 704(x28)\n"
			"flw.ps f2, 416(x28)\n"
			"for.pi f27, f15, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 160(x17)\n"
			"flw.ps f6, 608(x17)\n"
			"fmul.ps f11, f10, f6, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 192(x25)\n"
			"flw.ps f1, 800(x25)\n"
			"fsgnjx.s f2, f25, f1\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 224(x28)\n"
			"fsin.ps f25, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 768(x28)\n"
			"fcvt.pwu.ps f30, f25, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 640(x28)\n"
			"fcvt.f16.ps f10, f13\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_12:\n"
			"mov.m.x m1, x7, 0x6b\n" 
			"li x5, 0x803401E8\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_12:\n"
			"fltm.ps m3, f2, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 224(x25)\n"
			"flw.ps f30, 160(x25)\n"
			"feq.ps f13, f10, f30\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_12:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 480(x20)\n"
			"flw.ps f14, 864(x20)\n"
			"flw.ps f29, 512(x20)\n"
			"fnmsub.s f11, f19, f14, f29, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f14, 672(x27)\n"
			"flw.ps f25, 992(x27)\n"
			"fsll.pi f3, f14, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x72 << TENSOR_FMA16_SCP_LOC_B) | (0xd4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xda << TENSOR_IMA_SCP_LOC_B) | (0x96 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd << TENSOR_FMA32_SCP_LOC_B) | (0xc2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f28, 928(x15)\n"
			"flw.ps f17, 448(x15)\n"
			"feq.pi f2, f28, f17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 32(x15)\n"
			"frsq.ps f28, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 288(x17)\n"
			"fclass.s x17, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 800(x19)\n"
			"flw.ps f17, 224(x19)\n"
			"fadd.s f19, f14, f17, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 768(x17)\n"
			"fcvt.lu.s x19, f27, rup\n" 
			"li x25, 0xb44f70476edc9d5b\n"
			"fcvt.s.w f27, x25, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_12:\n"
			"fltm.pi m3, f3, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f17, 224(x20)\n"
			"flw.ps f10, 128(x20)\n"
			"fadd.pi f11, f17, f10\n" 
			"li x5, 0xfe45\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 928(x16)\n"
			"fcvt.pwu.ps f3, f14, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f11, 864(x15)\n"
			"flw.ps f10, 928(x15)\n"
			"fmulhu.pi f19, f11, f10\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 512(x9)\n"
			"flw.ps f11, 288(x9)\n"
			"fsub.pi f1, f1, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 512(x28)\n"
			"flw.ps f6, 160(x28)\n"
			"fadd.ps f27, f19, f6, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 608(x7)\n"
			"flw.ps f2, 64(x7)\n"
			"fle.s x28, f13, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 640(x7)\n"
			"fsub.ps f25, f10, f10, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 96(x9)\n"
			"fround.ps f17, f13, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 800(x25)\n"
			"fcvt.ps.pw f25, f27, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_182149_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9e << TENSOR_IMA_SCP_LOC_B) | (0xe0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_521837_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x5 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb3 << TENSOR_FMA32_SCP_LOC_B) | (0x6f << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x5, rand_int_32\n"
			"flw.ps f7, 320(x5)\n"
			"flw.ps f10, 320(x5)\n"
			"fand.pi f30, f7, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 32(x6)\n"
			"flw.ps f8, 864(x6)\n"
			"fsgnjn.s f10, f3, f8\n" 
			"la x7, rand_int_32\n"
			"flw.ps f2, 864(x7)\n"
			"flw.ps f20, 512(x7)\n"
			"fmaxu.pi f27, f2, f20\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f3, 544(x13)\n"
			"fcvt.ps.f16 f2, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 448(x27)\n"
			"flw.ps f8, 288(x27)\n"
			"fsgnjx.s f30, f15, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 224(x5)\n"
			"flw.ps f10, 608(x5)\n"
			"fmin.pi f2, f2, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 384(x7)\n"
			"flw.ps f10, 192(x7)\n"
			"fle.pi f23, f23, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 192(x7)\n"
			"fcvt.ps.pwu f20, f9, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"masknot m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 672(x20)\n"
			"flw.ps f24, 832(x20)\n"
			"fsgnjn.ps f27, f2, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"flem.ps m6, f31, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f10, 896(x7)\n"
			"flw.ps f26, 96(x7)\n"
			"fsub.pi f2, f10, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 0(x6)\n"
			"flw.ps f30, 896(x6)\n"
			"fxor.pi f7, f10, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 928(x12)\n"
			"fcvt.lu.s x10, f27, rne\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f31, 864(x10)\n"
			"fcvt.ps.f16 f3, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 768(x20)\n"
			"flw.ps f15, 544(x20)\n"
			"fsub.s f8, f23, f15, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fltm.pi m2, f10, f8\n" 
			"li x5, 0x803401E8\n"
			"ld x5, 0(x5)\n"
			"la x12, rand_int_32\n"
			"flw.ps f20, 608(x12)\n"
			"flw.ps f31, 736(x12)\n"
			"flt.pi f3, f20, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 608(x10)\n"
			"flw.ps f27, 96(x10)\n"
			"fcmovm.ps f24, f20, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 32(x26)\n"
			"fsrai.pi f4, f20, 0x7\n" 
			"maskpopc x20, m3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 928(x6)\n"
			"flw.ps f26, 480(x6)\n"
			"fmaxu.pi f3, f24, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"maskand m2, m3, m3\n" 
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x7f47ee824e57c539\n"
			"fcvt.s.wu f4, x5, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 960(x27)\n"
			"flw.ps f7, 672(x27)\n"
			"fsub.pi f31, f23, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 128(x18)\n"
			"flw.ps f24, 416(x18)\n"
			"fdiv.pi f30, f7, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 672(x26)\n"
			"flw.ps f30, 640(x26)\n"
			"flw.ps f3, 256(x26)\n"
			"fmadd.ps f8, f24, f30, f3, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 576(x18)\n"
			"fcvt.w.s x18, f26, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 64(x26)\n"
			"flw.ps f4, 224(x26)\n"
			"fsgnj.ps f20, f16, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 640(x18)\n"
			"ffrc.ps f26, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 768(x7)\n"
			"flw.ps f20, 256(x7)\n"
			"fmin.s f3, f27, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 352(x20)\n"
			"flw.ps f23, 0(x20)\n"
			"fsgnjx.ps f7, f9, f23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 928(x12)\n"
			"flw.ps f24, 448(x12)\n"
			"fcmovm.ps f9, f31, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 256(x6)\n"
			"flw.ps f27, 608(x6)\n"
			"fadd.ps f20, f7, f27, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 704(x10)\n"
			"flw.ps f26, 864(x10)\n"
			"fsgnj.s f30, f15, f26\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_13:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 640(x27)\n"
			"flog.ps f23, f9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 608(x18)\n"
			"flw.ps f3, 832(x18)\n"
			"fmul.ps f16, f2, f3, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 896(x20)\n"
			"flw.ps f31, 832(x20)\n"
			"fle.pi f23, f24, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 224(x26)\n"
			"flw.ps f30, 768(x26)\n"
			"fle.s x13, f27, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 352(x10)\n"
			"fsrli.pi f9, f27, 0xc\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 416(x7)\n"
			"flw.ps f30, 672(x7)\n"
			"fmul.s f2, f24, f30, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 288(x6)\n"
			"flw.ps f26, 800(x6)\n"
			"feq.ps f30, f2, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 160(x12)\n"
			"flw.ps f15, 992(x12)\n"
			"fadd.s f31, f26, f15, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 608(x26)\n"
			"flw.ps f31, 576(x26)\n"
			"fle.s x13, f4, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"maskor m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f9, 32(x5)\n"
			"fandi.pi f30, f9, 0x4b\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"fltm.ps m2, f20, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 0(x6)\n"
			"fcvt.pwu.ps f27, f31, rmm\n" 
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
			"la x10, rand_int_32\n"
			"flw.ps f16, 288(x10)\n"
			"flw.ps f4, 896(x10)\n"
			"fmaxu.pi f7, f16, f4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 928(x27)\n"
			"flw.ps f31, 832(x27)\n"
			"for.pi f16, f9, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 128(x27)\n"
			"feq.ps f20, f23, f23\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_13:\n"
			"maskxor m2, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f26, 416(x10)\n"
			"flw.ps f8, 672(x10)\n"
			"feq.pi f2, f26, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 512(x10)\n"
			"flw.ps f31, 256(x10)\n"
			"fadd.s f26, f15, f31, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 864(x6)\n"
			"flw.ps f30, 640(x6)\n"
			"fltu.pi f7, f23, f30\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 320(x18)\n"
			"flw.ps f2, 64(x18)\n"
			"flw.ps f27, 896(x18)\n"
			"fmadd.ps f27, f16, f2, f27, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 512(x5)\n"
			"fround.ps f7, f7, rne\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_13:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 704(x12)\n"
			"fexp.ps f8, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 800(x18)\n"
			"frsq.ps f26, f30\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 0(x18)\n"
			"fsatu8.pi f20, f8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 256(x12)\n"
			"flw.ps f3, 992(x12)\n"
			"fsra.pi f15, f27, f3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 32(x26)\n"
			"flw.ps f3, 32(x26)\n"
			"fsra.pi f8, f8, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 576(x27)\n"
			"flw.ps f31, 384(x27)\n"
			"fmul.ps f3, f26, f31, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 928(x10)\n"
			"fsin.ps f7, f2\n" 
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
			"li x6, 0x31e1b15a2f2a5cde\n"
			"fcvt.s.l f9, x6, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 800(x7)\n"
			"fcvt.l.s x6, f8, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 896(x18)\n"
			"flw.ps f20, 896(x18)\n"
			"flw.ps f8, 352(x18)\n"
			"fnmadd.s f27, f26, f20, f8, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 992(x5)\n"
			"flw.ps f31, 832(x5)\n"
			"fmul.ps f3, f3, f31, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 0(x5)\n"
			"flw.ps f8, 544(x5)\n"
			"flw.ps f9, 640(x5)\n"
			"fnmsub.s f27, f20, f8, f9, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 256(x7)\n"
			"fsin.ps f10, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 992(x6)\n"
			"fandi.pi f26, f3, 0xde\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_13:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 704(x20)\n"
			"flog.ps f30, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 704(x18)\n"
			"flw.ps f30, 992(x18)\n"
			"flw.ps f8, 672(x18)\n"
			"fnmadd.s f31, f15, f30, f8, rup\n" 
			"maskpopcz x6, m4\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_13:\n"
			"fltm.ps m2, f20, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd0312b298a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f9, 64(x5)\n"
			"fpackreph.pi f4, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 416(x10)\n"
			"fcvt.f16.ps f8, f16\n" 
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_13:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 224(x5)\n"
			"frcp.ps f3, f30\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_13:\n"
			"masknot m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_13:\n"
			"maskor m2, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"li x5, 0x277122cb05936bf5\n"
			"fcvt.s.w f23, x5, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 864(x5)\n"
			"flw.ps f20, 960(x5)\n"
			"fadd.ps f4, f4, f20, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 320(x13)\n"
			"fsat8.pi f8, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 256(x13)\n"
			"flw.ps f24, 0(x13)\n"
			"fsgnjn.s f27, f16, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 736(x5)\n"
			"flw.ps f15, 448(x5)\n"
			"flw.ps f20, 928(x5)\n"
			"fnmadd.ps f8, f24, f15, f20, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 544(x6)\n"
			"flw.ps f27, 416(x6)\n"
			"fle.pi f8, f8, f27\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f20, 128(x27)\n"
			"fcvt.ps.f16 f8, f20\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 736(x12)\n"
			"flw.ps f31, 256(x12)\n"
			"flw.ps f24, 0(x12)\n"
			"fmadd.ps f16, f2, f31, f24, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 576(x13)\n"
			"flw.ps f30, 96(x13)\n"
			"fsgnj.s f8, f31, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 448(x7)\n"
			"flw.ps f20, 480(x7)\n"
			"fltu.pi f8, f9, f20\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_13:\n"
			"maskxor m6, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 192(x10)\n"
			"flw.ps f8, 992(x10)\n"
			"fsgnjn.ps f2, f30, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 832(x27)\n"
			"flw.ps f9, 96(x27)\n"
			"fsra.pi f3, f3, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 800(x5)\n"
			"flw.ps f4, 288(x5)\n"
			"feq.s x12, f24, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 384(x10)\n"
			"flw.ps f4, 736(x10)\n"
			"fmax.s f15, f7, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 832(x12)\n"
			"flw.ps f10, 384(x12)\n"
			"fmin.s f2, f15, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
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
			"li x5, 0x6\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 256(x9)\n"
			"fswizz.ps f19, f28, 0x10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 480(x13)\n"
			"flw.ps f19, 352(x13)\n"
			"fcmovm.ps f12, f23, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 512(x7)\n"
			"flw.ps f0, 128(x7)\n"
			"fle.ps f28, f2, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 832(x13)\n"
			"frcp.ps f6, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 928(x18)\n"
			"flw.ps f4, 160(x18)\n"
			"feq.ps f23, f21, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 640(x11)\n"
			"flw.ps f6, 192(x11)\n"
			"fadd.ps f2, f4, f6, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 416(x25)\n"
			"flw.ps f23, 864(x25)\n"
			"fltu.pi f12, f18, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 864(x11)\n"
			"flw.ps f15, 64(x11)\n"
			"flt.s x11, f19, f15\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 800(x13)\n"
			"flw.ps f6, 576(x13)\n"
			"feq.ps f12, f19, f6\n" 
			"fbci.pi f5, 0x778e5\n" 
			"li x5, 0xed5b\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 96(x27)\n"
			"flw.ps f4, 0(x27)\n"
			"feq.s x27, f6, f4\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 448(x9)\n"
			"fpackreph.pi f12, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 864(x7)\n"
			"flw.ps f19, 32(x7)\n"
			"fdivu.pi f0, f0, f19\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 416(x25)\n"
			"flw.ps f4, 320(x25)\n"
			"fadd.ps f5, f0, f4, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"fsetm.pi m7, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 992(x18)\n"
			"fmax.s f23, f18, f18\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 960(x20)\n"
			"fcvt.f16.ps f28, f4\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 672(x25)\n"
			"fclass.s x20, f12\n" 
			"li x5, 0x4b31545720408074\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x145efe8c79832dd4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_14:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 672(x6)\n"
			"flog.ps f25, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"maskor m7, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f12, 320(x11)\n"
			"flw.ps f18, 672(x11)\n"
			"fsra.pi f23, f12, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 704(x18)\n"
			"faddi.pi f25, f4, 0x1af\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 928(x20)\n"
			"flw.ps f2, 480(x20)\n"
			"fdiv.ps f18, f28, f2, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 0(x9)\n"
			"fmin.s f4, f26, f26\n" 
			"mova.x.m x27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 352(x18)\n"
			"flw.ps f4, 544(x18)\n"
			"fle.s x23, f5, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_14:\n"
			"fltm.pi m2, f13, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f10, 736(x23)\n"
			"flw.ps f26, 576(x23)\n"
			"fmulhu.pi f26, f10, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_14:\n"
			"fltm.ps m2, f6, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f5, 512(x11)\n"
			"fcvt.ps.pw f12, f5, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_14:\n"
			"fsetm.pi m7, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 96(x27)\n"
			"flw.ps f6, 800(x27)\n"
			"feq.s x18, f18, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x27, 0x160000000307000b\n"
			"csrw tensor_reduce, x27\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_774488_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x33 << TENSOR_FMA32_SCP_LOC_B) | (0x9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
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
			"flw.ps f13, 960(x20)\n"
			"fcvt.ps.pw f23, f13, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 416(x7)\n"
			"fpackrepb.pi f6, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 416(x6)\n"
			"flw.ps f15, 32(x6)\n"
			"fle.s x9, f19, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 160(x18)\n"
			"flw.ps f4, 832(x18)\n"
			"fsub.ps f4, f13, f4, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 896(x27)\n"
			"flw.ps f6, 736(x27)\n"
			"fsra.pi f19, f12, f6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 704(x6)\n"
			"fslli.pi f28, f21, 0x3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_14:\n"
			"maskand m4, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 864(x25)\n"
			"flw.ps f13, 480(x25)\n"
			"flw.ps f28, 480(x25)\n"
			"fcmov.ps f21, f6, f13, f28\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 832(x25)\n"
			"flw.ps f26, 704(x25)\n"
			"fadd.pi f19, f13, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 224(x9)\n"
			"flw.ps f0, 0(x9)\n"
			"fmax.ps f5, f21, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_14:\n"
			"maskor m7, m3, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xcc14303737\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 320(x23)\n"
			"flw.ps f21, 896(x23)\n"
			"flw.ps f18, 288(x23)\n"
			"fnmsub.s f19, f12, f21, f18, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 544(x18)\n"
			"flw.ps f21, 928(x18)\n"
			"fxor.pi f21, f23, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 64(x23)\n"
			"flw.ps f0, 256(x23)\n"
			"fle.ps f19, f12, f0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 544(x11)\n"
			"fpackreph.pi f21, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 640(x27)\n"
			"flw.ps f5, 416(x27)\n"
			"fmax.s f6, f18, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x58 << TENSOR_IMA_SCP_LOC_B) | (0x37 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"maskpopc x7, m2\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 384(x11)\n"
			"fltu.pi f12, f5, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 32(x20)\n"
			"flw.ps f28, 128(x20)\n"
			"fltu.pi f25, f25, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 960(x11)\n"
			"flw.ps f15, 320(x11)\n"
			"feq.ps f26, f4, f15\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 928(x11)\n"
			"flw.ps f4, 928(x11)\n"
			"feq.ps f5, f19, f4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 352(x6)\n"
			"flw.ps f6, 544(x6)\n"
			"fsgnj.s f5, f15, f6\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 576(x27)\n"
			"flw.ps f4, 64(x27)\n"
			"fltu.pi f19, f10, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 288(x18)\n"
			"flw.ps f13, 768(x18)\n"
			"flw.ps f10, 832(x18)\n"
			"fcmov.ps f6, f2, f13, f10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f23, 288(x25)\n"
			"flw.ps f28, 384(x25)\n"
			"fsra.pi f23, f23, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 96(x9)\n"
			"flw.ps f6, 352(x9)\n"
			"flw.ps f4, 768(x9)\n"
			"fmadd.ps f21, f2, f6, f4, rup\n" 
			"fbci.pi f25, 0x4a661\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 352(x11)\n"
			"fsatu8.pi f0, f5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 512(x20)\n"
			"fclass.ps f6, f5\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_14:\n"
			"fltm.pi m3, f5, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_14:\n"
			"fsetm.pi m7, f5\n" 
			"li x5, 0x80340198\n"
			"ld x5, 0(x5)\n"
			"li x25, 0x319d12a5e7bf2419\n"
			"fcvt.s.w f5, x25, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_212460_HID_14:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_589359_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf3 << TENSOR_FMA16_SCP_LOC_B) | (0x6b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_322676_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x60 << TENSOR_FMA16_SCP_LOC_B) | (0xea << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x25, rand_int_32\n"
			"flw.ps f13, 64(x25)\n"
			"flw.ps f15, 864(x25)\n"
			"fadd.pi f2, f13, f15\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 832(x13)\n"
			"flw.ps f15, 608(x13)\n"
			"flw.ps f18, 704(x13)\n"
			"fmsub.ps f13, f25, f15, f18, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 192(x7)\n"
			"flw.ps f2, 224(x7)\n"
			"fmul.s f6, f5, f2, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 128(x27)\n"
			"flw.ps f10, 672(x27)\n"
			"feq.ps f2, f2, f10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f23, 448(x25)\n"
			"flw.ps f0, 32(x25)\n"
			"fremu.pi f25, f23, f0\n" 
			"li x5, 0x8ff4e7051356bd82\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4a9b646b38b771d3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 768(x13)\n"
			"flw.ps f2, 704(x13)\n"
			"flw.ps f0, 640(x13)\n"
			"fmadd.s f25, f15, f2, f0, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 544(x13)\n"
			"flw.ps f0, 640(x13)\n"
			"flw.ps f25, 224(x13)\n"
			"fnmadd.s f25, f10, f0, f25, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 768(x25)\n"
			"fround.ps f4, f10, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 256(x6)\n"
			"flw.ps f10, 768(x6)\n"
			"fmax.pi f13, f19, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 480(x11)\n"
			"fround.ps f0, f23, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 0(x13)\n"
			"fnot.pi f13, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 864(x6)\n"
			"fslli.pi f26, f13, 0xa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_int_32\n"
			"flw.ps f23, 608(x18)\n"
			"fsatu8.pi f5, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 320(x25)\n"
			"flw.ps f12, 320(x25)\n"
			"fmul.s f13, f26, f12, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 704(x27)\n"
			"flw.ps f26, 320(x27)\n"
			"fsub.ps f18, f23, f26, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_14:\n"
			"maskor m3, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x4000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
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
			"LBL_SEQID_5_FP_TRANS_ID_12_HID_14:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 32(x7)\n"
			"fexp.ps f28, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f0, 128(x9)\n"
			"faddi.pi f4, f0, 0x114\n" 
			"la x27, rand_int_32\n"
			"flw.ps f6, 896(x27)\n"
			"fnot.pi f4, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 576(x23)\n"
			"flw.ps f28, 192(x23)\n"
			"flt.pi f13, f0, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 416(x7)\n"
			"flw.ps f23, 32(x7)\n"
			"fsgnjn.s f25, f15, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 256(x27)\n"
			"ffrc.ps f10, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 992(x11)\n"
			"fcvt.pwu.ps f28, f2, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 288(x27)\n"
			"fle.s x11, f21, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 608(x13)\n"
			"flw.ps f10, 352(x13)\n"
			"fmin.ps f5, f28, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 768(x7)\n"
			"flw.ps f26, 192(x7)\n"
			"fsll.pi f6, f0, f26\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 640(x18)\n"
			"flw.ps f6, 96(x18)\n"
			"fmax.pi f12, f21, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 864(x9)\n"
			"fcvt.lu.s x13, f18, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 416(x13)\n"
			"fslli.pi f18, f23, 0x5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 928(x25)\n"
			"flw.ps f10, 0(x25)\n"
			"flw.ps f23, 96(x25)\n"
			"fmsub.s f2, f28, f10, f23, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_14:\n"
			"maskand m7, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_14:\n"
			"flem.ps m2, f25, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xb\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
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
			"csrwi mhpmevent3, 23\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f5,  416 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f19,  256 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
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
			"LBL_KERNEL_HID_15:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_15\n"
			"li x5, 0x6\n" 
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
			"fbci.ps f8, 0x5debd\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"feqm.ps m2, f8, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 512(x5)\n"
			"flw.ps f19, 608(x5)\n"
			"fmin.s f9, f11, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"flem.ps m6, f30, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 64(x15)\n"
			"flw.ps f4, 768(x15)\n"
			"fcmovm.ps f15, f20, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 384(x19)\n"
			"flw.ps f20, 256(x19)\n"
			"fmin.s f5, f2, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 992(x27)\n"
			"flw.ps f8, 352(x27)\n"
			"fle.pi f15, f2, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 544(x22)\n"
			"fmul.s f7, f11, f11, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 768(x5)\n"
			"flw.ps f9, 544(x5)\n"
			"fmsub.s f5, f8, f9, f8, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 704(x18)\n"
			"flw.ps f9, 960(x18)\n"
			"flt.ps f28, f5, f9\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 64(x18)\n"
			"flw.ps f30, 768(x18)\n"
			"fsrl.pi f2, f19, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 416(x6)\n"
			"flw.ps f20, 160(x6)\n"
			"fle.pi f7, f16, f20\n" 
			"maskpopcz x15, m2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 224(x6)\n"
			"fsrli.pi f10, f0, 0xe\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 32(x11)\n"
			"flw.ps f16, 992(x11)\n"
			"fadd.pi f7, f10, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 128(x6)\n"
			"fcvt.wu.s x6, f28, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f2, 160(x27)\n"
			"fpackrepb.pi f11, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 512(x5)\n"
			"flw.ps f30, 992(x5)\n"
			"fminu.pi f13, f28, f30\n" 
			"li x14, 0x68897b5254d645bb\n"
			"fcvt.s.lu f13, x14, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 416(x11)\n"
			"fcvt.ps.pw f10, f30, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 448(x19)\n"
			"fcvt.pw.ps f8, f5, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 352(x22)\n"
			"flw.ps f8, 480(x22)\n"
			"flw.ps f7, 0(x22)\n"
			"fnmsub.s f4, f20, f8, f7, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 544(x14)\n"
			"flw.ps f13, 224(x14)\n"
			"fsgnjn.s f4, f9, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 704(x22)\n"
			"flw.ps f30, 416(x22)\n"
			"fsgnjx.ps f20, f19, f30\n" 
			"li x22, 0x3e1c83e5a431e9b5\n"
			"fcvt.s.wu f8, x22, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 704(x27)\n"
			"fcmovm.ps f13, f19, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f0, 352(x22)\n"
			"flw.ps f19, 480(x22)\n"
			"fsra.pi f10, f0, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"fltm.pi m2, f11, f16\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"maskxor m2, m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 512(x6)\n"
			"fsin.ps f9, f15\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 512(x6)\n"
			"flw.ps f5, 608(x6)\n"
			"fsrl.pi f30, f30, f5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 0(x18)\n"
			"flw.ps f20, 480(x18)\n"
			"fdiv.pi f10, f5, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f28, 576(x5)\n"
			"fslli.pi f11, f28, 0x3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 384(x5)\n"
			"faddi.pi f7, f2, 0x3f\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 960(x27)\n"
			"fslli.pi f8, f20, 0x6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 0(x6)\n"
			"flw.ps f8, 320(x6)\n"
			"flw.ps f10, 32(x6)\n"
			"fmadd.s f20, f7, f8, f10, rdn\n" 
			"fbci.pi f13, 0x2ac11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 544(x27)\n"
			"flw.ps f10, 160(x27)\n"
			"flw.ps f16, 640(x27)\n"
			"fcmov.ps f13, f9, f10, f16\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 736(x27)\n"
			"flw.ps f9, 480(x27)\n"
			"fand.pi f4, f8, f9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 704(x19)\n"
			"fsin.ps f28, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 800(x22)\n"
			"flw.ps f11, 0(x22)\n"
			"fsgnj.ps f30, f5, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 224(x5)\n"
			"flw.ps f4, 704(x5)\n"
			"fmulhu.pi f30, f15, f4\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 832(x18)\n"
			"fpackreph.pi f11, f15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"fsetm.pi m1, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 288(x9)\n"
			"fcvt.f16.ps f11, f30\n" 
			"maskpopc x6, m1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 640(x18)\n"
			"fcvt.pwu.ps f0, f15, rtz\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 448(x15)\n"
			"fcvt.f16.ps f7, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 608(x6)\n"
			"fcvt.wu.s x22, f10, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 704(x6)\n"
			"fsqrt.s f0, f19, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 864(x19)\n"
			"flw.ps f20, 448(x19)\n"
			"fminu.pi f28, f30, f20\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 800(x5)\n"
			"flw.ps f8, 704(x5)\n"
			"fsub.ps f7, f20, f8, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 672(x14)\n"
			"flw.ps f4, 704(x14)\n"
			"fsub.pi f5, f10, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 576(x14)\n"
			"flw.ps f5, 352(x14)\n"
			"fle.ps f19, f10, f5\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 512(x19)\n"
			"flw.ps f15, 512(x19)\n"
			"fmul.pi f11, f30, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 704(x27)\n"
			"fround.ps f4, f19, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 160(x19)\n"
			"fslli.pi f9, f19, 0xe\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 960(x6)\n"
			"flw.ps f15, 768(x6)\n"
			"fadd.s f28, f13, f15, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_15:\n"
			"fltm.ps m6, f4, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 928(x18)\n"
			"flw.ps f2, 448(x18)\n"
			"fsgnj.s f30, f9, f2\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 352(x18)\n"
			"fandi.pi f13, f11, 0x145\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_15:\n"
			"fltm.pi m6, f8, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_15:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 608(x27)\n"
			"fexp.ps f0, f16\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_15:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_15:\n"
			"maskor m6, m6, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x33b33cdd87\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 864(x11)\n"
			"ffrc.ps f11, f5\n" 
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_15:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 384(x5)\n"
			"fexp.ps f16, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 768(x27)\n"
			"flw.ps f0, 128(x27)\n"
			"fsgnjx.s f13, f2, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 352(x18)\n"
			"fsqrt.s f10, f0, rmm\n" 
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_15:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 672(x6)\n"
			"fexp.ps f4, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 544(x11)\n"
			"fcvt.w.s x19, f30, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 128(x27)\n"
			"fpackreph.pi f15, f15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 448(x22)\n"
			"flw.ps f19, 800(x22)\n"
			"flw.ps f28, 960(x22)\n"
			"fmadd.ps f5, f7, f19, f28, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_15:\n"
			"mov.m.x m2, x19, 0x1c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f5, 128(x15)\n"
			"flw.ps f4, 352(x15)\n"
			"fmin.pi f4, f5, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 64(x18)\n"
			"flw.ps f5, 832(x18)\n"
			"flw.ps f8, 672(x18)\n"
			"fcmov.ps f2, f30, f5, f8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_15:\n"
			"maskor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f10, 320(x22)\n"
			"fslli.pi f11, f10, 0x6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 480(x11)\n"
			"flw.ps f10, 992(x11)\n"
			"fsub.s f15, f20, f10, rup\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 768(x9)\n"
			"flw.ps f4, 32(x9)\n"
			"fsub.s f7, f19, f4, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 640(x22)\n"
			"flw.ps f19, 672(x22)\n"
			"fmax.s f2, f20, f19\n" 
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_15:\n"
			"fltm.pi m5, f20, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f20, 384(x9)\n"
			"flw.ps f16, 736(x9)\n"
			"fmin.pi f2, f20, f16\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_15:\n"
			"fltm.ps m5, f16, f0\n" 
			"li x5, 0x80340190\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 192(x27)\n"
			"flw.ps f28, 448(x27)\n"
			"fsub.s f8, f2, f28, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 352(x27)\n"
			"flw.ps f0, 736(x27)\n"
			"fsub.ps f19, f7, f0, rdn\n" 
			"LBL_SEQID_5_FP_TRANS_ID_17_HID_15:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 768(x27)\n"
			"fexp.ps f10, f9\n" 
			"maskpopc x19, m2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 416(x18)\n"
			"flw.ps f9, 768(x18)\n"
			"fmax.s f13, f13, f9\n" 
			"fbci.ps f28, 0x62631\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 640(x6)\n"
			"flw.ps f9, 288(x6)\n"
			"fsub.s f9, f20, f9, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_18_HID_15:\n"
			"fltm.pi m2, f16, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 672(x22)\n"
			"fcvt.pw.ps f10, f30, rup\n" 
			"maskpopcz x5, m1\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 832(x22)\n"
			"fpackreph.pi f10, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
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
		0xf9af3a9c, 
		0xde342c19, 
		0xfbad5ba2, 
		0x42c03f99, 
		0x313861de, 
		0xde83cf64, 
		0xb1fc13bd, 
		0x63d26f0c, 
		0x80f1ceaf, 
		0x050bcd84, 
		0x56cd396f, 
		0x8954de3a, 
		0x4e6839fd, 
		0x6cc5daad, 
		0x75c7eeef, 
		0xd0961aee, 
		0x1f41d84b, 
		0x615ff3cb, 
		0xca144fc3, 
		0xf52d3342, 
		0xcd622e2f, 
		0x0f8fb262, 
		0xe1edb78b, 
		0x4918a34c, 
		0x1c013f8d, 
		0x69d2181f, 
		0xb6496ddd, 
		0x2c1cc0a1, 
		0x4de5e770, 
		0xda35e3b4, 
		0xbbd87f23, 
		0xc634e7a4, 
		0xf8b743e9, 
		0x23f38373, 
		0xab428790, 
		0xc0429445, 
		0x9532fe15, 
		0x6d938928, 
		0x6fe4107d, 
		0xba22a7e3, 
		0x7a1c56f1, 
		0x4d110ce9, 
		0x3e559fc8, 
		0xbecab294, 
		0x489a47ee, 
		0xfcdb712c, 
		0x7f819ff3, 
		0xb4db6d17, 
		0x764a9ba4, 
		0x641b6bb7, 
		0x91c8c3d1, 
		0xede9f8db, 
		0x4e6bf511, 
		0x1116584d, 
		0x155b0d55, 
		0x15c8196d, 
		0x89356bfc, 
		0x59c7840d, 
		0x40311e5d, 
		0xc9413edd, 
		0x637eb1ca, 
		0xc3ec364e, 
		0x41877400, 
		0xda4c5013, 
		0xf9067ba9, 
		0x7594d43b, 
		0x0d824c58, 
		0x13e953fc, 
		0x3895a6f4, 
		0x5622a022, 
		0x9983d6d2, 
		0x0f6c5c9c, 
		0x05ebd1c2, 
		0xa48ce4b6, 
		0x91340b64, 
		0x33539874, 
		0x20adba0b, 
		0x8f4c1f15, 
		0xa880ade4, 
		0xf08d2dd8, 
		0x0554dbda, 
		0x90a026c2, 
		0x78c4a980, 
		0xdc66458c, 
		0xeb6a9a20, 
		0x20b4cd1d, 
		0xac99d460, 
		0x57f5f8f5, 
		0xc75a198e, 
		0xc61fb45b, 
		0xbd46ed45, 
		0x3088f134, 
		0xd09750ba, 
		0x02486bd4, 
		0xe3b0c423, 
		0xd5ac5651, 
		0x01788198, 
		0x90e5d055, 
		0xc75e20d1, 
		0x9a545580, 
		0x470efd46, 
		0xd59e75e9, 
		0xa93af86e, 
		0xc6931ab0, 
		0x9eea782e, 
		0x0c1dd717, 
		0xc9659f6a, 
		0xb5e584b7, 
		0x9024784d, 
		0x45469203, 
		0x4d1f3f6e, 
		0x04811df9, 
		0x45274ba2, 
		0x535d6508, 
		0x026d013a, 
		0x1c567ff2, 
		0xd5324961, 
		0xa3fce591, 
		0xed9d3f1b, 
		0xf7d9d95b, 
		0x739651c8, 
		0xc682a67e, 
		0x779143c2, 
		0x12fe2ff7, 
		0xcfd483e3, 
		0x76a2d454, 
		0xa6b90e7e, 
		0x385b1cad, 
		0x43179093, 
		0x2744ff32, 
		0x8602d7e3, 
		0xe596b4e3, 
		0xfe76e07d, 
		0xa1db785f, 
		0x0c29a6c2, 
		0x26cd167a, 
		0xbeb4dbf7, 
		0xd5fb114f, 
		0xf0c1275c, 
		0x88a885ba, 
		0xd196ece3, 
		0xa2beb067, 
		0xad507e6f, 
		0x8d32efef, 
		0xd7b3e9ea, 
		0x9f3f6cd4, 
		0x7064cea9, 
		0x7526aa8e, 
		0x61685b8f, 
		0x832cf796, 
		0x242dd8fb, 
		0x80b4311c, 
		0x48c5cf8f, 
		0xbd5788dc, 
		0xf5d1dc0b, 
		0x343ba525, 
		0x3e30fed5, 
		0xadfd0930, 
		0x512528a7, 
		0xf8a37adb, 
		0xca8b45bc, 
		0x18dd0953, 
		0x185c42ee, 
		0xc04db4ab, 
		0x1541047e, 
		0x9c50579c, 
		0x767c592a, 
		0x71945b9e, 
		0xfe4dc035, 
		0x94759a32, 
		0x0d8cb0af, 
		0x15072f14, 
		0xc6e499a6, 
		0xe1dd65ce, 
		0x942abcb9, 
		0x52238956, 
		0x23086e1f, 
		0x5c1664a5, 
		0xc31fa93a, 
		0x31012443, 
		0x0eb2f5f9, 
		0x2e940fda, 
		0x101d4855, 
		0xcda76427, 
		0x40af5de6, 
		0xfbc96e4b, 
		0xab943f03, 
		0x91f2363f, 
		0xe853eaec, 
		0xd4661e32, 
		0x63165b65, 
		0x478bf403, 
		0x29c65a44, 
		0xecec5066, 
		0x16e9ccd4, 
		0x6d090e8c, 
		0x0a1a2b0c, 
		0x3ad2b79f, 
		0x460b643b, 
		0xf2e5acb2, 
		0x1f8c791b, 
		0x428923d0, 
		0xb4d777eb, 
		0x337e56ad, 
		0x1dc255ac, 
		0x255e6c84, 
		0x2058bfca, 
		0x23bafd1a, 
		0xdab8301e, 
		0xfe87bad3, 
		0x449a8269, 
		0x15718675, 
		0x89663076, 
		0x305de92a, 
		0xde4071dd, 
		0x788283ab, 
		0x3e05e469, 
		0x21c15712, 
		0x5f34ec9e, 
		0x9269dfa0, 
		0xd2711452, 
		0xa55ed06a, 
		0x2893a784, 
		0xbb27dc3c, 
		0x51cc31f5, 
		0x9792a8d3, 
		0x7b0edae1, 
		0xcd288462, 
		0x0505b32e, 
		0x30ca5a9d, 
		0xa69521a2, 
		0xbee5fa59, 
		0x40d9cb7c, 
		0xeb26305e, 
		0x9adc4f7d, 
		0xe74352bf, 
		0x44a2490c, 
		0xf17d4e43, 
		0x67e105f3, 
		0x7bc4bc5b, 
		0xe7c09bb6, 
		0x5a21372a, 
		0x492cdef3, 
		0x8c2527c3, 
		0xf665fddc, 
		0xe0785c31, 
		0xde5acdab, 
		0x2a45786a, 
		0x1f746406, 
		0x601bab36, 
		0x383df2e2, 
		0x322653c0, 
		0xbab34e3c, 
		0xe9c4eb1f, 
		0x86feb6d9, 
		0x503a5c6b  
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
		0xd0c0, 
		0xd63a, 
		0x8000, 
		0x5080, 
		0xc900, 
		0x4a80, 
		0xd360, 
		0xee2c, 
		0xd140, 
		0x5100, 
		0xfd22, 
		0x3914, 
		0xd3a0, 
		0x1a78, 
		0xca00, 
		0x4880, 
		0xccc0, 
		0x14e5, 
		0x50a0, 
		0x4400, 
		0xca00, 
		0x5b38, 
		0xc900, 
		0x4b80, 
		0xd3e0, 
		0x5200, 
		0x51c0, 
		0xb495, 
		0xbc00, 
		0x8b68, 
		0xcc00, 
		0xd0a0, 
		0x019d, 
		0x5220, 
		0x7c00, 
		0x4c00, 
		0xf894, 
		0x5100, 
		0x27ad, 
		0x152d, 
		0x53a0, 
		0x8000, 
		0x4d80, 
		0x6db0, 
		0xd040, 
		0x5380, 
		0xb3e6, 
		0x5120, 
		0xcc32, 
		0xcf40, 
		0xd040, 
		0xc980, 
		0xd140, 
		0xcf00, 
		0xfc00, 
		0x8000, 
		0x5320, 
		0x4d00, 
		0x2307, 
		0x2cc8, 
		0xce00, 
		0xd140, 
		0xc000, 
		0xd160, 
		0xd100, 
		0x51e0, 
		0xcf00, 
		0xd1e0, 
		0xd1a0, 
		0xd300, 
		0xda37, 
		0x4e80, 
		0x52c0, 
		0xe34f, 
		0x53e0, 
		0xd380, 
		0x8a62, 
		0x8608, 
		0x4f7c, 
		0xd0c0, 
		0xaed2, 
		0x3089, 
		0xd0a0, 
		0xd120, 
		0x385b, 
		0x5160, 
		0xdbcc, 
		0x35e9, 
		0x53a0, 
		0x53e0, 
		0xd340, 
		0x9465, 
		0x4e40, 
		0xbaf5, 
		0x53a0, 
		0xd060, 
		0x05db, 
		0xfc61, 
		0xd729, 
		0xdd01, 
		0x80fe, 
		0xd140, 
		0x51a0, 
		0x0000, 
		0x5260, 
		0xd3c0, 
		0x4c40, 
		0x4b00, 
		0xf436, 
		0x4500, 
		0x80a7, 
		0xd080, 
		0xaf00, 
		0x4d40, 
		0x4de0, 
		0xba47, 
		0x51e0, 
		0xc600, 
		0x4e80, 
		0xd5fc, 
		0x2f20, 
		0x0071, 
		0x4f40, 
		0x51e0, 
		0xee00, 
		0x4cc0, 
		0x4700, 
		0xcd40, 
		0x31a2, 
		0xa3e4, 
		0xfd6c, 
		0xbc00, 
		0x7c00, 
		0xbc99, 
		0x5100, 
		0xf6e0, 
		0xeca3, 
		0x0000, 
		0x0fdb, 
		0x50c0, 
		0xd1a0, 
		0x3f0c, 
		0x5380, 
		0x7c00, 
		0xe727, 
		0xd280, 
		0xa377, 
		0x5200, 
		0x4a80, 
		0x5260, 
		0x5300, 
		0x50e0, 
		0xd120, 
		0x0c16, 
		0x8000, 
		0x4b00, 
		0xf01f, 
		0x8000, 
		0x4200, 
		0x705c, 
		0x5200, 
		0xc0f4, 
		0x75be, 
		0xccc0, 
		0xc600, 
		0xd320, 
		0xd3e0, 
		0xfdc7, 
		0x0000, 
		0x5140, 
		0xf787, 
		0x64ee, 
		0x52c0, 
		0xcf80, 
		0x8000, 
		0x1af0, 
		0x02ab, 
		0x5380, 
		0xd0a0, 
		0x41f4, 
		0x4b80, 
		0x9a6a, 
		0xe254, 
		0x5360, 
		0x4e00, 
		0x4d00, 
		0x40ba, 
		0x8000, 
		0xd360, 
		0xb89f, 
		0x5000, 
		0xcfc0, 
		0x4600, 
		0x708e, 
		0x5000, 
		0x53f6, 
		0xe671, 
		0xfc00, 
		0x934a, 
		0x4980, 
		0x52c0, 
		0x4a00, 
		0xd260, 
		0xd2c0, 
		0xcd80, 
		0xf619, 
		0x4c6a, 
		0x5280, 
		0x5180, 
		0x5140, 
		0xcf80, 
		0xd0a0, 
		0x4a00, 
		0x5320, 
		0x4200, 
		0x4c80, 
		0x4ec0, 
		0xcd00, 
		0x5020, 
		0xd869, 
		0x6073, 
		0xcf80, 
		0x5653, 
		0xbc00, 
		0xcd80, 
		0xd1e0, 
		0x4f40, 
		0xd000, 
		0x51e0, 
		0x4e00, 
		0x5000, 
		0x53e0, 
		0x52c0, 
		0x4900, 
		0x8000, 
		0xfc85, 
		0xd0a0, 
		0x5160, 
		0x5300, 
		0xe395, 
		0x0000, 
		0x5180, 
		0x8000, 
		0xd1c0, 
		0x4880, 
		0x51e0, 
		0x7c00, 
		0x3e9f, 
		0xd0c0, 
		0x7c00, 
		0x51e0, 
		0xe215, 
		0x0566, 
		0x76e7, 
		0xe641, 
		0x8000, 
		0x4d40, 
		0xd320, 
		0xc980, 
		0x5360, 
		0x3272, 
		0xd1a0, 
		0xca00, 
		0xf03c, 
		0x4e80, 
		0x7b1f, 
		0xd1c0, 
		0xd180, 
		0xcd80, 
		0x9788, 
		0xc600, 
		0x5280, 
		0x8000, 
		0xa5fd, 
		0xd2c0, 
		0x5320, 
		0x4880, 
		0x62f7, 
		0x4d8f, 
		0x4500, 
		0x4e80, 
		0xdf25, 
		0xed76, 
		0xd200, 
		0x52a0, 
		0x5120, 
		0xd060, 
		0xaf0a, 
		0x5100, 
		0xc200, 
		0x51c0, 
		0x6cb7, 
		0xd380, 
		0xce40, 
		0xb3e6, 
		0x4c40, 
		0xa788, 
		0x5060, 
		0x53e0, 
		0xd060, 
		0x4980, 
		0x8000, 
		0x0f47, 
		0x50c0, 
		0x5180, 
		0x5140, 
		0xcf80, 
		0xf696, 
		0x301f, 
		0x3cf7, 
		0xeede, 
		0xcf00, 
		0xcb00, 
		0xdd32, 
		0x8000, 
		0xcd00, 
		0x0000, 
		0x910f, 
		0xcf80, 
		0x89b1, 
		0x7c00, 
		0x4f00, 
		0xc400, 
		0x3d95, 
		0x53e0, 
		0x61e1, 
		0xf066, 
		0xc980, 
		0x48d7, 
		0x5000, 
		0x5220, 
		0x52a0, 
		0xd380, 
		0xca00, 
		0xca00, 
		0x6ffb, 
		0x105f, 
		0xd0e0, 
		0x6a2f, 
		0xc200, 
		0xd0a0, 
		0x52a0, 
		0x4f00, 
		0xd360, 
		0xfddc, 
		0xda0b, 
		0xd1c0, 
		0x0000, 
		0xd040, 
		0xbc00, 
		0x4a80, 
		0xa1f6, 
		0x4400, 
		0x875a, 
		0x4b80, 
		0x4c80, 
		0x4700, 
		0x7c00, 
		0x50a0, 
		0xd380, 
		0x4b00, 
		0xcb00, 
		0x4980, 
		0x4600, 
		0x4c40, 
		0x4ec0, 
		0xce40, 
		0xf16b, 
		0x0a05, 
		0x5060, 
		0xd240, 
		0x4f00, 
		0xd3e0, 
		0xd385, 
		0x7c00, 
		0xc900, 
		0x5018, 
		0x3b17, 
		0x4cc0, 
		0x5380, 
		0x90c3, 
		0xd040, 
		0xcdc0, 
		0xfe7b, 
		0xb51c, 
		0xd2a0, 
		0xc500, 
		0xce80, 
		0x8000, 
		0x4f40, 
		0x8000, 
		0x5217, 
		0x53a0, 
		0x5140, 
		0x8713, 
		0x316e, 
		0x5280, 
		0x4c80, 
		0x4700, 
		0xc880, 
		0xd120, 
		0x8000, 
		0x4f40, 
		0x4c40, 
		0xd100, 
		0xbbe6, 
		0x973b, 
		0x5340, 
		0xca00, 
		0x36bf, 
		0x8000, 
		0x4c9a, 
		0x0000, 
		0x0000, 
		0x4d80, 
		0x5300, 
		0xd3c0, 
		0x7c00, 
		0xcd40, 
		0x5520, 
		0xfc00, 
		0x5120, 
		0xf35d, 
		0x8743, 
		0x5260, 
		0x5260, 
		0x5360, 
		0x4b00, 
		0xd060, 
		0x0000, 
		0xfc2d, 
		0x50c0, 
		0x5260, 
		0xd1a0, 
		0x4cc0, 
		0x4200, 
		0x1f9c, 
		0xd060, 
		0xc000, 
		0xd220, 
		0x43c1, 
		0xf38d, 
		0x53c0, 
		0x399b, 
		0xaa59, 
		0x7a1f, 
		0xe6ea, 
		0xd220, 
		0xd320, 
		0xd100, 
		0x5080, 
		0x4d40, 
		0x53c0, 
		0x51e0, 
		0x0000, 
		0xd060, 
		0xae0b, 
		0xc000, 
		0x9eac, 
		0xd260, 
		0x4f80, 
		0xc600, 
		0x91c7, 
		0xf3cc, 
		0xe282, 
		0xd200, 
		0x53a0, 
		0xd320, 
		0x4000, 
		0xd220, 
		0x51e0, 
		0x4f00, 
		0xd2c0, 
		0x1d61, 
		0xd280, 
		0x5080, 
		0x3c00, 
		0x3c00, 
		0x4f80, 
		0x2c6f, 
		0xd300, 
		0xfc00, 
		0xd060, 
		0xd280, 
		0x3817, 
		0xcf00, 
		0x1dcf, 
		0x52a0, 
		0x53e0, 
		0xfc00, 
		0x0000, 
		0x50e0, 
		0xcc80, 
		0xc700, 
		0x53c0, 
		0xc200, 
		0x8000, 
		0x4f00, 
		0x4dc0, 
		0x5ae8, 
		0x0000, 
		0xb81d, 
		0xcc40, 
		0xd1c0, 
		0x52a0, 
		0x5060, 
		0x0000, 
		0xcd40, 
		0xd200, 
		0x4700, 
		0xcc00, 
		0x8000, 
		0x5060, 
		0x8845, 
		0xbd4d, 
		0x4d40, 
		0x8000, 
		0x4f00, 
		0x51c0, 
		0x40b4, 
		0xccc0, 
		0x5060, 
		0x3b7d, 
		0x5300, 
		0xd120, 
		0x8f14, 
		0x38a8, 
		0xd2a0, 
		0x031a, 
		0xec40, 
		0xcc00, 
		0x4b00, 
		0xcf00, 
		0xfc00, 
		0x4d00, 
		0xccc0, 
		0xd300, 
		0x1a65, 
		0x5140, 
		0x453b, 
		0x5060, 
		0xd1a0, 
		0xc500, 
		0xd2a0, 
		0x51c0, 
		0x52e0, 
		0x5040, 
		0x50a0, 
		0x5040, 
		0x4c80, 
		0xcdc0, 
		0x50a0, 
		0x5280, 
		0x7c00, 
		0xc600, 
		0x32bc, 
		0x0000, 
		0x4f40, 
		0x4dc0, 
		0x5220, 
		0x250d, 
		0x87f3, 
		0xccc0, 
		0xfc00, 
		0xd120, 
		0x51e0, 
		0x5060, 
		0x17bd, 
		0xd3e0, 
		0x4400, 
		0x53a0, 
		0x0000, 
		0x3619, 
		0xc700, 
		0x1e12, 
		0x4e40, 
		0xa815, 
		0x6196, 
		0xd3e0, 
		0x0000, 
		0xce00, 
		0xcd40, 
		0xd180, 
		0x5080, 
		0x850e, 
		0xd3a0, 
		0xcc40, 
		0x778c, 
		0xdc01, 
		0x53a0, 
		0x51c0, 
		0xcf00, 
		0xce80, 
		0x103f, 
		0x53c0, 
		0x4e80, 
		0xbc00, 
		0xb296, 
		0x7ada, 
		0x3919, 
		0xfc00, 
		0xd3c0, 
		0x8000, 
		0x5340, 
		0x607f, 
		0x4e40, 
		0x52a0, 
		0xd0e0, 
		0x0000, 
		0xd160, 
		0x5020, 
		0xcbb9, 
		0xd100, 
		0x4980, 
		0xc800, 
		0x5360, 
		0xd58c, 
		0xd160, 
		0x4400, 
		0xefc5, 
		0x4700, 
		0x53c0, 
		0x8000, 
		0x4e00, 
		0xcd40, 
		0x4dc0, 
		0xc400, 
		0x24e3, 
		0xc200, 
		0xb56d, 
		0x318e, 
		0xd340, 
		0x1c9d, 
		0x8000, 
		0x013f, 
		0x7b68, 
		0xb3ce, 
		0xd380, 
		0x71de, 
		0xd160, 
		0x56e4, 
		0x4b00, 
		0x8000, 
		0xb8dc, 
		0x91ba, 
		0x7c00, 
		0xd020, 
		0x7c00, 
		0xfc00, 
		0x7c00, 
		0x4900, 
		0xc980, 
		0xf943, 
		0xcf80, 
		0x9556, 
		0x4e00, 
		0x5340, 
		0x7c00, 
		0xba48, 
		0x0000, 
		0xf95c, 
		0xc2b0, 
		0x30b7, 
		0xd240, 
		0xd2e0, 
		0xce80, 
		0xbf34, 
		0x8000, 
		0x54d8, 
		0x4400, 
		0x4a00, 
		0x4000, 
		0x52a0, 
		0x4e40, 
		0x8000, 
		0xcf80, 
		0xcdc0, 
		0x3e57, 
		0xd000, 
		0x4e00, 
		0x20b8, 
		0xd3a0, 
		0xbf16, 
		0xd340, 
		0xfc00, 
		0x0000, 
		0x4c40, 
		0x0471, 
		0xd040, 
		0x7c00, 
		0xc500, 
		0x5320, 
		0x3c00, 
		0x5100, 
		0xce80, 
		0x7111, 
		0xba5c, 
		0x2f5e, 
		0x1a58, 
		0x63fe, 
		0x4b00, 
		0xbfad, 
		0x5340, 
		0x5b10, 
		0x53c0, 
		0xd1a0, 
		0x4800, 
		0x5200, 
		0x4f00, 
		0x0000, 
		0x0578, 
		0x53c0, 
		0x4c80, 
		0x4f40, 
		0xc980, 
		0x36c3, 
		0x5260, 
		0xcf00, 
		0xd2c0, 
		0x0a20, 
		0xb1d0, 
		0xcf40, 
		0xd100, 
		0xd2e0, 
		0xd320, 
		0x4fe2, 
		0x5060, 
		0x4b00, 
		0x4e40, 
		0xcfc0, 
		0x5180, 
		0xc880, 
		0x4200, 
		0xd320, 
		0x4e00, 
		0xce00, 
		0xd200, 
		0x4b00, 
		0x2165, 
		0x598d, 
		0xc980, 
		0x4103, 
		0x057f, 
		0xd200, 
		0x700c, 
		0x51c0, 
		0x9f4c, 
		0x52e0, 
		0x5220, 
		0x2adc, 
		0xd180, 
		0x52b4, 
		0x52a0, 
		0x7e5a, 
		0x627b, 
		0xc700, 
		0xce71, 
		0xd100, 
		0xd74c, 
		0x4f80, 
		0x4ec0, 
		0xcb80, 
		0x8237, 
		0x4e40, 
		0xfc00, 
		0xc348, 
		0x4f00, 
		0x5100, 
		0xd0c0, 
		0xbc5b, 
		0x7c00, 
		0x1e20, 
		0x9180, 
		0x7c00, 
		0x5140, 
		0xd240, 
		0x5200, 
		0x7d78, 
		0x6820, 
		0xc400, 
		0xd000, 
		0xfc00, 
		0x4a80, 
		0xcf40, 
		0xd060, 
		0xc761, 
		0xd2e0, 
		0x08ab, 
		0xd2c0, 
		0x06f1, 
		0x5120, 
		0x0000, 
		0x51a0, 
		0x1659, 
		0x0000, 
		0x5100, 
		0x0a66, 
		0x4cc0, 
		0xb031, 
		0x0000, 
		0xcc00, 
		0xd0e0, 
		0x7c3c, 
		0x8000, 
		0x5160, 
		0xcf80, 
		0x4a4d, 
		0xd300, 
		0xfc00, 
		0x4f40, 
		0xa39b, 
		0xde47, 
		0x4f00, 
		0x6511, 
		0xa9cd, 
		0x8000, 
		0x7c00, 
		0x50c0, 
		0x3f78, 
		0x4d40, 
		0xdaff, 
		0xcd80, 
		0x7098, 
		0x638f, 
		0x4200, 
		0xd000, 
		0x8000, 
		0x4c80, 
		0xbccd, 
		0xcfb6, 
		0x5724, 
		0x51a0, 
		0x29ae, 
		0x9d42, 
		0xca00, 
		0x96aa, 
		0x02ff, 
		0x4500, 
		0x7c00, 
		0x52e0, 
		0xcd80, 
		0xc700, 
		0x4400, 
		0x2cd1, 
		0x809f, 
		0xfc90, 
		0xd220, 
		0x4cc0, 
		0x5160, 
		0x4e80, 
		0x4800, 
		0xd320, 
		0x256b, 
		0xb1f8, 
		0x4dbd, 
		0xfc00, 
		0x5380, 
		0xcdc9, 
		0x5340, 
		0xd0a0, 
		0x50a0, 
		0xf30a, 
		0x53e0, 
		0x0000, 
		0xd120, 
		0x8000, 
		0xd57a, 
		0x4a00, 
		0xd140, 
		0x309b, 
		0xd1c0, 
		0xccc0, 
		0x4c12, 
		0x4d00, 
		0x5000, 
		0x5260, 
		0x14ee, 
		0xcfc0, 
		0x4c96, 
		0x4f80, 
		0x8000, 
		0x0000, 
		0x5380, 
		0x4c80, 
		0xc900, 
		0xd3c0, 
		0xd3e0, 
		0x1383, 
		0xd37e, 
		0x3a64, 
		0x0d19, 
		0xca00, 
		0xcdc0, 
		0x52c0, 
		0x356d, 
		0x50a0, 
		0xd060, 
		0x4d00, 
		0x9e9a, 
		0xd3a0, 
		0xd040, 
		0xd220, 
		0xb04e, 
		0x4f00, 
		0xc500, 
		0x4000, 
		0xd320, 
		0x5040, 
		0xb002, 
		0x68cc, 
		0xbc00, 
		0xc500, 
		0x0e81, 
		0x1dd5, 
		0x5320, 
		0x7f2d, 
		0xcb00, 
		0x5100, 
		0xe288, 
		0x4000, 
		0x52c0, 
		0xee45, 
		0x324d, 
		0x0000, 
		0x4800, 
		0x8000, 
		0x8cbc, 
		0xc800, 
		0x4a00, 
		0xcf80, 
		0x5360, 
		0xd470, 
		0x5260, 
		0x1b5f, 
		0xb213, 
		0x2a75, 
		0x1bfa, 
		0x6265, 
		0x4a80, 
		0x455e, 
		0x4400, 
		0xd260, 
		0xcfc0, 
		0x5260, 
		0xc10e, 
		0x4f00, 
		0xccc0, 
		0xcd40, 
		0xfc20, 
		0x5000, 
		0xd1e0, 
		0xab35, 
		0xb249, 
		0xd3e0, 
		0x53e0, 
		0xd060, 
		0xfc00, 
		0xc000, 
		0xc500, 
		0xcf00, 
		0x8000, 
		0x4dc0, 
		0xc000, 
		0x8ec0, 
		0xd100, 
		0xd160, 
		0xc30a, 
		0xcb00, 
		0x4900, 
		0x5140, 
		0x8000, 
		0xd280, 
		0x4f40, 
		0x5260, 
		0x5100, 
		0xf2ce, 
		0x4c00, 
		0xd170, 
		0xd280, 
		0x5040, 
		0xd0e0, 
		0xccc0, 
		0x8000, 
		0xd140, 
		0x8000, 
		0x5060, 
		0xd3c0, 
		0xd160, 
		0xd300, 
		0xcd80, 
		0xf737, 
		0xd000, 
		0xcd40, 
		0x4800, 
		0x5260, 
		0xd220, 
		0xc000, 
		0xfc00, 
		0xd100, 
		0x15a5, 
		0xd120, 
		0x547b, 
		0x4000, 
		0xd1e0, 
		0x992e, 
		0xce40, 
		0x4dc0, 
		0x4d80, 
		0x2b62, 
		0xd94b, 
		0xd3e0, 
		0x4c80, 
		0x4dc0, 
		0x0000, 
		0xc600, 
		0x8000, 
		0xc589, 
		0xc600, 
		0xc980, 
		0x53a0, 
		0xd140, 
		0x5100, 
		0xe26b, 
		0x4f00, 
		0x5100, 
		0x4b00, 
		0x6ec6, 
		0x5100, 
		0x5040, 
		0xd000, 
		0xc76e, 
		0x53c0, 
		0xd000, 
		0xcf80, 
		0x8d6c, 
		0xc000, 
		0xd220, 
		0xd280, 
		0x4c00, 
		0x8000, 
		0x9816, 
		0xd380, 
		0xd140, 
		0xd200, 
		0x7feb, 
		0x53c0, 
		0x5240, 
		0x52c0, 
		0xc700, 
		0xd080, 
		0xd000, 
		0x50c0, 
		0xaa56, 
		0x0000, 
		0xca80, 
		0x8000, 
		0xcc00, 
		0xc999, 
		0xaa5b, 
		0x4700, 
		0xf93e, 
		0x0eba, 
		0xd380, 
		0xd3c0, 
		0xc000, 
		0xd180, 
		0x3c00, 
		0x1fe0, 
		0xd020, 
		0xd340, 
		0x4a53, 
		0x8000, 
		0xf54d, 
		0x5240, 
		0x3aca, 
		0x4600, 
		0x4400, 
		0xd340, 
		0xd2e0, 
		0x5100, 
		0x8000, 
		0xd100, 
		0xce80, 
		0x8000, 
		0x5040, 
		0xf3af, 
		0x4cc0, 
		0xd280, 
		0xc400, 
		0x08c2, 
		0xb58d, 
		0xc4f9, 
		0xc000, 
		0xd0a0, 
		0x50a0, 
		0x4f00, 
		0x86b8, 
		0x53e0, 
		0x3764, 
		0x7ca1, 
		0x8a6f, 
		0x4c40, 
		0xed9c, 
		0xfcd8, 
		0x28a8, 
		0x7c00, 
		0x1bdc, 
		0x834b, 
		0xccc0, 
		0x4d40, 
		0x50c0, 
		0xe48f, 
		0xe75b, 
		0x5040, 
		0x53c0, 
		0x8b85, 
		0x75d2, 
		0x4d40, 
		0x4e80, 
		0x4c2a, 
		0x00e9, 
		0xd2a0, 
		0x4e00, 
		0xd060, 
		0xcb80, 
		0x53a0, 
		0x8000, 
		0x4d00, 
		0xd1c0, 
		0x0000, 
		0x4f00, 
		0x83c1, 
		0xd240, 
		0xd4fe, 
		0x59ac, 
		0xb63f, 
		0x5080, 
		0xd380, 
		0x4e00, 
		0x53e0, 
		0x9f67, 
		0x26b5, 
		0x6367, 
		0x4f00, 
		0xfc00, 
		0x4880, 
		0x4e00, 
		0x4d40, 
		0x4a80, 
		0x3c00, 
		0xc000, 
		0x2f42, 
		0x52e0, 
		0x5180, 
		0xc000, 
		0x52e0, 
		0xdd7d, 
		0x5380, 
		0x4c40, 
		0xd100, 
		0x3d56, 
		0x62bd, 
		0xd3a0, 
		0x4d40, 
		0xd180, 
		0x7ce8, 
		0x2ce3, 
		0xc600, 
		0xf226, 
		0x8ba9, 
		0x4600, 
		0x52e0, 
		0xc000, 
		0xfc00, 
		0xd060, 
		0xca80, 
		0x5238, 
		0x7c00, 
		0xd2a0, 
		0x55d6, 
		0x5060, 
		0x4ec0, 
		0x5360, 
		0xeb4f, 
		0xca00, 
		0xa291, 
		0xcb80, 
		0x5180, 
		0x5755, 
		0x36c9, 
		0x7c00, 
		0xae2f, 
		0xce80, 
		0xce00, 
		0x0000, 
		0xce40, 
		0x4f40, 
		0x53a0, 
		0x930a, 
		0xcd00, 
		0x5020, 
		0x4200, 
		0x8000, 
		0x4f00, 
		0xd120, 
		0x5000, 
		0xd220, 
		0xd0a0, 
		0x4c00, 
		0xd100, 
		0xcb00, 
		0x2ed7, 
		0x5380, 
		0x4a80, 
		0x4f80, 
		0x52c0, 
		0x6b14, 
		0x587b, 
		0x5120, 
		0x1465, 
		0xe2e7, 
		0xd64e, 
		0xcfc0, 
		0x53c0, 
		0x5040, 
		0x4b00, 
		0x0000, 
		0xc700, 
		0x4c80, 
		0xc500, 
		0x4900, 
		0x5380, 
		0x7cb7, 
		0x8000, 
		0xd3e0, 
		0x5c71, 
		0x52a0, 
		0xd1c0, 
		0xcb80, 
		0xd140, 
		0xd360, 
		0xd120, 
		0xfc00, 
		0xd020, 
		0xc500, 
		0x4b00, 
		0xbcba, 
		0x8000, 
		0xfd5a, 
		0xcf80, 
		0xf2f0, 
		0xc89e, 
		0x7441, 
		0xd280, 
		0xc200, 
		0xcd80, 
		0xca80, 
		0x4700, 
		0xdb26, 
		0xc484, 
		0xabd6, 
		0x6cb7, 
		0x8000, 
		0xcb00, 
		0x5360, 
		0x50a0, 
		0x7562, 
		0x51e0, 
		0x2de4, 
		0x0000, 
		0x4e80, 
		0x5100, 
		0xb6f8, 
		0xcd53, 
		0x4900, 
		0xd300, 
		0x9a75, 
		0xcd40, 
		0x09bd, 
		0x0000, 
		0xd200, 
		0x4f80, 
		0x4880, 
		0xce80, 
		0x0000, 
		0xd3a0, 
		0xe6ea, 
		0x5320, 
		0xd380, 
		0x5140, 
		0x424d, 
		0xcf40, 
		0x47e5, 
		0x4600, 
		0x27fc, 
		0xcf58, 
		0x7c00, 
		0x4c00, 
		0x4700, 
		0x5300, 
		0x5240, 
		0x8686, 
		0x5080, 
		0x4b00, 
		0xb53b, 
		0xbc33, 
		0x4800, 
		0x4980, 
		0x571b, 
		0xfc00, 
		0x7fad, 
		0x058c, 
		0x8000, 
		0xc500, 
		0xd879, 
		0x4b00, 
		0x9a14, 
		0xc500, 
		0xea12, 
		0xab3d, 
		0xd260, 
		0x9386, 
		0x2126, 
		0xc000, 
		0xd080, 
		0x5040, 
		0xfc00, 
		0x7f27, 
		0xcc79, 
		0x0000, 
		0x21b1, 
		0xc880, 
		0x5260, 
		0x5340, 
		0x5000, 
		0xd180, 
		0x4900, 
		0xf7ca, 
		0x5923, 
		0xfc00, 
		0xbd55, 
		0xd320, 
		0x0dff, 
		0x5000, 
		0x5260, 
		0x0e0f, 
		0xd3a0, 
		0x5320, 
		0x9999, 
		0xd140, 
		0xfc00, 
		0xd0c0, 
		0xc900, 
		0xecbb, 
		0x8000, 
		0xcd00, 
		0xd360, 
		0x51e0, 
		0xcd80, 
		0xfc00, 
		0xa381, 
		0xd1a0, 
		0x35f3, 
		0xcd40, 
		0x4e80, 
		0xefb1, 
		0xd060, 
		0xce40, 
		0x4c80, 
		0x5200, 
		0x0000, 
		0x4700, 
		0xcfc0, 
		0x8000, 
		0x8000, 
		0xd100, 
		0x5340, 
		0x4ec0, 
		0x9053, 
		0xf4da, 
		0x8000, 
		0x8000, 
		0x3ca9, 
		0x4980, 
		0xca00, 
		0xaaa3, 
		0xe911, 
		0xdda8, 
		0x5040, 
		0x4eb6, 
		0x4ec0, 
		0xb5da, 
		0x0000, 
		0x4f40, 
		0x8000, 
		0x39ec, 
		0xd0e0, 
		0x3e74, 
		0xeb39, 
		0x4a80, 
		0x4d80, 
		0x8000, 
		0x8bcc, 
		0xccc0, 
		0x4a00, 
		0x02ea, 
		0xd1a0, 
		0x7de8, 
		0x5240, 
		0x8000, 
		0xbc00, 
		0x50c0, 
		0x4d80, 
		0xfc00, 
		0x8000, 
		0x556b, 
		0xd100, 
		0x27d3, 
		0xbe52, 
		0xd060, 
		0xd280, 
		0x4880, 
		0xd240, 
		0x77ce, 
		0x4a65, 
		0xcf40, 
		0xc9a3, 
		0xd1e0, 
		0xd0e0, 
		0xcf80, 
		0x4600, 
		0x7c00, 
		0x4e00, 
		0x5060, 
		0x4980, 
		0xcdc0, 
		0xd280, 
		0x8000, 
		0x5100, 
		0x707e, 
		0xf78c, 
		0x5000, 
		0x4400, 
		0xcb00, 
		0x4500, 
		0xd0e0, 
		0xa109, 
		0x4e00, 
		0xc800, 
		0x0000, 
		0xbc7b, 
		0x51c0, 
		0x7f10, 
		0xd360, 
		0x4400, 
		0xd040, 
		0xbc00, 
		0xd59b, 
		0xb133, 
		0xd1c0, 
		0x5020, 
		0x548b, 
		0xc000, 
		0xd340, 
		0x50e0, 
		0xd020, 
		0x78ad, 
		0x0000, 
		0xc800, 
		0xaf61, 
		0xc9ee, 
		0x9da3, 
		0xd2a0, 
		0x87ea, 
		0x2abb, 
		0x5220, 
		0x8000, 
		0xcd40, 
		0x4700, 
		0x5080, 
		0xfb9b, 
		0x6dfe, 
		0x4000, 
		0x7afe, 
		0xd0c0, 
		0xf77e, 
		0xcb00, 
		0x5d69, 
		0x5300, 
		0x4800, 
		0xd0a0, 
		0xc880, 
		0x0000, 
		0xb62c, 
		0xd101, 
		0xd180, 
		0x4700, 
		0x0000, 
		0xcf80, 
		0x4e00, 
		0x03c4, 
		0x51a0, 
		0x3e7a, 
		0xd0c0, 
		0xf11f, 
		0x4000, 
		0x4f80, 
		0xfc00, 
		0x52c0, 
		0x53a0, 
		0x66ef, 
		0x6faf, 
		0x5020, 
		0x4f40, 
		0xa951, 
		0x4500, 
		0x7f11, 
		0x4200, 
		0x5000, 
		0x82e8, 
		0x2449, 
		0xd2c0, 
		0x4d00, 
		0xd060, 
		0xd120, 
		0x4500, 
		0x483c, 
		0xce00, 
		0xe9dd, 
		0xc06d, 
		0x8000, 
		0x4000, 
		0x5320, 
		0x51e0, 
		0xc200, 
		0xc3af  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc145dc70, 
		0x00400000, 
		0x80000000, 
		0x4b8c4b40, 
		0x00100000, 
		0xd69ba319, 
		0xe6522220, 
		0x41700000, 
		0xc2100000, 
		0x0c7ff000, 
		0x80000004, 
		0x0c7fffc0, 
		0x0c7e0000, 
		0x61ede497, 
		0xede43af8, 
		0x00000100, 
		0x41300000, 
		0x80000000, 
		0x80000000, 
		0x0c7e0000, 
		0x2137fc0e, 
		0x42340000, 
		0x0e00000f, 
		0x80000000, 
		0x00002000, 
		0x425c0000, 
		0xc2140000, 
		0x80040000, 
		0xc2700000, 
		0x0c7fff00, 
		0x42500000, 
		0x7b603ad5, 
		0xc1100000, 
		0xaf27020c, 
		0xb76ea19c, 
		0xc2180000, 
		0x00000000, 
		0xfa152998, 
		0x80000000, 
		0x00000000, 
		0xc1300000, 
		0x7fc00000, 
		0x92196c3d, 
		0x00000400, 
		0x80000002, 
		0xc23c0000, 
		0xf32f0681, 
		0x41500000, 
		0x80000000, 
		0xf64f59f6, 
		0x41f80000, 
		0x74a1c2c2, 
		0x41300000, 
		0x99dd2547, 
		0x80000100, 
		0x00000004, 
		0x80800001, 
		0x80000010, 
		0xd40b8a4b, 
		0xff820cbc, 
		0x80002000, 
		0x00800001, 
		0x80800003, 
		0x7f800000, 
		0xffbc6d34, 
		0x0c7fffe0, 
		0x807abf0f, 
		0x425c0000, 
		0xff800001, 
		0x80000000, 
		0x00000010, 
		0x80000001, 
		0x2dfd979c, 
		0x00800002, 
		0x00010000, 
		0x0e0007ff, 
		0x80001000, 
		0xc1b80000, 
		0xc2180000, 
		0x0c7f8000, 
		0x00000000, 
		0x3b02850f, 
		0x41500000, 
		0x00002000, 
		0x27e03cb1, 
		0x2dba5293, 
		0x0c8b208e, 
		0xbf028f5c, 
		0x0387dcef, 
		0x55c357b4, 
		0x42040000, 
		0x00000000, 
		0x8f7fffff, 
		0x0e0007ff, 
		0xec09d982, 
		0x5a734cd7, 
		0x41a00000, 
		0x41c80000, 
		0x80000000, 
		0x383a4f6f, 
		0x0e0000ff, 
		0x7f80dee1, 
		0x80000001, 
		0x7f800000, 
		0xc1c00000, 
		0x80002000, 
		0x007fffff, 
		0x0e00001f, 
		0x930e3f1d, 
		0x0c7ffffc, 
		0x7e44c6a3, 
		0x80000080, 
		0x00000001, 
		0x41a00000, 
		0x8f7fffff, 
		0x80800003, 
		0x0e0007ff, 
		0x804aa96f, 
		0xc1b80000, 
		0x00000004, 
		0x00000000, 
		0x80000010, 
		0x41400000, 
		0x81ae28fe, 
		0x000f9599, 
		0x41900000, 
		0x0c7ffff8, 
		0xff000000, 
		0x41e00000, 
		0xff7ffffe, 
		0x425c0000, 
		0xc39615dd, 
		0x41980000, 
		0x41f00000, 
		0x00743799, 
		0xd67f13b4, 
		0x7f800000, 
		0x0c7ffff0, 
		0x41c80000, 
		0xf499df85, 
		0x0f7ffffe, 
		0xc2540000, 
		0x78064e68, 
		0x239269a5, 
		0x24ec5503, 
		0xc2040000, 
		0x17f86261, 
		0xc2780000, 
		0x2b814f21, 
		0xc1f00000, 
		0x0e0001ff, 
		0xd87eeaf7, 
		0x9c79569d, 
		0x7f800000, 
		0xc2280000, 
		0xb5a611fc, 
		0x48fb5f7c, 
		0x41880000, 
		0x0c7f8000, 
		0x80100000, 
		0x992624f4, 
		0x0e0003ff, 
		0x0e0000ff, 
		0x8216a5bf, 
		0x0f9e1cb5, 
		0x504fc4e0, 
		0x81435781, 
		0x80000000, 
		0xff7fffff, 
		0xd60ce57b, 
		0x80002000, 
		0x1c475acd, 
		0x41c80000, 
		0x80008000, 
		0x00001000, 
		0x444ee176, 
		0x19f4d7fb, 
		0xc2280000, 
		0x00000000, 
		0xc1800000, 
		0x00000004, 
		0x00800002, 
		0x79801980, 
		0x0c7fe000, 
		0x41d80000, 
		0x41f00000, 
		0xdacaa205, 
		0xc25c0000, 
		0x0e7fffff, 
		0x00400000, 
		0x84663f12, 
		0x00400000, 
		0x42200000, 
		0xff800000, 
		0x80000002, 
		0x00000001, 
		0x807ffffe, 
		0x15cffcc7, 
		0x80000000, 
		0x41f00000, 
		0x52ef37e0, 
		0x7f800000, 
		0x15eee5cb, 
		0x80000000, 
		0x427c0000, 
		0x46b40701, 
		0x00000020, 
		0x80ffffff, 
		0xffffffff, 
		0x50dd5198, 
		0x7f7fffff, 
		0x42180000, 
		0x00800000, 
		0xff7ffffe, 
		0xc70f2a2d, 
		0x0e3fffff, 
		0x6e0b7768, 
		0x5b9f63af, 
		0x0e07ffff, 
		0x30f1602b, 
		0xc1f00000, 
		0x80000000, 
		0xc2740000, 
		0xc1200000, 
		0x7f800000, 
		0x59606ca9, 
		0x00080000, 
		0xa9a84397, 
		0xaa8ea749, 
		0x57b2468b, 
		0x80800001, 
		0x0c400000, 
		0x1d5227a5, 
		0x80800003, 
		0xffa048f2, 
		0x00000000, 
		0x80002000, 
		0x00000000, 
		0x0e1fffff, 
		0x00010000, 
		0x80011111, 
		0xaaaaaaaa, 
		0x36c281be, 
		0xc1a00000, 
		0x4fa07cdf, 
		0xc2740000, 
		0x00000000, 
		0x9e2390c5, 
		0x53799e36, 
		0x80020000, 
		0x066eb2b4, 
		0x80002000, 
		0x42600000, 
		0x41700000, 
		0x0c7f0000, 
		0xc1500000, 
		0xbe2bfd38, 
		0x80000000, 
		0xee4a17e5, 
		0xffc00001, 
		0xc1c80000, 
		0x0c7ffe00, 
		0x0d000ff0, 
		0x40400000, 
		0xa5c510cd, 
		0xb9f75978, 
		0xff800000, 
		0x80011111, 
		0x3878d608, 
		0x80000000, 
		0xf5875034, 
		0x002daecb, 
		0x7f800000, 
		0x00020000, 
		0xffc8dd36, 
		0x80000000, 
		0x41800000, 
		0x7f7ffffe, 
		0xcfd7706d, 
		0x41d00000, 
		0x80000080, 
		0xc1b80000, 
		0x1d75b5db, 
		0x00000020, 
		0x60db17ac, 
		0x00000000, 
		0xc2780000, 
		0x40c00000, 
		0xcfd22ddd, 
		0x0c7fffe0, 
		0x42080000, 
		0x00000200, 
		0x80800002, 
		0x0e07ffff, 
		0x00000008, 
		0x80800003, 
		0x7f800000, 
		0x7f800000, 
		0x7f884bf4, 
		0x4356f892, 
		0xd9b08900, 
		0x426c0000, 
		0x42200000, 
		0x7d17e594, 
		0xa6b27c92, 
		0x00239bf5, 
		0xaaaaaaaa, 
		0x4b8c4b40, 
		0x527130ea, 
		0x80800003, 
		0x00020000, 
		0x69ae54b9, 
		0xc1600000, 
		0x41b80000, 
		0x42240000, 
		0x19200956, 
		0x80000000, 
		0x00002000, 
		0x00004000, 
		0xbf800000, 
		0x00100000, 
		0xc27c0000, 
		0x00104f67, 
		0x2f19742c, 
		0x7fa4b7f2, 
		0x00000008, 
		0xc2280000, 
		0x42200000, 
		0x41200000, 
		0x00000000, 
		0x7f800000, 
		0x42700000, 
		0x00000040, 
		0xff800000, 
		0x0c7e0000, 
		0x0c7fffc0, 
		0xa4247131, 
		0xc2400000, 
		0x80800000, 
		0xc1900000, 
		0x40c00000, 
		0x00000008, 
		0xffa9d4ab, 
		0x95466660, 
		0xb19e7de7, 
		0xcb000000, 
		0xff000000, 
		0xcccccccc, 
		0x80800003, 
		0xfde55b68, 
		0x41a80000, 
		0x0c780000, 
		0xc2380000, 
		0x00000000, 
		0x7f800000, 
		0x00800000, 
		0xff7ffffe, 
		0x7cf07d81, 
		0x002f0411, 
		0xffda9c43, 
		0xc1d00000, 
		0x00000000, 
		0x0e0001ff, 
		0xffbfcee1, 
		0x0c7ffe00, 
		0x80000000, 
		0x268a22a6, 
		0x80004000, 
		0xc2440000, 
		0xc0400000, 
		0x00000000, 
		0xb32641d0, 
		0x7f800000, 
		0x2e512c83, 
		0x32cebbcb, 
		0x80020000, 
		0x10d60b59, 
		0xc2500000, 
		0x3f800000, 
		0xffc00001, 
		0xca1503d5, 
		0xc2680000, 
		0x41880000, 
		0x80800000, 
		0x7f800000, 
		0x41c00000, 
		0xcb8c4b40, 
		0x0c7f0000, 
		0x050dbae2, 
		0x80001000, 
		0xc2240000, 
		0xff800000, 
		0x0c400000, 
		0xc1b00000, 
		0x80000000, 
		0x80800001, 
		0x230d6e75, 
		0x7f800001, 
		0xe486983f, 
		0x80000000, 
		0xb30cfe5c, 
		0x40000000, 
		0xc1e80000, 
		0xc25c0000, 
		0x0c7fffe0, 
		0x4aa40bd9, 
		0x42580000, 
		0x0d00fff0, 
		0xffffffff, 
		0x00000080, 
		0xd2b425ed, 
		0x3f028f5c, 
		0x1d8bb43f, 
		0xad0cd350, 
		0xc20c0000, 
		0xc20c0000, 
		0x10d7522c, 
		0x1ab01aa4, 
		0x425c0000, 
		0x00000800, 
		0x80000001, 
		0x3cc5adab, 
		0x00004000, 
		0x41880000, 
		0x596c94cc, 
		0x00800000, 
		0x80011111, 
		0x80800000, 
		0xc1d80000, 
		0x80000001, 
		0xf56af326, 
		0xf03f9397, 
		0x6dc03b0c, 
		0x8659e6f7, 
		0x5b7584d1, 
		0xed0c2d66, 
		0x00100000, 
		0x7fb3afa2, 
		0x42780000, 
		0x00400000, 
		0x0e00001f, 
		0x00000000, 
		0xc1200000, 
		0xc1400000, 
		0xc2400000, 
		0x0e00003f, 
		0x00000000, 
		0xc79e2099, 
		0xc2140000, 
		0xc1400000, 
		0x41e80000, 
		0x807fffff, 
		0x00ffffff, 
		0x0e000003, 
		0x42d432c0, 
		0xbfc3d97a, 
		0x589b6547, 
		0xc1a00000, 
		0x42500000, 
		0x00000000, 
		0xc0e00000, 
		0x00400000, 
		0x00000000, 
		0x3f800000, 
		0x23ce61c8, 
		0xc2180000, 
		0xffffffff, 
		0xc27c0000, 
		0xf95bc236, 
		0x420c0000, 
		0x007ffffe, 
		0x7f800001, 
		0xc1880000, 
		0x0f7ffffc, 
		0x0c7ffff8, 
		0xc1300000, 
		0x41d00000, 
		0x42680000, 
		0x00004000, 
		0x8f7ffffd, 
		0xcccccccc, 
		0xffffffff, 
		0x437b42a1, 
		0x0e007fff, 
		0x80000000, 
		0xaaaaaaaa, 
		0x80800000, 
		0x80800003, 
		0x00400000, 
		0x76d89ba4, 
		0x80800001, 
		0x41900000, 
		0xe44f679b, 
		0xc0000000, 
		0x0c7c0000, 
		0x760d4794, 
		0xc1900000, 
		0x807ffffe, 
		0x80008000, 
		0x80080000, 
		0x41600000, 
		0x0e0001ff, 
		0x248d56fb, 
		0x1ee5c239, 
		0x41600000, 
		0x80008000, 
		0xee19d79a, 
		0x7f000000, 
		0x42540000, 
		0x00000000, 
		0x635d1af7, 
		0x80000000, 
		0xbdfe367f, 
		0xc27c0000, 
		0xb0e61c2d, 
		0xff800000, 
		0xff800000, 
		0xc23c0000, 
		0x4b8c4b40, 
		0xb4350f88, 
		0x42640000, 
		0xc1700000, 
		0x0c7fc000, 
		0x0f7ffffd, 
		0x80020000, 
		0xff7ffffe, 
		0xfc8478af, 
		0xc2180000, 
		0xc1880000, 
		0x80000001, 
		0x00800002, 
		0x804d9d45, 
		0x0e00ffff, 
		0x0c7fffe0, 
		0xc2440000, 
		0xc2700000, 
		0x00800000, 
		0x00000040, 
		0xed5a0677, 
		0x00800001, 
		0x07709b8a, 
		0xc26c0000, 
		0xc0bd60e5, 
		0x0f7ffffe, 
		0xff800001, 
		0x80800003, 
		0xb7ade5ec, 
		0x80000400, 
		0x0d000ff0, 
		0xc1b00000, 
		0xff800000, 
		0x3d3f0dce, 
		0x80080000, 
		0xbf028f5c, 
		0x803a4feb, 
		0x8f7ffffd, 
		0x50dbb061, 
		0xc20c0000, 
		0x1c9d02ce, 
		0x40e00000, 
		0x7f800000, 
		0x41d80000, 
		0xea42dcf6, 
		0xc2200000, 
		0xc1800000, 
		0x7f800000, 
		0xc27c0000, 
		0x40a00000, 
		0xebf471a7, 
		0x0c7c0000, 
		0x42580000, 
		0x7f7ffffe, 
		0x80011111, 
		0x42600000, 
		0x80000080, 
		0xc2200000, 
		0x80000001, 
		0xff7fffff, 
		0xfc48b410, 
		0x00008000, 
		0xc1700000, 
		0x00000400, 
		0x426c0000, 
		0xc1c00000, 
		0xc2600000, 
		0xc25c0000, 
		0xd62e4790, 
		0x7f800000, 
		0x00000002, 
		0x0e0000ff, 
		0xff85b160, 
		0x7f7ffffe, 
		0x80000002, 
		0x0c7fffe0, 
		0x00000004, 
		0xc1200000, 
		0xff7ffffe, 
		0x80000100, 
		0xc1880000, 
		0x80800002, 
		0x41e80000, 
		0x41500000, 
		0x7f800000, 
		0xc1e80000, 
		0x1653799a, 
		0x80734e3b, 
		0x0c7f8000, 
		0xb3b3a40e, 
		0x41400000, 
		0x0c600000, 
		0x7f800000, 
		0x80000000, 
		0x80011111, 
		0xe0167a9a, 
		0xc2180000, 
		0x00000000, 
		0x80000000, 
		0x0c7ffffe, 
		0x40c16b61, 
		0x00004000, 
		0x87232fb2, 
		0x42400000, 
		0x0e000001, 
		0x7f800000, 
		0xc1c80000, 
		0x41980000, 
		0x00000008, 
		0xc1a00000, 
		0x80000001, 
		0x80000000, 
		0x00000000, 
		0x0c7ffffe, 
		0x8e7e57c5, 
		0x7f800000, 
		0x4b000000, 
		0x0c7ffffc, 
		0x61cd1ba4, 
		0xffbfffff, 
		0xc0e00000, 
		0x3f028f5c, 
		0x00001000, 
		0x80000000, 
		0x41e00000, 
		0x00020000, 
		0x00000100, 
		0x425c0000, 
		0xc0c00000, 
		0x80800002, 
		0x80000100, 
		0x41b00000, 
		0x80000002, 
		0x41500000, 
		0x807ffffe, 
		0xcc651d8f, 
		0xc4877f59, 
		0x80000000, 
		0x00000000, 
		0x007ffffe, 
		0x4d20d95f, 
		0x80800000, 
		0x00000080, 
		0x39fa1e06, 
		0x41000000, 
		0x80001000, 
		0x42700000, 
		0x0c7c0000, 
		0x0d00fff0, 
		0xcb000000, 
		0xd56a4ceb, 
		0x42040000, 
		0xff7fffff, 
		0xffc00001, 
		0x7f800000, 
		0x396b154e, 
		0xc2680000, 
		0x41900000, 
		0xc1d80000, 
		0x80ffffff, 
		0x0c7fffe0, 
		0x41200000, 
		0x80000002, 
		0x00800001, 
		0x8982eb60, 
		0x03e7e78b, 
		0xc8fd1bdf, 
		0xffb20df4, 
		0x8f7ffffe, 
		0x0e00ffff, 
		0x556531a1, 
		0xc2500000, 
		0x427c0000, 
		0x91fd7963, 
		0x8b84354d, 
		0x0d00fff0, 
		0x41800000, 
		0xc2240000, 
		0x40c00000, 
		0x80800003, 
		0x3f028f5c, 
		0xc2340000, 
		0x42000000, 
		0xaaaaaaaa, 
		0x42700000, 
		0xbf028f5c, 
		0x7f800000, 
		0x8049fab7, 
		0x00000040, 
		0x0f7fffff, 
		0xffc20bd7, 
		0x00800003, 
		0x7bc130ff, 
		0xc0400000, 
		0x00800002, 
		0x41100000, 
		0x00000002, 
		0x00002000, 
		0x80800001, 
		0xc1d80000, 
		0xcb000000, 
		0x3f05d90b, 
		0x16181169, 
		0x7fc00001, 
		0x00800003, 
		0x80000000, 
		0xc1f00000, 
		0x80000000, 
		0x7f7ffffe, 
		0x3705938d, 
		0xff800000, 
		0x6634117f, 
		0xff8a6f47, 
		0x00000008, 
		0xcb8c4b40, 
		0x422c0000, 
		0x807fffff, 
		0x5e860560, 
		0x80000001, 
		0x42780000, 
		0xff7ffffe, 
		0xbf800001, 
		0xc2680000, 
		0xc2540000, 
		0xc27c0000, 
		0xc1a80000, 
		0x80000000, 
		0x7f800001, 
		0xff418cad, 
		0xc1f80000, 
		0x80000010, 
		0x80000000, 
		0xc1f80000, 
		0xff800000, 
		0x00000800, 
		0xee4fc076, 
		0x00000008, 
		0x0f7ffffd, 
		0x00000008, 
		0xcce9f33b, 
		0x00400000, 
		0x474e8f47, 
		0xc2080000, 
		0xbff3d10d, 
		0x0c7fffc0, 
		0xc2740000, 
		0x8ac36a85, 
		0x469a26f6, 
		0xc1600000, 
		0xa39ce73f, 
		0x9d8183c5, 
		0x80011111, 
		0x00800000, 
		0x002f4f0e, 
		0x00800001, 
		0x80010000, 
		0xc2400000, 
		0x00000400, 
		0x0e1fffff, 
		0xffffffff, 
		0x00000002, 
		0x42600000, 
		0x0e00001f, 
		0x3aa1c744, 
		0xc4296cd3, 
		0x9a878877, 
		0x80080000, 
		0x68d2f325, 
		0xc1f00000, 
		0x7f800000, 
		0x08805809, 
		0xff800000, 
		0x42300000, 
		0x00000000, 
		0x40a00000, 
		0xc1500000, 
		0x41400000, 
		0x80020000, 
		0x52999ee0, 
		0xff800000, 
		0xc2280000, 
		0x00000000, 
		0x8cda0aaf, 
		0x80000000, 
		0x00011111, 
		0x00000000, 
		0x80000000, 
		0x007fffff, 
		0xe041a118, 
		0x80000000, 
		0x26868d1a, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x81e8eb25, 
		0x7f800000, 
		0x6ebd104a, 
		0x00400000, 
		0x41300000, 
		0x2c25ba85, 
		0x0c7ff000, 
		0xff800000, 
		0xc1f80000, 
		0xc88dbc72, 
		0x7fdc2eef, 
		0x18413b76, 
		0xc2740000, 
		0x42140000, 
		0x4dcaab63, 
		0x80010000, 
		0x424c0000, 
		0xc2680000, 
		0x00080000, 
		0x00800001, 
		0x0c600000, 
		0x42577485, 
		0xae68884a, 
		0x41980000, 
		0x53dafdd0, 
		0x75e172da, 
		0xc2180000, 
		0xffac8ae6, 
		0x0c7fff80, 
		0x4b8c4b40, 
		0xef1ea328, 
		0x716435a6, 
		0x41500000, 
		0x7bb6168d, 
		0x0c780000, 
		0x41980000, 
		0x0c7fff80, 
		0x41800000, 
		0xbf800001, 
		0x6e3e2b6d, 
		0xc1100000, 
		0x4b8c4b40, 
		0x69306641, 
		0xc2440000, 
		0x7f800000, 
		0xc1600000, 
		0x7f800000, 
		0x7fbfffff, 
		0x003b1b6f, 
		0x8b297ee9, 
		0xbf028f5c, 
		0x0e00003f, 
		0x10db7872, 
		0x80000000, 
		0x80000001, 
		0x7f800000, 
		0xccd124a0, 
		0xc20c0000, 
		0x007fffff, 
		0x42240000, 
		0xffa1a55d, 
		0xa838dd28, 
		0x0e0000ff, 
		0xdcbf4d21, 
		0xc2300000, 
		0x423c0000, 
		0x3d5842db, 
		0x4160a7fb, 
		0x00400000, 
		0x40e00000, 
		0x80000000, 
		0x7f800000, 
		0x41d00000, 
		0x00000000, 
		0x5b6c74bd, 
		0xcb000000, 
		0x427cd7b7, 
		0x426c0000, 
		0x653c6c99, 
		0x00011111, 
		0x80470986, 
		0x0e000001, 
		0x80000000, 
		0x49035c28, 
		0x40800000, 
		0xff800000, 
		0x33333333, 
		0xff800000, 
		0xc31bdac7, 
		0xeb74ee1d, 
		0x00000000, 
		0xc0a00000, 
		0x423c0000, 
		0x84b48335, 
		0xc27c0000, 
		0xf83b18ac, 
		0x9599f87b, 
		0x62cbb446, 
		0x4dc37218, 
		0xc2180000, 
		0x0e003fff, 
		0x80000080, 
		0xc634d40b, 
		0x0f7fffff, 
		0x4b000000, 
		0x0e0fffff, 
		0xc2700000, 
		0x42540000, 
		0xfe8ddd11, 
		0x80000020, 
		0x80800000, 
		0x7fbfffff, 
		0x80040000, 
		0xc2340000, 
		0x80008000, 
		0x550de3de, 
		0x41b80000, 
		0x42340000, 
		0x00000008, 
		0x806570b8, 
		0xf0f59e4b, 
		0x0c7e0000, 
		0xa107a210, 
		0x80010000, 
		0xba6f4010, 
		0x42300000, 
		0xaae5f5c8, 
		0x41300000, 
		0x00000400, 
		0x80001000, 
		0x00800001, 
		0x0e000001, 
		0x0e003fff, 
		0x0e000003, 
		0x900024de, 
		0x055d6024, 
		0x0c7ff000, 
		0x7f7ffffe, 
		0x007ffffe, 
		0x80080000, 
		0x80000200, 
		0x0fbc85fa, 
		0xd7e39bc0, 
		0xc0800000, 
		0x42500000, 
		0x00000000, 
		0xc1500000, 
		0x8f7ffffd, 
		0x80800001, 
		0xd83a823a, 
		0x00750d05, 
		0x80000000, 
		0x00020000, 
		0xc2780000, 
		0x8773eaeb, 
		0xc4c38a11, 
		0x0e07ffff, 
		0x88b271ce, 
		0x00000000, 
		0x41000000, 
		0xebc6ce52, 
		0x006f993d, 
		0x7f80de18, 
		0x7fc00001, 
		0x7fa3568c, 
		0x6de69142, 
		0x00000000, 
		0x00000000, 
		0x424c0000, 
		0xbf028f5c, 
		0xc2000000, 
		0x0e007fff, 
		0x0c7fffff, 
		0x4c9a021f, 
		0x1b3d38d8, 
		0x00000000, 
		0x00100000, 
		0x00000000, 
		0x0a6f2019, 
		0x00011111, 
		0xd893d96d, 
		0x0e00001f, 
		0x00000000, 
		0x2d0d4dee, 
		0xc2780000, 
		0xc1d80000, 
		0x00ffffff, 
		0x6ce4aebf, 
		0x80000000, 
		0x80000001, 
		0x5ef73316, 
		0x00000200, 
		0x0c400000, 
		0x7ff20038, 
		0xc2440000, 
		0xc180ddaf, 
		0x00000000, 
		0xe3c21135, 
		0x0e00007f, 
		0x195158dc, 
		0xc1a80000, 
		0x00000000, 
		0xc1500000, 
		0xc1e80000, 
		0xc2200000, 
		0x3ec409b9, 
		0x0f7ffffe, 
		0x41880000, 
		0x649c8363, 
		0xffbe340b, 
		0xab7cdec0, 
		0x80000000, 
		0x42280000, 
		0x7a1d5151, 
		0xd79dce99, 
		0x9d88c089, 
		0x42680000, 
		0x7fa4b36c, 
		0x0e000007, 
		0xc0a00000, 
		0x7f800000, 
		0x0e000003, 
		0x42300000, 
		0x001db56c, 
		0xc26c0000, 
		0x7f800000, 
		0x2f81b6f4, 
		0x40e00000, 
		0x80080000, 
		0x807ffffe, 
		0x00000000, 
		0x0c7fffe0, 
		0xc2040000, 
		0xc1600000, 
		0xb9f9063d, 
		0x4b8c4b40, 
		0xc25c0000, 
		0x80000000, 
		0x80000001, 
		0xcc32d915, 
		0x2936eb8f, 
		0x00ffffff, 
		0xc0800000, 
		0x80000004, 
		0xc1000000, 
		0x5d1bf5ce, 
		0xff95fe1c, 
		0x0c7f0000, 
		0x80011111, 
		0xee4971de, 
		0x00000800, 
		0x00ffffff, 
		0x0c7fffff, 
		0x7f800001, 
		0xc1c80000, 
		0x00386d75, 
		0x42400000, 
		0x424c0000, 
		0x913afb59, 
		0x7f7ffffe, 
		0xc2100000, 
		0x807ffffe, 
		0x00800001, 
		0x2e56a13c, 
		0x00000080, 
		0x00800000, 
		0x0c7fc000, 
		0x0c7f0000, 
		0xb4ad6000, 
		0xc2180000, 
		0x80000000, 
		0x75dccf32, 
		0x80900a5f, 
		0x41500000, 
		0x42480000, 
		0x0e00007f, 
		0x42340000, 
		0x00000100, 
		0x00400000, 
		0x00000800, 
		0x0c7ffffe, 
		0x0c7ff800, 
		0x00000020, 
		0x6268a593, 
		0xc1300000, 
		0x00000000, 
		0x80000040, 
		0x27b9f367, 
		0x7f7ffffe, 
		0xc33fe208, 
		0x41a00000, 
		0x41a80000, 
		0x0c7ffe00, 
		0x9e0321d5, 
		0xc2000000, 
		0x157c28cc, 
		0x0c700000, 
		0x396afe3f, 
		0x42240000, 
		0x80100000, 
		0x42180000, 
		0xff7ffffe, 
		0xcb000000, 
		0x36d8f820, 
		0x0c7c0000, 
		0x00008000, 
		0xd83f4f12, 
		0x8412a3c1, 
		0x00800000, 
		0xc2480000, 
		0x5c30e97d, 
		0x0e0000ff, 
		0x955afa19, 
		0x0e0003ff, 
		0x41b00000, 
		0x80800003, 
		0xff7ffffe, 
		0x0e000007, 
		0x0c7fffc0, 
		0x80800003, 
		0xc83011af, 
		0x7fc00000, 
		0x40a00000, 
		0x42000000, 
		0xcfd8addc, 
		0x0e000007, 
		0x0e7fffff, 
		0x17469450, 
		0xffffffff, 
		0x0c7ffff0, 
		0x00000040, 
		0x86f0973e, 
		0x22ce7258, 
		0x6bbce4d4, 
		0xc1300000, 
		0x68a5a5ff, 
		0x00400000, 
		0x80000400, 
		0x0e0001ff, 
		0xc9b287ad, 
		0x80000080, 
		0x3604028b, 
		0x0c7f0000, 
		0x1a789c64, 
		0x7f2f6fbd, 
		0xc0e00000, 
		0x00020000, 
		0x4b7a2c00, 
		0xc1c00000, 
		0x41900000, 
		0xc2000000, 
		0xdd14f5ae, 
		0x80080000, 
		0x41a80000, 
		0xd0ea2ca9, 
		0xc2440000, 
		0xc2340000, 
		0x00000080, 
		0x8f7ffffd, 
		0x0c7fffc0, 
		0x7fffffff, 
		0x42140000, 
		0x002859d9, 
		0x2b54835b, 
		0x80000000, 
		0x80200000, 
		0x6cc73ff4, 
		0x7fc00001, 
		0x315945fb, 
		0xc2400000, 
		0x42380000, 
		0x080c233d, 
		0x00800003, 
		0x80001000, 
		0x00200000, 
		0x0e003fff, 
		0x80000000, 
		0x00800001, 
		0x64a0f80e, 
		0x80000020, 
		0x00724d7f, 
		0x80000800, 
		0x0c7fffc0, 
		0x00000010, 
		0xaaaaaaaa, 
		0x41800000, 
		0x7f800000, 
		0xbfa41c96, 
		0x00000000, 
		0x7774a75f, 
		0xbfd3f00d, 
		0x3582283b, 
		0x0e1fffff, 
		0xc2280000, 
		0x55555555, 
		0x00000020, 
		0x0e00001f, 
		0x7f000000, 
		0x0e0007ff, 
		0x80000000, 
		0x0c7fffff, 
		0x80000000, 
		0x7916d279, 
		0x88ddcde0, 
		0xdc8cfa30, 
		0xadf615d6, 
		0xb53c811a, 
		0xfe07715f, 
		0x0e00007f, 
		0xb08ed3bb, 
		0x00080000, 
		0x80010000, 
		0x7b30dc49, 
		0x00008000, 
		0x80000000, 
		0x73c3f6ad, 
		0x0e001fff, 
		0x805cb3b8, 
		0x0e0fffff, 
		0x2654b285, 
		0xc1a00000, 
		0xc2040000, 
		0x80008000, 
		0xc2340000, 
		0x80200000, 
		0xff7ffffe, 
		0x36f19209, 
		0x00800001, 
		0xc1800000, 
		0xc1980000, 
		0x00000400, 
		0x27f306ee, 
		0x7f800000, 
		0xc26c0000, 
		0x41600000, 
		0x2aea39c1, 
		0x7fc00001, 
		0xc1000000, 
		0xd8bb47bb, 
		0x00000000, 
		0xd3c07849, 
		0xffac0136, 
		0x0e03ffff, 
		0x41880000, 
		0x80800003, 
		0xc1a80000, 
		0x42000000, 
		0x7f800001, 
		0x7fc00001, 
		0x4bd62116, 
		0x0c7fffc0, 
		0x42780000, 
		0xc1e80000, 
		0x41f00000, 
		0xc2280000, 
		0x00000000, 
		0x7f90e987, 
		0x0c400000, 
		0x7f800000, 
		0x42380000, 
		0x7f800000, 
		0x0f7fffff, 
		0x7f88f30f, 
		0x007fffff, 
		0x80000080, 
		0xaaaaaaaa, 
		0xcd9ea16a, 
		0x7f800000, 
		0x424c0000, 
		0xffffffff, 
		0x42780000, 
		0xc2100000, 
		0x7f800000, 
		0xff800000, 
		0xd2b3cd57, 
		0xf51b3e67, 
		0x80000000, 
		0x7f9da2d0, 
		0x3ed79a6f, 
		0xc1100000, 
		0x472ae51d, 
		0x80000000, 
		0x80011111, 
		0x41a00000, 
		0x80800000, 
		0x41f80000, 
		0x25286440, 
		0x80800000, 
		0x7f800000, 
		0xc1100000, 
		0xffc00001, 
		0x42380000, 
		0x7f800000, 
		0xc2180000, 
		0xc1100000, 
		0x37938e05, 
		0x7f800000, 
		0x80800000, 
		0x0c7fc000, 
		0xe84e67b4, 
		0x41700000, 
		0x0c7fff80, 
		0x77b9837e, 
		0x29406ec4, 
		0xc1980000, 
		0xa31bc72a, 
		0xc424939d, 
		0xf27a7827, 
		0x80002000, 
		0x80800003, 
		0xf354272d, 
		0xc1980000, 
		0x0e3fffff, 
		0x80000000, 
		0x6abddc6d, 
		0x42000000, 
		0xe9024f2a, 
		0xc0800000, 
		0xc25c0000, 
		0x8ff46748, 
		0xe74f076a, 
		0x9396be2f, 
		0x87c8d3ca, 
		0x007ffffe, 
		0xc2040000, 
		0x7f871c7a, 
		0x00000000, 
		0x415f4ec8, 
		0xa8368a5d, 
		0x504fb7c7, 
		0x805680aa, 
		0xa12423f7, 
		0x00800000, 
		0x3f50b344, 
		0x80000000, 
		0x00800000, 
		0x137ea5b5, 
		0xe950985d, 
		0x7f800000, 
		0x40400000, 
		0xc407bc09, 
		0x80000200, 
		0x41d00000, 
		0x0c7e0000, 
		0x0d00fff0, 
		0x0c7fff00, 
		0x80000000, 
		0x41a80000, 
		0x6769dd85, 
		0x80800001, 
		0xadb0dfcc, 
		0x00040000, 
		0xc1000000, 
		0x80000000, 
		0x00000000, 
		0x41c00000, 
		0x0c7fffff, 
		0x80000000, 
		0xdceacac0, 
		0x00184f35, 
		0x0e03ffff, 
		0x00000000, 
		0xc1b80000, 
		0xd5b3a906, 
		0x0e01ffff, 
		0x424c0000, 
		0x41c00000, 
		0x7f800000, 
		0xff07ad2b, 
		0x0e03ffff, 
		0x00000080, 
		0x80000001, 
		0xb97d072a, 
		0xc1000000, 
		0x2b328874, 
		0x5e8ff94e, 
		0x00000400, 
		0x0e0007ff, 
		0x42500000, 
		0x7f800000, 
		0x0016c22e, 
		0x298760db, 
		0x41c00000, 
		0x41600000, 
		0x80000010, 
		0xad51c4fe, 
		0x00000000, 
		0x80000000, 
		0xcccccccc, 
		0x00800000, 
		0xc23c0000, 
		0x80000001, 
		0x80100000, 
		0x996747be, 
		0x41600000, 
		0x00000200, 
		0xeaef5f89, 
		0x89ae64f5, 
		0x00800001, 
		0x4b000000, 
		0xe69e4485, 
		0x80800000, 
		0x41100000, 
		0xabf4a647, 
		0xb2ad283e, 
		0x2397769b, 
		0x42040000, 
		0x7feb49eb, 
		0xc2040000, 
		0x0c7ffe00, 
		0xdc7f7c1d, 
		0x80000040, 
		0xff800000, 
		0x0d000ff0, 
		0x2a78662f, 
		0x0c7ffffc, 
		0x7f800001, 
		0xbb964c36, 
		0xff000000, 
		0x0c400000, 
		0x41200000, 
		0x40800000, 
		0x0c7ffff8, 
		0x424c0000, 
		0x7d4bfa8b, 
		0x7f800000, 
		0x648786c6, 
		0xc1b80000, 
		0x69835d82, 
		0x41800000, 
		0xc2100000, 
		0x00010000, 
		0x0c7ffe00, 
		0x0c7ffffc, 
		0x41880000, 
		0x42080000, 
		0x80000000, 
		0x42200000, 
		0x7f000000, 
		0x00004000, 
		0x80800000, 
		0x8d0e6522, 
		0x807fffff, 
		0x3f028f5c, 
		0xff7ffffe, 
		0x89695595, 
		0x22a8e8c1, 
		0x42540000, 
		0x00004000, 
		0xd9374ac2, 
		0xc1500000, 
		0x0e812a0b, 
		0x00000040, 
		0x807ffffe, 
		0x80000001, 
		0x164f08a7, 
		0x80800001, 
		0x0c7fffc0, 
		0x80000000, 
		0x80000800, 
		0x5896813b, 
		0xc1d80000, 
		0x0e1fffff, 
		0xc2400000, 
		0x42d52910, 
		0xff800001, 
		0x00000800, 
		0x42140000, 
		0x446dd3a8, 
		0x41c00000, 
		0xff800000, 
		0x9872d725, 
		0x80002000, 
		0x7627f0ae, 
		0x0e003fff, 
		0xff7ffffe, 
		0x3f800001, 
		0x00800003, 
		0x80800001, 
		0x42640000, 
		0x0e3fffff, 
		0xba6d405e, 
		0xc1500000, 
		0x027079c4, 
		0x8a3c0c06, 
		0xc1880000, 
		0x00000000, 
		0x41880000, 
		0x00100000, 
		0x510586b7, 
		0xc2440000, 
		0x0e003fff, 
		0x00000000, 
		0x0e000001, 
		0x6d75a94e, 
		0x4ea8e165, 
		0x8ab747b0, 
		0xce161675, 
		0xcccccccc, 
		0xc1a00000, 
		0x41d80000, 
		0x7f800000, 
		0x00800001, 
		0x807fffff, 
		0xc1100000, 
		0x7f7ffffe, 
		0x7fbd89f5, 
		0x42280000, 
		0x00000000, 
		0x4b000000, 
		0x7fc00001, 
		0x4f7987b6, 
		0x80000000, 
		0x7fc00000, 
		0x0c7ff000, 
		0xef376b1f, 
		0x42580000, 
		0x806d3d2d, 
		0x0c400000, 
		0xff7ffffe, 
		0x93a17b70, 
		0x41200000, 
		0x0e7fffff, 
		0xd9092ecc, 
		0x00000080  
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
		0x00000035, 
		0x000002e1, 
		0x000003b7, 
		0x0000005b, 
		0x0000020d, 
		0x0000030b, 
		0x00000019, 
		0x00000271, 
		0x000001cb, 
		0x0000019d, 
		0x000001a9, 
		0x000002a1, 
		0x0000014d, 
		0x000000e9, 
		0x0000034b, 
		0x00000355, 
		0x00000399, 
		0x000003af, 
		0x00000339, 
		0x000002a9, 
		0x0000027d, 
		0x0000029b, 
		0x000003e3, 
		0x0000001f, 
		0x0000008b, 
		0x00000323, 
		0x000003df, 
		0x000002c3, 
		0x000001e9, 
		0x0000039f, 
		0x000003fd, 
		0x0000002b, 
		0x00000077, 
		0x000002e9, 
		0x0000005b, 
		0x00000049, 
		0x0000036f, 
		0x00000255, 
		0x00000067, 
		0x000003df, 
		0x000001bf, 
		0x000000a3, 
		0x00000137, 
		0x000003d9, 
		0x00000153, 
		0x00000093, 
		0x00000245, 
		0x000002eb, 
		0x000003cd, 
		0x00000021, 
		0x00000215, 
		0x00000099, 
		0x000000bb, 
		0x000003d9, 
		0x000000d9, 
		0x000001f7, 
		0x000001e1, 
		0x000000a9, 
		0x0000011d, 
		0x00000193, 
		0x0000017d, 
		0x000002cf, 
		0x00000167, 
		0x0000018f, 
		0x000003cd, 
		0x00000287, 
		0x000000b7, 
		0x000000a5, 
		0x00000063, 
		0x00000059, 
		0x00000115, 
		0x00000121, 
		0x00000311, 
		0x0000012f, 
		0x0000031b, 
		0x000000bd, 
		0x000002f9, 
		0x00000299, 
		0x0000037b, 
		0x00000317, 
		0x0000030b, 
		0x00000383, 
		0x0000005f, 
		0x0000039f, 
		0x0000015f, 
		0x000002c9, 
		0x000001dd, 
		0x0000020f, 
		0x000001ff, 
		0x000002db, 
		0x000000ef, 
		0x0000023d, 
		0x00000383, 
		0x00000265, 
		0x00000117, 
		0x000003fb, 
		0x000003ad, 
		0x000000c7, 
		0x000000b5, 
		0x00000203, 
		0x0000007d, 
		0x00000187, 
		0x00000193, 
		0x0000032d, 
		0x00000023, 
		0x000002f3, 
		0x000000c9, 
		0x00000239, 
		0x00000369, 
		0x00000185, 
		0x000003a1, 
		0x0000030f, 
		0x000002c9, 
		0x000003c7, 
		0x0000005b, 
		0x00000145, 
		0x000002f9, 
		0x000000b9, 
		0x0000034d, 
		0x000001db, 
		0x00000151, 
		0x00000193, 
		0x0000004d, 
		0x00000107, 
		0x000000c5, 
		0x0000010b, 
		0x00000109, 
		0x00000169, 
		0x00000303, 
		0x0000025d, 
		0x00000169, 
		0x00000115, 
		0x000002df, 
		0x000000a9, 
		0x000001cb, 
		0x0000033d, 
		0x000001a5, 
		0x0000036f, 
		0x0000008b, 
		0x0000038b, 
		0x000000f9, 
		0x00000093, 
		0x00000063, 
		0x000002b7, 
		0x000000bb, 
		0x000000ed, 
		0x000001c7, 
		0x000002b9, 
		0x000001bd, 
		0x00000373, 
		0x000003e3, 
		0x000000fb, 
		0x000001b1, 
		0x0000038b, 
		0x000001e7, 
		0x000002f5, 
		0x000003f3, 
		0x0000013d, 
		0x000001ed, 
		0x00000099, 
		0x0000018f, 
		0x000003fd, 
		0x00000051, 
		0x00000303, 
		0x00000179, 
		0x0000006d, 
		0x00000079, 
		0x00000293, 
		0x0000026f, 
		0x000002f1, 
		0x00000391, 
		0x00000369, 
		0x0000013f, 
		0x00000375, 
		0x00000319, 
		0x0000032b, 
		0x000000bf, 
		0x000000ab, 
		0x000003f7, 
		0x0000035d, 
		0x00000379, 
		0x000002e1, 
		0x000003a1, 
		0x00000079, 
		0x000003d5, 
		0x000002e5, 
		0x000002d3, 
		0x000000d5, 
		0x000002c9, 
		0x0000037d, 
		0x00000299, 
		0x00000077, 
		0x000002e9, 
		0x0000029b, 
		0x00000043, 
		0x000002f9, 
		0x00000385, 
		0x0000022d, 
		0x00000009, 
		0x000003f9, 
		0x00000353, 
		0x000003a5, 
		0x000001d3, 
		0x00000281, 
		0x0000010b, 
		0x0000022f, 
		0x000001c9, 
		0x000001bf, 
		0x0000025f, 
		0x00000265, 
		0x00000117, 
		0x000000c9, 
		0x000001ff, 
		0x00000051, 
		0x0000005d, 
		0x0000005b, 
		0x0000015d, 
		0x00000067, 
		0x00000257, 
		0x00000179, 
		0x0000022f, 
		0x0000019d, 
		0x0000018d, 
		0x000001b9, 
		0x0000014d, 
		0x000001a3, 
		0x000002bb, 
		0x000003fb, 
		0x000002dd, 
		0x000000df, 
		0x00000375, 
		0x0000011b, 
		0x0000038f, 
		0x000003bb, 
		0x00000221, 
		0x00000343, 
		0x00000207, 
		0x00000087, 
		0x000000e1, 
		0x000001e5, 
		0x00000093, 
		0x000001dd, 
		0x000000b3, 
		0x00000065, 
		0x000001ed, 
		0x0000010f, 
		0x000002d5, 
		0x00000147, 
		0x0000002b, 
		0x0000006f, 
		0x0000032d, 
		0x0000009b, 
		0x0000022b, 
		0x000000ed, 
		0x000001a9, 
		0x00000005  
};

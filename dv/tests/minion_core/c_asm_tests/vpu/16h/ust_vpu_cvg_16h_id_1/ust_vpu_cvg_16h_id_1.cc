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
			"faddi.pi f2, f0, 379\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
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
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
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
			"li x0, 0xc8\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x3\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 960(x15)\n"
			"flw.ps f16, 864(x15)\n"
			"fsgnjx.ps f27, f12, f16\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 320(x25)\n"
			"fexp.ps f1, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 928(x25)\n"
			"flw.ps f8, 800(x25)\n"
			"flw.ps f3, 576(x25)\n"
			"fmadd.s f19, f4, f8, f3, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 512(x14)\n"
			"fmin.ps f25, f5, f5\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 544(x7)\n"
			"flw.ps f4, 352(x7)\n"
			"fmul.pi f5, f31, f4\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 64(x26)\n"
			"fsrli.pi f3, f23, 0xf\n" 
			"la x7, rand_int_32\n"
			"flw.ps f22, 864(x7)\n"
			"fcvt.ps.pwu f0, f22, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 128(x25)\n"
			"flw.ps f16, 480(x25)\n"
			"flw.ps f12, 896(x25)\n"
			"fnmadd.ps f12, f9, f16, f12, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"flem.ps m7, f6, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x5443bae2bee95c5f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4360a10a06483011\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 288(x23)\n"
			"flw.ps f3, 800(x23)\n"
			"fsub.s f30, f5, f3, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 352(x25)\n"
			"flw.ps f23, 288(x25)\n"
			"fadd.ps f6, f29, f23, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 64(x28)\n"
			"fcvt.ps.pwu f14, f13, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"feqm.ps m0, f25, f14\n" 
			"li x5, 0x80340178\n"
			"ld x5, 0(x5)\n"
			"fbci.ps f7, 0x152bc\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 896(x5)\n"
			"fandi.pi f8, f27, 0x1a7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 800(x9)\n"
			"fcvt.wu.s x20, f10, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x10, 0x220000000476000b\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x7ca\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 928(x9)\n"
			"flw.ps f13, 320(x9)\n"
			"fmul.ps f24, f3, f13, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 96(x5)\n"
			"flw.ps f16, 896(x5)\n"
			"flw.ps f9, 576(x5)\n"
			"fnmsub.ps f18, f18, f16, f9, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"mova.m.x x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_16\n"
			"flw.ps f24, 640(x5)\n"
			"fcvt.ps.f16 f30, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 32(x25)\n"
			"fcvt.wu.s x26, f14, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 864(x14)\n"
			"flw.ps f2, 416(x14)\n"
			"fmin.pi f31, f24, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"mov.m.x m4, x27, 0x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 672(x19)\n"
			"fswizz.ps f23, f0, 0x1c\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 544(x6)\n"
			"flw.ps f0, 768(x6)\n"
			"feq.ps f0, f26, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 224(x12)\n"
			"flw.ps f25, 224(x12)\n"
			"fmax.s f13, f10, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 928(x9)\n"
			"flw.ps f19, 512(x9)\n"
			"fsgnjx.s f16, f17, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f26, 384(x25)\n"
			"flw.ps f31, 96(x25)\n"
			"fmaxu.pi f1, f26, f31\n" 
			"li x5, 0x9115\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_0:\n"
			"mov.m.x m0, x22, 0x3b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 224(x25)\n"
			"fcvt.pwu.ps f10, f1, rdn\n" 
			"maskpopc x6, m3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 992(x9)\n"
			"fnot.pi f8, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_364375_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_659751_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_686075_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x20, 0x200000008000019\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0xed << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xdc << TENSOR_IMA_SCP_LOC_B) | (0x44 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"masknot m3, m0\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 640(x20)\n"
			"flw.ps f30, 928(x20)\n"
			"fcmovm.ps f5, f16, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 192(x28)\n"
			"flw.ps f28, 832(x28)\n"
			"fmul.s f26, f21, f28, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 544(x23)\n"
			"faddi.pi f16, f14, 0x25\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 352(x28)\n"
			"flw.ps f16, 800(x28)\n"
			"fmax.pi f11, f20, f16\n" 
			"li x19, 0xc730bb5a995bc66c\n"
			"fcvt.s.w f3, x19, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 480(x16)\n"
			"flw.ps f24, 192(x16)\n"
			"fmax.ps f31, f3, f24\n" 
			"maskpopc x23, m2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 576(x11)\n"
			"flw.ps f29, 640(x11)\n"
			"fsub.s f23, f6, f29, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 480(x27)\n"
			"flw.ps f2, 896(x27)\n"
			"flw.ps f17, 416(x27)\n"
			"fcmov.ps f30, f20, f2, f17\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 448(x17)\n"
			"flw.ps f29, 512(x17)\n"
			"fsgnj.s f2, f5, f29\n" 
			"mova.x.m x14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 352(x11)\n"
			"fcvt.wu.s x7, f23, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 352(x14)\n"
			"flw.ps f9, 544(x14)\n"
			"fsgnj.ps f8, f28, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 608(x13)\n"
			"flw.ps f10, 992(x13)\n"
			"flw.ps f31, 736(x13)\n"
			"fmsub.ps f6, f22, f10, f31, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 96(x7)\n"
			"faddi.pi f0, f26, 0x67\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_533772_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x6b << TENSOR_FMA16_SCP_LOC_B) | (0xf9 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_53974_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x49 << TENSOR_IMA_SCP_LOC_B) | (0xe8 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_556232_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x26 << TENSOR_FMA32_SCP_LOC_B) | (0x1e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 512(x17)\n"
			"flw.ps f23, 768(x17)\n"
			"fadd.s f17, f11, f23, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f16, 832(x5)\n"
			"fminu.pi f18, f16, f16\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 128(x27)\n"
			"flw.ps f4, 224(x27)\n"
			"fnmsub.ps f11, f15, f4, f4, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 288(x22)\n"
			"flw.ps f9, 544(x22)\n"
			"fltu.pi f5, f18, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"maskxor m1, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f1, 576(x11)\n"
			"fsrli.pi f22, f1, 0x2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 512(x23)\n"
			"flw.ps f7, 832(x23)\n"
			"fsll.pi f15, f12, f7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 352(x19)\n"
			"fclass.s x13, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskor m1, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_1:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 704(x11)\n"
			"fexp.ps f1, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 96(x19)\n"
			"flw.ps f9, 544(x19)\n"
			"fmin.s f24, f6, f9\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 352(x6)\n"
			"flw.ps f13, 672(x6)\n"
			"fsll.pi f6, f1, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 480(x14)\n"
			"flw.ps f19, 0(x14)\n"
			"fcmovm.ps f22, f28, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 704(x13)\n"
			"flw.ps f18, 544(x13)\n"
			"fmin.ps f18, f30, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_1:\n"
			"maskor m4, m0, m3\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 224(x9)\n"
			"fcvt.w.s x11, f17, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 256(x5)\n"
			"flw.ps f14, 576(x5)\n"
			"fadd.s f30, f5, f14, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 960(x19)\n"
			"flog.ps f28, f1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 992(x20)\n"
			"fpackreph.pi f26, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 672(x18)\n"
			"flw.ps f15, 32(x18)\n"
			"flw.ps f28, 800(x18)\n"
			"fnmsub.s f19, f3, f15, f28, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 608(x13)\n"
			"fcvt.wu.s x25, f4, rne\n" 
			"maskpopc x20, m3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 672(x12)\n"
			"flw.ps f21, 64(x12)\n"
			"feq.pi f3, f28, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 64(x10)\n"
			"fcvt.w.s x19, f7, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 864(x7)\n"
			"flw.ps f18, 320(x7)\n"
			"fsub.ps f0, f28, f18, rdn\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f11, 800(x19)\n"
			"fcvt.ps.f16 f1, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 800(x11)\n"
			"flw.ps f4, 896(x11)\n"
			"flt.s x12, f29, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 128(x11)\n"
			"flw.ps f23, 640(x11)\n"
			"flw.ps f25, 480(x11)\n"
			"fnmadd.ps f21, f18, f23, f25, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 608(x26)\n"
			"flw.ps f16, 544(x26)\n"
			"fmax.ps f27, f11, f16\n" 
			"mova.x.m x17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 672(x26)\n"
			"flw.ps f26, 896(x26)\n"
			"for.pi f9, f8, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f0, 992(x20)\n"
			"flw.ps f29, 320(x20)\n"
			"feq.pi f22, f0, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 544(x22)\n"
			"flw.ps f19, 288(x22)\n"
			"fltu.pi f13, f30, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 960(x27)\n"
			"flw.ps f10, 160(x27)\n"
			"fsgnjn.ps f26, f26, f10\n" 
			"la x26, rand_int_32\n"
			"flw.ps f21, 736(x26)\n"
			"flw.ps f12, 512(x26)\n"
			"fsub.pi f11, f21, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 704(x5)\n"
			"flw.ps f27, 224(x5)\n"
			"fsgnjx.s f24, f21, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"mov.m.x m5, x20, 0x1b\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4d4b40e523\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 960(x17)\n"
			"flw.ps f13, 608(x17)\n"
			"fmul.ps f4, f4, f13, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_1:\n"
			"fltm.pi m4, f6, f28\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 160(x7)\n"
			"fround.ps f22, f20, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 224(x20)\n"
			"fcvt.pw.ps f15, f2, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_1:\n"
			"mov.m.x m5, x27, 0x74\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 864(x18)\n"
			"flw.ps f30, 640(x18)\n"
			"flw.ps f11, 416(x18)\n"
			"fmadd.ps f13, f14, f30, f11, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 448(x20)\n"
			"flw.ps f20, 192(x20)\n"
			"fle.s x6, f23, f20\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 480(x11)\n"
			"flw.ps f2, 224(x11)\n"
			"fle.s x15, f6, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 288(x28)\n"
			"flw.ps f23, 896(x28)\n"
			"fadd.ps f22, f26, f23, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 320(x10)\n"
			"flw.ps f17, 800(x10)\n"
			"fle.pi f21, f22, f17\n" 
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f16, 160(x14)\n"
			"flw.ps f23, 224(x14)\n"
			"fmax.pi f30, f16, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 608(x23)\n"
			"flw.ps f8, 736(x23)\n"
			"flw.ps f5, 640(x23)\n"
			"fmsub.ps f31, f15, f8, f5, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 928(x19)\n"
			"flw.ps f0, 640(x19)\n"
			"fmax.s f6, f12, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f7, 32(x27)\n"
			"flw.ps f21, 672(x27)\n"
			"flt.pi f10, f7, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 704(x15)\n"
			"fclass.ps f5, f22\n" 
			"la x13, rand_int_32\n"
			"flw.ps f6, 192(x13)\n"
			"fpackreph.pi f1, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.ps m5, f13, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f0, 32(x14)\n"
			"fsat8.pi f5, f0\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f23, 352(x26)\n"
			"fcvt.ps.f16 f19, f23\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 960(x16)\n"
			"flw.ps f16, 96(x16)\n"
			"fminu.pi f0, f20, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 544(x7)\n"
			"flw.ps f12, 0(x7)\n"
			"fminu.pi f19, f13, f12\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 352(x17)\n"
			"flw.ps f1, 256(x17)\n"
			"flw.ps f13, 192(x17)\n"
			"fnmadd.s f13, f18, f1, f13, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 448(x23)\n"
			"flw.ps f0, 800(x23)\n"
			"fmin.s f31, f26, f0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 192(x6)\n"
			"fmul.ps f23, f5, f5, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f22, 768(x16)\n"
			"flw.ps f10, 224(x16)\n"
			"fmul.pi f28, f22, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fbci.ps f26, 0x5a822\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x17, 0x220000000476000b\n"
			"csrw tensor_reduce, x17\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_739083_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 288(x18)\n"
			"flw.ps f5, 512(x18)\n"
			"fdiv.s f9, f6, f5, rmm\n" 
			"fbci.ps f31, 0x18e65\n" 
			"li x15, 0x8720fcfe8d3dbefe\n"
			"fcvt.s.w f7, x15, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 256(x20)\n"
			"feq.pi f27, f21, f21\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 64(x20)\n"
			"flw.ps f8, 416(x20)\n"
			"fle.ps f30, f30, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 736(x28)\n"
			"fcvt.wu.s x27, f23, rup\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f29, 320(x13)\n"
			"fcvt.ps.f16 f5, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 416(x6)\n"
			"flw.ps f13, 960(x6)\n"
			"fmsub.s f26, f14, f14, f13, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 704(x13)\n"
			"flw.ps f27, 928(x13)\n"
			"flw.ps f8, 608(x13)\n"
			"fmadd.s f27, f2, f27, f8, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 544(x15)\n"
			"flw.ps f6, 448(x15)\n"
			"fadd.pi f22, f31, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 64(x20)\n"
			"flw.ps f16, 608(x20)\n"
			"fmul.s f24, f9, f16, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"fltm.ps m0, f16, f20\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 832(x6)\n"
			"flw.ps f26, 128(x6)\n"
			"fsgnjx.ps f18, f4, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 704(x26)\n"
			"flw.ps f27, 864(x26)\n"
			"fadd.s f4, f28, f27, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f13, 96(x13)\n"
			"flw.ps f27, 832(x13)\n"
			"fmin.pi f30, f13, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 640(x10)\n"
			"flw.ps f22, 576(x10)\n"
			"flw.ps f20, 288(x10)\n"
			"fmadd.s f6, f31, f22, f20, rup\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x9c << TENSOR_FMA32_SCP_LOC_B) | (0xe9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 736(x20)\n"
			"flw.ps f29, 992(x20)\n"
			"fmax.ps f11, f24, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 224(x28)\n"
			"flw.ps f20, 32(x28)\n"
			"feq.s x28, f3, f20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 608(x13)\n"
			"fsin.ps f26, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 32(x10)\n"
			"flw.ps f26, 672(x10)\n"
			"fsgnj.ps f24, f15, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 928(x28)\n"
			"flw.ps f8, 64(x28)\n"
			"flw.ps f24, 960(x28)\n"
			"fnmsub.s f3, f20, f8, f24, rdn\n" 
			"la x16, rand_ieee754_16\n"
			"flw.ps f3, 640(x16)\n"
			"fcvt.ps.f16 f2, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 960(x17)\n"
			"flw.ps f22, 256(x17)\n"
			"fmax.pi f17, f8, f22\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 448(x20)\n"
			"flw.ps f19, 960(x20)\n"
			"flt.ps f7, f8, f19\n" 
			"maskpopc x27, m0\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 576(x10)\n"
			"flw.ps f8, 64(x10)\n"
			"fle.ps f14, f25, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 320(x19)\n"
			"flw.ps f29, 576(x19)\n"
			"fle.ps f30, f6, f29\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 480(x10)\n"
			"flw.ps f30, 864(x10)\n"
			"flw.ps f5, 864(x10)\n"
			"fmsub.ps f27, f20, f30, f5, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 128(x22)\n"
			"fsrai.pi f24, f15, 0x6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 800(x14)\n"
			"flw.ps f17, 704(x14)\n"
			"flt.s x12, f25, f17\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 544(x28)\n"
			"flw.ps f2, 608(x28)\n"
			"for.pi f7, f5, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 64(x14)\n"
			"flw.ps f23, 224(x14)\n"
			"fsgnj.ps f20, f29, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xf << TENSOR_IMA_SCP_LOC_B) | (0x65 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 224(x25)\n"
			"flw.ps f3, 992(x25)\n"
			"fsgnjx.s f23, f1, f3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 192(x28)\n"
			"flw.ps f1, 320(x28)\n"
			"flw.ps f14, 224(x28)\n"
			"fnmsub.s f13, f10, f1, f14, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 480(x20)\n"
			"flw.ps f23, 672(x20)\n"
			"feq.s x25, f22, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fltm.pi m4, f14, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 896(x22)\n"
			"flw.ps f22, 320(x22)\n"
			"flw.ps f6, 960(x22)\n"
			"fmsub.ps f5, f2, f22, f6, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 576(x11)\n"
			"flw.ps f0, 768(x11)\n"
			"fsub.ps f13, f26, f0, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 672(x28)\n"
			"fslli.pi f8, f15, 0xf\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 512(x19)\n"
			"fnot.pi f10, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 544(x28)\n"
			"flw.ps f23, 832(x28)\n"
			"fmul.s f9, f4, f23, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 128(x13)\n"
			"flw.ps f13, 256(x13)\n"
			"fsub.ps f8, f27, f13, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 832(x23)\n"
			"fadd.s f15, f31, f31, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_3:\n"
			"fltm.pi m1, f22, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f13, 928(x23)\n"
			"flw.ps f12, 736(x23)\n"
			"fminu.pi f5, f13, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_3:\n"
			"fsetm.pi m5, f11\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x193ce136db\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f4, 288(x15)\n"
			"fsrli.pi f31, f4, 0xf\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 256(x18)\n"
			"flw.ps f5, 544(x18)\n"
			"fmul.ps f30, f18, f5, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 192(x18)\n"
			"flw.ps f30, 864(x18)\n"
			"feq.ps f4, f10, f30\n" 
			"maskpopc x20, m4\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 160(x11)\n"
			"fandi.pi f23, f25, 0x12d\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_3:\n"
			"maskand m1, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 0(x14)\n"
			"fcvt.pwu.ps f3, f27, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 0(x5)\n"
			"flw.ps f30, 576(x5)\n"
			"fmul.pi f24, f23, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 96(x22)\n"
			"flw.ps f13, 896(x22)\n"
			"fxor.pi f7, f9, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 480(x14)\n"
			"flw.ps f5, 672(x14)\n"
			"flt.ps f30, f6, f5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 0(x13)\n"
			"flw.ps f20, 96(x13)\n"
			"fsgnjx.ps f27, f0, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 192(x20)\n"
			"flw.ps f11, 416(x20)\n"
			"fsgnjx.s f6, f14, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_3:\n"
			"maskor m6, m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 960(x17)\n"
			"ffrc.ps f16, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 576(x19)\n"
			"flw.ps f13, 704(x19)\n"
			"flw.ps f4, 448(x19)\n"
			"fnmadd.s f15, f16, f13, f4, rup\n" 
			"li x20, 0xa0f84377034e3510\n"
			"fcvt.s.w f18, x20, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 64(x27)\n"
			"flw.ps f1, 352(x27)\n"
			"fle.s x15, f4, f1\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f14, 864(x9)\n"
			"fcvt.ps.pw f27, f14, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 256(x20)\n"
			"flw.ps f15, 992(x20)\n"
			"fcmovm.ps f5, f10, f15\n" 
			"li x15, 0xc2d415665e154846\n"
			"fcvt.s.wu f14, x15, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_3:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 512(x12)\n"
			"fexp.ps f26, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 480(x9)\n"
			"fcvt.ps.pw f25, f1, rne\n" 
			"li x7, 0x6e2a2aa565733490\n"
			"fcvt.s.lu f29, x7, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 128(x26)\n"
			"flw.ps f21, 768(x26)\n"
			"flw.ps f10, 832(x26)\n"
			"fnmsub.s f30, f3, f21, f10, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 384(x6)\n"
			"fsrli.pi f13, f21, 0x5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 928(x15)\n"
			"flw.ps f10, 704(x15)\n"
			"flt.pi f8, f29, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_3:\n"
			"flem.ps m5, f8, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f23, 256(x6)\n"
			"flw.ps f12, 288(x6)\n"
			"fsra.pi f0, f23, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 96(x7)\n"
			"flw.ps f11, 384(x7)\n"
			"flt.s x11, f28, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f12, 128(x16)\n"
			"fsatu8.pi f10, f12\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 928(x27)\n"
			"fcvt.ps.pwu f18, f10, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 864(x16)\n"
			"flw.ps f14, 992(x16)\n"
			"flw.ps f10, 576(x16)\n"
			"fmadd.s f5, f31, f14, f10, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f6, 64(x22)\n"
			"flw.ps f19, 832(x22)\n"
			"fdivu.pi f9, f6, f19\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
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
			"LBL_KERNEL_HID_4:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_4\n"
			"li x5, 0x3\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f11, 0(x7)\n"
			"fnot.pi f25, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 224(x10)\n"
			"flw.ps f4, 864(x10)\n"
			"fxor.pi f28, f8, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 640(x22)\n"
			"faddi.pi f12, f14, 0xce\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 640(x12)\n"
			"flw.ps f8, 320(x12)\n"
			"fsub.ps f27, f16, f8, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 32(x14)\n"
			"fsub.s f17, f22, f22, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"fltm.pi m3, f11, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 128(x19)\n"
			"fround.ps f22, f23, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 512(x14)\n"
			"flw.ps f21, 160(x14)\n"
			"feq.s x15, f20, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"masknot m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f6, 480(x7)\n"
			"flw.ps f15, 96(x7)\n"
			"fmaxu.pi f3, f6, f15\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 192(x16)\n"
			"fltu.pi f1, f10, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 480(x14)\n"
			"flw.ps f8, 352(x14)\n"
			"flw.ps f24, 768(x14)\n"
			"fnmadd.ps f5, f7, f8, f24, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_4:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 192(x11)\n"
			"flog.ps f0, f17\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 736(x19)\n"
			"flw.ps f2, 672(x19)\n"
			"fmul.pi f3, f24, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 480(x10)\n"
			"fsrli.pi f29, f20, 0xc\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 128(x13)\n"
			"fclass.s x19, f21\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x5, 0x220000000476000b\n"
			"csrw tensor_reduce, x5\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x93 << TENSOR_IMA_SCP_LOC_B) | (0x60 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 288(x5)\n"
			"flw.ps f18, 32(x5)\n"
			"flw.ps f12, 832(x5)\n"
			"fnmadd.ps f7, f26, f18, f12, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 960(x13)\n"
			"fsatu8.pi f27, f23\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 160(x12)\n"
			"fpackreph.pi f26, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 480(x20)\n"
			"flw.ps f18, 640(x20)\n"
			"fsgnj.ps f31, f30, f18\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 256(x14)\n"
			"fsqrt.ps f31, f31\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_4:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 352(x10)\n"
			"fexp.ps f0, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"mov.m.x m0, x10, 0x6f\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_4:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 832(x26)\n"
			"flw.ps f18, 832(x26)\n"
			"fle.ps f8, f25, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 288(x19)\n"
			"flw.ps f6, 896(x19)\n"
			"fsub.s f8, f20, f6, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 928(x7)\n"
			"fcvt.pwu.ps f18, f9, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 288(x11)\n"
			"flw.ps f30, 736(x11)\n"
			"fsrl.pi f0, f22, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 256(x9)\n"
			"flw.ps f31, 352(x9)\n"
			"fmax.pi f8, f9, f31\n" 
			"li x19, 0xd6d436a77c7665d1\n"
			"fcvt.s.w f30, x19, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 672(x7)\n"
			"fpackreph.pi f17, f24\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 384(x19)\n"
			"fclass.ps f16, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_678574_HID_4:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_65106_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe2 << TENSOR_FMA32_SCP_LOC_B) | (0xb8 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_394910_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x3f << TENSOR_FMA32_SCP_LOC_B) | (0xef << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f27, 512(x6)\n"
			"fsrai.pi f28, f27, 0x4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 800(x9)\n"
			"fclass.s x27, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_4:\n"
			"fsetm.pi m1, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 544(x27)\n"
			"flw.ps f29, 224(x27)\n"
			"fsgnj.s f14, f24, f29\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 0(x25)\n"
			"flw.ps f23, 576(x25)\n"
			"fmulhu.pi f23, f1, f23\n" 
			"li x5, 0x9a6d17ec39897299\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x688d83a4c8716250\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_4:\n"
			"fltm.ps m1, f15, f27\n" 
			"li x5, 0x803401F0\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 704(x5)\n"
			"flw.ps f17, 704(x5)\n"
			"feq.pi f4, f16, f17\n" 
			"li x15, 0x7e7420503901027c\n"
			"fcvt.s.lu f24, x15, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 288(x23)\n"
			"fpackreph.pi f23, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 832(x22)\n"
			"frem.pi f20, f1, f1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 800(x9)\n"
			"flw.ps f3, 384(x9)\n"
			"fsgnjn.s f19, f14, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 224(x9)\n"
			"flw.ps f17, 544(x9)\n"
			"fsgnjn.ps f27, f31, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 192(x16)\n"
			"fcvt.ps.pwu f0, f30, rmm\n" 
			"li x26, 0x4d8460d42dd7c35d\n"
			"fcvt.s.wu f23, x26, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 608(x26)\n"
			"fmulh.pi f3, f11, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 736(x9)\n"
			"flw.ps f16, 896(x9)\n"
			"fsgnjn.s f26, f20, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_487738_HID_4:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_38493_HID_4:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f30,  64 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
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
			"li x5, 0x9328e3ba373af4db\n"
			"fcvt.s.wu f24, x5, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"mov.m.x m7, x5, 0x69\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x6, 0x42e45f8bcaff67ba\n"
			"fcvt.s.wu f30, x6, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 960(x10)\n"
			"flw.ps f9, 800(x10)\n"
			"fsub.pi f5, f6, f9\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 832(x20)\n"
			"flw.ps f20, 544(x20)\n"
			"fsra.pi f26, f6, f20\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 928(x5)\n"
			"fcvt.wu.s x26, f21, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 704(x22)\n"
			"fsrai.pi f7, f8, 0x5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 128(x11)\n"
			"fandi.pi f27, f18, 0x198\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 640(x16)\n"
			"fclass.ps f18, f28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 992(x13)\n"
			"flw.ps f5, 736(x13)\n"
			"for.pi f5, f14, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 800(x19)\n"
			"flw.ps f7, 832(x19)\n"
			"flw.ps f15, 32(x19)\n"
			"fnmsub.ps f3, f5, f7, f15, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 160(x15)\n"
			"fcvt.ps.pwu f15, f27, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 704(x28)\n"
			"flw.ps f13, 320(x28)\n"
			"fsgnj.ps f4, f25, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 576(x25)\n"
			"faddi.pi f18, f13, 0x139\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 736(x16)\n"
			"flw.ps f12, 704(x16)\n"
			"feq.s x27, f28, f12\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 576(x25)\n"
			"flw.ps f9, 736(x25)\n"
			"frem.pi f28, f13, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 96(x6)\n"
			"fnot.pi f1, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_5:\n"
			"fsetm.pi m5, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_5:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 544(x6)\n"
			"flog.ps f30, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"feqm.ps m4, f25, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 768(x6)\n"
			"fcvt.pwu.ps f31, f9, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_5:\n"
			"feqm.ps m6, f16, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 896(x13)\n"
			"flw.ps f10, 128(x13)\n"
			"flw.ps f31, 512(x13)\n"
			"fnmadd.ps f5, f3, f10, f31, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 0(x19)\n"
			"flw.ps f26, 960(x19)\n"
			"flw.ps f12, 384(x19)\n"
			"fnmadd.ps f31, f2, f26, f12, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 512(x25)\n"
			"fslli.pi f6, f2, 0x0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 768(x6)\n"
			"flw.ps f2, 864(x6)\n"
			"fsgnjn.s f30, f19, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 480(x28)\n"
			"flw.ps f12, 928(x28)\n"
			"fmul.ps f22, f4, f12, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 992(x6)\n"
			"flw.ps f17, 928(x6)\n"
			"fadd.pi f7, f23, f17\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 832(x18)\n"
			"flw.ps f10, 416(x18)\n"
			"feq.pi f13, f29, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 352(x6)\n"
			"ffrc.ps f7, f6\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 352(x16)\n"
			"flw.ps f25, 992(x16)\n"
			"fnmadd.s f6, f20, f25, f20, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_5:\n"
			"fltm.ps m1, f8, f25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe169e5ab6a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 800(x10)\n"
			"flw.ps f16, 32(x10)\n"
			"fsgnj.ps f3, f20, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 352(x25)\n"
			"flw.ps f17, 480(x25)\n"
			"fmul.ps f7, f25, f17, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 544(x23)\n"
			"flw.ps f27, 64(x23)\n"
			"fmin.ps f3, f9, f27\n" 
			"li x5, 0xd3e8951dd321c3df\n"
			"fcvt.s.wu f9, x5, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 288(x22)\n"
			"fswizz.ps f28, f10, 0x19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 896(x26)\n"
			"fsqrt.ps f19, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 128(x20)\n"
			"fsrli.pi f31, f11, 0xa\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 160(x19)\n"
			"flw.ps f29, 320(x19)\n"
			"fle.pi f6, f23, f29\n" 
			"mova.x.m x10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 832(x20)\n"
			"flw.ps f20, 640(x20)\n"
			"fle.ps f7, f19, f20\n" 
			"li x27, 0x8c7bec1c2fac6a97\n"
			"fcvt.s.wu f17, x27, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_5:\n"
			"flem.ps m1, f0, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 512(x28)\n"
			"flw.ps f28, 640(x28)\n"
			"flw.ps f19, 480(x28)\n"
			"fnmsub.ps f25, f12, f28, f19, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 960(x10)\n"
			"fsgnjn.s f11, f27, f27\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 128(x13)\n"
			"flw.ps f30, 64(x13)\n"
			"flt.pi f3, f3, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 352(x27)\n"
			"flw.ps f13, 512(x27)\n"
			"fadd.ps f26, f3, f13, rtz\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 128(x16)\n"
			"fcvt.pwu.ps f6, f9, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 448(x16)\n"
			"flw.ps f8, 0(x16)\n"
			"fltu.pi f4, f4, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 896(x17)\n"
			"fcvt.w.s x10, f18, rdn\n" 
			"fbci.pi f9, 0x1959d\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 640(x25)\n"
			"fcvt.pwu.ps f28, f5, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 608(x20)\n"
			"fcvt.pwu.ps f25, f15, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 736(x10)\n"
			"fsin.ps f31, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 96(x7)\n"
			"flw.ps f16, 160(x7)\n"
			"fltu.pi f7, f31, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 736(x10)\n"
			"flw.ps f22, 544(x10)\n"
			"fmax.s f21, f14, f22\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 800(x9)\n"
			"flw.ps f5, 896(x9)\n"
			"fmax.s f2, f24, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fbci.ps f19, 0x364da\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 256(x23)\n"
			"ffrc.ps f23, f3\n" 
			"li x11, 0xfa76aa287969b24\n"
			"fcvt.s.wu f15, x11, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 32(x19)\n"
			"flw.ps f5, 736(x19)\n"
			"flt.s x23, f20, f5\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 864(x10)\n"
			"fswizz.ps f17, f17, 0x1d\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 864(x27)\n"
			"fsat8.pi f20, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x5, 0x220000000476000b\n"
			"csrw tensor_reduce, x5\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_6:\n"
			"maskand m4, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_6:\n"
			"feqm.ps m4, f12, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 128(x12)\n"
			"flw.ps f18, 608(x12)\n"
			"fadd.ps f8, f11, f18, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"masknot m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 992(x16)\n"
			"flw.ps f26, 736(x16)\n"
			"fsub.ps f10, f18, f26, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"maskand m0, m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f28, 96(x22)\n"
			"flw.ps f14, 512(x22)\n"
			"fmulhu.pi f5, f28, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 672(x22)\n"
			"flw.ps f12, 736(x22)\n"
			"for.pi f30, f31, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 64(x9)\n"
			"flw.ps f21, 928(x9)\n"
			"fsgnj.s f21, f1, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 512(x16)\n"
			"flw.ps f5, 928(x16)\n"
			"fadd.ps f31, f10, f5, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 32(x23)\n"
			"flw.ps f8, 352(x23)\n"
			"flt.pi f4, f29, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"maskand m0, m5, m5\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_int_32\n"
			"flw.ps f18, 736(x11)\n"
			"flw.ps f20, 800(x11)\n"
			"fmul.pi f8, f18, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 160(x7)\n"
			"fcvt.w.s x6, f19, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 768(x7)\n"
			"flw.ps f13, 928(x7)\n"
			"fsgnj.ps f11, f21, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x19, 0x200000000000000\n"
			"csrw tensor_reduce, x19\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd1 << TENSOR_IMA_SCP_LOC_B) | (0xc5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x34 << TENSOR_FMA16_SCP_LOC_B) | (0x6b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f10, 768(x7)\n"
			"fslli.pi f20, f10, 0x0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 32(x13)\n"
			"flw.ps f24, 992(x13)\n"
			"flw.ps f20, 0(x13)\n"
			"fmsub.s f9, f6, f24, f20, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 832(x14)\n"
			"flw.ps f23, 480(x14)\n"
			"feq.ps f19, f24, f23\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x28, m5\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 640(x17)\n"
			"flw.ps f28, 832(x17)\n"
			"fsll.pi f13, f30, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 416(x22)\n"
			"flw.ps f14, 512(x22)\n"
			"flw.ps f29, 608(x22)\n"
			"fnmsub.s f13, f3, f14, f29, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 448(x25)\n"
			"ffrc.ps f8, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"maskand m5, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_6:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 672(x13)\n"
			"fexp.ps f25, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 224(x19)\n"
			"fswizz.ps f27, f5, 0x1a\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 992(x7)\n"
			"fpackreph.pi f28, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_int_32\n"
			"flw.ps f28, 864(x14)\n"
			"flw.ps f21, 96(x14)\n"
			"fmaxu.pi f30, f28, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 480(x17)\n"
			"flw.ps f9, 416(x17)\n"
			"flt.s x28, f26, f9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f25, 672(x10)\n"
			"fsatu8.pi f9, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 0(x27)\n"
			"flw.ps f1, 960(x27)\n"
			"flw.ps f9, 384(x27)\n"
			"fnmsub.ps f28, f11, f1, f9, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 800(x15)\n"
			"flw.ps f27, 960(x15)\n"
			"flt.ps f15, f11, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_327209_HID_6:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_33293_HID_6:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x84 << TENSOR_FMA16_SCP_LOC_B) | (0x46 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf8 << TENSOR_FMA16_SCP_LOC_B) | (0xc6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
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
			"la x6, rand_int_32\n"
			"flw.ps f5, 928(x6)\n"
			"flw.ps f28, 768(x6)\n"
			"for.pi f11, f5, f28\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f0, 608(x28)\n"
			"fcvt.ps.f16 f7, f0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 768(x11)\n"
			"fsrai.pi f20, f19, 0x3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 128(x15)\n"
			"flw.ps f23, 800(x15)\n"
			"flw.ps f14, 352(x15)\n"
			"fcmov.ps f19, f8, f23, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 512(x26)\n"
			"flw.ps f9, 800(x26)\n"
			"fadd.ps f3, f8, f9, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 256(x28)\n"
			"flw.ps f0, 320(x28)\n"
			"fltu.pi f2, f10, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 32(x12)\n"
			"flw.ps f26, 800(x12)\n"
			"fcmovm.ps f12, f9, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"maskand m6, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f16, 320(x18)\n"
			"flw.ps f10, 672(x18)\n"
			"fsra.pi f13, f16, f10\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 192(x13)\n"
			"flw.ps f12, 416(x13)\n"
			"fmul.pi f14, f18, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 480(x6)\n"
			"flw.ps f18, 480(x6)\n"
			"flt.ps f17, f8, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"masknot m3, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 576(x10)\n"
			"flw.ps f7, 384(x10)\n"
			"feq.s x27, f8, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f3, 672(x14)\n"
			"flw.ps f9, 736(x14)\n"
			"fmulhu.pi f2, f3, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 832(x13)\n"
			"flw.ps f19, 704(x13)\n"
			"fmul.pi f3, f10, f19\n" 
			"li x23, 0x5ca681556510a805\n"
			"fcvt.s.lu f1, x23, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f9, 960(x28)\n"
			"flw.ps f11, 320(x28)\n"
			"for.pi f27, f9, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 512(x7)\n"
			"ffrc.ps f3, f17\n" 
			"li x12, 0xbecb767fced302a7\n"
			"fcvt.s.wu f7, x12, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 544(x16)\n"
			"flw.ps f4, 736(x16)\n"
			"fmaxu.pi f11, f20, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 864(x28)\n"
			"flw.ps f24, 512(x28)\n"
			"fsll.pi f7, f9, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 672(x5)\n"
			"flw.ps f12, 736(x5)\n"
			"fcmovm.ps f24, f10, f12\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 320(x11)\n"
			"fcvt.pw.ps f21, f6, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 928(x11)\n"
			"flw.ps f30, 32(x11)\n"
			"fcmov.ps f10, f29, f30, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 288(x5)\n"
			"flw.ps f10, 544(x5)\n"
			"fmax.ps f7, f29, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 704(x28)\n"
			"flw.ps f27, 640(x28)\n"
			"fmin.ps f9, f24, f27\n" 
			"maskpopcz x7, m3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 64(x28)\n"
			"fpackrepb.pi f17, f13\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 416(x13)\n"
			"flw.ps f30, 192(x13)\n"
			"fsub.s f10, f29, f30, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 128(x10)\n"
			"flw.ps f11, 640(x10)\n"
			"for.pi f7, f1, f11\n" 
			"maskpopc x18, m3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 448(x18)\n"
			"flw.ps f3, 384(x18)\n"
			"frem.pi f30, f9, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 352(x23)\n"
			"fcvt.pw.ps f8, f24, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 352(x26)\n"
			"fcvt.ps.pwu f19, f10, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_2_HID_7:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 32(x10)\n"
			"frcp.ps f10, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 960(x14)\n"
			"flw.ps f22, 576(x14)\n"
			"fmul.pi f2, f31, f22\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 32(x6)\n"
			"fsrai.pi f14, f10, 0xa\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 224(x20)\n"
			"fcvt.ps.pwu f26, f9, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 608(x15)\n"
			"fcvt.ps.pwu f27, f23, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f27, 352(x27)\n"
			"flw.ps f7, 128(x27)\n"
			"fxor.pi f8, f27, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 864(x23)\n"
			"flw.ps f25, 512(x23)\n"
			"feq.pi f23, f30, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 672(x14)\n"
			"fswizz.ps f30, f24, 0x1f\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 512(x20)\n"
			"flw.ps f17, 576(x20)\n"
			"fsgnj.s f30, f6, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 704(x11)\n"
			"flw.ps f5, 384(x11)\n"
			"flw.ps f0, 704(x11)\n"
			"fnmadd.ps f17, f18, f5, f0, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 64(x14)\n"
			"flw.ps f30, 480(x14)\n"
			"fmax.ps f11, f29, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 928(x10)\n"
			"flw.ps f16, 800(x10)\n"
			"feq.pi f15, f14, f16\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 512(x19)\n"
			"flw.ps f16, 160(x19)\n"
			"fmulh.pi f7, f25, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 704(x18)\n"
			"fcvt.pw.ps f8, f26, rmm\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f25,  736 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 640(x17)\n"
			"flw.ps f3, 640(x17)\n"
			"fadd.ps f18, f7, f3, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 672(x18)\n"
			"flw.ps f30, 288(x18)\n"
			"fsra.pi f20, f19, f30\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 704(x18)\n"
			"fsatu8.pi f8, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 864(x11)\n"
			"fpackrepb.pi f3, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 320(x20)\n"
			"flw.ps f19, 640(x20)\n"
			"fsll.pi f0, f17, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 448(x17)\n"
			"flw.ps f26, 704(x17)\n"
			"fsgnjx.s f7, f11, f26\n" 
			"maskpopc x27, m6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 896(x10)\n"
			"flw.ps f8, 608(x10)\n"
			"fmulhu.pi f28, f23, f8\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f2, 64(x22)\n"
			"fcvt.ps.f16 f12, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 224(x20)\n"
			"fcvt.f16.ps f6, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 736(x14)\n"
			"flw.ps f26, 448(x14)\n"
			"fsgnjx.s f5, f11, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 800(x17)\n"
			"flw.ps f0, 672(x17)\n"
			"flt.s x19, f3, f0\n" 
			"fbci.pi f15, 0x736cf\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 160(x23)\n"
			"fnot.pi f26, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 736(x14)\n"
			"flw.ps f4, 576(x14)\n"
			"fsll.pi f11, f7, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 384(x12)\n"
			"fsin.ps f19, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x26, 0x220000000476000b\n"
			"csrw tensor_reduce, x26\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_647126_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x4 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x10 << TENSOR_FMA32_SCP_LOC_B) | (0x12 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4a << TENSOR_IMA_SCP_LOC_B) | (0xfd << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 384(x7)\n"
			"flw.ps f30, 544(x7)\n"
			"fadd.s f4, f14, f30, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f28, 608(x28)\n"
			"flw.ps f26, 384(x28)\n"
			"flt.pi f18, f28, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 384(x9)\n"
			"flw.ps f13, 544(x9)\n"
			"fsgnjn.ps f16, f20, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 576(x7)\n"
			"flw.ps f22, 512(x7)\n"
			"fle.ps f25, f27, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 544(x22)\n"
			"flw.ps f30, 544(x22)\n"
			"flw.ps f20, 864(x22)\n"
			"fnmsub.s f26, f21, f30, f20, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_8:\n"
			"maskor m1, m7, m6\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x30b7784e53429c9f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4efbf2670b478aad\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x25, rand_int_32\n"
			"flw.ps f2, 96(x25)\n"
			"flw.ps f30, 736(x25)\n"
			"fdiv.pi f7, f2, f30\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 800(x10)\n"
			"flw.ps f29, 672(x10)\n"
			"flw.ps f23, 128(x10)\n"
			"fcmov.ps f3, f31, f29, f23\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 640(x18)\n"
			"flw.ps f3, 736(x18)\n"
			"flt.pi f12, f24, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 320(x6)\n"
			"fcvt.wu.s x5, f20, rne\n" 
			"li x11, 0x7d8a5a03e762cd4f\n"
			"fcvt.s.lu f15, x11, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 320(x19)\n"
			"flw.ps f27, 416(x19)\n"
			"flw.ps f12, 896(x19)\n"
			"fmsub.ps f13, f1, f27, f12, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 544(x19)\n"
			"fsin.ps f21, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 608(x14)\n"
			"flw.ps f10, 192(x14)\n"
			"flw.ps f28, 640(x14)\n"
			"fnmadd.s f18, f5, f10, f28, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_8:\n"
			"fltm.pi m1, f13, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f9, 768(x22)\n"
			"fcvt.ps.pwu f27, f9, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_666556_HID_8:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_112109_HID_8:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x13 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f15, 768(x14)\n"
			"flw.ps f6, 160(x14)\n"
			"fsrl.pi f14, f15, f6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 608(x7)\n"
			"flw.ps f2, 512(x7)\n"
			"flw.ps f3, 928(x7)\n"
			"fnmsub.s f29, f11, f2, f3, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 608(x17)\n"
			"flw.ps f25, 128(x17)\n"
			"fadd.ps f18, f9, f25, rmm\n" 
			"li x10, 0x4ac0a87f5fd15668\n"
			"fcvt.s.w f3, x10, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 544(x14)\n"
			"flw.ps f18, 384(x14)\n"
			"feq.ps f17, f3, f18\n" 
			"maskpopcz x26, m1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 672(x5)\n"
			"flw.ps f3, 160(x5)\n"
			"feq.ps f22, f18, f3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 192(x17)\n"
			"flw.ps f3, 992(x17)\n"
			"fsgnjn.ps f6, f12, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 448(x9)\n"
			"fsat8.pi f23, f11\n" 
			"li x27, 0x5ae454f6bf7ce061\n"
			"fcvt.s.w f12, x27, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 480(x5)\n"
			"flw.ps f8, 544(x5)\n"
			"fle.ps f30, f3, f8\n" 
			"li x5, 0xa6ac9a1fe88b4866\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x1abd9e9c4a16a984\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 832(x6)\n"
			"fclass.s x13, f30\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 800(x9)\n"
			"ffrc.ps f23, f5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 480(x18)\n"
			"fcvt.ps.pwu f4, f21, rmm\n" 
			"li x5, 0x1a31\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_8:\n"
			"masknot m2, m7\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x12, rand_int_32\n"
			"flw.ps f8, 448(x12)\n"
			"fsrai.pi f6, f8, 0x4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
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
			"la x7, rand_ieee754_16\n"
			"flw.ps f27, 448(x7)\n"
			"fcvt.ps.f16 f28, f27\n" 
			"maskpopcz x17, m3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 960(x9)\n"
			"flw.ps f18, 672(x9)\n"
			"flw.ps f10, 864(x9)\n"
			"fnmsub.ps f19, f27, f18, f10, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 992(x5)\n"
			"flw.ps f22, 928(x5)\n"
			"fle.s x11, f5, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f20, 736(x20)\n"
			"flw.ps f26, 96(x20)\n"
			"fdiv.pi f18, f20, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 160(x9)\n"
			"flw.ps f26, 96(x9)\n"
			"fsrl.pi f17, f20, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 512(x20)\n"
			"flw.ps f24, 352(x20)\n"
			"flw.ps f30, 192(x20)\n"
			"fnmsub.ps f15, f14, f24, f30, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 608(x6)\n"
			"flw.ps f17, 448(x6)\n"
			"fsgnjn.ps f1, f13, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 320(x6)\n"
			"flw.ps f18, 928(x6)\n"
			"frem.pi f15, f29, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskor m1, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 928(x28)\n"
			"flw.ps f28, 864(x28)\n"
			"fsgnj.ps f7, f26, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 352(x12)\n"
			"fmin.ps f7, f24, f24\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 288(x28)\n"
			"fnot.pi f28, f7\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 608(x5)\n"
			"flw.ps f1, 160(x5)\n"
			"fsub.pi f5, f22, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 384(x19)\n"
			"flw.ps f10, 864(x19)\n"
			"fltu.pi f13, f5, f10\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 352(x9)\n"
			"fcvt.pw.ps f23, f21, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_9:\n"
			"maskor m0, m1, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401A8\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 704(x18)\n"
			"flw.ps f13, 224(x18)\n"
			"fmul.ps f27, f6, f13, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"mov.m.x m1, x16, 0x60\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f24, 224(x27)\n"
			"flw.ps f22, 0(x27)\n"
			"fminu.pi f23, f24, f22\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 224(x28)\n"
			"flw.ps f2, 352(x28)\n"
			"fdiv.pi f22, f12, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 768(x16)\n"
			"flw.ps f23, 288(x16)\n"
			"flw.ps f24, 64(x16)\n"
			"fmsub.ps f30, f1, f23, f24, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 128(x17)\n"
			"fpackrepb.pi f20, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 160(x19)\n"
			"flw.ps f6, 512(x19)\n"
			"flt.pi f19, f24, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 0(x5)\n"
			"flw.ps f29, 288(x5)\n"
			"flt.ps f12, f16, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"fltm.ps m3, f6, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 96(x26)\n"
			"fswizz.ps f5, f8, 0x1c\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"maskxor m1, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f7, 0x23046\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_9:\n"
			"flem.ps m5, f6, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xfdead0ef3\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 864(x14)\n"
			"fclass.s x11, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 768(x28)\n"
			"flw.ps f4, 224(x28)\n"
			"fadd.s f23, f13, f4, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 864(x22)\n"
			"flw.ps f4, 448(x22)\n"
			"fmul.ps f23, f18, f4, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 64(x18)\n"
			"flw.ps f26, 320(x18)\n"
			"feq.ps f29, f9, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 0(x23)\n"
			"flw.ps f24, 192(x23)\n"
			"flw.ps f3, 800(x23)\n"
			"fmsub.s f2, f27, f24, f3, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 352(x26)\n"
			"flw.ps f4, 96(x26)\n"
			"fxor.pi f13, f7, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 352(x28)\n"
			"fcvt.pwu.ps f8, f25, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_9:\n"
			"fsetm.pi m1, f18\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5736919cc2\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f18, 512(x19)\n"
			"fandi.pi f7, f18, 0xbc\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 768(x10)\n"
			"fclass.ps f24, f7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 800(x27)\n"
			"flw.ps f1, 0(x27)\n"
			"fsgnjn.s f27, f9, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 896(x16)\n"
			"flw.ps f15, 64(x16)\n"
			"feq.ps f10, f26, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 0(x28)\n"
			"flw.ps f11, 544(x28)\n"
			"fmin.ps f19, f18, f11\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"flem.ps m0, f8, f16\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f28, 704(x12)\n"
			"flw.ps f2, 64(x12)\n"
			"fmin.pi f23, f28, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_9:\n"
			"flem.ps m5, f7, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f17, 512(x16)\n"
			"flw.ps f3, 416(x16)\n"
			"fmin.pi f27, f17, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 384(x12)\n"
			"flw.ps f7, 480(x12)\n"
			"fadd.s f7, f29, f7, rtz\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 128(x17)\n"
			"flw.ps f16, 544(x17)\n"
			"flw.ps f21, 736(x17)\n"
			"fnmsub.ps f18, f7, f16, f21, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 96(x25)\n"
			"flw.ps f18, 352(x25)\n"
			"fmin.s f5, f31, f18\n" 
			"li x5, 0x6db8\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 864(x19)\n"
			"fsin.ps f12, f31\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 544(x13)\n"
			"ffrc.ps f22, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 928(x9)\n"
			"flw.ps f10, 224(x9)\n"
			"fmin.ps f1, f29, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 896(x11)\n"
			"flw.ps f22, 384(x11)\n"
			"flw.ps f12, 96(x11)\n"
			"fmsub.s f7, f2, f22, f12, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 704(x14)\n"
			"fsatu8.pi f18, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 448(x11)\n"
			"flw.ps f26, 160(x11)\n"
			"flw.ps f24, 0(x11)\n"
			"fcmov.ps f22, f8, f26, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"feqm.ps m7, f1, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f8, 608(x27)\n"
			"flw.ps f19, 672(x27)\n"
			"fdivu.pi f0, f8, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 896(x19)\n"
			"flw.ps f16, 32(x19)\n"
			"fmul.ps f15, f25, f16, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 608(x17)\n"
			"fclass.s x9, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 320(x27)\n"
			"flw.ps f9, 192(x27)\n"
			"fadd.s f1, f1, f9, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 480(x12)\n"
			"fpackreph.pi f15, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 224(x9)\n"
			"frsq.ps f23, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 32(x20)\n"
			"fcvt.ps.pw f1, f25, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x28, 0x220000000476000b\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 96(x7)\n"
			"flw.ps f2, 288(x7)\n"
			"flw.ps f19, 448(x7)\n"
			"fcmov.ps f21, f28, f2, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 576(x7)\n"
			"fclass.s x16, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 800(x25)\n"
			"flw.ps f2, 32(x25)\n"
			"flt.pi f30, f13, f2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 832(x12)\n"
			"flw.ps f22, 992(x12)\n"
			"fmul.s f9, f1, f22, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 480(x7)\n"
			"fround.ps f22, f5, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 960(x12)\n"
			"flw.ps f29, 768(x12)\n"
			"fmul.s f13, f25, f29, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_10:\n"
			"flem.ps m4, f6, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 192(x19)\n"
			"flw.ps f2, 0(x19)\n"
			"flw.ps f12, 864(x19)\n"
			"fmsub.s f8, f15, f2, f12, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 224(x26)\n"
			"flw.ps f11, 384(x26)\n"
			"feq.pi f20, f24, f11\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 128(x27)\n"
			"flw.ps f30, 384(x27)\n"
			"fmulhu.pi f9, f19, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 384(x12)\n"
			"flw.ps f15, 768(x12)\n"
			"for.pi f2, f13, f15\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 96(x14)\n"
			"flw.ps f10, 384(x14)\n"
			"fsrl.pi f8, f27, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 960(x10)\n"
			"flw.ps f26, 320(x10)\n"
			"fmax.pi f19, f22, f26\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_10:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 960(x17)\n"
			"frcp.ps f21, f8\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 128(x9)\n"
			"fpackreph.pi f4, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 64(x23)\n"
			"ffrc.ps f21, f18\n" 
			"li x5, 0x701912ec23b27bd2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xbecdb54c04b13705\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f31, 0(x11)\n"
			"flw.ps f25, 704(x11)\n"
			"fadd.pi f2, f31, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 96(x9)\n"
			"fsin.ps f20, f2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 512(x6)\n"
			"fcvt.ps.pwu f28, f29, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 384(x10)\n"
			"fandi.pi f1, f14, 0x1b7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 64(x9)\n"
			"flw.ps f8, 32(x9)\n"
			"fnmsub.ps f17, f0, f8, f8, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_10:\n"
			"mov.m.x m3, x11, 0x65\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f27, 736(x6)\n"
			"fsat8.pi f6, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 64(x23)\n"
			"flw.ps f28, 704(x23)\n"
			"flw.ps f10, 64(x23)\n"
			"fnmsub.ps f14, f23, f28, f10, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 480(x5)\n"
			"flw.ps f1, 0(x5)\n"
			"fmax.pi f29, f2, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 800(x26)\n"
			"flw.ps f17, 352(x26)\n"
			"fmul.s f14, f31, f17, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 32(x26)\n"
			"flw.ps f30, 160(x26)\n"
			"fsra.pi f30, f25, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f27, 224(x22)\n"
			"flw.ps f8, 960(x22)\n"
			"fsub.pi f3, f27, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 64(x12)\n"
			"frsq.ps f4, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f14, 800(x16)\n"
			"fpackreph.pi f8, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 128(x9)\n"
			"fsrai.pi f14, f12, 0x1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 480(x27)\n"
			"fslli.pi f1, f21, 0xe\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_102220_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_939756_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_450787_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
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
			"la x17, rand_int_32\n"
			"flw.ps f0, 896(x17)\n"
			"fslli.pi f28, f0, 0xb\n" 
			"la x28, rand_int_32\n"
			"flw.ps f3, 288(x28)\n"
			"flw.ps f31, 512(x28)\n"
			"fmaxu.pi f10, f3, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 512(x22)\n"
			"flw.ps f5, 672(x22)\n"
			"flw.ps f24, 160(x22)\n"
			"fnmadd.ps f24, f19, f5, f24, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 640(x10)\n"
			"flw.ps f26, 896(x10)\n"
			"fsub.ps f31, f3, f26, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 832(x11)\n"
			"flw.ps f6, 896(x11)\n"
			"fsub.ps f11, f17, f6, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 704(x18)\n"
			"fcvt.wu.s x12, f12, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"masknot m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f28, 352(x9)\n"
			"fsatu8.pi f0, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 0(x25)\n"
			"frsq.ps f18, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 576(x23)\n"
			"flw.ps f0, 448(x23)\n"
			"fsgnjx.s f11, f26, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskor m7, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f25, 0x465b3\n" 
			"la x16, rand_int_32\n"
			"flw.ps f23, 256(x16)\n"
			"fslli.pi f25, f23, 0x1\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 128(x22)\n"
			"fsrli.pi f8, f15, 0x1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 800(x20)\n"
			"fnot.pi f2, f1\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 800(x6)\n"
			"flw.ps f7, 0(x6)\n"
			"for.pi f14, f1, f7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f4, 192(x14)\n"
			"flw.ps f11, 704(x14)\n"
			"fsrl.pi f31, f4, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_11:\n"
			"maskor m4, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f8, 640(x28)\n"
			"flw.ps f28, 736(x28)\n"
			"fxor.pi f29, f8, f28\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 832(x16)\n"
			"fcvt.wu.s x13, f7, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f15, 608(x9)\n"
			"fsat8.pi f6, f15\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 608(x14)\n"
			"flw.ps f21, 224(x14)\n"
			"fmin.pi f29, f31, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 640(x18)\n"
			"flw.ps f20, 576(x18)\n"
			"fxor.pi f24, f13, f20\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 608(x6)\n"
			"faddi.pi f7, f30, 0x1f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"feqm.ps m0, f21, f25\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"fsetm.pi m6, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 832(x16)\n"
			"flw.ps f16, 768(x16)\n"
			"flw.ps f22, 320(x16)\n"
			"fnmadd.ps f12, f25, f16, f22, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 960(x19)\n"
			"fcvt.ps.pwu f25, f0, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 224(x10)\n"
			"flw.ps f22, 736(x10)\n"
			"fsgnjn.ps f15, f18, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"mov.m.x m6, x6, 0x77\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 0(x19)\n"
			"flw.ps f10, 768(x19)\n"
			"fsgnj.ps f11, f3, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f27, 288(x17)\n"
			"fsrai.pi f21, f27, 0x5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 640(x18)\n"
			"flw.ps f19, 96(x18)\n"
			"flw.ps f4, 352(x18)\n"
			"fmsub.ps f19, f2, f19, f4, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"maskxor m3, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 736(x10)\n"
			"fcvt.wu.s x12, f13, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 608(x18)\n"
			"flw.ps f5, 608(x18)\n"
			"fmulhu.pi f25, f7, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 192(x13)\n"
			"flw.ps f15, 768(x13)\n"
			"fle.pi f17, f4, f15\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 384(x23)\n"
			"flw.ps f0, 576(x23)\n"
			"fmul.pi f12, f14, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 544(x27)\n"
			"flw.ps f19, 416(x27)\n"
			"fsra.pi f16, f1, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 896(x10)\n"
			"fnot.pi f26, f1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 384(x6)\n"
			"flw.ps f22, 768(x6)\n"
			"fsgnjx.s f18, f21, f22\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 0(x22)\n"
			"fandi.pi f0, f12, 0x130\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_11:\n"
			"fltm.pi m3, f5, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 992(x14)\n"
			"ffrc.ps f5, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 832(x14)\n"
			"flw.ps f24, 832(x14)\n"
			"fmin.pi f19, f6, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 96(x17)\n"
			"flw.ps f28, 608(x17)\n"
			"feq.pi f10, f30, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 384(x13)\n"
			"flw.ps f18, 992(x13)\n"
			"fmin.s f16, f31, f18\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 64(x7)\n"
			"flw.ps f2, 896(x7)\n"
			"fsgnj.s f15, f9, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 160(x9)\n"
			"fsqrt.s f30, f10, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 608(x20)\n"
			"flw.ps f21, 64(x20)\n"
			"fle.ps f23, f24, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 448(x17)\n"
			"flw.ps f8, 480(x17)\n"
			"flw.ps f7, 544(x17)\n"
			"fmsub.s f13, f16, f8, f7, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 832(x15)\n"
			"flw.ps f7, 480(x15)\n"
			"flt.pi f14, f19, f7\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 576(x11)\n"
			"fcvt.ps.pwu f22, f16, rtz\n" 
			"li x5, 0x7b56\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f12, 352(x5)\n"
			"flw.ps f24, 0(x5)\n"
			"fmulh.pi f2, f12, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 704(x18)\n"
			"flw.ps f9, 0(x18)\n"
			"flw.ps f27, 416(x18)\n"
			"fmadd.ps f9, f12, f9, f27, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 160(x14)\n"
			"ffrc.ps f8, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 0(x9)\n"
			"flw.ps f29, 256(x9)\n"
			"fdiv.s f8, f17, f29, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 448(x13)\n"
			"flw.ps f1, 768(x13)\n"
			"flw.ps f6, 160(x13)\n"
			"fnmadd.s f2, f0, f1, f6, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 544(x15)\n"
			"fclass.ps f8, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 672(x25)\n"
			"flw.ps f25, 64(x25)\n"
			"fle.ps f19, f22, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 448(x20)\n"
			"flw.ps f13, 736(x20)\n"
			"flt.ps f2, f20, f13\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 416(x15)\n"
			"flw.ps f1, 32(x15)\n"
			"fsub.pi f1, f22, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 320(x25)\n"
			"flw.ps f13, 864(x25)\n"
			"fadd.ps f7, f22, f13, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x10, 0x220000000476000b\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_12:\n"
			"feqm.ps m5, f21, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x6676a1c0d2708fc3\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x942017551b212329\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 384(x26)\n"
			"flw.ps f27, 576(x26)\n"
			"flw.ps f29, 736(x26)\n"
			"fnmsub.ps f13, f11, f27, f29, rtz\n" 
			"mova.x.m x27\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 384(x28)\n"
			"flw.ps f4, 864(x28)\n"
			"fmul.s f6, f29, f4, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 128(x18)\n"
			"flw.ps f12, 256(x18)\n"
			"fsgnj.ps f0, f15, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 544(x7)\n"
			"flw.ps f3, 96(x7)\n"
			"feq.s x27, f10, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f25, 288(x9)\n"
			"flw.ps f13, 256(x9)\n"
			"fand.pi f30, f25, f13\n" 
			"fbci.ps f23, 0x38d28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 736(x11)\n"
			"flw.ps f11, 992(x11)\n"
			"feq.ps f13, f1, f11\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 608(x15)\n"
			"fpackreph.pi f1, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_12:\n"
			"feqm.ps m7, f22, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"maskor m6, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 576(x26)\n"
			"fcvt.w.s x16, f6, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 832(x16)\n"
			"flw.ps f9, 352(x16)\n"
			"fsgnj.ps f4, f28, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 672(x15)\n"
			"flw.ps f20, 640(x15)\n"
			"fnmsub.s f27, f24, f20, f20, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 256(x27)\n"
			"flw.ps f20, 992(x27)\n"
			"feq.ps f26, f5, f20\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"li x28, 0xdd961df3c13b50b8\n"
			"fcvt.s.wu f30, x28, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 544(x6)\n"
			"flw.ps f23, 896(x6)\n"
			"fltu.pi f18, f22, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 576(x9)\n"
			"fswizz.ps f20, f9, 0x1f\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 992(x17)\n"
			"flw.ps f30, 32(x17)\n"
			"fmulh.pi f17, f18, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 320(x15)\n"
			"flw.ps f16, 448(x15)\n"
			"fmadd.ps f27, f6, f6, f16, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 736(x5)\n"
			"flw.ps f10, 768(x5)\n"
			"fle.pi f6, f20, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 928(x22)\n"
			"flw.ps f30, 96(x22)\n"
			"flt.ps f14, f16, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 512(x25)\n"
			"flw.ps f13, 768(x25)\n"
			"feq.s x17, f19, f13\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 192(x27)\n"
			"flw.ps f22, 224(x27)\n"
			"fle.pi f30, f15, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 864(x13)\n"
			"flw.ps f29, 384(x13)\n"
			"flt.ps f30, f10, f29\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 768(x5)\n"
			"flw.ps f9, 32(x5)\n"
			"fmul.ps f27, f31, f9, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 0(x7)\n"
			"flw.ps f18, 928(x7)\n"
			"fadd.pi f10, f24, f18\n" 
			"li x18, 0xd4c84c7b1915d3d5\n"
			"fcvt.s.wu f7, x18, rmm\n" 
			"li x5, 0x45ae0b18d2877571\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x89097fa689393b36\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_12:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 384(x27)\n"
			"flog.ps f7, f15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"maskand m6, m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"fsetm.pi m3, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_921549_HID_12:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
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
			"LBL_KERNEL_HID_13:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_13\n"
			"li x5, 0x3\n" 
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 448(x14)\n"
			"flw.ps f5, 256(x14)\n"
			"fmin.ps f13, f9, f5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 160(x6)\n"
			"fsrli.pi f13, f23, 0xf\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 640(x16)\n"
			"flw.ps f24, 384(x16)\n"
			"fsgnj.s f12, f26, f24\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 640(x11)\n"
			"fcvt.ps.pwu f24, f5, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 128(x19)\n"
			"flw.ps f5, 768(x19)\n"
			"fmin.s f2, f27, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 32(x20)\n"
			"flw.ps f4, 320(x20)\n"
			"fmul.pi f22, f7, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 480(x27)\n"
			"fcvt.l.s x27, f28, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 704(x19)\n"
			"fcvt.pwu.ps f18, f12, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 768(x17)\n"
			"flw.ps f1, 640(x17)\n"
			"fmin.pi f10, f13, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 384(x19)\n"
			"flw.ps f16, 576(x19)\n"
			"flw.ps f11, 160(x19)\n"
			"fnmadd.s f15, f25, f16, f11, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 128(x5)\n"
			"flw.ps f13, 448(x5)\n"
			"flw.ps f7, 512(x5)\n"
			"fcmov.ps f23, f14, f13, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"mov.m.x m2, x16, 0x7c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 896(x6)\n"
			"flw.ps f14, 192(x6)\n"
			"fmax.ps f8, f2, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 544(x22)\n"
			"flw.ps f20, 32(x22)\n"
			"fsub.pi f23, f24, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 480(x7)\n"
			"frsq.ps f6, f0\n" 
			"fbci.ps f19, 0x41bea\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 832(x9)\n"
			"flw.ps f25, 800(x9)\n"
			"fsub.ps f5, f12, f25, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 864(x14)\n"
			"fsatu8.pi f25, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 640(x20)\n"
			"fcvt.ps.pw f10, f21, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 160(x25)\n"
			"fclass.s x12, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_13:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 736(x12)\n"
			"fexp.ps f26, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 544(x23)\n"
			"fsin.ps f4, f26\n" 
			"mova.x.m x15\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 384(x22)\n"
			"flw.ps f9, 448(x22)\n"
			"fmul.pi f21, f7, f9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f18, 0(x10)\n"
			"flw.ps f23, 224(x10)\n"
			"for.pi f2, f18, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 352(x15)\n"
			"fcvt.f16.ps f11, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 512(x15)\n"
			"flw.ps f14, 704(x15)\n"
			"fmaxu.pi f29, f2, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 0(x28)\n"
			"fcvt.f16.ps f15, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 0(x20)\n"
			"fcvt.w.s x23, f29, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fltm.ps m3, f16, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"masknot m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 640(x10)\n"
			"flw.ps f20, 608(x10)\n"
			"fsgnj.ps f29, f8, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f0, 480(x14)\n"
			"flw.ps f18, 992(x14)\n"
			"fdiv.pi f13, f0, f18\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 64(x22)\n"
			"flw.ps f29, 672(x22)\n"
			"fsgnjn.s f28, f26, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 96(x6)\n"
			"flw.ps f14, 320(x6)\n"
			"fle.pi f0, f17, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 896(x25)\n"
			"fround.ps f17, f13, rmm\n" 
			"li x10, 0xf10cdb797b1e76ec\n"
			"fcvt.s.w f24, x10, rne\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f25, 416(x19)\n"
			"fcvt.ps.f16 f15, f25\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_13:\n"
			"feqm.ps m1, f17, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 352(x18)\n"
			"flw.ps f22, 896(x18)\n"
			"fminu.pi f4, f1, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 672(x23)\n"
			"flw.ps f4, 384(x23)\n"
			"fle.ps f23, f3, f4\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 352(x7)\n"
			"flw.ps f11, 928(x7)\n"
			"fsrl.pi f23, f24, f11\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"mov.m.x m3, x6, 0x3c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f3, 608(x27)\n"
			"fnot.pi f19, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 640(x14)\n"
			"flw.ps f3, 256(x14)\n"
			"fsgnj.s f20, f20, f3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 384(x5)\n"
			"flw.ps f29, 352(x5)\n"
			"fmul.pi f9, f26, f29\n" 
			"mova.x.m x11\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 736(x6)\n"
			"fpackreph.pi f29, f16\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f25,  608 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"masknot m2, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6116f928ed\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xcd51bdb389b8d4a8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x98525e186521007e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 704(x23)\n"
			"flw.ps f30, 832(x23)\n"
			"fsgnj.s f21, f1, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f11, 416(x6)\n"
			"fpackreph.pi f21, f11\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 480(x13)\n"
			"flw.ps f7, 128(x13)\n"
			"fadd.pi f26, f4, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 544(x22)\n"
			"flw.ps f11, 832(x22)\n"
			"flw.ps f17, 192(x22)\n"
			"fmsub.ps f6, f5, f11, f17, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 384(x23)\n"
			"fsat8.pi f10, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 896(x19)\n"
			"fcvt.l.s x13, f1, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 960(x12)\n"
			"flw.ps f2, 608(x12)\n"
			"flw.ps f30, 320(x12)\n"
			"fmadd.s f28, f0, f2, f30, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 896(x16)\n"
			"fpackreph.pi f17, f17\n" 
			"li x15, 0x9b8814b9f119ba79\n"
			"fcvt.s.l f14, x15, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 544(x20)\n"
			"fclass.ps f26, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 224(x5)\n"
			"flw.ps f20, 32(x5)\n"
			"fsub.s f25, f19, f20, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f6, 384(x5)\n"
			"flw.ps f19, 160(x5)\n"
			"fand.pi f20, f6, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 928(x17)\n"
			"flw.ps f28, 704(x17)\n"
			"fmin.s f29, f6, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 288(x28)\n"
			"fcvt.pwu.ps f25, f29, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 160(x14)\n"
			"flw.ps f29, 576(x14)\n"
			"fmin.pi f25, f14, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_630852_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x7c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_14:\n"
			"li x11, 0x220000000476000b\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"fbci.ps f14, 0x5d7ce\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 480(x16)\n"
			"flw.ps f0, 832(x16)\n"
			"fsgnj.s f7, f12, f0\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 608(x10)\n"
			"flw.ps f19, 576(x10)\n"
			"flw.ps f3, 928(x10)\n"
			"fnmsub.ps f30, f9, f19, f3, rne\n" 
			"li x18, 0x31f7787db14bb3b6\n"
			"fcvt.s.l f31, x18, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 992(x27)\n"
			"flw.ps f4, 224(x27)\n"
			"flw.ps f10, 288(x27)\n"
			"fmsub.s f10, f24, f4, f10, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 320(x20)\n"
			"faddi.pi f6, f7, 0x1f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 704(x14)\n"
			"flw.ps f0, 960(x14)\n"
			"fmax.s f29, f7, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f12, 352(x6)\n"
			"flw.ps f30, 768(x6)\n"
			"fmin.pi f29, f12, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 480(x23)\n"
			"fround.ps f29, f28, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_14:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 256(x19)\n"
			"fexp.ps f9, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"maskxor m0, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 480(x25)\n"
			"flw.ps f12, 0(x25)\n"
			"flw.ps f20, 64(x25)\n"
			"fmadd.s f20, f11, f12, f20, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 96(x10)\n"
			"fsqrt.s f28, f20, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 0(x27)\n"
			"fsin.ps f4, f18\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 128(x23)\n"
			"fcvt.l.s x18, f8, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 448(x7)\n"
			"flw.ps f17, 416(x7)\n"
			"fadd.ps f0, f5, f17, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_421985_HID_14:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 736(x14)\n"
			"flw.ps f2, 640(x14)\n"
			"flt.ps f10, f0, f2\n" 
			"mova.x.m x25\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 64(x18)\n"
			"fcvt.ps.pwu f19, f7, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 0(x13)\n"
			"flw.ps f29, 448(x13)\n"
			"feq.ps f26, f24, f29\n" 
			"li x5, 0x6f81\n"
			"csrw tensor_mask, x5\n"	
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 832(x14)\n"
			"fswizz.ps f6, f18, 0x1b\n" 
			"li x5, 0xc5ed\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 576(x11)\n"
			"fcvt.pwu.ps f30, f8, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 832(x16)\n"
			"fsrai.pi f19, f2, 0x0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 896(x5)\n"
			"flw.ps f18, 512(x5)\n"
			"feq.ps f7, f25, f18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 960(x14)\n"
			"fcvt.ps.pwu f11, f5, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 608(x20)\n"
			"frsq.ps f31, f25\n" 
			"li x5, 0xa1261fc777a73773\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7a4f2dd60a5ac3c4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 576(x19)\n"
			"flw.ps f18, 64(x19)\n"
			"flw.ps f21, 960(x19)\n"
			"fmsub.s f14, f9, f18, f21, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 832(x5)\n"
			"flw.ps f18, 672(x5)\n"
			"fle.ps f8, f29, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 352(x15)\n"
			"flw.ps f17, 224(x15)\n"
			"flw.ps f7, 832(x15)\n"
			"fmadd.s f15, f23, f17, f7, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 960(x9)\n"
			"fpackreph.pi f3, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 864(x17)\n"
			"fround.ps f10, f26, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_14:\n"
			"feqm.ps m1, f28, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 448(x13)\n"
			"fclass.s x13, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 32(x18)\n"
			"ffrc.ps f10, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 768(x27)\n"
			"flw.ps f13, 320(x27)\n"
			"fmax.ps f12, f30, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 128(x15)\n"
			"flw.ps f5, 224(x15)\n"
			"flt.ps f13, f30, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 896(x9)\n"
			"flw.ps f4, 768(x9)\n"
			"fsgnjx.s f16, f13, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"mov.m.x m5, x15, 0x3b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 608(x27)\n"
			"flw.ps f2, 320(x27)\n"
			"fsgnjn.s f7, f30, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 352(x10)\n"
			"fcvt.ps.pw f26, f16, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 416(x28)\n"
			"fand.pi f4, f29, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 512(x7)\n"
			"flw.ps f12, 384(x7)\n"
			"fsgnjx.s f10, f6, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"fltm.pi m0, f20, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_15:\n"
			"mov.m.x m4, x17, 0x4b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f12, 320(x9)\n"
			"flw.ps f22, 768(x9)\n"
			"flt.pi f11, f12, f22\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 640(x11)\n"
			"fsub.s f25, f23, f23, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 288(x16)\n"
			"fcvt.ps.pwu f26, f8, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 256(x28)\n"
			"flw.ps f3, 832(x28)\n"
			"fmax.s f13, f31, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 96(x15)\n"
			"flw.ps f2, 352(x15)\n"
			"fmin.ps f2, f28, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"mov.m.x m5, x27, 0x3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 352(x5)\n"
			"fpackrepb.pi f8, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 96(x23)\n"
			"fnot.pi f29, f4\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_15:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 288(x18)\n"
			"flog.ps f18, f24\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 128(x18)\n"
			"flw.ps f23, 576(x18)\n"
			"fdiv.pi f19, f21, f23\n" 
			"li x12, 0x377dab6f0be434e3\n"
			"fcvt.s.w f19, x12, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 928(x23)\n"
			"flw.ps f5, 96(x23)\n"
			"flw.ps f10, 256(x23)\n"
			"fmadd.s f29, f26, f5, f10, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 704(x7)\n"
			"fsrli.pi f3, f0, 0x6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 320(x26)\n"
			"flw.ps f11, 768(x26)\n"
			"flw.ps f1, 192(x26)\n"
			"fnmadd.s f19, f8, f11, f1, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 512(x6)\n"
			"flw.ps f3, 576(x6)\n"
			"fsgnj.s f20, f29, f3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f3, 448(x15)\n"
			"fsatu8.pi f11, f3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 576(x23)\n"
			"flw.ps f29, 320(x23)\n"
			"fmin.pi f21, f3, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 320(x26)\n"
			"flw.ps f30, 320(x26)\n"
			"flw.ps f7, 96(x26)\n"
			"fmadd.ps f21, f4, f30, f7, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 416(x12)\n"
			"flw.ps f14, 224(x12)\n"
			"fmulh.pi f13, f18, f14\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 64(x27)\n"
			"flw.ps f28, 800(x27)\n"
			"fmin.s f28, f22, f28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 128(x13)\n"
			"flw.ps f3, 736(x13)\n"
			"fltu.pi f24, f28, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 544(x15)\n"
			"flw.ps f4, 800(x15)\n"
			"fmin.s f1, f15, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 32(x12)\n"
			"flw.ps f2, 576(x12)\n"
			"fmin.ps f0, f18, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 32(x16)\n"
			"fcvt.pw.ps f0, f28, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 864(x5)\n"
			"fround.ps f12, f9, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"maskxor m0, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 544(x26)\n"
			"fclass.s x7, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_15:\n"
			"mov.m.x m6, x18, 0x2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f10, 0x3483e\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_15:\n"
			"maskor m6, m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f24, 384(x7)\n"
			"fnot.pi f14, f24\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 320(x11)\n"
			"flw.ps f14, 192(x11)\n"
			"fltu.pi f26, f6, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 864(x22)\n"
			"flw.ps f26, 256(x22)\n"
			"fsgnjx.s f29, f21, f26\n" 
			"fbci.pi f22, 0x72082\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_15:\n"
			"maskor m7, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
		0xe6535443, 
		0x721bcba5, 
		0xdaad20c8, 
		0x0e7dbe26, 
		0x9bd3ed5d, 
		0x8de67774, 
		0x714f3b81, 
		0xc807feb5, 
		0x3ca2e903, 
		0x84185cbe, 
		0x3cc6fdad, 
		0x8a8408b1, 
		0x7cb5bc43, 
		0xb54e86e3, 
		0x22d1f542, 
		0x9481bd30, 
		0x4efe9f6c, 
		0x1badf398, 
		0x3b7bf1ad, 
		0x6e316072, 
		0xecd1e55d, 
		0x506d94a1, 
		0xdd130c55, 
		0xdf82f89c, 
		0x051459ce, 
		0xbbad4d4f, 
		0x8a18198c, 
		0xbff4a6f0, 
		0xf1f32b27, 
		0x01279bf9, 
		0x52ca09fb, 
		0x07c8f2ce, 
		0x5cff2605, 
		0xfee2373e, 
		0x4d8bf1b4, 
		0x6b453e70, 
		0xfe4d30dc, 
		0xa9e2a47f, 
		0x2b09ab8b, 
		0xed9b4a87, 
		0x4a75cc73, 
		0x38b7c61f, 
		0x01a69f9f, 
		0x0e6050e5, 
		0xff046e3b, 
		0x8e585688, 
		0x792b430c, 
		0x251ef39c, 
		0xf54d5e47, 
		0xc8e8bfa5, 
		0x293c26d4, 
		0xac9777f2, 
		0x041fa06e, 
		0x1f6aa2f3, 
		0x46488ba9, 
		0x53f67ed3, 
		0x4d15bf4a, 
		0xb378f5a0, 
		0x92433e24, 
		0xc1520868, 
		0x7451b0b2, 
		0xfb87d20d, 
		0x5238dc0e, 
		0xe11dcbc4, 
		0x9625c517, 
		0x23d045e7, 
		0x59e74114, 
		0x6e265973, 
		0x47ed7654, 
		0x0730ac5c, 
		0xc784466e, 
		0x271e6fc0, 
		0x9763bd02, 
		0x9fcd4a5f, 
		0xe30f8128, 
		0x80777ab3, 
		0x6388cb3f, 
		0x67fb4758, 
		0x53d613f7, 
		0x2fd7fbca, 
		0x9bb05eb3, 
		0xec99f5df, 
		0xfff6b52d, 
		0x55de588d, 
		0x09965871, 
		0x352e3997, 
		0xb95b2c41, 
		0xe16ac0a5, 
		0x728122c3, 
		0x4a35d10a, 
		0xc300b87b, 
		0x6e30a9e6, 
		0x93231255, 
		0x87f5776d, 
		0x28c03fca, 
		0x0b760419, 
		0xfde0845a, 
		0xbbc07ccd, 
		0x6d7ebf4c, 
		0x6fd95d76, 
		0x1e44bd24, 
		0x3f4785d9, 
		0xf784f269, 
		0x38acd01e, 
		0x109edaf3, 
		0x0d599f50, 
		0x420c2943, 
		0xfe9012e5, 
		0xc72d79ac, 
		0xdc43b4c6, 
		0xf427ed8b, 
		0x24c3c69d, 
		0x3ae3bfe0, 
		0x3c732dbf, 
		0xb91e2c08, 
		0xb6a25a34, 
		0x78ecf1fe, 
		0x55f6f895, 
		0xaec0c167, 
		0xce44f3c9, 
		0xb431e9ba, 
		0x83c70e52, 
		0xcabb4831, 
		0xc138dbda, 
		0xa0ff06d7, 
		0xb0e07487, 
		0x1f053356, 
		0xe7c8e3c4, 
		0x0b87714b, 
		0x39a997c8, 
		0xf3e9808b, 
		0xd8cea2ac, 
		0x916bbe61, 
		0xd974d7c6, 
		0x20361db5, 
		0x53afd4cf, 
		0x6ed86edd, 
		0x5f6a0294, 
		0x1e0ba4c6, 
		0x22da993d, 
		0xbfcc640a, 
		0x4d01d844, 
		0x841ae545, 
		0xd73b7201, 
		0x1b7b97d4, 
		0xe4571609, 
		0x9dcbadb6, 
		0x9c07e584, 
		0x4ba8039c, 
		0x01c07db0, 
		0xd32809bf, 
		0x5b8d4c14, 
		0xb64f824e, 
		0x24ed6c80, 
		0xd72d230f, 
		0x860ea459, 
		0x10453c14, 
		0x7255cb4f, 
		0x46bbd488, 
		0x009bf747, 
		0xd99528d4, 
		0xf6683663, 
		0xef7fffe7, 
		0xf652f134, 
		0x8ca40333, 
		0xdb1bc108, 
		0xc188d654, 
		0x8dc9c727, 
		0xd6e7bca4, 
		0x47cbae7f, 
		0x7ca37f8d, 
		0xec7e29d7, 
		0x46df9d3a, 
		0xea6e366e, 
		0xa2f8b6b7, 
		0x96017b49, 
		0x097d01c0, 
		0xc1196de1, 
		0x9f2fcc95, 
		0x9a2533fd, 
		0xe5da3cd7, 
		0xbca90748, 
		0x231fe85c, 
		0xb0a747ea, 
		0x547660ae, 
		0x685eda86, 
		0x7b4ce166, 
		0x1b267f45, 
		0x229e662d, 
		0x9be5da5f, 
		0xcd3c1e69, 
		0x2cc7e1a6, 
		0x5e6c3cd9, 
		0xfb888bab, 
		0xfac2fbd4, 
		0xed70b147, 
		0xd76aacfa, 
		0xe8764b41, 
		0x1bb665db, 
		0x2b3084f9, 
		0xe66007cd, 
		0x49832ece, 
		0xd223820c, 
		0x5241ca94, 
		0x50c416b6, 
		0xfb2e9459, 
		0x5d813ba8, 
		0x40a8a6b7, 
		0xb5e1e804, 
		0x86062466, 
		0x31d7edee, 
		0x8ab97188, 
		0x947525ef, 
		0x6b1b473b, 
		0xbc8a890f, 
		0xcf160f49, 
		0x96cc90ec, 
		0x5afe9add, 
		0xf2121aca, 
		0xf18fd5d9, 
		0xc6febfe9, 
		0x3083e556, 
		0x2bcd7bb6, 
		0xeaa2c5ce, 
		0xc4fe3052, 
		0xe11369f8, 
		0x08c739cb, 
		0x95f699d5, 
		0xae89e707, 
		0xe5db1738, 
		0xf5cd15ca, 
		0x394cef7e, 
		0x176dd62b, 
		0xd2ea304b, 
		0x4d6c10a1, 
		0xb837f705, 
		0x8ec8a10f, 
		0xa1e5b5a9, 
		0xbe4e1323, 
		0x7062c7f7, 
		0x2d1bdaff, 
		0x2d791509, 
		0x9d5bb12a, 
		0xcd7c0e62, 
		0xda722803, 
		0xdb52aed0, 
		0x061dde3b, 
		0x70728558, 
		0x5ee30712, 
		0x3e707ae8, 
		0x5c5f1b9d, 
		0x815907ff, 
		0xafba9cab, 
		0xdd097a56, 
		0x820def72, 
		0xa1af8587  
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
		0xd2a0, 
		0xcd40, 
		0x3c00, 
		0x4415, 
		0x4400, 
		0x4800, 
		0x4900, 
		0x4200, 
		0x4235, 
		0xb322, 
		0xd060, 
		0xc000, 
		0x0cda, 
		0xd220, 
		0x4a80, 
		0x5354, 
		0x5100, 
		0x1926, 
		0x6f54, 
		0x4a80, 
		0xd3e0, 
		0xad8a, 
		0xfced, 
		0xcd80, 
		0xd2a0, 
		0xfc90, 
		0x6b35, 
		0xc980, 
		0x4f40, 
		0xd0a0, 
		0x52e0, 
		0x4f00, 
		0xc700, 
		0x7c9a, 
		0x2fb1, 
		0xf6b2, 
		0xc000, 
		0x69c0, 
		0x4e40, 
		0xfc00, 
		0xd020, 
		0xf01a, 
		0x4f00, 
		0x914b, 
		0x4e80, 
		0x5220, 
		0xce60, 
		0x4700, 
		0x4b80, 
		0xcc80, 
		0x9815, 
		0x7dcf, 
		0x4980, 
		0xcd80, 
		0x4800, 
		0x8000, 
		0x5000, 
		0x70f0, 
		0x53a0, 
		0x951d, 
		0x453d, 
		0xc000, 
		0xd060, 
		0x4980, 
		0x8000, 
		0xd0e0, 
		0xd220, 
		0xfc00, 
		0xd240, 
		0xd360, 
		0x9522, 
		0xf22c, 
		0xcf80, 
		0x26bb, 
		0xd220, 
		0xcc80, 
		0x8210, 
		0x53e0, 
		0x4200, 
		0x51a0, 
		0xd1c0, 
		0xb449, 
		0x4dc0, 
		0x4d40, 
		0x7cda, 
		0x4500, 
		0x32d4, 
		0x8daa, 
		0xd360, 
		0x8b6b, 
		0x8000, 
		0x60e3, 
		0xd7e6, 
		0x4f40, 
		0xc600, 
		0xcc00, 
		0x7c00, 
		0xcfc0, 
		0xcb00, 
		0x9841, 
		0xd1a0, 
		0x8000, 
		0xd120, 
		0x0a8b, 
		0x78b4, 
		0x557b, 
		0xc900, 
		0x6484, 
		0xd2a0, 
		0x7c00, 
		0x66b5, 
		0x7afe, 
		0x21b3, 
		0xae6e, 
		0xd0c0, 
		0xce80, 
		0xd240, 
		0x4e00, 
		0x0dd9, 
		0xd200, 
		0xd020, 
		0x4cc0, 
		0xc723, 
		0xd320, 
		0x8514, 
		0xd200, 
		0x50a0, 
		0x4400, 
		0xd2c0, 
		0x8000, 
		0xb605, 
		0xaea0, 
		0x50e0, 
		0xce40, 
		0xccc0, 
		0x9875, 
		0xd6e5, 
		0x7c1d, 
		0x5240, 
		0xd3c0, 
		0x5300, 
		0x4d67, 
		0xf589, 
		0xd060, 
		0x5000, 
		0xd0c0, 
		0xd1c0, 
		0xd380, 
		0x4dc0, 
		0x4200, 
		0x4fc0, 
		0x5140, 
		0x2820, 
		0xcc40, 
		0x5060, 
		0x4f80, 
		0x51c0, 
		0x5180, 
		0xba19, 
		0x0336, 
		0x50e0, 
		0x7c00, 
		0xcc00, 
		0x51c0, 
		0x3027, 
		0xd360, 
		0x0000, 
		0x219a, 
		0x4980, 
		0xd040, 
		0x52c0, 
		0x63cb, 
		0x5160, 
		0x5340, 
		0xc800, 
		0xce40, 
		0x4800, 
		0x5320, 
		0x8000, 
		0x4200, 
		0x4c00, 
		0x830a, 
		0x4b00, 
		0x50a0, 
		0x5180, 
		0x5320, 
		0x51a0, 
		0x53a0, 
		0xd120, 
		0x51e0, 
		0x8000, 
		0x905a, 
		0xc0d0, 
		0xd314, 
		0x4e00, 
		0x938e, 
		0x685b, 
		0x5340, 
		0xd3a0, 
		0x41b2, 
		0x2b6c, 
		0xe23d, 
		0x3ae3, 
		0x41e0, 
		0xcb00, 
		0x406e, 
		0x8000, 
		0x4700, 
		0x4a80, 
		0x0e14, 
		0x4a80, 
		0x7c00, 
		0xd180, 
		0xd100, 
		0x0da1, 
		0xeb7e, 
		0xd120, 
		0x4fc0, 
		0xce20, 
		0xfc00, 
		0x7cd7, 
		0xd2e0, 
		0xce40, 
		0xd0e0, 
		0x9cfa, 
		0x5340, 
		0x6492, 
		0xc880, 
		0x1bb1, 
		0x96de, 
		0xbc00, 
		0x8000, 
		0x033c, 
		0xd220, 
		0xfc00, 
		0x5e0d, 
		0xacc4, 
		0xb5ca, 
		0x5160, 
		0x4200, 
		0x4f00, 
		0x2eb6, 
		0x5975, 
		0xed74, 
		0x6b8d, 
		0x30b3, 
		0xd300, 
		0xdf97, 
		0x52a0, 
		0x4f40, 
		0x80ad, 
		0x4d00, 
		0xfc00, 
		0x6772, 
		0xcc00, 
		0x5160, 
		0x283d, 
		0xa358, 
		0xe7e6, 
		0x7c00, 
		0x4800, 
		0x6b54, 
		0x678a, 
		0xcf00, 
		0x51c0, 
		0x4f80, 
		0xfc00, 
		0x8099, 
		0xfc83, 
		0x236b, 
		0xd2c0, 
		0x5020, 
		0x3c00, 
		0x52e0, 
		0x5000, 
		0xfc63, 
		0x7c00, 
		0x3c00, 
		0x0000, 
		0xc880, 
		0x4980, 
		0xe7fa, 
		0xd220, 
		0xce80, 
		0xd320, 
		0x4880, 
		0xd1c0, 
		0xcec0, 
		0x51c0, 
		0xd160, 
		0xa0e0, 
		0xd0e0, 
		0x7c00, 
		0x53c0, 
		0x51e0, 
		0x7cf6, 
		0xf24e, 
		0x4f00, 
		0x0acc, 
		0x5300, 
		0x5446, 
		0xd020, 
		0x8151, 
		0xcf00, 
		0xb583, 
		0x909f, 
		0xd1c0, 
		0x4e00, 
		0x52e0, 
		0x349c, 
		0x4600, 
		0xd2e0, 
		0x7ca7, 
		0xc500, 
		0x80ff, 
		0xd0c0, 
		0xadb3, 
		0x53c0, 
		0xcf00, 
		0xca80, 
		0xd3a0, 
		0xd2a0, 
		0xd220, 
		0xc600, 
		0x02d4, 
		0x4980, 
		0xca00, 
		0x0000, 
		0x4800, 
		0xfd59, 
		0xd1e0, 
		0xcd80, 
		0x5614, 
		0xcdc0, 
		0x26b5, 
		0xbc00, 
		0x7c00, 
		0x551d, 
		0xbc00, 
		0x375b, 
		0x5080, 
		0x7c00, 
		0xd1a0, 
		0xccc0, 
		0x4c00, 
		0xcf00, 
		0xc200, 
		0xcf40, 
		0xedf9, 
		0x50e0, 
		0x52c0, 
		0x4402, 
		0xcd80, 
		0x60b4, 
		0xcdc0, 
		0xd220, 
		0x4980, 
		0x5260, 
		0x3c00, 
		0xf828, 
		0x0000, 
		0xfc00, 
		0x8484, 
		0xfcfa, 
		0x52e0, 
		0x4fc0, 
		0xd0e0, 
		0xbc00, 
		0x83ee, 
		0x5260, 
		0xcf80, 
		0xd1a0, 
		0xfc68, 
		0xc800, 
		0x4c40, 
		0x8000, 
		0xd040, 
		0x4200, 
		0x586c, 
		0xc400, 
		0xcf80, 
		0x7cc8, 
		0xd1a0, 
		0xc400, 
		0xfe22, 
		0xd360, 
		0xc880, 
		0x7fb3, 
		0xcec0, 
		0xd1a0, 
		0xc825, 
		0x7c00, 
		0xcf00, 
		0x29a3, 
		0x52c0, 
		0xc900, 
		0x53a0, 
		0x4400, 
		0x5d43, 
		0xfde7, 
		0x4f80, 
		0xd2c0, 
		0x4200, 
		0xfc00, 
		0xd200, 
		0xc600, 
		0x4cc0, 
		0xd0a0, 
		0x8eb3, 
		0x5060, 
		0xc000, 
		0x0cc8, 
		0x6517, 
		0x55cf, 
		0x5160, 
		0x10de, 
		0x05ca, 
		0x4a80, 
		0xfc00, 
		0xcb00, 
		0xd180, 
		0x4900, 
		0xc880, 
		0xb0f1, 
		0x5280, 
		0x0d57, 
		0x4c80, 
		0x5000, 
		0xc9ed, 
		0xdedf, 
		0xccc0, 
		0x7c00, 
		0xac2f, 
		0xd0c0, 
		0xce00, 
		0x4f40, 
		0xf3e5, 
		0x4b00, 
		0xc980, 
		0x25c3, 
		0x4a00, 
		0x0000, 
		0x7c00, 
		0xd56e, 
		0xd040, 
		0xc400, 
		0x4f40, 
		0xd200, 
		0xf665, 
		0xce40, 
		0x4800, 
		0x4f00, 
		0xf4ff, 
		0xd320, 
		0x53c0, 
		0x93f8, 
		0xd1c0, 
		0x365b, 
		0xd530, 
		0x8000, 
		0x8000, 
		0xc800, 
		0xf6c2, 
		0xee4d, 
		0xd1e0, 
		0x4c80, 
		0xc900, 
		0xd240, 
		0x4c80, 
		0xd0e0, 
		0x5380, 
		0xc700, 
		0x4400, 
		0x50c0, 
		0xd2a0, 
		0xd240, 
		0xc000, 
		0xd1e0, 
		0x4dc0, 
		0x3c00, 
		0xd380, 
		0x4400, 
		0x6170, 
		0x4d40, 
		0x1d25, 
		0x50c0, 
		0x5280, 
		0xcc00, 
		0x4800, 
		0x010f, 
		0xcb80, 
		0xcd40, 
		0x4f00, 
		0x50c0, 
		0xd000, 
		0xcb00, 
		0xcd00, 
		0x4800, 
		0xe4d7, 
		0x51e0, 
		0x6185, 
		0x7d4b, 
		0xcc40, 
		0xfbc6, 
		0xc000, 
		0x92fd, 
		0x52c0, 
		0x4cc0, 
		0xedf5, 
		0x7c00, 
		0xd340, 
		0xcc40, 
		0x5c9f, 
		0x89fa, 
		0x7c00, 
		0xd140, 
		0x4cc0, 
		0xbf70, 
		0x906b, 
		0x5080, 
		0xcb0c, 
		0xc880, 
		0xe728, 
		0x7c00, 
		0x4a00, 
		0xca80, 
		0x0000, 
		0xc400, 
		0x30ae, 
		0x4c43, 
		0xe31b, 
		0x31e3, 
		0x51c0, 
		0xb99f, 
		0x59ce, 
		0xd1e0, 
		0xcb80, 
		0xccc0, 
		0x3bc4, 
		0x4a00, 
		0x5360, 
		0x649b, 
		0xc700, 
		0x6f10, 
		0x51a0, 
		0x5120, 
		0x5200, 
		0xd300, 
		0xab17, 
		0xf32b, 
		0x5080, 
		0xff7a, 
		0x0000, 
		0xfc00, 
		0x4a00, 
		0x51c0, 
		0x5200, 
		0x4b00, 
		0xd360, 
		0x4c80, 
		0x5789, 
		0x8000, 
		0x5080, 
		0xd1a0, 
		0xca00, 
		0xcc00, 
		0x4a00, 
		0x4700, 
		0xc880, 
		0xcd40, 
		0xd040, 
		0x4900, 
		0x62c7, 
		0x8000, 
		0xd3c0, 
		0x4954, 
		0x0000, 
		0x7af5, 
		0xc800, 
		0x8000, 
		0xfc00, 
		0x230f, 
		0x4f80, 
		0x4f40, 
		0x52e0, 
		0xb8d4, 
		0xf5fa, 
		0x0000, 
		0xd220, 
		0x5340, 
		0x4900, 
		0x51a0, 
		0xd020, 
		0xd340, 
		0xcc00, 
		0x49ab, 
		0xd3e0, 
		0xccc0, 
		0x8000, 
		0xc980, 
		0xcf40, 
		0xc600, 
		0x164d, 
		0x5080, 
		0xcb80, 
		0xd380, 
		0x53a0, 
		0xc800, 
		0xef75, 
		0x51c0, 
		0x5da7, 
		0x8000, 
		0x8000, 
		0x7c00, 
		0xd0e0, 
		0x4089, 
		0xe8f2, 
		0x8000, 
		0x4f80, 
		0xd3c0, 
		0x4d80, 
		0xd140, 
		0xe13c, 
		0x4f80, 
		0xce00, 
		0x7c00, 
		0xd020, 
		0xce40, 
		0x4000, 
		0x57e5, 
		0x5300, 
		0xd020, 
		0xf052, 
		0xb73a, 
		0xd1a0, 
		0xcec0, 
		0xce80, 
		0x4c00, 
		0xcdc0, 
		0x70cd, 
		0x8000, 
		0x4200, 
		0xc500, 
		0x51a0, 
		0x995e, 
		0x5240, 
		0x4600, 
		0xcf83, 
		0xc600, 
		0x8000, 
		0x51e0, 
		0xdeee, 
		0x52c0, 
		0x5180, 
		0x8e96, 
		0x5200, 
		0x3fc6, 
		0x5100, 
		0x8000, 
		0x51e0, 
		0xd0c0, 
		0xcb80, 
		0xbc00, 
		0x4fc0, 
		0xc5a5, 
		0x7c00, 
		0x52e0, 
		0xc11b, 
		0x8d7d, 
		0xd380, 
		0x51a0, 
		0x3f4c, 
		0x7053, 
		0x4dc0, 
		0xcf80, 
		0xd060, 
		0xedcf, 
		0xd1e0, 
		0xd180, 
		0xce80, 
		0x4e40, 
		0xd160, 
		0xd3a0, 
		0x4aba, 
		0x0146, 
		0x53a0, 
		0x5360, 
		0x426e, 
		0xe28c, 
		0x9c67, 
		0x5160, 
		0x5113, 
		0xd0c0, 
		0xd2c0, 
		0x7a2f, 
		0x5180, 
		0x11df, 
		0xa920, 
		0xcd00, 
		0xc500, 
		0xcdc0, 
		0x8638, 
		0x4900, 
		0xc9ca, 
		0xd274, 
		0x4a00, 
		0x5be8, 
		0xd340, 
		0x5080, 
		0x5140, 
		0x8000, 
		0x5380, 
		0xd974, 
		0x95a7, 
		0xc12b, 
		0xcd40, 
		0xd1e0, 
		0xca2e, 
		0x4b80, 
		0xe62a, 
		0x256a, 
		0x50a0, 
		0x1a7e, 
		0xfc00, 
		0x46eb, 
		0x3748, 
		0x5080, 
		0xa096, 
		0x53a0, 
		0x5340, 
		0x5320, 
		0x4a00, 
		0xcd00, 
		0x8000, 
		0x49a6, 
		0xc600, 
		0x7c00, 
		0x64e6, 
		0x53c0, 
		0x7de9, 
		0xcd00, 
		0x5380, 
		0x17b5, 
		0x8a04, 
		0xc7cd, 
		0xc500, 
		0x52a0, 
		0xca80, 
		0x8000, 
		0x4c80, 
		0xd260, 
		0x4e40, 
		0xa756, 
		0xd0c0, 
		0xcb80, 
		0xc947, 
		0x4fc0, 
		0x5220, 
		0xca80, 
		0xd140, 
		0xc200, 
		0xce00, 
		0xc700, 
		0x5180, 
		0x4980, 
		0xc400, 
		0x51e0, 
		0x5000, 
		0x7c00, 
		0x2473, 
		0xd320, 
		0x4c00, 
		0xc980, 
		0x50c0, 
		0x8fd2, 
		0x4600, 
		0xd280, 
		0xce00, 
		0xd140, 
		0x5000, 
		0xd0c0, 
		0xd060, 
		0x4f00, 
		0x4d80, 
		0x0000, 
		0x5300, 
		0x4000, 
		0x4cc0, 
		0x8277, 
		0x7632, 
		0xfe1c, 
		0x4f40, 
		0x52c0, 
		0xd3a0, 
		0xc563, 
		0x4d80, 
		0x5eec, 
		0x0613, 
		0xed01, 
		0x4c40, 
		0xcfc0, 
		0x4600, 
		0x3f68, 
		0x4c00, 
		0x7f83, 
		0xd2c0, 
		0x4c80, 
		0x5260, 
		0xf719, 
		0x52a0, 
		0x5cc3, 
		0x4600, 
		0xd000, 
		0x4f00, 
		0x4b80, 
		0xfce8, 
		0xcdc0, 
		0x5340, 
		0x208e, 
		0xbc00, 
		0xcc80, 
		0xd0c0, 
		0xd3e0, 
		0xd300, 
		0xd3a0, 
		0xd120, 
		0x0000, 
		0x4f00, 
		0x82bf, 
		0xc800, 
		0xd3c0, 
		0x3534, 
		0xce80, 
		0x52e0, 
		0x9e3e, 
		0x5240, 
		0x5140, 
		0xfc00, 
		0x5020, 
		0x4edd, 
		0x5160, 
		0x0000, 
		0xa9ae, 
		0x4c00, 
		0xe9b0, 
		0x8000, 
		0x5140, 
		0x0000, 
		0x755f, 
		0x86f9, 
		0x0f74, 
		0x6819, 
		0x5120, 
		0x5080, 
		0xd380, 
		0x5320, 
		0x1f27, 
		0xc980, 
		0xd120, 
		0xfc00, 
		0x3b17, 
		0x5060, 
		0xca00, 
		0xce80, 
		0x8000, 
		0x5220, 
		0x4e80, 
		0x5ec6, 
		0x2169, 
		0x3d0a, 
		0x0433, 
		0xbe7d, 
		0x4c80, 
		0xd000, 
		0xcc40, 
		0x58f4, 
		0xcb00, 
		0x8185, 
		0xd31e, 
		0xd060, 
		0xd380, 
		0xd320, 
		0xcd00, 
		0xe6e5, 
		0x7c00, 
		0xc600, 
		0x4e00, 
		0x5140, 
		0x3e06, 
		0x50a0, 
		0x5320, 
		0xc700, 
		0xcc40, 
		0xcb00, 
		0x5080, 
		0x4fc0, 
		0xfc1e, 
		0x5100, 
		0xd2e0, 
		0xcc40, 
		0xc500, 
		0x5080, 
		0x0000, 
		0xd260, 
		0xdddb, 
		0xce00, 
		0x50a0, 
		0x7e5c, 
		0x19d2, 
		0xcc40, 
		0x4e40, 
		0x5260, 
		0xfc00, 
		0x0000, 
		0xd1e0, 
		0x7c00, 
		0x6ee8, 
		0x75d7, 
		0x4b80, 
		0xd2e0, 
		0x5300, 
		0xc980, 
		0xfc00, 
		0x51a0, 
		0x51e0, 
		0xafc3, 
		0xc980, 
		0x92c6, 
		0x0000, 
		0x59d7, 
		0x4a00, 
		0xf22a, 
		0x4d80, 
		0xd3e0, 
		0x4fc0, 
		0x4dc0, 
		0xd320, 
		0x5140, 
		0xd3c0, 
		0x53e0, 
		0x5120, 
		0xc400, 
		0xbce4, 
		0x5040, 
		0x0000, 
		0x7788, 
		0xca80, 
		0x7c00, 
		0x4dc0, 
		0xd1a0, 
		0xcc28, 
		0x4c80, 
		0x6f36, 
		0xc000, 
		0x1534, 
		0xd180, 
		0x0c29, 
		0x5340, 
		0x5340, 
		0xcb85, 
		0xcd00, 
		0xd87e, 
		0xd1e0, 
		0x4d5b, 
		0x4000, 
		0x4900, 
		0xd200, 
		0x4e40, 
		0xc200, 
		0xedc5, 
		0xd824, 
		0x734b, 
		0x8891, 
		0xfdd8, 
		0xcf40, 
		0x9574, 
		0x0000, 
		0xd080, 
		0x4ec0, 
		0x8029, 
		0xd3c0, 
		0x0bac, 
		0x8000, 
		0xd020, 
		0xfb7b, 
		0xcfc0, 
		0x3c00, 
		0xcfc0, 
		0xcc00, 
		0xf549, 
		0x0000, 
		0x52a0, 
		0x4cc0, 
		0x59cc, 
		0xce80, 
		0xaeae, 
		0xcf80, 
		0x575b, 
		0x0000, 
		0xd2e0, 
		0xfd45, 
		0x4c80, 
		0xcf9a, 
		0xefaf, 
		0x52e0, 
		0xc900, 
		0x5220, 
		0x4b00, 
		0xc700, 
		0x50e0, 
		0xca80, 
		0xd2a0, 
		0xd240, 
		0xfc00, 
		0x593b, 
		0xca00, 
		0x4e40, 
		0x52c0, 
		0x2cec, 
		0x8000, 
		0x28ac, 
		0x7c00, 
		0x5040, 
		0x4bfa, 
		0xfc00, 
		0xd3a0, 
		0x8000, 
		0x5280, 
		0x1947, 
		0xc400, 
		0x5b91, 
		0x00fd, 
		0xc400, 
		0x5220, 
		0xba0d, 
		0x702b, 
		0x3177, 
		0xd020, 
		0x5080, 
		0xd180, 
		0x4b00, 
		0x4900, 
		0x92c9, 
		0xc800, 
		0x8000, 
		0x5340, 
		0x4fc0, 
		0xa363, 
		0xd1c0, 
		0x180b, 
		0x51a0, 
		0xd3a0, 
		0xd180, 
		0x5120, 
		0x4a93, 
		0xcb00, 
		0xa5c4, 
		0x4980, 
		0x7c00, 
		0xd0a0, 
		0xd2c0, 
		0x4000, 
		0xd1c0, 
		0x7c00, 
		0xcf95, 
		0xfc00, 
		0x4cc0, 
		0x4f40, 
		0xd180, 
		0x0000, 
		0xbdf5, 
		0xd2e0, 
		0x37fa, 
		0x54dd, 
		0x4c40, 
		0x5360, 
		0x7c00, 
		0xd3e0, 
		0xa7d8, 
		0x2f0e, 
		0x92d3, 
		0xcf40, 
		0x7c00, 
		0x4dc0, 
		0xd2c0, 
		0x874e, 
		0xd140, 
		0x5340, 
		0x2ecd, 
		0xa785, 
		0x5bbb, 
		0x4200, 
		0xca80, 
		0xaa57, 
		0x4a90, 
		0x4980, 
		0xd060, 
		0xcd00, 
		0xc092, 
		0x4200, 
		0x0ae4, 
		0x4b00, 
		0x6075, 
		0x5320, 
		0xab22, 
		0xd180, 
		0x1832, 
		0xd0e0, 
		0x1355, 
		0xfc00, 
		0x5340, 
		0xd120, 
		0xcb80, 
		0x2d22, 
		0xb60a, 
		0xd120, 
		0xd2a0, 
		0xcd40, 
		0x4500, 
		0x9fa6, 
		0x5380, 
		0xd2c0, 
		0x4000, 
		0x84be, 
		0x1e69, 
		0xd2e0, 
		0x5360, 
		0x4e00, 
		0x4500, 
		0xc500, 
		0x4d40, 
		0x4500, 
		0x4d00, 
		0x39b6, 
		0x52c0, 
		0x4a00, 
		0x50a0, 
		0x5658, 
		0xd100, 
		0x4fc0, 
		0x66d9, 
		0xd080, 
		0x3c00, 
		0xd180, 
		0x0be2, 
		0xd340, 
		0xbc00, 
		0x19f2, 
		0xd040, 
		0x1279, 
		0xd3e0, 
		0x53c0, 
		0x4200, 
		0xc880, 
		0x50e0, 
		0xc880, 
		0x4d92, 
		0xd3c0, 
		0x50a0, 
		0x39a5, 
		0x5260, 
		0x5340, 
		0x4f00, 
		0xd060, 
		0xcedb, 
		0x0474, 
		0x88f2, 
		0xd1a0, 
		0xd1c0, 
		0x4887, 
		0xcb80, 
		0xfda5, 
		0x4c00, 
		0x145f, 
		0x7c00, 
		0xd3e0, 
		0x9abd, 
		0xcb00, 
		0x0696, 
		0x4a00, 
		0xd200, 
		0xcf80, 
		0x53e0, 
		0xd200, 
		0x8000, 
		0xd020, 
		0x2f59, 
		0x0000, 
		0x393d, 
		0x5300, 
		0x50c0, 
		0x50c0, 
		0x4a80, 
		0x0000, 
		0x78f5, 
		0x6d7a, 
		0x7c00, 
		0xce80, 
		0xa39f, 
		0x51a0, 
		0x6c59, 
		0x80be, 
		0x5080, 
		0x0000, 
		0xce00, 
		0x8000, 
		0x1cb7, 
		0xd3c0, 
		0x5360, 
		0x52c0, 
		0x50e0, 
		0xd280, 
		0xaa1d, 
		0xd1e0, 
		0xd84f, 
		0x52c0, 
		0xc200, 
		0xce80, 
		0x03b9, 
		0xcc40, 
		0x2e29, 
		0xce40, 
		0x2535, 
		0xd160, 
		0x8000, 
		0x5360, 
		0xc200, 
		0xf8b6, 
		0x7c00, 
		0xca80, 
		0x5380, 
		0xd120, 
		0xd3a0, 
		0xc600, 
		0xbc00, 
		0x5240, 
		0x7c00, 
		0x4ec0, 
		0x0000, 
		0x625e, 
		0x7c00, 
		0x0000, 
		0xd260, 
		0x4980, 
		0x8643, 
		0xd1a0, 
		0x357b, 
		0x0b13, 
		0xd187, 
		0xaf41, 
		0xd1c0, 
		0xd00c, 
		0xcc00, 
		0xd340, 
		0xcf80, 
		0x7bf0, 
		0x6369, 
		0xc700, 
		0x4200, 
		0x0bf2, 
		0xfa6d, 
		0x53a0, 
		0x5140, 
		0x2721, 
		0x4f00, 
		0x4200, 
		0x53b3, 
		0xf972, 
		0xc980, 
		0xd340, 
		0x4d40, 
		0x53c0, 
		0x5a7f, 
		0x5340, 
		0x8079, 
		0x4ec0, 
		0x0000, 
		0xbac4, 
		0x4d40, 
		0x4a00, 
		0xcf40, 
		0x4200, 
		0xd020, 
		0xfd56, 
		0x5000, 
		0x4e00, 
		0x4e80, 
		0x649f, 
		0x5060, 
		0x252a, 
		0x51e0, 
		0xc880, 
		0x50e0, 
		0x4700, 
		0xcdc0, 
		0x4ec0, 
		0xd060, 
		0x4400, 
		0xc880, 
		0x50c0, 
		0xf5c7, 
		0xbc00, 
		0xd3e0, 
		0x5160, 
		0xfc00, 
		0x52c0, 
		0xc500, 
		0x53a0, 
		0xcb00, 
		0x941b, 
		0x50e0, 
		0xd120, 
		0xcd00, 
		0x5d6d, 
		0x4700, 
		0xd1e0, 
		0xce80, 
		0x5000, 
		0xce00, 
		0x51e0, 
		0x4700, 
		0xcd80, 
		0xb80c, 
		0x50e0, 
		0xc980, 
		0x4ec0, 
		0xfc00, 
		0xbaba, 
		0x960a, 
		0x4fc0, 
		0x0262, 
		0x4c00, 
		0x5000, 
		0x8000, 
		0x5020, 
		0x7c00, 
		0xd220, 
		0x5b83, 
		0xe766, 
		0x720a, 
		0x0000, 
		0x065c, 
		0xca00, 
		0x3c00, 
		0xcb80, 
		0x9c16, 
		0x4f38, 
		0xd380, 
		0x4f00, 
		0x5240, 
		0xd360, 
		0xca00, 
		0xe5f9, 
		0xfc00, 
		0x4cc0, 
		0xb0ff, 
		0xecd0, 
		0xed0f, 
		0xce40, 
		0x8000, 
		0x5280, 
		0xfc00, 
		0x5120, 
		0x4c00, 
		0xb552, 
		0xcc00, 
		0x52e0, 
		0x2ced, 
		0x4dd8, 
		0xc000, 
		0xa2f0, 
		0x9489, 
		0x4b80, 
		0x52c0, 
		0xd340, 
		0xcc40, 
		0x53e0, 
		0xd3e0, 
		0xfc00, 
		0x5380, 
		0x5340, 
		0xfc00, 
		0xd240, 
		0x50c0, 
		0xd220, 
		0x51a0, 
		0x4a00, 
		0x5020, 
		0x52e0, 
		0xd0a0, 
		0x92b2, 
		0x0261, 
		0xcbbb, 
		0xcb80, 
		0xc14f, 
		0x4700, 
		0xd80d, 
		0xcc40, 
		0x5300, 
		0xe22b, 
		0x4000, 
		0x4800, 
		0xd380, 
		0xf79b, 
		0xba94, 
		0x4d00, 
		0xca00, 
		0xcf9d, 
		0xfc1e, 
		0xd0e0, 
		0x8c4f, 
		0x8000, 
		0x5f6a, 
		0xc980, 
		0xccc0, 
		0x0256, 
		0x69ba, 
		0x920a, 
		0x4e33, 
		0xd2c0, 
		0x71a2, 
		0x5280, 
		0xc500, 
		0x19f9, 
		0xe5d2, 
		0xc600, 
		0x4600, 
		0xdfa1, 
		0xfc59, 
		0x4e40, 
		0xd0e0, 
		0x5320, 
		0x52a0, 
		0x4500, 
		0xd1e0, 
		0x5020, 
		0x02fd, 
		0x8ab1, 
		0x0000, 
		0x5320, 
		0xc04f, 
		0xcfc0, 
		0xcf00, 
		0x4500, 
		0xc880, 
		0xce00, 
		0x78ad, 
		0xd220, 
		0xc980, 
		0xcb00, 
		0x51a0, 
		0xc2b2, 
		0x4d00, 
		0x0000, 
		0xcd40, 
		0xd160, 
		0xfe26, 
		0x8000, 
		0x52e0, 
		0x5280, 
		0x5100, 
		0x760d, 
		0x8000, 
		0xd3a0, 
		0x33ef, 
		0xd080, 
		0x0568, 
		0x4cc0, 
		0xcc00, 
		0x5040, 
		0xc093, 
		0x51a0, 
		0xca80, 
		0x0246, 
		0xcf40, 
		0x5280, 
		0xd268, 
		0x7c00, 
		0xce80, 
		0x5425, 
		0xd2e0, 
		0xce40, 
		0x4600, 
		0x480b, 
		0xef09, 
		0xcec0, 
		0xd300, 
		0x4ec0, 
		0x6804, 
		0x5380, 
		0x3c00, 
		0x00cd, 
		0x53e0, 
		0x5060, 
		0xcf40, 
		0xd1e0, 
		0xfd73, 
		0x5260, 
		0x4880, 
		0xd320, 
		0xc400, 
		0xcf40, 
		0x9420, 
		0xd360, 
		0xc1de, 
		0x53c0, 
		0x3e9e, 
		0xd0c0, 
		0xcc00, 
		0x5320, 
		0xc700, 
		0x5301, 
		0x204b, 
		0x601c, 
		0x5320, 
		0x57a9, 
		0x53e0, 
		0xcae9, 
		0x5240, 
		0xd260, 
		0x0cb5, 
		0x5020, 
		0xc980, 
		0xd060, 
		0x4cc0, 
		0x4b80, 
		0xd0e0, 
		0xf11e, 
		0x1ce6, 
		0xfc71, 
		0xfa0e, 
		0x9702, 
		0x5140, 
		0x4c00, 
		0xd0c0, 
		0x52c0  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00000000, 
		0x80800001, 
		0x7f800000, 
		0x42440000, 
		0x0e000003, 
		0x41100000, 
		0x7f800000, 
		0xcb8c4b40, 
		0x80000000, 
		0xc1c80000, 
		0x40a00000, 
		0x6b741570, 
		0x422c0000, 
		0xcdec2f47, 
		0x73771f44, 
		0xf560c133, 
		0x3f028f5c, 
		0x056ba17a, 
		0x3d85b3b3, 
		0x80800001, 
		0xc1a80000, 
		0x15085c1f, 
		0xff7fffff, 
		0x80800003, 
		0xc1900000, 
		0x00000000, 
		0x41f80000, 
		0x00010000, 
		0xc1600000, 
		0x0f7ffffd, 
		0x67233a7d, 
		0xc2200000, 
		0xf9d0f6ee, 
		0x423c0000, 
		0x80000000, 
		0xc1100000, 
		0x0c600000, 
		0xc355bdc4, 
		0x42340000, 
		0x7f800000, 
		0x41d80000, 
		0x80000000, 
		0x1f8696c5, 
		0x4b8c4b40, 
		0x8f7ffffc, 
		0xc469274b, 
		0x00000001, 
		0x97b2ed86, 
		0xc20c0000, 
		0xe8d5c853, 
		0xc0e00000, 
		0x0c7fffc0, 
		0x484333f0, 
		0xc2500000, 
		0x0fd0f2b0, 
		0x00200000, 
		0x41600000, 
		0x9de0062e, 
		0x72ce7298, 
		0x80000000, 
		0x43fc522d, 
		0xb84b3f27, 
		0x41b80000, 
		0xc1600000, 
		0x7faccde5, 
		0x80000008, 
		0x0f7ffffe, 
		0xc2540000, 
		0x41d80000, 
		0xc1700000, 
		0xc2540000, 
		0x41d00000, 
		0x0c7fffc0, 
		0x42340000, 
		0x80000000, 
		0x7f7ffffe, 
		0x814834c5, 
		0x0c7fe000, 
		0x206c53dd, 
		0x42400000, 
		0x4cd6ac5c, 
		0x00000000, 
		0x7f800000, 
		0x0c7ffffc, 
		0x5bb9bc19, 
		0xff000000, 
		0xe031919a, 
		0x8ea60c3d, 
		0x00800000, 
		0x426c0000, 
		0x41e80000, 
		0x8b14863d, 
		0xcb8c4b40, 
		0xc27c0000, 
		0xff800000, 
		0x96321309, 
		0x80001000, 
		0x00000008, 
		0x41700000, 
		0x80000000, 
		0x80011111, 
		0xc27c0000, 
		0xf474515a, 
		0xc1400000, 
		0x00040000, 
		0xff800000, 
		0x80000002, 
		0x42140000, 
		0x9d11bc71, 
		0xdae31568, 
		0xc0400000, 
		0x0c600000, 
		0x42540000, 
		0x2dad05ac, 
		0x7f7ffffe, 
		0xfde4368c, 
		0x00002000, 
		0x80000010, 
		0xc0e00000, 
		0xff800000, 
		0x0e0001ff, 
		0xc2040000, 
		0xff7ffffe, 
		0xc1c80000, 
		0xffffffff, 
		0xff800000, 
		0x7f800000, 
		0x37b3d5f1, 
		0xff800000, 
		0xffffffff, 
		0x1a190ec4, 
		0x68dc45b5, 
		0x00000000, 
		0xc2340000, 
		0x0e0000ff, 
		0xc1a80000, 
		0x92bcb23e, 
		0x42000000, 
		0x647609b4, 
		0x10804aff, 
		0x00000000, 
		0xffc00000, 
		0x8f7ffffd, 
		0xc2580000, 
		0x04c26dba, 
		0x0e001fff, 
		0x41200000, 
		0x26fcaac7, 
		0xff7ffffe, 
		0xcb000000, 
		0xff800000, 
		0x00800001, 
		0xb4e36699, 
		0xc0800000, 
		0x98c8c0d1, 
		0xff8e1906, 
		0xc0a00000, 
		0x35cd87f6, 
		0x00000002, 
		0x0c7f0000, 
		0x80000800, 
		0x42726509, 
		0xa2b88c12, 
		0xffc00000, 
		0xff800001, 
		0x80800000, 
		0x80615661, 
		0xcf5467f4, 
		0xc2080000, 
		0x7f800001, 
		0xade422ac, 
		0x7f7ffffe, 
		0xc9fbd155, 
		0x80000000, 
		0x00800001, 
		0x80800001, 
		0x7fc00000, 
		0x0e00007f, 
		0xfb3fb488, 
		0x80800000, 
		0xffc00001, 
		0x9ace0ed3, 
		0x9af3e290, 
		0x40800000, 
		0x11e7e989, 
		0x0c7e0000, 
		0xef79a2cf, 
		0x427c0000, 
		0x00000010, 
		0xc2140000, 
		0x41200000, 
		0x80000000, 
		0x80000000, 
		0x40e00000, 
		0x80000000, 
		0xc2400000, 
		0x0c7f8000, 
		0x42480000, 
		0x80008000, 
		0xc1980000, 
		0xf718d941, 
		0x42200000, 
		0x00001000, 
		0x8f7ffffd, 
		0xfd3e6cab, 
		0x42740000, 
		0x00000000, 
		0xc433c1d5, 
		0xc0400000, 
		0x0e003fff, 
		0x00000000, 
		0x7f800000, 
		0x7fffffff, 
		0xc1a00000, 
		0x20bcab58, 
		0x504ae505, 
		0xc1600000, 
		0xb7bc5eab, 
		0x30fd9a68, 
		0x98e402f8, 
		0x00000200, 
		0x00000000, 
		0xc2500000, 
		0x0c700000, 
		0x8f7ffffc, 
		0x80800002, 
		0xffb63623, 
		0x41980000, 
		0x00800001, 
		0xffffffff, 
		0xff800000, 
		0x0af93a7e, 
		0x42680000, 
		0x9aff0e63, 
		0x80000001, 
		0xadbddf99, 
		0x08204351, 
		0x6e6313a3, 
		0x41c00000, 
		0x00040000, 
		0xcd198341, 
		0x80004000, 
		0x8f7fffff, 
		0x005e5951, 
		0x0f7fffff, 
		0x78a29f19, 
		0xc2540000, 
		0x41f80000, 
		0x80800000, 
		0x41300000, 
		0x63069da2, 
		0x00dbf0cb, 
		0x42780000, 
		0x0e001fff, 
		0x7f800000, 
		0x0c7e0000, 
		0x068697b1, 
		0xc26c0000, 
		0x42640000, 
		0x7f93a826, 
		0x7fbfffff, 
		0xc27c0000, 
		0x5ab2f321, 
		0x80000000, 
		0x80000001, 
		0x44d5465e, 
		0x9a473201, 
		0xc2040000, 
		0x00000004, 
		0xd4797ae9, 
		0xc1700000, 
		0x3f800000, 
		0x00000000, 
		0x40e00000, 
		0x76a77ef9, 
		0x00000000, 
		0xc1880000, 
		0x32fa3bbf, 
		0xc2580000, 
		0x5fddf6e4, 
		0x00000000, 
		0xc2400000, 
		0xc2140000, 
		0xff7fffff, 
		0x00000002, 
		0x40400000, 
		0x00008000, 
		0x80000000, 
		0x7fffffff, 
		0x40c00000, 
		0xc0e00000, 
		0x0e000fff, 
		0xbb5ca540, 
		0x0e01ffff, 
		0xf3dcf46f, 
		0x7fe2d8d5, 
		0x89abc7ee, 
		0x0e000fff, 
		0x42240000, 
		0x00800002, 
		0xbf26f6b3, 
		0x42700000, 
		0x80000800, 
		0x1f1a949e, 
		0x41980000, 
		0x00000000, 
		0x7fc00000, 
		0x0e01ffff, 
		0x78113472, 
		0x80000040, 
		0x7f800000, 
		0x80800001, 
		0x80000200, 
		0x0c780000, 
		0x73e42a97, 
		0xb0f136b3, 
		0xe0177c37, 
		0xad94924c, 
		0xdbaf4a89, 
		0x789a56e4, 
		0x007fffff, 
		0x00000000, 
		0x80000000, 
		0x40800000, 
		0x3f800000, 
		0xc0000000, 
		0x41300000, 
		0x40000000, 
		0x7f8dc0f7, 
		0xcde48df7, 
		0x0e000fff, 
		0x00800002, 
		0x41500000, 
		0x00000010, 
		0x421c0000, 
		0xb34888d7, 
		0xc1300000, 
		0x41a00000, 
		0x80008000, 
		0xffbfffff, 
		0x007ffffe, 
		0x40a00000, 
		0x2248d053, 
		0x80000010, 
		0x599eb24f, 
		0x4b8c4b40, 
		0x41900000, 
		0x00000000, 
		0x007ffffe, 
		0x00000000, 
		0x0f7ffffc, 
		0xf23529f5, 
		0x7f7fffff, 
		0x425c0000, 
		0x00800002, 
		0x5ed7432c, 
		0x00000000, 
		0x42740000, 
		0x0f7ffffc, 
		0x8f7ffffc, 
		0xffa0612b, 
		0x807fffff, 
		0x42680000, 
		0xffffffff, 
		0x0e0003ff, 
		0x7f800000, 
		0x42500000, 
		0x423c0000, 
		0xd9f84c13, 
		0xffa6ac2c, 
		0x0c2f5cde, 
		0xd07c21db, 
		0x5f730713, 
		0x377ba0e4, 
		0xc2540000, 
		0x0c7fff80, 
		0xff800001, 
		0xe7a102f3, 
		0x00800000, 
		0x80ffffff, 
		0x316787d0, 
		0x40400000, 
		0x0e07ffff, 
		0x00000000, 
		0x8f7fffff, 
		0x80001000, 
		0x42380000, 
		0x00000008, 
		0x00000000, 
		0x9072bdeb, 
		0xd42cb6e7, 
		0xc0400000, 
		0xec60fe9c, 
		0x4b000000, 
		0x3a80c63c, 
		0x80000010, 
		0x0d00fff0, 
		0x00000800, 
		0x555da5a2, 
		0x003b5e86, 
		0xc1d00000, 
		0xc2140000, 
		0x80800001, 
		0x0c7fff00, 
		0x80000080, 
		0xc1b00000, 
		0x79648a25, 
		0x0c7f0000, 
		0xff800000, 
		0x0c7ffff8, 
		0x00800001, 
		0x42380000, 
		0x41600000, 
		0x426c0000, 
		0x00800000, 
		0x7015c3d7, 
		0xc2400000, 
		0x40000000, 
		0xc2600000, 
		0xff800000, 
		0xcc4397bc, 
		0x0c780000, 
		0x42640000, 
		0x7f800000, 
		0x00800000, 
		0xc0e00000, 
		0x41900000, 
		0xff800000, 
		0x6895280c, 
		0xc2000000, 
		0xffc00001, 
		0x80000001, 
		0x725f00e2, 
		0x8eed590a, 
		0x0c7ffe00, 
		0xff800000, 
		0x006f199f, 
		0x0e7fffff, 
		0x6a70b5fa, 
		0xda5a5cc7, 
		0x0e0003ff, 
		0x34f35eb0, 
		0xc91932b4, 
		0xffa29090, 
		0xc2580000, 
		0x0c7fc000, 
		0xff7fffff, 
		0x2f6ac84b, 
		0x80000080, 
		0x521e27c5, 
		0x80000000, 
		0xc2180000, 
		0x80000000, 
		0xc1c00000, 
		0x0c700000, 
		0x42740000, 
		0x7fc00000, 
		0x80004000, 
		0xfa7a6281, 
		0x0c7fff00, 
		0xff800000, 
		0x732d1a39, 
		0xd0da7095, 
		0xde0e6eee, 
		0x491f9dcf, 
		0x7fbfffff, 
		0x41600000, 
		0x0c7fffc0, 
		0x0f7ffffc, 
		0xc86bae56, 
		0x33333333, 
		0x807fffff, 
		0x7f800000, 
		0x4b000000, 
		0x41b80000, 
		0x459ff9c9, 
		0x0f7fffff, 
		0x80000100, 
		0xff800000, 
		0xffc00000, 
		0x40800000, 
		0x658c22ae, 
		0x00080000, 
		0x80200000, 
		0x80800002, 
		0x80000100, 
		0xc23c0000, 
		0xffae0a4c, 
		0xbe2b6dbc, 
		0xa9a6a2fc, 
		0x80000000, 
		0x0e03ffff, 
		0x7f7ffffe, 
		0x00000000, 
		0xc1d80000, 
		0x1f218762, 
		0xff800000, 
		0x007fffff, 
		0xc1f80000, 
		0x5a71b4b9, 
		0x0c7ffffc, 
		0xbc8615b3, 
		0xc9a1b420, 
		0x80800001, 
		0x7f800000, 
		0x284df9db, 
		0x00001000, 
		0xc2700000, 
		0xc2700000, 
		0x00000010, 
		0x0c7ffff0, 
		0x0c600000, 
		0xff7fffff, 
		0xe08bdeed, 
		0x80800003, 
		0x00800001, 
		0xc1980000, 
		0xc0c00000, 
		0x42040000, 
		0x0c7ffc00, 
		0x2b8b6d84, 
		0xc20c0000, 
		0x00800002, 
		0x42240000, 
		0x21fbee0e, 
		0x0f7fffff, 
		0x1f8dac8c, 
		0x421c0000, 
		0x42100000, 
		0x42680000, 
		0x7fc00000, 
		0x7f800000, 
		0x42100000, 
		0xc0400000, 
		0x0c7ffff8, 
		0x427c0000, 
		0xc0e00000, 
		0x00000000, 
		0x3c0102a3, 
		0xffa3aada, 
		0x1724be26, 
		0x00100000, 
		0xc0400000, 
		0x80000001, 
		0x80001000, 
		0xcccccccc, 
		0x00400000, 
		0xff9a9d64, 
		0xabc73e27, 
		0x92cae8cf, 
		0x42400000, 
		0x0e0003ff, 
		0xbf800001, 
		0x7f000000, 
		0x59f6f53c, 
		0x749ec02b, 
		0xf5084e5c, 
		0x80000000, 
		0x42400000, 
		0xc1a00000, 
		0xe3e59a06, 
		0x0f7fffff, 
		0x80008000, 
		0x425c0000, 
		0x98eb1ff1, 
		0xa9aef7f6, 
		0xda7d6fd4, 
		0x244b93b2, 
		0x454bbb8f, 
		0x0e000001, 
		0x42a4580a, 
		0x206c3a77, 
		0xc21c0000, 
		0xc1d00000, 
		0xef2ec6e4, 
		0x0c7ffffc, 
		0x9318fe30, 
		0x0e01ffff, 
		0x00000000, 
		0xffbfcc2a, 
		0x42740000, 
		0x40a00000, 
		0xc1980000, 
		0x42700000, 
		0x41667d1e, 
		0x00000001, 
		0x3bd55a3a, 
		0xff800000, 
		0x313375a0, 
		0xff7ffffe, 
		0x474b486f, 
		0x423c0000, 
		0x41e80000, 
		0xe22d8d2c, 
		0x422c0000, 
		0x05752a97, 
		0xf079bad8, 
		0xffc00000, 
		0xc2440000, 
		0x424c0000, 
		0x0c7f0000, 
		0x80000000, 
		0x0c600000, 
		0xf3e89109, 
		0xc1000000, 
		0x80040000, 
		0x0e000003, 
		0x42500000, 
		0xb80cdf79, 
		0xffc00000, 
		0x2d8bdb7b, 
		0xff000000, 
		0xff7fffff, 
		0x0e0003ff, 
		0x0e00003f, 
		0x00020000, 
		0x60d94f2f, 
		0x41e00000, 
		0x805504e8, 
		0x5d99d2c2, 
		0xffb61c76, 
		0x7f800000, 
		0x00800000, 
		0xff800000, 
		0x479911a0, 
		0x04d92be4, 
		0xa1467fef, 
		0x14b2a3e4, 
		0x0c7f0000, 
		0xfa3785ab, 
		0x98ce9c59, 
		0xc2700000, 
		0x00800000, 
		0x80010000, 
		0x8f7fffff, 
		0x80000000, 
		0x7fb0b1aa, 
		0xcc658d12, 
		0x20b11313, 
		0x80000000, 
		0x0c7fffc0, 
		0x8a5270a7, 
		0x80800003, 
		0x7f800001, 
		0xc0800000, 
		0x00800000, 
		0x0c7e0000, 
		0xffb88f95, 
		0x80800000, 
		0x3e32d4c7, 
		0x42080000, 
		0xc1400000, 
		0xc2780000, 
		0x0e000001, 
		0x0f7ffffd, 
		0xc2240000, 
		0xff800000, 
		0xc40719b0, 
		0x00000200, 
		0xc0000000, 
		0x41d00000, 
		0x7f803980, 
		0x00800001, 
		0x66fb4d65, 
		0x00002000, 
		0xc1a00000, 
		0x0c7ff800, 
		0x80000004, 
		0x00000002, 
		0x80000400, 
		0x813fc037, 
		0xc20c0000, 
		0x00000002, 
		0xc0000000, 
		0xffa2438f, 
		0x00000020, 
		0x00800003, 
		0xab134e8c, 
		0x41c00000, 
		0xefc536da, 
		0x247b5e2d, 
		0xcb000000, 
		0x7f7fffff, 
		0xc26c0000, 
		0x00000010, 
		0xef68cc84, 
		0x00800002, 
		0x0e07ffff, 
		0x80620f75, 
		0x0e0000ff, 
		0x41d80000, 
		0xc1c00000, 
		0x8f7ffffd, 
		0x00517aff, 
		0x41300000, 
		0x00000020, 
		0x42500000, 
		0x80000000, 
		0x80000002, 
		0xc1c00000, 
		0x00000000, 
		0xc1d00000, 
		0xc24c0000, 
		0x855e5ee0, 
		0x0f7fffff, 
		0xff7ffffe, 
		0x80100000, 
		0xc2740000, 
		0xcccccccc, 
		0x00800000, 
		0x41880000, 
		0x80000000, 
		0x80000001, 
		0x8005f955, 
		0x7d80561e, 
		0x7378b3b8, 
		0x8062a82f, 
		0x0e000007, 
		0x7891649c, 
		0x67a6f130, 
		0x41300000, 
		0xcfcef509, 
		0x41a80000, 
		0x5daaf0b3, 
		0x42100000, 
		0x41b80000, 
		0x00000000, 
		0x7f000000, 
		0x0a081dca, 
		0xcdb2348a, 
		0x8b4714ca, 
		0xff000000, 
		0x41880000, 
		0xc2440000, 
		0xd57dd35f, 
		0x00000000, 
		0x80008000, 
		0x41600000, 
		0x00800001, 
		0x00000000, 
		0xff7fffff, 
		0xba5ceb0b, 
		0x80000100, 
		0xcf9856b4, 
		0x00000800, 
		0x0c7ffffc, 
		0x10c7e0a3, 
		0x0e7fffff, 
		0x80000100, 
		0x00000000, 
		0x35929ef9, 
		0xff800001, 
		0x80080000, 
		0x00000000, 
		0xe46f038a, 
		0x41a80000, 
		0x0c7ff000, 
		0xe9793dbc, 
		0x80000000, 
		0x4b000000, 
		0xe939b417, 
		0x0c400000, 
		0xdfec2949, 
		0x0c400000, 
		0x80800003, 
		0x42440000, 
		0xc1000000, 
		0xff7ffffe, 
		0x33333333, 
		0xffa292d2, 
		0x95a8f637, 
		0xc2040000, 
		0x41500000, 
		0x79a25616, 
		0xc9014d21, 
		0x42640000, 
		0x42380000, 
		0x623e6c8a, 
		0x0e003fff, 
		0x00000200, 
		0x1616981e, 
		0x0c7fc000, 
		0x00000008, 
		0x0c7fffe0, 
		0x00000000, 
		0xff800000, 
		0xc25c0000, 
		0xc2480000, 
		0x2d062eab, 
		0x58336550, 
		0x0c700000, 
		0x00000000, 
		0x0e000fff, 
		0x007ffffe, 
		0x0c7f8000, 
		0x00000080, 
		0x80080000, 
		0x80001000, 
		0x426c0000, 
		0x3f028f5c, 
		0x422c0000, 
		0xb9a52959, 
		0xc20c0000, 
		0xc2680000, 
		0x2b4773bc, 
		0x422c0000, 
		0x7f546cec, 
		0x0e0001ff, 
		0x0e0000ff, 
		0x0c7fffff, 
		0x00000000, 
		0x0e003fff, 
		0x00000000, 
		0xb9151918, 
		0x75e81914, 
		0x3ed9bea3, 
		0x80800000, 
		0x0e007fff, 
		0x00000400, 
		0x0c7e0000, 
		0x0e0001ff, 
		0x80000000, 
		0x00001000, 
		0x0c400000, 
		0x80000400, 
		0xd1f1593d, 
		0x20f604c2, 
		0x80800001, 
		0xa6c9bbbc, 
		0x00200000, 
		0x41200000, 
		0x7f8d2c89, 
		0x00000040, 
		0xff800001, 
		0x1d2445d6, 
		0x00000000, 
		0x0e000007, 
		0x4dc863f2, 
		0x00400000, 
		0xf863e596, 
		0x7f800000, 
		0xc2300000, 
		0x8e3d8452, 
		0x80000002, 
		0x00000000, 
		0xc1500000, 
		0x4369cb82, 
		0xbbdd97ce, 
		0x41d80000, 
		0x80000100, 
		0x2642b0a7, 
		0x41f00000, 
		0xc2200000, 
		0xa3660f8d, 
		0x00000000, 
		0x0e0fffff, 
		0xc2640000, 
		0xa6f8e341, 
		0x8f7fffff, 
		0x00020000, 
		0xf4891143, 
		0x0e00007f, 
		0x80000001, 
		0x42140000, 
		0x72acdc08, 
		0x0f7ffffc, 
		0x00010000, 
		0x1e2a1710, 
		0x7f800000, 
		0x41c00000, 
		0x00000000, 
		0xbd5f7308, 
		0x7f800000, 
		0x80000000, 
		0x3323b8fc, 
		0x1dd04a94, 
		0xff800000, 
		0x80000000, 
		0x80000000, 
		0x0e0003ff, 
		0xc2380000, 
		0x55acfd3f, 
		0x80000100, 
		0xff800000, 
		0x41f00000, 
		0xff95137c, 
		0x6fb1a304, 
		0x0e00003f, 
		0x41e00000, 
		0x4b8c4b40, 
		0xc1880000, 
		0x41900000, 
		0xc1c80000, 
		0xffc00000, 
		0x41b80000, 
		0x41900000, 
		0x00000800, 
		0xac4d53e1, 
		0x41d80000, 
		0x0c7e0000, 
		0x80000000, 
		0x0f7ffffd, 
		0xc1900000, 
		0xff800000, 
		0x80000002, 
		0x0f7ffffd, 
		0xc1b00000, 
		0x00800003, 
		0x00416134, 
		0x53eb42f3, 
		0xc25c0000, 
		0x42680000, 
		0xc1700000, 
		0xc0e00000, 
		0x83b9a413, 
		0x628e97c7, 
		0xffc00000, 
		0x41f00000, 
		0x00000040, 
		0x0c780000, 
		0x80000000, 
		0xff800000, 
		0xc2400000, 
		0xc2300000, 
		0xff800000, 
		0x41800000, 
		0x74871e4e, 
		0xc1a80000, 
		0x00000000, 
		0xc1000000, 
		0x41d80000, 
		0xc1880000, 
		0xce4c044f, 
		0x42280000, 
		0x80000000, 
		0x5dbd9f5c, 
		0xa6820a33, 
		0x00100000, 
		0x285d2aec, 
		0x0c7ffe00, 
		0x0e007fff, 
		0xff800000, 
		0x80002000, 
		0x80010000, 
		0x6d8ad953, 
		0x00400000, 
		0x00000100, 
		0x169a5e7a, 
		0x8021afba, 
		0x42640000, 
		0x80001000, 
		0xc27c0000, 
		0xffc01391, 
		0x80000020, 
		0x8ccf559d, 
		0x7f800001, 
		0x41200000, 
		0x0e07ffff, 
		0xc1880000, 
		0xe56b9b8d, 
		0x0c7ffffe, 
		0x80000000, 
		0x80004000, 
		0xff70d16e, 
		0x00340035, 
		0x71f761f1, 
		0x0c7ffff0, 
		0x1a0685a7, 
		0xc2400000, 
		0x47fb0a2f, 
		0x933d0d5c, 
		0x42000000, 
		0x4a903405, 
		0x007fffff, 
		0x80000000, 
		0x80000000, 
		0x80800001, 
		0xc2000000, 
		0xc25c0000, 
		0x00000000, 
		0x00000800, 
		0xc1800000, 
		0x0e01ffff, 
		0xff800000, 
		0x5f9ae031, 
		0x00000000, 
		0x7f800000, 
		0x0c7fc000, 
		0x7f7ffffe, 
		0xc27c0000, 
		0x41700000, 
		0x00000000, 
		0xff7fffff, 
		0x80100000, 
		0x0548e655, 
		0x7f7fffff, 
		0xc837eafc, 
		0x382d991a, 
		0x00008000, 
		0x46d1a065, 
		0x663ca36a, 
		0x424c0000, 
		0x00000000, 
		0x420c0000, 
		0x7fc00000, 
		0x0e789a6f, 
		0x42540000, 
		0x41d00000, 
		0x0c7ffc00, 
		0x806bf0ca, 
		0x80153b82, 
		0xd3e5c219, 
		0xffa3e115, 
		0xc1a00000, 
		0x40e00000, 
		0x7f7fffff, 
		0xc1f00000, 
		0xc1c80000, 
		0x3ebd63c3, 
		0x80000000, 
		0x41000000, 
		0x40000000, 
		0x41d00000, 
		0x420c0000, 
		0x203a9884, 
		0x3340d59b, 
		0x4d0aac91, 
		0xff7ffffe, 
		0xc2340000, 
		0x06d43858, 
		0x0e00ffff, 
		0xc0e00000, 
		0xff7ffffe, 
		0xa325539e, 
		0xc2040000, 
		0xc1200000, 
		0x990ffd16, 
		0x5878ffa8, 
		0x00800003, 
		0x5d402b6f, 
		0x00000000, 
		0x0c400000, 
		0xc20c0000, 
		0x80000000, 
		0x3f028f5c, 
		0xff800000, 
		0xf9b39121, 
		0x33333333, 
		0x80800002, 
		0x42080000, 
		0x42040000, 
		0x80000010, 
		0xd957b65b, 
		0x8f7ffffc, 
		0xc1500000, 
		0xc2440000, 
		0xc2040000, 
		0x8035ae49, 
		0x6edb8720, 
		0x4b000000, 
		0x00002000, 
		0xc2480000, 
		0x42680000, 
		0x0f7ffffe, 
		0x42000000, 
		0x42140000, 
		0x0c7fffff, 
		0x42680000, 
		0x0e07ffff, 
		0xc2280000, 
		0x00100000, 
		0x00040000, 
		0xc22c149a, 
		0x0f7ffffd, 
		0xc2040000, 
		0xc23c0000, 
		0xff800000, 
		0x2dd3d7e5, 
		0x0e1fffff, 
		0xff9227b5, 
		0xc2400000, 
		0xc1b00000, 
		0x8fe5db3d, 
		0x6a3f6a88, 
		0x7f800000, 
		0xb54eaee5, 
		0x80200000, 
		0x2baf98f5, 
		0xc1500000, 
		0xc2180000, 
		0x0e003fff, 
		0x80000200, 
		0x85d61fd4, 
		0x807ffffe, 
		0x42600000, 
		0xe6bb8772, 
		0x422c0000, 
		0x0f7ffffc, 
		0x0f7ffffd, 
		0x80000000, 
		0x80000080, 
		0xff7ffffe, 
		0x1f526253, 
		0x7604d7ad, 
		0x0e3fffff, 
		0x8f46ef1b, 
		0x7f7ffffe, 
		0xc1200000, 
		0x31f5d800, 
		0xcb8c4b40, 
		0x00000000, 
		0x80000001, 
		0xe8488eea, 
		0xc1200000, 
		0x42680000, 
		0x63f6149d, 
		0x2fe9a94a, 
		0x00011111, 
		0xcd72a56b, 
		0x7f800000, 
		0x80000000, 
		0xc1600000, 
		0x80000000, 
		0xc2680000, 
		0x0c7e0000, 
		0x7f800000, 
		0x8baaffb9, 
		0xffc00000, 
		0x80011111, 
		0x80000000, 
		0xdf3c51ea, 
		0x80000800, 
		0x00800001, 
		0xd48bc0fc, 
		0x8f7ffffd, 
		0x5877c3f3, 
		0x80000400, 
		0x42640000, 
		0x7f800001, 
		0x4b2b141a, 
		0x00200000, 
		0x73e51abe, 
		0x00010000, 
		0x80000000, 
		0xff800000, 
		0x00800002, 
		0xc0800000, 
		0x001f5b3b, 
		0xffffffff, 
		0x0c7e0000, 
		0x41000000, 
		0x7f7ffffe, 
		0x0de18e61, 
		0x41d80000, 
		0x00000000, 
		0x41600000, 
		0x80000000, 
		0x40c00000, 
		0xbddee2ca, 
		0x0c7fffc0, 
		0x7f83aab9, 
		0xc2440000, 
		0x226b3f79, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x0e0000ff, 
		0x7f800000, 
		0x9f210b33, 
		0x41980000, 
		0x9fca814b, 
		0xc0c00000, 
		0x7fd54878, 
		0x425c0000, 
		0x7fbfffff, 
		0x99778dbf, 
		0x0e0000ff, 
		0x80000000, 
		0x00000000, 
		0xc2380000, 
		0x7f800000, 
		0xc1980000, 
		0x9dffea25, 
		0x00000010, 
		0x007aa3cb, 
		0xc0800000, 
		0xc1a80000, 
		0xc1f80000, 
		0x41200000, 
		0x801f30a2, 
		0xc1a80000, 
		0x80800003, 
		0x004f7b50, 
		0x7f800000, 
		0x06c952b3, 
		0x2f7bff40, 
		0x7f800000, 
		0x33333333, 
		0xc1600000, 
		0xc1880000, 
		0x00000001, 
		0x3a2ed1ae, 
		0x202f5c3f, 
		0x3abd912b, 
		0x807419ce, 
		0x41c80000, 
		0x426c0000, 
		0x0f7ffffc, 
		0x0e00000f, 
		0x08fd272f, 
		0x3942ca05, 
		0x147e2ed8, 
		0x90e56535, 
		0x80000000, 
		0xc22c0000, 
		0x487889ab, 
		0x80800002, 
		0xc1800000, 
		0x421c0000, 
		0x80000800, 
		0x58782ba3, 
		0x41d80000, 
		0x80000200, 
		0xa6cff87a, 
		0xa9aab58d, 
		0x1a8034bd, 
		0x0c7ffffe, 
		0x00000000, 
		0x0e00007f, 
		0xff7ffffe, 
		0xc46a6600, 
		0xa810d541, 
		0x954ef015, 
		0x00000002, 
		0x9faae6b3, 
		0x7f800000, 
		0x80ffffff, 
		0xc20c0000, 
		0x42380000, 
		0xda44f4a2, 
		0xff800000, 
		0x7f800000, 
		0x00800002, 
		0x7f8d0925, 
		0xff7ffffe, 
		0x42100000, 
		0xbf028f5c, 
		0xf73749fe, 
		0x00000040, 
		0x0e000fff, 
		0x21474fb1, 
		0x75172b8b, 
		0x41b80000, 
		0x14bbcc65, 
		0xff7fffff, 
		0x00000000, 
		0x39b184da, 
		0x00004000, 
		0x00000000, 
		0x55555555, 
		0xffffffff, 
		0x42b35f0b, 
		0x80000040, 
		0x04142b42, 
		0x424c0000, 
		0xc2280000, 
		0x00000000, 
		0xffc00001, 
		0x80800001, 
		0x02aaf394, 
		0x0c400000, 
		0xffa29a79, 
		0x7f7fffff, 
		0x0e000fff, 
		0x00000000, 
		0xe32b9962, 
		0x9f8edeb3, 
		0x239b777f, 
		0xf17c4c3c, 
		0x41a00000, 
		0x80008000, 
		0x7f800000, 
		0x7f800000, 
		0x80000000, 
		0x7db44430, 
		0xf99c99c9, 
		0xcb0ebd9c, 
		0x80000001, 
		0x00000000, 
		0x427c0000, 
		0x41e80000, 
		0x4b8c4b40, 
		0x80000100, 
		0xc2380000, 
		0x80800000, 
		0x0e3fffff, 
		0x61137a95, 
		0x423c0000, 
		0xff800000, 
		0x80000400, 
		0xe2e6dab4, 
		0x42200000, 
		0x0e01ffff, 
		0x0e003fff, 
		0x80800001, 
		0xc25c0000, 
		0x00020000, 
		0x9bb061f0, 
		0x7f7ffffe, 
		0x00000000, 
		0x0e07ffff, 
		0x00008000, 
		0x80000000, 
		0x9af6bdd5, 
		0x0e03ffff, 
		0x7140fe92, 
		0x80000020, 
		0x64d0ea44, 
		0x6d4bb4eb, 
		0x1c9f654b, 
		0x42305a51, 
		0x00011111, 
		0xff93a156, 
		0x7f800000, 
		0x1a2f39b7, 
		0x8f7fffff, 
		0x986349c8, 
		0xe5147f7d, 
		0x6e047b84, 
		0xbbf45b5d, 
		0x5cfbd4cd, 
		0x80000000, 
		0x42780000, 
		0x6e6f35bf, 
		0xc1400000, 
		0x80000000, 
		0xac59d4bd, 
		0x80008000, 
		0x0c600000, 
		0x80000000, 
		0xff800000, 
		0xd60df8f1, 
		0x00000000, 
		0x24380d0f, 
		0xc1c80000, 
		0xbf4f4065, 
		0x40c00000, 
		0xc2540000, 
		0x42440000, 
		0x80608987, 
		0xffffffff, 
		0xc2580000, 
		0x80020000, 
		0x512d2264, 
		0x50112efc, 
		0x00800000, 
		0xff800000, 
		0x7f800000, 
		0x7fa52e94, 
		0x80404c09, 
		0x42080000, 
		0x0c7ff800, 
		0x424c0000, 
		0x225f12b7, 
		0x8008679c, 
		0x80011111, 
		0x0c7ffff8, 
		0x42780000, 
		0x0e3244bf, 
		0x007fffff, 
		0x80000008, 
		0xcc64202a, 
		0x41700000, 
		0x55555555, 
		0xc2780000, 
		0x00800001, 
		0xae913691, 
		0xc1300000, 
		0xc1000000, 
		0x00010000, 
		0x80800001, 
		0xc48f0535, 
		0x0c7ffe00, 
		0x00000000, 
		0x807ffffe, 
		0x0e3fffff, 
		0x52211a45, 
		0x96f88a45, 
		0xacb4a190, 
		0x7990449a, 
		0x40400000, 
		0xc2780000, 
		0x421c0000, 
		0x423c0000, 
		0x420c0000, 
		0xc1f80000, 
		0x00000002, 
		0x42040000, 
		0x41700000, 
		0xff800001, 
		0x80800001, 
		0x00000000, 
		0xc2280000, 
		0x7fc00000, 
		0xcb8c4b40, 
		0x81a34ace, 
		0xc20c0000, 
		0x00ffffff, 
		0x00000000, 
		0x42380000, 
		0x394f61c1, 
		0x1fda4a3a, 
		0x0e00000f, 
		0x41c00000, 
		0x80040000, 
		0x00000000, 
		0x7fb397d3, 
		0xc2280000, 
		0xacab6239, 
		0x94795424, 
		0xffc00000, 
		0x00000000, 
		0x802bda62, 
		0x42540000, 
		0xeb947752, 
		0xc2240000, 
		0x41f80000, 
		0x80010000, 
		0x0c7ff000, 
		0x866c2d94, 
		0x8e4a0507, 
		0x00800000, 
		0x503c6833, 
		0x41200000, 
		0xc1980000, 
		0x28d9e98e, 
		0xc1880000, 
		0x8f7ffffc, 
		0x7e00d2f2, 
		0xc27f0d69, 
		0x0e000003, 
		0x7f7ffffe, 
		0x42000000, 
		0x455c69ca, 
		0x80800003, 
		0x0c600000, 
		0x7f800000, 
		0x00008000, 
		0xc911420b, 
		0x42200000, 
		0xff800000, 
		0xc2280000, 
		0x7f800000, 
		0x00000000, 
		0x0c7ffff0, 
		0x7dceba09, 
		0xf1b780fb, 
		0x41d00000, 
		0x00000000, 
		0x00000040, 
		0x40e00000, 
		0x42600000, 
		0x41b00000, 
		0x80000000, 
		0x0c7f0000, 
		0x0c7ffe00, 
		0xffb691d8, 
		0x7f000000, 
		0x40400000, 
		0x00000002, 
		0x41100000, 
		0x125f64f7, 
		0x41200000, 
		0x0e000003, 
		0xbac1d24b, 
		0xc0400000, 
		0x41100000, 
		0x8a15568d, 
		0x3f800001, 
		0x42680000, 
		0x9f32d925, 
		0x80000400, 
		0x80000000, 
		0xaa220106, 
		0x42500000, 
		0x80000080, 
		0x7f7ffffe, 
		0x0c7ff000, 
		0xff800000, 
		0x42500000, 
		0x0e0000ff, 
		0xca303e8c, 
		0x979048f4, 
		0xc1700000, 
		0x7fbfffff, 
		0x0c700000, 
		0x0c7ffff0, 
		0xc1a00000, 
		0xc21c0000, 
		0x78671435, 
		0x12c84ec9, 
		0x42280000, 
		0x80800002, 
		0x4b8c4b40, 
		0xc23c0000, 
		0xc0800000, 
		0x0e0003ff, 
		0xb1f74fe8, 
		0x80011111, 
		0x0e007fff, 
		0x3341c996, 
		0x42780000, 
		0x7b69d2d9, 
		0x00000040, 
		0x0e000fff, 
		0xffbfc298, 
		0xc0800000, 
		0x7fffffff, 
		0x9b10313c, 
		0x7f7ffffe, 
		0x7085bb9c, 
		0xd192f4f3, 
		0x80001000, 
		0x13ddaf2c, 
		0x41d80000, 
		0x0e000fff, 
		0x80000008, 
		0x41600000, 
		0x4e77eba0, 
		0xff800000, 
		0xc0e00000, 
		0x9166987c, 
		0x002327f3, 
		0x0c7ffff8, 
		0x42740000, 
		0x0c7ffe00, 
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
		0x0000010b, 
		0x00000383, 
		0x0000022f, 
		0x0000015f, 
		0x0000013f, 
		0x00000043, 
		0x00000023, 
		0x000001f1, 
		0x0000018f, 
		0x00000051, 
		0x000000fd, 
		0x000000ad, 
		0x00000285, 
		0x000002db, 
		0x00000363, 
		0x000000af, 
		0x00000259, 
		0x000001ef, 
		0x000002df, 
		0x00000397, 
		0x0000012f, 
		0x000001a5, 
		0x00000293, 
		0x000003cf, 
		0x00000299, 
		0x000002b3, 
		0x0000023f, 
		0x000000b7, 
		0x000002dd, 
		0x000001ef, 
		0x0000016f, 
		0x00000395, 
		0x00000025, 
		0x000000a5, 
		0x000003ab, 
		0x000000bd, 
		0x000003c1, 
		0x00000219, 
		0x0000000b, 
		0x0000011f, 
		0x00000213, 
		0x000001a5, 
		0x00000383, 
		0x00000017, 
		0x000003af, 
		0x0000008b, 
		0x000001ad, 
		0x00000085, 
		0x000003dd, 
		0x00000279, 
		0x00000349, 
		0x000003ef, 
		0x000001e5, 
		0x00000359, 
		0x00000017, 
		0x00000103, 
		0x00000057, 
		0x000002fb, 
		0x000000ab, 
		0x00000129, 
		0x0000039f, 
		0x00000239, 
		0x00000215, 
		0x000001e5, 
		0x00000389, 
		0x000000cd, 
		0x0000015d, 
		0x000003c1, 
		0x000003eb, 
		0x0000030b, 
		0x00000165, 
		0x000003dd, 
		0x000003a1, 
		0x000002ad, 
		0x0000024f, 
		0x00000005, 
		0x0000035f, 
		0x00000313, 
		0x00000359, 
		0x00000137, 
		0x00000169, 
		0x00000275, 
		0x00000257, 
		0x000003ab, 
		0x00000319, 
		0x00000371, 
		0x0000027f, 
		0x0000022b, 
		0x0000006d, 
		0x00000017, 
		0x0000023f, 
		0x00000309, 
		0x000003b1, 
		0x000002bd, 
		0x0000015d, 
		0x00000193, 
		0x0000001f, 
		0x00000289, 
		0x0000030d, 
		0x0000027b, 
		0x000003a9, 
		0x00000049, 
		0x000003e7, 
		0x0000017f, 
		0x00000343, 
		0x000003a5, 
		0x000003cb, 
		0x000003ed, 
		0x000000dd, 
		0x0000037d, 
		0x000000dd, 
		0x00000391, 
		0x000003bf, 
		0x00000035, 
		0x000001db, 
		0x000001bf, 
		0x000003b1, 
		0x00000149, 
		0x00000025, 
		0x00000031, 
		0x000002cf, 
		0x00000201, 
		0x000000cb, 
		0x0000010b, 
		0x000001f3, 
		0x00000189, 
		0x0000035b, 
		0x00000227, 
		0x000002ab, 
		0x0000024b, 
		0x000001a9, 
		0x0000023d, 
		0x00000157, 
		0x000002ed, 
		0x00000097, 
		0x00000391, 
		0x00000371, 
		0x00000269, 
		0x0000017b, 
		0x00000287, 
		0x00000277, 
		0x000000f7, 
		0x0000013b, 
		0x00000061, 
		0x00000037, 
		0x0000011b, 
		0x00000297, 
		0x00000201, 
		0x000002a7, 
		0x00000383, 
		0x0000000d, 
		0x00000375, 
		0x000000db, 
		0x00000041, 
		0x000000ab, 
		0x0000032b, 
		0x000003ef, 
		0x000003f3, 
		0x000001c5, 
		0x0000019f, 
		0x0000022d, 
		0x000002ad, 
		0x0000020d, 
		0x00000053, 
		0x0000022f, 
		0x0000031d, 
		0x00000173, 
		0x00000185, 
		0x000003cf, 
		0x000000b1, 
		0x000003fb, 
		0x00000091, 
		0x00000117, 
		0x00000149, 
		0x000000b5, 
		0x00000285, 
		0x00000357, 
		0x00000055, 
		0x00000265, 
		0x0000038b, 
		0x00000271, 
		0x00000095, 
		0x00000225, 
		0x00000195, 
		0x000002eb, 
		0x000003df, 
		0x000002a9, 
		0x00000179, 
		0x000002c5, 
		0x00000153, 
		0x000000fd, 
		0x000000db, 
		0x0000031b, 
		0x00000153, 
		0x000002f9, 
		0x000002e3, 
		0x00000227, 
		0x00000115, 
		0x0000027b, 
		0x00000383, 
		0x00000041, 
		0x0000022f, 
		0x000003fb, 
		0x000003f7, 
		0x00000351, 
		0x000003ad, 
		0x0000024d, 
		0x00000271, 
		0x0000027f, 
		0x000000dd, 
		0x000003fd, 
		0x000003dd, 
		0x000000d3, 
		0x00000267, 
		0x0000004d, 
		0x00000113, 
		0x000000b7, 
		0x00000195, 
		0x00000361, 
		0x00000091, 
		0x00000377, 
		0x00000067, 
		0x0000000f, 
		0x000002e7, 
		0x000002d9, 
		0x000003c7, 
		0x00000027, 
		0x000002d3, 
		0x000003f5, 
		0x00000291, 
		0x00000111, 
		0x00000047, 
		0x0000031d, 
		0x000000db, 
		0x0000023f, 
		0x0000032d, 
		0x0000034f, 
		0x00000169, 
		0x000003e1, 
		0x00000219, 
		0x00000387, 
		0x0000000f, 
		0x000000f3, 
		0x0000013f, 
		0x0000025b, 
		0x00000317, 
		0x00000393, 
		0x000003f7, 
		0x00000147, 
		0x000002d1, 
		0x000000a3, 
		0x00000007, 
		0x00000091, 
		0x00000181, 
		0x0000034f, 
		0x00000085  
};

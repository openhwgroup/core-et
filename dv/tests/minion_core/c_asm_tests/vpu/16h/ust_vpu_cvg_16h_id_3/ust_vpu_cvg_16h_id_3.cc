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
			"li x5, 373\n"
			"faddi.pi f2, f0, 288\n"
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
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
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
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
			"li x0, 0x190\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 288(x12)\n"
			"flw.ps f15, 928(x12)\n"
			"fadd.s f3, f12, f15, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 640(x28)\n"
			"flw.ps f31, 576(x28)\n"
			"fle.pi f26, f26, f31\n" 
			"li x5, 0xf795\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 160(x16)\n"
			"fclass.s x26, f17\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 672(x16)\n"
			"flw.ps f20, 576(x16)\n"
			"flt.s x27, f2, f20\n" 
			"mova.x.m x12\n" 
			"la x25, rand_int_32\n"
			"flw.ps f4, 64(x25)\n"
			"fsrl.pi f17, f4, f4\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 480(x16)\n"
			"fclass.ps f31, f26\n" 
			"maskpopc x26, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x16, 0xe64469872a5d2ebe\n"
			"fcvt.s.w f25, x16, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 512(x12)\n"
			"flw.ps f25, 320(x12)\n"
			"flw.ps f8, 736(x12)\n"
			"fmadd.s f1, f30, f25, f8, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"fltm.pi m6, f12, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.ps m0, f5, f0\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 800(x5)\n"
			"flw.ps f12, 832(x5)\n"
			"flw.ps f15, 928(x5)\n"
			"fmsub.s f17, f6, f12, f15, rne\n" 
			"li x12, 0xd82dc7aa8fb2a36c\n"
			"fcvt.s.w f4, x12, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa115466a27\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f6, 448(x5)\n"
			"flw.ps f25, 992(x5)\n"
			"fmul.pi f2, f6, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x12, 0x7210011\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x12, x12, x7\n"
			"csrw tensor_reduce, x12\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_657024_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_655134_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
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
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb1 << TENSOR_IMA_SCP_LOC_B) | (0xc9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd7 << TENSOR_IMA_SCP_LOC_B) | (0x54 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f20, 384(x11)\n"
			"flw.ps f30, 768(x11)\n"
			"fmin.pi f1, f20, f30\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 928(x13)\n"
			"flw.ps f8, 800(x13)\n"
			"fmin.ps f4, f0, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 416(x11)\n"
			"frsq.ps f4, f6\n" 
			"li x5, 0x800e\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f0, 576(x23)\n"
			"flw.ps f15, 64(x23)\n"
			"fle.pi f17, f0, f15\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 608(x5)\n"
			"flw.ps f17, 448(x5)\n"
			"fmul.ps f30, f8, f17, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 64(x27)\n"
			"flw.ps f15, 480(x27)\n"
			"flw.ps f20, 192(x27)\n"
			"fcmov.ps f31, f2, f15, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"maskand m1, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 448(x5)\n"
			"flw.ps f15, 160(x5)\n"
			"fcmovm.ps f4, f5, f15\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 32(x23)\n"
			"flw.ps f31, 352(x23)\n"
			"fmulh.pi f0, f8, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 416(x28)\n"
			"flw.ps f8, 704(x28)\n"
			"fsub.ps f2, f15, f8, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"fltm.pi m1, f4, f30\n" 
			"li x5, 0x803401C0\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_int_32\n"
			"flw.ps f26, 640(x23)\n"
			"flw.ps f25, 576(x23)\n"
			"fle.pi f4, f26, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 960(x25)\n"
			"flw.ps f15, 32(x25)\n"
			"flw.ps f6, 160(x25)\n"
			"fmadd.s f3, f20, f15, f6, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 800(x23)\n"
			"fpackrepb.pi f20, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 480(x17)\n"
			"flw.ps f17, 800(x17)\n"
			"fle.ps f25, f5, f17\n" 
			"la x25, rand_int_32\n"
			"flw.ps f26, 0(x25)\n"
			"fpackrepb.pi f26, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_526223_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x4f << TENSOR_FMA32_SCP_LOC_B) | (0x29 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_424468_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x87 << TENSOR_IMA_SCP_LOC_B) | (0xb5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_110742_HID_0:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x27, 0x18000000041e0019\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x27, x27, x7\n"
			"csrw tensor_reduce, x27\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"li x27, 0x3ea5c7ccf3b1f69d\n"
			"fcvt.s.w f2, x27, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 960(x17)\n"
			"flw.ps f12, 416(x17)\n"
			"fsgnjn.ps f15, f26, f12\n" 
			"li x5, 0xe74373e062d7e222\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc26d8f67a1075107\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f5, 288(x23)\n"
			"flw.ps f3, 896(x23)\n"
			"fmul.pi f5, f5, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 576(x11)\n"
			"flw.ps f26, 768(x11)\n"
			"flt.pi f20, f15, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 608(x28)\n"
			"flw.ps f15, 928(x28)\n"
			"fsgnjn.ps f0, f1, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 672(x16)\n"
			"fswizz.ps f5, f4, 0xa\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 0(x5)\n"
			"fclass.ps f5, f15\n" 
			"li x5, 0xb4f6\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 736(x27)\n"
			"flog.ps f20, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 320(x27)\n"
			"flw.ps f5, 960(x27)\n"
			"flw.ps f20, 128(x27)\n"
			"fmsub.s f2, f6, f5, f20, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 0(x23)\n"
			"flw.ps f15, 832(x23)\n"
			"fsgnjn.s f3, f17, f15\n" 
			"li x5, 0x34a7\n"
			"csrw tensor_mask, x5\n"	
			"li x27, 0xa6f01b3d83f2eb57\n"
			"fcvt.s.wu f8, x27, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 928(x11)\n"
			"fpackreph.pi f5, f15\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 704(x17)\n"
			"flw.ps f15, 416(x17)\n"
			"fsgnjx.s f8, f26, f15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"flem.ps m2, f4, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 928(x12)\n"
			"frsq.ps f25, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 96(x13)\n"
			"fcvt.l.s x27, f25, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_453768_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x82 << TENSOR_IMA_SCP_LOC_B) | (0x94 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f12, 832(x11)\n"
			"flw.ps f31, 608(x11)\n"
			"fmulhu.pi f6, f12, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 32(x26)\n"
			"fcvt.lu.s x26, f8, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 0(x5)\n"
			"flw.ps f6, 896(x5)\n"
			"fle.ps f12, f17, f6\n" 
			"li x5, 0x5659\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 0(x23)\n"
			"flw.ps f2, 800(x23)\n"
			"flw.ps f31, 288(x23)\n"
			"fnmadd.ps f30, f1, f2, f31, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 576(x13)\n"
			"flw.ps f15, 576(x13)\n"
			"feq.ps f6, f8, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 64(x12)\n"
			"flw.ps f12, 352(x12)\n"
			"fsub.pi f3, f26, f12\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_0:\n"
			"maskand m2, m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 640(x5)\n"
			"flw.ps f12, 800(x5)\n"
			"flw.ps f1, 352(x5)\n"
			"fmsub.s f31, f20, f12, f1, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 512(x28)\n"
			"flw.ps f12, 384(x28)\n"
			"fadd.pi f0, f26, f12\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_0:\n"
			"masknot m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f1, 320(x28)\n"
			"fpackrepb.pi f0, f1\n" 
			"li x5, 0x45a9\n"
			"csrw tensor_mask, x5\n"	
			"li x13, 0x1473d2c919f5354e\n"
			"fcvt.s.w f3, x13, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 960(x25)\n"
			"flw.ps f1, 448(x25)\n"
			"fxor.pi f6, f8, f1\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 544(x28)\n"
			"flw.ps f15, 96(x28)\n"
			"fmaxu.pi f1, f5, f15\n" 
			"la x16, rand_int_32\n"
			"flw.ps f15, 256(x16)\n"
			"flw.ps f25, 704(x16)\n"
			"fmin.pi f4, f15, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 992(x25)\n"
			"fcvt.l.s x13, f8, rne\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"flw.ps f22, 640(x6)\n"
			"flw.ps f20, 704(x6)\n"
			"feq.ps f16, f22, f20\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 352(x12)\n"
			"flw.ps f4, 256(x12)\n"
			"fsgnj.ps f26, f20, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 928(x10)\n"
			"flw.ps f18, 256(x10)\n"
			"fsub.ps f0, f16, f18, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 0(x12)\n"
			"flw.ps f6, 864(x12)\n"
			"flw.ps f22, 128(x12)\n"
			"fmsub.ps f20, f0, f6, f22, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 672(x11)\n"
			"flw.ps f15, 992(x11)\n"
			"fsgnjn.s f26, f22, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 320(x9)\n"
			"fswizz.ps f16, f15, 0x5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 608(x28)\n"
			"fclass.s x6, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 608(x12)\n"
			"fsrai.pi f31, f31, 0x6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 288(x11)\n"
			"flw.ps f15, 672(x11)\n"
			"feq.ps f0, f13, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 320(x28)\n"
			"fcvt.pw.ps f4, f20, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 768(x11)\n"
			"flw.ps f0, 288(x11)\n"
			"fmul.s f17, f13, f0, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 384(x27)\n"
			"fnot.pi f22, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 160(x26)\n"
			"flw.ps f17, 224(x26)\n"
			"fxor.pi f15, f4, f17\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 704(x10)\n"
			"flw.ps f0, 416(x10)\n"
			"fsgnj.ps f27, f27, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 288(x10)\n"
			"fsrli.pi f4, f17, 0x1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"masknot m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f26, 192(x7)\n"
			"fsrai.pi f11, f26, 0xf\n" 
			"mova.x.m x9\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 96(x23)\n"
			"flw.ps f16, 672(x23)\n"
			"fcmovm.ps f18, f17, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 160(x12)\n"
			"flw.ps f0, 896(x12)\n"
			"flw.ps f2, 960(x12)\n"
			"fmadd.s f31, f10, f0, f2, rdn\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f27, 704(x7)\n"
			"fcvt.ps.f16 f0, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_1:\n"
			"mova.m.x x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f6, 128(x10)\n"
			"flw.ps f26, 192(x10)\n"
			"fmul.pi f18, f6, f26\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 352(x11)\n"
			"flw.ps f15, 960(x11)\n"
			"fmulh.pi f27, f16, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 576(x9)\n"
			"fsatu8.pi f22, f20\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f2, 64(x11)\n"
			"fcvt.ps.f16 f10, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 704(x12)\n"
			"flw.ps f27, 544(x12)\n"
			"fmul.pi f16, f16, f27\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 288(x28)\n"
			"flw.ps f2, 640(x28)\n"
			"fsll.pi f10, f15, f2\n" 
			"maskpopcz x26, m7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 224(x10)\n"
			"flw.ps f10, 416(x10)\n"
			"feq.pi f26, f27, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 96(x28)\n"
			"flw.ps f26, 448(x28)\n"
			"fadd.ps f11, f22, f26, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 32(x27)\n"
			"flw.ps f15, 864(x27)\n"
			"flw.ps f16, 960(x27)\n"
			"fmadd.ps f15, f22, f15, f16, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_1:\n"
			"fltm.pi m7, f11, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 448(x27)\n"
			"flw.ps f31, 352(x27)\n"
			"fsub.ps f11, f22, f31, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 224(x27)\n"
			"fclass.s x23, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 480(x28)\n"
			"fcvt.wu.s x23, f15, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 576(x23)\n"
			"flw.ps f6, 96(x23)\n"
			"fadd.ps f4, f13, f6, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 96(x27)\n"
			"flw.ps f18, 96(x27)\n"
			"fmax.s f22, f26, f18\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_1:\n"
			"masknot m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f10, 320(x9)\n"
			"faddi.pi f10, f10, 0x3a\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_1:\n"
			"maskand m7, m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"flem.ps m2, f22, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 704(x27)\n"
			"flw.ps f17, 640(x27)\n"
			"fsgnj.ps f17, f26, f17\n" 
			"li x10, 0xab20043b3cea2214\n"
			"fcvt.s.wu f31, x10, rtz\n" 
			"li x23, 0x1fa8a0a95f313637\n"
			"fcvt.s.l f0, x23, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 96(x6)\n"
			"flw.ps f18, 32(x6)\n"
			"fsgnj.s f27, f16, f18\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"fltm.pi m4, f22, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 960(x6)\n"
			"fcvt.pw.ps f11, f18, rdn\n" 
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
			"flw.ps f27, 320(x6)\n"
			"flw.ps f15, 800(x6)\n"
			"fmaxu.pi f4, f27, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 544(x28)\n"
			"flw.ps f15, 864(x28)\n"
			"fadd.pi f31, f13, f15\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 320(x11)\n"
			"flw.ps f15, 608(x11)\n"
			"fsgnj.ps f15, f11, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 288(x26)\n"
			"flw.ps f16, 384(x26)\n"
			"fadd.s f22, f31, f16, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 576(x28)\n"
			"fcvt.pw.ps f18, f27, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 512(x27)\n"
			"flw.ps f11, 928(x27)\n"
			"flw.ps f20, 640(x27)\n"
			"fnmsub.ps f2, f16, f11, f20, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_1:\n"
			"maskand m6, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f20, 0x1bbe9\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_1:\n"
			"fsetm.pi m6, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 800(x11)\n"
			"flw.ps f22, 32(x11)\n"
			"fsgnjn.s f31, f18, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 0(x10)\n"
			"flw.ps f31, 960(x10)\n"
			"flw.ps f11, 864(x10)\n"
			"fmsub.s f17, f2, f31, f11, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 128(x7)\n"
			"flw.ps f11, 96(x7)\n"
			"fmul.ps f22, f4, f11, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f31, 512(x27)\n"
			"faddi.pi f26, f31, 0x1f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 864(x7)\n"
			"flw.ps f10, 160(x7)\n"
			"flw.ps f0, 768(x7)\n"
			"fmsub.s f31, f18, f10, f0, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f31, 64(x26)\n"
			"flw.ps f0, 32(x26)\n"
			"fmulhu.pi f26, f31, f0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 256(x6)\n"
			"flw.ps f13, 576(x6)\n"
			"fsgnjx.ps f15, f31, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"LBL_HPM_4_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_FAIL_HID_1:\n"          
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
			"LBL_HPM_4_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
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
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_2\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 672(x16)\n"
			"fcvt.pwu.ps f7, f11, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 896(x11)\n"
			"flw.ps f27, 512(x11)\n"
			"fnmadd.s f23, f20, f27, f20, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 384(x18)\n"
			"flw.ps f3, 608(x18)\n"
			"flw.ps f19, 416(x18)\n"
			"fcmov.ps f4, f9, f3, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskand m7, m1, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 960(x17)\n"
			"fcvt.w.s x9, f22, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"fltm.ps m0, f22, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 576(x25)\n"
			"flw.ps f24, 608(x25)\n"
			"fsgnj.ps f26, f26, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f22, 608(x12)\n"
			"flw.ps f11, 544(x12)\n"
			"fmin.pi f18, f22, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 832(x12)\n"
			"fswizz.ps f3, f20, 0x9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 480(x25)\n"
			"flw.ps f21, 896(x25)\n"
			"flt.pi f3, f22, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 448(x16)\n"
			"flw.ps f8, 64(x16)\n"
			"fsgnjx.s f19, f7, f8\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 416(x25)\n"
			"flw.ps f26, 800(x25)\n"
			"fxor.pi f9, f22, f26\n" 
			"li x5, 0x6047\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f8, 832(x11)\n"
			"flw.ps f3, 448(x11)\n"
			"fsub.pi f27, f8, f3\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_2:\n"
			"fltm.pi m1, f18, f18\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd2548fabef\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f22, 384(x25)\n"
			"fslli.pi f20, f22, 0x5\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_2:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 352(x16)\n"
			"frcp.ps f26, f27\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xc4 << TENSOR_IMA_SCP_LOC_B) | (0x4e << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 896(x17)\n"
			"flw.ps f24, 416(x17)\n"
			"fsgnjx.ps f9, f9, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 832(x17)\n"
			"fround.ps f22, f21, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f19, 480(x12)\n"
			"flw.ps f24, 288(x12)\n"
			"fsub.pi f22, f19, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"mov.m.x m4, x18, 0x1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f11, 128(x9)\n"
			"flw.ps f4, 160(x9)\n"
			"fmul.pi f27, f11, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 0(x18)\n"
			"flw.ps f21, 64(x18)\n"
			"feq.s x13, f18, f21\n" 
			"fbci.ps f18, 0x51538\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 320(x18)\n"
			"flw.ps f27, 896(x18)\n"
			"fmul.s f20, f26, f27, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 672(x18)\n"
			"ffrc.ps f2, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_2:\n"
			"mov.m.x m7, x25, 0x4e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 864(x22)\n"
			"fnot.pi f19, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 192(x12)\n"
			"fcvt.ps.pw f18, f21, rup\n" 
			"fbci.pi f24, 0x2b8e2\n" 
			"mova.x.m x25\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 544(x12)\n"
			"flw.ps f22, 192(x12)\n"
			"flw.ps f26, 704(x12)\n"
			"fmadd.s f19, f18, f22, f26, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 768(x12)\n"
			"flw.ps f11, 128(x12)\n"
			"fle.ps f18, f22, f11\n" 
			"li x5, 0x3132\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_410276_HID_2:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_435231_HID_2:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x51 << TENSOR_IMA_SCP_LOC_B) | (0xd << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 864(x22)\n"
			"flw.ps f26, 864(x22)\n"
			"flw.ps f11, 448(x22)\n"
			"fcmov.ps f19, f7, f26, f11\n" 
			"la x11, rand_int_32\n"
			"flw.ps f26, 736(x11)\n"
			"flw.ps f24, 544(x11)\n"
			"fmulh.pi f4, f26, f24\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"fltm.ps m0, f3, f23\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x34798d74c7\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f26, 320(x12)\n"
			"fsatu8.pi f9, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f11, 448(x22)\n"
			"flw.ps f7, 672(x22)\n"
			"fsrl.pi f9, f11, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 960(x16)\n"
			"flw.ps f20, 448(x16)\n"
			"fsll.pi f20, f27, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f26, 672(x22)\n"
			"flw.ps f2, 704(x22)\n"
			"fmin.pi f27, f26, f2\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f4, 32(x18)\n"
			"fcvt.ps.f16 f18, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 320(x10)\n"
			"flw.ps f3, 736(x10)\n"
			"fmax.ps f9, f4, f3\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 896(x16)\n"
			"fcvt.wu.s x10, f23, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 32(x10)\n"
			"flw.ps f9, 384(x10)\n"
			"fxor.pi f27, f21, f9\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 864(x18)\n"
			"fsatu8.pi f24, f7\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 96(x25)\n"
			"flw.ps f21, 416(x25)\n"
			"fsub.ps f8, f4, f21, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 896(x9)\n"
			"fsatu8.pi f20, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 640(x22)\n"
			"flw.ps f3, 544(x22)\n"
			"fsub.s f7, f18, f3, rmm\n" 
			"la x16, rand_ieee754_16\n"
			"flw.ps f26, 832(x16)\n"
			"fcvt.ps.f16 f27, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f9, 256(x17)\n"
			"flw.ps f20, 640(x17)\n"
			"fmax.pi f9, f9, f20\n" 
			"fbci.pi f21, 0x19528\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 320(x22)\n"
			"flw.ps f4, 576(x22)\n"
			"fadd.ps f3, f23, f4, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 544(x18)\n"
			"fsrai.pi f20, f2, 0x6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 160(x12)\n"
			"flw.ps f4, 384(x12)\n"
			"fadd.ps f3, f22, f4, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 864(x18)\n"
			"fcvt.ps.pw f22, f18, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_int_32\n"
			"flw.ps f26, 32(x13)\n"
			"flw.ps f20, 640(x13)\n"
			"fltu.pi f27, f26, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 992(x13)\n"
			"fcvt.f16.ps f24, f4\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 448(x25)\n"
			"flw.ps f4, 96(x25)\n"
			"feq.pi f11, f9, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 768(x10)\n"
			"fpackrepb.pi f27, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f26, 512(x11)\n"
			"flw.ps f21, 512(x11)\n"
			"fand.pi f3, f26, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 128(x9)\n"
			"flw.ps f26, 992(x9)\n"
			"flt.pi f3, f3, f26\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_2:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 320(x17)\n"
			"flw.ps f2, 416(x17)\n"
			"feq.ps f11, f23, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 320(x16)\n"
			"fround.ps f11, f19, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 576(x25)\n"
			"flw.ps f26, 352(x25)\n"
			"fmin.ps f8, f4, f26\n" 
			"li x5, 0x3eeb\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_314185_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x22, rand_int_32\n"
			"flw.ps f10, 576(x22)\n"
			"flw.ps f9, 192(x22)\n"
			"flt.pi f28, f10, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fsetm.pi m7, f15\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_int_32\n"
			"flw.ps f25, 832(x15)\n"
			"flw.ps f17, 544(x15)\n"
			"fminu.pi f9, f25, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 512(x5)\n"
			"fclass.ps f23, f10\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 0(x18)\n"
			"flw.ps f29, 320(x18)\n"
			"flw.ps f30, 160(x18)\n"
			"fmsub.s f14, f10, f29, f30, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 800(x5)\n"
			"flw.ps f19, 128(x5)\n"
			"fminu.pi f22, f15, f19\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 704(x6)\n"
			"flw.ps f23, 224(x6)\n"
			"fcmovm.ps f18, f9, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc1d91a66ac\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 832(x5)\n"
			"fcvt.wu.s x15, f9, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_3:\n"
			"flem.ps m0, f29, f23\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 640(x22)\n"
			"fround.ps f15, f10, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 416(x23)\n"
			"fslli.pi f10, f19, 0x0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 320(x15)\n"
			"flw.ps f17, 320(x15)\n"
			"fadd.ps f29, f30, f17, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f3, 128(x15)\n"
			"flw.ps f14, 128(x15)\n"
			"fsub.pi f23, f3, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 832(x23)\n"
			"fsrli.pi f19, f28, 0x4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 896(x18)\n"
			"ffrc.ps f23, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 704(x15)\n"
			"flw.ps f18, 576(x15)\n"
			"fsub.ps f23, f10, f18, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 384(x5)\n"
			"flw.ps f1, 448(x5)\n"
			"fle.pi f3, f9, f1\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 256(x6)\n"
			"flw.ps f29, 416(x6)\n"
			"fsrl.pi f29, f15, f29\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 832(x7)\n"
			"fsrli.pi f19, f28, 0xc\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 416(x7)\n"
			"flw.ps f10, 192(x7)\n"
			"fsgnjx.ps f10, f18, f10\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 192(x18)\n"
			"flw.ps f28, 928(x18)\n"
			"fsll.pi f3, f19, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 928(x6)\n"
			"flw.ps f23, 768(x6)\n"
			"fadd.s f19, f29, f23, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 544(x18)\n"
			"fsatu8.pi f1, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"fsetm.pi m5, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f19, 960(x22)\n"
			"fsat8.pi f25, f19\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 0(x26)\n"
			"flw.ps f3, 896(x26)\n"
			"flt.pi f10, f30, f3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 864(x22)\n"
			"flw.ps f29, 544(x22)\n"
			"fand.pi f1, f9, f29\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 0(x23)\n"
			"flw.ps f23, 64(x23)\n"
			"fmaxu.pi f3, f9, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 736(x18)\n"
			"flw.ps f9, 32(x18)\n"
			"feq.s x18, f14, f9\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 384(x22)\n"
			"flw.ps f1, 352(x22)\n"
			"fmul.s f8, f30, f1, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 160(x22)\n"
			"flw.ps f3, 352(x22)\n"
			"flw.ps f30, 672(x22)\n"
			"fnmsub.ps f14, f1, f3, f30, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f15, 224(x5)\n"
			"fltu.pi f25, f15, f15\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 512(x6)\n"
			"flw.ps f18, 512(x6)\n"
			"fle.ps f3, f25, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 288(x23)\n"
			"flw.ps f19, 864(x23)\n"
			"fand.pi f3, f29, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 480(x7)\n"
			"fround.ps f18, f1, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_3:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f19, 512(x15)\n"
			"flw.ps f22, 896(x15)\n"
			"feq.pi f9, f19, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 416(x23)\n"
			"flw.ps f17, 192(x23)\n"
			"fmul.s f10, f10, f17, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_3:\n"
			"maskor m7, m0, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401E0\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 928(x15)\n"
			"flw.ps f9, 512(x15)\n"
			"flt.ps f8, f1, f9\n" 
			"li x10, 0xc6b32470484b72ca\n"
			"fcvt.s.lu f14, x10, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_3:\n"
			"fsetm.pi m7, f9\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x945d626bae\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f17, 448(x6)\n"
			"fcvt.ps.pwu f10, f17, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_3:\n"
			"maskor m0, m4, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 64(x6)\n"
			"fround.ps f25, f29, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 544(x18)\n"
			"fsin.ps f19, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 544(x6)\n"
			"flw.ps f28, 160(x6)\n"
			"fmin.s f15, f17, f28\n" 
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
			"mova.x.m x15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 128(x22)\n"
			"flw.ps f23, 544(x22)\n"
			"flw.ps f9, 832(x22)\n"
			"fcmov.ps f19, f22, f23, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 832(x7)\n"
			"flw.ps f25, 320(x7)\n"
			"flt.s x26, f10, f25\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 512(x15)\n"
			"flw.ps f25, 928(x15)\n"
			"flt.pi f1, f22, f25\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 256(x23)\n"
			"flw.ps f9, 960(x23)\n"
			"flw.ps f28, 640(x23)\n"
			"fnmadd.s f29, f1, f9, f28, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 32(x22)\n"
			"fdiv.pi f15, f25, f25\n" 
			"li x23, 0x8a3fa66eed97298e\n"
			"fcvt.s.l f8, x23, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_3:\n"
			"maskor m7, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f19, 704(x15)\n"
			"flw.ps f28, 736(x15)\n"
			"fle.pi f9, f19, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 0(x6)\n"
			"flw.ps f14, 640(x6)\n"
			"flw.ps f18, 992(x6)\n"
			"fnmadd.s f22, f8, f14, f18, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 544(x5)\n"
			"flw.ps f22, 192(x5)\n"
			"fmul.ps f23, f3, f22, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 672(x26)\n"
			"flw.ps f18, 160(x26)\n"
			"fmul.s f28, f8, f18, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 736(x5)\n"
			"fcvt.wu.s x28, f28, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 992(x22)\n"
			"flw.ps f1, 960(x22)\n"
			"fsub.ps f18, f23, f1, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f25, 832(x18)\n"
			"flw.ps f28, 576(x18)\n"
			"fsra.pi f3, f25, f28\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 704(x15)\n"
			"fsrai.pi f30, f15, 0x3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f27,  192 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x23, rand_int_32\n"
			"flw.ps f24, 256(x23)\n"
			"flw.ps f21, 320(x23)\n"
			"fmaxu.pi f27, f24, f21\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 0(x7)\n"
			"flw.ps f23, 736(x7)\n"
			"fmadd.ps f21, f25, f23, f23, rtz\n" 
			"li x5, 0x6123\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskor m7, m7, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x982be962e\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 480(x11)\n"
			"flw.ps f20, 768(x11)\n"
			"fle.ps f5, f4, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 864(x27)\n"
			"flw.ps f12, 928(x27)\n"
			"flw.ps f7, 352(x27)\n"
			"fnmsub.ps f23, f5, f12, f7, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 192(x11)\n"
			"flw.ps f25, 640(x11)\n"
			"fmul.pi f15, f15, f25\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 320(x18)\n"
			"fpackreph.pi f5, f5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 256(x23)\n"
			"fcvt.lu.s x14, f4, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 896(x28)\n"
			"flw.ps f24, 160(x28)\n"
			"fdiv.ps f20, f21, f24, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 256(x25)\n"
			"fcvt.wu.s x7, f21, rmm\n" 
			"li x5, 0x4500feeb6ccb484f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x65b3d89c965e743d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 192(x11)\n"
			"fround.ps f27, f12, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 160(x27)\n"
			"ffrc.ps f23, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 480(x9)\n"
			"fslli.pi f5, f5, 0x7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 960(x9)\n"
			"fclass.s x14, f12\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 96(x14)\n"
			"flw.ps f20, 352(x14)\n"
			"fle.pi f23, f0, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 224(x20)\n"
			"fclass.s x7, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x11, 0x210000\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x11, x11, x7\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_693536_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 704(x20)\n"
			"flw.ps f4, 0(x20)\n"
			"flw.ps f15, 64(x20)\n"
			"fnmsub.s f24, f27, f4, f15, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_4:\n"
			"flem.ps m0, f12, f9\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 704(x27)\n"
			"flw.ps f27, 320(x27)\n"
			"flw.ps f5, 544(x27)\n"
			"fmadd.ps f19, f0, f27, f5, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 992(x5)\n"
			"fcvt.ps.pw f19, f28, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f24, 960(x25)\n"
			"fsrai.pi f15, f24, 0x5\n" 
			"mova.x.m x14\n" 
			"li x5, 0x7b6c\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f28, 352(x20)\n"
			"flw.ps f0, 192(x20)\n"
			"fsub.pi f5, f28, f0\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 992(x20)\n"
			"flw.ps f25, 544(x20)\n"
			"fadd.pi f19, f27, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 704(x27)\n"
			"flw.ps f4, 960(x27)\n"
			"fmax.ps f20, f23, f4\n" 
			"li x5, 0x8d8d\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_4:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 896(x25)\n"
			"fexp.ps f23, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 896(x14)\n"
			"flw.ps f20, 320(x14)\n"
			"flw.ps f27, 256(x14)\n"
			"fcmov.ps f23, f24, f20, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"fltm.pi m7, f5, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 448(x7)\n"
			"ffrc.ps f19, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 896(x11)\n"
			"fclass.ps f4, f0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f4, 704(x25)\n"
			"fsatu8.pi f20, f4\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f23, 800(x27)\n"
			"fcvt.ps.f16 f21, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_474404_HID_4:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x82 << TENSOR_IMA_SCP_LOC_B) | (0xb9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"maskpopcz x27, m1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 352(x11)\n"
			"flw.ps f27, 96(x11)\n"
			"flw.ps f13, 928(x11)\n"
			"fmadd.ps f20, f21, f27, f13, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 544(x23)\n"
			"flw.ps f25, 128(x23)\n"
			"fsgnjx.ps f21, f15, f25\n" 
			"fbci.pi f7, 0x51bad\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 64(x23)\n"
			"flw.ps f19, 32(x23)\n"
			"fmul.ps f7, f4, f19, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"feqm.ps m1, f4, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 736(x14)\n"
			"flw.ps f13, 352(x14)\n"
			"flw.ps f12, 480(x14)\n"
			"fmadd.s f19, f0, f13, f12, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 640(x9)\n"
			"fclass.ps f13, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 928(x9)\n"
			"flw.ps f28, 416(x9)\n"
			"fmin.ps f9, f12, f28\n" 
			"maskpopc x11, m0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 608(x11)\n"
			"flw.ps f7, 352(x11)\n"
			"fdiv.pi f19, f5, f7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_4:\n"
			"fltm.pi m6, f9, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 672(x18)\n"
			"flw.ps f4, 384(x18)\n"
			"fsgnjn.ps f7, f20, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 768(x14)\n"
			"frsq.ps f25, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 256(x14)\n"
			"fcvt.ps.pw f15, f27, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 256(x7)\n"
			"flw.ps f13, 32(x7)\n"
			"fmin.s f24, f24, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_76820_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 224(x20)\n"
			"flw.ps f13, 896(x20)\n"
			"fmin.ps f19, f27, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 384(x23)\n"
			"flw.ps f0, 64(x23)\n"
			"fsrl.pi f23, f13, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 928(x28)\n"
			"flw.ps f5, 736(x28)\n"
			"fsgnjx.s f23, f4, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 256(x14)\n"
			"flw.ps f25, 320(x14)\n"
			"fsgnjx.s f28, f4, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_4:\n"
			"masknot m0, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x27, m7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 640(x20)\n"
			"fswizz.ps f9, f0, 0x1c\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 704(x11)\n"
			"fsrli.pi f13, f25, 0x7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 352(x9)\n"
			"flw.ps f19, 160(x9)\n"
			"flt.ps f13, f28, f19\n" 
			"mova.x.m x11\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 608(x27)\n"
			"fmulh.pi f25, f13, f13\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 256(x11)\n"
			"flw.ps f27, 736(x11)\n"
			"fmul.pi f24, f15, f27\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 800(x9)\n"
			"flw.ps f23, 384(x9)\n"
			"flt.pi f7, f20, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 448(x14)\n"
			"flw.ps f5, 768(x14)\n"
			"fdiv.ps f23, f19, f5, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_4:\n"
			"maskand m1, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x7bd148413516d642\n"
			"fcvt.s.wu f23, x5, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
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
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_5\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 352(x17)\n"
			"flw.ps f8, 64(x17)\n"
			"fsub.ps f12, f3, f8, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 128(x25)\n"
			"flw.ps f28, 736(x25)\n"
			"fsgnjn.s f13, f8, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 800(x12)\n"
			"flw.ps f10, 416(x12)\n"
			"fadd.s f17, f31, f10, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 64(x7)\n"
			"flw.ps f5, 192(x7)\n"
			"flt.ps f8, f14, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 352(x22)\n"
			"fsqrt.ps f13, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 672(x9)\n"
			"fround.ps f29, f18, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 320(x7)\n"
			"fsrai.pi f16, f4, 0x6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 608(x7)\n"
			"flw.ps f10, 576(x7)\n"
			"fsgnjx.ps f3, f17, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 128(x17)\n"
			"fclass.s x7, f31\n" 
			"la x25, rand_int_32\n"
			"flw.ps f21, 352(x25)\n"
			"fnot.pi f21, f21\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 800(x25)\n"
			"flw.ps f13, 384(x25)\n"
			"flw.ps f3, 960(x25)\n"
			"fmsub.ps f18, f4, f13, f3, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 0(x7)\n"
			"fround.ps f3, f5, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 448(x11)\n"
			"flw.ps f28, 672(x11)\n"
			"fsra.pi f28, f10, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 352(x5)\n"
			"flw.ps f1, 320(x5)\n"
			"fmul.pi f16, f8, f1\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 96(x5)\n"
			"fpackrepb.pi f1, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"flem.ps m5, f13, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_5:\n"
			"maskand m5, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 704(x7)\n"
			"flw.ps f28, 160(x7)\n"
			"feq.s x25, f10, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 928(x5)\n"
			"fsin.ps f14, f17\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 896(x5)\n"
			"flw.ps f12, 288(x5)\n"
			"for.pi f16, f21, f12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 832(x11)\n"
			"fnot.pi f16, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 96(x17)\n"
			"fadd.pi f29, f5, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 32(x17)\n"
			"flw.ps f12, 832(x17)\n"
			"fmin.ps f17, f28, f12\n" 
			"fbci.pi f18, 0x2763d\n" 
			"fbci.pi f18, 0x4d225\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 128(x22)\n"
			"flw.ps f29, 960(x22)\n"
			"frem.pi f12, f14, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"masknot m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 736(x9)\n"
			"fclass.ps f1, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 288(x5)\n"
			"flw.ps f10, 832(x5)\n"
			"fmul.pi f21, f18, f10\n" 
			"la x13, rand_int_32\n"
			"flw.ps f8, 736(x13)\n"
			"flw.ps f17, 800(x13)\n"
			"fltu.pi f18, f8, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"mov.m.x m4, x12, 0x4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f16, 736(x22)\n"
			"flw.ps f12, 512(x22)\n"
			"fltu.pi f5, f16, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 352(x13)\n"
			"flw.ps f31, 448(x13)\n"
			"fmin.s f14, f29, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_5:\n"
			"fltm.ps m4, f21, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x12, 0x2a930bdcc50b5164\n"
			"fcvt.s.wu f13, x12, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_5:\n"
			"fltm.pi m2, f13, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f21, 416(x22)\n"
			"fcvt.ps.pwu f17, f21, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 928(x6)\n"
			"fsqrt.ps f31, f3\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 0(x13)\n"
			"faddi.pi f13, f21, 0x59\n" 
			"maskpopc x22, m5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 864(x5)\n"
			"flw.ps f31, 992(x5)\n"
			"fmsub.ps f5, f29, f29, f31, rdn\n" 
			"maskpopcz x12, m5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_5:\n"
			"maskxor m5, m4, m2\n" 
			"li x5, 0x80340120\n"
			"ld x5, 0(x5)\n"
			"la x25, rand_int_32\n"
			"flw.ps f31, 192(x25)\n"
			"flw.ps f21, 512(x25)\n"
			"fand.pi f21, f31, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 608(x6)\n"
			"frsq.ps f18, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 32(x12)\n"
			"fcvt.f16.ps f13, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 448(x6)\n"
			"flw.ps f13, 448(x6)\n"
			"fle.s x9, f12, f13\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 224(x6)\n"
			"frsq.ps f21, f5\n" 
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 448(x17)\n"
			"flw.ps f29, 768(x17)\n"
			"fsgnjn.ps f18, f31, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_5:\n"
			"fsetm.pi m6, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_5:\n"
			"fltm.ps m5, f31, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 160(x5)\n"
			"flw.ps f13, 800(x5)\n"
			"flw.ps f17, 672(x5)\n"
			"fmadd.s f16, f14, f13, f17, rtz\n" 
			"maskpopcz x11, m6\n" 
			"la x17, rand_int_32\n"
			"flw.ps f4, 992(x17)\n"
			"flw.ps f5, 288(x17)\n"
			"fsll.pi f1, f4, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 960(x7)\n"
			"fmax.ps f8, f21, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 448(x17)\n"
			"flw.ps f18, 96(x17)\n"
			"fadd.pi f28, f1, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_5:\n"
			"feqm.ps m4, f8, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 512(x7)\n"
			"flw.ps f16, 480(x7)\n"
			"fmulh.pi f5, f17, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 544(x7)\n"
			"fcvt.wu.s x5, f13, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 224(x17)\n"
			"flw.ps f28, 224(x17)\n"
			"fadd.s f13, f29, f28, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_5:\n"
			"flem.ps m4, f10, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f16, 160(x25)\n"
			"faddi.pi f8, f16, 0x152\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 832(x12)\n"
			"flw.ps f5, 160(x12)\n"
			"fsgnjx.ps f8, f16, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 768(x9)\n"
			"flw.ps f3, 640(x9)\n"
			"fand.pi f8, f12, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_6:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 768(x7)\n"
			"fexp.ps f6, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"maskor m2, m1, m0\n" 
			"li x5, 0x803401B8\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_int_32\n"
			"flw.ps f26, 864(x7)\n"
			"flw.ps f10, 896(x7)\n"
			"fmax.pi f7, f26, f10\n" 
			"mova.x.m x10\n" 
			"la x27, rand_int_32\n"
			"flw.ps f6, 704(x27)\n"
			"flw.ps f12, 128(x27)\n"
			"fdiv.pi f24, f6, f12\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 416(x17)\n"
			"frsq.ps f12, f7\n" 
			"maskpopcz x22, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 672(x23)\n"
			"flw.ps f24, 576(x23)\n"
			"flw.ps f3, 288(x23)\n"
			"fnmadd.ps f3, f26, f24, f3, rdn\n" 
			"fbci.ps f30, 0x1bb3e\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 32(x18)\n"
			"flw.ps f12, 192(x18)\n"
			"flw.ps f23, 320(x18)\n"
			"fmsub.ps f0, f26, f12, f23, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"fltm.ps m0, f2, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x18, m2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 384(x23)\n"
			"fclass.s x10, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 384(x27)\n"
			"fcvt.pwu.ps f23, f3, rtz\n" 
			"li x5, 0xcbe2\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_6:\n"
			"fltm.ps m5, f26, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_4_HID_6:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 544(x11)\n"
			"flog.ps f2, f24\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x3\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f0, 736(x11)\n"
			"flw.ps f7, 896(x11)\n"
			"fxor.pi f24, f0, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 64(x22)\n"
			"flw.ps f7, 0(x22)\n"
			"fnmadd.ps f1, f23, f23, f7, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 288(x10)\n"
			"flw.ps f2, 544(x10)\n"
			"fxor.pi f30, f15, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 640(x10)\n"
			"fsqrt.ps f6, f23\n" 
			"li x17, 0x3ed146f36671019f\n"
			"fcvt.s.w f2, x17, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 608(x16)\n"
			"flw.ps f22, 800(x16)\n"
			"fadd.s f30, f2, f22, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 128(x11)\n"
			"fltu.pi f27, f27, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 192(x18)\n"
			"flw.ps f22, 640(x18)\n"
			"fadd.ps f28, f28, f22, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 704(x27)\n"
			"flw.ps f3, 704(x27)\n"
			"fsra.pi f26, f10, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"flem.ps m2, f12, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 832(x18)\n"
			"flw.ps f7, 896(x18)\n"
			"fadd.pi f30, f1, f7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 960(x10)\n"
			"flw.ps f24, 608(x10)\n"
			"fsrl.pi f1, f30, f24\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 704(x23)\n"
			"fcvt.ps.pw f15, f30, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 960(x17)\n"
			"fsin.ps f10, f28\n" 
			"la x18, rand_int_32\n"
			"flw.ps f3, 96(x18)\n"
			"flw.ps f2, 992(x18)\n"
			"fminu.pi f15, f3, f2\n" 
			"fbci.ps f2, 0x2f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x13, 0x18000000001e0000\n"
			"csrr x6, mhartid\n"
			"srli x6, x6, 4\n"
			"slli x6, x6, 6\n"
			"add x13, x13, x6\n"
			"csrw tensor_reduce, x13\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_371282_HID_6:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x87 << TENSOR_IMA_SCP_LOC_B) | (0x6f << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f6, 64(x17)\n"
			"faddi.pi f0, f6, 0x1cd\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 64(x23)\n"
			"fclass.ps f26, f7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 768(x23)\n"
			"flw.ps f22, 192(x23)\n"
			"fsub.ps f1, f15, f22, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 896(x17)\n"
			"fsin.ps f22, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 320(x22)\n"
			"flw.ps f2, 416(x22)\n"
			"fmax.s f7, f27, f2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 160(x16)\n"
			"fmulh.pi f7, f30, f30\n" 
			"fbci.pi f2, 0x374aa\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 864(x17)\n"
			"flw.ps f6, 704(x17)\n"
			"fmax.s f3, f27, f6\n" 
			"maskpopcz x17, m5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"fltm.ps m2, f6, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_6:\n"
			"fltm.pi m0, f2, f15\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f26, 512(x13)\n"
			"fandi.pi f26, f26, 0x1cb\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 512(x13)\n"
			"flw.ps f0, 992(x13)\n"
			"fmul.pi f2, f28, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_6:\n"
			"mov.m.x m2, x16, 0x75\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_16\n"
			"flw.ps f7, 576(x17)\n"
			"fcvt.ps.f16 f22, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 640(x22)\n"
			"fclass.ps f30, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_380824_HID_6:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 416(x16)\n"
			"fclass.ps f0, f12\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 704(x13)\n"
			"flw.ps f27, 864(x13)\n"
			"fmul.pi f28, f24, f27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 480(x16)\n"
			"fandi.pi f6, f1, 0x1bf\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 32(x10)\n"
			"fcvt.w.s x12, f2, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f1, 160(x13)\n"
			"flw.ps f24, 448(x13)\n"
			"fltu.pi f30, f1, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_6:\n"
			"fltm.pi m0, f12, f27\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 672(x22)\n"
			"ffrc.ps f0, f26\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 896(x23)\n"
			"flw.ps f30, 512(x23)\n"
			"fmin.s f28, f1, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 512(x12)\n"
			"flw.ps f2, 160(x12)\n"
			"fcmovm.ps f24, f23, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 480(x17)\n"
			"fsub.s f1, f6, f6, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 608(x16)\n"
			"fsrai.pi f28, f1, 0x2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 896(x13)\n"
			"flw.ps f6, 608(x13)\n"
			"fsgnjx.ps f10, f12, f6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_6:\n"
			"maskand m2, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f27, 736(x18)\n"
			"fcvt.ps.pwu f2, f27, rtz\n" 
			"li x5, 0x4ffc\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_int_32\n"
			"flw.ps f23, 352(x7)\n"
			"flw.ps f12, 0(x7)\n"
			"fminu.pi f26, f23, f12\n" 
			"LBL_SEQID_3_FP_TRANS_ID_11_HID_6:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 960(x22)\n"
			"frcp.ps f30, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 0(x15)\n"
			"flw.ps f17, 224(x15)\n"
			"fmin.ps f23, f11, f17\n" 
			"la x23, rand_int_32\n"
			"flw.ps f23, 672(x23)\n"
			"flw.ps f21, 320(x23)\n"
			"fmin.pi f3, f23, f21\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 384(x9)\n"
			"flw.ps f4, 512(x9)\n"
			"fnmadd.ps f11, f3, f4, f4, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f5, 32(x25)\n"
			"flw.ps f21, 960(x25)\n"
			"fsll.pi f27, f5, f21\n" 
			"li x27, 0xd4b9206731288c93\n"
			"fcvt.s.w f5, x27, rtz\n" 
			"fbci.pi f13, 0x4cb34\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 736(x15)\n"
			"flw.ps f4, 352(x15)\n"
			"fmin.s f20, f3, f4\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 128(x18)\n"
			"flw.ps f23, 384(x18)\n"
			"feq.pi f10, f5, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 0(x9)\n"
			"fround.ps f2, f4, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 928(x20)\n"
			"faddi.pi f23, f11, 0x186\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 224(x9)\n"
			"flw.ps f23, 608(x9)\n"
			"fadd.pi f5, f29, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 832(x27)\n"
			"frsq.ps f17, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 320(x16)\n"
			"flw.ps f3, 992(x16)\n"
			"flw.ps f4, 896(x16)\n"
			"fnmsub.s f3, f10, f3, f4, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 192(x20)\n"
			"fexp.ps f3, f17\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 256(x13)\n"
			"fclass.s x20, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 128(x9)\n"
			"flw.ps f5, 512(x9)\n"
			"fle.ps f29, f13, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_7:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 672(x18)\n"
			"flw.ps f24, 512(x18)\n"
			"flw.ps f11, 352(x18)\n"
			"fnmadd.ps f17, f27, f24, f11, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 416(x25)\n"
			"fsrli.pi f10, f10, 0x2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 768(x15)\n"
			"fcmovm.ps f29, f10, f10\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 0(x27)\n"
			"flw.ps f10, 288(x27)\n"
			"flw.ps f29, 192(x27)\n"
			"fmadd.s f2, f31, f10, f29, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_7:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 544(x25)\n"
			"fexp.ps f20, f13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 544(x5)\n"
			"flw.ps f10, 0(x5)\n"
			"fmin.ps f26, f29, f10\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f2, 512(x22)\n"
			"fcvt.ps.f16 f29, f2\n" 
			"li x25, 0x5e0175216f821665\n"
			"fcvt.s.w f23, x25, rdn\n" 
			"fbci.pi f21, 0x1946c\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 320(x5)\n"
			"fcvt.ps.pwu f20, f11, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 512(x9)\n"
			"fsrli.pi f31, f26, 0x7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 448(x20)\n"
			"fcvt.pwu.ps f17, f31, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f5, 480(x20)\n"
			"fsrli.pi f2, f5, 0xa\n" 
			"mova.x.m x23\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 384(x16)\n"
			"fclass.ps f2, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 320(x25)\n"
			"flw.ps f2, 640(x25)\n"
			"fmin.s f3, f4, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 576(x25)\n"
			"flw.ps f2, 288(x25)\n"
			"fsgnjn.ps f17, f10, f2\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 64(x27)\n"
			"fcvt.ps.pw f24, f13, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f13, 896(x15)\n"
			"flw.ps f31, 896(x15)\n"
			"feq.pi f23, f13, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 0(x20)\n"
			"fsqrt.ps f29, f5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 608(x13)\n"
			"flw.ps f23, 544(x13)\n"
			"fmax.s f4, f13, f23\n" 
			"fbci.ps f4, 0x3fbd5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 608(x20)\n"
			"fswizz.ps f29, f24, 0x1a\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 288(x20)\n"
			"frsq.ps f13, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 576(x23)\n"
			"flw.ps f21, 896(x23)\n"
			"fmax.pi f17, f11, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 672(x20)\n"
			"flw.ps f24, 576(x20)\n"
			"flt.pi f31, f23, f24\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_7:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 480(x15)\n"
			"frcp.ps f23, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_7:\n"
			"maskor m6, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_7:\n"
			"flem.ps m4, f13, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f17, 704(x5)\n"
			"fsrli.pi f3, f17, 0x9\n" 
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 0(x18)\n"
			"flw.ps f5, 224(x18)\n"
			"fle.s x18, f21, f5\n" 
			"fbci.ps f2, 0x387e0\n" 
			"la x16, rand_int_32\n"
			"flw.ps f29, 800(x16)\n"
			"fnot.pi f20, f29\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 896(x20)\n"
			"flw.ps f3, 800(x20)\n"
			"fmul.s f3, f4, f3, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 160(x22)\n"
			"flw.ps f17, 928(x22)\n"
			"fmax.s f31, f10, f17\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 832(x23)\n"
			"flw.ps f27, 512(x23)\n"
			"fminu.pi f24, f5, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 352(x27)\n"
			"flw.ps f20, 512(x27)\n"
			"fmsub.s f20, f21, f21, f20, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 608(x20)\n"
			"flw.ps f17, 768(x20)\n"
			"fle.ps f31, f13, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 384(x18)\n"
			"flw.ps f13, 768(x18)\n"
			"fle.s x22, f31, f13\n" 
			"li x20, 0x69a5fdad191754ac\n"
			"fcvt.s.w f27, x20, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 288(x15)\n"
			"flw.ps f27, 576(x15)\n"
			"feq.s x25, f24, f27\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_7:\n"
			"feqm.ps m7, f2, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 672(x5)\n"
			"flw.ps f13, 672(x5)\n"
			"flw.ps f5, 224(x5)\n"
			"fnmadd.s f10, f29, f13, f5, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 896(x9)\n"
			"flw.ps f2, 320(x9)\n"
			"fle.s x22, f10, f2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_7:\n"
			"maskand m7, m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_7:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 256(x25)\n"
			"fexp.ps f17, f17\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"maskxor m0, m7, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc242fde86c\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x6\n" 
			"li x13, 0x5941ce7a58a95609\n"
			"fcvt.s.wu f6, x13, rup\n" 
			"li x5, 0xddb9\n"
			"csrw tensor_mask, x5\n"	
			"maskpopc x14, m2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 128(x19)\n"
			"flw.ps f11, 416(x19)\n"
			"fsgnjx.ps f5, f7, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 576(x13)\n"
			"fcvt.pwu.ps f23, f16, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 512(x13)\n"
			"flw.ps f18, 0(x13)\n"
			"fmul.ps f6, f21, f18, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 992(x25)\n"
			"flw.ps f21, 416(x25)\n"
			"fsgnjn.s f0, f26, f21\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 928(x5)\n"
			"fand.pi f6, f26, f26\n" 
			"la x14, rand_int_32\n"
			"flw.ps f16, 320(x14)\n"
			"flw.ps f26, 320(x14)\n"
			"fxor.pi f18, f16, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 800(x5)\n"
			"flw.ps f10, 896(x5)\n"
			"fmadd.ps f18, f15, f15, f10, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 0(x17)\n"
			"fsub.s f23, f5, f5, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"mov.m.x m0, x15, 0x59\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 960(x6)\n"
			"fsqrt.ps f15, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 32(x10)\n"
			"flw.ps f11, 896(x10)\n"
			"flw.ps f5, 864(x10)\n"
			"fmadd.ps f10, f10, f11, f5, rmm\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f18, 160(x6)\n"
			"fcvt.ps.f16 f6, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_364529_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xaf << TENSOR_FMA32_SCP_LOC_B) | (0xcd << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x23 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"fltm.ps m7, f0, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f18, 320(x25)\n"
			"fcvt.ps.pw f24, f18, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"maskor m2, m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 992(x25)\n"
			"flw.ps f24, 160(x25)\n"
			"fsgnj.s f10, f21, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 320(x6)\n"
			"flw.ps f16, 576(x6)\n"
			"fle.ps f2, f2, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 512(x6)\n"
			"flw.ps f21, 704(x6)\n"
			"fsgnjx.ps f5, f0, f21\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 704(x10)\n"
			"fmin.pi f5, f2, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 768(x12)\n"
			"fnot.pi f11, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 448(x10)\n"
			"fcvt.f16.ps f10, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 448(x12)\n"
			"flw.ps f24, 864(x12)\n"
			"fmul.pi f6, f11, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 256(x12)\n"
			"flw.ps f2, 256(x12)\n"
			"fadd.pi f16, f5, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 864(x25)\n"
			"fcvt.wu.s x15, f5, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"mov.m.x m2, x12, 0x65\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f0, 256(x13)\n"
			"flw.ps f7, 672(x13)\n"
			"fxor.pi f21, f0, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 416(x14)\n"
			"flw.ps f18, 512(x14)\n"
			"feq.s x23, f15, f18\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 384(x13)\n"
			"flw.ps f5, 0(x13)\n"
			"feq.s x19, f7, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_475935_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x3\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_692586_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_689891_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0x40000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 288(x17)\n"
			"flw.ps f5, 256(x17)\n"
			"fadd.s f7, f6, f5, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 480(x25)\n"
			"fround.ps f26, f9, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_8:\n"
			"fltm.pi m7, f6, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 544(x6)\n"
			"flw.ps f11, 544(x6)\n"
			"flw.ps f18, 768(x6)\n"
			"fnmsub.ps f6, f15, f11, f18, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 768(x17)\n"
			"flw.ps f23, 736(x17)\n"
			"fadd.ps f10, f22, f23, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 832(x12)\n"
			"fsrai.pi f7, f24, 0x2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_int_32\n"
			"flw.ps f18, 384(x19)\n"
			"flw.ps f10, 992(x19)\n"
			"fminu.pi f18, f18, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 800(x6)\n"
			"flw.ps f15, 352(x6)\n"
			"flw.ps f9, 256(x6)\n"
			"fnmadd.ps f0, f24, f15, f9, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 832(x12)\n"
			"fcvt.ps.pw f21, f26, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 704(x10)\n"
			"flw.ps f21, 736(x10)\n"
			"fsgnjx.ps f5, f10, f21\n" 
			"li x5, 0xeb3f\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f21, 960(x10)\n"
			"flw.ps f22, 512(x10)\n"
			"fltu.pi f24, f21, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 992(x13)\n"
			"fround.ps f26, f26, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f11, 352(x17)\n"
			"flw.ps f2, 128(x17)\n"
			"fsll.pi f11, f11, f2\n" 
			"maskpopc x19, m7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 576(x10)\n"
			"flw.ps f0, 224(x10)\n"
			"fmax.pi f2, f2, f0\n" 
			"la x17, rand_int_32\n"
			"flw.ps f23, 608(x17)\n"
			"flw.ps f10, 288(x17)\n"
			"fmulh.pi f2, f23, f10\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xab << TENSOR_FMA32_SCP_LOC_B) | (0x7c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_8:\n"
			"mov.m.x m3, x15, 0x1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f21, 864(x25)\n"
			"flw.ps f7, 192(x25)\n"
			"fremu.pi f7, f21, f7\n" 
			"li x5, 0xbad4\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f26, 320(x5)\n"
			"flw.ps f10, 128(x5)\n"
			"fmaxu.pi f6, f26, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 96(x15)\n"
			"flw.ps f2, 704(x15)\n"
			"fand.pi f2, f9, f2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 896(x6)\n"
			"flw.ps f0, 480(x6)\n"
			"fsll.pi f15, f5, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 0(x17)\n"
			"fclass.ps f24, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 32(x6)\n"
			"fround.ps f18, f11, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 704(x17)\n"
			"fcvt.ps.pwu f21, f11, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 160(x19)\n"
			"fcvt.pw.ps f16, f22, rne\n" 
			"mova.x.m x13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 352(x10)\n"
			"fslli.pi f18, f6, 0xe\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 704(x19)\n"
			"flw.ps f18, 0(x19)\n"
			"fmin.s f22, f6, f18\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 448(x25)\n"
			"flw.ps f11, 896(x25)\n"
			"fmax.s f6, f16, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 768(x17)\n"
			"fsin.ps f0, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 96(x5)\n"
			"flw.ps f9, 192(x5)\n"
			"feq.pi f18, f18, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 480(x6)\n"
			"flw.ps f18, 800(x6)\n"
			"flw.ps f15, 192(x6)\n"
			"fnmsub.s f10, f10, f18, f15, rmm\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_311554_HID_8:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x179e8a3f5f8250ab\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb36d8ccf9385721d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb9 << TENSOR_FMA32_SCP_LOC_B) | (0x27 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 992(x16)\n"
			"flw.ps f14, 544(x16)\n"
			"fsgnjn.ps f6, f1, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 480(x9)\n"
			"flw.ps f8, 32(x9)\n"
			"fminu.pi f7, f7, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 64(x20)\n"
			"fcvt.wu.s x14, f3, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 32(x9)\n"
			"fcvt.f16.ps f8, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 352(x20)\n"
			"fcvt.ps.pwu f7, f0, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 128(x14)\n"
			"fclass.s x15, f0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 384(x28)\n"
			"flw.ps f2, 896(x28)\n"
			"fsll.pi f11, f0, f2\n" 
			"fbci.pi f23, 0x275eb\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 544(x18)\n"
			"fcvt.w.s x18, f2, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f23, 416(x14)\n"
			"fandi.pi f6, f23, 0x15b\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 160(x18)\n"
			"flw.ps f1, 32(x18)\n"
			"fsgnjn.ps f11, f6, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"maskand m0, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f11, 832(x28)\n"
			"fcvt.ps.pw f22, f11, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_9:\n"
			"flem.ps m4, f0, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f4, 64(x9)\n"
			"flw.ps f20, 832(x9)\n"
			"fmin.pi f8, f4, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 992(x14)\n"
			"flw.ps f3, 32(x14)\n"
			"fmin.pi f11, f0, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f6, 768(x18)\n"
			"flw.ps f0, 608(x18)\n"
			"flt.pi f0, f6, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"fltm.pi m0, f22, f20\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 800(x15)\n"
			"flw.ps f11, 256(x15)\n"
			"fmax.ps f14, f14, f11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 672(x18)\n"
			"fpackrepb.pi f14, f8\n" 
			"maskpopcz x12, m4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_9:\n"
			"mov.m.x m0, x20, 0x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x12, m1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 896(x14)\n"
			"flw.ps f1, 480(x14)\n"
			"for.pi f0, f2, f1\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 192(x16)\n"
			"faddi.pi f2, f1, 0xa4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 288(x12)\n"
			"fcvt.ps.pw f3, f0, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_9:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 96(x13)\n"
			"fexp.ps f1, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 0(x9)\n"
			"flw.ps f4, 928(x9)\n"
			"fsub.ps f20, f1, f4, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 416(x15)\n"
			"flw.ps f4, 768(x15)\n"
			"flw.ps f23, 960(x15)\n"
			"fnmadd.ps f23, f9, f4, f23, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 128(x28)\n"
			"fsatu8.pi f9, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"fbci.ps f23, 0x46d7d\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 864(x28)\n"
			"flw.ps f14, 832(x28)\n"
			"fsra.pi f14, f2, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f0, 288(x18)\n"
			"flw.ps f23, 800(x18)\n"
			"fsrl.pi f14, f0, f23\n" 
			"li x20, 0xa6b7965fbe83274d\n"
			"fcvt.s.wu f22, x20, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 96(x17)\n"
			"fnot.pi f2, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 320(x28)\n"
			"flw.ps f22, 768(x28)\n"
			"fremu.pi f22, f7, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 288(x16)\n"
			"fsrli.pi f2, f10, 0x9\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f7, 608(x17)\n"
			"fcvt.ps.f16 f14, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 736(x16)\n"
			"fsin.ps f8, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 160(x9)\n"
			"fcvt.wu.s x20, f0, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 160(x13)\n"
			"fround.ps f1, f10, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 416(x28)\n"
			"flw.ps f9, 800(x28)\n"
			"flw.ps f7, 96(x28)\n"
			"fmadd.ps f3, f8, f9, f7, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 448(x12)\n"
			"flw.ps f8, 608(x12)\n"
			"fltu.pi f10, f9, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 832(x14)\n"
			"flw.ps f24, 640(x14)\n"
			"fmin.ps f2, f22, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 768(x14)\n"
			"flw.ps f8, 832(x14)\n"
			"fsub.pi f6, f9, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 640(x28)\n"
			"fsrli.pi f1, f10, 0x8\n" 
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
			"flw.ps f4, 544(x17)\n"
			"flw.ps f20, 992(x17)\n"
			"fand.pi f20, f4, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 576(x20)\n"
			"flw.ps f20, 864(x20)\n"
			"fsub.ps f10, f0, f20, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 896(x12)\n"
			"flw.ps f2, 864(x12)\n"
			"fmulhu.pi f8, f4, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 672(x12)\n"
			"flw.ps f20, 384(x12)\n"
			"fsrl.pi f4, f11, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 736(x13)\n"
			"flw.ps f14, 416(x13)\n"
			"fle.pi f14, f3, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 512(x14)\n"
			"flw.ps f4, 128(x14)\n"
			"fmax.pi f8, f7, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 640(x12)\n"
			"ffrc.ps f11, f3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 704(x28)\n"
			"fcvt.ps.pwu f11, f7, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 160(x28)\n"
			"flw.ps f20, 512(x28)\n"
			"fsub.s f1, f14, f20, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 704(x18)\n"
			"flw.ps f7, 992(x18)\n"
			"flw.ps f0, 160(x18)\n"
			"fnmadd.ps f11, f11, f7, f0, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 768(x9)\n"
			"flw.ps f10, 32(x9)\n"
			"fsub.pi f9, f23, f10\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 416(x12)\n"
			"flw.ps f22, 640(x12)\n"
			"feq.pi f8, f10, f22\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 192(x9)\n"
			"fcvt.wu.s x14, f1, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 0(x15)\n"
			"flw.ps f7, 96(x15)\n"
			"fle.s x14, f1, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 0(x16)\n"
			"fnot.pi f10, f1\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 864(x13)\n"
			"fpackrepb.pi f22, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x26, rand_int_32\n"
			"flw.ps f9, 480(x26)\n"
			"flw.ps f2, 992(x26)\n"
			"fmul.pi f3, f9, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 736(x23)\n"
			"fclass.ps f0, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fsetm.pi m2, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 64(x17)\n"
			"flw.ps f2, 640(x17)\n"
			"fmul.s f24, f4, f2, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 704(x14)\n"
			"fcvt.ps.pwu f0, f13, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 544(x23)\n"
			"flw.ps f1, 544(x23)\n"
			"flw.ps f22, 128(x23)\n"
			"fmadd.ps f9, f25, f1, f22, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 256(x26)\n"
			"fnot.pi f22, f2\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 768(x11)\n"
			"flw.ps f2, 448(x11)\n"
			"fdiv.pi f0, f25, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"mov.m.x m6, x11, 0x57\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 608(x17)\n"
			"flw.ps f7, 864(x17)\n"
			"fmul.s f2, f19, f7, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 224(x19)\n"
			"fpackreph.pi f24, f0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 864(x16)\n"
			"flw.ps f25, 96(x16)\n"
			"fmin.ps f9, f7, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 768(x20)\n"
			"flw.ps f25, 480(x20)\n"
			"fle.ps f0, f19, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 352(x26)\n"
			"flw.ps f7, 512(x26)\n"
			"fmax.s f19, f24, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"flem.ps m6, f2, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f2, 384(x22)\n"
			"faddi.pi f25, f2, 0x11b\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_757987_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_420575_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 64(x14)\n"
			"flw.ps f9, 32(x14)\n"
			"fsub.s f9, f3, f9, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 704(x20)\n"
			"fcvt.wu.s x20, f21, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 800(x17)\n"
			"fsin.ps f13, f22\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 352(x19)\n"
			"fclass.s x26, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"mov.m.x m4, x14, 0x72\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 960(x16)\n"
			"flw.ps f7, 832(x16)\n"
			"fmin.s f0, f2, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 416(x22)\n"
			"flw.ps f17, 0(x22)\n"
			"fadd.pi f29, f7, f17\n" 
			"la x19, rand_int_32\n"
			"flw.ps f1, 352(x19)\n"
			"fcvt.ps.pwu f25, f1, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 512(x23)\n"
			"fcvt.w.s x11, f21, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 672(x16)\n"
			"flw.ps f14, 320(x16)\n"
			"feq.ps f22, f24, f14\n" 
			"li x5, 0x88d1\n"
			"csrw tensor_mask, x5\n"	
			"maskpopcz x11, m6\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_10:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 928(x22)\n"
			"flog.ps f22, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f7, 928(x17)\n"
			"fpackreph.pi f29, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 736(x14)\n"
			"fslli.pi f3, f1, 0x7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 608(x11)\n"
			"fcvt.w.s x11, f14, rdn\n" 
			"fbci.pi f29, 0x6d8f7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_87713_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
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
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x2 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xfc << TENSOR_FMA16_SCP_LOC_B) | (0xed << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 512(x16)\n"
			"flw.ps f9, 960(x16)\n"
			"flw.ps f2, 128(x16)\n"
			"fnmadd.ps f0, f0, f9, f2, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 320(x17)\n"
			"flw.ps f3, 352(x17)\n"
			"flt.ps f1, f7, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 480(x26)\n"
			"flw.ps f21, 96(x26)\n"
			"fadd.s f19, f1, f21, rne\n" 
			"li x23, 0xb27203019093d100\n"
			"fcvt.s.w f4, x23, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_10:\n"
			"maskor m7, m7, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd82a726f64\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 704(x16)\n"
			"frsq.ps f22, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 416(x22)\n"
			"flw.ps f9, 224(x22)\n"
			"flw.ps f2, 320(x22)\n"
			"fcmov.ps f1, f24, f9, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 672(x17)\n"
			"fcvt.wu.s x19, f21, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 448(x23)\n"
			"flw.ps f19, 768(x23)\n"
			"fadd.pi f21, f0, f19\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 352(x14)\n"
			"faddi.pi f19, f9, 0x9d\n" 
			"li x5, 0x5a5d\n"
			"csrw tensor_mask, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f2, 256(x14)\n"
			"flw.ps f21, 960(x14)\n"
			"fand.pi f1, f2, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 768(x14)\n"
			"flw.ps f13, 928(x14)\n"
			"feq.ps f19, f9, f13\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 384(x17)\n"
			"fandi.pi f24, f29, 0x179\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 928(x11)\n"
			"flw.ps f21, 256(x11)\n"
			"flw.ps f24, 928(x11)\n"
			"fmsub.ps f25, f17, f21, f24, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 32(x23)\n"
			"flw.ps f14, 800(x23)\n"
			"fltu.pi f19, f9, f14\n" 
			"maskpopc x22, m6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 96(x16)\n"
			"flw.ps f17, 608(x16)\n"
			"fle.s x6, f14, f17\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 928(x22)\n"
			"fpackreph.pi f0, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 512(x6)\n"
			"fslli.pi f19, f13, 0xa\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 192(x22)\n"
			"flw.ps f4, 320(x22)\n"
			"fremu.pi f3, f17, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 672(x14)\n"
			"flw.ps f29, 896(x14)\n"
			"fdiv.s f14, f2, f29, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 256(x22)\n"
			"fnot.pi f17, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_10:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 128(x22)\n"
			"fexp.ps f0, f29\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 896(x14)\n"
			"flw.ps f25, 256(x14)\n"
			"fmax.s f3, f21, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_10:\n"
			"maskxor m4, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 864(x26)\n"
			"fle.s x11, f3, f3\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_10:\n"
			"fltm.pi m7, f3, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f29, 352(x23)\n"
			"fsrai.pi f22, f29, 0xb\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_10:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 864(x14)\n"
			"frcp.ps f1, f21\n" 
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_10:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 864(x6)\n"
			"frcp.ps f14, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 224(x11)\n"
			"flw.ps f21, 64(x11)\n"
			"fsgnjx.ps f3, f3, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 192(x20)\n"
			"flw.ps f13, 448(x20)\n"
			"for.pi f9, f7, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x4525\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
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
			"LBL_KERNEL_HID_11:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_11\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"flw.ps f4, 544(x26)\n"
			"flw.ps f6, 608(x26)\n"
			"fmin.ps f11, f4, f6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 736(x14)\n"
			"fcvt.w.s x14, f0, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 672(x14)\n"
			"flw.ps f31, 128(x14)\n"
			"fmulh.pi f0, f6, f31\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 64(x28)\n"
			"flw.ps f30, 320(x28)\n"
			"fadd.pi f0, f24, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 864(x15)\n"
			"flw.ps f0, 480(x15)\n"
			"flw.ps f8, 960(x15)\n"
			"fmadd.s f11, f12, f0, f8, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"maskand m7, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskand m4, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 800(x28)\n"
			"flw.ps f19, 800(x28)\n"
			"flw.ps f6, 256(x28)\n"
			"fmadd.s f12, f9, f19, f6, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 864(x19)\n"
			"fclass.ps f29, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 96(x23)\n"
			"flw.ps f8, 800(x23)\n"
			"flw.ps f27, 832(x23)\n"
			"fnmsub.s f2, f9, f8, f27, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 192(x19)\n"
			"fsin.ps f8, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 800(x28)\n"
			"fsqrt.s f2, f31, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 608(x26)\n"
			"flw.ps f9, 768(x26)\n"
			"fsll.pi f11, f29, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 32(x9)\n"
			"flw.ps f8, 704(x9)\n"
			"fsgnjn.ps f27, f0, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 960(x20)\n"
			"flw.ps f24, 736(x20)\n"
			"fxor.pi f28, f6, f24\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 32(x23)\n"
			"flw.ps f31, 896(x23)\n"
			"frem.pi f27, f19, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 224(x20)\n"
			"fround.ps f9, f11, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 416(x19)\n"
			"flw.ps f31, 416(x19)\n"
			"fsra.pi f9, f14, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 0(x5)\n"
			"fsrli.pi f9, f9, 0xa\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 608(x6)\n"
			"flw.ps f2, 416(x6)\n"
			"feq.pi f8, f31, f2\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f4, 864(x28)\n"
			"fcvt.ps.f16 f11, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 64(x14)\n"
			"flw.ps f8, 928(x14)\n"
			"fsub.pi f24, f6, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 640(x28)\n"
			"fclass.s x28, f12\n" 
			"maskpopcz x28, m5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 96(x26)\n"
			"flw.ps f11, 608(x26)\n"
			"fmaxu.pi f8, f2, f11\n" 
			"fbci.pi f24, 0x598ac\n" 
			"maskpopc x20, m7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_11:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 320(x9)\n"
			"frcp.ps f2, f28\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_11:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 640(x20)\n"
			"flog.ps f9, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 832(x15)\n"
			"fsatu8.pi f4, f4\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 448(x20)\n"
			"flw.ps f4, 32(x20)\n"
			"fmul.s f11, f11, f4, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f4, 256(x14)\n"
			"flw.ps f8, 256(x14)\n"
			"fmulhu.pi f0, f4, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f27, 256(x14)\n"
			"flw.ps f8, 64(x14)\n"
			"fle.pi f29, f27, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 64(x5)\n"
			"fcvt.ps.pwu f9, f14, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 384(x20)\n"
			"flw.ps f0, 928(x20)\n"
			"fsub.ps f27, f2, f0, rmm\n" 
			"li x19, 0xeb6c6f585215f8ff\n"
			"fcvt.s.wu f24, x19, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 288(x19)\n"
			"fnot.pi f28, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 416(x26)\n"
			"fclass.ps f2, f27\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 32(x20)\n"
			"frsq.ps f11, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 864(x23)\n"
			"flw.ps f8, 608(x23)\n"
			"fsgnj.s f4, f4, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 128(x5)\n"
			"fclass.ps f14, f11\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 768(x28)\n"
			"flw.ps f11, 672(x28)\n"
			"fminu.pi f0, f4, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 928(x6)\n"
			"flw.ps f31, 320(x6)\n"
			"fle.s x20, f30, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 704(x15)\n"
			"flw.ps f11, 608(x15)\n"
			"flw.ps f6, 896(x15)\n"
			"fnmadd.s f30, f29, f11, f6, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 320(x14)\n"
			"flw.ps f8, 192(x14)\n"
			"fle.ps f27, f12, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 704(x6)\n"
			"flw.ps f0, 736(x6)\n"
			"fsrl.pi f11, f9, f0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 320(x28)\n"
			"flw.ps f19, 480(x28)\n"
			"fremu.pi f31, f14, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 192(x28)\n"
			"flw.ps f24, 768(x28)\n"
			"fminu.pi f30, f29, f24\n" 
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
			"la x6, rand_int_32\n"
			"flw.ps f6, 64(x6)\n"
			"flw.ps f9, 544(x6)\n"
			"fmaxu.pi f2, f6, f9\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f28, 896(x28)\n"
			"fcvt.ps.f16 f6, f28\n" 
			"la x15, rand_int_32\n"
			"flw.ps f14, 224(x15)\n"
			"flw.ps f8, 672(x15)\n"
			"fadd.pi f28, f14, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_11:\n"
			"flem.ps m4, f9, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f12, 608(x26)\n"
			"faddi.pi f30, f12, 0x111\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 96(x14)\n"
			"flw.ps f27, 736(x14)\n"
			"fsgnjx.ps f29, f4, f27\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 480(x20)\n"
			"flw.ps f28, 672(x20)\n"
			"for.pi f2, f27, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 288(x5)\n"
			"flw.ps f30, 544(x5)\n"
			"fcmov.ps f19, f11, f30, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 864(x9)\n"
			"flw.ps f30, 384(x9)\n"
			"fminu.pi f0, f28, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 320(x15)\n"
			"flw.ps f30, 992(x15)\n"
			"fmul.ps f0, f24, f30, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 512(x19)\n"
			"flw.ps f29, 608(x19)\n"
			"fmin.ps f27, f27, f29\n" 
			"mova.x.m x20\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 160(x6)\n"
			"flw.ps f30, 224(x6)\n"
			"fmadd.ps f8, f0, f0, f30, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 704(x18)\n"
			"flw.ps f2, 416(x18)\n"
			"fmin.s f12, f9, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f4, 736(x14)\n"
			"fnot.pi f2, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 192(x18)\n"
			"flw.ps f6, 32(x18)\n"
			"flt.ps f29, f24, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"li x15, 0xd4601f32a0505865\n"
			"fcvt.s.w f30, x15, rup\n" 
			"maskpopcz x22, m7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 608(x22)\n"
			"flw.ps f20, 224(x22)\n"
			"fmulhu.pi f8, f16, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f30, 896(x7)\n"
			"flw.ps f20, 384(x7)\n"
			"fsra.pi f25, f30, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 480(x7)\n"
			"flw.ps f18, 32(x7)\n"
			"fnmadd.ps f26, f29, f29, f18, rup\n" 
			"li x5, 0x979c\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 64(x7)\n"
			"flw.ps f21, 64(x7)\n"
			"fle.ps f21, f1, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f28, 224(x22)\n"
			"fpackreph.pi f31, f28\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 672(x7)\n"
			"flw.ps f8, 864(x7)\n"
			"fmin.pi f1, f28, f8\n" 
			"li x5, 0x1f5a\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 448(x15)\n"
			"fround.ps f1, f30, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 0(x7)\n"
			"flw.ps f16, 32(x7)\n"
			"feq.s x5, f28, f16\n" 
			"la x15, rand_int_32\n"
			"flw.ps f30, 800(x15)\n"
			"flw.ps f21, 224(x15)\n"
			"fmaxu.pi f29, f30, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 352(x15)\n"
			"flw.ps f20, 544(x15)\n"
			"fsgnj.s f21, f26, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 704(x22)\n"
			"flw.ps f29, 64(x22)\n"
			"fsub.pi f22, f12, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 96(x11)\n"
			"flw.ps f31, 32(x11)\n"
			"fsgnjn.ps f1, f1, f31\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 160(x22)\n"
			"fcvt.ps.pw f30, f28, rmm\n" 
			"li x5, 0x903298d6092488a0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x90cb582de301b823\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x11, 0x777595c48e0f1ce3\n"
			"fcvt.s.lu f17, x11, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_262169_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 0x80000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
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
			"mova.x.m x25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 704(x5)\n"
			"fsatu8.pi f21, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 640(x7)\n"
			"fcvt.pwu.ps f8, f31, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 928(x17)\n"
			"flw.ps f16, 960(x17)\n"
			"fmaxu.pi f22, f22, f16\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 480(x14)\n"
			"fsrai.pi f16, f1, 0xa\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_12:\n"
			"flem.ps m6, f18, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f6, 32(x14)\n"
			"flw.ps f26, 480(x14)\n"
			"fminu.pi f31, f6, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_12:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 576(x5)\n"
			"fsin.ps f31, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 960(x5)\n"
			"flw.ps f6, 800(x5)\n"
			"fsub.ps f22, f16, f6, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_12:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 832(x6)\n"
			"flog.ps f29, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 416(x15)\n"
			"flw.ps f22, 928(x15)\n"
			"fcmovm.ps f16, f21, f22\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 32(x25)\n"
			"flw.ps f26, 128(x25)\n"
			"fsll.pi f16, f20, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"maskand m7, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f31, 928(x14)\n"
			"fcvt.ps.pwu f28, f31, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 96(x11)\n"
			"flw.ps f12, 384(x11)\n"
			"fmaxu.pi f30, f6, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_551536_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xc4 << TENSOR_FMA16_SCP_LOC_B) | (0x89 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x5, 0x1df5\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f8, 544(x10)\n"
			"fandi.pi f12, f8, 0x141\n" 
			"mova.x.m x7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 128(x10)\n"
			"flw.ps f6, 256(x10)\n"
			"flt.pi f21, f16, f6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 608(x14)\n"
			"fcvt.wu.s x15, f6, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 928(x14)\n"
			"flw.ps f30, 736(x14)\n"
			"fand.pi f29, f25, f30\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 224(x5)\n"
			"fpackreph.pi f22, f8\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_12:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 160(x15)\n"
			"flog.ps f8, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 864(x11)\n"
			"flw.ps f1, 896(x11)\n"
			"fsub.ps f22, f16, f1, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 672(x17)\n"
			"fcvt.w.s x5, f30, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 992(x5)\n"
			"fslli.pi f17, f29, 0x7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 320(x17)\n"
			"flw.ps f18, 32(x17)\n"
			"flw.ps f8, 96(x17)\n"
			"fmadd.ps f20, f20, f18, f8, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 512(x10)\n"
			"flw.ps f31, 832(x10)\n"
			"fmin.ps f12, f1, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 128(x5)\n"
			"fcvt.lu.s x7, f17, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 384(x5)\n"
			"flw.ps f12, 64(x5)\n"
			"fcmov.ps f29, f30, f12, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"fltm.pi m7, f29, f30\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x67543c2fab\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f16, 480(x15)\n"
			"flw.ps f20, 352(x15)\n"
			"fmaxu.pi f21, f16, f20\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_404970_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 608(x25)\n"
			"flw.ps f17, 288(x25)\n"
			"flw.ps f21, 352(x25)\n"
			"fmsub.ps f16, f28, f17, f21, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 256(x17)\n"
			"flw.ps f26, 768(x17)\n"
			"fmul.ps f29, f21, f26, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 992(x15)\n"
			"flw.ps f30, 96(x15)\n"
			"fsub.s f18, f29, f30, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 320(x22)\n"
			"flw.ps f22, 288(x22)\n"
			"feq.pi f1, f28, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 672(x5)\n"
			"fcvt.pwu.ps f30, f31, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 160(x14)\n"
			"fcvt.w.s x22, f1, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 544(x11)\n"
			"flw.ps f6, 992(x11)\n"
			"fsgnj.s f17, f18, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 768(x22)\n"
			"flw.ps f8, 960(x22)\n"
			"fsgnjn.ps f21, f16, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 640(x25)\n"
			"flw.ps f22, 576(x25)\n"
			"fsgnjx.s f12, f28, f22\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_12:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 800(x11)\n"
			"frcp.ps f12, f17\n" 
			"li x5, 0x386a\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_12:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 160(x17)\n"
			"flog.ps f18, f17\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_12:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 800(x15)\n"
			"frcp.ps f12, f29\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 928(x14)\n"
			"fcvt.l.s x6, f6, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 640(x5)\n"
			"flw.ps f28, 992(x5)\n"
			"feq.ps f6, f16, f28\n" 
			"maskpopcz x14, m6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 32(x14)\n"
			"flw.ps f8, 32(x14)\n"
			"fsgnj.s f29, f1, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 352(x5)\n"
			"fclass.s x22, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 896(x9)\n"
			"flw.ps f30, 32(x9)\n"
			"flt.pi f6, f21, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 544(x23)\n"
			"flw.ps f6, 576(x23)\n"
			"fcmovm.ps f27, f8, f6\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 320(x16)\n"
			"flw.ps f8, 224(x16)\n"
			"flt.s x14, f22, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 160(x14)\n"
			"flw.ps f27, 32(x14)\n"
			"flw.ps f19, 192(x14)\n"
			"fnmsub.ps f27, f22, f27, f19, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"fltm.pi m0, f30, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 832(x16)\n"
			"flw.ps f8, 896(x16)\n"
			"fadd.s f0, f28, f8, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 192(x18)\n"
			"fclass.ps f1, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 800(x5)\n"
			"flw.ps f13, 512(x5)\n"
			"fsgnj.ps f10, f1, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 320(x10)\n"
			"flw.ps f8, 512(x10)\n"
			"fmax.ps f19, f10, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f30, 704(x27)\n"
			"flw.ps f21, 608(x27)\n"
			"fmax.pi f1, f30, f21\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 736(x25)\n"
			"flw.ps f25, 320(x25)\n"
			"fand.pi f27, f18, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"fltm.ps m2, f19, f25\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 544(x23)\n"
			"flw.ps f21, 736(x23)\n"
			"fsgnjx.ps f0, f10, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 256(x9)\n"
			"flw.ps f27, 608(x9)\n"
			"fadd.pi f23, f18, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 256(x9)\n"
			"fcvt.pw.ps f10, f10, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fltm.pi m4, f0, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 128(x16)\n"
			"fcvt.f16.ps f0, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 480(x16)\n"
			"flw.ps f13, 384(x16)\n"
			"flw.ps f1, 640(x16)\n"
			"fnmadd.s f8, f27, f13, f1, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 64(x10)\n"
			"flw.ps f25, 256(x10)\n"
			"fmin.ps f13, f23, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 864(x25)\n"
			"flw.ps f18, 288(x25)\n"
			"flw.ps f19, 224(x25)\n"
			"fnmadd.s f30, f22, f18, f19, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f27, 288(x18)\n"
			"flw.ps f6, 256(x18)\n"
			"flt.pi f27, f27, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 608(x20)\n"
			"flw.ps f30, 896(x20)\n"
			"fmul.ps f23, f26, f30, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 736(x9)\n"
			"flw.ps f23, 928(x9)\n"
			"fsgnj.ps f25, f19, f23\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 160(x9)\n"
			"flw.ps f18, 864(x9)\n"
			"for.pi f26, f23, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"flem.ps m5, f18, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x74fd69be8d\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f26, 256(x20)\n"
			"flw.ps f23, 832(x20)\n"
			"fmin.pi f25, f26, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 736(x23)\n"
			"flw.ps f0, 32(x23)\n"
			"fsgnj.s f27, f21, f0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_13:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 608(x18)\n"
			"flog.ps f1, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 128(x5)\n"
			"fround.ps f10, f19, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 640(x5)\n"
			"flw.ps f13, 800(x5)\n"
			"fsgnjn.s f19, f28, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 64(x14)\n"
			"flw.ps f10, 288(x14)\n"
			"flw.ps f19, 320(x14)\n"
			"fcmov.ps f19, f25, f10, f19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"maskor m4, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"maskor m4, m0, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_13:\n"
			"maskor m4, m5, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 416(x18)\n"
			"flw.ps f8, 64(x18)\n"
			"flt.s x14, f21, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 480(x20)\n"
			"flw.ps f21, 192(x20)\n"
			"fmax.pi f19, f30, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 320(x23)\n"
			"flw.ps f23, 960(x23)\n"
			"fsub.ps f30, f0, f23, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 384(x22)\n"
			"fsgnjn.ps f25, f6, f6\n" 
			"maskpopcz x27, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 512(x14)\n"
			"flw.ps f30, 512(x14)\n"
			"fsub.ps f25, f6, f30, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 416(x9)\n"
			"flw.ps f8, 480(x9)\n"
			"flt.pi f26, f21, f8\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 192(x25)\n"
			"flw.ps f10, 704(x25)\n"
			"fadd.pi f28, f18, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 864(x10)\n"
			"flw.ps f13, 160(x10)\n"
			"fsgnjx.s f30, f25, f13\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 352(x27)\n"
			"flw.ps f23, 448(x27)\n"
			"fdiv.pi f25, f10, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 512(x18)\n"
			"flw.ps f28, 320(x18)\n"
			"fle.s x25, f30, f28\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 736(x9)\n"
			"fcvt.ps.pw f1, f26, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 320(x10)\n"
			"fcvt.ps.pwu f1, f6, rup\n" 
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
			"la x27, rand_int_32\n"
			"flw.ps f0, 64(x27)\n"
			"flw.ps f13, 384(x27)\n"
			"fremu.pi f23, f0, f13\n" 
			"la x27, rand_int_32\n"
			"flw.ps f27, 736(x27)\n"
			"fcvt.ps.pwu f13, f27, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 64(x16)\n"
			"flw.ps f23, 160(x16)\n"
			"fsub.ps f0, f28, f23, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_13:\n"
			"fltm.ps m4, f19, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f25, 928(x18)\n"
			"fcvt.ps.pw f22, f25, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_13:\n"
			"fltm.ps m2, f10, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_13:\n"
			"flem.ps m4, f21, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 384(x18)\n"
			"flw.ps f19, 64(x18)\n"
			"fmadd.ps f27, f8, f19, f8, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 64(x10)\n"
			"fsatu8.pi f22, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 640(x25)\n"
			"flw.ps f8, 160(x25)\n"
			"fsll.pi f19, f0, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_13:\n"
			"fltm.ps m5, f23, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f6, 416(x23)\n"
			"flw.ps f26, 512(x23)\n"
			"fsub.pi f19, f6, f26\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 128(x27)\n"
			"flw.ps f0, 384(x27)\n"
			"fmul.pi f26, f26, f0\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f19, 896(x9)\n"
			"fcvt.ps.f16 f23, f19\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_13:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_13:\n"
			"maskand m5, m5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 320(x23)\n"
			"fsin.ps f2, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 0(x19)\n"
			"flw.ps f6, 192(x19)\n"
			"fmin.s f28, f11, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 480(x20)\n"
			"fcvt.w.s x26, f28, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 448(x9)\n"
			"fclass.ps f23, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 160(x5)\n"
			"flw.ps f1, 64(x5)\n"
			"fmulh.pi f2, f18, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 832(x23)\n"
			"flw.ps f10, 736(x23)\n"
			"fsgnjx.s f6, f28, f10\n" 
			"maskpopcz x25, m2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 832(x9)\n"
			"faddi.pi f17, f2, 0x99\n" 
			"li x23, 0x5928813a1a2aefa9\n"
			"fcvt.s.w f28, x23, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 128(x23)\n"
			"flw.ps f18, 768(x23)\n"
			"fmin.s f2, f17, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 928(x23)\n"
			"fsat8.pi f10, f11\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 352(x23)\n"
			"frcp.ps f10, f15\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 736(x11)\n"
			"flw.ps f3, 128(x11)\n"
			"fmulhu.pi f16, f1, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 416(x11)\n"
			"flw.ps f17, 192(x11)\n"
			"fmin.pi f29, f30, f17\n" 
			"li x15, 0x90de3958cb33c140\n"
			"fcvt.s.w f10, x15, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 992(x11)\n"
			"fclass.ps f18, f30\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x25, rand_int_32\n"
			"flw.ps f25, 960(x25)\n"
			"flw.ps f16, 576(x25)\n"
			"fminu.pi f16, f25, f16\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 960(x18)\n"
			"flw.ps f6, 480(x18)\n"
			"feq.pi f15, f29, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 32(x9)\n"
			"flw.ps f24, 96(x9)\n"
			"fsgnjn.s f1, f25, f24\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 608(x19)\n"
			"fcvt.l.s x5, f1, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_14:\n"
			"flem.ps m2, f24, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5cc705d1ae\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 992(x15)\n"
			"fsin.ps f23, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"fltm.ps m6, f17, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 288(x15)\n"
			"flw.ps f30, 416(x15)\n"
			"fmax.ps f16, f1, f30\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 384(x18)\n"
			"flw.ps f18, 256(x18)\n"
			"flt.s x25, f1, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 960(x19)\n"
			"flw.ps f3, 160(x19)\n"
			"flw.ps f6, 0(x19)\n"
			"fnmsub.ps f25, f24, f3, f6, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 192(x11)\n"
			"fcvt.w.s x19, f18, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 864(x20)\n"
			"flw.ps f15, 896(x20)\n"
			"fsgnj.s f24, f30, f15\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 480(x19)\n"
			"flw.ps f28, 768(x19)\n"
			"fsgnj.s f23, f24, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"maskand m2, m3, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x56f82574da\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 288(x25)\n"
			"flw.ps f10, 288(x25)\n"
			"fmin.ps f1, f2, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 64(x23)\n"
			"faddi.pi f17, f11, 0x4c\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_818245_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 384(x19)\n"
			"flw.ps f24, 768(x19)\n"
			"fmul.s f10, f17, f24, rne\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_14:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 736(x20)\n"
			"flog.ps f11, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 544(x11)\n"
			"flw.ps f24, 320(x11)\n"
			"fltu.pi f25, f23, f24\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_14:\n"
			"maskor m3, m6, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 608(x15)\n"
			"ffrc.ps f23, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 512(x9)\n"
			"flw.ps f2, 928(x9)\n"
			"flw.ps f17, 800(x9)\n"
			"fmadd.ps f29, f18, f2, f17, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 704(x5)\n"
			"flw.ps f15, 0(x5)\n"
			"fsgnjx.ps f23, f30, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 608(x23)\n"
			"flw.ps f30, 608(x23)\n"
			"fmul.s f18, f2, f30, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 736(x15)\n"
			"flw.ps f18, 192(x15)\n"
			"fmul.ps f11, f24, f18, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 192(x9)\n"
			"flw.ps f10, 992(x9)\n"
			"fmul.s f15, f29, f10, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 672(x9)\n"
			"fsat8.pi f1, f10\n" 
			"li x5, 0xffc0\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f30, 0x12065\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 832(x20)\n"
			"fcvt.w.s x25, f3, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_14:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 608(x11)\n"
			"flog.ps f2, f29\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 960(x5)\n"
			"flw.ps f23, 224(x5)\n"
			"flw.ps f10, 576(x5)\n"
			"fmsub.ps f30, f11, f23, f10, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 992(x19)\n"
			"flw.ps f3, 320(x19)\n"
			"flt.ps f15, f10, f3\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_566570_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_871248_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 576(x25)\n"
			"frsq.ps f2, f11\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 736(x18)\n"
			"flw.ps f29, 800(x18)\n"
			"fsgnj.s f25, f17, f29\n" 
			"fbci.ps f15, 0x160a5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 480(x26)\n"
			"fcvt.pwu.ps f3, f1, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 128(x26)\n"
			"flw.ps f23, 256(x26)\n"
			"fadd.ps f11, f2, f23, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 768(x18)\n"
			"flw.ps f6, 416(x18)\n"
			"fadd.s f2, f29, f6, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 768(x5)\n"
			"fcvt.ps.pw f28, f11, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 544(x18)\n"
			"flw.ps f16, 224(x18)\n"
			"feq.s x20, f17, f16\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 768(x5)\n"
			"fsgnj.ps f29, f10, f10\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 512(x26)\n"
			"flw.ps f28, 416(x26)\n"
			"fminu.pi f29, f25, f28\n" 
			"maskpopc x20, m3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 960(x9)\n"
			"flw.ps f6, 416(x9)\n"
			"fsub.pi f24, f16, f6\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_14:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 160(x5)\n"
			"fexp.ps f28, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 32(x5)\n"
			"fswizz.ps f25, f2, 0xf\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_14:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 480(x19)\n"
			"flog.ps f2, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 512(x23)\n"
			"flw.ps f11, 448(x23)\n"
			"fadd.ps f6, f25, f11, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xf << TENSOR_IMA_SCP_LOC_B) | (0xca << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
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
			"flw.ps f25, 160(x27)\n"
			"flw.ps f11, 320(x27)\n"
			"fsgnjx.ps f6, f25, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 0(x14)\n"
			"flw.ps f30, 544(x14)\n"
			"flt.s x15, f22, f30\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 288(x13)\n"
			"flw.ps f7, 352(x13)\n"
			"fmin.ps f25, f30, f7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 320(x7)\n"
			"flw.ps f18, 960(x7)\n"
			"fsgnjn.ps f25, f11, f18\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 800(x5)\n"
			"flw.ps f15, 192(x5)\n"
			"fdiv.pi f7, f18, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 352(x25)\n"
			"flw.ps f14, 608(x25)\n"
			"flw.ps f19, 768(x25)\n"
			"fnmadd.s f11, f7, f14, f19, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 544(x15)\n"
			"flw.ps f2, 384(x15)\n"
			"flt.pi f14, f17, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 256(x7)\n"
			"fpackreph.pi f2, f11\n" 
			"fbci.ps f2, 0x603db\n" 
			"li x14, 0xca5aa9163b258e53\n"
			"fcvt.s.w f12, x14, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 576(x14)\n"
			"fsrai.pi f10, f2, 0x4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 96(x13)\n"
			"fround.ps f12, f7, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 608(x14)\n"
			"fcvt.w.s x25, f15, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 160(x5)\n"
			"fcvt.lu.s x5, f23, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 256(x17)\n"
			"fslli.pi f6, f30, 0x3\n" 
			"fbci.pi f23, 0x5c46d\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 896(x5)\n"
			"flw.ps f30, 32(x5)\n"
			"fmul.s f6, f6, f30, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 576(x13)\n"
			"flw.ps f19, 192(x13)\n"
			"fsra.pi f22, f23, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 704(x13)\n"
			"fswizz.ps f11, f11, 0x1d\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_15:\n"
			"fltm.ps m7, f17, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 864(x13)\n"
			"fround.ps f25, f15, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 192(x20)\n"
			"flw.ps f23, 64(x20)\n"
			"fmax.ps f30, f7, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 544(x27)\n"
			"fcvt.pwu.ps f10, f18, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 448(x14)\n"
			"fpackreph.pi f15, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 256(x15)\n"
			"flw.ps f7, 192(x15)\n"
			"fle.ps f2, f1, f7\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 608(x13)\n"
			"fmul.s f22, f17, f17, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 288(x14)\n"
			"flw.ps f19, 320(x14)\n"
			"flt.ps f30, f10, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 32(x15)\n"
			"flw.ps f6, 576(x15)\n"
			"fle.ps f22, f1, f6\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 832(x27)\n"
			"fslli.pi f10, f23, 0x7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 864(x27)\n"
			"flw.ps f2, 384(x27)\n"
			"flt.s x20, f12, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 576(x5)\n"
			"fandi.pi f17, f15, 0xb5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 192(x15)\n"
			"fclass.ps f30, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 480(x13)\n"
			"flw.ps f25, 160(x13)\n"
			"flw.ps f19, 352(x13)\n"
			"fnmadd.s f12, f23, f25, f19, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 352(x17)\n"
			"flw.ps f7, 224(x17)\n"
			"fle.pi f6, f10, f7\n" 
			"li x15, 0x207f3b1488da962\n"
			"fcvt.s.l f18, x15, rup\n" 
			"maskpopcz x12, m6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 224(x12)\n"
			"flw.ps f18, 576(x12)\n"
			"fsub.ps f23, f1, f18, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 32(x5)\n"
			"flw.ps f14, 544(x5)\n"
			"fadd.s f25, f6, f14, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 800(x15)\n"
			"flw.ps f11, 384(x15)\n"
			"fsra.pi f25, f23, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 992(x12)\n"
			"flw.ps f23, 160(x12)\n"
			"fsgnjx.s f7, f14, f23\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 768(x12)\n"
			"flw.ps f12, 192(x12)\n"
			"fmul.s f11, f30, f12, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 704(x14)\n"
			"flw.ps f17, 896(x14)\n"
			"fadd.pi f30, f14, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 160(x12)\n"
			"flw.ps f22, 416(x12)\n"
			"fsgnj.ps f6, f2, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 576(x12)\n"
			"flw.ps f6, 672(x12)\n"
			"fsgnjx.s f22, f10, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 736(x27)\n"
			"flw.ps f1, 800(x27)\n"
			"fmax.s f17, f7, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 992(x20)\n"
			"flw.ps f22, 224(x20)\n"
			"fsgnjx.ps f22, f23, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 928(x12)\n"
			"flw.ps f10, 320(x12)\n"
			"for.pi f22, f19, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 544(x7)\n"
			"ffrc.ps f18, f15\n" 
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
			"flw.ps f7, 928(x15)\n"
			"fcvt.w.s x5, f7, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 384(x25)\n"
			"flw.ps f10, 256(x25)\n"
			"fmin.pi f2, f2, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 800(x7)\n"
			"flw.ps f2, 896(x7)\n"
			"fsgnjn.ps f11, f22, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 480(x25)\n"
			"flw.ps f15, 64(x25)\n"
			"flw.ps f19, 128(x25)\n"
			"fmsub.ps f25, f14, f15, f19, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 992(x17)\n"
			"flw.ps f7, 64(x17)\n"
			"flt.s x13, f11, f7\n" 
			"fbci.pi f12, 0x724c6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_1_HID_15:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f12, 480(x25)\n"
			"flw.ps f7, 800(x25)\n"
			"fsrl.pi f14, f12, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 512(x17)\n"
			"fsgnj.ps f30, f7, f7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 384(x15)\n"
			"flw.ps f30, 320(x15)\n"
			"fnmadd.s f15, f11, f11, f30, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 832(x15)\n"
			"fsqrt.ps f2, f14\n" 
			"LBL_SEQID_3_M0_WRITE_ID_2_HID_15:\n"
			"maskand m6, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 256(x13)\n"
			"flw.ps f6, 480(x13)\n"
			"fsgnjx.s f17, f12, f6\n" 
			"la x5, rand_ieee754_16\n"
			"flw.ps f11, 320(x5)\n"
			"fcvt.ps.f16 f25, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 672(x5)\n"
			"flw.ps f18, 384(x5)\n"
			"fsub.pi f11, f30, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_15:\n"
			"fltm.pi m7, f22, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
		0x5bb67345, 
		0x34b15dd9, 
		0xe3217d06, 
		0xdedf774b, 
		0xf85d023a, 
		0x0ec3f0b3, 
		0x7201abfa, 
		0xa0b02d5a, 
		0x5e0bc446, 
		0x2ec96efa, 
		0x39d9fdc0, 
		0x68ad935e, 
		0xf1fa31d4, 
		0x8b120666, 
		0x88405100, 
		0x3bf328c1, 
		0xde62223b, 
		0x25f95fdb, 
		0x5866b0a2, 
		0x768c7233, 
		0x67c4237b, 
		0x81f29afb, 
		0xa1d481ee, 
		0x3653d6a3, 
		0xa0faef7d, 
		0x1c16b098, 
		0x0a2d446b, 
		0x2c2e60ae, 
		0xfa214020, 
		0x561b1a93, 
		0x073c7f0f, 
		0x5e57fe61, 
		0x9420a3ea, 
		0xfbc4390b, 
		0x1fd1ee81, 
		0x62815e08, 
		0xfda2ec13, 
		0x8383e1e9, 
		0x93149ffc, 
		0xac7e2678, 
		0x4e16f71f, 
		0xf0fd07a2, 
		0x61ad26cc, 
		0xff7d33b3, 
		0x31fa9511, 
		0x7c000f43, 
		0x6ab4536e, 
		0x26937612, 
		0x07e10c30, 
		0x574cd061, 
		0xdeed0bda, 
		0xe8a54ad6, 
		0x6bddced9, 
		0x58cac5b9, 
		0xeb7a88f9, 
		0x20214acf, 
		0x69b01b08, 
		0x1cf56ba9, 
		0x402fc161, 
		0x9cd3fe5c, 
		0xa6f0d09f, 
		0xdce98965, 
		0x1b9cb5fb, 
		0xf0746e7a, 
		0x930ed480, 
		0x6c404fb3, 
		0x2cc2e00f, 
		0xe83b2d48, 
		0x59055777, 
		0x15313fb2, 
		0x715dba22, 
		0x7bb1256f, 
		0xec7b4cfe, 
		0x6ca1c965, 
		0x0fc5c092, 
		0x78c5cb8a, 
		0x031eff92, 
		0x3c3ed4ec, 
		0xe84282fc, 
		0x2f12994d, 
		0xbff7fef8, 
		0x65427745, 
		0x7ca219a4, 
		0x6884f2e7, 
		0x63e9853d, 
		0x317b65cc, 
		0x694a94ae, 
		0x40462315, 
		0x44fc0177, 
		0x34ed172a, 
		0x1d99fb49, 
		0x0c9525d6, 
		0xc6a98de5, 
		0x78777596, 
		0x9481a456, 
		0x6a8d45a1, 
		0xe240faf1, 
		0x732a38e4, 
		0x1ab46ef7, 
		0x019ce8eb, 
		0x2ec47132, 
		0x5ad75e51, 
		0x120c4514, 
		0x5906fcf6, 
		0x6324896d, 
		0xdd4ff97a, 
		0xd1c7f41d, 
		0x47f89926, 
		0x3672c4eb, 
		0x1cc1dff7, 
		0x5179f50d, 
		0x5df468d2, 
		0x3aa7335f, 
		0xf25ac055, 
		0x9ac180ce, 
		0x4e67b3d7, 
		0xf48f33e9, 
		0x2d26c5ba, 
		0x01b83869, 
		0x79b65e42, 
		0x698e1785, 
		0x0035da11, 
		0xada87c4a, 
		0x0f4451af, 
		0xa531ad20, 
		0x46d15b5f, 
		0xc5d637bc, 
		0xc3ba1db5, 
		0xc68093c9, 
		0x8edb634c, 
		0x5db860ea, 
		0xb8af0640, 
		0x582baa77, 
		0x4c6b03c8, 
		0x8626da6e, 
		0x60a2afc1, 
		0x7ab1fdf7, 
		0xcb964089, 
		0xe4050146, 
		0x5bbb61c5, 
		0xf3d55d08, 
		0x9072c6a9, 
		0x9621304a, 
		0xb20bf82b, 
		0x80e1574c, 
		0x56ca9895, 
		0xc4124f5b, 
		0xdb908f47, 
		0xa62790ed, 
		0x21e6114f, 
		0x26a0b9bf, 
		0xa0f9d255, 
		0x01e0453a, 
		0xf98b2412, 
		0x0f6e7186, 
		0x8348ca63, 
		0xe4c2428f, 
		0x0deb7441, 
		0xbe347d97, 
		0x692e787e, 
		0x8edae4fa, 
		0x5c2dc7fd, 
		0x8ae4ce3f, 
		0xae6e338c, 
		0xec674249, 
		0x97e00d41, 
		0x9ccdf306, 
		0xf9a4da08, 
		0xd6f38a0d, 
		0x93f2658f, 
		0x675c1de5, 
		0x41f6a3c5, 
		0x01871a46, 
		0x3234c107, 
		0x7e979278, 
		0x7ff97cfb, 
		0x1914a2d7, 
		0x94cd876a, 
		0xfcadf9eb, 
		0x8da4b0cd, 
		0xa6b5e888, 
		0x1d32d40b, 
		0x5e51f68b, 
		0x81e0d25c, 
		0xff9488a2, 
		0xd72dd982, 
		0xd1c72d91, 
		0x34731606, 
		0x7773cdb3, 
		0xb79407cc, 
		0x1873d602, 
		0x8a1017c2, 
		0x30220177, 
		0xeb411de8, 
		0x6d7d16b0, 
		0x146e9d7d, 
		0x6d92cbb5, 
		0xa0768335, 
		0xb0238402, 
		0x264f30fb, 
		0x35c201bf, 
		0xe8f9a302, 
		0x98f55684, 
		0xb6eec444, 
		0x15371d8a, 
		0xb6cebc2f, 
		0x780df93b, 
		0xfd47c0d2, 
		0xaf376e20, 
		0x28041408, 
		0xa5ac1c4e, 
		0x5eac722c, 
		0x67b25a58, 
		0xa55c11e0, 
		0xf654bc3b, 
		0xaf5e1e5c, 
		0xa49e9da6, 
		0xa23f21a0, 
		0x15b006e4, 
		0xa5764bd1, 
		0x1b107e18, 
		0x84c0677c, 
		0x81909b81, 
		0x048ccec7, 
		0x631a9119, 
		0x2253a17f, 
		0x9a08302c, 
		0xd7c60cfc, 
		0x37c0b864, 
		0x320c0ee7, 
		0xc555632c, 
		0x1473f226, 
		0x46424463, 
		0x3f1570c7, 
		0x14a8a17e, 
		0x356276bb, 
		0x339c83c4, 
		0xcf4c0870, 
		0xe97646f5, 
		0xf902e43e, 
		0xa2828ad8, 
		0x015a4e4a, 
		0xb27b7945, 
		0xd9374dbe, 
		0x25c54bb5, 
		0x37f154cc, 
		0x0fab6745, 
		0x3b7d71f4, 
		0xa91fbf4e, 
		0x0f5d0337, 
		0x3faf2ced, 
		0x1a03fc52, 
		0x22c06b8f, 
		0x4ce30933, 
		0x88a3d6cf, 
		0xe208f906  
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
		0xd3c0, 
		0xd280, 
		0xd100, 
		0x4fc0, 
		0x0000, 
		0x53c0, 
		0xcf40, 
		0x81bf, 
		0x53fc, 
		0x8000, 
		0x4980, 
		0xc980, 
		0x53a0, 
		0xcd00, 
		0x8000, 
		0x51e0, 
		0xd2c0, 
		0x5220, 
		0x4200, 
		0xc6b2, 
		0xd2e0, 
		0x4a00, 
		0xd200, 
		0xccc0, 
		0x5280, 
		0xe6a2, 
		0xd060, 
		0xb627, 
		0x4600, 
		0xb1b5, 
		0x4fc0, 
		0xd020, 
		0xfc00, 
		0x5060, 
		0x7f60, 
		0xfc12, 
		0xd3c0, 
		0x109f, 
		0xfdc4, 
		0x5100, 
		0xcfc0, 
		0xd340, 
		0x291c, 
		0x9919, 
		0xd1a0, 
		0x7c00, 
		0x4fc0, 
		0xfc00, 
		0x4500, 
		0xcd40, 
		0xd819, 
		0x1f0b, 
		0xd1e0, 
		0x0000, 
		0x51c0, 
		0x52e0, 
		0x8000, 
		0xd100, 
		0xcf00, 
		0xd140, 
		0x52e0, 
		0x4700, 
		0xd360, 
		0x4600, 
		0x8000, 
		0x5240, 
		0x50e0, 
		0x8000, 
		0xc800, 
		0x4980, 
		0x8a7b, 
		0x8b8f, 
		0xc500, 
		0xc500, 
		0xd280, 
		0xd0a0, 
		0x4700, 
		0x8000, 
		0x8000, 
		0xd360, 
		0xcc80, 
		0x76f1, 
		0xc8ab, 
		0xaca2, 
		0x4cc0, 
		0xe153, 
		0x4400, 
		0x4a45, 
		0x4c80, 
		0x53e0, 
		0xc200, 
		0x52a0, 
		0x5140, 
		0x6b36, 
		0x8000, 
		0x379d, 
		0xd3c0, 
		0x51d4, 
		0x8000, 
		0x3217, 
		0xa3e3, 
		0x5000, 
		0xce60, 
		0x4400, 
		0x7c00, 
		0xfc00, 
		0xb278, 
		0xd1e0, 
		0x5320, 
		0x232e, 
		0x683f, 
		0x2ee6, 
		0xcf80, 
		0xb65f, 
		0xd320, 
		0x8738, 
		0xcd00, 
		0x0000, 
		0x4200, 
		0x8a3f, 
		0xcc80, 
		0xd300, 
		0xd260, 
		0x2cdf, 
		0xd2e0, 
		0xcb00, 
		0xf264, 
		0xd280, 
		0x52c0, 
		0x4fc0, 
		0x9905, 
		0x5260, 
		0x50e0, 
		0x51af, 
		0xaec5, 
		0x5380, 
		0x6961, 
		0xd1e0, 
		0xd1e0, 
		0xda92, 
		0x386a, 
		0xbbee, 
		0x4600, 
		0xd200, 
		0xbc00, 
		0x792f, 
		0xc23b, 
		0x2c68, 
		0x4b00, 
		0xd080, 
		0xcc00, 
		0x4b71, 
		0x4700, 
		0x5360, 
		0x4f00, 
		0x804a, 
		0xd280, 
		0xfe6f, 
		0xd240, 
		0xd120, 
		0xc700, 
		0x4f40, 
		0xcd40, 
		0x8f04, 
		0x53c0, 
		0xb23c, 
		0xc400, 
		0xdb8c, 
		0x4700, 
		0xc000, 
		0x51e0, 
		0xd280, 
		0x5140, 
		0xcec0, 
		0x5300, 
		0x4758, 
		0xb8a4, 
		0x0319, 
		0x20ae, 
		0xd160, 
		0xca00, 
		0xcd80, 
		0xcc00, 
		0x7c00, 
		0x4e40, 
		0x24d9, 
		0xc84f, 
		0x5240, 
		0x0000, 
		0xcd80, 
		0x8000, 
		0x8000, 
		0x4c80, 
		0x15f7, 
		0x8000, 
		0xd3e0, 
		0xca00, 
		0x19ac, 
		0xd3a0, 
		0x53c0, 
		0x5200, 
		0x6e9a, 
		0x7c00, 
		0x4400, 
		0x4c40, 
		0x0000, 
		0x9d40, 
		0x4d00, 
		0x5220, 
		0xd140, 
		0x4700, 
		0x560a, 
		0x8000, 
		0x4200, 
		0x0000, 
		0x5320, 
		0x8000, 
		0x2fe0, 
		0x4dc0, 
		0x4ec0, 
		0xabc5, 
		0xd240, 
		0x4e40, 
		0x111f, 
		0x6dcf, 
		0xcf80, 
		0xee46, 
		0x0618, 
		0x086c, 
		0x4b00, 
		0xd3a0, 
		0x8893, 
		0x5240, 
		0x06e7, 
		0x8000, 
		0x51c0, 
		0xc843, 
		0xd200, 
		0x7c00, 
		0xa3b6, 
		0x0066, 
		0xfc00, 
		0x4cc0, 
		0xcb00, 
		0x5020, 
		0xd2e0, 
		0xd320, 
		0x7c00, 
		0xc900, 
		0x53e0, 
		0x8000, 
		0x5140, 
		0xc159, 
		0x4500, 
		0x4880, 
		0xd020, 
		0x36e7, 
		0x8000, 
		0x5320, 
		0x5f72, 
		0xb16f, 
		0xcdc0, 
		0xd3a0, 
		0xd060, 
		0x4e80, 
		0x5340, 
		0xce40, 
		0x4dc0, 
		0x0000, 
		0x36f2, 
		0xb485, 
		0x4d80, 
		0xca80, 
		0x4c80, 
		0x0bc3, 
		0x5360, 
		0xd1a0, 
		0x06c0, 
		0xf158, 
		0xd280, 
		0x4600, 
		0x417a, 
		0x5300, 
		0xfaa1, 
		0xd180, 
		0x6c89, 
		0x52c0, 
		0x5220, 
		0xbaa4, 
		0xcc40, 
		0xd100, 
		0xd240, 
		0x4fc0, 
		0x0000, 
		0x2947, 
		0xd340, 
		0x4e00, 
		0xd0a0, 
		0xf397, 
		0xcb00, 
		0xc900, 
		0xd0c0, 
		0x2075, 
		0xc800, 
		0x4fc0, 
		0xd320, 
		0x5060, 
		0xd2c0, 
		0x3b72, 
		0x7c2a, 
		0x0000, 
		0xd3a0, 
		0xc000, 
		0xd120, 
		0xd3e0, 
		0x4c40, 
		0x8a49, 
		0xd260, 
		0x51a0, 
		0xc500, 
		0xfc00, 
		0x0000, 
		0xd92a, 
		0x4f80, 
		0x52e0, 
		0xfc00, 
		0x0000, 
		0xd32c, 
		0xcfc0, 
		0x8457, 
		0x026c, 
		0xd060, 
		0x4c40, 
		0x4e80, 
		0x5080, 
		0x4880, 
		0x51e0, 
		0x5040, 
		0x0000, 
		0x43ec, 
		0x0000, 
		0xb536, 
		0xd3e0, 
		0xd1e0, 
		0x0000, 
		0xf8b1, 
		0x4ec0, 
		0x4b80, 
		0xc400, 
		0x50a0, 
		0xd2c0, 
		0x9781, 
		0xcf80, 
		0xd040, 
		0x4b00, 
		0xedd1, 
		0xccc0, 
		0xce00, 
		0xcdc0, 
		0xcfc0, 
		0x4000, 
		0x4d00, 
		0xca00, 
		0xce80, 
		0x5220, 
		0x50c0, 
		0x4f80, 
		0xe246, 
		0x45bd, 
		0xd0c0, 
		0x3126, 
		0xa1bb, 
		0xd2e0, 
		0x0000, 
		0xfc00, 
		0xd2c0, 
		0xfd28, 
		0xd2e0, 
		0x8000, 
		0xc400, 
		0xd0a0, 
		0x5160, 
		0xcc80, 
		0xbc5e, 
		0xd080, 
		0x8000, 
		0xac64, 
		0xcf00, 
		0x7bc1, 
		0xca00, 
		0x4e40, 
		0x50a0, 
		0x98c9, 
		0x42ab, 
		0xd1e8, 
		0xd260, 
		0xcd00, 
		0xcf00, 
		0xe274, 
		0xd100, 
		0x5200, 
		0x8000, 
		0xd220, 
		0x50c0, 
		0xc880, 
		0x082f, 
		0xfc00, 
		0x3534, 
		0x19d0, 
		0x8000, 
		0x8eb4, 
		0xcc00, 
		0xce00, 
		0xd2a0, 
		0x1012, 
		0x5a72, 
		0x51e0, 
		0xd2a0, 
		0x4dc0, 
		0x0000, 
		0x3ea5, 
		0xfcff, 
		0xcb6d, 
		0xd2e0, 
		0xb788, 
		0xd100, 
		0xe923, 
		0xc200, 
		0xd200, 
		0x52e0, 
		0x0000, 
		0x50a0, 
		0x9c89, 
		0xf749, 
		0xd0a0, 
		0xe614, 
		0x4400, 
		0xf9da, 
		0xd2e0, 
		0xd240, 
		0x64f4, 
		0x4800, 
		0xc900, 
		0x4cc0, 
		0xb970, 
		0xfd07, 
		0x9007, 
		0xcf40, 
		0xa573, 
		0x4fc0, 
		0xcb80, 
		0x5120, 
		0x7ba5, 
		0xc500, 
		0xd3a0, 
		0x4a00, 
		0x4e00, 
		0xd9a5, 
		0x4e40, 
		0xd0c0, 
		0xdd64, 
		0xd2a0, 
		0xcdc0, 
		0x5180, 
		0xfcd8, 
		0x7c8d, 
		0x5000, 
		0x6977, 
		0x5360, 
		0xcec0, 
		0xc880, 
		0x832d, 
		0x4d00, 
		0xc900, 
		0xca80, 
		0x525f, 
		0x4e40, 
		0xd3c0, 
		0x1d74, 
		0x4582, 
		0x4f80, 
		0x4b80, 
		0x4d00, 
		0xd220, 
		0x7c00, 
		0x4600, 
		0x51a0, 
		0xd260, 
		0x6b9c, 
		0x8fd6, 
		0xdce6, 
		0x3a24, 
		0xd200, 
		0x6393, 
		0x4568, 
		0x977e, 
		0x185c, 
		0x4200, 
		0xc980, 
		0x5280, 
		0x166b, 
		0xf2bd, 
		0xd3c0, 
		0x51e0, 
		0x4900, 
		0xfc00, 
		0xccb5, 
		0x5604, 
		0xaa9c, 
		0x7dac, 
		0xc088, 
		0xcd80, 
		0x52a0, 
		0xd000, 
		0x5060, 
		0x0000, 
		0x4200, 
		0xc700, 
		0x0e8d, 
		0x54c2, 
		0xd1e0, 
		0x80f4, 
		0xd380, 
		0xfc00, 
		0x4b80, 
		0xd160, 
		0x8000, 
		0xffdf, 
		0xce40, 
		0x1116, 
		0x4e40, 
		0xcc80, 
		0x8000, 
		0x3480, 
		0x0000, 
		0x9278, 
		0x4fc0, 
		0xad85, 
		0x51e0, 
		0xbc04, 
		0x4ef4, 
		0x4f00, 
		0x5202, 
		0xa410, 
		0xc600, 
		0x0000, 
		0x8dff, 
		0xd2a0, 
		0xce00, 
		0xd260, 
		0x51a0, 
		0xcd80, 
		0x519d, 
		0x4f3a, 
		0x94a9, 
		0xd3a0, 
		0x5120, 
		0xbd52, 
		0x5180, 
		0xd320, 
		0xd220, 
		0x4980, 
		0xcf40, 
		0x50e0, 
		0xa582, 
		0xd280, 
		0x55cb, 
		0xd220, 
		0xc200, 
		0x50e0, 
		0x4980, 
		0xea26, 
		0xc1c0, 
		0x9df1, 
		0x3d8e, 
		0xc880, 
		0x0955, 
		0xcc00, 
		0x9eb8, 
		0x53c0, 
		0xc200, 
		0xce23, 
		0xfc00, 
		0x3e48, 
		0x5140, 
		0x8000, 
		0x4a80, 
		0xd320, 
		0x0000, 
		0xd1e0, 
		0xce40, 
		0x0918, 
		0x7c00, 
		0x4b00, 
		0x4b80, 
		0xc900, 
		0xd1e0, 
		0xb98a, 
		0x4500, 
		0x5320, 
		0xcc00, 
		0x81ba, 
		0x51e0, 
		0x4200, 
		0x4880, 
		0xcd80, 
		0xc2e8, 
		0x5220, 
		0x4000, 
		0x43bf, 
		0xcb00, 
		0x6e02, 
		0x4e00, 
		0x5000, 
		0xbce9, 
		0x52a0, 
		0x50cb, 
		0xd360, 
		0x4b00, 
		0x3580, 
		0xdbb1, 
		0x8000, 
		0x7cbd, 
		0x4fc0, 
		0xc700, 
		0xdba7, 
		0xd3da, 
		0x8000, 
		0x1884, 
		0x5260, 
		0xa1ba, 
		0x1c4e, 
		0xfc00, 
		0x0000, 
		0xd0c0, 
		0x4dc0, 
		0x4900, 
		0xd0c0, 
		0x210b, 
		0xc880, 
		0xdd02, 
		0x53e0, 
		0xca00, 
		0x0fb2, 
		0xc980, 
		0x4c00, 
		0xcfc0, 
		0xce00, 
		0xcdc0, 
		0xd300, 
		0xcc40, 
		0xb218, 
		0xc700, 
		0x8beb, 
		0x2e7c, 
		0x50e0, 
		0x4400, 
		0x7acb, 
		0xc980, 
		0xcdc0, 
		0x52c0, 
		0x4a00, 
		0x0198, 
		0x4e40, 
		0xd0e0, 
		0x66b1, 
		0x5160, 
		0x4900, 
		0xccc0, 
		0x4e80, 
		0xc800, 
		0xd0c0, 
		0x3c00, 
		0xd100, 
		0x06d7, 
		0x5360, 
		0xce80, 
		0xd3e0, 
		0x52a0, 
		0xfaf9, 
		0xdcac, 
		0x4ec0, 
		0x4b80, 
		0x12d8, 
		0xe489, 
		0xd020, 
		0x2cff, 
		0x8e8b, 
		0x7c81, 
		0xd304, 
		0x5200, 
		0x0000, 
		0xcb00, 
		0xfc00, 
		0x52e0, 
		0xc853, 
		0xe063, 
		0xe615, 
		0x5220, 
		0x0000, 
		0xcc00, 
		0x4880, 
		0xfaa7, 
		0xe6a3, 
		0xc6f5, 
		0x4e80, 
		0xe160, 
		0xd040, 
		0xccc0, 
		0x80c0, 
		0xc600, 
		0x7c00, 
		0x4a80, 
		0x5120, 
		0x5320, 
		0xcb00, 
		0x203f, 
		0xf8ea, 
		0x2f8e, 
		0xcc80, 
		0x349d, 
		0x5220, 
		0xbe5e, 
		0x9280, 
		0x4b00, 
		0xf4d9, 
		0xca80, 
		0x4c40, 
		0xd320, 
		0x7057, 
		0xd180, 
		0xa36a, 
		0x0000, 
		0xcabd, 
		0xabec, 
		0xdbab, 
		0x18fd, 
		0x4f00, 
		0x0654, 
		0x132d, 
		0x52e0, 
		0xd0e0, 
		0x182d, 
		0xce00, 
		0x4400, 
		0x3beb, 
		0xd2c0, 
		0x5060, 
		0xc900, 
		0xcd80, 
		0xbdd5, 
		0x8000, 
		0xd040, 
		0x7dad, 
		0xd280, 
		0x5070, 
		0xd854, 
		0xbc00, 
		0x01c9, 
		0x3985, 
		0x8000, 
		0x182f, 
		0x0cca, 
		0xb944, 
		0x5c42, 
		0x9f81, 
		0x5320, 
		0x4f80, 
		0x4f80, 
		0x79c7, 
		0x4f00, 
		0x8000, 
		0x51a0, 
		0xd020, 
		0xcf80, 
		0xcf00, 
		0xd3a0, 
		0x5140, 
		0x35c3, 
		0x5320, 
		0x8000, 
		0xef84, 
		0xd320, 
		0x8000, 
		0xd120, 
		0x4f00, 
		0xd1c0, 
		0x7c00, 
		0xcc40, 
		0xca80, 
		0x071d, 
		0xb7ec, 
		0xcc40, 
		0xab52, 
		0xc600, 
		0xc700, 
		0xc980, 
		0xb49f, 
		0xd300, 
		0x5120, 
		0x874f, 
		0x7c52, 
		0xcc40, 
		0xcd80, 
		0xfc00, 
		0xfc00, 
		0x4a00, 
		0x3c6d, 
		0x8000, 
		0x25aa, 
		0x43dc, 
		0xce81, 
		0x5320, 
		0x1f36, 
		0xd0e0, 
		0x0000, 
		0x3c00, 
		0x98eb, 
		0xd020, 
		0x8000, 
		0x5060, 
		0x5180, 
		0xadaf, 
		0xd200, 
		0xc800, 
		0x4700, 
		0xccc0, 
		0x8c6a, 
		0x4700, 
		0xcc80, 
		0x5100, 
		0xd0c0, 
		0xfd98, 
		0x5240, 
		0xa338, 
		0x8000, 
		0x8000, 
		0x2e54, 
		0xd140, 
		0xfc00, 
		0x5240, 
		0xb5f3, 
		0x0000, 
		0xcd00, 
		0xd3c0, 
		0x51a0, 
		0xbbf9, 
		0x4fc0, 
		0xd100, 
		0x4800, 
		0xb633, 
		0xd1c0, 
		0x4b00, 
		0xd2c0, 
		0x51a0, 
		0xd2a0, 
		0x81bc, 
		0x4d00, 
		0xe4d2, 
		0x7c00, 
		0xfc00, 
		0x5200, 
		0x5120, 
		0xd2e0, 
		0x06a2, 
		0xded1, 
		0x0000, 
		0x9df5, 
		0x5280, 
		0xfc00, 
		0x0000, 
		0x7c00, 
		0xd300, 
		0xa148, 
		0xb1b3, 
		0x4c00, 
		0xe97d, 
		0xc500, 
		0xce00, 
		0x4f40, 
		0xd260, 
		0x4e33, 
		0x4700, 
		0x4dc0, 
		0x2015, 
		0xdead, 
		0xd3c0, 
		0xf479, 
		0xf93b, 
		0xd140, 
		0x50e0, 
		0xd100, 
		0xd1c0, 
		0x5380, 
		0xd320, 
		0x5200, 
		0xca80, 
		0xeffe, 
		0xd160, 
		0xfc00, 
		0xd060, 
		0x8000, 
		0xd240, 
		0x8000, 
		0x8000, 
		0x3f84, 
		0xcd00, 
		0x7a20, 
		0xca00, 
		0xc000, 
		0xc200, 
		0xe5cd, 
		0xd280, 
		0xb3fc, 
		0x7b06, 
		0xd320, 
		0xd280, 
		0xd2e0, 
		0x50a0, 
		0xd2c0, 
		0xa79e, 
		0xfc9f, 
		0xbc00, 
		0xd000, 
		0x8000, 
		0x5380, 
		0x5320, 
		0xcec0, 
		0x8000, 
		0xc200, 
		0x8000, 
		0x5240, 
		0x1eda, 
		0x5260, 
		0x4000, 
		0x8000, 
		0x94e4, 
		0xfc00, 
		0x50a8, 
		0x1fc6, 
		0xbd0d, 
		0x5340, 
		0xd2c0, 
		0xcb00, 
		0x150e, 
		0x53e0, 
		0x4f00, 
		0xcf40, 
		0xd5a1, 
		0xd3c0, 
		0x5360, 
		0xab22, 
		0x5332, 
		0x5080, 
		0x5360, 
		0x4fc0, 
		0xc700, 
		0x50a0, 
		0x7b02, 
		0x73bb, 
		0xd200, 
		0xc200, 
		0x0376, 
		0x321d, 
		0xd240, 
		0x5120, 
		0x0000, 
		0x4880, 
		0xd2e0, 
		0xd060, 
		0xc800, 
		0xfd9e, 
		0xd100, 
		0x32be, 
		0x0000, 
		0xa626, 
		0xd82d, 
		0xcf80, 
		0x4d80, 
		0xcb80, 
		0xbc00, 
		0xb45e, 
		0x1a8b, 
		0x51c0, 
		0xd000, 
		0x5380, 
		0x8000, 
		0x7c00, 
		0xe605, 
		0x0000, 
		0x4e00, 
		0x5160, 
		0xd2c0, 
		0x4d40, 
		0xd380, 
		0x1533, 
		0xd180, 
		0x4f80, 
		0x1172, 
		0x4b80, 
		0xce00, 
		0x5c2e, 
		0xccc0, 
		0xc500, 
		0x4148, 
		0xcec0, 
		0x7c00, 
		0xf002, 
		0x796e, 
		0x5040, 
		0x6450, 
		0x9d5f, 
		0xce00, 
		0x4c40, 
		0x4c00, 
		0xf566, 
		0x5040, 
		0x4ec0, 
		0x3c00, 
		0x51c0, 
		0x5300, 
		0xd160, 
		0xd140, 
		0x4c72, 
		0x7c00, 
		0x4800, 
		0xb222, 
		0x5d21, 
		0xd3e0, 
		0x8d49, 
		0xeb7e, 
		0xd117, 
		0x5140, 
		0xc7e4, 
		0xdc3a, 
		0x51e0, 
		0xd1e0, 
		0x4f80, 
		0xed7a, 
		0xb2b7, 
		0xd0e0, 
		0x4d40, 
		0x5160, 
		0xcd6e, 
		0x7a3d, 
		0xd0a0, 
		0xcdc0, 
		0xd180, 
		0x5300, 
		0xc500, 
		0xd0e0, 
		0x53e0, 
		0xc700, 
		0x5220, 
		0x4400, 
		0x5083, 
		0xd3e0, 
		0x4000, 
		0xd2a0, 
		0x9c34, 
		0x9f50, 
		0xa27a, 
		0x0422, 
		0x8ee5, 
		0x4ec0, 
		0xd3e0, 
		0x7c00, 
		0x150f, 
		0xcd80, 
		0x5300, 
		0x2598, 
		0x53c0, 
		0xcec0, 
		0x4718, 
		0x0000, 
		0x4a00, 
		0xd100, 
		0x425d, 
		0x51e0, 
		0xdec7, 
		0xfc00, 
		0x75da, 
		0x5100, 
		0x0000, 
		0x5300, 
		0x8f62, 
		0x8000, 
		0xd220, 
		0x9908, 
		0x309a, 
		0x7c00, 
		0x9359, 
		0xd380, 
		0x15d0, 
		0x52e0, 
		0xb8dd, 
		0xa61e, 
		0x4a00, 
		0x5697, 
		0xd260, 
		0xd81e, 
		0xd340, 
		0xd000, 
		0x2103, 
		0xd280, 
		0x9225, 
		0xce80, 
		0x5240, 
		0x9ad8, 
		0xa321, 
		0x5260, 
		0xb66f, 
		0xd360, 
		0xd3e0, 
		0x5120, 
		0xccc0, 
		0x4c40, 
		0xd100, 
		0x51c0, 
		0x4f00, 
		0x95b3, 
		0xb8a4, 
		0xc600, 
		0x9ffd, 
		0x3f58, 
		0x4f80, 
		0x6ada, 
		0xc689, 
		0x4980, 
		0x5080, 
		0x4cbf, 
		0x5340, 
		0x690d, 
		0x0f2e, 
		0x8000, 
		0x4f00, 
		0xfc00, 
		0xd300, 
		0x9ef0, 
		0xc880, 
		0x0000, 
		0x83e1, 
		0x0aac, 
		0x53c0, 
		0x65cc, 
		0x019a, 
		0xd080, 
		0xee34, 
		0xd060, 
		0xd040, 
		0xccc0, 
		0xf19e, 
		0xd064, 
		0xce40, 
		0x4900, 
		0x4d80, 
		0x049a, 
		0x4200, 
		0x25fe, 
		0x4880, 
		0x4f40, 
		0x446c, 
		0xd120, 
		0x4b00, 
		0x4880, 
		0xfdeb, 
		0x1164, 
		0x4200, 
		0x4500, 
		0x96cb, 
		0xd100, 
		0xc600, 
		0x7480, 
		0x5380, 
		0xcd40, 
		0xd060, 
		0xd3a0, 
		0x5060, 
		0x5260, 
		0x5060, 
		0xfc00, 
		0xea79, 
		0x4400, 
		0x4c00, 
		0x8000, 
		0x20c6, 
		0x4fc0, 
		0x53a0, 
		0x53a0, 
		0x4c00, 
		0x4280, 
		0x5380, 
		0xd320, 
		0xd3e0, 
		0x00c6, 
		0xd875, 
		0x5100, 
		0xcd40, 
		0x4880, 
		0x0000, 
		0xd340, 
		0xcc40, 
		0x53e0, 
		0xcb80, 
		0x5040, 
		0x48ed, 
		0xa0f8, 
		0x0259, 
		0xaa0b, 
		0xbc00, 
		0xcf40, 
		0xc980, 
		0x9de7, 
		0x1a69, 
		0x5300, 
		0xd0a0, 
		0xcd80, 
		0xd1c0, 
		0x4c00, 
		0x53e0, 
		0x0f24, 
		0x8000, 
		0x4c40, 
		0xccc0, 
		0xb9d9, 
		0xdf2e, 
		0x7c00, 
		0x0000, 
		0x5a20, 
		0x5040, 
		0xd280, 
		0x4c00, 
		0x90bb, 
		0xd320, 
		0x8000, 
		0x2d70, 
		0xcf40, 
		0x4d40, 
		0xc980, 
		0xf31b, 
		0xc900, 
		0x51e0, 
		0x52e0, 
		0xc523, 
		0xd638, 
		0xc200, 
		0xcd40, 
		0x5f2e, 
		0xcd00, 
		0x52a0, 
		0xce40, 
		0xcb80, 
		0x576d, 
		0x9de1, 
		0x5180, 
		0xe90b, 
		0x7c00, 
		0x4a80, 
		0x53c0, 
		0x4400, 
		0xd260, 
		0x80eb, 
		0x4700, 
		0xfc00, 
		0xd2c0, 
		0xc22c, 
		0xfc00, 
		0xd360, 
		0x4800, 
		0xd100, 
		0xe921, 
		0xd2a0, 
		0xe787, 
		0x7c00, 
		0x5080, 
		0x50e0, 
		0xd080, 
		0xd080, 
		0x5200, 
		0x4e00, 
		0x4dc0, 
		0xd220, 
		0x38bf, 
		0xd03a, 
		0xd360, 
		0x5280, 
		0xd100, 
		0xb7d0, 
		0x0000, 
		0x53e0, 
		0xd1e0, 
		0x306e, 
		0x4cc0, 
		0xce40, 
		0x53e0, 
		0x5080, 
		0xd260, 
		0xcf80, 
		0x5160, 
		0x4c80, 
		0x52e0, 
		0xcc40, 
		0xc600, 
		0xfc00, 
		0xc200, 
		0x5000, 
		0x0000, 
		0x7d35, 
		0x35fb, 
		0xcf80, 
		0x8f48, 
		0x5180, 
		0x5320, 
		0xd080, 
		0xd2e0, 
		0x4f00, 
		0xd100, 
		0x8000, 
		0x4f40, 
		0xc880, 
		0xcb80, 
		0xd340, 
		0x50a0, 
		0x7c93, 
		0xf3a5, 
		0x3aec, 
		0xce40, 
		0x5320, 
		0x0000, 
		0x863d, 
		0x8000, 
		0x50e0, 
		0xd3c0, 
		0xfc00, 
		0x5280, 
		0x5160, 
		0x4f40, 
		0x823e, 
		0x4600, 
		0xd56f, 
		0x995c, 
		0x5240, 
		0xa8b3, 
		0x1631, 
		0xd1c0, 
		0xd0c0, 
		0xcdc0, 
		0xec0b, 
		0xd280, 
		0x5120, 
		0xb816, 
		0xaaf5, 
		0x4e40, 
		0x5000, 
		0xc600, 
		0x8d8c, 
		0x0543, 
		0x8000, 
		0xebd1, 
		0x4fc0, 
		0xd080, 
		0xbb3a, 
		0xd8e3, 
		0xce40, 
		0x5d17, 
		0xcd40, 
		0xce80, 
		0x5020, 
		0x0000, 
		0xd080, 
		0x4d40, 
		0xa4c3, 
		0xd0a0, 
		0xced3, 
		0x50a0, 
		0xd360, 
		0xcdc0, 
		0x8778, 
		0xc000, 
		0xd300, 
		0xc700, 
		0x4980, 
		0xd000, 
		0x4c00, 
		0x1835, 
		0xd1c0, 
		0x51fc, 
		0xcb80, 
		0x4800, 
		0xd220, 
		0x4c40, 
		0xd380, 
		0x5340, 
		0xc800, 
		0xc9e8, 
		0x4000, 
		0x4f80, 
		0xb46c, 
		0xfd0f, 
		0x7c72, 
		0x8000, 
		0xafe9, 
		0xd240, 
		0xbab1, 
		0x5140, 
		0xd360, 
		0x4b80, 
		0xd040, 
		0xd1a0, 
		0x8000, 
		0x7c00, 
		0x9507, 
		0x5140, 
		0xfd67, 
		0x4600, 
		0x3c00, 
		0x028f, 
		0x8000, 
		0x4399, 
		0xca00, 
		0x8131, 
		0x8000, 
		0x8000, 
		0x52c0, 
		0xf694, 
		0xd060, 
		0x80de, 
		0xdc81, 
		0xcd80, 
		0x4f40, 
		0x4c00, 
		0x319a, 
		0xcec0, 
		0xfc34, 
		0x934b, 
		0xd040, 
		0x4ec0, 
		0x8000, 
		0x52a0, 
		0xbc00, 
		0x5320, 
		0xd120, 
		0x062b, 
		0x4600, 
		0xdc5e, 
		0x53e0, 
		0x7c2f, 
		0xd0a0, 
		0x5120, 
		0x4d80, 
		0xeafc, 
		0x4980, 
		0xcfc0, 
		0x63fb, 
		0x5040, 
		0xccc0, 
		0x521d, 
		0xcc80, 
		0x4000, 
		0x8000, 
		0xd0c0, 
		0xc760, 
		0x5870, 
		0xffe2, 
		0xb2a6, 
		0xd3c0, 
		0xabb2, 
		0xd280, 
		0x7688, 
		0xc900, 
		0xcc00, 
		0x5080, 
		0xd160, 
		0xc500, 
		0x4ec0, 
		0x5360, 
		0xd320, 
		0xcdc0, 
		0x9830, 
		0x52e0, 
		0x371d, 
		0xd340, 
		0x53c0, 
		0xd1e0, 
		0x4b80, 
		0x51e0, 
		0xd240, 
		0xfeab, 
		0x5280, 
		0x8000, 
		0x4ec0, 
		0x5000, 
		0x00b4, 
		0x5240, 
		0xd260, 
		0x8000, 
		0x5100, 
		0x4980, 
		0x52e0, 
		0x5220, 
		0x42a1, 
		0x0000, 
		0xc880, 
		0x4800, 
		0xcec0, 
		0x4a80, 
		0xd0e0, 
		0x3c00, 
		0x8000, 
		0x5080, 
		0x7c00, 
		0xd0e0, 
		0x5100, 
		0x5380, 
		0x428e, 
		0xc000, 
		0x2327, 
		0xd738, 
		0x7cda, 
		0x53e0, 
		0x4d80, 
		0x51a0, 
		0xc880, 
		0xcd00, 
		0x065a, 
		0xfc00, 
		0xfc21, 
		0x4f80, 
		0xc500, 
		0x5200, 
		0x7ccd, 
		0x28a0, 
		0xfc49, 
		0x4709, 
		0xd260, 
		0x801c  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00800001, 
		0x00475127, 
		0x292178ce, 
		0x0c600000, 
		0x74b5039f, 
		0xc0000000, 
		0x00000000, 
		0x31a3789f, 
		0x4a949af9, 
		0x689bb31f, 
		0xbd64f6d6, 
		0x1ab88066, 
		0xff7ffffe, 
		0x0d00fff0, 
		0x7f800000, 
		0x9cfef104, 
		0xc1b80000, 
		0x24114a54, 
		0x80000010, 
		0x80800001, 
		0x80000040, 
		0x007ffffe, 
		0x6c0614f4, 
		0x4ace667a, 
		0x421c0000, 
		0x00000001, 
		0xff7ffffe, 
		0x41880000, 
		0x00000000, 
		0x80000020, 
		0x38285dd2, 
		0xcbc5c331, 
		0xc0800000, 
		0x00400000, 
		0xf0dcefa5, 
		0xecadd955, 
		0x0f7ffffd, 
		0x00000800, 
		0x0889a6a1, 
		0xc0799500, 
		0x0c1285d3, 
		0x42480000, 
		0xff800000, 
		0xc40fff69, 
		0x80100000, 
		0x142e0316, 
		0xd178baaf, 
		0x80000000, 
		0x0e0000ff, 
		0x80000002, 
		0x7ef3bc77, 
		0x80000010, 
		0x891ff3e2, 
		0x00000002, 
		0x0e0000ff, 
		0x00800001, 
		0xdcd70f7e, 
		0x0e000001, 
		0x126a04e1, 
		0x420c0000, 
		0x08636a50, 
		0xe78b0393, 
		0x00000200, 
		0x28867943, 
		0x0c7e0000, 
		0x00000000, 
		0xffe127d8, 
		0x42400000, 
		0x0e000fff, 
		0x42240000, 
		0x3f800001, 
		0x57eaac86, 
		0x33ca5f12, 
		0x42740000, 
		0xc2780000, 
		0xc0c00000, 
		0x4b8c4b40, 
		0x16004831, 
		0x92be1ba2, 
		0x8f7ffffd, 
		0xa11729d7, 
		0x007fffff, 
		0x41600000, 
		0xdded7c08, 
		0x0c7fff80, 
		0x80000800, 
		0x7f800000, 
		0x00000000, 
		0x7fc00000, 
		0x6b1995f0, 
		0x7f800000, 
		0x8502aeb8, 
		0xff949c57, 
		0xffc00000, 
		0xc2440000, 
		0xc1900000, 
		0x00000010, 
		0x00002000, 
		0x0a12db74, 
		0xc2100000, 
		0x00000002, 
		0x7fb69a27, 
		0x00000000, 
		0x80080000, 
		0x0c7fffc0, 
		0x8f7fffff, 
		0xc2300000, 
		0x80000100, 
		0xa357e6e5, 
		0x42380000, 
		0x80000000, 
		0x7c7e7742, 
		0xc1b00000, 
		0x42040000, 
		0x0e01ffff, 
		0x268de928, 
		0xc1900000, 
		0x7978c12c, 
		0xc1b00000, 
		0x42240000, 
		0x7fc00001, 
		0x42000000, 
		0xa98638b0, 
		0x0e7fffff, 
		0x80080000, 
		0xffb1a88a, 
		0x7f7fffff, 
		0xff800000, 
		0x4ee8670c, 
		0x0c7ff000, 
		0x41200000, 
		0x00000400, 
		0xbbe11b41, 
		0xe7f29680, 
		0x0e00003f, 
		0xc0a00000, 
		0x9213878c, 
		0xcb000000, 
		0x0e0003ff, 
		0x20ae4cec, 
		0x0f7ffffc, 
		0x1d0da661, 
		0xb64c6411, 
		0x3dedb02c, 
		0x7f800000, 
		0xf8bdb80c, 
		0x42540000, 
		0xc3c2f090, 
		0x4b8c4b40, 
		0x28d7bfc9, 
		0x007ffffe, 
		0x5c137090, 
		0x00000004, 
		0xc2780000, 
		0x7f800000, 
		0x8eea194a, 
		0x42400000, 
		0x7f800000, 
		0x9bd12287, 
		0x0d000ff0, 
		0xc1c00000, 
		0xdad5c7db, 
		0x45247954, 
		0xf9d039e0, 
		0x6ea4d026, 
		0x5d7e38ec, 
		0x807fffff, 
		0x00000800, 
		0x00400000, 
		0xc77a2e07, 
		0xffb98ca7, 
		0xb9fd62f9, 
		0x0f7ffffc, 
		0x0c7fe000, 
		0x41c80000, 
		0x5c6007c6, 
		0x98a804b8, 
		0x0c7fffff, 
		0x80000000, 
		0x864b9f29, 
		0x0c400000, 
		0x5e15c6f1, 
		0x80020000, 
		0x628a2687, 
		0x0c600000, 
		0x09549d98, 
		0x41800000, 
		0x80000000, 
		0x0c7e0000, 
		0xc2600000, 
		0x00008000, 
		0xff800000, 
		0x80800000, 
		0x80000000, 
		0x004fc6d8, 
		0xff800000, 
		0x00000020, 
		0x40bf17ac, 
		0x00100000, 
		0xc1a00000, 
		0xf97479e3, 
		0xff800000, 
		0x80800000, 
		0x99e92e98, 
		0xd256d503, 
		0xa1ce67ff, 
		0x80000000, 
		0x00020000, 
		0x7f800000, 
		0xff7ffffe, 
		0x0f7fffff, 
		0x80004000, 
		0xd543490e, 
		0xff7fffff, 
		0xa920f75b, 
		0xa999609f, 
		0xc0800000, 
		0xc2180000, 
		0xc1d60da5, 
		0x0e7fffff, 
		0xff7ffffe, 
		0x70f4777b, 
		0x42300000, 
		0x80040000, 
		0xc2680000, 
		0xc1800000, 
		0x4b0b8a7c, 
		0xcb1984a5, 
		0x00000100, 
		0x0d9f5c2f, 
		0x80800000, 
		0x00000001, 
		0x0c7fffe0, 
		0x00000000, 
		0x0c700000, 
		0x0c7fffff, 
		0x007ffffe, 
		0x678eb46e, 
		0xc1200000, 
		0x8d5c0ec7, 
		0xb3aaae45, 
		0xff800001, 
		0x41a80000, 
		0x89e4afd8, 
		0x470a8a41, 
		0xe11a280a, 
		0x80800001, 
		0x55769bcc, 
		0x80800000, 
		0xa3e3beea, 
		0x80800003, 
		0x7e101052, 
		0x7f800000, 
		0xc2440000, 
		0x0c7fffc0, 
		0x3f800000, 
		0x6f692785, 
		0xb9e5523c, 
		0x80000000, 
		0x33333333, 
		0x934e0b2e, 
		0x00800003, 
		0x6308167d, 
		0xc1a00000, 
		0x0e000fff, 
		0x80800000, 
		0x41880000, 
		0x80000400, 
		0x41e80000, 
		0x80004000, 
		0x6bb49ffd, 
		0xa7f00ddf, 
		0x0c7fff80, 
		0x6ce43612, 
		0xd5565b4a, 
		0x7fbfffff, 
		0xc1f00000, 
		0x7507efec, 
		0x425c0000, 
		0xc1b00000, 
		0x80000000, 
		0xc0c6b6c7, 
		0xc1b80000, 
		0xffc00000, 
		0x7f800000, 
		0xc0800000, 
		0x00000002, 
		0x0e7fffff, 
		0x80010000, 
		0xc2780000, 
		0x7f800000, 
		0xc26c0000, 
		0x80800001, 
		0xff800000, 
		0xc21c0000, 
		0x8687430a, 
		0x7f800000, 
		0x6c7fb1c6, 
		0xc1f80000, 
		0x00000020, 
		0x0f7fffff, 
		0x3262c88b, 
		0x41800000, 
		0xcc8ee2e4, 
		0xd5aa580a, 
		0xc1c80000, 
		0x7f800000, 
		0x7f8a02a4, 
		0xd5e929e0, 
		0x40000000, 
		0x426c0000, 
		0x9be52a6f, 
		0x00000008, 
		0x7f800000, 
		0x80000200, 
		0x80000001, 
		0xbeada53d, 
		0xc0800000, 
		0x41980000, 
		0x6d52a68f, 
		0x0c7fff00, 
		0x0e7fffff, 
		0x8f7ffffd, 
		0x00000000, 
		0x7f800000, 
		0xc1f80000, 
		0xffbc730a, 
		0x8f7fffff, 
		0x7f800000, 
		0x0d000ff0, 
		0x7f8550cf, 
		0xc21c0000, 
		0x7fc00000, 
		0x00000010, 
		0x00800003, 
		0x2acaad0f, 
		0x80800000, 
		0x355e11d2, 
		0xc2400000, 
		0x7f800000, 
		0x96783b06, 
		0x0c7fffff, 
		0xe7ac8fef, 
		0x00200000, 
		0x42640000, 
		0x7f7ffffe, 
		0x3f028f5c, 
		0x8dac5dcd, 
		0x42540000, 
		0xc1980000, 
		0x00000080, 
		0x24be967c, 
		0xcecb6fd9, 
		0x41f80000, 
		0x0e000fff, 
		0x80800000, 
		0x179aa651, 
		0x80800000, 
		0x80000008, 
		0xecc6d645, 
		0x0c7ffff0, 
		0x80ffffff, 
		0xc756f1d3, 
		0x45b9e1dd, 
		0xefd0ac5b, 
		0xc1f00000, 
		0xff000000, 
		0x24bc0774, 
		0x0c7ffff8, 
		0x7f800000, 
		0x41d80000, 
		0x56ca7789, 
		0x41d00000, 
		0x80000008, 
		0x5875b7b1, 
		0x00000000, 
		0x80008000, 
		0x00020000, 
		0x6c928699, 
		0x0c7ff800, 
		0xc0400000, 
		0xf03836e6, 
		0xc2000000, 
		0x42440000, 
		0x880a89c8, 
		0x0e003fff, 
		0x41980000, 
		0x4571562c, 
		0xffc00001, 
		0xc1400000, 
		0xc2480000, 
		0xc2380000, 
		0x0c7f8000, 
		0x80040000, 
		0x80020000, 
		0x42080000, 
		0x331da002, 
		0xff7fffff, 
		0x00800000, 
		0xffbb66e8, 
		0x80000000, 
		0xfabc080f, 
		0x569095d4, 
		0x680c8e71, 
		0x54181e18, 
		0x7cf597be, 
		0xff800000, 
		0x8f7ffffe, 
		0x24ad1394, 
		0x0c7c0000, 
		0x007ffffe, 
		0xd6b4366b, 
		0x807ffffe, 
		0x2cb49afc, 
		0x1c8d4b08, 
		0x00011111, 
		0x80000040, 
		0xb0428fad, 
		0x80000001, 
		0x807fffff, 
		0x758189d8, 
		0xcb000000, 
		0x00000002, 
		0x00000002, 
		0xff800000, 
		0x0e001fff, 
		0x00020000, 
		0x2455d250, 
		0x41d80000, 
		0x0c780000, 
		0x00000000, 
		0x376730a9, 
		0x0c7c0000, 
		0x0f7fffff, 
		0x780c028f, 
		0x80000000, 
		0x71111c64, 
		0x8248d48e, 
		0x00200000, 
		0x46627f3c, 
		0x00100000, 
		0x80004000, 
		0x00800001, 
		0x9be7b3ef, 
		0x80001000, 
		0x0d000ff0, 
		0x7fbfffff, 
		0x00000080, 
		0x424c0000, 
		0xffbfffff, 
		0x0e00007f, 
		0x30b53345, 
		0x7f7ffffe, 
		0x00800001, 
		0x42200000, 
		0x391fec8d, 
		0x80000040, 
		0x0c7fff80, 
		0x3f800001, 
		0x2383948a, 
		0x72b13102, 
		0x00000000, 
		0x00000000, 
		0xff800000, 
		0x00000000, 
		0x8f7ffffc, 
		0x530b6f26, 
		0x80000000, 
		0x42600000, 
		0x000aab72, 
		0x007ffffe, 
		0x0c7fe000, 
		0x80800003, 
		0xc2480000, 
		0x4d1fd0a5, 
		0xff9e7d5b, 
		0x41200000, 
		0x4fb28351, 
		0x80800002, 
		0xb7e52efb, 
		0x6ccb74bd, 
		0x2dd1f042, 
		0xb87ba9d0, 
		0x7f800000, 
		0x00001000, 
		0xcf500875, 
		0x7e230565, 
		0xc1200000, 
		0xc2740000, 
		0x7f7ffffe, 
		0x0c7ff000, 
		0xc1c80000, 
		0x00ffffff, 
		0x424c0000, 
		0x0c7fff00, 
		0xc25c0000, 
		0x36e25152, 
		0x80000001, 
		0xc1400000, 
		0xff7ffffe, 
		0xce968983, 
		0x425c0000, 
		0xa576a71c, 
		0x427c0000, 
		0xffbfffff, 
		0x00000040, 
		0x80000000, 
		0x0c7f0000, 
		0x80767586, 
		0xff9ab51f, 
		0x00000200, 
		0x0c7ffff8, 
		0x40c00000, 
		0x51b7e4e4, 
		0x42280000, 
		0x80020000, 
		0x0c7f8000, 
		0x80800000, 
		0x83fb2f81, 
		0x0e000fff, 
		0x40a00000, 
		0x40a00000, 
		0xc2640000, 
		0x541b600c, 
		0xff615b4b, 
		0xc1a00000, 
		0x41900000, 
		0xd0e524aa, 
		0x80000001, 
		0xe26ceceb, 
		0x0e00003f, 
		0x765ec15b, 
		0x08479947, 
		0x0a57ebbd, 
		0xff7ffffe, 
		0x422c0000, 
		0x294fe9e5, 
		0x00000001, 
		0x7f800000, 
		0x6b4002c3, 
		0x00000000, 
		0x52c5b809, 
		0x7fc00001, 
		0x42680000, 
		0x03453fec, 
		0xc25c0000, 
		0x0c7ffff0, 
		0x28e2c78b, 
		0x00010000, 
		0x42700000, 
		0x9e387ed9, 
		0xb6f88f88, 
		0x7f822770, 
		0xc9d7ff0e, 
		0x294d27a7, 
		0x80000002, 
		0x00000002, 
		0x6407d70b, 
		0x4b000000, 
		0x41300000, 
		0x72e33589, 
		0xad8364c1, 
		0x8b15e7bb, 
		0x80040000, 
		0x8026da2d, 
		0x80000000, 
		0x42100000, 
		0x7597e486, 
		0x0c7ffffc, 
		0x80000020, 
		0x0c7ffffc, 
		0x8000e475, 
		0x0c7ffe00, 
		0x80000001, 
		0x80800000, 
		0x0c7ffff8, 
		0xbf800000, 
		0x4d9b033e, 
		0x3f028f5c, 
		0xcb000000, 
		0x42640000, 
		0x80000000, 
		0x7fe9bc21, 
		0x52f64e54, 
		0xffd01b4b, 
		0xc1100000, 
		0x8b2b04c1, 
		0x55946313, 
		0x80200000, 
		0x80020000, 
		0x80000080, 
		0x00000000, 
		0x8f7fffff, 
		0xc1980000, 
		0x7fca3018, 
		0xc26c0000, 
		0x80000001, 
		0xc1a00000, 
		0x7f800000, 
		0x0d000ff0, 
		0x44e44573, 
		0x80000000, 
		0x42600000, 
		0xd57d366e, 
		0x00004000, 
		0xc2500000, 
		0xc2440000, 
		0x41d00000, 
		0xff800000, 
		0xc1a80000, 
		0x3315d1a8, 
		0xbbda6c74, 
		0xc1b80000, 
		0x851c9410, 
		0x42140000, 
		0x7f800000, 
		0xc2600000, 
		0x6510afdb, 
		0xc27c0000, 
		0xa854d28d, 
		0x00010000, 
		0xff7ffffe, 
		0x42340000, 
		0x0e007fff, 
		0xff800000, 
		0x421c0000, 
		0xff7fffff, 
		0x5385cb75, 
		0x80000000, 
		0xc23c0000, 
		0x80001000, 
		0xa200c59e, 
		0x80000200, 
		0x1f1662fd, 
		0x00000000, 
		0x42640000, 
		0x3032b5b8, 
		0xff7ffffe, 
		0x80000002, 
		0x5e10b39f, 
		0x80000000, 
		0x80000020, 
		0xc0a00000, 
		0xc2440000, 
		0x8de5f14f, 
		0x80000000, 
		0x00080000, 
		0x0c7ffffe, 
		0xc2140000, 
		0x41a00000, 
		0xc0800000, 
		0x0c780000, 
		0x00000002, 
		0xc946e552, 
		0x41a00000, 
		0x35ce730a, 
		0x0d000ff0, 
		0x42f9dee4, 
		0xcb8c4b40, 
		0x80000010, 
		0x426c0000, 
		0xc1d00000, 
		0x41300000, 
		0x00000000, 
		0x80008000, 
		0xff800000, 
		0x0e0000ff, 
		0x00000000, 
		0x80000008, 
		0xc1500000, 
		0x0e003fff, 
		0xff7ffffe, 
		0x7f800000, 
		0x80800000, 
		0x40800000, 
		0x7fc00000, 
		0x7d9c56af, 
		0xc1c80000, 
		0x80002000, 
		0x2d3bc373, 
		0x0e000001, 
		0x42740000, 
		0x54088af3, 
		0xedbd845b, 
		0x80ffffff, 
		0x00200000, 
		0x00000001, 
		0x0e01ffff, 
		0xff800000, 
		0x0e001fff, 
		0x9dab4b7a, 
		0xc1f00000, 
		0x38f0738a, 
		0x41400000, 
		0xbf800000, 
		0x80000002, 
		0x7f800000, 
		0x00000080, 
		0x41900000, 
		0x8f7fffff, 
		0x80000001, 
		0x0c7fe000, 
		0xca75caff, 
		0xff7fffff, 
		0x421c0000, 
		0x80001000, 
		0x42200000, 
		0x80800000, 
		0xc1500000, 
		0x9b9b91c1, 
		0x0c7ffffc, 
		0x80000080, 
		0xbf800001, 
		0x42000000, 
		0x00000000, 
		0x7f7fffff, 
		0x6ee09529, 
		0x00080000, 
		0xf03f0849, 
		0x0e000003, 
		0x41300000, 
		0xc2280000, 
		0x427c0000, 
		0x80200000, 
		0x0e118182, 
		0x0c7fe000, 
		0x0e000fff, 
		0x0e0007ff, 
		0x41e00000, 
		0x80800002, 
		0x33333333, 
		0x00004000, 
		0xc1880000, 
		0x3f800001, 
		0x0e007fff, 
		0x0f248b7d, 
		0xc2040000, 
		0xc2700000, 
		0x42780000, 
		0x80000000, 
		0xff7fffff, 
		0x42140000, 
		0x00400000, 
		0x00800000, 
		0x9578a6a2, 
		0xc1f00000, 
		0x0e001fff, 
		0x80000000, 
		0x80000000, 
		0x00000000, 
		0x1c7b0b2d, 
		0x82b46051, 
		0xb798f2b6, 
		0xff905d37, 
		0x6a59dd0b, 
		0x00000100, 
		0x42640000, 
		0x42240000, 
		0x3b8cd1f9, 
		0x426c0000, 
		0x40400000, 
		0xd7cab883, 
		0x00000000, 
		0xc97c6cdb, 
		0x7fadc25c, 
		0x71c18a87, 
		0xde5d1fd4, 
		0x2b8d57b2, 
		0xf73216e4, 
		0x41800000, 
		0xcccccccc, 
		0x80000040, 
		0x54faa6a5, 
		0xc2500000, 
		0xa3c74b97, 
		0x298ea68f, 
		0x80011111, 
		0xfa77d266, 
		0x7f7ffffe, 
		0x00000020, 
		0x7f000000, 
		0xfee3c162, 
		0x40e00000, 
		0x41f80000, 
		0xff7ffffe, 
		0x178c0f8f, 
		0xb6c896df, 
		0x00400000, 
		0xc21c0000, 
		0xff800000, 
		0xc2600000, 
		0x41800000, 
		0x42180000, 
		0xc75240ae, 
		0xff800000, 
		0x007fffff, 
		0x42380000, 
		0x932cd32d, 
		0x00000000, 
		0x41c00000, 
		0xc27c0000, 
		0x80800000, 
		0x00000001, 
		0x00000000, 
		0x80011111, 
		0x26bd68c7, 
		0x26165f84, 
		0x80000004, 
		0x550994ac, 
		0x621551a1, 
		0x0c400000, 
		0x95ae2c06, 
		0x0c7ffffc, 
		0x2bd3642d, 
		0x0c600000, 
		0xc0e00000, 
		0xff7ffffe, 
		0x41d00000, 
		0x40e00000, 
		0x277ea436, 
		0xff7ffffe, 
		0x0e00ffff, 
		0x42740000, 
		0x3d353149, 
		0x80800001, 
		0xff800000, 
		0x0e003fff, 
		0x00000000, 
		0x5b9b7033, 
		0xf3414cef, 
		0x80000800, 
		0xc1400000, 
		0x41f80000, 
		0x0c7f0000, 
		0x11ed8676, 
		0x41a00000, 
		0x2350d235, 
		0x41e00000, 
		0xffbf7ca7, 
		0xa5e816bc, 
		0x458fb5e2, 
		0x8f7ffffe, 
		0xe7493fe3, 
		0x42480000, 
		0x00000000, 
		0x0c7ff000, 
		0x42440000, 
		0xff000000, 
		0x4b8c4b40, 
		0xdfd50ae6, 
		0x290c7622, 
		0x00800001, 
		0x00000200, 
		0xbeae8992, 
		0xc2700000, 
		0x009e63aa, 
		0x0c7c0000, 
		0x3bdc7a57, 
		0x80ffffff, 
		0x0c7ffc00, 
		0xc2200000, 
		0x80000000, 
		0x41a80000, 
		0xc26c0000, 
		0x0003c564, 
		0xff7ffffe, 
		0xfe265809, 
		0x42080000, 
		0xffc00000, 
		0xc2100000, 
		0x41d00000, 
		0x00008000, 
		0x2bfe18bf, 
		0xff7ffffe, 
		0x41c80000, 
		0x0e001fff, 
		0x0c7ff800, 
		0x42500000, 
		0x0c7ffffe, 
		0x7f7ffffe, 
		0x7f9791b8, 
		0x7f9dcae1, 
		0x42500000, 
		0x3661117e, 
		0x3f800000, 
		0x404b5613, 
		0x0e03ffff, 
		0xf1581e02, 
		0x7f7ffffe, 
		0x41c00000, 
		0x42580000, 
		0x00000001, 
		0x0c7fe000, 
		0x00000000, 
		0x0c7fffe0, 
		0x0c7ffff8, 
		0x0c7ffffe, 
		0xc2600000, 
		0x42340000, 
		0x7f7ffffe, 
		0x7f800000, 
		0x7f800000, 
		0xc23c0000, 
		0x491d8cc7, 
		0x00000004, 
		0xc22c0000, 
		0xca662fdf, 
		0x41500000, 
		0xff800001, 
		0x171dc9d2, 
		0xb7aefa0c, 
		0x80000080, 
		0x40000000, 
		0xb299112a, 
		0x80800000, 
		0xffc00001, 
		0x3b67e751, 
		0x42280000, 
		0xedcb6f91, 
		0xb0fd05d8, 
		0x807fffff, 
		0x00800002, 
		0x41f80000, 
		0xc2180000, 
		0x19d29a5c, 
		0x80000000, 
		0xc1d00000, 
		0x780e2e92, 
		0xffc00000, 
		0xff800000, 
		0x7f800000, 
		0xc2200000, 
		0xf7324fa2, 
		0x90b5f015, 
		0xc2440000, 
		0x00020000, 
		0x80000000, 
		0x42600000, 
		0xc1d00000, 
		0x80001000, 
		0x0c7fe000, 
		0x6362e01b, 
		0x42140000, 
		0x80000000, 
		0x42740000, 
		0x41e80000, 
		0xbb152322, 
		0x49b3e11a, 
		0x179c8cbc, 
		0x00000000, 
		0x42100000, 
		0x0c7ffff0, 
		0x0e00001f, 
		0x00000000, 
		0x41200000, 
		0xbf800000, 
		0x00000001, 
		0xff7fffff, 
		0x00000000, 
		0xf85add18, 
		0xfed88e65, 
		0xe99a0004, 
		0x41b00000, 
		0x42400000, 
		0x00002000, 
		0x4ea7b039, 
		0x42300000, 
		0x4b000000, 
		0x00000100, 
		0xa3c2a9e3, 
		0x807fffff, 
		0x7fc00000, 
		0xc2640000, 
		0x6a402c14, 
		0x893caf2e, 
		0x0e000001, 
		0x80000800, 
		0x00000000, 
		0x80000000, 
		0x41a00000, 
		0x0e03ffff, 
		0x0a6f2ee3, 
		0x80000000, 
		0x00800001, 
		0xc3419343, 
		0x8f7ffffe, 
		0x00000000, 
		0xffffffff, 
		0x7f967a12, 
		0x917061e1, 
		0x80000000, 
		0x80000000, 
		0xc1a00000, 
		0xc1a00000, 
		0x7f800000, 
		0xff800000, 
		0xc0cd70dc, 
		0xff9ccc67, 
		0xff800000, 
		0x40400000, 
		0x80080000, 
		0x0e00ffff, 
		0xbfc9113c, 
		0x00800002, 
		0xff800000, 
		0x8055d9cf, 
		0x00000000, 
		0x5d32123a, 
		0xff800000, 
		0x7fa84ffc, 
		0xffffffff, 
		0x80800003, 
		0xb4fc2185, 
		0x42240000, 
		0x00010000, 
		0xe9881fcf, 
		0x42280000, 
		0xebb6168f, 
		0xc2340000, 
		0xc2100000, 
		0x0015fd10, 
		0x59d1bdea, 
		0xff800000, 
		0x7fc00000, 
		0x00004000, 
		0xc2380000, 
		0xffc00000, 
		0x0e00003f, 
		0x80000000, 
		0x7fa45ed1, 
		0x42200000, 
		0x7f7ffffe, 
		0x41d80000, 
		0x7f800000, 
		0x986ffb8d, 
		0x42780000, 
		0x15eaba6b, 
		0x0c7fc000, 
		0x86d70fef, 
		0x00800001, 
		0xff800000, 
		0xc2580000, 
		0x95a3ae0e, 
		0xcb8c6d89, 
		0x80800001, 
		0x0e000fff, 
		0xff800000, 
		0x0c7e0000, 
		0x00008000, 
		0x0c7fff80, 
		0x7f800001, 
		0xbf800001, 
		0xff800000, 
		0x00040000, 
		0xdbd1a07f, 
		0xbf800000, 
		0x803ed447, 
		0x80000000, 
		0xc1d00000, 
		0xc1000000, 
		0x40000000, 
		0xc0e00000, 
		0xb1e0a242, 
		0x0e000fff, 
		0x807fffff, 
		0xff800000, 
		0x41980000, 
		0x80000008, 
		0x40e00000, 
		0xc2480000, 
		0x7d3ba914, 
		0x41300000, 
		0x80000008, 
		0x7373dd01, 
		0x0e003fff, 
		0x80100000, 
		0x00400000, 
		0x00000000, 
		0x7b469823, 
		0xc2580000, 
		0x00800003, 
		0x0e0003ff, 
		0xc27c0000, 
		0x0e0fffff, 
		0xf810f1d2, 
		0xff800000, 
		0x42400000, 
		0x4d6677b3, 
		0x7f7ffffe, 
		0x42180000, 
		0x80000000, 
		0x7f800000, 
		0x80000008, 
		0xcccccccc, 
		0x55555555, 
		0x7f800001, 
		0x00800001, 
		0xc2380000, 
		0x80000000, 
		0xc26c0000, 
		0x00000000, 
		0x41d80000, 
		0xa45a7882, 
		0xffe58d46, 
		0x0d00fff0, 
		0x5f629756, 
		0xc2000000, 
		0x80020000, 
		0x45fa148a, 
		0x1457b4bf, 
		0x80000000, 
		0x50a142d6, 
		0x80100000, 
		0xff800000, 
		0xc2780000, 
		0x734fa46e, 
		0x5610f6e4, 
		0x42100000, 
		0x41c00000, 
		0x0c7ffff0, 
		0x80000002, 
		0x42700000, 
		0x41800000, 
		0x7cfa791f, 
		0xff800000, 
		0x40400000, 
		0xc2500000, 
		0x80000000, 
		0x00000000, 
		0x899f8772, 
		0x9acd907e, 
		0x0a1f22ba, 
		0x41500000, 
		0x8cb354ad, 
		0x00020000, 
		0x42480000, 
		0xb71d0dd9, 
		0xcb8c4b40, 
		0xc1000000, 
		0xcb000000, 
		0x00000000, 
		0x7f9df9ef, 
		0x7fc00001, 
		0x0433d3d9, 
		0x8f7fffff, 
		0x80000001, 
		0x7f800000, 
		0xc8120cd7, 
		0x42200000, 
		0x0c780000, 
		0xff7ffffe, 
		0xc2000000, 
		0x02b75b0c, 
		0x00000000, 
		0x80000001, 
		0x4426a335, 
		0x41d80000, 
		0x0ac16527, 
		0x0e3fffff, 
		0xff800000, 
		0xc1880000, 
		0x7fc00001, 
		0x234fcaed, 
		0x00800000, 
		0xc0c00000, 
		0x80000000, 
		0x0e000001, 
		0xc1f80000, 
		0x007fffff, 
		0x00002000, 
		0x0e000007, 
		0x00000000, 
		0x3e88ee83, 
		0xc1d00000, 
		0x7f7ffffe, 
		0xc2080000, 
		0xc2740000, 
		0x0c7c0000, 
		0x80800001, 
		0x42500000, 
		0x00000001, 
		0xe2788009, 
		0x40000000, 
		0x0e1fffff, 
		0x0c7fff80, 
		0x7f800000, 
		0x80008000, 
		0xc2140000, 
		0xc2612746, 
		0x0d00fff0, 
		0x0c7fff80, 
		0x41b80000, 
		0xc33bcf91, 
		0x00000000, 
		0x15d5c6b0, 
		0xc1000000, 
		0x00000200, 
		0x0f7fffff, 
		0x00000001, 
		0x978ea366, 
		0x0c7ffc00, 
		0x427c0000, 
		0x300c5877, 
		0x87807c70, 
		0x80100000, 
		0x80000004, 
		0x496b6dc9, 
		0x7f800000, 
		0x7f7ffffe, 
		0x0e000001, 
		0x2f6b7432, 
		0xbf800000, 
		0x45af0a5a, 
		0x41600000, 
		0x561db87c, 
		0x33333333, 
		0x42180000, 
		0x00000040, 
		0xc2000000, 
		0x0e1fffff, 
		0xaaaaaaaa, 
		0xba4c27bb, 
		0x7f800000, 
		0x7f800000, 
		0x80000800, 
		0x00000000, 
		0x9baef857, 
		0xc2540000, 
		0x397186f4, 
		0xff800001, 
		0x437d5ebd, 
		0x80800002, 
		0xff800000, 
		0x42700000, 
		0x8013bd5d, 
		0xffb1746f, 
		0x7f7ffffe, 
		0x41f00000, 
		0x0e0000ff, 
		0x7f7ffffe, 
		0x41d00000, 
		0x0c7ff000, 
		0x42000000, 
		0x00034bb1, 
		0xbf800000, 
		0x8065fdc9, 
		0x0e000001, 
		0xc1000000, 
		0x4d6f5bc1, 
		0x00ffffff, 
		0x4cd35694, 
		0x8f7ffffe, 
		0x00000010, 
		0x421c0000, 
		0x42200000, 
		0xcb000000, 
		0xc2240000, 
		0x80800000, 
		0x41e00000, 
		0x80000000, 
		0x7525920c, 
		0xb331f830, 
		0xef2214ef, 
		0x7fa965fe, 
		0x0e0fffff, 
		0x0c7f8000, 
		0x80000001, 
		0x80000000, 
		0x0e0fffff, 
		0x41a80000, 
		0xcccccccc, 
		0xc2700000, 
		0x42080000, 
		0xc1400000, 
		0x00000000, 
		0x4943f87a, 
		0x42340000, 
		0x7f7ffffe, 
		0xc2540000, 
		0xc2380000, 
		0x41100000, 
		0x80000001, 
		0xccd7865f, 
		0x80000000, 
		0x7f800000, 
		0x00004000, 
		0x80011111, 
		0x80000400, 
		0x80000000, 
		0xff7ffffe, 
		0x00800001, 
		0x0c7f8000, 
		0x0e007fff, 
		0x800f69cf, 
		0x80000008, 
		0x80000020, 
		0x003b56e4, 
		0x51ab99c4, 
		0x42540000, 
		0x55555555, 
		0x7fa5afb2, 
		0x0e000001, 
		0x59b7af5d, 
		0x42240000, 
		0x73deb0ce, 
		0x7f800000, 
		0x30f242e2, 
		0x41e80000, 
		0x0c7ffff8, 
		0xc2700000, 
		0x162304e6, 
		0x00000200, 
		0xea86dafd, 
		0x0fdf5038, 
		0x4f528cfc, 
		0x00011111, 
		0x80000000, 
		0x0e0007ff, 
		0xf038bc4b, 
		0xc2380000, 
		0x7fb1a28f, 
		0xff800000, 
		0x0e0007ff, 
		0xc2080000, 
		0x80000001, 
		0x7f7ffffe, 
		0xffb98ba8, 
		0x00000004, 
		0x80000000, 
		0xc0000000, 
		0x42040000, 
		0x007fffff, 
		0x935778e2, 
		0x42740000, 
		0x80000000, 
		0x7f7ffffe, 
		0x007ffffe, 
		0x86cd519b, 
		0x828868e1, 
		0x00800000, 
		0x62e2b5ab, 
		0xc1e00000, 
		0xae7581a1, 
		0x7f7ffffe, 
		0x000c36f1, 
		0x3c51de54, 
		0xff7ffffe, 
		0x46fc62d0, 
		0x0d000ff0, 
		0xf1a67cd6, 
		0x00000000, 
		0x0d00fff0, 
		0x41b00000, 
		0xff800000, 
		0x42780000, 
		0x8c43043a, 
		0xe224987b, 
		0x7fa59950, 
		0x97833900, 
		0x7b2fad5d, 
		0x007ffffe, 
		0x80000000, 
		0xec3dd2e1, 
		0x41a80000, 
		0x8f7ffffc, 
		0xeae32223, 
		0xc2440000, 
		0x00000001, 
		0xff800000, 
		0xc27c0000, 
		0x80000040, 
		0x9b4c0e3e, 
		0x80800002, 
		0xe950519a, 
		0x7f7fffff, 
		0xcaf5a9e1, 
		0x80800002, 
		0x043268c3, 
		0x80ffffff, 
		0x7fbfffff, 
		0x440261a8, 
		0x00000002, 
		0xc1900000, 
		0x41e80000, 
		0x0f7ffffd, 
		0x7fbedcea, 
		0x3eac2f3d, 
		0x0c7fe000, 
		0xf808c386, 
		0x00800000, 
		0xd522154e, 
		0x42640000, 
		0xff7fffff, 
		0x0c780000, 
		0x42780000, 
		0xc2740000, 
		0xc1e00000, 
		0x7fc00001, 
		0x568b4093, 
		0xcccccccc, 
		0xa49ec86c, 
		0xf7b52f1a, 
		0x768da1a7, 
		0x41f80000, 
		0x0d000ff0, 
		0x42680000, 
		0x00000000, 
		0x14779134, 
		0x80000000, 
		0xff800000, 
		0x7f800000, 
		0x41900000, 
		0x427c0000, 
		0x80000000, 
		0xc2480000, 
		0xb9063709, 
		0xc2780000, 
		0xff800000, 
		0xb79eae42, 
		0x8f7ffffe, 
		0x80000000, 
		0x80800001, 
		0xceae4290, 
		0xff800000, 
		0x41d80000, 
		0x0e003fff, 
		0x00000008, 
		0x80800000, 
		0x41500000, 
		0x0052ca19, 
		0x49930f5a, 
		0x42580000, 
		0xc1e80000, 
		0xfdbeec83, 
		0xdd910900, 
		0x65985e39, 
		0x7f000000, 
		0x6b8a1ea4, 
		0x42280000, 
		0x80000800, 
		0xffc00001, 
		0xff800000, 
		0xcab172e3, 
		0x5be57979, 
		0x8c5580f2, 
		0xc1400000, 
		0x00004000, 
		0x80000000, 
		0x00000002, 
		0x42300000, 
		0xaf3c9e33, 
		0x6c622752, 
		0x80800003, 
		0x0f7ffffe, 
		0x42680000, 
		0xff7ffffe, 
		0xc27c0000, 
		0xff800000, 
		0xae78e4c8, 
		0xed24bfd5, 
		0xc25c0000, 
		0x1107c8bd, 
		0x8f7ffffd, 
		0x87b671b7, 
		0xda41ac8b, 
		0x23d66256, 
		0x41900000, 
		0x43cd9106, 
		0x00000000, 
		0xade9c5cb, 
		0xff800000, 
		0x80000000, 
		0xc1d80000, 
		0x2ef5c6b8, 
		0x00000000, 
		0x42080000, 
		0x3e82821e, 
		0x0c7fffc0, 
		0x80100000, 
		0x80800002, 
		0x00000000, 
		0x0132f83f, 
		0x0e003fff, 
		0x0e7fffff, 
		0x8cf3baed, 
		0x80800001, 
		0x2543c394, 
		0xe66c0c1e, 
		0x7fc00001, 
		0x95ec9d07, 
		0x80000000, 
		0x40e00000, 
		0xc8a73cbd, 
		0x6d0b463d, 
		0x53904cdc, 
		0x00000000, 
		0x80800000, 
		0x00000000, 
		0x3a4a2c73, 
		0xdacdc206, 
		0x00800001, 
		0x0c7ffffe, 
		0x80000000, 
		0x80000800, 
		0x42400000, 
		0x72158473, 
		0x1a657a53, 
		0x423c0000, 
		0xffef5919, 
		0x7f800000, 
		0xc2740000, 
		0x41980000, 
		0x60631d18, 
		0x53e7ef02, 
		0xbb74b4d9, 
		0x1d9eb71a, 
		0x80000000, 
		0x80800000, 
		0xc1d80000, 
		0x0e00ffff, 
		0xdcdd2038, 
		0x80000000, 
		0x7f800000, 
		0x00000000, 
		0x0c7ffffe, 
		0x3f800000, 
		0x00000002, 
		0x41200000, 
		0x002c4e45, 
		0xcfdd7b4d, 
		0x41c80000, 
		0xc1e80000, 
		0x805d1ee3, 
		0x00000002, 
		0x424c0000, 
		0xd18bf385  
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
		0x000001cb, 
		0x000003a3, 
		0x00000161, 
		0x000000bf, 
		0x00000011, 
		0x00000033, 
		0x00000275, 
		0x00000373, 
		0x000003c5, 
		0x00000207, 
		0x0000001b, 
		0x000001b1, 
		0x00000291, 
		0x000001eb, 
		0x00000149, 
		0x000002bd, 
		0x00000305, 
		0x0000018d, 
		0x000001d1, 
		0x000003bd, 
		0x0000031d, 
		0x00000213, 
		0x00000151, 
		0x000002c7, 
		0x0000020d, 
		0x000003ed, 
		0x0000007b, 
		0x00000323, 
		0x000001b9, 
		0x00000323, 
		0x0000027b, 
		0x0000036b, 
		0x000000e1, 
		0x000001e7, 
		0x00000181, 
		0x00000099, 
		0x00000107, 
		0x0000034d, 
		0x000001f9, 
		0x00000239, 
		0x00000315, 
		0x000001ff, 
		0x0000031b, 
		0x00000301, 
		0x00000071, 
		0x000003f7, 
		0x000000bd, 
		0x00000025, 
		0x000001f7, 
		0x00000041, 
		0x0000037b, 
		0x000002af, 
		0x0000004f, 
		0x000000af, 
		0x0000029d, 
		0x000003bf, 
		0x00000219, 
		0x00000085, 
		0x000000ff, 
		0x000003c1, 
		0x000002ad, 
		0x00000319, 
		0x0000038d, 
		0x0000012f, 
		0x00000077, 
		0x000001db, 
		0x0000015f, 
		0x000002f7, 
		0x00000077, 
		0x000000e9, 
		0x000001d3, 
		0x000002bb, 
		0x00000207, 
		0x000001d3, 
		0x00000191, 
		0x000000c7, 
		0x00000099, 
		0x00000107, 
		0x00000181, 
		0x00000335, 
		0x000001b5, 
		0x000002bd, 
		0x00000211, 
		0x0000013d, 
		0x0000021f, 
		0x000002d3, 
		0x0000031f, 
		0x0000001f, 
		0x00000299, 
		0x000002a1, 
		0x0000006b, 
		0x000001b3, 
		0x0000015b, 
		0x00000129, 
		0x000003e3, 
		0x00000307, 
		0x000001df, 
		0x0000033d, 
		0x000001f5, 
		0x00000369, 
		0x00000101, 
		0x0000037d, 
		0x000000fd, 
		0x00000247, 
		0x00000215, 
		0x00000333, 
		0x0000032f, 
		0x000000f3, 
		0x0000022d, 
		0x000002e7, 
		0x000000cf, 
		0x00000285, 
		0x0000036f, 
		0x00000373, 
		0x0000021f, 
		0x000000c7, 
		0x00000357, 
		0x000001d5, 
		0x00000057, 
		0x0000029d, 
		0x0000000d, 
		0x000000c5, 
		0x00000177, 
		0x000003dd, 
		0x0000028d, 
		0x0000012f, 
		0x0000000b, 
		0x0000010d, 
		0x00000313, 
		0x0000018b, 
		0x000002d3, 
		0x00000247, 
		0x0000009b, 
		0x000003c7, 
		0x000000d5, 
		0x0000025d, 
		0x00000013, 
		0x000000f9, 
		0x000001b3, 
		0x000000d7, 
		0x000003ad, 
		0x000002db, 
		0x000001cd, 
		0x000002b5, 
		0x000000bb, 
		0x00000171, 
		0x0000015b, 
		0x0000028f, 
		0x0000008b, 
		0x0000017b, 
		0x0000010d, 
		0x00000309, 
		0x000003f7, 
		0x00000229, 
		0x000000a5, 
		0x0000039b, 
		0x000002bf, 
		0x0000036b, 
		0x00000261, 
		0x000000e3, 
		0x00000353, 
		0x00000151, 
		0x0000039b, 
		0x0000029f, 
		0x000003fd, 
		0x00000047, 
		0x00000297, 
		0x0000001f, 
		0x0000019f, 
		0x0000029b, 
		0x000002b9, 
		0x00000389, 
		0x00000029, 
		0x00000393, 
		0x00000059, 
		0x00000161, 
		0x0000010d, 
		0x0000014d, 
		0x000003c7, 
		0x000001c9, 
		0x000001d9, 
		0x00000011, 
		0x00000321, 
		0x00000121, 
		0x0000023b, 
		0x0000023d, 
		0x00000089, 
		0x000000e3, 
		0x000001db, 
		0x0000038f, 
		0x000001ed, 
		0x0000005d, 
		0x00000059, 
		0x00000097, 
		0x000000f1, 
		0x0000024f, 
		0x00000069, 
		0x00000289, 
		0x00000057, 
		0x000002c5, 
		0x0000019d, 
		0x00000383, 
		0x00000039, 
		0x00000109, 
		0x0000037f, 
		0x0000002d, 
		0x00000263, 
		0x0000017f, 
		0x0000034f, 
		0x000003a1, 
		0x00000151, 
		0x000000ef, 
		0x000000c1, 
		0x00000391, 
		0x000003f5, 
		0x00000269, 
		0x000001bf, 
		0x00000341, 
		0x00000005, 
		0x00000281, 
		0x000002fb, 
		0x000003ab, 
		0x00000283, 
		0x0000037f, 
		0x0000015b, 
		0x00000229, 
		0x000003a1, 
		0x0000015f, 
		0x000002d7, 
		0x00000223, 
		0x000002b3, 
		0x0000026f, 
		0x000003db, 
		0x0000014d, 
		0x00000147, 
		0x00000273, 
		0x000001eb, 
		0x00000281, 
		0x0000030f, 
		0x0000037b, 
		0x000001cd, 
		0x00000265, 
		0x00000255, 
		0x00000161, 
		0x00000353, 
		0x00000237, 
		0x0000002f, 
		0x0000023f, 
		0x00000391, 
		0x0000024b, 
		0x000001cf, 
		0x000002c9, 
		0x00000143, 
		0x00000169, 
		0x000002e1, 
		0x000003bf  
};

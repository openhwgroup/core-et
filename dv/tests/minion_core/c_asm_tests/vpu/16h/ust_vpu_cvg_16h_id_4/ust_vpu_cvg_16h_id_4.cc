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
			"li x5, 481\n"
			"faddi.pi f2, f0, -415\n"
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
			"faddi.pi f2, f2, 128\n"
			"4:\n"
			"frcp.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"bne x0, x0, 4b\n"
			"fsub.ps f28, f27, f24\n"
			"fmul.pi f23, f30, f23\n"
			"fsub.ps f22, f31, f21\n"
			"fmin.ps f31, f20, f24\n"
			"fmul.pi f20, f19, f22\n"
			"fsub.pi f31, f24, f25\n"
			"fmul.pi f31, f27, f24\n"
			"fadd.pi f25, f19, f31\n"
			"1:\n"
			"frcp.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"bne x0, x0, 1b\n"
			"fmadd.ps f25, f24, f30, f25\n"
			"fmadd.ps f26, f21, f24, f24\n"
			"fmsub.ps f24, f24, f21, f25\n"
			"fnmadd.ps f26, f25, f23, f22\n"
			"fmsub.ps f26, f31, f28, f29\n"
			"fmsub.ps f23, f20, f25, f21\n"
			"frcp.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"frcp.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"frcp.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"fmax.pi f24, f24, f31\n"
			"fsub.pi f28, f28, f28\n"
			"fmin.ps f26, f30, f27\n"
			"frcp.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"frcp.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"fnmadd.ps f29, f28, f28, f26\n"
			"fnmsub.ps f25, f31, f30, f25\n"
			"fmsub.ps f28, f27, f29, f25\n"
			"frcp.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"frcp.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"fsub.ps f29, f30, f29\n"
			"fmin.pi f28, f31, f28\n"
			"fmin.ps f28, f27, f30\n"
			"fand.pi f27, f30, f30\n"
			"fsub.ps f31, f30, f31\n"
			"fmul.ps f27, f29, f31\n"
			"1:\n"
			"frcp.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"bne x0, x0, 1b\n"
			"frcp.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"fnmsub.ps f29, f31, f30, f29\n"
			"fmadd.ps f29, f30, f31, f31\n"
			"fmadd.ps f29, f29, f31, f29\n"
			"fmsub.ps f29, f31, f30, f30\n"
			"fmsub.ps f31, f31, f29, f31\n"
			"fmsub.ps f30, f31, f31, f30\n"
			"fnmsub.ps f29, f30, f31, f30\n"
			"fmadd.ps f29, f29, f30, f31\n"
			"frcp.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"4:\n"
			"frcp.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"bne x0, x0, 4b\n"
			"fadd.ps f31, f31, f31\n"
			"fsub.pi f31, f31, f31\n"
			"fadd.ps f31, f31, f31\n"
			"fadd.pi f31, f31, f31\n"
			"fadd.ps f31, f31, f31\n"
			"frcp.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"frcp.ps f1, f17\n"
			"faddi.pi f17, f17, 128\n"
			"fnmadd.ps f10, f5, f2, f7\n"
			"fnmadd.ps f30, f7, f17, f31\n"
			"fmsub.ps f13, f23, f13, f28\n"
			"fnmadd.ps f17, f28, f11, f27\n"
			"fnmsub.ps f9, f9, f16, f2\n"
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
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
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
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
			"li x0, 0x1f4\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x6\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 96(x19)\n"
			"flw.ps f19, 64(x19)\n"
			"flw.ps f31, 800(x19)\n"
			"fnmsub.s f9, f26, f19, f31, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 320(x14)\n"
			"flw.ps f31, 608(x14)\n"
			"fminu.pi f21, f7, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 992(x23)\n"
			"fmul.ps f10, f30, f30, rdn\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 864(x17)\n"
			"frcp.ps f14, f9\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 704(x15)\n"
			"frcp.ps f12, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"maskand m6, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_0:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 448(x14)\n"
			"frcp.ps f9, f22\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 640(x17)\n"
			"fcvt.ps.pwu f31, f10, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_0:\n"
			"fltm.ps m2, f1, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 672(x15)\n"
			"flw.ps f17, 192(x15)\n"
			"flw.ps f26, 416(x15)\n"
			"fmadd.ps f14, f22, f17, f26, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_0:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f26, 224(x17)\n"
			"fsrli.pi f14, f26, 0xb\n" 
			"li x5, 0xb67f\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_int_32\n"
			"flw.ps f28, 576(x7)\n"
			"fminu.pi f26, f28, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 288(x17)\n"
			"flw.ps f30, 320(x17)\n"
			"fmax.ps f31, f19, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 192(x28)\n"
			"flw.ps f17, 224(x28)\n"
			"feq.ps f14, f19, f17\n" 
			"mova.x.m x19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x7, 0x1a000000007f0008\n"
			"csrr x6, mhartid\n"
			"srli x6, x6, 4\n"
			"slli x6, x6, 6\n"
			"add x7, x7, x6\n"
			"csrw tensor_reduce, x7\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_532994_HID_0:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 480(x19)\n"
			"frsq.ps f10, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 384(x19)\n"
			"flw.ps f12, 960(x19)\n"
			"feq.s x14, f31, f12\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 768(x7)\n"
			"fsat8.pi f22, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f22, 352(x19)\n"
			"flw.ps f18, 448(x19)\n"
			"fsll.pi f30, f22, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f18, 864(x23)\n"
			"fsatu8.pi f18, f18\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 256(x19)\n"
			"flw.ps f22, 288(x19)\n"
			"fmulhu.pi f31, f7, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 864(x12)\n"
			"fround.ps f11, f7, rtz\n" 
			"li x5, 0x8a3d\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f10, 32(x28)\n"
			"fcvt.ps.pw f18, f10, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 128(x14)\n"
			"fandi.pi f18, f18, 0x174\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 640(x22)\n"
			"fsgnjn.s f1, f22, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 864(x12)\n"
			"fcvt.pw.ps f11, f30, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_0:\n"
			"flem.ps m2, f17, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xa3d9\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f14, 768(x28)\n"
			"flw.ps f22, 384(x28)\n"
			"fmul.pi f31, f14, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 768(x28)\n"
			"fcvt.f16.ps f19, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 992(x7)\n"
			"flw.ps f7, 160(x7)\n"
			"fsgnjx.ps f18, f28, f7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 576(x19)\n"
			"flw.ps f30, 0(x19)\n"
			"fand.pi f28, f21, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x14, 0x36000000063c000a\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_861500_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 992(x14)\n"
			"flw.ps f28, 512(x14)\n"
			"flw.ps f12, 576(x14)\n"
			"fnmadd.s f30, f10, f28, f12, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 320(x22)\n"
			"flw.ps f19, 576(x22)\n"
			"feq.ps f14, f11, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 640(x28)\n"
			"flw.ps f28, 512(x28)\n"
			"feq.pi f28, f9, f28\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_0:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 192(x22)\n"
			"flog.ps f28, f21\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 320(x14)\n"
			"flw.ps f19, 928(x14)\n"
			"feq.pi f7, f14, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 736(x7)\n"
			"flw.ps f14, 800(x7)\n"
			"fle.ps f1, f12, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 608(x14)\n"
			"fcvt.pwu.ps f19, f11, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f11, 320(x22)\n"
			"flw.ps f9, 928(x22)\n"
			"frem.pi f9, f11, f9\n" 
			"li x5, 0x120797614b029163\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa921c803d2519feb\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"mova.x.m x17\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 928(x12)\n"
			"flw.ps f10, 576(x12)\n"
			"fsub.pi f22, f17, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 512(x19)\n"
			"flw.ps f22, 448(x19)\n"
			"fltu.pi f11, f9, f22\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_0:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_0:\n"
			"feqm.ps m6, f1, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x11, 0x668886927349a873\n"
			"fcvt.s.wu f10, x11, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_10_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 960(x11)\n"
			"flog.ps f30, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 864(x22)\n"
			"flw.ps f31, 640(x22)\n"
			"fsgnj.ps f18, f12, f31\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_993668_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7 << TENSOR_IMA_SCP_LOC_B) | (0xef << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_594103_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1 << TENSOR_IMA_SCP_LOC_B) | (0x78 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x7e << TENSOR_FMA32_SCP_LOC_B) | (0x19 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"fbci.pi f26, 0x4d79f\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 736(x28)\n"
			"flw.ps f14, 288(x28)\n"
			"fsgnjn.s f11, f7, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 672(x19)\n"
			"flw.ps f10, 288(x19)\n"
			"fmax.pi f14, f18, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 832(x14)\n"
			"flw.ps f1, 480(x14)\n"
			"fadd.s f26, f10, f1, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_0:\n"
			"maskand m7, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 832(x28)\n"
			"flw.ps f19, 320(x28)\n"
			"fmul.s f18, f7, f19, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 576(x28)\n"
			"fcvt.f16.ps f14, f28\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 448(x12)\n"
			"flw.ps f12, 576(x12)\n"
			"fminu.pi f21, f21, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 576(x12)\n"
			"flw.ps f21, 416(x12)\n"
			"fsgnj.s f28, f12, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 864(x14)\n"
			"flw.ps f10, 96(x14)\n"
			"fsgnjn.s f19, f31, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 224(x14)\n"
			"flw.ps f14, 992(x14)\n"
			"flw.ps f31, 576(x14)\n"
			"fnmsub.s f11, f28, f14, f31, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 544(x15)\n"
			"flw.ps f18, 416(x15)\n"
			"fxor.pi f28, f28, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 0(x12)\n"
			"flw.ps f1, 960(x12)\n"
			"flw.ps f17, 480(x12)\n"
			"fnmsub.ps f11, f31, f1, f17, rtz\n" 
			"li x7, 0x86bacde5440bb109\n"
			"fcvt.s.l f30, x7, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 288(x22)\n"
			"flw.ps f7, 32(x22)\n"
			"fmul.s f19, f26, f7, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 864(x22)\n"
			"flw.ps f28, 928(x22)\n"
			"fcmovm.ps f1, f31, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_58577_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xf4 << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 224(x7)\n"
			"ffrc.ps f9, f17\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 96(x19)\n"
			"flw.ps f30, 992(x19)\n"
			"fminu.pi f22, f19, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 832(x19)\n"
			"flw.ps f18, 736(x19)\n"
			"flw.ps f11, 832(x19)\n"
			"fcmov.ps f26, f12, f18, f11\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_0:\n"
			"masknot m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 640(x17)\n"
			"flw.ps f31, 128(x17)\n"
			"fadd.s f1, f1, f31, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_0:\n"
			"fltm.ps m5, f10, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 512(x11)\n"
			"flw.ps f28, 896(x11)\n"
			"fmax.ps f17, f31, f28\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_0:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_15_HID_0:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 256(x15)\n"
			"flog.ps f22, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 224(x15)\n"
			"flw.ps f12, 544(x15)\n"
			"fmulh.pi f1, f1, f12\n" 
			"maskpopcz x28, m6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 736(x22)\n"
			"flw.ps f21, 256(x22)\n"
			"fmin.ps f14, f26, f21\n" 
			"la x22, rand_int_32\n"
			"flw.ps f26, 320(x22)\n"
			"flw.ps f30, 480(x22)\n"
			"fmulh.pi f28, f26, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 512(x14)\n"
			"flw.ps f14, 768(x14)\n"
			"fadd.pi f18, f10, f14\n" 
			"li x7, 0x7782ea15a248496\n"
			"fcvt.s.w f10, x7, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x2c56\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f19, 320(x15)\n"
			"flw.ps f10, 352(x15)\n"
			"fadd.pi f7, f19, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 288(x28)\n"
			"fround.ps f28, f18, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_0:\n"
			"maskor m7, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_17_HID_0:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f10, 384(x17)\n"
			"fsat8.pi f22, f10\n" 
			"la x12, rand_ieee754_16\n"
			"flw.ps f14, 448(x12)\n"
			"fcvt.ps.f16 f21, f14\n" 
			"mova.x.m x19\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 288(x23)\n"
			"flw.ps f31, 544(x23)\n"
			"fsgnj.s f30, f14, f31\n" 
			"LBL_SEQID_5_M0_WRITE_ID_18_HID_0:\n"
			"masknot m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 992(x22)\n"
			"flw.ps f12, 544(x22)\n"
			"fle.s x15, f10, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 448(x19)\n"
			"flw.ps f9, 96(x19)\n"
			"flw.ps f30, 160(x19)\n"
			"fnmadd.ps f17, f10, f9, f30, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_19_HID_0:\n"
			"maskand m6, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 64(x28)\n"
			"flw.ps f21, 768(x28)\n"
			"flw.ps f14, 704(x28)\n"
			"fmadd.ps f22, f7, f21, f14, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_20_HID_0:\n"
			"maskor m2, m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 320(x19)\n"
			"flw.ps f28, 608(x19)\n"
			"flt.s x14, f14, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 96(x12)\n"
			"flw.ps f28, 352(x12)\n"
			"feq.s x12, f22, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x32 << TENSOR_FMA16_SCP_LOC_B) | (0xe1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_HPM_3_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f21, 992(x16)\n"
			"flw.ps f13, 960(x16)\n"
			"flw.ps f24, 0(x16)\n"
			"fnmadd.ps f24, f21, f13, f24, rne\n" 
			"maskpopc x5, m6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 0(x5)\n"
			"flw.ps f7, 768(x5)\n"
			"fcmovm.ps f16, f13, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"maskxor m2, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f16, 576(x18)\n"
			"flw.ps f19, 576(x18)\n"
			"frem.pi f19, f16, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 448(x9)\n"
			"flw.ps f9, 352(x9)\n"
			"fsgnjx.ps f13, f16, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 544(x16)\n"
			"flw.ps f22, 960(x16)\n"
			"fmin.pi f6, f21, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 448(x27)\n"
			"flw.ps f1, 480(x27)\n"
			"fsub.ps f19, f24, f1, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 160(x25)\n"
			"flw.ps f19, 672(x25)\n"
			"fdiv.s f23, f1, f19, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 832(x27)\n"
			"fclass.s x18, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 992(x16)\n"
			"fpackrepb.pi f16, f24\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 544(x13)\n"
			"flw.ps f25, 224(x13)\n"
			"fsub.pi f24, f24, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f13, 736(x13)\n"
			"fcvt.ps.pw f2, f13, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskor m4, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f9, 704(x16)\n"
			"flw.ps f22, 864(x16)\n"
			"fmaxu.pi f6, f9, f22\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 320(x25)\n"
			"fpackrepb.pi f8, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f1, 704(x5)\n"
			"flw.ps f27, 576(x5)\n"
			"fsub.pi f1, f1, f27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 960(x5)\n"
			"flw.ps f23, 640(x5)\n"
			"fadd.pi f8, f8, f23\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 352(x27)\n"
			"fpackreph.pi f25, f28\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 192(x9)\n"
			"flw.ps f6, 608(x9)\n"
			"fremu.pi f22, f24, f6\n" 
			"maskpopc x25, m5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 256(x14)\n"
			"flw.ps f21, 544(x14)\n"
			"fsub.ps f13, f6, f21, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 480(x14)\n"
			"fpackreph.pi f19, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 960(x11)\n"
			"flw.ps f16, 672(x11)\n"
			"fsgnjn.ps f19, f9, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 96(x16)\n"
			"frsq.ps f23, f19\n" 
			"la x20, rand_int_32\n"
			"flw.ps f2, 768(x20)\n"
			"fcvt.ps.pw f19, f2, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"maskor m6, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 992(x25)\n"
			"fcvt.pwu.ps f13, f24, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 576(x13)\n"
			"flw.ps f8, 704(x13)\n"
			"flw.ps f6, 416(x13)\n"
			"fnmadd.s f28, f28, f8, f6, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 896(x25)\n"
			"flw.ps f2, 288(x25)\n"
			"fmin.ps f2, f1, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 608(x14)\n"
			"fswizz.ps f25, f23, 0xe\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 800(x27)\n"
			"flog.ps f25, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f8, 128(x27)\n"
			"flw.ps f21, 480(x27)\n"
			"fminu.pi f1, f8, f21\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 928(x27)\n"
			"flw.ps f1, 384(x27)\n"
			"fxor.pi f21, f9, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 224(x16)\n"
			"flw.ps f1, 224(x16)\n"
			"fsgnjn.ps f13, f9, f1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 352(x20)\n"
			"faddi.pi f27, f25, 0x1c\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 384(x18)\n"
			"flw.ps f21, 832(x18)\n"
			"fsub.pi f19, f2, f21\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 128(x13)\n"
			"fpackreph.pi f27, f23\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 832(x16)\n"
			"flw.ps f25, 576(x16)\n"
			"fsra.pi f23, f24, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 928(x14)\n"
			"flw.ps f13, 320(x14)\n"
			"fsra.pi f22, f6, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 0(x27)\n"
			"flw.ps f28, 832(x27)\n"
			"fadd.ps f16, f24, f28, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 704(x25)\n"
			"fnot.pi f9, f13\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 352(x16)\n"
			"fnot.pi f19, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 736(x5)\n"
			"fnot.pi f6, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 832(x18)\n"
			"flw.ps f23, 640(x18)\n"
			"flw.ps f22, 608(x18)\n"
			"fnmsub.s f19, f9, f23, f22, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f25, 800(x25)\n"
			"flw.ps f16, 32(x25)\n"
			"for.pi f22, f25, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 800(x16)\n"
			"flw.ps f16, 864(x16)\n"
			"flw.ps f1, 864(x16)\n"
			"fnmsub.ps f25, f19, f16, f1, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 160(x18)\n"
			"flw.ps f7, 800(x18)\n"
			"fsgnjn.ps f7, f27, f7\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 0(x9)\n"
			"flw.ps f19, 448(x9)\n"
			"flt.s x9, f1, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 512(x14)\n"
			"fmax.s f1, f8, f8\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 160(x16)\n"
			"fandi.pi f21, f8, 0x189\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 672(x20)\n"
			"flw.ps f8, 128(x20)\n"
			"fxor.pi f21, f27, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 512(x9)\n"
			"flw.ps f1, 544(x9)\n"
			"fsgnjn.ps f22, f27, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 256(x16)\n"
			"flw.ps f25, 320(x16)\n"
			"fmin.ps f23, f7, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 64(x14)\n"
			"flw.ps f28, 256(x14)\n"
			"flw.ps f27, 960(x14)\n"
			"fnmadd.s f6, f6, f28, f27, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 160(x14)\n"
			"fcvt.ps.pwu f22, f22, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 32(x9)\n"
			"flw.ps f13, 768(x9)\n"
			"fmin.ps f27, f2, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 768(x14)\n"
			"fpackreph.pi f25, f23\n" 
			"LBL_SEQID_3_FP_TRANS_ID_4_HID_1:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 384(x20)\n"
			"fexp.ps f2, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 768(x27)\n"
			"fclass.s x11, f22\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 0(x11)\n"
			"flw.ps f13, 416(x11)\n"
			"fle.pi f8, f19, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 416(x25)\n"
			"flw.ps f27, 352(x25)\n"
			"fcmovm.ps f22, f16, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f19, 544(x25)\n"
			"fnot.pi f23, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 544(x18)\n"
			"flw.ps f22, 288(x18)\n"
			"feq.pi f28, f2, f22\n" 
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
			"fbci.pi f8, 0x6219f\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 0(x16)\n"
			"flw.ps f2, 832(x16)\n"
			"fmul.ps f24, f8, f2, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 832(x14)\n"
			"flw.ps f24, 256(x14)\n"
			"fmin.pi f24, f22, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 544(x9)\n"
			"flw.ps f23, 864(x9)\n"
			"fmax.ps f22, f19, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f13, 160(x11)\n"
			"faddi.pi f16, f13, 0x59\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 416(x5)\n"
			"fcvt.pw.ps f13, f8, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 32(x25)\n"
			"fcvt.pwu.ps f7, f6, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 832(x14)\n"
			"flw.ps f7, 448(x14)\n"
			"fadd.s f23, f1, f7, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 832(x27)\n"
			"fcvt.pw.ps f13, f8, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 768(x18)\n"
			"fcvt.ps.pwu f1, f23, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 288(x16)\n"
			"flw.ps f13, 512(x16)\n"
			"fmax.ps f8, f27, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 416(x14)\n"
			"flw.ps f25, 320(x14)\n"
			"fle.s x9, f2, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 64(x13)\n"
			"flw.ps f9, 800(x13)\n"
			"fsrl.pi f21, f2, f9\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 672(x20)\n"
			"fand.pi f13, f6, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 256(x27)\n"
			"flw.ps f9, 192(x27)\n"
			"flw.ps f24, 960(x27)\n"
			"fnmsub.s f23, f19, f9, f24, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 704(x27)\n"
			"flw.ps f7, 224(x27)\n"
			"fmax.s f16, f27, f7\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f7, 736(x16)\n"
			"flw.ps f24, 992(x16)\n"
			"feq.pi f24, f7, f24\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 480(x25)\n"
			"fpackrepb.pi f6, f27\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 896(x18)\n"
			"faddi.pi f21, f22, 0x130\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 800(x9)\n"
			"fcvt.f16.ps f23, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 608(x13)\n"
			"flw.ps f16, 608(x13)\n"
			"fsgnjn.ps f16, f1, f16\n" 
			"LBL_SEQID_5_M0_WRITE_ID_5_HID_1:\n"
			"fltm.ps m2, f8, f19\n" 
			"li x5, 0x80340170\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 480(x14)\n"
			"flw.ps f8, 896(x14)\n"
			"fmin.ps f23, f22, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 96(x20)\n"
			"fclass.s x14, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 32(x25)\n"
			"fcvt.wu.s x14, f21, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 800(x11)\n"
			"fcvt.ps.pw f28, f28, rup\n" 
			"maskpopc x27, m2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 640(x25)\n"
			"flw.ps f6, 800(x25)\n"
			"fmul.s f6, f23, f6, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 800(x27)\n"
			"flw.ps f25, 512(x27)\n"
			"fmulhu.pi f24, f2, f25\n" 
			"LBL_SEQID_5_M0_WRITE_ID_6_HID_1:\n"
			"flem.ps m2, f8, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f13, 832(x5)\n"
			"flw.ps f9, 0(x5)\n"
			"fmax.pi f6, f13, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 576(x9)\n"
			"flw.ps f25, 960(x9)\n"
			"fcmovm.ps f9, f23, f25\n" 
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
			"LBL_HPM_3_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
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
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_2\n"
			"li x5, 0x6\n" 
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 384(x20)\n"
			"fsin.ps f26, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 608(x15)\n"
			"fcvt.lu.s x15, f16, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 224(x15)\n"
			"flw.ps f5, 96(x15)\n"
			"fmax.s f31, f25, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 96(x5)\n"
			"fround.ps f0, f26, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 640(x6)\n"
			"fcvt.pwu.ps f0, f9, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"mov.m.x m3, x28, 0x3f\n" 
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f26, 160(x10)\n"
			"faddi.pi f22, f26, 0x55\n" 
			"mova.x.m x14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 896(x26)\n"
			"flw.ps f31, 128(x26)\n"
			"flw.ps f25, 288(x26)\n"
			"fmsub.ps f5, f22, f31, f25, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 576(x10)\n"
			"flw.ps f9, 704(x10)\n"
			"fmax.ps f25, f0, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"mov.m.x m5, x20, 0x62\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_int_32\n"
			"flw.ps f1, 864(x17)\n"
			"flw.ps f16, 0(x17)\n"
			"fand.pi f22, f1, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 704(x17)\n"
			"fcvt.l.s x20, f0, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 96(x14)\n"
			"flw.ps f9, 608(x14)\n"
			"flw.ps f5, 768(x14)\n"
			"fcmov.ps f10, f30, f9, f5\n" 
			"li x26, 0xd9b861b6334b8c48\n"
			"fcvt.s.wu f27, x26, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_2:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 576(x12)\n"
			"frcp.ps f30, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x6, 0x1a000000077f0001\n"
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xf\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 288(x12)\n"
			"flw.ps f2, 736(x12)\n"
			"fmax.ps f2, f25, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 96(x10)\n"
			"fcvt.w.s x12, f26, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 128(x14)\n"
			"faddi.pi f9, f26, 0xa8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 736(x26)\n"
			"flw.ps f0, 544(x26)\n"
			"feq.pi f31, f2, f0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 832(x25)\n"
			"flw.ps f9, 672(x25)\n"
			"flw.ps f15, 864(x25)\n"
			"fcmov.ps f30, f31, f9, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 704(x28)\n"
			"fclass.s x15, f1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 224(x10)\n"
			"flw.ps f0, 128(x10)\n"
			"fmul.s f19, f2, f0, rtz\n" 
			"li x10, 0xbe6401e3316b3680\n"
			"fcvt.s.wu f15, x10, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 704(x17)\n"
			"fandi.pi f25, f30, 0x1b0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 864(x5)\n"
			"flw.ps f13, 832(x5)\n"
			"fsgnj.s f10, f22, f13\n" 
			"li x5, 0x7f15eee7f58e9f2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7bca6577a9475f85\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fbci.ps f16, 0x20f60\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"flem.ps m5, f5, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 192(x14)\n"
			"flw.ps f16, 64(x14)\n"
			"fmax.s f2, f26, f16\n" 
			"la x28, rand_int_32\n"
			"flw.ps f30, 800(x28)\n"
			"fsat8.pi f1, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f13, 64(x17)\n"
			"fslli.pi f19, f13, 0x9\n" 
			"fbci.pi f31, 0x2b91e\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x26, 0x36000000063c000a\n"
			"csrw tensor_reduce, x26\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_713388_HID_2:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f26, 896(x17)\n"
			"flw.ps f31, 736(x17)\n"
			"frem.pi f30, f26, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 32(x10)\n"
			"flw.ps f26, 576(x10)\n"
			"feq.s x17, f22, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 480(x6)\n"
			"faddi.pi f22, f26, 0x15b\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 832(x26)\n"
			"fclass.s x25, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 96(x25)\n"
			"flw.ps f15, 832(x25)\n"
			"flw.ps f30, 480(x25)\n"
			"fcmov.ps f19, f5, f15, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 64(x17)\n"
			"fnot.pi f26, f31\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 128(x10)\n"
			"fsrai.pi f2, f13, 0xe\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 288(x10)\n"
			"fandi.pi f2, f13, 0xc8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 64(x12)\n"
			"flw.ps f10, 416(x12)\n"
			"fremu.pi f2, f1, f10\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 384(x20)\n"
			"flw.ps f5, 576(x20)\n"
			"frem.pi f27, f13, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 32(x12)\n"
			"flw.ps f16, 608(x12)\n"
			"fmaxu.pi f25, f26, f16\n" 
			"li x10, 0x239a1c92b270f166\n"
			"fcvt.s.wu f31, x10, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_2:\n"
			"maskand m4, m2, m4\n" 
			"li x5, 0x803401E0\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 352(x14)\n"
			"flw.ps f26, 0(x14)\n"
			"fsub.s f16, f10, f26, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 800(x17)\n"
			"fnot.pi f19, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 128(x26)\n"
			"flw.ps f30, 544(x26)\n"
			"feq.pi f2, f16, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x20, rand_int_32\n"
			"flw.ps f22, 96(x20)\n"
			"fnot.pi f0, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 480(x5)\n"
			"flw.ps f19, 800(x5)\n"
			"flw.ps f0, 992(x5)\n"
			"fcmov.ps f27, f15, f19, f0\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 64(x26)\n"
			"flw.ps f16, 736(x26)\n"
			"flt.pi f2, f15, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 64(x26)\n"
			"flw.ps f25, 0(x26)\n"
			"for.pi f15, f1, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f26, 928(x25)\n"
			"fcvt.ps.pw f26, f26, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 864(x12)\n"
			"flw.ps f15, 224(x12)\n"
			"fdiv.ps f9, f1, f15, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 992(x5)\n"
			"flw.ps f1, 544(x5)\n"
			"fnmadd.ps f15, f30, f30, f1, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 576(x26)\n"
			"fnot.pi f9, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 576(x10)\n"
			"fpackrepb.pi f13, f30\n" 
			"maskpopcz x28, m5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 384(x6)\n"
			"flw.ps f15, 800(x6)\n"
			"fmaxu.pi f16, f13, f15\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 832(x14)\n"
			"fpackrepb.pi f22, f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 832(x14)\n"
			"fsin.ps f2, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 992(x25)\n"
			"ffrc.ps f30, f30\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_2:\n"
			"fltm.ps m5, f5, f2\n" 
			"li x5, 0x80340170\n"
			"ld x5, 0(x5)\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 800(x28)\n"
			"flw.ps f30, 992(x28)\n"
			"flw.ps f13, 320(x28)\n"
			"fcmov.ps f10, f2, f30, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_120506_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xdc << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x8c << TENSOR_FMA16_SCP_LOC_B) | (0xcc << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 736(x25)\n"
			"flw.ps f19, 128(x25)\n"
			"fsgnjn.s f15, f13, f19\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 224(x6)\n"
			"fsrli.pi f10, f26, 0x8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 256(x10)\n"
			"flw.ps f30, 512(x10)\n"
			"flw.ps f31, 928(x10)\n"
			"fcmov.ps f9, f19, f30, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 928(x5)\n"
			"flw.ps f22, 352(x5)\n"
			"fadd.s f1, f10, f22, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 320(x10)\n"
			"flw.ps f1, 832(x10)\n"
			"fsgnjx.s f13, f5, f1\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 992(x28)\n"
			"flw.ps f1, 800(x28)\n"
			"flw.ps f5, 992(x28)\n"
			"fmsub.ps f30, f31, f1, f5, rdn\n" 
			"li x5, 0x59ea\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_int_32\n"
			"flw.ps f26, 448(x15)\n"
			"flw.ps f22, 352(x15)\n"
			"fltu.pi f19, f26, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 512(x10)\n"
			"flw.ps f26, 416(x10)\n"
			"fmax.ps f16, f0, f26\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_2:\n"
			"masknot m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f26, 96(x6)\n"
			"flw.ps f22, 736(x6)\n"
			"fsub.pi f26, f26, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 32(x20)\n"
			"fmulhu.pi f16, f0, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 160(x12)\n"
			"frsq.ps f5, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 64(x15)\n"
			"fswizz.ps f26, f9, 0xa\n" 
			"li x5, 0xf53\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 384(x6)\n"
			"flw.ps f10, 480(x6)\n"
			"flw.ps f25, 320(x6)\n"
			"fnmsub.s f30, f5, f10, f25, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 128(x28)\n"
			"flw.ps f30, 288(x28)\n"
			"fand.pi f31, f22, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 992(x25)\n"
			"fcvt.pw.ps f10, f19, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x80b1442c045e781f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd2b9fe339db4885e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x11 << TENSOR_IMA_SCP_LOC_B) | (0x8c << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x42 << TENSOR_FMA16_SCP_LOC_B) | (0xe9 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 96(x28)\n"
			"flw.ps f25, 768(x28)\n"
			"flw.ps f10, 800(x28)\n"
			"fmsub.ps f0, f15, f25, f10, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 512(x15)\n"
			"flw.ps f0, 288(x15)\n"
			"fsgnjx.ps f22, f30, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 96(x26)\n"
			"flw.ps f30, 128(x26)\n"
			"flw.ps f2, 0(x26)\n"
			"fnmsub.ps f26, f27, f30, f2, rup\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_2:\n"
			"maskand m5, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 320(x28)\n"
			"flw.ps f2, 768(x28)\n"
			"fsub.s f13, f10, f2, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 864(x10)\n"
			"flw.ps f5, 896(x10)\n"
			"fdiv.s f22, f26, f5, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 96(x14)\n"
			"fslli.pi f10, f27, 0xe\n" 
			"li x25, 0xda3f688fb07edb71\n"
			"fcvt.s.wu f1, x25, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 256(x14)\n"
			"flw.ps f2, 192(x14)\n"
			"flw.ps f27, 928(x14)\n"
			"fmsub.ps f22, f19, f2, f27, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 480(x15)\n"
			"fcvt.pwu.ps f30, f0, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 768(x20)\n"
			"flw.ps f31, 0(x20)\n"
			"flw.ps f9, 448(x20)\n"
			"fcmov.ps f16, f2, f31, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_2:\n"
			"maskor m5, m3, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x3a2151d08b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 832(x17)\n"
			"flw.ps f31, 416(x17)\n"
			"fmin.s f15, f13, f31\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_2:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_2:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 352(x15)\n"
			"flw.ps f2, 320(x15)\n"
			"fmadd.ps f0, f1, f1, f2, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
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
			"fbci.pi f14, 0x4ff8f\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 640(x27)\n"
			"flw.ps f27, 672(x27)\n"
			"fmsub.ps f22, f7, f27, f7, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 416(x14)\n"
			"fpackrepb.pi f12, f14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 352(x11)\n"
			"flw.ps f19, 320(x11)\n"
			"fmax.ps f8, f17, f19\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f27, 544(x6)\n"
			"fcvt.ps.f16 f11, f27\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 160(x19)\n"
			"flw.ps f18, 704(x19)\n"
			"flw.ps f8, 768(x19)\n"
			"fnmsub.ps f2, f1, f18, f8, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fsetm.pi m5, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"flem.ps m0, f1, f18\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x944173348a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f29, 384(x18)\n"
			"fsatu8.pi f20, f29\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 0(x25)\n"
			"flw.ps f20, 480(x25)\n"
			"fmax.pi f1, f27, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f0, 96(x27)\n"
			"fandi.pi f14, f0, 0x1e0\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 416(x26)\n"
			"fslli.pi f17, f14, 0xe\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 672(x6)\n"
			"flw.ps f0, 352(x6)\n"
			"fsgnjn.ps f22, f19, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 64(x18)\n"
			"flw.ps f0, 896(x18)\n"
			"flt.pi f29, f18, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 64(x18)\n"
			"flw.ps f20, 224(x18)\n"
			"fmin.ps f1, f8, f20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f2, 128(x11)\n"
			"fsrai.pi f0, f2, 0x7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 448(x19)\n"
			"fswizz.ps f25, f14, 0x4\n" 
			"mova.x.m x25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 512(x6)\n"
			"flw.ps f12, 256(x6)\n"
			"fmax.pi f1, f17, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 128(x10)\n"
			"flw.ps f1, 224(x10)\n"
			"fsub.pi f14, f19, f1\n" 
			"fbci.ps f12, 0x712a3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 160(x22)\n"
			"flw.ps f12, 288(x22)\n"
			"flw.ps f27, 640(x22)\n"
			"fmadd.s f22, f19, f12, f27, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 224(x22)\n"
			"flw.ps f12, 512(x22)\n"
			"fsgnjn.s f1, f17, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"maskxor m4, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f8, 96(x26)\n"
			"fsat8.pi f27, f8\n" 
			"fbci.pi f7, 0x61ee9\n" 
			"fbci.ps f11, 0x4f523\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 864(x27)\n"
			"flw.ps f1, 736(x27)\n"
			"fmax.ps f20, f25, f1\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 736(x11)\n"
			"flw.ps f14, 672(x11)\n"
			"for.pi f8, f1, f14\n" 
			"maskpopcz x19, m6\n" 
			"li x22, 0xd4e933a8a4f002f5\n"
			"fcvt.s.wu f11, x22, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 512(x25)\n"
			"fnot.pi f1, f1\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 416(x22)\n"
			"flw.ps f27, 608(x22)\n"
			"flw.ps f29, 704(x22)\n"
			"fmsub.ps f0, f11, f27, f29, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 800(x26)\n"
			"flw.ps f18, 544(x26)\n"
			"flw.ps f19, 992(x26)\n"
			"fmsub.s f11, f7, f18, f19, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_3:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 224(x26)\n"
			"fcvt.w.s x10, f12, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_3:\n"
			"mov.m.x m4, x6, 0x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_3:\n"
			"maskxor m6, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f19, 576(x11)\n"
			"fsrli.pi f22, f19, 0x6\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 672(x18)\n"
			"fround.ps f25, f20, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 800(x6)\n"
			"flw.ps f20, 896(x6)\n"
			"fmul.ps f25, f8, f20, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 704(x11)\n"
			"flw.ps f12, 352(x11)\n"
			"fle.ps f0, f27, f12\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 160(x6)\n"
			"fpackrepb.pi f2, f19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 0(x26)\n"
			"flw.ps f0, 672(x26)\n"
			"fmax.ps f27, f2, f0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 928(x25)\n"
			"flw.ps f7, 480(x25)\n"
			"flw.ps f29, 32(x25)\n"
			"fmsub.s f11, f1, f7, f29, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_3:\n"
			"masknot m6, m6\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_int_32\n"
			"flw.ps f12, 768(x27)\n"
			"flw.ps f29, 992(x27)\n"
			"fsub.pi f19, f12, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f12, 832(x18)\n"
			"flw.ps f2, 992(x18)\n"
			"fadd.pi f17, f12, f2\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_3:\n"
			"masknot m4, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 928(x19)\n"
			"fswizz.ps f19, f12, 0x11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 864(x10)\n"
			"flw.ps f17, 512(x10)\n"
			"fmulh.pi f20, f8, f17\n" 
			"la x22, rand_int_32\n"
			"flw.ps f2, 256(x22)\n"
			"fpackreph.pi f22, f2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f2, 992(x6)\n"
			"flw.ps f1, 384(x6)\n"
			"fltu.pi f11, f2, f1\n" 
			"li x18, 0x80f21960ac796022\n"
			"fcvt.s.w f7, x18, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 448(x18)\n"
			"fpackrepb.pi f19, f2\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 128(x22)\n"
			"flw.ps f27, 768(x22)\n"
			"fsra.pi f19, f14, f27\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 0(x19)\n"
			"flw.ps f11, 736(x19)\n"
			"fle.s x26, f27, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 224(x25)\n"
			"flw.ps f17, 224(x25)\n"
			"fle.pi f0, f20, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 544(x18)\n"
			"flw.ps f19, 704(x18)\n"
			"feq.s x6, f7, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f0, 256(x22)\n"
			"fpackrepb.pi f11, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 832(x11)\n"
			"fcvt.pwu.ps f1, f27, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 640(x27)\n"
			"flw.ps f1, 256(x27)\n"
			"flw.ps f0, 64(x27)\n"
			"fmsub.ps f22, f18, f1, f0, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 416(x26)\n"
			"flw.ps f19, 288(x26)\n"
			"fsgnjn.ps f1, f14, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 832(x19)\n"
			"flw.ps f25, 800(x19)\n"
			"fdiv.ps f20, f18, f25, rdn\n" 
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 256(x10)\n"
			"flw.ps f12, 128(x10)\n"
			"fadd.ps f1, f18, f12, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 864(x6)\n"
			"flw.ps f25, 896(x6)\n"
			"fsub.pi f0, f14, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_3:\n"
			"feqm.ps m4, f0, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 32(x11)\n"
			"fsub.s f12, f29, f29, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 96(x11)\n"
			"flw.ps f1, 608(x11)\n"
			"flw.ps f7, 256(x11)\n"
			"fmsub.s f18, f8, f1, f7, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 64(x10)\n"
			"fcvt.ps.pwu f17, f27, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 480(x27)\n"
			"flw.ps f7, 0(x27)\n"
			"fsgnjn.s f27, f17, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 384(x22)\n"
			"flw.ps f1, 768(x22)\n"
			"fand.pi f29, f25, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 192(x14)\n"
			"flw.ps f27, 800(x14)\n"
			"fsgnj.ps f22, f0, f27\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_3:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 288(x14)\n"
			"fexp.ps f11, f18\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 672(x27)\n"
			"flw.ps f11, 608(x27)\n"
			"fmin.ps f11, f17, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 832(x6)\n"
			"flw.ps f2, 288(x6)\n"
			"flw.ps f1, 32(x6)\n"
			"fnmadd.ps f11, f18, f2, f1, rne\n" 
			"mova.x.m x11\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 608(x11)\n"
			"fsat8.pi f8, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f19, 800(x26)\n"
			"flw.ps f17, 416(x26)\n"
			"fxor.pi f12, f19, f17\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_3:\n"
			"maskand m0, m6, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f22, 384(x10)\n"
			"flw.ps f19, 480(x10)\n"
			"fmaxu.pi f22, f22, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 160(x22)\n"
			"flw.ps f19, 896(x22)\n"
			"fle.ps f11, f11, f19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 320(x26)\n"
			"flw.ps f2, 256(x26)\n"
			"flw.ps f25, 0(x26)\n"
			"fnmadd.ps f19, f27, f2, f25, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 928(x22)\n"
			"flw.ps f11, 192(x22)\n"
			"flw.ps f7, 480(x22)\n"
			"fmadd.ps f1, f1, f11, f7, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 416(x6)\n"
			"flw.ps f12, 64(x6)\n"
			"flw.ps f18, 320(x6)\n"
			"fnmadd.s f18, f27, f12, f18, rtz\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f20, 672(x27)\n"
			"fcvt.ps.f16 f2, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 0(x10)\n"
			"flw.ps f17, 0(x10)\n"
			"fsgnjx.ps f12, f25, f17\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_3:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 128(x25)\n"
			"flw.ps f7, 416(x25)\n"
			"fmul.ps f18, f18, f7, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f25, 256(x6)\n"
			"flw.ps f18, 416(x6)\n"
			"for.pi f19, f25, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 320(x6)\n"
			"flw.ps f19, 64(x6)\n"
			"fsgnjn.s f12, f17, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 288(x11)\n"
			"fclass.ps f2, f14\n" 
			"li x25, 0xefc97d31a046fa32\n"
			"fcvt.s.wu f2, x25, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 960(x27)\n"
			"faddi.pi f27, f11, 0x1f8\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 64(x10)\n"
			"flw.ps f2, 128(x10)\n"
			"fsll.pi f18, f17, f2\n" 
			"LBL_SEQID_5_FP_TRANS_ID_12_HID_3:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 608(x19)\n"
			"flog.ps f12, f14\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 704(x22)\n"
			"flw.ps f16, 576(x22)\n"
			"fsgnj.s f6, f0, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 864(x20)\n"
			"flw.ps f4, 864(x20)\n"
			"fle.s x16, f10, f4\n" 
			"la x26, rand_int_32\n"
			"flw.ps f19, 672(x26)\n"
			"fsrai.pi f16, f19, 0xe\n" 
			"li x18, 0x55f21079d1bbc110\n"
			"fcvt.s.w f19, x18, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 512(x19)\n"
			"flw.ps f30, 576(x19)\n"
			"fmin.pi f15, f10, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 512(x22)\n"
			"flw.ps f24, 672(x22)\n"
			"fmul.pi f15, f19, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f26, 352(x22)\n"
			"fsatu8.pi f5, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"mov.m.x m7, x9, 0x62\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f25, 448(x19)\n"
			"fcvt.ps.pw f6, f25, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 96(x26)\n"
			"fcvt.ps.pwu f5, f24, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 0(x28)\n"
			"fcvt.lu.s x22, f26, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 576(x28)\n"
			"flw.ps f4, 832(x28)\n"
			"flw.ps f10, 352(x28)\n"
			"fnmsub.ps f30, f22, f4, f10, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"feqm.ps m2, f19, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 32(x26)\n"
			"flw.ps f22, 640(x26)\n"
			"flw.ps f15, 896(x26)\n"
			"fnmsub.s f13, f6, f22, f15, rdn\n" 
			"fbci.ps f15, 0x57912\n" 
			"li x5, 0xb517\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 224(x6)\n"
			"fcvt.f16.ps f16, f4\n" 
			"li x5, 0x2ae8664ec8e68293\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x756ce4a977fabba1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0xecf8\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f15, 576(x16)\n"
			"flw.ps f19, 608(x16)\n"
			"fle.pi f30, f15, f19\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 288(x19)\n"
			"fcvt.ps.pw f22, f6, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 224(x26)\n"
			"fsat8.pi f24, f7\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 512(x26)\n"
			"fslli.pi f16, f26, 0xa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 992(x20)\n"
			"fcvt.wu.s x6, f26, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f15, 608(x20)\n"
			"frem.pi f24, f15, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 768(x16)\n"
			"flw.ps f15, 64(x16)\n"
			"flw.ps f13, 0(x16)\n"
			"fmadd.s f13, f19, f15, f13, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 448(x18)\n"
			"flw.ps f24, 480(x18)\n"
			"flw.ps f0, 608(x18)\n"
			"fmsub.ps f0, f30, f24, f0, rup\n" 
			"maskpopc x9, m3\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_4:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 928(x5)\n"
			"fexp.ps f19, f30\n" 
			"la x18, rand_int_32\n"
			"flw.ps f30, 128(x18)\n"
			"flw.ps f15, 288(x18)\n"
			"fltu.pi f7, f30, f15\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 384(x6)\n"
			"fround.ps f16, f10, rne\n" 
			"li x5, 0x6228a5a375c4619f\n"
			"fcvt.s.wu f19, x5, rtz\n" 
			"fbci.ps f26, 0x58f0f\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 448(x9)\n"
			"flw.ps f4, 352(x9)\n"
			"fsgnjx.ps f10, f5, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"maskxor m3, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_664237_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_452486_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x0\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x6, 0x36000000063c000a\n"
			"csrw tensor_reduce, x6\n" 
			"csrwi tensor_wait, 9\n" 
			"li x5, 0xc3dd\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_686685_HID_4:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_141056_HID_4:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0x80000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xb\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0xe000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 928(x6)\n"
			"flw.ps f15, 192(x6)\n"
			"fcmov.ps f15, f24, f15, f15\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 896(x6)\n"
			"flw.ps f13, 160(x6)\n"
			"fsub.ps f30, f16, f13, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"flem.ps m0, f15, f19\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 608(x20)\n"
			"flw.ps f22, 736(x20)\n"
			"fle.ps f5, f15, f22\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 64(x5)\n"
			"faddi.pi f10, f25, 0xed\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 704(x20)\n"
			"flw.ps f5, 256(x20)\n"
			"fsgnj.ps f6, f25, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 352(x9)\n"
			"fsrai.pi f22, f24, 0x4\n" 
			"li x5, 0xf03b\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f19, 576(x6)\n"
			"fpackrepb.pi f6, f19\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 992(x6)\n"
			"flw.ps f5, 992(x6)\n"
			"flw.ps f6, 896(x6)\n"
			"fcmov.ps f26, f15, f5, f6\n" 
			"li x22, 0xdc7142dd0c7d6f29\n"
			"fcvt.s.wu f25, x22, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 64(x6)\n"
			"flw.ps f25, 608(x6)\n"
			"fcmovm.ps f24, f26, f25\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 960(x22)\n"
			"fcvt.ps.pwu f24, f13, rdn\n" 
			"li x20, 0xefded9f92f3248af\n"
			"fcvt.s.w f15, x20, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_4:\n"
			"fltm.pi m7, f0, f24\n" 
			"li x5, 0x803401B8\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 480(x26)\n"
			"flw.ps f4, 992(x26)\n"
			"flt.s x22, f24, f4\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 864(x9)\n"
			"flw.ps f22, 480(x9)\n"
			"fmul.pi f24, f0, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x19, rand_int_32\n"
			"flw.ps f19, 64(x19)\n"
			"flw.ps f15, 928(x19)\n"
			"fmulh.pi f25, f19, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 512(x20)\n"
			"flw.ps f16, 576(x20)\n"
			"fmsub.ps f30, f25, f16, f16, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_4:\n"
			"maskxor m2, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 576(x26)\n"
			"fcvt.f16.ps f19, f16\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_4:\n"
			"masknot m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f4, 0(x19)\n"
			"flw.ps f15, 928(x19)\n"
			"fltu.pi f13, f4, f15\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 320(x6)\n"
			"fpackreph.pi f16, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 224(x22)\n"
			"fdiv.ps f15, f16, f16, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 448(x22)\n"
			"faddi.pi f15, f28, 0x3c\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 224(x20)\n"
			"flw.ps f0, 512(x20)\n"
			"fxor.pi f22, f10, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f25, 96(x9)\n"
			"fle.pi f13, f25, f25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f15, 352(x22)\n"
			"flw.ps f28, 992(x22)\n"
			"flt.s x28, f15, f28\n" 
			"li x22, 0xc01f6c23d8406410\n"
			"fcvt.s.wu f19, x22, rmm\n" 
			"li x5, 0xc3a509d83980f5a4\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2aa4574dfb95c123\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 32(x18)\n"
			"flw.ps f16, 768(x18)\n"
			"fmax.s f6, f10, f16\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_4:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 672(x20)\n"
			"frcp.ps f0, f22\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 192(x22)\n"
			"fcvt.ps.pw f13, f19, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_912142_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_317692_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x3\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x30\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 352(x6)\n"
			"fsqrt.ps f5, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 896(x26)\n"
			"flw.ps f0, 960(x26)\n"
			"fmin.s f30, f13, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 192(x26)\n"
			"fcvt.pwu.ps f16, f7, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 192(x22)\n"
			"flw.ps f16, 192(x22)\n"
			"fand.pi f19, f10, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 512(x6)\n"
			"fswizz.ps f7, f26, 0x0\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 992(x5)\n"
			"flw.ps f0, 256(x5)\n"
			"fmin.pi f6, f5, f0\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 960(x5)\n"
			"faddi.pi f24, f30, 0x18b\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 608(x18)\n"
			"ffrc.ps f30, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 0(x19)\n"
			"flw.ps f13, 96(x19)\n"
			"fsll.pi f5, f0, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 512(x20)\n"
			"flw.ps f6, 864(x20)\n"
			"fmaxu.pi f28, f22, f6\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_4:\n"
			"mov.m.x m3, x19, 0x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_4:\n"
			"maskor m7, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 96(x18)\n"
			"flw.ps f26, 960(x18)\n"
			"fle.ps f15, f30, f26\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 960(x19)\n"
			"flw.ps f4, 832(x19)\n"
			"fadd.pi f25, f30, f4\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 576(x18)\n"
			"fpackrepb.pi f24, f22\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 0(x18)\n"
			"fandi.pi f4, f5, 0xf7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"fbci.pi f24, 0xee4a\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 640(x22)\n"
			"feq.ps f25, f4, f4\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_4:\n"
			"fltm.pi m7, f24, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 192(x18)\n"
			"flw.ps f0, 288(x18)\n"
			"fmax.s f25, f25, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f16, 896(x18)\n"
			"flw.ps f15, 672(x18)\n"
			"flt.pi f16, f16, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 192(x28)\n"
			"fcvt.ps.pwu f4, f26, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 768(x18)\n"
			"flw.ps f24, 576(x18)\n"
			"fle.pi f0, f28, f24\n" 
			"li x5, 0x8b86\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 608(x6)\n"
			"flw.ps f7, 864(x6)\n"
			"fmadd.s f28, f24, f7, f24, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 832(x6)\n"
			"flw.ps f7, 128(x6)\n"
			"fle.s x5, f5, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 448(x18)\n"
			"frsq.ps f0, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 0(x6)\n"
			"flw.ps f30, 320(x6)\n"
			"fsub.pi f28, f16, f30\n" 
			"li x5, 0xd0ad\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 512(x16)\n"
			"fcvt.w.s x18, f30, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 224(x22)\n"
			"flw.ps f25, 832(x22)\n"
			"fsub.s f10, f7, f25, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 480(x9)\n"
			"flw.ps f7, 288(x9)\n"
			"fsgnj.ps f16, f24, f7\n" 
			"la x6, rand_int_32\n"
			"flw.ps f6, 768(x6)\n"
			"flw.ps f16, 928(x6)\n"
			"fdivu.pi f7, f6, f16\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 576(x28)\n"
			"flw.ps f19, 992(x28)\n"
			"fle.s x9, f4, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x92 << TENSOR_IMA_SCP_LOC_B) | (0xc7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xbc << TENSOR_FMA32_SCP_LOC_B) | (0x84 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f20, 960(x14)\n"
			"fpackrepb.pi f13, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 672(x22)\n"
			"flw.ps f30, 640(x22)\n"
			"fsgnj.ps f30, f22, f30\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 832(x13)\n"
			"fpackreph.pi f22, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 544(x14)\n"
			"fsrli.pi f21, f19, 0x5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"fltm.pi m3, f22, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 576(x28)\n"
			"fround.ps f19, f11, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 768(x19)\n"
			"flw.ps f28, 160(x19)\n"
			"fmaxu.pi f20, f12, f28\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_5:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 128(x28)\n"
			"fexp.ps f12, f23\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_5:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 288(x28)\n"
			"fexp.ps f22, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 416(x13)\n"
			"flw.ps f19, 96(x13)\n"
			"fadd.s f21, f9, f19, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_5:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 256(x28)\n"
			"fexp.ps f1, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 672(x22)\n"
			"fpackreph.pi f12, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 800(x28)\n"
			"flw.ps f1, 960(x28)\n"
			"fmul.s f11, f22, f1, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 704(x7)\n"
			"flw.ps f23, 800(x7)\n"
			"flw.ps f22, 320(x7)\n"
			"fnmsub.s f11, f21, f23, f22, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 992(x23)\n"
			"flw.ps f4, 128(x23)\n"
			"feq.s x10, f9, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 32(x10)\n"
			"fandi.pi f28, f21, 0x17b\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f24, 928(x14)\n"
			"fslli.pi f13, f24, 0x4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 0(x28)\n"
			"flw.ps f30, 96(x28)\n"
			"fsgnj.s f28, f4, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f22, 448(x7)\n"
			"flw.ps f24, 32(x7)\n"
			"fmulh.pi f5, f22, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 288(x10)\n"
			"fround.ps f24, f22, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_5:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 416(x13)\n"
			"frcp.ps f11, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 64(x22)\n"
			"flw.ps f5, 832(x22)\n"
			"flw.ps f20, 576(x22)\n"
			"fnmadd.ps f22, f21, f5, f20, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f5, 192(x22)\n"
			"flw.ps f21, 608(x22)\n"
			"fmulhu.pi f24, f5, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_5:\n"
			"maskor m3, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 416(x22)\n"
			"flw.ps f4, 960(x22)\n"
			"flt.s x23, f19, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 256(x10)\n"
			"flw.ps f16, 320(x10)\n"
			"fsgnjx.ps f30, f22, f16\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 928(x28)\n"
			"fminu.pi f9, f1, f1\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 544(x13)\n"
			"faddi.pi f16, f5, 0x41\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 928(x10)\n"
			"flw.ps f13, 256(x10)\n"
			"fand.pi f1, f16, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 960(x28)\n"
			"flw.ps f21, 480(x28)\n"
			"fle.s x7, f9, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 480(x14)\n"
			"frsq.ps f16, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 704(x13)\n"
			"fclass.ps f21, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 512(x19)\n"
			"flw.ps f1, 352(x19)\n"
			"fmul.ps f19, f9, f1, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 448(x28)\n"
			"flw.ps f23, 768(x28)\n"
			"fsub.pi f4, f5, f23\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f21, 640(x28)\n"
			"fcvt.ps.f16 f1, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 576(x15)\n"
			"flw.ps f12, 640(x15)\n"
			"fdiv.ps f1, f22, f12, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 128(x14)\n"
			"flw.ps f30, 448(x14)\n"
			"fminu.pi f16, f5, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 320(x28)\n"
			"flw.ps f21, 96(x28)\n"
			"fsgnjx.s f9, f1, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 992(x13)\n"
			"fcvt.f16.ps f28, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 384(x28)\n"
			"flw.ps f1, 960(x28)\n"
			"fmax.s f19, f28, f1\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 768(x13)\n"
			"flw.ps f24, 160(x13)\n"
			"fsgnjx.ps f30, f5, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 960(x27)\n"
			"fmin.s f16, f5, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 288(x13)\n"
			"flw.ps f1, 576(x13)\n"
			"fmul.pi f9, f30, f1\n" 
			"maskpopc x28, m3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 416(x23)\n"
			"flw.ps f13, 32(x23)\n"
			"fsgnjx.ps f16, f1, f13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 736(x28)\n"
			"flw.ps f28, 448(x28)\n"
			"fmax.pi f13, f5, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 352(x13)\n"
			"flt.ps f4, f4, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 544(x23)\n"
			"flw.ps f9, 0(x23)\n"
			"fminu.pi f1, f13, f9\n" 
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
			"la x23, rand_int_32\n"
			"flw.ps f21, 416(x23)\n"
			"flw.ps f16, 384(x23)\n"
			"fsub.pi f9, f21, f16\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_5:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 288(x19)\n"
			"flog.ps f12, f11\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 448(x15)\n"
			"flw.ps f4, 928(x15)\n"
			"fmul.pi f12, f21, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 0(x10)\n"
			"flw.ps f22, 320(x10)\n"
			"flt.ps f4, f1, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 608(x14)\n"
			"flw.ps f23, 480(x14)\n"
			"fle.ps f24, f1, f23\n" 
			"li x23, 0x65f62714f07cb1c5\n"
			"fcvt.s.lu f20, x23, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 256(x19)\n"
			"flw.ps f19, 928(x19)\n"
			"flt.pi f5, f24, f19\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_5:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 576(x15)\n"
			"frcp.ps f28, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 384(x27)\n"
			"flw.ps f20, 832(x27)\n"
			"fmax.s f12, f11, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 160(x23)\n"
			"flw.ps f24, 320(x23)\n"
			"fsgnjx.ps f9, f12, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 640(x22)\n"
			"fnot.pi f11, f20\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 832(x10)\n"
			"flw.ps f20, 224(x10)\n"
			"fltu.pi f21, f22, f20\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 384(x19)\n"
			"flw.ps f5, 352(x19)\n"
			"flw.ps f20, 128(x19)\n"
			"fnmadd.ps f1, f23, f5, f20, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 352(x23)\n"
			"flw.ps f22, 576(x23)\n"
			"fmul.ps f4, f1, f22, rdn\n" 
			"maskpopc x22, m6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_5:\n"
			"maskand m3, m5, m5\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_5:\n"
			"fltm.ps m3, f22, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 128(x22)\n"
			"fsin.ps f16, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 736(x10)\n"
			"flw.ps f13, 960(x10)\n"
			"fsgnjx.ps f30, f11, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 416(x10)\n"
			"fsatu8.pi f1, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 96(x10)\n"
			"ffrc.ps f28, f20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 544(x13)\n"
			"flw.ps f4, 96(x13)\n"
			"feq.ps f21, f16, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 640(x19)\n"
			"fcvt.pw.ps f5, f19, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 544(x7)\n"
			"ffrc.ps f12, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 480(x15)\n"
			"flw.ps f19, 768(x15)\n"
			"fsrl.pi f30, f22, f19\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 0(x27)\n"
			"fsat8.pi f21, f4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 992(x13)\n"
			"flw.ps f4, 672(x13)\n"
			"feq.pi f22, f22, f4\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_5:\n"
			"flem.ps m5, f1, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 800(x28)\n"
			"fcvt.w.s x10, f23, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 544(x23)\n"
			"flw.ps f13, 864(x23)\n"
			"for.pi f12, f4, f13\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 256(x22)\n"
			"fpackreph.pi f20, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 704(x27)\n"
			"flw.ps f30, 928(x27)\n"
			"fmul.ps f24, f21, f30, rne\n" 
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 64(x14)\n"
			"flw.ps f19, 576(x14)\n"
			"fsub.ps f21, f4, f19, rtz\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_5:\n"
			"maskand m6, m0, m3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_5:\n"
			"mov.m.x m0, x23, 0x2a\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 704(x23)\n"
			"fcvt.f16.ps f4, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 704(x10)\n"
			"fsat8.pi f1, f16\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 192(x22)\n"
			"fcvt.ps.pw f9, f19, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 256(x15)\n"
			"flw.ps f13, 288(x15)\n"
			"fmul.pi f1, f5, f13\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_5:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 864(x13)\n"
			"fclass.s x14, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 960(x28)\n"
			"fpackrepb.pi f11, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 224(x7)\n"
			"fsgnjn.s f1, f20, f20\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 64(x19)\n"
			"flw.ps f19, 608(x19)\n"
			"fcmovm.ps f30, f20, f19\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 160(x10)\n"
			"flw.ps f22, 224(x10)\n"
			"flt.ps f9, f13, f22\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 352(x27)\n"
			"flw.ps f20, 256(x27)\n"
			"fand.pi f24, f4, f20\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 544(x15)\n"
			"flw.ps f22, 192(x15)\n"
			"fsgnjx.ps f1, f4, f22\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 448(x14)\n"
			"flw.ps f30, 960(x14)\n"
			"fminu.pi f4, f24, f30\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
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
			"LBL_KERNEL_HID_6:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_6\n"
			"li x5, 0x6\n" 
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 800(x10)\n"
			"flw.ps f29, 832(x10)\n"
			"fnmadd.ps f27, f2, f29, f29, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 544(x5)\n"
			"flw.ps f5, 608(x5)\n"
			"fmsub.s f29, f2, f5, f5, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 992(x16)\n"
			"flw.ps f27, 448(x16)\n"
			"fmin.s f10, f11, f27\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"feqm.ps m7, f25, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 192(x15)\n"
			"flw.ps f19, 256(x15)\n"
			"fsgnjx.s f19, f5, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"maskand m7, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"flem.ps m1, f25, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_6:\n"
			"fsetm.pi m1, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f25, 0x7eb16\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 704(x5)\n"
			"flw.ps f13, 576(x5)\n"
			"fand.pi f25, f25, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 608(x14)\n"
			"flw.ps f19, 768(x14)\n"
			"feq.s x14, f0, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 960(x14)\n"
			"flw.ps f25, 512(x14)\n"
			"fsub.ps f11, f24, f25, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 64(x22)\n"
			"fclass.ps f26, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 256(x20)\n"
			"flw.ps f27, 640(x20)\n"
			"fmsub.ps f13, f13, f27, f13, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 480(x15)\n"
			"fcvt.wu.s x15, f19, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 448(x15)\n"
			"flw.ps f13, 704(x15)\n"
			"fsgnjn.ps f19, f22, f13\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 288(x16)\n"
			"flw.ps f12, 544(x16)\n"
			"fmul.ps f25, f24, f12, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 768(x5)\n"
			"flw.ps f26, 0(x5)\n"
			"fsgnj.ps f12, f13, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 0(x15)\n"
			"feq.s x10, f10, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 224(x22)\n"
			"flw.ps f10, 832(x22)\n"
			"fdivu.pi f5, f24, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 768(x14)\n"
			"flw.ps f27, 640(x14)\n"
			"flw.ps f26, 864(x14)\n"
			"fnmsub.ps f5, f12, f27, f26, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 288(x23)\n"
			"flw.ps f5, 928(x23)\n"
			"fle.ps f12, f0, f5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"fltm.ps m5, f12, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f25, 256(x20)\n"
			"fpackrepb.pi f24, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"maskand m4, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 544(x10)\n"
			"fclass.ps f24, f12\n" 
			"li x5, 0x582fc44a4400c0d6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6f2bd827ca2a99d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 704(x5)\n"
			"flw.ps f5, 864(x5)\n"
			"flw.ps f27, 992(x5)\n"
			"fnmsub.s f5, f24, f5, f27, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 640(x22)\n"
			"flw.ps f29, 832(x22)\n"
			"fsgnj.s f27, f27, f29\n" 
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_6:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 960(x27)\n"
			"frcp.ps f29, f5\n" 
			"LBL_SEQID_1_FP_TRANS_ID_7_HID_6:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 480(x17)\n"
			"fexp.ps f24, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 672(x22)\n"
			"flw.ps f13, 32(x22)\n"
			"fmsub.s f2, f24, f13, f24, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 96(x23)\n"
			"flw.ps f26, 192(x23)\n"
			"fsub.pi f4, f11, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x20, 0x36000000063c000a\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_337415_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xda << TENSOR_IMA_SCP_LOC_B) | (0x8a << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x8 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_68209_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xe1 << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x8 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xac << TENSOR_FMA32_SCP_LOC_B) | (0x75 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7d << TENSOR_IMA_SCP_LOC_B) | (0x75 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 64(x15)\n"
			"flw.ps f2, 896(x15)\n"
			"fmax.ps f9, f9, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 736(x20)\n"
			"faddi.pi f9, f26, 0x113\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f26, 960(x10)\n"
			"fcvt.ps.f16 f12, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 768(x22)\n"
			"flw.ps f11, 160(x22)\n"
			"flw.ps f13, 544(x22)\n"
			"fnmadd.s f19, f19, f11, f13, rmm\n" 
			"mova.x.m x20\n" 
			"li x5, 0xed60\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f24, 576(x23)\n"
			"fsrli.pi f4, f24, 0x6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f26, 288(x23)\n"
			"faddi.pi f19, f26, 0x1b5\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 480(x17)\n"
			"faddi.pi f22, f9, 0x10a\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 64(x20)\n"
			"fnot.pi f11, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 480(x15)\n"
			"flw.ps f13, 64(x15)\n"
			"flw.ps f10, 96(x15)\n"
			"fmadd.ps f4, f22, f13, f10, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f5, 0(x22)\n"
			"flw.ps f11, 608(x22)\n"
			"fsub.pi f26, f5, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 288(x17)\n"
			"flw.ps f24, 96(x17)\n"
			"flw.ps f13, 544(x17)\n"
			"fmsub.s f4, f10, f24, f13, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 192(x14)\n"
			"flw.ps f0, 928(x14)\n"
			"fmul.s f11, f27, f0, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 96(x23)\n"
			"flw.ps f11, 512(x23)\n"
			"fsub.s f9, f24, f11, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 832(x5)\n"
			"fpackreph.pi f24, f11\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_6:\n"
			"feqm.ps m7, f29, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x741a00a07f\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_372236_HID_6:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
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
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_6:\n"
			"mov.m.x m4, x23, 0x4f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_6:\n"
			"fsetm.pi m5, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 544(x27)\n"
			"flw.ps f4, 32(x27)\n"
			"fmax.s f29, f9, f4\n" 
			"mova.x.m x23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 128(x17)\n"
			"flw.ps f11, 480(x17)\n"
			"fsgnjx.ps f12, f2, f11\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 0(x16)\n"
			"flw.ps f19, 160(x16)\n"
			"fsgnj.ps f26, f25, f19\n" 
			"mova.x.m x28\n" 
			"li x5, 0x1b54\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 800(x27)\n"
			"fclass.ps f13, f9\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_6:\n"
			"maskand m7, m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 864(x14)\n"
			"fsin.ps f27, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 256(x10)\n"
			"flw.ps f29, 0(x10)\n"
			"fle.s x28, f9, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 704(x23)\n"
			"flw.ps f2, 480(x23)\n"
			"fmin.ps f13, f26, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 256(x17)\n"
			"fcvt.wu.s x16, f5, rtz\n" 
			"maskpopcz x16, m7\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_6:\n"
			"fltm.pi m4, f0, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f24, 896(x17)\n"
			"flw.ps f13, 960(x17)\n"
			"fsra.pi f22, f24, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f10, 544(x27)\n"
			"fand.pi f5, f10, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 928(x17)\n"
			"flw.ps f11, 768(x17)\n"
			"flw.ps f25, 256(x17)\n"
			"fmsub.s f0, f9, f11, f25, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 224(x5)\n"
			"fnot.pi f9, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 416(x15)\n"
			"fadd.ps f13, f4, f4, rne\n" 
			"li x5, 0x576247309c5e0897\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8fd6d807282c02bf\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f2, 448(x28)\n"
			"flw.ps f10, 704(x28)\n"
			"fmulhu.pi f5, f2, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 448(x22)\n"
			"flw.ps f26, 928(x22)\n"
			"flw.ps f27, 736(x22)\n"
			"fmsub.ps f11, f10, f26, f27, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 640(x5)\n"
			"flw.ps f5, 864(x5)\n"
			"fsgnj.s f29, f26, f5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 992(x28)\n"
			"flw.ps f10, 320(x28)\n"
			"fadd.ps f2, f11, f10, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x15, m1\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_6:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 0(x20)\n"
			"feq.ps f0, f13, f13\n" 
			"li x5, 0xd54009e9c2e85a1b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfc8405a83a16a9b1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_6:\n"
			"fltm.pi m5, f0, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 800(x27)\n"
			"fmax.ps f0, f9, f9\n" 
			"maskpopcz x16, m4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 320(x23)\n"
			"fsat8.pi f12, f9\n" 
			"LBL_SEQID_4_M0_WRITE_ID_15_HID_6:\n"
			"feqm.ps m1, f12, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_76907_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_253729_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 736(x20)\n"
			"fround.ps f2, f2, rmm\n" 
			"fbci.pi f12, 0x4c6a\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 320(x16)\n"
			"ffrc.ps f9, f5\n" 
			"mova.x.m x23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 736(x17)\n"
			"flw.ps f24, 352(x17)\n"
			"fsgnj.s f26, f11, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 544(x14)\n"
			"flw.ps f5, 928(x14)\n"
			"flw.ps f26, 608(x14)\n"
			"fmsub.s f29, f10, f5, f26, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 64(x20)\n"
			"flw.ps f10, 448(x20)\n"
			"fcmovm.ps f5, f5, f10\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 928(x5)\n"
			"fcvt.l.s x16, f2, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 416(x20)\n"
			"fcvt.wu.s x14, f25, rtz\n" 
			"mova.x.m x22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 768(x14)\n"
			"flw.ps f25, 640(x14)\n"
			"fmax.ps f10, f12, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 320(x15)\n"
			"fsin.ps f22, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 384(x23)\n"
			"flw.ps f19, 768(x23)\n"
			"fle.s x28, f13, f19\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 160(x23)\n"
			"fsrai.pi f12, f12, 0xb\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 832(x14)\n"
			"flw.ps f10, 928(x14)\n"
			"flw.ps f13, 928(x14)\n"
			"fmsub.ps f12, f19, f10, f13, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 256(x28)\n"
			"flw.ps f25, 768(x28)\n"
			"fmaxu.pi f26, f12, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_46142_HID_6:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
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
			"la x12, rand_int_32\n"
			"flw.ps f31, 992(x12)\n"
			"flw.ps f9, 928(x12)\n"
			"fltu.pi f28, f31, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 96(x13)\n"
			"flw.ps f4, 224(x13)\n"
			"flw.ps f28, 704(x13)\n"
			"fnmadd.ps f10, f20, f4, f28, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 768(x15)\n"
			"frcp.ps f16, f8\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 320(x15)\n"
			"flw.ps f25, 160(x15)\n"
			"fcmovm.ps f9, f10, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 672(x12)\n"
			"fandi.pi f4, f7, 0x1ec\n" 
			"maskpopcz x23, m2\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_7:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 288(x13)\n"
			"frcp.ps f16, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"mova.m.x x16\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf4dd9f9a28\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 256(x12)\n"
			"fcvt.f16.ps f8, f14\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f14, 352(x23)\n"
			"fcvt.ps.f16 f7, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 768(x16)\n"
			"flw.ps f3, 128(x16)\n"
			"flw.ps f31, 448(x16)\n"
			"fmadd.s f9, f20, f3, f31, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 928(x16)\n"
			"flw.ps f14, 704(x16)\n"
			"flw.ps f7, 352(x16)\n"
			"fnmadd.ps f16, f9, f14, f7, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 704(x12)\n"
			"flw.ps f20, 0(x12)\n"
			"feq.ps f8, f28, f20\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 384(x11)\n"
			"flw.ps f15, 352(x11)\n"
			"fadd.ps f19, f20, f15, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 224(x12)\n"
			"fpackreph.pi f19, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_7:\n"
			"masknot m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 608(x11)\n"
			"flw.ps f4, 224(x11)\n"
			"fsub.s f8, f15, f4, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 64(x19)\n"
			"flw.ps f20, 992(x19)\n"
			"flw.ps f15, 96(x19)\n"
			"fnmadd.s f14, f25, f20, f15, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 864(x6)\n"
			"flw.ps f3, 480(x6)\n"
			"fmul.pi f25, f19, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 384(x12)\n"
			"flw.ps f10, 960(x12)\n"
			"fmin.ps f25, f28, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 928(x16)\n"
			"fsat8.pi f6, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 992(x7)\n"
			"flw.ps f28, 832(x7)\n"
			"fsgnjn.s f10, f4, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_7:\n"
			"feqm.ps m0, f3, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f16, 320(x23)\n"
			"flw.ps f10, 576(x23)\n"
			"flt.pi f19, f16, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 512(x23)\n"
			"flw.ps f15, 160(x23)\n"
			"fle.ps f15, f1, f15\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 512(x19)\n"
			"flw.ps f7, 960(x19)\n"
			"fsra.pi f6, f6, f7\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 192(x13)\n"
			"flw.ps f10, 864(x13)\n"
			"flw.ps f14, 576(x13)\n"
			"fmsub.s f7, f9, f10, f14, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 320(x15)\n"
			"fswizz.ps f20, f4, 0x14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 96(x23)\n"
			"fswizz.ps f1, f15, 0x10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_7:\n"
			"feqm.ps m5, f25, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_7:\n"
			"fltm.ps m0, f16, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 224(x10)\n"
			"flw.ps f1, 672(x10)\n"
			"fsgnjx.s f7, f7, f1\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_7:\n"
			"fltm.ps m7, f20, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 0(x23)\n"
			"flw.ps f20, 416(x23)\n"
			"flw.ps f9, 544(x23)\n"
			"fmsub.ps f10, f15, f20, f9, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 448(x19)\n"
			"flt.pi f20, f8, f8\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 0(x10)\n"
			"flw.ps f1, 128(x10)\n"
			"for.pi f10, f19, f1\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_7:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 928(x11)\n"
			"frcp.ps f16, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_7:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 800(x15)\n"
			"flw.ps f8, 352(x15)\n"
			"flw.ps f7, 32(x15)\n"
			"fnmsub.s f1, f16, f8, f7, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 736(x12)\n"
			"frsq.ps f16, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 480(x10)\n"
			"flw.ps f14, 896(x10)\n"
			"feq.s x15, f7, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 512(x12)\n"
			"flw.ps f20, 832(x12)\n"
			"fadd.ps f4, f10, f20, rtz\n" 
			"li x6, 0x5643b4220ff967cd\n"
			"fcvt.s.wu f19, x6, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 128(x13)\n"
			"flw.ps f14, 448(x13)\n"
			"flw.ps f3, 928(x13)\n"
			"fnmadd.s f4, f19, f14, f3, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 352(x13)\n"
			"flw.ps f14, 896(x13)\n"
			"fmul.s f31, f19, f14, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 704(x7)\n"
			"flw.ps f6, 64(x7)\n"
			"fmadd.ps f31, f3, f6, f3, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 96(x6)\n"
			"flw.ps f8, 320(x6)\n"
			"fmin.s f4, f3, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 512(x19)\n"
			"flw.ps f16, 576(x19)\n"
			"fadd.s f20, f19, f16, rne\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f14, 544(x16)\n"
			"flw.ps f15, 96(x16)\n"
			"fremu.pi f9, f14, f15\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 800(x19)\n"
			"flw.ps f19, 896(x19)\n"
			"flw.ps f6, 864(x19)\n"
			"fmadd.s f3, f7, f19, f6, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 736(x11)\n"
			"flw.ps f16, 0(x11)\n"
			"fmul.s f28, f15, f16, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 416(x23)\n"
			"feq.ps f28, f10, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 0(x19)\n"
			"flw.ps f16, 192(x19)\n"
			"fsub.ps f9, f15, f16, rtz\n" 
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_7:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 928(x10)\n"
			"flog.ps f14, f25\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 128(x7)\n"
			"flw.ps f4, 992(x7)\n"
			"fmax.s f4, f10, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_7:\n"
			"mov.m.x m2, x6, 0x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f6, 384(x7)\n"
			"fpackrepb.pi f9, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 896(x19)\n"
			"flw.ps f7, 864(x19)\n"
			"fminu.pi f9, f3, f7\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 672(x13)\n"
			"flw.ps f8, 32(x13)\n"
			"fsll.pi f31, f10, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_7:\n"
			"fsetm.pi m5, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_7:\n"
			"maskxor m5, m2, m0\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"fbci.pi f8, 0x7836c\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 768(x11)\n"
			"fsrli.pi f1, f31, 0x6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 928(x10)\n"
			"flw.ps f20, 416(x10)\n"
			"fsgnjn.s f8, f3, f20\n" 
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
			"flw.ps f28, 768(x12)\n"
			"flw.ps f3, 928(x12)\n"
			"fsub.s f15, f28, f3, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 960(x16)\n"
			"fmul.ps f19, f31, f31, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_7:\n"
			"mov.m.x m2, x16, 0x46\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 928(x19)\n"
			"flw.ps f28, 224(x19)\n"
			"fmsub.ps f20, f16, f16, f28, rdn\n" 
			"LBL_SEQID_4_FP_TRANS_ID_15_HID_7:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 928(x19)\n"
			"frcp.ps f1, f8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 832(x12)\n"
			"fcvt.ps.pw f31, f4, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_16_HID_7:\n"
			"masknot m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 704(x19)\n"
			"frsq.ps f15, f10\n" 
			"fbci.ps f16, 0x6036b\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 608(x6)\n"
			"flw.ps f19, 352(x6)\n"
			"feq.pi f6, f10, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 864(x15)\n"
			"flw.ps f4, 160(x15)\n"
			"fadd.pi f9, f19, f4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 800(x16)\n"
			"fcvt.ps.pwu f28, f6, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 608(x7)\n"
			"flw.ps f28, 704(x7)\n"
			"fsub.ps f14, f25, f28, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 64(x10)\n"
			"fsatu8.pi f25, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 736(x11)\n"
			"flw.ps f19, 960(x11)\n"
			"fmin.ps f14, f28, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 832(x7)\n"
			"flw.ps f19, 96(x7)\n"
			"fsgnj.ps f8, f1, f19\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f14, 608(x7)\n"
			"fcvt.ps.pwu f15, f14, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 416(x6)\n"
			"flw.ps f25, 768(x6)\n"
			"fand.pi f28, f31, f25\n" 
			"LBL_SEQID_5_M0_WRITE_ID_17_HID_7:\n"
			"fltm.pi m5, f7, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 64(x16)\n"
			"frsq.ps f16, f7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 64(x10)\n"
			"flw.ps f19, 128(x10)\n"
			"flt.s x16, f8, f19\n" 
			"LBL_SEQID_5_M0_WRITE_ID_18_HID_7:\n"
			"maskxor m0, m2, m7\n" 
			"li x5, 0x803401A8\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 288(x11)\n"
			"fswizz.ps f1, f20, 0x3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 896(x12)\n"
			"fsatu8.pi f31, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 800(x6)\n"
			"flw.ps f19, 128(x6)\n"
			"flw.ps f1, 256(x6)\n"
			"fmsub.s f3, f3, f19, f1, rdn\n" 
			"LBL_SEQID_5_FP_TRANS_ID_19_HID_7:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 928(x7)\n"
			"frcp.ps f15, f16\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 352(x19)\n"
			"flw.ps f7, 0(x19)\n"
			"fsub.pi f28, f28, f7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 448(x19)\n"
			"flw.ps f14, 352(x19)\n"
			"fcmovm.ps f14, f19, f14\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 352(x16)\n"
			"fsrli.pi f28, f9, 0xe\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 0(x13)\n"
			"fnot.pi f4, f25\n" 
			"li x12, 0xca7db2814b323124\n"
			"fcvt.s.wu f9, x12, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 768(x15)\n"
			"flw.ps f25, 800(x15)\n"
			"flw.ps f15, 384(x15)\n"
			"fmadd.ps f16, f1, f25, f15, rmm\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
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
			"li x6, 0xa428484bf5defd29\n"
			"fcvt.s.l f18, x6, rtz\n" 
			"li x5, 0xd980\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"mov.m.x m1, x6, 0x5f\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_int_32\n"
			"flw.ps f10, 960(x16)\n"
			"fsat8.pi f19, f10\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 736(x13)\n"
			"flw.ps f27, 832(x13)\n"
			"for.pi f0, f5, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 64(x16)\n"
			"flw.ps f4, 512(x16)\n"
			"feq.s x13, f19, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 128(x14)\n"
			"fcvt.wu.s x26, f19, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 128(x10)\n"
			"flw.ps f8, 768(x10)\n"
			"fmin.pi f4, f13, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 960(x11)\n"
			"flw.ps f17, 192(x11)\n"
			"fsub.s f6, f5, f17, rtz\n" 
			"maskpopcz x16, m6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 832(x19)\n"
			"flw.ps f17, 576(x19)\n"
			"flw.ps f13, 96(x19)\n"
			"fnmadd.s f6, f23, f17, f13, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_8:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 800(x6)\n"
			"flog.ps f5, f30\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 416(x26)\n"
			"fpackreph.pi f19, f30\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 544(x10)\n"
			"flw.ps f4, 800(x10)\n"
			"fle.s x19, f23, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 896(x6)\n"
			"fpackrepb.pi f27, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 416(x11)\n"
			"flw.ps f19, 256(x11)\n"
			"feq.ps f24, f0, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 640(x17)\n"
			"flw.ps f4, 352(x17)\n"
			"feq.pi f30, f5, f4\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 928(x25)\n"
			"fswizz.ps f0, f6, 0x1e\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 512(x19)\n"
			"flw.ps f8, 864(x19)\n"
			"fmax.pi f27, f7, f8\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_8:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 576(x19)\n"
			"frcp.ps f5, f4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 672(x6)\n"
			"flw.ps f5, 256(x6)\n"
			"fmax.s f24, f0, f5\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 352(x16)\n"
			"fslli.pi f10, f10, 0x8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 640(x6)\n"
			"flw.ps f7, 480(x6)\n"
			"feq.ps f5, f8, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 96(x17)\n"
			"flw.ps f18, 768(x17)\n"
			"feq.pi f8, f17, f18\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 288(x25)\n"
			"flw.ps f30, 480(x25)\n"
			"fsgnjn.s f5, f27, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"mova.m.x x25\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x19, rand_int_32\n"
			"flw.ps f24, 992(x19)\n"
			"flw.ps f18, 128(x19)\n"
			"fxor.pi f17, f24, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"feqm.ps m7, f4, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f27, 512(x10)\n"
			"flw.ps f17, 544(x10)\n"
			"for.pi f10, f27, f17\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 352(x25)\n"
			"flw.ps f13, 512(x25)\n"
			"fle.s x11, f23, f13\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 512(x11)\n"
			"flw.ps f6, 768(x11)\n"
			"fle.pi f10, f4, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 928(x6)\n"
			"flw.ps f3, 352(x6)\n"
			"feq.ps f27, f6, f3\n" 
			"fbci.ps f24, 0x58fc2\n" 
			"li x5, 0xb4f3\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x10, 0x36000000063c000a\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x7\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 448(x16)\n"
			"flw.ps f0, 704(x16)\n"
			"fmax.s f6, f7, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 768(x10)\n"
			"fpackrepb.pi f4, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 832(x10)\n"
			"flw.ps f30, 832(x10)\n"
			"fmul.pi f27, f19, f30\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_8:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 64(x10)\n"
			"fexp.ps f18, f6\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f19, 448(x14)\n"
			"fcvt.ps.f16 f19, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 992(x16)\n"
			"fcvt.pw.ps f4, f27, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 288(x11)\n"
			"fcvt.pwu.ps f3, f5, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 448(x10)\n"
			"fsqrt.s f10, f6, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 544(x10)\n"
			"flw.ps f19, 640(x10)\n"
			"flw.ps f5, 128(x10)\n"
			"fcmov.ps f27, f8, f19, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 672(x25)\n"
			"ffrc.ps f17, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 0(x6)\n"
			"flw.ps f24, 704(x6)\n"
			"fmulh.pi f10, f17, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 672(x16)\n"
			"flw.ps f19, 416(x16)\n"
			"feq.s x10, f30, f19\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 320(x6)\n"
			"flw.ps f5, 480(x6)\n"
			"flw.ps f0, 800(x6)\n"
			"fnmsub.ps f4, f6, f5, f0, rmm\n" 
			"fbci.pi f5, 0x7e9b1\n" 
			"la x16, rand_int_32\n"
			"flw.ps f19, 384(x16)\n"
			"flw.ps f10, 256(x16)\n"
			"fmulh.pi f10, f19, f10\n" 
			"li x16, 0xaf5102dea52ff0c2\n"
			"fcvt.s.wu f3, x16, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_224300_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_8:\n"
			"masknot m7, m0\n" 
			"li x5, 0x803401D0\n"
			"ld x5, 0(x5)\n"
			"la x6, rand_int_32\n"
			"flw.ps f24, 32(x6)\n"
			"flw.ps f0, 64(x6)\n"
			"fadd.pi f17, f24, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 896(x11)\n"
			"fcvt.w.s x11, f17, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 160(x6)\n"
			"fcvt.ps.pwu f30, f27, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_8:\n"
			"flem.ps m0, f7, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 352(x25)\n"
			"flw.ps f24, 800(x25)\n"
			"flw.ps f6, 512(x25)\n"
			"fnmadd.ps f18, f27, f24, f6, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 736(x11)\n"
			"flw.ps f8, 544(x11)\n"
			"fsgnjx.s f10, f3, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 736(x6)\n"
			"fsrai.pi f5, f8, 0x6\n" 
			"li x5, 0xba3644143b9d5c1d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc70caa88a859e67b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 928(x26)\n"
			"flw.ps f19, 64(x26)\n"
			"fdiv.s f3, f0, f19, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 800(x19)\n"
			"flw.ps f13, 992(x19)\n"
			"fminu.pi f4, f19, f13\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 608(x19)\n"
			"flw.ps f3, 992(x19)\n"
			"fsgnj.s f24, f24, f3\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_8:\n"
			"fsetm.pi m6, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_8:\n"
			"maskand m0, m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f4, 608(x6)\n"
			"flw.ps f8, 928(x6)\n"
			"for.pi f6, f4, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_8:\n"
			"flem.ps m0, f6, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f18, 800(x26)\n"
			"flw.ps f17, 640(x26)\n"
			"for.pi f27, f18, f17\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 64(x26)\n"
			"fcvt.f16.ps f0, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 480(x11)\n"
			"flw.ps f27, 864(x11)\n"
			"fmax.ps f24, f10, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 480(x17)\n"
			"flw.ps f6, 864(x17)\n"
			"fsub.s f19, f24, f6, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 64(x13)\n"
			"flw.ps f8, 32(x13)\n"
			"fsgnjn.ps f19, f4, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 928(x26)\n"
			"flw.ps f24, 576(x26)\n"
			"fmul.pi f24, f17, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 64(x25)\n"
			"flw.ps f23, 608(x25)\n"
			"fsub.ps f24, f4, f23, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 0(x13)\n"
			"fsrai.pi f23, f3, 0xc\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 544(x14)\n"
			"flw.ps f27, 896(x14)\n"
			"feq.ps f27, f7, f27\n" 
			"fbci.pi f7, 0x63067\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 64(x26)\n"
			"fandi.pi f3, f30, 0x28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 320(x13)\n"
			"flw.ps f3, 96(x13)\n"
			"fmaxu.pi f23, f5, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 544(x17)\n"
			"fcvt.ps.pwu f4, f6, rne\n" 
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_8:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 928(x10)\n"
			"frcp.ps f8, f5\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_8:\n"
			"maskxor m6, m0, m0\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 576(x6)\n"
			"flw.ps f6, 768(x6)\n"
			"feq.s x11, f27, f6\n" 
			"li x5, 0x1de3\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_8:\n"
			"masknot m6, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x64d1b440b4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 736(x26)\n"
			"flw.ps f17, 64(x26)\n"
			"fsub.ps f17, f6, f17, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 800(x13)\n"
			"fand.pi f10, f0, f0\n" 
			"li x5, 0xdcab5cac0560b953\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfe42c1b4d7f6da05\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_8:\n"
			"maskand m1, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x25, 0xecccd2d676817d1e\n"
			"fcvt.s.w f27, x25, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 736(x14)\n"
			"fsgnjx.ps f18, f4, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 320(x6)\n"
			"flw.ps f30, 288(x6)\n"
			"fsrl.pi f4, f23, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 384(x14)\n"
			"fclass.ps f8, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 64(x25)\n"
			"fcvt.pwu.ps f0, f24, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 0(x25)\n"
			"flw.ps f7, 256(x25)\n"
			"fsgnjx.s f7, f24, f7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 416(x19)\n"
			"flw.ps f7, 704(x19)\n"
			"flw.ps f17, 288(x19)\n"
			"fnmadd.s f3, f0, f7, f17, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f19, 928(x25)\n"
			"fsrl.pi f10, f19, f19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 832(x26)\n"
			"flw.ps f5, 288(x26)\n"
			"fsgnjx.s f0, f30, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 448(x14)\n"
			"flw.ps f13, 576(x14)\n"
			"fsgnj.ps f30, f7, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 128(x16)\n"
			"fsin.ps f13, f4\n" 
			"li x5, 0xc3d3\n"
			"csrw tensor_mask, x5\n"	
			"mova.x.m x14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 800(x17)\n"
			"ffrc.ps f8, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_446523_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x72 << TENSOR_FMA32_SCP_LOC_B) | (0x36 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 384(x25)\n"
			"fcvt.pw.ps f12, f17, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 128(x6)\n"
			"fsrai.pi f23, f5, 0x5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 896(x27)\n"
			"flw.ps f24, 64(x27)\n"
			"fmax.s f11, f12, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 544(x10)\n"
			"flw.ps f24, 640(x10)\n"
			"fmsub.s f13, f11, f24, f11, rup\n" 
			"li x20, 0xd1f637e369469017\n"
			"fcvt.s.l f11, x20, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"mov.m.x m1, x7, 0x70\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 480(x25)\n"
			"flw.ps f24, 640(x25)\n"
			"fsgnj.ps f5, f7, f24\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f8, 704(x6)\n"
			"fcvt.ps.f16 f27, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"mov.m.x m2, x16, 0x1e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"flem.ps m3, f11, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 608(x6)\n"
			"flw.ps f30, 640(x6)\n"
			"fsgnjn.s f17, f18, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 288(x6)\n"
			"fandi.pi f12, f12, 0x120\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 736(x17)\n"
			"flw.ps f18, 320(x17)\n"
			"fmin.s f24, f3, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_9:\n"
			"flem.ps m2, f23, f23\n" 
			"li x5, 0x803401F0\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_int_32\n"
			"flw.ps f24, 512(x7)\n"
			"flw.ps f8, 128(x7)\n"
			"fltu.pi f1, f24, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 736(x23)\n"
			"flw.ps f18, 640(x23)\n"
			"feq.ps f11, f30, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f7, 480(x5)\n"
			"flw.ps f1, 544(x5)\n"
			"fsub.pi f17, f7, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 32(x17)\n"
			"fsgnj.ps f16, f1, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 256(x23)\n"
			"fround.ps f13, f29, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 352(x10)\n"
			"flw.ps f3, 320(x10)\n"
			"fmin.ps f5, f8, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 576(x27)\n"
			"flw.ps f29, 32(x27)\n"
			"fle.s x23, f23, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 128(x6)\n"
			"fsrai.pi f27, f12, 0x4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f29, 448(x27)\n"
			"flw.ps f23, 128(x27)\n"
			"fsub.pi f24, f29, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 256(x26)\n"
			"flw.ps f27, 864(x26)\n"
			"fsgnj.ps f23, f8, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 800(x26)\n"
			"fswizz.ps f11, f27, 0x7\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 416(x20)\n"
			"fsrli.pi f16, f27, 0xf\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"maskand m1, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 768(x6)\n"
			"fcvt.wu.s x20, f3, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_9:\n"
			"maskor m3, m4, m2\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_int_32\n"
			"flw.ps f24, 960(x17)\n"
			"fnot.pi f8, f24\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 960(x23)\n"
			"flw.ps f3, 288(x23)\n"
			"fmul.pi f7, f12, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 640(x6)\n"
			"flw.ps f5, 960(x6)\n"
			"fmadd.ps f12, f18, f5, f5, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 832(x7)\n"
			"fswizz.ps f27, f23, 0x17\n" 
			"li x25, 0x135a970227e145bf\n"
			"fcvt.s.w f11, x25, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f7, 320(x25)\n"
			"flw.ps f12, 288(x25)\n"
			"fadd.pi f11, f7, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_9:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 288(x10)\n"
			"flw.ps f1, 608(x10)\n"
			"fcmov.ps f13, f16, f1, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 448(x6)\n"
			"flw.ps f1, 960(x6)\n"
			"fadd.ps f18, f16, f1, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 224(x7)\n"
			"flw.ps f1, 32(x7)\n"
			"flt.s x17, f11, f1\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 384(x16)\n"
			"faddi.pi f24, f17, 0xb\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 224(x6)\n"
			"flw.ps f7, 608(x6)\n"
			"flw.ps f18, 768(x6)\n"
			"fnmadd.s f8, f8, f7, f18, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 960(x25)\n"
			"flw.ps f16, 768(x25)\n"
			"flw.ps f11, 384(x25)\n"
			"fnmadd.s f17, f24, f16, f11, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 320(x17)\n"
			"flw.ps f12, 672(x17)\n"
			"fadd.pi f1, f27, f12\n" 
			"mova.x.m x16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 192(x10)\n"
			"fsin.ps f7, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 320(x26)\n"
			"flw.ps f1, 928(x26)\n"
			"fsub.ps f3, f8, f1, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 160(x26)\n"
			"flw.ps f23, 192(x26)\n"
			"flw.ps f27, 864(x26)\n"
			"fmadd.ps f11, f18, f23, f27, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"feqm.ps m2, f8, f1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8982a5230e\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_9:\n"
			"fsetm.pi m2, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 704(x16)\n"
			"flw.ps f27, 960(x16)\n"
			"fmax.s f24, f17, f27\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 192(x20)\n"
			"fsrli.pi f30, f27, 0xf\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 192(x16)\n"
			"fcvt.ps.pwu f3, f7, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 320(x26)\n"
			"flw.ps f5, 448(x26)\n"
			"fmin.pi f23, f23, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 672(x25)\n"
			"flw.ps f29, 800(x25)\n"
			"fsub.ps f1, f7, f29, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_9:\n"
			"masknot m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 640(x6)\n"
			"frsq.ps f1, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 992(x6)\n"
			"fround.ps f16, f5, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 800(x5)\n"
			"flw.ps f12, 448(x5)\n"
			"flw.ps f7, 704(x5)\n"
			"fnmadd.ps f8, f13, f12, f7, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 32(x20)\n"
			"flw.ps f29, 832(x20)\n"
			"flw.ps f23, 576(x20)\n"
			"fmsub.s f30, f18, f29, f23, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 768(x10)\n"
			"fsatu8.pi f27, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 512(x26)\n"
			"fcvt.lu.s x7, f3, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f3, 384(x26)\n"
			"flw.ps f27, 768(x26)\n"
			"fmin.pi f12, f3, f27\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 224(x20)\n"
			"flw.ps f23, 928(x20)\n"
			"fsgnjn.ps f30, f12, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 448(x7)\n"
			"flw.ps f18, 704(x7)\n"
			"fsgnj.s f5, f16, f18\n" 
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 448(x25)\n"
			"flw.ps f12, 608(x25)\n"
			"fsgnjx.s f29, f11, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 224(x20)\n"
			"flw.ps f7, 704(x20)\n"
			"flw.ps f3, 640(x20)\n"
			"fmsub.ps f27, f13, f7, f3, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 512(x7)\n"
			"flw.ps f30, 640(x7)\n"
			"flw.ps f17, 256(x7)\n"
			"fnmadd.s f18, f23, f30, f17, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 416(x10)\n"
			"flw.ps f27, 224(x10)\n"
			"fcmovm.ps f18, f16, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 736(x26)\n"
			"fcvt.wu.s x10, f30, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 288(x17)\n"
			"flw.ps f12, 512(x17)\n"
			"fmul.s f8, f24, f12, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 544(x6)\n"
			"fsgnj.s f12, f24, f24\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f7, 896(x17)\n"
			"fcvt.ps.f16 f7, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_9:\n"
			"fsetm.pi m1, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 512(x23)\n"
			"flw.ps f16, 640(x23)\n"
			"fsgnj.ps f7, f12, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 928(x16)\n"
			"flw.ps f16, 672(x16)\n"
			"fminu.pi f17, f5, f16\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 384(x23)\n"
			"fdivu.pi f18, f30, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 320(x6)\n"
			"fclass.ps f11, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 800(x17)\n"
			"fcvt.wu.s x16, f12, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 0(x25)\n"
			"fsin.ps f24, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 768(x26)\n"
			"flw.ps f11, 416(x26)\n"
			"fsrl.pi f13, f13, f11\n" 
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
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_9:\n"
			"masknot m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f27, 128(x27)\n"
			"fsatu8.pi f8, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 800(x27)\n"
			"flw.ps f23, 352(x27)\n"
			"fadd.s f7, f30, f23, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 512(x7)\n"
			"flw.ps f8, 864(x7)\n"
			"fmax.s f18, f7, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 320(x5)\n"
			"fsatu8.pi f7, f17\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 416(x7)\n"
			"flw.ps f29, 192(x7)\n"
			"flw.ps f8, 928(x7)\n"
			"fcmov.ps f24, f30, f29, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 960(x26)\n"
			"flw.ps f3, 736(x26)\n"
			"fsub.s f1, f18, f3, rdn\n" 
			"li x20, 0xb0312ae5435fd3bd\n"
			"fcvt.s.w f12, x20, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 736(x6)\n"
			"flw.ps f5, 640(x6)\n"
			"flw.ps f30, 704(x6)\n"
			"fnmsub.ps f11, f23, f5, f30, rdn\n" 
			"mova.x.m x6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 480(x5)\n"
			"flw.ps f18, 352(x5)\n"
			"fsgnjn.ps f23, f16, f18\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 800(x27)\n"
			"flw.ps f18, 320(x27)\n"
			"flw.ps f29, 224(x27)\n"
			"fmadd.ps f30, f5, f18, f29, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 544(x10)\n"
			"frsq.ps f8, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 160(x17)\n"
			"fcvt.ps.pwu f5, f11, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 160(x20)\n"
			"flw.ps f27, 128(x20)\n"
			"fsgnjx.s f24, f30, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 960(x10)\n"
			"flw.ps f30, 928(x10)\n"
			"flw.ps f3, 544(x10)\n"
			"fnmadd.s f11, f13, f30, f3, rup\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 224(x26)\n"
			"flw.ps f16, 288(x26)\n"
			"fsgnj.ps f6, f31, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 960(x17)\n"
			"flw.ps f2, 864(x17)\n"
			"flw.ps f4, 288(x17)\n"
			"fmsub.s f17, f31, f2, f4, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 800(x17)\n"
			"flw.ps f8, 32(x17)\n"
			"fxor.pi f30, f9, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 32(x9)\n"
			"fcvt.pw.ps f19, f5, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 832(x26)\n"
			"flw.ps f15, 800(x26)\n"
			"fsub.ps f25, f6, f15, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 192(x25)\n"
			"flw.ps f2, 800(x25)\n"
			"fsrl.pi f25, f0, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"flem.ps m7, f31, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f20, 864(x17)\n"
			"flw.ps f9, 736(x17)\n"
			"fmax.pi f15, f20, f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 896(x14)\n"
			"flw.ps f19, 928(x14)\n"
			"flw.ps f2, 928(x14)\n"
			"fnmadd.s f8, f9, f19, f2, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 640(x16)\n"
			"flw.ps f19, 64(x16)\n"
			"fle.s x26, f5, f19\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 320(x13)\n"
			"fsrli.pi f19, f25, 0x6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 0(x14)\n"
			"flw.ps f2, 384(x14)\n"
			"fsgnjx.s f9, f17, f2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 192(x27)\n"
			"flw.ps f31, 928(x27)\n"
			"fsgnjx.ps f2, f6, f31\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 832(x16)\n"
			"flw.ps f2, 896(x16)\n"
			"fadd.pi f20, f30, f2\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 672(x13)\n"
			"fexp.ps f16, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 96(x6)\n"
			"flw.ps f25, 320(x6)\n"
			"feq.s x20, f31, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_652616_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x72 << TENSOR_IMA_SCP_LOC_B) | (0x49 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 32(x20)\n"
			"flw.ps f5, 576(x20)\n"
			"flw.ps f19, 736(x20)\n"
			"fmsub.s f8, f4, f5, f19, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 928(x13)\n"
			"fsrli.pi f4, f19, 0x6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 64(x27)\n"
			"flw.ps f20, 640(x27)\n"
			"fnmadd.ps f27, f6, f20, f6, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f25, 96(x20)\n"
			"flw.ps f20, 448(x20)\n"
			"fmax.pi f19, f25, f20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 576(x13)\n"
			"flw.ps f27, 544(x13)\n"
			"flw.ps f31, 864(x13)\n"
			"fnmadd.ps f2, f5, f27, f31, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 160(x27)\n"
			"fcvt.pw.ps f6, f25, rdn\n" 
			"li x5, 0x7831\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 352(x17)\n"
			"flw.ps f31, 512(x17)\n"
			"flw.ps f17, 288(x17)\n"
			"fmadd.ps f25, f0, f31, f17, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 768(x6)\n"
			"faddi.pi f25, f8, 0x158\n" 
			"fbci.ps f20, 0x2fdfb\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_10:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 544(x25)\n"
			"flog.ps f31, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f4, 64(x25)\n"
			"fandi.pi f15, f4, 0xd0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"flem.ps m3, f8, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"maskxor m3, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f2, 896(x13)\n"
			"flw.ps f19, 896(x13)\n"
			"fmin.pi f2, f2, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 576(x17)\n"
			"flw.ps f31, 672(x17)\n"
			"fsra.pi f15, f6, f31\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 288(x13)\n"
			"fnot.pi f25, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x16, 0x36000000063c000a\n"
			"csrw tensor_reduce, x16\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_868576_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_400205_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_643050_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
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
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 480(x6)\n"
			"fcvt.pw.ps f8, f8, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 512(x25)\n"
			"fcvt.l.s x27, f5, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_10:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_10:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 288(x20)\n"
			"frcp.ps f17, f25\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 992(x16)\n"
			"flw.ps f8, 864(x16)\n"
			"fsgnjx.ps f9, f2, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopcz x13, m3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_10:\n"
			"fsetm.pi m1, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_10:\n"
			"feqm.ps m7, f17, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f16, 32(x20)\n"
			"feq.pi f0, f16, f16\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 416(x17)\n"
			"flw.ps f16, 576(x17)\n"
			"fsra.pi f25, f30, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_16\n"
			"flw.ps f6, 864(x9)\n"
			"fcvt.ps.f16 f17, f6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 640(x14)\n"
			"fclass.s x27, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 480(x25)\n"
			"flw.ps f4, 512(x25)\n"
			"feq.s x6, f17, f4\n" 
			"fbci.pi f17, 0x7de8b\n" 
			"li x5, 0x1ac5\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_FP_TRANS_ID_9_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 320(x26)\n"
			"fexp.ps f17, f8\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_10:\n"
			"maskand m1, m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_707953_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x30\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"maskpopc x14, m1\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 352(x9)\n"
			"flw.ps f6, 480(x9)\n"
			"fsrl.pi f5, f20, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 96(x25)\n"
			"flw.ps f6, 736(x25)\n"
			"flw.ps f31, 480(x25)\n"
			"fnmsub.s f27, f20, f6, f31, rne\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f30, 192(x9)\n"
			"fcvt.ps.f16 f31, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 352(x9)\n"
			"flw.ps f9, 224(x9)\n"
			"fle.pi f27, f31, f9\n" 
			"li x5, 0xf83d\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 256(x20)\n"
			"flw.ps f20, 608(x20)\n"
			"feq.s x13, f27, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 384(x9)\n"
			"flw.ps f16, 480(x9)\n"
			"fsgnj.s f9, f30, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 288(x9)\n"
			"fnot.pi f17, f16\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 96(x13)\n"
			"flw.ps f9, 992(x13)\n"
			"feq.pi f5, f25, f9\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 320(x27)\n"
			"flw.ps f17, 832(x27)\n"
			"fadd.ps f20, f2, f17, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 544(x9)\n"
			"flw.ps f31, 736(x9)\n"
			"fsgnj.ps f20, f17, f31\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 736(x26)\n"
			"flw.ps f0, 512(x26)\n"
			"fmulhu.pi f15, f17, f0\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 160(x17)\n"
			"flw.ps f0, 128(x17)\n"
			"flt.pi f19, f19, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 256(x27)\n"
			"fcvt.pw.ps f15, f27, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 512(x9)\n"
			"flw.ps f5, 128(x9)\n"
			"fsub.pi f4, f27, f5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 512(x6)\n"
			"flw.ps f25, 896(x6)\n"
			"fxor.pi f30, f4, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_934774_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x4000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x3\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 224(x14)\n"
			"flw.ps f9, 384(x14)\n"
			"fsub.s f27, f30, f9, rup\n" 
			"li x17, 0xca0ccab2356ff4fe\n"
			"fcvt.s.w f27, x17, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f20, 768(x20)\n"
			"flw.ps f9, 352(x20)\n"
			"fmulh.pi f19, f20, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 224(x16)\n"
			"fandi.pi f17, f6, 0x18c\n" 
			"li x5, 0xcacf\n"
			"csrw tensor_mask, x5\n"	
			"fbci.pi f0, 0x572df\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 96(x17)\n"
			"flw.ps f5, 992(x17)\n"
			"fmax.ps f5, f19, f5\n" 
			"li x5, 0x82ba\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 832(x26)\n"
			"flw.ps f5, 320(x26)\n"
			"fsgnjx.ps f16, f15, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f30, 960(x9)\n"
			"flw.ps f27, 864(x9)\n"
			"fmax.pi f15, f30, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 480(x14)\n"
			"flw.ps f27, 352(x14)\n"
			"for.pi f9, f0, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 736(x25)\n"
			"flw.ps f20, 32(x25)\n"
			"fdiv.s f16, f30, f20, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_10:\n"
			"fltm.pi m7, f19, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_10:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 672(x13)\n"
			"flog.ps f9, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 256(x9)\n"
			"flw.ps f8, 224(x9)\n"
			"flw.ps f17, 512(x9)\n"
			"fmadd.s f15, f20, f8, f17, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 416(x20)\n"
			"flw.ps f15, 544(x20)\n"
			"fmadd.s f31, f6, f15, f6, rmm\n" 
			"li x5, 0xe367267e7879b3c6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x25bac54ee8a594ef\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 864(x13)\n"
			"fround.ps f30, f31, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 704(x27)\n"
			"flw.ps f17, 544(x27)\n"
			"flt.pi f19, f8, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f17, 736(x16)\n"
			"fsatu8.pi f5, f17\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 160(x13)\n"
			"flw.ps f15, 160(x13)\n"
			"for.pi f27, f19, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 448(x20)\n"
			"flw.ps f30, 896(x20)\n"
			"fnmsub.ps f15, f16, f30, f30, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 960(x16)\n"
			"flw.ps f19, 640(x16)\n"
			"fadd.pi f30, f27, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 896(x16)\n"
			"flw.ps f31, 256(x16)\n"
			"flw.ps f6, 96(x16)\n"
			"fnmsub.s f15, f15, f31, f6, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f30, 960(x25)\n"
			"flw.ps f8, 224(x25)\n"
			"for.pi f27, f30, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 480(x27)\n"
			"fmin.ps f30, f27, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 704(x17)\n"
			"flw.ps f2, 768(x17)\n"
			"fltu.pi f6, f6, f2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f19, 352(x16)\n"
			"fsrli.pi f20, f19, 0x5\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 800(x16)\n"
			"fsin.ps f20, f0\n" 
			"li x5, 0xc662\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_10:\n"
			"maskor m3, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 96(x16)\n"
			"flw.ps f27, 928(x16)\n"
			"fsub.s f20, f2, f27, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 768(x25)\n"
			"flw.ps f15, 96(x25)\n"
			"flw.ps f19, 480(x25)\n"
			"fnmsub.ps f27, f20, f15, f19, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 288(x13)\n"
			"flw.ps f4, 192(x13)\n"
			"fle.s x17, f8, f4\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 992(x26)\n"
			"ffrc.ps f6, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 960(x16)\n"
			"fsrai.pi f25, f25, 0x2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_2_HID_10:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"fltm.pi m1, f29, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 256(x10)\n"
			"flw.ps f23, 224(x10)\n"
			"flw.ps f19, 800(x10)\n"
			"fnmadd.ps f13, f26, f23, f19, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f16, 96(x5)\n"
			"flw.ps f7, 640(x5)\n"
			"feq.pi f29, f16, f7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 608(x15)\n"
			"fsat8.pi f15, f4\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 256(x26)\n"
			"fcvt.wu.s x25, f28, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 608(x17)\n"
			"flw.ps f15, 800(x17)\n"
			"fsgnj.s f31, f31, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 352(x26)\n"
			"fcvt.lu.s x17, f23, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 896(x25)\n"
			"fswizz.ps f26, f27, 0x1d\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 480(x26)\n"
			"flw.ps f23, 704(x26)\n"
			"fsgnj.s f11, f13, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskxor m6, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f26, 352(x25)\n"
			"flw.ps f30, 864(x25)\n"
			"fmax.pi f19, f26, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 576(x15)\n"
			"fclass.ps f26, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 640(x22)\n"
			"flw.ps f15, 704(x22)\n"
			"fmin.s f15, f9, f15\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_11:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 928(x27)\n"
			"flog.ps f7, f31\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 896(x27)\n"
			"fsra.pi f19, f19, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f7, 704(x15)\n"
			"flw.ps f9, 160(x15)\n"
			"for.pi f15, f7, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f11, 736(x28)\n"
			"fsrli.pi f27, f11, 0x0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"fltm.pi m6, f9, f4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xcbc857c655\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_11:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 864(x15)\n"
			"flog.ps f10, f4\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 288(x5)\n"
			"flw.ps f16, 0(x5)\n"
			"fsgnj.ps f30, f26, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 128(x14)\n"
			"flw.ps f26, 256(x14)\n"
			"fmadd.ps f10, f29, f26, f26, rdn\n" 
			"mova.x.m x15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 448(x25)\n"
			"flw.ps f28, 800(x25)\n"
			"flt.ps f19, f29, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"flem.ps m4, f4, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_11:\n"
			"flem.ps m5, f23, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f28, 928(x27)\n"
			"flw.ps f13, 96(x27)\n"
			"fltu.pi f31, f28, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_11:\n"
			"maskor m5, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x25, 0x8fcc6a31696b9282\n"
			"fcvt.s.lu f30, x25, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 160(x26)\n"
			"flw.ps f27, 416(x26)\n"
			"fmin.s f13, f23, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 320(x17)\n"
			"frsq.ps f23, f9\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 576(x25)\n"
			"fclass.ps f4, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 960(x28)\n"
			"fsatu8.pi f13, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 96(x5)\n"
			"flw.ps f11, 992(x5)\n"
			"fsub.ps f15, f7, f11, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 448(x10)\n"
			"flw.ps f4, 256(x10)\n"
			"flw.ps f28, 192(x10)\n"
			"fmadd.ps f30, f29, f4, f28, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 0(x26)\n"
			"flw.ps f11, 736(x26)\n"
			"fsub.pi f28, f28, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 512(x22)\n"
			"fcvt.f16.ps f27, f13\n" 
			"li x27, 0x2d0b25f447578953\n"
			"fcvt.s.wu f16, x27, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_11:\n"
			"fsetm.pi m4, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f16, 256(x14)\n"
			"fcvt.ps.pwu f30, f16, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 992(x15)\n"
			"fround.ps f31, f26, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 704(x26)\n"
			"flw.ps f26, 992(x26)\n"
			"fmin.pi f27, f4, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 832(x28)\n"
			"flw.ps f4, 480(x28)\n"
			"fsgnjx.ps f9, f16, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 608(x15)\n"
			"flw.ps f9, 224(x15)\n"
			"flt.ps f7, f15, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 704(x28)\n"
			"fpackreph.pi f27, f9\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 800(x25)\n"
			"fclass.s x27, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 448(x28)\n"
			"flw.ps f9, 256(x28)\n"
			"fle.s x7, f28, f9\n" 
			"fbci.pi f30, 0x3e6b2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 288(x15)\n"
			"flw.ps f7, 96(x15)\n"
			"fsgnjx.s f9, f23, f7\n" 
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
			"la x25, rand_ieee754_16\n"
			"flw.ps f23, 416(x25)\n"
			"fcvt.ps.f16 f27, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 224(x7)\n"
			"flw.ps f4, 608(x7)\n"
			"fltu.pi f7, f19, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 960(x14)\n"
			"flw.ps f16, 640(x14)\n"
			"fsgnjx.ps f7, f13, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 992(x7)\n"
			"fcvt.ps.pw f19, f13, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 32(x26)\n"
			"frsq.ps f30, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 96(x27)\n"
			"fclass.s x5, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 640(x26)\n"
			"fcvt.pwu.ps f27, f29, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 736(x17)\n"
			"flw.ps f31, 704(x17)\n"
			"fsra.pi f28, f29, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 160(x14)\n"
			"flw.ps f23, 352(x14)\n"
			"fsrl.pi f10, f31, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 768(x17)\n"
			"fclass.ps f28, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 544(x7)\n"
			"flw.ps f19, 928(x7)\n"
			"fmulh.pi f30, f29, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 736(x28)\n"
			"fcvt.w.s x5, f10, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 320(x15)\n"
			"feq.s x10, f26, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 960(x26)\n"
			"flw.ps f23, 736(x26)\n"
			"fmulhu.pi f30, f15, f23\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_11:\n"
			"mov.m.x m6, x22, 0x4a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 384(x10)\n"
			"flw.ps f4, 992(x10)\n"
			"feq.s x28, f19, f4\n" 
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 608(x17)\n"
			"flw.ps f13, 736(x17)\n"
			"flw.ps f7, 0(x17)\n"
			"fmsub.s f9, f16, f13, f7, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 544(x7)\n"
			"flw.ps f26, 128(x7)\n"
			"feq.ps f19, f31, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 224(x22)\n"
			"flw.ps f15, 832(x22)\n"
			"flt.ps f10, f13, f15\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_11:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 608(x5)\n"
			"frcp.ps f28, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 608(x28)\n"
			"flw.ps f16, 0(x28)\n"
			"flw.ps f7, 448(x28)\n"
			"fmadd.ps f7, f9, f16, f7, rtz\n" 
			"fbci.ps f7, 0x69f3a\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 704(x15)\n"
			"flw.ps f7, 288(x15)\n"
			"fmax.ps f13, f4, f7\n" 
			"maskpopcz x15, m6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 544(x14)\n"
			"fcvt.wu.s x22, f11, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 160(x5)\n"
			"flw.ps f16, 576(x5)\n"
			"fsgnjx.ps f27, f27, f16\n" 
			"fbci.ps f7, 0x3426d\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 256(x25)\n"
			"fandi.pi f28, f9, 0x165\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 864(x15)\n"
			"flw.ps f28, 704(x15)\n"
			"fsll.pi f15, f27, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 320(x10)\n"
			"fcvt.wu.s x17, f31, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 256(x22)\n"
			"fsatu8.pi f30, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 608(x28)\n"
			"fcvt.wu.s x27, f23, rup\n" 
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
			"la x14, rand_int_32\n"
			"flw.ps f16, 224(x14)\n"
			"flw.ps f15, 64(x14)\n"
			"fmulh.pi f29, f16, f15\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_11:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 736(x28)\n"
			"flog.ps f11, f29\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 160(x17)\n"
			"flw.ps f19, 832(x17)\n"
			"fsll.pi f4, f30, f19\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_11:\n"
			"maskand m6, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 352(x26)\n"
			"flw.ps f16, 480(x26)\n"
			"fsgnjx.ps f30, f29, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 448(x25)\n"
			"flw.ps f30, 160(x25)\n"
			"fmin.s f13, f23, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 896(x7)\n"
			"flw.ps f31, 576(x7)\n"
			"fmul.s f23, f13, f31, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 672(x25)\n"
			"fround.ps f29, f29, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 832(x15)\n"
			"flw.ps f30, 896(x15)\n"
			"fle.pi f30, f23, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 928(x27)\n"
			"flw.ps f30, 32(x27)\n"
			"fsgnjn.ps f9, f10, f30\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_11:\n"
			"feqm.ps m5, f4, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f4, 928(x25)\n"
			"flw.ps f15, 480(x25)\n"
			"fand.pi f7, f4, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 352(x28)\n"
			"flw.ps f19, 512(x28)\n"
			"flw.ps f28, 640(x28)\n"
			"fmsub.ps f4, f27, f19, f28, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 928(x22)\n"
			"fswizz.ps f31, f27, 0x1d\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_11:\n"
			"flem.ps m5, f29, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 480(x17)\n"
			"flw.ps f11, 672(x17)\n"
			"fmin.s f9, f29, f11\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
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
			"la x7, rand_int_32\n"
			"flw.ps f20, 288(x7)\n"
			"fsat8.pi f23, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 640(x23)\n"
			"fsrai.pi f8, f16, 0xe\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 576(x5)\n"
			"flw.ps f11, 928(x5)\n"
			"fsrl.pi f26, f23, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 0(x5)\n"
			"flw.ps f10, 864(x5)\n"
			"flw.ps f18, 512(x5)\n"
			"fnmadd.ps f8, f23, f10, f18, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 0(x5)\n"
			"fcvt.f16.ps f13, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 928(x7)\n"
			"fcvt.pwu.ps f18, f23, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 512(x5)\n"
			"flw.ps f7, 160(x5)\n"
			"fsgnjn.ps f8, f28, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"feqm.ps m7, f7, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"fltm.pi m3, f0, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 192(x28)\n"
			"flw.ps f13, 704(x28)\n"
			"fmul.s f10, f3, f13, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 640(x23)\n"
			"fandi.pi f18, f10, 0x196\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"maskand m7, m3, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2c64682c49\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f7, 224(x5)\n"
			"fsat8.pi f27, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 608(x17)\n"
			"flw.ps f27, 800(x17)\n"
			"fsgnjx.s f20, f16, f27\n" 
			"la x7, rand_int_32\n"
			"flw.ps f3, 992(x7)\n"
			"flw.ps f10, 992(x7)\n"
			"fmax.pi f16, f3, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 608(x28)\n"
			"flw.ps f18, 480(x28)\n"
			"fminu.pi f3, f26, f18\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 672(x22)\n"
			"fclass.ps f8, f7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 512(x7)\n"
			"flw.ps f24, 32(x7)\n"
			"flw.ps f7, 832(x7)\n"
			"fmsub.ps f7, f11, f24, f7, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_12:\n"
			"mov.m.x m3, x10, 0x7c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 448(x17)\n"
			"flw.ps f0, 672(x17)\n"
			"fsgnjx.ps f0, f16, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 64(x10)\n"
			"faddi.pi f13, f7, 0x13d\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 224(x17)\n"
			"fpackrepb.pi f11, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 256(x14)\n"
			"flw.ps f3, 704(x14)\n"
			"fle.ps f24, f7, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 320(x5)\n"
			"fcvt.f16.ps f18, f27\n" 
			"la x10, rand_int_32\n"
			"flw.ps f10, 544(x10)\n"
			"flw.ps f28, 832(x10)\n"
			"fadd.pi f11, f10, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 32(x17)\n"
			"fsrli.pi f8, f13, 0x5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 608(x17)\n"
			"flw.ps f7, 896(x17)\n"
			"fcmovm.ps f11, f26, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 192(x28)\n"
			"fswizz.ps f23, f8, 0x7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_12:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 992(x22)\n"
			"fexp.ps f24, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 512(x14)\n"
			"flw.ps f16, 640(x14)\n"
			"fsgnjx.ps f24, f24, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_12:\n"
			"masknot m0, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x12, 0x36000000063c000a\n"
			"csrw tensor_reduce, x12\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_12217_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_854366_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 832(x17)\n"
			"flw.ps f16, 832(x17)\n"
			"fmax.ps f23, f23, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f22, 960(x7)\n"
			"flw.ps f23, 448(x7)\n"
			"fsrl.pi f3, f22, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 992(x5)\n"
			"flw.ps f11, 288(x5)\n"
			"fsgnjn.ps f22, f10, f11\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 992(x28)\n"
			"flw.ps f10, 64(x28)\n"
			"fxor.pi f28, f18, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 0(x22)\n"
			"flw.ps f26, 960(x22)\n"
			"fmaxu.pi f23, f24, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_12:\n"
			"maskor m3, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 512(x10)\n"
			"fsgnj.s f20, f10, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 896(x22)\n"
			"flw.ps f26, 800(x22)\n"
			"flw.ps f8, 128(x22)\n"
			"fmadd.s f7, f7, f26, f8, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 736(x20)\n"
			"fcvt.f16.ps f3, f3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_12:\n"
			"fltm.ps m3, f13, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f11, 704(x20)\n"
			"fsatu8.pi f18, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 224(x5)\n"
			"fround.ps f20, f26, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 768(x14)\n"
			"flw.ps f23, 576(x14)\n"
			"fle.ps f8, f16, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 224(x23)\n"
			"flw.ps f27, 544(x23)\n"
			"flw.ps f11, 256(x23)\n"
			"fnmadd.ps f24, f23, f27, f11, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 320(x10)\n"
			"flw.ps f16, 160(x10)\n"
			"fsrl.pi f7, f22, f16\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 160(x5)\n"
			"fsrli.pi f22, f28, 0x4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x71 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"mova.x.m x12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 288(x7)\n"
			"fsub.s f20, f0, f0, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_12:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 224(x22)\n"
			"flw.ps f8, 512(x22)\n"
			"fcmovm.ps f26, f27, f8\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 544(x7)\n"
			"flw.ps f24, 224(x7)\n"
			"fminu.pi f11, f26, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 704(x20)\n"
			"fslli.pi f7, f16, 0x9\n" 
			"li x20, 0x3876c18406224122\n"
			"fcvt.s.wu f27, x20, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 544(x22)\n"
			"flw.ps f7, 800(x22)\n"
			"feq.s x10, f16, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 448(x17)\n"
			"flw.ps f13, 32(x17)\n"
			"flw.ps f24, 640(x17)\n"
			"fmsub.s f23, f22, f13, f24, rtz\n" 
			"fbci.ps f10, 0x6ebe6\n" 
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_12:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 928(x17)\n"
			"frcp.ps f28, f11\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 480(x14)\n"
			"flw.ps f22, 224(x14)\n"
			"fmax.pi f3, f28, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 96(x10)\n"
			"flw.ps f27, 800(x10)\n"
			"flw.ps f8, 736(x10)\n"
			"fnmadd.s f16, f23, f27, f8, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 320(x20)\n"
			"fnot.pi f20, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 512(x10)\n"
			"fnot.pi f3, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 352(x14)\n"
			"fmulhu.pi f22, f27, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x29 << TENSOR_IMA_SCP_LOC_B) | (0xcc << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 672(x14)\n"
			"fround.ps f23, f23, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 608(x22)\n"
			"flw.ps f24, 128(x22)\n"
			"flw.ps f10, 32(x22)\n"
			"fmadd.s f0, f22, f24, f10, rdn\n" 
			"la x20, rand_ieee754_16\n"
			"flw.ps f8, 960(x20)\n"
			"fcvt.ps.f16 f3, f8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_12:\n"
			"fsetm.pi m2, f11\n" 
			"li x5, 0x80340180\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 928(x5)\n"
			"flw.ps f8, 704(x5)\n"
			"fsub.ps f20, f20, f8, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_12:\n"
			"fltm.ps m0, f11, f26\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x49912fa478\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 32(x22)\n"
			"flw.ps f10, 320(x22)\n"
			"fsgnj.ps f28, f23, f10\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 480(x17)\n"
			"flw.ps f26, 256(x17)\n"
			"fmul.pi f13, f22, f26\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_12:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 160(x12)\n"
			"flw.ps f28, 672(x12)\n"
			"fsub.ps f22, f24, f28, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f28, 928(x28)\n"
			"fnot.pi f28, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 896(x20)\n"
			"fsin.ps f22, f26\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_12:\n"
			"fltm.ps m7, f3, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f16, 96(x23)\n"
			"flw.ps f13, 640(x23)\n"
			"for.pi f11, f16, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 64(x23)\n"
			"flw.ps f22, 32(x23)\n"
			"fsgnjx.ps f23, f20, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x12, 0xff4c387555aa8e67\n"
			"fcvt.s.w f23, x12, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x5a79\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_376105_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb3 << TENSOR_IMA_SCP_LOC_B) | (0x76 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f8, 704(x17)\n"
			"flw.ps f11, 64(x17)\n"
			"fsrl.pi f28, f8, f11\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 736(x12)\n"
			"faddi.pi f23, f18, 0x62\n" 
			"li x5, 0xedd9\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 96(x20)\n"
			"flw.ps f24, 0(x20)\n"
			"feq.s x20, f7, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f22, 256(x12)\n"
			"flw.ps f10, 352(x12)\n"
			"fle.pi f23, f22, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 992(x7)\n"
			"fcvt.ps.pw f28, f20, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 992(x20)\n"
			"flw.ps f24, 768(x20)\n"
			"feq.ps f22, f26, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 736(x22)\n"
			"flw.ps f3, 896(x22)\n"
			"fsub.ps f7, f26, f3, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 128(x10)\n"
			"flw.ps f24, 32(x10)\n"
			"fle.pi f7, f26, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 768(x5)\n"
			"flw.ps f8, 160(x5)\n"
			"fmul.ps f8, f27, f8, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_12:\n"
			"maskor m3, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x17, 0x5e90c202207b58a6\n"
			"fcvt.s.wu f24, x17, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 960(x14)\n"
			"flw.ps f7, 96(x14)\n"
			"fsub.pi f13, f28, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 480(x17)\n"
			"flw.ps f28, 448(x17)\n"
			"flw.ps f22, 160(x17)\n"
			"fmsub.ps f3, f7, f28, f22, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 64(x23)\n"
			"flw.ps f27, 736(x23)\n"
			"fmaxu.pi f7, f28, f27\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_12:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f0, 64(x23)\n"
			"flw.ps f16, 928(x23)\n"
			"fmul.pi f16, f0, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x8c54\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_781781_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_200357_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_811763_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1f << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
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
			"maskpopc x26, m4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 992(x15)\n"
			"flw.ps f10, 672(x15)\n"
			"flw.ps f24, 672(x15)\n"
			"fmsub.ps f16, f12, f10, f24, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 672(x27)\n"
			"flw.ps f6, 64(x27)\n"
			"fle.s x16, f7, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 192(x9)\n"
			"fcvt.wu.s x25, f13, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 896(x23)\n"
			"fcvt.lu.s x23, f13, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"fltm.ps m4, f24, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"feqm.ps m4, f2, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 640(x22)\n"
			"fcvt.pw.ps f18, f16, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 32(x18)\n"
			"fswizz.ps f24, f25, 0x1b\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"fsetm.pi m7, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_13:\n"
			"feqm.ps m0, f14, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x21c07d0071\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f10, 448(x14)\n"
			"fmax.pi f16, f10, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_13:\n"
			"fsetm.pi m6, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x22, 0x79ab357d9452f56e\n"
			"fcvt.s.l f6, x22, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 384(x18)\n"
			"flw.ps f10, 224(x18)\n"
			"fmax.pi f23, f14, f10\n" 
			"la x27, rand_int_32\n"
			"flw.ps f6, 288(x27)\n"
			"flw.ps f12, 192(x27)\n"
			"fmulhu.pi f7, f6, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"mova.x.m x25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 640(x22)\n"
			"flw.ps f23, 512(x22)\n"
			"flt.s x27, f7, f23\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f7, 576(x18)\n"
			"fcvt.ps.f16 f25, f7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 704(x27)\n"
			"flw.ps f14, 384(x27)\n"
			"fmul.ps f9, f23, f14, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 640(x9)\n"
			"flw.ps f30, 800(x9)\n"
			"fsgnjn.s f1, f18, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 0(x22)\n"
			"flw.ps f23, 896(x22)\n"
			"fxor.pi f30, f7, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 992(x25)\n"
			"flw.ps f12, 704(x25)\n"
			"flw.ps f25, 608(x25)\n"
			"fmsub.ps f14, f7, f12, f25, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 480(x9)\n"
			"flw.ps f13, 224(x9)\n"
			"fsgnjn.s f12, f9, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 864(x27)\n"
			"flw.ps f23, 384(x27)\n"
			"flt.s x26, f1, f23\n" 
			"fbci.ps f18, 0x6c9ea\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"maskxor m4, m0, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 64(x26)\n"
			"fmin.ps f3, f16, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 800(x15)\n"
			"flw.ps f16, 928(x15)\n"
			"flw.ps f30, 896(x15)\n"
			"fmsub.ps f25, f6, f16, f30, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 288(x22)\n"
			"fcvt.pw.ps f24, f3, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_13:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f24, 0(x15)\n"
			"flw.ps f12, 32(x15)\n"
			"fadd.pi f23, f24, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"fbci.ps f16, 0xb236\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 224(x22)\n"
			"fsub.s f18, f14, f14, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 512(x14)\n"
			"fsin.ps f25, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 256(x16)\n"
			"fcvt.ps.pwu f10, f6, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 96(x22)\n"
			"flw.ps f3, 192(x22)\n"
			"feq.pi f14, f13, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 64(x14)\n"
			"frsq.ps f6, f2\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 352(x18)\n"
			"flw.ps f12, 992(x18)\n"
			"fand.pi f23, f23, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 320(x25)\n"
			"flw.ps f18, 544(x25)\n"
			"fnmadd.ps f10, f9, f9, f18, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 608(x15)\n"
			"flw.ps f30, 320(x15)\n"
			"flw.ps f24, 960(x15)\n"
			"fnmsub.ps f23, f18, f30, f24, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 544(x27)\n"
			"fnot.pi f3, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 512(x27)\n"
			"flw.ps f30, 224(x27)\n"
			"flw.ps f14, 960(x27)\n"
			"fnmsub.s f24, f1, f30, f14, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_13:\n"
			"maskxor m4, m7, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 896(x18)\n"
			"fsat8.pi f10, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 640(x14)\n"
			"flw.ps f10, 320(x14)\n"
			"fmul.ps f9, f6, f10, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 224(x23)\n"
			"flw.ps f12, 128(x23)\n"
			"fmulh.pi f12, f24, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 512(x9)\n"
			"flw.ps f3, 928(x9)\n"
			"flw.ps f30, 928(x9)\n"
			"fmadd.s f12, f2, f3, f30, rne\n" 
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 416(x22)\n"
			"fsin.ps f18, f14\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_13:\n"
			"fsetm.pi m7, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 448(x23)\n"
			"fsrli.pi f3, f12, 0xa\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 672(x16)\n"
			"flw.ps f25, 864(x16)\n"
			"fsll.pi f3, f1, f25\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 608(x26)\n"
			"flw.ps f24, 800(x26)\n"
			"fmulhu.pi f1, f10, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 672(x14)\n"
			"flw.ps f18, 992(x14)\n"
			"fsra.pi f13, f12, f18\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 384(x22)\n"
			"flw.ps f25, 384(x22)\n"
			"flw.ps f13, 960(x22)\n"
			"fnmadd.s f6, f18, f25, f13, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 960(x25)\n"
			"fpackrepb.pi f10, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 672(x22)\n"
			"flw.ps f24, 128(x22)\n"
			"flw.ps f23, 0(x22)\n"
			"fmsub.ps f2, f18, f24, f23, rmm\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f14, 64(x14)\n"
			"fcvt.ps.f16 f10, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 224(x22)\n"
			"flw.ps f18, 352(x22)\n"
			"fand.pi f24, f10, f18\n" 
			"maskpopc x27, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x18, rand_int_32\n"
			"flw.ps f2, 192(x18)\n"
			"flw.ps f3, 736(x18)\n"
			"fsra.pi f18, f2, f3\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_13:\n"
			"fltm.pi m4, f3, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 704(x9)\n"
			"flw.ps f9, 352(x9)\n"
			"fsgnjn.ps f30, f10, f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 256(x14)\n"
			"flw.ps f3, 896(x14)\n"
			"fsgnjx.s f18, f18, f3\n" 
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
			"mov.m.x m6, x22, 0x7f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 128(x23)\n"
			"flw.ps f23, 288(x23)\n"
			"fsgnj.s f2, f1, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 608(x18)\n"
			"flw.ps f10, 256(x18)\n"
			"fsgnjx.s f1, f23, f10\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_13:\n"
			"maskxor m0, m0, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 416(x27)\n"
			"flw.ps f14, 384(x27)\n"
			"fmin.s f14, f7, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 256(x14)\n"
			"fpackreph.pi f13, f13\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_13:\n"
			"fltm.ps m4, f30, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 992(x25)\n"
			"flw.ps f18, 608(x25)\n"
			"fadd.ps f16, f6, f18, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 288(x14)\n"
			"flw.ps f30, 992(x14)\n"
			"fsub.pi f1, f2, f30\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_13:\n"
			"fltm.ps m7, f3, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 672(x15)\n"
			"flw.ps f7, 736(x15)\n"
			"fsgnjx.ps f1, f25, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 288(x16)\n"
			"flw.ps f16, 32(x16)\n"
			"fsgnj.ps f12, f6, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 480(x15)\n"
			"flw.ps f3, 672(x15)\n"
			"flw.ps f30, 928(x15)\n"
			"fmsub.ps f3, f23, f3, f30, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f25, 704(x15)\n"
			"flw.ps f2, 96(x15)\n"
			"fand.pi f10, f25, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 608(x26)\n"
			"fcvt.pwu.ps f25, f25, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 896(x23)\n"
			"fcvt.w.s x16, f3, rtz\n" 
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 160(x26)\n"
			"fsin.ps f10, f30\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_13:\n"
			"mov.m.x m6, x22, 0x5a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 64(x16)\n"
			"flw.ps f30, 544(x16)\n"
			"fle.ps f12, f10, f30\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 128(x18)\n"
			"flw.ps f18, 320(x18)\n"
			"flw.ps f16, 864(x18)\n"
			"fcmov.ps f14, f12, f18, f16\n" 
			"li x27, 0xe76bf93c9a60e2d2\n"
			"fcvt.s.w f6, x27, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 800(x22)\n"
			"flw.ps f13, 64(x22)\n"
			"fmaxu.pi f12, f14, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 32(x23)\n"
			"flw.ps f23, 736(x23)\n"
			"fmin.s f2, f16, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 352(x22)\n"
			"flw.ps f14, 320(x22)\n"
			"fsgnjn.s f24, f1, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 608(x25)\n"
			"fandi.pi f18, f2, 0xee\n" 
			"LBL_SEQID_5_FP_TRANS_ID_15_HID_13:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 768(x18)\n"
			"fexp.ps f30, f24\n" 
			"LBL_SEQID_5_FP_TRANS_ID_16_HID_13:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 320(x22)\n"
			"fexp.ps f25, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 672(x18)\n"
			"fclass.s x23, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 640(x25)\n"
			"flw.ps f16, 800(x25)\n"
			"fmin.ps f18, f24, f16\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 384(x25)\n"
			"flw.ps f24, 128(x25)\n"
			"fle.pi f13, f18, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 448(x16)\n"
			"fcvt.wu.s x14, f9, rtz\n" 
			"LBL_SEQID_5_FP_TRANS_ID_17_HID_13:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 448(x9)\n"
			"frcp.ps f12, f1\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 0(x12)\n"
			"flw.ps f3, 288(x12)\n"
			"fmin.ps f31, f30, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 192(x12)\n"
			"flw.ps f24, 160(x12)\n"
			"flw.ps f9, 32(x12)\n"
			"fmadd.s f10, f19, f24, f9, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 736(x27)\n"
			"flw.ps f30, 640(x27)\n"
			"fmul.s f13, f9, f30, rdn\n" 
			"li x19, 0xa00f462eab853b57\n"
			"fcvt.s.w f28, x19, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"fsetm.pi m6, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 640(x22)\n"
			"flw.ps f9, 704(x22)\n"
			"flw.ps f13, 416(x22)\n"
			"fcmov.ps f30, f25, f9, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 640(x10)\n"
			"flw.ps f3, 768(x10)\n"
			"fand.pi f31, f13, f3\n" 
			"li x5, 0x13b027773aa8a3cb\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x32c80ddd2de0070d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 544(x27)\n"
			"flw.ps f25, 416(x27)\n"
			"flw.ps f9, 864(x27)\n"
			"fnmadd.s f25, f8, f25, f9, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 288(x10)\n"
			"fle.s x10, f19, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 640(x15)\n"
			"flw.ps f30, 320(x15)\n"
			"fle.pi f31, f9, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 992(x11)\n"
			"fcvt.ps.pw f24, f0, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"masknot m2, m4\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_int_32\n"
			"flw.ps f31, 320(x16)\n"
			"fcvt.ps.pwu f28, f31, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 704(x26)\n"
			"fmax.pi f10, f24, f24\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_14:\n"
			"mov.m.x m6, x10, 0x71\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f30, 512(x27)\n"
			"flw.ps f16, 960(x27)\n"
			"fmul.pi f25, f30, f16\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x13 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 32(x23)\n"
			"fclass.ps f9, f13\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_14:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 224(x14)\n"
			"frcp.ps f3, f7\n" 
			"maskpopc x15, m4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 928(x11)\n"
			"flw.ps f13, 128(x11)\n"
			"fsgnj.s f28, f10, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 448(x26)\n"
			"fsrai.pi f7, f10, 0x7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 544(x10)\n"
			"flw.ps f9, 256(x10)\n"
			"fle.s x11, f16, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_14:\n"
			"fsetm.pi m2, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 512(x16)\n"
			"flw.ps f7, 512(x16)\n"
			"fmul.s f3, f30, f7, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 960(x16)\n"
			"flw.ps f31, 480(x16)\n"
			"fmul.ps f0, f5, f31, rtz\n" 
			"li x5, 0xf583cf9431bce6bf\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xda345d46a3a82516\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_14:\n"
			"flem.ps m6, f24, f9\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"li x22, 0x7ef6732b624a23e5\n"
			"fcvt.s.lu f8, x22, rdn\n" 
			"fbci.ps f0, 0x33ed2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 288(x15)\n"
			"fsin.ps f9, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 768(x26)\n"
			"flw.ps f30, 448(x26)\n"
			"fadd.pi f0, f8, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 384(x19)\n"
			"flw.ps f13, 544(x19)\n"
			"fminu.pi f8, f5, f13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 192(x19)\n"
			"fcvt.ps.pwu f5, f8, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_66690_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x15, 0x36000000063c000a\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x122490ae46b6d9eb\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x538df65b89f40d7a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"flw.ps f9, 640(x19)\n"
			"flw.ps f30, 768(x19)\n"
			"fmax.ps f13, f9, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 384(x22)\n"
			"frsq.ps f24, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 256(x19)\n"
			"flw.ps f0, 32(x19)\n"
			"flt.s x27, f8, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_14:\n"
			"fltm.ps m3, f19, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_14:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 352(x22)\n"
			"fexp.ps f7, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f3, 704(x22)\n"
			"fsrli.pi f3, f3, 0xb\n" 
			"maskpopc x11, m2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_14:\n"
			"maskand m3, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f31, 288(x19)\n"
			"flw.ps f28, 512(x19)\n"
			"fminu.pi f3, f31, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 224(x19)\n"
			"flw.ps f0, 544(x19)\n"
			"flw.ps f7, 768(x19)\n"
			"fcmov.ps f8, f5, f0, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 64(x16)\n"
			"flw.ps f16, 704(x16)\n"
			"fsgnjx.ps f30, f25, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f31, 992(x26)\n"
			"flw.ps f24, 736(x26)\n"
			"fle.pi f7, f31, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 768(x12)\n"
			"fsrai.pi f3, f8, 0xa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 928(x26)\n"
			"fround.ps f17, f9, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 288(x27)\n"
			"flw.ps f5, 800(x27)\n"
			"fsgnj.ps f31, f8, f5\n" 
			"li x15, 0x8f8d1cae9208730\n"
			"fcvt.s.wu f24, x15, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"li x5, 0x47c976bdd8ddecad\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x41a8265b1f2875d8\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 672(x12)\n"
			"flw.ps f30, 864(x12)\n"
			"feq.s x27, f9, f30\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_14:\n"
			"maskxor m4, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 608(x22)\n"
			"fcvt.pwu.ps f30, f30, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 224(x26)\n"
			"fclass.ps f28, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 384(x26)\n"
			"fslli.pi f13, f17, 0xc\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 448(x26)\n"
			"flw.ps f3, 800(x26)\n"
			"flw.ps f10, 192(x26)\n"
			"fnmsub.s f24, f16, f3, f10, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_14:\n"
			"flem.ps m2, f31, f0\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa7f73632c0\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 256(x22)\n"
			"flw.ps f5, 576(x22)\n"
			"flw.ps f8, 288(x22)\n"
			"fnmsub.s f8, f9, f5, f8, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 64(x23)\n"
			"flw.ps f16, 608(x23)\n"
			"fsgnj.s f19, f31, f16\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 32(x22)\n"
			"flw.ps f7, 736(x22)\n"
			"fmax.s f25, f0, f7\n" 
			"maskpopc x19, m6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 768(x27)\n"
			"fcvt.w.s x10, f0, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_14:\n"
			"fltm.ps m4, f7, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 960(x14)\n"
			"flw.ps f5, 928(x14)\n"
			"fmax.s f16, f19, f5\n" 
			"fbci.ps f3, 0x377ba\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_464758_HID_14:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_73472_HID_14:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_373970_HID_14:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x953fcff8de33591\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa5bd2630220eb7ec\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x4000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x18\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 480(x26)\n"
			"flw.ps f30, 928(x26)\n"
			"feq.s x16, f8, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 928(x14)\n"
			"flw.ps f7, 992(x14)\n"
			"frem.pi f8, f31, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 864(x22)\n"
			"flw.ps f25, 896(x22)\n"
			"fmul.pi f30, f10, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 800(x26)\n"
			"flw.ps f24, 576(x26)\n"
			"flw.ps f31, 864(x26)\n"
			"fmadd.ps f30, f7, f24, f31, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 608(x27)\n"
			"flw.ps f24, 672(x27)\n"
			"fltu.pi f28, f28, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 0(x23)\n"
			"flw.ps f31, 160(x23)\n"
			"fadd.ps f19, f3, f31, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 576(x23)\n"
			"flw.ps f24, 224(x23)\n"
			"fle.ps f25, f0, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 544(x16)\n"
			"flw.ps f31, 224(x16)\n"
			"flw.ps f5, 64(x16)\n"
			"fmsub.ps f10, f7, f31, f5, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 256(x12)\n"
			"flw.ps f3, 832(x12)\n"
			"fsgnjx.ps f9, f13, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 640(x22)\n"
			"fclass.s x16, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 544(x14)\n"
			"fclass.s x11, f16\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_14:\n"
			"maskand m4, m6, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x65a163a09c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 0(x10)\n"
			"fclass.s x27, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 512(x16)\n"
			"flw.ps f0, 672(x16)\n"
			"flt.pi f10, f25, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_int_32\n"
			"flw.ps f16, 992(x23)\n"
			"flw.ps f3, 576(x23)\n"
			"for.pi f19, f16, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 160(x11)\n"
			"flw.ps f17, 544(x11)\n"
			"fsub.pi f3, f8, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_156834_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"fbci.pi f24, 0x64cb9\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 512(x23)\n"
			"flw.ps f25, 352(x23)\n"
			"feq.ps f9, f3, f25\n" 
			"li x22, 0xaf54573452d5c511\n"
			"fcvt.s.wu f24, x22, rmm\n" 
			"fbci.ps f24, 0x7897a\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_14:\n"
			"flem.ps m4, f31, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f7, 0x59523\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 256(x10)\n"
			"flw.ps f30, 128(x10)\n"
			"fmax.ps f24, f10, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 160(x12)\n"
			"flw.ps f7, 224(x12)\n"
			"fsgnjn.ps f24, f5, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f16, 416(x14)\n"
			"flw.ps f9, 544(x14)\n"
			"fsub.pi f28, f16, f9\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 960(x23)\n"
			"flw.ps f10, 832(x23)\n"
			"fle.s x22, f31, f10\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f17, 416(x23)\n"
			"fcvt.ps.f16 f24, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 160(x16)\n"
			"flw.ps f3, 32(x16)\n"
			"fsub.pi f5, f10, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 352(x10)\n"
			"fle.s x23, f0, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 864(x12)\n"
			"flw.ps f9, 160(x12)\n"
			"fsub.s f10, f25, f9, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f19, 352(x11)\n"
			"flw.ps f28, 672(x11)\n"
			"fmulhu.pi f30, f19, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 160(x12)\n"
			"flw.ps f19, 608(x12)\n"
			"fsub.ps f10, f7, f19, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x27 << TENSOR_FMA16_SCP_LOC_B) | (0x7a << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 13\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
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
			"la x6, rand_int_32\n"
			"flw.ps f29, 288(x6)\n"
			"fcvt.ps.pwu f26, f29, rmm\n" 
			"maskpopcz x9, m6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 736(x17)\n"
			"fround.ps f24, f30, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 832(x9)\n"
			"flw.ps f20, 192(x9)\n"
			"fsgnjn.ps f10, f14, f20\n" 
			"fbci.ps f13, 0x1e8f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 224(x17)\n"
			"ffrc.ps f7, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 256(x9)\n"
			"flw.ps f19, 480(x9)\n"
			"fmulh.pi f18, f20, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 800(x19)\n"
			"frsq.ps f29, f23\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 448(x13)\n"
			"flw.ps f18, 192(x13)\n"
			"flw.ps f29, 320(x13)\n"
			"fcmov.ps f9, f23, f18, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskor m5, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f10, 256(x10)\n"
			"flw.ps f29, 288(x10)\n"
			"fmulh.pi f13, f10, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 480(x9)\n"
			"flw.ps f23, 96(x9)\n"
			"fmul.ps f10, f13, f23, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 704(x16)\n"
			"flw.ps f29, 864(x16)\n"
			"fmax.ps f13, f26, f29\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 672(x16)\n"
			"faddi.pi f7, f20, 0x197\n" 
			"la x19, rand_int_32\n"
			"flw.ps f26, 992(x19)\n"
			"flw.ps f24, 736(x19)\n"
			"fand.pi f1, f26, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 544(x6)\n"
			"fcvt.w.s x6, f13, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f29, 160(x16)\n"
			"fnot.pi f28, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 416(x22)\n"
			"flw.ps f24, 256(x22)\n"
			"flt.pi f28, f28, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 32(x10)\n"
			"flw.ps f14, 224(x10)\n"
			"feq.ps f23, f29, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 736(x6)\n"
			"fclass.s x22, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_15:\n"
			"maskor m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f9, 480(x13)\n"
			"fpackreph.pi f30, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 320(x13)\n"
			"fcvt.wu.s x17, f28, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 800(x20)\n"
			"flw.ps f19, 608(x20)\n"
			"fsgnjx.ps f18, f13, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 960(x13)\n"
			"flw.ps f1, 288(x13)\n"
			"feq.ps f13, f9, f1\n" 
			"fbci.pi f14, 0xe514\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 544(x20)\n"
			"flw.ps f10, 320(x20)\n"
			"feq.s x22, f13, f10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"fsetm.pi m3, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f1, 256(x13)\n"
			"flw.ps f18, 96(x13)\n"
			"fmin.pi f9, f1, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"maskor m3, m1, m3\n" 
			"li x5, 0x80340158\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 480(x20)\n"
			"ffrc.ps f10, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 704(x13)\n"
			"flw.ps f23, 160(x13)\n"
			"flt.ps f20, f29, f23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 608(x19)\n"
			"flw.ps f30, 160(x19)\n"
			"fsgnjx.s f26, f2, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f29, 32(x19)\n"
			"flw.ps f18, 480(x19)\n"
			"fand.pi f18, f29, f18\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"mov.m.x m6, x19, 0x5b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f23, 864(x16)\n"
			"flw.ps f13, 672(x16)\n"
			"fmaxu.pi f7, f23, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 608(x6)\n"
			"flw.ps f14, 192(x6)\n"
			"feq.pi f18, f7, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 576(x14)\n"
			"feq.ps f29, f1, f1\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 192(x22)\n"
			"flw.ps f24, 512(x22)\n"
			"fmax.pi f13, f19, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 832(x9)\n"
			"flw.ps f29, 992(x9)\n"
			"flw.ps f24, 384(x9)\n"
			"fnmsub.s f28, f1, f29, f24, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 192(x9)\n"
			"fcvt.w.s x19, f2, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 480(x6)\n"
			"flw.ps f7, 160(x6)\n"
			"fsll.pi f14, f30, f7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f19, 128(x25)\n"
			"fle.pi f26, f19, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 800(x22)\n"
			"fpackrepb.pi f13, f9\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 224(x14)\n"
			"flw.ps f29, 224(x14)\n"
			"fremu.pi f1, f14, f29\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 352(x19)\n"
			"fswizz.ps f19, f24, 0xe\n" 
			"maskpopcz x17, m3\n" 
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 160(x22)\n"
			"fcvt.pw.ps f24, f20, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 992(x6)\n"
			"flw.ps f7, 64(x6)\n"
			"fsub.s f30, f19, f7, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f2, 864(x22)\n"
			"fandi.pi f28, f2, 0x1b6\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_15:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 192(x14)\n"
			"frcp.ps f10, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 864(x25)\n"
			"flw.ps f1, 608(x25)\n"
			"fmax.pi f23, f2, f1\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 768(x13)\n"
			"fsat8.pi f7, f29\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 288(x10)\n"
			"flw.ps f9, 736(x10)\n"
			"flw.ps f18, 128(x10)\n"
			"fmadd.s f1, f23, f9, f18, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f19, 640(x16)\n"
			"fsatu8.pi f29, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 352(x13)\n"
			"flw.ps f1, 768(x13)\n"
			"flw.ps f29, 800(x13)\n"
			"fnmsub.ps f13, f14, f1, f29, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 448(x10)\n"
			"flw.ps f10, 352(x10)\n"
			"flw.ps f2, 64(x10)\n"
			"fmsub.ps f29, f30, f10, f2, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 384(x22)\n"
			"fsqrt.ps f26, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 32(x22)\n"
			"fslli.pi f24, f14, 0x0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 608(x20)\n"
			"flw.ps f20, 800(x20)\n"
			"fsub.s f26, f14, f20, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 96(x19)\n"
			"flw.ps f9, 704(x19)\n"
			"flw.ps f24, 864(x19)\n"
			"fmsub.s f1, f13, f9, f24, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_15:\n"
			"feqm.ps m5, f2, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f7, 608(x9)\n"
			"flw.ps f2, 256(x9)\n"
			"fmin.pi f29, f7, f2\n" 
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
			"la x13, rand_int_32\n"
			"flw.ps f26, 256(x13)\n"
			"flw.ps f9, 512(x13)\n"
			"fmul.pi f29, f26, f9\n" 
			"li x10, 0xec6d990b96c4b1a7\n"
			"fcvt.s.wu f10, x10, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 928(x25)\n"
			"flw.ps f13, 64(x25)\n"
			"fltu.pi f20, f29, f13\n" 
			"li x10, 0x7d85cf5d8a247070\n"
			"fcvt.s.w f14, x10, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 672(x19)\n"
			"fclass.ps f30, f7\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 192(x6)\n"
			"fandi.pi f2, f20, 0xd5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 288(x9)\n"
			"flw.ps f26, 416(x9)\n"
			"fsgnj.s f19, f1, f26\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 480(x10)\n"
			"flw.ps f24, 704(x10)\n"
			"fmax.ps f18, f1, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f20, 928(x20)\n"
			"fpackreph.pi f14, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 448(x20)\n"
			"flw.ps f1, 384(x20)\n"
			"fsgnjx.s f29, f14, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 576(x17)\n"
			"fclass.ps f2, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 832(x14)\n"
			"flw.ps f28, 448(x14)\n"
			"fsub.ps f14, f24, f28, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 96(x16)\n"
			"flw.ps f2, 160(x16)\n"
			"flw.ps f30, 192(x16)\n"
			"fcmov.ps f13, f19, f2, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 576(x6)\n"
			"faddi.pi f29, f7, 0xb\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 480(x14)\n"
			"flw.ps f13, 992(x14)\n"
			"fsub.ps f10, f2, f13, rne\n" 
			"fbci.pi f1, 0x6492f\n" 
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
			"LBL_SEQID_5_FP_TRANS_ID_8_HID_15:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 608(x10)\n"
			"fexp.ps f9, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 960(x6)\n"
			"fsrli.pi f28, f14, 0x9\n" 
			"maskpopc x20, m3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 704(x9)\n"
			"ffrc.ps f19, f14\n" 
			"fbci.ps f2, 0x3b380\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 640(x19)\n"
			"flw.ps f2, 576(x19)\n"
			"fsub.s f9, f20, f2, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 960(x9)\n"
			"flw.ps f2, 352(x9)\n"
			"fsgnjx.ps f28, f13, f2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 384(x19)\n"
			"flw.ps f1, 416(x19)\n"
			"fmul.pi f29, f28, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 576(x17)\n"
			"flw.ps f7, 160(x17)\n"
			"fsgnj.s f20, f13, f7\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 992(x9)\n"
			"flw.ps f29, 0(x9)\n"
			"fmulhu.pi f20, f9, f29\n" 
			"mova.x.m x16\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_15:\n"
			"flem.ps m6, f7, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 608(x9)\n"
			"flw.ps f14, 448(x9)\n"
			"fle.ps f20, f23, f14\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 768(x13)\n"
			"flw.ps f30, 832(x13)\n"
			"fremu.pi f28, f19, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 576(x19)\n"
			"fpackrepb.pi f14, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f19, 192(x16)\n"
			"flw.ps f10, 864(x16)\n"
			"fdivu.pi f14, f19, f10\n" 
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
		0xf414a140, 
		0xd682cfc9, 
		0x8e4a2dd7, 
		0x091805f1, 
		0xdf0f1ad5, 
		0xa6586bea, 
		0x3d5f081e, 
		0x7297a5a5, 
		0xd3132199, 
		0x52d9175b, 
		0x44cf6bf6, 
		0x3c569f20, 
		0x625715c6, 
		0x2905c025, 
		0x77d41430, 
		0xbcaf8d38, 
		0x94fdb937, 
		0xe6cd50dc, 
		0x1ce984e8, 
		0xb2ddba52, 
		0x53ed0273, 
		0xc2354ad0, 
		0x9fcf91f5, 
		0xc940f6ea, 
		0xfa355b9d, 
		0x1c282a3b, 
		0x9f6ce651, 
		0x1c8aaf5d, 
		0x010a08d4, 
		0x5b17010c, 
		0xad58d4ce, 
		0x68dc6594, 
		0xd8458e33, 
		0x82639b00, 
		0x8a0f61a7, 
		0xc9fd08e5, 
		0x44836349, 
		0xa2a3cb9d, 
		0xcd19bbca, 
		0xf5d6355f, 
		0xe60edc3a, 
		0xde043664, 
		0xafcc73dd, 
		0x21812c5d, 
		0x22d6a4e0, 
		0xf58620d1, 
		0x216f1719, 
		0x1b2f84cf, 
		0x6344a871, 
		0x06a6a7f2, 
		0xf2b000c2, 
		0xdd166293, 
		0xc4dbf0d7, 
		0x5799ea01, 
		0xf37101c8, 
		0x81af152c, 
		0xac6591d6, 
		0x0a9d2ca4, 
		0xb0e81442, 
		0x882e853e, 
		0x4d1e8397, 
		0x3374e007, 
		0x068883b2, 
		0xa3ecb51c, 
		0x4e8f1c7d, 
		0x061b87e9, 
		0x95a6fd35, 
		0x9f97cd9c, 
		0x7902eb94, 
		0xdfe7d623, 
		0xffd7a240, 
		0x74bc9878, 
		0xc28421b2, 
		0xc61d3d61, 
		0xbea4abed, 
		0x915591de, 
		0x62c7656d, 
		0x79b3378a, 
		0xb373a14a, 
		0x1470f82f, 
		0xda3dd21e, 
		0xc162ee2e, 
		0xd1cd3dac, 
		0x8f39c055, 
		0xb683fd85, 
		0x71eb2cbc, 
		0x07252cb2, 
		0xa70fc8c2, 
		0x39d78664, 
		0x1435d629, 
		0x31b9cf57, 
		0x197f32e4, 
		0x49a9d263, 
		0x41be2891, 
		0x7df21e60, 
		0x6c97197b, 
		0xabc71da2, 
		0xb276e6de, 
		0x154f2ac8, 
		0xe17b55c8, 
		0xe8a8d8f8, 
		0xe86cb7af, 
		0x4bb16427, 
		0xb11e4952, 
		0x755da8db, 
		0xe45b699f, 
		0x43b0a342, 
		0x1ea97152, 
		0x531e7885, 
		0x9e168bf6, 
		0x79bba5a7, 
		0xfd95e948, 
		0x9e27fa14, 
		0xcbbb84e2, 
		0x5b568485, 
		0x5c9593d2, 
		0x2dec209d, 
		0xa5e17a86, 
		0x986537c6, 
		0x9dc92ef3, 
		0xac58612b, 
		0x76812e03, 
		0xbdc3225a, 
		0xa560ebc1, 
		0x4fd61cc0, 
		0x9b87795b, 
		0x751cd619, 
		0x8d91dc2b, 
		0xc3e4d314, 
		0x9586204d, 
		0xb2eeb83e, 
		0xc57f47fb, 
		0x1c99d32a, 
		0x27335e57, 
		0xa7d8dd9a, 
		0x58944917, 
		0xfce4defe, 
		0x1fbb8b22, 
		0xcde71f27, 
		0x85befbd3, 
		0x5ceb97f2, 
		0x6ebeff23, 
		0x4ccdd6f0, 
		0x14ab5331, 
		0x84e3edaf, 
		0xf3b8b0da, 
		0x92a872df, 
		0xd80bd4ea, 
		0x1e160f70, 
		0x054a22cb, 
		0xbf3c17a5, 
		0x01aaad70, 
		0x2274dd92, 
		0x242069d9, 
		0x7aa52ba5, 
		0x804e4854, 
		0x7043bc51, 
		0xb4fd983d, 
		0xc5aa68ed, 
		0x4bdcff5c, 
		0x947f9ee9, 
		0x299bb783, 
		0xe4eb67d1, 
		0xf0fc2d5f, 
		0x50ca82f4, 
		0xcee839e8, 
		0x97a1c412, 
		0xd184b7ea, 
		0xad860860, 
		0x25efb4a9, 
		0x524d8860, 
		0x0c74c88a, 
		0x0f52ad14, 
		0xa3d605ba, 
		0xd3f7f388, 
		0xae9da92e, 
		0x8bfe2d5a, 
		0x79683be0, 
		0xfad11138, 
		0x1e8b08af, 
		0x607ff845, 
		0xd79421ac, 
		0xbc83275b, 
		0xa3010b83, 
		0x7c635a88, 
		0xe1bb7a7c, 
		0xd027244f, 
		0x076cd19e, 
		0x016f0532, 
		0xa5c83b20, 
		0xa34056b8, 
		0xffe44822, 
		0xaed7b3f6, 
		0xfb137938, 
		0x6477d2b2, 
		0x0bf9d1c0, 
		0x19379a21, 
		0x5647ad69, 
		0x16c9a315, 
		0xa326ad3d, 
		0x7c62ebb8, 
		0xe1f9274a, 
		0x673b81b9, 
		0x648a1571, 
		0x4d0b2f59, 
		0x6cbe06a9, 
		0x96b9f01b, 
		0x04089c5b, 
		0xbc67e896, 
		0xf5b6f549, 
		0x4469a8ad, 
		0x45a1058f, 
		0x7f94c151, 
		0xae551692, 
		0xb8abd468, 
		0xb3e42791, 
		0xfb581ab5, 
		0xfbd79a6b, 
		0x30f1da4e, 
		0x4688bf03, 
		0xdd043f91, 
		0x9535b875, 
		0x7be0db85, 
		0xdfdce318, 
		0x1a2c8e27, 
		0x21e2df23, 
		0xc58e9ac0, 
		0xede094cc, 
		0xf86f681b, 
		0xb70aa0d4, 
		0xe7fde280, 
		0x7e12749d, 
		0xda91c179, 
		0x9e1e0923, 
		0x7118622a, 
		0x47f432c7, 
		0x2282a4f1, 
		0xa1082782, 
		0x0446b8fe, 
		0x87114f53, 
		0xb356053a, 
		0x90ccc6f9, 
		0xa4c31339, 
		0x1044b6c4, 
		0xe1cbb084, 
		0xfd13a8a0, 
		0x5a4d6510, 
		0x2fd3394f, 
		0x80f1db02, 
		0xd8cf494d, 
		0xb58f9210, 
		0xc7ab8aa7, 
		0xa2672dc1, 
		0xe1ce0a05, 
		0xe45cacba, 
		0x2cd390e4  
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
		0xc500, 
		0xd3c0, 
		0x5180, 
		0xc400, 
		0xcc80, 
		0xc700, 
		0xfd6f, 
		0x8000, 
		0x5040, 
		0x52c0, 
		0x0000, 
		0xd360, 
		0x21ec, 
		0xcb00, 
		0xd140, 
		0x06fa, 
		0x52a0, 
		0x4c80, 
		0xd2c0, 
		0x51c0, 
		0xccc0, 
		0xe250, 
		0x90b1, 
		0xd260, 
		0x4700, 
		0xd120, 
		0x4880, 
		0xcd40, 
		0xfb25, 
		0x6b1e, 
		0x88e0, 
		0x1159, 
		0x4b80, 
		0x8b7e, 
		0x5060, 
		0x8000, 
		0x5060, 
		0xd160, 
		0x755a, 
		0x4e80, 
		0x4353, 
		0xcf80, 
		0x9b80, 
		0x4e40, 
		0xc900, 
		0x4b00, 
		0xd040, 
		0x4b00, 
		0xcf80, 
		0x8000, 
		0x4600, 
		0xd0a0, 
		0x5080, 
		0xc854, 
		0x51e0, 
		0x3cfc, 
		0x4fc0, 
		0x52a0, 
		0x5360, 
		0x52a0, 
		0xd120, 
		0x8000, 
		0x4900, 
		0x8000, 
		0x4f80, 
		0xc600, 
		0x5100, 
		0xcb80, 
		0xc500, 
		0xd160, 
		0x300f, 
		0xd160, 
		0x50e0, 
		0xd0a0, 
		0x5020, 
		0x52c0, 
		0xb792, 
		0x53c0, 
		0xc500, 
		0x4d00, 
		0x4d40, 
		0x7c00, 
		0xa1ab, 
		0x37b8, 
		0xc980, 
		0x20d0, 
		0x4b80, 
		0x5100, 
		0x5160, 
		0x0000, 
		0x8000, 
		0x4b00, 
		0x53a0, 
		0xbf28, 
		0x0000, 
		0xfdae, 
		0xc000, 
		0x2fe3, 
		0x5260, 
		0xbd6d, 
		0x674c, 
		0x7203, 
		0x4600, 
		0x5100, 
		0x5260, 
		0x4400, 
		0x4e40, 
		0x52e0, 
		0x8000, 
		0x8000, 
		0x4c00, 
		0x1828, 
		0x4900, 
		0xc200, 
		0xd2a0, 
		0xd320, 
		0x52e0, 
		0x5240, 
		0x5100, 
		0x4e80, 
		0xd3c0, 
		0xc33e, 
		0x13f2, 
		0x0000, 
		0xf758, 
		0xd3eb, 
		0xd2c0, 
		0xcf80, 
		0x1222, 
		0x18eb, 
		0x0000, 
		0x5000, 
		0xce40, 
		0x8000, 
		0xd180, 
		0x4c80, 
		0xc700, 
		0x8827, 
		0x4d00, 
		0xc800, 
		0xc200, 
		0x3c00, 
		0xca80, 
		0x52a0, 
		0x2ac7, 
		0xd3a0, 
		0x9a6b, 
		0x5300, 
		0x4ec0, 
		0xeaac, 
		0x22cd, 
		0x5380, 
		0x50e0, 
		0xd260, 
		0xf84e, 
		0x9664, 
		0xd0e0, 
		0xf155, 
		0xcdc0, 
		0x0000, 
		0xd040, 
		0xd0c0, 
		0x4500, 
		0x50a0, 
		0xce40, 
		0xd3a0, 
		0xfcaa, 
		0x53a0, 
		0xd040, 
		0xa99a, 
		0xfdd4, 
		0x4b00, 
		0xd0c0, 
		0xce40, 
		0xd0c0, 
		0xcf40, 
		0x5200, 
		0x51a0, 
		0xd020, 
		0x4980, 
		0x5040, 
		0x8000, 
		0xcf80, 
		0x8731, 
		0xcdc0, 
		0x7294, 
		0xce32, 
		0x4e40, 
		0x1318, 
		0xd200, 
		0xca00, 
		0x51a0, 
		0xd080, 
		0x825a, 
		0xc200, 
		0x4400, 
		0x4d40, 
		0x5260, 
		0x53c0, 
		0xd180, 
		0xd340, 
		0x3ac2, 
		0x0000, 
		0xd043, 
		0xf84a, 
		0x23e4, 
		0x0000, 
		0x5728, 
		0x37db, 
		0x4cc0, 
		0xd280, 
		0xd020, 
		0xd1c0, 
		0x4e40, 
		0x4200, 
		0x5180, 
		0x1153, 
		0xd2a0, 
		0xd360, 
		0x5300, 
		0xffaa, 
		0xd1a0, 
		0x4c00, 
		0xffde, 
		0xd618, 
		0x7c00, 
		0xd160, 
		0xdb82, 
		0xd140, 
		0x4800, 
		0xd78e, 
		0xd160, 
		0x4d00, 
		0xcdc0, 
		0x5240, 
		0x5040, 
		0x7c2a, 
		0xcf00, 
		0xc880, 
		0x83a1, 
		0x5360, 
		0x4f40, 
		0x7c00, 
		0xfc00, 
		0xce40, 
		0x5160, 
		0xd260, 
		0x5080, 
		0x1178, 
		0x2ef4, 
		0xf1a4, 
		0xe29a, 
		0x8000, 
		0xd280, 
		0x4a80, 
		0xfc00, 
		0x5360, 
		0xd060, 
		0xe702, 
		0x5020, 
		0x11d6, 
		0x4000, 
		0xd340, 
		0x4dc0, 
		0x53c0, 
		0xd1c0, 
		0xfc00, 
		0xc880, 
		0x4c80, 
		0xd180, 
		0xd140, 
		0x4e00, 
		0xcbcb, 
		0xca00, 
		0x4fc0, 
		0x5040, 
		0x0048, 
		0xd220, 
		0x4f80, 
		0xd411, 
		0x302c, 
		0x5000, 
		0xd380, 
		0x8a1b, 
		0x4f80, 
		0x9f40, 
		0x5280, 
		0xc880, 
		0x6d47, 
		0xd160, 
		0x7534, 
		0xcc40, 
		0x7c00, 
		0x7474, 
		0xd1a0, 
		0x5060, 
		0x4f40, 
		0xcf00, 
		0xd040, 
		0x5200, 
		0x9f51, 
		0x7c00, 
		0x05c2, 
		0x8cca, 
		0x7dd7, 
		0x170e, 
		0xd3c0, 
		0xc48e, 
		0xd160, 
		0xc74f, 
		0xc980, 
		0x4880, 
		0x8000, 
		0xcfc0, 
		0x9182, 
		0x4000, 
		0x4cc0, 
		0xc700, 
		0xc52e, 
		0xafea, 
		0xd3a0, 
		0x52a0, 
		0xd060, 
		0x4c80, 
		0x49a6, 
		0x52a0, 
		0x5280, 
		0xd1e0, 
		0x77dd, 
		0xc200, 
		0xd149, 
		0xcd00, 
		0xb086, 
		0xa562, 
		0xc486, 
		0xce00, 
		0x5160, 
		0xcf40, 
		0xc000, 
		0x7c9e, 
		0x044b, 
		0xba8a, 
		0x5060, 
		0x5000, 
		0xd340, 
		0xa878, 
		0xcec0, 
		0xa610, 
		0x4db1, 
		0x4cc0, 
		0xd240, 
		0xccc0, 
		0xfb76, 
		0xc800, 
		0x4900, 
		0xd638, 
		0x50e0, 
		0xab87, 
		0xc200, 
		0xd040, 
		0xd0c0, 
		0x3d66, 
		0x5380, 
		0xca80, 
		0x8000, 
		0x6c7c, 
		0xd260, 
		0xd2c0, 
		0x4b00, 
		0xcd00, 
		0xc200, 
		0xd0a0, 
		0x50fb, 
		0x53c0, 
		0x04ee, 
		0xd1a0, 
		0x2175, 
		0x6c7c, 
		0x1392, 
		0x4d80, 
		0x247c, 
		0x50c0, 
		0xc200, 
		0x52a0, 
		0xcdc0, 
		0x4f40, 
		0x4800, 
		0xd1a0, 
		0x5300, 
		0x4907, 
		0x5220, 
		0x5140, 
		0xfc00, 
		0xfc00, 
		0xc980, 
		0x93ec, 
		0xcdc0, 
		0xe146, 
		0x4dc0, 
		0xde03, 
		0xedd5, 
		0x164e, 
		0x8000, 
		0x4d80, 
		0x9fe6, 
		0xbc1b, 
		0x5380, 
		0xcf00, 
		0x53c0, 
		0xcf80, 
		0xfc00, 
		0xd220, 
		0x7345, 
		0x5060, 
		0xfc00, 
		0xfc00, 
		0xc800, 
		0x5320, 
		0x3216, 
		0x8000, 
		0x899b, 
		0x52c0, 
		0x451e, 
		0xcc40, 
		0x53c0, 
		0x595c, 
		0xcb80, 
		0xd3a0, 
		0xce40, 
		0x4fc0, 
		0x51e0, 
		0xd294, 
		0xcd40, 
		0xd0c0, 
		0x5120, 
		0x3c00, 
		0xb4e9, 
		0x5000, 
		0x1f88, 
		0x4c40, 
		0xcf00, 
		0x3636, 
		0x8000, 
		0xd0c0, 
		0x50c0, 
		0x5080, 
		0x7c00, 
		0x0e7c, 
		0xd2c0, 
		0xcf80, 
		0x5140, 
		0x4baf, 
		0x4e00, 
		0x4200, 
		0x4ec0, 
		0x8000, 
		0x6019, 
		0x7c00, 
		0xfc00, 
		0x4d00, 
		0x4d40, 
		0xccc0, 
		0x033e, 
		0xd120, 
		0x0000, 
		0xd260, 
		0x52c0, 
		0x4cc0, 
		0xe411, 
		0x076f, 
		0x7b02, 
		0x4f81, 
		0x8000, 
		0x50a0, 
		0x4499, 
		0xb100, 
		0xcf80, 
		0xd2e0, 
		0xe47a, 
		0xde37, 
		0x5240, 
		0xb64f, 
		0xcc40, 
		0x944b, 
		0xd1c0, 
		0xd0a0, 
		0x8000, 
		0xfda4, 
		0x80d0, 
		0x1c3f, 
		0x6653, 
		0xe85b, 
		0xc9b1, 
		0xb2d6, 
		0xc800, 
		0x0000, 
		0xd2c0, 
		0x8000, 
		0xb79a, 
		0x0b0a, 
		0x50e0, 
		0x742d, 
		0x4f00, 
		0x4e80, 
		0x53e0, 
		0xc900, 
		0xd2c0, 
		0xba07, 
		0xc77c, 
		0x4c40, 
		0xcd5e, 
		0x4700, 
		0x4e41, 
		0x7c00, 
		0x3c00, 
		0x5160, 
		0x4500, 
		0x49ee, 
		0xd080, 
		0xfd8e, 
		0xb341, 
		0x4e80, 
		0xccc0, 
		0x4a80, 
		0x4ec1, 
		0x5bc8, 
		0x4f40, 
		0x52e0, 
		0xefcd, 
		0x5340, 
		0xcf80, 
		0x4980, 
		0x2071, 
		0x51e0, 
		0x6863, 
		0x88fe, 
		0x1d89, 
		0x4e80, 
		0xd0e0, 
		0x4e80, 
		0x3489, 
		0x4ec0, 
		0xc200, 
		0x52c0, 
		0x75fc, 
		0xd3a0, 
		0x4c00, 
		0xd1c0, 
		0x5160, 
		0x7bb7, 
		0xd340, 
		0xca80, 
		0xcc40, 
		0xb9e2, 
		0x5300, 
		0xd040, 
		0xfc00, 
		0xd280, 
		0x4980, 
		0x4000, 
		0x4a80, 
		0xcf00, 
		0xfadb, 
		0x51c0, 
		0x47e7, 
		0xd3b1, 
		0x5380, 
		0x4400, 
		0xd000, 
		0x8000, 
		0x3fc9, 
		0x5000, 
		0x5140, 
		0xddd1, 
		0x7c00, 
		0x4d40, 
		0x0000, 
		0xd360, 
		0x53c0, 
		0x52c0, 
		0x3c00, 
		0xd040, 
		0xcb80, 
		0x4dc0, 
		0x52e0, 
		0xcd80, 
		0x411b, 
		0xc000, 
		0xcdc0, 
		0xc245, 
		0xd0c0, 
		0xd160, 
		0x108b, 
		0xecf8, 
		0x4d40, 
		0x1dd4, 
		0x4800, 
		0xce00, 
		0x0000, 
		0x69b6, 
		0xcbf9, 
		0xfc00, 
		0xd3c0, 
		0x4e80, 
		0xcb00, 
		0xe015, 
		0xcfc0, 
		0x8000, 
		0xd240, 
		0xce40, 
		0x51e0, 
		0x0d23, 
		0x272b, 
		0xd360, 
		0xe9fa, 
		0xcb80, 
		0xe1ee, 
		0xfa00, 
		0xfc00, 
		0x5360, 
		0x5632, 
		0xd260, 
		0xc6ef, 
		0x4e00, 
		0x5260, 
		0x7c23, 
		0x4600, 
		0x0957, 
		0xfc00, 
		0x03c0, 
		0xd160, 
		0xa24a, 
		0xd220, 
		0x75c9, 
		0xcc00, 
		0x5380, 
		0x0000, 
		0x4600, 
		0x1a7b, 
		0x5866, 
		0xd060, 
		0x0ff7, 
		0xb0cb, 
		0xc109, 
		0x50a0, 
		0xd360, 
		0xca80, 
		0x4500, 
		0xd1c0, 
		0xd040, 
		0xfc00, 
		0x4f00, 
		0x7c00, 
		0x1047, 
		0x4980, 
		0xfc00, 
		0x53a0, 
		0xd120, 
		0xd240, 
		0xaa23, 
		0xc500, 
		0xd100, 
		0xd240, 
		0x8000, 
		0x8000, 
		0x8000, 
		0x40d9, 
		0x4520, 
		0x858c, 
		0xc366, 
		0x9b36, 
		0x5200, 
		0x4500, 
		0xdca8, 
		0xd2c0, 
		0x8000, 
		0xcf00, 
		0x8000, 
		0xfd4f, 
		0xcc80, 
		0x4d40, 
		0x7ccf, 
		0x9d4c, 
		0x52c0, 
		0x5120, 
		0xce40, 
		0xce80, 
		0x4800, 
		0xd100, 
		0x5160, 
		0xc980, 
		0xcc80, 
		0xcc00, 
		0xd0a0, 
		0xd140, 
		0xcf40, 
		0xc475, 
		0xf046, 
		0xd2e0, 
		0x5320, 
		0x5a0e, 
		0xe94e, 
		0x4fc0, 
		0x9bb1, 
		0xcf40, 
		0xfc00, 
		0x7c00, 
		0xcf00, 
		0xc880, 
		0x5120, 
		0xde96, 
		0x4200, 
		0x4400, 
		0xcdc0, 
		0xd200, 
		0x5120, 
		0x905d, 
		0x2d91, 
		0x0511, 
		0x4e00, 
		0x4dc0, 
		0xcb80, 
		0xc200, 
		0x8000, 
		0x4a00, 
		0xd23c, 
		0xaa63, 
		0x086e, 
		0xd000, 
		0x5aa4, 
		0x8000, 
		0xc57a, 
		0xd16e, 
		0x43d6, 
		0x51e0, 
		0x863e, 
		0x0000, 
		0xbbb5, 
		0xd2e0, 
		0x58ff, 
		0x97c9, 
		0x4600, 
		0x0b2f, 
		0xd3c0, 
		0xf53f, 
		0x5340, 
		0x7104, 
		0xfc00, 
		0xd000, 
		0x51a0, 
		0xcec0, 
		0x5220, 
		0x8000, 
		0xd3e0, 
		0x81fe, 
		0x4dc0, 
		0xcf80, 
		0x3c00, 
		0x59ef, 
		0x4d00, 
		0x5cb2, 
		0x4d40, 
		0x5180, 
		0x89a6, 
		0x6d36, 
		0xfc00, 
		0x4500, 
		0x4d80, 
		0xbc00, 
		0x3c00, 
		0x2c85, 
		0x0505, 
		0x4c40, 
		0x4e80, 
		0x0dd2, 
		0x5280, 
		0x8000, 
		0x0501, 
		0x5100, 
		0x4880, 
		0xf1ae, 
		0xe677, 
		0xb6ae, 
		0x8000, 
		0x5260, 
		0x52c0, 
		0xcf40, 
		0x4a80, 
		0x5020, 
		0x4e40, 
		0x7e8f, 
		0xd2e0, 
		0x53c0, 
		0xd360, 
		0x5220, 
		0xd040, 
		0xd260, 
		0x8000, 
		0xbd5a, 
		0x5180, 
		0x297b, 
		0x0000, 
		0x17be, 
		0xfd1a, 
		0xfc00, 
		0x7c00, 
		0xfc00, 
		0x51a0, 
		0x4c00, 
		0x4dc0, 
		0xfd17, 
		0x5280, 
		0x1151, 
		0x5120, 
		0xd080, 
		0xc800, 
		0x4880, 
		0xd0a0, 
		0x5040, 
		0x5380, 
		0xd3c0, 
		0x0000, 
		0xd3e0, 
		0x1e2c, 
		0x4a7b, 
		0x4980, 
		0xd140, 
		0x5140, 
		0x4e80, 
		0xcfc0, 
		0x8000, 
		0x0000, 
		0xd080, 
		0x4f80, 
		0xcb80, 
		0x50c0, 
		0xc600, 
		0x4c40, 
		0x0000, 
		0xce40, 
		0x4cc0, 
		0x4ec0, 
		0xcf80, 
		0x4258, 
		0x4e00, 
		0x8881, 
		0xcc40, 
		0xcd00, 
		0x4023, 
		0xcfc0, 
		0xfc00, 
		0x4e80, 
		0xadf6, 
		0xfc00, 
		0x4000, 
		0xbe59, 
		0xcec0, 
		0x4a80, 
		0x4d00, 
		0x0000, 
		0x4f00, 
		0x5280, 
		0xccc3, 
		0xeda5, 
		0x5260, 
		0xd200, 
		0xc600, 
		0x6f56, 
		0xdc71, 
		0xfc00, 
		0x53e0, 
		0x4e40, 
		0xb459, 
		0xd200, 
		0x4e00, 
		0xd2a0, 
		0x5140, 
		0xcf80, 
		0xd100, 
		0x6b6d, 
		0x53e0, 
		0xc600, 
		0x9432, 
		0xe2ae, 
		0xb4c2, 
		0x53e0, 
		0x51af, 
		0x3182, 
		0xaf9b, 
		0x4fc0, 
		0x4d40, 
		0x3901, 
		0x80d5, 
		0xd300, 
		0x5220, 
		0x4500, 
		0x4000, 
		0xd220, 
		0x4d80, 
		0xf3a0, 
		0xcdc0, 
		0x4a00, 
		0x4d40, 
		0x41a5, 
		0x0000, 
		0x8000, 
		0xe5ea, 
		0x258e, 
		0x7c2c, 
		0x4c40, 
		0x51c0, 
		0xcfc0, 
		0x5180, 
		0x5220, 
		0x24eb, 
		0x70c9, 
		0xccc0, 
		0x53e0, 
		0x49e4, 
		0xd2e0, 
		0xc900, 
		0xccc0, 
		0x4e40, 
		0xe677, 
		0xd040, 
		0x4b00, 
		0x2c54, 
		0x5100, 
		0xd380, 
		0xd2a0, 
		0xd2a0, 
		0x8000, 
		0x4d80, 
		0x7ce1, 
		0xe7a9, 
		0x52e0, 
		0xd180, 
		0x3150, 
		0x4ec0, 
		0x52c0, 
		0x50a0, 
		0x5300, 
		0x6d28, 
		0x51e0, 
		0x0000, 
		0xcb5d, 
		0x4c00, 
		0xd180, 
		0xcb80, 
		0xd040, 
		0x4f40, 
		0x91e7, 
		0xd1a0, 
		0x5260, 
		0xf642, 
		0x4d80, 
		0xcf00, 
		0xc500, 
		0x2f32, 
		0x612e, 
		0xd280, 
		0x5140, 
		0xb8e8, 
		0xf109, 
		0x0000, 
		0x7b2f, 
		0xd0c0, 
		0xfdfb, 
		0x4e40, 
		0x52c0, 
		0xd0e0, 
		0x52a0, 
		0xcd40, 
		0x8000, 
		0x53c0, 
		0x15b2, 
		0x5f58, 
		0x4f80, 
		0x5120, 
		0xcd00, 
		0x52c0, 
		0xa1fb, 
		0x0000, 
		0x4a80, 
		0x4f80, 
		0x4f80, 
		0x836b, 
		0x2d9c, 
		0x0000, 
		0x0000, 
		0x5220, 
		0x4e80, 
		0xfc00, 
		0x4d80, 
		0xc900, 
		0x4980, 
		0x4fc0, 
		0x4200, 
		0x50a0, 
		0x0000, 
		0x2250, 
		0xc980, 
		0x5360, 
		0xfc00, 
		0xcc00, 
		0xce40, 
		0xc980, 
		0x0d53, 
		0x4880, 
		0xa75e, 
		0xd040, 
		0x50a0, 
		0x0000, 
		0x4000, 
		0xe847, 
		0xcd80, 
		0xcdc0, 
		0x52c0, 
		0x7d73, 
		0x0000, 
		0xc200, 
		0x8c61, 
		0xfc00, 
		0xd060, 
		0x534c, 
		0xd6b6, 
		0xd320, 
		0x4700, 
		0x9a82, 
		0x51e0, 
		0x4f80, 
		0x11e8, 
		0x6b60, 
		0xfc00, 
		0xc800, 
		0xc400, 
		0x51e0, 
		0x8000, 
		0xcfc0, 
		0xd120, 
		0x34fc, 
		0x0545, 
		0x4b00, 
		0xd1c0, 
		0xdec4, 
		0x63b7, 
		0x4a80, 
		0x4600, 
		0x4c40, 
		0x4400, 
		0xa83f, 
		0x86a4, 
		0x2d6f, 
		0x5120, 
		0xbc00, 
		0x8000, 
		0x53a0, 
		0x6cd2, 
		0xd1e0, 
		0xbcd2, 
		0x8000, 
		0xd220, 
		0x562c, 
		0xa2f9, 
		0xd0e0, 
		0xc400, 
		0x8037, 
		0xc880, 
		0xd220, 
		0xd100, 
		0x4f80, 
		0x0cf8, 
		0xcc00, 
		0xd649, 
		0x0109, 
		0x4c40, 
		0xd120, 
		0x5160, 
		0x8000, 
		0x9fed, 
		0xd320, 
		0xc000, 
		0xd140, 
		0x6da3, 
		0xd1e0, 
		0x20cd, 
		0x4d40, 
		0x4f40, 
		0x0fda, 
		0xce40, 
		0x0a77, 
		0x4d40, 
		0x4dc0, 
		0xc200, 
		0x2e0a, 
		0x3405, 
		0xe43f, 
		0x51e0, 
		0x4a80, 
		0x5300, 
		0xd280, 
		0x4a00, 
		0x4a80, 
		0x58c4, 
		0xc900, 
		0xd380, 
		0x8000, 
		0x8000, 
		0x4b80, 
		0x9fe1, 
		0x3fbd, 
		0x28c9, 
		0xa55f, 
		0xe769, 
		0x5120, 
		0xa982, 
		0xd412, 
		0x5140, 
		0x8000, 
		0x4abf, 
		0xd2c0, 
		0xb7c2, 
		0x5060, 
		0x7c00, 
		0x25c8, 
		0xec29, 
		0xf555, 
		0xd040, 
		0x62fb, 
		0x2aac, 
		0xccc0, 
		0xd340, 
		0x036c, 
		0xd2e0, 
		0x0d3b, 
		0x4200, 
		0x3c00, 
		0xa756, 
		0xd3a0, 
		0x5240, 
		0x8000, 
		0x5240, 
		0xfcd1, 
		0x53e0, 
		0x9262, 
		0x7c00, 
		0x5180, 
		0x66ac, 
		0x8000, 
		0xfc00, 
		0xd280, 
		0xcc80, 
		0x7ebc, 
		0x5cc6, 
		0x52c0, 
		0x581d, 
		0xbc00, 
		0xfc83, 
		0x4700, 
		0x5000, 
		0xb0d8, 
		0xfc00, 
		0xe9b2, 
		0x4f95, 
		0x4f40, 
		0x4d40, 
		0x4d00, 
		0xcd7b, 
		0x4880, 
		0x4500, 
		0xb2f7, 
		0xfd5f, 
		0x53c0, 
		0xcc80, 
		0xc980, 
		0x4c40, 
		0x500b, 
		0xd180, 
		0xce40, 
		0x4f40, 
		0x5260, 
		0x5040, 
		0x0d16, 
		0x7cb6, 
		0xcf80, 
		0xfc00, 
		0x5000, 
		0xcd80, 
		0x5240, 
		0xf8a9, 
		0xc200, 
		0xcc80, 
		0x5120, 
		0x5200, 
		0xc600, 
		0x5100, 
		0x5200, 
		0x3c00, 
		0x4500, 
		0x5020, 
		0x4000, 
		0x5300, 
		0xd100, 
		0x4f80, 
		0x4a80, 
		0xce40, 
		0xd380, 
		0x78c3, 
		0xc000, 
		0x8a14, 
		0xd1e0, 
		0x0000, 
		0xd120, 
		0x6ee2, 
		0xd2e0, 
		0xb2ac, 
		0xa1b2, 
		0x4d40, 
		0xde4f, 
		0x0e7d, 
		0xd2a0, 
		0x7d71, 
		0x0000, 
		0x8000, 
		0x4980, 
		0x4400, 
		0x1673, 
		0x4e00, 
		0x5280, 
		0x52e0, 
		0x5000, 
		0xcd40, 
		0x4c40, 
		0x4e00, 
		0xe0cd, 
		0x3519, 
		0x8000, 
		0xca00, 
		0x51c0, 
		0x9b20, 
		0x4d40, 
		0x8000, 
		0x5080, 
		0x744a, 
		0x5060, 
		0x52a0, 
		0xac1f, 
		0xd160, 
		0x89aa, 
		0xb8d0, 
		0xfc00, 
		0xd060, 
		0xca4c, 
		0x5320, 
		0xca00, 
		0x51a0, 
		0xd260, 
		0xc000, 
		0xd1c0, 
		0xfc00, 
		0x4cc0, 
		0x4400, 
		0x359c, 
		0xd160, 
		0x4fc0, 
		0xc247, 
		0xb009, 
		0x5000, 
		0x6c2e, 
		0x4f00, 
		0x4900, 
		0x53a0, 
		0x4800, 
		0x7d83, 
		0xd2e0, 
		0xe2d9, 
		0x3e91, 
		0x4700, 
		0x4fc0, 
		0x5160, 
		0xcd80, 
		0x5080, 
		0xb1b3, 
		0x903a, 
		0x5240, 
		0xd240, 
		0xc200, 
		0x4acc, 
		0x814d, 
		0x4e00, 
		0xc980, 
		0xd020, 
		0xc000, 
		0xe692, 
		0x4f40, 
		0x4200, 
		0x1821, 
		0xed1a, 
		0x8000, 
		0x8000, 
		0x8e51, 
		0x8005, 
		0x9f3c, 
		0x4dc0, 
		0x5300, 
		0xd0c0, 
		0x7c01, 
		0xd000, 
		0x5100, 
		0x5140, 
		0xd220, 
		0x5160, 
		0x93d3, 
		0x5060, 
		0xb4c3, 
		0x08c8, 
		0xfc00, 
		0x3328, 
		0xd3a0, 
		0xc880, 
		0xd360, 
		0xd360, 
		0x4e80, 
		0x1e10, 
		0x4c48, 
		0x5380, 
		0x2234, 
		0x4ec0, 
		0x4d80, 
		0x8000, 
		0xd1c0, 
		0x4d00, 
		0x1290, 
		0xd160, 
		0x50a0, 
		0x5220, 
		0xd1a0, 
		0xfdd8, 
		0x0000, 
		0xd340, 
		0xd180, 
		0x3c00, 
		0xd160, 
		0x7376, 
		0x0000, 
		0xd3a0, 
		0xce00, 
		0xce00, 
		0x8ab5, 
		0xd3e0, 
		0xe58f, 
		0x4420, 
		0x4ec0, 
		0xfc00, 
		0xc200, 
		0xa49d, 
		0x4e80, 
		0x5340, 
		0x7c00, 
		0x5240, 
		0x50e0, 
		0x4400, 
		0xd3e0, 
		0x50a0, 
		0xcfc0, 
		0xccc0, 
		0xd020, 
		0xc900, 
		0xd2e0, 
		0xce00, 
		0xaa37, 
		0x4f40, 
		0xd220, 
		0xc400, 
		0xcd00, 
		0x7cd7, 
		0x3c00, 
		0xcd40, 
		0x51c0, 
		0xd240, 
		0xd7f8, 
		0xd3a0, 
		0x2e22, 
		0x4a80, 
		0x4c40, 
		0x4dc0, 
		0xd240, 
		0xd180, 
		0x73d1, 
		0x4c00, 
		0xf6ee, 
		0x6fb9, 
		0x5160, 
		0x5240, 
		0xc2ff, 
		0x5200, 
		0xce80, 
		0x4ee4, 
		0xf63b, 
		0xc33f, 
		0xd080, 
		0x5200, 
		0xd200, 
		0x310b, 
		0xc980, 
		0x5160, 
		0xcd00, 
		0xcfc0, 
		0x53a0, 
		0xd160, 
		0xd020, 
		0x4200, 
		0x53a0, 
		0xbd94, 
		0xb9ce, 
		0xd260, 
		0xce80, 
		0xc600, 
		0x4d40, 
		0x4200, 
		0xd234, 
		0xd3a0, 
		0x8ae5, 
		0x4000, 
		0xd060, 
		0x53e0, 
		0xd240, 
		0xca80, 
		0x7283, 
		0x5a92, 
		0x7cd9, 
		0x7c00, 
		0x0578, 
		0x2b9b, 
		0x0000, 
		0x51a0, 
		0x4b00, 
		0x30a7, 
		0xd320, 
		0x5100, 
		0x5200, 
		0xcd00, 
		0xd040, 
		0x4800, 
		0x4dc0, 
		0x50c0, 
		0xd3e0, 
		0xfc74, 
		0xd3a0, 
		0x2d65, 
		0x52c0, 
		0xf9d6, 
		0x85bc, 
		0xc000, 
		0xf821, 
		0x4c00, 
		0x5656, 
		0x400c, 
		0xb678, 
		0xd140, 
		0xd200, 
		0x51c0, 
		0x4900, 
		0x4c40, 
		0x8000, 
		0xa6be, 
		0x5240, 
		0x4a00, 
		0xd340, 
		0xd360, 
		0xd1c0, 
		0xd340, 
		0xd260, 
		0x7c00, 
		0xd2e0, 
		0x4e40, 
		0xd380, 
		0xd120, 
		0x5380, 
		0x0635, 
		0x4700, 
		0xc200, 
		0xcd40, 
		0x5280, 
		0x8000, 
		0x4d80, 
		0xd100, 
		0x4700, 
		0xd2a0, 
		0xc900, 
		0x5220, 
		0x4f40, 
		0xe6b4, 
		0xd340, 
		0xfc23, 
		0x4cc0, 
		0xcc40, 
		0x4e00, 
		0x4a09, 
		0x50a0, 
		0x0981, 
		0xc50b, 
		0x3c00, 
		0xcfc0, 
		0x50a0, 
		0xfd22, 
		0xd0a0, 
		0xfc00, 
		0xd100, 
		0xc500, 
		0x1812, 
		0xc700, 
		0x4f80, 
		0xfc00, 
		0x4e40, 
		0xcc80, 
		0x4a00, 
		0x8000, 
		0xa5a0, 
		0xc200, 
		0x4e40, 
		0xe925, 
		0x4d40, 
		0x4800, 
		0xd320, 
		0xf5f9, 
		0x5160, 
		0x4800, 
		0xa011, 
		0x4800, 
		0x4e40, 
		0x7c00, 
		0x5200, 
		0xce40, 
		0x4000, 
		0x8000  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xe00e2da0, 
		0x1bc36884, 
		0x80000000, 
		0x830ea2be, 
		0x619303e4, 
		0xffa9717a, 
		0xa8fc9bc3, 
		0xc2000000, 
		0x0c780000, 
		0x00000008, 
		0x40000000, 
		0xc1100000, 
		0xc2540000, 
		0x00800003, 
		0x005a611a, 
		0x7fc00001, 
		0x6beb6fc0, 
		0x41e00000, 
		0x417a138e, 
		0xff800000, 
		0xaaaaaaaa, 
		0x00011111, 
		0x42580000, 
		0x42300000, 
		0x836ec90e, 
		0x7f800000, 
		0x4561aecb, 
		0xc1f00000, 
		0xfe08b1d7, 
		0x00000000, 
		0xbf028f5c, 
		0x00000000, 
		0x060181dc, 
		0x00000000, 
		0x84934b35, 
		0xffa1b4df, 
		0xff800000, 
		0x4b8c4b40, 
		0x00400000, 
		0x00800003, 
		0x0c7e0000, 
		0xc1800000, 
		0x807fffff, 
		0x936be677, 
		0x007ffffe, 
		0x41100000, 
		0x00000000, 
		0x8b95bcb7, 
		0xb1f38ac8, 
		0x99244853, 
		0x00011111, 
		0x80000000, 
		0x00001000, 
		0x0c7f8000, 
		0x7f800000, 
		0x80000000, 
		0xc2240000, 
		0x7fffffff, 
		0x80000080, 
		0xff800001, 
		0x80000020, 
		0x42440000, 
		0x00400000, 
		0x21b271e3, 
		0x80800000, 
		0x0c7fff80, 
		0x80800003, 
		0x42240000, 
		0xc0e00000, 
		0x7fffffff, 
		0x0e00000f, 
		0x0038ad14, 
		0x8ea77aab, 
		0x0e007fff, 
		0xc1e80000, 
		0xa05f6608, 
		0x41400000, 
		0xef1a5edb, 
		0xc5a7a545, 
		0x7fc00000, 
		0x00724241, 
		0xc2340000, 
		0x0c600000, 
		0x9c431e4b, 
		0x00001000, 
		0xff800000, 
		0x307f30ab, 
		0x0c7fffc0, 
		0x3f800000, 
		0x420c0000, 
		0x00800000, 
		0x4524e971, 
		0xbf028f5c, 
		0x22279039, 
		0x4b000000, 
		0x7fc00000, 
		0x00000000, 
		0xc2540000, 
		0x0c7ff800, 
		0x0c7fff80, 
		0x804208b4, 
		0x00002000, 
		0x007fffff, 
		0xc2100000, 
		0x80000000, 
		0xc2740000, 
		0xb870a953, 
		0xc2600000, 
		0x801351b5, 
		0x1035c0c5, 
		0x007fffff, 
		0x42500000, 
		0x3b2cc720, 
		0x235b92a6, 
		0x40400000, 
		0x807fffff, 
		0xc2380000, 
		0xe07ad6f2, 
		0x7fffffff, 
		0x42640000, 
		0x80800000, 
		0xff949f80, 
		0x42100000, 
		0xe01424e4, 
		0x00000100, 
		0x8c43231d, 
		0x0e000001, 
		0x0e00001f, 
		0x0e07ffff, 
		0x7cdc9b22, 
		0xff7ffffe, 
		0x80000000, 
		0x00004000, 
		0x444bbd32, 
		0xbf800000, 
		0xcc1e3e1a, 
		0x00020000, 
		0x7fc00001, 
		0x423c0000, 
		0xee7a4fc6, 
		0x333bc280, 
		0xeee20391, 
		0x55ad9506, 
		0x80001000, 
		0xc2500000, 
		0x0c700000, 
		0xff7ffffe, 
		0x41800000, 
		0x80000100, 
		0x41800000, 
		0x7fad988d, 
		0xc2400000, 
		0x00400000, 
		0x0c7f0000, 
		0x40e00000, 
		0xb8dbb810, 
		0x426c0000, 
		0x8f7ffffe, 
		0xd5b81182, 
		0xc9a0db02, 
		0x80800003, 
		0x00000010, 
		0x7028abd7, 
		0xb807ad12, 
		0x42400000, 
		0x0e000007, 
		0x6e8e8d37, 
		0x7fae713f, 
		0xc19189e3, 
		0x33779614, 
		0x00000004, 
		0x0e003fff, 
		0x80100000, 
		0xc2780000, 
		0x80000000, 
		0x0c7fffff, 
		0xff7fffff, 
		0x41880000, 
		0x0c7ffffe, 
		0x80000000, 
		0x0e7fffff, 
		0x80000010, 
		0x103b1ef3, 
		0x7f7fffff, 
		0x8c5b34d9, 
		0x0e001fff, 
		0x0c600000, 
		0x80800003, 
		0x7fac9956, 
		0x08ec676f, 
		0x906b16cb, 
		0x00000000, 
		0x0e0fffff, 
		0x00000008, 
		0xec87639a, 
		0x241e23c3, 
		0x0c780000, 
		0x41b80000, 
		0x80000001, 
		0xff800000, 
		0xf0c8c24e, 
		0x00000200, 
		0x7f40c2d7, 
		0xc2640000, 
		0x28627363, 
		0x00000001, 
		0x3c26e6b0, 
		0x7fc00001, 
		0x6156f723, 
		0xc1e80000, 
		0x00000000, 
		0x41100000, 
		0x7f800000, 
		0xfeb6bda4, 
		0x4b000000, 
		0x8792354f, 
		0xc2040000, 
		0x80800003, 
		0x41c00000, 
		0xc2500000, 
		0x41400000, 
		0xc20c0000, 
		0x28dca0af, 
		0xd7ae6965, 
		0xffa6d7dd, 
		0x80000000, 
		0x00000002, 
		0x00020000, 
		0xc2480000, 
		0xc2140000, 
		0x80000000, 
		0x5ad7f6b0, 
		0x80800001, 
		0x80004000, 
		0x00800001, 
		0xc2380000, 
		0x49efb17a, 
		0xcef0cae3, 
		0x80000000, 
		0x0f7ffffd, 
		0x00000001, 
		0x0c780000, 
		0x7f800000, 
		0x42700000, 
		0xc24c0000, 
		0x41500000, 
		0x0e03ffff, 
		0xff800000, 
		0x0c7ffffc, 
		0x18fb72bb, 
		0x41980000, 
		0xbace08c2, 
		0xc23c0000, 
		0x80000002, 
		0x0c7fffe0, 
		0xffc00001, 
		0x7fbac574, 
		0xff800000, 
		0x42540000, 
		0x1676e799, 
		0x5ff81652, 
		0x00000040, 
		0x80000000, 
		0xc0c00000, 
		0x7f800000, 
		0x42340000, 
		0x80000000, 
		0x0c7ff800, 
		0x80000400, 
		0x80ffffff, 
		0xffabd1ff, 
		0x2b09c92f, 
		0xbf028f5c, 
		0xc2400000, 
		0x0c7fffc0, 
		0x0e0001ff, 
		0x0e0000ff, 
		0x80000400, 
		0x55555555, 
		0x40800000, 
		0x40000000, 
		0x9fa87f9b, 
		0x7f8eae09, 
		0x576aec11, 
		0x80000008, 
		0x80000000, 
		0x00002000, 
		0x33333333, 
		0x00000020, 
		0xf7338cda, 
		0x42280000, 
		0x00002000, 
		0x7f800000, 
		0xc1300000, 
		0x3f800001, 
		0xcb79d063, 
		0x41400000, 
		0xc1f80000, 
		0xff8fb471, 
		0x7f800000, 
		0xffdf0d6a, 
		0xffd59e18, 
		0x422c0000, 
		0x80000010, 
		0x40e00000, 
		0xc1100000, 
		0x80800003, 
		0x7fffffff, 
		0x4376bb45, 
		0xc2180000, 
		0x7fbfffff, 
		0x41f00000, 
		0xffb025a5, 
		0xd7aa3c1c, 
		0xbad370e2, 
		0x0e01ffff, 
		0x0e000001, 
		0x00200000, 
		0x00000000, 
		0x65a986f5, 
		0x2a8970f2, 
		0x00040000, 
		0xcb8c4b40, 
		0x41700000, 
		0x80011111, 
		0x42000000, 
		0xbf800000, 
		0x427c0000, 
		0x80011111, 
		0x4b71e9b3, 
		0xff800000, 
		0x3f800001, 
		0xc1d80000, 
		0xc1d00000, 
		0xbf028f5c, 
		0x0e000007, 
		0xc2200000, 
		0x7376e9ef, 
		0x00000000, 
		0x00000000, 
		0x80ffffff, 
		0x00000002, 
		0x72e89928, 
		0x00010000, 
		0x00000000, 
		0x007ffffe, 
		0x2b4411dc, 
		0x80000000, 
		0xc26c0000, 
		0x7f800000, 
		0x0e07ffff, 
		0xc2140000, 
		0x7fc00001, 
		0x807fffff, 
		0x00ffffff, 
		0x2d004bba, 
		0xfc111fc1, 
		0x994a03bb, 
		0x424c0000, 
		0x57be39b8, 
		0x3bc78a39, 
		0x841e6d05, 
		0x0c700000, 
		0xf1359d8b, 
		0x1931acc6, 
		0x00675345, 
		0xc2140000, 
		0x00000000, 
		0x41200000, 
		0xa2908493, 
		0xcccccccc, 
		0xc1980000, 
		0x0c7fffc0, 
		0xa3fb99b2, 
		0x0c400000, 
		0x0c7c0000, 
		0x42640000, 
		0x6b9561a8, 
		0x80000080, 
		0x42200000, 
		0x80000000, 
		0x8f523042, 
		0x0e000003, 
		0xedfed25b, 
		0x67b1da77, 
		0xfd0cc700, 
		0xc22c0000, 
		0x01156f4b, 
		0x0c7ff000, 
		0xc813406d, 
		0xc2000000, 
		0x0c400000, 
		0x68ab7630, 
		0x42640000, 
		0xc24c0000, 
		0xff800001, 
		0xc2500000, 
		0x0e0000ff, 
		0x7f800000, 
		0xff7fffff, 
		0x3f1f27df, 
		0x0e0003ff, 
		0xffffffff, 
		0xff7ffffe, 
		0x00200000, 
		0x8d7387bb, 
		0x42640000, 
		0xd8d2e06b, 
		0x80000000, 
		0xc2100000, 
		0x00000000, 
		0x0c7fffc0, 
		0x0e03ffff, 
		0x4bcd5b59, 
		0x80800002, 
		0x41c00000, 
		0xc1b00000, 
		0x7fbe5b11, 
		0x0c7ffff8, 
		0x80000000, 
		0x87e7b193, 
		0x0d000ff0, 
		0x7f800000, 
		0xc20c0000, 
		0x7f800000, 
		0xac50aa2b, 
		0xbf028f5c, 
		0x7ea1903b, 
		0x8f7ffffd, 
		0x41300000, 
		0x1d9fcc2b, 
		0x80011111, 
		0x7f7ffffe, 
		0xffbfffff, 
		0x3f800000, 
		0x0e003fff, 
		0x9b5ecdac, 
		0x807fffff, 
		0x00011111, 
		0xc20c0000, 
		0xc22c0000, 
		0x00000020, 
		0x7fabbb93, 
		0xff8f8530, 
		0xc2640000, 
		0xef0a4260, 
		0x9ce61103, 
		0x25072758, 
		0x0c7ffe00, 
		0xc1300000, 
		0x32dead03, 
		0x34d98aa2, 
		0x7fc00001, 
		0x94d44db9, 
		0xc2340000, 
		0x42340000, 
		0x0c7fff00, 
		0x0e01ffff, 
		0x0c7fc000, 
		0x0e7fffff, 
		0x71de04d6, 
		0x86ccd1d0, 
		0x0c400000, 
		0xd4d3b75e, 
		0x7f800000, 
		0xff800000, 
		0xff800000, 
		0x7f7fffff, 
		0x5aee82df, 
		0xffc00001, 
		0x41800000, 
		0x0c7fe000, 
		0xc1e00000, 
		0x80800002, 
		0x42280000, 
		0x7fc00001, 
		0x0c7fffc0, 
		0x0e0000ff, 
		0x80010000, 
		0xc2640000, 
		0xc1300000, 
		0x00800000, 
		0xff800000, 
		0x7f9617d2, 
		0x42700000, 
		0xff800000, 
		0x420c0000, 
		0xc1e00000, 
		0x41858d74, 
		0xa927d900, 
		0xff7fffff, 
		0xcd334dea, 
		0x002a7875, 
		0xd70c2582, 
		0x0007f913, 
		0x42180000, 
		0x42600000, 
		0x00000000, 
		0x421c0000, 
		0xff800000, 
		0x7f800000, 
		0x00000080, 
		0xc1c80000, 
		0xc8ec4e65, 
		0x00800000, 
		0x00011111, 
		0x42340000, 
		0x13e643c1, 
		0xc2140000, 
		0x80000080, 
		0x0012d6d7, 
		0xddbdb23d, 
		0xba777a65, 
		0x0c7fff00, 
		0x007fffff, 
		0x0cf12d54, 
		0x0e0fffff, 
		0x42740000, 
		0x804c85a6, 
		0x828583ce, 
		0x00000000, 
		0x4e01f85e, 
		0x0cb576f8, 
		0x807ffffe, 
		0xff7ffffe, 
		0xbf800000, 
		0x0f7ffffc, 
		0x60eab3c0, 
		0x80800002, 
		0xa6ab5ccd, 
		0xc0800000, 
		0x80000000, 
		0x0e007fff, 
		0x664b9acd, 
		0xc1880000, 
		0x80000000, 
		0x0e07ffff, 
		0x42080000, 
		0x7f800000, 
		0xff2c2a20, 
		0x0e000003, 
		0x00040000, 
		0x80000000, 
		0x41880000, 
		0x2edbd25c, 
		0xff7ffffe, 
		0x7f800000, 
		0xc2780000, 
		0x0e0000ff, 
		0xc2080000, 
		0xff800000, 
		0x804c1639, 
		0xc0c00000, 
		0xc2080000, 
		0x41800000, 
		0x610c8f3f, 
		0x42680000, 
		0x807ffffe, 
		0x0b7790fd, 
		0x42300000, 
		0xb439da5c, 
		0x80000000, 
		0x42780000, 
		0x00200000, 
		0x80000000, 
		0x4b8c4b40, 
		0x0d000ff0, 
		0x0c7fff80, 
		0x0d8b70b6, 
		0x5a59f0f9, 
		0x80000001, 
		0x0013746e, 
		0xff800000, 
		0x42040000, 
		0x2c954266, 
		0xff800000, 
		0x4b4bfe08, 
		0x41800000, 
		0x0e0001ff, 
		0xd4cbcdd0, 
		0xc25c0000, 
		0x43ea8d08, 
		0x007ffffe, 
		0xce64f8ec, 
		0x0d00fff0, 
		0xd17a2694, 
		0xff800000, 
		0x8e44e8fd, 
		0x7f800000, 
		0x0e00000f, 
		0xe58c73da, 
		0x134fe860, 
		0xd6b9e749, 
		0x41600000, 
		0xc2300000, 
		0xc24c0000, 
		0xc0000000, 
		0xcb8c4b40, 
		0xf049607e, 
		0xc1900000, 
		0xc2480000, 
		0xfc16137b, 
		0x00000000, 
		0x0e00003f, 
		0xc25c0000, 
		0x80000000, 
		0xff7fffff, 
		0x41a00000, 
		0x41e80000, 
		0x7fbffd5e, 
		0x063946ec, 
		0x679eb46f, 
		0xc1000000, 
		0xbf028f5c, 
		0x41f80000, 
		0x36268700, 
		0xc1900000, 
		0xd72376ce, 
		0x95d3e25b, 
		0xc1100000, 
		0xc0896c73, 
		0xc2500000, 
		0x7e222b03, 
		0x7f7fffff, 
		0x7f800000, 
		0x80080000, 
		0x00000100, 
		0x7f7ffffe, 
		0x80800003, 
		0x055d4a99, 
		0x00800001, 
		0x7f800000, 
		0xbf800001, 
		0x543591b7, 
		0xee75b01e, 
		0x80000000, 
		0x42080000, 
		0xbf5118b1, 
		0xe52603ee, 
		0x1f0ddbeb, 
		0xc5ad3ae6, 
		0x80001000, 
		0x00004000, 
		0x3b9ab5f3, 
		0x00002000, 
		0x42440000, 
		0x42140000, 
		0x41d00000, 
		0x007fffff, 
		0x41a00000, 
		0xc353234c, 
		0x0c7e0000, 
		0x58bce5fe, 
		0x36958eae, 
		0x00000000, 
		0x0d8f102e, 
		0x007ffffe, 
		0x7f7ffffe, 
		0xff7ffffe, 
		0xfd7bdfba, 
		0xc80bf4e7, 
		0x943ef306, 
		0x0c7fffe0, 
		0x85dfacc2, 
		0x765cba56, 
		0xc2180000, 
		0xd909ebf7, 
		0x7f800000, 
		0x423c0000, 
		0x04f5f373, 
		0x00011111, 
		0x44acfe7b, 
		0xc2480000, 
		0xc1b80000, 
		0x8f7ffffe, 
		0x80000000, 
		0x84e39954, 
		0x41e00000, 
		0xb931b01b, 
		0x7f7ffffe, 
		0xc1d80000, 
		0x0e03ffff, 
		0xc0000000, 
		0x00000010, 
		0x310de09a, 
		0xdf2c6c5f, 
		0xff800000, 
		0x80000000, 
		0xc2340000, 
		0x0c7fffe0, 
		0xc23c0000, 
		0xc1e80000, 
		0x41800000, 
		0x0ed590e8, 
		0x0e000fff, 
		0x7f800000, 
		0x42500000, 
		0xc2680000, 
		0x00800003, 
		0x007fffff, 
		0x41f80000, 
		0x80000040, 
		0xc1000000, 
		0x0cf2569d, 
		0x3e75f367, 
		0xc2200000, 
		0x80000010, 
		0xbf800000, 
		0xc2740000, 
		0x0c7fffff, 
		0xff7ffffe, 
		0x420c0000, 
		0x886c00de, 
		0xbf800001, 
		0xff800000, 
		0x80ffffff, 
		0xffc07b40, 
		0x7f7fffff, 
		0xc22c0000, 
		0xc0800000, 
		0x5fed64be, 
		0xc1b00000, 
		0xce9410a3, 
		0xc2400000, 
		0x80000020, 
		0x7fc00000, 
		0x425c0000, 
		0xffe54ef9, 
		0xc1600000, 
		0x42680000, 
		0xc0e00000, 
		0xc0c00000, 
		0x42740000, 
		0x00000000, 
		0x2de168b1, 
		0x0f7ffffe, 
		0x42400000, 
		0x80000000, 
		0x4b8c4b40, 
		0x803b55ca, 
		0x0e000003, 
		0xff800000, 
		0x7f7fffff, 
		0x4d90480d, 
		0x42580000, 
		0x0e000003, 
		0xbdef9b69, 
		0xff800000, 
		0xc2540000, 
		0x00000008, 
		0x80ffffff, 
		0x41c80000, 
		0xc21c0000, 
		0xda525538, 
		0x42e213b4, 
		0x0b780832, 
		0xa09b6649, 
		0x41300000, 
		0x23bf731d, 
		0x7f800000, 
		0x7fc00000, 
		0x0e3fffff, 
		0xc2680000, 
		0x0c7fff80, 
		0x7f800000, 
		0x00400000, 
		0xc1e00000, 
		0x80000000, 
		0x5a0ce60d, 
		0x41e80000, 
		0x00800003, 
		0x80100000, 
		0x00800001, 
		0x22699ab2, 
		0x00080000, 
		0x80800003, 
		0xc23c0000, 
		0xc1a80000, 
		0xaaaaaaaa, 
		0x0e00003f, 
		0x00008000, 
		0x41f00000, 
		0x4510b091, 
		0xdda9bce6, 
		0x42140000, 
		0x00000000, 
		0xbf800000, 
		0x41880000, 
		0x0c7ffe00, 
		0x00000001, 
		0x564c2249, 
		0x00000000, 
		0xc1c00000, 
		0xc1700000, 
		0x7f800000, 
		0xd483386a, 
		0x0ba5da1b, 
		0xbd56aed5, 
		0x42480000, 
		0x0a45d722, 
		0xc1a00000, 
		0x5f721d3f, 
		0xc1880000, 
		0x61897b64, 
		0x3ec6ea82, 
		0x0c780000, 
		0x0c7c0000, 
		0x00000001, 
		0xf6f30193, 
		0x49f4fb99, 
		0x0e0000ff, 
		0x0c7e0000, 
		0x533f8427, 
		0x0e0003ff, 
		0x80011111, 
		0xc1e00000, 
		0x427c0000, 
		0x00800003, 
		0x3f028f5c, 
		0x00000080, 
		0x80011111, 
		0x80000100, 
		0x291b2e9e, 
		0x80000000, 
		0x80000000, 
		0x5a02b688, 
		0xc1c00000, 
		0xae718258, 
		0x42340000, 
		0x00000400, 
		0xffbfffff, 
		0x53981011, 
		0x00000010, 
		0x42240000, 
		0x0e00003f, 
		0x426c0000, 
		0xcccccccc, 
		0x16c83ef0, 
		0x80000010, 
		0x7f800000, 
		0x9d855a5a, 
		0x42140000, 
		0x42700000, 
		0xc2540000, 
		0x0c7ffe00, 
		0x80200000, 
		0x7f8ec0ca, 
		0x40400000, 
		0x3f800000, 
		0xc0a00000, 
		0x80000000, 
		0x426c0000, 
		0xb885579d, 
		0xbf800001, 
		0x75a50cfb, 
		0x80000000, 
		0x41e00000, 
		0x00040000, 
		0x4b8c4b40, 
		0x5d0f8cde, 
		0x00004000, 
		0x7f800000, 
		0x427c0000, 
		0x88abdc65, 
		0x41500000, 
		0xbe67af00, 
		0xc2480000, 
		0x00000000, 
		0x80000010, 
		0xe0e16601, 
		0x0c400000, 
		0x42180000, 
		0x4b095821, 
		0x80000000, 
		0xff7ffffe, 
		0x0f7fffff, 
		0x8795c645, 
		0xc2200000, 
		0x320380ee, 
		0x42480000, 
		0x7cc142cb, 
		0x8697d615, 
		0x420c0000, 
		0x7fafe1aa, 
		0x3cf41f14, 
		0x00ffffff, 
		0xa18cedc5, 
		0x7f800001, 
		0xdf2b72de, 
		0xffc00000, 
		0x7fb34093, 
		0x00800003, 
		0xc1d00000, 
		0x5342c23e, 
		0x007fffff, 
		0x00000400, 
		0xbf028f5c, 
		0x00008000, 
		0x6ce72982, 
		0x0221b300, 
		0x80800003, 
		0x807ffffe, 
		0x7f800000, 
		0x80000040, 
		0xc2080000, 
		0x80000000, 
		0x80000001, 
		0x7f800000, 
		0x80000000, 
		0x8f7ffffc, 
		0x7fffd585, 
		0x41400000, 
		0xb48215e3, 
		0x7fc00000, 
		0xf4990e93, 
		0xc26c0000, 
		0x40800000, 
		0x00000000, 
		0x80000010, 
		0xff800000, 
		0x00000000, 
		0x3f800001, 
		0x1397f8a0, 
		0x7f800000, 
		0xdb2013c6, 
		0xcb8c4b40, 
		0xc2740000, 
		0x7f7ffffe, 
		0x3f800001, 
		0xc21c0000, 
		0x42300000, 
		0x42340000, 
		0xd4e82ff4, 
		0xa3ae507b, 
		0x41400000, 
		0x51aa1742, 
		0xffffffff, 
		0x4b63f572, 
		0x42780000, 
		0x006f6dcc, 
		0x7ee1d5fe, 
		0x41100000, 
		0xc1e00000, 
		0x114cbe77, 
		0x4b000000, 
		0x80800000, 
		0x0e000fff, 
		0xc2100000, 
		0x00800000, 
		0xabbc67bc, 
		0x55555555, 
		0x0c7ff000, 
		0x9fa2aca2, 
		0xc1c80000, 
		0x41e00000, 
		0x7227a6c4, 
		0xc0e00000, 
		0xea3ec566, 
		0x00000800, 
		0xc2e2254b, 
		0x41600000, 
		0x7f7ffffe, 
		0xd0e821d7, 
		0x42000000, 
		0x0c7fe000, 
		0x80000800, 
		0x42580000, 
		0xb22acaac, 
		0x80000001, 
		0x0c7ffff0, 
		0x00000000, 
		0x8cf31998, 
		0xf0019132, 
		0x0e01ffff, 
		0x7f95a99b, 
		0x80000000, 
		0x2993eb88, 
		0x05af6572, 
		0x3f800001, 
		0x33333333, 
		0xcd480a82, 
		0x423c0000, 
		0xb8fc4903, 
		0x80800001, 
		0x0c7ffc00, 
		0xba3382f5, 
		0x42700000, 
		0x465c3203, 
		0x41f00000, 
		0xc2280000, 
		0x80000000, 
		0xc2700000, 
		0x7f000000, 
		0xc1100000, 
		0xff800000, 
		0xc1f80000, 
		0x23e4d3ea, 
		0x42240000, 
		0x8f7ffffe, 
		0x80000000, 
		0x00000001, 
		0x7fab647e, 
		0xff800000, 
		0xc1300000, 
		0x7f800000, 
		0x0c7ff000, 
		0x0e1fffff, 
		0x0c7f0000, 
		0x33c05de4, 
		0x80800001, 
		0xc2000000, 
		0x80ffffff, 
		0x00000000, 
		0x00000000, 
		0x0c7ffffe, 
		0x47a11534, 
		0xff7ffffe, 
		0xc1600000, 
		0x41400000, 
		0x0b3653ef, 
		0xea56c124, 
		0x80011111, 
		0x81d06c37, 
		0x8f7ffffd, 
		0xb5ea9463, 
		0x427c0000, 
		0x80100000, 
		0x00000004, 
		0xb1dd61a3, 
		0x692b7c73, 
		0x42340000, 
		0x0e01ffff, 
		0x25f6f89d, 
		0xc1800000, 
		0x859626dd, 
		0x4e48e535, 
		0x80000000, 
		0xf510d8a4, 
		0xffc00001, 
		0xff8bb950, 
		0x42380000, 
		0x1684b808, 
		0x42780000, 
		0xc21c0000, 
		0x98329a1a, 
		0xc1980000, 
		0xc1b00000, 
		0xcccccccc, 
		0x0c7f8000, 
		0xc2240000, 
		0xc2080000, 
		0x15f7464a, 
		0x00795474, 
		0xef482efc, 
		0x6be79976, 
		0x90314406, 
		0x80000000, 
		0x98d603bd, 
		0xff800001, 
		0xc0e00000, 
		0xff7fffff, 
		0x00000000, 
		0x41980000, 
		0xffffffff, 
		0x42140000, 
		0xc1000000, 
		0x0e003fff, 
		0x00000000, 
		0x41e00000, 
		0x42340000, 
		0x83c25e08, 
		0x7f7ffffe, 
		0xe12b2bdc, 
		0x0c780000, 
		0x3cce544a, 
		0x508ca089, 
		0x7fc00000, 
		0x2bf5f9e6, 
		0xc2700000, 
		0xe1c5944c, 
		0xc2040000, 
		0x41b80000, 
		0x13e1da6e, 
		0x80000000, 
		0xc1c80000, 
		0x80000000, 
		0x0191cf43, 
		0x8b1273d0, 
		0x00800001, 
		0xb02dd73a, 
		0xaa9eac8b, 
		0xc22c0000, 
		0x80000000, 
		0x5258052c, 
		0x41900000, 
		0x41980000, 
		0xfdd10c98, 
		0x7fffffff, 
		0x80002000, 
		0x7fc00000, 
		0x00800000, 
		0xa8fde5a2, 
		0x174d9d73, 
		0xc0000000, 
		0x42740000, 
		0x80000001, 
		0x40e00000, 
		0xcafa5a1f, 
		0x9c06f42d, 
		0x41c80000, 
		0xff9abd9d, 
		0x80000010, 
		0x05afcea6, 
		0x7f800001, 
		0x00000000, 
		0xdede979f, 
		0x00400000, 
		0xc27c0000, 
		0x7f7ffffe, 
		0x97aa86a2, 
		0x7fffffff, 
		0xa5dff438, 
		0x80800002, 
		0x0c7ffff8, 
		0x87bd5eac, 
		0x00001000, 
		0x7f7fffff, 
		0xc1800000, 
		0x8f7ffffd, 
		0x42540000, 
		0x00000000, 
		0x80000000, 
		0x421c0000, 
		0xff7ffffe, 
		0x563c404b, 
		0xc1600000, 
		0x7d0e3e8b, 
		0xff800000, 
		0x40c00000, 
		0x4df13c6a, 
		0x0f7ffffe, 
		0x40400000, 
		0x80000000, 
		0x00800000, 
		0x00800001, 
		0xd55f76e7, 
		0x0c7f8000, 
		0xc2700000, 
		0x00000008, 
		0x7fc00001, 
		0x00800000, 
		0x07ca4544, 
		0xc2540000, 
		0x0c700000, 
		0xd836d27d, 
		0x41c00000, 
		0x7f923c6f, 
		0x80000000, 
		0x0c7fffe0, 
		0xc2080000, 
		0x00080000, 
		0xc2580000, 
		0x1c58731a, 
		0x00080000, 
		0x80000100, 
		0x80000010, 
		0x00000040, 
		0x80000000, 
		0x09014909, 
		0x0c7c0000, 
		0x7f800000, 
		0xaabff6b9, 
		0x7f800000, 
		0xf2c68003, 
		0x1f53f328, 
		0x8f7fffff, 
		0x80000000, 
		0x7f7ffffe, 
		0x00000001, 
		0x80000000, 
		0x00800003, 
		0x3f800001, 
		0xffb21ae5, 
		0xcccccccc, 
		0x41c00000, 
		0x42100000, 
		0x00080000, 
		0x0e000001, 
		0xed7cb1cf, 
		0x42680000, 
		0x6a9bb23b, 
		0x80000020, 
		0x41000000, 
		0xc1300000, 
		0x692c3430, 
		0x903328f7, 
		0x40a00000, 
		0x80000000, 
		0x41000000, 
		0x81c9f178, 
		0x11223181, 
		0x80800002, 
		0x00200000, 
		0xee6baa50, 
		0x60393084, 
		0x0f7ffffc, 
		0x00000001, 
		0x41100000, 
		0xc2440000, 
		0x41b00000, 
		0x0c7fffc0, 
		0x650a4e03, 
		0xe0ac6cbc, 
		0xc1f00000, 
		0xbf7d7b41, 
		0x00200000, 
		0x427c0000, 
		0x7f7ffffe, 
		0x80000400, 
		0x00000020, 
		0x72fd435a, 
		0x41300000, 
		0x00000040, 
		0xaf3f9aa9, 
		0x420c0000, 
		0x3f800000, 
		0x424c0000, 
		0x2f3facfb, 
		0xff800000, 
		0xc1d00000, 
		0x00800001, 
		0xbf028f5c, 
		0x80ffffff, 
		0xff000000, 
		0x0e0fffff, 
		0x7f800000, 
		0xc26c0000, 
		0xc0a00000, 
		0x00200000, 
		0x425c0000, 
		0x8f7fffff, 
		0xffbfffff, 
		0x00800001, 
		0x80800002, 
		0x00000040, 
		0x807fffff, 
		0x41b00000, 
		0xc1200000, 
		0x00008000, 
		0xc98e200a, 
		0x0c7f8000, 
		0x4cc72b89, 
		0x00800002, 
		0xdb544361, 
		0x0e003fff, 
		0x4b000000, 
		0xa3c1411f, 
		0x00000000, 
		0xa62894b1, 
		0x0d00fff0, 
		0x425c0000, 
		0xc25c0000, 
		0xb8318d57, 
		0x4b8c4b40, 
		0x00400000, 
		0x0c7fc000, 
		0x0c7fffc0, 
		0x411371d1, 
		0xff8c81ba, 
		0x0c7fffff, 
		0x80000001, 
		0x8c39cf4c, 
		0x830793fa, 
		0x80800000, 
		0x80000001, 
		0x82ab7ec4, 
		0x80800002, 
		0x41400000, 
		0x0c7ffff0, 
		0xc2680000, 
		0x3f028f5c, 
		0xc2200000, 
		0x41880000, 
		0x807ffffe, 
		0x7f7ffffe, 
		0xff800000, 
		0x2570bc71, 
		0x00800001, 
		0x41e80000, 
		0x29dbeac7, 
		0x41880000, 
		0xe3ccd05a, 
		0xb95433f3, 
		0xcb8c4b40, 
		0x00000000, 
		0x41abbe6d, 
		0xc1100000, 
		0xc2540000, 
		0xff800000, 
		0x42280000, 
		0x80000000, 
		0x00800003, 
		0x0c7f0000, 
		0x41f80000, 
		0x4c088c69, 
		0x42ab4b1d, 
		0xc1700000, 
		0xb5d7f8ac, 
		0x0e007fff, 
		0x80800001, 
		0xc1300000, 
		0xdaf418e7, 
		0x0c7fffff, 
		0x00800002, 
		0xc1d00000, 
		0x7fc00000, 
		0xc0800000, 
		0x80011111, 
		0x41204fe3, 
		0x0c7fff80, 
		0x0e00000f, 
		0x28b51597, 
		0xc1f80000, 
		0xcccccccc, 
		0x42780000, 
		0x7f800000, 
		0xcb000000, 
		0xe058fada, 
		0x80000000, 
		0x80000100, 
		0xeefd9f7c, 
		0xff800000, 
		0x42480000, 
		0x00800000, 
		0x00000000, 
		0x6020e8df, 
		0x25e47bad, 
		0x42380000, 
		0x00008000, 
		0x75db1999, 
		0xc0c00000, 
		0x00000080, 
		0x00011111, 
		0x80000200, 
		0x13ded1d8, 
		0x41500000, 
		0x7070c97f, 
		0x6bee5a0a, 
		0x3f800001, 
		0x42680000, 
		0x01742b73, 
		0x41000000, 
		0x9b8095d4, 
		0x422c0000, 
		0x3f34306f, 
		0x918242e8, 
		0x41a00000, 
		0x00010000, 
		0xbf800000, 
		0x51730448, 
		0xc2380000, 
		0x80000000, 
		0xc0e00000, 
		0x80800002, 
		0x00800002, 
		0x678a98b9, 
		0x41b00000, 
		0x42240000, 
		0xe0e3005a, 
		0xff800000, 
		0x0f7ffffd, 
		0xff7ffffe, 
		0x7eef0397, 
		0x80000400, 
		0x41980000, 
		0x80000800, 
		0xffbfffff, 
		0x41b00000, 
		0x0e000007, 
		0xec7d420f, 
		0xc2580000, 
		0x7f800000, 
		0x0e7fffff, 
		0x00000001, 
		0x41980000, 
		0xffbfffff, 
		0xc1a80000, 
		0x0c700000, 
		0x80000080, 
		0x7f800000, 
		0x289d7406, 
		0x0f7ffffd, 
		0x420c0000, 
		0x42340000, 
		0x0004079b, 
		0x3f028f5c, 
		0x0e000fff, 
		0x3ef2298d, 
		0xfb04af00, 
		0xc1100000, 
		0x80100000, 
		0x80000001, 
		0x422c0000, 
		0xc2080000, 
		0xc8cb0b71, 
		0x0e000003, 
		0x704deb53, 
		0xff800001, 
		0x552b0def, 
		0x7f800000, 
		0x0c7f0000, 
		0x80000040, 
		0x27ae83b7, 
		0x42180000, 
		0x00000000, 
		0xc1e80000, 
		0x7fc00000, 
		0x41c80000, 
		0x77626c59, 
		0x006d5e8c, 
		0xadcd5f89, 
		0xbef5559f, 
		0x41900000, 
		0x205a06b2, 
		0x80020000, 
		0x41400000, 
		0x36c61a24, 
		0xf0d9310e, 
		0xe442ebd3, 
		0x00800000, 
		0xff800000, 
		0x41200000, 
		0x7f800000, 
		0x49c166c8, 
		0xc2080000, 
		0x0e0007ff, 
		0x7f7ffffe, 
		0x1c31e2ea, 
		0x80000000, 
		0x41600000, 
		0xc1c80000, 
		0x7fc00001, 
		0xc2380000, 
		0x3a8d4b2c, 
		0x9f1d0d02, 
		0x00800002, 
		0xe9842648, 
		0xc2200000, 
		0x42500000, 
		0x00000000, 
		0xc1200000, 
		0x0ea91824, 
		0x89a1dfac, 
		0x6184f472, 
		0x80000000, 
		0x80000000, 
		0x810b7f9c, 
		0x41a00000, 
		0x7fffffff, 
		0x0c7f8000, 
		0x7c3d1cb4, 
		0x80000020, 
		0x80000001, 
		0x8ff8f1fe, 
		0xc2600000, 
		0x8317436c, 
		0xae5e2662, 
		0x41d80000, 
		0x00000002, 
		0x41a80000, 
		0x0e01ffff, 
		0x2b27fef5, 
		0xcbf90dac, 
		0xc1a80000, 
		0x00000000, 
		0x0c7fff00, 
		0xdb1a18b7, 
		0x61562275, 
		0xbf028f5c, 
		0x6771c2e0, 
		0xcb8c4b40, 
		0x800f15b2, 
		0x80000008, 
		0x07f85ab2, 
		0x80000010, 
		0x80000000, 
		0x80000080, 
		0x62f9a992, 
		0x00004000, 
		0x0c7ffff8, 
		0x42300000, 
		0x0e0fffff, 
		0x41d80000, 
		0xc2740000, 
		0x00800001, 
		0x80011111, 
		0x80000001, 
		0x80100000, 
		0xc1f00000, 
		0x00002000, 
		0x8ce4b979, 
		0x0e007fff, 
		0xff800000, 
		0xff800000, 
		0x41b00000, 
		0x89a1f3a7, 
		0x984cc7f4, 
		0x41880000, 
		0x0e000007, 
		0x3f800001, 
		0x8c4145a3, 
		0x675caa97, 
		0xaaaaaaaa, 
		0x7f7fffff, 
		0x00ffffff, 
		0x7f800000, 
		0xc2500000, 
		0x88991077, 
		0x7f7ffffe, 
		0x7f800000, 
		0x7fbfffff, 
		0x0c400000, 
		0x00000000, 
		0x5d953057, 
		0xcccccccc, 
		0xc0e00000, 
		0xffffffff, 
		0x41d00000, 
		0xff800000, 
		0xc2140000, 
		0x114f53fe, 
		0x0c7ff000, 
		0x52c4ce6d, 
		0xff000000, 
		0x0c600000, 
		0x80800001  
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
		0x0000022d, 
		0x00000259, 
		0x00000291, 
		0x000002c1, 
		0x000000af, 
		0x000000fd, 
		0x000000f7, 
		0x0000026f, 
		0x000000f3, 
		0x00000383, 
		0x00000349, 
		0x00000069, 
		0x00000121, 
		0x0000031d, 
		0x000002a3, 
		0x00000095, 
		0x0000036f, 
		0x0000019f, 
		0x00000299, 
		0x000001b1, 
		0x000000d9, 
		0x000003b9, 
		0x000000e5, 
		0x000002bb, 
		0x00000329, 
		0x00000075, 
		0x00000249, 
		0x00000229, 
		0x0000031d, 
		0x0000022b, 
		0x00000049, 
		0x0000037b, 
		0x000003a5, 
		0x00000367, 
		0x00000341, 
		0x00000135, 
		0x00000035, 
		0x000003ad, 
		0x000000cd, 
		0x00000145, 
		0x00000155, 
		0x00000097, 
		0x00000119, 
		0x0000005f, 
		0x000002c1, 
		0x00000079, 
		0x00000211, 
		0x000000a3, 
		0x0000035d, 
		0x0000031f, 
		0x00000303, 
		0x0000001d, 
		0x000001fd, 
		0x000002ef, 
		0x000002db, 
		0x000001a5, 
		0x000001a3, 
		0x00000333, 
		0x00000111, 
		0x000001cd, 
		0x000002cb, 
		0x000000c5, 
		0x000002b1, 
		0x000003c5, 
		0x00000239, 
		0x000000a9, 
		0x000001cb, 
		0x0000022d, 
		0x0000016d, 
		0x000000a5, 
		0x0000023d, 
		0x000003e9, 
		0x00000233, 
		0x000000bd, 
		0x00000067, 
		0x00000269, 
		0x00000319, 
		0x0000019d, 
		0x000002f1, 
		0x0000036f, 
		0x00000241, 
		0x00000085, 
		0x000002e5, 
		0x000000cb, 
		0x000000a1, 
		0x0000017b, 
		0x000000a9, 
		0x000000f1, 
		0x000003eb, 
		0x0000007d, 
		0x000000ad, 
		0x00000027, 
		0x000000f7, 
		0x000003bd, 
		0x000002e3, 
		0x000003c7, 
		0x000000ad, 
		0x000003a3, 
		0x000002db, 
		0x000000d3, 
		0x0000034f, 
		0x000002e5, 
		0x00000003, 
		0x000001f1, 
		0x000000b9, 
		0x00000349, 
		0x00000145, 
		0x000001a5, 
		0x000003a1, 
		0x0000025d, 
		0x00000347, 
		0x0000008d, 
		0x0000037d, 
		0x00000369, 
		0x00000167, 
		0x000003b3, 
		0x000000c5, 
		0x0000028f, 
		0x0000032b, 
		0x0000035b, 
		0x0000036b, 
		0x000003b7, 
		0x0000020d, 
		0x0000009b, 
		0x000002af, 
		0x0000030b, 
		0x0000016d, 
		0x000000eb, 
		0x00000139, 
		0x000001c5, 
		0x00000237, 
		0x0000026d, 
		0x00000313, 
		0x000000f5, 
		0x00000127, 
		0x0000004f, 
		0x000000d3, 
		0x00000019, 
		0x000000c1, 
		0x00000017, 
		0x0000009f, 
		0x000001d5, 
		0x000001f1, 
		0x00000351, 
		0x0000030d, 
		0x000003bb, 
		0x000003a1, 
		0x00000011, 
		0x000000c5, 
		0x00000247, 
		0x00000041, 
		0x000003a3, 
		0x000001d5, 
		0x0000031b, 
		0x000000df, 
		0x0000023f, 
		0x00000191, 
		0x0000028b, 
		0x00000319, 
		0x00000113, 
		0x0000029d, 
		0x0000016b, 
		0x00000175, 
		0x000003b9, 
		0x0000023b, 
		0x00000349, 
		0x000001cb, 
		0x000003a9, 
		0x00000351, 
		0x000001ab, 
		0x00000285, 
		0x000000c9, 
		0x0000029f, 
		0x0000002d, 
		0x000001a1, 
		0x00000185, 
		0x000000b3, 
		0x000003cd, 
		0x0000031f, 
		0x00000063, 
		0x00000121, 
		0x000000c3, 
		0x00000255, 
		0x00000047, 
		0x000003b3, 
		0x00000039, 
		0x000000cb, 
		0x000002f5, 
		0x0000004f, 
		0x00000367, 
		0x00000111, 
		0x00000245, 
		0x000001f5, 
		0x00000043, 
		0x00000381, 
		0x000000bb, 
		0x0000038d, 
		0x000001a1, 
		0x000003e9, 
		0x0000026f, 
		0x00000273, 
		0x00000347, 
		0x000000fb, 
		0x0000008d, 
		0x000002ff, 
		0x00000027, 
		0x000001c7, 
		0x000000b5, 
		0x000000e5, 
		0x0000028b, 
		0x00000239, 
		0x00000393, 
		0x00000069, 
		0x00000137, 
		0x000002ed, 
		0x0000015b, 
		0x000001c3, 
		0x00000281, 
		0x000002fd, 
		0x000002b3, 
		0x000001b1, 
		0x0000007b, 
		0x0000039f, 
		0x0000039d, 
		0x00000101, 
		0x00000159, 
		0x000002ad, 
		0x00000315, 
		0x00000085, 
		0x00000205, 
		0x000002a5, 
		0x00000089, 
		0x000003d1, 
		0x00000201, 
		0x00000051, 
		0x000000f3, 
		0x00000173, 
		0x000003df, 
		0x00000179, 
		0x00000041, 
		0x00000021, 
		0x000001cd, 
		0x000000fd, 
		0x00000277, 
		0x000003db, 
		0x000003f9, 
		0x000003cf, 
		0x0000015f, 
		0x00000257, 
		0x00000101, 
		0x00000133, 
		0x000002db, 
		0x00000105, 
		0x00000041, 
		0x0000039f, 
		0x000002fb  
};

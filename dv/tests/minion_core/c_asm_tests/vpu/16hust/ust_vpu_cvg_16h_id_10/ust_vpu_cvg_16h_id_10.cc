/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "macros.h"
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
void c_sync_point(uint64_t shire_id, uint64_t sync_odd_harts){
		__asm__ __volatile__ ( 
			".equ BARRIER_COUNT, 0x7\n"
			"li x5, (BARRIER_COUNT << 5 )\n"
			"beq %[sync_odd_harts], x0, 4f\n"
			"add x5, x5, x5\n"
			"add x5, x5, %[sync_odd_harts]\n"
			"4:\n"          
			"csrrw x31, 0x820, x5\n" 
			"beq x31, x0, 1f\n" 
			"add x5, %[shire_id], x0 \n"
			"slli x5, x5, 22 \n"
			"li x31, 0x803400C0 \n"
			"add x5, x5, x31 \n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"beq %[sync_odd_harts], x0, 3f\n"
			"sd x31, 16(x5)\n"
			"3:\n"          
			"fence\n"          
			"csrw 0x821, x0\n" 
			"j 2f\n"
			"1:\n"          
			"csrw 0x821, x0\n" 
			"2:\n"          
			"nop\n"
			: : [shire_id] "r" (shire_id), [sync_odd_harts] "r" (sync_odd_harts)  :      
		);
}
void trans(void){
		__asm__ __volatile__ ( 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_int_32\n"
			"flw.ps f0, 0 (x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 0 (x5)\n"
			"li x5, 493\n"
			"faddi.pi f2, f0, 184\n"
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
			"3:\n"
			"fexp.ps f18, f2\n"
			"faddi.pi f2, f2, 128\n"
			"bne x0, x0, 3b\n"
			"4:\n"
			"fexp.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"bne x0, x0, 4b\n"
			"fmadd.ps f25, f26, f31, f22\n"
			"fmadd.ps f20, f31, f30, f22\n"
			"fmsub.ps f19, f26, f22, f22\n"
			"fnmadd.ps f19, f27, f25, f30\n"
			"fmsub.ps f25, f30, f20, f22\n"
			"fexp.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"fexp.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"fmadd.ps f31, f24, f22, f22\n"
			"fnmsub.ps f22, f30, f23, f21\n"
			"fnmadd.ps f24, f28, f23, f30\n"
			"fmsub.ps f22, f28, f29, f30\n"
			"fmadd.ps f29, f29, f24, f31\n"
			"fmadd.ps f22, f29, f24, f22\n"
			"fmsub.ps f24, f24, f26, f21\n"
			"fmsub.ps f28, f31, f30, f22\n"
			"fexp.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"fexp.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"fmadd.ps f31, f30, f31, f25\n"
			"fmsub.ps f26, f29, f27, f27\n"
			"fmsub.ps f27, f24, f31, f23\n"
			"fmadd.ps f23, f26, f25, f31\n"
			"fmsub.ps f31, f30, f25, f27\n"
			"fmsub.ps f30, f31, f30, f30\n"
			"1:\n"
			"fexp.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"bne x0, x0, 1b\n"
			"fmax.ps f26, f25, f27\n"
			"fmax.ps f31, f28, f27\n"
			"fmin.ps f30, f29, f30\n"
			"fmax.pi f25, f27, f29\n"
			"fmax.pi f27, f29, f24\n"
			"fmin.ps f25, f27, f25\n"
			"fmin.pi f26, f26, f25\n"
			"fmin.pi f29, f31, f29\n"
			"fexp.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"fexp.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"fexp.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"fmin.ps f28, f28, f30\n"
			"fmax.ps f30, f27, f28\n"
			"fmax.ps f27, f29, f31\n"
			"fmul.ps f29, f30, f29\n"
			"fexp.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"fmin.pi f30, f30, f29\n"
			"fadd.pi f30, f30, f29\n"
			"fmin.pi f31, f28, f31\n"
			"fadd.ps f31, f28, f28\n"
			"fmin.ps f30, f28, f28\n"
			"fexp.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"fexp.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"fnmadd.ps f30, f31, f31, f31\n"
			"fmsub.ps f30, f30, f31, f31\n"
			"fnmadd.ps f30, f30, f31, f31\n"
			"fnmsub.ps f31, f30, f31, f30\n"
			"fmadd.ps f31, f30, f31, f31\n"
			"fmsub.ps f31, f31, f30, f30\n"
			"fnmadd.ps f31, f31, f31, f30\n"
			"fnmsub.ps f30, f30, f30, f30\n"
			"fexp.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fexp.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"fsub.pi f4, f18, f0\n"
			"fand.pi f19, f29, f2\n"
			"fmin.pi f4, f22, f22\n"
			"fand.pi f27, f18, f21\n"
			"fexp.ps f1, f17\n"
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
	uint64_t shire_id;
	hid = get_hart_id();
	shire_id = hid >> 6;
	if ( hid == 0 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
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
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
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
			"li x31, 0x803400C0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400C8\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D8\n" 
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
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0xa\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 800(x28)\n"
			"flw.ps f1, 352(x28)\n"
			"fle.ps f3, f13, f1\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 224(x26)\n"
			"faddi.pi f15, f27, 0x1a4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 928(x15)\n"
			"flw.ps f9, 352(x15)\n"
			"flw.ps f15, 256(x15)\n"
			"fmadd.s f1, f6, f9, f15, rdn\n" 
			"fbci.ps f9, 0x7b041\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 0(x15)\n"
			"flw.ps f27, 704(x15)\n"
			"flw.ps f4, 672(x15)\n"
			"fmadd.s f9, f9, f27, f4, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f27, 224(x27)\n"
			"fandi.pi f21, f27, 0x17b\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 384(x25)\n"
			"fltu.pi f20, f13, f13\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 928(x28)\n"
			"fexp.ps f4, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 928(x28)\n"
			"flw.ps f26, 192(x28)\n"
			"fminu.pi f16, f12, f26\n" 
			"fbci.ps f27, 0x72eb7\n" 
			"fbci.ps f3, 0x7ac57\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 704(x28)\n"
			"flw.ps f21, 512(x28)\n"
			"fltu.pi f17, f6, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskor m6, m6, m6\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 32(x17)\n"
			"flw.ps f19, 128(x17)\n"
			"flw.ps f21, 448(x17)\n"
			"fnmsub.ps f21, f2, f19, f21, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 544(x17)\n"
			"flw.ps f2, 448(x17)\n"
			"feq.ps f9, f20, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 128(x15)\n"
			"flw.ps f19, 960(x15)\n"
			"fsub.ps f27, f26, f19, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x0 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x1 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x18 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xc << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1f << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 512(x14)\n"
			"flw.ps f12, 192(x14)\n"
			"flt.ps f20, f2, f12\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 192(x5)\n"
			"flw.ps f13, 32(x5)\n"
			"fmin.pi f4, f20, f13\n" 
			"li x14, 0x81c298605758a43b\n"
			"fcvt.s.w f6, x14, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f13, 512(x28)\n"
			"flw.ps f1, 32(x28)\n"
			"fsub.pi f17, f13, f1\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 192(x7)\n"
			"flw.ps f20, 288(x7)\n"
			"for.pi f2, f21, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 736(x17)\n"
			"flw.ps f16, 32(x17)\n"
			"fmax.s f16, f21, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 576(x7)\n"
			"fmul.pi f21, f21, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 288(x14)\n"
			"fcvt.lu.s x25, f19, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f6, 480(x16)\n"
			"flw.ps f17, 64(x16)\n"
			"fmaxu.pi f9, f6, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"mov.m.x m1, x14, 0x46\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 864(x7)\n"
			"flw.ps f16, 224(x7)\n"
			"fsgnjx.ps f9, f27, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 352(x26)\n"
			"flw.ps f13, 448(x26)\n"
			"fle.ps f13, f20, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 896(x25)\n"
			"fsatu8.pi f1, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 192(x5)\n"
			"flw.ps f21, 320(x5)\n"
			"fmin.ps f13, f4, f21\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 928(x14)\n"
			"flw.ps f1, 544(x14)\n"
			"fmulhu.pi f20, f20, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 64(x16)\n"
			"flw.ps f2, 384(x16)\n"
			"fmin.s f6, f9, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 992(x15)\n"
			"flw.ps f19, 960(x15)\n"
			"fmul.s f27, f27, f19, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 448(x16)\n"
			"fcvt.wu.s x17, f17, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 608(x27)\n"
			"flw.ps f15, 256(x27)\n"
			"fsgnjx.ps f9, f9, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f9, 448(x16)\n"
			"flw.ps f2, 864(x16)\n"
			"fmax.pi f6, f9, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 416(x25)\n"
			"flw.ps f9, 576(x25)\n"
			"fsgnjx.s f1, f13, f9\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 640(x5)\n"
			"flw.ps f13, 640(x5)\n"
			"frem.pi f9, f12, f13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f21, 480(x28)\n"
			"flw.ps f27, 576(x28)\n"
			"fxor.pi f17, f21, f27\n" 
			"fbci.pi f6, 0x805d\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 384(x25)\n"
			"flw.ps f21, 416(x25)\n"
			"feq.ps f16, f13, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 416(x28)\n"
			"flw.ps f4, 384(x28)\n"
			"fmax.pi f17, f16, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_0:\n"
			"maskor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 96(x14)\n"
			"flw.ps f16, 416(x14)\n"
			"fmadd.s f20, f17, f16, f16, rmm\n" 
			"maskpopcz x25, m6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 704(x25)\n"
			"fle.ps f19, f17, f17\n" 
			"la x25, rand_int_32\n"
			"flw.ps f19, 480(x25)\n"
			"flw.ps f27, 992(x25)\n"
			"fsll.pi f16, f19, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 576(x15)\n"
			"flw.ps f9, 192(x15)\n"
			"fsgnjn.s f6, f26, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x3 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1c << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xb << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 64(x28)\n"
			"flw.ps f12, 544(x28)\n"
			"flt.s x28, f3, f12\n" 
			"la x5, rand_int_32\n"
			"flw.ps f6, 352(x5)\n"
			"flw.ps f19, 704(x5)\n"
			"fmulhu.pi f19, f6, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 768(x15)\n"
			"flw.ps f13, 864(x15)\n"
			"fsgnjn.s f1, f4, f13\n" 
			"li x5, 0xf6ee\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_int_32\n"
			"flw.ps f20, 768(x15)\n"
			"fslli.pi f15, f20, 0x6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f21, 512(x28)\n"
			"flw.ps f20, 96(x28)\n"
			"fsll.pi f20, f21, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 576(x7)\n"
			"flw.ps f3, 160(x7)\n"
			"fmulhu.pi f2, f6, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 864(x27)\n"
			"fswizz.ps f26, f12, 0x0\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 864(x7)\n"
			"flw.ps f17, 768(x7)\n"
			"fmulhu.pi f2, f27, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 192(x5)\n"
			"fcvt.pwu.ps f1, f13, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 0(x26)\n"
			"flw.ps f4, 832(x26)\n"
			"flt.ps f2, f15, f4\n" 
			"li x5, 0xe496dfca37f6307a\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x70ad48c513a8ee3e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f12, 480(x5)\n"
			"flw.ps f1, 640(x5)\n"
			"fsrl.pi f15, f12, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 576(x14)\n"
			"flw.ps f15, 768(x14)\n"
			"fmin.pi f13, f17, f15\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f15, 256(x25)\n"
			"fcvt.ps.f16 f1, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 864(x26)\n"
			"flw.ps f13, 160(x26)\n"
			"fsub.pi f12, f16, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 448(x26)\n"
			"flw.ps f15, 736(x26)\n"
			"fmin.s f27, f16, f15\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 864(x14)\n"
			"flw.ps f26, 224(x14)\n"
			"fremu.pi f9, f12, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 608(x15)\n"
			"flw.ps f27, 928(x15)\n"
			"fadd.s f13, f2, f27, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f19, 192(x26)\n"
			"flw.ps f3, 864(x26)\n"
			"flt.pi f15, f19, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 640(x26)\n"
			"flw.ps f6, 800(x26)\n"
			"flt.ps f19, f12, f6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 960(x14)\n"
			"fnot.pi f4, f19\n" 
			"fbci.ps f9, 0x2919c\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 256(x27)\n"
			"flw.ps f21, 704(x27)\n"
			"fdiv.ps f15, f2, f21, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_int_32\n"
			"flw.ps f27, 640(x14)\n"
			"flw.ps f20, 352(x14)\n"
			"fsll.pi f6, f27, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 0(x17)\n"
			"fcvt.pw.ps f1, f6, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 864(x5)\n"
			"flw.ps f20, 896(x5)\n"
			"fmax.ps f26, f9, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 800(x26)\n"
			"fpackrepb.pi f3, f2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 352(x5)\n"
			"flw.ps f6, 832(x5)\n"
			"fsgnjn.s f26, f2, f6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f21, 704(x26)\n"
			"flw.ps f27, 320(x26)\n"
			"fxor.pi f3, f21, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 192(x27)\n"
			"flw.ps f17, 320(x27)\n"
			"flw.ps f27, 704(x27)\n"
			"fnmsub.ps f26, f9, f17, f27, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 608(x17)\n"
			"flw.ps f27, 480(x17)\n"
			"fsub.pi f26, f13, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 832(x5)\n"
			"flw.ps f12, 928(x5)\n"
			"flw.ps f21, 128(x5)\n"
			"fmadd.ps f21, f4, f12, f21, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 576(x27)\n"
			"flw.ps f3, 704(x27)\n"
			"flw.ps f4, 448(x27)\n"
			"fcmov.ps f13, f27, f3, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_5_M0_WRITE_ID_4_HID_0:\n"
			"fltm.ps m5, f13, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_FP_TRANS_ID_5_HID_0:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 608(x25)\n"
			"fexp.ps f2, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 576(x14)\n"
			"flw.ps f26, 0(x14)\n"
			"flw.ps f20, 512(x14)\n"
			"fnmsub.s f3, f4, f26, f20, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 576(x28)\n"
			"ffrc.ps f15, f15\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 896(x15)\n"
			"fnot.pi f1, f21\n" 
			"li x27, 0x8d0786c365d933b\n"
			"fcvt.s.w f15, x27, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 320(x26)\n"
			"fpackreph.pi f6, f1\n" 
			"LBL_SEQID_5_FP_TRANS_ID_6_HID_0:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 384(x16)\n"
			"fexp.ps f15, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 640(x16)\n"
			"flw.ps f19, 320(x16)\n"
			"fmax.pi f27, f4, f19\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 320(x26)\n"
			"fpackrepb.pi f3, f12\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 736(x26)\n"
			"fsrl.pi f6, f9, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 544(x7)\n"
			"flw.ps f17, 480(x7)\n"
			"flt.pi f4, f21, f17\n" 
			"li x5, 0x9c4e91478f55faae\n"
			"fcvt.s.lu f2, x5, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 192(x17)\n"
			"flw.ps f16, 832(x17)\n"
			"fsll.pi f20, f1, f16\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 544(x27)\n"
			"fslli.pi f16, f4, 0x2\n" 
			"LBL_SEQID_5_FP_TRANS_ID_7_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 640(x27)\n"
			"frcp.ps f6, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_477132_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"LBL_HPM_3_CORE_TS_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
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
	if ( hid == 1 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fsetm.pi m7, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 480(x13)\n"
			"flw.ps f20, 96(x13)\n"
			"flw.ps f9, 480(x13)\n"
			"fmadd.s f20, f18, f20, f9, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 832(x6)\n"
			"flw.ps f13, 704(x6)\n"
			"fmin.s f20, f18, f13\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fltm.pi m7, f24, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd165d40b4f\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 704(x25)\n"
			"fswizz.ps f27, f20, 0x5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 736(x13)\n"
			"flw.ps f25, 192(x13)\n"
			"fsub.s f28, f28, f25, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 640(x14)\n"
			"flw.ps f24, 704(x14)\n"
			"flw.ps f29, 960(x14)\n"
			"fmsub.ps f10, f18, f24, f29, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"fltm.pi m7, f20, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f8, 992(x25)\n"
			"flw.ps f22, 192(x25)\n"
			"fxor.pi f29, f8, f22\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 640(x6)\n"
			"fpackrepb.pi f23, f27\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_1:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 864(x17)\n"
			"flog.ps f28, f9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 320(x12)\n"
			"flw.ps f21, 608(x12)\n"
			"fand.pi f20, f18, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_1:\n"
			"maskor m7, m3, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f13, 128(x6)\n"
			"flw.ps f4, 224(x6)\n"
			"fsra.pi f25, f13, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 384(x14)\n"
			"flw.ps f21, 160(x14)\n"
			"fmaxu.pi f18, f24, f21\n" 
			"la x22, rand_int_32\n"
			"flw.ps f23, 96(x22)\n"
			"flw.ps f18, 0(x22)\n"
			"fltu.pi f9, f23, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 736(x6)\n"
			"flw.ps f13, 704(x6)\n"
			"fcmovm.ps f13, f27, f13\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 160(x12)\n"
			"flw.ps f21, 960(x12)\n"
			"flw.ps f28, 672(x12)\n"
			"fnmsub.ps f23, f15, f21, f28, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 288(x17)\n"
			"flw.ps f13, 608(x17)\n"
			"fle.s x6, f15, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 736(x14)\n"
			"flw.ps f15, 416(x14)\n"
			"feq.s x12, f24, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 992(x16)\n"
			"frsq.ps f24, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 864(x13)\n"
			"flw.ps f25, 192(x13)\n"
			"fsgnj.ps f9, f22, f25\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 384(x25)\n"
			"flw.ps f22, 896(x25)\n"
			"fdiv.pi f15, f9, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 288(x16)\n"
			"fsrli.pi f28, f25, 0xa\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 160(x6)\n"
			"flw.ps f27, 928(x6)\n"
			"flw.ps f29, 320(x6)\n"
			"fmsub.ps f15, f15, f27, f29, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 992(x6)\n"
			"flw.ps f22, 0(x6)\n"
			"fmin.s f22, f15, f22\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 224(x22)\n"
			"flw.ps f20, 864(x22)\n"
			"fxor.pi f27, f8, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 224(x25)\n"
			"flw.ps f15, 256(x25)\n"
			"fsgnj.ps f27, f27, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 128(x10)\n"
			"flw.ps f10, 416(x10)\n"
			"for.pi f20, f23, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 608(x14)\n"
			"flw.ps f15, 928(x14)\n"
			"fsub.s f15, f21, f15, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_1:\n"
			"mov.m.x m0, x10, 0xa\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc8271e3682\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 384(x17)\n"
			"flw.ps f13, 352(x17)\n"
			"fsgnj.s f23, f21, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 416(x13)\n"
			"flw.ps f27, 960(x13)\n"
			"fmul.ps f8, f10, f27, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 192(x10)\n"
			"fcvt.pwu.ps f10, f29, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 288(x14)\n"
			"fsatu8.pi f29, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 0(x16)\n"
			"flw.ps f29, 672(x16)\n"
			"feq.s x12, f28, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 864(x10)\n"
			"faddi.pi f23, f29, 0x10a\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 192(x6)\n"
			"fslli.pi f13, f8, 0x3\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 576(x13)\n"
			"fcvt.ps.pwu f13, f24, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 352(x25)\n"
			"flw.ps f13, 352(x25)\n"
			"fsra.pi f27, f27, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 992(x6)\n"
			"flw.ps f15, 864(x6)\n"
			"fminu.pi f28, f8, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 928(x12)\n"
			"fcvt.f16.ps f23, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 512(x14)\n"
			"flw.ps f9, 96(x14)\n"
			"fsll.pi f15, f8, f9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 928(x12)\n"
			"fandi.pi f25, f27, 0x70\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 128(x10)\n"
			"flw.ps f15, 704(x10)\n"
			"fmulhu.pi f15, f4, f15\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_1:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 704(x12)\n"
			"flog.ps f13, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 480(x13)\n"
			"fcvt.ps.pw f25, f4, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 288(x6)\n"
			"flw.ps f27, 480(x6)\n"
			"fmulh.pi f9, f20, f27\n" 
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
			"la x25, rand_int_32\n"
			"flw.ps f29, 96(x25)\n"
			"fmax.pi f27, f29, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 576(x22)\n"
			"fclass.s x12, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 672(x17)\n"
			"flw.ps f23, 576(x17)\n"
			"fadd.ps f13, f29, f23, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 576(x14)\n"
			"fpackrepb.pi f28, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 0(x25)\n"
			"ffrc.ps f20, f8\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 224(x22)\n"
			"flw.ps f28, 96(x22)\n"
			"fminu.pi f24, f25, f28\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 608(x25)\n"
			"flw.ps f24, 192(x25)\n"
			"feq.pi f22, f27, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 288(x6)\n"
			"flw.ps f13, 320(x6)\n"
			"flt.s x16, f24, f13\n" 
			"maskpopcz x10, m7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 672(x12)\n"
			"fclass.s x26, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 64(x16)\n"
			"flw.ps f22, 640(x16)\n"
			"fsgnjn.s f25, f28, f22\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 96(x16)\n"
			"ffrc.ps f21, f27\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_1:\n"
			"masknot m0, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f23, 128(x16)\n"
			"fdiv.pi f27, f23, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 352(x17)\n"
			"flw.ps f28, 768(x17)\n"
			"flw.ps f4, 928(x17)\n"
			"fnmadd.ps f20, f22, f28, f4, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 864(x6)\n"
			"flw.ps f23, 96(x6)\n"
			"fmul.ps f15, f13, f23, rup\n" 
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
			"la x26, rand_int_32\n"
			"flw.ps f22, 512(x26)\n"
			"flw.ps f29, 128(x26)\n"
			"fmulh.pi f10, f22, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 768(x26)\n"
			"flw.ps f4, 384(x26)\n"
			"flw.ps f29, 160(x26)\n"
			"fmsub.ps f29, f25, f4, f29, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 480(x10)\n"
			"flw.ps f18, 512(x10)\n"
			"for.pi f4, f27, f18\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_1:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 320(x22)\n"
			"flog.ps f24, f24\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 640(x25)\n"
			"flw.ps f28, 992(x25)\n"
			"for.pi f24, f20, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 544(x26)\n"
			"flw.ps f10, 32(x26)\n"
			"flt.ps f9, f22, f10\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 640(x14)\n"
			"fmax.pi f13, f22, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 544(x6)\n"
			"fclass.s x17, f15\n" 
			"la x16, rand_int_32\n"
			"flw.ps f23, 0(x16)\n"
			"flw.ps f13, 352(x16)\n"
			"fxor.pi f20, f23, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 576(x22)\n"
			"flw.ps f22, 992(x22)\n"
			"fmul.s f15, f27, f22, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 0(x14)\n"
			"flw.ps f27, 736(x14)\n"
			"fsgnj.s f29, f29, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 960(x14)\n"
			"flw.ps f4, 480(x14)\n"
			"fadd.pi f29, f9, f4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 928(x13)\n"
			"flw.ps f8, 128(x13)\n"
			"for.pi f29, f10, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 416(x26)\n"
			"flw.ps f4, 256(x26)\n"
			"fadd.ps f9, f28, f4, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 416(x10)\n"
			"fsrai.pi f4, f8, 0xe\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 384(x16)\n"
			"flw.ps f25, 224(x16)\n"
			"fmulhu.pi f24, f27, f25\n" 
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
			"la x25, rand_int_32\n"
			"flw.ps f23, 576(x25)\n"
			"fcvt.ps.pw f21, f23, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 608(x14)\n"
			"flw.ps f9, 480(x14)\n"
			"fmaxu.pi f8, f8, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 320(x12)\n"
			"flw.ps f9, 768(x12)\n"
			"fadd.s f21, f22, f9, rtz\n" 
			"maskpopc x16, m7\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_1:\n"
			"maskor m3, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 224(x25)\n"
			"fsin.ps f27, f22\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_1:\n"
			"mov.m.x m2, x16, 0x51\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 32(x26)\n"
			"flw.ps f25, 832(x26)\n"
			"flt.pi f15, f27, f25\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 256(x6)\n"
			"fsqrt.s f9, f25, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 640(x6)\n"
			"flw.ps f4, 896(x6)\n"
			"fxor.pi f10, f24, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 512(x12)\n"
			"flw.ps f4, 32(x12)\n"
			"fsll.pi f10, f28, f4\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 832(x16)\n"
			"flw.ps f4, 672(x16)\n"
			"fsgnjn.ps f22, f22, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 480(x22)\n"
			"fsqrt.ps f13, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 640(x13)\n"
			"flw.ps f20, 768(x13)\n"
			"fle.ps f9, f27, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 224(x17)\n"
			"fcvt.pwu.ps f8, f23, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 704(x25)\n"
			"flw.ps f10, 224(x25)\n"
			"flw.ps f20, 992(x25)\n"
			"fmadd.s f28, f21, f10, f20, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 2 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f25,  736 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskand m2, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f6, 768(x18)\n"
			"flw.ps f11, 192(x18)\n"
			"fsra.pi f14, f6, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 736(x11)\n"
			"fcvt.w.s x5, f12, rtz\n" 
			"li x5, 0xcf04\n"
			"csrw tensor_mask, x5\n"	
			"li x13, 0xdb7331a2a78425c6\n"
			"fcvt.s.wu f15, x13, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 480(x18)\n"
			"flw.ps f20, 352(x18)\n"
			"fxor.pi f14, f14, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 832(x28)\n"
			"flw.ps f23, 352(x28)\n"
			"fxor.pi f16, f9, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"flem.ps m2, f15, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f21, 0x13356\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 416(x28)\n"
			"fcvt.pw.ps f21, f13, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 160(x25)\n"
			"fsrli.pi f16, f9, 0xd\n" 
			"la x5, rand_int_32\n"
			"flw.ps f6, 896(x5)\n"
			"fcvt.ps.pwu f6, f6, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 928(x18)\n"
			"flw.ps f14, 224(x18)\n"
			"flt.s x13, f12, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 704(x22)\n"
			"fslli.pi f6, f9, 0x6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 640(x20)\n"
			"flw.ps f31, 320(x20)\n"
			"flw.ps f21, 768(x20)\n"
			"fnmsub.s f16, f6, f31, f21, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 288(x7)\n"
			"flw.ps f7, 0(x7)\n"
			"fle.s x25, f20, f7\n" 
			"li x5, 0xf11e\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_2:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x2 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xb << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1b << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x1 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f6, 448(x11)\n"
			"flw.ps f14, 992(x11)\n"
			"fdivu.pi f21, f6, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 576(x7)\n"
			"flw.ps f16, 64(x7)\n"
			"feq.ps f21, f7, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"maskor m3, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f23, 608(x6)\n"
			"flw.ps f20, 992(x6)\n"
			"fle.pi f8, f23, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 160(x22)\n"
			"fcvt.w.s x13, f9, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 928(x13)\n"
			"flw.ps f19, 32(x13)\n"
			"fadd.s f7, f16, f19, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 928(x6)\n"
			"flw.ps f14, 704(x6)\n"
			"fle.ps f16, f15, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 960(x18)\n"
			"fsrai.pi f6, f8, 0x8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 832(x20)\n"
			"flw.ps f14, 960(x20)\n"
			"flw.ps f20, 544(x20)\n"
			"fmsub.ps f28, f11, f14, f20, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 992(x25)\n"
			"fsin.ps f7, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 384(x5)\n"
			"fswizz.ps f16, f15, 0x4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 384(x13)\n"
			"fsatu8.pi f31, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 512(x6)\n"
			"flw.ps f7, 256(x6)\n"
			"fdiv.s f28, f28, f7, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f20, 288(x11)\n"
			"flw.ps f11, 64(x11)\n"
			"fremu.pi f19, f20, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"maskxor m3, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 992(x25)\n"
			"flw.ps f31, 448(x25)\n"
			"flw.ps f21, 160(x25)\n"
			"fnmsub.s f15, f15, f31, f21, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xb16b\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x9d << TENSOR_FMA16_SCP_LOC_B) | (0x3c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x68 << TENSOR_FMA16_SCP_LOC_B) | (0xba << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f6, 736(x11)\n"
			"fcvt.ps.pw f13, f6, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_2:\n"
			"feqm.ps m3, f15, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc405e6f0e6\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x7\n" 
			"li x5, 0x3457\n"
			"csrw tensor_mask, x5\n"	
			"li x10, 0x9d732305b40412fd\n"
			"fcvt.s.w f8, x10, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 288(x13)\n"
			"fsrli.pi f16, f28, 0x6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 576(x10)\n"
			"flw.ps f15, 288(x10)\n"
			"flt.s x5, f19, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 288(x7)\n"
			"flw.ps f31, 32(x7)\n"
			"fadd.s f7, f15, f31, rmm\n" 
			"fbci.ps f9, 0x55f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 288(x10)\n"
			"flw.ps f6, 960(x10)\n"
			"fle.ps f12, f12, f6\n" 
			"li x7, 0xd0f03ae3d39dc3a0\n"
			"fcvt.s.wu f23, x7, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_2:\n"
			"fltm.pi m2, f8, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 64(x6)\n"
			"flw.ps f19, 64(x6)\n"
			"fsub.s f7, f6, f19, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 192(x10)\n"
			"fsrli.pi f12, f12, 0x8\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 0(x7)\n"
			"fnot.pi f15, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 960(x6)\n"
			"fcvt.lu.s x22, f14, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x65 << TENSOR_FMA32_SCP_LOC_B) | (0xeb << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x72 << TENSOR_FMA32_SCP_LOC_B) | (0xab << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"flw.ps f11, 96(x5)\n"
			"fcvt.wu.s x25, f11, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 416(x13)\n"
			"flw.ps f13, 0(x13)\n"
			"fsgnjx.s f12, f16, f13\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_2:\n"
			"maskand m2, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f20, 352(x25)\n"
			"flw.ps f7, 512(x25)\n"
			"feq.pi f14, f20, f7\n" 
			"la x20, rand_int_32\n"
			"flw.ps f28, 992(x20)\n"
			"fnot.pi f28, f28\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 256(x25)\n"
			"flw.ps f11, 832(x25)\n"
			"fmulh.pi f23, f14, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 352(x20)\n"
			"flw.ps f19, 704(x20)\n"
			"flw.ps f15, 128(x20)\n"
			"fmsub.s f6, f8, f19, f15, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f28, 448(x6)\n"
			"fsrli.pi f19, f28, 0x4\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 192(x25)\n"
			"flw.ps f13, 608(x25)\n"
			"fmin.s f12, f23, f13\n" 
			"li x5, 0x40c0804f5b60aa36\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x583078be8bb30158\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 128(x20)\n"
			"flw.ps f15, 960(x20)\n"
			"fsgnjn.s f19, f19, f15\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 32(x7)\n"
			"flw.ps f13, 704(x7)\n"
			"fle.pi f16, f9, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 448(x10)\n"
			"flw.ps f23, 832(x10)\n"
			"flw.ps f15, 192(x10)\n"
			"fmsub.ps f9, f6, f23, f15, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 96(x10)\n"
			"flw.ps f15, 960(x10)\n"
			"fle.ps f23, f14, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 32(x6)\n"
			"fslli.pi f23, f6, 0x9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 736(x10)\n"
			"flw.ps f14, 992(x10)\n"
			"fadd.pi f12, f11, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 448(x28)\n"
			"fround.ps f6, f19, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 480(x25)\n"
			"flw.ps f21, 896(x25)\n"
			"flw.ps f8, 928(x25)\n"
			"fmsub.s f16, f11, f21, f8, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 0(x28)\n"
			"flw.ps f21, 128(x28)\n"
			"flw.ps f14, 832(x28)\n"
			"fnmsub.s f12, f23, f21, f14, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 896(x5)\n"
			"fsrli.pi f9, f23, 0x7\n" 
			"maskpopc x20, m1\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 672(x28)\n"
			"flw.ps f7, 96(x28)\n"
			"fmulhu.pi f20, f23, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 896(x28)\n"
			"fround.ps f15, f14, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 896(x20)\n"
			"flw.ps f9, 320(x20)\n"
			"fsub.s f9, f8, f9, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 192(x5)\n"
			"fcvt.lu.s x22, f13, rmm\n" 
			"li x5, 0x56a0\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 800(x22)\n"
			"fclass.ps f31, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 640(x28)\n"
			"fsrli.pi f20, f14, 0x0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 704(x28)\n"
			"fadd.pi f11, f13, f13\n" 
			"li x22, 0xd4ccb7fa2b8f0d4f\n"
			"fcvt.s.wu f12, x22, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 448(x13)\n"
			"fcvt.l.s x10, f8, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 832(x10)\n"
			"flw.ps f14, 768(x10)\n"
			"flw.ps f7, 960(x10)\n"
			"fmadd.s f20, f13, f14, f7, rne\n" 
			"li x5, 0x72759193796a5482\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3cdd3ace9f61619e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 0(x10)\n"
			"flw.ps f19, 64(x10)\n"
			"flw.ps f11, 160(x10)\n"
			"fnmadd.ps f23, f13, f19, f11, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_2:\n"
			"masknot m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7f << TENSOR_IMA_SCP_LOC_B) | (0x9e << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_2:\n"
			"fltm.pi m4, f8, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 160(x6)\n"
			"flw.ps f20, 640(x6)\n"
			"flw.ps f16, 544(x6)\n"
			"fnmadd.s f15, f15, f20, f16, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 448(x5)\n"
			"fsqrt.ps f13, f13\n" 
			"li x22, 0xf71cd0b7d7604afd\n"
			"fcvt.s.wu f16, x22, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 832(x5)\n"
			"fsat8.pi f28, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 672(x5)\n"
			"flw.ps f20, 928(x5)\n"
			"fmin.ps f28, f16, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 736(x10)\n"
			"flw.ps f28, 896(x10)\n"
			"flt.ps f7, f8, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 416(x10)\n"
			"flw.ps f6, 0(x10)\n"
			"fmulhu.pi f14, f11, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 704(x22)\n"
			"flw.ps f16, 800(x22)\n"
			"fsgnj.s f28, f13, f16\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_2:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 288(x18)\n"
			"flog.ps f14, f14\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 608(x20)\n"
			"flw.ps f7, 320(x20)\n"
			"fle.ps f23, f12, f7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 416(x10)\n"
			"flw.ps f7, 288(x10)\n"
			"flw.ps f31, 416(x10)\n"
			"fnmsub.ps f12, f15, f7, f31, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 640(x22)\n"
			"fcvt.l.s x13, f23, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 992(x22)\n"
			"fsin.ps f20, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 896(x18)\n"
			"flw.ps f19, 512(x18)\n"
			"fminu.pi f16, f28, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 800(x10)\n"
			"flw.ps f13, 928(x10)\n"
			"for.pi f15, f23, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 3 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fltm.ps m4, f31, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 512(x12)\n"
			"flw.ps f18, 704(x12)\n"
			"flw.ps f5, 32(x12)\n"
			"fmsub.s f2, f6, f18, f5, rtz\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f10, 448(x6)\n"
			"fcvt.ps.f16 f6, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f25, 928(x6)\n"
			"fsatu8.pi f10, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 768(x19)\n"
			"flw.ps f15, 896(x19)\n"
			"fmin.s f31, f25, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 960(x7)\n"
			"flw.ps f31, 640(x7)\n"
			"fsgnjx.ps f13, f16, f31\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 64(x26)\n"
			"flw.ps f15, 960(x26)\n"
			"frem.pi f5, f8, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 256(x20)\n"
			"flw.ps f2, 160(x20)\n"
			"fle.s x26, f16, f2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 384(x19)\n"
			"flw.ps f15, 128(x19)\n"
			"fsgnjx.s f21, f21, f15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 448(x15)\n"
			"fclass.s x7, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 480(x26)\n"
			"fmulhu.pi f18, f16, f16\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_3:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 544(x26)\n"
			"fexp.ps f8, f22\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 608(x15)\n"
			"fround.ps f6, f23, rdn\n" 
			"mova.x.m x19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 512(x12)\n"
			"flw.ps f16, 704(x12)\n"
			"flt.s x20, f23, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_3:\n"
			"fltm.pi m0, f5, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 480(x26)\n"
			"fsin.ps f5, f9\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f31, 352(x7)\n"
			"fcvt.ps.f16 f16, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 832(x17)\n"
			"flw.ps f15, 192(x17)\n"
			"fmul.s f10, f5, f15, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 128(x19)\n"
			"fslli.pi f25, f23, 0xe\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 832(x12)\n"
			"flw.ps f31, 928(x12)\n"
			"fadd.s f5, f2, f31, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 704(x17)\n"
			"flw.ps f31, 128(x17)\n"
			"fadd.pi f22, f25, f31\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f3, 96(x17)\n"
			"fcvt.ps.f16 f6, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 608(x7)\n"
			"flw.ps f2, 800(x7)\n"
			"flw.ps f18, 384(x7)\n"
			"fnmsub.s f23, f9, f2, f18, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 320(x6)\n"
			"frsq.ps f18, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 768(x26)\n"
			"flw.ps f16, 800(x26)\n"
			"fcmovm.ps f22, f31, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 672(x26)\n"
			"flw.ps f21, 736(x26)\n"
			"flt.ps f23, f31, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 512(x14)\n"
			"flw.ps f25, 896(x14)\n"
			"fmax.ps f23, f10, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 192(x15)\n"
			"fclass.ps f2, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 192(x26)\n"
			"flw.ps f31, 352(x26)\n"
			"flt.s x6, f22, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"maskxor m4, m4, m7\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 288(x17)\n"
			"flw.ps f23, 96(x17)\n"
			"flw.ps f18, 160(x17)\n"
			"fnmsub.s f3, f10, f23, f18, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 544(x7)\n"
			"fswizz.ps f6, f2, 0x1c\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 768(x9)\n"
			"fcvt.ps.pwu f21, f5, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 864(x15)\n"
			"flw.ps f16, 512(x15)\n"
			"flt.ps f16, f2, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_3:\n"
			"maskand m4, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 640(x7)\n"
			"fcvt.f16.ps f8, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 320(x15)\n"
			"fcvt.ps.pwu f23, f23, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 544(x17)\n"
			"fround.ps f9, f21, rtz\n" 
			"la x12, rand_ieee754_16\n"
			"flw.ps f9, 896(x12)\n"
			"fcvt.ps.f16 f9, f9\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f15, 672(x19)\n"
			"fcvt.ps.f16 f21, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 160(x12)\n"
			"fcvt.w.s x17, f5, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 256(x14)\n"
			"flw.ps f15, 768(x14)\n"
			"flw.ps f2, 224(x14)\n"
			"fmsub.s f10, f5, f15, f2, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 64(x26)\n"
			"flw.ps f6, 448(x26)\n"
			"fmul.s f31, f21, f6, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 192(x9)\n"
			"flw.ps f2, 288(x9)\n"
			"fmax.pi f6, f31, f2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 960(x19)\n"
			"flw.ps f31, 256(x19)\n"
			"fle.pi f15, f3, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 128(x9)\n"
			"flt.pi f15, f2, f2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 512(x12)\n"
			"flw.ps f3, 64(x12)\n"
			"fmadd.s f18, f5, f3, f5, rdn\n" 
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
			"la x6, rand_int_32\n"
			"flw.ps f21, 800(x6)\n"
			"flw.ps f23, 704(x6)\n"
			"fle.pi f31, f21, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 832(x19)\n"
			"ffrc.ps f25, f13\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 256(x17)\n"
			"fmulhu.pi f31, f22, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 32(x14)\n"
			"flw.ps f16, 224(x14)\n"
			"flw.ps f23, 544(x14)\n"
			"fnmadd.s f8, f31, f16, f23, rtz\n" 
			"fbci.pi f8, 0x130d\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 96(x20)\n"
			"fpackrepb.pi f3, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 960(x7)\n"
			"fmin.s f16, f31, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 384(x15)\n"
			"flw.ps f25, 576(x15)\n"
			"fsll.pi f22, f21, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 480(x26)\n"
			"fcvt.pwu.ps f16, f9, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 736(x26)\n"
			"flw.ps f2, 224(x26)\n"
			"fmul.s f3, f21, f2, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 320(x12)\n"
			"frsq.ps f2, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 96(x26)\n"
			"fsqrt.s f3, f23, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 128(x17)\n"
			"flw.ps f10, 256(x17)\n"
			"fle.pi f6, f6, f10\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f16, 768(x19)\n"
			"fcvt.ps.f16 f8, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 736(x9)\n"
			"flw.ps f31, 608(x9)\n"
			"fadd.ps f9, f5, f31, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f21, 224(x26)\n"
			"flw.ps f25, 0(x26)\n"
			"fminu.pi f9, f21, f25\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_3:\n"
			"feqm.ps m4, f23, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 448(x17)\n"
			"flw.ps f18, 192(x17)\n"
			"flw.ps f6, 192(x17)\n"
			"fmsub.s f3, f21, f18, f6, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 288(x14)\n"
			"flw.ps f8, 320(x14)\n"
			"fxor.pi f21, f31, f8\n" 
			"mova.x.m x12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 64(x19)\n"
			"fround.ps f22, f6, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_3:\n"
			"fltm.pi m0, f9, f16\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 512(x19)\n"
			"fcvt.pw.ps f16, f2, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 256(x6)\n"
			"flw.ps f16, 416(x6)\n"
			"feq.ps f15, f18, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 64(x19)\n"
			"flw.ps f8, 448(x19)\n"
			"fle.ps f6, f10, f8\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_3:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 992(x6)\n"
			"flog.ps f2, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 480(x20)\n"
			"flw.ps f31, 416(x20)\n"
			"fadd.pi f18, f6, f31\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_3:\n"
			"maskand m7, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 864(x15)\n"
			"flw.ps f18, 960(x15)\n"
			"flt.s x14, f16, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 896(x15)\n"
			"fsub.pi f9, f15, f15\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_3:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 160(x15)\n"
			"fexp.ps f25, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 32(x15)\n"
			"fsin.ps f5, f3\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 736(x15)\n"
			"fround.ps f9, f2, rne\n" 
			"li x6, 0x203bc7cf13dec6ff\n"
			"fcvt.s.w f10, x6, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 192(x19)\n"
			"flw.ps f9, 736(x19)\n"
			"for.pi f21, f8, f9\n" 
			"maskpopc x20, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_3:\n"
			"maskand m0, m3, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_3:\n"
			"flem.ps m0, f5, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 288(x7)\n"
			"fcvt.pwu.ps f10, f13, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 320(x12)\n"
			"flw.ps f9, 384(x12)\n"
			"fle.s x9, f15, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 320(x26)\n"
			"flw.ps f23, 736(x26)\n"
			"feq.pi f16, f25, f23\n" 
			"li x12, 0x49d7db76e66a97bc\n"
			"fcvt.s.wu f13, x12, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 960(x7)\n"
			"fsat8.pi f23, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 416(x20)\n"
			"flw.ps f25, 0(x20)\n"
			"flw.ps f15, 992(x20)\n"
			"fnmadd.s f8, f9, f25, f15, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 640(x14)\n"
			"fmul.ps f21, f5, f5, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 128(x12)\n"
			"fpackreph.pi f2, f21\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 512(x7)\n"
			"flw.ps f15, 416(x7)\n"
			"fadd.ps f25, f9, f15, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 384(x7)\n"
			"ffrc.ps f10, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 4 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f20, 384(x17)\n"
			"flw.ps f18, 736(x17)\n"
			"fminu.pi f12, f20, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskand m6, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 896(x17)\n"
			"fclass.ps f24, f12\n" 
			"fbci.ps f11, 0x4932c\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"maskor m6, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f10, 512(x15)\n"
			"fandi.pi f21, f10, 0x1e7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 928(x15)\n"
			"flw.ps f8, 576(x15)\n"
			"fsgnjn.ps f21, f12, f8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f7, 832(x17)\n"
			"fslli.pi f12, f7, 0x9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 320(x26)\n"
			"fmin.pi f12, f24, f24\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 544(x16)\n"
			"flw.ps f20, 608(x16)\n"
			"fltu.pi f4, f8, f20\n" 
			"la x16, rand_int_32\n"
			"flw.ps f19, 576(x16)\n"
			"flw.ps f7, 512(x16)\n"
			"fmul.pi f8, f19, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 800(x16)\n"
			"flw.ps f19, 416(x16)\n"
			"flw.ps f24, 640(x16)\n"
			"fcmov.ps f11, f4, f19, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 384(x22)\n"
			"flw.ps f10, 0(x22)\n"
			"fmul.pi f7, f31, f10\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 384(x13)\n"
			"fsrli.pi f19, f19, 0xa\n" 
			"la x20, rand_int_32\n"
			"flw.ps f12, 224(x20)\n"
			"flw.ps f20, 352(x20)\n"
			"fle.pi f12, f12, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 928(x25)\n"
			"fround.ps f8, f28, rmm\n" 
			"li x5, 0x2dc0\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"li x5, 0xb15\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_939788_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 608(x20)\n"
			"fsin.ps f0, f10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 128(x25)\n"
			"fslli.pi f11, f1, 0xd\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 800(x16)\n"
			"flw.ps f19, 928(x16)\n"
			"fmax.s f18, f0, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 992(x15)\n"
			"flw.ps f21, 288(x15)\n"
			"for.pi f11, f18, f21\n" 
			"la x25, rand_int_32\n"
			"flw.ps f24, 832(x25)\n"
			"flw.ps f21, 480(x25)\n"
			"fremu.pi f18, f24, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 960(x17)\n"
			"flw.ps f11, 352(x17)\n"
			"flw.ps f19, 704(x17)\n"
			"fmsub.ps f1, f7, f11, f19, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 64(x25)\n"
			"fandi.pi f7, f10, 0x41\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f0, 96(x16)\n"
			"flw.ps f20, 32(x16)\n"
			"fand.pi f18, f0, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 960(x16)\n"
			"fcvt.wu.s x14, f0, rne\n" 
			"li x5, 0x9e63\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 192(x17)\n"
			"flw.ps f8, 960(x17)\n"
			"fsgnjn.s f8, f21, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 864(x14)\n"
			"flw.ps f24, 736(x14)\n"
			"flw.ps f8, 320(x14)\n"
			"fcmov.ps f8, f12, f24, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 224(x14)\n"
			"flw.ps f4, 0(x14)\n"
			"fadd.ps f11, f28, f4, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 640(x13)\n"
			"fswizz.ps f31, f4, 0x6\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 352(x7)\n"
			"fcvt.ps.pw f24, f6, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f7, 640(x17)\n"
			"fandi.pi f12, f7, 0x1ca\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 960(x26)\n"
			"flw.ps f28, 0(x26)\n"
			"flt.ps f19, f31, f28\n" 
			"li x5, 0xb21079369ca62fbd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6e850ef19ab10119\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb2 << TENSOR_IMA_SCP_LOC_B) | (0x3f << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 160(x23)\n"
			"fcvt.w.s x25, f7, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 224(x23)\n"
			"flw.ps f12, 704(x23)\n"
			"fsgnjn.ps f19, f11, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 384(x7)\n"
			"flw.ps f18, 896(x7)\n"
			"fsgnjx.ps f7, f8, f18\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 352(x26)\n"
			"flw.ps f28, 288(x26)\n"
			"flt.ps f19, f24, f28\n" 
			"fbci.pi f0, 0x7d49\n" 
			"LBL_SEQID_2_FP_TRANS_ID_2_HID_4:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 544(x20)\n"
			"flog.ps f21, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 864(x26)\n"
			"flw.ps f8, 896(x26)\n"
			"fsgnj.s f6, f0, f8\n" 
			"li x23, 0x1f749e4a9daba1f6\n"
			"fcvt.s.w f10, x23, rup\n" 
			"mova.x.m x15\n" 
			"li x5, 0xe413\n"
			"csrw tensor_mask, x5\n"	
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 800(x25)\n"
			"flw.ps f31, 800(x25)\n"
			"fmul.ps f6, f18, f31, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 864(x20)\n"
			"fcvt.ps.pw f4, f8, rdn\n" 
			"fbci.ps f6, 0x25a0d\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_4:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 512(x7)\n"
			"flog.ps f20, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 736(x26)\n"
			"fclass.ps f11, f4\n" 
			"li x26, 0xd3478ab7d8b0c63c\n"
			"fcvt.s.lu f7, x26, rmm\n" 
			"li x5, 0x8e61\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"mov.m.x m1, x14, 0x24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x3 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x3 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1d << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x5 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x4 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1d << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xd << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x13, rand_int_32\n"
			"flw.ps f8, 288(x13)\n"
			"flw.ps f7, 928(x13)\n"
			"fmul.pi f31, f8, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 608(x20)\n"
			"flw.ps f0, 928(x20)\n"
			"fmax.s f28, f12, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 928(x14)\n"
			"fclass.s x13, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 736(x26)\n"
			"flw.ps f1, 640(x26)\n"
			"fsub.s f6, f28, f1, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 544(x25)\n"
			"fcvt.wu.s x22, f10, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_4:\n"
			"fltm.pi m3, f10, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f4, 352(x20)\n"
			"flw.ps f24, 160(x20)\n"
			"fmulhu.pi f6, f4, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 384(x25)\n"
			"flw.ps f20, 544(x25)\n"
			"flw.ps f18, 352(x25)\n"
			"fnmadd.ps f21, f21, f20, f18, rtz\n" 
			"li x5, 0x1e6c791d00b4d6ea\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5685e9c3de68523e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_4:\n"
			"feqm.ps m3, f4, f7\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 352(x14)\n"
			"flw.ps f12, 768(x14)\n"
			"flw.ps f31, 544(x14)\n"
			"fcmov.ps f19, f21, f12, f31\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 416(x13)\n"
			"flw.ps f28, 352(x13)\n"
			"fltu.pi f7, f19, f28\n" 
			"li x26, 0xa24b1cb27503081e\n"
			"fcvt.s.wu f11, x26, rmm\n" 
			"li x5, 0x5c8f3321c192fefd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x49d60770d06d8b28\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f18, 832(x26)\n"
			"fsatu8.pi f0, f18\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f4, 896(x17)\n"
			"fcvt.ps.f16 f7, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 160(x22)\n"
			"fcvt.lu.s x17, f19, rtz\n" 
			"fbci.pi f11, 0x408e7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_819276_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x6 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x5 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1a << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 736(x25)\n"
			"flw.ps f31, 544(x25)\n"
			"flt.s x16, f20, f31\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 320(x16)\n"
			"faddi.pi f24, f6, 0xc5\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 32(x17)\n"
			"flw.ps f4, 448(x17)\n"
			"fmaxu.pi f12, f20, f4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 224(x17)\n"
			"flw.ps f11, 384(x17)\n"
			"fmaxu.pi f28, f20, f11\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_4:\n"
			"flem.ps m7, f31, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f0, 480(x13)\n"
			"fcvt.ps.pw f1, f0, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 512(x15)\n"
			"flw.ps f0, 736(x15)\n"
			"fsra.pi f19, f21, f0\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 928(x7)\n"
			"fsatu8.pi f6, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 896(x16)\n"
			"flw.ps f28, 224(x16)\n"
			"flw.ps f19, 0(x16)\n"
			"fmadd.ps f11, f11, f28, f19, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 96(x17)\n"
			"flw.ps f8, 384(x17)\n"
			"fltu.pi f6, f1, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 288(x20)\n"
			"fpackreph.pi f19, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 736(x13)\n"
			"flw.ps f18, 448(x13)\n"
			"flw.ps f4, 448(x13)\n"
			"fnmadd.s f4, f20, f18, f4, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 928(x25)\n"
			"flw.ps f28, 0(x25)\n"
			"flw.ps f12, 32(x25)\n"
			"fcmov.ps f7, f20, f28, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 448(x25)\n"
			"flw.ps f8, 128(x25)\n"
			"flw.ps f12, 608(x25)\n"
			"fmsub.s f0, f21, f8, f12, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 384(x15)\n"
			"flw.ps f0, 736(x15)\n"
			"fsub.s f1, f7, f0, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 320(x16)\n"
			"flw.ps f0, 480(x16)\n"
			"flw.ps f8, 32(x16)\n"
			"fmadd.ps f11, f28, f0, f8, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x6 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x4 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x9 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x3 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 896(x7)\n"
			"flw.ps f20, 128(x7)\n"
			"flw.ps f28, 224(x7)\n"
			"fmsub.ps f7, f4, f20, f28, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 256(x22)\n"
			"flw.ps f1, 928(x22)\n"
			"fmax.s f6, f7, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 736(x14)\n"
			"fnot.pi f12, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 832(x14)\n"
			"ffrc.ps f11, f24\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 672(x7)\n"
			"flw.ps f4, 672(x7)\n"
			"fmulh.pi f18, f18, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 160(x15)\n"
			"flw.ps f18, 288(x15)\n"
			"fsgnjx.ps f12, f1, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f0, 416(x17)\n"
			"fcvt.ps.pwu f28, f0, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 224(x16)\n"
			"fslli.pi f6, f0, 0xe\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 192(x17)\n"
			"fcvt.pw.ps f8, f12, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 992(x23)\n"
			"flw.ps f24, 544(x23)\n"
			"fmul.s f21, f6, f24, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f6, 896(x13)\n"
			"flw.ps f7, 704(x13)\n"
			"for.pi f1, f6, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 0(x20)\n"
			"flw.ps f11, 192(x20)\n"
			"fsgnjx.s f0, f28, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f8, 224(x7)\n"
			"flw.ps f0, 32(x7)\n"
			"fmulh.pi f19, f8, f0\n" 
			"li x5, 0xe010\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 736(x16)\n"
			"flw.ps f18, 864(x16)\n"
			"fadd.ps f10, f0, f18, rdn\n" 
			"li x5, 0xd86831211c2585f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x56dcf243ff3374de\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 448(x22)\n"
			"flw.ps f19, 480(x22)\n"
			"flw.ps f21, 864(x22)\n"
			"fnmsub.s f20, f8, f19, f21, rtz\n" 
			"li x5, 0x87b4\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 736(x15)\n"
			"flw.ps f20, 192(x15)\n"
			"flw.ps f6, 160(x15)\n"
			"fmadd.s f20, f0, f20, f6, rmm\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xb << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x6 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x9 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1d << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x4 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 5 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 576(x5)\n"
			"flw.ps f28, 288(x5)\n"
			"fmax.ps f28, f14, f28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 64(x6)\n"
			"flw.ps f11, 800(x6)\n"
			"fmax.pi f27, f29, f11\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 736(x25)\n"
			"fcvt.f16.ps f0, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 480(x27)\n"
			"flw.ps f15, 64(x27)\n"
			"fsgnjn.ps f11, f11, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 736(x9)\n"
			"flw.ps f17, 480(x9)\n"
			"fsgnj.ps f14, f29, f17\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 128(x23)\n"
			"flt.s x19, f22, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 928(x14)\n"
			"flw.ps f28, 416(x14)\n"
			"fmin.ps f3, f29, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 128(x27)\n"
			"flw.ps f11, 544(x27)\n"
			"fmin.s f1, f28, f11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"feqm.ps m2, f27, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 192(x15)\n"
			"flw.ps f22, 768(x15)\n"
			"fle.s x28, f27, f22\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 544(x9)\n"
			"fcvt.w.s x6, f18, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 288(x15)\n"
			"flw.ps f14, 800(x15)\n"
			"fsgnj.s f28, f17, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 448(x28)\n"
			"fclass.ps f1, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 64(x25)\n"
			"flw.ps f11, 896(x25)\n"
			"fle.pi f25, f22, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 736(x9)\n"
			"flw.ps f22, 288(x9)\n"
			"fltu.pi f8, f1, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"feqm.ps m1, f15, f29\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xbed857a562\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 960(x28)\n"
			"flw.ps f18, 704(x28)\n"
			"fmsub.s f22, f22, f18, f22, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 64(x25)\n"
			"fcvt.ps.pw f1, f15, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 384(x23)\n"
			"flw.ps f29, 480(x23)\n"
			"fmax.pi f3, f28, f29\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 640(x17)\n"
			"flw.ps f18, 96(x17)\n"
			"fsgnjn.s f29, f15, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 384(x17)\n"
			"fcvt.ps.pwu f15, f25, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 192(x17)\n"
			"fcvt.wu.s x5, f25, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 896(x15)\n"
			"flw.ps f14, 960(x15)\n"
			"flt.s x15, f22, f14\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 672(x27)\n"
			"fcvt.ps.pwu f14, f17, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 960(x6)\n"
			"ffrc.ps f22, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 32(x9)\n"
			"flw.ps f14, 608(x9)\n"
			"fsgnjn.ps f17, f1, f14\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 128(x17)\n"
			"fandi.pi f27, f22, 0xd1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 288(x27)\n"
			"flw.ps f28, 224(x27)\n"
			"fmulh.pi f25, f26, f28\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 576(x25)\n"
			"fpackreph.pi f14, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 928(x15)\n"
			"fpackrepb.pi f27, f29\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 768(x14)\n"
			"fround.ps f17, f15, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 544(x17)\n"
			"flw.ps f28, 512(x17)\n"
			"fsgnjx.s f15, f3, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 960(x9)\n"
			"flw.ps f18, 672(x9)\n"
			"fle.s x14, f17, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 704(x6)\n"
			"flw.ps f26, 896(x6)\n"
			"fcmovm.ps f17, f8, f26\n" 
			"fbci.ps f11, 0x71d0\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 192(x23)\n"
			"flw.ps f27, 512(x23)\n"
			"feq.pi f18, f8, f27\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 960(x15)\n"
			"fsatu8.pi f11, f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_5:\n"
			"mov.m.x m1, x6, 0x69\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 96(x9)\n"
			"flw.ps f22, 672(x9)\n"
			"flt.ps f17, f14, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 288(x6)\n"
			"flw.ps f3, 928(x6)\n"
			"flw.ps f15, 832(x6)\n"
			"fmadd.ps f11, f1, f3, f15, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_5:\n"
			"mov.m.x m7, x14, 0x2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f22, 672(x25)\n"
			"flw.ps f0, 800(x25)\n"
			"for.pi f17, f22, f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 128(x15)\n"
			"flw.ps f18, 960(x15)\n"
			"feq.ps f14, f8, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 96(x23)\n"
			"flw.ps f22, 64(x23)\n"
			"feq.pi f11, f28, f22\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 992(x23)\n"
			"flw.ps f0, 960(x23)\n"
			"fmax.pi f26, f8, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_5:\n"
			"masknot m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x23, 0xdb0970aa1edc1446\n"
			"fcvt.s.wu f28, x23, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 64(x6)\n"
			"flw.ps f8, 320(x6)\n"
			"fsgnj.ps f11, f14, f8\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_5:\n"
			"maskxor m1, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 896(x9)\n"
			"flw.ps f0, 256(x9)\n"
			"flw.ps f26, 704(x9)\n"
			"fcmov.ps f29, f17, f0, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 0(x17)\n"
			"flw.ps f26, 32(x17)\n"
			"fcmovm.ps f26, f3, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 704(x14)\n"
			"flw.ps f26, 160(x14)\n"
			"fmul.ps f9, f27, f26, rmm\n" 
			"fbci.ps f29, 0x5e826\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 32(x28)\n"
			"flw.ps f26, 320(x28)\n"
			"fsub.pi f3, f29, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 384(x15)\n"
			"flw.ps f9, 896(x15)\n"
			"fsub.ps f14, f14, f9, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 800(x19)\n"
			"flw.ps f18, 288(x19)\n"
			"flw.ps f22, 96(x19)\n"
			"fnmadd.ps f17, f25, f18, f22, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 96(x6)\n"
			"flw.ps f27, 768(x6)\n"
			"fsra.pi f9, f9, f27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 800(x5)\n"
			"fxor.pi f18, f18, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 576(x5)\n"
			"flw.ps f29, 544(x5)\n"
			"flw.ps f18, 320(x5)\n"
			"fcmov.ps f14, f3, f29, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 544(x15)\n"
			"flw.ps f3, 704(x15)\n"
			"fmul.ps f28, f8, f3, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 480(x23)\n"
			"flw.ps f11, 0(x23)\n"
			"flw.ps f17, 896(x23)\n"
			"fcmov.ps f29, f8, f11, f17\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_5:\n"
			"fltm.pi m6, f1, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_5:\n"
			"masknot m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 384(x14)\n"
			"flw.ps f27, 32(x14)\n"
			"fmax.ps f26, f3, f27\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_5:\n"
			"feqm.ps m7, f15, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f25, 32(x19)\n"
			"flw.ps f14, 96(x19)\n"
			"fxor.pi f0, f25, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 192(x14)\n"
			"fpackreph.pi f27, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 896(x9)\n"
			"flw.ps f0, 160(x9)\n"
			"fmul.s f28, f27, f0, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_5:\n"
			"fltm.pi m2, f0, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_5:\n"
			"maskor m7, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f11, 512(x28)\n"
			"for.pi f18, f11, f11\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 288(x15)\n"
			"flw.ps f18, 384(x15)\n"
			"fmaxu.pi f25, f28, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 448(x5)\n"
			"fcvt.pw.ps f8, f25, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 736(x5)\n"
			"flw.ps f9, 640(x5)\n"
			"fsgnjx.ps f1, f22, f9\n" 
			"la x6, rand_int_32\n"
			"flw.ps f18, 864(x6)\n"
			"flw.ps f29, 736(x6)\n"
			"fmaxu.pi f0, f18, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 800(x23)\n"
			"flw.ps f18, 64(x23)\n"
			"fsgnjx.ps f17, f14, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 512(x9)\n"
			"flw.ps f11, 256(x9)\n"
			"flt.ps f17, f3, f11\n" 
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_5:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 544(x6)\n"
			"flog.ps f25, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 32(x23)\n"
			"flw.ps f17, 128(x23)\n"
			"fsgnjn.ps f1, f26, f17\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 928(x5)\n"
			"fsrai.pi f3, f27, 0xd\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 768(x6)\n"
			"flw.ps f28, 608(x6)\n"
			"flt.ps f18, f0, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 928(x27)\n"
			"ffrc.ps f9, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 448(x17)\n"
			"flw.ps f15, 864(x17)\n"
			"flt.s x14, f14, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 352(x9)\n"
			"flw.ps f26, 992(x9)\n"
			"fmax.ps f26, f15, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 128(x28)\n"
			"flw.ps f18, 896(x28)\n"
			"flw.ps f0, 512(x28)\n"
			"fmadd.s f3, f3, f18, f0, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 224(x17)\n"
			"fmax.s f18, f14, f14\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_5:\n"
			"maskand m7, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 640(x23)\n"
			"flw.ps f26, 64(x23)\n"
			"fsgnj.s f22, f0, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 512(x5)\n"
			"flw.ps f27, 256(x5)\n"
			"fmul.s f11, f28, f27, rdn\n" 
			"mova.x.m x25\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 128(x23)\n"
			"flw.ps f3, 288(x23)\n"
			"fadd.ps f11, f9, f3, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 288(x23)\n"
			"flw.ps f28, 832(x23)\n"
			"fdivu.pi f9, f0, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 896(x19)\n"
			"fswizz.ps f28, f29, 0x2\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_5:\n"
			"fsetm.pi m2, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 992(x23)\n"
			"fclass.s x23, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 800(x9)\n"
			"fsqrt.s f15, f17, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 6 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f7, 512(x11)\n"
			"flw.ps f25, 256(x11)\n"
			"fle.pi f19, f7, f25\n" 
			"fbci.pi f31, 0xa9a0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 992(x12)\n"
			"fround.ps f31, f11, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 832(x28)\n"
			"flw.ps f21, 896(x28)\n"
			"flw.ps f3, 256(x28)\n"
			"fmsub.ps f9, f19, f21, f3, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 128(x16)\n"
			"flw.ps f23, 832(x16)\n"
			"fmax.s f23, f7, f23\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 192(x10)\n"
			"fcvt.w.s x19, f25, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"mov.m.x m4, x6, 0x29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 864(x19)\n"
			"fxor.pi f6, f9, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 736(x28)\n"
			"flw.ps f19, 96(x28)\n"
			"fmaxu.pi f14, f23, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 704(x14)\n"
			"fcvt.wu.s x6, f9, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 352(x28)\n"
			"flw.ps f17, 0(x28)\n"
			"fsub.ps f20, f23, f17, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 800(x14)\n"
			"flw.ps f31, 544(x14)\n"
			"fsgnj.s f19, f11, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 384(x10)\n"
			"flw.ps f23, 832(x10)\n"
			"fmul.s f23, f14, f23, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 192(x6)\n"
			"flw.ps f7, 704(x6)\n"
			"flw.ps f9, 992(x6)\n"
			"fmadd.s f23, f13, f7, f9, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_6:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 992(x26)\n"
			"flog.ps f21, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f7, 96(x9)\n"
			"fslli.pi f20, f7, 0x2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"maskand m4, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f25, 256(x26)\n"
			"flw.ps f7, 224(x26)\n"
			"fmin.pi f3, f25, f7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_6:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 320(x6)\n"
			"fexp.ps f29, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 896(x9)\n"
			"flw.ps f25, 832(x9)\n"
			"fmulh.pi f11, f29, f25\n" 
			"li x5, 0x507a74fa4e621d91\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe62940969aa79d25\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_int_32\n"
			"flw.ps f7, 64(x18)\n"
			"flw.ps f23, 512(x18)\n"
			"for.pi f10, f7, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 320(x28)\n"
			"flw.ps f17, 480(x28)\n"
			"flw.ps f23, 928(x28)\n"
			"fcmov.ps f3, f6, f17, f23\n" 
			"maskpopc x28, m5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 928(x26)\n"
			"flw.ps f21, 960(x26)\n"
			"fmin.ps f9, f23, f21\n" 
			"fbci.ps f20, 0x3d8eb\n" 
			"fbci.pi f25, 0x7de16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 384(x14)\n"
			"flw.ps f14, 672(x14)\n"
			"flt.ps f9, f10, f14\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 576(x16)\n"
			"flw.ps f25, 256(x16)\n"
			"fsll.pi f11, f7, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 832(x9)\n"
			"fandi.pi f14, f29, 0x140\n" 
			"li x5, 0xc937b05c2ce99898\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xcff92a9c46cb0948\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"maskand m7, m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 352(x12)\n"
			"flw.ps f25, 64(x12)\n"
			"flw.ps f21, 960(x12)\n"
			"fcmov.ps f17, f10, f25, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 832(x10)\n"
			"flw.ps f3, 32(x10)\n"
			"fsgnjn.s f13, f17, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_342455_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xa1 << TENSOR_FMA32_SCP_LOC_B) | (0x22 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_431678_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x6e << TENSOR_FMA16_SCP_LOC_B) | (0xf8 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_386139_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xef << TENSOR_FMA16_SCP_LOC_B) | (0x73 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 64(x28)\n"
			"fsin.ps f19, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 672(x28)\n"
			"flw.ps f7, 704(x28)\n"
			"for.pi f20, f19, f7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 288(x12)\n"
			"fsub.ps f3, f14, f14, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 864(x6)\n"
			"frsq.ps f19, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 416(x16)\n"
			"flw.ps f21, 992(x16)\n"
			"fsgnjx.ps f9, f6, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 224(x28)\n"
			"flw.ps f19, 192(x28)\n"
			"fmax.ps f13, f31, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 352(x11)\n"
			"flw.ps f23, 480(x11)\n"
			"fmax.s f29, f21, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 608(x26)\n"
			"flw.ps f31, 576(x26)\n"
			"flw.ps f19, 480(x26)\n"
			"fmsub.s f3, f10, f31, f19, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 320(x9)\n"
			"fcvt.pwu.ps f29, f7, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 512(x26)\n"
			"fclass.ps f13, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f29, 544(x11)\n"
			"fsrai.pi f19, f29, 0x7\n" 
			"maskpopc x6, m5\n" 
			"li x18, 0xe884d1a9858faea8\n"
			"fcvt.s.l f20, x18, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 352(x28)\n"
			"fpackreph.pi f29, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 32(x11)\n"
			"flw.ps f3, 256(x11)\n"
			"flw.ps f29, 128(x11)\n"
			"fnmadd.ps f9, f23, f3, f29, rdn\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_6:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 608(x6)\n"
			"frcp.ps f25, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_856133_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xdc << TENSOR_FMA16_SCP_LOC_B) | (0x7b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x4 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xf << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 544(x9)\n"
			"flw.ps f10, 288(x9)\n"
			"flt.s x11, f17, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 32(x16)\n"
			"flw.ps f7, 672(x16)\n"
			"feq.ps f21, f17, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 864(x9)\n"
			"flw.ps f20, 640(x9)\n"
			"flw.ps f9, 64(x9)\n"
			"fmadd.s f3, f17, f20, f9, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 320(x12)\n"
			"flw.ps f25, 384(x12)\n"
			"fsgnjx.s f19, f23, f25\n" 
			"li x11, 0x9ed1617d03dbf497\n"
			"fcvt.s.w f19, x11, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_6:\n"
			"mov.m.x m5, x9, 0x3b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_6:\n"
			"flem.ps m6, f13, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 704(x28)\n"
			"flw.ps f21, 192(x28)\n"
			"fmax.ps f6, f19, f21\n" 
			"li x5, 0x1292\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f25, 416(x11)\n"
			"flw.ps f20, 160(x11)\n"
			"fmulh.pi f3, f25, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 256(x9)\n"
			"flw.ps f21, 704(x9)\n"
			"fsgnjn.s f19, f20, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 448(x19)\n"
			"feq.ps f21, f21, f21\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 448(x10)\n"
			"flw.ps f31, 320(x10)\n"
			"fand.pi f25, f6, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 576(x9)\n"
			"fpackreph.pi f31, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 384(x9)\n"
			"fdiv.pi f6, f21, f21\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 832(x26)\n"
			"fslli.pi f11, f17, 0x3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 448(x28)\n"
			"fandi.pi f29, f6, 0xe2\n" 
			"li x5, 0x47bf\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xc6 << TENSOR_FMA16_SCP_LOC_B) | (0x7c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 832(x6)\n"
			"flw.ps f14, 928(x6)\n"
			"flw.ps f21, 992(x6)\n"
			"fmsub.s f9, f6, f14, f21, rmm\n" 
			"mova.x.m x9\n" 
			"li x5, 0xe0d5037ea1bb7058\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x57573b212f18c17d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 928(x9)\n"
			"fround.ps f19, f10, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 704(x6)\n"
			"fslli.pi f10, f20, 0x8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x16, m4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 128(x6)\n"
			"flw.ps f9, 128(x6)\n"
			"fmulh.pi f31, f17, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 288(x6)\n"
			"flw.ps f14, 640(x6)\n"
			"fsub.ps f25, f11, f14, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 256(x6)\n"
			"fsrai.pi f29, f3, 0xb\n" 
			"li x9, 0x840519956060dd04\n"
			"fcvt.s.wu f21, x9, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 416(x6)\n"
			"flw.ps f23, 640(x6)\n"
			"fmul.pi f11, f21, f23\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 32(x12)\n"
			"fsin.ps f14, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 224(x10)\n"
			"flw.ps f19, 928(x10)\n"
			"fsgnjx.ps f11, f6, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 288(x11)\n"
			"flw.ps f20, 800(x11)\n"
			"fmax.s f10, f21, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 864(x14)\n"
			"flw.ps f25, 416(x14)\n"
			"fle.pi f17, f14, f25\n" 
			"mova.x.m x12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f17, 768(x11)\n"
			"flw.ps f25, 800(x11)\n"
			"fmaxu.pi f6, f17, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_96831_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x4 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x6 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x4 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x47 << TENSOR_FMA16_SCP_LOC_B) | (0xb9 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x47 << TENSOR_FMA32_SCP_LOC_B) | (0x9d << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4a << TENSOR_IMA_SCP_LOC_B) | (0xac << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"maskpopc x10, m5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 448(x14)\n"
			"flw.ps f11, 32(x14)\n"
			"fmin.s f9, f25, f11\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 480(x26)\n"
			"flw.ps f31, 0(x26)\n"
			"for.pi f29, f7, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 96(x6)\n"
			"fsat8.pi f29, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 896(x9)\n"
			"flw.ps f29, 160(x9)\n"
			"flw.ps f6, 992(x9)\n"
			"fnmadd.s f21, f31, f29, f6, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_6:\n"
			"masknot m5, m5\n" 
			"li x5, 0x80340168\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x9d32\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f17, 992(x12)\n"
			"flw.ps f23, 512(x12)\n"
			"fltu.pi f7, f17, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 320(x14)\n"
			"flw.ps f21, 64(x14)\n"
			"flw.ps f23, 576(x14)\n"
			"fnmadd.ps f31, f14, f21, f23, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 736(x28)\n"
			"flw.ps f6, 992(x28)\n"
			"fmax.s f19, f10, f6\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_6:\n"
			"fltm.ps m7, f19, f7\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_int_32\n"
			"flw.ps f9, 896(x18)\n"
			"flw.ps f11, 416(x18)\n"
			"fmax.pi f9, f9, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 864(x10)\n"
			"flw.ps f13, 672(x10)\n"
			"flt.pi f7, f17, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 832(x26)\n"
			"fsatu8.pi f19, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 448(x11)\n"
			"flw.ps f21, 800(x11)\n"
			"fmsub.ps f13, f29, f21, f21, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 672(x16)\n"
			"flw.ps f14, 736(x16)\n"
			"fsra.pi f21, f17, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 192(x28)\n"
			"flw.ps f23, 32(x28)\n"
			"fmul.s f19, f21, f23, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4d << TENSOR_IMA_SCP_LOC_B) | (0x74 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 7 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 448(x20)\n"
			"flw.ps f6, 512(x20)\n"
			"fadd.pi f16, f31, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 416(x17)\n"
			"fcvt.pw.ps f25, f25, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 64(x22)\n"
			"fpackreph.pi f16, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 576(x6)\n"
			"flw.ps f10, 96(x6)\n"
			"flw.ps f19, 608(x6)\n"
			"fnmadd.s f30, f21, f10, f19, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 224(x25)\n"
			"fsrli.pi f1, f17, 0x0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 576(x12)\n"
			"fcvt.pw.ps f8, f21, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 224(x6)\n"
			"flw.ps f14, 960(x6)\n"
			"flw.ps f7, 928(x6)\n"
			"fnmadd.s f14, f16, f14, f7, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 576(x17)\n"
			"faddi.pi f17, f28, 0x31\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 96(x20)\n"
			"fsub.pi f17, f7, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 512(x20)\n"
			"fcvt.wu.s x25, f16, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 800(x6)\n"
			"flw.ps f31, 480(x6)\n"
			"flt.ps f10, f17, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 160(x9)\n"
			"flw.ps f14, 352(x9)\n"
			"flw.ps f2, 640(x9)\n"
			"fmsub.ps f30, f21, f14, f2, rne\n" 
			"li x25, 0x3e080a80f86ee8f4\n"
			"fcvt.s.wu f1, x25, rtz\n" 
			"fbci.pi f14, 0x26dad\n" 
			"la x20, rand_int_32\n"
			"flw.ps f19, 864(x20)\n"
			"faddi.pi f14, f19, 0x15a\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 96(x22)\n"
			"flw.ps f28, 640(x22)\n"
			"feq.pi f28, f25, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 192(x15)\n"
			"flw.ps f17, 0(x15)\n"
			"fsub.s f10, f20, f17, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 192(x26)\n"
			"fcvt.ps.pwu f19, f1, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 320(x26)\n"
			"flw.ps f30, 768(x26)\n"
			"fmin.s f20, f1, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 352(x22)\n"
			"flw.ps f10, 256(x22)\n"
			"flt.pi f7, f8, f10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 928(x25)\n"
			"flw.ps f14, 768(x25)\n"
			"for.pi f1, f8, f14\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 992(x20)\n"
			"fcvt.f16.ps f20, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_7:\n"
			"mov.m.x m7, x15, 0x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f28, 832(x18)\n"
			"flw.ps f1, 448(x18)\n"
			"flt.pi f20, f28, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 480(x15)\n"
			"fcvt.pwu.ps f20, f31, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 224(x9)\n"
			"flw.ps f25, 0(x9)\n"
			"flw.ps f28, 864(x9)\n"
			"fnmsub.s f28, f21, f25, f28, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 704(x26)\n"
			"flw.ps f31, 832(x26)\n"
			"feq.s x25, f2, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_7:\n"
			"maskxor m4, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 768(x22)\n"
			"flw.ps f1, 640(x22)\n"
			"fsgnjx.ps f2, f14, f1\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 32(x9)\n"
			"fslli.pi f16, f1, 0x5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 160(x25)\n"
			"flw.ps f14, 32(x25)\n"
			"fmax.ps f30, f21, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_7:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4bbc60b700\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 896(x6)\n"
			"fswizz.ps f28, f7, 0xd\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 384(x20)\n"
			"flw.ps f28, 960(x20)\n"
			"fminu.pi f31, f6, f28\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_7:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 256(x6)\n"
			"flog.ps f31, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_7:\n"
			"maskand m4, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_7:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 704(x6)\n"
			"fexp.ps f10, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"fltm.ps m3, f10, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f6, 736(x25)\n"
			"fslli.pi f6, f6, 0x1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 480(x9)\n"
			"flw.ps f2, 128(x9)\n"
			"fadd.s f20, f31, f2, rdn\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_7:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 960(x25)\n"
			"frcp.ps f20, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 832(x20)\n"
			"flw.ps f14, 64(x20)\n"
			"fsgnjx.s f19, f20, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 32(x6)\n"
			"flw.ps f8, 640(x6)\n"
			"flt.pi f21, f28, f8\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f14, 928(x26)\n"
			"fcvt.ps.f16 f2, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 192(x22)\n"
			"flw.ps f6, 128(x22)\n"
			"flw.ps f1, 448(x22)\n"
			"fnmsub.ps f20, f2, f6, f1, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 288(x6)\n"
			"flw.ps f17, 544(x6)\n"
			"fsub.s f21, f10, f17, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_7:\n"
			"masknot m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 512(x22)\n"
			"flw.ps f25, 192(x22)\n"
			"flt.ps f30, f10, f25\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 608(x15)\n"
			"flw.ps f14, 960(x15)\n"
			"feq.s x9, f16, f14\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 640(x26)\n"
			"fcvt.ps.pw f20, f8, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 928(x17)\n"
			"flw.ps f17, 96(x17)\n"
			"fmax.pi f6, f6, f17\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 672(x26)\n"
			"flw.ps f20, 576(x26)\n"
			"flw.ps f1, 448(x26)\n"
			"fmadd.ps f7, f25, f20, f1, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 96(x6)\n"
			"flw.ps f8, 672(x6)\n"
			"flw.ps f19, 992(x6)\n"
			"fmadd.ps f7, f1, f8, f19, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 448(x18)\n"
			"flw.ps f7, 512(x18)\n"
			"flt.ps f25, f20, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 0(x18)\n"
			"flw.ps f19, 320(x18)\n"
			"fadd.pi f7, f1, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 544(x17)\n"
			"flw.ps f7, 32(x17)\n"
			"fle.s x18, f2, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 992(x9)\n"
			"flw.ps f7, 448(x9)\n"
			"fsub.ps f8, f1, f7, rtz\n" 
			"mova.x.m x26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 224(x18)\n"
			"flw.ps f21, 576(x18)\n"
			"fsgnjn.s f14, f25, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 576(x6)\n"
			"flw.ps f30, 896(x6)\n"
			"flt.ps f17, f25, f30\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 800(x15)\n"
			"flw.ps f31, 512(x15)\n"
			"fltu.pi f20, f20, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 896(x18)\n"
			"flw.ps f6, 896(x18)\n"
			"fsgnj.s f28, f20, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 512(x6)\n"
			"flw.ps f21, 704(x6)\n"
			"fnmadd.s f30, f19, f19, f21, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_7:\n"
			"maskxor m2, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x20, rand_int_32\n"
			"flw.ps f19, 832(x20)\n"
			"flw.ps f20, 672(x20)\n"
			"flt.pi f31, f19, f20\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 928(x17)\n"
			"flw.ps f25, 352(x17)\n"
			"fdiv.pi f25, f31, f25\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 384(x22)\n"
			"flw.ps f10, 640(x22)\n"
			"fmul.pi f30, f20, f10\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_7:\n"
			"fltm.ps m3, f8, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 544(x9)\n"
			"flw.ps f17, 0(x9)\n"
			"fmul.s f16, f30, f17, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_7:\n"
			"fsetm.pi m3, f17\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"li x20, 0xf6e83d9ee764a92e\n"
			"fcvt.s.w f8, x20, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 256(x15)\n"
			"flw.ps f7, 160(x15)\n"
			"fmin.ps f8, f1, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 960(x20)\n"
			"flw.ps f21, 544(x20)\n"
			"fsgnj.s f25, f10, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 576(x17)\n"
			"flw.ps f16, 544(x17)\n"
			"fsgnj.s f31, f30, f16\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 320(x6)\n"
			"flw.ps f20, 64(x6)\n"
			"fsrl.pi f19, f28, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 544(x26)\n"
			"flw.ps f28, 320(x26)\n"
			"fsub.pi f31, f1, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 768(x6)\n"
			"flw.ps f19, 736(x6)\n"
			"fle.ps f2, f10, f19\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 256(x26)\n"
			"flw.ps f8, 192(x26)\n"
			"fltu.pi f7, f14, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 384(x6)\n"
			"flw.ps f21, 32(x6)\n"
			"flw.ps f6, 128(x6)\n"
			"fnmsub.s f28, f16, f21, f6, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 640(x20)\n"
			"flw.ps f25, 128(x20)\n"
			"fsll.pi f16, f21, f25\n" 
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
			"la x20, rand_int_32\n"
			"flw.ps f7, 576(x20)\n"
			"flw.ps f6, 672(x20)\n"
			"fmulhu.pi f28, f7, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 160(x9)\n"
			"flw.ps f28, 800(x9)\n"
			"fsgnj.s f1, f6, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 0(x15)\n"
			"fle.s x25, f30, f30\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 768(x18)\n"
			"fpackrepb.pi f19, f19\n" 
			"fbci.ps f7, 0x2c146\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 384(x25)\n"
			"flw.ps f8, 768(x25)\n"
			"fmulh.pi f14, f14, f8\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 736(x22)\n"
			"flw.ps f31, 544(x22)\n"
			"fsll.pi f7, f7, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 384(x9)\n"
			"fcvt.f16.ps f6, f2\n" 
			"fbci.pi f8, 0x7a6de\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 64(x15)\n"
			"flw.ps f28, 480(x15)\n"
			"flw.ps f30, 544(x15)\n"
			"fmadd.s f14, f19, f28, f30, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 32(x12)\n"
			"flw.ps f17, 576(x12)\n"
			"fmulh.pi f31, f21, f17\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_7:\n"
			"fsetm.pi m4, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 64(x26)\n"
			"flw.ps f6, 352(x26)\n"
			"fsgnjx.ps f25, f10, f6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 288(x6)\n"
			"fsatu8.pi f20, f10\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 736(x9)\n"
			"flw.ps f16, 704(x9)\n"
			"fsub.pi f2, f14, f16\n" 
			"li x20, 0x635072df87cb605a\n"
			"fcvt.s.w f31, x20, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 8 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f5,  416 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 896(x9)\n"
			"fsgnj.s f18, f18, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 96(x7)\n"
			"fcvt.pw.ps f29, f9, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 448(x26)\n"
			"fsrai.pi f19, f0, 0x8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 992(x18)\n"
			"fcvt.f16.ps f13, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"maskor m3, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 512(x26)\n"
			"flw.ps f25, 160(x26)\n"
			"fcmovm.ps f10, f0, f25\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 800(x26)\n"
			"fslli.pi f2, f13, 0xf\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 800(x20)\n"
			"flw.ps f17, 192(x20)\n"
			"feq.ps f3, f9, f17\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 320(x18)\n"
			"flw.ps f6, 832(x18)\n"
			"fadd.s f9, f9, f6, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 224(x23)\n"
			"flw.ps f3, 416(x23)\n"
			"flw.ps f10, 64(x23)\n"
			"fnmsub.ps f24, f26, f3, f10, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_8:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 640(x9)\n"
			"frcp.ps f17, f9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 864(x9)\n"
			"fmax.pi f10, f9, f9\n" 
			"fbci.ps f26, 0x440ee\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 320(x19)\n"
			"fcvt.ps.pwu f29, f18, rmm\n" 
			"li x5, 0xa13f\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f3, 0x11c61\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 672(x23)\n"
			"fround.ps f9, f6, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x1 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x18 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x1 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f0, 832(x23)\n"
			"flw.ps f2, 192(x23)\n"
			"for.pi f17, f0, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 32(x9)\n"
			"frsq.ps f9, f18\n" 
			"li x5, 0x9542\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f26, 192(x17)\n"
			"faddi.pi f6, f26, 0x17e\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 512(x23)\n"
			"flw.ps f9, 256(x23)\n"
			"feq.pi f24, f6, f9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 224(x19)\n"
			"flw.ps f20, 928(x19)\n"
			"fmax.ps f6, f13, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 512(x25)\n"
			"flw.ps f25, 672(x25)\n"
			"fle.s x20, f29, f25\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_8:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 352(x16)\n"
			"flog.ps f19, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f24, 96(x27)\n"
			"fslli.pi f13, f24, 0xe\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 992(x20)\n"
			"fcvt.wu.s x16, f9, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"feqm.ps m7, f29, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xbabe\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 512(x19)\n"
			"frsq.ps f18, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 0(x26)\n"
			"flw.ps f29, 736(x26)\n"
			"flw.ps f2, 864(x26)\n"
			"fmadd.s f20, f6, f29, f2, rup\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_8:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 96(x27)\n"
			"flog.ps f17, f17\n" 
			"mova.x.m x18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f0, 0(x27)\n"
			"flw.ps f17, 320(x27)\n"
			"fle.pi f18, f0, f17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 0(x27)\n"
			"flw.ps f10, 288(x27)\n"
			"fadd.ps f10, f25, f10, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_879937_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5c << TENSOR_FMA16_SCP_LOC_B) | (0xaf << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f31, 608(x16)\n"
			"flw.ps f29, 800(x16)\n"
			"fsll.pi f29, f31, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 992(x18)\n"
			"flw.ps f2, 320(x18)\n"
			"fmin.s f29, f29, f2\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_8:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 160(x27)\n"
			"fexp.ps f10, f31\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_8:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 896(x20)\n"
			"fexp.ps f24, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f18, 704(x26)\n"
			"flw.ps f2, 480(x26)\n"
			"fmulhu.pi f6, f18, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 320(x16)\n"
			"fcvt.pwu.ps f0, f26, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 0(x23)\n"
			"flw.ps f24, 544(x23)\n"
			"fand.pi f31, f19, f24\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_8:\n"
			"flem.ps m7, f10, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f24, 608(x16)\n"
			"fcvt.ps.pwu f26, f24, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 160(x9)\n"
			"flw.ps f20, 480(x9)\n"
			"fle.ps f6, f26, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 320(x17)\n"
			"flw.ps f0, 992(x17)\n"
			"fadd.s f31, f17, f0, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 768(x25)\n"
			"flw.ps f17, 96(x25)\n"
			"flw.ps f6, 352(x25)\n"
			"fnmadd.s f31, f25, f17, f6, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 448(x9)\n"
			"ffrc.ps f26, f25\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_8:\n"
			"maskor m3, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f0, 448(x20)\n"
			"fsub.pi f25, f0, f0\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 704(x23)\n"
			"flw.ps f10, 960(x23)\n"
			"fltu.pi f24, f3, f10\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 608(x27)\n"
			"ffrc.ps f0, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_8:\n"
			"flem.ps m5, f24, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f20, 0x69e2d\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 64(x26)\n"
			"flw.ps f10, 544(x26)\n"
			"fle.s x20, f19, f10\n" 
			"fbci.ps f10, 0x37b04\n" 
			"li x5, 0x74f80b451bb1fdd9\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2ed0433e5b2c78ee\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f2, 864(x9)\n"
			"flw.ps f0, 736(x9)\n"
			"fmulh.pi f29, f2, f0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 192(x25)\n"
			"flw.ps f25, 64(x25)\n"
			"flw.ps f13, 416(x25)\n"
			"fnmsub.s f26, f18, f25, f13, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 288(x17)\n"
			"fpackrepb.pi f18, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 416(x20)\n"
			"fswizz.ps f13, f25, 0x1b\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_8:\n"
			"maskor m5, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 736(x25)\n"
			"flw.ps f10, 704(x25)\n"
			"flw.ps f9, 352(x25)\n"
			"fmsub.s f19, f2, f10, f9, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 928(x25)\n"
			"flw.ps f9, 128(x25)\n"
			"fsgnjx.ps f13, f0, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 768(x7)\n"
			"flw.ps f9, 960(x7)\n"
			"flw.ps f6, 992(x7)\n"
			"fnmadd.s f20, f0, f9, f6, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 768(x9)\n"
			"flw.ps f3, 928(x9)\n"
			"flw.ps f17, 672(x9)\n"
			"fcmov.ps f26, f0, f3, f17\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 608(x7)\n"
			"flw.ps f9, 64(x7)\n"
			"fsub.ps f6, f17, f9, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 416(x18)\n"
			"flw.ps f9, 352(x18)\n"
			"fsrl.pi f26, f24, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_61901_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x2 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x7 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x5 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x0 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
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
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_8:\n"
			"mov.m.x m3, x27, 0x4d\n" 
			"li x5, 0x803401D0\n"
			"ld x5, 0(x5)\n"
			"la x19, rand_int_32\n"
			"flw.ps f17, 256(x19)\n"
			"fsrai.pi f25, f17, 0x2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_8:\n"
			"maskor m5, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f3, 288(x25)\n"
			"faddi.pi f6, f3, 0x144\n" 
			"maskpopc x9, m5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 64(x19)\n"
			"flw.ps f0, 32(x19)\n"
			"fsgnj.s f10, f31, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 928(x26)\n"
			"frsq.ps f3, f3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 480(x17)\n"
			"flw.ps f24, 672(x17)\n"
			"fle.ps f18, f19, f24\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 192(x7)\n"
			"flw.ps f9, 896(x7)\n"
			"fand.pi f19, f6, f9\n" 
			"li x16, 0x56c702299d89976c\n"
			"fcvt.s.l f24, x16, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 800(x7)\n"
			"fcvt.w.s x16, f2, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 288(x16)\n"
			"flw.ps f26, 512(x16)\n"
			"fsgnjn.ps f13, f0, f26\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 96(x27)\n"
			"flw.ps f24, 832(x27)\n"
			"fsll.pi f19, f13, f24\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 992(x27)\n"
			"fsatu8.pi f17, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 320(x26)\n"
			"fsat8.pi f24, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 928(x25)\n"
			"flw.ps f19, 64(x25)\n"
			"flw.ps f10, 224(x25)\n"
			"fnmadd.ps f9, f2, f19, f10, rdn\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x9, rand_int_32\n"
			"flw.ps f24, 256(x9)\n"
			"fcvt.ps.pw f0, f24, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 64(x19)\n"
			"fcvt.w.s x9, f9, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 160(x7)\n"
			"flw.ps f17, 736(x7)\n"
			"fadd.s f17, f10, f17, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 256(x19)\n"
			"fpackreph.pi f29, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 384(x17)\n"
			"flw.ps f9, 672(x17)\n"
			"flw.ps f3, 256(x17)\n"
			"fnmsub.s f24, f2, f9, f3, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_8:\n"
			"fltm.pi m0, f3, f25\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f6, 992(x7)\n"
			"flw.ps f3, 448(x7)\n"
			"fltu.pi f26, f6, f3\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_8:\n"
			"flem.ps m7, f10, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 64(x9)\n"
			"flw.ps f9, 544(x9)\n"
			"flw.ps f25, 256(x9)\n"
			"fnmsub.s f6, f26, f9, f25, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 64(x19)\n"
			"flw.ps f0, 608(x19)\n"
			"flw.ps f6, 640(x19)\n"
			"fmsub.ps f24, f20, f0, f6, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_8:\n"
			"flem.ps m7, f17, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_8:\n"
			"fltm.ps m3, f2, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f6, 640(x23)\n"
			"flw.ps f10, 896(x23)\n"
			"for.pi f2, f6, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_FP_TRANS_ID_17_HID_8:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 480(x17)\n"
			"fexp.ps f18, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 32(x25)\n"
			"flw.ps f31, 992(x25)\n"
			"fmax.ps f19, f25, f31\n" 
			"LBL_SEQID_5_FP_TRANS_ID_18_HID_8:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 352(x23)\n"
			"flog.ps f0, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x1118\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_769444_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x7c << TENSOR_FMA32_SCP_LOC_B) | (0x76 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xee << TENSOR_FMA16_SCP_LOC_B) | (0xc2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x80 << TENSOR_IMA_SCP_LOC_B) | (0xeb << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 9 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f23, 96(x5)\n"
			"fminu.pi f23, f23, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 256(x18)\n"
			"fcvt.wu.s x10, f2, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 384(x17)\n"
			"fpackrepb.pi f4, f10\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 224(x18)\n"
			"flw.ps f14, 480(x18)\n"
			"flw.ps f10, 928(x18)\n"
			"fnmsub.ps f14, f2, f14, f10, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_9:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 800(x5)\n"
			"flog.ps f4, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 320(x20)\n"
			"fround.ps f21, f30, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 928(x22)\n"
			"flw.ps f30, 768(x22)\n"
			"fadd.s f31, f18, f30, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 448(x14)\n"
			"flw.ps f0, 480(x14)\n"
			"flt.s x5, f4, f0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 0(x6)\n"
			"fslli.pi f31, f31, 0x2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 288(x22)\n"
			"fcvt.wu.s x9, f14, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 864(x22)\n"
			"flw.ps f31, 96(x22)\n"
			"fcmovm.ps f0, f26, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 192(x10)\n"
			"flw.ps f20, 224(x10)\n"
			"fle.ps f25, f10, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 768(x16)\n"
			"fcvt.pwu.ps f3, f14, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 896(x10)\n"
			"flw.ps f8, 64(x10)\n"
			"fle.pi f14, f20, f8\n" 
			"maskpopc x16, m4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 320(x14)\n"
			"flw.ps f30, 640(x14)\n"
			"fmin.s f18, f10, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f8, 128(x6)\n"
			"flw.ps f20, 544(x6)\n"
			"fsub.pi f21, f8, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 512(x20)\n"
			"fcvt.pw.ps f10, f8, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f10, 64(x10)\n"
			"flw.ps f0, 192(x10)\n"
			"for.pi f14, f10, f0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 448(x18)\n"
			"fexp.ps f2, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 608(x22)\n"
			"flw.ps f14, 832(x22)\n"
			"fmul.pi f26, f25, f14\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 352(x18)\n"
			"flw.ps f18, 480(x18)\n"
			"fdiv.s f9, f26, f18, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 608(x18)\n"
			"flw.ps f21, 0(x18)\n"
			"fmin.ps f26, f18, f21\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 416(x6)\n"
			"flw.ps f8, 448(x6)\n"
			"fsrl.pi f26, f9, f8\n" 
			"la x22, rand_int_32\n"
			"flw.ps f0, 448(x22)\n"
			"fcvt.ps.pwu f4, f0, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 704(x6)\n"
			"flw.ps f4, 928(x6)\n"
			"fadd.ps f25, f10, f4, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 544(x16)\n"
			"flw.ps f2, 192(x16)\n"
			"fmulhu.pi f10, f26, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"fltm.pi m5, f30, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 992(x5)\n"
			"flw.ps f26, 544(x5)\n"
			"flw.ps f21, 896(x5)\n"
			"fmadd.s f10, f2, f26, f21, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 384(x5)\n"
			"flw.ps f2, 384(x5)\n"
			"fle.pi f18, f10, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 704(x10)\n"
			"fcvt.ps.pw f4, f20, rtz\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f26, 128(x6)\n"
			"fcvt.ps.f16 f31, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 544(x20)\n"
			"flw.ps f30, 128(x20)\n"
			"fmul.ps f20, f8, f30, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 512(x17)\n"
			"flw.ps f20, 704(x17)\n"
			"fsgnjx.ps f3, f26, f20\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 640(x16)\n"
			"fpackrepb.pi f30, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 704(x16)\n"
			"flw.ps f26, 224(x16)\n"
			"fmulh.pi f26, f30, f26\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_9:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 320(x22)\n"
			"flog.ps f4, f26\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 96(x18)\n"
			"fsatu8.pi f20, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_9:\n"
			"maskxor m5, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 672(x18)\n"
			"flw.ps f3, 384(x18)\n"
			"fsgnjn.ps f14, f2, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 512(x14)\n"
			"frsq.ps f18, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 544(x10)\n"
			"flw.ps f2, 640(x10)\n"
			"fand.pi f20, f23, f2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 448(x5)\n"
			"flw.ps f2, 896(x5)\n"
			"fcmovm.ps f8, f14, f2\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 128(x22)\n"
			"fandi.pi f10, f18, 0x19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 800(x18)\n"
			"flw.ps f8, 704(x18)\n"
			"flt.pi f31, f18, f8\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 896(x9)\n"
			"flw.ps f14, 32(x9)\n"
			"fmul.pi f4, f9, f14\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 800(x20)\n"
			"fnot.pi f21, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 896(x10)\n"
			"flw.ps f14, 0(x10)\n"
			"fnmsub.s f23, f0, f14, f14, rdn\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_9:\n"
			"masknot m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 0(x17)\n"
			"fcvt.w.s x16, f3, rtz\n" 
			"fbci.pi f2, 0x347e1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 672(x14)\n"
			"fcvt.f16.ps f31, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 896(x16)\n"
			"flw.ps f30, 992(x16)\n"
			"flw.ps f9, 544(x16)\n"
			"fmsub.s f31, f14, f30, f9, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 288(x18)\n"
			"fremu.pi f18, f21, f21\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 704(x5)\n"
			"flw.ps f4, 832(x5)\n"
			"fadd.s f20, f14, f4, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 896(x18)\n"
			"fslli.pi f14, f20, 0x3\n" 
			"fbci.ps f30, 0x2a5f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 512(x6)\n"
			"flw.ps f0, 768(x6)\n"
			"feq.pi f9, f30, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 512(x17)\n"
			"flw.ps f18, 256(x17)\n"
			"fmin.s f3, f3, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 704(x9)\n"
			"fsin.ps f2, f26\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 224(x16)\n"
			"flw.ps f9, 352(x16)\n"
			"fsub.pi f26, f26, f9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 160(x9)\n"
			"flw.ps f3, 736(x9)\n"
			"fsll.pi f26, f0, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 608(x17)\n"
			"flw.ps f14, 864(x17)\n"
			"feq.pi f9, f10, f14\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_9:\n"
			"feqm.ps m5, f23, f30\n" 
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
			"la x14, rand_int_32\n"
			"flw.ps f23, 0(x14)\n"
			"fsrai.pi f14, f23, 0x1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 352(x18)\n"
			"flw.ps f10, 160(x18)\n"
			"fle.ps f18, f3, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 960(x16)\n"
			"flw.ps f25, 768(x16)\n"
			"fsll.pi f2, f3, f25\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 672(x10)\n"
			"flw.ps f3, 32(x10)\n"
			"flw.ps f26, 640(x10)\n"
			"fnmadd.s f26, f25, f3, f26, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f20, 896(x20)\n"
			"fslli.pi f23, f20, 0xe\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 736(x16)\n"
			"fcvt.w.s x9, f18, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 256(x20)\n"
			"fcvt.w.s x22, f18, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 448(x20)\n"
			"flw.ps f18, 800(x20)\n"
			"flw.ps f0, 672(x20)\n"
			"fmadd.ps f30, f25, f18, f0, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 928(x14)\n"
			"flw.ps f21, 992(x14)\n"
			"flw.ps f0, 160(x14)\n"
			"fmadd.ps f14, f23, f21, f0, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 928(x14)\n"
			"flw.ps f3, 96(x14)\n"
			"fmul.pi f0, f20, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 704(x6)\n"
			"flw.ps f30, 256(x6)\n"
			"fmax.ps f9, f4, f30\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 128(x20)\n"
			"fsgnjn.s f10, f4, f4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 288(x16)\n"
			"fsatu8.pi f4, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 32(x22)\n"
			"fcvt.pwu.ps f26, f31, rmm\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_9:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 800(x17)\n"
			"fexp.ps f8, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_9:\n"
			"mova.m.x x5\n" 
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
			"fbci.pi f31, 0x72018\n" 
			"la x5, rand_int_32\n"
			"flw.ps f4, 768(x5)\n"
			"fsrli.pi f26, f4, 0x3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 128(x5)\n"
			"flw.ps f2, 672(x5)\n"
			"fmsub.ps f31, f10, f10, f2, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 288(x6)\n"
			"flw.ps f10, 320(x6)\n"
			"fdiv.ps f14, f8, f10, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_9:\n"
			"fsetm.pi m6, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 416(x9)\n"
			"flw.ps f26, 896(x9)\n"
			"fmin.s f14, f9, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 320(x6)\n"
			"flw.ps f2, 384(x6)\n"
			"fminu.pi f10, f23, f2\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 800(x22)\n"
			"flw.ps f20, 640(x22)\n"
			"fle.pi f20, f3, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 416(x18)\n"
			"fsrai.pi f30, f14, 0x1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 288(x17)\n"
			"flw.ps f20, 416(x17)\n"
			"fsgnjn.s f3, f23, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 320(x14)\n"
			"fcvt.f16.ps f26, f21\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 640(x18)\n"
			"flw.ps f23, 576(x18)\n"
			"fmax.ps f23, f14, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 992(x9)\n"
			"fclass.ps f9, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 288(x14)\n"
			"flw.ps f18, 672(x14)\n"
			"fcmovm.ps f10, f21, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 512(x9)\n"
			"flw.ps f31, 288(x9)\n"
			"fand.pi f21, f3, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 288(x10)\n"
			"flw.ps f8, 64(x10)\n"
			"fsgnjn.s f4, f10, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 10 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 672(x22)\n"
			"flw.ps f28, 480(x22)\n"
			"fadd.s f12, f29, f28, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 256(x5)\n"
			"flw.ps f23, 160(x5)\n"
			"fsgnjx.ps f25, f28, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 320(x28)\n"
			"flw.ps f2, 192(x28)\n"
			"flw.ps f31, 128(x28)\n"
			"fnmadd.ps f2, f10, f2, f31, rup\n" 
			"li x5, 0x98ff013f13775f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6809338c03f1aaf9\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 352(x5)\n"
			"flw.ps f23, 896(x5)\n"
			"flw.ps f31, 544(x5)\n"
			"fmsub.ps f25, f25, f23, f31, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 640(x7)\n"
			"flw.ps f29, 224(x7)\n"
			"fle.s x28, f30, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 320(x11)\n"
			"flw.ps f29, 896(x11)\n"
			"fsub.ps f21, f24, f29, rup\n" 
			"fbci.pi f31, 0x790ca\n" 
			"li x19, 0x38ac1cc19c632329\n"
			"fcvt.s.l f29, x19, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 608(x5)\n"
			"flw.ps f12, 32(x5)\n"
			"fmax.ps f1, f28, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fsetm.pi m3, f30\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 640(x20)\n"
			"frsq.ps f30, f28\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 416(x22)\n"
			"flw.ps f30, 640(x22)\n"
			"fmaxu.pi f19, f29, f30\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 736(x10)\n"
			"flw.ps f19, 800(x10)\n"
			"fsgnjx.s f12, f10, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 800(x9)\n"
			"fclass.ps f13, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 32(x18)\n"
			"flw.ps f19, 64(x18)\n"
			"feq.pi f28, f24, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 800(x11)\n"
			"fround.ps f2, f21, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_301197_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_533357_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x4 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xa << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1c << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xb << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1f << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"maskpopcz x28, m3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 416(x19)\n"
			"flw.ps f3, 864(x19)\n"
			"fmin.ps f29, f21, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 352(x9)\n"
			"flw.ps f12, 32(x9)\n"
			"fsgnj.ps f31, f1, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 384(x20)\n"
			"flw.ps f30, 640(x20)\n"
			"fmax.pi f10, f0, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 736(x7)\n"
			"flw.ps f31, 672(x7)\n"
			"flw.ps f13, 320(x7)\n"
			"fnmadd.s f24, f23, f31, f13, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 384(x22)\n"
			"fandi.pi f3, f25, 0xeb\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_10:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_10:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 512(x22)\n"
			"fexp.ps f29, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 704(x28)\n"
			"fcvt.w.s x22, f10, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_16\n"
			"flw.ps f21, 384(x7)\n"
			"fcvt.ps.f16 f1, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"maskor m7, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 960(x14)\n"
			"flw.ps f0, 416(x14)\n"
			"flw.ps f21, 768(x14)\n"
			"fnmadd.ps f28, f30, f0, f21, rdn\n" 
			"li x5, 0xce60\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 320(x19)\n"
			"fcvt.w.s x7, f25, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 640(x11)\n"
			"flw.ps f31, 672(x11)\n"
			"fmax.s f25, f1, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 384(x28)\n"
			"fcvt.f16.ps f29, f13\n" 
			"mova.x.m x28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"li x5, 0x1017\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x5 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x47 << TENSOR_FMA16_SCP_LOC_B) | (0xc3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 384(x19)\n"
			"flw.ps f29, 800(x19)\n"
			"fmsub.ps f24, f10, f10, f29, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 160(x11)\n"
			"flw.ps f29, 768(x11)\n"
			"fsub.s f2, f25, f29, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f28, 128(x20)\n"
			"flt.pi f25, f28, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 288(x10)\n"
			"flw.ps f10, 32(x10)\n"
			"fminu.pi f13, f2, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 992(x10)\n"
			"flw.ps f24, 960(x10)\n"
			"fminu.pi f24, f31, f24\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 512(x18)\n"
			"flw.ps f19, 160(x18)\n"
			"fmin.pi f23, f2, f19\n" 
			"mova.x.m x7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 576(x28)\n"
			"flw.ps f31, 768(x28)\n"
			"fle.ps f13, f0, f31\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_10:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 960(x10)\n"
			"fexp.ps f13, f13\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f28, 672(x11)\n"
			"fcvt.ps.f16 f25, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 160(x5)\n"
			"flw.ps f28, 672(x5)\n"
			"fmaxu.pi f24, f10, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 448(x14)\n"
			"flw.ps f21, 736(x14)\n"
			"fminu.pi f10, f30, f21\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 512(x14)\n"
			"flw.ps f21, 416(x14)\n"
			"fsll.pi f31, f23, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 704(x11)\n"
			"flw.ps f23, 512(x11)\n"
			"flt.ps f10, f0, f23\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f2, 864(x18)\n"
			"fcvt.ps.f16 f3, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 448(x7)\n"
			"flw.ps f31, 0(x7)\n"
			"flw.ps f23, 448(x7)\n"
			"fmsub.s f0, f25, f31, f23, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_455314_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_149511_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x9 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1a << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xf << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_371284_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x0 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xa << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1f << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xf << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x47 << TENSOR_FMA32_SCP_LOC_B) | (0x60 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_10:\n"
			"flem.ps m4, f2, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f29, 32(x11)\n"
			"fcvt.ps.pw f24, f29, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 800(x19)\n"
			"flw.ps f10, 640(x19)\n"
			"feq.s x22, f28, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 352(x7)\n"
			"frsq.ps f19, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 256(x28)\n"
			"flw.ps f19, 672(x28)\n"
			"flw.ps f23, 64(x28)\n"
			"fmadd.s f21, f31, f19, f23, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 128(x18)\n"
			"flw.ps f31, 768(x18)\n"
			"flt.s x11, f1, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 128(x18)\n"
			"flt.s x20, f19, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 384(x19)\n"
			"flw.ps f12, 576(x19)\n"
			"fsgnjx.ps f1, f29, f12\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_10:\n"
			"fltm.pi m5, f29, f0\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_int_32\n"
			"flw.ps f24, 352(x11)\n"
			"flw.ps f25, 288(x11)\n"
			"fsra.pi f25, f24, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 128(x14)\n"
			"fsatu8.pi f28, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 992(x10)\n"
			"flw.ps f23, 768(x10)\n"
			"fmaxu.pi f1, f30, f23\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 448(x22)\n"
			"fnot.pi f28, f25\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 832(x10)\n"
			"flw.ps f3, 256(x10)\n"
			"flw.ps f19, 128(x10)\n"
			"fnmsub.ps f25, f24, f3, f19, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 672(x14)\n"
			"flw.ps f3, 416(x14)\n"
			"flt.ps f28, f25, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 288(x5)\n"
			"fcvt.w.s x28, f29, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x3 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x3 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1f << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xe << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 128(x20)\n"
			"fsqrt.s f29, f21, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 320(x10)\n"
			"flw.ps f13, 320(x10)\n"
			"feq.pi f19, f2, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 704(x28)\n"
			"flw.ps f19, 192(x28)\n"
			"feq.ps f0, f13, f19\n" 
			"li x5, 0xb9f8\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f19, 640(x28)\n"
			"flw.ps f12, 800(x28)\n"
			"flt.pi f10, f19, f12\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 448(x14)\n"
			"flw.ps f23, 960(x14)\n"
			"fxor.pi f10, f28, f23\n" 
			"la x5, rand_ieee754_16\n"
			"flw.ps f29, 64(x5)\n"
			"fcvt.ps.f16 f24, f29\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 960(x20)\n"
			"flw.ps f1, 640(x20)\n"
			"flt.pi f0, f13, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 672(x19)\n"
			"flw.ps f25, 768(x19)\n"
			"fsgnj.s f0, f10, f25\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 576(x11)\n"
			"flw.ps f2, 288(x11)\n"
			"fcmovm.ps f21, f3, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 224(x10)\n"
			"frsq.ps f19, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 448(x9)\n"
			"frsq.ps f2, f10\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_10:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 448(x5)\n"
			"fexp.ps f19, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 800(x20)\n"
			"flw.ps f1, 672(x20)\n"
			"fsgnj.ps f21, f23, f1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 992(x7)\n"
			"flw.ps f25, 768(x7)\n"
			"fmul.ps f24, f19, f25, rdn\n" 
			"li x5, 0xbe4833da8e8ae1e0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x54f5eda174975300\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_10:\n"
			"maskand m3, m5, m7\n" 
			"li x5, 0x803401D0\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 64(x5)\n"
			"flw.ps f3, 800(x5)\n"
			"feq.ps f23, f28, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_594875_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf4 << TENSOR_FMA16_SCP_LOC_B) | (0xcc << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x6 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x5 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x4 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x6 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xd << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1d << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 192(x5)\n"
			"fcvt.pw.ps f10, f0, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 512(x20)\n"
			"fslli.pi f21, f10, 0xc\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 32(x5)\n"
			"flw.ps f25, 800(x5)\n"
			"fmin.s f3, f28, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 192(x11)\n"
			"flw.ps f23, 992(x11)\n"
			"for.pi f1, f1, f23\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_10:\n"
			"maskxor m3, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 416(x22)\n"
			"flw.ps f10, 640(x22)\n"
			"fmadd.s f30, f19, f10, f10, rtz\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_10:\n"
			"fltm.ps m5, f13, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 96(x22)\n"
			"flw.ps f28, 576(x22)\n"
			"fmax.ps f1, f31, f28\n" 
			"li x5, 0x513\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f28, 0x6ca13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 32(x5)\n"
			"fclass.ps f28, f13\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 416(x5)\n"
			"fsatu8.pi f1, f30\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 96(x9)\n"
			"flw.ps f13, 32(x9)\n"
			"fmul.ps f24, f12, f13, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 768(x22)\n"
			"fcvt.wu.s x20, f28, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 544(x11)\n"
			"fcvt.ps.pw f24, f1, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 480(x9)\n"
			"fcvt.w.s x9, f29, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f0, 192(x10)\n"
			"flw.ps f29, 800(x10)\n"
			"fsra.pi f12, f0, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_681591_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x0 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xb << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1a << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xb << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 11 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 704(x27)\n"
			"flw.ps f5, 640(x27)\n"
			"fle.ps f20, f1, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 992(x12)\n"
			"flw.ps f6, 96(x12)\n"
			"fadd.pi f3, f17, f6\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 384(x27)\n"
			"fsatu8.pi f7, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 0(x23)\n"
			"flw.ps f19, 288(x23)\n"
			"fmin.s f13, f7, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"feqm.ps m1, f25, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f25, 832(x10)\n"
			"fslli.pi f20, f25, 0x9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 576(x27)\n"
			"fcvt.ps.pwu f19, f5, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 672(x12)\n"
			"flw.ps f14, 768(x12)\n"
			"fmul.pi f3, f8, f14\n" 
			"mova.x.m x10\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 32(x27)\n"
			"flw.ps f2, 224(x27)\n"
			"flw.ps f6, 0(x27)\n"
			"fnmsub.s f20, f17, f2, f6, rmm\n" 
			"maskpopcz x14, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x14, rand_int_32\n"
			"flw.ps f13, 672(x14)\n"
			"flw.ps f4, 544(x14)\n"
			"fsub.pi f13, f13, f4\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 992(x23)\n"
			"flw.ps f6, 320(x23)\n"
			"fsgnj.s f8, f19, f6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 512(x19)\n"
			"flw.ps f14, 672(x19)\n"
			"fsgnjn.ps f19, f1, f14\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 352(x10)\n"
			"fcvt.w.s x25, f5, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskand m1, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_11:\n"
			"masknot m1, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x80b47515f8\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 768(x27)\n"
			"flw.ps f1, 224(x27)\n"
			"fle.s x10, f25, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 96(x27)\n"
			"fpackreph.pi f6, f5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 352(x6)\n"
			"fpackreph.pi f13, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 960(x25)\n"
			"flw.ps f3, 352(x25)\n"
			"fmul.ps f7, f25, f3, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 704(x14)\n"
			"fsrai.pi f3, f25, 0x8\n" 
			"fbci.pi f3, 0x5a01f\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 960(x12)\n"
			"flw.ps f2, 512(x12)\n"
			"feq.ps f19, f17, f2\n" 
			"fbci.ps f1, 0x41447\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 928(x27)\n"
			"flw.ps f8, 640(x27)\n"
			"fmul.ps f24, f6, f8, rup\n" 
			"fbci.pi f8, 0x3109d\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 608(x10)\n"
			"flw.ps f1, 832(x10)\n"
			"fle.s x19, f27, f1\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f8, 992(x19)\n"
			"fcvt.ps.f16 f13, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 448(x23)\n"
			"flw.ps f3, 704(x23)\n"
			"fcmovm.ps f5, f20, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 448(x10)\n"
			"flw.ps f13, 704(x10)\n"
			"flw.ps f3, 192(x10)\n"
			"fmadd.s f24, f4, f13, f3, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 352(x14)\n"
			"flw.ps f4, 800(x14)\n"
			"fadd.s f6, f1, f4, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 640(x26)\n"
			"flw.ps f1, 832(x26)\n"
			"fsgnjn.s f27, f7, f1\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 512(x6)\n"
			"flw.ps f25, 544(x6)\n"
			"fltu.pi f5, f4, f25\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 672(x10)\n"
			"fsrli.pi f19, f13, 0x8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 192(x27)\n"
			"fsrli.pi f7, f8, 0x3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 192(x14)\n"
			"fcvt.f16.ps f19, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 128(x23)\n"
			"flw.ps f27, 192(x23)\n"
			"fle.ps f17, f20, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 960(x14)\n"
			"fpackrepb.pi f6, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 736(x25)\n"
			"flw.ps f1, 32(x25)\n"
			"fdiv.s f14, f7, f1, rtz\n" 
			"mova.x.m x27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f2, 672(x12)\n"
			"flw.ps f14, 608(x12)\n"
			"fminu.pi f19, f2, f14\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 32(x27)\n"
			"flw.ps f19, 224(x27)\n"
			"fmaxu.pi f27, f5, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 384(x15)\n"
			"flw.ps f24, 800(x15)\n"
			"fand.pi f7, f27, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 576(x14)\n"
			"fxor.pi f2, f17, f17\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 640(x19)\n"
			"fnot.pi f5, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 0(x26)\n"
			"flw.ps f17, 96(x26)\n"
			"flt.s x20, f13, f17\n" 
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
			"flw.ps f14, 128(x6)\n"
			"fpackrepb.pi f25, f14\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 896(x12)\n"
			"fsat8.pi f2, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 224(x23)\n"
			"frsq.ps f7, f5\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f13, 544(x19)\n"
			"fcvt.ps.f16 f1, f13\n" 
			"fbci.ps f27, 0x201e7\n" 
			"LBL_SEQID_3_FP_TRANS_ID_4_HID_11:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 544(x25)\n"
			"flog.ps f6, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 160(x14)\n"
			"ffrc.ps f7, f25\n" 
			"la x25, rand_int_32\n"
			"flw.ps f25, 800(x25)\n"
			"fpackrepb.pi f14, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 256(x15)\n"
			"flw.ps f19, 224(x15)\n"
			"flw.ps f27, 864(x15)\n"
			"fnmsub.ps f8, f17, f19, f27, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f14, 512(x20)\n"
			"flw.ps f19, 160(x20)\n"
			"fltu.pi f1, f14, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 608(x12)\n"
			"fswizz.ps f3, f7, 0x13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 768(x19)\n"
			"flw.ps f20, 512(x19)\n"
			"fle.pi f19, f7, f20\n" 
			"li x25, 0xbff8857192c8f563\n"
			"fcvt.s.l f19, x25, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 320(x23)\n"
			"flw.ps f1, 736(x23)\n"
			"fxor.pi f1, f6, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 416(x20)\n"
			"flw.ps f5, 864(x20)\n"
			"fmul.ps f20, f24, f5, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 800(x14)\n"
			"flw.ps f6, 608(x14)\n"
			"fmin.ps f19, f4, f6\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 0(x6)\n"
			"fswizz.ps f7, f20, 0x3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f14, 928(x15)\n"
			"fslli.pi f24, f14, 0xc\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 320(x12)\n"
			"fpackreph.pi f7, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 192(x23)\n"
			"faddi.pi f1, f7, 0x1e7\n" 
			"fbci.ps f1, 0x26c8a\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 864(x25)\n"
			"flw.ps f5, 896(x25)\n"
			"flw.ps f25, 896(x25)\n"
			"fcmov.ps f1, f24, f5, f25\n" 
			"li x20, 0xae45ad84580e773a\n"
			"fcvt.s.w f17, x20, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 192(x12)\n"
			"flw.ps f24, 896(x12)\n"
			"flw.ps f1, 512(x12)\n"
			"fnmadd.ps f3, f8, f24, f1, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 384(x6)\n"
			"flw.ps f14, 0(x6)\n"
			"fmulh.pi f1, f27, f14\n" 
			"la x12, rand_int_32\n"
			"flw.ps f25, 224(x12)\n"
			"fsrli.pi f25, f25, 0x8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_11:\n"
			"masknot m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 96(x23)\n"
			"fswizz.ps f2, f13, 0x1f\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 96(x25)\n"
			"fclass.s x23, f20\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_11:\n"
			"fltm.pi m1, f8, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f8, 768(x20)\n"
			"flw.ps f2, 320(x20)\n"
			"fmaxu.pi f5, f8, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 928(x20)\n"
			"flw.ps f8, 768(x20)\n"
			"fmin.s f13, f7, f8\n" 
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
			"la x23, rand_int_32\n"
			"flw.ps f6, 512(x23)\n"
			"fsrli.pi f2, f6, 0xe\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 768(x27)\n"
			"flw.ps f3, 832(x27)\n"
			"flw.ps f4, 224(x27)\n"
			"fnmsub.ps f8, f5, f3, f4, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 800(x23)\n"
			"flw.ps f4, 544(x23)\n"
			"flw.ps f1, 640(x23)\n"
			"fmadd.s f17, f5, f4, f1, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_11:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f2, 960(x12)\n"
			"flw.ps f1, 352(x12)\n"
			"fmaxu.pi f20, f2, f1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f1, 832(x23)\n"
			"fnot.pi f6, f1\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_11:\n"
			"maskxor m0, m7, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x7724bfaf57\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x23\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 480(x20)\n"
			"fcvt.pw.ps f5, f27, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 480(x12)\n"
			"flw.ps f13, 768(x12)\n"
			"feq.s x25, f17, f13\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 864(x12)\n"
			"feq.s x14, f6, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 288(x10)\n"
			"fsqrt.ps f17, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 128(x14)\n"
			"flw.ps f7, 288(x14)\n"
			"fmul.s f4, f27, f7, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 448(x12)\n"
			"flw.ps f17, 128(x12)\n"
			"fsll.pi f19, f13, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 384(x6)\n"
			"flw.ps f5, 160(x6)\n"
			"fmul.pi f13, f8, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 704(x19)\n"
			"flw.ps f14, 320(x19)\n"
			"fmadd.ps f7, f7, f14, f14, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 12 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 32(x27)\n"
			"flw.ps f30, 288(x27)\n"
			"flw.ps f13, 512(x27)\n"
			"fcmov.ps f2, f1, f30, f13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 352(x5)\n"
			"flw.ps f4, 160(x5)\n"
			"fsgnjn.ps f9, f21, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 640(x12)\n"
			"fsin.ps f21, f21\n" 
			"li x5, 0x6caf\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xbca335e169\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f9, 800(x12)\n"
			"flw.ps f31, 320(x12)\n"
			"fmulh.pi f18, f9, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 352(x5)\n"
			"flw.ps f4, 896(x5)\n"
			"fsub.ps f14, f22, f4, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 224(x12)\n"
			"fsrai.pi f2, f9, 0x3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 0(x27)\n"
			"flw.ps f25, 224(x27)\n"
			"flw.ps f30, 544(x27)\n"
			"fnmsub.s f14, f2, f25, f30, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 800(x15)\n"
			"ffrc.ps f1, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 672(x15)\n"
			"flw.ps f13, 832(x15)\n"
			"flw.ps f1, 832(x15)\n"
			"fnmadd.s f14, f25, f13, f1, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 192(x10)\n"
			"fcvt.f16.ps f2, f12\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 384(x5)\n"
			"flw.ps f14, 576(x5)\n"
			"fadd.pi f20, f13, f14\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 256(x20)\n"
			"fslli.pi f27, f30, 0x1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 672(x22)\n"
			"fsin.ps f21, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 768(x22)\n"
			"flw.ps f2, 672(x22)\n"
			"fmul.ps f13, f30, f2, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 416(x12)\n"
			"ffrc.ps f19, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x6 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f19, 736(x5)\n"
			"fand.pi f9, f19, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 928(x14)\n"
			"flw.ps f18, 448(x14)\n"
			"fle.ps f31, f21, f18\n" 
			"fbci.ps f13, 0x3baa0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 416(x14)\n"
			"fcvt.wu.s x12, f21, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 96(x27)\n"
			"fsatu8.pi f9, f20\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 416(x15)\n"
			"flw.ps f4, 288(x15)\n"
			"fsub.ps f25, f27, f4, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_12:\n"
			"maskxor m7, m3, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"flem.ps m3, f13, f2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc85db32ac1\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 0(x25)\n"
			"flw.ps f9, 544(x25)\n"
			"flw.ps f4, 288(x25)\n"
			"fmadd.ps f30, f19, f9, f4, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 736(x15)\n"
			"flw.ps f22, 864(x15)\n"
			"fdiv.pi f27, f9, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 288(x25)\n"
			"flw.ps f31, 480(x25)\n"
			"flw.ps f2, 448(x25)\n"
			"fcmov.ps f31, f27, f31, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 288(x14)\n"
			"flw.ps f31, 800(x14)\n"
			"fsgnjx.s f18, f4, f31\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_12:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 736(x10)\n"
			"fexp.ps f12, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_12:\n"
			"feqm.ps m0, f31, f21\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc4ddb0bd6b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 32(x12)\n"
			"flw.ps f18, 32(x12)\n"
			"fle.s x15, f14, f18\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 672(x18)\n"
			"flw.ps f21, 160(x18)\n"
			"flw.ps f22, 480(x18)\n"
			"fmsub.ps f1, f27, f21, f22, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_342184_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x18 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xc << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_12:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 896(x25)\n"
			"frcp.ps f1, f14\n" 
			"li x5, 0x4a45ecf22a7066ca\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x68a81eb8bf8dd18d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 672(x9)\n"
			"fclass.ps f19, f4\n" 
			"li x5, 0x28bb\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f1, 128(x10)\n"
			"flw.ps f20, 800(x10)\n"
			"fsrl.pi f1, f1, f20\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 416(x9)\n"
			"flw.ps f1, 32(x9)\n"
			"fmulhu.pi f4, f22, f1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 384(x22)\n"
			"flw.ps f25, 352(x22)\n"
			"fsgnjn.ps f19, f18, f25\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 640(x18)\n"
			"flw.ps f18, 864(x18)\n"
			"flt.s x14, f4, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 224(x5)\n"
			"frsq.ps f18, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 640(x20)\n"
			"fswizz.ps f13, f30, 0x1e\n" 
			"la x22, rand_int_32\n"
			"flw.ps f21, 256(x22)\n"
			"flw.ps f12, 288(x22)\n"
			"fsrl.pi f20, f21, f12\n" 
			"li x5, 0x199f\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 704(x10)\n"
			"fround.ps f9, f9, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 32(x12)\n"
			"fcvt.wu.s x15, f25, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 800(x25)\n"
			"flw.ps f9, 800(x25)\n"
			"fsub.s f31, f27, f9, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_12:\n"
			"feqm.ps m6, f25, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 832(x14)\n"
			"fclass.ps f22, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f30, 512(x25)\n"
			"fandi.pi f12, f30, 0x76\n" 
			"li x20, 0x6ed824111e5e68ee\n"
			"fcvt.s.wu f25, x20, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x7 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x9 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x2 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
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
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_12:\n"
			"mov.m.x m3, x27, 0x57\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4d3dfe7478\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xf13\n"
			"csrw tensor_mask, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f14, 128(x14)\n"
			"flw.ps f19, 320(x14)\n"
			"fadd.pi f19, f14, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 352(x22)\n"
			"flw.ps f12, 896(x22)\n"
			"fsra.pi f20, f14, f12\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 768(x22)\n"
			"fslli.pi f13, f18, 0x4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 256(x15)\n"
			"fsrai.pi f4, f31, 0xc\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 352(x20)\n"
			"flw.ps f4, 192(x20)\n"
			"fsub.ps f13, f1, f4, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 992(x12)\n"
			"fpackrepb.pi f9, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 576(x27)\n"
			"flw.ps f19, 832(x27)\n"
			"fmax.s f13, f4, f19\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_12:\n"
			"mov.m.x m6, x10, 0x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f9, 384(x10)\n"
			"flw.ps f25, 448(x10)\n"
			"fmulhu.pi f25, f9, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 160(x27)\n"
			"fsin.ps f1, f4\n" 
			"li x5, 0x60cfc8f01333ffa2\n"
			"fcvt.s.w f1, x5, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 224(x27)\n"
			"fclass.s x25, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 32(x18)\n"
			"flw.ps f18, 64(x18)\n"
			"fltu.pi f22, f20, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_12:\n"
			"fltm.pi m0, f9, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f12, 960(x9)\n"
			"flw.ps f20, 736(x9)\n"
			"feq.pi f13, f12, f20\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x3 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xa << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x19 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xb << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x7 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x9 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xf << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1c << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xa << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 512(x12)\n"
			"flw.ps f2, 288(x12)\n"
			"fle.s x10, f25, f2\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 640(x27)\n"
			"flw.ps f1, 640(x27)\n"
			"flt.pi f30, f21, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 384(x18)\n"
			"flw.ps f2, 768(x18)\n"
			"flw.ps f31, 192(x18)\n"
			"fmadd.ps f31, f18, f2, f31, rne\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_12:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 576(x20)\n"
			"frcp.ps f31, f22\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_12:\n"
			"maskxor m7, m7, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x44c2a619a0\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f18, 672(x15)\n"
			"fsrli.pi f27, f18, 0xe\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_12:\n"
			"maskxor m7, m3, m7\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 608(x22)\n"
			"flw.ps f30, 256(x22)\n"
			"fcmov.ps f12, f18, f18, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 512(x5)\n"
			"flw.ps f14, 256(x5)\n"
			"fadd.ps f4, f4, f14, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 576(x14)\n"
			"flw.ps f9, 224(x14)\n"
			"fsgnjx.s f30, f21, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f25, 896(x15)\n"
			"flw.ps f27, 224(x15)\n"
			"fadd.pi f1, f25, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 416(x27)\n"
			"flw.ps f27, 512(x27)\n"
			"fltu.pi f9, f4, f27\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 576(x14)\n"
			"fcvt.pwu.ps f9, f1, rup\n" 
			"li x5, 0xf693\n"
			"csrw tensor_mask, x5\n"	
			"li x12, 0x500b76f61e862121\n"
			"fcvt.s.wu f12, x12, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f2, 128(x22)\n"
			"faddi.pi f18, f2, 0x7f\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 320(x22)\n"
			"flw.ps f14, 864(x22)\n"
			"flw.ps f2, 416(x22)\n"
			"fmadd.ps f20, f1, f14, f2, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x0 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xa << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f9, 992(x14)\n"
			"flw.ps f2, 832(x14)\n"
			"fle.pi f14, f9, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 736(x18)\n"
			"flw.ps f2, 64(x18)\n"
			"fadd.s f1, f20, f2, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 0(x5)\n"
			"flw.ps f12, 544(x5)\n"
			"fminu.pi f12, f19, f12\n" 
			"li x5, 0x2a5d22ba47f4ee14\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x794f1a20ea89850\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f19, 960(x22)\n"
			"fcvt.ps.pw f20, f19, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopcz x10, m6\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 832(x15)\n"
			"fpackreph.pi f13, f4\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 96(x25)\n"
			"fandi.pi f12, f18, 0x83\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 160(x10)\n"
			"flw.ps f25, 448(x10)\n"
			"fxor.pi f1, f31, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f22, 64(x20)\n"
			"fpackreph.pi f9, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 256(x27)\n"
			"flw.ps f20, 480(x27)\n"
			"fcmovm.ps f18, f31, f20\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 544(x12)\n"
			"fmin.s f12, f25, f25\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_12:\n"
			"fltm.pi m7, f30, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 512(x9)\n"
			"flw.ps f13, 608(x9)\n"
			"flw.ps f19, 864(x9)\n"
			"fmadd.s f22, f12, f13, f19, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_12:\n"
			"flem.ps m0, f4, f20\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f21, 416(x15)\n"
			"flw.ps f1, 128(x15)\n"
			"feq.pi f2, f21, f1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 128(x22)\n"
			"flw.ps f22, 96(x22)\n"
			"fmul.ps f21, f20, f22, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xb6 << TENSOR_FMA16_SCP_LOC_B) | (0x85 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 13 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"maskpopcz x9, m4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 864(x23)\n"
			"flw.ps f29, 448(x23)\n"
			"fmulhu.pi f9, f11, f29\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_13:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 384(x17)\n"
			"flog.ps f2, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 128(x18)\n"
			"fnot.pi f22, f13\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 224(x18)\n"
			"fcvt.pwu.ps f9, f31, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 32(x7)\n"
			"flw.ps f31, 480(x7)\n"
			"fmax.ps f30, f14, f31\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 864(x18)\n"
			"fandi.pi f23, f15, 0x30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 256(x16)\n"
			"flw.ps f17, 448(x16)\n"
			"flt.s x17, f28, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 224(x12)\n"
			"flw.ps f0, 224(x12)\n"
			"flt.s x28, f22, f0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 928(x5)\n"
			"flw.ps f9, 224(x5)\n"
			"fsgnj.s f17, f0, f9\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 128(x17)\n"
			"fandi.pi f14, f22, 0x1d0\n" 
			"fbci.ps f17, 0x64cc7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 480(x9)\n"
			"flw.ps f13, 384(x9)\n"
			"fcmovm.ps f31, f17, f13\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"masknot m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"maskor m4, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 192(x28)\n"
			"flw.ps f15, 416(x28)\n"
			"fnmadd.ps f2, f30, f30, f15, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 672(x16)\n"
			"flw.ps f28, 96(x16)\n"
			"fsgnj.s f5, f0, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 960(x6)\n"
			"flw.ps f9, 832(x6)\n"
			"fmin.s f0, f2, f9\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 320(x18)\n"
			"fslli.pi f0, f9, 0xa\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 960(x5)\n"
			"flw.ps f24, 128(x5)\n"
			"fsll.pi f5, f29, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 224(x6)\n"
			"flw.ps f30, 512(x6)\n"
			"fmin.s f14, f29, f30\n" 
			"li x18, 0xaa83c066447a15eb\n"
			"fcvt.s.wu f13, x18, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 672(x18)\n"
			"fsatu8.pi f24, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 96(x9)\n"
			"flw.ps f22, 96(x9)\n"
			"feq.s x7, f30, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"fltm.pi m2, f29, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"mova.m.x x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 864(x6)\n"
			"flw.ps f11, 960(x6)\n"
			"fdiv.ps f2, f28, f11, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 960(x7)\n"
			"flw.ps f28, 128(x7)\n"
			"flw.ps f30, 896(x7)\n"
			"fnmsub.s f22, f0, f28, f30, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 352(x18)\n"
			"flw.ps f17, 384(x18)\n"
			"flt.pi f11, f9, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"fltm.ps m2, f22, f17\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xebb9ec81aa\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 480(x6)\n"
			"fcvt.pwu.ps f2, f23, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 96(x20)\n"
			"fpackreph.pi f17, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 992(x20)\n"
			"flw.ps f17, 352(x20)\n"
			"flw.ps f15, 800(x20)\n"
			"fmsub.s f14, f14, f17, f15, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 384(x16)\n"
			"flw.ps f13, 896(x16)\n"
			"fle.s x28, f22, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f30, 864(x18)\n"
			"flw.ps f29, 544(x18)\n"
			"fmax.pi f2, f30, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 96(x23)\n"
			"flw.ps f29, 736(x23)\n"
			"fsgnjn.s f0, f22, f29\n" 
			"fbci.ps f11, 0x2d1f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 864(x12)\n"
			"flw.ps f29, 896(x12)\n"
			"flw.ps f17, 384(x12)\n"
			"fnmsub.ps f11, f13, f29, f17, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 192(x16)\n"
			"flw.ps f29, 832(x16)\n"
			"fsgnj.s f22, f31, f29\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 32(x12)\n"
			"flw.ps f9, 128(x12)\n"
			"fminu.pi f24, f0, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 736(x23)\n"
			"fpackrepb.pi f22, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 256(x28)\n"
			"flw.ps f9, 960(x28)\n"
			"fmul.s f23, f13, f9, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 832(x17)\n"
			"flw.ps f2, 288(x17)\n"
			"fsgnjn.s f29, f17, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 160(x28)\n"
			"flw.ps f5, 768(x28)\n"
			"flw.ps f31, 128(x28)\n"
			"fmadd.s f24, f9, f5, f31, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 96(x17)\n"
			"faddi.pi f28, f11, 0x130\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 416(x17)\n"
			"frsq.ps f23, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f11, 64(x6)\n"
			"flw.ps f22, 384(x6)\n"
			"fmul.pi f13, f11, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 928(x12)\n"
			"fsrli.pi f31, f0, 0x7\n" 
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
			"la x9, rand_int_32\n"
			"flw.ps f13, 128(x9)\n"
			"fsat8.pi f14, f13\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 352(x18)\n"
			"fcvt.pw.ps f9, f9, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_13:\n"
			"fltm.pi m4, f30, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 960(x20)\n"
			"fswizz.ps f29, f0, 0x1a\n" 
			"maskpopcz x6, m1\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_13:\n"
			"fltm.pi m1, f9, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f29, 448(x18)\n"
			"flw.ps f2, 960(x18)\n"
			"fle.pi f28, f29, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 896(x23)\n"
			"flw.ps f2, 160(x23)\n"
			"fxor.pi f14, f31, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 608(x20)\n"
			"flw.ps f23, 608(x20)\n"
			"fxor.pi f5, f11, f23\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 672(x18)\n"
			"fpackrepb.pi f23, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 448(x7)\n"
			"frsq.ps f9, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_13:\n"
			"fltm.pi m4, f0, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f28, 64(x16)\n"
			"flw.ps f0, 672(x16)\n"
			"fdiv.pi f9, f28, f0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 992(x28)\n"
			"flw.ps f11, 32(x28)\n"
			"fmin.pi f24, f24, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 960(x5)\n"
			"flw.ps f5, 896(x5)\n"
			"flw.ps f24, 352(x5)\n"
			"fnmsub.s f15, f13, f5, f24, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 288(x6)\n"
			"flw.ps f24, 544(x6)\n"
			"fxor.pi f0, f29, f24\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 96(x6)\n"
			"flw.ps f22, 64(x6)\n"
			"flw.ps f23, 736(x6)\n"
			"fmsub.ps f23, f28, f22, f23, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 736(x7)\n"
			"fcvt.lu.s x28, f15, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 512(x12)\n"
			"flw.ps f23, 800(x12)\n"
			"flt.pi f23, f31, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 224(x7)\n"
			"flw.ps f15, 928(x7)\n"
			"fmul.s f31, f23, f15, rtz\n" 
			"fbci.ps f15, 0x6ab50\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 928(x18)\n"
			"flw.ps f0, 576(x18)\n"
			"fdiv.s f13, f9, f0, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 768(x9)\n"
			"flw.ps f9, 256(x9)\n"
			"feq.ps f24, f22, f9\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_13:\n"
			"fltm.pi m4, f9, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f14, 704(x6)\n"
			"fnot.pi f17, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 736(x23)\n"
			"flw.ps f22, 128(x23)\n"
			"fsub.pi f23, f11, f22\n" 
			"maskpopc x5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 800(x9)\n"
			"flw.ps f23, 416(x9)\n"
			"fcmovm.ps f22, f11, f23\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_13:\n"
			"mova.m.x x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 640(x18)\n"
			"frsq.ps f28, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 352(x23)\n"
			"fmin.pi f31, f30, f30\n" 
			"fbci.ps f31, 0x7f571\n" 
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
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_13:\n"
			"fsetm.pi m2, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 960(x6)\n"
			"feq.ps f9, f23, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 800(x28)\n"
			"flw.ps f29, 256(x28)\n"
			"feq.pi f9, f17, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 672(x5)\n"
			"fandi.pi f5, f13, 0x139\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 256(x5)\n"
			"fcvt.wu.s x12, f30, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 992(x6)\n"
			"fclass.ps f13, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 992(x12)\n"
			"flw.ps f9, 640(x12)\n"
			"feq.s x17, f31, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 960(x28)\n"
			"flw.ps f11, 480(x28)\n"
			"fxor.pi f13, f24, f11\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 736(x18)\n"
			"fcvt.wu.s x18, f11, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 320(x12)\n"
			"flw.ps f5, 928(x12)\n"
			"feq.ps f11, f31, f5\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 640(x12)\n"
			"flw.ps f15, 864(x12)\n"
			"fmax.ps f2, f2, f15\n" 
			"li x5, 0x9595ec8e9a55d50e\n"
			"fcvt.s.w f28, x5, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 32(x9)\n"
			"flw.ps f2, 896(x9)\n"
			"fadd.ps f13, f13, f2, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 0(x5)\n"
			"flw.ps f24, 224(x5)\n"
			"fmax.s f11, f11, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 832(x17)\n"
			"flw.ps f17, 832(x17)\n"
			"flw.ps f11, 768(x17)\n"
			"fmadd.s f30, f31, f17, f11, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 352(x20)\n"
			"fcvt.lu.s x7, f29, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 14 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 17\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
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
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 704(x11)\n"
			"flw.ps f22, 992(x11)\n"
			"flt.s x28, f29, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 832(x10)\n"
			"flw.ps f22, 864(x10)\n"
			"fmax.ps f15, f19, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 352(x27)\n"
			"fcvt.wu.s x5, f17, rdn\n" 
			"fbci.ps f0, 0x23b69\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 352(x28)\n"
			"fswizz.ps f18, f14, 0x2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 320(x11)\n"
			"flw.ps f9, 224(x11)\n"
			"fdiv.ps f17, f3, f9, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 160(x19)\n"
			"flw.ps f29, 768(x19)\n"
			"fadd.pi f18, f2, f29\n" 
			"fbci.pi f17, 0x4ee6a\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 704(x14)\n"
			"fexp.ps f15, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 448(x14)\n"
			"fcvt.wu.s x27, f0, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f2, 288(x11)\n"
			"fcvt.ps.pwu f29, f2, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 256(x14)\n"
			"fcvt.pw.ps f12, f25, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 256(x5)\n"
			"flw.ps f2, 32(x5)\n"
			"fmul.s f27, f13, f2, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 800(x19)\n"
			"fclass.ps f29, f14\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 864(x10)\n"
			"frsq.ps f22, f9\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 960(x14)\n"
			"flw.ps f27, 736(x14)\n"
			"fminu.pi f9, f9, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x372\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe6 << TENSOR_FMA32_SCP_LOC_B) | (0x2d << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xe4 << TENSOR_IMA_SCP_LOC_B) | (0x6b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 800(x15)\n"
			"fclass.s x22, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 352(x6)\n"
			"fcvt.l.s x10, f15, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 480(x10)\n"
			"flw.ps f9, 384(x10)\n"
			"flw.ps f18, 352(x10)\n"
			"fnmsub.ps f2, f0, f9, f18, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 832(x6)\n"
			"flw.ps f22, 256(x6)\n"
			"fmul.s f3, f18, f22, rne\n" 
			"fbci.pi f3, 0x558c\n" 
			"mova.x.m x14\n" 
			"li x22, 0x17c36cd4dcfdc3b1\n"
			"fcvt.s.wu f19, x22, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 96(x6)\n"
			"fcvt.wu.s x19, f29, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 864(x10)\n"
			"flw.ps f9, 896(x10)\n"
			"fmax.pi f27, f2, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 320(x19)\n"
			"faddi.pi f9, f9, 0x14b\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 608(x5)\n"
			"flw.ps f22, 352(x5)\n"
			"fmax.pi f27, f19, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_14:\n"
			"fltm.ps m6, f25, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 128(x6)\n"
			"flw.ps f22, 64(x6)\n"
			"flw.ps f0, 32(x6)\n"
			"fnmadd.s f2, f25, f22, f0, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 800(x5)\n"
			"flw.ps f0, 64(x5)\n"
			"fsgnjn.s f29, f25, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xcaf05ee6df\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f1, 672(x15)\n"
			"flw.ps f9, 512(x15)\n"
			"for.pi f19, f1, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_409815_HID_14:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5e << TENSOR_FMA16_SCP_LOC_B) | (0x4b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_14:\n"
			"maskxor m0, m4, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x28, m7\n" 
			"li x15, 0xf30344d0d1283635\n"
			"fcvt.s.wu f18, x15, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 512(x15)\n"
			"fcvt.wu.s x27, f18, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 960(x22)\n"
			"flw.ps f19, 384(x22)\n"
			"flt.pi f9, f12, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_16\n"
			"flw.ps f15, 32(x11)\n"
			"fcvt.ps.f16 f3, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 192(x27)\n"
			"flw.ps f3, 672(x27)\n"
			"flt.ps f15, f2, f3\n" 
			"li x15, 0xf5cea4d3475acf7a\n"
			"fcvt.s.w f1, x15, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 480(x22)\n"
			"flw.ps f13, 224(x22)\n"
			"fminu.pi f14, f29, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 512(x6)\n"
			"flw.ps f13, 608(x6)\n"
			"fmulhu.pi f2, f15, f13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f15, 608(x19)\n"
			"flw.ps f22, 640(x19)\n"
			"fmulhu.pi f14, f15, f22\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 256(x10)\n"
			"flw.ps f0, 256(x10)\n"
			"fmulh.pi f14, f17, f0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 0(x22)\n"
			"flw.ps f9, 704(x22)\n"
			"flw.ps f13, 32(x22)\n"
			"fnmsub.ps f22, f25, f9, f13, rup\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_14:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 256(x10)\n"
			"fexp.ps f27, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 672(x28)\n"
			"flw.ps f29, 192(x28)\n"
			"fmin.ps f17, f14, f29\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 96(x6)\n"
			"fcvt.wu.s x28, f27, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xb << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1d << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x0 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f3, 928(x10)\n"
			"faddi.pi f22, f3, 0x4f\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 960(x15)\n"
			"fcvt.w.s x19, f13, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 352(x19)\n"
			"fround.ps f25, f9, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 512(x11)\n"
			"fround.ps f3, f25, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 928(x28)\n"
			"flw.ps f3, 608(x28)\n"
			"flw.ps f17, 128(x28)\n"
			"fmsub.s f18, f27, f3, f17, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 512(x6)\n"
			"flw.ps f13, 480(x6)\n"
			"fand.pi f0, f17, f13\n" 
			"mova.x.m x28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 448(x19)\n"
			"flw.ps f15, 704(x19)\n"
			"fmax.ps f18, f0, f15\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 0(x19)\n"
			"fsrli.pi f13, f2, 0xa\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 384(x5)\n"
			"fcvt.pwu.ps f14, f18, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 448(x19)\n"
			"flw.ps f2, 288(x19)\n"
			"fminu.pi f13, f25, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f22, 416(x28)\n"
			"flw.ps f17, 672(x28)\n"
			"fsra.pi f18, f22, f17\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 160(x28)\n"
			"fsrai.pi f22, f18, 0x7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 800(x22)\n"
			"flw.ps f14, 992(x22)\n"
			"fsgnjx.ps f27, f17, f14\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_14:\n"
			"maskor m0, m7, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 544(x22)\n"
			"flw.ps f2, 736(x22)\n"
			"flt.ps f22, f22, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xcb << TENSOR_FMA32_SCP_LOC_B) | (0x52 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x59 << TENSOR_IMA_SCP_LOC_B) | (0x76 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x10, rand_int_32\n"
			"flw.ps f19, 672(x10)\n"
			"flw.ps f9, 960(x10)\n"
			"fand.pi f15, f19, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 672(x10)\n"
			"flw.ps f29, 64(x10)\n"
			"fcmovm.ps f19, f9, f29\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 832(x11)\n"
			"flw.ps f29, 352(x11)\n"
			"fand.pi f14, f19, f29\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_14:\n"
			"maskand m0, m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f27, 512(x11)\n"
			"fpackreph.pi f15, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 704(x14)\n"
			"flw.ps f3, 32(x14)\n"
			"fltu.pi f1, f0, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 864(x22)\n"
			"flw.ps f2, 0(x22)\n"
			"feq.s x11, f18, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 160(x15)\n"
			"fcvt.pwu.ps f14, f12, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 480(x5)\n"
			"ffrc.ps f25, f27\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_14:\n"
			"fsetm.pi m7, f17\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x28, rand_int_32\n"
			"flw.ps f19, 640(x28)\n"
			"flw.ps f0, 608(x28)\n"
			"fsrl.pi f2, f19, f0\n" 
			"li x5, 0xc39a\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f0, 128(x22)\n"
			"flw.ps f25, 512(x22)\n"
			"fminu.pi f12, f0, f25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 416(x5)\n"
			"flw.ps f0, 544(x5)\n"
			"for.pi f29, f27, f0\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_14:\n"
			"flem.ps m0, f15, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f25, 992(x11)\n"
			"fsra.pi f9, f25, f25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f18, 640(x6)\n"
			"flw.ps f19, 352(x6)\n"
			"fltu.pi f17, f18, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x1 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x18 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f27, 544(x27)\n"
			"flw.ps f14, 736(x27)\n"
			"fsub.pi f13, f27, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 160(x22)\n"
			"flw.ps f1, 704(x22)\n"
			"fdiv.s f29, f0, f1, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 672(x28)\n"
			"flw.ps f18, 704(x28)\n"
			"fsub.s f3, f29, f18, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 64(x28)\n"
			"flw.ps f14, 224(x28)\n"
			"fmul.ps f14, f19, f14, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_14:\n"
			"flem.ps m7, f17, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f18, 736(x6)\n"
			"flw.ps f29, 992(x6)\n"
			"fsll.pi f22, f18, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 960(x22)\n"
			"flw.ps f17, 608(x22)\n"
			"fmax.s f19, f19, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 512(x19)\n"
			"flw.ps f12, 96(x19)\n"
			"fle.ps f18, f3, f12\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 64(x22)\n"
			"fcvt.ps.pwu f12, f3, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 608(x15)\n"
			"fsat8.pi f25, f1\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 384(x11)\n"
			"flw.ps f3, 288(x11)\n"
			"for.pi f0, f14, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 960(x27)\n"
			"fclass.s x6, f19\n" 
			"li x5, 0xad5b\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 704(x22)\n"
			"fsin.ps f27, f25\n" 
			"fbci.pi f12, 0x5cacf\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_14:\n"
			"masknot m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc0166c0167\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x15, m4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
	if ( hid == 15 ) { 
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
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 544(x10)\n"
			"fcvt.l.s x13, f18, rtz\n" 
			"mova.x.m x15\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 352(x5)\n"
			"flw.ps f24, 800(x5)\n"
			"flw.ps f5, 352(x5)\n"
			"fnmsub.s f5, f8, f24, f5, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 768(x28)\n"
			"fsub.s f26, f6, f6, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 224(x6)\n"
			"flw.ps f28, 416(x6)\n"
			"fmul.s f24, f13, f28, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f2, 64(x6)\n"
			"fpackrepb.pi f8, f2\n" 
			"maskpopcz x10, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x6, rand_int_32\n"
			"flw.ps f25, 96(x6)\n"
			"fandi.pi f20, f25, 0x16b\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 576(x20)\n"
			"fsgnj.s f26, f31, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 256(x7)\n"
			"flw.ps f31, 960(x7)\n"
			"fmin.s f6, f1, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskxor m3, m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x80340180\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 96(x10)\n"
			"fcvt.pwu.ps f28, f20, rdn\n" 
			"fbci.ps f13, 0x10992\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"mov.m.x m0, x14, 0x60\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 704(x20)\n"
			"flw.ps f31, 896(x20)\n"
			"feq.ps f28, f1, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 32(x5)\n"
			"flw.ps f28, 736(x5)\n"
			"for.pi f25, f25, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 576(x7)\n"
			"fsgnjn.ps f1, f5, f5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 768(x15)\n"
			"flw.ps f6, 704(x15)\n"
			"fsgnj.s f24, f1, f6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 352(x10)\n"
			"flw.ps f20, 96(x10)\n"
			"fxor.pi f2, f1, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 640(x28)\n"
			"flw.ps f25, 128(x28)\n"
			"fnmadd.s f21, f24, f24, f25, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 864(x12)\n"
			"flw.ps f24, 800(x12)\n"
			"feq.s x20, f6, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 448(x15)\n"
			"flw.ps f25, 992(x15)\n"
			"fnmsub.s f3, f2, f2, f25, rdn\n" 
			"fbci.pi f31, 0x1326b\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 416(x13)\n"
			"flw.ps f9, 128(x13)\n"
			"fmin.pi f20, f24, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"maskxor m0, m3, m0\n" 
			"li x5, 0x80340158\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_int_32\n"
			"flw.ps f8, 416(x5)\n"
			"fpackreph.pi f2, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 160(x14)\n"
			"flw.ps f1, 960(x14)\n"
			"fadd.s f1, f20, f1, rdn\n" 
			"li x28, 0x9d86fb09276a5864\n"
			"fcvt.s.wu f31, x28, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"fltm.pi m5, f21, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 160(x13)\n"
			"ffrc.ps f8, f24\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 672(x28)\n"
			"fcvt.ps.pw f3, f5, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 928(x7)\n"
			"fpackreph.pi f21, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 992(x10)\n"
			"fcvt.pwu.ps f9, f9, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 416(x14)\n"
			"ffrc.ps f3, f24\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 352(x15)\n"
			"flw.ps f8, 512(x15)\n"
			"fsra.pi f28, f26, f8\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"fsetm.pi m3, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"maskxor m3, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_15:\n"
			"maskxor m5, m3, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa4a59f564a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f31, 160(x7)\n"
			"flw.ps f18, 768(x7)\n"
			"for.pi f31, f31, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 32(x7)\n"
			"ffrc.ps f18, f3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 224(x28)\n"
			"fsrai.pi f3, f13, 0x7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 704(x5)\n"
			"flw.ps f31, 32(x5)\n"
			"flw.ps f3, 224(x5)\n"
			"fnmadd.ps f21, f25, f31, f3, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 832(x7)\n"
			"fcvt.ps.pwu f28, f18, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_15:\n"
			"fsetm.pi m3, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x7, 0x48bd9b91d3d3c58f\n"
			"fcvt.s.w f18, x7, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 320(x7)\n"
			"flw.ps f1, 128(x7)\n"
			"flw.ps f25, 896(x7)\n"
			"fmadd.ps f21, f9, f1, f25, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 928(x5)\n"
			"flw.ps f1, 864(x5)\n"
			"fdivu.pi f24, f31, f1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 448(x12)\n"
			"flw.ps f25, 736(x12)\n"
			"fmin.ps f8, f31, f25\n" 
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 576(x13)\n"
			"flw.ps f28, 864(x13)\n"
			"fcmovm.ps f2, f1, f28\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_15:\n"
			"maskor m0, m5, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f6, 672(x12)\n"
			"flw.ps f26, 704(x12)\n"
			"for.pi f18, f6, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 128(x20)\n"
			"flt.ps f31, f24, f24\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 992(x10)\n"
			"flw.ps f18, 480(x10)\n"
			"flt.pi f6, f3, f18\n" 
			"mova.x.m x13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 992(x20)\n"
			"flw.ps f28, 256(x20)\n"
			"fand.pi f18, f25, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 576(x5)\n"
			"flw.ps f31, 608(x5)\n"
			"fsgnjx.s f31, f28, f31\n" 
			"fbci.pi f24, 0xf0bd\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_15:\n"
			"feqm.ps m5, f24, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 480(x13)\n"
			"fsqrt.ps f21, f28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 512(x6)\n"
			"fsrai.pi f1, f1, 0x0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 640(x12)\n"
			"fcvt.pwu.ps f24, f28, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 224(x6)\n"
			"flw.ps f24, 864(x6)\n"
			"fle.s x10, f3, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_15:\n"
			"maskor m5, m6, m6\n" 
			"li x5, 0x80340110\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 32(x13)\n"
			"flw.ps f18, 0(x13)\n"
			"fsgnjn.ps f24, f13, f18\n" 
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
			"flw.ps f8, 384(x13)\n"
			"fsat8.pi f26, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 672(x20)\n"
			"flw.ps f21, 928(x20)\n"
			"fmul.ps f28, f20, f21, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 736(x6)\n"
			"flw.ps f3, 128(x6)\n"
			"fmul.s f24, f20, f3, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 384(x6)\n"
			"flw.ps f9, 800(x6)\n"
			"fcmovm.ps f20, f13, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 192(x13)\n"
			"flw.ps f24, 832(x13)\n"
			"feq.s x6, f9, f24\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 672(x6)\n"
			"flw.ps f13, 288(x6)\n"
			"fmul.pi f26, f28, f13\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 832(x12)\n"
			"flw.ps f31, 768(x12)\n"
			"fsgnjx.ps f21, f2, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 992(x5)\n"
			"fnot.pi f25, f26\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 736(x7)\n"
			"fcvt.wu.s x6, f5, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_15:\n"
			"maskor m6, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 96(x13)\n"
			"fsqrt.s f18, f5, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 0(x14)\n"
			"flw.ps f2, 64(x14)\n"
			"fle.s x15, f1, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 896(x6)\n"
			"fround.ps f24, f28, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 640(x6)\n"
			"fsll.pi f5, f13, f13\n" 
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_15:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 128(x6)\n"
			"frcp.ps f13, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 224(x20)\n"
			"flw.ps f31, 544(x20)\n"
			"flw.ps f13, 224(x20)\n"
			"fmsub.s f9, f20, f31, f13, rne\n" 
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
			"flw.ps f2, 896(x15)\n"
			"flw.ps f9, 192(x15)\n"
			"fmin.ps f5, f2, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 352(x15)\n"
			"flw.ps f8, 512(x15)\n"
			"fsgnjx.ps f3, f25, f8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 704(x14)\n"
			"fsrli.pi f13, f21, 0x6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 576(x12)\n"
			"fcvt.w.s x14, f8, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_15:\n"
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 480(x5)\n"
			"flw.ps f24, 256(x5)\n"
			"flw.ps f28, 960(x5)\n"
			"fmsub.ps f18, f25, f24, f28, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 512(x12)\n"
			"fcvt.pwu.ps f9, f1, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 160(x12)\n"
			"flw.ps f8, 32(x12)\n"
			"fand.pi f28, f3, f8\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 96(x15)\n"
			"flw.ps f26, 32(x15)\n"
			"fsgnjx.s f28, f8, f26\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 832(x7)\n"
			"flw.ps f2, 672(x7)\n"
			"fmax.ps f9, f5, f2\n" 
			"fbci.ps f13, 0x7f158\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 32(x7)\n"
			"flw.ps f28, 448(x7)\n"
			"feq.ps f28, f5, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 832(x5)\n"
			"frsq.ps f26, f9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f6, 672(x12)\n"
			"flw.ps f21, 960(x12)\n"
			"fdivu.pi f20, f6, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 736(x15)\n"
			"flw.ps f5, 768(x15)\n"
			"flw.ps f6, 768(x15)\n"
			"fmadd.ps f9, f13, f5, f6, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
		0x0f9f0c74, 
		0x574b87c1, 
		0xc7d6c4db, 
		0x0b731671, 
		0x92f41012, 
		0xaf7a2ff9, 
		0xf8aab7cb, 
		0xf1aa270a, 
		0xbc46f2a9, 
		0x53dca4ee, 
		0xd8c24217, 
		0xd785bc24, 
		0x2c4e8645, 
		0x48a0e8a7, 
		0x6f9f32b2, 
		0x0475dd23, 
		0xe9d9f88c, 
		0xfe9cccbf, 
		0xf8bb935b, 
		0x567b6d50, 
		0xb6474d9c, 
		0x9eda4927, 
		0x90749bed, 
		0xdff2758f, 
		0xe2148416, 
		0x1cab3264, 
		0xcac63fd0, 
		0x89548357, 
		0xd8c2f7e6, 
		0x84c43efd, 
		0xbb4e5166, 
		0x41645dbf, 
		0x2a2b8c87, 
		0x19354a97, 
		0xb3d4decd, 
		0xdb79cd4b, 
		0x8a400fe9, 
		0xe9e6541a, 
		0xb3488495, 
		0x3f933284, 
		0x95f27805, 
		0x6c991593, 
		0x7ebc078c, 
		0xb2ad39fc, 
		0x88c264f5, 
		0x40491864, 
		0xf9082998, 
		0x0db93740, 
		0xec126d95, 
		0x214fa2e9, 
		0x491d4343, 
		0xc7d35188, 
		0xef858842, 
		0x01db7c4c, 
		0x3c6b34aa, 
		0x800c2bcd, 
		0xb2d6c6a0, 
		0xca47c53d, 
		0x9a9b268d, 
		0xd52a0145, 
		0x68d00db3, 
		0xa1e03179, 
		0xda17ca57, 
		0x8ad733dd, 
		0x5632162a, 
		0x76d88684, 
		0xe4b2c552, 
		0x757064cc, 
		0x237b962c, 
		0x0b86d092, 
		0xa973d530, 
		0x8d59a4bf, 
		0xf57f1934, 
		0x90153b34, 
		0xdd94279d, 
		0xbaddf5f9, 
		0x4e2df2b6, 
		0x11ce89b6, 
		0xb3866903, 
		0x65d40adf, 
		0xbb487a8d, 
		0x7ff540c1, 
		0x40b49082, 
		0x46831786, 
		0x423085f1, 
		0xc1fb1347, 
		0x3952f752, 
		0x9f3a5e43, 
		0x6a0c695b, 
		0x935f429a, 
		0xa628924e, 
		0x17fb09f0, 
		0x39f90fd2, 
		0x3fbfed3a, 
		0x6212c13e, 
		0xcf4e2c83, 
		0xe0f859e7, 
		0x2c8c9185, 
		0x7f0ba617, 
		0xce4f5899, 
		0x9449d7b5, 
		0x80b84ebd, 
		0xc70301ea, 
		0xb921786e, 
		0x375429b0, 
		0x43f40e31, 
		0xf4f993c5, 
		0xddd6775d, 
		0x52dd9b16, 
		0xd69dbc21, 
		0x0c1e463f, 
		0x39b0df6b, 
		0xbc9ce1e0, 
		0xe0a22edb, 
		0x7d388888, 
		0x8480b7b0, 
		0x04a2187a, 
		0xda554f6f, 
		0xf66bd75c, 
		0x703e45a8, 
		0x3500473c, 
		0xb3e5c7a4, 
		0xd643263d, 
		0xfd35ebc0, 
		0x30872872, 
		0x338c3592, 
		0x108812c4, 
		0x4db2de44, 
		0xca076ee3, 
		0x3c7211a4, 
		0x23c8f9fb, 
		0x60fb14a5, 
		0xc36dae75, 
		0x5310cc73, 
		0x8ce52570, 
		0x14ff6d9a, 
		0x1ccdb874, 
		0x0f0896c4, 
		0xf1d8b223, 
		0x6f7cec98, 
		0xb486ed53, 
		0xf752e210, 
		0x99ed5c60, 
		0x12b58f42, 
		0x6a6a1988, 
		0xa1e2863e, 
		0xaf887ccf, 
		0xbdfa9a36, 
		0xfde82bd4, 
		0x004c491e, 
		0xd12f5116, 
		0x91935b1f, 
		0xe1bba374, 
		0xcf77dffc, 
		0x5eae3294, 
		0x2a522940, 
		0x91d5c9c7, 
		0xd64aad7a, 
		0x6ef79ab2, 
		0x5b231f94, 
		0x885043a3, 
		0x9d3a5dbe, 
		0xf9510301, 
		0x74bd50cc, 
		0x99dab7a4, 
		0x657b60c6, 
		0x8ae53c7a, 
		0x40629197, 
		0xf932f6b7, 
		0xa470af35, 
		0xa7fffdf6, 
		0x1d58b95e, 
		0x2c02553a, 
		0x2a929fef, 
		0x1418c792, 
		0xe4e2d0da, 
		0xfdbc5745, 
		0xb7f70c53, 
		0x781ca038, 
		0x9af1977d, 
		0xbbaeb73c, 
		0x060a8abb, 
		0x838a3766, 
		0x6e108078, 
		0xd9fcc72a, 
		0x01ccaa73, 
		0xbe644d7d, 
		0x65a7457e, 
		0xd6ed616d, 
		0x1a4bda90, 
		0xc92c6264, 
		0x57961be7, 
		0x8bbb5ff0, 
		0xc2ad42e7, 
		0xa6c291b9, 
		0x6feb0fda, 
		0x588dd036, 
		0x452745ce, 
		0xadaae4a9, 
		0x9d5e766a, 
		0x5cf74990, 
		0x44dad98d, 
		0xecb83aef, 
		0x81515523, 
		0x7ca3bec0, 
		0xf0c81897, 
		0x626d12e6, 
		0x9ac863ef, 
		0x307c6661, 
		0xe2d1a198, 
		0xf3f41011, 
		0x6ae05de2, 
		0xcbd9707e, 
		0xd2290765, 
		0x3dcbc1c2, 
		0xf392e854, 
		0x2a942ce5, 
		0x01bc7852, 
		0x165e80ce, 
		0xc59c6280, 
		0x68021a8f, 
		0xdf887c77, 
		0x29821888, 
		0xb074ce5c, 
		0xd2ce0b9d, 
		0x96aa510c, 
		0x7bdaeea9, 
		0x7a428a42, 
		0xd034e3cf, 
		0xecb4885a, 
		0x01a65826, 
		0x28931ce3, 
		0x43e60483, 
		0x52c46152, 
		0x09bbdd0e, 
		0x12dcf138, 
		0xe9fbc424, 
		0x6c1534fe, 
		0xa01ce130, 
		0xdecbce2b, 
		0xd7be4e37, 
		0x0271d5f2, 
		0x85b03733, 
		0x56c1e203, 
		0x65645fc6, 
		0xbeeefdc2, 
		0xe297d775, 
		0xe180b12f, 
		0xaf3ab7c7, 
		0x6bff3321, 
		0x3e61f4db, 
		0x2fe17dd5, 
		0xeb67ec77, 
		0xa6c14d6c, 
		0xd2f81342, 
		0x8aef727c  
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
		0x5380, 
		0x8000, 
		0x4e00, 
		0x5060, 
		0x5040, 
		0xfc00, 
		0x53e0, 
		0xd260, 
		0x4f00, 
		0x4e40, 
		0xb42b, 
		0x4880, 
		0xa494, 
		0xfc00, 
		0x4d00, 
		0xc3bd, 
		0xd406, 
		0x5000, 
		0x51e0, 
		0xcf80, 
		0x4d40, 
		0xc400, 
		0x5240, 
		0xca00, 
		0x4200, 
		0xce00, 
		0xcf00, 
		0xfd7d, 
		0x5260, 
		0xa8fc, 
		0x4f40, 
		0xc000, 
		0x0000, 
		0x4e80, 
		0xf54c, 
		0x5120, 
		0x01c4, 
		0x5040, 
		0xd0a0, 
		0xc700, 
		0x8852, 
		0x4c80, 
		0xc500, 
		0xbc00, 
		0x78d1, 
		0x4ec0, 
		0xcdc0, 
		0xd1e0, 
		0x53c0, 
		0x5300, 
		0x4900, 
		0x942c, 
		0xd3a0, 
		0xd020, 
		0x4f80, 
		0xff06, 
		0xe571, 
		0xd6bf, 
		0x52e0, 
		0xfcea, 
		0xcc00, 
		0x7c8a, 
		0xcf80, 
		0x50e0, 
		0x52c0, 
		0x7093, 
		0x4200, 
		0xcd00, 
		0xca00, 
		0xfc00, 
		0xd180, 
		0xce80, 
		0x5160, 
		0x8000, 
		0x52c0, 
		0xd180, 
		0x53c0, 
		0xcf40, 
		0x0000, 
		0xd080, 
		0xd2a0, 
		0x4a80, 
		0xc800, 
		0xfa8b, 
		0x577f, 
		0xd320, 
		0x3049, 
		0x232d, 
		0xd180, 
		0x8000, 
		0x8000, 
		0xcdee, 
		0x53e0, 
		0xd040, 
		0xc500, 
		0x012b, 
		0x3c00, 
		0x5ad5, 
		0x7c66, 
		0x5538, 
		0xd260, 
		0xedaf, 
		0x4f00, 
		0x4900, 
		0x8c68, 
		0xc700, 
		0xcf40, 
		0x4f40, 
		0xfc33, 
		0x5100, 
		0x0000, 
		0x8000, 
		0xe7b9, 
		0xc7b7, 
		0xcf40, 
		0x4c40, 
		0x51c0, 
		0xc000, 
		0x8000, 
		0xd020, 
		0x960f, 
		0x4800, 
		0xa5c0, 
		0x52e0, 
		0x4980, 
		0xab0b, 
		0xfdfc, 
		0xb8be, 
		0xd184, 
		0xb978, 
		0xc19b, 
		0x9fb2, 
		0xcf80, 
		0x9b76, 
		0x7c00, 
		0x5180, 
		0xd160, 
		0x69f9, 
		0xf5f7, 
		0x6fc7, 
		0x6890, 
		0xccc0, 
		0xc900, 
		0xd200, 
		0xa9e3, 
		0xc600, 
		0xfc00, 
		0xca80, 
		0xcd80, 
		0x4fc0, 
		0x5300, 
		0xd320, 
		0xd0e0, 
		0x7c00, 
		0x4c80, 
		0xd7e9, 
		0xd200, 
		0xd180, 
		0x5160, 
		0x4600, 
		0xc700, 
		0x5020, 
		0x50a0, 
		0xca80, 
		0xcb00, 
		0xd320, 
		0x0000, 
		0xd220, 
		0x7c00, 
		0x5160, 
		0x5380, 
		0x5340, 
		0x5300, 
		0xcb00, 
		0x70f9, 
		0xd3e0, 
		0x52a0, 
		0x5240, 
		0xd080, 
		0xaea8, 
		0x8157, 
		0x8000, 
		0xcd40, 
		0x7c00, 
		0xd080, 
		0xcc00, 
		0x0000, 
		0x8158, 
		0x5240, 
		0x4600, 
		0xc500, 
		0x0872, 
		0x7c00, 
		0x52a0, 
		0x4d40, 
		0x112e, 
		0x4c80, 
		0xc135, 
		0x8000, 
		0x5120, 
		0x4ec0, 
		0x4900, 
		0x5360, 
		0x52a0, 
		0xe3c0, 
		0xd434, 
		0xcfc0, 
		0x92e5, 
		0x4980, 
		0x0f8c, 
		0x4d40, 
		0xf8b4, 
		0xb5c8, 
		0xcfc0, 
		0x8000, 
		0x8000, 
		0x7c78, 
		0x4d4d, 
		0x4c40, 
		0x7d4c, 
		0x4d40, 
		0x4400, 
		0x52e0, 
		0x04e9, 
		0xc400, 
		0x4e80, 
		0xd120, 
		0x7c6c, 
		0x4600, 
		0xfc00, 
		0x4c40, 
		0xca80, 
		0x5120, 
		0xd000, 
		0x7c00, 
		0x6781, 
		0xabef, 
		0xd040, 
		0xc800, 
		0x4dc0, 
		0x7556, 
		0x5080, 
		0xd1c0, 
		0xbc00, 
		0x4e00, 
		0x1790, 
		0xd080, 
		0xd2c0, 
		0xd240, 
		0xdcc5, 
		0x4cc0, 
		0x8ed3, 
		0xc470, 
		0x4f00, 
		0x4a32, 
		0x0000, 
		0xd220, 
		0x8000, 
		0x8586, 
		0xfc00, 
		0xd0a0, 
		0xce40, 
		0x4400, 
		0xd000, 
		0x4f00, 
		0xd3a0, 
		0xfdb5, 
		0xd004, 
		0x4000, 
		0xad15, 
		0xcfc0, 
		0x1644, 
		0x4980, 
		0xc200, 
		0xef12, 
		0x3c00, 
		0x4b00, 
		0x6925, 
		0x4880, 
		0x51bb, 
		0x5260, 
		0xabc0, 
		0x4d00, 
		0x4fc0, 
		0xd1c0, 
		0xc700, 
		0xb29d, 
		0x5340, 
		0xa81b, 
		0x4e40, 
		0x5360, 
		0xcdc0, 
		0x2b17, 
		0xd12c, 
		0xcc80, 
		0xfc00, 
		0x4d40, 
		0xcf00, 
		0xe13d, 
		0x4c40, 
		0xc900, 
		0xd340, 
		0x5180, 
		0xd160, 
		0xc2dd, 
		0x2891, 
		0xd060, 
		0x8305, 
		0xfc00, 
		0xc500, 
		0xcfc0, 
		0xd000, 
		0xb208, 
		0xaa16, 
		0xd160, 
		0x4900, 
		0x4fc0, 
		0xd060, 
		0x4b80, 
		0xcd00, 
		0xc409, 
		0x7c00, 
		0x51e0, 
		0x02e2, 
		0x50e0, 
		0x5040, 
		0x4d00, 
		0xc600, 
		0xc980, 
		0x51e0, 
		0xc500, 
		0x5360, 
		0xca80, 
		0xbbd5, 
		0x4700, 
		0xe0c8, 
		0x8e8b, 
		0x50c0, 
		0xcb00, 
		0x4e80, 
		0x6405, 
		0xfa66, 
		0xfc6b, 
		0xcec0, 
		0x4dc0, 
		0x53a0, 
		0x3507, 
		0x0000, 
		0x50a0, 
		0xc700, 
		0xae77, 
		0xd3e0, 
		0x5380, 
		0x9031, 
		0xcf80, 
		0xd1e0, 
		0x5033, 
		0xcf40, 
		0xefae, 
		0x3f8b, 
		0x50e0, 
		0x11ce, 
		0x75a3, 
		0xfd2f, 
		0x07c0, 
		0x4f00, 
		0xf7ac, 
		0x5380, 
		0x51e0, 
		0xbc00, 
		0x4d00, 
		0x0c63, 
		0x4a80, 
		0xc800, 
		0xfc00, 
		0x53e0, 
		0x4000, 
		0xffc6, 
		0x4c00, 
		0xfc00, 
		0xd1e0, 
		0x4c40, 
		0x1c79, 
		0xc400, 
		0xc400, 
		0x5280, 
		0x6731, 
		0x4f84, 
		0xd120, 
		0xd220, 
		0x50c0, 
		0x8000, 
		0xe2b1, 
		0x5320, 
		0xca80, 
		0xcfc0, 
		0xb3f3, 
		0xc200, 
		0x52c0, 
		0xd340, 
		0xd2c0, 
		0x53a0, 
		0xcfc0, 
		0x5280, 
		0x80da, 
		0x4700, 
		0x5200, 
		0x0000, 
		0x4200, 
		0x069f, 
		0xc000, 
		0xb52e, 
		0x53c0, 
		0xd0a0, 
		0x086c, 
		0xe1bb, 
		0x5160, 
		0x4ec0, 
		0xdb31, 
		0xd320, 
		0xd1c0, 
		0xd080, 
		0xd160, 
		0xae0c, 
		0x5000, 
		0xfd8a, 
		0x8000, 
		0xd8ca, 
		0xd000, 
		0x77dd, 
		0x8000, 
		0x5340, 
		0x4d80, 
		0x1566, 
		0x924c, 
		0xc800, 
		0x7355, 
		0xca80, 
		0x246d, 
		0xdcea, 
		0xd0a0, 
		0x473e, 
		0x80b1, 
		0x4f00, 
		0xd3c0, 
		0xc200, 
		0x8000, 
		0xd380, 
		0x5280, 
		0xfc00, 
		0xcf80, 
		0x4b10, 
		0xd1e0, 
		0xf543, 
		0x3484, 
		0x4d00, 
		0x4e61, 
		0xcb80, 
		0x5060, 
		0x4f00, 
		0x4c80, 
		0x0081, 
		0xa391, 
		0xc323, 
		0xd4f3, 
		0xd100, 
		0x391a, 
		0x8000, 
		0xd0c0, 
		0xcd40, 
		0x17b3, 
		0xcd00, 
		0x4500, 
		0x5160, 
		0x5340, 
		0x5120, 
		0x5160, 
		0xbc00, 
		0x4200, 
		0xd1e0, 
		0xcfc0, 
		0x52c0, 
		0x51c0, 
		0x8000, 
		0x7924, 
		0xd060, 
		0x5220, 
		0x5240, 
		0x4111, 
		0xcea8, 
		0x3fc3, 
		0x627f, 
		0x5040, 
		0xc18a, 
		0x4500, 
		0xc7ca, 
		0xc9c2, 
		0xffef, 
		0x9fe1, 
		0xcd00, 
		0x3d31, 
		0x8376, 
		0x1202, 
		0xccc0, 
		0xd2c0, 
		0x4cc0, 
		0x6ecc, 
		0x7c00, 
		0xd3c0, 
		0x52a0, 
		0x5260, 
		0x5a44, 
		0xcc40, 
		0x5180, 
		0xfc00, 
		0x7d28, 
		0x53e0, 
		0xd040, 
		0x3c00, 
		0xcc00, 
		0x4d80, 
		0x4dc0, 
		0x5300, 
		0xd240, 
		0x8000, 
		0x4fc0, 
		0xd020, 
		0x98ee, 
		0xd240, 
		0xcc80, 
		0xd180, 
		0xc700, 
		0xc980, 
		0xbc00, 
		0x4d80, 
		0xd180, 
		0xd2e0, 
		0x3e84, 
		0x4200, 
		0xd260, 
		0x8000, 
		0x5a23, 
		0x4c80, 
		0xcd40, 
		0x4900, 
		0xdf23, 
		0x4200, 
		0xd120, 
		0x52a0, 
		0x5fc0, 
		0xd040, 
		0xbc00, 
		0x5000, 
		0xc900, 
		0x5020, 
		0x4e00, 
		0x8000, 
		0x5020, 
		0xc2c6, 
		0x07fe, 
		0x0000, 
		0x53c0, 
		0xfc00, 
		0xcf40, 
		0x1fb7, 
		0x4f3e, 
		0x8000, 
		0xcdc0, 
		0xd240, 
		0xc142, 
		0x043c, 
		0x5060, 
		0x52c0, 
		0x4400, 
		0xc700, 
		0xc006, 
		0xd020, 
		0x4cc0, 
		0x7c00, 
		0x4c80, 
		0x5340, 
		0x4b80, 
		0x4900, 
		0x4c80, 
		0xcfc0, 
		0x4dc0, 
		0x0922, 
		0x5080, 
		0x02eb, 
		0xc200, 
		0xfc00, 
		0x4fc0, 
		0x4600, 
		0x0862, 
		0xd380, 
		0xc400, 
		0xccc0, 
		0xd1a0, 
		0xd300, 
		0x6482, 
		0xd0e0, 
		0x4b80, 
		0xb1b3, 
		0xc800, 
		0xfc00, 
		0x7cd8, 
		0x5180, 
		0x7c00, 
		0x50c0, 
		0xfc00, 
		0x2795, 
		0x5300, 
		0x0000, 
		0x4600, 
		0x0817, 
		0x5060, 
		0xcfc0, 
		0x8000, 
		0xfcf6, 
		0x064c, 
		0xd3e0, 
		0xcd80, 
		0xd3c0, 
		0x4a00, 
		0xcec0, 
		0xd1e0, 
		0x1851, 
		0x4e00, 
		0x0000, 
		0xd169, 
		0x4cc0, 
		0xa3ca, 
		0x4c36, 
		0xefb4, 
		0x4600, 
		0x8000, 
		0x5040, 
		0xca80, 
		0x8000, 
		0xd240, 
		0x4500, 
		0x6145, 
		0xfc00, 
		0xcdc0, 
		0xcdc0, 
		0xd240, 
		0xd120, 
		0x5100, 
		0x5320, 
		0x61c7, 
		0x4900, 
		0x0031, 
		0x4fc0, 
		0x4500, 
		0xd220, 
		0xc980, 
		0xfc00, 
		0x8000, 
		0x5320, 
		0xce40, 
		0xd3e0, 
		0xd320, 
		0x273d, 
		0x5060, 
		0xd140, 
		0xce80, 
		0xaeda, 
		0xbc00, 
		0x3230, 
		0x7e42, 
		0xd2c0, 
		0x5120, 
		0x5240, 
		0xc331, 
		0xd240, 
		0xdfa7, 
		0x53c0, 
		0x4800, 
		0xc62d, 
		0x4b3f, 
		0xd200, 
		0xcf40, 
		0x6c9a, 
		0x5080, 
		0xcc40, 
		0xd0a0, 
		0x5320, 
		0x5120, 
		0xd198, 
		0x5140, 
		0xcd40, 
		0x4635, 
		0x5140, 
		0x50c0, 
		0x5220, 
		0x5140, 
		0xce40, 
		0xcf80, 
		0x32d1, 
		0x4b00, 
		0x51c0, 
		0x4700, 
		0x5080, 
		0x35f1, 
		0x50a0, 
		0x5280, 
		0x5100, 
		0xcdda, 
		0x0000, 
		0x55e7, 
		0x0000, 
		0xca00, 
		0xd080, 
		0x53e0, 
		0xc900, 
		0x182a, 
		0x51c0, 
		0x52e0, 
		0x5380, 
		0xd2e0, 
		0x2007, 
		0x4d00, 
		0xd2a0, 
		0xe89a, 
		0xd0e0, 
		0x8897, 
		0x5000, 
		0x50a0, 
		0xce40, 
		0xd3c0, 
		0xcc80, 
		0xfc00, 
		0x5040, 
		0xd140, 
		0x78e2, 
		0x5060, 
		0xd1a0, 
		0x4ec0, 
		0xd040, 
		0x5260, 
		0xfc00, 
		0xcfc0, 
		0x21ba, 
		0xcc00, 
		0x215d, 
		0x8d9f, 
		0x0990, 
		0x53a0, 
		0x12da, 
		0x4dc0, 
		0x73d9, 
		0x4b80, 
		0x8000, 
		0xe390, 
		0x50e0, 
		0x5180, 
		0x9c09, 
		0xca28, 
		0xd2a0, 
		0xd3e0, 
		0x449a, 
		0x5140, 
		0x7c00, 
		0xfc00, 
		0x5320, 
		0x5040, 
		0x4a80, 
		0x4400, 
		0xee5c, 
		0x4800, 
		0xc200, 
		0x5578, 
		0x8000, 
		0xa352, 
		0x5380, 
		0xbaeb, 
		0xd3e0, 
		0x4dc0, 
		0x4c40, 
		0x28ce, 
		0x5000, 
		0x2642, 
		0x608f, 
		0x4411, 
		0x50a0, 
		0xcb00, 
		0x4b80, 
		0x2fa7, 
		0xe706, 
		0xd0e0, 
		0x4d3a, 
		0xcf00, 
		0xcd00, 
		0x90ef, 
		0xc200, 
		0x8000, 
		0xe593, 
		0xd140, 
		0x86fd, 
		0x9755, 
		0x7c00, 
		0xd180, 
		0x51e0, 
		0x53e0, 
		0x8000, 
		0xc000, 
		0x4f00, 
		0x4ec0, 
		0xd140, 
		0xc6e4, 
		0x8000, 
		0xd040, 
		0x51c0, 
		0x0000, 
		0x4e40, 
		0x4f00, 
		0x568c, 
		0x6fcf, 
		0xd0a0, 
		0x5060, 
		0xc6a3, 
		0x207d, 
		0x8000, 
		0x4b80, 
		0x4d80, 
		0x4d00, 
		0xcf80, 
		0xd060, 
		0xc894, 
		0xa830, 
		0xbc00, 
		0x4900, 
		0xcb00, 
		0xfc00, 
		0x8632, 
		0xd220, 
		0x1d1d, 
		0x7ff9, 
		0xd2c0, 
		0xfd51, 
		0x8000, 
		0x9c94, 
		0xc800, 
		0x5140, 
		0x50a0, 
		0x8000, 
		0x8000, 
		0x4a80, 
		0x4500, 
		0xce00, 
		0xc200, 
		0xc980, 
		0xc600, 
		0x8000, 
		0xdc8c, 
		0x4dc0, 
		0x8000, 
		0xd240, 
		0xd3a0, 
		0x6bc9, 
		0xd300, 
		0x5020, 
		0x8d8a, 
		0x5000, 
		0xd2c0, 
		0xc782, 
		0x5000, 
		0x4611, 
		0xd000, 
		0x5340, 
		0x50c0, 
		0x22f7, 
		0xf3df, 
		0xd040, 
		0x9fcf, 
		0xfc5b, 
		0xcf00, 
		0x51c0, 
		0xb2be, 
		0x3bc2, 
		0xc980, 
		0xd280, 
		0x5160, 
		0x4a80, 
		0x075d, 
		0x51c0, 
		0xcb80, 
		0x4400, 
		0xb93e, 
		0x8a81, 
		0x4cc0, 
		0x8000, 
		0xd360, 
		0x5040, 
		0x647e, 
		0xd0e0, 
		0xcf00, 
		0xcf40, 
		0x94a7, 
		0x5020, 
		0x42aa, 
		0xd040, 
		0xd160, 
		0x3f1f, 
		0xd3e0, 
		0xcc40, 
		0xb099, 
		0x4200, 
		0x40a5, 
		0xb06a, 
		0x50c0, 
		0x4efd, 
		0x4c40, 
		0x4900, 
		0x5280, 
		0x80b2, 
		0x5300, 
		0x7c00, 
		0xcc00, 
		0xcf40, 
		0xd2e0, 
		0x5040, 
		0x4600, 
		0xd1e0, 
		0x918e, 
		0xd0a0, 
		0x570b, 
		0xb2e6, 
		0x4b80, 
		0x0000, 
		0x5320, 
		0x5180, 
		0x4d40, 
		0xe23b, 
		0x8000, 
		0x50e0, 
		0xc980, 
		0xd380, 
		0x5120, 
		0xc600, 
		0xd100, 
		0x4980, 
		0xd240, 
		0x8000, 
		0xd140, 
		0xeaaf, 
		0xfc00, 
		0xc000, 
		0x5000, 
		0x8000, 
		0xd2e0, 
		0xba85, 
		0xb378, 
		0x4a00, 
		0x6999, 
		0xfc00, 
		0xf571, 
		0xcb80, 
		0x4d80, 
		0xd237, 
		0x48a4, 
		0x57c9, 
		0x16b9, 
		0xeebe, 
		0xcc40, 
		0xd1a0, 
		0x0c79, 
		0x8000, 
		0xfcee, 
		0xd0a0, 
		0xce40, 
		0x51a0, 
		0x8000, 
		0xcb00, 
		0x7c00, 
		0xab54, 
		0x4cc0, 
		0x4f00, 
		0xcf00, 
		0xd320, 
		0xca80, 
		0xc411, 
		0xd040, 
		0xcc80, 
		0x5100, 
		0x5000, 
		0x51c0, 
		0x1cde, 
		0xee7b, 
		0x02bf, 
		0x0000, 
		0xc600, 
		0x5180, 
		0xd1e0, 
		0xcfc0, 
		0xd3a0, 
		0x234e, 
		0x4fc0, 
		0xc600, 
		0x50c2, 
		0x4f80, 
		0xeb04, 
		0xc900, 
		0xc0be, 
		0x4000, 
		0xf95d, 
		0xd3e0, 
		0x7c00, 
		0xd380, 
		0x5060, 
		0x4a80, 
		0xdaca, 
		0xaf52, 
		0x4b80, 
		0x9c51, 
		0xd06c, 
		0xfc00, 
		0xc400, 
		0xcd00, 
		0x1824, 
		0x50e0, 
		0x5100, 
		0x7c00, 
		0xde55, 
		0x4800, 
		0x4b80, 
		0x52c0, 
		0x8000, 
		0xd180, 
		0xfc00, 
		0xc800, 
		0x4400, 
		0x7c00, 
		0xa303, 
		0x161b, 
		0x4a80, 
		0xcd80, 
		0xfc00, 
		0x4e00, 
		0x5240, 
		0x0000, 
		0xd200, 
		0x88ff, 
		0x9b48, 
		0x7c00, 
		0x5140, 
		0x7c00, 
		0xab1b, 
		0x5220, 
		0xd2a0, 
		0xcc80, 
		0xc96d, 
		0xce80, 
		0xd2a0, 
		0xd220, 
		0xd1a0, 
		0x4cc0, 
		0x3e0a, 
		0x15fb, 
		0x8796, 
		0xe169, 
		0xd300, 
		0x9251, 
		0xcd40, 
		0x03ce, 
		0xe5f6, 
		0xcfc0, 
		0x4880, 
		0xd3c0, 
		0x5120, 
		0xd13c, 
		0xd240, 
		0x1abf, 
		0x5300, 
		0x0000, 
		0xfda8, 
		0x4e00, 
		0xcc00, 
		0xc3cf, 
		0x4c00, 
		0x4e80, 
		0xd320, 
		0x7d3f, 
		0x53c0, 
		0x3195, 
		0xcd40, 
		0x50c0, 
		0x4636, 
		0x8000, 
		0x7de6, 
		0x740c, 
		0x5060, 
		0x920f, 
		0x83f9, 
		0xcc40, 
		0x4dc0, 
		0x5edd, 
		0x1a40, 
		0x965a, 
		0xd000, 
		0xb5e2, 
		0xcb45, 
		0xe38b, 
		0xd1c0, 
		0xd1a0, 
		0xeeda, 
		0x4f00, 
		0x409d, 
		0xc700, 
		0xb9ca, 
		0x4e00, 
		0xa00c, 
		0x8221, 
		0x4e00, 
		0x6d30, 
		0x4d80, 
		0xd3e0, 
		0x674e, 
		0xfc00, 
		0x4600, 
		0x4b00, 
		0xd3c0, 
		0x9032, 
		0x4800, 
		0xca80, 
		0x5100, 
		0x4900, 
		0xfc00, 
		0xd2e0, 
		0xd3e0, 
		0xc721, 
		0x78af, 
		0x8a4e, 
		0xd300, 
		0x51c0, 
		0xae25, 
		0x4f80, 
		0x4400, 
		0xfd38, 
		0xd5f8, 
		0x8000, 
		0xd000, 
		0xd040, 
		0x5080, 
		0xd0e0, 
		0xd100, 
		0xc50b, 
		0xcff2, 
		0xc600, 
		0xc4aa, 
		0xc500, 
		0x4661, 
		0x4900, 
		0xd160, 
		0x12c5, 
		0xa988, 
		0x51e0, 
		0x50a0, 
		0xd060, 
		0x6013, 
		0x4d80, 
		0x4fc0, 
		0x77ae, 
		0xd1c0, 
		0x4d80, 
		0xd180, 
		0x850e, 
		0x8000, 
		0xcc40, 
		0x4800, 
		0x1211, 
		0xf6cb, 
		0x51c0, 
		0xce09, 
		0x4600, 
		0xeba9, 
		0x4880, 
		0x4d40, 
		0x5160, 
		0x620e, 
		0xfdf4, 
		0x170a, 
		0xe19e, 
		0xd0c0, 
		0xefe7, 
		0x4d00, 
		0x4600, 
		0xd080, 
		0x7c35, 
		0xd2e0, 
		0x5040, 
		0x52c0, 
		0xca00, 
		0x5020, 
		0xd000, 
		0x5541, 
		0xfc00, 
		0xca00, 
		0xfc00, 
		0x0000, 
		0x9511, 
		0xd0e0, 
		0xaeed, 
		0x2370, 
		0x4600, 
		0xd220, 
		0xd3a0, 
		0xd300, 
		0x5000, 
		0xc133, 
		0xb6ff, 
		0x9b49, 
		0x4fc0, 
		0x4e40, 
		0x51c0, 
		0xd100, 
		0xd280, 
		0x5080, 
		0x52a0, 
		0x4f80, 
		0x4d80, 
		0x50c0, 
		0x4200, 
		0xfc00, 
		0x8000, 
		0x3c00, 
		0x4dc0, 
		0x260d, 
		0xd180, 
		0x5020, 
		0x4d80, 
		0xcd00, 
		0xcb00, 
		0x29a6, 
		0xfc00, 
		0x0d81, 
		0x7c00, 
		0x7c00, 
		0x925c, 
		0xca00, 
		0x5340, 
		0x50c0, 
		0x4c4e, 
		0x5280, 
		0xd0a0, 
		0x0000, 
		0x52a0, 
		0x1fc0, 
		0x7c2b, 
		0xc8d5, 
		0xd120, 
		0x5340, 
		0x8000, 
		0xd2e0, 
		0x4880, 
		0xd340, 
		0x0000, 
		0x90bd, 
		0x52c0, 
		0x5240, 
		0xccc0, 
		0x4fc0, 
		0xd180, 
		0x4800, 
		0xd0a0, 
		0xfd5d, 
		0xd060, 
		0x50c0, 
		0xd260, 
		0xcc80, 
		0x7c00, 
		0x0000, 
		0xd360, 
		0x7c00, 
		0x0fcf, 
		0x4c00, 
		0x0000, 
		0x5140, 
		0xfce8, 
		0xca80, 
		0x5160, 
		0xc200, 
		0xedff, 
		0xd060, 
		0x3c00, 
		0x0189, 
		0xcebc, 
		0x3fec, 
		0xcf00, 
		0xf2ef, 
		0x50a0, 
		0xce80, 
		0x4a00, 
		0xac99, 
		0xa419, 
		0x4880, 
		0x5260, 
		0xc400, 
		0x4f80, 
		0x52a0, 
		0xcd0f, 
		0x33c6, 
		0xfc00, 
		0xcd00, 
		0x4200, 
		0x51dd, 
		0x8120, 
		0x52e0, 
		0xd0a0, 
		0x68f3, 
		0x5898, 
		0xd180, 
		0x7c00, 
		0xcc80, 
		0x4c80, 
		0xd692, 
		0xfc00, 
		0x9727, 
		0x5040, 
		0x2565, 
		0x468c, 
		0xdd9a, 
		0x4e00, 
		0x5140, 
		0xfc00, 
		0x4500, 
		0xc000, 
		0x0000, 
		0xd2e0, 
		0xd514, 
		0x4d80, 
		0x3d94, 
		0x3375, 
		0xd2e0, 
		0x5120, 
		0xd1c0, 
		0x5120, 
		0xd0a0, 
		0x4cc0, 
		0x4f40, 
		0x0000, 
		0xe601, 
		0xb141, 
		0x4c40, 
		0x65e7, 
		0x51a0, 
		0xce80, 
		0x60dc, 
		0xd2a0, 
		0xa424, 
		0x2f9a, 
		0xd040, 
		0x5e63, 
		0x4d40, 
		0x4fc0, 
		0x28c0, 
		0x8000, 
		0x5320, 
		0x310f, 
		0xd020, 
		0x9b9c, 
		0x0212, 
		0x7b09, 
		0x4d00, 
		0xfd1f, 
		0x1445, 
		0x0000, 
		0x8000, 
		0xd1c0, 
		0xcfc0, 
		0x4000, 
		0x32f1, 
		0x4400, 
		0xc600, 
		0x4f80, 
		0xc200, 
		0xb2e8, 
		0xd2e0, 
		0xcd00, 
		0x8065, 
		0xd360, 
		0x9dc6, 
		0x5060, 
		0x4cc0, 
		0xfc00, 
		0xcc00, 
		0x4a80, 
		0xbb19, 
		0x62a2, 
		0xcf80, 
		0x477a, 
		0x5200, 
		0x53c0, 
		0x4cc0, 
		0x5111, 
		0xcb80, 
		0x5280, 
		0xd040, 
		0xc880, 
		0x4f25, 
		0x5320, 
		0x5300, 
		0x5180, 
		0x12f9, 
		0xd200, 
		0x53ee, 
		0x4f80, 
		0xcd00, 
		0x52a0, 
		0xd240, 
		0x7359, 
		0xd060, 
		0xed54, 
		0x52e0, 
		0x8321, 
		0xcf40, 
		0xd0c0, 
		0xd0e0, 
		0x5140, 
		0xeb1c, 
		0x4cd3, 
		0xd100, 
		0x5020, 
		0xce40, 
		0x4dc0, 
		0xbffb, 
		0xcf40, 
		0x51e0, 
		0x4f00, 
		0xaded, 
		0x5140, 
		0x8369, 
		0xd2e0, 
		0xd160, 
		0x4cc0, 
		0x2e2a, 
		0x4ec0, 
		0x0000, 
		0xd300, 
		0xc200, 
		0xbc00, 
		0xdb93, 
		0xd340, 
		0x5060, 
		0x3fc1, 
		0xfc00, 
		0x4d40, 
		0x52c0, 
		0xcd80, 
		0x5997, 
		0x4cc0, 
		0x4d80, 
		0x4e80, 
		0x8bd0, 
		0x4500, 
		0x0000, 
		0x5040, 
		0x52c0, 
		0xd140, 
		0xecf1, 
		0xd2c0, 
		0xd240, 
		0xcd00, 
		0x5320, 
		0x7941, 
		0xd020, 
		0x3787, 
		0xcb80, 
		0xe5e8, 
		0x4b8e, 
		0x1090, 
		0x4f80, 
		0x4a00, 
		0x50e0, 
		0x4800, 
		0x4c00, 
		0xc600, 
		0xc980, 
		0xd0a0, 
		0xca15, 
		0xcd00, 
		0xd0e0, 
		0x4d40, 
		0x5240, 
		0x51a0, 
		0x7c00, 
		0xcb80, 
		0x2802, 
		0x4b80, 
		0xfc00, 
		0x02a3, 
		0x5240, 
		0x10ce, 
		0xa0df, 
		0xcfc0, 
		0x3632, 
		0xc273, 
		0xd2e0, 
		0x90d0, 
		0xd0c0, 
		0x5240, 
		0x0000, 
		0xd240, 
		0xd140, 
		0x5020, 
		0x5140, 
		0x0000, 
		0x03b3, 
		0x4624, 
		0x4e40, 
		0x4e00, 
		0x5220, 
		0x0973, 
		0x53a0, 
		0x5180, 
		0x0000, 
		0xc400, 
		0xc900, 
		0xc800, 
		0xca80, 
		0x4f80, 
		0xd380, 
		0xd1e0, 
		0x5140, 
		0x98f1, 
		0x4f80, 
		0x5300, 
		0xd000, 
		0xd380, 
		0xd1a0, 
		0x148d, 
		0xcd80, 
		0x1493, 
		0xc700, 
		0x5040, 
		0x4e00  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x94a56f13, 
		0x80000100, 
		0xc2780000, 
		0x5d1aa526, 
		0xc1400000, 
		0xc23b231e, 
		0x41600000, 
		0xd65e7135, 
		0x80000000, 
		0x00000000, 
		0xc2100000, 
		0xdc11f973, 
		0xbf028f5c, 
		0x0e000007, 
		0xf7101e57, 
		0x42780000, 
		0x00800000, 
		0x8008ee61, 
		0x80000001, 
		0x80000001, 
		0x42240000, 
		0xbb804e4c, 
		0x41000000, 
		0x00000000, 
		0xc1200000, 
		0xc26c0000, 
		0x6b7d1412, 
		0xc2480000, 
		0xe8b5c733, 
		0xc1b00000, 
		0xc1d80000, 
		0x41980000, 
		0xc1600000, 
		0x00000004, 
		0x6db45c43, 
		0x42080000, 
		0x0c7ffc00, 
		0x6cdc81bf, 
		0x80000020, 
		0xff800000, 
		0xd4b69633, 
		0xb478b3be, 
		0x00000000, 
		0x00000000, 
		0xe9eb4f7b, 
		0x7f800000, 
		0x074930a8, 
		0xe6388579, 
		0x43bbeb2d, 
		0x162ac76d, 
		0x0c7fc000, 
		0x0e01ffff, 
		0xff7fffff, 
		0x80ffffff, 
		0xff7ffffe, 
		0x00800001, 
		0x675d67ac, 
		0xe99894e3, 
		0x5e128435, 
		0x00000000, 
		0xb3e0cc6c, 
		0xff800001, 
		0x806e0713, 
		0x7f800000, 
		0x42340000, 
		0xc2100000, 
		0xe4a557d2, 
		0x7fbfffff, 
		0x42500000, 
		0x7f7fffff, 
		0x85f3789c, 
		0x7f77a989, 
		0x0c7fffe0, 
		0xc1c80000, 
		0x7f800000, 
		0x00000080, 
		0x42680000, 
		0x80000000, 
		0x7152f155, 
		0x0d00fff0, 
		0x7f7fffff, 
		0x41300000, 
		0x2e0d9149, 
		0x41a00000, 
		0x424c0000, 
		0x80040000, 
		0x427c0000, 
		0x80011111, 
		0x0e0fffff, 
		0x80100000, 
		0x80800002, 
		0x0c7f0000, 
		0x80000000, 
		0x42300000, 
		0x546cd27f, 
		0x00000080, 
		0xa861eb74, 
		0xc2680000, 
		0xff912123, 
		0x41c00000, 
		0xa5140fc5, 
		0xff7ffffe, 
		0x0c7ffff8, 
		0x42600000, 
		0x80200000, 
		0x76a34370, 
		0x73a2d8c5, 
		0xff7fffff, 
		0x1128ed58, 
		0x80000000, 
		0x0c7fffe0, 
		0xc694d8e1, 
		0x41300000, 
		0xff7fffff, 
		0x80080000, 
		0x8845bb56, 
		0x40800000, 
		0xad0bdd81, 
		0xc1b80000, 
		0x7f800000, 
		0xc2700000, 
		0xad4eb8f6, 
		0xbf800001, 
		0x666a2f1e, 
		0x421c0000, 
		0x42380000, 
		0x0f7fffff, 
		0xff7ffffe, 
		0x7f7fffff, 
		0xc2380000, 
		0xaa36770f, 
		0x4b000000, 
		0x59af2269, 
		0xc1b00000, 
		0xc2440000, 
		0x0c7fc000, 
		0x0027fd27, 
		0x41d80000, 
		0xffab9a08, 
		0x0e0007ff, 
		0x7f7ffffe, 
		0x0e001fff, 
		0x42680000, 
		0x00080000, 
		0x80ffffff, 
		0x00000001, 
		0xffbfffff, 
		0xc2080000, 
		0x7fc00000, 
		0x5c43cfcf, 
		0x0e0001ff, 
		0x42180000, 
		0x26c41656, 
		0xff800000, 
		0x41e00000, 
		0x58877b40, 
		0x80004000, 
		0x0c7fe000, 
		0x0c7fffe0, 
		0xff7ffffe, 
		0x80000000, 
		0x7f800000, 
		0x80008000, 
		0x89027a3b, 
		0x80008000, 
		0x00800002, 
		0x7fa0de1f, 
		0x00000000, 
		0x0a0995c2, 
		0xff800000, 
		0x3f028f5c, 
		0x7f800000, 
		0xc1200000, 
		0x80000002, 
		0x0d00fff0, 
		0x42140000, 
		0x3f22212d, 
		0x856f2a91, 
		0xdb61134a, 
		0xbf944584, 
		0x42140000, 
		0x42000000, 
		0x0c7fff00, 
		0xff800000, 
		0x00000000, 
		0x0c7ffffc, 
		0x41000000, 
		0x422c0000, 
		0x80000000, 
		0x3d910111, 
		0x00000000, 
		0x601f3ae3, 
		0x0e003fff, 
		0x0c7ff800, 
		0x0e001fff, 
		0xc1900000, 
		0xc1000000, 
		0xf7797403, 
		0xff800000, 
		0xf2344d96, 
		0xc1800000, 
		0x0c7e0000, 
		0x0e0001ff, 
		0x0c7fffc0, 
		0xce133156, 
		0x42000000, 
		0xc1900000, 
		0x00000000, 
		0x00000000, 
		0x80001000, 
		0x0e0001ff, 
		0xcccccccc, 
		0x00000001, 
		0x0e0003ff, 
		0xff800000, 
		0xbf028f5c, 
		0x007ffffe, 
		0xcccccccc, 
		0x0e000fff, 
		0x001ed32b, 
		0x42340000, 
		0xc77ceccd, 
		0x00008000, 
		0x0c7ffff0, 
		0xc2700000, 
		0x42600000, 
		0xe7c7bc17, 
		0x3255ed8d, 
		0x80000002, 
		0x80000000, 
		0x00ee9cb2, 
		0x80800002, 
		0x7f7ffffe, 
		0x8021b83b, 
		0x41d00000, 
		0x80000010, 
		0x0e003fff, 
		0xffc00000, 
		0xc1980000, 
		0x00000000, 
		0xd79bc33a, 
		0x00000000, 
		0x00000001, 
		0xc25c0000, 
		0x7f7ffffe, 
		0xe3dabb45, 
		0x00400000, 
		0x6f21d4c6, 
		0x80000000, 
		0xbbeafe8f, 
		0x41d80000, 
		0x80020000, 
		0xc24c0000, 
		0x007fffff, 
		0x80004000, 
		0x427c0000, 
		0x80800000, 
		0x00800000, 
		0x40800000, 
		0x42300000, 
		0x00000001, 
		0x50bcbcca, 
		0x0c7ffffe, 
		0x80011111, 
		0x7f7fffff, 
		0x80800000, 
		0x00000020, 
		0xa8bb77e1, 
		0xde6a33e2, 
		0xc2440000, 
		0x3f800001, 
		0xbf900778, 
		0x0e0001ff, 
		0x8baebf79, 
		0x3f800001, 
		0x0e0001ff, 
		0x40e00000, 
		0x7f800000, 
		0x00000000, 
		0x80000004, 
		0x41c80000, 
		0x9b314804, 
		0x4ae1db9e, 
		0x007ffffe, 
		0x7148c738, 
		0x40000000, 
		0x422c0000, 
		0x00000000, 
		0x42140000, 
		0x42480000, 
		0x00000002, 
		0x0c7fffc0, 
		0x80020000, 
		0xa8f16147, 
		0x4b8c4b40, 
		0x80800000, 
		0xff000000, 
		0x41600000, 
		0x00000002, 
		0x198ee61d, 
		0x0c7ffc00, 
		0x7fc00001, 
		0xffc00001, 
		0xc1d80000, 
		0xc25c0000, 
		0xfffd6d82, 
		0x80800002, 
		0x26525a00, 
		0x804bbfd3, 
		0x5ed62a57, 
		0x42240000, 
		0x80000000, 
		0x80000000, 
		0x89580e28, 
		0x42100000, 
		0x80800002, 
		0x00000000, 
		0x0e3fffff, 
		0x00800002, 
		0x0c7ffff8, 
		0xc20c0000, 
		0xff7fffff, 
		0xc2080000, 
		0x00800000, 
		0xff7fffff, 
		0x00000000, 
		0x0400361a, 
		0x00800001, 
		0xe15c096f, 
		0xc2240000, 
		0xc2700000, 
		0x53456f71, 
		0x7f800000, 
		0xc0800000, 
		0x00800001, 
		0x41c00000, 
		0x22f4ac8c, 
		0x7f800000, 
		0x00000004, 
		0xc1800000, 
		0x41a00000, 
		0x00002000, 
		0x42540000, 
		0xbf800001, 
		0xd70bc4f6, 
		0x41a00000, 
		0xc2480000, 
		0x0e00003f, 
		0x7f7ffffe, 
		0x42580000, 
		0x80648eaa, 
		0x41e80000, 
		0xc2400000, 
		0x00000000, 
		0x0c400000, 
		0x5f84ae4c, 
		0x007fffff, 
		0xcb000000, 
		0x0e000001, 
		0x0f7fffff, 
		0xb2b8bc8a, 
		0x42040000, 
		0x007ffffe, 
		0x00000000, 
		0x00800001, 
		0x7f800000, 
		0xef286a4f, 
		0xc2240000, 
		0xc1880000, 
		0x5e6645bd, 
		0x00100000, 
		0x41e80000, 
		0x0d00fff0, 
		0xc1c00000, 
		0xbf028f5c, 
		0xd8f69c24, 
		0x41800000, 
		0x42080000, 
		0x7f800000, 
		0xadbce14b, 
		0xff7ffffe, 
		0x00000000, 
		0x80800003, 
		0x42240000, 
		0x0c7f0000, 
		0x3c052658, 
		0x80000080, 
		0x80000000, 
		0xc2040000, 
		0xffb5e694, 
		0xedfa946e, 
		0xb3430a5e, 
		0x41f00000, 
		0x0c7fff80, 
		0x568f9601, 
		0x9e9a40c0, 
		0xe0c523b7, 
		0x80800000, 
		0x7faa25f2, 
		0x872d0b76, 
		0x7f7fffff, 
		0x2ce8dea7, 
		0xff800000, 
		0xc1a00000, 
		0x42200000, 
		0xffaf8dcf, 
		0x5874dbcf, 
		0x41d80000, 
		0xc1100000, 
		0x00a4cb25, 
		0x0e01ffff, 
		0x80000010, 
		0x00000000, 
		0x00008000, 
		0xff800000, 
		0x33333333, 
		0x576f579f, 
		0x6fecfa73, 
		0xa9b75524, 
		0x0e0007ff, 
		0x00040000, 
		0xffffffff, 
		0x41d00000, 
		0x00000000, 
		0x0e00000f, 
		0xc1f80000, 
		0xc1900000, 
		0x8f7ffffc, 
		0xff800000, 
		0x19b2e034, 
		0x7ebea5ac, 
		0x40cb7aad, 
		0x00800002, 
		0xc2600000, 
		0x80020000, 
		0x80000000, 
		0x37ab19f5, 
		0x7551e5ee, 
		0x80040000, 
		0x0e0fffff, 
		0x41500000, 
		0xc2400000, 
		0x7f800000, 
		0x00000080, 
		0x0c7ff000, 
		0x4bacbdb5, 
		0x51f5d207, 
		0xc1b80000, 
		0x7f7ffffe, 
		0xfe704157, 
		0xc7463d50, 
		0x41d00000, 
		0xa6f49c53, 
		0x7fc00001, 
		0xc2180000, 
		0x00800002, 
		0x8345950a, 
		0x800e01a1, 
		0x7fa07566, 
		0xa0fcf365, 
		0x0c7c0000, 
		0x8f7ffffd, 
		0x00800000, 
		0x00000008, 
		0x41b80000, 
		0x7f9804cc, 
		0x80000100, 
		0xc1700000, 
		0xb945bfc7, 
		0x00800002, 
		0x42440000, 
		0x42440000, 
		0x41b80000, 
		0x80000002, 
		0x0e007fff, 
		0x47345f6f, 
		0x7fffffff, 
		0xff800000, 
		0xff800000, 
		0x0e1fffff, 
		0x8ab3cec5, 
		0xc2240000, 
		0xe3c82407, 
		0xcccccccc, 
		0x0d00fff0, 
		0xcb000000, 
		0xc2040000, 
		0x94393057, 
		0x694f4b77, 
		0x0c7fc000, 
		0x41f00000, 
		0xcb8c4b40, 
		0x7f7ffffe, 
		0xc0800000, 
		0x8e4f4fcf, 
		0x7f800000, 
		0x80000000, 
		0x7f800000, 
		0xc1900000, 
		0x0e3fffff, 
		0x00000000, 
		0x66bce0a0, 
		0x42500000, 
		0x00001000, 
		0xc1c00000, 
		0x00000001, 
		0x0e001fff, 
		0x00000020, 
		0x0e0001ff, 
		0x00000000, 
		0x00100000, 
		0x0c7ffff8, 
		0x7f7ffffe, 
		0x732c7092, 
		0x427c0000, 
		0x41e80000, 
		0x0c7fe000, 
		0x40a00000, 
		0x0c7ffffe, 
		0x00000000, 
		0x0e0001ff, 
		0x80800001, 
		0x00000000, 
		0xc20c0000, 
		0xb6d46023, 
		0xcb8c4b40, 
		0xff800000, 
		0x42440000, 
		0xff7fffff, 
		0x41200000, 
		0x425c0000, 
		0x00000001, 
		0x7fffffff, 
		0x73876bd6, 
		0xc2140000, 
		0xc2440000, 
		0xfff9a690, 
		0x42500000, 
		0x7413e1c5, 
		0x00000002, 
		0x807ffffe, 
		0x00000000, 
		0x80000000, 
		0xe14a9d9f, 
		0xc2680000, 
		0x00800001, 
		0x0c600000, 
		0x41b80000, 
		0x00000010, 
		0x00800003, 
		0xcccccccc, 
		0xc20c0000, 
		0x00800000, 
		0x7822f0b0, 
		0x0780452f, 
		0x7f800001, 
		0x3f800000, 
		0xffc00001, 
		0xffae5238, 
		0x80000000, 
		0x0f7ffffd, 
		0x76840e1f, 
		0x34d194b3, 
		0x5e7559ce, 
		0x40e00000, 
		0x00800001, 
		0x3b9ea186, 
		0x00000002, 
		0xc1e00000, 
		0x7f7fffff, 
		0x0c7c0000, 
		0x00000000, 
		0x00080000, 
		0x0c700000, 
		0x80100000, 
		0x00000000, 
		0x80000000, 
		0xc24c0000, 
		0xff800000, 
		0x42600000, 
		0x80000000, 
		0x7af15064, 
		0x420c0000, 
		0x41000000, 
		0xc2540000, 
		0x42000000, 
		0x00200000, 
		0x00000002, 
		0xe45a00c2, 
		0x80000100, 
		0x40000000, 
		0x42780000, 
		0x0c600000, 
		0xc2340000, 
		0xe6fa2a48, 
		0x0c7f0000, 
		0x0c7ffc00, 
		0x80000000, 
		0x41d00000, 
		0x41a00000, 
		0xff800000, 
		0xc1c00000, 
		0x575c2415, 
		0x42600000, 
		0x42480000, 
		0x0c7ffe00, 
		0xffc00001, 
		0x80000000, 
		0x0c7ffffc, 
		0x42000000, 
		0xd77b1fd2, 
		0xc2580000, 
		0x08f34879, 
		0x00000000, 
		0x42780000, 
		0x80800001, 
		0xc2000000, 
		0xc1400000, 
		0xc0e00000, 
		0x7f800000, 
		0x7f85930d, 
		0xc0e00000, 
		0x8adb9dd8, 
		0x0a1c67d6, 
		0xc0a00000, 
		0x0b613156, 
		0x41f00000, 
		0x6645e5f0, 
		0x87042981, 
		0xc1300000, 
		0xc2240000, 
		0x8100b66b, 
		0x426c0000, 
		0x80000080, 
		0xc2140000, 
		0x7f800001, 
		0xfb3f9ada, 
		0xcae31a48, 
		0x7f800000, 
		0x00000001, 
		0x42500000, 
		0xdd119103, 
		0xc1500000, 
		0xff7fffff, 
		0x0e1fffff, 
		0x40000000, 
		0x80000000, 
		0xc2140000, 
		0x80000000, 
		0x80000400, 
		0x8f7fffff, 
		0xfc5bb5ac, 
		0xc53e7e56, 
		0xc1a80000, 
		0xd563207b, 
		0xc21c0000, 
		0x0f7fffff, 
		0x98217b55, 
		0xe36c7b60, 
		0x41400000, 
		0x80800002, 
		0x3f800000, 
		0x423c0000, 
		0x40800000, 
		0xff800001, 
		0xff800000, 
		0x00800003, 
		0x7f800000, 
		0xf1eb32a6, 
		0x80ffffff, 
		0x7fec9ab0, 
		0x423c0000, 
		0xb0914504, 
		0xc0e00000, 
		0xc1500000, 
		0x00000000, 
		0xc1f80000, 
		0x41f80000, 
		0xc2440000, 
		0x0c7fffff, 
		0xffc118a3, 
		0x00000040, 
		0x59a927e1, 
		0xc2700000, 
		0x80000800, 
		0x00800000, 
		0xede2b928, 
		0x80ffffff, 
		0x41d80000, 
		0x80800003, 
		0x7fc00000, 
		0x00800003, 
		0x00040000, 
		0x00069721, 
		0x42640000, 
		0x0e001fff, 
		0x00000000, 
		0x42040000, 
		0x00080000, 
		0x77606e3a, 
		0x00000001, 
		0x7f9529f9, 
		0x41f00000, 
		0xaad6d35b, 
		0x0c7f0000, 
		0x6db74ced, 
		0xce3717a6, 
		0xbc757578, 
		0x80000000, 
		0xed1be653, 
		0xc1880000, 
		0xc2380000, 
		0x7f7ffffe, 
		0x80080000, 
		0x3ef0dc3a, 
		0x80000020, 
		0x80000010, 
		0x0e0001ff, 
		0x7fc00000, 
		0x80000000, 
		0x420c0000, 
		0xe5875929, 
		0x06aebfe3, 
		0x42680000, 
		0x42180000, 
		0xc2680000, 
		0x0c7ff800, 
		0x00000001, 
		0xc6e3ddd7, 
		0xd0633a09, 
		0xc2100000, 
		0xff800001, 
		0x80000080, 
		0x41f80000, 
		0x00800001, 
		0xc160c47d, 
		0xa874f296, 
		0xc2400000, 
		0xc27c0000, 
		0x0c7ff000, 
		0xc1d80000, 
		0x41f80000, 
		0xc2100000, 
		0x80000080, 
		0x2ad7c9a8, 
		0xb9bac527, 
		0xb851c9d3, 
		0x7f7ffffe, 
		0x3f800000, 
		0x42600000, 
		0x42280000, 
		0x80000000, 
		0x42640000, 
		0xc1400000, 
		0xe4c561ff, 
		0xc2180000, 
		0x9a94e826, 
		0xff000000, 
		0x8042f028, 
		0x41a00000, 
		0xd3be7afd, 
		0xff7ffffe, 
		0xfc262437, 
		0x52d5c498, 
		0x0f7ffffd, 
		0xe4b59a36, 
		0x00000000, 
		0x42000000, 
		0xc1b80000, 
		0x004b95fc, 
		0x007ffffe, 
		0x00000010, 
		0x0e01ffff, 
		0x41900000, 
		0x80800001, 
		0x00008000, 
		0x42200000, 
		0x80800001, 
		0x41f00000, 
		0x2ac2759f, 
		0xff800000, 
		0x00000000, 
		0xcd8ad076, 
		0xbf028f5c, 
		0xc22c0000, 
		0x00002000, 
		0xc1e80000, 
		0x80000004, 
		0xc1c80000, 
		0x42480000, 
		0x80000200, 
		0x11297677, 
		0xc0400000, 
		0x00000000, 
		0x00000000, 
		0x254583e4, 
		0x0c7ffffc, 
		0x0e00000f, 
		0x42000000, 
		0x835d1d59, 
		0x00000002, 
		0x7f15962d, 
		0xafab01d9, 
		0x7f7ffffe, 
		0x1011f9d5, 
		0x80800000, 
		0x0c7ffe00, 
		0xc2380000, 
		0x7fc00001, 
		0xff9e5c0a, 
		0xc25c0000, 
		0x791865b1, 
		0x7fc00001, 
		0xc2540000, 
		0x00100000, 
		0x00000080, 
		0xff800001, 
		0x894f67f8, 
		0xff7fffff, 
		0xc2300000, 
		0xbf028f5c, 
		0xc2080000, 
		0xda9d1b86, 
		0x00008000, 
		0x9bcadf33, 
		0x80800003, 
		0x00800000, 
		0xffb4a321, 
		0xcb8c4b40, 
		0x0e0000ff, 
		0x4833870b, 
		0x42380000, 
		0xff7ffffe, 
		0x6f0b7e17, 
		0x0c7ffc00, 
		0x0c780000, 
		0x00000000, 
		0xf6a5f5e4, 
		0x7f000000, 
		0xffbfffff, 
		0x00000000, 
		0x6ba0ef54, 
		0xc20c0000, 
		0x00000200, 
		0x1192b632, 
		0x3f028f5c, 
		0xc2000000, 
		0xff800000, 
		0x0e3fffff, 
		0xc1d00000, 
		0x1545cc54, 
		0x00800001, 
		0xb5196a43, 
		0x42300000, 
		0x42540000, 
		0x00000000, 
		0x42300000, 
		0x9b25b236, 
		0x42500000, 
		0x26d03d54, 
		0xff800001, 
		0x80000000, 
		0x80000000, 
		0x80000200, 
		0x00000040, 
		0x42480000, 
		0x0e000fff, 
		0x41200000, 
		0x7a91d743, 
		0x0e07ffff, 
		0xc1300000, 
		0xc1700000, 
		0xff800000, 
		0x4c4be0f5, 
		0xc2040000, 
		0xbf800000, 
		0x7efd55e8, 
		0x00000002, 
		0xff800000, 
		0x0e00001f, 
		0x1c4a4810, 
		0x00000010, 
		0x7fbc5a6d, 
		0x0c7f8000, 
		0xae71200e, 
		0x80000002, 
		0x0e0001ff, 
		0xc1a80000, 
		0x9c4df1fc, 
		0x7230c92c, 
		0x42680000, 
		0x7fffffff, 
		0xc1cbdcfa, 
		0x0e0000ff, 
		0x7f800000, 
		0xdaca846c, 
		0x0c7ffffc, 
		0x00ffffff, 
		0x427c0000, 
		0x0e000fff, 
		0x80000000, 
		0xff7ffffe, 
		0x00000040, 
		0x42700000, 
		0x39e41cff, 
		0x6563af5b, 
		0xd912e494, 
		0xe82ec982, 
		0x0e0000ff, 
		0xc1800000, 
		0xa3b04a24, 
		0x00400000, 
		0x41800000, 
		0x41600000, 
		0x7f000000, 
		0x8f7fffff, 
		0xb444d233, 
		0x0c7fffe0, 
		0x8f7ffffc, 
		0x0c7fe000, 
		0xc2480000, 
		0x0e03ffff, 
		0x00800001, 
		0xd1616676, 
		0xc2140000, 
		0x7fc00001, 
		0x88378aec, 
		0x42580000, 
		0x425c0000, 
		0xbc86b716, 
		0x0c700000, 
		0x1a6b35e9, 
		0x3c3dea5b, 
		0x0c7fffff, 
		0xa14e599b, 
		0x41200000, 
		0x00000000, 
		0x41200000, 
		0x0c7f0000, 
		0xc1880000, 
		0x04b45473, 
		0xfa2e1051, 
		0x7f800000, 
		0x0c366bb6, 
		0x80000000, 
		0xc2600000, 
		0x42480000, 
		0xffaf9d95, 
		0x80000000, 
		0x6c2261fb, 
		0xff7fffff, 
		0xc2000000, 
		0x4b000000, 
		0x42000000, 
		0x80000400, 
		0x0e000fff, 
		0x7f9b77e7, 
		0x80000000, 
		0x002189fc, 
		0x4d89c23d, 
		0xc1d80000, 
		0x00000000, 
		0xff800000, 
		0x5d605480, 
		0xfc27ea34, 
		0x80147b85, 
		0x491b1f8a, 
		0x80000000, 
		0x0c7fffe0, 
		0xa7093e45, 
		0x0c7e0000, 
		0xff7ffffe, 
		0x7fd2540e, 
		0xc1d1e2ae, 
		0xfb867386, 
		0x80001000, 
		0x8bf464ea, 
		0x80200000, 
		0x42300000, 
		0x7f7ffffe, 
		0x369968bf, 
		0xc2400000, 
		0x00000000, 
		0xc2180000, 
		0x41800000, 
		0xc1600000, 
		0xa8d4cdce, 
		0x00800002, 
		0x00000000, 
		0x7f800000, 
		0x80800001, 
		0x80008000, 
		0xff7fffff, 
		0x00002000, 
		0xf5b53429, 
		0x44944253, 
		0xb18d5cca, 
		0x80000000, 
		0xc2680000, 
		0xd74393e6, 
		0x4b8c4b40, 
		0x93806675, 
		0x37d0508a, 
		0x42180000, 
		0xc1b80000, 
		0xee3e3234, 
		0x00000000, 
		0x7f800000, 
		0x81aaf60e, 
		0xc22c0000, 
		0x80000002, 
		0x40000000, 
		0x0e001fff, 
		0x80004000, 
		0x6801fe5c, 
		0xc2140000, 
		0xc2100000, 
		0xc60a27d8, 
		0x437e3861, 
		0x41200000, 
		0x00000000, 
		0x8f7ffffd, 
		0x00000000, 
		0xfff68ad6, 
		0x575f5691, 
		0x230785d1, 
		0x807fffff, 
		0x42500000, 
		0x3865c0cd, 
		0x80008000, 
		0x40a00000, 
		0x62e87859, 
		0x80000100, 
		0xc2200000, 
		0x7f800000, 
		0x00000000, 
		0x00000000, 
		0x7f81a2ff, 
		0xff800000, 
		0x7fa677e4, 
		0xc2280000, 
		0x80000000, 
		0x08916ddf, 
		0x0c7ffff0, 
		0xff000000, 
		0xe36c8d7b, 
		0x41200000, 
		0xc2080000, 
		0xc2440000, 
		0x170afdf2, 
		0x00200000, 
		0x0c7f0000, 
		0xff800001, 
		0x7f800000, 
		0x7f7ffffe, 
		0x3f800000, 
		0x40800000, 
		0x00000000, 
		0x2e796e53, 
		0x4b8c4b40, 
		0x00001000, 
		0x4a04417d, 
		0x42100000, 
		0x7fc00001, 
		0xc1880000, 
		0x4e8a75ba, 
		0x80000000, 
		0x2d42f991, 
		0xb83e70c7, 
		0x80800001, 
		0x00800003, 
		0xb52b6bff, 
		0x00000000, 
		0xc22c0000, 
		0xa84893bd, 
		0x80080000, 
		0xc2680000, 
		0x00800002, 
		0x80004000, 
		0x0e0000ff, 
		0x0f7ffffe, 
		0x3c3ee28d, 
		0xd3869776, 
		0x42440000, 
		0x420c0000, 
		0xc1900000, 
		0xb55c834b, 
		0xc1000000, 
		0x0c7fffe0, 
		0xc1980000, 
		0x7fc00001, 
		0x39f1843e, 
		0x00800000, 
		0x80241502, 
		0xff9983b2, 
		0xc2540000, 
		0x40a00000, 
		0x42440000, 
		0xff7ffffe, 
		0x41880000, 
		0x00800003, 
		0x00000200, 
		0x0f7fffff, 
		0x42280000, 
		0xa0b85658, 
		0x41f00000, 
		0x7fd5574b, 
		0x5c5d077c, 
		0x0c7fffe0, 
		0xa9121ed8, 
		0xc23c0000, 
		0x80004000, 
		0x0e00007f, 
		0x33050588, 
		0x7f800000, 
		0xc20c0000, 
		0xc1700000, 
		0x0e0007ff, 
		0x0e03ffff, 
		0x00400000, 
		0xc1c00000, 
		0x28cb49a9, 
		0xff800000, 
		0x005cdaa8, 
		0x00200000, 
		0xc23c0000, 
		0x7f800000, 
		0x41b00000, 
		0x4e426522, 
		0xc2580000, 
		0x80000000, 
		0x7f800000, 
		0x00ffffff, 
		0x7fbfffff, 
		0x2ea8bc2e, 
		0x911d8d24, 
		0x380ae5d4, 
		0x4bb26deb, 
		0x80200000, 
		0x41d00000, 
		0x13301e5e, 
		0x0e0003ff, 
		0x0c7fff80, 
		0x80004000, 
		0xc2140000, 
		0x6d023f8a, 
		0xc22c0000, 
		0xff800000, 
		0xebfa8f0c, 
		0x7fa49971, 
		0x00010000, 
		0x00008000, 
		0x0e0fffff, 
		0x94470b9a, 
		0xc1880000, 
		0x0e0fffff, 
		0xff800000, 
		0x807fffff, 
		0x424c0000, 
		0x41700000, 
		0xc1c00000, 
		0xc21c0000, 
		0xffa6b928, 
		0xc21c0000, 
		0x939077b8, 
		0xec4dcb87, 
		0x80000000, 
		0xffbfffff, 
		0xc2680000, 
		0x00800003, 
		0x93a0d285, 
		0xa34663cd, 
		0x0d000ff0, 
		0x034ddf7c, 
		0x2443e5b3, 
		0xff800000, 
		0x00000001, 
		0x15c32589, 
		0xffffffff, 
		0x42780000, 
		0x0c7ffc00, 
		0x003a311b, 
		0xe2cb81cc, 
		0x0c600000, 
		0x0e00003f, 
		0x7f800000, 
		0x80800002, 
		0x425c0000, 
		0x0c700000, 
		0x80000002, 
		0x41f00000, 
		0x0636c2c1, 
		0x0e000003, 
		0x00800003, 
		0xe76e7280, 
		0xffbb2454, 
		0x41880000, 
		0x42780000, 
		0xff7fffff, 
		0x41a00000, 
		0x76ab9340, 
		0x00000100, 
		0x42400000, 
		0xc2380000, 
		0x80000000, 
		0x0e0fffff, 
		0xc0c00000, 
		0xf7b9cfba, 
		0x00800003, 
		0x5e8dc148, 
		0x7f9a69e4, 
		0x41400000, 
		0xc2780000, 
		0x00000000, 
		0x7fd5753b, 
		0x80800000, 
		0x0c7e0000, 
		0x41400000, 
		0x80010000, 
		0x7f800000, 
		0x2e2a86f2, 
		0x00ffffff, 
		0xcffafe87, 
		0x59f8262a, 
		0xd7c0912f, 
		0x80000001, 
		0x33333333, 
		0x80800000, 
		0x7fc00001, 
		0x0c7ffe00, 
		0x00040000, 
		0x41200000, 
		0xff7ffffe, 
		0xc27c0000, 
		0xbb733c4c, 
		0x80000000, 
		0x89018a4c, 
		0x23ea3752, 
		0x80332257, 
		0xf4bd1689, 
		0x536d1214, 
		0xe2fbc295, 
		0x7f800000, 
		0xd2c104db, 
		0x7f800000, 
		0x8027d475, 
		0x42c13295, 
		0x80000000, 
		0xcccccccc, 
		0xc26c0000, 
		0x42440000, 
		0x272cf910, 
		0xc2540000, 
		0x807ffffe, 
		0xc0800000, 
		0xc2380000, 
		0x0c7ff800, 
		0xa442e597, 
		0x4689dc9f, 
		0x80800000, 
		0x834f9633, 
		0x40a00000, 
		0xc2280000, 
		0xc0c00000, 
		0xc1bd0705, 
		0x80000000, 
		0x7e964c5d, 
		0x421c0000, 
		0xc0c00000, 
		0xcb000000, 
		0x0d000ff0, 
		0xa3d60952, 
		0x80000000, 
		0x80000004, 
		0x80800001, 
		0x00000000, 
		0x0c7fffff, 
		0x0e00000f, 
		0x0d000ff0, 
		0x8201247c, 
		0x423c0000, 
		0x41f00000, 
		0x7f7fffff, 
		0x4b8c4b40, 
		0xc2300000, 
		0x003bd007, 
		0x7fbaf763, 
		0x41c00000, 
		0x80000000, 
		0x423c0000, 
		0x0c7ffe00, 
		0x00000000, 
		0x67aa509a, 
		0x4a76c75c, 
		0x0e0fffff, 
		0x3bd7454f, 
		0x58edf7c1, 
		0xc25c0000, 
		0xc1100000, 
		0x00800001, 
		0xfe617401, 
		0x8608086d, 
		0xffc00001, 
		0xf69272bb, 
		0xe9ce2815, 
		0x42300000, 
		0x8f7ffffe, 
		0x0c7ffffc, 
		0xffc00001, 
		0x3f800001, 
		0x00800001, 
		0x0e00ffff, 
		0xbf2d9dc8, 
		0x0c7ff800, 
		0x7f7ffffe, 
		0x7fb21901, 
		0x544e697c, 
		0xde295beb, 
		0xc1d00000, 
		0x7309ca7b, 
		0xbd79b376, 
		0x00800001, 
		0x80000000, 
		0x7b2dc794, 
		0xe7ab2adf, 
		0x420c0000, 
		0x00800003, 
		0x80800000, 
		0x80000000, 
		0xcd093e1c, 
		0x8085b6c9, 
		0x80000400, 
		0xff800000, 
		0xdac32b8b, 
		0x40a00000, 
		0x40c00000, 
		0xc2200000, 
		0x41d80000, 
		0x7fc00000, 
		0x352ca842, 
		0xd9d85ed1, 
		0x7f800000, 
		0x007fffff, 
		0x7101692a, 
		0x80010000, 
		0xc21c0000, 
		0x80ffffff, 
		0xdaeec34e, 
		0x0e3fffff, 
		0x3f028f5c, 
		0x9583f0a4, 
		0xc0800000, 
		0x250b68d6, 
		0x776c76aa, 
		0xff000000, 
		0x0d00fff0, 
		0x80800001, 
		0x40800000, 
		0x00800000, 
		0x486da43a, 
		0xc2640000, 
		0x00000000, 
		0x8f7ffffc, 
		0x41e00000, 
		0x99b3b1d5, 
		0x00000000, 
		0x8b01324e, 
		0x7f7ffffe, 
		0xc6aa8480, 
		0x0c780000, 
		0x3851107f, 
		0x807ffffe, 
		0x4b8c4b40, 
		0x41100000, 
		0xc1880000, 
		0xc21c0000, 
		0x00800000, 
		0xa8f86af6, 
		0x4fccf9ef, 
		0xc0c00000, 
		0x7f7ffffe, 
		0xc2140000, 
		0x427c0000, 
		0x7b068af6, 
		0xcaf9c95f, 
		0x41a80000, 
		0x7f994589, 
		0x7f9cd11c, 
		0x3f800001, 
		0xd57091f5, 
		0xd67f9886, 
		0x48f46511, 
		0x00040000, 
		0x80000800, 
		0x42740000, 
		0x7fb175e5, 
		0x0c7ff800, 
		0xdb7a2699, 
		0x969779c6, 
		0x41500000, 
		0x3986d43f, 
		0x50098993, 
		0x0c7ff000, 
		0xc6ad7354, 
		0x7fc00001, 
		0x4445c350, 
		0x80000000, 
		0xff800000, 
		0xffa870e2, 
		0x41f00000, 
		0x553161f2, 
		0xc0e00000, 
		0x80000008, 
		0x80000020, 
		0xcccccccc, 
		0x41f00000, 
		0x00000000, 
		0x00358cb1, 
		0x0e00ffff, 
		0xaf2d0392, 
		0x9db2b7d2, 
		0x0573f384, 
		0x69c8f888, 
		0xb5cec2b8, 
		0x0d000ff0, 
		0x42540000, 
		0x3c4b4bfb, 
		0x42100000, 
		0x80000008, 
		0xb601fc33, 
		0xd2babf03, 
		0x3b8d13a2, 
		0x41a80000, 
		0x41e00000, 
		0x00200000, 
		0x00000020, 
		0xb434a4f6, 
		0x0c600000, 
		0x00ffffff, 
		0x00000200, 
		0xfcfc38ab, 
		0xb93bc5e7, 
		0xff800000, 
		0x40000000, 
		0x807fffff, 
		0x423c0000, 
		0x42480000, 
		0xc2240000, 
		0xb20d5d3d, 
		0xff7ffffe, 
		0xc1980000, 
		0xd736ab7e, 
		0x80800000, 
		0x00020000, 
		0x55c44ff7, 
		0xfff885a5, 
		0xc23c0000, 
		0x0c7ff000, 
		0x9bb1bd9f, 
		0x33333333, 
		0xc1f00000, 
		0x7f800000, 
		0x50b55fa4, 
		0xfa46b5f6, 
		0xffedd09b, 
		0x7f7fffff, 
		0x0f7ffffc, 
		0xffc00000, 
		0x7f000000, 
		0x41300000, 
		0x41c80000, 
		0x0e00003f, 
		0x00000000, 
		0x424c0000, 
		0xc07b0dd8, 
		0xc26c0000, 
		0x7f000000, 
		0xffffffff, 
		0x00004000, 
		0x80000000, 
		0x02cbd5b2, 
		0x0e07ffff, 
		0x42300000, 
		0x0c7fffc0, 
		0x42280000, 
		0xff7ffffe, 
		0x00000800, 
		0x0f7ffffd, 
		0x80000000, 
		0xc1980000, 
		0x0e0fffff, 
		0x201cf251, 
		0x7892a3ff, 
		0x1c85fc3c, 
		0x00800003, 
		0x40a00000, 
		0xff800000, 
		0xac5827a5, 
		0x00001000, 
		0x1743ee85, 
		0x80000000, 
		0x80000020, 
		0x827c0d95, 
		0x911fb98c, 
		0x80800000, 
		0x80002000, 
		0xffc00001, 
		0x934d2c9e, 
		0x42a266fa, 
		0xc41c649d, 
		0x00400000, 
		0x8af6c349, 
		0xfb23b576, 
		0x80000001, 
		0xd6e7d19a, 
		0x0c7fffc0, 
		0xffffffff, 
		0xc025443d, 
		0x0f7ffffd, 
		0x00020000, 
		0x41700000, 
		0xcb8c4b40, 
		0x42ab75ae, 
		0x80000000, 
		0xbe5fa9ce, 
		0x0e0003ff, 
		0x369517b3, 
		0x7f800000, 
		0x5c8e136f, 
		0xa00a4809, 
		0xc2380000, 
		0x00000000, 
		0x3f028f5c, 
		0x7f800000, 
		0x80000000, 
		0x80ffffff  
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
		0x00000195, 
		0x000001c1, 
		0x0000007b, 
		0x0000029d, 
		0x0000013d, 
		0x0000025f, 
		0x00000019, 
		0x000000f5, 
		0x000002dd, 
		0x000000e1, 
		0x0000001f, 
		0x000001c5, 
		0x0000003b, 
		0x00000179, 
		0x0000014f, 
		0x0000025b, 
		0x000002eb, 
		0x000000f3, 
		0x000003dd, 
		0x00000011, 
		0x000003af, 
		0x000003cb, 
		0x000000fb, 
		0x00000069, 
		0x00000157, 
		0x000001f5, 
		0x00000011, 
		0x000002b5, 
		0x0000039d, 
		0x00000387, 
		0x0000007b, 
		0x0000000b, 
		0x00000207, 
		0x0000026b, 
		0x0000036f, 
		0x0000023b, 
		0x000000b9, 
		0x000001fd, 
		0x0000015d, 
		0x0000024f, 
		0x00000033, 
		0x000002cb, 
		0x0000015b, 
		0x000003a9, 
		0x0000019d, 
		0x000002ed, 
		0x000003fd, 
		0x000003eb, 
		0x000003cb, 
		0x00000013, 
		0x00000399, 
		0x00000355, 
		0x00000171, 
		0x0000021d, 
		0x00000333, 
		0x000000cd, 
		0x00000293, 
		0x00000139, 
		0x000002ed, 
		0x000003f1, 
		0x00000091, 
		0x000002bf, 
		0x00000005, 
		0x00000223, 
		0x00000379, 
		0x000001d9, 
		0x000002ab, 
		0x0000032d, 
		0x000000c7, 
		0x000000fb, 
		0x000002b7, 
		0x00000237, 
		0x000002d1, 
		0x00000209, 
		0x00000183, 
		0x00000145, 
		0x000003df, 
		0x0000010d, 
		0x0000022d, 
		0x000003e3, 
		0x0000010b, 
		0x000000b9, 
		0x0000030d, 
		0x000003bb, 
		0x00000047, 
		0x000001c9, 
		0x00000055, 
		0x000002ef, 
		0x0000023b, 
		0x0000002f, 
		0x0000037f, 
		0x000000e7, 
		0x0000024d, 
		0x000000a7, 
		0x000003b5, 
		0x000001f1, 
		0x0000025b, 
		0x0000032b, 
		0x00000081, 
		0x00000019, 
		0x00000107, 
		0x0000015b, 
		0x000002ab, 
		0x000002f1, 
		0x00000379, 
		0x0000002f, 
		0x0000026f, 
		0x0000035d, 
		0x00000061, 
		0x000002f1, 
		0x00000117, 
		0x00000315, 
		0x00000299, 
		0x000002f1, 
		0x000001fd, 
		0x0000038b, 
		0x00000269, 
		0x000001e3, 
		0x00000305, 
		0x0000031b, 
		0x000002d1, 
		0x000003c1, 
		0x000000db, 
		0x000003bf, 
		0x00000395, 
		0x000001c5, 
		0x0000019f, 
		0x00000337, 
		0x0000027f, 
		0x00000047, 
		0x0000001b, 
		0x0000022b, 
		0x00000281, 
		0x00000087, 
		0x000003b3, 
		0x00000311, 
		0x000000ad, 
		0x000000fd, 
		0x00000203, 
		0x0000029b, 
		0x000000af, 
		0x00000277, 
		0x000003af, 
		0x0000004f, 
		0x000000d1, 
		0x00000041, 
		0x000003bd, 
		0x0000011d, 
		0x000003e3, 
		0x0000019f, 
		0x00000391, 
		0x00000223, 
		0x000003eb, 
		0x00000107, 
		0x000003af, 
		0x000003ad, 
		0x000002d7, 
		0x000003a9, 
		0x00000003, 
		0x0000000b, 
		0x0000004f, 
		0x0000017f, 
		0x00000035, 
		0x000002b1, 
		0x000000f9, 
		0x000002df, 
		0x00000235, 
		0x000002ab, 
		0x00000263, 
		0x000000dd, 
		0x000002f1, 
		0x0000012b, 
		0x0000006b, 
		0x00000251, 
		0x000000d5, 
		0x00000179, 
		0x0000019b, 
		0x000003e7, 
		0x00000185, 
		0x00000179, 
		0x000000f1, 
		0x00000249, 
		0x0000007f, 
		0x0000009d, 
		0x000003bd, 
		0x000002b3, 
		0x000000f1, 
		0x00000241, 
		0x000003e7, 
		0x00000241, 
		0x000001e9, 
		0x00000369, 
		0x000001bd, 
		0x00000083, 
		0x0000016f, 
		0x00000319, 
		0x000000d7, 
		0x0000022f, 
		0x00000359, 
		0x000001eb, 
		0x00000385, 
		0x000003fb, 
		0x000003e9, 
		0x000000eb, 
		0x000003b5, 
		0x000003f9, 
		0x00000211, 
		0x00000191, 
		0x0000028b, 
		0x000001bf, 
		0x000003a9, 
		0x0000010f, 
		0x00000131, 
		0x00000355, 
		0x00000009, 
		0x0000032d, 
		0x000003d7, 
		0x00000163, 
		0x00000389, 
		0x00000117, 
		0x0000025d, 
		0x0000039d, 
		0x00000001, 
		0x00000017, 
		0x00000325, 
		0x000000c7, 
		0x000002a5, 
		0x00000067, 
		0x000003fd, 
		0x000001b1, 
		0x00000005, 
		0x0000017d, 
		0x00000195, 
		0x0000029d, 
		0x00000167, 
		0x000000a1, 
		0x000000b3, 
		0x00000013, 
		0x0000037f, 
		0x0000035f, 
		0x0000024b, 
		0x000002f9, 
		0x0000019f, 
		0x000000b7, 
		0x00000357, 
		0x000003b9, 
		0x00000361, 
		0x00000209, 
		0x00000129, 
		0x00000321, 
		0x000000b3, 
		0x000000b5, 
		0x000002cd, 
		0x000003fb, 
		0x00000091, 
		0x000003b5  
};

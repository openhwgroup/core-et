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
			"li x5, 379\n"
			"faddi.pi f2, f0, 476\n"
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
			"faddi.pi f2, f2, 128\n"
			"fexp.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"fmin.pi f30, f23, f23\n"
			"fadd.pi f25, f31, f26\n"
			"fadd.pi f26, f23, f22\n"
			"fmul.ps f20, f20, f21\n"
			"fmax.pi f22, f24, f25\n"
			"fexp.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"2:\n"
			"fexp.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"bne x0, x0, 2b\n"
			"3:\n"
			"fexp.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"bne x0, x0, 3b\n"
			"fnmadd.ps f26, f22, f24, f26\n"
			"fmsub.ps f25, f23, f27, f26\n"
			"fmadd.ps f25, f29, f26, f29\n"
			"fmadd.ps f29, f27, f30, f27\n"
			"fnmadd.ps f30, f23, f25, f25\n"
			"fnmsub.ps f28, f22, f26, f23\n"
			"fmadd.ps f24, f30, f28, f24\n"
			"fexp.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"1:\n"
			"fexp.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"bne x0, x0, 1b\n"
			"2:\n"
			"fexp.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"bne x0, x0, 2b\n"
			"fmsub.ps f26, f28, f31, f31\n"
			"fmadd.ps f26, f28, f30, f25\n"
			"fnmsub.ps f25, f29, f26, f31\n"
			"fnmsub.ps f30, f30, f31, f31\n"
			"fmsub.ps f29, f29, f27, f31\n"
			"fmadd.ps f27, f30, f31, f27\n"
			"fnmsub.ps f29, f29, f27, f29\n"
			"fmsub.ps f30, f26, f25, f25\n"
			"fnmadd.ps f26, f29, f28, f30\n"
			"fnmadd.ps f27, f25, f29, f26\n"
			"fexp.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"fmul.ps f27, f26, f28\n"
			"fmul.pi f27, f26, f28\n"
			"fmax.ps f29, f30, f31\n"
			"fadd.ps f27, f27, f26\n"
			"fmax.pi f28, f28, f27\n"
			"fsub.pi f30, f31, f31\n"
			"fmax.pi f30, f30, f29\n"
			"fadd.ps f31, f27, f26\n"
			"fmul.pi f28, f31, f26\n"
			"4:\n"
			"fexp.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"bne x0, x0, 4b\n"
			"fexp.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"fmin.pi f31, f30, f29\n"
			"fadd.ps f29, f31, f29\n"
			"fand.pi f29, f30, f28\n"
			"fsub.pi f29, f29, f29\n"
			"fmul.pi f29, f30, f30\n"
			"fsub.ps f29, f29, f28\n"
			"fmax.ps f30, f31, f28\n"
			"fmax.pi f31, f29, f31\n"
			"fsub.ps f31, f29, f28\n"
			"2:\n"
			"fexp.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"bne x0, x0, 2b\n"
			"fsub.pi f31, f30, f30\n"
			"fmul.ps f31, f30, f30\n"
			"fand.pi f30, f30, f30\n"
			"fmin.ps f31, f29, f30\n"
			"fsub.pi f29, f29, f29\n"
			"fmul.ps f29, f31, f31\n"
			"fsub.ps f30, f30, f30\n"
			"fmin.pi f29, f31, f29\n"
			"fexp.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"4:\n"
			"fexp.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"bne x0, x0, 4b\n"
			"fexp.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
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
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"LBL_EN_COOP_ESR_HID_0:\n"          
			"li x5, 0x80740290\n"  
			"li x31, 1\n"
			"sd x31, 0(x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x0, 0x8\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 576(x15)\n"
			"flw.ps f2, 960(x15)\n"
			"flt.s x27, f10, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 800(x7)\n"
			"flw.ps f22, 0(x7)\n"
			"fmul.ps f22, f9, f22, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 288(x11)\n"
			"flw.ps f23, 352(x11)\n"
			"fmulh.pi f22, f20, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 512(x7)\n"
			"flw.ps f9, 352(x7)\n"
			"fsra.pi f6, f31, f9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 832(x27)\n"
			"flw.ps f10, 544(x27)\n"
			"fltu.pi f31, f9, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 256(x15)\n"
			"fcvt.l.s x26, f23, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 960(x11)\n"
			"frcp.ps f9, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 608(x13)\n"
			"flw.ps f6, 576(x13)\n"
			"fsgnj.s f22, f3, f6\n" 
			"la x9, rand_int_32\n"
			"flw.ps f15, 512(x9)\n"
			"flw.ps f6, 800(x9)\n"
			"fxor.pi f20, f15, f6\n" 
			"maskpopc x16, m1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 512(x9)\n"
			"flw.ps f15, 160(x9)\n"
			"fmsub.ps f3, f2, f15, f2, rdn\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 128(x19)\n"
			"flog.ps f7, f0\n" 
			"la x16, rand_ieee754_16\n"
			"flw.ps f15, 960(x16)\n"
			"fcvt.ps.f16 f10, f15\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f20, 800(x9)\n"
			"fcvt.ps.f16 f9, f20\n" 
			"li x5, 0x609deacc042dad84\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc7a1377dd0680aac\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 864(x15)\n"
			"fcvt.pw.ps f0, f31, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 128(x26)\n"
			"flw.ps f2, 800(x26)\n"
			"fmin.pi f16, f23, f2\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 544(x27)\n"
			"flw.ps f12, 704(x27)\n"
			"flw.ps f17, 224(x27)\n"
			"fnmsub.ps f14, f26, f12, f17, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 416(x20)\n"
			"fcvt.ps.pwu f14, f21, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 256(x6)\n"
			"flw.ps f26, 448(x6)\n"
			"flw.ps f21, 576(x6)\n"
			"fmadd.s f1, f6, f26, f21, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 832(x11)\n"
			"flw.ps f1, 864(x11)\n"
			"flw.ps f20, 608(x11)\n"
			"fmsub.s f31, f6, f1, f20, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 544(x12)\n"
			"flw.ps f10, 736(x12)\n"
			"flt.pi f17, f1, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 64(x6)\n"
			"flw.ps f5, 128(x6)\n"
			"fdiv.s f22, f12, f5, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 128(x6)\n"
			"fpackrepb.pi f14, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"flem.ps m6, f24, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 544(x22)\n"
			"flw.ps f26, 224(x22)\n"
			"fle.s x11, f31, f26\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 736(x7)\n"
			"fsatu8.pi f21, f24\n" 
			"la x19, rand_int_32\n"
			"flw.ps f17, 32(x19)\n"
			"flw.ps f29, 544(x19)\n"
			"fand.pi f25, f17, f29\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 288(x20)\n"
			"flw.ps f12, 992(x20)\n"
			"fmsub.ps f29, f8, f12, f12, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 800(x7)\n"
			"flw.ps f6, 352(x7)\n"
			"fadd.s f21, f20, f6, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 384(x10)\n"
			"fxor.pi f29, f29, f29\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 960(x10)\n"
			"fclass.ps f31, f24\n" 
			"li x28, 0xb585ee69cbb97641\n"
			"fcvt.s.l f5, x28, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
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
			"LBL_HPM_4_CORE_BRANCHES0_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f23, 416(x14)\n"
			"flw.ps f24, 928(x14)\n"
			"fmaxu.pi f18, f23, f24\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 992(x13)\n"
			"flw.ps f5, 608(x13)\n"
			"fltu.pi f19, f4, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 704(x26)\n"
			"flw.ps f28, 192(x26)\n"
			"fdiv.ps f23, f4, f28, rdn\n" 
			"maskpopcz x28, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x7, rand_int_32\n"
			"flw.ps f5, 800(x7)\n"
			"faddi.pi f19, f5, 0x1cd\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 608(x13)\n"
			"flw.ps f4, 704(x13)\n"
			"fsgnjn.ps f6, f5, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 480(x28)\n"
			"flw.ps f9, 864(x28)\n"
			"fltu.pi f9, f10, f9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 608(x18)\n"
			"flw.ps f17, 32(x18)\n"
			"flw.ps f5, 864(x18)\n"
			"fmadd.s f17, f19, f17, f5, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 192(x26)\n"
			"flw.ps f15, 896(x26)\n"
			"fxor.pi f0, f9, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 448(x26)\n"
			"flw.ps f18, 192(x26)\n"
			"flw.ps f29, 864(x26)\n"
			"fnmadd.s f24, f5, f18, f29, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 736(x17)\n"
			"flw.ps f0, 704(x17)\n"
			"fmulh.pi f4, f6, f0\n" 
			"li x5, 0xf99a67db404feac0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x21173d33a61f67f0\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f6, 448(x20)\n"
			"flw.ps f17, 640(x20)\n"
			"fle.pi f24, f6, f17\n" 
			"mova.x.m x18\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f10, 896(x26)\n"
			"fcvt.ps.f16 f9, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.ps m7, f7, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x14, m7\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 896(x22)\n"
			"flw.ps f15, 320(x22)\n"
			"flw.ps f28, 736(x22)\n"
			"fnmsub.ps f13, f12, f15, f28, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"maskor m4, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 544(x19)\n"
			"flw.ps f12, 800(x19)\n"
			"flw.ps f29, 736(x19)\n"
			"fcmov.ps f25, f2, f12, f29\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_3:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 672(x14)\n"
			"frcp.ps f0, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 320(x12)\n"
			"flw.ps f29, 0(x12)\n"
			"fmax.s f2, f13, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 480(x12)\n"
			"fsin.ps f9, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 480(x22)\n"
			"flw.ps f31, 448(x22)\n"
			"fxor.pi f30, f17, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 640(x7)\n"
			"flw.ps f2, 416(x7)\n"
			"flw.ps f21, 32(x7)\n"
			"fnmsub.s f2, f17, f2, f21, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 448(x7)\n"
			"fswizz.ps f17, f2, 0xb\n" 
			"li x7, 0x22f9d4b532a01798\n"
			"fcvt.s.w f2, x7, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 448(x23)\n"
			"fpackrepb.pi f0, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 384(x22)\n"
			"fclass.s x11, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 672(x19)\n"
			"flw.ps f21, 864(x19)\n"
			"fltu.pi f31, f12, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 128(x23)\n"
			"flw.ps f9, 64(x23)\n"
			"feq.ps f20, f30, f9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 0(x11)\n"
			"flw.ps f17, 384(x11)\n"
			"fsgnjn.s f29, f18, f17\n" 
			"la x11, rand_int_32\n"
			"flw.ps f12, 320(x11)\n"
			"fsatu8.pi f29, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f5, 288(x10)\n"
			"fsrli.pi f25, f5, 0x1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 864(x12)\n"
			"fdiv.ps f11, f0, f0, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 864(x22)\n"
			"flw.ps f17, 352(x22)\n"
			"fsgnj.s f8, f5, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 480(x20)\n"
			"fcvt.wu.s x10, f2, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 256(x27)\n"
			"fandi.pi f0, f8, 0x1b2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 512(x7)\n"
			"flw.ps f5, 896(x7)\n"
			"feq.s x16, f31, f5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f17, 480(x18)\n"
			"flw.ps f11, 896(x18)\n"
			"fmax.pi f14, f17, f11\n" 
			"li x27, 0xc4d71f186a2853c5\n"
			"fcvt.s.w f26, x27, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 224(x18)\n"
			"ffrc.ps f0, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 256(x22)\n"
			"flw.ps f18, 128(x22)\n"
			"fmulh.pi f18, f17, f18\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 928(x5)\n"
			"fsrai.pi f25, f0, 0xb\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 192(x20)\n"
			"flw.ps f2, 128(x20)\n"
			"fsub.s f17, f17, f2, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 608(x10)\n"
			"faddi.pi f31, f8, 0x123\n" 
			"li x5, 0xfa5f\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 928(x7)\n"
			"fcvt.lu.s x10, f17, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 896(x20)\n"
			"flw.ps f12, 384(x20)\n"
			"for.pi f19, f10, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 416(x20)\n"
			"flw.ps f5, 800(x20)\n"
			"flt.s x18, f17, f5\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_634885_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x5 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x7 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x18 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xa << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xe << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x54 << TENSOR_FMA32_SCP_LOC_B) | (0x46 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f11, 32(x26)\n"
			"fpackrepb.pi f28, f11\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 864(x25)\n"
			"flw.ps f28, 128(x25)\n"
			"fsgnjx.s f12, f9, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 736(x25)\n"
			"fcvt.wu.s x10, f9, rmm\n" 
			"fbci.pi f31, 0x5a361\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 0(x14)\n"
			"flw.ps f30, 192(x14)\n"
			"fmulh.pi f25, f27, f30\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 0(x5)\n"
			"fpackrepb.pi f21, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 864(x10)\n"
			"fclass.s x25, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 704(x6)\n"
			"flw.ps f31, 320(x6)\n"
			"fmax.s f27, f22, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 192(x19)\n"
			"flw.ps f7, 896(x19)\n"
			"flw.ps f31, 448(x19)\n"
			"fmadd.ps f25, f9, f7, f31, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 960(x23)\n"
			"flw.ps f31, 96(x23)\n"
			"fmax.s f22, f21, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"flem.ps m4, f25, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 416(x6)\n"
			"flw.ps f25, 704(x6)\n"
			"fmsub.ps f4, f12, f25, f25, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 96(x23)\n"
			"frsq.ps f4, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 704(x26)\n"
			"fswizz.ps f23, f21, 0xe\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 256(x19)\n"
			"flw.ps f7, 160(x19)\n"
			"feq.s x5, f23, f7\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 384(x25)\n"
			"flw.ps f7, 384(x25)\n"
			"fmul.s f23, f9, f7, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"li x12, 0xb5d9683afb3b51c5\n"
			"fcvt.s.wu f10, x12, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 992(x15)\n"
			"flw.ps f1, 704(x15)\n"
			"fadd.ps f6, f19, f1, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 704(x12)\n"
			"flw.ps f1, 512(x12)\n"
			"fsgnjx.ps f19, f29, f1\n" 
			"li x5, 0x1d45\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 896(x10)\n"
			"flw.ps f3, 224(x10)\n"
			"feq.ps f23, f14, f3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 992(x28)\n"
			"fclass.ps f3, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 736(x16)\n"
			"flw.ps f23, 288(x16)\n"
			"fsgnjx.s f4, f27, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 544(x28)\n"
			"fpackrepb.pi f30, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f30, 800(x10)\n"
			"flw.ps f23, 256(x10)\n"
			"fsra.pi f27, f30, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 160(x28)\n"
			"fpackrepb.pi f29, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 224(x26)\n"
			"flw.ps f6, 832(x26)\n"
			"fle.ps f19, f1, f6\n" 
			"li x25, 0xa3ad097371143345\n"
			"fcvt.s.wu f10, x25, rne\n" 
			"li x28, 0xde13ab9e3cfc1c3c\n"
			"fcvt.s.wu f15, x28, rdn\n" 
			"li x5, 0xdae0e54ac2e8caa0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xec3157eeed0694f7\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 128(x15)\n"
			"flw.ps f6, 128(x15)\n"
			"feq.s x16, f3, f6\n" 
			"maskpopcz x28, m5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 448(x6)\n"
			"fsatu8.pi f25, f26\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_6:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 288(x10)\n"
			"frcp.ps f3, f20\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_420762_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x1 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x75 << TENSOR_IMA_SCP_LOC_B) | (0xec << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x0 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xa << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1c << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xe << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"maskxor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f22, 352(x22)\n"
			"fsrai.pi f7, f22, 0x6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 608(x10)\n"
			"flw.ps f8, 896(x10)\n"
			"fmulhu.pi f3, f5, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 192(x6)\n"
			"fnot.pi f19, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 224(x7)\n"
			"flw.ps f17, 224(x7)\n"
			"flw.ps f7, 192(x7)\n"
			"fnmadd.ps f8, f31, f17, f7, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 192(x7)\n"
			"fcvt.f16.ps f3, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 608(x22)\n"
			"flw.ps f4, 576(x22)\n"
			"fmulh.pi f31, f8, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 96(x22)\n"
			"flw.ps f4, 576(x22)\n"
			"flw.ps f19, 448(x22)\n"
			"fnmadd.ps f31, f11, f4, f19, rtz\n" 
			"li x10, 0x85b72e7af36de385\n"
			"fcvt.s.w f14, x10, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f3, 448(x6)\n"
			"flw.ps f19, 288(x6)\n"
			"for.pi f11, f3, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"flem.ps m2, f23, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 512(x7)\n"
			"flw.ps f4, 768(x7)\n"
			"fsub.s f30, f14, f4, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 416(x13)\n"
			"fclass.ps f18, f19\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 288(x11)\n"
			"fsatu8.pi f7, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"fltm.pi m4, f27, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 544(x10)\n"
			"flw.ps f18, 224(x10)\n"
			"fmul.ps f5, f8, f18, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f10, 480(x20)\n"
			"flw.ps f17, 832(x20)\n"
			"fmaxu.pi f31, f10, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 384(x20)\n"
			"fcvt.pw.ps f22, f26, rmm\n" 
			"maskpopcz x20, m2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 256(x7)\n"
			"flw.ps f28, 640(x7)\n"
			"fsgnj.s f26, f31, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"mov.m.x m6, x22, 0x79\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 864(x16)\n"
			"flw.ps f28, 0(x16)\n"
			"fmax.s f31, f8, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 224(x12)\n"
			"fsqrt.ps f24, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 128(x19)\n"
			"flw.ps f26, 192(x19)\n"
			"fsgnjx.s f9, f29, f26\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 832(x28)\n"
			"fcvt.ps.pwu f9, f10, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"mov.m.x m1, x7, 0x5b\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f28, 640(x28)\n"
			"faddi.pi f28, f28, 0x132\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_8:\n"
			"feqm.ps m1, f26, f22\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf3dfab4b4a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 896(x6)\n"
			"flw.ps f10, 704(x6)\n"
			"fadd.s f16, f9, f10, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 704(x19)\n"
			"flw.ps f13, 224(x19)\n"
			"flt.ps f13, f11, f13\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_8:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 608(x6)\n"
			"fexp.ps f9, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 224(x27)\n"
			"fdiv.s f29, f3, f3, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x46 << TENSOR_IMA_SCP_LOC_B) | (0x9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_9:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 544(x9)\n"
			"frcp.ps f11, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 352(x16)\n"
			"fsat8.pi f0, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"masknot m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f10, 0(x16)\n"
			"flw.ps f27, 96(x16)\n"
			"fmul.pi f27, f10, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 448(x5)\n"
			"flw.ps f7, 384(x5)\n"
			"fnmsub.s f1, f30, f7, f30, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 448(x16)\n"
			"flw.ps f16, 768(x16)\n"
			"fmax.ps f27, f26, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 512(x10)\n"
			"flw.ps f13, 864(x10)\n"
			"fsgnjn.ps f16, f10, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 288(x16)\n"
			"flw.ps f14, 800(x16)\n"
			"flw.ps f30, 384(x16)\n"
			"fmsub.ps f13, f13, f14, f30, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 320(x28)\n"
			"flw.ps f16, 64(x28)\n"
			"fmin.ps f14, f27, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 288(x25)\n"
			"fcvt.f16.ps f10, f26\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 64(x7)\n"
			"flw.ps f2, 704(x7)\n"
			"fsgnjx.ps f0, f13, f2\n" 
			"la x25, rand_int_32\n"
			"flw.ps f7, 384(x25)\n"
			"fmin.pi f11, f7, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 672(x9)\n"
			"fclass.s x27, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"maskand m2, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x28, m2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 512(x16)\n"
			"fcvt.pwu.ps f2, f11, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 448(x20)\n"
			"frcp.ps f3, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 640(x5)\n"
			"flw.ps f8, 832(x5)\n"
			"fsgnjx.ps f6, f11, f8\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 288(x13)\n"
			"fslli.pi f15, f19, 0x4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 192(x19)\n"
			"flw.ps f4, 128(x19)\n"
			"fcmovm.ps f5, f5, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 448(x27)\n"
			"flw.ps f3, 896(x27)\n"
			"fmax.s f5, f23, f3\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 608(x28)\n"
			"flog.ps f5, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"mov.m.x m1, x19, 0x3f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f7, 704(x27)\n"
			"flw.ps f11, 480(x27)\n"
			"fmulh.pi f8, f7, f11\n" 
			"maskpopcz x20, m5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 192(x13)\n"
			"flw.ps f8, 480(x13)\n"
			"fle.s x13, f7, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 608(x26)\n"
			"fandi.pi f8, f4, 0xa\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 480(x28)\n"
			"flw.ps f10, 704(x28)\n"
			"fminu.pi f26, f7, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 160(x7)\n"
			"flw.ps f26, 832(x7)\n"
			"fmaxu.pi f17, f10, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 192(x23)\n"
			"fclass.s x10, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_10:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 288(x28)\n"
			"flw.ps f11, 960(x28)\n"
			"fsgnjn.ps f19, f9, f11\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xa << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x19 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xd << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x0 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xb << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 64(x14)\n"
			"fmin.s f11, f2, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 352(x14)\n"
			"fcvt.ps.pwu f11, f17, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 736(x14)\n"
			"fsatu8.pi f5, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f5, 320(x22)\n"
			"flw.ps f30, 416(x22)\n"
			"fmin.pi f27, f5, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 800(x22)\n"
			"flw.ps f11, 384(x22)\n"
			"fsll.pi f30, f13, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 320(x6)\n"
			"flw.ps f31, 736(x6)\n"
			"fcmovm.ps f18, f23, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 288(x11)\n"
			"fround.ps f13, f30, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"maskxor m5, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 512(x28)\n"
			"flw.ps f15, 0(x28)\n"
			"fmax.ps f17, f7, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 864(x12)\n"
			"flt.s x13, f31, f31\n" 
			"li x12, 0xdc04a6e3dd049ca2\n"
			"fcvt.s.wu f1, x12, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 768(x28)\n"
			"flw.ps f3, 800(x28)\n"
			"fcmovm.ps f1, f11, f3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 416(x28)\n"
			"flw.ps f24, 416(x28)\n"
			"fxor.pi f23, f17, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 288(x13)\n"
			"flw.ps f2, 224(x13)\n"
			"fsgnjx.s f2, f23, f2\n" 
			"fbci.pi f5, 0x7b493\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 992(x28)\n"
			"frsq.ps f3, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f26, 416(x11)\n"
			"flw.ps f25, 896(x11)\n"
			"flt.pi f11, f26, f25\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 736(x23)\n"
			"flw.ps f5, 256(x23)\n"
			"fmadd.ps f4, f26, f5, f26, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 64(x11)\n"
			"fclass.ps f29, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 288(x11)\n"
			"flw.ps f21, 800(x11)\n"
			"flw.ps f22, 32(x11)\n"
			"fmsub.ps f26, f5, f21, f22, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 352(x6)\n"
			"faddi.pi f19, f19, 0x9e\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 736(x9)\n"
			"flw.ps f4, 640(x9)\n"
			"fmax.pi f5, f23, f4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 288(x13)\n"
			"fslli.pi f27, f26, 0xf\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 928(x23)\n"
			"flw.ps f6, 736(x23)\n"
			"fmulhu.pi f6, f3, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 224(x23)\n"
			"flw.ps f23, 192(x23)\n"
			"fmul.pi f29, f17, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 992(x15)\n"
			"flw.ps f3, 512(x15)\n"
			"fadd.s f29, f6, f3, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 288(x23)\n"
			"flw.ps f6, 800(x23)\n"
			"fmax.pi f4, f3, f6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 640(x23)\n"
			"flw.ps f23, 928(x23)\n"
			"fsgnjn.ps f29, f21, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 928(x25)\n"
			"frsq.ps f22, f5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 960(x23)\n"
			"flw.ps f26, 576(x23)\n"
			"fmax.ps f26, f23, f26\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 320(x6)\n"
			"flw.ps f5, 736(x6)\n"
			"flw.ps f23, 192(x6)\n"
			"fmadd.s f6, f6, f5, f23, rup\n" 
			"li x14, 0x15cc5c947d886ad9\n"
			"fcvt.s.wu f26, x14, rmm\n" 
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_903488_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f26,  768 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"fbci.ps f26, 0x6de6e\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 32(x6)\n"
			"fround.ps f19, f31, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 512(x27)\n"
			"flw.ps f19, 128(x27)\n"
			"for.pi f11, f28, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 736(x18)\n"
			"fsat8.pi f19, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 704(x7)\n"
			"flw.ps f19, 480(x7)\n"
			"fsgnjx.s f28, f1, f19\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 832(x6)\n"
			"flw.ps f23, 384(x6)\n"
			"fsub.s f4, f22, f23, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 384(x22)\n"
			"flw.ps f4, 576(x22)\n"
			"for.pi f17, f19, f4\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 928(x5)\n"
			"fround.ps f4, f4, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"fltm.pi m4, f22, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f5, 672(x26)\n"
			"flw.ps f2, 224(x26)\n"
			"fmulh.pi f1, f5, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"masknot m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"maskand m0, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f27, 256(x5)\n"
			"flw.ps f28, 544(x5)\n"
			"fminu.pi f23, f27, f28\n" 
			"la x12, rand_int_32\n"
			"flw.ps f23, 800(x12)\n"
			"flw.ps f31, 544(x12)\n"
			"fminu.pi f30, f23, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 960(x5)\n"
			"flw.ps f27, 192(x5)\n"
			"feq.pi f3, f31, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f25,  736 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 704(x23)\n"
			"flw.ps f1, 64(x23)\n"
			"fsgnj.s f19, f19, f1\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 672(x28)\n"
			"flw.ps f15, 416(x28)\n"
			"fsrl.pi f4, f23, f15\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 864(x19)\n"
			"flw.ps f1, 928(x19)\n"
			"fsub.ps f19, f9, f1, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 960(x28)\n"
			"fswizz.ps f3, f6, 0x10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 736(x14)\n"
			"flw.ps f31, 928(x14)\n"
			"fmax.ps f3, f3, f31\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 928(x25)\n"
			"frsq.ps f26, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 992(x14)\n"
			"flw.ps f27, 0(x14)\n"
			"flw.ps f4, 224(x14)\n"
			"fmadd.s f3, f29, f27, f4, rup\n" 
			"li x5, 0x5f00\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 672(x19)\n"
			"flw.ps f15, 544(x19)\n"
			"fsgnjx.s f4, f13, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 384(x9)\n"
			"ffrc.ps f31, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f11, 640(x13)\n"
			"fslli.pi f11, f11, 0x6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 96(x25)\n"
			"flw.ps f13, 704(x25)\n"
			"fxor.pi f13, f29, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 896(x14)\n"
			"flw.ps f1, 864(x14)\n"
			"feq.pi f6, f19, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"mova.m.x x13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 320(x28)\n"
			"flw.ps f31, 256(x28)\n"
			"fmax.ps f27, f27, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 384(x11)\n"
			"fcvt.pw.ps f27, f19, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"flem.ps m4, f11, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x2 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x5 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1c << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xd << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 672(x17)\n"
			"flw.ps f25, 32(x17)\n"
			"fsgnjn.s f21, f29, f25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 576(x6)\n"
			"fsrli.pi f30, f13, 0xb\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 416(x27)\n"
			"flw.ps f30, 512(x27)\n"
			"fsra.pi f0, f21, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 448(x17)\n"
			"ffrc.ps f17, f21\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 448(x23)\n"
			"fcvt.ps.pwu f0, f28, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"masknot m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f28, 128(x14)\n"
			"flw.ps f5, 736(x14)\n"
			"fmax.pi f27, f28, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 384(x20)\n"
			"flw.ps f27, 640(x20)\n"
			"fxor.pi f0, f16, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f29, 736(x27)\n"
			"flw.ps f27, 768(x27)\n"
			"fle.pi f22, f29, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 768(x12)\n"
			"fcvt.ps.pwu f16, f16, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 928(x13)\n"
			"flw.ps f28, 0(x13)\n"
			"feq.s x20, f16, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 224(x27)\n"
			"flw.ps f21, 576(x27)\n"
			"fmax.s f12, f28, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 416(x10)\n"
			"fround.ps f5, f3, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f17, 928(x13)\n"
			"flw.ps f13, 512(x13)\n"
			"for.pi f30, f17, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 0(x14)\n"
			"flw.ps f28, 288(x14)\n"
			"flt.ps f7, f12, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 832(x19)\n"
			"flw.ps f25, 256(x19)\n"
			"fsgnjx.ps f24, f0, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
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
		0x761e0f3d, 
		0xc04eca6c, 
		0x21a1488b, 
		0x52d26411, 
		0x57cd7a6f, 
		0xbc961249, 
		0x8cf3bbf3, 
		0xdc828ed6, 
		0x44b30fe0, 
		0xd877bf35, 
		0xbf31a4f4, 
		0xb974b0a5, 
		0x8a63f951, 
		0x9d52ea75, 
		0x4f31050e, 
		0xe96dac05, 
		0x4795eeeb, 
		0x60ce485a, 
		0x10201c39, 
		0x0bd683d5, 
		0xf5f05112, 
		0xa713519f, 
		0xf0d8fb0b, 
		0xd8377dcc, 
		0x38327db3, 
		0x9708512e, 
		0x97506787, 
		0x97ae27f7, 
		0x6827bb2e, 
		0x7842b9b6, 
		0xe5e7e6b4, 
		0xb06a2811, 
		0xba44c757, 
		0x493624b8, 
		0x7d75665b, 
		0x0c6dca30, 
		0xd1869502, 
		0x5a37b823, 
		0x17a893c9, 
		0x426cd018, 
		0xe3a0c54c, 
		0x510a69d7, 
		0x136a894c, 
		0x4857f797, 
		0x03d4ba7a, 
		0x247d2717, 
		0x90093fd2, 
		0x0c9b43cd, 
		0x7af607ea, 
		0xaf30fc32, 
		0x9e75e6d5, 
		0x3b4ae2f6, 
		0xe46ca9d3, 
		0xa3e2e5df, 
		0x03b24f15, 
		0xc3871722, 
		0x9f82bbdf, 
		0xb4000511, 
		0x6f418ae0, 
		0x96c6ac60, 
		0xda672ea5, 
		0x119ccb5f, 
		0xa8036e74, 
		0xa074be0a, 
		0x160e19a7, 
		0xe39161f3, 
		0x323c273e, 
		0x0be691d5, 
		0xd84ea622, 
		0xf681b750, 
		0xfb30e303, 
		0x90c1120b, 
		0x5ed9fc5c, 
		0xd0843d57, 
		0x0955611f, 
		0x71cfa474, 
		0x78809c9a, 
		0xee693117, 
		0xf32d5670, 
		0xbd30612a, 
		0x20ad68ef, 
		0x361f425f, 
		0xcf76a91a, 
		0x1dfe8289, 
		0xbe4a05d4, 
		0x03e9bd28, 
		0xc4fcacfe, 
		0xb714c6c0, 
		0x5b613212, 
		0xc8cf4f27, 
		0xce457aa0, 
		0x8a28673b, 
		0xa46049bb, 
		0xcef868dd, 
		0xe6135d40, 
		0xb9c82927, 
		0x806cdc09, 
		0x3a4534e9, 
		0x4dc9f906, 
		0x54bcc668, 
		0xfac85687, 
		0xd505a23e, 
		0x523457d0, 
		0xf512f269, 
		0x6718d154, 
		0x608c324d, 
		0xe0257115, 
		0xabd04765, 
		0x2e65b6f0, 
		0xf9b33934, 
		0x05613a47, 
		0xa3b53b93, 
		0xfe229064, 
		0x4520e284, 
		0xbb439a83, 
		0x7db7960f, 
		0xfe712aea, 
		0x42f05071, 
		0xf4e44b8e, 
		0x3caa2787, 
		0xb332b362, 
		0x54671fae, 
		0xfbd470d8, 
		0x451ebdd5, 
		0x81c186b9, 
		0x0477ec10, 
		0xdddcb9be, 
		0xc84ade0c, 
		0xe639d985, 
		0x0f63ff7f, 
		0xa98ee8c1, 
		0xf10fafdd, 
		0x1bdaeb96, 
		0xe22d17fa, 
		0x95c0c5e6, 
		0x3e81bf81, 
		0xda4638d1, 
		0x5dea9e41, 
		0xcd07d1b4, 
		0x06a64f76, 
		0x280b51d3, 
		0xb91ccc15, 
		0xfbd4ea0b, 
		0x7363dc0d, 
		0x3813e82d, 
		0x78704b64, 
		0x4bc8523e, 
		0x2105aa2b, 
		0x16e19e1f, 
		0xc4eb146e, 
		0x6ec5709d, 
		0xf3ea42d9, 
		0x8c9f7f46, 
		0x879e8b65, 
		0x587ac2a8, 
		0xef3b59bf, 
		0x1dc9a3d3, 
		0x7443adcc, 
		0x7a584159, 
		0xcad1c1f0, 
		0xd9383084, 
		0xe6029f99, 
		0xa3a51803, 
		0xc1dc5502, 
		0xd1b108ee, 
		0x560ec3ab, 
		0x466a6b77, 
		0x3c9542b9, 
		0xa38306bf, 
		0x2f3f6cdd, 
		0xd83e6998, 
		0x7551eeb3, 
		0xa2770596, 
		0xfa46d639, 
		0x59893108, 
		0x08381748, 
		0x26f66267, 
		0xdbc022dc, 
		0x23fd04ce, 
		0x68c959a2, 
		0xf43912fd, 
		0x9d83a8c0, 
		0xdb92acc3, 
		0xcef724b5, 
		0x64e0c5ed, 
		0x2fb8c398, 
		0x243be120, 
		0x00fd1015, 
		0xcb27f9e3, 
		0x2a1f57a6, 
		0x7fe0081b, 
		0x23b44822, 
		0xad5dec11, 
		0xc4193217, 
		0x535143f3, 
		0x839b6cbc, 
		0x16afc9ab, 
		0x4a42920f, 
		0xdf63163b, 
		0x25026cb6, 
		0x6f3716f3, 
		0xc50bf00a, 
		0x9753fa37, 
		0x65f311e0, 
		0xb32a281b, 
		0xe9fd6752, 
		0xff166f4a, 
		0xc5663135, 
		0x2d54e596, 
		0x48cb89ac, 
		0x4e88b2d1, 
		0xd0c962e2, 
		0xdbdfa142, 
		0x3bc0c870, 
		0x835dcf84, 
		0x15840a37, 
		0xc140c650, 
		0x3bfbbe2e, 
		0x1d8094cf, 
		0x3dda3283, 
		0x315ae973, 
		0xbfb8f1d8, 
		0x1df71103, 
		0x58bba6ec, 
		0xd6d56275, 
		0x696bc9d7, 
		0x9335039a, 
		0xb02fa897, 
		0x5a3f9d66, 
		0xd6557ab3, 
		0xace6feb8, 
		0xcc863b15, 
		0x23bec8e7, 
		0xaafce730, 
		0xd65028e6, 
		0x5ba0f03d, 
		0xfd46dad9, 
		0x5ea7704a, 
		0x6054221f, 
		0x9f4d7cb5, 
		0x120717ff, 
		0x39af3846, 
		0xb843dc5c, 
		0x2def7454, 
		0x388d2360, 
		0x26c33942, 
		0x9fea1e62, 
		0xb5cb9e0f, 
		0x215dd15d, 
		0x9908fd50, 
		0x8ddd3cac, 
		0xf9e777e2, 
		0xa3b9db62, 
		0xecce5daf, 
		0x5bc69263, 
		0x8afa4450  
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
		0x4c40, 
		0x5060, 
		0x1854, 
		0x31dc, 
		0x5340, 
		0xd360, 
		0x4cc0, 
		0x61f1, 
		0xd240, 
		0x5040, 
		0xc800, 
		0x5000, 
		0x5f63, 
		0x59e2, 
		0xc500, 
		0xfdd3, 
		0x53c0, 
		0x0000, 
		0xd240, 
		0x4880, 
		0x4dc0, 
		0x0000, 
		0x0000, 
		0x73a6, 
		0x4c80, 
		0x4c00, 
		0x4f40, 
		0x5200, 
		0x7c00, 
		0xd2a0, 
		0xa817, 
		0x5340, 
		0x5200, 
		0x3ca3, 
		0xc132, 
		0xc500, 
		0x5020, 
		0xc400, 
		0x20bb, 
		0x5340, 
		0x7ce1, 
		0xce80, 
		0x2fe9, 
		0xb011, 
		0x4880, 
		0xcfc0, 
		0xb5f5, 
		0x1f48, 
		0xae19, 
		0x4e00, 
		0x4000, 
		0x4a00, 
		0xf98c, 
		0xd240, 
		0x53c0, 
		0xce80, 
		0x0000, 
		0xc1d4, 
		0xd4c4, 
		0x0000, 
		0x52e0, 
		0xef4a, 
		0xd2c0, 
		0x4a80, 
		0x4dc0, 
		0x801d, 
		0xcb00, 
		0xcf80, 
		0x0000, 
		0xfa8e, 
		0xf871, 
		0x258e, 
		0x4d00, 
		0x4d80, 
		0xe808, 
		0x8000, 
		0x8d51, 
		0x34ea, 
		0xa825, 
		0x2e02, 
		0x4200, 
		0x5020, 
		0xccc0, 
		0xd280, 
		0xd380, 
		0x4a80, 
		0xd180, 
		0xd240, 
		0xd522, 
		0xc900, 
		0x4d00, 
		0xd160, 
		0x53c0, 
		0x8000, 
		0xfc00, 
		0x4980, 
		0x5140, 
		0x5060, 
		0x8000, 
		0x1b8e, 
		0xd2a0, 
		0xa850, 
		0x6573, 
		0x4dc0, 
		0x4a00, 
		0x4e80, 
		0x07bd, 
		0x8000, 
		0x4cc0, 
		0xfc00, 
		0xcb00, 
		0x50e0, 
		0xd0c0, 
		0xc900, 
		0xc880, 
		0x4cc0, 
		0x5300, 
		0xd140, 
		0xfc00, 
		0x80ba, 
		0x7d46, 
		0xc800, 
		0xd2c0, 
		0x22a5, 
		0xd59c, 
		0x83eb, 
		0x0000, 
		0x4d72, 
		0x5060, 
		0xd534, 
		0xcd00, 
		0x05a8, 
		0x5380, 
		0x4c00, 
		0x7c00, 
		0xda68, 
		0x8000, 
		0xc500, 
		0x7ee9, 
		0x1a72, 
		0x52a0, 
		0xcc80, 
		0x5080, 
		0xf6a4, 
		0xc600, 
		0x6b61, 
		0x7c0c, 
		0x52c0, 
		0xcc80, 
		0xc807, 
		0xd360, 
		0x4d00, 
		0xd140, 
		0xd340, 
		0x8000, 
		0x5140, 
		0x300a, 
		0xd340, 
		0x1f7a, 
		0xd280, 
		0xb77d, 
		0x51c0, 
		0x5f7e, 
		0xcb80, 
		0xd120, 
		0x4c40, 
		0x4e00, 
		0xe42d, 
		0x0000, 
		0xcb80, 
		0x5280, 
		0x4f40, 
		0xfa3c, 
		0x1d96, 
		0xd2a0, 
		0x8000, 
		0x8000, 
		0xd0a0, 
		0xcf40, 
		0x0000, 
		0xd280, 
		0x8000, 
		0xcdc0, 
		0xcbae, 
		0x4800, 
		0xfc00, 
		0xd080, 
		0xc79f, 
		0xd060, 
		0x4f40, 
		0xccc0, 
		0xd3a0, 
		0xccc0, 
		0x50b7, 
		0x0f06, 
		0xcfc0, 
		0xcdc0, 
		0xbc4f, 
		0x03ee, 
		0xd180, 
		0xd1a0, 
		0x5380, 
		0x4a68, 
		0x5040, 
		0xce00, 
		0x2562, 
		0xd260, 
		0xcd80, 
		0xd1a0, 
		0x838a, 
		0xd120, 
		0x5100, 
		0x4c40, 
		0xcc00, 
		0x5240, 
		0xcec0, 
		0x8000, 
		0xfbe8, 
		0x4df8, 
		0xbc00, 
		0x5340, 
		0x0000, 
		0x6c70, 
		0x4d40, 
		0x8000, 
		0x5320, 
		0xcfc0, 
		0xd1a0, 
		0x84f0, 
		0xecaa, 
		0x2002, 
		0xcf00, 
		0x4c80, 
		0x50c0, 
		0x4a00, 
		0x4a00, 
		0xc700, 
		0x4e80, 
		0xd1c0, 
		0x4d40, 
		0xce00, 
		0xc980, 
		0x4e80, 
		0x4e40, 
		0xdb6e, 
		0x0a6c, 
		0xd3e0, 
		0x5e49, 
		0x5afd, 
		0xbcc3, 
		0x8ea3, 
		0x850b, 
		0x4d40, 
		0x4e00, 
		0xd100, 
		0xcd00, 
		0xd0a0, 
		0xd2e0, 
		0xcd80, 
		0xd320, 
		0x0280, 
		0xc500, 
		0xd040, 
		0xd060, 
		0x4d40, 
		0x4500, 
		0x799c, 
		0xd1a0, 
		0xca00, 
		0x50c0, 
		0x8ab5, 
		0x4500, 
		0x5060, 
		0x4eb3, 
		0x5380, 
		0x4d00, 
		0xd380, 
		0xc723, 
		0x5300, 
		0xbc00, 
		0x7c60, 
		0xc400, 
		0xd2c0, 
		0x4e80, 
		0x86b3, 
		0x9b56, 
		0xcbfd, 
		0xd1c0, 
		0x51a0, 
		0xc222, 
		0x5360, 
		0x6cea, 
		0x4d40, 
		0x4a00, 
		0x5040, 
		0xd300, 
		0xe5d7, 
		0x1659, 
		0x4d40, 
		0xd220, 
		0x4e40, 
		0xd000, 
		0xd140, 
		0x782d, 
		0xe48b, 
		0xd140, 
		0x67cb, 
		0xd340, 
		0x4980, 
		0x4b80, 
		0x0000, 
		0x3835, 
		0x5020, 
		0x9a23, 
		0x8000, 
		0x12ad, 
		0x3866, 
		0x8000, 
		0x3ede, 
		0xfc00, 
		0xca80, 
		0x0000, 
		0x0000, 
		0xcc80, 
		0x092e, 
		0xce00, 
		0xcd00, 
		0x8000, 
		0x1f28, 
		0x8740, 
		0xd1c0, 
		0x4000, 
		0x4fc0, 
		0xd280, 
		0xf622, 
		0x4880, 
		0xd1c0, 
		0x8000, 
		0x0bef, 
		0x53e0, 
		0xd280, 
		0x1493, 
		0xd380, 
		0x5340, 
		0x67de, 
		0xd3a0, 
		0x51e0, 
		0xd160, 
		0x9afb, 
		0xd2c0, 
		0x5260, 
		0x8000, 
		0x7c00, 
		0xcb00, 
		0x5100, 
		0x52e0, 
		0xfc00, 
		0x0000, 
		0xd360, 
		0x5060, 
		0xd8cf, 
		0xdc2d, 
		0x6bc3, 
		0x4329, 
		0xd2e0, 
		0xcd00, 
		0xc600, 
		0xc880, 
		0x0000, 
		0x7073, 
		0x207a, 
		0x50a0, 
		0x4a00, 
		0x4200, 
		0x7c11, 
		0xbdfe, 
		0x3d8e, 
		0xc700, 
		0x903c, 
		0x7b64, 
		0x4b00, 
		0x06d4, 
		0xd9b2, 
		0x9584, 
		0x0000, 
		0xd380, 
		0x0000, 
		0x5200, 
		0xcc00, 
		0xd641, 
		0x067c, 
		0x3c00, 
		0x8000, 
		0xd340, 
		0xe8cc, 
		0x4880, 
		0x5300, 
		0xeedc, 
		0x004e, 
		0xcc80, 
		0x4c00, 
		0xce00, 
		0xd160, 
		0x4f40, 
		0x4d80, 
		0x4700, 
		0x51e0, 
		0x5000, 
		0x4700, 
		0x4c00, 
		0x5380, 
		0xd160, 
		0xcd40, 
		0x5200, 
		0x92a0, 
		0x4b80, 
		0x50a0, 
		0x2dce, 
		0x4600, 
		0x7d55, 
		0x3c00, 
		0x4ed4, 
		0x4d00, 
		0x5140, 
		0xc400, 
		0x640d, 
		0x4ec0, 
		0x52a0, 
		0x0000, 
		0x1c83, 
		0xd1e0, 
		0x97da, 
		0x194d, 
		0x5140, 
		0x5100, 
		0xd000, 
		0x9a68, 
		0xcf00, 
		0x1fe4, 
		0x53a0, 
		0x9c0f, 
		0x5000, 
		0xfe51, 
		0x7c00, 
		0x8e2b, 
		0xa93c, 
		0x7cb2, 
		0x8000, 
		0xcc00, 
		0xd0e0, 
		0xf6bc, 
		0x4600, 
		0x0000, 
		0x6006, 
		0x6db4, 
		0x5300, 
		0x53a0, 
		0xcb80, 
		0x8000, 
		0x50a0, 
		0x9843, 
		0xfd04, 
		0xb813, 
		0xdd35, 
		0x5280, 
		0xfc00, 
		0xd3e0, 
		0x4d80, 
		0xd1c0, 
		0xd2a0, 
		0xc880, 
		0xf8b1, 
		0xd3a0, 
		0x4e80, 
		0xc500, 
		0xdb30, 
		0xc900, 
		0x1d90, 
		0xc980, 
		0x4b00, 
		0x2692, 
		0x8990, 
		0x8000, 
		0x7dca, 
		0x4f80, 
		0x798d, 
		0x0662, 
		0x4900, 
		0x89dc, 
		0x4279, 
		0xd3e0, 
		0x53e0, 
		0xd380, 
		0xccc0, 
		0x0000, 
		0xcf80, 
		0x47ce, 
		0x5f0f, 
		0xfc0a, 
		0xb3b8, 
		0x36b1, 
		0xcdc0, 
		0xcec0, 
		0x5140, 
		0x6854, 
		0x5260, 
		0x5320, 
		0xe47b, 
		0xaee2, 
		0x5140, 
		0x5000, 
		0x0451, 
		0xd020, 
		0xc2fb, 
		0x4e80, 
		0xd3c0, 
		0x4880, 
		0x3706, 
		0xd40e, 
		0x4b00, 
		0xce40, 
		0xfc00, 
		0x02a2, 
		0x40de, 
		0x4998, 
		0xe998, 
		0x51c0, 
		0x4900, 
		0xb212, 
		0xd300, 
		0xd220, 
		0xd160, 
		0xce00, 
		0x52c0, 
		0x7c00, 
		0xc700, 
		0x4f85, 
		0x4b00, 
		0x4ec0, 
		0x8f46, 
		0xd200, 
		0xcec0, 
		0x7c00, 
		0x4900, 
		0xd694, 
		0x5380, 
		0x52c0, 
		0xd1e0, 
		0x4193, 
		0xc880, 
		0xd000, 
		0x4f40, 
		0x4000, 
		0x52a0, 
		0x8015, 
		0xc400, 
		0xc700, 
		0xcd40, 
		0x4fc0, 
		0xcdc0, 
		0xcd80, 
		0xc800, 
		0x7086, 
		0x4800, 
		0xff14, 
		0xd000, 
		0xfb28, 
		0x4e80, 
		0x52a0, 
		0xd2e0, 
		0x3fdb, 
		0xd2a0, 
		0xcdab, 
		0x53a0, 
		0xd1e0, 
		0xc200, 
		0xc500, 
		0x3778, 
		0x521f, 
		0xcc80, 
		0x3705, 
		0x4348, 
		0x4c00, 
		0xd0e0, 
		0x5200, 
		0x2a03, 
		0xce80, 
		0xe5cf, 
		0x0000, 
		0xd120, 
		0x5360, 
		0x5120, 
		0xcd00, 
		0xfc00, 
		0xd3a0, 
		0x7c00, 
		0x4800, 
		0xd140, 
		0x4f8a, 
		0x4e80, 
		0xd1c0, 
		0xc8f2, 
		0x4900, 
		0x1424, 
		0x6fef, 
		0xd3c0, 
		0xc880, 
		0x7c2d, 
		0x4dc0, 
		0xf84f, 
		0x4a00, 
		0x4f00, 
		0xcf80, 
		0x5260, 
		0x4c31, 
		0x4700, 
		0x51c0, 
		0x52a0, 
		0x4dc0, 
		0xf2b9, 
		0x5100, 
		0x7c00, 
		0x4980, 
		0x5360, 
		0xfcb6, 
		0xb3f3, 
		0x0000, 
		0x9c8c, 
		0x1006, 
		0xed62, 
		0x8000, 
		0xf3d7, 
		0xd2e0, 
		0x76c4, 
		0x4c80, 
		0xd0a0, 
		0xcf00, 
		0xd0a0, 
		0x5380, 
		0x749b, 
		0xfc00, 
		0x4c80, 
		0x8000, 
		0xd300, 
		0x4c80, 
		0x4a80, 
		0xfc00, 
		0x4dc0, 
		0xccc0, 
		0x3c00, 
		0xd360, 
		0xd260, 
		0xce80, 
		0x99e0, 
		0x4a00, 
		0x3b64, 
		0x52c0, 
		0xcc4b, 
		0xc000, 
		0x5300, 
		0x4fc0, 
		0x81b0, 
		0xd320, 
		0x50e0, 
		0x8000, 
		0x90b1, 
		0xca00, 
		0xd080, 
		0x9dcc, 
		0x0000, 
		0xc200, 
		0x7d26, 
		0x4f40, 
		0x0b7f, 
		0x9ce8, 
		0x4880, 
		0xcfc0, 
		0x4600, 
		0x5280, 
		0x0000, 
		0x5280, 
		0x4c80, 
		0xcf00, 
		0xfd88, 
		0xf875, 
		0x4d7d, 
		0xd2b3, 
		0x5100, 
		0x5100, 
		0xcdc0, 
		0x7c00, 
		0xd180, 
		0xfc00, 
		0xcdc0, 
		0x5340, 
		0xd200, 
		0x031b, 
		0x5280, 
		0xd240, 
		0x5220, 
		0x4ad7, 
		0xd060, 
		0xfc00, 
		0x735d, 
		0xf551, 
		0xd0e0, 
		0xd120, 
		0x51e0, 
		0x4e00, 
		0x5060, 
		0x66dd, 
		0xd189, 
		0x7c00, 
		0x4fc0, 
		0xd320, 
		0xf11a, 
		0x51c0, 
		0x8000, 
		0x51e0, 
		0xfd50, 
		0x4e80, 
		0x7c94, 
		0x7550, 
		0xcfc0, 
		0x4e40, 
		0x813e, 
		0x8088, 
		0x4880, 
		0xc000, 
		0x4400, 
		0xce00, 
		0x4e40, 
		0x5220, 
		0x8401, 
		0x9da1, 
		0x52e0, 
		0x03a7, 
		0xc6e7, 
		0xf061, 
		0xbe5f, 
		0x5260, 
		0x8000, 
		0x50e0, 
		0xd300, 
		0xc069, 
		0xcc80, 
		0x51e0, 
		0x064d, 
		0xcf40, 
		0xd260, 
		0x5300, 
		0x80fe, 
		0xfc00, 
		0xcd40, 
		0xaee8, 
		0x4786, 
		0xd280, 
		0xb3bb, 
		0x51e0, 
		0x8000, 
		0xc900, 
		0x50a0, 
		0xcf40, 
		0x53e0, 
		0xcb80, 
		0x1db1, 
		0x293e, 
		0x525c, 
		0x5080, 
		0xc700, 
		0x5360, 
		0x8000, 
		0xd000, 
		0xcf80, 
		0xd742, 
		0xd2a0, 
		0x5140, 
		0x4c80, 
		0x4b80, 
		0x53c0, 
		0x4d00, 
		0xcf80, 
		0xf830, 
		0x4700, 
		0x5530, 
		0x4f00, 
		0xd060, 
		0xf982, 
		0xd360, 
		0x0000, 
		0x18e4, 
		0x5380, 
		0x53e0, 
		0x5220, 
		0x00b4, 
		0xd1c0, 
		0x0000, 
		0xfc00, 
		0xd164, 
		0x2274, 
		0x4a00, 
		0x51c0, 
		0x8000, 
		0xce00, 
		0x3c00, 
		0xccc0, 
		0x5380, 
		0x4c80, 
		0x6075, 
		0x0000, 
		0x8000, 
		0x4a80, 
		0x7d07, 
		0x5320, 
		0xcc40, 
		0xf416, 
		0x0000, 
		0x4200, 
		0x4880, 
		0x4b80, 
		0x50a0, 
		0x4600, 
		0xd56d, 
		0xcd40, 
		0x7c00, 
		0x8ec5, 
		0x6f0c, 
		0x4980, 
		0x9241, 
		0x8000, 
		0x50e0, 
		0x5220, 
		0xfc00, 
		0x3c00, 
		0x53e0, 
		0x672f, 
		0x4dc0, 
		0xc980, 
		0x5220, 
		0x5240, 
		0x4600, 
		0xce80, 
		0x4b00, 
		0xd0c0, 
		0x34ae, 
		0xcf40, 
		0xd3a0, 
		0x8000, 
		0x9482, 
		0x35ec, 
		0x4bf4, 
		0x8000, 
		0x4f40, 
		0x7c00, 
		0xd080, 
		0x94c5, 
		0x7c00, 
		0x5080, 
		0xd1c0, 
		0x4824, 
		0xbfdb, 
		0xca00, 
		0xd200, 
		0xd180, 
		0x5280, 
		0x3c00, 
		0x5314, 
		0x5200, 
		0x0000, 
		0x0681, 
		0xb85b, 
		0x4c00, 
		0xd82c, 
		0xc600, 
		0xd170, 
		0x5160, 
		0x50a0, 
		0xd270, 
		0x8000, 
		0xca80, 
		0x6318, 
		0xd27f, 
		0xd280, 
		0xbd13, 
		0xca00, 
		0x7c00, 
		0xd3c0, 
		0x4c40, 
		0x4800, 
		0x1d10, 
		0xbdc0, 
		0x8000, 
		0x1bcc, 
		0x4e00, 
		0x2312, 
		0xfc00, 
		0xd0e0, 
		0x3d71, 
		0xd360, 
		0xc600, 
		0xc880, 
		0x905e, 
		0xce00, 
		0x5080, 
		0x50a0, 
		0x4c80, 
		0xe5f0, 
		0xd0c0, 
		0xd1a0, 
		0xed94, 
		0x5240, 
		0x5340, 
		0x52a0, 
		0xd1a0, 
		0x7c00, 
		0xfc00, 
		0x8000, 
		0xca00, 
		0x8000, 
		0x8000, 
		0xccc0, 
		0x4e40, 
		0xd100, 
		0xd200, 
		0xd340, 
		0x0f3f, 
		0xf87e, 
		0x7c00, 
		0x51c0, 
		0x4500, 
		0xc880, 
		0xd2c0, 
		0xd120, 
		0xd180, 
		0xd260, 
		0x0000, 
		0xcc40, 
		0x004e, 
		0x51a0, 
		0x6265, 
		0x4a80, 
		0x4dc0, 
		0x4d80, 
		0x8000, 
		0xd040, 
		0xd3e0, 
		0x4dc0, 
		0xfc00, 
		0x4c80, 
		0xcd00, 
		0x4b00, 
		0x4f24, 
		0x020c, 
		0xa837, 
		0x4c00, 
		0x4d00, 
		0xd37a, 
		0xd300, 
		0xca4a, 
		0x5180, 
		0xd040, 
		0x43de, 
		0x5320, 
		0xcb80, 
		0x4129, 
		0x7c00, 
		0xefd7, 
		0xfc00, 
		0x4dc0, 
		0x5080, 
		0x5040, 
		0xcdc0, 
		0xa64c, 
		0x7eb3, 
		0xd220, 
		0xfc00, 
		0x84af, 
		0xd2a0, 
		0x8000, 
		0x8000, 
		0x7c00, 
		0x10c0, 
		0x043d, 
		0x5140, 
		0xcf80, 
		0x7b9a, 
		0x50e0, 
		0xcec0, 
		0xcb80, 
		0x5000, 
		0x2fd1, 
		0x5160, 
		0x7c00, 
		0x5240, 
		0xc2ed, 
		0x0d62, 
		0x5280, 
		0x8000, 
		0x0862, 
		0x8000, 
		0xd080, 
		0x8dcd, 
		0xd3c0, 
		0xcdc0, 
		0x53e0, 
		0x4500, 
		0x4700, 
		0xf4ab, 
		0xcf40, 
		0x5280, 
		0xcd80, 
		0x5100, 
		0xd100, 
		0xce80, 
		0x4a00, 
		0xd180, 
		0x8990, 
		0x7c00, 
		0xea81, 
		0x7b44, 
		0x4c14, 
		0x5320, 
		0xc5bf, 
		0x4e00, 
		0xd160, 
		0xcb00, 
		0x79e7, 
		0x5180, 
		0xdc7e, 
		0xfa08, 
		0xd0e0, 
		0x4fc0, 
		0xfb99, 
		0xd160, 
		0x5040, 
		0x5160, 
		0xd3a0, 
		0xcd00, 
		0xce00, 
		0x3c4c, 
		0x0000, 
		0xd140, 
		0xcfe6, 
		0x5000, 
		0x5360, 
		0xd220, 
		0x4980, 
		0xb524, 
		0x5000, 
		0x5000, 
		0xd2c0, 
		0x51e0, 
		0xd200, 
		0xd1e0, 
		0x48b0, 
		0x7c9b, 
		0x2d8c, 
		0x92ed, 
		0xd3c6, 
		0x4b00, 
		0x5453, 
		0xfc00, 
		0x1e00, 
		0xc700, 
		0xa23a, 
		0x4600, 
		0x4a00, 
		0x5340, 
		0x4c80, 
		0x52a0, 
		0x4500, 
		0xcb80, 
		0x1d91, 
		0x4010, 
		0xca80, 
		0x0000, 
		0x7c00, 
		0xd340, 
		0x4d40, 
		0x50a0, 
		0x4c80, 
		0x8000, 
		0xcb00, 
		0xcd80, 
		0xd1e0, 
		0x7d12, 
		0x4200, 
		0x4dc0, 
		0xcdc0, 
		0x5100, 
		0x51e0, 
		0xd0c0, 
		0xd0a0, 
		0xd360, 
		0x7c00, 
		0x0000, 
		0xbc00, 
		0x4a80, 
		0x5220, 
		0x53c0, 
		0x53a0, 
		0x4e40, 
		0xd0c0, 
		0x5240, 
		0xcf00, 
		0x53a0, 
		0xf2a0, 
		0x51e0, 
		0xd080, 
		0xd380, 
		0xe83d, 
		0xcf40, 
		0xd0e0, 
		0xd0a0, 
		0x53c0, 
		0xd0c0, 
		0x5320, 
		0x2dd6, 
		0x4500, 
		0x8d04, 
		0x6972, 
		0xbf24, 
		0x0000, 
		0xcf00, 
		0xaba3, 
		0x8f34, 
		0xd2a3, 
		0x9618, 
		0x0000, 
		0x50a0, 
		0xd0a0, 
		0xd3dc, 
		0xcf80, 
		0x4200, 
		0x4400, 
		0x13df, 
		0x0b07, 
		0x7799, 
		0xe6be, 
		0xcfc0, 
		0x5160, 
		0x5120, 
		0x4ec0, 
		0x7c00, 
		0xd0a0, 
		0xc200, 
		0xb54d, 
		0x4cc0, 
		0x5040, 
		0x4400, 
		0x4d00, 
		0x0000, 
		0xd140, 
		0x52c0, 
		0xd320, 
		0xade9, 
		0x51c0, 
		0xb5a2, 
		0x0000, 
		0x8888, 
		0x4d40, 
		0xfc00, 
		0x53e0, 
		0xd2c0, 
		0x4c00, 
		0x04ec, 
		0x4700, 
		0xca00, 
		0x480c, 
		0xcd40, 
		0xce80, 
		0x0000, 
		0xe237, 
		0x50e0, 
		0x0870, 
		0xcc80, 
		0x8000, 
		0x7c00, 
		0x5180, 
		0x7b50, 
		0xc200, 
		0x08f2, 
		0xc700, 
		0xd340, 
		0x52e0, 
		0xc400, 
		0xe2f4, 
		0xb91d, 
		0x8000, 
		0x0000, 
		0xf087, 
		0x8569, 
		0xca80, 
		0xafda, 
		0xce40, 
		0x5260, 
		0xd360, 
		0xcc00, 
		0xd0e0, 
		0x5220, 
		0xd120, 
		0x5220, 
		0x1fc8, 
		0xd2c0, 
		0xd380, 
		0x51c0, 
		0x7c00, 
		0xd3e0, 
		0x60be, 
		0x4600, 
		0xc980, 
		0x870e, 
		0x89d8, 
		0x3b2a, 
		0xcf40, 
		0xcc00, 
		0x53e0, 
		0x1d13, 
		0xd1e0, 
		0x61de, 
		0xd0a0, 
		0x4900, 
		0xb9b0, 
		0x0000, 
		0x7c00, 
		0x53e0, 
		0x8000, 
		0xfa00, 
		0xfc23, 
		0x7d8a, 
		0x51c0, 
		0xd080, 
		0xd220, 
		0xd080, 
		0x2863, 
		0x2720, 
		0xc880, 
		0x5240, 
		0xca00, 
		0x1f70, 
		0x52e0, 
		0x4f00, 
		0xd160, 
		0xcf40, 
		0xd1c0, 
		0x4b80, 
		0xd3e0, 
		0x8000, 
		0xcdc0, 
		0xad89, 
		0xd320, 
		0xbc78, 
		0x5180, 
		0x9efe, 
		0x4fc0, 
		0x5000, 
		0x50c0, 
		0xf6db, 
		0x5000, 
		0x7c00, 
		0x7c71, 
		0x8346, 
		0xd220, 
		0x8000, 
		0x2ed5, 
		0x5080, 
		0x3477, 
		0x53ec, 
		0x5200, 
		0xbc00, 
		0x5200, 
		0x96cb, 
		0x51c0, 
		0xf41c, 
		0xf76b, 
		0x7c66, 
		0x5380, 
		0x4900, 
		0xb8e4, 
		0x0000, 
		0xb4ca, 
		0x4700, 
		0x4980, 
		0x5260, 
		0xd060, 
		0x4ec0, 
		0x8c81, 
		0xcdc0, 
		0x37b2, 
		0xd340, 
		0x4b00, 
		0xd360, 
		0x6e5f, 
		0x4f80, 
		0xd080, 
		0x4e00, 
		0x7c00, 
		0xbd8c, 
		0xafc6, 
		0x8b76, 
		0xce40, 
		0xb2d6, 
		0x4800, 
		0x5220, 
		0xcc00, 
		0x4880, 
		0xc700, 
		0x5320, 
		0x6525, 
		0x125f, 
		0x129e, 
		0xd9bd, 
		0x4dc0, 
		0x4c40, 
		0x4c80, 
		0x4f40, 
		0xfc00, 
		0xd180, 
		0x4400, 
		0xd160, 
		0xac3a, 
		0x52a0, 
		0x114c, 
		0xa774, 
		0x4932, 
		0xc200, 
		0xd220, 
		0x0000, 
		0x4a00, 
		0xd180, 
		0x8000, 
		0x4cc0, 
		0x5140, 
		0xbc00, 
		0xd084, 
		0xcd80, 
		0x9650, 
		0xd0e0, 
		0xb183, 
		0x2503, 
		0x4c80, 
		0x3673, 
		0x4200, 
		0x8000, 
		0x9b33, 
		0xc400, 
		0xa61e, 
		0x0000, 
		0x0000, 
		0xd300, 
		0x53a0, 
		0xd1c0, 
		0x0000, 
		0x4fc0, 
		0x51c0, 
		0xd220, 
		0x3779, 
		0x8004, 
		0xd360, 
		0xd1a0, 
		0xc600, 
		0xc047, 
		0xccc0, 
		0xd1e0, 
		0xd380, 
		0x7c00, 
		0x53e0, 
		0x1d4f, 
		0x5000, 
		0xd180, 
		0x8000, 
		0x9988, 
		0xc700, 
		0x5200, 
		0x7c00, 
		0x4000, 
		0x0000, 
		0xe5f3, 
		0xc980, 
		0xb6a4, 
		0xd2c0, 
		0x4000, 
		0x51c0, 
		0x5065, 
		0xd260, 
		0x4c00, 
		0xcc00, 
		0xbc00, 
		0xcf40, 
		0xd120, 
		0x0000, 
		0x16f1, 
		0x8246, 
		0xd0a0, 
		0x5c7e, 
		0x5120, 
		0xcd40, 
		0xd240, 
		0x8307, 
		0x5080, 
		0x5060, 
		0xf206, 
		0x746f, 
		0xc000, 
		0xc20a, 
		0xfc71, 
		0x0000, 
		0x4f80, 
		0x5260, 
		0x4c00, 
		0x52a0, 
		0xcf00, 
		0x53a0, 
		0xd230, 
		0xc500, 
		0xd180, 
		0xcb80, 
		0x5000, 
		0x8714, 
		0xcf40, 
		0xd1a0, 
		0x4185, 
		0x7f8a, 
		0x01ac, 
		0xa031, 
		0x53e0, 
		0xcd80, 
		0x8000, 
		0x8a09, 
		0x5300, 
		0x5120, 
		0xdfc5, 
		0x7c00, 
		0x8000, 
		0x5142, 
		0x5200, 
		0xc700, 
		0x2c68, 
		0x4000, 
		0x4800, 
		0xd0c0, 
		0x4400, 
		0x524f, 
		0x23d3, 
		0x4cc0, 
		0x50a0, 
		0x50a0, 
		0x4c40, 
		0xe7b6, 
		0x5300, 
		0x06a5, 
		0x4702, 
		0x5300, 
		0xc880, 
		0x4b27, 
		0x4a00, 
		0xd280, 
		0xfd6a, 
		0xcc00, 
		0xd0e2, 
		0x08ce, 
		0x4d00, 
		0xce00, 
		0xbc00, 
		0xcd80, 
		0x4f00, 
		0xd180, 
		0x65ed, 
		0xc900, 
		0x4480, 
		0xd3a0, 
		0x8000, 
		0xccc0, 
		0xa0f3, 
		0x4c40, 
		0xd200, 
		0x0000, 
		0x4e80, 
		0x752d, 
		0x7153, 
		0x50c0, 
		0x5000, 
		0xcd40, 
		0x52a0, 
		0x4b00, 
		0xcf80, 
		0x4d80, 
		0x52c0, 
		0x5100, 
		0xd000, 
		0xce80, 
		0xd240, 
		0x7c00, 
		0xca00, 
		0xda55, 
		0xa3bf, 
		0xd280, 
		0xd360, 
		0x877b, 
		0xca80, 
		0x4618, 
		0xc500, 
		0xd0c0, 
		0xd140, 
		0xf9e2, 
		0x5320, 
		0xd1c0, 
		0x4880, 
		0x5140, 
		0xd0a0, 
		0x97de, 
		0x53a0, 
		0x4ed8, 
		0x4c80, 
		0x5020, 
		0x3c00, 
		0x50a0, 
		0xd120, 
		0xfc00, 
		0x4e40, 
		0x4b00, 
		0xcb00, 
		0x8ad2, 
		0xd320, 
		0x4200, 
		0x5300, 
		0x4a00, 
		0xdb44, 
		0xc880, 
		0x5504, 
		0x73f5, 
		0x1b41, 
		0xce80, 
		0xd340, 
		0x27bd, 
		0x5280  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc25c0000, 
		0xc0000000, 
		0x80000002, 
		0x0025ae51, 
		0x0c400000, 
		0x41700000, 
		0xbf800000, 
		0x00000000, 
		0xc2380000, 
		0xc2600000, 
		0x0e0000ff, 
		0x80800003, 
		0x7f7ffffe, 
		0x2e051424, 
		0x00000001, 
		0x00000800, 
		0x11f7fb8b, 
		0xf087e030, 
		0x00000040, 
		0x58607e1f, 
		0xc21c0000, 
		0x07b551a5, 
		0x41700000, 
		0x0c7fffff, 
		0x0d4a63d6, 
		0x41b00000, 
		0xc0e00000, 
		0x00000001, 
		0x98a8663b, 
		0x807ffffe, 
		0xff944871, 
		0x0e003fff, 
		0xd31bf8d2, 
		0x41d00000, 
		0x00000000, 
		0xe4b14266, 
		0x007fffff, 
		0xa2768234, 
		0xffffffff, 
		0x00800003, 
		0x8788be50, 
		0x7fc00001, 
		0xc2100000, 
		0xa01bf4f6, 
		0x0e003fff, 
		0x00000000, 
		0x30301ef6, 
		0x00800002, 
		0x7f800000, 
		0x80800001, 
		0x41900000, 
		0x40e00000, 
		0xf2803d6f, 
		0x0c7fff00, 
		0x0e0007ff, 
		0xff800001, 
		0x0c7fffe0, 
		0x680de57d, 
		0xc2580000, 
		0xc1c80000, 
		0xff7ffffe, 
		0xa7e3efd8, 
		0x00001000, 
		0x0c7fffff, 
		0x80000008, 
		0x42700000, 
		0x80800002, 
		0x41880000, 
		0x42380000, 
		0x0fb9f224, 
		0xeffff411, 
		0x7fdae1ff, 
		0x40c00000, 
		0x7f7ffffe, 
		0x42040000, 
		0x42500000, 
		0x156394e9, 
		0xc2580000, 
		0x2587c3c4, 
		0xd1591a15, 
		0xff800000, 
		0x0e7fffff, 
		0x80800001, 
		0xffc00000, 
		0x80000002, 
		0x80004000, 
		0x00000080, 
		0x30d3741a, 
		0x80002000, 
		0x2c59b2a7, 
		0xc2640000, 
		0x00000000, 
		0x7f800000, 
		0x80000080, 
		0x7abb5303, 
		0x80000000, 
		0x41c00000, 
		0x6cc13857, 
		0x87bdae3f, 
		0x80000200, 
		0x80000020, 
		0x7fe231e4, 
		0x7f800000, 
		0x0e00001f, 
		0x0c7fffc0, 
		0xff906b82, 
		0xa2e415ba, 
		0x42580000, 
		0x610cb35a, 
		0x80000000, 
		0x40e00000, 
		0x42100000, 
		0x0c7ffff0, 
		0x0c7fffff, 
		0xff92a5d1, 
		0x80008000, 
		0xc0000000, 
		0x00800003, 
		0x00800003, 
		0x41500000, 
		0x41c00000, 
		0xb4c6e81e, 
		0xf15bf6c1, 
		0x41a00000, 
		0xa012edc4, 
		0x0c7e0000, 
		0x92cb62b9, 
		0x2b0abad8, 
		0xc0400000, 
		0xc21c0000, 
		0xc2000000, 
		0xa37f35cc, 
		0xc1000000, 
		0xffffffff, 
		0xffc00000, 
		0x80000000, 
		0x42600000, 
		0x00322df1, 
		0x0e0007ff, 
		0x41600000, 
		0x42480000, 
		0x0e003fff, 
		0x528b1a42, 
		0x9d5df8eb, 
		0x1b0fb6d2, 
		0x80080000, 
		0x80000001, 
		0x2a205316, 
		0x8d499864, 
		0xc0c00000, 
		0xff800000, 
		0x374816a4, 
		0x80000001, 
		0x80000000, 
		0xb6fd6921, 
		0x00ffffff, 
		0xe5030f57, 
		0xc27c0000, 
		0xa8b593bf, 
		0xeadc39c0, 
		0xc1600000, 
		0x80800001, 
		0x731fe9d6, 
		0x42380000, 
		0x00000000, 
		0x80800000, 
		0x0e003fff, 
		0x420c0000, 
		0xcd276ec4, 
		0x80000000, 
		0x80800001, 
		0x00800001, 
		0x1083aaf1, 
		0xff856255, 
		0x80000080, 
		0xff000000, 
		0xb3e60ec3, 
		0x0c7c0000, 
		0x420c0000, 
		0xfcc04b95, 
		0x42180000, 
		0x40e00000, 
		0x422c0000, 
		0x41400000, 
		0x0c7ffff8, 
		0xc2440000, 
		0xc5af8bab, 
		0x0e000001, 
		0x01a4ebae, 
		0x061fcceb, 
		0x2a2dcb42, 
		0x00002000, 
		0x3687b831, 
		0x00000000, 
		0x00001000, 
		0x0e000007, 
		0x0e000fff, 
		0x3d878e41, 
		0xb6bf8c59, 
		0x3d309882, 
		0x00000004, 
		0xc2200000, 
		0x803aedcf, 
		0x74883d91, 
		0x41700000, 
		0xffffffff, 
		0x41400000, 
		0xc0400000, 
		0xc1e80000, 
		0x3f800001, 
		0x80000001, 
		0x1f853439, 
		0xc2440000, 
		0x8ba20a61, 
		0x0c7c0000, 
		0x7f000000, 
		0xfbda68d8, 
		0x00000001, 
		0x80001000, 
		0x0f7ffffd, 
		0x00000000, 
		0x00000001, 
		0x41400000, 
		0xc2000000, 
		0xc1f00000, 
		0x80000000, 
		0x41900000, 
		0x80800000, 
		0xffc00001, 
		0x0e1fffff, 
		0x9c5f90cb, 
		0x5157716e, 
		0xa82913fd, 
		0xee149450, 
		0xd896397d, 
		0x00000008, 
		0xcb000000, 
		0x00011111, 
		0xc1a00000, 
		0x00800000, 
		0xc1000000, 
		0xc2240000, 
		0x7fc00000, 
		0x423c0000, 
		0x00000001, 
		0x7f7fffff, 
		0xa724addf, 
		0x426c0000, 
		0x7e6766a6, 
		0xb6e3d962, 
		0xbf800001, 
		0x7fc00000, 
		0xffffffff, 
		0x0e07ffff, 
		0xdadc9804, 
		0xc25c0000, 
		0xc1e80000, 
		0xc2600000, 
		0x699c0242, 
		0xf394460d, 
		0xff7ffffe, 
		0x00800000, 
		0x0c7ff000, 
		0xda1745b5, 
		0x00001000, 
		0x0c6b5897, 
		0xff800000, 
		0x7fc00001, 
		0xc2580000, 
		0x7faa6cd7, 
		0x8380cc61, 
		0x0c7ffc00, 
		0x0c7fff00, 
		0xc2400000, 
		0x8f7ffffc, 
		0xc2500000, 
		0xffc00001, 
		0xc2640000, 
		0x0e00ffff, 
		0x7f800001, 
		0x6ac8cec4, 
		0x41b80000, 
		0x41a00000, 
		0x42240000, 
		0x80200000, 
		0x41300000, 
		0x8004e41b, 
		0x80800000, 
		0x9bfceae8, 
		0x0f7ffffd, 
		0x42180000, 
		0x4b8c4b40, 
		0x00800000, 
		0x80000000, 
		0x0e00007f, 
		0xacfe529d, 
		0x2d58c83e, 
		0x41400000, 
		0x94931df7, 
		0xda83d409, 
		0x42240000, 
		0x42440000, 
		0x7f800000, 
		0xff800000, 
		0x7f000000, 
		0x16d53666, 
		0xc1e00000, 
		0x0e03ffff, 
		0x1a533420, 
		0xa31ab86a, 
		0x41f80000, 
		0xff800000, 
		0x7c830d71, 
		0x5600cc7f, 
		0xcedb7f3d, 
		0x41300000, 
		0xfc3150c1, 
		0x10be7106, 
		0xc1700000, 
		0x0e1fffff, 
		0x0c7fff80, 
		0x41880000, 
		0x595babcc, 
		0x0e000007, 
		0x0e001fff, 
		0xf57adc1a, 
		0x00000000, 
		0x6e9ab54b, 
		0xaaaaaaaa, 
		0xff000000, 
		0xffbfffff, 
		0xe25251a9, 
		0x8f7ffffd, 
		0xc2540000, 
		0x0d000ff0, 
		0xc2300000, 
		0xc940d377, 
		0xffbfffff, 
		0x5723ab1c, 
		0x80000100, 
		0x01c9aee5, 
		0x7f8226bd, 
		0xc2580000, 
		0x80000000, 
		0x0e00007f, 
		0x0c7fff00, 
		0x0f832dbe, 
		0x00011111, 
		0x1d66d406, 
		0x4926c92e, 
		0xc81d702b, 
		0xc2000000, 
		0x0e0001ff, 
		0x1c9e121e, 
		0x0e00ffff, 
		0xc1b80000, 
		0x173401af, 
		0x80800001, 
		0xd4cf36ad, 
		0xc21c0000, 
		0x0c7e0000, 
		0xd241279f, 
		0xc25c0000, 
		0x7649270d, 
		0x8f7ffffe, 
		0x42140000, 
		0x4b8c4b40, 
		0xc2740000, 
		0x41300000, 
		0x41880000, 
		0x97dce362, 
		0xc2100000, 
		0xc2040000, 
		0xc0e00000, 
		0xc0c00000, 
		0xadc7f0ba, 
		0xc1e00000, 
		0x427c0000, 
		0x7f800001, 
		0x41d80000, 
		0x42100000, 
		0x7fab45ea, 
		0xc1d80000, 
		0x807fffff, 
		0xc2680000, 
		0xc1914c7c, 
		0x80100000, 
		0xaaaaaaaa, 
		0x0c7ffe00, 
		0x80000000, 
		0xc21c0000, 
		0xf6c84a32, 
		0x80000400, 
		0x7f83a11c, 
		0x8f7ffffd, 
		0x41b80000, 
		0xf9d241da, 
		0x00040000, 
		0x80008000, 
		0xb00bf0fd, 
		0x00000000, 
		0xaf4071f8, 
		0x42400000, 
		0xee690fe0, 
		0x0e003fff, 
		0x3ac52975, 
		0x7ebf3a52, 
		0x42280000, 
		0x007ffffe, 
		0x00000080, 
		0x00000001, 
		0x0e00000f, 
		0xc1300000, 
		0x40e00000, 
		0x807ffffe, 
		0x93859a46, 
		0xff000000, 
		0xc7d91f26, 
		0x8f7ffffc, 
		0x00000000, 
		0xc1300000, 
		0x42200000, 
		0xd5dbf065, 
		0x80000080, 
		0xc1500000, 
		0xbc398a21, 
		0x42780000, 
		0x0c7ffe00, 
		0x42200000, 
		0x00200000, 
		0xff7fffff, 
		0xc20c0000, 
		0x4ca73792, 
		0x00800002, 
		0xc1500000, 
		0xc2040000, 
		0x00800001, 
		0x30370363, 
		0x42740000, 
		0x26e1aa6a, 
		0x41a80000, 
		0x80010000, 
		0x0e07ffff, 
		0x00000000, 
		0x269028bd, 
		0xffb9d4bf, 
		0xc2740000, 
		0xd5bfe459, 
		0x41800000, 
		0x0c7fff80, 
		0xc1f00000, 
		0xc2400000, 
		0x4a3ab03a, 
		0x19399ec9, 
		0x9cfa50da, 
		0x0f7ffffc, 
		0xc2200000, 
		0xff800000, 
		0xce21f6af, 
		0x422c0000, 
		0x00000000, 
		0x36cef726, 
		0x42480000, 
		0x00800000, 
		0xc1300000, 
		0xff800000, 
		0x0c700000, 
		0x80011111, 
		0x00000000, 
		0x41500000, 
		0x42300000, 
		0xfffe18d5, 
		0x7f000000, 
		0x0d000ff0, 
		0xc2640000, 
		0x421c0000, 
		0x80000040, 
		0x002076ad, 
		0xbf3f875d, 
		0x80000000, 
		0x80000002, 
		0x40400000, 
		0xa5b88950, 
		0x0c7fc000, 
		0x00000400, 
		0x91d8dc52, 
		0x0c7ffffc, 
		0x576d13dd, 
		0x80000001, 
		0x0e03ffff, 
		0x0c7fffff, 
		0xc1880000, 
		0xc0000000, 
		0xc2480000, 
		0x42200000, 
		0x23d70ebf, 
		0x1885129d, 
		0x80800001, 
		0xff000000, 
		0x40a00000, 
		0x8f7ffffc, 
		0xc1c00000, 
		0x8f7ffffc, 
		0x9e8ce68f, 
		0xc24c0000, 
		0xc63cc49d, 
		0x67991e36, 
		0x7f800000, 
		0xfd1fed7e, 
		0xffb61431, 
		0xc20c0000, 
		0xc1a00000, 
		0x0c7ffffe, 
		0x007fffff, 
		0x80000000, 
		0x42080000, 
		0x40400000, 
		0x25ac080e, 
		0x0e1fffff, 
		0x80000020, 
		0x0c780000, 
		0xff800000, 
		0x00001000, 
		0x7fb7ac23, 
		0x7f800001, 
		0xbbbab605, 
		0x80002000, 
		0x354a2b51, 
		0xcb000000, 
		0xbf800000, 
		0x00000000, 
		0x00800000, 
		0xc2300000, 
		0xab29f908, 
		0x19c2e559, 
		0x55c1f5f3, 
		0x0e000007, 
		0x0e00001f, 
		0x7f800000, 
		0x42440000, 
		0x80000000, 
		0x443c565f, 
		0xc2180000, 
		0xfa7a01e1, 
		0xe5bc07d6, 
		0x40e00000, 
		0x4b8867db, 
		0x0c7ffff0, 
		0xc2080000, 
		0x80800001, 
		0xe64c8c9c, 
		0x42680000, 
		0x40e00000, 
		0x7fc00001, 
		0xff800000, 
		0xc1d80000, 
		0x3d77c883, 
		0xff800000, 
		0x00800001, 
		0x1f123239, 
		0xffbfffff, 
		0x41880000, 
		0x0e007fff, 
		0x00000004, 
		0x41500000, 
		0x80000000, 
		0xb32f7581, 
		0xc2200000, 
		0x42740000, 
		0x42240000, 
		0x0c464dad, 
		0x0c7fc000, 
		0x2bc44a27, 
		0x00000800, 
		0xa3be1d91, 
		0xb4e27365, 
		0x8a73c596, 
		0x2ce1326e, 
		0xcb8c4b40, 
		0xc2600000, 
		0x0e00ffff, 
		0x7258e12a, 
		0x42180000, 
		0x00004000, 
		0xbaac69de, 
		0x00000000, 
		0xcb000000, 
		0x0c7ffffc, 
		0xc24c0000, 
		0x4ed3980c, 
		0x00080000, 
		0x41f00000, 
		0xc2640000, 
		0x0e00003f, 
		0xff800000, 
		0xc0400000, 
		0xff7ffffe, 
		0x80000000, 
		0xff996199, 
		0x00040000, 
		0x5c620012, 
		0x78e45b42, 
		0x926208d6, 
		0xc2040000, 
		0xabb0ee2d, 
		0x00000020, 
		0x0e1fffff, 
		0xe0b71fc9, 
		0xff800000, 
		0x7f800000, 
		0x42740000, 
		0xc2200000, 
		0x7f7ffffe, 
		0x764c777d, 
		0x676cc597, 
		0x41980000, 
		0x41300000, 
		0xc2340000, 
		0xc1c00000, 
		0x0c7ffc00, 
		0x80000000, 
		0xc1f80000, 
		0x8800fa34, 
		0x00800003, 
		0x80000001, 
		0x42540000, 
		0x42780000, 
		0x00000000, 
		0x807fffff, 
		0x41a80000, 
		0x42400000, 
		0x41700000, 
		0x566c34a6, 
		0x41100000, 
		0x709f5c08, 
		0xff800000, 
		0x00800002, 
		0xc1900000, 
		0x633c00a1, 
		0x002f3c6a, 
		0x0f7fffff, 
		0x0c7e0000, 
		0x00000002, 
		0x80000000, 
		0x0856dcde, 
		0x80100000, 
		0xff93302d, 
		0x49cb5b10, 
		0x964d532b, 
		0x80000200, 
		0x422c0000, 
		0xb7f7ffb5, 
		0x80000000, 
		0xc2040000, 
		0x41000000, 
		0xff800000, 
		0xc1d5a244, 
		0xc2280000, 
		0x0e00001f, 
		0x7f800000, 
		0x7f7ffffe, 
		0x42780000, 
		0x7fc00000, 
		0xc0800000, 
		0xffffffff, 
		0x33333333, 
		0x80001000, 
		0x80800002, 
		0x42500000, 
		0x422c0000, 
		0x7f000000, 
		0xc2280000, 
		0xc1d80000, 
		0x8f7ffffe, 
		0x80000001, 
		0x80000000, 
		0x80800000, 
		0x00000004, 
		0x00000002, 
		0xc1000000, 
		0xd1d23bb9, 
		0xff7ffffe, 
		0x00800001, 
		0x00000002, 
		0x7fbfffff, 
		0x00000000, 
		0xb0b3eee3, 
		0xba4da72e, 
		0x00000000, 
		0x42180000, 
		0x80040000, 
		0xfeb08cfc, 
		0x00000000, 
		0xe332062b, 
		0x40800000, 
		0x41400000, 
		0x00000000, 
		0x10a5fa13, 
		0xad37b454, 
		0x006f7977, 
		0x42480000, 
		0x7ba0ce7e, 
		0xac878679, 
		0x3c70ce55, 
		0x80000000, 
		0x28070e55, 
		0x00ffffff, 
		0x33ab2e2b, 
		0x80040000, 
		0x00000080, 
		0x77f09859, 
		0x807fffff, 
		0xc0e00000, 
		0xfe455bfc, 
		0x00040000, 
		0x42600000, 
		0xbf028f5c, 
		0xc1000000, 
		0x80000000, 
		0x0c7fff00, 
		0xc601b98c, 
		0x141e6c62, 
		0x0e00ffff, 
		0x00100000, 
		0x00000001, 
		0xc1300000, 
		0xa689ee4b, 
		0x28d9b654, 
		0x80000000, 
		0x42680000, 
		0x7f800000, 
		0xf050157a, 
		0x0c7ffff0, 
		0x00000000, 
		0x0e0001ff, 
		0x00800001, 
		0x6726e976, 
		0xff800000, 
		0x00011111, 
		0x3ea8942f, 
		0xff7ffffe, 
		0x0c7e0000, 
		0xeb9c8061, 
		0x42280000, 
		0x007ffffe, 
		0x00000000, 
		0x42780000, 
		0x7fffffff, 
		0x55555555, 
		0x80000000, 
		0x784a8cc1, 
		0x0d000ff0, 
		0x0c7e0000, 
		0x41a00000, 
		0x0e00ffff, 
		0xef3209e4, 
		0x23d0863d, 
		0xff7ffffe, 
		0x0c7e0000, 
		0xffb8454c, 
		0xff815892, 
		0x80200000, 
		0x00000000, 
		0x80000800, 
		0xc0a00000, 
		0x42700000, 
		0x0c7fffff, 
		0x41d80000, 
		0x00000400, 
		0xd818a5e2, 
		0xeaa5738e, 
		0xc2600000, 
		0xc2600000, 
		0x02d56c47, 
		0xf9122ff7, 
		0x80800000, 
		0x0c7fffc0, 
		0x41e00000, 
		0x29a16ff9, 
		0xf8e0533d, 
		0xe913edfc, 
		0x0e001fff, 
		0xc1800000, 
		0x0c7f8000, 
		0xc2540000, 
		0xc0c00000, 
		0x1b81e5f8, 
		0x0c7ff000, 
		0x6bde4bda, 
		0xc2100000, 
		0x80800002, 
		0xf24e6363, 
		0x42600000, 
		0x17bed418, 
		0xc1900000, 
		0x7f800000, 
		0x7fc00000, 
		0x7f7ffffe, 
		0x80080000, 
		0x8d147139, 
		0x00200000, 
		0x421c0000, 
		0x80000001, 
		0x0e0001ff, 
		0x80000080, 
		0x00000200, 
		0x00200000, 
		0x8b6ac5d8, 
		0x475c44cd, 
		0x3a22d8ee, 
		0x42280000, 
		0xfff06471, 
		0x00000000, 
		0x0e000fff, 
		0xc1980000, 
		0x80ffffff, 
		0xcb0429c8, 
		0xc1e80000, 
		0x4b8c4b40, 
		0x7f800000, 
		0x7f7fffff, 
		0x7f800001, 
		0x0c7c0000, 
		0xc1f00000, 
		0x886ff245, 
		0x41d80000, 
		0xc2780000, 
		0xc2780000, 
		0xc1980000, 
		0xcb8c4b40, 
		0xe2ca991b, 
		0x00800003, 
		0x42280000, 
		0x42740000, 
		0xcccccccc, 
		0x42700000, 
		0xc2780000, 
		0x746e8dfd, 
		0x77495dc3, 
		0x408dbb3e, 
		0x80800000, 
		0x0c7ffc00, 
		0xc0c00000, 
		0xd50132aa, 
		0x7f800000, 
		0x67a26e77, 
		0xff800000, 
		0xec7fcc5c, 
		0x3c6b4d8e, 
		0xd5a856d1, 
		0x0f7ffffc, 
		0x41e80000, 
		0x7f800000, 
		0xc1500000, 
		0xfff45303, 
		0xc9ba8810, 
		0xc2480000, 
		0x80004000, 
		0x00000000, 
		0x00000000, 
		0xbf800001, 
		0xc1100000, 
		0x920a1c3b, 
		0xe1905d26, 
		0xc1b80000, 
		0xc1200000, 
		0x0e7fffff, 
		0xc1c80000, 
		0xc2200000, 
		0xc1700000, 
		0xff800000, 
		0x41000000, 
		0xf8a15a27, 
		0x80ffffff, 
		0x0e000fff, 
		0xffc00001, 
		0x1ac56c3f, 
		0x800dbff7, 
		0x8f7ffffd, 
		0x7f800000, 
		0x7f7ffffe, 
		0x007d535f, 
		0x0e000007, 
		0x41f00000, 
		0x40400000, 
		0xa9bd697b, 
		0xd1e86284, 
		0xc2480000, 
		0x00000100, 
		0x7f7fffff, 
		0x80080000, 
		0x00ffffff, 
		0xff7ffffe, 
		0x8a5dbc45, 
		0x07528e09, 
		0x9d71b79e, 
		0xff800000, 
		0xdcd60e45, 
		0x55555555, 
		0x42240000, 
		0xc1300000, 
		0xfd2d2884, 
		0x40000000, 
		0x00000200, 
		0x7f7ffffe, 
		0x0e0003ff, 
		0x00002000, 
		0x42040000, 
		0x00040000, 
		0x29ab4df2, 
		0x6050a403, 
		0x0c7ffc00, 
		0x7fc00000, 
		0xad02a5ec, 
		0x425c0000, 
		0x00080000, 
		0x42480000, 
		0x42740000, 
		0xff7ffffe, 
		0x4062ef65, 
		0x71dfc6bd, 
		0x7f800000, 
		0x00030c74, 
		0xc2180000, 
		0xecc38931, 
		0x80200000, 
		0x00800001, 
		0x00800001, 
		0x3a01aaa3, 
		0x00000000, 
		0x0e000001, 
		0x42240000, 
		0xc1a00000, 
		0x55555555, 
		0x7fc00000, 
		0x0e0001ff, 
		0x80000000, 
		0xc0e00000, 
		0x26c8daad, 
		0x6dfd0fbb, 
		0xcd530a00, 
		0x42200000, 
		0x2c8f81f5, 
		0x80010000, 
		0x471bebe2, 
		0xc1e80000, 
		0xf0657763, 
		0xd0f5b441, 
		0xc1000000, 
		0x42140000, 
		0x997edcbc, 
		0xc22c0000, 
		0x0e7fffff, 
		0x41b00000, 
		0xffbfffff, 
		0x42580000, 
		0xff7ffffe, 
		0x2db3ee72, 
		0xbbcbc72e, 
		0x0c7fffc0, 
		0x7f800000, 
		0x41000000, 
		0x00080000, 
		0x00800002, 
		0x00000001, 
		0x427c0000, 
		0xff7ffffe, 
		0x0e007fff, 
		0x80000000, 
		0x00010000, 
		0x211d68f1, 
		0xff788836, 
		0x0c7ffff0, 
		0x80800000, 
		0xc0a00000, 
		0x3ce4d29f, 
		0xff1a1dfe, 
		0x42580000, 
		0x80011111, 
		0x007ffffe, 
		0x687efa48, 
		0x00800001, 
		0x0e32bd83, 
		0x7096d31c, 
		0x81ccf3b3, 
		0x80000001, 
		0x80002000, 
		0x0940d4b9, 
		0x40c00000, 
		0x41700000, 
		0x42140000, 
		0x7f77d74d, 
		0x00001000, 
		0x807fffff, 
		0x7f7fffff, 
		0x00000000, 
		0xa815de10, 
		0x80800002, 
		0x50b3253a, 
		0x00000000, 
		0x5d91e61a, 
		0xc2740000, 
		0xc24c0000, 
		0x00000020, 
		0xc2680000, 
		0x80000200, 
		0xc0800000, 
		0x42240000, 
		0x9914aeb7, 
		0x00000000, 
		0xc2440000, 
		0x00000000, 
		0x41d00000, 
		0x41000000, 
		0x00000000, 
		0xc1100000, 
		0xc1f80000, 
		0x0c7ffff8, 
		0x7f948658, 
		0x80000010, 
		0xdd292494, 
		0x91f981d8, 
		0xffc00001, 
		0x80800002, 
		0x80000000, 
		0x80800001, 
		0x092bdd37, 
		0xcc2c2382, 
		0x0c400000, 
		0x0d00fff0, 
		0x0c700000, 
		0x00000080, 
		0x41880000, 
		0xb70cdb13, 
		0x0e000fff, 
		0xb9f7e13b, 
		0x59ec34f9, 
		0x27747ed4, 
		0x41500000, 
		0x7fc00000, 
		0x00000000, 
		0xc1d8c308, 
		0x42440000, 
		0x2226c99a, 
		0x80080000, 
		0x9edbbe4b, 
		0x4b000000, 
		0x40000000, 
		0xdbf0ac12, 
		0x00000000, 
		0x00000080, 
		0x8e5a2d2b, 
		0x7f7fffff, 
		0x0e0003ff, 
		0x00000000, 
		0x20e38b0e, 
		0xc2080000, 
		0x7f800000, 
		0xc2480000, 
		0x0c7ffff8, 
		0x40d5b428, 
		0xf913cf15, 
		0x42440000, 
		0xa1122bab, 
		0xda85d0ac, 
		0xc2000000, 
		0x3d7656fd, 
		0x807ffffe, 
		0xc1100000, 
		0x3f800000, 
		0xc1c00000, 
		0x42480000, 
		0x00002000, 
		0xbf028f5c, 
		0xff7ffffe, 
		0x00699755, 
		0x80000001, 
		0x80000000, 
		0xff800000, 
		0x8661470a, 
		0x00000010, 
		0x00800002, 
		0xc26c0000, 
		0x00000000, 
		0xf5d172fc, 
		0xff800001, 
		0x41e00000, 
		0xb33ff415, 
		0x803db776, 
		0x4b8c4b40, 
		0x77e0ec7e, 
		0x13525e88, 
		0x25df0b31, 
		0x5c598068, 
		0x42240000, 
		0x80000000, 
		0x338613de, 
		0x8016041d, 
		0xbf800001, 
		0xcb8c4b40, 
		0xb789db51, 
		0xa5c29cc9, 
		0x00000000, 
		0x8f7ffffd, 
		0x0e000007, 
		0x3ac9e187, 
		0x0e9bbed3, 
		0x7f800000, 
		0x92efa6e2, 
		0x0e3fffff, 
		0xc1e00000, 
		0x00000800, 
		0x80000000, 
		0xc2380000, 
		0x80001000, 
		0x41980000, 
		0x0c7ffff8, 
		0x00011111, 
		0x41b00000, 
		0x80000040, 
		0x3da924ac, 
		0x821889f0, 
		0xb438def7, 
		0xc1a00000, 
		0xa3c6cf23, 
		0x00000080, 
		0xc1200000, 
		0xb930d1ce, 
		0xffffffff, 
		0xc2480000, 
		0x42040000, 
		0x0bbc1a20, 
		0xffa1ed39, 
		0x42280000, 
		0xc0c00000, 
		0xc20c0000, 
		0x80000000, 
		0x1f480bcb, 
		0x0c600000, 
		0x0c7fffff, 
		0xbad0847c, 
		0xffa2a1c1, 
		0x0c700000, 
		0xbf800000, 
		0xffbfffff, 
		0x00040000, 
		0x00000000, 
		0x00000002, 
		0x00000000, 
		0xff000000, 
		0x7f7ffffe, 
		0x0c7ffff8, 
		0xcae9a5e7, 
		0x80800001, 
		0x36a79917, 
		0xc1c00000, 
		0x42600000, 
		0x563daef5, 
		0x302b2abc, 
		0x756707b5, 
		0x531ff5bb, 
		0xc3d129b2, 
		0x41600000, 
		0x0c7ffff8, 
		0xc1f00000, 
		0x42140000, 
		0x14d0d0b3, 
		0x2b7574b6, 
		0x41b00000, 
		0x80000001, 
		0x437eea26, 
		0xc2000000, 
		0x00020000, 
		0x80000000, 
		0x0e00001f, 
		0xffbfde3f, 
		0x41200000, 
		0x7f800000, 
		0xffa42e8e, 
		0x42000000, 
		0xeebef09e, 
		0x91861edb, 
		0x0d00fff0, 
		0x41c80000, 
		0x80000000, 
		0x8e7beca7, 
		0xc0c00000, 
		0x701c8169, 
		0x4b000000, 
		0xbf028f5c, 
		0x80080000, 
		0x00800000, 
		0x00000000, 
		0x80000000, 
		0xc1500000, 
		0x422c0000, 
		0xb1c0e8ab, 
		0x7fc00000, 
		0x7f800000, 
		0x42680000, 
		0x00011111, 
		0xa66eefc8, 
		0x00800003, 
		0x7fc00000, 
		0x00800002, 
		0xc1a80000, 
		0xbf028f5c, 
		0x86139428, 
		0xbf800001, 
		0x0064804d, 
		0x9501d777, 
		0x00100000, 
		0x42100000, 
		0x00001000, 
		0x41a80000, 
		0x00000000, 
		0x00000004, 
		0x278dbd6a, 
		0x9228d49b, 
		0x800a54d3, 
		0xc2640000, 
		0x42640000, 
		0xc17fc852, 
		0xcb3a803e, 
		0x548651ff, 
		0x80011111, 
		0x42780000, 
		0x40a00000, 
		0x00020000, 
		0xe7186288, 
		0x3f800001, 
		0xf246f023, 
		0xbce8e976, 
		0x41b80000, 
		0x9fb49783, 
		0x80020000, 
		0xffb9cdca, 
		0x7f800000, 
		0x0f7ffffc, 
		0xff7ffffe, 
		0x41b80000, 
		0xc2600000, 
		0x42680000, 
		0xc2100000, 
		0x41d80000, 
		0x41c00000, 
		0x42680000, 
		0x42300000, 
		0x34722379, 
		0x0c780000, 
		0x5243db0c, 
		0x00178183, 
		0x80200000, 
		0xfab30d48, 
		0xb746ba9f, 
		0x80800003, 
		0x00000000, 
		0x42700000, 
		0x42040000, 
		0x0c7ffff8, 
		0x78afbae5, 
		0x80000200, 
		0x0e00003f, 
		0x80000004, 
		0xc0400000, 
		0xe0b4c9ff, 
		0xc1200000, 
		0x00800001, 
		0x42200000, 
		0x41f00000, 
		0xe78bfc17, 
		0x0e000003, 
		0xebd7b1fa, 
		0x00004000, 
		0x80000000, 
		0xff910808, 
		0x00000008, 
		0xcacac290, 
		0x41b00000, 
		0x00000000, 
		0x7fae29bb, 
		0x80000001, 
		0x42080000, 
		0x93a716b9, 
		0x0f7ffffc, 
		0xe19c1f0a, 
		0xfc78f7fd, 
		0x80000000, 
		0xc1880000, 
		0x7fb05d0b, 
		0x6713d1c7, 
		0xbe333523, 
		0x75a358d7, 
		0xbf028f5c, 
		0x424c0000, 
		0x0e00001f, 
		0x80000000, 
		0x00010000, 
		0x42380000, 
		0xc1980000, 
		0xae224993, 
		0x7f800000, 
		0x40800000, 
		0x00000000, 
		0x0c700000, 
		0xff7fffff, 
		0x325f4f81, 
		0x0c7f8000, 
		0x7d0820cc, 
		0xff800000, 
		0xff800000, 
		0x737b6e5f, 
		0x419e84c4, 
		0xff8e5d52, 
		0xff800000, 
		0xff800000, 
		0x3707f44a, 
		0xff000000, 
		0x00800000, 
		0xc0a00000, 
		0x0c7fffe0, 
		0x0e000001, 
		0x0c7ffe00, 
		0xc2500000, 
		0x42380000, 
		0x80001000, 
		0xf028977b, 
		0x421c0000, 
		0x0c7fffc0, 
		0x40400000, 
		0xee828329, 
		0x339ab709, 
		0x00002000, 
		0x0073bc66, 
		0x85d00906, 
		0x80000000, 
		0x680aafed, 
		0x0e00001f, 
		0xff800000, 
		0x80000000, 
		0x0e6a0daa, 
		0xffc00000, 
		0xba5105f5, 
		0x232b9591, 
		0xf222f72f, 
		0xbf800001, 
		0xc0e00000, 
		0x7fb84cec, 
		0xf18525a9, 
		0xff800000, 
		0x42400000, 
		0xc1c80000, 
		0x00800002, 
		0xe771368e, 
		0x00000001, 
		0x694dd738, 
		0x0e00001f, 
		0x00000100, 
		0xc2280000, 
		0xc2180000, 
		0x42300000, 
		0x0f7fffff, 
		0x5edb326c, 
		0x42700000, 
		0x80000000, 
		0x41a80000, 
		0x80800000, 
		0x0e07ffff, 
		0xc2100000, 
		0x80000020, 
		0x41a00000, 
		0x6423408e, 
		0x42580000, 
		0x666a5ad5, 
		0x00000800, 
		0xdad3d8fa, 
		0x00000000, 
		0x00000000, 
		0x7f800001, 
		0xc2740000, 
		0x0e00ffff, 
		0x80000010, 
		0x7f800001, 
		0x00100000, 
		0x7fc00001, 
		0x80002000, 
		0xc1500000, 
		0x00400000, 
		0x41980000, 
		0xc2440000, 
		0x80000004, 
		0xc1200000, 
		0x80000000, 
		0x00800001, 
		0xff8ecf4e, 
		0xc1300000, 
		0x0c7ffffc, 
		0x40e00000, 
		0xd1698017, 
		0x96e6bf17, 
		0x00000020, 
		0x00011111, 
		0x8028d4a2, 
		0x41e00000, 
		0x80000001, 
		0xc2680000, 
		0x42140000, 
		0xc2100000, 
		0x00000001, 
		0x0c7fff00, 
		0x41e80000, 
		0xaaaaaaaa, 
		0x7f800000, 
		0x80000000, 
		0xbcdb2af0, 
		0x00000020, 
		0xf31c06cc, 
		0x1a0f213e, 
		0x00000000, 
		0xcb000000, 
		0x00000200, 
		0x0e007fff, 
		0x125d9cc3, 
		0x402f43c0, 
		0x7f7fffff, 
		0x007ffffe, 
		0x80000800, 
		0x42080000, 
		0xff800000, 
		0x0e007fff, 
		0x80800000, 
		0xd045e9ed, 
		0xc6f37298, 
		0xf6e80b4d, 
		0x80ffffff, 
		0x30a83304, 
		0x51785f11, 
		0x42280000, 
		0x00000000, 
		0xcc6dabc1, 
		0x0e00003f, 
		0x2388b8b3, 
		0x80000000, 
		0xc24c0000, 
		0x00800001, 
		0xc1a00000, 
		0x5ba77b75, 
		0x42180000, 
		0x80200000, 
		0x0c600000, 
		0x0c7ff000, 
		0x8f7ffffc, 
		0x4ad47c36, 
		0xc1600000, 
		0x7a14f08d, 
		0x00400000, 
		0x00505321, 
		0x689d463e, 
		0x00040000, 
		0x80011111, 
		0x3cfedba3, 
		0xc2480000, 
		0x0e00000f, 
		0x42700000, 
		0xff800000, 
		0xc0400000, 
		0x7e26275f, 
		0x0e003fff, 
		0x0c7ffe00, 
		0x803da80d, 
		0x80000200, 
		0xcccccccc, 
		0xc26c0000, 
		0x0e007fff, 
		0xc1880000, 
		0x0b4c8251, 
		0x42240000, 
		0xff800000, 
		0xc2680000, 
		0x80800001, 
		0xff3161df, 
		0xb671c9ae, 
		0x463ba97e, 
		0xcb8c4b40, 
		0xe7364428, 
		0xc2440000, 
		0x4b8c4b40, 
		0x7d3b681d, 
		0x0e0001ff, 
		0x00800002, 
		0x7f800000, 
		0x4b655005, 
		0x7fbfffff, 
		0x41880000, 
		0x3880eb91, 
		0x0e0000ff, 
		0xc0a00000, 
		0xc2680000, 
		0xf8b31eb4, 
		0x80000200, 
		0xff7fffff, 
		0x41b80000, 
		0x00000080, 
		0xf1bd16e8, 
		0xc2580000, 
		0x3f800000, 
		0x42080000, 
		0x0f7ffffe, 
		0x00040000, 
		0xd90d678f, 
		0x805e4fc1, 
		0x88b42664, 
		0x4c204942, 
		0x701826da, 
		0x4b8c4b40, 
		0x230fbdcd, 
		0x421c0000, 
		0xed6f5603, 
		0x8042f4b6, 
		0x0c7fffe0, 
		0x41c80000, 
		0x00800000, 
		0xff7fffff, 
		0x14c2d42e, 
		0xc1a00000, 
		0x24758911, 
		0x007ffffe, 
		0x41100000, 
		0x42500000, 
		0x40400000, 
		0x00800001, 
		0x7f800000, 
		0x00000000, 
		0x42540000, 
		0xc2300000, 
		0x0e07ffff  
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
		0x000003c5, 
		0x000003e5, 
		0x0000034f, 
		0x000001a7, 
		0x000002ed, 
		0x0000030d, 
		0x00000189, 
		0x0000025d, 
		0x00000253, 
		0x000001db, 
		0x0000028b, 
		0x00000319, 
		0x0000035f, 
		0x000001f5, 
		0x000003db, 
		0x000002c1, 
		0x00000329, 
		0x000003f1, 
		0x0000008d, 
		0x00000385, 
		0x000000e1, 
		0x00000057, 
		0x000000dd, 
		0x000002a5, 
		0x00000225, 
		0x0000025b, 
		0x00000197, 
		0x000003df, 
		0x00000241, 
		0x0000030d, 
		0x000002b9, 
		0x000003f7, 
		0x0000036f, 
		0x00000079, 
		0x00000391, 
		0x00000117, 
		0x00000381, 
		0x00000003, 
		0x00000091, 
		0x00000077, 
		0x00000021, 
		0x00000387, 
		0x00000085, 
		0x000003d9, 
		0x00000037, 
		0x00000253, 
		0x00000179, 
		0x0000013f, 
		0x0000014f, 
		0x000000f7, 
		0x00000017, 
		0x0000033b, 
		0x00000241, 
		0x000003b9, 
		0x00000313, 
		0x00000245, 
		0x0000024f, 
		0x0000030b, 
		0x0000032b, 
		0x000002b5, 
		0x00000213, 
		0x000001a3, 
		0x000001d9, 
		0x0000032f, 
		0x0000007f, 
		0x000000e9, 
		0x00000339, 
		0x00000199, 
		0x0000037d, 
		0x0000029f, 
		0x00000035, 
		0x0000030b, 
		0x000001d7, 
		0x000001f5, 
		0x000002af, 
		0x000002cd, 
		0x00000187, 
		0x000003e7, 
		0x000000b1, 
		0x000000f3, 
		0x0000021f, 
		0x000003a9, 
		0x00000323, 
		0x000001af, 
		0x00000311, 
		0x000001b3, 
		0x00000161, 
		0x00000067, 
		0x0000011d, 
		0x00000287, 
		0x0000031f, 
		0x000003f5, 
		0x00000313, 
		0x000000f3, 
		0x000000ef, 
		0x000002c1, 
		0x00000301, 
		0x00000135, 
		0x000003bd, 
		0x0000036b, 
		0x00000223, 
		0x00000223, 
		0x0000016d, 
		0x000002b9, 
		0x000002c1, 
		0x000002d3, 
		0x000002f7, 
		0x00000193, 
		0x000001c7, 
		0x00000293, 
		0x00000321, 
		0x000002ad, 
		0x000003cd, 
		0x0000008f, 
		0x000000c1, 
		0x00000111, 
		0x00000061, 
		0x000002e5, 
		0x000001a9, 
		0x000000d5, 
		0x00000065, 
		0x0000001f, 
		0x000001f7, 
		0x00000103, 
		0x000001fb, 
		0x000001cb, 
		0x00000001, 
		0x00000325, 
		0x000000ff, 
		0x00000399, 
		0x00000375, 
		0x00000075, 
		0x0000024f, 
		0x0000011d, 
		0x00000273, 
		0x0000010d, 
		0x000003b9, 
		0x000002a1, 
		0x00000315, 
		0x0000029f, 
		0x00000321, 
		0x00000213, 
		0x00000335, 
		0x000001c9, 
		0x000001ab, 
		0x00000145, 
		0x000003a3, 
		0x000003c5, 
		0x0000030d, 
		0x00000187, 
		0x000001af, 
		0x000000ff, 
		0x000001b9, 
		0x00000173, 
		0x0000008d, 
		0x000002cf, 
		0x0000021d, 
		0x000002ab, 
		0x000001f9, 
		0x000001f5, 
		0x000000e3, 
		0x0000035f, 
		0x00000259, 
		0x000001a7, 
		0x00000179, 
		0x000002db, 
		0x000000ad, 
		0x000001fb, 
		0x00000121, 
		0x000002e1, 
		0x00000349, 
		0x000001c3, 
		0x0000038b, 
		0x000002cb, 
		0x000003df, 
		0x00000143, 
		0x000001d1, 
		0x0000019d, 
		0x00000047, 
		0x0000000b, 
		0x00000325, 
		0x000003d9, 
		0x000001fd, 
		0x000000f7, 
		0x000003ff, 
		0x0000005d, 
		0x000003e7, 
		0x0000031f, 
		0x00000143, 
		0x00000395, 
		0x000001db, 
		0x000003b7, 
		0x0000028d, 
		0x0000020d, 
		0x000000bd, 
		0x00000251, 
		0x0000018b, 
		0x00000087, 
		0x00000357, 
		0x0000021f, 
		0x00000129, 
		0x00000131, 
		0x00000317, 
		0x00000141, 
		0x000001e1, 
		0x000002e7, 
		0x000003cf, 
		0x00000217, 
		0x00000339, 
		0x00000141, 
		0x00000209, 
		0x000002b3, 
		0x00000121, 
		0x000000d3, 
		0x000003ad, 
		0x000000cf, 
		0x000003db, 
		0x00000023, 
		0x00000259, 
		0x000001af, 
		0x00000233, 
		0x000001a7, 
		0x000001b5, 
		0x00000389, 
		0x00000369, 
		0x00000137, 
		0x000001a3, 
		0x00000081, 
		0x00000101, 
		0x0000038b, 
		0x000001eb, 
		0x000000eb, 
		0x00000345, 
		0x00000345, 
		0x0000023b, 
		0x00000239, 
		0x00000085, 
		0x000000ab, 
		0x000000f9, 
		0x00000293, 
		0x00000325, 
		0x00000027, 
		0x0000009b, 
		0x000003b5, 
		0x00000221, 
		0x000003df, 
		0x000003c9, 
		0x0000012f, 
		0x000000a7, 
		0x0000003f, 
		0x00000173, 
		0x000001eb, 
		0x00000075, 
		0x00000023, 
		0x00000377, 
		0x00000201  
};

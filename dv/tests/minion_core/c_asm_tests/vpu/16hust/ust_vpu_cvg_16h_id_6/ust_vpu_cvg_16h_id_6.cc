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
			"li x5, 190\n"
			"faddi.pi f2, f0, -132\n"
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
			"faddi.pi f2, f2, 128\n"
			"fsub.pi f23, f21, f22\n"
			"fmin.ps f28, f23, f30\n"
			"fadd.ps f22, f24, f28\n"
			"fmin.ps f23, f24, f19\n"
			"fadd.pi f26, f18, f21\n"
			"fmul.ps f22, f31, f29\n"
			"fmin.ps f21, f20, f31\n"
			"fmax.ps f19, f25, f31\n"
			"fmax.pi f25, f31, f29\n"
			"fmax.ps f30, f21, f22\n"
			"flog.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"fnmadd.ps f24, f28, f28, f20\n"
			"fnmsub.ps f27, f19, f25, f27\n"
			"fnmsub.ps f19, f24, f20, f26\n"
			"fmsub.ps f28, f27, f28, f29\n"
			"fmsub.ps f27, f28, f28, f29\n"
			"fmadd.ps f21, f28, f28, f30\n"
			"fnmsub.ps f31, f25, f23, f25\n"
			"fnmsub.ps f23, f25, f19, f31\n"
			"fnmsub.ps f20, f30, f20, f23\n"
			"fnmadd.ps f29, f27, f19, f24\n"
			"flog.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"fmax.ps f22, f21, f27\n"
			"fadd.ps f30, f20, f20\n"
			"fsub.ps f25, f22, f25\n"
			"fmax.ps f21, f20, f27\n"
			"fmul.pi f31, f29, f22\n"
			"2:\n"
			"flog.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"bne x0, x0, 2b\n"
			"fnmadd.ps f28, f26, f25, f28\n"
			"fnmadd.ps f25, f24, f27, f31\n"
			"fnmadd.ps f21, f30, f22, f30\n"
			"fnmsub.ps f30, f23, f27, f27\n"
			"fnmadd.ps f31, f28, f22, f27\n"
			"3:\n"
			"flog.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"bne x0, x0, 3b\n"
			"flog.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"flog.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"fmsub.ps f31, f27, f26, f28\n"
			"fmsub.ps f25, f24, f31, f25\n"
			"fmadd.ps f29, f29, f31, f31\n"
			"fmsub.ps f27, f26, f27, f30\n"
			"flog.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"fmsub.ps f29, f25, f26, f29\n"
			"fnmadd.ps f31, f26, f29, f27\n"
			"fnmadd.ps f27, f28, f29, f28\n"
			"fmsub.ps f28, f31, f29, f29\n"
			"fnmadd.ps f31, f31, f26, f29\n"
			"fnmadd.ps f29, f31, f30, f31\n"
			"flog.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"fnmadd.ps f26, f27, f26, f29\n"
			"fmadd.ps f27, f26, f30, f29\n"
			"fnmsub.ps f26, f27, f26, f31\n"
			"fnmsub.ps f29, f29, f28, f31\n"
			"fmadd.ps f31, f27, f30, f30\n"
			"flog.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"fand.pi f29, f30, f27\n"
			"fand.pi f30, f29, f30\n"
			"fsub.ps f30, f28, f29\n"
			"fmin.ps f28, f31, f31\n"
			"fadd.ps f31, f28, f27\n"
			"flog.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"fnmadd.ps f30, f28, f30, f31\n"
			"fnmadd.ps f29, f30, f30, f28\n"
			"fmsub.ps f30, f30, f30, f31\n"
			"fmadd.ps f28, f28, f31, f30\n"
			"fmsub.ps f31, f31, f28, f29\n"
			"fnmadd.ps f28, f30, f29, f28\n"
			"fnmsub.ps f29, f29, f29, f28\n"
			"fnmadd.ps f31, f28, f31, f31\n"
			"fmsub.ps f30, f28, f31, f29\n"
			"fmsub.ps f30, f28, f30, f29\n"
			"2:\n"
			"flog.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"bne x0, x0, 2b\n"
			"3:\n"
			"flog.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"bne x0, x0, 3b\n"
			"fnmadd.ps f30, f31, f31, f30\n"
			"fmsub.ps f30, f30, f30, f31\n"
			"fmsub.ps f30, f31, f31, f31\n"
			"fmadd.ps f31, f30, f31, f30\n"
			"fnmsub.ps f31, f31, f30, f31\n"
			"fnmsub.ps f30, f31, f30, f30\n"
			"fnmadd.ps f30, f30, f30, f30\n"
			"flog.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"flog.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"fnmsub.ps f2, f4, f14, f21\n"
			"fnmsub.ps f0, f21, f25, f3\n"
			"fmadd.ps f3, f27, f22, f23\n"
			"fnmsub.ps f16, f24, f30, f29\n"
			"flog.ps f1, f17\n"
			"faddi.pi f17, f17, 128\n"
			"fand.pi f7, f21, f28\n"
			"fmin.pi f7, f25, f6\n"
			"fadd.ps f31, f14, f9\n"
			"fsub.ps f4, f14, f11\n"
			"fadd.ps f20, f14, f22\n"
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
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
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
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
			"li x0, 0x6\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"li x5, 0x5a599e4be7697597\n"
			"fcvt.s.w f11, x5, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 832(x13)\n"
			"flw.ps f5, 640(x13)\n"
			"fle.s x26, f10, f5\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f11, 768(x19)\n"
			"fcvt.ps.f16 f6, f11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskand m1, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f16, 832(x6)\n"
			"flw.ps f2, 160(x6)\n"
			"fmulh.pi f28, f16, f2\n" 
			"fbci.ps f6, 0x7e3fc\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 544(x7)\n"
			"fsgnjn.s f7, f6, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 0(x5)\n"
			"flw.ps f7, 928(x5)\n"
			"fmadd.s f4, f6, f7, f6, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 576(x13)\n"
			"fsat8.pi f14, f20\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 544(x20)\n"
			"flw.ps f6, 416(x20)\n"
			"fremu.pi f14, f16, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 768(x20)\n"
			"flw.ps f25, 800(x20)\n"
			"fmaxu.pi f7, f26, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 352(x13)\n"
			"fcvt.lu.s x6, f7, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"flem.ps m2, f4, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 896(x19)\n"
			"flw.ps f5, 768(x19)\n"
			"flw.ps f7, 960(x19)\n"
			"fmsub.s f11, f3, f5, f7, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 704(x19)\n"
			"fclass.ps f11, f25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 128(x6)\n"
			"fcvt.ps.pw f2, f14, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xa3 << TENSOR_FMA32_SCP_LOC_B) | (0x68 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m4, f15, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f23, 288(x25)\n"
			"flw.ps f29, 768(x25)\n"
			"fxor.pi f11, f23, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 576(x10)\n"
			"fcvt.ps.pwu f11, f6, rne\n" 
			"li x16, 0x73d8e3255df90030\n"
			"fcvt.s.w f0, x16, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 224(x10)\n"
			"flw.ps f15, 992(x10)\n"
			"flw.ps f29, 0(x10)\n"
			"fmadd.s f23, f0, f15, f29, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 192(x10)\n"
			"fclass.ps f27, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 320(x6)\n"
			"fsrli.pi f5, f0, 0xd\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 352(x16)\n"
			"flw.ps f31, 608(x16)\n"
			"fmin.s f23, f25, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 192(x6)\n"
			"flw.ps f0, 32(x6)\n"
			"feq.pi f20, f15, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskxor m6, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 640(x18)\n"
			"flw.ps f17, 640(x18)\n"
			"flw.ps f8, 128(x18)\n"
			"fnmadd.ps f6, f6, f17, f8, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"maskor m4, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f5, 896(x23)\n"
			"fpackrepb.pi f23, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 96(x7)\n"
			"flw.ps f27, 576(x7)\n"
			"flw.ps f6, 384(x7)\n"
			"fmsub.ps f20, f11, f27, f6, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_1:\n"
			"maskand m1, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x25\n" 
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 160(x17)\n"
			"fadd.ps f4, f19, f19, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 672(x6)\n"
			"flw.ps f14, 768(x6)\n"
			"fmulh.pi f6, f0, f14\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 544(x17)\n"
			"flw.ps f27, 320(x17)\n"
			"fminu.pi f29, f20, f27\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 992(x7)\n"
			"flw.ps f19, 160(x7)\n"
			"flw.ps f24, 192(x7)\n"
			"fnmadd.s f27, f18, f19, f24, rmm\n" 
			"li x5, 0x734413a33b5028e6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe0c0c58e605da74\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f17, 928(x9)\n"
			"fslli.pi f17, f17, 0x7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 448(x28)\n"
			"fcvt.ps.pw f19, f20, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 992(x9)\n"
			"flw.ps f4, 992(x9)\n"
			"fsgnjn.ps f18, f29, f4\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 768(x11)\n"
			"fcvt.ps.pw f0, f27, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 832(x16)\n"
			"flw.ps f27, 640(x16)\n"
			"fsgnj.ps f14, f6, f27\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 640(x11)\n"
			"fpackrepb.pi f31, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 320(x28)\n"
			"fcvt.ps.pwu f20, f5, rdn\n" 
			"li x5, 0x8183\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f27, 64(x9)\n"
			"flw.ps f13, 672(x9)\n"
			"fmaxu.pi f24, f27, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 320(x16)\n"
			"flw.ps f27, 544(x16)\n"
			"feq.s x22, f4, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 384(x18)\n"
			"flw.ps f6, 640(x18)\n"
			"fsgnjn.ps f17, f23, f6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 256(x7)\n"
			"flw.ps f20, 288(x7)\n"
			"fcmovm.ps f2, f23, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 928(x18)\n"
			"fmul.ps f14, f27, f27, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x3 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x3 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"flem.ps m5, f31, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 288(x22)\n"
			"fround.ps f23, f9, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 768(x25)\n"
			"fpackreph.pi f10, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 352(x14)\n"
			"flw.ps f10, 288(x14)\n"
			"fminu.pi f14, f22, f10\n" 
			"maskpopc x14, m7\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 576(x12)\n"
			"flw.ps f9, 288(x12)\n"
			"fmin.pi f21, f21, f9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 160(x25)\n"
			"flw.ps f31, 800(x25)\n"
			"fsra.pi f4, f14, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 64(x22)\n"
			"flw.ps f17, 352(x22)\n"
			"fsub.ps f9, f23, f17, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 512(x16)\n"
			"flw.ps f10, 416(x16)\n"
			"fadd.ps f23, f22, f10, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 992(x16)\n"
			"fcvt.w.s x23, f23, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"fltm.pi m7, f22, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f14, 992(x5)\n"
			"flw.ps f2, 0(x5)\n"
			"fsra.pi f9, f14, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_3:\n"
			"flem.ps m7, f2, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f4, 320(x23)\n"
			"flw.ps f31, 192(x23)\n"
			"fsra.pi f16, f4, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 256(x9)\n"
			"flw.ps f17, 896(x9)\n"
			"fdivu.pi f4, f3, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 384(x9)\n"
			"fsrai.pi f31, f10, 0x7\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskor m5, m3, m4\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 928(x12)\n"
			"fcvt.pw.ps f3, f22, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 32(x5)\n"
			"fslli.pi f22, f3, 0x9\n" 
			"fbci.pi f16, 0x44582\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 448(x26)\n"
			"fcvt.ps.pwu f12, f26, rup\n" 
			"li x5, 0x7654\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 960(x28)\n"
			"fcvt.pw.ps f4, f2, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 704(x20)\n"
			"flw.ps f28, 928(x20)\n"
			"flw.ps f8, 640(x20)\n"
			"fmadd.s f29, f31, f28, f8, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 256(x16)\n"
			"flw.ps f2, 672(x16)\n"
			"feq.ps f23, f27, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"maskand m2, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_4:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 864(x5)\n"
			"fexp.ps f20, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 672(x26)\n"
			"flw.ps f29, 992(x26)\n"
			"flt.ps f12, f4, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 224(x26)\n"
			"flw.ps f6, 192(x26)\n"
			"flw.ps f4, 832(x26)\n"
			"fmsub.ps f4, f16, f6, f4, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 768(x16)\n"
			"flw.ps f7, 256(x16)\n"
			"fle.ps f12, f12, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 192(x16)\n"
			"flw.ps f29, 448(x16)\n"
			"flt.ps f6, f3, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 960(x25)\n"
			"flw.ps f31, 64(x25)\n"
			"fle.s x16, f2, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 160(x28)\n"
			"flw.ps f27, 256(x28)\n"
			"flt.ps f22, f12, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x2f << TENSOR_FMA32_SCP_LOC_B) | (0x34 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 672(x19)\n"
			"fcvt.wu.s x20, f24, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_5:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 672(x11)\n"
			"flog.ps f14, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 224(x20)\n"
			"flw.ps f24, 608(x20)\n"
			"fmax.ps f22, f0, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 416(x9)\n"
			"feq.ps f24, f19, f19\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 768(x14)\n"
			"flw.ps f11, 0(x14)\n"
			"fmulh.pi f13, f6, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 96(x17)\n"
			"flw.ps f26, 768(x17)\n"
			"fmul.s f0, f22, f26, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 736(x19)\n"
			"flw.ps f25, 32(x19)\n"
			"fle.ps f0, f7, f25\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 736(x20)\n"
			"fsll.pi f14, f13, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 160(x17)\n"
			"fcvt.pwu.ps f19, f21, rup\n" 
			"fbci.ps f14, 0x74912\n" 
			"mova.x.m x5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 160(x11)\n"
			"fclass.ps f14, f19\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 32(x6)\n"
			"flw.ps f24, 160(x6)\n"
			"fadd.pi f21, f7, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 416(x17)\n"
			"flw.ps f24, 800(x17)\n"
			"fsrl.pi f25, f14, f24\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 704(x9)\n"
			"flw.ps f22, 320(x9)\n"
			"fmax.pi f27, f9, f22\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 768(x23)\n"
			"flw.ps f1, 32(x23)\n"
			"fcmovm.ps f28, f29, f1\n" 
			"maskpopc x19, m2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 32(x19)\n"
			"flw.ps f21, 64(x19)\n"
			"fmul.s f30, f1, f21, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 960(x26)\n"
			"flw.ps f26, 192(x26)\n"
			"fsub.ps f29, f21, f26, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 672(x6)\n"
			"flw.ps f27, 864(x6)\n"
			"fadd.pi f12, f22, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 672(x23)\n"
			"flw.ps f26, 512(x23)\n"
			"fadd.s f11, f11, f26, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 736(x19)\n"
			"flw.ps f4, 128(x19)\n"
			"fsgnjx.ps f21, f21, f4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 800(x16)\n"
			"flw.ps f1, 832(x16)\n"
			"fsub.pi f18, f30, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f28, 672(x16)\n"
			"flw.ps f22, 576(x16)\n"
			"feq.pi f12, f28, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_6:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 128(x6)\n"
			"flog.ps f12, f27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 256(x5)\n"
			"flw.ps f10, 960(x5)\n"
			"fxor.pi f29, f5, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"masknot m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f1, 448(x26)\n"
			"flw.ps f26, 352(x26)\n"
			"fminu.pi f27, f1, f26\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 992(x25)\n"
			"flw.ps f29, 480(x25)\n"
			"fmul.s f18, f12, f29, rdn\n" 
			"fbci.ps f21, 0x7c44c\n" 
			"li x5, 0xacb6052e8ccc8790\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x27b96ece25de3e55\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x26, 0xc08fab824a5b187b\n"
			"fcvt.s.w f28, x26, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_606017_HID_6:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 448(x22)\n"
			"frsq.ps f21, f23\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 128(x16)\n"
			"flw.ps f29, 448(x16)\n"
			"flw.ps f25, 192(x16)\n"
			"fmsub.ps f23, f4, f29, f25, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 32(x7)\n"
			"flw.ps f4, 128(x7)\n"
			"fsra.pi f1, f17, f4\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 416(x25)\n"
			"flw.ps f29, 288(x25)\n"
			"fsub.pi f6, f17, f29\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 64(x7)\n"
			"flw.ps f29, 256(x7)\n"
			"for.pi f29, f25, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 928(x7)\n"
			"flw.ps f9, 864(x7)\n"
			"fnmadd.s f6, f17, f9, f9, rne\n" 
			"fbci.pi f21, 0x20cd4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 544(x22)\n"
			"flw.ps f3, 896(x22)\n"
			"fmin.s f17, f29, f3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 608(x26)\n"
			"flw.ps f1, 512(x26)\n"
			"fmul.pi f10, f4, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 64(x26)\n"
			"flw.ps f9, 736(x26)\n"
			"fsgnjx.s f1, f4, f9\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 96(x22)\n"
			"flw.ps f25, 352(x22)\n"
			"fmulh.pi f6, f17, f25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 160(x22)\n"
			"flw.ps f24, 288(x22)\n"
			"feq.ps f25, f29, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 256(x17)\n"
			"faddi.pi f16, f3, 0x114\n" 
			"la x22, rand_int_32\n"
			"flw.ps f23, 64(x22)\n"
			"flw.ps f30, 128(x22)\n"
			"fle.pi f4, f23, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"flem.ps m2, f24, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"mov.m.x m5, x12, 0x1\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
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
			"flw.ps f20, 928(x20)\n"
			"fslli.pi f30, f20, 0xf\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f10, 256(x20)\n"
			"fpackreph.pi f9, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 256(x7)\n"
			"flw.ps f21, 576(x7)\n"
			"flw.ps f9, 800(x7)\n"
			"fmsub.s f21, f11, f21, f9, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 224(x20)\n"
			"fslli.pi f24, f13, 0x6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 96(x28)\n"
			"fpackrepb.pi f2, f18\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 768(x7)\n"
			"flw.ps f12, 544(x7)\n"
			"fadd.pi f21, f10, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 672(x6)\n"
			"feq.ps f18, f20, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 160(x7)\n"
			"flw.ps f1, 640(x7)\n"
			"fsub.pi f12, f24, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"fsetm.pi m7, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 416(x5)\n"
			"flw.ps f13, 928(x5)\n"
			"fmax.s f1, f3, f13\n" 
			"fbci.pi f30, 0x20b68\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 704(x14)\n"
			"flw.ps f13, 800(x14)\n"
			"fsgnjx.s f29, f18, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 736(x14)\n"
			"fclass.ps f24, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 800(x20)\n"
			"flw.ps f1, 512(x20)\n"
			"fsgnj.s f14, f14, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f3, 288(x14)\n"
			"fsat8.pi f18, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 448(x25)\n"
			"flw.ps f12, 736(x25)\n"
			"fremu.pi f11, f14, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x4 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xe << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x2 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x7 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x4 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"masknot m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f24, 928(x26)\n"
			"flw.ps f12, 288(x26)\n"
			"fmulh.pi f31, f24, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"maskand m6, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 800(x7)\n"
			"flw.ps f18, 256(x7)\n"
			"fadd.s f20, f7, f18, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 512(x6)\n"
			"fcvt.wu.s x26, f6, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 928(x26)\n"
			"fcvt.w.s x9, f12, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 896(x13)\n"
			"flw.ps f13, 928(x13)\n"
			"fsgnjx.ps f5, f8, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 768(x11)\n"
			"fcvt.l.s x9, f8, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 64(x6)\n"
			"fcvt.pwu.ps f2, f13, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 512(x26)\n"
			"flw.ps f2, 672(x26)\n"
			"fmaxu.pi f8, f13, f2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 320(x11)\n"
			"flw.ps f13, 320(x11)\n"
			"flw.ps f8, 288(x11)\n"
			"fnmadd.ps f15, f24, f13, f8, rtz\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f20, 960(x7)\n"
			"fcvt.ps.f16 f25, f20\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 992(x25)\n"
			"faddi.pi f24, f18, 0x1af\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 608(x14)\n"
			"fcvt.w.s x6, f5, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f12, 736(x11)\n"
			"fpackreph.pi f30, f12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 64(x11)\n"
			"fcvt.ps.pwu f24, f8, rne\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 64(x9)\n"
			"fcvt.l.s x22, f30, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f1, 576(x22)\n"
			"flw.ps f12, 704(x22)\n"
			"fmin.pi f30, f1, f12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f12, 288(x22)\n"
			"fcvt.ps.pwu f2, f12, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 992(x16)\n"
			"flw.ps f27, 288(x16)\n"
			"flw.ps f1, 768(x16)\n"
			"fnmadd.s f12, f6, f27, f1, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 896(x25)\n"
			"flw.ps f10, 576(x25)\n"
			"fsub.pi f25, f1, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 928(x19)\n"
			"flw.ps f3, 576(x19)\n"
			"fle.s x22, f1, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_int_32\n"
			"flw.ps f0, 576(x13)\n"
			"fpackreph.pi f1, f0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 192(x22)\n"
			"flw.ps f28, 64(x22)\n"
			"flw.ps f29, 96(x22)\n"
			"fnmsub.s f25, f10, f28, f29, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 768(x13)\n"
			"fsin.ps f28, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 288(x5)\n"
			"fcvt.f16.ps f13, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 64(x16)\n"
			"flog.ps f29, f10\n" 
			"li x5, 0x8e0f\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"masknot m6, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2be631d82f\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"maskxor m5, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 832(x25)\n"
			"flw.ps f16, 832(x25)\n"
			"fmul.s f28, f3, f16, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 960(x20)\n"
			"flw.ps f13, 896(x20)\n"
			"fsub.s f22, f16, f13, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 128(x20)\n"
			"fsatu8.pi f22, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 608(x10)\n"
			"flw.ps f21, 448(x10)\n"
			"fsgnj.ps f1, f11, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 576(x23)\n"
			"flw.ps f11, 64(x23)\n"
			"feq.ps f11, f28, f11\n" 
			"mova.x.m x5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 960(x18)\n"
			"flw.ps f9, 224(x18)\n"
			"fmulhu.pi f0, f10, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 608(x20)\n"
			"flw.ps f9, 352(x20)\n"
			"fmax.s f25, f21, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 544(x16)\n"
			"flw.ps f8, 384(x16)\n"
			"fmax.pi f1, f10, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"maskor m1, m2, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xae987010ec\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"mov.m.x m2, x6, 0x48\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 288(x7)\n"
			"fclass.s x16, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 832(x23)\n"
			"fclass.ps f20, f5\n" 
			"maskpopc x6, m4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"feqm.ps m1, f3, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 0(x22)\n"
			"flw.ps f21, 864(x22)\n"
			"fadd.ps f28, f8, f21, rtz\n" 
			"li x22, 0xfcc055c3e24101eb\n"
			"fcvt.s.w f0, x22, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f25, 64(x12)\n"
			"flw.ps f5, 416(x12)\n"
			"fmax.pi f31, f25, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 544(x6)\n"
			"flw.ps f1, 0(x6)\n"
			"flw.ps f22, 832(x6)\n"
			"fcmov.ps f20, f28, f1, f22\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"fltm.pi m1, f25, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 576(x9)\n"
			"flw.ps f28, 96(x9)\n"
			"fcmovm.ps f1, f30, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 768(x15)\n"
			"flw.ps f28, 576(x15)\n"
			"fcmovm.ps f18, f19, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 480(x17)\n"
			"flw.ps f10, 384(x17)\n"
			"fmaxu.pi f28, f11, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 800(x6)\n"
			"flw.ps f15, 416(x6)\n"
			"fsub.pi f19, f26, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f20, 896(x6)\n"
			"flw.ps f26, 960(x6)\n"
			"fsrl.pi f27, f20, f26\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 32(x28)\n"
			"fclass.ps f20, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 352(x28)\n"
			"fcvt.l.s x26, f18, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 416(x26)\n"
			"fsqrt.s f30, f3, rdn\n" 
			"fbci.ps f29, 0x2e2a1\n" 
			"li x23, 0x23c29c116a1cd3e5\n"
			"fcvt.s.w f15, x23, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 960(x17)\n"
			"fcvt.pwu.ps f23, f23, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 736(x20)\n"
			"for.pi f29, f18, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 224(x20)\n"
			"flw.ps f25, 352(x20)\n"
			"flt.ps f0, f15, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"fltm.pi m1, f28, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 352(x20)\n"
			"fsin.ps f29, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_64996_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x0 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x4 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1a << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x2 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x0 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 416(x17)\n"
			"flw.ps f26, 480(x17)\n"
			"fdiv.s f2, f28, f26, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 544(x22)\n"
			"flw.ps f24, 224(x22)\n"
			"feq.ps f9, f2, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 96(x17)\n"
			"fcvt.pw.ps f23, f16, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 160(x17)\n"
			"flw.ps f26, 576(x17)\n"
			"fxor.pi f12, f28, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 512(x10)\n"
			"fcvt.ps.pwu f26, f7, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 448(x26)\n"
			"flw.ps f2, 640(x26)\n"
			"fmulhu.pi f8, f7, f2\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 64(x17)\n"
			"flw.ps f5, 352(x17)\n"
			"fmulhu.pi f2, f8, f5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 384(x26)\n"
			"flw.ps f7, 640(x26)\n"
			"fle.pi f25, f15, f7\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 224(x11)\n"
			"flw.ps f2, 352(x11)\n"
			"fnmsub.s f2, f7, f7, f2, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_13:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 512(x25)\n"
			"flog.ps f26, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 800(x26)\n"
			"flw.ps f2, 128(x26)\n"
			"flt.pi f20, f28, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 544(x18)\n"
			"flw.ps f26, 672(x18)\n"
			"fmax.ps f25, f20, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 672(x26)\n"
			"fclass.s x7, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 352(x18)\n"
			"fcvt.f16.ps f16, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 192(x11)\n"
			"flw.ps f28, 64(x11)\n"
			"fdiv.s f24, f15, f28, rtz\n" 
			"mova.x.m x10\n" 
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
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
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"mov.m.x m6, x14, 0x74\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 736(x12)\n"
			"flw.ps f0, 960(x12)\n"
			"fsgnjn.s f12, f24, f0\n" 
			"maskpopc x11, m1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 480(x12)\n"
			"flw.ps f27, 576(x12)\n"
			"fsgnj.ps f14, f29, f27\n" 
			"la x10, rand_int_32\n"
			"flw.ps f0, 640(x10)\n"
			"fcvt.ps.pw f30, f0, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"fltm.pi m2, f27, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 736(x10)\n"
			"fadd.ps f22, f12, f12, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 160(x14)\n"
			"fsqrt.ps f8, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 320(x14)\n"
			"flw.ps f22, 992(x14)\n"
			"flt.ps f23, f14, f22\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 544(x15)\n"
			"flw.ps f14, 608(x15)\n"
			"flw.ps f23, 0(x15)\n"
			"fcmov.ps f8, f7, f14, f23\n" 
			"li x18, 0x50b1c92e5d46558b\n"
			"fcvt.s.wu f9, x18, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 704(x14)\n"
			"fcvt.f16.ps f22, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 864(x12)\n"
			"fcvt.ps.pwu f29, f24, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 288(x18)\n"
			"flw.ps f12, 384(x18)\n"
			"flw.ps f22, 864(x18)\n"
			"fcmov.ps f8, f29, f12, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 416(x14)\n"
			"fcvt.wu.s x12, f7, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 992(x9)\n"
			"flw.ps f12, 160(x9)\n"
			"fmax.ps f8, f18, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 5\n"   
			"csrwi mhpmevent6, 7\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f23, 192(x10)\n"
			"flw.ps f2, 192(x10)\n"
			"fmulh.pi f14, f23, f2\n" 
			"fbci.ps f19, 0x7f19d\n" 
			"maskpopcz x26, m7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 896(x20)\n"
			"flw.ps f30, 480(x20)\n"
			"fsub.s f27, f21, f30, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"feqm.ps m2, f2, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 192(x18)\n"
			"flw.ps f30, 960(x18)\n"
			"flw.ps f19, 736(x18)\n"
			"fnmadd.s f8, f22, f30, f19, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 896(x15)\n"
			"fpackrepb.pi f29, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 896(x11)\n"
			"flw.ps f27, 832(x11)\n"
			"flw.ps f23, 768(x11)\n"
			"fmadd.ps f10, f12, f27, f23, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 640(x7)\n"
			"faddi.pi f21, f21, 0x1d3\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 32(x10)\n"
			"flw.ps f29, 800(x10)\n"
			"fltu.pi f30, f7, f29\n" 
			"la x16, rand_int_32\n"
			"flw.ps f12, 704(x16)\n"
			"flw.ps f22, 256(x16)\n"
			"fmulh.pi f12, f12, f22\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 576(x18)\n"
			"fclass.s x9, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 64(x11)\n"
			"flw.ps f2, 832(x11)\n"
			"flt.pi f1, f27, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 672(x26)\n"
			"flw.ps f7, 160(x26)\n"
			"flw.ps f12, 928(x26)\n"
			"fmsub.ps f7, f30, f7, f12, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"fltm.ps m7, f19, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f21, 160(x11)\n"
			"flw.ps f20, 288(x11)\n"
			"fsrl.pi f7, f21, f20\n" 
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
		0x25101b7b, 
		0x5966e0df, 
		0x68bf4a97, 
		0x66a41c63, 
		0x18b7d0b2, 
		0xade2b9a6, 
		0x3e99aecc, 
		0x79c84ed4, 
		0x22d06efe, 
		0xb6cd1a35, 
		0x45e00773, 
		0xa52731a4, 
		0xb733daeb, 
		0x1a23de2b, 
		0xbacf0af6, 
		0x1b7f606a, 
		0x3deb6c03, 
		0xf884c523, 
		0x968b0fe2, 
		0xc79eb4ac, 
		0xa4785eec, 
		0x1ec2320a, 
		0x356b89fa, 
		0xbfd8ab38, 
		0x3628511d, 
		0x6484c971, 
		0xe5da750b, 
		0x38e51180, 
		0xd30b336f, 
		0xc388d767, 
		0x9bdaa689, 
		0x09618295, 
		0xec7f647f, 
		0x1ec1e698, 
		0xc311a0d1, 
		0xec02a29a, 
		0xee9eb6f5, 
		0x0d9d5d28, 
		0xe6d465cf, 
		0x84d218d4, 
		0x718af9f3, 
		0x96986afa, 
		0x5d24b82f, 
		0xb728fe2c, 
		0x0a5fec8b, 
		0x2a04a5c5, 
		0x1d09f838, 
		0x39c190e4, 
		0xb355bc08, 
		0x1d2ec2bd, 
		0x4bc74f00, 
		0x3b995d2f, 
		0x8159ab4e, 
		0x838567d6, 
		0xb9bb5f64, 
		0xbd4984b2, 
		0xfdd7dba6, 
		0xffedef8d, 
		0xa074de5d, 
		0xbf59aaff, 
		0x71b7a265, 
		0xd3df48bb, 
		0x5dc932d4, 
		0x595911a2, 
		0x18c47f87, 
		0xeed115b5, 
		0x2166f497, 
		0x6c82e584, 
		0x0cfc6260, 
		0x51e99a11, 
		0x8c62972a, 
		0xf1c138b0, 
		0x68e99485, 
		0x14de5786, 
		0x321d02cc, 
		0xf3dd5c28, 
		0x16ed80d4, 
		0x734a0be2, 
		0xaeb4d240, 
		0x9012dd9e, 
		0x26e810a2, 
		0xc2a4b92c, 
		0x99cfa794, 
		0xc4f36eeb, 
		0x52592de5, 
		0x003ef0ec, 
		0xa9d0efd2, 
		0xa0dfa158, 
		0x7b200788, 
		0x0dd11f38, 
		0x18d6300b, 
		0x49a962c6, 
		0x1cc60dc7, 
		0x8bfa5110, 
		0x40469305, 
		0x647f3ba4, 
		0x6d1bc79c, 
		0xb5e51490, 
		0xb654ccf3, 
		0x4daed4e4, 
		0x2488acc2, 
		0x4ab17a4d, 
		0x9c2f7970, 
		0x370d0c32, 
		0xeb8fd703, 
		0x7c04d771, 
		0x8779818f, 
		0xbb1f7149, 
		0x2e23b1cc, 
		0xa0a2a98e, 
		0x31d98e8b, 
		0x7dd48d78, 
		0x74ac066e, 
		0xc262f034, 
		0xf0a64f7c, 
		0xb6333869, 
		0xf21b3a68, 
		0x21ea8adf, 
		0x3e6414af, 
		0x010677b1, 
		0x3228c503, 
		0xea6f7bff, 
		0xa8e5798b, 
		0xebee7961, 
		0xcf2cbc90, 
		0x0446c7a4, 
		0xe0bd035c, 
		0x725591d9, 
		0xe53a2622, 
		0xd7a9c89a, 
		0xaae93b7f, 
		0xad8f09b7, 
		0x66232be9, 
		0xcc7da43b, 
		0x81a10daf, 
		0xc1608c6b, 
		0x192162e2, 
		0xcd0f34d9, 
		0x1ce7eb6c, 
		0x0f26d352, 
		0x86dd7035, 
		0x6a8f9450, 
		0x954b2827, 
		0x81c33199, 
		0xefaa116f, 
		0x67036122, 
		0xb53cc0f6, 
		0x0b557bb8, 
		0x86f9ab29, 
		0xff8a997b, 
		0x9ef65134, 
		0x5d782e4b, 
		0x0ed32657, 
		0xb7ac3843, 
		0xd29baa91, 
		0x35921649, 
		0x15e77103, 
		0xa5c30533, 
		0x49ef0922, 
		0x0344b109, 
		0x327e7341, 
		0xf54a0722, 
		0xf0bc69ac, 
		0x83a3e32f, 
		0x43a937d1, 
		0x8be0c6da, 
		0x30066a4b, 
		0x369e9ff8, 
		0xfb63562d, 
		0xe60f3a81, 
		0xfd44da25, 
		0x4ea88c62, 
		0x7193d8fb, 
		0xb37b3432, 
		0x96f5adc2, 
		0xf0c96437, 
		0x52702957, 
		0x90cab741, 
		0x89f66b81, 
		0x2aa61729, 
		0x93fd934e, 
		0x256f910a, 
		0xd87cf802, 
		0x34bc3823, 
		0xf5836160, 
		0xfb331f15, 
		0x7be92072, 
		0x21c8f234, 
		0x88ec50f8, 
		0xe1bf28f3, 
		0xad191fa3, 
		0x443a6f38, 
		0x954b850b, 
		0xb4da587a, 
		0xea8c7076, 
		0x92d3fbd6, 
		0xc5893294, 
		0x66468f6b, 
		0xf69da5f1, 
		0xf9c69ec1, 
		0x75911579, 
		0xcc9d87f3, 
		0x7d00951d, 
		0x9b46e4f8, 
		0xef903251, 
		0x392801a9, 
		0x1ca074dc, 
		0x1d1fd9c3, 
		0x78e75819, 
		0x73203416, 
		0x82f13fbb, 
		0x43f49af5, 
		0x6fb0ebd5, 
		0x5ecff0d6, 
		0x910ee443, 
		0xd7eae207, 
		0x0258055b, 
		0x8c715976, 
		0xb79f53aa, 
		0x7050b5fd, 
		0xedeb820c, 
		0x6306bec0, 
		0x1434b9f1, 
		0xa178fec9, 
		0x896a8e94, 
		0x1fb3c5a3, 
		0x6aa8801c, 
		0x9b3a641b, 
		0x2773d78a, 
		0x5ca9ee93, 
		0x81936c29, 
		0x5ae4d2c4, 
		0xc7bc0652, 
		0xaffe6265, 
		0xc315bc4b, 
		0x86ee84f8, 
		0x762d65c0, 
		0x75d4c198, 
		0x799c730c, 
		0xc04fba89, 
		0x5ae38041, 
		0x6c9622a5, 
		0xb6a98fab, 
		0xe2df6c71, 
		0xb13ceb04, 
		0x44047e85, 
		0x401e1ef4, 
		0xce3ebbf1, 
		0x8dc55abb, 
		0xc43d0003, 
		0x5b03d173, 
		0xd9752144, 
		0x1a8e10f8, 
		0x4ffe2f85, 
		0x93f0b0d6, 
		0x444a5354  
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
		0x383f, 
		0x9006, 
		0xd320, 
		0xcf40, 
		0xd1cb, 
		0x9df5, 
		0xebe3, 
		0x7dc5, 
		0x50a0, 
		0xd3c0, 
		0xc400, 
		0x0000, 
		0x84be, 
		0xd000, 
		0x53a0, 
		0x834a, 
		0xd140, 
		0xfc00, 
		0x5360, 
		0x8000, 
		0x4b80, 
		0x3e10, 
		0x4980, 
		0xca46, 
		0xb4d0, 
		0x0f64, 
		0xe91a, 
		0xd2e0, 
		0x5d6c, 
		0x5240, 
		0x53a0, 
		0xbc00, 
		0x9cef, 
		0x52a0, 
		0x4e00, 
		0xf6d4, 
		0xc1bb, 
		0xbc8f, 
		0x53e0, 
		0x4c40, 
		0x4400, 
		0x641d, 
		0x8023, 
		0x4ec3, 
		0x0d15, 
		0xf36d, 
		0xcdc0, 
		0x53e0, 
		0x4500, 
		0x0000, 
		0x4d40, 
		0x8000, 
		0xfc00, 
		0x0000, 
		0xd0c0, 
		0x53e0, 
		0x4200, 
		0xcec0, 
		0x1510, 
		0xd3c0, 
		0xd0a0, 
		0x9134, 
		0xfc00, 
		0x4c26, 
		0x5080, 
		0x4e80, 
		0xd2a0, 
		0x50a0, 
		0xe902, 
		0x5320, 
		0xc600, 
		0x5300, 
		0xd3c0, 
		0xcd40, 
		0x4500, 
		0xeb7a, 
		0xbc00, 
		0xbb5d, 
		0xce80, 
		0x4fc0, 
		0x92ae, 
		0x51a0, 
		0xce80, 
		0x4523, 
		0xd3c0, 
		0xd060, 
		0x8000, 
		0x0000, 
		0xd120, 
		0xd1a4, 
		0x4d80, 
		0x8000, 
		0xcf40, 
		0x19c3, 
		0xd1a0, 
		0xd0a0, 
		0xccc0, 
		0x3289, 
		0x3bf7, 
		0xcec0, 
		0xcec0, 
		0xfc00, 
		0xd200, 
		0x53e0, 
		0xfc00, 
		0x8686, 
		0xd160, 
		0xd0be, 
		0xd240, 
		0x4b80, 
		0xd2c0, 
		0x5340, 
		0xed88, 
		0x4e40, 
		0x0ed1, 
		0xd380, 
		0x4b00, 
		0xd1a0, 
		0x51c0, 
		0x005c, 
		0x85d6, 
		0xcdc0, 
		0xccc0, 
		0x2b0d, 
		0x0dca, 
		0xe477, 
		0x4e80, 
		0xcb00, 
		0x59f1, 
		0x4f40, 
		0xa8f4, 
		0x0530, 
		0x291f, 
		0xd2a0, 
		0xcda2, 
		0xb06e, 
		0xca80, 
		0x4cc0, 
		0x0000, 
		0x4980, 
		0x522e, 
		0xd300, 
		0xfc00, 
		0x7c00, 
		0xc400, 
		0xf99a, 
		0xc700, 
		0xce40, 
		0x5140, 
		0x5300, 
		0x4fc0, 
		0xf26b, 
		0x7c00, 
		0xcd40, 
		0x5220, 
		0xa63c, 
		0x8825, 
		0x7dde, 
		0xc000, 
		0x4b80, 
		0xcf00, 
		0x3c00, 
		0x8000, 
		0x5260, 
		0x4460, 
		0xd000, 
		0x4ec0, 
		0xc200, 
		0xfc00, 
		0x4189, 
		0x8000, 
		0x0000, 
		0xc980, 
		0x3fac, 
		0x4dc0, 
		0xd1e0, 
		0x5100, 
		0xcc9b, 
		0x13f0, 
		0x0eab, 
		0xccc7, 
		0x4000, 
		0x8217, 
		0x2382, 
		0xf4c5, 
		0x4dc0, 
		0xc600, 
		0xcec0, 
		0xd020, 
		0x5080, 
		0xf8be, 
		0xe714, 
		0xfc00, 
		0xcd00, 
		0xd2a0, 
		0x5380, 
		0x5180, 
		0xd280, 
		0xd2c0, 
		0xcdc0, 
		0x8000, 
		0x2518, 
		0x0ad0, 
		0x2ae6, 
		0x5280, 
		0xd160, 
		0xfcf3, 
		0xcec0, 
		0x4200, 
		0x5a1c, 
		0xe5ed, 
		0x74ad, 
		0xc200, 
		0xc700, 
		0x7c00, 
		0x4000, 
		0x4200, 
		0x1286, 
		0xd260, 
		0xccc0, 
		0x4400, 
		0xcd80, 
		0x4a00, 
		0x4b80, 
		0x31a9, 
		0xd120, 
		0x4c00, 
		0x4800, 
		0xd280, 
		0xa1f0, 
		0x8000, 
		0xc880, 
		0xcec0, 
		0xd940, 
		0x8066, 
		0xcf00, 
		0xd200, 
		0xcf40, 
		0x4f80, 
		0x8000, 
		0x0000, 
		0x1386, 
		0x718f, 
		0x0778, 
		0xd0e0, 
		0xc600, 
		0x3e43, 
		0x4ec0, 
		0xd3c0, 
		0x7c00, 
		0x3188, 
		0xce40, 
		0xc900, 
		0xd1c0, 
		0xdec4, 
		0x3d1b, 
		0xd2e0, 
		0xcec0, 
		0xfc00, 
		0xb41d, 
		0x4600, 
		0xcd00, 
		0x1c0c, 
		0xc800, 
		0xcf80, 
		0xe49d, 
		0x7d85, 
		0x4b80, 
		0xcdc0, 
		0x0f8b, 
		0xc9c7, 
		0xd0e0, 
		0x4880, 
		0x4c40, 
		0xc200, 
		0xd240, 
		0xdbd6, 
		0xd360, 
		0x4a00, 
		0x33e1, 
		0xce40, 
		0xd080, 
		0x77b5, 
		0x5200, 
		0xcb00, 
		0x84c4, 
		0x5000, 
		0xcdc0, 
		0x5080, 
		0x67e6, 
		0xd000, 
		0x4a80, 
		0x51c0, 
		0xd1a0, 
		0xc600, 
		0x4f00, 
		0x4c40, 
		0xd3c0, 
		0x2ec0, 
		0xfd6c, 
		0x5160, 
		0xdc12, 
		0xa111, 
		0x5060, 
		0x4000, 
		0x4f00, 
		0xd3e0, 
		0xd360, 
		0x7c49, 
		0xcd80, 
		0xd300, 
		0x7abb, 
		0xc980, 
		0xc980, 
		0xd300, 
		0x4cc0, 
		0x5360, 
		0x036f, 
		0x6485, 
		0x5380, 
		0x5ead, 
		0xd120, 
		0x001d, 
		0xcdc0, 
		0x4e6d, 
		0x4c00, 
		0x5060, 
		0x4f00, 
		0x3893, 
		0x4e40, 
		0xd2a0, 
		0x4880, 
		0x52e0, 
		0x5160, 
		0x4200, 
		0xfc00, 
		0x8bf3, 
		0x1b28, 
		0x4e00, 
		0xd300, 
		0xd280, 
		0x4c00, 
		0x5060, 
		0x50a0, 
		0x74d6, 
		0x51e0, 
		0x695c, 
		0xc800, 
		0x5000, 
		0x6bc8, 
		0x5260, 
		0xcd40, 
		0x4cc0, 
		0x7db7, 
		0x7022, 
		0x4ec0, 
		0x6c60, 
		0x03a5, 
		0xd0a0, 
		0xc880, 
		0x4200, 
		0x0e68, 
		0xcb80, 
		0x5300, 
		0xfc00, 
		0x64b3, 
		0xc400, 
		0x0000, 
		0xd1a0, 
		0x5ac0, 
		0x5080, 
		0xc700, 
		0x52e0, 
		0xfc00, 
		0xe3eb, 
		0x3c11, 
		0x5180, 
		0xd340, 
		0xcd40, 
		0x4f80, 
		0xcfc0, 
		0x4000, 
		0x7c00, 
		0xd120, 
		0x51c0, 
		0x6d57, 
		0x8000, 
		0xc880, 
		0x4700, 
		0xd080, 
		0x3c00, 
		0xc500, 
		0xfc00, 
		0xd0a0, 
		0x0000, 
		0xccc0, 
		0xcf80, 
		0xe5cf, 
		0xfd20, 
		0x4f80, 
		0xed23, 
		0x921f, 
		0x51a0, 
		0x9e14, 
		0xfcb5, 
		0xc500, 
		0xcec0, 
		0x53e0, 
		0xcf40, 
		0x4d00, 
		0x0000, 
		0xcc80, 
		0xc1e3, 
		0x5000, 
		0x5020, 
		0x4200, 
		0xca4a, 
		0xcf40, 
		0x585b, 
		0x4770, 
		0xd040, 
		0xd1e0, 
		0x5180, 
		0x3efa, 
		0xc500, 
		0x53ee, 
		0x5040, 
		0x5140, 
		0x5100, 
		0x5080, 
		0xce00, 
		0x4b00, 
		0x4700, 
		0x8000, 
		0x1188, 
		0xc000, 
		0x5000, 
		0xd300, 
		0xb5d0, 
		0x50a0, 
		0x40e4, 
		0x50c0, 
		0xa04d, 
		0x560b, 
		0x0000, 
		0xd260, 
		0xddbc, 
		0xd200, 
		0x2643, 
		0x4fc0, 
		0xd0e0, 
		0xcf80, 
		0x0000, 
		0x14f2, 
		0xcdc0, 
		0x4800, 
		0xc880, 
		0xfc00, 
		0xd1c0, 
		0xab8e, 
		0x7c00, 
		0x628f, 
		0x0000, 
		0x5000, 
		0xcd00, 
		0xcc80, 
		0x4c40, 
		0x53a0, 
		0xd120, 
		0x0000, 
		0x1d6c, 
		0xcb79, 
		0x7c6e, 
		0xcc40, 
		0x4c40, 
		0x5d7b, 
		0x3c00, 
		0xfbbb, 
		0x5020, 
		0xc600, 
		0xcdc0, 
		0xcfc0, 
		0x6f82, 
		0x7b78, 
		0x4c80, 
		0x5060, 
		0x87f7, 
		0x12fa, 
		0x1d21, 
		0xd380, 
		0x5340, 
		0xcf80, 
		0x0000, 
		0xd3c0, 
		0xbc00, 
		0x4e00, 
		0xcb80, 
		0xd260, 
		0xd120, 
		0xd020, 
		0x4fc0, 
		0xd180, 
		0x50c0, 
		0xfc00, 
		0x8dd6, 
		0x6d15, 
		0xfb93, 
		0xc7f9, 
		0x5140, 
		0x152c, 
		0xcb00, 
		0x4000, 
		0x4b80, 
		0x6215, 
		0xcc40, 
		0xec1e, 
		0xd360, 
		0xd1a0, 
		0x0699, 
		0xd120, 
		0x1a30, 
		0xd3c0, 
		0x413c, 
		0x50a0, 
		0x858c, 
		0x5280, 
		0x5000, 
		0xcad0, 
		0x7c00, 
		0x02c7, 
		0x8000, 
		0xaf63, 
		0xd065, 
		0x4b80, 
		0x99f2, 
		0xc200, 
		0x5280, 
		0x53c0, 
		0xc600, 
		0xe74b, 
		0xd020, 
		0x6e24, 
		0x11a4, 
		0x639f, 
		0xcf80, 
		0x2f5b, 
		0xd340, 
		0x1966, 
		0xce40, 
		0xbd9e, 
		0xcf40, 
		0x4f00, 
		0x4980, 
		0xff71, 
		0xe72e, 
		0x4900, 
		0x4e00, 
		0xcc00, 
		0x4a80, 
		0xd0c0, 
		0xc880, 
		0x5240, 
		0x4000, 
		0x4a80, 
		0x4389, 
		0xd320, 
		0xcd80, 
		0x9e6a, 
		0xbd26, 
		0x5080, 
		0x50c0, 
		0xd1c0, 
		0xc900, 
		0x5380, 
		0x4dc0, 
		0x71fc, 
		0xce80, 
		0xf5fb, 
		0x5020, 
		0xabd8, 
		0xc400, 
		0x5140, 
		0x0000, 
		0xc500, 
		0x5160, 
		0xa6cc, 
		0xbbdb, 
		0x5060, 
		0x6f0e, 
		0xd040, 
		0x0000, 
		0x8000, 
		0xfc00, 
		0xcb80, 
		0xce80, 
		0x4c80, 
		0xc828, 
		0x8000, 
		0xfcde, 
		0x89f6, 
		0x8000, 
		0x419e, 
		0xd060, 
		0x50c0, 
		0x2f1b, 
		0x5200, 
		0x51e0, 
		0x5040, 
		0x00d7, 
		0x4692, 
		0x5080, 
		0xd000, 
		0xd1e0, 
		0xea55, 
		0x4d40, 
		0x4a00, 
		0xc322, 
		0xd5b0, 
		0xefa8, 
		0xcd40, 
		0xd1ed, 
		0x5260, 
		0x5220, 
		0x5200, 
		0x1c14, 
		0x4f40, 
		0x8000, 
		0xd340, 
		0xcb80, 
		0x4c00, 
		0x8000, 
		0x4d40, 
		0xd320, 
		0x7cb8, 
		0x4a00, 
		0x42f7, 
		0xcec0, 
		0xc600, 
		0x5180, 
		0x4700, 
		0xd260, 
		0xd240, 
		0xce40, 
		0xcc80, 
		0xca80, 
		0xd040, 
		0x4000, 
		0x4a00, 
		0xd2c0, 
		0xd260, 
		0x2fa1, 
		0xb933, 
		0xd320, 
		0x4f80, 
		0x5240, 
		0x5340, 
		0xfc00, 
		0x5e94, 
		0x2c0e, 
		0x5200, 
		0x7c00, 
		0x231d, 
		0x52e0, 
		0x5220, 
		0x0000, 
		0xd1c0, 
		0xd2c0, 
		0x5100, 
		0x3cb9, 
		0x52c0, 
		0x0000, 
		0x0d89, 
		0xd280, 
		0xcf80, 
		0x5300, 
		0x8000, 
		0x5020, 
		0x425a, 
		0x4c80, 
		0x5380, 
		0x52c0, 
		0x1b39, 
		0xc800, 
		0x0000, 
		0xce40, 
		0x0000, 
		0x5380, 
		0xd240, 
		0xd140, 
		0x4f80, 
		0x52a0, 
		0x49cc, 
		0xfc00, 
		0x5260, 
		0x53e0, 
		0xd380, 
		0x4ec0, 
		0x4c80, 
		0x4d40, 
		0x4e80, 
		0xa533, 
		0x5380, 
		0x2989, 
		0x0000, 
		0x0000, 
		0x6aa8, 
		0xca00, 
		0x03cc, 
		0xa207, 
		0xd340, 
		0x4cc0, 
		0x7c00, 
		0xd260, 
		0x53c0, 
		0x4800, 
		0x4b80, 
		0x8000, 
		0xc500, 
		0x50a0, 
		0xa7a9, 
		0xd080, 
		0x4d80, 
		0x5160, 
		0xdb76, 
		0xd1c0, 
		0xb47d, 
		0xd0c0, 
		0x8000, 
		0x7379, 
		0xd300, 
		0x0000, 
		0xed3e, 
		0xce00, 
		0x0000, 
		0x4700, 
		0xcec0, 
		0x4200, 
		0x09ba, 
		0x4f00, 
		0x82de, 
		0x4d00, 
		0xcf00, 
		0x32ef, 
		0x4500, 
		0x449c, 
		0xcd00, 
		0xd100, 
		0xe6d6, 
		0xc000, 
		0xcc80, 
		0x61d1, 
		0x82a2, 
		0x51e0, 
		0x5140, 
		0xd3c0, 
		0xce40, 
		0x82aa, 
		0xd1e0, 
		0x4c80, 
		0x5280, 
		0x4cc0, 
		0x51c0, 
		0x5380, 
		0xc200, 
		0xfc00, 
		0x2652, 
		0xd1a0, 
		0xcec0, 
		0x0a40, 
		0x29df, 
		0x0000, 
		0xf703, 
		0x4900, 
		0xc000, 
		0x9e94, 
		0xd000, 
		0x5240, 
		0x0000, 
		0xb3cb, 
		0x4400, 
		0xcc00, 
		0x4cc0, 
		0xd340, 
		0xd320, 
		0xd1a0, 
		0x8000, 
		0xd140, 
		0xd040, 
		0xcd00, 
		0x8000, 
		0x4cc0, 
		0xcf00, 
		0x5360, 
		0x51a0, 
		0xce80, 
		0xf0e0, 
		0xc900, 
		0xd260, 
		0xd320, 
		0x5120, 
		0xd1c0, 
		0xc700, 
		0x4b00, 
		0x4600, 
		0x51e0, 
		0xd2c0, 
		0xd100, 
		0x825e, 
		0x4f80, 
		0x4200, 
		0xd160, 
		0x50a0, 
		0x8000, 
		0x5426, 
		0xca80, 
		0x51b9, 
		0x3c00, 
		0x1f11, 
		0x51e0, 
		0xc900, 
		0xc431, 
		0xfc00, 
		0x50e0, 
		0x5220, 
		0x93d4, 
		0xca00, 
		0x4000, 
		0x5120, 
		0xc000, 
		0x50c0, 
		0xd020, 
		0x53c0, 
		0xb0b0, 
		0x91ea, 
		0xcc80, 
		0xca80, 
		0xcc40, 
		0xf971, 
		0xc600, 
		0xcf40, 
		0xff7e, 
		0x02fc, 
		0xfc82, 
		0x712f, 
		0x5260, 
		0x4cc0, 
		0xd26d, 
		0xcd80, 
		0x8da8, 
		0x4dc0, 
		0x3a60, 
		0xfc00, 
		0x0000, 
		0x51e0, 
		0xd220, 
		0x51b1, 
		0x7c00, 
		0x7c00, 
		0x50a0, 
		0x0000, 
		0x0c01, 
		0xd3e0, 
		0xd2e0, 
		0xd040, 
		0xd240, 
		0x50e0, 
		0x4fc0, 
		0xcd40, 
		0xcd80, 
		0x1119, 
		0x4d00, 
		0x6430, 
		0x0000, 
		0xba76, 
		0xfc00, 
		0xcf40, 
		0xcf00, 
		0x5040, 
		0xc900, 
		0x53a0, 
		0xd0c0, 
		0xca4e, 
		0x5160, 
		0x4e00, 
		0xd1a0, 
		0x8c24, 
		0x4e40, 
		0x7435, 
		0x8000, 
		0x6099, 
		0xccc0, 
		0xcb00, 
		0x4ec0, 
		0x47b3, 
		0x5ac1, 
		0x2fc3, 
		0xfd81, 
		0xd2c0, 
		0x4000, 
		0x7c00, 
		0x4e40, 
		0x8000, 
		0x5220, 
		0xc2d6, 
		0xda60, 
		0xa8f3, 
		0x4e00, 
		0x5697, 
		0x52a0, 
		0x8000, 
		0x4d80, 
		0xcb00, 
		0x0034, 
		0x8000, 
		0x8000, 
		0x51a0, 
		0x50c6, 
		0x4d80, 
		0x4c80, 
		0xccc0, 
		0xce00, 
		0x1706, 
		0xc500, 
		0xcfc0, 
		0xd240, 
		0xc880, 
		0x5300, 
		0xd260, 
		0x5120, 
		0x52a0, 
		0x4980, 
		0x1dd8, 
		0xd140, 
		0xcd00, 
		0xcd00, 
		0xe721, 
		0xd3e0, 
		0x4c31, 
		0x0000, 
		0x5220, 
		0x3c00, 
		0x5020, 
		0xc800, 
		0xcc00, 
		0xbc00, 
		0x1caf, 
		0x3304, 
		0xef9f, 
		0x3c00, 
		0xe19e, 
		0x5240, 
		0x4500, 
		0xcc00, 
		0xc600, 
		0xa082, 
		0xca80, 
		0xcdc0, 
		0x4cc0, 
		0xd220, 
		0xc01f, 
		0x4980, 
		0x5000, 
		0xc000, 
		0x4fc0, 
		0x9d10, 
		0x4550, 
		0x53e0, 
		0x5060, 
		0xcc80, 
		0xe5eb, 
		0xd080, 
		0x50e0, 
		0x4e38, 
		0xe1ad, 
		0x5160, 
		0xd1a0, 
		0x83d4, 
		0x4a00, 
		0xd3c0, 
		0x5320, 
		0x0000, 
		0x51a0, 
		0x52a0, 
		0x426d, 
		0xd0c3, 
		0xa34f, 
		0x4369, 
		0xcb00, 
		0x122b, 
		0x0000, 
		0x5220, 
		0xf126, 
		0xcfc0, 
		0xcfc0, 
		0x51a0, 
		0xca80, 
		0xd220, 
		0x5040, 
		0xd1e0, 
		0x7c00, 
		0xcf00, 
		0x53c0, 
		0xcdc0, 
		0xff09, 
		0x4e00, 
		0xd1e0, 
		0x2784, 
		0xd120, 
		0x0000, 
		0xd340, 
		0x7f86, 
		0x5340, 
		0x8000, 
		0xe2a1, 
		0xdec3, 
		0xbfd6, 
		0xc755, 
		0x4d40, 
		0xf897, 
		0x5260, 
		0xd177, 
		0xc500, 
		0x53a0, 
		0xec60, 
		0x08cf, 
		0x4c40, 
		0xc900, 
		0xa4b4, 
		0xd2c0, 
		0xde18, 
		0xce7c, 
		0x4c40, 
		0xd060, 
		0x8dae, 
		0xd320, 
		0xc400, 
		0x4e80, 
		0xf73f, 
		0xc800, 
		0xd120, 
		0x50c0, 
		0xc167, 
		0x52c0, 
		0xd88b, 
		0x52c0, 
		0xbaca, 
		0x043f, 
		0xfdff, 
		0x4d80, 
		0xc000, 
		0xd360, 
		0x7c00, 
		0xd140, 
		0x4700, 
		0xcc00, 
		0xcc40, 
		0x5060, 
		0x7c00, 
		0x2991, 
		0x5160, 
		0x8336, 
		0x2809, 
		0x51a0, 
		0xcf80, 
		0xe559, 
		0x4e00, 
		0x7c00, 
		0x3c00, 
		0x0000, 
		0x0635, 
		0x5220, 
		0x4d00, 
		0x9633, 
		0xd100, 
		0x60de, 
		0x5300, 
		0x4e80, 
		0xd220, 
		0xd100, 
		0x3c00, 
		0x275e, 
		0x4700, 
		0xd020, 
		0x51a0, 
		0xd000, 
		0x926d, 
		0xc000, 
		0x5c45, 
		0xd200, 
		0xa9af, 
		0x8000, 
		0x5180, 
		0xcadd, 
		0x4f80, 
		0xd3c0, 
		0xa458, 
		0xadb2, 
		0xccc0, 
		0x485a, 
		0x7c00, 
		0xc3ef, 
		0x7ced, 
		0x74b1, 
		0x5360, 
		0xd2c0, 
		0xc1eb, 
		0xd3c0, 
		0xce00, 
		0xb01d, 
		0x5200, 
		0x7dd0, 
		0x5120, 
		0x0aa1, 
		0x5200, 
		0x8000, 
		0xa853, 
		0xd000, 
		0xbc00, 
		0x4b00, 
		0xab44, 
		0x7cfa, 
		0xd140, 
		0xd0e0, 
		0xd3e0, 
		0xf808, 
		0x51a0, 
		0x5cd6, 
		0x4f00, 
		0xce40, 
		0x4dc0, 
		0xd0a0, 
		0x2bd4, 
		0x4dc0, 
		0x4a80, 
		0x53e0, 
		0x0752, 
		0xd000, 
		0xcb80, 
		0x5cf7, 
		0x53e0, 
		0x9963, 
		0xc880, 
		0x25fb, 
		0x4b00, 
		0xfc00, 
		0xfc7d, 
		0xc880, 
		0x52f9, 
		0x7c00, 
		0xc400, 
		0x1eb2, 
		0x0280, 
		0x56e4, 
		0x7619, 
		0x53e0, 
		0xd020, 
		0xbc00, 
		0x5000, 
		0xd8f4, 
		0x5320, 
		0xd86c, 
		0x4607, 
		0x862f, 
		0x5240, 
		0xd3a0, 
		0xd220, 
		0xcf80, 
		0xcf80, 
		0x4a00, 
		0x53c0, 
		0xd0c0, 
		0xbf09, 
		0xe6ea, 
		0xd380, 
		0xcdc0, 
		0xa370, 
		0x94eb, 
		0xc441, 
		0x2127, 
		0xb9c0, 
		0x4a00, 
		0x0000, 
		0xd080, 
		0xccc0, 
		0x5180, 
		0x0f49, 
		0x4c40, 
		0x4700, 
		0xcc80, 
		0x9d8e, 
		0x4880, 
		0x8000, 
		0xd340, 
		0x8000, 
		0xccc0, 
		0xce80, 
		0x4534, 
		0xbc00, 
		0x01f8, 
		0xd2c0, 
		0x4dc0, 
		0x3de0, 
		0x5240, 
		0xfc00, 
		0x8000, 
		0xcee2, 
		0xd0c0, 
		0xd020, 
		0xcb80, 
		0xce40, 
		0x8000, 
		0x6f03, 
		0xfd6a, 
		0xcc80, 
		0x8000, 
		0x5280, 
		0xfc00, 
		0x4bb3, 
		0x5020, 
		0xd3c0, 
		0x4dc0, 
		0xd1e0, 
		0x40a7, 
		0x4cc0, 
		0xd200, 
		0x4cc0, 
		0xd3a0, 
		0x5040, 
		0x5260, 
		0x5f65, 
		0xd300, 
		0xf150, 
		0x7d3d, 
		0x4e00, 
		0xcac9, 
		0xd0c0, 
		0x0000, 
		0x5bc7, 
		0x5340, 
		0x4c40, 
		0x5000, 
		0x4c40, 
		0x7735, 
		0x57be, 
		0xc700, 
		0x4e40, 
		0x8000, 
		0xd260, 
		0x7e3a, 
		0x5180, 
		0x4b00, 
		0xcb80, 
		0x5080, 
		0xd340, 
		0x4a00, 
		0xbd16, 
		0x6e12, 
		0xb46f, 
		0xca80, 
		0xd360, 
		0xcd00, 
		0xd300, 
		0x0000, 
		0x4cc0, 
		0xc400, 
		0x5320, 
		0x67be, 
		0xd100, 
		0x0000, 
		0xcb80, 
		0xacb3, 
		0xcf40, 
		0x7590, 
		0x4e80, 
		0xf958, 
		0x0000, 
		0x4b00, 
		0x8000, 
		0x09d5, 
		0xa4b8, 
		0x8000, 
		0xd3c0, 
		0xfc00, 
		0xd320, 
		0x612b, 
		0x5360, 
		0xa858, 
		0x0b7d, 
		0x4400, 
		0x52c0, 
		0x3c00, 
		0xcdc0, 
		0x6827, 
		0xd67e, 
		0x5240, 
		0xd3e0, 
		0x7c9c, 
		0x4d40, 
		0xd0e0, 
		0x7ce2, 
		0xc600, 
		0x44b3, 
		0x4b80, 
		0x5020, 
		0x5180, 
		0xc600, 
		0xd200, 
		0xcec0, 
		0x52c0, 
		0xcd40, 
		0x3c00, 
		0x51a0, 
		0xd060, 
		0x5340, 
		0x4b00, 
		0x5ddc, 
		0x9f66, 
		0x5100, 
		0x52c0, 
		0xd0c0, 
		0x6bd0, 
		0x5220, 
		0xcd40, 
		0xb9f1, 
		0x5751, 
		0x5247, 
		0xcb80, 
		0x4e80, 
		0x2415, 
		0x8000, 
		0xc3da, 
		0x7d13, 
		0x0000, 
		0x4200, 
		0xcfc0, 
		0xc500, 
		0x5000, 
		0x4b80, 
		0xfc00, 
		0x8f3e, 
		0x5120, 
		0x08be, 
		0xd0a0, 
		0x89a2, 
		0x2f15, 
		0x4880, 
		0xd000, 
		0x3cf6, 
		0xd340, 
		0x59af, 
		0x4d80, 
		0xfc00, 
		0xfd4d, 
		0x4a80, 
		0xc600, 
		0xc052, 
		0x0000, 
		0x78af, 
		0x20ee, 
		0x0000, 
		0x53a0, 
		0x5280, 
		0x52c0, 
		0xd300, 
		0xd1a0, 
		0x0000, 
		0x4ec0, 
		0xca80, 
		0xd200, 
		0x5120, 
		0x716e, 
		0x4c40, 
		0x5380, 
		0xd0c0, 
		0x4dc0, 
		0xd1a0, 
		0x3f06, 
		0x4880, 
		0x4ce3, 
		0x0000, 
		0x4e40, 
		0x55df, 
		0x542e, 
		0xd1e0, 
		0x5260, 
		0xc654, 
		0xcc00, 
		0x27f6, 
		0x0000, 
		0x8486, 
		0x5160, 
		0x4b80, 
		0xd3c0, 
		0xd3a0, 
		0x53a0, 
		0x5140, 
		0x4d80, 
		0x4c80, 
		0x0000, 
		0x4f00, 
		0xb0bb, 
		0xbc00, 
		0x0a36, 
		0x4e80, 
		0x8000, 
		0xf06f, 
		0x4ec0, 
		0xd0e0, 
		0x5320, 
		0xf5fe, 
		0x4e40, 
		0xd220, 
		0xd260, 
		0x76e5, 
		0x86c6, 
		0x5300, 
		0x4500, 
		0x71d9, 
		0xa08b, 
		0xcc00, 
		0x5020, 
		0x7da1, 
		0xc500, 
		0xd220, 
		0xcc80, 
		0xfd30, 
		0xd181, 
		0x8fbf, 
		0x4cc0, 
		0x5140, 
		0xb717, 
		0xcf40, 
		0xc400, 
		0xc600, 
		0xc000, 
		0xeaab, 
		0xd380, 
		0x4400, 
		0x4a80, 
		0xd040, 
		0x8255, 
		0x5040, 
		0x5240, 
		0x647b, 
		0x0000, 
		0x8757, 
		0x5300, 
		0xd040, 
		0xd260, 
		0x7c47, 
		0x52a0, 
		0x8b79, 
		0xce00, 
		0x52e0, 
		0x4f40, 
		0xbbfa, 
		0xcd80, 
		0x52e0, 
		0xb5ed, 
		0x8000, 
		0xd240, 
		0xcec0, 
		0x1f5a, 
		0x3c00, 
		0x50c0, 
		0x2879, 
		0x5340, 
		0x4e40, 
		0xc880, 
		0xd260, 
		0xd3e0, 
		0x7155, 
		0xccc0, 
		0x46af, 
		0xfc00, 
		0xd0e0, 
		0x4d40, 
		0xd060, 
		0x4e80, 
		0xc980, 
		0xd1c0, 
		0x5340, 
		0x4e40, 
		0xfc00, 
		0xd3c0, 
		0x50a0, 
		0x4d00, 
		0x7cf8, 
		0x5041, 
		0x4cc0, 
		0x8698, 
		0x52e0, 
		0xd2c0, 
		0x15d4, 
		0x5300, 
		0xcdc0, 
		0xec89, 
		0x51c0, 
		0xc170, 
		0xd3a0, 
		0x0000, 
		0xfc00, 
		0x4700, 
		0x51fc, 
		0xac49, 
		0xc200, 
		0xcc40, 
		0xe47f, 
		0xcc80, 
		0xcfc0, 
		0x7fc8, 
		0xca00, 
		0x5080, 
		0xa656, 
		0xd0e0, 
		0xd260, 
		0x4000, 
		0xd1c0, 
		0x5140, 
		0x0000, 
		0x4c40, 
		0xd340, 
		0x3740, 
		0x4f0c, 
		0x4e40, 
		0x9f82, 
		0x53e0, 
		0x4a80, 
		0xc600, 
		0xc500, 
		0x4000, 
		0x5280, 
		0xd360, 
		0xb97f, 
		0x4980, 
		0x5060  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc0c00000, 
		0xe00e1c9d, 
		0xff800000, 
		0x00020000, 
		0x00040000, 
		0x0c7ff800, 
		0x00386838, 
		0x55555555, 
		0xc2500000, 
		0x0ee534b1, 
		0x6e887e19, 
		0x40400000, 
		0x41d00000, 
		0xcaa36fa3, 
		0xd23b6bf6, 
		0x80000080, 
		0x00800000, 
		0x0e000fff, 
		0xc1e00000, 
		0x8f7fffff, 
		0xc2700000, 
		0x42280000, 
		0x0c7fffe0, 
		0xff7fffff, 
		0xe89426b8, 
		0x516c0942, 
		0x2841fb1f, 
		0x00000000, 
		0x00000000, 
		0x00000000, 
		0x00000008, 
		0xc2580000, 
		0xa5333ce5, 
		0x3f028f5c, 
		0x0f7fffff, 
		0xff931fec, 
		0x42040000, 
		0xc1c00000, 
		0x0e000003, 
		0x00000001, 
		0x0e01ffff, 
		0x0f7ffffd, 
		0x0e007fff, 
		0x08e36fe5, 
		0xc22c0000, 
		0xcd657b20, 
		0x41500000, 
		0x80000000, 
		0x0c7ffffe, 
		0x0c3ba777, 
		0xda6f9ad1, 
		0xff800000, 
		0xc1f00000, 
		0x8e3f983a, 
		0x00000000, 
		0xc1a00000, 
		0x0c7ffffe, 
		0xd4feddba, 
		0x0c780000, 
		0xff800000, 
		0x0d00fff0, 
		0x0f7fffff, 
		0x7fc00000, 
		0x41c80000, 
		0x7f7fffff, 
		0xc1e80000, 
		0x3d35ec67, 
		0xa5af0c00, 
		0x6763fb86, 
		0x8f7fffff, 
		0x6542d01e, 
		0x80000200, 
		0x007fffff, 
		0xc2080000, 
		0x42300000, 
		0x00800003, 
		0xac75d9c6, 
		0xff970f1f, 
		0xc06e5672, 
		0xc8ba9816, 
		0x0038ebe2, 
		0x7f7ffffe, 
		0x7fbfffff, 
		0x91ae67af, 
		0x0c7f8000, 
		0x80800002, 
		0xffc00000, 
		0x00000008, 
		0x00000000, 
		0x3f800001, 
		0x42780000, 
		0x9679aada, 
		0xdab8bd3a, 
		0x57485e7a, 
		0xc2340000, 
		0x0c7fff00, 
		0xc1b00000, 
		0x7f7ffffe, 
		0x42580000, 
		0x00800001, 
		0x7f7ffffe, 
		0x007ffffe, 
		0xff800000, 
		0xa8193f3c, 
		0x5ac9bc1e, 
		0x7f800000, 
		0x80040000, 
		0xabe6c1ad, 
		0x00040000, 
		0xc0a00000, 
		0x0e3fffff, 
		0x42540000, 
		0xad657034, 
		0x42480000, 
		0x0f7ffffe, 
		0xaaaaaaaa, 
		0xbf800000, 
		0x42500000, 
		0x7f800000, 
		0x00400000, 
		0x42500000, 
		0x7f7fffff, 
		0xbf324f79, 
		0x80000000, 
		0x807ffffe, 
		0xeccb089d, 
		0x0c700000, 
		0x00800003, 
		0xc2540000, 
		0x48315e56, 
		0x00800001, 
		0x80011111, 
		0x807fffff, 
		0xb1043294, 
		0xc2100000, 
		0xbf800001, 
		0x1bbf1eba, 
		0x00ffffff, 
		0x80000000, 
		0xa712393d, 
		0x4060dd45, 
		0xfd57a114, 
		0x9a8a24a3, 
		0x00800003, 
		0xcccccccc, 
		0x42180000, 
		0x42080000, 
		0xcb000000, 
		0x80100000, 
		0xa4886aa6, 
		0x0e03ffff, 
		0x8aa62831, 
		0x80100000, 
		0x95540685, 
		0xc27c0000, 
		0xc1100000, 
		0x80000008, 
		0x404f163b, 
		0x00000000, 
		0x0e03ffff, 
		0x7f800000, 
		0x0e001fff, 
		0x46a1a5c7, 
		0xff7fffff, 
		0xacd5cd28, 
		0x0c7fffc0, 
		0x804c9271, 
		0x6d1a659f, 
		0xaa0fb02f, 
		0x00200000, 
		0x00800001, 
		0xcd168df6, 
		0xc2180000, 
		0xff800000, 
		0x0c7fffe0, 
		0xff82cffc, 
		0xc2340000, 
		0x8f7ffffe, 
		0x7f800000, 
		0x42600000, 
		0x7f800000, 
		0xc23c0000, 
		0x70e23a75, 
		0x0c7ff800, 
		0xc22c0000, 
		0xc2640000, 
		0x0946f53d, 
		0x7fc00000, 
		0x7a535828, 
		0x42000000, 
		0x80004000, 
		0x80000000, 
		0x42640000, 
		0x00800003, 
		0x42200000, 
		0x4c62526e, 
		0x80800002, 
		0xc2040000, 
		0x00800001, 
		0x00000000, 
		0x0e1fffff, 
		0x424c0000, 
		0x92b4ee04, 
		0xfe0881ba, 
		0x80000000, 
		0x715df799, 
		0x797ea9d4, 
		0xc2600000, 
		0x0e00ffff, 
		0x42080000, 
		0x42140000, 
		0xff7fffff, 
		0x40000000, 
		0x4e82af76, 
		0x7f7fffff, 
		0x85135c9b, 
		0x0e003fff, 
		0x0c780000, 
		0x083a1c0e, 
		0x42640000, 
		0x0c7ffffc, 
		0x80000020, 
		0x67458f2f, 
		0x214624c3, 
		0x5224a59f, 
		0xff800000, 
		0xb1b524d6, 
		0xc0a00000, 
		0x0d000ff0, 
		0xff800000, 
		0x0e000001, 
		0xeb28e9cb, 
		0x3e3d6c33, 
		0x00010000, 
		0xc1200000, 
		0xffbaf191, 
		0x80000000, 
		0xc2380000, 
		0xc1a80000, 
		0xe983feea, 
		0xc7f9d9fd, 
		0x0c780000, 
		0x00000000, 
		0x0f7ffffe, 
		0x80000001, 
		0x00100000, 
		0xc0000000, 
		0xc89fa9cb, 
		0x0e007fff, 
		0xc1e80000, 
		0xc2780000, 
		0x00004000, 
		0x65d4cb63, 
		0x00000010, 
		0xaaaaaaaa, 
		0x80800003, 
		0xa9d82c92, 
		0xe85f266b, 
		0x8076a9bb, 
		0x0c7fc000, 
		0x0f7ffffe, 
		0x0e1fffff, 
		0x7f800001, 
		0x42240000, 
		0xbf800000, 
		0x41b80000, 
		0xcf034cdd, 
		0x7f7ffffe, 
		0xf605697f, 
		0x41b80000, 
		0x80800002, 
		0x7fac0b99, 
		0xff7ffffe, 
		0x7f800000, 
		0xc1e00000, 
		0x4e2a635b, 
		0xff800000, 
		0x0e003fff, 
		0x80080000, 
		0xff800000, 
		0xd327c162, 
		0x7f800000, 
		0x7fc00001, 
		0xc0e00000, 
		0xbb5d824d, 
		0xbf028f5c, 
		0xa56c5344, 
		0x80000000, 
		0x0f7ffffc, 
		0x0e0fffff, 
		0x0e000fff, 
		0x68741e62, 
		0x0c7c0000, 
		0x80002000, 
		0x0c600000, 
		0x7f7ffffe, 
		0x21dc7e83, 
		0xff7ffffe, 
		0x717a813b, 
		0x00011111, 
		0x00000000, 
		0x80000000, 
		0x7f7fffff, 
		0xc0000000, 
		0xc2280000, 
		0x80515015, 
		0xb06c5fd0, 
		0x00040000, 
		0x807fffff, 
		0x596a8fa6, 
		0xcb000000, 
		0xff800000, 
		0xffffffff, 
		0x80000000, 
		0x8f7ffffc, 
		0xc1600000, 
		0x40a00000, 
		0xac8d8b60, 
		0xc0400000, 
		0x00000000, 
		0x00100000, 
		0xc2200000, 
		0x41a80000, 
		0x41100000, 
		0x80000100, 
		0xc6d7cadd, 
		0x591a00fd, 
		0xf227be6f, 
		0x0c7ff000, 
		0xc1980000, 
		0x80000800, 
		0xa2b111a2, 
		0x7f7ffffe, 
		0xcccccccc, 
		0x007fffff, 
		0x80020000, 
		0x2adb4fcd, 
		0xffbfffff, 
		0xb036989b, 
		0x42700000, 
		0xc1600000, 
		0x64b7e934, 
		0x0c7f8000, 
		0x80000008, 
		0xffc00000, 
		0x80000002, 
		0x00080000, 
		0x80000400, 
		0xc24c0000, 
		0x41e00000, 
		0x80000001, 
		0xeabb7de5, 
		0x426c0000, 
		0xff800000, 
		0xdb9c1ee3, 
		0xa222f5be, 
		0x80000010, 
		0xbe4410f0, 
		0x9f3aef25, 
		0x7f800001, 
		0xff6a71fc, 
		0x143214bc, 
		0x80000000, 
		0xc0400000, 
		0x80800000, 
		0x0e000001, 
		0x00400000, 
		0x41000000, 
		0x80000000, 
		0x42540000, 
		0x80080000, 
		0x6df1f096, 
		0xc20c0000, 
		0x0e03ffff, 
		0x0c7fffe0, 
		0x80001000, 
		0xff800000, 
		0x61a6f8f1, 
		0x42140000, 
		0x41880000, 
		0x6710e348, 
		0x00008000, 
		0x7fb240d9, 
		0xff800000, 
		0x00004000, 
		0x16bbb4bc, 
		0x80800002, 
		0x0c7ffc00, 
		0x0e000003, 
		0x00000400, 
		0xc22c0000, 
		0x8f7fffff, 
		0x8f7ffffd, 
		0x00002000, 
		0x99a9b661, 
		0x7f889432, 
		0x80000100, 
		0x0f7fffff, 
		0xc23c0000, 
		0x0f7ffffd, 
		0x41800000, 
		0x6b4dc9da, 
		0xbf800001, 
		0x80000000, 
		0x235f5328, 
		0x2ac0f583, 
		0xc2600000, 
		0xc2100000, 
		0x80000004, 
		0x00000004, 
		0x4057a66d, 
		0x80000001, 
		0x1951073a, 
		0x3f800001, 
		0x80000000, 
		0x07b84ba4, 
		0xc25c0000, 
		0x9c081eae, 
		0x80800001, 
		0x41200000, 
		0x0c7ffc00, 
		0x00000000, 
		0x85c2754b, 
		0x945c9114, 
		0xc1400000, 
		0xff9ac818, 
		0x80000000, 
		0x94489ded, 
		0xc1800000, 
		0x0c7ffc00, 
		0xc1100000, 
		0xc2200000, 
		0xff99cada, 
		0x420c0000, 
		0x90a5933a, 
		0x41f80000, 
		0x424c0000, 
		0x80000000, 
		0xe0b4bbc6, 
		0xc1c00000, 
		0x40800000, 
		0x41700000, 
		0x80000002, 
		0x80100000, 
		0xc2680000, 
		0x8f7fffff, 
		0x428f9543, 
		0xc2100000, 
		0x7f800000, 
		0xadffa0cb, 
		0x00000010, 
		0x41c00000, 
		0xfcbe9c23, 
		0x80000040, 
		0x80800002, 
		0x42380000, 
		0x41a80000, 
		0x40c00000, 
		0xff7ffffe, 
		0xc2480000, 
		0x6ea2248d, 
		0xd0756e07, 
		0x80472e56, 
		0x2988ae48, 
		0x807fffff, 
		0x0c7ffe00, 
		0x42180000, 
		0x2e9e1f35, 
		0x41a80000, 
		0x28745cb7, 
		0x80656e9f, 
		0xf57865a9, 
		0xfeb30026, 
		0x42580000, 
		0x07716968, 
		0xc1880000, 
		0x00800003, 
		0x52d6ab6d, 
		0xffae9521, 
		0xb623626c, 
		0x8f4c522f, 
		0x0f7ffffe, 
		0x42580000, 
		0x0b1bae48, 
		0x3f800001, 
		0x6d47ae09, 
		0x00000000, 
		0x7fc00001, 
		0x0f7ffffe, 
		0x00800003, 
		0x41c80000, 
		0x349318c6, 
		0x0655a22a, 
		0xc1800000, 
		0xc21c0000, 
		0x0c7fffc0, 
		0x4b8c4b40, 
		0x7f800000, 
		0x90dedf66, 
		0x0c7ffffc, 
		0x00000002, 
		0xcb000000, 
		0xe8326a8a, 
		0x00004000, 
		0x41f00000, 
		0x0c7fff80, 
		0x0c7f8000, 
		0x0e1fffff, 
		0x80008000, 
		0x80000000, 
		0xbac6e3cd, 
		0x80000002, 
		0x80000080, 
		0x00080000, 
		0x80020000, 
		0x7f800000, 
		0x00000001, 
		0x0e01ffff, 
		0x3f028f5c, 
		0x568ada13, 
		0xff800000, 
		0x7fc00000, 
		0xc2340000, 
		0x00000000, 
		0x41100000, 
		0x42040000, 
		0x007fffff, 
		0x00000001, 
		0x80000000, 
		0x0c7ffffc, 
		0x41400000, 
		0x41a00000, 
		0xc0c00000, 
		0x00000000, 
		0x7f800001, 
		0x1e28c425, 
		0xff800000, 
		0x8f7ffffd, 
		0x80000001, 
		0x00000008, 
		0x00040000, 
		0x0e03ffff, 
		0x0db5f9c7, 
		0xf9b4219f, 
		0x84ac3365, 
		0x42340000, 
		0xc1f80000, 
		0xffc00001, 
		0x80000000, 
		0x203cd22c, 
		0x77396333, 
		0x80001000, 
		0x42680000, 
		0x41700000, 
		0xb2612006, 
		0xc2440000, 
		0x7f800000, 
		0xff800000, 
		0xc24c0000, 
		0x0c7fffff, 
		0xe451ca80, 
		0xedd8b216, 
		0xc1c80000, 
		0x41980000, 
		0x42600000, 
		0x90c594de, 
		0x00800001, 
		0xc77dba07, 
		0x40000000, 
		0x43604d8d, 
		0xc0e00000, 
		0x7f800000, 
		0x40000000, 
		0xffa6f628, 
		0xc2080000, 
		0xa69547c7, 
		0x81630f5a, 
		0xc1c80000, 
		0x2337a1ac, 
		0xdea7ae98, 
		0x42180000, 
		0x0e0003ff, 
		0x0e00001f, 
		0xdf92541b, 
		0x0e7fffff, 
		0x7f800000, 
		0xc2740000, 
		0x0e1fffff, 
		0xff800000, 
		0x42600000, 
		0xce5bf90f, 
		0xc2140000, 
		0x00000000, 
		0x7f9d7aed, 
		0x0e03ffff, 
		0x40c00000, 
		0x01daf5fc, 
		0x653a834a, 
		0x80000000, 
		0x0c7f0000, 
		0x4878d24f, 
		0xc2440000, 
		0x0e0000ff, 
		0xc2740000, 
		0xb7031e04, 
		0x0e003fff, 
		0x193a0224, 
		0x41c80000, 
		0x80011111, 
		0xb93c2aef, 
		0x8005eb01, 
		0xd4c13480, 
		0x40800000, 
		0x80000000, 
		0x00800000, 
		0x00800003, 
		0x41e80000, 
		0xc2240000, 
		0x00000100, 
		0x1245545c, 
		0xbf35c3d2, 
		0x42700000, 
		0x0c7ffc00, 
		0x00800001, 
		0x4b000000, 
		0x00000010, 
		0x0c7ffff8, 
		0xff7ffffe, 
		0xc2300000, 
		0x349abbdf, 
		0xea6eb1a8, 
		0xc1900000, 
		0x41600000, 
		0xffbfffff, 
		0x7fc00001, 
		0x0e7fffff, 
		0x4645eb24, 
		0x00000000, 
		0xd8e5fb57, 
		0x40800000, 
		0xff800000, 
		0x42000000, 
		0x0e000fff, 
		0x0c7fffe0, 
		0xd6c6fed3, 
		0x0c7e0000, 
		0x41a00000, 
		0x103c4f34, 
		0xfb9dba62, 
		0xc20c0000, 
		0xc1880000, 
		0x00800000, 
		0x00000008, 
		0x80000000, 
		0xaaaaaaaa, 
		0x42480000, 
		0x2c71c27a, 
		0xd51d8a69, 
		0xc2680000, 
		0x0f7fffff, 
		0xc2080000, 
		0xc0c00000, 
		0x41e80000, 
		0x42500000, 
		0x00000000, 
		0x238a61b2, 
		0x0c7e0000, 
		0x0054f8ac, 
		0x0e4b0b26, 
		0x80000000, 
		0x4d353bec, 
		0x2513f34b, 
		0x01c2b816, 
		0xc1000000, 
		0xc2380000, 
		0x3043f850, 
		0x80100000, 
		0x00000080, 
		0x00000004, 
		0x41a00000, 
		0x41c00000, 
		0x80000000, 
		0x0c7ffffc, 
		0x80200000, 
		0x00800001, 
		0x80040000, 
		0xc2640000, 
		0x41b80000, 
		0xf847ca35, 
		0x806f1706, 
		0x80000000, 
		0x00000001, 
		0x80001000, 
		0xc2240000, 
		0xbf800001, 
		0xfc3ed63e, 
		0x0c700000, 
		0x00000000, 
		0x0c7fffff, 
		0x421c0000, 
		0x42700000, 
		0xf0c18ae1, 
		0xff800000, 
		0xc0400000, 
		0x4800001b, 
		0x64af10b4, 
		0x00000000, 
		0xff7ffffe, 
		0x41c80000, 
		0x4d85dc1f, 
		0x0f7ffffe, 
		0x42140000, 
		0x80800001, 
		0xbcebdf6f, 
		0x80000000, 
		0x007fffff, 
		0x0e0fffff, 
		0x80800001, 
		0x14db9181, 
		0x69923c6b, 
		0x0f7ffffc, 
		0x41d00000, 
		0x42100000, 
		0xff800000, 
		0x42080000, 
		0x42200000, 
		0x80000100, 
		0x80001000, 
		0x0c7fff80, 
		0x3880869e, 
		0x0126c6f2, 
		0x00800002, 
		0x4574bd48, 
		0xc2480000, 
		0x00000001, 
		0x8f7ffffd, 
		0x41000000, 
		0x135f55e7, 
		0x4d80e2ee, 
		0x423c0000, 
		0x80000000, 
		0xc1500000, 
		0x0e00000f, 
		0x0e0003ff, 
		0x7f88ab0c, 
		0x7f800000, 
		0x41a00000, 
		0xc1800000, 
		0x0c7ffe00, 
		0x8f7ffffe, 
		0x0c780000, 
		0x7c377dc1, 
		0x0053df3d, 
		0xc2000000, 
		0x0c7f0000, 
		0x0c600000, 
		0x00ffffff, 
		0x791a6944, 
		0x7fc00000, 
		0x0e03ffff, 
		0x0f7ffffd, 
		0x420c0000, 
		0x8f7ffffd, 
		0x0e00ffff, 
		0x00400000, 
		0x861a669f, 
		0xbf800001, 
		0x80000000, 
		0x0c7f8000, 
		0x146fc777, 
		0x42380000, 
		0x41880000, 
		0xbea05dc6, 
		0x80000000, 
		0x0e000001, 
		0x80800002, 
		0xf73728a7, 
		0xc0c00000, 
		0xc2140000, 
		0x7f800000, 
		0x80000040, 
		0xdcf885a2, 
		0x00800001, 
		0xff800000, 
		0x0c7fff00, 
		0xc2480000, 
		0xc1300000, 
		0x8da4fe14, 
		0x42040000, 
		0xc0c00000, 
		0x00345692, 
		0xf6b9b985, 
		0x00800003, 
		0x42580000, 
		0x0c400000, 
		0x811f0c7c, 
		0x00100000, 
		0x00040000, 
		0x00200000, 
		0x80000000, 
		0x89b4e66b, 
		0x80000000, 
		0xc21c0000, 
		0xc1300000, 
		0xff7fffff, 
		0x42180000, 
		0xff92d7d0, 
		0xff7fffff, 
		0x80800001, 
		0x80000020, 
		0xc2580000, 
		0x3f800000, 
		0xc6bf369c, 
		0x094e07dc, 
		0x80000001, 
		0x00000800, 
		0x0c700000, 
		0x42780000, 
		0xc0800000, 
		0x8f7970db, 
		0xff800000, 
		0x7f7fffff, 
		0x89a29242, 
		0xaa7f78e6, 
		0xff800000, 
		0x7a95dff8, 
		0xc1e80000, 
		0x69e5e7af, 
		0xc1e00000, 
		0x80200000, 
		0x0e00ffff, 
		0xb2d286f9, 
		0x00004000, 
		0x8e77b14b, 
		0x8f7ffffe, 
		0x425c0000, 
		0x80000080, 
		0xc2580000, 
		0xcb000000, 
		0xc0c00000, 
		0xffae8900, 
		0x420c0000, 
		0x0e00000f, 
		0x41d80000, 
		0x76aee1eb, 
		0x1c224041, 
		0xc1b00000, 
		0x00000001, 
		0x7f800000, 
		0x5a33a9d7, 
		0x000e0762, 
		0xd37baf6e, 
		0x80800001, 
		0x72cad624, 
		0x48a5ea1c, 
		0xff7fffff, 
		0x863aff50, 
		0x86d655e4, 
		0x81d4ba89, 
		0x00000000, 
		0x41a00000, 
		0x43780ad6, 
		0x00800000, 
		0x8071cdec, 
		0x0e003fff, 
		0xc2600000, 
		0xaaaaaaaa, 
		0x78409ef0, 
		0xff7fffff, 
		0x0f7ffffc, 
		0x7fc00001, 
		0x8bd979c5, 
		0x80004000, 
		0xda051f44, 
		0xc2140000, 
		0xc1c80000, 
		0x80000000, 
		0x7f800000, 
		0xc1100000, 
		0xc1500000, 
		0xff7ffffe, 
		0x494b8c40, 
		0x80800001, 
		0x00800001, 
		0x80000002, 
		0x00800003, 
		0xc2d74f7a, 
		0x80000040, 
		0x0b58c0c6, 
		0x0c780000, 
		0xc2440000, 
		0x80000000, 
		0x80000000, 
		0x7f800000, 
		0x4a93bfdf, 
		0xd411e3ea, 
		0x0f7ffffe, 
		0xc331fa73, 
		0x42500000, 
		0xff800000, 
		0xff000000, 
		0x80010000, 
		0x00000000, 
		0x67daa5d7, 
		0x80010000, 
		0xe6cb35a4, 
		0x80004000, 
		0x41100000, 
		0x4d415c76, 
		0x8ef04641, 
		0x0e7fffff, 
		0x7f7ffffe, 
		0x80000008, 
		0x0c7ffffc, 
		0x4e860777, 
		0x80800001, 
		0x7f800000, 
		0x00020000, 
		0x00000000, 
		0xff000000, 
		0xc0a00000, 
		0x42640000, 
		0x41e80000, 
		0x8f7ffffd, 
		0x80000200, 
		0x00000000, 
		0x807fffff, 
		0xc2140000, 
		0x00100000, 
		0xc2700000, 
		0xc2740000, 
		0x4b000000, 
		0xff800000, 
		0x80000000, 
		0x5adc406e, 
		0xe3942d65, 
		0x807fffff, 
		0x41000000, 
		0x7f941d29, 
		0x080537e6, 
		0xc1100000, 
		0x00000040, 
		0x80100000, 
		0x02363c1b, 
		0xbb76e6bf, 
		0x5203e411, 
		0x0e00ffff, 
		0x41e80000, 
		0x00000004, 
		0xc1700000, 
		0xcf7e9a0b, 
		0xafeabb1d, 
		0x42542623, 
		0x7f7fffff, 
		0x0c7fe000, 
		0xff7ffffe, 
		0x0c7fffe0, 
		0xc1b00000, 
		0x80000001, 
		0xff7ffffe, 
		0x5dcd6815, 
		0x42100000, 
		0x00800003, 
		0xc1d00000, 
		0x5a9019f5, 
		0xc1880000, 
		0x420c0000, 
		0xc2440000, 
		0xa3ccf620, 
		0x0c7ff800, 
		0xdb048ea1, 
		0x80000200, 
		0xc1300000, 
		0x8f7ffffe, 
		0x00020000, 
		0x004d1ab9, 
		0x1cef18e2, 
		0x1ebe0a34, 
		0x7f7fffff, 
		0x0d000ff0, 
		0xf44d1452, 
		0x420c0000, 
		0x00000002, 
		0x801910f5, 
		0x80000000, 
		0x41c80000, 
		0x80000080, 
		0xc1880000, 
		0x41d00000, 
		0x0c400000, 
		0xc1a00000, 
		0x4b000000, 
		0x651caa25, 
		0x7f7fffff, 
		0xff7fffff, 
		0x780ca850, 
		0x0e0001ff, 
		0x42700000, 
		0xc22c0000, 
		0x00000100, 
		0xff8b3cc0, 
		0x651eeca7, 
		0x02677c20, 
		0xff000000, 
		0x39382281, 
		0x4b000000, 
		0x7fc00001, 
		0xa1d541ea, 
		0x00200000, 
		0x69e879ad, 
		0x41100000, 
		0x9907f7f9, 
		0x0d000ff0, 
		0x80000002, 
		0x0e0003ff, 
		0x90f82e7b, 
		0x007fffff, 
		0xc2780000, 
		0x00000004, 
		0xc1900000, 
		0x12f1fbc8, 
		0x41f00000, 
		0x0e0003ff, 
		0x00000004, 
		0x37b7d2fe, 
		0x819d95df, 
		0x5ea9914f, 
		0x80800003, 
		0x6eff72b6, 
		0x00200000, 
		0x80000020, 
		0x80000001, 
		0xc2180000, 
		0xffffffff, 
		0xc1500000, 
		0x41f80000, 
		0x00000010, 
		0xff800000, 
		0xfb27f2a8, 
		0x80000004, 
		0x7f800001, 
		0xbb1b785a, 
		0x0cae7af0, 
		0xc21c0000, 
		0x943f67e3, 
		0x40e00000, 
		0x6dff3e6f, 
		0xa436f3c0, 
		0xc2500000, 
		0xc1800000, 
		0x00ffffff, 
		0x425c0000, 
		0x7f800000, 
		0xaaaaaaaa, 
		0xa9c399a7, 
		0x422c0000, 
		0x7fc00001, 
		0x80000000, 
		0x3f028f5c, 
		0x0c7fffff, 
		0x7f7fffff, 
		0xc2400000, 
		0x42300000, 
		0x0c780000, 
		0x6f4290ac, 
		0x80000001, 
		0x00011111, 
		0x00000400, 
		0xc2440000, 
		0xff800000, 
		0x1b51591e, 
		0x00000400, 
		0x00000000, 
		0x41400000, 
		0x7f8a673c, 
		0x42580000, 
		0x8f7ffffe, 
		0xdba63979, 
		0x42380000, 
		0x00000001, 
		0x7f923298, 
		0xc0400000, 
		0xff89b287, 
		0x426c0000, 
		0x00515b5f, 
		0xff000000, 
		0xc2480000, 
		0x0e001fff, 
		0xc1b80000, 
		0x00000000, 
		0x42680000, 
		0x80800001, 
		0x80000400, 
		0x80800002, 
		0xc0f7d7fa, 
		0xcd40e130, 
		0x41400000, 
		0xc1900000, 
		0x8041e913, 
		0x41e00000, 
		0x42580000, 
		0x8f2ca9e5, 
		0x318e8d97, 
		0x44791ea0, 
		0xff7fffff, 
		0x41800000, 
		0x16b91e9c, 
		0xe8eec102, 
		0xc27c0000, 
		0x7fc00000, 
		0x0e00007f, 
		0x0e00ffff, 
		0x736d98fd, 
		0xd94b7b1a, 
		0xff2ee8ab, 
		0x7fc00001, 
		0xff800000, 
		0x0c700000, 
		0x42340000, 
		0x298e1632, 
		0x63db9bbc, 
		0x3292cea7, 
		0x42480000, 
		0xbb8d8e1d, 
		0x00000000, 
		0xdd71e430, 
		0x423c0000, 
		0x0f7ffffd, 
		0x7f800000, 
		0x1d846ff9, 
		0x4b8c4b40, 
		0x0e00ffff, 
		0x00000008, 
		0x32917cda, 
		0x80000008, 
		0x0e007fff, 
		0x424c0000, 
		0xc2400000, 
		0xb23fbf68, 
		0x0c7fff80, 
		0x41300000, 
		0xcccccccc, 
		0x3f2e2a3b, 
		0x80000200, 
		0xffc00000, 
		0xc1100000, 
		0x61bbe844, 
		0x00000080, 
		0xff800000, 
		0xa66bc533, 
		0xbea10367, 
		0x46203f07, 
		0xc2640000, 
		0x3c21185f, 
		0x0f7ffffc, 
		0x41a00000, 
		0x0555c88f, 
		0x7994dfba, 
		0x54474c2f, 
		0xc0000000, 
		0xc2580000, 
		0x80800003, 
		0x0c7ffc00, 
		0xc2340000, 
		0x257dad81, 
		0xaaaaaaaa, 
		0xf6df7c58, 
		0xbf028f5c, 
		0xc2540000, 
		0x80000040, 
		0x80000000, 
		0x00000000, 
		0x8f7fffff, 
		0x5739a7f1, 
		0x42500000, 
		0x41d00000, 
		0x80000000, 
		0x3f028f5c, 
		0x40000000, 
		0x00800002, 
		0xc1100000, 
		0x00000001, 
		0xec607e7a, 
		0x0c7fffff, 
		0x0e001fff, 
		0x286e5dbd, 
		0xff800000, 
		0x00000002, 
		0x0d00fff0, 
		0xbf800001, 
		0xff000000, 
		0x42280000, 
		0x40a00000, 
		0x61e9fc1d, 
		0x00257d0f, 
		0x0c7ffc00, 
		0x42600000, 
		0x420c0000, 
		0x80000100, 
		0x00200000, 
		0x00800001, 
		0x421c0000, 
		0x42540000, 
		0x258c9487, 
		0x0e0007ff, 
		0xff800001, 
		0x422c0000, 
		0x45634867, 
		0x96d9def1, 
		0x80800001, 
		0x41d80000, 
		0xdc07ed23, 
		0x7f800000, 
		0xa9372f08, 
		0x00000002, 
		0x80800003, 
		0x80000000, 
		0x45115eba, 
		0xf7d7e0be, 
		0xc0a00000, 
		0x8f7fffff, 
		0x41d80000, 
		0xc2700000, 
		0xc1700000, 
		0x42280000, 
		0xffc00000, 
		0x80000001, 
		0x80000001, 
		0x80020000, 
		0xff800000, 
		0xaaaaaaaa, 
		0x80004000, 
		0xffffffff, 
		0xc26c0000, 
		0xc1900000, 
		0x80000000, 
		0xc2480000, 
		0xc1c80000, 
		0x80000100, 
		0xc2740000, 
		0x00001000, 
		0xc2440000, 
		0xf9c291a2, 
		0x807ffffe, 
		0x0e00001f, 
		0x0c7c0000, 
		0x0e001fff, 
		0x42680000, 
		0x00293f22, 
		0x0c7e0000, 
		0x933162c6, 
		0x423c0000, 
		0x48db607c, 
		0x80002000, 
		0xece066f7, 
		0x00000200, 
		0x7fbfffff, 
		0x00000277, 
		0x00010000, 
		0x9e07d59a, 
		0x80011111, 
		0xb555a5e7, 
		0xff7ffffe, 
		0x7f7fffff, 
		0x80001000, 
		0x3fca44d6, 
		0x007fffff, 
		0x8c838d06, 
		0x00400000, 
		0xc2440000, 
		0x007fffff, 
		0x0c7fc000, 
		0x0e0003ff, 
		0x41a80000, 
		0x0c7ffffe, 
		0x40a00000, 
		0xff800000, 
		0xc0a00000, 
		0x7f800000, 
		0xe1420f98, 
		0xffbc21c7, 
		0x00000100, 
		0x661a9e70, 
		0xe23e0335, 
		0x42480000, 
		0x424c0000, 
		0x8e0439bb, 
		0x42080000, 
		0x003ea3b2, 
		0xffadb1c7, 
		0x4b3e6fa3, 
		0x8029e255, 
		0xc2780000, 
		0x0c7ffff8, 
		0xffc00001, 
		0x4c03518a, 
		0xc2100000, 
		0x80000100, 
		0x3ee8f08f, 
		0xa7774bcf, 
		0x80000000, 
		0xc1d80000, 
		0x426c0000, 
		0x0c7fff80, 
		0x7fc00001, 
		0xc2040000, 
		0xff7fffff, 
		0xd37b445a, 
		0x74e37226, 
		0xff7ffffe, 
		0x80002000, 
		0x90773a0d, 
		0xff800000, 
		0xa317eb79, 
		0x00000001, 
		0x3c3ff74e, 
		0x1b9010af, 
		0x42200000, 
		0x80000001, 
		0x498927ad, 
		0x80000000, 
		0xd2847cb6, 
		0x7fc00001, 
		0xfc4d4d08, 
		0x80000000, 
		0x10345f4e, 
		0x6adb5ea1, 
		0xc0a00000, 
		0x6f4dbb50, 
		0xc1800000, 
		0xff7fffff, 
		0x80000000, 
		0x0f7ffffd, 
		0xe2562b44, 
		0xb8b3964c, 
		0xebeabe2f, 
		0xc1500000, 
		0x41d80000, 
		0x41a00000, 
		0xc2340000, 
		0x80000000, 
		0xc1600000, 
		0x002ffbd7, 
		0x7fc61a8c, 
		0xff7ffffe, 
		0x80000000, 
		0x00000000, 
		0x64db68d5, 
		0x00200000, 
		0xd4c06b1e, 
		0xc23c0000, 
		0x00001000, 
		0xc2080000, 
		0x6e2538ea, 
		0x41880000, 
		0x80008000, 
		0xcb8c4b40, 
		0x00000002, 
		0x7dde6920, 
		0xde49825a, 
		0x42340000, 
		0x80000800, 
		0x00800002, 
		0x80000000, 
		0x7f800000, 
		0x625079df, 
		0xd42bec81, 
		0xcccccccc, 
		0x80800002, 
		0xc1900000, 
		0x00800001, 
		0x7a4f03f0, 
		0x80134b7e, 
		0x9e54bdeb, 
		0x80000001, 
		0x007ffffe, 
		0x024e49b8, 
		0xdae813ed, 
		0x484199e6, 
		0x41d00000, 
		0x80000000, 
		0xc1e80000, 
		0x00000004, 
		0x420c0000, 
		0x41700000, 
		0x4c2bb538, 
		0x80000400, 
		0x80080000, 
		0x42580000, 
		0x25d26989, 
		0x0c7fffc0, 
		0x5538cbc9, 
		0xce5b1e3b, 
		0x0c700000, 
		0x67a63203, 
		0xbf800001, 
		0x5bd5ec02, 
		0x0e0001ff, 
		0x5dbd9d1b, 
		0x420c0000, 
		0x00000000, 
		0x0c7fc000, 
		0x80004000, 
		0xcccccccc, 
		0x7e1ab9ca, 
		0x42540000, 
		0x0c7ffc00, 
		0x00000000, 
		0x0e00003f, 
		0x42640000, 
		0x80100000, 
		0x42400000, 
		0x7ca39b8a, 
		0x80080000, 
		0xe079424b, 
		0xcd7df721, 
		0x4ae628c1, 
		0x4de8a6e4, 
		0x0c400000, 
		0x4b000000, 
		0x00800000, 
		0x7f7ffffe, 
		0x00000000, 
		0x00800002, 
		0xaf12dab0, 
		0x80800003, 
		0x48ce8888, 
		0xbf800000, 
		0xc1100000, 
		0x0e000007, 
		0xc1a00000, 
		0xc2380000, 
		0xc1a80000, 
		0x80800003, 
		0x0e03ffff, 
		0xbcf5058e, 
		0x42380000, 
		0x0e000001, 
		0x6590d995, 
		0x66e0ce6b, 
		0x7f800000, 
		0x41400000, 
		0x0c7e0000, 
		0x153047ca, 
		0xc1880000, 
		0x41980000, 
		0x80000200, 
		0x00400000, 
		0xcb000000, 
		0xc91bd024, 
		0x1f324a47, 
		0xc26c0000, 
		0xae71371f, 
		0xc21c0000, 
		0x5f70cb97, 
		0x7fbfffff, 
		0x7f7ffffe, 
		0x00200000, 
		0x41e87136, 
		0xc0000000, 
		0x80008000, 
		0xc2740000, 
		0x80000000, 
		0x0e000001, 
		0x00000004, 
		0x7f88b32e, 
		0xf87656f1, 
		0x41700000, 
		0xc114f0e0, 
		0x0e000003, 
		0x42680000, 
		0x7f800000, 
		0x0e0007ff, 
		0xc3b4057f, 
		0xc2440000, 
		0x7f800000, 
		0xff000000, 
		0x40000000, 
		0x3b9c2e37, 
		0x7678e6a9, 
		0xc57dc7c3, 
		0x80020000, 
		0xff7ffffe, 
		0xc2640000, 
		0x7f800000, 
		0xffffffff, 
		0xc2700000, 
		0x7f800000, 
		0x71ace7c8, 
		0x0c400000, 
		0xc2340000, 
		0x80000000, 
		0x492b4262, 
		0x0e01ffff, 
		0x80000000, 
		0x1fdc0b6c, 
		0x0f7ffffe, 
		0xa7083609, 
		0xc1a00000, 
		0x80800000, 
		0x41980000, 
		0x80008000, 
		0x42640000, 
		0x80000000, 
		0x7f800000, 
		0x42000000, 
		0xd5956e14, 
		0x7fc00000, 
		0x00000000, 
		0x00000001, 
		0x80000000, 
		0x28603296, 
		0xe14966c3, 
		0xbe77fbf1, 
		0x42500000, 
		0x00000004, 
		0x7f800000, 
		0x80800003, 
		0x8d2f8020, 
		0xa9dbb672, 
		0xff7ffffe, 
		0x0e00001f, 
		0x41f80000, 
		0x00000000, 
		0x00800000, 
		0x0e3fffff, 
		0x80800003, 
		0x7f800000, 
		0x0c700000, 
		0xc1b80000, 
		0x420c0000, 
		0x80000008, 
		0xed276de1, 
		0x00000400, 
		0xc2780000, 
		0x5baea798  
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
		0x00000069, 
		0x000000a5, 
		0x00000087, 
		0x0000019d, 
		0x0000026b, 
		0x0000026b, 
		0x00000319, 
		0x00000129, 
		0x000000a9, 
		0x00000073, 
		0x000002f1, 
		0x00000375, 
		0x00000313, 
		0x000001cf, 
		0x0000008f, 
		0x00000323, 
		0x00000327, 
		0x000002cf, 
		0x0000018f, 
		0x000002bd, 
		0x0000026d, 
		0x000000f3, 
		0x00000267, 
		0x00000337, 
		0x000002c1, 
		0x0000016f, 
		0x00000189, 
		0x000002d1, 
		0x0000038d, 
		0x00000331, 
		0x000002f3, 
		0x0000017d, 
		0x0000019d, 
		0x0000015f, 
		0x000001ab, 
		0x0000037d, 
		0x0000022d, 
		0x000001bd, 
		0x00000261, 
		0x000001f1, 
		0x000002b9, 
		0x000000c3, 
		0x00000293, 
		0x0000001b, 
		0x000002a3, 
		0x00000035, 
		0x00000035, 
		0x0000027b, 
		0x00000011, 
		0x0000027b, 
		0x0000026f, 
		0x00000191, 
		0x000002c1, 
		0x000002c9, 
		0x0000026b, 
		0x00000271, 
		0x00000209, 
		0x00000323, 
		0x000003e5, 
		0x00000179, 
		0x00000341, 
		0x0000012b, 
		0x00000043, 
		0x000000f9, 
		0x0000009b, 
		0x0000021b, 
		0x00000081, 
		0x00000069, 
		0x00000173, 
		0x0000020d, 
		0x00000047, 
		0x0000001f, 
		0x00000043, 
		0x0000025d, 
		0x000003f5, 
		0x000001dd, 
		0x000001ff, 
		0x0000024f, 
		0x00000271, 
		0x000002ff, 
		0x0000009f, 
		0x000003cf, 
		0x00000225, 
		0x00000223, 
		0x000000bf, 
		0x0000011b, 
		0x00000241, 
		0x00000081, 
		0x000002fb, 
		0x000001eb, 
		0x000002b9, 
		0x00000389, 
		0x000003d7, 
		0x00000353, 
		0x000002f1, 
		0x00000207, 
		0x00000113, 
		0x000000a7, 
		0x00000243, 
		0x00000183, 
		0x000000e3, 
		0x000002e7, 
		0x00000211, 
		0x00000175, 
		0x000001f3, 
		0x00000377, 
		0x000003dd, 
		0x000003b3, 
		0x0000021f, 
		0x00000355, 
		0x000001d3, 
		0x00000275, 
		0x00000203, 
		0x000002fb, 
		0x00000341, 
		0x0000036f, 
		0x00000067, 
		0x00000297, 
		0x000003d7, 
		0x0000014d, 
		0x00000067, 
		0x000000a9, 
		0x000000ab, 
		0x00000347, 
		0x000000eb, 
		0x00000355, 
		0x00000369, 
		0x000000c1, 
		0x0000030d, 
		0x000002c3, 
		0x000000e1, 
		0x0000022d, 
		0x00000185, 
		0x000002eb, 
		0x00000213, 
		0x0000001f, 
		0x0000006b, 
		0x000001fd, 
		0x00000367, 
		0x00000351, 
		0x000002cf, 
		0x000002a9, 
		0x0000002b, 
		0x0000022d, 
		0x00000299, 
		0x000000ed, 
		0x00000247, 
		0x000003e7, 
		0x000003d5, 
		0x000003fd, 
		0x00000075, 
		0x000002b7, 
		0x00000103, 
		0x0000015d, 
		0x00000013, 
		0x000000a9, 
		0x000001f7, 
		0x0000007f, 
		0x00000115, 
		0x0000012d, 
		0x000001c3, 
		0x000003af, 
		0x00000149, 
		0x000003ed, 
		0x0000010d, 
		0x00000277, 
		0x000000cb, 
		0x0000016b, 
		0x0000038d, 
		0x000002a5, 
		0x000003ff, 
		0x000003ed, 
		0x000000d5, 
		0x00000097, 
		0x0000004f, 
		0x00000383, 
		0x00000187, 
		0x00000295, 
		0x000002ed, 
		0x00000323, 
		0x00000189, 
		0x0000021d, 
		0x00000037, 
		0x000003bb, 
		0x000003e1, 
		0x00000261, 
		0x0000034f, 
		0x00000015, 
		0x00000385, 
		0x000003b1, 
		0x000001f3, 
		0x00000325, 
		0x0000037b, 
		0x00000241, 
		0x000001ad, 
		0x00000273, 
		0x0000038b, 
		0x00000173, 
		0x00000141, 
		0x0000012d, 
		0x0000033d, 
		0x00000117, 
		0x000003a1, 
		0x000002c5, 
		0x000003c7, 
		0x000000ab, 
		0x00000357, 
		0x000003e1, 
		0x000002bd, 
		0x000001a7, 
		0x00000351, 
		0x00000093, 
		0x00000375, 
		0x000001fd, 
		0x00000033, 
		0x000002e9, 
		0x000001af, 
		0x00000177, 
		0x00000213, 
		0x000000b3, 
		0x00000273, 
		0x0000026f, 
		0x000000ff, 
		0x0000034b, 
		0x000003a1, 
		0x00000207, 
		0x000002a1, 
		0x000000f9, 
		0x00000307, 
		0x0000028b, 
		0x00000225, 
		0x000002ff, 
		0x0000020d, 
		0x000003b1, 
		0x000002cd, 
		0x000000e5, 
		0x0000008f, 
		0x000003f7, 
		0x0000030b, 
		0x000003f1, 
		0x000002ed, 
		0x000001f5, 
		0x000000f3, 
		0x0000000d, 
		0x000001db, 
		0x0000015b, 
		0x000003b3, 
		0x00000215, 
		0x0000018d, 
		0x00000393, 
		0x0000003d, 
		0x0000007d, 
		0x000000c3, 
		0x00000367, 
		0x000003c5, 
		0x000001cf  
};

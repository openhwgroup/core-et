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
			"li x5, 298\n"
			"faddi.pi f2, f0, 330\n"
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
			"fmax.ps f30, f27, f18\n"
			"fmax.pi f18, f20, f28\n"
			"fmin.pi f25, f20, f23\n"
			"fmin.ps f19, f28, f24\n"
			"fadd.ps f22, f27, f19\n"
			"fadd.ps f19, f31, f27\n"
			"fadd.pi f21, f28, f20\n"
			"fadd.ps f18, f23, f20\n"
			"fsub.ps f31, f24, f22\n"
			"fexp.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"fexp.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"fnmsub.ps f31, f24, f23, f25\n"
			"fnmsub.ps f28, f24, f20, f31\n"
			"fmadd.ps f22, f31, f20, f20\n"
			"2:\n"
			"fexp.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"bne x0, x0, 2b\n"
			"3:\n"
			"fexp.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"bne x0, x0, 3b\n"
			"fexp.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"fexp.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"fsub.pi f28, f29, f30\n"
			"fmax.pi f28, f25, f24\n"
			"fmax.ps f28, f26, f27\n"
			"fmul.ps f27, f25, f24\n"
			"fadd.pi f26, f29, f30\n"
			"fmax.ps f28, f29, f30\n"
			"fmin.ps f26, f31, f26\n"
			"fmin.ps f26, f30, f30\n"
			"fadd.pi f28, f30, f25\n"
			"fmin.pi f28, f30, f27\n"
			"fexp.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"fnmadd.ps f29, f31, f31, f26\n"
			"fnmsub.ps f29, f30, f30, f31\n"
			"fmsub.ps f27, f31, f28, f26\n"
			"fmsub.ps f30, f30, f31, f27\n"
			"fnmadd.ps f30, f27, f26, f29\n"
			"3:\n"
			"fexp.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"bne x0, x0, 3b\n"
			"fmul.ps f26, f29, f28\n"
			"fand.pi f27, f29, f31\n"
			"fsub.ps f29, f31, f26\n"
			"fmul.ps f28, f26, f28\n"
			"fand.pi f31, f27, f30\n"
			"fmul.ps f31, f29, f31\n"
			"fmin.ps f26, f27, f30\n"
			"fexp.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"fnmadd.ps f30, f31, f28, f27\n"
			"fnmsub.ps f30, f29, f27, f28\n"
			"fnmadd.ps f28, f29, f31, f28\n"
			"fmsub.ps f27, f29, f31, f29\n"
			"fmsub.ps f27, f31, f30, f27\n"
			"fnmadd.ps f30, f30, f28, f27\n"
			"1:\n"
			"fexp.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"bne x0, x0, 1b\n"
			"fmadd.ps f30, f31, f29, f30\n"
			"fnmsub.ps f30, f28, f31, f30\n"
			"fnmsub.ps f30, f29, f31, f30\n"
			"fmadd.ps f29, f30, f30, f31\n"
			"fnmsub.ps f30, f29, f28, f29\n"
			"fmsub.ps f28, f28, f29, f28\n"
			"fmadd.ps f30, f31, f30, f31\n"
			"fnmsub.ps f28, f29, f30, f30\n"
			"fexp.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"fmadd.ps f30, f31, f31, f29\n"
			"fnmadd.ps f29, f30, f31, f31\n"
			"fmsub.ps f30, f30, f30, f29\n"
			"fmadd.ps f29, f31, f29, f30\n"
			"fmadd.ps f31, f29, f29, f30\n"
			"fnmadd.ps f31, f30, f29, f31\n"
			"fexp.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"fexp.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"1:\n"
			"fexp.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"bne x0, x0, 1b\n"
			"fand.pi f13, f29, f9\n"
			"fmul.pi f19, f8, f0\n"
			"fadd.pi f12, f7, f16\n"
			"fmul.pi f11, f26, f31\n"
			"fmin.ps f31, f13, f7\n"
			"fadd.ps f18, f23, f2\n"
			"fsub.pi f28, f19, f23\n"
			"fmul.pi f1, f14, f23\n"
			"fadd.pi f19, f13, f6\n"
			"fmul.pi f3, f27, f18\n"
			"fexp.ps f1, f17\n"
			"faddi.pi f17, f17, 128\n"
			"fmadd.ps f2, f8, f5, f4\n"
			"fnmadd.ps f12, f12, f6, f10\n"
			"fmsub.ps f25, f12, f30, f1\n"
			"fmsub.ps f20, f29, f8, f15\n"
			"fnmadd.ps f2, f11, f7, f15\n"
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
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
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
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
			"li x0, 0x7\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 608(x11)\n"
			"flw.ps f10, 32(x11)\n"
			"fmsub.ps f25, f24, f10, f10, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 800(x11)\n"
			"flw.ps f1, 640(x11)\n"
			"fle.ps f10, f2, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"masknot m0, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f10, 32(x7)\n"
			"flw.ps f1, 480(x7)\n"
			"fsll.pi f2, f10, f1\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 416(x18)\n"
			"flog.ps f4, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 448(x20)\n"
			"flw.ps f15, 704(x20)\n"
			"fltu.pi f9, f11, f15\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 544(x11)\n"
			"flw.ps f1, 32(x11)\n"
			"flw.ps f25, 672(x11)\n"
			"fmadd.s f6, f8, f1, f25, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 640(x7)\n"
			"flw.ps f6, 32(x7)\n"
			"flw.ps f7, 544(x7)\n"
			"fnmadd.ps f28, f9, f6, f7, rmm\n" 
			"li x5, 0x4200\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f24, 128(x10)\n"
			"flw.ps f9, 608(x10)\n"
			"fminu.pi f15, f24, f9\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f8, 224(x27)\n"
			"fcvt.ps.f16 f2, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 0(x22)\n"
			"flw.ps f15, 736(x22)\n"
			"fsub.ps f5, f24, f15, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 800(x27)\n"
			"flw.ps f6, 832(x27)\n"
			"fadd.ps f2, f1, f6, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 256(x18)\n"
			"fsrli.pi f20, f8, 0x9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 32(x10)\n"
			"fsrai.pi f15, f5, 0xf\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"maskor m4, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f1, 864(x12)\n"
			"fsrai.pi f25, f1, 0x3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x81 << TENSOR_FMA32_SCP_LOC_B) | (0x8d << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x59 << TENSOR_FMA16_SCP_LOC_B) | (0xa2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 32(x10)\n"
			"fcvt.w.s x10, f7, rup\n" 
			"maskpopcz x27, m7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 704(x27)\n"
			"flw.ps f28, 352(x27)\n"
			"fdiv.s f28, f25, f28, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 128(x10)\n"
			"flw.ps f20, 352(x10)\n"
			"fsgnjx.ps f2, f6, f20\n" 
			"li x5, 0xa82a3ed18a93af17\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3495de18e3ea496e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x7, 0x6ad07737be3ab0f0\n"
			"fcvt.s.l f24, x7, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 320(x28)\n"
			"flw.ps f1, 448(x28)\n"
			"fle.s x28, f11, f1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 0(x12)\n"
			"fround.ps f28, f1, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 736(x22)\n"
			"flw.ps f6, 384(x22)\n"
			"flt.ps f2, f1, f6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 480(x7)\n"
			"fcvt.f16.ps f7, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 640(x28)\n"
			"flw.ps f28, 0(x28)\n"
			"fand.pi f9, f10, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 320(x7)\n"
			"flw.ps f20, 192(x7)\n"
			"fmin.ps f8, f9, f20\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 704(x11)\n"
			"fexp.ps f8, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 608(x17)\n"
			"flw.ps f5, 64(x17)\n"
			"fmax.s f6, f20, f5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 768(x28)\n"
			"fclass.ps f29, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f4, 32(x20)\n"
			"flw.ps f10, 896(x20)\n"
			"fand.pi f11, f4, f10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_0:\n"
			"flem.ps m0, f11, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x2e2b177a418efe84\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x9f21a253f9c3e5f4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f5, 32(x17)\n"
			"faddi.pi f10, f5, 0x135\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"mov.m.x m7, x28, 0x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 352(x28)\n"
			"flw.ps f25, 928(x28)\n"
			"fsgnj.ps f15, f10, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 640(x17)\n"
			"fcvt.pw.ps f1, f10, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 992(x27)\n"
			"flw.ps f1, 608(x27)\n"
			"fsgnj.s f9, f2, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 480(x11)\n"
			"flw.ps f15, 32(x11)\n"
			"fsgnjx.ps f2, f2, f15\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 640(x17)\n"
			"flw.ps f25, 224(x17)\n"
			"fsgnjx.s f10, f10, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 64(x27)\n"
			"flw.ps f28, 416(x27)\n"
			"fle.s x28, f29, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 384(x10)\n"
			"fround.ps f11, f28, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 416(x17)\n"
			"for.pi f20, f9, f9\n" 
			"mova.x.m x17\n" 
			"la x27, rand_int_32\n"
			"flw.ps f6, 32(x27)\n"
			"flw.ps f11, 864(x27)\n"
			"fmax.pi f28, f6, f11\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 64(x22)\n"
			"flw.ps f10, 32(x22)\n"
			"fxor.pi f6, f20, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_0:\n"
			"maskxor m0, m7, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401B8\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f10, 0(x10)\n"
			"fsrai.pi f20, f10, 0x3\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x4 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x9c << TENSOR_FMA32_SCP_LOC_B) | (0x8e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x92 << TENSOR_FMA32_SCP_LOC_B) | (0xf1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x29 << TENSOR_FMA16_SCP_LOC_B) | (0xbc << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 32(x22)\n"
			"flw.ps f25, 480(x22)\n"
			"flw.ps f2, 544(x22)\n"
			"fmsub.s f2, f4, f25, f2, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 224(x28)\n"
			"flw.ps f5, 224(x28)\n"
			"fsgnjx.ps f10, f11, f5\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 640(x10)\n"
			"flw.ps f6, 384(x10)\n"
			"feq.ps f9, f10, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 576(x28)\n"
			"flw.ps f6, 960(x28)\n"
			"fsgnj.s f11, f4, f6\n" 
			"li x10, 0xe212b48017ac7c46\n"
			"fcvt.s.w f5, x10, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 416(x28)\n"
			"fsrli.pi f1, f20, 0x5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 160(x18)\n"
			"fsrli.pi f1, f24, 0x3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 224(x28)\n"
			"flw.ps f1, 992(x28)\n"
			"fmin.ps f2, f10, f1\n" 
			"fbci.pi f25, 0x1735c\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopcz x7, m7\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_0:\n"
			"mov.m.x m1, x11, 0x7c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_0:\n"
			"maskand m7, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x10, 0x95d7cd36789f59fa\n"
			"fcvt.s.w f29, x10, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 768(x27)\n"
			"fclass.s x17, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 416(x27)\n"
			"flw.ps f1, 928(x27)\n"
			"fadd.ps f6, f10, f1, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 224(x28)\n"
			"flw.ps f4, 480(x28)\n"
			"flw.ps f6, 928(x28)\n"
			"fcmov.ps f2, f29, f4, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xb << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x1 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"LBL_HPM_3_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"mov.m.x m6, x17, 0x39\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f7, 896(x18)\n"
			"flw.ps f19, 512(x18)\n"
			"fmax.pi f8, f7, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskor m5, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f10, 864(x19)\n"
			"fcvt.ps.pwu f20, f10, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 640(x23)\n"
			"flw.ps f29, 320(x23)\n"
			"flw.ps f14, 896(x23)\n"
			"fnmadd.s f8, f11, f29, f14, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 160(x9)\n"
			"frcp.ps f20, f5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 512(x28)\n"
			"flw.ps f25, 544(x28)\n"
			"fmax.pi f7, f20, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_1:\n"
			"maskor m5, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f5, 416(x17)\n"
			"flw.ps f7, 832(x17)\n"
			"fle.pi f19, f5, f7\n" 
			"mova.x.m x19\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 960(x23)\n"
			"fsrli.pi f10, f14, 0xf\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 672(x6)\n"
			"flw.ps f7, 672(x6)\n"
			"fsub.pi f7, f14, f7\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 928(x11)\n"
			"flw.ps f10, 384(x11)\n"
			"fsgnj.s f7, f8, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 992(x19)\n"
			"flw.ps f25, 448(x19)\n"
			"fsgnjn.s f26, f14, f25\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 352(x28)\n"
			"fcvt.f16.ps f19, f25\n" 
			"li x17, 0x2261e19fd36031a1\n"
			"fcvt.s.w f14, x17, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 224(x17)\n"
			"flw.ps f25, 736(x17)\n"
			"fsub.s f26, f22, f25, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f1, 768(x7)\n"
			"fxor.pi f27, f1, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"mov.m.x m4, x23, 0x5b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f14, 960(x23)\n"
			"flw.ps f24, 352(x23)\n"
			"feq.pi f10, f14, f24\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f5, 896(x17)\n"
			"fcvt.ps.f16 f25, f5\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 544(x9)\n"
			"fexp.ps f11, f26\n" 
			"fbci.ps f7, 0x51703\n" 
			"la x7, rand_int_32\n"
			"flw.ps f8, 928(x7)\n"
			"faddi.pi f7, f8, 0xd4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 800(x19)\n"
			"flw.ps f11, 384(x19)\n"
			"flw.ps f26, 896(x19)\n"
			"fnmsub.s f24, f22, f11, f26, rup\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f19, 128(x13)\n"
			"fcvt.ps.f16 f8, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 32(x12)\n"
			"fpackrepb.pi f19, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 192(x18)\n"
			"frsq.ps f8, f25\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 256(x6)\n"
			"ffrc.ps f22, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 96(x9)\n"
			"flw.ps f29, 384(x9)\n"
			"flw.ps f19, 864(x9)\n"
			"fmadd.s f20, f26, f29, f19, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 576(x6)\n"
			"fclass.ps f5, f11\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 256(x13)\n"
			"flw.ps f11, 0(x13)\n"
			"fand.pi f22, f24, f11\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 64(x17)\n"
			"flw.ps f8, 768(x17)\n"
			"flw.ps f20, 544(x17)\n"
			"fmadd.s f25, f14, f8, f20, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 320(x17)\n"
			"flw.ps f14, 480(x17)\n"
			"fsgnjx.s f29, f22, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 480(x9)\n"
			"flw.ps f29, 992(x9)\n"
			"feq.s x13, f26, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"maskand m5, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 896(x13)\n"
			"flw.ps f27, 384(x13)\n"
			"flt.s x19, f29, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 832(x18)\n"
			"fcvt.pw.ps f20, f29, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 416(x13)\n"
			"fsin.ps f1, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 640(x28)\n"
			"flw.ps f11, 544(x28)\n"
			"fxor.pi f7, f20, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 160(x12)\n"
			"flw.ps f8, 512(x12)\n"
			"fmul.s f24, f10, f8, rdn\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_1:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 672(x11)\n"
			"flog.ps f5, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 192(x23)\n"
			"fpackreph.pi f8, f14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 608(x11)\n"
			"fmax.s f19, f9, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_1:\n"
			"mova.m.x x13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f5, 672(x9)\n"
			"flw.ps f19, 512(x9)\n"
			"feq.pi f11, f5, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 768(x19)\n"
			"flw.ps f11, 704(x19)\n"
			"fmax.ps f8, f10, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 512(x9)\n"
			"flw.ps f19, 128(x9)\n"
			"fltu.pi f11, f20, f19\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 736(x19)\n"
			"flw.ps f26, 832(x19)\n"
			"flt.s x7, f1, f26\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 320(x11)\n"
			"fsrli.pi f7, f22, 0x7\n" 
			"li x17, 0x9a78a81900256fe7\n"
			"fcvt.s.lu f27, x17, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 64(x6)\n"
			"flw.ps f24, 416(x6)\n"
			"fmulhu.pi f20, f27, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 416(x7)\n"
			"fround.ps f1, f24, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_1:\n"
			"maskand m6, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_1:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_1:\n"
			"maskor m3, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 320(x11)\n"
			"flw.ps f1, 288(x11)\n"
			"fsgnjn.s f5, f9, f1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 512(x7)\n"
			"flw.ps f5, 256(x7)\n"
			"fdiv.ps f24, f24, f5, rmm\n" 
			"maskpopcz x13, m5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 736(x11)\n"
			"flw.ps f8, 576(x11)\n"
			"fcmovm.ps f10, f5, f8\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 544(x11)\n"
			"fslli.pi f24, f9, 0x2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 800(x6)\n"
			"flw.ps f10, 192(x6)\n"
			"fand.pi f7, f27, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 576(x19)\n"
			"flw.ps f20, 576(x19)\n"
			"flt.pi f5, f9, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 928(x28)\n"
			"flw.ps f27, 64(x28)\n"
			"fmul.ps f9, f9, f27, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 160(x16)\n"
			"flw.ps f12, 608(x16)\n"
			"fsgnjx.ps f23, f18, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 160(x5)\n"
			"flw.ps f10, 704(x5)\n"
			"fsgnjn.s f6, f26, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 640(x16)\n"
			"flw.ps f13, 640(x16)\n"
			"flt.ps f20, f11, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 128(x16)\n"
			"flw.ps f3, 832(x16)\n"
			"fsgnjn.s f20, f17, f3\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 416(x18)\n"
			"flw.ps f17, 32(x18)\n"
			"fsgnjx.ps f10, f20, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 992(x11)\n"
			"flw.ps f9, 704(x11)\n"
			"flw.ps f17, 928(x11)\n"
			"fnmsub.s f31, f20, f9, f17, rtz\n" 
			"li x16, 0xcb9689a69363f58b\n"
			"fcvt.s.w f20, x16, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.ps m5, f24, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f17, 256(x5)\n"
			"fandi.pi f3, f17, 0x1e5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 352(x20)\n"
			"fsrli.pi f3, f24, 0xf\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 384(x28)\n"
			"fcvt.wu.s x14, f11, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_2:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 544(x28)\n"
			"frcp.ps f3, f17\n" 
			"maskpopcz x20, m7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 0(x18)\n"
			"flw.ps f13, 544(x18)\n"
			"fsgnj.ps f10, f26, f13\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 416(x17)\n"
			"flw.ps f9, 576(x17)\n"
			"fsrl.pi f22, f17, f9\n" 
			"maskpopc x14, m7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x3 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x3 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1c << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xd << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1a\n"
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
			"la x18, rand_int_32\n"
			"flw.ps f13, 288(x18)\n"
			"flw.ps f18, 352(x18)\n"
			"fle.pi f13, f13, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 288(x5)\n"
			"flw.ps f22, 448(x5)\n"
			"fcmovm.ps f26, f17, f22\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 192(x18)\n"
			"fsrai.pi f24, f20, 0xc\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 544(x9)\n"
			"fcvt.wu.s x20, f26, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 224(x17)\n"
			"flw.ps f13, 960(x17)\n"
			"fmul.pi f12, f3, f13\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 960(x18)\n"
			"fclass.s x17, f18\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 384(x20)\n"
			"fsatu8.pi f11, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 736(x5)\n"
			"flw.ps f18, 640(x5)\n"
			"fnmadd.ps f10, f6, f18, f18, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"flem.ps m7, f13, f13\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x919565cbc9\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"mova.m.x x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f22, 480(x18)\n"
			"flw.ps f23, 928(x18)\n"
			"feq.pi f12, f22, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f26, 64(x11)\n"
			"flw.ps f20, 832(x11)\n"
			"fmulh.pi f18, f26, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 608(x28)\n"
			"fpackreph.pi f26, f23\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 960(x16)\n"
			"flw.ps f20, 672(x16)\n"
			"fdiv.s f20, f12, f20, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fbci.pi f1, 0x6d37e\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 768(x7)\n"
			"fnot.pi f18, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_502135_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x30\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xb << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f3, 96(x28)\n"
			"flw.ps f1, 256(x28)\n"
			"feq.pi f1, f3, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 736(x7)\n"
			"flw.ps f17, 960(x7)\n"
			"fsub.s f13, f12, f17, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 672(x9)\n"
			"fsrli.pi f6, f24, 0xe\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 768(x7)\n"
			"flw.ps f6, 96(x7)\n"
			"fltu.pi f1, f12, f6\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 352(x18)\n"
			"flw.ps f20, 800(x18)\n"
			"fsgnjn.ps f13, f13, f20\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 544(x9)\n"
			"flw.ps f13, 704(x9)\n"
			"fsra.pi f17, f18, f13\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_2:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 512(x18)\n"
			"frcp.ps f18, f12\n" 
			"li x18, 0x9c2f39feed9a99d0\n"
			"fcvt.s.wu f3, x18, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 160(x17)\n"
			"flw.ps f1, 832(x17)\n"
			"fmul.s f10, f26, f1, rdn\n" 
			"fbci.pi f17, 0x65bef\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 384(x5)\n"
			"flw.ps f31, 608(x5)\n"
			"fmax.pi f3, f17, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_2:\n"
			"maskor m4, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 224(x18)\n"
			"flw.ps f18, 288(x18)\n"
			"fmin.s f13, f31, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 352(x6)\n"
			"flw.ps f12, 736(x6)\n"
			"flw.ps f9, 128(x6)\n"
			"fnmadd.s f17, f31, f12, f9, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 448(x7)\n"
			"flw.ps f1, 0(x7)\n"
			"fand.pi f11, f11, f1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 448(x9)\n"
			"ffrc.ps f24, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x0 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x2 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x19 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 128(x28)\n"
			"frsq.ps f22, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 768(x5)\n"
			"fclass.ps f10, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 256(x28)\n"
			"fround.ps f20, f23, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 480(x16)\n"
			"flw.ps f20, 32(x16)\n"
			"fsra.pi f6, f10, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f9, 192(x11)\n"
			"flw.ps f31, 256(x11)\n"
			"fltu.pi f6, f9, f31\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_2:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 96(x7)\n"
			"frcp.ps f6, f1\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 256(x5)\n"
			"fnot.pi f23, f12\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_2:\n"
			"fltm.ps m7, f1, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xcfd0848bd9\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 480(x18)\n"
			"flw.ps f20, 128(x18)\n"
			"fmsub.ps f3, f26, f26, f20, rmm\n" 
			"maskpopcz x6, m2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 800(x18)\n"
			"flw.ps f13, 480(x18)\n"
			"fmax.ps f24, f22, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 832(x28)\n"
			"fsqrt.s f1, f18, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 192(x20)\n"
			"flw.ps f22, 288(x20)\n"
			"fle.ps f24, f23, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 160(x14)\n"
			"flw.ps f13, 672(x14)\n"
			"fmax.s f24, f10, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 0(x28)\n"
			"fcvt.pwu.ps f23, f26, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 672(x17)\n"
			"flw.ps f23, 896(x17)\n"
			"fsub.ps f6, f3, f23, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_947288_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x4 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x2 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_520527_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x5 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xa << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_465038_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x7 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x9 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x3 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x0 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xf << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x4 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x8 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1d << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x0 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f29, 96(x10)\n"
			"flw.ps f25, 640(x10)\n"
			"fmaxu.pi f19, f29, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"feqm.ps m2, f19, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f11, 32(x10)\n"
			"fslli.pi f21, f11, 0xa\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskor m1, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f6, 896(x25)\n"
			"for.pi f17, f6, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 640(x5)\n"
			"flw.ps f11, 928(x5)\n"
			"feq.s x25, f25, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 992(x10)\n"
			"flw.ps f17, 384(x10)\n"
			"flw.ps f13, 544(x10)\n"
			"fmadd.s f5, f29, f17, f13, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_3:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 704(x26)\n"
			"frcp.ps f26, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 32(x19)\n"
			"flw.ps f30, 640(x19)\n"
			"fsgnjx.ps f13, f0, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 224(x10)\n"
			"fsat8.pi f28, f6\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 832(x17)\n"
			"flw.ps f2, 192(x17)\n"
			"fsll.pi f28, f28, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 448(x17)\n"
			"flw.ps f2, 736(x17)\n"
			"flw.ps f19, 544(x17)\n"
			"fmadd.ps f25, f26, f2, f19, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 448(x19)\n"
			"fcvt.w.s x9, f26, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 864(x11)\n"
			"fswizz.ps f4, f26, 0x19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 928(x11)\n"
			"flw.ps f4, 896(x11)\n"
			"feq.s x20, f17, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_3:\n"
			"feqm.ps m1, f29, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_3:\n"
			"fltm.ps m7, f28, f30\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x25dfce52cc\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_3:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 512(x11)\n"
			"frcp.ps f11, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 960(x10)\n"
			"fcvt.f16.ps f0, f28\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 640(x11)\n"
			"flw.ps f30, 32(x11)\n"
			"fsll.pi f19, f11, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 128(x9)\n"
			"flw.ps f25, 576(x9)\n"
			"fand.pi f25, f28, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 992(x19)\n"
			"flw.ps f29, 480(x19)\n"
			"feq.ps f19, f21, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 576(x26)\n"
			"flw.ps f21, 64(x26)\n"
			"fsub.s f27, f17, f21, rtz\n" 
			"maskpopcz x11, m7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 160(x19)\n"
			"fcvt.ps.pw f5, f19, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 160(x10)\n"
			"fcvt.f16.ps f25, f30\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 320(x20)\n"
			"flw.ps f21, 544(x20)\n"
			"fsgnjn.s f6, f5, f21\n" 
			"mova.x.m x10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 384(x19)\n"
			"fclass.ps f28, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 928(x7)\n"
			"flw.ps f11, 224(x7)\n"
			"fadd.ps f17, f21, f11, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 992(x5)\n"
			"flw.ps f17, 416(x5)\n"
			"fminu.pi f21, f29, f17\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 928(x25)\n"
			"frsq.ps f19, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_3:\n"
			"masknot m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_3:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 32(x19)\n"
			"fmul.ps f2, f28, f28, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 992(x9)\n"
			"flw.ps f13, 192(x9)\n"
			"fsub.s f25, f2, f13, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 192(x9)\n"
			"fswizz.ps f2, f21, 0x1e\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 256(x20)\n"
			"flw.ps f6, 512(x20)\n"
			"fmax.ps f29, f13, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 736(x25)\n"
			"flw.ps f2, 800(x25)\n"
			"fcmovm.ps f4, f21, f2\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_3:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 896(x9)\n"
			"flog.ps f6, f5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 672(x5)\n"
			"fsrli.pi f25, f2, 0xe\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 160(x20)\n"
			"fcvt.pw.ps f21, f26, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 800(x19)\n"
			"flw.ps f28, 512(x19)\n"
			"fmulh.pi f19, f27, f28\n" 
			"li x17, 0x1c40b5a2faa8c53b\n"
			"fcvt.s.w f19, x17, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 512(x10)\n"
			"fslli.pi f25, f19, 0x5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 864(x5)\n"
			"fnot.pi f11, f28\n" 
			"LBL_SEQID_2_FP_TRANS_ID_9_HID_3:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 448(x20)\n"
			"flog.ps f27, f17\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 192(x11)\n"
			"fnot.pi f21, f19\n" 
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
			"la x11, rand_int_32\n"
			"flw.ps f17, 352(x11)\n"
			"flw.ps f26, 128(x11)\n"
			"fmax.pi f6, f17, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 256(x9)\n"
			"flw.ps f4, 608(x9)\n"
			"flw.ps f17, 288(x9)\n"
			"fmadd.s f2, f27, f4, f17, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 384(x10)\n"
			"fsub.ps f19, f19, f19, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 800(x5)\n"
			"flw.ps f13, 896(x5)\n"
			"flw.ps f26, 896(x5)\n"
			"fmsub.ps f27, f30, f13, f26, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 256(x19)\n"
			"flw.ps f4, 256(x19)\n"
			"fmin.pi f11, f19, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 32(x11)\n"
			"fcvt.pw.ps f11, f29, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 928(x9)\n"
			"flw.ps f17, 224(x9)\n"
			"flt.pi f6, f11, f17\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 416(x5)\n"
			"flw.ps f21, 224(x5)\n"
			"fmaxu.pi f17, f28, f21\n" 
			"mova.x.m x5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 224(x26)\n"
			"flw.ps f2, 32(x26)\n"
			"fmul.pi f28, f6, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 960(x5)\n"
			"flw.ps f27, 128(x5)\n"
			"fmax.pi f4, f13, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 320(x10)\n"
			"flw.ps f30, 192(x10)\n"
			"flw.ps f21, 512(x10)\n"
			"fnmsub.ps f6, f25, f30, f21, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 320(x19)\n"
			"fsqrt.ps f30, f11\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 800(x19)\n"
			"flw.ps f6, 640(x19)\n"
			"fmaxu.pi f25, f0, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 32(x20)\n"
			"flw.ps f13, 576(x20)\n"
			"fadd.s f4, f26, f13, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 768(x25)\n"
			"flw.ps f28, 512(x25)\n"
			"flw.ps f26, 32(x25)\n"
			"fnmadd.ps f25, f0, f28, f26, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 576(x15)\n"
			"flw.ps f28, 864(x15)\n"
			"fsgnjn.ps f28, f29, f28\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 160(x16)\n"
			"frsq.ps f4, f16\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 608(x22)\n"
			"flw.ps f13, 32(x22)\n"
			"flw.ps f26, 672(x22)\n"
			"fmadd.s f19, f7, f13, f26, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"fsetm.pi m0, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f24, 320(x9)\n"
			"fsat8.pi f13, f24\n" 
			"li x17, 0x504ea48a627a2c4\n"
			"fcvt.s.w f20, x17, rmm\n" 
			"li x5, 0x8f54d3ec84290a00\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc79024212807ff38\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f12, 256(x13)\n"
			"flw.ps f28, 192(x13)\n"
			"fsub.pi f28, f12, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 576(x17)\n"
			"flw.ps f29, 64(x17)\n"
			"fsgnj.s f13, f26, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 832(x7)\n"
			"flw.ps f26, 32(x7)\n"
			"fsgnj.ps f13, f10, f26\n" 
			"la x16, rand_int_32\n"
			"flw.ps f29, 768(x16)\n"
			"fsatu8.pi f4, f29\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 832(x15)\n"
			"flw.ps f28, 192(x15)\n"
			"fmaxu.pi f24, f4, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 672(x15)\n"
			"flw.ps f12, 800(x15)\n"
			"fmin.ps f13, f16, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"maskxor m0, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 224(x7)\n"
			"fround.ps f12, f20, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 672(x16)\n"
			"flw.ps f13, 832(x16)\n"
			"fnmsub.s f19, f12, f13, f13, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 192(x14)\n"
			"flw.ps f5, 64(x14)\n"
			"flw.ps f10, 864(x14)\n"
			"fmadd.s f19, f4, f5, f10, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_287479_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x66 << TENSOR_FMA32_SCP_LOC_B) | (0xfc << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x3 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xa << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_4:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 480(x13)\n"
			"frcp.ps f26, f15\n" 
			"li x5, 0x3f5e\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_int_32\n"
			"flw.ps f13, 576(x7)\n"
			"fsat8.pi f7, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 672(x16)\n"
			"flw.ps f29, 512(x16)\n"
			"fsub.s f9, f5, f29, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 480(x13)\n"
			"flw.ps f12, 640(x13)\n"
			"fmaxu.pi f16, f29, f12\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_4:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 480(x22)\n"
			"flog.ps f4, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 672(x22)\n"
			"flw.ps f15, 736(x22)\n"
			"fnmsub.ps f28, f26, f26, f15, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 448(x13)\n"
			"flw.ps f5, 736(x13)\n"
			"fsgnjx.ps f10, f10, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 192(x14)\n"
			"flw.ps f20, 320(x14)\n"
			"flw.ps f29, 384(x14)\n"
			"fnmsub.ps f29, f28, f20, f29, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"maskxor m3, m0, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f26, 256(x7)\n"
			"flw.ps f4, 352(x7)\n"
			"fmin.pi f19, f26, f4\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_4:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 256(x7)\n"
			"frcp.ps f7, f7\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 384(x9)\n"
			"flw.ps f12, 896(x9)\n"
			"fadd.pi f19, f10, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 64(x14)\n"
			"flw.ps f19, 96(x14)\n"
			"fmax.s f16, f29, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 800(x7)\n"
			"fcvt.ps.pwu f26, f13, rne\n" 
			"li x6, 0x67e9ec171fcc28ec\n"
			"fcvt.s.wu f24, x6, rup\n" 
			"fbci.pi f13, 0x7b9fb\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x6 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x3 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x18 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x4 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f7, 288(x6)\n"
			"flw.ps f26, 960(x6)\n"
			"fsra.pi f26, f7, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_4:\n"
			"mov.m.x m3, x17, 0xd\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_4:\n"
			"fltm.pi m1, f15, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 352(x6)\n"
			"flw.ps f29, 128(x6)\n"
			"fmul.s f13, f7, f29, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 96(x7)\n"
			"flw.ps f2, 544(x7)\n"
			"flt.ps f26, f19, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 704(x17)\n"
			"flw.ps f5, 96(x17)\n"
			"fcmov.ps f15, f7, f7, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 800(x14)\n"
			"flw.ps f26, 64(x14)\n"
			"fmul.s f28, f9, f26, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 384(x15)\n"
			"flw.ps f4, 320(x15)\n"
			"fsgnjn.ps f2, f9, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_4:\n"
			"maskxor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_4:\n"
			"feqm.ps m5, f20, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 320(x11)\n"
			"flw.ps f2, 352(x11)\n"
			"fsgnj.ps f12, f10, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 832(x7)\n"
			"flw.ps f9, 576(x7)\n"
			"fmaxu.pi f9, f16, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 256(x9)\n"
			"flw.ps f12, 704(x9)\n"
			"fdiv.ps f16, f15, f12, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 32(x7)\n"
			"fcvt.l.s x22, f20, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 160(x16)\n"
			"flw.ps f13, 256(x16)\n"
			"fadd.ps f12, f28, f13, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 544(x14)\n"
			"fsat8.pi f15, f19\n" 
			"li x5, 0xaa09b4e215e14307\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xbfb0fa1b2836b11a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_529344_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x7 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1d << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xd << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x7 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x5 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_4:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 64(x14)\n"
			"fexp.ps f29, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_4:\n"
			"mov.m.x m0, x15, 0x43\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x34dd\n"
			"csrw tensor_mask, x5\n"	
			"li x11, 0xcb02669660dfdff6\n"
			"fcvt.s.l f5, x11, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 128(x13)\n"
			"flw.ps f4, 576(x13)\n"
			"fle.s x22, f16, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 192(x9)\n"
			"flw.ps f5, 192(x9)\n"
			"flw.ps f28, 416(x9)\n"
			"fnmsub.ps f7, f24, f5, f28, rtz\n" 
			"fbci.ps f7, 0x4279c\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 160(x22)\n"
			"fclass.ps f13, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 576(x7)\n"
			"frsq.ps f4, f10\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_4:\n"
			"fltm.ps m5, f10, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 896(x9)\n"
			"flw.ps f16, 608(x9)\n"
			"fadd.s f12, f28, f16, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 96(x15)\n"
			"fswizz.ps f2, f13, 0x4\n" 
			"maskpopc x26, m5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 672(x14)\n"
			"flw.ps f10, 640(x14)\n"
			"fsra.pi f10, f2, f10\n" 
			"maskpopcz x22, m3\n" 
			"li x15, 0x17e77ed9ce540ecb\n"
			"fcvt.s.wu f4, x15, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 192(x6)\n"
			"flw.ps f5, 544(x6)\n"
			"fmul.ps f24, f29, f5, rdn\n" 
			"li x5, 0x8660b17a6ae9e5bb\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa641494f24559b1e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 288(x28)\n"
			"flw.ps f29, 768(x28)\n"
			"fsgnjx.s f23, f26, f29\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 160(x17)\n"
			"flw.ps f24, 608(x17)\n"
			"flw.ps f9, 0(x17)\n"
			"fcmov.ps f28, f25, f24, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 128(x28)\n"
			"flw.ps f23, 864(x28)\n"
			"flw.ps f24, 768(x28)\n"
			"fcmov.ps f27, f1, f23, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 160(x26)\n"
			"fsrli.pi f5, f25, 0xe\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 736(x17)\n"
			"fcvt.wu.s x17, f2, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 704(x17)\n"
			"flw.ps f2, 0(x17)\n"
			"flw.ps f31, 96(x17)\n"
			"fmsub.s f8, f9, f2, f31, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 256(x26)\n"
			"flw.ps f27, 960(x26)\n"
			"fmul.ps f1, f5, f27, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 608(x7)\n"
			"flw.ps f12, 768(x7)\n"
			"fmul.s f31, f30, f12, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"fltm.pi m1, f21, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f24, 448(x5)\n"
			"flw.ps f31, 480(x5)\n"
			"fmax.pi f5, f24, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 736(x14)\n"
			"flw.ps f25, 128(x14)\n"
			"fmulh.pi f5, f30, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"masknot m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"fltm.pi m6, f24, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 320(x15)\n"
			"fcvt.f16.ps f21, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 256(x26)\n"
			"flw.ps f12, 480(x26)\n"
			"fsgnj.ps f1, f23, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_5:\n"
			"maskor m5, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 640(x26)\n"
			"fsin.ps f24, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_5:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 672(x6)\n"
			"frcp.ps f21, f29\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 864(x13)\n"
			"frsq.ps f24, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_5:\n"
			"mov.m.x m1, x28, 0x43\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_5:\n"
			"maskand m3, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f26, 704(x26)\n"
			"fslli.pi f12, f26, 0x0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 992(x18)\n"
			"fsrai.pi f31, f21, 0x8\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 320(x13)\n"
			"ffrc.ps f31, f23\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 832(x17)\n"
			"fslli.pi f8, f28, 0xd\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 0(x17)\n"
			"fclass.ps f26, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 992(x6)\n"
			"flw.ps f1, 864(x6)\n"
			"fadd.ps f27, f27, f1, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 608(x26)\n"
			"flw.ps f25, 224(x26)\n"
			"fdivu.pi f9, f26, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 64(x13)\n"
			"flw.ps f5, 576(x13)\n"
			"fmulh.pi f21, f31, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 128(x17)\n"
			"fmul.ps f21, f2, f2, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_7_HID_5:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 416(x18)\n"
			"fexp.ps f31, f5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 288(x5)\n"
			"flw.ps f21, 224(x5)\n"
			"fsub.pi f9, f5, f21\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_5:\n"
			"masknot m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_5:\n"
			"maskand m1, m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f5, 128(x17)\n"
			"for.pi f30, f5, f5\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 832(x7)\n"
			"flw.ps f5, 0(x7)\n"
			"fmax.pi f28, f26, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 864(x15)\n"
			"flw.ps f9, 352(x15)\n"
			"feq.pi f2, f2, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 64(x26)\n"
			"flw.ps f25, 0(x26)\n"
			"fmin.s f12, f21, f25\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 32(x6)\n"
			"flw.ps f26, 640(x6)\n"
			"flt.s x15, f23, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_5:\n"
			"maskor m3, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 64(x28)\n"
			"flw.ps f31, 384(x28)\n"
			"flw.ps f5, 480(x28)\n"
			"fnmadd.ps f5, f1, f31, f5, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 352(x6)\n"
			"fsat8.pi f27, f31\n" 
			"li x28, 0xd7f85d6b59a9005f\n"
			"fcvt.s.lu f28, x28, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_5:\n"
			"maskand m1, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 800(x15)\n"
			"frsq.ps f28, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 896(x14)\n"
			"flw.ps f30, 96(x14)\n"
			"fand.pi f29, f21, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_12_HID_5:\n"
			"maskxor m5, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 128(x15)\n"
			"flw.ps f26, 288(x15)\n"
			"fmax.ps f2, f5, f26\n" 
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
			"flw.ps f29, 512(x7)\n"
			"fandi.pi f21, f29, 0x118\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 864(x26)\n"
			"flw.ps f29, 928(x26)\n"
			"fsgnj.ps f26, f26, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_5:\n"
			"feqm.ps m5, f9, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 576(x5)\n"
			"flw.ps f9, 32(x5)\n"
			"fle.s x28, f25, f9\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 672(x5)\n"
			"flw.ps f21, 160(x5)\n"
			"fle.pi f30, f29, f21\n" 
			"maskpopc x18, m1\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 640(x17)\n"
			"flw.ps f21, 736(x17)\n"
			"fmin.pi f31, f1, f21\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 224(x6)\n"
			"flw.ps f25, 992(x6)\n"
			"fmaxu.pi f2, f30, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 800(x14)\n"
			"flw.ps f24, 288(x14)\n"
			"fcmovm.ps f30, f29, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f21, 0(x26)\n"
			"flw.ps f28, 768(x26)\n"
			"fsll.pi f27, f21, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 96(x5)\n"
			"flw.ps f8, 992(x5)\n"
			"fmin.s f30, f24, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_14_HID_5:\n"
			"mov.m.x m6, x18, 0x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 416(x26)\n"
			"fnot.pi f5, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 384(x26)\n"
			"fcvt.f16.ps f27, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_15_HID_5:\n"
			"mov.m.x m1, x5, 0x21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 256(x28)\n"
			"ffrc.ps f24, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x18, rand_int_32\n"
			"flw.ps f24, 960(x18)\n"
			"fslli.pi f4, f24, 0x8\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_6:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 0(x14)\n"
			"fexp.ps f4, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 352(x25)\n"
			"flw.ps f11, 832(x25)\n"
			"fsgnj.ps f0, f19, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 736(x12)\n"
			"flw.ps f10, 0(x12)\n"
			"fsgnj.ps f16, f25, f10\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 416(x12)\n"
			"fcvt.ps.pwu f31, f19, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_6:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 128(x11)\n"
			"frcp.ps f19, f9\n" 
			"li x5, 0xc44dfe634f24c828\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xaa2f3654bd342d95\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x25, rand_int_32\n"
			"flw.ps f0, 160(x25)\n"
			"flw.ps f7, 448(x25)\n"
			"fltu.pi f12, f0, f7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f16, 832(x27)\n"
			"flw.ps f9, 896(x27)\n"
			"for.pi f0, f16, f9\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 0(x27)\n"
			"flw.ps f11, 672(x27)\n"
			"flw.ps f26, 128(x27)\n"
			"fmadd.ps f26, f0, f11, f26, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f12, 800(x12)\n"
			"flw.ps f19, 96(x12)\n"
			"frem.pi f16, f12, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f6, 320(x18)\n"
			"flw.ps f19, 288(x18)\n"
			"fmulh.pi f19, f6, f19\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 608(x9)\n"
			"fsat8.pi f12, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 512(x27)\n"
			"flw.ps f6, 64(x27)\n"
			"fmax.ps f6, f7, f6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 160(x12)\n"
			"fcvt.pw.ps f10, f25, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 640(x11)\n"
			"flw.ps f26, 32(x11)\n"
			"fmul.pi f9, f6, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x3 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1f << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x4 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x5 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x3 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1d << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x9 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 736(x11)\n"
			"flw.ps f4, 896(x11)\n"
			"flw.ps f7, 672(x11)\n"
			"fmadd.s f14, f9, f4, f7, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 736(x9)\n"
			"flw.ps f7, 320(x9)\n"
			"fmul.s f25, f31, f7, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 64(x12)\n"
			"flw.ps f6, 640(x12)\n"
			"fle.pi f0, f24, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 640(x11)\n"
			"fsrli.pi f11, f11, 0xc\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 32(x10)\n"
			"fcvt.l.s x14, f11, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 544(x7)\n"
			"flw.ps f0, 960(x7)\n"
			"flw.ps f31, 192(x7)\n"
			"fnmsub.s f16, f12, f0, f31, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 736(x25)\n"
			"flw.ps f7, 160(x25)\n"
			"fmul.s f0, f26, f7, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"fltm.pi m3, f12, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 128(x15)\n"
			"flw.ps f11, 160(x15)\n"
			"fsgnjx.s f6, f19, f11\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 544(x11)\n"
			"fpackreph.pi f14, f10\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 704(x18)\n"
			"flw.ps f26, 544(x18)\n"
			"flw.ps f31, 800(x18)\n"
			"fnmsub.ps f6, f11, f26, f31, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 960(x12)\n"
			"fcvt.pwu.ps f19, f12, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 544(x7)\n"
			"fcvt.w.s x7, f10, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 608(x27)\n"
			"flw.ps f19, 224(x27)\n"
			"flw.ps f2, 832(x27)\n"
			"fnmadd.s f16, f7, f19, f2, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 640(x10)\n"
			"fand.pi f31, f14, f14\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f9, 288(x10)\n"
			"fcvt.ps.f16 f24, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_6:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 320(x7)\n"
			"frcp.ps f24, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 960(x7)\n"
			"flw.ps f31, 576(x7)\n"
			"feq.ps f4, f6, f31\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 448(x12)\n"
			"fpackreph.pi f12, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 992(x15)\n"
			"flw.ps f24, 640(x15)\n"
			"fxor.pi f0, f31, f24\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 0(x7)\n"
			"flw.ps f0, 416(x7)\n"
			"feq.pi f14, f25, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 992(x18)\n"
			"flw.ps f25, 640(x18)\n"
			"flw.ps f7, 448(x18)\n"
			"fmadd.ps f4, f12, f25, f7, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 384(x14)\n"
			"flw.ps f4, 320(x14)\n"
			"fmul.ps f25, f9, f4, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 512(x12)\n"
			"fcvt.wu.s x25, f14, rmm\n" 
			"li x5, 0x398bb2c33da840d1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x588edf2df774daf2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 384(x18)\n"
			"flw.ps f19, 416(x18)\n"
			"fmsub.s f2, f14, f19, f14, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 256(x14)\n"
			"flw.ps f12, 512(x14)\n"
			"flt.pi f26, f31, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 224(x9)\n"
			"flw.ps f31, 448(x9)\n"
			"fsgnjx.ps f26, f16, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 416(x14)\n"
			"fandi.pi f14, f25, 0x10c\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 960(x9)\n"
			"flw.ps f7, 896(x9)\n"
			"fand.pi f11, f24, f7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 608(x27)\n"
			"flw.ps f0, 128(x27)\n"
			"fltu.pi f25, f19, f0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 288(x11)\n"
			"fsatu8.pi f24, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_6:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_202881_HID_6:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9c << TENSOR_IMA_SCP_LOC_B) | (0x3a << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 416(x9)\n"
			"flw.ps f10, 576(x9)\n"
			"flw.ps f25, 352(x9)\n"
			"fnmadd.s f16, f12, f10, f25, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 512(x7)\n"
			"flw.ps f2, 704(x7)\n"
			"fmulh.pi f4, f4, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 928(x25)\n"
			"flw.ps f25, 288(x25)\n"
			"fmul.ps f0, f4, f25, rne\n" 
			"li x5, 0x4898\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 32(x7)\n"
			"flw.ps f26, 192(x7)\n"
			"fsgnjx.ps f19, f31, f26\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 736(x27)\n"
			"flw.ps f11, 736(x27)\n"
			"fmin.s f19, f7, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 704(x11)\n"
			"fclass.ps f7, f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 672(x15)\n"
			"flw.ps f9, 160(x15)\n"
			"fmin.s f16, f6, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 384(x11)\n"
			"fnot.pi f6, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 864(x15)\n"
			"flw.ps f2, 960(x15)\n"
			"fcmovm.ps f26, f16, f2\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_6:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 512(x10)\n"
			"frcp.ps f7, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 224(x25)\n"
			"flw.ps f2, 64(x25)\n"
			"fmin.ps f24, f6, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 160(x10)\n"
			"fcvt.ps.pw f2, f4, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 736(x15)\n"
			"flw.ps f19, 608(x15)\n"
			"fmulh.pi f31, f9, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 704(x15)\n"
			"fswizz.ps f2, f26, 0x8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 128(x14)\n"
			"flw.ps f12, 192(x14)\n"
			"fmin.pi f16, f31, f12\n" 
			"la x27, rand_int_32\n"
			"flw.ps f31, 320(x27)\n"
			"fsat8.pi f4, f31\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_618715_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x4 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x5 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x18 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x1 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_211536_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_809450_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x0 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x9 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe << TENSOR_FMA16_SCP_LOC_B) | (0x25 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 896(x26)\n"
			"flw.ps f29, 224(x26)\n"
			"fadd.s f8, f14, f29, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 320(x7)\n"
			"flw.ps f19, 96(x7)\n"
			"flw.ps f9, 352(x7)\n"
			"fmsub.ps f10, f16, f19, f9, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 480(x14)\n"
			"frcp.ps f0, f5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 416(x14)\n"
			"flw.ps f16, 704(x14)\n"
			"fmul.pi f4, f13, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"maskxor m2, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 704(x7)\n"
			"fcvt.w.s x9, f13, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 832(x14)\n"
			"flw.ps f8, 640(x14)\n"
			"flw.ps f0, 160(x14)\n"
			"fcmov.ps f8, f14, f8, f0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 320(x27)\n"
			"fcvt.wu.s x25, f10, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 96(x25)\n"
			"flw.ps f30, 480(x25)\n"
			"flw.ps f8, 896(x25)\n"
			"fnmadd.s f4, f13, f30, f8, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 928(x12)\n"
			"flw.ps f24, 800(x12)\n"
			"fltu.pi f30, f4, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"fltm.pi m2, f8, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_7:\n"
			"mov.m.x m2, x9, 0x59\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 448(x14)\n"
			"flw.ps f13, 416(x14)\n"
			"fmax.ps f14, f9, f13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 32(x9)\n"
			"flw.ps f30, 352(x9)\n"
			"feq.pi f23, f10, f30\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 736(x25)\n"
			"fandi.pi f29, f0, 0x75\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 512(x13)\n"
			"fclass.ps f19, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_7:\n"
			"feqm.ps m1, f19, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_7:\n"
			"maskor m6, m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 448(x18)\n"
			"flw.ps f4, 864(x18)\n"
			"fmin.s f23, f5, f4\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 928(x26)\n"
			"flw.ps f5, 416(x26)\n"
			"fadd.pi f19, f30, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 576(x11)\n"
			"flw.ps f13, 640(x11)\n"
			"fmulhu.pi f19, f14, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 384(x12)\n"
			"fcvt.ps.pw f23, f9, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f24, 768(x27)\n"
			"faddi.pi f0, f24, 0x1aa\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 544(x11)\n"
			"flw.ps f8, 960(x11)\n"
			"flw.ps f10, 96(x11)\n"
			"fmsub.s f24, f12, f8, f10, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 928(x26)\n"
			"fcvt.ps.pwu f0, f13, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 640(x27)\n"
			"fpackreph.pi f29, f23\n" 
			"fbci.pi f13, 0xfb3b\n" 
			"la x20, rand_int_32\n"
			"flw.ps f14, 640(x20)\n"
			"flw.ps f12, 512(x20)\n"
			"fmaxu.pi f4, f14, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 672(x20)\n"
			"flw.ps f8, 64(x20)\n"
			"flt.ps f17, f5, f8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 352(x12)\n"
			"flw.ps f4, 128(x12)\n"
			"fmulhu.pi f13, f10, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_7:\n"
			"mov.m.x m6, x13, 0x62\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 576(x9)\n"
			"fclass.ps f13, f14\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_7:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 224(x26)\n"
			"frcp.ps f29, f30\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 512(x20)\n"
			"fcvt.f16.ps f19, f17\n" 
			"la x27, rand_int_32\n"
			"flw.ps f14, 448(x27)\n"
			"flw.ps f8, 832(x27)\n"
			"fsub.pi f10, f14, f8\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 320(x11)\n"
			"flw.ps f4, 128(x11)\n"
			"feq.pi f13, f8, f4\n" 
			"mova.x.m x25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 64(x27)\n"
			"flw.ps f8, 672(x27)\n"
			"fadd.s f23, f30, f8, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_7:\n"
			"flem.ps m6, f19, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_7:\n"
			"feqm.ps m2, f16, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 160(x11)\n"
			"ffrc.ps f30, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 896(x14)\n"
			"flw.ps f13, 192(x14)\n"
			"feq.pi f29, f14, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 512(x26)\n"
			"flw.ps f5, 704(x26)\n"
			"fadd.pi f10, f8, f5\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 256(x12)\n"
			"flw.ps f4, 960(x12)\n"
			"flw.ps f30, 864(x12)\n"
			"fmsub.s f14, f12, f4, f30, rne\n" 
			"li x14, 0xaf92d724ca06bb2b\n"
			"fcvt.s.wu f16, x14, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 320(x20)\n"
			"fpackrepb.pi f23, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 128(x7)\n"
			"flw.ps f14, 704(x7)\n"
			"flt.s x14, f5, f14\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 512(x20)\n"
			"fclass.ps f29, f16\n" 
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 640(x25)\n"
			"flw.ps f17, 832(x25)\n"
			"fle.s x26, f10, f17\n" 
			"la x18, rand_int_32\n"
			"flw.ps f12, 160(x18)\n"
			"fsat8.pi f29, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 352(x14)\n"
			"fcvt.pwu.ps f29, f12, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f24, 736(x27)\n"
			"fpackrepb.pi f30, f24\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 544(x11)\n"
			"flw.ps f24, 768(x11)\n"
			"fminu.pi f29, f23, f24\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 480(x11)\n"
			"flw.ps f29, 288(x11)\n"
			"fle.ps f16, f4, f29\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 512(x12)\n"
			"flw.ps f19, 640(x12)\n"
			"for.pi f14, f17, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 640(x7)\n"
			"flw.ps f9, 768(x7)\n"
			"flw.ps f5, 736(x7)\n"
			"fnmadd.ps f14, f19, f9, f5, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 896(x26)\n"
			"feq.ps f23, f9, f9\n" 
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_7:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 576(x20)\n"
			"frcp.ps f24, f29\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 32(x12)\n"
			"flw.ps f30, 896(x12)\n"
			"fmul.pi f30, f17, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 544(x26)\n"
			"flw.ps f30, 128(x26)\n"
			"fsgnjn.s f8, f12, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 384(x7)\n"
			"flw.ps f30, 160(x7)\n"
			"fsgnjx.s f16, f14, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 480(x7)\n"
			"flw.ps f23, 0(x7)\n"
			"fsrl.pi f13, f9, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 672(x7)\n"
			"flw.ps f5, 64(x7)\n"
			"flw.ps f24, 928(x7)\n"
			"fmadd.s f0, f0, f5, f24, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 704(x13)\n"
			"fsat8.pi f24, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x9, rand_int_32\n"
			"flw.ps f23, 96(x9)\n"
			"fpackrepb.pi f2, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 896(x7)\n"
			"fmul.s f26, f4, f4, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 192(x28)\n"
			"flw.ps f16, 864(x28)\n"
			"fadd.s f26, f6, f16, rdn\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_8:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 576(x27)\n"
			"fexp.ps f26, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 576(x20)\n"
			"flw.ps f24, 864(x20)\n"
			"fand.pi f19, f26, f24\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 192(x15)\n"
			"flw.ps f30, 480(x15)\n"
			"fremu.pi f2, f21, f30\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 928(x20)\n"
			"fsrai.pi f0, f27, 0xd\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 960(x19)\n"
			"fpackreph.pi f22, f4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 256(x15)\n"
			"fandi.pi f21, f2, 0xa4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 288(x20)\n"
			"flw.ps f27, 544(x20)\n"
			"fdivu.pi f27, f3, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 608(x16)\n"
			"flw.ps f3, 32(x16)\n"
			"fle.ps f4, f26, f3\n" 
			"li x5, 0xad6e\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 608(x27)\n"
			"fcvt.pwu.ps f26, f0, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 672(x15)\n"
			"flw.ps f4, 928(x15)\n"
			"fsub.s f26, f6, f4, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 416(x28)\n"
			"flw.ps f16, 640(x28)\n"
			"fsgnj.s f7, f2, f16\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 64(x28)\n"
			"flw.ps f16, 32(x28)\n"
			"feq.pi f23, f0, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f14, 512(x16)\n"
			"fslli.pi f22, f14, 0xa\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_971279_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xfd << TENSOR_FMA32_SCP_LOC_B) | (0xf9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 352(x16)\n"
			"fswizz.ps f24, f2, 0xc\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 320(x9)\n"
			"fsrli.pi f14, f23, 0x9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 704(x15)\n"
			"fsat8.pi f14, f22\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 640(x7)\n"
			"flw.ps f23, 960(x7)\n"
			"for.pi f16, f27, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_8:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 416(x15)\n"
			"frcp.ps f22, f24\n" 
			"li x15, 0x9a19f962f0f70a75\n"
			"fcvt.s.wu f6, x15, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 128(x19)\n"
			"fswizz.ps f16, f7, 0x8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"maskand m0, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f3, 224(x19)\n"
			"flw.ps f30, 416(x19)\n"
			"fsll.pi f22, f3, f30\n" 
			"la x15, rand_int_32\n"
			"flw.ps f16, 480(x15)\n"
			"flw.ps f4, 640(x15)\n"
			"fmulh.pi f19, f16, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"fsetm.pi m7, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x4af9\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"feqm.ps m0, f24, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_8:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 192(x23)\n"
			"frcp.ps f27, f21\n" 
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_8:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 0(x7)\n"
			"frcp.ps f24, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_8:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 480(x27)\n"
			"flw.ps f27, 448(x27)\n"
			"fsub.s f16, f21, f27, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_435294_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x3 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x2 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x4 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa5 << TENSOR_FMA16_SCP_LOC_B) | (0xa0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_8:\n"
			"maskxor m0, m1, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 512(x23)\n"
			"flw.ps f27, 928(x23)\n"
			"fsgnjn.ps f27, f23, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_int_32\n"
			"flw.ps f19, 544(x19)\n"
			"fsatu8.pi f14, f19\n" 
			"maskpopcz x23, m6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 896(x28)\n"
			"ffrc.ps f3, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_8:\n"
			"feqm.ps m7, f2, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 448(x28)\n"
			"flw.ps f6, 928(x28)\n"
			"flw.ps f14, 640(x28)\n"
			"fmsub.ps f22, f23, f6, f14, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 0(x19)\n"
			"flw.ps f6, 416(x19)\n"
			"flw.ps f26, 640(x19)\n"
			"fnmsub.s f26, f23, f6, f26, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 704(x16)\n"
			"flw.ps f23, 960(x16)\n"
			"flw.ps f21, 0(x16)\n"
			"fnmsub.ps f2, f2, f23, f21, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_8:\n"
			"mova.m.x x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_8:\n"
			"flem.ps m0, f19, f26\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 288(x27)\n"
			"flw.ps f22, 928(x27)\n"
			"fle.s x27, f27, f22\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 96(x23)\n"
			"fslli.pi f4, f27, 0x5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 576(x6)\n"
			"fslli.pi f7, f27, 0xa\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 320(x9)\n"
			"fswizz.ps f16, f16, 0xa\n" 
			"maskpopc x23, m1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x7 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x3 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1f << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x6 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f23, 352(x15)\n"
			"fslli.pi f19, f23, 0x0\n" 
			"maskpopcz x15, m1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 640(x12)\n"
			"flw.ps f16, 96(x12)\n"
			"flw.ps f24, 0(x12)\n"
			"fnmsub.s f21, f2, f16, f24, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 544(x23)\n"
			"flw.ps f23, 864(x23)\n"
			"fand.pi f21, f7, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 864(x6)\n"
			"flw.ps f6, 544(x6)\n"
			"feq.pi f4, f21, f6\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 192(x7)\n"
			"flw.ps f14, 928(x7)\n"
			"fmulh.pi f0, f24, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 384(x12)\n"
			"flw.ps f14, 128(x12)\n"
			"flt.s x7, f23, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 480(x23)\n"
			"flw.ps f27, 544(x23)\n"
			"fmin.s f24, f24, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 576(x16)\n"
			"flw.ps f0, 832(x16)\n"
			"fmul.s f30, f4, f0, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 832(x9)\n"
			"flw.ps f2, 576(x9)\n"
			"for.pi f14, f23, f2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 704(x27)\n"
			"flw.ps f16, 608(x27)\n"
			"fle.ps f22, f30, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 928(x15)\n"
			"flw.ps f24, 960(x15)\n"
			"fdiv.ps f7, f19, f24, rne\n" 
			"LBL_SEQID_3_FP_TRANS_ID_12_HID_8:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 480(x20)\n"
			"frcp.ps f16, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 672(x20)\n"
			"flw.ps f4, 352(x20)\n"
			"fmin.ps f4, f21, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 384(x12)\n"
			"fslli.pi f19, f0, 0x5\n" 
			"LBL_SEQID_3_FP_TRANS_ID_13_HID_8:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 32(x9)\n"
			"flog.ps f22, f26\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_636241_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x6f << TENSOR_FMA32_SCP_LOC_B) | (0xc2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x35 << TENSOR_FMA32_SCP_LOC_B) | (0x23 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 800(x25)\n"
			"fmul.s f22, f5, f5, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 64(x15)\n"
			"flw.ps f29, 256(x15)\n"
			"fsgnj.s f4, f13, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 128(x25)\n"
			"flw.ps f30, 288(x25)\n"
			"feq.ps f5, f14, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 256(x19)\n"
			"fcvt.ps.pw f28, f28, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 256(x26)\n"
			"flw.ps f2, 256(x26)\n"
			"fsub.ps f31, f11, f2, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 288(x7)\n"
			"flw.ps f31, 992(x7)\n"
			"fle.s x19, f29, f31\n" 
			"maskpopc x7, m6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 512(x14)\n"
			"flw.ps f4, 192(x14)\n"
			"fsgnjn.s f21, f9, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 640(x9)\n"
			"flw.ps f28, 320(x9)\n"
			"feq.s x28, f9, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 608(x28)\n"
			"fcvt.pwu.ps f2, f31, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 448(x9)\n"
			"fclass.s x6, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 128(x25)\n"
			"flw.ps f11, 800(x25)\n"
			"flw.ps f13, 352(x25)\n"
			"fmadd.s f5, f9, f11, f13, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 864(x28)\n"
			"fsub.pi f28, f2, f2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 512(x15)\n"
			"fandi.pi f20, f2, 0x111\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f4, 192(x7)\n"
			"fcvt.ps.f16 f9, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 896(x9)\n"
			"flw.ps f30, 832(x9)\n"
			"fadd.ps f28, f14, f30, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f30, 960(x15)\n"
			"fcvt.ps.pwu f31, f30, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 384(x6)\n"
			"flw.ps f2, 160(x6)\n"
			"fmin.s f28, f31, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 64(x15)\n"
			"fclass.ps f30, f10\n" 
			"maskpopc x6, m0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 608(x25)\n"
			"flw.ps f20, 512(x25)\n"
			"flt.ps f14, f5, f20\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 128(x6)\n"
			"fswizz.ps f4, f9, 0x7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 544(x26)\n"
			"fmin.s f31, f29, f29\n" 
			"LBL_SEQID_1_FP_TRANS_ID_0_HID_9:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 0(x28)\n"
			"flog.ps f2, f18\n" 
			"maskpopcz x15, m1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 320(x19)\n"
			"flw.ps f20, 32(x19)\n"
			"fmin.ps f9, f21, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 768(x26)\n"
			"fround.ps f21, f31, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 32(x7)\n"
			"flw.ps f14, 512(x7)\n"
			"fsgnj.ps f13, f13, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 256(x25)\n"
			"flw.ps f5, 0(x25)\n"
			"fsgnjx.ps f29, f18, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 928(x6)\n"
			"flw.ps f4, 320(x6)\n"
			"flw.ps f21, 480(x6)\n"
			"fnmsub.s f28, f9, f4, f21, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 64(x19)\n"
			"fround.ps f5, f13, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 768(x14)\n"
			"flt.s x26, f22, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f2, 608(x15)\n"
			"fsrli.pi f31, f2, 0x9\n" 
			"li x7, 0x6ccbd755fa60d3fc\n"
			"fcvt.s.l f5, x7, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 992(x14)\n"
			"flw.ps f29, 640(x14)\n"
			"fadd.pi f9, f13, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 448(x28)\n"
			"flw.ps f2, 672(x28)\n"
			"fsgnjn.ps f11, f18, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 448(x26)\n"
			"flw.ps f14, 96(x26)\n"
			"flt.ps f18, f28, f14\n" 
			"LBL_SEQID_2_FP_TRANS_ID_1_HID_9:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 64(x26)\n"
			"frcp.ps f13, f9\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 960(x25)\n"
			"fswizz.ps f14, f5, 0x1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 960(x15)\n"
			"fround.ps f5, f2, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 192(x15)\n"
			"fcvt.f16.ps f21, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f2, 992(x7)\n"
			"fcvt.ps.pw f30, f2, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_9:\n"
			"maskor m6, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f13, 672(x9)\n"
			"flw.ps f20, 768(x9)\n"
			"fxor.pi f30, f13, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 128(x15)\n"
			"fsrli.pi f14, f31, 0xb\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 448(x15)\n"
			"flw.ps f28, 384(x15)\n"
			"fnmadd.s f11, f4, f28, f28, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 256(x15)\n"
			"flw.ps f2, 800(x15)\n"
			"fmin.s f28, f10, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 640(x23)\n"
			"flw.ps f2, 640(x23)\n"
			"flt.s x7, f29, f2\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_9:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f11, 512(x9)\n"
			"fsat8.pi f14, f11\n" 
			"LBL_SEQID_3_FP_TRANS_ID_4_HID_9:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 768(x26)\n"
			"frcp.ps f18, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 576(x23)\n"
			"flw.ps f10, 640(x23)\n"
			"fadd.s f2, f13, f10, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 640(x7)\n"
			"flw.ps f5, 192(x7)\n"
			"fcmovm.ps f22, f13, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 352(x9)\n"
			"ffrc.ps f2, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 160(x14)\n"
			"flw.ps f28, 416(x14)\n"
			"fsgnj.ps f2, f5, f28\n" 
			"maskpopc x14, m6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 416(x6)\n"
			"frsq.ps f20, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 224(x23)\n"
			"flw.ps f11, 128(x23)\n"
			"fsll.pi f21, f21, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 576(x23)\n"
			"faddi.pi f14, f2, 0x3b\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 928(x25)\n"
			"flw.ps f30, 768(x25)\n"
			"flw.ps f29, 576(x25)\n"
			"fnmsub.s f28, f9, f30, f29, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 0(x6)\n"
			"flw.ps f11, 160(x6)\n"
			"fdivu.pi f10, f30, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 160(x9)\n"
			"flw.ps f18, 512(x9)\n"
			"fle.s x7, f20, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 768(x26)\n"
			"flw.ps f28, 832(x26)\n"
			"fsub.pi f28, f2, f28\n" 
			"li x14, 0xa7247507c0022872\n"
			"fcvt.s.w f29, x14, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f30,  64 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x13, rand_int_32\n"
			"flw.ps f12, 96(x13)\n"
			"flw.ps f26, 928(x13)\n"
			"fmaxu.pi f3, f12, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 384(x9)\n"
			"flw.ps f14, 256(x9)\n"
			"fmax.pi f8, f20, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 800(x11)\n"
			"flw.ps f0, 160(x11)\n"
			"fdiv.pi f26, f8, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 992(x17)\n"
			"fsin.ps f7, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 896(x26)\n"
			"fcvt.pw.ps f8, f3, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 896(x9)\n"
			"flw.ps f8, 96(x9)\n"
			"fsub.s f20, f7, f8, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 864(x5)\n"
			"flw.ps f23, 448(x5)\n"
			"fmul.s f23, f0, f23, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 960(x25)\n"
			"frcp.ps f20, f4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 960(x13)\n"
			"fcvt.pw.ps f14, f7, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 192(x25)\n"
			"flw.ps f27, 160(x25)\n"
			"fmulh.pi f26, f3, f27\n" 
			"fbci.pi f3, 0x6f063\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 288(x9)\n"
			"fpackrepb.pi f23, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 352(x9)\n"
			"fcvt.pw.ps f3, f26, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 320(x5)\n"
			"flw.ps f26, 224(x5)\n"
			"fminu.pi f27, f3, f26\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 352(x7)\n"
			"fcvt.ps.pw f26, f23, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 576(x19)\n"
			"flw.ps f5, 800(x19)\n"
			"fmin.s f11, f23, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_175195_HID_10:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 160(x26)\n"
			"flog.ps f5, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_10:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 416(x14)\n"
			"fexp.ps f12, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"feqm.ps m6, f26, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f7, 608(x14)\n"
			"flw.ps f27, 960(x14)\n"
			"fsrl.pi f26, f7, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 992(x13)\n"
			"fsin.ps f31, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 864(x7)\n"
			"fslli.pi f16, f31, 0xf\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 736(x9)\n"
			"fsqrt.ps f11, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 960(x19)\n"
			"fcvt.wu.s x19, f20, rtz\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f5, 448(x26)\n"
			"fcvt.ps.f16 f27, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 864(x26)\n"
			"fcvt.f16.ps f14, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"maskand m4, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_10:\n"
			"mov.m.x m6, x17, 0x6c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f20, 640(x25)\n"
			"flw.ps f31, 800(x25)\n"
			"flt.pi f4, f20, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 864(x26)\n"
			"fcvt.wu.s x17, f11, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 736(x14)\n"
			"fsrai.pi f0, f20, 0xe\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 512(x11)\n"
			"fsgnj.s f7, f4, f4\n" 
			"li x5, 0x1009\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"li x5, 0x5936\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_727317_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x0 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x3 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x5 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_617014_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x7 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xa << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x19 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x7 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_10:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f8, 768(x13)\n"
			"flw.ps f9, 992(x13)\n"
			"fmulhu.pi f7, f8, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 832(x7)\n"
			"fcvt.lu.s x26, f7, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 160(x5)\n"
			"flw.ps f23, 928(x5)\n"
			"flt.s x25, f26, f23\n" 
			"la x19, rand_int_32\n"
			"flw.ps f11, 992(x19)\n"
			"faddi.pi f23, f11, 0x13b\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 608(x17)\n"
			"flw.ps f3, 128(x17)\n"
			"fmin.s f3, f7, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 160(x7)\n"
			"frsq.ps f31, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_10:\n"
			"maskxor m7, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 608(x14)\n"
			"flw.ps f31, 512(x14)\n"
			"flt.s x17, f5, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_10:\n"
			"feqm.ps m1, f0, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f4, 320(x5)\n"
			"fcvt.ps.pwu f4, f4, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 672(x25)\n"
			"flw.ps f0, 800(x25)\n"
			"fmin.s f16, f16, f0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 864(x5)\n"
			"frsq.ps f23, f20\n" 
			"li x5, 0x4498\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 576(x5)\n"
			"flw.ps f4, 672(x5)\n"
			"flw.ps f0, 928(x5)\n"
			"fnmsub.s f16, f26, f4, f0, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 0(x14)\n"
			"flw.ps f0, 256(x14)\n"
			"fsub.ps f7, f27, f0, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_10:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 320(x9)\n"
			"flw.ps f26, 96(x9)\n"
			"fmul.s f4, f20, f26, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x7562ba7858349180\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb0ff0236fe52a5ef\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x6 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x0 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1e << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xe << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x4 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xf << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x18 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xa << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 640(x19)\n"
			"flw.ps f27, 192(x19)\n"
			"fsgnj.ps f20, f23, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 672(x14)\n"
			"flw.ps f27, 32(x14)\n"
			"fmax.pi f27, f5, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 352(x14)\n"
			"flw.ps f4, 896(x14)\n"
			"fadd.ps f23, f5, f4, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 736(x13)\n"
			"fpackrepb.pi f11, f27\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_10:\n"
			"maskand m4, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xb28a4d5cd1ef146f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5b1e072ac03d40a9\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 640(x26)\n"
			"ffrc.ps f12, f9\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_10:\n"
			"maskor m1, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_10:\n"
			"maskor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x88aae7e0e1\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_10:\n"
			"mov.m.x m6, x14, 0x78\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 992(x14)\n"
			"flw.ps f27, 128(x14)\n"
			"fle.s x25, f11, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 0(x25)\n"
			"fsrai.pi f8, f0, 0x0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_FP_TRANS_ID_13_HID_10:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 96(x19)\n"
			"fexp.ps f31, f8\n" 
			"LBL_SEQID_3_FP_TRANS_ID_14_HID_10:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 832(x7)\n"
			"frcp.ps f9, f16\n" 
			"LBL_SEQID_3_M0_WRITE_ID_15_HID_10:\n"
			"fsetm.pi m4, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f11, 864(x5)\n"
			"fpackreph.pi f14, f11\n" 
			"fbci.ps f0, 0x6f4f1\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_255528_HID_10:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_782053_HID_10:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x8 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x4 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x18, rand_int_32\n"
			"flw.ps f12, 896(x18)\n"
			"flw.ps f23, 608(x18)\n"
			"fsra.pi f26, f12, f23\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 960(x16)\n"
			"fnot.pi f24, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 800(x28)\n"
			"fcvt.f16.ps f8, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 480(x10)\n"
			"ffrc.ps f21, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 512(x16)\n"
			"flw.ps f8, 192(x16)\n"
			"fmaxu.pi f21, f6, f8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 576(x17)\n"
			"fcvt.ps.pwu f9, f3, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 800(x9)\n"
			"fround.ps f24, f7, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 320(x20)\n"
			"fcvt.lu.s x23, f4, rmm\n" 
			"li x20, 0xfd73c5a0e59c5617\n"
			"fcvt.s.wu f26, x20, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_11:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 704(x16)\n"
			"flog.ps f5, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 352(x16)\n"
			"flw.ps f24, 288(x16)\n"
			"fsub.ps f8, f23, f24, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 0(x20)\n"
			"fsat8.pi f13, f24\n" 
			"fbci.pi f4, 0x3909f\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 64(x28)\n"
			"flw.ps f4, 800(x28)\n"
			"fsll.pi f13, f13, f4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 768(x13)\n"
			"flw.ps f28, 320(x13)\n"
			"flt.pi f23, f26, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 960(x19)\n"
			"flw.ps f5, 480(x19)\n"
			"fsub.ps f23, f24, f5, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 224(x19)\n"
			"feq.s x13, f24, f24\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 416(x18)\n"
			"fsrli.pi f12, f9, 0x8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 0(x28)\n"
			"fsat8.pi f30, f24\n" 
			"maskpopcz x9, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 864(x10)\n"
			"fcvt.l.s x19, f21, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 160(x20)\n"
			"flw.ps f9, 416(x20)\n"
			"fmax.ps f30, f4, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 704(x23)\n"
			"flw.ps f7, 0(x23)\n"
			"fmin.pi f5, f21, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_11:\n"
			"fsetm.pi m3, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f9, 256(x10)\n"
			"flw.ps f8, 96(x10)\n"
			"fmul.pi f26, f9, f8\n" 
			"li x13, 0x27ae7506dc938382\n"
			"fcvt.s.wu f24, x13, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_11:\n"
			"maskand m2, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f5, 96(x13)\n"
			"flw.ps f0, 224(x13)\n"
			"fminu.pi f21, f5, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 32(x28)\n"
			"flw.ps f12, 352(x28)\n"
			"fadd.ps f8, f24, f12, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 352(x17)\n"
			"flw.ps f12, 608(x17)\n"
			"flt.ps f23, f0, f12\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 256(x16)\n"
			"feq.pi f24, f3, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f28, 448(x25)\n"
			"flw.ps f4, 416(x25)\n"
			"fmulh.pi f30, f28, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f30, 416(x28)\n"
			"flw.ps f0, 928(x28)\n"
			"fle.pi f7, f30, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_11:\n"
			"maskand m2, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f12, 672(x10)\n"
			"flw.ps f5, 800(x10)\n"
			"fsra.pi f26, f12, f5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_11:\n"
			"fltm.pi m4, f21, f5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4b3f185459\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_11:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 448(x18)\n"
			"flog.ps f4, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 0(x19)\n"
			"fround.ps f0, f8, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 64(x10)\n"
			"flw.ps f13, 992(x10)\n"
			"fsgnjx.ps f4, f9, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 480(x23)\n"
			"fcvt.pwu.ps f3, f21, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f13, 320(x13)\n"
			"feq.pi f21, f13, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 64(x17)\n"
			"flw.ps f30, 480(x17)\n"
			"flt.s x25, f9, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 160(x28)\n"
			"fandi.pi f26, f13, 0x171\n" 
			"maskpopc x23, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 0(x18)\n"
			"flw.ps f8, 736(x18)\n"
			"fmax.ps f0, f7, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 224(x25)\n"
			"flw.ps f8, 928(x25)\n"
			"fmul.ps f26, f0, f8, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"flem.ps m4, f30, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 992(x13)\n"
			"flw.ps f6, 288(x13)\n"
			"fsgnjx.s f5, f13, f6\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_11:\n"
			"fsetm.pi m4, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 512(x28)\n"
			"fround.ps f12, f0, rup\n" 
			"li x17, 0xe1d5e24b92edb80c\n"
			"fcvt.s.w f7, x17, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 800(x16)\n"
			"faddi.pi f26, f21, 0x12d\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 544(x17)\n"
			"flw.ps f30, 864(x17)\n"
			"feq.s x25, f9, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 736(x28)\n"
			"fsin.ps f7, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 544(x13)\n"
			"fcmovm.ps f13, f9, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 736(x16)\n"
			"ffrc.ps f4, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 32(x17)\n"
			"flw.ps f9, 640(x17)\n"
			"fmul.pi f30, f26, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 288(x16)\n"
			"fsatu8.pi f13, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_11:\n"
			"fltm.ps m2, f26, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f3, 320(x20)\n"
			"fandi.pi f9, f3, 0x37\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 640(x9)\n"
			"flw.ps f21, 896(x9)\n"
			"fmin.pi f4, f6, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 704(x17)\n"
			"flw.ps f30, 384(x17)\n"
			"fsgnjx.ps f24, f6, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 800(x17)\n"
			"flw.ps f24, 0(x17)\n"
			"fmulh.pi f5, f28, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 960(x25)\n"
			"flw.ps f13, 608(x25)\n"
			"fadd.ps f13, f9, f13, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f25,  736 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
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
			"LBL_KERNEL_HID_12:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_12\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 224(x26)\n"
			"flw.ps f26, 96(x26)\n"
			"fmin.s f24, f21, f26\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f1, 960(x10)\n"
			"fcvt.ps.f16 f14, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 32(x19)\n"
			"fcvt.w.s x26, f2, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 864(x12)\n"
			"flw.ps f24, 288(x12)\n"
			"flt.pi f23, f14, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 608(x17)\n"
			"flw.ps f16, 608(x17)\n"
			"fand.pi f30, f14, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 64(x17)\n"
			"fcvt.pw.ps f2, f14, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 704(x28)\n"
			"flw.ps f24, 960(x28)\n"
			"fmax.ps f2, f26, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 640(x9)\n"
			"flw.ps f23, 0(x9)\n"
			"fmin.ps f21, f21, f23\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 928(x10)\n"
			"flw.ps f16, 320(x10)\n"
			"flw.ps f30, 160(x10)\n"
			"fnmsub.s f26, f26, f16, f30, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"fltm.pi m5, f25, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f21, 576(x5)\n"
			"fpackrepb.pi f25, f21\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 704(x25)\n"
			"fclass.s x9, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 544(x12)\n"
			"flw.ps f11, 352(x12)\n"
			"fsgnjn.s f16, f31, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 608(x17)\n"
			"flw.ps f21, 32(x17)\n"
			"fmax.s f26, f16, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f11, 992(x28)\n"
			"flw.ps f4, 640(x28)\n"
			"fsrl.pi f21, f11, f4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 800(x28)\n"
			"flw.ps f26, 320(x28)\n"
			"fsgnjn.s f1, f23, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd1 << TENSOR_FMA32_SCP_LOC_B) | (0x6f << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_12:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 256(x28)\n"
			"frcp.ps f21, f24\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 192(x10)\n"
			"flw.ps f25, 128(x10)\n"
			"fsra.pi f10, f16, f25\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 0(x12)\n"
			"fclass.ps f26, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 256(x27)\n"
			"fclass.s x10, f16\n" 
			"li x28, 0xc6e6a07d55a985e6\n"
			"fcvt.s.wu f18, x28, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f21, 960(x10)\n"
			"fandi.pi f2, f21, 0x15a\n" 
			"la x12, rand_int_32\n"
			"flw.ps f23, 672(x12)\n"
			"flw.ps f24, 992(x12)\n"
			"fsll.pi f21, f23, f24\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 768(x27)\n"
			"flw.ps f31, 928(x27)\n"
			"fdivu.pi f26, f4, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 64(x10)\n"
			"fcvt.w.s x5, f10, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_12:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 320(x28)\n"
			"frcp.ps f1, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 864(x19)\n"
			"flw.ps f18, 896(x19)\n"
			"flw.ps f10, 512(x19)\n"
			"fmadd.s f2, f26, f18, f10, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_12:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 704(x17)\n"
			"flog.ps f10, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 960(x10)\n"
			"fcvt.ps.pw f26, f14, rdn\n" 
			"fbci.ps f10, 0x7d681\n" 
			"li x5, 0xa56\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 544(x10)\n"
			"flw.ps f26, 640(x10)\n"
			"flw.ps f23, 160(x10)\n"
			"fnmsub.s f2, f31, f26, f23, rne\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f24, 512(x26)\n"
			"fcvt.ps.f16 f2, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x39 << TENSOR_FMA32_SCP_LOC_B) | (0x34 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 384(x17)\n"
			"fcvt.pw.ps f18, f0, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"feqm.ps m4, f18, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f30, 800(x25)\n"
			"flw.ps f31, 256(x25)\n"
			"fltu.pi f18, f30, f31\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 992(x12)\n"
			"fclass.ps f25, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 0(x26)\n"
			"fround.ps f21, f14, rup\n" 
			"fbci.ps f25, 0x1afb3\n" 
			"li x19, 0x2beaf73a34e9cdce\n"
			"fcvt.s.w f18, x19, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 736(x25)\n"
			"flw.ps f26, 864(x25)\n"
			"flw.ps f25, 128(x25)\n"
			"fnmsub.s f14, f4, f26, f25, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 928(x5)\n"
			"fround.ps f24, f21, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 192(x28)\n"
			"flw.ps f14, 288(x28)\n"
			"fxor.pi f14, f0, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopcz x12, m4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"mova.m.x x26\n" 
			"li x5, 0x80340140\n"
			"ld x5, 0(x5)\n"
			"maskpopc x17, m3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 800(x12)\n"
			"flw.ps f25, 160(x12)\n"
			"fsll.pi f24, f4, f25\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 288(x28)\n"
			"fandi.pi f18, f14, 0xa6\n" 
			"li x5, 0x2d89\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_12:\n"
			"fsetm.pi m7, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_975027_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x6 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x19 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xd << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x5 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xf << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x28, rand_int_32\n"
			"flw.ps f21, 832(x28)\n"
			"flw.ps f4, 384(x28)\n"
			"fxor.pi f30, f21, f4\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f21, 0(x5)\n"
			"fpackrepb.pi f10, f21\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_12:\n"
			"mov.m.x m7, x17, 0x6e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xa811\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f1, 416(x12)\n"
			"faddi.pi f1, f1, 0xb1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 320(x10)\n"
			"fround.ps f23, f1, rmm\n" 
			"li x10, 0x65f0af3ab5984af8\n"
			"fcvt.s.l f18, x10, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 640(x5)\n"
			"flw.ps f23, 640(x5)\n"
			"fmin.s f16, f11, f23\n" 
			"la x5, rand_int_32\n"
			"flw.ps f16, 480(x5)\n"
			"flw.ps f11, 160(x5)\n"
			"flt.pi f11, f16, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 800(x26)\n"
			"flw.ps f24, 416(x26)\n"
			"fmul.s f16, f18, f24, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 544(x5)\n"
			"flw.ps f31, 320(x5)\n"
			"fltu.pi f26, f0, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 896(x27)\n"
			"flw.ps f26, 832(x27)\n"
			"fsub.ps f23, f25, f26, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 384(x19)\n"
			"flw.ps f30, 64(x19)\n"
			"fsgnjx.ps f24, f0, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 768(x26)\n"
			"flw.ps f0, 352(x26)\n"
			"fsgnjn.s f14, f16, f0\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f10, 160(x26)\n"
			"fcvt.ps.f16 f1, f10\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 192(x17)\n"
			"flw.ps f30, 0(x17)\n"
			"fsra.pi f30, f26, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 64(x27)\n"
			"fsat8.pi f14, f10\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_139801_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x3 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x6 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x19 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"fbci.ps f11, 0x6993a\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 512(x27)\n"
			"flw.ps f16, 32(x27)\n"
			"fsub.ps f7, f19, f16, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 736(x5)\n"
			"flw.ps f14, 896(x5)\n"
			"flw.ps f17, 800(x5)\n"
			"fnmadd.s f15, f25, f14, f17, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 128(x10)\n"
			"flw.ps f18, 768(x10)\n"
			"fmax.pi f28, f14, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 128(x5)\n"
			"flw.ps f14, 256(x5)\n"
			"fmul.s f14, f6, f14, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 608(x10)\n"
			"flw.ps f20, 960(x10)\n"
			"fmax.pi f7, f14, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 480(x27)\n"
			"fandi.pi f16, f12, 0x103\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 768(x17)\n"
			"fcvt.pw.ps f14, f14, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_13:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 192(x5)\n"
			"fexp.ps f15, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 960(x17)\n"
			"flw.ps f28, 576(x17)\n"
			"fxor.pi f20, f19, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"maskxor m5, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f6, 640(x11)\n"
			"fsatu8.pi f14, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 64(x11)\n"
			"fnot.pi f7, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 544(x10)\n"
			"flw.ps f19, 736(x10)\n"
			"fmaxu.pi f7, f11, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f11, 832(x25)\n"
			"flw.ps f6, 608(x25)\n"
			"fsra.pi f14, f11, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 224(x11)\n"
			"flw.ps f15, 800(x11)\n"
			"flw.ps f17, 704(x11)\n"
			"fnmsub.ps f19, f18, f15, f17, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 704(x11)\n"
			"flw.ps f20, 576(x11)\n"
			"fsgnjx.s f20, f25, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 416(x7)\n"
			"fcvt.wu.s x16, f20, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 992(x5)\n"
			"fcvt.f16.ps f28, f14\n" 
			"li x16, 0xc50f3acdb1cad077\n"
			"fcvt.s.lu f25, x16, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 576(x5)\n"
			"flt.s x26, f11, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 416(x27)\n"
			"fcvt.w.s x7, f17, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 512(x25)\n"
			"flw.ps f19, 224(x25)\n"
			"fxor.pi f14, f9, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 320(x16)\n"
			"flw.ps f27, 704(x16)\n"
			"flw.ps f11, 672(x16)\n"
			"fmadd.s f28, f17, f27, f11, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f25, 832(x23)\n"
			"fpackrepb.pi f28, f25\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 704(x27)\n"
			"fpackrepb.pi f15, f20\n" 
			"li x10, 0x511c0f48e97d829b\n"
			"fcvt.s.w f16, x10, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 352(x23)\n"
			"flw.ps f16, 576(x23)\n"
			"fmax.pi f14, f19, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 288(x23)\n"
			"flw.ps f10, 32(x23)\n"
			"fsgnjx.ps f27, f6, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 896(x26)\n"
			"flw.ps f6, 320(x26)\n"
			"feq.s x27, f9, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 736(x15)\n"
			"flw.ps f27, 256(x15)\n"
			"fadd.ps f18, f9, f27, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"maskxor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f18, 672(x5)\n"
			"flw.ps f16, 352(x5)\n"
			"fxor.pi f19, f18, f16\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 256(x5)\n"
			"flw.ps f19, 672(x5)\n"
			"feq.s x26, f17, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_13:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f19, 160(x27)\n"
			"flw.ps f9, 384(x27)\n"
			"for.pi f25, f19, f9\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 352(x5)\n"
			"flw.ps f28, 192(x5)\n"
			"fmulh.pi f19, f14, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 864(x25)\n"
			"flw.ps f6, 352(x25)\n"
			"fmax.ps f15, f28, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 32(x26)\n"
			"fsgnj.ps f12, f20, f20\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_13:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 992(x26)\n"
			"frcp.ps f7, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 320(x23)\n"
			"flw.ps f7, 224(x23)\n"
			"flt.s x23, f25, f7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 704(x27)\n"
			"flw.ps f19, 736(x27)\n"
			"fminu.pi f27, f15, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 800(x17)\n"
			"fslli.pi f18, f15, 0x7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"mov.m.x m0, x5, 0x52\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 224(x27)\n"
			"fcvt.pwu.ps f10, f15, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 160(x5)\n"
			"flw.ps f12, 608(x5)\n"
			"fsgnjx.s f17, f16, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 576(x17)\n"
			"fsat8.pi f28, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 448(x5)\n"
			"flw.ps f17, 800(x5)\n"
			"fsra.pi f11, f7, f17\n" 
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
			"la x25, rand_int_32\n"
			"flw.ps f9, 864(x25)\n"
			"flw.ps f15, 288(x25)\n"
			"feq.pi f18, f9, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 192(x27)\n"
			"flw.ps f28, 256(x27)\n"
			"fmaxu.pi f27, f10, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 256(x23)\n"
			"fswizz.ps f18, f18, 0x15\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 192(x5)\n"
			"ffrc.ps f9, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 352(x26)\n"
			"fnot.pi f10, f14\n" 
			"la x27, rand_int_32\n"
			"flw.ps f7, 608(x27)\n"
			"fcvt.ps.pwu f20, f7, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 736(x11)\n"
			"flw.ps f15, 0(x11)\n"
			"fle.ps f7, f25, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 800(x27)\n"
			"flw.ps f16, 160(x27)\n"
			"for.pi f18, f15, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 832(x7)\n"
			"fpackrepb.pi f16, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 512(x26)\n"
			"fround.ps f14, f6, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 960(x16)\n"
			"fsin.ps f12, f12\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 160(x16)\n"
			"flw.ps f27, 608(x16)\n"
			"fdiv.pi f17, f28, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 960(x27)\n"
			"fmax.s f19, f28, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 640(x7)\n"
			"flw.ps f19, 832(x7)\n"
			"fmax.s f7, f20, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f28, 32(x25)\n"
			"flw.ps f27, 288(x25)\n"
			"fxor.pi f11, f28, f27\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 384(x10)\n"
			"flw.ps f27, 928(x10)\n"
			"fle.pi f10, f14, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 21\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"feqm.ps m0, f2, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f2, 896(x12)\n"
			"flw.ps f10, 608(x12)\n"
			"fadd.pi f4, f2, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 32(x6)\n"
			"flw.ps f24, 384(x6)\n"
			"fsll.pi f18, f27, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 512(x26)\n"
			"flw.ps f10, 800(x26)\n"
			"fmulh.pi f18, f27, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"mov.m.x m1, x9, 0x5e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_14:\n"
			"fltm.pi m0, f2, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_14:\n"
			"flem.ps m1, f25, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 320(x6)\n"
			"flw.ps f3, 576(x6)\n"
			"fsgnjx.ps f5, f17, f3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 704(x28)\n"
			"fcvt.pw.ps f3, f23, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_14:\n"
			"maskxor m7, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 32(x12)\n"
			"flw.ps f24, 128(x12)\n"
			"flt.s x22, f18, f24\n" 
			"maskpopc x14, m1\n" 
			"li x25, 0x466e2e3f5a7a3102\n"
			"fcvt.s.wu f5, x25, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 352(x16)\n"
			"flw.ps f3, 928(x16)\n"
			"fsgnjn.s f2, f0, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 192(x12)\n"
			"flw.ps f10, 64(x12)\n"
			"flw.ps f5, 960(x12)\n"
			"fcmov.ps f10, f23, f10, f5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_14:\n"
			"fltm.ps m1, f18, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_828706_HID_14:\n"
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_870040_HID_14:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_353898_HID_14:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x69 << TENSOR_IMA_SCP_LOC_B) | (0x6c << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f30, 448(x6)\n"
			"fcvt.ps.pw f29, f30, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f21, 704(x25)\n"
			"flt.pi f10, f21, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 480(x14)\n"
			"flw.ps f17, 64(x14)\n"
			"flt.ps f4, f5, f17\n" 
			"fbci.ps f25, 0x43876\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 640(x14)\n"
			"ffrc.ps f21, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 928(x25)\n"
			"flw.ps f27, 192(x25)\n"
			"flw.ps f29, 352(x25)\n"
			"fmsub.s f30, f24, f27, f29, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 192(x23)\n"
			"flw.ps f10, 512(x23)\n"
			"fmax.s f29, f5, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 512(x16)\n"
			"flw.ps f10, 288(x16)\n"
			"flt.s x22, f30, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 704(x23)\n"
			"flw.ps f23, 224(x23)\n"
			"fcmovm.ps f0, f3, f23\n" 
			"li x5, 0xfd6\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_14:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 576(x22)\n"
			"flog.ps f25, f18\n" 
			"mova.x.m x15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 576(x12)\n"
			"flw.ps f10, 192(x12)\n"
			"fsgnjn.s f24, f17, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 864(x15)\n"
			"flw.ps f21, 512(x15)\n"
			"for.pi f25, f27, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 608(x26)\n"
			"flw.ps f18, 992(x26)\n"
			"fmin.s f24, f21, f18\n" 
			"li x5, 0x824610457bb81f44\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8c8fd7cc94a4524c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f21, 288(x9)\n"
			"fxor.pi f24, f21, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 192(x15)\n"
			"flw.ps f0, 0(x15)\n"
			"fadd.ps f27, f17, f0, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_692282_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x97 << TENSOR_IMA_SCP_LOC_B) | (0x80 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_869403_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x41 << TENSOR_FMA16_SCP_LOC_B) | (0x94 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_378438_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x94 << TENSOR_IMA_SCP_LOC_B) | (0xf4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f21, 672(x26)\n"
			"fcvt.ps.pwu f17, f21, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 512(x25)\n"
			"fsin.ps f21, f18\n" 
			"li x5, 0xf1ed\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 288(x9)\n"
			"fcvt.w.s x16, f19, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_14:\n"
			"maskand m7, m0, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x6, 0x8ed7a689577081bc\n"
			"fcvt.s.w f5, x6, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 704(x28)\n"
			"flw.ps f2, 320(x28)\n"
			"fsrl.pi f24, f18, f2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 128(x22)\n"
			"flw.ps f27, 576(x22)\n"
			"feq.ps f0, f5, f27\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 64(x14)\n"
			"flw.ps f29, 160(x14)\n"
			"fnmsub.ps f27, f27, f29, f27, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 992(x15)\n"
			"fclass.ps f19, f4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 832(x16)\n"
			"fnot.pi f29, f2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 544(x12)\n"
			"flw.ps f24, 480(x12)\n"
			"fsgnjn.s f24, f2, f24\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 352(x9)\n"
			"flw.ps f25, 608(x9)\n"
			"flw.ps f4, 640(x9)\n"
			"fnmsub.ps f17, f30, f25, f4, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 896(x26)\n"
			"flw.ps f29, 800(x26)\n"
			"flw.ps f10, 64(x26)\n"
			"fnmadd.s f4, f24, f29, f10, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_14:\n"
			"feqm.ps m0, f21, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 736(x16)\n"
			"flw.ps f10, 576(x16)\n"
			"fdiv.s f24, f27, f10, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 576(x14)\n"
			"flw.ps f3, 96(x14)\n"
			"flt.s x25, f23, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x13e0\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 928(x25)\n"
			"fclass.ps f30, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_14:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 96(x28)\n"
			"flog.ps f27, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 832(x25)\n"
			"flw.ps f19, 992(x25)\n"
			"flw.ps f0, 512(x25)\n"
			"fmsub.s f29, f4, f19, f0, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 192(x14)\n"
			"flw.ps f4, 672(x14)\n"
			"fmin.ps f27, f21, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_14:\n"
			"flem.ps m1, f27, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 480(x28)\n"
			"flw.ps f17, 192(x28)\n"
			"fmin.ps f30, f4, f17\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_14:\n"
			"maskor m0, m6, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 832(x23)\n"
			"fclass.s x23, f24\n" 
			"LBL_SEQID_3_FP_TRANS_ID_12_HID_14:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 992(x6)\n"
			"flog.ps f5, f27\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 608(x12)\n"
			"flw.ps f17, 960(x12)\n"
			"fmin.s f25, f4, f17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 96(x15)\n"
			"flw.ps f27, 992(x15)\n"
			"fsub.s f23, f3, f27, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 32(x12)\n"
			"fcvt.ps.pwu f21, f10, rdn\n" 
			"maskpopcz x23, m7\n" 
			"li x5, 0x449\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_14:\n"
			"mov.m.x m1, x6, 0x4c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f17, 832(x28)\n"
			"flw.ps f25, 352(x28)\n"
			"fand.pi f23, f17, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 512(x12)\n"
			"flw.ps f29, 512(x12)\n"
			"fsrl.pi f3, f21, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x18 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x6 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 4\n" 
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
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
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x6, rand_ieee754_16\n"
			"flw.ps f9, 672(x6)\n"
			"fcvt.ps.f16 f22, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 672(x13)\n"
			"flw.ps f23, 928(x13)\n"
			"fmax.s f0, f31, f23\n" 
			"fbci.pi f18, 0x3763d\n" 
			"li x11, 0xdfa11f9982fd324c\n"
			"fcvt.s.wu f29, x11, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 800(x23)\n"
			"flw.ps f25, 736(x23)\n"
			"fsub.s f9, f29, f25, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 672(x10)\n"
			"fclass.s x10, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 768(x6)\n"
			"flw.ps f9, 864(x6)\n"
			"flw.ps f22, 640(x6)\n"
			"fnmsub.ps f0, f29, f9, f22, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"flem.ps m2, f18, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 672(x13)\n"
			"flw.ps f16, 960(x13)\n"
			"fmul.s f29, f0, f16, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"fltm.pi m0, f3, f31\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xdf1f8e1667\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 480(x7)\n"
			"flw.ps f0, 256(x7)\n"
			"fadd.ps f3, f9, f0, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 896(x7)\n"
			"flw.ps f13, 608(x7)\n"
			"fmin.pi f18, f9, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 704(x26)\n"
			"fclass.ps f3, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 384(x15)\n"
			"flw.ps f20, 0(x15)\n"
			"flw.ps f13, 480(x15)\n"
			"fmsub.s f10, f1, f20, f13, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 256(x23)\n"
			"feq.ps f16, f23, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 352(x11)\n"
			"flw.ps f20, 960(x11)\n"
			"flt.s x26, f10, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f16, 416(x7)\n"
			"flw.ps f18, 64(x7)\n"
			"fltu.pi f31, f16, f18\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 480(x26)\n"
			"flw.ps f18, 352(x26)\n"
			"fdiv.s f10, f22, f18, rne\n" 
			"fbci.pi f18, 0x1ae46\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"maskand m1, m6, m6\n" 
			"li x5, 0x803401B0\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_int_32\n"
			"flw.ps f0, 288(x17)\n"
			"flw.ps f1, 864(x17)\n"
			"feq.pi f9, f0, f1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 640(x10)\n"
			"flw.ps f18, 800(x10)\n"
			"fmax.ps f13, f13, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 352(x6)\n"
			"fcvt.wu.s x10, f0, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 128(x15)\n"
			"flw.ps f1, 992(x15)\n"
			"flw.ps f29, 416(x15)\n"
			"fcmov.ps f31, f3, f1, f29\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 704(x15)\n"
			"fsrli.pi f29, f22, 0x6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 192(x14)\n"
			"fcvt.ps.pw f29, f22, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 288(x6)\n"
			"flw.ps f18, 320(x6)\n"
			"fle.s x19, f1, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 704(x7)\n"
			"flw.ps f25, 864(x7)\n"
			"fmax.s f13, f13, f25\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 800(x7)\n"
			"fsat8.pi f31, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 256(x7)\n"
			"flw.ps f20, 480(x7)\n"
			"fcmovm.ps f16, f23, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 0(x10)\n"
			"fcvt.w.s x15, f31, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 544(x17)\n"
			"flw.ps f9, 864(x17)\n"
			"fadd.s f13, f24, f9, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_16\n"
			"flw.ps f22, 480(x10)\n"
			"fcvt.ps.f16 f25, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 288(x17)\n"
			"flw.ps f13, 96(x17)\n"
			"fadd.ps f18, f0, f13, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 768(x19)\n"
			"flw.ps f24, 160(x19)\n"
			"flw.ps f22, 224(x19)\n"
			"fnmadd.s f31, f16, f24, f22, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 832(x26)\n"
			"flw.ps f29, 672(x26)\n"
			"flw.ps f23, 128(x26)\n"
			"fmsub.s f22, f1, f29, f23, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 928(x11)\n"
			"flw.ps f1, 576(x11)\n"
			"fsgnjx.ps f0, f0, f1\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f20, 512(x19)\n"
			"fcvt.ps.f16 f25, f20\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 768(x6)\n"
			"flw.ps f9, 352(x6)\n"
			"flt.ps f20, f20, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_15:\n"
			"masknot m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 608(x14)\n"
			"flw.ps f18, 736(x14)\n"
			"flt.ps f20, f0, f18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 64(x14)\n"
			"fcvt.ps.pwu f31, f13, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 384(x17)\n"
			"flw.ps f31, 640(x17)\n"
			"fsub.s f29, f23, f31, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 768(x23)\n"
			"fcvt.w.s x17, f20, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 768(x7)\n"
			"flw.ps f0, 32(x7)\n"
			"for.pi f12, f16, f0\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 96(x15)\n"
			"flw.ps f13, 576(x15)\n"
			"fmulh.pi f25, f24, f13\n" 
			"la x17, rand_int_32\n"
			"flw.ps f16, 448(x17)\n"
			"flw.ps f25, 608(x17)\n"
			"fmin.pi f31, f16, f25\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 32(x19)\n"
			"flw.ps f13, 256(x19)\n"
			"fsgnjx.s f29, f9, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 992(x6)\n"
			"flw.ps f22, 64(x6)\n"
			"fmul.pi f12, f29, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f25, 704(x15)\n"
			"flw.ps f22, 160(x15)\n"
			"fmulh.pi f13, f25, f22\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 64(x26)\n"
			"fsrli.pi f9, f12, 0xe\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 288(x23)\n"
			"flw.ps f29, 864(x23)\n"
			"flt.s x6, f16, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_15:\n"
			"mov.m.x m0, x11, 0x56\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_15:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 384(x17)\n"
			"fexp.ps f0, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 960(x17)\n"
			"fclass.ps f20, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 896(x17)\n"
			"flw.ps f10, 704(x17)\n"
			"flw.ps f31, 192(x17)\n"
			"fnmadd.ps f25, f22, f10, f31, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 288(x13)\n"
			"flw.ps f25, 32(x13)\n"
			"fsub.ps f18, f1, f25, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 576(x10)\n"
			"flw.ps f31, 544(x10)\n"
			"fmax.s f13, f10, f31\n" 
			"fbci.pi f31, 0x32d8a\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_15:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f0, 480(x15)\n"
			"fcvt.ps.pw f18, f0, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 928(x11)\n"
			"flw.ps f29, 608(x11)\n"
			"frem.pi f1, f20, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 576(x23)\n"
			"fclass.s x10, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
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
		0xe3d77f01, 
		0x082f1a43, 
		0x0f8044a8, 
		0x9e43e933, 
		0x63922438, 
		0x99a16b9e, 
		0xd5bd0132, 
		0x9be4078c, 
		0x50f7b168, 
		0xba4ee77a, 
		0x2a9dcb87, 
		0x1de067d0, 
		0xcd45f31a, 
		0x7a1a3293, 
		0x557985e0, 
		0x47a7fde0, 
		0x99933bf7, 
		0xd526e8f9, 
		0x95acd14a, 
		0x3f0121f3, 
		0x73866561, 
		0xb04516b7, 
		0x524f853f, 
		0x449d27f9, 
		0xc8789ae0, 
		0x25a1ba53, 
		0x7ffe6c7d, 
		0x88d8c0a5, 
		0x61b99161, 
		0xfb7678d3, 
		0x653f387f, 
		0xb555b9fa, 
		0xed0e4528, 
		0x628da935, 
		0x8a6243fd, 
		0x100899d1, 
		0x522c9583, 
		0x33adba6f, 
		0x3673174d, 
		0x1799a7da, 
		0x4a30189b, 
		0x5be04057, 
		0xdb611f75, 
		0x3f0dd583, 
		0x7e46da13, 
		0xddca8b0c, 
		0x14ece04c, 
		0x07c597f7, 
		0x9b6d4eb5, 
		0x1815f07d, 
		0x90c2ed6d, 
		0x36ad61dd, 
		0x47a293f3, 
		0x2182e980, 
		0xd7ffc8cd, 
		0x56be6d2a, 
		0xfe9f0bb4, 
		0x60d1d905, 
		0x070b80f4, 
		0xb4a041f3, 
		0xe511b411, 
		0xa3ccb0a4, 
		0xec125488, 
		0x17076e31, 
		0x81aa0cf0, 
		0x2ec37ac9, 
		0xd98592ee, 
		0xb8808c83, 
		0x2c10514f, 
		0xf11425e4, 
		0xf0f058c5, 
		0xeb4acb49, 
		0x7bc1bdc0, 
		0x19dedb49, 
		0xbf8b90fa, 
		0xa70b407e, 
		0x78817548, 
		0x5f26f21f, 
		0x63da3177, 
		0x5ffee55e, 
		0x61307c05, 
		0x70fe98a0, 
		0xcebbdcb7, 
		0xe4653d35, 
		0xe99f4a92, 
		0xcc816356, 
		0xd534c087, 
		0xbfc43ff7, 
		0xc73fa908, 
		0xf53c77bf, 
		0x133d4b63, 
		0xf8e96431, 
		0x3bdfae68, 
		0x5db44741, 
		0x54fc94a4, 
		0xbc6e9d5f, 
		0xe3aa471c, 
		0x263e8db3, 
		0x6b134907, 
		0x3f2b7713, 
		0x0681edaf, 
		0x4beac505, 
		0xcddc68d6, 
		0x42bb68de, 
		0x1bf702d8, 
		0x7b80f213, 
		0x8371f5f2, 
		0xecdbc47b, 
		0x8f58640b, 
		0xd5d50f76, 
		0x1e832d72, 
		0xc13de7cf, 
		0xf87fcf8e, 
		0x3cf74354, 
		0x63e08fb2, 
		0x29858691, 
		0xfa811b6d, 
		0xa3ca8d60, 
		0x81feaf2b, 
		0x82c2c4ba, 
		0x7168fcfb, 
		0x495125cc, 
		0x5c2f7626, 
		0x68b053ed, 
		0x2e4177ed, 
		0xd7e730ed, 
		0x2cf5ec78, 
		0x99c453ef, 
		0xd4376fb5, 
		0xbb18f1be, 
		0xc2e33943, 
		0x2ce1a325, 
		0x4edbfef8, 
		0x0291be02, 
		0x850203ab, 
		0xea8f3be0, 
		0x58ff0624, 
		0xf2159ff5, 
		0x171fddd2, 
		0xc352b37e, 
		0xaa5d0b4b, 
		0x3f933587, 
		0xfc57b67c, 
		0x0963423a, 
		0xb3c721a8, 
		0xdbaaae92, 
		0xee9f585d, 
		0x1243749c, 
		0xe99c7e50, 
		0xde3b3ddd, 
		0xe5e61cd7, 
		0xbb1f453d, 
		0x22662de7, 
		0x16ad95c8, 
		0x9e7bf788, 
		0x2afa3645, 
		0x4fd98632, 
		0xf4921539, 
		0x04fac06e, 
		0xbd1ea0e8, 
		0x42ec600e, 
		0x71b7e67c, 
		0x2dd11155, 
		0x45e42f4d, 
		0x77001ae3, 
		0xc2f268b9, 
		0x1c2b94eb, 
		0x1f1d7202, 
		0xe26a86b8, 
		0x3a1ed8f1, 
		0x65886209, 
		0xa28ecd3f, 
		0xb1a16a1b, 
		0x0944e14c, 
		0x5cfe42a6, 
		0x6694b89e, 
		0xd6ac6c77, 
		0xb72ce129, 
		0x8fa2fc70, 
		0x5a79b902, 
		0xded8ddd2, 
		0x02f53c3b, 
		0x2fffb94b, 
		0x53089e3f, 
		0xab4cc89d, 
		0x39b8f4a7, 
		0x0a4eecb2, 
		0x9ef50006, 
		0x9f9bc6d3, 
		0x19baa4a4, 
		0x037fb23b, 
		0x0a175b0e, 
		0x1cf070c7, 
		0x2abf1627, 
		0xe6c38898, 
		0x15a01783, 
		0xee92b445, 
		0x70a25794, 
		0x82fa5847, 
		0xe29bd78f, 
		0xea63fc95, 
		0x93cce111, 
		0x462c3476, 
		0xbc65f6c0, 
		0x62fb96f0, 
		0x5de7818b, 
		0x7a54c2e3, 
		0xd19e2a95, 
		0x07ed25f3, 
		0x556b29dd, 
		0x8bc11ff7, 
		0x657e08bc, 
		0xec97d7e1, 
		0xf3bb6654, 
		0x535282cb, 
		0x4519feb0, 
		0x375504a5, 
		0x0593c11a, 
		0x8d16c274, 
		0x591631cd, 
		0xa8603999, 
		0x8459d2f4, 
		0xd596a703, 
		0xc349dc1a, 
		0x855b9df9, 
		0xef175e5d, 
		0x6ab03eaa, 
		0xab11f5e0, 
		0xfedf9a7d, 
		0xb418b27a, 
		0x1a7592a5, 
		0x1e110eb0, 
		0x264e5ace, 
		0x1c6c347d, 
		0xda080c92, 
		0xb1511400, 
		0x5a453866, 
		0x5a5b2c16, 
		0x986d7a4c, 
		0xa5f08356, 
		0xfd5ec696, 
		0x6173db2a, 
		0x4cce4a50, 
		0x8970978f, 
		0xcd8e4dc5, 
		0x3b603d92, 
		0x54803006, 
		0x5368de8b, 
		0xf8dce53f, 
		0xf4b6c7c1, 
		0x068c1935, 
		0xe55929b1, 
		0x6e182b31, 
		0x76d8fc8f, 
		0xad1d2cb9, 
		0x3ab18dae  
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
		0x51e0, 
		0x39f3, 
		0x5300, 
		0x3146, 
		0xd3c0, 
		0xd140, 
		0xc000, 
		0x4b00, 
		0xd4a1, 
		0x38bb, 
		0xcc40, 
		0xdda6, 
		0xd0a0, 
		0x50e0, 
		0x7c00, 
		0xd180, 
		0xca00, 
		0xcd80, 
		0x4e40, 
		0x53e0, 
		0xfe4d, 
		0x5280, 
		0x470a, 
		0x0caf, 
		0xeb34, 
		0xcd00, 
		0xce00, 
		0x311b, 
		0x4400, 
		0xcc40, 
		0xd1c0, 
		0x8000, 
		0x5320, 
		0x2390, 
		0xc980, 
		0x8000, 
		0xcc00, 
		0x4fc0, 
		0x4500, 
		0xd300, 
		0xf778, 
		0x50c0, 
		0xcb00, 
		0xd360, 
		0x0000, 
		0x2999, 
		0x0cb2, 
		0x51a0, 
		0x4dc0, 
		0xd3a0, 
		0x5240, 
		0x4b7d, 
		0x5140, 
		0xcd00, 
		0xd380, 
		0x52c0, 
		0xc700, 
		0x5220, 
		0x5360, 
		0x5180, 
		0x52e0, 
		0x5060, 
		0x8000, 
		0x4f80, 
		0x262e, 
		0xef2f, 
		0x4d00, 
		0x51a0, 
		0xcf40, 
		0x5200, 
		0xc4d2, 
		0xd1c0, 
		0xcdc0, 
		0xd320, 
		0xc8fd, 
		0xcb00, 
		0x5a0e, 
		0xc600, 
		0xcfc0, 
		0x2810, 
		0xcec0, 
		0xce40, 
		0xd080, 
		0x5120, 
		0xcd40, 
		0x4800, 
		0x8ee0, 
		0x4980, 
		0x5080, 
		0xd120, 
		0xb761, 
		0xd080, 
		0x8578, 
		0xdad1, 
		0x33a0, 
		0x012b, 
		0x5040, 
		0x0a70, 
		0x52e0, 
		0xd020, 
		0x4cc0, 
		0xcfc0, 
		0xc700, 
		0xcc40, 
		0x5e16, 
		0xd2e0, 
		0xcd80, 
		0x6f0f, 
		0x22ba, 
		0x4400, 
		0x4e80, 
		0x5efa, 
		0xd000, 
		0xb3ca, 
		0x5300, 
		0xce40, 
		0x3ab2, 
		0x8bd0, 
		0xfe30, 
		0xd3a0, 
		0x88ad, 
		0x5180, 
		0x4980, 
		0xcc80, 
		0x4c40, 
		0xce00, 
		0xdcf8, 
		0xc000, 
		0x5d33, 
		0xd240, 
		0x5743, 
		0xc700, 
		0xd040, 
		0xcdc0, 
		0xecd4, 
		0xce40, 
		0x3c00, 
		0x53c0, 
		0x4500, 
		0xd260, 
		0x50c0, 
		0x4b00, 
		0x994d, 
		0x5360, 
		0xc84a, 
		0x3c00, 
		0xd3e0, 
		0x6242, 
		0x4a80, 
		0x9f23, 
		0x785e, 
		0x8c8e, 
		0x0000, 
		0x30bf, 
		0x7c00, 
		0xd0a0, 
		0x9a97, 
		0x5100, 
		0x51e0, 
		0xc700, 
		0xd180, 
		0x4f80, 
		0x4500, 
		0xc880, 
		0x41b2, 
		0x96c1, 
		0x4d00, 
		0x5300, 
		0x71c3, 
		0x0421, 
		0xfc00, 
		0x5080, 
		0xcd40, 
		0xc000, 
		0xbc0a, 
		0x2805, 
		0xd0e0, 
		0x0e2c, 
		0xc200, 
		0x52c0, 
		0xeb3e, 
		0xcf40, 
		0xb1ed, 
		0xc78b, 
		0x0000, 
		0xcdc0, 
		0x5a51, 
		0xc800, 
		0xcdc4, 
		0x5020, 
		0x4f40, 
		0xd380, 
		0xc700, 
		0x5462, 
		0xc400, 
		0xc9d6, 
		0xc600, 
		0x584c, 
		0x4c40, 
		0x7c82, 
		0x7c00, 
		0xd160, 
		0x4de7, 
		0x017f, 
		0x8000, 
		0x4600, 
		0x4500, 
		0xce80, 
		0x92d5, 
		0x52c0, 
		0xcc00, 
		0xc880, 
		0xd2c0, 
		0x635e, 
		0xd0e0, 
		0x8000, 
		0x4f80, 
		0xc000, 
		0xfc00, 
		0x116d, 
		0x7ba5, 
		0x9ec7, 
		0x4000, 
		0x4072, 
		0xd0e0, 
		0x4dc0, 
		0xc700, 
		0xd200, 
		0x4bfe, 
		0x855f, 
		0x53c0, 
		0x0cbb, 
		0xdf62, 
		0x5240, 
		0xd200, 
		0x7c00, 
		0x5140, 
		0xce80, 
		0x4ec0, 
		0xd120, 
		0xcf80, 
		0x5140, 
		0xac07, 
		0xc980, 
		0x21d7, 
		0x0000, 
		0xa442, 
		0xdc23, 
		0x0319, 
		0xcc80, 
		0x832b, 
		0xd0c0, 
		0xd200, 
		0xce00, 
		0x50c0, 
		0xb37e, 
		0xdd5a, 
		0xf4fa, 
		0x5180, 
		0x4c40, 
		0x8000, 
		0xb647, 
		0x1535, 
		0x8fb0, 
		0xc980, 
		0xc980, 
		0x50c0, 
		0xe6c3, 
		0xcf40, 
		0xd180, 
		0x7508, 
		0x0000, 
		0x1c31, 
		0x5320, 
		0xd260, 
		0x22f0, 
		0xd380, 
		0xca80, 
		0x4900, 
		0xd100, 
		0x1d16, 
		0x8000, 
		0x4ec0, 
		0x5120, 
		0x8000, 
		0xcb80, 
		0x71a9, 
		0x4980, 
		0xa7c3, 
		0xd200, 
		0x4200, 
		0xc880, 
		0x52d8, 
		0x04a0, 
		0x4c80, 
		0x8342, 
		0x52c0, 
		0xc500, 
		0x7c00, 
		0xcfc0, 
		0xe697, 
		0xcd00, 
		0xfc3d, 
		0x51c0, 
		0xfc00, 
		0x5000, 
		0xc800, 
		0x4a00, 
		0x3707, 
		0xd300, 
		0xcc00, 
		0x4b80, 
		0xc600, 
		0x5177, 
		0x441e, 
		0xd080, 
		0x9e8c, 
		0xd280, 
		0x5340, 
		0x5120, 
		0x4a80, 
		0x0000, 
		0xa242, 
		0x5280, 
		0xd320, 
		0x050d, 
		0x30d1, 
		0xd040, 
		0xccc0, 
		0x50c0, 
		0x73bd, 
		0x285b, 
		0x8000, 
		0x5a71, 
		0x7721, 
		0xd1a0, 
		0x19be, 
		0xd300, 
		0xce00, 
		0xd100, 
		0x4cc0, 
		0x4c40, 
		0x4cc0, 
		0x3ebc, 
		0x4980, 
		0x5160, 
		0xccf1, 
		0xd0c0, 
		0x0000, 
		0xbc00, 
		0xe015, 
		0xd3e0, 
		0x4800, 
		0x4b80, 
		0x98e6, 
		0x7c89, 
		0x7c1f, 
		0xd180, 
		0x8000, 
		0xd1c0, 
		0xcf80, 
		0x5040, 
		0x2447, 
		0x4e40, 
		0x8000, 
		0xaec1, 
		0xcd00, 
		0xc600, 
		0xc700, 
		0xce40, 
		0xd3c0, 
		0xae29, 
		0x5220, 
		0x5220, 
		0xf68b, 
		0x4f80, 
		0xfc00, 
		0x5877, 
		0xd180, 
		0x0000, 
		0xce40, 
		0xbfe3, 
		0x8000, 
		0xa34d, 
		0xb7e8, 
		0xd731, 
		0xfb5e, 
		0xd1c0, 
		0x9444, 
		0xfc00, 
		0x22b2, 
		0x4cc0, 
		0x2d87, 
		0xd160, 
		0x4b80, 
		0x4cc0, 
		0xb16e, 
		0xfc00, 
		0xcd40, 
		0x5060, 
		0x50a0, 
		0xce40, 
		0x5320, 
		0x06b9, 
		0x51c0, 
		0x7c00, 
		0xa413, 
		0x0a5d, 
		0x50e0, 
		0xfdab, 
		0x5000, 
		0x5020, 
		0x4980, 
		0x7c00, 
		0x4c00, 
		0xd360, 
		0xcc00, 
		0x3578, 
		0xcf00, 
		0xd66f, 
		0x51a0, 
		0x5220, 
		0x0eaa, 
		0x5140, 
		0x4900, 
		0xc5a9, 
		0x0000, 
		0xfc00, 
		0x9b90, 
		0x5300, 
		0x53c0, 
		0xc000, 
		0x0000, 
		0xfab9, 
		0xc700, 
		0xd060, 
		0xd060, 
		0x4b80, 
		0x4f00, 
		0xc900, 
		0x4a80, 
		0x0242, 
		0x4700, 
		0xf039, 
		0x6c34, 
		0x9e13, 
		0x4dc0, 
		0xad89, 
		0xcd40, 
		0xc800, 
		0xc000, 
		0xce00, 
		0x50e0, 
		0x4fc0, 
		0xd0e0, 
		0xd1c0, 
		0x9436, 
		0xc500, 
		0xde56, 
		0x51c0, 
		0x4e40, 
		0xd2c0, 
		0x7c00, 
		0xc500, 
		0x5280, 
		0x2330, 
		0x50a0, 
		0xcec0, 
		0xd0e0, 
		0x9f84, 
		0x4700, 
		0x52e0, 
		0x7386, 
		0xd120, 
		0x7c00, 
		0xd2e0, 
		0x7c00, 
		0xcdc0, 
		0x171d, 
		0x10cf, 
		0x01a9, 
		0xcd40, 
		0xcec0, 
		0xce00, 
		0x4cc0, 
		0xd2e0, 
		0x6dca, 
		0xd040, 
		0xd200, 
		0x7909, 
		0xc700, 
		0x4cc0, 
		0xad86, 
		0xccc0, 
		0x4f00, 
		0x7ccb, 
		0x4cc0, 
		0xd99c, 
		0x4400, 
		0x4b80, 
		0x4165, 
		0xd020, 
		0x7c00, 
		0x53c0, 
		0xd1e0, 
		0xca80, 
		0x7ccf, 
		0x5380, 
		0xd060, 
		0xce66, 
		0xd380, 
		0x5180, 
		0xd320, 
		0xcd40, 
		0x4600, 
		0x5140, 
		0x5320, 
		0x9871, 
		0x12fd, 
		0x3c25, 
		0x4500, 
		0x8297, 
		0x8d80, 
		0xdbc5, 
		0x5140, 
		0xd180, 
		0xa461, 
		0xd220, 
		0x4b00, 
		0x53e0, 
		0xc500, 
		0xa0a8, 
		0x7c07, 
		0x51c0, 
		0x5080, 
		0xc800, 
		0xd3e0, 
		0xd2a0, 
		0xcf80, 
		0x7c00, 
		0x3c00, 
		0xd320, 
		0x53c0, 
		0x64b0, 
		0xd180, 
		0x4200, 
		0x5240, 
		0x7c00, 
		0xc880, 
		0x4900, 
		0x531a, 
		0x7c00, 
		0x4880, 
		0x1e7a, 
		0x8f3f, 
		0x2fc3, 
		0xd0a0, 
		0x4400, 
		0xd0a0, 
		0x4c80, 
		0x7c00, 
		0xd0e0, 
		0xd060, 
		0x4e80, 
		0xd2e0, 
		0xce00, 
		0xcec0, 
		0x8000, 
		0x8000, 
		0x1f78, 
		0x8000, 
		0x4ca7, 
		0x7c00, 
		0xd160, 
		0x6f28, 
		0xee90, 
		0xfc00, 
		0x7c46, 
		0x1e5a, 
		0xc800, 
		0xd220, 
		0x8000, 
		0xb7cb, 
		0x4f40, 
		0x8534, 
		0x0000, 
		0x52a0, 
		0xcb00, 
		0x5340, 
		0x5140, 
		0x3761, 
		0xcc00, 
		0x50a0, 
		0x4980, 
		0x4dc0, 
		0x90de, 
		0x8000, 
		0x5220, 
		0x5140, 
		0x51c0, 
		0x4f00, 
		0xd240, 
		0xafd3, 
		0x440a, 
		0xd1c0, 
		0x4a80, 
		0x5340, 
		0xcaf1, 
		0xfc00, 
		0xd2e0, 
		0xc67f, 
		0x5300, 
		0xd360, 
		0x51c0, 
		0xcd40, 
		0x4175, 
		0x0000, 
		0xab02, 
		0xadef, 
		0x8000, 
		0x4c80, 
		0xca00, 
		0x7c00, 
		0x4d80, 
		0x662e, 
		0x52a0, 
		0x43db, 
		0x9b56, 
		0x7c00, 
		0x473b, 
		0x4d00, 
		0xcfc0, 
		0xcd31, 
		0xd080, 
		0xd380, 
		0xf7c7, 
		0x4e40, 
		0x92ba, 
		0x51c0, 
		0x5100, 
		0xd0c0, 
		0x5040, 
		0x4e80, 
		0xe792, 
		0x4500, 
		0x5060, 
		0xfca6, 
		0x3574, 
		0xa5cd, 
		0x0000, 
		0xfc00, 
		0xf448, 
		0x8357, 
		0xd4b5, 
		0xefc3, 
		0xc33a, 
		0x5180, 
		0xcf00, 
		0xc500, 
		0xfc00, 
		0x2ebb, 
		0xd2e0, 
		0x4800, 
		0x0000, 
		0xceeb, 
		0xd0a0, 
		0x5b67, 
		0x7423, 
		0xd6d2, 
		0xe66a, 
		0xfc00, 
		0x1171, 
		0x7c00, 
		0xce80, 
		0x0000, 
		0x4ec0, 
		0x5340, 
		0xc43d, 
		0xca00, 
		0x0000, 
		0x212e, 
		0x9e3a, 
		0x5080, 
		0x7ce5, 
		0x3980, 
		0xd360, 
		0x3590, 
		0x697d, 
		0xd5ea, 
		0xdb86, 
		0x50c3, 
		0x5260, 
		0x5080, 
		0x3f1d, 
		0xd0c0, 
		0xcb06, 
		0x0d6b, 
		0x52a0, 
		0xd160, 
		0xfc3c, 
		0xcc40, 
		0xc880, 
		0x902e, 
		0xf41f, 
		0x136f, 
		0x51c0, 
		0xf95a, 
		0x5380, 
		0xd1a0, 
		0xc400, 
		0xb102, 
		0x0000, 
		0x9b40, 
		0x4c40, 
		0x3c00, 
		0x4ec0, 
		0xfc1d, 
		0xd120, 
		0x8320, 
		0x4e80, 
		0x7d70, 
		0x5160, 
		0xfc00, 
		0x1fa0, 
		0x5200, 
		0x4a00, 
		0x3e4a, 
		0x71e2, 
		0x5260, 
		0xd092, 
		0xd2a0, 
		0x6bf3, 
		0x8000, 
		0xc880, 
		0x13a2, 
		0xd0e0, 
		0x4400, 
		0x4600, 
		0x4f80, 
		0xd080, 
		0x8000, 
		0x4500, 
		0x5360, 
		0x53a0, 
		0x173b, 
		0xc726, 
		0xd2c0, 
		0x4880, 
		0xc800, 
		0x02d3, 
		0xcd40, 
		0xd808, 
		0x6e08, 
		0x4e80, 
		0xcec0, 
		0xd3e8, 
		0xc600, 
		0x60f5, 
		0x4980, 
		0x5060, 
		0x50e0, 
		0x524f, 
		0xd060, 
		0x8000, 
		0x4880, 
		0x4b00, 
		0xbbb2, 
		0x6c3d, 
		0x0000, 
		0xd0e0, 
		0x4dc0, 
		0xdc8a, 
		0xd0a0, 
		0xd1a0, 
		0x4200, 
		0x5200, 
		0x4d00, 
		0x4d00, 
		0xe900, 
		0x5280, 
		0x8000, 
		0xc600, 
		0xd2c0, 
		0x8000, 
		0xc8a6, 
		0x4400, 
		0x52c0, 
		0xc87d, 
		0x4d00, 
		0xc400, 
		0xcd00, 
		0x7dc1, 
		0xd300, 
		0x00e0, 
		0xd020, 
		0xcf00, 
		0x52e0, 
		0x52c0, 
		0xce40, 
		0xad08, 
		0x4980, 
		0xd448, 
		0x8000, 
		0x4880, 
		0xcf80, 
		0xc500, 
		0x4880, 
		0x4880, 
		0xc800, 
		0xcf40, 
		0xfc00, 
		0x2537, 
		0x5280, 
		0xf3bb, 
		0x5160, 
		0xd160, 
		0x4200, 
		0xd160, 
		0x6968, 
		0xf0c3, 
		0xd280, 
		0x4d00, 
		0x8894, 
		0x0189, 
		0xcd00, 
		0xc880, 
		0x2810, 
		0xd1c0, 
		0xd300, 
		0x51c0, 
		0x6af3, 
		0x51e0, 
		0x4d40, 
		0x2b0c, 
		0xd300, 
		0x0000, 
		0x7177, 
		0xccc0, 
		0x5020, 
		0x4600, 
		0xcd40, 
		0xd180, 
		0x4f80, 
		0x4200, 
		0x5300, 
		0x4ad9, 
		0x8079, 
		0x39c3, 
		0x4b80, 
		0x4800, 
		0x51c0, 
		0x5380, 
		0xd300, 
		0xc800, 
		0x4880, 
		0xce72, 
		0x4b00, 
		0xd320, 
		0xc400, 
		0x4f80, 
		0x9056, 
		0xeaed, 
		0x4880, 
		0xcc40, 
		0x4a00, 
		0x5060, 
		0x5ff0, 
		0x53a0, 
		0x4dc0, 
		0x0000, 
		0x65ff, 
		0xb737, 
		0xcfc0, 
		0xd1c0, 
		0xcd40, 
		0x51d6, 
		0x8000, 
		0xd1a0, 
		0x5080, 
		0xd3a0, 
		0xd180, 
		0x50c0, 
		0x341a, 
		0x40ef, 
		0x53a0, 
		0xc000, 
		0x1f56, 
		0x4e40, 
		0xd0a0, 
		0x5040, 
		0xd3a0, 
		0x1960, 
		0xcd80, 
		0x8000, 
		0x166f, 
		0xcb00, 
		0x5380, 
		0xfc8d, 
		0x4f80, 
		0x97a5, 
		0xc200, 
		0xc980, 
		0xd240, 
		0x0000, 
		0x52c0, 
		0x4900, 
		0xf648, 
		0x5da2, 
		0x4600, 
		0xd2c0, 
		0xd380, 
		0xd280, 
		0x4880, 
		0xd020, 
		0xd200, 
		0x52a0, 
		0xc400, 
		0xd1a0, 
		0xd100, 
		0x7c00, 
		0x4fc0, 
		0xfc00, 
		0x7c00, 
		0xca00, 
		0xc900, 
		0x4dc0, 
		0x33d4, 
		0x52bb, 
		0x4ec0, 
		0x415d, 
		0xebe3, 
		0xd3c0, 
		0x4980, 
		0xc600, 
		0x5260, 
		0xd280, 
		0xb6a8, 
		0x54dd, 
		0x4b00, 
		0xc000, 
		0xd240, 
		0xcb80, 
		0xccc0, 
		0x5380, 
		0xb2d2, 
		0xe9ee, 
		0x0000, 
		0x51e0, 
		0xd380, 
		0x5e5f, 
		0xd506, 
		0xd140, 
		0xca80, 
		0xd1a0, 
		0x0000, 
		0xca80, 
		0x8f32, 
		0xd180, 
		0x4900, 
		0x20b3, 
		0xd280, 
		0x1055, 
		0xd3a0, 
		0x4cc0, 
		0x5140, 
		0xc200, 
		0x5100, 
		0x52a0, 
		0x8bb6, 
		0x5020, 
		0x5020, 
		0x4800, 
		0x8000, 
		0xd0c0, 
		0x94c1, 
		0x7f5c, 
		0xdc12, 
		0x50e0, 
		0xd0e0, 
		0x4a00, 
		0xca00, 
		0x4e00, 
		0xd060, 
		0x84c6, 
		0x2301, 
		0xbc00, 
		0xc200, 
		0x22e6, 
		0x4f00, 
		0x50c5, 
		0x4e80, 
		0xcd40, 
		0x4c00, 
		0x4700, 
		0x5360, 
		0xae18, 
		0x0d2d, 
		0x5120, 
		0x7a85, 
		0x5360, 
		0x4e40, 
		0x4b80, 
		0xfc00, 
		0x92a3, 
		0xcc00, 
		0x52e0, 
		0x8000, 
		0x7c00, 
		0x5100, 
		0xe7b6, 
		0x65d3, 
		0xcc80, 
		0x4c40, 
		0xd1c0, 
		0x5a7b, 
		0x50c0, 
		0x8000, 
		0xc200, 
		0x4c80, 
		0xce80, 
		0xcf00, 
		0x0000, 
		0xd2a0, 
		0x2ca8, 
		0x198d, 
		0x9391, 
		0x5340, 
		0x63c3, 
		0x9a6b, 
		0xd040, 
		0x5240, 
		0x7bd6, 
		0x8506, 
		0x2cb1, 
		0x0b4a, 
		0xd1e0, 
		0xc900, 
		0x3c00, 
		0x2cea, 
		0xd120, 
		0xd340, 
		0x50c0, 
		0xcc00, 
		0x5240, 
		0x5080, 
		0xd320, 
		0x3c00, 
		0x76ea, 
		0xd020, 
		0xd120, 
		0x5000, 
		0xed3b, 
		0x4f80, 
		0x7761, 
		0xd1a0, 
		0xd220, 
		0x2de2, 
		0x3100, 
		0x4a00, 
		0x5120, 
		0xd280, 
		0x4f40, 
		0x4880, 
		0x8000, 
		0x0000, 
		0xcc80, 
		0x50e0, 
		0xaf9a, 
		0xcf80, 
		0xcfc0, 
		0xc900, 
		0xca80, 
		0x7c00, 
		0xf7d3, 
		0x322c, 
		0xd2e0, 
		0x4e47, 
		0x4200, 
		0x9a19, 
		0xd140, 
		0x6afb, 
		0x0000, 
		0xd240, 
		0x56d7, 
		0xd300, 
		0x4e40, 
		0x50c0, 
		0xc000, 
		0xdb7d, 
		0xcb00, 
		0xd160, 
		0x7a99, 
		0xf839, 
		0xbc00, 
		0xd280, 
		0xc8db, 
		0x4cc0, 
		0x1669, 
		0xb8de, 
		0x6406, 
		0xd280, 
		0x2212, 
		0x4f40, 
		0x61da, 
		0xfa0c, 
		0x53c0, 
		0x5300, 
		0xd200, 
		0xd260, 
		0x3cb0, 
		0xe6e3, 
		0x4e40, 
		0x4d00, 
		0x51a0, 
		0x9a8b, 
		0x5320, 
		0x4c80, 
		0xca00, 
		0x4b80, 
		0x435d, 
		0xd060, 
		0xc213, 
		0x5380, 
		0x7c00, 
		0x32cf, 
		0xd380, 
		0x5260, 
		0x51a0, 
		0xdec4, 
		0xccc0, 
		0x5360, 
		0x7c2b, 
		0xba7b, 
		0x4d00, 
		0x0000, 
		0xce4a, 
		0xca36, 
		0xcd00, 
		0x5080, 
		0x4c40, 
		0xcb80, 
		0xd100, 
		0xb216, 
		0x5180, 
		0xfc8f, 
		0xb671, 
		0xfd7c, 
		0x4800, 
		0xd100, 
		0x7d1b, 
		0x5080, 
		0x8cc6, 
		0x4700, 
		0x20e1, 
		0x52e0, 
		0x4b80, 
		0x67b4, 
		0x52e0, 
		0x4500, 
		0xfc00, 
		0x4980, 
		0x0857, 
		0x4d00, 
		0x4400, 
		0x5120, 
		0x186e, 
		0x4e40, 
		0x13eb, 
		0x5120, 
		0x5280, 
		0x5320, 
		0x53a0, 
		0xcdc0, 
		0x749c, 
		0xcc40, 
		0x8000, 
		0x8000, 
		0x8000, 
		0xd220, 
		0xca80, 
		0xf032, 
		0x7c00, 
		0xce40, 
		0x4d00, 
		0x0000, 
		0x52a0, 
		0xafd7, 
		0x9d14, 
		0xd0c0, 
		0x8000, 
		0xccc0, 
		0xfc00, 
		0xc900, 
		0xc3b9, 
		0x0000, 
		0xd040, 
		0xc900, 
		0xd2a0, 
		0xca00, 
		0x4f1c, 
		0x4a80, 
		0x5060, 
		0xcb00, 
		0x5300, 
		0x557a, 
		0x4400, 
		0x5509, 
		0xd0a0, 
		0x4e80, 
		0xfc00, 
		0xb6bc, 
		0x6359, 
		0xd100, 
		0x328d, 
		0xd1c0, 
		0x5020, 
		0x8000, 
		0x0000, 
		0xd1a0, 
		0x4ec0, 
		0xca80, 
		0xd220, 
		0x8000, 
		0xd180, 
		0x54bf, 
		0x9ba1, 
		0x5380, 
		0xf83f, 
		0x4d00, 
		0x4a80, 
		0x4ec0, 
		0x52a0, 
		0xd100, 
		0xce00, 
		0xd000, 
		0x4f40, 
		0xf90a, 
		0xc200, 
		0xf4ac, 
		0xd280, 
		0x5380, 
		0xc9fe, 
		0x7462, 
		0x5200, 
		0xd3c0, 
		0x53e0, 
		0xce31, 
		0x5100, 
		0x8000, 
		0x5340, 
		0xd360, 
		0x5340, 
		0xfa8a, 
		0xc600, 
		0xd0c3, 
		0x50a0, 
		0x04d7, 
		0x0000, 
		0x7c00, 
		0x7868, 
		0xcdc0, 
		0x8000, 
		0xd3a0, 
		0x4980, 
		0x5100, 
		0x8000, 
		0xd280, 
		0x7c0e, 
		0x10d8, 
		0x5040, 
		0xfc00, 
		0x8e63, 
		0x4498, 
		0x51c0, 
		0x0000, 
		0x52c0, 
		0xfca5, 
		0xcf40, 
		0xc700, 
		0x51a0, 
		0x5000, 
		0x1499, 
		0xca00, 
		0x4d00, 
		0xbc00, 
		0x81a7, 
		0xc600, 
		0x0000, 
		0x52c0, 
		0x4f80, 
		0x0000, 
		0x4f80, 
		0x825c, 
		0x52c0, 
		0x5380, 
		0x0000, 
		0x8000, 
		0x1c48, 
		0x4900, 
		0x5451, 
		0xd020, 
		0x4880, 
		0x1939, 
		0x5120, 
		0xcec0, 
		0x407e, 
		0x4429, 
		0x4600, 
		0xc200, 
		0xa2ae, 
		0x52e0, 
		0xcd40, 
		0x4980, 
		0xd280, 
		0x8000, 
		0x9da2, 
		0xc500, 
		0xa111, 
		0x35ab, 
		0x0c49, 
		0xc600, 
		0xd3e0, 
		0xd2c0, 
		0x8203, 
		0x92ae, 
		0xfc00, 
		0xd300, 
		0x0df0, 
		0x4c00, 
		0x0000, 
		0x0000, 
		0xd0e0, 
		0x4f80, 
		0x4f00, 
		0xdf52, 
		0x5280, 
		0x3cb8, 
		0x4c00, 
		0xc600, 
		0x5b7d, 
		0xcf00, 
		0x4b00, 
		0x4980, 
		0x0b80, 
		0x4900, 
		0xca00, 
		0xd1a0, 
		0xfd3f, 
		0x957f, 
		0xc000, 
		0xc417, 
		0x52e0, 
		0x247a, 
		0xa2d2, 
		0xfc2e, 
		0x50a0, 
		0xd160, 
		0x8000, 
		0x716f, 
		0x8f05, 
		0xcf80, 
		0x56f1, 
		0xa632, 
		0x5160, 
		0x5280, 
		0x52e0, 
		0xfc00, 
		0x5160, 
		0x5380, 
		0xcf40, 
		0xb0ca, 
		0xd260, 
		0x0000, 
		0xd1a0, 
		0xd200, 
		0x5080, 
		0x4cc0, 
		0x4cc0, 
		0x243f, 
		0xf0b2, 
		0x52c0, 
		0x4e00, 
		0xd060, 
		0x7517, 
		0xc000, 
		0xca80, 
		0xd3e0, 
		0x5240, 
		0x4dc0, 
		0xd0a0, 
		0xcfc0, 
		0x4b00, 
		0xd824, 
		0x4eb3, 
		0x9ed6, 
		0x4f40, 
		0xca00, 
		0x4d4e, 
		0x807e, 
		0x4ec0, 
		0x50e0, 
		0xd2c0, 
		0x3c00, 
		0x4b00, 
		0xd3e0, 
		0x4f80, 
		0x4cf7, 
		0xcc80, 
		0xc200, 
		0xe6dc, 
		0x5986, 
		0x4f40, 
		0x9b33, 
		0xd3c0, 
		0xd040, 
		0x3f7d, 
		0xce40, 
		0x4800, 
		0xd280, 
		0x4dc0, 
		0x8000, 
		0x5280, 
		0xc57a, 
		0x117d, 
		0xd1e0, 
		0x3365, 
		0xcb00, 
		0xfc00, 
		0x8360, 
		0x51c0, 
		0xfc8d, 
		0xcdc0, 
		0x5160, 
		0x52a0, 
		0xd360, 
		0x4f00, 
		0x8000, 
		0xcd40, 
		0xcd80, 
		0xa4ef, 
		0xd0c0, 
		0x6ef0, 
		0x4200, 
		0x4f00, 
		0xce00, 
		0xcdc0, 
		0x53e0, 
		0x0e95, 
		0xce40, 
		0x5020, 
		0x7c00, 
		0xca24, 
		0xcf00, 
		0x2935, 
		0x6353, 
		0xc880, 
		0xd020, 
		0x53e0, 
		0xc700, 
		0xd140, 
		0xcd00, 
		0x4900, 
		0x0000, 
		0xd100, 
		0x4d80, 
		0x0000, 
		0xef4e, 
		0x1d42, 
		0x2d7e, 
		0xd2a0, 
		0x4000, 
		0x5d2c, 
		0xf0d2, 
		0xcb00, 
		0xc700, 
		0xd100, 
		0xcf80, 
		0x4f40, 
		0x4980, 
		0xfd88, 
		0xfd82, 
		0x2516, 
		0x7c00, 
		0xce40, 
		0x5240, 
		0x4f00, 
		0x4ec0, 
		0xd360, 
		0x5360, 
		0xd304, 
		0x9964, 
		0x3bad, 
		0x5380, 
		0x3249, 
		0x5020, 
		0x5180, 
		0xb660, 
		0xe802, 
		0x5000, 
		0xc700, 
		0x7d2c, 
		0x8343, 
		0x52e0, 
		0x15a6, 
		0xc000, 
		0x5100, 
		0xccc0, 
		0x5220, 
		0x4cc0, 
		0x4dc0, 
		0x4e27, 
		0x5300, 
		0xfc00, 
		0x4000, 
		0x5300, 
		0xd240, 
		0x7c00, 
		0xd1a0, 
		0x5564, 
		0x4a80, 
		0x0000, 
		0x4b40, 
		0x035c, 
		0x0000, 
		0x4e80, 
		0x5000, 
		0xd160, 
		0x5020, 
		0xabac, 
		0x4f00, 
		0x4fc0, 
		0x7009, 
		0x5140, 
		0xe418, 
		0x4e80, 
		0x4400  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x0e07ffff, 
		0xc1e80000, 
		0x00001000, 
		0x0c7ff000, 
		0xc1900000, 
		0xa396ce1c, 
		0x0c7e0000, 
		0x41c80000, 
		0xc7c11a37, 
		0x42200000, 
		0x822122ec, 
		0x3ce8af0a, 
		0x7f7ffffe, 
		0x00000004, 
		0x0e3fffff, 
		0x80000000, 
		0x0e00ffff, 
		0x0f7ffffe, 
		0x90863662, 
		0x80000000, 
		0x00800003, 
		0xc20c0000, 
		0x5e5239d4, 
		0x7f7ffffe, 
		0x0e0001ff, 
		0xf30fa941, 
		0xc1b80000, 
		0x7f7ffffe, 
		0xb876786c, 
		0x0e007fff, 
		0x0f7ffffc, 
		0xeb0e050d, 
		0x80000000, 
		0x00000002, 
		0x0c7fffff, 
		0x80010000, 
		0x7fc00001, 
		0x7f800000, 
		0x7fbfffff, 
		0x00080000, 
		0xc22c0000, 
		0x0c7ff000, 
		0x41880000, 
		0xdb61a4cc, 
		0x42500000, 
		0x0c7c0000, 
		0xc1700000, 
		0x80000000, 
		0x00000001, 
		0x42340000, 
		0x41500000, 
		0xc1500000, 
		0x42240000, 
		0xc21c0000, 
		0xc1a80000, 
		0x00800001, 
		0x0c7ff000, 
		0x80000008, 
		0xc8df0035, 
		0xc2080000, 
		0xee8e51b0, 
		0x0c7fffc0, 
		0xc2400000, 
		0xa597e40c, 
		0x3acaf781, 
		0x7f7ffffe, 
		0x6742182e, 
		0x0f2886b3, 
		0xff800000, 
		0xb574f60f, 
		0x793b8956, 
		0x00000004, 
		0xc2100000, 
		0x216e80fb, 
		0xd8cedff5, 
		0x0c600000, 
		0xff000000, 
		0x8054908c, 
		0xffc00000, 
		0xc1d00000, 
		0x7b7ae685, 
		0x0e1fffff, 
		0x55d91c8f, 
		0xc232a6f6, 
		0x0e0fffff, 
		0xc1900000, 
		0xeb8a71b0, 
		0x42180000, 
		0x7f800000, 
		0xab27ff7d, 
		0x160b56db, 
		0xc2380000, 
		0xef0d1de0, 
		0x7dec8826, 
		0x00400000, 
		0x42280000, 
		0x00000008, 
		0x40c00000, 
		0x00000400, 
		0x80000001, 
		0x0c7fffff, 
		0x7b790307, 
		0x0c780000, 
		0x00200000, 
		0xc26c0000, 
		0x007fffff, 
		0x39a4b3ca, 
		0x41e80000, 
		0xc2440000, 
		0x427c0000, 
		0x80000000, 
		0xf34517dd, 
		0x80000800, 
		0x0c600000, 
		0xff800000, 
		0xc2440000, 
		0xe8251397, 
		0x410d122c, 
		0x8f7ffffe, 
		0x80000100, 
		0xba3b1bf4, 
		0xc2380000, 
		0x42600000, 
		0x5fd78305, 
		0x1b3f1ac0, 
		0x5db90a39, 
		0x00000000, 
		0xff000000, 
		0x42180000, 
		0x4b8c4b40, 
		0x41100000, 
		0x1e6cc52f, 
		0x41c80000, 
		0x27d0b0fe, 
		0xd370eb98, 
		0x80000000, 
		0x0c7fffe0, 
		0x00000800, 
		0x40c00000, 
		0x42400000, 
		0x0c7ffe00, 
		0xff800000, 
		0xc25c0000, 
		0xc2540000, 
		0x40c00000, 
		0x80000100, 
		0x42040000, 
		0xff8d450d, 
		0x80544cd1, 
		0xc1200000, 
		0x5e4cb0d4, 
		0x7f7ffffe, 
		0x0c7ffc00, 
		0x7372e20e, 
		0x80800003, 
		0xa6507796, 
		0x80000000, 
		0xc2080000, 
		0xc2380000, 
		0xc2340000, 
		0x80000000, 
		0xc2180000, 
		0xd1f3847a, 
		0x80000000, 
		0x00000000, 
		0x41f00000, 
		0x66a19a7d, 
		0x0e00ffff, 
		0xc1d00000, 
		0x20383c76, 
		0xe7396212, 
		0xc2240000, 
		0xc422a70e, 
		0x00000000, 
		0x80000000, 
		0xc2100000, 
		0x80000000, 
		0x42100000, 
		0x0e007fff, 
		0x7f7ffffe, 
		0x00000000, 
		0xc1700000, 
		0x9c71931a, 
		0x80800001, 
		0x80000000, 
		0x80000080, 
		0xffbfffff, 
		0xf617b2b2, 
		0x0e3fffff, 
		0xe7a035fd, 
		0x7f9152d9, 
		0x59593ef1, 
		0x42780000, 
		0x80800000, 
		0x80000000, 
		0x42480000, 
		0xb90da23e, 
		0x80800002, 
		0xff800001, 
		0x00000010, 
		0x40400000, 
		0x0c700000, 
		0x00000001, 
		0xcb000000, 
		0x80000000, 
		0x332b9a80, 
		0x00200000, 
		0xc4ae5a4d, 
		0xa5b59470, 
		0x00800001, 
		0x2a38e01f, 
		0xffffffff, 
		0xd075f76d, 
		0x42700000, 
		0x42000000, 
		0xbfa11ae7, 
		0x42500000, 
		0xc1500000, 
		0xff00c8cf, 
		0xd28fcf48, 
		0x80547d48, 
		0x80800001, 
		0xc1980000, 
		0x00400000, 
		0x3f028f5c, 
		0x00400000, 
		0x762081f9, 
		0x7f800000, 
		0x41400000, 
		0xc1f00000, 
		0x60829542, 
		0x0c7f8000, 
		0x527d2fc9, 
		0x80800002, 
		0x00000002, 
		0x0e0007ff, 
		0xb20dc051, 
		0x420c0000, 
		0xc1a80000, 
		0xc2580000, 
		0xc9b5846f, 
		0x2aa00113, 
		0xff7ffffe, 
		0x48c16a81, 
		0x41d80000, 
		0x41e00000, 
		0x7f7fffff, 
		0x00000000, 
		0xc1300000, 
		0xc489d388, 
		0x42040000, 
		0x00000010, 
		0x80138f73, 
		0x92c6d362, 
		0x0c7fffc0, 
		0x427c0000, 
		0x42200000, 
		0x50dd0a0d, 
		0x0e0007ff, 
		0x00020000, 
		0xffc00001, 
		0xff800001, 
		0xcb000000, 
		0x6247e901, 
		0xff800000, 
		0x0e00003f, 
		0x47aacf02, 
		0x00010000, 
		0x395865f4, 
		0x0e0007ff, 
		0xc1300000, 
		0xc2440000, 
		0xdd5119bc, 
		0xc22c0000, 
		0x8b7e466d, 
		0xc2480000, 
		0x80000000, 
		0xb8645c8e, 
		0x7fbea272, 
		0xff800000, 
		0xe4001203, 
		0xd1e1084d, 
		0x00000008, 
		0xc1e80000, 
		0x332be670, 
		0x42440000, 
		0x80800002, 
		0x355da6e6, 
		0x426c0000, 
		0xc2640000, 
		0x42440000, 
		0x80000008, 
		0xff800000, 
		0xcb000000, 
		0xc22c0000, 
		0x80000020, 
		0x00000000, 
		0x00008000, 
		0x33333333, 
		0x80000200, 
		0x41400000, 
		0xc27328fd, 
		0x00800001, 
		0x0e0000ff, 
		0x0e0001ff, 
		0x42740000, 
		0x8bca06bb, 
		0xc1300000, 
		0x0e0000ff, 
		0x7f800000, 
		0x0c7ffc00, 
		0x402a2f87, 
		0x80000001, 
		0x42480000, 
		0x80000002, 
		0x41880000, 
		0xc1c00000, 
		0x007ffffe, 
		0xff800000, 
		0x55d0306b, 
		0x0c7fff00, 
		0x5babfd09, 
		0x422c0000, 
		0x80800002, 
		0xc0800000, 
		0xc1900000, 
		0x6eb02154, 
		0x7f7ffffe, 
		0xc2580000, 
		0xf7bd4458, 
		0x0c400000, 
		0x80800000, 
		0xc0e00000, 
		0x58eca4ac, 
		0x00001000, 
		0x807aac75, 
		0x42500000, 
		0xc2280000, 
		0xb50d5875, 
		0x7fa60663, 
		0xa367fb33, 
		0x7f7ffffe, 
		0x00000000, 
		0x6e2c1100, 
		0x7ad5ab45, 
		0xc1d00000, 
		0x00000000, 
		0xc1c80000, 
		0xf583bd81, 
		0xc2340000, 
		0xc1d00000, 
		0x80002000, 
		0x007fffff, 
		0x00000020, 
		0x7f800001, 
		0xc2580000, 
		0x007fffff, 
		0xb835a65a, 
		0x00000000, 
		0x80000000, 
		0x969295f6, 
		0xff800000, 
		0xc1500000, 
		0x80020000, 
		0x422c0000, 
		0x426c0000, 
		0x0e00007f, 
		0x007e9483, 
		0x0c2a0c01, 
		0xff7fffff, 
		0x427c0000, 
		0x0c7ff800, 
		0x41880000, 
		0x7fc00001, 
		0xe0dd537d, 
		0x63bc30b7, 
		0x41a80000, 
		0xc2200000, 
		0x8f7fffff, 
		0x00000020, 
		0x42580000, 
		0x8f7fffff, 
		0x7f800000, 
		0x80000000, 
		0x80000000, 
		0x2ab68293, 
		0x1f47ce53, 
		0xb7217015, 
		0x7f7fffff, 
		0x80022ac2, 
		0x42680000, 
		0xc1100000, 
		0xff000000, 
		0x9f485a09, 
		0x7fc00000, 
		0x80200000, 
		0x0f7ffffd, 
		0x00040000, 
		0xea595e21, 
		0x0d00fff0, 
		0x426c0000, 
		0xffc00001, 
		0x42680000, 
		0xc26c0000, 
		0x36ddac90, 
		0x420c0000, 
		0x8d59140d, 
		0x80000000, 
		0x80000004, 
		0x2c1bd28b, 
		0x4ee81cba, 
		0xc1a80000, 
		0x42040000, 
		0x41100000, 
		0x80000008, 
		0xff000000, 
		0xff800000, 
		0xbf028f5c, 
		0x0f7ffffc, 
		0x454f271b, 
		0xc1980000, 
		0x80000020, 
		0xa6218935, 
		0x4b989f75, 
		0x007fffff, 
		0x8f7ffffe, 
		0x0e7fffff, 
		0xfe32be4c, 
		0x0e00007f, 
		0x0c7fff00, 
		0x579a7adf, 
		0x80004000, 
		0x00000002, 
		0x00000001, 
		0x7f800000, 
		0xff7fffff, 
		0xd6dafab1, 
		0x7ba14408, 
		0x00000040, 
		0x00800003, 
		0xff800000, 
		0xe52c548d, 
		0x00000000, 
		0x00000000, 
		0xc2340000, 
		0xc1d00000, 
		0xc25c0000, 
		0xaaaaaaaa, 
		0xc0c00000, 
		0xed7daf19, 
		0x80000040, 
		0xe4f1c822, 
		0x7a672151, 
		0xa34e173f, 
		0x00020000, 
		0x00000000, 
		0xd437cf08, 
		0x80001000, 
		0x807fffff, 
		0xff800000, 
		0xc24c0000, 
		0xc21c0000, 
		0x41700000, 
		0x0e0001ff, 
		0xff800000, 
		0xc21c0000, 
		0x85afaa0b, 
		0x41100000, 
		0x4d87ae78, 
		0x2668e927, 
		0x5393e0a1, 
		0x91b97b51, 
		0x3f028f5c, 
		0xff800001, 
		0x7f7ffffe, 
		0x00800000, 
		0x2fe66bc3, 
		0x804d93d0, 
		0x42400000, 
		0x7f88a0de, 
		0x42440000, 
		0x8f7ffffc, 
		0x0c7e0000, 
		0xc1f80000, 
		0x424c0000, 
		0x0c7ffc00, 
		0xff7ffffe, 
		0x7f7ffffe, 
		0x2cabf167, 
		0xd467f20f, 
		0x00800001, 
		0x0f7ffffc, 
		0x7f800000, 
		0x5bd8c217, 
		0xba230a70, 
		0x42100000, 
		0x422c0000, 
		0x85e7adc1, 
		0x754755c5, 
		0x0075bd2b, 
		0x42500000, 
		0xf79e7c97, 
		0x00002000, 
		0x00080000, 
		0x94ed0c90, 
		0x817af441, 
		0xc85dac81, 
		0x3870d721, 
		0x4b000000, 
		0xc2300000, 
		0xff800000, 
		0x80000000, 
		0x7f812591, 
		0x7fc00001, 
		0xd3bf282d, 
		0x427c0000, 
		0x421c0000, 
		0xc23c0000, 
		0x0f7ffffc, 
		0x80000100, 
		0x74c4ff8b, 
		0xbefcdcc5, 
		0xb50fd471, 
		0x767e3799, 
		0x1f1c64d0, 
		0x41300000, 
		0x80008000, 
		0x41e00000, 
		0xc1d00000, 
		0x7f7ffffe, 
		0xff800000, 
		0xbbedb016, 
		0x0c700000, 
		0x42440000, 
		0x0f7ffffd, 
		0xc1700000, 
		0xc0c00000, 
		0x00010000, 
		0x0e0003ff, 
		0xcaad389a, 
		0x0e07ffff, 
		0xf4dbd998, 
		0x80800000, 
		0xff7fffff, 
		0xc2640000, 
		0x007fffff, 
		0x00afc65a, 
		0x0c7f0000, 
		0xc4834528, 
		0xc1e80000, 
		0xc22c0000, 
		0x70c9afb3, 
		0x41100000, 
		0x0c7ff000, 
		0x00800003, 
		0x41000000, 
		0x41c80000, 
		0x00000000, 
		0xff800000, 
		0xc2700000, 
		0x42100000, 
		0x92faa1e9, 
		0x00011111, 
		0x00040000, 
		0xc1e00000, 
		0xc1a80000, 
		0x80000000, 
		0x8b6df094, 
		0xc2040000, 
		0x00000000, 
		0x00800003, 
		0x0c7f0000, 
		0xc2680000, 
		0x0c7fffc0, 
		0x3f800000, 
		0x00106298, 
		0x0d000ff0, 
		0x4d6c518b, 
		0xff7ffffe, 
		0x41f00000, 
		0x80000200, 
		0x00200000, 
		0xf7899b00, 
		0x1d0ca593, 
		0xc25c0000, 
		0x0f7ffffe, 
		0xff7ffffe, 
		0x807ffffe, 
		0x00000000, 
		0xe63956dd, 
		0xc1f00000, 
		0x42440000, 
		0xc22c0000, 
		0x5665de86, 
		0x00000002, 
		0x0c7fc000, 
		0x00000000, 
		0x00800001, 
		0xf7bb5862, 
		0xc2140000, 
		0xc27c0000, 
		0x9d4c4c6a, 
		0x58e8a1a6, 
		0xc1500000, 
		0x1dac3a1c, 
		0x42040000, 
		0x80800000, 
		0x7fc00000, 
		0x007fffff, 
		0x09ca31f1, 
		0xc1b00000, 
		0xffffffff, 
		0xc1c80000, 
		0x3529a4ec, 
		0x0e00000f, 
		0xe47389f3, 
		0x80000004, 
		0xc1400000, 
		0x41900000, 
		0x03e3bfac, 
		0xc1c00000, 
		0x0c7ffffe, 
		0x41000000, 
		0x14ea5a5a, 
		0xe2f5de97, 
		0x2c7219ea, 
		0x41100000, 
		0x42140000, 
		0x40000000, 
		0x686b2c68, 
		0x42740000, 
		0x40c00000, 
		0x423c0000, 
		0x7f800000, 
		0x00000800, 
		0x0f7ffffe, 
		0x80000000, 
		0x7f8a81ea, 
		0x00020000, 
		0xff000000, 
		0x00800002, 
		0x41f00000, 
		0x0c7ffff8, 
		0xda26eae8, 
		0x00000000, 
		0x0c7fc000, 
		0x4b000000, 
		0xed2b5e57, 
		0x77c893a1, 
		0x421c0000, 
		0x42440000, 
		0x80800003, 
		0xef5bc16d, 
		0x41800000, 
		0x41900000, 
		0x331e9a66, 
		0x80800000, 
		0x00800001, 
		0x227b3009, 
		0x1c452156, 
		0xc0000000, 
		0xc24c0000, 
		0xffdded4a, 
		0x1cde051e, 
		0x3f94852f, 
		0xc2040000, 
		0x0e0000ff, 
		0x00000008, 
		0x80000000, 
		0xb5a1b19f, 
		0x0f7fffff, 
		0x7f800001, 
		0xc1600000, 
		0xc2700000, 
		0x00000000, 
		0x00000000, 
		0x00000008, 
		0x487e83d9, 
		0x40000000, 
		0xffc00000, 
		0x7f907156, 
		0x19eb3540, 
		0x1638df92, 
		0x426c0000, 
		0xc2300000, 
		0x80020000, 
		0x80000100, 
		0xc2440000, 
		0x0c7ff000, 
		0x80000010, 
		0x0c7f8000, 
		0x0c7fff00, 
		0x6f2393b0, 
		0x7f000000, 
		0x9a9ef2e9, 
		0x7f800001, 
		0x421c0000, 
		0x80800003, 
		0x08cbb8ae, 
		0x00000000, 
		0x000d7187, 
		0x447b5832, 
		0x0e000fff, 
		0xe3573d1d, 
		0x80000000, 
		0x4932faaa, 
		0x00010000, 
		0xff800000, 
		0x42100000, 
		0xdd269bb8, 
		0x80000400, 
		0x7f800000, 
		0xc1200000, 
		0xc1400000, 
		0xff800000, 
		0xc1e00000, 
		0x7f7fffff, 
		0xc0c00000, 
		0x00000000, 
		0x0c7ffffe, 
		0x80800002, 
		0xff800000, 
		0xbb025b30, 
		0x7f800000, 
		0x7fbfffff, 
		0xff7ffffe, 
		0x0c7fc000, 
		0x00000000, 
		0x00000008, 
		0x00002000, 
		0x8f7ffffd, 
		0x007ffffe, 
		0x2b585443, 
		0x7f7fffff, 
		0x7f800000, 
		0x0c7ff000, 
		0xf6409bc7, 
		0x00000002, 
		0xc1a00000, 
		0x00000800, 
		0x5ffd8c4a, 
		0x666ed243, 
		0x0c7fe000, 
		0x62f1e748, 
		0x0e000007, 
		0xf2f03958, 
		0xf53f8c3f, 
		0x41c80000, 
		0x00000002, 
		0x7f800000, 
		0xbf800000, 
		0x7f800000, 
		0x425c0000, 
		0x80000200, 
		0xfaa54ca3, 
		0x80000000, 
		0x4f418eeb, 
		0x019208ee, 
		0xc1f80000, 
		0x80800002, 
		0x41d00000, 
		0x80011111, 
		0x00008000, 
		0x80000020, 
		0x7fffffff, 
		0x42680000, 
		0x80000002, 
		0x8da83153, 
		0x0f7ffffd, 
		0x1423c608, 
		0x40c00000, 
		0x608aa183, 
		0x80000002, 
		0x42540000, 
		0x00040000, 
		0x0f7ffffd, 
		0x42180000, 
		0xdc5945c1, 
		0xc1a00000, 
		0xc2000000, 
		0x804a38a6, 
		0xeaa9eeb7, 
		0x00333f63, 
		0x00800000, 
		0x00040000, 
		0xc2700000, 
		0x7f800000, 
		0x427c0000, 
		0x0f9b7004, 
		0x007ffffe, 
		0x3f800001, 
		0x00000000, 
		0xff000000, 
		0xc2440000, 
		0x00000020, 
		0x42400000, 
		0x00100000, 
		0x00000000, 
		0x8f0cea26, 
		0xff7ffffe, 
		0x00000000, 
		0xde65eefc, 
		0xc20c0000, 
		0x0e000007, 
		0x41800000, 
		0x6d8767d0, 
		0x0c7ffff8, 
		0xc24c0000, 
		0x0c7ffe00, 
		0x41d00000, 
		0xdb933fc2, 
		0xc1700000, 
		0xff7fffff, 
		0xff800000, 
		0x800ea5a4, 
		0x007ffffe, 
		0x00000000, 
		0xc1300000, 
		0x0e000007, 
		0x80000010, 
		0x7f7ffffe, 
		0x09cb1573, 
		0x00000000, 
		0xa2576994, 
		0x80000002, 
		0x0e00007f, 
		0x42340000, 
		0x1c421028, 
		0x41800000, 
		0xc2440000, 
		0x82c23f0d, 
		0x80080000, 
		0x80011111, 
		0xc0a00000, 
		0x3f800000, 
		0x0e000003, 
		0x42300000, 
		0x80000000, 
		0x80000004, 
		0x042fcd8c, 
		0x05310618, 
		0xc1b00000, 
		0x7f800001, 
		0x80800000, 
		0x42340000, 
		0x22bb52f6, 
		0x00800002, 
		0x010f2e63, 
		0xc2240000, 
		0x17e0663a, 
		0x7fbfffff, 
		0x41b80000, 
		0x0a64a211, 
		0xffb7d4c2, 
		0xc0e00000, 
		0x0f7ffffc, 
		0x45a9c971, 
		0x7f7ffffe, 
		0xf635cf10, 
		0xc2000000, 
		0x80000002, 
		0x00800003, 
		0xe222bfff, 
		0x80000000, 
		0x0cbe3ce7, 
		0x0f7ffffc, 
		0x0c7fffc0, 
		0x59bc4785, 
		0x80800002, 
		0x42680000, 
		0xff800000, 
		0xd6acff60, 
		0x41a00000, 
		0xc1800000, 
		0x0e0003ff, 
		0xc0c00000, 
		0x80080000, 
		0x42680000, 
		0xeb68014a, 
		0xb52f47ed, 
		0xc2200000, 
		0xc2400000, 
		0xc6415972, 
		0x420c0000, 
		0x0e00000f, 
		0x7f7ffffe, 
		0x80000000, 
		0x2c02e59e, 
		0x8d8ca7ab, 
		0x00800000, 
		0xc2000000, 
		0xaf7a25b6, 
		0x41d00000, 
		0x327e43d0, 
		0x80800002, 
		0x9b811d44, 
		0x7f800000, 
		0xffffffff, 
		0x9949d1a1, 
		0x4841c5b7, 
		0x7f800000, 
		0x0e0003ff, 
		0x0e000003, 
		0xefab4fac, 
		0x0f7ffffe, 
		0xc50791af, 
		0x41f80000, 
		0x0f7fffff, 
		0x00000000, 
		0x0e07ffff, 
		0x420c0000, 
		0xe97a841e, 
		0x9a92ba46, 
		0x7f800000, 
		0x0c7ffff0, 
		0x41f00000, 
		0x00800001, 
		0x00000000, 
		0xc1500000, 
		0x425c0000, 
		0xaa33810e, 
		0x5a90f7e6, 
		0xff98c02f, 
		0x5769e1be, 
		0xff800001, 
		0x0c7ffff0, 
		0xff7ffffe, 
		0x7fa3f5a7, 
		0xa59ca232, 
		0xc2580000, 
		0x8b25dc95, 
		0xc0000000, 
		0x80800002, 
		0xffbfffff, 
		0x669f74ac, 
		0x80011111, 
		0xc2500000, 
		0x56bf2202, 
		0xc2080000, 
		0x42380000, 
		0x7fc00000, 
		0x426c0000, 
		0x00000000, 
		0x42000000, 
		0x80800003, 
		0xc1900000, 
		0x41200000, 
		0x00000002, 
		0xc1800000, 
		0x0e000fff, 
		0xbf800001, 
		0x41400000, 
		0xaaaaaaaa, 
		0x80200000, 
		0x9236602d, 
		0x9a0f99f7, 
		0x0c7fffc0, 
		0xea898356, 
		0x7fffffff, 
		0x143e4c73, 
		0xc2271a1c, 
		0x41c00000, 
		0x00000200, 
		0x7e5aa924, 
		0xff800000, 
		0xc1980000, 
		0xff7ffffe, 
		0xc2240000, 
		0x0c7ffc00, 
		0x7f800000, 
		0xc2500000, 
		0x41b00000, 
		0xffc00001, 
		0x4584403d, 
		0x807ffffe, 
		0x7f800000, 
		0x41300000, 
		0x00000800, 
		0x82346f3e, 
		0x00000000, 
		0x42280000, 
		0x42780000, 
		0x42180000, 
		0x5a3a4a88, 
		0x0e0fffff, 
		0x7a499a4c, 
		0x7f800000, 
		0x80000000, 
		0xc1400000, 
		0x7f800000, 
		0x00000000, 
		0x00800001, 
		0x80000000, 
		0xe63753c2, 
		0x7f84122a, 
		0x00200000, 
		0x42440000, 
		0x714b834d, 
		0x423c0000, 
		0x0e000fff, 
		0x79dde932, 
		0x9834b650, 
		0x42640000, 
		0x2dafadec, 
		0x423c0000, 
		0x7fae6800, 
		0x136ddc79, 
		0xffc00001, 
		0x8f7fffff, 
		0x7f800000, 
		0x0c7fe000, 
		0xc1200000, 
		0x40c00000, 
		0x0c600000, 
		0xc0400000, 
		0xff800000, 
		0xc1000000, 
		0x00000000, 
		0x00400000, 
		0xffbabc0f, 
		0x00000008, 
		0x423c0000, 
		0x0e000007, 
		0x42380000, 
		0x75c16b9b, 
		0xff000000, 
		0x481dda04, 
		0x534ca80f, 
		0xc2100000, 
		0x80800002, 
		0x180ae7cd, 
		0x05243475, 
		0xc1880000, 
		0x80000000, 
		0x0e001fff, 
		0x80000000, 
		0xc1d80000, 
		0x6be67179, 
		0x40a00000, 
		0x0e0000ff, 
		0x00000000, 
		0x42140000, 
		0x41e00000, 
		0x55555555, 
		0x1badc3e5, 
		0x422c0000, 
		0x395e88cd, 
		0xc1000000, 
		0x7f800000, 
		0xc1100000, 
		0x00040000, 
		0x7f800000, 
		0x00000200, 
		0x427c0000, 
		0xcccccccc, 
		0x41400000, 
		0x42080000, 
		0xaeec6fb5, 
		0xc1700000, 
		0x00100000, 
		0x9d267cca, 
		0x00000000, 
		0xff9b7ca9, 
		0x8f7ffffc, 
		0x7fc00001, 
		0x21f48ca8, 
		0x951da519, 
		0x0e00007f, 
		0x80000000, 
		0xc21c0000, 
		0x00000000, 
		0xff800000, 
		0x00800000, 
		0xff95c7d9, 
		0xdb8e390b, 
		0x41200000, 
		0x80000020, 
		0x41800000, 
		0xf3f8faac, 
		0xc2440000, 
		0xc1c80000, 
		0xfa216a9f, 
		0xd889e672, 
		0x267c1fbc, 
		0x00000008, 
		0x7f800000, 
		0x421c0000, 
		0x80000020, 
		0x7fbfffff, 
		0x7f7fffff, 
		0x42773c9b, 
		0x0e0007ff, 
		0x8e8b300d, 
		0x80000400, 
		0x42580000, 
		0x80000004, 
		0xdd6a9956, 
		0x422c0000, 
		0x90ee9b17, 
		0x0c7ffffe, 
		0x1f75338a, 
		0x42200000, 
		0xc2240000, 
		0x426c0000, 
		0x3f800001, 
		0x00000002, 
		0xff7ffffe, 
		0x7f8efdba, 
		0xfaffe6d0, 
		0x0c7ffc00, 
		0x42740000, 
		0xc2640000, 
		0xe55cd089, 
		0x3df0abfd, 
		0xc2440000, 
		0x9144da2e, 
		0xa3f316c1, 
		0x42640000, 
		0x00000000, 
		0x0e00001f, 
		0xc1f00000, 
		0x00400000, 
		0xc1300000, 
		0x80000004, 
		0xc1500000, 
		0xc27c0000, 
		0x80000000, 
		0x768ba1b8, 
		0x42780000, 
		0x6c1e4dcc, 
		0x0abc60cb, 
		0x42740000, 
		0x00000800, 
		0x41b80000, 
		0x8052bac4, 
		0x00800003, 
		0x3dfb9241, 
		0x0e03ffff, 
		0x71cf5743, 
		0x7f800000, 
		0xff800000, 
		0x00000200, 
		0xb2c2aa04, 
		0x3b73d57a, 
		0x6485913a, 
		0x3528239d, 
		0x0e000003, 
		0xc0e00000, 
		0x80011111, 
		0x80800003, 
		0xc1f00000, 
		0xff800000, 
		0x00800001, 
		0xc1b80000, 
		0xff7ffffe, 
		0xb8acd563, 
		0x80011111, 
		0x97731c02, 
		0x00ffffff, 
		0x41e00000, 
		0xff7ffffe, 
		0xff7fffff, 
		0xdbd39691, 
		0x2957caaf, 
		0x0f7fffff, 
		0x41400000, 
		0x00800002, 
		0x80000004, 
		0x7c718b7f, 
		0x00000000, 
		0x42240000, 
		0x0c7ffe00, 
		0x6064ba87, 
		0x1f097bf8, 
		0xc2180000, 
		0x00000000, 
		0xff000000, 
		0x42300000, 
		0x7fc00001, 
		0x80000001, 
		0x50671c61, 
		0x42480000, 
		0x80000000, 
		0x41a80000, 
		0x42480000, 
		0x24582dbf, 
		0x42680000, 
		0x00000001, 
		0x80000000, 
		0x19a07b7b, 
		0x80000000, 
		0x956b51d4, 
		0x76fd7eee, 
		0xff800000, 
		0x46bf8512, 
		0x00000010, 
		0x0e3fffff, 
		0x00000800, 
		0x00000100, 
		0x0b4204b4, 
		0x42480000, 
		0xf5fafa2a, 
		0x7f800000, 
		0x42640000, 
		0xc2400000, 
		0x0c7c0000, 
		0x7f7ffffe, 
		0x7fca560f, 
		0x225a46e7, 
		0xd2334a94, 
		0x7f7fffff, 
		0x00000000, 
		0x365c8933, 
		0x0072fce0, 
		0xe7929d2a, 
		0x00000400, 
		0x7f7fffff, 
		0x375e2a45, 
		0x00000000, 
		0x41f80000, 
		0x426c0000, 
		0xffc00001, 
		0x7f7ffffe, 
		0xff800000, 
		0x19569c6f, 
		0x0c7c0000, 
		0x423c0000, 
		0xcd9710f7, 
		0x42180000, 
		0x7f800000, 
		0x00200000, 
		0xff800000, 
		0x41400000, 
		0xa6c369fe, 
		0x0c69bbcd, 
		0x007fffff, 
		0x0c600000, 
		0xb1077de8, 
		0x02c897f4, 
		0xff000000, 
		0x0e000001, 
		0x0d79a677, 
		0x42080000, 
		0x18e3f2cf, 
		0x818aaa23, 
		0xc1b80000, 
		0xc1c00000, 
		0x7c9bb048, 
		0xffc00001, 
		0x0c7fffff, 
		0x7f800000, 
		0x4923cded, 
		0xc6f00ed5, 
		0xff800000, 
		0x40000000, 
		0x33d17f68, 
		0x80000200, 
		0xc2040000, 
		0xc1c80000, 
		0x0c7ff800, 
		0xfa650569, 
		0xd02b7980, 
		0x00000000, 
		0xc2640000, 
		0x41f80000, 
		0xff800001, 
		0x00800000, 
		0x0e00003f, 
		0x80000040, 
		0xc2680000, 
		0x00040000, 
		0x3ee12138, 
		0x8f7ffffe, 
		0x0e000001, 
		0xc1b80000, 
		0x596c3dd8, 
		0x00008000, 
		0xc0e00000, 
		0xc1300000, 
		0x0c7ff800, 
		0x40c00000, 
		0x51fa552d, 
		0x8dd930b2, 
		0x3818a548, 
		0x7fc00001, 
		0xff7fffff, 
		0x80000800, 
		0x82fe097e, 
		0x42340000, 
		0x0f7fffff, 
		0xcccccccc, 
		0x361bee78, 
		0x42780000, 
		0x00400000, 
		0xc2480000, 
		0x118872ab, 
		0x7db95d9f, 
		0x80000020, 
		0x7f7ffffe, 
		0x28f7df10, 
		0x00000000, 
		0x37831764, 
		0x80000010, 
		0xc2500000, 
		0xff800000, 
		0x42500000, 
		0x00800002, 
		0xc1f80000, 
		0x00000000, 
		0x80000000, 
		0x7fc00000, 
		0xc2340000, 
		0x9001308f, 
		0xa22b9672, 
		0xc2140000, 
		0x80000020, 
		0x0c7ffff0, 
		0x0e00000f, 
		0x41f80000, 
		0x7f7fffff, 
		0x16d7b431, 
		0x7fffffff, 
		0x00000001, 
		0x0f7ffffc, 
		0xb87d3eed, 
		0x0e003fff, 
		0x7f800000, 
		0xff800000, 
		0x41f00000, 
		0x7f800000, 
		0x41000000, 
		0x007ffffe, 
		0x80000004, 
		0x7c434438, 
		0x3f800000, 
		0x80000000, 
		0x80000000, 
		0xffeb9734, 
		0x94694158, 
		0xff7ffffe, 
		0x0c7fe000, 
		0x64d11001, 
		0x0f7ffffc, 
		0x7f800000, 
		0xd7d82836, 
		0x0e0003ff, 
		0xc1c80000, 
		0xe0a49f01, 
		0x0e1fffff, 
		0x7d9d7ea4, 
		0x0034c8b0, 
		0x80000800, 
		0xc2400000, 
		0x00800001, 
		0xc2600000, 
		0xff7ffffe, 
		0x420c0000, 
		0x00800000, 
		0x3e916c71, 
		0xc2340000, 
		0xff7ffffe, 
		0xb5d40dc7, 
		0xadd2406a, 
		0x00008000, 
		0x80000000, 
		0x3f028f5c, 
		0x8e9d70c2, 
		0x00000080, 
		0x23320a63, 
		0x89c9fedd, 
		0x0e0007ff, 
		0xaf1cca44, 
		0x80800001, 
		0xd70359b6, 
		0x42640000, 
		0x80800002, 
		0x1e2e8849, 
		0x42100000, 
		0x007fffff, 
		0x7fc00000, 
		0x80080000, 
		0xbf800001, 
		0x41a80000, 
		0x80000000, 
		0x425c0000, 
		0x80a56dfb, 
		0x802eda2b, 
		0x2f6548df, 
		0x33333333, 
		0xa86751aa, 
		0xed8c29ef, 
		0x80000200, 
		0x0e00001f, 
		0x1780e5d9, 
		0xc1c80000, 
		0x0e000003, 
		0x00000000, 
		0x80800001, 
		0x00800000, 
		0x0e00001f, 
		0x80000000, 
		0x006a6878, 
		0x3fa82651, 
		0xc23c0000, 
		0xb0d72760, 
		0x3179bb57, 
		0x0c7ffff0, 
		0x80000000, 
		0x00000000, 
		0x0c7c0000, 
		0x80000000, 
		0x42700000, 
		0xc2080000, 
		0x00000200, 
		0xff800000, 
		0xb77d82d3, 
		0x63054e0b, 
		0xc2740000, 
		0xff800000, 
		0xcb525831, 
		0x8127b135, 
		0xc1c00000, 
		0x00001000, 
		0xb2393224, 
		0xa52ee9aa, 
		0x7fc00001, 
		0x00000004, 
		0x00000000, 
		0xc2280000, 
		0xffefe188, 
		0xc1300000, 
		0xedc3dea9, 
		0xff800000, 
		0x5ac9a0d7, 
		0xffc00000, 
		0xc2040000, 
		0x0f7ffffe, 
		0xe052636b, 
		0xcccccccc, 
		0xc25c0000, 
		0xc2180000, 
		0xc3fbf941, 
		0xc2440000, 
		0x0e000001, 
		0x0c7f0000, 
		0x357f6268, 
		0xa818fa4f, 
		0x42580000, 
		0x42580000, 
		0x80001000, 
		0xc1d00000, 
		0x88afbf97, 
		0xff7ffffe, 
		0xc1e80000, 
		0x00800001, 
		0x0c7fff00, 
		0x7f800000, 
		0x41c00000, 
		0x8f7fffff, 
		0x80008000, 
		0x42480000, 
		0x0c780000, 
		0x41e00000, 
		0x0e000001, 
		0xcb000000, 
		0xf76b1bcd, 
		0x0c7c0000, 
		0xffac1b99, 
		0x0c7ffff0, 
		0x00000020, 
		0x0e000003, 
		0xa48e07ad, 
		0x529800e1, 
		0x00000800, 
		0xfaf24307, 
		0x0e3fffff, 
		0x00000002, 
		0xe43279fc, 
		0xca86a92c, 
		0xc1980000, 
		0xc1300000, 
		0xa4cc8313, 
		0x00000000, 
		0x0fcc4cdc, 
		0xff800000, 
		0xf04e2852, 
		0x00000008, 
		0x806f9a41, 
		0xc106ae4b, 
		0x00000000, 
		0xbf028f5c, 
		0x42380000, 
		0xc2200000, 
		0x0a2aa684, 
		0x0e00ffff, 
		0x41600000, 
		0x00800000, 
		0xc2000000, 
		0xcb8c4b40, 
		0x80011111, 
		0x80000000, 
		0x42380000, 
		0x40000000, 
		0x0c700000, 
		0x80011111, 
		0x426c0000, 
		0xff800000, 
		0x00000000, 
		0xc0400000, 
		0xc27c0000, 
		0x7f800000, 
		0x41800000, 
		0x06a0ab93, 
		0x41800000, 
		0x00000004, 
		0x1b2d1c49, 
		0x428d7ed5, 
		0xe1f96929, 
		0x0c7ffc00, 
		0x0a57ffb5, 
		0x2f6ac5af, 
		0x47a4216b, 
		0x0e001fff, 
		0xecd812f5, 
		0xff000000, 
		0x42400000, 
		0x42080000, 
		0xc2080000, 
		0xff7ffffe, 
		0x0c7fe000, 
		0x0c7fffc0, 
		0xffbfffff, 
		0xc2200000, 
		0x0f7ffffe, 
		0x41300000, 
		0x50f64aac  
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
		0x000000bd, 
		0x00000019, 
		0x000000b9, 
		0x00000213, 
		0x00000083, 
		0x0000018b, 
		0x00000167, 
		0x000001a5, 
		0x000001ab, 
		0x00000195, 
		0x000002a5, 
		0x0000037d, 
		0x0000032b, 
		0x0000018b, 
		0x000001ad, 
		0x00000349, 
		0x000000e5, 
		0x000001e7, 
		0x000000df, 
		0x000003fb, 
		0x000003b5, 
		0x00000229, 
		0x00000351, 
		0x000002c3, 
		0x000000bb, 
		0x0000011d, 
		0x00000045, 
		0x00000333, 
		0x0000015b, 
		0x0000027f, 
		0x000000cd, 
		0x000002bf, 
		0x000003ff, 
		0x000001ff, 
		0x00000117, 
		0x000003cd, 
		0x0000022d, 
		0x0000013d, 
		0x00000329, 
		0x00000207, 
		0x00000081, 
		0x000003dd, 
		0x000001ab, 
		0x00000169, 
		0x000002dd, 
		0x00000051, 
		0x000002ad, 
		0x000002d5, 
		0x00000291, 
		0x0000032f, 
		0x00000063, 
		0x000001cf, 
		0x00000163, 
		0x000001b3, 
		0x0000001d, 
		0x00000229, 
		0x000002f9, 
		0x000001bd, 
		0x00000355, 
		0x0000008f, 
		0x000003ad, 
		0x0000000b, 
		0x00000185, 
		0x00000041, 
		0x000001d9, 
		0x00000193, 
		0x00000225, 
		0x000002cd, 
		0x00000257, 
		0x0000018b, 
		0x0000006d, 
		0x000002b7, 
		0x000000ed, 
		0x000001ff, 
		0x000002bb, 
		0x00000075, 
		0x00000261, 
		0x000000c1, 
		0x000001a1, 
		0x000000ff, 
		0x00000337, 
		0x000002fd, 
		0x000001d5, 
		0x000001c1, 
		0x0000011f, 
		0x00000171, 
		0x00000283, 
		0x0000019d, 
		0x0000017f, 
		0x000003e5, 
		0x00000361, 
		0x0000014f, 
		0x00000069, 
		0x0000019f, 
		0x00000383, 
		0x00000175, 
		0x00000121, 
		0x00000391, 
		0x000000b9, 
		0x0000026f, 
		0x00000073, 
		0x000003db, 
		0x000002d5, 
		0x0000014f, 
		0x000001c7, 
		0x0000039b, 
		0x000002e5, 
		0x000001ad, 
		0x000003cf, 
		0x00000291, 
		0x00000041, 
		0x00000045, 
		0x0000011d, 
		0x00000087, 
		0x00000255, 
		0x000002af, 
		0x000000bf, 
		0x00000147, 
		0x000001ed, 
		0x00000093, 
		0x00000065, 
		0x00000069, 
		0x00000229, 
		0x00000043, 
		0x0000008f, 
		0x00000321, 
		0x000003a9, 
		0x000001c5, 
		0x000003b1, 
		0x000002c9, 
		0x000003e7, 
		0x0000025b, 
		0x0000019b, 
		0x00000043, 
		0x00000123, 
		0x00000043, 
		0x0000037d, 
		0x0000017d, 
		0x00000139, 
		0x000001a7, 
		0x00000017, 
		0x00000251, 
		0x00000257, 
		0x000002b3, 
		0x000001f1, 
		0x000000a1, 
		0x0000008d, 
		0x000002ef, 
		0x0000009b, 
		0x000001cd, 
		0x000001c7, 
		0x000000ab, 
		0x0000016b, 
		0x00000137, 
		0x00000363, 
		0x000000e7, 
		0x0000019d, 
		0x000001b1, 
		0x000003b9, 
		0x0000033b, 
		0x000002c3, 
		0x0000028d, 
		0x000003e7, 
		0x0000029f, 
		0x000000ff, 
		0x00000001, 
		0x0000039f, 
		0x00000015, 
		0x00000023, 
		0x000000ef, 
		0x000002f7, 
		0x00000385, 
		0x000000a1, 
		0x0000002f, 
		0x000000f3, 
		0x00000179, 
		0x000002c3, 
		0x00000325, 
		0x00000241, 
		0x000000c1, 
		0x000002b9, 
		0x00000397, 
		0x00000357, 
		0x0000015f, 
		0x0000012f, 
		0x00000105, 
		0x0000033f, 
		0x000000dd, 
		0x000001d1, 
		0x00000201, 
		0x0000035f, 
		0x000001a3, 
		0x00000287, 
		0x00000259, 
		0x00000229, 
		0x0000008d, 
		0x00000197, 
		0x000002e1, 
		0x000000f7, 
		0x000003b7, 
		0x00000235, 
		0x000003b9, 
		0x0000023d, 
		0x00000337, 
		0x000002a9, 
		0x00000299, 
		0x00000117, 
		0x000000eb, 
		0x00000281, 
		0x00000151, 
		0x000003c5, 
		0x000003a7, 
		0x00000005, 
		0x000002cf, 
		0x00000383, 
		0x00000167, 
		0x00000349, 
		0x0000013f, 
		0x000001c9, 
		0x0000015b, 
		0x000000b7, 
		0x00000143, 
		0x000000f7, 
		0x00000041, 
		0x000000c5, 
		0x00000127, 
		0x00000243, 
		0x00000339, 
		0x0000039f, 
		0x00000317, 
		0x00000019, 
		0x000003d1, 
		0x00000147, 
		0x000002fd, 
		0x0000034f, 
		0x0000029f, 
		0x000002b5, 
		0x00000185, 
		0x000002bb, 
		0x000000a7, 
		0x000003d7, 
		0x000001cf, 
		0x000002c7, 
		0x0000032b, 
		0x0000039d, 
		0x00000001, 
		0x0000033b, 
		0x00000087, 
		0x000000cd, 
		0x00000373, 
		0x000001df, 
		0x00000023, 
		0x00000025, 
		0x0000011d, 
		0x00000285, 
		0x00000055  
};

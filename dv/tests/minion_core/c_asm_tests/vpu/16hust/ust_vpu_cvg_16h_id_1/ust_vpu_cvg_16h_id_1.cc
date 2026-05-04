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
			"li x5, 383\n"
			"faddi.pi f2, f0, -90\n"
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
			"fnmadd.ps f29, f28, f22, f31\n"
			"fmsub.ps f23, f28, f22, f31\n"
			"fmsub.ps f25, f24, f19, f19\n"
			"fmadd.ps f18, f20, f28, f23\n"
			"fmadd.ps f29, f22, f25, f18\n"
			"fnmsub.ps f23, f31, f27, f28\n"
			"fmadd.ps f20, f30, f18, f28\n"
			"fmsub.ps f25, f28, f26, f25\n"
			"fnmsub.ps f28, f29, f30, f25\n"
			"fnmsub.ps f28, f23, f26, f31\n"
			"frcp.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"1:\n"
			"frcp.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"bne x0, x0, 1b\n"
			"fadd.ps f24, f31, f25\n"
			"fsub.pi f29, f30, f21\n"
			"fmul.pi f27, f23, f21\n"
			"fsub.ps f20, f30, f30\n"
			"fmax.ps f24, f26, f31\n"
			"2:\n"
			"frcp.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"bne x0, x0, 2b\n"
			"3:\n"
			"frcp.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"bne x0, x0, 3b\n"
			"frcp.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"frcp.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"2:\n"
			"frcp.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"bne x0, x0, 2b\n"
			"fnmadd.ps f30, f31, f26, f26\n"
			"fnmadd.ps f30, f31, f27, f30\n"
			"fmadd.ps f27, f31, f25, f26\n"
			"fmadd.ps f30, f28, f29, f31\n"
			"3:\n"
			"frcp.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"bne x0, x0, 3b\n"
			"fnmadd.ps f30, f26, f28, f31\n"
			"fnmsub.ps f27, f29, f26, f29\n"
			"fnmsub.ps f26, f27, f29, f28\n"
			"fmsub.ps f30, f31, f26, f27\n"
			"fmsub.ps f27, f28, f30, f26\n"
			"fmsub.ps f26, f28, f26, f26\n"
			"fmsub.ps f30, f31, f26, f26\n"
			"frcp.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"frcp.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"frcp.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"frcp.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"fsub.ps f30, f30, f31\n"
			"fadd.pi f30, f30, f30\n"
			"fmin.pi f30, f30, f31\n"
			"fadd.pi f31, f30, f31\n"
			"fadd.ps f31, f31, f31\n"
			"fmax.ps f31, f31, f31\n"
			"fand.pi f31, f31, f31\n"
			"fmin.pi f31, f31, f31\n"
			"fadd.ps f31, f31, f31\n"
			"fadd.pi f31, f30, f31\n"
			"frcp.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"frcp.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"fnmsub.ps f22, f23, f18, f13\n"
			"fmsub.ps f6, f27, f12, f10\n"
			"fnmsub.ps f9, f24, f10, f26\n"
			"fnmsub.ps f21, f9, f24, f4\n"
			"fnmadd.ps f27, f0, f27, f3\n"
			"fnmsub.ps f6, f11, f17, f12\n"
			"fnmadd.ps f22, f13, f31, f12\n"
			"fmadd.ps f4, f10, f29, f9\n"
			"2:\n"
			"frcp.ps f1, f17\n"
			"faddi.pi f17, f17, 128\n"
			"bne x0, x0, 2b\n"
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
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
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
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
			"li x0, 0x1\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 480(x25)\n"
			"flw.ps f9, 0(x25)\n"
			"fmul.s f26, f15, f9, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 832(x15)\n"
			"flw.ps f27, 672(x15)\n"
			"flw.ps f26, 192(x15)\n"
			"fnmsub.s f31, f21, f27, f26, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 800(x26)\n"
			"flw.ps f11, 0(x26)\n"
			"flw.ps f28, 192(x26)\n"
			"fmadd.ps f28, f3, f11, f28, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 896(x15)\n"
			"flw.ps f2, 320(x15)\n"
			"fmax.ps f24, f28, f2\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f3, 768(x25)\n"
			"fcvt.ps.f16 f18, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 768(x9)\n"
			"flw.ps f26, 896(x9)\n"
			"flw.ps f11, 608(x9)\n"
			"fmadd.s f11, f18, f26, f11, rne\n" 
			"li x5, 0xd6d6\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f9, 704(x9)\n"
			"flw.ps f21, 288(x9)\n"
			"for.pi f9, f9, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 320(x12)\n"
			"flw.ps f27, 0(x12)\n"
			"fminu.pi f25, f3, f27\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 896(x28)\n"
			"flw.ps f2, 96(x28)\n"
			"fmul.s f3, f25, f2, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 96(x15)\n"
			"flw.ps f28, 352(x15)\n"
			"fsub.s f25, f24, f28, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 768(x25)\n"
			"fswizz.ps f16, f15, 0x2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 864(x28)\n"
			"ffrc.ps f24, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 800(x15)\n"
			"flw.ps f29, 800(x15)\n"
			"fsub.ps f11, f15, f29, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f25, 832(x28)\n"
			"flw.ps f18, 832(x28)\n"
			"fltu.pi f26, f25, f18\n" 
			"maskpopc x16, m3\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"flem.ps m0, f28, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_690404_HID_0:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_646412_HID_0:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_583089_HID_0:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x2547\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x16 << TENSOR_FMA16_SCP_LOC_B) | (0xa6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa0 << TENSOR_FMA16_SCP_LOC_B) | (0x1a << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 224(x9)\n"
			"fround.ps f24, f25, rmm\n" 
			"li x28, 0x4e67e0d48e76805a\n"
			"fcvt.s.w f17, x28, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 416(x25)\n"
			"flw.ps f7, 288(x25)\n"
			"frem.pi f0, f27, f7\n" 
			"maskpopcz x25, m6\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 448(x16)\n"
			"fsat8.pi f25, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 512(x25)\n"
			"flw.ps f20, 416(x25)\n"
			"flw.ps f17, 928(x25)\n"
			"fnmadd.ps f7, f31, f20, f17, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 384(x25)\n"
			"flw.ps f20, 864(x25)\n"
			"fadd.pi f9, f0, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 896(x27)\n"
			"flw.ps f11, 640(x27)\n"
			"fsgnjx.s f27, f18, f11\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 896(x19)\n"
			"frsq.ps f6, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 768(x19)\n"
			"fclass.ps f9, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.ps m2, f24, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 640(x7)\n"
			"flw.ps f20, 736(x7)\n"
			"fsgnjx.ps f6, f13, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 864(x9)\n"
			"frsq.ps f17, f6\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f7, 352(x26)\n"
			"fcvt.ps.f16 f7, f7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 576(x19)\n"
			"flw.ps f11, 960(x19)\n"
			"fsra.pi f20, f25, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 768(x7)\n"
			"fadd.pi f18, f24, f24\n" 
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
			"LBL_HPM_3_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
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
			"la x16, rand_int_32\n"
			"flw.ps f30, 800(x16)\n"
			"flw.ps f10, 288(x16)\n"
			"fmul.pi f8, f30, f10\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 32(x9)\n"
			"flw.ps f2, 928(x9)\n"
			"fmul.ps f28, f11, f2, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 352(x23)\n"
			"flw.ps f2, 256(x23)\n"
			"fmsub.ps f8, f11, f11, f2, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 800(x27)\n"
			"flw.ps f24, 288(x27)\n"
			"flw.ps f23, 832(x27)\n"
			"fnmadd.s f29, f8, f24, f23, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 256(x23)\n"
			"flw.ps f2, 992(x23)\n"
			"fle.pi f30, f24, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 640(x6)\n"
			"flw.ps f13, 320(x6)\n"
			"fmin.ps f9, f21, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 448(x22)\n"
			"flw.ps f21, 768(x22)\n"
			"fmax.s f30, f19, f21\n" 
			"maskpopcz x5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 608(x26)\n"
			"fclass.s x22, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.pi m7, f6, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 0(x5)\n"
			"fsqrt.s f21, f29, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 384(x10)\n"
			"flw.ps f28, 96(x10)\n"
			"fminu.pi f29, f8, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"fltm.pi m3, f21, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 512(x10)\n"
			"flw.ps f9, 256(x10)\n"
			"fsgnj.s f2, f24, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 672(x26)\n"
			"flw.ps f27, 0(x26)\n"
			"fadd.ps f24, f30, f27, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 64(x10)\n"
			"ffrc.ps f23, f30\n" 
			"li x5, 0xf1d6\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
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
			"la x22, rand_int_32\n"
			"flw.ps f25, 992(x22)\n"
			"flw.ps f2, 352(x22)\n"
			"flt.pi f21, f25, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 0(x17)\n"
			"flw.ps f23, 384(x17)\n"
			"flt.ps f23, f19, f23\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_3:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 864(x12)\n"
			"fexp.ps f28, f6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 288(x14)\n"
			"flw.ps f25, 800(x14)\n"
			"fsll.pi f19, f19, f25\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_3:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 704(x22)\n"
			"flog.ps f28, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 896(x10)\n"
			"flw.ps f6, 480(x10)\n"
			"fmul.s f26, f7, f6, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 832(x5)\n"
			"fpackrepb.pi f25, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 96(x10)\n"
			"fsra.pi f19, f5, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 416(x26)\n"
			"flw.ps f19, 416(x26)\n"
			"flw.ps f7, 64(x26)\n"
			"fnmadd.s f25, f5, f19, f7, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 736(x22)\n"
			"fpackreph.pi f5, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 416(x26)\n"
			"flw.ps f19, 928(x26)\n"
			"flw.ps f24, 96(x26)\n"
			"fmsub.s f2, f25, f19, f24, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 832(x23)\n"
			"flw.ps f0, 992(x23)\n"
			"fmin.ps f23, f21, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 192(x18)\n"
			"flw.ps f26, 256(x18)\n"
			"fsgnj.ps f21, f25, f26\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 224(x10)\n"
			"flw.ps f2, 512(x10)\n"
			"fsub.s f16, f21, f2, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 96(x26)\n"
			"flw.ps f25, 512(x26)\n"
			"fcmovm.ps f21, f21, f25\n" 
			"mova.x.m x5\n" 
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f7, 640(x15)\n"
			"flw.ps f27, 192(x15)\n"
			"for.pi f0, f7, f27\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f21, 640(x23)\n"
			"fcvt.ps.f16 f15, f21\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 768(x5)\n"
			"flw.ps f3, 480(x5)\n"
			"fsgnj.ps f4, f4, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 288(x6)\n"
			"fcvt.ps.pwu f13, f27, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 64(x15)\n"
			"fsin.ps f15, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 960(x26)\n"
			"flw.ps f24, 768(x26)\n"
			"fmul.s f7, f4, f24, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 160(x14)\n"
			"faddi.pi f27, f9, 0x1a8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"feqm.ps m4, f0, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 128(x26)\n"
			"flw.ps f6, 672(x26)\n"
			"fmax.s f0, f15, f6\n" 
			"fbci.pi f3, 0x394c5\n" 
			"la x7, rand_int_32\n"
			"flw.ps f15, 704(x7)\n"
			"fandi.pi f3, f15, 0xdd\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x28, m4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 736(x20)\n"
			"flw.ps f10, 608(x20)\n"
			"fmul.pi f21, f3, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_4:\n"
			"fltm.ps m5, f13, f26\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x37f0c3e4a6\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f26, 160(x14)\n"
			"flw.ps f18, 896(x14)\n"
			"fxor.pi f24, f26, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xb << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xb << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x18 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x5 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 128(x7)\n"
			"flw.ps f17, 544(x7)\n"
			"fsgnj.ps f24, f28, f17\n" 
			"maskpopcz x22, m1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 640(x22)\n"
			"fcvt.f16.ps f17, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"masknot m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 768(x9)\n"
			"flw.ps f21, 800(x9)\n"
			"fle.s x28, f24, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f28, 224(x28)\n"
			"flw.ps f8, 960(x28)\n"
			"fxor.pi f5, f28, f8\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 928(x25)\n"
			"flw.ps f16, 448(x25)\n"
			"for.pi f6, f17, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa45446f103\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 160(x22)\n"
			"fsrli.pi f21, f27, 0xe\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 384(x25)\n"
			"flw.ps f10, 512(x25)\n"
			"fsll.pi f27, f18, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f21, 96(x16)\n"
			"flw.ps f10, 64(x16)\n"
			"fmul.ps f22, f21, f10, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 640(x22)\n"
			"flw.ps f16, 608(x22)\n"
			"fmax.pi f17, f4, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 256(x16)\n"
			"flw.ps f5, 0(x16)\n"
			"fmsub.ps f23, f8, f8, f5, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 288(x22)\n"
			"flw.ps f13, 96(x22)\n"
			"fand.pi f18, f28, f13\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 736(x16)\n"
			"flw.ps f6, 512(x16)\n"
			"fmulhu.pi f22, f28, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"flem.ps m0, f5, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 64(x5)\n"
			"flw.ps f31, 192(x5)\n"
			"flt.ps f20, f5, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 480(x6)\n"
			"flw.ps f20, 576(x6)\n"
			"fsub.s f31, f1, f20, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 928(x19)\n"
			"flw.ps f12, 672(x19)\n"
			"flw.ps f20, 160(x19)\n"
			"fmadd.ps f21, f21, f12, f20, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f28, 544(x28)\n"
			"faddi.pi f20, f28, 0x49\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 256(x26)\n"
			"flw.ps f5, 928(x26)\n"
			"flw.ps f9, 640(x26)\n"
			"fmsub.s f2, f26, f5, f9, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"feqm.ps m4, f21, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f1, 704(x15)\n"
			"flw.ps f13, 928(x15)\n"
			"fmax.pi f26, f1, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 384(x12)\n"
			"flw.ps f26, 416(x12)\n"
			"for.pi f31, f1, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 800(x5)\n"
			"flw.ps f5, 352(x5)\n"
			"flt.ps f20, f13, f5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 384(x18)\n"
			"flw.ps f26, 928(x18)\n"
			"fadd.ps f20, f28, f26, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"feqm.ps m0, f23, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f5, 672(x18)\n"
			"flw.ps f12, 992(x18)\n"
			"fsub.pi f20, f5, f12\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 448(x15)\n"
			"flw.ps f5, 768(x15)\n"
			"fsgnjx.ps f23, f23, f5\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 32(x17)\n"
			"flw.ps f12, 512(x17)\n"
			"fsll.pi f2, f24, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 608(x28)\n"
			"fcvt.pwu.ps f15, f20, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 736(x6)\n"
			"flw.ps f20, 800(x6)\n"
			"for.pi f20, f12, f20\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xe9 << TENSOR_IMA_SCP_LOC_B) | (0x52 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 32(x25)\n"
			"flw.ps f20, 896(x25)\n"
			"fsgnjx.s f25, f24, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 224(x27)\n"
			"fclass.s x18, f3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 416(x23)\n"
			"flw.ps f17, 192(x23)\n"
			"fsrl.pi f7, f20, f17\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 960(x27)\n"
			"fslli.pi f24, f9, 0x2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 544(x19)\n"
			"fcvt.wu.s x10, f16, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 160(x18)\n"
			"flw.ps f30, 512(x18)\n"
			"flw.ps f3, 672(x18)\n"
			"fmadd.ps f30, f21, f30, f3, rdn\n" 
			"maskpopc x10, m6\n" 
			"li x15, 0x4fdc3c380b43a1ab\n"
			"fcvt.s.l f30, x15, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"maskor m6, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f25, 992(x15)\n"
			"flw.ps f0, 96(x15)\n"
			"fmulhu.pi f24, f25, f0\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 0(x15)\n"
			"flw.ps f17, 192(x15)\n"
			"fadd.pi f3, f12, f17\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"maskor m5, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f3, 128(x5)\n"
			"flw.ps f21, 960(x5)\n"
			"fmulhu.pi f16, f3, f21\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 576(x15)\n"
			"flw.ps f13, 672(x15)\n"
			"for.pi f25, f21, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 32(x15)\n"
			"flw.ps f14, 288(x15)\n"
			"flw.ps f30, 736(x15)\n"
			"fmsub.ps f12, f20, f14, f30, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 576(x5)\n"
			"flw.ps f7, 992(x5)\n"
			"feq.pi f8, f21, f7\n" 
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f26,  768 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
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
			"maskpopcz x26, m5\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f18, 0(x15)\n"
			"fcvt.ps.f16 f11, f18\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 640(x22)\n"
			"flw.ps f22, 192(x22)\n"
			"fle.ps f11, f26, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"maskxor m6, m5, m6\n" 
			"li x5, 0x80340160\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 32(x22)\n"
			"flw.ps f13, 0(x22)\n"
			"fle.ps f8, f3, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 96(x12)\n"
			"fandi.pi f18, f18, 0x1cf\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 480(x20)\n"
			"flw.ps f14, 352(x20)\n"
			"flw.ps f22, 448(x20)\n"
			"fcmov.ps f28, f28, f14, f22\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 576(x26)\n"
			"flw.ps f17, 96(x26)\n"
			"fsra.pi f11, f26, f17\n" 
			"la x12, rand_int_32\n"
			"flw.ps f22, 224(x12)\n"
			"fsrli.pi f4, f22, 0x8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 736(x19)\n"
			"flw.ps f5, 768(x19)\n"
			"fmax.s f26, f14, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 160(x12)\n"
			"fandi.pi f0, f28, 0x188\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 384(x26)\n"
			"flw.ps f22, 928(x26)\n"
			"fle.s x5, f2, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 32(x26)\n"
			"flw.ps f28, 384(x26)\n"
			"fmin.ps f2, f8, f28\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 480(x27)\n"
			"flw.ps f23, 544(x27)\n"
			"fsra.pi f2, f26, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 352(x15)\n"
			"fsqrt.s f11, f11, rtz\n" 
			"li x5, 0xef17\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"fltm.ps m5, f11, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_397186_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x4d << TENSOR_FMA16_SCP_LOC_B) | (0xe2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x2b << TENSOR_FMA16_SCP_LOC_B) | (0x27 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 800(x9)\n"
			"fsub.ps f16, f24, f24, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 544(x5)\n"
			"flw.ps f10, 384(x5)\n"
			"fmul.s f30, f29, f10, rne\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f3, 416(x23)\n"
			"fcvt.ps.f16 f14, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 64(x7)\n"
			"fsin.ps f18, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 960(x19)\n"
			"fcvt.pw.ps f15, f13, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 448(x23)\n"
			"flw.ps f12, 352(x23)\n"
			"feq.pi f10, f10, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 512(x22)\n"
			"flw.ps f9, 320(x22)\n"
			"fadd.ps f30, f13, f9, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f24, 512(x25)\n"
			"fslli.pi f15, f24, 0x3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 352(x23)\n"
			"fcvt.ps.pw f14, f30, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 160(x5)\n"
			"flw.ps f12, 672(x5)\n"
			"fmulhu.pi f12, f10, f12\n" 
			"la x23, rand_int_32\n"
			"flw.ps f18, 256(x23)\n"
			"flw.ps f9, 0(x23)\n"
			"fmin.pi f10, f18, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 32(x12)\n"
			"flw.ps f12, 928(x12)\n"
			"fle.ps f5, f30, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f23, 352(x17)\n"
			"fslli.pi f30, f23, 0x0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 544(x17)\n"
			"flw.ps f1, 352(x17)\n"
			"fcmovm.ps f10, f18, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskor m4, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f18, 0xef32\n" 
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
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
			"flw.ps f18, 672(x9)\n"
			"flw.ps f2, 928(x9)\n"
			"fadd.ps f11, f18, f2, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 800(x10)\n"
			"flw.ps f10, 416(x10)\n"
			"fmax.ps f26, f31, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fsetm.pi m7, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x18, 0x4bc78004d1dd5ac9\n"
			"fcvt.s.l f26, x18, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 96(x12)\n"
			"flw.ps f17, 192(x12)\n"
			"fmul.ps f29, f24, f17, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 544(x9)\n"
			"flw.ps f24, 384(x9)\n"
			"fmulhu.pi f11, f29, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x16\n" 
			"li x5, 0xd95a\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 480(x19)\n"
			"flw.ps f10, 96(x19)\n"
			"fsgnjx.ps f11, f12, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 512(x23)\n"
			"fcvt.pw.ps f11, f16, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f12, 960(x12)\n"
			"fpackrepb.pi f31, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 384(x26)\n"
			"flw.ps f2, 800(x26)\n"
			"flw.ps f3, 416(x26)\n"
			"fnmadd.ps f31, f26, f2, f3, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 672(x12)\n"
			"fcvt.f16.ps f19, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 128(x23)\n"
			"fmulhu.pi f10, f2, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 288(x23)\n"
			"flw.ps f11, 896(x23)\n"
			"flw.ps f10, 256(x23)\n"
			"fmadd.s f11, f4, f11, f10, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 576(x19)\n"
			"fsub.pi f16, f2, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x5996\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_443684_HID_10:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_218054_HID_10:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_252628_HID_10:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x6f << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbb << TENSOR_FMA16_SCP_LOC_B) | (0x46 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f5,  416 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 928(x27)\n"
			"fsin.ps f9, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 640(x5)\n"
			"flw.ps f13, 416(x5)\n"
			"fand.pi f25, f31, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 864(x15)\n"
			"fsqrt.s f31, f21, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"flem.ps m3, f7, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 992(x26)\n"
			"flw.ps f7, 448(x26)\n"
			"feq.ps f21, f4, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"fsetm.pi m2, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 512(x25)\n"
			"fswizz.ps f8, f28, 0x16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 480(x26)\n"
			"flw.ps f5, 128(x26)\n"
			"fmul.pi f15, f17, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 608(x12)\n"
			"fsat8.pi f9, f9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 192(x9)\n"
			"fle.pi f20, f7, f7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 224(x10)\n"
			"faddi.pi f31, f17, 0x19a\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 128(x12)\n"
			"flw.ps f5, 448(x12)\n"
			"flw.ps f13, 640(x12)\n"
			"fmsub.s f13, f28, f5, f13, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 352(x5)\n"
			"flw.ps f15, 416(x5)\n"
			"fminu.pi f2, f7, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 352(x9)\n"
			"flw.ps f11, 0(x9)\n"
			"fsra.pi f8, f13, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 192(x10)\n"
			"fpackrepb.pi f28, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 576(x18)\n"
			"flw.ps f20, 544(x18)\n"
			"flw.ps f4, 512(x18)\n"
			"fnmadd.ps f15, f5, f20, f4, rmm\n" 
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
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
			"maskxor m2, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 960(x26)\n"
			"fswizz.ps f14, f2, 0xb\n" 
			"li x5, 0xec97609fbdab85dd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5d539e739a85827a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f8, 864(x26)\n"
			"fpackrepb.pi f31, f8\n" 
			"li x5, 0x3f22b6c3180f424\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x262e3584e44a2978\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 832(x14)\n"
			"frcp.ps f11, f21\n" 
			"fbci.ps f11, 0x3008b\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"maskand m7, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_12:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 288(x16)\n"
			"flog.ps f10, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 608(x12)\n"
			"flw.ps f22, 960(x12)\n"
			"fsgnjx.s f9, f9, f22\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 352(x28)\n"
			"flw.ps f18, 224(x28)\n"
			"fmulhu.pi f2, f31, f18\n" 
			"li x5, 0xc1e6cd184faa60ef\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd7104ede77d82c46\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 928(x20)\n"
			"fcvt.w.s x10, f2, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_12:\n"
			"flem.ps m2, f14, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 864(x10)\n"
			"fcvt.w.s x18, f8, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 32(x28)\n"
			"flw.ps f21, 960(x28)\n"
			"fmax.s f8, f18, f21\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 928(x16)\n"
			"fpackrepb.pi f18, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 832(x28)\n"
			"flw.ps f2, 448(x28)\n"
			"flw.ps f31, 896(x28)\n"
			"fnmsub.s f8, f22, f2, f31, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 160(x16)\n"
			"flw.ps f24, 32(x16)\n"
			"fsgnj.ps f21, f9, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_347011_HID_12:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xc << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1f << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xe << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
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
			"la x19, rand_int_32\n"
			"flw.ps f3, 992(x19)\n"
			"flw.ps f18, 896(x19)\n"
			"feq.pi f2, f3, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"fltm.ps m1, f17, f13\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x136dc9bcf8\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 896(x15)\n"
			"fclass.ps f18, f24\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f13, 192(x15)\n"
			"fcvt.ps.f16 f31, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 96(x18)\n"
			"flw.ps f13, 512(x18)\n"
			"fmul.pi f1, f28, f13\n" 
			"maskpopc x17, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 832(x5)\n"
			"flw.ps f11, 832(x5)\n"
			"fle.ps f10, f10, f11\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 256(x18)\n"
			"fcvt.pw.ps f7, f17, rne\n" 
			"la x5, rand_ieee754_16\n"
			"flw.ps f1, 544(x5)\n"
			"fcvt.ps.f16 f1, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 96(x23)\n"
			"fround.ps f28, f1, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 736(x11)\n"
			"flw.ps f10, 672(x11)\n"
			"flt.s x19, f2, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 0(x23)\n"
			"flw.ps f10, 32(x23)\n"
			"fadd.pi f24, f8, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 64(x16)\n"
			"flw.ps f3, 96(x16)\n"
			"fsgnjn.s f11, f8, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 256(x19)\n"
			"fround.ps f11, f17, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 736(x20)\n"
			"flw.ps f25, 704(x20)\n"
			"fsub.ps f10, f3, f25, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 192(x16)\n"
			"fcvt.ps.pwu f2, f7, rdn\n" 
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 832(x28)\n"
			"flw.ps f18, 448(x28)\n"
			"flw.ps f22, 800(x28)\n"
			"fnmsub.s f7, f31, f18, f22, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 704(x7)\n"
			"flw.ps f12, 768(x7)\n"
			"for.pi f18, f9, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 416(x14)\n"
			"flw.ps f30, 768(x14)\n"
			"fadd.s f12, f25, f30, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 960(x22)\n"
			"flw.ps f22, 288(x22)\n"
			"fmul.pi f5, f31, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 128(x13)\n"
			"flw.ps f3, 768(x13)\n"
			"flw.ps f25, 928(x13)\n"
			"fnmsub.s f6, f22, f3, f25, rdn\n" 
			"li x5, 0x60e1\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f11, 672(x11)\n"
			"flw.ps f9, 896(x11)\n"
			"fadd.pi f27, f11, f9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 736(x11)\n"
			"flw.ps f5, 704(x11)\n"
			"flw.ps f25, 640(x11)\n"
			"fnmadd.ps f6, f9, f5, f25, rne\n" 
			"li x5, 0x7cf6\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f22, 32(x26)\n"
			"flw.ps f3, 768(x26)\n"
			"fsrl.pi f23, f22, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 640(x13)\n"
			"flw.ps f27, 928(x13)\n"
			"fmax.ps f6, f9, f27\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 160(x28)\n"
			"flw.ps f18, 992(x28)\n"
			"flw.ps f27, 0(x28)\n"
			"fnmadd.ps f10, f25, f18, f27, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 64(x7)\n"
			"flw.ps f5, 96(x7)\n"
			"fmul.ps f30, f27, f5, rmm\n" 
			"li x5, 0xd19c\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f12, 832(x26)\n"
			"flw.ps f18, 32(x26)\n"
			"fsra.pi f5, f12, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"fltm.pi m0, f18, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_14:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 288(x11)\n"
			"fexp.ps f3, f23\n" 
			"la x19, rand_int_32\n"
			"flw.ps f11, 96(x19)\n"
			"flw.ps f30, 448(x19)\n"
			"fmaxu.pi f11, f11, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 320(x17)\n"
			"fsrli.pi f22, f3, 0x0\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x1 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1e << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xc << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x5 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1c << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x3 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 5\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 13\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskand m3, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f21, 928(x22)\n"
			"flw.ps f5, 352(x22)\n"
			"for.pi f20, f21, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 192(x6)\n"
			"fsqrt.s f5, f6, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 224(x15)\n"
			"flw.ps f13, 800(x15)\n"
			"feq.s x26, f28, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 672(x10)\n"
			"flw.ps f16, 448(x10)\n"
			"for.pi f6, f9, f16\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 352(x10)\n"
			"fpackrepb.pi f6, f21\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 672(x15)\n"
			"flw.ps f13, 64(x15)\n"
			"fadd.pi f15, f17, f13\n" 
			"li x6, 0xac1a1dff0953d8e4\n"
			"fcvt.s.wu f1, x6, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 288(x10)\n"
			"fcvt.ps.pw f24, f21, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 672(x10)\n"
			"frsq.ps f6, f5\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 832(x10)\n"
			"flw.ps f24, 960(x10)\n"
			"fmin.s f20, f3, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 288(x23)\n"
			"flw.ps f20, 192(x23)\n"
			"fsgnjn.s f9, f10, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 544(x23)\n"
			"flw.ps f8, 704(x23)\n"
			"fsgnj.ps f21, f28, f8\n" 
			"maskpopcz x10, m3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 448(x7)\n"
			"flw.ps f16, 256(x7)\n"
			"flw.ps f1, 320(x7)\n"
			"fmadd.ps f16, f3, f16, f1, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 928(x19)\n"
			"flw.ps f10, 224(x19)\n"
			"fsub.ps f16, f5, f10, rne\n" 
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
		0xe10f238a, 
		0x4ea8ba03, 
		0x7f7713d1, 
		0xacdd471a, 
		0x5eea1539, 
		0x8dcc1817, 
		0x83618765, 
		0x9a90105e, 
		0x46affd36, 
		0x7dec95c0, 
		0x804b4b70, 
		0x43c1ef62, 
		0x3b05cac0, 
		0x82e66b1d, 
		0x2135a722, 
		0xec2ba5ea, 
		0x117c7dcb, 
		0x0fd1f298, 
		0xda3ed7b2, 
		0x10f04b49, 
		0x024b5a59, 
		0x89b6df05, 
		0x361ba5de, 
		0x333a6521, 
		0x3bc7bda8, 
		0x35f3a315, 
		0xe3127fc4, 
		0x73dfc87c, 
		0x54d6f493, 
		0x68216f2b, 
		0xa3e02767, 
		0xc2f0d5d7, 
		0x37e715ad, 
		0x2d310b5d, 
		0x19081ff0, 
		0xcb7bf940, 
		0x2573d716, 
		0xd53dcf4f, 
		0x266a1d83, 
		0x582591ce, 
		0x96ec1b5c, 
		0x71491b28, 
		0xcb200053, 
		0xaa2e29a4, 
		0x1e837f23, 
		0x33ce604c, 
		0x1dadecf8, 
		0x31de54a5, 
		0x5994a1a1, 
		0x9d903e47, 
		0x62f3db21, 
		0xd4881f67, 
		0xe09ab5f2, 
		0xe92c15e4, 
		0x1d2480ca, 
		0xcc2b38b3, 
		0xe2ade707, 
		0x8842c61c, 
		0xee4721d7, 
		0x5edc0277, 
		0x74d2d35a, 
		0x5a151335, 
		0x0461587b, 
		0x2099a8a6, 
		0x2b031301, 
		0x911acc7b, 
		0x26455098, 
		0xa097021c, 
		0x229be150, 
		0x9cae8b34, 
		0x3c4508b1, 
		0xa5227b6f, 
		0x2bea3fc4, 
		0xdb1c99df, 
		0x4f4b245b, 
		0x6da8ec24, 
		0x8cca96b3, 
		0xe2e7b339, 
		0xea97b607, 
		0xd8445118, 
		0xaf3c7029, 
		0x2fd16ec8, 
		0x88dbf8cc, 
		0xfc315153, 
		0xf66418a4, 
		0xdecd2cd9, 
		0xaa74b7a8, 
		0x1ed3879d, 
		0xf2d4e1c8, 
		0x35cd901a, 
		0xd9107ec8, 
		0x67281434, 
		0x0ca8fab1, 
		0xf11b3de5, 
		0x01bf57b8, 
		0x22ad1f66, 
		0xe3ec24e0, 
		0x240f60c5, 
		0x3a6338ee, 
		0xdf183cd5, 
		0x79981798, 
		0x5b0a90ec, 
		0x12fc0ebb, 
		0x34890f59, 
		0xc353bd86, 
		0x354fc5f4, 
		0x29e14534, 
		0x5ce5d225, 
		0x5c3e8ccb, 
		0x9c14ef2e, 
		0xde0a7162, 
		0x164ab910, 
		0x57a92bdf, 
		0x04983bde, 
		0x5de4c9d4, 
		0x8f25027b, 
		0x22b11cce, 
		0xff2e9189, 
		0x9ae81c0b, 
		0x92a8db77, 
		0x7b1d93c3, 
		0x124d37a7, 
		0xdbf2bfe9, 
		0x53d4e3f6, 
		0x502540d7, 
		0x7cd25089, 
		0xf3681a9d, 
		0x4b655508, 
		0xa23a732c, 
		0x0c710261, 
		0x6e72dbe2, 
		0x37a7d901, 
		0x90c7531d, 
		0x9376cda0, 
		0x9bca1c4e, 
		0xace621d4, 
		0x529a1a4a, 
		0x5a0a1251, 
		0x65a2329d, 
		0xdfbc323d, 
		0x7301e56c, 
		0xd528d775, 
		0x3e2e6761, 
		0x3f637df1, 
		0xbdba0fb4, 
		0x81d32bd9, 
		0x91ea1fb1, 
		0xb3186bd8, 
		0xcb8b5d57, 
		0x7f46d848, 
		0x0362cba4, 
		0xbdfa62ee, 
		0x96b2853f, 
		0xb9d48fa2, 
		0xd254eddb, 
		0x7012192e, 
		0x16973b75, 
		0x5b696159, 
		0x294ff575, 
		0xd53ba0be, 
		0xf4459073, 
		0x913d1c68, 
		0x7d8bff24, 
		0x0f346c6c, 
		0xa788bce4, 
		0x82714c26, 
		0xfcd1d8a3, 
		0xfe5c53d9, 
		0x1f93484b, 
		0x431740ab, 
		0xfdcd9d48, 
		0x5477351b, 
		0x4ee36e18, 
		0x6d6a8c99, 
		0x8d580102, 
		0xf485f033, 
		0xc9999406, 
		0xfb37b6ce, 
		0x1e7adbdb, 
		0xbc0eb77f, 
		0x70872ded, 
		0x713e7db4, 
		0xd5d676d8, 
		0x0dd49263, 
		0xb8d5ad41, 
		0x63372b56, 
		0x7181d777, 
		0x2e96b4da, 
		0x7330ff0b, 
		0x63c5b89c, 
		0xe9a33aee, 
		0x990f0fb1, 
		0xc6959f0c, 
		0x8e726096, 
		0x3ad4e5cb, 
		0xd98b45cb, 
		0x710c8882, 
		0x1a053326, 
		0x77e1bdc7, 
		0xee332bb9, 
		0x771ed053, 
		0x81e9acce, 
		0x2a087c63, 
		0x2621e880, 
		0x255d4c4a, 
		0xf8a3ba1d, 
		0x9c6b8aa4, 
		0xe08e9cd2, 
		0x1a5b09d8, 
		0x0d761339, 
		0x47b61c52, 
		0x03a02ba7, 
		0x7203c4b7, 
		0x94c8a62c, 
		0xc0c441ba, 
		0x2c94cdd2, 
		0x7d4614f6, 
		0x721e8405, 
		0x756fe900, 
		0x89c1cd9b, 
		0x89405803, 
		0xd2f8b954, 
		0xa2afdd0f, 
		0x766c5cca, 
		0x37d7adfa, 
		0x3ecde79c, 
		0xc7d757d7, 
		0xc27d7656, 
		0x540a3d84, 
		0xf1509089, 
		0x5822f194, 
		0x68a9a629, 
		0x397f8974, 
		0xb2d7fd3d, 
		0x2cf5e8af, 
		0xf5bc54ce, 
		0x38e1a266, 
		0x9c81fe8f, 
		0x627b2637, 
		0x4b2e8ab3, 
		0x6f18285a, 
		0xceec2aa7, 
		0x273ad5ce, 
		0xfcb84e2c, 
		0xc0e34232, 
		0x24ed9e01, 
		0x6fdfa20f, 
		0xf92aa6b2, 
		0x35643c4f, 
		0x288bdb4b, 
		0xf7dda4ed, 
		0x1eada597, 
		0x21400052, 
		0x2318358c, 
		0x2c952a4f, 
		0x7d1cf870  
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
		0x0222, 
		0xc880, 
		0x4e80, 
		0xcec0, 
		0x5060, 
		0x401f, 
		0x1ec2, 
		0xcf40, 
		0x6e14, 
		0x4880, 
		0xd120, 
		0x91c9, 
		0xfc00, 
		0x3298, 
		0xd300, 
		0xb3dd, 
		0x14f4, 
		0x4000, 
		0xcd80, 
		0x5340, 
		0x1b43, 
		0x0000, 
		0xd1c0, 
		0xf0f2, 
		0xcefe, 
		0xbc00, 
		0x4c00, 
		0xfc00, 
		0x8000, 
		0xd180, 
		0xcc80, 
		0xe814, 
		0x4c00, 
		0x5080, 
		0x5380, 
		0x8000, 
		0xd020, 
		0x4a6a, 
		0x0228, 
		0xfbfe, 
		0xbc78, 
		0x4e80, 
		0x306d, 
		0x4b00, 
		0x020d, 
		0xd2a0, 
		0x4980, 
		0x53c0, 
		0x8000, 
		0xda62, 
		0x5060, 
		0xa7a4, 
		0xd080, 
		0x44b2, 
		0xce5d, 
		0x5120, 
		0xc303, 
		0x8ff9, 
		0x53e0, 
		0xd140, 
		0x5020, 
		0xd0c0, 
		0x47fe, 
		0x5340, 
		0x5320, 
		0xcf80, 
		0xdfe8, 
		0xc500, 
		0xd060, 
		0xcc00, 
		0xd0a0, 
		0x5080, 
		0xccc0, 
		0xcf40, 
		0x8000, 
		0x4a00, 
		0xd1ff, 
		0x1839, 
		0x4a1f, 
		0xd020, 
		0x2377, 
		0x4980, 
		0xd08a, 
		0xc900, 
		0x4800, 
		0x3a72, 
		0xd180, 
		0x52c0, 
		0xd180, 
		0x4dc0, 
		0xd060, 
		0x8000, 
		0x51a0, 
		0xc000, 
		0xd3e0, 
		0xd0c0, 
		0x4f40, 
		0x51c0, 
		0xd200, 
		0x5120, 
		0x4f00, 
		0x4cc0, 
		0xfc00, 
		0x53a0, 
		0x8000, 
		0x5020, 
		0x5e8c, 
		0xcd40, 
		0xd0a0, 
		0x94ee, 
		0x82cf, 
		0x10ff, 
		0x53c0, 
		0x32d4, 
		0xef05, 
		0xcc40, 
		0xd340, 
		0x52c0, 
		0x4700, 
		0x5220, 
		0x5116, 
		0xaf22, 
		0xc800, 
		0xfdc7, 
		0x0000, 
		0xe461, 
		0x4c40, 
		0x5280, 
		0x4ee1, 
		0xd080, 
		0xfc00, 
		0x6840, 
		0xca80, 
		0x4fc0, 
		0x8000, 
		0x50a0, 
		0x8000, 
		0x0000, 
		0xd140, 
		0xc980, 
		0x51a0, 
		0x4980, 
		0xf314, 
		0xd240, 
		0x50ef, 
		0xcc80, 
		0xee26, 
		0xcf00, 
		0xca00, 
		0xcd40, 
		0x2372, 
		0x4900, 
		0x5260, 
		0x4c40, 
		0xd120, 
		0x8000, 
		0xd2a0, 
		0x3cde, 
		0xd0c0, 
		0xce80, 
		0xd060, 
		0xcf40, 
		0x8c47, 
		0xd140, 
		0x4b80, 
		0xd3d0, 
		0x5340, 
		0xc600, 
		0x4e00, 
		0x4fc0, 
		0x4ec0, 
		0xf966, 
		0x5080, 
		0x4200, 
		0xcc40, 
		0x0d44, 
		0xcd40, 
		0x4b00, 
		0xcdc0, 
		0x4e00, 
		0x4e00, 
		0x601e, 
		0xee21, 
		0x4f00, 
		0x4800, 
		0xd200, 
		0x5120, 
		0x80c8, 
		0x3c00, 
		0xc98b, 
		0xc400, 
		0xfd1e, 
		0x4dc0, 
		0xf679, 
		0xc400, 
		0xc200, 
		0xd1a0, 
		0x4c80, 
		0x5100, 
		0x4500, 
		0xcf80, 
		0x1639, 
		0xc600, 
		0xc600, 
		0xca00, 
		0x4ec0, 
		0x50c0, 
		0xfa48, 
		0x4e00, 
		0x7c00, 
		0x4980, 
		0x53a0, 
		0x4600, 
		0x9735, 
		0xd180, 
		0xcec0, 
		0xece0, 
		0x70e6, 
		0x4c00, 
		0xbc00, 
		0x53e0, 
		0xf3cf, 
		0x5020, 
		0x51a0, 
		0xd0e0, 
		0x9ac0, 
		0x3b81, 
		0x7c00, 
		0x3d68, 
		0x82b6, 
		0x5060, 
		0xfc00, 
		0xc900, 
		0x52e0, 
		0xc300, 
		0x4200, 
		0x241a, 
		0x8281, 
		0x6053, 
		0x5340, 
		0x5260, 
		0xcdf5, 
		0x003d, 
		0x5060, 
		0x5040, 
		0x4d80, 
		0xbf7f, 
		0x4f80, 
		0x5180, 
		0x0000, 
		0xc000, 
		0x5240, 
		0x4d80, 
		0x4900, 
		0x4500, 
		0x5100, 
		0x5340, 
		0x7c00, 
		0xd180, 
		0x4f00, 
		0xcdc0, 
		0x5360, 
		0xc5b5, 
		0x5300, 
		0xc880, 
		0x4900, 
		0xd160, 
		0x1b58, 
		0x7c00, 
		0xd3e0, 
		0xd000, 
		0xf198, 
		0xb22c, 
		0x7c00, 
		0x4000, 
		0x8000, 
		0x6734, 
		0x9b49, 
		0x118a, 
		0x4d80, 
		0x4a00, 
		0xc700, 
		0x2435, 
		0x51c1, 
		0x5160, 
		0xd200, 
		0xd300, 
		0xa346, 
		0xbad7, 
		0x976e, 
		0x5140, 
		0xd380, 
		0x4800, 
		0x66e1, 
		0x5cef, 
		0xcb80, 
		0x1b66, 
		0xc168, 
		0x0000, 
		0x51e0, 
		0x4cc0, 
		0xd2a0, 
		0x5300, 
		0xf9ee, 
		0x4900, 
		0x0000, 
		0x0443, 
		0x4dc0, 
		0x4e00, 
		0xdfe5, 
		0xdd95, 
		0x4f80, 
		0xcf80, 
		0x56d4, 
		0xcc80, 
		0x50a0, 
		0xd060, 
		0x8000, 
		0xc980, 
		0xd040, 
		0xcf41, 
		0x36f2, 
		0xd1c0, 
		0x0000, 
		0xfc00, 
		0x5260, 
		0x0000, 
		0xc200, 
		0x612d, 
		0x4bc3, 
		0x19d1, 
		0xc900, 
		0x2592, 
		0x8000, 
		0xd180, 
		0x9106, 
		0xd3a0, 
		0xd160, 
		0xbcf7, 
		0x0c6f, 
		0xcf80, 
		0xc980, 
		0x3897, 
		0x4b00, 
		0xd180, 
		0xd3a0, 
		0x77f0, 
		0x51a0, 
		0x476d, 
		0xd300, 
		0x0000, 
		0x0c01, 
		0x8000, 
		0xb479, 
		0xc602, 
		0x8000, 
		0x6ab1, 
		0x5060, 
		0x51e0, 
		0xd040, 
		0x5140, 
		0x22d5, 
		0x5960, 
		0xd280, 
		0x0a1d, 
		0xcf00, 
		0x2359, 
		0xa77d, 
		0x53c0, 
		0x7c00, 
		0xd040, 
		0x8067, 
		0x4a80, 
		0x5260, 
		0x4b00, 
		0x1d18, 
		0xd180, 
		0xcf05, 
		0x4200, 
		0x5000, 
		0x7d5f, 
		0xd2a0, 
		0xce80, 
		0x5020, 
		0x5d7a, 
		0x07bc, 
		0x5160, 
		0xcf29, 
		0xe56a, 
		0x8000, 
		0xcb80, 
		0xd220, 
		0xd060, 
		0xcf00, 
		0x4a80, 
		0x7c00, 
		0xcc00, 
		0xb999, 
		0x0000, 
		0x4cc0, 
		0xc066, 
		0xd2c0, 
		0xd53d, 
		0x8000, 
		0xd0a0, 
		0x5000, 
		0xfc00, 
		0x4c40, 
		0x5140, 
		0x093f, 
		0xa4a4, 
		0x5020, 
		0xd3a0, 
		0x5020, 
		0xd220, 
		0xf78c, 
		0x4500, 
		0xc980, 
		0xd3a0, 
		0x4600, 
		0x6870, 
		0x30f8, 
		0xc456, 
		0x5140, 
		0xcd00, 
		0x4200, 
		0xd260, 
		0x2a85, 
		0x4500, 
		0x53e0, 
		0xd200, 
		0xbbf4, 
		0x4f40, 
		0x52a0, 
		0x2f30, 
		0x47cb, 
		0x8000, 
		0x6402, 
		0x4400, 
		0xd080, 
		0x4dc0, 
		0xa867, 
		0xd0a0, 
		0x53a0, 
		0xd120, 
		0xc700, 
		0x4c00, 
		0xcdc0, 
		0xc700, 
		0xc880, 
		0x2c64, 
		0x4d00, 
		0x66cf, 
		0x79a0, 
		0x0000, 
		0x63ac, 
		0x98e4, 
		0xcb80, 
		0xc600, 
		0xc663, 
		0xd140, 
		0x4fc0, 
		0xde88, 
		0x51c0, 
		0x4aa6, 
		0x4a80, 
		0x5060, 
		0xcfc0, 
		0x4f00, 
		0xcd00, 
		0x0000, 
		0x5020, 
		0x4ec0, 
		0xe422, 
		0xcf40, 
		0xb71e, 
		0xcc00, 
		0x5080, 
		0xd0e0, 
		0x53a0, 
		0xb43f, 
		0x5020, 
		0xd080, 
		0x8914, 
		0xd180, 
		0xd160, 
		0xfc00, 
		0x52c0, 
		0xe1bb, 
		0x50a0, 
		0x4900, 
		0xcdb8, 
		0x09bd, 
		0x72c1, 
		0x227b, 
		0xce40, 
		0x5120, 
		0x57c6, 
		0x7e04, 
		0xce00, 
		0xd040, 
		0x4b80, 
		0xd3a0, 
		0x0000, 
		0x5080, 
		0x4600, 
		0xcd40, 
		0x5ca1, 
		0x8f9b, 
		0xd2e0, 
		0xe993, 
		0x69ff, 
		0x4e80, 
		0xce00, 
		0x4fc0, 
		0xcb80, 
		0xe702, 
		0x451e, 
		0xcd40, 
		0xf882, 
		0xb0e6, 
		0xcdc0, 
		0x8b11, 
		0xd3e0, 
		0xd1e0, 
		0x4d7f, 
		0x4cc0, 
		0x4e40, 
		0x9670, 
		0x5080, 
		0xd160, 
		0x0363, 
		0xc000, 
		0xcd80, 
		0x53e0, 
		0x8000, 
		0xcc80, 
		0x8000, 
		0x0709, 
		0x5120, 
		0x4fc0, 
		0x6188, 
		0xd300, 
		0xc000, 
		0x87b6, 
		0xc409, 
		0x0000, 
		0x3f5e, 
		0x9502, 
		0x0eca, 
		0xd180, 
		0x45a4, 
		0xd380, 
		0x1e1f, 
		0xef5c, 
		0x7c00, 
		0xe648, 
		0xd040, 
		0xcc00, 
		0x4980, 
		0xbae3, 
		0xd120, 
		0x0000, 
		0xd300, 
		0xf59a, 
		0xd180, 
		0xd1c0, 
		0x51c0, 
		0x5080, 
		0xc980, 
		0x51e0, 
		0x0000, 
		0xd0c0, 
		0xaaf1, 
		0x9fdf, 
		0x4600, 
		0xcec0, 
		0xca80, 
		0x5120, 
		0x8000, 
		0xfc00, 
		0xcf00, 
		0x65a8, 
		0x5040, 
		0x8000, 
		0x50e0, 
		0x4656, 
		0xc880, 
		0x3c00, 
		0xd100, 
		0xd3c0, 
		0x0fd2, 
		0x348f, 
		0x166f, 
		0x511d, 
		0x51a0, 
		0xd280, 
		0xce80, 
		0x4800, 
		0xd320, 
		0xd889, 
		0x52c0, 
		0xab8a, 
		0x4dc0, 
		0x3c00, 
		0x4f40, 
		0x8000, 
		0xd0a0, 
		0x4900, 
		0x16d9, 
		0xd100, 
		0x1ae5, 
		0x4d00, 
		0xd240, 
		0xd3e0, 
		0x4980, 
		0xd300, 
		0x6a3e, 
		0x4d6d, 
		0x4dc0, 
		0xdb80, 
		0x8000, 
		0x4ec0, 
		0x4800, 
		0xed7a, 
		0x9f93, 
		0x5180, 
		0xfc00, 
		0x84e7, 
		0x5280, 
		0x5e0a, 
		0x0000, 
		0x268e, 
		0x4fa3, 
		0xd300, 
		0x52e0, 
		0x4c80, 
		0xf45c, 
		0x39f3, 
		0xca00, 
		0x5cd1, 
		0x4000, 
		0x4f80, 
		0xcec0, 
		0xfc00, 
		0x04e1, 
		0x7ca0, 
		0xd1e0, 
		0x0092, 
		0x2ff3, 
		0x77e4, 
		0x51a0, 
		0x0000, 
		0x2bb8, 
		0xcdc0, 
		0xc12e, 
		0x384c, 
		0x4b80, 
		0x0000, 
		0xd2a0, 
		0xd320, 
		0xd380, 
		0x4ec0, 
		0x5080, 
		0x5360, 
		0x7c00, 
		0x4900, 
		0x0000, 
		0xcf40, 
		0x6fae, 
		0x4dc0, 
		0xa0fd, 
		0x8000, 
		0x5380, 
		0x1ec2, 
		0x5220, 
		0x52a0, 
		0xd320, 
		0xb21b, 
		0xd0e0, 
		0xd260, 
		0x6603, 
		0xc08a, 
		0xcec0, 
		0x52e0, 
		0xf0e0, 
		0x4f92, 
		0xd120, 
		0xd3c0, 
		0xf1c7, 
		0x4b00, 
		0x5320, 
		0x209f, 
		0x5000, 
		0xd9b8, 
		0x4f00, 
		0x4880, 
		0x4d40, 
		0xcb00, 
		0x5191, 
		0x3c00, 
		0x4cc0, 
		0xcfc0, 
		0x5080, 
		0xdafc, 
		0x45f4, 
		0x4b00, 
		0x51a0, 
		0xab36, 
		0x4400, 
		0x5080, 
		0x9bae, 
		0xd3e0, 
		0x17e3, 
		0x5220, 
		0x5160, 
		0x4d40, 
		0x4200, 
		0x3a29, 
		0xd120, 
		0x8000, 
		0x4b80, 
		0x7c00, 
		0x53e0, 
		0x51a0, 
		0x8000, 
		0xd200, 
		0x15e4, 
		0x5000, 
		0xcc40, 
		0x81d0, 
		0xfc00, 
		0x0399, 
		0x4cc0, 
		0xd100, 
		0x3072, 
		0x5146, 
		0xd200, 
		0x5140, 
		0x0000, 
		0x4dd9, 
		0xc715, 
		0xfc00, 
		0xd180, 
		0x28b6, 
		0x5cfa, 
		0xd0c0, 
		0x4900, 
		0xfc00, 
		0xcb80, 
		0xa5e9, 
		0xcd80, 
		0xf919, 
		0x979c, 
		0x5300, 
		0x8000, 
		0x4f1a, 
		0xfc00, 
		0xd0e0, 
		0x8000, 
		0x0000, 
		0xd1c0, 
		0xcd80, 
		0xd120, 
		0xd220, 
		0xc700, 
		0x4700, 
		0xd1c0, 
		0x5160, 
		0xeebf, 
		0x5280, 
		0x51a0, 
		0x52c0, 
		0xd160, 
		0xbc00, 
		0x3c00, 
		0x0000, 
		0xd060, 
		0x72ae, 
		0x5340, 
		0x8000, 
		0xbc36, 
		0x5080, 
		0xd040, 
		0xcc00, 
		0x4200, 
		0x5000, 
		0x4f40, 
		0xd0a0, 
		0xfc00, 
		0xd2e0, 
		0x4c00, 
		0xd280, 
		0xccc0, 
		0x4d00, 
		0x786f, 
		0xd300, 
		0xc880, 
		0xcd00, 
		0x8d34, 
		0x5200, 
		0xc600, 
		0xd2e0, 
		0x4c40, 
		0xd140, 
		0xe70f, 
		0x5300, 
		0xcc80, 
		0x03dc, 
		0x53a0, 
		0x8000, 
		0x98d7, 
		0x7c14, 
		0x7c8e, 
		0xce80, 
		0xa032, 
		0xd160, 
		0xd380, 
		0x4800, 
		0x0000, 
		0x5140, 
		0xedd8, 
		0x9ff2, 
		0x6b0b, 
		0xd180, 
		0x5120, 
		0xd160, 
		0xd200, 
		0x1d37, 
		0xcd40, 
		0x5b91, 
		0xca80, 
		0x50e0, 
		0xd0e0, 
		0x63c2, 
		0xce40, 
		0xe75c, 
		0x0855, 
		0x4d40, 
		0x52c0, 
		0xd000, 
		0x5260, 
		0x7231, 
		0xd0e0, 
		0xe322, 
		0xd3a0, 
		0xd220, 
		0xd300, 
		0x5220, 
		0xd360, 
		0x4700, 
		0xfc00, 
		0x4600, 
		0xcf8a, 
		0x7c00, 
		0xc000, 
		0x5260, 
		0x4e00, 
		0x4e40, 
		0x3a44, 
		0xcb00, 
		0x22d0, 
		0xc900, 
		0x5340, 
		0x4000, 
		0x50e0, 
		0x09df, 
		0x7c00, 
		0x4a80, 
		0x929f, 
		0xf10d, 
		0x8000, 
		0x5120, 
		0x0000, 
		0x0000, 
		0xcfc0, 
		0x4d00, 
		0xd000, 
		0xc200, 
		0x7be5, 
		0x8000, 
		0xd2e0, 
		0xd150, 
		0xc48b, 
		0xcc40, 
		0xd240, 
		0xcb00, 
		0xe76d, 
		0xd2a0, 
		0x8d6d, 
		0xb3bb, 
		0x5280, 
		0x5380, 
		0x51e0, 
		0xd3c0, 
		0x5dea, 
		0xcc80, 
		0xd032, 
		0xc400, 
		0xbc95, 
		0x2f1f, 
		0xce00, 
		0xff2c, 
		0xcc40, 
		0x50c0, 
		0x4f00, 
		0x8000, 
		0x4900, 
		0x53c0, 
		0xb904, 
		0x7c00, 
		0x4c80, 
		0xcfc0, 
		0x4b80, 
		0xd0a0, 
		0x4c80, 
		0xcc40, 
		0x6ca0, 
		0x4e00, 
		0xc129, 
		0x52e0, 
		0x4ec0, 
		0xd340, 
		0xa134, 
		0x5180, 
		0xd260, 
		0xd140, 
		0x0b4a, 
		0x4e80, 
		0xa7d6, 
		0x4b00, 
		0xd280, 
		0x50a0, 
		0xc03c, 
		0x5200, 
		0xd2c0, 
		0xd240, 
		0xd3c0, 
		0xcc40, 
		0x4c40, 
		0x4cc0, 
		0x53c0, 
		0xd280, 
		0x5180, 
		0xccc0, 
		0x691b, 
		0xb1b6, 
		0x4800, 
		0x5180, 
		0x5120, 
		0xcd80, 
		0xc600, 
		0xfc00, 
		0xd280, 
		0x4800, 
		0x53e0, 
		0xc853, 
		0x75ab, 
		0xd000, 
		0xd2a0, 
		0x4a00, 
		0xd2c0, 
		0x7c04, 
		0x5553, 
		0xcdc0, 
		0xfc00, 
		0x734b, 
		0xf9c4, 
		0xcc00, 
		0xb773, 
		0x5140, 
		0xcc40, 
		0x5020, 
		0x0000, 
		0x754f, 
		0x4500, 
		0x52a0, 
		0xcc00, 
		0x52e0, 
		0xff7e, 
		0x14f6, 
		0x9528, 
		0x6496, 
		0x6fcd, 
		0x53c0, 
		0xd5b7, 
		0x8000, 
		0xcec0, 
		0xd160, 
		0xcc00, 
		0xd060, 
		0x3908, 
		0xd240, 
		0xf47d, 
		0x819f, 
		0x51c0, 
		0xd2c0, 
		0xfc00, 
		0x6df4, 
		0xd040, 
		0x4a80, 
		0x5180, 
		0xcf00, 
		0xce40, 
		0x4b80, 
		0x4c80, 
		0xd260, 
		0x0000, 
		0x98ef, 
		0x50a0, 
		0xecfc, 
		0x50c0, 
		0x4cc0, 
		0x4e40, 
		0x50e0, 
		0x5000, 
		0x5240, 
		0x77be, 
		0xce00, 
		0xd260, 
		0x872a, 
		0xabb4, 
		0xd3e0, 
		0x5160, 
		0x5000, 
		0xd220, 
		0x4e00, 
		0x4ec0, 
		0x0000, 
		0x8000, 
		0x4f00, 
		0x70d0, 
		0xd240, 
		0x52e0, 
		0x4d40, 
		0xd340, 
		0x4600, 
		0xcc00, 
		0xcbc8, 
		0x7c00, 
		0x4b00, 
		0xd300, 
		0xd93a, 
		0x5040, 
		0x87d3, 
		0xd2c0, 
		0xbc00, 
		0x4700, 
		0x4d00, 
		0x4400, 
		0x5020, 
		0x4200, 
		0xae5a, 
		0x4e40, 
		0x5180, 
		0x8094, 
		0x4e00, 
		0x5200, 
		0x5180, 
		0x0000, 
		0x4a00, 
		0x4980, 
		0x925a, 
		0x6789, 
		0x5da1, 
		0x5360, 
		0xd080, 
		0xd160, 
		0x52e0, 
		0x5200, 
		0xc900, 
		0xd2e0, 
		0x5000, 
		0xfc00, 
		0x4b80, 
		0xd240, 
		0xdb98, 
		0xd240, 
		0xca00, 
		0xd3c0, 
		0xcc40, 
		0x5120, 
		0x254a, 
		0xcc40, 
		0x4e80, 
		0x5380, 
		0x5dd6, 
		0x5340, 
		0x7c00, 
		0x5000, 
		0xace5, 
		0x5100, 
		0xd040, 
		0xd1c0, 
		0x4ec0, 
		0xd080, 
		0xc000, 
		0x801e, 
		0x4c40, 
		0x5180, 
		0xfc00, 
		0x67dc, 
		0x53c0, 
		0x5200, 
		0xc705, 
		0x6217, 
		0x4c80, 
		0xcb80, 
		0x4a00, 
		0x4b00, 
		0xc054, 
		0x0000, 
		0xd2a0, 
		0xcb80, 
		0xa3f9, 
		0x4c40, 
		0xa0c1, 
		0x93f8, 
		0xfa0e, 
		0xd1e0, 
		0xf8ea, 
		0xd2e0, 
		0x7b64, 
		0x4c00, 
		0xcc40, 
		0x4980, 
		0xc000, 
		0x5200, 
		0x3ea9, 
		0x5020, 
		0x8000, 
		0xd2e0, 
		0xd1c0, 
		0xcec0, 
		0xd0e0, 
		0x5300, 
		0x0000, 
		0x2576, 
		0xd1a0, 
		0xb357, 
		0x0000, 
		0xc500, 
		0x5180, 
		0xfc00, 
		0x886b, 
		0xba3b, 
		0x0dac, 
		0xce00, 
		0xef4e, 
		0x5280, 
		0xce80, 
		0x53a0, 
		0xc000, 
		0xd1a0, 
		0x7c21, 
		0x5120, 
		0xd1e0, 
		0x4980, 
		0xd240, 
		0x5300, 
		0x4900, 
		0x15fb, 
		0xce40, 
		0x866e, 
		0xc600, 
		0x08ef, 
		0x7ff0, 
		0x2eaf, 
		0xc800, 
		0xcb00, 
		0x9ff8, 
		0xd340, 
		0x8000, 
		0x2aed, 
		0xc880, 
		0x0ea0, 
		0x0041, 
		0xd1e0, 
		0x95fa, 
		0x1d13, 
		0x8000, 
		0x2d9a, 
		0x5040, 
		0xe7fa, 
		0x00dd, 
		0x3a3b, 
		0x001e, 
		0xe3b7, 
		0xc400, 
		0xc700, 
		0xc000, 
		0x4700, 
		0xcc80, 
		0xca00, 
		0xb314, 
		0x8000, 
		0x7c00, 
		0x5200, 
		0x4dc0, 
		0x5020, 
		0xd2a0, 
		0xebf4, 
		0x52a0, 
		0x88b0, 
		0x0000, 
		0x4700, 
		0x4c80, 
		0xfa5c, 
		0x436f, 
		0x80bc, 
		0xbcf9, 
		0x0000, 
		0xeaff, 
		0xb360, 
		0xbe0e, 
		0xfc00, 
		0x4f40, 
		0xc600, 
		0x52c0, 
		0xc600, 
		0xd040, 
		0xd300, 
		0xc000, 
		0xca80, 
		0x4a00, 
		0x4700, 
		0x4e80, 
		0xe580, 
		0xc72f, 
		0x5100, 
		0xd220, 
		0xcd40, 
		0x70e9, 
		0x5000, 
		0x5507, 
		0xd300, 
		0x5180, 
		0xd2e0, 
		0x52a0, 
		0x4600, 
		0xd2c0, 
		0x4cc0, 
		0xd300, 
		0xcd40, 
		0xfc72, 
		0xb2f1, 
		0x5020, 
		0x5340, 
		0xd360, 
		0x6b1c, 
		0x00fe, 
		0xd220, 
		0xcf00, 
		0x85ab, 
		0xd1e0, 
		0x5100, 
		0x4e80, 
		0x4e80, 
		0xcb80, 
		0x7c00, 
		0x4b00, 
		0xd100, 
		0xd2c0, 
		0x5220, 
		0x7c00, 
		0xd060, 
		0x7ca6, 
		0x8000, 
		0xd340, 
		0x34cf, 
		0x1deb, 
		0xd000, 
		0xc700, 
		0xfc00, 
		0x4cc0, 
		0x3c00, 
		0x52a0, 
		0x0000, 
		0x8000, 
		0xc000, 
		0x5080, 
		0x4d40, 
		0xd300, 
		0xd0d9, 
		0x5182, 
		0xd3a0, 
		0xcd40, 
		0xd060, 
		0xcdc0, 
		0x4980, 
		0x53e0, 
		0x51a0, 
		0x4dc0, 
		0xce40, 
		0xb3e2, 
		0x7c00, 
		0x7cf8, 
		0xd0e0, 
		0x0000, 
		0xd340, 
		0xc500, 
		0xd280, 
		0xcc40, 
		0xd3a0, 
		0x53c0, 
		0x398d, 
		0x5080, 
		0x6af7, 
		0x0000, 
		0xbd3d, 
		0xd360, 
		0xd380, 
		0x0973, 
		0x4f80, 
		0x0000, 
		0xd300, 
		0xa1a0, 
		0x4a80, 
		0x5080, 
		0x4f40, 
		0x4a80, 
		0x52c0, 
		0xebc6, 
		0x4b00, 
		0x2f27, 
		0xd160, 
		0xfc25, 
		0x4e40, 
		0x5360, 
		0x7c00, 
		0x8000, 
		0x4b00, 
		0x52c0, 
		0x5000, 
		0x4c40, 
		0xfc00, 
		0xd0a0, 
		0x5260, 
		0xd1e0, 
		0x8000, 
		0x4d00, 
		0xd1b5, 
		0x770c, 
		0xcb80, 
		0xd3a0, 
		0xca00, 
		0x53c0, 
		0x4acd, 
		0x0b99, 
		0xa648, 
		0xc700, 
		0xc200, 
		0xc880, 
		0x4dc0, 
		0x4f00, 
		0x8e78, 
		0xd1a0, 
		0xc000, 
		0x8000, 
		0x5a5a, 
		0x7c34, 
		0xfc00, 
		0x4800, 
		0x5ae1, 
		0xcc40, 
		0x5baf, 
		0xd240, 
		0xd040, 
		0x4600, 
		0x4880, 
		0xd0a0, 
		0xd280, 
		0x4e00, 
		0x8000, 
		0x4400, 
		0x4700, 
		0x4b00, 
		0x51e0, 
		0xe146, 
		0x8000, 
		0x5260, 
		0x8000, 
		0x452d, 
		0x5240, 
		0xec40, 
		0xcb00, 
		0xca00, 
		0xce40, 
		0x5140, 
		0x4c80, 
		0xf577, 
		0xd360, 
		0xcf80, 
		0xd0a0, 
		0x7091, 
		0xcf80, 
		0x5020, 
		0x4500, 
		0x4c00, 
		0x4f80, 
		0xd3c0, 
		0xd020, 
		0x5300, 
		0x5280, 
		0x4c00, 
		0xd0a0, 
		0xccb7, 
		0x3aa4, 
		0xc000, 
		0x53db, 
		0xfc00, 
		0xbe97, 
		0x8000, 
		0xd200, 
		0x4f40, 
		0x0000, 
		0x4e80, 
		0xcd00, 
		0xee43, 
		0xc800, 
		0xdbde, 
		0xb2b0, 
		0xd360, 
		0xa271, 
		0x4d80, 
		0x0fd8, 
		0xce80, 
		0x2c52, 
		0xd3e0, 
		0x5224, 
		0x4700, 
		0xd020, 
		0xdb77, 
		0x7dd7, 
		0xc880, 
		0x8000, 
		0x5320, 
		0x5180, 
		0x4d80, 
		0xd020, 
		0x8000, 
		0xc880, 
		0x1165, 
		0x47a8, 
		0x4a00, 
		0x4980, 
		0xcef6, 
		0xe607, 
		0xd0a0, 
		0xfc5c, 
		0x50c0, 
		0x0a46, 
		0xcf00, 
		0x53c0, 
		0xcd29, 
		0x4f00, 
		0x5220, 
		0x8000, 
		0xd904, 
		0xf909, 
		0x4a80, 
		0x4f80, 
		0xe454, 
		0x1d94, 
		0x7364, 
		0x4700, 
		0xd380, 
		0x8000, 
		0x5180, 
		0xdeaa, 
		0xcf40, 
		0xcc00, 
		0xcd40, 
		0xd2c0, 
		0x5140, 
		0x9934, 
		0x021e, 
		0x407b, 
		0xbc00, 
		0x46dc, 
		0x69ce, 
		0x4b00, 
		0x8102, 
		0xfc00, 
		0x5140, 
		0x1602, 
		0x7507, 
		0xcfc0, 
		0x7cd2, 
		0xd2a0, 
		0xd300, 
		0xd180, 
		0x5200, 
		0x4980, 
		0x59e2, 
		0xa598, 
		0x86db, 
		0x419e, 
		0x4a80, 
		0x55a0, 
		0x4200, 
		0x4000, 
		0x8000, 
		0x60c2, 
		0xc82c, 
		0xcec0, 
		0x5360, 
		0x50c0, 
		0xcd00, 
		0xc980, 
		0x4400, 
		0xfc00, 
		0x4400, 
		0x02f7, 
		0xedcc, 
		0x8000, 
		0x50a0, 
		0x8000, 
		0x5160, 
		0x4fc0, 
		0xd320, 
		0xd180, 
		0x4a00, 
		0x50e0, 
		0x53e0, 
		0x4e2e, 
		0x4f80, 
		0x7c00, 
		0xa3d7, 
		0x8000, 
		0xd0c0, 
		0xd240, 
		0x6cb5, 
		0xc880, 
		0xcf40, 
		0x4d80  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00000000, 
		0x7f800000, 
		0x09dbbb64, 
		0x226310e8, 
		0x007fffff, 
		0x007fffff, 
		0x007ffffe, 
		0xb6b92d4e, 
		0x00000000, 
		0xa7a39c5a, 
		0x0e00000f, 
		0xff800000, 
		0x0c7fffc0, 
		0x0e00007f, 
		0x0e000001, 
		0x42040000, 
		0xc1e80000, 
		0x76f1bdb9, 
		0x80000000, 
		0x0069aa87, 
		0xc2440000, 
		0x007fffff, 
		0x00000100, 
		0x42080000, 
		0xb8ecaa71, 
		0x805a0b32, 
		0x8fd2fd04, 
		0x421c0000, 
		0x1d66205c, 
		0x42580000, 
		0x00000200, 
		0xc2700000, 
		0x5918889b, 
		0xed2b6d8f, 
		0x0e003fff, 
		0x55555555, 
		0x40800000, 
		0x3f028f5c, 
		0x40a1b647, 
		0x00020000, 
		0x42040000, 
		0xc27c0000, 
		0x5c6df109, 
		0x3a041eef, 
		0xff000000, 
		0x00000002, 
		0x07af8f57, 
		0xc2100000, 
		0xa2ac54a3, 
		0x7fa5c8f0, 
		0x2aa72680, 
		0x2945c04d, 
		0xc2780000, 
		0xc1e80000, 
		0x41600000, 
		0x7fc00001, 
		0x00000004, 
		0x42480000, 
		0x0c7c0000, 
		0x7fa0c09a, 
		0x6260ac70, 
		0x81089cd9, 
		0x412a9880, 
		0xc2340000, 
		0x7fc00001, 
		0xb1414eee, 
		0x0c700000, 
		0x41f00000, 
		0x0e000003, 
		0xcccccccc, 
		0xc2240000, 
		0xff800000, 
		0xc2100000, 
		0x42700000, 
		0x7f7ffffe, 
		0x80000004, 
		0xd8465dda, 
		0x36c0d32a, 
		0x4b8c4b40, 
		0x42cc3a42, 
		0x7f800000, 
		0x42500000, 
		0x0c700000, 
		0x80000800, 
		0xc0e00000, 
		0xcc0196d5, 
		0xc0c00000, 
		0x80000200, 
		0x32faeeca, 
		0x41d00000, 
		0x80800000, 
		0xff000000, 
		0xc2300000, 
		0x00020000, 
		0x80000000, 
		0x7f800000, 
		0xc2500000, 
		0x0f7fffff, 
		0x54bf2c4b, 
		0x80800003, 
		0x00000000, 
		0x8f7ffffd, 
		0x7f800000, 
		0x0c7ffe00, 
		0x0f7fffff, 
		0xff8055d1, 
		0xff800000, 
		0xc2000000, 
		0x0c780000, 
		0xff7ffffe, 
		0x00800000, 
		0x80001000, 
		0x19311c5f, 
		0x423c0000, 
		0x2178aac7, 
		0x0c7f8000, 
		0x7f7fffff, 
		0x0e000007, 
		0x69e5f0f7, 
		0x00000100, 
		0xc1e80000, 
		0x652b290b, 
		0x3f028f5c, 
		0x41e80000, 
		0x00000000, 
		0x0e000003, 
		0x80800000, 
		0x73e7f54d, 
		0x00000000, 
		0xc5f08f4e, 
		0xc1b80000, 
		0x425c0000, 
		0x0c7fff80, 
		0x41800000, 
		0x8475446a, 
		0x00800002, 
		0x0c7ffffe, 
		0xb807ced0, 
		0xff800000, 
		0xc23c0000, 
		0x41700000, 
		0x7fc00001, 
		0xcccccccc, 
		0xc2080000, 
		0x4207a1a5, 
		0xe04a81ad, 
		0x00000000, 
		0xc2380000, 
		0x01721a95, 
		0x00002000, 
		0x8dd3cdf5, 
		0xff7ffffe, 
		0xc2780000, 
		0x00000002, 
		0x0e000001, 
		0x80000000, 
		0xd3c0a00a, 
		0x80800000, 
		0x7fc8fd9c, 
		0x0c7ffffc, 
		0x00000800, 
		0x42440000, 
		0x0c7ffffc, 
		0xc2140000, 
		0xc1a00000, 
		0x1966cad2, 
		0xf9d2956f, 
		0xc22c0000, 
		0xc0400000, 
		0xc2200000, 
		0x0e000007, 
		0x00000000, 
		0x80000800, 
		0xc2500000, 
		0x0c7ffffe, 
		0x80800003, 
		0x41c00000, 
		0x00400000, 
		0x6bb6d4f3, 
		0xc1800000, 
		0x0c7ffe00, 
		0xc2280000, 
		0xdc5312ea, 
		0xff800000, 
		0x007fffff, 
		0x00200000, 
		0xc2640000, 
		0x7f90e77d, 
		0x80010000, 
		0x80000000, 
		0xff000000, 
		0x00800000, 
		0x75e93aa9, 
		0xc1d80000, 
		0xbf800001, 
		0x0c7e0000, 
		0x00000080, 
		0xe3d47951, 
		0xc2240000, 
		0x0c7fe000, 
		0x40a00000, 
		0x078e579b, 
		0x41700000, 
		0xc2680000, 
		0xc1a80000, 
		0xff000000, 
		0x80000000, 
		0x33ed6131, 
		0xc2300000, 
		0xffc044c6, 
		0x51c34cec, 
		0x80000000, 
		0x423c0000, 
		0x0e000007, 
		0xc1600000, 
		0x80008000, 
		0x00000000, 
		0x9fe24a96, 
		0xc0c00000, 
		0x42080000, 
		0x0c7f0000, 
		0xff800000, 
		0x0c7fc000, 
		0x0c7ffff8, 
		0xcb8c4b40, 
		0x7f800000, 
		0xff800000, 
		0xffffffff, 
		0x3f800001, 
		0x7f7fffff, 
		0x80800003, 
		0x42400000, 
		0x00200000, 
		0x80800002, 
		0x80000020, 
		0xcb000000, 
		0x00800002, 
		0x002264e0, 
		0x7fffffff, 
		0xc2740000, 
		0x0e03ffff, 
		0x00000000, 
		0x7b7b5c32, 
		0x42400000, 
		0x00010000, 
		0x0c7fc000, 
		0x3c5118ca, 
		0x5be17fab, 
		0x41900000, 
		0x0e000001, 
		0x0e001fff, 
		0xa595f550, 
		0xcb70dc10, 
		0x00004000, 
		0x42740000, 
		0x77a45b81, 
		0xc2740000, 
		0x0c6df1f8, 
		0x00800001, 
		0x42ee2cde, 
		0x0e03ffff, 
		0xff955b8f, 
		0xc1f00000, 
		0x7fc00000, 
		0xb6f61442, 
		0xc1500000, 
		0xb987399f, 
		0xc2280000, 
		0x80000000, 
		0xd2895392, 
		0x00100000, 
		0xbf028f5c, 
		0x00800000, 
		0x80020000, 
		0x6efad7c2, 
		0xc2100000, 
		0x0e000001, 
		0x41e00000, 
		0xffc00000, 
		0x08496623, 
		0x01d0a994, 
		0x1c5bdf0d, 
		0xc21c0000, 
		0x14ad4824, 
		0x80000000, 
		0xc1200000, 
		0x423c0000, 
		0xbf028f5c, 
		0x0e00ffff, 
		0x4384dcfc, 
		0x00000800, 
		0x18049235, 
		0xe1f33966, 
		0x426c0000, 
		0x8010fa90, 
		0x00004000, 
		0xc2680000, 
		0x80800002, 
		0x00000000, 
		0x6045f776, 
		0xd0817581, 
		0x7f800000, 
		0xff800000, 
		0xff800000, 
		0x80800000, 
		0x42480000, 
		0x00000000, 
		0x0f7ffffe, 
		0x41980000, 
		0x80000000, 
		0x0c7ff000, 
		0x41d00000, 
		0x80000000, 
		0x8daa0f85, 
		0x80800002, 
		0x7f800000, 
		0xff000000, 
		0x00000000, 
		0x41500000, 
		0x558a5b64, 
		0x00000000, 
		0x4b8c4b40, 
		0x41f80000, 
		0xf65f65db, 
		0x4b8c4b40, 
		0x420c0000, 
		0x00000010, 
		0x41700000, 
		0x0c7f8000, 
		0x975659e4, 
		0x0c7ffc00, 
		0x0e00ffff, 
		0xf6c23574, 
		0x242dab61, 
		0x41600000, 
		0x80000001, 
		0x0f7ffffe, 
		0x0c7fffff, 
		0x42600000, 
		0x7fe30c05, 
		0x421c0000, 
		0x80000010, 
		0x0ced3522, 
		0xc2300000, 
		0x0c7fffe0, 
		0xc1a00000, 
		0x80800000, 
		0x0d000ff0, 
		0x4483cfb9, 
		0x00000020, 
		0xc2740000, 
		0xc2580000, 
		0x0c7f8000, 
		0xb8a40fe2, 
		0x79c4ede5, 
		0x7d2cd41c, 
		0xffc00000, 
		0x00000000, 
		0x80000000, 
		0xc2540000, 
		0x5f325ffc, 
		0xd9cb3578, 
		0xff000000, 
		0xc26c0000, 
		0x00020000, 
		0x2540253a, 
		0x70e1a9ba, 
		0x41980000, 
		0x07518ee0, 
		0x80000000, 
		0xc0a00000, 
		0x0e0fffff, 
		0x41d80000, 
		0x80800000, 
		0x00800000, 
		0x41b00000, 
		0xc1500000, 
		0x7b1b59ae, 
		0x41100000, 
		0x41000000, 
		0xab6da1cd, 
		0x0f7ffffc, 
		0xc1e00000, 
		0x7c3a0494, 
		0x5d036ac7, 
		0x50436f6b, 
		0x42600000, 
		0xc1a00000, 
		0x41b00000, 
		0x47005d9f, 
		0xab2c32e2, 
		0x116d5797, 
		0x40c00000, 
		0x3f028f5c, 
		0xc2380000, 
		0x425c0000, 
		0x00000000, 
		0x6dc63a18, 
		0x7f000000, 
		0x3f800000, 
		0x00000000, 
		0x302abb2e, 
		0x00000040, 
		0x42280000, 
		0x0c0a13ae, 
		0x3a605ed5, 
		0x929609bd, 
		0xb043a97f, 
		0x247f9e06, 
		0xc2240000, 
		0xcc649ae4, 
		0x0e000001, 
		0xdf4a04ef, 
		0x00800003, 
		0xff800000, 
		0x08ef5f08, 
		0x80800003, 
		0xaac9c693, 
		0x7f800001, 
		0xff800000, 
		0xc21c0000, 
		0xd427e9b9, 
		0x41a80000, 
		0x424c0000, 
		0x42780000, 
		0x8fff3dbe, 
		0x0e00ffff, 
		0x807fffff, 
		0xff9d850e, 
		0x802bd77f, 
		0xe30ca342, 
		0xbef9e0d9, 
		0x41b00000, 
		0x41f80000, 
		0x0c7fffc0, 
		0xeaad79ac, 
		0x42100000, 
		0x6a621e36, 
		0x9b149cd5, 
		0x0c7c0000, 
		0x7f800000, 
		0x00100000, 
		0x7f7fffff, 
		0xd77a13d1, 
		0xc2040000, 
		0x15f2ccee, 
		0x41800000, 
		0x007ffffe, 
		0x0c7ffffc, 
		0x80800003, 
		0x1c77d31b, 
		0xc2080000, 
		0xff000000, 
		0x80000000, 
		0x8f7fffff, 
		0xc1a80000, 
		0x7fffffff, 
		0x51637ceb, 
		0x42300000, 
		0x40800000, 
		0x00000000, 
		0x00000100, 
		0x00000004, 
		0x41b00000, 
		0x00000000, 
		0x00000001, 
		0x304407b2, 
		0x00800000, 
		0x801485f2, 
		0xc5bd3d17, 
		0x40c00000, 
		0xff7ffffe, 
		0xfe762c82, 
		0x427c0000, 
		0x007fffff, 
		0x3182ade2, 
		0x2665400c, 
		0x7f7ffffe, 
		0x7fc00000, 
		0x41f80000, 
		0x4cb4b4c3, 
		0x00400000, 
		0x0e07ffff, 
		0x7f8a901d, 
		0x40400000, 
		0x00200000, 
		0xdd7ab983, 
		0x00000080, 
		0x40e26dd7, 
		0x007ffffe, 
		0x330c389f, 
		0xc1100000, 
		0x5a3e8801, 
		0xffbfffff, 
		0x425c0000, 
		0x782b2c1a, 
		0xc1e80000, 
		0xc0a00000, 
		0xf46dcb9f, 
		0x971f2031, 
		0x0e0fffff, 
		0x42780000, 
		0x42480000, 
		0x00000800, 
		0x0e00003f, 
		0x4b000000, 
		0x0f7ffffd, 
		0x7fb927e5, 
		0x3c4ca83c, 
		0x80000040, 
		0x80000001, 
		0x0e007fff, 
		0x42440000, 
		0x8f7ffffe, 
		0x00000000, 
		0xc1000000, 
		0x41700000, 
		0x0e07ffff, 
		0xff800000, 
		0x41500000, 
		0x80000000, 
		0x7f800000, 
		0x7f7ffffe, 
		0x3728b951, 
		0xc95aa228, 
		0x7f800000, 
		0x0c7c0000, 
		0xbfdf198a, 
		0x00800003, 
		0x7f943f3f, 
		0x7f800000, 
		0x41980000, 
		0x7fa2736b, 
		0xada5b76d, 
		0x80000010, 
		0xc1c00000, 
		0x208f7f4f, 
		0x41b00000, 
		0xc5c7820d, 
		0x364cce1a, 
		0x58e61289, 
		0x0f7ffffc, 
		0x6a6db1a1, 
		0xc2580000, 
		0x0c7ff800, 
		0xd09f2f94, 
		0x9d6062be, 
		0x3a91c2b4, 
		0x80800000, 
		0x42400000, 
		0x00000000, 
		0x80000000, 
		0x222444c9, 
		0x0e00ffff, 
		0x72d0c4a3, 
		0x0c7ffff8, 
		0x00040000, 
		0x1ae5c397, 
		0x80800003, 
		0x2e2cffdc, 
		0x42540000, 
		0xcccccccc, 
		0x00040000, 
		0x00000000, 
		0x0e003fff, 
		0x0e00007f, 
		0x80000000, 
		0x8f7ffffd, 
		0xf1c15094, 
		0x00000002, 
		0x41200000, 
		0x80000002, 
		0x0c7ffff0, 
		0xffc00000, 
		0x0e001fff, 
		0x7f800000, 
		0x14a81405, 
		0xff7ffffe, 
		0x80000200, 
		0x0e1fffff, 
		0x1a60920d, 
		0xb230423c, 
		0xff7fffff, 
		0x00080000, 
		0xfb3ba708, 
		0xc0000000, 
		0x41e00000, 
		0x0e1fffff, 
		0x88405c32, 
		0x00010000, 
		0x7f7ffffe, 
		0xa294d4c2, 
		0x98c84ded, 
		0x51f5180a, 
		0xf4f3bfe2, 
		0x7b191e2d, 
		0x29a64500, 
		0xff800000, 
		0x343ff9fa, 
		0x4d3935f7, 
		0x0e07ffff, 
		0x80000000, 
		0x8e5952b1, 
		0x00000000, 
		0x41d80000, 
		0xd66ad5d6, 
		0x6734336d, 
		0x00000000, 
		0x41b80000, 
		0x40e00000, 
		0x00000000, 
		0xef96110d, 
		0x4afb565c, 
		0xc1700000, 
		0x00000000, 
		0xc0000000, 
		0x42140000, 
		0x80002000, 
		0x80000000, 
		0xc2580000, 
		0x0d5d1c99, 
		0xc8c266af, 
		0x40c00000, 
		0x00000010, 
		0x00040000, 
		0xb3e4e47c, 
		0x0a3b9a95, 
		0x7fc00000, 
		0x00000000, 
		0x7fc00001, 
		0x00000000, 
		0x0c9da1f1, 
		0x7f7ffffe, 
		0x23a3ef37, 
		0x3d872468, 
		0x0c7fffff, 
		0x00000000, 
		0x0c7ffff8, 
		0xc0a00000, 
		0x0c7fffff, 
		0x0c7fff00, 
		0x00000000, 
		0x0e00003f, 
		0x42080000, 
		0xff7ffffe, 
		0xc2040000, 
		0xe73e76cc, 
		0x00000020, 
		0x00000100, 
		0x42240000, 
		0x40e00000, 
		0x00000000, 
		0xc1d00000, 
		0x00000000, 
		0xc1c80000, 
		0x0a3b264b, 
		0x3f800000, 
		0xcb2e9181, 
		0xc2780000, 
		0x0e0fffff, 
		0xffffffff, 
		0x7fb884b2, 
		0x0c7fffff, 
		0xc2500000, 
		0x077b51ed, 
		0xcccccccc, 
		0x33d08d5a, 
		0x7fc00001, 
		0x64500fea, 
		0x40e00000, 
		0xc1100000, 
		0xb28fd03d, 
		0x00000000, 
		0x80800000, 
		0x0c7fff80, 
		0x0c7f0000, 
		0xc1b00000, 
		0x84851c78, 
		0xa9e1cfe8, 
		0x80010000, 
		0x0e1fffff, 
		0x00000002, 
		0xfeed85d4, 
		0xafe1006a, 
		0x8f7fffff, 
		0x00000040, 
		0x4047a341, 
		0x0c7ffe00, 
		0x0e0007ff, 
		0xbf800001, 
		0x0e00000f, 
		0x90291460, 
		0x80000000, 
		0x0c7ffffe, 
		0x0c7ffc00, 
		0x42200000, 
		0xd204e655, 
		0x00000000, 
		0x41800000, 
		0xff800000, 
		0xff800000, 
		0x41600000, 
		0x91af71ca, 
		0x0c7ffe00, 
		0x41f80000, 
		0x1c9466a6, 
		0xa5571298, 
		0x42480000, 
		0x43dfe964, 
		0xf7b2d7de, 
		0x00010000, 
		0x0c7f8000, 
		0xc22c0000, 
		0x64130e78, 
		0x42180000, 
		0x00000000, 
		0x80800000, 
		0x00000000, 
		0x0e7fffff, 
		0x00000000, 
		0x00000000, 
		0x0e7fffff, 
		0x00100000, 
		0xff800000, 
		0xc25c0000, 
		0x80000008, 
		0x91517b6a, 
		0x7f800000, 
		0x007ffffe, 
		0x7f8ac41b, 
		0xc1a80000, 
		0xc20c0000, 
		0x703d8e49, 
		0x53f188bc, 
		0x7066c2df, 
		0x7f800000, 
		0x00000010, 
		0xc1980000, 
		0x80000002, 
		0x00011111, 
		0x060b215a, 
		0x80200000, 
		0x00000000, 
		0x9f1b1c13, 
		0xff800000, 
		0x00800001, 
		0x0f7ffffd, 
		0x41900000, 
		0x00000080, 
		0x0e3fffff, 
		0x3bb2c07c, 
		0x80800002, 
		0xd3204934, 
		0x00000000, 
		0x0c7e0000, 
		0x0c700000, 
		0x7f7ffffe, 
		0x04365b3d, 
		0x00011111, 
		0x0c780000, 
		0xc20c0000, 
		0xcd58f77e, 
		0x7a61ca2e, 
		0x2a84901c, 
		0x00800000, 
		0x7f800000, 
		0x0c7fff00, 
		0x425c0000, 
		0x80000004, 
		0x0c7c0000, 
		0x1b987118, 
		0x88dc57c5, 
		0x10cd268e, 
		0x80800001, 
		0xb52d7f85, 
		0x681c0713, 
		0x78183734, 
		0xc2440000, 
		0xd57cf391, 
		0x0f7ffffd, 
		0xffb976d2, 
		0x42340000, 
		0xff800000, 
		0x2a3d517c, 
		0x40000000, 
		0x7f7ffffe, 
		0xc1880000, 
		0x42100000, 
		0xc21c0000, 
		0xf0b16f1c, 
		0xc2300000, 
		0x80000040, 
		0x80000002, 
		0xc23c0000, 
		0xc1f00000, 
		0x80000000, 
		0xc1600000, 
		0x3d3fc3ed, 
		0x00000000, 
		0x0c7ffffc, 
		0x5cea30a2, 
		0x0c7fffe0, 
		0x0e01ffff, 
		0x42000000, 
		0x0d5da483, 
		0x0e00001f, 
		0xff800001, 
		0xc2580000, 
		0xc2780000, 
		0x7f000000, 
		0x469fa9a8, 
		0x00000000, 
		0xc2300000, 
		0xc0e00000, 
		0x80000000, 
		0x42180000, 
		0x422c0000, 
		0xffbfffff, 
		0xe82441d6, 
		0x4b8c4b40, 
		0x20c4513f, 
		0x42500000, 
		0x5d9cc3e0, 
		0x80000000, 
		0x80000000, 
		0x4b8c4b40, 
		0xc2280000, 
		0xb14cb711, 
		0xc2480000, 
		0x0c400000, 
		0x42540000, 
		0x0e000001, 
		0x0e7fffff, 
		0x69e62667, 
		0x00000008, 
		0x40e00000, 
		0xa0f0775d, 
		0x42340000, 
		0xc2100000, 
		0x80000010, 
		0x7bc2f01a, 
		0x0f7ffffc, 
		0xffc30c6e, 
		0xc1600000, 
		0xff7fffff, 
		0xc21c0000, 
		0x337c9b3f, 
		0x5bea7e4d, 
		0x0c7e0000, 
		0x41a80000, 
		0xc6331ef9, 
		0x8f7ffffd, 
		0xc0a00000, 
		0x41200000, 
		0xd42e124b, 
		0x41f80000, 
		0x9236d919, 
		0x80100000, 
		0x40000000, 
		0x00000040, 
		0x0c7c0000, 
		0xff800000, 
		0xc0a00000, 
		0xbc396729, 
		0xc1d00000, 
		0x00000200, 
		0xfa16500b, 
		0xff800000, 
		0x7f800000, 
		0x42600000, 
		0x41a00000, 
		0x508c6a08, 
		0xc1a80000, 
		0x00008000, 
		0x80108376, 
		0x80800003, 
		0x80000800, 
		0xc1800000, 
		0xc2540000, 
		0x40e00000, 
		0x80040000, 
		0xa40c630d, 
		0x80800000, 
		0xb75cd5d8, 
		0x80800000, 
		0x24bc210d, 
		0x31e9f577, 
		0x2ea017ff, 
		0x2b551d81, 
		0x00000000, 
		0x00000001, 
		0xc2280000, 
		0x80000000, 
		0x3cbb7a95, 
		0x41000000, 
		0x7f7ffffe, 
		0x42300000, 
		0x26135a13, 
		0x0b412af1, 
		0x00000000, 
		0xfc98281b, 
		0xc2600000, 
		0xc2200000, 
		0x80800000, 
		0x80000000, 
		0x7fa88d0f, 
		0x03482699, 
		0xffffffff, 
		0x42740000, 
		0x0ecc657e, 
		0x00000100, 
		0x41c80000, 
		0x80800001, 
		0xc2400000, 
		0x7f000000, 
		0xc1880000, 
		0xc0e00000, 
		0x7f800001, 
		0xcbab4a55, 
		0xeafae7cf, 
		0xbf80d731, 
		0x8f7ffffe, 
		0x424c0000, 
		0x80000001, 
		0xffc00001, 
		0xc2100000, 
		0xbf028f5c, 
		0x4c67b5f8, 
		0xc1880000, 
		0xc7209c97, 
		0xc1d00000, 
		0x2c6d7e22, 
		0x80010000, 
		0x9fcca424, 
		0x8015325c, 
		0x78388e2b, 
		0xe944b565, 
		0x2dabe252, 
		0x7f800000, 
		0xaaaaaaaa, 
		0x49516632, 
		0xffffffff, 
		0x0c7f8000, 
		0x41600000, 
		0xff7ffffe, 
		0x00010000, 
		0xc21c0000, 
		0x69caaa51, 
		0x41000000, 
		0x460b49f4, 
		0x40c00000, 
		0x80020000, 
		0x80800000, 
		0xcd3f347f, 
		0xc2440000, 
		0x42500000, 
		0x00800002, 
		0xcb8c4b40, 
		0xaa88b7ca, 
		0x00800001, 
		0x41200000, 
		0xff800001, 
		0x0e00ffff, 
		0x80000002, 
		0x80800001, 
		0xb3745830, 
		0x0e007fff, 
		0x00004000, 
		0x42500000, 
		0x0e00ffff, 
		0x41600000, 
		0x8f11bedd, 
		0x3c78a093, 
		0xc1d80000, 
		0x007fffff, 
		0xc0400000, 
		0x0e1fffff, 
		0x40c00000, 
		0x80ffffff, 
		0x00020000, 
		0xff7ffffe, 
		0x0c7ff000, 
		0x00ffffff, 
		0x80000000, 
		0x3ffed038, 
		0x80080000, 
		0x6ebf5ee7, 
		0xc27c0000, 
		0x48548861, 
		0x264654e0, 
		0xaaaaaaaa, 
		0xffc00000, 
		0x80000000, 
		0xc1600000, 
		0x166bb23c, 
		0x7fc00000, 
		0x5c5efc36, 
		0xec5c2520, 
		0xf6d4a948, 
		0x0e03ffff, 
		0xc1500000, 
		0x426c0000, 
		0x7f800000, 
		0x1e3c1149, 
		0x0f7ffffd, 
		0xc1200000, 
		0x3f028f5c, 
		0xc1600000, 
		0xffaaeb5e, 
		0x42640000, 
		0x807fffff, 
		0xc2340000, 
		0x80000800, 
		0x853d96b5, 
		0x42240000, 
		0x1bec2b28, 
		0x00000000, 
		0x41300000, 
		0x7fc00000, 
		0xd2946cec, 
		0xb24f69f9, 
		0x0e001fff, 
		0x80000000, 
		0x80000000, 
		0x42700000, 
		0x80000000, 
		0x0e00000f, 
		0x7bd22b02, 
		0x0e001fff, 
		0x00000000, 
		0x7fbe1214, 
		0x918caae6, 
		0x3f028f5c, 
		0x95b97328, 
		0x5208fb97, 
		0x0c7fffc0, 
		0x0c7ffffe, 
		0x42180000, 
		0x7fc00001, 
		0x0c7ffff0, 
		0x0e1fffff, 
		0x8037dacc, 
		0xb605968e, 
		0x0f7fffff, 
		0x0e003fff, 
		0x69e34bfe, 
		0x07976037, 
		0xb971ed3e, 
		0xc0a00000, 
		0x00000002, 
		0xbf800001, 
		0x00001000, 
		0xd9bc2a89, 
		0x8789e753, 
		0x41e80000, 
		0xc6cfe12d, 
		0x27d222fb, 
		0x00000000, 
		0x00000000, 
		0x7f800000, 
		0x41b80000, 
		0x4683f71d, 
		0x00400000, 
		0x00008000, 
		0xc3c6548c, 
		0x0c7ffff8, 
		0x0e00000f, 
		0x00000000, 
		0x2415adc2, 
		0x00800001, 
		0x40400000, 
		0xff800000, 
		0x5c94e55f, 
		0x0e00001f, 
		0x80100000, 
		0x0e01ffff, 
		0x80000000, 
		0x7fc00001, 
		0x00000000, 
		0xcb000000, 
		0x7fbfffff, 
		0xc2380000, 
		0xff7fffff, 
		0x208e2ce6, 
		0xff7fffff, 
		0x41d80000, 
		0x12c5fcbc, 
		0xff000000, 
		0x41800000, 
		0x40c00000, 
		0xff800001, 
		0x41a00000, 
		0x80000000, 
		0xc7ec01e3, 
		0xc1800000, 
		0x7fa7c711, 
		0xff800000, 
		0x00800003, 
		0xdd5551f4, 
		0x7f800001, 
		0xff000000, 
		0xff7ffffe, 
		0xc1300000, 
		0x00000000, 
		0x41700000, 
		0x1a4ee577, 
		0x14e1386f, 
		0x0d000ff0, 
		0xb0631473, 
		0x80000400, 
		0x0e000003, 
		0x0e000003, 
		0x6896607e, 
		0xff800000, 
		0x1ad3b461, 
		0xa8f6b7a7, 
		0x0e0001ff, 
		0x40000000, 
		0x80000000, 
		0xff12960b, 
		0xc2400000, 
		0x422c0000, 
		0x00687d31, 
		0x00001000, 
		0x41e00000, 
		0x0e0003ff, 
		0x0c7fe000, 
		0xffb80753, 
		0x40c00000, 
		0xffa06c25, 
		0xae4a5e73, 
		0xa48ed7b6, 
		0x00400000, 
		0x92983f57, 
		0xc2740000, 
		0xc2a8dc5e, 
		0x80800000, 
		0x02500d18, 
		0x96ccce84, 
		0x00400000, 
		0x00800002, 
		0x41400000, 
		0x40000000, 
		0x28cd7200, 
		0x42440000, 
		0x00400000, 
		0xc2240000, 
		0x0c700000, 
		0xff800000, 
		0x05a41122, 
		0x41000000, 
		0x7fe3adc2, 
		0x80616094, 
		0x8f7fffff, 
		0x42540000, 
		0x0c7ffc00, 
		0x857e5e3a, 
		0xff80f8f5, 
		0xff800000, 
		0x00040000, 
		0xd7ae0085, 
		0x0c7fffff, 
		0x0c7ffffc, 
		0xc2640000, 
		0xc0000000, 
		0xc0800000, 
		0x7fffffff, 
		0x007ccc55, 
		0xff800000, 
		0x896d820d, 
		0xc1b80000, 
		0x00004000, 
		0x69baa1d6, 
		0x00000100, 
		0x8f7ffffc, 
		0x7f000000, 
		0x40a00000, 
		0x9a48eaad, 
		0xc2300000, 
		0xcb000000, 
		0x7f7ffffe, 
		0x42180000, 
		0xaf3b89ae, 
		0x75992054, 
		0x6fa7f88d, 
		0x0c7f8000, 
		0x80000400, 
		0xff800000, 
		0x0e03ffff, 
		0x42280000, 
		0x00800001, 
		0xc22c0000, 
		0xff9e8c65, 
		0x423c0000, 
		0xe1e69778, 
		0x00000100, 
		0x41c00000, 
		0x80000080, 
		0x8f7ffffe, 
		0xa59a0187, 
		0x0e000007, 
		0xde38cecc, 
		0x26b0fee3, 
		0x3f800000, 
		0xace248b5, 
		0x42080000, 
		0x00000000, 
		0xff800000, 
		0xeac5ce89, 
		0x41e80000, 
		0xd4302377, 
		0xffc773a7, 
		0x217054da, 
		0x0e007fff, 
		0xa47f282d, 
		0xf6c5b346, 
		0x21821566, 
		0x7c14d8bd, 
		0x41d80000, 
		0x0c7ff000, 
		0x00008000, 
		0x7f800000, 
		0x0e6dab83, 
		0x420c0000, 
		0x41c80000, 
		0x3f800001, 
		0x00000004, 
		0x30a7cca7, 
		0xcb8c4b40, 
		0x0e0007ff, 
		0xc2100000, 
		0x7fc00001, 
		0x85477dd5, 
		0x3eea6e0b, 
		0x3f800000, 
		0x42780000, 
		0x80800003, 
		0x00000004, 
		0x80000000, 
		0xc1c80000, 
		0x067ee118, 
		0x5f4f3dba, 
		0x80800001, 
		0x0e0001ff, 
		0x41a00000, 
		0x42280000, 
		0x00000000, 
		0x41500000, 
		0x40a00000, 
		0xff7ffffe, 
		0x3bb9d251, 
		0x8762b157, 
		0x589f5811, 
		0xff7ffffe, 
		0xc25c0000, 
		0x4519ad10, 
		0x80000001, 
		0x32016168, 
		0x0e007fff, 
		0x0c7ffff8, 
		0xbd88608e, 
		0xc9281208, 
		0x00004000, 
		0x1dd91cae, 
		0x0c700000, 
		0xff800000, 
		0x6a450918, 
		0x00040000, 
		0x42180000, 
		0x80000200, 
		0x00000020, 
		0x53939b2e, 
		0xcb8c4b40, 
		0xc1c80000, 
		0x3d3ac243, 
		0x00800002, 
		0x41a00000, 
		0x67d417c6, 
		0xcb285adb, 
		0x00010000, 
		0x42300000, 
		0x80008000, 
		0x41300000, 
		0xc1a80000, 
		0xbd3c220d, 
		0x42540000, 
		0x7f7fffff, 
		0xc1d00000, 
		0x0c7fe000, 
		0x00800000, 
		0xfc75d000, 
		0x00000100, 
		0xad7978e6, 
		0x3f800000, 
		0xf6afb4eb, 
		0x0e04529d, 
		0x80800001, 
		0xc2380000, 
		0x80000000, 
		0x8f7ffffe, 
		0x0f7ffffe, 
		0xcb8c4b40, 
		0xc1b00000, 
		0x80000000, 
		0x0c7ffe00, 
		0x7f800000, 
		0xef0d073e, 
		0x7f7ffffe, 
		0x00001000, 
		0x4b8c4b40, 
		0xf66cedf7, 
		0x427c0000, 
		0x00000000, 
		0x41f80000, 
		0x4bd3fc63, 
		0x64bbe390, 
		0xc2500000, 
		0xff9ae6bf, 
		0x7f800000, 
		0x00800003, 
		0x427c0000, 
		0x6ce7e233, 
		0x0c7ffffc, 
		0x1cfbbb31, 
		0x421c0000, 
		0x0e0007ff, 
		0x3f028f5c, 
		0xff8d884f, 
		0x0d000ff0, 
		0xc2080000, 
		0x3456d3ed, 
		0x4b8c4b40, 
		0x00800002, 
		0xc2680000, 
		0xf657e823, 
		0xc0a00000, 
		0x76290a3e, 
		0x80000000, 
		0x41c80000, 
		0x00800002, 
		0x395d2f1e, 
		0x0ea2e22d, 
		0x0c7ff800, 
		0x67a3d24e, 
		0x00040000, 
		0x42700000, 
		0x80000000, 
		0x4ca93e5b, 
		0x7077d1af, 
		0x0c7f0000, 
		0xbf800001, 
		0x00800000, 
		0x80ffffff, 
		0x00001000, 
		0x7fc00001, 
		0x40a00000, 
		0xafdf4c21, 
		0xff7fffff, 
		0x2ec27a94, 
		0x42040000, 
		0x770b9872, 
		0x80000200, 
		0x7fffffff, 
		0xc1a00000, 
		0xd46ec110, 
		0xc1300000, 
		0xc1f80000, 
		0x4c333cc7, 
		0xc2280000, 
		0x001b66b0, 
		0x42280000, 
		0xffbfffff, 
		0xd29d5ff0, 
		0xc1100000, 
		0x0d000ff0, 
		0x41300000, 
		0x1849319a, 
		0x427c0000, 
		0x0c780000, 
		0x7f800000, 
		0xf44aa36b, 
		0x0a606484, 
		0x0d000ff0, 
		0xc5da8ba3, 
		0x662d1873, 
		0xc2140000, 
		0xc1880000, 
		0x137a22eb, 
		0x00040000, 
		0x0e000001, 
		0xc27c0000, 
		0x5ccd0056, 
		0xc1b00000, 
		0x7f7fffff, 
		0x80800002, 
		0xbb5f0409, 
		0x84d5aa4b, 
		0xde8633f3, 
		0x00000000, 
		0x2ebe59d9, 
		0x0019ea80, 
		0x80800003, 
		0x427c0000, 
		0x00080000, 
		0x201dc4a1, 
		0x80000000, 
		0x0e000fff, 
		0x80ffffff, 
		0x00000000, 
		0xa0d4997a, 
		0x41200000, 
		0x35768b92, 
		0xc2440000, 
		0xc0a00000, 
		0x8f7ffffd, 
		0x004c9dd8, 
		0x0cee4405, 
		0x4b000000, 
		0xd807b441, 
		0x2e6b8c50, 
		0x80000000, 
		0x80800000, 
		0x41800000, 
		0x0c7ffffe, 
		0x7f7fffff, 
		0xc0400000, 
		0x0c7fffc0, 
		0x0e0003ff, 
		0xfdf157e0, 
		0xc1c80000, 
		0x509af712, 
		0x00000000, 
		0x42480000, 
		0x80000000, 
		0xc8bcb00b, 
		0x00800001, 
		0x667a1b0a, 
		0x90b7405c, 
		0x42640000, 
		0x0c7f8000, 
		0x80000000, 
		0xc2300000, 
		0x0c7fff80, 
		0x8f7ffffc, 
		0x7f800000, 
		0xff999c73, 
		0x06391dca, 
		0xc1a00000, 
		0xbd9ef482, 
		0x80000000, 
		0x8848f0e3, 
		0x6653f20d, 
		0x00000000, 
		0xc1b00000, 
		0xc1880000, 
		0xc1a00000, 
		0x0c7fff80, 
		0x7f800000, 
		0x0c7f8000, 
		0x00000000, 
		0xffc00000, 
		0xc1980000, 
		0x00800002, 
		0x80000002, 
		0x4b8c4b40, 
		0xaaaaaaaa, 
		0x0b4cc858, 
		0x00000010, 
		0x80800000, 
		0xff7ffffe, 
		0x00000000, 
		0x80000000, 
		0xff7fffff, 
		0x309260c2, 
		0x80800000, 
		0x25294175, 
		0xc4245863, 
		0x8167be36, 
		0xb3b41973, 
		0x7fc00000, 
		0x7f800000, 
		0x1e87a8ef, 
		0x7f800000, 
		0xa2d59e52, 
		0x0e000001, 
		0xd2e399b1, 
		0x40c00000, 
		0x7f800000, 
		0xc2580000, 
		0x80800003, 
		0x0e1fffff, 
		0x0c7fc000, 
		0xff800000, 
		0x00000000, 
		0x42180000, 
		0x41c80000, 
		0x7f800000, 
		0xd7eebe14, 
		0x52744f2e, 
		0xffc00001, 
		0xc2100000, 
		0x00000000, 
		0x03202d31, 
		0xedb555d8, 
		0x807fffff, 
		0x7f800000, 
		0xcccccccc, 
		0x1df2475f, 
		0xc25c0000, 
		0xc84006f6, 
		0x42240000, 
		0xbb9810e8, 
		0xc2500000, 
		0x42500000, 
		0xc2700000, 
		0xc2440000, 
		0x426c0000, 
		0x3f800001, 
		0x0e0007ff, 
		0x807fffff, 
		0x198aded6, 
		0xe9f0dcde, 
		0x613a3af3, 
		0x7fc00001, 
		0x0dc30d9b, 
		0xaa693e16, 
		0x41880000, 
		0xc1800000, 
		0x3f028f5c, 
		0x7f800000, 
		0x3f800001, 
		0x807ffffe, 
		0x80800003, 
		0x00010000, 
		0x78fb8e9d, 
		0x2fc8c7ac  
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
		0x00000173, 
		0x00000129, 
		0x000001d3, 
		0x00000387, 
		0x000002ed, 
		0x0000030d, 
		0x0000011b, 
		0x0000024d, 
		0x000000cf, 
		0x000003ad, 
		0x00000133, 
		0x000002cd, 
		0x00000283, 
		0x0000037f, 
		0x00000059, 
		0x000001eb, 
		0x00000361, 
		0x00000013, 
		0x000002ef, 
		0x00000283, 
		0x0000020b, 
		0x000001d3, 
		0x000000fb, 
		0x000002e5, 
		0x00000315, 
		0x00000351, 
		0x00000251, 
		0x00000075, 
		0x00000371, 
		0x0000007b, 
		0x000003fd, 
		0x00000117, 
		0x00000155, 
		0x000003f9, 
		0x000000a5, 
		0x000001f3, 
		0x000003e3, 
		0x0000016b, 
		0x000000fd, 
		0x00000267, 
		0x0000016d, 
		0x000003db, 
		0x00000031, 
		0x000001bb, 
		0x0000015f, 
		0x00000381, 
		0x00000109, 
		0x0000014d, 
		0x000003a1, 
		0x00000325, 
		0x0000026f, 
		0x000002cd, 
		0x0000039f, 
		0x00000209, 
		0x000000c7, 
		0x000000af, 
		0x0000030f, 
		0x000000c3, 
		0x0000002d, 
		0x000001dd, 
		0x00000247, 
		0x0000038d, 
		0x000000e5, 
		0x0000017f, 
		0x00000043, 
		0x00000307, 
		0x00000093, 
		0x000000ed, 
		0x0000033f, 
		0x00000259, 
		0x000000c5, 
		0x000001ff, 
		0x00000275, 
		0x000001ab, 
		0x000001a9, 
		0x000003b1, 
		0x0000032b, 
		0x00000253, 
		0x000003b7, 
		0x00000159, 
		0x0000011f, 
		0x0000033d, 
		0x000000ff, 
		0x0000034d, 
		0x000003eb, 
		0x00000145, 
		0x0000038b, 
		0x000001b3, 
		0x0000026d, 
		0x00000323, 
		0x000000c9, 
		0x00000105, 
		0x000001b7, 
		0x000001cf, 
		0x0000038d, 
		0x0000030f, 
		0x0000037d, 
		0x00000371, 
		0x00000269, 
		0x000003f5, 
		0x000001dd, 
		0x00000015, 
		0x0000014d, 
		0x0000003b, 
		0x0000000b, 
		0x000001f5, 
		0x000001fd, 
		0x000001df, 
		0x0000009d, 
		0x00000201, 
		0x00000259, 
		0x00000297, 
		0x000001b1, 
		0x00000311, 
		0x0000027d, 
		0x0000025f, 
		0x0000039f, 
		0x0000014f, 
		0x0000000d, 
		0x000002c9, 
		0x0000025d, 
		0x0000028d, 
		0x00000103, 
		0x0000013f, 
		0x00000151, 
		0x000000af, 
		0x00000059, 
		0x0000029b, 
		0x000002c9, 
		0x000000d1, 
		0x00000391, 
		0x0000011f, 
		0x000002fb, 
		0x000000df, 
		0x000002c7, 
		0x00000289, 
		0x000001b7, 
		0x00000341, 
		0x0000030d, 
		0x00000079, 
		0x00000335, 
		0x000003c1, 
		0x000000cf, 
		0x000003a7, 
		0x00000227, 
		0x000002ab, 
		0x00000003, 
		0x00000047, 
		0x000001e5, 
		0x00000243, 
		0x0000016d, 
		0x00000051, 
		0x00000363, 
		0x00000291, 
		0x00000055, 
		0x000000d3, 
		0x000000f7, 
		0x00000377, 
		0x00000155, 
		0x000002df, 
		0x00000339, 
		0x000001f5, 
		0x00000029, 
		0x00000039, 
		0x00000041, 
		0x00000257, 
		0x0000003f, 
		0x000002e7, 
		0x00000397, 
		0x000003fd, 
		0x000000f3, 
		0x000001ef, 
		0x00000345, 
		0x0000035d, 
		0x000003d1, 
		0x000002bb, 
		0x000001b9, 
		0x000003fb, 
		0x00000183, 
		0x000000bb, 
		0x000000f7, 
		0x0000030d, 
		0x0000004f, 
		0x00000091, 
		0x00000165, 
		0x00000057, 
		0x00000329, 
		0x00000283, 
		0x000000d7, 
		0x0000034d, 
		0x00000265, 
		0x00000135, 
		0x0000026d, 
		0x00000301, 
		0x000002cf, 
		0x0000028d, 
		0x0000017f, 
		0x000001d3, 
		0x00000371, 
		0x000000d7, 
		0x000003df, 
		0x000003ef, 
		0x0000022b, 
		0x00000049, 
		0x000000b7, 
		0x000001d1, 
		0x000001af, 
		0x0000004d, 
		0x00000309, 
		0x00000055, 
		0x00000231, 
		0x000000ed, 
		0x00000231, 
		0x000000e3, 
		0x00000259, 
		0x00000315, 
		0x000001e1, 
		0x0000025b, 
		0x000001e1, 
		0x000000c7, 
		0x0000005d, 
		0x000000d7, 
		0x0000025b, 
		0x000001eb, 
		0x00000301, 
		0x00000075, 
		0x000001ad, 
		0x000003db, 
		0x0000015b, 
		0x000001c9, 
		0x0000006f, 
		0x0000006b, 
		0x000000f7, 
		0x000003ff, 
		0x0000008d, 
		0x0000033d, 
		0x00000029, 
		0x00000241, 
		0x0000004b, 
		0x00000057, 
		0x00000017, 
		0x00000369, 
		0x00000309, 
		0x0000037d, 
		0x00000155, 
		0x000002f9, 
		0x00000009, 
		0x00000175, 
		0x0000030f, 
		0x00000089, 
		0x000001ff, 
		0x0000036b, 
		0x0000023f, 
		0x00000091, 
		0x00000293, 
		0x00000077  
};

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
			"li x5, 444\n"
			"faddi.pi f2, f0, -192\n"
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
			"fmsub.ps f21, f20, f21, f20\n"
			"fnmsub.ps f22, f26, f24, f29\n"
			"fmadd.ps f29, f31, f22, f21\n"
			"4:\n"
			"flog.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"bne x0, x0, 4b\n"
			"flog.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"fadd.ps f29, f27, f24\n"
			"fadd.ps f21, f24, f28\n"
			"fadd.pi f28, f28, f21\n"
			"fsub.pi f26, f25, f31\n"
			"fmax.ps f21, f28, f30\n"
			"fadd.pi f27, f27, f21\n"
			"fmin.ps f27, f24, f31\n"
			"fadd.ps f20, f21, f31\n"
			"flog.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"flog.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"flog.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"fand.pi f31, f30, f23\n"
			"fadd.ps f31, f25, f25\n"
			"fmax.ps f23, f25, f26\n"
			"fmul.ps f29, f31, f28\n"
			"fmin.pi f28, f24, f23\n"
			"fmax.pi f28, f27, f31\n"
			"fmin.ps f25, f30, f28\n"
			"fmin.pi f23, f31, f24\n"
			"fmul.pi f26, f26, f31\n"
			"fadd.pi f25, f28, f26\n"
			"1:\n"
			"flog.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"bne x0, x0, 1b\n"
			"flog.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"fmul.ps f28, f31, f26\n"
			"fsub.ps f31, f26, f29\n"
			"fadd.pi f30, f29, f30\n"
			"fmul.pi f30, f30, f28\n"
			"fmax.pi f30, f31, f29\n"
			"fmin.pi f25, f27, f31\n"
			"fmul.ps f30, f28, f31\n"
			"fmin.ps f27, f30, f29\n"
			"fadd.pi f31, f30, f25\n"
			"flog.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"fmadd.ps f31, f29, f29, f29\n"
			"fnmadd.ps f28, f31, f30, f30\n"
			"fnmadd.ps f31, f26, f27, f26\n"
			"fnmadd.ps f29, f30, f31, f26\n"
			"fmadd.ps f28, f26, f27, f27\n"
			"fmadd.ps f28, f29, f29, f28\n"
			"fmadd.ps f28, f26, f26, f30\n"
			"fmsub.ps f29, f27, f29, f26\n"
			"fnmsub.ps f30, f29, f26, f26\n"
			"fmadd.ps f26, f26, f29, f29\n"
			"flog.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"fmin.ps f30, f28, f31\n"
			"fmul.ps f29, f27, f29\n"
			"fmul.pi f29, f29, f29\n"
			"fmul.pi f29, f29, f30\n"
			"fmul.pi f28, f30, f28\n"
			"fsub.ps f28, f29, f27\n"
			"fmax.pi f31, f31, f27\n"
			"fadd.ps f31, f31, f31\n"
			"fmax.pi f28, f31, f29\n"
			"fsub.ps f27, f28, f31\n"
			"flog.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"flog.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"fmin.ps f31, f30, f29\n"
			"fmin.ps f29, f31, f29\n"
			"fmax.ps f30, f30, f30\n"
			"fadd.pi f29, f30, f29\n"
			"fand.pi f31, f30, f29\n"
			"flog.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"flog.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"flog.ps f0, f16\n"
			"faddi.pi f16, f16, 128\n"
			"flog.ps f1, f17\n"
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
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
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
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
			"li x0, 0x2\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x3\n" 
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
			"la x27, rand_int_32\n"
			"flw.ps f9, 736(x27)\n"
			"fpackreph.pi f16, f9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 608(x25)\n"
			"fpackrepb.pi f21, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 832(x27)\n"
			"flw.ps f28, 992(x27)\n"
			"fmul.pi f2, f17, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 800(x17)\n"
			"faddi.pi f14, f29, 0xd7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"fsetm.pi m4, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"mov.m.x m2, x13, 0x41\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x610555d8a8\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 96(x11)\n"
			"flw.ps f5, 96(x11)\n"
			"flw.ps f4, 480(x11)\n"
			"fmsub.s f4, f22, f5, f4, rne\n" 
			"mova.x.m x28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 800(x23)\n"
			"fandi.pi f24, f9, 0x9\n" 
			"li x15, 0xcc27f19ce10a7611\n"
			"fcvt.s.l f0, x15, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 224(x12)\n"
			"flw.ps f9, 160(x12)\n"
			"fand.pi f9, f13, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 288(x28)\n"
			"flw.ps f22, 128(x28)\n"
			"fmax.s f30, f18, f22\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 640(x22)\n"
			"fnot.pi f22, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 160(x22)\n"
			"flw.ps f18, 448(x22)\n"
			"fsub.s f27, f16, f18, rdn\n" 
			"li x5, 0xf32d711fdc6670d6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xed7ff86ef32894ca\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"maskand m1, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 736(x11)\n"
			"flw.ps f18, 608(x11)\n"
			"flw.ps f23, 832(x11)\n"
			"fmsub.ps f26, f1, f18, f23, rmm\n" 
			"li x5, 0x88c3be6a299d5aee\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x865f9e3ba340d484\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 608(x28)\n"
			"flw.ps f19, 544(x28)\n"
			"fsgnjn.ps f2, f4, f19\n" 
			"li x5, 0x2593\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 864(x13)\n"
			"flw.ps f25, 736(x13)\n"
			"fsgnj.ps f30, f2, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f22, 576(x19)\n"
			"flw.ps f9, 416(x19)\n"
			"fle.pi f23, f22, f9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f4, 160(x27)\n"
			"flw.ps f10, 320(x27)\n"
			"fxor.pi f11, f4, f10\n" 
			"mova.x.m x9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 864(x16)\n"
			"flw.ps f25, 288(x16)\n"
			"fxor.pi f26, f8, f25\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_0:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 32(x13)\n"
			"frcp.ps f6, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 544(x12)\n"
			"flw.ps f25, 448(x12)\n"
			"fmul.s f15, f16, f25, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 928(x15)\n"
			"flw.ps f11, 128(x15)\n"
			"fmin.s f16, f8, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 864(x17)\n"
			"fcvt.f16.ps f3, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 448(x12)\n"
			"flw.ps f23, 736(x12)\n"
			"fmax.s f3, f11, f23\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f21, 416(x11)\n"
			"fcvt.ps.f16 f25, f21\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_0:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 64(x19)\n"
			"fexp.ps f14, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 384(x10)\n"
			"flw.ps f14, 192(x10)\n"
			"fsub.s f9, f16, f14, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f1, 992(x5)\n"
			"fslli.pi f14, f1, 0x2\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 864(x22)\n"
			"flw.ps f24, 128(x22)\n"
			"fadd.pi f10, f29, f24\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x5 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 800(x13)\n"
			"flw.ps f20, 800(x13)\n"
			"fmax.ps f19, f0, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 416(x10)\n"
			"flw.ps f16, 64(x10)\n"
			"fsgnj.ps f26, f26, f16\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 800(x20)\n"
			"flw.ps f0, 384(x20)\n"
			"feq.pi f23, f29, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 640(x14)\n"
			"flw.ps f30, 288(x14)\n"
			"fle.ps f28, f29, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_0:\n"
			"flem.ps m3, f25, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f15, 192(x9)\n"
			"flw.ps f29, 992(x9)\n"
			"fle.pi f2, f15, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 32(x12)\n"
			"fcvt.w.s x9, f23, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 896(x12)\n"
			"flw.ps f10, 256(x12)\n"
			"fminu.pi f29, f29, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 384(x6)\n"
			"flw.ps f5, 832(x6)\n"
			"fand.pi f12, f1, f5\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f30, 576(x11)\n"
			"fcvt.ps.f16 f26, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 64(x11)\n"
			"flw.ps f14, 960(x11)\n"
			"flw.ps f29, 224(x11)\n"
			"fcmov.ps f2, f13, f14, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 352(x10)\n"
			"fsat8.pi f3, f13\n" 
			"li x5, 0x91ec\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f18, 480(x17)\n"
			"flw.ps f28, 544(x17)\n"
			"feq.pi f7, f18, f28\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 704(x7)\n"
			"fcvt.ps.pwu f26, f20, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f11, 960(x13)\n"
			"flw.ps f12, 576(x13)\n"
			"fminu.pi f22, f11, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"maskand m6, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x3 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x1 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1a << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x4 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x9 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1c << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x0 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x4 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x18 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x9 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_5_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"maskxor m6, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f12, 384(x25)\n"
			"fsatu8.pi f26, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"feqm.ps m7, f4, f13\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 32(x26)\n"
			"flw.ps f5, 384(x26)\n"
			"fcmovm.ps f11, f7, f5\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 704(x12)\n"
			"flw.ps f12, 928(x12)\n"
			"fsgnj.s f18, f13, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"masknot m5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 512(x17)\n"
			"fcvt.wu.s x19, f28, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 256(x25)\n"
			"flw.ps f19, 672(x25)\n"
			"fsgnj.ps f6, f27, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 864(x13)\n"
			"ffrc.ps f22, f0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 384(x22)\n"
			"fcvt.pw.ps f13, f6, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 704(x10)\n"
			"fcvt.w.s x18, f22, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 416(x12)\n"
			"fcvt.pw.ps f18, f16, rtz\n" 
			"fbci.ps f1, 0x64ee6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 576(x20)\n"
			"flw.ps f1, 224(x20)\n"
			"fmin.ps f6, f11, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 512(x14)\n"
			"fsgnjx.ps f6, f8, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 512(x19)\n"
			"fcvt.f16.ps f3, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 672(x10)\n"
			"fcvt.pwu.ps f28, f5, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 64(x17)\n"
			"flw.ps f25, 64(x17)\n"
			"feq.pi f17, f30, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 512(x25)\n"
			"fclass.s x18, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_1:\n"
			"maskxor m0, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f29, 608(x23)\n"
			"fcvt.ps.pw f8, f29, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 352(x17)\n"
			"fcvt.w.s x18, f0, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 960(x16)\n"
			"flw.ps f2, 992(x16)\n"
			"fmin.s f9, f6, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 448(x10)\n"
			"fclass.s x19, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 384(x27)\n"
			"flw.ps f25, 864(x27)\n"
			"fltu.pi f0, f8, f25\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 32(x23)\n"
			"fround.ps f20, f11, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 672(x27)\n"
			"fexp.ps f27, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 320(x9)\n"
			"flw.ps f7, 640(x9)\n"
			"flw.ps f3, 192(x9)\n"
			"fnmsub.ps f20, f12, f7, f3, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 672(x11)\n"
			"flw.ps f24, 640(x11)\n"
			"fle.pi f3, f0, f24\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f16, 736(x6)\n"
			"fcvt.ps.f16 f18, f16\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 640(x20)\n"
			"flw.ps f18, 640(x20)\n"
			"feq.ps f24, f28, f18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 832(x14)\n"
			"fsrai.pi f24, f24, 0xc\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"feqm.ps m7, f8, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f0, 736(x23)\n"
			"faddi.pi f21, f0, 0x27\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 160(x14)\n"
			"flw.ps f28, 256(x14)\n"
			"flw.ps f4, 800(x14)\n"
			"fnmadd.ps f12, f23, f28, f4, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f11, 352(x14)\n"
			"flw.ps f28, 928(x14)\n"
			"feq.pi f8, f11, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 160(x20)\n"
			"flw.ps f1, 928(x20)\n"
			"flw.ps f13, 864(x20)\n"
			"fmsub.ps f16, f4, f1, f13, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 928(x19)\n"
			"flw.ps f18, 960(x19)\n"
			"fadd.s f5, f14, f18, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 768(x26)\n"
			"flw.ps f10, 960(x26)\n"
			"fadd.s f4, f22, f10, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"maskand m2, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f6, 864(x12)\n"
			"fcvt.ps.pw f1, f6, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 160(x23)\n"
			"fandi.pi f24, f27, 0x151\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 0(x23)\n"
			"flw.ps f0, 960(x23)\n"
			"fmaxu.pi f11, f22, f0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 512(x6)\n"
			"flw.ps f3, 896(x6)\n"
			"fle.pi f11, f14, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 352(x25)\n"
			"fswizz.ps f11, f18, 0x19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 896(x19)\n"
			"flw.ps f9, 384(x19)\n"
			"fmul.s f27, f10, f9, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 640(x11)\n"
			"flw.ps f2, 736(x11)\n"
			"fle.pi f2, f15, f2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 704(x9)\n"
			"fnot.pi f17, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_SHORT_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_SHORT_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"li x16, 0xdf81f734c8c62761\n"
			"fcvt.s.wu f23, x16, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 480(x17)\n"
			"faddi.pi f1, f30, 0x105\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 288(x28)\n"
			"ffrc.ps f23, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 0(x28)\n"
			"flw.ps f16, 640(x28)\n"
			"flw.ps f22, 512(x28)\n"
			"fnmsub.ps f4, f23, f16, f22, rdn\n" 
			"li x5, 0x5a5718eed0fafb08\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x705aa34ca9ec2b6a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 128(x23)\n"
			"flw.ps f3, 160(x23)\n"
			"fle.s x16, f7, f3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 800(x12)\n"
			"flw.ps f2, 480(x12)\n"
			"fle.pi f27, f4, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 96(x20)\n"
			"ffrc.ps f9, f8\n" 
			"li x5, 0x62e367bb71b6caa3\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5cf27ae567556ce9\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 544(x7)\n"
			"fcvt.w.s x13, f3, rtz\n" 
			"maskpopc x13, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 480(x9)\n"
			"frsq.ps f8, f6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 32(x6)\n"
			"fsrli.pi f6, f30, 0xc\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 832(x17)\n"
			"flw.ps f3, 96(x17)\n"
			"flt.s x25, f30, f3\n" 
			"la x16, rand_int_32\n"
			"flw.ps f13, 832(x16)\n"
			"flw.ps f21, 608(x16)\n"
			"fsll.pi f21, f13, f21\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 928(x27)\n"
			"flw.ps f23, 800(x27)\n"
			"for.pi f9, f28, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 960(x23)\n"
			"flw.ps f16, 256(x23)\n"
			"fxor.pi f10, f20, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f15, 416(x25)\n"
			"faddi.pi f21, f15, 0x20\n" 
			"li x5, 0x6e6aaf1a1d7ce381\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x76dcba342190befc\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_135762_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x1\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x64dd988517064029\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x9eacdb7bb7f38556\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_2:\n"
			"feqm.ps m0, f11, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f17, 448(x25)\n"
			"fsat8.pi f20, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 192(x9)\n"
			"flw.ps f5, 352(x9)\n"
			"flt.s x13, f20, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 128(x5)\n"
			"flw.ps f16, 736(x5)\n"
			"fmul.s f9, f27, f16, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 736(x12)\n"
			"fcvt.pw.ps f14, f18, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 256(x6)\n"
			"flw.ps f7, 128(x6)\n"
			"fcmovm.ps f30, f11, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"flem.ps m5, f23, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"masknot m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"fltm.ps m5, f10, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 384(x23)\n"
			"fclass.ps f0, f16\n" 
			"maskpopc x19, m6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"flem.ps m0, f4, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f18, 192(x26)\n"
			"flw.ps f14, 288(x26)\n"
			"fsrl.pi f23, f18, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 896(x19)\n"
			"flw.ps f29, 320(x19)\n"
			"fmulh.pi f12, f25, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 512(x23)\n"
			"fswizz.ps f25, f28, 0xc\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 32(x10)\n"
			"flw.ps f26, 224(x10)\n"
			"fadd.s f30, f25, f26, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x84 << TENSOR_FMA32_SCP_LOC_B) | (0xb3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f30, 448(x19)\n"
			"flw.ps f16, 800(x19)\n"
			"fand.pi f0, f30, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 64(x12)\n"
			"flw.ps f17, 192(x12)\n"
			"fsgnjn.ps f19, f7, f17\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_2:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 64(x6)\n"
			"fcvt.f16.ps f22, f23\n" 
			"maskpopc x27, m5\n" 
			"li x20, 0xb9b47614839b5943\n"
			"fcvt.s.wu f0, x20, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 480(x17)\n"
			"fpackreph.pi f10, f18\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 512(x6)\n"
			"faddi.pi f25, f24, 0x5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 128(x6)\n"
			"fsat8.pi f25, f4\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 64(x23)\n"
			"flw.ps f21, 32(x23)\n"
			"fsgnjn.s f11, f23, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 256(x11)\n"
			"flw.ps f15, 736(x11)\n"
			"flw.ps f10, 992(x11)\n"
			"fmsub.s f11, f19, f15, f10, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 0(x6)\n"
			"flw.ps f3, 32(x6)\n"
			"fremu.pi f23, f21, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f26, 896(x25)\n"
			"fcvt.ps.pw f18, f26, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 256(x7)\n"
			"fround.ps f3, f6, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 480(x22)\n"
			"flw.ps f15, 992(x22)\n"
			"fmin.s f10, f23, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 704(x27)\n"
			"fpackrepb.pi f26, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_2:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 512(x11)\n"
			"flw.ps f24, 576(x11)\n"
			"fcmovm.ps f11, f8, f24\n" 
			"maskpopcz x5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x20, rand_int_32\n"
			"flw.ps f16, 896(x20)\n"
			"fsat8.pi f22, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 96(x17)\n"
			"flw.ps f16, 64(x17)\n"
			"fsgnjn.ps f16, f23, f16\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 416(x13)\n"
			"flw.ps f5, 288(x13)\n"
			"fle.s x16, f10, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 320(x7)\n"
			"flw.ps f13, 480(x7)\n"
			"fadd.ps f9, f3, f13, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"masknot m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 448(x23)\n"
			"flw.ps f11, 864(x23)\n"
			"fdiv.s f4, f17, f11, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"fltm.ps m1, f0, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 544(x10)\n"
			"flw.ps f23, 160(x10)\n"
			"fnmadd.s f23, f7, f23, f23, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 704(x23)\n"
			"fandi.pi f28, f17, 0x1fc\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 128(x11)\n"
			"flw.ps f21, 704(x11)\n"
			"fmulhu.pi f30, f5, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 576(x18)\n"
			"flw.ps f21, 864(x18)\n"
			"fmax.pi f31, f15, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f16, 672(x17)\n"
			"fslli.pi f22, f16, 0x0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 448(x22)\n"
			"flw.ps f15, 672(x22)\n"
			"fcmovm.ps f13, f7, f15\n" 
			"fbci.pi f20, 0x5b3a\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 736(x13)\n"
			"flw.ps f0, 704(x13)\n"
			"fmul.s f19, f15, f0, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 864(x5)\n"
			"fsrai.pi f17, f29, 0xc\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 96(x10)\n"
			"flw.ps f7, 640(x10)\n"
			"for.pi f23, f29, f7\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 384(x26)\n"
			"flw.ps f7, 192(x26)\n"
			"fmaxu.pi f17, f20, f7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 896(x15)\n"
			"flw.ps f29, 832(x15)\n"
			"fadd.pi f4, f9, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"fltm.ps m5, f28, f30\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf907f4a9db\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 544(x13)\n"
			"flw.ps f9, 352(x13)\n"
			"fadd.s f29, f7, f9, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"maskor m0, m3, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2df952117d\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f11, 544(x14)\n"
			"flw.ps f24, 160(x14)\n"
			"fmul.pi f28, f11, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 928(x5)\n"
			"flw.ps f1, 800(x5)\n"
			"fmul.pi f28, f11, f1\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 0(x12)\n"
			"flw.ps f28, 128(x12)\n"
			"fsub.pi f15, f8, f28\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 0(x28)\n"
			"fpackrepb.pi f10, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 192(x22)\n"
			"flw.ps f23, 32(x22)\n"
			"fmul.ps f11, f9, f23, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 736(x14)\n"
			"flw.ps f22, 480(x14)\n"
			"fle.ps f23, f0, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 736(x16)\n"
			"flw.ps f2, 320(x16)\n"
			"fdivu.pi f12, f26, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f5, 448(x20)\n"
			"fpackreph.pi f10, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"fbci.ps f19, 0x7e6e6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 672(x11)\n"
			"flw.ps f11, 416(x11)\n"
			"fmulhu.pi f11, f28, f11\n" 
			"maskpopc x11, m4\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 960(x20)\n"
			"flw.ps f12, 640(x20)\n"
			"flw.ps f30, 864(x20)\n"
			"fnmsub.s f18, f26, f12, f30, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 448(x10)\n"
			"flw.ps f24, 320(x10)\n"
			"fsgnjx.s f11, f2, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 928(x15)\n"
			"fswizz.ps f1, f2, 0x1c\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 928(x15)\n"
			"frsq.ps f7, f21\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 544(x26)\n"
			"fsrai.pi f8, f28, 0x1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_3:\n"
			"maskand m7, m5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f5, 736(x26)\n"
			"flw.ps f26, 32(x26)\n"
			"fminu.pi f16, f5, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 320(x26)\n"
			"flw.ps f9, 192(x26)\n"
			"fmul.ps f29, f19, f9, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 192(x27)\n"
			"fadd.ps f31, f30, f30, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_3:\n"
			"fltm.pi m7, f1, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_3:\n"
			"mov.m.x m2, x7, 0x28\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe8c8cfad54\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 896(x25)\n"
			"flw.ps f16, 224(x25)\n"
			"fmin.ps f2, f15, f16\n" 
			"mova.x.m x22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"li x6, 0xb44e7bfa6ba64b4d\n"
			"fcvt.s.wu f18, x6, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 416(x6)\n"
			"fswizz.ps f11, f15, 0x13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 864(x9)\n"
			"fnot.pi f7, f29\n" 
			"li x5, 0x81a9\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f15, 768(x9)\n"
			"flw.ps f4, 160(x9)\n"
			"fminu.pi f13, f15, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 224(x18)\n"
			"fcvt.f16.ps f9, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 640(x25)\n"
			"fclass.ps f8, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 288(x7)\n"
			"flw.ps f7, 672(x7)\n"
			"fmulhu.pi f27, f25, f7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 320(x23)\n"
			"flw.ps f7, 704(x23)\n"
			"flw.ps f13, 960(x23)\n"
			"fmadd.ps f28, f19, f7, f13, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 0(x13)\n"
			"flw.ps f18, 160(x13)\n"
			"feq.pi f0, f15, f18\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 128(x10)\n"
			"flw.ps f11, 192(x10)\n"
			"fmax.ps f17, f12, f11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f17, 128(x18)\n"
			"flw.ps f24, 800(x18)\n"
			"fltu.pi f10, f17, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 896(x12)\n"
			"flw.ps f14, 864(x12)\n"
			"fmin.s f3, f13, f14\n" 
			"fbci.ps f16, 0x143ed\n" 
			"la x18, rand_int_32\n"
			"flw.ps f3, 736(x18)\n"
			"fandi.pi f25, f3, 0xe5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 704(x20)\n"
			"flw.ps f10, 704(x20)\n"
			"fmul.pi f20, f16, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 256(x7)\n"
			"flw.ps f23, 704(x7)\n"
			"fmsub.s f27, f30, f30, f23, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x6 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x2 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1c << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x8 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 544(x9)\n"
			"flw.ps f4, 128(x9)\n"
			"flw.ps f17, 768(x9)\n"
			"fnmsub.s f17, f15, f4, f17, rtz\n" 
			"fbci.pi f24, 0x426\n" 
			"la x11, rand_int_32\n"
			"flw.ps f29, 416(x11)\n"
			"fcvt.ps.pw f14, f29, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_4:\n"
			"maskand m3, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 192(x19)\n"
			"flw.ps f1, 992(x19)\n"
			"flw.ps f8, 128(x19)\n"
			"fnmsub.ps f12, f26, f1, f8, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f19, 416(x27)\n"
			"flw.ps f26, 544(x27)\n"
			"fmax.pi f18, f19, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 704(x10)\n"
			"flw.ps f18, 608(x10)\n"
			"fmulhu.pi f23, f1, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 288(x17)\n"
			"flw.ps f0, 640(x17)\n"
			"fsll.pi f12, f3, f0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 832(x27)\n"
			"flw.ps f1, 192(x27)\n"
			"feq.s x27, f31, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 320(x14)\n"
			"flw.ps f24, 512(x14)\n"
			"fadd.pi f25, f25, f24\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f17, 928(x11)\n"
			"fcvt.ps.f16 f11, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 800(x9)\n"
			"flw.ps f1, 864(x9)\n"
			"flw.ps f8, 160(x9)\n"
			"fmsub.ps f14, f25, f1, f8, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 320(x5)\n"
			"flw.ps f1, 128(x5)\n"
			"fle.s x6, f15, f1\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 480(x5)\n"
			"flw.ps f29, 320(x5)\n"
			"fxor.pi f20, f26, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 256(x25)\n"
			"fcvt.f16.ps f22, f26\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 0(x13)\n"
			"flw.ps f28, 864(x13)\n"
			"fmax.pi f12, f15, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_800622_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x1 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x1f44\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 160(x14)\n"
			"fcvt.w.s x5, f17, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 480(x16)\n"
			"flw.ps f18, 992(x16)\n"
			"fmaxu.pi f26, f0, f18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 928(x14)\n"
			"fnot.pi f18, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f0, 544(x15)\n"
			"flw.ps f9, 416(x15)\n"
			"fsll.pi f2, f0, f9\n" 
			"LBL_SEQID_2_FP_TRANS_ID_1_HID_4:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 864(x16)\n"
			"fexp.ps f18, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 128(x26)\n"
			"fcvt.f16.ps f18, f20\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 448(x20)\n"
			"flw.ps f20, 864(x20)\n"
			"fmul.pi f1, f21, f20\n" 
			"li x5, 0xa3dd\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 704(x13)\n"
			"fcvt.pwu.ps f3, f8, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 672(x23)\n"
			"fpackreph.pi f12, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_4:\n"
			"maskor m5, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 64(x5)\n"
			"flw.ps f0, 576(x5)\n"
			"fsgnjx.ps f21, f11, f0\n" 
			"maskpopc x17, m6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 544(x25)\n"
			"flw.ps f25, 448(x25)\n"
			"fnmsub.s f10, f27, f27, f25, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 96(x17)\n"
			"feq.s x18, f17, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 736(x5)\n"
			"flw.ps f22, 0(x5)\n"
			"fsub.ps f21, f5, f22, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 352(x10)\n"
			"flw.ps f27, 480(x10)\n"
			"fle.s x7, f2, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_61502_HID_4:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"flem.ps m2, f11, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 320(x11)\n"
			"flw.ps f22, 672(x11)\n"
			"fsgnj.s f2, f20, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 960(x13)\n"
			"flw.ps f23, 672(x13)\n"
			"flw.ps f25, 640(x13)\n"
			"fmadd.ps f0, f30, f23, f25, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 160(x28)\n"
			"ffrc.ps f14, f30\n" 
			"li x23, 0x11d737ef64e69af4\n"
			"fcvt.s.w f12, x23, rup\n" 
			"li x26, 0xfdeda2bf8c213d58\n"
			"fcvt.s.w f20, x26, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 992(x26)\n"
			"flw.ps f9, 992(x26)\n"
			"fsub.pi f11, f20, f9\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 480(x25)\n"
			"fcvt.pw.ps f26, f20, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 512(x5)\n"
			"flw.ps f26, 768(x5)\n"
			"flt.s x20, f22, f26\n" 
			"la x25, rand_int_32\n"
			"flw.ps f23, 672(x25)\n"
			"flw.ps f12, 96(x25)\n"
			"fminu.pi f7, f23, f12\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 384(x26)\n"
			"fandi.pi f14, f26, 0x102\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 384(x19)\n"
			"flw.ps f7, 768(x19)\n"
			"fsgnjx.ps f3, f4, f7\n" 
			"fbci.pi f18, 0x33728\n" 
			"maskpopcz x11, m2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 128(x11)\n"
			"fsin.ps f17, f3\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 640(x16)\n"
			"flw.ps f14, 704(x16)\n"
			"fmin.ps f12, f8, f14\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"maskpopc x6, m1\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_5:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 768(x28)\n"
			"flog.ps f7, f27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f14, 832(x16)\n"
			"flw.ps f18, 864(x16)\n"
			"fsub.pi f15, f14, f18\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 160(x27)\n"
			"fnot.pi f14, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 544(x23)\n"
			"fcvt.f16.ps f25, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 0(x22)\n"
			"flw.ps f24, 512(x22)\n"
			"fsub.ps f28, f30, f24, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 160(x28)\n"
			"flw.ps f24, 256(x28)\n"
			"flw.ps f7, 192(x28)\n"
			"fmsub.s f23, f9, f24, f7, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 736(x6)\n"
			"fpackrepb.pi f28, f7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 928(x28)\n"
			"fsrli.pi f7, f6, 0x7\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 416(x12)\n"
			"flw.ps f17, 352(x12)\n"
			"feq.pi f21, f31, f17\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 64(x16)\n"
			"fswizz.ps f15, f24, 0x9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 960(x18)\n"
			"flw.ps f13, 896(x18)\n"
			"feq.ps f1, f3, f13\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 800(x27)\n"
			"fnot.pi f0, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 608(x9)\n"
			"flw.ps f18, 160(x9)\n"
			"feq.pi f5, f2, f18\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 288(x28)\n"
			"fsat8.pi f21, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 608(x23)\n"
			"flw.ps f3, 992(x23)\n"
			"flt.s x23, f15, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 256(x14)\n"
			"flw.ps f10, 960(x14)\n"
			"fmsub.ps f9, f9, f9, f10, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 640(x14)\n"
			"flw.ps f30, 704(x14)\n"
			"fmulh.pi f26, f21, f30\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 256(x15)\n"
			"flw.ps f7, 640(x15)\n"
			"fand.pi f28, f17, f7\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 928(x13)\n"
			"flw.ps f20, 576(x13)\n"
			"for.pi f2, f18, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_5:\n"
			"maskxor m7, m6, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 992(x5)\n"
			"flw.ps f5, 896(x5)\n"
			"fadd.ps f19, f10, f5, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 224(x20)\n"
			"flw.ps f17, 704(x20)\n"
			"fmul.s f19, f9, f17, rne\n" 
			"fbci.ps f31, 0x2518f\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 64(x10)\n"
			"fslli.pi f2, f4, 0xd\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_5:\n"
			"fltm.pi m5, f10, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 864(x13)\n"
			"flw.ps f0, 128(x13)\n"
			"flw.ps f26, 96(x13)\n"
			"fmadd.ps f1, f23, f0, f26, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 960(x20)\n"
			"fcvt.ps.pwu f16, f21, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 320(x26)\n"
			"fsgnjn.s f19, f8, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 864(x6)\n"
			"fsll.pi f19, f28, f28\n" 
			"maskpopcz x23, m7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 960(x15)\n"
			"fandi.pi f26, f17, 0xc3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 672(x9)\n"
			"flw.ps f14, 736(x9)\n"
			"flw.ps f11, 224(x9)\n"
			"fnmadd.s f28, f17, f14, f11, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 128(x11)\n"
			"flw.ps f12, 416(x11)\n"
			"fadd.s f24, f31, f12, rtz\n" 
			"li x5, 0xe1c471b84fe8d6cc\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4866758693419a0d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"fltm.ps m4, f21, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f10, 512(x23)\n"
			"flw.ps f30, 544(x23)\n"
			"fmulh.pi f20, f10, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 96(x22)\n"
			"fcvt.f16.ps f15, f16\n" 
			"la x15, rand_int_32\n"
			"flw.ps f0, 608(x15)\n"
			"flw.ps f12, 352(x15)\n"
			"fsll.pi f0, f0, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 480(x16)\n"
			"flw.ps f15, 960(x16)\n"
			"feq.s x19, f11, f15\n" 
			"la x25, rand_int_32\n"
			"flw.ps f11, 128(x25)\n"
			"faddi.pi f26, f11, 0x62\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 160(x14)\n"
			"flw.ps f30, 64(x14)\n"
			"feq.pi f11, f31, f30\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_6:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 576(x17)\n"
			"fexp.ps f30, f24\n" 
			"li x5, 0x3fd733bd5ccd174b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd06e9c1ca19f9b76\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 288(x26)\n"
			"flw.ps f0, 896(x26)\n"
			"fsgnjn.s f14, f29, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 608(x28)\n"
			"flw.ps f10, 224(x28)\n"
			"fmin.s f23, f6, f10\n" 
			"maskpopc x6, m3\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f2, 544(x17)\n"
			"fcvt.ps.f16 f2, f2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 800(x22)\n"
			"fround.ps f20, f17, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"masknot m3, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6876f75f77\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 384(x19)\n"
			"flw.ps f29, 192(x19)\n"
			"fmin.ps f25, f31, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 864(x7)\n"
			"flw.ps f31, 352(x7)\n"
			"fsgnj.s f9, f18, f31\n" 
			"maskpopcz x27, m1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 608(x19)\n"
			"flw.ps f5, 736(x19)\n"
			"fadd.pi f20, f7, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 160(x9)\n"
			"fpackreph.pi f8, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 608(x6)\n"
			"fpackrepb.pi f19, f1\n" 
			"li x18, 0xb566c15d2f4cfd24\n"
			"fcvt.s.l f9, x18, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 384(x20)\n"
			"flw.ps f8, 160(x20)\n"
			"fcmovm.ps f21, f19, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 256(x22)\n"
			"flw.ps f4, 0(x22)\n"
			"fadd.s f6, f20, f4, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 768(x27)\n"
			"flw.ps f4, 128(x27)\n"
			"flw.ps f18, 928(x27)\n"
			"fmadd.s f8, f25, f4, f18, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 864(x16)\n"
			"fandi.pi f2, f10, 0x60\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 800(x11)\n"
			"fcvt.pw.ps f11, f1, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 992(x12)\n"
			"flw.ps f13, 32(x12)\n"
			"fmin.ps f23, f30, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"fltm.ps m3, f26, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 960(x5)\n"
			"flw.ps f20, 256(x5)\n"
			"flw.ps f6, 64(x5)\n"
			"fnmsub.ps f24, f2, f20, f6, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 608(x12)\n"
			"fnot.pi f9, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xc6 << TENSOR_IMA_SCP_LOC_B) | (0x3d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 128(x13)\n"
			"fcvt.wu.s x22, f8, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 864(x17)\n"
			"fnot.pi f10, f1\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 864(x7)\n"
			"flw.ps f6, 192(x7)\n"
			"feq.pi f2, f20, f6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 160(x13)\n"
			"flw.ps f11, 96(x13)\n"
			"fdiv.pi f26, f31, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 864(x23)\n"
			"flw.ps f23, 320(x23)\n"
			"fsrl.pi f27, f10, f23\n" 
			"li x6, 0xab51e18a883f3df1\n"
			"fcvt.s.w f23, x6, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 256(x22)\n"
			"fle.ps f30, f23, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 832(x22)\n"
			"flw.ps f23, 672(x22)\n"
			"fmsub.s f15, f12, f12, f23, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 704(x28)\n"
			"flw.ps f6, 608(x28)\n"
			"fsub.pi f28, f4, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f19, 224(x20)\n"
			"flw.ps f14, 768(x20)\n"
			"fminu.pi f6, f19, f14\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 576(x10)\n"
			"flw.ps f2, 64(x10)\n"
			"fle.pi f21, f4, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 416(x14)\n"
			"flw.ps f27, 32(x14)\n"
			"flw.ps f9, 32(x14)\n"
			"fcmov.ps f20, f29, f27, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 672(x5)\n"
			"flw.ps f20, 992(x5)\n"
			"fsub.s f27, f25, f20, rmm\n" 
			"li x5, 0xe606537bc88c5cc5\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7cfeb0f87f1b37ab\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 960(x20)\n"
			"fround.ps f13, f21, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 480(x9)\n"
			"flw.ps f30, 480(x9)\n"
			"fltu.pi f30, f4, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 480(x9)\n"
			"flw.ps f9, 96(x9)\n"
			"fmulhu.pi f28, f14, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x6960\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x5 << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x23, rand_int_32\n"
			"flw.ps f0, 832(x23)\n"
			"fsrai.pi f10, f0, 0xb\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 832(x28)\n"
			"fsin.ps f22, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"fltm.pi m7, f12, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f15, 832(x22)\n"
			"flw.ps f7, 960(x22)\n"
			"fmulh.pi f15, f15, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 992(x14)\n"
			"fcvt.ps.pw f18, f28, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 288(x22)\n"
			"flw.ps f31, 64(x22)\n"
			"fsgnjn.ps f17, f27, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 992(x6)\n"
			"fcvt.pw.ps f8, f31, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 64(x18)\n"
			"flw.ps f19, 288(x18)\n"
			"fsll.pi f26, f7, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"feqm.ps m5, f15, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f11, 416(x7)\n"
			"flw.ps f26, 320(x7)\n"
			"for.pi f24, f11, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 864(x22)\n"
			"flw.ps f10, 448(x22)\n"
			"fmulhu.pi f19, f8, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 384(x26)\n"
			"flw.ps f11, 608(x26)\n"
			"feq.s x15, f30, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 256(x5)\n"
			"flw.ps f4, 864(x5)\n"
			"flw.ps f2, 288(x5)\n"
			"fmadd.ps f17, f31, f4, f2, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 32(x17)\n"
			"flw.ps f13, 992(x17)\n"
			"fsub.s f3, f24, f13, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 736(x20)\n"
			"flw.ps f6, 896(x20)\n"
			"fmax.pi f13, f10, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 640(x11)\n"
			"fandi.pi f0, f8, 0x8f\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"fbci.pi f0, 0x4349b\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 704(x10)\n"
			"flw.ps f7, 256(x10)\n"
			"fmul.ps f5, f30, f7, rtz\n" 
			"maskpopcz x28, m0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_7:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 448(x5)\n"
			"frcp.ps f18, f31\n" 
			"fbci.ps f15, 0x30d0e\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 320(x7)\n"
			"flw.ps f27, 832(x7)\n"
			"fsra.pi f11, f7, f27\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 320(x22)\n"
			"flw.ps f14, 960(x22)\n"
			"fminu.pi f23, f3, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 800(x28)\n"
			"fmin.ps f25, f5, f5\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_7:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 512(x11)\n"
			"frcp.ps f16, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 416(x12)\n"
			"flw.ps f18, 736(x12)\n"
			"flt.ps f1, f11, f18\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 224(x12)\n"
			"flw.ps f12, 704(x12)\n"
			"fsll.pi f18, f0, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 928(x26)\n"
			"flw.ps f26, 928(x26)\n"
			"fadd.ps f24, f20, f26, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 192(x6)\n"
			"flw.ps f18, 896(x6)\n"
			"fadd.s f16, f3, f18, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_7:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 960(x6)\n"
			"fexp.ps f17, f15\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 576(x17)\n"
			"flw.ps f17, 192(x17)\n"
			"fmulh.pi f10, f30, f17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 544(x27)\n"
			"fcvt.wu.s x10, f28, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 864(x16)\n"
			"flw.ps f16, 256(x16)\n"
			"fmax.s f17, f23, f16\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 96(x11)\n"
			"flw.ps f20, 992(x11)\n"
			"fadd.ps f7, f15, f20, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f6, 128(x22)\n"
			"fslli.pi f27, f6, 0xf\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 32(x6)\n"
			"fclass.ps f31, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 128(x14)\n"
			"flw.ps f26, 864(x14)\n"
			"fmsub.ps f3, f29, f26, f26, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 0(x11)\n"
			"fsrli.pi f22, f31, 0x4\n" 
			"li x9, 0xd76dd48499965b63\n"
			"fcvt.s.w f2, x9, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 512(x20)\n"
			"flw.ps f12, 640(x20)\n"
			"fsgnjx.s f8, f15, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 736(x25)\n"
			"flw.ps f25, 832(x25)\n"
			"fmin.s f31, f22, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 448(x14)\n"
			"flw.ps f6, 256(x14)\n"
			"flt.pi f10, f20, f6\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 928(x9)\n"
			"fsat8.pi f11, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 576(x20)\n"
			"fpackrepb.pi f7, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 384(x9)\n"
			"fcvt.ps.pwu f17, f5, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 992(x11)\n"
			"flw.ps f27, 96(x11)\n"
			"fmul.pi f22, f28, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_7:\n"
			"mov.m.x m4, x18, 0x67\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"feqm.ps m3, f24, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 960(x7)\n"
			"flw.ps f23, 928(x7)\n"
			"flt.s x19, f9, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"flem.ps m2, f7, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_8:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 480(x18)\n"
			"fexp.ps f25, f29\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 608(x19)\n"
			"flw.ps f25, 832(x19)\n"
			"fsgnjn.ps f4, f11, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 480(x19)\n"
			"flw.ps f22, 288(x19)\n"
			"fmul.ps f5, f8, f22, rdn\n" 
			"li x5, 0xccddb0f9ca4aa2bd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x801aa089abe2c21c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 512(x12)\n"
			"flw.ps f29, 416(x12)\n"
			"fsgnjx.ps f0, f14, f29\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 608(x6)\n"
			"fcvt.w.s x12, f12, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 96(x5)\n"
			"fsin.ps f0, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 320(x12)\n"
			"fnot.pi f18, f1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 480(x12)\n"
			"flw.ps f10, 384(x12)\n"
			"flw.ps f4, 896(x12)\n"
			"fcmov.ps f12, f17, f10, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 896(x12)\n"
			"fandi.pi f29, f7, 0x0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_8:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 192(x9)\n"
			"frcp.ps f25, f16\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_8:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 224(x20)\n"
			"fexp.ps f13, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 896(x9)\n"
			"fcvt.w.s x28, f25, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f27, 608(x17)\n"
			"fpackrepb.pi f22, f27\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 544(x6)\n"
			"fsrai.pi f14, f1, 0xb\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
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
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"maskor m0, m3, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f23, 768(x7)\n"
			"flw.ps f22, 928(x7)\n"
			"fsub.pi f1, f23, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 224(x12)\n"
			"fcvt.pw.ps f7, f5, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x22, m5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_8:\n"
			"fltm.ps m5, f24, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 896(x26)\n"
			"flw.ps f25, 384(x26)\n"
			"fsgnjx.ps f29, f12, f25\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_8:\n"
			"flem.ps m7, f1, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 96(x26)\n"
			"ffrc.ps f30, f7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f3, 896(x28)\n"
			"fcvt.ps.pw f27, f3, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 672(x22)\n"
			"flw.ps f28, 640(x22)\n"
			"fsgnj.ps f31, f31, f28\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 96(x26)\n"
			"flw.ps f1, 224(x26)\n"
			"fand.pi f20, f12, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 352(x26)\n"
			"fclass.ps f1, f28\n" 
			"li x5, 0xb0b7976aa8aaa336\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7e36dd795180733\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 256(x12)\n"
			"flw.ps f9, 544(x12)\n"
			"fmul.ps f5, f3, f9, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 704(x19)\n"
			"flw.ps f17, 192(x19)\n"
			"flw.ps f8, 992(x19)\n"
			"fnmsub.s f13, f29, f17, f8, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 384(x28)\n"
			"fsin.ps f15, f28\n" 
			"LBL_SEQID_1_FP_TRANS_ID_7_HID_8:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 192(x11)\n"
			"frcp.ps f2, f5\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x2 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x9 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f12, 480(x18)\n"
			"flw.ps f29, 96(x18)\n"
			"fle.pi f27, f12, f29\n" 
			"li x5, 0x47b7\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 288(x15)\n"
			"flw.ps f1, 992(x15)\n"
			"feq.s x25, f2, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 576(x27)\n"
			"fround.ps f9, f5, rne\n" 
			"li x7, 0x63fef1433727de58\n"
			"fcvt.s.wu f30, x7, rup\n" 
			"fbci.ps f9, 0x500c\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 448(x15)\n"
			"flw.ps f28, 480(x15)\n"
			"fle.s x22, f0, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 288(x5)\n"
			"flw.ps f18, 832(x5)\n"
			"fsgnjx.ps f17, f10, f18\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 448(x18)\n"
			"flw.ps f20, 864(x18)\n"
			"fadd.s f12, f1, f20, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 608(x26)\n"
			"flw.ps f19, 384(x26)\n"
			"fmax.ps f20, f18, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 800(x27)\n"
			"frsq.ps f16, f1\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 576(x25)\n"
			"fcvt.ps.pwu f1, f13, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 992(x14)\n"
			"flw.ps f29, 448(x14)\n"
			"fadd.pi f26, f31, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 992(x18)\n"
			"flw.ps f29, 384(x18)\n"
			"frem.pi f2, f2, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 160(x18)\n"
			"fpackreph.pi f5, f26\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 512(x27)\n"
			"flw.ps f9, 448(x27)\n"
			"flw.ps f16, 256(x27)\n"
			"fcmov.ps f1, f5, f9, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 96(x7)\n"
			"flt.pi f18, f24, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_296771_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x4 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x7 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f29,  96 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f26,  768 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 288(x9)\n"
			"ffrc.ps f19, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 544(x28)\n"
			"flw.ps f20, 832(x28)\n"
			"fsgnjx.ps f9, f24, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 256(x16)\n"
			"flw.ps f5, 0(x16)\n"
			"fadd.s f16, f15, f5, rne\n" 
			"maskpopcz x25, m0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 576(x10)\n"
			"fsat8.pi f19, f21\n" 
			"li x11, 0xfa063d4671474837\n"
			"fcvt.s.w f30, x11, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 96(x12)\n"
			"fsqrt.s f23, f12, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 672(x22)\n"
			"flw.ps f5, 64(x22)\n"
			"fmul.s f21, f10, f5, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 288(x9)\n"
			"flw.ps f21, 640(x9)\n"
			"flw.ps f23, 864(x9)\n"
			"fmsub.s f16, f11, f21, f23, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 768(x16)\n"
			"fsrli.pi f28, f28, 0xa\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 384(x17)\n"
			"flw.ps f4, 864(x17)\n"
			"feq.ps f12, f14, f4\n" 
			"fbci.pi f25, 0x12e12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 480(x10)\n"
			"faddi.pi f18, f26, 0x69\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 896(x19)\n"
			"flw.ps f8, 608(x19)\n"
			"fadd.s f0, f10, f8, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"mov.m.x m5, x6, 0x6c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f30, 192(x15)\n"
			"flw.ps f22, 192(x15)\n"
			"flt.pi f9, f30, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 736(x6)\n"
			"flw.ps f17, 128(x6)\n"
			"fmin.s f19, f2, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 768(x6)\n"
			"flw.ps f18, 512(x6)\n"
			"fmin.ps f2, f22, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 608(x11)\n"
			"flw.ps f16, 864(x11)\n"
			"fmulhu.pi f12, f9, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_9:\n"
			"feqm.ps m4, f6, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 448(x10)\n"
			"flw.ps f6, 640(x10)\n"
			"fmul.ps f1, f22, f6, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 544(x13)\n"
			"fclass.s x18, f30\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 576(x23)\n"
			"flw.ps f28, 576(x23)\n"
			"fminu.pi f13, f17, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 928(x28)\n"
			"flw.ps f9, 256(x28)\n"
			"flw.ps f11, 704(x28)\n"
			"fnmadd.s f13, f22, f9, f11, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"maskxor m5, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 992(x22)\n"
			"fcvt.pw.ps f4, f23, rmm\n" 
			"li x7, 0x28103e8b67fdcca3\n"
			"fcvt.s.l f20, x7, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 480(x5)\n"
			"flw.ps f28, 288(x5)\n"
			"fmax.ps f25, f7, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 64(x15)\n"
			"fround.ps f0, f16, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 384(x18)\n"
			"flw.ps f2, 640(x18)\n"
			"flw.ps f28, 960(x18)\n"
			"fnmsub.s f6, f15, f2, f28, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 384(x13)\n"
			"frsq.ps f3, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"flem.ps m4, f0, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f7, 192(x7)\n"
			"flw.ps f6, 480(x7)\n"
			"fmulhu.pi f19, f7, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 736(x25)\n"
			"fcvt.wu.s x13, f6, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_9:\n"
			"fltm.ps m1, f0, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 800(x23)\n"
			"flw.ps f23, 736(x23)\n"
			"fle.ps f31, f4, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 736(x23)\n"
			"flw.ps f14, 832(x23)\n"
			"fadd.ps f31, f7, f14, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f23, 224(x6)\n"
			"fcvt.ps.pw f23, f23, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 608(x16)\n"
			"flw.ps f18, 640(x16)\n"
			"flw.ps f17, 160(x16)\n"
			"fnmsub.s f9, f0, f18, f17, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 608(x11)\n"
			"flw.ps f23, 960(x11)\n"
			"fsll.pi f25, f25, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 0(x26)\n"
			"fsgnjn.s f5, f10, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 512(x16)\n"
			"flw.ps f29, 992(x16)\n"
			"fmax.pi f30, f1, f29\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 768(x13)\n"
			"flw.ps f18, 416(x13)\n"
			"fsub.pi f15, f15, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 640(x11)\n"
			"fcvt.pw.ps f30, f20, rmm\n" 
			"maskpopcz x16, m4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 960(x19)\n"
			"flw.ps f10, 512(x19)\n"
			"fminu.pi f27, f18, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_9:\n"
			"flem.ps m5, f25, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f28, 384(x10)\n"
			"flw.ps f31, 736(x10)\n"
			"fxor.pi f0, f28, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fltm.ps m6, f23, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 0(x18)\n"
			"fcvt.pwu.ps f30, f14, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 384(x10)\n"
			"fcvt.ps.pwu f0, f15, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 256(x11)\n"
			"faddi.pi f4, f8, 0x29\n" 
			"li x5, 0x5e78\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 96(x15)\n"
			"fswizz.ps f8, f13, 0x18\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 352(x22)\n"
			"fsin.ps f21, f2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 960(x16)\n"
			"flw.ps f6, 928(x16)\n"
			"fltu.pi f15, f20, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 512(x22)\n"
			"fsin.ps f3, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 864(x26)\n"
			"flw.ps f8, 960(x26)\n"
			"fmax.pi f29, f14, f8\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 352(x25)\n"
			"flw.ps f21, 224(x25)\n"
			"fmul.s f20, f8, f21, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 256(x14)\n"
			"flw.ps f18, 32(x14)\n"
			"fmul.s f4, f21, f18, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 224(x27)\n"
			"fslli.pi f28, f3, 0x3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 0(x26)\n"
			"flw.ps f26, 608(x26)\n"
			"fsrl.pi f3, f27, f26\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 32(x27)\n"
			"flw.ps f23, 192(x27)\n"
			"fmul.s f5, f30, f23, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 960(x23)\n"
			"flw.ps f3, 576(x23)\n"
			"for.pi f23, f5, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 0(x26)\n"
			"fclass.ps f24, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x7 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x0 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x18 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x5 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 192(x28)\n"
			"flw.ps f20, 800(x28)\n"
			"feq.s x13, f30, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 224(x22)\n"
			"flw.ps f30, 96(x22)\n"
			"flw.ps f18, 160(x22)\n"
			"fcmov.ps f14, f25, f30, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 704(x11)\n"
			"flw.ps f21, 608(x11)\n"
			"fmin.s f19, f0, f21\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 352(x16)\n"
			"flw.ps f20, 224(x16)\n"
			"fltu.pi f19, f0, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 960(x16)\n"
			"flw.ps f7, 384(x16)\n"
			"fsgnj.s f2, f12, f7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 256(x15)\n"
			"flw.ps f0, 704(x15)\n"
			"fsll.pi f13, f1, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 640(x28)\n"
			"fround.ps f18, f4, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 416(x18)\n"
			"fclass.ps f15, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 0(x7)\n"
			"fclass.s x6, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 224(x9)\n"
			"fclass.s x5, f21\n" 
			"fbci.ps f1, 0x732de\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 384(x22)\n"
			"flw.ps f29, 384(x22)\n"
			"fltu.pi f19, f28, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 608(x5)\n"
			"fround.ps f28, f7, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_10:\n"
			"flem.ps m5, f13, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 864(x23)\n"
			"flw.ps f26, 256(x23)\n"
			"flw.ps f10, 32(x23)\n"
			"fmadd.s f4, f9, f26, f10, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 736(x18)\n"
			"fclass.s x27, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_942068_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x3 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x1 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x19 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x2 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f29, 832(x7)\n"
			"flw.ps f13, 32(x7)\n"
			"fltu.pi f5, f29, f13\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 352(x13)\n"
			"flw.ps f12, 640(x13)\n"
			"fadd.pi f22, f16, f12\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 64(x27)\n"
			"flw.ps f9, 576(x27)\n"
			"flw.ps f17, 192(x27)\n"
			"fmadd.s f11, f2, f9, f17, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 416(x12)\n"
			"flw.ps f15, 640(x12)\n"
			"fle.pi f20, f20, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 192(x7)\n"
			"flw.ps f23, 512(x7)\n"
			"flw.ps f24, 544(x7)\n"
			"fnmsub.s f23, f3, f23, f24, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 384(x17)\n"
			"flw.ps f24, 544(x17)\n"
			"fmax.ps f6, f2, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 320(x18)\n"
			"flw.ps f7, 192(x18)\n"
			"flw.ps f8, 576(x18)\n"
			"fnmsub.s f10, f9, f7, f8, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 992(x9)\n"
			"flw.ps f28, 128(x9)\n"
			"flw.ps f29, 608(x9)\n"
			"fmsub.s f14, f13, f28, f29, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 256(x10)\n"
			"flw.ps f19, 32(x10)\n"
			"fmulh.pi f3, f28, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f21, 416(x16)\n"
			"flw.ps f12, 288(x16)\n"
			"fle.s x27, f21, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_10:\n"
			"fsetm.pi m5, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x1b94\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f28, 192(x17)\n"
			"flw.ps f12, 128(x17)\n"
			"fsrl.pi f1, f28, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 352(x6)\n"
			"flw.ps f0, 928(x6)\n"
			"fmax.s f3, f24, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 800(x27)\n"
			"flw.ps f22, 64(x27)\n"
			"fdiv.pi f14, f11, f22\n" 
			"li x10, 0x911e8d12fd01c46c\n"
			"fcvt.s.l f6, x10, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 288(x6)\n"
			"flw.ps f6, 672(x6)\n"
			"flw.ps f26, 800(x6)\n"
			"fmsub.s f8, f24, f6, f26, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x5 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x0 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x7 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x26, rand_int_32\n"
			"flw.ps f1, 192(x26)\n"
			"flw.ps f23, 288(x26)\n"
			"fxor.pi f3, f1, f23\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 960(x27)\n"
			"flw.ps f4, 704(x27)\n"
			"feq.pi f2, f12, f4\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_11:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 832(x15)\n"
			"fexp.ps f29, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 608(x16)\n"
			"fround.ps f0, f13, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 992(x13)\n"
			"fcvt.pw.ps f4, f6, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"flem.ps m4, f23, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 704(x9)\n"
			"flw.ps f26, 224(x9)\n"
			"fadd.s f26, f4, f26, rdn\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_11:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 448(x17)\n"
			"fexp.ps f9, f26\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 128(x28)\n"
			"fsrai.pi f8, f6, 0x9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 320(x11)\n"
			"flw.ps f23, 480(x11)\n"
			"fadd.s f23, f3, f23, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 192(x20)\n"
			"flw.ps f17, 352(x20)\n"
			"fcmovm.ps f0, f12, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 672(x20)\n"
			"flw.ps f11, 640(x20)\n"
			"feq.s x5, f20, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 128(x9)\n"
			"faddi.pi f10, f16, 0x1d0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 224(x25)\n"
			"flw.ps f23, 128(x25)\n"
			"fle.pi f3, f20, f23\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 768(x27)\n"
			"flw.ps f12, 320(x27)\n"
			"fle.ps f25, f0, f12\n" 
			"mova.x.m x22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"flem.ps m4, f8, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f2, 576(x15)\n"
			"fcvt.ps.pwu f7, f2, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 896(x26)\n"
			"ffrc.ps f22, f3\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_11:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 864(x5)\n"
			"flog.ps f2, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 160(x12)\n"
			"flw.ps f14, 672(x12)\n"
			"fle.ps f19, f31, f14\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 96(x10)\n"
			"flw.ps f27, 896(x10)\n"
			"fsra.pi f24, f19, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 832(x26)\n"
			"fsrai.pi f15, f9, 0x8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"maskand m3, m2, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x1816d0e0c0\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 352(x6)\n"
			"flw.ps f7, 832(x6)\n"
			"fmul.ps f16, f3, f7, rtz\n" 
			"maskpopc x16, m5\n" 
			"li x23, 0x12256c76f6bf8507\n"
			"fcvt.s.wu f2, x23, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f1, 576(x7)\n"
			"flw.ps f22, 704(x7)\n"
			"fand.pi f21, f1, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_11:\n"
			"maskxor m6, m7, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 480(x7)\n"
			"fclass.ps f26, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 480(x20)\n"
			"flw.ps f21, 32(x20)\n"
			"fmax.s f5, f0, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_11:\n"
			"maskxor m3, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 448(x7)\n"
			"flw.ps f5, 832(x7)\n"
			"flw.ps f1, 576(x7)\n"
			"fnmadd.s f26, f24, f5, f1, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 32(x28)\n"
			"fround.ps f9, f16, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 32(x11)\n"
			"fcvt.f16.ps f3, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 256(x6)\n"
			"flw.ps f3, 416(x6)\n"
			"fsgnjx.s f7, f25, f3\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 160(x7)\n"
			"flw.ps f26, 544(x7)\n"
			"fand.pi f3, f14, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 384(x23)\n"
			"flw.ps f5, 0(x23)\n"
			"fsgnj.s f20, f30, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 832(x9)\n"
			"fcvt.ps.pwu f31, f4, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 160(x13)\n"
			"flw.ps f15, 704(x13)\n"
			"flw.ps f2, 672(x13)\n"
			"fnmadd.s f2, f29, f15, f2, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 800(x19)\n"
			"flw.ps f30, 64(x19)\n"
			"fadd.pi f9, f7, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_11:\n"
			"fltm.ps m6, f7, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_16\n"
			"flw.ps f18, 672(x5)\n"
			"fcvt.ps.f16 f30, f18\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 128(x16)\n"
			"flw.ps f22, 768(x16)\n"
			"flt.ps f16, f8, f22\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 992(x25)\n"
			"flw.ps f10, 864(x25)\n"
			"fadd.pi f24, f27, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_11:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 608(x16)\n"
			"flw.ps f12, 928(x16)\n"
			"fmax.ps f5, f27, f12\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 800(x23)\n"
			"flw.ps f9, 544(x23)\n"
			"fand.pi f30, f0, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 832(x5)\n"
			"flw.ps f22, 448(x5)\n"
			"flw.ps f1, 544(x5)\n"
			"fnmsub.ps f26, f15, f22, f1, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 896(x25)\n"
			"flw.ps f21, 832(x25)\n"
			"flw.ps f23, 224(x25)\n"
			"fmadd.s f2, f14, f21, f23, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"maskor m4, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 224(x22)\n"
			"fclass.ps f9, f12\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 576(x11)\n"
			"flw.ps f27, 64(x11)\n"
			"fmax.s f18, f14, f27\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 896(x23)\n"
			"flog.ps f27, f15\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x22, 0xa2421d133c74a1cf\n"
			"fcvt.s.w f23, x22, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 736(x27)\n"
			"fcvt.f16.ps f13, f31\n" 
			"maskpopc x18, m7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 224(x18)\n"
			"flw.ps f15, 480(x18)\n"
			"feq.s x15, f24, f15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 96(x22)\n"
			"flw.ps f12, 480(x22)\n"
			"flt.s x7, f7, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 64(x14)\n"
			"flw.ps f11, 480(x14)\n"
			"flw.ps f10, 128(x14)\n"
			"fnmadd.s f26, f22, f11, f10, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 96(x12)\n"
			"flw.ps f28, 192(x12)\n"
			"fsgnj.ps f22, f7, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 128(x15)\n"
			"flw.ps f2, 992(x15)\n"
			"fsgnjx.ps f26, f27, f2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 704(x15)\n"
			"flw.ps f12, 960(x15)\n"
			"flt.pi f6, f21, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xca << TENSOR_FMA16_SCP_LOC_B) | (0x78 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f27, 384(x18)\n"
			"flw.ps f29, 96(x18)\n"
			"for.pi f24, f27, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 896(x22)\n"
			"fsrai.pi f10, f16, 0x6\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 704(x27)\n"
			"flw.ps f25, 608(x27)\n"
			"fminu.pi f12, f3, f25\n" 
			"li x27, 0xb9b837361d4a089a\n"
			"fcvt.s.wu f15, x27, rmm\n" 
			"li x5, 0x812\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_int_32\n"
			"flw.ps f14, 896(x19)\n"
			"fsatu8.pi f21, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 128(x22)\n"
			"fslli.pi f15, f14, 0x9\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 640(x17)\n"
			"flw.ps f1, 800(x17)\n"
			"fmulhu.pi f11, f8, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 832(x19)\n"
			"fsub.pi f22, f3, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 576(x18)\n"
			"flw.ps f21, 928(x18)\n"
			"feq.pi f17, f29, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 960(x15)\n"
			"flw.ps f8, 576(x15)\n"
			"fmin.s f27, f30, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 992(x18)\n"
			"fsgnjx.ps f6, f17, f17\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 416(x12)\n"
			"fcvt.ps.pw f25, f24, rmm\n" 
			"li x5, 0x7c7dc7232f3b77cb\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfb5ff8b27c5925bc\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f7, 992(x23)\n"
			"flw.ps f17, 0(x23)\n"
			"fmulhu.pi f27, f7, f17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 352(x15)\n"
			"fswizz.ps f19, f1, 0xf\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 704(x17)\n"
			"fcvt.pw.ps f19, f24, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 928(x5)\n"
			"fcvt.ps.pwu f14, f25, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x3 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xe << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x2 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f24, 320(x5)\n"
			"flw.ps f21, 608(x5)\n"
			"fmax.pi f8, f24, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 672(x17)\n"
			"flw.ps f16, 480(x17)\n"
			"fmaxu.pi f3, f30, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 64(x25)\n"
			"fsin.ps f20, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_12:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x15, m1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"maskand m5, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f26, 32(x13)\n"
			"fcvt.ps.pwu f24, f26, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"maskxor m3, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x3eaa\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 704(x16)\n"
			"flw.ps f11, 608(x16)\n"
			"flw.ps f27, 544(x16)\n"
			"fmsub.s f28, f24, f11, f27, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 864(x26)\n"
			"flw.ps f2, 128(x26)\n"
			"fsgnjx.s f9, f7, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_12:\n"
			"fltm.ps m1, f16, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f3, 640(x7)\n"
			"fsat8.pi f17, f3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 576(x27)\n"
			"fcvt.f16.ps f3, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 96(x13)\n"
			"fswizz.ps f7, f21, 0xc\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_12:\n"
			"fltm.pi m7, f11, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_12:\n"
			"maskxor m4, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 672(x23)\n"
			"flw.ps f14, 224(x23)\n"
			"fmax.ps f13, f1, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"flem.ps m4, f30, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 384(x9)\n"
			"feq.s x16, f27, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 0(x10)\n"
			"flw.ps f16, 768(x10)\n"
			"fcmovm.ps f5, f13, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 704(x9)\n"
			"fcvt.w.s x6, f14, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 928(x17)\n"
			"flw.ps f15, 576(x17)\n"
			"fnmsub.ps f27, f0, f15, f15, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 576(x16)\n"
			"flw.ps f23, 608(x16)\n"
			"fsgnjn.ps f15, f28, f23\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 480(x10)\n"
			"flw.ps f31, 32(x10)\n"
			"feq.s x26, f0, f31\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 864(x7)\n"
			"fsrli.pi f20, f20, 0x3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 192(x5)\n"
			"flw.ps f24, 672(x5)\n"
			"fmax.pi f18, f8, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 96(x25)\n"
			"flw.ps f4, 224(x25)\n"
			"fsgnjn.s f18, f7, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 800(x27)\n"
			"flw.ps f24, 512(x27)\n"
			"fsgnjx.ps f13, f1, f24\n" 
			"la x25, rand_int_32\n"
			"flw.ps f5, 448(x25)\n"
			"fsrli.pi f28, f5, 0x0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 64(x6)\n"
			"flw.ps f8, 416(x6)\n"
			"fmulh.pi f13, f0, f8\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 992(x13)\n"
			"fswizz.ps f1, f16, 0x1a\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 608(x5)\n"
			"flw.ps f2, 960(x5)\n"
			"fmul.pi f22, f0, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 448(x19)\n"
			"fclass.ps f7, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 768(x25)\n"
			"fpackreph.pi f16, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_13:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 640(x12)\n"
			"frcp.ps f21, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 960(x9)\n"
			"flw.ps f10, 832(x9)\n"
			"fmul.s f15, f24, f10, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 0(x11)\n"
			"flw.ps f31, 224(x11)\n"
			"fmin.ps f1, f12, f31\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 736(x13)\n"
			"fle.ps f0, f8, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 0(x18)\n"
			"flw.ps f14, 960(x18)\n"
			"fmax.s f4, f23, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 352(x26)\n"
			"flw.ps f29, 448(x26)\n"
			"fsgnjn.ps f1, f8, f29\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 192(x16)\n"
			"flw.ps f3, 672(x16)\n"
			"fadd.ps f13, f4, f3, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 832(x23)\n"
			"flw.ps f3, 896(x23)\n"
			"fsgnjn.ps f6, f31, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fsetm.pi m5, f25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x67bb169d47\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"fsetm.pi m3, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f28, 320(x14)\n"
			"fcvt.ps.pwu f9, f28, rdn\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f0, 160(x13)\n"
			"fcvt.ps.f16 f19, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 352(x13)\n"
			"frsq.ps f2, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 768(x17)\n"
			"fround.ps f2, f13, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 896(x7)\n"
			"flw.ps f13, 256(x7)\n"
			"flw.ps f5, 928(x7)\n"
			"fnmsub.ps f4, f17, f13, f5, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 288(x9)\n"
			"fpackreph.pi f4, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 832(x9)\n"
			"flw.ps f11, 384(x9)\n"
			"fmax.pi f16, f22, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 704(x15)\n"
			"flw.ps f5, 480(x15)\n"
			"flw.ps f15, 96(x15)\n"
			"fnmsub.ps f2, f18, f5, f15, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f3, 256(x15)\n"
			"flw.ps f20, 96(x15)\n"
			"fltu.pi f25, f3, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_13:\n"
			"mov.m.x m7, x5, 0x38\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 32(x12)\n"
			"ffrc.ps f14, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"feqm.ps m4, f3, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f6, 576(x26)\n"
			"flw.ps f28, 352(x26)\n"
			"fsub.pi f19, f6, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 416(x26)\n"
			"flw.ps f30, 800(x26)\n"
			"fmin.ps f31, f10, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"fltm.ps m7, f10, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 32(x19)\n"
			"flw.ps f12, 64(x19)\n"
			"flt.ps f24, f15, f12\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f24, 64(x26)\n"
			"fcvt.ps.f16 f2, f24\n" 
			"li x6, 0xca0295598aa865dc\n"
			"fcvt.s.w f26, x6, rdn\n" 
			"maskpopcz x26, m4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 416(x17)\n"
			"ffrc.ps f29, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f31, 448(x17)\n"
			"fpackreph.pi f19, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"feqm.ps m5, f26, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf41d73e34d\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f4, 448(x18)\n"
			"fsrli.pi f9, f4, 0xd\n" 
			"la x28, rand_int_32\n"
			"flw.ps f3, 672(x28)\n"
			"fcvt.ps.pwu f9, f3, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 192(x27)\n"
			"fpackreph.pi f18, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 800(x22)\n"
			"flw.ps f30, 672(x22)\n"
			"fsgnj.s f26, f16, f30\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 32(x13)\n"
			"fsin.ps f7, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"mova.m.x x26\n" 
			"li x5, 0x803401E8\n"
			"ld x5, 0(x5)\n"
			"la x9, rand_int_32\n"
			"flw.ps f30, 256(x9)\n"
			"flw.ps f0, 256(x9)\n"
			"fmax.pi f29, f30, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 256(x26)\n"
			"flw.ps f31, 96(x26)\n"
			"fsgnjn.ps f31, f21, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 544(x7)\n"
			"flw.ps f22, 512(x7)\n"
			"flw.ps f26, 320(x7)\n"
			"fcmov.ps f27, f0, f22, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 512(x26)\n"
			"flw.ps f4, 864(x26)\n"
			"for.pi f14, f2, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 64(x15)\n"
			"flw.ps f21, 128(x15)\n"
			"feq.ps f24, f18, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 800(x6)\n"
			"fclass.s x14, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 608(x9)\n"
			"fxor.pi f20, f13, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 480(x7)\n"
			"flw.ps f9, 672(x7)\n"
			"fmax.ps f11, f3, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 672(x26)\n"
			"flw.ps f13, 160(x26)\n"
			"flw.ps f12, 768(x26)\n"
			"fmadd.ps f29, f7, f13, f12, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"mova.m.x x12\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"maskpopc x9, m7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 736(x22)\n"
			"flw.ps f1, 320(x22)\n"
			"fadd.pi f28, f4, f1\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 416(x18)\n"
			"flw.ps f24, 448(x18)\n"
			"flt.pi f8, f4, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 224(x27)\n"
			"flw.ps f4, 768(x27)\n"
			"flw.ps f9, 384(x27)\n"
			"fmsub.s f3, f30, f4, f9, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 96(x23)\n"
			"fcvt.w.s x6, f26, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 992(x25)\n"
			"flw.ps f25, 64(x25)\n"
			"fle.s x22, f21, f25\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 928(x28)\n"
			"flw.ps f30, 192(x28)\n"
			"flw.ps f16, 608(x28)\n"
			"fcmov.ps f21, f28, f30, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"fltm.ps m2, f25, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 800(x7)\n"
			"fclass.ps f31, f30\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_14:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 384(x25)\n"
			"fexp.ps f28, f21\n" 
			"li x10, 0x5a2def4061446ec6\n"
			"fcvt.s.w f9, x10, rtz\n" 
			"li x5, 0x9a5fd67252f05243\n"
			"fcvt.s.w f1, x5, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 640(x16)\n"
			"flw.ps f27, 128(x16)\n"
			"for.pi f19, f18, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 192(x15)\n"
			"fclass.s x22, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x6d << TENSOR_FMA32_SCP_LOC_B) | (0xce << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xf2 << TENSOR_IMA_SCP_LOC_B) | (0xc6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_14:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 288(x10)\n"
			"fexp.ps f5, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f24, 192(x10)\n"
			"flw.ps f11, 640(x10)\n"
			"fle.pi f12, f24, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 736(x22)\n"
			"flw.ps f1, 0(x22)\n"
			"feq.s x17, f6, f1\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 768(x26)\n"
			"flw.ps f9, 704(x26)\n"
			"feq.pi f0, f6, f9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 704(x25)\n"
			"flw.ps f17, 704(x25)\n"
			"for.pi f8, f20, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 64(x11)\n"
			"flw.ps f0, 800(x11)\n"
			"fsgnjn.s f19, f19, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f19, 448(x16)\n"
			"flw.ps f11, 64(x16)\n"
			"fmin.pi f10, f19, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 256(x14)\n"
			"fcvt.f16.ps f26, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 864(x23)\n"
			"flw.ps f27, 512(x23)\n"
			"fmin.pi f22, f4, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 960(x26)\n"
			"flw.ps f30, 896(x26)\n"
			"feq.ps f3, f18, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 512(x19)\n"
			"flw.ps f12, 160(x19)\n"
			"fsra.pi f21, f5, f12\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_14:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 320(x5)\n"
			"fexp.ps f3, f20\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 448(x5)\n"
			"fslli.pi f10, f10, 0x6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 640(x28)\n"
			"fcvt.ps.pwu f18, f4, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f29, 576(x27)\n"
			"fpackreph.pi f27, f29\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 32(x13)\n"
			"fcvt.f16.ps f30, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xc << TENSOR_IMA_SCP_LOC_B) | (0x9b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
		c_sync_point(shire_id, sync_odd_harts=1); 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 24\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
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
			"li x5, 0x3\n" 
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
			"la x22, rand_int_32\n"
			"flw.ps f3, 288(x22)\n"
			"fsat8.pi f22, f3\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_15:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 288(x14)\n"
			"flog.ps f19, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f25, 640(x12)\n"
			"flw.ps f3, 224(x12)\n"
			"fsll.pi f3, f25, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 96(x15)\n"
			"flw.ps f29, 96(x15)\n"
			"fsgnjn.s f10, f27, f29\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 960(x25)\n"
			"flw.ps f16, 928(x25)\n"
			"fadd.pi f13, f3, f16\n" 
			"maskpopc x25, m1\n" 
			"la x5, rand_ieee754_16\n"
			"flw.ps f13, 288(x5)\n"
			"fcvt.ps.f16 f21, f13\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 96(x11)\n"
			"flw.ps f28, 224(x11)\n"
			"flw.ps f31, 384(x11)\n"
			"fmadd.ps f2, f21, f28, f31, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 672(x26)\n"
			"flw.ps f9, 224(x26)\n"
			"fmin.ps f19, f19, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 0(x15)\n"
			"flw.ps f12, 768(x15)\n"
			"fsgnj.s f23, f28, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 832(x23)\n"
			"fclass.ps f15, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 832(x17)\n"
			"flw.ps f15, 480(x17)\n"
			"feq.pi f20, f26, f15\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 192(x25)\n"
			"fsat8.pi f29, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 192(x10)\n"
			"fcvt.wu.s x11, f17, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 352(x20)\n"
			"flw.ps f30, 800(x20)\n"
			"fle.ps f22, f2, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"flem.ps m4, f30, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 992(x17)\n"
			"fround.ps f14, f13, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 224(x19)\n"
			"flw.ps f7, 0(x19)\n"
			"fsgnj.s f14, f30, f7\n" 
			"fbci.pi f6, 0x2045d\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 320(x11)\n"
			"flw.ps f26, 64(x11)\n"
			"feq.s x19, f7, f26\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 0(x7)\n"
			"flw.ps f9, 512(x7)\n"
			"fsgnjn.s f12, f15, f9\n" 
			"maskpopcz x23, m3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 32(x27)\n"
			"flw.ps f23, 704(x27)\n"
			"fmul.ps f9, f4, f23, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 672(x14)\n"
			"flw.ps f25, 448(x14)\n"
			"flw.ps f10, 512(x14)\n"
			"fnmsub.ps f19, f13, f25, f10, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 608(x15)\n"
			"flw.ps f29, 224(x15)\n"
			"fsgnj.s f13, f2, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_15:\n"
			"masknot m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_15:\n"
			"maskor m4, m1, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x14, 0xc058a491b6162530\n"
			"fcvt.s.wu f23, x14, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 192(x16)\n"
			"flw.ps f26, 288(x16)\n"
			"fmul.s f14, f6, f26, rup\n" 
			"maskpopcz x20, m3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 32(x26)\n"
			"flw.ps f3, 352(x26)\n"
			"fadd.s f30, f0, f3, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_15:\n"
			"feqm.ps m4, f5, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x7, 0x452868ea6a8103f1\n"
			"fcvt.s.lu f15, x7, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 416(x11)\n"
			"flw.ps f12, 288(x11)\n"
			"fxor.pi f1, f15, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 768(x9)\n"
			"flw.ps f5, 576(x9)\n"
			"flw.ps f16, 256(x9)\n"
			"fmsub.ps f24, f19, f5, f16, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 128(x7)\n"
			"flw.ps f9, 960(x7)\n"
			"feq.s x5, f27, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_15:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f25, 608(x17)\n"
			"flw.ps f0, 416(x17)\n"
			"fmaxu.pi f8, f25, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_15:\n"
			"maskand m0, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 64(x25)\n"
			"flw.ps f31, 704(x25)\n"
			"flw.ps f17, 544(x25)\n"
			"fnmadd.s f2, f9, f31, f17, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 672(x15)\n"
			"flw.ps f7, 576(x15)\n"
			"fle.pi f11, f5, f7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 704(x28)\n"
			"fslli.pi f2, f9, 0xd\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 96(x26)\n"
			"flw.ps f2, 704(x26)\n"
			"flw.ps f3, 992(x26)\n"
			"fnmadd.s f22, f31, f2, f3, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f5, 32(x20)\n"
			"fsatu8.pi f16, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 256(x15)\n"
			"flw.ps f6, 768(x15)\n"
			"feq.pi f31, f5, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 32(x10)\n"
			"flw.ps f27, 800(x10)\n"
			"fdiv.ps f23, f4, f27, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
		0x233b9d70, 
		0x96cada02, 
		0x0b9c0581, 
		0x1d78c18a, 
		0x4b3398be, 
		0xca107fd9, 
		0xc2569c78, 
		0x34d1233a, 
		0xd6da9158, 
		0x084fa099, 
		0x1cbbf78d, 
		0x1c9e5f0d, 
		0x97f63cbd, 
		0xf2b3e9a3, 
		0xbe672eef, 
		0x2c4501f3, 
		0xeb3fb6ec, 
		0xd40ac764, 
		0xb6c88ed8, 
		0x44904b2f, 
		0xce83d4f9, 
		0x4d4d739c, 
		0x161f4c69, 
		0xfbd7d8e7, 
		0x77b30256, 
		0x9615b85a, 
		0xf83a1526, 
		0x12668515, 
		0xdd71899d, 
		0x31a7d505, 
		0xd81ab937, 
		0xda185ffa, 
		0xfe30bad1, 
		0x9f741d28, 
		0xdf7ccf89, 
		0x6068d652, 
		0x397e2988, 
		0xdb4fc364, 
		0x7155d602, 
		0x7c7b906b, 
		0xd016e0e9, 
		0x7590c875, 
		0x6df6a20e, 
		0xdffc29ca, 
		0x67342fa0, 
		0x1e79c4de, 
		0x49c4745c, 
		0xe2622659, 
		0xfc94491f, 
		0xb0bd5368, 
		0xde803c60, 
		0xc0852376, 
		0xb335e862, 
		0xbb15ef5b, 
		0xa40bbbcf, 
		0x34fc899c, 
		0x2eb26dd5, 
		0xd9eb2507, 
		0x1a518b07, 
		0x49a17cb2, 
		0x44320706, 
		0xd1ee93e2, 
		0x7da56266, 
		0xf338cd8a, 
		0x09dc829d, 
		0x87c3356d, 
		0xd1f68dbe, 
		0x193c0361, 
		0x2c7527b2, 
		0x2fd580b8, 
		0xc87998ca, 
		0x9bbb017e, 
		0x6591725a, 
		0xa0ed0e92, 
		0x3ff2d9eb, 
		0xf4b2f068, 
		0xb41c299b, 
		0x986dce9d, 
		0x30eb686e, 
		0x0fb9a025, 
		0x7c60f8db, 
		0x9ad96a33, 
		0xb46cdc8a, 
		0x6aa04748, 
		0x52651daa, 
		0x3efa5ee7, 
		0xcf924f3a, 
		0x579a44f5, 
		0xadf47da9, 
		0xc916042e, 
		0xd935436a, 
		0xeb89569f, 
		0x6d1d4954, 
		0x9cd62a81, 
		0x47f66759, 
		0xf505f04c, 
		0x4118a18d, 
		0xa5336d67, 
		0x375552c0, 
		0x5a9ee0f2, 
		0x56c73a1a, 
		0x1c6de1be, 
		0x4c985c19, 
		0xd29cc9cf, 
		0x419d1434, 
		0x4390a633, 
		0x90efb403, 
		0x8e345003, 
		0x631b3a67, 
		0x6524d00a, 
		0x85b4e2d2, 
		0x1b42cca2, 
		0x64c64e1f, 
		0x69ca5217, 
		0xd0536dc2, 
		0x885ff34b, 
		0xa188eb76, 
		0xd41553c3, 
		0x02497b2d, 
		0x06b5e6f3, 
		0xf88fa015, 
		0xa85d762d, 
		0xc7338c58, 
		0xaa1f3360, 
		0x6020e700, 
		0x4544723c, 
		0x43ca07ba, 
		0xddc26624, 
		0x01c2f899, 
		0x059ea216, 
		0x2f1a4b2c, 
		0xc8e59b51, 
		0x91efe692, 
		0xb21e96ac, 
		0xe361c03f, 
		0x963139f8, 
		0x059670d3, 
		0x1a30d532, 
		0x1e50cbde, 
		0x4f609792, 
		0x464f4a93, 
		0xfbbf7a77, 
		0xf6f32dc4, 
		0x69bb09bd, 
		0x12067066, 
		0xd0fd6169, 
		0x669caf10, 
		0xb4d12500, 
		0xea2cbccc, 
		0x6d4ebbde, 
		0x45e15d6d, 
		0xaef1b132, 
		0x606a27c1, 
		0x4c5e25c1, 
		0xa25c6fba, 
		0x0cd81986, 
		0xf7e5971c, 
		0x579dc3fc, 
		0x1352bf08, 
		0x8033da71, 
		0x87827d89, 
		0xc192cc7e, 
		0xdaaffc11, 
		0x3ab291b9, 
		0x7ac9cd5e, 
		0x56d441d7, 
		0xf0c8faf2, 
		0x74d1d80c, 
		0x87a28657, 
		0xe12da36f, 
		0x98b9fc45, 
		0x8524c972, 
		0x48d3ccf6, 
		0x4698fd24, 
		0xa4339217, 
		0x082eab29, 
		0xc729b9e8, 
		0xf664edbf, 
		0x6eaa37ba, 
		0x8a99bd3b, 
		0xcd1640c6, 
		0xad4b03e1, 
		0x93c0ffaf, 
		0x7c30576e, 
		0x02fe8df3, 
		0x57f705e8, 
		0x65381699, 
		0xc177f3a2, 
		0xdcc0a970, 
		0x52dc5f0c, 
		0xa13e07e0, 
		0x0518cdf6, 
		0x2ea4fe10, 
		0xabf905dd, 
		0x746a95fa, 
		0x57a3737c, 
		0x1407613d, 
		0x0d26214f, 
		0x448a0fa8, 
		0xb2504c71, 
		0x61738578, 
		0xb46491ad, 
		0xd1011900, 
		0x379dd781, 
		0x8f36478a, 
		0xb990c768, 
		0x374c57e6, 
		0xb0cbc1d6, 
		0x2e41ca6e, 
		0x81099617, 
		0xd515d3b8, 
		0x8b923123, 
		0x3e680a7a, 
		0xd415bd0e, 
		0x6f12b1e3, 
		0x00f550ef, 
		0xa0c407c0, 
		0xffec1063, 
		0xf5944ecb, 
		0xaec3e0a3, 
		0x98093048, 
		0xbe869dc7, 
		0x16208f2d, 
		0x771d0767, 
		0xb25df433, 
		0xa9bd04b2, 
		0xfa37be66, 
		0xbd6c61d9, 
		0x64bf8b18, 
		0x9cc1ded3, 
		0x98f2245c, 
		0xc991b0be, 
		0xbda57ce2, 
		0x25e8dff0, 
		0x3603437a, 
		0x0baba573, 
		0x6b5ec63d, 
		0xd17cf226, 
		0x2c0e07e3, 
		0x6093be18, 
		0xc50a2e77, 
		0x06169f89, 
		0x260ccbf3, 
		0x241faad3, 
		0x1cb237c8, 
		0x35803868, 
		0xb3f87364, 
		0x1daa487d, 
		0x3d452639, 
		0x88561148, 
		0xb7131b83, 
		0x3e116404, 
		0x8c94d9f2, 
		0x70161cb5, 
		0x67974bc2, 
		0xb3b1ab03  
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
		0x0000, 
		0x05eb, 
		0x8000, 
		0x4f40, 
		0x5140, 
		0x3f2f, 
		0xd380, 
		0x7c00, 
		0x4200, 
		0xc1cb, 
		0x4fb0, 
		0x0767, 
		0xd000, 
		0x5100, 
		0x50e0, 
		0x0000, 
		0xd731, 
		0x4980, 
		0xcb80, 
		0xd140, 
		0x4cc0, 
		0x4ca4, 
		0x8000, 
		0xd120, 
		0x50e0, 
		0x6755, 
		0x5260, 
		0xcfcd, 
		0xabb1, 
		0xd2c0, 
		0x4880, 
		0x5160, 
		0x5574, 
		0x50a0, 
		0x5a57, 
		0xcec0, 
		0xf910, 
		0xcd40, 
		0x8000, 
		0x5260, 
		0xd200, 
		0xafd3, 
		0x5100, 
		0x8000, 
		0x5080, 
		0x4880, 
		0x6ea5, 
		0xd300, 
		0xc800, 
		0xd320, 
		0x6321, 
		0xd260, 
		0x4e40, 
		0x4c00, 
		0xcf40, 
		0x0000, 
		0x7c8e, 
		0x8000, 
		0x1f5e, 
		0x53a0, 
		0x0000, 
		0x0000, 
		0x5040, 
		0x0000, 
		0xbe01, 
		0x8000, 
		0x0439, 
		0x5260, 
		0xcb00, 
		0xe693, 
		0x4000, 
		0xfc00, 
		0x5300, 
		0xd060, 
		0x96e0, 
		0xfc00, 
		0x5040, 
		0xd8ae, 
		0xd160, 
		0xb23e, 
		0x8000, 
		0xda28, 
		0xf3e1, 
		0x7d9b, 
		0x4dab, 
		0xbf57, 
		0xb1a0, 
		0x3c00, 
		0xfc00, 
		0x53a0, 
		0x9387, 
		0xd3e0, 
		0x9a5e, 
		0xb5aa, 
		0x10fa, 
		0xe02f, 
		0x4200, 
		0xd180, 
		0xce40, 
		0x9709, 
		0x8000, 
		0x3b71, 
		0xc600, 
		0x0000, 
		0x52e0, 
		0x52c0, 
		0x5040, 
		0xd47f, 
		0x0da2, 
		0x3a30, 
		0xfc00, 
		0xd3e0, 
		0x77b0, 
		0x0000, 
		0xfc00, 
		0xcc80, 
		0x51c0, 
		0xcf40, 
		0xd0e0, 
		0x5240, 
		0x4700, 
		0xd120, 
		0x4e40, 
		0xc000, 
		0xc800, 
		0x53c0, 
		0x7c00, 
		0x4f7c, 
		0xd3c0, 
		0x4c00, 
		0xbf1c, 
		0xd200, 
		0xca00, 
		0x8000, 
		0xfc00, 
		0x79cc, 
		0xd300, 
		0xd000, 
		0xcf40, 
		0x057c, 
		0x52a0, 
		0xd3e0, 
		0x9fa1, 
		0x3e82, 
		0x4b80, 
		0x5260, 
		0xcf40, 
		0xcf80, 
		0xfc1d, 
		0xd120, 
		0x5847, 
		0x04d0, 
		0x496e, 
		0x4900, 
		0xce00, 
		0xa27c, 
		0x4700, 
		0xcec0, 
		0x4f80, 
		0x52e0, 
		0x52e0, 
		0x65fc, 
		0xd0e0, 
		0x5240, 
		0x6b36, 
		0xc500, 
		0x53c0, 
		0xd2e0, 
		0x4a00, 
		0x53c0, 
		0xcd00, 
		0x4c80, 
		0xcc80, 
		0x5180, 
		0xc600, 
		0xd380, 
		0x4e40, 
		0xd1e0, 
		0xd260, 
		0x5000, 
		0xca80, 
		0x60a4, 
		0x4400, 
		0x5220, 
		0xd160, 
		0x5160, 
		0xd180, 
		0xca80, 
		0x9627, 
		0xf169, 
		0xc400, 
		0x5240, 
		0x0000, 
		0xcec0, 
		0x5300, 
		0x24b9, 
		0xd3e0, 
		0xaade, 
		0x2921, 
		0xce00, 
		0xcd80, 
		0x8bb6, 
		0x51a7, 
		0x4b80, 
		0xc200, 
		0xc200, 
		0x5120, 
		0x4fc0, 
		0x4980, 
		0x7db2, 
		0x4d00, 
		0xcd7f, 
		0x8000, 
		0xd2e0, 
		0xcb80, 
		0x5340, 
		0xd180, 
		0x4f80, 
		0xfda6, 
		0xd160, 
		0xfc00, 
		0xe665, 
		0xd140, 
		0xc400, 
		0x4600, 
		0xce80, 
		0x09c1, 
		0x4900, 
		0x52c0, 
		0x44dc, 
		0x2abb, 
		0xfcc5, 
		0x0000, 
		0xd5a4, 
		0xcb00, 
		0x3b66, 
		0x4e00, 
		0x49dd, 
		0xd315, 
		0xd280, 
		0xc500, 
		0x6ab9, 
		0x5100, 
		0x8000, 
		0xd120, 
		0xd0e0, 
		0xc200, 
		0x4400, 
		0xb79e, 
		0x2157, 
		0xd220, 
		0xd2c0, 
		0x4f40, 
		0xc888, 
		0x0000, 
		0x8000, 
		0x0000, 
		0xd1e0, 
		0xcb80, 
		0x8000, 
		0x9959, 
		0xc700, 
		0xcfc0, 
		0x21d9, 
		0xb980, 
		0x52e0, 
		0x0000, 
		0x4d00, 
		0xd060, 
		0x5280, 
		0x4d40, 
		0x5340, 
		0x53e0, 
		0x4900, 
		0xd1a0, 
		0xded2, 
		0xd060, 
		0xc200, 
		0xc900, 
		0x7a4b, 
		0x5120, 
		0x52e0, 
		0xd280, 
		0xd160, 
		0xad29, 
		0x4980, 
		0x693b, 
		0x5320, 
		0x5280, 
		0xd2e0, 
		0x4fc0, 
		0xc220, 
		0x7c00, 
		0x4600, 
		0xcc40, 
		0x4500, 
		0xe14b, 
		0x5160, 
		0x99cf, 
		0x79d0, 
		0xd2c0, 
		0x4dc0, 
		0xd220, 
		0xd3e0, 
		0x0000, 
		0xd380, 
		0x5280, 
		0x5000, 
		0xc600, 
		0x4d80, 
		0xca80, 
		0xcb03, 
		0xcf40, 
		0xd46b, 
		0xfc00, 
		0xd1a0, 
		0x2f9c, 
		0x252e, 
		0xc980, 
		0x5079, 
		0x628f, 
		0xd203, 
		0x709a, 
		0xf3b9, 
		0x5380, 
		0xc880, 
		0x4a00, 
		0x5260, 
		0x4200, 
		0x8b05, 
		0x50c0, 
		0xc600, 
		0x30de, 
		0x2eb4, 
		0x5240, 
		0xd280, 
		0x52e0, 
		0xd100, 
		0x4880, 
		0xcd80, 
		0x8000, 
		0xd5e1, 
		0xd260, 
		0xd1a0, 
		0xc900, 
		0x7c00, 
		0x559e, 
		0xc928, 
		0x4d00, 
		0xbc00, 
		0x1aa5, 
		0x0007, 
		0xc500, 
		0x0c66, 
		0x52a0, 
		0x7876, 
		0x6df4, 
		0x5380, 
		0xdc6c, 
		0x5200, 
		0x4500, 
		0xdc01, 
		0x53c0, 
		0xd200, 
		0xea3b, 
		0xd220, 
		0x4fc0, 
		0x0000, 
		0x7c00, 
		0xcc00, 
		0x5040, 
		0x52e0, 
		0x4fc0, 
		0x0c27, 
		0x6367, 
		0x7f14, 
		0xca80, 
		0xbc00, 
		0xcd00, 
		0xcd00, 
		0x4ec0, 
		0x53c0, 
		0xd060, 
		0xcdc0, 
		0x277f, 
		0x5080, 
		0x1821, 
		0x4cc0, 
		0xca80, 
		0x7c00, 
		0x0000, 
		0x94e8, 
		0xcec0, 
		0xd100, 
		0x50c0, 
		0xd140, 
		0xcfc0, 
		0xce00, 
		0x5000, 
		0xd3e0, 
		0x5240, 
		0xb16e, 
		0xd1a0, 
		0xfa99, 
		0xcb80, 
		0x70d7, 
		0x8000, 
		0x4e00, 
		0x2d76, 
		0xff96, 
		0xd100, 
		0xd260, 
		0x4980, 
		0xcf40, 
		0xd040, 
		0x8000, 
		0x8000, 
		0xde7e, 
		0xc700, 
		0x50e0, 
		0x52a0, 
		0x5340, 
		0x4b80, 
		0x4880, 
		0x7c00, 
		0x8000, 
		0x4c00, 
		0x4e00, 
		0xd080, 
		0x4000, 
		0x4e80, 
		0x53c0, 
		0x0000, 
		0xfc00, 
		0x7a28, 
		0x5320, 
		0x7c00, 
		0xc600, 
		0x4f80, 
		0x52a0, 
		0x4000, 
		0xb239, 
		0xcdc0, 
		0xcf80, 
		0x50c0, 
		0x0000, 
		0x4d80, 
		0xd0a0, 
		0x4e00, 
		0x5000, 
		0xcc80, 
		0x1879, 
		0x4b80, 
		0x3c00, 
		0x5080, 
		0x5140, 
		0x5040, 
		0xd2c0, 
		0xc200, 
		0x5020, 
		0xce00, 
		0x3c00, 
		0xd380, 
		0xddef, 
		0xd240, 
		0x4f80, 
		0xd3e0, 
		0x52a0, 
		0x5040, 
		0x4880, 
		0x5080, 
		0xfd29, 
		0x9c54, 
		0x03e7, 
		0x51c0, 
		0x0000, 
		0x84df, 
		0xc600, 
		0xbcbf, 
		0xd2c0, 
		0xcff2, 
		0x7405, 
		0x1c35, 
		0x8000, 
		0xcf40, 
		0xc900, 
		0x085a, 
		0x4c00, 
		0xd080, 
		0xdb61, 
		0x0000, 
		0xd0c0, 
		0xd000, 
		0xfc00, 
		0xce00, 
		0x193b, 
		0x4c40, 
		0x927a, 
		0xce40, 
		0x8eff, 
		0xa305, 
		0xd340, 
		0x3c00, 
		0x3741, 
		0x4980, 
		0x4a00, 
		0x4cc0, 
		0x4f00, 
		0xd360, 
		0xcb00, 
		0xfc00, 
		0xd160, 
		0x7c00, 
		0x5020, 
		0xcd99, 
		0x5360, 
		0x52c0, 
		0x8000, 
		0x0000, 
		0x961c, 
		0x730e, 
		0x50c0, 
		0xd300, 
		0x5020, 
		0x83c5, 
		0xd2c0, 
		0x5220, 
		0xd0e0, 
		0x21a5, 
		0xcf00, 
		0x50c0, 
		0x5080, 
		0xfc00, 
		0x4c80, 
		0xcf80, 
		0xd623, 
		0xd120, 
		0xc000, 
		0xd1e0, 
		0x8000, 
		0x5240, 
		0x8e76, 
		0x4fc0, 
		0x8000, 
		0x4d80, 
		0x4ec0, 
		0xcf40, 
		0x8000, 
		0x4ec0, 
		0x5380, 
		0xfc00, 
		0x8000, 
		0xc600, 
		0xd3c0, 
		0x3802, 
		0x8000, 
		0xd0e0, 
		0x7c00, 
		0xce00, 
		0x0000, 
		0x9a62, 
		0x4d40, 
		0x4cc0, 
		0xfc00, 
		0xc200, 
		0x4c00, 
		0xfc00, 
		0x4c40, 
		0xce80, 
		0xc900, 
		0x0049, 
		0xb5ce, 
		0x4980, 
		0x8e42, 
		0xfc00, 
		0x1698, 
		0xd514, 
		0x5140, 
		0x5300, 
		0x5020, 
		0xd3e0, 
		0xcf40, 
		0xcca8, 
		0xd0a0, 
		0xd200, 
		0x8000, 
		0xcfc0, 
		0x013c, 
		0x7c00, 
		0x5000, 
		0x070e, 
		0xce00, 
		0xcc56, 
		0x5280, 
		0x4500, 
		0xcc80, 
		0x5300, 
		0x7e91, 
		0x4600, 
		0x3672, 
		0x5360, 
		0xd3cc, 
		0xd3c0, 
		0x5220, 
		0xcc40, 
		0xd2c0, 
		0xfdbe, 
		0xd000, 
		0x0000, 
		0xc900, 
		0x72d8, 
		0x4b00, 
		0xcc80, 
		0xfc16, 
		0x4d80, 
		0xd140, 
		0xcac2, 
		0x5340, 
		0xca80, 
		0xd1a0, 
		0xd020, 
		0xdac4, 
		0xca80, 
		0xcd80, 
		0x53c0, 
		0xcf80, 
		0x5040, 
		0x3c33, 
		0xd160, 
		0xc880, 
		0xcc80, 
		0xca80, 
		0xd260, 
		0x7c00, 
		0x4400, 
		0xd1c0, 
		0x57c2, 
		0x2b44, 
		0x4700, 
		0xd120, 
		0x3c00, 
		0x5ed6, 
		0xfd73, 
		0x5020, 
		0x8043, 
		0x51a0, 
		0x5120, 
		0x4000, 
		0xc400, 
		0x4f00, 
		0xcdc0, 
		0xe6ea, 
		0xfc00, 
		0x723a, 
		0x6012, 
		0xd020, 
		0x5140, 
		0xd1e0, 
		0x8000, 
		0xd100, 
		0x4880, 
		0xcf80, 
		0x1892, 
		0xd0e0, 
		0xcf40, 
		0xdc6f, 
		0xc900, 
		0x5340, 
		0x5283, 
		0xe524, 
		0xd1c0, 
		0x4d40, 
		0x0000, 
		0xd0e0, 
		0x51a0, 
		0xd3a0, 
		0xc500, 
		0xd3e0, 
		0x2c28, 
		0x4d00, 
		0xd080, 
		0xd2a0, 
		0x0939, 
		0x78fb, 
		0xd2a0, 
		0x86a4, 
		0x4b00, 
		0xc900, 
		0xd3e0, 
		0xcfc0, 
		0xce00, 
		0x7c00, 
		0x6b3a, 
		0x4700, 
		0xaeae, 
		0xcec0, 
		0xb411, 
		0x4400, 
		0x4900, 
		0xcfae, 
		0x4200, 
		0x50c0, 
		0x8303, 
		0x0fc9, 
		0x4200, 
		0xcd40, 
		0x51c0, 
		0x4d40, 
		0x5120, 
		0x589a, 
		0x4c40, 
		0xce80, 
		0xc800, 
		0xe3f8, 
		0xd000, 
		0xc980, 
		0x4c40, 
		0x5a19, 
		0xfa85, 
		0x6442, 
		0xd999, 
		0xd260, 
		0x751b, 
		0xcdc0, 
		0xd240, 
		0xeec5, 
		0x5431, 
		0x4e80, 
		0xd3e0, 
		0x84af, 
		0x000c, 
		0x1e14, 
		0x50a0, 
		0x5080, 
		0xcf80, 
		0x8000, 
		0xc800, 
		0xd240, 
		0x40c6, 
		0x2125, 
		0x8000, 
		0xd2c0, 
		0xd0e0, 
		0x8178, 
		0xcf80, 
		0xa8c2, 
		0x8000, 
		0xd0c0, 
		0xd080, 
		0xd180, 
		0xd359, 
		0x4f00, 
		0xc900, 
		0xd060, 
		0xcec0, 
		0x8000, 
		0xb7f0, 
		0x2c04, 
		0xd040, 
		0xcf80, 
		0x4ec0, 
		0x4800, 
		0xd2a0, 
		0x5120, 
		0xca80, 
		0xce00, 
		0xf735, 
		0xe9c4, 
		0xcfc0, 
		0xaa99, 
		0x4cc0, 
		0x4cc0, 
		0xe8ef, 
		0xd2a0, 
		0x50e0, 
		0x53a0, 
		0xd0a0, 
		0xcf00, 
		0x5080, 
		0x467c, 
		0xd3e0, 
		0x50a0, 
		0x5e48, 
		0x51a0, 
		0xc400, 
		0x7c00, 
		0x8ee7, 
		0xd280, 
		0x5140, 
		0x5120, 
		0x0f34, 
		0x9e3f, 
		0xfd84, 
		0x5180, 
		0xebd7, 
		0x49fa, 
		0xfdee, 
		0xcf40, 
		0xd140, 
		0x8000, 
		0x5320, 
		0x5360, 
		0x4f80, 
		0x4200, 
		0xd180, 
		0xd3e0, 
		0x50a0, 
		0x51a0, 
		0xd1c0, 
		0xcd00, 
		0xc200, 
		0xc965, 
		0xd380, 
		0x14ef, 
		0x96db, 
		0xc000, 
		0x5000, 
		0xc200, 
		0x5260, 
		0x2b72, 
		0x4e00, 
		0xce00, 
		0xd3c0, 
		0x53e0, 
		0x4b80, 
		0xf92b, 
		0x72f6, 
		0x9f35, 
		0x0d55, 
		0x53a0, 
		0x4880, 
		0x51e0, 
		0x6229, 
		0xd74e, 
		0xc600, 
		0x08f3, 
		0x011a, 
		0x0b48, 
		0x53e0, 
		0xa6f3, 
		0xe27f, 
		0xca80, 
		0xce00, 
		0xd3c0, 
		0x3a4a, 
		0xd3c0, 
		0xbc00, 
		0x4f00, 
		0xd000, 
		0x4c80, 
		0xd2e0, 
		0x01fa, 
		0xcc00, 
		0x4f00, 
		0x7c00, 
		0xd2c0, 
		0x39a4, 
		0x53e0, 
		0xd3e0, 
		0x0000, 
		0x3987, 
		0x5240, 
		0x51a0, 
		0x8ccb, 
		0xca80, 
		0xd1a0, 
		0x4fc0, 
		0xcdc0, 
		0x5180, 
		0x67a5, 
		0xd260, 
		0x08ad, 
		0x84b4, 
		0x5260, 
		0xd260, 
		0x5060, 
		0x5280, 
		0x4c40, 
		0x51e0, 
		0xbbcb, 
		0x7c67, 
		0x2ee2, 
		0xd220, 
		0xcb80, 
		0x9d32, 
		0x8000, 
		0x6b7d, 
		0xac35, 
		0x9599, 
		0x4600, 
		0x1ba4, 
		0xcafc, 
		0x5020, 
		0xcec0, 
		0x4d80, 
		0xd1a0, 
		0xd2e0, 
		0x5adb, 
		0x5140, 
		0xf911, 
		0xcd23, 
		0x9eed, 
		0xd020, 
		0xfd55, 
		0x5839, 
		0x4000, 
		0x9882, 
		0xca80, 
		0x4bdc, 
		0xcb00, 
		0x53c0, 
		0xc880, 
		0x5300, 
		0xfb34, 
		0x0513, 
		0xccc0, 
		0x5200, 
		0x143f, 
		0xcec0, 
		0x4c00, 
		0x25b8, 
		0x8000, 
		0x4d80, 
		0x5160, 
		0x4600, 
		0x6ec7, 
		0x1259, 
		0xcb80, 
		0xccc0, 
		0x94dd, 
		0x510a, 
		0xd0c0, 
		0xd200, 
		0xe1db, 
		0x4f40, 
		0x4f00, 
		0xb8f4, 
		0x4200, 
		0x4c40, 
		0xd1fc, 
		0xcd4c, 
		0x4d00, 
		0x5160, 
		0xcf40, 
		0x4800, 
		0xc700, 
		0xc000, 
		0xd100, 
		0xc929, 
		0x4200, 
		0xbc00, 
		0xfc00, 
		0xcd80, 
		0x4400, 
		0xd2a0, 
		0xfede, 
		0x4e80, 
		0xf12e, 
		0xd3a0, 
		0x23a5, 
		0xd300, 
		0xce80, 
		0x4980, 
		0x5060, 
		0x8ed1, 
		0x53a0, 
		0xc800, 
		0x6969, 
		0xce40, 
		0xce00, 
		0xce87, 
		0xcfc0, 
		0xd000, 
		0xfc00, 
		0x5380, 
		0xcec0, 
		0xcdc0, 
		0x8000, 
		0x4000, 
		0xcd00, 
		0xd000, 
		0xca80, 
		0x4c00, 
		0x1675, 
		0x4c40, 
		0xd0c0, 
		0xd100, 
		0xc400, 
		0x7c00, 
		0x39c2, 
		0xc400, 
		0x2892, 
		0x51a0, 
		0xfc00, 
		0xf64c, 
		0xcb80, 
		0x11ef, 
		0x5180, 
		0x4e40, 
		0x8f24, 
		0x4c00, 
		0xa194, 
		0xc1ea, 
		0x4600, 
		0xcd00, 
		0xcfc0, 
		0x2aad, 
		0xd2e0, 
		0x4000, 
		0xd000, 
		0x7022, 
		0x5120, 
		0xc900, 
		0xcb80, 
		0xb046, 
		0xa8ad, 
		0xd2e0, 
		0x8e6c, 
		0xfd8c, 
		0xd1a0, 
		0x1b0a, 
		0xd2a0, 
		0x7c00, 
		0xd020, 
		0x5380, 
		0x0000, 
		0x8000, 
		0xe21a, 
		0xd3c0, 
		0x0000, 
		0x8000, 
		0x7d38, 
		0xcfc0, 
		0xd380, 
		0x96e2, 
		0x48df, 
		0xcfc0, 
		0x75da, 
		0xa052, 
		0xcb00, 
		0x3508, 
		0x4c00, 
		0x5000, 
		0x5100, 
		0x5020, 
		0xcd7f, 
		0x50e0, 
		0xd0c0, 
		0xcf00, 
		0xd300, 
		0x7ab1, 
		0xc980, 
		0x4500, 
		0xc3d5, 
		0xeafc, 
		0x4b00, 
		0xd140, 
		0x50c0, 
		0x4200, 
		0x4e00, 
		0x4900, 
		0xd020, 
		0xb3fd, 
		0x786a, 
		0xd1e0, 
		0xcdc0, 
		0xd1a0, 
		0xc880, 
		0x4800, 
		0x53a0, 
		0xd1a0, 
		0xd320, 
		0xd1c0, 
		0x53e0, 
		0xfc00, 
		0x66ea, 
		0xb3a4, 
		0x4fc0, 
		0xcfc0, 
		0xfc00, 
		0xa11c, 
		0x5020, 
		0xc9f2, 
		0x9192, 
		0x8000, 
		0xd240, 
		0xd380, 
		0xfc24, 
		0x4793, 
		0xcb80, 
		0x0000, 
		0x53e0, 
		0xd2a0, 
		0xd0e0, 
		0x5080, 
		0xbeee, 
		0xcdc0, 
		0x4800, 
		0x4400, 
		0xd2e0, 
		0xc436, 
		0xbc00, 
		0x4800, 
		0xc000, 
		0x5140, 
		0x5380, 
		0xfc00, 
		0xccc0, 
		0xd0a0, 
		0x462b, 
		0xcec0, 
		0x4d00, 
		0xce00, 
		0x5080, 
		0x4b80, 
		0x878c, 
		0x4d00, 
		0x4700, 
		0xcc00, 
		0xcec0, 
		0x7b37, 
		0xcf31, 
		0xd200, 
		0x142c, 
		0x949f, 
		0x8000, 
		0xc600, 
		0x5380, 
		0xb7a7, 
		0xd240, 
		0x1924, 
		0xcc80, 
		0x50e0, 
		0xf709, 
		0x5080, 
		0x2b32, 
		0x4b80, 
		0xcf40, 
		0xd060, 
		0x8000, 
		0x5240, 
		0x4700, 
		0x0000, 
		0xd2a0, 
		0xce00, 
		0x4b80, 
		0x3cfc, 
		0x8b40, 
		0xd2e0, 
		0xb582, 
		0xd360, 
		0x8033, 
		0x4500, 
		0xd200, 
		0xb3f7, 
		0x6ed5, 
		0xd160, 
		0x8000, 
		0x5060, 
		0xd340, 
		0xcd00, 
		0x326d, 
		0x8000, 
		0x3487, 
		0xcec0, 
		0x7d7f, 
		0x4ec0, 
		0xd3a0, 
		0xf7fa, 
		0xe99a, 
		0x91b2, 
		0x6541, 
		0x4a9a, 
		0x8000, 
		0xc400, 
		0x8855, 
		0x4e00, 
		0xcf00, 
		0xd000, 
		0x5140, 
		0x51c0, 
		0x5340, 
		0x5260, 
		0x4200, 
		0x4cc0, 
		0xc500, 
		0x7639, 
		0xc600, 
		0x51a0, 
		0x4c40, 
		0xd180, 
		0x5240, 
		0x8688, 
		0x5220, 
		0x4ec0, 
		0xc500, 
		0x11c1, 
		0x8000, 
		0xb4be, 
		0x4a80, 
		0x4000, 
		0x52a0, 
		0xd3c0, 
		0x51a0, 
		0x0000, 
		0x8000, 
		0x720f, 
		0xe304, 
		0x0000, 
		0x3990, 
		0xd340, 
		0x6465, 
		0xc200, 
		0x0000, 
		0x6939, 
		0x4d40, 
		0xc500, 
		0x3c00, 
		0x14a0, 
		0x51a0, 
		0x7c00, 
		0x0bf3, 
		0x51a0, 
		0xd360, 
		0xca71, 
		0xab18, 
		0x7c00, 
		0x5100, 
		0xcb80, 
		0xca00, 
		0xd1e0, 
		0x7803, 
		0xd000, 
		0xc980, 
		0x4d40, 
		0x4e40, 
		0xcf80, 
		0x4700, 
		0x4900, 
		0x0162, 
		0x314a, 
		0x4400, 
		0xca00, 
		0xc900, 
		0x8000, 
		0x09fc, 
		0x2681, 
		0xca80, 
		0xbc00, 
		0x6d3e, 
		0xfc00, 
		0x51c0, 
		0x06f2, 
		0x3840, 
		0x7c6a, 
		0x8c05, 
		0xfc7f, 
		0x53e0, 
		0x5300, 
		0x50e0, 
		0x0000, 
		0x4c80, 
		0x4289, 
		0xd180, 
		0x4f40, 
		0x5220, 
		0x7daa, 
		0x5d85, 
		0xe774, 
		0x4500, 
		0x0000, 
		0x4200, 
		0xc500, 
		0x52e0, 
		0xd300, 
		0x0000, 
		0x4400, 
		0xd360, 
		0xfadd, 
		0xd300, 
		0x9cc0, 
		0xcd40, 
		0xce80, 
		0x7c00, 
		0x0142, 
		0x8040, 
		0x4d40, 
		0x53c0, 
		0x0000, 
		0x4f40, 
		0x4e00, 
		0x5320, 
		0x5220, 
		0xf5dd, 
		0x4d80, 
		0xd080, 
		0x4cc0, 
		0x7f07, 
		0xe1f7, 
		0xc648, 
		0x5020, 
		0x5340, 
		0x83f4, 
		0xaee2, 
		0x50fd, 
		0xb91b, 
		0x4793, 
		0x52a0, 
		0xc729, 
		0x855a, 
		0x7c00, 
		0xe27b, 
		0xe13f, 
		0x55de, 
		0x4800, 
		0xadd3, 
		0xbca5, 
		0x123b, 
		0xd060, 
		0x2887, 
		0x4e40, 
		0x5020, 
		0xcdc0, 
		0x5280, 
		0xb252, 
		0xd0c0, 
		0xccc0, 
		0x52e0, 
		0xd140, 
		0x2804, 
		0xc980, 
		0xd260, 
		0x33fc, 
		0x1ad0, 
		0xfc00, 
		0xcdc0, 
		0x51a0, 
		0xc469, 
		0x0d90, 
		0xcd00, 
		0xc500, 
		0xe78b, 
		0xd100, 
		0xe7ec, 
		0x7925, 
		0x554d, 
		0x1cdf, 
		0x12c4, 
		0x5320, 
		0x4880, 
		0xfb4c, 
		0x03b2, 
		0xccc0, 
		0x50c0, 
		0xf524, 
		0xb409, 
		0x5320, 
		0x3959, 
		0xc400, 
		0xd380, 
		0xcdb7, 
		0xc600, 
		0x8000, 
		0xfc00, 
		0xa240, 
		0x4500, 
		0x8000, 
		0x51c0, 
		0xd2a0, 
		0xc094, 
		0xd3c0, 
		0xce80, 
		0xd060, 
		0x5e5b, 
		0xcc00, 
		0xbf33, 
		0xb857, 
		0x8000, 
		0x72c4, 
		0x8000, 
		0xaac8, 
		0x00ba, 
		0xcc40, 
		0xd180, 
		0xd1c0, 
		0xd0c0, 
		0x80e9, 
		0xcb80, 
		0x41f9, 
		0xc500, 
		0xd020, 
		0x53a0, 
		0x81cd, 
		0x4dc0, 
		0x8000, 
		0xed36, 
		0x8000, 
		0xd360, 
		0xfc00, 
		0x2aa4, 
		0x4d00, 
		0xcc06, 
		0xca00, 
		0xd1c0, 
		0x5618, 
		0xd280, 
		0xd1c0, 
		0xf570, 
		0xc880, 
		0x0000, 
		0x5040, 
		0x896c, 
		0x8000, 
		0xccc0, 
		0xcd40, 
		0x50e0, 
		0x4880, 
		0x8000, 
		0x3c00, 
		0x5140, 
		0x8000, 
		0x4e40, 
		0x4327, 
		0x8000, 
		0x4880, 
		0x4b80, 
		0x0000, 
		0x4d80, 
		0xa355, 
		0xd5e2, 
		0xd000, 
		0x98d1, 
		0x31b5, 
		0xd040, 
		0x7f42, 
		0xd220, 
		0x39cd, 
		0x4e40, 
		0x51a0, 
		0x7c00, 
		0xfd1c, 
		0x5320, 
		0xd000, 
		0x5240, 
		0x52c0, 
		0xcf80, 
		0x1d94, 
		0xbc00, 
		0xcec0, 
		0xd1a0, 
		0x5360, 
		0x0000, 
		0xf97c, 
		0x5120, 
		0x0000, 
		0x4ec0, 
		0x5340, 
		0x0417, 
		0x80ee, 
		0xc800, 
		0x4400, 
		0xc566, 
		0x4f80, 
		0x7c00, 
		0x5320, 
		0x4200, 
		0x02c0, 
		0x8000, 
		0x5180, 
		0xd180, 
		0x2106, 
		0xd260, 
		0x9953, 
		0x4000, 
		0x81c3, 
		0xfc1d, 
		0x16f4, 
		0x0000, 
		0x6888, 
		0xd2c0, 
		0x4f40, 
		0xd180, 
		0x4600, 
		0x50c0, 
		0x4600, 
		0x4980, 
		0xd1a0, 
		0x0000, 
		0xcd40, 
		0xeb8d, 
		0xbc4b, 
		0xe0c4, 
		0x5e63, 
		0x06a5, 
		0xd0c0, 
		0xfc00, 
		0x93b1, 
		0x51a0, 
		0x27ef, 
		0xdd2b, 
		0x335d, 
		0xc1ef, 
		0xcec0, 
		0x53e0, 
		0x4e40, 
		0x4880, 
		0xd200, 
		0xce00, 
		0x865e, 
		0x6a6a, 
		0x52c0, 
		0x0000, 
		0x3c00, 
		0x3c00, 
		0x5000, 
		0xbc00, 
		0x4d80, 
		0x6a64, 
		0xd3c0, 
		0x51e0, 
		0x5973, 
		0x4a00, 
		0x8000, 
		0x5cb5, 
		0x82f0, 
		0x5340, 
		0xcd40, 
		0x0000, 
		0xd120, 
		0x3e47, 
		0xd1c0, 
		0xd320, 
		0xd3c0, 
		0xc500, 
		0xcb80, 
		0xcc80  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x80000004, 
		0xc1b80000, 
		0x0c700000, 
		0xc2500000, 
		0x00000000, 
		0x42300000, 
		0xc1980000, 
		0x0c7fff00, 
		0x80000000, 
		0xe81a599b, 
		0xff7ffffe, 
		0x80040000, 
		0xff8610bf, 
		0x00000000, 
		0xc27c0000, 
		0xc1700000, 
		0x00ffffff, 
		0x8f7ffffe, 
		0xd0324bd4, 
		0x7f7fffff, 
		0x31003994, 
		0xad689106, 
		0xff800001, 
		0x41e80000, 
		0xffa540e5, 
		0xc1600000, 
		0x41a00000, 
		0x0d00fff0, 
		0x7d642d56, 
		0x0c780000, 
		0xff800000, 
		0xc1300000, 
		0xc25c0000, 
		0x0e007fff, 
		0x00400000, 
		0x42500000, 
		0xc1f80000, 
		0xc4c80b58, 
		0xcb000000, 
		0x80008000, 
		0x0e007fff, 
		0x0e00ffff, 
		0x0c700000, 
		0x42100000, 
		0xc2200000, 
		0x00000000, 
		0x0c7ff000, 
		0xc1000000, 
		0x63c06759, 
		0xeaf5d29a, 
		0x80004000, 
		0x80001000, 
		0x8fa214cb, 
		0x80800001, 
		0x8b9df311, 
		0x40a00000, 
		0x4b8c4b40, 
		0xff800000, 
		0xeded3572, 
		0xc2400000, 
		0xc4972758, 
		0x0c7f0000, 
		0x0c7c0000, 
		0xfdf6ef51, 
		0x150b7ffb, 
		0x80000002, 
		0xff800000, 
		0xf802ded5, 
		0x41900000, 
		0xa8b34ab9, 
		0x80000000, 
		0x0e0001ff, 
		0x36abd360, 
		0x4811d57b, 
		0x80200000, 
		0x74243e41, 
		0xf39d7adf, 
		0xc2400000, 
		0xccd0142f, 
		0x7f800000, 
		0x42440000, 
		0x33333333, 
		0xc0800000, 
		0x00000000, 
		0xc2100000, 
		0x007fffff, 
		0x7c97667b, 
		0xa0bc8eb2, 
		0x54e730ee, 
		0x0c7fff80, 
		0x00000001, 
		0xc1a00000, 
		0xc0800000, 
		0xc21c0000, 
		0x0eaa8d5c, 
		0x7f800000, 
		0x00400000, 
		0x0c400000, 
		0xb802f501, 
		0xc401daba, 
		0x00020000, 
		0xc1000000, 
		0x42480000, 
		0x2518d9fa, 
		0xffbee0de, 
		0xc1200000, 
		0x00001000, 
		0x00000002, 
		0x7f800000, 
		0x0e003fff, 
		0x7f7fffff, 
		0x7f800000, 
		0xff7ffffe, 
		0x0c7ffffe, 
		0xc1200000, 
		0x80000000, 
		0x422c0000, 
		0x00271869, 
		0x2a1ea541, 
		0x5ef02c72, 
		0x7f5e0f69, 
		0x0e00003f, 
		0x80000004, 
		0xff800000, 
		0x42580000, 
		0xb1619a76, 
		0x80000001, 
		0xc1880000, 
		0x63c44b61, 
		0x80000000, 
		0x2d00fe54, 
		0xf93e15e9, 
		0xe84b2366, 
		0x0e001fff, 
		0x42680000, 
		0x7f800000, 
		0x0e007fff, 
		0x80800002, 
		0x7f800000, 
		0x17cdc3a8, 
		0x00800001, 
		0x00000000, 
		0x00008000, 
		0x65169a7b, 
		0x0e000001, 
		0x80000000, 
		0x00800000, 
		0xc2000000, 
		0x0e03ffff, 
		0x00800000, 
		0x40a00000, 
		0x4c255a79, 
		0x00000001, 
		0x42700000, 
		0x96427372, 
		0x80000000, 
		0x80800002, 
		0x0e001fff, 
		0x80000000, 
		0x0566639d, 
		0x00000000, 
		0x48415a0d, 
		0x00000800, 
		0xc2340000, 
		0x0f7ffffe, 
		0xff800000, 
		0x7f800000, 
		0x1849931b, 
		0x3201d5dd, 
		0x0f7ffffd, 
		0x7fc00001, 
		0xcdc900b1, 
		0x0f7ffffc, 
		0x7fffffff, 
		0xff800000, 
		0x42680000, 
		0xc1b00000, 
		0x00800003, 
		0x0c7fe000, 
		0x42140000, 
		0x80011111, 
		0xc1f80000, 
		0x2cdf9b38, 
		0x7fb99697, 
		0x0c7ff800, 
		0x00000002, 
		0xc1300000, 
		0x00ffffff, 
		0x7f7ffffe, 
		0x41a00000, 
		0x00200000, 
		0x0c7fc000, 
		0x00000000, 
		0xc2740000, 
		0x41300000, 
		0x0e7fffff, 
		0x7f7fffff, 
		0xc2240000, 
		0x007fffff, 
		0x502f14b7, 
		0x7fc00000, 
		0x00000000, 
		0xb7984070, 
		0x7f800000, 
		0xff800000, 
		0xc2200000, 
		0x41200000, 
		0x80000000, 
		0x0e000fff, 
		0x00800001, 
		0x80000000, 
		0x42300000, 
		0x0703dacc, 
		0x7fbfffff, 
		0x0e07ffff, 
		0xe6e4d386, 
		0x42040000, 
		0xbe1df2c1, 
		0x7f800000, 
		0x6ddae59a, 
		0x7f800000, 
		0x1f9d7bce, 
		0xcb6fb084, 
		0xbb422c9b, 
		0xc1f80000, 
		0x3f800000, 
		0xa46754eb, 
		0x0c7ffff0, 
		0x3af9da5f, 
		0xffe9eeeb, 
		0xb7da2ac2, 
		0x00800003, 
		0x80800003, 
		0xbf800001, 
		0x0c7fff00, 
		0x80000000, 
		0xc1d80000, 
		0x0a6d7edf, 
		0x0c700000, 
		0x004f5ad1, 
		0x41e80000, 
		0x80000000, 
		0x41000000, 
		0x77f19077, 
		0x0e0000ff, 
		0x0e000003, 
		0x80004000, 
		0x25cccd91, 
		0x5c530c13, 
		0x0e00007f, 
		0x80000000, 
		0x00800001, 
		0x0071a847, 
		0x0c7ff800, 
		0x0c600000, 
		0x42740000, 
		0x00000000, 
		0x41f80000, 
		0x00000000, 
		0x00000100, 
		0x802cf9d4, 
		0x80000001, 
		0xd685a556, 
		0x80004000, 
		0x7f800000, 
		0x7285af0c, 
		0x80000000, 
		0x41a00000, 
		0x80000000, 
		0x8f3cc8f6, 
		0xc2240000, 
		0xd34ac77b, 
		0x42540000, 
		0x00000000, 
		0x7fe36f12, 
		0x80000002, 
		0xa93832d9, 
		0x00200000, 
		0x00000000, 
		0x00000001, 
		0xf10593a6, 
		0xff800000, 
		0x0e07ffff, 
		0x1cbaa431, 
		0x80800003, 
		0xc1000000, 
		0xc2200000, 
		0x0c7fff00, 
		0x00200000, 
		0x0c7ffff0, 
		0xffbaef1d, 
		0x0c7f0000, 
		0x6050dee8, 
		0x1df73cac, 
		0xff800000, 
		0x6934dde5, 
		0x0e7fffff, 
		0xcb8c4b40, 
		0x5724e443, 
		0x42640000, 
		0x0e003fff, 
		0x7f953c98, 
		0x7fc00001, 
		0x80000020, 
		0x80000002, 
		0xc8b0f5d6, 
		0x00000000, 
		0x3f800000, 
		0x982facb7, 
		0x00000000, 
		0x80000000, 
		0xc2000000, 
		0x25a52732, 
		0x42680000, 
		0x97c8f5c0, 
		0x80000000, 
		0x0dcd2e67, 
		0x3f79c80d, 
		0x41300000, 
		0xbbb75e3d, 
		0x0c7ffff0, 
		0x42680000, 
		0x00000004, 
		0x8023ba79, 
		0x528467ae, 
		0x0e00000f, 
		0x00000010, 
		0x80000000, 
		0xc2100000, 
		0xa38a6e8e, 
		0x2fdfe74f, 
		0xc2480000, 
		0x9a21718e, 
		0x007fffff, 
		0xb0f27b9b, 
		0x80000000, 
		0xb221ffe8, 
		0x80000000, 
		0x80011111, 
		0x1aa68a0d, 
		0x80002000, 
		0xc2780000, 
		0x00800003, 
		0xc2440000, 
		0x425c0000, 
		0x41300000, 
		0xc2700000, 
		0x41e80000, 
		0x00001000, 
		0x7f800000, 
		0xc27c0000, 
		0x41a00000, 
		0xc1300000, 
		0xc1d80000, 
		0xc2700000, 
		0x80800000, 
		0x00800000, 
		0x41700000, 
		0xc1d80000, 
		0x9243385d, 
		0xad69c13e, 
		0x80000000, 
		0x41800000, 
		0x4b8c4b40, 
		0x42280000, 
		0x80000000, 
		0x00800000, 
		0x77fde232, 
		0xc2240000, 
		0x00000000, 
		0x00002000, 
		0x80000040, 
		0xc23c0000, 
		0xc1a80000, 
		0x00800003, 
		0x03b63258, 
		0x0c700000, 
		0x0e1fffff, 
		0xc0e00000, 
		0x0c7fe000, 
		0xc1e80000, 
		0xddbf681d, 
		0x3f800001, 
		0xffc3ee19, 
		0x0c7fff80, 
		0x643e2ab8, 
		0xc1d00000, 
		0x00010000, 
		0x7f8d9390, 
		0x41700000, 
		0xcccccccc, 
		0x0f7ffffc, 
		0x1d392182, 
		0x00000002, 
		0x7f7fffff, 
		0x40400000, 
		0x3174b0ce, 
		0x80000001, 
		0xc1900000, 
		0x00000000, 
		0x80000000, 
		0x7f800000, 
		0x00000001, 
		0x80000001, 
		0xc1b00000, 
		0xc1700000, 
		0x0e000001, 
		0x0e000007, 
		0x0e0000ff, 
		0x7a4e0120, 
		0x41d00000, 
		0x00000000, 
		0xc1a00000, 
		0xc1d00000, 
		0x80010000, 
		0xc0a00000, 
		0xffffffff, 
		0x0e00ffff, 
		0xc24c0000, 
		0x80800003, 
		0x00100000, 
		0x5efb2a08, 
		0xd20145d9, 
		0x56f5395b, 
		0x7f800000, 
		0xff800000, 
		0xa7c47ecf, 
		0x41a80000, 
		0x41f80000, 
		0x42640000, 
		0x42280000, 
		0x0e00000f, 
		0x80000010, 
		0x80800000, 
		0x4b000000, 
		0x7f800000, 
		0x42040000, 
		0x2293f439, 
		0x80011111, 
		0x0c600000, 
		0xd9424a1b, 
		0xc21c0000, 
		0xcccccccc, 
		0x00000001, 
		0x6527ca0f, 
		0x41a80000, 
		0xeb0ebc8d, 
		0x80000000, 
		0x41a80000, 
		0x6d916efb, 
		0x80000000, 
		0x80800002, 
		0x7f800000, 
		0x40800000, 
		0x00000400, 
		0x0e000001, 
		0x0e0000ff, 
		0x0e0003ff, 
		0x422c0000, 
		0xff9040b4, 
		0xc2480000, 
		0x11a06e6c, 
		0xacf7bdad, 
		0xc1d80000, 
		0x0c7ff800, 
		0x9c8a90e8, 
		0x00000000, 
		0x0307f055, 
		0x8bfe2928, 
		0x0c7ffffe, 
		0xff800000, 
		0x8529d325, 
		0xc26c0000, 
		0xc1c00000, 
		0x70eaa5e9, 
		0x00000000, 
		0xaaaaaaaa, 
		0x00000000, 
		0x4748ce22, 
		0x0dc4b37b, 
		0x7f800000, 
		0x02244e00, 
		0x6b3669af, 
		0x0e03ffff, 
		0x085ad224, 
		0x42100000, 
		0x80800002, 
		0x7f7fffff, 
		0x66e6d922, 
		0x00010000, 
		0x7f7ffffe, 
		0x0e0001ff, 
		0x8fe48848, 
		0xde3f42ef, 
		0x80000800, 
		0x4b8c4b40, 
		0x0f7ffffc, 
		0xe716ba2f, 
		0xbf800001, 
		0x00400000, 
		0x42380000, 
		0x80100000, 
		0x0c7e0000, 
		0x80008000, 
		0x20a51f24, 
		0x0c7c0000, 
		0x425c0000, 
		0xc20c0000, 
		0xc2280000, 
		0x41c80000, 
		0xe965607e, 
		0xc2100000, 
		0x00000000, 
		0xbf800000, 
		0xff940fd3, 
		0xc25c0000, 
		0x00800003, 
		0xab9c2136, 
		0x42040000, 
		0x42700000, 
		0x42740000, 
		0x0f7fffff, 
		0x41d00000, 
		0x8f7ffffd, 
		0xc2000000, 
		0x8e2b6f7c, 
		0x7f7fffff, 
		0xf2392d4e, 
		0xaf5388e1, 
		0x0f7ffffe, 
		0x007fffff, 
		0xff800000, 
		0xdeff2978, 
		0x80000002, 
		0x8f7fffff, 
		0x00ffffff, 
		0x80000080, 
		0x424c0000, 
		0xc1f80000, 
		0x80200000, 
		0x42400000, 
		0x00000400, 
		0x00000400, 
		0x00000000, 
		0x00040000, 
		0x00000000, 
		0xa9a3c49b, 
		0x7fc00001, 
		0xc2700000, 
		0x0c7fe000, 
		0x41980000, 
		0xc0800000, 
		0x7f7ffffe, 
		0x4aab1e31, 
		0x80001000, 
		0xdf6b6da5, 
		0x42540000, 
		0xc2640000, 
		0x00020000, 
		0x4ee677dd, 
		0xff7ffffe, 
		0xc1a00000, 
		0xee4c895b, 
		0x80000000, 
		0x40e00000, 
		0x0c7c0000, 
		0x00000080, 
		0x426c0000, 
		0x80800001, 
		0x80000004, 
		0x80800002, 
		0xff800000, 
		0xc26c0000, 
		0xc25c0000, 
		0xc0400000, 
		0x80000001, 
		0x8631fb50, 
		0x00000002, 
		0x427c0000, 
		0xffb002e9, 
		0x2ef96e4b, 
		0x7ff21073, 
		0xab6575b3, 
		0x0c700000, 
		0x00000002, 
		0xfd20eb86, 
		0xc2480000, 
		0xfdaab474, 
		0x0c7fffe0, 
		0x42200000, 
		0x80002000, 
		0x00800002, 
		0xff800000, 
		0x42580000, 
		0x41880000, 
		0x0c7f8000, 
		0xbc00f921, 
		0xff81f0c7, 
		0xebe2ec01, 
		0x0c7e0000, 
		0x41400000, 
		0x0c780000, 
		0x0f221ed8, 
		0x4a8716f5, 
		0x7fa99cfe, 
		0xc1800000, 
		0xdeaab2af, 
		0x7cb6fbb6, 
		0x8f7fffff, 
		0x7fbe55d5, 
		0x050c8f71, 
		0x0f7ffffd, 
		0x74dc92b1, 
		0x42580000, 
		0x0c7c0000, 
		0x41a80000, 
		0x80000000, 
		0xb1712ac3, 
		0x23711f2b, 
		0xc1300000, 
		0x42740000, 
		0xcd79fa55, 
		0xaa8f6b7d, 
		0x007fffff, 
		0x80000400, 
		0xc1800000, 
		0x00800000, 
		0x80000008, 
		0xc1800000, 
		0xc27c0000, 
		0x0e000fff, 
		0x41400000, 
		0x80800000, 
		0x80800001, 
		0xff7ffffe, 
		0x8f7ffffc, 
		0x0787892c, 
		0x80002000, 
		0xb6e32f87, 
		0x40800000, 
		0x7fc00001, 
		0xc2300000, 
		0xccb01bcc, 
		0x7f800000, 
		0x00800003, 
		0x00000000, 
		0xa2a42459, 
		0x42140000, 
		0x0d00fff0, 
		0xa3aa210a, 
		0xb8097aee, 
		0x0e00001f, 
		0x7f000000, 
		0xc2200000, 
		0x00000000, 
		0x00400000, 
		0x42340000, 
		0x7f908b80, 
		0xcb8c4b40, 
		0x3cbdbc1c, 
		0x0c7ffff0, 
		0xc1500000, 
		0x54419b96, 
		0x0e00007f, 
		0xb8f038ba, 
		0x0c7ff000, 
		0x00800002, 
		0x40000000, 
		0x54445319, 
		0x42180000, 
		0x80000200, 
		0x42540000, 
		0x12b22987, 
		0x806eb682, 
		0x931768a5, 
		0x052881d7, 
		0x00000000, 
		0x33333333, 
		0x84d9b9f7, 
		0x9b4c4104, 
		0x42700000, 
		0xfe85445b, 
		0x6e3a1f1b, 
		0xff800000, 
		0x6f17f3cf, 
		0x8d4875c6, 
		0x80800000, 
		0x75ca4cb0, 
		0x00800002, 
		0x02929380, 
		0xc1e80000, 
		0xd8a0a0c0, 
		0x00770595, 
		0x7f800000, 
		0x1a0eadbb, 
		0x0c400000, 
		0x42200000, 
		0x80000200, 
		0x42640000, 
		0x80000002, 
		0x00000000, 
		0x42780000, 
		0x00011111, 
		0x7fe24330, 
		0xb70a940d, 
		0x00000020, 
		0x6f46b343, 
		0x55555555, 
		0x4cd84812, 
		0x00000800, 
		0x2696963e, 
		0x0e0003ff, 
		0x2da2223b, 
		0x151195eb, 
		0x80800002, 
		0x40800000, 
		0x3f800000, 
		0x417d5020, 
		0x82b437e0, 
		0x80011111, 
		0x91603297, 
		0x00011111, 
		0x9fbdb4d8, 
		0x5311db0f, 
		0xb8ecffda, 
		0x00000000, 
		0xbcdf2b7a, 
		0x00100000, 
		0xc1900000, 
		0x007ffffe, 
		0xff80c3b0, 
		0x0c7ffff8, 
		0x80004000, 
		0x00000100, 
		0x41000000, 
		0x8fa40e82, 
		0xa5860d7d, 
		0x80000000, 
		0x0c7e0000, 
		0x421c0000, 
		0xb1f39afc, 
		0x00000000, 
		0x37aebc44, 
		0x7f7ffffe, 
		0x00000002, 
		0x119e6d6c, 
		0x00000008, 
		0x91081ade, 
		0x0c600000, 
		0x0e001fff, 
		0xc2080000, 
		0x0c7f0000, 
		0xa797a201, 
		0x0e7fffff, 
		0x0c7ffff0, 
		0x80000000, 
		0xfeb29e77, 
		0xeb0e21a0, 
		0xdd2a0ae9, 
		0xaaaaaaaa, 
		0x80000000, 
		0xf739267d, 
		0x0c7ff000, 
		0x4a6ca9fc, 
		0x80800003, 
		0x8809fb7f, 
		0x40730526, 
		0x00010000, 
		0x52408a63, 
		0xbf800001, 
		0x58cc6eb3, 
		0xc1200000, 
		0x423c0000, 
		0x00000800, 
		0x427c0000, 
		0x8657ee97, 
		0x0c7c0000, 
		0x27f6f6c2, 
		0x8e49774e, 
		0x00000000, 
		0x42340000, 
		0x3f028f5c, 
		0x0e00007f, 
		0x80800002, 
		0x42080000, 
		0x41000000, 
		0x5d349bc1, 
		0x7f800000, 
		0x0e007fff, 
		0xc2000000, 
		0x45d53f0d, 
		0x280bda89, 
		0x424c0000, 
		0x0e0fffff, 
		0x80000000, 
		0xc1f80000, 
		0xc2080000, 
		0x42700000, 
		0x777f027c, 
		0x40800000, 
		0x00000001, 
		0xa978a9da, 
		0x00000800, 
		0x0e00001f, 
		0x42580000, 
		0x42700000, 
		0x42740000, 
		0x7f800000, 
		0x193e4f6c, 
		0x3f800001, 
		0xf853b5ff, 
		0xff000000, 
		0x41700000, 
		0x4b8c4b40, 
		0x0c780000, 
		0x09dbbd21, 
		0x0c7ffffc, 
		0xcb8c4b40, 
		0x00000001, 
		0xaaaaaaaa, 
		0x00800001, 
		0x80000000, 
		0x0e07ffff, 
		0x0df52dbf, 
		0x41c00000, 
		0x7f7ffffe, 
		0xff7ffffe, 
		0x80080000, 
		0x863310cf, 
		0x80800003, 
		0x874a9b29, 
		0x80000000, 
		0xe9d4b94e, 
		0xff800000, 
		0x00000004, 
		0x80000800, 
		0xc22ea838, 
		0x00000000, 
		0xc1100000, 
		0xff874275, 
		0xc0e00000, 
		0xc0400000, 
		0x42680000, 
		0xc1c00000, 
		0xb03bdacb, 
		0x0c7fff00, 
		0x42100000, 
		0x00000004, 
		0x0c7fffe0, 
		0x00020000, 
		0xdb74659f, 
		0x41e80000, 
		0x40e00000, 
		0xd15771f0, 
		0xdf96538f, 
		0xbfd7bd59, 
		0x0e000fff, 
		0x3e65ccc3, 
		0xc1b00000, 
		0x2b9e8d43, 
		0x4e4bd021, 
		0x80800002, 
		0x7f800000, 
		0xc1700000, 
		0xc1800000, 
		0x00400000, 
		0x41700000, 
		0x42580000, 
		0x81c4d0a3, 
		0x7f800001, 
		0xff7ffffe, 
		0x42ab6eaf, 
		0xfb3b0d99, 
		0xff800000, 
		0x41100000, 
		0x80000800, 
		0xe83e257b, 
		0x20402b27, 
		0x80008000, 
		0x7ff3563a, 
		0x00000020, 
		0x0d00fff0, 
		0x0f7ffffc, 
		0x41100000, 
		0x41e80000, 
		0x42700000, 
		0x0e000001, 
		0xec6bf064, 
		0xc2080000, 
		0x866a3321, 
		0x41d00000, 
		0xbc0581df, 
		0x80000000, 
		0x80800002, 
		0x0c0ffbad, 
		0xb1a1e0fa, 
		0x41200000, 
		0x8ffce77c, 
		0x80000020, 
		0x00400000, 
		0xff7fffff, 
		0xc0000000, 
		0x00800002, 
		0x80000020, 
		0x0f7ffffd, 
		0x00000020, 
		0x00020000, 
		0xc1f00000, 
		0xff000000, 
		0x84c875c0, 
		0x00000080, 
		0x422c0000, 
		0x80000010, 
		0x80000000, 
		0x00010000, 
		0x7f800000, 
		0x8f7ffffd, 
		0x0c700000, 
		0xff000000, 
		0x42440000, 
		0x2ddb31b0, 
		0xc1500000, 
		0xff7fffff, 
		0xe202abfe, 
		0x00000000, 
		0x813517e7, 
		0xff7fffff, 
		0x00200000, 
		0xc1e80000, 
		0x0d000ff0, 
		0x0c7fffc0, 
		0x7fc00001, 
		0x9ccfcaa2, 
		0x00000004, 
		0x0e7fffff, 
		0xaf174c1c, 
		0x805d2284, 
		0x00000000, 
		0x647c0a2f, 
		0x7f800000, 
		0xbf028f5c, 
		0x00000000, 
		0xc2100000, 
		0x1aa13ade, 
		0x005393d9, 
		0x209ea7e8, 
		0x3f800000, 
		0x80011111, 
		0x7f800000, 
		0x7f8e1515, 
		0x007fffff, 
		0x9dd87ae4, 
		0xceae1a6c, 
		0x00800003, 
		0xc1880000, 
		0x42780000, 
		0x00800001, 
		0x0c7ffffc, 
		0x00000000, 
		0x46e7ced9, 
		0x93a21c81, 
		0x80000001, 
		0x8f7ffffe, 
		0xc24c0000, 
		0xc1f80000, 
		0x0e01ffff, 
		0x80800001, 
		0xc1980000, 
		0xc1500000, 
		0x42180000, 
		0x80000020, 
		0x0e0001ff, 
		0x8467594c, 
		0xc1700000, 
		0x0d000ff0, 
		0x40000000, 
		0x80800001, 
		0x41d80000, 
		0xff7fffff, 
		0x0f7fffff, 
		0x80000000, 
		0x80002000, 
		0xc2180000, 
		0x00008000, 
		0xc1b00000, 
		0xc1600000, 
		0xdedde019, 
		0x4c5aa14b, 
		0x42680000, 
		0x80000000, 
		0x80000000, 
		0x80000200, 
		0xc2000000, 
		0xff800001, 
		0x00800001, 
		0x745a1b69, 
		0x00000000, 
		0x42000000, 
		0x42040000, 
		0x80080000, 
		0x1997b8f8, 
		0xbf800001, 
		0xc7cc85c3, 
		0xc2240000, 
		0xc2700000, 
		0x7f800000, 
		0xaaaaaaaa, 
		0xc1f80000, 
		0x2d0d3b1d, 
		0x7fc00001, 
		0xff800000, 
		0x0c7ffffc, 
		0x0c7fffe0, 
		0xff800000, 
		0xffc00000, 
		0x80000001, 
		0x80ffffff, 
		0x35714b02, 
		0x0c780000, 
		0x42340000, 
		0x00800002, 
		0xff000000, 
		0x7f7fffff, 
		0x42140000, 
		0x80800000, 
		0xffa4e947, 
		0xcb8c4b40, 
		0x41f00000, 
		0xc2340000, 
		0x7af523ef, 
		0xffc00001, 
		0x42600000, 
		0x80000100, 
		0x40000000, 
		0x6d658fdd, 
		0x00000000, 
		0x0c7fffff, 
		0x8f7ffffe, 
		0x0f7ffffd, 
		0x00000400, 
		0x00000001, 
		0x7f800000, 
		0x0f7ffffe, 
		0x00000001, 
		0x80200000, 
		0x807fffff, 
		0x42680000, 
		0x00800001, 
		0x208cdf81, 
		0x42700000, 
		0x0f81881d, 
		0x1bf9abd5, 
		0x9b83222f, 
		0x0c7c0000, 
		0xb6feb1af, 
		0x0e00001f, 
		0xa574dd6b, 
		0x80800001, 
		0x807fffff, 
		0x3c1bc8f3, 
		0xc1d00000, 
		0xcccccccc, 
		0xdfe75eed, 
		0x41a80000, 
		0x00011111, 
		0x41900000, 
		0xcccccccc, 
		0x30a08a03, 
		0x00010000, 
		0xc7c75159, 
		0x424c0000, 
		0x689fbf8c, 
		0x60de057a, 
		0x40c00000, 
		0xc24c0000, 
		0x807fffff, 
		0xc1d80000, 
		0x1e9ecf69, 
		0x0e000001, 
		0x41200000, 
		0x00000000, 
		0x40000000, 
		0x0e01ffff, 
		0xc20c0000, 
		0xe28ac062, 
		0x0c7ffffc, 
		0x427c0000, 
		0xff7ffffe, 
		0x0e00000f, 
		0x42040000, 
		0x218fc5d4, 
		0x6b6b810f, 
		0x01a749d8, 
		0x86a541da, 
		0x80000002, 
		0x26d13c86, 
		0x80200000, 
		0x00000000, 
		0x0f7ffffc, 
		0x00020000, 
		0xc9fb1a77, 
		0x4130f737, 
		0xc0a00000, 
		0xff7ffffe, 
		0x59b3d8d1, 
		0x0e07ffff, 
		0xfef7d545, 
		0xff7ffffe, 
		0xc1a80000, 
		0x42440000, 
		0xc2180000, 
		0x0c7fffff, 
		0xc2100000, 
		0xc1d80000, 
		0x33333333, 
		0x084d3f30, 
		0x0e000007, 
		0x80000000, 
		0x80000000, 
		0xff800000, 
		0xc1880000, 
		0x40400000, 
		0xc2740000, 
		0x80800000, 
		0x41800000, 
		0x263a8782, 
		0x806cf818, 
		0x00000000, 
		0xff7fffff, 
		0xc1f80000, 
		0xcccccccc, 
		0x80002000, 
		0xe50019af, 
		0x0c7ffe00, 
		0x4b000000, 
		0x42400000, 
		0xc1900000, 
		0x73085743, 
		0xc2480000, 
		0x80020000, 
		0x40400000, 
		0x8ac11f7f, 
		0x4636eae6, 
		0x7d859fb6, 
		0xff7ffffe, 
		0x7fb666b1, 
		0x804ea2c1, 
		0x80000000, 
		0x2e7f713b, 
		0x00010000, 
		0x00000000, 
		0x6a599846, 
		0xc0e00000, 
		0x40a00000, 
		0x0f7fffff, 
		0x004c7dc4, 
		0x80000000, 
		0x7f800000, 
		0x8f7ffffd, 
		0x80000008, 
		0x00800001, 
		0x00800003, 
		0xc03d0b42, 
		0x8f7ffffd, 
		0xe38d4d41, 
		0x00000020, 
		0xc2500000, 
		0x80000000, 
		0x00800000, 
		0x508625b6, 
		0xff800000, 
		0x80000100, 
		0x7f7ffffe, 
		0x00000008, 
		0x7fa35ef5, 
		0x41800000, 
		0x9e602f04, 
		0x0c7fffc0, 
		0x80000080, 
		0x80000000, 
		0x89b40411, 
		0x00800002, 
		0x41b80000, 
		0x26766eea, 
		0x41800000, 
		0x17734161, 
		0x0c700000, 
		0xc2240000, 
		0xa4de815d, 
		0xd9937a1b, 
		0x83757e4f, 
		0x94d38bd1, 
		0x007fffff, 
		0x9deec22a, 
		0xff800000, 
		0x80011111, 
		0x7aec4baf, 
		0x3a1b710e, 
		0x44d8cc67, 
		0x0f7ffffd, 
		0x00000000, 
		0x00000000, 
		0x0e1fffff, 
		0x80800002, 
		0xaa91f248, 
		0x6b429d1b, 
		0x0e0000ff, 
		0x41d00000, 
		0x80000004, 
		0x42400000, 
		0x41200000, 
		0x3c0f3e98, 
		0x80000000, 
		0x00001000, 
		0xbd816812, 
		0xa5ea31b7, 
		0x7f7fffff, 
		0xc1200000, 
		0x00100000, 
		0x00800002, 
		0xc1980000, 
		0xc2400000, 
		0xb2127f22, 
		0x3edffcc5, 
		0x42100000, 
		0x77191e9a, 
		0x3f800001, 
		0xac12e8de, 
		0xa152c066, 
		0xc0b1713a, 
		0x1baa5cd0, 
		0x25bed865, 
		0x421c0000, 
		0x00000080, 
		0x0c7fc000, 
		0x80000002, 
		0x80000000, 
		0x80000000, 
		0x0e0000ff, 
		0xcaecd9cf, 
		0x7f800000, 
		0x8c9cf4e1, 
		0x41d80000, 
		0x4b8c4b40, 
		0x00000000, 
		0x80000008, 
		0x80008000, 
		0x80000040, 
		0x7f800000, 
		0xab047b3e, 
		0x55555555, 
		0x7fa39c57, 
		0x80800000, 
		0x41e00000, 
		0xffbabe25, 
		0xc2740000, 
		0xc0000000, 
		0x0c400000, 
		0x807ffffe, 
		0x42600000, 
		0x80000001, 
		0xb246af3f, 
		0xf3ca4f4f, 
		0xcccccccc, 
		0x00040000, 
		0x9768b03d, 
		0x20117e44, 
		0x32b1144a, 
		0x007ffffe, 
		0x0c600000, 
		0x807ffffe, 
		0x0c7f8000, 
		0x42080000, 
		0x00000001, 
		0xc1f00000, 
		0xc2040000, 
		0x40800000, 
		0x737bb7d7, 
		0xc2180000, 
		0xffe68539, 
		0xff7ffffe, 
		0x4b748b3e, 
		0x00000000, 
		0xff7ffffe, 
		0xf551e566, 
		0xc26c0000, 
		0xbf028f5c, 
		0xf331e270, 
		0xc1200000, 
		0xc24c0000, 
		0x00000040, 
		0x9f1aad7a, 
		0xc1000000, 
		0x64a24a72, 
		0xc2600000, 
		0x80040000, 
		0xc2340000, 
		0xdc0d301d, 
		0x0e03ffff, 
		0x42580000, 
		0x0e0fffff, 
		0x6512c668, 
		0x42400000, 
		0x6d98fb3b, 
		0xc0e00000, 
		0x5eab276a, 
		0x41a00000, 
		0x421c0000, 
		0x41a80000, 
		0x2f1f009c, 
		0x425c0000, 
		0xc2280000, 
		0xb08ace46, 
		0x0c400000, 
		0xd5bdee3c, 
		0xc1d80000, 
		0x82259eb6, 
		0x36f2b3c9, 
		0x9ff4a2f5, 
		0xc2600000, 
		0x80011111, 
		0x80626d08, 
		0x0f7ffffc, 
		0x0f7ffffc, 
		0xbf028f5c, 
		0x41900000, 
		0xff000000, 
		0x58bed7c5, 
		0x41700000, 
		0xc1b80000, 
		0x80000004, 
		0x42700000, 
		0x7bf941c3, 
		0x424c0000, 
		0x8246372d, 
		0xc2180000, 
		0xc2540000, 
		0x80100000, 
		0x00800000, 
		0x7f800000, 
		0x0060d1b7, 
		0x80000400, 
		0xc1600000, 
		0x42080000, 
		0xc0800000, 
		0x1cd03f94, 
		0xb20260e1, 
		0xc2540000, 
		0x42380000, 
		0xc1d80000, 
		0x007fffff, 
		0x00010000, 
		0xdee2e65a, 
		0xc1a00000, 
		0x42500000, 
		0x0c600000, 
		0xff800000, 
		0x80800003, 
		0x7f800000, 
		0xc2fe2707, 
		0x41500000, 
		0x0e007fff, 
		0x42340000, 
		0x0c7e0000, 
		0xc1400000, 
		0x00001000, 
		0x7f800000, 
		0x5742669d, 
		0xc2040000, 
		0x0c7fff80, 
		0x7fc00000, 
		0x99cc93e9, 
		0xc2500000, 
		0x80000000, 
		0x8ec56b31, 
		0x0e007fff, 
		0x80200000, 
		0xc1980000, 
		0x80800001, 
		0x00000002, 
		0xc2700000, 
		0x00000000, 
		0x00000000, 
		0xff7fffff, 
		0x80000000, 
		0xbf1fc9c8, 
		0x41d80000, 
		0x0c7ffff0, 
		0x49735a82, 
		0xc1980000, 
		0x3ace79b1, 
		0x4b8c4b40, 
		0x00800001, 
		0x806269c0, 
		0x4579a5c8, 
		0x00000020, 
		0x8f7fffff, 
		0x31d8ef4a, 
		0x0e001fff, 
		0x80000020, 
		0xa4d303ae, 
		0x7f7ffffe, 
		0xd8d8fc61, 
		0x42680000, 
		0x8f7fffff, 
		0x3f800001, 
		0xbf800001, 
		0x40c00000, 
		0xc2680000, 
		0x41500000, 
		0xe39f1696, 
		0x41300000, 
		0xc1500000, 
		0x0e007fff, 
		0x0e00000f, 
		0xff87a5e6, 
		0x7381d1ab, 
		0xc1c80000, 
		0x7fbfb9b2, 
		0x0c7ffff0, 
		0xfe150d67, 
		0x6dfdc120, 
		0x0c7ffffc, 
		0x33333333, 
		0x80020000, 
		0x426c0000, 
		0x42180000, 
		0xc1500000, 
		0x48421eeb, 
		0xbc0a6d17, 
		0x42680000, 
		0xc1980000, 
		0x80000000, 
		0x0e7fffff, 
		0x80000200, 
		0xc0c00000, 
		0x2c4e7b5f, 
		0x00002000, 
		0x42600000, 
		0x0c7e0000, 
		0xae89a9bc, 
		0xff800000, 
		0x7f800000, 
		0xff7fffff, 
		0x80020000, 
		0x7fbfffff, 
		0xff800000, 
		0xfa828c12, 
		0x80000000, 
		0x3799e64c, 
		0x80000000, 
		0xf7782dca, 
		0xcfe63439, 
		0x00000000, 
		0xf4822d1f, 
		0xc1100000, 
		0xd0997c42, 
		0xc1800000, 
		0xed4617f0, 
		0xc1900000, 
		0x00ffffff, 
		0xc1e80000, 
		0x80000000, 
		0x41200000, 
		0x7f800000, 
		0xc2680000, 
		0x420c0000, 
		0x00000000, 
		0xb11d8af3, 
		0xc2540000, 
		0x41c00000, 
		0x087cdae5, 
		0x41600000, 
		0xc2440000, 
		0x0e000001, 
		0xaaaaaaaa, 
		0x80010000, 
		0xc1d00000, 
		0xcbfe6a56, 
		0x3a4854b8, 
		0x00800001, 
		0x0c7ffff8, 
		0x42380000, 
		0x0c7fff00, 
		0x522c72af, 
		0x0c7ffffe, 
		0xffdf94cd, 
		0x00000000, 
		0xf1c49ce4, 
		0x2c3ea12b, 
		0x0e000003, 
		0x41000000, 
		0xc1880000, 
		0x80000001, 
		0x41900000, 
		0x7f7fffff, 
		0xf9abd354, 
		0x0e0fffff, 
		0xb60eb5f2, 
		0x0c780000, 
		0xc23c0000, 
		0x7f7fffff, 
		0xa59023e9, 
		0xc2300000, 
		0xc1300000, 
		0xba1f56f7, 
		0x0c7f0000, 
		0x516bddbb, 
		0x423c0000, 
		0xec362b3c, 
		0x7fffffff, 
		0x0e0001ff, 
		0x00800001, 
		0xe03212b0, 
		0xc2240000, 
		0x41100000, 
		0x00000000, 
		0xf6c70dea, 
		0x7f800000, 
		0x00200000, 
		0x00000000, 
		0xc2180000, 
		0xc23c0000, 
		0x7f800000, 
		0x232f5edf, 
		0x00000002, 
		0x80000100, 
		0x3f800000, 
		0xc0400000, 
		0x0f7ffffc, 
		0x00295ea4, 
		0x41800000, 
		0xc2080000, 
		0xa2841600, 
		0x00000000, 
		0xc2100000  
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
		0x0000037d, 
		0x000001dd, 
		0x00000337, 
		0x000000bf, 
		0x000000f1, 
		0x0000003b, 
		0x000003d7, 
		0x00000141, 
		0x00000175, 
		0x00000183, 
		0x00000083, 
		0x00000015, 
		0x00000175, 
		0x0000028f, 
		0x00000349, 
		0x000003c3, 
		0x00000395, 
		0x00000329, 
		0x00000161, 
		0x000000d1, 
		0x000001d3, 
		0x000002dd, 
		0x00000359, 
		0x000002a1, 
		0x00000181, 
		0x0000010f, 
		0x000000ff, 
		0x0000017d, 
		0x000000cf, 
		0x00000193, 
		0x00000129, 
		0x00000307, 
		0x00000081, 
		0x000000a5, 
		0x000000b9, 
		0x0000036b, 
		0x0000029b, 
		0x00000163, 
		0x00000049, 
		0x000000b7, 
		0x000001cb, 
		0x0000005b, 
		0x00000187, 
		0x000002a3, 
		0x0000039b, 
		0x0000021f, 
		0x00000155, 
		0x00000051, 
		0x00000337, 
		0x00000195, 
		0x000000d9, 
		0x0000008d, 
		0x000000d5, 
		0x000000b3, 
		0x000003ef, 
		0x00000175, 
		0x000003c9, 
		0x000000fb, 
		0x00000081, 
		0x0000026f, 
		0x000003a5, 
		0x00000293, 
		0x0000020b, 
		0x0000001b, 
		0x000000d1, 
		0x000001b7, 
		0x00000191, 
		0x000002fd, 
		0x0000008d, 
		0x000002ef, 
		0x000001e9, 
		0x0000030f, 
		0x0000010f, 
		0x000002cd, 
		0x000001f9, 
		0x00000099, 
		0x00000061, 
		0x000001b9, 
		0x0000020f, 
		0x00000051, 
		0x00000137, 
		0x000003ef, 
		0x0000025d, 
		0x000000ff, 
		0x00000261, 
		0x00000103, 
		0x0000026f, 
		0x00000157, 
		0x000003ef, 
		0x000002c1, 
		0x00000259, 
		0x00000261, 
		0x000001e9, 
		0x000000f9, 
		0x00000343, 
		0x000003ed, 
		0x000002e1, 
		0x00000373, 
		0x0000001b, 
		0x0000008b, 
		0x0000034d, 
		0x00000285, 
		0x000000c5, 
		0x0000013b, 
		0x0000029f, 
		0x00000355, 
		0x00000149, 
		0x00000199, 
		0x000001cd, 
		0x00000361, 
		0x00000117, 
		0x000003f3, 
		0x000001a9, 
		0x00000399, 
		0x000002ab, 
		0x0000038b, 
		0x000002b5, 
		0x00000115, 
		0x000002d9, 
		0x000000c9, 
		0x000000b1, 
		0x00000127, 
		0x000001a9, 
		0x000003c9, 
		0x000003b3, 
		0x000000a9, 
		0x0000031d, 
		0x00000363, 
		0x000001cb, 
		0x00000261, 
		0x0000035d, 
		0x00000359, 
		0x0000020d, 
		0x0000001d, 
		0x000001ef, 
		0x00000353, 
		0x000002bf, 
		0x000003c1, 
		0x00000149, 
		0x00000299, 
		0x000002cd, 
		0x000000af, 
		0x00000253, 
		0x000000c1, 
		0x00000265, 
		0x00000323, 
		0x00000175, 
		0x000000c9, 
		0x0000012f, 
		0x00000299, 
		0x00000319, 
		0x000001df, 
		0x000000e9, 
		0x00000379, 
		0x00000023, 
		0x000002a7, 
		0x0000025d, 
		0x00000375, 
		0x000002b3, 
		0x000000b5, 
		0x0000018b, 
		0x00000389, 
		0x000003f3, 
		0x000000d9, 
		0x00000201, 
		0x000003c7, 
		0x0000004f, 
		0x00000117, 
		0x0000017b, 
		0x0000005b, 
		0x000001eb, 
		0x0000001d, 
		0x000001af, 
		0x000003d3, 
		0x000000d1, 
		0x0000033d, 
		0x000000fb, 
		0x000002af, 
		0x000001ed, 
		0x000001a3, 
		0x00000271, 
		0x000003ef, 
		0x0000031d, 
		0x0000017b, 
		0x000000cb, 
		0x000002dd, 
		0x0000030f, 
		0x0000026d, 
		0x000003ef, 
		0x00000365, 
		0x000001c5, 
		0x0000004d, 
		0x0000009f, 
		0x00000377, 
		0x00000331, 
		0x00000077, 
		0x00000273, 
		0x000001ad, 
		0x0000011d, 
		0x000002c3, 
		0x0000028f, 
		0x0000025b, 
		0x00000299, 
		0x0000030f, 
		0x0000033f, 
		0x00000297, 
		0x000000a3, 
		0x0000035d, 
		0x000003d7, 
		0x0000014b, 
		0x00000365, 
		0x00000183, 
		0x00000377, 
		0x0000012d, 
		0x000001fd, 
		0x00000301, 
		0x00000027, 
		0x00000215, 
		0x000003b7, 
		0x000000a5, 
		0x0000000d, 
		0x0000014d, 
		0x000000cf, 
		0x00000293, 
		0x0000000b, 
		0x000002b5, 
		0x00000041, 
		0x000003ef, 
		0x0000016d, 
		0x000001c3, 
		0x000002fb, 
		0x00000299, 
		0x000002bd, 
		0x00000029, 
		0x0000006d, 
		0x0000037b, 
		0x0000029f, 
		0x0000029f, 
		0x0000035f, 
		0x0000006d, 
		0x00000057, 
		0x0000006d, 
		0x000003c3, 
		0x00000043, 
		0x0000002f, 
		0x0000003f, 
		0x000002f3, 
		0x000003af, 
		0x00000373, 
		0x000001ad, 
		0x000002e5, 
		0x000000f1, 
		0x0000035d, 
		0x0000001d, 
		0x0000037b, 
		0x00000071  
};

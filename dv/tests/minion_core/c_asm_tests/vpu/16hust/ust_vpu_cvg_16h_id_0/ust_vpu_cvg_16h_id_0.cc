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
			"li x5, 172\n"
			"faddi.pi f2, f0, -471\n"
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
			"flog.ps f19, f3\n"
			"faddi.pi f3, f3, 128\n"
			"1:\n"
			"flog.ps f20, f4\n"
			"faddi.pi f4, f4, 128\n"
			"bne x0, x0, 1b\n"
			"flog.ps f21, f5\n"
			"faddi.pi f5, f5, 128\n"
			"3:\n"
			"flog.ps f22, f6\n"
			"faddi.pi f6, f6, 128\n"
			"bne x0, x0, 3b\n"
			"4:\n"
			"flog.ps f23, f7\n"
			"faddi.pi f7, f7, 128\n"
			"bne x0, x0, 4b\n"
			"fnmadd.ps f30, f31, f30, f30\n"
			"fnmsub.ps f30, f27, f24, f23\n"
			"fnmsub.ps f28, f28, f24, f27\n"
			"fmadd.ps f23, f31, f24, f30\n"
			"1:\n"
			"flog.ps f24, f8\n"
			"faddi.pi f8, f8, 128\n"
			"bne x0, x0, 1b\n"
			"2:\n"
			"flog.ps f25, f9\n"
			"faddi.pi f9, f9, 128\n"
			"bne x0, x0, 2b\n"
			"fmadd.ps f26, f26, f31, f30\n"
			"fmsub.ps f29, f26, f26, f25\n"
			"fnmadd.ps f27, f29, f29, f30\n"
			"fmadd.ps f25, f26, f29, f28\n"
			"fmadd.ps f25, f27, f26, f30\n"
			"fmadd.ps f25, f29, f31, f25\n"
			"3:\n"
			"flog.ps f26, f10\n"
			"faddi.pi f10, f10, 128\n"
			"bne x0, x0, 3b\n"
			"flog.ps f27, f11\n"
			"faddi.pi f11, f11, 128\n"
			"flog.ps f28, f12\n"
			"faddi.pi f12, f12, 128\n"
			"fnmadd.ps f28, f30, f29, f29\n"
			"fnmsub.ps f30, f31, f30, f31\n"
			"fmadd.ps f30, f31, f30, f28\n"
			"fnmadd.ps f28, f28, f29, f29\n"
			"fnmsub.ps f31, f28, f28, f30\n"
			"fmadd.ps f29, f29, f28, f31\n"
			"fmsub.ps f29, f29, f29, f31\n"
			"fnmsub.ps f30, f30, f28, f29\n"
			"flog.ps f29, f13\n"
			"faddi.pi f13, f13, 128\n"
			"fmin.pi f29, f31, f31\n"
			"fmax.ps f31, f31, f31\n"
			"fmax.pi f31, f30, f29\n"
			"fadd.pi f31, f29, f31\n"
			"fmin.pi f30, f29, f30\n"
			"fsub.pi f29, f31, f31\n"
			"fmin.pi f31, f30, f29\n"
			"fmax.pi f29, f29, f29\n"
			"fmin.ps f31, f29, f30\n"
			"fmul.pi f30, f31, f30\n"
			"flog.ps f30, f14\n"
			"faddi.pi f14, f14, 128\n"
			"fmax.ps f30, f31, f30\n"
			"fmax.ps f31, f31, f30\n"
			"fadd.ps f31, f30, f30\n"
			"fmin.pi f30, f30, f31\n"
			"fmax.pi f31, f30, f30\n"
			"fadd.pi f30, f31, f30\n"
			"fadd.pi f30, f31, f31\n"
			"fsub.ps f30, f30, f31\n"
			"flog.ps f31, f15\n"
			"faddi.pi f15, f15, 128\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fnmsub.ps f31, f31, f31, f31\n"
			"fnmadd.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
			"fmsub.ps f31, f31, f31, f31\n"
			"fmadd.ps f31, f31, f31, f31\n"
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
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
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
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
			"li x0, 0x0\n" 
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
		c_sync_point(shire_id, sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 64(x5)\n"
			"fclass.s x12, f10\n" 
			"li x5, 0x24b5\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f11, 480(x12)\n"
			"fpackreph.pi f4, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 192(x27)\n"
			"flw.ps f5, 800(x27)\n"
			"fsgnj.s f4, f8, f5\n" 
			"fbci.pi f11, 0xc5c7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 832(x6)\n"
			"fcvt.wu.s x5, f4, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 800(x14)\n"
			"fcvt.l.s x17, f4, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x278a93b888\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 928(x17)\n"
			"flw.ps f4, 0(x17)\n"
			"fnmsub.ps f17, f24, f4, f4, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 384(x12)\n"
			"fcvt.pwu.ps f8, f16, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"feqm.ps m2, f10, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 736(x26)\n"
			"flw.ps f24, 512(x26)\n"
			"fmul.s f17, f18, f24, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 608(x18)\n"
			"flw.ps f9, 288(x18)\n"
			"flt.pi f10, f7, f9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f18, 576(x27)\n"
			"flw.ps f26, 416(x27)\n"
			"fmaxu.pi f5, f18, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 128(x18)\n"
			"flw.ps f5, 288(x18)\n"
			"flw.ps f24, 800(x18)\n"
			"fcmov.ps f30, f12, f5, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 544(x5)\n"
			"flw.ps f16, 352(x5)\n"
			"fsub.ps f17, f31, f16, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f18, 736(x27)\n"
			"fpackreph.pi f26, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x4 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x0 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f11, 928(x5)\n"
			"faddi.pi f31, f11, 0x1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 672(x5)\n"
			"flw.ps f11, 384(x5)\n"
			"fle.ps f4, f17, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 448(x14)\n"
			"flw.ps f7, 96(x14)\n"
			"fnmsub.ps f17, f24, f24, f7, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 128(x14)\n"
			"flw.ps f5, 96(x14)\n"
			"feq.s x18, f12, f5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 96(x5)\n"
			"flw.ps f24, 288(x5)\n"
			"fadd.pi f18, f5, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 256(x26)\n"
			"ffrc.ps f16, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 128(x10)\n"
			"fslli.pi f24, f30, 0x6\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 480(x5)\n"
			"faddi.pi f8, f26, 0x90\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 448(x10)\n"
			"fsrai.pi f11, f31, 0x3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"fltm.pi m0, f26, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 704(x26)\n"
			"flw.ps f29, 352(x26)\n"
			"fadd.ps f8, f31, f29, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 832(x17)\n"
			"flw.ps f29, 480(x17)\n"
			"fand.pi f11, f26, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 288(x5)\n"
			"flw.ps f24, 896(x5)\n"
			"fminu.pi f17, f12, f24\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_0:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 608(x14)\n"
			"frcp.ps f4, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 640(x22)\n"
			"flw.ps f26, 224(x22)\n"
			"fnmsub.s f12, f7, f26, f7, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_309054_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_27108_HID_0:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x5 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x6 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_627429_HID_0:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xa << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x4 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f30, 736(x26)\n"
			"flw.ps f7, 0(x26)\n"
			"fmaxu.pi f18, f30, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 256(x17)\n"
			"flw.ps f8, 800(x17)\n"
			"feq.s x26, f26, f8\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 512(x27)\n"
			"flw.ps f7, 992(x27)\n"
			"flw.ps f11, 224(x27)\n"
			"fmsub.s f7, f16, f7, f11, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 576(x17)\n"
			"flw.ps f7, 256(x17)\n"
			"fsgnj.s f5, f12, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 672(x18)\n"
			"fandi.pi f7, f4, 0x117\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 32(x17)\n"
			"flw.ps f12, 320(x17)\n"
			"feq.s x26, f10, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 192(x12)\n"
			"flw.ps f9, 576(x12)\n"
			"fsgnjn.ps f16, f17, f9\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_0:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 384(x6)\n"
			"flog.ps f31, f7\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_0:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 544(x18)\n"
			"frcp.ps f11, f4\n" 
			"li x18, 0xeaac4e8c7d86cf59\n"
			"fcvt.s.wu f29, x18, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 64(x12)\n"
			"fpackrepb.pi f24, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 640(x10)\n"
			"fcvt.f16.ps f10, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 896(x17)\n"
			"fsqrt.s f11, f29, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 32(x27)\n"
			"flw.ps f31, 960(x27)\n"
			"fsgnjn.ps f10, f10, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 256(x22)\n"
			"fswizz.ps f9, f17, 0x1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 992(x10)\n"
			"flw.ps f24, 416(x10)\n"
			"flw.ps f4, 352(x10)\n"
			"fnmadd.ps f29, f16, f24, f4, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x197\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xd2 << TENSOR_FMA16_SCP_LOC_B) | (0x3b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
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
			"LBL_HPM_3_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 704(x23)\n"
			"flw.ps f30, 928(x23)\n"
			"fsgnjn.s f9, f29, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 320(x17)\n"
			"flw.ps f8, 992(x17)\n"
			"fand.pi f28, f18, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 960(x26)\n"
			"fpackreph.pi f30, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 672(x23)\n"
			"fpackrepb.pi f0, f22\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 704(x17)\n"
			"fslli.pi f30, f31, 0x5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 320(x25)\n"
			"fsrai.pi f2, f29, 0x9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 416(x9)\n"
			"flw.ps f31, 864(x9)\n"
			"fmul.ps f4, f22, f31, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 992(x6)\n"
			"frsq.ps f4, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 928(x28)\n"
			"flw.ps f4, 608(x28)\n"
			"fle.s x28, f15, f4\n" 
			"mova.x.m x17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 576(x9)\n"
			"flw.ps f10, 576(x9)\n"
			"flw.ps f26, 192(x9)\n"
			"fnmadd.ps f28, f0, f10, f26, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 992(x17)\n"
			"flw.ps f26, 32(x17)\n"
			"fsub.ps f2, f18, f26, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 544(x26)\n"
			"flw.ps f22, 832(x26)\n"
			"fsgnj.ps f15, f2, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 864(x28)\n"
			"fcvt.pw.ps f29, f27, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 864(x9)\n"
			"flog.ps f4, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fsetm.pi m5, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 512(x15)\n"
			"flw.ps f18, 896(x15)\n"
			"fle.s x13, f3, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"fsetm.pi m1, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 224(x26)\n"
			"flt.ps f8, f0, f0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 448(x25)\n"
			"fsqrt.ps f18, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 832(x26)\n"
			"fclass.s x26, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 512(x9)\n"
			"flw.ps f22, 736(x9)\n"
			"fle.pi f3, f31, f22\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 832(x15)\n"
			"flw.ps f0, 256(x15)\n"
			"flw.ps f31, 800(x15)\n"
			"fmadd.ps f18, f3, f0, f31, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 672(x23)\n"
			"flw.ps f8, 288(x23)\n"
			"fmax.pi f28, f31, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 64(x17)\n"
			"fclass.s x28, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_1:\n"
			"fltm.pi m5, f4, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 96(x17)\n"
			"fcvt.w.s x26, f26, rtz\n" 
			"fbci.pi f27, 0x12e88\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 512(x13)\n"
			"flw.ps f0, 832(x13)\n"
			"feq.ps f27, f31, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 992(x23)\n"
			"flw.ps f27, 672(x23)\n"
			"flw.ps f22, 640(x23)\n"
			"fnmsub.s f28, f31, f27, f22, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 864(x13)\n"
			"ffrc.ps f29, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 384(x19)\n"
			"flw.ps f27, 480(x19)\n"
			"fadd.ps f30, f4, f27, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 864(x13)\n"
			"fcvt.w.s x23, f3, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 832(x26)\n"
			"flw.ps f26, 128(x26)\n"
			"fsgnjx.s f31, f29, f26\n" 
			"fbci.pi f8, 0x18278\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 992(x28)\n"
			"flw.ps f2, 992(x28)\n"
			"fmsub.ps f4, f9, f9, f2, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 512(x15)\n"
			"flw.ps f4, 448(x15)\n"
			"fsgnjn.s f30, f18, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 832(x23)\n"
			"flw.ps f18, 448(x23)\n"
			"fdivu.pi f30, f0, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 640(x26)\n"
			"flw.ps f2, 224(x26)\n"
			"fsra.pi f22, f22, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 64(x28)\n"
			"fclass.ps f9, f27\n" 
			"fbci.ps f4, 0x6fc07\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 800(x17)\n"
			"flw.ps f30, 896(x17)\n"
			"fmin.pi f18, f31, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 0(x23)\n"
			"flw.ps f26, 224(x23)\n"
			"fle.ps f31, f0, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_1:\n"
			"fltm.ps m5, f0, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f22, 896(x6)\n"
			"flw.ps f3, 640(x6)\n"
			"fle.pi f27, f22, f3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 320(x28)\n"
			"flw.ps f2, 736(x28)\n"
			"fsgnjx.ps f15, f26, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 512(x26)\n"
			"flw.ps f15, 288(x26)\n"
			"fadd.pi f2, f22, f15\n" 
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
			"LBL_HPM_3_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
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
			"la x19, rand_int_32\n"
			"flw.ps f31, 768(x19)\n"
			"faddi.pi f9, f31, 0x2b\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 672(x28)\n"
			"flw.ps f5, 832(x28)\n"
			"fadd.s f2, f15, f5, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f7, 832(x25)\n"
			"flw.ps f25, 192(x25)\n"
			"fremu.pi f21, f7, f25\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 608(x25)\n"
			"flw.ps f14, 672(x25)\n"
			"for.pi f25, f8, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 352(x9)\n"
			"flw.ps f4, 288(x9)\n"
			"flw.ps f5, 0(x9)\n"
			"fmsub.ps f5, f31, f4, f5, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 768(x13)\n"
			"flw.ps f10, 896(x13)\n"
			"fmaxu.pi f21, f15, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 896(x19)\n"
			"fandi.pi f8, f5, 0xd3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 256(x23)\n"
			"fcvt.ps.pw f2, f17, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f8, 352(x28)\n"
			"fsrai.pi f2, f8, 0x6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 832(x10)\n"
			"fswizz.ps f17, f21, 0x18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_int_32\n"
			"flw.ps f18, 992(x13)\n"
			"flw.ps f14, 224(x13)\n"
			"fltu.pi f21, f18, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 704(x25)\n"
			"flw.ps f31, 32(x25)\n"
			"fsub.pi f9, f2, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 576(x9)\n"
			"fandi.pi f15, f24, 0x11a\n" 
			"la x26, rand_int_32\n"
			"flw.ps f31, 928(x26)\n"
			"fpackreph.pi f7, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 864(x23)\n"
			"flw.ps f26, 512(x23)\n"
			"fmin.ps f26, f15, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 672(x26)\n"
			"flw.ps f2, 352(x26)\n"
			"fmin.ps f10, f25, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_2:\n"
			"feqm.ps m6, f8, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"mov.m.x m1, x26, 0x7e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f9, 864(x17)\n"
			"flw.ps f15, 768(x17)\n"
			"feq.pi f10, f9, f15\n" 
			"li x5, 0xc34278e689f0d414\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x89c751aeebb1b998\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x11, 0x876f9d4062c04f76\n"
			"fcvt.s.wu f26, x11, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 128(x19)\n"
			"fsat8.pi f2, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 128(x25)\n"
			"ffrc.ps f24, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 672(x10)\n"
			"fsatu8.pi f25, f21\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 768(x23)\n"
			"flw.ps f25, 448(x23)\n"
			"fsll.pi f18, f9, f25\n" 
			"li x5, 0x9851\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 960(x28)\n"
			"fclass.ps f18, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 448(x11)\n"
			"fclass.ps f26, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 640(x17)\n"
			"fcvt.pwu.ps f14, f7, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 864(x23)\n"
			"flw.ps f8, 384(x23)\n"
			"fadd.ps f5, f4, f8, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 480(x17)\n"
			"fsin.ps f31, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 32(x23)\n"
			"fsqrt.ps f5, f7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 224(x23)\n"
			"flw.ps f31, 896(x23)\n"
			"flw.ps f5, 928(x23)\n"
			"fmadd.s f10, f21, f31, f5, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 736(x28)\n"
			"flw.ps f2, 192(x28)\n"
			"flw.ps f25, 192(x28)\n"
			"fnmadd.s f31, f8, f2, f25, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_999616_HID_2:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x6 << TENSOR_FMA32_SCP_LOC_B) | (0x64 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"li x11, 0x53b9947aec2fe0a0\n"
			"fcvt.s.w f10, x11, rmm\n" 
			"li x5, 0x8f0f\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f26, 416(x23)\n"
			"flw.ps f7, 608(x23)\n"
			"fand.pi f17, f26, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 96(x17)\n"
			"flw.ps f24, 768(x17)\n"
			"fle.pi f25, f17, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 960(x25)\n"
			"fcvt.pw.ps f9, f26, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_2:\n"
			"fltm.ps m4, f14, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xd0c9a8fc4afdaeb9\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xae5846557df05a09\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_2:\n"
			"fltm.ps m5, f14, f26\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x1cb3e040a4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x28, m6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 768(x23)\n"
			"flw.ps f24, 736(x23)\n"
			"fmax.s f10, f26, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 896(x26)\n"
			"flw.ps f31, 0(x26)\n"
			"feq.s x10, f5, f31\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 192(x25)\n"
			"flw.ps f10, 384(x25)\n"
			"fltu.pi f7, f14, f10\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 576(x5)\n"
			"flw.ps f7, 928(x5)\n"
			"fmul.pi f24, f2, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 256(x17)\n"
			"flw.ps f31, 480(x17)\n"
			"fsrl.pi f7, f14, f31\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_2:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 704(x13)\n"
			"frcp.ps f2, f24\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 416(x11)\n"
			"fpackrepb.pi f25, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 0(x26)\n"
			"flw.ps f7, 800(x26)\n"
			"fsgnj.ps f8, f14, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 384(x28)\n"
			"flw.ps f8, 992(x28)\n"
			"fsgnjx.ps f5, f14, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_530755_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x32 << TENSOR_FMA16_SCP_LOC_B) | (0x55 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x37 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x41 << TENSOR_FMA16_SCP_LOC_B) | (0xb << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa8 << TENSOR_IMA_SCP_LOC_B) | (0x4b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fltm.pi m0, f29, f20\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskor m2, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f25, 576(x10)\n"
			"fsrai.pi f29, f25, 0x4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 0(x17)\n"
			"flw.ps f5, 928(x17)\n"
			"flt.pi f22, f25, f5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 512(x28)\n"
			"fcvt.pw.ps f4, f7, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 704(x11)\n"
			"flw.ps f9, 576(x11)\n"
			"flt.pi f15, f15, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_3:\n"
			"fltm.ps m5, f25, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f7, 704(x28)\n"
			"flw.ps f9, 672(x28)\n"
			"fle.pi f15, f7, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 864(x17)\n"
			"flw.ps f20, 800(x17)\n"
			"flw.ps f23, 896(x17)\n"
			"fmadd.s f16, f7, f20, f23, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 928(x28)\n"
			"flw.ps f16, 32(x28)\n"
			"fsgnjn.ps f6, f9, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_3:\n"
			"maskand m0, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f6, 128(x27)\n"
			"flw.ps f20, 512(x27)\n"
			"fsrl.pi f25, f6, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 416(x28)\n"
			"flw.ps f4, 864(x28)\n"
			"flw.ps f6, 512(x28)\n"
			"fmsub.ps f20, f16, f4, f6, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 64(x25)\n"
			"flw.ps f4, 672(x25)\n"
			"fsgnjx.s f14, f16, f4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 320(x17)\n"
			"ffrc.ps f9, f20\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 512(x17)\n"
			"flw.ps f28, 224(x17)\n"
			"fsgnj.s f4, f4, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"li x17, 0x22d499e622e2deda\n"
			"fcvt.s.w f14, x17, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 992(x9)\n"
			"fcvt.f16.ps f5, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 384(x9)\n"
			"flw.ps f4, 736(x9)\n"
			"fmax.ps f28, f16, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_3:\n"
			"fltm.pi m0, f5, f31\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401A8\n"
			"ld x5, 0(x5)\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 320(x28)\n"
			"flw.ps f15, 928(x28)\n"
			"fsgnj.ps f21, f25, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f14, 416(x27)\n"
			"flw.ps f9, 128(x27)\n"
			"fmul.s f4, f14, f9, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 864(x25)\n"
			"flw.ps f29, 384(x25)\n"
			"flt.pi f9, f20, f29\n" 
			"fbci.ps f25, 0x27966\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 864(x11)\n"
			"fsatu8.pi f28, f4\n" 
			"maskpopc x11, m2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 896(x11)\n"
			"flw.ps f15, 960(x11)\n"
			"flt.ps f31, f6, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_3:\n"
			"fltm.pi m4, f31, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 992(x13)\n"
			"fcvt.pw.ps f5, f22, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 224(x11)\n"
			"fswizz.ps f28, f7, 0x11\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 800(x27)\n"
			"flw.ps f16, 320(x27)\n"
			"fmaxu.pi f16, f4, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_3:\n"
			"fltm.pi m2, f7, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 384(x28)\n"
			"flw.ps f9, 640(x28)\n"
			"fmax.ps f5, f21, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_3:\n"
			"maskxor m4, m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f21, 0x9a61\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_3:\n"
			"fltm.pi m4, f29, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f25, 352(x25)\n"
			"fpackreph.pi f6, f25\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 224(x26)\n"
			"fsat8.pi f25, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 512(x9)\n"
			"fcvt.f16.ps f14, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 320(x26)\n"
			"flw.ps f31, 768(x26)\n"
			"fsgnjn.s f15, f20, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_3:\n"
			"feqm.ps m4, f20, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f28, 608(x27)\n"
			"fslli.pi f28, f28, 0xd\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 352(x11)\n"
			"flw.ps f4, 640(x11)\n"
			"fsgnjx.ps f20, f6, f4\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 448(x26)\n"
			"flw.ps f25, 224(x26)\n"
			"flw.ps f5, 704(x26)\n"
			"fmadd.ps f29, f21, f25, f5, rne\n" 
			"LBL_SEQID_2_FP_TRANS_ID_10_HID_3:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 224(x25)\n"
			"fexp.ps f28, f31\n" 
			"mova.x.m x13\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 192(x6)\n"
			"flw.ps f15, 672(x6)\n"
			"flt.ps f31, f16, f15\n" 
			"maskpopcz x13, m5\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 480(x10)\n"
			"fcvt.pw.ps f24, f13, rmm\n" 
			"li x12, 0x45e910b3e66f26ea\n"
			"fcvt.s.lu f19, x12, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskand m7, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 384(x20)\n"
			"flw.ps f21, 768(x20)\n"
			"fsgnjn.s f25, f11, f21\n" 
			"la x19, rand_int_32\n"
			"flw.ps f31, 192(x19)\n"
			"flw.ps f13, 928(x19)\n"
			"flt.pi f15, f31, f13\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 576(x13)\n"
			"flw.ps f11, 160(x13)\n"
			"fminu.pi f13, f18, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 704(x20)\n"
			"fround.ps f19, f25, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 320(x26)\n"
			"flw.ps f10, 672(x26)\n"
			"fmin.ps f31, f31, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 544(x19)\n"
			"flw.ps f12, 352(x19)\n"
			"fmin.s f31, f15, f12\n" 
			"li x5, 0x46c1\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f1, 800(x20)\n"
			"flw.ps f25, 448(x20)\n"
			"fsrl.pi f24, f1, f25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 864(x5)\n"
			"flw.ps f24, 928(x5)\n"
			"fadd.pi f7, f19, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 512(x13)\n"
			"fswizz.ps f13, f10, 0x1d\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 832(x20)\n"
			"fandi.pi f21, f10, 0x4a\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 448(x25)\n"
			"flw.ps f18, 768(x25)\n"
			"flw.ps f24, 928(x25)\n"
			"fmsub.ps f19, f0, f18, f24, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 160(x12)\n"
			"fsin.ps f25, f11\n" 
			"li x26, 0x75e9863986c5f63d\n"
			"fcvt.s.w f25, x26, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xc3 << TENSOR_IMA_SCP_LOC_B) | (0xe6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 192(x25)\n"
			"fswizz.ps f20, f24, 0xe\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 608(x10)\n"
			"flw.ps f29, 96(x10)\n"
			"fmadd.ps f25, f19, f29, f19, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_4:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 512(x25)\n"
			"flog.ps f29, f7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 192(x15)\n"
			"fcvt.wu.s x26, f25, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 832(x10)\n"
			"flw.ps f7, 576(x10)\n"
			"fdiv.ps f25, f11, f7, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 256(x10)\n"
			"flw.ps f19, 416(x10)\n"
			"flt.s x19, f0, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 896(x15)\n"
			"flw.ps f25, 128(x15)\n"
			"fsgnj.ps f19, f21, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 0(x20)\n"
			"flw.ps f11, 256(x20)\n"
			"fmax.s f1, f10, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 32(x10)\n"
			"fpackrepb.pi f25, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_int_32\n"
			"flw.ps f21, 832(x13)\n"
			"flw.ps f15, 128(x13)\n"
			"flt.pi f19, f21, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 192(x5)\n"
			"fpackreph.pi f24, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 608(x25)\n"
			"flw.ps f20, 832(x25)\n"
			"flw.ps f19, 64(x25)\n"
			"fmsub.ps f25, f18, f20, f19, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 576(x12)\n"
			"flw.ps f21, 960(x12)\n"
			"fmadd.ps f7, f31, f31, f21, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 768(x15)\n"
			"flw.ps f15, 832(x15)\n"
			"fmulh.pi f18, f21, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb03f72077e\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"fltm.ps m7, f24, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x3 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x9 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x2 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x2 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x7 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 288(x25)\n"
			"flw.ps f24, 384(x25)\n"
			"fadd.ps f24, f21, f24, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 160(x15)\n"
			"flw.ps f31, 160(x15)\n"
			"fmax.pi f19, f20, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 480(x19)\n"
			"fcvt.w.s x13, f7, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 864(x19)\n"
			"fcvt.lu.s x10, f10, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 0(x26)\n"
			"flw.ps f11, 512(x26)\n"
			"feq.s x13, f25, f11\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 384(x20)\n"
			"flw.ps f25, 64(x20)\n"
			"fand.pi f21, f29, f25\n" 
			"li x5, 0x5cba\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"maskxor m4, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 384(x20)\n"
			"flw.ps f10, 160(x20)\n"
			"flw.ps f1, 128(x20)\n"
			"fnmsub.ps f1, f31, f10, f1, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f24, 768(x5)\n"
			"feq.pi f19, f24, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 320(x26)\n"
			"flw.ps f15, 480(x26)\n"
			"fsra.pi f29, f13, f15\n" 
			"li x5, 0x2323\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f20, 768(x13)\n"
			"flw.ps f0, 672(x13)\n"
			"fsll.pi f12, f20, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 0(x12)\n"
			"ffrc.ps f24, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_4:\n"
			"mov.m.x m5, x10, 0x69\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_4:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 352(x20)\n"
			"flog.ps f11, f19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 320(x26)\n"
			"flw.ps f24, 544(x26)\n"
			"fsgnj.s f10, f18, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 896(x16)\n"
			"fswizz.ps f31, f15, 0x1c\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_61092_HID_4:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x5 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x3 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x9 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1a << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x9 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
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
			"maskpopc x27, m2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 32(x9)\n"
			"flw.ps f25, 64(x9)\n"
			"fadd.pi f26, f20, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f22, 320(x12)\n"
			"flw.ps f26, 704(x12)\n"
			"fmulhu.pi f0, f22, f26\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 768(x10)\n"
			"flw.ps f26, 128(x10)\n"
			"fsgnjn.ps f20, f24, f26\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_5:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 768(x12)\n"
			"frcp.ps f0, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"mov.m.x m3, x7, 0x1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 576(x9)\n"
			"fcvt.pw.ps f14, f20, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"feqm.ps m3, f11, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 608(x9)\n"
			"fswizz.ps f11, f31, 0xb\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_5:\n"
			"mov.m.x m2, x7, 0x32\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_5:\n"
			"fltm.ps m2, f1, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 416(x7)\n"
			"fcvt.w.s x7, f26, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_5:\n"
			"fltm.ps m1, f15, f20\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x28fac65ad\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_6_HID_5:\n"
			"fltm.pi m3, f25, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 800(x11)\n"
			"flw.ps f25, 640(x11)\n"
			"fsgnjx.s f9, f22, f25\n" 
			"LBL_SEQID_0_FP_TRANS_ID_7_HID_5:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 928(x7)\n"
			"fexp.ps f11, f0\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 384(x10)\n"
			"flw.ps f15, 288(x10)\n"
			"flw.ps f9, 992(x10)\n"
			"fmadd.ps f25, f11, f15, f9, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 128(x9)\n"
			"flw.ps f28, 544(x9)\n"
			"flt.s x10, f0, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 160(x10)\n"
			"flw.ps f15, 768(x10)\n"
			"fmin.pi f25, f9, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 512(x10)\n"
			"fsrai.pi f11, f16, 0x6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 160(x5)\n"
			"flw.ps f9, 96(x5)\n"
			"fmin.ps f11, f20, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 128(x11)\n"
			"flw.ps f26, 832(x11)\n"
			"frem.pi f16, f31, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 448(x22)\n"
			"ffrc.ps f11, f10\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 800(x27)\n"
			"flw.ps f1, 992(x27)\n"
			"flt.s x20, f20, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 896(x5)\n"
			"flw.ps f31, 832(x5)\n"
			"feq.ps f28, f3, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 992(x18)\n"
			"fcvt.w.s x7, f26, rne\n" 
			"maskpopcz x20, m4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 960(x10)\n"
			"flw.ps f22, 288(x10)\n"
			"fsgnjn.ps f26, f0, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 704(x12)\n"
			"flw.ps f15, 640(x12)\n"
			"fsgnj.ps f26, f28, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_5:\n"
			"maskor m4, m4, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x1702fda025\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 512(x22)\n"
			"frsq.ps f0, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 832(x18)\n"
			"flw.ps f20, 512(x18)\n"
			"fmax.s f14, f24, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 832(x9)\n"
			"flw.ps f3, 224(x9)\n"
			"fsub.s f9, f11, f3, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 768(x7)\n"
			"fpackreph.pi f10, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 640(x5)\n"
			"flw.ps f10, 896(x5)\n"
			"fmin.ps f16, f16, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 288(x22)\n"
			"fsqrt.ps f9, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f22, 512(x27)\n"
			"flw.ps f31, 320(x27)\n"
			"feq.pi f11, f22, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 192(x5)\n"
			"fcvt.pwu.ps f26, f16, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_5:\n"
			"maskand m1, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f14, 0(x20)\n"
			"flw.ps f16, 352(x20)\n"
			"fminu.pi f10, f14, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 256(x9)\n"
			"flw.ps f15, 352(x9)\n"
			"fmax.pi f3, f3, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 320(x20)\n"
			"flw.ps f0, 960(x20)\n"
			"fadd.ps f28, f28, f0, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 32(x9)\n"
			"fcvt.ps.pwu f28, f14, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 768(x27)\n"
			"fclass.s x22, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_5:\n"
			"feqm.ps m3, f25, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f28, 864(x7)\n"
			"flw.ps f15, 0(x7)\n"
			"feq.pi f28, f28, f15\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 768(x7)\n"
			"fsrli.pi f31, f25, 0x8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 576(x5)\n"
			"flw.ps f16, 896(x5)\n"
			"fsll.pi f16, f31, f16\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 0(x15)\n"
			"flw.ps f11, 0(x15)\n"
			"flw.ps f23, 672(x15)\n"
			"fmadd.s f9, f13, f11, f23, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 480(x14)\n"
			"flw.ps f1, 128(x14)\n"
			"fmulhu.pi f14, f31, f1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 736(x20)\n"
			"flw.ps f16, 128(x20)\n"
			"fmax.pi f11, f13, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 416(x12)\n"
			"flw.ps f30, 608(x12)\n"
			"fmin.s f7, f3, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 224(x25)\n"
			"fsub.s f30, f1, f1, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 192(x13)\n"
			"fcvt.pw.ps f3, f16, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 512(x20)\n"
			"fcvt.ps.pwu f28, f7, rne\n" 
			"li x5, 0x92e8efa440bfd0a2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2561a642ad6245f1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_6:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 672(x14)\n"
			"flog.ps f18, f8\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 160(x27)\n"
			"flw.ps f13, 832(x27)\n"
			"fsgnjx.s f1, f7, f13\n" 
			"mova.x.m x27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f29, 576(x16)\n"
			"faddi.pi f11, f29, 0xaa\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 224(x16)\n"
			"fpackrepb.pi f14, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 800(x13)\n"
			"flw.ps f1, 608(x13)\n"
			"flw.ps f14, 32(x13)\n"
			"fnmsub.ps f21, f18, f1, f14, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 608(x9)\n"
			"flw.ps f3, 128(x9)\n"
			"fmul.s f28, f28, f3, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 800(x25)\n"
			"fcvt.pwu.ps f29, f3, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f3, 512(x15)\n"
			"flw.ps f23, 256(x15)\n"
			"fltu.pi f29, f3, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_546481_HID_6:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f8, 928(x27)\n"
			"flw.ps f13, 64(x27)\n"
			"fle.pi f7, f8, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_6:\n"
			"fltm.ps m6, f23, f28\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"flem.ps m3, f30, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 576(x10)\n"
			"fswizz.ps f9, f29, 0x6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 480(x25)\n"
			"fcvt.wu.s x15, f13, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 448(x12)\n"
			"fclass.s x9, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 704(x25)\n"
			"fcvt.f16.ps f8, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 608(x15)\n"
			"fpackreph.pi f29, f18\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 864(x13)\n"
			"flw.ps f16, 352(x13)\n"
			"fmadd.ps f30, f13, f16, f16, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 384(x10)\n"
			"fclass.ps f11, f28\n" 
			"li x14, 0x5f7c89b229223484\n"
			"fcvt.s.lu f16, x14, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 768(x9)\n"
			"flw.ps f30, 64(x9)\n"
			"flt.s x10, f23, f30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 960(x16)\n"
			"flw.ps f11, 448(x16)\n"
			"fsub.s f29, f30, f11, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 832(x13)\n"
			"flw.ps f9, 800(x13)\n"
			"fmulh.pi f30, f16, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"fltm.ps m5, f16, f23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xadcf616558\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"feqm.ps m3, f3, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x65 << TENSOR_FMA32_SCP_LOC_B) | (0xb8 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5f << TENSOR_FMA16_SCP_LOC_B) | (0x81 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xba << TENSOR_IMA_SCP_LOC_B) | (0xee << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f31, 96(x27)\n"
			"fpackreph.pi f7, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_6:\n"
			"flem.ps m3, f14, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f28, 352(x25)\n"
			"flw.ps f3, 288(x25)\n"
			"fsll.pi f8, f28, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 864(x15)\n"
			"flw.ps f3, 416(x15)\n"
			"fle.ps f8, f29, f3\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 832(x20)\n"
			"fcvt.w.s x16, f18, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 384(x15)\n"
			"frsq.ps f13, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 416(x12)\n"
			"flw.ps f30, 0(x12)\n"
			"fnmadd.ps f30, f3, f30, f3, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"fsetm.pi m6, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_6:\n"
			"fltm.ps m5, f3, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 64(x25)\n"
			"flw.ps f7, 480(x25)\n"
			"feq.s x20, f11, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 992(x9)\n"
			"flw.ps f23, 672(x9)\n"
			"fadd.s f23, f29, f23, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 96(x12)\n"
			"fandi.pi f7, f16, 0x1ca\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 256(x13)\n"
			"flw.ps f13, 480(x13)\n"
			"fmax.ps f8, f11, f13\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_6:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 288(x22)\n"
			"flog.ps f9, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 192(x13)\n"
			"flw.ps f31, 704(x13)\n"
			"flw.ps f9, 992(x13)\n"
			"fnmadd.ps f21, f1, f31, f9, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 832(x13)\n"
			"flw.ps f1, 768(x13)\n"
			"fminu.pi f7, f28, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x95 << TENSOR_IMA_SCP_LOC_B) | (0x80 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
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
			"la x13, rand_int_32\n"
			"flw.ps f22, 864(x13)\n"
			"fsat8.pi f19, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 864(x7)\n"
			"fexp.ps f24, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 736(x16)\n"
			"flw.ps f15, 576(x16)\n"
			"fmin.ps f3, f27, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 192(x20)\n"
			"fcvt.pw.ps f31, f18, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 224(x5)\n"
			"fswizz.ps f1, f1, 0x1f\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 672(x27)\n"
			"flw.ps f19, 896(x27)\n"
			"fmul.s f17, f15, f19, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 768(x18)\n"
			"flw.ps f27, 800(x18)\n"
			"flt.pi f18, f1, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 736(x27)\n"
			"fclass.s x7, f18\n" 
			"li x7, 0xa7274490f48e87d9\n"
			"fcvt.s.wu f29, x7, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 576(x13)\n"
			"flw.ps f27, 32(x13)\n"
			"fsrl.pi f31, f26, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 256(x22)\n"
			"flw.ps f3, 160(x22)\n"
			"feq.s x16, f27, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 416(x13)\n"
			"fcvt.wu.s x27, f16, rne\n" 
			"maskpopcz x7, m0\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 32(x22)\n"
			"fcvt.ps.pwu f12, f19, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 800(x22)\n"
			"fnot.pi f17, f3\n" 
			"maskpopc x13, m5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"fbci.pi f27, 0x7ac58\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 896(x13)\n"
			"flw.ps f17, 64(x13)\n"
			"fsrl.pi f27, f26, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 992(x9)\n"
			"faddi.pi f12, f24, 0x10f\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_7:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f29, 736(x16)\n"
			"flog.ps f3, f29\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 864(x20)\n"
			"flw.ps f25, 256(x20)\n"
			"fmin.s f16, f29, f25\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 992(x22)\n"
			"flw.ps f16, 448(x22)\n"
			"fminu.pi f22, f24, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 576(x9)\n"
			"flw.ps f16, 736(x9)\n"
			"fadd.pi f16, f18, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 800(x9)\n"
			"flw.ps f13, 704(x9)\n"
			"fsll.pi f3, f18, f13\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 608(x7)\n"
			"flw.ps f25, 480(x7)\n"
			"feq.pi f19, f12, f25\n" 
			"fbci.ps f29, 0x17fbc\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_7:\n"
			"fltm.ps m5, f24, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 224(x5)\n"
			"flw.ps f16, 736(x5)\n"
			"flw.ps f24, 352(x5)\n"
			"fmadd.s f13, f31, f16, f24, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f24, 832(x5)\n"
			"flw.ps f29, 352(x5)\n"
			"fmax.pi f29, f24, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 64(x5)\n"
			"fcvt.ps.pw f1, f15, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 160(x22)\n"
			"flw.ps f17, 576(x22)\n"
			"flw.ps f26, 736(x22)\n"
			"fnmsub.s f26, f27, f17, f26, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_7:\n"
			"fltm.ps m6, f25, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_7:\n"
			"masknot m3, m3\n" 
			"li x5, 0x803401D8\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 800(x13)\n"
			"flw.ps f17, 960(x13)\n"
			"fmul.ps f16, f19, f17, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_7:\n"
			"feqm.ps m5, f19, f3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2a690863b3\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 576(x5)\n"
			"flw.ps f26, 704(x5)\n"
			"fsgnjn.s f19, f19, f26\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 544(x5)\n"
			"flw.ps f3, 288(x5)\n"
			"feq.pi f16, f18, f3\n" 
			"la x20, rand_int_32\n"
			"flw.ps f12, 192(x20)\n"
			"flw.ps f3, 640(x20)\n"
			"fadd.pi f24, f12, f3\n" 
			"la x27, rand_int_32\n"
			"flw.ps f25, 800(x27)\n"
			"flw.ps f22, 160(x27)\n"
			"fminu.pi f26, f25, f22\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 864(x17)\n"
			"flw.ps f12, 32(x17)\n"
			"fle.pi f15, f1, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"mov.m.x m5, x27, 0x2b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x9, 0x10d71d4810072c5\n"
			"fcvt.s.w f22, x9, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 384(x5)\n"
			"flw.ps f24, 800(x5)\n"
			"fsub.s f24, f31, f24, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_7:\n"
			"mov.m.x m5, x13, 0x63\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 800(x18)\n"
			"flw.ps f18, 512(x18)\n"
			"fle.ps f3, f26, f18\n" 
			"la x5, rand_int_32\n"
			"flw.ps f1, 352(x5)\n"
			"flw.ps f25, 896(x5)\n"
			"fxor.pi f17, f1, f25\n" 
			"maskpopc x17, m5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_7:\n"
			"maskand m3, m0, m0\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 448(x14)\n"
			"fround.ps f30, f13, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 832(x14)\n"
			"fandi.pi f1, f13, 0x1f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 224(x14)\n"
			"flw.ps f2, 960(x14)\n"
			"flw.ps f13, 704(x14)\n"
			"fnmadd.s f6, f6, f2, f13, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"maskand m7, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f20, 960(x27)\n"
			"flw.ps f27, 160(x27)\n"
			"feq.pi f25, f20, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 704(x15)\n"
			"flt.s x5, f16, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 288(x26)\n"
			"flw.ps f13, 384(x26)\n"
			"fmin.s f27, f2, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 0(x14)\n"
			"faddi.pi f20, f13, 0x1aa\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 864(x16)\n"
			"flw.ps f8, 0(x16)\n"
			"fmul.pi f13, f6, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 352(x20)\n"
			"flw.ps f27, 384(x20)\n"
			"flw.ps f30, 128(x20)\n"
			"fmadd.s f30, f25, f27, f30, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"masknot m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 800(x19)\n"
			"fsqrt.ps f20, f13\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 576(x5)\n"
			"fsrli.pi f30, f27, 0xe\n" 
			"li x15, 0x9cba1b125177450b\n"
			"fcvt.s.w f21, x15, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 256(x15)\n"
			"fcvt.w.s x15, f29, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_8:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 352(x15)\n"
			"frcp.ps f5, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa8 << TENSOR_IMA_SCP_LOC_B) | (0x4d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_8:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x15, rand_ieee754_16\n"
			"flw.ps f27, 800(x15)\n"
			"fcvt.ps.f16 f5, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"fltm.pi m4, f2, f4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x3969db969f\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x1378\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 704(x16)\n"
			"flw.ps f4, 960(x16)\n"
			"fsgnjx.ps f20, f5, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 544(x7)\n"
			"fcvt.f16.ps f20, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 96(x14)\n"
			"flw.ps f5, 96(x14)\n"
			"feq.pi f1, f1, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 832(x6)\n"
			"fsgnj.s f1, f1, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 224(x26)\n"
			"flw.ps f1, 96(x26)\n"
			"flt.s x27, f28, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 576(x27)\n"
			"fcvt.wu.s x27, f5, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 128(x19)\n"
			"fcvt.f16.ps f8, f25\n" 
			"fbci.pi f4, 0x76b39\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 832(x20)\n"
			"flw.ps f2, 256(x20)\n"
			"feq.ps f6, f4, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 352(x26)\n"
			"flw.ps f4, 736(x26)\n"
			"fremu.pi f29, f29, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f25, 384(x6)\n"
			"fsat8.pi f5, f25\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 576(x5)\n"
			"flw.ps f6, 992(x5)\n"
			"fcmovm.ps f1, f4, f6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 0(x14)\n"
			"flw.ps f1, 832(x14)\n"
			"fmul.s f1, f30, f1, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_8:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"mova.x.m x6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 0(x14)\n"
			"flw.ps f16, 384(x14)\n"
			"flt.pi f21, f29, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 896(x19)\n"
			"flw.ps f10, 576(x19)\n"
			"fsgnjx.s f21, f6, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 608(x7)\n"
			"flw.ps f29, 64(x7)\n"
			"flt.pi f4, f6, f29\n" 
			"li x5, 0xa175\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 480(x7)\n"
			"flw.ps f6, 384(x7)\n"
			"fsgnj.ps f27, f29, f6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_int_32\n"
			"flw.ps f1, 384(x27)\n"
			"flw.ps f25, 960(x27)\n"
			"fsra.pi f6, f1, f25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 736(x5)\n"
			"flw.ps f5, 32(x5)\n"
			"fmulh.pi f20, f27, f5\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f28, 608(x6)\n"
			"fcvt.ps.f16 f16, f28\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 928(x16)\n"
			"fsatu8.pi f21, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 480(x20)\n"
			"flw.ps f16, 64(x20)\n"
			"fsgnjx.s f25, f10, f16\n" 
			"fbci.pi f13, 0x5b1c1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 256(x5)\n"
			"fround.ps f29, f28, rup\n" 
			"li x5, 0x55d7\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 32(x19)\n"
			"flw.ps f5, 128(x19)\n"
			"flt.ps f27, f6, f5\n" 
			"fbci.pi f6, 0x7e3fa\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 192(x14)\n"
			"flw.ps f4, 896(x14)\n"
			"flw.ps f25, 448(x14)\n"
			"fmadd.s f10, f1, f4, f25, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_8:\n"
			"maskand m6, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_3503_HID_8:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb8 << TENSOR_FMA32_SCP_LOC_B) | (0xd3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 64(x19)\n"
			"fcmovm.ps f2, f27, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 928(x25)\n"
			"flw.ps f20, 736(x25)\n"
			"fmaxu.pi f30, f9, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f3, 512(x7)\n"
			"flw.ps f28, 480(x7)\n"
			"fmax.pi f8, f3, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f18, 800(x10)\n"
			"flw.ps f9, 672(x10)\n"
			"fsub.pi f9, f18, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 832(x7)\n"
			"fswizz.ps f3, f28, 0x1e\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 128(x25)\n"
			"flw.ps f27, 896(x25)\n"
			"flw.ps f9, 896(x25)\n"
			"fmsub.ps f26, f3, f27, f9, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 512(x25)\n"
			"flw.ps f8, 864(x25)\n"
			"flw.ps f27, 704(x25)\n"
			"fmsub.ps f14, f18, f8, f27, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 576(x23)\n"
			"flw.ps f27, 672(x23)\n"
			"fle.s x15, f0, f27\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 128(x18)\n"
			"fcvt.ps.pw f4, f14, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_9:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 480(x19)\n"
			"flog.ps f24, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 896(x25)\n"
			"fcvt.ps.pwu f28, f2, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 800(x18)\n"
			"fclass.s x23, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 608(x7)\n"
			"flw.ps f4, 480(x7)\n"
			"flw.ps f20, 608(x7)\n"
			"fnmsub.ps f28, f0, f4, f20, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 544(x10)\n"
			"flw.ps f18, 448(x10)\n"
			"flw.ps f8, 0(x10)\n"
			"fnmadd.s f28, f28, f18, f8, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 192(x6)\n"
			"flw.ps f10, 960(x6)\n"
			"fadd.ps f18, f26, f10, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 512(x19)\n"
			"flw.ps f10, 768(x19)\n"
			"fdiv.s f28, f9, f10, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 448(x26)\n"
			"flw.ps f2, 288(x26)\n"
			"fsrl.pi f27, f0, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f24, 288(x7)\n"
			"fsat8.pi f27, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 0(x23)\n"
			"flw.ps f10, 256(x23)\n"
			"fadd.ps f8, f3, f10, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 256(x6)\n"
			"flog.ps f27, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 640(x26)\n"
			"fsrli.pi f0, f28, 0x6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f24, 160(x26)\n"
			"flw.ps f12, 960(x26)\n"
			"fmul.pi f20, f24, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 960(x26)\n"
			"flw.ps f2, 128(x26)\n"
			"fdiv.ps f3, f26, f2, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 832(x7)\n"
			"fcvt.wu.s x15, f20, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 800(x18)\n"
			"flw.ps f28, 64(x18)\n"
			"fmin.s f26, f20, f28\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_9:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 576(x15)\n"
			"flog.ps f28, f9\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 224(x6)\n"
			"fslli.pi f0, f27, 0x0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 96(x6)\n"
			"flw.ps f28, 224(x6)\n"
			"fsub.ps f12, f2, f28, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 160(x7)\n"
			"fcvt.ps.pwu f28, f20, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 320(x7)\n"
			"flw.ps f27, 768(x7)\n"
			"flw.ps f14, 896(x7)\n"
			"fnmadd.s f20, f9, f27, f14, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f26, 864(x23)\n"
			"fcvt.ps.pw f4, f26, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 32(x6)\n"
			"fclass.s x15, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 224(x18)\n"
			"flw.ps f2, 992(x18)\n"
			"fsgnjx.s f12, f10, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 576(x18)\n"
			"flw.ps f27, 480(x18)\n"
			"fsgnjn.ps f27, f30, f27\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 352(x19)\n"
			"fswizz.ps f18, f4, 0x1a\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 512(x19)\n"
			"flw.ps f3, 320(x19)\n"
			"fmax.ps f8, f4, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 160(x6)\n"
			"flw.ps f3, 64(x6)\n"
			"fsra.pi f24, f14, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 704(x19)\n"
			"flw.ps f14, 448(x19)\n"
			"fmul.s f12, f8, f14, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 832(x6)\n"
			"flw.ps f24, 640(x6)\n"
			"flw.ps f0, 928(x6)\n"
			"fmsub.s f3, f3, f24, f0, rdn\n" 
			"fbci.pi f28, 0xf500\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 288(x6)\n"
			"flw.ps f9, 608(x6)\n"
			"fcmovm.ps f24, f27, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 128(x6)\n"
			"flw.ps f14, 512(x6)\n"
			"fsgnj.ps f4, f18, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_9:\n"
			"maskxor m4, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 608(x7)\n"
			"flw.ps f27, 320(x7)\n"
			"fmax.ps f27, f30, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 864(x15)\n"
			"frsq.ps f30, f2\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 832(x18)\n"
			"flw.ps f3, 448(x18)\n"
			"fmul.pi f27, f26, f3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f30,  64 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fsetm.pi m6, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f31, 608(x26)\n"
			"flw.ps f30, 736(x26)\n"
			"fmulhu.pi f30, f31, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_10:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 416(x19)\n"
			"fexp.ps f31, f11\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_10:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 96(x17)\n"
			"fexp.ps f12, f23\n" 
			"la x17, rand_int_32\n"
			"flw.ps f4, 352(x17)\n"
			"flw.ps f11, 128(x17)\n"
			"fxor.pi f5, f4, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 864(x17)\n"
			"flw.ps f21, 352(x17)\n"
			"fsgnj.ps f11, f16, f21\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 480(x10)\n"
			"flw.ps f4, 384(x10)\n"
			"fsub.pi f5, f23, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 992(x15)\n"
			"fsqrt.s f5, f21, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 512(x7)\n"
			"fcvt.ps.pwu f27, f31, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 192(x14)\n"
			"fsgnjx.s f4, f30, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 480(x14)\n"
			"flw.ps f27, 800(x14)\n"
			"fmaxu.pi f8, f7, f27\n" 
			"li x5, 0xd9a2\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f11, 256(x23)\n"
			"fandi.pi f23, f11, 0x1fa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f11, 736(x12)\n"
			"flw.ps f3, 256(x12)\n"
			"fmulhu.pi f29, f11, f3\n" 
			"LBL_SEQID_0_FP_TRANS_ID_4_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 128(x26)\n"
			"fexp.ps f31, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 416(x15)\n"
			"flw.ps f31, 832(x15)\n"
			"fmul.s f12, f1, f31, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x10 << TENSOR_FMA16_SCP_LOC_B) | (0x2d << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f12, 768(x14)\n"
			"flw.ps f21, 864(x14)\n"
			"frem.pi f19, f12, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_10:\n"
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401A8\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 928(x7)\n"
			"flw.ps f19, 448(x7)\n"
			"fmax.s f23, f29, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 608(x12)\n"
			"flw.ps f7, 160(x12)\n"
			"fmul.s f29, f29, f7, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 576(x7)\n"
			"flw.ps f11, 384(x7)\n"
			"flw.ps f23, 992(x7)\n"
			"fnmsub.ps f30, f19, f11, f23, rne\n" 
			"li x12, 0xa67311c506f02c5b\n"
			"fcvt.s.w f3, x12, rup\n" 
			"li x9, 0xa4b7590cdf96d270\n"
			"fcvt.s.wu f8, x9, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 448(x17)\n"
			"flw.ps f23, 128(x17)\n"
			"fmulh.pi f8, f30, f23\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 288(x9)\n"
			"fpackreph.pi f8, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 640(x26)\n"
			"flw.ps f3, 32(x26)\n"
			"flw.ps f29, 64(x26)\n"
			"fmadd.s f7, f4, f3, f29, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 288(x14)\n"
			"flw.ps f29, 992(x14)\n"
			"fminu.pi f3, f21, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 192(x9)\n"
			"flw.ps f19, 32(x9)\n"
			"flw.ps f12, 832(x9)\n"
			"fmsub.s f3, f23, f19, f12, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 480(x23)\n"
			"flw.ps f12, 96(x23)\n"
			"flw.ps f3, 864(x23)\n"
			"fmsub.ps f7, f5, f12, f3, rtz\n" 
			"li x18, 0xcbc1a4e23b03603f\n"
			"fcvt.s.wu f1, x18, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_int_32\n"
			"flw.ps f30, 480(x23)\n"
			"flw.ps f16, 192(x23)\n"
			"fsub.pi f19, f30, f16\n" 
			"fbci.ps f31, 0x7e0e3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_640700_HID_10:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_850877_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xb1 << TENSOR_FMA16_SCP_LOC_B) | (0xa0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_494826_HID_10:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 576(x18)\n"
			"fsin.ps f3, f16\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 32(x14)\n"
			"flw.ps f12, 320(x14)\n"
			"fmulh.pi f7, f27, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 832(x12)\n"
			"fsrli.pi f19, f16, 0x6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 736(x10)\n"
			"flw.ps f30, 384(x10)\n"
			"fxor.pi f21, f23, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_10:\n"
			"masknot m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 352(x14)\n"
			"flw.ps f27, 672(x14)\n"
			"fle.ps f19, f31, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_10:\n"
			"feqm.ps m0, f31, f29\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f31, 0x5c6f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_10:\n"
			"fsetm.pi m7, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_10:\n"
			"feqm.ps m6, f23, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 64(x12)\n"
			"flw.ps f23, 864(x12)\n"
			"fmadd.ps f27, f8, f8, f23, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 832(x19)\n"
			"flw.ps f23, 960(x19)\n"
			"fmaxu.pi f4, f12, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 288(x18)\n"
			"flw.ps f4, 384(x18)\n"
			"fmadd.s f5, f3, f4, f4, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f11, 64(x15)\n"
			"fcvt.ps.pwu f3, f11, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 640(x26)\n"
			"flw.ps f31, 704(x26)\n"
			"fsgnjn.s f1, f7, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 192(x26)\n"
			"flw.ps f12, 768(x26)\n"
			"fsub.s f4, f21, f12, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
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
			"la x26, rand_int_32\n"
			"flw.ps f2, 704(x26)\n"
			"fandi.pi f30, f2, 0xde\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 32(x26)\n"
			"flw.ps f9, 0(x26)\n"
			"fsgnjx.ps f28, f19, f9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 224(x9)\n"
			"fsra.pi f19, f13, f13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 896(x9)\n"
			"flw.ps f24, 832(x9)\n"
			"fle.pi f17, f13, f24\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 32(x28)\n"
			"flw.ps f5, 736(x28)\n"
			"fsub.pi f9, f12, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 288(x6)\n"
			"flw.ps f0, 416(x6)\n"
			"fsgnj.ps f25, f19, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 928(x20)\n"
			"flw.ps f16, 704(x20)\n"
			"feq.ps f2, f18, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 992(x16)\n"
			"fcvt.ps.pwu f8, f28, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 128(x9)\n"
			"flw.ps f9, 608(x9)\n"
			"fand.pi f25, f16, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 608(x26)\n"
			"flw.ps f5, 64(x26)\n"
			"fsub.s f9, f12, f5, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 32(x28)\n"
			"flw.ps f14, 928(x28)\n"
			"fmulhu.pi f2, f5, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 768(x17)\n"
			"frsq.ps f17, f25\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f14, 224(x19)\n"
			"fcvt.ps.f16 f17, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 544(x9)\n"
			"fswizz.ps f14, f0, 0x11\n" 
			"mova.x.m x6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 512(x26)\n"
			"fpackrepb.pi f17, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f28, 704(x16)\n"
			"flw.ps f2, 288(x16)\n"
			"fremu.pi f19, f28, f2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 864(x19)\n"
			"ffrc.ps f25, f0\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 320(x20)\n"
			"fsat8.pi f8, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 96(x6)\n"
			"fclass.s x26, f25\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 928(x16)\n"
			"fsat8.pi f0, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_11:\n"
			"maskxor m0, m2, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 352(x5)\n"
			"fsgnjx.s f8, f28, f28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 704(x6)\n"
			"flw.ps f30, 704(x6)\n"
			"fsll.pi f13, f9, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 128(x5)\n"
			"flt.s x16, f25, f25\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_11:\n"
			"maskxor m0, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f12, 448(x16)\n"
			"flw.ps f19, 224(x16)\n"
			"fadd.pi f9, f12, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 992(x17)\n"
			"flw.ps f30, 768(x17)\n"
			"fsgnjn.s f25, f2, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 320(x6)\n"
			"flw.ps f18, 320(x6)\n"
			"flw.ps f0, 128(x6)\n"
			"fmadd.s f25, f30, f18, f0, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 896(x6)\n"
			"flw.ps f9, 352(x6)\n"
			"fmin.pi f14, f5, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 384(x26)\n"
			"flw.ps f30, 416(x26)\n"
			"fxor.pi f24, f16, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 128(x26)\n"
			"flw.ps f2, 320(x26)\n"
			"flt.s x20, f17, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 576(x10)\n"
			"fsin.ps f8, f18\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 960(x20)\n"
			"flw.ps f0, 0(x20)\n"
			"frem.pi f18, f17, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f25, 192(x10)\n"
			"fpackrepb.pi f19, f25\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_11:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 608(x19)\n"
			"fclass.s x6, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_11:\n"
			"maskor m4, m4, m2\n" 
			"li x5, 0x80340160\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f5, 64(x10)\n"
			"fsatu8.pi f14, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 896(x9)\n"
			"flw.ps f30, 768(x9)\n"
			"fdiv.ps f14, f14, f30, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 608(x20)\n"
			"fclass.s x9, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 480(x26)\n"
			"flw.ps f5, 832(x26)\n"
			"flt.s x17, f12, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 640(x19)\n"
			"flw.ps f12, 448(x19)\n"
			"flw.ps f9, 800(x19)\n"
			"fmsub.ps f14, f24, f12, f9, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f25, 512(x6)\n"
			"flw.ps f5, 736(x6)\n"
			"fle.pi f25, f25, f5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_11:\n"
			"masknot m1, m1\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 512(x26)\n"
			"flw.ps f2, 768(x26)\n"
			"fnmsub.s f25, f24, f2, f24, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 96(x23)\n"
			"ffrc.ps f13, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 256(x9)\n"
			"flw.ps f0, 544(x9)\n"
			"fle.s x17, f25, f0\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 928(x17)\n"
			"flw.ps f11, 0(x17)\n"
			"fsgnjx.ps f10, f3, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 800(x7)\n"
			"flw.ps f2, 992(x7)\n"
			"flw.ps f20, 96(x7)\n"
			"fnmadd.s f18, f29, f2, f20, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 704(x20)\n"
			"flw.ps f24, 224(x20)\n"
			"fmaxu.pi f5, f29, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 960(x14)\n"
			"fsrli.pi f2, f5, 0x2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 640(x20)\n"
			"flw.ps f2, 512(x20)\n"
			"fadd.pi f24, f9, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"fltm.ps m5, f3, f20\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x64f5e33918\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f23, 640(x9)\n"
			"flw.ps f11, 384(x9)\n"
			"flt.pi f1, f23, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 384(x17)\n"
			"flw.ps f10, 768(x17)\n"
			"fsgnj.s f18, f18, f10\n" 
			"mova.x.m x5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 800(x14)\n"
			"flw.ps f9, 768(x14)\n"
			"flw.ps f18, 832(x14)\n"
			"fnmadd.s f5, f2, f9, f18, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 384(x10)\n"
			"flw.ps f13, 448(x10)\n"
			"fadd.ps f1, f1, f13, rdn\n" 
			"fbci.ps f24, 0x2b7bb\n" 
			"la x10, rand_int_32\n"
			"flw.ps f17, 160(x10)\n"
			"faddi.pi f18, f17, 0x70\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 160(x5)\n"
			"flw.ps f3, 448(x5)\n"
			"fsgnjn.s f29, f11, f3\n" 
			"li x5, 0xd19b\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f1, 928(x11)\n"
			"flw.ps f11, 960(x11)\n"
			"fxor.pi f1, f1, f11\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 160(x10)\n"
			"fadd.pi f3, f15, f15\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_285966_HID_12:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x2 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1a << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x4 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x19 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x6 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xf << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x0 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x10, rand_int_32\n"
			"flw.ps f3, 64(x10)\n"
			"fandi.pi f9, f3, 0x115\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 608(x7)\n"
			"fcvt.wu.s x10, f8, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_12:\n"
			"maskor m5, m5, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x616f3b4b2a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 640(x7)\n"
			"flw.ps f8, 0(x7)\n"
			"fsgnjx.ps f18, f24, f8\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 576(x13)\n"
			"flw.ps f13, 224(x13)\n"
			"fltu.pi f11, f29, f13\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 192(x13)\n"
			"fsqrt.ps f15, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 832(x14)\n"
			"fcvt.w.s x11, f8, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 992(x5)\n"
			"flw.ps f17, 544(x5)\n"
			"flt.s x7, f8, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 832(x18)\n"
			"fclass.ps f24, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"fltm.ps m7, f24, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 64(x18)\n"
			"flw.ps f1, 416(x18)\n"
			"fmsub.s f3, f9, f9, f1, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 448(x20)\n"
			"fsat8.pi f13, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"maskxor m5, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f9, 736(x18)\n"
			"flw.ps f1, 416(x18)\n"
			"fmax.pi f18, f9, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 224(x11)\n"
			"flw.ps f8, 928(x11)\n"
			"fsgnjn.ps f17, f24, f8\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_12:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 256(x11)\n"
			"flog.ps f1, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x5 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x19 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xe << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x3 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x4 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xa << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f20, 800(x10)\n"
			"flw.ps f23, 896(x10)\n"
			"fmax.pi f8, f20, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f1, 160(x13)\n"
			"flw.ps f8, 256(x13)\n"
			"fmin.pi f23, f1, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 640(x17)\n"
			"fsin.ps f29, f2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 384(x5)\n"
			"flw.ps f18, 192(x5)\n"
			"flw.ps f1, 224(x5)\n"
			"fmsub.ps f15, f29, f18, f1, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f15, 288(x20)\n"
			"flw.ps f10, 768(x20)\n"
			"fand.pi f3, f15, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"feqm.ps m7, f13, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f24, 512(x11)\n"
			"faddi.pi f10, f24, 0x52\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 704(x14)\n"
			"flw.ps f18, 640(x14)\n"
			"fsub.pi f5, f29, f18\n" 
			"fbci.pi f9, 0x3085b\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_12:\n"
			"fltm.pi m7, f8, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_12:\n"
			"feqm.ps m3, f11, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_12:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 992(x18)\n"
			"fexp.ps f24, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 32(x22)\n"
			"flw.ps f17, 32(x22)\n"
			"fmin.pi f3, f1, f17\n" 
			"li x5, 0x62047ec4c78016cd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd7c6274d2024ed86\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f11, 736(x22)\n"
			"fsrli.pi f15, f11, 0xd\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 704(x14)\n"
			"flw.ps f17, 640(x14)\n"
			"fxor.pi f24, f1, f17\n" 
			"maskpopc x11, m2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 768(x10)\n"
			"fcvt.wu.s x19, f14, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"maskxor m3, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 0(x18)\n"
			"fcvt.f16.ps f26, f25\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 736(x18)\n"
			"flw.ps f15, 960(x18)\n"
			"fadd.ps f25, f14, f15, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f27, 320(x18)\n"
			"fmax.pi f5, f27, f27\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_13:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 256(x20)\n"
			"frcp.ps f15, f31\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 448(x23)\n"
			"flw.ps f11, 736(x23)\n"
			"fmaxu.pi f11, f27, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 480(x9)\n"
			"flw.ps f6, 672(x9)\n"
			"fsub.ps f30, f5, f6, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 544(x18)\n"
			"flw.ps f8, 256(x18)\n"
			"fsub.ps f5, f0, f8, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 960(x23)\n"
			"frsq.ps f11, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"flem.ps m4, f10, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f0, 544(x7)\n"
			"flw.ps f1, 416(x7)\n"
			"fxor.pi f10, f0, f1\n" 
			"li x12, 0x9d9385dbbdfcc2fc\n"
			"fcvt.s.wu f31, x12, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_13:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 192(x20)\n"
			"fexp.ps f0, f5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 96(x14)\n"
			"faddi.pi f5, f2, 0x1f7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 480(x23)\n"
			"fcvt.w.s x11, f11, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 832(x12)\n"
			"fclass.ps f1, f5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 672(x20)\n"
			"fcvt.pwu.ps f11, f6, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 800(x23)\n"
			"flw.ps f25, 736(x23)\n"
			"flw.ps f8, 480(x23)\n"
			"fmadd.s f2, f15, f25, f8, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"fltm.pi m6, f31, f5\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"mov.m.x m7, x14, 0x2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f26, 864(x12)\n"
			"fpackreph.pi f1, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 640(x14)\n"
			"flw.ps f0, 736(x14)\n"
			"fsgnjn.s f27, f5, f0\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 192(x7)\n"
			"flw.ps f2, 96(x7)\n"
			"fsgnjn.s f11, f5, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 0(x7)\n"
			"flw.ps f6, 96(x7)\n"
			"fsrl.pi f5, f27, f6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 0(x26)\n"
			"faddi.pi f30, f14, 0xa\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 608(x9)\n"
			"flw.ps f14, 320(x9)\n"
			"fle.pi f27, f1, f14\n" 
			"li x7, 0x1c54e343a39b6f81\n"
			"fcvt.s.wu f25, x7, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 384(x20)\n"
			"frsq.ps f15, f2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 64(x9)\n"
			"flw.ps f26, 160(x9)\n"
			"for.pi f8, f27, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 416(x9)\n"
			"fremu.pi f26, f5, f5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 608(x23)\n"
			"flw.ps f27, 832(x23)\n"
			"fadd.s f10, f30, f27, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 480(x20)\n"
			"fsqrt.s f1, f6, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 928(x7)\n"
			"flw.ps f30, 64(x7)\n"
			"fmax.ps f5, f27, f30\n" 
			"li x19, 0xcbcf6175f4961073\n"
			"fcvt.s.lu f8, x19, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 544(x26)\n"
			"flw.ps f14, 128(x26)\n"
			"fmin.ps f6, f2, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"fsetm.pi m7, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 544(x14)\n"
			"flw.ps f1, 288(x14)\n"
			"fnmsub.s f31, f31, f1, f1, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 992(x18)\n"
			"flw.ps f14, 576(x18)\n"
			"flw.ps f11, 224(x18)\n"
			"fmadd.ps f15, f15, f14, f11, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 320(x14)\n"
			"flw.ps f25, 320(x14)\n"
			"fand.pi f10, f1, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 448(x9)\n"
			"flw.ps f8, 224(x9)\n"
			"flw.ps f11, 800(x9)\n"
			"fmadd.s f14, f10, f8, f11, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 832(x14)\n"
			"fsqrt.ps f6, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 992(x20)\n"
			"fcvt.pw.ps f5, f1, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 960(x9)\n"
			"fcvt.wu.s x10, f0, rdn\n" 
			"li x14, 0xa47d7251f93e89b0\n"
			"fcvt.s.wu f5, x14, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 224(x9)\n"
			"flw.ps f30, 416(x9)\n"
			"fmul.ps f30, f6, f30, rdn\n" 
			"fbci.pi f2, 0x6f8e3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 128(x10)\n"
			"fcvt.wu.s x9, f6, rtz\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 480(x22)\n"
			"flt.s x5, f22, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 128(x22)\n"
			"fclass.s x18, f29\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 512(x16)\n"
			"flw.ps f11, 32(x16)\n"
			"fmul.pi f29, f5, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 864(x9)\n"
			"fclass.ps f7, f7\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 352(x17)\n"
			"frcp.ps f23, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"feqm.ps m6, f5, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f22, 384(x9)\n"
			"fcvt.ps.pw f29, f22, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 736(x15)\n"
			"fsin.ps f2, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 480(x17)\n"
			"flw.ps f11, 160(x17)\n"
			"flw.ps f16, 640(x17)\n"
			"fnmsub.ps f10, f7, f11, f16, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f2, 128(x11)\n"
			"flw.ps f7, 544(x11)\n"
			"fmulhu.pi f2, f2, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 768(x17)\n"
			"fsgnjx.s f22, f0, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 928(x18)\n"
			"flw.ps f4, 192(x18)\n"
			"fmaxu.pi f22, f29, f4\n" 
			"maskpopc x11, m1\n" 
			"li x5, 0xc4bc\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f14, 0x64bbb\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 704(x15)\n"
			"fround.ps f28, f19, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f2, 512(x11)\n"
			"flw.ps f10, 832(x11)\n"
			"fle.pi f19, f2, f10\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f0, 800(x11)\n"
			"flw.ps f11, 32(x11)\n"
			"fltu.pi f10, f0, f11\n" 
			"maskpopcz x15, m4\n" 
			"li x5, 0xb8f7\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 192(x15)\n"
			"flw.ps f11, 64(x15)\n"
			"feq.s x7, f19, f11\n" 
			"fbci.ps f22, 0x2d8ac\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 832(x16)\n"
			"flw.ps f5, 736(x16)\n"
			"fmulhu.pi f9, f7, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 128(x22)\n"
			"fclass.ps f10, f0\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 736(x15)\n"
			"flw.ps f28, 992(x15)\n"
			"fsll.pi f9, f29, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 576(x18)\n"
			"frsq.ps f2, f16\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 416(x5)\n"
			"fcvt.ps.pw f2, f23, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 0(x15)\n"
			"flw.ps f9, 768(x15)\n"
			"fremu.pi f21, f28, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f5, 512(x7)\n"
			"flw.ps f7, 960(x7)\n"
			"fmul.pi f2, f5, f7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 384(x23)\n"
			"flw.ps f10, 768(x23)\n"
			"fsgnjn.s f21, f23, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 128(x15)\n"
			"flw.ps f9, 544(x15)\n"
			"fsgnj.ps f19, f22, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 224(x7)\n"
			"fclass.ps f2, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 576(x18)\n"
			"flw.ps f22, 256(x18)\n"
			"feq.ps f29, f4, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5e << TENSOR_FMA16_SCP_LOC_B) | (0x16 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_14:\n"
			"fltm.pi m6, f11, f9\n" 
			"li x5, 0x80340188\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f28, 224(x16)\n"
			"flw.ps f2, 960(x16)\n"
			"fltu.pi f0, f28, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 640(x23)\n"
			"flw.ps f0, 896(x23)\n"
			"flt.pi f29, f19, f0\n" 
			"fbci.ps f28, 0x159f3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_14:\n"
			"mov.m.x m6, x16, 0x7a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 384(x18)\n"
			"flw.ps f9, 0(x18)\n"
			"flw.ps f22, 0(x18)\n"
			"fcmov.ps f2, f29, f9, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 960(x23)\n"
			"fsin.ps f10, f4\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 32(x16)\n"
			"fcvt.f16.ps f21, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 320(x9)\n"
			"flw.ps f28, 992(x9)\n"
			"fsgnjn.ps f29, f19, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 32(x18)\n"
			"flw.ps f10, 192(x18)\n"
			"fmin.s f11, f22, f10\n" 
			"li x5, 0xe00c\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f5, 160(x17)\n"
			"fpackrepb.pi f2, f5\n" 
			"maskpopcz x11, m6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 704(x23)\n"
			"flw.ps f7, 960(x23)\n"
			"fmin.pi f22, f10, f7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f10, 96(x15)\n"
			"fpackrepb.pi f29, f10\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 0(x9)\n"
			"fmin.pi f28, f29, f29\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 480(x16)\n"
			"fcvt.ps.pw f28, f10, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_538356_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 0xa7a6\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbb << TENSOR_IMA_SCP_LOC_B) | (0x99 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 11\n"   
			"csrwi mhpmevent6, 5\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f27, 672(x15)\n"
			"feq.pi f28, f27, f27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 352(x16)\n"
			"fnot.pi f4, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 448(x18)\n"
			"fsin.ps f20, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 32(x15)\n"
			"flw.ps f3, 96(x15)\n"
			"fminu.pi f18, f27, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 416(x11)\n"
			"frsq.ps f31, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 736(x16)\n"
			"fcvt.f16.ps f4, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 0(x13)\n"
			"fsrai.pi f31, f18, 0x6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"fltm.ps m6, f27, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 832(x12)\n"
			"flw.ps f20, 192(x12)\n"
			"flw.ps f3, 576(x12)\n"
			"fmadd.ps f14, f30, f20, f3, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 288(x13)\n"
			"flw.ps f15, 96(x13)\n"
			"fsll.pi f31, f24, f15\n" 
			"la x20, rand_int_32\n"
			"flw.ps f14, 832(x20)\n"
			"flw.ps f24, 800(x20)\n"
			"fand.pi f4, f14, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 864(x22)\n"
			"fcvt.pwu.ps f8, f18, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 64(x9)\n"
			"flw.ps f18, 224(x9)\n"
			"fminu.pi f20, f28, f18\n" 
			"maskpopcz x20, m5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 640(x13)\n"
			"flw.ps f22, 960(x13)\n"
			"fmin.pi f20, f31, f22\n" 
			"maskpopc x9, m5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"maskpopcz x10, m2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 608(x22)\n"
			"flw.ps f31, 256(x22)\n"
			"fmax.s f30, f22, f31\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 704(x22)\n"
			"flw.ps f0, 800(x22)\n"
			"fltu.pi f0, f4, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 416(x13)\n"
			"flw.ps f14, 576(x13)\n"
			"fadd.ps f7, f24, f14, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 864(x12)\n"
			"fsatu8.pi f23, f20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 608(x11)\n"
			"fnot.pi f18, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_15:\n"
			"mov.m.x m6, x16, 0x34\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f23, 704(x12)\n"
			"fpackreph.pi f22, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 992(x10)\n"
			"fsra.pi f0, f4, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 864(x22)\n"
			"flw.ps f23, 864(x22)\n"
			"fle.ps f23, f7, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"fltm.pi m2, f27, f31\n" 
			"li x5, 0x80340190\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 928(x10)\n"
			"fcvt.pw.ps f14, f18, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 0(x16)\n"
			"fround.ps f27, f28, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 160(x16)\n"
			"fcvt.w.s x13, f24, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 32(x13)\n"
			"flw.ps f3, 992(x13)\n"
			"fmulhu.pi f20, f4, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 736(x12)\n"
			"flw.ps f3, 128(x12)\n"
			"fnmadd.s f8, f22, f22, f3, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 352(x12)\n"
			"flw.ps f3, 160(x12)\n"
			"flw.ps f18, 0(x12)\n"
			"fnmadd.s f18, f14, f3, f18, rup\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_15:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 896(x20)\n"
			"frcp.ps f18, f22\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 928(x11)\n"
			"fmax.pi f4, f30, f30\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_15:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 512(x22)\n"
			"fexp.ps f15, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 192(x22)\n"
			"fcvt.f16.ps f15, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 96(x16)\n"
			"flw.ps f4, 256(x16)\n"
			"fmul.pi f28, f24, f4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 928(x15)\n"
			"flw.ps f30, 512(x15)\n"
			"feq.pi f24, f4, f30\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 960(x16)\n"
			"flw.ps f8, 64(x16)\n"
			"fsra.pi f4, f7, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 928(x16)\n"
			"flw.ps f14, 32(x16)\n"
			"fsgnjx.ps f14, f30, f14\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 128(x13)\n"
			"fsqrt.s f20, f28, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 832(x20)\n"
			"fcvt.pw.ps f14, f23, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"maskand m6, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f15, 96(x9)\n"
			"fsrai.pi f0, f15, 0x2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_15:\n"
			"fsetm.pi m5, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 128(x20)\n"
			"flw.ps f23, 992(x20)\n"
			"fsub.ps f31, f24, f23, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_15:\n"
			"feqm.ps m0, f14, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
		0x69560c06, 
		0x312a4c3c, 
		0x64119d38, 
		0xf60ce881, 
		0x855a7d8d, 
		0xb9534807, 
		0x6f1d5baf, 
		0x9e203161, 
		0x70fe3cfc, 
		0x3e332f35, 
		0xd7f14ec2, 
		0x3246b7d9, 
		0x0b2591c4, 
		0x2258edf3, 
		0xaf3cb9d0, 
		0xfd05a5f5, 
		0x5690f74d, 
		0xaf797546, 
		0x29c836e0, 
		0x702a5b67, 
		0xa9c9a24e, 
		0x77f4531b, 
		0x258694b6, 
		0xc110412c, 
		0xc06bad38, 
		0x8a647b9f, 
		0xe46232e7, 
		0xb44788bd, 
		0x05727bb5, 
		0x3513198c, 
		0xda98da57, 
		0x7d5d9031, 
		0x8986148f, 
		0x03ea038f, 
		0xc1d7fbe9, 
		0x4fe4b735, 
		0xbeb9c9e2, 
		0x833cf90f, 
		0x2743e0df, 
		0x42260a44, 
		0x97f0bfcf, 
		0x38330558, 
		0x56fb5c72, 
		0x16c7354a, 
		0x27a0008c, 
		0x9045a445, 
		0x7abc9f0b, 
		0x51c6a2e4, 
		0x0637fdca, 
		0xf64e8f4a, 
		0xc2fab184, 
		0xe299cb48, 
		0x6e05724d, 
		0x45a16970, 
		0x62614790, 
		0x5804a881, 
		0x8abb572c, 
		0x8d5f52c0, 
		0xdcebef56, 
		0x54e27ea6, 
		0xfec24805, 
		0x1152797e, 
		0x920c904e, 
		0x854ca896, 
		0x961139cf, 
		0x7c3ad037, 
		0xc82a6c15, 
		0x58ea0417, 
		0xb5178761, 
		0xfedcb8f9, 
		0xf63d307c, 
		0x66270136, 
		0xbecba4c8, 
		0x5931c9b4, 
		0xda2e6e6c, 
		0x37503d77, 
		0xd4062cce, 
		0xfbcefcd0, 
		0xc503ccb5, 
		0xd1bfbaba, 
		0xf33c4233, 
		0x08fce7af, 
		0x9c993735, 
		0x1d5a474c, 
		0xb63c1dd8, 
		0xfaa2cbad, 
		0x72f78920, 
		0x68bfbf7e, 
		0xfabbc6b6, 
		0xff283550, 
		0x2c94151d, 
		0xb2e02961, 
		0x080638c9, 
		0xd5e32aef, 
		0x44f06c4e, 
		0xa94501d7, 
		0xfaaf500c, 
		0xb55dc442, 
		0x23d83f22, 
		0x98decbf6, 
		0xc9345ce7, 
		0x6b291a13, 
		0x951a0518, 
		0x1f7550bc, 
		0x5fdd5329, 
		0x854df9b6, 
		0xced4dd7c, 
		0x623a35ae, 
		0x3d0daad0, 
		0x4eec95b0, 
		0xd3100ac5, 
		0xe775f27e, 
		0xf5d61476, 
		0xc0ff34b4, 
		0x86861f29, 
		0x3f1cd31b, 
		0x481ce3f9, 
		0x677abeb7, 
		0x0ef05b6f, 
		0x5b7801ea, 
		0x3fccb2c1, 
		0x25323fd3, 
		0x06f9f4e6, 
		0xf190cb53, 
		0x8f5a5ade, 
		0xbdf2b5a1, 
		0xb8d13539, 
		0xd8efb5c9, 
		0x0ff73f25, 
		0x2af595a3, 
		0x816a5ea8, 
		0x3665b6a5, 
		0x7e4132ad, 
		0x14ed05bd, 
		0xfbdc678a, 
		0xbee5f64d, 
		0x65739938, 
		0xb9e7f780, 
		0xdaace3a3, 
		0x723dc8f5, 
		0x827acd0d, 
		0x25d203e3, 
		0xaab6941b, 
		0xf6b4160b, 
		0xb0335368, 
		0x2234c40f, 
		0x659badcb, 
		0x793910e6, 
		0x70bc3ec1, 
		0xe9bf79f4, 
		0x6175f16a, 
		0x94291726, 
		0x80635ef2, 
		0xd7e23908, 
		0x3055d84e, 
		0x3765002d, 
		0x83add758, 
		0x8278e1e5, 
		0x577aa515, 
		0x6ef4d6b4, 
		0xbac114b5, 
		0xf7de1a61, 
		0x119b0fb4, 
		0xdfa2e52b, 
		0xcfd09276, 
		0x0438b3d4, 
		0xedaeec39, 
		0xd273dcbe, 
		0x5acecdbf, 
		0x9502e1d5, 
		0xc8359b91, 
		0xb311d9c4, 
		0xc4a0c1cd, 
		0x881d0ee1, 
		0xebbcf1f6, 
		0x32620328, 
		0x2dc6c2ac, 
		0xa878e876, 
		0x817131eb, 
		0xdc7843b3, 
		0xe7eb533c, 
		0x0ddd49b7, 
		0xee4b0498, 
		0x80a17176, 
		0x6b58996c, 
		0x9e7f0271, 
		0xb6e4d16e, 
		0x235f6be1, 
		0x463e4336, 
		0xfa2225d7, 
		0x54a1396c, 
		0x13f1484a, 
		0xb6fece9a, 
		0x9ba66e98, 
		0xec7b009c, 
		0xc42b43fa, 
		0xc5fa8ac9, 
		0xd6b6b5d0, 
		0x7b768615, 
		0xc3ac232c, 
		0x43215bf4, 
		0xc8fe0b2d, 
		0xa2a1fa45, 
		0x13218ef7, 
		0x1f742e70, 
		0xfa4abdec, 
		0xe2cbef40, 
		0xd1de4fa8, 
		0xfc7aad95, 
		0x0c780f2f, 
		0x07117e78, 
		0x663eab2b, 
		0xb40eb501, 
		0x681d30de, 
		0xd63a7486, 
		0xf49eb5a6, 
		0xd4ae3486, 
		0x9398dbd6, 
		0x49adce95, 
		0x40b1f64d, 
		0x48df480e, 
		0xc5cf255f, 
		0x81de9808, 
		0x550c54d4, 
		0x25365ca8, 
		0x1004d975, 
		0x5f7e07d3, 
		0x40fd8aaa, 
		0x7c34e6eb, 
		0x1598734a, 
		0x50c67350, 
		0x3825a7e1, 
		0xa4efedef, 
		0x82edc5ab, 
		0x120a202f, 
		0x27e8aeae, 
		0x90b15e3d, 
		0xaa0cc327, 
		0x863c6992, 
		0x90b6e394, 
		0xba13834f, 
		0xd09e1abb, 
		0x586076e3, 
		0xfa29559d, 
		0x83278f64, 
		0x13b2cc15, 
		0x1c67556d, 
		0x9e76ef95, 
		0x8bb62aff, 
		0x5bd38392, 
		0xf146b080, 
		0x2e0885fc, 
		0x44ee5071, 
		0xf26ce972, 
		0xa78c1cda, 
		0xe31b9123  
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
		0x46a3, 
		0xd160, 
		0xce80, 
		0xce00, 
		0x5dd2, 
		0xcd50, 
		0xc880, 
		0xa17a, 
		0x50a0, 
		0x4ec0, 
		0xd380, 
		0xc500, 
		0xd15a, 
		0x9761, 
		0x77b1, 
		0x53a0, 
		0xce40, 
		0xca80, 
		0xcb00, 
		0xcec0, 
		0xc600, 
		0xa60d, 
		0x51a0, 
		0x5100, 
		0x0000, 
		0xd360, 
		0xcb00, 
		0xd1a0, 
		0x4527, 
		0xf2b2, 
		0x743d, 
		0xd3c0, 
		0xd040, 
		0xcb80, 
		0xfc00, 
		0xa3c9, 
		0x03fc, 
		0x036c, 
		0xfc00, 
		0x0015, 
		0xd0a0, 
		0xd140, 
		0x5c84, 
		0x7c00, 
		0x50a0, 
		0xd2a0, 
		0xca00, 
		0x4c40, 
		0xd220, 
		0x5020, 
		0x1c51, 
		0xd260, 
		0x725b, 
		0x5180, 
		0x0f98, 
		0x7493, 
		0xcfc0, 
		0x8000, 
		0x4c80, 
		0x7c04, 
		0xd340, 
		0xcd80, 
		0x5340, 
		0xcd80, 
		0x4f40, 
		0x31f6, 
		0x51a0, 
		0x4992, 
		0x4d80, 
		0x4b00, 
		0x02dd, 
		0x4c80, 
		0x5020, 
		0x7d0c, 
		0xcfc0, 
		0x5020, 
		0xd3a0, 
		0x5200, 
		0x8000, 
		0xcfc0, 
		0x5424, 
		0xcb80, 
		0xc200, 
		0x1d91, 
		0x4a00, 
		0x629e, 
		0x4400, 
		0x4f40, 
		0x4a00, 
		0xf751, 
		0x405e, 
		0xd2e0, 
		0xc000, 
		0xcb00, 
		0xc700, 
		0xfc00, 
		0xd686, 
		0x8000, 
		0xd3c0, 
		0x3cdd, 
		0x21d1, 
		0x4980, 
		0xaef4, 
		0x6dfd, 
		0xcb80, 
		0x0372, 
		0x5280, 
		0xd1c0, 
		0x53c0, 
		0x4c80, 
		0x454a, 
		0xce00, 
		0x8000, 
		0x4e40, 
		0xc200, 
		0x8893, 
		0x4880, 
		0xcc40, 
		0x30a0, 
		0xe0d9, 
		0xd320, 
		0x4c40, 
		0xd2c0, 
		0x2b01, 
		0xc900, 
		0xce00, 
		0xccc0, 
		0x4a00, 
		0x72d8, 
		0x834a, 
		0xcfc0, 
		0xa281, 
		0xcdc0, 
		0x7c00, 
		0x16d4, 
		0x0c6d, 
		0x4a00, 
		0xcc00, 
		0xc900, 
		0xdc4d, 
		0xd140, 
		0x4700, 
		0x5140, 
		0xcc80, 
		0x5220, 
		0xd3c0, 
		0xcd00, 
		0x4ec0, 
		0x00fe, 
		0xf00b, 
		0xdd54, 
		0xfd32, 
		0x5300, 
		0x771a, 
		0x4900, 
		0x4c80, 
		0x4200, 
		0xd380, 
		0x467d, 
		0x9b5a, 
		0x5040, 
		0xd180, 
		0xcec0, 
		0x4e40, 
		0x9670, 
		0x1d5c, 
		0xcec0, 
		0x294b, 
		0xe7ba, 
		0xd380, 
		0x66e3, 
		0x5300, 
		0xce00, 
		0x52c0, 
		0x5280, 
		0xc980, 
		0xd3e0, 
		0xc800, 
		0x4c00, 
		0x3c00, 
		0xb68d, 
		0x803b, 
		0x261c, 
		0xc800, 
		0x0460, 
		0xd020, 
		0x5320, 
		0x4fc0, 
		0xd220, 
		0xbd9a, 
		0x4a80, 
		0x4a80, 
		0xcf40, 
		0x50e0, 
		0x922e, 
		0x7b59, 
		0xce00, 
		0x5300, 
		0x4d40, 
		0x5020, 
		0xc984, 
		0xd360, 
		0x4a00, 
		0x4900, 
		0x618a, 
		0x5360, 
		0xfc00, 
		0x52c0, 
		0x8000, 
		0xc542, 
		0xcc40, 
		0xa71b, 
		0x4dc0, 
		0xd280, 
		0x5120, 
		0x8000, 
		0xcc00, 
		0x11d7, 
		0x7c00, 
		0xd120, 
		0xd340, 
		0x41a8, 
		0xd0c0, 
		0x51e0, 
		0x4e40, 
		0x16a4, 
		0xcf40, 
		0xfc00, 
		0x4900, 
		0x4000, 
		0x9e78, 
		0xd160, 
		0x8fef, 
		0x4980, 
		0x5200, 
		0xa9f1, 
		0xc700, 
		0xcc40, 
		0xcc80, 
		0xcdc0, 
		0xfbe0, 
		0xcf00, 
		0xa9ad, 
		0xb9c2, 
		0x50e0, 
		0xce00, 
		0xd3e0, 
		0x4f40, 
		0x4200, 
		0x6f91, 
		0x0000, 
		0xd320, 
		0x0000, 
		0x59ba, 
		0x50a0, 
		0x5300, 
		0xd360, 
		0x0000, 
		0x4343, 
		0xb422, 
		0x8c3e, 
		0x553d, 
		0xb82a, 
		0xd0c0, 
		0x51e0, 
		0x50a0, 
		0x4700, 
		0x4400, 
		0x0000, 
		0xcc8f, 
		0xd340, 
		0x7c4f, 
		0x0000, 
		0xd140, 
		0xd300, 
		0x42df, 
		0xd3e0, 
		0x717e, 
		0x5607, 
		0xcf80, 
		0x757d, 
		0xa244, 
		0x026c, 
		0xd1a0, 
		0xc57f, 
		0x3c00, 
		0x4c80, 
		0xcfc0, 
		0x1351, 
		0x2745, 
		0xd200, 
		0x4d80, 
		0xd462, 
		0x7c00, 
		0xd477, 
		0xe46a, 
		0xd080, 
		0xc800, 
		0x4400, 
		0x0bde, 
		0xf90d, 
		0xd000, 
		0xcf40, 
		0x5140, 
		0x22a0, 
		0xfbce, 
		0x1f7d, 
		0x8000, 
		0xd120, 
		0x8000, 
		0x4200, 
		0xf59f, 
		0xc980, 
		0x3f81, 
		0xb078, 
		0x8000, 
		0x4dc0, 
		0xdb7a, 
		0xcb80, 
		0xfc00, 
		0x59b8, 
		0x5300, 
		0xdfbc, 
		0xd2a0, 
		0x8000, 
		0x24b5, 
		0x5000, 
		0xc1dc, 
		0x5000, 
		0xe124, 
		0xcb00, 
		0x8000, 
		0x7c00, 
		0x5040, 
		0x3a39, 
		0xd340, 
		0xcf00, 
		0x4200, 
		0xc5c8, 
		0x516d, 
		0x50e0, 
		0x4800, 
		0x4a00, 
		0xd0a0, 
		0x2f63, 
		0xd1e0, 
		0xc15e, 
		0x7c00, 
		0x8000, 
		0xab94, 
		0xbc00, 
		0x4c80, 
		0xd300, 
		0x4e00, 
		0x4200, 
		0x1684, 
		0xc880, 
		0xc880, 
		0x4900, 
		0x51a0, 
		0xd220, 
		0xd2a0, 
		0x4500, 
		0x4a80, 
		0xcc00, 
		0xcd77, 
		0xd060, 
		0xcd40, 
		0xd4dc, 
		0x52a0, 
		0xd320, 
		0x95b8, 
		0xd040, 
		0xcc40, 
		0xe88d, 
		0x5120, 
		0x50a0, 
		0x5160, 
		0x4d00, 
		0xd3a0, 
		0xcec0, 
		0xd0e0, 
		0x53e0, 
		0x4a00, 
		0x8ea5, 
		0x4d00, 
		0xd300, 
		0xcd40, 
		0x2f0a, 
		0x50e0, 
		0xd1c0, 
		0x0000, 
		0x723c, 
		0x5380, 
		0x4900, 
		0xfc00, 
		0x605c, 
		0x5360, 
		0x7874, 
		0xcc40, 
		0xd160, 
		0xcb00, 
		0x5080, 
		0xc900, 
		0x036a, 
		0xc5d0, 
		0xd73c, 
		0x5020, 
		0xd2c0, 
		0xd140, 
		0x028d, 
		0xae11, 
		0xd0e0, 
		0x9566, 
		0x53e0, 
		0xd2e0, 
		0x4800, 
		0xd2c0, 
		0xb302, 
		0x4b80, 
		0x4c40, 
		0x4f80, 
		0xfca5, 
		0xe530, 
		0xcc00, 
		0xc632, 
		0x9871, 
		0x5340, 
		0xfc00, 
		0x5100, 
		0x4095, 
		0x8000, 
		0x8000, 
		0xc800, 
		0xd1c0, 
		0xd040, 
		0x4700, 
		0xd2a0, 
		0xcd80, 
		0xd060, 
		0xcdc0, 
		0xd040, 
		0xcf80, 
		0xd260, 
		0xf5ad, 
		0x5240, 
		0x0000, 
		0x8000, 
		0x8000, 
		0xd080, 
		0x4800, 
		0xf52e, 
		0xc152, 
		0xc200, 
		0x5300, 
		0xf99e, 
		0x6fa4, 
		0xbaf6, 
		0x1fce, 
		0xcc00, 
		0x4880, 
		0xbc00, 
		0x11ed, 
		0x52e0, 
		0x4b00, 
		0xc200, 
		0xce80, 
		0x0389, 
		0x5570, 
		0x4a00, 
		0x5200, 
		0xd0e6, 
		0xfc00, 
		0x4c40, 
		0xd180, 
		0xd000, 
		0xd320, 
		0x272c, 
		0x5d59, 
		0x0196, 
		0x4700, 
		0x5160, 
		0xd240, 
		0xd0e0, 
		0xd2e0, 
		0xca00, 
		0x5140, 
		0x53a0, 
		0xfc00, 
		0x5220, 
		0x8000, 
		0x4d40, 
		0xc800, 
		0xd320, 
		0x8000, 
		0x5afa, 
		0xd1a0, 
		0x8000, 
		0x5360, 
		0x4200, 
		0xd1c0, 
		0xd100, 
		0xd100, 
		0x4c00, 
		0x5140, 
		0xcb00, 
		0x53c0, 
		0x8000, 
		0x681c, 
		0xd1a0, 
		0xd340, 
		0x7308, 
		0x6fd7, 
		0x4c00, 
		0xff41, 
		0x4980, 
		0x4b00, 
		0x53a0, 
		0x2d0f, 
		0xca00, 
		0x5300, 
		0x8000, 
		0x52e0, 
		0xd220, 
		0x5f34, 
		0x4ec0, 
		0x9ae8, 
		0x32f2, 
		0x4cc0, 
		0xd100, 
		0xcec0, 
		0xdbd0, 
		0xd140, 
		0xd2e0, 
		0x5280, 
		0x4880, 
		0x7cde, 
		0xd000, 
		0xd3e0, 
		0x033e, 
		0xd060, 
		0x8000, 
		0x404c, 
		0xcd00, 
		0xca80, 
		0x4d40, 
		0x957e, 
		0x8000, 
		0x064c, 
		0x51a0, 
		0x5140, 
		0xd140, 
		0x4c99, 
		0x4400, 
		0x8000, 
		0x52e0, 
		0xd140, 
		0xc880, 
		0x50a0, 
		0x4d80, 
		0x582d, 
		0x4c00, 
		0x5220, 
		0x0777, 
		0x5240, 
		0xd120, 
		0x4d40, 
		0x4200, 
		0xfa0f, 
		0xca80, 
		0xcdc0, 
		0x52a0, 
		0x7c00, 
		0x5160, 
		0xcf80, 
		0xc880, 
		0x4ec0, 
		0x4e40, 
		0x50a0, 
		0xe894, 
		0xc900, 
		0xb731, 
		0xd020, 
		0x7c00, 
		0xd0a0, 
		0x5280, 
		0x767e, 
		0x1e0e, 
		0x53c0, 
		0xc900, 
		0x0000, 
		0xd220, 
		0x50c0, 
		0x5100, 
		0x4c00, 
		0x7c00, 
		0xe728, 
		0x4c40, 
		0xc000, 
		0xcf00, 
		0xcd80, 
		0xe500, 
		0x586c, 
		0xed2d, 
		0x5020, 
		0x9fab, 
		0x4f40, 
		0xd3c0, 
		0x4e00, 
		0x58ad, 
		0xd280, 
		0x4200, 
		0x8000, 
		0xc534, 
		0x0201, 
		0xcdc0, 
		0x687d, 
		0xcf80, 
		0x752e, 
		0xd320, 
		0x68c6, 
		0x52c0, 
		0xd300, 
		0xd2e0, 
		0xd0c0, 
		0x5360, 
		0x7b11, 
		0x5260, 
		0xc091, 
		0x4500, 
		0xce40, 
		0x5360, 
		0x7194, 
		0x50a0, 
		0xcba4, 
		0xc880, 
		0x5260, 
		0xf71b, 
		0x4d00, 
		0x296f, 
		0x7c00, 
		0xd080, 
		0xd300, 
		0xe6a3, 
		0x5140, 
		0x4a80, 
		0x62fb, 
		0xa958, 
		0x002b, 
		0xa3f7, 
		0xeb51, 
		0xd0e0, 
		0x4400, 
		0x83bd, 
		0xbc13, 
		0xcf80, 
		0xcc80, 
		0xd120, 
		0xcf80, 
		0xd3c0, 
		0xd2a0, 
		0x37dd, 
		0xfd9b, 
		0x0000, 
		0x4200, 
		0x4c80, 
		0xd160, 
		0x4600, 
		0xff04, 
		0x64e5, 
		0x8443, 
		0x5060, 
		0x63a7, 
		0x3c00, 
		0xd1e0, 
		0x4d40, 
		0x0000, 
		0xd060, 
		0xcd80, 
		0xd1e0, 
		0x53e0, 
		0x50e0, 
		0xfa78, 
		0x0000, 
		0xcfc0, 
		0xd380, 
		0x4a00, 
		0x4a80, 
		0xca80, 
		0x81d1, 
		0xe278, 
		0xfc00, 
		0x7c00, 
		0xcb80, 
		0xc700, 
		0xcdc0, 
		0xca80, 
		0x8000, 
		0x7c00, 
		0xc800, 
		0x4ed2, 
		0xd380, 
		0xfc00, 
		0x5320, 
		0xd2c0, 
		0xddbf, 
		0xd000, 
		0xd100, 
		0xd180, 
		0x4eb2, 
		0x0000, 
		0x50e0, 
		0x42f7, 
		0x5060, 
		0xe19c, 
		0xac10, 
		0x5c5e, 
		0x5220, 
		0x5020, 
		0x5060, 
		0x3d0a, 
		0x4a00, 
		0xd2e0, 
		0x33de, 
		0x51c0, 
		0x5220, 
		0x75e8, 
		0xc600, 
		0xe67c, 
		0x6c15, 
		0xd160, 
		0xdd0f, 
		0x8000, 
		0x4fc0, 
		0xd380, 
		0x4b00, 
		0x8000, 
		0xd0c0, 
		0xccc0, 
		0x8000, 
		0xcdc0, 
		0x5340, 
		0x4ec0, 
		0xdf13, 
		0x108d, 
		0x3c00, 
		0x52c0, 
		0x4257, 
		0x5020, 
		0x4f40, 
		0xcfc0, 
		0xdb46, 
		0x5100, 
		0xc400, 
		0xcf00, 
		0x6eef, 
		0x4df9, 
		0xfc00, 
		0x5320, 
		0x4d80, 
		0x51c0, 
		0xa191, 
		0x4700, 
		0x03fd, 
		0x0b2e, 
		0x149b, 
		0x857b, 
		0x5180, 
		0x7e5b, 
		0x51e0, 
		0xceb5, 
		0xb8c1, 
		0xd340, 
		0x6e19, 
		0xca80, 
		0xd320, 
		0xd0aa, 
		0x5000, 
		0x5020, 
		0x4dc0, 
		0x52a0, 
		0x4a00, 
		0x4cc0, 
		0xd1c0, 
		0x5160, 
		0xd060, 
		0x51e0, 
		0x5040, 
		0x1ccd, 
		0xc980, 
		0xca80, 
		0xcd40, 
		0x1420, 
		0x5280, 
		0x03fc, 
		0x50a0, 
		0xf2d8, 
		0xefc7, 
		0x893e, 
		0x4d40, 
		0x4b80, 
		0x22e0, 
		0x5000, 
		0xd360, 
		0xd0a0, 
		0xc706, 
		0x50e0, 
		0x8000, 
		0xb010, 
		0xcc00, 
		0xd360, 
		0x7330, 
		0x63ca, 
		0x7f9b, 
		0xc880, 
		0xd2e0, 
		0x4200, 
		0xab31, 
		0x50a0, 
		0xeb5f, 
		0xd100, 
		0xce40, 
		0x53e0, 
		0x1e1c, 
		0xce40, 
		0x4a80, 
		0x5040, 
		0x4cc0, 
		0x9964, 
		0xd200, 
		0x17fe, 
		0xc900, 
		0x5080, 
		0x4700, 
		0xc900, 
		0x4a80, 
		0x5020, 
		0xcf00, 
		0xffd8, 
		0x7d5e, 
		0xf6ae, 
		0x3bcd, 
		0x837c, 
		0xfd8e, 
		0x158c, 
		0xd100, 
		0x51e0, 
		0xc200, 
		0x5238, 
		0x4ec0, 
		0xccc0, 
		0x779a, 
		0xd2c0, 
		0xcc80, 
		0x4e80, 
		0xc880, 
		0xac5d, 
		0xd300, 
		0xfaa2, 
		0x38cc, 
		0x3c00, 
		0xc469, 
		0xc4f0, 
		0x25bb, 
		0x0067, 
		0x5140, 
		0xcc00, 
		0xd380, 
		0x4f80, 
		0x35ec, 
		0xd2a0, 
		0x4f00, 
		0x4f80, 
		0x4500, 
		0x7c00, 
		0x7db3, 
		0x4ec0, 
		0x80b3, 
		0x3ccf, 
		0x9f39, 
		0xcd80, 
		0xcb00, 
		0xd160, 
		0xa590, 
		0x4c00, 
		0xfc71, 
		0xcb00, 
		0xce00, 
		0x3f7b, 
		0x5240, 
		0x50a0, 
		0x6c24, 
		0xd2e0, 
		0x0000, 
		0x0000, 
		0xd360, 
		0xd41b, 
		0x272f, 
		0x8000, 
		0x4700, 
		0xcd00, 
		0x6e4b, 
		0x7cda, 
		0x90ba, 
		0xd1e0, 
		0xc700, 
		0x4f40, 
		0x8a17, 
		0x1594, 
		0x237f, 
		0x72b1, 
		0xc826, 
		0x624b, 
		0x4600, 
		0x5220, 
		0xaf18, 
		0x7d5f, 
		0x50c0, 
		0x7c00, 
		0xd2e0, 
		0x252c, 
		0xef53, 
		0xd180, 
		0x50c0, 
		0x4e00, 
		0x4fc0, 
		0xd320, 
		0xbb2d, 
		0xd1e0, 
		0x5060, 
		0xcfcf, 
		0x5300, 
		0x028f, 
		0xd100, 
		0xb593, 
		0x4200, 
		0x3956, 
		0xd0e0, 
		0x79e5, 
		0x0000, 
		0x4cc0, 
		0x7f9a, 
		0xc52b, 
		0x2497, 
		0x0795, 
		0x8000, 
		0x50c0, 
		0x7c00, 
		0x5360, 
		0x5771, 
		0xc200, 
		0x51a0, 
		0xd3c0, 
		0x4200, 
		0x343d, 
		0xd1e0, 
		0xd360, 
		0x722c, 
		0x0284, 
		0xfc00, 
		0x4f40, 
		0x8000, 
		0x11d7, 
		0x8000, 
		0x5000, 
		0xd280, 
		0x4400, 
		0x7c00, 
		0x52e0, 
		0x88ec, 
		0xc900, 
		0xebd1, 
		0xb3ec, 
		0x4800, 
		0x14a9, 
		0x52a0, 
		0xc500, 
		0xcf40, 
		0xcc40, 
		0x5000, 
		0x0000, 
		0x3f2a, 
		0xd000, 
		0xcf40, 
		0xd180, 
		0xbb79, 
		0x8000, 
		0x5060, 
		0x4c80, 
		0xd3a0, 
		0xfc00, 
		0x3ef4, 
		0xfc00, 
		0xc700, 
		0x0000, 
		0xc800, 
		0x5020, 
		0xc000, 
		0xce40, 
		0x88c6, 
		0x2804, 
		0x52c0, 
		0xc700, 
		0x0000, 
		0xa380, 
		0xcc9f, 
		0x80dd, 
		0x8000, 
		0x9e05, 
		0x68fd, 
		0x5060, 
		0xd200, 
		0x5320, 
		0x4500, 
		0x4f40, 
		0x5240, 
		0xcc40, 
		0x0000, 
		0xccd1, 
		0x53c0, 
		0x4b00, 
		0x30e6, 
		0x0000, 
		0xcd40, 
		0xd1c0, 
		0x50e0, 
		0xcfc0, 
		0xd060, 
		0x4800, 
		0x1df5, 
		0x97f5, 
		0xc400, 
		0x4880, 
		0xcc40, 
		0x4f13, 
		0xddf6, 
		0x27eb, 
		0xc500, 
		0xd1a0, 
		0x0de4, 
		0xcf40, 
		0xce00, 
		0xd53c, 
		0xd020, 
		0x5360, 
		0x5300, 
		0x8d08, 
		0x7fee, 
		0xfc00, 
		0x0000, 
		0x8000, 
		0x874e, 
		0xb831, 
		0x0000, 
		0x52c0, 
		0x03e0, 
		0x5380, 
		0x04b2, 
		0xcb00, 
		0x4800, 
		0x5000, 
		0x5300, 
		0x54e1, 
		0x0000, 
		0x53c0, 
		0x4cc0, 
		0x0000, 
		0x0000, 
		0xe80a, 
		0x0d7c, 
		0x4a10, 
		0xc400, 
		0x4d00, 
		0x51a0, 
		0x4a00, 
		0x52a0, 
		0xc000, 
		0xc400, 
		0x4b00, 
		0xd360, 
		0x4b80, 
		0xd120, 
		0x5280, 
		0x8000, 
		0x0bc0, 
		0x4000, 
		0xd0c0, 
		0x4b00, 
		0x8107, 
		0x4700, 
		0xa516, 
		0x3173, 
		0x8da3, 
		0xd320, 
		0x5060, 
		0x4c40, 
		0x0db3, 
		0xc500, 
		0x0000, 
		0xcc80, 
		0x5e01, 
		0x1dd9, 
		0x7c00, 
		0xb3e7, 
		0xef97, 
		0xd020, 
		0xb97f, 
		0x4c00, 
		0xd300, 
		0xd360, 
		0xcec0, 
		0x0f34, 
		0xd180, 
		0x4dc0, 
		0x4600, 
		0xd100, 
		0xcd00, 
		0xd280, 
		0xd0a0, 
		0x7d76, 
		0x4d40, 
		0x5360, 
		0x5160, 
		0xd300, 
		0xe0b0, 
		0xcdc0, 
		0xcd40, 
		0x4c40, 
		0xc600, 
		0x87ba, 
		0xd8d1, 
		0xcf00, 
		0x51c0, 
		0x0000, 
		0x3de8, 
		0x4f80, 
		0x4e80, 
		0xd2c0, 
		0x8531, 
		0x503d, 
		0x4d40, 
		0x4900, 
		0x43fd, 
		0x0455, 
		0x53c0, 
		0x0000, 
		0x0000, 
		0x4a00, 
		0x4c00, 
		0xfc00, 
		0xd200, 
		0x0000, 
		0xca80, 
		0x5360, 
		0x5320, 
		0x3c00, 
		0x8000, 
		0xae76, 
		0x7d0b, 
		0xc900, 
		0x8000, 
		0xa30e, 
		0x4400, 
		0xa26d, 
		0x0000, 
		0x692e, 
		0x7dc0, 
		0x9c41, 
		0xe73b, 
		0xa5b1, 
		0xd0c0, 
		0xd260, 
		0xd90c, 
		0x4e80, 
		0x51c0, 
		0x4980, 
		0x3c00, 
		0xd1e0, 
		0x4e80, 
		0x2c77, 
		0x4e80, 
		0x2bbb, 
		0x4e80, 
		0x4f80, 
		0xf743, 
		0x4f80, 
		0xfc00, 
		0xd020, 
		0x53c0, 
		0x0000, 
		0xd180, 
		0xcf40, 
		0x4e40, 
		0x7c84, 
		0x9b19, 
		0x5260, 
		0xc368, 
		0x4d80, 
		0x65ae, 
		0xaf93, 
		0x4176, 
		0x6eff, 
		0xf15e, 
		0x59ca, 
		0x4968, 
		0x8000, 
		0x8000, 
		0x2a10, 
		0x459f, 
		0xc700, 
		0x5020, 
		0x5160, 
		0x5320, 
		0x25ee, 
		0xcb80, 
		0x7d2d, 
		0x4fc0, 
		0xd320, 
		0x6d62, 
		0xd2c0, 
		0x698a, 
		0xd040, 
		0xd160, 
		0xc2c3, 
		0xcb80, 
		0xd2a0, 
		0x80e0, 
		0x4600, 
		0x4980, 
		0x50a0, 
		0x53e0, 
		0x5060, 
		0xcdc0, 
		0x357b, 
		0xc500, 
		0xc567, 
		0xc500, 
		0xce00, 
		0xd100, 
		0x4cc0, 
		0x5000, 
		0xce40, 
		0xc965, 
		0xe4d0, 
		0x59b4, 
		0x7fe9, 
		0xd1e0, 
		0x52a0, 
		0x3cb1, 
		0x9358, 
		0x8000, 
		0xc919, 
		0x4800, 
		0x4dc0, 
		0x4c49, 
		0x087c, 
		0xb0e9, 
		0xd180, 
		0xd0c0, 
		0x2820, 
		0xc500, 
		0xd328, 
		0x4500, 
		0xaf4b, 
		0xd7ca, 
		0x7d7e, 
		0xc880, 
		0x4e80, 
		0x52a0, 
		0xc0c4, 
		0x53a0, 
		0xd080, 
		0x4500, 
		0x0000, 
		0x4d00, 
		0x71b7, 
		0xc400, 
		0x4d00, 
		0x4d00, 
		0x52a0, 
		0xc000, 
		0x9071, 
		0xc400, 
		0x4a80, 
		0x000a, 
		0x5000, 
		0xc980, 
		0xee49, 
		0xe486, 
		0x7c00, 
		0xd2e0, 
		0x627c, 
		0x5380, 
		0x4700, 
		0x134b, 
		0x52c0, 
		0x4cc0, 
		0xd2a0, 
		0x4880, 
		0x35c0, 
		0x5eb1, 
		0xcf00, 
		0x53c0, 
		0x5140, 
		0x0000, 
		0xca00, 
		0xdf7b, 
		0xbc00, 
		0x5000, 
		0x4500, 
		0xd160, 
		0xb578, 
		0x2c13, 
		0x2335, 
		0xca80, 
		0xd4cd, 
		0x5300, 
		0x5340, 
		0xd220, 
		0x02c5, 
		0xd100, 
		0xd160, 
		0x5f01, 
		0xd280, 
		0xd3a0, 
		0x4c40, 
		0xce00, 
		0xd2e0, 
		0xe832, 
		0xd120, 
		0xc600, 
		0x130a, 
		0xd120, 
		0x7c00, 
		0x4c00, 
		0x4e40, 
		0x7c00, 
		0x52c0, 
		0x5733, 
		0x797a, 
		0x7c00, 
		0xd200, 
		0x8000, 
		0x1d43, 
		0x4200, 
		0xd140, 
		0x8000, 
		0xc900, 
		0x4077, 
		0xcd38, 
		0x8000, 
		0x8661, 
		0xd280, 
		0xd240, 
		0x4cc0, 
		0x4700, 
		0x50e0, 
		0x0000, 
		0xd3c0, 
		0xd100, 
		0x8000, 
		0xcb00, 
		0x635a, 
		0x50a0, 
		0x0000, 
		0x5180, 
		0x4d40, 
		0x4e00, 
		0x4600, 
		0x5040, 
		0x7d62, 
		0x4cc0, 
		0x8000, 
		0x4fc0, 
		0x4500, 
		0xece6, 
		0xc6ca, 
		0x8000, 
		0x5040, 
		0x4400, 
		0xc900, 
		0xd3c0, 
		0x2e1a, 
		0x5060, 
		0x53c0, 
		0xce80, 
		0x52a0, 
		0xd3a0, 
		0x717d, 
		0xfc00, 
		0xe62b, 
		0xd1a0, 
		0x4a00, 
		0xfa5a, 
		0x784a, 
		0xc462, 
		0x53a0, 
		0xcf40, 
		0x4aa0, 
		0x96a9, 
		0x4cf7, 
		0xd380, 
		0x118c, 
		0xd380, 
		0x5180, 
		0xca00, 
		0x5340, 
		0x52a0, 
		0x4f40, 
		0x51e0, 
		0xd160, 
		0x2664, 
		0xcf80, 
		0x452c, 
		0x4f80, 
		0x5340, 
		0xd340, 
		0xaa04, 
		0x8000, 
		0x4f00, 
		0xc500, 
		0x51e0, 
		0x4f40, 
		0x4e00, 
		0x7c00, 
		0xb514, 
		0xcc40, 
		0x8000, 
		0x4ae5, 
		0xca00, 
		0x4df3, 
		0xd3e0, 
		0xfc00, 
		0xe8d8, 
		0x4cc0, 
		0x5060, 
		0xa8c6, 
		0x4e80, 
		0x5360, 
		0x351c, 
		0x5180, 
		0xddbb, 
		0x5320, 
		0xcf80, 
		0xd260, 
		0x8000, 
		0x7c00, 
		0x5264, 
		0x4200, 
		0xca80, 
		0xd260, 
		0x4800, 
		0x5040, 
		0xd380, 
		0xcec0, 
		0xd1a0, 
		0x4a80, 
		0x5280, 
		0x1b7a, 
		0xedcf, 
		0x3e7b, 
		0x4c40, 
		0x66b8, 
		0xd300, 
		0x400f, 
		0x4c80, 
		0xcd80, 
		0x5060, 
		0xd0e0, 
		0x4d00, 
		0x5160, 
		0x7c00, 
		0x49f5, 
		0x4d80, 
		0x5380, 
		0xdd36, 
		0x4b80, 
		0xac59, 
		0xd060, 
		0xccc0, 
		0x53e0, 
		0x1a1b, 
		0x20c0, 
		0xca80, 
		0x5160, 
		0xd080, 
		0xac58, 
		0xcc80, 
		0xd240, 
		0xce40, 
		0xe9ba, 
		0xd000, 
		0xce00, 
		0x4cc0, 
		0xfc1a, 
		0xc800, 
		0xcd40, 
		0x52c0, 
		0x50a0, 
		0x53a0, 
		0xd040, 
		0x5040, 
		0x8855, 
		0xd3c0, 
		0x5100, 
		0x4f19, 
		0xd240, 
		0xecd4, 
		0xcb80, 
		0xd220, 
		0xd120, 
		0x63bf, 
		0xaa8c, 
		0x5140, 
		0x1543, 
		0xd360, 
		0x4f40, 
		0xcc30, 
		0xd380, 
		0x0e70, 
		0xcbe6, 
		0xc800, 
		0x4500, 
		0x5060, 
		0x2292, 
		0x5834, 
		0x2257, 
		0xd360, 
		0xcd00, 
		0x4e00, 
		0x4cc0, 
		0x5160, 
		0xc400, 
		0x4900, 
		0xca00, 
		0x5100  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x070993e4, 
		0x00800002, 
		0x41900000, 
		0x425c0000, 
		0x0e000001, 
		0x80800001, 
		0x0c7fff80, 
		0x10352473, 
		0xc1200000, 
		0xff800000, 
		0x7f800000, 
		0x00000000, 
		0xc0000000, 
		0x00000008, 
		0xc2400000, 
		0x422c0000, 
		0xff800000, 
		0x80000100, 
		0x0c7fff00, 
		0x0e1fffff, 
		0x80000000, 
		0xbfda344d, 
		0x41d80000, 
		0xc1000000, 
		0x1b200534, 
		0x42740000, 
		0x7f800000, 
		0xa666dfd5, 
		0x7fc00001, 
		0x576acc5e, 
		0x00000100, 
		0xbf800001, 
		0x00000200, 
		0xc2240000, 
		0x0c7f0000, 
		0x260b3167, 
		0x41f00000, 
		0x41c80000, 
		0x42080000, 
		0x0f92cc77, 
		0x80800000, 
		0x0c7fffff, 
		0xff800000, 
		0x7f800000, 
		0x807fffff, 
		0xc2440000, 
		0x80000400, 
		0xc2700000, 
		0x8d955ce1, 
		0xc2140000, 
		0x80000000, 
		0x40a00000, 
		0x80000000, 
		0x42700000, 
		0x80000080, 
		0xc0800000, 
		0x80002000, 
		0xff800000, 
		0x0d00fff0, 
		0xc2700000, 
		0x3299ad0e, 
		0x0e00007f, 
		0x7fc00fd3, 
		0x00000000, 
		0xdc1a3d2c, 
		0x0c7ffffc, 
		0x41b80000, 
		0xa331d5c9, 
		0xd0900233, 
		0x420c0000, 
		0x41a80000, 
		0x0e00007f, 
		0xb387c5a8, 
		0xc1500000, 
		0x00011111, 
		0xd1963fd0, 
		0x80800001, 
		0xff800000, 
		0xc2480000, 
		0x80800003, 
		0x7f7ffffe, 
		0x1ac3001e, 
		0x00000002, 
		0xc1980000, 
		0x42440000, 
		0x0e00003f, 
		0xec1ae8fa, 
		0x40800000, 
		0x42500000, 
		0xff800000, 
		0x0c7fe000, 
		0x00000400, 
		0x8f7fffff, 
		0xa8ab74d8, 
		0x80000020, 
		0x80008000, 
		0x4b32b499, 
		0x42540000, 
		0x80001000, 
		0x007ffffe, 
		0x00800001, 
		0x41c00000, 
		0x00100000, 
		0x8f7ffffe, 
		0xa393a7a5, 
		0x0c0d9e4f, 
		0xc0e00000, 
		0x80800000, 
		0x0e00007f, 
		0x7f8ba499, 
		0x230afe30, 
		0x35fd8d59, 
		0x0e000fff, 
		0x80002000, 
		0x1c0c976f, 
		0x0e03ffff, 
		0x00800001, 
		0x42400000, 
		0x00000000, 
		0x00011111, 
		0xc2540000, 
		0x80000800, 
		0x00800000, 
		0x0e0001ff, 
		0x80200000, 
		0xff800000, 
		0x40e00000, 
		0x38b8c4ce, 
		0x0c7ffffc, 
		0x3c561368, 
		0x10cbbe92, 
		0x59f96176, 
		0xc2000000, 
		0xc1200000, 
		0x4b000000, 
		0xc2380000, 
		0x42280000, 
		0x42580000, 
		0x80000002, 
		0xf24b9a11, 
		0x41300000, 
		0x6c47d0b7, 
		0x149f2415, 
		0x2f146576, 
		0x42440000, 
		0xefd86f66, 
		0x7f7ffffe, 
		0x80008000, 
		0xc25c0000, 
		0xdbc74af8, 
		0x80001000, 
		0x2fc5150e, 
		0x8ab97947, 
		0x831e86a6, 
		0x42600000, 
		0x8f7fffff, 
		0x7f7fffff, 
		0x41a00000, 
		0x42480000, 
		0x7f800000, 
		0x00ffffff, 
		0x80000020, 
		0xb64a0167, 
		0x9eda0e5d, 
		0x918c7903, 
		0x7963ff8a, 
		0x80010000, 
		0xff7ffffe, 
		0x40000000, 
		0x42040000, 
		0x4dd368a9, 
		0x21579f31, 
		0x7a1707e7, 
		0x41d00000, 
		0xcccccccc, 
		0x41c80000, 
		0x801927ab, 
		0xef62ee4a, 
		0xc1500000, 
		0x7f800000, 
		0x00000002, 
		0xc1d80000, 
		0xb4ce0b04, 
		0x7f800000, 
		0xc2480000, 
		0xff7ffffe, 
		0xd2cee634, 
		0x0c7ff000, 
		0x00000008, 
		0x0c7e0000, 
		0x0b015091, 
		0x2be37e11, 
		0xe2498a7d, 
		0x00000000, 
		0x00800003, 
		0x80004000, 
		0x79491f14, 
		0x41880000, 
		0x0c7ffffc, 
		0x422c0000, 
		0x00000000, 
		0x3690c6c8, 
		0x657a340f, 
		0x80000000, 
		0x7fede03f, 
		0x4daff28c, 
		0x00ffffff, 
		0x80800000, 
		0x7f800000, 
		0x00000200, 
		0xcb000000, 
		0xc86497a7, 
		0xc1500000, 
		0xc1e80000, 
		0x0bc3280a, 
		0x00000400, 
		0x9a892aa6, 
		0x33333333, 
		0x4f348c89, 
		0x0e000001, 
		0x80010000, 
		0x0c400000, 
		0x00100000, 
		0x33333333, 
		0x00800001, 
		0x80080000, 
		0x29551283, 
		0x41880000, 
		0xffffffff, 
		0x486bacd8, 
		0x0e0003ff, 
		0x0c7ffe00, 
		0xc1d00000, 
		0xff800001, 
		0x4d051eda, 
		0x007fffff, 
		0x00000002, 
		0xffbd1f0a, 
		0x0c780000, 
		0x7fa15be3, 
		0x41c80000, 
		0xbe9196d7, 
		0xaa545866, 
		0x42280000, 
		0xc2640000, 
		0x00000000, 
		0x80004000, 
		0x426c0000, 
		0x00040000, 
		0x7fc00001, 
		0x32e9ad1e, 
		0xc0800000, 
		0x80000000, 
		0x80000000, 
		0x00004000, 
		0xc1a00000, 
		0x42380000, 
		0x847618d2, 
		0x421c0000, 
		0xc1100000, 
		0x001a3df0, 
		0x0f7ffffc, 
		0x41e00000, 
		0xff800000, 
		0xff9291e0, 
		0x00800002, 
		0x08547f43, 
		0x0c780000, 
		0x40000000, 
		0xc2340000, 
		0xffc00001, 
		0xffc00000, 
		0xeabad8cf, 
		0x00000100, 
		0xffc00001, 
		0x0b2280bb, 
		0xff800000, 
		0x40800000, 
		0xcccccccc, 
		0x80000002, 
		0x41100000, 
		0xffc00001, 
		0x80000000, 
		0x0e00000f, 
		0xa9b367eb, 
		0x0e001fff, 
		0xff7ffffe, 
		0xc1c80000, 
		0xc0c00000, 
		0x246ec76f, 
		0x7f800000, 
		0x9fec2260, 
		0x7fbfffff, 
		0xff962e72, 
		0x7f7ffffe, 
		0x0e482ec1, 
		0x00800003, 
		0x20825211, 
		0xd021d853, 
		0x80000001, 
		0x80800000, 
		0xd5b7c42d, 
		0x00800001, 
		0xc1f00000, 
		0xac6b12f4, 
		0x7f800001, 
		0x1b94e787, 
		0x80001000, 
		0xc2140000, 
		0x00800002, 
		0x00000000, 
		0xc0000000, 
		0xc2780000, 
		0x0e07ffff, 
		0x814d020b, 
		0xc1f80000, 
		0xf8c0f93c, 
		0x80000000, 
		0x00000001, 
		0xb1837b99, 
		0x00004000, 
		0x42040000, 
		0x80800003, 
		0x00800000, 
		0x8e9b2e38, 
		0xff000000, 
		0x6e8a05a0, 
		0xc098ad8b, 
		0x42200000, 
		0xbf800000, 
		0x42100000, 
		0x55160a9a, 
		0x41100000, 
		0xb3df1ad3, 
		0xc2700000, 
		0xc1800000, 
		0x9b3fdad2, 
		0x00000000, 
		0xdf7aba2d, 
		0xc0c00000, 
		0x41a00000, 
		0xc27c0000, 
		0x80000000, 
		0xe40fcfcf, 
		0x41c80000, 
		0x0e0fffff, 
		0x80800000, 
		0x138f86c9, 
		0x74acef42, 
		0x42540000, 
		0xc25c0000, 
		0x0e0fffff, 
		0xf394c5de, 
		0x00000000, 
		0x80000000, 
		0xff7fffff, 
		0x80000020, 
		0x509a0db6, 
		0xff800000, 
		0xc0800000, 
		0xc7ddfb01, 
		0xc1500000, 
		0x00000000, 
		0x42040000, 
		0xb34fbb6d, 
		0x12947392, 
		0x807ffffe, 
		0xccde428f, 
		0x00001000, 
		0xcac2ad0d, 
		0x6bb00573, 
		0x8f7ffffe, 
		0xc2300000, 
		0x8e6d48f8, 
		0xc1200000, 
		0x5f161ccc, 
		0xe4ea4005, 
		0x7f800000, 
		0xff9c9a71, 
		0xf885b9c4, 
		0xff800000, 
		0xc1400000, 
		0x41200000, 
		0xf25bd9d6, 
		0x7fc00001, 
		0xc20c0000, 
		0xc0000000, 
		0xff800000, 
		0xd004e07e, 
		0x42300000, 
		0xffffffff, 
		0x80008000, 
		0x1d9f2436, 
		0x0061e15c, 
		0x40c00000, 
		0x80800001, 
		0x7fc00000, 
		0x7f7ffffe, 
		0x133107f3, 
		0xa9dc4250, 
		0x7328bfb3, 
		0xab658eff, 
		0xc2040000, 
		0x7f7fffff, 
		0x42200000, 
		0x83f93056, 
		0xff800000, 
		0x80800000, 
		0x40800000, 
		0x80000000, 
		0xc27c0000, 
		0xc1980000, 
		0x0e00003f, 
		0x422c0000, 
		0x0865f517, 
		0xc2180000, 
		0x421c0000, 
		0x65501939, 
		0x462b858e, 
		0x0c400000, 
		0xc380ad29, 
		0x80000100, 
		0xbf783836, 
		0xc1d00000, 
		0x0e003fff, 
		0xc1e80000, 
		0x00000000, 
		0x0c7c0000, 
		0x41880000, 
		0x80000001, 
		0xff7ffffe, 
		0x00800000, 
		0xd40cf433, 
		0x80ffffff, 
		0x80000000, 
		0x9900e7c3, 
		0x80000002, 
		0x7f800000, 
		0x0e01ffff, 
		0x80800001, 
		0x0c7ff800, 
		0xcaf3c00c, 
		0x00800003, 
		0xc22c0000, 
		0xb0cce818, 
		0x781a4319, 
		0x00000010, 
		0x7d3eda56, 
		0x11932e13, 
		0xe47fd2ef, 
		0xc23c0000, 
		0xff800000, 
		0x0e001fff, 
		0x41a00000, 
		0x7f800000, 
		0x80800000, 
		0x0c7ffff0, 
		0x80000000, 
		0x7f7ffffe, 
		0x41100000, 
		0x00800002, 
		0xd82a7688, 
		0x27c9c6a3, 
		0x7f7fffff, 
		0xbb11e9fb, 
		0x426c0000, 
		0x0e00001f, 
		0x7bd68a08, 
		0x01f2a69c, 
		0x6eaa162f, 
		0xaaaaaaaa, 
		0xc2080000, 
		0x41a80000, 
		0xc24c0000, 
		0x80000001, 
		0xc2240000, 
		0x80800001, 
		0x69d2dde8, 
		0x0c7ffffc, 
		0x80000000, 
		0x422c0000, 
		0x4b8c4b40, 
		0x80010000, 
		0x62790404, 
		0x41a80000, 
		0x80000000, 
		0x42300000, 
		0xc1880000, 
		0x8e0ce282, 
		0xd096f6aa, 
		0xc1e00000, 
		0xc0800000, 
		0x80484e08, 
		0xc230e018, 
		0x425c0000, 
		0x80000000, 
		0x80800000, 
		0xc20c0000, 
		0x00000000, 
		0xffbfffff, 
		0x80000002, 
		0xc1a80000, 
		0x8f7ffffd, 
		0x0bef09c4, 
		0x00000000, 
		0x0f7ffffc, 
		0xac2df4ed, 
		0x00000000, 
		0x423c0000, 
		0x2c040079, 
		0x0c7e0000, 
		0xe73b8837, 
		0x0e000003, 
		0x00000000, 
		0x7f9f8769, 
		0xff7ffffe, 
		0x0c7fffff, 
		0x7f7ffffe, 
		0x665172f7, 
		0x41000000, 
		0x41d00000, 
		0x3812cd56, 
		0x00000000, 
		0xc2280000, 
		0x00000000, 
		0x7fc00001, 
		0x00000000, 
		0xc2180000, 
		0x43333f7f, 
		0x41300000, 
		0x6e995a6b, 
		0x55555555, 
		0x41700000, 
		0x0e03ffff, 
		0x0c400000, 
		0x0e03ffff, 
		0xffa7e34d, 
		0x0c7fc000, 
		0x6bde9c37, 
		0x8076a8ac, 
		0x80000000, 
		0x0e0fffff, 
		0xe7f67368, 
		0x7f800000, 
		0x0e00003f, 
		0x80000002, 
		0x80000010, 
		0x0c7ffffc, 
		0x80080000, 
		0x41400000, 
		0x00800000, 
		0x7f800000, 
		0xff800000, 
		0xc2180000, 
		0x41b00000, 
		0x424c0000, 
		0x13b64884, 
		0x21fcb900, 
		0x9357f95c, 
		0x00002000, 
		0x6b56bcf3, 
		0xc1800000, 
		0x9ca1cec5, 
		0x41100000, 
		0x80000000, 
		0xc2000000, 
		0x41700000, 
		0x0e003fff, 
		0x0c7fffff, 
		0xa44520bc, 
		0x00800001, 
		0x2f5bdd86, 
		0x0e001fff, 
		0xc2740000, 
		0x948251f1, 
		0xc2680000, 
		0x42580000, 
		0x23bb85d4, 
		0x08427863, 
		0x00008000, 
		0xbe0eec82, 
		0xff800000, 
		0x0e3fffff, 
		0x41b00000, 
		0x41f80000, 
		0x42100000, 
		0x00000010, 
		0x394096f4, 
		0x911322fc, 
		0xc0e00000, 
		0x00000000, 
		0xc1f80000, 
		0x3f800000, 
		0x061bb345, 
		0xc2100000, 
		0xc2500000, 
		0x80800002, 
		0x41700000, 
		0xc0800000, 
		0x7cd0223e, 
		0x41f80000, 
		0x8605872a, 
		0x00000000, 
		0x00000000, 
		0x7f9c382f, 
		0x40a00000, 
		0x41b00000, 
		0x4b8c4b40, 
		0x80000000, 
		0xff7ffffe, 
		0xff92ec5e, 
		0x09223d32, 
		0xd517143f, 
		0x00800001, 
		0x68edc410, 
		0x00010000, 
		0x40400000, 
		0x0c400000, 
		0x42080000, 
		0x80000000, 
		0xc1100000, 
		0x80008000, 
		0x744ce841, 
		0xfae5b528, 
		0x0c7e0000, 
		0xc1d00000, 
		0x80800002, 
		0xb9b96404, 
		0x0c7f0000, 
		0xf39313e5, 
		0xffa7bba1, 
		0x80000000, 
		0x0c600000, 
		0xff800000, 
		0x0f7ffffd, 
		0x00000000, 
		0x80000002, 
		0x00000002, 
		0x00000008, 
		0x00000100, 
		0xb6662009, 
		0xc1000000, 
		0xae7a4682, 
		0xc2600000, 
		0x229af820, 
		0x7f9b558c, 
		0x80000000, 
		0x2fbddd1d, 
		0xf5b7ce17, 
		0x80100000, 
		0x000095f0, 
		0x997f3b2b, 
		0x80000000, 
		0x7f800000, 
		0xaec495cf, 
		0xc2680000, 
		0x00000000, 
		0x0c7fffff, 
		0xc1200000, 
		0xf382459b, 
		0x42380000, 
		0x00080000, 
		0x3f6561a2, 
		0x42700000, 
		0x41c00000, 
		0x7fc00001, 
		0x425c0000, 
		0xc0800000, 
		0xff000000, 
		0xc1100000, 
		0xc1e00000, 
		0xd7cb7cce, 
		0x40cb888d, 
		0x80001000, 
		0x0e0fffff, 
		0x301aa287, 
		0x80d02d2c, 
		0x00000000, 
		0x00800001, 
		0x87205711, 
		0xc4fb0f6a, 
		0x00000020, 
		0x00011111, 
		0xc2000000, 
		0x42780000, 
		0x1b164f6d, 
		0x59db759e, 
		0x0c7ffffc, 
		0x41b80000, 
		0xc22c0000, 
		0xc1100000, 
		0x00008000, 
		0x007fffff, 
		0x0e3fffff, 
		0x80000200, 
		0xc25c0000, 
		0xffffffff, 
		0x424c0000, 
		0x0c780000, 
		0x8f2e6d56, 
		0x6f84d9fb, 
		0x42540000, 
		0x42180000, 
		0x0e0001ff, 
		0x807fffff, 
		0x29dbc0a9, 
		0xff7fffff, 
		0x7fb3d8ff, 
		0x34f67248, 
		0xc2740000, 
		0x00100000, 
		0x00800001, 
		0x86e55770, 
		0x80000000, 
		0x80040000, 
		0x0c780000, 
		0x40400000, 
		0xc2300000, 
		0x80200000, 
		0x41100000, 
		0xc2180000, 
		0x7f800000, 
		0x42380000, 
		0x80000000, 
		0x80000002, 
		0x80000000, 
		0x42400000, 
		0x6e2f9138, 
		0xb831c76c, 
		0xc26c0000, 
		0x811bd5ed, 
		0x0c700000, 
		0x007ffffe, 
		0xb5e7ee0c, 
		0xffffffff, 
		0x0c7fe000, 
		0xff7ffffe, 
		0x00200000, 
		0xa91765ab, 
		0xc2600000, 
		0x0e001fff, 
		0x0e03ffff, 
		0x7f800000, 
		0xffbfffff, 
		0xf6412281, 
		0xc2680000, 
		0x0d000ff0, 
		0x80800002, 
		0x80000002, 
		0x0e00003f, 
		0x41980000, 
		0xc1980000, 
		0x806ded9f, 
		0xc2380000, 
		0x00000000, 
		0x00800000, 
		0x80010000, 
		0x42140000, 
		0x00000000, 
		0x4c872966, 
		0xc22c0000, 
		0x42440000, 
		0x00ffffff, 
		0xa1c397f2, 
		0x80020000, 
		0xc1f80000, 
		0x80000000, 
		0x80800002, 
		0x0e00ffff, 
		0xcce4d5ce, 
		0x80000400, 
		0x7f7ffffe, 
		0x00ffffff, 
		0x0f7ffffe, 
		0xc22c0000, 
		0x42580000, 
		0x55d690f2, 
		0x80000002, 
		0x80010000, 
		0x41500000, 
		0x80800001, 
		0xff7ffffe, 
		0x0c7ff800, 
		0xc1d00000, 
		0xae26936d, 
		0x596fd241, 
		0x42640000, 
		0x42540000, 
		0x3f028f5c, 
		0xbf800001, 
		0x007fffff, 
		0x15423795, 
		0x00008000, 
		0x0e0000ff, 
		0x00800003, 
		0xc0e00000, 
		0x80000000, 
		0x00800002, 
		0xc2200000, 
		0x80000000, 
		0x41100000, 
		0x42200000, 
		0x80200000, 
		0x7f800000, 
		0x0c7fe000, 
		0xc2680000, 
		0x558c82e7, 
		0xb98b1cb6, 
		0x80001000, 
		0x0e00000f, 
		0x2d98a7c0, 
		0x1bbf379a, 
		0x7fc00001, 
		0x80008000, 
		0x00000000, 
		0x421c0000, 
		0x77df6ed1, 
		0xbf028f5c, 
		0x7f977709, 
		0x42580000, 
		0x80020000, 
		0x80000000, 
		0xffffffff, 
		0xffc00000, 
		0x41700000, 
		0x427c0000, 
		0xc26c0000, 
		0x42680000, 
		0xc47237f0, 
		0xc2080000, 
		0x00800003, 
		0xffc00001, 
		0x27e2d68e, 
		0x80000004, 
		0x0c7ffffe, 
		0xc1a80000, 
		0xc1e00000, 
		0x185a4cd7, 
		0x3a601f81, 
		0x0d000ff0, 
		0xbe0b5933, 
		0x42040000, 
		0xcccccccc, 
		0x00000000, 
		0x422c0000, 
		0x0e3fffff, 
		0x7d66b71f, 
		0x80000000, 
		0xc9dca64b, 
		0x00000000, 
		0xbf800001, 
		0xcda03762, 
		0x00008000, 
		0x8f7ffffd, 
		0x6fe09614, 
		0x7f800001, 
		0x7555b91b, 
		0x0c7ffc00, 
		0x0c7fffff, 
		0x0c7fc000, 
		0x40800000, 
		0x80800002, 
		0x00008000, 
		0x1619bb7d, 
		0xc1d80000, 
		0xff7ffffe, 
		0x41800000, 
		0xa2e26c1b, 
		0x0b02dcb5, 
		0x00000200, 
		0x7f800000, 
		0x1dce2085, 
		0x80000000, 
		0x9ee3d166, 
		0x07b673c6, 
		0x83a94758, 
		0x0c7ffffc, 
		0xc98d79b5, 
		0x0c7e0000, 
		0x80100000, 
		0x41700000, 
		0xd9f972ba, 
		0x41e00000, 
		0xb28cf07b, 
		0x7f7ffffe, 
		0x80004000, 
		0x7f000000, 
		0xc24c0000, 
		0xc187d69c, 
		0x0c700000, 
		0x41600000, 
		0x42140000, 
		0x807ffffe, 
		0x7f9b1948, 
		0x8042c787, 
		0xff800000, 
		0x00000000, 
		0xda3fe52c, 
		0x0c7fff80, 
		0xc1600000, 
		0x80000001, 
		0xffffffff, 
		0x00000000, 
		0xc2440000, 
		0x00001000, 
		0xc1000000, 
		0x7f7ffffe, 
		0x36d8bab2, 
		0x2d7e9050, 
		0x41100000, 
		0x00004000, 
		0x422c0000, 
		0x00000400, 
		0x0162323c, 
		0xc2740000, 
		0xc2700000, 
		0x0e007fff, 
		0x7fe5083a, 
		0x59350ede, 
		0x80000000, 
		0xc2600000, 
		0x4526bd15, 
		0x00000000, 
		0x80000000, 
		0x00800003, 
		0x80000004, 
		0x7f7fffff, 
		0xc2280000, 
		0xff800000, 
		0xea0efafc, 
		0xc1880000, 
		0x80800001, 
		0xffc00000, 
		0xc2140000, 
		0x999dccfe, 
		0x7f7ffffe, 
		0x41e00000, 
		0x80008000, 
		0xc1c80000, 
		0xdb208dbb, 
		0xdfcbbdab, 
		0x80000000, 
		0x00000002, 
		0x0e0000ff, 
		0x0c7fc000, 
		0xa7f3222c, 
		0x431358cf, 
		0xaaaaaaaa, 
		0x41a80000, 
		0xf7093880, 
		0x0b293ff6, 
		0x807fffff, 
		0x0c7fff80, 
		0x8f7fffff, 
		0x421c0000, 
		0x0e0000ff, 
		0xaed99fa0, 
		0xc2400000, 
		0xbb0dc862, 
		0xffec48d5, 
		0xc7a783a2, 
		0x28831bc5, 
		0x6aeeae9e, 
		0x1990406f, 
		0x572b5b4e, 
		0xfa8d92cd, 
		0x41700000, 
		0x80020000, 
		0xc2500000, 
		0x0e07ffff, 
		0xc24c0000, 
		0x00000000, 
		0x69257ad7, 
		0xaeb03a34, 
		0xa1aad07f, 
		0xff7ffffe, 
		0x00100000, 
		0x42480000, 
		0x807fffff, 
		0xffc00000, 
		0x41e00000, 
		0x0b44c092, 
		0xc9cad145, 
		0x41700000, 
		0x0f7ffffd, 
		0x42500000, 
		0xbf800000, 
		0xc24c0000, 
		0x80010000, 
		0x7fd15267, 
		0xcccccccc, 
		0x7f9f1926, 
		0xff800000, 
		0x00000000, 
		0x42180000, 
		0x0e00003f, 
		0xc1100000, 
		0x801475bf, 
		0x3c14dfe5, 
		0x007fffff, 
		0x80000001, 
		0x42200000, 
		0x007fffff, 
		0x00492cb8, 
		0x0c7fffff, 
		0xc2380000, 
		0x427c0000, 
		0x0c7ffff0, 
		0x00800003, 
		0x0e0007ff, 
		0xc1200000, 
		0x106f469f, 
		0x00011111, 
		0xc1000000, 
		0xc2000000, 
		0xc2740000, 
		0x00000000, 
		0x4144777f, 
		0x80000002, 
		0x00000000, 
		0x006287e5, 
		0x0f28639c, 
		0xff7ffffe, 
		0xc2640000, 
		0xc1c00000, 
		0x77d4bc1a, 
		0xc116e71e, 
		0x5895edb8, 
		0x9fa225ab, 
		0xd0022d60, 
		0xd7a133fd, 
		0x41000000, 
		0x8d2f5d4a, 
		0x1953e763, 
		0x6ea0a8e3, 
		0x80080000, 
		0xc2280000, 
		0xc24c0000, 
		0xb5f739e9, 
		0xc22c0000, 
		0x423c0000, 
		0xff7ffffe, 
		0xc1300000, 
		0x0c7f8000, 
		0x0c780000, 
		0x0c400000, 
		0x41800000, 
		0x80080000, 
		0xc1c00000, 
		0xff7fffff, 
		0x0f7fffff, 
		0x00000000, 
		0x42540000, 
		0x80000004, 
		0x00800003, 
		0x80800003, 
		0x0e03ffff, 
		0x80010000, 
		0x40e00000, 
		0x00400000, 
		0x00000000, 
		0xff800000, 
		0x00002000, 
		0x00080000, 
		0x42000000, 
		0x00000800, 
		0x42080000, 
		0xc0e00000, 
		0xfa30f73f, 
		0xcf526a78, 
		0x00000000, 
		0x00000200, 
		0x80000000, 
		0xd7314271, 
		0x8de7c920, 
		0x41d80000, 
		0xff7ffffe, 
		0x80800001, 
		0x1f507b77, 
		0x8075eb4e, 
		0x00008000, 
		0x0c7ffffc, 
		0xbd4c4521, 
		0x0e003fff, 
		0x7c15ab67, 
		0x0c7f8000, 
		0x2815d730, 
		0x42480000, 
		0x41000000, 
		0x807ffffe, 
		0x9e596372, 
		0x0e07ffff, 
		0x55555555, 
		0xc0a00000, 
		0xc23c0000, 
		0x03ed894c, 
		0x80001000, 
		0x0f7fffff, 
		0x7f800000, 
		0x0e000001, 
		0x80000000, 
		0x80000004, 
		0x4189cefe, 
		0x0449f2aa, 
		0xaceabe3a, 
		0x8508cc9a, 
		0xc2100000, 
		0xc2240000, 
		0xc2500000, 
		0x00008000, 
		0x0e3fffff, 
		0xecdf0adb, 
		0x304e636a, 
		0x42480000, 
		0x00800001, 
		0x0d000ff0, 
		0x18cff9e5, 
		0xcccccccc, 
		0x0c7ffff0, 
		0xc20c0000, 
		0xd04ed817, 
		0xc0a00000, 
		0x41600000, 
		0x7e75a1fb, 
		0xffbfffff, 
		0x41800000, 
		0x0c7fffff, 
		0x5a33edc2, 
		0x00200000, 
		0x00000100, 
		0xff91faa2, 
		0x856516eb, 
		0x423bf1cd, 
		0x7f7fffff, 
		0x05a3f48c, 
		0xc2600000, 
		0xc1880000, 
		0x0e007fff, 
		0x8dfaacaa, 
		0x0e00000f, 
		0x6bc193fa, 
		0xc1a00000, 
		0xec3a6489, 
		0x80000800, 
		0xc0400000, 
		0x80000000, 
		0x7c9060c5, 
		0x0e0000ff, 
		0xf4a05bee, 
		0x41c00000, 
		0x0f7fffff, 
		0xc2740000, 
		0x0e00000f, 
		0x3f800001, 
		0x42100000, 
		0x0c7fff80, 
		0xaaaaaaaa, 
		0x0e0fffff, 
		0xece6e74c, 
		0x0e03ffff, 
		0x00000000, 
		0x003c14b1, 
		0x00004000, 
		0xf5f20ac6, 
		0x0c7ffff8, 
		0x8923f0c9, 
		0x007fffff, 
		0x00000004, 
		0x92784d76, 
		0x427c0000, 
		0x7f800000, 
		0x0e03ffff, 
		0x0e1fffff, 
		0x00011111, 
		0x971a14e4, 
		0xdbc20f47, 
		0xc3e1632c, 
		0x80000100, 
		0x04a4c125, 
		0x91790670, 
		0x80000001, 
		0x48c3c45a, 
		0x281aa5cc, 
		0xc1500000, 
		0x00040000, 
		0x7f7ffffe, 
		0xc2780000, 
		0xff866e62, 
		0x0c7ff800, 
		0x427c0000, 
		0x00000002, 
		0x42400000, 
		0x002267b4, 
		0xd2792439, 
		0x40e00000, 
		0x59a470bd, 
		0x8f7ffffe, 
		0x0c400000, 
		0x80000000, 
		0x427c0000, 
		0xc99c7e3f, 
		0x00000000, 
		0x80000080, 
		0x00000400, 
		0x0c7fc000, 
		0x00800000, 
		0xfd89a4a0, 
		0xff800000, 
		0xa48acd10, 
		0xc1e80000, 
		0xc1400000, 
		0x1511453f, 
		0xffc00001, 
		0x00020000, 
		0x80000040, 
		0xc22c0000, 
		0x0c7f0000, 
		0x0f7ffffe, 
		0x5d09b20e, 
		0x4f1c3fc5, 
		0x0c7fffc0, 
		0x80200000, 
		0x8b118941, 
		0x42080000, 
		0x4b000000, 
		0x80100000, 
		0xc2680000, 
		0xc2180000, 
		0x80000010, 
		0x7fc00000, 
		0x0c7ffffc, 
		0xbf800001, 
		0x41200000, 
		0xc1509f7e, 
		0x9cdebec8, 
		0xc1e00000, 
		0x39285564, 
		0xc1e80000, 
		0xcb000000, 
		0x00000000, 
		0x7f800001, 
		0x0c7fffc0, 
		0x80000001, 
		0xc2080000, 
		0xc1600000, 
		0x41a80000, 
		0xc891a620, 
		0x7f7fffff, 
		0xcd3f865a, 
		0x40c00000, 
		0x0f064efa, 
		0x0c7f8000, 
		0x33333333, 
		0xcccccccc, 
		0xc1400000, 
		0x00400000, 
		0x7f800000, 
		0x53fdd26f, 
		0x0c7ffff0, 
		0x8d7d1799, 
		0xc2500000, 
		0x861843c8, 
		0x2eab6b82, 
		0x41600000, 
		0xfb0d0f6f, 
		0x0e000003, 
		0x41700000, 
		0xc2780000, 
		0x80008000, 
		0xa8924cc3, 
		0x42500000, 
		0x7f800000, 
		0xc21c0000, 
		0x1292f3fa, 
		0x41f00000, 
		0x00000080, 
		0xa6c74019, 
		0x41880000, 
		0x3f800001, 
		0x0e003fff, 
		0xc2500000, 
		0x423c0000, 
		0x42600000, 
		0x7f800000, 
		0x00200000, 
		0x42440000, 
		0x46b72049, 
		0x80004000, 
		0x80000000, 
		0xc2700000, 
		0xffffffff, 
		0x80800001, 
		0xbae697f1, 
		0x9892abaa, 
		0x00000400, 
		0xcccccccc, 
		0x40e00000, 
		0x0c7fffc0, 
		0x6b85e9ac, 
		0x6c9cc3e5, 
		0x80008000, 
		0x7fa2cb5e, 
		0x41f00000, 
		0x80800003, 
		0x424c0000, 
		0x00010000, 
		0x41b80000, 
		0xc1b00000, 
		0x1e83cb51, 
		0x8f07b2fe, 
		0x4031fbdf, 
		0x80000020, 
		0x0e00ffff, 
		0xc2300000, 
		0xc1100000, 
		0x0e3fffff, 
		0x7f7ffffe, 
		0x4278914e, 
		0x00000000, 
		0x9c8d71e0, 
		0xc20c0000, 
		0xc1e00000, 
		0x807ffffe, 
		0x00800000, 
		0xc1100000, 
		0x4b1c0814, 
		0x7fbfffff, 
		0x80000000, 
		0x00000100, 
		0x7f800000, 
		0x41e80000, 
		0x00000000, 
		0xc6c62231, 
		0x19cdcfac, 
		0x41e00000, 
		0xc1c00000, 
		0x2975479f, 
		0xcdaf18fc, 
		0xc2240000, 
		0x0c7ffffc, 
		0x8f7fffff, 
		0x0c7ff000, 
		0xcccccccc, 
		0xc0000000, 
		0xc6165dc3, 
		0x00000000, 
		0xedff9fd8, 
		0x00800000, 
		0xc1500000, 
		0xaaaaaaaa, 
		0xc1900000, 
		0x42100000, 
		0x0e1fffff, 
		0x658ff0ad, 
		0x00000000, 
		0x40e00000, 
		0x7fc00001, 
		0x00800001, 
		0x00000100, 
		0x00000000, 
		0x0f7fffff, 
		0xc1900000, 
		0x7f800000, 
		0xff800000, 
		0x0e1fffff, 
		0x21e28437, 
		0xa79eadb2, 
		0xbf028f5c, 
		0x40800000, 
		0xffffffff, 
		0x002cf5d5, 
		0xffbfffff, 
		0x42380000, 
		0x80800001, 
		0x00098b30, 
		0x7fb31032, 
		0xc2100000, 
		0x42580000, 
		0xc1980000, 
		0xcc31a2f9, 
		0x80000001, 
		0x420c0000, 
		0x00000000, 
		0xc2780000, 
		0x0f567507, 
		0x7f7ffffe, 
		0x5363498a, 
		0x3a9f5c54, 
		0x1956fee2, 
		0x00000010, 
		0x00040000, 
		0x7f800000, 
		0xc1f00000, 
		0x00000001, 
		0x7f800001, 
		0xbf800001, 
		0x80000080, 
		0xf912e93a, 
		0x41100000, 
		0x4b8c4b40, 
		0xc1000000, 
		0x2fb7eee4, 
		0x42280000, 
		0xaaaaaaaa, 
		0xc21c0000, 
		0x7f800000, 
		0xc2440000, 
		0x00800003, 
		0x0e7fffff, 
		0x41600000, 
		0x00000800, 
		0x42280000, 
		0x7fffffff, 
		0x4b000000, 
		0x421c0000, 
		0xd1e67dd4, 
		0xffb7b2ae, 
		0x3d72f300, 
		0x42440000, 
		0x7b6e17ea, 
		0x42100000, 
		0xee51bb4b, 
		0xc1c80000, 
		0xc1400000, 
		0x42200000, 
		0x2aa302ef, 
		0x0c7ffff0, 
		0x00400000, 
		0xc1d80000, 
		0x1719f99e, 
		0xd6ce0099, 
		0x00000010, 
		0x0f7ffffe, 
		0x41028132, 
		0xc2240000, 
		0xff87591e, 
		0x0f7fffff, 
		0x41200000, 
		0xb4db2b22, 
		0x61a6b04f, 
		0x0e0000ff, 
		0x0e000fff, 
		0x42140000, 
		0x067f1d18, 
		0x00000200, 
		0x0e001fff, 
		0x00000080, 
		0xc0e00000, 
		0x1780f192, 
		0xc1600000, 
		0x41400000, 
		0xff800000, 
		0xf297c0d7, 
		0x80000000, 
		0xff800000, 
		0xc264ec98, 
		0x42340000, 
		0x7f800000, 
		0x0938a8fb, 
		0xff800000, 
		0xe41056d9, 
		0x00000000, 
		0xaa48b48b, 
		0x0c7ffe00, 
		0x41900000, 
		0x0c7ffff0, 
		0x8f7ffffd, 
		0x9280bda5, 
		0x595fbdb6, 
		0xcccccccc, 
		0xff99322c, 
		0x221ab594, 
		0xc20c0000, 
		0x80000100, 
		0xbf800000, 
		0xc2700000, 
		0x4162e797, 
		0xa6dac86d, 
		0x40400000, 
		0x201d78d8, 
		0xff7ffffe, 
		0x63b6ed35, 
		0x0e000001, 
		0x80100000, 
		0x0e0007ff, 
		0x41600000, 
		0x0e3fffff, 
		0x00000800, 
		0x00800001, 
		0x16ae5010, 
		0x9f3ff49a, 
		0x0c7fffc0, 
		0x80000000, 
		0x7f800000, 
		0x5cd1851c, 
		0x40800000, 
		0xbf028f5c, 
		0x30321efe, 
		0x36e6c5e3, 
		0x0012af3a, 
		0x5d62f40b, 
		0x24a27883, 
		0x311f47a7, 
		0x00000020, 
		0x0d00fff0, 
		0x41700000, 
		0xef881f56, 
		0xbbb7727e, 
		0x00000010, 
		0xb7ec70cf, 
		0x0c7fff00, 
		0x7f800000, 
		0x40800000, 
		0x80200000, 
		0x0ef89c0d, 
		0xa747fba4, 
		0xc1100000, 
		0xc2500000, 
		0x09cd6fe4, 
		0xec42edab, 
		0xb3f78f9f, 
		0x80000400, 
		0x80000400, 
		0x2b3b14d0, 
		0x3f028f5c, 
		0x777639b9, 
		0x98c9971b, 
		0x6f822cbf, 
		0x80000000, 
		0x00400000, 
		0xdf4d54f1, 
		0x007fffff, 
		0x452e23d7, 
		0xc1a80000, 
		0x2a9fd5c9, 
		0x8003dfec  
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
		0x0000039d, 
		0x000001a3, 
		0x00000029, 
		0x000000eb, 
		0x000001b1, 
		0x00000367, 
		0x000003f3, 
		0x00000121, 
		0x00000123, 
		0x00000147, 
		0x000001f1, 
		0x000003ff, 
		0x00000145, 
		0x00000159, 
		0x000001af, 
		0x0000025d, 
		0x0000021d, 
		0x000000bb, 
		0x0000018d, 
		0x00000241, 
		0x0000006d, 
		0x000003fd, 
		0x00000093, 
		0x0000005f, 
		0x0000018b, 
		0x0000000b, 
		0x0000008f, 
		0x00000183, 
		0x0000037b, 
		0x000001ed, 
		0x0000025b, 
		0x00000119, 
		0x000000df, 
		0x0000033b, 
		0x00000113, 
		0x00000291, 
		0x00000229, 
		0x000001c7, 
		0x0000000b, 
		0x000003d9, 
		0x000000e9, 
		0x000000ab, 
		0x000000b3, 
		0x000003fd, 
		0x0000015d, 
		0x00000067, 
		0x000001ff, 
		0x00000173, 
		0x00000143, 
		0x00000219, 
		0x0000014d, 
		0x000003ed, 
		0x000001c1, 
		0x000002f1, 
		0x0000031d, 
		0x00000271, 
		0x000003f5, 
		0x000001b5, 
		0x000001f9, 
		0x000002f3, 
		0x00000141, 
		0x000002bf, 
		0x000002cb, 
		0x000001d1, 
		0x00000011, 
		0x000000f3, 
		0x00000079, 
		0x00000231, 
		0x00000081, 
		0x00000341, 
		0x00000271, 
		0x000000e3, 
		0x00000037, 
		0x0000036d, 
		0x000002a9, 
		0x0000025d, 
		0x00000059, 
		0x00000089, 
		0x000002fd, 
		0x0000008f, 
		0x0000010b, 
		0x0000019f, 
		0x000003eb, 
		0x00000033, 
		0x00000273, 
		0x00000207, 
		0x00000219, 
		0x00000189, 
		0x000003c5, 
		0x00000313, 
		0x000003a1, 
		0x0000020b, 
		0x00000031, 
		0x00000239, 
		0x00000239, 
		0x00000117, 
		0x000002c9, 
		0x00000073, 
		0x000001bf, 
		0x000000bb, 
		0x00000227, 
		0x0000013d, 
		0x000001a9, 
		0x00000067, 
		0x000003f5, 
		0x000001d3, 
		0x00000149, 
		0x000000b5, 
		0x000000b3, 
		0x0000031b, 
		0x000002dd, 
		0x00000291, 
		0x000002e9, 
		0x00000087, 
		0x0000029d, 
		0x000002d5, 
		0x00000115, 
		0x00000357, 
		0x000000e7, 
		0x000003cf, 
		0x000000f5, 
		0x000002f5, 
		0x0000036d, 
		0x000002af, 
		0x00000393, 
		0x00000365, 
		0x000000e3, 
		0x000001f1, 
		0x000001ef, 
		0x0000011b, 
		0x00000271, 
		0x0000007f, 
		0x000000f9, 
		0x000003e7, 
		0x00000317, 
		0x000000e7, 
		0x00000147, 
		0x00000259, 
		0x00000379, 
		0x00000275, 
		0x000001bd, 
		0x00000031, 
		0x00000065, 
		0x0000020b, 
		0x000003c7, 
		0x000000e3, 
		0x0000012f, 
		0x000001c3, 
		0x00000011, 
		0x00000369, 
		0x0000007f, 
		0x00000225, 
		0x00000163, 
		0x00000121, 
		0x000001ad, 
		0x00000331, 
		0x00000077, 
		0x00000001, 
		0x000003cd, 
		0x0000032b, 
		0x0000003f, 
		0x00000381, 
		0x00000085, 
		0x00000085, 
		0x000002df, 
		0x0000009f, 
		0x00000051, 
		0x000002c7, 
		0x0000020d, 
		0x0000002d, 
		0x00000245, 
		0x000000e5, 
		0x0000011f, 
		0x00000097, 
		0x00000303, 
		0x0000020f, 
		0x000003a1, 
		0x00000393, 
		0x00000073, 
		0x000001f7, 
		0x000001b7, 
		0x0000021d, 
		0x00000393, 
		0x00000097, 
		0x0000020d, 
		0x00000153, 
		0x00000383, 
		0x00000167, 
		0x00000315, 
		0x0000011b, 
		0x00000059, 
		0x0000036d, 
		0x000003f7, 
		0x00000351, 
		0x0000004d, 
		0x00000181, 
		0x000000e9, 
		0x000002ab, 
		0x0000020d, 
		0x0000010f, 
		0x0000029f, 
		0x0000025f, 
		0x000003fb, 
		0x00000251, 
		0x000002ab, 
		0x000002fd, 
		0x00000391, 
		0x00000035, 
		0x000002e3, 
		0x00000059, 
		0x0000026f, 
		0x00000227, 
		0x00000185, 
		0x0000031f, 
		0x0000032b, 
		0x0000010b, 
		0x000000bf, 
		0x000000e7, 
		0x000000ff, 
		0x000001a5, 
		0x000001bd, 
		0x000002c9, 
		0x00000283, 
		0x00000371, 
		0x000003c9, 
		0x000003f1, 
		0x000000c5, 
		0x000003e5, 
		0x00000045, 
		0x000001ed, 
		0x00000071, 
		0x00000285, 
		0x00000093, 
		0x00000225, 
		0x0000014d, 
		0x000002b7, 
		0x000001cf, 
		0x0000034f, 
		0x000000c9, 
		0x0000032f, 
		0x000002d1, 
		0x0000007f, 
		0x000000b3, 
		0x000001a1, 
		0x000002c7, 
		0x000003bf, 
		0x000000d1, 
		0x0000038d, 
		0x0000010d, 
		0x000003b9, 
		0x00000297, 
		0x0000023f, 
		0x000002eb, 
		0x00000033, 
		0x00000303, 
		0x00000263  
};

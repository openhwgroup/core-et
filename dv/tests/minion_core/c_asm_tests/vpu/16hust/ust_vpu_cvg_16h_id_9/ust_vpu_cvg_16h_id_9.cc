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
			"li x5, 347\n"
			"faddi.pi f2, f0, 306\n"
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
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
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
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
			"li x0, 0x9\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"feqm.ps m5, f3, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 992(x19)\n"
			"flw.ps f1, 320(x19)\n"
			"fadd.ps f31, f11, f1, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 128(x10)\n"
			"flw.ps f13, 160(x10)\n"
			"fmadd.s f6, f3, f13, f13, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 736(x13)\n"
			"flw.ps f6, 384(x13)\n"
			"fmul.ps f29, f29, f6, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 832(x19)\n"
			"flw.ps f31, 480(x19)\n"
			"fsll.pi f20, f9, f31\n" 
			"li x14, 0x6d532580638159d6\n"
			"fcvt.s.w f9, x14, rne\n" 
			"li x19, 0xe943076b8b694ce6\n"
			"fcvt.s.w f3, x19, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"feqm.ps m6, f9, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 288(x25)\n"
			"flw.ps f29, 416(x25)\n"
			"fsub.s f6, f1, f29, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 768(x18)\n"
			"flw.ps f21, 192(x18)\n"
			"fsgnjx.ps f8, f3, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"mov.m.x m5, x14, 0x40\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xfc8d6e4701\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f8, 96(x14)\n"
			"flw.ps f11, 896(x14)\n"
			"fmulhu.pi f11, f8, f11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_0:\n"
			"masknot m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_4_HID_0:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 928(x17)\n"
			"fexp.ps f8, f29\n" 
			"maskpopc x26, m7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 0(x26)\n"
			"flw.ps f10, 256(x26)\n"
			"flw.ps f31, 864(x26)\n"
			"fnmsub.s f1, f12, f10, f31, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_174568_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0xc7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x0 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1d << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x9 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_0:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 576(x25)\n"
			"fexp.ps f21, f11\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 352(x19)\n"
			"fcvt.ps.pwu f10, f23, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 544(x14)\n"
			"flw.ps f1, 96(x14)\n"
			"fcmovm.ps f13, f15, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 896(x25)\n"
			"flw.ps f23, 384(x25)\n"
			"fmsub.ps f3, f6, f23, f6, rne\n" 
			"li x5, 0x7c8e\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 192(x6)\n"
			"flw.ps f20, 960(x6)\n"
			"fsgnjx.ps f1, f31, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 864(x13)\n"
			"fnot.pi f10, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 640(x13)\n"
			"flw.ps f9, 896(x13)\n"
			"fsll.pi f21, f23, f9\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 416(x6)\n"
			"flw.ps f6, 448(x6)\n"
			"for.pi f9, f29, f6\n" 
			"li x5, 0x7a3a20ff07cc22bc\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x32cef28d42313b5d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 64(x13)\n"
			"flw.ps f3, 160(x13)\n"
			"flw.ps f29, 384(x13)\n"
			"fmsub.ps f13, f8, f3, f29, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 224(x17)\n"
			"flw.ps f9, 416(x17)\n"
			"fmul.s f13, f21, f9, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_0:\n"
			"mova.m.x x17\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f8, 896(x17)\n"
			"flw.ps f2, 832(x17)\n"
			"fsll.pi f12, f8, f2\n" 
			"li x10, 0x47e4ae96daf06897\n"
			"fcvt.s.l f29, x10, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_0:\n"
			"fltm.ps m6, f20, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_16\n"
			"flw.ps f31, 0(x14)\n"
			"fcvt.ps.f16 f23, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_0:\n"
			"flem.ps m0, f15, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_91063_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xa << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x0 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
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
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_0:\n"
			"fltm.ps m0, f3, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 448(x17)\n"
			"fcvt.pwu.ps f10, f13, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_0:\n"
			"maskand m7, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f2, 0x5d4c0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_0:\n"
			"fsetm.pi m7, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 768(x13)\n"
			"fclass.s x10, f1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 448(x10)\n"
			"flw.ps f9, 416(x10)\n"
			"fle.ps f1, f3, f9\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 512(x27)\n"
			"fcvt.pw.ps f1, f29, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 960(x14)\n"
			"ffrc.ps f8, f13\n" 
			"li x5, 0x3cc000d2d4f22fe7\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xaf600569f833ea38\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x25, rand_int_32\n"
			"flw.ps f13, 480(x25)\n"
			"flw.ps f23, 96(x25)\n"
			"flt.pi f3, f13, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_12_HID_0:\n"
			"maskand m5, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f3, 832(x19)\n"
			"fcvt.ps.pw f15, f3, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 864(x10)\n"
			"flw.ps f3, 608(x10)\n"
			"fle.ps f23, f21, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 896(x17)\n"
			"flw.ps f11, 224(x17)\n"
			"flt.pi f23, f13, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 64(x26)\n"
			"fswizz.ps f13, f1, 0x18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 256(x14)\n"
			"faddi.pi f2, f20, 0x13a\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x6 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xd << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1d << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xf << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_HPM_3_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_INST_PASS_HID_0:\n"          
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
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 704(x26)\n"
			"flw.ps f22, 320(x26)\n"
			"fcmovm.ps f15, f15, f22\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 352(x17)\n"
			"flw.ps f7, 352(x17)\n"
			"fle.pi f0, f8, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 0(x14)\n"
			"flw.ps f29, 192(x14)\n"
			"fmax.pi f26, f2, f29\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 64(x13)\n"
			"flw.ps f20, 960(x13)\n"
			"fsgnjx.s f29, f14, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 256(x15)\n"
			"fsrai.pi f9, f29, 0x8\n" 
			"mova.x.m x11\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 992(x11)\n"
			"frcp.ps f11, f11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fltm.pi m2, f6, f12\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 768(x18)\n"
			"flw.ps f9, 800(x18)\n"
			"fsrl.pi f0, f1, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 672(x23)\n"
			"flw.ps f20, 960(x23)\n"
			"fsub.pi f6, f22, f20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 288(x11)\n"
			"flw.ps f5, 224(x11)\n"
			"fmaxu.pi f22, f22, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 224(x7)\n"
			"fcvt.w.s x26, f20, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f5, 640(x26)\n"
			"flw.ps f7, 992(x26)\n"
			"fremu.pi f0, f5, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 832(x23)\n"
			"flw.ps f9, 640(x23)\n"
			"fminu.pi f14, f11, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 288(x26)\n"
			"fpackrepb.pi f9, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 576(x14)\n"
			"fmin.ps f5, f12, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 320(x18)\n"
			"flw.ps f20, 928(x18)\n"
			"feq.ps f14, f6, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 800(x14)\n"
			"flw.ps f9, 224(x14)\n"
			"fsub.pi f11, f22, f9\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_1:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 224(x23)\n"
			"fexp.ps f15, f20\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f8, 928(x14)\n"
			"fcvt.ps.f16 f12, f8\n" 
			"la x13, rand_int_32\n"
			"flw.ps f7, 128(x13)\n"
			"flw.ps f22, 832(x13)\n"
			"fmulh.pi f1, f7, f22\n" 
			"la x13, rand_int_32\n"
			"flw.ps f9, 384(x13)\n"
			"fcvt.ps.pw f1, f9, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 96(x18)\n"
			"faddi.pi f2, f15, 0x1df\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 512(x7)\n"
			"flw.ps f14, 608(x7)\n"
			"fdiv.pi f8, f0, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 320(x14)\n"
			"flw.ps f22, 672(x14)\n"
			"feq.pi f15, f29, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 800(x26)\n"
			"flw.ps f1, 800(x26)\n"
			"fsgnj.s f26, f8, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 640(x14)\n"
			"flw.ps f1, 352(x14)\n"
			"fmaxu.pi f26, f9, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 576(x17)\n"
			"flw.ps f29, 864(x17)\n"
			"fcmovm.ps f5, f15, f29\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 544(x15)\n"
			"frsq.ps f12, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 544(x17)\n"
			"flw.ps f8, 224(x17)\n"
			"fsgnj.ps f12, f0, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 288(x15)\n"
			"fcvt.ps.pwu f11, f26, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 384(x14)\n"
			"flw.ps f15, 448(x14)\n"
			"fdivu.pi f2, f2, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f14, 448(x13)\n"
			"for.pi f29, f14, f14\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 32(x18)\n"
			"flw.ps f9, 32(x18)\n"
			"fsgnjn.s f0, f7, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 928(x23)\n"
			"flw.ps f14, 896(x23)\n"
			"feq.pi f0, f5, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 704(x11)\n"
			"flw.ps f26, 800(x11)\n"
			"flt.pi f9, f20, f26\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 448(x16)\n"
			"flw.ps f20, 256(x16)\n"
			"flw.ps f6, 640(x16)\n"
			"fnmadd.s f15, f5, f20, f6, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_1:\n"
			"maskor m3, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_1:\n"
			"maskand m5, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x171f8746be\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f22, 0x2f0f8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 160(x14)\n"
			"flw.ps f20, 288(x14)\n"
			"fmul.pi f1, f1, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 544(x23)\n"
			"fsrli.pi f0, f9, 0x7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 64(x23)\n"
			"flw.ps f0, 480(x23)\n"
			"fmul.ps f0, f9, f0, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 544(x13)\n"
			"fswizz.ps f14, f22, 0x7\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 64(x7)\n"
			"flw.ps f22, 416(x7)\n"
			"fminu.pi f14, f7, f22\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 64(x7)\n"
			"flw.ps f7, 224(x7)\n"
			"fcmov.ps f5, f12, f7, f7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 384(x15)\n"
			"flw.ps f20, 160(x15)\n"
			"fsub.pi f9, f1, f20\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 480(x16)\n"
			"flw.ps f26, 768(x16)\n"
			"fand.pi f7, f5, f26\n" 
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
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
			"LBL_HPM_4_CORE_L2_MISS_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f5,  416 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
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
			"la x10, rand_int_32\n"
			"flw.ps f11, 288(x10)\n"
			"fslli.pi f11, f11, 0xa\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 832(x15)\n"
			"flw.ps f13, 864(x15)\n"
			"frem.pi f12, f8, f13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 704(x5)\n"
			"flw.ps f4, 608(x5)\n"
			"flw.ps f6, 160(x5)\n"
			"fmadd.s f11, f3, f4, f6, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 448(x13)\n"
			"frsq.ps f3, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 704(x25)\n"
			"flw.ps f8, 160(x25)\n"
			"flw.ps f3, 448(x25)\n"
			"fmadd.s f13, f4, f8, f3, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 96(x10)\n"
			"flw.ps f3, 224(x10)\n"
			"fsgnjx.ps f1, f31, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 192(x25)\n"
			"flw.ps f24, 384(x25)\n"
			"fle.pi f4, f3, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 288(x26)\n"
			"flw.ps f8, 352(x26)\n"
			"fmax.pi f3, f29, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 704(x28)\n"
			"flw.ps f31, 128(x28)\n"
			"fadd.pi f3, f9, f31\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 480(x13)\n"
			"flw.ps f11, 256(x13)\n"
			"fsub.ps f12, f8, f11, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 960(x12)\n"
			"fsub.s f24, f6, f6, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 608(x12)\n"
			"flw.ps f25, 896(x12)\n"
			"flw.ps f29, 512(x12)\n"
			"fnmadd.ps f11, f13, f25, f29, rdn\n" 
			"li x27, 0xbdcc2ff2ba2de706\n"
			"fcvt.s.w f13, x27, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 704(x28)\n"
			"flw.ps f26, 832(x28)\n"
			"feq.s x28, f11, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskand m7, m4, m1\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 896(x26)\n"
			"fcvt.wu.s x15, f25, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x4 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1e << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xf << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_2:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 480(x5)\n"
			"flog.ps f31, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"maskand m4, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f9, 544(x28)\n"
			"fsatu8.pi f16, f9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 288(x12)\n"
			"flw.ps f12, 256(x12)\n"
			"fsrl.pi f31, f3, f12\n" 
			"la x12, rand_ieee754_16\n"
			"flw.ps f11, 704(x12)\n"
			"fcvt.ps.f16 f25, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 160(x5)\n"
			"flw.ps f1, 256(x5)\n"
			"fand.pi f31, f25, f1\n" 
			"li x27, 0x59f0e3f6f053ccca\n"
			"fcvt.s.w f4, x27, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"flem.ps m1, f26, f24\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2bd87b1015\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"mov.m.x m1, x28, 0xa\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5ea05b60c0\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 544(x13)\n"
			"frsq.ps f9, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 512(x10)\n"
			"fslli.pi f25, f30, 0x4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 128(x13)\n"
			"flw.ps f12, 896(x13)\n"
			"fcmovm.ps f13, f30, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 448(x28)\n"
			"flw.ps f11, 384(x28)\n"
			"fsub.s f31, f25, f11, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 896(x28)\n"
			"flw.ps f26, 480(x28)\n"
			"fsll.pi f6, f31, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_2:\n"
			"maskor m7, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f11, 480(x15)\n"
			"fsrli.pi f31, f11, 0xb\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_655822_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4d << TENSOR_IMA_SCP_LOC_B) | (0x79 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x2 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x6 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1e << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xa << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"maskand m1, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 896(x12)\n"
			"flw.ps f24, 640(x12)\n"
			"fmin.ps f3, f6, f24\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_2:\n"
			"fltm.ps m7, f8, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 832(x12)\n"
			"flw.ps f25, 608(x12)\n"
			"fsgnj.s f9, f31, f25\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 256(x28)\n"
			"fandi.pi f24, f6, 0x28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 576(x11)\n"
			"flw.ps f4, 224(x11)\n"
			"fsub.ps f31, f30, f4, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f12, 800(x12)\n"
			"fmulh.pi f6, f12, f12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_int_32\n"
			"flw.ps f16, 192(x15)\n"
			"flw.ps f24, 32(x15)\n"
			"fmax.pi f11, f16, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 896(x27)\n"
			"fcvt.pw.ps f1, f24, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 480(x11)\n"
			"flw.ps f29, 736(x11)\n"
			"fadd.pi f29, f16, f29\n" 
			"li x5, 0x1677\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f24, 96(x26)\n"
			"flw.ps f8, 288(x26)\n"
			"feq.pi f1, f24, f8\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_2:\n"
			"maskor m4, m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x5, m6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 576(x26)\n"
			"flw.ps f6, 256(x26)\n"
			"fmaxu.pi f30, f24, f6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 64(x12)\n"
			"flw.ps f25, 224(x12)\n"
			"fmulh.pi f13, f13, f25\n" 
			"fbci.ps f24, 0x1bc52\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_625906_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_933912_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xc << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x3 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x63 << TENSOR_FMA32_SCP_LOC_B) | (0xd1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x4 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x42 << TENSOR_FMA32_SCP_LOC_B) | (0x9a << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 608(x14)\n"
			"flw.ps f25, 64(x14)\n"
			"fmax.ps f11, f18, f25\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 576(x12)\n"
			"flw.ps f4, 320(x12)\n"
			"flw.ps f18, 320(x12)\n"
			"fmsub.s f2, f20, f4, f18, rmm\n" 
			"fbci.ps f3, 0x5b1ba\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 0(x12)\n"
			"fcvt.wu.s x9, f18, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 32(x27)\n"
			"flw.ps f2, 192(x27)\n"
			"fsub.ps f21, f25, f2, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 192(x27)\n"
			"fpackrepb.pi f14, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 960(x11)\n"
			"flw.ps f18, 128(x11)\n"
			"fsgnj.ps f21, f25, f18\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 960(x10)\n"
			"flw.ps f11, 960(x10)\n"
			"feq.s x9, f21, f11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 896(x18)\n"
			"flw.ps f0, 832(x18)\n"
			"fdivu.pi f25, f11, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 640(x9)\n"
			"flw.ps f21, 0(x9)\n"
			"fcmovm.ps f18, f9, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 480(x9)\n"
			"fcvt.ps.pwu f14, f3, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f16, 896(x14)\n"
			"flw.ps f15, 320(x14)\n"
			"fsll.pi f14, f16, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 256(x10)\n"
			"fpackreph.pi f18, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 448(x14)\n"
			"fnot.pi f21, f14\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 896(x20)\n"
			"fsrai.pi f26, f0, 0xc\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 352(x18)\n"
			"fcvt.pwu.ps f4, f3, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f14, 384(x9)\n"
			"fnot.pi f11, f14\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 384(x10)\n"
			"fcvt.pwu.ps f15, f9, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 352(x20)\n"
			"fsgnj.s f11, f18, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f25, 64(x9)\n"
			"flw.ps f7, 416(x9)\n"
			"fsll.pi f8, f25, f7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_0_HID_3:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 352(x23)\n"
			"flog.ps f11, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_3:\n"
			"fltm.ps m0, f21, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f0, 256(x27)\n"
			"flw.ps f25, 416(x27)\n"
			"fltu.pi f4, f0, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 256(x27)\n"
			"flw.ps f0, 224(x27)\n"
			"fmax.s f21, f21, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 640(x14)\n"
			"flw.ps f0, 768(x14)\n"
			"fcmovm.ps f18, f7, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 768(x20)\n"
			"flw.ps f15, 224(x20)\n"
			"feq.ps f9, f3, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 736(x12)\n"
			"flw.ps f20, 192(x12)\n"
			"fcmovm.ps f8, f0, f20\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 960(x9)\n"
			"flw.ps f9, 928(x9)\n"
			"fsrl.pi f3, f26, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 352(x9)\n"
			"flw.ps f14, 480(x9)\n"
			"fle.ps f14, f9, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"fltm.ps m5, f11, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f15, 352(x20)\n"
			"flw.ps f16, 128(x20)\n"
			"fminu.pi f4, f15, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 160(x9)\n"
			"flw.ps f8, 32(x9)\n"
			"fmul.s f9, f15, f8, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 800(x14)\n"
			"flw.ps f16, 96(x14)\n"
			"for.pi f9, f18, f16\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 128(x12)\n"
			"fsatu8.pi f18, f7\n" 
			"maskpopcz x11, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x27, rand_int_32\n"
			"flw.ps f7, 352(x27)\n"
			"fnot.pi f0, f7\n" 
			"li x18, 0x47197dd22e7c2f3e\n"
			"fcvt.s.lu f0, x18, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 608(x14)\n"
			"flw.ps f0, 256(x14)\n"
			"flw.ps f20, 384(x14)\n"
			"fnmadd.s f2, f8, f0, f20, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 832(x16)\n"
			"frsq.ps f20, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 384(x11)\n"
			"flw.ps f3, 480(x11)\n"
			"fnmadd.ps f9, f21, f3, f21, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 0(x27)\n"
			"flw.ps f26, 576(x27)\n"
			"fsub.ps f11, f15, f26, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 96(x27)\n"
			"fsqrt.s f3, f9, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_3:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 224(x11)\n"
			"frcp.ps f9, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 320(x6)\n"
			"flw.ps f18, 800(x6)\n"
			"fsrl.pi f18, f4, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 224(x20)\n"
			"flw.ps f16, 576(x20)\n"
			"flw.ps f3, 384(x20)\n"
			"fmadd.ps f18, f11, f16, f3, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_3:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f3, 128(x23)\n"
			"fsat8.pi f8, f3\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f27, 864(x27)\n"
			"flw.ps f10, 384(x27)\n"
			"fminu.pi f24, f27, f10\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 96(x17)\n"
			"flw.ps f20, 256(x17)\n"
			"fsrl.pi f25, f9, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 448(x18)\n"
			"flw.ps f23, 448(x18)\n"
			"flw.ps f11, 384(x18)\n"
			"fnmsub.ps f9, f3, f23, f11, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f23, 704(x22)\n"
			"fpackrepb.pi f15, f23\n" 
			"li x5, 0x5857\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskor m7, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 640(x15)\n"
			"flw.ps f20, 928(x15)\n"
			"fmax.ps f24, f5, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 320(x26)\n"
			"flw.ps f30, 0(x26)\n"
			"flw.ps f15, 160(x26)\n"
			"fmsub.ps f30, f19, f30, f15, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 992(x10)\n"
			"fcvt.w.s x18, f19, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 224(x15)\n"
			"flw.ps f9, 32(x15)\n"
			"fsgnjx.s f3, f23, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 832(x10)\n"
			"flw.ps f10, 0(x10)\n"
			"fmul.s f25, f27, f10, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 480(x12)\n"
			"flw.ps f27, 896(x12)\n"
			"fsub.s f24, f24, f27, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 640(x10)\n"
			"flw.ps f11, 384(x10)\n"
			"fsrl.pi f23, f29, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 640(x22)\n"
			"flw.ps f9, 416(x22)\n"
			"fxor.pi f5, f27, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 160(x12)\n"
			"flw.ps f25, 288(x12)\n"
			"flw.ps f2, 416(x12)\n"
			"fnmsub.s f30, f8, f25, f2, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 128(x15)\n"
			"flw.ps f8, 160(x15)\n"
			"fnmadd.s f11, f30, f8, f30, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 448(x22)\n"
			"fcvt.ps.pwu f19, f9, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x2 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xd << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x2 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 192(x17)\n"
			"flw.ps f23, 768(x17)\n"
			"flw.ps f9, 128(x17)\n"
			"fnmadd.ps f20, f2, f23, f9, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 96(x7)\n"
			"fsqrt.ps f8, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 96(x18)\n"
			"fcvt.wu.s x15, f19, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 768(x7)\n"
			"fandi.pi f27, f29, 0x163\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 704(x7)\n"
			"flw.ps f5, 864(x7)\n"
			"fadd.s f11, f25, f5, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 256(x10)\n"
			"flw.ps f15, 352(x10)\n"
			"fsub.ps f3, f11, f15, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 704(x10)\n"
			"flw.ps f10, 576(x10)\n"
			"flt.ps f23, f8, f10\n" 
			"li x5, 0x4834\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 352(x10)\n"
			"flw.ps f27, 128(x10)\n"
			"fcmovm.ps f5, f11, f27\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f5, 512(x27)\n"
			"fcvt.ps.f16 f20, f5\n" 
			"li x27, 0xec1a58769d6c781b\n"
			"fcvt.s.w f3, x27, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 256(x17)\n"
			"flw.ps f24, 928(x17)\n"
			"fxor.pi f24, f10, f24\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 128(x17)\n"
			"flw.ps f8, 32(x17)\n"
			"fadd.ps f9, f9, f8, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_4:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 576(x27)\n"
			"flog.ps f5, f25\n" 
			"la x22, rand_int_32\n"
			"flw.ps f2, 512(x22)\n"
			"fcvt.ps.pwu f10, f2, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 992(x27)\n"
			"flw.ps f5, 896(x27)\n"
			"fadd.s f11, f10, f5, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"fltm.ps m5, f9, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x2 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x5 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 576(x27)\n"
			"flw.ps f19, 192(x27)\n"
			"fsub.ps f2, f9, f19, rup\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_4:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 544(x7)\n"
			"frcp.ps f25, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 352(x15)\n"
			"flw.ps f15, 96(x15)\n"
			"fadd.ps f11, f11, f15, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 288(x17)\n"
			"fcvt.pw.ps f19, f25, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 896(x10)\n"
			"flw.ps f30, 384(x10)\n"
			"fle.ps f27, f8, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 384(x26)\n"
			"fsin.ps f15, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"feqm.ps m2, f20, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 704(x26)\n"
			"flw.ps f15, 256(x26)\n"
			"fmin.pi f2, f27, f15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 928(x22)\n"
			"flw.ps f5, 64(x22)\n"
			"fmax.ps f29, f8, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 480(x15)\n"
			"flw.ps f27, 672(x15)\n"
			"fsll.pi f24, f2, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 224(x22)\n"
			"flw.ps f9, 352(x22)\n"
			"fsgnjx.ps f29, f3, f9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 928(x10)\n"
			"flw.ps f2, 448(x10)\n"
			"fle.pi f23, f30, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 608(x25)\n"
			"flw.ps f25, 192(x25)\n"
			"fadd.ps f27, f19, f25, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_int_32\n"
			"flw.ps f2, 288(x15)\n"
			"fsrai.pi f11, f2, 0x6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 480(x7)\n"
			"flw.ps f19, 640(x7)\n"
			"flw.ps f30, 128(x7)\n"
			"fnmadd.ps f25, f20, f19, f30, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 512(x10)\n"
			"flw.ps f23, 448(x10)\n"
			"fltu.pi f2, f19, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x0 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1f << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xf << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x5 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xa << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xd << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x5 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x4 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1b << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x1 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 992(x9)\n"
			"fcvt.w.s x18, f19, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 800(x14)\n"
			"flw.ps f21, 0(x14)\n"
			"fcmovm.ps f28, f11, f21\n" 
			"fbci.ps f4, 0x6c2cd\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 64(x18)\n"
			"fround.ps f22, f17, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 608(x17)\n"
			"flw.ps f19, 320(x17)\n"
			"fmulh.pi f31, f24, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 96(x17)\n"
			"flw.ps f28, 864(x17)\n"
			"fmin.pi f28, f10, f28\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 352(x22)\n"
			"flw.ps f11, 160(x22)\n"
			"flt.pi f24, f16, f11\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 96(x12)\n"
			"flw.ps f17, 640(x12)\n"
			"flt.pi f31, f11, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 416(x5)\n"
			"flw.ps f26, 0(x5)\n"
			"fsub.ps f26, f31, f26, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 608(x17)\n"
			"flw.ps f22, 96(x17)\n"
			"fmax.ps f28, f4, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"fsetm.pi m0, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 800(x7)\n"
			"fcvt.pwu.ps f14, f17, rdn\n" 
			"li x14, 0x91d33e0ed0305351\n"
			"fcvt.s.w f11, x14, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_5:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 768(x11)\n"
			"flog.ps f24, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 640(x11)\n"
			"fpackreph.pi f28, f0\n" 
			"fbci.pi f17, 0x4be25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 672(x10)\n"
			"flw.ps f11, 544(x10)\n"
			"flw.ps f0, 896(x10)\n"
			"fcmov.ps f24, f10, f11, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 448(x12)\n"
			"flw.ps f22, 32(x12)\n"
			"fmul.ps f4, f14, f22, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 128(x22)\n"
			"flw.ps f19, 320(x22)\n"
			"flt.s x17, f17, f19\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 736(x10)\n"
			"flw.ps f22, 704(x10)\n"
			"fsgnj.ps f10, f0, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 800(x17)\n"
			"flw.ps f31, 512(x17)\n"
			"fsgnj.s f21, f11, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 960(x5)\n"
			"fcvt.pwu.ps f22, f2, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 96(x12)\n"
			"fmulhu.pi f21, f19, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"feqm.ps m0, f11, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 64(x22)\n"
			"flw.ps f0, 480(x22)\n"
			"flw.ps f12, 544(x22)\n"
			"fnmsub.s f19, f21, f0, f12, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 32(x17)\n"
			"flw.ps f26, 448(x17)\n"
			"fsra.pi f24, f28, f26\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 96(x12)\n"
			"flw.ps f26, 224(x12)\n"
			"fmulh.pi f2, f11, f26\n" 
			"la x11, rand_int_32\n"
			"flw.ps f24, 384(x11)\n"
			"fpackrepb.pi f31, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"fltm.ps m0, f28, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 992(x5)\n"
			"fsin.ps f17, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 768(x9)\n"
			"fsrli.pi f26, f19, 0x5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 160(x22)\n"
			"flw.ps f4, 224(x22)\n"
			"flw.ps f11, 608(x22)\n"
			"fnmsub.ps f0, f21, f4, f11, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 448(x9)\n"
			"flw.ps f2, 672(x9)\n"
			"fsub.ps f26, f14, f2, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 640(x22)\n"
			"fcvt.pw.ps f12, f4, rne\n" 
			"mova.x.m x14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 32(x14)\n"
			"fswizz.ps f19, f14, 0x1e\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 672(x11)\n"
			"flw.ps f12, 96(x11)\n"
			"fmax.pi f19, f21, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 32(x14)\n"
			"fsin.ps f10, f2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 160(x9)\n"
			"flw.ps f11, 928(x9)\n"
			"fmul.pi f31, f22, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 64(x7)\n"
			"fclass.s x9, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 32(x22)\n"
			"flw.ps f16, 384(x22)\n"
			"flw.ps f19, 672(x22)\n"
			"fnmsub.ps f4, f4, f16, f19, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 928(x5)\n"
			"flw.ps f4, 384(x5)\n"
			"fle.ps f26, f28, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 160(x12)\n"
			"fcvt.ps.pwu f26, f11, rup\n" 
			"mova.x.m x7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 960(x22)\n"
			"flw.ps f31, 224(x22)\n"
			"fmul.ps f14, f10, f31, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 960(x5)\n"
			"flw.ps f14, 160(x5)\n"
			"fle.ps f0, f16, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 160(x22)\n"
			"flw.ps f14, 960(x22)\n"
			"for.pi f10, f28, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 64(x14)\n"
			"flw.ps f12, 256(x14)\n"
			"fsgnjn.s f17, f24, f12\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"maskxor m6, m5, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa6259215c6\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"fsetm.pi m6, f29\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x28d3c7b023\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 32(x17)\n"
			"flw.ps f13, 256(x17)\n"
			"fsub.s f12, f11, f13, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 800(x25)\n"
			"fandi.pi f0, f16, 0x30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 320(x9)\n"
			"fpackrepb.pi f29, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"masknot m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 288(x16)\n"
			"flw.ps f30, 576(x16)\n"
			"flw.ps f11, 544(x16)\n"
			"fnmadd.ps f14, f7, f30, f11, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 480(x17)\n"
			"flw.ps f7, 832(x17)\n"
			"fnmsub.s f23, f0, f7, f0, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 320(x17)\n"
			"flw.ps f28, 160(x17)\n"
			"feq.pi f23, f15, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_6:\n"
			"fltm.pi m5, f16, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f11, 448(x17)\n"
			"flw.ps f28, 896(x17)\n"
			"fsra.pi f7, f11, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 640(x22)\n"
			"flw.ps f1, 128(x22)\n"
			"fadd.s f0, f11, f1, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 544(x9)\n"
			"for.pi f27, f23, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 768(x5)\n"
			"fcvt.wu.s x17, f16, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 288(x22)\n"
			"flw.ps f13, 480(x22)\n"
			"for.pi f16, f1, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 352(x16)\n"
			"flw.ps f10, 864(x16)\n"
			"fsub.s f12, f28, f10, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x6 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f30, 416(x18)\n"
			"fsatu8.pi f30, f30\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 320(x13)\n"
			"flw.ps f23, 480(x13)\n"
			"fxor.pi f30, f10, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 960(x9)\n"
			"frsq.ps f14, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 320(x22)\n"
			"flw.ps f7, 64(x22)\n"
			"fle.ps f28, f16, f7\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 704(x5)\n"
			"fcvt.ps.pw f12, f7, rtz\n" 
			"li x5, 0x7d9149543de4315\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3eca369ffc012aeb\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f22, 192(x17)\n"
			"fslli.pi f30, f22, 0x7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f28, 768(x28)\n"
			"flw.ps f29, 800(x28)\n"
			"fmaxu.pi f22, f28, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"maskxor m4, m5, m7\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 480(x16)\n"
			"flw.ps f7, 672(x16)\n"
			"flw.ps f16, 544(x16)\n"
			"fnmadd.ps f16, f10, f7, f16, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 96(x16)\n"
			"flw.ps f14, 160(x16)\n"
			"flw.ps f10, 32(x16)\n"
			"fmsub.s f30, f13, f14, f10, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 768(x28)\n"
			"fcvt.w.s x9, f14, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"fltm.pi m5, f27, f7\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 480(x17)\n"
			"fpackrepb.pi f0, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 288(x22)\n"
			"flw.ps f13, 448(x22)\n"
			"fsgnjn.s f12, f27, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 352(x23)\n"
			"flw.ps f16, 288(x23)\n"
			"fsub.pi f28, f0, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 640(x17)\n"
			"flw.ps f14, 544(x17)\n"
			"flw.ps f7, 672(x17)\n"
			"fmadd.s f14, f23, f14, f7, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x0 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x6 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x18 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x0 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 0(x14)\n"
			"fround.ps f23, f29, rup\n" 
			"li x5, 0xec35bb00dd289ccc\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xaeab2f9ab95e4760\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x16, rand_int_32\n"
			"flw.ps f28, 704(x16)\n"
			"fsrli.pi f16, f28, 0x4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 384(x17)\n"
			"fsat8.pi f13, f10\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 800(x5)\n"
			"flw.ps f16, 576(x5)\n"
			"fltu.pi f1, f10, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 864(x25)\n"
			"fcvt.w.s x22, f28, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 192(x16)\n"
			"flw.ps f13, 736(x16)\n"
			"fadd.s f14, f1, f13, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 160(x23)\n"
			"fswizz.ps f14, f7, 0x6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 288(x23)\n"
			"fpackreph.pi f1, f27\n" 
			"la x13, rand_int_32\n"
			"flw.ps f13, 128(x13)\n"
			"flw.ps f15, 0(x13)\n"
			"fmulhu.pi f27, f13, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 800(x23)\n"
			"flw.ps f27, 448(x23)\n"
			"fmin.s f22, f23, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 512(x22)\n"
			"fsqrt.ps f7, f29\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 992(x23)\n"
			"flw.ps f16, 672(x23)\n"
			"fsll.pi f27, f11, f16\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 736(x17)\n"
			"flw.ps f1, 288(x17)\n"
			"fand.pi f29, f22, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 992(x18)\n"
			"frsq.ps f27, f28\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 768(x18)\n"
			"fandi.pi f10, f15, 0x1db\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4b << TENSOR_IMA_SCP_LOC_B) | (0x5f << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 960(x19)\n"
			"flw.ps f31, 32(x19)\n"
			"fle.s x9, f7, f31\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 864(x14)\n"
			"frcp.ps f4, f8\n" 
			"la x9, rand_int_32\n"
			"flw.ps f17, 416(x9)\n"
			"flw.ps f4, 704(x9)\n"
			"fsra.pi f12, f17, f4\n" 
			"li x14, 0x26deaf1620a7bb63\n"
			"fcvt.s.l f27, x14, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 864(x9)\n"
			"flw.ps f31, 864(x9)\n"
			"fmin.ps f12, f6, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 608(x14)\n"
			"flw.ps f6, 960(x14)\n"
			"fsll.pi f17, f24, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 544(x11)\n"
			"fnot.pi f15, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 256(x19)\n"
			"flw.ps f8, 928(x19)\n"
			"fsgnjn.ps f30, f17, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 256(x7)\n"
			"flw.ps f7, 288(x7)\n"
			"flw.ps f12, 256(x7)\n"
			"fcmov.ps f31, f10, f7, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 128(x22)\n"
			"flw.ps f27, 800(x22)\n"
			"flw.ps f4, 320(x22)\n"
			"fnmsub.ps f6, f5, f27, f4, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 928(x22)\n"
			"fcvt.ps.pw f23, f17, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 640(x28)\n"
			"fcvt.f16.ps f31, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 480(x7)\n"
			"fmulh.pi f11, f10, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 96(x7)\n"
			"flw.ps f17, 160(x7)\n"
			"fxor.pi f5, f4, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 768(x9)\n"
			"fcvt.f16.ps f11, f11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"flem.ps m1, f15, f25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9fd2490011\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 544(x22)\n"
			"flw.ps f10, 960(x22)\n"
			"fadd.s f8, f17, f10, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 224(x22)\n"
			"flw.ps f8, 320(x22)\n"
			"flw.ps f17, 64(x22)\n"
			"fnmsub.s f17, f23, f8, f17, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 896(x19)\n"
			"flw.ps f27, 320(x19)\n"
			"fle.ps f24, f8, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 544(x18)\n"
			"flw.ps f10, 320(x18)\n"
			"flw.ps f8, 896(x18)\n"
			"fmadd.ps f27, f11, f10, f8, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 608(x19)\n"
			"flw.ps f23, 960(x19)\n"
			"fmax.ps f31, f5, f23\n" 
			"fbci.pi f6, 0x45d2e\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 96(x9)\n"
			"flw.ps f25, 512(x9)\n"
			"fsgnj.s f6, f24, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 384(x11)\n"
			"fsrai.pi f4, f5, 0x7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_7:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 192(x17)\n"
			"frcp.ps f27, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 320(x28)\n"
			"ffrc.ps f27, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 736(x11)\n"
			"flw.ps f11, 64(x11)\n"
			"fmax.pi f5, f10, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 384(x22)\n"
			"frsq.ps f25, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 576(x9)\n"
			"flw.ps f25, 288(x9)\n"
			"fsgnjx.s f25, f11, f25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 832(x22)\n"
			"flw.ps f31, 160(x22)\n"
			"fmax.s f11, f24, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 224(x26)\n"
			"flw.ps f12, 384(x26)\n"
			"feq.ps f27, f27, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_7:\n"
			"masknot m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 32(x18)\n"
			"flw.ps f30, 128(x18)\n"
			"feq.ps f31, f15, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_7:\n"
			"mov.m.x m3, x26, 0x40\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb7dab86d8a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_7:\n"
			"flem.ps m5, f8, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 864(x17)\n"
			"flw.ps f31, 32(x17)\n"
			"fmin.ps f24, f7, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 960(x18)\n"
			"fsgnjn.ps f23, f4, f4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 608(x17)\n"
			"flw.ps f6, 64(x17)\n"
			"fltu.pi f11, f10, f6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"flem.ps m5, f10, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 832(x14)\n"
			"flw.ps f31, 736(x14)\n"
			"flw.ps f17, 448(x14)\n"
			"fnmadd.ps f24, f7, f31, f17, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 128(x14)\n"
			"flw.ps f30, 352(x14)\n"
			"fle.pi f5, f25, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f6, 224(x22)\n"
			"flw.ps f5, 576(x22)\n"
			"fsra.pi f8, f6, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 384(x9)\n"
			"flw.ps f12, 960(x9)\n"
			"fadd.pi f25, f7, f12\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 416(x18)\n"
			"fcvt.f16.ps f10, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 960(x17)\n"
			"fsin.ps f25, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 768(x14)\n"
			"flw.ps f27, 96(x14)\n"
			"fsrl.pi f10, f23, f27\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 192(x9)\n"
			"fsat8.pi f12, f4\n" 
			"li x22, 0x9875ac43e0bec3fc\n"
			"fcvt.s.l f25, x22, rtz\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 192(x18)\n"
			"flw.ps f28, 320(x18)\n"
			"fcmovm.ps f9, f31, f28\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 192(x16)\n"
			"flw.ps f10, 384(x16)\n"
			"feq.pi f20, f9, f10\n" 
			"li x5, 0xc9b5\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f6, 128(x26)\n"
			"flw.ps f5, 800(x26)\n"
			"fsub.pi f25, f6, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 800(x25)\n"
			"flw.ps f15, 736(x25)\n"
			"fsgnj.ps f25, f20, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f8, 96(x26)\n"
			"flw.ps f26, 0(x26)\n"
			"flt.pi f6, f8, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 608(x26)\n"
			"flw.ps f4, 736(x26)\n"
			"fmul.ps f14, f20, f4, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 384(x28)\n"
			"flw.ps f5, 672(x28)\n"
			"flw.ps f6, 896(x28)\n"
			"fmadd.ps f14, f15, f5, f6, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"flem.ps m0, f26, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"fsetm.pi m0, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x9, 0xdef54b821c933046\n"
			"fcvt.s.w f9, x9, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 736(x10)\n"
			"flw.ps f10, 896(x10)\n"
			"fmulhu.pi f1, f31, f10\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 832(x20)\n"
			"flw.ps f1, 640(x20)\n"
			"fsra.pi f14, f6, f1\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 672(x16)\n"
			"flw.ps f4, 32(x16)\n"
			"fsrl.pi f1, f6, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_8:\n"
			"mov.m.x m7, x20, 0x5f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 192(x25)\n"
			"fcvt.pw.ps f28, f14, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 480(x9)\n"
			"fswizz.ps f5, f25, 0x19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x9b34\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f5, 896(x18)\n"
			"fslli.pi f15, f5, 0x7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f29, 256(x16)\n"
			"frsq.ps f5, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"maskxor m4, m1, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f6, 768(x14)\n"
			"flw.ps f15, 192(x14)\n"
			"fand.pi f1, f6, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 672(x9)\n"
			"flw.ps f29, 608(x9)\n"
			"fsub.ps f4, f31, f29, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 800(x9)\n"
			"flw.ps f1, 32(x9)\n"
			"feq.ps f9, f8, f1\n" 
			"mova.x.m x12\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 480(x16)\n"
			"flw.ps f9, 320(x16)\n"
			"fltu.pi f6, f26, f9\n" 
			"li x5, 0x9dcef046aed80942\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x26afa92e4a225397\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f20, 256(x10)\n"
			"flw.ps f10, 608(x10)\n"
			"fmulhu.pi f4, f20, f10\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 544(x12)\n"
			"fcvt.pwu.ps f28, f15, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 96(x26)\n"
			"flw.ps f5, 736(x26)\n"
			"flt.s x12, f26, f5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 288(x25)\n"
			"fslli.pi f25, f15, 0x8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 960(x16)\n"
			"flw.ps f14, 416(x16)\n"
			"flw.ps f31, 512(x16)\n"
			"fmsub.s f15, f28, f14, f31, rne\n" 
			"li x25, 0x801fb8c4a1536858\n"
			"fcvt.s.w f30, x25, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 352(x26)\n"
			"flw.ps f9, 448(x26)\n"
			"flt.ps f31, f14, f9\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 192(x9)\n"
			"fcvt.ps.pw f25, f28, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x3 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xb << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x0 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_8:\n"
			"fltm.pi m4, f9, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x20, 0x932285be73a3ccb4\n"
			"fcvt.s.w f8, x20, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 128(x14)\n"
			"flw.ps f15, 544(x14)\n"
			"fsgnjn.ps f30, f9, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 992(x25)\n"
			"flw.ps f26, 768(x25)\n"
			"fle.s x12, f29, f26\n" 
			"li x5, 0xca85\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f6, 32(x10)\n"
			"fslli.pi f20, f6, 0x5\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 736(x10)\n"
			"fsrai.pi f9, f30, 0xe\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 672(x9)\n"
			"flw.ps f26, 192(x9)\n"
			"fsub.ps f6, f29, f26, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 896(x14)\n"
			"flw.ps f31, 832(x14)\n"
			"fsrl.pi f5, f26, f31\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 416(x18)\n"
			"flw.ps f26, 480(x18)\n"
			"flt.pi f10, f8, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 928(x9)\n"
			"fadd.ps f25, f30, f30, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 512(x10)\n"
			"flw.ps f14, 224(x10)\n"
			"feq.pi f28, f1, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 352(x28)\n"
			"fandi.pi f30, f26, 0x7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 640(x10)\n"
			"frsq.ps f4, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 768(x18)\n"
			"flw.ps f14, 992(x18)\n"
			"fmax.s f5, f28, f14\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 0(x10)\n"
			"flw.ps f6, 704(x10)\n"
			"fsll.pi f30, f28, f6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 480(x10)\n"
			"flw.ps f9, 384(x10)\n"
			"flw.ps f5, 800(x10)\n"
			"fmsub.ps f1, f31, f9, f5, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xb << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1d << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xa << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f2, 448(x5)\n"
			"fpackreph.pi f21, f2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 192(x22)\n"
			"fround.ps f21, f19, rtz\n" 
			"mova.x.m x14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 448(x26)\n"
			"ffrc.ps f31, f31\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 960(x7)\n"
			"fsat8.pi f25, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"feqm.ps m0, f19, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 288(x11)\n"
			"flw.ps f21, 384(x11)\n"
			"fmsub.s f30, f19, f21, f21, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 800(x7)\n"
			"flw.ps f2, 256(x7)\n"
			"fmul.ps f22, f30, f2, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 0(x28)\n"
			"flw.ps f2, 352(x28)\n"
			"fminu.pi f15, f22, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 192(x6)\n"
			"fcvt.w.s x14, f21, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 928(x22)\n"
			"flw.ps f11, 352(x22)\n"
			"fsgnjx.ps f11, f5, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 320(x28)\n"
			"flw.ps f12, 160(x28)\n"
			"flw.ps f4, 832(x28)\n"
			"fmadd.s f11, f0, f12, f4, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 576(x5)\n"
			"flw.ps f12, 480(x5)\n"
			"fcmovm.ps f19, f30, f12\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 768(x22)\n"
			"flw.ps f12, 800(x22)\n"
			"fsub.pi f15, f19, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 64(x20)\n"
			"frsq.ps f13, f25\n" 
			"la x20, rand_ieee754_16\n"
			"flw.ps f30, 256(x20)\n"
			"fcvt.ps.f16 f13, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 608(x11)\n"
			"flw.ps f21, 704(x11)\n"
			"fadd.ps f12, f22, f21, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 384(x14)\n"
			"fsin.ps f11, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_9:\n"
			"flem.ps m5, f12, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 352(x22)\n"
			"flw.ps f11, 928(x22)\n"
			"fmsub.ps f12, f21, f11, f11, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 128(x28)\n"
			"fnot.pi f19, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 224(x14)\n"
			"fclass.s x7, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 0(x10)\n"
			"fsrai.pi f13, f19, 0xc\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 704(x20)\n"
			"fexp.ps f5, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 768(x7)\n"
			"flw.ps f13, 960(x7)\n"
			"flw.ps f4, 512(x7)\n"
			"fcmov.ps f13, f25, f13, f4\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 32(x11)\n"
			"fmulh.pi f19, f28, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 672(x10)\n"
			"flw.ps f4, 576(x10)\n"
			"fsll.pi f4, f22, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 224(x28)\n"
			"flw.ps f31, 384(x28)\n"
			"fle.pi f19, f0, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 416(x14)\n"
			"fandi.pi f22, f19, 0x16b\n" 
			"fbci.pi f25, 0x532c0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 416(x22)\n"
			"flw.ps f15, 0(x22)\n"
			"fsub.ps f31, f22, f15, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 992(x10)\n"
			"flw.ps f12, 576(x10)\n"
			"flt.ps f13, f28, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 0(x26)\n"
			"flw.ps f19, 480(x26)\n"
			"fadd.ps f12, f0, f19, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_9:\n"
			"mov.m.x m1, x11, 0x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_9:\n"
			"fltm.ps m1, f2, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 96(x7)\n"
			"fcvt.f16.ps f25, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 192(x5)\n"
			"flw.ps f15, 960(x5)\n"
			"fsub.s f31, f31, f15, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 896(x10)\n"
			"flw.ps f5, 96(x10)\n"
			"fmul.ps f28, f11, f5, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 480(x11)\n"
			"flw.ps f5, 256(x11)\n"
			"fmul.pi f2, f0, f5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_9:\n"
			"masknot m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_9:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 832(x20)\n"
			"flog.ps f19, f22\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_9:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 384(x6)\n"
			"flog.ps f19, f0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 928(x6)\n"
			"flw.ps f5, 32(x6)\n"
			"fsgnj.s f4, f0, f5\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 96(x7)\n"
			"flw.ps f15, 704(x7)\n"
			"fmulhu.pi f21, f12, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 288(x26)\n"
			"flw.ps f30, 960(x26)\n"
			"fremu.pi f18, f25, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 384(x11)\n"
			"flw.ps f31, 0(x11)\n"
			"fsrl.pi f18, f15, f31\n" 
			"fbci.ps f25, 0x24c7e\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 160(x7)\n"
			"flw.ps f4, 896(x7)\n"
			"fsll.pi f12, f25, f4\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f27,  192 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f16,  992 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 992(x11)\n"
			"flw.ps f23, 960(x11)\n"
			"fle.s x16, f4, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fltm.pi m6, f5, f27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4706cc3fda\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 96(x12)\n"
			"fexp.ps f27, f4\n" 
			"li x5, 0xd7c4\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"fltm.pi m3, f15, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 352(x12)\n"
			"ffrc.ps f11, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 224(x20)\n"
			"flw.ps f11, 224(x20)\n"
			"fdiv.s f4, f4, f11, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 352(x17)\n"
			"flw.ps f14, 672(x17)\n"
			"fnmadd.ps f28, f11, f14, f14, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f23, 992(x17)\n"
			"fsrai.pi f15, f23, 0xc\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 288(x11)\n"
			"fsatu8.pi f0, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 960(x17)\n"
			"fcvt.wu.s x11, f15, rup\n" 
			"li x5, 0x3df0\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f3, 128(x11)\n"
			"flw.ps f30, 96(x11)\n"
			"fsrl.pi f3, f3, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_10:\n"
			"masknot m2, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f3, 832(x19)\n"
			"flw.ps f11, 576(x19)\n"
			"fsub.pi f21, f3, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 992(x7)\n"
			"flw.ps f0, 32(x7)\n"
			"fmin.pi f15, f21, f0\n" 
			"li x17, 0xb2b431b8041cce7e\n"
			"fcvt.s.wu f5, x17, rdn\n" 
			"li x5, 0x4139\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f4, 256(x20)\n"
			"fandi.pi f27, f4, 0xba\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f7, 96(x10)\n"
			"flw.ps f10, 32(x10)\n"
			"for.pi f30, f7, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 160(x6)\n"
			"flw.ps f21, 960(x6)\n"
			"fadd.ps f7, f14, f21, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"fsetm.pi m2, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f7, 576(x19)\n"
			"fcvt.ps.pwu f11, f7, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 192(x10)\n"
			"flw.ps f10, 544(x10)\n"
			"fle.ps f11, f21, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 224(x7)\n"
			"flw.ps f11, 32(x7)\n"
			"fmulhu.pi f30, f27, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_10:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 480(x12)\n"
			"flw.ps f4, 960(x12)\n"
			"fsgnj.ps f31, f31, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_10:\n"
			"maskand m3, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401D8\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 448(x16)\n"
			"frsq.ps f11, f14\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 672(x10)\n"
			"flw.ps f5, 896(x10)\n"
			"fsgnjx.s f16, f3, f5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_10:\n"
			"fsetm.pi m0, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 352(x6)\n"
			"flw.ps f5, 928(x6)\n"
			"fmin.ps f15, f0, f5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 352(x27)\n"
			"flw.ps f15, 320(x27)\n"
			"fle.ps f14, f31, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f30, 992(x12)\n"
			"fmax.pi f30, f30, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 352(x19)\n"
			"fsra.pi f7, f7, f7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x1 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x63 << TENSOR_IMA_SCP_LOC_B) | (0xeb << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f27, 64(x20)\n"
			"flw.ps f7, 32(x20)\n"
			"fmul.pi f7, f27, f7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 224(x12)\n"
			"flw.ps f15, 160(x12)\n"
			"flw.ps f4, 96(x12)\n"
			"fnmadd.ps f11, f16, f15, f4, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 864(x16)\n"
			"fcvt.pwu.ps f7, f16, rne\n" 
			"li x5, 0x5d24\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f4, 768(x10)\n"
			"fandi.pi f21, f4, 0xb6\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 608(x16)\n"
			"fpackreph.pi f27, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 32(x17)\n"
			"flw.ps f23, 864(x17)\n"
			"flw.ps f31, 96(x17)\n"
			"fmsub.s f3, f4, f23, f31, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 608(x12)\n"
			"flw.ps f23, 640(x12)\n"
			"flt.ps f28, f10, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 384(x7)\n"
			"flw.ps f4, 448(x7)\n"
			"fle.s x10, f0, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 192(x19)\n"
			"fcvt.wu.s x27, f21, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 160(x12)\n"
			"flw.ps f3, 512(x12)\n"
			"fle.ps f7, f11, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 768(x6)\n"
			"flw.ps f14, 224(x6)\n"
			"fsra.pi f4, f4, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 384(x12)\n"
			"fswizz.ps f5, f10, 0x0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 896(x11)\n"
			"flw.ps f5, 736(x11)\n"
			"fmulhu.pi f21, f10, f5\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 576(x16)\n"
			"flw.ps f7, 0(x16)\n"
			"fdiv.pi f3, f3, f7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 64(x19)\n"
			"flw.ps f11, 832(x19)\n"
			"fadd.ps f23, f31, f11, rmm\n" 
			"fbci.pi f11, 0x5c47\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"masknot m1, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 864(x11)\n"
			"flw.ps f8, 96(x11)\n"
			"fsgnj.ps f17, f0, f8\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_11:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 256(x14)\n"
			"fexp.ps f18, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"mova.m.x x13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 448(x13)\n"
			"flw.ps f0, 896(x13)\n"
			"fle.s x10, f21, f0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 960(x16)\n"
			"fswizz.ps f20, f17, 0x18\n" 
			"maskpopc x22, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 256(x18)\n"
			"fcvt.w.s x11, f31, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 160(x19)\n"
			"fcvt.pw.ps f8, f31, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_11:\n"
			"mov.m.x m2, x22, 0x4e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_11:\n"
			"feqm.ps m2, f19, f7\n" 
			"li x5, 0x80340130\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_int_32\n"
			"flw.ps f19, 160(x16)\n"
			"flw.ps f6, 640(x16)\n"
			"feq.pi f15, f19, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 928(x13)\n"
			"flw.ps f7, 896(x13)\n"
			"fmul.s f6, f11, f7, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 896(x6)\n"
			"flt.s x17, f8, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 160(x16)\n"
			"flw.ps f31, 576(x16)\n"
			"flw.ps f26, 480(x16)\n"
			"fmsub.ps f6, f11, f31, f26, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 192(x11)\n"
			"flw.ps f0, 672(x11)\n"
			"flw.ps f15, 736(x11)\n"
			"fnmadd.ps f7, f22, f0, f15, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 960(x19)\n"
			"flw.ps f20, 288(x19)\n"
			"flw.ps f22, 416(x19)\n"
			"fnmadd.s f31, f2, f20, f22, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 928(x14)\n"
			"flw.ps f19, 0(x14)\n"
			"fminu.pi f22, f8, f19\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 160(x16)\n"
			"flw.ps f26, 32(x16)\n"
			"feq.pi f8, f6, f26\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 320(x18)\n"
			"flw.ps f19, 704(x18)\n"
			"fsra.pi f2, f11, f19\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 704(x11)\n"
			"fsrli.pi f11, f21, 0x1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 288(x22)\n"
			"fswizz.ps f0, f20, 0x15\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 192(x16)\n"
			"faddi.pi f18, f7, 0x8c\n" 
			"maskpopc x19, m1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 160(x11)\n"
			"flw.ps f17, 832(x11)\n"
			"fsgnj.ps f17, f0, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 896(x18)\n"
			"flw.ps f2, 512(x18)\n"
			"flt.s x16, f17, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 736(x18)\n"
			"fround.ps f31, f20, rtz\n" 
			"maskpopcz x17, m6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 672(x13)\n"
			"flw.ps f7, 0(x13)\n"
			"flw.ps f19, 64(x13)\n"
			"fmadd.ps f20, f30, f7, f19, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 576(x19)\n"
			"fadd.ps f18, f0, f0, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 32(x13)\n"
			"fcvt.ps.pw f30, f15, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_11:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 832(x13)\n"
			"fexp.ps f15, f19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 192(x22)\n"
			"flw.ps f21, 544(x22)\n"
			"fsgnjx.s f31, f31, f21\n" 
			"la x11, rand_int_32\n"
			"flw.ps f17, 224(x11)\n"
			"flw.ps f18, 256(x11)\n"
			"fsra.pi f21, f17, f18\n" 
			"fbci.ps f17, 0x252ee\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 512(x19)\n"
			"fsqrt.s f20, f22, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 512(x13)\n"
			"flw.ps f19, 384(x13)\n"
			"fle.s x11, f2, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 192(x18)\n"
			"fround.ps f6, f15, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 320(x11)\n"
			"flw.ps f8, 704(x11)\n"
			"fadd.pi f2, f22, f8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 992(x17)\n"
			"fpackrepb.pi f22, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 928(x16)\n"
			"fsqrt.ps f7, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 224(x6)\n"
			"flw.ps f18, 704(x6)\n"
			"flw.ps f11, 64(x6)\n"
			"fmsub.ps f22, f0, f18, f11, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 736(x17)\n"
			"flw.ps f18, 352(x17)\n"
			"flw.ps f11, 0(x17)\n"
			"fmadd.ps f26, f7, f18, f11, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 864(x16)\n"
			"flw.ps f15, 768(x16)\n"
			"fsub.ps f6, f26, f15, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 704(x11)\n"
			"flw.ps f15, 256(x11)\n"
			"fnmadd.ps f26, f26, f15, f26, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 928(x13)\n"
			"flw.ps f22, 64(x13)\n"
			"flw.ps f17, 416(x13)\n"
			"fmsub.ps f22, f15, f22, f17, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 64(x6)\n"
			"flw.ps f6, 576(x6)\n"
			"fand.pi f15, f17, f6\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_12:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 768(x18)\n"
			"flog.ps f6, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 736(x25)\n"
			"flw.ps f21, 992(x25)\n"
			"fsgnjn.s f21, f27, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 512(x22)\n"
			"flw.ps f12, 0(x22)\n"
			"fmin.s f27, f8, f12\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 64(x11)\n"
			"flw.ps f10, 640(x11)\n"
			"flw.ps f2, 64(x11)\n"
			"fnmadd.ps f29, f12, f10, f2, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 608(x14)\n"
			"flw.ps f18, 736(x14)\n"
			"fmsub.s f14, f14, f18, f18, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"fltm.ps m4, f13, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"fsetm.pi m4, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f29, 416(x14)\n"
			"fltu.pi f10, f29, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 896(x18)\n"
			"flw.ps f29, 192(x18)\n"
			"fdiv.s f28, f19, f29, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 960(x25)\n"
			"flw.ps f13, 512(x25)\n"
			"flw.ps f2, 224(x25)\n"
			"fnmsub.ps f31, f8, f13, f2, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 128(x28)\n"
			"flw.ps f31, 992(x28)\n"
			"flw.ps f18, 192(x28)\n"
			"fmadd.ps f2, f24, f31, f18, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_12:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 800(x22)\n"
			"flog.ps f2, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 896(x11)\n"
			"flw.ps f0, 896(x11)\n"
			"fsub.ps f8, f29, f0, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f31, 256(x7)\n"
			"fsat8.pi f10, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 704(x5)\n"
			"flw.ps f12, 992(x5)\n"
			"fcmov.ps f31, f2, f12, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 128(x14)\n"
			"flw.ps f21, 960(x14)\n"
			"fsub.ps f27, f0, f21, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_33545_HID_12:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x2 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x3 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x18 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x4 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x5 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xa << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_12:\n"
			"maskxor m2, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 640(x6)\n"
			"fsqrt.ps f2, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_12:\n"
			"masknot m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc769fafcc8\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f6, 864(x18)\n"
			"fsatu8.pi f18, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 448(x6)\n"
			"fsub.ps f13, f21, f21, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_12:\n"
			"fltm.pi m4, f13, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 576(x25)\n"
			"flw.ps f2, 0(x25)\n"
			"flw.ps f29, 96(x25)\n"
			"fmsub.ps f31, f0, f2, f29, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 352(x28)\n"
			"flw.ps f24, 320(x28)\n"
			"fsra.pi f12, f13, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 864(x25)\n"
			"frsq.ps f31, f6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 64(x22)\n"
			"flw.ps f18, 512(x22)\n"
			"feq.ps f10, f21, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 960(x15)\n"
			"flw.ps f14, 384(x15)\n"
			"fle.ps f12, f19, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f10, 896(x28)\n"
			"flw.ps f29, 320(x28)\n"
			"fsrl.pi f2, f10, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_12:\n"
			"mova.m.x x18\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_12:\n"
			"fsetm.pi m3, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_9_HID_12:\n"
			"fsetm.pi m4, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_10_HID_12:\n"
			"fsetm.pi m4, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x8b << TENSOR_FMA32_SCP_LOC_B) | (0xfc << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_12:\n"
			"feqm.ps m3, f14, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_12_HID_12:\n"
			"mov.m.x m5, x14, 0x3d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_13_HID_12:\n"
			"mov.m.x m2, x25, 0x7e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 32(x28)\n"
			"fswizz.ps f14, f14, 0x19\n" 
			"li x5, 0x34bb\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f28, 960(x6)\n"
			"fsrli.pi f27, f28, 0x5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 448(x7)\n"
			"flw.ps f14, 512(x7)\n"
			"flw.ps f28, 352(x7)\n"
			"fnmsub.ps f12, f12, f14, f28, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 896(x22)\n"
			"flw.ps f2, 832(x22)\n"
			"fmadd.ps f14, f24, f24, f2, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f2, 448(x5)\n"
			"fpackreph.pi f14, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f13, 544(x25)\n"
			"flw.ps f31, 224(x25)\n"
			"fadd.pi f14, f13, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_14_HID_12:\n"
			"maskor m4, m2, m4\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"la x6, rand_int_32\n"
			"flw.ps f18, 416(x6)\n"
			"flw.ps f10, 480(x6)\n"
			"fand.pi f0, f18, f10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 480(x6)\n"
			"flw.ps f24, 288(x6)\n"
			"fsll.pi f28, f12, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 992(x25)\n"
			"flw.ps f13, 992(x25)\n"
			"fmul.s f6, f2, f13, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_15_HID_12:\n"
			"maskxor m5, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f14, 832(x14)\n"
			"flw.ps f12, 480(x14)\n"
			"fmul.pi f21, f14, f12\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 960(x28)\n"
			"fsatu8.pi f10, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb1 << TENSOR_IMA_SCP_LOC_B) | (0xea << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
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
			"la x28, rand_int_32\n"
			"flw.ps f13, 32(x28)\n"
			"fmin.pi f1, f13, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 480(x28)\n"
			"flw.ps f18, 992(x28)\n"
			"fadd.s f14, f24, f18, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 704(x17)\n"
			"flw.ps f0, 928(x17)\n"
			"fltu.pi f17, f9, f0\n" 
			"mova.x.m x23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 736(x5)\n"
			"flw.ps f28, 64(x5)\n"
			"flt.s x19, f18, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 256(x17)\n"
			"flw.ps f24, 544(x17)\n"
			"fsub.ps f14, f9, f24, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"masknot m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 800(x19)\n"
			"flw.ps f29, 288(x19)\n"
			"flt.ps f14, f9, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 352(x23)\n"
			"fsub.s f30, f6, f6, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 864(x16)\n"
			"fcvt.ps.pw f21, f24, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"fltm.pi m3, f13, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 576(x17)\n"
			"flw.ps f21, 832(x17)\n"
			"fadd.ps f9, f28, f21, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 352(x19)\n"
			"flw.ps f1, 928(x19)\n"
			"fxor.pi f12, f24, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 512(x17)\n"
			"flw.ps f0, 704(x17)\n"
			"feq.s x17, f15, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 480(x11)\n"
			"flw.ps f17, 0(x11)\n"
			"fdiv.ps f1, f12, f17, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 704(x28)\n"
			"flw.ps f13, 736(x28)\n"
			"fmul.pi f1, f1, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 832(x17)\n"
			"flw.ps f1, 800(x17)\n"
			"flw.ps f28, 480(x17)\n"
			"fmadd.ps f1, f17, f1, f28, rup\n" 
			"mova.x.m x16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"maskxor m4, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 160(x19)\n"
			"flw.ps f9, 704(x19)\n"
			"fmax.ps f6, f13, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 704(x26)\n"
			"fcvt.pw.ps f18, f30, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 320(x11)\n"
			"flw.ps f24, 960(x11)\n"
			"fdiv.ps f28, f28, f24, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"mov.m.x m4, x26, 0x1c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 160(x23)\n"
			"flw.ps f18, 736(x23)\n"
			"fsgnjx.ps f24, f15, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 192(x11)\n"
			"flw.ps f6, 896(x11)\n"
			"feq.pi f15, f1, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 512(x28)\n"
			"flw.ps f9, 128(x28)\n"
			"fsra.pi f13, f14, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"maskand m4, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 576(x26)\n"
			"flw.ps f30, 32(x26)\n"
			"fle.s x17, f29, f30\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f12, 800(x19)\n"
			"fcvt.ps.f16 f15, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 224(x25)\n"
			"flw.ps f9, 224(x25)\n"
			"flw.ps f18, 192(x25)\n"
			"fnmsub.ps f10, f21, f9, f18, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 768(x11)\n"
			"fpackreph.pi f13, f30\n" 
			"li x25, 0x71cbbdbf5ea708a1\n"
			"fcvt.s.w f18, x25, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"maskor m1, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 384(x19)\n"
			"fclass.ps f17, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"maskxor m4, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f9, 192(x17)\n"
			"fpackreph.pi f17, f9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 544(x19)\n"
			"fclass.ps f14, f17\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 256(x23)\n"
			"flw.ps f24, 416(x23)\n"
			"fsgnjx.s f21, f15, f24\n" 
			"la x16, rand_int_32\n"
			"flw.ps f24, 864(x16)\n"
			"fnot.pi f30, f24\n" 
			"maskpopc x17, m3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 992(x28)\n"
			"fcvt.pw.ps f13, f24, rup\n" 
			"li x5, 0xad103fccd5b80444\n"
			"fcvt.s.wu f28, x5, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 960(x5)\n"
			"flw.ps f28, 288(x5)\n"
			"fsub.pi f18, f21, f28\n" 
			"maskpopc x25, m5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_13:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 448(x5)\n"
			"fcvt.pw.ps f13, f24, rne\n" 
			"maskpopcz x23, m1\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 704(x22)\n"
			"fsat8.pi f0, f28\n" 
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
			"csrwi mhpmevent3, 28\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 14\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
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
			"la x7, rand_int_32\n"
			"flw.ps f26, 224(x7)\n"
			"flw.ps f31, 704(x7)\n"
			"fxor.pi f31, f26, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 288(x27)\n"
			"flw.ps f19, 960(x27)\n"
			"fsub.ps f20, f3, f19, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"flem.ps m1, f22, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 992(x12)\n"
			"flw.ps f28, 832(x12)\n"
			"flt.s x13, f25, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 480(x14)\n"
			"flw.ps f22, 32(x14)\n"
			"fmaxu.pi f30, f31, f22\n" 
			"la x22, rand_int_32\n"
			"flw.ps f5, 640(x22)\n"
			"flw.ps f19, 96(x22)\n"
			"fsub.pi f31, f5, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 896(x14)\n"
			"flw.ps f30, 64(x14)\n"
			"feq.s x13, f4, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 32(x27)\n"
			"flw.ps f5, 448(x27)\n"
			"fsra.pi f22, f28, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f30, 224(x15)\n"
			"flw.ps f5, 576(x15)\n"
			"flt.pi f5, f30, f5\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 32(x6)\n"
			"flw.ps f19, 608(x6)\n"
			"fand.pi f3, f28, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"maskor m1, m2, m6\n" 
			"li x5, 0x80340130\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_int_32\n"
			"flw.ps f30, 768(x15)\n"
			"flw.ps f22, 800(x15)\n"
			"for.pi f20, f30, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 416(x13)\n"
			"flw.ps f30, 96(x13)\n"
			"fmadd.s f4, f0, f0, f30, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 352(x22)\n"
			"flw.ps f23, 32(x22)\n"
			"for.pi f26, f19, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 704(x28)\n"
			"flw.ps f0, 256(x28)\n"
			"flt.pi f17, f22, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 448(x28)\n"
			"fcvt.pw.ps f4, f23, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x12, rand_int_32\n"
			"flw.ps f0, 800(x12)\n"
			"fsrli.pi f17, f0, 0x3\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 160(x27)\n"
			"flw.ps f16, 832(x27)\n"
			"feq.pi f23, f4, f16\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 768(x27)\n"
			"flw.ps f30, 832(x27)\n"
			"feq.pi f28, f23, f30\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 960(x6)\n"
			"fpackreph.pi f4, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 0(x7)\n"
			"fround.ps f22, f31, rtz\n" 
			"li x5, 0x461410a619deab76\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd1e73c73d4be57e2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"maskor m7, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 992(x20)\n"
			"flw.ps f13, 608(x20)\n"
			"fmin.s f0, f4, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 832(x27)\n"
			"flw.ps f28, 128(x27)\n"
			"fle.s x12, f20, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 928(x28)\n"
			"flw.ps f0, 288(x28)\n"
			"feq.ps f4, f13, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 512(x13)\n"
			"flw.ps f4, 672(x13)\n"
			"fmax.ps f20, f5, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 736(x15)\n"
			"flw.ps f0, 576(x15)\n"
			"feq.ps f28, f19, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 896(x14)\n"
			"fsin.ps f28, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 448(x6)\n"
			"flw.ps f20, 128(x6)\n"
			"fsgnjn.s f5, f5, f20\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 960(x15)\n"
			"flw.ps f17, 736(x15)\n"
			"flw.ps f5, 160(x15)\n"
			"fmsub.s f3, f31, f17, f5, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 768(x5)\n"
			"fround.ps f16, f13, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 544(x5)\n"
			"flw.ps f28, 192(x5)\n"
			"fxor.pi f5, f5, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x4 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x0 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1c << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x2 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 96(x22)\n"
			"fsin.ps f25, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 576(x14)\n"
			"fsatu8.pi f0, f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 704(x15)\n"
			"flw.ps f28, 736(x15)\n"
			"flw.ps f31, 352(x15)\n"
			"fmsub.s f26, f19, f28, f31, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 800(x15)\n"
			"flw.ps f28, 864(x15)\n"
			"fand.pi f23, f22, f28\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 96(x20)\n"
			"flw.ps f17, 288(x20)\n"
			"feq.pi f4, f0, f17\n" 
			"li x5, 0x273d\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_14:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 32(x6)\n"
			"frcp.ps f3, f30\n" 
			"li x5, 0x2d8bb51a5d6ab458\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x54c21d81ca8ed526\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f22, 160(x28)\n"
			"fsrai.pi f30, f22, 0x8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 512(x7)\n"
			"flw.ps f13, 384(x7)\n"
			"fmax.s f0, f31, f13\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 512(x22)\n"
			"flw.ps f25, 992(x22)\n"
			"fadd.pi f3, f22, f25\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 800(x20)\n"
			"flw.ps f0, 992(x20)\n"
			"flt.pi f23, f31, f0\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 96(x13)\n"
			"flw.ps f23, 928(x13)\n"
			"fmin.pi f23, f19, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 288(x13)\n"
			"flw.ps f22, 992(x13)\n"
			"for.pi f23, f19, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f22, 608(x12)\n"
			"flw.ps f0, 896(x12)\n"
			"fmul.pi f31, f22, f0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 256(x27)\n"
			"flw.ps f28, 448(x27)\n"
			"fle.ps f30, f0, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 320(x14)\n"
			"flw.ps f19, 352(x14)\n"
			"fsll.pi f20, f31, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 640(x12)\n"
			"fsrli.pi f26, f3, 0xb\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x4 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x3 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x19 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x2 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x3 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x3 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x3 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x18 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x2 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 10\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 192(x20)\n"
			"fclass.ps f21, f18\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 960(x25)\n"
			"fcvt.ps.pwu f13, f27, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 768(x13)\n"
			"flw.ps f30, 480(x13)\n"
			"flt.s x7, f19, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 352(x28)\n"
			"flw.ps f8, 224(x28)\n"
			"fnmadd.ps f19, f28, f8, f28, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 128(x14)\n"
			"flw.ps f9, 928(x14)\n"
			"fcmovm.ps f19, f18, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 544(x9)\n"
			"fcvt.pw.ps f11, f18, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f8, 32(x28)\n"
			"fnot.pi f0, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskand m6, m4, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x7b9c24d3be\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"fltm.ps m7, f0, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 672(x7)\n"
			"flw.ps f8, 96(x7)\n"
			"fmax.s f19, f27, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 96(x9)\n"
			"flw.ps f13, 896(x9)\n"
			"flw.ps f8, 96(x9)\n"
			"fmadd.ps f30, f11, f13, f8, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 32(x19)\n"
			"flw.ps f14, 448(x19)\n"
			"fand.pi f13, f28, f14\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 992(x16)\n"
			"flw.ps f25, 256(x16)\n"
			"fsll.pi f8, f0, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 160(x9)\n"
			"faddi.pi f21, f21, 0x35\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 256(x13)\n"
			"flw.ps f8, 32(x13)\n"
			"fnmsub.ps f27, f24, f8, f24, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"fbci.pi f19, 0x7f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 736(x19)\n"
			"fcmovm.ps f30, f25, f25\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 704(x16)\n"
			"flw.ps f27, 384(x16)\n"
			"fadd.ps f13, f12, f27, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 384(x7)\n"
			"flw.ps f27, 160(x7)\n"
			"flt.s x14, f24, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 320(x13)\n"
			"fcvt.wu.s x13, f8, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"maskor m4, m4, m1\n" 
			"li x5, 0x803401C0\n"
			"ld x5, 0(x5)\n"
			"li x20, 0x6c2144362dd0bdf1\n"
			"fcvt.s.wu f8, x20, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 64(x9)\n"
			"flw.ps f16, 736(x9)\n"
			"flt.ps f12, f27, f16\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 480(x14)\n"
			"flw.ps f24, 320(x14)\n"
			"fxor.pi f14, f14, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 64(x26)\n"
			"flw.ps f8, 512(x26)\n"
			"fsgnjx.ps f27, f0, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"mov.m.x m7, x9, 0x7e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_15:\n"
			"fltm.ps m4, f12, f13\n" 
			"li x5, 0x80340198\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 224(x14)\n"
			"flw.ps f30, 544(x14)\n"
			"flw.ps f24, 736(x14)\n"
			"fmsub.ps f21, f28, f30, f24, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 896(x9)\n"
			"flw.ps f24, 64(x9)\n"
			"feq.s x13, f12, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_15:\n"
			"maskor m7, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_15:\n"
			"maskor m7, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 224(x9)\n"
			"fcvt.w.s x28, f0, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 480(x14)\n"
			"feq.pi f8, f19, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 640(x9)\n"
			"flw.ps f9, 192(x9)\n"
			"fmax.ps f27, f27, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 128(x9)\n"
			"flw.ps f21, 192(x9)\n"
			"fmul.ps f28, f27, f21, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 576(x25)\n"
			"flw.ps f30, 928(x25)\n"
			"fsgnjn.ps f14, f25, f30\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 576(x9)\n"
			"fadd.pi f12, f11, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 480(x20)\n"
			"flw.ps f30, 256(x20)\n"
			"flw.ps f13, 0(x20)\n"
			"fmadd.s f21, f19, f30, f13, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 32(x7)\n"
			"flw.ps f8, 768(x7)\n"
			"fadd.ps f19, f18, f8, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 96(x25)\n"
			"flw.ps f18, 384(x25)\n"
			"feq.s x13, f16, f18\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_15:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 608(x25)\n"
			"flog.ps f14, f28\n" 
			"maskpopcz x27, m4\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 832(x26)\n"
			"fround.ps f0, f9, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 352(x16)\n"
			"flw.ps f13, 352(x16)\n"
			"fsgnjn.s f21, f16, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 32(x14)\n"
			"flw.ps f11, 64(x14)\n"
			"fltu.pi f11, f24, f11\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 736(x14)\n"
			"flw.ps f18, 960(x14)\n"
			"fmax.pi f25, f14, f18\n" 
			"la x28, rand_int_32\n"
			"flw.ps f21, 672(x28)\n"
			"fpackrepb.pi f13, f21\n" 
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
		0xbce350c7, 
		0xd492ba15, 
		0x516819cb, 
		0x66d1506b, 
		0x3b5d970e, 
		0x59ba74d8, 
		0x928dfd93, 
		0x51bbdeb5, 
		0xbd36c115, 
		0xee23244e, 
		0x6b7e3be0, 
		0xb8188d6f, 
		0xe3a7ee04, 
		0xbbc288eb, 
		0x308a3035, 
		0xad0d312d, 
		0xed4c1f2d, 
		0x9ed42087, 
		0x12c715eb, 
		0x87e2b7af, 
		0x93b1afd3, 
		0xc088b0d4, 
		0xd7862f75, 
		0x827dbfdb, 
		0xbbcbc87d, 
		0xa434e9ea, 
		0xa34c4098, 
		0xd039f149, 
		0x728989e7, 
		0x97f5545e, 
		0xc6ca3a19, 
		0x3c0d56ea, 
		0x36394229, 
		0x85468d75, 
		0x337ed570, 
		0xb66da2f2, 
		0xd14efecf, 
		0x3843eb68, 
		0xff7e2a82, 
		0xbe6cc408, 
		0x47ca0039, 
		0x2e595df2, 
		0x0ca8c451, 
		0xbc55e96c, 
		0x0a360c7d, 
		0x8c4b8443, 
		0xe03ce0c6, 
		0xe915f11b, 
		0x64b91721, 
		0x42cb69be, 
		0x5569e4d2, 
		0xdbf5db00, 
		0xff5076ad, 
		0xb58b2b33, 
		0x7564e9dc, 
		0xafb21d2d, 
		0x8039aa44, 
		0x7b6af028, 
		0xa03a1ebd, 
		0x3a0c0cab, 
		0xed85b812, 
		0x424a02c7, 
		0x7c8e409e, 
		0x910c8b12, 
		0xfb129e8b, 
		0xc35715c2, 
		0x87b808fc, 
		0x703e6d21, 
		0x15d78e73, 
		0x1d363c78, 
		0xccde00ab, 
		0x54fc0d41, 
		0x0be1d349, 
		0x0fdcefc2, 
		0x727c3e2b, 
		0x5d17e66c, 
		0xacb87c04, 
		0x17dff744, 
		0x4c43512d, 
		0xcbe1c2bc, 
		0xc3d862e9, 
		0xdd985851, 
		0x8657d1b5, 
		0xbc85c03f, 
		0xbb822024, 
		0xdfe1682b, 
		0xac41ec1c, 
		0xd6b721cf, 
		0x830cd630, 
		0xb07b5176, 
		0x119d01da, 
		0x63f8d1e5, 
		0x28c18f6f, 
		0xa149ddf4, 
		0x5dae9881, 
		0xdfa47314, 
		0x7e966176, 
		0x76bd7cd1, 
		0x77bb6846, 
		0xa5202ed2, 
		0xd6eba609, 
		0x1726295a, 
		0xab9c0c20, 
		0x9d495af5, 
		0xf23070d0, 
		0xdc9ecaf7, 
		0x884c60bf, 
		0x45df8758, 
		0x15b823e0, 
		0x05d0418e, 
		0x73d29241, 
		0xffc5cff6, 
		0x250fa6a6, 
		0x7d48925f, 
		0x683b744e, 
		0x880765a2, 
		0xe40b225d, 
		0x87e2e421, 
		0xa6046cd4, 
		0xce7cfa72, 
		0x4a4c34ff, 
		0x01c68e1f, 
		0xb6938e41, 
		0x77bff5b1, 
		0x0ee6eae3, 
		0xf53ddba9, 
		0xe029f4c1, 
		0xff610f15, 
		0x6e44c9d6, 
		0x93a156fc, 
		0x8ca61daa, 
		0x2df472d5, 
		0xe85862e4, 
		0x9823cc38, 
		0xa72c4cf5, 
		0x18e815bb, 
		0x4e9c8e80, 
		0xd86161bb, 
		0x6fff3d93, 
		0x054f7f09, 
		0xcdff9a04, 
		0xf97eb549, 
		0x03cf7602, 
		0x317bc993, 
		0x7774f12a, 
		0x5804dfc8, 
		0x4958cb0d, 
		0x14f2cfe9, 
		0xfd1f216e, 
		0xcd52e243, 
		0x52c348bf, 
		0x6d82d566, 
		0xc3276b93, 
		0x65752954, 
		0x5eefb8e4, 
		0xa840ffd8, 
		0x65e60ea7, 
		0xd6a8bc19, 
		0xfbf44780, 
		0xdbd2d2b6, 
		0xd02207d8, 
		0x80f38e17, 
		0x1073867b, 
		0x1f38b565, 
		0x0e345e10, 
		0x7d42d807, 
		0xe886ae89, 
		0x3da630f7, 
		0x4097984d, 
		0xff8752c1, 
		0xc8e974a0, 
		0xe072f5db, 
		0x564d3c9f, 
		0x2a2c4963, 
		0x4a377ea9, 
		0x3fd33024, 
		0x5a4e250e, 
		0x909305c2, 
		0x8d3bf91b, 
		0x1acefbb0, 
		0x11e22d8a, 
		0xcc543db9, 
		0x9b0651d8, 
		0xe1dff0e5, 
		0x6da6a6ef, 
		0x7103bfdd, 
		0xefe00352, 
		0x53083957, 
		0x112c4831, 
		0xaa4a7295, 
		0x4343cdb7, 
		0x7bd74b00, 
		0x0fde9965, 
		0x800575e5, 
		0x89ace202, 
		0x6dae03b3, 
		0xe2060a94, 
		0xf191cd85, 
		0x38d031c7, 
		0xe3312841, 
		0x8d4ce4b6, 
		0xf46e60cf, 
		0x2e8ceb93, 
		0xb403ad66, 
		0xfb122e51, 
		0xb0d8d892, 
		0x3e0e9720, 
		0xe738db53, 
		0xed7519ce, 
		0x79e1c727, 
		0x0f76e68d, 
		0xc6323fae, 
		0x90f5883d, 
		0xbf402130, 
		0x7292ab45, 
		0xa2cb0bdf, 
		0x843c8aef, 
		0x2338cad6, 
		0x1941f8c2, 
		0xe96cdf5d, 
		0xe0f776b0, 
		0x8884f217, 
		0x9807150f, 
		0x094ce72f, 
		0x05c32a22, 
		0xb9b6d928, 
		0xc091b03c, 
		0xa4ceba99, 
		0x5572ff74, 
		0x6c862484, 
		0xb55667b5, 
		0xe54eedee, 
		0x846de715, 
		0x232f49d2, 
		0x4c38757e, 
		0x585d34eb, 
		0x4ceccc60, 
		0xec59cd5c, 
		0xe4d3bbdb, 
		0x70727525, 
		0xb62fbd3c, 
		0xcb030ee0, 
		0x713598cb, 
		0xb8e9cecc, 
		0xa23a2ede, 
		0x631b166d, 
		0x4ce53ae7, 
		0x9377cb82, 
		0x50938581, 
		0xec39040f, 
		0x7aaca4ca, 
		0x1ec06aac, 
		0xf9562aff, 
		0x2d2a1682, 
		0x4070f752, 
		0xc4939e43  
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
		0x4c1e, 
		0x5200, 
		0xd380, 
		0xd280, 
		0xbfa7, 
		0x0000, 
		0xccc0, 
		0x5f1d, 
		0xc23a, 
		0x0000, 
		0x41f9, 
		0x4dc0, 
		0x6978, 
		0x7c00, 
		0xfc00, 
		0xfc00, 
		0xcd80, 
		0x97f0, 
		0x8000, 
		0xfc00, 
		0xcc40, 
		0xd180, 
		0x4900, 
		0x7fc3, 
		0xfc00, 
		0x20c3, 
		0x4400, 
		0x4a00, 
		0x26ba, 
		0xc880, 
		0x0000, 
		0xd3a0, 
		0xe9c7, 
		0xd060, 
		0x5340, 
		0xc500, 
		0x4d80, 
		0x5020, 
		0x03e6, 
		0x4900, 
		0xd3c0, 
		0x8000, 
		0x7c00, 
		0xff60, 
		0xd380, 
		0x51c0, 
		0x4900, 
		0xc500, 
		0xc900, 
		0x4c00, 
		0x4d40, 
		0x5040, 
		0x52c0, 
		0x09ce, 
		0xfd0f, 
		0x8000, 
		0x4cc0, 
		0x8000, 
		0xaedf, 
		0x831e, 
		0xe140, 
		0xd360, 
		0xd3a0, 
		0x4cc0, 
		0x03c2, 
		0x51c9, 
		0xc700, 
		0x4f40, 
		0x4ec0, 
		0xd080, 
		0xd2e0, 
		0xd2c0, 
		0x0000, 
		0x4700, 
		0xc200, 
		0x4e80, 
		0x53a0, 
		0x0000, 
		0xcd40, 
		0x4385, 
		0x5220, 
		0xc7c7, 
		0xe270, 
		0x2ff4, 
		0x254c, 
		0x4d80, 
		0x7c00, 
		0xad74, 
		0x5080, 
		0x5065, 
		0x7ee6, 
		0xd380, 
		0xc800, 
		0x51a0, 
		0x50c0, 
		0x5200, 
		0xd060, 
		0xd2a0, 
		0x5140, 
		0x1217, 
		0xd0c0, 
		0x0000, 
		0xe24c, 
		0x51c0, 
		0x5280, 
		0x5120, 
		0xeb0a, 
		0xcc68, 
		0xd0a0, 
		0x0000, 
		0xce80, 
		0x52e0, 
		0xce40, 
		0x20f0, 
		0xfc00, 
		0xcd40, 
		0x953f, 
		0xcb80, 
		0x8000, 
		0x4dc0, 
		0xc900, 
		0xee0e, 
		0x8000, 
		0x95d5, 
		0x4dc0, 
		0xd180, 
		0xd1e0, 
		0xcd40, 
		0xcb00, 
		0x5100, 
		0x4b80, 
		0x4900, 
		0xccc0, 
		0x5260, 
		0x4500, 
		0xcc40, 
		0x5260, 
		0xd0c0, 
		0x8000, 
		0xd280, 
		0xf05f, 
		0x4880, 
		0x4f00, 
		0x7c00, 
		0xd140, 
		0xd120, 
		0x7d25, 
		0xd0e0, 
		0x133a, 
		0xd000, 
		0xfdc9, 
		0x4fc0, 
		0x1abc, 
		0xcf40, 
		0x4500, 
		0xd1c0, 
		0x8000, 
		0x5220, 
		0x100b, 
		0x8000, 
		0xd220, 
		0x2e06, 
		0xd300, 
		0x53e0, 
		0x4500, 
		0x3eda, 
		0x0000, 
		0xd380, 
		0x9d78, 
		0xca00, 
		0x5340, 
		0xfc00, 
		0xb55e, 
		0x4400, 
		0xc000, 
		0xd200, 
		0xaebd, 
		0x4980, 
		0x4c80, 
		0xfc00, 
		0xfcff, 
		0x31cf, 
		0x8000, 
		0x2fac, 
		0x5140, 
		0xd380, 
		0x0232, 
		0x51c0, 
		0xd220, 
		0x4fc0, 
		0x4dc0, 
		0x4b00, 
		0x50e0, 
		0x6268, 
		0x1206, 
		0xd160, 
		0x3e8e, 
		0x0000, 
		0xcf40, 
		0x7c00, 
		0x51c0, 
		0x51c0, 
		0x514a, 
		0xd0c3, 
		0x5140, 
		0xd260, 
		0xc1cb, 
		0xce08, 
		0xd360, 
		0x6b88, 
		0x25c2, 
		0x9b0c, 
		0x7872, 
		0x0000, 
		0xd2a0, 
		0x5200, 
		0x0000, 
		0xcc40, 
		0x6f00, 
		0xaaa2, 
		0x8000, 
		0x45e8, 
		0x4880, 
		0x84c4, 
		0x4980, 
		0xca80, 
		0x0000, 
		0xc500, 
		0xcf79, 
		0x0000, 
		0x2c47, 
		0xcdc0, 
		0x5100, 
		0x5260, 
		0xd300, 
		0x4500, 
		0xd1c0, 
		0x4ec0, 
		0x52c0, 
		0x4400, 
		0x8000, 
		0x3c00, 
		0xd220, 
		0x4f40, 
		0xcc40, 
		0x4a80, 
		0xcd00, 
		0xb8c8, 
		0x40b0, 
		0xccc0, 
		0x5220, 
		0x4900, 
		0x7c00, 
		0x7c89, 
		0x8000, 
		0x53a0, 
		0x1250, 
		0xd060, 
		0x0000, 
		0x7a35, 
		0xd76a, 
		0x8be1, 
		0x016a, 
		0xc800, 
		0x0000, 
		0xd140, 
		0xcb00, 
		0x5120, 
		0x4b00, 
		0xacc6, 
		0x53e0, 
		0x53c0, 
		0xcb00, 
		0xf542, 
		0x4dc0, 
		0x5060, 
		0xfc00, 
		0xd260, 
		0xdcb7, 
		0xca00, 
		0xe85f, 
		0x4980, 
		0xcc00, 
		0x0049, 
		0xcc40, 
		0xfc4b, 
		0x4cc0, 
		0x7c00, 
		0xd260, 
		0xcb00, 
		0x0000, 
		0x5200, 
		0xbc00, 
		0x4f40, 
		0x4e00, 
		0x5280, 
		0x5590, 
		0x4200, 
		0x4d40, 
		0xd2e0, 
		0x52e0, 
		0x7951, 
		0xd140, 
		0x5380, 
		0xd3a0, 
		0x8000, 
		0xccc0, 
		0xf3c2, 
		0x8000, 
		0x4880, 
		0x8000, 
		0x3dc2, 
		0xcf00, 
		0x8000, 
		0x2495, 
		0xe68b, 
		0xc200, 
		0xce00, 
		0xcfc0, 
		0x4800, 
		0x50e0, 
		0xd2b0, 
		0xce80, 
		0x5749, 
		0x51e0, 
		0x2b07, 
		0xbdf8, 
		0x0000, 
		0xd140, 
		0xcc00, 
		0x53a0, 
		0x8000, 
		0x8000, 
		0xf02a, 
		0x53a0, 
		0x4f80, 
		0xcc40, 
		0x4c80, 
		0xfd5b, 
		0x7b91, 
		0x4b80, 
		0x0000, 
		0xcdc0, 
		0x4f40, 
		0x5140, 
		0xcdc0, 
		0xd040, 
		0x287a, 
		0x50c0, 
		0x5220, 
		0x4500, 
		0x9f9a, 
		0x5060, 
		0xd240, 
		0xd360, 
		0xc200, 
		0xd080, 
		0xc000, 
		0x0000, 
		0xcc80, 
		0x5040, 
		0x7cb0, 
		0x749e, 
		0xd1c0, 
		0x5360, 
		0x53a0, 
		0xc200, 
		0x52c0, 
		0xd320, 
		0x0148, 
		0x5100, 
		0x4ec0, 
		0x4880, 
		0x4400, 
		0x4ec0, 
		0x4800, 
		0x3889, 
		0xfcb9, 
		0x5160, 
		0x4c80, 
		0xcc80, 
		0x8000, 
		0xc600, 
		0x8000, 
		0x8642, 
		0xc670, 
		0xc700, 
		0x4e4a, 
		0x4e40, 
		0xd020, 
		0x5020, 
		0xcb80, 
		0xcf80, 
		0x5140, 
		0xb37c, 
		0x5320, 
		0x48da, 
		0x277d, 
		0x4ca9, 
		0x7c95, 
		0xf965, 
		0xfc00, 
		0xc980, 
		0x44c4, 
		0x4a00, 
		0x7c00, 
		0x4000, 
		0xd0c0, 
		0x01e5, 
		0x5205, 
		0x4cc0, 
		0xd080, 
		0xc500, 
		0xc3e0, 
		0x5000, 
		0xd2e0, 
		0x5180, 
		0xc500, 
		0xd3c0, 
		0xce40, 
		0x39be, 
		0xc600, 
		0x7c00, 
		0x5260, 
		0xfc00, 
		0x4d59, 
		0x751e, 
		0xd260, 
		0x4800, 
		0xd0c0, 
		0x4500, 
		0x7c58, 
		0x4fc0, 
		0x4200, 
		0xcd40, 
		0x4800, 
		0x46b0, 
		0x0370, 
		0x4fab, 
		0x50a0, 
		0x7a4f, 
		0x5320, 
		0x5120, 
		0x8000, 
		0xc880, 
		0x0541, 
		0x1908, 
		0x3ac0, 
		0x4e40, 
		0xdcdd, 
		0x38c5, 
		0x5240, 
		0x6053, 
		0x5140, 
		0xc880, 
		0x8000, 
		0xc200, 
		0x4d00, 
		0xf8ed, 
		0xc600, 
		0xce80, 
		0x5100, 
		0x3b73, 
		0x3056, 
		0x856a, 
		0x4e97, 
		0xcd80, 
		0xcfc0, 
		0x0b57, 
		0x5200, 
		0x7482, 
		0xd180, 
		0xcd7a, 
		0x4fc0, 
		0x4d80, 
		0x5180, 
		0x5380, 
		0x4880, 
		0xe91a, 
		0x5100, 
		0x4cc0, 
		0x902a, 
		0x4b80, 
		0x5e3f, 
		0x8000, 
		0xfb11, 
		0x4a00, 
		0x52e0, 
		0x8000, 
		0xc980, 
		0x164f, 
		0xcbe1, 
		0x0264, 
		0xcf80, 
		0x8000, 
		0xce00, 
		0xc980, 
		0x7c00, 
		0xcb80, 
		0x021e, 
		0x1d1c, 
		0x8000, 
		0xd080, 
		0x7c38, 
		0x5360, 
		0x5040, 
		0x5180, 
		0x0727, 
		0xa2da, 
		0xcdc0, 
		0x7c00, 
		0x4400, 
		0xce80, 
		0xf289, 
		0xfc00, 
		0xcd5f, 
		0x4d00, 
		0x4fc0, 
		0x2ea3, 
		0xcf40, 
		0x2050, 
		0x4d00, 
		0xfede, 
		0xd360, 
		0x1fd1, 
		0x066a, 
		0x6f22, 
		0x8000, 
		0x6fe8, 
		0x7c85, 
		0x5340, 
		0xd280, 
		0x4031, 
		0x52a0, 
		0xd200, 
		0x53a0, 
		0xc700, 
		0x3373, 
		0xd3a0, 
		0xd000, 
		0xfc00, 
		0xcf00, 
		0x4600, 
		0xc500, 
		0x4e40, 
		0x9b41, 
		0x0000, 
		0xd120, 
		0xd220, 
		0xc400, 
		0xb97b, 
		0x8000, 
		0xd080, 
		0xbc00, 
		0x1f86, 
		0xcf00, 
		0x7c00, 
		0x53e0, 
		0xde14, 
		0x371b, 
		0x896b, 
		0x4b80, 
		0x3747, 
		0xcb24, 
		0x51c0, 
		0xaff6, 
		0x4a80, 
		0xe252, 
		0xcf24, 
		0x53e0, 
		0x5000, 
		0xc200, 
		0xcf80, 
		0x4e80, 
		0x4500, 
		0x4500, 
		0xd360, 
		0x8000, 
		0xd300, 
		0x4e80, 
		0x8000, 
		0xe208, 
		0xc400, 
		0x5100, 
		0x0000, 
		0x5280, 
		0x22ac, 
		0x4d80, 
		0x4d00, 
		0xc880, 
		0x5060, 
		0xd655, 
		0x8000, 
		0xd280, 
		0xd160, 
		0x4e00, 
		0xca00, 
		0xfdc2, 
		0xc200, 
		0xd020, 
		0x4c40, 
		0x52a0, 
		0xd3a0, 
		0x53c0, 
		0x5200, 
		0xd240, 
		0xd140, 
		0x79a8, 
		0x85c6, 
		0x8000, 
		0x53e0, 
		0xc500, 
		0x4880, 
		0x4800, 
		0x5240, 
		0x52c0, 
		0x4ec0, 
		0xc600, 
		0x0000, 
		0xd3a0, 
		0xccc0, 
		0x5060, 
		0x7c00, 
		0xcf3a, 
		0x5000, 
		0x5280, 
		0xa42d, 
		0xd2e0, 
		0x6713, 
		0x3c8d, 
		0xc200, 
		0xca80, 
		0xd260, 
		0x4b80, 
		0x5100, 
		0xcf40, 
		0x7c00, 
		0xcd40, 
		0xc500, 
		0x11f1, 
		0x4f80, 
		0x7c00, 
		0x4700, 
		0x3c00, 
		0xc500, 
		0x50a0, 
		0x4200, 
		0xd1e0, 
		0x52a0, 
		0xe766, 
		0x4200, 
		0x5380, 
		0x5140, 
		0x53a0, 
		0x5060, 
		0x2d76, 
		0xc500, 
		0x4dc0, 
		0x8f03, 
		0x998d, 
		0x105d, 
		0x4e40, 
		0x515a, 
		0x8140, 
		0x0fb1, 
		0x4d80, 
		0x5160, 
		0x5320, 
		0x52a0, 
		0x7b64, 
		0xd2a0, 
		0xd2c0, 
		0xd0a0, 
		0x53a0, 
		0x0000, 
		0xef02, 
		0x0a9d, 
		0x5180, 
		0xc000, 
		0x4a00, 
		0xd0a0, 
		0xbefa, 
		0xd180, 
		0xd49e, 
		0x4b00, 
		0xccc0, 
		0x72fe, 
		0xc000, 
		0x4c40, 
		0x0011, 
		0x5140, 
		0xcf40, 
		0xbe61, 
		0xa2b0, 
		0x51c0, 
		0x4200, 
		0xcd00, 
		0x4a00, 
		0x5260, 
		0xcfc0, 
		0xc1d9, 
		0x4e40, 
		0x0000, 
		0x4e80, 
		0x19c3, 
		0xc83e, 
		0x53e0, 
		0xe412, 
		0x5260, 
		0xc48f, 
		0xd340, 
		0xce40, 
		0x8000, 
		0xce80, 
		0x4800, 
		0xd020, 
		0x5060, 
		0xc500, 
		0x52c0, 
		0xea49, 
		0x982f, 
		0x74a2, 
		0x4b80, 
		0x4a80, 
		0x0000, 
		0xe5ad, 
		0xd3c0, 
		0x4cc0, 
		0x4c40, 
		0x51a0, 
		0xb1cb, 
		0x4700, 
		0xd2ba, 
		0x4c00, 
		0x4c80, 
		0xce00, 
		0x43b7, 
		0x0000, 
		0x4e00, 
		0xeaa3, 
		0x3c00, 
		0xcfc0, 
		0xb40f, 
		0x8add, 
		0x4c40, 
		0xce80, 
		0x5260, 
		0xd120, 
		0x5360, 
		0x4400, 
		0x5100, 
		0x439f, 
		0xd1a0, 
		0x5060, 
		0x4dc0, 
		0xd360, 
		0xfa19, 
		0xcd80, 
		0xbc00, 
		0xf7e7, 
		0x277f, 
		0x52c0, 
		0x5e4e, 
		0x0000, 
		0xc700, 
		0xd320, 
		0x7c00, 
		0x18f8, 
		0x3c00, 
		0x52e0, 
		0xc980, 
		0x4fc0, 
		0x5120, 
		0x21f3, 
		0x8000, 
		0xcec0, 
		0x8000, 
		0x51c0, 
		0x9583, 
		0x8b77, 
		0xcd00, 
		0x50a0, 
		0xd3a0, 
		0x4800, 
		0xc800, 
		0x8000, 
		0x4e00, 
		0x4b80, 
		0x4500, 
		0x9b98, 
		0x7be5, 
		0x4b00, 
		0x4e00, 
		0x5320, 
		0x51c0, 
		0xc600, 
		0x5220, 
		0xb54b, 
		0x53a0, 
		0xe137, 
		0xd2a0, 
		0xce00, 
		0x4c40, 
		0xc880, 
		0xc980, 
		0x5280, 
		0xf200, 
		0x4cc0, 
		0xf0d1, 
		0x4400, 
		0x5dfb, 
		0x4493, 
		0x4f80, 
		0xca08, 
		0x4f80, 
		0x5040, 
		0xaabe, 
		0x48b5, 
		0x4980, 
		0x4400, 
		0xd1a0, 
		0x1cb0, 
		0xd1a0, 
		0x4e00, 
		0xd040, 
		0x3d1c, 
		0x4900, 
		0x50a0, 
		0xc800, 
		0x387d, 
		0x6009, 
		0x4fc0, 
		0xcb00, 
		0xa616, 
		0xd1c0, 
		0x5000, 
		0xae24, 
		0xd340, 
		0xd200, 
		0xc200, 
		0x3279, 
		0x4fdf, 
		0xd280, 
		0x4c76, 
		0x0ee2, 
		0x5140, 
		0xa1c5, 
		0x7837, 
		0x4880, 
		0xe8da, 
		0xc980, 
		0x02e0, 
		0x22cf, 
		0xfd5a, 
		0xcab0, 
		0xdc05, 
		0xd340, 
		0x2ded, 
		0xcc19, 
		0x52e0, 
		0x5120, 
		0xd2a0, 
		0x7c00, 
		0xd280, 
		0xd3e0, 
		0xf92b, 
		0x5260, 
		0xd1e0, 
		0x0bc4, 
		0x0000, 
		0x4880, 
		0xce00, 
		0xcb80, 
		0xd2a0, 
		0x4b00, 
		0xc000, 
		0xd392, 
		0x5040, 
		0xfd4c, 
		0x4500, 
		0x4f00, 
		0xd060, 
		0x4ea3, 
		0x5240, 
		0x8000, 
		0xb37e, 
		0x4e00, 
		0x4400, 
		0x0000, 
		0xe851, 
		0x5160, 
		0xc200, 
		0xdc19, 
		0x53c0, 
		0x4f40, 
		0x5000, 
		0xd220, 
		0xc400, 
		0xdb4a, 
		0x5220, 
		0x81c6, 
		0x8000, 
		0xcd80, 
		0x8000, 
		0x5120, 
		0xd0ab, 
		0x1bb3, 
		0x3cf2, 
		0xd020, 
		0xd020, 
		0xcfed, 
		0x4d00, 
		0x4e00, 
		0xcb80, 
		0xfcca, 
		0x0000, 
		0x4d80, 
		0x52c0, 
		0x2c6e, 
		0x4200, 
		0x2997, 
		0xfc00, 
		0x65b3, 
		0xb010, 
		0xbbfb, 
		0xd0c0, 
		0x0000, 
		0x5340, 
		0x14dc, 
		0x0000, 
		0xca03, 
		0x5160, 
		0xd320, 
		0x4200, 
		0x6cee, 
		0xd7a4, 
		0xb13d, 
		0xd160, 
		0xd0e0, 
		0x1c2b, 
		0x5360, 
		0x958f, 
		0x5020, 
		0x7da0, 
		0xcd40, 
		0x5b1f, 
		0xd060, 
		0x5180, 
		0x7ee9, 
		0x1da5, 
		0x4101, 
		0x7712, 
		0xbd09, 
		0x51c0, 
		0x4200, 
		0x5220, 
		0x4ec0, 
		0xe962, 
		0x4600, 
		0x8000, 
		0x52e0, 
		0xfbaa, 
		0xcc80, 
		0x8000, 
		0x4000, 
		0xd3a0, 
		0x7dfc, 
		0x8a28, 
		0x0000, 
		0xd340, 
		0x53a0, 
		0xd120, 
		0x5f3a, 
		0x7dae, 
		0x4b7f, 
		0x53c0, 
		0xd280, 
		0x4b80, 
		0xcc40, 
		0xd300, 
		0xcfc0, 
		0x5200, 
		0xcd80, 
		0x9e23, 
		0xd200, 
		0xcf70, 
		0xd300, 
		0xd040, 
		0xd89c, 
		0x6d2f, 
		0x4980, 
		0x8014, 
		0x896c, 
		0x4fc0, 
		0x4d40, 
		0x2555, 
		0xd240, 
		0x7c00, 
		0x51c0, 
		0xc169, 
		0xd180, 
		0x7df5, 
		0x4c40, 
		0x4980, 
		0xd080, 
		0x4d80, 
		0x5240, 
		0x8000, 
		0x52b2, 
		0xd040, 
		0xce40, 
		0x4a00, 
		0xd100, 
		0xd280, 
		0x52c0, 
		0x50e0, 
		0x51c0, 
		0x022d, 
		0x4e40, 
		0x8000, 
		0xd120, 
		0x4900, 
		0x5320, 
		0xd320, 
		0x4a80, 
		0x0000, 
		0x645e, 
		0xce40, 
		0xd6ee, 
		0x4b00, 
		0x5120, 
		0x4d80, 
		0x1516, 
		0x4f80, 
		0x7dd1, 
		0xf68a, 
		0x4f00, 
		0x5060, 
		0xf017, 
		0x52a0, 
		0x4f76, 
		0x50a0, 
		0x50c0, 
		0xd2c0, 
		0xce3b, 
		0xd080, 
		0xcd40, 
		0x0034, 
		0xd200, 
		0x2883, 
		0xddf2, 
		0x00d2, 
		0xcf00, 
		0xd2a0, 
		0xd3e0, 
		0xfca8, 
		0x8000, 
		0x4f00, 
		0x53a0, 
		0xb80a, 
		0x5020, 
		0x4700, 
		0xd260, 
		0xc400, 
		0xca80, 
		0x5003, 
		0xd160, 
		0x1e29, 
		0x7c00, 
		0x4800, 
		0xcd40, 
		0x53c0, 
		0x7c00, 
		0xbdba, 
		0xce37, 
		0xcec0, 
		0x5380, 
		0x8000, 
		0x0000, 
		0x5100, 
		0xc800, 
		0xd200, 
		0xd320, 
		0xd8de, 
		0xd040, 
		0xcc80, 
		0x4b80, 
		0x0c94, 
		0x50c0, 
		0x8000, 
		0xfdab, 
		0x7c00, 
		0xccc0, 
		0xe8f5, 
		0x0000, 
		0xfc00, 
		0x5340, 
		0x7ffa, 
		0xd020, 
		0x51e0, 
		0xcb80, 
		0x5100, 
		0xd320, 
		0x7c07, 
		0xba4d, 
		0x5100, 
		0xd0e0, 
		0x7c00, 
		0x5000, 
		0xfc00, 
		0xfc00, 
		0xae12, 
		0xfc00, 
		0x5160, 
		0xcd40, 
		0x9520, 
		0x5220, 
		0x4b80, 
		0xdda2, 
		0x4d00, 
		0x4f80, 
		0xbde7, 
		0x51a0, 
		0x4a80, 
		0xd3a0, 
		0x52a0, 
		0x76ff, 
		0x8e72, 
		0xd3e0, 
		0xd300, 
		0x52a0, 
		0x4f40, 
		0x80bd, 
		0xd280, 
		0xcf00, 
		0xd060, 
		0x6382, 
		0xd240, 
		0x1858, 
		0x5f13, 
		0x4f80, 
		0xd320, 
		0xd260, 
		0x1c04, 
		0x00a1, 
		0xcf80, 
		0xcd80, 
		0xcc80, 
		0x50a0, 
		0xcec0, 
		0x4980, 
		0xd1e0, 
		0x52e0, 
		0xbc00, 
		0xcc80, 
		0xd260, 
		0xd060, 
		0x52a0, 
		0x0279, 
		0xd1e0, 
		0xc800, 
		0xa247, 
		0x0000, 
		0x4e00, 
		0xd3a0, 
		0xc980, 
		0xc8fc, 
		0x82e7, 
		0xd240, 
		0xd1c0, 
		0xcc80, 
		0xd3a0, 
		0x53e0, 
		0x8000, 
		0x5260, 
		0xd2c0, 
		0x3037, 
		0x7c00, 
		0x0000, 
		0x929d, 
		0x7c46, 
		0x7cd4, 
		0xaa49, 
		0x3e07, 
		0x0c9d, 
		0x75b2, 
		0x99c2, 
		0x4dc6, 
		0x3026, 
		0xed87, 
		0x698a, 
		0x6a2c, 
		0xc800, 
		0xcd18, 
		0xd3e0, 
		0x3615, 
		0x51e0, 
		0xd0e0, 
		0x4dc0, 
		0x4880, 
		0x4b80, 
		0xd180, 
		0x9cfd, 
		0xe083, 
		0x1345, 
		0x4600, 
		0x4b00, 
		0xd280, 
		0xd0e0, 
		0x5100, 
		0xb997, 
		0x5297, 
		0x2e3c, 
		0x9867, 
		0x51e0, 
		0xaf32, 
		0xbc8c, 
		0xd240, 
		0x0000, 
		0xb26a, 
		0x155b, 
		0x6177, 
		0x5280, 
		0x50c0, 
		0xebb7, 
		0x4600, 
		0xd340, 
		0xc387, 
		0xcf40, 
		0x5180, 
		0x8000, 
		0xcd00, 
		0x32df, 
		0x4f80, 
		0x50e0, 
		0x7df6, 
		0xd320, 
		0xd340, 
		0xccc0, 
		0xfc00, 
		0x5280, 
		0x7562, 
		0xd236, 
		0x8000, 
		0x4b28, 
		0xd2e0, 
		0x7d45, 
		0x5020, 
		0xfc00, 
		0x8000, 
		0x5040, 
		0x7c00, 
		0xca80, 
		0x4d00, 
		0xc980, 
		0xd300, 
		0xc800, 
		0xcdc0, 
		0xd000, 
		0x4b00, 
		0x7615, 
		0x1099, 
		0x4980, 
		0xcd80, 
		0x4ee3, 
		0x8000, 
		0x7c00, 
		0xd140, 
		0xcd00, 
		0xd060, 
		0xd120, 
		0x7c76, 
		0x4980, 
		0xc200, 
		0x53e0, 
		0x4ec0, 
		0x5020, 
		0x3485, 
		0x6682, 
		0x4f40, 
		0x5060, 
		0x4980, 
		0x4dc0, 
		0x0000, 
		0x4500, 
		0xd1a0, 
		0x4ec0, 
		0x8399, 
		0x1809, 
		0xc5f8, 
		0x4f00, 
		0x0000, 
		0x50a0, 
		0x97ba, 
		0xce80, 
		0x5120, 
		0xcd80, 
		0xd240, 
		0xf4f7, 
		0x0d8d, 
		0x45c1, 
		0xd120, 
		0x4f00, 
		0x4f40, 
		0xca80, 
		0xd340, 
		0xfc00, 
		0xd3c0, 
		0x98c9, 
		0x7c00, 
		0xcd40, 
		0x17d1, 
		0x51a0, 
		0xe5d2, 
		0xf93e, 
		0x8000, 
		0x8e4a, 
		0x2873, 
		0x90f5, 
		0x4c40, 
		0xc000, 
		0x9091, 
		0x7c00, 
		0xb8e8, 
		0xce80, 
		0x5280, 
		0xc600, 
		0xf026, 
		0x8000, 
		0x5320, 
		0x50a0, 
		0xd2a0, 
		0x50c0, 
		0xd3c0, 
		0x5200, 
		0xdb3b, 
		0xc600, 
		0xd220, 
		0xd040, 
		0x4200, 
		0xfedb, 
		0x5020, 
		0xd226, 
		0x5240, 
		0xd380, 
		0xc000, 
		0xcd00, 
		0x53a0, 
		0x0000, 
		0xa4ca, 
		0x21a4, 
		0x4a00, 
		0x8f98, 
		0xff63, 
		0x9ee4, 
		0xfc00, 
		0xcfc0, 
		0xc702, 
		0xcdc0, 
		0xd3a0, 
		0x5180, 
		0xcc00, 
		0x4f00, 
		0xebcf, 
		0x5220, 
		0x52a0, 
		0x5100, 
		0x51e0, 
		0x07a7, 
		0xd3e0, 
		0x0b0d, 
		0x5220, 
		0x0000, 
		0x576c, 
		0x50e0, 
		0xbc00, 
		0x4b80, 
		0x52c0, 
		0xcdc0, 
		0x8000, 
		0xd040, 
		0x8000, 
		0x5380, 
		0xc400, 
		0xea62, 
		0xcdc0, 
		0xce80, 
		0xc200, 
		0xcf40, 
		0xcb80, 
		0x5340, 
		0xd2c0, 
		0xd0c0, 
		0x4b00, 
		0x1643, 
		0x4d80, 
		0xce40, 
		0x4500, 
		0x51e0, 
		0xc900, 
		0x22e0, 
		0x6c49, 
		0x5160, 
		0xd140, 
		0x5240, 
		0x7949, 
		0xd360, 
		0x3c00, 
		0x1c26, 
		0x7c00, 
		0x710d, 
		0x8000, 
		0x52c0, 
		0xd180, 
		0x4f80, 
		0x4900, 
		0x4400, 
		0xd120, 
		0x0000, 
		0xd300, 
		0xd0c0, 
		0x4b80, 
		0x3007, 
		0xd3c0, 
		0xbc78, 
		0x9f5c, 
		0x4700, 
		0x52c0, 
		0xda64, 
		0x4a00, 
		0x4a00, 
		0xca00, 
		0x1612, 
		0x52c0, 
		0x5080, 
		0xd0b4, 
		0xd518, 
		0xce00, 
		0xd7bd, 
		0x6322, 
		0x8000, 
		0xd280, 
		0x7c00, 
		0x53c0, 
		0x2760, 
		0x5100, 
		0x4900, 
		0xcd00, 
		0x4980, 
		0x0000, 
		0xce00, 
		0xcd40, 
		0xd320, 
		0xccc0, 
		0x5060, 
		0xabd9, 
		0xc800, 
		0x238a, 
		0x4c40, 
		0xd1a0, 
		0x5280, 
		0x4900, 
		0xd300, 
		0xc0e8, 
		0x4600, 
		0xd0a0, 
		0x5220, 
		0xe36c, 
		0x001f, 
		0xdfa5, 
		0x5040, 
		0xc200, 
		0x0000, 
		0x4980, 
		0xd220, 
		0x4d00, 
		0x8f40, 
		0x4e80, 
		0xd0e0, 
		0xe460, 
		0xcd00, 
		0x73f9, 
		0xc600, 
		0x4ddd, 
		0x5000, 
		0x5140, 
		0xdba3, 
		0x4c40, 
		0x51a0, 
		0xce40, 
		0x8c75, 
		0x7a09, 
		0xfc00, 
		0xc980, 
		0x7c88, 
		0x50e0, 
		0x7c00, 
		0x53c0, 
		0x46ed, 
		0xcc80, 
		0xc880, 
		0xd060, 
		0x4cc0, 
		0x5080, 
		0xcb80, 
		0xd0c0, 
		0xca80, 
		0x1371, 
		0xe247, 
		0x8a17, 
		0xc200, 
		0x0000, 
		0x8000, 
		0x4e00, 
		0x357b, 
		0x5340, 
		0x4700, 
		0x4600, 
		0xd0a0, 
		0x4dc0, 
		0xcb00, 
		0xc880, 
		0x9177, 
		0x41ff, 
		0xecc5, 
		0x52c0, 
		0xd3c0, 
		0x4f00, 
		0x4fc0, 
		0x0000, 
		0xac53, 
		0x5100, 
		0x92f8, 
		0x1c6c, 
		0x4600, 
		0x829c  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xd7e88926, 
		0x1ca3d3fe, 
		0x0f41d1d4, 
		0x7fbacb10, 
		0x41900000, 
		0x0598bdd6, 
		0x6e951464, 
		0x9b1ba0ab, 
		0x595e1dad, 
		0x9cfcf0ad, 
		0x7f800000, 
		0x7f800000, 
		0xffc00000, 
		0x6fafb5a0, 
		0x41800000, 
		0x42400000, 
		0x802ae78e, 
		0x00295fd1, 
		0xcccccccc, 
		0xffe76169, 
		0xc1100000, 
		0x3e8775f1, 
		0x80000000, 
		0xc2700000, 
		0x0e0fffff, 
		0x800ab943, 
		0x7fc00001, 
		0x7f800000, 
		0x50e3868e, 
		0x3fac7a5f, 
		0x80000000, 
		0x5cd8277b, 
		0xff800000, 
		0xf76d8d95, 
		0x0e007fff, 
		0xc0800000, 
		0xc1b80000, 
		0x0f7fffff, 
		0x3fa0e325, 
		0x8079c9af, 
		0xf380b198, 
		0xe0e2892f, 
		0xc1c80000, 
		0x00800001, 
		0x42600000, 
		0xd23a5ff4, 
		0x0c7fe000, 
		0xc1b80000, 
		0x0c7ffe00, 
		0xbf028f5c, 
		0x80000000, 
		0x80004000, 
		0xff800000, 
		0x7ff7ba04, 
		0xc2100000, 
		0x00000000, 
		0xc25c0000, 
		0x00000001, 
		0x697663f8, 
		0x41880000, 
		0xc2640000, 
		0x1b314115, 
		0x6777c5d2, 
		0x7fffffff, 
		0x8e93cf21, 
		0xa27bdc5b, 
		0x33333333, 
		0x80000000, 
		0x0c7fc000, 
		0x41e00000, 
		0xc1b80000, 
		0x85e71b3d, 
		0x41e80000, 
		0x8036a2b7, 
		0x0c7ffc00, 
		0x39021174, 
		0xc1d80000, 
		0xf6036d2b, 
		0x0c7ffff8, 
		0x42400000, 
		0xff800000, 
		0xc26c0000, 
		0x435cb356, 
		0xc2680000, 
		0x42440000, 
		0x42600000, 
		0x4daf2ea4, 
		0x40a00000, 
		0xc1000000, 
		0x42180000, 
		0x00000004, 
		0x80000000, 
		0xa5a7abaf, 
		0xff7fffff, 
		0x80000000, 
		0x80000000, 
		0x98463394, 
		0x80000002, 
		0xc1c80000, 
		0x00400000, 
		0x42480000, 
		0x00008000, 
		0x00800001, 
		0x7f800001, 
		0x0c7fffe0, 
		0xf204f32a, 
		0x3cf6be12, 
		0x00800000, 
		0x0e1fffff, 
		0xc23c0000, 
		0x0e000007, 
		0x807fffff, 
		0x80008000, 
		0xc2240000, 
		0x0784971d, 
		0x7a54785b, 
		0x41800000, 
		0xc2000000, 
		0xc2000000, 
		0x7f7fffff, 
		0x0d5fbd63, 
		0x80000002, 
		0xf3ba640e, 
		0x0e00ffff, 
		0x00000000, 
		0xc0e00000, 
		0x42400000, 
		0x9350c0ef, 
		0x00100000, 
		0x4b000000, 
		0x00000002, 
		0x3f028f5c, 
		0x8f7ffffe, 
		0xffc00001, 
		0x8074b21d, 
		0x00800002, 
		0x89d5bec2, 
		0x80800003, 
		0x41100000, 
		0x00000020, 
		0x80020000, 
		0x0e3fffff, 
		0xe3f61b4d, 
		0x007fffff, 
		0x9e9a20c3, 
		0x86f2104c, 
		0xc2680000, 
		0xc2440000, 
		0x41f00000, 
		0x001a47e5, 
		0x3b1fcfee, 
		0xc1800000, 
		0xc1100000, 
		0xff800000, 
		0xc0a00000, 
		0x53f3b1b3, 
		0xc1a80000, 
		0xc2100000, 
		0x42040000, 
		0x80800001, 
		0xc1c80000, 
		0xc1300000, 
		0x42180000, 
		0x00000080, 
		0x00800000, 
		0x7fc00001, 
		0x647d55a7, 
		0xf6fa4650, 
		0xff7fffff, 
		0x00008000, 
		0x30dfd328, 
		0x42180000, 
		0x41200000, 
		0x193af08d, 
		0x42280000, 
		0x27a29c34, 
		0x1df4b55c, 
		0xc2700000, 
		0xc0e00000, 
		0xe88b2888, 
		0xb73007b2, 
		0x41b00000, 
		0x421c0000, 
		0x424c0000, 
		0xca8ce519, 
		0x42640000, 
		0xff800000, 
		0xc1400000, 
		0xf4351fd8, 
		0x0c7ffffe, 
		0x7f800000, 
		0x41f00000, 
		0xc0800000, 
		0xc1300000, 
		0xc2180000, 
		0x80000000, 
		0x0f7ffffe, 
		0xc2440000, 
		0xffffffff, 
		0x41400000, 
		0x0e01ffff, 
		0x8f7ffffc, 
		0x33930a96, 
		0x42540000, 
		0x9a15eca7, 
		0x80800002, 
		0x7f000000, 
		0x427c0000, 
		0x6f8a878a, 
		0x00000000, 
		0xcccccccc, 
		0x0c7fff80, 
		0x42580000, 
		0x00008000, 
		0xc2580000, 
		0x6a7ef630, 
		0x6a80a0ac, 
		0x7fc00001, 
		0x00000000, 
		0xff7fffff, 
		0x0c780000, 
		0x4a21fc23, 
		0x18e8b1b8, 
		0x93dedda6, 
		0x80000000, 
		0xc2200000, 
		0x0115122c, 
		0x00000000, 
		0xf2a6d8c3, 
		0x0e007fff, 
		0x0012914b, 
		0xc1a80000, 
		0x0c780000, 
		0x40c00000, 
		0x0030c240, 
		0x42340000, 
		0x00008000, 
		0xb7a79efd, 
		0xc0e00000, 
		0x8f7ffffc, 
		0x7018a146, 
		0xc2500000, 
		0xbb51dbf4, 
		0x00400000, 
		0x80000000, 
		0x40400000, 
		0x4e5445bb, 
		0x42480000, 
		0xc1800000, 
		0xbb410553, 
		0x0e00000f, 
		0xa63a5b58, 
		0x80002000, 
		0x1be038c8, 
		0xe1bc2ded, 
		0x7f947875, 
		0xf3626049, 
		0x053f1e2f, 
		0xd95c1ed8, 
		0x80000000, 
		0x10e72309, 
		0x0f7ffffd, 
		0x41a80000, 
		0x0c7ffff8, 
		0x00000000, 
		0x0e00001f, 
		0xff88ca87, 
		0x00200000, 
		0xff8c2e16, 
		0x624ff5a8, 
		0xcccccccc, 
		0xc1200000, 
		0xc2700000, 
		0x70a9315d, 
		0x40000000, 
		0x807fffff, 
		0xff800000, 
		0x42340000, 
		0x0c7fc000, 
		0x7f7fffff, 
		0x00800003, 
		0x1124bcce, 
		0x80000000, 
		0x58c6511e, 
		0xc1a00000, 
		0xcccccccc, 
		0x00400000, 
		0x7fc00001, 
		0x00000200, 
		0x5d662b78, 
		0x80000800, 
		0x80000020, 
		0x80ffffff, 
		0x0e0000ff, 
		0xc2240000, 
		0x847ba192, 
		0x80000000, 
		0xc2780000, 
		0x80000001, 
		0x41800000, 
		0x804e14a1, 
		0xc1000000, 
		0xfae85811, 
		0x787b6720, 
		0x7f800000, 
		0x7fc00000, 
		0x1e9faecc, 
		0x00000000, 
		0x42780000, 
		0x426c0000, 
		0x00080000, 
		0x4cee40d3, 
		0x00200000, 
		0x0e000007, 
		0x69dca3aa, 
		0x00004000, 
		0x42180000, 
		0xc2700000, 
		0x805a0d50, 
		0xac701a9f, 
		0x00000000, 
		0xc9d5d99b, 
		0x33333333, 
		0x80000001, 
		0x0c7ffc00, 
		0x6ae30882, 
		0x80ffffff, 
		0x42500000, 
		0x0eeb5b2f, 
		0x5518e833, 
		0x665a2779, 
		0x00000000, 
		0x00000002, 
		0x60dc7a76, 
		0x0e00003f, 
		0x00010000, 
		0xc2040000, 
		0x7fffffff, 
		0x7f000000, 
		0x007ffffe, 
		0xf8db9fd7, 
		0x80000001, 
		0x41800000, 
		0x80000000, 
		0x00800003, 
		0xa5afae85, 
		0xc2100000, 
		0x00008000, 
		0x7f800000, 
		0xc0a00000, 
		0x0e0007ff, 
		0x42500000, 
		0xc2400000, 
		0x80000020, 
		0x00000002, 
		0x80800003, 
		0xaa3125c2, 
		0x42340000, 
		0x807ffffe, 
		0x80800001, 
		0xff800000, 
		0xc1a80000, 
		0xc1880000, 
		0x80ffffff, 
		0x40800000, 
		0x007ffffe, 
		0x7f7ffffe, 
		0x7fc3fb97, 
		0x165f27b4, 
		0xff800000, 
		0x00000000, 
		0x40a00000, 
		0x00800000, 
		0x0e07ffff, 
		0x651d8ec5, 
		0x24743805, 
		0x80000100, 
		0xfdb93d00, 
		0xc2400000, 
		0xc1f80000, 
		0xe0fd616c, 
		0x80000000, 
		0x41c00000, 
		0x80100000, 
		0x41880000, 
		0x0e0001ff, 
		0xc20c0000, 
		0x40e00000, 
		0xc26c0000, 
		0x00000000, 
		0x8041d78a, 
		0x221155f2, 
		0x00000008, 
		0x7d332ee4, 
		0x00000001, 
		0x4a556f82, 
		0x80000000, 
		0x00000008, 
		0xc26c0000, 
		0x7fadca46, 
		0x80008000, 
		0xbb679252, 
		0x00000000, 
		0x00800001, 
		0x7f800000, 
		0xc1c80000, 
		0x00000100, 
		0x0d00fff0, 
		0xc2140000, 
		0x86b1fea7, 
		0x624bff34, 
		0x5576be67, 
		0xe8dd97b6, 
		0x0c7ffe00, 
		0x247a3c7e, 
		0x3f800000, 
		0x7f800000, 
		0xcd4fe75b, 
		0xc2700000, 
		0x80000000, 
		0x41f80000, 
		0x0c7fff00, 
		0x0c7ffff0, 
		0x42480000, 
		0x00800000, 
		0xff800000, 
		0xcccccccc, 
		0x0e000003, 
		0x00000010, 
		0xc1e00000, 
		0x42740000, 
		0x24456a22, 
		0xf3fd46e1, 
		0x81e44f61, 
		0xffbfffff, 
		0x7fc00000, 
		0x41f80000, 
		0x80800001, 
		0x7f800000, 
		0x00800002, 
		0x41600000, 
		0x00000000, 
		0x00800001, 
		0x80000020, 
		0xf5ce2afb, 
		0xf2e34dd5, 
		0x0c7ffffe, 
		0xdbc1eb3b, 
		0xf172632c, 
		0x63b495bb, 
		0x42300000, 
		0x80a331b2, 
		0x4e213de0, 
		0x80000000, 
		0xc01bff34, 
		0x0c7ffffe, 
		0x80000000, 
		0x00000100, 
		0xc1c80000, 
		0x3f028f5c, 
		0x80000000, 
		0xc2100000, 
		0xff7fffff, 
		0x4b8c4b40, 
		0xff7fffff, 
		0x422c0000, 
		0xffc00001, 
		0x3014d265, 
		0x80000001, 
		0x0c7fffc0, 
		0x6c9efc1a, 
		0xa7568e97, 
		0x41200000, 
		0xc1400000, 
		0x7fc00000, 
		0x80800001, 
		0x0c7fffe0, 
		0x00000400, 
		0x80000002, 
		0xff91a025, 
		0xc2300000, 
		0x80000000, 
		0xc2140000, 
		0xc1700000, 
		0x8f7ffffd, 
		0xffc00000, 
		0xff800000, 
		0xff800000, 
		0x69054d09, 
		0xc1c00000, 
		0x41400000, 
		0xa250f263, 
		0x64647d9f, 
		0x807fffff, 
		0xcfba4721, 
		0xd7fa4ef5, 
		0x42480000, 
		0x48c3b342, 
		0x00800001, 
		0x00000010, 
		0x00000000, 
		0x80004000, 
		0x42140000, 
		0x00800003, 
		0x507be5e7, 
		0x724f0971, 
		0xc2500000, 
		0x00000001, 
		0xc2380000, 
		0xea72ab3f, 
		0x7f800000, 
		0xc0400000, 
		0x2689ffef, 
		0xff96d159, 
		0x0e000fff, 
		0x8f7ffffe, 
		0x5130cddf, 
		0x7f9f3eda, 
		0xc1e00000, 
		0x4bd44a42, 
		0x42740000, 
		0xff000000, 
		0xff800000, 
		0x9f3ae3a0, 
		0x80000000, 
		0x16cd2e45, 
		0xc1200000, 
		0x42080000, 
		0x80000000, 
		0x40a00000, 
		0x00000000, 
		0xc0800000, 
		0x42280000, 
		0x2eff09ae, 
		0xc26c0000, 
		0x0c7fffc0, 
		0x42540000, 
		0x12268fde, 
		0x00000020, 
		0x00040000, 
		0x80000000, 
		0x80800003, 
		0xff800001, 
		0x3eea57c9, 
		0x41f80000, 
		0x4ea74f9d, 
		0x00000040, 
		0x80008000, 
		0xc1b00000, 
		0xb8032d42, 
		0x41f00000, 
		0x80800001, 
		0xffbfffff, 
		0x0c7ff800, 
		0x0e1fffff, 
		0xc1900000, 
		0x0e3fffff, 
		0x0e000001, 
		0xc2140000, 
		0x0e01ffff, 
		0xffc00000, 
		0x41880000, 
		0xcc24959d, 
		0xc1400000, 
		0x0bfcb4ec, 
		0x80000002, 
		0x42300000, 
		0x70b41638, 
		0x0c7e0000, 
		0x8075a5d0, 
		0x8cbc5d04, 
		0xc22c0000, 
		0x00004000, 
		0x80000000, 
		0x7fa9cffe, 
		0x7f8da49d, 
		0xc2540000, 
		0x80800003, 
		0x0e00003f, 
		0x7f7ffffe, 
		0x3f800001, 
		0x007ffffe, 
		0x7f8ac600, 
		0xbcc4a162, 
		0x7fb79afd, 
		0x80200000, 
		0x7f800000, 
		0xc1c00000, 
		0x7f7ffffe, 
		0xfe9f8c7e, 
		0x0c601899, 
		0x0c7ff000, 
		0xff800000, 
		0x00800001, 
		0x921b64af, 
		0x905620a8, 
		0xc1000000, 
		0xc2240000, 
		0x80000200, 
		0x42380000, 
		0x7fbfffff, 
		0x7f800000, 
		0x0c400000, 
		0x80000000, 
		0x7f000000, 
		0xc22c0000, 
		0x0e03ffff, 
		0x0e1fffff, 
		0x80000001, 
		0x7f7fffff, 
		0xa2a7f442, 
		0x00000000, 
		0x92457f7d, 
		0x8f7ffffe, 
		0xcb000000, 
		0x41400000, 
		0x4374e271, 
		0xff89c3da, 
		0x42680000, 
		0x3f800000, 
		0xc1e80000, 
		0xc2740000, 
		0xff7ffffe, 
		0x42100000, 
		0xc0000000, 
		0x7f800000, 
		0x7f800000, 
		0x372210e8, 
		0xcb8c4b40, 
		0x00000000, 
		0x5c2ccfe3, 
		0x80000000, 
		0x3f028f5c, 
		0xc2340000, 
		0x41600000, 
		0xd745008b, 
		0x3b17ccbe, 
		0xc2100000, 
		0xbf800000, 
		0x4b000000, 
		0x42640000, 
		0xffc00001, 
		0x41a80000, 
		0x423c0000, 
		0x391f42fa, 
		0xff800000, 
		0x7f800000, 
		0xc2280000, 
		0x9795bee0, 
		0x7f800000, 
		0x40c00000, 
		0x0e00ffff, 
		0x78c24759, 
		0xc2480000, 
		0x40800000, 
		0xc1b00000, 
		0x0e07ffff, 
		0x00100000, 
		0xbb5c7a13, 
		0x63de277e, 
		0x00800001, 
		0x007ffffe, 
		0xff800000, 
		0x00000000, 
		0x42500000, 
		0x0c7f8000, 
		0x0c7c0000, 
		0x80000020, 
		0x41e80000, 
		0x0e00007f, 
		0x03d352c3, 
		0x80200000, 
		0x00000000, 
		0xc1200000, 
		0x00002000, 
		0x80000000, 
		0x80000001, 
		0x80000000, 
		0x00000000, 
		0xffffffff, 
		0xc0c00000, 
		0xff800000, 
		0x7f800000, 
		0x80800003, 
		0x6146380e, 
		0xc2740000, 
		0x00080000, 
		0xc2200000, 
		0x5db1ebc7, 
		0xc7b42af0, 
		0xc1b00000, 
		0xd1fcdc84, 
		0x0c400000, 
		0x0e7fffff, 
		0x4e47598e, 
		0x7fc00000, 
		0xaaaaaaaa, 
		0x00040000, 
		0x42080000, 
		0xb88d8a9d, 
		0x0c7ffc00, 
		0x0c780000, 
		0x41c80000, 
		0xe6b3102a, 
		0x87dd4441, 
		0x051fc629, 
		0x00000000, 
		0xe5847b46, 
		0x3f800000, 
		0x1f6df9ba, 
		0x80008000, 
		0x1bc36daf, 
		0x0be1e44f, 
		0xcf847037, 
		0x00800001, 
		0x604a929e, 
		0x41000000, 
		0x00400000, 
		0xc1d80000, 
		0x1efef989, 
		0x807ffffe, 
		0x0c7fffc0, 
		0xc20c0000, 
		0x80800003, 
		0x41c80000, 
		0xa24ba7bd, 
		0x0e007fff, 
		0x42600000, 
		0x67b5a727, 
		0x80002000, 
		0xb0377e61, 
		0xff800000, 
		0x00496966, 
		0x7f800000, 
		0x0e000003, 
		0x41100000, 
		0x80ffffff, 
		0x0e007fff, 
		0x0c7c0000, 
		0x0165c582, 
		0xc1600000, 
		0xa521cb73, 
		0x06c286d1, 
		0x27cc7761, 
		0x0e000003, 
		0xc1d80000, 
		0xc2780000, 
		0x00002000, 
		0x41c00000, 
		0xc1200000, 
		0x7fa2614d, 
		0x7fc00000, 
		0x0e000003, 
		0x8001796b, 
		0x0f7ffffd, 
		0x0e03ffff, 
		0x4b000000, 
		0xbd367ae7, 
		0xc1400000, 
		0x0e00003f, 
		0x0c600000, 
		0xc1f00000, 
		0x42380000, 
		0xfd1f95d4, 
		0x0c7fff80, 
		0x80004000, 
		0xbf028f5c, 
		0xc2740000, 
		0x0e0fffff, 
		0x0c7fe000, 
		0x747ea59a, 
		0xf7d7f2f8, 
		0x42440000, 
		0x00800003, 
		0x00800001, 
		0x5154561d, 
		0x80000001, 
		0x80000010, 
		0xeec31b2e, 
		0x0c7c0000, 
		0x41e80000, 
		0x3f800000, 
		0x0e0001ff, 
		0x1f659dbd, 
		0xff800000, 
		0x00000000, 
		0xc27c0000, 
		0x225d304f, 
		0xd13a00c7, 
		0x00200000, 
		0x42180000, 
		0x41a80000, 
		0x41700000, 
		0xc2580000, 
		0x0f7ffffc, 
		0xff7ffffe, 
		0x20e3aa63, 
		0xb474044b, 
		0x75585e5d, 
		0x429f1c88, 
		0x00000000, 
		0x42580000, 
		0xc2a9bc3a, 
		0xc2200000, 
		0x80800000, 
		0x42240000, 
		0xc0000000, 
		0x0c7ffffe, 
		0xc1700000, 
		0xc24c0000, 
		0x8016b707, 
		0xc2100000, 
		0x42540000, 
		0x687275ba, 
		0x8f7ffffe, 
		0x7f800000, 
		0x80000001, 
		0x41b80000, 
		0x5b88be4e, 
		0x40800000, 
		0x80000002, 
		0x80100000, 
		0x0c7ff800, 
		0xf1e88311, 
		0xa9b33a17, 
		0xee1e2bfd, 
		0x80080000, 
		0xff800000, 
		0xc26c0000, 
		0x1ba7be32, 
		0x0c7ffff8, 
		0xc1600000, 
		0x0c7ffe00, 
		0x0c7ffff0, 
		0x41700000, 
		0x00800003, 
		0x42380000, 
		0x007ffffe, 
		0xff7ffffe, 
		0x80000000, 
		0x0e0fffff, 
		0x7f800000, 
		0xc1e00000, 
		0x647fa234, 
		0x80000040, 
		0x00000000, 
		0x40a00000, 
		0xc1e80000, 
		0x6f760fc4, 
		0x76135918, 
		0xe4b86007, 
		0x00800001, 
		0x0e0001ff, 
		0xda8f8128, 
		0x80000000, 
		0x0c7ffffe, 
		0xcb000000, 
		0xc2780000, 
		0xb5a95250, 
		0x0e00ffff, 
		0xce911d2a, 
		0x80001000, 
		0xd4f83f8e, 
		0x80000000, 
		0x1171cf89, 
		0xc2680000, 
		0xb214b9f1, 
		0x41e80000, 
		0x00000002, 
		0x0e07ffff, 
		0x5e4680b4, 
		0x00800002, 
		0x0c7ffffc, 
		0xbdde8caa, 
		0xc0400000, 
		0x80000010, 
		0xdd163b49, 
		0x88494885, 
		0x00400000, 
		0x00474413, 
		0x42380000, 
		0x00800002, 
		0x7f800000, 
		0xb7ae9b5f, 
		0xa66c86ae, 
		0xb0088799, 
		0x80800002, 
		0xca87e791, 
		0x40a00000, 
		0x0c700000, 
		0x80000000, 
		0xc1d80000, 
		0x3f800000, 
		0x0e000fff, 
		0x0c7ffffe, 
		0x77f17ec3, 
		0x0c7fe000, 
		0x80100000, 
		0x80800001, 
		0x804a33c0, 
		0xc2540000, 
		0x0e4a44ae, 
		0x0c7ffe00, 
		0x7f7ffffe, 
		0x00394185, 
		0x0c7fffff, 
		0x42540000, 
		0x89fdfa72, 
		0x11558fa7, 
		0x41980000, 
		0xabb20247, 
		0x3f800000, 
		0x684b9c79, 
		0x80000000, 
		0xfa6a2a1d, 
		0x8f7ffffc, 
		0x00400000, 
		0xc26c0000, 
		0x7f800000, 
		0x00000000, 
		0x0e00001f, 
		0xff800000, 
		0xdf7c45bd, 
		0x0c7fffc0, 
		0xe8bea9a2, 
		0x0c7f8000, 
		0xc1880000, 
		0x41e00000, 
		0x007fffff, 
		0x0892693a, 
		0xc2140000, 
		0x0e0fffff, 
		0xc1b80000, 
		0x7d3f10bf, 
		0x41c80000, 
		0xc1e80000, 
		0x0d000ff0, 
		0xffc00001, 
		0x00800000, 
		0x007fffff, 
		0x7f897cc1, 
		0x42340000, 
		0xc1980000, 
		0xc2180000, 
		0x41e00000, 
		0x80000200, 
		0x42780000, 
		0x00000000, 
		0x80011111, 
		0x00000002, 
		0x42240000, 
		0x42600000, 
		0x7f42a4d0, 
		0xff800001, 
		0x55f1bf62, 
		0x00000040, 
		0x27018eb4, 
		0x80800003, 
		0x0e001fff, 
		0x80010000, 
		0x80000800, 
		0xc1e80000, 
		0x0d000ff0, 
		0x41700000, 
		0x7fc00001, 
		0x80000001, 
		0x7fbfffff, 
		0x80000000, 
		0xdf00fc85, 
		0xffbaac7f, 
		0x0c7fffff, 
		0x80000080, 
		0xc2300000, 
		0xc2700000, 
		0xa4f946ac, 
		0xc3a51c0a, 
		0x0e01ffff, 
		0x5698cfad, 
		0x0c7fc000, 
		0x80000000, 
		0x00000002, 
		0x00000001, 
		0x41980000, 
		0x42bcd0d4, 
		0xaaaaaaaa, 
		0xc1c00000, 
		0xada2371b, 
		0x0c7fffe0, 
		0x00000100, 
		0x42140000, 
		0x2289278e, 
		0x7f7fffff, 
		0x41000000, 
		0x80800000, 
		0x327eeddd, 
		0xf51f22ae, 
		0xbf800001, 
		0x95da6306, 
		0x113a0d58, 
		0x4e122346, 
		0x42180000, 
		0xc24c0000, 
		0xc2140000, 
		0x9f6c9a77, 
		0xc25c0000, 
		0x80000020, 
		0x80000008, 
		0x0e003fff, 
		0x0c700000, 
		0x00020000, 
		0x00400000, 
		0x00002000, 
		0x80000001, 
		0xc2480000, 
		0x422c0000, 
		0x80000000, 
		0xff7fffff, 
		0xc1500000, 
		0xc0800000, 
		0x80000000, 
		0x460dd357, 
		0x0e000003, 
		0xa0045af9, 
		0x80800003, 
		0x2e47362b, 
		0xff86cc66, 
		0xffa5646e, 
		0x00000200, 
		0x461c7ead, 
		0xc2280000, 
		0x4ebd6107, 
		0x41e80000, 
		0xc2740000, 
		0x42200000, 
		0x41880000, 
		0x00000010, 
		0x00800003, 
		0x42780000, 
		0x7f800000, 
		0x00400000, 
		0x7f800000, 
		0xc1b80000, 
		0x00400000, 
		0x42700000, 
		0x0c7fffc0, 
		0xc2380000, 
		0xffc00001, 
		0xcf606ad9, 
		0x7fc00000, 
		0x0e001fff, 
		0x40c00000, 
		0x420c0000, 
		0x43660599, 
		0xc1000000, 
		0xff800000, 
		0x30bcb021, 
		0x8f7ffffc, 
		0x41a80000, 
		0x00000000, 
		0x41300000, 
		0x80800003, 
		0x663dcad4, 
		0xf87c0583, 
		0x00000100, 
		0xffc00001, 
		0xd8d97ee6, 
		0xdfa6dfbe, 
		0x00011111, 
		0x80800000, 
		0x80800000, 
		0x42680000, 
		0xc2580000, 
		0x421c0000, 
		0x0e0000ff, 
		0xaf6ef824, 
		0x41b80000, 
		0x1d7f771e, 
		0x80000000, 
		0x80000100, 
		0x0c7fc000, 
		0xcb000000, 
		0x535b5ba3, 
		0x40000000, 
		0x8055559c, 
		0x5cc3c23d, 
		0xf05bc473, 
		0x40400000, 
		0x7f800000, 
		0x80200000, 
		0x420c0000, 
		0x53791801, 
		0x0c7ffffe, 
		0xc2400000, 
		0xc1300000, 
		0x80000200, 
		0x00002000, 
		0x41d00000, 
		0x40a00000, 
		0xc21c0000, 
		0xc2600000, 
		0x0e0fffff, 
		0x7f800000, 
		0x807bd12b, 
		0xc25c0000, 
		0x80800000, 
		0x0c7fffff, 
		0x3f028f5c, 
		0xc2740000, 
		0x834bcc31, 
		0x12d23ce3, 
		0x0a4bb19e, 
		0x8f7fffff, 
		0xffa2a22f, 
		0xc1f80000, 
		0x80800000, 
		0x00400000, 
		0x6febe995, 
		0x00000000, 
		0x8ca2d27d, 
		0xc2400000, 
		0x0e00000f, 
		0xff7ffffe, 
		0x0e0fffff, 
		0xc0400000, 
		0x7edcb18f, 
		0xf6c69f5d, 
		0xc2200000, 
		0x0e007fff, 
		0x42340000, 
		0x42240000, 
		0xc2600000, 
		0x0e0000ff, 
		0x80800000, 
		0x0f7ffffe, 
		0x0c600000, 
		0xff800000, 
		0x0c7ffffc, 
		0x5911b091, 
		0x41600000, 
		0xffc00000, 
		0xb09cc41f, 
		0x00000000, 
		0xff1a71c7, 
		0x7f800000, 
		0x3f800000, 
		0xc2540000, 
		0x7fc00001, 
		0x0e0000ff, 
		0x40e1a67f, 
		0x9d914416, 
		0x00000040, 
		0x42380000, 
		0x41000000, 
		0xc24c0000, 
		0xc2600000, 
		0x42b241e0, 
		0x0c7fffff, 
		0x686b9c18, 
		0x13462b1e, 
		0x80000000, 
		0xff800000, 
		0x4bc56749, 
		0x0e00001f, 
		0x0e1fffff, 
		0x425c0000, 
		0x07958024, 
		0xc1e80000, 
		0xf0f31ae5, 
		0x42380000, 
		0xcbb73549, 
		0x42640000, 
		0x41a00000, 
		0x80000000, 
		0x80000000, 
		0x41f00000, 
		0xbf028f5c, 
		0x0e0aa0a0, 
		0x80000400, 
		0xcb000000, 
		0xff800000, 
		0x00800000, 
		0x40800000, 
		0x421da456, 
		0xc2000000, 
		0x0c7c0000, 
		0xff7fffff, 
		0xc1400000, 
		0xce79b880, 
		0x0c7fffff, 
		0x0c7ffffc, 
		0x80000001, 
		0x80000001, 
		0x7fbfffff, 
		0x3417c27a, 
		0x423c0000, 
		0xe78d6836, 
		0x00010000, 
		0x0e00ffff, 
		0x00800002, 
		0xc1900000, 
		0x0e0fffff, 
		0x41e80000, 
		0x3f800001, 
		0x00000008, 
		0x0e0fffff, 
		0xc1800000, 
		0xc1880000, 
		0x80800001, 
		0x00000200, 
		0xbf800000, 
		0x40800000, 
		0x96d7a17b, 
		0x7fffffff, 
		0x7f7fffff, 
		0x7f7ffffe, 
		0x21f25d6f, 
		0x00000000, 
		0x225e0202, 
		0x7594a7c3, 
		0x42780000, 
		0x42040000, 
		0xc2540000, 
		0x0f7ffffe, 
		0xc20c0000, 
		0x80000008, 
		0x806dcc8b, 
		0xf383e006, 
		0x0e00001f, 
		0x49faf421, 
		0x807fffff, 
		0xac54e9ce, 
		0xb33768b6, 
		0x00000001, 
		0x0e0003ff, 
		0x804ffb1f, 
		0x00000000, 
		0xff800000, 
		0x80800001, 
		0x6ff76730, 
		0x00000200, 
		0x0c7ffff8, 
		0x00000000, 
		0x00800002, 
		0x2895a107, 
		0x3b1a2b9c, 
		0x41a80000, 
		0xc1000000, 
		0x0e1fffff, 
		0x0c7f0000, 
		0xa119c88d, 
		0x00000002, 
		0xff7ffffe, 
		0xb983beb1, 
		0x35fa90fb, 
		0x5183be30, 
		0xff800000, 
		0x396f2de7, 
		0x7f7ffffe, 
		0x80000000, 
		0xf6474f28, 
		0x0d9414f1, 
		0x51e69ac6, 
		0x9a45c28c, 
		0xc20c0000, 
		0x19a664b4, 
		0x3d501af9, 
		0x00004000, 
		0x80000000, 
		0xc2680000, 
		0xc2040000, 
		0x7f83b9f7, 
		0x42080000, 
		0x0e00007f, 
		0x00000400, 
		0xc2280000, 
		0x00000000, 
		0xffc00000, 
		0x5eabb11b, 
		0xc46430e1, 
		0x7f7ffffe, 
		0x80000020, 
		0x0d00fff0, 
		0xbf800001, 
		0xc1100000, 
		0x42600000, 
		0x941cb6d2, 
		0x00800000, 
		0x00000020, 
		0x41500000, 
		0xad37c78f, 
		0x7f800000, 
		0x475d97d4, 
		0x8f7fffff, 
		0xb6b9c61e, 
		0x62150962, 
		0xc1c00000, 
		0x7fb9ebe6, 
		0xbf800001, 
		0x3ca23e30, 
		0x0c7f8000, 
		0x80000800, 
		0x722339fc, 
		0x78756049, 
		0x8f7ffffc, 
		0x00000000, 
		0xc2280000, 
		0x627eae22, 
		0xc9da4070, 
		0x2f49955b, 
		0x00000020, 
		0xe75bab63, 
		0x42480000, 
		0x36274097, 
		0x00040000, 
		0xa0f0ca9b, 
		0xa07bea9a, 
		0xc2680000, 
		0x80000800, 
		0xdb170533, 
		0x80080000, 
		0x41b80000, 
		0xff7fffff, 
		0x41f80000, 
		0xc1500000, 
		0xf258251a, 
		0x41f80000, 
		0x0e000001, 
		0x0e00007f, 
		0x0e000007, 
		0x00000002, 
		0xbf800001, 
		0xc3aaea80, 
		0x00000000, 
		0xc1f00000, 
		0x587458e9, 
		0x0c7ff000, 
		0xaaea3cf6, 
		0x00400000, 
		0x80000080, 
		0xbf800001, 
		0x0f7ffffc, 
		0xc1d80000, 
		0x4b000000, 
		0x80800003, 
		0xff7ffffe, 
		0x0e01ffff, 
		0x80000000, 
		0x0007c301, 
		0x00002000, 
		0x854ca69b, 
		0x0f7fffff, 
		0xff800000, 
		0x424c0000, 
		0x40400000, 
		0xc2200000, 
		0x08481def, 
		0x7fc00001, 
		0x33333333, 
		0x0e001fff, 
		0x42280000, 
		0x4b8c4b40, 
		0x80000000, 
		0x4f80bdc0, 
		0x00000000, 
		0xc1a00000, 
		0xc1400000, 
		0xc2580000, 
		0xc2280000, 
		0xc1900000, 
		0x41c80000, 
		0x41600000, 
		0x80000001, 
		0xcb05dee5, 
		0x4b000000, 
		0x3f800001, 
		0x8012d499, 
		0x41f80000, 
		0xc1a00000, 
		0x80000000, 
		0x382955fe, 
		0xc2540000, 
		0xff7ffffe, 
		0x7f800000, 
		0x80800003, 
		0x00000002, 
		0x0d00fff0, 
		0x8329710c, 
		0xc25c0000, 
		0x80100000, 
		0x7c3985e7, 
		0xff9751e5, 
		0xdd8accfb, 
		0x00000800, 
		0x80ffffff, 
		0x0c7c0000, 
		0x42080000, 
		0x00ffffff, 
		0x0301fd32, 
		0x80000000, 
		0xc26c0000, 
		0x1b05fbc4, 
		0x00000000, 
		0xbf800000, 
		0x05b19a4e, 
		0xcb000000, 
		0x00000000, 
		0x81cf01fb, 
		0xbf028f5c, 
		0x80000000, 
		0xda1fa9a3, 
		0x42040000, 
		0x00000000, 
		0xc2740000, 
		0x424c0000, 
		0xe2832f2a, 
		0x80000000, 
		0x96439302, 
		0x42200000, 
		0xc1d00000, 
		0x42140000, 
		0x046f2800, 
		0xc1e80000, 
		0x80000400, 
		0xbf800000, 
		0xfe4ec1f7, 
		0xc2740000, 
		0x41c80000, 
		0xda8ef2bb, 
		0x63b3038c, 
		0x5fa36049, 
		0xff7fffff, 
		0x7f800001, 
		0x427c0000, 
		0x80000000, 
		0x00004000, 
		0xfac2fe56, 
		0x7a4a819a, 
		0x7f800000, 
		0x0f7ffffd, 
		0x42340000, 
		0x80000800, 
		0x804df21e, 
		0x00000800, 
		0xff800000, 
		0x7f7fffff, 
		0x00020000, 
		0x80800000, 
		0x005d7eb0, 
		0x50cdb7b0, 
		0xd92de3ae, 
		0xaaaaaaaa, 
		0x00000000, 
		0x007ffffe, 
		0xcf8707b2, 
		0x7c4b6680, 
		0x41a00000, 
		0x00008000, 
		0x0c7fffe0, 
		0x33333333, 
		0x80000100, 
		0x41f80000, 
		0x80000000, 
		0xc2580000, 
		0x8f7fffff, 
		0xff800000, 
		0xc1a00000, 
		0x93b3a4cf, 
		0x42540000, 
		0x426c0000, 
		0xc1880000, 
		0x80000100, 
		0xb2b9fb78, 
		0xbf800000, 
		0x42480000, 
		0xaaaaaaaa, 
		0xc20c0000, 
		0xff800000, 
		0xb6f507b8, 
		0x00800000, 
		0x12ed6caa, 
		0xffa07a4c, 
		0xd6106461, 
		0x472e2e90, 
		0x0c7fffe0, 
		0xbf800001, 
		0x80040000, 
		0xbf800001, 
		0x0f7ffffe, 
		0xc88f3bd0, 
		0x4b8c4b40, 
		0x0f7ffffc, 
		0xfed3fbf8, 
		0x0d00fff0, 
		0xc2480000, 
		0xc0e00000, 
		0x0e03ffff, 
		0x7f9b0205, 
		0x42340000, 
		0xc1200000, 
		0x42040000, 
		0x0e0fffff, 
		0x420c0000, 
		0x4b000000, 
		0x0c7fc000, 
		0x0894ed82, 
		0x727a1799, 
		0x0e000fff, 
		0xbf028f5c, 
		0x0c400000, 
		0x9b19dde0, 
		0x41980000, 
		0xc1400000, 
		0x8f7ffffe, 
		0x46a01467, 
		0xc1900000, 
		0xffdf4a45, 
		0xc1400000, 
		0x7e9ad071, 
		0x8f7ffffd, 
		0xc1300000, 
		0x4aa7e22c, 
		0xbf800001, 
		0x7fe7b5c2, 
		0x00001000, 
		0x007ffffe, 
		0xd7796973, 
		0xfd15f4bf, 
		0xc2400000, 
		0x40a00000, 
		0xffb16070, 
		0xc1700000, 
		0xc2180000, 
		0x8f7ffffd, 
		0x41600000, 
		0xff7ffffe, 
		0x7f800000  
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
		0x00000077, 
		0x000000fd, 
		0x0000008f, 
		0x000002e9, 
		0x0000010d, 
		0x000000d5, 
		0x0000021f, 
		0x000001b7, 
		0x00000227, 
		0x0000020b, 
		0x00000243, 
		0x000001f3, 
		0x000003bb, 
		0x000003e1, 
		0x0000036d, 
		0x0000008f, 
		0x00000271, 
		0x0000021f, 
		0x00000097, 
		0x000003a9, 
		0x00000119, 
		0x000002f5, 
		0x00000133, 
		0x000003df, 
		0x000000f3, 
		0x0000006f, 
		0x00000393, 
		0x0000027f, 
		0x000002c1, 
		0x000003ef, 
		0x000001dd, 
		0x000002c3, 
		0x00000151, 
		0x000003e7, 
		0x0000028f, 
		0x00000131, 
		0x00000379, 
		0x000001bb, 
		0x000002b5, 
		0x00000391, 
		0x00000373, 
		0x0000015d, 
		0x00000109, 
		0x00000293, 
		0x00000059, 
		0x000002a7, 
		0x000001ef, 
		0x00000395, 
		0x000003af, 
		0x00000391, 
		0x00000201, 
		0x000000d3, 
		0x00000365, 
		0x00000271, 
		0x00000273, 
		0x000001e9, 
		0x000003f5, 
		0x000001db, 
		0x0000007f, 
		0x000003cd, 
		0x00000359, 
		0x0000010f, 
		0x0000034f, 
		0x00000215, 
		0x000001a3, 
		0x000001f3, 
		0x000000f5, 
		0x000002bf, 
		0x0000030d, 
		0x00000049, 
		0x000000ef, 
		0x000003c9, 
		0x000000c5, 
		0x00000005, 
		0x00000169, 
		0x00000033, 
		0x0000007f, 
		0x000003b3, 
		0x00000223, 
		0x000001ed, 
		0x00000179, 
		0x0000016b, 
		0x000001c9, 
		0x00000317, 
		0x00000121, 
		0x000002a7, 
		0x0000013d, 
		0x00000043, 
		0x00000271, 
		0x00000023, 
		0x000001e9, 
		0x00000381, 
		0x000002b3, 
		0x000003e3, 
		0x00000233, 
		0x00000013, 
		0x0000025f, 
		0x000001d9, 
		0x000001ab, 
		0x000001ff, 
		0x00000351, 
		0x0000025b, 
		0x000002f9, 
		0x0000006d, 
		0x00000119, 
		0x000001b5, 
		0x0000002d, 
		0x00000361, 
		0x0000008f, 
		0x000000c3, 
		0x000002f9, 
		0x000003eb, 
		0x0000015f, 
		0x00000063, 
		0x000003c5, 
		0x00000181, 
		0x000001f7, 
		0x000000cf, 
		0x0000015d, 
		0x0000025b, 
		0x0000036f, 
		0x00000057, 
		0x000003eb, 
		0x00000087, 
		0x000001ad, 
		0x00000143, 
		0x000003f7, 
		0x00000333, 
		0x000001d9, 
		0x00000247, 
		0x00000213, 
		0x00000263, 
		0x000003d7, 
		0x00000143, 
		0x00000115, 
		0x000002c5, 
		0x000003fd, 
		0x000000e3, 
		0x00000025, 
		0x000000c1, 
		0x0000003f, 
		0x0000027d, 
		0x000002f1, 
		0x00000099, 
		0x000003f5, 
		0x00000207, 
		0x000001c7, 
		0x000000cf, 
		0x0000031d, 
		0x00000005, 
		0x0000026b, 
		0x0000022b, 
		0x000001fb, 
		0x000000bb, 
		0x00000037, 
		0x0000026b, 
		0x000001a1, 
		0x000001af, 
		0x00000355, 
		0x00000017, 
		0x000001a1, 
		0x0000005b, 
		0x000000fb, 
		0x00000227, 
		0x0000015b, 
		0x00000161, 
		0x000003d5, 
		0x000003f1, 
		0x000002bd, 
		0x00000187, 
		0x000000cb, 
		0x00000265, 
		0x0000021b, 
		0x00000109, 
		0x000001c7, 
		0x0000031f, 
		0x000000c7, 
		0x000002eb, 
		0x000003f3, 
		0x00000089, 
		0x0000028f, 
		0x00000169, 
		0x00000185, 
		0x00000361, 
		0x00000167, 
		0x000003c1, 
		0x000001f1, 
		0x00000265, 
		0x00000255, 
		0x0000009d, 
		0x000003ad, 
		0x0000024f, 
		0x000003ab, 
		0x0000002d, 
		0x0000022f, 
		0x000003b3, 
		0x0000034d, 
		0x0000001d, 
		0x00000237, 
		0x00000223, 
		0x000003fb, 
		0x00000065, 
		0x000003e3, 
		0x0000016d, 
		0x000003c9, 
		0x0000002b, 
		0x000002bf, 
		0x00000189, 
		0x00000177, 
		0x00000039, 
		0x000001cf, 
		0x0000013f, 
		0x0000016f, 
		0x000002f7, 
		0x000000c7, 
		0x0000034b, 
		0x00000099, 
		0x000002e7, 
		0x00000355, 
		0x000002dd, 
		0x00000383, 
		0x000003e7, 
		0x00000171, 
		0x00000353, 
		0x00000145, 
		0x00000031, 
		0x0000022d, 
		0x00000255, 
		0x000002e5, 
		0x0000037b, 
		0x000000e9, 
		0x000000c7, 
		0x00000227, 
		0x0000024f, 
		0x0000033f, 
		0x0000027d, 
		0x000001e9, 
		0x000000dd, 
		0x0000029b, 
		0x0000028f, 
		0x000003b1, 
		0x00000187, 
		0x000003f5, 
		0x00000323, 
		0x0000030b, 
		0x00000189, 
		0x000002d3, 
		0x0000038f, 
		0x0000003b, 
		0x00000109, 
		0x000002af, 
		0x000001b5, 
		0x00000055, 
		0x000001df, 
		0x000001ff, 
		0x000001fb  
};

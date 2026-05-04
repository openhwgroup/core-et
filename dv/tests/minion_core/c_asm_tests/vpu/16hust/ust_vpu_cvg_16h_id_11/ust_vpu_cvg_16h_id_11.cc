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
			"li x5, 129\n"
			"faddi.pi f2, f0, 133\n"
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
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
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
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
			"li x0, 0xb\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_int_32\n"
			"flw.ps f26, 448(x22)\n"
			"flw.ps f10, 128(x22)\n"
			"feq.pi f10, f26, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 928(x15)\n"
			"flw.ps f26, 608(x15)\n"
			"flw.ps f1, 352(x15)\n"
			"fnmadd.s f13, f2, f26, f1, rtz\n" 
			"fbci.pi f0, 0x125ab\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 384(x22)\n"
			"fsrli.pi f0, f1, 0xa\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 544(x22)\n"
			"flw.ps f24, 896(x22)\n"
			"fsll.pi f24, f8, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 64(x7)\n"
			"flw.ps f1, 896(x7)\n"
			"feq.s x18, f8, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 96(x14)\n"
			"flw.ps f27, 96(x14)\n"
			"fminu.pi f0, f12, f27\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m7, x5, 0x75\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"mov.m.x m1, x19, 0x54\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 64(x7)\n"
			"flt.ps f2, f24, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 256(x5)\n"
			"flw.ps f26, 416(x5)\n"
			"fmaxu.pi f26, f8, f26\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 416(x19)\n"
			"fcvt.w.s x22, f27, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f0, 576(x15)\n"
			"flw.ps f24, 736(x15)\n"
			"fdiv.pi f24, f0, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 448(x22)\n"
			"fslli.pi f26, f12, 0xa\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 608(x14)\n"
			"flw.ps f2, 256(x14)\n"
			"flw.ps f29, 512(x14)\n"
			"fcmov.ps f23, f14, f2, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 192(x18)\n"
			"flw.ps f17, 256(x18)\n"
			"fsgnjn.ps f17, f1, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f21, 96(x15)\n"
			"flw.ps f15, 384(x15)\n"
			"fmin.pi f16, f21, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 832(x28)\n"
			"fcvt.pw.ps f17, f20, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 896(x5)\n"
			"flw.ps f9, 480(x5)\n"
			"flt.s x6, f24, f9\n" 
			"fbci.ps f17, 0x5af12\n" 
			"la x28, rand_int_32\n"
			"flw.ps f20, 640(x28)\n"
			"flw.ps f0, 192(x28)\n"
			"fmulhu.pi f20, f20, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 576(x19)\n"
			"flw.ps f31, 928(x19)\n"
			"fmul.s f23, f18, f31, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 512(x5)\n"
			"fmulhu.pi f28, f18, f18\n" 
			"li x25, 0xb90dd7dbbca2270d\n"
			"fcvt.s.wu f9, x25, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 704(x28)\n"
			"flw.ps f0, 192(x28)\n"
			"fle.ps f15, f28, f0\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 864(x5)\n"
			"flw.ps f15, 0(x5)\n"
			"fsll.pi f27, f27, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f15, 192(x12)\n"
			"flw.ps f27, 224(x12)\n"
			"fminu.pi f21, f15, f27\n" 
			"mova.x.m x12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 960(x12)\n"
			"flw.ps f17, 96(x12)\n"
			"fmulhu.pi f21, f10, f17\n" 
			"fbci.pi f27, 0xfa49\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 576(x19)\n"
			"flw.ps f28, 928(x19)\n"
			"fsgnj.ps f27, f31, f28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f18, 352(x6)\n"
			"fand.pi f15, f18, f18\n" 
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_SHORT_OPS_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_SHORT_OPS_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
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
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_2\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f0, 640(x28)\n"
			"flw.ps f16, 672(x28)\n"
			"fsll.pi f18, f0, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 160(x9)\n"
			"flw.ps f14, 96(x9)\n"
			"fmulh.pi f16, f8, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 704(x16)\n"
			"frsq.ps f20, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskor m1, m1, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 32(x11)\n"
			"frsq.ps f0, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f10, 352(x15)\n"
			"flw.ps f28, 416(x15)\n"
			"fmulhu.pi f25, f10, f28\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_2:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 896(x16)\n"
			"frcp.ps f8, f7\n" 
			"fbci.pi f18, 0x3db\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 672(x27)\n"
			"flw.ps f18, 448(x27)\n"
			"fminu.pi f25, f11, f18\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 480(x22)\n"
			"fsat8.pi f0, f29\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 320(x12)\n"
			"flw.ps f8, 192(x12)\n"
			"fsll.pi f11, f10, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 544(x26)\n"
			"fsatu8.pi f16, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 0(x11)\n"
			"flw.ps f17, 32(x11)\n"
			"fmaxu.pi f11, f20, f17\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_2:\n"
			"fltm.pi m2, f31, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_2:\n"
			"fltm.pi m1, f8, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f28, 992(x27)\n"
			"flw.ps f14, 640(x27)\n"
			"fxor.pi f8, f28, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x2 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xc << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x5 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 736(x25)\n"
			"flw.ps f4, 608(x25)\n"
			"flt.s x20, f30, f4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 864(x17)\n"
			"flw.ps f2, 800(x17)\n"
			"fadd.pi f31, f20, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 608(x26)\n"
			"flw.ps f6, 32(x26)\n"
			"fmax.ps f27, f25, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f5, 160(x12)\n"
			"flw.ps f17, 576(x12)\n"
			"fremu.pi f30, f5, f17\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 448(x10)\n"
			"flw.ps f0, 640(x10)\n"
			"fle.s x20, f14, f0\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 32(x12)\n"
			"fsrai.pi f17, f4, 0x9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 288(x11)\n"
			"flw.ps f31, 384(x11)\n"
			"fcmovm.ps f11, f7, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 512(x11)\n"
			"fcvt.f16.ps f27, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 0(x20)\n"
			"flw.ps f25, 288(x20)\n"
			"fsgnjn.ps f20, f7, f25\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 256(x11)\n"
			"flw.ps f2, 608(x11)\n"
			"fsgnjx.s f20, f6, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 64(x26)\n"
			"flw.ps f25, 800(x26)\n"
			"flw.ps f27, 192(x26)\n"
			"fnmadd.s f17, f20, f25, f27, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"mov.m.x m1, x15, 0x52\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"li x15, 0x26bc7685346f5411\n"
			"fcvt.s.w f13, x15, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 800(x15)\n"
			"flw.ps f11, 0(x15)\n"
			"fmin.pi f27, f20, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 384(x6)\n"
			"fcvt.w.s x12, f20, rne\n" 
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 832(x17)\n"
			"fsgnjn.ps f7, f27, f27\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f17, 928(x22)\n"
			"fcvt.ps.f16 f9, f17\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 608(x28)\n"
			"flw.ps f21, 896(x28)\n"
			"fmulhu.pi f17, f22, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 0(x11)\n"
			"fclass.ps f26, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 832(x10)\n"
			"fcvt.ps.pw f26, f13, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 672(x28)\n"
			"fsin.ps f30, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_4:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 864(x28)\n"
			"flog.ps f22, f7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 480(x10)\n"
			"feq.pi f3, f7, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 128(x17)\n"
			"flw.ps f2, 704(x17)\n"
			"fadd.ps f22, f25, f2, rdn\n" 
			"li x5, 0x9b3aa5f9b1d660ab\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfff485c58bf808b4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 384(x25)\n"
			"fcvt.f16.ps f13, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 32(x18)\n"
			"ffrc.ps f9, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 640(x10)\n"
			"flw.ps f0, 352(x10)\n"
			"flw.ps f19, 288(x10)\n"
			"fnmadd.ps f3, f27, f0, f19, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 768(x6)\n"
			"flw.ps f3, 512(x6)\n"
			"fadd.ps f3, f21, f3, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 640(x19)\n"
			"flw.ps f2, 928(x19)\n"
			"flw.ps f22, 928(x19)\n"
			"fmsub.ps f26, f17, f2, f22, rdn\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f9, 352(x19)\n"
			"fcvt.ps.f16 f25, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 128(x17)\n"
			"flw.ps f12, 128(x17)\n"
			"fadd.ps f12, f6, f12, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x1 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x8 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
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
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_5\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 64(x11)\n"
			"fcvt.pwu.ps f23, f8, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 864(x14)\n"
			"fle.pi f17, f22, f22\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 960(x14)\n"
			"flw.ps f2, 704(x14)\n"
			"fmul.pi f15, f23, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 832(x6)\n"
			"flw.ps f26, 608(x6)\n"
			"flt.s x11, f17, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 448(x22)\n"
			"fcvt.pwu.ps f3, f29, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 736(x5)\n"
			"fsgnjx.s f8, f12, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 192(x22)\n"
			"flw.ps f16, 544(x22)\n"
			"feq.ps f26, f23, f16\n" 
			"mova.x.m x14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 416(x15)\n"
			"fsin.ps f2, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 864(x5)\n"
			"flw.ps f3, 192(x5)\n"
			"fmin.s f12, f13, f3\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"fltm.pi m5, f17, f9\n" 
			"li x5, 0x80340158\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 480(x5)\n"
			"flw.ps f29, 384(x5)\n"
			"flw.ps f16, 256(x5)\n"
			"fmsub.ps f9, f10, f29, f16, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"maskand m7, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 896(x5)\n"
			"flw.ps f26, 672(x5)\n"
			"fsgnjx.s f3, f23, f26\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 32(x14)\n"
			"flw.ps f15, 992(x14)\n"
			"fltu.pi f3, f26, f15\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"feqm.ps m5, f8, f20\n" 
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
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
			"la x6, rand_int_32\n"
			"flw.ps f1, 704(x6)\n"
			"fcvt.ps.pw f17, f1, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 128(x28)\n"
			"fcvt.pwu.ps f25, f23, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 704(x17)\n"
			"flw.ps f5, 832(x17)\n"
			"fsub.pi f10, f1, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 224(x6)\n"
			"flw.ps f5, 352(x6)\n"
			"fadd.s f17, f12, f5, rdn\n" 
			"maskpopcz x7, m2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 352(x5)\n"
			"flw.ps f17, 608(x5)\n"
			"fadd.s f28, f12, f17, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 224(x7)\n"
			"fcvt.ps.pw f28, f10, rup\n" 
			"li x6, 0xbf73a20f575d583\n"
			"fcvt.s.wu f1, x6, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 224(x5)\n"
			"flw.ps f25, 352(x5)\n"
			"fsub.pi f28, f8, f25\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 320(x15)\n"
			"flw.ps f4, 704(x15)\n"
			"feq.pi f30, f12, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"flem.ps m2, f10, f1\n" 
			"li x5, 0x80340180\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f4, 352(x5)\n"
			"flw.ps f11, 96(x5)\n"
			"fmax.pi f30, f4, f11\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_6:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 480(x7)\n"
			"frcp.ps f17, f18\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 448(x16)\n"
			"flw.ps f23, 960(x16)\n"
			"fadd.s f11, f10, f23, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 32(x13)\n"
			"flw.ps f11, 160(x13)\n"
			"fle.pi f4, f21, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 576(x6)\n"
			"flw.ps f11, 160(x6)\n"
			"fadd.ps f8, f8, f11, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
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
			"maskand m6, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_16\n"
			"flw.ps f7, 96(x18)\n"
			"fcvt.ps.f16 f22, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 608(x14)\n"
			"flw.ps f24, 640(x14)\n"
			"fmin.ps f22, f1, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 320(x18)\n"
			"flw.ps f7, 32(x18)\n"
			"fmax.ps f29, f29, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"fltm.pi m6, f18, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f22, 896(x10)\n"
			"flw.ps f20, 512(x10)\n"
			"fle.pi f27, f22, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"maskand m0, m7, m0\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe87459af85\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f7, 448(x11)\n"
			"fsatu8.pi f10, f7\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f3, 352(x22)\n"
			"fcvt.ps.f16 f19, f3\n" 
			"maskpopcz x11, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 320(x5)\n"
			"flw.ps f0, 96(x5)\n"
			"fsub.ps f7, f6, f0, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_7:\n"
			"fltm.ps m4, f0, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f19, 224(x11)\n"
			"flw.ps f29, 480(x11)\n"
			"fsll.pi f20, f19, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 672(x5)\n"
			"flw.ps f6, 384(x5)\n"
			"fle.pi f4, f3, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 800(x5)\n"
			"fswizz.ps f18, f10, 0x18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 704(x26)\n"
			"fsrai.pi f22, f10, 0xc\n" 
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
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
			"la x25, rand_int_32\n"
			"flw.ps f8, 832(x25)\n"
			"fsrli.pi f3, f8, 0x7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 96(x5)\n"
			"flw.ps f0, 576(x5)\n"
			"fsub.ps f30, f3, f0, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 416(x22)\n"
			"flw.ps f10, 768(x22)\n"
			"flw.ps f17, 544(x22)\n"
			"fnmadd.ps f18, f18, f10, f17, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 224(x19)\n"
			"flw.ps f0, 416(x19)\n"
			"fadd.ps f23, f3, f0, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_8:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 704(x5)\n"
			"frcp.ps f30, f0\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 768(x22)\n"
			"flw.ps f25, 576(x22)\n"
			"fand.pi f10, f4, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 224(x5)\n"
			"fcvt.pwu.ps f17, f24, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 672(x10)\n"
			"fround.ps f3, f25, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 128(x9)\n"
			"flw.ps f29, 288(x9)\n"
			"flw.ps f2, 192(x9)\n"
			"fnmadd.s f24, f18, f29, f2, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 448(x9)\n"
			"flw.ps f26, 800(x9)\n"
			"fnmadd.s f8, f25, f26, f25, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 160(x13)\n"
			"fcvt.pwu.ps f25, f30, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 0(x9)\n"
			"flw.ps f3, 864(x9)\n"
			"fle.pi f5, f2, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 832(x5)\n"
			"flw.ps f5, 864(x5)\n"
			"flw.ps f30, 800(x5)\n"
			"fmsub.ps f2, f21, f5, f30, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 96(x11)\n"
			"flw.ps f0, 832(x11)\n"
			"flt.s x9, f10, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 544(x19)\n"
			"flw.ps f21, 672(x19)\n"
			"flw.ps f4, 160(x19)\n"
			"fnmadd.s f0, f2, f21, f4, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 640(x25)\n"
			"flw.ps f25, 416(x25)\n"
			"fmin.s f5, f26, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_551856_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x5 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xe << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x1 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
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
			"la x19, rand_int_32\n"
			"flw.ps f14, 480(x19)\n"
			"flw.ps f21, 704(x19)\n"
			"fmul.pi f30, f14, f21\n" 
			"maskpopc x17, m6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"feqm.ps m6, f5, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f2, 608(x26)\n"
			"fmin.pi f21, f2, f2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 384(x12)\n"
			"flw.ps f20, 928(x12)\n"
			"fxor.pi f16, f28, f20\n" 
			"la x19, rand_int_32\n"
			"flw.ps f22, 832(x19)\n"
			"flw.ps f21, 480(x19)\n"
			"fmin.pi f5, f22, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 480(x18)\n"
			"fsat8.pi f2, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 512(x12)\n"
			"flw.ps f21, 64(x12)\n"
			"fcmovm.ps f30, f14, f21\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 224(x18)\n"
			"flw.ps f18, 384(x18)\n"
			"fmin.s f2, f21, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 416(x26)\n"
			"fsatu8.pi f18, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 832(x26)\n"
			"flw.ps f14, 352(x26)\n"
			"fcmov.ps f26, f26, f26, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 416(x7)\n"
			"fpackreph.pi f16, f21\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 352(x20)\n"
			"flw.ps f14, 160(x20)\n"
			"flw.ps f13, 960(x20)\n"
			"fnmadd.ps f15, f30, f14, f13, rmm\n" 
			"fbci.pi f13, 0x4d3aa\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 160(x19)\n"
			"flw.ps f9, 960(x19)\n"
			"fsrl.pi f18, f18, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"maskor m4, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 960(x26)\n"
			"fround.ps f18, f10, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 608(x23)\n"
			"fclass.ps f23, f16\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 800(x13)\n"
			"fsatu8.pi f4, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 96(x27)\n"
			"flw.ps f20, 736(x27)\n"
			"fsrl.pi f26, f28, f20\n" 
			"li x5, 0x6a8b\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 544(x13)\n"
			"fle.s x13, f20, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 320(x6)\n"
			"flw.ps f30, 448(x6)\n"
			"flw.ps f18, 96(x6)\n"
			"fmsub.s f10, f28, f30, f18, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 224(x13)\n"
			"fpackrepb.pi f30, f22\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 96(x26)\n"
			"fpackrepb.pi f6, f23\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 512(x5)\n"
			"flog.ps f21, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 672(x26)\n"
			"flw.ps f18, 928(x26)\n"
			"flw.ps f8, 352(x26)\n"
			"fmsub.s f8, f10, f18, f8, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 256(x20)\n"
			"fsin.ps f27, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 640(x6)\n"
			"fround.ps f10, f14, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 544(x10)\n"
			"flw.ps f26, 928(x10)\n"
			"fmulh.pi f28, f30, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 256(x7)\n"
			"fswizz.ps f23, f4, 0x9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 768(x13)\n"
			"fcvt.pwu.ps f16, f2, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x7 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x2 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x1 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f27, 288(x5)\n"
			"flw.ps f5, 32(x5)\n"
			"fsra.pi f7, f27, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 672(x9)\n"
			"flw.ps f19, 672(x9)\n"
			"fmin.ps f31, f27, f19\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 256(x5)\n"
			"fcvt.f16.ps f9, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"feqm.ps m1, f9, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 768(x17)\n"
			"flw.ps f9, 256(x17)\n"
			"flw.ps f1, 544(x17)\n"
			"fcmov.ps f24, f12, f9, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 320(x5)\n"
			"flw.ps f31, 704(x5)\n"
			"fsub.ps f27, f11, f31, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 320(x17)\n"
			"fswizz.ps f15, f24, 0xb\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 544(x9)\n"
			"fcvt.f16.ps f7, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 928(x5)\n"
			"flw.ps f10, 64(x5)\n"
			"flw.ps f19, 736(x5)\n"
			"fnmsub.ps f19, f9, f10, f19, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 128(x5)\n"
			"flw.ps f15, 640(x5)\n"
			"fsub.ps f9, f21, f15, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f7, 992(x15)\n"
			"fsrai.pi f7, f7, 0x2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 256(x15)\n"
			"fcvt.w.s x17, f24, rdn\n" 
			"li x13, 0xcdfe8e0b3e4dbbd9\n"
			"fcvt.s.w f31, x13, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 256(x27)\n"
			"flw.ps f15, 992(x27)\n"
			"fsgnjx.ps f9, f21, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 480(x20)\n"
			"fround.ps f5, f24, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"feqm.ps m0, f27, f31\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa10e081419\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
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
			"masknot m2, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xdfded48b21\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f21, 0x22c1e\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 672(x27)\n"
			"frcp.ps f0, f7\n" 
			"la x25, rand_int_32\n"
			"flw.ps f7, 32(x25)\n"
			"flw.ps f10, 384(x25)\n"
			"fadd.pi f5, f7, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 832(x27)\n"
			"fcvt.w.s x28, f5, rne\n" 
			"li x5, 0xfb11\n"
			"csrw tensor_mask, x5\n"	
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 512(x25)\n"
			"flw.ps f3, 800(x25)\n"
			"flt.ps f31, f12, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 864(x14)\n"
			"fswizz.ps f20, f18, 0x1b\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 640(x20)\n"
			"flw.ps f7, 640(x20)\n"
			"flw.ps f2, 832(x20)\n"
			"fnmsub.ps f10, f15, f7, f2, rdn\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_12:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 832(x10)\n"
			"fexp.ps f5, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_12:\n"
			"fltm.pi m1, f27, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_12:\n"
			"maskand m3, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x17, 0xb87d70a387ff0652\n"
			"fcvt.s.lu f27, x17, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 320(x28)\n"
			"flw.ps f7, 256(x28)\n"
			"flw.ps f21, 928(x28)\n"
			"fnmadd.ps f15, f20, f7, f21, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 32(x27)\n"
			"fcvt.wu.s x22, f15, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 512(x20)\n"
			"flw.ps f15, 224(x20)\n"
			"fminu.pi f3, f21, f15\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 384(x14)\n"
			"flw.ps f14, 320(x14)\n"
			"flw.ps f31, 384(x14)\n"
			"fnmsub.s f27, f29, f14, f31, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_44286_HID_12:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
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
			"LBL_KERNEL_HID_13:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_13\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f10, 832(x13)\n"
			"fcvt.ps.pwu f5, f10, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"maskor m6, m3, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f20, 288(x13)\n"
			"flw.ps f24, 704(x13)\n"
			"fminu.pi f22, f20, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 352(x27)\n"
			"flw.ps f10, 192(x27)\n"
			"flw.ps f6, 960(x27)\n"
			"fmadd.ps f9, f20, f10, f6, rmm\n" 
			"li x7, 0xef695633c03f9894\n"
			"fcvt.s.wu f24, x7, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 32(x11)\n"
			"fcvt.f16.ps f16, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 736(x9)\n"
			"faddi.pi f1, f5, 0x1eb\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 768(x17)\n"
			"frsq.ps f1, f29\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 736(x15)\n"
			"flw.ps f16, 160(x15)\n"
			"fsra.pi f6, f18, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 0(x14)\n"
			"flw.ps f7, 800(x14)\n"
			"fsgnj.ps f18, f1, f7\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_13:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 768(x9)\n"
			"frcp.ps f21, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 512(x9)\n"
			"flw.ps f18, 416(x9)\n"
			"fxor.pi f22, f7, f18\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 480(x27)\n"
			"flw.ps f23, 288(x27)\n"
			"feq.s x22, f6, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 672(x14)\n"
			"flw.ps f20, 256(x14)\n"
			"fmax.s f9, f19, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 256(x14)\n"
			"flw.ps f7, 480(x14)\n"
			"flw.ps f16, 928(x14)\n"
			"fmadd.s f29, f19, f7, f16, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"fltm.ps m7, f18, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 16\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f6, 480(x5)\n"
			"flw.ps f24, 416(x5)\n"
			"fmulh.pi f27, f6, f24\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f30, 384(x20)\n"
			"fexp.ps f9, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 352(x19)\n"
			"fswizz.ps f21, f27, 0xf\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 288(x14)\n"
			"fclass.ps f24, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 416(x19)\n"
			"fcvt.wu.s x20, f9, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 416(x5)\n"
			"fsgnj.ps f6, f22, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 160(x20)\n"
			"fslli.pi f25, f6, 0xb\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 672(x5)\n"
			"faddi.pi f6, f28, 0x184\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 640(x28)\n"
			"flw.ps f10, 960(x28)\n"
			"feq.ps f24, f22, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"fsetm.pi m5, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f6, 128(x28)\n"
			"fsrai.pi f28, f6, 0xf\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f17, 576(x28)\n"
			"fslli.pi f27, f17, 0x8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 640(x6)\n"
			"fsrai.pi f6, f30, 0x7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 384(x6)\n"
			"flw.ps f15, 352(x6)\n"
			"flt.s x25, f9, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 64(x20)\n"
			"flw.ps f24, 320(x20)\n"
			"flt.ps f28, f21, f24\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 160(x6)\n"
			"fpackrepb.pi f28, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x4 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x1 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1e << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrwi mhpmevent3, 7\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
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
			"la x12, rand_int_32\n"
			"flw.ps f8, 64(x12)\n"
			"fslli.pi f2, f8, 0x8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 640(x9)\n"
			"fcvt.f16.ps f15, f31\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 160(x28)\n"
			"fpackrepb.pi f1, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 224(x13)\n"
			"fcvt.f16.ps f13, f31\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 384(x7)\n"
			"flw.ps f11, 544(x7)\n"
			"fadd.pi f8, f26, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 704(x13)\n"
			"fclass.s x17, f31\n" 
			"la x13, rand_int_32\n"
			"flw.ps f11, 576(x13)\n"
			"fcvt.ps.pw f1, f11, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 64(x15)\n"
			"flw.ps f2, 64(x15)\n"
			"fcmovm.ps f15, f25, f2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 160(x16)\n"
			"fsrli.pi f11, f31, 0x5\n" 
			"maskpopcz x13, m4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 576(x28)\n"
			"fclass.ps f24, f1\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 768(x9)\n"
			"flw.ps f1, 448(x9)\n"
			"fmax.pi f0, f10, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"masknot m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f8, 96(x16)\n"
			"flw.ps f10, 320(x16)\n"
			"fmul.pi f15, f8, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 96(x28)\n"
			"fclass.ps f0, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 448(x15)\n"
			"fsatu8.pi f0, f24\n" 
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
		0x9dc3eb68, 
		0xe33bcb5e, 
		0xa0ed6ab8, 
		0x1d381e91, 
		0x7405f5f8, 
		0xaa5b19ba, 
		0x3ae303f8, 
		0x92a09317, 
		0xa098d15a, 
		0x72627625, 
		0x97e6c86a, 
		0x9835d8fa, 
		0xee7dfbc6, 
		0x77fba7fe, 
		0xfe2cd7cf, 
		0x501e6eaf, 
		0xbc9bac6e, 
		0x66efcf8e, 
		0x7db77776, 
		0x3bef4f67, 
		0xf00cd4e7, 
		0xd44bc849, 
		0xcc346177, 
		0x82f85ec6, 
		0xfe621d37, 
		0x10984529, 
		0xdebeb4a5, 
		0xb62de46f, 
		0xcea2a74a, 
		0x743da5dc, 
		0x894c3fba, 
		0xdd94a46c, 
		0x322c6495, 
		0xa4c9f3e1, 
		0x12d41aff, 
		0x8e1441e4, 
		0x3de6f0e0, 
		0xcccac83a, 
		0xdae474d5, 
		0xe7db455c, 
		0x5da862fc, 
		0x19afd0de, 
		0xdd66ddef, 
		0xd5790372, 
		0x1e0e45a3, 
		0x637d6cc7, 
		0x9df759aa, 
		0xe6dbf329, 
		0x12670800, 
		0xa113e465, 
		0x6e46c8b2, 
		0x9a63021a, 
		0xb1e1f9d2, 
		0x20c688e1, 
		0x776a09b0, 
		0x210cf419, 
		0x1db3bdfa, 
		0xf0e75be0, 
		0x5b87a208, 
		0x32374ec6, 
		0xfeadad5a, 
		0xb11053b8, 
		0x6883db31, 
		0x2572055f, 
		0xa1982f92, 
		0x6cba8f47, 
		0x68b3af8a, 
		0x74f05b70, 
		0x81a91f69, 
		0xf1104848, 
		0xb4208fe9, 
		0x3c2ce319, 
		0x944af399, 
		0x48ed8d7e, 
		0xdabeb09a, 
		0x7476374e, 
		0xef061fea, 
		0x633bbb50, 
		0xbe484c87, 
		0x99ec65c7, 
		0x106ed76f, 
		0x6b4a1631, 
		0xf45acc44, 
		0xb1e5755d, 
		0x3ea5e331, 
		0x695a0546, 
		0x406c9da4, 
		0x6baf5122, 
		0x57711534, 
		0x9738ac08, 
		0x19742a15, 
		0xe5823bd5, 
		0x5d13b508, 
		0x5fde3f09, 
		0xa20c1fd1, 
		0x781cca9e, 
		0x5c9b344d, 
		0x50e782eb, 
		0x9b04c976, 
		0xf86c961d, 
		0xf9f91af4, 
		0x6527e171, 
		0xd423b215, 
		0xb377af34, 
		0xff64b0ee, 
		0x359d05ef, 
		0x65075ea7, 
		0x1562a748, 
		0xbb1a8b09, 
		0x1ea3e019, 
		0x77fa6882, 
		0x040b7e6f, 
		0xdc147884, 
		0x0630ddb9, 
		0x59e20525, 
		0xca0a97d0, 
		0x1fe52d27, 
		0x9160366a, 
		0x831f9c86, 
		0x4b41b4e4, 
		0xc708b90d, 
		0x69f6d5e6, 
		0x73a1ed8e, 
		0x8ee0ebd3, 
		0x7599fb0e, 
		0x27a3ed81, 
		0x86642bc6, 
		0xf6b981a0, 
		0x02bdb2f1, 
		0xd184e67e, 
		0xb66d8c4e, 
		0x2813e6c3, 
		0x074e3ff6, 
		0x1c39ab0c, 
		0x0bcbaf78, 
		0x6a880739, 
		0x140ed8e5, 
		0xeb38002e, 
		0x7d2ead5b, 
		0xf24a6c6e, 
		0xe7638e61, 
		0x4ca20ea2, 
		0x421ebc23, 
		0x5e2bf42d, 
		0x7a144b1c, 
		0x6e34e42c, 
		0x7368d8cd, 
		0x0cb080a7, 
		0x0fcdf40e, 
		0x8722ac82, 
		0x8c2bb775, 
		0x5f36e6bd, 
		0x507d73f1, 
		0x024b2632, 
		0xf75f8395, 
		0x97e68ec8, 
		0xf5f6fbbb, 
		0x4d19ae8b, 
		0x96e52bbd, 
		0x50ac851a, 
		0x7f64d30d, 
		0xc0f7d1a0, 
		0x17c3d652, 
		0xdc2c5e40, 
		0x66fc8f71, 
		0xae0d3e22, 
		0x15e0e569, 
		0xfbedfa05, 
		0xd4df9d54, 
		0x5403fc07, 
		0x513e9bfd, 
		0xd1c19ccb, 
		0xc9ce29d1, 
		0xa46bd464, 
		0xc37a1045, 
		0x0c467c01, 
		0x9fec7658, 
		0x8d55cb55, 
		0xed649a0a, 
		0xc0b31d58, 
		0xbca66006, 
		0x6b18a854, 
		0x13304d85, 
		0xc8098bb1, 
		0xc06639c3, 
		0x1e33d738, 
		0xd5822689, 
		0x1e4ee4b3, 
		0x8f6972ff, 
		0x360de42c, 
		0x6a31a124, 
		0x894cd25a, 
		0x1f948adb, 
		0xdfab62cf, 
		0x78eeeee3, 
		0x60a59869, 
		0xed9c9e4c, 
		0x7e5e13a4, 
		0xc7517d38, 
		0x95f8792e, 
		0x33be6f22, 
		0x598929d5, 
		0x63786675, 
		0xd7a0ec31, 
		0x95756b12, 
		0xfb093fca, 
		0x53f93f84, 
		0xb28a6c03, 
		0x42e22ae6, 
		0xbafc1bad, 
		0x9331cbb8, 
		0xd328752d, 
		0x60a889b4, 
		0xb76b14cb, 
		0x9e1d3caa, 
		0x8f3d5c2d, 
		0xf9ca5b9f, 
		0x74ae2f78, 
		0x7c9f029c, 
		0x3b37e4bb, 
		0xd8f1598c, 
		0xb2cf2480, 
		0x916d7ff8, 
		0x8d2f706b, 
		0x5541d342, 
		0x26cc6590, 
		0xc3c54edf, 
		0xce45b6a9, 
		0x1901880c, 
		0xc8b973d4, 
		0x9c1ddafd, 
		0x1eed3f50, 
		0xc3cf97ac, 
		0x951b0a1f, 
		0xec870d8f, 
		0xb074888b, 
		0x5d3073b1, 
		0x43c4b8b4, 
		0x3fcb79e6, 
		0x05346c2e, 
		0x4b047813, 
		0x21ada2cb, 
		0x7c206342, 
		0x45ca3d30, 
		0x825b4ec9, 
		0xdfb260c0, 
		0xdad00218, 
		0x9c015d97, 
		0xbbe1fe92, 
		0x4285c284, 
		0x0de8ac90, 
		0x3bc1f191, 
		0x5ed797df, 
		0xbaaefc67, 
		0x8171faa5, 
		0x3b709c1a  
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
		0x7403, 
		0xd300, 
		0x296b, 
		0x4700, 
		0x0000, 
		0x5280, 
		0x46b8, 
		0x4980, 
		0xf6d2, 
		0x52a0, 
		0x53c0, 
		0x4900, 
		0x1799, 
		0x5000, 
		0x4f00, 
		0x71b6, 
		0xcb00, 
		0x6614, 
		0xd360, 
		0x4a80, 
		0x50c0, 
		0xd360, 
		0x52a0, 
		0x4cc0, 
		0xce40, 
		0x7c00, 
		0x51e0, 
		0xd360, 
		0xd3a0, 
		0xd160, 
		0x5280, 
		0x5140, 
		0x5180, 
		0x3c00, 
		0xcd7b, 
		0xd4f1, 
		0x4d80, 
		0x4cc0, 
		0x5000, 
		0x53a0, 
		0xd0a0, 
		0x4dc0, 
		0xcdc0, 
		0x993f, 
		0x4800, 
		0xc000, 
		0xcfc0, 
		0x485f, 
		0xce00, 
		0x7c00, 
		0x4ec0, 
		0x52a0, 
		0x8fa5, 
		0xd3e0, 
		0x4c40, 
		0xd0c0, 
		0xb7a1, 
		0xc800, 
		0x8000, 
		0x4600, 
		0x8000, 
		0x52c0, 
		0x4c80, 
		0x4e80, 
		0xd220, 
		0x97ba, 
		0x4c80, 
		0x208b, 
		0xb2e9, 
		0xd2e0, 
		0x4c80, 
		0xd3c0, 
		0xcf40, 
		0xcdc0, 
		0x4b80, 
		0x5120, 
		0xc980, 
		0x5060, 
		0xfc00, 
		0xfc00, 
		0x4900, 
		0x793d, 
		0x7c00, 
		0xd380, 
		0xcc80, 
		0xeb93, 
		0xbc28, 
		0x4c80, 
		0xd3c0, 
		0x8000, 
		0x182f, 
		0x5260, 
		0x21b3, 
		0x8000, 
		0x4dc0, 
		0x3c00, 
		0x50c0, 
		0x5160, 
		0xd1a0, 
		0xfc00, 
		0xcc00, 
		0x178a, 
		0xa555, 
		0x463f, 
		0xd040, 
		0xd300, 
		0x1acf, 
		0x5080, 
		0x0164, 
		0x7c00, 
		0xfc00, 
		0x8000, 
		0x5040, 
		0xfc5d, 
		0x53e0, 
		0x051f, 
		0x2734, 
		0x4f80, 
		0x1f9d, 
		0x7c00, 
		0x51a0, 
		0x0000, 
		0x2f5e, 
		0xfc00, 
		0xccc0, 
		0xce40, 
		0xd0e0, 
		0xd3c0, 
		0x6931, 
		0x4b80, 
		0x50c0, 
		0x46a2, 
		0x5380, 
		0x110a, 
		0xd200, 
		0x50e0, 
		0x4dc0, 
		0x3b79, 
		0xd060, 
		0xd0a0, 
		0x8000, 
		0x5d6a, 
		0x4000, 
		0x53e0, 
		0x4f40, 
		0x2dde, 
		0xce40, 
		0x4200, 
		0x2fb6, 
		0xd2a0, 
		0x50a0, 
		0xac70, 
		0x4ec0, 
		0x1374, 
		0x7de5, 
		0x7c00, 
		0x4e40, 
		0x2d98, 
		0x5160, 
		0xd360, 
		0x50a0, 
		0xfcdd, 
		0xd2c0, 
		0xd0c0, 
		0xd2c0, 
		0x4b80, 
		0x52e0, 
		0xce00, 
		0xd040, 
		0xfc00, 
		0xc400, 
		0x4e00, 
		0xd060, 
		0xcb80, 
		0x4dc0, 
		0xe731, 
		0x0000, 
		0x4700, 
		0xd3a0, 
		0xd340, 
		0x4e00, 
		0xd240, 
		0xc700, 
		0x8000, 
		0x5360, 
		0x424b, 
		0xdb75, 
		0x5e42, 
		0x4fc0, 
		0xc900, 
		0x5260, 
		0x5220, 
		0xd000, 
		0xcf80, 
		0xcc00, 
		0x4e80, 
		0xd1c0, 
		0xad38, 
		0x4f00, 
		0xfc00, 
		0x0b2b, 
		0xcec0, 
		0x5320, 
		0x51c0, 
		0x3944, 
		0xcc00, 
		0x5280, 
		0xcc00, 
		0x82f1, 
		0xcc40, 
		0xf6b0, 
		0xcfc0, 
		0x50a0, 
		0xcf40, 
		0xd532, 
		0xca80, 
		0x8241, 
		0x3311, 
		0x5020, 
		0xcb80, 
		0x4f00, 
		0x50c0, 
		0xd000, 
		0x4700, 
		0x0000, 
		0x6176, 
		0xdeba, 
		0x1703, 
		0x6645, 
		0xc400, 
		0x59f4, 
		0xd280, 
		0xcdc0, 
		0x4a00, 
		0x8000, 
		0xc400, 
		0x31bb, 
		0x0000, 
		0x50c0, 
		0xb6d3, 
		0x4c80, 
		0xc980, 
		0x7c00, 
		0x9728, 
		0xcc40, 
		0x0479, 
		0xcc80, 
		0xcfc0, 
		0x5020, 
		0x53c0, 
		0xce80, 
		0x8000, 
		0x4ec0, 
		0xeb84, 
		0xd1a0, 
		0xce80, 
		0x4200, 
		0xd2c0, 
		0xfc00, 
		0xc700, 
		0xcb00, 
		0x69c3, 
		0x4d80, 
		0xd2c0, 
		0xd2d4, 
		0x4ec0, 
		0x303d, 
		0x53e0, 
		0x0000, 
		0x7c62, 
		0x4500, 
		0x8f2d, 
		0x52a0, 
		0xd2a0, 
		0xd89d, 
		0x6528, 
		0xc800, 
		0x51a0, 
		0x51c0, 
		0x4fc0, 
		0x50c0, 
		0x3c00, 
		0x51c0, 
		0x52a0, 
		0x90a7, 
		0x5380, 
		0x7c00, 
		0xc600, 
		0x6d90, 
		0xbc00, 
		0x5360, 
		0xc400, 
		0x7d24, 
		0x50a0, 
		0x5240, 
		0xccc0, 
		0xcc00, 
		0x5080, 
		0xdd7d, 
		0x4637, 
		0xd220, 
		0x2224, 
		0xc600, 
		0xc000, 
		0x456e, 
		0xc500, 
		0x3c00, 
		0xd0e0, 
		0xcf00, 
		0xcf0d, 
		0x0000, 
		0x5360, 
		0xf9ca, 
		0xfdf6, 
		0x0000, 
		0xcf40, 
		0x653f, 
		0x4d80, 
		0x7c00, 
		0xef30, 
		0x3024, 
		0x60a8, 
		0xccc0, 
		0xd100, 
		0xd280, 
		0xd007, 
		0x8000, 
		0xd220, 
		0xb9fb, 
		0x0000, 
		0xd855, 
		0x5000, 
		0xce40, 
		0x5200, 
		0xd300, 
		0xc008, 
		0x5280, 
		0x0878, 
		0x51c0, 
		0x97c6, 
		0x42a2, 
		0x7c00, 
		0x53c0, 
		0x0000, 
		0x0000, 
		0x5220, 
		0x4f00, 
		0xcc00, 
		0xd000, 
		0xd040, 
		0xcf40, 
		0x5180, 
		0x4c80, 
		0x4d00, 
		0x0000, 
		0xd220, 
		0x5c30, 
		0xd1a0, 
		0xcb00, 
		0xca00, 
		0x4f40, 
		0x4dc0, 
		0x5320, 
		0x52e0, 
		0x4880, 
		0x7c00, 
		0x4d40, 
		0xb646, 
		0x7c00, 
		0x8000, 
		0x50e0, 
		0xd3e0, 
		0x0000, 
		0xd300, 
		0x52c0, 
		0xd888, 
		0x5220, 
		0x2524, 
		0x50a0, 
		0xd020, 
		0x0284, 
		0xd1e0, 
		0xd040, 
		0x4b00, 
		0xce6c, 
		0x5515, 
		0xc242, 
		0x4c00, 
		0x8000, 
		0x518f, 
		0x5040, 
		0xcd80, 
		0x5020, 
		0x51c0, 
		0xb63b, 
		0x06c1, 
		0xcc40, 
		0xccc0, 
		0x5380, 
		0x0000, 
		0xcf00, 
		0xd0e0, 
		0xfab1, 
		0x5040, 
		0xc800, 
		0x4f80, 
		0xcf40, 
		0x1678, 
		0x3b61, 
		0xc600, 
		0xb065, 
		0x4c40, 
		0xcdc0, 
		0xce00, 
		0xc700, 
		0xd1c0, 
		0xd320, 
		0x4700, 
		0x8000, 
		0x4a00, 
		0xca00, 
		0xd1c0, 
		0x4d00, 
		0x52ce, 
		0xd2c0, 
		0x51e0, 
		0xf038, 
		0xfd26, 
		0xd080, 
		0xcc80, 
		0x5360, 
		0x3c00, 
		0x5380, 
		0x53a0, 
		0x7c00, 
		0x326e, 
		0xce00, 
		0xd3e0, 
		0x5206, 
		0xd0e0, 
		0x5240, 
		0xf16e, 
		0x4b80, 
		0xce80, 
		0xc500, 
		0xd380, 
		0xcdb2, 
		0xcb00, 
		0xd040, 
		0x5300, 
		0xb78e, 
		0xcf80, 
		0x8000, 
		0x53a0, 
		0x4c00, 
		0x4c40, 
		0xd240, 
		0x0000, 
		0x5140, 
		0x4dc0, 
		0x8000, 
		0x5140, 
		0x9237, 
		0xd3a0, 
		0xf7af, 
		0x41fb, 
		0x50e0, 
		0x4e80, 
		0x8000, 
		0xd340, 
		0xcc00, 
		0x5120, 
		0xcf40, 
		0xd280, 
		0xd120, 
		0x20b0, 
		0xfc00, 
		0xca80, 
		0x59af, 
		0x920a, 
		0x4f40, 
		0x4fc0, 
		0x53e0, 
		0xb04f, 
		0x4f00, 
		0x5e06, 
		0xfc00, 
		0x4f80, 
		0x9297, 
		0x51a0, 
		0x51a0, 
		0x36ba, 
		0x8000, 
		0x5100, 
		0x03b0, 
		0xca00, 
		0xcc80, 
		0xce80, 
		0xd3c0, 
		0x7c00, 
		0x9383, 
		0x51a0, 
		0x79b3, 
		0x50e0, 
		0x5648, 
		0x847b, 
		0x4880, 
		0xd0c0, 
		0x7dd1, 
		0xca80, 
		0xc5b0, 
		0x51a0, 
		0xd3a0, 
		0x51c0, 
		0x6a85, 
		0x4d80, 
		0x00b8, 
		0xcb80, 
		0x50c0, 
		0xc900, 
		0xd260, 
		0xd360, 
		0xd040, 
		0xccc0, 
		0x5380, 
		0xd080, 
		0xcd80, 
		0x4d00, 
		0xfc00, 
		0xff46, 
		0xd000, 
		0xd1e0, 
		0xd380, 
		0x15dd, 
		0xc880, 
		0xf157, 
		0x8000, 
		0xfc60, 
		0x4800, 
		0x5040, 
		0xc69e, 
		0x52c0, 
		0x4500, 
		0x4700, 
		0x50cb, 
		0xfc04, 
		0xd160, 
		0x6ea5, 
		0x4d80, 
		0x7c00, 
		0xb8ed, 
		0xd2a0, 
		0x5380, 
		0xd2a0, 
		0x4980, 
		0x5080, 
		0xfdc0, 
		0x4200, 
		0xd080, 
		0xd0c0, 
		0xbc00, 
		0x0a1b, 
		0xc880, 
		0xd2e0, 
		0xc700, 
		0xd260, 
		0xcd80, 
		0xd180, 
		0x4200, 
		0x5100, 
		0x51e0, 
		0x50e0, 
		0x2cb5, 
		0x4a80, 
		0x3c00, 
		0xd040, 
		0x1cc2, 
		0x5100, 
		0xd060, 
		0xcfc0, 
		0x4500, 
		0x53c0, 
		0x4980, 
		0x4a00, 
		0xd300, 
		0xd2c0, 
		0xca80, 
		0xd360, 
		0x0369, 
		0x4e40, 
		0x4b80, 
		0x50c0, 
		0x4880, 
		0x75aa, 
		0x0000, 
		0x5080, 
		0xd100, 
		0x5260, 
		0xca80, 
		0x4880, 
		0x4fc0, 
		0xcf80, 
		0xc041, 
		0x8000, 
		0x5040, 
		0xb012, 
		0xcdc0, 
		0x5140, 
		0xa67e, 
		0x8000, 
		0xd040, 
		0x4d00, 
		0x4f80, 
		0xd0e0, 
		0x0000, 
		0x8000, 
		0xc500, 
		0x119c, 
		0xfd61, 
		0xd020, 
		0xbc00, 
		0xd0c0, 
		0xcb97, 
		0x7c00, 
		0x7c66, 
		0x4c00, 
		0x8000, 
		0xf704, 
		0x6f89, 
		0x8000, 
		0x4f00, 
		0xbad7, 
		0xcf80, 
		0xca00, 
		0x7c00, 
		0x5300, 
		0x9252, 
		0xd300, 
		0x4b80, 
		0x4c00, 
		0xc700, 
		0x7c00, 
		0x5140, 
		0x5280, 
		0xd13f, 
		0xd300, 
		0x7c00, 
		0x7a93, 
		0xd414, 
		0x4980, 
		0xd3e0, 
		0x0000, 
		0x4fc0, 
		0xd0a0, 
		0xd260, 
		0xc700, 
		0x5300, 
		0xd3a0, 
		0x67a3, 
		0x9508, 
		0x4d80, 
		0xd000, 
		0xd340, 
		0xd2e0, 
		0xf6ee, 
		0x0000, 
		0x4700, 
		0x72b4, 
		0xcd80, 
		0xce40, 
		0x51e0, 
		0xcc80, 
		0xd040, 
		0xcf40, 
		0x95cf, 
		0xce00, 
		0x37ae, 
		0xd160, 
		0x4c40, 
		0xd380, 
		0xd120, 
		0x613b, 
		0xae8f, 
		0x5180, 
		0x0000, 
		0xfb41, 
		0x5340, 
		0x0000, 
		0x8695, 
		0xcc00, 
		0x6667, 
		0x5360, 
		0x7968, 
		0xc880, 
		0x5100, 
		0x4800, 
		0x82fb, 
		0xd000, 
		0x4980, 
		0xfc00, 
		0xd0e0, 
		0x5160, 
		0xac0f, 
		0xadf5, 
		0xb966, 
		0xd340, 
		0x8963, 
		0x4fc0, 
		0x034e, 
		0x9589, 
		0xc000, 
		0x4700, 
		0x5220, 
		0xcfc0, 
		0x8000, 
		0x4ec0, 
		0x50c0, 
		0xcb00, 
		0x5100, 
		0x7c00, 
		0x4980, 
		0x8000, 
		0xc000, 
		0x6f2d, 
		0x4700, 
		0xfc00, 
		0x8000, 
		0x79df, 
		0xdda9, 
		0xcf40, 
		0xf127, 
		0xe596, 
		0x4e80, 
		0xd2c0, 
		0x4a00, 
		0xd020, 
		0x1ba1, 
		0xcec0, 
		0xa4ef, 
		0x4f00, 
		0x4c80, 
		0x4d00, 
		0xcbcf, 
		0x50e0, 
		0xc500, 
		0x4a80, 
		0x4200, 
		0xcb00, 
		0x7c00, 
		0xce80, 
		0x4f40, 
		0xce00, 
		0x1604, 
		0x0b48, 
		0xd1a0, 
		0x4800, 
		0x50a0, 
		0x0000, 
		0xd200, 
		0xd1e0, 
		0x4500, 
		0xd340, 
		0x4f80, 
		0x4932, 
		0x4c00, 
		0x4a00, 
		0xa24f, 
		0xd140, 
		0x8000, 
		0xfedc, 
		0xd340, 
		0xce40, 
		0x4367, 
		0x4f00, 
		0x81ab, 
		0xc400, 
		0x4600, 
		0x9841, 
		0x2082, 
		0x4900, 
		0xd0e0, 
		0xd40f, 
		0x5080, 
		0x4200, 
		0x3c94, 
		0x53c0, 
		0x0000, 
		0x6eec, 
		0x5100, 
		0x4e40, 
		0x5300, 
		0x9529, 
		0x52e0, 
		0x2a78, 
		0x0000, 
		0x7c00, 
		0x4900, 
		0xe058, 
		0x5340, 
		0x4e7d, 
		0xd140, 
		0xce00, 
		0xcf40, 
		0x4c40, 
		0xc000, 
		0xcd00, 
		0x4a80, 
		0x4a80, 
		0xcb80, 
		0xd1e0, 
		0xc700, 
		0x84cd, 
		0x52e0, 
		0xd1a0, 
		0x1830, 
		0xc800, 
		0x5280, 
		0x9c11, 
		0x4200, 
		0xd100, 
		0x4cc0, 
		0x8000, 
		0x50ce, 
		0xfc00, 
		0xfc00, 
		0xf634, 
		0x4d40, 
		0x8000, 
		0x8000, 
		0x2045, 
		0x4e00, 
		0x9d75, 
		0xd160, 
		0x4e78, 
		0x4a00, 
		0xcb00, 
		0x70ab, 
		0x3079, 
		0x4c40, 
		0x4b26, 
		0xcdc3, 
		0xd240, 
		0xd340, 
		0x4000, 
		0x9bd4, 
		0x02bb, 
		0xd140, 
		0x8000, 
		0xcd40, 
		0x4d80, 
		0xd091, 
		0xbc00, 
		0xc400, 
		0x7c00, 
		0xcf80, 
		0xc980, 
		0xcb80, 
		0xc700, 
		0xb30e, 
		0x53e0, 
		0x7d6d, 
		0xd3c0, 
		0xfc00, 
		0x8c7b, 
		0x5100, 
		0xd35a, 
		0xfcac, 
		0x39ec, 
		0x467c, 
		0x50c0, 
		0x4200, 
		0x4d80, 
		0x285b, 
		0x7ba2, 
		0x4880, 
		0x31c4, 
		0xd200, 
		0xea43, 
		0xd2bc, 
		0x5160, 
		0x7c00, 
		0xa9fa, 
		0x5300, 
		0x4e80, 
		0x4f40, 
		0x5180, 
		0xd72f, 
		0x4f80, 
		0xfd72, 
		0x52e0, 
		0xd371, 
		0xb59b, 
		0xc900, 
		0xc200, 
		0x60c7, 
		0x51a0, 
		0x3c00, 
		0x8000, 
		0xd0a0, 
		0xd2e0, 
		0x994f, 
		0xce00, 
		0x5240, 
		0x5380, 
		0x5a66, 
		0x5380, 
		0x51c0, 
		0xc0e6, 
		0x4d00, 
		0xcc3c, 
		0xcb80, 
		0x4700, 
		0x5140, 
		0x06c2, 
		0x23fd, 
		0x2457, 
		0x0c2d, 
		0x0000, 
		0xd0c0, 
		0xbeb8, 
		0xdf01, 
		0xd0a0, 
		0x7084, 
		0xbc00, 
		0x4dc0, 
		0xfc00, 
		0xd320, 
		0x52e0, 
		0xc3ef, 
		0x5020, 
		0x5260, 
		0x8e1a, 
		0xd1a0, 
		0x4f00, 
		0x2a0d, 
		0x5340, 
		0xa148, 
		0xfc00, 
		0xfc15, 
		0x426c, 
		0xd020, 
		0x52c0, 
		0x164b, 
		0xf083, 
		0x0000, 
		0xe6f0, 
		0x0dce, 
		0x4000, 
		0x7c00, 
		0xc000, 
		0xd0e0, 
		0x0f15, 
		0x5080, 
		0xfc00, 
		0xcc80, 
		0xfc00, 
		0xd180, 
		0x6f75, 
		0x7c48, 
		0x49bf, 
		0x9937, 
		0x0000, 
		0x5360, 
		0xbc00, 
		0x4800, 
		0xc900, 
		0x8000, 
		0xcdc0, 
		0xc200, 
		0xed13, 
		0x4f40, 
		0xfc00, 
		0x5060, 
		0xd100, 
		0xcd00, 
		0xc500, 
		0x6aba, 
		0xd0c0, 
		0xcd00, 
		0x4800, 
		0x5180, 
		0x7c00, 
		0x80e7, 
		0xbd07, 
		0x8000, 
		0x452c, 
		0x7c00, 
		0x7c00, 
		0x5360, 
		0x4e80, 
		0x7c00, 
		0xfc00, 
		0x0969, 
		0x1c7a, 
		0x4200, 
		0xfc00, 
		0x594f, 
		0xce00, 
		0xd000, 
		0x599c, 
		0x4980, 
		0x357a, 
		0x4fc0, 
		0x50c0, 
		0x4fc0, 
		0x4d00, 
		0xcd40, 
		0x50a0, 
		0xbbb2, 
		0xd220, 
		0xcec0, 
		0xb8ad, 
		0x3245, 
		0x5260, 
		0x0f2f, 
		0x3c00, 
		0x0000, 
		0xd300, 
		0x3a4f, 
		0x453e, 
		0x5180, 
		0x4fc0, 
		0xc500, 
		0x8000, 
		0x7c00, 
		0x0000, 
		0x50c0, 
		0x4dc0, 
		0x5080, 
		0xda1a, 
		0x0e22, 
		0xcdc0, 
		0xea10, 
		0xfc00, 
		0x5080, 
		0xce40, 
		0x5220, 
		0xd140, 
		0xcd00, 
		0xc400, 
		0x7b64, 
		0x52e0, 
		0xc06b, 
		0xd320, 
		0x852e, 
		0xad26, 
		0xd0a0, 
		0xc200, 
		0xbc02, 
		0x4700, 
		0x8000, 
		0x4dbf, 
		0x3e9f, 
		0xb8ef, 
		0x0000, 
		0xe20f, 
		0xfc00, 
		0x5240, 
		0x1916, 
		0x0f87, 
		0x8000, 
		0xc000, 
		0x5280, 
		0x5100, 
		0xd2a0, 
		0x7c00, 
		0x10bb, 
		0x99d5, 
		0x53e0, 
		0xd200, 
		0x5100, 
		0xd280, 
		0x0000, 
		0xcf80, 
		0xf87b, 
		0xfddb, 
		0x6058, 
		0x0000, 
		0xd000, 
		0x4d80, 
		0xd0e0, 
		0x62ef, 
		0x387e, 
		0xe759, 
		0xce80, 
		0x5220, 
		0x7948, 
		0xcfc0, 
		0x5200, 
		0x984f, 
		0xd1c0, 
		0xd3a0, 
		0xd2a0, 
		0xc880, 
		0xc900, 
		0xfc00, 
		0x5360, 
		0x5320, 
		0xcc80, 
		0xa5c2, 
		0x51da, 
		0x7ff7, 
		0x33aa, 
		0x0bb5, 
		0x3c19, 
		0x53a0, 
		0xcc40, 
		0x51a0, 
		0x52e0, 
		0x4ec0, 
		0xccc0, 
		0x7df6, 
		0x14e5, 
		0xb1ec, 
		0x2580, 
		0xe947, 
		0x8000, 
		0x51a0, 
		0x7861, 
		0xb149, 
		0x4fc0, 
		0x5200, 
		0x50c0, 
		0x8000, 
		0x5200, 
		0xa9e5, 
		0xccc0, 
		0x4200, 
		0x4a00, 
		0x0000, 
		0x8328, 
		0xd320, 
		0xfc00, 
		0x50a0, 
		0x55e9, 
		0x35bb, 
		0x5080, 
		0x8000, 
		0x11e6, 
		0x4f00, 
		0xca80, 
		0xb327, 
		0xc800, 
		0x6efa, 
		0x4900, 
		0xcc40, 
		0x4700, 
		0x3c94, 
		0xcf80, 
		0x1598, 
		0x7dfb, 
		0xc35e, 
		0x352a, 
		0x0000, 
		0x0e8d, 
		0x4dc0, 
		0xfa30, 
		0xcc80, 
		0x50c5, 
		0xca80, 
		0xc600, 
		0xe93e, 
		0xfd9a, 
		0x53c0, 
		0x5000, 
		0x5080, 
		0x2496, 
		0xd380, 
		0x8000, 
		0x52c0, 
		0x4dc0, 
		0xd060, 
		0xd180, 
		0x53a0, 
		0x8000, 
		0xcb80, 
		0x74cd, 
		0xe475, 
		0x4c80, 
		0xedae, 
		0x4b00, 
		0xcf00, 
		0x5000, 
		0x7c00, 
		0x7c00, 
		0xdde7, 
		0xfc23, 
		0xd320, 
		0x0a02, 
		0x55fc, 
		0x5100, 
		0x4d80, 
		0xb073, 
		0xca77, 
		0xcb80, 
		0xa741, 
		0x08ee, 
		0xd120, 
		0xe0e3, 
		0x03dd, 
		0x50a0, 
		0xcb80, 
		0xd020, 
		0xf49d, 
		0xd3a0, 
		0x82f6, 
		0xd360, 
		0x9925, 
		0xcdc0, 
		0x8859, 
		0xce00, 
		0xd120, 
		0x4b80, 
		0x8000, 
		0x4900, 
		0x02fe, 
		0x7bc4, 
		0x0000, 
		0x0b28, 
		0xc880, 
		0xcd40, 
		0x0000, 
		0xc980, 
		0xcf40, 
		0x003f, 
		0x4dc0, 
		0xd2e0, 
		0xc900, 
		0xc980, 
		0x3c00, 
		0x0000, 
		0x588d, 
		0x5300, 
		0x4900, 
		0x5408, 
		0xd240, 
		0x2215, 
		0x4700, 
		0xd060, 
		0x8e20, 
		0x0121, 
		0xc400, 
		0xe567, 
		0x08d9, 
		0x4e00, 
		0x52a0, 
		0x1e54, 
		0xd160, 
		0x5320, 
		0x1d03, 
		0x50a0, 
		0x5160, 
		0x4e40, 
		0xc900, 
		0x4500, 
		0x4f80, 
		0xcdc0, 
		0xd040, 
		0xce00, 
		0x4c40, 
		0x4e00, 
		0x5100, 
		0xfc00, 
		0xd160, 
		0x4000, 
		0x4200, 
		0x4dc0, 
		0x5120, 
		0x5120, 
		0xcb80, 
		0x3c00, 
		0x88b4, 
		0x3d44, 
		0xd020, 
		0x4a00, 
		0xcf00, 
		0xcfc0, 
		0x4432, 
		0xc600, 
		0x53c0, 
		0xd1e0, 
		0xcc40, 
		0x505e, 
		0xd260, 
		0xcd40, 
		0x2f17, 
		0xfdf4, 
		0xc400, 
		0x8000, 
		0x6149, 
		0x8000, 
		0x8000, 
		0x6718, 
		0xd260, 
		0x5280, 
		0xd380, 
		0xe333, 
		0x5300, 
		0xce40, 
		0x7c00, 
		0xc600, 
		0x51a0, 
		0x76ad, 
		0x53e0, 
		0xcf40, 
		0xed78, 
		0x5d3f, 
		0x4cc0, 
		0x4980, 
		0xd080, 
		0xcf00, 
		0x92e5, 
		0xcec0, 
		0x5360, 
		0x1c8f, 
		0x4600, 
		0xccc0, 
		0xe3dd, 
		0xcf40, 
		0x4700, 
		0x10fa, 
		0x4500, 
		0x8e8a, 
		0x52e0, 
		0x5300, 
		0x4900, 
		0xc4fe, 
		0x5020, 
		0x0000, 
		0xd0c0, 
		0xd100, 
		0x4900, 
		0xcf00, 
		0xbc00, 
		0x8239, 
		0xcf00, 
		0xcdc0, 
		0xa146, 
		0xc800, 
		0xcb80, 
		0x4beb, 
		0xd160, 
		0xc700, 
		0x0fa2, 
		0xd187, 
		0xd3a0, 
		0xd320, 
		0xac41, 
		0xca80, 
		0x8cd3, 
		0x017b, 
		0xd2a0, 
		0x1732, 
		0x4f80, 
		0x5d71, 
		0xd3e0, 
		0xc227, 
		0xa75d, 
		0xd2c0, 
		0xd1c0, 
		0x138d, 
		0x7c00, 
		0x0732, 
		0x0000, 
		0x4b80, 
		0x5180, 
		0x4850, 
		0x4b80, 
		0xd340, 
		0xd3e0, 
		0x74fb, 
		0xd000, 
		0x5320, 
		0xe23e, 
		0xd280, 
		0xd3a0, 
		0x8000, 
		0x21cb, 
		0xf9ae, 
		0x7857, 
		0x4980, 
		0x3c00, 
		0x5100, 
		0x56a2, 
		0xd0c0, 
		0x0000, 
		0xd1e0, 
		0xc700, 
		0x6334, 
		0x8000, 
		0xd3c0, 
		0xd320, 
		0x7346, 
		0x5980, 
		0xc356, 
		0x0000, 
		0xce80, 
		0xfdaa, 
		0x5300, 
		0x2e79, 
		0x0000, 
		0x50a0, 
		0x8000, 
		0xca00, 
		0x433f, 
		0x4400, 
		0x4d00, 
		0x4f40, 
		0xd120, 
		0x4000, 
		0x4000, 
		0x5200, 
		0x5220, 
		0x5ef8, 
		0xda93, 
		0x4a00, 
		0xcc00, 
		0xcd80, 
		0x5020, 
		0xc5e6, 
		0xce00, 
		0x4e00, 
		0x4c40, 
		0x5040, 
		0xd340, 
		0xcb00, 
		0xe190, 
		0x7db7, 
		0xa641, 
		0xf722, 
		0xd1c0, 
		0xbc00, 
		0x4c00, 
		0x51e0, 
		0x4900, 
		0x65ec, 
		0xd3c0, 
		0xc500, 
		0x4d40, 
		0x4d80, 
		0xfc00, 
		0x88d1, 
		0xfd5a, 
		0xc48c, 
		0xca00, 
		0x3a57, 
		0x5020, 
		0xd1e0, 
		0x7d22, 
		0x53e0, 
		0xc700, 
		0x857c, 
		0xfd60, 
		0x52a0, 
		0x8000, 
		0xfc00, 
		0x4880, 
		0x5120, 
		0xc700, 
		0xd3c0, 
		0xfb13, 
		0xcafb, 
		0x52a0, 
		0xce40, 
		0xfdb0, 
		0xd240, 
		0x53a0, 
		0x5220, 
		0xcf00, 
		0x0221, 
		0x9a35, 
		0xd0e0, 
		0x4c80, 
		0x5340, 
		0x3597, 
		0xd0a0, 
		0x000a, 
		0x0000, 
		0x7c00, 
		0xa8b8, 
		0xcb00, 
		0x67c2, 
		0xd517, 
		0x53c0, 
		0x4880, 
		0x4600, 
		0x033e, 
		0xc7e3, 
		0x4cc0, 
		0x5000, 
		0xfd60, 
		0x4c00, 
		0xa29e, 
		0x4a80, 
		0x0000, 
		0xedc2, 
		0x0000, 
		0x896a, 
		0x4d80, 
		0xcf80, 
		0x05e6, 
		0x4600, 
		0x5180, 
		0x3c6d, 
		0xccc0, 
		0x5200, 
		0xd140, 
		0xd2a0, 
		0x4d80, 
		0xcd40, 
		0x0dba, 
		0x4c00, 
		0x06ab, 
		0x51e0, 
		0xf186, 
		0xc000, 
		0xd1e0, 
		0x6770, 
		0xd040, 
		0x5280, 
		0xcec0, 
		0x7c00, 
		0xd280, 
		0xd1a0, 
		0xd260, 
		0x4f80, 
		0x7d14, 
		0xc869, 
		0xd000, 
		0x9dda, 
		0xcf40, 
		0xd280, 
		0x5040, 
		0xa990, 
		0x0000, 
		0xca80  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc2600000, 
		0x0e01ffff, 
		0x00ffffff, 
		0x0e000001, 
		0x42580000, 
		0x00200000, 
		0x420c0000, 
		0x80800003, 
		0x42300000, 
		0xc2400000, 
		0xffa52d01, 
		0xbd481de7, 
		0x40e00000, 
		0xe0a09ff2, 
		0x70ea1eec, 
		0x00800001, 
		0x00000200, 
		0xc9622a5e, 
		0x8d267612, 
		0x00000000, 
		0x248f653b, 
		0xc0400000, 
		0x80000080, 
		0xc20c0000, 
		0xc8d83a4a, 
		0x82a2945f, 
		0x80000200, 
		0x80000000, 
		0x41500000, 
		0x296d71e7, 
		0xc2140000, 
		0xa73d8612, 
		0x00000000, 
		0xffc00000, 
		0xc2410e5d, 
		0x80008000, 
		0xff930937, 
		0x42100000, 
		0xc2140000, 
		0x006fe095, 
		0xbf800000, 
		0xff800000, 
		0x0c7c0000, 
		0xff825678, 
		0x80000000, 
		0x0e003fff, 
		0x00000000, 
		0xf6e7cd24, 
		0x0c780000, 
		0xc2300000, 
		0x0c7fffe0, 
		0xc1d80000, 
		0xffffffff, 
		0xc1f00000, 
		0x80000000, 
		0xc23c0000, 
		0x42440000, 
		0xc1f00000, 
		0x33333333, 
		0x5c1f701a, 
		0xb4f162cc, 
		0xc2600000, 
		0x296a1a22, 
		0xb9cfdd66, 
		0xbf800001, 
		0x00800003, 
		0x80000000, 
		0x007ffffe, 
		0xbf800001, 
		0xc1700000, 
		0x007fffff, 
		0x00000002, 
		0x3f800001, 
		0xe6d3d6d4, 
		0xb12216a0, 
		0x41980000, 
		0xe8b64e42, 
		0x41e00000, 
		0x80000000, 
		0x41e80000, 
		0x00800001, 
		0x7fc00001, 
		0xd710b682, 
		0x7f800000, 
		0x00011111, 
		0xfcb4b171, 
		0x22744845, 
		0xb5dc3224, 
		0x0e007fff, 
		0xc1100000, 
		0x80800001, 
		0x0c7e0000, 
		0x00000000, 
		0x0c7e0000, 
		0x7f800000, 
		0x80000000, 
		0x43347b24, 
		0xeb5e4a2c, 
		0x0f7ffffd, 
		0xdb13eeee, 
		0x41600000, 
		0x1516abc1, 
		0x7fffffff, 
		0xc1b80000, 
		0x00002000, 
		0x00000000, 
		0xc2180000, 
		0x08030581, 
		0xbf028f5c, 
		0x0e001fff, 
		0x41b80000, 
		0x0e00007f, 
		0x00004000, 
		0x8f7ffffc, 
		0x41100000, 
		0x15ef7d3f, 
		0x0ff1a709, 
		0xff800001, 
		0x73a41357, 
		0x7807e9fb, 
		0x00400000, 
		0x0e000fff, 
		0x1bc4c0ad, 
		0xe43b3306, 
		0x807ffffe, 
		0xc27c0000, 
		0x0e00003f, 
		0x80000001, 
		0xbcc819e0, 
		0x0d00fff0, 
		0xe0da5a08, 
		0xc1c80000, 
		0x0b2a9309, 
		0x7f7fffff, 
		0x0e7fffff, 
		0x41900000, 
		0x80001000, 
		0xc2740000, 
		0x8f7fffff, 
		0x7fbfffff, 
		0x41a80000, 
		0xffb7a525, 
		0x0e00003f, 
		0xc0800000, 
		0xff7fffff, 
		0x450613f6, 
		0x80000000, 
		0x80000000, 
		0x807ffffe, 
		0x7f800001, 
		0x80000000, 
		0x2690a56b, 
		0x0e0001ff, 
		0x887c95ae, 
		0x42080000, 
		0x7f000000, 
		0x42540000, 
		0x7f7ffffe, 
		0x7fbfffff, 
		0x007fffff, 
		0x80000020, 
		0x00000000, 
		0xc1400000, 
		0x80000000, 
		0xc0e181ae, 
		0x99155f93, 
		0x80800001, 
		0x0c7ffffc, 
		0x80000000, 
		0x00000400, 
		0xff800000, 
		0x80000800, 
		0xc82da809, 
		0x562e8eb5, 
		0x7fbfffff, 
		0x7fc00000, 
		0x0c7fc000, 
		0x0e7fffff, 
		0xfdd43199, 
		0xc0800000, 
		0x80000000, 
		0x42580000, 
		0x7fc00001, 
		0xff7fffff, 
		0x7f7fffff, 
		0x00800001, 
		0xc6cd72c8, 
		0x2420a57b, 
		0x0933d19a, 
		0xc1f00000, 
		0x41100000, 
		0x0e000007, 
		0x00000200, 
		0xff7ffffe, 
		0xc1c80000, 
		0x00000000, 
		0x7f8da5cb, 
		0x00000000, 
		0x08b13413, 
		0x8e0acf99, 
		0x0f7ffffe, 
		0x00000020, 
		0xc1300000, 
		0x41b80000, 
		0x41b80000, 
		0xff7ffffe, 
		0xff800000, 
		0x003f8146, 
		0x80569c34, 
		0x0e00007f, 
		0x4f5ee617, 
		0x0e00001f, 
		0xd3779ce4, 
		0xd5c86121, 
		0x0c600000, 
		0x007ffffe, 
		0x80800001, 
		0xc1c00000, 
		0x80001000, 
		0x0e000003, 
		0x0e00ffff, 
		0x80000080, 
		0x8e37bb4f, 
		0x41700000, 
		0x00000040, 
		0x7bd95e74, 
		0x806e420f, 
		0x42180000, 
		0x5b1d8818, 
		0xc21c0000, 
		0xdc06c0d8, 
		0x383757a0, 
		0xc26c0000, 
		0x00000004, 
		0x80002000, 
		0x423c0000, 
		0xc2140000, 
		0x0c7ff000, 
		0xc1e00000, 
		0xc1700000, 
		0x80000000, 
		0x00010000, 
		0x42400000, 
		0x7f7ffffe, 
		0x8f7ffffc, 
		0x7f7ffffe, 
		0x80000002, 
		0xff7ffffe, 
		0x80000000, 
		0x0c7fffc0, 
		0x41980000, 
		0xc1e00000, 
		0xc1c80000, 
		0x80000800, 
		0xc2140000, 
		0x420c0000, 
		0x00000000, 
		0x00400000, 
		0x9efe10f8, 
		0x59227d90, 
		0x41200000, 
		0x41200000, 
		0x00000080, 
		0x00000000, 
		0x4e2fea39, 
		0x7fbe1ab9, 
		0xd390816c, 
		0x00000000, 
		0x0e883417, 
		0x80000000, 
		0x0d00fff0, 
		0x00000020, 
		0x80000002, 
		0x0e00003f, 
		0x80800002, 
		0x80001000, 
		0x7f7ffffe, 
		0x42600000, 
		0x41500000, 
		0x80059ba3, 
		0x67577392, 
		0x02e731aa, 
		0xe1abbdee, 
		0x00800000, 
		0xf1d8b90b, 
		0x80800000, 
		0x523639ab, 
		0x0c7ffffe, 
		0x64ea96cf, 
		0xff7fffff, 
		0x72224823, 
		0xc1b00000, 
		0xff800000, 
		0x00020000, 
		0x2ea934ca, 
		0x00020000, 
		0x8f7fffff, 
		0x0c600000, 
		0xc2240000, 
		0x4b000000, 
		0x8a5021f7, 
		0x8f7ffffd, 
		0x00020000, 
		0x41200000, 
		0xb50cb41b, 
		0x00000020, 
		0x0e1fffff, 
		0xc1000000, 
		0x40800000, 
		0x91a18d89, 
		0x00000001, 
		0x7fc00000, 
		0x24056081, 
		0x9508dc3d, 
		0x80123c54, 
		0x42680000, 
		0x0c7ffff0, 
		0x80000000, 
		0x25332f1a, 
		0xdac51bc2, 
		0x16a31e6a, 
		0xbf907434, 
		0xc2040000, 
		0x0c7fffc0, 
		0x3fa50005, 
		0xc0e00000, 
		0x80000200, 
		0x4b000000, 
		0x00200000, 
		0xffdd5283, 
		0xc0c00000, 
		0x80000400, 
		0x0e000003, 
		0xc1800000, 
		0xc1c80000, 
		0x80000000, 
		0x426c0000, 
		0x7fc00001, 
		0xc1d80000, 
		0x0d00fff0, 
		0x00800002, 
		0x1807e7f0, 
		0x2ceb152d, 
		0xc2080000, 
		0x0d00fff0, 
		0x80000000, 
		0xc2780000, 
		0x80000001, 
		0x0aaf1009, 
		0xff7ffffe, 
		0x42680000, 
		0xff800000, 
		0x80000000, 
		0xc1769889, 
		0x00000000, 
		0xffffffff, 
		0x00000200, 
		0xc1e80000, 
		0x80800003, 
		0x80800002, 
		0x80000000, 
		0x7f7fffff, 
		0xff800000, 
		0x7f7ffffe, 
		0x80000000, 
		0x42000000, 
		0xc1500000, 
		0x00000000, 
		0x3541d6f9, 
		0xc2780000, 
		0xc292d33b, 
		0x00080000, 
		0x7f7ffffe, 
		0xd41d37c3, 
		0x7f7fffff, 
		0x9a3511de, 
		0x423c0000, 
		0xc2180000, 
		0x80000001, 
		0x00020000, 
		0xc8906327, 
		0x5f73dbe9, 
		0x42740000, 
		0x1400024c, 
		0x0e000003, 
		0x42180000, 
		0xcc456256, 
		0x00000001, 
		0x00000002, 
		0x00000000, 
		0xcccccccc, 
		0x0f7ffffd, 
		0xb3621e8e, 
		0x75d1a31d, 
		0x80800000, 
		0x00000000, 
		0x2a915c86, 
		0xd62892b0, 
		0x32030ca9, 
		0x0c7fff00, 
		0x80800001, 
		0x0c7fffc0, 
		0x42140000, 
		0xc0400000, 
		0x37e7997f, 
		0x41100000, 
		0x23244c81, 
		0x80000000, 
		0xc5ef4e85, 
		0x3cd78442, 
		0x4845298f, 
		0x80519678, 
		0x93cd07ca, 
		0x00800000, 
		0x00011111, 
		0xc22c0115, 
		0x00000000, 
		0x00000000, 
		0x422c0000, 
		0xc1700000, 
		0x421c0000, 
		0x0c7fff00, 
		0xf031fbbe, 
		0xc0800000, 
		0xc3c9f42c, 
		0x25fa3cd9, 
		0xa09ecfaf, 
		0x80000000, 
		0x72ff5853, 
		0x42400000, 
		0x3d30a185, 
		0x0c600000, 
		0x422c0000, 
		0x8f7ffffd, 
		0x53cd74a2, 
		0x427a37c1, 
		0xc2280000, 
		0x00080000, 
		0x2886a40a, 
		0xc1b00000, 
		0xc2440000, 
		0xc1900000, 
		0x7f800000, 
		0x80000080, 
		0xc1b00000, 
		0x427c0000, 
		0x00800002, 
		0x41a00000, 
		0xc1d00000, 
		0xc6d596d1, 
		0x42180000, 
		0x00800000, 
		0x42500000, 
		0x00000000, 
		0x7f800000, 
		0xf49bec5c, 
		0x00800003, 
		0x80001000, 
		0x42100000, 
		0x939584a3, 
		0xc1300000, 
		0x41700000, 
		0x42440000, 
		0xc2500000, 
		0x80000080, 
		0xff800000, 
		0x7fc741cb, 
		0xc21c0000, 
		0x00000000, 
		0x80000000, 
		0x00000000, 
		0xdeefd613, 
		0x00040000, 
		0x00000000, 
		0x0c400000, 
		0x0d00fff0, 
		0x41200000, 
		0x00000000, 
		0x80004000, 
		0x8e7b726c, 
		0x7fa62ffd, 
		0x69fde716, 
		0x237004bc, 
		0x41c00000, 
		0x80000002, 
		0x423c0000, 
		0x80ffffff, 
		0xeaff231a, 
		0x0e0001ff, 
		0x42100000, 
		0x0c7fc000, 
		0x0c7fff80, 
		0x00080000, 
		0x35355c69, 
		0xc1f80000, 
		0x6e4fa91a, 
		0xc2280000, 
		0x0006aa92, 
		0x80000000, 
		0x8cd622ec, 
		0xc2300000, 
		0x42680000, 
		0x7f7ffffe, 
		0xffc00001, 
		0x80000000, 
		0x80800000, 
		0x36e0a48b, 
		0x41f80000, 
		0xe0d1c25d, 
		0xffb4a928, 
		0xd30608dd, 
		0x42780000, 
		0xc1500000, 
		0x7f800000, 
		0xc0800000, 
		0xffc00001, 
		0x00800001, 
		0xb1df5f2d, 
		0xd8b9b011, 
		0xfc2cdca8, 
		0x42640000, 
		0xc1200000, 
		0x4ad5fc32, 
		0x33318536, 
		0x80000020, 
		0x9f083e8a, 
		0x80000000, 
		0xa5132f00, 
		0x41f80000, 
		0xb6feef7d, 
		0x00000000, 
		0x80800002, 
		0x00000000, 
		0xc2300000, 
		0xdf7d748f, 
		0x0e003fff, 
		0x246de7bd, 
		0x2db78e91, 
		0x0f7ffffc, 
		0xcb000000, 
		0x80001000, 
		0x0c7e0000, 
		0x42240000, 
		0x80000080, 
		0x0e0001ff, 
		0x80000000, 
		0x6b2f7a7a, 
		0x42740000, 
		0xc2540000, 
		0x42140000, 
		0x42340000, 
		0xc2040000, 
		0x80000001, 
		0x91c59297, 
		0x9d0e834e, 
		0x0c7ff000, 
		0x42480000, 
		0x989cae3b, 
		0x8f7ffffd, 
		0xce7cb3dd, 
		0x0f7ffffe, 
		0x464a13c9, 
		0xffa2d38a, 
		0xff925bbd, 
		0x0c780000, 
		0x41700000, 
		0xffffffff, 
		0xcf45a876, 
		0x80040000, 
		0xc1700000, 
		0x80000001, 
		0x41d80000, 
		0x00000000, 
		0xf7a19609, 
		0x90efbd52, 
		0xd84ded8b, 
		0x80800003, 
		0x6fa461d6, 
		0x80000800, 
		0x5feb6a19, 
		0x00000000, 
		0xe2d56fd0, 
		0x00000000, 
		0x0b3065ea, 
		0x00000000, 
		0xc5e882b1, 
		0x2c26a4be, 
		0xd1805b21, 
		0x00ffffff, 
		0x7f800000, 
		0xff9c44df, 
		0x7ee8369b, 
		0x00010000, 
		0xe02404d0, 
		0x0e01ffff, 
		0x42300000, 
		0x6abc907d, 
		0x42180000, 
		0x41a80000, 
		0xaeed2409, 
		0x00008000, 
		0xc2000000, 
		0x64fd8057, 
		0xc2500000, 
		0xbce23266, 
		0xffbfffff, 
		0x0e001fff, 
		0x221efe57, 
		0x0c7f8000, 
		0xc02edc53, 
		0x00020000, 
		0xc0000000, 
		0xc0c00000, 
		0x00000000, 
		0xc2680000, 
		0xab136f1a, 
		0x3983b84c, 
		0xc1f00000, 
		0x0c600000, 
		0x00000000, 
		0xc1f80000, 
		0xc1d00000, 
		0x80000004, 
		0x00000200, 
		0x00000800, 
		0xa5a038d2, 
		0x6ccba2c9, 
		0x00000040, 
		0x0e001fff, 
		0x80000000, 
		0xff7ffffe, 
		0xb8014248, 
		0x13da7c63, 
		0xff800000, 
		0x97e47860, 
		0xc2680000, 
		0x00000020, 
		0x00000002, 
		0x00452359, 
		0x7f800000, 
		0xff9463e0, 
		0x80008000, 
		0xafbc27ee, 
		0x80001000, 
		0xbf2a4846, 
		0x42700000, 
		0xc2140000, 
		0x41800000, 
		0x80000020, 
		0xc1500000, 
		0x0e00000f, 
		0xabee1a78, 
		0xffa6a6b8, 
		0x80000040, 
		0x40a00000, 
		0x42740000, 
		0xc409747d, 
		0x41c00000, 
		0xffab8650, 
		0xc0400000, 
		0x42040000, 
		0x1c317251, 
		0xd35e532e, 
		0x41f00000, 
		0xc2380000, 
		0xff800000, 
		0x00080000, 
		0x80000040, 
		0x2d310fbb, 
		0xffbfffff, 
		0xe3e10fa7, 
		0x0e0000ff, 
		0x55555555, 
		0xa5cbb5fb, 
		0x7fbfffff, 
		0x4b000000, 
		0x0c7ffff0, 
		0x42080000, 
		0x7f800000, 
		0x2b5827ad, 
		0x0e0000ff, 
		0x884024eb, 
		0x00a3537a, 
		0x7f7fffff, 
		0x41000000, 
		0xaba2ddf7, 
		0x7e805be1, 
		0x41b00000, 
		0x0034c581, 
		0x00000000, 
		0x6724128d, 
		0x82e6ffe6, 
		0x00000100, 
		0x007fffff, 
		0x40a00000, 
		0x8f7ffffc, 
		0x18c63c9f, 
		0x18a20512, 
		0x007fffff, 
		0x0e000003, 
		0xfe46e0eb, 
		0x0c7f8000, 
		0xc1c00000, 
		0xc2500000, 
		0xdeb297b2, 
		0x00000004, 
		0x0c400000, 
		0x7f800000, 
		0x0f7ffffd, 
		0xabc63054, 
		0xcf487f81, 
		0x80800001, 
		0x0c600000, 
		0x00000001, 
		0xc1a80000, 
		0x00000008, 
		0x00000008, 
		0xaaaaaaaa, 
		0xff800000, 
		0x00000000, 
		0x80000000, 
		0x7fffffff, 
		0x7f7ffffe, 
		0xc1100000, 
		0x4b000000, 
		0x807fffff, 
		0x8f7ffffc, 
		0x47fdfa40, 
		0x7f7ffffe, 
		0x0e0000ff, 
		0xee3c7061, 
		0xc1e80000, 
		0x4e742dd9, 
		0x0e00001f, 
		0xff800000, 
		0x7f7ffffe, 
		0x00000000, 
		0x80000000, 
		0x00000020, 
		0xaaaaaaaa, 
		0x80800003, 
		0x0a2c3256, 
		0x0e01ffff, 
		0x41000000, 
		0x0e00003f, 
		0xff9cf750, 
		0x1b06a28b, 
		0x00000000, 
		0x0c7f0000, 
		0x925a9ae7, 
		0x70f37219, 
		0x00000000, 
		0x7f800000, 
		0x601a6d57, 
		0xc1600000, 
		0x53277551, 
		0xc2280000, 
		0x54bf23bd, 
		0x04c0bf16, 
		0x62a94959, 
		0x7f819250, 
		0xec2cddd2, 
		0x71bd401d, 
		0x80000200, 
		0x7f800000, 
		0x00000040, 
		0x0c7fc000, 
		0x00800001, 
		0x00ffffff, 
		0x1a8916cc, 
		0x5f607544, 
		0xc1200000, 
		0x007fffff, 
		0x0c7ffff0, 
		0x779526ad, 
		0x42280000, 
		0xd8ad074a, 
		0xdfb5ffae, 
		0xc1880000, 
		0x41c00000, 
		0x0c700000, 
		0xc21c0000, 
		0x56a386bf, 
		0x5887cf8b, 
		0xb6464cc5, 
		0x8f7ffffd, 
		0xff7fffff, 
		0xc25c0000, 
		0x41980000, 
		0x7c6e077f, 
		0xa9f07ebc, 
		0x0c7ffffc, 
		0x421c0000, 
		0x0c7ffc00, 
		0xd9987707, 
		0xc1d00000, 
		0x80000000, 
		0x4b000000, 
		0x7f800000, 
		0x67f5295e, 
		0xff800000, 
		0x77d55412, 
		0xc1e00000, 
		0x4fd020fb, 
		0x00ffffff, 
		0x40800000, 
		0x608f5aaf, 
		0x00000000, 
		0x35582703, 
		0xc2540000, 
		0xff9aec84, 
		0x41f80000, 
		0xe2f129c3, 
		0x00800001, 
		0x0c7fffff, 
		0xa3bc6d96, 
		0x42580000, 
		0x420c0000, 
		0x80000000, 
		0x9759d774, 
		0x81ceaec7, 
		0x7faa1143, 
		0xc1700000, 
		0x801a63d8, 
		0xe8a20bce, 
		0x57c73385, 
		0x0c7ffffc, 
		0x60d43d02, 
		0x8011aa5a, 
		0x80000002, 
		0xc1300000, 
		0x7f000000, 
		0xc1100000, 
		0x00000004, 
		0x42400000, 
		0x7f000000, 
		0xca5151f6, 
		0x42040000, 
		0x0c7ffc00, 
		0x83a66e6b, 
		0xc2280000, 
		0xc2640000, 
		0xff8eec97, 
		0x5cf4127f, 
		0x42180000, 
		0x0c7ffc00, 
		0xc2440000, 
		0x300d8975, 
		0xaf91b7ae, 
		0x41800000, 
		0x00000000, 
		0xc2440000, 
		0x00000040, 
		0x642f9fd3, 
		0x5ee16546, 
		0xc2480000, 
		0xc1100000, 
		0xffacb641, 
		0x41d80000, 
		0x0e00ffff, 
		0x80000200, 
		0x56245305, 
		0x80000002, 
		0x80000080, 
		0x7f7ffffe, 
		0x124adad4, 
		0x0e00003f, 
		0x24cb5592, 
		0xc1a80000, 
		0x3f800001, 
		0xd5bd059a, 
		0x0436331c, 
		0xff800000, 
		0x00200000, 
		0xe36b51c3, 
		0x80000004, 
		0x00000002, 
		0xc0400000, 
		0x00001000, 
		0x80008000, 
		0x42580000, 
		0x7f800000, 
		0x42500000, 
		0xff7ffffe, 
		0xce502e97, 
		0x00000000, 
		0xffa3383a, 
		0x0e00003f, 
		0x7f800000, 
		0x76f8346d, 
		0xc1300000, 
		0x0c7fffe0, 
		0x80800003, 
		0x3b0d9ccd, 
		0xe539b509, 
		0x7f92e429, 
		0xffffffff, 
		0xc2600000, 
		0x80800001, 
		0x7f7fffff, 
		0xff800000, 
		0xc2440000, 
		0x0c7f0000, 
		0x0e0007ff, 
		0xc27c0000, 
		0x00000000, 
		0xc1c00000, 
		0x0e00007f, 
		0xc1200000, 
		0x80000001, 
		0x42580000, 
		0x401311cd, 
		0x0d00fff0, 
		0x007fffff, 
		0x7f800000, 
		0x00004000, 
		0x0c700000, 
		0x00800000, 
		0xebc6eb1a, 
		0xfe2b41eb, 
		0x3f800000, 
		0x0e7fffff, 
		0x5b781c81, 
		0x80000000, 
		0xc0400000, 
		0x0c700000, 
		0x80000000, 
		0xc841114b, 
		0x80001000, 
		0x135f3067, 
		0xc0ad8755, 
		0x0c7f0000, 
		0x7f800000, 
		0xc1d00000, 
		0x7fc00001, 
		0x80000000, 
		0x0f7ffffc, 
		0xb560ad12, 
		0x00000000, 
		0xc0000000, 
		0x80ffffff, 
		0x00011111, 
		0x80800003, 
		0xa81013fd, 
		0xff9ce650, 
		0xc23c0000, 
		0xff7ffffe, 
		0x326118e7, 
		0x76acd25d, 
		0x9f8b06ce, 
		0xe2244f4e, 
		0xc23c0000, 
		0x40e00000, 
		0xff800000, 
		0xc1200000, 
		0xec145c47, 
		0x0c7fffe0, 
		0x1079ff70, 
		0x0c7ffc00, 
		0x00040000, 
		0xf7cf9be7, 
		0x9c8ef532, 
		0x0070d105, 
		0xcccccccc, 
		0xc2380000, 
		0x5530da0c, 
		0x076d49d3, 
		0x41000000, 
		0x7fffffff, 
		0x86ab8779, 
		0x80800002, 
		0x00000100, 
		0xff800000, 
		0x944002c8, 
		0x641e361c, 
		0x47dc1575, 
		0x892d7ff2, 
		0xcccccccc, 
		0x80000004, 
		0xff800000, 
		0x524dadb9, 
		0x4698bcfe, 
		0x00000010, 
		0xc2100000, 
		0x00800000, 
		0xc1a00000, 
		0x79915283, 
		0x00000040, 
		0x89bcb9d2, 
		0xc0800000, 
		0x00200000, 
		0xc0c00000, 
		0xc22c0000, 
		0x00000001, 
		0x7f800000, 
		0xc2340000, 
		0x00020000, 
		0xbf028f5c, 
		0x424c0000, 
		0xc2140000, 
		0x424c0000, 
		0x42500000, 
		0xed8b0283, 
		0x42540000, 
		0x80800001, 
		0x0e0001ff, 
		0x18b0e586, 
		0x00000000, 
		0xc2c02820, 
		0x42d5b5df, 
		0xdd2651e7, 
		0x00000040, 
		0xc1900000, 
		0x80000100, 
		0x32460933, 
		0x80000010, 
		0xc1000000, 
		0xebdcbadb, 
		0x16c687e2, 
		0x80800001, 
		0x0c400000, 
		0xc2540000, 
		0x0e007fff, 
		0xc1a80000, 
		0xfff53a02, 
		0x0c7ff800, 
		0xff800000, 
		0xc1f00000, 
		0x2403d206, 
		0x0c7fffff, 
		0xa746efcf, 
		0x807ffffe, 
		0x80000002, 
		0x342d9d01, 
		0x422c0000, 
		0x80008000, 
		0x80000400, 
		0xff7ffffe, 
		0xc2480000, 
		0x6be8d2b1, 
		0x00400000, 
		0xffffffff, 
		0x42140000, 
		0x4b0d9597, 
		0x6e3b609e, 
		0x0022640e, 
		0xc1000000, 
		0x0c7fe000, 
		0xfc69f59d, 
		0x80800001, 
		0x42300000, 
		0x42500000, 
		0x00400000, 
		0x0c7fff00, 
		0xc1400000, 
		0xc1800000, 
		0x7f800001, 
		0xa0c64093, 
		0x963aba69, 
		0x80000100, 
		0x4b8c4b40, 
		0xff7fffff, 
		0x00800002, 
		0x160f7eca, 
		0x42640000, 
		0xc0c00000, 
		0x76e2c3b4, 
		0xc03d8e77, 
		0x005a5d91, 
		0x80000000, 
		0xc2480000, 
		0x80000080, 
		0x703b8c7d, 
		0x80000000, 
		0x5080ec1c, 
		0xc2580000, 
		0x804d8cee, 
		0x332270ba, 
		0xc1900000, 
		0xc2700000, 
		0x7ed23c9a, 
		0x0c7fffc0, 
		0x00000000, 
		0xc2600000, 
		0xc1500000, 
		0xfbb14cb3, 
		0xc24c0000, 
		0xc2380000, 
		0x41800000, 
		0x00400000, 
		0x234a1a76, 
		0xd955f3ed, 
		0x3f028f5c, 
		0x80002000, 
		0x6f366958, 
		0xb423c40b, 
		0x44d410d4, 
		0x00001000, 
		0x6f6684f5, 
		0x03f1f37a, 
		0x0e003fff, 
		0x00400000, 
		0x00001000, 
		0x7fbfffff, 
		0xffa92ff9, 
		0x387debaf, 
		0x80000002, 
		0x4556c0b3, 
		0xc1c00000, 
		0x3a44f611, 
		0xc25c0000, 
		0x744802e8, 
		0x80ffffff, 
		0x00800000, 
		0x80040000, 
		0x80002000, 
		0x0c400000, 
		0x00000010, 
		0x42740000, 
		0x80000001, 
		0xdac197bd, 
		0x0e000fff, 
		0x41c80000, 
		0x4c5c659e, 
		0xa60cf6fa, 
		0x0d00fff0, 
		0x80000000, 
		0xf15b1c1c, 
		0xc2500000, 
		0x80001000, 
		0xff800000, 
		0x00000000, 
		0x0e001fff, 
		0xb7ecbd56, 
		0x42400000, 
		0x1f01c72c, 
		0x00000008, 
		0x15b883b6, 
		0x00000000, 
		0xc1980000, 
		0xb2f142ea, 
		0x424c0000, 
		0x784c75bf, 
		0x00011111, 
		0xebc5d927, 
		0x707431ed, 
		0x00800002, 
		0xc1100000, 
		0x80000000, 
		0x0c7f0000, 
		0x8e81a880, 
		0x692803c6, 
		0x7fc00000, 
		0x0f7ffffc, 
		0x00000000, 
		0x00800001, 
		0x5dea70e4, 
		0xaf41add9, 
		0x00000800, 
		0xd93e784d, 
		0x80ffffff, 
		0x42000000, 
		0x80000000, 
		0x80800002, 
		0xc1200000, 
		0x42480000, 
		0x17605f3b, 
		0x42600000, 
		0x41880000, 
		0x00000200, 
		0x00000020, 
		0xc1880000, 
		0x42780000, 
		0x36a77c88, 
		0x7f7ffffe, 
		0x9d7b6354, 
		0xc8f9283e, 
		0x0c7fc000, 
		0x41a80000, 
		0x0e00000f, 
		0x7f7ffffe, 
		0x80800001, 
		0x0e12cd3f, 
		0x7b5a21d2, 
		0x80000000, 
		0xff800000, 
		0x0c700000, 
		0x00800001, 
		0x1e1569e5, 
		0x7f7ffffe, 
		0xc1300000, 
		0xc1e80000, 
		0x42240000, 
		0xaf66c8f4, 
		0x41c80000, 
		0x57403614, 
		0x80000001, 
		0x00000000, 
		0x00000000, 
		0x89a8df02, 
		0x04e1f6f5, 
		0x80800000, 
		0x0c6d76b6, 
		0x00800000, 
		0xbf028f5c, 
		0x80080000, 
		0xc1d80000, 
		0x7f3543a4, 
		0x80000000, 
		0x80000000, 
		0x8f7ffffd, 
		0x00000000, 
		0xbc42f94c, 
		0x7f800000, 
		0x80001000, 
		0xd4dd4e25, 
		0xc2780000, 
		0xf0a09393, 
		0xc26c0000, 
		0x7fffffff, 
		0x0f7ffffe, 
		0x053988b1, 
		0x41d80000, 
		0x00000000, 
		0x5b7eefcd, 
		0x352a04d5, 
		0x80000000, 
		0x00000100, 
		0x80000000, 
		0x8f7fffff, 
		0x0c7f0000, 
		0x00800002, 
		0xc0400000, 
		0x7f800000, 
		0x00000020, 
		0x80000800, 
		0x80000000, 
		0x0c7ffe00, 
		0xc1e00000, 
		0x00000000, 
		0x0c7ff000, 
		0x0e00003f, 
		0x45c91ba6, 
		0x0e03ffff, 
		0x290f2183, 
		0x00800002, 
		0x40800000, 
		0xc0c00000, 
		0xffffffff, 
		0x0c600000, 
		0xc1a00000, 
		0x80001000, 
		0x0e000001, 
		0x36e3466f, 
		0x99fcb809, 
		0x0e000003, 
		0x42200000, 
		0x00000002, 
		0x00000000, 
		0x15580429, 
		0x86cc4f7c, 
		0xfd673593, 
		0xfa39f21f, 
		0x985fb382, 
		0x41300000, 
		0x807ffffe, 
		0x42793791, 
		0xc2040000, 
		0x1229c9a5, 
		0xc2080000, 
		0x0d00fff0, 
		0x06f8df2a, 
		0x421c0000, 
		0x0d000ff0, 
		0x80000800, 
		0x06073d33, 
		0x80000080, 
		0xff9034f5, 
		0x00000040, 
		0x80000000, 
		0xdfe11064, 
		0x80000040, 
		0x00000000, 
		0x427c0000, 
		0xb454eb8c, 
		0xc24c0000, 
		0x700e8548, 
		0x41980000, 
		0xd937d35e, 
		0x41c80000, 
		0xc1400000, 
		0x0c7ffffe, 
		0x4374954a, 
		0x42480000, 
		0xf31b4c3c, 
		0x00800003, 
		0x7f800001, 
		0xbf0d42cc, 
		0x00011111, 
		0x1c4c6a89, 
		0x007ffffe, 
		0x00800003, 
		0xc0e00000, 
		0x8d5a4a5e, 
		0xc1400000, 
		0x40400000, 
		0x0c7f0000, 
		0x07ad50de, 
		0x42140000, 
		0x42200000, 
		0x40a00000, 
		0x85fb98cb, 
		0x7fbfbc19, 
		0x0c7fffc0, 
		0x80100000, 
		0xcb8c4b40, 
		0x40000000, 
		0x7fc00001, 
		0x42480000, 
		0x42200000, 
		0x80000000, 
		0x00040000, 
		0x0e003fff, 
		0xc2680000, 
		0xfd2bf5fc, 
		0x7f800000, 
		0xc2180000, 
		0xffb2d560, 
		0x0f7ffffc, 
		0x0c400000, 
		0x8f7ffffe, 
		0xff800000, 
		0x42340000, 
		0xb319380e, 
		0x41b80000, 
		0xc4869a8b, 
		0xc1c80000, 
		0x40a00000, 
		0xc20c0000, 
		0x0e1fffff, 
		0x5619f925, 
		0x0c7ffe00, 
		0x322745be, 
		0x42540000, 
		0x42180000, 
		0xc3afb4df, 
		0xc2140000, 
		0xb2adbdec, 
		0xff800000, 
		0x425c0000, 
		0xd77e720c, 
		0x00000000, 
		0x5c54a085, 
		0x807fffff, 
		0x0031aec7, 
		0x00800000, 
		0x80000000, 
		0x0e03ffff, 
		0x3918d7e1, 
		0xc1d80000, 
		0x15d3619a, 
		0x80000000, 
		0x423c0000, 
		0x425c0000, 
		0x439b532a, 
		0x80011111, 
		0xc1300000, 
		0xc1a80000, 
		0xc1000000, 
		0xa40abfc3, 
		0x6cc0dfe0, 
		0x80000010, 
		0xf4f71084, 
		0x42000000, 
		0x7f7ffffe, 
		0xa4a9591d, 
		0x00800003, 
		0x00348e56, 
		0xc2040000, 
		0xc2200000, 
		0xc1600000, 
		0x41400000, 
		0xc2240000, 
		0xc2440000, 
		0xc2740000, 
		0xef0f030b, 
		0x80100000, 
		0xff7ffffe, 
		0x807fffff, 
		0xff800000, 
		0x46821eb9, 
		0xc1500000, 
		0x42300000, 
		0x41200000, 
		0x00000400, 
		0x0c7fc000, 
		0x00000000, 
		0x8705b7ff, 
		0x5091b4ff, 
		0x42340000, 
		0xcb8c4b40, 
		0x80800002, 
		0x00000000, 
		0x41e80000, 
		0x00800003, 
		0x420c0000, 
		0x42000000, 
		0x55555555, 
		0x00100000, 
		0x0f7ffffe, 
		0x0c7fff00, 
		0x006ede89, 
		0x80ffffff, 
		0x00001000, 
		0x00000000, 
		0xbf028f5c, 
		0xc2540000, 
		0x7f800000, 
		0xc1e00000, 
		0x00000100, 
		0x00000000, 
		0x0c7fffff, 
		0x42740000, 
		0x0c7fffc0, 
		0xc2440000, 
		0xad2940ad, 
		0x62f3fe3a, 
		0x00000400, 
		0x0c7c0000, 
		0x7fc00000, 
		0x80000002, 
		0xc797488a, 
		0xffc00001, 
		0x55555555, 
		0x026ca58d, 
		0x41c00000, 
		0x00000080, 
		0xca0b8479, 
		0x00011111, 
		0xffc00000, 
		0x99af407d, 
		0x00002000, 
		0x0e01ffff, 
		0xc0e00000, 
		0xffa7885c, 
		0x00080000, 
		0x425c0000, 
		0x80000080, 
		0x7f800000, 
		0xc20c0000, 
		0x80000000, 
		0xc1c00000, 
		0x42080000, 
		0xa9bad87c, 
		0x7f800000, 
		0xc2ab9c77, 
		0xf7095c11, 
		0x41200000, 
		0x00000000, 
		0x42240000, 
		0x80000004, 
		0x00800002, 
		0x00400000, 
		0xc0800000, 
		0x7f7ffffe, 
		0x42180000, 
		0x80800000, 
		0x42540000, 
		0xff7fffff, 
		0xcb000000, 
		0xc22c0000, 
		0x806a878b, 
		0xf9cb7697, 
		0x7f7ffffe, 
		0x00000002, 
		0x0e0007ff, 
		0xc2280000, 
		0xdce77e4e, 
		0x0c600000, 
		0x00000080, 
		0x474899c5, 
		0x5bf1c7da, 
		0x0e000007, 
		0x42580000, 
		0x00000008, 
		0x41880000, 
		0x00800000, 
		0x00800001, 
		0x41700000, 
		0x00000000, 
		0x00000200, 
		0x42400000, 
		0x1a14313a, 
		0xc2540000, 
		0x7f000000, 
		0xc1900000, 
		0xb7730e7a, 
		0x42380000, 
		0x80000080, 
		0x3f800000, 
		0x789d1ad0, 
		0x00000002, 
		0x0e000fff, 
		0xbf800001, 
		0x0e00ffff, 
		0xc1980000, 
		0xbcca4707, 
		0x7f800000, 
		0x424c0000, 
		0x2a2dd9f6, 
		0x7fc00000, 
		0x42700000, 
		0xff8b27ff, 
		0x8f7fffff, 
		0x80000000, 
		0x72491b67, 
		0x80002000, 
		0xc2640000, 
		0x80800000, 
		0x42140000, 
		0xcb8c4b40, 
		0x6fe25695, 
		0x424c0000, 
		0x80000000, 
		0xc25c0000, 
		0x00008000, 
		0x00800001, 
		0x99dfa358, 
		0xc20c0000, 
		0x42340000, 
		0x43093d22, 
		0x80000800, 
		0x7fa69750, 
		0xc41bb1d0, 
		0x80000800, 
		0xc1000000  
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
		0x00000231, 
		0x00000347, 
		0x0000008f, 
		0x00000163, 
		0x000003d3, 
		0x000003a1, 
		0x000002af, 
		0x00000145, 
		0x00000271, 
		0x000001bd, 
		0x00000019, 
		0x00000259, 
		0x000002a3, 
		0x0000019b, 
		0x000000b9, 
		0x000002e3, 
		0x00000169, 
		0x00000371, 
		0x00000087, 
		0x00000169, 
		0x0000005b, 
		0x0000004b, 
		0x000002b9, 
		0x000001d9, 
		0x000002a5, 
		0x0000035f, 
		0x000001a1, 
		0x0000017b, 
		0x00000103, 
		0x000001cf, 
		0x00000357, 
		0x0000034d, 
		0x000000f5, 
		0x000001db, 
		0x000000b5, 
		0x00000385, 
		0x00000039, 
		0x000002b1, 
		0x00000073, 
		0x00000397, 
		0x00000281, 
		0x0000029b, 
		0x0000029f, 
		0x00000009, 
		0x000001af, 
		0x00000197, 
		0x000002bb, 
		0x00000321, 
		0x000002cd, 
		0x00000119, 
		0x00000351, 
		0x000003c3, 
		0x000003eb, 
		0x0000025f, 
		0x000003d3, 
		0x00000071, 
		0x000002e9, 
		0x000003ff, 
		0x0000005b, 
		0x000003c5, 
		0x00000213, 
		0x0000013b, 
		0x000002e1, 
		0x00000219, 
		0x00000203, 
		0x000003fb, 
		0x00000289, 
		0x00000111, 
		0x000003cb, 
		0x00000055, 
		0x000001b5, 
		0x000001cd, 
		0x000003fb, 
		0x0000024b, 
		0x00000239, 
		0x0000028f, 
		0x00000273, 
		0x000000c9, 
		0x0000000f, 
		0x0000025b, 
		0x00000341, 
		0x00000317, 
		0x0000031d, 
		0x0000001f, 
		0x000000eb, 
		0x00000111, 
		0x000000bb, 
		0x0000017b, 
		0x000002e5, 
		0x0000020d, 
		0x00000301, 
		0x00000061, 
		0x000001cf, 
		0x000000f1, 
		0x000001ed, 
		0x000003ed, 
		0x000003cd, 
		0x000000f3, 
		0x000003a9, 
		0x000003e9, 
		0x0000001d, 
		0x00000287, 
		0x000001eb, 
		0x0000039d, 
		0x00000185, 
		0x00000279, 
		0x000003b7, 
		0x000001f5, 
		0x000001e9, 
		0x00000213, 
		0x00000145, 
		0x000002af, 
		0x000000ad, 
		0x00000109, 
		0x00000309, 
		0x000002a3, 
		0x000003f9, 
		0x000001a1, 
		0x0000031b, 
		0x0000022d, 
		0x00000027, 
		0x0000037f, 
		0x000001df, 
		0x00000025, 
		0x000003ed, 
		0x0000016f, 
		0x00000359, 
		0x000002a3, 
		0x000002d5, 
		0x0000030f, 
		0x00000315, 
		0x000002db, 
		0x00000205, 
		0x0000010d, 
		0x00000337, 
		0x000002c7, 
		0x000001eb, 
		0x0000002f, 
		0x00000165, 
		0x00000169, 
		0x0000024f, 
		0x000003a5, 
		0x000001a3, 
		0x00000111, 
		0x00000005, 
		0x00000129, 
		0x000003c1, 
		0x0000001b, 
		0x00000013, 
		0x00000159, 
		0x0000015b, 
		0x000003d7, 
		0x000001ad, 
		0x00000169, 
		0x00000045, 
		0x000002e1, 
		0x00000247, 
		0x00000073, 
		0x00000013, 
		0x00000173, 
		0x00000269, 
		0x000002b5, 
		0x0000030d, 
		0x000003c5, 
		0x000002cf, 
		0x00000377, 
		0x00000145, 
		0x0000015b, 
		0x000001ab, 
		0x0000038d, 
		0x00000121, 
		0x000000f9, 
		0x000003b3, 
		0x00000121, 
		0x000003ed, 
		0x000003df, 
		0x00000067, 
		0x0000017d, 
		0x0000038f, 
		0x000003d3, 
		0x00000349, 
		0x00000391, 
		0x0000025f, 
		0x000000d7, 
		0x000003fd, 
		0x0000020d, 
		0x0000005f, 
		0x000003a1, 
		0x00000263, 
		0x00000217, 
		0x000001d5, 
		0x0000036d, 
		0x0000015f, 
		0x000002a9, 
		0x00000047, 
		0x000001fd, 
		0x00000065, 
		0x000001a9, 
		0x0000001b, 
		0x00000031, 
		0x0000011b, 
		0x000002cd, 
		0x00000365, 
		0x000003c9, 
		0x000002fd, 
		0x0000037b, 
		0x000000d5, 
		0x000001ed, 
		0x00000181, 
		0x00000033, 
		0x0000037d, 
		0x000001d7, 
		0x00000251, 
		0x000002b7, 
		0x00000209, 
		0x00000045, 
		0x0000009f, 
		0x00000223, 
		0x000000f5, 
		0x00000053, 
		0x000000b3, 
		0x000003c5, 
		0x0000018b, 
		0x000001b1, 
		0x00000085, 
		0x00000275, 
		0x00000257, 
		0x00000125, 
		0x00000079, 
		0x000001a3, 
		0x000001e3, 
		0x000002eb, 
		0x000002d5, 
		0x0000015b, 
		0x000001db, 
		0x00000381, 
		0x000003b5, 
		0x0000036d, 
		0x0000009f, 
		0x000000ab, 
		0x00000195, 
		0x00000231, 
		0x00000365, 
		0x0000021b, 
		0x00000381, 
		0x00000181, 
		0x00000111, 
		0x000000c5, 
		0x000002af, 
		0x000001c9, 
		0x0000029d, 
		0x000000a5, 
		0x0000034f, 
		0x000003fb, 
		0x000001b1, 
		0x0000002b  
};

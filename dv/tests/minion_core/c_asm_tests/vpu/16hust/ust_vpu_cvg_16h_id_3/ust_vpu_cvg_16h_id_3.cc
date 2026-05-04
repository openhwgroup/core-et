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
			"li x5, 128\n"
			"faddi.pi f2, f0, 87\n"
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
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
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f27,  192 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
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
			"li x0, 0x3\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x2\n" 
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 256(x27)\n"
			"flw.ps f12, 416(x27)\n"
			"fmul.ps f13, f2, f12, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 224(x11)\n"
			"flw.ps f30, 512(x11)\n"
			"fmin.ps f31, f8, f30\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 416(x26)\n"
			"fcvt.ps.pw f4, f30, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 512(x17)\n"
			"fsatu8.pi f12, f15\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 672(x11)\n"
			"frcp.ps f10, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.ps m6, f4, f12\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 800(x7)\n"
			"fsub.ps f0, f13, f13, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 384(x7)\n"
			"flw.ps f27, 288(x7)\n"
			"fadd.ps f30, f4, f27, rdn\n" 
			"li x5, 0xf0ccf2bb0406a7c6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xbef9f9f844f44a23\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 0(x27)\n"
			"flw.ps f2, 512(x27)\n"
			"fadd.ps f15, f9, f2, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 64(x11)\n"
			"flw.ps f2, 960(x11)\n"
			"flt.pi f2, f8, f2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 928(x6)\n"
			"flw.ps f15, 768(x6)\n"
			"for.pi f0, f10, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 256(x20)\n"
			"fclass.ps f27, f23\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 992(x26)\n"
			"fpackreph.pi f15, f23\n" 
			"li x5, 0xe686\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f13, 928(x26)\n"
			"flw.ps f27, 640(x26)\n"
			"fmin.pi f9, f13, f27\n" 
			"maskpopcz x11, m7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 0(x27)\n"
			"fsll.pi f30, f23, f23\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x7 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x6 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xa << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f13, 288(x5)\n"
			"flw.ps f7, 32(x5)\n"
			"fle.pi f0, f13, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 160(x5)\n"
			"flw.ps f0, 96(x5)\n"
			"flw.ps f8, 640(x5)\n"
			"fnmsub.s f7, f23, f0, f8, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 992(x7)\n"
			"flw.ps f24, 960(x7)\n"
			"feq.ps f7, f4, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f2, 896(x23)\n"
			"flw.ps f12, 800(x23)\n"
			"fle.pi f27, f2, f12\n" 
			"li x27, 0xecde8248e813a462\n"
			"fcvt.s.wu f2, x27, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 320(x11)\n"
			"fle.s x20, f0, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 608(x17)\n"
			"flw.ps f31, 832(x17)\n"
			"fmax.ps f9, f0, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 64(x11)\n"
			"fclass.ps f23, f0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 544(x5)\n"
			"flw.ps f31, 320(x5)\n"
			"fmax.s f10, f4, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 768(x6)\n"
			"flw.ps f30, 320(x6)\n"
			"fminu.pi f8, f7, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f8, 672(x7)\n"
			"flw.ps f2, 0(x7)\n"
			"fsub.pi f10, f8, f2\n" 
			"li x11, 0x4875fdb659515371\n"
			"fcvt.s.wu f30, x11, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 96(x5)\n"
			"fsrai.pi f2, f2, 0xe\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 32(x5)\n"
			"flw.ps f8, 128(x5)\n"
			"fcmovm.ps f4, f27, f8\n" 
			"li x5, 0xc897\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_int_32\n"
			"flw.ps f2, 544(x27)\n"
			"flw.ps f8, 384(x27)\n"
			"fand.pi f23, f2, f8\n" 
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x0 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xa << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 2\n" 
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
			"LBL_HPM_3_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_INST_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 448(x27)\n"
			"fswizz.ps f19, f25, 0x2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"maskand m5, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f24, 64(x7)\n"
			"flw.ps f23, 704(x7)\n"
			"fltu.pi f16, f24, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 512(x7)\n"
			"flw.ps f9, 928(x7)\n"
			"fsgnjx.ps f22, f14, f9\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 0(x22)\n"
			"flw.ps f29, 928(x22)\n"
			"fsgnj.s f30, f23, f29\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 704(x25)\n"
			"fle.pi f2, f9, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fsetm.pi m2, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f20, 864(x26)\n"
			"fslli.pi f14, f20, 0x8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f27, 608(x27)\n"
			"fpackrepb.pi f17, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 288(x23)\n"
			"fcvt.wu.s x6, f22, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 544(x13)\n"
			"flw.ps f28, 352(x13)\n"
			"fmin.s f9, f19, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 512(x10)\n"
			"flw.ps f20, 800(x10)\n"
			"fsgnj.s f17, f23, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 960(x27)\n"
			"fpackrepb.pi f28, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 512(x6)\n"
			"fcvt.ps.pw f29, f27, rtz\n" 
			"mova.x.m x25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 192(x25)\n"
			"feq.s x6, f24, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f30, 192(x23)\n"
			"fsrai.pi f4, f30, 0x8\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 96(x23)\n"
			"fsrli.pi f30, f16, 0x3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 992(x14)\n"
			"frsq.ps f17, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 96(x22)\n"
			"flw.ps f16, 480(x22)\n"
			"fdiv.s f19, f17, f16, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 768(x14)\n"
			"flw.ps f4, 224(x14)\n"
			"fmaxu.pi f20, f28, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 0(x14)\n"
			"flw.ps f19, 224(x14)\n"
			"for.pi f27, f24, f19\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 992(x6)\n"
			"flw.ps f30, 320(x6)\n"
			"fsgnj.s f22, f28, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 832(x14)\n"
			"flw.ps f9, 0(x14)\n"
			"fcmovm.ps f27, f28, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 224(x13)\n"
			"flw.ps f2, 768(x13)\n"
			"fadd.pi f27, f24, f2\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_1:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 928(x7)\n"
			"fexp.ps f19, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 832(x6)\n"
			"flw.ps f16, 32(x6)\n"
			"fmin.ps f29, f2, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 896(x14)\n"
			"flw.ps f17, 288(x14)\n"
			"fmin.s f9, f14, f17\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 736(x25)\n"
			"flw.ps f9, 320(x25)\n"
			"flw.ps f14, 992(x25)\n"
			"fnmadd.s f27, f27, f9, f14, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 864(x6)\n"
			"flw.ps f9, 416(x6)\n"
			"flw.ps f4, 640(x6)\n"
			"fcmov.ps f14, f30, f9, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 96(x10)\n"
			"for.pi f23, f29, f29\n" 
			"fbci.pi f20, 0x7a11b\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f19, 32(x15)\n"
			"flw.ps f28, 672(x15)\n"
			"fmax.pi f0, f19, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 864(x12)\n"
			"flw.ps f19, 384(x12)\n"
			"fsgnjx.ps f6, f17, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 224(x17)\n"
			"fcvt.ps.pwu f6, f19, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 992(x15)\n"
			"flw.ps f19, 0(x15)\n"
			"fmul.pi f6, f5, f19\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 928(x19)\n"
			"fpackreph.pi f6, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 96(x5)\n"
			"fclass.ps f27, f12\n" 
			"la x19, rand_int_32\n"
			"flw.ps f17, 800(x19)\n"
			"flw.ps f24, 896(x19)\n"
			"fmulhu.pi f24, f17, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 160(x5)\n"
			"flw.ps f6, 320(x5)\n"
			"feq.pi f28, f5, f6\n" 
			"maskpopc x19, m5\n" 
			"li x6, 0x211d6e9b612e2ed2\n"
			"fcvt.s.wu f24, x6, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 800(x12)\n"
			"flw.ps f20, 32(x12)\n"
			"fxor.pi f10, f27, f20\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 224(x6)\n"
			"flw.ps f25, 64(x6)\n"
			"fsub.pi f6, f5, f25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 32(x5)\n"
			"fcvt.ps.pwu f22, f25, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.ps m0, f6, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 640(x28)\n"
			"frsq.ps f20, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 544(x23)\n"
			"flw.ps f27, 0(x23)\n"
			"fsgnjn.s f0, f19, f27\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 128(x6)\n"
			"flw.ps f30, 576(x6)\n"
			"fle.s x6, f12, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f6, 384(x12)\n"
			"flw.ps f7, 768(x12)\n"
			"fsub.pi f25, f6, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"fsetm.pi m4, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f6, 128(x13)\n"
			"flw.ps f10, 128(x13)\n"
			"fltu.pi f27, f6, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 480(x13)\n"
			"flw.ps f11, 896(x13)\n"
			"fadd.ps f17, f20, f11, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 256(x6)\n"
			"fmulh.pi f0, f10, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 864(x23)\n"
			"fcvt.pw.ps f30, f0, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"feqm.ps m0, f30, f20\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f12, 352(x12)\n"
			"flw.ps f19, 384(x12)\n"
			"fadd.pi f7, f12, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 832(x28)\n"
			"flw.ps f11, 960(x28)\n"
			"fsgnjn.s f27, f22, f11\n" 
			"la x19, rand_int_32\n"
			"flw.ps f11, 384(x19)\n"
			"flw.ps f28, 640(x19)\n"
			"fmulh.pi f11, f11, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 256(x14)\n"
			"flw.ps f0, 192(x14)\n"
			"fltu.pi f17, f12, f0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 896(x5)\n"
			"flw.ps f30, 32(x5)\n"
			"feq.ps f5, f10, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"fltm.ps m4, f0, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 128(x12)\n"
			"fcvt.pwu.ps f27, f28, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 128(x5)\n"
			"flw.ps f0, 448(x5)\n"
			"fsub.ps f28, f24, f0, rne\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f13,  96 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"maskpopcz x16, m7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 192(x16)\n"
			"flw.ps f18, 0(x16)\n"
			"fadd.ps f6, f12, f18, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 96(x19)\n"
			"flw.ps f16, 352(x19)\n"
			"fsub.ps f29, f20, f16, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 480(x16)\n"
			"flw.ps f29, 992(x16)\n"
			"fadd.ps f6, f1, f29, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 992(x22)\n"
			"flw.ps f14, 288(x22)\n"
			"fmadd.s f12, f16, f14, f16, rtz\n" 
			"li x19, 0x396b6a641b734a46\n"
			"fcvt.s.wu f12, x19, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 384(x25)\n"
			"flw.ps f12, 576(x25)\n"
			"fmaxu.pi f2, f14, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 512(x16)\n"
			"flw.ps f12, 416(x16)\n"
			"fsgnjx.s f2, f20, f12\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f17, 608(x19)\n"
			"fcvt.ps.f16 f6, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 352(x9)\n"
			"fandi.pi f14, f14, 0x2f\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 320(x9)\n"
			"fsat8.pi f14, f29\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 192(x13)\n"
			"flw.ps f26, 544(x13)\n"
			"feq.pi f14, f20, f26\n" 
			"la x28, rand_int_32\n"
			"flw.ps f27, 864(x28)\n"
			"flw.ps f8, 640(x28)\n"
			"fadd.pi f8, f27, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"mov.m.x m7, x19, 0x21\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf1b4b6ae54\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xf3685a82348cca7a\n"
			"fcvt.s.wu f2, x5, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 192(x13)\n"
			"flw.ps f29, 640(x13)\n"
			"fsra.pi f6, f27, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 768(x19)\n"
			"flw.ps f16, 320(x19)\n"
			"fsub.ps f27, f27, f16, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 32(x13)\n"
			"flw.ps f27, 256(x13)\n"
			"fmin.pi f9, f12, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 64(x13)\n"
			"flw.ps f18, 832(x13)\n"
			"flw.ps f17, 832(x13)\n"
			"fmadd.ps f29, f1, f18, f17, rup\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_3:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 608(x13)\n"
			"fexp.ps f14, f20\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 416(x10)\n"
			"fsra.pi f2, f1, f1\n" 
			"fbci.pi f29, 0x7f7ed\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 448(x9)\n"
			"flw.ps f17, 224(x9)\n"
			"feq.ps f26, f6, f17\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 384(x17)\n"
			"flw.ps f20, 160(x17)\n"
			"fmax.s f27, f18, f20\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 960(x5)\n"
			"flt.s x10, f12, f12\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 704(x22)\n"
			"flw.ps f6, 0(x22)\n"
			"fle.s x9, f19, f6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 608(x13)\n"
			"flw.ps f16, 480(x13)\n"
			"fand.pi f26, f27, f16\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 960(x22)\n"
			"flw.ps f16, 32(x22)\n"
			"flw.ps f9, 256(x22)\n"
			"fmadd.s f1, f2, f16, f9, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 320(x13)\n"
			"flw.ps f24, 992(x13)\n"
			"fmin.ps f1, f6, f24\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 864(x19)\n"
			"flw.ps f16, 96(x19)\n"
			"flt.pi f9, f14, f16\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 960(x17)\n"
			"flw.ps f26, 768(x17)\n"
			"flt.pi f8, f6, f26\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_3:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 864(x13)\n"
			"frcp.ps f20, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskxor m5, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f8, 448(x26)\n"
			"fpackrepb.pi f8, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 480(x20)\n"
			"fsatu8.pi f16, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f18, 800(x6)\n"
			"flw.ps f20, 352(x6)\n"
			"fxor.pi f2, f18, f20\n" 
			"li x5, 0xb94b\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f0, 896(x23)\n"
			"flw.ps f26, 128(x23)\n"
			"flt.pi f29, f0, f26\n" 
			"maskpopcz x27, m5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"maskand m2, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 96(x23)\n"
			"flw.ps f17, 256(x23)\n"
			"fnmsub.ps f12, f20, f17, f17, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 160(x6)\n"
			"fslli.pi f30, f24, 0x1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_4:\n"
			"feqm.ps m3, f25, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x23, 0x3f7768f1ba0966d6\n"
			"fcvt.s.wu f20, x23, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 288(x5)\n"
			"fcvt.pwu.ps f17, f16, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 160(x7)\n"
			"fslli.pi f24, f23, 0x3\n" 
			"la x7, rand_int_32\n"
			"flw.ps f2, 416(x7)\n"
			"flw.ps f23, 192(x7)\n"
			"fand.pi f0, f2, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_4:\n"
			"fltm.pi m5, f29, f23\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 416(x26)\n"
			"fcvt.lu.s x23, f25, rdn\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f18, 0(x28)\n"
			"flw.ps f31, 608(x28)\n"
			"fmax.pi f8, f18, f31\n" 
			"li x19, 0xa5489b15a406399f\n"
			"fcvt.s.w f30, x19, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 640(x17)\n"
			"fpackreph.pi f1, f24\n" 
			"li x5, 0xfc83\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"fltm.pi m3, f20, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f2, 96(x27)\n"
			"flw.ps f20, 384(x27)\n"
			"feq.pi f2, f2, f20\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 480(x6)\n"
			"fcvt.ps.pw f20, f16, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 160(x7)\n"
			"fnot.pi f25, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 512(x19)\n"
			"flw.ps f29, 864(x19)\n"
			"flw.ps f25, 416(x19)\n"
			"fmsub.ps f2, f20, f29, f25, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 352(x7)\n"
			"flw.ps f1, 768(x7)\n"
			"fsub.ps f31, f23, f1, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_4:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 896(x19)\n"
			"fexp.ps f23, f31\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 480(x7)\n"
			"flw.ps f30, 704(x7)\n"
			"fle.pi f26, f17, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 768(x23)\n"
			"flw.ps f8, 288(x23)\n"
			"fle.ps f30, f24, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 640(x20)\n"
			"frsq.ps f30, f8\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f31, 576(x6)\n"
			"fcvt.ps.f16 f20, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 544(x23)\n"
			"flw.ps f25, 416(x23)\n"
			"fadd.ps f18, f31, f25, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 32(x23)\n"
			"for.pi f30, f24, f24\n" 
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
			"li x5, 0x21a42ed1f0885868\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2dcdf4829f5386f1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_19584_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbd << TENSOR_FMA16_SCP_LOC_B) | (0xe4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 0xf379\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x49 << TENSOR_IMA_SCP_LOC_B) | (0x64 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 608(x10)\n"
			"flw.ps f0, 288(x10)\n"
			"fsgnj.ps f19, f26, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 608(x19)\n"
			"flw.ps f18, 288(x19)\n"
			"fsgnjx.s f17, f29, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 448(x6)\n"
			"flw.ps f12, 576(x6)\n"
			"fsgnjx.s f29, f7, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 96(x19)\n"
			"fmax.s f12, f18, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 736(x17)\n"
			"fcvt.lu.s x19, f25, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 960(x19)\n"
			"fpackreph.pi f13, f0\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 928(x10)\n"
			"fclass.s x19, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f17, 0(x13)\n"
			"fsrai.pi f18, f17, 0x0\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 768(x19)\n"
			"fandi.pi f18, f2, 0x1e6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 448(x26)\n"
			"fmulh.pi f18, f17, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 352(x19)\n"
			"fsgnjx.ps f11, f26, f26\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 0(x17)\n"
			"flw.ps f17, 864(x17)\n"
			"fle.pi f13, f18, f17\n" 
			"la x16, rand_ieee754_16\n"
			"flw.ps f12, 320(x16)\n"
			"fcvt.ps.f16 f30, f12\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 544(x17)\n"
			"flw.ps f17, 704(x17)\n"
			"flw.ps f19, 448(x17)\n"
			"fmadd.s f16, f2, f17, f19, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 288(x26)\n"
			"flw.ps f8, 0(x26)\n"
			"fle.pi f30, f27, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 352(x16)\n"
			"flw.ps f29, 736(x16)\n"
			"flw.ps f12, 192(x16)\n"
			"fnmsub.ps f8, f26, f29, f12, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f27, 480(x19)\n"
			"flw.ps f13, 448(x19)\n"
			"fmul.pi f18, f27, f13\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 832(x13)\n"
			"frsq.ps f0, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 96(x10)\n"
			"flw.ps f13, 928(x10)\n"
			"feq.s x25, f12, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 768(x20)\n"
			"flw.ps f25, 704(x20)\n"
			"fsgnjx.s f16, f11, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 704(x25)\n"
			"flw.ps f7, 128(x25)\n"
			"flw.ps f27, 64(x25)\n"
			"fnmadd.ps f19, f19, f7, f27, rmm\n" 
			"mova.x.m x13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 576(x25)\n"
			"flw.ps f0, 96(x25)\n"
			"feq.s x6, f8, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 992(x19)\n"
			"flw.ps f25, 704(x19)\n"
			"flt.ps f30, f13, f25\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 736(x17)\n"
			"flw.ps f11, 0(x17)\n"
			"fdiv.pi f19, f26, f11\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 32(x25)\n"
			"flw.ps f7, 864(x25)\n"
			"fcmovm.ps f13, f26, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_5:\n"
			"feqm.ps m2, f11, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 320(x6)\n"
			"flw.ps f19, 256(x6)\n"
			"flt.ps f11, f8, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 704(x17)\n"
			"flw.ps f0, 768(x17)\n"
			"fmin.ps f19, f27, f0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_5:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 32(x25)\n"
			"fexp.ps f8, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 384(x10)\n"
			"fsgnj.ps f13, f27, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 672(x6)\n"
			"fcvt.pwu.ps f30, f26, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 192(x28)\n"
			"flw.ps f3, 800(x28)\n"
			"feq.ps f9, f23, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 192(x5)\n"
			"fcvt.f16.ps f14, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 704(x6)\n"
			"fclass.s x9, f13\n" 
			"li x9, 0xb9e1145cfda3d5a6\n"
			"fcvt.s.w f9, x9, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 0(x9)\n"
			"flw.ps f15, 864(x9)\n"
			"fdiv.pi f10, f14, f15\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 64(x16)\n"
			"fcvt.ps.pw f10, f25, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 64(x27)\n"
			"flw.ps f25, 160(x27)\n"
			"fle.s x26, f15, f25\n" 
			"maskpopc x9, m5\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 96(x12)\n"
			"flw.ps f27, 992(x12)\n"
			"flw.ps f14, 192(x12)\n"
			"fmsub.ps f15, f13, f27, f14, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 832(x16)\n"
			"flw.ps f7, 960(x16)\n"
			"fadd.ps f25, f3, f7, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 896(x26)\n"
			"flw.ps f13, 896(x26)\n"
			"fcmovm.ps f4, f11, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 704(x9)\n"
			"flw.ps f11, 480(x9)\n"
			"feq.s x14, f18, f11\n" 
			"li x5, 0x9f29\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 64(x9)\n"
			"ffrc.ps f11, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 224(x27)\n"
			"flw.ps f15, 960(x27)\n"
			"fsgnj.s f3, f18, f15\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 160(x14)\n"
			"flw.ps f9, 224(x14)\n"
			"fmul.ps f18, f15, f9, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"feqm.ps m4, f16, f25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x41a4f8fdec\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x6 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xf << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"maskpopcz x23, m5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 448(x23)\n"
			"flw.ps f4, 736(x23)\n"
			"flt.ps f13, f9, f4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 192(x6)\n"
			"fcvt.f16.ps f14, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 160(x23)\n"
			"flw.ps f9, 320(x23)\n"
			"fsgnjx.ps f27, f1, f9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 480(x9)\n"
			"flw.ps f18, 0(x9)\n"
			"fmin.ps f15, f4, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_6:\n"
			"fsetm.pi m0, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x9f3f\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f11, 544(x6)\n"
			"flw.ps f7, 256(x6)\n"
			"fmulh.pi f13, f11, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 32(x23)\n"
			"flw.ps f3, 704(x23)\n"
			"fxor.pi f3, f21, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"fltm.pi m0, f10, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f1, 928(x26)\n"
			"flw.ps f16, 640(x26)\n"
			"fmulhu.pi f3, f1, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 288(x16)\n"
			"ffrc.ps f11, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_6:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 192(x27)\n"
			"flog.ps f21, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 320(x5)\n"
			"flw.ps f23, 640(x5)\n"
			"fsgnjn.s f11, f14, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 672(x6)\n"
			"flw.ps f11, 832(x6)\n"
			"fmulhu.pi f21, f1, f11\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 672(x6)\n"
			"flw.ps f16, 96(x6)\n"
			"fmin.pi f23, f27, f16\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 736(x16)\n"
			"flw.ps f21, 640(x16)\n"
			"fmax.pi f3, f18, f21\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_522304_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbc << TENSOR_IMA_SCP_LOC_B) | (0x68 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 992(x23)\n"
			"flw.ps f15, 576(x23)\n"
			"fmin.ps f5, f9, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 288(x16)\n"
			"flw.ps f5, 640(x16)\n"
			"fmsub.ps f24, f12, f5, f12, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 64(x6)\n"
			"flw.ps f20, 672(x6)\n"
			"fsll.pi f5, f1, f20\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 864(x15)\n"
			"flw.ps f14, 928(x15)\n"
			"fsub.s f15, f25, f14, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 576(x13)\n"
			"flw.ps f24, 192(x13)\n"
			"flw.ps f22, 416(x13)\n"
			"fmadd.ps f9, f15, f24, f22, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 256(x13)\n"
			"frsq.ps f23, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 992(x13)\n"
			"fcvt.pw.ps f23, f24, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"fsetm.pi m6, f0\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 64(x23)\n"
			"fcvt.pwu.ps f8, f25, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 960(x19)\n"
			"fsrli.pi f14, f0, 0x7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 736(x16)\n"
			"flw.ps f25, 288(x16)\n"
			"fsgnjn.s f12, f15, f25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 224(x6)\n"
			"fcvt.ps.pwu f8, f4, rmm\n" 
			"fbci.pi f12, 0x665b1\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_7:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 864(x17)\n"
			"frcp.ps f24, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 448(x16)\n"
			"frsq.ps f22, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f5, 288(x6)\n"
			"flw.ps f15, 288(x6)\n"
			"fmulhu.pi f25, f5, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 608(x23)\n"
			"fcvt.w.s x13, f26, rup\n" 
			"fbci.pi f4, 0x4aae5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 864(x19)\n"
			"flw.ps f5, 352(x19)\n"
			"feq.ps f1, f15, f5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 288(x18)\n"
			"fcvt.w.s x18, f26, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_7:\n"
			"maskxor m6, m4, m4\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"fbci.pi f12, 0x4d7cb\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 576(x15)\n"
			"flw.ps f25, 96(x15)\n"
			"fremu.pi f1, f22, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 864(x15)\n"
			"flw.ps f25, 416(x15)\n"
			"fmul.s f0, f14, f25, rdn\n" 
			"li x13, 0x4219468a14c1ccc9\n"
			"fcvt.s.wu f14, x13, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 64(x13)\n"
			"flw.ps f6, 544(x13)\n"
			"flw.ps f0, 832(x13)\n"
			"fcmov.ps f15, f20, f6, f0\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 512(x20)\n"
			"fcvt.ps.pw f5, f1, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 608(x15)\n"
			"fcvt.wu.s x6, f20, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 992(x5)\n"
			"ffrc.ps f24, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 288(x18)\n"
			"flw.ps f14, 320(x18)\n"
			"fmadd.ps f23, f24, f14, f24, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 256(x6)\n"
			"flw.ps f15, 32(x6)\n"
			"feq.ps f22, f6, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f13, 640(x16)\n"
			"flw.ps f6, 736(x16)\n"
			"fmul.pi f23, f13, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 608(x26)\n"
			"flw.ps f7, 160(x26)\n"
			"flw.ps f6, 928(x26)\n"
			"fcmov.ps f7, f22, f7, f6\n" 
			"li x18, 0x39a06c9374a1e257\n"
			"fcvt.s.wu f8, x18, rup\n" 
			"li x26, 0x264fc57f883fe593\n"
			"fcvt.s.wu f20, x26, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 864(x12)\n"
			"flw.ps f25, 96(x12)\n"
			"fdiv.ps f23, f11, f25, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f30, 320(x12)\n"
			"flw.ps f6, 544(x12)\n"
			"fsll.pi f27, f30, f6\n" 
			"maskpopc x22, m4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 672(x9)\n"
			"flw.ps f7, 576(x9)\n"
			"fsgnjn.ps f17, f6, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"maskand m7, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x26, 0xa05fe6ec6cdbf912\n"
			"fcvt.s.wu f6, x26, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 448(x19)\n"
			"flw.ps f23, 32(x19)\n"
			"fmul.ps f15, f30, f23, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 704(x11)\n"
			"flw.ps f15, 960(x11)\n"
			"flt.s x12, f1, f15\n" 
			"li x5, 0x43e7\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f6, 128(x28)\n"
			"fsat8.pi f20, f6\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 160(x26)\n"
			"fnot.pi f25, f6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 0(x12)\n"
			"fcvt.w.s x22, f1, rne\n" 
			"li x5, 0x2c26ed344f7b477c\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x9adfa4c487744408\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 96(x18)\n"
			"flw.ps f7, 704(x18)\n"
			"fle.s x18, f6, f7\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_371027_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xcb << TENSOR_IMA_SCP_LOC_B) | (0x5a << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"addi x30, x30, 1\n"
			"fbci.ps f8, 0x17d05\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 640(x26)\n"
			"flw.ps f22, 224(x26)\n"
			"fle.pi f7, f23, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 992(x22)\n"
			"fswizz.ps f30, f1, 0x5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_8:\n"
			"maskxor m7, m7, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 608(x25)\n"
			"fswizz.ps f22, f17, 0x16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"mov.m.x m6, x25, 0x63\n" 
			"li x5, 0x80340120\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 928(x26)\n"
			"feq.s x19, f30, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"maskand m6, m0, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 320(x9)\n"
			"fround.ps f7, f15, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 224(x18)\n"
			"fnot.pi f30, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f17, 96(x25)\n"
			"flw.ps f15, 832(x25)\n"
			"fmin.ps f8, f17, f15\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 416(x18)\n"
			"flw.ps f17, 896(x18)\n"
			"fmul.pi f11, f15, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 928(x19)\n"
			"flw.ps f11, 256(x19)\n"
			"fmin.s f6, f6, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"fsetm.pi m6, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_8:\n"
			"fltm.pi m0, f6, f8\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xa4 << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x13, rand_int_32\n"
			"flw.ps f22, 64(x13)\n"
			"flw.ps f25, 768(x13)\n"
			"fsra.pi f9, f22, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 896(x12)\n"
			"flw.ps f13, 352(x12)\n"
			"fsub.pi f6, f5, f13\n" 
			"fbci.pi f6, 0x6ad09\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 64(x27)\n"
			"flw.ps f5, 0(x27)\n"
			"feq.s x13, f12, f5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 512(x28)\n"
			"flw.ps f7, 800(x28)\n"
			"fminu.pi f1, f31, f7\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 736(x20)\n"
			"flw.ps f13, 608(x20)\n"
			"fxor.pi f15, f31, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 32(x27)\n"
			"flw.ps f16, 384(x27)\n"
			"fsub.s f7, f25, f16, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskand m2, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f15, 864(x10)\n"
			"fsatu8.pi f28, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 416(x27)\n"
			"flw.ps f0, 192(x27)\n"
			"fmax.s f13, f12, f0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 416(x6)\n"
			"fnot.pi f6, f12\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 864(x28)\n"
			"flw.ps f0, 224(x28)\n"
			"for.pi f22, f9, f0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 352(x27)\n"
			"flw.ps f2, 160(x27)\n"
			"fsgnjn.s f2, f16, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"flem.ps m2, f31, f13\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb83ca42616\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f13, 0(x28)\n"
			"fcvt.ps.pw f13, f13, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 352(x17)\n"
			"fclass.s x13, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f9, 576(x27)\n"
			"flw.ps f27, 928(x27)\n"
			"fmulhu.pi f12, f9, f27\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 480(x25)\n"
			"fexp.ps f5, f5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 672(x28)\n"
			"flw.ps f9, 928(x28)\n"
			"fmin.s f25, f16, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"mov.m.x m6, x26, 0x4e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f22, 96(x12)\n"
			"fsat8.pi f31, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 512(x12)\n"
			"fcvt.f16.ps f2, f25\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 544(x17)\n"
			"fcvt.ps.pwu f0, f5, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 64(x17)\n"
			"flw.ps f31, 608(x17)\n"
			"feq.s x20, f13, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 896(x6)\n"
			"flw.ps f5, 480(x6)\n"
			"fand.pi f9, f31, f5\n" 
			"fbci.pi f2, 0x3388b\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"fltm.pi m6, f1, f1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xef546a930d\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 128(x20)\n"
			"fsat8.pi f1, f31\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_9:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 64(x13)\n"
			"fexp.ps f16, f27\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 800(x19)\n"
			"flw.ps f1, 160(x19)\n"
			"fltu.pi f12, f2, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 608(x20)\n"
			"fcvt.wu.s x26, f27, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 672(x12)\n"
			"flw.ps f28, 480(x12)\n"
			"fmax.s f9, f1, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f30,  64 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f4,  320 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 416(x7)\n"
			"fexp.ps f25, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 64(x16)\n"
			"fcvt.lu.s x7, f8, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 512(x11)\n"
			"flw.ps f31, 448(x11)\n"
			"fsgnjn.ps f20, f10, f31\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 832(x10)\n"
			"fcvt.ps.pwu f25, f31, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f10, 352(x10)\n"
			"fsat8.pi f2, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 384(x16)\n"
			"fclass.ps f9, f8\n" 
			"maskpopc x9, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 800(x9)\n"
			"flw.ps f27, 128(x9)\n"
			"flw.ps f9, 64(x9)\n"
			"fmadd.ps f2, f25, f27, f9, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 192(x12)\n"
			"flw.ps f14, 704(x12)\n"
			"feq.ps f18, f10, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 576(x7)\n"
			"fcvt.f16.ps f27, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 352(x7)\n"
			"flw.ps f20, 608(x7)\n"
			"fmax.ps f10, f9, f20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 928(x11)\n"
			"flw.ps f18, 96(x11)\n"
			"fsrl.pi f13, f14, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 320(x9)\n"
			"flw.ps f31, 64(x9)\n"
			"flw.ps f16, 416(x9)\n"
			"fmsub.ps f19, f25, f31, f16, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"maskand m1, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f27, 928(x12)\n"
			"flw.ps f18, 288(x12)\n"
			"fltu.pi f31, f27, f18\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 480(x10)\n"
			"fsatu8.pi f4, f2\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_616512_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_922566_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x3 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x7 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x5 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_121299_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x3 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x4 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f14, 256(x5)\n"
			"fsatu8.pi f9, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_10:\n"
			"maskor m0, m4, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x65eb\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f20, 480(x9)\n"
			"fsat8.pi f31, f20\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 544(x12)\n"
			"flw.ps f25, 32(x12)\n"
			"fmulh.pi f19, f17, f25\n" 
			"la x20, rand_int_32\n"
			"flw.ps f4, 992(x20)\n"
			"flw.ps f17, 0(x20)\n"
			"fle.pi f31, f4, f17\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 384(x10)\n"
			"flw.ps f25, 192(x10)\n"
			"fmul.pi f19, f13, f25\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 480(x5)\n"
			"fcvt.ps.pw f20, f14, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 736(x12)\n"
			"fcvt.pw.ps f2, f18, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 544(x7)\n"
			"fswizz.ps f14, f25, 0x9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 576(x10)\n"
			"flw.ps f19, 704(x10)\n"
			"fmul.pi f10, f27, f19\n" 
			"la x13, rand_int_32\n"
			"flw.ps f17, 736(x13)\n"
			"fsrli.pi f27, f17, 0x6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"fsetm.pi m0, f6\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 736(x20)\n"
			"fcvt.ps.pw f20, f31, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 384(x20)\n"
			"flw.ps f25, 256(x20)\n"
			"flw.ps f18, 192(x20)\n"
			"fmsub.ps f27, f31, f25, f18, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 864(x27)\n"
			"flw.ps f31, 640(x27)\n"
			"fsub.pi f6, f17, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"fsetm.pi m0, f13\n" 
			"mov.m.x m0, x0, 0xff\n"
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xc1 << TENSOR_FMA16_SCP_LOC_B) | (0x64 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
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
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
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
			"LBL_KERNEL_HID_11:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_11\n"
			"li x5, 0x2\n" 
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
			"maskpopcz x25, m2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 192(x25)\n"
			"flw.ps f6, 544(x25)\n"
			"fmax.ps f30, f22, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 320(x5)\n"
			"flw.ps f27, 480(x5)\n"
			"fmin.s f30, f19, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 928(x17)\n"
			"flw.ps f26, 800(x17)\n"
			"fle.s x22, f22, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"fltm.pi m4, f8, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f3, 736(x19)\n"
			"flw.ps f8, 192(x19)\n"
			"fadd.pi f27, f3, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 480(x14)\n"
			"flw.ps f18, 96(x14)\n"
			"fmin.s f6, f22, f18\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_11:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 256(x14)\n"
			"fexp.ps f6, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f30, 928(x19)\n"
			"flw.ps f8, 288(x19)\n"
			"fxor.pi f31, f30, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_11:\n"
			"masknot m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb2d0032e53\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 352(x19)\n"
			"flw.ps f31, 832(x19)\n"
			"fmin.s f21, f27, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_11:\n"
			"fltm.pi m2, f22, f18\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_int_32\n"
			"flw.ps f6, 320(x11)\n"
			"flw.ps f15, 416(x11)\n"
			"fmin.pi f20, f6, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 0(x5)\n"
			"fcvt.ps.pwu f18, f3, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 576(x18)\n"
			"flw.ps f18, 448(x18)\n"
			"feq.ps f6, f27, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 768(x10)\n"
			"flw.ps f3, 128(x10)\n"
			"flw.ps f22, 704(x10)\n"
			"fmadd.s f22, f15, f3, f22, rup\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f6, 384(x17)\n"
			"fcvt.ps.f16 f31, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f26, 800(x19)\n"
			"flw.ps f31, 768(x19)\n"
			"fmax.pi f19, f26, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 512(x5)\n"
			"fcvt.ps.pwu f21, f18, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 896(x14)\n"
			"flw.ps f27, 32(x14)\n"
			"fmul.pi f22, f20, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"maskand m6, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f1, 960(x25)\n"
			"flw.ps f8, 288(x25)\n"
			"feq.pi f30, f1, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_11:\n"
			"fsetm.pi m3, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_11:\n"
			"feqm.ps m4, f30, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 352(x25)\n"
			"flw.ps f31, 96(x25)\n"
			"flw.ps f20, 256(x25)\n"
			"fmadd.s f6, f22, f31, f20, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 768(x25)\n"
			"flw.ps f31, 128(x25)\n"
			"fminu.pi f15, f0, f31\n" 
			"maskpopc x20, m3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 608(x10)\n"
			"flw.ps f27, 608(x10)\n"
			"flt.ps f4, f22, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 736(x14)\n"
			"flw.ps f4, 160(x14)\n"
			"fsra.pi f4, f31, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 288(x11)\n"
			"flw.ps f26, 640(x11)\n"
			"feq.ps f19, f22, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 832(x20)\n"
			"flw.ps f27, 288(x20)\n"
			"fltu.pi f31, f18, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x7, rand_int_32\n"
			"flw.ps f24, 736(x7)\n"
			"fslli.pi f23, f24, 0xb\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 224(x17)\n"
			"flw.ps f31, 896(x17)\n"
			"fmulhu.pi f23, f2, f31\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 160(x16)\n"
			"flw.ps f6, 544(x16)\n"
			"flt.s x16, f18, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 64(x13)\n"
			"fsqrt.ps f31, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_int_32\n"
			"flw.ps f28, 160(x19)\n"
			"fsrli.pi f15, f28, 0x5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"masknot m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 992(x22)\n"
			"flw.ps f21, 160(x22)\n"
			"fsgnjn.s f28, f18, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 416(x17)\n"
			"fclass.s x17, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 64(x23)\n"
			"flw.ps f4, 416(x23)\n"
			"fsub.s f0, f29, f4, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 224(x11)\n"
			"fcvt.wu.s x11, f18, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 160(x14)\n"
			"flw.ps f25, 320(x14)\n"
			"feq.s x19, f15, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"flem.ps m3, f11, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"mov.m.x m6, x28, 0x3\n" 
			"li x5, 0x80340190\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_int_32\n"
			"flw.ps f21, 224(x7)\n"
			"flw.ps f29, 864(x7)\n"
			"fmaxu.pi f28, f21, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 896(x7)\n"
			"fclass.s x28, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 64(x11)\n"
			"ffrc.ps f3, f18\n" 
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x31 << TENSOR_FMA16_SCP_LOC_B) | (0x7f << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f21, 736(x16)\n"
			"flw.ps f18, 32(x16)\n"
			"fsll.pi f29, f21, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 384(x7)\n"
			"fcvt.pwu.ps f23, f21, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"mov.m.x m1, x17, 0x25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xec8bccc923\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 160(x11)\n"
			"flw.ps f6, 32(x11)\n"
			"fmin.ps f11, f2, f6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 768(x23)\n"
			"flw.ps f2, 256(x23)\n"
			"fle.ps f28, f0, f2\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 256(x16)\n"
			"flw.ps f29, 96(x16)\n"
			"fsll.pi f7, f18, f29\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 224(x14)\n"
			"flw.ps f18, 192(x14)\n"
			"fcmovm.ps f4, f28, f18\n" 
			"li x5, 0x71ff85fbfdb43d9c\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7ba9251212c50bd3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_ieee754_16\n"
			"flw.ps f31, 320(x22)\n"
			"fcvt.ps.f16 f31, f31\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 128(x14)\n"
			"flw.ps f2, 768(x14)\n"
			"fsra.pi f0, f0, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_12:\n"
			"masknot m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 640(x28)\n"
			"flw.ps f7, 352(x28)\n"
			"fmul.s f0, f4, f7, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f25, 960(x23)\n"
			"feq.pi f23, f25, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 384(x17)\n"
			"fcvt.f16.ps f31, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 576(x19)\n"
			"flw.ps f25, 672(x19)\n"
			"flw.ps f2, 768(x19)\n"
			"fmsub.s f0, f15, f25, f2, rmm\n" 
			"li x5, 0x609f\n"
			"csrw tensor_mask, x5\n"	
			"fbci.pi f7, 0x36b3a\n" 
			"li x22, 0x654b8ad277ad7ac5\n"
			"fcvt.s.wu f18, x22, rdn\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x75 << TENSOR_FMA32_SCP_LOC_B) | (0xb7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"mova.x.m x12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"masknot m1, m1\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"fbci.pi f30, 0x296d2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 544(x14)\n"
			"flw.ps f1, 960(x14)\n"
			"flw.ps f17, 576(x14)\n"
			"fnmsub.ps f22, f9, f1, f17, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"feqm.ps m1, f5, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 960(x12)\n"
			"flw.ps f30, 96(x12)\n"
			"fmin.ps f10, f5, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 288(x12)\n"
			"flw.ps f5, 416(x12)\n"
			"fmax.ps f25, f30, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 832(x11)\n"
			"fcvt.w.s x10, f28, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"maskxor m1, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_13:\n"
			"maskxor m1, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 352(x27)\n"
			"fsub.s f3, f25, f25, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 448(x11)\n"
			"flw.ps f25, 608(x11)\n"
			"flt.s x11, f9, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 224(x27)\n"
			"fclass.s x10, f30\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 832(x5)\n"
			"flw.ps f28, 640(x5)\n"
			"fle.pi f15, f11, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_13:\n"
			"mova.m.x x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x14, 0xa9d7458a458616fa\n"
			"fcvt.s.w f11, x14, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 544(x5)\n"
			"flw.ps f9, 416(x5)\n"
			"fmin.ps f1, f3, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 480(x12)\n"
			"fswizz.ps f5, f17, 0x17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 128(x27)\n"
			"flw.ps f10, 704(x27)\n"
			"flw.ps f1, 832(x27)\n"
			"fnmadd.ps f9, f17, f10, f1, rtz\n" 
			"li x23, 0x8451fbba16620772\n"
			"fcvt.s.lu f30, x23, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 768(x5)\n"
			"flw.ps f1, 192(x5)\n"
			"flt.pi f10, f26, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 160(x27)\n"
			"fpackrepb.pi f9, f9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 640(x27)\n"
			"flw.ps f15, 416(x27)\n"
			"fmul.pi f15, f3, f15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 608(x10)\n"
			"flw.ps f25, 864(x10)\n"
			"fdiv.ps f4, f28, f25, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 832(x11)\n"
			"flw.ps f10, 320(x11)\n"
			"fsra.pi f25, f30, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 864(x14)\n"
			"fcvt.pwu.ps f1, f29, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 864(x26)\n"
			"fclass.ps f4, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 608(x23)\n"
			"flw.ps f23, 448(x23)\n"
			"feq.ps f11, f4, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 512(x5)\n"
			"flw.ps f5, 736(x5)\n"
			"fsgnjn.s f28, f30, f5\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 768(x22)\n"
			"flw.ps f30, 96(x22)\n"
			"fsrl.pi f29, f4, f30\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 416(x10)\n"
			"fclass.s x23, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 768(x14)\n"
			"fswizz.ps f26, f17, 0x16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 608(x28)\n"
			"fclass.ps f6, f6\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 960(x26)\n"
			"frcp.ps f2, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 192(x26)\n"
			"flw.ps f16, 384(x26)\n"
			"fmul.ps f16, f24, f16, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 128(x26)\n"
			"flw.ps f13, 256(x26)\n"
			"fxor.pi f13, f7, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 224(x10)\n"
			"flw.ps f7, 608(x10)\n"
			"fadd.ps f10, f19, f7, rdn\n" 
			"li x5, 0x817ca5f515b9b1dd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xad389cd99997cd6f\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 512(x26)\n"
			"fmul.s f24, f7, f7, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 896(x9)\n"
			"flw.ps f10, 960(x9)\n"
			"fsgnjn.s f5, f26, f10\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 480(x26)\n"
			"fsat8.pi f15, f26\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 320(x11)\n"
			"flw.ps f6, 544(x11)\n"
			"flt.ps f26, f5, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 608(x9)\n"
			"flw.ps f24, 544(x9)\n"
			"fmax.s f16, f31, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 512(x26)\n"
			"flw.ps f7, 736(x26)\n"
			"fmulh.pi f16, f24, f7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 32(x12)\n"
			"fsgnjx.ps f31, f19, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 960(x19)\n"
			"flw.ps f2, 800(x19)\n"
			"flw.ps f24, 224(x19)\n"
			"fmsub.s f15, f20, f2, f24, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 64(x14)\n"
			"fsrai.pi f16, f26, 0xc\n" 
			"la x19, rand_int_32\n"
			"flw.ps f26, 704(x19)\n"
			"fnot.pi f7, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"masknot m1, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf1583fb94e\n"
			"fsw.ps f0, 0(x5)\n"
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 96(x19)\n"
			"flw.ps f16, 224(x19)\n"
			"feq.ps f5, f2, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 864(x26)\n"
			"flw.ps f15, 832(x26)\n"
			"fmul.s f15, f19, f15, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 64(x19)\n"
			"fclass.ps f28, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 288(x12)\n"
			"fclass.ps f26, f5\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 0(x10)\n"
			"fcvt.l.s x10, f10, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 224(x12)\n"
			"flw.ps f6, 448(x12)\n"
			"fsgnjn.ps f13, f30, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 192(x26)\n"
			"fcvt.f16.ps f23, f23\n" 
			"maskpopcz x27, m5\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 64(x10)\n"
			"flw.ps f10, 960(x10)\n"
			"fand.pi f5, f19, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 864(x16)\n"
			"flw.ps f10, 512(x16)\n"
			"fsgnjn.s f10, f20, f10\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 672(x14)\n"
			"flw.ps f23, 480(x14)\n"
			"fdiv.pi f20, f2, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 288(x14)\n"
			"flw.ps f28, 256(x14)\n"
			"fminu.pi f26, f6, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"fltm.pi m7, f31, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 160(x11)\n"
			"flw.ps f16, 864(x11)\n"
			"feq.ps f24, f15, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 544(x26)\n"
			"flw.ps f13, 800(x26)\n"
			"fmadd.ps f30, f28, f13, f28, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 608(x11)\n"
			"flw.ps f13, 320(x11)\n"
			"flw.ps f10, 160(x11)\n"
			"fnmadd.s f7, f5, f13, f10, rmm\n" 
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_999623_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x3 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xb << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_670656_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x7\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_4669_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x2 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x6 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1a << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x4 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 10\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 640(x22)\n"
			"fclass.ps f9, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 224(x22)\n"
			"flw.ps f17, 768(x22)\n"
			"feq.s x20, f14, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 896(x20)\n"
			"flw.ps f30, 64(x20)\n"
			"fadd.s f17, f15, f30, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 448(x18)\n"
			"fclass.ps f30, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 320(x26)\n"
			"flw.ps f9, 992(x26)\n"
			"fmul.ps f25, f29, f9, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 800(x11)\n"
			"flw.ps f12, 960(x11)\n"
			"fmul.pi f25, f20, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 960(x25)\n"
			"fcvt.pwu.ps f23, f25, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 352(x11)\n"
			"flw.ps f18, 128(x11)\n"
			"fmax.pi f16, f25, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskand m3, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 864(x26)\n"
			"flw.ps f17, 448(x26)\n"
			"flw.ps f18, 160(x26)\n"
			"fnmsub.s f20, f12, f17, f18, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 640(x18)\n"
			"faddi.pi f18, f7, 0xdd\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 0(x27)\n"
			"fround.ps f18, f12, rne\n" 
			"fbci.ps f20, 0x4a82f\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"maskor m3, m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f18, 384(x18)\n"
			"flw.ps f14, 544(x18)\n"
			"fmulhu.pi f29, f18, f14\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 864(x18)\n"
			"flw.ps f20, 544(x18)\n"
			"fmax.ps f29, f7, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f24, 192(x27)\n"
			"flw.ps f18, 928(x27)\n"
			"feq.pi f16, f24, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"maskand m4, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f17, 896(x20)\n"
			"flw.ps f20, 832(x20)\n"
			"fmin.pi f16, f17, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 288(x18)\n"
			"flw.ps f16, 576(x18)\n"
			"flw.ps f12, 352(x18)\n"
			"fcmov.ps f14, f17, f16, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 736(x5)\n"
			"flw.ps f17, 800(x5)\n"
			"fmul.ps f24, f16, f17, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 32(x11)\n"
			"flw.ps f23, 992(x11)\n"
			"fle.ps f14, f12, f23\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 96(x12)\n"
			"flw.ps f16, 224(x12)\n"
			"fcmovm.ps f20, f29, f16\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 608(x14)\n"
			"flw.ps f25, 384(x14)\n"
			"for.pi f16, f23, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 0(x11)\n"
			"flw.ps f17, 384(x11)\n"
			"fmul.pi f17, f30, f17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 960(x27)\n"
			"flw.ps f5, 576(x27)\n"
			"fmul.s f16, f30, f5, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 512(x18)\n"
			"fnot.pi f18, f29\n" 
			"fbci.ps f20, 0x5b1c\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 64(x5)\n"
			"flw.ps f9, 832(x5)\n"
			"fle.s x18, f24, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 256(x20)\n"
			"flw.ps f24, 736(x20)\n"
			"fmax.s f17, f23, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 704(x14)\n"
			"fsrli.pi f14, f17, 0x3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"mov.m.x m4, x25, 0x6f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
		0xb26ec1fc, 
		0xbb4fcd4c, 
		0x547677c1, 
		0x9079e326, 
		0x8912b4f1, 
		0x02a39920, 
		0x4e0f6002, 
		0x8810fac6, 
		0x2c6552fe, 
		0xf65a8828, 
		0x505a89b5, 
		0x9c5c6cae, 
		0xfbd2e4d8, 
		0x2a42079b, 
		0x8082c003, 
		0xbdb7739d, 
		0xc36bda16, 
		0xaa6e6439, 
		0x1ab38061, 
		0x5530ef12, 
		0xccaaf07e, 
		0x70b569ff, 
		0xb931a5d3, 
		0xf46b915b, 
		0xdc20cd54, 
		0x03e38c35, 
		0x6f5e72e1, 
		0x59407dfe, 
		0x78c12d3b, 
		0xa11b09a1, 
		0x13f937b3, 
		0xc22fbb1f, 
		0x556f6c55, 
		0xce77e5db, 
		0x891b103c, 
		0x4ab2458e, 
		0x2bf51a8e, 
		0x5f6e09a8, 
		0x74a00f09, 
		0x829e94fa, 
		0xd2240d07, 
		0xebe398e0, 
		0xa039b890, 
		0xbbf44546, 
		0xa40366d3, 
		0xe3ad2419, 
		0xbe91de33, 
		0xbdfb3b87, 
		0xff9caeaf, 
		0xb0e1f4d8, 
		0xd022a093, 
		0xc8656641, 
		0x2a159a45, 
		0xdab77828, 
		0xb6ed9ef5, 
		0x430cebaf, 
		0xfdcc807a, 
		0xefcda33c, 
		0x3ce046b0, 
		0x1e624c22, 
		0x19e0dc57, 
		0x533f1420, 
		0x4d3787d8, 
		0x99598aca, 
		0xa2f78cb5, 
		0xc870728f, 
		0x908c32b0, 
		0x79e01d07, 
		0x3d98eafd, 
		0x0905cd93, 
		0x12fa92eb, 
		0xd264f326, 
		0x2dd9126b, 
		0x794cd21b, 
		0x53a095b2, 
		0xd2fb9b04, 
		0x89b89738, 
		0x8b9c4304, 
		0x51a0557a, 
		0x921e7fae, 
		0x8f44eda5, 
		0x606b2094, 
		0x744bd46c, 
		0x67af5f0a, 
		0xeb18ca67, 
		0x6b4a50d2, 
		0x5d701ac8, 
		0x020db3b8, 
		0xcc44568f, 
		0x0b420b68, 
		0x6f2c10e3, 
		0xf2b4b17f, 
		0xf119d6c1, 
		0x2f06c56f, 
		0x992c2e59, 
		0x621b5c0e, 
		0xb1ab45bd, 
		0x28b79b33, 
		0xbc86e580, 
		0x283925cf, 
		0x4358cf46, 
		0xdf66fa47, 
		0xe2f52e19, 
		0x92ab1ccb, 
		0x179da664, 
		0x1b20f50d, 
		0xcad77da1, 
		0x42627878, 
		0xa594038f, 
		0xc60872e2, 
		0x135063e4, 
		0xcc34bf41, 
		0xba2e795a, 
		0xd1f1667b, 
		0x3bbd3feb, 
		0x43742127, 
		0xbecef5a5, 
		0xd91e1018, 
		0xf0309afa, 
		0x5b5370c3, 
		0x21cae119, 
		0xfdca6bdc, 
		0xac2ebcb1, 
		0x3d75c4fe, 
		0x7af27557, 
		0x5addfffc, 
		0xac1a6181, 
		0xdf0237ee, 
		0x8efc5a69, 
		0x643868ea, 
		0x4d1c78f9, 
		0x95f58794, 
		0xdb0c1c1b, 
		0x31cb51c7, 
		0xae81f649, 
		0xbb293dd0, 
		0x8a3fd107, 
		0xe997c125, 
		0xdd5d16dc, 
		0x92e56650, 
		0x25c3f2b8, 
		0x9ff26319, 
		0x1b2fdf7f, 
		0x5ad5e87a, 
		0x160a004f, 
		0x80d94b50, 
		0x52e6802e, 
		0xe6ba1ec8, 
		0xc7c79afd, 
		0x1be56ce5, 
		0xb8ab1709, 
		0xd90c22d2, 
		0x2edae9d8, 
		0x2f1cc974, 
		0xb070f94a, 
		0xa8c99f6d, 
		0x245063b1, 
		0x7c254018, 
		0xac19b4d6, 
		0xac86a931, 
		0xe14029a1, 
		0xa3fdabda, 
		0x9ee17bdd, 
		0x72dfab82, 
		0xc6a18948, 
		0xefc9e53e, 
		0x4b6bd02f, 
		0x7019927f, 
		0x808e53be, 
		0x3c82571e, 
		0x2908cfd4, 
		0x8703e252, 
		0x1905644b, 
		0xfb8721e4, 
		0x62cbe729, 
		0xd50ff8af, 
		0x8b420d70, 
		0x2a30a3dd, 
		0x79363bf4, 
		0x2cff2c5a, 
		0x550a6b26, 
		0x758b4535, 
		0xe81abf52, 
		0x9292ac2a, 
		0x5bd9587e, 
		0x09060ca8, 
		0x2df6fa51, 
		0x1e961d06, 
		0xdca4f473, 
		0xe5866a4d, 
		0x0cd54250, 
		0x57f714be, 
		0x5a1f9244, 
		0x1145e5ed, 
		0x610d5cc1, 
		0x63bbc9eb, 
		0x95b5fdd5, 
		0x170173ef, 
		0x3f2d09ca, 
		0x64bd592a, 
		0xf2d93769, 
		0x905e9324, 
		0x3dbe0179, 
		0x7409dfd7, 
		0xce88fb19, 
		0xbe2b2e66, 
		0xf59a5aaa, 
		0x1b259b3b, 
		0x6c3933b8, 
		0xb510dd8f, 
		0x4c0c534b, 
		0xffa7ae37, 
		0x7bc1ea6e, 
		0xde0bf392, 
		0x95855d12, 
		0xb73074ee, 
		0x7000fe0e, 
		0xfbcab28d, 
		0x4ab39472, 
		0xe3b93313, 
		0xb9123234, 
		0x0ac4085f, 
		0xcdc121d7, 
		0x03b1d7bb, 
		0xc14097a5, 
		0xde575fd8, 
		0x733ea73f, 
		0x3e8d577a, 
		0xaedd8d72, 
		0xa4f012c6, 
		0x91eddbde, 
		0x0657ff10, 
		0x4777637c, 
		0x00b5b7dd, 
		0xe6191277, 
		0xe146c9bb, 
		0x01f8152f, 
		0x79adcf72, 
		0x65f13f69, 
		0x04e03ebf, 
		0x8e88da75, 
		0xaa958733, 
		0x3f61b06d, 
		0x8002fe48, 
		0x117381c9, 
		0xddcdbe2c, 
		0x04c374c5, 
		0xb0eac223, 
		0x501bc7a9, 
		0x0be5b223, 
		0x133dc832, 
		0x1c85fb23, 
		0xb3e76fbf, 
		0x61abba7a, 
		0xd40693cd, 
		0x5bd5cb11  
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
		0x3c57, 
		0xd180, 
		0x5180, 
		0x4980, 
		0x5380, 
		0xc466, 
		0x4800, 
		0x4a80, 
		0xd260, 
		0xfa93, 
		0x4d80, 
		0xd3a0, 
		0x7c00, 
		0x57f5, 
		0xd0c0, 
		0x36e0, 
		0x4fc0, 
		0xd300, 
		0x4600, 
		0x5340, 
		0x5140, 
		0xcf9b, 
		0xd1c0, 
		0xcec0, 
		0xa245, 
		0x50e0, 
		0xc3e8, 
		0xc000, 
		0x8955, 
		0xd160, 
		0x297b, 
		0x4d80, 
		0x2e18, 
		0xca80, 
		0x7c00, 
		0xf6d8, 
		0xd0c0, 
		0x4f40, 
		0xf08d, 
		0x8000, 
		0x5020, 
		0x5080, 
		0x5080, 
		0xd0e0, 
		0x0000, 
		0xcd40, 
		0x4980, 
		0x7c00, 
		0xa551, 
		0x200b, 
		0xd160, 
		0xfc00, 
		0x04e3, 
		0x0000, 
		0xd0a0, 
		0xcb00, 
		0x52a0, 
		0x5300, 
		0xd360, 
		0xfc00, 
		0x4980, 
		0xd060, 
		0x9e58, 
		0xc800, 
		0xe360, 
		0xaf07, 
		0x5180, 
		0x5000, 
		0xc500, 
		0x5100, 
		0xca80, 
		0x4200, 
		0xfc93, 
		0x7c00, 
		0x5300, 
		0x5320, 
		0x8130, 
		0x51a0, 
		0x5060, 
		0xd320, 
		0x40d1, 
		0x0639, 
		0xde6a, 
		0x5040, 
		0x5340, 
		0x8000, 
		0x0258, 
		0x4cc0, 
		0x5180, 
		0x4e00, 
		0xbc00, 
		0xaed1, 
		0x8000, 
		0x8000, 
		0x5100, 
		0x9652, 
		0x9b81, 
		0xd2a0, 
		0x575c, 
		0x4d3a, 
		0xcc80, 
		0x6974, 
		0x5080, 
		0x5172, 
		0xd220, 
		0x740f, 
		0xcd80, 
		0x5320, 
		0xfc00, 
		0x50a0, 
		0x51a0, 
		0xd000, 
		0x4d00, 
		0x4600, 
		0x4600, 
		0x51e0, 
		0xc400, 
		0xca00, 
		0x7364, 
		0x50e0, 
		0xa269, 
		0x5260, 
		0xd080, 
		0xcc80, 
		0x7c00, 
		0xbc00, 
		0x4c00, 
		0x8000, 
		0xc74c, 
		0xd260, 
		0x52a0, 
		0xa02f, 
		0x804d, 
		0xd000, 
		0xd260, 
		0xd000, 
		0x4c40, 
		0xd340, 
		0x0000, 
		0xfc00, 
		0xd140, 
		0x3c00, 
		0xf8ff, 
		0x8377, 
		0x8000, 
		0xc700, 
		0x5200, 
		0x2b9e, 
		0x5140, 
		0xa977, 
		0xcb80, 
		0xd180, 
		0xfc00, 
		0x4cc0, 
		0x6e5f, 
		0xd180, 
		0x637b, 
		0x0f0f, 
		0xfc00, 
		0x4a72, 
		0xa2ef, 
		0x34cb, 
		0x9f09, 
		0x51e0, 
		0x2d47, 
		0x4e80, 
		0x4d4b, 
		0xf3e5, 
		0xcd40, 
		0x2702, 
		0xfdfc, 
		0x3e2f, 
		0x0074, 
		0x53c0, 
		0xd060, 
		0x4339, 
		0xf24d, 
		0x3c00, 
		0x4f80, 
		0xde51, 
		0x5100, 
		0xf4ab, 
		0x8000, 
		0x50ba, 
		0xcf80, 
		0x4f00, 
		0x26c1, 
		0x724e, 
		0x53bb, 
		0x7388, 
		0xc706, 
		0xd180, 
		0x4500, 
		0x4ec0, 
		0xfc00, 
		0xd3e0, 
		0x3c1c, 
		0x5380, 
		0x4c80, 
		0xbf5d, 
		0xdd92, 
		0x5260, 
		0xc880, 
		0xd1c0, 
		0x2742, 
		0x77a3, 
		0xd380, 
		0xc65b, 
		0x6d23, 
		0x4388, 
		0xc2ff, 
		0x5020, 
		0x4880, 
		0x5200, 
		0xd060, 
		0xd3e0, 
		0x7c00, 
		0x0eeb, 
		0xc000, 
		0x4e80, 
		0x74f5, 
		0x0cb8, 
		0x50c0, 
		0x5320, 
		0xd320, 
		0xd3e0, 
		0x4e00, 
		0xcb69, 
		0xcab5, 
		0x5180, 
		0x8000, 
		0xd100, 
		0x4d80, 
		0xcc40, 
		0x4c00, 
		0x5040, 
		0xcc00, 
		0x4d80, 
		0x4f80, 
		0x24d0, 
		0xdcfb, 
		0xcd80, 
		0xaf68, 
		0x19b6, 
		0xdca3, 
		0x7a05, 
		0x4500, 
		0x2552, 
		0x50a0, 
		0x4200, 
		0x4e40, 
		0xce40, 
		0xcabc, 
		0xcf80, 
		0xcf40, 
		0x5260, 
		0xb033, 
		0x6baa, 
		0xd3a0, 
		0x4b00, 
		0x1f68, 
		0xcf40, 
		0xcc80, 
		0x50e0, 
		0x1430, 
		0x9afe, 
		0xd340, 
		0xbc00, 
		0xce00, 
		0x10b1, 
		0xbe99, 
		0xf8ac, 
		0x5200, 
		0xcf00, 
		0x9a65, 
		0xcfc0, 
		0x7dfe, 
		0x4d40, 
		0x2b0b, 
		0x50e0, 
		0xd020, 
		0x8d52, 
		0x52c0, 
		0xc3f0, 
		0xd120, 
		0xf0b7, 
		0xd000, 
		0xd360, 
		0xca80, 
		0xa07d, 
		0xd591, 
		0xcf40, 
		0xd240, 
		0xd040, 
		0x7c00, 
		0xd280, 
		0xc232, 
		0xd0c0, 
		0xd1c0, 
		0x7bbb, 
		0xc500, 
		0xc980, 
		0x96a1, 
		0xce00, 
		0x4f00, 
		0x4fc0, 
		0xfc00, 
		0xc600, 
		0x8efc, 
		0xce40, 
		0x5340, 
		0x5ead, 
		0xc800, 
		0x3c00, 
		0x51a0, 
		0x8000, 
		0x5300, 
		0x4d80, 
		0x4fc0, 
		0xd200, 
		0x5200, 
		0xc880, 
		0x5ee3, 
		0xd360, 
		0x5280, 
		0x16e4, 
		0xd080, 
		0x4c00, 
		0x81bd, 
		0x4200, 
		0x0000, 
		0xd90a, 
		0xcb80, 
		0xf66e, 
		0x5833, 
		0xc203, 
		0x4dc0, 
		0xd927, 
		0x51e0, 
		0xb9e4, 
		0xca00, 
		0xbc00, 
		0xfc00, 
		0xcdc0, 
		0xcb80, 
		0x5280, 
		0x4d40, 
		0xd320, 
		0xc576, 
		0x0cc8, 
		0xc700, 
		0x48fa, 
		0x4e00, 
		0x4cb9, 
		0xd1c0, 
		0x7c00, 
		0x50f1, 
		0x4d80, 
		0xd2dd, 
		0x013d, 
		0x52a0, 
		0xfc00, 
		0x5020, 
		0xd5c5, 
		0xcc80, 
		0xd3c0, 
		0xd160, 
		0x4d80, 
		0x4c00, 
		0x49c6, 
		0x4980, 
		0xcec0, 
		0x8000, 
		0xcf40, 
		0x52a0, 
		0xd1e0, 
		0xd1a0, 
		0xd200, 
		0xd000, 
		0x375d, 
		0x51a0, 
		0x2e00, 
		0x52c0, 
		0xcf00, 
		0x4dc0, 
		0x4d00, 
		0x5300, 
		0xcd00, 
		0x4c80, 
		0x33b0, 
		0x50c0, 
		0x50c0, 
		0xfd8a, 
		0xac58, 
		0xd360, 
		0x5320, 
		0x4a00, 
		0x5180, 
		0xbc00, 
		0x8000, 
		0x741a, 
		0xd040, 
		0x5020, 
		0xcc80, 
		0x7c00, 
		0x32ba, 
		0x51e0, 
		0x5080, 
		0x06fe, 
		0x2e47, 
		0x2d15, 
		0xf0d4, 
		0x9b62, 
		0x53a0, 
		0x7c00, 
		0x5240, 
		0xcd00, 
		0x834b, 
		0xd220, 
		0x5360, 
		0x4c40, 
		0xfc00, 
		0xd300, 
		0xca80, 
		0x51c0, 
		0xca80, 
		0x51c0, 
		0x4ec0, 
		0xcd40, 
		0x8000, 
		0x08f8, 
		0x3c00, 
		0x52a0, 
		0xc000, 
		0x29bd, 
		0xd000, 
		0xd180, 
		0xc000, 
		0x3bd3, 
		0x3d3e, 
		0x5240, 
		0x3c9a, 
		0xfd5c, 
		0x0000, 
		0x68bc, 
		0x4e61, 
		0x4e40, 
		0xb078, 
		0x4f80, 
		0xcf80, 
		0xd060, 
		0x4d00, 
		0x0000, 
		0xbb53, 
		0x10e8, 
		0x6a44, 
		0x4719, 
		0x4f40, 
		0x52a0, 
		0xd300, 
		0xd200, 
		0xd200, 
		0x4400, 
		0xd3a0, 
		0x7c00, 
		0x52a0, 
		0x8000, 
		0xc800, 
		0x4980, 
		0x7c00, 
		0xcf40, 
		0x5120, 
		0x9fa9, 
		0x50c0, 
		0xce40, 
		0x5140, 
		0xea6b, 
		0x3aca, 
		0x0000, 
		0xcd40, 
		0x0000, 
		0x4500, 
		0xd3e0, 
		0x54b1, 
		0x52a0, 
		0xb7a4, 
		0xcd00, 
		0x27b2, 
		0x3b21, 
		0xd380, 
		0x7c00, 
		0xaf3d, 
		0x7018, 
		0x4200, 
		0x3a51, 
		0xd100, 
		0xc000, 
		0xc944, 
		0x8000, 
		0x4827, 
		0x5080, 
		0x4d80, 
		0x51c0, 
		0xae31, 
		0x51a0, 
		0xaeab, 
		0x8000, 
		0xa1d1, 
		0x4a00, 
		0xbc00, 
		0x4c00, 
		0x95a1, 
		0x5340, 
		0x8000, 
		0x5120, 
		0x4700, 
		0x5180, 
		0x52c0, 
		0xc600, 
		0xd260, 
		0x4500, 
		0xd3c0, 
		0xcf40, 
		0x2a47, 
		0x4800, 
		0x5140, 
		0x4700, 
		0xd2a0, 
		0x3c00, 
		0x5000, 
		0x4f41, 
		0xd160, 
		0x981f, 
		0xc441, 
		0x5260, 
		0xc700, 
		0xd360, 
		0x5380, 
		0x8000, 
		0x5260, 
		0xa626, 
		0x4880, 
		0xef3b, 
		0x8000, 
		0xfd31, 
		0x4c40, 
		0x078a, 
		0xa6c6, 
		0x5260, 
		0xcb80, 
		0xb8e7, 
		0xcd40, 
		0x4dc0, 
		0x4b59, 
		0x4cc0, 
		0x6f02, 
		0xc900, 
		0xbc00, 
		0xcd00, 
		0xf3c4, 
		0x9905, 
		0x4000, 
		0xeced, 
		0xd1c0, 
		0xd2a0, 
		0xd080, 
		0x4f80, 
		0x4e00, 
		0x5060, 
		0xd3c0, 
		0x4c40, 
		0xe331, 
		0x5240, 
		0xccc0, 
		0x4e30, 
		0x4f00, 
		0x5140, 
		0xcd80, 
		0x50e0, 
		0x7431, 
		0xd060, 
		0xd1e0, 
		0xd260, 
		0xd120, 
		0x738d, 
		0xbb97, 
		0x4c00, 
		0x51c0, 
		0xd220, 
		0x8000, 
		0x3c00, 
		0x5240, 
		0xcf00, 
		0xcb00, 
		0xd30c, 
		0x4f40, 
		0xa98e, 
		0x03ed, 
		0x7166, 
		0xffc1, 
		0xcb00, 
		0xd320, 
		0x51c0, 
		0x5380, 
		0x4e40, 
		0x4c00, 
		0x4f00, 
		0x0d8c, 
		0xd360, 
		0xce80, 
		0xce00, 
		0xc600, 
		0x4000, 
		0xc880, 
		0xc880, 
		0x0000, 
		0x4e00, 
		0xd2e0, 
		0x8000, 
		0xc400, 
		0x0262, 
		0x61d2, 
		0x3c00, 
		0xca00, 
		0xef3c, 
		0xd080, 
		0x4980, 
		0xcd40, 
		0xfcce, 
		0x5380, 
		0xa389, 
		0x4c80, 
		0x15e9, 
		0x4c40, 
		0x4f00, 
		0x2f62, 
		0xce40, 
		0x06ea, 
		0x5020, 
		0xd299, 
		0xd1a0, 
		0xb35c, 
		0x4e00, 
		0x52e0, 
		0xa6fe, 
		0xcfc0, 
		0x50c0, 
		0x7917, 
		0x53a0, 
		0x5100, 
		0xc600, 
		0xcdc0, 
		0x109b, 
		0x51e0, 
		0x5000, 
		0x2b1a, 
		0x09f0, 
		0x6872, 
		0x2740, 
		0xd220, 
		0x9631, 
		0x8052, 
		0x5300, 
		0xaa2e, 
		0x4b80, 
		0xd1a0, 
		0x4b00, 
		0x5140, 
		0x5100, 
		0xebad, 
		0xd3a0, 
		0x5380, 
		0x1727, 
		0xc980, 
		0x51a0, 
		0xc400, 
		0x0000, 
		0x8e99, 
		0xd200, 
		0xd0e0, 
		0x4d40, 
		0x50e0, 
		0x5060, 
		0xfc00, 
		0x5fee, 
		0x044b, 
		0xce00, 
		0x50a0, 
		0xc900, 
		0xc012, 
		0x3c00, 
		0x4c40, 
		0x4a81, 
		0x4c00, 
		0x4c40, 
		0xe2ca, 
		0x0000, 
		0x4dc0, 
		0x50a0, 
		0xcc80, 
		0x3ad1, 
		0x52e0, 
		0xd3e0, 
		0x0709, 
		0xd240, 
		0x5280, 
		0xd238, 
		0xc200, 
		0xfc00, 
		0x4800, 
		0xd1e0, 
		0x6b76, 
		0x8000, 
		0x4d40, 
		0x4a00, 
		0x4f40, 
		0xa27c, 
		0xc880, 
		0x0000, 
		0xbbf7, 
		0xd200, 
		0xc400, 
		0xbc00, 
		0xcec0, 
		0x7c00, 
		0xd300, 
		0x53e0, 
		0x940b, 
		0xd0a0, 
		0x8000, 
		0x4fc0, 
		0x89e9, 
		0xd080, 
		0xd340, 
		0xd1a0, 
		0xd2e0, 
		0x8b74, 
		0xd2a0, 
		0x4e80, 
		0x4400, 
		0x4200, 
		0xdb45, 
		0x50a0, 
		0x5040, 
		0xd0c0, 
		0x037c, 
		0x5380, 
		0x373a, 
		0xd120, 
		0x5240, 
		0x4e80, 
		0x5260, 
		0xd180, 
		0xd260, 
		0x5360, 
		0xe433, 
		0x412f, 
		0x4d00, 
		0xa8eb, 
		0xcc80, 
		0x5766, 
		0xd080, 
		0xcec0, 
		0x8000, 
		0x873e, 
		0x52c0, 
		0xd080, 
		0xc200, 
		0xd260, 
		0xcf00, 
		0x838b, 
		0xcd80, 
		0x5260, 
		0x7a43, 
		0x51e0, 
		0xc880, 
		0xe416, 
		0xce40, 
		0xcc40, 
		0x5000, 
		0x0b5b, 
		0xc400, 
		0x4c80, 
		0xce80, 
		0xc8aa, 
		0xd3e0, 
		0x4700, 
		0xd160, 
		0xd1c0, 
		0xfca5, 
		0x5340, 
		0x5240, 
		0x4e80, 
		0x0619, 
		0xb233, 
		0xce40, 
		0xc09f, 
		0xd300, 
		0xcdc0, 
		0xccc0, 
		0x4f80, 
		0x7c31, 
		0xd3e0, 
		0x4d80, 
		0xc3d3, 
		0x83b6, 
		0x5260, 
		0xd2c0, 
		0x50c0, 
		0xd0a0, 
		0x8de4, 
		0xd280, 
		0xc500, 
		0xfa92, 
		0x5100, 
		0x838c, 
		0x4d00, 
		0xadf1, 
		0xd2a0, 
		0x6a23, 
		0xd0e0, 
		0x4b80, 
		0x4900, 
		0x7c00, 
		0x9550, 
		0x0b8c, 
		0x5020, 
		0xac5f, 
		0x3c00, 
		0xd060, 
		0xc980, 
		0xcc80, 
		0xd3c0, 
		0x4200, 
		0x4b80, 
		0x91d8, 
		0x0000, 
		0x5160, 
		0x5020, 
		0xd485, 
		0xc71e, 
		0xcf00, 
		0x4ee1, 
		0xd280, 
		0xca00, 
		0xd360, 
		0x345f, 
		0xd200, 
		0xcd40, 
		0xa0a6, 
		0xeaec, 
		0x836f, 
		0xcb80, 
		0x4f40, 
		0x3c00, 
		0xd1a0, 
		0x4800, 
		0x7d2e, 
		0xa663, 
		0xd360, 
		0xd1a0, 
		0xcf80, 
		0xd2a0, 
		0xaf4d, 
		0xd080, 
		0x4e00, 
		0x19c8, 
		0xd140, 
		0x092a, 
		0x8a5b, 
		0xca00, 
		0xadf5, 
		0x52c0, 
		0x459e, 
		0x0000, 
		0xd200, 
		0x5000, 
		0x4f00, 
		0x2232, 
		0xd180, 
		0xd060, 
		0xd340, 
		0xd260, 
		0x5320, 
		0x4600, 
		0x52a0, 
		0xce00, 
		0xd1e0, 
		0x0000, 
		0xd3e0, 
		0x5380, 
		0xfacd, 
		0xd0c0, 
		0x5020, 
		0x5872, 
		0xd160, 
		0x0000, 
		0xcd40, 
		0x8000, 
		0x50c0, 
		0x53c0, 
		0xce80, 
		0xd0a0, 
		0x50c0, 
		0xb2d3, 
		0x4d40, 
		0x76f6, 
		0x4e00, 
		0xdb25, 
		0xc744, 
		0xf9e0, 
		0xd160, 
		0xd260, 
		0xcc40, 
		0x6c94, 
		0x979b, 
		0xd025, 
		0x6084, 
		0x4c40, 
		0x7c94, 
		0x5100, 
		0xcc80, 
		0xae42, 
		0x5ad4, 
		0x5280, 
		0x4c40, 
		0xc000, 
		0x7c00, 
		0x5080, 
		0xf266, 
		0xd3c0, 
		0x4f40, 
		0x5200, 
		0xfc6f, 
		0xc500, 
		0x4a80, 
		0xd1c6, 
		0x4880, 
		0x23cb, 
		0xd3c0, 
		0x9943, 
		0x342e, 
		0xc600, 
		0xd240, 
		0x0000, 
		0x4c00, 
		0xfc00, 
		0x5080, 
		0x5140, 
		0x5080, 
		0xd100, 
		0xd1e0, 
		0x5000, 
		0x0000, 
		0x5d24, 
		0xd200, 
		0x0b9f, 
		0x5040, 
		0xd100, 
		0x4b80, 
		0xdec2, 
		0x5300, 
		0xd2e0, 
		0x52e0, 
		0x51c0, 
		0xabdb, 
		0xcc80, 
		0x4f00, 
		0xcec0, 
		0xcd40, 
		0x8cf8, 
		0x53e0, 
		0x4b80, 
		0xd0e0, 
		0xa206, 
		0x52c0, 
		0x2aae, 
		0x51c0, 
		0xe54d, 
		0x4f80, 
		0x5140, 
		0x5140, 
		0x5280, 
		0x4500, 
		0xbee6, 
		0x6a1c, 
		0xc8aa, 
		0x5120, 
		0xcc40, 
		0xd2a0, 
		0xd100, 
		0xd380, 
		0x4500, 
		0xc200, 
		0x0000, 
		0x5200, 
		0xd000, 
		0x5380, 
		0xc000, 
		0xd1e0, 
		0x8000, 
		0x53c0, 
		0x51a0, 
		0x9f34, 
		0x0000, 
		0x4cc0, 
		0xd000, 
		0xce80, 
		0x5000, 
		0xd060, 
		0x283c, 
		0xc103, 
		0xccc0, 
		0x51e0, 
		0x5320, 
		0xd3e0, 
		0x4a00, 
		0xd1c0, 
		0x3704, 
		0x4600, 
		0x92ab, 
		0x5180, 
		0x4900, 
		0x53e0, 
		0xf822, 
		0x4f00, 
		0x3e61, 
		0xcfc0, 
		0xcd40, 
		0xd140, 
		0xc30e, 
		0xfc00, 
		0x8000, 
		0xd88a, 
		0x4200, 
		0x4fc0, 
		0xf209, 
		0x4c40, 
		0xc880, 
		0x8000, 
		0x25fd, 
		0x0d6a, 
		0x5360, 
		0xb048, 
		0xd2e0, 
		0xc880, 
		0x4394, 
		0x4700, 
		0x9ed0, 
		0xd300, 
		0x5d21, 
		0xd1e0, 
		0x4200, 
		0x5220, 
		0xd180, 
		0x50a0, 
		0x53e0, 
		0x35a4, 
		0xd1c0, 
		0x260b, 
		0x42f3, 
		0x8000, 
		0x21dc, 
		0xcbcb, 
		0x5340, 
		0xc980, 
		0x8000, 
		0xcc00, 
		0x5040, 
		0x4600, 
		0xc900, 
		0xc900, 
		0xd300, 
		0x54fc, 
		0xd100, 
		0x5080, 
		0x4b00, 
		0xcd80, 
		0x52e0, 
		0xc900, 
		0xa0bc, 
		0xce80, 
		0x0000, 
		0xd180, 
		0xd020, 
		0x5ca5, 
		0x6464, 
		0xd000, 
		0x51e0, 
		0xd380, 
		0x5000, 
		0x5080, 
		0x51e0, 
		0xc600, 
		0x5000, 
		0x8000, 
		0x72d7, 
		0xc800, 
		0x95a7, 
		0xd240, 
		0xbe47, 
		0xd3a0, 
		0x5240, 
		0xcfc0, 
		0x5120, 
		0x5200, 
		0xac2f, 
		0xd100, 
		0xfc6d, 
		0x5360, 
		0xd240, 
		0xd03c, 
		0x4e80, 
		0xcdc0, 
		0xa3cd, 
		0x4400, 
		0x5e34, 
		0xfd94, 
		0xce40, 
		0x111b, 
		0xd040, 
		0xcf80, 
		0x4d80, 
		0x77e8, 
		0x7c00, 
		0x50a0, 
		0xd5c8, 
		0xfc00, 
		0xc800, 
		0x5060, 
		0x5300, 
		0xcf40, 
		0x599d, 
		0x4e00, 
		0xc000, 
		0xce80, 
		0x5040, 
		0x4f40, 
		0x023a, 
		0x5200, 
		0xd0c0, 
		0xcf40, 
		0x6c75, 
		0x0000, 
		0x5100, 
		0xd1e0, 
		0xccc0, 
		0xd260, 
		0x52a0, 
		0x53a0, 
		0x4c80, 
		0xcd00, 
		0x511d, 
		0xcdc0, 
		0xd2a0, 
		0x10e9, 
		0xd320, 
		0xc5f8, 
		0x5060, 
		0xc900, 
		0xccc0, 
		0xd020, 
		0xd1a0, 
		0x4b00, 
		0x4880, 
		0x5340, 
		0xcfc0, 
		0xd280, 
		0x4e00, 
		0x0000, 
		0xd020, 
		0xd180, 
		0x4c40, 
		0xcd00, 
		0x5280, 
		0xbc37, 
		0x775f, 
		0xcc00, 
		0x6b64, 
		0x0118, 
		0xac5b, 
		0xd1c0, 
		0x41c0, 
		0xd360, 
		0xf0e5, 
		0xcf00, 
		0x4dc0, 
		0xdec0, 
		0xbc00, 
		0xd802, 
		0x7c00, 
		0x0000, 
		0x8633, 
		0x4a00, 
		0x40b0, 
		0xfbd0, 
		0x5240, 
		0xcb00, 
		0x52c0, 
		0xfc00, 
		0x4fc0, 
		0x5140, 
		0xcb00, 
		0xd3c0, 
		0x38e8, 
		0xa8ff, 
		0x53c0, 
		0xc800, 
		0x5200, 
		0x5200, 
		0x51a0, 
		0x5140, 
		0x4e00, 
		0xd3c0, 
		0xd020, 
		0xd060, 
		0x51e0, 
		0xbc00, 
		0xd1a0, 
		0xd200, 
		0xd2b0, 
		0x6abb, 
		0xcf80, 
		0x3470, 
		0x5140, 
		0xc700, 
		0x93dc, 
		0xce80, 
		0x79b1, 
		0x15e3, 
		0x5240, 
		0x5e80, 
		0x5180, 
		0xfc00, 
		0x5360, 
		0x0000, 
		0x4e00, 
		0x651b, 
		0xf360, 
		0xc600, 
		0xbc32, 
		0x087b, 
		0x1f94, 
		0x5260, 
		0x53a0, 
		0xc000, 
		0x89d6, 
		0x0000, 
		0x90e2, 
		0x5060, 
		0x53c0, 
		0x51a0, 
		0x4cc0, 
		0xe140, 
		0x4980, 
		0x51e0, 
		0x5080, 
		0x7c00, 
		0x5360, 
		0x550d, 
		0xce00, 
		0xef80, 
		0x0000, 
		0x5740, 
		0xd1e0, 
		0x5aba, 
		0x79d1, 
		0x8c41, 
		0x202c, 
		0x3b3e, 
		0xae98, 
		0xd1e0, 
		0xca00, 
		0xd0a0, 
		0x1acc, 
		0x23e5, 
		0xd100, 
		0xc11a, 
		0xcb80, 
		0xd2a0, 
		0x7ff8, 
		0x5040, 
		0x4b00, 
		0xd080, 
		0x0000, 
		0xcc00, 
		0xd160, 
		0xccc0, 
		0x4000, 
		0xd040, 
		0x51e0, 
		0x5320, 
		0xcd80, 
		0xf5f5, 
		0x8233, 
		0xd180, 
		0x55ef, 
		0xcd80, 
		0x7c00, 
		0xcdc0, 
		0x4000, 
		0x51a0, 
		0xa1d7, 
		0x5280, 
		0x742e, 
		0x5180, 
		0x53a0, 
		0x4200, 
		0x1480, 
		0x4827, 
		0xcdc0, 
		0x4c80, 
		0x4a80, 
		0x5040, 
		0x8000, 
		0x4cc0, 
		0x8000, 
		0x8000, 
		0x4b80, 
		0x5380, 
		0x4e00, 
		0xcf40, 
		0x0275, 
		0xd1c0, 
		0xcfbe, 
		0xd280, 
		0xd3e0, 
		0x5000, 
		0x632c, 
		0xd340, 
		0x4d80, 
		0xcf40, 
		0xc900, 
		0xfc00, 
		0xbf7e, 
		0x1f30, 
		0xd220, 
		0x4000, 
		0x4e80, 
		0x4c00, 
		0xd320, 
		0xd200, 
		0x53c0, 
		0xd0c0, 
		0x53e0, 
		0xe38d, 
		0x5240, 
		0xd100, 
		0xcd00, 
		0x4dc0, 
		0x7c00, 
		0x7c00, 
		0x5380, 
		0x5080, 
		0xcc40, 
		0xeef5, 
		0x4800, 
		0xc880, 
		0xd180, 
		0x4d00, 
		0x6269, 
		0xd120, 
		0xcb80, 
		0xae98, 
		0x7eba, 
		0x7ee2, 
		0xd100, 
		0xd220, 
		0x5280, 
		0xc7f2, 
		0x8000, 
		0x8278, 
		0xbcee, 
		0xfc00, 
		0x5180, 
		0xd100, 
		0xd060, 
		0xcd40, 
		0x0000, 
		0xd020, 
		0x4dc0, 
		0x7c00, 
		0xc400, 
		0xc400, 
		0x53a0, 
		0x53a0, 
		0x5200, 
		0xa50d, 
		0x3b19, 
		0x5180, 
		0x26e8, 
		0xc000, 
		0xc880, 
		0xcc40, 
		0x6b80, 
		0x4d80, 
		0xcdc0, 
		0xd1c0, 
		0x2c88, 
		0x33aa, 
		0x0675, 
		0x4e80, 
		0x5000, 
		0x4700, 
		0x5040, 
		0x5220, 
		0xca80, 
		0x50a0, 
		0xc400, 
		0xd0a0, 
		0xd2e0, 
		0x0000, 
		0xe973, 
		0x5b87, 
		0x4700, 
		0x4e00, 
		0x53a0, 
		0x4000, 
		0xa1e1, 
		0xc800, 
		0x45e5, 
		0x2638, 
		0xd020, 
		0x5080, 
		0x8000, 
		0xcb80, 
		0x7c00, 
		0x4c40, 
		0x8329, 
		0xe6cb, 
		0xaeca, 
		0x4880, 
		0x0000, 
		0xd220, 
		0x34aa, 
		0x4a80, 
		0xce80, 
		0xd300, 
		0xae4d, 
		0x0dbb, 
		0x53e0, 
		0x5160, 
		0x5240, 
		0xe360, 
		0xd080, 
		0xfc00, 
		0xea43, 
		0xcec0, 
		0xba4d, 
		0x462c, 
		0xa92d, 
		0x33ae, 
		0x8000, 
		0xce80, 
		0xc900, 
		0x4980, 
		0xd2c0, 
		0xd220, 
		0xd2c0, 
		0x4ec0, 
		0xc600, 
		0xd1c0, 
		0x5320, 
		0x46df, 
		0xc500, 
		0xd080, 
		0xd2a0, 
		0xc700, 
		0xd020, 
		0x44ee, 
		0x5140, 
		0x8000, 
		0x4fc0, 
		0x51e0, 
		0x31fe, 
		0xd2c0, 
		0xc800, 
		0xcb80, 
		0x5220, 
		0xfcc3, 
		0x50a0, 
		0xd2e0, 
		0x0916, 
		0xfd73, 
		0xd340, 
		0x9135, 
		0x4e00, 
		0x5220, 
		0x4ec0, 
		0x5300, 
		0xfc00, 
		0xd380, 
		0x4ba5, 
		0xd0a0, 
		0xd100, 
		0x118b, 
		0xcc00, 
		0x4c80, 
		0xa8a6, 
		0xfd8c, 
		0xd3c0, 
		0x7c00, 
		0x2057, 
		0xcec0, 
		0x50c0, 
		0xa579, 
		0x4d40, 
		0xc186, 
		0x3405, 
		0x0000, 
		0xd0a0, 
		0xd080, 
		0x53a0, 
		0xbb98, 
		0xd2c0, 
		0xce80, 
		0x4000, 
		0x5b26, 
		0x5360, 
		0xc700, 
		0xd040, 
		0x4f80, 
		0xd1a0, 
		0xc900, 
		0x5b35, 
		0x8000, 
		0xd811, 
		0x8f82, 
		0x19b8, 
		0x4dc0, 
		0x0e8c, 
		0x4cc0, 
		0xd220, 
		0x5160, 
		0x4a80, 
		0x5020  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x0e07ffff, 
		0x42080000, 
		0x42740000, 
		0xab6f4383, 
		0xb4730935, 
		0x5174c188, 
		0x0d000ff0, 
		0x00000020, 
		0x80000001, 
		0x0e00000f, 
		0xc2280000, 
		0x2b2881d8, 
		0xff800000, 
		0x00000000, 
		0x33f167de, 
		0x80011111, 
		0x00000000, 
		0xbf028f5c, 
		0x0e001fff, 
		0x4c170c71, 
		0x00000100, 
		0xbe52ced3, 
		0xe8aa6d02, 
		0x80000000, 
		0x716778ac, 
		0xb27c210a, 
		0x0e7fffff, 
		0x80800000, 
		0x00000020, 
		0x77a7639d, 
		0x0c7fc000, 
		0x7f7ffffe, 
		0x42580000, 
		0x489127a2, 
		0x40a00000, 
		0x8f7ffffd, 
		0x8f7ffffd, 
		0x7f800000, 
		0x08ce3593, 
		0x8aeb40a7, 
		0x41100000, 
		0xc2580000, 
		0xc22c0000, 
		0x0c7f8000, 
		0x0c7fff00, 
		0xc1600000, 
		0x7f800000, 
		0x4b8c4b40, 
		0x802ecb54, 
		0x0e0001ff, 
		0x40a00000, 
		0xd2fbd8f6, 
		0x0f7fffff, 
		0xc0000000, 
		0x80000000, 
		0x3ef8ddcd, 
		0x41980000, 
		0x0e000001, 
		0xd9705a12, 
		0x80000000, 
		0x00000000, 
		0x7fc00001, 
		0x03028479, 
		0x00000004, 
		0x80800003, 
		0xc2680000, 
		0x00400000, 
		0x0c7f0000, 
		0x80008000, 
		0x00000000, 
		0xff800000, 
		0xbf028f5c, 
		0x7fc00000, 
		0x481a5f19, 
		0xffbfffff, 
		0x4058d185, 
		0x0c7ff800, 
		0x00000040, 
		0x00020000, 
		0x7f953c74, 
		0x0c7f8000, 
		0x3f800001, 
		0x80000000, 
		0x42780000, 
		0x00000008, 
		0xffa9b3f1, 
		0x00000000, 
		0x00000001, 
		0xc1300000, 
		0x3b4fbb3c, 
		0xc1e00000, 
		0x42100000, 
		0xea42a9f4, 
		0x80004000, 
		0xc23c0000, 
		0xff7ffffe, 
		0x509ec4ce, 
		0xc2700000, 
		0x0d00fff0, 
		0x3f800000, 
		0x8a9e9732, 
		0x0c7fff00, 
		0xc1a80000, 
		0x4b000000, 
		0x0e00000f, 
		0xc1600000, 
		0xc2600000, 
		0xff7ffffe, 
		0x4497e611, 
		0x80000000, 
		0x35d1e9fe, 
		0x0c7fffff, 
		0x28a8264c, 
		0x064b2885, 
		0x00000000, 
		0x40e00000, 
		0xe5c0cd99, 
		0x0f7fffff, 
		0x42680000, 
		0x00800001, 
		0xba66723a, 
		0xb903ceb2, 
		0x00000000, 
		0x00200000, 
		0x00040000, 
		0x41600000, 
		0xc1d3c566, 
		0x779cf653, 
		0x0011d89c, 
		0x0c7ffe00, 
		0x80000000, 
		0x64bf1880, 
		0xb1a969b6, 
		0xc2100000, 
		0xc2300000, 
		0x41400000, 
		0xd6f9fe65, 
		0x0e3fffff, 
		0x0e38a5a8, 
		0xc1100000, 
		0xa7673f9f, 
		0xc9088745, 
		0x297ebf67, 
		0x40c00000, 
		0x908f2019, 
		0x00800001, 
		0x42400000, 
		0x2067bd84, 
		0x00800001, 
		0x8f7fffff, 
		0x00001000, 
		0xc3c59370, 
		0x80000080, 
		0xc22c0000, 
		0xf216c4d4, 
		0x80000001, 
		0x80800002, 
		0xc22c0000, 
		0xffc2fffe, 
		0x80100000, 
		0x41700000, 
		0x80020000, 
		0xaf77ccfe, 
		0xc2780000, 
		0x0e3fffff, 
		0x70b8cb3a, 
		0x80000800, 
		0x00800003, 
		0x7fb4bf7e, 
		0x427c0000, 
		0x41f00000, 
		0x80000080, 
		0x42100000, 
		0x88d52a6c, 
		0xcccccccc, 
		0x85a254b9, 
		0xbf3962b7, 
		0xc1a00000, 
		0x00400000, 
		0xff88138b, 
		0xff800000, 
		0x0c7fc000, 
		0x41500000, 
		0x0c7ffc00, 
		0xc2140000, 
		0xc2340000, 
		0xc2740000, 
		0x8e2bbf88, 
		0x00020000, 
		0xc2680000, 
		0x40e00000, 
		0x80000001, 
		0xd1bffccb, 
		0x80800001, 
		0xc25c0000, 
		0x42300000, 
		0xe6f3d637, 
		0x7f7fffff, 
		0x0034e6c5, 
		0x00800001, 
		0x8071551a, 
		0x0c7f0000, 
		0x00000000, 
		0x0c7ffffc, 
		0xc1c00000, 
		0x41d80000, 
		0x80000000, 
		0xff800001, 
		0x41f80000, 
		0xc26c0000, 
		0x86d49cb0, 
		0xff000000, 
		0x40800000, 
		0x007fffff, 
		0x80000000, 
		0x00000002, 
		0xaa5ee851, 
		0x65b4dff6, 
		0xa49b9e97, 
		0x0e1fffff, 
		0x80000000, 
		0x00400000, 
		0xc2240000, 
		0x42180000, 
		0x0c7ffc00, 
		0x80000010, 
		0x41a80000, 
		0xc1c80000, 
		0x00800000, 
		0x80800001, 
		0xc1200000, 
		0xffffffff, 
		0x4208be5d, 
		0x42540000, 
		0xe28896e9, 
		0xc26c0000, 
		0xc1e80000, 
		0x7f7ffffe, 
		0xc2100000, 
		0x425c0000, 
		0x42740000, 
		0x7f800000, 
		0xd11d231e, 
		0x0e000003, 
		0x7fa4ea57, 
		0xc21c0000, 
		0xc2040000, 
		0x00000000, 
		0x42540000, 
		0x007fffff, 
		0x0e0001ff, 
		0x0c600000, 
		0xcb000000, 
		0xc1a00000, 
		0xd0f97a08, 
		0x4bbdc3ec, 
		0x97ffd73f, 
		0x0c7ff800, 
		0x80000000, 
		0x00100000, 
		0xc0800000, 
		0x87101d53, 
		0x00000001, 
		0xffc00000, 
		0x77151fdf, 
		0xaaaaaaaa, 
		0x42340000, 
		0x0c7ffc00, 
		0xbc05fa63, 
		0x0c7e0000, 
		0xff800000, 
		0x0f7ffffe, 
		0x1d8fdbdc, 
		0xc1a00000, 
		0x00040000, 
		0x0c780000, 
		0x7fffffff, 
		0x544a8d9c, 
		0x41880000, 
		0xc25c0000, 
		0x278fd526, 
		0x80000000, 
		0x42480000, 
		0x003b74ec, 
		0x80000004, 
		0x42380000, 
		0x80002000, 
		0x41600000, 
		0x0e03ffff, 
		0x0c7ffffe, 
		0x785c5f79, 
		0xff800000, 
		0xc1c00000, 
		0x004a3fa9, 
		0x80000002, 
		0x988d6272, 
		0x5ccb65bf, 
		0x41100000, 
		0xa0a7246d, 
		0x00800003, 
		0x32c11d2e, 
		0x00000400, 
		0x42540000, 
		0xc23c0000, 
		0x80000080, 
		0x630b0dce, 
		0x5681b020, 
		0x41c80000, 
		0x0e001fff, 
		0xc22c0000, 
		0x80000000, 
		0xff7ffffe, 
		0xc2700000, 
		0xff800000, 
		0xc2040000, 
		0x42540000, 
		0x00800000, 
		0xb8d73c52, 
		0x00200000, 
		0xff7fffff, 
		0x0c7fffc0, 
		0x56c2da87, 
		0xff7ffffe, 
		0x80000000, 
		0xc21c0000, 
		0xa68b01e8, 
		0xad533add, 
		0x00800001, 
		0xc2480000, 
		0xe2aff1c3, 
		0x80000000, 
		0x61863951, 
		0xbd232fbf, 
		0xff94d6f3, 
		0x80200000, 
		0xac87b354, 
		0x7fc00000, 
		0x6cdf5b92, 
		0x80010000, 
		0x80800003, 
		0x00000000, 
		0x3f800001, 
		0x00020000, 
		0xc1700000, 
		0x00000001, 
		0x00000000, 
		0x00000000, 
		0x7f4e1582, 
		0xc2400000, 
		0xc0c00000, 
		0x4b000000, 
		0x42440000, 
		0x0e000003, 
		0x7e33b565, 
		0xc1700000, 
		0xbac1a70e, 
		0x8f7fffff, 
		0x49e2cd52, 
		0xff7ffffe, 
		0x00000000, 
		0xc1a80000, 
		0x0e000001, 
		0x42740000, 
		0x00000100, 
		0xe911c442, 
		0xc1b00000, 
		0x0c7fffe0, 
		0xf13ad8b6, 
		0x00000000, 
		0xff8f0624, 
		0xc2580000, 
		0x41e80000, 
		0xe398cf98, 
		0x41500000, 
		0x00800002, 
		0x80000001, 
		0x0e01ffff, 
		0x42400000, 
		0x00000000, 
		0x00080000, 
		0x42240000, 
		0x0f7fffff, 
		0x80000000, 
		0xc2600000, 
		0xa82308cc, 
		0xc018e394, 
		0xc23c0000, 
		0x7fb485ad, 
		0x8f7fffff, 
		0xc1c00000, 
		0x41c80000, 
		0x00000080, 
		0xdac43e7b, 
		0xffad2e90, 
		0x00000000, 
		0xc1f80000, 
		0x00011111, 
		0x1f1cea2c, 
		0x068ed6ee, 
		0x0e0003ff, 
		0xc1200000, 
		0xd3499040, 
		0xff800000, 
		0x00004000, 
		0x1329ddb9, 
		0x370844c5, 
		0x91ec4d71, 
		0x80080000, 
		0xd8e3a5e6, 
		0x41700000, 
		0x0c7fff00, 
		0xf7f09c99, 
		0x41a80000, 
		0xf815fd3d, 
		0x425e6c91, 
		0x41f80000, 
		0x80000080, 
		0xc0a00000, 
		0x80000000, 
		0xc2340000, 
		0x80000200, 
		0x0e0001ff, 
		0x42200000, 
		0xcaece569, 
		0x04475eba, 
		0xc0800000, 
		0xb69585d4, 
		0x00000002, 
		0x0e0001ff, 
		0x80000000, 
		0x80000001, 
		0x0e1fffff, 
		0x05b07c3d, 
		0x80800001, 
		0x7f7ffffe, 
		0x5274798e, 
		0x0d00fff0, 
		0x41900000, 
		0x80000002, 
		0xc0400000, 
		0x0c7fff80, 
		0x00800000, 
		0x7f800000, 
		0x007fffff, 
		0x44ef13c3, 
		0x00004000, 
		0x42540000, 
		0x422c0000, 
		0x0c7fe000, 
		0x0e003fff, 
		0x41500000, 
		0x0e000001, 
		0x19e1afb6, 
		0x41e00000, 
		0x4f8a1188, 
		0x25fc6d3e, 
		0x27dd7f16, 
		0x41b00000, 
		0xb4542094, 
		0x00800003, 
		0xbf800000, 
		0x00000000, 
		0x41e80000, 
		0xd02fd545, 
		0x00000400, 
		0x0e001fff, 
		0xbf800000, 
		0x02d0305e, 
		0x80000001, 
		0x0e00000f, 
		0x00000001, 
		0xb8b2a916, 
		0x4e1c3984, 
		0x1da4279b, 
		0xdf1fa39d, 
		0x0e003fff, 
		0xc2180000, 
		0x904964a4, 
		0x46e47244, 
		0x0e00ffff, 
		0x420c0000, 
		0x0c7fff00, 
		0x00768695, 
		0xfff560ac, 
		0xc2780000, 
		0x00000080, 
		0x42580000, 
		0x00000001, 
		0x00000040, 
		0x600415ba, 
		0x0d00fff0, 
		0x80800003, 
		0x0c7fe000, 
		0x7f800000, 
		0x00000000, 
		0xa98a05cf, 
		0xff800000, 
		0xffa34bfa, 
		0xc1500000, 
		0x80000001, 
		0xc20c0000, 
		0x0572a220, 
		0xcccccccc, 
		0x0c7f8000, 
		0x40a00000, 
		0x42040000, 
		0x00000000, 
		0xaea4aa23, 
		0x00000000, 
		0x91199a4c, 
		0x42540000, 
		0xc2080000, 
		0xc2740000, 
		0x00ffffff, 
		0x00800002, 
		0xff800000, 
		0xc1e80000, 
		0x2903b031, 
		0xc2000000, 
		0x9e6bbec8, 
		0x2aabf2a6, 
		0x80000000, 
		0x2bab2d46, 
		0xa12c4306, 
		0x7fbfffff, 
		0x01385b89, 
		0xc2400000, 
		0x80800000, 
		0x33bdb5f5, 
		0x00800003, 
		0xff800000, 
		0x694dbd00, 
		0x80000001, 
		0xbf800000, 
		0x0c7fff80, 
		0xa7b50269, 
		0xc2500000, 
		0x0c7ffffc, 
		0x80b23175, 
		0x0c7ffe00, 
		0x42280000, 
		0x00000080, 
		0x00000000, 
		0x40400000, 
		0x431a7ae5, 
		0xff7ffffe, 
		0x0e01ffff, 
		0x263f2e5b, 
		0xc1980000, 
		0x802f3ab6, 
		0x3f800001, 
		0x80ffffff, 
		0x00800002, 
		0x4b000000, 
		0x8c7d61d8, 
		0x5e8994af, 
		0x0c7ffff8, 
		0x98f448d2, 
		0x0f85f47b, 
		0x1ce2aadd, 
		0x0e03ffff, 
		0xff800000, 
		0xc22c0000, 
		0x3f028f5c, 
		0x030dea33, 
		0x0f7ffffd, 
		0x80800001, 
		0x8420e8a9, 
		0x0f7ffffc, 
		0xbf800001, 
		0x3c6df2c2, 
		0xffc00000, 
		0x00011111, 
		0x00080000, 
		0x80000001, 
		0x2a35fded, 
		0xf9a72042, 
		0x00800000, 
		0x00000001, 
		0x4b000000, 
		0xc1300000, 
		0x00001000, 
		0x0e000001, 
		0x0c7ffff8, 
		0x0c7c0000, 
		0x80000000, 
		0x0e03ffff, 
		0xff800000, 
		0x80010000, 
		0xf2f1e11c, 
		0x46361b85, 
		0xe9e5d88d, 
		0x62c0a70b, 
		0x0c7fffe0, 
		0x00000000, 
		0x80000200, 
		0x0f7ffffe, 
		0x0e00000f, 
		0x8f0a9a62, 
		0xc2240000, 
		0xc2680000, 
		0xfe85b5aa, 
		0x2631ca0b, 
		0x00000000, 
		0xc2780000, 
		0x42640000, 
		0x5d8cad88, 
		0x424c0000, 
		0x0c7fffe0, 
		0xb284fc72, 
		0x80000000, 
		0x00800001, 
		0xcccccccc, 
		0xe3f5b3b5, 
		0x41800000, 
		0x00010000, 
		0xc2540000, 
		0x00008000, 
		0x80000000, 
		0xd567628f, 
		0x7fed4797, 
		0x80000000, 
		0x80100000, 
		0x561622d0, 
		0x42380000, 
		0xc2140000, 
		0x0e00ffff, 
		0x0e0001ff, 
		0x00000000, 
		0x0c400000, 
		0x0e00000f, 
		0xc0400000, 
		0x7f800000, 
		0x0c7f8000, 
		0xc1f00000, 
		0xdce68ccb, 
		0x00800000, 
		0x41b80000, 
		0x41400000, 
		0x42300000, 
		0xec735233, 
		0x54b2f602, 
		0x80011111, 
		0x00800000, 
		0xbafb7e45, 
		0xb7444326, 
		0x41980000, 
		0x42180000, 
		0x39862b0e, 
		0xff800000, 
		0x803d5b73, 
		0x42640000, 
		0xc2380000, 
		0xc25c0000, 
		0x0e007fff, 
		0x7f800001, 
		0x420c0000, 
		0xffc00001, 
		0x00000200, 
		0xc1c00000, 
		0x00200000, 
		0x00000040, 
		0x5d15256c, 
		0x7fed6e48, 
		0xc1200000, 
		0x83435d70, 
		0x34a79fb2, 
		0xc1000000, 
		0xc7976950, 
		0x80800002, 
		0xf6f5f01e, 
		0xffa3ce13, 
		0x70dc4aa0, 
		0x422c0000, 
		0x42180000, 
		0x00ffffff, 
		0xffc00001, 
		0x0767d219, 
		0xf1fea6f6, 
		0xff800000, 
		0x80010000, 
		0xc2340000, 
		0xff912727, 
		0xc1b00000, 
		0xc0a00000, 
		0xf6d0c4c7, 
		0xeb6bdff4, 
		0x0c7f0000, 
		0xd402e590, 
		0xbf800001, 
		0xc2780000, 
		0x9f413ede, 
		0x1f30c3e8, 
		0x7f800000, 
		0x00000002, 
		0x00000000, 
		0x0c7fe000, 
		0xef501aef, 
		0x80800001, 
		0x9090a1d6, 
		0x80000000, 
		0x26b2d738, 
		0x7d4d1fb6, 
		0x0c600000, 
		0xc2000000, 
		0x0c7ffff0, 
		0x4b8c4b40, 
		0xc2040000, 
		0xcee63413, 
		0x00008000, 
		0x85e096e5, 
		0x41f80000, 
		0xff7fffff, 
		0x0064fd13, 
		0xb3e06dba, 
		0x42300000, 
		0x0e0007ff, 
		0x00020000, 
		0x425c0000, 
		0x0c7fffc0, 
		0x40800000, 
		0x0e0003ff, 
		0x0c7ffffe, 
		0x10e4acad, 
		0x00000000, 
		0x80000008, 
		0xffbfffff, 
		0xf4a35a15, 
		0x00010000, 
		0x4b8c4b40, 
		0xc1f80000, 
		0xaaaaaaaa, 
		0xc2080000, 
		0x80260495, 
		0xad08ef26, 
		0xf4fe4ec8, 
		0x109c6f2f, 
		0x0e000003, 
		0x00400000, 
		0x00000800, 
		0x84ca2898, 
		0x41da913b, 
		0x566f49f8, 
		0x79b5c219, 
		0x41a80000, 
		0xc2400000, 
		0xda128d8b, 
		0x41100000, 
		0xc0400000, 
		0x4b000000, 
		0x0c7c0000, 
		0xffbc8ada, 
		0x42500000, 
		0x1baadfb5, 
		0xbff9ce44, 
		0xb4b804d8, 
		0x42540000, 
		0xe6bdc13d, 
		0xc1d00000, 
		0x00800002, 
		0xb3c675db, 
		0x0e03ffff, 
		0x0c7ffff0, 
		0x00000000, 
		0xff800001, 
		0x80800003, 
		0x0e007fff, 
		0x422c0000, 
		0x42300000, 
		0x80000000, 
		0x80000008, 
		0xc2480000, 
		0x40a00000, 
		0x00010000, 
		0xc0a00000, 
		0x80000020, 
		0x0e07ffff, 
		0x00752ef1, 
		0x32909ab0, 
		0xbd31f464, 
		0x4e9892ed, 
		0xc3a74770, 
		0x40800000, 
		0xc89f0364, 
		0x80000000, 
		0x80000002, 
		0x7fbfffff, 
		0xb2f34c02, 
		0x42000000, 
		0xc22c0000, 
		0xff800000, 
		0x00000010, 
		0x7f7ffffe, 
		0x7f000000, 
		0x64087242, 
		0x68b3e306, 
		0x0ce878bd, 
		0x2f40177e, 
		0x53a9e4f0, 
		0x8f7ffffc, 
		0x00400000, 
		0xa3a6624d, 
		0x3ed4a80d, 
		0x80800001, 
		0xa2c2877f, 
		0x7ce24404, 
		0x9e157219, 
		0xc21c0000, 
		0x80000000, 
		0xff800000, 
		0x00000008, 
		0x8f7ffffd, 
		0x0e003fff, 
		0xc2200000, 
		0x7fec9928, 
		0xa37f098e, 
		0x0e001fff, 
		0x00002000, 
		0xc1400000, 
		0x80737441, 
		0x00010000, 
		0x41300000, 
		0xff800000, 
		0x00000002, 
		0xff800000, 
		0x80011111, 
		0x80800003, 
		0x000ba4a8, 
		0xac0e7eaa, 
		0xc27c0000, 
		0x41d80000, 
		0x00000001, 
		0x42100000, 
		0x40000000, 
		0x00800001, 
		0xc0800000, 
		0xbf800000, 
		0x2058f06c, 
		0x0e007fff, 
		0x00800000, 
		0xc1100000, 
		0xc21c0000, 
		0x0c7fff80, 
		0x80800002, 
		0xc77c7e77, 
		0xc1880000, 
		0x41e80000, 
		0x80000000, 
		0x00800003, 
		0xc1b00000, 
		0x1bffbb75, 
		0x00020000, 
		0x8f7ffffd, 
		0x00020000, 
		0x422c0000, 
		0x0c7f8000, 
		0xd4f881c9, 
		0x12f7abb4, 
		0x0f7fffff, 
		0x42340000, 
		0x3f800001, 
		0x08f05a1d, 
		0x0f7ffffc, 
		0x0e3fffff, 
		0x4a6b79ba, 
		0xc1f80000, 
		0x7fc00000, 
		0x0f7ffffd, 
		0xff800000, 
		0x80000010, 
		0xc1700000, 
		0x0c7fffff, 
		0x45987d9d, 
		0x41500000, 
		0x42580000, 
		0xc2200000, 
		0x0d000ff0, 
		0xc20c0000, 
		0x0c7fff80, 
		0x425c0000, 
		0xc2740000, 
		0xf9201627, 
		0x0f3fb3d7, 
		0xa9dfa904, 
		0x7fbfffff, 
		0x0e7fffff, 
		0x16cd275e, 
		0x80000000, 
		0x41c80000, 
		0xf45af4ae, 
		0x0c847ab4, 
		0x20ee2bb5, 
		0x92248a26, 
		0x0c7fffff, 
		0x212666cb, 
		0x00800000, 
		0x4643e219, 
		0x00800001, 
		0xad847ac8, 
		0xc2140000, 
		0x1483f434, 
		0x41d80000, 
		0xff800000, 
		0x00800000, 
		0x97fab25e, 
		0xec961c80, 
		0x80002000, 
		0xff800000, 
		0x80800001, 
		0xc2700000, 
		0xc20c0000, 
		0x42080000, 
		0x9e62ae5c, 
		0xc1600000, 
		0xc2040000, 
		0x80000000, 
		0xbf028f5c, 
		0x0fde564e, 
		0x41c80000, 
		0x41400000, 
		0x4b000000, 
		0x0e0000ff, 
		0x7fffffff, 
		0x8d9ef23f, 
		0x34fefe62, 
		0xff7ffffe, 
		0x3f800001, 
		0xf19063be, 
		0x10cf9e05, 
		0xc0800000, 
		0x42740000, 
		0x523189c9, 
		0x00800000, 
		0x07f32754, 
		0xc0a00000, 
		0xbcf89aea, 
		0xc1e80000, 
		0xc1d80000, 
		0x42300000, 
		0xc2640000, 
		0xff800001, 
		0x42200000, 
		0x267659bd, 
		0x7f800000, 
		0x80800000, 
		0x80000000, 
		0x7cb6e38a, 
		0x344283c1, 
		0x61bdc215, 
		0xc1500000, 
		0x00000000, 
		0x00020000, 
		0xc20c0000, 
		0xc24c0000, 
		0x40a00000, 
		0x00800003, 
		0x41500000, 
		0xc4b504f0, 
		0xc2780000, 
		0x40400000, 
		0x3ecc6cb8, 
		0x00010000, 
		0x552e8f19, 
		0x964c2e45, 
		0x42080000, 
		0x00800003, 
		0xff7ffffe, 
		0x7f800000, 
		0xff8cef4e, 
		0x00000000, 
		0xc1a00000, 
		0x38403572, 
		0x41100000, 
		0x7fc00001, 
		0x40800000, 
		0x8f7ffffc, 
		0x42340000, 
		0x42000000, 
		0x856cc7a0, 
		0x42040000, 
		0x00000040, 
		0xa5a9e9c7, 
		0x427c0000, 
		0x424c0000, 
		0x0e000003, 
		0x80000000, 
		0x42f00795, 
		0x00000000, 
		0x42000000, 
		0x40a00000, 
		0x80000001, 
		0x42580000, 
		0xc2740000, 
		0xf62d78b6, 
		0x00800000, 
		0x42500000, 
		0xffc00001, 
		0x4b02f402, 
		0x00000000, 
		0x7f7ffffe, 
		0x641148e9, 
		0x0e0000ff, 
		0x0c7fe000, 
		0xcccccccc, 
		0x2cef2060, 
		0xdd599ac2, 
		0x80000040, 
		0x80800001, 
		0x0c7fc000, 
		0x00000000, 
		0x00008000, 
		0x826da52f, 
		0x40000000, 
		0xff8da51a, 
		0x00000002, 
		0xff7fffff, 
		0x5e5de567, 
		0xf72ae89c, 
		0xf279edcb, 
		0xc0e00000, 
		0x48bd13b2, 
		0x7f7fffff, 
		0xc1d80000, 
		0x23e62d66, 
		0xc21c0000, 
		0xc1e80000, 
		0x80000004, 
		0x42280000, 
		0xc1400000, 
		0x42140000, 
		0x00008000, 
		0x7f800000, 
		0xc0800000, 
		0x007fffff, 
		0x0c7ffffc, 
		0xc1600000, 
		0x00800003, 
		0x0f7ffffe, 
		0xc2600000, 
		0x9003047a, 
		0xc2540000, 
		0x8e68157b, 
		0x42000000, 
		0x41200000, 
		0x0e0003ff, 
		0x80800000, 
		0x0e0007ff, 
		0x0e003fff, 
		0x0cfa119d, 
		0x00ffffff, 
		0x80000020, 
		0xffc00000, 
		0x427c0000, 
		0x80800003, 
		0x7fbfffff, 
		0xff800000, 
		0x7b17e962, 
		0x7b9b651d, 
		0x19867313, 
		0xeda2ec78, 
		0xc0e00000, 
		0x463bf3bd, 
		0x00000001, 
		0xc2380000, 
		0x80800003, 
		0x8edf8d83, 
		0x80000000, 
		0x0f7ffffd, 
		0xff000000, 
		0x8f7fffff, 
		0x00000000, 
		0xef5144b3, 
		0xff800000, 
		0xc2500000, 
		0x0c7ffff0, 
		0x42200000, 
		0x00010000, 
		0x00008000, 
		0x0c7fffc0, 
		0x80000002, 
		0x18112d6f, 
		0x427c0000, 
		0xff800000, 
		0x42740000, 
		0x1718863d, 
		0x80800001, 
		0x7fc00001, 
		0xbf028f5c, 
		0x007ffffe, 
		0xff7ffffe, 
		0x42740000, 
		0x7feda74a, 
		0x00011111, 
		0xc1a00000, 
		0x98afe9ac, 
		0x401bd643, 
		0xf2b78061, 
		0xc25c0000, 
		0x7fffffff, 
		0xc1a80000, 
		0xc1600000, 
		0xc1880000, 
		0x7fae8ece, 
		0xc1400000, 
		0x80002000, 
		0xa4602e63, 
		0x41600000, 
		0x80000004, 
		0x8a2e7b26, 
		0x42100000, 
		0x80800003, 
		0xff800000, 
		0xffc00001, 
		0x4b8c4b40, 
		0x005ab0f7, 
		0xd2c25837, 
		0x14f07240, 
		0x5353aabb, 
		0x0e01ffff, 
		0x2e9b26f6, 
		0x645d36e4, 
		0xcccccccc, 
		0x624e643a, 
		0x424c0000, 
		0x8e55b508, 
		0x0e00003f, 
		0x00011111, 
		0xc0800000, 
		0x64368954, 
		0x0c7ffe00, 
		0x658ba25e, 
		0xf5e134fc, 
		0x7fd00583, 
		0x48c2c38f, 
		0xc2700000, 
		0x13676b2c, 
		0x425c0000, 
		0x0e00000f, 
		0x00000010, 
		0x0f7fffff, 
		0x55555555, 
		0xad1bf12b, 
		0xa3597946, 
		0x7f9cd9aa, 
		0x00000000, 
		0x0c7ffff0, 
		0xc0400000, 
		0x0c7fc000, 
		0xc1e0d0ee, 
		0x00000002, 
		0xc1900000, 
		0xc1500000, 
		0x0e7fffff, 
		0x0c700000, 
		0x0e3fffff, 
		0x40000000, 
		0x07a0c6b0, 
		0xc0a00000, 
		0x7f800000, 
		0xcb8c4b40, 
		0x80000000, 
		0x7ecc89d3, 
		0x805d7129, 
		0x40c00000, 
		0xad287c71, 
		0x0c7fffe0, 
		0xff800000, 
		0x3f028f5c, 
		0xf95d8dde, 
		0x80000100, 
		0x0c7fff00, 
		0x00800002, 
		0x80000400, 
		0x80000040, 
		0xc4321dd9, 
		0x00800002, 
		0x40400000, 
		0x00000000, 
		0x0069f799, 
		0xffd02212, 
		0xff800000, 
		0xc1300000, 
		0x80002000, 
		0x80000000, 
		0x80800003, 
		0xc1000000, 
		0x43beda49, 
		0x42200000, 
		0xf69d7446, 
		0xd286d40a, 
		0x8f7fffff, 
		0x42480000, 
		0x80000800, 
		0x80000040, 
		0x42140000, 
		0x7f0c98ca, 
		0x00002000, 
		0xd1cc4667, 
		0x41800000, 
		0x214efa68, 
		0x7f96850b, 
		0xc23c0000, 
		0xffffffff, 
		0x80000002, 
		0x7f89efd3, 
		0x150a8095, 
		0x0e003fff, 
		0x41f80000, 
		0x80008000, 
		0x41d00000, 
		0xfff57ad2, 
		0xc1100000, 
		0x7f800000, 
		0xc1100000, 
		0x80800002, 
		0x4b000000, 
		0xc1e00000, 
		0xff58b397, 
		0xc2340000, 
		0x80008000, 
		0x42400000, 
		0x0e0007ff, 
		0x80040000, 
		0x84d5c2d7, 
		0x0c600000, 
		0xf24053b8, 
		0x52a12c46, 
		0x0c7fffe0, 
		0x0e000fff, 
		0x4fac4abf, 
		0xd52c4046, 
		0x80800000, 
		0x7fe2b10d, 
		0x0e00007f, 
		0xd15489e4, 
		0x40400000, 
		0x4a28f7da, 
		0xaff4fc92, 
		0xc25c0000, 
		0x00001000, 
		0xffc00001, 
		0x5285de31, 
		0x1f8b76d7, 
		0x00001000, 
		0x0e000fff, 
		0xcc6cbf8a, 
		0x33333333, 
		0x7f800000, 
		0x10768c0a, 
		0x4cf204a4, 
		0x42400000, 
		0x411ff815, 
		0x8f7ffffc, 
		0xc1000000, 
		0xff800000, 
		0xccb6ab2d, 
		0x80800003, 
		0x602ae7ba, 
		0xc2140000, 
		0x80000004, 
		0x00000000, 
		0x2d973ace, 
		0x426c0000, 
		0xbf800001, 
		0x0c7fffe0, 
		0x007fffff, 
		0x00000010, 
		0x00000002, 
		0x55525118, 
		0x5d3bc302, 
		0xfa10a857, 
		0x0e1fffff, 
		0xc1f00000, 
		0x87630a83, 
		0x007ffffe, 
		0x377c0f73, 
		0x80000000, 
		0xff800000, 
		0x42540000, 
		0xc1980000, 
		0xc1f80000, 
		0x0c7fffe0, 
		0xabc2d091, 
		0x80000000, 
		0x0e00007f, 
		0xffb61aeb, 
		0xffae426b, 
		0xc22c0000, 
		0x00000001, 
		0x00002000, 
		0xff8d8142, 
		0x1cc3fe5a, 
		0x77b1d28f, 
		0x42440000, 
		0x41200000, 
		0xc2600000, 
		0x0e00003f, 
		0x00400000, 
		0x0e00ffff, 
		0xc2340000, 
		0x41400000, 
		0xc2640000, 
		0xff7ffffe, 
		0x00800001, 
		0x41a80000, 
		0x4b8c4b40, 
		0xcccccccc, 
		0xa76fe339, 
		0xc1c80000, 
		0x7373fe09, 
		0x00000040, 
		0xc1200000, 
		0x80800001, 
		0x4b971784, 
		0x7f89dd12, 
		0x80000200, 
		0x80000000, 
		0x0f7ffffe, 
		0xff800001, 
		0xa7955ff0, 
		0x237cf1e6, 
		0x3b073dd3, 
		0xff7ffffe, 
		0xddbc6233, 
		0x7fbfffff, 
		0x00800000, 
		0x00002000, 
		0x7f7ffffe, 
		0x7fe96f59, 
		0x7f000000, 
		0xa4790141, 
		0xc1500000, 
		0xc0800000, 
		0xd0061c88, 
		0x0e00000f, 
		0xa3acaabe, 
		0x3258aa26, 
		0x80000400, 
		0xc0400000, 
		0xeaad8840, 
		0x00000000, 
		0xfc082ec1, 
		0x60aa3a27, 
		0xc2200000, 
		0x80000001, 
		0xff800001, 
		0xc2300000, 
		0x0076f8b6, 
		0x80ffffff, 
		0x7009d4d8, 
		0x486be8e1, 
		0x41d80000, 
		0xff800000, 
		0x80000000, 
		0xc26c0000, 
		0x80000000, 
		0x80000001, 
		0x0e001fff, 
		0x00400000, 
		0xc2500000, 
		0x0e00003f, 
		0x42780000, 
		0x42600000, 
		0x42500000, 
		0x42400000, 
		0xffbfffff, 
		0x00000004, 
		0x80000400, 
		0x0e000001, 
		0xc2300000, 
		0x00000000, 
		0xc2700000, 
		0x7f800000, 
		0xc0a00000, 
		0x80800003, 
		0x00000000, 
		0xe50fda7c, 
		0x80000000, 
		0xc2380000, 
		0x7f7ffffe, 
		0x00000400, 
		0x41a00000, 
		0xaaaaaaaa, 
		0x7f800000, 
		0x7f800000, 
		0x27e2ca69, 
		0x00000000, 
		0x0c7ffc00, 
		0x007fffff, 
		0x41880000, 
		0x1786063b, 
		0xff7ffffe, 
		0xff92385b, 
		0xa2248a2a, 
		0xd3f780f5, 
		0x80011111, 
		0x80000002, 
		0x7f800000, 
		0x42640000, 
		0x00230757, 
		0x7261ac79, 
		0xff81c0b8, 
		0x42180000, 
		0xd7d49da9, 
		0x807fffff, 
		0xdea14c45, 
		0xffbfffff, 
		0xbaefbaa4, 
		0x1ccbd30e, 
		0x00000800, 
		0xc0a00000, 
		0x00800002, 
		0xc2040000, 
		0x02594da8, 
		0x80000000, 
		0xc1700000, 
		0x7f9dc7b4, 
		0x00020000, 
		0x41a80000, 
		0x4b000000, 
		0x730b0833, 
		0xc1e00000, 
		0x82f92225, 
		0x8c37ddc6, 
		0xc0c00000, 
		0xc1100000, 
		0x80000000, 
		0x0e00001f, 
		0xff7fffff, 
		0x55555555, 
		0x423c0000, 
		0x71404dbd, 
		0x0e00001f, 
		0xace6a006, 
		0x0c780000, 
		0xff7ffffe, 
		0x690fe72a, 
		0x41500000, 
		0x80000000, 
		0x0e00000f, 
		0x00000020, 
		0x7fc00000, 
		0x7f920cc3, 
		0x0e0003ff, 
		0xff1a12de, 
		0xe3765ab2, 
		0xc0e00000, 
		0x41100000, 
		0x00800001, 
		0xc232ae0e, 
		0x0c7fffe0, 
		0x423c0000, 
		0x00000000, 
		0x0c7c0000, 
		0x36b644bc, 
		0x80000001, 
		0x80000002, 
		0xcb59717f, 
		0x1d2000f4, 
		0xc2180000, 
		0x7ffcb9f9, 
		0x79429fc1, 
		0x421c0000, 
		0xb8140a50, 
		0x80000800, 
		0xff800000, 
		0xc2300000, 
		0x7f800000, 
		0x40a00000, 
		0x0e000007, 
		0xff800000, 
		0x0f7ffffc, 
		0x41700000, 
		0xfff72862, 
		0x247afbce, 
		0x80000002, 
		0x80000000, 
		0xc1e00000, 
		0x00000008, 
		0x0e00001f, 
		0x0c7c0000, 
		0xc1800000, 
		0x7fc00001, 
		0x2ae64c5e, 
		0x00020000, 
		0x0b96287c, 
		0x16a3f449, 
		0x3e0574fd, 
		0x0c7ffff0, 
		0x725638f3, 
		0x00000000, 
		0x3d8f2317, 
		0x80800002, 
		0x41500000, 
		0x40400000, 
		0x0e03ffff, 
		0xe7fd948d, 
		0x0c7fc000, 
		0xc1d00000, 
		0x80000001, 
		0xd3f641d7, 
		0x42040000, 
		0xbf800000, 
		0x0e001fff, 
		0x00800001, 
		0xee2140e7, 
		0xc24c0000, 
		0x7a71a90d, 
		0xfa377f52, 
		0xc1980000, 
		0x00800003, 
		0x2ccfcc0d, 
		0xffaa8dae, 
		0x8f7fffff, 
		0x80800002, 
		0x41900000, 
		0x00800000, 
		0xad1fc0d3, 
		0x00000001, 
		0x0c7ff800, 
		0xacbbdd27, 
		0x6f8a41fb, 
		0x82db0cf9, 
		0x7ddab383, 
		0x00800003, 
		0x80000002, 
		0x00004000, 
		0x42700000, 
		0x80000800, 
		0x42440000, 
		0x42640000, 
		0xff7fffff, 
		0x80000000, 
		0x2e0a45b6, 
		0x8e4e94be, 
		0x80000000  
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
		0x000003f5, 
		0x000000d5, 
		0x000000c5, 
		0x000000fd, 
		0x00000227, 
		0x000003ab, 
		0x0000012b, 
		0x0000018f, 
		0x00000167, 
		0x000001d9, 
		0x000001a7, 
		0x000003c7, 
		0x00000269, 
		0x00000301, 
		0x00000363, 
		0x0000023d, 
		0x00000001, 
		0x000002b9, 
		0x00000295, 
		0x000001ed, 
		0x00000189, 
		0x00000061, 
		0x000000c3, 
		0x00000071, 
		0x00000033, 
		0x0000000b, 
		0x0000023f, 
		0x00000031, 
		0x00000015, 
		0x000002ff, 
		0x00000025, 
		0x000003cf, 
		0x00000059, 
		0x000003d7, 
		0x000001ed, 
		0x000000cf, 
		0x000003b5, 
		0x000000cf, 
		0x00000067, 
		0x0000002b, 
		0x00000141, 
		0x0000019f, 
		0x000000dd, 
		0x00000285, 
		0x00000271, 
		0x0000031b, 
		0x000001d3, 
		0x000001e5, 
		0x0000014d, 
		0x0000005b, 
		0x00000101, 
		0x00000021, 
		0x000003c9, 
		0x00000313, 
		0x000000b5, 
		0x000002e7, 
		0x000001d5, 
		0x00000375, 
		0x00000337, 
		0x000001ff, 
		0x000003b7, 
		0x0000003d, 
		0x000002a9, 
		0x000001af, 
		0x000002a3, 
		0x000003cd, 
		0x00000387, 
		0x00000321, 
		0x00000285, 
		0x00000213, 
		0x000003a1, 
		0x00000213, 
		0x00000109, 
		0x00000393, 
		0x00000249, 
		0x00000053, 
		0x000001b9, 
		0x00000295, 
		0x000003d7, 
		0x00000253, 
		0x00000207, 
		0x0000009d, 
		0x00000043, 
		0x0000035d, 
		0x00000289, 
		0x00000047, 
		0x000002b5, 
		0x00000097, 
		0x00000347, 
		0x000002b9, 
		0x000003f1, 
		0x0000025b, 
		0x0000035d, 
		0x00000135, 
		0x000003cd, 
		0x00000341, 
		0x000003b7, 
		0x00000177, 
		0x000002ff, 
		0x00000317, 
		0x0000025b, 
		0x00000363, 
		0x00000139, 
		0x00000375, 
		0x00000205, 
		0x000001df, 
		0x0000025d, 
		0x0000012b, 
		0x00000149, 
		0x000002a5, 
		0x0000008b, 
		0x00000085, 
		0x000002df, 
		0x00000357, 
		0x0000028d, 
		0x00000291, 
		0x000002cf, 
		0x000001d3, 
		0x000001d1, 
		0x0000023f, 
		0x000000cd, 
		0x000003ab, 
		0x0000010d, 
		0x0000029f, 
		0x000002bf, 
		0x00000165, 
		0x0000038d, 
		0x000001d3, 
		0x00000239, 
		0x00000195, 
		0x000002e1, 
		0x00000015, 
		0x00000371, 
		0x000001bf, 
		0x00000249, 
		0x000001c1, 
		0x0000032d, 
		0x00000099, 
		0x0000015f, 
		0x0000036b, 
		0x000000db, 
		0x000000ed, 
		0x000003d3, 
		0x000000b1, 
		0x00000141, 
		0x0000025f, 
		0x00000069, 
		0x0000025d, 
		0x000001f3, 
		0x00000323, 
		0x0000000d, 
		0x000002d9, 
		0x000000f1, 
		0x000001cf, 
		0x00000289, 
		0x00000321, 
		0x00000153, 
		0x000000c9, 
		0x00000397, 
		0x000002b7, 
		0x00000233, 
		0x000000db, 
		0x00000325, 
		0x0000021d, 
		0x00000175, 
		0x00000197, 
		0x00000399, 
		0x000002cd, 
		0x000001db, 
		0x0000030d, 
		0x0000035f, 
		0x00000377, 
		0x00000231, 
		0x00000039, 
		0x00000285, 
		0x00000195, 
		0x000000a9, 
		0x00000175, 
		0x00000251, 
		0x0000030d, 
		0x00000377, 
		0x000001ff, 
		0x0000026b, 
		0x00000117, 
		0x0000025d, 
		0x000001ef, 
		0x00000155, 
		0x000003cf, 
		0x000002f5, 
		0x000002f9, 
		0x00000111, 
		0x000003ad, 
		0x000002c3, 
		0x00000233, 
		0x000003bd, 
		0x00000093, 
		0x00000341, 
		0x000003c3, 
		0x000001ff, 
		0x0000006f, 
		0x000003c3, 
		0x000002df, 
		0x000003ed, 
		0x00000117, 
		0x00000093, 
		0x000001b9, 
		0x00000307, 
		0x0000023f, 
		0x0000016d, 
		0x000002df, 
		0x0000012d, 
		0x000000cd, 
		0x00000283, 
		0x000001c3, 
		0x000002b7, 
		0x00000009, 
		0x0000015b, 
		0x000002e1, 
		0x00000155, 
		0x0000010b, 
		0x000000a9, 
		0x000001e7, 
		0x000002e1, 
		0x0000030f, 
		0x000003ef, 
		0x000000c5, 
		0x0000030b, 
		0x00000041, 
		0x00000093, 
		0x000001bf, 
		0x000001c7, 
		0x0000026b, 
		0x000003e5, 
		0x000002ed, 
		0x000003fb, 
		0x00000259, 
		0x000002f7, 
		0x0000035d, 
		0x00000061, 
		0x0000003b, 
		0x000002a9, 
		0x00000177, 
		0x000001a9, 
		0x0000030d, 
		0x0000039b, 
		0x000002cb, 
		0x0000004d, 
		0x00000393, 
		0x0000012f, 
		0x00000171, 
		0x0000036f, 
		0x00000387, 
		0x00000217, 
		0x00000351, 
		0x000000a3, 
		0x00000079  
};

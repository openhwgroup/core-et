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
			"li x5, 482\n"
			"faddi.pi f2, f0, 60\n"
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
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
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
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
			"li x0, 0x32\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 992(x7)\n"
			"fcvt.wu.s x11, f20, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 384(x11)\n"
			"flw.ps f3, 320(x11)\n"
			"fsll.pi f14, f8, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 320(x17)\n"
			"flw.ps f11, 320(x17)\n"
			"fmul.pi f10, f19, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 864(x26)\n"
			"fcvt.pw.ps f18, f26, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m6, x26, 0x38\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 256(x7)\n"
			"flw.ps f15, 352(x7)\n"
			"fsgnj.s f18, f26, f15\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskor m0, m1, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6a1debfa5c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"fltm.pi m6, f15, f28\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x77a97ea7cb\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 128(x20)\n"
			"flw.ps f14, 64(x20)\n"
			"feq.s x16, f28, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 96(x17)\n"
			"ffrc.ps f26, f17\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 512(x7)\n"
			"fcvt.wu.s x26, f18, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 736(x17)\n"
			"flw.ps f19, 128(x17)\n"
			"fsgnjn.ps f20, f29, f19\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 256(x16)\n"
			"flw.ps f4, 352(x16)\n"
			"fminu.pi f29, f10, f4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 288(x17)\n"
			"flw.ps f14, 928(x17)\n"
			"flw.ps f15, 704(x17)\n"
			"fnmadd.ps f20, f20, f14, f15, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 480(x7)\n"
			"flw.ps f7, 416(x7)\n"
			"for.pi f26, f25, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 800(x5)\n"
			"frsq.ps f10, f11\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_113886_HID_0:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x2 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x6 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 896(x11)\n"
			"flw.ps f8, 480(x11)\n"
			"fmadd.s f4, f17, f17, f8, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 192(x11)\n"
			"flw.ps f14, 608(x11)\n"
			"fsgnjn.ps f26, f29, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_0:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 160(x19)\n"
			"frcp.ps f25, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"maskor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f8, 256(x5)\n"
			"flw.ps f20, 320(x5)\n"
			"fmul.pi f8, f8, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 320(x7)\n"
			"fsgnjn.ps f19, f25, f25\n" 
			"fbci.pi f3, 0x6e054\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 288(x12)\n"
			"flw.ps f14, 864(x12)\n"
			"fle.pi f20, f18, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 928(x17)\n"
			"flw.ps f25, 448(x17)\n"
			"fsgnjx.s f3, f26, f25\n" 
			"fbci.pi f20, 0x839f\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 672(x28)\n"
			"fsrli.pi f19, f7, 0xd\n" 
			"li x5, 0x8161\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 672(x28)\n"
			"flw.ps f14, 608(x28)\n"
			"flt.ps f14, f8, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_0:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 832(x17)\n"
			"fexp.ps f10, f4\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f20, 768(x12)\n"
			"fnot.pi f7, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 288(x7)\n"
			"ffrc.ps f18, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 960(x28)\n"
			"flw.ps f17, 800(x28)\n"
			"flw.ps f19, 640(x28)\n"
			"fnmsub.ps f15, f7, f17, f19, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x6 << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TS_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f20,  192 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 96(x18)\n"
			"flw.ps f11, 256(x18)\n"
			"fmul.ps f23, f8, f11, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 320(x23)\n"
			"flw.ps f4, 672(x23)\n"
			"fle.s x17, f20, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 512(x12)\n"
			"flw.ps f24, 736(x12)\n"
			"fsub.ps f12, f3, f24, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f26, 544(x25)\n"
			"flw.ps f8, 352(x25)\n"
			"fsrl.pi f20, f26, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 960(x23)\n"
			"flw.ps f28, 768(x23)\n"
			"fmsub.ps f20, f30, f28, f28, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 544(x13)\n"
			"flw.ps f22, 704(x13)\n"
			"fltu.pi f11, f23, f22\n" 
			"li x18, 0x74c783c2db08385d\n"
			"fcvt.s.w f15, x18, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 512(x15)\n"
			"fswizz.ps f8, f8, 0x1a\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m2, f29, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f27, 512(x18)\n"
			"flw.ps f15, 288(x18)\n"
			"fand.pi f20, f27, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 256(x28)\n"
			"flw.ps f28, 288(x28)\n"
			"fand.pi f22, f4, f28\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 576(x11)\n"
			"flw.ps f15, 512(x11)\n"
			"fadd.pi f5, f20, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f8, 448(x28)\n"
			"flw.ps f29, 896(x28)\n"
			"for.pi f28, f8, f29\n" 
			"la x11, rand_int_32\n"
			"flw.ps f12, 32(x11)\n"
			"flw.ps f26, 352(x11)\n"
			"fxor.pi f24, f12, f26\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 192(x6)\n"
			"flog.ps f26, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 544(x28)\n"
			"fround.ps f23, f5, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f22, 320(x17)\n"
			"flw.ps f30, 576(x17)\n"
			"flt.pi f28, f22, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 128(x28)\n"
			"flw.ps f4, 480(x28)\n"
			"fadd.pi f11, f23, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 96(x18)\n"
			"frsq.ps f4, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 576(x25)\n"
			"flw.ps f29, 288(x25)\n"
			"flw.ps f27, 512(x25)\n"
			"fmsub.ps f5, f28, f29, f27, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"feqm.ps m2, f26, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 608(x12)\n"
			"flw.ps f3, 96(x12)\n"
			"fadd.s f12, f22, f3, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 96(x6)\n"
			"fcvt.f16.ps f24, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 256(x12)\n"
			"fclass.s x12, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_1:\n"
			"maskor m7, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f30, 64(x12)\n"
			"flw.ps f15, 288(x12)\n"
			"fand.pi f12, f30, f15\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 448(x13)\n"
			"flw.ps f22, 384(x13)\n"
			"fsll.pi f15, f20, f22\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 896(x17)\n"
			"fsrai.pi f22, f24, 0xd\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"maskxor m0, m7, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_1:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x62272bc8e1\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f30, 416(x9)\n"
			"flw.ps f24, 32(x9)\n"
			"fsra.pi f11, f30, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 96(x9)\n"
			"frsq.ps f12, f4\n" 
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f10,  288 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 576(x11)\n"
			"flw.ps f8, 544(x11)\n"
			"flw.ps f15, 800(x11)\n"
			"fmsub.ps f1, f3, f8, f15, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 288(x10)\n"
			"flw.ps f23, 160(x10)\n"
			"fmul.ps f18, f5, f23, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 128(x23)\n"
			"fsrli.pi f20, f10, 0x6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 32(x20)\n"
			"flw.ps f15, 992(x20)\n"
			"fle.pi f6, f31, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 384(x26)\n"
			"fclass.s x18, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 896(x18)\n"
			"flw.ps f27, 736(x18)\n"
			"fadd.s f15, f15, f27, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 704(x13)\n"
			"fsrai.pi f23, f18, 0xa\n" 
			"maskpopc x6, m5\n" 
			"fbci.pi f3, 0x50cee\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f2, 448(x6)\n"
			"fcvt.ps.f16 f31, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 512(x23)\n"
			"fcvt.ps.pwu f23, f6, rtz\n" 
			"li x5, 0xd336\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_2:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 672(x13)\n"
			"flog.ps f16, f8\n" 
			"maskpopc x11, m5\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_2:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 96(x26)\n"
			"fexp.ps f16, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 448(x16)\n"
			"fmax.s f15, f8, f8\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 0(x23)\n"
			"flw.ps f3, 928(x23)\n"
			"fmulh.pi f16, f27, f3\n" 
			"li x5, 0xbaf9\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x89 << TENSOR_FMA32_SCP_LOC_B) | (0x10 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xb << TENSOR_FMA16_SCP_LOC_B) | (0xfa << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xff << TENSOR_IMA_SCP_LOC_B) | (0xfe << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 384(x13)\n"
			"fround.ps f8, f20, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 352(x26)\n"
			"flw.ps f27, 160(x26)\n"
			"fmin.pi f6, f15, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 576(x26)\n"
			"flw.ps f6, 448(x26)\n"
			"fmax.pi f9, f1, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 512(x20)\n"
			"fandi.pi f31, f9, 0xbe\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"feqm.ps m7, f30, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f23, 576(x11)\n"
			"flw.ps f8, 512(x11)\n"
			"for.pi f20, f23, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"maskxor m3, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f3, 896(x23)\n"
			"frem.pi f2, f3, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 640(x23)\n"
			"flw.ps f9, 864(x23)\n"
			"fmin.ps f3, f2, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 224(x11)\n"
			"fnot.pi f8, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 928(x10)\n"
			"frsq.ps f2, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 160(x18)\n"
			"faddi.pi f2, f2, 0x83\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 352(x20)\n"
			"flw.ps f31, 896(x20)\n"
			"fltu.pi f23, f27, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 352(x26)\n"
			"frsq.ps f9, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 928(x20)\n"
			"flw.ps f10, 672(x20)\n"
			"flw.ps f3, 224(x20)\n"
			"fmsub.ps f27, f15, f10, f3, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 576(x26)\n"
			"fsat8.pi f30, f15\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe3 << TENSOR_FMA16_SCP_LOC_B) | (0x2e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_2:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"feqm.ps m1, f27, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskor m0, m5, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 96(x26)\n"
			"flw.ps f28, 384(x26)\n"
			"fnmadd.s f8, f24, f28, f24, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_3:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 576(x11)\n"
			"frcp.ps f11, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_3:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 64(x22)\n"
			"fexp.ps f18, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 736(x19)\n"
			"flw.ps f17, 736(x19)\n"
			"flw.ps f14, 640(x19)\n"
			"fnmsub.ps f11, f23, f17, f14, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 576(x10)\n"
			"flw.ps f28, 288(x10)\n"
			"flw.ps f23, 480(x10)\n"
			"fmadd.s f5, f17, f28, f23, rmm\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f22, 0(x6)\n"
			"fcvt.ps.f16 f11, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 256(x12)\n"
			"faddi.pi f0, f13, 0x160\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 416(x20)\n"
			"fcvt.f16.ps f10, f22\n" 
			"fbci.pi f8, 0x28588\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 320(x10)\n"
			"fpackreph.pi f28, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_3:\n"
			"maskor m0, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_int_32\n"
			"flw.ps f5, 512(x11)\n"
			"flw.ps f10, 544(x11)\n"
			"fmulhu.pi f18, f5, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 544(x10)\n"
			"flw.ps f11, 736(x10)\n"
			"fmul.pi f19, f22, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 192(x20)\n"
			"flw.ps f13, 192(x20)\n"
			"flw.ps f22, 128(x20)\n"
			"fnmsub.ps f17, f18, f13, f22, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 448(x20)\n"
			"flw.ps f22, 224(x20)\n"
			"fsub.ps f14, f17, f22, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_3:\n"
			"fsetm.pi m5, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f0, 928(x12)\n"
			"flw.ps f18, 832(x12)\n"
			"fmulh.pi f27, f0, f18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_3:\n"
			"fltm.pi m0, f0, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f19, 576(x11)\n"
			"flw.ps f22, 384(x11)\n"
			"fxor.pi f27, f19, f22\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 800(x11)\n"
			"fcvt.f16.ps f24, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 704(x20)\n"
			"frsq.ps f11, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 512(x11)\n"
			"fcvt.wu.s x10, f8, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 768(x19)\n"
			"flw.ps f27, 0(x19)\n"
			"fremu.pi f10, f0, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 224(x16)\n"
			"flw.ps f23, 704(x16)\n"
			"flt.ps f24, f13, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 672(x22)\n"
			"flw.ps f19, 0(x22)\n"
			"fadd.ps f24, f17, f19, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 256(x10)\n"
			"flw.ps f24, 64(x10)\n"
			"fmax.ps f22, f23, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 416(x6)\n"
			"flw.ps f11, 928(x6)\n"
			"feq.ps f13, f28, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 320(x16)\n"
			"flw.ps f20, 608(x16)\n"
			"fsub.pi f24, f28, f20\n" 
			"LBL_SEQID_1_FP_TRANS_ID_7_HID_3:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 64(x20)\n"
			"fexp.ps f23, f10\n" 
			"fbci.pi f27, 0x62374\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
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
			"la x6, rand_int_32\n"
			"flw.ps f17, 0(x6)\n"
			"fsatu8.pi f0, f17\n" 
			"li x5, 0xdafe\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 384(x20)\n"
			"fcvt.pw.ps f25, f17, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 352(x12)\n"
			"fcvt.wu.s x6, f23, rne\n" 
			"maskpopcz x6, m6\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 480(x27)\n"
			"faddi.pi f18, f5, 0x126\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 960(x12)\n"
			"fcvt.ps.pw f5, f5, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 896(x20)\n"
			"flw.ps f24, 96(x20)\n"
			"flw.ps f18, 384(x20)\n"
			"fnmadd.ps f25, f17, f24, f18, rmm\n" 
			"li x5, 0x31db\n"
			"csrw tensor_mask, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f19, 704(x14)\n"
			"flw.ps f0, 896(x14)\n"
			"fmax.pi f18, f19, f0\n" 
			"mova.x.m x12\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 96(x14)\n"
			"flw.ps f6, 320(x14)\n"
			"fmulh.pi f19, f24, f6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f19, 800(x20)\n"
			"flw.ps f25, 992(x20)\n"
			"fand.pi f20, f19, f25\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 768(x10)\n"
			"flw.ps f25, 192(x10)\n"
			"feq.pi f17, f29, f25\n" 
			"li x17, 0x1b17f1f80fbac9e8\n"
			"fcvt.s.w f18, x17, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 800(x5)\n"
			"fsin.ps f2, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 896(x20)\n"
			"fclass.s x17, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 96(x13)\n"
			"flw.ps f25, 832(x13)\n"
			"fmaxu.pi f23, f22, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xe826\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_4:\n"
			"maskand m6, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f5, 32(x5)\n"
			"flw.ps f6, 736(x5)\n"
			"feq.pi f25, f5, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_4:\n"
			"flem.ps m1, f2, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 800(x17)\n"
			"flw.ps f25, 928(x17)\n"
			"fmin.ps f29, f24, f25\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 672(x27)\n"
			"flw.ps f12, 128(x27)\n"
			"fmaxu.pi f2, f5, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"fsetm.pi m2, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_4:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 224(x20)\n"
			"flog.ps f26, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 576(x10)\n"
			"fpackrepb.pi f22, f23\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"flem.ps m6, f25, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f15, 96(x10)\n"
			"flw.ps f26, 544(x10)\n"
			"fadd.pi f26, f15, f26\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f24, 128(x17)\n"
			"fminu.pi f5, f24, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 960(x14)\n"
			"flw.ps f25, 768(x14)\n"
			"fmin.ps f5, f23, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 32(x17)\n"
			"fsin.ps f19, f6\n" 
			"li x5, 0xba04\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 736(x13)\n"
			"flw.ps f18, 576(x13)\n"
			"fsub.s f15, f23, f18, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 992(x14)\n"
			"flw.ps f22, 384(x14)\n"
			"fmin.ps f2, f24, f22\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 320(x27)\n"
			"fsatu8.pi f15, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf << TENSOR_FMA16_SCP_LOC_B) | (0x2c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
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
			"fbci.pi f28, 0x63884\n" 
			"mova.x.m x7\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 544(x5)\n"
			"fpackrepb.pi f10, f25\n" 
			"li x18, 0x81f0ef6de47fb12d\n"
			"fcvt.s.wu f16, x18, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 288(x12)\n"
			"flw.ps f15, 0(x12)\n"
			"flw.ps f27, 32(x12)\n"
			"fmsub.s f10, f26, f15, f27, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 480(x26)\n"
			"flw.ps f9, 224(x26)\n"
			"fadd.pi f26, f15, f9\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 544(x22)\n"
			"flw.ps f10, 256(x22)\n"
			"fle.pi f9, f15, f10\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_5:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 224(x26)\n"
			"frcp.ps f26, f15\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_5:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f17, 128(x19)\n"
			"fexp.ps f10, f17\n" 
			"fbci.pi f24, 0x40f80\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 224(x28)\n"
			"fsgnjx.ps f27, f8, f8\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 928(x12)\n"
			"flw.ps f11, 672(x12)\n"
			"fsra.pi f26, f17, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 672(x5)\n"
			"flw.ps f16, 192(x5)\n"
			"fltu.pi f0, f27, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 640(x17)\n"
			"flw.ps f17, 480(x17)\n"
			"fsgnjn.s f11, f27, f17\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 192(x13)\n"
			"flw.ps f24, 800(x13)\n"
			"flt.ps f27, f16, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 512(x22)\n"
			"fcvt.wu.s x13, f22, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 320(x28)\n"
			"flw.ps f26, 512(x28)\n"
			"fsgnjn.s f25, f28, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 960(x26)\n"
			"faddi.pi f11, f0, 0x104\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 672(x18)\n"
			"flw.ps f9, 448(x18)\n"
			"fadd.pi f27, f26, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f8, 480(x13)\n"
			"flw.ps f10, 736(x13)\n"
			"fand.pi f15, f8, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 320(x20)\n"
			"flw.ps f15, 0(x20)\n"
			"flw.ps f17, 224(x20)\n"
			"fnmsub.ps f26, f0, f15, f17, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"maskand m1, m1, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xee322dd57b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 288(x19)\n"
			"fcvt.ps.pwu f26, f9, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"feqm.ps m2, f16, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 864(x7)\n"
			"flw.ps f26, 96(x7)\n"
			"fle.s x19, f25, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 352(x5)\n"
			"flw.ps f28, 320(x5)\n"
			"flw.ps f31, 288(x5)\n"
			"fnmadd.s f11, f27, f28, f31, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 288(x22)\n"
			"fcvt.wu.s x17, f10, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f31, 192(x19)\n"
			"fslli.pi f16, f31, 0x5\n" 
			"maskpopc x28, m3\n" 
			"fbci.pi f8, 0x72e2a\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f11, 288(x22)\n"
			"fcvt.ps.f16 f8, f11\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 384(x19)\n"
			"flw.ps f11, 992(x19)\n"
			"feq.ps f10, f9, f11\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
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
			"flw.ps f20, 384(x28)\n"
			"fcvt.wu.s x16, f20, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 384(x10)\n"
			"flw.ps f15, 64(x10)\n"
			"fmax.pi f29, f29, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 32(x20)\n"
			"flw.ps f22, 992(x20)\n"
			"flw.ps f31, 160(x20)\n"
			"fnmsub.s f13, f2, f22, f31, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 96(x16)\n"
			"flw.ps f17, 352(x16)\n"
			"fsgnjx.ps f8, f15, f17\n" 
			"maskpopc x16, m1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"maskand m6, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 288(x14)\n"
			"flw.ps f22, 320(x14)\n"
			"fsgnjn.ps f15, f29, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 160(x15)\n"
			"fpackrepb.pi f17, f31\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 0(x20)\n"
			"flw.ps f11, 0(x20)\n"
			"fmax.pi f13, f25, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 960(x20)\n"
			"fcvt.pw.ps f31, f29, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 96(x20)\n"
			"flw.ps f31, 416(x20)\n"
			"fsll.pi f29, f17, f31\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 864(x20)\n"
			"flw.ps f22, 576(x20)\n"
			"fmaxu.pi f17, f24, f22\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 224(x15)\n"
			"flw.ps f11, 96(x15)\n"
			"flt.s x28, f13, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 320(x16)\n"
			"fsat8.pi f11, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 544(x28)\n"
			"flw.ps f24, 64(x28)\n"
			"fsgnjn.s f22, f31, f24\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 864(x10)\n"
			"flw.ps f25, 832(x10)\n"
			"fsll.pi f31, f31, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xe375b34c6c35c7b2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x409b3dde82b96d10\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_277378_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_669554_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0xe << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1e << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xb << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_999284_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xc << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x8 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x2 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x0 << TENSOR_LOAD_VADDR) | (0x7 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1b << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x8 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 640(x15)\n"
			"flw.ps f22, 128(x15)\n"
			"fle.ps f22, f7, f22\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 672(x15)\n"
			"fcvt.wu.s x28, f28, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 672(x14)\n"
			"fnot.pi f2, f25\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_6:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 608(x12)\n"
			"fexp.ps f22, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 64(x17)\n"
			"flw.ps f25, 320(x17)\n"
			"fxor.pi f13, f22, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 352(x15)\n"
			"flw.ps f24, 864(x15)\n"
			"flw.ps f22, 224(x15)\n"
			"fcmov.ps f7, f1, f24, f22\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 352(x28)\n"
			"flw.ps f20, 864(x28)\n"
			"fand.pi f31, f22, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 576(x16)\n"
			"flw.ps f22, 64(x16)\n"
			"fsgnjx.ps f20, f20, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f2, 896(x20)\n"
			"fpackreph.pi f20, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 480(x12)\n"
			"flw.ps f8, 768(x12)\n"
			"flw.ps f20, 480(x12)\n"
			"fmadd.ps f31, f13, f8, f20, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_6:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 800(x20)\n"
			"flog.ps f15, f17\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 160(x10)\n"
			"flw.ps f29, 480(x10)\n"
			"fmadd.s f28, f25, f25, f29, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 576(x14)\n"
			"flw.ps f31, 320(x14)\n"
			"fmin.pi f25, f29, f31\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 928(x10)\n"
			"fsrl.pi f8, f13, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"fltm.ps m7, f22, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f22, 896(x14)\n"
			"fnot.pi f8, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x0 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xe << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1c << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x2 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_6:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x7 << TENSOR_LOAD_I16_DST_START) | (0x0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xa << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x19 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xe << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f1, 352(x15)\n"
			"fsat8.pi f16, f1\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 128(x15)\n"
			"fsrai.pi f0, f12, 0x7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 224(x16)\n"
			"flw.ps f12, 512(x16)\n"
			"fmax.pi f1, f31, f12\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 864(x5)\n"
			"fnot.pi f28, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 160(x16)\n"
			"fcvt.f16.ps f28, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 320(x27)\n"
			"flw.ps f0, 640(x27)\n"
			"fmulhu.pi f8, f21, f0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 704(x16)\n"
			"fcvt.f16.ps f14, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 0(x20)\n"
			"frcp.ps f28, f1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 224(x23)\n"
			"flw.ps f12, 96(x23)\n"
			"for.pi f0, f16, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 256(x16)\n"
			"flw.ps f19, 608(x16)\n"
			"fmin.s f28, f11, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 320(x7)\n"
			"fsrli.pi f15, f28, 0xd\n" 
			"li x7, 0xa7e97745cd67b44e\n"
			"fcvt.s.l f19, x7, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 480(x7)\n"
			"flw.ps f11, 0(x7)\n"
			"fle.s x20, f14, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 96(x26)\n"
			"flw.ps f11, 192(x26)\n"
			"fmul.ps f8, f28, f11, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 992(x7)\n"
			"fmax.ps f8, f28, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 192(x23)\n"
			"flw.ps f16, 384(x23)\n"
			"fdiv.s f28, f12, f16, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f10, 224(x5)\n"
			"fsatu8.pi f31, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 960(x7)\n"
			"fslli.pi f8, f19, 0x6\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 384(x18)\n"
			"flw.ps f16, 640(x18)\n"
			"fmulhu.pi f9, f28, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 608(x7)\n"
			"fslli.pi f28, f9, 0x1\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_7:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 384(x27)\n"
			"flog.ps f1, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 128(x23)\n"
			"flw.ps f31, 576(x23)\n"
			"fmul.pi f12, f9, f31\n" 
			"maskpopc x16, m4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 800(x19)\n"
			"flw.ps f22, 448(x19)\n"
			"fxor.pi f0, f12, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 352(x5)\n"
			"flw.ps f11, 64(x5)\n"
			"fsgnjx.s f16, f28, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 896(x7)\n"
			"fsgnjx.ps f6, f14, f14\n" 
			"mova.x.m x20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 320(x15)\n"
			"flw.ps f0, 672(x15)\n"
			"fmulhu.pi f21, f28, f0\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 544(x7)\n"
			"flw.ps f10, 32(x7)\n"
			"for.pi f6, f14, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 384(x26)\n"
			"flw.ps f6, 192(x26)\n"
			"flw.ps f8, 736(x26)\n"
			"fnmsub.s f8, f19, f6, f8, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f11, 448(x16)\n"
			"fpackreph.pi f0, f11\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 64(x16)\n"
			"flw.ps f10, 32(x16)\n"
			"fsgnjn.ps f1, f12, f10\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 896(x22)\n"
			"fclass.s x11, f24\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 416(x11)\n"
			"flw.ps f31, 800(x11)\n"
			"flw.ps f4, 736(x11)\n"
			"fmadd.s f23, f7, f31, f4, rne\n" 
			"li x12, 0x3e924d198b2ae525\n"
			"fcvt.s.l f4, x12, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 512(x22)\n"
			"flw.ps f12, 128(x22)\n"
			"flw.ps f15, 0(x22)\n"
			"fcmov.ps f5, f6, f12, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 928(x12)\n"
			"flw.ps f26, 704(x12)\n"
			"fsll.pi f22, f29, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 960(x14)\n"
			"fcvt.pw.ps f24, f30, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 384(x18)\n"
			"flw.ps f24, 640(x18)\n"
			"fsgnjn.s f31, f27, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 64(x9)\n"
			"fcvt.lu.s x15, f6, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 608(x15)\n"
			"fsrai.pi f31, f15, 0x4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 736(x18)\n"
			"flw.ps f6, 256(x18)\n"
			"fcmovm.ps f29, f7, f6\n" 
			"maskpopcz x16, m3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 384(x12)\n"
			"flw.ps f6, 416(x12)\n"
			"for.pi f24, f26, f6\n" 
			"la x9, rand_int_32\n"
			"flw.ps f30, 128(x9)\n"
			"fsrli.pi f4, f30, 0xe\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 512(x18)\n"
			"flw.ps f29, 640(x18)\n"
			"fmax.ps f22, f27, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 96(x18)\n"
			"flw.ps f27, 0(x18)\n"
			"flw.ps f7, 352(x18)\n"
			"fnmadd.ps f23, f31, f27, f7, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 256(x12)\n"
			"flw.ps f26, 160(x12)\n"
			"fsgnjn.ps f22, f21, f26\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_565910_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x4 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0xd << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 640(x18)\n"
			"flw.ps f15, 960(x18)\n"
			"fsgnjn.ps f31, f27, f15\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 352(x22)\n"
			"flw.ps f29, 512(x22)\n"
			"fle.pi f23, f24, f29\n" 
			"li x5, 0xbced348b87788825\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6844eb8d37e1ef19\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 192(x12)\n"
			"ffrc.ps f15, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 800(x9)\n"
			"flw.ps f7, 576(x9)\n"
			"fsgnj.s f6, f27, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 416(x22)\n"
			"flw.ps f26, 928(x22)\n"
			"fsub.s f5, f31, f26, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 896(x14)\n"
			"flw.ps f7, 256(x14)\n"
			"fmax.s f7, f5, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_FP_TRANS_ID_0_HID_8:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 640(x12)\n"
			"fexp.ps f15, f15\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 512(x20)\n"
			"flw.ps f26, 608(x20)\n"
			"frem.pi f5, f30, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 736(x12)\n"
			"flw.ps f24, 96(x12)\n"
			"fmin.s f27, f26, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 96(x18)\n"
			"fclass.s x10, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_8:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 800(x18)\n"
			"fexp.ps f7, f24\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_8:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 608(x18)\n"
			"flog.ps f23, f31\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 576(x16)\n"
			"fsrli.pi f21, f5, 0xf\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 512(x22)\n"
			"flw.ps f21, 96(x22)\n"
			"feq.pi f26, f30, f21\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 576(x20)\n"
			"flw.ps f27, 544(x20)\n"
			"fmin.pi f29, f24, f27\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f11, 0(x10)\n"
			"fcvt.ps.f16 f15, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_63993_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xdd << TENSOR_FMA32_SCP_LOC_B) | (0x8e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_240143_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x4 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x41 << TENSOR_FMA32_SCP_LOC_B) | (0x77 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x49 << TENSOR_IMA_SCP_LOC_B) | (0x1c << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
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
			"li x22, 0x5a7d0c75b2eb6da1\n"
			"fcvt.s.l f31, x22, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 832(x18)\n"
			"flw.ps f18, 928(x18)\n"
			"fcmovm.ps f2, f2, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 0(x11)\n"
			"fround.ps f5, f18, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 512(x6)\n"
			"flw.ps f2, 384(x6)\n"
			"fmin.pi f16, f12, f2\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 768(x17)\n"
			"flw.ps f5, 832(x17)\n"
			"fsrl.pi f30, f10, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 384(x11)\n"
			"flw.ps f10, 832(x11)\n"
			"fmsub.ps f5, f3, f3, f10, rne\n" 
			"fbci.pi f30, 0x5d18e\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 480(x22)\n"
			"fswizz.ps f10, f28, 0xd\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 864(x5)\n"
			"fslli.pi f8, f12, 0xb\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 512(x6)\n"
			"flw.ps f11, 640(x6)\n"
			"fmaxu.pi f16, f17, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 480(x6)\n"
			"flw.ps f31, 384(x6)\n"
			"fsgnj.ps f31, f20, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 64(x27)\n"
			"flw.ps f2, 704(x27)\n"
			"fsub.s f2, f12, f2, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 416(x6)\n"
			"flw.ps f12, 480(x6)\n"
			"fsgnjn.ps f2, f17, f12\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 96(x5)\n"
			"flw.ps f17, 800(x5)\n"
			"for.pi f3, f10, f17\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskor m6, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x18, 0x67c04c2caaf4516c\n"
			"fcvt.s.lu f22, x18, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 192(x6)\n"
			"ffrc.ps f18, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 128(x17)\n"
			"flw.ps f12, 800(x17)\n"
			"fmul.ps f18, f2, f12, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 448(x28)\n"
			"flw.ps f22, 352(x28)\n"
			"flt.ps f3, f3, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_9:\n"
			"maskxor m3, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 128(x28)\n"
			"frcp.ps f20, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 576(x5)\n"
			"flw.ps f17, 256(x5)\n"
			"feq.pi f31, f3, f17\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 800(x18)\n"
			"fexp.ps f8, f17\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 736(x5)\n"
			"flw.ps f31, 320(x5)\n"
			"fmaxu.pi f17, f19, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"fsetm.pi m6, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_9:\n"
			"flem.ps m6, f30, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f3, 288(x11)\n"
			"flw.ps f19, 928(x11)\n"
			"fdivu.pi f18, f3, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_9:\n"
			"fltm.ps m4, f22, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x17, 0xe1427008864d21be\n"
			"fcvt.s.wu f19, x17, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 928(x17)\n"
			"flw.ps f22, 288(x17)\n"
			"fadd.ps f28, f2, f22, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 800(x27)\n"
			"flw.ps f17, 64(x27)\n"
			"fcmovm.ps f2, f5, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 288(x18)\n"
			"flw.ps f2, 480(x18)\n"
			"flw.ps f28, 224(x18)\n"
			"fnmadd.ps f16, f19, f2, f28, rtz\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
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
			"la x18, rand_int_32\n"
			"flw.ps f11, 768(x18)\n"
			"fpackreph.pi f2, f11\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 896(x12)\n"
			"flw.ps f11, 32(x12)\n"
			"flt.pi f18, f29, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 96(x14)\n"
			"flw.ps f23, 512(x14)\n"
			"fdiv.s f21, f15, f23, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 128(x18)\n"
			"flw.ps f30, 800(x18)\n"
			"flw.ps f26, 512(x18)\n"
			"fnmadd.s f19, f7, f30, f26, rdn\n" 
			"maskpopc x15, m4\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 160(x25)\n"
			"flw.ps f29, 928(x25)\n"
			"fsub.ps f19, f8, f29, rdn\n" 
			"fbci.pi f2, 0x4782f\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 64(x13)\n"
			"flw.ps f20, 256(x13)\n"
			"flw.ps f29, 992(x13)\n"
			"fmsub.ps f23, f26, f20, f29, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 736(x13)\n"
			"flw.ps f2, 544(x13)\n"
			"fltu.pi f21, f30, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 640(x18)\n"
			"flw.ps f15, 992(x18)\n"
			"fmin.s f0, f22, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 928(x23)\n"
			"fround.ps f19, f20, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fltm.pi m5, f7, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x9, m3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 768(x9)\n"
			"flw.ps f0, 64(x9)\n"
			"fmax.ps f29, f18, f0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 32(x11)\n"
			"flw.ps f20, 896(x11)\n"
			"feq.pi f26, f15, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"mov.m.x m4, x13, 0x3f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_167956_HID_10:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x6 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xc << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x9 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_123892_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd6 << TENSOR_IMA_SCP_LOC_B) | (0x8b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 320(x9)\n"
			"fswizz.ps f21, f29, 0x16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 320(x12)\n"
			"fcvt.f16.ps f21, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 576(x18)\n"
			"flw.ps f23, 704(x18)\n"
			"fmax.pi f20, f8, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_10:\n"
			"mov.m.x m5, x23, 0x2e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 160(x9)\n"
			"fsin.ps f29, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 608(x25)\n"
			"flw.ps f20, 96(x25)\n"
			"fmadd.s f15, f30, f20, f20, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 96(x14)\n"
			"fcvt.w.s x11, f30, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 608(x23)\n"
			"fnot.pi f29, f21\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 288(x23)\n"
			"flw.ps f30, 352(x23)\n"
			"fminu.pi f28, f21, f30\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 512(x13)\n"
			"flw.ps f8, 896(x13)\n"
			"fmul.pi f22, f21, f8\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 736(x19)\n"
			"flw.ps f7, 704(x19)\n"
			"fmulhu.pi f26, f0, f7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 576(x12)\n"
			"flw.ps f2, 32(x12)\n"
			"flw.ps f11, 192(x12)\n"
			"fnmadd.ps f29, f29, f2, f11, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f7, 704(x25)\n"
			"flw.ps f8, 768(x25)\n"
			"fltu.pi f30, f7, f8\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 256(x13)\n"
			"flw.ps f0, 0(x13)\n"
			"flw.ps f11, 896(x13)\n"
			"fnmadd.s f7, f19, f0, f11, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 928(x9)\n"
			"flw.ps f7, 672(x9)\n"
			"fsra.pi f19, f0, f7\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 640(x25)\n"
			"flw.ps f19, 736(x25)\n"
			"for.pi f20, f29, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_296010_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x60 << TENSOR_IMA_SCP_LOC_B) | (0xca << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5a << TENSOR_FMA16_SCP_LOC_B) | (0x88 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
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
			"la x19, rand_int_32\n"
			"flw.ps f21, 288(x19)\n"
			"flw.ps f18, 64(x19)\n"
			"fsll.pi f3, f21, f18\n" 
			"maskpopcz x15, m3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 64(x14)\n"
			"flw.ps f24, 0(x14)\n"
			"fmin.pi f26, f26, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 672(x9)\n"
			"frsq.ps f21, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 960(x17)\n"
			"flw.ps f28, 96(x17)\n"
			"flw.ps f11, 864(x17)\n"
			"fnmadd.s f20, f20, f28, f11, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 512(x25)\n"
			"flw.ps f1, 288(x25)\n"
			"fmul.s f31, f26, f1, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 416(x14)\n"
			"fandi.pi f28, f27, 0x1c6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 512(x25)\n"
			"fsat8.pi f27, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 448(x9)\n"
			"fclass.ps f9, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 960(x15)\n"
			"flw.ps f30, 96(x15)\n"
			"fsub.ps f15, f31, f30, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 800(x22)\n"
			"flw.ps f24, 448(x22)\n"
			"fsgnjx.s f20, f19, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 448(x18)\n"
			"fswizz.ps f20, f18, 0xb\n" 
			"maskpopcz x18, m7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 64(x5)\n"
			"flw.ps f22, 480(x5)\n"
			"fsub.s f30, f26, f22, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 512(x17)\n"
			"flw.ps f19, 800(x17)\n"
			"fmul.pi f15, f3, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 928(x17)\n"
			"flw.ps f30, 32(x17)\n"
			"flw.ps f31, 64(x17)\n"
			"fmsub.s f9, f26, f30, f31, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f28, 928(x15)\n"
			"flw.ps f26, 768(x15)\n"
			"feq.pi f27, f28, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 704(x14)\n"
			"flw.ps f21, 256(x14)\n"
			"flw.ps f22, 320(x14)\n"
			"fnmadd.s f27, f9, f21, f22, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 384(x23)\n"
			"flw.ps f1, 544(x23)\n"
			"fmin.ps f20, f11, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 512(x18)\n"
			"flw.ps f26, 864(x18)\n"
			"feq.s x9, f18, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 416(x22)\n"
			"flw.ps f19, 352(x22)\n"
			"for.pi f20, f28, f19\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 704(x23)\n"
			"flw.ps f31, 896(x23)\n"
			"fle.s x15, f19, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 992(x15)\n"
			"fsin.ps f21, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 64(x17)\n"
			"flw.ps f11, 128(x17)\n"
			"feq.pi f11, f26, f11\n" 
			"LBL_SEQID_1_FP_TRANS_ID_0_HID_11:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 736(x22)\n"
			"flog.ps f21, f30\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 0(x18)\n"
			"flw.ps f31, 480(x18)\n"
			"flw.ps f21, 800(x18)\n"
			"fnmadd.ps f26, f15, f31, f21, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 768(x23)\n"
			"flw.ps f27, 992(x23)\n"
			"fsub.pi f1, f21, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 640(x5)\n"
			"flw.ps f26, 320(x5)\n"
			"fmin.ps f28, f21, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 416(x9)\n"
			"flw.ps f28, 128(x9)\n"
			"fle.ps f1, f30, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 288(x19)\n"
			"fcvt.w.s x14, f22, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 480(x23)\n"
			"flw.ps f22, 768(x23)\n"
			"flt.ps f9, f27, f22\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 704(x19)\n"
			"flw.ps f3, 0(x19)\n"
			"feq.s x19, f9, f3\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 960(x15)\n"
			"flw.ps f11, 448(x15)\n"
			"fmin.s f25, f0, f11\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 928(x19)\n"
			"flw.ps f21, 288(x19)\n"
			"fle.s x17, f0, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 192(x19)\n"
			"flw.ps f13, 544(x19)\n"
			"fmax.s f7, f22, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f25, 160(x18)\n"
			"fsrli.pi f17, f25, 0x9\n" 
			"fbci.ps f29, 0x4c557\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"maskor m2, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 0(x28)\n"
			"flw.ps f9, 544(x28)\n"
			"fle.s x19, f29, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 32(x19)\n"
			"fsat8.pi f11, f7\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 736(x11)\n"
			"flw.ps f19, 928(x11)\n"
			"fmin.ps f7, f20, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 64(x20)\n"
			"flw.ps f12, 224(x20)\n"
			"flw.ps f29, 448(x20)\n"
			"fnmadd.ps f7, f17, f12, f29, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 736(x15)\n"
			"fcvt.pwu.ps f10, f0, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 288(x17)\n"
			"flw.ps f13, 992(x17)\n"
			"fmul.ps f22, f2, f13, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 640(x19)\n"
			"flw.ps f13, 640(x19)\n"
			"fmul.pi f13, f23, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 544(x20)\n"
			"flw.ps f20, 832(x20)\n"
			"fle.ps f19, f0, f20\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 608(x11)\n"
			"flw.ps f9, 928(x11)\n"
			"fsub.ps f17, f19, f9, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 320(x22)\n"
			"fcvt.f16.ps f9, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x0 << TENSOR_LOAD_T8_DST_START) | (0x0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xc << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0xf << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 448(x14)\n"
			"flw.ps f20, 384(x14)\n"
			"fmsub.s f2, f9, f20, f9, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 544(x19)\n"
			"fpackrepb.pi f17, f21\n" 
			"mova.x.m x11\n" 
			"fbci.ps f19, 0x60b20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_12:\n"
			"maskxor m7, m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"maskor m1, m1, m1\n" 
			"li x5, 0x80340160\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_int_32\n"
			"flw.ps f21, 64(x20)\n"
			"fsll.pi f2, f21, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 736(x28)\n"
			"flw.ps f7, 832(x28)\n"
			"fle.ps f9, f21, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 544(x18)\n"
			"flw.ps f13, 480(x18)\n"
			"flw.ps f9, 992(x18)\n"
			"fnmsub.ps f20, f17, f13, f9, rtz\n" 
			"li x23, 0xa4fe59d748aadfda\n"
			"fcvt.s.w f17, x23, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 32(x28)\n"
			"flw.ps f13, 768(x28)\n"
			"flw.ps f20, 384(x28)\n"
			"fmadd.ps f25, f25, f13, f20, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 384(x11)\n"
			"flw.ps f7, 672(x11)\n"
			"fsra.pi f2, f20, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 480(x18)\n"
			"flw.ps f21, 480(x18)\n"
			"fsll.pi f9, f11, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"maskxor m2, m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 864(x22)\n"
			"flw.ps f19, 128(x22)\n"
			"flw.ps f2, 0(x22)\n"
			"fcmov.ps f25, f23, f19, f2\n" 
			"li x5, 0x523797e1e2f8e9bc\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x12629328da6bf425\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 352(x19)\n"
			"flw.ps f22, 96(x19)\n"
			"fsgnj.s f9, f21, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_410572_HID_12:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_718607_HID_12:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 640(x12)\n"
			"fclass.ps f19, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 928(x25)\n"
			"flw.ps f21, 0(x25)\n"
			"flt.s x25, f3, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"maskand m7, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 352(x12)\n"
			"flw.ps f5, 576(x12)\n"
			"fmul.s f15, f28, f5, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_13:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 96(x12)\n"
			"flog.ps f25, f9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 384(x25)\n"
			"flw.ps f16, 608(x25)\n"
			"fmul.pi f9, f29, f16\n" 
			"fbci.ps f19, 0x6ad25\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 480(x17)\n"
			"fsrai.pi f15, f21, 0x6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 864(x25)\n"
			"flw.ps f28, 256(x25)\n"
			"feq.pi f3, f15, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 864(x20)\n"
			"fcvt.pw.ps f16, f31, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 0(x10)\n"
			"flw.ps f16, 192(x10)\n"
			"fsub.pi f19, f31, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"fltm.ps m5, f30, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 0(x10)\n"
			"frsq.ps f2, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_13:\n"
			"flem.ps m7, f29, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f31, 672(x6)\n"
			"flw.ps f28, 352(x6)\n"
			"fmaxu.pi f9, f31, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f25, 480(x10)\n"
			"fnot.pi f29, f25\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 320(x25)\n"
			"flw.ps f31, 800(x25)\n"
			"fmax.s f21, f3, f31\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 928(x25)\n"
			"flw.ps f16, 192(x25)\n"
			"fmax.s f30, f15, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 512(x23)\n"
			"flw.ps f29, 736(x23)\n"
			"flw.ps f30, 448(x23)\n"
			"fnmadd.s f15, f19, f29, f30, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 32(x11)\n"
			"fpackrepb.pi f15, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 672(x16)\n"
			"flw.ps f15, 416(x16)\n"
			"fnmsub.ps f25, f3, f15, f15, rup\n" 
			"fbci.pi f3, 0x107c1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 928(x27)\n"
			"flw.ps f22, 640(x27)\n"
			"feq.pi f25, f28, f22\n" 
			"mova.x.m x10\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 288(x6)\n"
			"fcvt.ps.pw f5, f16, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f22, 768(x16)\n"
			"fcvt.ps.pw f21, f22, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 416(x6)\n"
			"fcvt.ps.pw f5, f27, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 32(x20)\n"
			"flw.ps f29, 576(x20)\n"
			"fmax.pi f3, f27, f29\n" 
			"fbci.pi f7, 0x53608\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 608(x27)\n"
			"fcvt.w.s x23, f22, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 192(x6)\n"
			"flw.ps f9, 96(x6)\n"
			"fadd.pi f9, f7, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 384(x16)\n"
			"flw.ps f7, 608(x16)\n"
			"flw.ps f28, 672(x16)\n"
			"fnmadd.ps f16, f25, f7, f28, rne\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 16\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f21,  992 (x5)\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 576(x13)\n"
			"fround.ps f2, f30, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"mov.m.x m0, x20, 0x28\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f30, 352(x25)\n"
			"fcvt.ps.pwu f8, f30, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 704(x16)\n"
			"flw.ps f19, 800(x16)\n"
			"fmul.s f12, f27, f19, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 32(x17)\n"
			"flw.ps f31, 96(x17)\n"
			"fsgnjn.ps f5, f16, f31\n" 
			"li x23, 0xd6aa3bbf30304df8\n"
			"fcvt.s.w f3, x23, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 640(x17)\n"
			"flw.ps f21, 608(x17)\n"
			"fmin.ps f30, f31, f21\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f21, 608(x25)\n"
			"fcvt.ps.f16 f24, f21\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 320(x26)\n"
			"flw.ps f16, 416(x26)\n"
			"for.pi f3, f30, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 544(x14)\n"
			"fcvt.wu.s x23, f30, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"maskor m2, m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xd6027586fbdb0062\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x279ad4ae611ac3c1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f24, 256(x26)\n"
			"fsat8.pi f16, f24\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 192(x5)\n"
			"flw.ps f17, 640(x5)\n"
			"fmin.s f5, f19, f17\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 64(x20)\n"
			"fsrai.pi f16, f23, 0x5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 992(x25)\n"
			"fslli.pi f30, f17, 0x5\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 64(x16)\n"
			"flw.ps f24, 864(x16)\n"
			"fmin.ps f16, f3, f24\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_645911_HID_14:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_208453_HID_14:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_722431_HID_14:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x7 << TENSOR_LOAD_T16_DST_START) | (0x0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xd << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1f << TENSOR_LOAD_I8_DST_START) | (0x0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xa << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_14:\n"
            "la x7, rand_ieee754_32\n"    //FIX
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x0 << TENSOR_LOAD_T32_VADDR) | (0x6 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x0 << TENSOR_LOAD_SETUP_B_VADDR) | (0x1 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"add x5, x5, x7\n"    //FIX
			"add x6, x6, x7\n"    //FIX
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f23, 864(x5)\n"
			"flw.ps f12, 512(x5)\n"
			"fsub.pi f12, f23, f12\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 0(x13)\n"
			"flw.ps f27, 608(x13)\n"
			"for.pi f21, f16, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 704(x17)\n"
			"flw.ps f23, 832(x17)\n"
			"fadd.ps f24, f3, f23, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 640(x20)\n"
			"flw.ps f8, 288(x20)\n"
			"fmin.ps f30, f21, f8\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_14:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 544(x16)\n"
			"flog.ps f19, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 960(x26)\n"
			"frsq.ps f5, f30\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 352(x26)\n"
			"flw.ps f17, 320(x26)\n"
			"fminu.pi f16, f11, f17\n" 
			"li x5, 0x43aca8734f42d7f1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x779fc67756de548b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 128(x17)\n"
			"flw.ps f19, 800(x17)\n"
			"fle.s x26, f30, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 384(x22)\n"
			"fcvt.w.s x13, f31, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 512(x13)\n"
			"fswizz.ps f30, f30, 0x9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 32(x5)\n"
			"flw.ps f2, 608(x5)\n"
			"flw.ps f17, 160(x5)\n"
			"fcmov.ps f24, f12, f2, f17\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 352(x7)\n"
			"flw.ps f16, 928(x7)\n"
			"fsub.ps f21, f21, f16, rmm\n" 
			"li x20, 0xc6eb93d29aa533a6\n"
			"fcvt.s.l f30, x20, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 192(x22)\n"
			"flw.ps f24, 128(x22)\n"
			"fsgnjx.ps f5, f12, f24\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_14:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 672(x13)\n"
			"frcp.ps f31, f27\n" 
			"li x5, 0xc88b\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
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
			"csrwi mhpmevent3, 12\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
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
			"maskpopcz x25, m2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 704(x7)\n"
			"fswizz.ps f3, f3, 0x12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 992(x16)\n"
			"flw.ps f0, 800(x16)\n"
			"fle.s x20, f26, f0\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 0(x17)\n"
			"flw.ps f0, 896(x17)\n"
			"fmin.pi f26, f9, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"fltm.pi m7, f26, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x20, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 672(x14)\n"
			"flw.ps f13, 320(x14)\n"
			"flw.ps f20, 416(x14)\n"
			"fmsub.s f0, f0, f13, f20, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 96(x20)\n"
			"fsgnjn.s f19, f3, f3\n" 
			"fbci.ps f1, 0x1858f\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 384(x5)\n"
			"flw.ps f13, 96(x5)\n"
			"fsgnj.ps f0, f20, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 544(x20)\n"
			"flw.ps f1, 384(x20)\n"
			"flw.ps f19, 416(x20)\n"
			"fmsub.ps f16, f6, f1, f19, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_15:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 64(x7)\n"
			"frcp.ps f13, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 800(x20)\n"
			"flw.ps f13, 928(x20)\n"
			"fltu.pi f18, f1, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 608(x25)\n"
			"flw.ps f7, 512(x25)\n"
			"fadd.ps f18, f18, f7, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_15:\n"
			"masknot m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f26, 704(x28)\n"
			"fsra.pi f13, f26, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f16, 480(x10)\n"
			"fsat8.pi f13, f16\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 192(x28)\n"
			"flw.ps f8, 0(x28)\n"
			"fsgnj.s f7, f26, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 704(x10)\n"
			"flw.ps f7, 128(x10)\n"
			"flw.ps f20, 960(x10)\n"
			"fnmadd.ps f0, f9, f7, f20, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 288(x14)\n"
			"flw.ps f22, 32(x14)\n"
			"fle.ps f1, f31, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 224(x20)\n"
			"flw.ps f6, 736(x20)\n"
			"fsll.pi f8, f3, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 544(x25)\n"
			"flw.ps f3, 704(x25)\n"
			"fmin.s f26, f8, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 768(x26)\n"
			"flw.ps f8, 864(x26)\n"
			"feq.s x5, f19, f8\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_15:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 192(x25)\n"
			"flog.ps f3, f18\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 544(x5)\n"
			"fpackreph.pi f26, f0\n" 
			"fbci.pi f1, 0x656b5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 512(x28)\n"
			"flw.ps f18, 672(x28)\n"
			"flw.ps f31, 96(x28)\n"
			"fmadd.ps f19, f24, f18, f31, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_15:\n"
			"mov.m.x m2, x28, 0x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 800(x10)\n"
			"flw.ps f16, 800(x10)\n"
			"fnmsub.ps f16, f0, f0, f16, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 480(x17)\n"
			"flw.ps f0, 192(x17)\n"
			"flw.ps f16, 832(x17)\n"
			"fcmov.ps f20, f18, f0, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 928(x26)\n"
			"flw.ps f7, 384(x26)\n"
			"fmin.pi f1, f0, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f16, 160(x16)\n"
			"fpackreph.pi f3, f16\n" 
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
		0x8caad1e4, 
		0xf6651050, 
		0xc7354024, 
		0x1651f5ee, 
		0x124cb6a2, 
		0xbfbc4e18, 
		0x5200e5f1, 
		0x85699505, 
		0x73cb72d9, 
		0xcd1f047f, 
		0x9bd9b2fa, 
		0x7634cb92, 
		0x66fa3379, 
		0xa252f3c2, 
		0xb5ddd817, 
		0x66cf4aa5, 
		0xc5c372fe, 
		0xc6a4444f, 
		0xd4af1a32, 
		0x50aef87e, 
		0x61e297eb, 
		0x400ed484, 
		0xfd9a51cf, 
		0x2dbc2166, 
		0xc917b034, 
		0x2a2c7e41, 
		0x1bac09fc, 
		0x9d6b9003, 
		0x5b0b505c, 
		0xdb620c93, 
		0x47209c4d, 
		0x3f9ec674, 
		0x95d47855, 
		0x9f72fd89, 
		0x1bdb0cff, 
		0x1bf6f662, 
		0xc3dfb0c2, 
		0x85bac33f, 
		0xff895e8d, 
		0x3023b2f3, 
		0xab9b4678, 
		0xe584a689, 
		0x1031020d, 
		0x9a0d6cc2, 
		0x0abdee12, 
		0x80040545, 
		0x8f31269f, 
		0xf2439e9e, 
		0xd0ecab80, 
		0xa45c088f, 
		0x824109be, 
		0x91d35416, 
		0x481e4a28, 
		0xfabaa80f, 
		0x059ba054, 
		0x23982643, 
		0xf55d7790, 
		0x0b8390a1, 
		0xf4c4e97c, 
		0xe37edab8, 
		0xd1e6b09a, 
		0xf6cb64e8, 
		0x19f0acd5, 
		0x600cd9d5, 
		0x3ea0eb38, 
		0x94dd0449, 
		0x8ff8904d, 
		0x7a17d019, 
		0x2996241c, 
		0x189b7527, 
		0x7b5dc4aa, 
		0x301115ea, 
		0x3bd7584d, 
		0xeaa8ff99, 
		0xaa189524, 
		0x72b1ccf0, 
		0x5555e30f, 
		0x409273c9, 
		0xa01ca7dd, 
		0x34a7f3d8, 
		0x77769b62, 
		0xb790c199, 
		0xa15b5cf1, 
		0xdf53e8d7, 
		0xc1a36590, 
		0x61e305ba, 
		0xdfe573c3, 
		0xdc58ad07, 
		0xc87e2f87, 
		0x41f13abc, 
		0x2d06ef82, 
		0x44e359c0, 
		0x20da1f05, 
		0x16b11e13, 
		0x277db2d9, 
		0x3fb656b8, 
		0xccdf2f4d, 
		0x5d3ce9f2, 
		0x4991d5f9, 
		0x75e29a1f, 
		0xc1d46ef8, 
		0xedaad777, 
		0x853b1db2, 
		0xa17cca0a, 
		0x5b1a290d, 
		0xa50b586a, 
		0x59763318, 
		0x065227a5, 
		0x947e30ce, 
		0xcc4bb6d6, 
		0x3d6daac7, 
		0x4a38cb00, 
		0x0812c5a8, 
		0x86472158, 
		0xa7e81633, 
		0x16b567cb, 
		0x0ce93f4b, 
		0xc1fde034, 
		0x251e41dd, 
		0x98135409, 
		0x931d8e26, 
		0xf26a9707, 
		0x99d63b0b, 
		0x9fd5eaac, 
		0xb85358d6, 
		0x01764727, 
		0x745e1bb5, 
		0x1636e897, 
		0x1f4b3f06, 
		0x8621d023, 
		0xcc78da4e, 
		0x5e469d91, 
		0xb1ecd7a7, 
		0xbf281984, 
		0x1f8f3939, 
		0xf511ebdf, 
		0x6149e1bc, 
		0x741fff5e, 
		0x7edc1582, 
		0xcf67bc24, 
		0x06bd4053, 
		0x15f88b62, 
		0x638e6607, 
		0x8053ef65, 
		0x16acc917, 
		0x565e257c, 
		0x64ca7050, 
		0x50e97ff8, 
		0xc5ca93c6, 
		0x3013038e, 
		0x10f4192b, 
		0xb2419e3c, 
		0x06c8f225, 
		0x68613acb, 
		0x1cfdf1b8, 
		0xdade4e64, 
		0xd848cec6, 
		0x5f0e7f35, 
		0xf55b126d, 
		0x6e8d16c6, 
		0xfb5ae432, 
		0x50e87a9d, 
		0x9cda4ad0, 
		0x74835ef1, 
		0x506eb679, 
		0xad3bc907, 
		0x898db04e, 
		0xd3b30a54, 
		0xd6fcc7d4, 
		0xbacfc592, 
		0xf369b0a8, 
		0x97bb816c, 
		0xddc2821a, 
		0x6c4fec3a, 
		0x1cd957bc, 
		0x866e0006, 
		0x911bc245, 
		0x95564620, 
		0xf06a8f13, 
		0x5c4b36be, 
		0xfe25df3d, 
		0x4997de78, 
		0x4e56dc8c, 
		0x3ce9cf82, 
		0xe3fa2576, 
		0x38ed4c1d, 
		0xec0527e8, 
		0x52772e40, 
		0x1353b0da, 
		0xbedf6091, 
		0x3bcbfdcd, 
		0xcd533fee, 
		0x2e857a96, 
		0x84fd54da, 
		0x3d24eab1, 
		0x4bf43017, 
		0xcaf47d05, 
		0x065721b6, 
		0xf8b2d383, 
		0xb123825a, 
		0x28836818, 
		0x786389bd, 
		0x17838e5e, 
		0x0fb423d5, 
		0x0e6cce57, 
		0xc27c4029, 
		0x81ea2a96, 
		0x4cf16a76, 
		0xe54f646a, 
		0x0611a301, 
		0x3145f523, 
		0x3b4fb91f, 
		0xe658e26e, 
		0xe83d2e52, 
		0xc3e40457, 
		0xf181e9ee, 
		0xb37e6c2d, 
		0xa23a147f, 
		0xc8339db6, 
		0x16dd9e02, 
		0xb7a9675f, 
		0x51fd0a2d, 
		0xa25f1828, 
		0x71f768f0, 
		0x272050fa, 
		0x74b5e2f1, 
		0x930a643c, 
		0xe18791ad, 
		0xfea4f256, 
		0xfdb93253, 
		0xb533b37d, 
		0x73887728, 
		0x76b508ec, 
		0x0d4deec8, 
		0xaf0896be, 
		0x2eef3385, 
		0xd40c47d9, 
		0x4d1d89e4, 
		0x124f70f0, 
		0xd5b36877, 
		0xacd62d3d, 
		0x0878ddfc, 
		0x9edf0bfe, 
		0x551fd1d9, 
		0x56b0caaa, 
		0x7194fa31, 
		0x0ebf1cdc, 
		0x19f22aa0, 
		0x97e1519d, 
		0xa9e8bf0d, 
		0x1300fe06, 
		0x03479761, 
		0xd800b40b, 
		0xcc4e2015, 
		0x240488d0, 
		0x6f4345f0  
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
		0xcb80, 
		0x23d5, 
		0xce80, 
		0x44da, 
		0xd340, 
		0x88b4, 
		0xed86, 
		0xd100, 
		0x4000, 
		0xac0b, 
		0xc900, 
		0xd0a0, 
		0x4c40, 
		0x60de, 
		0xce00, 
		0xcd80, 
		0x4b00, 
		0x4c80, 
		0x51a0, 
		0x6a6e, 
		0x4d00, 
		0x5100, 
		0x7c45, 
		0xfc16, 
		0xf0b0, 
		0x4a00, 
		0x50e0, 
		0xc700, 
		0x7a93, 
		0xd3a0, 
		0x3ebb, 
		0xbc00, 
		0x4e80, 
		0x5180, 
		0xd8db, 
		0xa5db, 
		0xd340, 
		0xd080, 
		0x50e0, 
		0x916e, 
		0x20b0, 
		0xca80, 
		0x514d, 
		0xf41b, 
		0x0000, 
		0x4c80, 
		0x4c00, 
		0x23f5, 
		0xd3e0, 
		0xb678, 
		0x5200, 
		0xd060, 
		0x4a80, 
		0x8000, 
		0x8000, 
		0x8000, 
		0x5380, 
		0xca00, 
		0xfd3d, 
		0x814b, 
		0x4fc2, 
		0x5200, 
		0xc400, 
		0x4fef, 
		0x4a2e, 
		0x95d3, 
		0xcdc0, 
		0x124e, 
		0x8000, 
		0x51c0, 
		0xc700, 
		0x20c0, 
		0xd1e0, 
		0xd120, 
		0x8000, 
		0xcdc0, 
		0xce40, 
		0x52e0, 
		0x4c80, 
		0xce00, 
		0x52c0, 
		0xcc7c, 
		0x5020, 
		0x58de, 
		0xd1a0, 
		0x51c0, 
		0x1f6d, 
		0x5200, 
		0x0000, 
		0x4b87, 
		0xcdc0, 
		0x4ce9, 
		0xd0a0, 
		0x4900, 
		0x9bbf, 
		0xcfc0, 
		0xd0a0, 
		0x4800, 
		0x4d80, 
		0xd100, 
		0x4e40, 
		0xd320, 
		0xc980, 
		0x5020, 
		0x5180, 
		0x50a0, 
		0x0000, 
		0x4d40, 
		0xd3c0, 
		0x4b80, 
		0x1e63, 
		0xd060, 
		0x0000, 
		0xd100, 
		0x4200, 
		0x7606, 
		0x7c00, 
		0x34a6, 
		0xd180, 
		0xd060, 
		0x4877, 
		0xd140, 
		0xd1a0, 
		0x4dc0, 
		0x382b, 
		0xd120, 
		0xd280, 
		0xd1c0, 
		0xd000, 
		0x1519, 
		0x5380, 
		0x4a00, 
		0x50e0, 
		0xcb80, 
		0x5ee8, 
		0xc800, 
		0xb607, 
		0x0000, 
		0x5360, 
		0x6612, 
		0x5040, 
		0xc600, 
		0xfc00, 
		0x52e0, 
		0x9d1d, 
		0x4500, 
		0x8000, 
		0xce80, 
		0x7c00, 
		0x53a0, 
		0xd380, 
		0xd280, 
		0xcec0, 
		0x5320, 
		0x4880, 
		0x8000, 
		0x4f40, 
		0x3e66, 
		0x6557, 
		0x5f7e, 
		0xe20a, 
		0x5340, 
		0x5220, 
		0xca80, 
		0x5020, 
		0xd040, 
		0x4d40, 
		0x5080, 
		0x0dba, 
		0xc980, 
		0xc400, 
		0xc000, 
		0x4f80, 
		0x5320, 
		0xd040, 
		0xc700, 
		0xd9a2, 
		0x51c0, 
		0xd52a, 
		0xd380, 
		0xd2e0, 
		0x4500, 
		0xd1e0, 
		0xcf00, 
		0xd1c0, 
		0x4600, 
		0xcd0b, 
		0x4c40, 
		0x5240, 
		0x77d5, 
		0x5160, 
		0xcc00, 
		0xcb37, 
		0x4900, 
		0xf54d, 
		0xcc80, 
		0x7da8, 
		0x5360, 
		0x5e5a, 
		0xc200, 
		0x0d30, 
		0xbc00, 
		0x4d00, 
		0xd2a0, 
		0xd320, 
		0x4c40, 
		0x12a5, 
		0xcd80, 
		0x50e0, 
		0x4d00, 
		0xfc00, 
		0x7c00, 
		0xce80, 
		0xa84b, 
		0xd3c0, 
		0x8000, 
		0x50e0, 
		0x5200, 
		0xc200, 
		0x7e32, 
		0xadc6, 
		0x5000, 
		0xcf40, 
		0xd380, 
		0x081c, 
		0xcf80, 
		0xb287, 
		0x51e0, 
		0x52a0, 
		0xd060, 
		0xd3a0, 
		0x29d0, 
		0x7c46, 
		0xa8b6, 
		0x4e00, 
		0xd3c0, 
		0x4e80, 
		0xcf00, 
		0x422d, 
		0x04d7, 
		0x45c9, 
		0xd180, 
		0x9cd6, 
		0x53e0, 
		0x69f3, 
		0x4e00, 
		0x5300, 
		0x4a00, 
		0x5220, 
		0x021c, 
		0x53c0, 
		0x0000, 
		0xce80, 
		0x0000, 
		0xcdc0, 
		0xcf40, 
		0x5320, 
		0xd3a0, 
		0xd300, 
		0x4000, 
		0xb41b, 
		0x4c40, 
		0x4e00, 
		0xd2c0, 
		0xd2e0, 
		0x5100, 
		0xcaea, 
		0xc880, 
		0xf9ce, 
		0x4ad3, 
		0xd3e0, 
		0x4e40, 
		0x5000, 
		0x8000, 
		0x4e80, 
		0x7c00, 
		0x4880, 
		0x4cc0, 
		0x9adf, 
		0xd3c0, 
		0xfe08, 
		0x8000, 
		0xdf57, 
		0x6d19, 
		0x4e80, 
		0x43e3, 
		0x4cc0, 
		0xcf00, 
		0x5120, 
		0xcd00, 
		0xcec0, 
		0xcb00, 
		0xd380, 
		0x0000, 
		0x52a0, 
		0xb8bf, 
		0x4980, 
		0xa29b, 
		0xc700, 
		0x52e0, 
		0x5080, 
		0x0000, 
		0x4600, 
		0x4e80, 
		0x5360, 
		0x1d0f, 
		0x50a0, 
		0x7274, 
		0xaf7b, 
		0x4f80, 
		0xd000, 
		0x5020, 
		0x8000, 
		0xc980, 
		0x4880, 
		0xd080, 
		0xcd40, 
		0x8000, 
		0xd000, 
		0x3514, 
		0xd300, 
		0xca80, 
		0x6aa3, 
		0x7e23, 
		0xd260, 
		0x7cbc, 
		0x4c40, 
		0x4e80, 
		0x0925, 
		0x8000, 
		0xec51, 
		0xd2a0, 
		0x0e95, 
		0xd3a0, 
		0x8de7, 
		0xad56, 
		0xd320, 
		0x53e0, 
		0x9638, 
		0xccc0, 
		0xfc5f, 
		0xd0a0, 
		0xcdd0, 
		0x7ddd, 
		0xd380, 
		0xcc0a, 
		0x6047, 
		0xcc00, 
		0x4600, 
		0x5100, 
		0xd320, 
		0xce00, 
		0xcce2, 
		0xccc0, 
		0x3c00, 
		0xcc80, 
		0x4c40, 
		0xd2e0, 
		0x7d99, 
		0xfc00, 
		0xfc00, 
		0x4e00, 
		0xcf80, 
		0xbf8c, 
		0xd340, 
		0x4ec0, 
		0xec94, 
		0x698f, 
		0xac16, 
		0x4800, 
		0xd000, 
		0x4b00, 
		0xcc40, 
		0xd0a0, 
		0xcdc0, 
		0xce40, 
		0x7d2e, 
		0xcf40, 
		0x8000, 
		0x3db3, 
		0x4700, 
		0xd380, 
		0x4c80, 
		0x8000, 
		0x5320, 
		0x4abd, 
		0xccc0, 
		0x0000, 
		0x43f2, 
		0xd0a0, 
		0xd220, 
		0xd380, 
		0xcd40, 
		0x5140, 
		0xd0a0, 
		0xd080, 
		0xd3a0, 
		0x53e0, 
		0x7866, 
		0x8000, 
		0xcd00, 
		0x68e7, 
		0x5280, 
		0xd2c0, 
		0x4383, 
		0x5140, 
		0xcb80, 
		0x3340, 
		0xc600, 
		0xd2c0, 
		0x1a23, 
		0x4d40, 
		0x489f, 
		0x7271, 
		0x4a80, 
		0x47c1, 
		0x4e80, 
		0xd300, 
		0xcec0, 
		0xd340, 
		0x76dd, 
		0xcd40, 
		0x5320, 
		0xd0c0, 
		0x4a80, 
		0x4e00, 
		0xfc00, 
		0x4200, 
		0xc980, 
		0x5320, 
		0x1047, 
		0x0000, 
		0xd2e0, 
		0x7283, 
		0x2404, 
		0x7c00, 
		0x5280, 
		0x5140, 
		0xcdc0, 
		0x4000, 
		0xca00, 
		0xcdc0, 
		0x7caa, 
		0xd140, 
		0xd004, 
		0x8000, 
		0xd180, 
		0xd100, 
		0xe5ac, 
		0x4e80, 
		0x361e, 
		0xd060, 
		0xe008, 
		0x0105, 
		0x5200, 
		0xc800, 
		0xc400, 
		0xc500, 
		0x7c00, 
		0xcb00, 
		0xcb80, 
		0xb107, 
		0xd200, 
		0x7dcc, 
		0xd3e0, 
		0x47a0, 
		0x4800, 
		0x4500, 
		0xfac5, 
		0x50a0, 
		0xd200, 
		0xd300, 
		0x4171, 
		0x78a9, 
		0x5040, 
		0xccc0, 
		0xc500, 
		0x4000, 
		0xc900, 
		0xb00a, 
		0x5240, 
		0xfa1c, 
		0x53a0, 
		0x50e0, 
		0xd9a1, 
		0xcfc0, 
		0xd0e0, 
		0x20a5, 
		0x4425, 
		0xd0c0, 
		0xf6a4, 
		0x4d80, 
		0xa24f, 
		0xc700, 
		0x30cc, 
		0xcc80, 
		0xd3c0, 
		0x3c00, 
		0x5080, 
		0x50a0, 
		0x8eda, 
		0xd3e0, 
		0xc880, 
		0xd2c0, 
		0x50a0, 
		0x5240, 
		0x7c00, 
		0x0000, 
		0xd1e0, 
		0xd380, 
		0xccc0, 
		0xd060, 
		0x2ba1, 
		0xdd1b, 
		0x5360, 
		0xb5e3, 
		0xd220, 
		0x4f8e, 
		0xeb8e, 
		0x4800, 
		0xca80, 
		0x4b80, 
		0xd300, 
		0x02a9, 
		0x3251, 
		0x8000, 
		0xd2e0, 
		0x43a2, 
		0xd080, 
		0xd160, 
		0xcf40, 
		0x65c1, 
		0xce00, 
		0x0000, 
		0x437a, 
		0x4800, 
		0xc800, 
		0x3c00, 
		0x95e2, 
		0xcf40, 
		0xd200, 
		0xaa6f, 
		0xd2a0, 
		0xd000, 
		0xcca7, 
		0x4400, 
		0xe8f2, 
		0x3c00, 
		0x4ec0, 
		0x5240, 
		0x6c58, 
		0x8000, 
		0x4d80, 
		0x312b, 
		0xca80, 
		0x00cc, 
		0x6033, 
		0x52e0, 
		0x835e, 
		0x7c54, 
		0x7c00, 
		0xa2c8, 
		0xd2e0, 
		0xd000, 
		0xd0c0, 
		0x5080, 
		0xc900, 
		0xd1c0, 
		0x4c00, 
		0xccc0, 
		0xcfc0, 
		0xc900, 
		0x8fc0, 
		0x53a0, 
		0x7c00, 
		0xd1c0, 
		0x53c0, 
		0x1212, 
		0x0000, 
		0x5260, 
		0x50a0, 
		0x4dc0, 
		0x605b, 
		0xd240, 
		0x11b1, 
		0xd300, 
		0xca80, 
		0xcb80, 
		0x4500, 
		0xcfc0, 
		0xcf39, 
		0x9044, 
		0x4c80, 
		0x5180, 
		0xa013, 
		0xc880, 
		0x9311, 
		0xd260, 
		0xcc00, 
		0x7c00, 
		0x0000, 
		0x50a0, 
		0x5140, 
		0x5020, 
		0x8000, 
		0xd020, 
		0xef57, 
		0x5140, 
		0xd080, 
		0x8000, 
		0xd200, 
		0x5140, 
		0x53c0, 
		0xc600, 
		0xde8f, 
		0x8000, 
		0x9114, 
		0xfc00, 
		0x5360, 
		0xd0e0, 
		0x4600, 
		0x4a80, 
		0xcb00, 
		0x5280, 
		0x5200, 
		0xc000, 
		0x0000, 
		0x4a80, 
		0xd0c0, 
		0x9ffa, 
		0xc3c6, 
		0x1be7, 
		0x62c6, 
		0x4e40, 
		0x7dad, 
		0x5340, 
		0xd040, 
		0xfc00, 
		0x4cc0, 
		0xcc00, 
		0xd2e0, 
		0x8000, 
		0x50a0, 
		0xcf80, 
		0xcf80, 
		0x6a0c, 
		0x4cce, 
		0x6483, 
		0xd000, 
		0x4a80, 
		0xca80, 
		0xcc40, 
		0xd020, 
		0x4e80, 
		0x8000, 
		0x04ea, 
		0x8000, 
		0x50a0, 
		0x7c00, 
		0x50a0, 
		0x0000, 
		0xcd80, 
		0x53c0, 
		0x8cd7, 
		0xc900, 
		0xd0a0, 
		0xc524, 
		0xb6cf, 
		0xd060, 
		0x4c40, 
		0x0000, 
		0x7c00, 
		0xbc00, 
		0xd240, 
		0x43fe, 
		0xe83a, 
		0xd1be, 
		0x5360, 
		0x4d40, 
		0x51a0, 
		0x2795, 
		0x22e3, 
		0x4f40, 
		0xe098, 
		0x3c00, 
		0xd0e0, 
		0x3b05, 
		0x0000, 
		0x4085, 
		0xfe22, 
		0x5080, 
		0x4600, 
		0x203a, 
		0xa134, 
		0x5360, 
		0xcb00, 
		0x4b80, 
		0xcc40, 
		0xb645, 
		0x3c00, 
		0xdba0, 
		0x70d2, 
		0x3f5a, 
		0x3858, 
		0x4b80, 
		0xc900, 
		0xd160, 
		0x8000, 
		0x4cc0, 
		0x337f, 
		0xd000, 
		0xd3e0, 
		0x51e0, 
		0x7bc0, 
		0x52a0, 
		0xd000, 
		0x4d40, 
		0x8000, 
		0xd200, 
		0xcc40, 
		0x3fcf, 
		0xce80, 
		0x51e0, 
		0x6ef7, 
		0xd360, 
		0x5200, 
		0xdeff, 
		0xa020, 
		0xf38a, 
		0x8000, 
		0xd100, 
		0x33b5, 
		0xcd40, 
		0x617c, 
		0x4880, 
		0x412d, 
		0x4500, 
		0x4c80, 
		0x5260, 
		0xd2e0, 
		0x4000, 
		0xd160, 
		0x4200, 
		0xd2a0, 
		0xd120, 
		0x4c40, 
		0xcc00, 
		0x5240, 
		0xd180, 
		0x5120, 
		0x5140, 
		0xcf80, 
		0xd120, 
		0xd120, 
		0x13fc, 
		0xd380, 
		0xcf00, 
		0x4c00, 
		0x5260, 
		0x4d00, 
		0x7c00, 
		0x8e45, 
		0xd3c0, 
		0x52a0, 
		0x5220, 
		0x5340, 
		0x7c12, 
		0xef11, 
		0x1201, 
		0xca80, 
		0xbc00, 
		0xd1a0, 
		0x1659, 
		0x4e80, 
		0x51a0, 
		0xb049, 
		0x4a00, 
		0x4f40, 
		0x4200, 
		0xd0e0, 
		0xd240, 
		0xcec0, 
		0x4600, 
		0xd990, 
		0xcd00, 
		0xc09a, 
		0x4c80, 
		0xbc00, 
		0xcb80, 
		0x5320, 
		0x61b5, 
		0x4d80, 
		0x4f80, 
		0x52e0, 
		0x5020, 
		0x4f40, 
		0xd140, 
		0x5240, 
		0x6ab9, 
		0xbd48, 
		0x4fc0, 
		0x3c00, 
		0x8000, 
		0xcc80, 
		0x4b00, 
		0xcd00, 
		0x0000, 
		0x53e0, 
		0xd1e0, 
		0xca00, 
		0x4d80, 
		0x7c00, 
		0x5280, 
		0x7e6b, 
		0x8000, 
		0xd0c0, 
		0x52e0, 
		0x8c91, 
		0x5020, 
		0x08e5, 
		0xfc00, 
		0xc500, 
		0xcc80, 
		0xfc00, 
		0x8000, 
		0x4200, 
		0x4600, 
		0x4cc0, 
		0xfc00, 
		0x4d00, 
		0xd0a0, 
		0x4700, 
		0xcf40, 
		0xcd00, 
		0x5260, 
		0x9276, 
		0x50e0, 
		0xfc00, 
		0x50c0, 
		0xc8f9, 
		0x262e, 
		0x4d00, 
		0x4600, 
		0xc880, 
		0xd080, 
		0xc400, 
		0xc500, 
		0x7c8a, 
		0x52c0, 
		0xfa48, 
		0x4f00, 
		0x4d80, 
		0x4000, 
		0xc200, 
		0x4500, 
		0x5140, 
		0x4f80, 
		0xcf80, 
		0x6df7, 
		0x0000, 
		0x64b7, 
		0x7c00, 
		0xf59e, 
		0xd3a0, 
		0xd340, 
		0xc400, 
		0x4800, 
		0x5380, 
		0x5240, 
		0xd360, 
		0x5280, 
		0x5060, 
		0xcb00, 
		0xcf40, 
		0x8000, 
		0x52e0, 
		0x5260, 
		0x25b3, 
		0x4c40, 
		0x64a1, 
		0x7c00, 
		0x01a9, 
		0x5380, 
		0xfc00, 
		0xcf80, 
		0x5100, 
		0x4d00, 
		0x5180, 
		0x4f80, 
		0x5040, 
		0xc500, 
		0x5040, 
		0xcd40, 
		0x8000, 
		0x8000, 
		0xc7b1, 
		0x5300, 
		0x4000, 
		0xd160, 
		0x28af, 
		0xd100, 
		0xce00, 
		0x4b80, 
		0xd1a0, 
		0x4880, 
		0x2b7c, 
		0x8000, 
		0xccc0, 
		0x6fe4, 
		0xc000, 
		0xcf40, 
		0x4d80, 
		0x4200, 
		0x7c6d, 
		0xcc40, 
		0x53e0, 
		0x480a, 
		0x42ad, 
		0xc200, 
		0x8000, 
		0x4405, 
		0xd0e0, 
		0xd100, 
		0xca00, 
		0x52a0, 
		0x0cbc, 
		0x52c0, 
		0x4d40, 
		0xa09d, 
		0xd1a0, 
		0x8000, 
		0x9490, 
		0x9e5e, 
		0x8edc, 
		0xd0c0, 
		0x0000, 
		0x7c00, 
		0xc4a7, 
		0xc980, 
		0xd0a0, 
		0xd020, 
		0x24a6, 
		0x4980, 
		0x50a0, 
		0xca00, 
		0xce80, 
		0x03a2, 
		0xc000, 
		0xe627, 
		0xe843, 
		0xce00, 
		0x011d, 
		0x315f, 
		0x53c0, 
		0x43c6, 
		0xafa9, 
		0xce80, 
		0xcb00, 
		0x9da4, 
		0xd340, 
		0xcc40, 
		0xfc00, 
		0xbba6, 
		0x4900, 
		0x33f5, 
		0xd380, 
		0x8000, 
		0x0e38, 
		0xd3e0, 
		0xccc0, 
		0x4e00, 
		0x74b2, 
		0xd280, 
		0x4cc0, 
		0x8c48, 
		0x5965, 
		0xcd40, 
		0x4200, 
		0xa88a, 
		0xc500, 
		0x0000, 
		0x3755, 
		0x0932, 
		0xe0dc, 
		0x52e0, 
		0x48b6, 
		0xd2c0, 
		0x5000, 
		0x5080, 
		0xd120, 
		0xe485, 
		0xfc00, 
		0xd000, 
		0x5240, 
		0x4d40, 
		0x53c0, 
		0x8000, 
		0x4f40, 
		0xd2c0, 
		0xcbfd, 
		0x5240, 
		0x4880, 
		0xce80, 
		0xfc27, 
		0xe417, 
		0x017c, 
		0x4a80, 
		0xcfc0, 
		0x4b00, 
		0xfdcc, 
		0x4e40, 
		0x4800, 
		0x4e00, 
		0xd120, 
		0x7c00, 
		0xc700, 
		0x5020, 
		0x4600, 
		0xde82, 
		0xc200, 
		0xbbd5, 
		0x31f0, 
		0x53c0, 
		0xcf00, 
		0x50a0, 
		0xc500, 
		0xd000, 
		0xa769, 
		0x5040, 
		0x50ac, 
		0xce00, 
		0xcf00, 
		0xd120, 
		0x21ac, 
		0x4cc0, 
		0x7c00, 
		0x4b80, 
		0x4a80, 
		0x0000, 
		0x5240, 
		0xcd40, 
		0x4500, 
		0xd360, 
		0xd3e0, 
		0x4274, 
		0xd260, 
		0xd140, 
		0x97f5, 
		0x4294, 
		0x42ea, 
		0x5380, 
		0xd180, 
		0x4b2b, 
		0x5180, 
		0x4800, 
		0xd260, 
		0x53c0, 
		0x7a7c, 
		0xd200, 
		0xda7e, 
		0x7c00, 
		0xd220, 
		0xd140, 
		0x85d1, 
		0x1960, 
		0x8000, 
		0xbc00, 
		0x4f00, 
		0x4f00, 
		0xbc00, 
		0xcf40, 
		0x2b4e, 
		0xfdf5, 
		0x4a00, 
		0x5340, 
		0x5220, 
		0xd160, 
		0x51c0, 
		0xd0e0, 
		0x8000, 
		0x7143, 
		0x29b8, 
		0x8000, 
		0x833c, 
		0xfc00, 
		0x2ad3, 
		0x80a3, 
		0x7c00, 
		0x3c00, 
		0x5280, 
		0xd81f, 
		0x8000, 
		0x7c00, 
		0xd2e0, 
		0xd020, 
		0x697c, 
		0x4c40, 
		0xcd80, 
		0xd180, 
		0x0000, 
		0xc500, 
		0xc800, 
		0xfc16, 
		0x9385, 
		0x0000, 
		0xbf96, 
		0xca00, 
		0x51c0, 
		0x0899, 
		0xb92d, 
		0xd340, 
		0x7407, 
		0x6d71, 
		0x95b5, 
		0xc980, 
		0x8000, 
		0x5200, 
		0x5bd7, 
		0x51a0, 
		0x8000, 
		0x8000, 
		0x5360, 
		0xd040, 
		0xfc00, 
		0xd1c0, 
		0xd90f, 
		0xd060, 
		0xf198, 
		0xd1c0, 
		0x5080, 
		0xd140, 
		0x5060, 
		0x5120, 
		0x891b, 
		0xd080, 
		0x7cc1, 
		0x4500, 
		0x4a00, 
		0x8000, 
		0x51c0, 
		0xd280, 
		0xa7be, 
		0x53a0, 
		0x5080, 
		0x403e, 
		0x4ec0, 
		0xff7a, 
		0x4c5a, 
		0x0583, 
		0x4b00, 
		0x4b00, 
		0x689d, 
		0xca00, 
		0x5120, 
		0xcd00, 
		0xc780, 
		0x4e80, 
		0x53c0, 
		0x4b80, 
		0xd000, 
		0xda69, 
		0x5b61, 
		0x4a00, 
		0xb35e, 
		0x7c42, 
		0xd300, 
		0x4f00, 
		0x52e0, 
		0xd180, 
		0xfc00, 
		0x5d18, 
		0xce6a, 
		0xcc80, 
		0x5360, 
		0xcc80, 
		0xca00, 
		0xb28d, 
		0x01c0, 
		0xd200, 
		0x30f9, 
		0x4b00, 
		0x5360, 
		0xd3f5, 
		0xd380, 
		0x4d00, 
		0x4c40, 
		0x53c0, 
		0xd2e0, 
		0xdda3, 
		0x8f86, 
		0xd300, 
		0xc3c3, 
		0xcc40, 
		0xfc3f, 
		0xaa03, 
		0x752b, 
		0x63ad, 
		0x53e0, 
		0xc500, 
		0x53e0, 
		0xcfc0, 
		0x7494, 
		0xcd80, 
		0x4f80, 
		0xce80, 
		0x8000, 
		0x471c, 
		0xc500, 
		0xf075, 
		0xcec0, 
		0x7c00, 
		0x4c80, 
		0x0000, 
		0xce40, 
		0x9c02, 
		0xd040, 
		0xcc00, 
		0xd160, 
		0x4b00, 
		0xd280, 
		0xd360, 
		0x2073, 
		0x7782, 
		0x4e9f, 
		0x52a0, 
		0x26be, 
		0x7c00, 
		0xd340, 
		0x025c, 
		0xcc40, 
		0xcf80, 
		0xcb80, 
		0xcec0, 
		0xccc0, 
		0x51a0, 
		0x1524, 
		0x5000, 
		0x4700, 
		0xfc00, 
		0x4980, 
		0xaf69, 
		0x7edc, 
		0xd1c0, 
		0x8b26, 
		0xcc52, 
		0xd280, 
		0xd040, 
		0x0091, 
		0xd92a, 
		0x871c, 
		0xd320, 
		0xd160, 
		0x5340, 
		0xc800, 
		0xd3a0, 
		0xa27d, 
		0x0000, 
		0x3c00, 
		0x2604, 
		0xd1a0, 
		0xd360, 
		0x0000, 
		0x50c0, 
		0xc400, 
		0xcc80, 
		0x4b3e, 
		0x4cc0, 
		0xcb00, 
		0xd1c0, 
		0x6520, 
		0xcf00, 
		0x2e76, 
		0xb5ed, 
		0xc4e5, 
		0xd100, 
		0x7e6f, 
		0xcf00, 
		0x64f6, 
		0xd1c0, 
		0x4dc0, 
		0x5080, 
		0x5300, 
		0xd000, 
		0x4e00, 
		0x5320, 
		0x0e00, 
		0xca00, 
		0xd360, 
		0xd260, 
		0xc500, 
		0x4f00, 
		0xc980, 
		0x50a0, 
		0x4e00, 
		0x82b3, 
		0x03d8, 
		0xd380, 
		0xd0a0, 
		0xa4f8, 
		0x29e1, 
		0x51e0, 
		0x3be5, 
		0x4536, 
		0x5020, 
		0xd120, 
		0x65cb, 
		0x61e8, 
		0xc600, 
		0x4f80, 
		0xd000, 
		0xcd00, 
		0x5240, 
		0xd200, 
		0xce40, 
		0xd3a0, 
		0x51a0, 
		0x0000, 
		0x4980, 
		0x5360, 
		0xe72a, 
		0xc900, 
		0x51e0, 
		0xf135, 
		0xd180, 
		0xd080, 
		0x4c40, 
		0x82b5, 
		0x7d6b, 
		0x010c, 
		0x3c00, 
		0x5120, 
		0x4f80, 
		0x5300, 
		0xfa10, 
		0xcd80, 
		0x0000, 
		0xc700, 
		0x269b, 
		0xc900, 
		0x8000, 
		0x8000, 
		0x5200, 
		0x4ec0, 
		0x368c, 
		0xd3e0, 
		0x4788, 
		0xd2c0, 
		0x00a1, 
		0xd15f, 
		0x039b, 
		0xc417, 
		0xd3e0, 
		0x0c55, 
		0xfc00, 
		0xd160, 
		0x2694, 
		0xd0e0, 
		0x4b00, 
		0xd3c0, 
		0x4880, 
		0xfc00, 
		0x0000, 
		0xce40, 
		0xcfad, 
		0x5380, 
		0xb0d2, 
		0xa27b, 
		0xca45, 
		0xd220, 
		0x5040, 
		0x9360, 
		0x2eca, 
		0x3912, 
		0x51e0, 
		0x4c80, 
		0x5320, 
		0x5020, 
		0x5200, 
		0xd349, 
		0xca00, 
		0xd060, 
		0xcf00, 
		0xeea2, 
		0x84ff, 
		0x06ce, 
		0xcfc0, 
		0x50c0, 
		0x8014, 
		0xd3e0, 
		0x0000, 
		0x5140, 
		0xd120, 
		0x0593, 
		0xd220, 
		0x7ce7, 
		0x4c80, 
		0xd603, 
		0x4f40, 
		0x53e0, 
		0xd88e, 
		0x4880, 
		0x61f0, 
		0x0000, 
		0xcdc0, 
		0xcf00, 
		0x5280, 
		0xca80, 
		0xb370, 
		0x67f7, 
		0xd340, 
		0x6a47, 
		0xcdc0, 
		0x52e0, 
		0x1c7f, 
		0x2c23, 
		0x4880, 
		0x2e1a, 
		0x6688, 
		0xcc00, 
		0x4dc0, 
		0x8000, 
		0x5120, 
		0xce00, 
		0xd000, 
		0x9ba0, 
		0x4e40, 
		0x4700, 
		0xd572, 
		0x5120, 
		0x5280, 
		0x69b4, 
		0xfc00, 
		0x53a0, 
		0x4980, 
		0xad7b, 
		0xcb00, 
		0xd020, 
		0xcd9d, 
		0x0000, 
		0xd160, 
		0x4700, 
		0xd1a0, 
		0xd040, 
		0x697b, 
		0x7239, 
		0x53e0, 
		0x8000, 
		0x5380, 
		0x40e2, 
		0xc3c9, 
		0xd080, 
		0xcc40, 
		0x7462, 
		0xd3e0, 
		0x4400, 
		0xc900, 
		0x1f7e, 
		0x3544, 
		0x5340, 
		0x8000, 
		0xd220, 
		0xd0a0, 
		0x6a8e, 
		0xc980, 
		0x4b00, 
		0xc8d9, 
		0xca80, 
		0xcf40, 
		0x4500, 
		0x9c9f, 
		0x5340, 
		0x5120, 
		0xc800, 
		0xd2a0, 
		0x5260, 
		0x9049, 
		0xd65c, 
		0x797f, 
		0x0000, 
		0xed2b, 
		0xe90a, 
		0xd0c0, 
		0x5280, 
		0xf42e, 
		0x4dc0, 
		0x3c00, 
		0xd3c0, 
		0x5765, 
		0x4f00, 
		0x463d, 
		0x4ee0, 
		0x5160, 
		0x5220, 
		0xe517, 
		0x4900, 
		0x4d80, 
		0x4800, 
		0x4500, 
		0x039a, 
		0x210f, 
		0x53a0, 
		0xc880, 
		0xd160, 
		0xca00, 
		0xcb00, 
		0xcc40, 
		0xd1c2, 
		0x6dab, 
		0xcc00, 
		0x52c0, 
		0x5080, 
		0x4e80, 
		0x4980, 
		0x55ba, 
		0x7c00, 
		0xcdd9, 
		0xc600, 
		0x51e0, 
		0x5240, 
		0xcc00, 
		0x8000, 
		0xd140, 
		0xfc00, 
		0xba25, 
		0xcdc0, 
		0xd300, 
		0x5380, 
		0x5100, 
		0x50a0, 
		0x4500, 
		0xdf0a, 
		0x3578, 
		0xc800, 
		0x4d00, 
		0x1f0f, 
		0xef0b  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xbe53e7a0, 
		0x0c7ffc00, 
		0x807ffffe, 
		0xc1200000, 
		0x0e8247ef, 
		0xf79e6c91, 
		0x41000000, 
		0x0c7f0000, 
		0x80000000, 
		0x42f56c6b, 
		0xfab116d4, 
		0x7fc00001, 
		0x00004000, 
		0x80000000, 
		0x32be5cd8, 
		0xeb274312, 
		0xc1980000, 
		0x1e23512e, 
		0xc2580000, 
		0xc2300000, 
		0x7eff8b76, 
		0xea1a8e6c, 
		0x80000000, 
		0x4b61b7da, 
		0xa234dd77, 
		0xcccccccc, 
		0x63379763, 
		0x41b00000, 
		0x80000008, 
		0xc5178585, 
		0x42400000, 
		0x0c7ffff0, 
		0x6fec08fa, 
		0x7ff57475, 
		0xc22c0000, 
		0x41c00000, 
		0x41980000, 
		0x80000000, 
		0x00800000, 
		0x42380000, 
		0xc2780000, 
		0x0f7ffffe, 
		0xd6c9b427, 
		0x00000000, 
		0x9ef02b15, 
		0xee260361, 
		0x80000000, 
		0xc1400000, 
		0xc1900000, 
		0xc2500000, 
		0x5dd43d0a, 
		0x7f800000, 
		0x42500000, 
		0xc0800000, 
		0xc2340000, 
		0xc2380000, 
		0x42540000, 
		0xc0000000, 
		0x00800001, 
		0x704bdf2b, 
		0x00004000, 
		0x41900000, 
		0x80008000, 
		0x422c0000, 
		0x423c0000, 
		0xaccbbefe, 
		0x3ddd7463, 
		0x7f800000, 
		0x0e007fff, 
		0x00010000, 
		0x7f800000, 
		0x0f7ffffe, 
		0x7f800000, 
		0x2263a01e, 
		0x427c0000, 
		0xc2400000, 
		0x82038077, 
		0x0e00000f, 
		0x41900000, 
		0xaef066d1, 
		0x8bed9a04, 
		0xc2540000, 
		0x0c7fffff, 
		0xc434cb46, 
		0x380c7e56, 
		0x7f800000, 
		0x00000800, 
		0xbf800000, 
		0xefe47858, 
		0x00020000, 
		0x0c7e0000, 
		0x00000000, 
		0x426c0000, 
		0x80000000, 
		0x41d80000, 
		0x7f7ffffe, 
		0xc0c00000, 
		0x8e4e0a4e, 
		0x2a1acbf8, 
		0x7fbfffff, 
		0xa20ddb22, 
		0x00010000, 
		0x41600000, 
		0x0c400000, 
		0xc1500000, 
		0x80000000, 
		0x41880000, 
		0xcb000000, 
		0x6f541458, 
		0x0c7fff80, 
		0x0e007fff, 
		0x00000000, 
		0x7fb4d99c, 
		0x00000000, 
		0x7f800000, 
		0x7555d750, 
		0x539f9ba8, 
		0xc1e00000, 
		0x1492aae0, 
		0x08f614bf, 
		0x80000080, 
		0x80000000, 
		0x41f00000, 
		0x7f800000, 
		0x80181f0d, 
		0x41980000, 
		0xc2500000, 
		0x0c7fff00, 
		0x9c0950b1, 
		0xff800000, 
		0x494ad8fb, 
		0x3f800000, 
		0x713ab26a, 
		0x5420a881, 
		0x42600000, 
		0xcbffda43, 
		0x40000000, 
		0x0f7ffffc, 
		0xc0e00000, 
		0x41980000, 
		0x9c9103ba, 
		0xc1d80000, 
		0xff7fffff, 
		0x0c7fffe0, 
		0x41a00000, 
		0x80000100, 
		0xf20c0696, 
		0x0e0001ff, 
		0x0e1b0d8f, 
		0x0c7ffff0, 
		0x2ce40086, 
		0x41d80000, 
		0xec1ec86c, 
		0xebd4e52c, 
		0xde4a15d5, 
		0xf8fede01, 
		0xff800000, 
		0xc2300000, 
		0x91d65f12, 
		0x40a00000, 
		0x80000080, 
		0x47268e6c, 
		0x00800003, 
		0x7fbfffff, 
		0x0c400000, 
		0xadb45074, 
		0x7f7fffff, 
		0xfa7caf49, 
		0x80200000, 
		0x0c7f0000, 
		0xff7ffffe, 
		0x7f000000, 
		0x5e1f9cc0, 
		0xdce3e77b, 
		0x00000000, 
		0x3f800001, 
		0x420c0000, 
		0x00040000, 
		0x66732dc9, 
		0x7f800000, 
		0xaafbd21a, 
		0x0e1fffff, 
		0x580dfd3c, 
		0x00800003, 
		0x00800001, 
		0x80002000, 
		0x2f668b6a, 
		0xab373fed, 
		0x3c248f88, 
		0x00c4e115, 
		0xc2680000, 
		0x007fffff, 
		0xe3523a5d, 
		0x280685d4, 
		0x0e00000f, 
		0x42100000, 
		0x0c7f8000, 
		0xcba28ff6, 
		0x00000000, 
		0xcccccccc, 
		0x4073823f, 
		0x05302050, 
		0x00800002, 
		0x8f7ffffc, 
		0x80008000, 
		0x80800000, 
		0xb8292eed, 
		0x00000000, 
		0xc22c0000, 
		0x807a6c3a, 
		0x80000400, 
		0x00000008, 
		0x40175ded, 
		0x8f7fffff, 
		0x427c0000, 
		0x55555555, 
		0x608fbf29, 
		0x8f7ffffe, 
		0xff800000, 
		0x3138fc96, 
		0x00000000, 
		0x65249c00, 
		0x6e8f12ac, 
		0x3809818e, 
		0x80000000, 
		0x0c7fe000, 
		0xf9ebe96c, 
		0x00585553, 
		0x00800001, 
		0x007ffffe, 
		0x0e07ffff, 
		0x00800003, 
		0x0e00001f, 
		0xff9500de, 
		0xf3e0f17d, 
		0x42100000, 
		0x6c855f21, 
		0x41600000, 
		0x42280000, 
		0x00000000, 
		0x42280000, 
		0x80000000, 
		0xc2000000, 
		0xc2240000, 
		0xff800000, 
		0x7f800000, 
		0xc1800000, 
		0x0c7ffff0, 
		0x710db66d, 
		0x807fffff, 
		0x0c7ffffc, 
		0x1151c201, 
		0x41900000, 
		0x80000000, 
		0x42300000, 
		0xff800000, 
		0xc1d80000, 
		0x7f800000, 
		0x6413316b, 
		0x33333333, 
		0xc1e80000, 
		0x0c7f8000, 
		0x80000000, 
		0xed1e91b6, 
		0x42240000, 
		0xff7ffffe, 
		0x41100000, 
		0x0c7fff00, 
		0x80008000, 
		0xc1980000, 
		0x00000002, 
		0xbca42bee, 
		0x4b000000, 
		0x42380000, 
		0xebac8686, 
		0xc676dd62, 
		0x99b9a8ed, 
		0x424c0000, 
		0x00040000, 
		0x80800001, 
		0x007fffff, 
		0x7f7fffff, 
		0xbf800001, 
		0x00100000, 
		0x34519df9, 
		0x0c7ffff0, 
		0xff4b605d, 
		0x00008000, 
		0xc21c0000, 
		0x00000040, 
		0x8f7ffffd, 
		0x00010000, 
		0xc1880000, 
		0x80000000, 
		0x00000000, 
		0xedd686a9, 
		0x7fc00001, 
		0x7f800000, 
		0x6ea40baa, 
		0xc1700000, 
		0x41b00000, 
		0xc904dd57, 
		0x41800000, 
		0x0c7fffff, 
		0x80000000, 
		0x7bda319a, 
		0xb4291b9d, 
		0x83b1d1b3, 
		0x2ab26360, 
		0x77824bb7, 
		0x0c7ffc00, 
		0x0e007fff, 
		0xc2640000, 
		0x3f028f5c, 
		0xc20c0000, 
		0x007ffffe, 
		0xc2780000, 
		0xff800000, 
		0x7f800000, 
		0x0e03ffff, 
		0x20ae7d44, 
		0x0e000fff, 
		0x007ffffe, 
		0x9b647f6a, 
		0xc1a80000, 
		0x80000000, 
		0x8034927b, 
		0xc2240000, 
		0x183efa09, 
		0x8dc2e41a, 
		0xff800001, 
		0xa73b7647, 
		0x42240000, 
		0xf12c2ca7, 
		0xca5dc4cb, 
		0xc27c0000, 
		0x3d7e6e92, 
		0xc1700000, 
		0xff7fffff, 
		0xb0fb3330, 
		0x79ef8f28, 
		0x081be539, 
		0x42480000, 
		0x80000040, 
		0xc0000000, 
		0x7fc00000, 
		0x528386bd, 
		0x41880000, 
		0x10115f93, 
		0x40e00000, 
		0xfce93820, 
		0x42440000, 
		0x35009594, 
		0xc2500000, 
		0xf0194823, 
		0x00000000, 
		0x80800002, 
		0x0c7e0000, 
		0x14176c32, 
		0x33333333, 
		0x00000000, 
		0x8fec1047, 
		0x425c0000, 
		0x80000002, 
		0x15637731, 
		0x42240000, 
		0x00000000, 
		0xff800000, 
		0x1a9508fa, 
		0xe14e5b0f, 
		0xc2140000, 
		0xc2300000, 
		0x55555555, 
		0x424c0000, 
		0x80001000, 
		0x42040000, 
		0x0e00001f, 
		0x0e3fffff, 
		0x42500000, 
		0xb519c6cc, 
		0xc1f80000, 
		0x41600000, 
		0x6471a25d, 
		0x0d000ff0, 
		0xd4a6396b, 
		0x80020000, 
		0x00000000, 
		0x30bea282, 
		0xe7fd20fd, 
		0x80004000, 
		0xffbfffff, 
		0xe6d7a07d, 
		0x00040000, 
		0x944b7165, 
		0x585b181e, 
		0x0c7ffffe, 
		0x42240000, 
		0x64495b5b, 
		0x2aa75c7a, 
		0x41a00000, 
		0x5b7a8489, 
		0x00000400, 
		0xc2740000, 
		0x0c7ffff0, 
		0x00800002, 
		0x7bdcb555, 
		0x80000001, 
		0x00000000, 
		0x00004000, 
		0xc1d00000, 
		0x8f7ffffc, 
		0x7a4658df, 
		0x80000080, 
		0xa39b0eaa, 
		0x41d00000, 
		0xffc00000, 
		0xff800000, 
		0x3f57cf8f, 
		0x00000000, 
		0x0c7ff800, 
		0x0079a50b, 
		0x7f7fffff, 
		0x00000000, 
		0x38ae6093, 
		0x00000008, 
		0x8f7fffff, 
		0x41a00000, 
		0xc2680000, 
		0x80000200, 
		0x41700000, 
		0x41400000, 
		0x80000001, 
		0x7f000000, 
		0x80000001, 
		0x41100000, 
		0x1eed0f27, 
		0x0c400000, 
		0x42300000, 
		0xda3cfc0b, 
		0x0e00001f, 
		0x80800002, 
		0x00000010, 
		0x7c4dc907, 
		0x0e0fffff, 
		0x0e0001ff, 
		0x80000000, 
		0x41300000, 
		0x00000000, 
		0x8f7ffffe, 
		0xd9e031e6, 
		0x4c4537dc, 
		0xcccccccc, 
		0x17a18131, 
		0xc1b00000, 
		0x93207c05, 
		0x460ae21a, 
		0x9cf691b0, 
		0x00800000, 
		0xc2540000, 
		0x6b7ca20a, 
		0x1f225061, 
		0x7f800000, 
		0x00000008, 
		0x8f7ffffe, 
		0x00000000, 
		0x9736e4cc, 
		0x7faddb60, 
		0x43c15480, 
		0xa6b2855d, 
		0x00000000, 
		0x80100000, 
		0x0740bae8, 
		0xffbaeec2, 
		0x164fb37b, 
		0x421c0000, 
		0x0c600000, 
		0x40190f71, 
		0x80000800, 
		0xe0362c9a, 
		0xeeab2102, 
		0xab462516, 
		0x80137499, 
		0xec0c94fc, 
		0x5526c6e2, 
		0xff800000, 
		0x0e007fff, 
		0x00400000, 
		0xcc7fc485, 
		0x42400000, 
		0xe2f7a403, 
		0x3e3c9f7f, 
		0x80000008, 
		0x1189ae38, 
		0x80100000, 
		0xec047c1b, 
		0x7fbf2833, 
		0x00800000, 
		0x00000000, 
		0x8f7ffffc, 
		0xff7ffffe, 
		0xff800000, 
		0xaa13c9b5, 
		0x7f800000, 
		0x42580000, 
		0xb4fa9a4d, 
		0x00001000, 
		0xc2080000, 
		0x00000002, 
		0xa6e49cfb, 
		0x80000000, 
		0x0f7ffffc, 
		0x0e1fffff, 
		0x0ba7723d, 
		0xff800000, 
		0x80800001, 
		0x3160c51a, 
		0x42a900c4, 
		0x00080000, 
		0x00000200, 
		0x00ffffff, 
		0xc1400000, 
		0x83111a8b, 
		0x00800002, 
		0x00000000, 
		0xc1700000, 
		0x00000000, 
		0xbd59816c, 
		0x7f800000, 
		0x80000000, 
		0x0c7ff000, 
		0x8142ca9c, 
		0x00800000, 
		0x7fae9b90, 
		0x7f000000, 
		0x1ff6b2c3, 
		0x42700000, 
		0x00100000, 
		0xe6fc6190, 
		0x42080000, 
		0x0c7fe000, 
		0x9c18737f, 
		0xd0198ee7, 
		0x80000000, 
		0xff800000, 
		0x80080000, 
		0x9b4a2af9, 
		0x80800002, 
		0x00004000, 
		0xa74d1ca8, 
		0x28f40678, 
		0x80000000, 
		0x42500000, 
		0xc1c00000, 
		0xffffffff, 
		0x0e1fffff, 
		0x427c0000, 
		0xc2500000, 
		0x80800001, 
		0x80200000, 
		0x41d00000, 
		0xb4fd04f8, 
		0x42700000, 
		0x0e1fffff, 
		0x8064f04f, 
		0x88c6d5d2, 
		0x80000800, 
		0x7fc00001, 
		0xc1800000, 
		0x44ce41ee, 
		0x8b6e0cf6, 
		0xc2740000, 
		0x00000000, 
		0x00000000, 
		0xc1700000, 
		0xb3b75ed3, 
		0xc1700000, 
		0xd182bf25, 
		0x9ede1f79, 
		0x42680000, 
		0x40c00000, 
		0xbe75bca9, 
		0x5a5ad434, 
		0xc1500000, 
		0x841945fe, 
		0x0c7c0000, 
		0x807ffffe, 
		0x0e0fffff, 
		0x0068c566, 
		0x42600000, 
		0x00000000, 
		0x42100000, 
		0xdac7b62e, 
		0x33333333, 
		0x80000000, 
		0x00000080, 
		0x0e0001ff, 
		0x327d9853, 
		0x0e000fff, 
		0x80010000, 
		0xc25c0000, 
		0x4281c606, 
		0x0c7e0000, 
		0x0e007fff, 
		0x80000000, 
		0x80800003, 
		0x80000080, 
		0xc2100000, 
		0x41200000, 
		0x80000000, 
		0x24217306, 
		0x00000000, 
		0xff7ffffe, 
		0x00008000, 
		0xc1a80000, 
		0xff000000, 
		0xf4235ced, 
		0x40800000, 
		0xc1d00000, 
		0x4d9fe03a, 
		0x00000800, 
		0x0038bcba, 
		0x171af9dc, 
		0x80000000, 
		0x41880000, 
		0xc24c0000, 
		0x48024643, 
		0x80000000, 
		0xc0a00000, 
		0x0c7c0000, 
		0x41f80000, 
		0xc1000000, 
		0x42280000, 
		0x00000200, 
		0x3f800001, 
		0xafc4db5a, 
		0x9916aedc, 
		0xc23c0000, 
		0x80000008, 
		0x0e000fff, 
		0x19ab57bb, 
		0x84f1bb9b, 
		0x80000002, 
		0x80800002, 
		0x80000000, 
		0xc1600000, 
		0x31302066, 
		0x41b00000, 
		0x41f00000, 
		0x0e00001f, 
		0xa14e9d20, 
		0x0c7ffe00, 
		0x00000800, 
		0xc2740000, 
		0x41b80000, 
		0x42100000, 
		0x80100000, 
		0x0c600000, 
		0x3ba1fbd6, 
		0x2f5e7453, 
		0x8b742bd8, 
		0x0e00003f, 
		0x00800001, 
		0x00000000, 
		0x26534f75, 
		0xff800001, 
		0x670cf962, 
		0x06e3f863, 
		0xd9793c30, 
		0x00000000, 
		0x42700000, 
		0x0c4c621b, 
		0xbeb9308a, 
		0xf3d081dd, 
		0x802a7056, 
		0x42500000, 
		0x7fac3321, 
		0x80000800, 
		0xd80fe1c5, 
		0x042b5d2a, 
		0x7594e3d5, 
		0x0c7fff80, 
		0x443af6b0, 
		0xff800000, 
		0x7f7ffffe, 
		0x4b000000, 
		0x00000000, 
		0x80800002, 
		0x863359ad, 
		0x20bcd4b7, 
		0x42340000, 
		0x42740000, 
		0xe42980ec, 
		0x8044d0e1, 
		0x41c80000, 
		0x41e80000, 
		0xc1700000, 
		0xff800000, 
		0xc1000000, 
		0x00000040, 
		0xbf800001, 
		0x80000000, 
		0xcf283164, 
		0xc9c3b180, 
		0xffc00000, 
		0x807fffff, 
		0x18e2529a, 
		0x427c0000, 
		0xc2100000, 
		0x0e01ffff, 
		0xc2300000, 
		0x00080000, 
		0x775842c3, 
		0x3f800000, 
		0x41d00000, 
		0xc2640000, 
		0xc1c80000, 
		0x807ffffe, 
		0x00000000, 
		0xff800000, 
		0xb813671c, 
		0xc2400000, 
		0xa4d609a1, 
		0xc1e00000, 
		0x2af69b34, 
		0xc1200000, 
		0x80000000, 
		0xdfa3b28e, 
		0x00000000, 
		0x5c087b54, 
		0x00002000, 
		0x00000200, 
		0x857ed6f3, 
		0xc25c0000, 
		0x0e7fffff, 
		0x0c7ff800, 
		0xcac6c151, 
		0x7f7fffff, 
		0x588f8364, 
		0x7fa61575, 
		0x80080000, 
		0x7f000000, 
		0xc0c00000, 
		0x3b07ac00, 
		0xc0000000, 
		0x80000000, 
		0x97b73e04, 
		0x7f7ffffe, 
		0x007ffffe, 
		0x41f00000, 
		0xa02e8220, 
		0x41300000, 
		0xffffffff, 
		0xe77d35aa, 
		0xf4548b11, 
		0x00008000, 
		0xd4d1e10c, 
		0x00000002, 
		0xff7ffffe, 
		0x33a521b3, 
		0xffb24600, 
		0xd0c58dfe, 
		0xff7fffff, 
		0x55555555, 
		0x00000010, 
		0x5ae8f751, 
		0x5ac21b9b, 
		0x420c0000, 
		0x802235f0, 
		0xcf72a453, 
		0x83cd0958, 
		0x00010000, 
		0x41d00000, 
		0x00800000, 
		0x00800000, 
		0xff800000, 
		0xc23c0000, 
		0x00400000, 
		0xc96583bf, 
		0xd207951c, 
		0x44eb01a2, 
		0xc0a00000, 
		0x80000100, 
		0x80000000, 
		0x7fb9b0cd, 
		0xc1f00000, 
		0xc0ff338f, 
		0x7fbfffff, 
		0x00800002, 
		0x42640000, 
		0x40000000, 
		0x148a8837, 
		0x41200000, 
		0xea8a93a7, 
		0xc2100000, 
		0x3fa258e5, 
		0x7fa9447a, 
		0x007ffffe, 
		0x86d155f3, 
		0x7fc00001, 
		0x0e0000ff, 
		0xc2380000, 
		0x427c0000, 
		0x0e00ffff, 
		0x00800001, 
		0x42780000, 
		0xfc7e3142, 
		0x80800002, 
		0x40400000, 
		0x33333333, 
		0xbd2c34cb, 
		0xc0a00000, 
		0x42440000, 
		0x0c005c60, 
		0x426c0000, 
		0x0c7ff800, 
		0x00000400, 
		0x41b80000, 
		0x00800001, 
		0xe5ea2d70, 
		0x8dad3303, 
		0xc2640000, 
		0x23750513, 
		0x00800001, 
		0x7f7ffffe, 
		0x7504a2d6, 
		0xc1c80000, 
		0x2722c65e, 
		0xc1c00000, 
		0x80000001, 
		0x0e00ffff, 
		0x41980000, 
		0x00800000, 
		0x7f8e74f5, 
		0xffffffff, 
		0x00010000, 
		0x33333333, 
		0xfe5c9637, 
		0x5d6e46da, 
		0x6d66a299, 
		0xc618ceab, 
		0x0c7fff00, 
		0xce2e666f, 
		0x40c00000, 
		0xc2580000, 
		0xc37b2e82, 
		0xb7fdc82c, 
		0x2c0472d1, 
		0xaaaaaaaa, 
		0x42400000, 
		0x80800001, 
		0xc2240000, 
		0x420c0000, 
		0x41300000, 
		0x0cb5d3a3, 
		0x80000000, 
		0x2b974e94, 
		0xffbfffff, 
		0x1fe7df3c, 
		0xc2f9c44c, 
		0x41f80000, 
		0x0e003fff, 
		0xd3c28b61, 
		0xc2300000, 
		0x54b926f4, 
		0x846020d7, 
		0x0d00fff0, 
		0x426c0000, 
		0xc1000000, 
		0x80000000, 
		0x77cf0422, 
		0x5cea7485, 
		0x3f800000, 
		0xffffffff, 
		0x007ffffe, 
		0x80000000, 
		0xa45d6653, 
		0xf458cdf0, 
		0x80000000, 
		0x3a513d0b, 
		0x85e03c06, 
		0x0e000001, 
		0x0f7fffff, 
		0x0e000001, 
		0x007fffff, 
		0x390ac37c, 
		0xa6125be1, 
		0x00000000, 
		0x424c0000, 
		0x80000002, 
		0x41200000, 
		0x7127c142, 
		0x7fc00001, 
		0x420c0000, 
		0xac44bc08, 
		0xb451c010, 
		0xa878d20e, 
		0x42400000, 
		0x804957bc, 
		0x0c7ffe00, 
		0xda3846ed, 
		0x00000002, 
		0x0e000001, 
		0x424c0000, 
		0x0e07ffff, 
		0x42000000, 
		0x80000001, 
		0xffc00000, 
		0xbf94a652, 
		0xfbe3817b, 
		0x00400000, 
		0xc2240000, 
		0x41a00000, 
		0xff800000, 
		0xf09552da, 
		0x41927728, 
		0x8062524a, 
		0x00616284, 
		0x01cb9a9e, 
		0xc0a00000, 
		0x6155f729, 
		0x0c7ffffe, 
		0xff800000, 
		0xff7fffff, 
		0xc2340000, 
		0x007fffff, 
		0xc1880000, 
		0x0e01ffff, 
		0xcb000000, 
		0x0e07ffff, 
		0xff7fffff, 
		0x8f7ffffe, 
		0x9af53c0d, 
		0xcb8c4b40, 
		0xff7ffffe, 
		0xafa3d717, 
		0x41700000, 
		0x9c618629, 
		0x0e0001ff, 
		0x00008000, 
		0xffffffff, 
		0xb8f8a465, 
		0xaaaaaaaa, 
		0x0c600000, 
		0x7f800000, 
		0xffc00001, 
		0x80000000, 
		0x41800000, 
		0xc1f80000, 
		0x41e80000, 
		0xff932ecb, 
		0xbf028f5c, 
		0xa4b60cdc, 
		0xe8f0402b, 
		0x00000000, 
		0x00000000, 
		0x0c600000, 
		0x42780000, 
		0xf5be35e2, 
		0x41000000, 
		0x0c7fc000, 
		0x7b731a42, 
		0x420c0000, 
		0x80800001, 
		0x7f7fffff, 
		0xe4190bfc, 
		0x084f1a6a, 
		0xffb5d8ca, 
		0x80000004, 
		0x80000002, 
		0x80800000, 
		0x7fc00001, 
		0x2832b7ef, 
		0x0e0003ff, 
		0xa8d14895, 
		0x69f691ca, 
		0x00000001, 
		0x42340000, 
		0x7f7ffffe, 
		0xda8ac049, 
		0xbf028f5c, 
		0x80000000, 
		0x80800003, 
		0xff800000, 
		0x8c6a0493, 
		0x421c0000, 
		0xe6f054e0, 
		0xf3f312ae, 
		0x00000800, 
		0x00008000, 
		0x80000001, 
		0x0c400000, 
		0x80800002, 
		0xff800000, 
		0x7f7fffff, 
		0xc2780000, 
		0x42280000, 
		0xc1b00000, 
		0x00000000, 
		0x6fe34389, 
		0x8f7ffffe, 
		0x0e000003, 
		0x662f8130, 
		0x00000002, 
		0x614d7cec, 
		0x80040000, 
		0x00000000, 
		0x42140000, 
		0x00800001, 
		0x80010000, 
		0xd38996fa, 
		0x00100000, 
		0x80002000, 
		0x80800000, 
		0xc1800000, 
		0x61b6c637, 
		0x41700000, 
		0x0c7f0000, 
		0x5bc2e7f9, 
		0x7f000000, 
		0x005d7086, 
		0x00000200, 
		0x80000020, 
		0xb92418a7, 
		0xc1f80000, 
		0xce0fd440, 
		0x42040000, 
		0x0e7fffff, 
		0x0e0000ff, 
		0x0bce4ded, 
		0x80800000, 
		0xc0000000, 
		0x0d00fff0, 
		0x73b784ed, 
		0x80000000, 
		0xcb000000, 
		0x80000400, 
		0x80000800, 
		0x80800001, 
		0x426c0000, 
		0x00200000, 
		0xc2180000, 
		0x4bd9923a, 
		0x80000000, 
		0x0c7c0000, 
		0x4c78f23d, 
		0x0e3fffff, 
		0xc2040000, 
		0x0c555288, 
		0x80001000, 
		0x425c0000, 
		0x4cf3ff2a, 
		0x296d67b9, 
		0x0c7fc000, 
		0xd789e72e, 
		0x8f7fffff, 
		0x6deeca7d, 
		0x0e00003f, 
		0x80000400, 
		0x42180000, 
		0x41980000, 
		0x0f7ffffe, 
		0x254d2778, 
		0xc2040000, 
		0x0c7fe000, 
		0x0e000007, 
		0xfa0274bc, 
		0x80000000, 
		0xc2280000, 
		0x6c1df7a2, 
		0x8f7ffffe, 
		0x1c807cc1, 
		0xf34fb7b5, 
		0xf2d5f7ee, 
		0x00040000, 
		0x0c780000, 
		0xd4652d74, 
		0x481b2718, 
		0x0c7c0000, 
		0x7f800000, 
		0x00000002, 
		0x7fc00000, 
		0xc2000000, 
		0x40a00000, 
		0x80800000, 
		0xc1600000, 
		0xffc00000, 
		0x41500000, 
		0x00000002, 
		0xc1880000, 
		0x42140000, 
		0x3f800000, 
		0x7f800000, 
		0x0c7c0000, 
		0x41600000, 
		0x80000000, 
		0x00000001, 
		0x80000040, 
		0x0e0fffff, 
		0x939640e8, 
		0xc1500000, 
		0x00001000, 
		0x001ab828, 
		0x7f800000, 
		0xc2400000, 
		0xf38b8791, 
		0x40400000, 
		0xc1d00000, 
		0x7f7fffff, 
		0xc1200000, 
		0x75be3e92, 
		0x10c6b428, 
		0x0f7fffff, 
		0x8f7ffffe, 
		0xc0e00000, 
		0x42400000, 
		0x0c7ffc00, 
		0x7f7ffffe, 
		0x7fa69a1f, 
		0x80000080, 
		0xc2080000, 
		0x80000000, 
		0x1ce12bce, 
		0x00800001, 
		0x00800003, 
		0x41c80000, 
		0x80800000, 
		0x0c7f8000, 
		0xfabb0922, 
		0x7fa8a25e, 
		0x42480000, 
		0x0e001fff, 
		0x8f7fffff, 
		0x42240000, 
		0x80000200, 
		0x41200000, 
		0x41e00000, 
		0x8caa763c, 
		0xff8a605e, 
		0x5319e2c9, 
		0xc97cccc1, 
		0xf8d8d2af, 
		0xcf09bfdc, 
		0xc1f80000, 
		0x00000000, 
		0x05e02a26, 
		0x80020000, 
		0x0f7fffff, 
		0xff800000, 
		0xe5ff20c0, 
		0x00000000, 
		0x00000000, 
		0x2453663e, 
		0x0c7ffffc, 
		0xbc3cfb9a, 
		0x80800001, 
		0x4b000000, 
		0xc1880000, 
		0x0e7fffff, 
		0xc2580000, 
		0xdc5a59bf, 
		0x0e000007, 
		0x7fb42c00, 
		0x42280000, 
		0xbd686b6d, 
		0x41100000, 
		0x00000000, 
		0xc1e80000, 
		0x3bdc785b, 
		0xfe593b60, 
		0xc1a80000, 
		0x5da3916a, 
		0xc2340000, 
		0xc0400000, 
		0xc1500000, 
		0x00000001, 
		0x78e0fe7d, 
		0x41d80000, 
		0x00800000, 
		0xc1700000, 
		0xc0c00000, 
		0x0e0003ff, 
		0x00000000, 
		0x00284bdd, 
		0xc1700000, 
		0x00000000, 
		0xc1c80000, 
		0x89ce02c9, 
		0xc0c00000, 
		0x007fffff, 
		0x598ec647, 
		0x80000004, 
		0x31d2a5de, 
		0x422c0000, 
		0xc1700000, 
		0x423c0000, 
		0x56cefba5, 
		0x7b4e5d7b, 
		0xc1880000, 
		0xc6c7c359, 
		0x7fbe4036, 
		0x8f7ffffd, 
		0x80800001, 
		0xbf028f5c, 
		0x42380000, 
		0x00400000, 
		0x42400000, 
		0x00000100, 
		0x70401fa4, 
		0x2a4403e7, 
		0x00080000, 
		0xc27c0000, 
		0xc2440000, 
		0x00800001, 
		0xff800000, 
		0xe83f5828, 
		0x41300000, 
		0x00800001, 
		0x80000000, 
		0x7fc00000, 
		0xc1e00000, 
		0x00000002, 
		0x80000020, 
		0xc21c0000, 
		0x0c700000, 
		0xff800000, 
		0xc2440000, 
		0xad10bc6d, 
		0x7a7d1a97, 
		0x41980000, 
		0x7f000000, 
		0x80000000, 
		0x0c7ffff8, 
		0xc2440000, 
		0xc1e80000, 
		0x72879bb1, 
		0x3f028f5c, 
		0x41b80000, 
		0xffc00001, 
		0x423c0000, 
		0xd7733f97, 
		0xff800000, 
		0x00000000, 
		0xc2000000, 
		0xc2380000, 
		0x80000000, 
		0x41800000, 
		0x00000002, 
		0xc2740000, 
		0x41000000, 
		0x41900000, 
		0x27db9903, 
		0x80000000, 
		0x42540000, 
		0x807ffffe, 
		0x80800001, 
		0x00100000, 
		0x40800000, 
		0x7a82a6ab, 
		0xfd370a7b, 
		0x64e1ba48, 
		0x00800000, 
		0xc0c00000, 
		0x00800002, 
		0x4949d081, 
		0xc1980000, 
		0xae4785b8, 
		0x7f800000, 
		0x80000000, 
		0x424c0000, 
		0x0c7fe000, 
		0x9defe39b, 
		0xff800000, 
		0x00800002, 
		0xaaaaaaaa, 
		0x00000004, 
		0xaaaaaaaa, 
		0x007ffffe, 
		0x6a568c95, 
		0xfde4ce7a, 
		0xae83bfae, 
		0x4feb3940, 
		0x426c0000, 
		0xf6fef18c, 
		0x00000000, 
		0x278f0540, 
		0x0e0007ff, 
		0x75442960, 
		0x00010000, 
		0xd4cd0ab9, 
		0x80800002, 
		0x80001000, 
		0xf30a9ed4, 
		0x41d00000, 
		0x00800000, 
		0x41300000, 
		0xc1a80000, 
		0x81a9f8cb, 
		0xc1400000, 
		0xbf800000, 
		0x0c7c0000, 
		0xc1c80000, 
		0x80004000, 
		0x80000001, 
		0xa40af85a, 
		0x80000000, 
		0x42080000, 
		0x007fffff, 
		0x42140000, 
		0x1813c311, 
		0xc2080000, 
		0xc1d80000, 
		0x00000400, 
		0x80000010, 
		0x00800000, 
		0x41200000, 
		0x00000000, 
		0x00000000, 
		0xc1e00000, 
		0x421c0000, 
		0x41e80000, 
		0xede90d3e, 
		0x42080000, 
		0xc2200000, 
		0x42340000, 
		0x56a8f4f8, 
		0x425c0000, 
		0xd392dde1, 
		0xe533c464, 
		0xf6fcaa71, 
		0x8a8165bd, 
		0xc2700000, 
		0x41000000, 
		0xc2400000, 
		0xc9391b8c, 
		0x00800000, 
		0xffb94ecb, 
		0x42080000, 
		0x42600000, 
		0x0c7ffffe, 
		0x80200000, 
		0xc1980000, 
		0xc2740000, 
		0x0f7ffffd, 
		0xed09d29d, 
		0xc1d00000, 
		0x80000080, 
		0x6f7bc540, 
		0x42100000, 
		0x692b17a3, 
		0x007fffff, 
		0x41400000, 
		0x7de35344, 
		0x41a00000, 
		0x424c0000, 
		0x00000000, 
		0xff800000, 
		0xff800000, 
		0x7341c5b9, 
		0x7f800000, 
		0x7dcf7be8, 
		0x1276889f, 
		0x00008000, 
		0x66b5cdb3, 
		0x6d88e18a, 
		0x0e0007ff, 
		0x42680000, 
		0xcd8902a2, 
		0x80800000, 
		0x00800000, 
		0x40e00000, 
		0x80000000, 
		0xff800000, 
		0xc1b00000, 
		0x80800001, 
		0x7f9a7d2c, 
		0xc1100000, 
		0x0c780000, 
		0x0c65971a, 
		0x0c7fc000, 
		0x80800003, 
		0x467e3807, 
		0xffbfffff, 
		0x7f86b20d, 
		0x42680000, 
		0x00000001, 
		0xdeb00bae, 
		0x0c57eba5, 
		0xc2640000, 
		0x00000000, 
		0x0e000001, 
		0xdacdb147, 
		0xc1600000, 
		0x00200000, 
		0x00000008, 
		0xc23c0000, 
		0x0e00003f, 
		0xc26c0000, 
		0x80000004, 
		0x007fffff, 
		0xc1a80000, 
		0xf2565bef, 
		0x00200000, 
		0xdfcb1eff, 
		0x00000000, 
		0xc1800000, 
		0x44f86736, 
		0xff7fffff, 
		0x815ecf36, 
		0xc1200000, 
		0xa3021706, 
		0x4b8c4b40, 
		0x7f800000, 
		0x33333333, 
		0x80000100, 
		0x80ffffff, 
		0xc2300000, 
		0x7f80632e, 
		0xa70fb2f4, 
		0x00800001, 
		0x80000000, 
		0x3cc36a18, 
		0xd2cebca7, 
		0x7fc00001, 
		0x8c4c561c, 
		0x91420269, 
		0x0e0000ff, 
		0x00011111, 
		0x80000000, 
		0x00000000, 
		0x0e0fffff, 
		0x00800002, 
		0xc1200000, 
		0xe70811c2, 
		0xee923b9a, 
		0xff800000, 
		0xc2140000, 
		0x40800000, 
		0x422c0000, 
		0xbf800001, 
		0x420c0000, 
		0x42600000, 
		0x92065425, 
		0x807fffff, 
		0x3cd2521c, 
		0x0e3fffff, 
		0xb3ddcef8, 
		0x00000000, 
		0x42640000, 
		0xca76c184, 
		0x41700000, 
		0x518422fd, 
		0x80800000, 
		0x0c7fffc0, 
		0x42040000, 
		0x963ce548, 
		0xff7ffffe, 
		0x0c7f8000, 
		0x0c600000, 
		0xc1900000, 
		0x00800001, 
		0x0048594b, 
		0xffc00001, 
		0xc1980000, 
		0x0e1da1d4, 
		0x41a00000, 
		0x0e00000f, 
		0x7f800000, 
		0x421c0000, 
		0x6ba816f0, 
		0xcccccccc, 
		0xcd86575e, 
		0xff800000, 
		0x967d2013, 
		0xcccccccc, 
		0x40400000, 
		0x41980000, 
		0x00000000, 
		0x7f7ffffe, 
		0x00000000, 
		0xd5323a4e, 
		0x00004000, 
		0x42780000, 
		0xcffcf7a8, 
		0xffe3b302, 
		0x80000000, 
		0xc27c0000, 
		0x8350ecd0, 
		0xf3d2c5fb, 
		0x00000002, 
		0xc2540000, 
		0x0e007fff, 
		0x7f800001, 
		0x0e0000ff, 
		0xce490a96, 
		0xc26c0000, 
		0x0f7fffff, 
		0x7714f41b, 
		0x803fc8ff, 
		0x7fc67b92, 
		0xea10c7e4, 
		0x8f7ffffc, 
		0x80002000, 
		0x6006d56c, 
		0xfb47f464, 
		0xffaba1fb, 
		0x00800003, 
		0x80000000, 
		0x42480000, 
		0xbb542be5, 
		0x0a0086dc, 
		0x8011b582, 
		0x986c7011, 
		0xd4849b25, 
		0xff800000, 
		0x0e00001f, 
		0x38b66f7b, 
		0x9cdc9959, 
		0xc1980000, 
		0xa73aa0a3, 
		0xb88eee21, 
		0x5a1cc234, 
		0xc1000000, 
		0x80000000, 
		0xc23c0000, 
		0x0e003fff, 
		0xff800000, 
		0x0e00ffff, 
		0xc0c00000, 
		0x7bd3c39b, 
		0x0c7fff00, 
		0x1f25cb6c, 
		0x7fc00001, 
		0x80800002, 
		0x0e01ffff, 
		0x0c7ffff8, 
		0x0c7fff00  
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
		0x00000335, 
		0x0000027d, 
		0x000001ab, 
		0x00000333, 
		0x00000233, 
		0x0000029f, 
		0x00000261, 
		0x000000b9, 
		0x00000113, 
		0x000001d3, 
		0x0000016b, 
		0x00000261, 
		0x00000225, 
		0x000001d9, 
		0x00000137, 
		0x000002f5, 
		0x0000005d, 
		0x000001e1, 
		0x000001ff, 
		0x00000187, 
		0x00000095, 
		0x00000157, 
		0x00000355, 
		0x00000209, 
		0x000003c7, 
		0x000000f3, 
		0x0000032b, 
		0x00000133, 
		0x000003a3, 
		0x00000347, 
		0x00000137, 
		0x0000032d, 
		0x00000001, 
		0x000003f5, 
		0x00000187, 
		0x000003d7, 
		0x0000033b, 
		0x0000014d, 
		0x000003bf, 
		0x0000001d, 
		0x00000303, 
		0x00000047, 
		0x000000b9, 
		0x0000023f, 
		0x000002bb, 
		0x00000181, 
		0x000000d5, 
		0x000000b7, 
		0x00000201, 
		0x00000085, 
		0x00000267, 
		0x00000081, 
		0x00000093, 
		0x000003eb, 
		0x000000ef, 
		0x00000027, 
		0x0000032f, 
		0x0000031b, 
		0x00000093, 
		0x0000003d, 
		0x00000213, 
		0x00000319, 
		0x000001a7, 
		0x00000047, 
		0x0000009d, 
		0x00000279, 
		0x000002ed, 
		0x000001d1, 
		0x0000039d, 
		0x000001b7, 
		0x000001e7, 
		0x0000032d, 
		0x00000109, 
		0x00000205, 
		0x000000a3, 
		0x000002cd, 
		0x0000037b, 
		0x000001a9, 
		0x000001a9, 
		0x000002c1, 
		0x00000243, 
		0x000003a7, 
		0x000000cd, 
		0x000003b9, 
		0x00000091, 
		0x0000028d, 
		0x0000001f, 
		0x00000021, 
		0x000002ff, 
		0x0000024f, 
		0x00000087, 
		0x00000099, 
		0x000003bd, 
		0x000002ad, 
		0x0000028d, 
		0x000002f5, 
		0x00000245, 
		0x000003e3, 
		0x0000030f, 
		0x00000077, 
		0x000001f9, 
		0x00000335, 
		0x0000012b, 
		0x00000049, 
		0x00000315, 
		0x0000003b, 
		0x0000013f, 
		0x00000157, 
		0x00000193, 
		0x0000016b, 
		0x0000009f, 
		0x000002d1, 
		0x00000143, 
		0x000000ed, 
		0x000003b7, 
		0x000000a5, 
		0x000001cd, 
		0x0000007f, 
		0x00000287, 
		0x000000dd, 
		0x00000291, 
		0x0000026d, 
		0x00000335, 
		0x000000a7, 
		0x00000159, 
		0x00000057, 
		0x000002e3, 
		0x00000315, 
		0x00000357, 
		0x000002cf, 
		0x000003c7, 
		0x000001d1, 
		0x00000123, 
		0x000003ef, 
		0x000002c3, 
		0x0000022f, 
		0x00000123, 
		0x00000123, 
		0x00000193, 
		0x00000035, 
		0x000002b3, 
		0x0000024b, 
		0x000003d3, 
		0x000001f7, 
		0x0000018b, 
		0x00000031, 
		0x0000003b, 
		0x0000021b, 
		0x0000019d, 
		0x0000018d, 
		0x000002a5, 
		0x000001c7, 
		0x000001a7, 
		0x0000000d, 
		0x00000151, 
		0x0000014b, 
		0x000001f1, 
		0x000000df, 
		0x00000235, 
		0x000003db, 
		0x0000010b, 
		0x00000077, 
		0x0000038d, 
		0x0000001b, 
		0x00000245, 
		0x0000002d, 
		0x00000283, 
		0x000001d3, 
		0x00000337, 
		0x000000af, 
		0x000003ad, 
		0x000001af, 
		0x0000002b, 
		0x000002ef, 
		0x00000033, 
		0x0000003f, 
		0x0000003d, 
		0x00000365, 
		0x00000287, 
		0x000001eb, 
		0x0000029b, 
		0x000000ed, 
		0x000003f7, 
		0x000003f1, 
		0x000003c1, 
		0x000002b9, 
		0x00000255, 
		0x000003d1, 
		0x0000002b, 
		0x0000030d, 
		0x00000289, 
		0x000001b3, 
		0x00000365, 
		0x000002ad, 
		0x00000111, 
		0x000001e9, 
		0x0000018b, 
		0x00000001, 
		0x0000038f, 
		0x000003b1, 
		0x000002a1, 
		0x000002f5, 
		0x00000399, 
		0x0000027d, 
		0x000002bf, 
		0x00000057, 
		0x000000bd, 
		0x000000a7, 
		0x00000091, 
		0x00000239, 
		0x00000207, 
		0x00000035, 
		0x00000257, 
		0x00000357, 
		0x0000039d, 
		0x00000105, 
		0x0000027d, 
		0x00000359, 
		0x0000012f, 
		0x00000119, 
		0x0000023d, 
		0x00000063, 
		0x0000002d, 
		0x00000321, 
		0x000002c3, 
		0x000001fb, 
		0x000003f9, 
		0x00000139, 
		0x00000283, 
		0x00000309, 
		0x00000241, 
		0x0000007d, 
		0x000000dd, 
		0x00000041, 
		0x000002d7, 
		0x00000329, 
		0x0000027f, 
		0x000001bd, 
		0x00000099, 
		0x0000039d, 
		0x000001fd, 
		0x000003b5, 
		0x00000041, 
		0x00000035, 
		0x000001c1, 
		0x000000e1, 
		0x0000018d, 
		0x00000271, 
		0x000003c7, 
		0x00000367, 
		0x000001bf, 
		0x000002f3, 
		0x000002e7, 
		0x00000259, 
		0x00000305, 
		0x000001e9  
};

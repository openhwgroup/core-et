/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "minion.h"

volatile uint32_t m_op[256] __attribute__ ((aligned (4096))) = {
		// f0 is designated 0 register
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		// f1 is designated offset for memory ops
		0x000002dc,
		0x00000378,
		0x000001d4,
		0x000002a4,
		0x00000264,
		0x0000005c,
		0x000000e8,
		0x000000ac,
		// f2
		0x42640000,
		0x80000000,
		0x42340000,
		0x41a00000,
		0x3f800000,
		0x41b80000,
		0x80000000,
		0x41980000,
		// f3
		0x42440000,
		0x427c0000,
		0x42640000,
		0x41e80000,
		0x42200000,
		0x40800000,
		0x422c0000,
		0x42700000,
		// f4
		0x41300000,
		0x40a00000,
		0x42380000,
		0x42700000,
		0x421c0000,
		0x42700000,
		0x41300000,
		0x41a00000,
		// f5
		0x42640000,
		0x41d80000,
		0x40e00000,
		0x42780000,
		0x42080000,
		0x40800000,
		0x42500000,
		0x422c0000,
		// f6
		0x8000ffff,
		0x41b80000,
		0x41b00000,
		0x41a80000,
		0x42340000,
		0x42100000,
		0xffa00000,
		0xffa00000,
		// f7
		0x42280000,
		0x42180000,
		0x42340000,
		0x41880000,
		0x40400000,
		0x41300000,
		0x41400000,
		0x00000000,
		// f8
		0xff800000,
		0x40800000,
		0x42400000,
		0x41300000,
		0x42200000,
		0x41500000,
		0x40800000,
		0x41a00000,
		// f9
		0x42580000,
		0x40400000,
		0x42440000,
		0x41e80000,
		0x7fa00000,
		0x42700000,
		0x3f800000,
		0x00000000,
		// f10
		0x41100000,
		0x41000000,
		0x7fa00000,
		0x41a00000,
		0x41800000,
		0x41100000,
		0x00000000,
		0x41f80000,
		// f11
		0xffa00000,
		0x424c0000,
		0x41d80000,
		0x426c0000,
		0x42000000,
		0x40000000,
		0x40c00000,
		0x421c0000,
		// f12
		0x41c00000,
		0x42380000,
		0x42740000,
		0x41c80000,
		0x42740000,
		0x42780000,
		0xffa00000,
		0x41e80000,
		// f13
		0x40000000,
		0x42400000,
		0x42640000,
		0x00000000,
		0x40c00000,
		0x41880000,
		0x7f800000,
		0x41b80000,
		// f14
		0x42780000,
		0x42200000,
		0x40e00000,
		0xff800000,
		0xff800000,
		0x40000000,
		0x425c0000,
		0x41e80000,
		// f15
		0x41900000,
		0x42700000,
		0x41c80000,
		0x41b00000,
		0x7fc00000,
		0x42680000,
		0x40e00000,
		0x41100000,
		// f16
		0x7fc00000,
		0x42340000,
		0x00000000,
		0x41600000,
		0x422c0000,
		0x41f00000,
		0x41000000,
		0x41c00000,
		// f17
		0x42280000,
		0x41f00000,
		0x40e00000,
		0x42500000,
		0x41880000,
		0x42480000,
		0x425c0000,
		0x42780000,
		// f18
		0x427c0000,
		0x41e80000,
		0x42640000,
		0xffc00000,
		0x41900000,
		0x420c0000,
		0x7fa00000,
		0x41e00000,
		// f19
		0x42640000,
		0x42600000,
		0x42540000,
		0x425c0000,
		0x423c0000,
		0xffc00000,
		0x41f80000,
		0x427c0000,
		// f20
		0x00000000,
		0x42080000,
		0x42280000,
		0x42300000,
		0x422c0000,
		0x40a00000,
		0x0000ffff,
		0x0000ffff,
		// f21
		0x41980000,
		0x42600000,
		0x7fa00000,
		0x41f00000,
		0x42740000,
		0x42180000,
		0x42540000,
		0x427c0000,
		// f22
		0x41a80000,
		0x42140000,
		0x41b80000,
		0x80000000,
		0x420c0000,
		0x7fa00000,
		0x41980000,
		0x42580000,
		// f23
		0x41b80000,
		0x41e80000,
		0x42100000,
		0x00000000,
		0x7fc00000,
		0x41100000,
		0x41e80000,
		0x40800000,
		// f24
		0x42140000,
		0x3f800000,
		0x42640000,
		0x41e00000,
		0x42240000,
		0x41900000,
		0x00000000,
		0x3f800000,
		// f25
		0x42680000,
		0xffc00000,
		0x42740000,
		0x40800000,
		0x421c0000,
		0x40a00000,
		0xffc00000,
		0x426c0000,
		// f26
		0x41d00000,
		0x42380000,
		0x41700000,
		0x42100000,
		0x42280000,
		0x7fc00000,
		0x41f80000,
		0x41800000,
		// f27
		0x40000000,
		0x42480000,
		0x7fa00000,
		0x421c0000,
		0x42480000,
		0x41600000,
		0x422c0000,
		0x41b00000,
		// f28
		0x42740000,
		0x42300000,
		0x41400000,
		0x42700000,
		0x42040000,
		0x40800000,
		0x41000000,
		0x7fa00000,
		// f29
		0x7fa00000,
		0x41900000,
		0x42300000,
		0x40c00000,
		0x42500000,
		0x41d00000,
		0x42400000,
		0x40c00000,
		// f30
		0x42780000,
		0x41e00000,
		0x41100000,
		0x426c0000,
		0x42300000,
		0x42300000,
		0x42700000,
		0x40c00000,
		// f31
		0x425c0000,
		0x41980000,
		0x41700000,
		0x42140000,
		0x41500000,
		0x41f00000,
		0xff800000,
		0x3f800000
};

volatile uint32_t m_trans_data[72] __attribute__ ((aligned (4096))) = {
		0x3f000000,
		0x3f000001,
		0x3f000002,
		0x3f000003,
		0x3f000004,
		0x3f000005,
		0x3f000006,
		0x3f000007,
		0x3f000008,
		0x3f000009,
		0x3f00000a,
		0x3f00000b,
		0x3f00000c,
		0x3f00000d,
		0x3f00000e,
		0x3f00000f,
		0x3f000010,
		0x3f000011,
		0x3f000012,
		0x3f000013,
		0x3f000014,
		0x3f000015,
		0x3f000016,
		0x3f000017,
		0x3f000018,
		0x3f000019,
		0x3f00001a,
		0x3f00001b,
		0x3f00001c,
		0x3f00001d,
		0x3f00001e,
		0x3f00001f,
		0x3f000020,
		0x3f000021,
		0x3f000022,
		0x3f000023,
		0x3f000024,
		0x3f000025,
		0x3f000026,
		0x3f000027,
		0x3f000028,
		0x3f000029,
		0x3f00002a,
		0x3f00002b,
		0x3f00002c,
		0x3f00002d,
		0x3f00002e,
		0x3f00002f,
		0x3f000030,
		0x3f000031,
		0x3f000032,
		0x3f000033,
		0x3f000034,
		0x3f000035,
		0x3f000036,
		0x3f000037,
		0x3f000038,
		0x3f000039,
		0x3f00003a,
		0x3f00003b,
		0x3f00003c,
		0x3f00003d,
		0x3f00003e,
		0x3f00003f,
		0x00000040,
		0x00000040,
		0x00000040,
		0x00000040,
		0x00000040,
		0x00000040,
		0x00000040,
		0x00000040
};



/// Macros

#define VSNIP_RSV           : : [m_op] "r" (m_op), [m_trans_data] "r" (m_trans_data) : "x3", "x4", "x5", "x6", "x7", "x11", "x12", "x13", "x30", "x31", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"





int main() {

	uint64_t mid, tid;
	mid = get_minion_id();
	tid = get_thread_id();

	if (mid == 0 && tid == 0) { 

		C_TEST_START;

		__asm__ __volatile__ (
		"LBL_C_TEST_START_M0_T0:\n"
			"j LBL_SETUP_EXCEPTION_VECTOR_M0_T0\n"


			".align 12\n"
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
			".equ EXCP_DATA_BUS_ERROR,                  29\n"
			".equ EXCP_MCODE_EMULATION,                 30\n"
			".equ ECALL_SETUP_SCP,                      0\n"
			".equ ECALL_SWITCH_TO_M_MODE,               15\n"
			"sw x3, 64 (%[m_op])\n"
			"sw x4, 96 (%[m_op])\n"
			"sw x5, 128 (%[m_op])\n"
			"sw x6, 160 (%[m_op])\n"
		"LBL_M_MODE_E_H_M0_T0:\n"
			"csrr x5, mcause\n"      // read mcause
			"bltz x5, LBL_INTERRUPT_TABLE_M0_T0\n"
		"LBL_EXCEPTION_TABLE_M0_T0:\n"
			"li x6, -1\n"
			"srli x6, x6, 1\n"
			"and x5, x5, x6\n"
			"li x6, EXCP_INSTRUCTION_ADDRESS_MISALIGN\n"
			"beq x5, x6, LBL_INSTRUCTION_ADDRESS_MISALIGN_M0_T0\n"
			"li x6, EXCP_INSTRUCTION_ACCESS_FAULT\n"
			"beq x5, x6, LBL_INSTRUCTION_ACCESS_FAULT_M0_T0\n"
			"li x6, EXCP_ILLEGAL_INSTRUCTION\n"
			"beq x5, x6, LBL_ILLEGAL_INSTRUCTION_M0_T0\n"
			"li x6, EXCP_BREAKPOINT\n"
			"beq x5, x6, LBL_BREAKPOINT_M0_T0\n"
			"li x6, EXCP_LOAD_ADDRESS_MISALIGNED\n"
			"beq x5, x6, LBL_LOAD_ADDRESS_MISALIGNED_M0_T0\n"
			"li x6, EXCP_LOAD_ACCESS_FAULT\n"
			"beq x5, x6, LBL_LOAD_ACCESS_FAULT_M0_T0\n"
			"li x6, EXCP_STORE_OR_AMO_ADDRESS_MISALIGNED\n"
			"beq x5, x6, LBL_STORE_OR_AMO_ADDRESS_MISALIGNED_M0_T0\n"
			"li x6, EXCP_STORE_OR_AMO_ACCESS_FAULT\n"
			"beq x5, x6, LBL_STORE_OR_AMO_ACCESS_FAULT_M0_T0\n"
			"li x6, EXCP_ECALL_FROM_UMODE\n"
			"beq x5, x6, LBL_ECALL_FROM_UMODE_M0_T0\n"
			"li x6, EXCP_ECALL_FROM_SMODE\n"
			"beq x5, x6, LBL_ECALL_FROM_SMODE_M0_T0\n"
			"li x6, EXCP_ECALL_FROM_MMODE\n"
			"beq x5, x6, LBL_ECALL_FROM_MMODE_M0_T0\n"
			"li x6, EXCP_INSTRUCTION_PAGE_FAULT\n"
			"beq x5, x6, LBL_INSTRUCTION_PAGE_FAULT_M0_T0\n"
			"li x6, EXCP_LOAD_PAGE_FAULT\n"
			"beq x5, x6, LBL_LOAD_PAGE_FAULT_M0_T0\n"
			"li x6, EXCP_STORE_OR_AMO_PAGE_FAULT\n"
			"beq x5, x6, LBL_STORE_OR_AMO_PAGE_FAULT_M0_T0\n"
			"li x6, EXCP_INSTRUCTION_BUS_ERROR\n"
			"beq x5, x6, LBL_INSTRUCTION_BUS_ERROR_M0_T0\n"
			"li x6, EXCP_INSTRUCTION_ECC_ERROR\n"
			"beq x5, x6, LBL_INSTRUCTION_ECC_ERROR_M0_T0\n"
			"li x6, EXCP_LOAD_SPLIT_PAGE_FAULT\n"
			"beq x5, x6, LBL_LOAD_SPLIT_PAGE_FAULT_M0_T0\n"
			"li x6, EXCP_STORE_SPLIT_PAGE_FAULT\n"
			"beq x5, x6, LBL_STORE_SPLIT_PAGE_FAULT_M0_T0\n"
			"li x6, EXCP_DATA_BUS_ERROR\n"
			"beq x5, x6, LBL_DATA_BUS_ERROR_M0_T0\n"
			"li x6, EXCP_MCODE_EMULATION\n"
			"beq x5, x6, LBL_MCODE_EMULATION_M0_T0\n"
		"LBL_INSTRUCTION_ADDRESS_MISALIGN_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_INSTRUCTION_ACCESS_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_ILLEGAL_INSTRUCTION_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_BREAKPOINT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_LOAD_ADDRESS_MISALIGNED_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_LOAD_ACCESS_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_STORE_OR_AMO_ADDRESS_MISALIGNED_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_STORE_OR_AMO_ACCESS_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_ECALL_FROM_UMODE_M0_T0:\n"
			"li x5, 0x1800\n"      // mstatus[12:11] = mpp = 2'b11
			"csrrs   zero, mstatus, x5\n"      // mpp = m-mode
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_ECALL_FROM_SMODE_M0_T0:\n"
			"li x5, 0x1800\n"      // mstatus[12:11] = mpp = 2'b11
			"csrrs   zero, mstatus, x5\n"      // mpp = m-mode
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_ECALL_FROM_MMODE_M0_T0:\n"
			"li x6, ECALL_SWITCH_TO_M_MODE\n"
			"beq x3, x6, LBL_SWITCH_TO_M_MODE_M0_T0\n"
			"li x6, ECALL_SETUP_SCP\n"
			"beq x3, x6, LBL_SETUP_SCP_M0_T0\n"
		"LBL_SWITCH_TO_M_MODE_M0_T0:\n"
			"li x5, 0x1800\n"      // mstatus[12:11] = mpp = 2'b11
			"csrrs   zero, mstatus, x5\n"      // mpp = m-mode
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_SETUP_SCP_M0_T0:\n"
			"csrrs	x0, mcache_control, 1\n"
			"fence\n"
			"csrrs	x0, mcache_control, 3\n"
			"fence\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_INSTRUCTION_PAGE_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_LOAD_PAGE_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_STORE_OR_AMO_PAGE_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_INSTRUCTION_BUS_ERROR_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_INSTRUCTION_ECC_ERROR_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_LOAD_SPLIT_PAGE_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_STORE_SPLIT_PAGE_FAULT_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_DATA_BUS_ERROR_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_MCODE_EMULATION_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_INTERRUPT_TABLE_M0_T0:\n"
			"j LBL_RET_TO_CALLER_M0_T0\n"
		"LBL_RET_TO_CALLER_M0_T0:\n"
			"csrrw x0, mcause,x0\n"      // mcause = 0
			"csrrw x0, mtval,x0\n"      // mtval = 0
			"csrr	x5, mepc\n"       // x5 = mepc
			"addi	x5,x5,4\n"        // x5+=4
			"csrw	 mepc,x5\n"       // mepc = x5
			"lw x3, 64 (%[m_op])\n"
			"lw x4, 96 (%[m_op])\n"
			"lw x5, 128 (%[m_op])\n"
			"lw x6, 160 (%[m_op])\n"
			"mret\n"                    // return


			".align 12\n"
		"LBL_SETUP_EXCEPTION_VECTOR_M0_T0:\n"
			"la x5, LBL_M_MODE_E_H_M0_T0\n"
			"csrrw x0, mtvec, x5\n"     // mtvec = M_MODE_E_H
			"li x5, 0xffffffffffffffff\n"
			"mova.m.x x5\n"
			"flw.ps f0,  0 (%[m_op])\n"
			"flw.ps f1,  0 (%[m_op])\n"
			"flw.ps f2,  0 (%[m_op])\n"
			"flw.ps f3,  0 (%[m_op])\n"
			"flw.ps f4,  0 (%[m_op])\n"
			"flw.ps f5,  0 (%[m_op])\n"
			"flw.ps f6,  0 (%[m_op])\n"
			"flw.ps f7,  0 (%[m_op])\n"
			"flw.ps f8,  0 (%[m_op])\n"
			"flw.ps f9,  0 (%[m_op])\n"
			"flw.ps f10,  0 (%[m_op])\n"
			"flw.ps f11,  0 (%[m_op])\n"
			"flw.ps f12,  0 (%[m_op])\n"
			"flw.ps f13,  0 (%[m_op])\n"
			"flw.ps f14,  0 (%[m_op])\n"
			"flw.ps f15,  0 (%[m_op])\n"
			"flw.ps f16,  0 (%[m_op])\n"
			"flw.ps f17,  0 (%[m_op])\n"
			"flw.ps f18,  0 (%[m_op])\n"
			"flw.ps f19,  0 (%[m_op])\n"
			"flw.ps f20,  0 (%[m_op])\n"
			"flw.ps f21,  0 (%[m_op])\n"
			"flw.ps f22,  0 (%[m_op])\n"
			"flw.ps f23,  0 (%[m_op])\n"
			"flw.ps f24,  0 (%[m_op])\n"
			"flw.ps f25,  0 (%[m_op])\n"
			"flw.ps f26,  0 (%[m_op])\n"
			"flw.ps f27,  0 (%[m_op])\n"
			"flw.ps f28,  0 (%[m_op])\n"
			"flw.ps f29,  0 (%[m_op])\n"
			"flw.ps f30,  0 (%[m_op])\n"
			"flw.ps f31,  0 (%[m_op])\n"
			"fence\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ (

		"LBL_VPU_SEQ_START_0:\n"
			".equ TOTAL_TRANS_VECTORS, 0x1000000 \n"
			".equ VECTORS_PER_ITER, 0x40 \n"
			"li x5, 0xffffffffffffffff\n"
			"mova.m.x x5\n"
			"flw.ps f2,  0 (%[m_trans_data])\n"
			"flw.ps f3,  32 (%[m_trans_data])\n"
			"flw.ps f4,  64 (%[m_trans_data])\n"
			"flw.ps f5,  96 (%[m_trans_data])\n"
			"flw.ps f6,  128 (%[m_trans_data])\n"
			"flw.ps f7,  160 (%[m_trans_data])\n"
			"flw.ps f8,  192 (%[m_trans_data])\n"
			"flw.ps f9,  224 (%[m_trans_data])\n"
			"flw.ps f10,  256 (%[m_trans_data])\n"
			"li x5, TOTAL_TRANS_VECTORS \n"
			"li x6, VECTORS_PER_ITER \n"

		"LBL_VPU_TRANS_DATA_ITER_0:\n"
			"FCVT.PS.PW f12, f2\n"
			"FCVT.PS.PW f13, f3\n"
			"FCVT.PS.PW f14, f4\n"
			"FCVT.PS.PW f15, f5\n"
			"FCVT.PS.PW f16, f6\n"
			"FCVT.PS.PW f17, f7\n"
			"FCVT.PS.PW f18, f8\n"
			"FCVT.PS.PW f19, f9\n"
			"fadd.pi f2, f2, f10\n"
			"fadd.pi f3, f3, f10\n"
			"fadd.pi f4, f4, f10\n"
			"fadd.pi f5, f5, f10\n"
			"fadd.pi f6, f6, f10\n"
			"fadd.pi f7, f7, f10\n"
			"fadd.pi f8, f8, f10\n"
			"fadd.pi f9, f9, f10\n"
			"sub x5, x5, x6\n"
			"bne x5, x0,  LBL_VPU_TRANS_DATA_ITER_0\n"

			"fence\n"

			"fence\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ (
		"LBL_C_TEST_PASS_M0_T0:\n"
			VSNIP_RSV
		);
		C_TEST_PASS;
		return 0;
	};
}

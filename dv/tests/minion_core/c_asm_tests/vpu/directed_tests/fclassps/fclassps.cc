/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


/// Macros

#define VSNIP_RSV           : : [operands] "r" (operands): "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
#define TSNIP_RSV          : : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x31"
#define RESET_FCSR            __asm__ __volatile__ ( "csrw    fcsr, zero\n" VSNIP_RSV);
#define RESET_TENSOR_COOP     __asm__ __volatile__ ( "csrw    0x804, zero\n" VSNIP_RSV);

#define ECALL                 __asm__ __volatile__ ( "ecall\n" VSNIP_RSV);
#define MRET                  __asm__ __volatile__ ( "mret\n" VSNIP_RSV);

#define TENSOR_WAIT_TL0       __asm__ __volatile__ ( "csrwi   0x830, 0x0\n" VSNIP_RSV);
#define TENSOR_WAIT_TL1       __asm__ __volatile__ ( "csrwi   0x830, 0x1\n" VSNIP_RSV);
#define TENSOR_WAIT_COP       __asm__ __volatile__ ( "csrwi   0x830, 0x6\n" VSNIP_RSV);
#define TENSOR_WAIT_FMA       __asm__ __volatile__ ( "csrwi   0x830, 0x7\n" VSNIP_RSV);
#define TENSOR_WAIT_STR       __asm__ __volatile__ ( "csrwi   0x830, 0x8\n" VSNIP_RSV);
#define TENSOR_WAIT_RED       __asm__ __volatile__ ( "csrwi   0x830, 0x9\n" VSNIP_RSV);
#define TENSOR_WAIT_QNT       __asm__ __volatile__ ( "csrwi   0x830, 0xa\n" VSNIP_RSV);
#define SET_TENSOR_MASK    \
   __asm__ __volatile__ ( \
         "li x5, 0xFFFF\n" \
         "csrrw zero, 0x805, x5\n" \
         VSNIP_RSV);

#define SET_SCP_CTL    \
   __asm__ __volatile__ ( \
         "csrrs	x0, 0x7e0, 3\n" \
         VSNIP_RSV);

int main() {

	uint64_t mid, tid;
	mid = get_minion_id();
	tid = get_thread_id();
	if (mid != 0 && tid != 0) { C_TEST_FAIL; return -1;};
	
	volatile uint32_t operands[248] __attribute__ ((aligned (4096))) = {
                        0x34008400,
                        0x41000076,
                        0x01007FFB,
                        0x0D80013E,
                        0x1F7F8008,
                        0x33A9D9DB,
                        0x39FFFF07,
                        0x3C6426F6,
                        0x3D9FF7FF,
                        0x3F8008FF,
                        0x3FFFFFC2,
                        0x45E68D66,
                        0x4BCB12EF,
                        0x4F80000B,
                        0x4FE061BA,
                        0x52FFFFF9,
                        0x5DA5BBF7,
                        0x64005FFE,
                        0x7CFDEFFE,
                        0x8134B2D7,
                        0xA0000F7F,
                        0xAC00081E,
                        0xB38DE576,
                        0xB980201F,
                        0xBC7F03FF,
                        0xBDF7FFFB,
                        0xBDFF803E,
                        0xBE5C5E05,
                        0xBE8007EF,
                        0xBF94D20A,
                        0xC21003FE,
                        0xC3E00000,
                        0xC5FFF9FF,
                        0xC76FF800,
                        0xC98E0000,
                        0xCB8FFF7F,
                        0xCF7C2357,
                        0xCF7FFFFE,
                        0xCF800000,
                        0xCF800001,
                        0xCFFFFFFF,
                        0xDE000000,
                        0xDE000001,
                        0xDE7FFFFF,
                        0xDE800000,
                        0xDE800001,
                        0xDEFFFFFE,
                        0xDF000000,
                        0x00000000,
                        0x00000001,
                        0x00000002,
                        0x007ffffe,
                        0x007fffff,
                        0x3f800000,
                        0x3f800001,
                        0x00800000,
                        0x00800001,
                        0x00ffffff,
                        0x7f000000,
                        0x7f7fffff,
                        0x7f7ffffe,
                        0x7f800001,
                        0x7fbfffff,
                        0x7fc00000,
                        0x7fc00001,
                        0x7fffffff,
                        0x7f800000,
                        0x80000000,
                        0x80000001,
                        0x80000002,
                        0x807ffffe,
                        0x807fffff,
                        0xbf800000,
                        0xbf800001,
                        0x80800000,
                        0x80800001,
                        0x80ffffff,
                        0xff000000,
                        0xff7fffff,
                        0xff7ffffe,
                        0xff800001,
                        0xffbfffff,
                        0xffc00000,
                        0xffc00001,
                        0xffffffff,
                        0xff800000,
                        0xAAAAAAAA,
                        0x55555555,
                        0xCCCCCCCC,
                        0x33333333,
                        0x0e000001,
                        0x0e000002,
                        0x0e000004,
                        0x0e000008,
                        0x0e000010,
                        0x0e000020,
                        0x0e000040,
                        0x0e000080,
                        0x0e000100,
                        0x0e000200,
                        0x0e000400,
                        0x0e000800,
                        0x0e001000,
                        0x0e002000,
                        0x0e004000,
                        0x0e008000,
                        0x0e010000,
                        0x0e020000,
                        0x0e040000,
                        0x0e080000,
                        0x0e100000,
                        0x0e200000,
                        0x0e400000,
                        0x8e000001,
                        0x8e000002,
                        0x8e000004,
                        0x8e000008,
                        0x8e000010,
                        0x8e000020,
                        0x8e000040,
                        0x8e000080,
                        0x8e000100,
                        0x8e000200,
                        0x8e000400,
                        0x8e000800,
                        0x8e001000,
                        0x8e002000,
                        0x8e004000,
                        0x8e008000,
                        0x8e010000,
                        0x8e020000,
                        0x8e040000,
                        0x8e080000,
                        0x8e100000,
                        0x8e200000,
                        0x0e400000,
                        0x00800000,
                        0x00800001,
                        0x00800002,
                        0x00800003,
                        0x7f7fffff,
                        0x7f7ffffe,
                        0x7f7ffffe,
                        0x7f7ffffe,
                        0x80800000,
                        0x80800001,
                        0x80800002,
                        0x80800003,
                        0xff7fffff,
                        0xff7ffffe,
                        0xff7ffffe,
                        0xff7ffffe,
                        0x0f7ffffc,
                        0x0f7ffffd,
                        0x0f7ffffe,
                        0x0f7fffff,
                        0x8f7ffffc,
                        0x8f7ffffd,
                        0x8f7ffffe,
                        0x8f7fffff,
                        0x00800000,
                        0x00800001,
                        0x00800002,
                        0x00800003,
                        0x80800000,
                        0x80800001,
                        0x80800002,
                        0x80800003,
                        0x0e000001,
                        0x0e000003,
                        0x0e000007,
                        0x0e00000f,
                        0x0e00001f,
                        0x0e00003f,
                        0x0e00007f,
                        0x0e0000ff,
                        0x0e0001ff,
                        0x0e0003ff,
                        0x0e0007ff,
                        0x0e000fff,
                        0x0e001fff,
                        0x0e003fff,
                        0x0e007fff,
                        0x0e00ffff,
                        0x0e01ffff,
                        0x0e03ffff,
                        0x0e07ffff,
                        0x0e0fffff,
                        0x0e1fffff,
                        0x0e3fffff,
                        0x0e7fffff,
                        0x0c400000,
                        0x0c600000,
                        0x0c700000,
                        0x0c780000,
                        0x0c7c0000,
                        0x0c7e0000,
                        0x0c7f0000,
                        0x0c7f8000,
                        0x0c7fc000,
                        0x0c7fe000,
                        0x0c7ff000,
                        0x0c7ff800,
                        0x0c7ffc00,
                        0x0c7ffe00,
                        0x0c7fff00,
                        0x0c7fff80,
                        0x0c7fffc0,
                        0x0c7fffe0,
                        0x0c7ffff0,
                        0x0c7ffff8,
                        0x0c7ffffc,
                        0x0c7ffffe,
                        0x0c7fffff,
                        0x0d000ff0,
                        0x0d00fff0,
                        0x7e800000,
                        0x7e800001,
                        0x7e8000CC,
                        0xfe800000,
                        0xfe800001,
                        0xfe8000AA,
                        0x0b800000,
                        0x0b800001,
                        0x0b8000CC,
                        0x8b800000,
                        0x8b800001,
                        0x8b8000CC,
                        0x3e800000,
                        0x3e800001,
                        0x3e800000,
                        0x3f7fffff,
                        0xde800000,
                        0xde800001,
                        0xde800000,
                        0xdf7fffff,
                        0x43000000,
                        0x43000001,
                        0x42ffffff,
                        0x42fc0000,
                        0x42fc0001,
                        0x42fbffff,
                        0xC3000000,
                        0xC3000001,
                        0xC2ffffff,
                        0xC2fc0000,
                        0xC2fc0001,
                        0xC2fbffff                         
               
	};


	C_TEST_START;

	__asm__ __volatile__ (
	"LBL_C_TEST_START:\n"
		"j LBL_SETUP_EV\n"
		".align 12\n"
		"M_MODE_E_H:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR\n"
	"LBL_SETUP_SCP:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
	"LBL_RET_TO_CLR:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
	"LBL_S_MODE_E_H:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
	"LBL_SETUP_EV:\n"
		"la x5, M_MODE_E_H\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"

		"mv x4, %[operands]\n"
		"flw.ps f0,  0 (x4)\n"
		"flw.ps f1,  32 (x4)\n"
		"flw.ps f2,  64 (x4)\n"
		"flw.ps f3,  96 (x4)\n"
		"flw.ps f4,  128 (x4)\n"
		"flw.ps f5,  160 (x4)\n"
		"flw.ps f6,  192 (x4)\n"
		"flw.ps f7,  224 (x4)\n"
		"flw.ps f8,  256 (x4)\n"
		"flw.ps f9,  288 (x4)\n"
		"flw.ps f10,  320 (x4)\n"
		"flw.ps f11,  352 (x4)\n"
		"flw.ps f12,  384 (x4)\n"
		"flw.ps f13,  416 (x4)\n"
		"flw.ps f14,  448 (x4)\n"
		"flw.ps f15,  480 (x4)\n"
		"flw.ps f16,  512 (x4)\n"
		"flw.ps f17,  544 (x4)\n"
		"flw.ps f18,  576 (x4)\n"
		"flw.ps f19,  608 (x4)\n"
		"flw.ps f20,  640 (x4)\n"
		"flw.ps f21,  672 (x4)\n"
		"flw.ps f22,  704 (x4)\n"
		"flw.ps f23,  736 (x4)\n"
		"flw.ps f24,  768 (x4)\n"
		"flw.ps f25,  800 (x4)\n"
		"flw.ps f26,  832 (x4)\n"
		"flw.ps f27,  864 (x4)\n"
		"flw.ps f28,  896 (x4)\n"
		"flw.ps f29,  928 (x4)\n"
		"flw.ps f30,  960 (x4)\n"
		"flw.ps f31,  992 (x4)\n"

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

	"LBL_VPU_SEQ_START_0:\n"
                 "fclass.PS f31, f0\n"
                 "fclass.PS f31, f1\n"
                 "fclass.PS f31, f2\n"
                 "fclass.PS f31, f3\n"
                 "fclass.PS f31, f4\n"
                 "fclass.PS f31, f5\n"
                 "fclass.PS f31, f6\n"
                 "fclass.PS f31, f7\n"
                 "fclass.PS f31, f8\n"
                 "fclass.PS f31, f9\n"
                 "fclass.PS f31, f10\n"
                 "fclass.PS f31, f11\n"
                 "fclass.PS f31, f12\n"
                 "fclass.PS f31, f13\n"
                 "fclass.PS f31, f14\n"
                 "fclass.PS f31, f15\n"
                 "fclass.PS f31, f16\n"
                 "fclass.PS f31, f17\n"
                 "fclass.PS f31, f18\n"
                 "fclass.PS f31, f19\n"
                 "fclass.PS f31, f20\n"
                 "fclass.PS f31, f21\n"
                 "fclass.PS f31, f22\n"
                 "fclass.PS f31, f23\n"
                 "fclass.PS f31, f24\n"
                 "fclass.PS f31, f25\n"
                 "fclass.PS f31, f26\n"
                 "fclass.PS f31, f27\n"
                 "fclass.PS f31, f28\n"
                 "fclass.PS f31, f29\n"
                 "fclass.PS f31, f30\n"


		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}

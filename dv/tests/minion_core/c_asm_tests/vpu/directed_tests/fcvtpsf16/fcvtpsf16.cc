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

	volatile uint32_t operands[120] __attribute__ ((aligned (4096))) = {
                        0x00000000,
                        0x00000001,
                        0x00000002,
                        0x000003fe,
                        0x000003ff,
                        0x00003c00,
                        0x00003c01,
                        0x00000400,
                        0x00000401,
                        0x000007ff,
                        0x00007800,
                        0x00007bff,
                        0x00007bfe,
                        0x00007c01,
                        0x00007dff,
                        0x00007e00,
                        0x00007e01,
                        0x00007fff,
                        0x00007c00,
                        0x00008000,
                        0x00008001,
                        0x00008002,
                        0x000083fe,
                        0x000083ff,
                        0x0000bc00,
                        0x0000bc01,
                        0x00008400,
                        0x00008401,
                        0x000087ff,
                        0x0000f800,
                        0x0000fbff,
                        0x0000fbfe,
                        0x0000fc01,
                        0x0000fdff,
                        0x0000fe00,
                        0x0000fe01,
                        0x0000ffff,
                        0x0000fc00,
                        0x0000AAAA,
                        0x00005555,
                        0x0000CCCC,
                        0x00003333,
                        0x00000001,
                        0x00000002,
                        0x00000004,
                        0x00000008,
                        0x00000010,
                        0x00000020,
                        0x00000040,
                        0x00000080,
                        0x00000100,
                        0x00000200,
                        0x00000400,
                        0x00000800,
                        0x00008001,
                        0x00008002,
                        0x00008004,
                        0x00008008,
                        0x00008010,
                        0x00008020,
                        0x00008040,
                        0x00008080,
                        0x00008100,
                        0x00008200,
                        0x00008400,
                        0x00008800,
                        0x00009000,
                        0x0000a000,
                        0x0000c000,
                        0x0000e000,
                        0x00005001,
                        0x00005002,
                        0x00005004,
                        0x00005008,
                        0x00005010,
                        0x00005020,
                        0x00005040,
                        0x00005080,
                        0x00005100,
                        0x00005200,
                        0x00005400,
                        0x00005800,
                        0x0000a001,
                        0x0000a002,
                        0x0000a004,
                        0x0000a008,
                        0x0000a010,
                        0x0000a020,
                        0x0000a040,
                        0x0000a080,
                        0x0000a100,
                        0x0000a200,
                        0x0000a400,
                        0x0000a800,
                        0x00003001,
                        0x00003003,
                        0x00003007,
                        0x0000300f,
                        0x0000301f,
                        0x0000303f,
                        0x0000307f,
                        0x000030ff,
                        0x000031ff,
                        0x000033ff,
                        0x000037ff,
                        0x00003fff,
                        0x00002400,
                        0x00002600,
                        0x00002700,
                        0x00002780,
                        0x000027c0,
                        0x000027e0,
                        0x000027f0,
                        0x000027f8,
                        0x000027fc,
                        0x000027fe,
                        0x000027ff,
                        0xffff27ff,
                        0xffff0000,
                        0xAAAACCCC

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
                 "FCVT.PS.F16 f31, f0\n"
                 "FCVT.PS.F16 f31, f1\n"
                 "FCVT.PS.F16 f31, f2\n"
                 "FCVT.PS.F16 f31, f3\n"
                 "FCVT.PS.F16 f31, f4\n"
                 "FCVT.PS.F16 f31, f5\n"
                 "FCVT.PS.F16 f31, f6\n"
                 "FCVT.PS.F16 f31, f7\n"
                 "FCVT.PS.F16 f31, f8\n"
                 "FCVT.PS.F16 f31, f9\n"
                 "FCVT.PS.F16 f31, f10\n"
                 "FCVT.PS.F16 f31, f11\n"
                 "FCVT.PS.F16 f31, f12\n"
                 "FCVT.PS.F16 f31, f13\n"
                 "FCVT.PS.F16 f31, f14\n"
                 "FCVT.PS.F16 f31, f15\n"
                 "FCVT.PS.F16 f31, f16\n"
                 "FCVT.PS.F16 f31, f17\n"
                 "FCVT.PS.F16 f31, f18\n"
                 "FCVT.PS.F16 f31, f19\n"
                 "FCVT.PS.F16 f31, f20\n"
                 "FCVT.PS.F16 f31, f21\n"
                 "FCVT.PS.F16 f31, f22\n"
                 "FCVT.PS.F16 f31, f23\n"
                 "FCVT.PS.F16 f31, f24\n"
                 "FCVT.PS.F16 f31, f25\n"
                 "FCVT.PS.F16 f31, f26\n"
                 "FCVT.PS.F16 f31, f27\n"
                 "FCVT.PS.F16 f31, f28\n"
                 "FCVT.PS.F16 f31, f29\n"
                 "FCVT.PS.F16 f31, f30\n"

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

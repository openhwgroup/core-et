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
	
	volatile uint32_t operands[184] __attribute__ ((aligned (4096))) = {
                        0xFFFFFFFF,
                        0xAAAAAAAA,
                        0x55555555,
                        0xCCCCCCCC,
                        0x10101010,
                        0x01010101,
                        0x90101010,
                        0x81010101,
                        0x00000000,
                        0x00000001,
                        0x00000002,
                        0x00000004,
                        0x00000008,
                        0x0000000a,
                        0x0000000c,
                        0x0000000e,
                        0x00000010,
                        0x00000010,
                        0x00000020,
                        0x00000040,
                        0x00000080,
                        0x000000c0,
                        0x000000e0,
                        0x00000100,
                        0x00000200,
                        0x00000400,
                        0x00000800,
                        0x00000a00,
                        0x00000c00,
                        0x00000e00,
                        0x00001000,
                        0x00002000,
                        0x00004000,
                        0x00008000,
                        0x0000a000,
                        0x0000c000,
                        0x0000e000,
                        0x00010000,
                        0x00020000,
                        0x00040000,
                        0x00080000,
                        0x000a0000,
                        0x000c0000,
                        0x000e0000,
                        0x00100000,
                        0x00200000,
                        0x00400000,
                        0x00800000,
                        0x00a00000,
                        0x00c00000,
                        0x00e00000,
                        0x01000000,
                        0x02000000,
                        0x04000000,
                        0x08000000,
                        0x0a000000,
                        0x0c000000,
                        0x0e000000,
                        0x10000000,
                        0x20000000,
                        0x40000000,
                        0x80000000,
                        0xc0000000,
                        0xe0000000,
                        0x00000003,
                        0x00000007,
                        0x0000000f,
                        0x0000001f,
                        0x0000003f,
                        0x0000007f,
                        0x000000ff,
                        0x000001ff,
                        0x000003ff,
                        0x000007ff,
                        0x00000fff,
                        0x00001fff,
                        0x00003fff,
                        0x00007fff,
                        0x0000ffff,
                        0x0001ffff,
                        0x0003ffff,
                        0x0007ffff,
                        0x000fffff,
                        0x001fffff,
                        0x003fffff,
                        0x007fffff,
                        0x00ffffff,
                        0x01ffffff,
                        0x03ffffff,
                        0x07ffffff,
                        0x0fffffff,
                        0x1fffffff,
                        0x3fffffff,
                        0x7fffffff,
                        0x80000000,
                        0x80000001,
                        0x80000002,
                        0x80000004,
                        0x80000008,
                        0x8000000a,
                        0x8000000c,
                        0x8000000e,
                        0x80000010,
                        0x80000010,
                        0x80000020,
                        0x80000040,
                        0x80000080,
                        0x800000c0,
                        0x800000e0,
                        0x80000100,
                        0x80000200,
                        0x80000400,
                        0x80000800,
                        0x80000a00,
                        0x80000c00,
                        0x80000e00,
                        0x80001000,
                        0x80002000,
                        0x80004000,
                        0x80008000,
                        0x8000a000,
                        0x8000c000,
                        0x8000e000,
                        0x80010000,
                        0x80020000,
                        0x80040000,
                        0x80080000,
                        0x800a0000,
                        0x800c0000,
                        0x800e0000,
                        0x80100000,
                        0x80200000,
                        0x80400000,
                        0x80800000,
                        0x80a00000,
                        0x80c00000,
                        0x80e00000,
                        0x81000000,
                        0x82000000,
                        0x84000000,
                        0x88000000,
                        0x8a000000,
                        0x8c000000,
                        0x8e000000,
                        0x80000000,
                        0x80000000,
                        0x80000000,
                        0x80000000,
                        0x80000000,
                        0x80000000,
                        0x80000003,
                        0x80000007,
                        0x8000000f,
                        0x8000001f,
                        0x8000003f,
                        0x8000007f,
                        0x800000ff,
                        0x800001ff,
                        0x800003ff,
                        0x800007ff,
                        0x80000fff,
                        0x80001fff,
                        0x80003fff,
                        0x80007fff,
                        0x8000ffff,
                        0x8001ffff,
                        0x8003ffff,
                        0x8007ffff,
                        0x800fffff,
                        0x801fffff,
                        0x803fffff,
                        0x807fffff,
                        0x80ffffff,
                        0x81ffffff,
                        0x83ffffff,
                        0x87ffffff,
                        0x8fffffff,
                        0x9fffffff,
                        0xbfffffff,
                        0x00000005,
                        0x00000007,
                        0x0000000d,
                        0x00000029,
                        0x00000093               
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
                 
                 "FCVT.PS.PW f31, f0, rne\n"
                 "FCVT.PS.PW f31, f1, rne\n"
                 "FCVT.PS.PW f31, f2, rne\n"
                 "FCVT.PS.PW f31, f3, rne\n"
                 "FCVT.PS.PW f31, f4, rne\n"
                 "FCVT.PS.PW f31, f5, rne\n"
                 "FCVT.PS.PW f31, f6, rne\n"
                 "FCVT.PS.PW f31, f7, rne\n"
                 "FCVT.PS.PW f31, f8, rne\n"
                 "FCVT.PS.PW f31, f9, rne\n"
                 "FCVT.PS.PW f31, f10, rne\n"
                 "FCVT.PS.PW f31, f11, rne\n"
                 "FCVT.PS.PW f31, f12, rne\n"
                 "FCVT.PS.PW f31, f13, rne\n"
                 "FCVT.PS.PW f31, f14, rne\n"
                 "FCVT.PS.PW f31, f15, rne\n"
                 "FCVT.PS.PW f31, f16, rne\n"
                 "FCVT.PS.PW f31, f17, rne\n"
                 "FCVT.PS.PW f31, f18, rne\n"
                 "FCVT.PS.PW f31, f19, rne\n"
                 "FCVT.PS.PW f31, f20, rne\n"
                 "FCVT.PS.PW f31, f21, rne\n"
                 "FCVT.PS.PW f31, f22, rne\n"
                 "FCVT.PS.PW f31, f23, rne\n"
                 "FCVT.PS.PW f31, f24, rne\n"
                 "FCVT.PS.PW f31, f25, rne\n"
                 "FCVT.PS.PW f31, f26, rne\n"
                 "FCVT.PS.PW f31, f27, rne\n"
                 "FCVT.PS.PW f31, f28, rne\n"
                 "FCVT.PS.PW f31, f29, rne\n"
                 "FCVT.PS.PW f31, f30, rne\n"

                 "FCVT.PS.PW f31, f0, rtz\n"
                 "FCVT.PS.PW f31, f1, rtz\n"
                 "FCVT.PS.PW f31, f2, rtz\n"
                 "FCVT.PS.PW f31, f3, rtz\n"
                 "FCVT.PS.PW f31, f4, rtz\n"
                 "FCVT.PS.PW f31, f5, rtz\n"
                 "FCVT.PS.PW f31, f6, rtz\n"
                 "FCVT.PS.PW f31, f7, rtz\n"
                 "FCVT.PS.PW f31, f8, rtz\n"
                 "FCVT.PS.PW f31, f9, rtz\n"
                 "FCVT.PS.PW f31, f10, rtz\n"
                 "FCVT.PS.PW f31, f11, rtz\n"
                 "FCVT.PS.PW f31, f12, rtz\n"
                 "FCVT.PS.PW f31, f13, rtz\n"
                 "FCVT.PS.PW f31, f14, rtz\n"
                 "FCVT.PS.PW f31, f15, rtz\n"
                 "FCVT.PS.PW f31, f16, rtz\n"
                 "FCVT.PS.PW f31, f17, rtz\n"
                 "FCVT.PS.PW f31, f18, rtz\n"
                 "FCVT.PS.PW f31, f19, rtz\n"
                 "FCVT.PS.PW f31, f20, rtz\n"
                 "FCVT.PS.PW f31, f21, rtz\n"
                 "FCVT.PS.PW f31, f22, rtz\n"
                 "FCVT.PS.PW f31, f23, rtz\n"
                 "FCVT.PS.PW f31, f24, rtz\n"
                 "FCVT.PS.PW f31, f25, rtz\n"
                 "FCVT.PS.PW f31, f26, rtz\n"
                 "FCVT.PS.PW f31, f27, rtz\n"
                 "FCVT.PS.PW f31, f28, rtz\n"
                 "FCVT.PS.PW f31, f29, rtz\n"
                 "FCVT.PS.PW f31, f30, rtz\n"

                 "FCVT.PS.PW f31, f0, rdn\n"
                 "FCVT.PS.PW f31, f1, rdn\n"
                 "FCVT.PS.PW f31, f2, rdn\n"
                 "FCVT.PS.PW f31, f3, rdn\n"
                 "FCVT.PS.PW f31, f4, rdn\n"
                 "FCVT.PS.PW f31, f5, rdn\n"
                 "FCVT.PS.PW f31, f6, rdn\n"
                 "FCVT.PS.PW f31, f7, rdn\n"
                 "FCVT.PS.PW f31, f8, rdn\n"
                 "FCVT.PS.PW f31, f9, rdn\n"
                 "FCVT.PS.PW f31, f10, rdn\n"
                 "FCVT.PS.PW f31, f11, rdn\n"
                 "FCVT.PS.PW f31, f12, rdn\n"
                 "FCVT.PS.PW f31, f13, rdn\n"
                 "FCVT.PS.PW f31, f14, rdn\n"
                 "FCVT.PS.PW f31, f15, rdn\n"
                 "FCVT.PS.PW f31, f16, rdn\n"
                 "FCVT.PS.PW f31, f17, rdn\n"
                 "FCVT.PS.PW f31, f18, rdn\n"
                 "FCVT.PS.PW f31, f19, rdn\n"
                 "FCVT.PS.PW f31, f20, rdn\n"
                 "FCVT.PS.PW f31, f21, rdn\n"
                 "FCVT.PS.PW f31, f22, rdn\n"
                 "FCVT.PS.PW f31, f23, rdn\n"
                 "FCVT.PS.PW f31, f24, rdn\n"
                 "FCVT.PS.PW f31, f25, rdn\n"
                 "FCVT.PS.PW f31, f26, rdn\n"
                 "FCVT.PS.PW f31, f27, rdn\n"
                 "FCVT.PS.PW f31, f28, rdn\n"
                 "FCVT.PS.PW f31, f29, rdn\n"
                 "FCVT.PS.PW f31, f30, rdn\n"

                 "FCVT.PS.PW f31, f0, rup\n"
                 "FCVT.PS.PW f31, f1, rup\n"
                 "FCVT.PS.PW f31, f2, rup\n"
                 "FCVT.PS.PW f31, f3, rup\n"
                 "FCVT.PS.PW f31, f4, rup\n"
                 "FCVT.PS.PW f31, f5, rup\n"
                 "FCVT.PS.PW f31, f6, rup\n"
                 "FCVT.PS.PW f31, f7, rup\n"
                 "FCVT.PS.PW f31, f8, rup\n"
                 "FCVT.PS.PW f31, f9, rup\n"
                 "FCVT.PS.PW f31, f10, rup\n"
                 "FCVT.PS.PW f31, f11, rup\n"
                 "FCVT.PS.PW f31, f12, rup\n"
                 "FCVT.PS.PW f31, f13, rup\n"
                 "FCVT.PS.PW f31, f14, rup\n"
                 "FCVT.PS.PW f31, f15, rup\n"
                 "FCVT.PS.PW f31, f16, rup\n"
                 "FCVT.PS.PW f31, f17, rup\n"
                 "FCVT.PS.PW f31, f18, rup\n"
                 "FCVT.PS.PW f31, f19, rup\n"
                 "FCVT.PS.PW f31, f20, rup\n"
                 "FCVT.PS.PW f31, f21, rup\n"
                 "FCVT.PS.PW f31, f22, rup\n"
                 "FCVT.PS.PW f31, f23, rup\n"
                 "FCVT.PS.PW f31, f24, rup\n"
                 "FCVT.PS.PW f31, f25, rup\n"
                 "FCVT.PS.PW f31, f26, rup\n"
                 "FCVT.PS.PW f31, f27, rup\n"
                 "FCVT.PS.PW f31, f28, rup\n"
                 "FCVT.PS.PW f31, f29, rup\n"
                 "FCVT.PS.PW f31, f30, rup\n"

                 "FCVT.PS.PW f31, f0, rmm\n"
                 "FCVT.PS.PW f31, f1, rmm\n"
                 "FCVT.PS.PW f31, f2, rmm\n"
                 "FCVT.PS.PW f31, f3, rmm\n"
                 "FCVT.PS.PW f31, f4, rmm\n"
                 "FCVT.PS.PW f31, f5, rmm\n"
                 "FCVT.PS.PW f31, f6, rmm\n"
                 "FCVT.PS.PW f31, f7, rmm\n"
                 "FCVT.PS.PW f31, f8, rmm\n"
                 "FCVT.PS.PW f31, f9, rmm\n"
                 "FCVT.PS.PW f31, f10, rmm\n"
                 "FCVT.PS.PW f31, f11, rmm\n"
                 "FCVT.PS.PW f31, f12, rmm\n"
                 "FCVT.PS.PW f31, f13, rmm\n"
                 "FCVT.PS.PW f31, f14, rmm\n"
                 "FCVT.PS.PW f31, f15, rmm\n"
                 "FCVT.PS.PW f31, f16, rmm\n"
                 "FCVT.PS.PW f31, f17, rmm\n"
                 "FCVT.PS.PW f31, f18, rmm\n"
                 "FCVT.PS.PW f31, f19, rmm\n"
                 "FCVT.PS.PW f31, f20, rmm\n"
                 "FCVT.PS.PW f31, f21, rmm\n"
                 "FCVT.PS.PW f31, f22, rmm\n"
                 "FCVT.PS.PW f31, f23, rmm\n"
                 "FCVT.PS.PW f31, f24, rmm\n"
                 "FCVT.PS.PW f31, f25, rmm\n"
                 "FCVT.PS.PW f31, f26, rmm\n"
                 "FCVT.PS.PW f31, f27, rmm\n"
                 "FCVT.PS.PW f31, f28, rmm\n"
                 "FCVT.PS.PW f31, f29, rmm\n"
                 "FCVT.PS.PW f31, f30, rmm\n"

                 "FCVT.PS.PWU f31, f0, rne\n"
                 "FCVT.PS.PWU f31, f1, rne\n"
                 "FCVT.PS.PWU f31, f2, rne\n"
                 "FCVT.PS.PWU f31, f3, rne\n"
                 "FCVT.PS.PWU f31, f4, rne\n"
                 "FCVT.PS.PWU f31, f5, rne\n"
                 "FCVT.PS.PWU f31, f6, rne\n"
                 "FCVT.PS.PWU f31, f7, rne\n"
                 "FCVT.PS.PWU f31, f8, rne\n"
                 "FCVT.PS.PWU f31, f9, rne\n"
                 "FCVT.PS.PWU f31, f10, rne\n"
                 "FCVT.PS.PWU f31, f11, rne\n"
                 "FCVT.PS.PWU f31, f12, rne\n"
                 "FCVT.PS.PWU f31, f13, rne\n"
                 "FCVT.PS.PWU f31, f14, rne\n"
                 "FCVT.PS.PWU f31, f15, rne\n"
                 "FCVT.PS.PWU f31, f16, rne\n"
                 "FCVT.PS.PWU f31, f17, rne\n"
                 "FCVT.PS.PWU f31, f18, rne\n"
                 "FCVT.PS.PWU f31, f19, rne\n"
                 "FCVT.PS.PWU f31, f20, rne\n"
                 "FCVT.PS.PWU f31, f21, rne\n"
                 "FCVT.PS.PWU f31, f22, rne\n"
                 "FCVT.PS.PWU f31, f23, rne\n"
                 "FCVT.PS.PWU f31, f24, rne\n"
                 "FCVT.PS.PWU f31, f25, rne\n"
                 "FCVT.PS.PWU f31, f26, rne\n"
                 "FCVT.PS.PWU f31, f27, rne\n"
                 "FCVT.PS.PWU f31, f28, rne\n"
                 "FCVT.PS.PWU f31, f29, rne\n"
                 "FCVT.PS.PWU f31, f30, rne\n"

                 "FCVT.PS.PWU f31, f0, rtz\n"
                 "FCVT.PS.PWU f31, f1, rtz\n"
                 "FCVT.PS.PWU f31, f2, rtz\n"
                 "FCVT.PS.PWU f31, f3, rtz\n"
                 "FCVT.PS.PWU f31, f4, rtz\n"
                 "FCVT.PS.PWU f31, f5, rtz\n"
                 "FCVT.PS.PWU f31, f6, rtz\n"
                 "FCVT.PS.PWU f31, f7, rtz\n"
                 "FCVT.PS.PWU f31, f8, rtz\n"
                 "FCVT.PS.PWU f31, f9, rtz\n"
                 "FCVT.PS.PWU f31, f10, rtz\n"
                 "FCVT.PS.PWU f31, f11, rtz\n"
                 "FCVT.PS.PWU f31, f12, rtz\n"
                 "FCVT.PS.PWU f31, f13, rtz\n"
                 "FCVT.PS.PWU f31, f14, rtz\n"
                 "FCVT.PS.PWU f31, f15, rtz\n"
                 "FCVT.PS.PWU f31, f16, rtz\n"
                 "FCVT.PS.PWU f31, f17, rtz\n"
                 "FCVT.PS.PWU f31, f18, rtz\n"
                 "FCVT.PS.PWU f31, f19, rtz\n"
                 "FCVT.PS.PWU f31, f20, rtz\n"
                 "FCVT.PS.PWU f31, f21, rtz\n"
                 "FCVT.PS.PWU f31, f22, rtz\n"
                 "FCVT.PS.PWU f31, f23, rtz\n"
                 "FCVT.PS.PWU f31, f24, rtz\n"
                 "FCVT.PS.PWU f31, f25, rtz\n"
                 "FCVT.PS.PWU f31, f26, rtz\n"
                 "FCVT.PS.PWU f31, f27, rtz\n"
                 "FCVT.PS.PWU f31, f28, rtz\n"
                 "FCVT.PS.PWU f31, f29, rtz\n"
                 "FCVT.PS.PWU f31, f30, rtz\n"

                 "FCVT.PS.PWU f31, f0, rdn\n"
                 "FCVT.PS.PWU f31, f1, rdn\n"
                 "FCVT.PS.PWU f31, f2, rdn\n"
                 "FCVT.PS.PWU f31, f3, rdn\n"
                 "FCVT.PS.PWU f31, f4, rdn\n"
                 "FCVT.PS.PWU f31, f5, rdn\n"
                 "FCVT.PS.PWU f31, f6, rdn\n"
                 "FCVT.PS.PWU f31, f7, rdn\n"
                 "FCVT.PS.PWU f31, f8, rdn\n"
                 "FCVT.PS.PWU f31, f9, rdn\n"
                 "FCVT.PS.PWU f31, f10, rdn\n"
                 "FCVT.PS.PWU f31, f11, rdn\n"
                 "FCVT.PS.PWU f31, f12, rdn\n"
                 "FCVT.PS.PWU f31, f13, rdn\n"
                 "FCVT.PS.PWU f31, f14, rdn\n"
                 "FCVT.PS.PWU f31, f15, rdn\n"
                 "FCVT.PS.PWU f31, f16, rdn\n"
                 "FCVT.PS.PWU f31, f17, rdn\n"
                 "FCVT.PS.PWU f31, f18, rdn\n"
                 "FCVT.PS.PWU f31, f19, rdn\n"
                 "FCVT.PS.PWU f31, f20, rdn\n"
                 "FCVT.PS.PWU f31, f21, rdn\n"
                 "FCVT.PS.PWU f31, f22, rdn\n"
                 "FCVT.PS.PWU f31, f23, rdn\n"
                 "FCVT.PS.PWU f31, f24, rdn\n"
                 "FCVT.PS.PWU f31, f25, rdn\n"
                 "FCVT.PS.PWU f31, f26, rdn\n"
                 "FCVT.PS.PWU f31, f27, rdn\n"
                 "FCVT.PS.PWU f31, f28, rdn\n"
                 "FCVT.PS.PWU f31, f29, rdn\n"
                 "FCVT.PS.PWU f31, f30, rdn\n"

                 "FCVT.PS.PWU f31, f0, rup\n"
                 "FCVT.PS.PWU f31, f1, rup\n"
                 "FCVT.PS.PWU f31, f2, rup\n"
                 "FCVT.PS.PWU f31, f3, rup\n"
                 "FCVT.PS.PWU f31, f4, rup\n"
                 "FCVT.PS.PWU f31, f5, rup\n"
                 "FCVT.PS.PWU f31, f6, rup\n"
                 "FCVT.PS.PWU f31, f7, rup\n"
                 "FCVT.PS.PWU f31, f8, rup\n"
                 "FCVT.PS.PWU f31, f9, rup\n"
                 "FCVT.PS.PWU f31, f10, rup\n"
                 "FCVT.PS.PWU f31, f11, rup\n"
                 "FCVT.PS.PWU f31, f12, rup\n"
                 "FCVT.PS.PWU f31, f13, rup\n"
                 "FCVT.PS.PWU f31, f14, rup\n"
                 "FCVT.PS.PWU f31, f15, rup\n"
                 "FCVT.PS.PWU f31, f16, rup\n"
                 "FCVT.PS.PWU f31, f17, rup\n"
                 "FCVT.PS.PWU f31, f18, rup\n"
                 "FCVT.PS.PWU f31, f19, rup\n"
                 "FCVT.PS.PWU f31, f20, rup\n"
                 "FCVT.PS.PWU f31, f21, rup\n"
                 "FCVT.PS.PWU f31, f22, rup\n"
                 "FCVT.PS.PWU f31, f23, rup\n"
                 "FCVT.PS.PWU f31, f24, rup\n"
                 "FCVT.PS.PWU f31, f25, rup\n"
                 "FCVT.PS.PWU f31, f26, rup\n"
                 "FCVT.PS.PWU f31, f27, rup\n"
                 "FCVT.PS.PWU f31, f28, rup\n"
                 "FCVT.PS.PWU f31, f29, rup\n"
                 "FCVT.PS.PWU f31, f30, rup\n"

                 "FCVT.PS.PWU f31, f0, rmm\n"
                 "FCVT.PS.PWU f31, f1, rmm\n"
                 "FCVT.PS.PWU f31, f2, rmm\n"
                 "FCVT.PS.PWU f31, f3, rmm\n"
                 "FCVT.PS.PWU f31, f4, rmm\n"
                 "FCVT.PS.PWU f31, f5, rmm\n"
                 "FCVT.PS.PWU f31, f6, rmm\n"
                 "FCVT.PS.PWU f31, f7, rmm\n"
                 "FCVT.PS.PWU f31, f8, rmm\n"
                 "FCVT.PS.PWU f31, f9, rmm\n"
                 "FCVT.PS.PWU f31, f10, rmm\n"
                 "FCVT.PS.PWU f31, f11, rmm\n"
                 "FCVT.PS.PWU f31, f12, rmm\n"
                 "FCVT.PS.PWU f31, f13, rmm\n"
                 "FCVT.PS.PWU f31, f14, rmm\n"
                 "FCVT.PS.PWU f31, f15, rmm\n"
                 "FCVT.PS.PWU f31, f16, rmm\n"
                 "FCVT.PS.PWU f31, f17, rmm\n"
                 "FCVT.PS.PWU f31, f18, rmm\n"
                 "FCVT.PS.PWU f31, f19, rmm\n"
                 "FCVT.PS.PWU f31, f20, rmm\n"
                 "FCVT.PS.PWU f31, f21, rmm\n"
                 "FCVT.PS.PWU f31, f22, rmm\n"
                 "FCVT.PS.PWU f31, f23, rmm\n"
                 "FCVT.PS.PWU f31, f24, rmm\n"
                 "FCVT.PS.PWU f31, f25, rmm\n"
                 "FCVT.PS.PWU f31, f26, rmm\n"
                 "FCVT.PS.PWU f31, f27, rmm\n"
                 "FCVT.PS.PWU f31, f28, rmm\n"
                 "FCVT.PS.PWU f31, f29, rmm\n"
                 "FCVT.PS.PWU f31, f30, rmm\n"


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

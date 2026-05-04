/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//////////////////////////////////////////////////////////////////////// 
////// VPU FOCUS TEST: seed = 0x9e1e34f3c4  = 679111619524
////// VPU FOCUS TEST: isa_focus = vpu_non_tensor
////// VPU FOCUS TEST: uarch_focus = uarch_default
////// VPU FOCUS TEST: cvg_choice = 0
//////////////////////////////////////////////////////////////////////// 
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


/// Macros

#define VSNIP_RSV           : : [operands] "r" (operands) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
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
	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 // vpu register f0 is designated 0 register
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,

			 // vpu register f1 is designated offset register for memory access ops
			 0x0000068c,
			 0x000000f0,
			 0x00000490,
			 0x000002e8,
			 0x000005e0,
			 0x00000528,
			 0x000001c0,
			 0x00000610,

			 /// vpu register f2
			 0x40400000,
			 0x41d80000,
			 0x42000000,
			 0x41900000,
			 0x40800000,
			 0x40c00000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f3
			 0x41a80000,
			 0x40e00000,
			 0x41d80000,
			 0x41c00000,
			 0x41b00000,
			 0x41100000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f4
			 0x41500000,
			 0x41d00000,
			 0x41500000,
			 0x41000000,
			 0x3f800000,
			 0x41c80000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f5
			 0xcf21ffe8,
			 0x41900000,
			 0x40400000,
			 0x41e80000,
			 0x41a00000,
			 0x41500000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f6
			 0x41e80000,
			 0x41300000,
			 0x41900000,
			 0x41400000,
			 0x40e00000,
			 0x41a80000,
			 0x41b80000,
			 0x40400000,

			 /// vpu register f7
			 0x41600000,
			 0x41c80000,
			 0x41700000,
			 0x41f00000,
			 0x40800000,
			 0x41d80000,
			 0x42000000,
			 0x40e00000,

			 /// vpu register f8
			 0x41c80000,
			 0x40c00000,
			 0x41400000,
			 0x41a80000,
			 0x41d80000,
			 0x41e00000,
			 0x41700000,
			 0x3f800000,

			 /// vpu register f9
			 0x41e80000,
			 0x41d00000,
			 0x41800000,
			 0x41a80000,
			 0x41c00000,
			 0x41b00000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f10
			 0x41f80000,
			 0x41300000,
			 0x40000000,
			 0x41100000,
			 0x41900000,
			 0x41400000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f11
			 0x41900000,
			 0x41700000,
			 0x41100000,
			 0x41980000,
			 0x41300000,
			 0x41600000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f12
			 0x41e00000,
			 0x41c00000,
			 0x42000000,
			 0x40400000,
			 0x41f00000,
			 0x41600000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f13
			 0x40e00000,
			 0x41400000,
			 0x41b00000,
			 0x41880000,
			 0x40800000,
			 0x40400000,
			 0x41880000,
			 0x41f00000,

			 /// vpu register f14
			 0x41400000,
			 0x40e00000,
			 0x41880000,
			 0x42000000,
			 0x41f80000,
			 0x41100000,
			 0x40e00000,
			 0x41f80000,

			 /// vpu register f15
			 0x41d80000,
			 0x41c00000,
			 0x41100000,
			 0x40800000,
			 0x41000000,
			 0x41e00000,
			 0x41c80000,
			 0x41000000,

			 /// vpu register f16
			 0x41f80000,
			 0x40000000,
			 0x40a00000,
			 0x41f00000,
			 0x41f80000,
			 0x40e00000,
			 0x41a80000,
			 0x41c80000,

			 /// vpu register f17
			 0x40e00000,
			 0x41800000,
			 0x40e00000,
			 0x41a80000,
			 0x40800000,
			 0x41700000,
			 0x40000000,
			 0x41b00000,

			 /// vpu register f18
			 0x41f80000,
			 0x41b80000,
			 0x41a80000,
			 0x41400000,
			 0x41200000,
			 0x41100000,
			 0x41f00000,
			 0x41300000,

			 /// vpu register f19
			 0x41700000,
			 0x41a00000,
			 0x41d00000,
			 0x41600000,
			 0x41800000,
			 0x41b00000,
			 0x41500000,
			 0x40a00000,

			 /// vpu register f20
			 0x41980000,
			 0x41d80000,
			 0x41900000,
			 0x41f00000,
			 0x41900000,
			 0x41f00000,
			 0x41e80000,
			 0x41f80000,

			 /// vpu register f21
			 0x41a00000,
			 0x41500000,
			 0x41880000,
			 0x41e00000,
			 0x40e00000,
			 0x41400000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f22
			 0x41b80000,
			 0x3f800000,
			 0x41500000,
			 0x41b00000,
			 0x41d80000,
			 0x41500000,
			 0x41f80000,
			 0x41e80000,

			 /// vpu register f23
			 0x41b80000,
			 0x41f80000,
			 0x41b00000,
			 0x41700000,
			 0x40a00000,
			 0x41e80000,
			 0x41400000,
			 0x42000000,

			 /// vpu register f24
			 0x41880000,
			 0x41a00000,
			 0x41900000,
			 0x40a00000,
			 0x41100000,
			 0x41c80000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f25
			 0x41c80000,
			 0x41600000,
			 0x42000000,
			 0x40000000,
			 0x41600000,
			 0x41d00000,
			 0x41c00000,
			 0x41100000,

			 /// vpu register f26
			 0xdecd0734,
			 0x3f800000,
			 0x41a80000,
			 0x41400000,
			 0x40e00000,
			 0x41a80000,
			 0x41f80000,
			 0x3f800000,

			 /// vpu register f27
			 0x41100000,
			 0x41c80000,
			 0x40c00000,
			 0x41700000,
			 0x41b80000,
			 0x41c00000,
			 0x3f800000,
			 0x41a00000,

			 /// vpu register f28
			 0x41500000,
			 0x42000000,
			 0x41200000,
			 0x41500000,
			 0x41f00000,
			 0x41f80000,
			 0x41d00000,
			 0x41600000,

			 /// vpu register f29
			 0x40000000,
			 0x41d00000,
			 0x41e00000,
			 0x40a00000,
			 0x41e00000,
			 0x41980000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f30
			 0x41e00000,
			 0x3f800000,
			 0x41200000,
			 0x41100000,
			 0x41e00000,
			 0x41100000,
			 0x41980000,
			 0x3f800000,

			 /// vpu register f31
			 0x41200000,
			 0x41800000,
			 0x41000000,
			 0x40800000,
			 0x41980000,
			 0x41f00000,
			 0x41a80000,
			 0x3f800000     /// 1.0                   
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
    
		"fnmadd.s f1, f5, f5, f26, rmm\n"                    ///  0,  1777 (0xcf21ffe8 (-2.7179e+09) * 0xcf21ffe8 (-2.7179e+09) + 0xdecd0734 (-7.38692e+18))
        

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

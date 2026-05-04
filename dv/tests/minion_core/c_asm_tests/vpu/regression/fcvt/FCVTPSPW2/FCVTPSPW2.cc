/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

	volatile uint32_t operands[240] __attribute__ ((aligned (4096))) = {
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0x80000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xc0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0xe0000000,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000003,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x00000007,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000000f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000001f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000003f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x0000007f,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000000ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000001ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000003ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x000007ff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00000fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00001fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x00007fff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0000ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0001ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0003ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x0007ffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x000fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x001fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x003fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x007fffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x00ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x01ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x03ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff,
			 0x07ffffff
};

C_TEST_START;
__asm__ __volatile__ (
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
                "flw.ps f0,0(%[operands])\n"
		"flw.ps f1,32(%[operands])\n"
		"flw.ps f2,64(%[operands])\n"
		"flw.ps f3,96(%[operands])\n"
		"flw.ps f4,128(%[operands])\n"
		"flw.ps f5,160(%[operands])\n"
		"flw.ps f6,192(%[operands])\n"
		"flw.ps f7,224(%[operands])\n"
		"flw.ps f8,256(%[operands])\n"
		"flw.ps f9,288(%[operands])\n"
		"flw.ps f10,320(%[operands])\n"
		"flw.ps f11,352(%[operands])\n"
		"flw.ps f12,384(%[operands])\n"
		"flw.ps f13,416(%[operands])\n"
		"flw.ps f14,448(%[operands])\n"
		"flw.ps f15,480(%[operands])\n"
		"flw.ps f16,512(%[operands])\n"
		"flw.ps f17,544(%[operands])\n"
		"flw.ps f18,576(%[operands])\n"
		"flw.ps f19,608(%[operands])\n"
		"flw.ps f20,640(%[operands])\n"
		"flw.ps f21,672(%[operands])\n"
		"flw.ps f22,704(%[operands])\n"
		"flw.ps f23,736(%[operands])\n"
		"flw.ps f24,768(%[operands])\n"
		"flw.ps f25,800(%[operands])\n"
		"flw.ps f26,832(%[operands])\n"
		"flw.ps f27,864(%[operands])\n"
		"flw.ps f28,896(%[operands])\n"
		"flw.ps f29,928(%[operands])\n"
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

  
		"wfi\n" 

	// Asm output results to C code
	:
	// Asm input data from C code
        :[operands] "r" (operands)
	// Registers used in Asm so C knows they change
	: "x5", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
        C_TEST_PASS;
	return 0;
}

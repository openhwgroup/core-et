/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

	volatile uint32_t operands[240] __attribute__ ((aligned (4096))) = {
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a100,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a200,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a400,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x0000a800,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003001,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003003,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x00003007,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000300f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000301f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000303f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x0000307f,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000030ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000031ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000033ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x000037ff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00003fff,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002400,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002600,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002700,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x00002780,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027c0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027e0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f0,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027f8,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fc,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027fe,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x000027ff,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000
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

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

	volatile uint32_t operands[240] __attribute__ ((aligned (4096))) = {
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7f8000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fc000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7fe000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff000,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ff800,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffc00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7ffe00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff00,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fff80,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffc0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7fffe0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff0,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffff8,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffc,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7ffffe,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0c7fffff,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d000ff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
			 0x0d00fff0,
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
	        "FCVT.F16.PS f31, f0\n"
		"FCVT.F16.PS f31, f1\n"
		"FCVT.F16.PS f31, f2\n"
		"FCVT.F16.PS f31, f3\n"
		"FCVT.F16.PS f31, f4\n"
		"FCVT.F16.PS f31, f5\n"
		"FCVT.F16.PS f31, f6\n"
		"FCVT.F16.PS f31, f7\n"
		"FCVT.F16.PS f31, f8\n"
		"FCVT.F16.PS f31, f9\n"
		"FCVT.F16.PS f31, f10\n"
		"FCVT.F16.PS f31, f11\n"
		"FCVT.F16.PS f31, f12\n"
		"FCVT.F16.PS f31, f13\n"
		"FCVT.F16.PS f31, f14\n"
		"FCVT.F16.PS f31, f15\n"
		"FCVT.F16.PS f31, f16\n"
		"FCVT.F16.PS f31, f17\n"
		"FCVT.F16.PS f31, f18\n"
		"FCVT.F16.PS f31, f19\n"
		"FCVT.F16.PS f31, f20\n"
		"FCVT.F16.PS f31, f21\n"
		"FCVT.F16.PS f31, f22\n"
		"FCVT.F16.PS f31, f23\n"
		"FCVT.F16.PS f31, f24\n"
		"FCVT.F16.PS f31, f25\n"
		"FCVT.F16.PS f31, f26\n"
		"FCVT.F16.PS f31, f27\n"
		"FCVT.F16.PS f31, f28\n"
		"FCVT.F16.PS f31, f29\n"


  
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

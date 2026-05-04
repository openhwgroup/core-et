/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

	C_TEST_START;

	__asm__ __volatile__ (
        "li x1, 0x1111111111111111\n"
        "li x3, 0x3333333333333333\n"
        "li x4, 0x4444444444444444\n"
		"li x5, 0x5555555555555555\n"
		"li x6, 0x6666666666666666\n"
        "fence\n"
		"mova.m.x x1\n"             //  masks = 1111111111111111
		"mova.x.m x3\n"             //     x3 = 1111111111111111
		"mova.x.m x4\n"             //     x4 = 1111111111111111
        //"nop\n"
		"mova.m.x x5\n"             //     masks = 5555555555555555
		"mova.m.x x6\n"             //     masks = 6666666666666666
        
	// Asm output results to C code
	:
	// Asm input data from C code
	: 
	// Registers used in Asm so C knows they change
	: "x1", "x3", "x4", "x5", "x6" 
	);
	C_TEST_PASS;
	return 0;
}

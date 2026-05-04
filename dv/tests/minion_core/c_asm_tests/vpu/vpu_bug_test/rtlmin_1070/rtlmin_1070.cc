/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {
        C_TEST_START;
	__asm__ __volatile__ (
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"fcmov.ps f2, f2, f0, f0\n"
		"fsgnjn.ps f3, f3, f1\n"
		"masknot m0, m5\n"    /// Causes test to fail
		//"maskand m0, m4, m5\n"  // replacing the prior instruction with this causes test to pass
		"feqm.ps m3, f2, f3\n"
	: : :  "f0", "f1", "f2", "f3"
	);
         C_TEST_PASS;
	return 0;
}

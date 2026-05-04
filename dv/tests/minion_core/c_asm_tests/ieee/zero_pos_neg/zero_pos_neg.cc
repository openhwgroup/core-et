/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

int main()
{
    // Init marker
    C_TEST_START;

    // Test
    __asm__ __volatile__ (
        // Enables all elements of FPU
        "mov.m.x m0, zero, 0xff\n"
        "fbci.ps f0, 0x00000\n"
        "fbci.ps f1, 0x3f800\n"
        "fbci.ps f2, 0x80000\n"
        // Adds
        "fadd.ps f3, f0, f0\n"
        "fadd.ps f3, f0, f2\n"
        "fadd.ps f3, f2, f0\n"
        "fadd.ps f3, f2, f2\n"
        // Subs
        "fsub.ps f3, f0, f0\n"
        "fsub.ps f3, f0, f2\n"
        "fsub.ps f3, f2, f0\n"
        "fsub.ps f3, f2, f2\n"
        // FMAs
        "fmadd.ps f3, f0, f0, f0\n"
        "fmadd.ps f3, f0, f0, f2\n"
        "fmadd.ps f3, f0, f2, f0\n"
        "fmadd.ps f3, f0, f2, f2\n"
        "fmadd.ps f3, f2, f0, f0\n"
        "fmadd.ps f3, f2, f0, f2\n"
        "fmadd.ps f3, f2, f2, f0\n"
        "fmadd.ps f3, f2, f2, f2\n"
        // FMSs
        "fmsub.ps f3, f0, f0, f0\n"
        "fmsub.ps f3, f0, f0, f2\n"
        "fmsub.ps f3, f0, f2, f0\n"
        "fmsub.ps f3, f0, f2, f2\n"
        "fmsub.ps f3, f2, f0, f0\n"
        "fmsub.ps f3, f2, f0, f2\n"
        "fmsub.ps f3, f2, f2, f0\n"
        "fmsub.ps f3, f2, f2, f2\n"
        // FNMAs
        "fnmadd.ps f3, f0, f0, f0\n"
        "fnmadd.ps f3, f0, f0, f2\n"
        "fnmadd.ps f3, f0, f2, f0\n"
        "fnmadd.ps f3, f0, f2, f2\n"
        "fnmadd.ps f3, f2, f0, f0\n"
        "fnmadd.ps f3, f2, f0, f2\n"
        "fnmadd.ps f3, f2, f2, f0\n"
        "fnmadd.ps f3, f2, f2, f2\n"
        // FNMSs
        "fnmsub.ps f3, f0, f0, f0\n"
        "fnmsub.ps f3, f0, f0, f2\n"
        "fnmsub.ps f3, f0, f2, f0\n"
        "fnmsub.ps f3, f0, f2, f2\n"
        "fnmsub.ps f3, f2, f0, f0\n"
        "fnmsub.ps f3, f2, f0, f2\n"
        "fnmsub.ps f3, f2, f2, f0\n"
        "fnmsub.ps f3, f2, f2, f2\n"
      : 
      : 
      : "f0", "f1", "f2", "f3"
    );

    // End marker
    C_TEST_PASS;
    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
    uint64_t data   = 0x80000000;
    uint64_t offset = 0x1000;

    // Test
    __asm__ __volatile__ (
        "csrr x31, mhartid\n"
        "bne  x31, zero, do_nothing\n"
        "addi x1, %[data], 0\n"
        "addi x3, zero, 512\n"
        "loop:\n"
        "ld   t3, 0x0(x1)\n"
        "add  x1, x1, %[offset]\n"
        "addi x3, x3, -1\n"
        "bne  x3, zero, loop\n"
        "do_nothing:\n"
      : 
      : [data] "r" (data),
        [offset] "r" (offset)
      : "x1", "t3", "x3"
    );

    C_TEST_PASS;
    return 0;
}

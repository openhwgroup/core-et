/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
   C_TEST_START

      // Test
      __asm__ __volatile__ (
            "addi a1, zero, 13\n"
            "addi a2, zero, 3\n"
            "addi a3, zero, 1\n"
            "loop:\n"
            "div  zero, a1, a2\n"
            "bne  a3, zero, loop\n"	    :
            : 
            : "a1", "a2", "a3"
            );


   C_TEST_PASS
   return 0;
}

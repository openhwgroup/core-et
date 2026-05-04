/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"

int main()
{
   volatile int __attribute__((aligned(4096))) ptr[1024];

   // Init marker
   C_TEST_START;

   for (int ii = 0; ii < 1024; ii += 16) {
      ptr[ii] = ii;
   }

   // Enable SCP
   __asm__ __volatile__ ( "csrsi 0x810, 1\n" : : );

   for (int ii = 0; ii < 1024; ii += 16) {
      ptr[ii] = ii;
   }

   // Disable SCP
   __asm__ __volatile__ ( "csrci 0x810, 1\n" : : );

   C_TEST_PASS;
   return *ptr;
}


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   // Can be done like this too

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }
   FENCE;
   // Evict way0, set4, numLines=16
   evict_sw(false,  0x1, 0x0, 0x4,   15       );
   WAIT_CACHEOPS;

   // Evict way1, set4, numLines=16
   evict_sw(false,  0x1, 0x1, 0x4,   15       );
   WAIT_CACHEOPS;

   // Evict way2, set4, numLines=16
   evict_sw(false,  0x1, 0x2, 0x4,   15       );
   WAIT_CACHEOPS;

   // Evict way3, set4, numLines=16
   evict_sw(false,  0x1, 0x3, 0x4,   15       );
   WAIT_CACHEOPS;



   /* Can be done like this

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i; 
   }
   FENCE;
   // Evict way0, set2, numLines=16
   evict_sw(false,  0x1, 0x0, 0x2,   15       );
   WAIT_CACHEOPS;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i+1; 
   }
   FENCE;
   // Evict way1, set5, numLines=16
   evict_sw(false,  0x1, 0x1, 0x5,   15       );
   WAIT_CACHEOPS;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i+2; 
   }
   FENCE;
   // Evict way2, set8, numLines=16
   evict_sw(false,  0x1, 0x2, 0x8,   15       );
   WAIT_CACHEOPS;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i+3; 
   }
   FENCE;
   // Evict way3, set11, numLines=16
   evict_sw(false,  0x1, 0x3, 0xb,   15       );
   WAIT_CACHEOPS;
   */

   C_TEST_PASS;
   return *ptr;
}

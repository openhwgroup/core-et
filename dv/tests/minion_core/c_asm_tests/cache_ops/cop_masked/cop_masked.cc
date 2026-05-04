/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }


   // Setting up tensor mask

   //          zeros mask_bits
   //          ----- ---------
   tensor_mask(0x0,  0x0      );
   FENCE;
   //   __asm__ __volatile__ (
   //         "add x31, zero, %[x31_enc]\n"
   //         "csrw 0x83f, %[csr_enc]\n"
   //         :
   //         : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
   //         : "x31"
   //   );


   // Lock L1 lines
   for (int i = 0; i < 4; i++)
   {
      //      use_tmask  addr                              num_lines  stride  id
      //      ---------  --------------------------------  ---------  ------  --
      lock_va(true,      (uint64_t)(void*)(ptr+(i*256)),   15,        64,     0 );
      WAIT_CACHEOPS;
   }


   // Lock L1 lines
   for (int i = 0; i < 4; i++)
   {
      //        use_tmask  addr                             num_lines  stride  id
      //        ---------  ------------------------------   ---------  ------  --
      unlock_va(true,      (uint64_t)(void*)(ptr+(i*256)),  15,        64,     0 );
      WAIT_CACHEOPS;
   }


   for(int i = 0; i < 4; i++)
   {
      // Evict L1 lines.
      //       use_tmask dst   addr     			num_lines stride id
      //       --------- ---   ------------------------------  --------- ------ --
      evict_va(true,     1,    (uint64_t)(void*)(ptr+(i*256)), 15,       64,    0 );
      WAIT_CACHEOPS;
   }

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }


   for(int i = 0; i < 4; i++)
   {
      // Flush L1 lines
      //       use_tmask dst addr                            num_lines stride id
      //       --------- --- -----------------------------   --------- ------ --
      flush_va(true,     1,  (uint64_t)(void*)(ptr+(i*256)), 15,       64,    0 );
      WAIT_CACHEOPS;
   }


   for(int i = 0; i < 4; i++)
   {
      // Prefetch L1 lines
      //           use_tmask dst addr                            num_lines stride id
      //           --------- --- -----------------------------   --------- ------ --
      prefetch_va(true,      0,  (uint64_t)(void*)(ptr+(i*256)), 15,       64,    0 );
      WAIT_PREFETCH_0;
   }

   // Initialize array
   //for (int i = 0; i < 1024; ++i) {
   //	  i = ptr[i];
   // }

   C_TEST_PASS;
}

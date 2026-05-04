/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "cacheops.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;


   if (get_hart_id() == 0) {

      // Evict cache
      for (unsigned way = 0; way < 4; way++) {
        evict_sw(false,     to_L2, way, 0x0,   15       );
      }
      WAIT_CACHEOPS;

      // LOCKVA
      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr),        15,        64,     0 );

      WAIT_CACHEOPS;

      // LOCKVA
      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(256)),  15,        64,     0 );

      WAIT_CACHEOPS;

      // LOCKVA
      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(512)),  15,        64,     0 );

      WAIT_CACHEOPS;

/*
    __asm__ __volatile__ (
        "csrr t1, 0x808\n"
        "andi t1, t1, 0x020\n"
      :
      :
      : "a7", "t1"
    );
*/


   C_TEST_PASS;
}
}

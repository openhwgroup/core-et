/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "esr.h"
#include "cacheops.h"
#include "et_test_common.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define NUM_ITER 10
#define BUF_SIZE (1 << 22) 
#define BUF_SLOT BUF_SIZE/256

uint64_t buf[BUF_SIZE] __attribute__((aligned(65536)));

inline __attribute__((always_inline)) void load(long addr,long value) {

    __asm__ __volatile__
        (
         "  ld %[value], 0(%[addr])\n"
         :[value] "+&r" (value)
         :[addr] "r" (addr)         
         :"memory"
        );
}

int main()
{
   uint64_t tid, hid, mid, ii;
   long data = 0;

   volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );

   // Get thread and shire ID
   tid = get_thread_id();
   hid = get_hart_id();   
   mid = get_minion_id();

   // Init marker
   C_TEST_START;

   if (tid == 0 && mid <= 7) {
     int idx = (hid>>1) << 3;

     for (ii = 0; ii < NUM_ITER; ++ii) {
      load((long(&buf[idx])), data);
      data++;
      idx+=0x10000;
      WAIT_CACHEOPS;
     }
   }
   C_TEST_PASS;
   return 0;
}


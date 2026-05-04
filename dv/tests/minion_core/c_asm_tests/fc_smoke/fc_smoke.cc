/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of basic traffic of different nature to gate the Full Chip
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define ESR_SHIRE_FEATURE 0x80340008

int main()
{
   minion_cache_line_t cache_lines[2048];

   int hid = get_hart_id();

   // Setup machine trap vector
   setup_mtrap_vector();

   // Init marker
   C_TEST_START;

   // simple store
   cache_lines[hid].w[0] = hid;
   FENCE;

   // evict up to memory
   evict_va(0, to_Mem, (uint64_t)(void*)&(cache_lines[hid].w[0]));
   WAIT_CACHEOPS;

   // simple UC load
   uint64_t shire_feature = *(uint64_t*)ESR_SHIRE_FEATURE;

   //
   // simple coherent store
   //
   asm volatile (
         "li t0, 0xCAFEBABE\n"
         "shg t0, (%[addr])\n"
         :
         : [addr] "r" (&(cache_lines[hid].d[4]))
         : "t0"
   );


   //
   // simple atomic
   //
   asm volatile (
         "li t0, 0xB16B00B5\n"
         "amoswapg.w x0, t0, (%[addr])\n"
         :
         : [addr] "r" (&(cache_lines[hid].d[7]))
         : "t0"
   );

   C_TEST_PASS;
   return shire_feature;
}


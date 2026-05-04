/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

bool isMemFault() {
  uint64_t terror;
  __asm__ __volatile__ ("csrr %[terror], tensor_error\n" : [terror] "=r" (terror));
  return (terror & 0x80);
}

void evict() {
   // Evict cache
   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 0, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 1, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 2, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 3, set,   0       );
   }
   WAIT_CACHEOPS;
}

int main()
{

   uint64_t tid;

   // VA_MAX_PREF = VA_MASK = 0xFFFFFFFFFFC0; //64B aligned
   uint64_t VA_ADDR_PREF    = 0x83FFFFFBC0ULL;  // Default DRAM size is 16GB as per RTLMIN-5227

   tid = get_thread_id();
   if (tid == 0) {
      // Setup machine trap vector
      setup_mtrap_vector();
      excl_mode(1);
/**
#ifdef L1D_SPLIT_MODE
      mcache_control(1,0,0);
#endif
#ifdef L1D_SCP_MODE
      mcache_control(1,0,0);
      mcache_control(1,1,0);
#endif
**/
      setup_l1dcache_split_or_scp();  // PRM-8; VERIF-3295
      excl_mode(0);
   } else {
      delay(30);
   }

   // Init marker
   C_TEST_START;

    // Initialize 1024B [0xFFFFFFFFFBC0 .. 0xFFFFFFFFFFC0)
   //for (uint32_t i = 0; i < 255; ++i) {
   //   *((uint32_t*)(VA_ADDR_PREF) + i) = i;
   //}

   evict();

   uint64_t use_tmask       = false;
   uint64_t dst_level       = et_get_rand_word(0,2);
   uint64_t stride          = 64;
   uint64_t overflow_stride = 128;
   uint64_t num_lines       = 15;
   uint64_t id              = 0;
   uint64_t warl            = 0;

   //Prefetch (no error)
   prefetch_va(use_tmask,     dst_level,     VA_ADDR_PREF,     num_lines,     stride,     id,     warl);

   // If destination is L1, fencing is done with a wait cacheops followed by a fence (L1 prefetches go to replayq)
   WAIT_CACHEOPS;
   FENCE;
   // If destination is L2, need to do a wait prefetch
   WAIT_PREFETCH_0;

   if (isMemFault() == true) {
       C_TEST_FAIL;
   }

   //Prefetch (tensor error 0x80; exception suppressed)
   prefetch_va(use_tmask,     dst_level,     VA_ADDR_PREF,     num_lines,     overflow_stride,     id,     warl);

   // If destination is L1, fencing is done with a wait cacheops followed by a fence (L1 prefetches go to replayq)
   WAIT_CACHEOPS;
   FENCE;
   // If destination is L2, need to do a wait prefetch
   WAIT_PREFETCH_0;

   if (isMemFault() == true) {
       C_TEST_PASS;
   } else {
       C_TEST_FAIL;
   }
}

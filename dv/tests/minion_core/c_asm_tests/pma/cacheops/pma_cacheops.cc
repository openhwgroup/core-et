/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does various cacheops to all Memory map regions
//               to ensure that the permissions (or lack thereof) are respected
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define MAXION_REGION   0x0000000
#define IO_REGION       0x10000000
#define MAILBOX_REGION  0x20000000
#define SP_REGION       0x40000000
#define SCP_REGION      0x80000000
#define RESERVED_REGION 0x200000000
#define PCIE_REGION     0x4000000000
#define MCODE_REGION    0x8000000000
#define OS_REGION       0x8000200000
#define DRAM_REGION     0x8100000000

#define NUM_REGIONS     11

int main()
{
   uint64_t tid;
   uint64_t regions[NUM_REGIONS] = {MAXION_REGION, IO_REGION,  MAILBOX_REGION,  SP_REGION,
                                   SCP_REGION,    ESR_REGION, RESERVED_REGION, PCIE_REGION,
                                   MCODE_REGION,  OS_REGION,  DRAM_REGION};

   tid = get_thread_id();

   // Setup machine trap vector
   setup_mtrap_vector();

   // Init marker
   C_TEST_START;

   __asm__ __volatile__ (
   "li a0, 0x00000901\n"
   "li a1, 0\n"
   "slli a1, a1, 22\n"

   "li a2, 0x80F1E030\n"
   "add a2, a2, a1\n"
   "sd a0, 0(a2)\n"
   "fence iorw, iorw\n" 
   );

   if (tid == 0)
   {
      for (unsigned ii = 0; ii < NUM_REGIONS; ++ii) {

         if (regions[ii] != SCP_REGION) { // avoid illegal cases for Shire Cache otherwise we get a bus error
            prefetch_va(false, to_L3, regions[ii]);
            WAIT_CACHEOPS;

            prefetch_va(false, to_L2, regions[ii]);
            WAIT_CACHEOPS;
         }

         prefetch_va(false, to_L1, regions[ii]);
         WAIT_CACHEOPS;

         lock_va(false, regions[ii]);
         WAIT_CACHEOPS;

         unlock_va(false, regions[ii]);
         WAIT_CACHEOPS;

         for (unsigned jj = to_L1; jj < to_Mem; ++jj) {
            if ((regions[ii] != SCP_REGION) || (ii < to_L2)) { // avoid illegal cases for Shire Cache otherwise we get a bus error
               evict_va(false, jj, regions[ii]);
               WAIT_CACHEOPS;

               flush_va(false, jj, regions[ii]);
               WAIT_CACHEOPS;
            }
         }

         lock_sw(0, regions[ii]);
         WAIT_CACHEOPS;

         unlock_sw(0, regions[ii]);
         WAIT_CACHEOPS;

      }
   }

   C_TEST_PASS;
   return 0;
}


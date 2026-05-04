/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <cstdint>
#include "macros.h"
#include "minion.h"
#include <stdio.h>
#include <stdlib.h>

#define USE_FENCE 0

#define SHIRE_CONFIG_ESR_ADDR ((unsigned long int*)(0x0340008)) // shire config ESR register

#define STORE_DATA    (uint64_t(0x33))

void inline __attribute__((always_inline)) myfence() {
#if USE_FENCE
   FENCE
#endif
}


int main()
{
   // uint64_t hid, mid, tid, sid;

   //C_TEST_START

   // Get minion ID and hart id
   /* hid = get_minion_id();
      tid = get_thread_id();
      mid = hid & 0x7F;
      sid = get_shire_id();


   // We only want thread 0
   if ( tid == 0) {
   // minion 0 on shire 0 generates load/store
   if ( hid == 0 ) {*/

   volatile uint64_t * esr_addr = SHIRE_CONFIG_ESR_ADDR;
   uint64_t  load_addr;
   // the data to be stored
   uint32_t store_data = STORE_DATA;

   // Store data in ESR address
   *esr_addr = store_data;
   // Load data
   load_addr = *esr_addr;


   myfence();
   //}
   //  }

   //C_TEST_PASS
   WFI

}

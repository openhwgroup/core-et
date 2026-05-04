/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "cacheops.h"


int main()
{
   int j = 0;
   volatile char __attribute__((aligned(4096))) ptr[4096];

   // Init marker
   C_TEST_START;

   // Only thread 0 executes this
   if (get_hart_id() == 0) {

      //Initial D$ filling: and Write/Store non-zero new value:
      for (int i = 0; i < 1024; i++) {
         ptr[i] = i+1;
      }

        // Each cache line is locked
        //      use_tmask  addr                              num_lines  stride  id
        //      ---------  ------------------------------    ---------  ------  --
        lock_va(false,     (uint64_t)(void*)(ptr),           15,        64,     0 );

        WAIT_CACHEOPS;

      // j is incremented if a line is not zeroed
      // (j is number of lines being non-zero)
      for (int i = 0; i < 1024; i++) {
        //if (ptr[i] == i+1) {    // Charles: better changed to code below:
        if (ptr[i] != 0) {
          j++;   
        }
        else { 
          continue;
        }
      }

   }

   // Self-checking
   // All lines should be zeroed
   if (j == 0) {
     // End marker
     C_TEST_PASS;
   }
   else {
     C_TEST_FAIL;
   } 

   return 0;
}

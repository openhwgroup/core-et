/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test exercise basic Fast Credit Counter functionality
//               Should be run on a full Minion Shire
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include "fcc.h"

#define NUM_ITER 16

volatile uint32_t check_var = 0;

int main()
{
   int hid = get_hart_id();

   // Setup machine trap vector
   setup_mtrap_vector();

   // Init marker
   C_TEST_START;

   for (uint32_t i = 0; i < NUM_ITER; ++i) {

      // Hart 0 is the master and will increment everyone's (including ourself) credit after atomically incrementing a shared variable
      if (hid == 0) {
         delay(et_get_rand_word(1, 10));
         asm volatile (
               "li t0, 1\n"
               "amoaddl.w x0, t0, (%[addr])\n"
               :
               : [addr] "r" (&check_var)
               : "t0"
         );
         FENCE;
         give_credit_local(-1, -1, 0);

         // wait for all other harts to be done consuming the shared counter, plus the credit we just sent to ourselves
         for (uint32_t j = 0; j < 63 + 1; ++j) {
            wait_for_credit(0);
         }

         // Make sure the credit counter is 0
         if (get_credit_count_nb(0) != 0) {
            C_TEST_FAIL;
         }

      // All other threads in the shire wait for a credit and try to read the shared counter.
      // If the value is not the incremented one, the credit wait was not successful so jump to fail.
      } else if (hid < 64) {
         uint32_t check = 0;
         delay(et_get_rand_word(1, 10));
         wait_for_credit(0);

         // Make sure the credit counter is 0
         if (get_credit_count_nb(0) != 0) {
            C_TEST_FAIL;
         }

         // Read the shared variable from local coherent memory
         asm volatile ( "amoorl.w %[check], x0, (%[addr])\n" : [check] "=r" (check) : [addr] "r" (&check_var));
         FENCE
         if (check != i+1) {
            C_TEST_FAIL;
         }
         give_credit_local(0, 0, 0);
      }
   }

   C_TEST_PASS;
   return 0;
}


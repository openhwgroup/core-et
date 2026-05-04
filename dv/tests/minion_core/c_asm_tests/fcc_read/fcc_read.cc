/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Very basic S0:M0:M0:T0 check of blocking, non-blocking read, and counter decrementing

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include "fcc.h"
#include "common.h"

#include "fcc_read.h"

#define DEBUG(x) __asm__ __volatile__ ("csrw validation3, %[data]" : : [data] "r" (x) );
int main()
{

  C_TEST_START;
  volatile uint64_t hart_id = getHartId();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);


  // see fcc_read.h for the iteration configuration
  // ( a different counter, different credit consumer and nr credits each time)
  for ( uint64_t iter = 0; iter < ITERATIONS; iter++) {
    volatile int counter = test_config[(iter + minion_id) % ITERATIONS].counter;
    FENCE
    volatile uint64_t nr_credits= test_config[(iter + minion_id) % ITERATIONS].nr_credits;
    FENCE
    ////////////////////////////////////////////////////////////////////////////////
    // thread sending the credits
    ////////////////////////////////////////////////////////////////////////////////
    if (thread_id == test_config[(iter + minion_id) % ITERATIONS].producer) {
      DEBUG(0 | (counter << 8) | (nr_credits<<16));
      for ( uint64_t i = 0; i < nr_credits; i++)
        give_credit(shire_id, minion_id, !thread_id, counter);
        FENCE
      // and credit to the next counter, to tell consumer it can start
      give_credit(shire_id, minion_id, !thread_id, (counter + 1) % NR_COUNTERS );
      FENCE
      DEBUG(1 | (counter << 8) | (nr_credits<<16));

      // and wait for the other thread to be done (credit in the same counter)
      wait_for_credit(counter);
      FENCE
    }

    ////////////////////////////////////////////////////////////////////////////////
    // thread receiving the credits
    ////////////////////////////////////////////////////////////////////////////////
    else {
      DEBUG(2 | (counter << 8) | (nr_credits<<16));
      // wait for all the credits to be sent
      wait_for_credit( (counter + 1) % NR_COUNTERS);
      FENCE
      DEBUG(3 | (counter << 8) | (nr_credits<<16));
      // and start consuming the credits
      volatile uint64_t rec;
      while ( nr_credits > 0) {
        rec = get_credit_count_nb(counter);
        FENCE
        if ( rec != nr_credits) {
          et_printf("FAIL. counter "); print_number(counter);
          et_printf(": rec "); print_number(rec);
          et_printf(" exp "); print_number(nr_credits);
          C_TEST_FAIL;
        }
        wait_for_credit(counter);
        FENCE
        nr_credits--;
        FENCE
      }
      rec = get_credit_count_nb(counter);
      FENCE
      if ( rec != 0 ){
        et_printf("FAIL. exp 0 credits in counter "); print_number(counter);
        et_printf("got "); print_number(rec);
        C_TEST_FAIL;
      }

      // and tell other thread we are done
      give_credit(shire_id, minion_id, !thread_id, counter);
      FENCE
    }
  }
  FENCE
  C_TEST_PASS;
  return 0;
}

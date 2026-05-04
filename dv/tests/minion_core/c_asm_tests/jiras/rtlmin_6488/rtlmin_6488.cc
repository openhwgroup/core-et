/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"

/* Test to reproduce RTLMIN-6488: Writes to PMU registers may happen twice
 * 
 * when accessing a different pmu counter, there is a stall and the csr
 * instruction is replayed. If there is a write, the write takes place
 * twice!
 *
 * To test this, the test will use two counters (counter3 configured to count
 * cycles and counter4) and execute the following sequence:
 * 
 *  counter3=0
 *  counter4=0
 *  nops (N times with N large)
 *  csrrwi cycles, counter3, 0 => read counter 3 and reset it
 *
 * The read/write will stall (because it is using a different pmu counter it used
 * in the previous pmu access => the erroneous double write will take place.
 * Since the read takes place after the first write, the read value will be incorrect
 * (it has to be at least N cycles because there are N nops)
 *
 */

static constexpr size_t ITERS = 30;
static constexpr size_t NR_NOPS = 256;

int main(){
  C_TEST_START;
  uint64_t cycles;
  __asm__("csrwi mhpmevent3, 1\n"); // configure pmu to count cycles

  for ( size_t i = 0; i < ITERS; i++) {
    __asm__ __volatile__
      (
       "csrwi mhpmcounter3, 0\n" // reset cnt3
       "csrwi mhpmcounter4, 0\n" // write cnt4
       ".rept %[NR_NOPS]\n"      // nops to allow the cycle counter to be incremented
       "    nop\n"
       ".endr\n"
       "csrrwi %[cycles], mhpmcounter3, 0\n" // read and reset cnt3
       "csrw validation3, %[cycles]\n" // just for debugging purposes
       : [cycles] "=r" (cycles)
       : [NR_NOPS] "i" (NR_NOPS)
       );
    
    // error if the counter is less than the NR_NOPS => double write took place
    if (cycles < NR_NOPS) {
      C_TEST_FAIL;
    }
  }
  C_TEST_PASS;
}
    

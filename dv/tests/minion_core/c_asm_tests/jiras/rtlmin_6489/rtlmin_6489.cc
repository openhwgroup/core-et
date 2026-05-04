/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"

/* Test to reproduce RTLMIN-6489: incorrect values read when incrementing
 * counter around the following condition: counter % 128 == 127
 *
 */

//#define ADD_PRINTS 
static constexpr size_t MAX_ERRORS = 10;
static constexpr size_t ITERS_PER_CASE = 4;

static constexpr size_t PMU_PRE_BITS = 7;
static constexpr size_t PMU_MAX_PRE = (1 << PMU_PRE_BITS) -1 ;


/* parameterized test class
   It uses the 'retired' counter for hart0: it initializes the counter to a known
   value, executes several 'nops' to increment the counter to produce overflow in the 
   7 LSB, and checks the expected counter value afterwards.

   Parameters are:
      PRE_START_VAL = counter init value[7:0]
      POS_START_VAL = counter init value[63:8]
      SURPASS = number of instructions to execute after overflowing the 7 LSB
                the final counter value will be: (init_value & ~7F) + 0x80 + SURPASS
 */

template<size_t PRE_START_VAL = PMU_MAX_PRE, size_t POST_START_VAL = 0, size_t SURPASS=0>
class pmu_pre_ovf_test {
public:
  static_assert(PRE_START_VAL <=PMU_MAX_PRE, "pre start val too big");
  static bool run(){
    constexpr uint64_t START_VAL = PRE_START_VAL | (POST_START_VAL << PMU_PRE_BITS);
    constexpr uint64_t INC = PMU_MAX_PRE + 1 - PRE_START_VAL + SURPASS;
    for (size_t i = 0 ; i < ITERS_PER_CASE; i++) {
      uint64_t retired;
      __asm__ __volatile__
        (
         ".p2align 6\n"
         "csrw mhpmcounter3, %[START_VAL]\n"
         ".rept %[INC]\n" // increment retired instructions
         "    nop\n"
         ".endr\n"
         "csrrwi %[retired], mhpmcounter3, 0\n" // read counter
         : [retired] "=r" (retired)
         : [START_VAL] "r" (START_VAL), [INC] "i" (INC)
         );
      
      constexpr uint64_t expected = START_VAL + INC;
      
      // show values in validation3, just for waveform debug
      asm("csrw validation3, %0" : : "r" (expected | (retired << 32)));
      if (retired != expected) {
#ifdef ADD_PRINTS
        et_printf_long("RTLMIN-648X: failing with START_VAL=0x%lx and INC=0x%lx: rec:%lx, exp:%lx\n",
                       START_VAL, INC, retired, expected);
#endif
        return false;
      }
    }
    return true;
  }
};



// make a table with all the combinations we want to test

static constexpr size_t MAX_INC = 20;
static constexpr size_t MAX_SURPASS = 10;

using func_t = decltype( pmu_pre_ovf_test<>::run)*;
constexpr auto cases = make_tests < pmu_pre_ovf_test, func_t,
                                    range<PMU_MAX_PRE, PMU_MAX_PRE - MAX_INC>, //PRE_START_VAL
                                    range<10,0, 3>, //POST_START_VAL
                                    range<MAX_SURPASS,0> //INC
                                    > :: sweep();


int main(){
  C_TEST_START;

  __asm__("csrwi mhpmevent3, 2\n" // configure to count retired instructions by t0
          "csrwi mhpmcounter3, 0");
   size_t err = 0;
  
  // run the test cases
  for (auto const  &t: cases) {
    if (!t() )  err++;
    if ( err > MAX_ERRORS) break;
  }

  // and check
  if (err == 0 ) {
    C_TEST_PASS;
  } else {
    C_TEST_FAIL;
  }
}
    

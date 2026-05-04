/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"

/* Test to reproduce RTLMIN-6487: prefetch CO should be completely finished after tensorwait + fence
 */

static constexpr size_t MAX_ERRORS = 100;  // stop the test after this number of errors
//#define ADD_PRINTS
#define INLINE  __attribute__ ((always_inline))
#define CL_ALIGN __attribute__ ((aligned(64)))

constexpr size_t BUFFER_CL = 16; // because CO uses at most 16 CL
CL_ALIGN  char buffer[BUFFER_CL*64];


// test parameters:
//   * CO_TO_WAIT: nops between prefetch and tensor wait
//   * WAIT_TO_FENCE: nops between tensor wait and prefetch
//   * nr_lines: number of lines to prefetch
//   * iterations: the loop will be repeated this many times
//
// The test will:
//     prefetch
//     nops
//     tensorwait
//     nops
//     fence
//     load => measure time of this load, that should finish "immediately"
//
// To ignore icache being warm, errors not taken into account in the first iteration
// note that very large values for nr nops might cause the test to fail if it doesn't fit into the ucache

template<size_t CO_TO_WAIT = 0, size_t WAIT_TO_FENCE = 0, size_t nr_lines = 1, size_t iterations = 2>
class prefetch_end_test {
public:

  static bool run(){
    const uint64_t csrconf_L1 = csrconf(0);
    const uint64_t csrconf_L3 = csrconf(2);
    for ( size_t i = 0 ; i < iterations; i++) {
      
      // first evict the whole buffer to L3
      __asm__ __volatile__
        (
         "csrw evict_va, %0\n"
         "csrwi tensor_wait, 6\n"
         : : "r" (csrconf_L3)
         );
      

      // and prefetch
      uint64_t cycles;
      __asm__ __volatile__
        (
         ".p2align 6\n"
         // the Prefetch
         "li x31, 64\n" //id=0, stride=1
         "csrw prefetch_va, %[csrconf]\n"
         
         // variable number of cycles between CO and tensor wait
         ".rept %[CO_TO_WAIT]\n"
         "  nop\n"
         ".endr\n"
         // tensor wait
         "csrwi tensor_wait, 6\n"
         
         // variable number of cycles before fencing
         ".rept %[WAIT_TO_FENCE]\n"
         "  nop\n"
         ".endr\n"
         
         // the fence
         "fence\n"
         
         // test load in the last prefetched CL  => should hit and start immediately
         "csrwi mhpmcounter3, 0\n" // reset cycle counter
         "lb %[cycles], %[ptr]\n" // load
         "addi %[cycles], %[cycles], 0\n" // wait for data
         "csrr %[cycles], mhpmcounter3\n" // read cycles
         
         : [cycles] "=&r" (cycles)
         : [csrconf] "r" (csrconf_L1), [ptr] "m" (buffer[(nr_lines-1)*64]),
           [CO_TO_WAIT] "i" (CO_TO_WAIT), [WAIT_TO_FENCE] "i" (WAIT_TO_FENCE)
         : "x31"
         );

      // just for waveform debugging, write interesting values in validation3
      __asm__ ("csrw validation3, %0" : : "r" ((cycles << 32) | test_id ));
      
      if (i > 0 && cycles > load_threshold) {
#ifdef ADD_PRINTS
        et_printf_long("RTLMIN-6487: failing with CO_TO_WAIT=%ld, WAIT_TO_FENCE=%ld, nr_lines=%ld\n",
                       CO_TO_WAIT, WAIT_TO_FENCE, nr_lines);
#endif
        return false;
      }
    }
    return true;
  }
  
private:
  static uint64_t csrconf(size_t dest){ //dest=> 0:L1, 1:L2, ...
    return  (dest << 58) | reinterpret_cast<uintptr_t>(buffer) | (nr_lines-1);
  }

  
  static constexpr uint64_t test_id = (CO_TO_WAIT) | (WAIT_TO_FENCE << 8) | (nr_lines << 16);
  static constexpr size_t load_threshold = 10;   // if cycles measurement for load more than this, error!
  
  static_assert(nr_lines <= BUFFER_CL && nr_lines > 0, "0 < nr lines <= buffer size");
  static_assert(iterations > 1, "iterations should be > 1");

  
};
  


// make a table with all the combinations we want to test
using func_t = decltype( prefetch_end_test<>::run)*;
constexpr auto cases = make_tests < prefetch_end_test, func_t,
                                    range<4,0>, //CO_TO_WAIT
                                    range<4,0>, // WAIT_TO_FENCE
                                    range<16,1>, //nr_lines
                                    range<5,5> // iterations
                                    > :: sweep();

int main(){
  C_TEST_START;
  __asm__("csrwi mhpmevent3, 1\n"); // configure pmu to count cycles
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
    

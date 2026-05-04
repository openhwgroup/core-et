/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"

#define TEST_WORKAROUND
#include "rtlmin_6469.h"
asm(".include \"rtlmin_6469.s\"\n"); // assembly test macros


/* Test to reproduce RTLMIN-6469: implicit x31 depency tracking for cacheops
 Note that the test is compiled with -ffixed-x31, so that x31 will not be
 allocated by the C++ code (changes within asm blocks will be preserved)
 */

static constexpr size_t NR_ITERS = 3;
static constexpr size_t MAX_STRIDE = 2;
static constexpr size_t MIN_LINES_IN_TEST = 2;
static constexpr size_t MAX_NOPS=7;
static constexpr size_t BUFFER_LINES = (MIN_LINES_IN_TEST + NR_ITERS) * (MAX_STRIDE +1);

CL_ALIGN uint8_t  buffer[BUFFER_LINES*64];

static_assert(BUFFER_LINES <= 16*3); // at  least 1 way per set free (or loading x31 from dcache will interfere the cacheops)

// cacheops_test
// templated class that checks amoswap instruction with the following snippet:
//      set_x31
//      nop (N times)
//      instruction under test
//      self-check
//
// The template parameters configure the snippet:
//     OP => one of the cacheop_t enum values
//     nr_nops => number of nops
//     x31_src => how is the x31 written. one value corresponding one of X31_XXX constants below
//     stride=> in number of lines (to configure x31)
//
// How do the checks work?
// * evict/flush
//     init: buffer line i is written with  (iter + i) in L2 and ~(iter +i) in L1
//     check: after the evict/flush, data is read with amo local from L2. If the line
//            had evicted/flushed with the stride configuration, it should be ~(iter+i),
//            otherwise (iter+i)
// * lock
//     init: buffer line i is written with ~(iter +i)
//     check: lines that had to be locked will read 0, and ~(iter+i) otherwise (+unlocks)
//  * prefetch:
//     init: the whole buffer is evicted to L3
//     check: measure cycles taken to load the line: time would be different depending on
//            whether the line was prefethed or not (0x9 cycles vs 31)
//

// Note: because sysemu doesn't know about caches, this test will only pass in the RTL
enum cacheop_t { EVICT = 0, FLUSH, PREFETCH, LOCK, NR_CACHEOPS};  // has to match the .equs in the .S

template<size_t OP = 0, size_t nr_nops = 0, size_t x31_src = 0, size_t stride = 1>
class cacheops_test {
public:
  static bool run() {
    __asm__ ("csrw validation3, %0" : : "r" (OP | (nr_nops<<8) | (x31_src<<16) | (stride << 24)));
    uint64_t x31, x31_bad;
    for ( size_t it = 0 ; it < NR_ITERS; it++ ) {
      // setup:
      x31 = stride << 6;      // value of x31 we want
      x31_bad = (stride+1) << 6;  // an incorrect value of  x31 that would make the test fail
      init(it);

      // run the test (calling the assembly macro)
      __asm__ __volatile__
        (
         "cacheops  %[op], %[csr] %[nr_nops], %[x31_src], %[x31], %[x31_ptr], %[x31_bad]\n"
         :
         : [op] "i" (OP), [csr] "r" (csr_conf(it)),
           [x31] "r" (x31), [x31_ptr] "r" (&x31), "m" (x31), [x31_bad] "r" (x31_bad),
           [nr_nops] "i" (nr_nops),
           [x31_src] "i" (x31_src)
         : "memory", "t0"
         );
      
      // check
      bool ok =  check(it);

      if constexpr ( OP == LOCK) unlock();
      if (! ok) {
        et_printf_long("RTLMIN-6469: case op=%ld nops=%ld x31_src=%ld stride=%ld fails at it %d\n",  OP, nr_nops, x31_src, stride, it);
        return false;
      }
    }

    return true;
  }
private:
  static void init(size_t it){
    constexpr bool writeL2 = OP == FLUSH || OP == EVICT;
    constexpr bool writeL1 = OP != PREFETCH;
    constexpr bool evictL3 = OP == PREFETCH;
    // write L2: with line[i] = it +i
    if constexpr(writeL2) { 
      for (size_t i = 0 ; i < BUFFER_LINES; i++) buffer[i*64] = (it + i);
      evict_va(buffer, BUFFER_LINES);
      }
    
    // write L1 contents with line[i] = ~(it +i)
    if constexpr(writeL1) {
        for (size_t i = 0 ; i < BUFFER_LINES; i++) buffer[i*64] = ~(it + i);
        __asm__ __volatile("fence");
      }

    // evict to L3
    if (evictL3) evict_va(buffer, BUFFER_LINES, 2);
  }
  

  static bool check(size_t it) {
    // expected memory contents in L2 => if line was evicted ~(it+i), else (it+1)
    size_t nr_lines = it + MIN_LINES_IN_TEST;
    for (size_t i = 0 ; i < BUFFER_LINES; i++) {
      bool changed = (i < nr_lines * stride) && (i % stride == 0);
      bool ok;
      if constexpr (OP == FLUSH || OP == EVICT) ok = check_EvictFlush(i, it, changed);
      else if constexpr( OP == LOCK) ok = check_Lock(i, it, changed);
      else ok = check_Prefetch(i, changed);
      if (!ok) return false;
    }
    return true;
  }

  INLINE static bool check_EvictFlush(size_t line, size_t it, bool changed){
    uintptr_t ptr = reinterpret_cast<uintptr_t> (buffer) + line*64;
    uint8_t exp = changed ? ~(it +line) : (it +line);
    uint8_t rec;
    __asm__ __volatile__ ("amoorl.w %0, x0, (%1)\n" : "=r" (rec) : "r" (ptr));
   
    return (exp == rec);
  }
  
  INLINE static bool check_Lock(size_t line, size_t it, bool changed){

    uint8_t exp = changed ? 0 : ~(it + line);
    return exp == buffer[line*64];
  }
  INLINE static bool check_Prefetch(size_t line, bool changed){
    constexpr uint64_t threshold = 15; // data in L1 must take less than this in the following check
    uint64_t cycles;
    __asm__ __volatile__
      (
       ".p2align 6\n" // align to CL so that icache misses don't affect the cycle counter
       "csrwi mhpmcounter3, 0\n" // reset cycle counter
       "lb %[cycles], %[ptr]\n" // load
       "addi %[cycles], %[cycles], 0\n" // wait for data
       "csrr %[cycles], mhpmcounter3\n" // read cycles
       : [cycles] "=&r" (cycles)
       : [ptr] "m" (buffer[line*64])
       );

    if (changed && cycles >= threshold) return false; // data expected to hit L1, but took too long
    else if (!changed && cycles < threshold) return false; // data expected to miss L1, but took too little
    else return true;
  }

  
  // returns CSR configuration
  INLINE static uint64_t csr_conf(size_t it) {
    size_t nr_lines = it + MIN_LINES_IN_TEST;
    uintptr_t ptr = reinterpret_cast<uintptr_t> (buffer);
    if constexpr (OP == FLUSH || OP == EVICT) {
        return (1ULL << 58) | ptr | (nr_lines -1); // dest=l2
      }
    else if constexpr (OP == LOCK) {
        return ptr | (nr_lines -1 );
      }
    else if constexpr (OP == PREFETCH) {
        return (0ULL << 58) | ptr | (nr_lines -1); //dest=l1
      }
    return 0;
  }
    

  static void unlock(){
    // unlocks any lines locked in the L1
    uint64_t conf = 0;
    for ( size_t way = 0 ;  way < 4; way ++, conf+= (1ULL<<55))
      for (size_t set = 0; set < 16; set++, conf+= 1<<6)
        __asm__ __volatile__ ("csrw unlock_sw, %0" : : "r" (conf));
    __asm__ __volatile("csrwi tensor_wait, 6\nfence");
  }
  
  INLINE static void evict_va(uint8_t *ptr, size_t nr_lines,  size_t dest = 1, bool wait = true){
    __asm__("li x31, 64");
    uintptr_t va = reinterpret_cast<uintptr_t> (ptr);
    while(nr_lines > 0 ) {
      size_t count = std::min(size_t(16), nr_lines);
      uint64_t conf = (dest << 58) | va | (nr_lines-1);
      __asm__ __volatile__ ("csrw evict_va, %0" : : "r" (conf));
      nr_lines-=count;
    }
    if (wait) {
      __asm__ __volatile("csrwi tensor_wait, 6\nfence");
    }
  }


  
  static_assert( x31_src < X31_NR_SRCS, "bad x31 src");
  static_assert( stride>0 && stride <= MAX_STRIDE, "bad stride");
  static_assert(OP < size_t(NR_CACHEOPS), "bad op");
};


// make a table with all the combinations we want to test
using func_t = decltype( cacheops_test<>::run)*;
constexpr auto cases = make_tests < cacheops_test, func_t,
#if 1
                                    range<NR_CACHEOPS-1, 0>,   // cache operation
                                    range<MAX_NOPS, 0>,        // number of nops
                                    range<X31_NR_SRCS -1, 0>,  // where the x31 is loaded from
                                    range<MAX_STRIDE, 1>       // stride
#else
                                    range<PREFETCH, PREFETCH>, // cache operation
                                    range<1, 1>,             // number of nops
                                    range<X31_ALU, X31_ALU>, // where the x31 is loaded from
                                    range<2, 2>              // stride
#endif
                                    > :: sweep();

int main(){
  C_TEST_START;
  __asm__("csrwi mhpmevent3, 1\n"); // configure pmu to count cycles
    size_t err = 0;
  
  // run the test cases
  for (auto const  &t: cases) {
    if (!t() )  err++;
  }

  // and check
  if (err == 0 ) {
    C_TEST_PASS;
  } else {
    C_TEST_FAIL;
  }
}
    

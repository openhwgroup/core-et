/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"

#define TEST_WORKAROUND
#include "rtlmin_6469.h"
asm(".include \"rtlmin_6469.s\"\n"); // assembly test macros

/* Test to reproduce RTLMIN-6469: implicit x31 depency tracking for amocmpswap
 Note that the test is compiled with -ffixed-x31, so that x31 will not be
 allocated by the C++ code (changes within asm blocks will be preserved)
 */

CL_ALIGN uint64_t scratch; // memory test value

// amcmpswap_test
// templated class that checks amoswap instruction with the following snippet:
//      set_x31
//      nop (N times)
//      amocmpswap
//      self-check
//
// The template parameters configure the snippet:
//     l_g => 0 for local, 1 for global
//     w_d => 0 for word, 1 for double word
//     nr_nops => number of nops
//     do_swap => if 1, tests cases where x31 == mem[rs1]
//     x31_src => how is the x31 written. one value corresponding one of X31_XXX constants below




static constexpr size_t NR_ITERS = 10;

template<size_t l_g = 0, size_t w_d = 0, size_t nr_nops = 0, size_t do_swap = 0, size_t x31_src = 0>
class amocmpswap_test {
public:
  using reg_t = typename std::conditional<w_d == 0, uint32_t, uint64_t>::type;
  static bool run() {
    asm ("csrw validation3, %0":: "r" (l_g | (w_d << 1) | (do_swap << 2) | (x31_src<<8) | (nr_nops << 16))); //just to locate tests easily in the waveform
    reg_t mem, src, dst, x31, x31_bad;
    uintptr_t ptr = reinterpret_cast<uintptr_t> (&scratch);
    for ( size_t it = 0 ; it < NR_ITERS; it++ ) {
      // setup:
      src = it ^ 0xAAAAAAAA;
      mem = it ^ 0x55555555;
      x31 = do_swap ? mem : ~mem;
      x31_bad = ~x31;
      write_mem(ptr, mem);
      
      __asm__ __volatile__ 
        (
         "amocmpswap %[dst], %[src], %[ptr], %[l_g], %[w_d], %[nr_nops], %[x31_src], %[x31], %[x31_ptr], %[x31_bad]\n"
         : [dst] "=r" (dst)
         : [src] "r" (src), [ptr] "r" (ptr), [x31] "r" (x31), [x31_ptr] "r" (&x31), "m" (x31), [x31_bad] "r" (x31_bad),
           [l_g] "i" (l_g),
           [w_d] "i" (w_d),
           [nr_nops] "i" (nr_nops),
           [x31_src] "i" (x31_src)
         : "memory", "t0"
         );
      
      // check
      reg_t mem_out = read_mem(ptr);
      bool ok = (dst == mem) && ( (do_swap && mem_out == src) || (!do_swap && mem_out == mem));
      if (! ok) {
        et_printf_long("RTLMIN-6469: case l_g=%ld w_d=%ld nops=%ld cmp=%ld x31_src=%ld fails at it %d\n", l_g, w_d, nr_nops, do_swap, x31_src, it);
        return false;
      }
    }

    //    et_printf_long("RTLMIN-6469: case %ld %ld %ld %ld %ld passes\n", l_g, w_d, nr_nops, do_swap, x31_src);
    return true;
  }
private:
  
  INLINE static uint64_t read_mem(uintptr_t ptr){
    uint64_t d;
    if constexpr (l_g)
      __asm__ __volatile__ ("amoorg.d %0, x0, (%1)\n" : "=r" (d) : "r" (ptr));
    if constexpr (!l_g)
      __asm__ __volatile__ ("amoorl.d %0, x0, (%1)\n" : "=r" (d) : "r" (ptr));
    return d;
  }


  INLINE static void write_mem(uintptr_t ptr, uint64_t d){
    if constexpr (l_g)
      __asm__ __volatile__ ("amoswapg.d x0, %0, (%1)\n" : : "r" (d), "r" (ptr));
    if constexpr (!l_g )
      __asm__ __volatile__ ("amoswapl.d x0, %0, (%1)\n" : : "r" (d), "r" (ptr));
    
    __asm__ __volatile__("fence");
  }

  static_assert( x31_src < X31_NR_SRCS, "bad x31 src");
 
};


// make a table with all the combinations we want to test
using func_t = decltype( amocmpswap_test<>::run)*;
constexpr auto cases = make_tests < amocmpswap_test, func_t,
#if 1
                                    range<1,0>,               // local/global
                                    range<1,0>,               // word/double word
                                    range<8,0>,              // number of nops
                                    range<1,0>,               // do swap
                                    range<X31_NR_SRCS -1, 0>  // where the x31 is loaded from
#else
                                    range<0,0>,               // local
                                    range<1,1>,               // double word
                                    range<2,2>,              // number of nops
                                    range<1,1>,               // do swap
                                    range<X31_NR_SRCS -1, 0>  // where the x31 is loaded from
#endif
                                    > :: sweep();
int main(){
  C_TEST_START;
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
    

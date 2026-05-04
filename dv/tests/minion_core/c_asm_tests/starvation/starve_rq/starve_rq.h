/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _RQ_STARVATION_H_
#define _RQ_STARVATION_H_

#include "starvationTest.h"


  static constexpr std::array<uint8_t, 512> scratch_ = {0};


template<size_t repeat,
         size_t NR_HITS, size_t NR_NOPS_T0,
         size_t NR_ESRS, size_t NR_NOPS_T1>

class rq_starvation_test :
  public starvation_test_base < repeat,
                                rq_starvation_test<repeat, NR_HITS, NR_NOPS_T0, NR_ESRS, NR_NOPS_T1> > {
public:
  
  static constexpr uint64_t min_iter_first_loop(){ return 0; }
  static constexpr uint64_t min_iter_second_loop(){ return 0; }
  
  
  static void ALWAYS_INLINE first_thread_iter() {
    // NR_HITS load that hit
    // NR_NOPS_T0 nops
    __asm__ __volatile__
      (
       ".rept %0\n"
       "    ld x0, 0(%2)\n"
       ".endr\n"
       ".rept %1\n"
       "    nop\n"
       ".endr\n"
       : : "i" (NR_HITS), "i" (NR_NOPS_T0), "r" (scratch_.data())
       );
    
  }

  static void ALWAYS_INLINE second_thread_iter() {
    volatile uint64_t *flb0 = reinterpret_cast<volatile uint64_t*> (0x80340100);
    // NR_ESRS loads to an ESR
    // NR_NOPS_T1 nops
  __asm__ __volatile__
      (
       ".rept %0\n"
       "    ld x0, %2\n"
       ".endr\n"
       ".rept %1\n"
       "    nop\n"
       ".endr\n"
       : : "i" (NR_ESRS), "i" (NR_NOPS_T1), "m" (*flb0)
       );
  }

  
  // prefetch scratch data
  static void setup() {
    asm ("ld x0, 0(%0)" : : "r" (scratch_.data()));
  }
  
  
};


#endif

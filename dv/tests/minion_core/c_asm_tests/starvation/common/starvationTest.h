/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _STARVATION_TEST_H_
#define _STARVATION_TEST_H_

#include "starvation_defines.h"


////////////////////////////////////////////////////////////////////////////////
// Base class for starvation tests
// virtual functions to be defined:
//   void setup() => to be executed before the test (e.g. setup offsets for gsc)
//   first_thread_iter()  => what one of the threads will do in a loop
//   second_thread_iter() => what tje other thread will do in a loop  
////////////////////////////////////////////////////////////////////////////////


template<size_t count>
static void ALWAYS_INLINE nops() {
  __asm__ __volatile__ 
    (
     ".rept %0\n"
     "nop\n"
     ".endr\n"
     : : "i" (count) );
}  


template<size_t unroll_loop_count, class T>
class starvation_test_base {
public:
  
  static void run(unsigned id)  {
    if (id) {
      uint64_t cnt = testWrapper(first_thread_loop);
      // check it did a minimum number of iterations
      if ( cnt < T::min_iter_first_loop())
        C_TEST_FAIL;
    }
    else {
      uint64_t cnt = testWrapper(second_thread_loop);
      // check it did a minimum number of iterations
      if ( cnt < T::min_iter_second_loop())
        C_TEST_FAIL;
    }
  }
  
  
private:
  starvation_test_base() = default; // private so that it is not instanced
  
  static void setup() { T::setup(); }
  static void ALWAYS_INLINE first_thread_iter() { T::first_thread_iter();}
  static void ALWAYS_INLINE second_thread_iter(){ T::second_thread_iter();}
  
  static uint64_t ALWAYS_INLINE testWrapper(iter_func_t f) {
    volatile uint64_t* cred_inc_control = (volatile uint64_t*) CREDIT_INC_AD_T0;
    uint64_t control_hart = get_hart_id() & ~(THREADS_PER_NEIGH-1);
    uint64_t credit_mask = 1ull << (control_hart/2);
    setup();
    
    // sync with control thread
    *cred_inc_control = credit_mask;

    // and start infinite loop
    // uses x31 to keep track of the counter of iterations
    uint64_t cnt;
    __asm__ __volatile__
      (
       TEST_START
       "j %[dest]\n"
       TEST_END
       : [cnt] "=r" (cnt)
       : [dest] "i" (f)
       : TEST_REG_MODS
       );
    return cnt;   
  }
  
  
  static void NAKED ALIGNED NO_INLINE first_thread_loop() {
    while(true){
      iter_wrapper<unroll_loop_count, first_thread_iter>::fnc();
    }
  }
  
  static void NAKED ALIGNED NO_INLINE second_thread_loop(){
    while(true){
      iter_wrapper<unroll_loop_count, second_thread_iter>::fnc ();
    }
  }
  
  
  template <size_t unroll, iter_func_t f>
  struct iter_wrapper {
    static ALWAYS_INLINE void fnc() {
      static_assert(unroll != 0, "unroll==0.. cannot end recursion");
      f();
      iter_wrapper<unroll-1,f>::fnc ();
    }
  };
  
  template <iter_func_t f>
  struct iter_wrapper<1,f> { 
    static ALWAYS_INLINE  void fnc(){ f(); }
  };
  
};
#endif

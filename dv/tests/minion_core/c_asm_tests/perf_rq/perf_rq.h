/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _RQ_TEST_H_
#define _RQ_TEST_H_
#include <inttypes.h>
#include <utility>
#include <tuple>

#include "fcc.h"
#include "shire.h"
#include "et_test_common.h"
extern char __scratch[];

#define INLINE inline __attribute__((always_inline))
#define NOINLINE inline __attribute__((noinline))

namespace RQTest{
  
  ////////////////////////////////////////////////////////////////////////////////
  // test class. It will:
  //  - call thread0() in a loop of t0_iterations if it's th 0
  //  - call thread1() in a loop if it's th 1, ending the loop when t0 is done
  // It measures the time it takes for t0 to complete the loop, and prints it out
  ////////////////////////////////////////////////////////////////////////////////

  template<class thread0, class thread1, unsigned t0_iterations = 25>
  class Test  {
  public:
    static NOINLINE void run() {
      auto thread = get_thread_id();
      auto minion = get_minion_id();

      // code prefetch
      if (thread==0){
        volatile uint64_t *esr = reinterpret_cast<volatile uint64_t*> ( 0x803402F8);
        uint64_t conf;
        __asm__ __volatile__ ("1: la %0, 1b" : "=r" (conf));
        conf&=~0x3ful;
        conf |= (1 << (get_neigh_id() + 48)) | 31; // 32 lines
        *esr = conf;
      }
      
      // inc validation3, just for waveforms
      uint64_t tmp;
      __asm__ __volatile__
        ("csrr %0, validation3\n"
         "addi %0, %0, 1\n"
         "csrw validation3, %0\n"
         : "=&r" (tmp)
         );
      
      if (thread) {
        thread1 t;
        // wait for t0 to be ready to start
        wait_for_credit(0);
        
        // tell thread0 to start by sending credit
        give_credit_local(minion, 0, 0);
        
        // run until thread0 is done (until credit is received)
        asm(".balign 32");
        for ( bool done = false; !done; done = (get_credit_count_nb(0) > 0))
          t();
        
        // consume the credit
        wait_for_credit(0);
      }
      else{
        thread0 t;
        // tell thread1 it's ready to start
        give_credit_local(minion, 1, 0);
        
        // wait for t1 to send credit
        wait_for_credit(0);
        
        // run
        auto start = et_get_cycle_count();
        asm(".balign 32"); // align to 32B boundary...
        for(unsigned i = 0 ; i < t0_iterations; i++) 
          t();
        auto end = et_get_cycle_count();
        
        // tell thread1  to stop
        give_credit_local(minion, 1, 0);
        et_printf_long("report time %s vs %s: %d\n", typeid(thread0).name(), typeid(thread1).name(), end - start);
      }
    }
  };
  ////////////////////////////////////////////////////////////////////////////////
  // test runner class
  // it simply calls the static run() method of all the classes in the template
  // parameters
  ////////////////////////////////////////////////////////////////////////////////


template<class ...T>
class TestRunner {
public:
  using types = std::tuple<T...>;
  static constexpr size_t nrTypes = sizeof...(T);

  template<size_t idx = 0>
  static void run(){
    using T1 =  std::tuple_element_t<idx, types>;
    outer_loop<T1>(std::make_index_sequence<nrTypes>{});
    if constexpr (idx != nrTypes -1) run<idx+1>();
  }
  
  template<class T1, size_t ...idx>
  static void outer_loop(std::index_sequence<idx...>){
    int dummy[] = {inner_loop<T1, idx>()...};
    (void)(dummy);
  }
  template<class T1, size_t idx2>
  static int inner_loop(){
    using T2 =  std::tuple_element_t<idx2, types>;
    Test<T1, T2>::run();
    return 0;
  }

};

  
  ////////////////////////////////////////////////////////////////////////////////
  // test cases.. intented to be used as template params for Test
  ////////////////////////////////////////////////////////////////////////////////
 class Idle {
  public:
   INLINE void operator()(){ }
 };
 
  class LoadHit {
  public:
    LoadHit(){
      // prefetch the line it will load from + fence
      __asm__ __volatile__
        ( "ld x0, %0\n"
          "fence\n"
          "ld x0, %0\n"
          : :"m" (__scratch)
          );
    }

   // 32 loads to the same address
    INLINE void operator()(){
      __asm__ __volatile__
        ( ".rept 32\n"
          "ld x0, %0\n"
          ".endr\n"
          : :"m" (__scratch)
          );
    }
  };

  class LoadMiss {
  public:

   // 64 loads, address increments each time by 64
   // all sets used => when starting the next iter, it will miss
    INLINE void operator()(){
      __asm__ __volatile__
        (
         ".macro  __LOAD_MISS%= start=0\n"
         "ld x0, 64*(\\start) -32*64(%0)\n"
         ".if 63-\\start\n"
         "    __LOAD_MISS%=   \"(\\start+1)\"\n"
         ".endif\n"
         ".endm\n"
         "__LOAD_MISS%=\n"
         : :"r" (__scratch+0x1000)
         );
    }
  };
  
  class UCStore{
  public:
    // 32 Stores to an ESR
    INLINE void operator()(){
      volatile uint64_t *esr = reinterpret_cast<volatile uint64_t*> ( 0x803400C8);
      __asm__ __volatile__
        ( ".rept 32\n"
          "sd x0, %0\n"
          ".endr\n"
          : :"m" (*esr)
          );
    }
  };
  
  class UCLoad{
  public:
    // 32 Loads from an ESR
    INLINE void operator()(){
      volatile uint64_t *esr = reinterpret_cast<volatile uint64_t*> ( 0x803400C8);
      __asm__ __volatile__
        ( ".rept 32\n"
          "ld x0, %0\n"
          ".endr\n"
          : :"m" (*esr)
          );
    }
  };
  
  class LoadMissSec{
  public:
    // load misses, but including secondary misses
    INLINE void operator()(){
      __asm__ __volatile__
        (
         ".macro  __LOAD_MISS%= start=0\n"
         "ld x0, 64*(\\start) -32*64(%0)\n"
         "ld x0, 32+64*(\\start) -32*64(%0)\n"
         ".if 63-\\start\n"
         "    __LOAD_MISS%=   \"(\\start+1)\"\n"
         ".endif\n"
         ".endm\n"
         "__LOAD_MISS%=\n"
         : :"r" (__scratch+0x1000)
         );
    }
  };
}
#endif

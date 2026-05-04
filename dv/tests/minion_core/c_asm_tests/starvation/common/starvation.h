/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _STARVATION_H_
#define _STARVATION_H_

#include "starvation_defines.h"
#include "param_sweep.h"
#include "fcc.h"


////////////////////////////////////////////////////////////////////////////////
// Main class that all tests inherit from
// the run_tests virtual function needs to be implemented
////////////////////////////////////////////////////////////////////////////////
template<class T, size_t NR_HARTS = THREADS_PER_NEIGH>
class starvation {
protected:
  // helper functions to concat arrays
  template<size_t N1, size_t... I1, size_t N2, size_t... I2>
  static constexpr std::array<run_func_t, N1+N2> concat
  ( const std::array<run_func_t, N1>& a, const std::array<run_func_t, N2>& b, 
    std::index_sequence<I1...>, std::index_sequence<I2...>){
    return { a[I1]..., b[I2]... };
  }
  
  template<size_t N1, size_t N2>
  static constexpr std::array<run_func_t, N1 + N2> concat
  ( const std::array<run_func_t, N1> a,  const std::array<run_func_t, N2> b) {
    return  concat(a, b, std::make_index_sequence<N1>{}, std::make_index_sequence<N2>{});
  }
  
  template<size_t first, size_t second, size_t... N>
  static constexpr auto concat
  ( const std::array<run_func_t, first> a,  const std::array<run_func_t, second> b, const std::array<run_func_t, N> ... rest) {
    return concat(a, concat(b, rest...));
  }
  
  // actual test functions
  static void notify_end() {
    volatile uint64_t* cred_inc_control = (volatile uint64_t*) CREDIT_INC_AD_T0;
    volatile uint64_t* cred_inc_end = (volatile uint64_t*) CREDIT_INC_AD_END;

    // notify both end of test and end of iteration
    uint64_t control_hart = get_hart_id() & ~(THREADS_PER_NEIGH-1);
    uint64_t mask = 1ull << (control_hart/2);
    *cred_inc_end = mask;
    *cred_inc_control = mask;
  }
  
  static void run_tests(unsigned id){
    T::run_tests(id);
  }

  static constexpr size_t TEST_THREADS_PER_NEIGH  = (NR_HARTS - 2);
  static_assert(NR_HARTS % 2 == 0 && NR_HARTS > 0);
  static void test_control(unsigned neigh) {

    // some ESRs:
    volatile uint64_t* msip = (volatile uint64_t*) TRIGGER_MSIP_AD ;
    volatile uint64_t* cred_inc_t0 = (volatile uint64_t*) CREDIT_INC_AD_T0;
    volatile uint64_t* cred_inc_t1 = (volatile uint64_t*) CREDIT_INC_AD_T1;

    // values to write to these ESRs (for harts 2 to 15 of this neigh)
    uint64_t msip_mask = 0xFFFCull << (neigh*16);
    uint64_t cred_inc_mask = 0xFE << (neigh*8);

    // for each small test, wait for EXPECTED_CYCLES and go to next one (by sending msip)
    uint64_t count = 0;
    __asm__ __volatile__ ("csrwi mhpmevent3, 1"); // to count cycles

    while(true) {
      
      // wait for notification test threads to start
      // set validation3, just for waveform debug purposes
      __asm__ __volatile__
        (
         "li t0, -1\n"
         "csrw validation3, t0\n"
         ".rept %[n_credits]\n" 
         "csrw fcc, x0\n"
         ".endr\n"
         "csrw validation3, %[d]\n"
         : : [d] "r" (count), [n_credits] "i" (TEST_THREADS_PER_NEIGH) : "t0");
      count++;
      
      // read FCC1, if equal to TEST_THREADS_PER_NEIGH, it means we are done with the test
      uint64_t nb_fcc;
      __asm__ __volatile__ ("csrr %0, fccnb\n" : "=r" (nb_fcc));
      if ( (nb_fcc >> 16 )  == TEST_THREADS_PER_NEIGH) break;
      
      
      // otherwise, do next iteration
      // tell test threads to start looping by incrementing fcc0
      *cred_inc_t0 = cred_inc_mask;
      *cred_inc_t1 = cred_inc_mask;
    
      // start counting
      __asm__ __volatile__ ("csrwi mhpmcounter3, 0"); // reset nr cycles
      uint64_t cycles;
      do {
        __asm__ __volatile__ ("csrr %[cycles], mhpmcounter3" : [cycles] "=r" (cycles));
      } while ( cycles < EXPECTED_CYCLES);
      
      // trigger msip in thest threads, so that they advance to the next iterations
      *msip = msip_mask;
    }

    #ifdef POSTSI_UST
      // minion 0 (hart 0) give (increment) FCC credit to hart 1 (unused) of the same neigh 
      FENCE;
      give_credit_local(neigh*8, 1, 0);    // from "fcc.h"
    #endif
 
    c_test_pass();
  }


  static void  __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler(){
    // set mepc = mscratch 
    __asm__ __volatile__
      (
       "csrr t0, mscratch\n"
       "csrw mepc, t0\n"
       : : : "t0"
       );
    // clear msip
    uint64_t id = get_hart_id() % 64;
    CLEAR_MSIP(id);
    __asm__ __volatile__ ("fence");
    
  }
  
  
public:
  static void run() {
    C_TEST_START;
    unsigned id = get_hart_id();
    unsigned hart = id % THREADS_PER_NEIGH;
    unsigned neigh = (id / THREADS_PER_NEIGH) & 3;
    // neigh 1, core 0, thread 0 will control the test
    // minions in neigh 0 will be the ones being tested
    // other minions / harts will do nothing
    if (hart == CONTROL_HART) test_control(neigh);
    else if (hart != UNUSED_HART) {
      // before starting, setup trap handler (for MSIP)
      __asm__ __volatile__ 
        (
         "csrw mtvec, %0\n"
         "csrwi mie, 8\n"
         "csrsi mstatus, 8\n"
         : : "r" (trap_handler)
         );
      run_tests( (hart ^ (hart >> 1)) & 1 );
      notify_end();
      c_test_pass();
    }
    else { // other minions / harts => hart 1 of each neigh is unused
      #ifdef POSTSI_UST
        // (mainly for hart 1): wait for FCC credit from hart 0:
          wait_for_credit(0);    // from "fcc.h" 
      #endif
      c_test_pass();
    }
  }

  
};


#endif

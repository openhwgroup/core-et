/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef RTLMIN_5705_H
#define RTLMIN_5705_H

//#define FAST_COMP //just uncomment to have the code compile faster, for testing only

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "rand_tables.h"
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////
// functions
////////////////////////////////////////////////////////////////////////////////
void test_control();
void run_tests(unsigned id);
extern "C" void   __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler();

////////////////////////////////////////////////////////////////////////////////
// Test parameters
////////////////////////////////////////////////////////////////////////////////

#define EXPECTED_CYCLES 8000 //long enough to get timeout if a thread does not retire instructions (plusarg timeout is 3000)

#ifndef FAST_COMP
#define NR_TESTS 50
#define RAND_TABLES_SIZE 512
#define MAX_ALIGNMENT INS_PER_LINE-1
#define MAX_LOOP_SIZE INS_PER_LINE*4-1
#define MAX_CAT_JUMPS 16
#else
#define NR_TESTS 13
#define RAND_TABLES_SIZE  2
#define MAX_ALIGNMENT 2
#define MAX_LOOP_SIZE 2
#define MAX_CAT_JUMPS 4
#endif

////////////////////////////////////////////////////////////////////////////////
// other constants and macros
////////////////////////////////////////////////////////////////////////////////
#define CONTROL_HART 16
#define THREADS_PER_NEIGH 16
#define THREADS_PER_NEIGH_L2 4
#define INS_PER_LINE 16


#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))
#define CREDIT_INC_AD_T0   0x803400C0
#define CREDIT_INC_AD_T1   0x803400D0

#define TRIGGER_MSIP_N0 do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 0xFFFF; } while(0)
#define CLEAR_MSIP(id)  do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1<<id; } while(0)
#define CRED_INC_H16    do { *((volatile uint64_t*) CREDIT_INC_AD_T0   ) = 1<<8; } while(0)
#define CRED_INC_N0     do {                              \
    *((volatile uint64_t*) CREDIT_INC_AD_T0   ) = 0xFF;   \
    *((volatile uint64_t*) CREDIT_INC_AD_T1    ) = 0xFF;  \
  } while(0)

////////////////////////////////////////////////////////////////////////////////
// macros for inline assembly
////////////////////////////////////////////////////////////////////////////////
#define ALIGN_LINE    ".align 6\n"
#define ALIGN_HALF_LINE    ".align 6\n"
#define TEST_START "la t0, 999f\ncsrw mscratch, t0\ncsrw fcc, x0\n"
#define TEST_END "999: nop\n"
#define TEST_REG_MODS "t0"

////////////////////////////////////////////////////////////////////////////////
// macros for function attributes
////////////////////////////////////////////////////////////////////////////////
#define NO_INLINE  __attribute__ ((noinline))   __attribute__ ((aligned (1<<6)))
#define ALWAYS_INLINE  __attribute__((always_inline)) inline
#define NAKED __attribute__ ((naked))


////////////////////////////////////////////////////////////////////////////////
// single loops
////////////////////////////////////////////////////////////////////////////////
template <unsigned thread, unsigned alignment, unsigned loop_size> // alignment in instructions
struct jump_loop{
  static ALWAYS_INLINE void fnc() {
    // sync with H16
    CRED_INC_H16;
    
    // and the loop itself
    __asm__ __volatile__
      (
       TEST_START
       ALIGN_LINE
       ".rept %[alignment]"
       "nop\n"
       ".endr\n"
       "1:\n"
       ".rept %[loop_size]"
       "nop\n"
       ".endr\n"
       "j 1b\n"
       TEST_END
       : : [alignment] "i" (alignment), [loop_size] "i" (loop_size)
       : TEST_REG_MODS
       );
  }
};

#define do_loop(a,l) jump_loop<thread, a, l>::fnc()
#define self_jump_end_of_line jump_loop<thread, INS_PER_LINE-1,0>::fnc
#define self_jump_end_of_half_line jump_loop<thread, INS_PER_LINE/2-1,0>::fnc

////////////////////////////////////////////////////////////////////////////////
// concatenated loops
////////////////////////////////////////////////////////////////////////////////
template<unsigned thread, unsigned iteration, unsigned NR_JUMPS>
struct concatenated_jumps {

  typedef rand_table_range <RAND_TABLES_SIZE, 0, MAX_ALIGNMENT, defaultseed ^ thread>  rand_alignment;

  template<unsigned alignment, void(*dest)(), unsigned offset>
  struct jump_{
    static ALWAYS_INLINE void fnc() {
      __asm__ __volatile__ 
        (
         ".rept %[nr_nops]\n"
         "nop\n"
         ".endr\n"
         "j %[dest]\n"
         "csrwi validation3, %[nr_nops]\n" // won't be executed, just to help debug
         :
         : [nr_nops] "i" (alignment), [dest] "i" ( (uint8_t*)(dest) + offset*4 )
         :
         );
    }
  };

  template <unsigned IDX, unsigned tid>
  struct jump_link {
    static void NAKED NO_INLINE fnc(){
      constexpr unsigned alignment = rand_alignment::get(IDX + iteration*NR_JUMPS);
      constexpr unsigned offset = rand_alignment::get(IDX + 1 + iteration*NR_JUMPS);
      jump_<alignment, jump_link<IDX+1, tid>::fnc, offset>::fnc();
    }
  };
  
  template<unsigned tid>
  struct jump_link <NR_JUMPS-1, tid> {
    static void NAKED NO_INLINE  fnc(){
      constexpr unsigned alignment = rand_alignment::get(NR_JUMPS-1 + iteration*NR_JUMPS);
      constexpr unsigned offset = rand_alignment::get(iteration*NR_JUMPS);
      jump_<alignment, jump_link<0, tid>::fnc, offset>::fnc();
    }
  };
  
  static void ALWAYS_INLINE fnc(){
    // sync with H16
    CRED_INC_H16;
    
    // and start infinite loop
    __asm__ __volatile__
      (
       TEST_START
       "j %[dest]\n"
       TEST_END
       :
       : [dest] "i" (jump_link<0, thread>::fnc)
       : TEST_REG_MODS
       );

  }
  
};


////////////////////////////////////////////////////////////////////////////////
// main test function
////////////////////////////////////////////////////////////////////////////////

template<unsigned thread, unsigned iteration>
struct test{
  typedef rand_table<RAND_TABLES_SIZE> rand_int;
  typedef rand_table_range<RAND_TABLES_SIZE, 0, MAX_ALIGNMENT, defaultseed ^ thread>  unif_rand_alignment;
  typedef rand_table_range<RAND_TABLES_SIZE, 0, MAX_LOOP_SIZE, defaultseed ^ thread > unif_rand_loop_size;
  typedef rand_table_range<RAND_TABLES_SIZE, 2, MAX_CAT_JUMPS, defaultseed ^ thread > unif_rand_nr_jumps;
  typedef rand_mask_bag<RAND_TABLES_SIZE,
#ifdef TEST_THREADS_LIMIT
                        TEST_THREADS_LIMIT,
#else
                        THREADS_PER_NEIGH,
#endif
                        uint16_t, defaultseed,
                        bag_bin<unsigned, 1, 20>,
                        bag_bin<unsigned, 2, 8>,
                        bag_bin<unsigned, 3, 6>,
                        bag_bin<unsigned, 4, 2>,
                        bag_bin<unsigned, 5, 2> > rand_mask;
  
  enum operation_t { ALL_JUMP_EOL, ALL_JUMP_HOL, MOST_EOL, MOST_HOL, RAND_LOOP, J2J, ALL_RAND, NR_OPERATIONS};
  typedef rand_table_range<RAND_TABLES_SIZE, 0, NR_OPERATIONS-1, defaultseed> rand_op; // note that the seed doesn't depend on the thread

  static void NO_INLINE fnc(){
    
    uint16_t mask = rand_mask::get(iteration);
    operation_t op = static_cast<operation_t>(rand_op::get(iteration));
    
    // write what loop it is doing, just for debugging purposes
    __asm__ __volatile__
      ("csrw validation3, %[v]\n"
       : : [v] "r" ( uint64_t(op) | ( (uint64_t)mask << 16)));

#ifdef TEST_THREADS_LIMIT
    if ( thread >= TEST_THREADS_LIMIT ) {
      // sync with H16
      CRED_INC_H16;
      __asm__ __volatile
        (
         TEST_START
         "wfi\n"
         TEST_END
         : : : TEST_REG_MODS );
      return;
    }
#endif


    switch (op) {
      // jump to itself, at end of line
    case ALL_JUMP_EOL:   
      self_jump_end_of_line();
      break;
      
      // jump to itself, at end of half line
    case ALL_JUMP_HOL:
      self_jump_end_of_half_line();
      break;

    // some threads doing nops, other threads jumping to itself at end of line
    case MOST_EOL:

      if ( ((mask >> thread) & 1)  == 1) do_loop(0, MAX_ALIGNMENT); 
      else self_jump_end_of_line();
      break;

      // some threads doing nops, other threads jumping to itself at half of line
    case MOST_HOL:
      if ( ((mask >> thread) & 1)  == 1) do_loop(0, MAX_ALIGNMENT); 
      else self_jump_end_of_half_line();
      break;

    // random alignments and loop sizes, following uniform distribution
    case RAND_LOOP:
      do_loop(unif_rand_alignment::get(iteration), unif_rand_loop_size::get(iteration));
      break;
      
      // concatenated jumps (jump to another jump instruction, with random alignments)
    case J2J:
      concatenated_jumps<thread, iteration, unif_rand_nr_jumps::get(iteration)>::fnc();
      break;
      
    default: // ALL_RAND:
      if (rand_int::get(iteration) % 2 == 0)
        do_loop(unif_rand_alignment::get(iteration), unif_rand_loop_size::get(iteration));
      else
        concatenated_jumps<thread, iteration, unif_rand_nr_jumps::get(iteration)>::fnc();
      break;
      
    }
  }
};


struct test_wrapper{
  static void fnc(unsigned id, unsigned iter) {
    table_<>::get( (id & (THREADS_PER_NEIGH-1)) | (iter << THREADS_PER_NEIGH_L2)) ();
  }
  typedef void(*func_t)();

  template<unsigned IDX=0, func_t...funcs>
  struct table_ : table_<IDX+1, funcs..., test<IDX & (THREADS_PER_NEIGH -1), (IDX>>THREADS_PER_NEIGH_L2) >::fnc> { };
  
  // last element
  template<func_t ...funcs>
  struct table_<NR_TESTS*THREADS_PER_NEIGH, funcs...> {
    static func_t get(const unsigned idx) {
      constexpr static std::array<func_t, NR_TESTS*THREADS_PER_NEIGH> data_ = { funcs...};
      return data_[idx%(NR_TESTS*THREADS_PER_NEIGH)];
    }
    
  };
};


#endif

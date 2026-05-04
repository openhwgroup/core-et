/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _GSC_STARVATION_H_
#define _GSC_STARVATION_H_

#include "starvationTest.h"
#define FLB0                0x80F40100

enum shared_resource_t {DIV, DIVW, MUL, MULW, EXCL, LD, NR_RES};
enum gsc_t {G_MEM, S_MEM, G_ESR, S_ESR, NR_GSC};

template<size_t repeat, size_t nops_before_gsc, size_t nops_after_gsc,
         size_t nops_second_thread, size_t shared_res, size_t gsc_ins>

class gsc_starvation_test :
  public starvation_test_base < repeat,
                                gsc_starvation_test<repeat, nops_before_gsc, nops_after_gsc,
                                                    nops_second_thread, shared_res, gsc_ins> > {
public:

  static constexpr uint64_t min_iter_first_loop(){
    constexpr uint64_t loop_cycles = (nops_before_gsc + nops_after_gsc + 
                                      8 + /*gsc time*/ 
                                      shared_resource_cycles()) * repeat  + 1 + 12; /*1: x31 inc, 12:jump back time*/ 
    return EXPECTED_CYCLES / (loop_cycles * 20); // 20 => at least 5 % of that


  }

  static constexpr uint64_t min_iter_second_loop(){
    constexpr uint64_t loop_cycles = (nops_second_thread +
                                      shared_resource_cycles()) * repeat  + 1 + 12;  /*1: x31 inc, 12:jump back time*/ 
    return EXPECTED_CYCLES / (loop_cycles * 20); // 20 => at least 5 % of that
  }
  static void ALWAYS_INLINE first_thread_iter() {
    nops<nops_before_gsc>();
    gsc();
    nops<nops_after_gsc>();
    shared_resource();
    incx31();
  }

  static void ALWAYS_INLINE second_thread_iter() {
    nops<nops_second_thread>();
    shared_resource();
    incx31();
  }

  static void ALWAYS_INLINE incx31() {
    __asm__ __volatile ( "addi x31, x31, 1\n");
  }
  
  // prepare the offsets before running the test
  static void setup() {
    // entering with ft1={0,64,...}
    // and ft2={0,8,16..}
    int32_t offsets1[8];
    int32_t offsets2[8];
    for( int i = 0; i < 8 ; i++) {
      offsets1[i] = i*64;
      offsets2[i] = i*8;
    }
    __asm__ __volatile__ ( "flq2 ft1, %0\n"
			   "flq2 ft2, %1\n"
			   : :
			     "m" (*(const int32_t(*)[8]) offsets1),
			     "m" (*(const int32_t(*)[8]) offsets2) );  
  }

private:

  static void ALWAYS_INLINE gsc() {
    uint8_t x[512];
    // entering with ft1={0,64,...}
    // and ft2={0,8,16..}
    switch(gsc_ins){
    case G_MEM: asm("fgw.ps ft0, ft1(%0)" : : "r" (x)); break;
    case S_MEM: asm("fscw.ps ft0, ft1(%0)" : : "r" (x)); break;
    case G_ESR: asm("fgw.ps ft0, ft2(%0)" : : "r" (FLB0)); break;
    case S_ESR: asm("fscw.ps ft0, ft2(%0)" : : "r" (FLB0)); break;
    default:
      C_TEST_FAIL; break;
    }
  }

  static void ALWAYS_INLINE shared_resource(){
    uint64_t in1 = 100, in2=4;
    switch(shared_res){
    case DIV:  asm volatile ("div x0, %0, %1" : : "r" (in1), "r" (in2)); break;
    case MUL:  asm volatile ("mul x0, %0, %1" : : "r" (in1), "r" (in2)); break;
    case DIVW:  asm volatile ("divw x0, %0, %1" : : "r" (in1), "r" (in2)); break;
    case MULW:  asm volatile ("mulw x0, %0, %1" : : "r" (in1), "r" (in2)); break;
    case EXCL: asm volatile ( "csrwi excl_mode, 1\n"
			      "csrwi excl_mode, 0\n"
			      ); 
      break;
    case LD: asm volatile("ld x0, %0" : : "m" (*(const int64_t(*)[1]) &in1) ); break;
    default: C_TEST_FAIL; break;
    }
  }
  
  static constexpr uint64_t shared_resource_cycles () {
    return 
      shared_res == DIV ? 64 :
      shared_res == MUL? 8 :
      shared_res == DIVW? 32 :
      shared_res == MULW? 8 :
      shared_res == EXCL? 5 :
      0;
  }
};


#endif

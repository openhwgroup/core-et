/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



#ifndef _MSIP_H_
#define _MSIP_H_

#include "esr_defines.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 10

#define CAUSE_USER_ECALL             0x8
#define CAUSE_SUPERVISOR_ECALL       0x9
#define CAUSE_MACHINE_ECALL          0xb
#define CAUSE_BAD_RED               ((1ULL<<63) | 16)
#define WAIT_FCC0  __asm__ __volatile__ ("csrw fcc, x0\n")


EXTERN_C void  __attribute__ ((interrupt)) __attribute__((aligned(4096)))  mtrap_handler();


#define REDIRECT_DEST_FN(n)                                     \
  void __attribute__ ((naked)) redirect_destination##n();       \
  void redirect_destination##n() {                              \
    /* just jump to to PC stored in 's11'*/                     \
    __asm__ __volatile__                                        \
      (                                                         \
       "nop\n"                                                  \
       "nop\n"                                                  \
       "mv t0, s11\n"                                           \
       "jalr x0, t0 ,0\n"                                       \
       "nop\n"                                                  \
       : : : "t0", "s11" );                                     \
    C_TEST_FAIL;                                                \
  }


#define REDIRECT_WAIT(asm_code)                              \
  __asm__ __volatile__                                       \
  (                                                          \
   "la t0, 1000f\n"                                          \
   "mv s11, t0\n"                                            \
   "li t0,  1\n"                                             \
   "sd t0, 0(%[fcc_dest])\n"                                 \
   asm_code                                                  \
   "1000: nop\n"                                             \
   :                                                         \
   : [fcc_dest] "r" ( fcc[!id])                              \
   : "t0", "s11" )



inline unsigned int get_hart_id() __attribute__((always_inline));
void test_receive(unsigned id);
void test_send(unsigned id);



#endif

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 5
#define OP_INCREMENT 100

#define SAME_SET (64*16)
#define STRING(x) #x

#define LOAD(dest, ptr, offset) \
  __asm__ __volatile__ (  "ld " STRING(dest) ", %[offs](%[pt])\n" \
                          : : [pt] "r" (& (ptr)), [offs] "i" (offset) : STRING(dest) );


#define STORE(src, ptr, offset)                                         \
  __asm__ __volatile__ ( "sd " STRING(src) ", %[offs](%[pt])\n" \
                         : : [pt] "r" (&(ptr)),  [offs] "i" (offset) );




volatile unsigned shared_var; // var shared between the 2 threads


int excl_mode_op(unsigned iters){
  // increments by 1 (thread0) and by 2 (thread1) the variable
  // but using memory to store intermediate results so that
  // concurrent threads bother each other
  shared_var = 0;
  int inc;
  __asm__ __volatile ( "csrr %[inc], mhartid\n" : [inc] "=r" (inc) );
  inc++;

  for ( unsigned i = 0; i < iters; i++) {
    __asm__ __volatile__
      (
       "ld t0, %[var]\n"
       "add t0, t0, %[inc]\n"
       "sd t0, %[var]\n"
       :
       : [var] "m" (shared_var), [inc] "r" (inc)
       : "t0"
       );
  }

  return ( shared_var == inc*iters);
}




int main()
{

  int i;
  volatile char __attribute__((aligned(4096))) ptr[8192];


  for ( i = 0; i < ITERATIONS; i++) {

    // do stuff dealing with memory...
    // trying to get to the L2 (loading/storing from the same set => ad +=64*16)
    LOAD(a0,   ptr[SAME_SET*0],  0);
    LOAD(a1,   ptr[SAME_SET*2],  0);
    LOAD(a2,   ptr[SAME_SET*3],  0);
    LOAD(a3,   ptr[SAME_SET*4],  0);
    LOAD(a4,   ptr[SAME_SET*5],  0);
    LOAD(a5,   ptr[SAME_SET*6],  0);
    LOAD(a6,   ptr[SAME_SET*7],  0);
    LOAD(a7,   ptr[SAME_SET*0],  0);
    LOAD(t0,   ptr[SAME_SET*1],  0);
    LOAD(t1,   ptr[SAME_SET*2],  0);
    STORE(a0,  ptr[SAME_SET*0],  0);
    STORE(a1,  ptr[SAME_SET*2],  0);
    STORE(a2,  ptr[SAME_SET*3],  0);
    STORE(a3,  ptr[SAME_SET*4],  0);
    STORE(a4,  ptr[SAME_SET*5],  0);
    STORE(a5,  ptr[SAME_SET*6],  0);
    STORE(a6,  ptr[SAME_SET*7],  0);
    STORE(a7,  ptr[SAME_SET*0],  0);
    STORE(t0,  ptr[SAME_SET*1],  0);
    STORE(t1,  ptr[SAME_SET*2], 0);


    // enter excl mode
    __asm__ __volatile__ ("csrwi 0x7d3, 1\n");
    if ( ! excl_mode_op(OP_INCREMENT) ){
      // test failed... infinite loop that will cause a timeout
      for(;;);
    }

    // exit excl mode
    __asm__ __volatile__ ("csrwi 0x7d3, 0\n");

  }


  // End marker
  C_TEST_PASS;

  return 0;
}

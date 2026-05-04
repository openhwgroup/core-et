/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <stdlib.h>
#include "macros.h"


#define ITERATIONS 3

#define RBOX_BASE 0xFFF40000

volatile uint64_t *rbox = (uint64_t*) RBOX_BASE;

int getHartId() {
    int  thread;
    __asm__ __volatile__
      (
       "  csrr %[tid], mhartid\n" // software thread
       : [tid] "=r" (thread)
       :
       :
       );
    return thread;
}

int asdf[] ={1,2,3,4,5,6,7};
char asdf3[] ={1,2,3};

static inline void wfi(){
  __asm__ __volatile__ 
    (
     "wfi\n"
     : 
     : 
     :
     );
}

int main()
{
  int thread = getHartId();

  C_TEST_START;

  // set mask to all 1's
  __asm__ __volatile__ 
    (
     "li a7, 0xff\n"
     "mova.m.x a7\n"
     : 
     : 
     : "a7"
     );
  
  
  for ( int i = 0 ; i < ITERATIONS; i++) {
    // set credit
    *rbox = thread;

    // wait for rbox data
    wfi();

      __asm__ __volatile__ 
        (
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         :
         :
         :
         );

    for( int j=0; j < 8; j++) {
      // read the CSR reg 8 times
      uint64_t data;
      __asm__ __volatile__ 
        (
         "csrr	%[data],0xfc1\n"
         : [data] "=r" (data)
         :
         :
         );
    }
  }
  
  return 0;
}

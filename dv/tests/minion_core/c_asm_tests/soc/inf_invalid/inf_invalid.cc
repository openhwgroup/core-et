/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

constexpr size_t NUM_UNIMP = 1024;

/*
 * Generate a very big secuence of unimplemented instructions 
 * to test the minion on a "hostile environment"
 */

void setup_trap_vector(void);

int main()
{
  C_TEST_START;
  // Don't setup mtrap_vector for now. Trap to PC:XXXXX so the ICACHE can't send a response
  setup_trap_vector();
  
  while(1) {
  #pragma GCC unroll NUM_UNIMP 
    for(size_t i = 0; i < NUM_UNIMP; ++i) {
      __asm__ __volatile__ ("unimp");
    }
  }
   

  C_TEST_FAIL;
  return 0;
}

void success_routine() {
  C_TEST_PASS;
}

__attribute__((naked)) void mtrap_vector(void)
{
  __asm__ __volatile__ ("unimp");

}

__attribute__((naked)) void strap_vector(void)
{
  __asm__ __volatile__ ("unimp");
}

// Setting up trap vector
void setup_trap_vector(void)
{
    void (*mtvec_ptr)(void) = &mtrap_vector;
    void (*stvec_ptr)(void) = &strap_vector;
    __asm__ __volatile__ (
        "mv t0, %[mtvec]\n\t"
        "mv t1, %[stvec]\n\t"
        "csrw mtvec, t0\n\t"
        "csrw stvec, t1\n\t"
        :
        :[mtvec] "r" (mtvec_ptr), [stvec] "r" (stvec_ptr)
        :
  );
}



/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#define S_ECALL 9

void inline to_s_mode() {
  __asm__ __volatile__ 
    (
     "csrr t0, mstatus\n"
     "li t1, 0x1880\n" // clear mpp and mpie
     "xori t1, t1, -1\n"
     "and t0, t0, t1\n"
     "li t1, 0x800\n"
     "or t0, t0, t1\n"
     "csrw mstatus, t0\n"
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mode
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t1", "t0"
     );
}

void inline to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}



#ifdef __cplusplus
extern "C"
#endif
void __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler()
{
  uint64_t mcause, mepc;
  __asm__ __volatile__
    (
     "csrr %[mcause], mcause\n"
     "csrr %[mepc], mepc\n"      
     : [mcause] "=r" (mcause), [mepc] "=r" (mepc) );

  if (mcause == S_ECALL) {
    // set mepc to +4, and mpp to 3
    mepc+=4;
    __asm__ __volatile__
      (
       "csrw mepc, %[mepc]\n"
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       : [mepc] "r" (mepc)
       : "t0" );
    
  }else {
    C_TEST_FAIL;
  }
  
}

void inline setup_trap_vector() {
  // configure trap vector
  __asm__ __volatile__ 
    (
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}


int main(){
  C_TEST_START;
  setup_trap_vector();
    
  uint64_t val;
  for ( int i = 0; i < 16; i++) {
    val = i | ( i<<4);
    val |= (val<<8);
    val |= (val<<16);
    
    // write and read in m-mode
    __asm__ __volatile__
      (
       "csrr t0, mie\n"
       "csrw mie, %[val]\n"
       "csrr t0, mie\n"
       :
       : [val] "r" (val)
       : "t0"
       );


    // write and read in S mode
    to_s_mode();
    __asm__ __volatile__
      (
       "csrr t0, sie\n"
       "csrw sie, %[val]\n"
       "csrr t0, sie\n"
       :
       : [val] "r" (val)
       : "t0"
       );


    to_m_mode();
  }

  C_TEST_PASS;
  return 0;
}

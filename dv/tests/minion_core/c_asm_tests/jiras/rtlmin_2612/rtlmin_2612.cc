/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>


#ifdef __cplusplus
extern "C"
#endif
void   __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler();

int main(){
  C_TEST_START;

  // set trap vector
  __asm__ __volatile__  ("csrw mtvec, %[vect]\n" : : [vect] "r" (trap_handler));


  //accessing ad=0 => exception expected
  __asm__ __volatile__
    (
     "amoorl.w x1, x1, (x0)\n"
     :
     :
     : "x1"
     );
  
  // fail if not exception
  C_TEST_FAIL;
  return 0;
}


#define STORE_ACCESS_FAULT_CAUSE 7
void trap_handler(){
  // read mcause
  uint64_t mcause;
  __asm__ __volatile__   (
     "csrr %[c], mcause\n"
     : [c] "=r" (mcause) );
  
  // mcause has to be STORE ACCESS FAULT
  if ( mcause == STORE_ACCESS_FAULT_CAUSE) {
    C_TEST_PASS;
  }
  else{
    C_TEST_FAIL;
  }


}

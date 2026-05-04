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
  
  __asm__ __volatile__
    (
     "add x1, x30 ,x31\n"
     "c.addi x1, 1\n"
     "add x1, x30 ,x31\n"
     "c.nop\n"
     "c.addi x0, 1\n"
     "c.addi x0, 0\n"
     "nop\n"
     :
     :
     : "x1"
     );
  
  C_TEST_PASS;
  return 0;
}



void trap_handler(){
  C_TEST_FAIL;
}

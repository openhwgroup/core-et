/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef __cplusplus
extern "C"
#endif

void __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler() {
  uint64_t mcause, mtval;

  __asm__ __volatile__
  (
    "csrr %[c], mcause\n"
    "csrr %[v], mtval\n"
    : [c] "=r" (mcause), [v] "=r" (mtval)
  );

  if (mcause == 2 && mtval == WFI_INS) {
    C_TEST_PASS;
  }
  else {
    C_TEST_FAIL;
  }
}


void change_to_user_mode (){
  // register long s asm("t0") = 0;  unused variable

  // configure trap vector and move to user mode
    __asm__ __volatile__
      (
       "la t0, trap_handler\n"
       "csrw mtvec, t0\n"
       // Configure satp to use physical addresses 
       "li t0, 0xF000000000000000\n" // Mode Sv48
       "csrrc x0, satp, t0\n"
       "li t0, 0x1800\n"
       "csrrc x0, mstatus, t0\n"  // clear mstatus.mpp
       "la t0, 1f\n"
       "csrw mepc, t0\n"          // set mepc to user-mode entry point
       "mret\n"                   // go to user mode
       "1:\n"                     // label to referr to user-mode
       :
       :
       : "t0"
       );
}
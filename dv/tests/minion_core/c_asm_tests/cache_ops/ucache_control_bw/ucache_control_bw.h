/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _RTLMIN_UCACHE_BW__
#define _RTLMIN_UCACHE_BW__

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define CAUSE_USER_ECALL         0x8
#define CAUSE_SERVICE_ECALL         0x9
#define CAUSE_MACHINE_ECALL         0xb



EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval);
EXTERN_C void test_mtrap_vector();

void m_to_u_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t0"
     );
}

void to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}
#endif

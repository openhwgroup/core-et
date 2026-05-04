/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef ALLOW_TRAP_TO_DEBUG
constexpr bool CAN_TRAP_TO_DEBUG = true;
#else
constexpr bool CAN_TRAP_TO_DEBUG = false;
#endif

#define CAUSE_USER_ECALL         0x8
#define CAUSE_SERVICE_ECALL         0x9
#define CAUSE_MACHINE_ECALL         0xb

#ifdef ALLOW_TRAP_TO_DEBUG
  #define ITERATIONS 50 // The debugger is VERY SLOW. Could take hours. Reducing iterations to something more resonable
#else
  #define ITERATIONS 200
#endif

constexpr uint64_t CAUSE_BUS_ERROR = 23;


EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t thread);
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

void m_to_s_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6800\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t0"
     );
}

void to_m_mode(uint64_t &priv_mod) {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
  priv_mod=3;
}

inline void ebreak() {
  __asm__ __volatile__ ("ebreak\n");
}

inline void set_mscratch(uint64_t value) {
  __asm__ __volatile__ 
    (
     "csrrw zero, mscratch, %[v]\n"
     :
     : [v] "r" (value)
     : 
     );
}

inline uint64_t get_mscratch() {
  uint64_t value;
    __asm__ __volatile__ 
    (
     "csrr %[v], mscratch\n"
     : [v] "=r" (value)
     :
     : 
     );
    return value;
}





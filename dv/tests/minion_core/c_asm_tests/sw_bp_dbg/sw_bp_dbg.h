/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#include <inttypes.h> 

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

constexpr uint64_t NUM_HARTS = 2048;

constexpr uint64_t TRAPS_TO_DEBUG = 0x1;
constexpr uint64_t TRAPS_TO_MHANDLER = 0x2;
  

#define CAUSE_USER_ECALL         0x8
#define CAUSE_SERVICE_ECALL         0x9
#define CAUSE_MACHINE_ECALL         0xb

#define CAUSE_BREAKPOINT 3

EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t thread);
EXTERN_C void test_mtrap_vector();

inline void random_delay(uint64_t d);

void print_boolean(bool val){
 if(val) 
      et_write_val2(0x7e5);
    else 
      et_write_val2(0x40);
}


void m_to_u_mode(uint64_t &priv_mod) {
  priv_mod = 0;
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

void m_to_s_mode(uint64_t &priv_mod) {
  priv_mod = 1;
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
  __asm__ __volatile__ 
    ("addi x31,x0, 0\n"
     "ebreak\n"
     "nop\n"
     :
     :
     : "x31"
     );
}

inline void ebreak_done() {
  __asm__ __volatile__ 
    ("addi x31,x0, 1\n"
     "ebreak\n"
     "nop\n"
     :
     :
     : "x31"
     );
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



inline uint64_t atomic_swap(volatile uint64_t* addr, uint64_t new_value) {
  uint64_t old_value;
  
  __asm__ __volatile__ 
    (
    "amoandl.d %[old_value], %[new_value], (%[addr])\n"
    : [old_value] "=r" (old_value)
    : [new_value] "r" (new_value), [addr] "r" (addr)
    :
     );
  return old_value;
}

inline void atomic_set(volatile uint64_t* addr, uint64_t new_value) {
  const uint64_t old_value = atomic_swap(addr, new_value);
  (void)old_value;
}

inline void set_flb(uint64_t hart, uint64_t value) {
  uint64_t reg_addr = 0x020ULL + hart;
  uint64_t esr_addr = 1ULL << 32 | 0xFFULL << 22 | 0b11010ULL << 17 | reg_addr << 3;
  volatile uint64_t* esr_addr_p = reinterpret_cast<volatile uint64_t*>(esr_addr);
  *esr_addr_p = value;
}

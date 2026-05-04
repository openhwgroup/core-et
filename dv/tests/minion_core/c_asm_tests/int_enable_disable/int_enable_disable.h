/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _INT_EN_DIS_H_
#define _INT_EN_DIS_H_

#include "esr_defines.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define CAUSE_ILLEGAL                0x2
#define CAUSE_USER_ECALL             0x8
#define CAUSE_SUPERVISOR_ECALL       0x9
#define CAUSE_MACHINE_ECALL          0xb
#define CAUSE_MSIP                  ((1ULL<<63) | 3)
#define CAUSE_SSIP                  ((1ULL<<63) | 1)


#define WAIT_FCC0  __asm__ __volatile__ ("csrw fcc, x0\n")

#define ITERATIONS 20
#define MSIP_PER_ITERATION 15

#define RANDOM_EXEC(_percent_, _what_, _debug_)        \
  do {                                                 \
    if (get_rand_percent(rand_ptr) <= (_percent_)) {   \
      _what_;                                          \
      debug|= 1ULL<<(32+ (_debug_));                   \
    }                                                  \
  } while(0)


// probabilities in percentages
#define PROB_CHANGE_PRV 60
#define PROB_SET_SSIP 25
#define PROB_DELEG_SSIP 30
#define PROB_CHANGE_IE 66
#define PROB_ILLEGAL_INSN 12

// helper functions to access CSRs
inline void install_trap_handler(void (*m)(), void (*s)());
inline void mstatus_mie_sie(bool mie, bool sie);
inline void sstatus_sie(bool sie);
inline unsigned int get_hart_id();
inline void enable_shadows();
inline void set_sip_ssip(bool en);
inline void set_mip_ssip(bool en);
inline void deleg_ssip(bool en);
inline void set_mie(bool msie, bool ssie);
inline void set_mie(uint64_t x);
inline uint64_t get_mie();
inline void set_sie(bool ssie);
inline bool get_sie();
inline void clear_msip();
inline bool peek_msip(unsigned thread);
inline void trigger_msip(unsigned thread);
inline void privilege_up(unsigned &prv, uint64_t debug = 0);
inline void privilege_down(unsigned &prv, uint64_t debug = 0);
inline uint64_t peek_fcc(uint64_t cnt);
inline void inc_fcc0(unsigned thread);
inline unsigned get_rand_percent(unsigned &ptr);
inline unsigned get_rand_bit(unsigned &ptr);
inline void set_debug(uint64_t d, unsigned prv);
inline void illegal_insn0();
inline void illegal_insn1();
inline void illegal_insn2();
void random_delay(unsigned d);

#define READ_CSR(c, d)   __asm__ __volatile__ ("csrr %[dd]," c "\n" : [dd] "=r" (d))

// the test functions
void test_receiver(unsigned &prv);
void test_sender();

// the trap handlers
EXTERN_C void  __attribute__ ((interrupt)) __attribute__((aligned(4096)))  mtrap_handler_receiver();
EXTERN_C void  __attribute__ ((interrupt)) __attribute__((aligned(4096)))  mtrap_handler_sender();
EXTERN_C void  __attribute__ ((interrupt("supervisor"))) __attribute__((aligned(4096)))  strap_handler_receiver();
EXTERN_C void  __attribute__ ((interrupt("supervisor"))) __attribute__((aligned(4096)))  strap_handler_sender();
void mtrap_handler(bool interrupts_allowed, uint64_t ecall_param);
void strap_handler(bool interrupts_allowed);


#endif

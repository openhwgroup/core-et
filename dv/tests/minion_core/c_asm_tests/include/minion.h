/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __MINION_H
#define __MINION_H

#ifdef __cplusplus
  #include <cstdint>
#else
  #include <stdint.h>
#endif

typedef enum {
    /* Standard Exceptions */
    MINION_XCTP_CAUSE_INST_ADDR_MISALIGNED  = 0,  /* Instruction address misaligned */
    MINION_XCTP_CAUSE_INST_ACCESS_FAULT     = 1,  /* Instruction access fault */
    MINION_XCTP_CAUSE_ILLEGAL_INST          = 2,  /* Illegal instruction */
    MINION_XCTP_CAUSE_BREAKPOINT            = 3,  /* Breakpoint */
    MINION_XCTP_CAUSE_LOAD_ADDR_MISALIGNED  = 4,  /* Load address misaligned */
    MINION_XCTP_CAUSE_LOAD_ACCESS_FAULT     = 5,  /* Load access fault */
    MINION_XCTP_CAUSE_STORE_ADDR_MISALIGNED = 6,  /* Store/AMO address misaligned */
    MINION_XCTP_CAUSE_STORE_ACCESS_FAULT    = 7,  /* Store/AMO access fault */
    MINION_XCTP_CAUSE_ENV_CALL_FROM_U_MODE  = 8,  /* Environment call from U-mode */
    MINION_XCTP_CAUSE_ENV_CALL_FROM_S_MODE  = 9,  /* Environment call from S-mode */
    /* 10 is Reserved */
    MINION_XCTP_CAUSE_ENV_CALL_FROM_M_MODE  = 11, /* Environment call from M-mode */
    MINION_XCTP_CAUSE_INST_PAGE_FAULT       = 12, /* Instruction page fault */
    MINION_XCTP_CAUSE_LOAD_PAGE_FAULT       = 13, /* Load page fault */
    /* 14 is Reserved */
    MINION_XCTP_CAUSE_STORE_PAGE_FAULT      = 15, /* Store/AMO page fault */

    /* Hypervisor/Virtualization Exceptions */
    MINION_XCTP_CAUSE_INST_GUEST_PAGE_FAULT  = 20, /* Instruction guest page fault */
    MINION_XCTP_CAUSE_LOAD_GUEST_PAGE_FAULT  = 21, /* Load guest page fault */
    MINION_XCTP_CAUSE_VIRTUAL_INST           = 22, /* Virtual instruction */
    MINION_XCTP_CAUSE_STORE_GUEST_PAGE_FAULT = 23  /* Store/AMO guest page fault */
} minion_xctp_cause_t;    

typedef volatile __attribute__((aligned(64))) union {
   uint8_t  b[64];
   uint16_t h[32];
   uint32_t w[16];
   uint64_t d[8];
} minion_cache_line_t;


typedef volatile __attribute__((aligned(4096))) struct {
   minion_cache_line_t cl[64];
} minion_dcache_t;


inline unsigned int __attribute__((always_inline)) get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], mhartid\n"
      : [ret] "=r" (ret)
      :
      :
    );
    return ret;
}

// Get hart id relative to the shire (0..63)
inline unsigned int __attribute__((always_inline)) get_shire_hart_id()
{
    return get_hart_id() & 0x3F;
}

inline unsigned int __attribute__((always_inline)) get_shire_id()
{
   return (get_hart_id() >> 6) & 0x3F;
}

inline unsigned int __attribute__((always_inline)) get_neigh_id()
{
  return (get_hart_id() >> 4) & 3;
}

inline unsigned int __attribute__((always_inline)) get_minion_id()
{
   return (get_hart_id() >> 1) & 0x1F;
}

inline unsigned int __attribute__((always_inline)) get_thread_id()
{
   return get_hart_id() & 1;
}

inline uint64_t __attribute__((always_inline)) wait_for_credits(uint64_t credit_counter) {
  uint64_t result;
   __asm__ __volatile__ (
         "csrrw  %[result], 0x821 ,%[credit_counter]\n"
         : [result] "=r" (result)
         : [credit_counter] "r" (credit_counter)
	 : );
   return result;
}

#endif // ! __MINION_H

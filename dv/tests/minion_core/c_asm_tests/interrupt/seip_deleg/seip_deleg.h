/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _SEIP_DELEG_H_
#define _SEIP_DELEG_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 100
#define CAUSE_MACHINE_EXTERNAL_INTR ((1ULL<<63) | 11)
#define CAUSE_SUPERVISOR_EXTERNAL_INTR ((1ULL<<63) | 9)
#define CAUSE_USER_ECALL         0x8
#define CAUSE_SUPERVISOR_ECALL   0x9
#define CAUSE_MACHINE_ECALL      0xb

#define CREDIT_INC_AD           0x803400D0
#define CRED_INC     do { *((volatile uint64_t*) CREDIT_INC_AD   ) = 1; } while(0)

// Return using sret by declaring interrupt as supervisor
EXTERN_C void  __attribute__ ((interrupt ("supervisor"))) supervisor_handler();
EXTERN_C void  __attribute__ ((interrupt)) machine_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();

EXTERN_C void mtrap_vector();
inline unsigned int get_hart_id() __attribute__((always_inline));
inline void m_to_s_mode() __attribute__((always_inline));
void test_t0();
void test_t1();
inline void dump_irq_count(int cnt);
inline void set_external_interrupt(bool inject);

#endif //_SEIP_DELEG_H_

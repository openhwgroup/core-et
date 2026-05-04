/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PMC_ACCESS_H_
#define _PMC_ACCESS_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 20
#define PMC_NON_IMPLEMENTED  24
#define PMC_LAST_IMPLEMENTED  9
#define CAUSE_ILLEGAL_PMC_ACCESS 0x2 
#define CAUSE_USER_ECALL         0x8
#define CAUSE_SUPERVISOR_ECALL   0x9
#define CAUSE_MACHINE_ECALL      0xb

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
EXTERN_C void mtrap_vector();

inline void m_to_s_mode() __attribute__((always_inline));
inline void m_to_u_mode() __attribute__((always_inline));
inline void to_m_mode() __attribute__((always_inline));
inline unsigned int get_hart_id() __attribute__((always_inline));
void access_all_pmcs();
inline void dump_irq_count(int cnt);

#endif //_PMC_ACCESS_H_

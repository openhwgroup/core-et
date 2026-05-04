/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TRAP_DELEGATE_H_
#define _TRAP_DELEGATE_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 100
#define CAUSE_USER_ECALL         0x8
#define CAUSE_SUPERVISOR_ECALL   0x9
#define CAUSE_MACHINE_ECALL      0xb

// Return using sret by declaring interrupt as supervisor
EXTERN_C __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt ("supervisor"))) void supervisor_handler();
EXTERN_C __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) void machine_handler();

inline unsigned int get_hart_id() __attribute__((always_inline));
inline void m_to_u_mode() __attribute__((always_inline));

#endif //_TRAP_DELEGATE_H_

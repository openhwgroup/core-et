/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PU_SRAM_RW_H_
#define _PU_SRAM_RW_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define CAUSE_USER_ECALL         0x8
#define CAUSE_SUPERVISOR_ECALL   0x9
#define CAUSE_MACHINE_ECALL      0xb

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
//EXTERN_C void mtrap_vector();

inline void m_to_s_mode() __attribute__((always_inline));
inline void m_to_u_mode() __attribute__((always_inline));
inline void to_m_mode() __attribute__((always_inline));

#endif //_PMC_ACCESS_H_

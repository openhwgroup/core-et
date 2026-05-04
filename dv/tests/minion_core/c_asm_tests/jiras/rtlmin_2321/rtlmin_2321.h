/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _RTLMIN_851_H_
#define _RTLMIN_851_H_


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define CAUSE_FETCH_PAGE_FAULT   0xc
#define CAUSE_USER_ECALL         0x8

#define PAGE_FAULT_ADDRESS0 (0x8040000000ULL-2)
#define PAGE_FAULT_ADDRESS1 (0x80C0000000ULL-2)

EXTERN_C void __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler();

void setup_vmem();
void inline to_u_mode();
void inline setup_trap_vector();
void flush_l1();

#endif

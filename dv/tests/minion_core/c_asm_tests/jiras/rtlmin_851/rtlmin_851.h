/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _RTLMIN_851_H_
#define _RTLMIN_851_H_

#define CAUSE_STORE_PF     0xF
#define CAUSE_LOAD_PF      0xd
#define CAUSE_USER_ECALL   0x8

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, /*uint64_t mtval,*/ uint64_t *regs);

EXTERN_C void test_mtrap_vector();

void setup_vmem();

extern int32_t test_offsets[][8];
extern uint8_t test_masks[];
extern uint64_t NR_TESTS;
#endif

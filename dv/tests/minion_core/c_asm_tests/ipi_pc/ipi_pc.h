/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

constexpr uint64_t CAUSE_USER_ECALL = 0x8;

//EXTERN_C void  __attribute__ ((interrupt)) irq_handler();
//EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart);
EXTERN_C void test_mtrap_vector();
uint64_t get_hart_id();
uint64_t get_shire_id();
uint64_t get_minion_id();
uint64_t get_thread_id();

void test0();
void handler0() __attribute__ ((interrupt));
void setup_trap_vector();
void setup_interrupts();
uint64_t get_flb(uint64_t cnt, uint64_t n);
void m_to_u_mode();
void to_m_mode();
void setup_pc_redirect();
void delay(int d);

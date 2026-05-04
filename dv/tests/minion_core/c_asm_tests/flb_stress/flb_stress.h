/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _FLB_STRESS_H_
#define _FLB_STRESS_H_

//#define SHORT_TEST  // if defined, test is will be much shorter
//#define ONLY_CNT_0 // if defined, test will only use flb0 (easier for debugging)


#define NR_THREADS 64
#define NR_FLB 32

#ifndef SHORT_TEST

#define ITERATIONS 15
#define REPETITIONS 8
#define REP_CHECKS 4
#define NR_FLB 32
#define MAX_FLB_CNT 256

#else

#define ITERATIONS 5
#define REPETITIONS 16
#define REP_CHECKS 4
#define MAX_FLB_CNT 0x16

#endif



void set_flb(uint64_t cnt, uint64_t n, bool fence = true);
uint64_t get_flb(uint64_t cnt, uint64_t n);
uint64_t get_hart_id();

uint64_t wait_fcc(uint64_t cnt);
void inc_fcc0(unsigned thread0_mask, unsigned thread1_mask);
uint8_t get_rand_byte(unsigned &ptr);

void set_debug(uint64_t d);
#endif

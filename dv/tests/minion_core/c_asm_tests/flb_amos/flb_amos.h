/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _FLB_AMOS_H_
#define _FLB_AMOS_H_

#define ITERATIONS 100
#define MAX_FLB_CNT 0xff

void test_t0();
void test_t1();
uint64_t peek_fcc(uint64_t cnt);
void inc_fcc0(unsigned thread);
uint8_t get_rand_byte(unsigned &ptr);
void set_flb(uint64_t cnt, uint64_t n);
uint64_t get_flb(uint64_t cnt, uint64_t n);
uint64_t get_hart_id();
void set_debug(uint64_t d);
#endif

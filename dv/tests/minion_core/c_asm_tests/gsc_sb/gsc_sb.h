/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _GSC_SB_H_
#define _GSC_SB_H_

#define ITERATIONS 50
#define REPEATS 10

inline void set_mask(uint8_t m);
inline void delay();
inline void set_debug(uint64_t d);
void set_progress(unsigned p);
void evict_l1();
void evict_va(uintptr_t ad);
uint64_t get_hart_id();

uint64_t peek_fcc(uint64_t cnt);
void inc_fcc0(unsigned thread);

void test_t0();
void test_t1();

#endif

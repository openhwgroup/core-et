/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "slt_barrier.h"

__asm__(".equ CREDINC0,        0x803400C0");
__asm__(".equ CREDINC1,        0x803400C8");
__asm__(".equ CREDINC2,        0x803400D0");
__asm__(".equ CREDINC3,        0x803400D8");
__asm__(".equ SHIRE_COOP_MODE, 0x80740290");
__asm__(".equ BARRIER0,        0x80340100");
__asm__(".equ BARRIER1,        0x80340101");
__asm__(".equ BARRIER2,        0x80340102");
__asm__(".equ BARRIER3,        0x80340103");
__asm__(".equ BARRIER4,        0x80340104");
__asm__(".equ BARRIER5,        0x80340105");
__asm__(".equ BARRIER6,        0x80340106");
__asm__(".equ BARRIER7,        0x80340107");
__asm__(".equ BARRIER8,        0x80340108");
__asm__(".equ BARRIER9,        0x80340109");
__asm__(".equ BARRIER10,       0x8034010A");
__asm__(".equ BARRIER11,       0x8034010B");
__asm__(".equ BARRIER12,       0x8034010C");
__asm__(".equ BARRIER13,       0x8034010D");
__asm__(".equ BARRIER14,       0x8034010E");
__asm__(".equ BARRIER15,       0x8034010F");
__asm__(".equ BARRIER16,       0x80340110");
__asm__(".equ BARRIER17,       0x80340111");
__asm__(".equ BARRIER18,       0x80340112");
__asm__(".equ BARRIER19,       0x80340113");
__asm__(".equ BARRIER20,       0x80340114");
__asm__(".equ BARRIER21,       0x80340115");
__asm__(".equ BARRIER22,       0x80340116");
__asm__(".equ BARRIER23,       0x80340117");
__asm__(".equ BARRIER24,       0x80340118");
__asm__(".equ BARRIER25,       0x80340119");
__asm__(".equ BARRIER26,       0x8034011A");
__asm__(".equ BARRIER27,       0x8034011B");
__asm__(".equ BARRIER28,       0x8034011C");
__asm__(".equ BARRIER29,       0x8034011D");
__asm__(".equ BARRIER30,       0x8034011E");
__asm__(".equ BARRIER31,       0x8034011F");

void c_sync_point(uint64_t sync_odd_harts){
    uint64_t neigh_id = get_neigh_id();
    uint64_t fcc_mask = 0xff << (neigh_id * 8 );

    uint64_t barrier_num   = sync_odd_harts ?  1 : 0;
    uint64_t barrier_count = sync_odd_harts ? 15 : 7;

    uint64_t last = fast_local_barrier(barrier_num, barrier_count);
    if (last) {
       send_even_local_fcc(fcc_mask);
       if (sync_odd_harts) {
          send_odd_local_fcc(fcc_mask);
       }
    }
    wait_fcc();
}

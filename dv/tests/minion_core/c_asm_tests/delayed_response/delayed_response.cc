/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Test: delayed_response
 * Author: Calvin Bulla <calvin.bulla@esperantotech.com>
 *
 * Description:
 *  This test reproduces an issue in cosim, where FLB accesses may be checked out of order.
 *  It uses the validation1 CSR to delay memory accesses via EVL.
 *
 * See also: VERIF-1671, VERIF-1703
 */

#include "et_test_common.h"
#include "macros.h"
#include "minion.h"
#include <cassert>

#define LINE_SIZE 64
#define DWORD_SIZE sizeof(uint64_t)
#define DWORDS_PER_LINE (LINE_SIZE/DWORD_SIZE)

#define ALWAYS_INLINE inline __attribute__((always_inline))
#define LINE_ALIGNED __attribute__((aligned(LINE_SIZE)))

#define ESR_FLB_REG_00 0x80340100ULL

ALWAYS_INLINE void evict_pa(uint64_t paddr) {
    uint64_t set = (paddr >> 6) & 0xFULL;
    evict_sw(0, 0x3, 0, set);
    evict_sw(0, 0x3, 1, set);
    evict_sw(0, 0x3, 2, set);
    evict_sw(0, 0x3, 3, set);
    FENCE;
}

ALWAYS_INLINE void init_barrier(uint64_t barrier, uint64_t value = 0) {
    volatile uint64_t *flb_esr = (uint64_t *) (ESR_FLB_REG_00 + (barrier<<3));
    *flb_esr = value;
    FENCE;
}

ALWAYS_INLINE uint64_t join_barrier(uint64_t barrier, uint64_t players) {
    uint64_t match = players - 1;
    uint64_t csr_enc = ((match   & 0xFF) << 5 ) |
                       ((barrier & 0x1F)      );
    uint64_t last_entry;
    asm volatile("csrrw %[last_entry], 0x820, %[csr_enc]\n"
                 : [last_entry]"=r"(last_entry)
                 : [csr_enc]"r"(csr_enc)
                 :
                );
    return last_entry;
}

ALWAYS_INLINE void atomic_store(uint64_t addr, uint64_t value) {
    asm volatile("amoswapl.d x0, %[value], (%[addr])\n"
                 :
                 : [value]"r"(value),
                   [addr]"r"(addr)
                 :
                );
}

ALWAYS_INLINE uint64_t atomic_load(uint64_t addr) {
    uint64_t result;
    asm volatile("amoorl.d %[result], x0, (%[addr])\n"
                 : [result]"=r"(result)
                 : [addr]"r"(addr)
                 :
                );
    return result;
}

volatile uint64_t shared_data[LINE_SIZE] LINE_ALIGNED = { 0 };

void hart0() {
    static volatile uint64_t local_data[LINE_SIZE] LINE_ALIGNED;
    evict_pa((uint64_t)local_data);
    et_delay_response((uint64_t)local_data, 10);

    evict_pa((uint64_t)shared_data);

    // Setup FLBs
    init_barrier(0);

    // Sync all threads
    give_credit(0, -1, -1, 0);
    wait_for_credit(0);

    // bubble ~10 cycles
    *local_data = 0xab0;
    FENCE;

    atomic_store((uint64_t)shared_data, 1453);
    FENCE;

    uint64_t is_last = join_barrier(0, 2);
    if (is_last) {
        give_credit(0, 1, 0, 0);
    } else {
        wait_for_credit(0);
    }
}

void hart1() {
    static volatile uint64_t local_data[LINE_SIZE] LINE_ALIGNED;
    evict_pa((uint64_t)local_data);
    et_delay_response((uint64_t)local_data, 2001);

    wait_for_credit(0);

    // bubble ~2000 cycles
    *local_data = 0xab1;
    FENCE;
}

void hart2() {
    static volatile uint64_t local_data[LINE_SIZE] LINE_ALIGNED;
    evict_pa((uint64_t)local_data);
    et_delay_response((uint64_t)local_data, 1002);

    evict_pa((uint64_t)shared_data);

    wait_for_credit(0);

    // bubble ~1000 cycles
    *local_data = 0xab2;
    FENCE;

    uint64_t is_last = join_barrier(0, 2);
    if (is_last) {
        give_credit(0, 0, 0, 0);
    } else {
        wait_for_credit(0);
    }

    uint64_t value = shared_data[0];
    if (value != 1453) {
        C_TEST_FAIL;
    }
}

int main() {
    int hart_id = get_hart_id();

    C_TEST_START;

    switch (hart_id) {
        case 0: hart0(); break;
        case 1: hart1(); break;
        case 2: hart2(); break;
        default: break;
    }

    C_TEST_PASS;
    return 0;
}

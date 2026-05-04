/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <array>

#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#include "erbium_clint_msip.h"

// at 10 MHz timer clk, and 1000 MHz cpu clk, 500 inst ~= 5 ticks
// likely 3 instructions generated in a loop => ~750 inst executed
#define WAIT for (int i = 0; i < 10; i++) __asm__ __volatile__ ("nop\n")

uint64_t esr_read(const uint64_t addr) {
    uint64_t rd_val = 0xdead0baddead0bad;
    rd_val = *(volatile uint64_t *)addr;
    return rd_val;
}

void esr_write(const uint64_t addr, const uint64_t val) {
    *(volatile uint64_t *)addr = val;
}

struct regs_t { std::uint64_t addr, def; };

enum {
    n_mtime_local_target,
    n_mipi_trigger, n_mipi_trigger_clear,
    _n_cnt
};


constexpr std::array<regs_t, _n_cnt> make_clint_regs() {
    std::array<regs_t, _n_cnt> a{}; // value-initialize all entries to {0,0}

    a[n_mtime_local_target]   = { 0x80f40218ull, 0xffffull }; // 16 harts
    a[n_mipi_trigger]         = { 0x80f40090, 0x0 };
    a[n_mipi_trigger_clear]   = { 0x80f40098, 0x0 };

    return a;
}

constexpr auto regs = make_clint_regs();

// use mtime local target as sync point for all harts as uncached location
uint64_t get_scratch() { return esr_read(regs[n_mtime_local_target].addr); }
void set_scratch(const uint64_t val) {
    esr_write(regs[n_mtime_local_target].addr, val);
}

constexpr uint64_t cmp_inc = 20;
int main() {
    C_TEST_START;

    // configure trap vector
    // and enable machine software interrupts (mie.MSIE)
    __asm__ __volatile__
    (
        "la t0, mtrap_vector_v\n"
        "ori t0, t0, 1\n"
        "csrw mtvec, t0\n"
        "li t0, 0x8\n"
        "csrw mie, t0\n"
        "csrsi mstatus, 0x08\n"
        : : : "t0"
    );

    // first hart sets itself up for timer interrupts
    uint64_t hid = get_shire_hart_id();

    if (hid == 0) {
        set_scratch(0);
        WAIT;
        esr_write(regs[n_mipi_trigger].addr, 1 << (hid + 1)); // ipi next hart
        __asm__ __volatile__ ("wfi\n");
        if (get_scratch() != 16) C_TEST_FAIL
    } else {
        // put all other harts to sleep
        __asm__ __volatile__ ("wfi\n");
    }

    C_TEST_PASS;
    return 0;
}

void irq_handler() {
    // clear interrupt
    uint64_t hid = get_shire_hart_id();
    esr_write(regs[n_mipi_trigger_clear].addr, (1 << hid));
    WAIT; // scratch update needs time to go through APB and back from prev. ipi
    set_scratch(get_scratch() + 1);
    if (hid > 0) {
        // ipi next hart, hart 0 is last, no further ipis
        esr_write(regs[n_mipi_trigger].addr, 1 << ((hid + 1) & 0xf));
    }
}

void bad_trap() {
    C_TEST_FAIL;
}

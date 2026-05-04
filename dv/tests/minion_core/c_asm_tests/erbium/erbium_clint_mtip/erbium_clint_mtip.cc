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

#include "erbium_clint_mtip.h"

uint64_t esr_read(const uint64_t addr) {
    uint64_t rd_val = 0xdead0baddead0bad;
    rd_val = *(volatile uint64_t *)addr;
    return rd_val;
}

void esr_write(const uint64_t addr, const uint64_t val) {
    *(volatile uint64_t *)addr = val;
}

struct regs_t { std::uint64_t addr, def; };

enum { n_mtime, n_mtimecmp, n_mtime_local_target, _n_cnt };

constexpr std::array<regs_t, _n_cnt> make_clint_regs() {
    std::array<regs_t, _n_cnt> a{}; // value-initialize all entries to {0,0}

    a[n_mtime]                = { 0x80f40200ull, 0x0ull };
    a[n_mtimecmp]             = { 0x80f40208ull, 0xffff'ffff'ffff'ffffull };
    a[n_mtime_local_target]   = { 0x80f40218ull, 0xffffull }; // 16 harts

    return a;
}

constexpr auto regs = make_clint_regs();

constexpr uint64_t cmp_inc = 20;
int main() {
    C_TEST_START;

    // configure trap vector
    // and enable machine timer interrupts (mie.MTIE)
    __asm__ __volatile__
    (
        "la t0, mtrap_vector_v\n"
        "ori t0, t0, 1\n"
        "csrw mtvec, t0\n"
        "li t0, 0x80\n"
        "csrw mie, t0\n"
        "csrsi mstatus, 0x08\n"
        : : : "t0"
    );

    // first hart sets itself up for timer interrupts
    uint64_t hid = get_shire_hart_id();
    if (hid == 0) {
        esr_write(regs[n_mtime_local_target].addr, 0x1);
        esr_write(regs[n_mtimecmp].addr, cmp_inc);
    }
    // put all harts to sleep
    __asm__ __volatile__ ("wfi\n");

    if (hid != 0) C_TEST_PASS

    // -------------------------------------------------------------------------
    // catch timeout with hart 0
    while (esr_read(regs[n_mtime].addr) < ((cmp_inc*16) + (cmp_inc >> 1))) {
        __asm__ __volatile__ ("nop\n");
    }
    // shift will push all of the bits out
    if (esr_read(regs[n_mtime_local_target].addr) != 0) C_TEST_FAIL
    esr_write(regs[n_mtimecmp].addr, esr_read(regs[n_mtime].addr) + cmp_inc);

    // -------------------------------------------------------------------------
    // test if writing mtimecmp value less than mtime generates interrupt
    esr_write(regs[n_mtime_local_target].addr, 0x1);
    esr_write(regs[n_mtimecmp].addr, 20);
    // interrupt expected right away, too late to go to wfi, nop a bit instead
    for (int i = 0; i < 20; i++) __asm__ __volatile__ ("nop\n");
    if (esr_read(regs[n_mtime_local_target].addr) != 0x2) C_TEST_FAIL

    C_TEST_PASS;
    return 0;
}

void irq_handler() {
    // clear interrupt
    esr_write(regs[n_mtimecmp].addr, esr_read(regs[n_mtime].addr) + cmp_inc);
    // and set mask for the next hart
    esr_write(
        regs[n_mtime_local_target].addr,
        (esr_read(regs[n_mtime_local_target].addr) << 1)
    );
}

void bad_trap() {
    C_TEST_FAIL;
}

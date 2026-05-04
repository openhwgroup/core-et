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

#define C_TEST_FAIL_ID(x) \
    { \
        __asm__ __volatile__ ("csrwi validation3,"#x"\n"); \
        C_TEST_FAIL; \
    }

// at 10 MHz timer clk, and 1000 MHz cpu clk, 500 inst ~= 5 ticks
// likely 3 instructions generated in a loop => ~750 inst executed
#define WAIT for (int i = 0; i < 250; i++) __asm__ __volatile__ ("nop\n")

uint64_t esr_read(const uint64_t addr) {
    uint64_t rd_val = 0xdead0baddead0bad;
    rd_val = *(volatile uint64_t *)addr;
    return rd_val;
}

void esr_write(const uint64_t addr, const uint64_t val) {
    *(volatile uint64_t *)addr = val;
}

void check_esr_read(const uint64_t addr, const uint64_t val) {
    if (esr_read(addr) != val) C_TEST_FAIL;
}

void check_esr_read(
    const uint64_t addr, const uint64_t val, const uint64_t thr)
{
    if (esr_read(addr) >= (val + thr)) C_TEST_FAIL;
}

void check_esr_write(const uint64_t addr, const uint64_t val) {
    esr_write(addr, val);
    check_esr_read(addr, val);
}

void check_esr_write(
    const uint64_t addr, const uint64_t val, const uint64_t thr)
{
    esr_write(addr, val);
    check_esr_read(addr, val, thr);
}

struct regs_t { std::uint64_t addr, def; };

enum {
    n_mtime, n_mtimecmp, n_time_config, n_mtime_local_target,
    n_mipi_trigger, n_mipi_trigger_clear,
    n_ipi_redirect_trigger, n_ipi_redirect_filter, n_ipi_redirect_pc,
    _n_cnt
};

constexpr std::array<regs_t, _n_cnt> make_clint_regs() {
    std::array<regs_t, _n_cnt> a{}; // value-initialize all entries to {0,0}

    a[n_mtime]                = { 0x80f40200ull, 0x0ull };
    a[n_mtimecmp]             = { 0x80f40208ull, 0xffff'ffff'ffff'ffffull };
    a[n_time_config]          = { 0x80f40210ull, 0x14ull };
    a[n_mtime_local_target]   = { 0x80f40218ull, 0xffffull }; // 16 harts
    a[n_mipi_trigger]         = { 0x80f40090ull, 0x0 };
    a[n_mipi_trigger_clear]   = { 0x80f40098ull, 0x0 };
    a[n_ipi_redirect_trigger] = { 0x80340080ull, 0x0 };
    a[n_ipi_redirect_filter]  = { 0x80f40088ull, 0xffffull }; // 16 harts
    a[n_ipi_redirect_pc]      = { 0x80100040ull, 0x0 };

    return a;
}

constexpr auto regs = make_clint_regs();

int main() {
    C_TEST_START;

    setup_mtrap_vector(); // no interrupts, only read/write to CLINT regs

    uint64_t hid = get_shire_hart_id();
    if (hid != 0) C_TEST_PASS; // hart 0 will carry out all testing

    // -------------------------------------------------------------------------
    // check that ESR exists and reset values are as expected

    // timer
    check_esr_read(regs[n_mtime].addr, regs[n_mtime].def, 50);
    check_esr_read(regs[n_mtimecmp].addr, regs[n_mtimecmp].def);
    check_esr_read(regs[n_time_config].addr, regs[n_time_config].def);
    check_esr_read(regs[n_mtime_local_target].addr, regs[n_mtime_local_target].def);

    // mipi
    check_esr_read(regs[n_mipi_trigger].addr, regs[n_mipi_trigger].def);
    check_esr_read(regs[n_mipi_trigger_clear].addr, regs[n_mipi_trigger_clear].def);

    // ipi redirect
    check_esr_read(regs[n_ipi_redirect_trigger].addr, regs[n_ipi_redirect_trigger].def);
    check_esr_read(regs[n_ipi_redirect_filter].addr, regs[n_ipi_redirect_filter].def);
    check_esr_read(regs[n_ipi_redirect_pc].addr, regs[n_ipi_redirect_pc].def);

    // -------------------------------------------------------------------------
    // check read/write

    // timer
    check_esr_write(regs[n_mtime].addr, 0, 2);
    check_esr_write(regs[n_mtimecmp].addr, 2000);
    check_esr_write(regs[n_time_config].addr, 0x10);
    check_esr_write(regs[n_mtime_local_target].addr, 0xff);

    // mipi
    check_esr_write(regs[n_mipi_trigger].addr, 0x8);
    esr_write(regs[n_mipi_trigger_clear].addr, 0x8);
    check_esr_read(regs[n_mipi_trigger].addr, 0x0); // should lower pending interrupt
    check_esr_read(regs[n_mipi_trigger_clear].addr, regs[n_mipi_trigger_clear].def); // 'clear' reg is WAR0

    // ipi redirect
    esr_write(regs[n_ipi_redirect_trigger].addr, 0x8); // filter is disabled, so no redirection will happen
    check_esr_read(regs[n_ipi_redirect_trigger].addr, regs[n_ipi_redirect_trigger].def); // reg is WAR0
    check_esr_write(regs[n_ipi_redirect_filter].addr, 0x10);
    check_esr_write(regs[n_ipi_redirect_pc].addr, 0x40002010); // made up pc

    // -------------------------------------------------------------------------
    // explicit checks on illegal/WAR0 writes

    // timer
    esr_write(regs[n_time_config].addr, 0xffff'ffff'ffff'ff15); // [63:8] WAR0
    check_esr_read(regs[n_time_config].addr, 0x15);
    esr_write(regs[n_mtime_local_target].addr, 0xffff'ffff'ffff'0100); // [63:16] WAR0
    check_esr_read(regs[n_mtime_local_target].addr, 0x100);

    // mipi
    esr_write(regs[n_mipi_trigger].addr, 0xffff'ffff'ffff'0000); // [63:16] WAR0
    check_esr_read(regs[n_mipi_trigger].addr, 0x0);
    esr_write(regs[n_mipi_trigger_clear].addr, 0xffff'ffff'ffff'ffff); // WAR0 reg
    check_esr_read(regs[n_mipi_trigger_clear].addr, 0x0);

    // ipi redirect
    esr_write(regs[n_ipi_redirect_trigger].addr, 0xffff'ffff'ffff'0200); // [63:16] WAR0
    check_esr_read(regs[n_ipi_redirect_trigger].addr, 0x0); // read is always 0
    esr_write(regs[n_ipi_redirect_filter].addr, 0xffff'ffff'ffff'0010); // [63:16] WAR0
    check_esr_read(regs[n_ipi_redirect_filter].addr, 0x0010);
    esr_write(regs[n_ipi_redirect_pc].addr, 0xffff'0000'0000'8001); // [63:48] & [0] WAR0
    check_esr_read(regs[n_ipi_redirect_pc].addr, 0x8000);

    // -------------------------------------------------------------------------
    // check that mtime actually increments
    uint64_t mtime_start, mtime_diff, mtime_diff_t;

    mtime_start = esr_read(regs[n_mtime].addr);
    WAIT;
    if ((esr_read(regs[n_mtime].addr) - mtime_start) < 5) C_TEST_FAIL_ID(1);

    // -------------------------------------------------------------------------
    // test if changing clock source works and is (possibly) faster
    esr_write(regs[n_mtimecmp].addr, regs[n_mtimecmp].def); // disable int.

    // -- slow
    mtime_start = esr_read(regs[n_mtime].addr);
    WAIT;
    mtime_diff = (esr_read(regs[n_mtime].addr) - mtime_start);

    // -- fast
    esr_write(regs[n_time_config].addr, ((1 << 7) | regs[n_time_config].def));
    mtime_start = esr_read(regs[n_mtime].addr);
    WAIT;
    mtime_diff_t = (esr_read(regs[n_mtime].addr) - mtime_start);

    // should be ~5x larger in RTL (depending on clk sources)
    if (!((mtime_diff_t + 5) > (mtime_diff * 5))) C_TEST_FAIL_ID(2);
    // but the same in emulation
    //if (!(mtime_diff_t >= mtime_diff)) C_TEST_FAIL_ID(2);

    // -------------------------------------------------------------------------
    // test that the prescaler works
    esr_write(regs[n_time_config].addr, (regs[n_time_config].def << 1));

    mtime_start = esr_read(regs[n_mtime].addr);
    WAIT;
    mtime_diff_t = (esr_read(regs[n_mtime].addr) - mtime_start);
    // should be ~2x slower in RTL compared to default threshold
    if (!((mtime_diff + 5) > (mtime_diff_t * 2))) C_TEST_FAIL_ID(3);

    C_TEST_PASS;
    return 0;
}

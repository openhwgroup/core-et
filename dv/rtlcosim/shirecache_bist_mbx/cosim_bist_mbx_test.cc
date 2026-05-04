// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bist_mbx vs shirecache_bist_mbx

#include "Vcosim_bist_mbx_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_bist_mbx_tb;

// Parameters matching the TB
static constexpr int DATA_SIZE = 16;
static constexpr int ADDR_SIZE = 4;
static constexpr int RAM_DELAY = 2;
static constexpr int NUM_ADDRS = 1 << ADDR_SIZE;  // 16

// ── Helpers ──────────────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->mbx_impl_i   = 1;
    d->mbist_sel_i   = 0;
    d->mbist_rd_en_i = 0;
    d->mbist_wr_en_i = 0;
    d->mbist_addr_i  = 0;
    d->mbist_wdata_i = 0;
    d->mbx_rdata_i   = 0;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("mbist_rdata",
                (uint32_t)d->orig_mbist_rdata_o,
                (uint32_t)d->new_mbist_rdata_o);
    sim.compare("mbx_sel",
                (uint32_t)d->orig_mbx_sel_o,
                (uint32_t)d->new_mbx_sel_o);
    sim.compare("mbx_rd_en",
                (uint32_t)d->orig_mbx_rd_en_o,
                (uint32_t)d->new_mbx_rd_en_o);
    sim.compare("mbx_wr_en",
                (uint32_t)d->orig_mbx_wr_en_o,
                (uint32_t)d->new_mbx_wr_en_o);
    sim.compare("mbx_addr",
                (uint32_t)d->orig_mbx_addr_o,
                (uint32_t)d->new_mbx_addr_o);
    sim.compare("mbx_wdata",
                (uint32_t)d->orig_mbx_wdata_o,
                (uint32_t)d->new_mbx_wdata_o);
}

// ── Main ─────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Reset + idle ────────────────────────────────────
    printf("phase 1: reset + idle\n");
    clear_inputs(d);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 2: Single read access (impl=1) ─────────────────────
    printf("phase 2: single read access (impl=1)\n");
    clear_inputs(d);
    d->mbx_impl_i   = 1;
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x5;
    d->mbx_rdata_i   = 0xBEEF;
    sim.tick();
    compare_all(sim);

    // After access: rd_en should be blocked for RAM_DELAY-1 cycles
    // Keep sel asserted with rd_en to test blocking
    for (int i = 0; i < RAM_DELAY + 1; i++) {
        d->mbx_rdata_i = 0xCAFE;
        sim.tick();
        compare_all(sim);
    }

    // Deassert, let blocking drain
    clear_inputs(d);
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 3: Single write access ─────────────────────────────
    printf("phase 3: single write access\n");
    clear_inputs(d);
    d->mbx_impl_i   = 1;
    d->mbist_sel_i   = 1;
    d->mbist_wr_en_i = 1;
    d->mbist_addr_i  = 0xA;
    d->mbist_wdata_i = 0xDEAD;
    sim.tick();
    compare_all(sim);

    // Keep asserting to test blocking
    for (int i = 0; i < RAM_DELAY + 1; i++) {
        sim.tick();
        compare_all(sim);
    }

    clear_inputs(d);
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 4: Burst access (back-to-back reads) ───────────────
    printf("phase 4: burst access (back-to-back reads)\n");
    for (int addr = 0; addr < NUM_ADDRS; addr++) {
        clear_inputs(d);
        d->mbx_impl_i   = 1;
        d->mbist_sel_i   = 1;
        d->mbist_rd_en_i = 1;
        d->mbist_addr_i  = addr;
        d->mbx_rdata_i   = (addr * 0x1111) & 0xFFFF;
        sim.tick();
        compare_all(sim);
    }
    // Drain
    clear_inputs(d);
    for (int i = 0; i < RAM_DELAY + 2; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 5: Burst access (back-to-back writes) ──────────────
    printf("phase 5: burst access (back-to-back writes)\n");
    for (int addr = 0; addr < NUM_ADDRS; addr++) {
        clear_inputs(d);
        d->mbx_impl_i   = 1;
        d->mbist_sel_i   = 1;
        d->mbist_wr_en_i = 1;
        d->mbist_addr_i  = addr;
        d->mbist_wdata_i = (addr * 0x2222) & 0xFFFF;
        sim.tick();
        compare_all(sim);
    }
    // Drain
    clear_inputs(d);
    for (int i = 0; i < RAM_DELAY + 2; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 6: Alternating read/write ──────────────────────────
    printf("phase 6: alternating read/write\n");
    for (int i = 0; i < 8; i++) {
        clear_inputs(d);
        d->mbx_impl_i = 1;
        d->mbist_sel_i = 1;
        if (i & 1) {
            d->mbist_wr_en_i = 1;
            d->mbist_wdata_i = (i * 0x3333) & 0xFFFF;
        } else {
            d->mbist_rd_en_i = 1;
            d->mbx_rdata_i   = (i * 0x4444) & 0xFFFF;
        }
        d->mbist_addr_i = i & 0xF;
        sim.tick();
        compare_all(sim);
    }
    // Drain
    clear_inputs(d);
    for (int i = 0; i < RAM_DELAY + 2; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 7: Blocking pattern — hold sel+rd_en continuously ──
    printf("phase 7: blocking pattern (continuous sel+rd_en)\n");
    clear_inputs(d);
    d->mbx_impl_i = 1;
    for (int i = 0; i < 20; i++) {
        d->mbist_sel_i   = 1;
        d->mbist_rd_en_i = 1;
        d->mbist_addr_i  = i & 0xF;
        d->mbx_rdata_i   = (i * 0x5555) & 0xFFFF;
        sim.tick();
        compare_all(sim);
    }
    clear_inputs(d);
    for (int i = 0; i < RAM_DELAY + 2; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 8: impl=0 (mbist_rdata should be 0) ───────────────
    printf("phase 8: impl=0 (rdata should be zero)\n");
    clear_inputs(d);
    d->mbx_impl_i   = 0;
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x3;
    d->mbx_rdata_i   = 0xFFFF;
    sim.tick();
    compare_all(sim);

    // Write with impl=0
    d->mbist_rd_en_i = 0;
    d->mbist_wr_en_i = 1;
    d->mbist_wdata_i = 0x1234;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    d->mbx_impl_i = 0;
    for (int i = 0; i < RAM_DELAY + 2; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 9: sel=0 (no access, outputs should be zero/pass) ─
    printf("phase 9: sel=0 (no access)\n");
    clear_inputs(d);
    d->mbx_impl_i   = 1;
    d->mbist_sel_i   = 0;
    d->mbist_rd_en_i = 1;  // rd_en without sel
    d->mbist_addr_i  = 0x7;
    d->mbx_rdata_i   = 0xAAAA;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 10: Random stimulus ────────────────────────────────
    printf("phase 10: random stimulus (200 cycles)\n");
    srand(42);
    for (int cycle = 0; cycle < 200; cycle++) {
        d->mbx_impl_i   = (rand() & 0x7) ? 1 : 0;  // mostly impl=1
        d->mbist_sel_i   = (rand() & 0x3) ? 1 : 0;  // mostly sel=1
        d->mbist_rd_en_i = rand() & 1;
        d->mbist_wr_en_i = rand() & 1;
        d->mbist_addr_i  = rand() & 0xF;
        d->mbist_wdata_i = rand() & 0xFFFF;
        d->mbx_rdata_i   = rand() & 0xFFFF;
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 11: Drain and check final idle ─────────────────────
    printf("phase 11: drain and final idle\n");
    clear_inputs(d);
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

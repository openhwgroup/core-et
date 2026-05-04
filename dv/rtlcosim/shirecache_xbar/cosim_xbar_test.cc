// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_xbar vs shirecache_xbar — BOTH configurations
// Config A: SrcArb=all-ones (source-side arb, multi-hot), AddOutputRelay=2'b10
// Config B: SrcArb=0 (xbar-side arb, one-hot), AddOutputRelay=0
// TB params: NumSrc=3, NumDest=2, DataWidth=16, FifoDepth=3

#include "Vcosim_xbar_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_xbar_tb;
static constexpr int NS = 3, ND = 2, DW = 16;

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    // Config A (SrcArb=1, OutputRelay on dest1)
    sim.compare("A:src_ready",     d->a_orig_src_ready_o,     d->a_new_src_ready_o);
    sim.compare("A:src_2_credits", d->a_orig_src_2_credits_o, d->a_new_src_2_credits_o);
    sim.compare("A:dest_valid",    d->a_orig_dest_valid_o,    d->a_new_dest_valid_o);
    sim.compare("A:dest_data",     d->a_orig_dest_data_o,     d->a_new_dest_data_o);
    // Config B (SrcArb=0, no OutputRelay)
    sim.compare("B:src_ready",     d->b_orig_src_ready_o,     d->b_new_src_ready_o);
    sim.compare("B:src_2_credits", d->b_orig_src_2_credits_o, d->b_new_src_2_credits_o);
    sim.compare("B:dest_valid",    d->b_orig_dest_valid_o,    d->b_new_dest_valid_o);
    sim.compare("B:dest_data",     d->b_orig_dest_data_o,     d->b_new_dest_data_o);
    // Config C (SrcArb=1, NumSrcMask=3'b101 — src1 disabled)
    sim.compare("C:src_ready",     d->c_orig_src_ready_o,     d->c_new_src_ready_o);
    sim.compare("C:src_2_credits", d->c_orig_src_2_credits_o, d->c_new_src_2_credits_o);
    sim.compare("C:dest_valid",    d->c_orig_dest_valid_o,    d->c_new_dest_valid_o);
    sim.compare("C:dest_data",     d->c_orig_dest_data_o,     d->c_new_dest_data_o);
}

static void tick_cmp(CosimCtrl<DUT>& sim) { sim.tick(); compare_all(sim); }

static void set_src(DUT* d, int src, uint8_t dest_mask, uint16_t data) {
    uint8_t v = d->src_valid_i;
    v &= ~(((1<<ND)-1) << (src*ND));
    v |= (dest_mask & ((1<<ND)-1)) << (src*ND);
    d->src_valid_i = v;
    uint64_t dv = d->src_data_i;
    dv &= ~(((uint64_t)(1<<DW)-1) << (src*DW));
    dv |= ((uint64_t)data) << (src*DW);
    d->src_data_i = dv;
}

static void clear(DUT* d) { d->src_valid_i = 0; d->src_data_i = 0; d->dest_ready_i = 0; }

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset(5);
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ── Phase 1: single src→dest0 ────────────────────────
    printf("phase 1: src0→dest0\n");
    set_src(d, 0, 0x1, 0xA001);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 2: src→dest1 ───────────────────────────────
    printf("phase 2: src1→dest1\n");
    set_src(d, 1, 0x2, 0xB002);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 3: src2→dest0 ──────────────────────────────
    printf("phase 3: src2→dest0\n");
    set_src(d, 2, 0x1, 0xB003);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 4: broadcast (multi-hot valid) ─────────────
    printf("phase 4: broadcast src0→both dests\n");
    set_src(d, 0, 0x3, 0xC003);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 5: all src→dest0 (contention) ──────────────
    printf("phase 5: all src→dest0\n");
    for (int s = 0; s < NS; s++) set_src(d, s, 0x1, 0xD000 + s);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 6: all src→dest1 ───────────────────────────
    printf("phase 6: all src→dest1\n");
    for (int s = 0; s < NS; s++) set_src(d, s, 0x2, 0xE000 + s);
    tick_cmp(sim); clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 7: backpressure dest0 ──────────────────────
    printf("phase 7: backpressure dest0\n");
    d->dest_ready_i = 0x2;
    set_src(d, 0, 0x1, 0xF001);
    tick_cmp(sim); clear(d);
    for (int i = 0; i < 5; i++) { d->dest_ready_i = 0x2; tick_cmp(sim); }
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 8: backpressure dest1 ──────────────────────
    printf("phase 8: backpressure dest1\n");
    d->dest_ready_i = 0x1;
    set_src(d, 2, 0x2, 0xF002);
    tick_cmp(sim); clear(d);
    for (int i = 0; i < 5; i++) { d->dest_ready_i = 0x1; tick_cmp(sim); }
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 9: backpressure both ───────────────────────
    printf("phase 9: backpressure both\n");
    d->dest_ready_i = 0;
    set_src(d, 0, 0x1, 0xF010);
    set_src(d, 1, 0x2, 0xF020);
    tick_cmp(sim); clear(d);
    for (int i = 0; i < 5; i++) { d->dest_ready_i = 0; tick_cmp(sim); }
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 10: fill FIFOs (depth=3) ───────────────────
    printf("phase 10: fill FIFOs\n");
    d->dest_ready_i = 0;
    for (int i = 0; i < 6; i++) {
        set_src(d, 0, 0x1, 0x1000 + i);
        tick_cmp(sim); clear(d);
    }
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 11: credit check ───────────────────────────
    printf("phase 11: credit tracking\n");
    d->dest_ready_i = 0;
    for (int i = 0; i < 3; i++) {
        set_src(d, 1, 0x2, 0x2000 + i);
        tick_cmp(sim); clear(d);
    }
    for (int i = 0; i < 3; i++) tick_cmp(sim);
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 12: all src×dest combos ────────────────────
    printf("phase 12: all combos\n");
    for (int s = 0; s < NS; s++) {
        for (int dd = 0; dd < ND; dd++) {
            clear(d); d->dest_ready_i = 0x3;
            set_src(d, s, 1 << dd, 0x3000 + s*ND + dd);
            tick_cmp(sim);
        }
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 13: rapid alternating ──────────────────────
    printf("phase 13: rapid alternating\n");
    for (int i = 0; i < 20; i++) {
        clear(d); d->dest_ready_i = 0x3;
        set_src(d, i % NS, 1 << (i % ND), 0x4000 + i);
        tick_cmp(sim);
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 14: burst broadcasts ───────────────────────
    printf("phase 14: burst broadcasts\n");
    for (int i = 0; i < 8; i++) {
        clear(d); d->dest_ready_i = 0x3;
        set_src(d, i % NS, 0x3, 0x5000 + i);
        tick_cmp(sim);
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 15: sustained multi-source contention ──────
    printf("phase 15: sustained contention\n");
    for (int i = 0; i < 15; i++) {
        for (int s = 0; s < NS; s++) set_src(d, s, 0x1, 0x6000 + i*NS + s);
        d->dest_ready_i = 0x3;
        tick_cmp(sim);
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 16: alternating backpressure ────────────────
    printf("phase 16: alternating backpressure\n");
    for (int i = 0; i < 20; i++) {
        set_src(d, i % NS, 1 << (i % ND), 0x7000 + i);
        d->dest_ready_i = (i & 1) ? 0x3 : 0x0;
        tick_cmp(sim);
        clear(d);
    }
    d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 17: mixed broadcast + unicast ──────────────
    printf("phase 17: mixed broadcast + unicast\n");
    for (int i = 0; i < 10; i++) {
        clear(d); d->dest_ready_i = 0x3;
        set_src(d, 0, (i & 1) ? 0x3 : 0x1, 0x8000 + i); // alternate broadcast/unicast
        set_src(d, 1, 0x2, 0x8100 + i); // always dest1
        tick_cmp(sim);
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 18: random (500 cycles) ────────────────────
    printf("phase 18: random (500 cycles)\n");
    uint32_t seed = 0xDEAD1234;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        d->src_valid_i = seed & ((1 << (NS*ND)) - 1);
        d->src_data_i = ((uint64_t)(seed & 0xFFFF))
                      | ((uint64_t)((seed>>16)&0xFFFF) << DW)
                      | ((uint64_t)((seed>>8)&0xFFFF) << (2*DW));
        d->dest_ready_i = (seed >> 24) & ((1 << ND) - 1);
        tick_cmp(sim);
    }
    clear(d); d->dest_ready_i = 0x3;
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    return sim.finish();
}

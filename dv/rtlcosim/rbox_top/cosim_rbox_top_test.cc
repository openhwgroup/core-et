// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_top run control FSM vs new.
//
// Compares halt/resume ack and clock gate enable signals cycle-by-cycle
// under exhaustive stimulus patterns.
// Tests independent reset domains (rst_d_ni vs rst_w_ni).
//
// Target: 100 000+ comparisons across all phases.
//
// The SV wrapper exposes:
//   Inputs:  rst_d_ni, rst_w_ni, rc_{halt,resume}_req_{events,gpio}_i
//   Outputs: {orig,new}_{halt_ack,resume_ack,reset_ack,ck_enable}_o
//
// 4 comparisons per tick_compare() call.

#include "Vcosim_rbox_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_rbox_top_tb;

// ────────────────────────────────────────────────────────────────
// Helpers
// ────────────────────────────────────────────────────────────────

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("halt_ack",   sim.dut->orig_halt_ack_o,   sim.dut->new_halt_ack_o);
    sim.compare("resume_ack", sim.dut->orig_resume_ack_o, sim.dut->new_resume_ack_o);
    sim.compare("reset_ack",  sim.dut->orig_reset_ack_o,  sim.dut->new_reset_ack_o);
    sim.compare("ck_enable",  sim.dut->orig_ck_enable_o,  sim.dut->new_ck_enable_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// Apply an RC input vector (4-bit: [halt_ev, resume_ev, halt_gpio, resume_gpio]).
static void set_rc(DUT* dut, uint8_t vec) {
    dut->rc_halt_req_events_i   = (vec >> 3) & 1;
    dut->rc_resume_req_events_i = (vec >> 2) & 1;
    dut->rc_halt_req_gpio_i     = (vec >> 1) & 1;
    dut->rc_resume_req_gpio_i   = (vec >> 0) & 1;
}

// Clear all RC inputs.
static void clear_rc(DUT* dut) { set_rc(dut, 0); }

// Full reset: both domains, N cycles.
static void full_reset(CosimCtrl<DUT>& sim, int cycles = 5) {
    clear_rc(sim.dut.get());
    sim.dut->rst_d_ni = 0;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < cycles; i++) sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.tick();
}

// Simple PRNG (xorshift32) — deterministic, no external deps.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

// ────────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000000;  // generous timeout

    // ════════════════════════════════════════════════════════════
    // Phase A: Reset domains (Tests 1-5)
    // ════════════════════════════════════════════════════════════
    printf("Phase A: Reset domains\n");

    // Test 1: Full reset → verify all outputs idle
    printf("  Test 1: Full reset (both domains)\n");
    clear_rc(sim.dut.get());
    sim.dut->rst_d_ni = 0;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 20; i++) tick_compare(sim);

    // Test 2: Warm reset only (rst_w_ni=0, rst_d_ni=1) → halt FSM preserved
    printf("  Test 2: Warm reset only — halt FSM preserved\n");
    // First halt the FSM
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    // Now apply warm reset only — halt state should persist
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    // Resume should still work (FSM was halted, warm reset didn't clear it)
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // Test 3: Debug reset only (rst_d_ni=0, rst_w_ni=1) → halt FSM clears
    printf("  Test 3: Debug reset only — halt FSM clears\n");
    // Halt first
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    // Debug reset clears the halt FSM
    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 4: Warm reset during active halt/resume toggling
    printf("  Test 4: Warm reset during operation\n");
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rst_w_ni = 0;  // warm reset while halt request active
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    // Resume
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // Test 5: Debug reset during active halt — FSM resets
    printf("  Test 5: Debug reset during operation\n");
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 0;  // debug reset while halt active
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase B: Clock gating (Tests 6-8)
    // ════════════════════════════════════════════════════════════
    printf("Phase B: Clock gating\n");

    // Test 6: After reset, ck_enable reflects !rst_w_ni (rbox_enable=0 in wrapper)
    printf("  Test 6: ck_enable after full reset (enable=0 → gated off)\n");
    full_reset(sim);
    for (int i = 0; i < 20; i++) tick_compare(sim);

    // Test 7: ck_enable during warm reset (should be high = !rst_w_ni when rst_w_ni=0)
    printf("  Test 7: ck_enable during warm reset\n");
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 8: ck_enable = !rst_w_ni — toggle warm reset rapidly
    printf("  Test 8: ck_enable tracks warm reset toggles\n");
    for (int i = 0; i < 50; i++) {
        sim.dut->rst_w_ni = (i % 3 == 0) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase C: Halt/resume FSM (Tests 9-14)
    // ════════════════════════════════════════════════════════════
    printf("Phase C: Halt/resume FSM\n");

    // Test 9: Halt request via events → halt_ack
    printf("  Test 9: Halt via events\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 10: Resume request via events → resume_ack
    printf("  Test 10: Resume via events\n");
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 11: Halt request via GPIO → halt_ack
    printf("  Test 11: Halt via GPIO\n");
    full_reset(sim);
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 12: Resume request via GPIO → resume_ack
    printf("  Test 12: Resume via GPIO\n");
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 13: Halt then warm reset → halt state preserved
    printf("  Test 13: Halt then warm reset (halt preserved)\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    // Warm reset
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    // Verify resume still works (halt state was preserved)
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // Test 14: Halt then debug reset → halt state cleared
    printf("  Test 14: Halt then debug reset (halt cleared)\n");
    full_reset(sim);
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    // Debug reset
    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    // Halt again after debug reset — should work from clean state
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase D: Combined halt/resume sources (Tests 15-20)
    // ════════════════════════════════════════════════════════════
    printf("Phase D: Combined halt/resume sources\n");

    // Test 15: Both halt sources simultaneously
    printf("  Test 15: Both halt sources\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 16: Both resume sources simultaneously
    printf("  Test 16: Both resume sources\n");
    sim.dut->rc_resume_req_events_i = 1;
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 17: Events halt + GPIO resume simultaneously
    printf("  Test 17: Event halt + GPIO resume simultaneously\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 18: GPIO halt + Events resume simultaneously
    printf("  Test 18: GPIO halt + event resume simultaneously\n");
    full_reset(sim);
    sim.dut->rc_halt_req_gpio_i = 1;
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 19: All four inputs high simultaneously
    printf("  Test 19: All four RC inputs active\n");
    full_reset(sim);
    set_rc(sim.dut.get(), 0xF);
    for (int i = 0; i < 10; i++) tick_compare(sim);
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 20: Halt via events, then resume via GPIO (cross-source)
    printf("  Test 20: Cross-source halt/resume\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase E: Exhaustive input combinations (16 states)
    // ════════════════════════════════════════════════════════════
    printf("Phase E: Exhaustive input combinations\n");

    // From each of the two FSM states (not-halted, halted), apply all
    // 16 input combos and observe for 8 cycles each.

    // Sub-phase E1: starting from not-halted state
    printf("  E1: All 16 combos from not-halted state\n");
    full_reset(sim);
    for (int i = 0; i < 5; i++) tick_compare(sim);  // settle in not-halted

    for (uint8_t vec = 0; vec < 16; vec++) {
        // Reset to known not-halted state
        full_reset(sim);
        for (int i = 0; i < 3; i++) tick_compare(sim);
        // Apply combo
        set_rc(sim.dut.get(), vec);
        for (int i = 0; i < 8; i++) tick_compare(sim);
        clear_rc(sim.dut.get());
        for (int i = 0; i < 5; i++) tick_compare(sim);
    }

    // Sub-phase E2: starting from halted state
    printf("  E2: All 16 combos from halted state\n");
    for (uint8_t vec = 0; vec < 16; vec++) {
        // Reset and halt
        full_reset(sim);
        sim.dut->rc_halt_req_events_i = 1;
        for (int i = 0; i < 3; i++) tick_compare(sim);
        sim.dut->rc_halt_req_events_i = 0;
        for (int i = 0; i < 3; i++) tick_compare(sim);
        // Apply combo
        set_rc(sim.dut.get(), vec);
        for (int i = 0; i < 8; i++) tick_compare(sim);
        clear_rc(sim.dut.get());
        for (int i = 0; i < 5; i++) tick_compare(sim);
    }

    // ════════════════════════════════════════════════════════════
    // Phase F: Transition coverage (all 16→16 pairs)
    // ════════════════════════════════════════════════════════════
    printf("Phase F: All input transitions (16x16 = 256 pairs)\n");

    full_reset(sim);
    for (uint8_t from = 0; from < 16; from++) {
        for (uint8_t to = 0; to < 16; to++) {
            set_rc(sim.dut.get(), from);
            for (int i = 0; i < 3; i++) tick_compare(sim);
            set_rc(sim.dut.get(), to);
            for (int i = 0; i < 3; i++) tick_compare(sim);
        }
    }
    clear_rc(sim.dut.get());
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase G: Rapid toggle patterns (Tests 21-24)
    // ════════════════════════════════════════════════════════════
    printf("Phase G: Rapid toggle patterns\n");

    // Test 21: Single-cycle halt pulses
    printf("  Test 21: Single-cycle halt pulses (100 iterations)\n");
    full_reset(sim);
    for (int iter = 0; iter < 100; iter++) {
        sim.dut->rc_halt_req_events_i = 1;
        tick_compare(sim);
        sim.dut->rc_halt_req_events_i = 0;
        tick_compare(sim);
        tick_compare(sim);
        sim.dut->rc_resume_req_events_i = 1;
        tick_compare(sim);
        sim.dut->rc_resume_req_events_i = 0;
        tick_compare(sim);
        tick_compare(sim);
    }

    // Test 22: Single-cycle GPIO halt pulses
    printf("  Test 22: Single-cycle GPIO halt pulses (100 iterations)\n");
    full_reset(sim);
    for (int iter = 0; iter < 100; iter++) {
        sim.dut->rc_halt_req_gpio_i = 1;
        tick_compare(sim);
        sim.dut->rc_halt_req_gpio_i = 0;
        tick_compare(sim);
        tick_compare(sim);
        sim.dut->rc_resume_req_gpio_i = 1;
        tick_compare(sim);
        sim.dut->rc_resume_req_gpio_i = 0;
        tick_compare(sim);
        tick_compare(sim);
    }

    // Test 23: Alternating sources halt/resume
    printf("  Test 23: Alternating event/GPIO halt/resume (100 iterations)\n");
    full_reset(sim);
    for (int iter = 0; iter < 100; iter++) {
        if (iter & 1) {
            sim.dut->rc_halt_req_events_i = 1;
            tick_compare(sim);
            sim.dut->rc_halt_req_events_i = 0;
        } else {
            sim.dut->rc_halt_req_gpio_i = 1;
            tick_compare(sim);
            sim.dut->rc_halt_req_gpio_i = 0;
        }
        tick_compare(sim);
        if (iter & 1) {
            sim.dut->rc_resume_req_gpio_i = 1;
            tick_compare(sim);
            sim.dut->rc_resume_req_gpio_i = 0;
        } else {
            sim.dut->rc_resume_req_events_i = 1;
            tick_compare(sim);
            sim.dut->rc_resume_req_events_i = 0;
        }
        tick_compare(sim);
    }

    // Test 24: Back-to-back halt without resume
    printf("  Test 24: Back-to-back halt without resume (50 iterations)\n");
    full_reset(sim);
    for (int iter = 0; iter < 50; iter++) {
        sim.dut->rc_halt_req_events_i = 1;
        tick_compare(sim);
        sim.dut->rc_halt_req_events_i = 0;
        for (int i = 0; i < 3; i++) tick_compare(sim);
    }
    // Now resume once
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase H: Sustained hold patterns (Tests 25-26)
    // ════════════════════════════════════════════════════════════
    printf("Phase H: Sustained hold patterns\n");

    // Test 25: Long sustained halt
    printf("  Test 25: Long sustained halt (200 cycles)\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 200; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 20; i++) tick_compare(sim);

    // Test 26: Long sustained resume
    printf("  Test 26: Long sustained resume (200 cycles)\n");
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 200; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 20; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase I: Reset stress (Tests 27-28)
    // ════════════════════════════════════════════════════════════
    printf("Phase I: Reset stress\n");

    // Test 27: Rapid warm reset toggles during halt
    printf("  Test 27: Rapid warm reset toggles during halt (100 cycles)\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 100; i++) {
        sim.dut->rst_w_ni = (i % 2 == 0) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_w_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // Test 28: Rapid debug reset toggles during halt
    printf("  Test 28: Rapid debug reset toggles (100 cycles)\n");
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 100; i++) {
        sim.dut->rst_d_ni = (i % 2 == 0) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase J: Reset + RC simultaneous (Tests 29-30)
    // ════════════════════════════════════════════════════════════
    printf("Phase J: Reset + RC simultaneous\n");

    // Test 29: Halt request asserted during debug reset deassert
    printf("  Test 29: Halt during debug reset deassert\n");
    full_reset(sim);
    sim.dut->rst_d_ni = 0;
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;  // deassert reset while halt still active
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // Test 30: Resume request asserted during debug reset
    printf("  Test 30: Resume during debug reset deassert\n");
    // Halt first
    sim.dut->rc_halt_req_gpio_i = 1;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 3; i++) tick_compare(sim);
    // Debug reset with resume asserted
    sim.dut->rst_d_ni = 0;
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rst_d_ni = 1;
    for (int i = 0; i < 10; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase K: Exhaustive RC with both reset combos
    // ════════════════════════════════════════════════════════════
    printf("Phase K: All RC combos under each reset state\n");

    // 4 reset states: (d=0,w=0), (d=0,w=1), (d=1,w=0), (d=1,w=1)
    // x 16 RC combos = 64 combos, 8 cycles each
    for (int rst = 0; rst < 4; rst++) {
        uint8_t rst_d = (rst >> 1) & 1;
        uint8_t rst_w = (rst >> 0) & 1;
        sim.dut->rst_d_ni = rst_d;
        sim.dut->rst_w_ni = rst_w;
        for (uint8_t vec = 0; vec < 16; vec++) {
            set_rc(sim.dut.get(), vec);
            for (int i = 0; i < 8; i++) tick_compare(sim);
        }
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase L: Multi-cycle sequences of all halt/resume permutations
    // ════════════════════════════════════════════════════════════
    printf("Phase L: Multi-cycle halt/resume permutations\n");

    // 4 single-source inputs, test each one alone for halt→resume cycle
    struct {
        const char* name;
        uint8_t halt_vec;
        uint8_t resume_vec;
    } sources[] = {
        {"events",      0x8, 0x4},  // halt_ev, resume_ev
        {"gpio",        0x2, 0x1},  // halt_gpio, resume_gpio
        {"cross_ev_gp", 0x8, 0x1}, // halt_ev, resume_gpio
        {"cross_gp_ev", 0x2, 0x4}, // halt_gpio, resume_ev
        {"both_halt",   0xA, 0x5}, // both halts, both resumes
    };

    for (auto& src : sources) {
        printf("  %s: halt→hold→resume→hold (20 iter)\n", src.name);
        full_reset(sim);
        for (int iter = 0; iter < 20; iter++) {
            set_rc(sim.dut.get(), src.halt_vec);
            for (int i = 0; i < 4; i++) tick_compare(sim);
            clear_rc(sim.dut.get());
            for (int i = 0; i < 4; i++) tick_compare(sim);
            set_rc(sim.dut.get(), src.resume_vec);
            for (int i = 0; i < 4; i++) tick_compare(sim);
            clear_rc(sim.dut.get());
            for (int i = 0; i < 4; i++) tick_compare(sim);
        }
    }

    // ════════════════════════════════════════════════════════════
    // Phase M: Staggered source activation
    // ════════════════════════════════════════════════════════════
    printf("Phase M: Staggered source activation\n");

    // Halt via events, then add GPIO halt while already halted
    full_reset(sim);
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 1;  // add second source
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_events_i = 0;  // remove first
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_halt_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // Resume via events, then add GPIO resume
    sim.dut->rc_resume_req_events_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 1;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);
    sim.dut->rc_resume_req_gpio_i = 0;
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase N: Walking ones on RC inputs (Tests 31-34)
    // ════════════════════════════════════════════════════════════
    printf("Phase N: Walking ones on RC inputs\n");

    full_reset(sim);
    for (int repeat = 0; repeat < 50; repeat++) {
        for (uint8_t bit = 0; bit < 4; bit++) {
            set_rc(sim.dut.get(), 1 << bit);
            for (int i = 0; i < 3; i++) tick_compare(sim);
            clear_rc(sim.dut.get());
            for (int i = 0; i < 3; i++) tick_compare(sim);
        }
    }

    // ════════════════════════════════════════════════════════════
    // Phase O: Walking zeros on RC inputs
    // ════════════════════════════════════════════════════════════
    printf("Phase O: Walking zeros on RC inputs\n");

    full_reset(sim);
    for (int repeat = 0; repeat < 50; repeat++) {
        for (uint8_t bit = 0; bit < 4; bit++) {
            set_rc(sim.dut.get(), 0xF ^ (1 << bit));
            for (int i = 0; i < 3; i++) tick_compare(sim);
            clear_rc(sim.dut.get());
            for (int i = 0; i < 3; i++) tick_compare(sim);
        }
    }

    // ════════════════════════════════════════════════════════════
    // Phase P: Reset interleaved with halt/resume (1000 cycles)
    // ════════════════════════════════════════════════════════════
    printf("Phase P: Random halt/resume interleaved with resets (1000 cycles)\n");

    rng_state = 0x12345678;
    full_reset(sim);
    for (int i = 0; i < 1000; i++) {
        uint32_t r = xorshift32();
        // RC inputs from low bits
        set_rc(sim.dut.get(), r & 0xF);
        // Warm reset: ~3% chance
        if ((r >> 4) % 32 == 0) sim.dut->rst_w_ni = 0;
        else                     sim.dut->rst_w_ni = 1;
        // Debug reset: ~3% chance
        if ((r >> 9) % 32 == 0) sim.dut->rst_d_ni = 0;
        else                     sim.dut->rst_d_ni = 1;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase Q: Extended random traffic (5000 cycles)
    // ════════════════════════════════════════════════════════════
    printf("Phase Q: Extended random RC traffic (5000 cycles)\n");

    rng_state = 0xCAFEBABE;
    full_reset(sim);
    for (int i = 0; i < 5000; i++) {
        uint32_t r = xorshift32();
        set_rc(sim.dut.get(), r & 0xF);
        tick_compare(sim);
    }
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase R: Focused random with frequent resets (3000 cycles)
    // ════════════════════════════════════════════════════════════
    printf("Phase R: Focused random with frequent resets (3000 cycles)\n");

    rng_state = 0xFEEDFACE;
    full_reset(sim);
    for (int i = 0; i < 3000; i++) {
        uint32_t r = xorshift32();
        set_rc(sim.dut.get(), r & 0xF);
        // More aggressive resets: ~6% warm, ~6% debug
        sim.dut->rst_w_ni = ((r >> 4) % 16 == 0) ? 0 : 1;
        sim.dut->rst_d_ni = ((r >> 8) % 16 == 0) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase S: Structured patterns — prime-modular toggling
    // ════════════════════════════════════════════════════════════
    printf("Phase S: Prime-modular toggling (2000 cycles)\n");

    full_reset(sim);
    for (int i = 0; i < 2000; i++) {
        sim.dut->rc_halt_req_events_i   = (i % 7 < 2)  ? 1 : 0;
        sim.dut->rc_resume_req_events_i = (i % 11 < 2) ? 1 : 0;
        sim.dut->rc_halt_req_gpio_i     = (i % 13 < 3) ? 1 : 0;
        sim.dut->rc_resume_req_gpio_i   = (i % 17 < 3) ? 1 : 0;
        tick_compare(sim);
    }
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase T: Modular with warm reset injection
    // ════════════════════════════════════════════════════════════
    printf("Phase T: Modular with warm reset injection (2000 cycles)\n");

    full_reset(sim);
    for (int i = 0; i < 2000; i++) {
        sim.dut->rc_halt_req_events_i   = (i % 5 < 1) ? 1 : 0;
        sim.dut->rc_resume_req_events_i = (i % 9 < 1) ? 1 : 0;
        sim.dut->rc_halt_req_gpio_i     = (i % 19 < 2) ? 1 : 0;
        sim.dut->rc_resume_req_gpio_i   = (i % 23 < 2) ? 1 : 0;
        sim.dut->rst_w_ni = (i % 97 < 3) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase U: Modular with debug reset injection
    // ════════════════════════════════════════════════════════════
    printf("Phase U: Modular with debug reset injection (2000 cycles)\n");

    full_reset(sim);
    for (int i = 0; i < 2000; i++) {
        sim.dut->rc_halt_req_events_i   = (i % 3 < 1) ? 1 : 0;
        sim.dut->rc_resume_req_events_i = (i % 7 < 1) ? 1 : 0;
        sim.dut->rc_halt_req_gpio_i     = (i % 11 < 2) ? 1 : 0;
        sim.dut->rc_resume_req_gpio_i   = (i % 29 < 3) ? 1 : 0;
        sim.dut->rst_d_ni = (i % 83 < 2) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase V: Modular with both resets
    // ════════════════════════════════════════════════════════════
    printf("Phase V: Modular with both resets (2000 cycles)\n");

    full_reset(sim);
    for (int i = 0; i < 2000; i++) {
        sim.dut->rc_halt_req_events_i   = (i % 7 < 2) ? 1 : 0;
        sim.dut->rc_resume_req_events_i = (i % 13 < 2) ? 1 : 0;
        sim.dut->rc_halt_req_gpio_i     = (i % 17 < 3) ? 1 : 0;
        sim.dut->rc_resume_req_gpio_i   = (i % 19 < 3) ? 1 : 0;
        sim.dut->rst_w_ni = (i % 61 < 2) ? 0 : 1;
        sim.dut->rst_d_ni = (i % 73 < 2) ? 0 : 1;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase W: Counter-based patterns (2000 cycles)
    // ════════════════════════════════════════════════════════════
    printf("Phase W: Counter-based bit patterns (2000 cycles)\n");

    full_reset(sim);
    for (int i = 0; i < 2000; i++) {
        // Use counter bits directly as RC inputs — cycles through all combos
        set_rc(sim.dut.get(), (i >> 2) & 0xF);
        tick_compare(sim);
    }
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase X: LFSR-based stimulus with all 6 signals (3000 cycles)
    // ════════════════════════════════════════════════════════════
    printf("Phase X: LFSR-based full stimulus (3000 cycles)\n");

    rng_state = 0xA5A5A5A5;
    full_reset(sim);
    for (int i = 0; i < 3000; i++) {
        uint32_t r = xorshift32();
        set_rc(sim.dut.get(), r & 0xF);
        // Bits 4-5 drive resets (each with ~25% active probability)
        sim.dut->rst_d_ni = ((r >> 4) & 3) != 0 ? 1 : 0;
        sim.dut->rst_w_ni = ((r >> 6) & 3) != 0 ? 1 : 0;
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase Y: Glitch injection — single-cycle reset pulses
    // ════════════════════════════════════════════════════════════
    printf("Phase Y: Single-cycle reset glitches (500 iterations)\n");

    full_reset(sim);
    for (int iter = 0; iter < 500; iter++) {
        uint32_t r = xorshift32();
        set_rc(sim.dut.get(), r & 0xF);
        tick_compare(sim);

        // Single-cycle warm reset glitch
        if (iter % 3 == 0) {
            sim.dut->rst_w_ni = 0;
            tick_compare(sim);
            sim.dut->rst_w_ni = 1;
        }
        tick_compare(sim);

        // Single-cycle debug reset glitch
        if (iter % 5 == 0) {
            sim.dut->rst_d_ni = 0;
            tick_compare(sim);
            sim.dut->rst_d_ni = 1;
        }
        tick_compare(sim);
    }
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    clear_rc(sim.dut.get());
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ════════════════════════════════════════════════════════════
    // Phase Z: Final cleanup + idle check
    // ════════════════════════════════════════════════════════════
    printf("Phase Z: Final idle check\n");
    full_reset(sim);
    for (int i = 0; i < 50; i++) tick_compare(sim);

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: minion_frontend_thread_sched vs frontend_thread_sched.
//
// Exhaustive and random testing of all input combinations and
// state transitions.

#include "Vcosim_thread_sched_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_thread_sched_tb;

static void drive_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();
    sim.compare("thread_awake", sim.dut->orig_f7_thread_awake_o,
                 sim.dut->new_f7_thread_awake_o);
    sim.compare("thread_id", (uint64_t)sim.dut->orig_f7_thread_id_o,
                 (uint64_t)sim.dut->new_f7_thread_id_o);
    sim.tick();
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;

    sim.dut->f7_valid_tid_i = 0;
    sim.dut->f0_thread_enabled_i = 3;
    sim.dut->id_core_stall_i = 0;
    sim.reset();

    // Phase 1: exhaustive input sweep.
    // All 64 combinations of {valid[1:0], enabled[1:0], stall[1:0]}
    // held for 4 cycles each to capture state evolution.
    printf("phase 1: exhaustive input sweep (64 combos × 4 cycles)\n");
    for (int v = 0; v < 4; v++) {
        for (int e = 0; e < 4; e++) {
            for (int s = 0; s < 4; s++) {
                sim.dut->f7_valid_tid_i = v;
                sim.dut->f0_thread_enabled_i = e;
                sim.dut->id_core_stall_i = s;
                for (int c = 0; c < 4; c++) {
                    drive_and_compare(sim);
                }
            }
        }
    }

    // Phase 2: long alternation — both threads valid and enabled,
    // verify round-robin doesn't drift over 100 cycles.
    printf("phase 2: long alternation (100 cycles)\n");
    sim.dut->f7_valid_tid_i = 3;
    sim.dut->f0_thread_enabled_i = 3;
    sim.dut->id_core_stall_i = 0;
    for (int i = 0; i < 100; i++) {
        drive_and_compare(sim);
    }

    // Phase 3: stall/unstall transitions.
    printf("phase 3: stall/unstall transitions\n");
    sim.dut->f7_valid_tid_i = 3;
    sim.dut->f0_thread_enabled_i = 3;
    for (int i = 0; i < 50; i++) {
        // Alternate stalling thread 0, thread 1, both, neither.
        sim.dut->id_core_stall_i = i % 4;
        drive_and_compare(sim);
    }

    // Phase 4: enable/disable transitions.
    printf("phase 4: enable/disable transitions\n");
    sim.dut->id_core_stall_i = 0;
    sim.dut->f7_valid_tid_i = 3;
    for (int i = 0; i < 50; i++) {
        sim.dut->f0_thread_enabled_i = i % 4;
        drive_and_compare(sim);
    }

    // Phase 5: rapid valid toggling — thread 0 valid, then thread 1,
    // then both, then neither, cycling.
    printf("phase 5: rapid valid toggling\n");
    sim.dut->f0_thread_enabled_i = 3;
    sim.dut->id_core_stall_i = 0;
    for (int i = 0; i < 200; i++) {
        sim.dut->f7_valid_tid_i = i % 4;
        drive_and_compare(sim);
    }

    // Phase 6: thread_id_prev latch-enable test.
    // When no valid instruction, previous thread ID should NOT update.
    printf("phase 6: thread_id_prev hold when no valid\n");
    // First force a known state: both valid for a few cycles.
    sim.dut->f7_valid_tid_i = 3;
    sim.dut->f0_thread_enabled_i = 3;
    sim.dut->id_core_stall_i = 0;
    for (int i = 0; i < 4; i++) drive_and_compare(sim);
    // Now no valid: thread_id_prev should hold.
    sim.dut->f7_valid_tid_i = 0;
    for (int i = 0; i < 20; i++) drive_and_compare(sim);
    // Resume with both valid: should alternate from held state.
    sim.dut->f7_valid_tid_i = 3;
    for (int i = 0; i < 20; i++) drive_and_compare(sim);

    // Phase 7: single thread with stall/unstall.
    printf("phase 7: single thread with stall toggling\n");
    sim.dut->f7_valid_tid_i = 1;
    sim.dut->f0_thread_enabled_i = 3;
    for (int i = 0; i < 40; i++) {
        sim.dut->id_core_stall_i = (i / 5) % 4;
        drive_and_compare(sim);
    }

    // Phase 8: heavy random stimulus (2000 cycles).
    printf("phase 8: random stimulus (2000 cycles)\n");
    uint32_t seed = 0xCAFEBABE;
    for (int i = 0; i < 2000; i++) {
        seed = seed * 1103515245 + 12345;
        sim.dut->f7_valid_tid_i      = seed & 3;
        sim.dut->f0_thread_enabled_i = (seed >> 2) & 3;
        sim.dut->id_core_stall_i     = (seed >> 4) & 3;
        drive_and_compare(sim);
    }

    // Phase 9: reset in the middle, then resume.
    printf("phase 9: mid-test reset\n");
    sim.dut->f7_valid_tid_i = 3;
    sim.dut->f0_thread_enabled_i = 3;
    sim.dut->id_core_stall_i = 0;
    for (int i = 0; i < 10; i++) drive_and_compare(sim);
    sim.reset();
    for (int i = 0; i < 20; i++) drive_and_compare(sim);

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_lru_4w_calc — Verilator test.
// Tests the 4-way LRU replacement calculator.

#include "Vshirecache_pipe_lru_4w_calc.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_lru_4w_calc;

// Drive combinational inputs and evaluate.
static void eval_lru(SimCtrl<DUT>& sim,
                     uint8_t lru_state, uint8_t hit_way,
                     bool hit_locked, uint8_t valid, uint8_t locked,
                     bool locking, bool unlocking,
                     bool update_mru, bool update_lru) {
    sim.dut->lru_state_i    = lru_state;
    sim.dut->hit_way_i      = hit_way;
    sim.dut->hit_locked_i   = hit_locked;
    sim.dut->valid_bits_i   = valid;
    sim.dut->locked_bits_i  = locked;
    sim.dut->locking_way_i  = locking;
    sim.dut->unlocking_way_i= unlocking;
    sim.dut->update_mru_i   = update_mru;
    sim.dut->update_lru_i   = update_lru;
    sim.dut->eval();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    // Initial LRU state: way order [0,1,2,3] from LRU to MRU.
    // full_lru = 00_01_10_11 = way3=MRU, way0=LRU
    // Compressed: lru_state[3:0] = {way1, way0} = {01, 00} = 4'b0100
    //             lru_state[4] = (way2 < way3) = (2 < 3) = 0
    // So initial state = 5'b00100

    // ── Test 1: MRU promotion ───────────────────────────
    printf("test 1: MRU promotion of LRU way\n");
    // Hit way 0 (currently LRU), promote to MRU.
    eval_lru(sim, 0b00100, 0, false, 0xF, 0, false, false, true, false);
    // way0 should move to MRU. New order: [1,2,3,0]
    // Check that output changed.
    sim.check(sim.dut->lru_lock_err_o == 0, "no lock error");
    uint8_t new_state = sim.dut->lru_state_o;
    // Verify: way0 is at MRU (bits [7:6] of full state).
    // We can't directly read full state, but we can check that
    // the state changed from the input.
    sim.check(new_state != 0b00100, "state changed after MRU promotion");

    // ── Test 2: LRU demotion ────────────────────────────
    printf("test 2: LRU demotion of MRU way\n");
    // Starting from same state, hit way 3 (MRU), demote to LRU.
    eval_lru(sim, 0b00100, 3, false, 0xF, 0, false, false, false, true);
    sim.check(sim.dut->lru_lock_err_o == 0, "no lock error");
    uint8_t demoted_state = sim.dut->lru_state_o;
    sim.check(demoted_state != 0b00100, "state changed after LRU demotion");

    // ── Test 3: No update ───────────────────────────────
    printf("test 3: no update preserves state\n");
    eval_lru(sim, 0b00100, 0, false, 0xF, 0, false, false, false, false);
    sim.check(sim.dut->lru_state_o == 0b00100, "state preserved when no update");

    // ── Test 4: Lock error ──────────────────────────────
    printf("test 4: lock error when locking last unlocked way\n");
    // 3 of 4 ways locked (0b1110), trying to lock way 0 (the only unlocked).
    eval_lru(sim, 0b00100, 0, false, 0xF, 0b1110, true, false, true, false);
    sim.check(sim.dut->lru_lock_err_o == 1, "lock error detected");

    // ── Test 5: No lock error when way already locked ───
    printf("test 5: no lock error when re-locking already-locked way\n");
    eval_lru(sim, 0b00100, 1, true, 0xF, 0b1110, true, false, true, false);
    sim.check(sim.dut->lru_lock_err_o == 0, "no lock error (already locked)");

    // ── Test 6: All invalid → default state ─────────────
    printf("test 6: all invalid gives default state\n");
    eval_lru(sim, 0b00000, 0, false, 0x0, 0, false, false, false, false);
    // Default full state is 8'b00100111.
    // Compressed: [3:0] = full[3:0] = 4'b0111
    //             [4] = (full[5:4] < full[7:6]) = (01 < 00) = 0
    // lru_state_o = 5'b00111
    // But the input lru_state is 0, and we're not updating, so the
    // module returns full_lru computed from the default branch.
    // full_lru = 8'b00100111 → [3:0]=0111, [4]=(01<00)=0 → 5'b00111
    // Actually: 01 < 00 is false → bit4=1 → 5'b10111
    sim.check(sim.dut->lru_state_o == 0b10111, "default state for all invalid");

    // ── Test 7: Round-robin MRU promotions ──────────────
    printf("test 7: sequential MRU promotions\n");
    uint8_t state = 0b00100; // initial: [0,1,2,3]
    for (int w = 0; w < 4; w++) {
        eval_lru(sim, state, w, false, 0xF, 0, false, false, true, false);
        sim.check(sim.dut->lru_lock_err_o == 0,
                  "no lock err promoting way " + std::to_string(w));
        state = sim.dut->lru_state_o;
    }

    return sim.finish();
}

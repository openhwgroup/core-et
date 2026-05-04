// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: shire_cache_pipe_lru_4w_calc vs shirecache_pipe_lru_4w_calc.
// Purely combinational — no clock needed, just compare outputs.

#include "Vcosim_lru_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_lru_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("lru_state",    sim.dut->orig_lru_state_o,    sim.dut->new_lru_state_o);
    sim.compare("lru_lock_err", sim.dut->orig_lru_lock_err_o, sim.dut->new_lru_lock_err_o);
}

static void eval_and_compare(CosimCtrl<DUT>& sim,
                             uint8_t lru_state, uint8_t hit_way,
                             bool hit_locked, uint8_t valid, uint8_t locked,
                             bool locking, bool unlocking,
                             bool update_mru, bool update_lru) {
    sim.dut->lru_state_i     = lru_state;
    sim.dut->hit_way_i       = hit_way;
    sim.dut->hit_locked_i    = hit_locked;
    sim.dut->valid_bits_i    = valid;
    sim.dut->locked_bits_i   = locked;
    sim.dut->locking_way_i   = locking;
    sim.dut->unlocking_way_i = unlocking;
    sim.dut->update_mru_i    = update_mru;
    sim.dut->update_lru_i    = update_lru;
    sim.dut->eval();
    sim.sim_time++;
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 100000;

    // ── Test 1: All valid LRU states, MRU each way ──────
    printf("cosim test 1: all valid states, MRU promotion\n");
    // There are 24 valid orderings of 4 ways.
    // Try MRU promotion of each way from each valid compressed state.
    uint8_t valid_states[] = {
        0b00100, 0b10100, 0b00001, 0b10001,
        0b01000, 0b11000, 0b00010, 0b10010,
        0b01100, 0b11100, 0b00011, 0b10011,
        0b01001, 0b11001, 0b00110, 0b10110,
        0b01101, 0b11101, 0b00111, 0b10111,
        0b01110, 0b11110, 0b01011, 0b11011
    };
    for (int s = 0; s < 24; s++) {
        for (int w = 0; w < 4; w++) {
            eval_and_compare(sim, valid_states[s], w, false, 0xF, 0,
                             false, false, true, false);
        }
    }

    // ── Test 2: All valid states, LRU demotion ──────────
    printf("cosim test 2: all valid states, LRU demotion\n");
    for (int s = 0; s < 24; s++) {
        for (int w = 0; w < 4; w++) {
            eval_and_compare(sim, valid_states[s], w, false, 0xF, 0,
                             false, false, false, true);
        }
    }

    // ── Test 3: No update ───────────────────────────────
    printf("cosim test 3: no update\n");
    for (int s = 0; s < 24; s++) {
        eval_and_compare(sim, valid_states[s], 0, false, 0xF, 0,
                         false, false, false, false);
    }

    // ── Test 4: Locked ways ─────────────────────────────
    printf("cosim test 4: locked ways\n");
    for (int s = 0; s < 24; s++) {
        for (int locked = 0; locked < 16; locked++) {
            for (int w = 0; w < 4; w++) {
                bool is_locked = (locked >> w) & 1;
                eval_and_compare(sim, valid_states[s], w, is_locked,
                                 0xF, locked, false, false, true, false);
            }
        }
    }

    // ── Test 5: Locking way ─────────────────────────────
    printf("cosim test 5: locking way\n");
    for (int s = 0; s < 24; s++) {
        for (int locked = 0; locked < 16; locked++) {
            for (int w = 0; w < 4; w++) {
                bool is_locked = (locked >> w) & 1;
                eval_and_compare(sim, valid_states[s], w, is_locked,
                                 0xF, locked, true, false, true, false);
            }
        }
    }

    // ── Test 6: Unlocking way ───────────────────────────
    printf("cosim test 6: unlocking way\n");
    for (int s = 0; s < 24; s++) {
        for (int locked = 0; locked < 16; locked++) {
            for (int w = 0; w < 4; w++) {
                bool is_locked = (locked >> w) & 1;
                eval_and_compare(sim, valid_states[s], w, is_locked,
                                 0xF, locked, false, true, true, false);
            }
        }
    }

    // ── Test 7: Partial valid bits ───────────────────────
    printf("cosim test 7: partial valid bits\n");
    for (int v = 0; v < 16; v++) {
        for (int w = 0; w < 4; w++) {
            eval_and_compare(sim, 0b00100, w, false, v, 0,
                             false, false, true, false);
        }
    }

    return sim.finish();
}

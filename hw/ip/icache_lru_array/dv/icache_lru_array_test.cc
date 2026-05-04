// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_lru_array_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vicache_lru_array_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static uint8_t lru_victim(uint16_t lru_in) {
    uint8_t victim = 0;
    for (int i = 0; i < 4; ++i) {
        bool v = true;
        for (int j = 0; j < i; ++j) {
            v &= ((lru_in >> (i * 4 + j)) & 1u) != 0;
        }
        for (int j = i + 1; j < 4; ++j) {
            v &= ((lru_in >> (j * 4 + i)) & 1u) == 0;
        }
        if (v) victim |= (1u << i);
    }
    return victim;
}

static uint16_t lru_update(uint16_t lru_in, uint8_t way_en) {
    uint16_t lru_out = lru_in;
    for (int i = 0; i < 4; ++i) {
        if ((way_en >> i) & 1u) {
            for (int j = 0; j < i; ++j) {
                lru_out &= ~(1u << (i * 4 + j));
            }
            for (int j = i + 1; j < 4; ++j) {
                lru_out |= (1u << (j * 4 + i));
            }
        }
    }
    return lru_out;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    std::array<uint16_t, 128> rf_model{};
    std::array<bool, 128> init_model{};
    uint16_t f1_lru_data_q = 0;
    uint8_t f1_victim_q = 0;
    bool saw_f1_victim = false;
    uint8_t last_read_set = 0;
    bool have_f1_state = false;

    auto apply_cycle = [&](bool f0_valid,
                           uint8_t f0_access_set,
                           bool f1_update_en_early,
                           bool f1_update_en,
                           uint8_t f1_update_set,
                           uint8_t f1_update_access_way,
                           const std::string& msg) {
        const uint16_t new_lru_data = lru_update(f1_lru_data_q, f1_update_access_way);
        const uint16_t read_lru_data = init_model[f0_access_set] ? rf_model[f0_access_set] : 0;
        const uint16_t read_lru_byp =
            (f1_update_en && (f1_update_set == f0_access_set)) ? new_lru_data : read_lru_data;
        const uint8_t victim = lru_victim(read_lru_byp);

        sim.dut->f0_valid_i = f0_valid;
        sim.dut->f0_access_set_i = f0_access_set;
        sim.dut->f1_update_en_early_i = f1_update_en_early;
        sim.dut->f1_update_en_i = f1_update_en;
        sim.dut->f1_update_set_i = f1_update_set;
        sim.dut->f1_update_access_way_i = f1_update_access_way;
        sim.tick();

        if (f1_update_en) {
            init_model[f1_update_set] = true;
            if (f1_update_en_early) {
                rf_model[f1_update_set] = new_lru_data;
            }
        }
        if (f0_valid) {
            f1_lru_data_q = read_lru_byp;
            f1_victim_q = victim;
            saw_f1_victim = true;
            last_read_set = f0_access_set;
            have_f1_state = true;
        }
        if (saw_f1_victim) {
            sim.check(sim.dut->f1_victim_o == f1_victim_q, msg);
        }
    };

    sim.dut->f0_valid_i = 0;
    sim.dut->f0_access_set_i = 0;
    sim.dut->f1_update_en_early_i = 0;
    sim.dut->f1_update_en_i = 0;
    sim.dut->f1_update_set_i = 0;
    sim.dut->f1_update_access_way_i = 0;
    sim.reset();

    apply_cycle(true, 0, false, false, 0, 0, "initial victim for uninitialized set is way 0");
    apply_cycle(true, 0, true, true, 0, 0x2, "same-set bypass uses updated LRU state");
    apply_cycle(true, 0, false, false, 0, 0, "committed update persists in RF");
    apply_cycle(true, 1, false, false, 0, 0, "different set remains independently uninitialized");

    apply_cycle(true, 1, true, true, 1, 0x8, "update on second set uses previous read state");
    apply_cycle(true, 1, false, false, 0, 0, "second set write persists");

    apply_cycle(false, 0, false, true, 1, 0x1, "update without early arm keeps stale RF contents");
    apply_cycle(true, 1, false, false, 0, 0, "missing early arm does not modify stored LRU state");

    uint32_t rng = 0x1badb002u;
    for (int i = 0; i < 2048; ++i) {
        const bool f0_valid = true;
        uint8_t f0_access_set = static_cast<uint8_t>(next_rand(rng) & 0x7f);
        const bool f1_update_en = have_f1_state && ((next_rand(rng) & 0x3) != 0);
        const bool use_same_set_bypass = f1_update_en && ((next_rand(rng) & 0x1) != 0);
        const uint8_t f1_update_set = have_f1_state ? last_read_set : 0;
        const uint8_t access_way = static_cast<uint8_t>(1u << (next_rand(rng) & 0x3));
        if (use_same_set_bypass) {
            f0_access_set = f1_update_set;
        }

        apply_cycle(f0_valid,
                    f0_access_set,
                    f1_update_en,
                    f1_update_en,
                    f1_update_set,
                    access_way,
                    "random pipeline-consistent cycle " + std::to_string(i));
    }

    return sim.finish();
}

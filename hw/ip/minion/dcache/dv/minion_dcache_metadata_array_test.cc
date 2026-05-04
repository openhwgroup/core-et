// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_metadata_array — Verilator test.

#include "Vminion_dcache_metadata_array_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_metadata_array_tb;

namespace {

struct MetaData {
    uint8_t coh = 0;
    uint64_t tag = 0;
};

void tick_and_settle(SimCtrl<DUT>& sim) {
    sim.dut->eval();  // Settle combinational wrapper inputs before the negedge capture.
    sim.tick();
    sim.dut->eval();  // Settle unpacked-array read outputs after the write edge.
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void check_read_set(SimCtrl<DUT>& sim,
                    const std::array<std::array<bool, 4>, 16>& valid,
                    const std::array<std::array<MetaData, 4>, 16>& mem,
                    uint8_t set,
                    const std::string& label) {
    const uint8_t expect_valid =
        (valid[set][0] ? 1u : 0u) |
        (valid[set][1] ? 2u : 0u) |
        (valid[set][2] ? 4u : 0u) |
        (valid[set][3] ? 8u : 0u);

    sim.check(sim.dut->s1_read_resp_valid_o == expect_valid,
              label + " valid got=" + std::to_string(sim.dut->s1_read_resp_valid_o) +
              " expect=" + std::to_string(expect_valid));
    if (valid[set][0]) {
        sim.check(sim.dut->way0_coh_state_o == mem[set][0].coh,
                  label + " way0 coh got=" + std::to_string(sim.dut->way0_coh_state_o) +
                  " expect=" + std::to_string(mem[set][0].coh));
        sim.check(sim.dut->way0_tag_o == mem[set][0].tag,
                  label + " way0 tag got=" + std::to_string(sim.dut->way0_tag_o) +
                  " expect=" + std::to_string(mem[set][0].tag));
    }
    if (valid[set][1]) {
        sim.check(sim.dut->way1_coh_state_o == mem[set][1].coh,
                  label + " way1 coh got=" + std::to_string(sim.dut->way1_coh_state_o) +
                  " expect=" + std::to_string(mem[set][1].coh));
        sim.check(sim.dut->way1_tag_o == mem[set][1].tag,
                  label + " way1 tag got=" + std::to_string(sim.dut->way1_tag_o) +
                  " expect=" + std::to_string(mem[set][1].tag));
    }
    if (valid[set][2]) {
        sim.check(sim.dut->way2_coh_state_o == mem[set][2].coh,
                  label + " way2 coh got=" + std::to_string(sim.dut->way2_coh_state_o) +
                  " expect=" + std::to_string(mem[set][2].coh));
        sim.check(sim.dut->way2_tag_o == mem[set][2].tag,
                  label + " way2 tag got=" + std::to_string(sim.dut->way2_tag_o) +
                  " expect=" + std::to_string(mem[set][2].tag));
    }
    if (valid[set][3]) {
        sim.check(sim.dut->way3_coh_state_o == mem[set][3].coh,
                  label + " way3 coh got=" + std::to_string(sim.dut->way3_coh_state_o) +
                  " expect=" + std::to_string(mem[set][3].coh));
        sim.check(sim.dut->way3_tag_o == mem[set][3].tag,
                  label + " way3 tag got=" + std::to_string(sim.dut->way3_tag_o) +
                  " expect=" + std::to_string(mem[set][3].tag));
    }
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    std::array<std::array<bool, 4>, 16> valid{};
    std::array<std::array<MetaData, 4>, 16> mem{};
    MetaData captured_write_data{};

    sim.dut->s1_read_set_i = 0;
    sim.dut->s1_write_valid_i = 0;
    sim.dut->s1_write_valid_prev_i = 0;
    sim.dut->s1_write_set_i = 0;
    sim.dut->s1_write_way_en_i = 0;
    sim.dut->s1_write_coh_state_i = 0;
    sim.dut->s1_write_tag_i = 0;
    sim.dut->cfg_clear_all_i = 0;
    sim.dut->cfg_clear_low_i = 0;
    sim.reset();
    sim.dut->eval();

    printf("test 1: directed metadata updates\n");
    tick_and_settle(sim);
    check_read_set(sim, valid, mem, 0, "reset");

    sim.dut->s1_read_set_i = 5;
    sim.dut->s1_write_valid_i = 1;
    sim.dut->s1_write_valid_prev_i = 1;
    sim.dut->s1_write_set_i = 5;
    sim.dut->s1_write_way_en_i = 0b0101;
    sim.dut->s1_write_coh_state_i = 0b10;
    sim.dut->s1_write_tag_i = 0x155555555ULL;
    // Latch transparent: current s1_write_valid_prev_i=1 → RF captures data this negedge.
    captured_write_data = {0b10, 0x155555555ULL};
    tick_and_settle(sim);
    for (int way : {0, 2}) {
        mem[5][way] = captured_write_data;
        valid[5][way] = true;
    }
    check_read_set(sim, valid, mem, 5, "write set 5");

    sim.dut->s1_read_set_i = 6;
    sim.dut->s1_write_valid_i = 1;
    sim.dut->s1_write_valid_prev_i = 0;
    sim.dut->s1_write_set_i = 6;
    sim.dut->s1_write_way_en_i = 0b0010;
    sim.dut->s1_write_coh_state_i = 0b01;
    sim.dut->s1_write_tag_i = 0x0ABCDEF01ULL;
    // Latch opaque: current s1_write_valid_prev_i=0 → no capture, stale data committed.
    tick_and_settle(sim);
    mem[6][1] = captured_write_data;
    valid[6][1] = true;
    check_read_set(sim, valid, mem, 6, "valid-only update");

    sim.dut->cfg_clear_low_i = 1;
    sim.dut->s1_write_valid_i = 0;
    sim.dut->s1_write_valid_prev_i = 0;
    sim.dut->s1_read_set_i = 5;
    tick_and_settle(sim);
    for (int set = 0; set < 14; ++set) {
        for (int way = 0; way < 4; ++way) {
            valid[set][way] = false;
        }
    }
    check_read_set(sim, valid, mem, 5, "clear low");

    sim.dut->cfg_clear_low_i = 0;
    sim.dut->cfg_clear_all_i = 1;
    sim.dut->s1_read_set_i = 15;
    tick_and_settle(sim);
    for (auto& set_valid : valid) {
        set_valid = {false, false, false, false};
    }
    check_read_set(sim, valid, mem, 15, "clear all");
    sim.dut->cfg_clear_all_i = 0;

    printf("test 2: random metadata traffic\n");
    uint32_t seed = 0x51A7C0DEu;
    for (int i = 0; i < 256; ++i) {
        const uint8_t read_set = xorshift32(seed) & 0xF;
        const bool write_valid = (xorshift32(seed) & 1u) != 0;
        const bool write_prev = (xorshift32(seed) & 1u) != 0;
        const uint8_t write_set = xorshift32(seed) & 0xF;
        const uint8_t way_en = xorshift32(seed) & 0xF;
        const uint8_t coh = xorshift32(seed) & 0x3;
        const uint64_t tag = xorshift32(seed) & 0x1FFFFFFFFULL;
        const bool clear_all = (xorshift32(seed) & 0x1Fu) == 0;
        const bool clear_low = !clear_all && ((xorshift32(seed) & 0xFu) == 0);

        sim.dut->s1_read_set_i = read_set;
        sim.dut->s1_write_valid_i = write_valid;
        sim.dut->s1_write_valid_prev_i = write_prev;
        sim.dut->s1_write_set_i = write_set;
        sim.dut->s1_write_way_en_i = way_en;
        sim.dut->s1_write_coh_state_i = coh;
        sim.dut->s1_write_tag_i = tag;
        sim.dut->cfg_clear_all_i = clear_all;
        sim.dut->cfg_clear_low_i = clear_low;
        // Latch transparent on negedge when current s1_write_valid_prev_i=1.
        if (write_prev) {
            captured_write_data = {coh, tag};
        }
        tick_and_settle(sim);

        if (write_valid) {
            for (int way = 0; way < 4; ++way) {
                if ((way_en >> way) & 1u) {
                    mem[write_set][way] = captured_write_data;
                }
            }
        }
        if (clear_all) {
            for (auto& set_valid : valid) {
                set_valid = {false, false, false, false};
            }
        } else if (clear_low) {
            for (int set = 0; set < 14; ++set) {
                valid[set] = {false, false, false, false};
            }
        } else if (write_valid) {
            for (int way = 0; way < 4; ++way) {
                if ((way_en >> way) & 1u) {
                    valid[write_set][way] = (coh != 0);
                }
            }
        }

        check_read_set(sim, valid, mem, read_set, "random cycle " + std::to_string(i));
    }

    return sim.finish();
}

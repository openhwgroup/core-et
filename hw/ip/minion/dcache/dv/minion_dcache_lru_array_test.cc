// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_lru_array — Verilator test.

#include "Vminion_dcache_lru_array_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vminion_dcache_lru_array_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    std::array<uint16_t, 16> model{};

    sim.dut->read_set_i = 0;
    sim.dut->write_en_i = 0;
    sim.dut->write_set_i = 0;
    sim.dut->write_data_i = 0;
    sim.reset();

    printf("test 1: reset and directed writes\n");
    for (int set = 0; set < 16; ++set) {
        sim.dut->read_set_i = set;
        sim.tick();
        sim.check(sim.dut->read_data_o == 0, "reset state set " + std::to_string(set));
    }

    sim.dut->read_set_i = 3;
    sim.dut->write_en_i = 1;
    sim.dut->write_set_i = 3;
    sim.dut->write_data_i = 0xA55A;
    sim.tick();
    model[3] = 0xA55A;
    sim.check(sim.dut->read_data_o == model[3], "write set 3");

    sim.dut->read_set_i = 7;
    sim.dut->write_en_i = 1;
    sim.dut->write_set_i = 7;
    sim.dut->write_data_i = 0x0F0F;
    sim.tick();
    model[7] = 0x0F0F;
    sim.check(sim.dut->read_data_o == model[7], "write set 7");

    sim.dut->write_en_i = 0;
    sim.dut->read_set_i = 3;
    sim.tick();
    sim.check(sim.dut->read_data_o == model[3], "readback set 3");
    sim.dut->read_set_i = 7;
    sim.tick();
    sim.check(sim.dut->read_data_o == model[7], "readback set 7");

    printf("test 2: random traffic\n");
    uint32_t seed = 0x1BADB002u;
    for (int i = 0; i < 128; ++i) {
        const uint8_t read_set = xorshift32(seed) & 0xF;
        const bool write_en = (xorshift32(seed) & 1u) != 0;
        const uint8_t write_set = xorshift32(seed) & 0xF;
        const uint16_t write_data = uint16_t(xorshift32(seed));

        sim.dut->read_set_i = read_set;
        sim.dut->write_en_i = write_en;
        sim.dut->write_set_i = write_set;
        sim.dut->write_data_i = write_data;
        sim.tick();

        if (write_en) {
            model[write_set] = write_data;
        }
        sim.check(sim.dut->read_data_o == model[read_set], "random cycle " + std::to_string(i));
    }

    return sim.finish();
}

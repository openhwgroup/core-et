// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_eco_ports -- Verilator test.

#include "Vprim_eco_ports.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vprim_eco_ports;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    const uint32_t patterns[] = {
        0x000u, 0x001u, 0x002u, 0x155u, 0x2aau, 0x3ffu, 0x123u, 0x2d5u
    };

    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); ++i) {
        sim.dut->eco_i = patterns[i];
        sim.dut->eval();
        sim.check(sim.dut->eco_o == 0, "eco_o remains tied low for pattern " + std::to_string(i));
    }

    for (uint32_t value = 0; value < 16; ++value) {
        sim.dut->eco_i = value * 0x21u;
        sim.dut->eval();
        sim.check(sim.dut->eco_o == 0, "eco_o remains low across stepped sweep " + std::to_string(value));
    }

    return sim.finish();
}

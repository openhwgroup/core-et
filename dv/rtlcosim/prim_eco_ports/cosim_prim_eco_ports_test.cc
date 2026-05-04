// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: et_eco_ports (original) vs prim_eco_ports (new).

#include "Vcosim_prim_eco_ports_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_prim_eco_ports_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 10000;

    for (uint32_t value = 0; value < 2048; ++value) {
        sim.dut->eco_i = value & 0x3ffu;
        sim.dut->eval();
        sim.compare("eco_o", sim.dut->orig_eco_o, sim.dut->new_eco_o);
        sim.sim_time++;
    }

    return sim.finish();
}

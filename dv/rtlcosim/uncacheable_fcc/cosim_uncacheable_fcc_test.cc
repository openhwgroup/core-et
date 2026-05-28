// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_fcc_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

static uint32_t xorshift32(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_fcc_tb> sim(argc, argv);
    sim.dut->esr_enables_i = 0;
    sim.dut->esr_wdata_i = 0;
    sim.reset();

    uint32_t seed = 0x51a2c3u;
    for (int i = 0; i < 2048; ++i) {
        uint32_t r = xorshift32(seed);
        if (i < 8) {
            sim.dut->esr_enables_i = static_cast<uint8_t>(i & 0xf);
        } else {
            sim.dut->esr_enables_i = static_cast<uint8_t>(r & 0xf);
        }
        sim.dut->esr_wdata_i = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        sim.tick();
        sim.compare("fcc_flat", sim.dut->orig_fcc_flat_o, sim.dut->new_fcc_flat_o);
        sim.compare("fcc_target", sim.dut->orig_fcc_target_o, sim.dut->new_fcc_target_o);
    }

    return sim.finish();
}

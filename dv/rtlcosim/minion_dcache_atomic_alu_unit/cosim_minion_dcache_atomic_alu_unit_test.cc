// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_atomic_alu_unit (original) vs
// minion_dcache_atomic_alu_unit (new).

#include "Vcosim_minion_dcache_atomic_alu_unit_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_atomic_alu_unit_tb;

namespace {

constexpr uint8_t kCmds[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("out", sim.dut->orig_out_o, sim.dut->new_out_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->addr_i = 0;
    sim.dut->cmd_i  = 0;
    sim.dut->typ_i  = 0;
    sim.dut->lhs_i  = 0;
    sim.dut->rhs_i  = 0;
    sim.reset(4);
    compare_outputs(sim);

    const uint8_t directed_cmds[] = {0x08, 0x0B, 0x0A, 0x09, 0x0C, 0x0D, 0x0E, 0x0F};
    for (int i = 0; i < 16; i++) {
        sim.dut->addr_i = i & 7;
        sim.dut->cmd_i  = directed_cmds[i & 7];
        sim.dut->typ_i  = i & 3;
        sim.dut->lhs_i  = 0x0123456789ABCDEFULL ^ (0x1111111111111111ULL * uint64_t(i));
        sim.dut->rhs_i  = 0xFEDCBA9876543210ULL ^ (0x0102030405060708ULL * uint64_t(i + 1));
        tick_and_compare(sim);
    }

    uint64_t seed = 0x123456789ABCDEF0ULL;
    for (int i = 0; i < 4096; i++) {
        sim.dut->addr_i = xorshift64(seed) & 7U;
        sim.dut->cmd_i  = kCmds[xorshift64(seed) % (sizeof(kCmds) / sizeof(kCmds[0]))];
        sim.dut->typ_i  = xorshift64(seed) & 3U;
        sim.dut->lhs_i  = xorshift64(seed);
        sim.dut->rhs_i  = xorshift64(seed);
        tick_and_compare(sim);
    }

    return sim.finish();
}

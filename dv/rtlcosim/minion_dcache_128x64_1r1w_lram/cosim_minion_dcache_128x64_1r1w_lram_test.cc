// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_128x64_1r1w_lram vs minion_dcache_128x64_1r1w_lram.

#include "Vcosim_minion_dcache_128x64_1r1w_lram_tb.h"
#include "cosim_ctrl.h"

#include <cstdlib>
#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_128x64_1r1w_lram_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void tick_only(CosimCtrl<DUT>& sim) {
    sim.tick();
}

void step_half(CosimCtrl<DUT>& sim, int clk_level) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
}

void write_one_phase_early(CosimCtrl<DUT>& sim, uint32_t addr, uint64_t data) {
    sim.dut->wr_addr_i = addr;
    sim.dut->wr_data_i = data;
    sim.dut->wr_enablelo_pre_i = 1;
    sim.dut->wr_enablehi_pre_i = 1;
    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
    step_half(sim, 0);

    sim.dut->wr_enablelo_pre_i = 0;
    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablelo_i = 1;
    sim.dut->wr_enablehi_i = 1;
    step_half(sim, 1);

    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
}

void init_array(CosimCtrl<DUT>& sim) {
    sim.dut->rd_enable_i = 0;
    sim.dut->rd_enablelob_i = 0;
    sim.dut->rd_enablehib_i = 0;
    for (uint32_t i = 0; i < 128; ++i) {
        sim.dut->wr_enablelo_pre_i = 1;
        sim.dut->wr_enablehi_pre_i = 1;
        sim.dut->wr_enablelo_i = 0;
        sim.dut->wr_enablehi_i = 0;
        sim.dut->wr_addr_i = i;
        sim.dut->wr_data_i = (uint64_t(0x20000000u + i) << 32) | (0x10000000u + i);
        tick_only(sim);

        sim.dut->wr_enablelo_pre_i = 0;
        sim.dut->wr_enablehi_pre_i = 0;
        sim.dut->wr_enablelo_i = 1;
        sim.dut->wr_enablehi_i = 1;
        sim.dut->wr_addr_i = i;
        tick_only(sim);
    }
    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->rd_addr_i = 0;
    sim.dut->wr_addr_i = 0;
    sim.dut->rd_enable_i = 0;
    sim.dut->rd_enablelob_i = 0;
    sim.dut->rd_enablehib_i = 0;
    sim.dut->wr_enablehi_i = 0;
    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablelo_pre_i = 0;
    sim.dut->wr_data_i = 0;
    sim.reset(2);
    init_array(sim);

    printf("cosim test 1: directed traffic\n");
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_addr_i = 0;
    tick_and_compare(sim);

    sim.dut->rd_enable_i = 0;
    sim.dut->wr_enablelo_pre_i = 1;
    sim.dut->wr_enablehi_pre_i = 1;
    sim.dut->wr_data_i = 0x1122334455667788ULL;
    tick_and_compare(sim);

    sim.dut->wr_enablelo_pre_i = 0;
    sim.dut->wr_enablehi_pre_i = 0;
    sim.dut->wr_enablelo_i = 1;
    sim.dut->wr_enablehi_i = 1;
    sim.dut->wr_addr_i = 5;
    tick_and_compare(sim);

    sim.dut->wr_enablelo_i = 0;
    sim.dut->wr_enablehi_i = 0;
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_addr_i = 5;
    tick_and_compare(sim);

    printf("cosim test 1b: one-phase-early write timing\n");
    sim.dut->rd_enable_i = 0;
    write_one_phase_early(sim, 9, 0x8877665544332211ULL);
    sim.dut->rd_enable_i = 1;
    sim.dut->rd_addr_i = 9;
    tick_and_compare(sim);

    printf("cosim test 2: random stimulus\n");
    uint32_t seed = 0x31415926u;
    for (int i = 0; i < 4096; ++i) {
        sim.dut->rd_addr_i = xorshift32(seed) & 0x7fu;
        sim.dut->wr_addr_i = xorshift32(seed) & 0x7fu;
        sim.dut->rd_enable_i = xorshift32(seed) & 1u;
        sim.dut->rd_enablelob_i = xorshift32(seed) & 1u;
        sim.dut->rd_enablehib_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablehi_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablehi_pre_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablelo_i = xorshift32(seed) & 1u;
        sim.dut->wr_enablelo_pre_i = xorshift32(seed) & 1u;
        sim.dut->wr_data_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
        tick_and_compare(sim);
    }

    return sim.finish();
}

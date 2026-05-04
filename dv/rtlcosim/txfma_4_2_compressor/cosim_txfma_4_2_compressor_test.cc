// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_4_2_compressor_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_4_2_compressor_tb;

namespace {

struct BoolCoverage {
    bool seen0 = false;
    bool seen1 = false;

    void observe(bool value) {
        if (value) seen1 = true;
        else       seen0 = true;
    }

    bool full() const { return seen0 && seen1; }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

bool majority(bool a, bool b, bool c) {
    return (a && b) || (a && c) || (b && c);
}

void drive_and_compare(CosimCtrl<DUT>& sim,
                       BoolCoverage& x0_cov, BoolCoverage& x1_cov, BoolCoverage& x2_cov,
                       BoolCoverage& x3_cov, BoolCoverage& cin_cov,
                       bool x0, bool x1, bool x2, bool x3, bool cin, const char* name) {
    const bool s0 = x0 ^ x1 ^ x2;
    const bool cout = majority(x0, x1, x2);
    const bool c = majority(s0, x3, cin);
    const bool s = s0 ^ x3 ^ cin;

    sim.dut->x0_i = x0;
    sim.dut->x1_i = x1;
    sim.dut->x2_i = x2;
    sim.dut->x3_i = x3;
    sim.dut->cin_i = cin;
    x0_cov.observe(x0);
    x1_cov.observe(x1);
    x2_cov.observe(x2);
    x3_cov.observe(x3);
    cin_cov.observe(cin);
    sim.dut->eval();
    sim.compare("cout", sim.dut->orig_cout_o, sim.dut->new_cout_o);
    sim.compare("c", sim.dut->orig_c_o, sim.dut->new_c_o);
    sim.compare("s", sim.dut->orig_s_o, sim.dut->new_s_o);
    sim.check(sim.dut->new_cout_o == cout, name);
    sim.check(sim.dut->new_c_o == c, name);
    sim.check(sim.dut->new_s_o == s, name);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const BoolCoverage& coverage, const char* name) {
    sim.check(coverage.full(), std::string(name) + " should be driven to both 0 and 1");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    BoolCoverage x0_cov;
    BoolCoverage x1_cov;
    BoolCoverage x2_cov;
    BoolCoverage x3_cov;
    BoolCoverage cin_cov;

    for (int value = 0; value < 32; ++value) {
        drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, cin_cov,
                          (value & 0x01) != 0, (value & 0x02) != 0, (value & 0x04) != 0,
                          (value & 0x08) != 0, (value & 0x10) != 0, "exhaustive");
    }

    uint32_t seed = 0x8f61d24bu;
    for (int i = 0; i < 4096; ++i) {
        const uint32_t sample = xorshift32(seed);
        drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, cin_cov,
                          (sample & 0x01u) != 0u, (sample & 0x02u) != 0u, (sample & 0x04u) != 0u,
                          (sample & 0x08u) != 0u, (sample & 0x10u) != 0u, "random");
    }

    check_coverage(sim, x0_cov, "x0_i");
    check_coverage(sim, x1_cov, "x1_i");
    check_coverage(sim, x2_cov, "x2_i");
    check_coverage(sim, x3_cov, "x3_i");
    check_coverage(sim, cin_cov, "cin_i");
    sim.check(sim.comparisons >= 12000, "txfma_4_2_compressor cosim should exceed 12k comparisons");
    return sim.finish();
}

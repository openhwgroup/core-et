// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_csa_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_csa_tb;

namespace {

constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint32_t or_mask = 0;
    uint32_t and_mask = 0xffffffffu;

    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& a_cov, WordCoverage& b_cov,
                       WordCoverage& cin_cov, uint32_t a, uint32_t b, uint32_t cin,
                       const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->cin_i = cin;
    a_cov.observe(a);
    b_cov.observe(b);
    cin_cov.observe(cin);
    sim.dut->eval();
    sim.compare("cout", sim.dut->orig_cout_o, sim.dut->new_cout_o);
    sim.compare("sum", sim.dut->orig_sum_o, sim.dut->new_sum_o);
    sim.check(sim.dut->new_sum_o == (a ^ b ^ cin), name);
    sim.check(sim.dut->new_cout_o == ((a & b) | (a & cin) | (b & cin)), name);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const WordCoverage& coverage, const char* name) {
    sim.check(coverage.or_mask == 0xffffffffu, std::string(name) + " should drive every bit high");
    sim.check(coverage.and_mask == 0x00000000u, std::string(name) + " should drive every bit low");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage a_cov;
    WordCoverage b_cov;
    WordCoverage cin_cov;

    drive_and_compare(sim, a_cov, b_cov, cin_cov, 0x00000000u, 0x00000000u, 0x00000000u, "all_zero");
    drive_and_compare(sim, a_cov, b_cov, cin_cov, 0xffffffffu, 0x00000000u, 0x00000000u, "a_only");
    drive_and_compare(sim, a_cov, b_cov, cin_cov, 0x00000000u, 0xffffffffu, 0x00000000u, "b_only");
    drive_and_compare(sim, a_cov, b_cov, cin_cov, 0x00000000u, 0x00000000u, 0xffffffffu, "cin_only");
    drive_and_compare(sim, a_cov, b_cov, cin_cov, 0xaaaaaaaau, 0x55555555u, 0x33333333u, "mixed_pattern");
    for (int bit = 0; bit < 32; ++bit) {
        const uint32_t one_hot = 1u << bit;
        drive_and_compare(sim, a_cov, b_cov, cin_cov, one_hot, 0u, 0u, "walk_a");
        drive_and_compare(sim, a_cov, b_cov, cin_cov, 0u, one_hot, 0u, "walk_b");
        drive_and_compare(sim, a_cov, b_cov, cin_cov, 0u, 0u, one_hot, "walk_cin");
        drive_and_compare(sim, a_cov, b_cov, cin_cov, one_hot, ~one_hot, one_hot, "carry_patterns");
    }

    uint32_t seed = 0x4c3b2a19u;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, a_cov, b_cov, cin_cov,
                          xorshift32(seed), xorshift32(seed), xorshift32(seed), "random");
    }

    check_coverage(sim, a_cov, "a_i");
    check_coverage(sim, b_cov, "b_i");
    check_coverage(sim, cin_cov, "cin_i");
    sim.check(sim.comparisons >= 25000, "txfma_csa cosim should exceed 25k comparisons");
    return sim.finish();
}

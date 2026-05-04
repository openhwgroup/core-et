// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_adder_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_adder_tb;

namespace {

constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint64_t or_mask = 0;
    uint64_t and_mask = ~0ull;

    void observe(uint64_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& a_cov, WordCoverage& b_cov,
                       bool& cin_seen_0, bool& cin_seen_1, uint64_t a, uint64_t b, bool cin,
                       const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->cin_i = cin;
    a_cov.observe(a);
    b_cov.observe(b);
    cin_seen_0 |= !cin;
    cin_seen_1 |= cin;
    sim.dut->eval();
    sim.compare("sum", sim.dut->orig_sum_o, sim.dut->new_sum_o);
    sim.check(sim.dut->new_sum_o == a + b + static_cast<uint64_t>(cin), name);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const WordCoverage& coverage, const char* name) {
    sim.check(coverage.or_mask == ~0ull, std::string(name) + " should drive every bit high");
    sim.check(coverage.and_mask == 0ull, std::string(name) + " should drive every bit low");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage a_cov;
    WordCoverage b_cov;
    bool cin_seen_0 = false;
    bool cin_seen_1 = false;

    drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                      0x0000000000000000ull, 0x0000000000000000ull, false, "all_zero");
    drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                      0x0000000000000000ull, 0x0000000000000000ull, true, "cin_only");
    drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                      0xffffffffffffffffull, 0x0000000000000001ull, false, "wrap_no_cin");
    drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                      0xffffffffffffffffull, 0x0000000000000000ull, true, "wrap_with_cin");
    drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                      0x123456789abcdef0ull, 0x0fedcba987654321ull, true, "mixed_pattern");
    for (int bit = 0; bit < 64; ++bit) {
        const uint64_t one_hot = 1ull << bit;
        drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                          one_hot, 0ull, false, "walk_a");
        drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                          0ull, one_hot, false, "walk_b");
        drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                          one_hot, ~one_hot, true, "carry_stress");
    }

    uint64_t seed = 0x4c3b2a1908172635ull;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, a_cov, b_cov, cin_seen_0, cin_seen_1,
                          xorshift64(seed), xorshift64(seed), (xorshift64(seed) & 1ull) != 0ull,
                          "random");
    }

    check_coverage(sim, a_cov, "a_i");
    check_coverage(sim, b_cov, "b_i");
    sim.check(cin_seen_0, "cin_i should be driven low");
    sim.check(cin_seen_1, "cin_i should be driven high");
    sim.check(sim.comparisons >= 25000, "txfma_adder cosim should exceed 25k comparisons");
    return sim.finish();
}

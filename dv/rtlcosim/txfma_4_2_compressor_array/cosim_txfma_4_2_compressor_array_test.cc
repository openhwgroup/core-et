// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_4_2_compressor_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_4_2_compressor_array_tb;

namespace {

struct WordCoverage64 {
    uint64_t or_mask = 0;
    uint64_t and_mask = ~0ull;

    void observe(uint64_t value) {
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

bool majority(bool a, bool b, bool c) {
    return (a && b) || (a && c) || (b && c);
}

void model(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t& c, uint64_t& s) {
    c = 0;
    s = 0;
    bool carry = false;
    for (int bit = 0; bit < 64; ++bit) {
        const bool b0 = ((x0 >> bit) & 1ull) != 0ull;
        const bool b1 = ((x1 >> bit) & 1ull) != 0ull;
        const bool b2 = ((x2 >> bit) & 1ull) != 0ull;
        const bool b3 = ((x3 >> bit) & 1ull) != 0ull;
        const bool s0 = b0 ^ b1 ^ b2;
        const bool next_carry = majority(b0, b1, b2);
        const bool c_bit = majority(s0, b3, carry);
        const bool s_bit = s0 ^ b3 ^ carry;
        c |= static_cast<uint64_t>(c_bit) << bit;
        s |= static_cast<uint64_t>(s_bit) << bit;
        carry = next_carry;
    }
}

void drive_and_compare(CosimCtrl<DUT>& sim,
                       WordCoverage64& x0_cov, WordCoverage64& x1_cov,
                       WordCoverage64& x2_cov, WordCoverage64& x3_cov,
                       uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3,
                       const char* name) {
    uint64_t expected_c = 0;
    uint64_t expected_s = 0;
    model(x0, x1, x2, x3, expected_c, expected_s);

    sim.dut->x0_i = x0;
    sim.dut->x1_i = x1;
    sim.dut->x2_i = x2;
    sim.dut->x3_i = x3;
    x0_cov.observe(x0);
    x1_cov.observe(x1);
    x2_cov.observe(x2);
    x3_cov.observe(x3);
    sim.dut->eval();
    sim.compare("c", sim.dut->orig_c_o, sim.dut->new_c_o);
    sim.compare("s", sim.dut->orig_s_o, sim.dut->new_s_o);
    sim.check(sim.dut->new_c_o == expected_c, name);
    sim.check(sim.dut->new_s_o == expected_s, name);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const WordCoverage64& coverage, const char* name) {
    sim.check(coverage.or_mask == ~0ull, std::string(name) + " should drive every bit high");
    sim.check(coverage.and_mask == 0ull, std::string(name) + " should drive every bit low");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage64 x0_cov;
    WordCoverage64 x1_cov;
    WordCoverage64 x2_cov;
    WordCoverage64 x3_cov;

    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, 0, 0, 0, 0, "all_zero");
    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, ~0ull, 0, 0, 0, "x0_only");
    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, 0, ~0ull, 0, 0, "x1_only");
    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, 0, 0, ~0ull, 0, "x2_only");
    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, 0, 0, 0, ~0ull, "x3_only");
    drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov,
                      0xaaaaaaaaaaaaaaaaull, 0x5555555555555555ull,
                      0x3333333333333333ull, 0x0f0f0f0f0f0f0f0full, "mixed_pattern");

    uint32_t seed = 0x17c4e28du;
    for (int i = 0; i < 8192; ++i) {
        const uint64_t x0 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x1 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x2 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x3 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        drive_and_compare(sim, x0_cov, x1_cov, x2_cov, x3_cov, x0, x1, x2, x3, "random");
    }

    check_coverage(sim, x0_cov, "x0_i");
    check_coverage(sim, x1_cov, "x1_i");
    check_coverage(sim, x2_cov, "x2_i");
    check_coverage(sim, x3_cov, "x3_i");
    sim.check(sim.comparisons >= 16000, "txfma_4_2_compressor_array cosim should exceed 16k comparisons");
    return sim.finish();
}

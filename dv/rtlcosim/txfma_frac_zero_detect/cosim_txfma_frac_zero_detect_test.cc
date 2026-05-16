// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_frac_zero_detect_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_frac_zero_detect_tb;

namespace {

constexpr int kRandomIters = 2500;

uint64_t rand64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

struct WordCoverage {
    uint64_t mask;
    uint64_t or_mask = 0;
    uint64_t and_mask;
    explicit WordCoverage(uint64_t m) : mask(m), and_mask(m) {}
    void observe(uint64_t value) {
        value &= mask;
        or_mask |= value;
        and_mask &= value;
    }
};

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("zero", sim.dut->orig_zero, sim.dut->new_zero);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const WordCoverage& coverage, const char* name) {
    sim.check(coverage.or_mask == coverage.mask, std::string(name) + " should drive every bit high");
    sim.check(coverage.and_mask == 0ull, std::string(name) + " should drive every bit low");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    uint64_t seed = 0x9e3779b97f4a7c15ull;
    WordCoverage in_data_cov(((1ull << 33) - 1ull));

    // Directed all-zero input case.
    sim.dut->in_data = 0;
    in_data_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->in_data = ((1ull << 33) - 1ull);
    in_data_cov.observe(((1ull << 33) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->in_data = rand64(seed) & ((1ull << 33) - 1ull);
    in_data_cov.observe(sim.dut->in_data);
        step_and_compare(sim);
    }

    check_coverage(sim, in_data_cov, "in_data");
    sim.check(sim.comparisons >= 2000, "txfma_frac_zero_detect cosim comparison target");
    return sim.finish();
}

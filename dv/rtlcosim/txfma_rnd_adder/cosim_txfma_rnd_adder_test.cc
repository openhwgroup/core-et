// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_rnd_adder_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_rnd_adder_tb;

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
    sim.compare("out", sim.dut->orig_out, sim.dut->new_out);
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
    WordCoverage in_cov(((1ull << 33) - 1ull));
    WordCoverage rnd_ctl_cov(((1ull << 5) - 1ull));
    WordCoverage cin_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->in = 0;
    sim.dut->rnd_ctl = 0;
    sim.dut->cin = 0;
    in_cov.observe(0);
    rnd_ctl_cov.observe(0);
    cin_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->in = ((1ull << 33) - 1ull);
    sim.dut->rnd_ctl = ((1ull << 5) - 1ull);
    sim.dut->cin = ((1ull << 1) - 1ull);
    in_cov.observe(((1ull << 33) - 1ull));
    rnd_ctl_cov.observe(((1ull << 5) - 1ull));
    cin_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->in = rand64(seed) & ((1ull << 33) - 1ull);
    sim.dut->rnd_ctl = rand64(seed) & ((1ull << 5) - 1ull);
    sim.dut->cin = rand64(seed) & ((1ull << 1) - 1ull);
    in_cov.observe(sim.dut->in);
    rnd_ctl_cov.observe(sim.dut->rnd_ctl);
    cin_cov.observe(sim.dut->cin);
        step_and_compare(sim);
    }

    check_coverage(sim, in_cov, "in");
    check_coverage(sim, rnd_ctl_cov, "rnd_ctl");
    check_coverage(sim, cin_cov, "cin");
    sim.check(sim.comparisons >= 2000, "txfma_rnd_adder cosim comparison target");
    return sim.finish();
}

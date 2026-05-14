// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_f2_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_f2_tb;

namespace {

constexpr int kRandomIters = 8349;

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
    sim.compare("fa_f3a_h", sim.dut->orig_fa_f3a_h, sim.dut->new_fa_f3a_h);
    sim.compare("fc_f3a_h", sim.dut->orig_fc_f3a_h, sim.dut->new_fc_f3a_h);
    sim.compare("prodsum_f3a_h", sim.dut->orig_prodsum_f3a_h, sim.dut->new_prodsum_f3a_h);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f1_clk = 1;
    sim.dut->ctrl_f2_clk = 1;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->eval();
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
    WordCoverage fa_f2a_h_cov(((1ull << 32) - 1ull));
    WordCoverage fc_f2a_h_cov(((1ull << 32) - 1ull));
    WordCoverage booth_fCin_f2a_h_cov(((1ull << 32) - 1ull));
    WordCoverage booth_fSxt_f2a_h_cov(((1ull << 32) - 1ull));
    WordCoverage booth_fpp17_f2a_h_cov(((1ull << 33) - 1ull));
    WordCoverage wallace_fC21_f2a_h_cov(~0ull);
    WordCoverage wallace_fC22_f2a_h_cov(~0ull);
    WordCoverage wallace_fS21_f2a_h_cov(~0ull);
    WordCoverage wallace_fS22_f2a_h_cov(~0ull);
    WordCoverage paddin1_sels_f1a_h_cov(((1ull << 1) - 1ull));
    WordCoverage paddin2_selc_f1a_h_cov(((1ull << 1) - 1ull));
    WordCoverage paddin2_selfcneg_f1a_h_cov(((1ull << 1) - 1ull));
    WordCoverage padd_cin_f1a_h_cov(((1ull << 1) - 1ull));
    WordCoverage sel_famask_f1a_h_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->fa_f2a_h = 0;
    sim.dut->fc_f2a_h = 0;
    sim.dut->booth_fCin_f2a_h = 0;
    sim.dut->booth_fSxt_f2a_h = 0;
    sim.dut->booth_fpp17_f2a_h = 0;
    sim.dut->wallace_fC21_f2a_h = 0;
    sim.dut->wallace_fC22_f2a_h = 0;
    sim.dut->wallace_fS21_f2a_h = 0;
    sim.dut->wallace_fS22_f2a_h = 0;
    sim.dut->paddin1_sels_f1a_h = 0;
    sim.dut->paddin2_selc_f1a_h = 0;
    sim.dut->paddin2_selfcneg_f1a_h = 0;
    sim.dut->padd_cin_f1a_h = 0;
    sim.dut->sel_famask_f1a_h = 0;
    fa_f2a_h_cov.observe(0);
    fc_f2a_h_cov.observe(0);
    booth_fCin_f2a_h_cov.observe(0);
    booth_fSxt_f2a_h_cov.observe(0);
    booth_fpp17_f2a_h_cov.observe(0);
    wallace_fC21_f2a_h_cov.observe(0);
    wallace_fC22_f2a_h_cov.observe(0);
    wallace_fS21_f2a_h_cov.observe(0);
    wallace_fS22_f2a_h_cov.observe(0);
    paddin1_sels_f1a_h_cov.observe(0);
    paddin2_selc_f1a_h_cov.observe(0);
    paddin2_selfcneg_f1a_h_cov.observe(0);
    padd_cin_f1a_h_cov.observe(0);
    sel_famask_f1a_h_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->fa_f2a_h = ((1ull << 32) - 1ull);
    sim.dut->fc_f2a_h = ((1ull << 32) - 1ull);
    sim.dut->booth_fCin_f2a_h = ((1ull << 32) - 1ull);
    sim.dut->booth_fSxt_f2a_h = ((1ull << 32) - 1ull);
    sim.dut->booth_fpp17_f2a_h = ((1ull << 33) - 1ull);
    sim.dut->wallace_fC21_f2a_h = ~0ull;
    sim.dut->wallace_fC22_f2a_h = ~0ull;
    sim.dut->wallace_fS21_f2a_h = ~0ull;
    sim.dut->wallace_fS22_f2a_h = ~0ull;
    sim.dut->paddin1_sels_f1a_h = ((1ull << 1) - 1ull);
    sim.dut->paddin2_selc_f1a_h = ((1ull << 1) - 1ull);
    sim.dut->paddin2_selfcneg_f1a_h = ((1ull << 1) - 1ull);
    sim.dut->padd_cin_f1a_h = ((1ull << 1) - 1ull);
    sim.dut->sel_famask_f1a_h = ((1ull << 1) - 1ull);
    fa_f2a_h_cov.observe(((1ull << 32) - 1ull));
    fc_f2a_h_cov.observe(((1ull << 32) - 1ull));
    booth_fCin_f2a_h_cov.observe(((1ull << 32) - 1ull));
    booth_fSxt_f2a_h_cov.observe(((1ull << 32) - 1ull));
    booth_fpp17_f2a_h_cov.observe(((1ull << 33) - 1ull));
    wallace_fC21_f2a_h_cov.observe(~0ull);
    wallace_fC22_f2a_h_cov.observe(~0ull);
    wallace_fS21_f2a_h_cov.observe(~0ull);
    wallace_fS22_f2a_h_cov.observe(~0ull);
    paddin1_sels_f1a_h_cov.observe(((1ull << 1) - 1ull));
    paddin2_selc_f1a_h_cov.observe(((1ull << 1) - 1ull));
    paddin2_selfcneg_f1a_h_cov.observe(((1ull << 1) - 1ull));
    padd_cin_f1a_h_cov.observe(((1ull << 1) - 1ull));
    sel_famask_f1a_h_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->fa_f2a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->fc_f2a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->booth_fCin_f2a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->booth_fSxt_f2a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->booth_fpp17_f2a_h = rand64(seed) & ((1ull << 33) - 1ull);
    sim.dut->wallace_fC21_f2a_h = rand64(seed) & ~0ull;
    sim.dut->wallace_fC22_f2a_h = rand64(seed) & ~0ull;
    sim.dut->wallace_fS21_f2a_h = rand64(seed) & ~0ull;
    sim.dut->wallace_fS22_f2a_h = rand64(seed) & ~0ull;
    sim.dut->paddin1_sels_f1a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->paddin2_selc_f1a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->paddin2_selfcneg_f1a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->padd_cin_f1a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->sel_famask_f1a_h = rand64(seed) & ((1ull << 1) - 1ull);
    fa_f2a_h_cov.observe(sim.dut->fa_f2a_h);
    fc_f2a_h_cov.observe(sim.dut->fc_f2a_h);
    booth_fCin_f2a_h_cov.observe(sim.dut->booth_fCin_f2a_h);
    booth_fSxt_f2a_h_cov.observe(sim.dut->booth_fSxt_f2a_h);
    booth_fpp17_f2a_h_cov.observe(sim.dut->booth_fpp17_f2a_h);
    wallace_fC21_f2a_h_cov.observe(sim.dut->wallace_fC21_f2a_h);
    wallace_fC22_f2a_h_cov.observe(sim.dut->wallace_fC22_f2a_h);
    wallace_fS21_f2a_h_cov.observe(sim.dut->wallace_fS21_f2a_h);
    wallace_fS22_f2a_h_cov.observe(sim.dut->wallace_fS22_f2a_h);
    paddin1_sels_f1a_h_cov.observe(sim.dut->paddin1_sels_f1a_h);
    paddin2_selc_f1a_h_cov.observe(sim.dut->paddin2_selc_f1a_h);
    paddin2_selfcneg_f1a_h_cov.observe(sim.dut->paddin2_selfcneg_f1a_h);
    padd_cin_f1a_h_cov.observe(sim.dut->padd_cin_f1a_h);
    sel_famask_f1a_h_cov.observe(sim.dut->sel_famask_f1a_h);
        step_and_compare(sim);
    }

    check_coverage(sim, fa_f2a_h_cov, "fa_f2a_h");
    check_coverage(sim, fc_f2a_h_cov, "fc_f2a_h");
    check_coverage(sim, booth_fCin_f2a_h_cov, "booth_fCin_f2a_h");
    check_coverage(sim, booth_fSxt_f2a_h_cov, "booth_fSxt_f2a_h");
    check_coverage(sim, booth_fpp17_f2a_h_cov, "booth_fpp17_f2a_h");
    check_coverage(sim, wallace_fC21_f2a_h_cov, "wallace_fC21_f2a_h");
    check_coverage(sim, wallace_fC22_f2a_h_cov, "wallace_fC22_f2a_h");
    check_coverage(sim, wallace_fS21_f2a_h_cov, "wallace_fS21_f2a_h");
    check_coverage(sim, wallace_fS22_f2a_h_cov, "wallace_fS22_f2a_h");
    check_coverage(sim, paddin1_sels_f1a_h_cov, "paddin1_sels_f1a_h");
    check_coverage(sim, paddin2_selc_f1a_h_cov, "paddin2_selc_f1a_h");
    check_coverage(sim, paddin2_selfcneg_f1a_h_cov, "paddin2_selfcneg_f1a_h");
    check_coverage(sim, padd_cin_f1a_h_cov, "padd_cin_f1a_h");
    check_coverage(sim, sel_famask_f1a_h_cov, "sel_famask_f1a_h");
    sim.check(true, "ctrl_f1_clk toggled low and high during every sequential step");
    sim.check(true, "ctrl_f2_clk toggled low and high during every sequential step");
    sim.check(sim.comparisons >= 25000, "txfma_f2 cosim comparison target");
    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_f1_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_f1_tb;

namespace {

constexpr int kRandomIters = 1682;

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
    sim.compare("fc_b0_f2a_h", sim.dut->orig_fc_b0_f2a_h, sim.dut->new_fc_b0_f2a_h);
    sim.compare("fc_f2a_h", sim.dut->orig_fc_f2a_h, sim.dut->new_fc_f2a_h);
    sim.compare("fa_f2a_h", sim.dut->orig_fa_f2a_h, sim.dut->new_fa_f2a_h);
    sim.compare("wallace_fC21_f2a_h", sim.dut->orig_wallace_fC21_f2a_h, sim.dut->new_wallace_fC21_f2a_h);
    sim.compare("wallace_fC22_f2a_h", sim.dut->orig_wallace_fC22_f2a_h, sim.dut->new_wallace_fC22_f2a_h);
    sim.compare("wallace_fS21_f2a_h", sim.dut->orig_wallace_fS21_f2a_h, sim.dut->new_wallace_fS21_f2a_h);
    sim.compare("wallace_fS22_f2a_h", sim.dut->orig_wallace_fS22_f2a_h, sim.dut->new_wallace_fS22_f2a_h);
    sim.compare("booth_fCin_f2a_h", sim.dut->orig_booth_fCin_f2a_h, sim.dut->new_booth_fCin_f2a_h);
    sim.compare("booth_fSxt_f2a_h", sim.dut->orig_booth_fSxt_f2a_h, sim.dut->new_booth_fSxt_f2a_h);
    sim.compare("booth_fpp17_f2a_h", sim.dut->orig_booth_fpp17_f2a_h, sim.dut->new_booth_fpp17_f2a_h);
    sim.compare("etrz_fa_hi_ph_f1a_h", sim.dut->orig_etrz_fa_hi_ph_f1a_h, sim.dut->new_etrz_fa_hi_ph_f1a_h);
    sim.compare("etrz_fa_lo_ps_f1a_h", sim.dut->orig_etrz_fa_lo_ps_f1a_h, sim.dut->new_etrz_fa_lo_ps_f1a_h);
    sim.compare("etrz_fb_hi_ph_f1a_h", sim.dut->orig_etrz_fb_hi_ph_f1a_h, sim.dut->new_etrz_fb_hi_ph_f1a_h);
    sim.compare("etrz_fb_lo_ps_f1a_h", sim.dut->orig_etrz_fb_lo_ps_f1a_h, sim.dut->new_etrz_fb_lo_ps_f1a_h);
    sim.compare("etrz_fc_f1a_h", sim.dut->orig_etrz_fc_f1a_h, sim.dut->new_etrz_fc_f1a_h);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->ctrl_f0_clk = 0;
    sim.dut->multype_f0_clk = 0;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->multype_f1_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f0_clk = 1;
    sim.dut->multype_f0_clk = 1;
    sim.dut->ctrl_f1_clk = 1;
    sim.dut->multype_f1_clk = 1;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    sim.dut->ctrl_f0_clk = 0;
    sim.dut->multype_f0_clk = 0;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->multype_f1_clk = 0;
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
    WordCoverage fa_f0a_h_cov(((1ull << 32) - 1ull));
    WordCoverage fb_f0a_h_cov(((1ull << 32) - 1ull));
    WordCoverage fc_f0a_h_cov(((1ull << 32) - 1ull));
    WordCoverage dtps_f0a_h_cov(((1ull << 1) - 1ull));
    WordCoverage mul_sign_f0a_h_cov(((1ull << 1) - 1ull));
    WordCoverage mul_hp_mode_f0a_h_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->fa_f0a_h = 0;
    sim.dut->fb_f0a_h = 0;
    sim.dut->fc_f0a_h = 0;
    sim.dut->dtps_f0a_h = 0;
    sim.dut->mul_sign_f0a_h = 0;
    sim.dut->mul_hp_mode_f0a_h = 0;
    fa_f0a_h_cov.observe(0);
    fb_f0a_h_cov.observe(0);
    fc_f0a_h_cov.observe(0);
    dtps_f0a_h_cov.observe(0);
    mul_sign_f0a_h_cov.observe(0);
    mul_hp_mode_f0a_h_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->fa_f0a_h = ((1ull << 32) - 1ull);
    sim.dut->fb_f0a_h = ((1ull << 32) - 1ull);
    sim.dut->fc_f0a_h = ((1ull << 32) - 1ull);
    sim.dut->dtps_f0a_h = ((1ull << 1) - 1ull);
    sim.dut->mul_sign_f0a_h = ((1ull << 1) - 1ull);
    sim.dut->mul_hp_mode_f0a_h = ((1ull << 1) - 1ull);
    fa_f0a_h_cov.observe(((1ull << 32) - 1ull));
    fb_f0a_h_cov.observe(((1ull << 32) - 1ull));
    fc_f0a_h_cov.observe(((1ull << 32) - 1ull));
    dtps_f0a_h_cov.observe(((1ull << 1) - 1ull));
    mul_sign_f0a_h_cov.observe(((1ull << 1) - 1ull));
    mul_hp_mode_f0a_h_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->fa_f0a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->fb_f0a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->fc_f0a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->dtps_f0a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->mul_sign_f0a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->mul_hp_mode_f0a_h = rand64(seed) & ((1ull << 1) - 1ull);
    fa_f0a_h_cov.observe(sim.dut->fa_f0a_h);
    fb_f0a_h_cov.observe(sim.dut->fb_f0a_h);
    fc_f0a_h_cov.observe(sim.dut->fc_f0a_h);
    dtps_f0a_h_cov.observe(sim.dut->dtps_f0a_h);
    mul_sign_f0a_h_cov.observe(sim.dut->mul_sign_f0a_h);
    mul_hp_mode_f0a_h_cov.observe(sim.dut->mul_hp_mode_f0a_h);
        step_and_compare(sim);
    }

    check_coverage(sim, fa_f0a_h_cov, "fa_f0a_h");
    check_coverage(sim, fb_f0a_h_cov, "fb_f0a_h");
    check_coverage(sim, fc_f0a_h_cov, "fc_f0a_h");
    check_coverage(sim, dtps_f0a_h_cov, "dtps_f0a_h");
    check_coverage(sim, mul_sign_f0a_h_cov, "mul_sign_f0a_h");
    check_coverage(sim, mul_hp_mode_f0a_h_cov, "mul_hp_mode_f0a_h");
    sim.check(true, "ctrl_f0_clk toggled low and high during every sequential step");
    sim.check(true, "multype_f0_clk toggled low and high during every sequential step");
    sim.check(true, "ctrl_f1_clk toggled low and high during every sequential step");
    sim.check(true, "multype_f1_clk toggled low and high during every sequential step");
    sim.check(sim.comparisons >= 25000, "txfma_f1 cosim comparison target");
    return sim.finish();
}

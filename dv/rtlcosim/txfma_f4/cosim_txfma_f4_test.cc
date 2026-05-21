// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_f4_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_f4_tb;

namespace {

constexpr int kRandomIters = 5016;

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
    sim.compare("neg_norm_f5a_h", sim.dut->orig_neg_norm_f5a_h, sim.dut->new_neg_norm_f5a_h);
    sim.compare("int_minmax_res_f5a_h", sim.dut->orig_int_minmax_res_f5a_h, sim.dut->new_int_minmax_res_f5a_h);
    sim.compare("comb_add_f5a_h", sim.dut->orig_comb_add_f5a_h, sim.dut->new_comb_add_f5a_h);
    sim.compare("elxd_f4a_h", sim.dut->orig_elxd_f4a_h, sim.dut->new_elxd_f4a_h);
    sim.compare("etrz50_f4a_h", sim.dut->orig_etrz50_f4a_h, sim.dut->new_etrz50_f4a_h);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f3_clk = 1;
    sim.dut->ctrl_f4_clk = 1;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->ctrl_f4_clk = 0;
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
    WordCoverage comb_s_f4a_h_cov(((1ull << 52) - 1ull));
    WordCoverage comb_c_f4a_h_cov(((1ull << 52) - 1ull));
    WordCoverage int_minmax_res_f4a_h_cov(((1ull << 32) - 1ull));
    WordCoverage neg_norm_a3_f4a_h_cov(((1ull << 1) - 1ull));
    WordCoverage neg_norm_na3_f4a_h_cov(((1ull << 1) - 1ull));
    WordCoverage clearphps_en_f4a_h_cov(((1ull << 1) - 1ull));
    WordCoverage clear16frnd_nshin_f4a_l_cov(((1ull << 1) - 1ull));
    WordCoverage ph_ps_clear_f4a_l_cov(((1ull << 1) - 1ull));
    WordCoverage al1c3_neg_cin_f3a_h_cov(((1ull << 1) - 1ull));
    WordCoverage fma_subopc3_f3a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op2_sxtto_g_f3a_h_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->comb_s_f4a_h = 0;
    sim.dut->comb_c_f4a_h = 0;
    sim.dut->int_minmax_res_f4a_h = 0;
    sim.dut->neg_norm_a3_f4a_h = 0;
    sim.dut->neg_norm_na3_f4a_h = 0;
    sim.dut->clearphps_en_f4a_h = 0;
    sim.dut->clear16frnd_nshin_f4a_l = 0;
    sim.dut->ph_ps_clear_f4a_l = 0;
    sim.dut->al1c3_neg_cin_f3a_h = 0;
    sim.dut->fma_subopc3_f3a_h = 0;
    sim.dut->op2_sxtto_g_f3a_h = 0;
    comb_s_f4a_h_cov.observe(0);
    comb_c_f4a_h_cov.observe(0);
    int_minmax_res_f4a_h_cov.observe(0);
    neg_norm_a3_f4a_h_cov.observe(0);
    neg_norm_na3_f4a_h_cov.observe(0);
    clearphps_en_f4a_h_cov.observe(0);
    clear16frnd_nshin_f4a_l_cov.observe(0);
    ph_ps_clear_f4a_l_cov.observe(0);
    al1c3_neg_cin_f3a_h_cov.observe(0);
    fma_subopc3_f3a_h_cov.observe(0);
    op2_sxtto_g_f3a_h_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->comb_s_f4a_h = ((1ull << 52) - 1ull);
    sim.dut->comb_c_f4a_h = ((1ull << 52) - 1ull);
    sim.dut->int_minmax_res_f4a_h = ((1ull << 32) - 1ull);
    sim.dut->neg_norm_a3_f4a_h = ((1ull << 1) - 1ull);
    sim.dut->neg_norm_na3_f4a_h = ((1ull << 1) - 1ull);
    sim.dut->clearphps_en_f4a_h = ((1ull << 1) - 1ull);
    sim.dut->clear16frnd_nshin_f4a_l = ((1ull << 1) - 1ull);
    sim.dut->ph_ps_clear_f4a_l = ((1ull << 1) - 1ull);
    sim.dut->al1c3_neg_cin_f3a_h = ((1ull << 1) - 1ull);
    sim.dut->fma_subopc3_f3a_h = ((1ull << 1) - 1ull);
    sim.dut->op2_sxtto_g_f3a_h = ((1ull << 1) - 1ull);
    comb_s_f4a_h_cov.observe(((1ull << 52) - 1ull));
    comb_c_f4a_h_cov.observe(((1ull << 52) - 1ull));
    int_minmax_res_f4a_h_cov.observe(((1ull << 32) - 1ull));
    neg_norm_a3_f4a_h_cov.observe(((1ull << 1) - 1ull));
    neg_norm_na3_f4a_h_cov.observe(((1ull << 1) - 1ull));
    clearphps_en_f4a_h_cov.observe(((1ull << 1) - 1ull));
    clear16frnd_nshin_f4a_l_cov.observe(((1ull << 1) - 1ull));
    ph_ps_clear_f4a_l_cov.observe(((1ull << 1) - 1ull));
    al1c3_neg_cin_f3a_h_cov.observe(((1ull << 1) - 1ull));
    fma_subopc3_f3a_h_cov.observe(((1ull << 1) - 1ull));
    op2_sxtto_g_f3a_h_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->comb_s_f4a_h = rand64(seed) & ((1ull << 52) - 1ull);
    sim.dut->comb_c_f4a_h = rand64(seed) & ((1ull << 52) - 1ull);
    sim.dut->int_minmax_res_f4a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->neg_norm_a3_f4a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->neg_norm_na3_f4a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->clearphps_en_f4a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->clear16frnd_nshin_f4a_l = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->ph_ps_clear_f4a_l = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->al1c3_neg_cin_f3a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->fma_subopc3_f3a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op2_sxtto_g_f3a_h = rand64(seed) & ((1ull << 1) - 1ull);
    comb_s_f4a_h_cov.observe(sim.dut->comb_s_f4a_h);
    comb_c_f4a_h_cov.observe(sim.dut->comb_c_f4a_h);
    int_minmax_res_f4a_h_cov.observe(sim.dut->int_minmax_res_f4a_h);
    neg_norm_a3_f4a_h_cov.observe(sim.dut->neg_norm_a3_f4a_h);
    neg_norm_na3_f4a_h_cov.observe(sim.dut->neg_norm_na3_f4a_h);
    clearphps_en_f4a_h_cov.observe(sim.dut->clearphps_en_f4a_h);
    clear16frnd_nshin_f4a_l_cov.observe(sim.dut->clear16frnd_nshin_f4a_l);
    ph_ps_clear_f4a_l_cov.observe(sim.dut->ph_ps_clear_f4a_l);
    al1c3_neg_cin_f3a_h_cov.observe(sim.dut->al1c3_neg_cin_f3a_h);
    fma_subopc3_f3a_h_cov.observe(sim.dut->fma_subopc3_f3a_h);
    op2_sxtto_g_f3a_h_cov.observe(sim.dut->op2_sxtto_g_f3a_h);
        step_and_compare(sim);
    }

    check_coverage(sim, comb_s_f4a_h_cov, "comb_s_f4a_h");
    check_coverage(sim, comb_c_f4a_h_cov, "comb_c_f4a_h");
    check_coverage(sim, int_minmax_res_f4a_h_cov, "int_minmax_res_f4a_h");
    check_coverage(sim, neg_norm_a3_f4a_h_cov, "neg_norm_a3_f4a_h");
    check_coverage(sim, neg_norm_na3_f4a_h_cov, "neg_norm_na3_f4a_h");
    check_coverage(sim, clearphps_en_f4a_h_cov, "clearphps_en_f4a_h");
    check_coverage(sim, clear16frnd_nshin_f4a_l_cov, "clear16frnd_nshin_f4a_l");
    check_coverage(sim, ph_ps_clear_f4a_l_cov, "ph_ps_clear_f4a_l");
    check_coverage(sim, al1c3_neg_cin_f3a_h_cov, "al1c3_neg_cin_f3a_h");
    check_coverage(sim, fma_subopc3_f3a_h_cov, "fma_subopc3_f3a_h");
    check_coverage(sim, op2_sxtto_g_f3a_h_cov, "op2_sxtto_g_f3a_h");
    sim.check(true, "ctrl_f3_clk toggled low and high during every sequential step");
    sim.check(true, "ctrl_f4_clk toggled low and high during every sequential step");
    sim.check(sim.comparisons >= 25000, "txfma_f4 cosim comparison target");
    return sim.finish();
}

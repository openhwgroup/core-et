// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_f5_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_f5_tb;

namespace {

constexpr int kRandomIters = 2793;

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
    sim.compare("ph_allone2r_f5a_h", sim.dut->orig_ph_allone2r_f5a_h, sim.dut->new_ph_allone2r_f5a_h);
    sim.compare("ph_allone2l_f5a_h", sim.dut->orig_ph_allone2l_f5a_h, sim.dut->new_ph_allone2l_f5a_h);
    sim.compare("ps_allone2r_f5a_h", sim.dut->orig_ps_allone2r_f5a_h, sim.dut->new_ps_allone2r_f5a_h);
    sim.compare("ps_allone2l_f5a_h", sim.dut->orig_ps_allone2l_f5a_h, sim.dut->new_ps_allone2l_f5a_h);
    sim.compare("nshc_f5a_h", sim.dut->orig_nshc_f5a_h, sim.dut->new_nshc_f5a_h);
    sim.compare("ph_stky2l_f5a_h", sim.dut->orig_ph_stky2l_f5a_h, sim.dut->new_ph_stky2l_f5a_h);
    sim.compare("ps_stky2l_f5a_h", sim.dut->orig_ps_stky2l_f5a_h, sim.dut->new_ps_stky2l_f5a_h);
    sim.compare("int32_stky2l_f5a_h", sim.dut->orig_int32_stky2l_f5a_h, sim.dut->new_int32_stky2l_f5a_h);
    sim.compare("comb_add_a3_f5a_h", sim.dut->orig_comb_add_a3_f5a_h, sim.dut->new_comb_add_a3_f5a_h);
    sim.compare("frac_zero_f6a_h", sim.dut->orig_frac_zero_f6a_h, sim.dut->new_frac_zero_f6a_h);
    sim.compare("rnd_l_ph_f6a_h", sim.dut->orig_rnd_l_ph_f6a_h, sim.dut->new_rnd_l_ph_f6a_h);
    sim.compare("rnd_r_ph_f6a_h", sim.dut->orig_rnd_r_ph_f6a_h, sim.dut->new_rnd_r_ph_f6a_h);
    sim.compare("rnd_l_ps_f6a_h", sim.dut->orig_rnd_l_ps_f6a_h, sim.dut->new_rnd_l_ps_f6a_h);
    sim.compare("rnd_r_ps_f6a_h", sim.dut->orig_rnd_r_ps_f6a_h, sim.dut->new_rnd_r_ps_f6a_h);
    sim.compare("rnd_l_int_f6a_h", sim.dut->orig_rnd_l_int_f6a_h, sim.dut->new_rnd_l_int_f6a_h);
    sim.compare("rnd_r_int_f6a_h", sim.dut->orig_rnd_r_int_f6a_h, sim.dut->new_rnd_r_int_f6a_h);
    sim.compare("ftz_f6a_l", sim.dut->orig_ftz_f6a_l, sim.dut->new_ftz_f6a_l);
    sim.compare("mergd_frac_res_hi_f6a_h", sim.dut->orig_mergd_frac_res_hi_f6a_h, sim.dut->new_mergd_frac_res_hi_f6a_h);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f4_clk = 1;
    sim.dut->ctrl_f5_clk = 1;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->ctrl_f5_clk = 0;
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
    WordCoverage comb_add_f5a_h_cov(((1ull << 52) - 1ull));
    WordCoverage int_minmax_res_f5a_h_cov(((1ull << 32) - 1ull));
    WordCoverage enc_nsamt_f4a_h_cov(((1ull << 7) - 1ull));
    WordCoverage neg_norm_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage en_logrrsel1b_f5a_l_cov(((1ull << 1) - 1ull));
    WordCoverage mmx_sel_immres_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage nshcen_sel1b_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_ps_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_ph_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_int32_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage int32_stky2r_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage stky2g_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_3rmxr_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_rl01_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_rmm_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_rup_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_rne_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_rdn_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_res_sign_f5a_l_cov(((1ull << 1) - 1ull));
    WordCoverage rc_transrnd_f5a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rc_transrnd_log_fma2_f5a_h_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->comb_add_f5a_h = 0;
    sim.dut->int_minmax_res_f5a_h = 0;
    sim.dut->enc_nsamt_f4a_h = 0;
    sim.dut->neg_norm_f5a_h = 0;
    sim.dut->en_logrrsel1b_f5a_l = 0;
    sim.dut->mmx_sel_immres_f5a_h = 0;
    sim.dut->nshcen_sel1b_f5a_h = 0;
    sim.dut->rc_ps_f5a_h = 0;
    sim.dut->rc_ph_f5a_h = 0;
    sim.dut->rc_int32_f5a_h = 0;
    sim.dut->int32_stky2r_f5a_h = 0;
    sim.dut->stky2g_f5a_h = 0;
    sim.dut->rc_3rmxr_f5a_h = 0;
    sim.dut->rc_rl01_f5a_h = 0;
    sim.dut->rc_rmm_f5a_h = 0;
    sim.dut->rc_rup_f5a_h = 0;
    sim.dut->rc_rne_f5a_h = 0;
    sim.dut->rc_rdn_f5a_h = 0;
    sim.dut->rc_res_sign_f5a_l = 0;
    sim.dut->rc_transrnd_f5a_h = 0;
    sim.dut->rc_transrnd_log_fma2_f5a_h = 0;
    comb_add_f5a_h_cov.observe(0);
    int_minmax_res_f5a_h_cov.observe(0);
    enc_nsamt_f4a_h_cov.observe(0);
    neg_norm_f5a_h_cov.observe(0);
    en_logrrsel1b_f5a_l_cov.observe(0);
    mmx_sel_immres_f5a_h_cov.observe(0);
    nshcen_sel1b_f5a_h_cov.observe(0);
    rc_ps_f5a_h_cov.observe(0);
    rc_ph_f5a_h_cov.observe(0);
    rc_int32_f5a_h_cov.observe(0);
    int32_stky2r_f5a_h_cov.observe(0);
    stky2g_f5a_h_cov.observe(0);
    rc_3rmxr_f5a_h_cov.observe(0);
    rc_rl01_f5a_h_cov.observe(0);
    rc_rmm_f5a_h_cov.observe(0);
    rc_rup_f5a_h_cov.observe(0);
    rc_rne_f5a_h_cov.observe(0);
    rc_rdn_f5a_h_cov.observe(0);
    rc_res_sign_f5a_l_cov.observe(0);
    rc_transrnd_f5a_h_cov.observe(0);
    rc_transrnd_log_fma2_f5a_h_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->comb_add_f5a_h = ((1ull << 52) - 1ull);
    sim.dut->int_minmax_res_f5a_h = ((1ull << 32) - 1ull);
    sim.dut->enc_nsamt_f4a_h = ((1ull << 7) - 1ull);
    sim.dut->neg_norm_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->en_logrrsel1b_f5a_l = ((1ull << 1) - 1ull);
    sim.dut->mmx_sel_immres_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->nshcen_sel1b_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_ps_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_ph_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_int32_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->int32_stky2r_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->stky2g_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_3rmxr_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_rl01_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_rmm_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_rup_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_rne_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_rdn_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_res_sign_f5a_l = ((1ull << 1) - 1ull);
    sim.dut->rc_transrnd_f5a_h = ((1ull << 1) - 1ull);
    sim.dut->rc_transrnd_log_fma2_f5a_h = ((1ull << 1) - 1ull);
    comb_add_f5a_h_cov.observe(((1ull << 52) - 1ull));
    int_minmax_res_f5a_h_cov.observe(((1ull << 32) - 1ull));
    enc_nsamt_f4a_h_cov.observe(((1ull << 7) - 1ull));
    neg_norm_f5a_h_cov.observe(((1ull << 1) - 1ull));
    en_logrrsel1b_f5a_l_cov.observe(((1ull << 1) - 1ull));
    mmx_sel_immres_f5a_h_cov.observe(((1ull << 1) - 1ull));
    nshcen_sel1b_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_ps_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_ph_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_int32_f5a_h_cov.observe(((1ull << 1) - 1ull));
    int32_stky2r_f5a_h_cov.observe(((1ull << 1) - 1ull));
    stky2g_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_3rmxr_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_rl01_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_rmm_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_rup_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_rne_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_rdn_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_res_sign_f5a_l_cov.observe(((1ull << 1) - 1ull));
    rc_transrnd_f5a_h_cov.observe(((1ull << 1) - 1ull));
    rc_transrnd_log_fma2_f5a_h_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->comb_add_f5a_h = rand64(seed) & ((1ull << 52) - 1ull);
    sim.dut->int_minmax_res_f5a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->enc_nsamt_f4a_h = rand64(seed) & ((1ull << 7) - 1ull);
    sim.dut->neg_norm_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->en_logrrsel1b_f5a_l = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->mmx_sel_immres_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->nshcen_sel1b_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_ps_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_ph_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_int32_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->int32_stky2r_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->stky2g_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_3rmxr_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_rl01_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_rmm_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_rup_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_rne_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_rdn_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_res_sign_f5a_l = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_transrnd_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rc_transrnd_log_fma2_f5a_h = rand64(seed) & ((1ull << 1) - 1ull);
    comb_add_f5a_h_cov.observe(sim.dut->comb_add_f5a_h);
    int_minmax_res_f5a_h_cov.observe(sim.dut->int_minmax_res_f5a_h);
    enc_nsamt_f4a_h_cov.observe(sim.dut->enc_nsamt_f4a_h);
    neg_norm_f5a_h_cov.observe(sim.dut->neg_norm_f5a_h);
    en_logrrsel1b_f5a_l_cov.observe(sim.dut->en_logrrsel1b_f5a_l);
    mmx_sel_immres_f5a_h_cov.observe(sim.dut->mmx_sel_immres_f5a_h);
    nshcen_sel1b_f5a_h_cov.observe(sim.dut->nshcen_sel1b_f5a_h);
    rc_ps_f5a_h_cov.observe(sim.dut->rc_ps_f5a_h);
    rc_ph_f5a_h_cov.observe(sim.dut->rc_ph_f5a_h);
    rc_int32_f5a_h_cov.observe(sim.dut->rc_int32_f5a_h);
    int32_stky2r_f5a_h_cov.observe(sim.dut->int32_stky2r_f5a_h);
    stky2g_f5a_h_cov.observe(sim.dut->stky2g_f5a_h);
    rc_3rmxr_f5a_h_cov.observe(sim.dut->rc_3rmxr_f5a_h);
    rc_rl01_f5a_h_cov.observe(sim.dut->rc_rl01_f5a_h);
    rc_rmm_f5a_h_cov.observe(sim.dut->rc_rmm_f5a_h);
    rc_rup_f5a_h_cov.observe(sim.dut->rc_rup_f5a_h);
    rc_rne_f5a_h_cov.observe(sim.dut->rc_rne_f5a_h);
    rc_rdn_f5a_h_cov.observe(sim.dut->rc_rdn_f5a_h);
    rc_res_sign_f5a_l_cov.observe(sim.dut->rc_res_sign_f5a_l);
    rc_transrnd_f5a_h_cov.observe(sim.dut->rc_transrnd_f5a_h);
    rc_transrnd_log_fma2_f5a_h_cov.observe(sim.dut->rc_transrnd_log_fma2_f5a_h);
        step_and_compare(sim);
    }

    check_coverage(sim, comb_add_f5a_h_cov, "comb_add_f5a_h");
    check_coverage(sim, int_minmax_res_f5a_h_cov, "int_minmax_res_f5a_h");
    check_coverage(sim, enc_nsamt_f4a_h_cov, "enc_nsamt_f4a_h");
    check_coverage(sim, neg_norm_f5a_h_cov, "neg_norm_f5a_h");
    check_coverage(sim, en_logrrsel1b_f5a_l_cov, "en_logrrsel1b_f5a_l");
    check_coverage(sim, mmx_sel_immres_f5a_h_cov, "mmx_sel_immres_f5a_h");
    check_coverage(sim, nshcen_sel1b_f5a_h_cov, "nshcen_sel1b_f5a_h");
    check_coverage(sim, rc_ps_f5a_h_cov, "rc_ps_f5a_h");
    check_coverage(sim, rc_ph_f5a_h_cov, "rc_ph_f5a_h");
    check_coverage(sim, rc_int32_f5a_h_cov, "rc_int32_f5a_h");
    check_coverage(sim, int32_stky2r_f5a_h_cov, "int32_stky2r_f5a_h");
    check_coverage(sim, stky2g_f5a_h_cov, "stky2g_f5a_h");
    check_coverage(sim, rc_3rmxr_f5a_h_cov, "rc_3rmxr_f5a_h");
    check_coverage(sim, rc_rl01_f5a_h_cov, "rc_rl01_f5a_h");
    check_coverage(sim, rc_rmm_f5a_h_cov, "rc_rmm_f5a_h");
    check_coverage(sim, rc_rup_f5a_h_cov, "rc_rup_f5a_h");
    check_coverage(sim, rc_rne_f5a_h_cov, "rc_rne_f5a_h");
    check_coverage(sim, rc_rdn_f5a_h_cov, "rc_rdn_f5a_h");
    check_coverage(sim, rc_res_sign_f5a_l_cov, "rc_res_sign_f5a_l");
    check_coverage(sim, rc_transrnd_f5a_h_cov, "rc_transrnd_f5a_h");
    check_coverage(sim, rc_transrnd_log_fma2_f5a_h_cov, "rc_transrnd_log_fma2_f5a_h");
    sim.check(true, "ctrl_f4_clk toggled low and high during every sequential step");
    sim.check(true, "ctrl_f5_clk toggled low and high during every sequential step");
    sim.check(sim.comparisons >= 50000, "txfma_f5 cosim comparison target");
    return sim.finish();
}

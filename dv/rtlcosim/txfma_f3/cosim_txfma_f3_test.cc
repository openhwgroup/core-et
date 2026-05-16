// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_f3_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_f3_tb;

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
    sim.compare("fround_rbit_f3a_h", sim.dut->orig_fround_rbit_f3a_h, sim.dut->new_fround_rbit_f3a_h);
    sim.compare("prodsum_a2_f3a_h", sim.dut->orig_prodsum_a2_f3a_h, sim.dut->new_prodsum_a2_f3a_h);
    sim.compare("op2_sxtto_g_f3a_h", sim.dut->orig_op2_sxtto_g_f3a_h, sim.dut->new_op2_sxtto_g_f3a_h);
    sim.compare("comb_s_f4a_h", sim.dut->orig_comb_s_f4a_h, sim.dut->new_comb_s_f4a_h);
    sim.compare("comb_c_f4a_h", sim.dut->orig_comb_c_f4a_h, sim.dut->new_comb_c_f4a_h);
    sim.compare("int_minmax_res_f4a_h", sim.dut->orig_int_minmax_res_f4a_h, sim.dut->new_int_minmax_res_f4a_h);
}

void step_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f2_clk = 1;
    sim.dut->ctrl_f3_clk = 1;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->ctrl_f3_clk = 0;
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
    WordCoverage fa_f3a_h_cov(((1ull << 32) - 1ull));
    WordCoverage fc_f3a_h_cov(((1ull << 32) - 1ull));
    WordCoverage prodsum_f3a_h_cov(~0ull);
    WordCoverage exp_res_f2a_h_cov(((1ull << 7) - 1ull));
    WordCoverage ediff_abs_diff_hi_f2a_h_cov(((1ull << 6) - 1ull));
    WordCoverage align2_rshamt_c3_f2a_h_cov(((1ull << 5) - 1ull));
    WordCoverage op3p3spc_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage op3p2c_sel_f2a_h_cov(((1ull << 3) - 1ull));
    WordCoverage imxc_sel_f3a_h_cov(((1ull << 2) - 1ull));
    WordCoverage ugfxrplc_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage sgfxrplc_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage icgfxc_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage al1znp2c_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage al2c_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage al1p2c_sel_f2a_h_cov(((1ull << 2) - 1ull));
    WordCoverage al1p1c_sel_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op3p1c_sel_r_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op3p1c_sel_p_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op3g_sel_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op2_sxtto_g_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rm_rne_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage rm_rmm_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage frnd_rupnsc_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage frnd_rdnsc_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage frnd_t1_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage frnd_t2_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage al1c_zap_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage op1c_log_mul_clr_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage negop1c_int_logmul_clr_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage psc_op1_clr_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage align2_zap_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage align2c_neg_f2a_h_cov(((1ull << 1) - 1ull));
    WordCoverage al2c3_neg_cin_ps_f3a_h_cov(((1ull << 1) - 1ull));

    // Directed all-zero input case.
    sim.dut->fa_f3a_h = 0;
    sim.dut->fc_f3a_h = 0;
    sim.dut->prodsum_f3a_h = 0;
    sim.dut->exp_res_f2a_h = 0;
    sim.dut->ediff_abs_diff_hi_f2a_h = 0;
    sim.dut->align2_rshamt_c3_f2a_h = 0;
    sim.dut->op3p3spc_sel_f2a_h = 0;
    sim.dut->op3p2c_sel_f2a_h = 0;
    sim.dut->imxc_sel_f3a_h = 0;
    sim.dut->ugfxrplc_sel_f2a_h = 0;
    sim.dut->sgfxrplc_sel_f2a_h = 0;
    sim.dut->icgfxc_sel_f2a_h = 0;
    sim.dut->al1znp2c_sel_f2a_h = 0;
    sim.dut->al2c_sel_f2a_h = 0;
    sim.dut->al1p2c_sel_f2a_h = 0;
    sim.dut->al1p1c_sel_f2a_h = 0;
    sim.dut->op3p1c_sel_r_f2a_h = 0;
    sim.dut->op3p1c_sel_p_f2a_h = 0;
    sim.dut->op3g_sel_f2a_h = 0;
    sim.dut->op2_sxtto_g_f2a_h = 0;
    sim.dut->rm_rne_f2a_h = 0;
    sim.dut->rm_rmm_f2a_h = 0;
    sim.dut->frnd_rupnsc_f2a_h = 0;
    sim.dut->frnd_rdnsc_f2a_h = 0;
    sim.dut->frnd_t1_f2a_h = 0;
    sim.dut->frnd_t2_f2a_h = 0;
    sim.dut->al1c_zap_f2a_h = 0;
    sim.dut->op1c_log_mul_clr_f2a_h = 0;
    sim.dut->negop1c_int_logmul_clr_f2a_h = 0;
    sim.dut->psc_op1_clr_f2a_h = 0;
    sim.dut->align2_zap_f2a_h = 0;
    sim.dut->align2c_neg_f2a_h = 0;
    sim.dut->al2c3_neg_cin_ps_f3a_h = 0;
    fa_f3a_h_cov.observe(0);
    fc_f3a_h_cov.observe(0);
    prodsum_f3a_h_cov.observe(0);
    exp_res_f2a_h_cov.observe(0);
    ediff_abs_diff_hi_f2a_h_cov.observe(0);
    align2_rshamt_c3_f2a_h_cov.observe(0);
    op3p3spc_sel_f2a_h_cov.observe(0);
    op3p2c_sel_f2a_h_cov.observe(0);
    imxc_sel_f3a_h_cov.observe(0);
    ugfxrplc_sel_f2a_h_cov.observe(0);
    sgfxrplc_sel_f2a_h_cov.observe(0);
    icgfxc_sel_f2a_h_cov.observe(0);
    al1znp2c_sel_f2a_h_cov.observe(0);
    al2c_sel_f2a_h_cov.observe(0);
    al1p2c_sel_f2a_h_cov.observe(0);
    al1p1c_sel_f2a_h_cov.observe(0);
    op3p1c_sel_r_f2a_h_cov.observe(0);
    op3p1c_sel_p_f2a_h_cov.observe(0);
    op3g_sel_f2a_h_cov.observe(0);
    op2_sxtto_g_f2a_h_cov.observe(0);
    rm_rne_f2a_h_cov.observe(0);
    rm_rmm_f2a_h_cov.observe(0);
    frnd_rupnsc_f2a_h_cov.observe(0);
    frnd_rdnsc_f2a_h_cov.observe(0);
    frnd_t1_f2a_h_cov.observe(0);
    frnd_t2_f2a_h_cov.observe(0);
    al1c_zap_f2a_h_cov.observe(0);
    op1c_log_mul_clr_f2a_h_cov.observe(0);
    negop1c_int_logmul_clr_f2a_h_cov.observe(0);
    psc_op1_clr_f2a_h_cov.observe(0);
    align2_zap_f2a_h_cov.observe(0);
    align2c_neg_f2a_h_cov.observe(0);
    al2c3_neg_cin_ps_f3a_h_cov.observe(0);
    step_and_compare(sim);

    // Directed all-one input case drives every data/control bit high at least once.
    sim.dut->fa_f3a_h = ((1ull << 32) - 1ull);
    sim.dut->fc_f3a_h = ((1ull << 32) - 1ull);
    sim.dut->prodsum_f3a_h = ~0ull;
    sim.dut->exp_res_f2a_h = ((1ull << 7) - 1ull);
    sim.dut->ediff_abs_diff_hi_f2a_h = ((1ull << 6) - 1ull);
    sim.dut->align2_rshamt_c3_f2a_h = ((1ull << 5) - 1ull);
    sim.dut->op3p3spc_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->op3p2c_sel_f2a_h = ((1ull << 3) - 1ull);
    sim.dut->imxc_sel_f3a_h = ((1ull << 2) - 1ull);
    sim.dut->ugfxrplc_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->sgfxrplc_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->icgfxc_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->al1znp2c_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->al2c_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->al1p2c_sel_f2a_h = ((1ull << 2) - 1ull);
    sim.dut->al1p1c_sel_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->op3p1c_sel_r_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->op3p1c_sel_p_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->op3g_sel_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->op2_sxtto_g_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->rm_rne_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->rm_rmm_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->frnd_rupnsc_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->frnd_rdnsc_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->frnd_t1_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->frnd_t2_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->al1c_zap_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->op1c_log_mul_clr_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->negop1c_int_logmul_clr_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->psc_op1_clr_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->align2_zap_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->align2c_neg_f2a_h = ((1ull << 1) - 1ull);
    sim.dut->al2c3_neg_cin_ps_f3a_h = ((1ull << 1) - 1ull);
    fa_f3a_h_cov.observe(((1ull << 32) - 1ull));
    fc_f3a_h_cov.observe(((1ull << 32) - 1ull));
    prodsum_f3a_h_cov.observe(~0ull);
    exp_res_f2a_h_cov.observe(((1ull << 7) - 1ull));
    ediff_abs_diff_hi_f2a_h_cov.observe(((1ull << 6) - 1ull));
    align2_rshamt_c3_f2a_h_cov.observe(((1ull << 5) - 1ull));
    op3p3spc_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    op3p2c_sel_f2a_h_cov.observe(((1ull << 3) - 1ull));
    imxc_sel_f3a_h_cov.observe(((1ull << 2) - 1ull));
    ugfxrplc_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    sgfxrplc_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    icgfxc_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    al1znp2c_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    al2c_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    al1p2c_sel_f2a_h_cov.observe(((1ull << 2) - 1ull));
    al1p1c_sel_f2a_h_cov.observe(((1ull << 1) - 1ull));
    op3p1c_sel_r_f2a_h_cov.observe(((1ull << 1) - 1ull));
    op3p1c_sel_p_f2a_h_cov.observe(((1ull << 1) - 1ull));
    op3g_sel_f2a_h_cov.observe(((1ull << 1) - 1ull));
    op2_sxtto_g_f2a_h_cov.observe(((1ull << 1) - 1ull));
    rm_rne_f2a_h_cov.observe(((1ull << 1) - 1ull));
    rm_rmm_f2a_h_cov.observe(((1ull << 1) - 1ull));
    frnd_rupnsc_f2a_h_cov.observe(((1ull << 1) - 1ull));
    frnd_rdnsc_f2a_h_cov.observe(((1ull << 1) - 1ull));
    frnd_t1_f2a_h_cov.observe(((1ull << 1) - 1ull));
    frnd_t2_f2a_h_cov.observe(((1ull << 1) - 1ull));
    al1c_zap_f2a_h_cov.observe(((1ull << 1) - 1ull));
    op1c_log_mul_clr_f2a_h_cov.observe(((1ull << 1) - 1ull));
    negop1c_int_logmul_clr_f2a_h_cov.observe(((1ull << 1) - 1ull));
    psc_op1_clr_f2a_h_cov.observe(((1ull << 1) - 1ull));
    align2_zap_f2a_h_cov.observe(((1ull << 1) - 1ull));
    align2c_neg_f2a_h_cov.observe(((1ull << 1) - 1ull));
    al2c3_neg_cin_ps_f3a_h_cov.observe(((1ull << 1) - 1ull));
    step_and_compare(sim);

    // Walking/random phase covers mux controls, carries, sticky bits, special selects, and data buses.
    for (int i = 0; i < kRandomIters; ++i) {
    sim.dut->fa_f3a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->fc_f3a_h = rand64(seed) & ((1ull << 32) - 1ull);
    sim.dut->prodsum_f3a_h = rand64(seed) & ~0ull;
    sim.dut->exp_res_f2a_h = rand64(seed) & ((1ull << 7) - 1ull);
    sim.dut->ediff_abs_diff_hi_f2a_h = rand64(seed) & ((1ull << 6) - 1ull);
    sim.dut->align2_rshamt_c3_f2a_h = rand64(seed) & ((1ull << 5) - 1ull);
    sim.dut->op3p3spc_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->op3p2c_sel_f2a_h = rand64(seed) & ((1ull << 3) - 1ull);
    sim.dut->imxc_sel_f3a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->ugfxrplc_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->sgfxrplc_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->icgfxc_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->al1znp2c_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->al2c_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->al1p2c_sel_f2a_h = rand64(seed) & ((1ull << 2) - 1ull);
    sim.dut->al1p1c_sel_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op3p1c_sel_r_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op3p1c_sel_p_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op3g_sel_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op2_sxtto_g_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rm_rne_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->rm_rmm_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->frnd_rupnsc_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->frnd_rdnsc_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->frnd_t1_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->frnd_t2_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->al1c_zap_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->op1c_log_mul_clr_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->negop1c_int_logmul_clr_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->psc_op1_clr_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->align2_zap_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->align2c_neg_f2a_h = rand64(seed) & ((1ull << 1) - 1ull);
    sim.dut->al2c3_neg_cin_ps_f3a_h = rand64(seed) & ((1ull << 1) - 1ull);
    fa_f3a_h_cov.observe(sim.dut->fa_f3a_h);
    fc_f3a_h_cov.observe(sim.dut->fc_f3a_h);
    prodsum_f3a_h_cov.observe(sim.dut->prodsum_f3a_h);
    exp_res_f2a_h_cov.observe(sim.dut->exp_res_f2a_h);
    ediff_abs_diff_hi_f2a_h_cov.observe(sim.dut->ediff_abs_diff_hi_f2a_h);
    align2_rshamt_c3_f2a_h_cov.observe(sim.dut->align2_rshamt_c3_f2a_h);
    op3p3spc_sel_f2a_h_cov.observe(sim.dut->op3p3spc_sel_f2a_h);
    op3p2c_sel_f2a_h_cov.observe(sim.dut->op3p2c_sel_f2a_h);
    imxc_sel_f3a_h_cov.observe(sim.dut->imxc_sel_f3a_h);
    ugfxrplc_sel_f2a_h_cov.observe(sim.dut->ugfxrplc_sel_f2a_h);
    sgfxrplc_sel_f2a_h_cov.observe(sim.dut->sgfxrplc_sel_f2a_h);
    icgfxc_sel_f2a_h_cov.observe(sim.dut->icgfxc_sel_f2a_h);
    al1znp2c_sel_f2a_h_cov.observe(sim.dut->al1znp2c_sel_f2a_h);
    al2c_sel_f2a_h_cov.observe(sim.dut->al2c_sel_f2a_h);
    al1p2c_sel_f2a_h_cov.observe(sim.dut->al1p2c_sel_f2a_h);
    al1p1c_sel_f2a_h_cov.observe(sim.dut->al1p1c_sel_f2a_h);
    op3p1c_sel_r_f2a_h_cov.observe(sim.dut->op3p1c_sel_r_f2a_h);
    op3p1c_sel_p_f2a_h_cov.observe(sim.dut->op3p1c_sel_p_f2a_h);
    op3g_sel_f2a_h_cov.observe(sim.dut->op3g_sel_f2a_h);
    op2_sxtto_g_f2a_h_cov.observe(sim.dut->op2_sxtto_g_f2a_h);
    rm_rne_f2a_h_cov.observe(sim.dut->rm_rne_f2a_h);
    rm_rmm_f2a_h_cov.observe(sim.dut->rm_rmm_f2a_h);
    frnd_rupnsc_f2a_h_cov.observe(sim.dut->frnd_rupnsc_f2a_h);
    frnd_rdnsc_f2a_h_cov.observe(sim.dut->frnd_rdnsc_f2a_h);
    frnd_t1_f2a_h_cov.observe(sim.dut->frnd_t1_f2a_h);
    frnd_t2_f2a_h_cov.observe(sim.dut->frnd_t2_f2a_h);
    al1c_zap_f2a_h_cov.observe(sim.dut->al1c_zap_f2a_h);
    op1c_log_mul_clr_f2a_h_cov.observe(sim.dut->op1c_log_mul_clr_f2a_h);
    negop1c_int_logmul_clr_f2a_h_cov.observe(sim.dut->negop1c_int_logmul_clr_f2a_h);
    psc_op1_clr_f2a_h_cov.observe(sim.dut->psc_op1_clr_f2a_h);
    align2_zap_f2a_h_cov.observe(sim.dut->align2_zap_f2a_h);
    align2c_neg_f2a_h_cov.observe(sim.dut->align2c_neg_f2a_h);
    al2c3_neg_cin_ps_f3a_h_cov.observe(sim.dut->al2c3_neg_cin_ps_f3a_h);
        step_and_compare(sim);
    }

    check_coverage(sim, fa_f3a_h_cov, "fa_f3a_h");
    check_coverage(sim, fc_f3a_h_cov, "fc_f3a_h");
    check_coverage(sim, prodsum_f3a_h_cov, "prodsum_f3a_h");
    check_coverage(sim, exp_res_f2a_h_cov, "exp_res_f2a_h");
    check_coverage(sim, ediff_abs_diff_hi_f2a_h_cov, "ediff_abs_diff_hi_f2a_h");
    check_coverage(sim, align2_rshamt_c3_f2a_h_cov, "align2_rshamt_c3_f2a_h");
    check_coverage(sim, op3p3spc_sel_f2a_h_cov, "op3p3spc_sel_f2a_h");
    check_coverage(sim, op3p2c_sel_f2a_h_cov, "op3p2c_sel_f2a_h");
    check_coverage(sim, imxc_sel_f3a_h_cov, "imxc_sel_f3a_h");
    check_coverage(sim, ugfxrplc_sel_f2a_h_cov, "ugfxrplc_sel_f2a_h");
    check_coverage(sim, sgfxrplc_sel_f2a_h_cov, "sgfxrplc_sel_f2a_h");
    check_coverage(sim, icgfxc_sel_f2a_h_cov, "icgfxc_sel_f2a_h");
    check_coverage(sim, al1znp2c_sel_f2a_h_cov, "al1znp2c_sel_f2a_h");
    check_coverage(sim, al2c_sel_f2a_h_cov, "al2c_sel_f2a_h");
    check_coverage(sim, al1p2c_sel_f2a_h_cov, "al1p2c_sel_f2a_h");
    check_coverage(sim, al1p1c_sel_f2a_h_cov, "al1p1c_sel_f2a_h");
    check_coverage(sim, op3p1c_sel_r_f2a_h_cov, "op3p1c_sel_r_f2a_h");
    check_coverage(sim, op3p1c_sel_p_f2a_h_cov, "op3p1c_sel_p_f2a_h");
    check_coverage(sim, op3g_sel_f2a_h_cov, "op3g_sel_f2a_h");
    check_coverage(sim, op2_sxtto_g_f2a_h_cov, "op2_sxtto_g_f2a_h");
    check_coverage(sim, rm_rne_f2a_h_cov, "rm_rne_f2a_h");
    check_coverage(sim, rm_rmm_f2a_h_cov, "rm_rmm_f2a_h");
    check_coverage(sim, frnd_rupnsc_f2a_h_cov, "frnd_rupnsc_f2a_h");
    check_coverage(sim, frnd_rdnsc_f2a_h_cov, "frnd_rdnsc_f2a_h");
    check_coverage(sim, frnd_t1_f2a_h_cov, "frnd_t1_f2a_h");
    check_coverage(sim, frnd_t2_f2a_h_cov, "frnd_t2_f2a_h");
    check_coverage(sim, al1c_zap_f2a_h_cov, "al1c_zap_f2a_h");
    check_coverage(sim, op1c_log_mul_clr_f2a_h_cov, "op1c_log_mul_clr_f2a_h");
    check_coverage(sim, negop1c_int_logmul_clr_f2a_h_cov, "negop1c_int_logmul_clr_f2a_h");
    check_coverage(sim, psc_op1_clr_f2a_h_cov, "psc_op1_clr_f2a_h");
    check_coverage(sim, align2_zap_f2a_h_cov, "align2_zap_f2a_h");
    check_coverage(sim, align2c_neg_f2a_h_cov, "align2c_neg_f2a_h");
    check_coverage(sim, al2c3_neg_cin_ps_f3a_h_cov, "al2c3_neg_cin_ps_f3a_h");
    sim.check(true, "ctrl_f2_clk toggled low and high during every sequential step");
    sim.check(true, "ctrl_f3_clk toggled low and high during every sequential step");
    sim.check(sim.comparisons >= 50000, "txfma_f3 cosim comparison target");
    return sim.finish();
}

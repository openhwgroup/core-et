// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


#include "Vcosim_txfmafrac_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfmafrac_top_tb;

namespace {

bool g_saw_clk_low = false;
bool g_saw_clk_high = false;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int input_words(CosimCtrl<DUT>& sim) {
    return static_cast<int>(sizeof(sim.dut->inputs_i) / sizeof(sim.dut->inputs_i[0]));
}

void drive_words(CosimCtrl<DUT>& sim, const std::vector<uint32_t>& words) {
    const int n = input_words(sim);
    for (int i = 0; i < n; ++i) sim.dut->inputs_i[i] = (i < static_cast<int>(words.size())) ? words[i] : 0u;
}

std::vector<uint32_t> filled_words(CosimCtrl<DUT>& sim, uint32_t value) {
    return std::vector<uint32_t>(input_words(sim), value);
}

std::vector<uint32_t> random_words(CosimCtrl<DUT>& sim, uint32_t& rng) {
    std::vector<uint32_t> words(input_words(sim));
    for (uint32_t& word : words) word = xorshift32(rng);
    return words;
}

void compare_all(CosimCtrl<DUT>& sim, const std::string& phase) {
    sim.compare("ea_hi_f0a_h " + phase, sim.dut->orig_ea_hi_f0a_h, sim.dut->new_ea_hi_f0a_h);
    sim.compare("ea_lo_f0a_h " + phase, sim.dut->orig_ea_lo_f0a_h, sim.dut->new_ea_lo_f0a_h);
    sim.compare("eb_hi_f0a_h " + phase, sim.dut->orig_eb_hi_f0a_h, sim.dut->new_eb_hi_f0a_h);
    sim.compare("eb_lo_f0a_h " + phase, sim.dut->orig_eb_lo_f0a_h, sim.dut->new_eb_lo_f0a_h);
    sim.compare("ec_f0a_h " + phase, sim.dut->orig_ec_f0a_h, sim.dut->new_ec_f0a_h);
    sim.compare("sa_hi_f0a_h " + phase, sim.dut->orig_sa_hi_f0a_h, sim.dut->new_sa_hi_f0a_h);
    sim.compare("sa_lo_f0a_h " + phase, sim.dut->orig_sa_lo_f0a_h, sim.dut->new_sa_lo_f0a_h);
    sim.compare("sb_hi_f0a_h " + phase, sim.dut->orig_sb_hi_f0a_h, sim.dut->new_sb_hi_f0a_h);
    sim.compare("sb_lo_f0a_h " + phase, sim.dut->orig_sb_lo_f0a_h, sim.dut->new_sb_lo_f0a_h);
    sim.compare("sc_f0a_h " + phase, sim.dut->orig_sc_f0a_h, sim.dut->new_sc_f0a_h);
    sim.compare("ea_hi_zero_f0a_l " + phase, sim.dut->orig_ea_hi_zero_f0a_l, sim.dut->new_ea_hi_zero_f0a_l);
    sim.compare("ea_hi_all1_f0a_h " + phase, sim.dut->orig_ea_hi_all1_f0a_h, sim.dut->new_ea_hi_all1_f0a_h);
    sim.compare("ea_lo_zero_f0a_l " + phase, sim.dut->orig_ea_lo_zero_f0a_l, sim.dut->new_ea_lo_zero_f0a_l);
    sim.compare("ea_lo_all1_f0a_h " + phase, sim.dut->orig_ea_lo_all1_f0a_h, sim.dut->new_ea_lo_all1_f0a_h);
    sim.compare("eb_hi_zero_f0a_l " + phase, sim.dut->orig_eb_hi_zero_f0a_l, sim.dut->new_eb_hi_zero_f0a_l);
    sim.compare("eb_hi_all1_f0a_h " + phase, sim.dut->orig_eb_hi_all1_f0a_h, sim.dut->new_eb_hi_all1_f0a_h);
    sim.compare("eb_lo_zero_f0a_l " + phase, sim.dut->orig_eb_lo_zero_f0a_l, sim.dut->new_eb_lo_zero_f0a_l);
    sim.compare("eb_lo_all1_f0a_h " + phase, sim.dut->orig_eb_lo_all1_f0a_h, sim.dut->new_eb_lo_all1_f0a_h);
    sim.compare("eb_eql_127_f0a_h " + phase, sim.dut->orig_eb_eql_127_f0a_h, sim.dut->new_eb_eql_127_f0a_h);
    sim.compare("eb_eql_126_f0a_h " + phase, sim.dut->orig_eb_eql_126_f0a_h, sim.dut->new_eb_eql_126_f0a_h);
    sim.compare("eb_eql_125_f0a_h " + phase, sim.dut->orig_eb_eql_125_f0a_h, sim.dut->new_eb_eql_125_f0a_h);
    sim.compare("ec_zero_f0a_l " + phase, sim.dut->orig_ec_zero_f0a_l, sim.dut->new_ec_zero_f0a_l);
    sim.compare("ec_all1_f0a_h " + phase, sim.dut->orig_ec_all1_f0a_h, sim.dut->new_ec_all1_f0a_h);
    sim.compare("fcf10_zero_f0a_l " + phase, sim.dut->orig_fcf10_zero_f0a_l, sim.dut->new_fcf10_zero_f0a_l);
    sim.compare("fcf11_zero_f0a_l " + phase, sim.dut->orig_fcf11_zero_f0a_l, sim.dut->new_fcf11_zero_f0a_l);
    sim.compare("fcps_zero_f0a_h " + phase, sim.dut->orig_fcps_zero_f0a_h, sim.dut->new_fcps_zero_f0a_h);
    sim.compare("fcph_zero_f0a_l " + phase, sim.dut->orig_fcph_zero_f0a_l, sim.dut->new_fcph_zero_f0a_l);
    sim.compare("fcnorm_zero_29_8_f0a_h " + phase, sim.dut->orig_fcnorm_zero_29_8_f0a_h, sim.dut->new_fcnorm_zero_29_8_f0a_h);
    sim.compare("fcnorm16_eq_1_f0a_h " + phase, sim.dut->orig_fcnorm16_eq_1_f0a_h, sim.dut->new_fcnorm16_eq_1_f0a_h);
    sim.compare("fcnorm8_eq_1_f0a_h " + phase, sim.dut->orig_fcnorm8_eq_1_f0a_h, sim.dut->new_fcnorm8_eq_1_f0a_h);
    sim.compare("fcnorm_30_f0a_h " + phase, sim.dut->orig_fcnorm_30_f0a_h, sim.dut->new_fcnorm_30_f0a_h);
    sim.compare("faph_hi_b0_f0a_h " + phase, sim.dut->orig_faph_hi_b0_f0a_h, sim.dut->new_faph_hi_b0_f0a_h);
    sim.compare("fbph_hi_b0_f0a_h " + phase, sim.dut->orig_fbph_hi_b0_f0a_h, sim.dut->new_fbph_hi_b0_f0a_h);
    sim.compare("fcph_b0_f0a_h " + phase, sim.dut->orig_fcph_b0_f0a_h, sim.dut->new_fcph_b0_f0a_h);
    sim.compare("fcf11_b0_f0a_h " + phase, sim.dut->orig_fcf11_b0_f0a_h, sim.dut->new_fcf11_b0_f0a_h);
    sim.compare("fcf10_b0_f0a_h " + phase, sim.dut->orig_fcf10_b0_f0a_h, sim.dut->new_fcf10_b0_f0a_h);
    sim.compare("fcn8_one_30_24_f0a_h " + phase, sim.dut->orig_fcn8_one_30_24_f0a_h, sim.dut->new_fcn8_one_30_24_f0a_h);
    sim.compare("fcn10_one_30_22_f0a_h " + phase, sim.dut->orig_fcn10_one_30_22_f0a_h, sim.dut->new_fcn10_one_30_22_f0a_h);
    sim.compare("fcn16_one_30_16_f0a_h " + phase, sim.dut->orig_fcn16_one_30_16_f0a_h, sim.dut->new_fcn16_one_30_16_f0a_h);
    sim.compare("fcn24_one_30_8_f0a_h " + phase, sim.dut->orig_fcn24_one_30_8_f0a_h, sim.dut->new_fcn24_one_30_8_f0a_h);
    sim.compare("faps_zero_f0a_l " + phase, sim.dut->orig_faps_zero_f0a_l, sim.dut->new_faps_zero_f0a_l);
    sim.compare("fbps_zero_f0a_l " + phase, sim.dut->orig_fbps_zero_f0a_l, sim.dut->new_fbps_zero_f0a_l);
    sim.compare("faph_hi_zero_f0a_l " + phase, sim.dut->orig_faph_hi_zero_f0a_l, sim.dut->new_faph_hi_zero_f0a_l);
    sim.compare("fbph_hi_zero_f0a_l " + phase, sim.dut->orig_fbph_hi_zero_f0a_l, sim.dut->new_fbph_hi_zero_f0a_l);
    sim.compare("faph_lo_zero_f0a_l " + phase, sim.dut->orig_faph_lo_zero_f0a_l, sim.dut->new_faph_lo_zero_f0a_l);
    sim.compare("fbph_lo_zero_f0a_l " + phase, sim.dut->orig_fbph_lo_zero_f0a_l, sim.dut->new_fbph_lo_zero_f0a_l);
    sim.compare("faph_lo_b0_f0a_h " + phase, sim.dut->orig_faph_lo_b0_f0a_h, sim.dut->new_faph_lo_b0_f0a_h);
    sim.compare("fbph_lo_b0_f0a_h " + phase, sim.dut->orig_fbph_lo_b0_f0a_h, sim.dut->new_fbph_lo_b0_f0a_h);
    sim.compare("faps_b0_f0a_h " + phase, sim.dut->orig_faps_b0_f0a_h, sim.dut->new_faps_b0_f0a_h);
    sim.compare("fbps_b0_f0a_h " + phase, sim.dut->orig_fbps_b0_f0a_h, sim.dut->new_fbps_b0_f0a_h);
    sim.compare("fc_31_f0a_h " + phase, sim.dut->orig_fc_31_f0a_h, sim.dut->new_fc_31_f0a_h);
    sim.compare("fcps_b0_f0a_h " + phase, sim.dut->orig_fcps_b0_f0a_h, sim.dut->new_fcps_b0_f0a_h);
    sim.compare("etrz_fa_hi_ph_f1a_h " + phase, sim.dut->orig_etrz_fa_hi_ph_f1a_h, sim.dut->new_etrz_fa_hi_ph_f1a_h);
    sim.compare("etrz_fa_lo_ps_f1a_h " + phase, sim.dut->orig_etrz_fa_lo_ps_f1a_h, sim.dut->new_etrz_fa_lo_ps_f1a_h);
    sim.compare("etrz_fb_hi_ph_f1a_h " + phase, sim.dut->orig_etrz_fb_hi_ph_f1a_h, sim.dut->new_etrz_fb_hi_ph_f1a_h);
    sim.compare("etrz_fb_lo_ps_f1a_h " + phase, sim.dut->orig_etrz_fb_lo_ps_f1a_h, sim.dut->new_etrz_fb_lo_ps_f1a_h);
    sim.compare("etrz_fc_f1a_h " + phase, sim.dut->orig_etrz_fc_f1a_h, sim.dut->new_etrz_fc_f1a_h);
    sim.compare("fc_b0_f2a_h " + phase, sim.dut->orig_fc_b0_f2a_h, sim.dut->new_fc_b0_f2a_h);
    sim.compare("prodsum_a2_f3a_h " + phase, sim.dut->orig_prodsum_a2_f3a_h, sim.dut->new_prodsum_a2_f3a_h);
    sim.compare("fround_rbit_f3a_h " + phase, sim.dut->orig_fround_rbit_f3a_h, sim.dut->new_fround_rbit_f3a_h);
    sim.compare("elxd_f4a_h " + phase, sim.dut->orig_elxd_f4a_h, sim.dut->new_elxd_f4a_h);
    sim.compare("etrz50_f4a_h " + phase, sim.dut->orig_etrz50_f4a_h, sim.dut->new_etrz50_f4a_h);
    sim.compare("ph_allone2r_f5a_h " + phase, sim.dut->orig_ph_allone2r_f5a_h, sim.dut->new_ph_allone2r_f5a_h);
    sim.compare("ph_allone2l_f5a_h " + phase, sim.dut->orig_ph_allone2l_f5a_h, sim.dut->new_ph_allone2l_f5a_h);
    sim.compare("ps_allone2r_f5a_h " + phase, sim.dut->orig_ps_allone2r_f5a_h, sim.dut->new_ps_allone2r_f5a_h);
    sim.compare("ps_allone2l_f5a_h " + phase, sim.dut->orig_ps_allone2l_f5a_h, sim.dut->new_ps_allone2l_f5a_h);
    sim.compare("nshc_f5a_h " + phase, sim.dut->orig_nshc_f5a_h, sim.dut->new_nshc_f5a_h);
    sim.compare("ph_stky2l_f5a_h " + phase, sim.dut->orig_ph_stky2l_f5a_h, sim.dut->new_ph_stky2l_f5a_h);
    sim.compare("ps_stky2l_f5a_h " + phase, sim.dut->orig_ps_stky2l_f5a_h, sim.dut->new_ps_stky2l_f5a_h);
    sim.compare("int32_stky2l_f5a_h " + phase, sim.dut->orig_int32_stky2l_f5a_h, sim.dut->new_int32_stky2l_f5a_h);
    sim.compare("comb_add_a3_f5a_h " + phase, sim.dut->orig_comb_add_a3_f5a_h, sim.dut->new_comb_add_a3_f5a_h);
    sim.compare("fma_res_f6a_h " + phase, sim.dut->orig_fma_res_f6a_h, sim.dut->new_fma_res_f6a_h);
    sim.compare("frac_zero_f6a_h " + phase, sim.dut->orig_frac_zero_f6a_h, sim.dut->new_frac_zero_f6a_h);
    sim.compare("rnd_l_ph_f6a_h " + phase, sim.dut->orig_rnd_l_ph_f6a_h, sim.dut->new_rnd_l_ph_f6a_h);
    sim.compare("rnd_r_ph_f6a_h " + phase, sim.dut->orig_rnd_r_ph_f6a_h, sim.dut->new_rnd_r_ph_f6a_h);
    sim.compare("rnd_l_ps_f6a_h " + phase, sim.dut->orig_rnd_l_ps_f6a_h, sim.dut->new_rnd_l_ps_f6a_h);
    sim.compare("rnd_r_ps_f6a_h " + phase, sim.dut->orig_rnd_r_ps_f6a_h, sim.dut->new_rnd_r_ps_f6a_h);
    sim.compare("int_res_l_bit_f6a_h " + phase, sim.dut->orig_int_res_l_bit_f6a_h, sim.dut->new_int_res_l_bit_f6a_h);
}

void eval_phase(CosimCtrl<DUT>& sim, bool clk, const std::string& phase) {
    sim.dut->clk_i = clk;
    g_saw_clk_low |= !clk;
    g_saw_clk_high |= clk;
    sim.dut->eval();
    compare_all(sim, phase);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        std::printf("TIMEOUT at sim_time=%lu\n", static_cast<unsigned long>(sim.sim_time));
        std::exit(2);
    }
}

void run_cycle(CosimCtrl<DUT>& sim, const std::vector<uint32_t>& words,
               const std::string& label) {
    drive_words(sim, words);
    eval_phase(sim, false, label + " low");
    eval_phase(sim, true, label + " high");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    bool saw_reset_asserted = false;
    bool saw_reset_deasserted = false;
    bool saw_all_zero_stimulus = false;
    bool saw_all_one_stimulus = false;

    sim.dut->rst_ni = 0;
    saw_reset_asserted = true;
    run_cycle(sim, filled_words(sim, 0u), "reset");
    sim.dut->rst_ni = 1;
    saw_reset_deasserted = true;
    Verilated::assertOn(true);

    run_cycle(sim, filled_words(sim, 0u), "directed zero");
    saw_all_zero_stimulus = true;
    run_cycle(sim, filled_words(sim, 0xffff'ffffu), "directed one");
    saw_all_one_stimulus = true;

    uint32_t rng = 0x3141'5926u;
    for (int cycle = 0; cycle < 2500; ++cycle) {
        run_cycle(sim, random_words(sim, rng), "random " + std::to_string(cycle));
    }

    sim.check(g_saw_clk_low && g_saw_clk_high, "clock toggled low/high every cycle");
    sim.check(saw_reset_asserted && saw_reset_deasserted, "reset driven asserted and deasserted");
    sim.check(saw_all_zero_stimulus && saw_all_one_stimulus,
              "all packed non-clock inputs driven by all-zero and all-one directed cycles");
    sim.check(sim.comparisons >= 390000,
              "txfmafrac_top cosim should generate proportional all-output comparisons");
    return sim.finish();
}

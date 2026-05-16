// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f6_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f6_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->mergd_frac_res_hi_f6a_h = 0;
    sim.dut->classify_f6a_h = 0;
    sim.dut->sel_classify_out_f6a_h = 0;
    sim.dut->sel_nr1_frcpres_f6a_h = 0;
    sim.dut->rnd_l_ph_f6a_h = 0;
    sim.dut->rnd_r_ph_f6a_h = 0;
    sim.dut->rnd_l_ps_f6a_h = 0;
    sim.dut->rnd_r_ps_f6a_h = 0;
    sim.dut->rnd_l_int_f6a_h = 0;
    sim.dut->rnd_r_int_f6a_h = 0;
    sim.dut->ftz_f6a_l = 0;
    sim.dut->rc6_ph_f5a_h = 0;
    sim.dut->rc6_ps_f5a_h = 0;
    sim.dut->rc_int32_f5a_h = 0;
    sim.dut->normmask1_f6a_h = 0;
    sim.dut->normmask2_f6a_h = 0;
    sim.dut->trans_sinp2m_f6a_h = 0;
    sim.dut->s_res_f6a_h = 0;
    sim.dut->exp_res_f6a_h = 0;
    sim.dut->trans2_ps_roundup_r_f5a_h = 0;
    sim.dut->ps_rndovf_f6a_h = 0;
    sim.dut->ph_rndovf_f6a_h = 0;
    sim.dut->sel_set_bit0_one_f6a_h = 0;
    sim.dut->effsub1_set_sign1_f6a_h = 0;
    sim.dut->rrmux32_sel_f6a_h = 0;
    sim.dut->rrmux16l_sel_f6a_l = 0;
    sim.dut->f10f11mux_sel_f6a_h = 0;
    sim.dut->sel_psexc_sr_f6a_h = 0;
    sim.dut->sel_bit31_one_f6a_h = 0;
    sim.dut->sel_b30to0_ones_f6a_h = 0;
    sim.dut->sel_zerores_f6a_h = 0;
    sim.dut->sel_ps_unf_f6a_h = 0;
    sim.dut->sel_ps_normone_f6a_h = 0;
    sim.dut->sel_ps_inf_f6a_h = 0;
    sim.dut->sel_ps_cqn_f6a_h = 0;
    sim.dut->sel_ps_large_f6a_h = 0;
    sim.dut->sel_phsp_f6a_h = 0;
    sim.dut->sel_phhi_sr_f6a_h = 0;
    sim.dut->sel_phhi_large_f6a_h = 0;
    sim.dut->sel_phhi_unf_f6a_h = 0;
    sim.dut->sel_phhi_infres_f6a_h = 0;
    sim.dut->sel_f11inf_f6a_h = 0;
    sim.dut->sel_f10cqnan_f6a_h = 0;
    sim.dut->sel_f11sp_f6a_h = 0;
    sim.dut->sel_f10sp_f6a_h = 0;
    sim.dut->sel_f11large_f6a_h = 0;
    sim.dut->sel_f10large_f6a_h = 0;
    sim.dut->sel_gmaxu24_f6a_h = 0;
    sim.dut->sel_gmaxu16_f6a_h = 0;
    sim.dut->sel_gmaxs16_f6a_h = 0;
    sim.dut->sel_gmins16_f6a_h = 0;
    sim.dut->sel_gmaxu2_f6a_h = 0;
    sim.dut->sel_gmaxu8_f6a_h = 0;
    sim.dut->sel_gmaxs8_f6a_h = 0;
    sim.dut->sel_gmins8_f6a_h = 0;
    sim.dut->sel_gfxmax_f6a_h = 0;
    sim.dut->sel_excreshi_f6a_h = 0;
    sim.dut->sel_excreslo_f6a_h = 0;
    sim.dut->sel_fmares_hi_f6a_h = 0;
    sim.dut->sel_fmares_lo_f6a_h = 0;
}

void step_clock(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f5_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

uint32_t rr32_expected(bool sign, uint32_t exp, uint32_t rndres32) {
    return (static_cast<uint32_t>(sign) << 31) | ((exp & 0xffu) << 23) | ((rndres32 >> 8) & 0x7fffffu);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    sim.dut->classify_f6a_h = 0x155;
    sim.dut->sel_classify_out_f6a_h = 1;
    sim.dut->rrmux32_sel_f6a_h = 2;
    sim.dut->sel_fmares_hi_f6a_h = 2;
    sim.dut->sel_fmares_lo_f6a_h = 2;
    step_clock(sim);
    sim.check(sim.dut->fma_res_f6a_h == 0x155u, "classify_result_mux");

    clear_inputs(sim);
    sim.dut->sel_fmares_hi_f6a_h = 0;
    sim.dut->sel_fmares_lo_f6a_h = 0;
    sim.dut->sel_excreshi_f6a_h = 1;
    sim.dut->sel_excreslo_f6a_h = 1;
    sim.dut->s_res_f6a_h = 1;
    sim.dut->sel_psexc_sr_f6a_h = 1;
    sim.dut->sel_ps_inf_f6a_h = 1;
    step_clock(sim);
    sim.check(sim.dut->fma_res_f6a_h == 0xff800000u, "ps_negative_infinity_exception_result");

    clear_inputs(sim);
    const uint32_t rndres = 0x00abcde1u;
    sim.dut->mergd_frac_res_hi_f6a_h = (static_cast<uint64_t>(rndres) << 1) & kMask33;
    sim.dut->ftz_f6a_l = 3;
    sim.dut->s_res_f6a_h = 1;
    sim.dut->exp_res_f6a_h = 0x55;
    sim.dut->rrmux32_sel_f6a_h = 0;
    sim.dut->sel_fmares_hi_f6a_h = 2;
    sim.dut->sel_fmares_lo_f6a_h = 2;
    step_clock(sim);
    sim.check(sim.dut->fma_res_f6a_h == rr32_expected(true, 0x55, rndres), "rr32_normal_result_mux");
    sim.check(sim.dut->int_res_l_bit_f6a_h == (rndres & 1u), "int_result_l_bit_from_round_adder");

    clear_inputs(sim);
    sim.dut->sel_fmares_hi_f6a_h = 0;
    sim.dut->sel_fmares_lo_f6a_h = 0;
    sim.dut->sel_excreshi_f6a_h = 2;
    sim.dut->sel_excreslo_f6a_h = 2;
    sim.dut->sel_gmaxu8_f6a_h = 1;
    sim.dut->sel_gfxmax_f6a_h = 1;
    step_clock(sim);
    sim.check((sim.dut->fma_res_f6a_h & 0xffffu) == 0x00ffu, "graphics_u8_max_exception_low_result");

    return sim.finish();
}

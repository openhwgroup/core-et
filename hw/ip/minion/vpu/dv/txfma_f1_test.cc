// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f1_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f1_tb;

namespace {

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f0_clk = 0;
    sim.dut->multype_f0_clk = 0;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->multype_f1_clk = 0;
    sim.dut->fa_f0a_h = 0;
    sim.dut->fb_f0a_h = 0;
    sim.dut->fc_f0a_h = 0;
    sim.dut->dtps_f0a_h = 0;
    sim.dut->mul_sign_f0a_h = 0;
    sim.dut->mul_hp_mode_f0a_h = 0;
}

void step_all_clocks(SimCtrl<DUT>& sim) {
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
    sim.sim_time++;
    sim.dut->ctrl_f0_clk = 0;
    sim.dut->multype_f0_clk = 0;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->multype_f1_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

uint32_t trz_model(uint32_t frac, int width) {
    for (int i = 0; i < width; ++i) {
        if (((frac >> i) & 1u) != 0) return static_cast<uint32_t>(i);
    }
    return static_cast<uint32_t>(width);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->fa_f2a_h == 0, "zero_pipeline_fa");
    sim.check(sim.dut->fc_f2a_h == 0, "zero_pipeline_fc");
    sim.check(sim.dut->booth_fCin_f2a_h == 0, "zero_booth_cin");

    const uint32_t fa = 0x00200400u;   // PH-high lowest one at bit 0, PS lowest one at bit 2.
    const uint32_t fb = 0x00100200u;   // PH-high lowest one at bit 1, PS multiplier lowest one at bit 3.
    const uint32_t fc = 0x40001000u;   // FC b0 bit set, PS lowest one at bit 4.
    sim.dut->fa_f0a_h = fa;
    sim.dut->fb_f0a_h = fb;
    sim.dut->fc_f0a_h = fc;
    sim.dut->dtps_f0a_h = 1;
    step_all_clocks(sim);
    sim.check(sim.dut->etrz_fa_lo_ps_f1a_h == trz_model(fa >> 8, 24), "fa_ps_trailing_zero_index");
    sim.check(sim.dut->etrz_fb_lo_ps_f1a_h == trz_model(fb >> 6, 24), "fb_ps_trailing_zero_index");
    sim.check(sim.dut->etrz_fc_f1a_h == trz_model(fc >> 8, 24), "fc_ps_trailing_zero_index");
    sim.check(sim.dut->etrz_fa_hi_ph_f1a_h == trz_model(fa >> 21, 11), "fa_ph_hi_trailing_zero_index");
    sim.check(sim.dut->etrz_fb_hi_ph_f1a_h == trz_model(fb >> 19, 11), "fb_ph_hi_trailing_zero_index");

    step_all_clocks(sim);
    sim.check(sim.dut->fa_f2a_h == fa, "fa_pipeline_to_f2");
    sim.check(sim.dut->fc_f2a_h == fc, "fc_pipeline_to_f2");
    sim.check(sim.dut->fc_b0_f2a_h == ((fc >> 30) & 1u), "fc_b0_uses_shifted_bit_62");

    clear_inputs(sim);
    sim.dut->fa_f0a_h = 0x00000020u;
    sim.dut->fb_f0a_h = 0x00000040u;
    sim.dut->dtps_f0a_h = 0;
    step_all_clocks(sim);
    sim.check(sim.dut->etrz_fa_lo_ps_f1a_h == trz_model((0x00000020u >> 1) & 0x7ffu, 24),
              "ph_low_fa_trz_path_when_dtps_clear");
    sim.check(sim.dut->etrz_fb_lo_ps_f1a_h == trz_model((0x00000040u >> 1) & 0x7ffu, 24),
              "ph_low_fb_trz_path_when_dtps_clear");

    return sim.finish();
}

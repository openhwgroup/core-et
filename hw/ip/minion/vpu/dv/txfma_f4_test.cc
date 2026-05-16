// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f4_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f4_tb;

namespace {

constexpr uint64_t kMask52 = (1ull << 52) - 1ull;

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->ctrl_f4_clk = 0;
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
}

void step_all_clocks(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f3_clk = 1;
    sim.dut->ctrl_f4_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->comb_add_f5a_h == 0, "zero_comb_add");
    sim.check(sim.dut->int_minmax_res_f5a_h == 0, "zero_int_minmax_pipeline");
    sim.check(sim.dut->neg_norm_f5a_h == 0, "zero_neg_norm");
    sim.check(sim.dut->etrz50_f4a_h == 52, "zero_addend_trailing_zero_count");

    clear_inputs(sim);
    sim.dut->comb_s_f4a_h = 0x12345ull;
    sim.dut->comb_c_f4a_h = 0;
    sim.dut->int_minmax_res_f4a_h = 0x89abcdefu;
    sim.dut->ph_ps_clear_f4a_l = 1;
    sim.dut->clear16frnd_nshin_f4a_l = 1;
    step_all_clocks(sim);
    sim.check(sim.dut->comb_add_f5a_h == 0x12345ull, "combining_adder_preserved_when_clear_masks_deasserted");
    sim.check(sim.dut->int_minmax_res_f5a_h == 0x89abcdefu, "int_minmax_pipeline");

    sim.dut->comb_s_f4a_h = 0;
    sim.dut->comb_c_f4a_h = 0;
    sim.dut->al1c3_neg_cin_f3a_h = 1;
    sim.dut->ph_ps_clear_f4a_l = 1;
    sim.dut->clear16frnd_nshin_f4a_l = 1;
    step_all_clocks(sim);  // load carry-in control
    step_all_clocks(sim);  // observe the carry at F5
    sim.check(sim.dut->comb_add_f5a_h == 1, "combining_adder_carry_in");

    sim.dut->comb_s_f4a_h = 1ull << 51;
    sim.dut->comb_c_f4a_h = 0;
    sim.dut->neg_norm_a3_f4a_h = 1;
    sim.dut->neg_norm_na3_f4a_h = 0;
    sim.dut->al1c3_neg_cin_f3a_h = 0;
    step_all_clocks(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->comb_add_f5a_h == ((1ull << 51) & kMask52), "combining_adder_a3_bit");
    sim.check(sim.dut->neg_norm_f5a_h == 1, "neg_norm_uses_a3_sum_bit");

    clear_inputs(sim);
    sim.dut->neg_norm_na3_f4a_h = 1;
    step_all_clocks(sim);
    sim.check(sim.dut->neg_norm_f5a_h == 1, "neg_norm_na3_forces_negative_normalize");

    return sim.finish();
}

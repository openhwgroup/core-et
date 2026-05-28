// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f2_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f2_tb;

namespace {

constexpr uint64_t kMask64 = ~0ull;

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->ctrl_f2_clk = 0;
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
}

void step_all_clocks(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f1_clk = 1;
    sim.dut->ctrl_f2_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f1_clk = 0;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

uint64_t ext32(uint32_t x) {
    return (static_cast<uint64_t>(x) << 29) & kMask64;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->fa_f3a_h == 0, "zero_fa_pipeline");
    sim.check(sim.dut->fc_f3a_h == 0, "zero_fc_pipeline");
    sim.check(sim.dut->prodsum_f3a_h == 0, "zero_product_sum");

    const uint32_t fa = 0x00000005u;
    const uint32_t fc = 0x00000003u;
    sim.dut->fa_f2a_h = fa;
    sim.dut->fc_f2a_h = fc;
    sim.dut->paddin1_sels_f1a_h = 0;
    sim.dut->paddin2_selc_f1a_h = 0;
    sim.dut->paddin2_selfcneg_f1a_h = 0;
    sim.dut->padd_cin_f1a_h = 0;
    sim.dut->sel_famask_f1a_h = 0;
    step_all_clocks(sim);
    sim.check(sim.dut->fa_f3a_h == fa, "fa_pipeline_without_mask");
    sim.check(sim.dut->fc_f3a_h == fc, "fc_pipeline_passthrough");
    sim.check(sim.dut->prodsum_f3a_h == ((ext32(fa) + ext32(fc)) & kMask64),
              "product_adder_extends_fa_and_fc");

    sim.dut->fa_f2a_h = 0;
    sim.dut->fc_f2a_h = 1;
    sim.dut->paddin2_selfcneg_f1a_h = 1;
    sim.dut->padd_cin_f1a_h = 1;
    step_all_clocks(sim);  // load F2 local controls
    step_all_clocks(sim);  // observe them at the F3 output flops
    sim.check(sim.dut->prodsum_f3a_h == ((~ext32(1) + 1ull) & kMask64),
              "negated_fc_plus_carry_forms_twos_complement_sum");

    sim.dut->fa_f2a_h = 0x80000055u;
    sim.dut->fc_f2a_h = 0x12345678u;
    sim.dut->paddin2_selfcneg_f1a_h = 0;
    sim.dut->padd_cin_f1a_h = 0;
    sim.dut->sel_famask_f1a_h = 1;
    step_all_clocks(sim);  // load F2 local controls
    step_all_clocks(sim);  // observe masked FA
    const uint32_t expected_masked_fa = 0x80000055u | (((1u << 23) - 1u) << 8);
    sim.check(sim.dut->fa_f3a_h == expected_masked_fa, "fa_mask_forces_b19_through_a2_bits");
    sim.check(sim.dut->fc_f3a_h == 0x12345678u, "fc_pipeline_with_fa_mask");

    return sim.finish();
}

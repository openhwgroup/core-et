// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_depth_offset.
//
// Verifies the depth interpolation offset calculator.
// Three pipeline stages: S0 (multiply+normalize, registered via valid_stg1),
// S1-S2 (inside rbox_fp_adder_2stg, registered via valid_stg2).
//
// depth_offset = depth_coef_a * step_x + depth_coef_b * step_y

#include "Vrbox_depth_offset_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vrbox_depth_offset_tb;

static uint32_t fp32(int sign, int exp, uint32_t mant) {
    return ((uint32_t)sign << 31) | ((uint32_t)(exp & 0xFF) << 23) | (mant & 0x7FFFFF);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->depth_coef_a_i = 0;
    sim.dut->depth_coef_b_i = 0;
    sim.dut->step_x_i = 0;
    sim.dut->step_y_i = 0;
    sim.dut->valid_stg1_i = 0;
    sim.dut->valid_stg2_i = 0;
    sim.reset(3);

    // -- Test 1: Zero steps -> zero offset --
    printf("test 1: zero steps\n");
    sim.dut->depth_coef_a_i = fp32(0, 127, 0);  // 1.0
    sim.dut->depth_coef_b_i = fp32(0, 127, 0);  // 1.0
    sim.dut->step_x_i = 0;
    sim.dut->step_y_i = 0;
    sim.dut->valid_stg1_i = 1;
    sim.tick();  // S0 computes, registers
    sim.dut->valid_stg1_i = 0;
    sim.dut->valid_stg2_i = 1;
    sim.tick();  // S1 of adder latches
    sim.dut->valid_stg2_i = 0;
    // S2 output is now combinationally available
    sim.check(sim.dut->out_zero == 1, "zero steps -> zero offset");

    // -- Test 2: Zero coefficients -> zero offset --
    printf("test 2: zero coefficients\n");
    sim.dut->depth_coef_a_i = fp32(0, 0, 0);  // 0.0
    sim.dut->depth_coef_b_i = fp32(0, 0, 0);  // 0.0
    sim.dut->step_x_i = 16;  // 1.0 in 2.4
    sim.dut->step_y_i = 16;
    sim.dut->valid_stg1_i = 1;
    sim.tick();
    sim.dut->valid_stg1_i = 0;
    sim.dut->valid_stg2_i = 1;
    sim.tick();
    sim.dut->valid_stg2_i = 0;
    sim.check(sim.dut->out_zero == 1, "zero coefs -> zero offset");

    // -- Test 3: coef_a=1.0, step_x=1.0, coef_b=0, step_y=0 --
    printf("test 3: a=1.0, step_x=1.0, b=0, step_y=0\n");
    sim.dut->depth_coef_a_i = fp32(0, 127, 0);  // 1.0
    sim.dut->depth_coef_b_i = fp32(0, 0, 0);    // 0.0
    sim.dut->step_x_i = 16;  // 1.0 in 2.4
    sim.dut->step_y_i = 0;
    sim.dut->valid_stg1_i = 1;
    sim.tick();
    sim.dut->valid_stg1_i = 0;
    sim.dut->valid_stg2_i = 1;
    sim.tick();
    sim.dut->valid_stg2_i = 0;
    sim.check(sim.dut->out_zero == 0, "1.0*1.0 + 0: not zero");
    sim.check(sim.dut->out_infinite == 0, "1.0*1.0 + 0: not inf");
    sim.check(sim.dut->out_nan == 0, "1.0*1.0 + 0: not nan");
    sim.check(sim.dut->out_s == 0, "1.0*1.0 + 0: positive");

    // -- Test 4: NaN coefficient propagates --
    printf("test 4: NaN coefficient\n");
    sim.dut->depth_coef_a_i = fp32(0, 255, 0x400000);  // NaN
    sim.dut->depth_coef_b_i = fp32(0, 0, 0);
    sim.dut->step_x_i = 16;
    sim.dut->step_y_i = 0;
    sim.dut->valid_stg1_i = 1;
    sim.tick();
    sim.dut->valid_stg1_i = 0;
    sim.dut->valid_stg2_i = 1;
    sim.tick();
    sim.dut->valid_stg2_i = 0;
    sim.check(sim.dut->out_nan == 1, "NaN coef -> NaN offset");

    return sim.finish();
}

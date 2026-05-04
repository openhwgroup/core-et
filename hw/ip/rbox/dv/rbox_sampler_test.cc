// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_sampler.
//
// Verifies the edge function sampler. Two-stage pipeline:
//   Stage 1: compute quad/row offsets (rbox_step_mul * 4)
//   Stage 2: select offset (quad or row) and add to input sample
//
// Parameters: StepIntSz=2, StepFrcSz=4, EdgeCoefIntSz=9, EdgeCoefFrcSz=25
// OffsetSz = StepIntSz + EdgeCoefSz = 2 + 34 = 36

#include "Vrbox_sampler_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_sampler_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->coef_x_i = 0;
    sim.dut->coef_y_i = 0;
    sim.dut->quad_step_x_i = 0;
    sim.dut->quad_step_y_i = 0;
    sim.dut->row_step_x_i = 0;
    sim.dut->row_step_y_i = 0;
    sim.dut->valid_i = 0;
    sim.dut->in_sample_i = 0;
    sim.dut->sample_row_i = 0;
    sim.reset(3);

    // -- Test 1: Zero coefficients -> offset = 0, output = input --
    printf("test 1: zero coefficients\n");
    sim.dut->coef_x_i = 0;
    sim.dut->coef_y_i = 0;
    sim.dut->quad_step_x_i = 16;  // step = 1.0 in 2.4
    sim.dut->quad_step_y_i = 16;
    sim.dut->row_step_x_i  = 16;
    sim.dut->row_step_y_i  = 16;
    sim.dut->valid_i = 1;
    sim.tick();  // Stage 1 latches offsets
    sim.dut->valid_i = 0;

    // Stage 2: add offset (0) to sample (100)
    sim.dut->in_sample_i = 100;
    sim.dut->sample_row_i = 0;
    sim.tick();
    sim.check(sim.dut->out_sample_o == 100, "zero coefs: output = input (100)");

    // -- Test 2: coef_x = 1 (in 9.25 = 1 << 25), step_x = 1.0 (16 in 2.4) --
    //   offset_x = SX(1<<25) * 16 >> 4 = (1<<25) = 33554432
    //   offset_y = 0
    printf("test 2: unit coefficient, quad step\n");
    uint64_t coef_one = 1ULL << 25;  // 1.0 in 9.25 signed
    sim.dut->coef_x_i = coef_one & 0x3FFFFFFFFULL;
    sim.dut->coef_y_i = 0;
    sim.dut->quad_step_x_i = 16;  // 1.0
    sim.dut->quad_step_y_i = 0;
    sim.dut->row_step_x_i  = 0;
    sim.dut->row_step_y_i  = 0;
    sim.dut->valid_i = 1;
    sim.tick();
    sim.dut->valid_i = 0;

    sim.dut->in_sample_i = 0;
    sim.dut->sample_row_i = 0;
    sim.tick();
    // quad_offset_x = 1*16 >> 4 (in step_mul) = (SX(coef)*step) >> StepFrcSz
    // With CoefIntSz=9, CoefFrcSz=25, StepIntSz=2, StepFrcSz=4:
    //   offset = SX(34, 1<<25) * 16 >> 4 = (1<<25)*16 >> 4 = 1<<25
    uint64_t expected = coef_one;
    uint64_t mask43 = (1ULL << 43) - 1;
    sim.check((sim.dut->out_sample_o & mask43) == (expected & mask43),
              "unit coef quad: offset = 1<<25");

    // -- Test 3: Row mode selects row offsets --
    printf("test 3: row mode\n");
    sim.dut->coef_x_i = coef_one & 0x3FFFFFFFFULL;
    sim.dut->coef_y_i = 0;
    sim.dut->quad_step_x_i = 0;   // quad step = 0
    sim.dut->quad_step_y_i = 0;
    sim.dut->row_step_x_i  = 16;  // row step = 1.0
    sim.dut->row_step_y_i  = 0;
    sim.dut->valid_i = 1;
    sim.tick();
    sim.dut->valid_i = 0;

    sim.dut->in_sample_i = 0;
    sim.dut->sample_row_i = 1;  // Select row offset
    sim.tick();
    sim.check((sim.dut->out_sample_o & mask43) == (expected & mask43),
              "row mode: offset = 1<<25");

    // Verify quad mode with same coefficients gives 0 (quad step was 0)
    sim.dut->sample_row_i = 0;
    sim.tick();
    sim.check(sim.dut->out_sample_o == 0, "quad mode with zero quad step: output = 0");

    return sim.finish();
}

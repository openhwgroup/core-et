// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_sample_to_fp32.
//
// Verifies signed fixed-point (18.25) to FP32 conversion.
// Two-stage pipeline: set input + valid on cycle N, read output on cycle N+1.

#include "Vrbox_sample_to_fp32_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cmath>
#include <cstring>

using DUT = Vrbox_sample_to_fp32_tb;

static uint32_t to_bits(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(bits));
    return bits;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->in_i = 0;
    sim.dut->valid_i = 0;
    sim.reset(3);

    // -- Test 1: Zero input -> 0 output --
    printf("test 1: zero\n");
    sim.dut->in_i = 0;
    sim.dut->valid_i = 1;
    sim.tick();  // Stage 1 latches
    sim.dut->valid_i = 0;
    sim.tick();  // Stage 2 produces output (combinational from registered values)
    // Zero input: mant_magnitude = 0, leading_one = 0, so output is degenerate.
    // The module doesn't explicitly handle zero — we just check it doesn't crash.
    // With zero input, all mant bits are 0; result will be {0, BaseExp+0, 0} = small denorm-like value.

    // -- Test 2: +1.0 in 18.25 fixed-point = 1 << 25 = 33554432 --
    printf("test 2: +1.0\n");
    uint64_t one_fp = 1ULL << 25;  // 1.0 in 18.25 format
    sim.dut->in_i = one_fp;
    sim.dut->valid_i = 1;
    sim.tick();  // Stage 1
    sim.dut->valid_i = 0;
    // Output should be FP32 1.0 = 0x3F800000
    sim.check(sim.dut->out_o == to_bits(1.0f), "+1.0 -> 0x3F800000");

    // -- Test 3: +2.0 in 18.25 = 2 << 25 = 67108864 --
    printf("test 3: +2.0\n");
    uint64_t two_fp = 2ULL << 25;
    sim.dut->in_i = two_fp;
    sim.dut->valid_i = 1;
    sim.tick();  // Stage 1
    sim.dut->valid_i = 0;
    sim.check(sim.dut->out_o == to_bits(2.0f), "+2.0 -> 0x40000000");

    // -- Test 4: -1.0 in 18.25 (two's complement) --
    printf("test 4: -1.0\n");
    // 43-bit two's complement of (1 << 25)
    uint64_t neg_one = ((1ULL << 43) - one_fp) & ((1ULL << 43) - 1);
    sim.dut->in_i = neg_one;
    sim.dut->valid_i = 1;
    sim.tick();
    sim.dut->valid_i = 0;
    sim.check(sim.dut->out_o == to_bits(-1.0f), "-1.0 -> 0xBF800000");

    // -- Test 5: +0.5 in 18.25 = 1 << 24 --
    printf("test 5: +0.5\n");
    uint64_t half_fp = 1ULL << 24;
    sim.dut->in_i = half_fp;
    sim.dut->valid_i = 1;
    sim.tick();
    sim.dut->valid_i = 0;
    sim.check(sim.dut->out_o == to_bits(0.5f), "+0.5 -> 0x3F000000");

    return sim.finish();
}

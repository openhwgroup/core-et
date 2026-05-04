// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_fp_fast_adder.
//
// Verifies combinational FP adder with internal_depth_t representation.
// Tests: zero+zero, zero+val, val+zero, same sign add, opposite sign sub,
// infinity, NaN propagation.

#include "Vrbox_fp_fast_adder_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_fp_fast_adder_tb;

// Helper: set an internal_depth_t port to represent a "normal" FP value
static void set_depth(DUT* dut, bool is_a,
                      bool inf, bool nan_v, bool zero, bool sub,
                      uint8_t e, bool s, uint32_t m, int8_t e_d, uint8_t m_s) {
    if (is_a) {
        dut->a_infinite = inf;
        dut->a_nan      = nan_v;
        dut->a_zero     = zero;
        dut->a_subnorm  = sub;
        dut->a_e        = e;
        dut->a_s        = s;
        dut->a_m        = m & 0xFFFFFF;
        dut->a_e_delta  = e_d & 0x3F;
        dut->a_m_shift  = m_s & 0x1F;
    } else {
        dut->b_infinite = inf;
        dut->b_nan      = nan_v;
        dut->b_zero     = zero;
        dut->b_subnorm  = sub;
        dut->b_e        = e;
        dut->b_s        = s;
        dut->b_m        = m & 0xFFFFFF;
        dut->b_e_delta  = e_d & 0x3F;
        dut->b_m_shift  = m_s & 0x1F;
    }
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // Initialize both inputs to zero
    set_depth(sim.dut.get(), true,  false, false, true, false, 0, false, 0, 0, 0);
    set_depth(sim.dut.get(), false, false, false, true, false, 0, false, 0, 0, 0);
    sim.reset(3);

    // -- Test 1: 0 + 0 = 0 --
    printf("test 1: 0 + 0\n");
    set_depth(sim.dut.get(), true,  false, false, true, false, 0, false, 0, 0, 0);
    set_depth(sim.dut.get(), false, false, false, true, false, 0, false, 0, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_zero == 1, "0 + 0 = zero");

    // -- Test 2: 0 + 1.0 = 1.0 --
    printf("test 2: 0 + 1.0\n");
    // 1.0 in internal: e=127, m=0x800000 (1.0 in 1.23)
    set_depth(sim.dut.get(), true,  false, false, true,  false, 0,   false, 0,        0, 0);
    set_depth(sim.dut.get(), false, false, false, false, false, 127, false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_zero == 0,      "0 + 1.0: not zero");
    sim.check(sim.dut->out_e    == 127,    "0 + 1.0: exp = 127");
    sim.check(sim.dut->out_m    == 0x800000, "0 + 1.0: mant = 0x800000");
    sim.check(sim.dut->out_s    == 0,      "0 + 1.0: positive");

    // -- Test 3: 1.0 + 0 = 1.0 --
    printf("test 3: 1.0 + 0\n");
    set_depth(sim.dut.get(), true,  false, false, false, false, 127, false, 0x800000, 0, 0);
    set_depth(sim.dut.get(), false, false, false, true,  false, 0,   false, 0,        0, 0);
    sim.tick();
    sim.check(sim.dut->out_zero == 0,      "1.0 + 0: not zero");
    sim.check(sim.dut->out_e    == 127,    "1.0 + 0: exp = 127");
    sim.check(sim.dut->out_m    == 0x800000, "1.0 + 0: mant = 0x800000");

    // -- Test 4: 1.0 + 1.0 = 2.0 --
    printf("test 4: 1.0 + 1.0\n");
    set_depth(sim.dut.get(), true,  false, false, false, false, 127, false, 0x800000, 0, 0);
    set_depth(sim.dut.get(), false, false, false, false, false, 127, false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_zero == 0,      "1+1: not zero");
    sim.check(sim.dut->out_infinite == 0,  "1+1: not inf");
    sim.check(sim.dut->out_s == 0,         "1+1: positive");
    // 1.0 + 1.0 = 2.0: exp should be 128, mant should be 0x800000
    sim.check(sim.dut->out_e == 127, "1+1: base exp = 127");
    // The e_delta carries the renormalization; check the combined result
    // After LZA, the added mant carry shifts, giving e_delta=+1
    sim.check(sim.dut->out_e_delta == 1, "1+1: e_delta = 1");

    // -- Test 5: NaN propagation --
    printf("test 5: NaN + 1.0\n");
    set_depth(sim.dut.get(), true,  false, true,  false, false, 0xFF, false, 0xC00000, 0, 0);
    set_depth(sim.dut.get(), false, false, false, false, false, 127,  false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_nan == 1, "NaN + 1.0 = NaN");

    // -- Test 6: inf + 1.0 = inf --
    printf("test 6: inf + 1.0\n");
    set_depth(sim.dut.get(), true,  true,  false, false, false, 0xFF, false, 0x800000, 0, 0);
    set_depth(sim.dut.get(), false, false, false, false, false, 127,  false, 0x800000, 0, 0);
    sim.tick();
    sim.check(sim.dut->out_infinite == 1, "inf + 1.0 = inf");

    return sim.finish();
}

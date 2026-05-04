// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_inst_bits_stage (new) vs (original).
//
// Tests 3 parameter configurations with structured and random stimulus.
// Exercises all control inputs: enable, is_csr, is_br_jal, te_enable, xcpt.

#include "Vcosim_intpipe_inst_bits_stage_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_intpipe_inst_bits_stage_tb;

static uint32_t seed = 0xDEADBEEF;
static uint32_t xorshift32() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("cfg0", (uint64_t)d->orig_out_0_o, (uint64_t)d->new_out_0_o);
    sim.compare("cfg1", (uint64_t)d->orig_out_1_o, (uint64_t)d->new_out_1_o);
    sim.compare("cfg2", (uint64_t)d->orig_out_2_o, (uint64_t)d->new_out_2_o);
}

static void step(CosimCtrl<DUT>& sim) {
    sim.dut->eval();
    compare_all(sim);
    sim.tick();
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->stim_enable = 0;
    d->stim_is_csr = 0;
    d->stim_is_br_jal = 0;
    d->stim_te_enable = 0;
    d->stim_xcpt = 0;
    d->stim_in = 0;
    sim.reset();

    // Phase 1: enable=1, only Mask bits should be stored
    printf("=== Phase 1: enable only (Mask bits) ===\n");
    d->stim_enable = 1;
    d->stim_in = 0xDEADBEEF;
    step(sim);
    d->stim_in = 0x12345678;
    step(sim);
    d->stim_enable = 0;
    d->stim_in = 0xFFFFFFFF;  // should not be stored
    step(sim);
    step(sim);

    // Phase 2: is_br_jal — BrMask bits should also be stored
    printf("=== Phase 2: enable + is_br_jal (Mask + BrMask bits) ===\n");
    d->stim_enable = 1;
    d->stim_is_br_jal = 1;
    d->stim_in = 0xCAFEBABE;
    step(sim);
    d->stim_in = 0xBAADF00D;
    step(sim);
    d->stim_is_br_jal = 0;
    d->stim_enable = 0;
    step(sim);

    // Phase 3: is_csr — all bits stored
    printf("=== Phase 3: enable + is_csr (all bits) ===\n");
    d->stim_enable = 1;
    d->stim_is_csr = 1;
    d->stim_in = 0x11111111;
    step(sim);
    d->stim_in = 0x22222222;
    step(sim);
    d->stim_is_csr = 0;
    d->stim_enable = 0;
    step(sim);

    // Phase 4: te_enable — all bits stored
    printf("=== Phase 4: enable + te_enable (all bits) ===\n");
    d->stim_enable = 1;
    d->stim_te_enable = 1;
    d->stim_in = 0x33333333;
    step(sim);
    d->stim_in = 0x44444444;
    step(sim);
    d->stim_te_enable = 0;
    d->stim_enable = 0;
    step(sim);

    // Phase 5: xcpt — all bits stored regardless of enable
    printf("=== Phase 5: xcpt (all bits, ignores enable) ===\n");
    d->stim_enable = 0;
    d->stim_xcpt = 1;
    d->stim_in = 0x55555555;
    step(sim);
    d->stim_in = 0x66666666;
    step(sim);
    d->stim_xcpt = 0;
    step(sim);

    // Phase 6: enable=0, no xcpt — nothing stored
    printf("=== Phase 6: disabled (no storage) ===\n");
    d->stim_enable = 0;
    d->stim_in = 0x77777777;
    step(sim);
    d->stim_in = 0x88888888;
    step(sim);
    step(sim);

    // Phase 7: Heavy random (20K cycles)
    printf("=== Phase 7: random stimulus (20000 cycles) ===\n");
    for (int i = 0; i < 20000; i++) {
        uint32_t r = xorshift32();
        d->stim_enable     = (r >> 0) & 1;
        d->stim_is_csr     = (r >> 1) & 1;
        d->stim_is_br_jal  = (r >> 2) & 1;
        d->stim_te_enable  = (r >> 3) & 1;
        d->stim_xcpt       = ((r >> 4) & 0xF) == 0;  // ~6.25%
        d->stim_in         = xorshift32();
        step(sim);
    }

    // Phase 8: Random with second seed
    printf("=== Phase 8: random stimulus seed 2 (20000 cycles) ===\n");
    seed = 0xCAFEF00D;
    for (int i = 0; i < 20000; i++) {
        uint32_t r = xorshift32();
        d->stim_enable     = (r >> 0) & 1;
        d->stim_is_csr     = ((r >> 1) & 0x7) == 0;  // ~12.5%
        d->stim_is_br_jal  = ((r >> 4) & 0x3) == 0;  // ~25%
        d->stim_te_enable  = ((r >> 6) & 0xF) == 0;   // ~6.25%
        d->stim_xcpt       = ((r >> 10) & 0x1F) == 0;  // ~3%
        d->stim_in         = xorshift32();
        step(sim);
    }

    return sim.finish();
}

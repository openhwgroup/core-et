// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_set_calc — Verilator test.

#include "Vshirecache_pipe_set_calc.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_set_calc;

static void eval_set(SimCtrl<DUT>& sim,
                     bool region_op, uint16_t addr_set,
                     uint16_t base, uint16_t size,
                     uint16_t mask, uint16_t tag_mask) {
    sim.dut->region_op_i   = region_op;
    sim.dut->address_set_i = addr_set;
    sim.dut->esr_set_base_i = base;
    sim.dut->esr_set_size_i = size;
    sim.dut->esr_set_mask_i = mask;
    sim.dut->esr_tag_mask_i = tag_mask;
    sim.dut->eval();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    // Default config: 256 sets, base=0, size=256, mask=0xFF, tag_mask=0xFF

    // ── Test 1: Region disabled → output 0 ──────────────
    printf("test 1: region disabled\n");
    eval_set(sim, false, 42, 0, 256, 0xFF, 0xFF);
    sim.check(sim.dut->pipe_set_o == 0, "pipe_set = 0 when region off");

    // ── Test 2: Identity mapping (base=0, full size) ────
    printf("test 2: identity mapping\n");
    for (int i = 0; i < 256; i += 17) {
        eval_set(sim, true, i, 0, 256, 0xFF, 0xFF);
        sim.check(sim.dut->pipe_set_o == (uint16_t)i,
                  "set[" + std::to_string(i) + "] = " + std::to_string(i));
    }

    // ── Test 3: Offset base ─────────────────────────────
    printf("test 3: offset base (base=64, size=64)\n");
    // 64 sets starting at 64, mask=0x3F (6 bits)
    for (int i = 0; i < 64; i += 7) {
        eval_set(sim, true, i, 64, 64, 0x3F, 0x3F);
        uint16_t result = sim.dut->pipe_set_o;
        sim.check(result >= 64 && result < 128,
                  "set in range [64,128) for addr " + std::to_string(i));
    }

    // ── Test 4: Small allocation (base=0, size=16) ──────
    printf("test 4: small allocation (size=16)\n");
    // 16 sets, mask=0xF
    for (int i = 0; i < 256; i += 13) {
        eval_set(sim, true, i, 0, 16, 0xF, 0xF);
        uint16_t result = sim.dut->pipe_set_o;
        sim.check(result < 16,
                  "set < 16 for addr " + std::to_string(i));
    }

    // ── Test 5: Power-of-2 sizes ────────────────────────
    printf("test 5: various power-of-2 sizes\n");
    int sizes[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    for (int s = 0; s < 9; s++) {
        int sz = sizes[s];
        int msk = sz - 1;
        eval_set(sim, true, 100, 0, sz, msk, msk);
        uint16_t result = sim.dut->pipe_set_o;
        sim.check(result < (uint16_t)sz,
                  "set < " + std::to_string(sz) + " for size=" + std::to_string(sz));
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_set_calc_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_set_calc_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 100000;

    // ── Test 1: Identity mapping ────────────────────────
    printf("cosim test 1: identity mapping (base=0, size=256)\n");
    for (int i = 0; i < 256; i++) {
        sim.dut->region_op_i    = 1;
        sim.dut->address_set_i  = i;
        sim.dut->esr_set_base_i = 0;
        sim.dut->esr_set_size_i = 256;
        sim.dut->esr_set_mask_i = 0xFF;
        sim.dut->esr_tag_mask_i = 0xFF;
        sim.dut->eval();
        sim.sim_time++;
        sim.compare("pipe_set", sim.dut->orig_pipe_set_o, sim.dut->new_pipe_set_o);
    }

    // ── Test 2: Offset base ─────────────────────────────
    printf("cosim test 2: offset base\n");
    int bases[] = {0, 16, 32, 64, 128, 192};
    int sizes[] = {256, 16, 32, 64, 128, 64};
    for (int t = 0; t < 6; t++) {
        int sz = sizes[t];
        int msk = sz - 1;
        for (int i = 0; i < 256; i++) {
            sim.dut->region_op_i    = 1;
            sim.dut->address_set_i  = i;
            sim.dut->esr_set_base_i = bases[t];
            sim.dut->esr_set_size_i = sz;
            sim.dut->esr_set_mask_i = msk;
            sim.dut->esr_tag_mask_i = msk;
            sim.dut->eval();
            sim.sim_time++;
            sim.compare("pipe_set", sim.dut->orig_pipe_set_o, sim.dut->new_pipe_set_o);
        }
    }

    // ── Test 3: Region disabled ─────────────────────────
    printf("cosim test 3: region disabled\n");
    for (int i = 0; i < 16; i++) {
        sim.dut->region_op_i    = 0;
        sim.dut->address_set_i  = i;
        sim.dut->esr_set_base_i = 0;
        sim.dut->esr_set_size_i = 256;
        sim.dut->esr_set_mask_i = 0xFF;
        sim.dut->esr_tag_mask_i = 0xFF;
        sim.dut->eval();
        sim.sim_time++;
        sim.compare("pipe_set", sim.dut->orig_pipe_set_o, sim.dut->new_pipe_set_o);
    }

    return sim.finish();
}

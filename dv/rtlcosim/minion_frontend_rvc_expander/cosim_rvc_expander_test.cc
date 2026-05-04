// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: minion_frontend_rvc_expander vs frontend_rvc_expander.
//
// Exhaustively tests all 65536 possible 16-bit compressed instruction
// encodings plus a sweep of 32-bit pass-through values.

#include "Vcosim_rvc_expander_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rvc_expander_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    sim.dut->rst_ni = 1;

    // Phase 1: Exhaustive compressed instruction test (bits[1:0] != 11).
    // Test all 65536 possible 16-bit values as the lower half,
    // with upper 16 bits zero (typical for compressed fetch).
    printf("phase 1: exhaustive 16-bit compressed test (65536 values)\n");
    for (uint32_t i = 0; i < 65536; i++) {
        // Skip values where bits[1:0] == 11 (32-bit pass-through)
        if ((i & 0x3) == 0x3) continue;

        sim.dut->stim_i = i;
        sim.dut->eval();

        if (sim.dut->orig_out_o != sim.dut->new_out_o) {
            printf("  IN=0x%04x ORIG=0x%08x NEW=0x%08x\n", i, sim.dut->orig_out_o, sim.dut->new_out_o);
        }
        sim.compare("out_bits", sim.dut->orig_out_o, sim.dut->new_out_o);
    }

    // Phase 2: 32-bit pass-through test (bits[1:0] == 11).
    // These should pass through unchanged.
    printf("phase 2: 32-bit pass-through test\n");
    for (uint32_t i = 0; i < 4096; i++) {
        // Generate 32-bit values with bits[1:0] == 11
        uint32_t val = (i << 2) | 0x3;
        sim.dut->stim_i = val;
        sim.dut->eval();

        sim.compare("passthru", sim.dut->orig_out_o, sim.dut->new_out_o);
    }

    // Phase 3: Specific known compressed instructions.
    printf("phase 3: known instruction patterns\n");
    struct {
        uint32_t in;
        const char* name;
    } known[] = {
        {0x0001, "C.NOP"},
        {0x0505, "C.ADDI x10, 1"},
        {0x4501, "C.LI x10, 0"},
        {0x6105, "C.ADDI16SP 32"},
        {0x6585, "C.LUI x11, 2"},
        {0xA001, "C.J 0"},
        {0xC001, "C.BEQZ x8, 0"},
        {0xE001, "C.BNEZ x8, 0"},
        {0x0002, "C.SLLI x0, 0"},
        {0x8082, "C.JR x1"},
        {0x9002, "C.EBREAK"},
        {0x9082, "C.JALR x1"},
    };
    for (auto& k : known) {
        sim.dut->stim_i = k.in;
        sim.dut->eval();
        sim.compare(k.name, sim.dut->orig_out_o, sim.dut->new_out_o);
    }

    return sim.finish();
}

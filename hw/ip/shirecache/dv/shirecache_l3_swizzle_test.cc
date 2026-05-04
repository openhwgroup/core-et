// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_l3_swizzle_get/put — Verilator test.
// Tests the round-trip: put fields into address, then get them back.

#include "Vshirecache_l3_swizzle_tb.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_l3_swizzle_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 1000;

    // Default swizzle config: identity mapping
    // sub_bank bits at positions 0,1,2
    // bank bits at positions 3,4,5
    // shire bits at positions 6,7,8,9,10,11
    sim.dut->swizzle_ctl_i = 0;
    // Pack the sel fields: each is L3SwizzleBitSelSize=4 bits wide.
    // Identity: sub_bank_sel_b0=0, b1=1, b2=2, bank_sel_b0=3, b1=4, b2=5, shire_sel_b0=6..b5=11
    // Packed struct order (MSB first): all_shire_aliasing, two_shire_aliasing, sub_bank_sel_b2..b0, bank_sel_b2..b0, shire_sel_b5..b0
    // That's 2 + 3*4 + 3*4 + 6*4 = 2 + 12 + 12 + 24 = 50 bits
    // Let's just set it field by field via bit manipulation.
    uint64_t ctl = 0;
    // Fields from LSB (shire_sel_b0) to MSB (all_shire_aliasing):
    // shire_sel_b0=6, b1=7, b2=8, b3=9, b4=10, b5=11
    // bank_sel_b0=3, b1=4, b2=5
    // sub_bank_sel_b0=0, b1=1, b2=2
    // two_shire_aliasing=0, all_shire_aliasing=0
    int pos = 0;
    int sels[] = {6, 7, 8, 9, 10, 11,  // shire_sel b0-b5
                  3, 4, 5,              // bank_sel b0-b2
                  0, 1, 2,              // sub_bank_sel b0-b2
                  0, 0};                // two_shire, all_shire
    for (int i = 0; i < 14; i++) {
        ctl |= ((uint64_t)sels[i] & 0xF) << pos;
        pos += (i < 12) ? 4 : 1;  // sel fields are 4 bits, flags are 1 bit
    }
    sim.dut->swizzle_ctl_i = ctl;

    // ── Test 1: Round-trip identity ─────────────────────
    printf("test 1: round-trip identity mapping\n");
    for (int sub = 0; sub < 4; sub++) {
        for (int bank = 0; bank < 4; bank++) {
            for (int shire = 0; shire < 32; shire++) {
                sim.dut->put_sub_bank_id_i = sub;
                sim.dut->put_bank_id_i     = bank;
                sim.dut->put_shire_id_i    = shire;
                sim.dut->eval();

                // Feed put output to get input
                sim.dut->get_address_i = sim.dut->put_address_o;
                sim.dut->eval();

                sim.check(sim.dut->get_sub_bank_id_o == (uint8_t)(sub & 0x3),
                          "sub_bank round-trip");
                sim.check(sim.dut->get_bank_id_o == (uint8_t)(bank & 0x3),
                          "bank round-trip");
                sim.check(sim.dut->get_shire_id_o == (uint8_t)(shire & 0x1F),
                          "shire round-trip");
            }
        }
    }

    return sim.finish();
}

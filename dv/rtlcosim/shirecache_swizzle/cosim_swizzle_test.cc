// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_swizzle_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_swizzle_tb;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 100000;

    // Identity swizzle config: sub_bank bits 0-2, bank bits 3-5, shire bits 6-11.
    // Pack into the esr_sc_l3_shire_swizzle_ctl_t struct.
    // Field order (LSB first): shire_sel_b0..b5, bank_sel_b0..b2, sub_bank_sel_b0..b2, flags
    uint64_t ctl = 0;
    int sels[] = {6, 7, 8, 9, 10, 11, 3, 4, 5, 0, 1, 2, 0, 0};
    int pos = 0;
    for (int i = 0; i < 14; i++) {
        ctl |= ((uint64_t)(sels[i] & 0xF)) << pos;
        pos += (i < 12) ? 4 : 1;
    }
    sim.dut->swizzle_ctl_i = ctl;

    // ── Test 1: GET — sweep addresses ───────────────────
    printf("cosim test 1: GET — sweep addresses\n");
    for (uint64_t addr = 0; addr < 0x10000; addr += 64) {
        sim.dut->get_address_i = addr;
        sim.dut->eval();
        sim.sim_time++;
        sim.compare("get_sub_bank", sim.dut->orig_get_sub_bank_o, sim.dut->new_get_sub_bank_o);
        sim.compare("get_bank",     sim.dut->orig_get_bank_o,     sim.dut->new_get_bank_o);
        sim.compare("get_shire",    sim.dut->orig_get_shire_o,    sim.dut->new_get_shire_o);
    }

    // ── Test 2: PUT — sweep field values ────────────────
    printf("cosim test 2: PUT — sweep field values\n");
    for (int sub = 0; sub < 4; sub++) {
        for (int bank = 0; bank < 4; bank++) {
            for (int shire = 0; shire < 32; shire++) {
                sim.dut->put_sub_bank_i = sub;
                sim.dut->put_bank_i     = bank;
                sim.dut->put_shire_i    = shire;
                sim.dut->eval();
                sim.sim_time++;
                sim.compare("put_addr",
                    sim.dut->orig_put_address_o,
                    sim.dut->new_put_address_o);
            }
        }
    }

    return sim.finish();
}

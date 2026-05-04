// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_tag_ram_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_tag_ram_tb;

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    // Compare all 4 words of 116-bit tag data.
    for (int w = 0; w < 4; w++) {
        sim.compare("rd_data[" + std::to_string(w) + "]",
                     sim.dut->orig_rd_data_o[w],
                     sim.dut->new_rd_data_o[w]);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->addr_i  = 0;
    for (int i = 0; i < 4; i++) sim.dut->wr_data_i[i] = 0;
    sim.reset(10);

    // ── Test 1: Write and read ──────────────────────────
    printf("cosim test 1: write and read\n");
    for (int a = 0; a < 16; a++) {
        sim.dut->wr_en_i      = 1;
        sim.dut->addr_i       = a;
        sim.dut->wr_data_i[0] = 0xA000 + a;
        sim.dut->wr_data_i[1] = 0xB000 + a;
        sim.dut->wr_data_i[2] = 0xC000 + a;
        sim.dut->wr_data_i[3] = a & 0xFFFFF;
        tick_and_compare(sim);
    }
    sim.dut->wr_en_i = 0;
    tick_and_compare(sim);

    for (int a = 0; a < 16; a++) {
        sim.dut->rd_en_i = 1;
        sim.dut->addr_i  = a;
        tick_and_compare(sim);
        sim.dut->rd_en_i = 0;
        tick_and_compare(sim);
    }

    // ── Test 2: Random ──────────────────────────────────
    printf("cosim test 2: random (200 cycles)\n");
    uint32_t seed = 0x55AA1234;
    for (int i = 0; i < 200; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;

        int do_rd = (seed >> 0) & 1;
        int do_wr = (seed >> 1) & 1;
        if (do_rd && do_wr) do_wr = 0;  // mutually exclusive

        sim.dut->rd_en_i      = do_rd;
        sim.dut->wr_en_i      = do_wr;
        sim.dut->addr_i       = (seed >> 2) & 0xFF;
        sim.dut->wr_data_i[0] = seed;
        sim.dut->wr_data_i[1] = ~seed;
        sim.dut->wr_data_i[2] = seed >> 8;
        sim.dut->wr_data_i[3] = seed & 0xFFFFF;
        tick_and_compare(sim);
    }
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    tick_and_compare(sim);

    return sim.finish();
}

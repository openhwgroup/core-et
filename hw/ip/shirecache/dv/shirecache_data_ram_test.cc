// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_data_ram_wrap — Verilator test.
// DataRamDataSize = 576 bits = VlWide<18> (18 x 32-bit words)

#include "Vshirecache_pipe_data_ram_wrap.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_data_ram_wrap;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->clock_en_i = 1;
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->ram_delay_i = 2;
    sim.dut->rd_en_i    = 0;
    sim.dut->wr_en_i    = 0;
    sim.dut->addr_i     = 0;
    sim.dut->wr_qwen_i  = 0;
    sim.dut->ram_cfg_i  = 0;
    sim.dut->dft_i      = 0;
    // 576 bits = 18 x 32-bit words
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = 0;
    sim.reset();

    // ── Test 1: Write all QWs, read back ────────────────
    printf("test 1: write all quadwords\n");
    sim.dut->wr_en_i   = 1;
    sim.dut->addr_i    = 0;
    sim.dut->wr_qwen_i = 0xF;
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = 0xAA00 + i;
    sim.tick();

    sim.dut->wr_en_i = 0;
    sim.dut->rd_en_i = 1;
    sim.dut->addr_i  = 0;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check(sim.dut->rd_data_o[0] == 0xAA00, "QW0 word0");
    sim.check(sim.dut->rd_data_o[1] == 0xAA01, "QW0 word1");
    sim.tick();

    // ── Test 2: Partial QW write ────────────────────────
    printf("test 2: partial QW write\n");
    // Write QW0 only at addr 1
    sim.dut->wr_en_i   = 1;
    sim.dut->addr_i    = 1;
    sim.dut->wr_qwen_i = 0b0001;
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = 0xBB00 + i;
    sim.tick();

    // Write QW2 only at addr 1 with different data
    sim.dut->wr_qwen_i = 0b0100;
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = 0xCC00 + i;
    sim.tick();

    // Read back — QW0 should have BB data, QW2 should have CC data
    sim.dut->wr_en_i = 0;
    sim.dut->rd_en_i = 1;
    sim.dut->addr_i  = 1;
    sim.tick();
    sim.dut->rd_en_i = 0;
    // QW0 = words [0..4] (144 bits = 4.5 words, but each QW is 144/32 = 4.5 → stored as 5 words)
    // Actually 144 bits per QW, 18 words total for 4 QWs (576 bits).
    // QW0 = wr_data_i[0..4] (bits 0-143), words 0-3 and lower 16 bits of word 4
    sim.check(sim.dut->rd_data_o[0] == 0xBB00, "QW0 has BB data");
    // QW2 starts at bit 288. 288/32 = word 9
    sim.check(sim.dut->rd_data_o[9] == 0xCC09, "QW2 has CC data");
    sim.tick();

    // ── Test 3: Multiple addresses ──────────────────────
    printf("test 3: multiple addresses\n");
    for (int a = 0; a < 4; a++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->addr_i    = 10 + a;
        sim.dut->wr_qwen_i = 0xF;
        for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = (a << 8) | i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;

    for (int a = 0; a < 4; a++) {
        sim.dut->rd_en_i = 1;
        sim.dut->addr_i  = 10 + a;
        sim.tick();
        sim.dut->rd_en_i = 0;
        sim.check(sim.dut->rd_data_o[0] == (uint32_t)((a << 8) | 0),
                  "addr " + std::to_string(10+a) + " QW0 word0");
        sim.tick();
    }

    return sim.finish();
}

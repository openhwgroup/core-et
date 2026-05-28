// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_data_ram_wrap — Verilator test.
// DataRamDataSize = 576 bits = VlWide<18> (18 x 32-bit words).
// The wrapper preserves the original sram_clock_pre pulse generator, so each
// request must pulse clock_en_i for one base clock.

#include "Vshirecache_pipe_data_ram_wrap.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_data_ram_wrap;

static void clear_req(SimCtrl<DUT>& sim) {
    sim.dut->clock_en_i = 0;
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_qwen_i = 0;
}

static void idle(SimCtrl<DUT>& sim, int cycles = 1) {
    clear_req(sim);
    for (int i = 0; i < cycles; ++i) sim.tick();
}

static void write_addr(SimCtrl<DUT>& sim, uint16_t addr, uint8_t qwen, uint32_t base) {
    sim.dut->clock_en_i = 1;
    sim.dut->wr_en_i = 1;
    sim.dut->rd_en_i = 0;
    sim.dut->addr_i = addr;
    sim.dut->wr_qwen_i = qwen;
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = base + i;
    sim.tick();
    idle(sim);
}

static void read_addr(SimCtrl<DUT>& sim, uint16_t addr) {
    sim.dut->clock_en_i = 1;
    sim.dut->rd_en_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->addr_i = addr;
    sim.tick();
    idle(sim);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_req(sim);
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->ram_delay_i = 2;
    sim.dut->addr_i = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i = 0;
    for (int i = 0; i < 18; i++) sim.dut->wr_data_i[i] = 0;
    sim.reset();

    // ── Test 1: Write all QWs, read back ────────────────
    printf("test 1: write all quadwords\n");
    write_addr(sim, 0, 0xF, 0xAA00);
    read_addr(sim, 0);
    sim.check(sim.dut->rd_data_o[0] == 0xAA00, "QW0 word0");
    sim.check(sim.dut->rd_data_o[1] == 0xAA01, "QW0 word1");

    // ── Test 2: Partial QW write ────────────────────────
    printf("test 2: partial QW write\n");
    // Write QW0 only at addr 1.
    write_addr(sim, 1, 0b0001, 0xBB00);
    // Write QW2 only at addr 1 with different data.
    write_addr(sim, 1, 0b0100, 0xCC00);

    read_addr(sim, 1);
    // QW0 = words [0..4] (144 bits); QW2 starts at bit 288, word 9.
    sim.check(sim.dut->rd_data_o[0] == 0xBB00, "QW0 has BB data");
    sim.check(sim.dut->rd_data_o[9] == 0xCC09, "QW2 has CC data");

    // ── Test 3: Multiple addresses ──────────────────────
    printf("test 3: multiple addresses\n");
    for (int a = 0; a < 4; a++) {
        write_addr(sim, 10 + a, 0xF, (uint32_t)(a << 8));
    }

    for (int a = 0; a < 4; a++) {
        read_addr(sim, 10 + a);
        sim.check(sim.dut->rd_data_o[0] == (uint32_t)((a << 8) | 0),
                  "addr " + std::to_string(10+a) + " QW0 word0");
    }

    return sim.finish();
}

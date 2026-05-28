// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_tag_state_ram_wrap — Verilator test.
// Tests dual-port mode (independent read/write addresses).  The translated
// wrapper preserves the original sram_clock_pre pulse generator, so each
// request must pulse clock_en_i for one base clock.

#include "Vshirecache_pipe_tag_state_ram_wrap.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_tag_state_ram_wrap;

static void clear_req(SimCtrl<DUT>& sim) {
    sim.dut->clock_en_i = 0;
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
}

static void idle(SimCtrl<DUT>& sim, int cycles = 1) {
    clear_req(sim);
    for (int i = 0; i < cycles; ++i) sim.tick();
}

static void write_addr(SimCtrl<DUT>& sim, uint16_t addr, uint64_t data) {
    sim.dut->clock_en_i = 1;
    sim.dut->wr_en_i = 1;
    sim.dut->rd_en_i = 0;
    sim.dut->wr_addr_i = addr;
    sim.dut->wr_data_i = data;
    sim.tick();
    idle(sim);
}

static uint64_t read_addr(SimCtrl<DUT>& sim, uint16_t addr) {
    sim.dut->clock_en_i = 1;
    sim.dut->rd_en_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->rd_addr_i = addr;
    sim.tick();
    idle(sim);
    return sim.dut->rd_data_o;
}

static uint64_t readwrite_addr(SimCtrl<DUT>& sim, uint16_t rd_addr, uint16_t wr_addr,
                               uint64_t wr_data) {
    sim.dut->clock_en_i = 1;
    sim.dut->rd_en_i = 1;
    sim.dut->wr_en_i = 1;
    sim.dut->rd_addr_i = rd_addr;
    sim.dut->wr_addr_i = wr_addr;
    sim.dut->wr_data_i = wr_data;
    sim.tick();
    idle(sim);
    return sim.dut->rd_data_o;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_req(sim);
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->ram_delay_i = 2;
    sim.dut->rd_addr_i = 0;
    sim.dut->wr_addr_i = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i = 0;
    sim.reset();

    // ── Test 1: Write and read back ─────────────────────
    printf("test 1: write and read\n");
    write_addr(sim, 3, 0xABCDE);
    sim.check((read_addr(sim, 3) & 0xFFFFF) == 0xABCDE, "read back matches");

    // ── Test 2: Simultaneous write and read (dual-port) ─
    printf("test 2: simultaneous write and read (different addresses)\n");
    write_addr(sim, 10, 0x12345);
    sim.check((readwrite_addr(sim, 10, 11, 0x67890) & 0xFFFFF) == 0x12345,
              "read addr 10 while writing addr 11");
    sim.check((read_addr(sim, 11) & 0xFFFFF) == 0x67890, "addr 11 written correctly");

    // ── Test 3: Multiple addresses ──────────────────────
    printf("test 3: multiple addresses\n");
    for (int i = 0; i < 8; i++) {
        write_addr(sim, 20 + i, 0xF0000 + i);
    }
    for (int i = 0; i < 8; i++) {
        sim.check((read_addr(sim, 20 + i) & 0xFFFFF) == (uint64_t)(0xF0000 + i),
                  "addr[" + std::to_string(20+i) + "]");
    }

    // ── Test 4: Output holds when not reading ───────────
    printf("test 4: output holds\n");
    uint64_t held = sim.dut->rd_data_o;
    idle(sim, 2);
    sim.check(sim.dut->rd_data_o == held, "output holds");

    return sim.finish();
}

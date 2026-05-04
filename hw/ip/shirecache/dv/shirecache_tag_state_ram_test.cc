// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_tag_state_ram_wrap — Verilator test.
// Tests dual-port mode (independent read/write addresses).

#include "Vshirecache_pipe_tag_state_ram_wrap.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_tag_state_ram_wrap;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->clock_en_i = 1;
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->ram_delay_i = 2;
    sim.dut->rd_en_i    = 0;
    sim.dut->wr_en_i    = 0;
    sim.dut->rd_addr_i  = 0;
    sim.dut->wr_addr_i  = 0;
    sim.dut->wr_data_i  = 0;  // 40 bits, fits in uint64_t
    sim.dut->ram_cfg_i  = 0;
    sim.dut->dft_i      = 0;
    sim.reset();

    // ── Test 1: Write and read back ─────────────────────
    printf("test 1: write and read\n");
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_addr_i = 3;
    sim.dut->wr_data_i = 0xABCDE;  // 20-bit value in 40-bit field
    sim.tick();
    sim.dut->wr_en_i = 0;

    sim.dut->rd_en_i  = 1;
    sim.dut->rd_addr_i = 3;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check((sim.dut->rd_data_o & 0xFFFFF) == 0xABCDE, "read back matches");
    sim.tick();

    // ── Test 2: Simultaneous write and read (dual-port) ─
    printf("test 2: simultaneous write and read (different addresses)\n");
    // Pre-write addr 10.
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_addr_i = 10;
    sim.dut->wr_data_i = 0x12345;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Simultaneous: write addr 11, read addr 10.
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_addr_i = 11;
    sim.dut->wr_data_i = 0x67890;
    sim.dut->rd_en_i   = 1;
    sim.dut->rd_addr_i = 10;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.dut->rd_en_i = 0;
    sim.check((sim.dut->rd_data_o & 0xFFFFF) == 0x12345, "read addr 10 while writing addr 11");
    sim.tick();

    // Verify addr 11 was written.
    sim.dut->rd_en_i   = 1;
    sim.dut->rd_addr_i = 11;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check((sim.dut->rd_data_o & 0xFFFFF) == 0x67890, "addr 11 written correctly");
    sim.tick();

    // ── Test 3: Multiple addresses ──────────────────────
    printf("test 3: multiple addresses\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_addr_i = 20 + i;
        sim.dut->wr_data_i = 0xF0000 + i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;
    sim.tick();

    for (int i = 0; i < 8; i++) {
        sim.dut->rd_en_i   = 1;
        sim.dut->rd_addr_i = 20 + i;
        sim.tick();
        sim.dut->rd_en_i = 0;
        sim.check((sim.dut->rd_data_o & 0xFFFFF) == (uint64_t)(0xF0000 + i),
                  "addr[" + std::to_string(20+i) + "]");
        sim.tick();
    }

    // ── Test 4: Output holds when not reading ───────────
    printf("test 4: output holds\n");
    uint64_t held = sim.dut->rd_data_o;
    sim.tick();
    sim.tick();
    sim.check(sim.dut->rd_data_o == held, "output holds");

    return sim.finish();
}

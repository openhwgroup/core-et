// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_sram Ports=1 — Verilator test.
// Tests single-port mode where read blocks write.

#include "Vprim_fifo_sram_1p_tb.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_sram_1p_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    sim.dut->wr_en_i   = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->rd_en_i   = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i     = 0;
    sim.reset();

    // ── Test 1: Basic write/read works ──────────────────
    printf("test 1: basic write and read (Ports=1)\n");
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xCAFE;
    sim.dut->eval();
    sim.check(sim.dut->wr_done_o == 1, "wr_done when no read contention");
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    sim.dut->rd_en_i = 1;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,      "rd_valid 1 cycle after rd_en");
    sim.check(sim.dut->rd_data_o  == 0xCAFE, "rd_data matches");
    sim.tick();

    // ── Test 2: Simultaneous read+write — read wins ─────
    printf("test 2: simultaneous r+w — read wins, write blocked\n");
    // Write two items first.
    for (int i = 0; i < 2; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_data_i = 0xA0 + i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Now assert both rd_en and wr_en simultaneously.
    sim.dut->rd_en_i   = 1;
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xFF;
    sim.dut->eval();
    sim.check(sim.dut->wr_done_o == 0, "wr_done BLOCKED during simultaneous read");
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,   "rd_valid from the read");
    sim.check(sim.dut->rd_data_o  == 0xA0, "read got correct data (0xA0)");
    sim.tick();

    // The write of 0xFF should NOT have gone through.
    // Write pointer should not have advanced. Write 0xBB now, then read two items.
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xBB;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Read: should get 0xA1 (second item from earlier), then 0xBB.
    sim.dut->rd_en_i = 1;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,   "rd_valid for second read");
    sim.check(sim.dut->rd_data_o  == 0xA1, "got 0xA1 — write of 0xFF was correctly blocked");
    sim.tick();

    sim.dut->rd_en_i = 1;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,   "rd_valid for third read");
    sim.check(sim.dut->rd_data_o  == 0xBB, "got 0xBB — subsequent write went through");
    sim.tick();

    // ── Test 3: Write when no read contention ───────────
    printf("test 3: write succeeds when no read active\n");
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xDD;
    sim.dut->rd_en_i   = 0;
    sim.dut->eval();
    sim.check(sim.dut->wr_done_o == 1, "wr_done when rd_en=0");
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Read it back.
    sim.dut->rd_en_i = 1;
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,   "rd_valid");
    sim.check(sim.dut->rd_data_o  == 0xDD, "read back 0xDD");
    sim.tick();

    return sim.finish();
}

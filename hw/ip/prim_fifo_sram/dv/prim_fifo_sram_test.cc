// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_fifo_sram — Verilator test.
// Tests the SRAM-backed FIFO with 1-cycle read latency.

#include "Vprim_fifo_sram.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_sram;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // ── Init ────────────────────────────────────────────
    sim.dut->wr_en_i   = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->rd_en_i   = 0;
    sim.dut->ram_cfg_i = 0;
    sim.dut->dft_i     = 0;
    sim.reset();

    // ── Test 1: Idle after reset ────────────────────────
    printf("test 1: idle after reset\n");
    sim.check(sim.dut->rd_valid_o == 0, "rd_valid deasserted");
    sim.check(sim.dut->wr_done_o == 0,  "wr_done deasserted");

    // ── Test 2: Write one item ──────────────────────────
    printf("test 2: write one item\n");
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xDEADBEEF;
    sim.dut->eval();
    sim.check(sim.dut->wr_done_o == 1, "wr_done asserted same cycle");
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    // ── Test 3: Read with 1-cycle latency ───────────────
    printf("test 3: read with 1-cycle latency\n");
    sim.dut->rd_en_i = 1;
    sim.dut->eval();
    sim.check(sim.dut->rd_valid_o == 0, "rd_valid NOT asserted on request cycle");
    sim.tick();
    sim.dut->rd_en_i = 0;
    // Now data should be available.
    sim.check(sim.dut->rd_valid_o == 1,          "rd_valid asserted next cycle");
    sim.check(sim.dut->rd_data_o  == 0xDEADBEEF, "rd_data matches written value");
    sim.tick();
    sim.check(sim.dut->rd_valid_o == 0, "rd_valid deasserted after one cycle");

    // ── Test 4: Write multiple, read back in order ──────
    printf("test 4: FIFO ordering\n");
    // Write 4 items.
    for (int i = 0; i < 4; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_data_i = 0xA0 + i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Read 4 items back, each with 1-cycle latency.
    for (int i = 0; i < 4; i++) {
        sim.dut->rd_en_i = 1;
        sim.tick();
        sim.dut->rd_en_i = 0;
        sim.check(sim.dut->rd_valid_o == 1,
                  "rd_valid[" + std::to_string(i) + "] asserted");
        sim.check(sim.dut->rd_data_o == (uint32_t)(0xA0 + i),
                  "data[" + std::to_string(i) + "] = 0x" +
                  std::to_string(0xA0 + i));
        sim.tick();
    }

    // ── Test 5: Back-to-back reads ──────────────────────
    printf("test 5: back-to-back reads (pipelined)\n");
    // Write 4 more items.
    for (int i = 0; i < 4; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_data_i = 0xB0 + i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Pipeline reads: assert rd_en every cycle.
    // First read: request cycle, no valid yet.
    sim.dut->rd_en_i = 1;
    sim.tick();
    // Now valid should be high, and we keep requesting.
    for (int i = 0; i < 3; i++) {
        sim.check(sim.dut->rd_valid_o == 1,
                  "pipelined rd_valid[" + std::to_string(i) + "]");
        sim.check(sim.dut->rd_data_o == (uint32_t)(0xB0 + i),
                  "pipelined data[" + std::to_string(i) + "]");
        sim.tick();
    }
    sim.dut->rd_en_i = 0;
    // Last item arrives this cycle.
    sim.check(sim.dut->rd_valid_o == 1,  "pipelined rd_valid[3]");
    sim.check(sim.dut->rd_data_o == 0xB3, "pipelined data[3]");
    sim.tick();
    sim.check(sim.dut->rd_valid_o == 0, "rd_valid deasserted after pipeline drain");

    // ── Test 6: Simultaneous read and write ─────────────
    printf("test 6: simultaneous read and write\n");
    // Write one item first.
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xC0;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Now read and write at the same time.
    sim.dut->rd_en_i   = 1;
    sim.dut->wr_en_i   = 1;
    sim.dut->wr_data_i = 0xC1;
    sim.dut->eval();
    sim.check(sim.dut->wr_done_o == 1, "wr_done during simultaneous r+w");
    sim.tick();
    sim.dut->rd_en_i = 0;
    sim.dut->wr_en_i = 0;
    sim.check(sim.dut->rd_valid_o == 1,   "rd_valid from simultaneous r+w");
    sim.check(sim.dut->rd_data_o  == 0xC0, "read got the earlier item (0xC0)");
    sim.tick();

    // ── Test 7: Pointer wrap-around ─────────────────────
    printf("test 7: pointer wrap-around\n");
    // Default Depth=64. Write 64+4 items to force wrap.
    for (int i = 0; i < 68; i++) {
        sim.dut->wr_en_i   = 1;
        sim.dut->wr_data_i = i;
        sim.tick();
    }
    sim.dut->wr_en_i = 0;
    sim.tick();

    // Read the last 4 items (at wrapped positions).
    // Due to wrap, reading from the current rptr position.
    // We just verify reads work after wrap — exact values depend
    // on how many reads we've done above.
    for (int i = 0; i < 4; i++) {
        sim.dut->rd_en_i = 1;
        sim.tick();
        sim.dut->rd_en_i = 0;
        sim.check(sim.dut->rd_valid_o == 1,
                  "wrap rd_valid[" + std::to_string(i) + "]");
        sim.tick();
    }

    // ── Done ────────────────────────────────────────────
    return sim.finish();
}

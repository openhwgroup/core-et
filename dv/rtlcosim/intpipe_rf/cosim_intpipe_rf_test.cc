// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_rf (new) vs intpipe_rf (original).
//
// Sequential register file wrapper. 2 read ports, 1 write port.
// Tests:
//   1. Write to non-zero entries, read back from both ports
//   2. x0 hardwired zero (entries 0 and 32 for threads 0 and 1)
//   3. x31 parallel read (entries 31 and 63)
//   4. 20K random cycles

#include "Vcosim_intpipe_rf_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdlib>

using DUT = Vcosim_intpipe_rf_tb;

static uint32_t seed = 0xDEADBEEF;
static uint32_t xorshift32() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

static uint64_t rand64() {
    return ((uint64_t)xorshift32() << 32) | xorshift32();
}

static void step_half(CosimCtrl<DUT>& sim, int clk_level) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

// Compare 128-bit wb_x31_reg as two 64-bit halves.
static void compare_x31(CosimCtrl<DUT>& sim, const uint32_t* orig, const uint32_t* newv) {
    uint64_t orig_lo = (uint64_t)orig[0] | ((uint64_t)orig[1] << 32);
    uint64_t orig_hi = (uint64_t)orig[2] | ((uint64_t)orig[3] << 32);
    uint64_t new_lo  = (uint64_t)newv[0] | ((uint64_t)newv[1] << 32);
    uint64_t new_hi  = (uint64_t)newv[2] | ((uint64_t)newv[3] << 32);
    sim.compare("wb_x31[0]", orig_lo, new_lo);
    sim.compare("wb_x31[1]", orig_hi, new_hi);
}

// Write a value to a register using the proper 2-phase protocol:
//   Phase 1 (negedge): latch write data (wr_en_early)
//   Phase 2 (posedge): commit write (wr_en)
static void write_reg(CosimCtrl<DUT>& sim, uint8_t tid, uint8_t addr, uint64_t data) {
    auto* d = sim.dut.get();

    // Negedge: latch write data
    d->stim_wr_en_early = 1;
    d->stim_wr_data = data;
    d->stim_wr_thread_id = tid;
    d->stim_wr_addr = addr;
    d->stim_wr_en = 0;
    sim.tick();

    // Posedge: commit
    d->stim_wr_en = 1;
    d->stim_wr_en_early = 0;
    sim.tick();

    d->stim_wr_en = 0;
}

// Write a value using the true "1 phase early" protocol:
//   High phase before write: wr_en_early/data/addr become valid
//   Negedge: LATCH_P2 / data-latch capture
//   Following posedge: wr_en commits the write
//
// This is intentionally tighter than write_reg() and is the case most likely
// to expose a mistranslation of LATCH_P2 into edge-triggered storage.
static void write_reg_one_phase_early(CosimCtrl<DUT>& sim, uint8_t tid, uint8_t addr, uint64_t data) {
    auto* d = sim.dut.get();

    d->stim_wr_thread_id = tid;
    d->stim_wr_addr = addr;
    d->stim_wr_data = data;
    d->stim_wr_en_early = 1;
    d->stim_wr_en = 1;

    step_half(sim, 0);  // negedge: phase-2 latch / RF data latch
    step_half(sim, 1);  // posedge: commit write

    d->stim_wr_en = 0;
    d->stim_wr_en_early = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->stim_rd_en = 0x3;
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr_0 = 0;
    d->stim_rd_addr_1 = 0;
    d->stim_wr_en = 0;
    d->stim_wr_en_early = 0;
    d->stim_wr_thread_id = 0;
    d->stim_wr_addr = 0;
    d->stim_wr_data = 0;

    sim.reset();

    // ── Phase 1: Write to all non-zero entries, read back ──
    printf("=== Phase 1: Write non-zero entries, read back ===\n");

    // Write entries 1..31 (thread 0) and 33..63 (thread 1)
    for (int tid = 0; tid < 2; tid++) {
        for (int reg = 1; reg < 32; reg++) {
            uint64_t wdata = 0x1000000000000000ULL + (uint64_t)(tid * 32 + reg);
            write_reg(sim, tid, reg, wdata);
        }
    }

    // Read back from both threads
    for (int tid = 0; tid < 2; tid++) {
        d->stim_rd_thread_id = tid;
        for (int reg = 0; reg < 32; reg++) {
            d->stim_rd_addr_0 = reg;
            d->stim_rd_addr_1 = (reg + 7) & 0x1F;
            d->eval();
            sim.compare("rd_data_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
            sim.compare("rd_data_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);
            compare_x31(sim, d->orig_wb_x31_reg_o.data(), d->new_wb_x31_reg_o.data());
        }
    }
    sim.tick();

    // ── Phase 1b: True one-phase-early write timing ──
    // Current cosim only exercised wr_en_early a full cycle before wr_en.
    // The original interface is tighter: wr_en_early is only one phase early.
    // That should distinguish a real LATCH_P2 from a negedge FF feeding a
    // primitive that also samples on negedge.
    printf("=== Phase 1b: One-phase-early write timing ===\n");

    write_reg(sim, 0, 5, 0x1111111111111111ULL);
    write_reg_one_phase_early(sim, 0, 5, 0x2222222222222222ULL);
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr_0 = 5;
    d->stim_rd_addr_1 = 5;
    d->eval();
    sim.compare("phase1b_t0_rd_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
    sim.compare("phase1b_t0_rd_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);

    write_reg(sim, 1, 6, 0x3333333333333333ULL);
    write_reg_one_phase_early(sim, 1, 6, 0x4444444444444444ULL);
    d->stim_rd_thread_id = 1;
    d->stim_rd_addr_0 = 6;
    d->stim_rd_addr_1 = 6;
    d->eval();
    sim.compare("phase1b_t1_rd_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
    sim.compare("phase1b_t1_rd_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);
    sim.tick();

    // ── Phase 2: x0 hardwired zero ──
    printf("=== Phase 2: x0 hardwired zero ===\n");

    // Try to write to x0 for both threads
    for (int tid = 0; tid < 2; tid++) {
        write_reg(sim, tid, 0, 0xFFFFFFFFFFFFFFFFULL);

        // Read back — should be 0
        d->stim_rd_thread_id = tid;
        d->stim_rd_addr_0 = 0;
        d->stim_rd_addr_1 = 0;
        d->eval();
        sim.compare("x0_rd_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
        sim.compare("x0_rd_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);
    }
    sim.tick();

    // ── Phase 3: x31 parallel read ──
    printf("=== Phase 3: x31 parallel read ===\n");

    // Write distinctive values to x31 for both threads
    write_reg(sim, 0, 31, 0xCAFEBABE12345678ULL);
    write_reg(sim, 1, 31, 0xDEADC0DE9ABCDEF0ULL);

    // Check parallel read
    d->eval();
    compare_x31(sim, d->orig_wb_x31_reg_o.data(), d->new_wb_x31_reg_o.data());

    // Also read via normal ports
    for (int tid = 0; tid < 2; tid++) {
        d->stim_rd_thread_id = tid;
        d->stim_rd_addr_0 = 31;
        d->stim_rd_addr_1 = 31;
        d->eval();
        sim.compare("x31_rd_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
        sim.compare("x31_rd_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);
    }
    sim.tick();

    // ── Phase 4: 20K random cycles ──
    // Follow the exact same 2-phase write protocol as the deterministic tests
    // and as the prim_rf_2r1w cosim:
    //   Cycle A: set wr_data_en_1p, wr_data, wr_addr -> tick (negedge latches data)
    //   Cycle B: set wr_en -> tick (posedge commits write) -> clear wr_en
    printf("=== Phase 4: Random stimulus (20000 cycles) ===\n");
    for (int i = 0; i < 20000; i++) {
        uint32_t r = xorshift32();

        // Random read addresses (using both threads)
        d->stim_rd_thread_id = r & 1;
        d->stim_rd_addr_0 = (r >> 1) & 0x1F;
        d->stim_rd_addr_1 = (r >> 6) & 0x1F;
        d->stim_rd_en = (r >> 11) & 0x3;

        // Random negedge data latch enable
        d->stim_wr_en_early = (r >> 13) & 1;

        // Random write data (only meaningful when wr_en_early is set)
        d->stim_wr_data = rand64();

        // Random write target
        d->stim_wr_thread_id = (r >> 14) & 1;
        d->stim_wr_addr = (r >> 15) & 0x1F;

        sim.tick();  // negedge: latch data if wr_en_early=1; posedge: commit if wr_en=1

        // Random write enable for NEXT posedge
        d->stim_wr_en = (r >> 20) & 1;
        d->stim_wr_en_early = 0;  // clear early enable after its cycle

        sim.tick();  // negedge: nothing latched; posedge: write if wr_en=1

        // Compare all outputs after write settles
        d->eval();
        sim.compare("rand_rd_0", d->orig_rd_data_0_o, d->new_rd_data_0_o);
        sim.compare("rand_rd_1", d->orig_rd_data_1_o, d->new_rd_data_1_o);
        compare_x31(sim, d->orig_wb_x31_reg_o.data(), d->new_wb_x31_reg_o.data());

        d->stim_wr_en = 0;
    }

    return sim.finish();
}

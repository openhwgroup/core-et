// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: prim_rf_2r1w vs rf_latch_2r_1w.
//
// Parameters (intpipe_rf): Width=64, Entries=64,
//   Zero={2{32'b1}} (entries 0,32), Parallel={2{32'h80000000}} (entries 31,63),
//   ParallelW=128, Level2CkGate=1.
//
// Test phases:
//   1. Write to non-zero entries, read back from both ports
//   2. Write to zero entries (0, 32) — should read back as 0
//   3. Read parallel output (x31 registers)
//   4. 50K random cycles

#include "Vcosim_rf_2r1w_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_rf_2r1w_tb;

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

static void compare_par(CosimCtrl<DUT>& sim, const uint32_t* orig, const uint32_t* newv);

static void compare_outputs(CosimCtrl<DUT>& sim, const char* rd_a_name,
                            const char* rd_b_name) {
    auto* d = sim.dut.get();
    sim.compare(rd_a_name, d->orig_rd_data_a_o, d->new_rd_data_a_o);
    sim.compare(rd_b_name, d->orig_rd_data_b_o, d->new_rd_data_b_o);
    compare_par(sim, d->orig_rd_par_o.data(), d->new_rd_par_o.data());
}

static void eval_and_compare(CosimCtrl<DUT>& sim, const char* rd_a_name, const char* rd_b_name) {
    sim.dut->eval();
    compare_outputs(sim, rd_a_name, rd_b_name);
}

static void step_half(CosimCtrl<DUT>& sim, int clk_level, const char* rd_a_name,
                      const char* rd_b_name) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    compare_outputs(sim, rd_a_name, rd_b_name);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

// Compare 128-bit rd_par as two 64-bit halves.
// Verilator exposes >64-bit signals as WData (uint32_t[]).
static void compare_par(CosimCtrl<DUT>& sim, const uint32_t* orig, const uint32_t* newv) {
    uint64_t orig_lo = (uint64_t)orig[0] | ((uint64_t)orig[1] << 32);
    uint64_t orig_hi = (uint64_t)orig[2] | ((uint64_t)orig[3] << 32);
    uint64_t new_lo  = (uint64_t)newv[0] | ((uint64_t)newv[1] << 32);
    uint64_t new_hi  = (uint64_t)newv[2] | ((uint64_t)newv[3] << 32);
    sim.compare("rd_par[63:0]",   orig_lo, new_lo);
    sim.compare("rd_par[127:64]", orig_hi, new_hi);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->rd_addr_a_i = 0;
    d->rd_addr_b_i = 0;
    d->wr_en_i = 0;
    d->wr_data_en_1p_i = 0;
    d->wr_addr_i = 0;
    d->wr_data_i = 0;

    sim.reset();

    // ── Phase 1: Write to non-zero entries, read back from both ports ──
    printf("=== Phase 1: Write non-zero entries, read back ===\n");

    // Write entries 1..31 (thread 0, skipping 0) and 33..63 (thread 1, skipping 32)
    for (int entry = 0; entry < 64; entry++) {
        // Skip zero-hardwired entries (0 and 32)
        if (entry == 0 || entry == 32) continue;

        uint64_t wdata = 0x1000000000000000ULL + (uint64_t)entry;

        // Negedge: latch write data
        d->wr_data_en_1p_i = 1;
        d->wr_data_i = wdata;
        d->wr_addr_i = entry;
        sim.tick();

        // Posedge: write
        d->wr_en_i = 1;
        d->wr_data_en_1p_i = 0;
        sim.tick();
        d->wr_en_i = 0;
    }

    // Read back all entries from both ports
    for (int entry = 0; entry < 64; entry++) {
        d->rd_addr_a_i = entry;
        d->rd_addr_b_i = (entry + 17) & 0x3F;  // stagger port B
        eval_and_compare(sim, "rd_data_a", "rd_data_b");
    }
    sim.tick();

    // ── Phase 2: Write to zero entries — should read back as 0 ──
    printf("=== Phase 2: Write to zero-hardwired entries (0 and 32) ===\n");

    for (int entry : {0, 32}) {
        uint64_t wdata = 0xFFFFFFFFFFFFFFFFULL;

        d->wr_data_en_1p_i = 1;
        d->wr_data_i = wdata;
        d->wr_addr_i = entry;
        sim.tick();

        d->wr_en_i = 1;
        d->wr_data_en_1p_i = 0;
        sim.tick();
        d->wr_en_i = 0;

        // Read back — should be 0
        d->rd_addr_a_i = entry;
        d->rd_addr_b_i = entry;
        eval_and_compare(sim, "zero_rd_a", "zero_rd_b");
    }
    sim.tick();

    // ── Phase 3: Read parallel output (entries 31 and 63) ──
    printf("=== Phase 3: Parallel read of x31 registers ===\n");

    // Write distinctive values to entry 31 and 63
    for (int entry : {31, 63}) {
        uint64_t wdata = (entry == 31) ? 0xCAFEBABE12345678ULL : 0xDEADC0DE9ABCDEF0ULL;

        d->wr_data_en_1p_i = 1;
        d->wr_data_i = wdata;
        d->wr_addr_i = entry;
        sim.tick();

        d->wr_en_i = 1;
        d->wr_data_en_1p_i = 0;
        sim.tick();
        d->wr_en_i = 0;
    }

    // Compare parallel read
    eval_and_compare(sim, "par_rd_a", "par_rd_b");

    // Also read them via normal ports to double-check
    d->rd_addr_a_i = 31;
    d->rd_addr_b_i = 63;
    eval_and_compare(sim, "rd_x31_a", "rd_x31_b");
    sim.tick();

    // ── Phase 4: Half-cycle timing ──
    printf("=== Phase 4: Half-cycle timing ===\n");
    d->rd_addr_a_i = 12;
    d->rd_addr_b_i = 45;
    d->wr_addr_i = 45;
    d->wr_data_en_1p_i = 1;
    d->wr_en_i = 0;
    d->wr_data_i = 0x123456789ABCDEF0ULL;
    step_half(sim, 0, "half_rd_a", "half_rd_b");
    d->wr_data_en_1p_i = 0;
    d->wr_data_i = 0x0FEDCBA987654321ULL;
    eval_and_compare(sim, "half_low_mut_a", "half_low_mut_b");
    d->wr_en_i = 1;
    step_half(sim, 1, "half_high_a", "half_high_b");
    d->wr_en_i = 0;
    d->rd_addr_a_i = 45;
    d->rd_addr_b_i = 31;
    eval_and_compare(sim, "half_readback_a", "half_readback_b");

    d->wr_addr_i = 46;
    d->wr_data_en_1p_i = 0;
    d->wr_en_i = 0;
    step_half(sim, 0, "half_idle_a", "half_idle_b");
    d->wr_data_en_1p_i = 1;
    d->wr_data_i = 0xCAFED00D1234ABCDULL;
    eval_and_compare(sim, "half_late_a", "half_late_b");
    d->wr_en_i = 1;
    step_half(sim, 1, "half_commit_a", "half_commit_b");
    d->wr_data_en_1p_i = 0;
    d->wr_en_i = 0;

    // ── Phase 5: Half-cycle random stimulus ──
    printf("=== Phase 5: Half-cycle random (2048 iterations) ===\n");
    for (int i = 0; i < 2048; i++) {
        uint32_t r = xorshift32();
        d->rd_addr_a_i = r & 0x3F;
        d->rd_addr_b_i = (r >> 6) & 0x3F;
        d->wr_data_en_1p_i = (r >> 12) & 1;
        d->wr_addr_i = (r >> 13) & 0x3F;
        d->wr_data_i = rand64();
        d->wr_en_i = 0;
        step_half(sim, 0, "half_rand_a", "half_rand_b");

        r = xorshift32();
        d->rd_addr_a_i = r & 0x3F;
        d->rd_addr_b_i = (r >> 6) & 0x3F;
        d->wr_data_en_1p_i = (r >> 12) & 1;
        d->wr_data_i = rand64();
        eval_and_compare(sim, "half_mut_a", "half_mut_b");
        d->wr_en_i = (r >> 19) & 1;
        step_half(sim, 1, "half_commit_rand_a", "half_commit_rand_b");
        d->wr_en_i = 0;
    }

    // ── Phase 6: 50K random cycles ──
    printf("=== Phase 6: Random stimulus (50000 cycles) ===\n");
    for (int i = 0; i < 50000; i++) {
        uint32_t r = xorshift32();

        // Random read addresses
        d->rd_addr_a_i = r & 0x3F;
        d->rd_addr_b_i = (r >> 6) & 0x3F;

        // Random negedge data latch enable
        d->wr_data_en_1p_i = (r >> 12) & 1;

        // Random write data
        d->wr_data_i = rand64();

        // Random write address
        d->wr_addr_i = (r >> 13) & 0x3F;

        sim.tick();

        // Random write enable (posedge)
        d->wr_en_i = (r >> 19) & 1;

        // Compare all outputs
        eval_and_compare(sim, "rand_rd_a", "rand_rd_b");

        sim.tick();
        d->wr_en_i = 0;
    }

    return sim.finish();
}

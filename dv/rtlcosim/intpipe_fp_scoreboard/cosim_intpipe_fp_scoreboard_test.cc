// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_fp_scoreboard (new) vs (original).
//
// Pure combinational. Drives random scoreboard states and read addresses,
// compares rd_data and rd_data_dcache outputs. 50K random combinations.

#include "Vcosim_intpipe_fp_scoreboard_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_intpipe_fp_scoreboard_tb;

static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    return ((uint64_t)xorshift32() << 32) | xorshift32();
}

// Build a minion_reg_dest_t packed value: {fp[1], addr[5], thread_id[1]}
static uint8_t make_reg_dest(uint8_t fp, uint8_t addr, uint8_t tid) {
    return ((fp & 1) << 6) | ((addr & 0x1F) << 1) | (tid & 1);
}

// Build dcache_scoreboard_t: {valid[8], dest[8][7]} = 64 bits
static uint64_t make_dcache_sb(uint8_t valid, const uint8_t dest[8]) {
    uint64_t result = 0;
    for (int i = 0; i < 8; i++)
        result |= ((uint64_t)(dest[i] & 0x7F)) << (i * 7);
    result |= ((uint64_t)valid) << 56;
    return result;
}

static void drive_and_compare(CosimCtrl<DUT>& sim, const char* desc) {
    auto* d = sim.dut.get();
    d->eval();

    sim.compare(std::string(desc) + "_rd_data",
                (uint64_t)d->orig_rd_data_o,
                (uint64_t)d->new_rd_data_o);
    sim.compare(std::string(desc) + "_rd_data_dcache",
                (uint64_t)d->orig_rd_data_dcache_o,
                (uint64_t)d->new_rd_data_dcache_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;

    // ── Phase 1: Targeted tests ──
    printf("phase 1: targeted tests\n");

    // All zeros — no hazard
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr_0 = 0;
    d->stim_rd_addr_1 = 0;
    d->stim_rd_addr_2 = 0;
    d->stim_dcache_sb = 0;
    for (int w = 0; w < 6; w++) d->stim_vpu_sb[w] = 0;
    drive_and_compare(sim, "all_zero");

    // Test dcache FP hazard: dcache dest fp=1, addr=5, tid=0
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr_0 = 5;
    d->stim_rd_addr_1 = 10;
    d->stim_rd_addr_2 = 15;
    {
        uint8_t dests[8] = {};
        dests[0] = make_reg_dest(1, 5, 0);  // FP match for port 0
        d->stim_dcache_sb = make_dcache_sb(0x01, dests);
    }
    drive_and_compare(sim, "dcache_fp_match_port0");

    // Thread mismatch
    d->stim_rd_thread_id = 1;
    drive_and_compare(sim, "dcache_thread_mismatch");

    // FP=0 in dcache should not match (FP scoreboard expects fp=1)
    d->stim_rd_thread_id = 0;
    {
        uint8_t dests[8] = {};
        dests[0] = make_reg_dest(0, 5, 0);  // int, not fp
        d->stim_dcache_sb = make_dcache_sb(0x01, dests);
    }
    drive_and_compare(sim, "dcache_int_no_match");

    d->stim_dcache_sb = 0;

    // ── Phase 2: Random sweep (50K) ──
    printf("phase 2: random sweep (50000 combinations)\n");
    int phase2_mm = 0;

    for (int i = 0; i < 50000; i++) {
        d->stim_rd_thread_id = xorshift32() & 1;
        d->stim_rd_addr_0 = xorshift32() & 0x1F;
        d->stim_rd_addr_1 = xorshift32() & 0x1F;
        d->stim_rd_addr_2 = xorshift32() & 0x1F;

        d->stim_dcache_sb = xorshift64();

        // vpu scoreboard is 164 bits = 6 x 32-bit words
        uint32_t vpu_words[6];
        for (int w = 0; w < 6; w++)
            vpu_words[w] = xorshift32();
        vpu_words[5] &= 0x0000000F;
        for (int w = 0; w < 6; w++)
            d->stim_vpu_sb[w] = vpu_words[w];

        d->eval();

        int mm_before = sim.mismatches;
        sim.compare("rand_rd_data",
                    (uint64_t)d->orig_rd_data_o,
                    (uint64_t)d->new_rd_data_o);
        sim.compare("rand_rd_data_dcache",
                    (uint64_t)d->orig_rd_data_dcache_o,
                    (uint64_t)d->new_rd_data_dcache_o);

        if (sim.mismatches > mm_before) {
            phase2_mm++;
            if (phase2_mm <= 20) {
                printf("  iter=%d tid=%d addr={%d,%d,%d} dcache=0x%016llx\n",
                       i, d->stim_rd_thread_id,
                       d->stim_rd_addr_0, d->stim_rd_addr_1, d->stim_rd_addr_2,
                       (unsigned long long)d->stim_dcache_sb);
            }
        }
    }

    if (phase2_mm > 20)
        printf("  ... and %d more mismatches (truncated)\n", phase2_mm - 20);

    return sim.finish();
}

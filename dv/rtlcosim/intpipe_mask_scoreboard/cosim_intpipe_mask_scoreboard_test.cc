// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_mask_scoreboard (new) vs (original).
//
// Pure combinational. Drives random VPU mask scoreboard states and
// rd/wd addresses, compares rd_data and wd_data outputs.
// 50K random combinations.

#include "Vcosim_intpipe_mask_scoreboard_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_intpipe_mask_scoreboard_tb;

static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static void drive_and_compare(CosimCtrl<DUT>& sim, const char* desc) {
    auto* d = sim.dut.get();
    d->eval();

    sim.compare(std::string(desc) + "_rd_data",
                (uint64_t)d->orig_rd_data_o,
                (uint64_t)d->new_rd_data_o);
    sim.compare(std::string(desc) + "_wd_data",
                (uint64_t)d->orig_wd_data_o,
                (uint64_t)d->new_wd_data_o);
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
    d->stim_rd_addr = 0;
    d->stim_wd_addr = 0;
    for (int w = 0; w < 6; w++) d->stim_vpu_sb[w] = 0;
    drive_and_compare(sim, "all_zero");

    // All ones in addresses, no valid scoreboard
    d->stim_rd_addr = 0xFF;
    d->stim_wd_addr = 0xFF;
    drive_and_compare(sim, "all_addr_ones_no_valid");

    // Single mask valid entry, matching thread and address
    // vpu_scoreboard_t layout (MSB to LSB):
    //   fp_valid[14], fp_dest[14][7], toint_valid[4], toint_dest[4][7],
    //   mask_valid[4], mask_dest[4][4]
    // mask_dest[0] is at bits [3:0]: {addr[3], thread_id[1]} = 4 bits
    // mask_valid[0] is at bit 16
    // So mask section is bits [19:0] of the full 164-bit struct
    //   bits [15:0]  = mask_dest[3:0] (4x4 = 16 bits)
    //   bits [19:16] = mask_valid[3:0]

    // Set mask_valid[0]=1, mask_dest[0]={addr=2, tid=0}
    // mask_dest[0] packed = {addr[2:0], thread_id} = {010, 0} = 0x4
    {
        uint32_t vpu[6] = {};
        vpu[0] = (1 << 16) | 0x4;  // mask_valid[0]=1, mask_dest[0]=0x4 (addr=2, tid=0)
        for (int w = 0; w < 6; w++)
            d->stim_vpu_sb[w] = vpu[w];
    }
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr = (1 << 2);  // bit 2 set — matches addr=2
    d->stim_wd_addr = (1 << 2);
    drive_and_compare(sim, "single_mask_match");

    // Thread mismatch
    d->stim_rd_thread_id = 1;
    drive_and_compare(sim, "mask_thread_mismatch");

    // Address mismatch
    d->stim_rd_thread_id = 0;
    d->stim_rd_addr = (1 << 3);  // bit 3, not bit 2
    d->stim_wd_addr = (1 << 3);
    drive_and_compare(sim, "mask_addr_mismatch");

    // rd matches but wd doesn't
    d->stim_rd_addr = (1 << 2);
    d->stim_wd_addr = (1 << 3);
    drive_and_compare(sim, "rd_match_wd_no");

    for (int w = 0; w < 6; w++) d->stim_vpu_sb[w] = 0;

    // ── Phase 2: Random sweep (50K) ──
    printf("phase 2: random sweep (50000 combinations)\n");
    int phase2_mm = 0;

    for (int i = 0; i < 50000; i++) {
        d->stim_rd_thread_id = xorshift32() & 1;
        d->stim_rd_addr = xorshift32() & 0xFF;
        d->stim_wd_addr = xorshift32() & 0xFF;

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
        sim.compare("rand_wd_data",
                    (uint64_t)d->orig_wd_data_o,
                    (uint64_t)d->new_wd_data_o);

        if (sim.mismatches > mm_before) {
            phase2_mm++;
            if (phase2_mm <= 20) {
                printf("  iter=%d tid=%d rd_addr=0x%02x wd_addr=0x%02x\n",
                       i, d->stim_rd_thread_id,
                       d->stim_rd_addr, d->stim_wd_addr);
            }
        }
    }

    if (phase2_mm > 20)
        printf("  ... and %d more mismatches (truncated)\n", phase2_mm - 20);

    return sim.finish();
}

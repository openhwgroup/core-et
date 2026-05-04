// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: prim_rf_1r1w_diff vs rf_latch_1r_1w_diff_widths.
//
// Tests all read addresses, write with partial enables, wrap-around reads,
// and heavy random stimulus.

#include "Vcosim_rf_1r1w_diff_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_rf_1r1w_diff_tb;

static uint32_t seed = 0xDEADBEEF;
static uint32_t xorshift32() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

static void set_wr_data(DUT* d, const uint32_t data[8]) {
    for (int i = 0; i < 8; i++)
        d->wr_data_i[i] = data[i];
}

static void compare_rd(CosimCtrl<DUT>& sim, const char* name) {
    auto* d = sim.dut.get();
    sim.compare(name, (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
}

static void eval_and_compare(CosimCtrl<DUT>& sim, const char* name) {
    sim.dut->eval();
    compare_rd(sim, name);
}

static void step_half(CosimCtrl<DUT>& sim, int clk_level, const char* name) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    compare_rd(sim, name);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;
    d->rd_addr_i = 0;
    d->wr_addr_i = 0;
    d->wr_en_i = 0;
    d->wr_data_en_1p_i = 0;
    for (int i = 0; i < 8; i++) d->wr_data_i[i] = 0;

    sim.reset();

    // Phase 1: Write to buffer 0 with all enables, read all 32 addresses
    printf("=== Phase 1: Full write buffer 0, read all addresses ===\n");
    uint32_t data0[8] = {0xAABBCCDD, 0x11223344, 0x55667788, 0x99AABBCC,
                         0xDDEEFF00, 0x12345678, 0x9ABCDEF0, 0xFEDCBA98};
    // Set prediction
    d->wr_data_en_1p_i = 0xFF;
    set_wr_data(d, data0);
    d->wr_addr_i = 0;
    sim.tick();

    // Write
    d->wr_en_i = 0xFF;
    d->wr_data_en_1p_i = 0;
    sim.tick();
    d->wr_en_i = 0;

    // Read all addresses
    for (int addr = 0; addr < 32; addr++) {
        d->rd_addr_i = addr;
        d->eval();
        sim.compare("rd_data", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    }
    sim.tick();

    // Phase 2: Write to buffer 1
    printf("=== Phase 2: Full write buffer 1, read all addresses ===\n");
    uint32_t data1[8] = {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
                         0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20};
    d->wr_data_en_1p_i = 0xFF;
    set_wr_data(d, data1);
    d->wr_addr_i = 1;
    sim.tick();

    d->wr_en_i = 0xFF;
    d->wr_data_en_1p_i = 0;
    sim.tick();
    d->wr_en_i = 0;

    for (int addr = 0; addr < 32; addr++) {
        d->rd_addr_i = addr;
        d->eval();
        sim.compare("rd_data", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    }
    sim.tick();

    // Phase 3: Partial write enables
    printf("=== Phase 3: Partial write enables ===\n");
    uint32_t data_partial[8] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    // Only enable words 2 and 5
    d->wr_data_en_1p_i = 0x24; // bits 2 and 5
    set_wr_data(d, data_partial);
    d->wr_addr_i = 0;
    sim.tick();

    d->wr_en_i = 0x24;
    d->wr_data_en_1p_i = 0;
    sim.tick();
    d->wr_en_i = 0;

    for (int addr = 0; addr < 32; addr++) {
        d->rd_addr_i = addr;
        d->eval();
        sim.compare("rd_data_partial", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    }
    sim.tick();

    // Phase 4: Wrap-around read (addr 15 reads across buffer 0→1 boundary)
    printf("=== Phase 4: Wrap-around boundary reads ===\n");
    for (int addr = 14; addr <= 17; addr++) {
        d->rd_addr_i = addr;
        d->eval();
        sim.compare("wrap_read", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    }
    // addr 31 wraps from buffer 1 end to buffer 0 start
    d->rd_addr_i = 31;
    d->eval();
    sim.compare("wrap_read_31", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    sim.tick();

    // Phase 5: Half-cycle timing
    printf("=== Phase 5: Half-cycle timing ===\n");
    uint32_t data_half_a[8] = {0xA0A0A0A0, 0xB1B1B1B1, 0xC2C2C2C2, 0xD3D3D3D3,
                               0xE4E4E4E4, 0xF5F5F5F5, 0x06060606, 0x17171717};
    uint32_t data_half_b[8] = {0x11111111, 0x22222222, 0x33333333, 0x44444444,
                               0x55555555, 0x66666666, 0x77777777, 0x88888888};
    d->wr_addr_i = 0;
    d->rd_addr_i = 0;
    d->wr_en_i = 0;
    d->wr_data_en_1p_i = 0x0F;
    set_wr_data(d, data_half_a);
    step_half(sim, 0, "half_low_a");
    d->wr_data_en_1p_i = 0xF0;
    set_wr_data(d, data_half_b);
    eval_and_compare(sim, "half_low_mutate");
    d->wr_en_i = 0x0F;
    step_half(sim, 1, "half_high_a");
    d->wr_en_i = 0;
    d->wr_data_en_1p_i = 0;

    d->wr_addr_i = 1;
    step_half(sim, 0, "half_low_idle");
    d->wr_data_en_1p_i = 0x3C;
    set_wr_data(d, data_half_b);
    eval_and_compare(sim, "half_low_late_enable");
    d->wr_en_i = 0x3C;
    step_half(sim, 1, "half_high_b");
    d->wr_en_i = 0;
    d->wr_data_en_1p_i = 0;

    for (int addr = 0; addr < 32; addr++) {
        d->rd_addr_i = addr;
        d->eval();
        sim.compare("half_readback", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);
    }
    sim.tick();

    // Phase 6: Half-cycle random stimulus
    printf("=== Phase 6: Half-cycle random (4096 iterations) ===\n");
    for (int i = 0; i < 4096; i++) {
        uint32_t r = xorshift32();
        d->rd_addr_i = r & 0x1F;
        d->wr_data_en_1p_i = (r >> 5) & 0xFF;
        d->wr_addr_i = (r >> 13) & 1;
        uint32_t wd_lo[8];
        for (int w = 0; w < 8; w++) wd_lo[w] = xorshift32();
        set_wr_data(d, wd_lo);
        d->wr_en_i = 0;
        step_half(sim, 0, "half_rand_low");

        r = xorshift32();
        d->rd_addr_i = r & 0x1F;
        d->wr_data_en_1p_i = (r >> 7) & 0xFF;
        uint32_t wd_hi[8];
        for (int w = 0; w < 8; w++) wd_hi[w] = xorshift32();
        set_wr_data(d, wd_hi);
        eval_and_compare(sim, "half_rand_mutate");
        d->wr_en_i = (r >> 15) & 0xFF;
        step_half(sim, 1, "half_rand_high");
        d->wr_en_i = 0;
    }

    // Phase 7: Heavy random stimulus (50000 cycles)
    printf("=== Phase 7: Random stimulus (50000 cycles) ===\n");
    for (int i = 0; i < 50000; i++) {
        uint32_t r = xorshift32();

        // Random read address
        d->rd_addr_i = r & 0x1F;

        // Random write prediction (negedge enable)
        d->wr_data_en_1p_i = (r >> 5) & 0xFF;

        // Random write data
        uint32_t wd[8];
        for (int w = 0; w < 8; w++) wd[w] = xorshift32();
        set_wr_data(d, wd);

        // Random write address
        d->wr_addr_i = (r >> 13) & 1;

        sim.tick();

        // Random write enable (posedge)
        d->wr_en_i = (r >> 14) & 0xFF;

        // Compare read output
        d->eval();
        sim.compare("random_rd", (uint64_t)d->orig_rd_data_o, (uint64_t)d->new_rd_data_o);

        sim.tick();
        d->wr_en_i = 0;
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_1r1w_diff_preview_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdlib>

using DUT = Vcosim_rf_1r1w_diff_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void set_wide_data(DUT* d, const uint32_t words[8]) {
    for (int i = 0; i < 8; ++i) {
        d->wide_wr_data_i[i] = words[i];
    }
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    uint32_t rng = 0x89abcdefu;

    auto compare_all = [&]() {
        sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
        sim.compare("wide_rd_data", sim.dut->wide_orig_rd_data_o, sim.dut->wide_new_rd_data_o);
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint8_t rd_addr, uint16_t wr_data, uint8_t wr_addr,
                    uint8_t wr_en) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->rd_addr_i = rd_addr;
        sim.dut->wr_data_i = wr_data;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_en_i = wr_en;

        sim.dut->eval();
        compare_all();
        sim.sim_time++;
        if (sim.sim_time >= sim.max_time) {
            printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
            std::exit(2);
        }
    };

    sim.dut->preview_clk_i = 1;
    sim.dut->rf_clk_i = 0;
    sim.dut->wr_data_en_1p_next_i = 0;
    sim.dut->rd_addr_i = 0;
    sim.dut->wr_data_i = 0;
    sim.dut->wr_addr_i = 0;
    sim.dut->wr_en_i = 0;
    sim.dut->wide_wr_data_en_1p_next_i = 0;
    sim.dut->wide_rd_addr_i = 0;
    sim.dut->wide_wr_addr_i = 0;
    sim.dut->wide_wr_en_i = 0;
    uint32_t wide_zero[8] = {};
    set_wide_data(sim.dut.get(), wide_zero);

    step(1, 1, 0, 0, 0x0000, 0, 0);
    step(0, 0, 0x3, 0, 0x3412, 0, 0);
    step(1, 1, 0x0, 0, 0x7856, 1, 0x3);
    step(0, 0, 0x2, 0, 0x9abc, 0, 0);
    step(1, 1, 0x0, 0, 0xdef0, 1, 0);
    step(0, 0, 0x3, 4, 0x1122, 0, 0x3);
    step(1, 1, 0x0, 4, 0x3344, 0, 0x3);
    step(1, 1, 0x0, 0, 0x0000, 0, 0x0);
    step(0, 1, 0x1, 0, 0x00aa, 0, 0x0);
    step(0, 0, 0x1, 0, 0x00aa, 0, 0x0);
    step(0, 1, 0x1, 0, 0x00aa, 0, 0x1);
    step(1, 1, 0x0, 0, 0x0000, 0, 0x0);
    step(0, 1, 0x2, 4, 0xbb00, 1, 0x0);
    step(0, 0, 0x2, 4, 0xbb00, 1, 0x0);
    step(0, 1, 0x2, 4, 0xcc00, 1, 0x0);
    step(0, 0, 0x2, 4, 0xdd00, 1, 0x0);
    step(0, 1, 0x2, 4, 0xbb00, 1, 0x2);
    step(1, 1, 0x0, 4, 0x0000, 1, 0x0);

    uint32_t wide_a[8] = {
        0x01234567u, 0x89abcdefu, 0x13579bdfu, 0x2468ace0u,
        0xdeadbeefu, 0xcafef00du, 0x0ddc0ffeu, 0xe0ddf00du,
    };
    uint32_t wide_b[8] = {
        0x11112222u, 0x33334444u, 0x55556666u, 0x77778888u,
        0x9999aaaau, 0xbbbbccccu, 0xddddeeeeu, 0xffff0000u,
    };
    uint32_t wide_c[8] = {
        0x01020304u, 0x05060708u, 0x090a0b0cu, 0x0d0e0f10u,
        0x11121314u, 0x15161718u, 0x191a1b1cu, 0x1d1e1f20u,
    };

    sim.dut->wide_rd_addr_i = 0;
    sim.dut->wide_wr_addr_i = 0;
    sim.dut->wide_wr_en_i = 0;
    sim.dut->wide_wr_data_en_1p_next_i = 0xff;
    set_wide_data(sim.dut.get(), wide_a);
    step(0, 0, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_en_i = 0xff;
    sim.dut->wide_wr_addr_i = 0;
    set_wide_data(sim.dut.get(), wide_b);
    step(1, 1, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_en_i = 0;
    for (int addr = 0; addr < 16; ++addr) {
        sim.dut->wide_rd_addr_i = addr;
        sim.dut->eval();
        compare_all();
    }

    sim.dut->wide_wr_data_en_1p_next_i = 0x3c;
    sim.dut->wide_wr_addr_i = 1;
    set_wide_data(sim.dut.get(), wide_c);
    step(0, 0, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_data_en_1p_next_i = 0;
    sim.dut->wide_wr_en_i = 0x3c;
    set_wide_data(sim.dut.get(), wide_b);
    step(1, 1, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_en_i = 0;
    for (int addr = 0; addr < 32; ++addr) {
        sim.dut->wide_rd_addr_i = addr;
        sim.dut->eval();
        compare_all();
    }

    // Raw next must not arm write-data capture while the preview clock is held high.
    sim.dut->wide_wr_data_en_1p_next_i = 0xff;
    sim.dut->wide_wr_en_i = 0;
    set_wide_data(sim.dut.get(), wide_b);
    step(1, 0, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_en_i = 0xff;
    step(1, 1, 0, 0, 0x0000, 0, 0);
    sim.dut->wide_wr_en_i = 0;

    for (int i = 0; i < 4096; ++i) {
        uint32_t wide_words[8];
        for (int w = 0; w < 8; ++w) {
            wide_words[w] = next_rand(rng);
        }
        sim.dut->wide_wr_data_en_1p_next_i = next_rand(rng) & 0xff;
        sim.dut->wide_rd_addr_i = next_rand(rng) & 0x1f;
        sim.dut->wide_wr_addr_i = next_rand(rng) & 0x1;
        sim.dut->wide_wr_en_i = next_rand(rng) & 0xff;
        set_wide_data(sim.dut.get(), wide_words);
        step(next_rand(rng) & 0x1,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x3,
             next_rand(rng) & 0x3,
             next_rand(rng) & 0xffff,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x3);
    }

    return sim.finish();
}

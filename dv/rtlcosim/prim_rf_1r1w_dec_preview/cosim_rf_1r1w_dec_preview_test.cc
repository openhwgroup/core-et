// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_1r1w_dec_preview_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_rf_1r1w_dec_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 12000;

    uint32_t rng = 0xdec0de42u;

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
        sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
        sim.sim_time++;
        if (sim.sim_time >= sim.max_time) {
            std::printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
            std::exit(2);
        }
    };

    step(1, 1, 0, 0x00, 0x0000, 0x00, 0);
    step(0, 0, 1, 0x00, 0x5102, 0x00, 0);
    step(1, 1, 0, 0x04, 0x6a6a, 0x04, 1);
    step(0, 0, 0, 0x04, 0x7777, 0x00, 0);
    step(1, 1, 0, 0x18, 0x8888, 0x18, 1);
    step(0, 0, 1, 0x00, 0xa55a, 0x00, 0);
    step(1, 1, 0, 0x18, 0xffff, 0x42, 1);

    for (int i = 0; i < 4096; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0xff,
             next_rand(rng) & 0xffff,
             next_rand(rng) & 0xff,
             next_rand(rng) & 0x1);
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_3r2w_preview_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_rf_3r2w_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 16000;

    uint32_t rng = 0x33557799u;

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk,
                    uint8_t rd_addr_a, uint8_t rd_addr_b, uint8_t rd_addr_c,
                    uint8_t wr_en_a, uint8_t wr_addr_a, uint8_t preview_a, uint16_t wr_data_a,
                    uint8_t wr_en_b, uint8_t wr_addr_b, uint8_t preview_b, uint16_t wr_data_b) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->rd_addr_a_i = rd_addr_a;
        sim.dut->rd_addr_b_i = rd_addr_b;
        sim.dut->rd_addr_c_i = rd_addr_c;
        sim.dut->wr_en_a_i = wr_en_a;
        sim.dut->wr_addr_a_i = wr_addr_a;
        sim.dut->wr_data_a_en_1p_next_i = preview_a;
        sim.dut->wr_data_a_i = wr_data_a;
        sim.dut->wr_en_b_i = wr_en_b;
        sim.dut->wr_addr_b_i = wr_addr_b;
        sim.dut->wr_data_b_en_1p_next_i = preview_b;
        sim.dut->wr_data_b_i = wr_data_b;

        sim.dut->eval();
        sim.compare("rd_data_a", sim.dut->orig_rd_data_a_o, sim.dut->new_rd_data_a_o);
        sim.compare("rd_data_b", sim.dut->orig_rd_data_b_o, sim.dut->new_rd_data_b_o);
        sim.compare("rd_data_c", sim.dut->orig_rd_data_c_o, sim.dut->new_rd_data_c_o);
        sim.sim_time++;
        if (sim.sim_time >= sim.max_time) {
            std::printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
            std::exit(2);
        }
    };

    step(1, 1, 0, 0, 0, 0, 0, 0, 0x0000, 0, 0, 0, 0x0000);
    step(0, 0, 0, 0, 0, 0, 0, 1, 0x1111, 0, 0, 1, 0x2222);
    step(1, 1, 1, 2, 1, 1, 1, 0, 0xaaaa, 1, 2, 0, 0xbbbb);
    step(0, 0, 1, 2, 1, 0, 0, 0, 0x3333, 0, 0, 0, 0x4444);
    step(1, 1, 3, 4, 3, 1, 3, 0, 0x5555, 1, 4, 0, 0x6666);
    step(0, 0, 3, 4, 3, 0, 0, 1, 0x7777, 0, 0, 1, 0x8888);
    step(1, 1, 5, 5, 5, 1, 5, 0, 0x9999, 1, 5, 0, 0xaaaa);

    for (int i = 0; i < 4096; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0xffff,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0xffff);
    }

    return sim.finish();
}

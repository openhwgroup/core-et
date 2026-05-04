// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_2r1w_preview_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_rf_2r1w_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 14000;

    uint32_t rng = 0x22114488u;

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t rd_addr_a,
                    uint8_t rd_addr_b, uint8_t wr_en, uint8_t preview_next,
                    uint8_t wr_addr, uint16_t wr_data) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->rd_addr_a_i = rd_addr_a;
        sim.dut->rd_addr_b_i = rd_addr_b;
        sim.dut->wr_en_i = wr_en;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_data_i = wr_data;

        sim.dut->eval();
        sim.compare("rd_data_a", sim.dut->orig_rd_data_a_o, sim.dut->new_rd_data_a_o);
        sim.compare("rd_data_b", sim.dut->orig_rd_data_b_o, sim.dut->new_rd_data_b_o);
        sim.compare("rd_par", sim.dut->orig_rd_par_o, sim.dut->new_rd_par_o);
        sim.sim_time++;
        if (sim.sim_time >= sim.max_time) {
            std::printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
            std::exit(2);
        }
    };

    step(1, 1, 0, 0, 0, 0, 0, 0x0000);
    step(0, 0, 0, 0, 0, 1, 0, 0x1234);
    step(1, 1, 1, 1, 1, 0, 1, 0xabcd);
    step(0, 0, 1, 1, 0, 0, 0, 0x5555);
    step(1, 1, 1, 2, 1, 0, 2, 0xffff);
    step(0, 0, 0, 0, 0, 1, 0, 0xeeee);
    step(1, 1, 0, 0, 1, 0, 0, 0x7777);
    step(0, 0, 7, 1, 0, 1, 0, 0x9a9a);
    step(1, 1, 7, 1, 1, 0, 7, 0x1111);

    for (int i = 0; i < 4096; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0xffff);
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_2r1w_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_2r1w_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 24000;

    std::array<uint16_t, 8> rf_ref = {};
    uint16_t wr_data_del_ref = 0;
    bool wr_data_en_1p_ref = false;
    uint8_t prev_rf_clk = 1;
    uint32_t rng = 0x2211aaccu;

    auto apply_model = [&]() {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
        }
        if ((prev_rf_clk == 1) && (sim.dut->rf_clk_i == 0)) {
            if (wr_data_en_1p_ref) {
                wr_data_del_ref = static_cast<uint16_t>(sim.dut->wr_data_i);
            }
        }
        if ((prev_rf_clk == 0) && (sim.dut->rf_clk_i == 1) && sim.dut->wr_en_i) {
            const uint8_t wr_addr = sim.dut->wr_addr_i & 0x7;
            if (wr_addr != 0) {
                rf_ref[wr_addr] = wr_data_del_ref;
            }
        }
        rf_ref[0] = 0;
        prev_rf_clk = sim.dut->rf_clk_i;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t rd_addr_a,
                    uint8_t rd_addr_b, uint8_t wr_en, uint8_t preview_next,
                    uint8_t wr_addr, uint16_t wr_data, const std::string& msg) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->rd_addr_a_i = rd_addr_a;
        sim.dut->rd_addr_b_i = rd_addr_b;
        sim.dut->wr_en_i = wr_en;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_data_i = wr_data;
        apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_a_o == rf_ref[rd_addr_a & 0x7], msg + " read A");
        sim.check(sim.dut->rd_data_b_o == rf_ref[rd_addr_b & 0x7], msg + " read B");
        sim.check(sim.dut->rd_par_o == rf_ref[7], msg + " parallel");
    };

    step(1, 1, 0, 0, 0, 0, 0, 0x0000, "initial state");
    step(0, 0, 0, 0, 0, 1, 0, 0x1234, "preview arm");
    step(1, 1, 1, 1, 1, 0, 1, 0xabcd, "write entry 1");
    step(0, 0, 1, 1, 0, 0, 0, 0x5555, "drop preview");
    step(1, 1, 1, 2, 1, 0, 2, 0xffff, "stale preview reused");
    step(0, 0, 0, 0, 0, 1, 0, 0xeeee, "arm zero entry");
    step(1, 1, 0, 0, 1, 0, 0, 0x7777, "zero entry remains zero");
    step(0, 0, 7, 1, 0, 1, 0, 0x9a9a, "arm parallel entry");
    step(1, 1, 7, 1, 1, 0, 7, 0x1111, "parallel entry updates");

    for (int i = 0; i < 2048; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0x7,
             next_rand(rng) & 0xffff,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

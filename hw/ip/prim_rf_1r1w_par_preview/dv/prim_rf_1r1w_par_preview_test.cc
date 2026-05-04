// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_par_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_1r1w_par_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static uint32_t pack_rf(const std::array<uint8_t, 4>& rf_ref) {
    return static_cast<uint32_t>(rf_ref[0]) |
           (static_cast<uint32_t>(rf_ref[1]) << 8) |
           (static_cast<uint32_t>(rf_ref[2]) << 16) |
           (static_cast<uint32_t>(rf_ref[3]) << 24);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    std::array<uint8_t, 4> rf_ref = {};
    uint8_t wr_data_del_ref = 0;
    bool wr_data_en_1p_ref = false;
    uint8_t prev_rf_clk = 1;
    uint32_t rng = 0x1fa71234u;

    auto apply_model = [&]() -> uint32_t {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
        }
        if ((prev_rf_clk == 1) && (sim.dut->rf_clk_i == 0)) {
            if (wr_data_en_1p_ref) {
                wr_data_del_ref = static_cast<uint8_t>(sim.dut->wr_data_i);
            }
        }
        if ((prev_rf_clk == 0) && (sim.dut->rf_clk_i == 1) && sim.dut->wr_en_i) {
            rf_ref[sim.dut->wr_addr_i & 0x3] = wr_data_del_ref;
        }
        prev_rf_clk = sim.dut->rf_clk_i;
        return pack_rf(rf_ref);
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint8_t wr_data, uint8_t wr_addr, uint8_t wr_en,
                    const std::string& msg) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->wr_data_i = wr_data;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_en_i = wr_en;
        const uint32_t rd_ref = apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_o == rd_ref, msg);
    };

    step(1, 1, 0, 0x00, 0, 0, "initial state");
    step(0, 0, 1, 0x12, 0, 0, "preview arm entry 0");
    step(1, 1, 0, 0x56, 0, 1, "write uses captured preview data");
    step(0, 0, 0, 0xab, 0, 0, "drop preview next");
    step(1, 1, 0, 0xcd, 1, 1, "stale captured data reused");
    step(0, 0, 1, 0x9a, 0, 0, "re-arm preview");
    step(1, 1, 0, 0xbc, 3, 1, "new preview commits to another entry");

    for (int i = 0; i < 2048; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0xff,
             next_rand(rng) & 0x3,
             next_rand(rng) & 0x1,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_1r1w_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    std::array<uint8_t, 4> rf_ref = {};
    uint8_t wr_data_del_ref = 0;
    bool wr_data_en_1p_ref = false;
    uint8_t prev_clk = 1;
    uint32_t rng = 0x31415926u;

    auto apply_model = [&]() {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
        }
        if (prev_clk == 1 && sim.dut->rf_clk_i == 0) {
            if (wr_data_en_1p_ref) {
                wr_data_del_ref = static_cast<uint8_t>(sim.dut->wr_data_i);
            }
        }
        if (prev_clk == 0 && sim.dut->rf_clk_i == 1) {
            if (sim.dut->wr_en_i) {
                rf_ref[sim.dut->wr_addr_i & 0x3] = wr_data_del_ref;
            }
        }
        prev_clk = sim.dut->rf_clk_i;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint8_t wr_en, uint8_t wr_addr, uint8_t wr_data,
                    uint8_t rd_addr, const std::string& msg) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->wr_en_i = wr_en;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_data_i = wr_data;
        sim.dut->rd_addr_i = rd_addr;
        apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_o == rf_ref[rd_addr & 0x3], msg);
    };

    step(1, 1, 0, 0, 0, 0x00, 0, "initial state");
    step(0, 0, 1, 0, 0, 0x12, 0, "preview transparent while clock low");
    step(1, 1, 0, 1, 1, 0x34, 1, "write commits latched preview data");

    step(0, 0, 0, 1, 1, 0x56, 1, "dropping preview next while low updates latch");
    step(1, 1, 1, 1, 2, 0x9a, 1, "unrelated write leaves prior entry unchanged");
    step(0, 0, 1, 0, 3, 0xbc, 2, "preview can arm while write is still idle");
    step(1, 1, 0, 0, 1, 0xde, 2, "idle address churn does not commit data");
    step(0, 0, 1, 1, 3, 0xf0, 3, "preview re-arms on same clock domain");
    step(1, 1, 0, 1, 3, 0xf0, 3, "write commits only after matching write enable");

    for (int i = 0; i < 2048; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        const uint8_t preview_next = next_rand(rng) & 0x1;
        const uint8_t wr_en = next_rand(rng) & 0x1;
        const uint8_t wr_addr = next_rand(rng) & 0x3;
        const uint8_t wr_data = next_rand(rng) & 0xff;
        const uint8_t rd_addr = next_rand(rng) & 0x3;
        step(clk, clk, preview_next, wr_en, wr_addr, wr_data, rd_addr,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

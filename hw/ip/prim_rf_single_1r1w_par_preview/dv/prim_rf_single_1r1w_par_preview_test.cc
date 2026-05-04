// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_single_1r1w_par_preview_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vprim_rf_single_1r1w_par_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    uint16_t rf_ref = 0;
    uint16_t wr_data_del_ref = 0;
    bool wr_data_en_1p_ref = false;
    uint8_t prev_rf_clk = 1;
    uint32_t rng = 0x51a9beefu;

    auto apply_model = [&]() -> uint16_t {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
        }
        if ((prev_rf_clk == 1) && (sim.dut->rf_clk_i == 0)) {
            if (wr_data_en_1p_ref) {
                wr_data_del_ref = sim.dut->wr_data_i;
            }
        }
        if ((prev_rf_clk == 0) && (sim.dut->rf_clk_i == 1) && sim.dut->wr_en_i) {
            rf_ref = wr_data_del_ref;
        }
        prev_rf_clk = sim.dut->rf_clk_i;
        return rf_ref;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint16_t wr_data, uint8_t wr_en, const std::string& msg) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->wr_data_i = wr_data;
        sim.dut->wr_en_i = wr_en;
        const uint16_t rd_ref = apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_o == rd_ref, msg);
    };

    step(1, 1, 0, 0x0000, 0, "initial state");
    step(0, 0, 1, 0x2468, 0, "preview arms first write");
    step(1, 1, 0, 0x1357, 1, "commit uses previewed payload");
    step(0, 0, 0, 0xaaaa, 0, "drop preview arm");
    step(1, 1, 0, 0xaaaa, 1, "stale previewed payload is reused");
    step(0, 0, 1, 0xbeef, 0, "re-arm preview payload");
    step(1, 1, 0, 0x9999, 1, "new previewed payload commits");

    for (int i = 0; i < 2048; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x1,
             static_cast<uint16_t>(next_rand(rng) & 0xffff),
             next_rand(rng) & 0x1,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

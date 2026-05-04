// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_dec_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_1r1w_dec_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    std::array<uint16_t, 8> rf_ref = {};
    uint16_t wr_data_del_ref = 0;
    bool wr_data_en_1p_ref = false;
    uint8_t prev_rf_clk = 1;
    uint32_t rng = 0x1dec0deu;

    auto apply_model = [&]() -> uint16_t {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
        }
        if ((prev_rf_clk == 1) && (sim.dut->rf_clk_i == 0)) {
            if (wr_data_en_1p_ref) {
                wr_data_del_ref = static_cast<uint16_t>(sim.dut->wr_data_i);
            }
        }
        if ((prev_rf_clk == 0) && (sim.dut->rf_clk_i == 1) && sim.dut->wr_en_i) {
            for (int i = 0; i < 8; ++i) {
                if ((sim.dut->wr_addr_i >> i) & 0x1) {
                    rf_ref[i] = wr_data_del_ref;
                }
            }
        }
        prev_rf_clk = sim.dut->rf_clk_i;

        uint16_t rd_ref = 0;
        for (int i = 0; i < 8; ++i) {
            if ((sim.dut->rd_addr_i >> i) & 0x1) {
                rd_ref |= rf_ref[i];
            }
        }
        return rd_ref;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint8_t rd_addr, uint16_t wr_data, uint8_t wr_addr,
                    uint8_t wr_en, const std::string& msg) {
        sim.dut->preview_clk_i = preview_clk;
        sim.dut->rf_clk_i = rf_clk;
        sim.dut->wr_data_en_1p_next_i = preview_next;
        sim.dut->rd_addr_i = rd_addr;
        sim.dut->wr_data_i = wr_data;
        sim.dut->wr_addr_i = wr_addr;
        sim.dut->wr_en_i = wr_en;
        const uint16_t rd_ref = apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_o == rd_ref, msg);
    };

    step(1, 1, 0, 0x00, 0x0000, 0x00, 0, "initial state");
    step(0, 0, 1, 0x00, 0x5102, 0x00, 0, "preview arm entry 2");
    step(1, 1, 0, 0x04, 0x6a6a, 0x04, 1, "write uses captured preview data");
    step(0, 0, 0, 0x04, 0x7777, 0x00, 0, "drop preview next");
    step(1, 1, 0, 0x18, 0x8888, 0x18, 1, "multi-write reuses previous captured data");
    step(0, 0, 1, 0x00, 0xa55a, 0x00, 0, "re-arm preview");
    step(1, 1, 0, 0x18, 0xffff, 0x42, 1, "unrelated write leaves multi-read stable");

    for (int i = 0; i < 2048; ++i) {
        const uint8_t clk = next_rand(rng) & 0x1;
        step(clk,
             clk,
             next_rand(rng) & 0x1,
             next_rand(rng) & 0xff,
             next_rand(rng) & 0xffff,
             next_rand(rng) & 0xff,
             next_rand(rng) & 0x1,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

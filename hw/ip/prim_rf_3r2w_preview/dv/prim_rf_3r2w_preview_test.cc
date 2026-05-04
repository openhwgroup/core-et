// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_3r2w_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_3r2w_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 26000;

    std::array<uint16_t, 8> rf_ref = {};
    uint16_t wr_data_a_del_ref = 0;
    uint16_t wr_data_b_del_ref = 0;
    bool wr_data_a_en_1p_ref = false;
    bool wr_data_b_en_1p_ref = false;
    uint8_t prev_rf_clk = 1;
    uint32_t rng = 0x33227755u;

    auto apply_model = [&]() {
        if (!sim.dut->preview_clk_i) {
            wr_data_a_en_1p_ref = sim.dut->wr_data_a_en_1p_next_i;
            wr_data_b_en_1p_ref = sim.dut->wr_data_b_en_1p_next_i;
        }
        if ((prev_rf_clk == 1) && (sim.dut->rf_clk_i == 0)) {
            if (wr_data_a_en_1p_ref) {
                wr_data_a_del_ref = static_cast<uint16_t>(sim.dut->wr_data_a_i);
            }
            if (wr_data_b_en_1p_ref) {
                wr_data_b_del_ref = static_cast<uint16_t>(sim.dut->wr_data_b_i);
            }
        }
        if (prev_rf_clk == 0 && sim.dut->rf_clk_i == 1) {
            if (sim.dut->wr_en_b_i) {
                rf_ref[sim.dut->wr_addr_b_i & 0x7] = wr_data_b_del_ref;
            }
            if (sim.dut->wr_en_a_i) {
                rf_ref[sim.dut->wr_addr_a_i & 0x7] = wr_data_a_del_ref;
            }
        }
        prev_rf_clk = sim.dut->rf_clk_i;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk,
                    uint8_t rd_addr_a, uint8_t rd_addr_b, uint8_t rd_addr_c,
                    uint8_t wr_en_a, uint8_t wr_addr_a, uint8_t preview_a, uint16_t wr_data_a,
                    uint8_t wr_en_b, uint8_t wr_addr_b, uint8_t preview_b, uint16_t wr_data_b,
                    const std::string& msg) {
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
        apply_model();
        sim.dut->eval();
        sim.sim_time++;
        sim.check(sim.dut->rd_data_a_o == rf_ref[rd_addr_a & 0x7], msg + " read A");
        sim.check(sim.dut->rd_data_b_o == rf_ref[rd_addr_b & 0x7], msg + " read B");
        sim.check(sim.dut->rd_data_c_o == rf_ref[rd_addr_c & 0x7], msg + " read C");
    };

    step(1, 1, 0, 0, 0, 0, 0, 0, 0x0000, 0, 0, 0, 0x0000, "initial state");
    step(0, 0, 0, 0, 0, 0, 0, 1, 0x1111, 0, 0, 1, 0x2222, "preview arm");
    step(1, 1, 1, 2, 1, 1, 1, 0, 0xaaaa, 1, 2, 0, 0xbbbb, "write distinct entries");
    step(0, 0, 1, 2, 1, 0, 0, 0, 0x3333, 0, 0, 0, 0x4444, "drop preview");
    step(1, 1, 3, 4, 3, 1, 3, 0, 0x5555, 1, 4, 0, 0x6666, "stale preview reused");
    step(0, 0, 3, 4, 3, 0, 0, 1, 0x7777, 0, 0, 1, 0x8888, "re-arm preview");
    step(1, 1, 5, 5, 5, 1, 5, 0, 0x9999, 1, 5, 0, 0xaaaa, "same-address conflict");

    for (int i = 0; i < 2048; ++i) {
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
             next_rand(rng) & 0xffff,
             "random step " + std::to_string(i));
    }

    return sim.finish();
}

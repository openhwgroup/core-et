// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_diff_preview_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vprim_rf_1r1w_diff_preview_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    std::array<uint16_t, 2> rf_ref = {};
    uint16_t wr_data_del_ref = 0;
    uint8_t wr_data_en_1p_ref = 0;
    uint8_t prev_clk = 1;
    uint32_t rng = 0x27182818u;

    auto read_model = [&](uint8_t rd_addr) {
        const uint32_t rf_full = static_cast<uint32_t>(rf_ref[0]) |
                                 (static_cast<uint32_t>(rf_ref[1]) << 16);
        const uint64_t rf_full_ext =
            (static_cast<uint64_t>(rf_full & 0xffu) << 32) | static_cast<uint64_t>(rf_full);
        return static_cast<uint8_t>((rf_full_ext >> ((rd_addr & 0x7) * 8)) & 0xffu);
    };

    auto apply_model = [&]() {
        if (!sim.dut->preview_clk_i) {
            wr_data_en_1p_ref = static_cast<uint8_t>(sim.dut->wr_data_en_1p_next_i);
        }
        if (prev_clk == 1 && sim.dut->rf_clk_i == 0) {
            for (int j = 0; j < 2; ++j) {
                if ((wr_data_en_1p_ref >> j) & 0x1) {
                    const uint16_t mask = static_cast<uint16_t>(0xffu << (j * 8));
                    wr_data_del_ref = static_cast<uint16_t>((wr_data_del_ref & ~mask) |
                                                            (sim.dut->wr_data_i & mask));
                }
            }
        }
        if (prev_clk == 0 && sim.dut->rf_clk_i == 1) {
            for (int j = 0; j < 2; ++j) {
                if ((sim.dut->wr_en_i >> j) & 0x1) {
                    const uint16_t mask = static_cast<uint16_t>(0xffu << (j * 8));
                    rf_ref[sim.dut->wr_addr_i & 0x1] =
                        static_cast<uint16_t>((rf_ref[sim.dut->wr_addr_i & 0x1] & ~mask) |
                                              (wr_data_del_ref & mask));
                }
            }
        }
        prev_clk = sim.dut->rf_clk_i;
    };

    auto step = [&](uint8_t preview_clk, uint8_t rf_clk, uint8_t preview_next,
                    uint8_t wr_en, uint8_t wr_addr, uint16_t wr_data,
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
        sim.check(sim.dut->rd_data_o == read_model(rd_addr), msg);
    };

    step(1, 1, 0, 0, 0, 0x0000, 0, "initial state");
    step(0, 0, 0x3, 0, 0, 0x3412, 0, "preview captures both subwords");
    step(1, 1, 0x0, 0x3, 1, 0x7856, 4, "write commits both bytes");

    step(0, 0, 0x1, 0x3, 1, 0xabcd, 4, "preview updates low byte only");
    step(1, 1, 0x0, 0x3, 1, 0xabcd, 4, "write updates only enabled low byte");
    step(0, 0, 0x2, 0x0, 0, 0x55ee, 4, "preview can arm upper byte while write idle");
    step(1, 1, 0x0, 0x0, 1, 0x9911, 4, "idle address churn does not commit mixed-width data");
    step(0, 0, 0x3, 0x3, 0, 0x1234, 0, "preview can capture both subwords again");
    step(1, 1, 0x0, 0x3, 0, 0x1234, 0, "matching write enable commits captured subwords");

    step(1, 1, 0x0, 0x0, 0, 0x0000, 0, "separate-clock baseline");
    step(0, 1, 0x1, 0x0, 0, 0x00aa, 0, "preview can arm low byte while RF clock stays high");
    step(0, 0, 0x1, 0x0, 0, 0x00aa, 0, "RF falling edge captures previewed low byte");
    step(0, 1, 0x1, 0x1, 0, 0x00aa, 0, "RF rising edge commits low byte while preview clock stays low");
    step(1, 1, 0x0, 0x0, 0, 0x0000, 0, "releasing preview clock holds committed data");

    step(0, 1, 0x2, 0x0, 1, 0xbb00, 4, "preview can arm upper byte on a different write address");
    step(0, 0, 0x2, 0x0, 1, 0xbb00, 4, "RF falling edge captures previewed upper byte");
    step(0, 1, 0x2, 0x0, 1, 0xcc00, 4, "write enable low keeps upper byte pending but uncommitted");
    step(0, 0, 0x2, 0x0, 1, 0xdd00, 4, "additional RF falling edges do not commit without wr_en");
    step(0, 1, 0x2, 0x2, 1, 0xbb00, 4, "later RF rising edge commits only the armed upper byte");
    step(1, 1, 0x0, 0x0, 1, 0x0000, 4, "committed upper byte is stable after preview clock release");

    for (int i = 0; i < 2048; ++i) {
      const uint8_t preview_clk = next_rand(rng) & 0x1;
      const uint8_t rf_clk = next_rand(rng) & 0x1;
      const uint8_t preview_next = next_rand(rng) & 0x3;
      const uint8_t wr_en = next_rand(rng) & 0x3;
      const uint8_t wr_addr = next_rand(rng) & 0x1;
      const uint16_t wr_data = static_cast<uint16_t>(next_rand(rng) & 0xffffu);
      const uint8_t rd_addr = next_rand(rng) & 0x3;
      step(preview_clk, rf_clk, preview_next, wr_en, wr_addr, wr_data, rd_addr,
           "random step " + std::to_string(i));
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_clk_prims -- Verilator UART summary test.

#include "Vtest_clk_prims.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

using DUT = Vtest_clk_prims;

namespace {

constexpr uint32_t kClksPerBit = 1'152'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko test_clk_prims";
constexpr std::string_view kStatusLine = "TEST CLK PRIMS PASSED";
constexpr std::size_t kSummaryLines = 9;

bool wait_for_start_bit(SimCtrl<DUT>& sim, uint32_t max_cycles, const std::string& what) {
    for (uint32_t i = 0; i < max_cycles; ++i) {
        if (sim.dut->uart_tx_o == 0) return true;
        sim.tick();
    }
    sim.check(false, what + ": UART start bit observed");
    return false;
}

uint8_t recv_byte(SimCtrl<DUT>& sim, const std::string& what) {
    if (!wait_for_start_bit(sim, 400 * kClksPerBit, what)) return 0;

    const uint32_t half_bit = kClksPerBit / 2;
    for (uint32_t i = 0; i < half_bit; ++i) sim.tick();
    sim.check(sim.dut->uart_tx_o == 0, what + ": start bit held low");

    uint8_t data = 0;
    for (int bit = 0; bit < 8; ++bit) {
        for (uint32_t i = 0; i < kClksPerBit; ++i) sim.tick();
        data |= static_cast<uint8_t>(sim.dut->uart_tx_o << bit);
    }

    for (uint32_t i = 0; i < kClksPerBit; ++i) sim.tick();
    sim.check(sim.dut->uart_tx_o == 1, what + ": stop bit high");
    return data;
}

std::string recv_line(SimCtrl<DUT>& sim, std::size_t line_idx) {
    std::string line;
    while (true) {
        const char ch = static_cast<char>(recv_byte(sim, "line[" + std::to_string(line_idx) + "]"));
        if (ch == '\r') {
            sim.check(static_cast<char>(recv_byte(sim, "line[" + std::to_string(line_idx) + "].lf")) == '\n',
                      "UART line terminates with LF");
            return line;
        }
        line.push_back(ch);
    }
}

bool has_prefix(std::string_view text, std::string_view prefix) {
    return text.substr(0, prefix.size()) == prefix;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 180'000;

    sim.dut->rst_ni = 1;
    sim.tick();
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();

    std::vector<std::string> lines;
    for (std::size_t i = 0; i < kSummaryLines + 3; ++i) {
        lines.push_back(recv_line(sim, i));
        if (lines.back() == kStatusLine || lines.back() == "TEST CLK PRIMS FAILED") break;
    }

    sim.check(lines.size() == kSummaryLines + 2, "UART summary line count matches expected output");
    if (!lines.empty()) sim.check(lines[0] == kBootLine, "Boot banner matches");

    constexpr std::string_view kPrefixes[] = {
        "G0 C=", "G1 C=", "G2 C=", "GS C=", "N0 C=", "N1 C=", "N2 C=", "M0 D=", "M1 D=",
    };
    for (std::size_t i = 0; i < std::size(kPrefixes); ++i) {
        if (lines.size() > i + 1) {
            sim.check(has_prefix(lines[i + 1], kPrefixes[i]),
                      "Summary line " + std::to_string(i + 1) + " prefix matches");
        }
    }

    if (lines.size() >= kSummaryLines + 2) {
        sim.check(lines[kSummaryLines + 1] == kStatusLine, "Status line matches");
    }

    if (sim.errors) {
        std::string got;
        for (const auto& line : lines) {
            got += line;
            got += "\r\n";
        }
        std::printf("Observed UART log (%zu bytes):\n%s\n", got.size(), got.c_str());
    }

    return sim.finish();
}

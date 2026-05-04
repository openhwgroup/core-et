// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// tinyon_top_exec -- Verilator UART summary test.

#include "Vtinyon_top_exec.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string_view>
#include <string>
#include <vector>

using DUT = Vtinyon_top_exec;

namespace {

constexpr uint32_t kClksPerBit = 25'000'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko tinyon_top_exec";
constexpr std::string_view kReq0Line = "Q0 V=1 A=0000000000001000";
constexpr std::string_view kRet0Line = "R0 P=0000000000001000 I=00000013 X=0";
constexpr std::string_view kRet1Line = "R1 P=0000000000001004 I=00000013 X=0";
constexpr std::string_view kStatusLine = "TINYON TOP EXEC TEST PASSED";

bool starts_with(std::string_view text, std::string_view prefix) {
    return text.rfind(prefix, 0) == 0;
}

std::vector<std::string> split_lines(const std::string& text) {
    std::vector<std::string> lines;
    std::size_t start = 0;

    while (start < text.size()) {
        const std::size_t end = text.find("\r\n", start);
        if (end == std::string::npos) break;
        lines.push_back(text.substr(start, end - start));
        start = end + 2;
    }

    return lines;
}

bool wait_for_start_bit(SimCtrl<DUT>& sim, uint32_t max_cycles, const std::string& what) {
    for (uint32_t i = 0; i < max_cycles; ++i) {
        if (sim.dut->uart_tx_o == 0) return true;
        sim.tick();
    }
    sim.check(false, what + ": UART start bit observed");
    return false;
}

uint8_t recv_byte(SimCtrl<DUT>& sim, const std::string& what) {
    if (!wait_for_start_bit(sim, 200 * kClksPerBit, what)) return 0;

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

std::string recv_log(SimCtrl<DUT>& sim, std::size_t len) {
    std::string out;
    for (size_t i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(recv_byte(sim, "uart[" + std::to_string(i) + "]")));
    }
    return out;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4'000'000;

    sim.dut->rst_ni = 1;
    sim.tick();
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();

    const std::string got = recv_log(sim, 184);
    const auto lines = split_lines(got);

    sim.check(lines.size() == 6, "UART summary line count matches expected output");
    if (lines.size() >= 1) sim.check(lines[0] == kBootLine, "Boot banner matches");
    if (lines.size() >= 2) sim.check(lines[1] == kReq0Line, "First request debug line matches");
    if (lines.size() >= 3) {
        sim.check(starts_with(lines[2], "Q1 V="), "Second request debug line starts with Q1");
        sim.check(lines[2].find(" A=") != std::string::npos, "Second request debug line carries an address");
    }
    if (lines.size() >= 4) sim.check(lines[3] == kRet0Line, "First retire line matches");
    if (lines.size() >= 5) sim.check(lines[4] == kRet1Line, "Second retire line matches");
    if (lines.size() >= 6) sim.check(lines[5] == kStatusLine, "Status line matches");

    if (sim.errors) {
        std::printf("Observed UART log (%zu bytes):\n%s\n", got.size(), got.c_str());
    }

    return sim.finish();
}

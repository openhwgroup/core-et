// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_intpipe -- Verilator UART summary test.

#include "Vtest_intpipe.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using DUT = Vtest_intpipe;

namespace {

constexpr uint32_t kClksPerBit = 25'000'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko test_intpipe";
constexpr std::string_view kStatusLine = "TEST INTPIPE PASSED";

constexpr std::string_view kExpectedLines[] = {
    "I0 P=0000000000001000 I=00100093",
    "I1 P=0000000000001004 I=00200113",
    "I2 P=0000000000001008 I=00208463",
    "I3 P=000000000000100C I=00300193",
    "I4 P=0000000000001010 I=00108463",
    "I5 P=0000000000001018 I=0080006F",
    "I6 P=0000000000001020 I=02208233",
    "I7 P=0000000000001024 I=021242B3",
    "I8 P=0000000000001028 I=34019073",
    "I9 P=000000000000102C I=00000393",
    "IA P=0000000000001030 I=00000413",
    "IB P=0000000000001034 I=00000493",
    "IC P=0000000000001038 I=34002373",
    "ID P=000000000000103C I=00000693",
    "IE P=0000000000001040 I=00330463",
    "IF P=0000000000001048 I=00A00613",
    "J0 V=1 P=0000000000001018",
    "J1 V=1 P=0000000000001020",
    "J2 V=1 P=0000000000001048",
    "R0 P=0000000000001000 I=00100093 X=0",
    "R1 P=0000000000001004 I=00200113 X=0",
    "R2 P=0000000000001008 I=00208463 X=0",
    "R3 P=000000000000100C I=00300193 X=0",
    "R4 P=0000000000001010 I=00108463 X=0",
    "R5 P=0000000000001018 I=0080006F X=0",
    "R6 P=0000000000001020 I=02208233 X=0",
    "R7 P=0000000000001024 I=021242B3 X=0",
    "R8 P=0000000000001028 I=34019073 X=0",
    "R9 P=000000000000102C I=00000393 X=0",
    "RA P=0000000000001030 I=00000413 X=0",
    "RB P=0000000000001034 I=00000493 X=0",
    "RC P=0000000000001038 I=34002373 X=0",
    "RD P=000000000000103C I=00000693 X=0",
    "RE P=0000000000001040 I=00330463 X=0",
    "RF P=0000000000001048 I=00A00613 X=0",
};

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

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 16'000'000;

    sim.dut->rst_ni = 1;
    sim.tick();
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();

    std::vector<std::string> lines;
    for (std::size_t i = 0; i < 64; ++i) {
        lines.push_back(recv_line(sim, i));
        if (lines.back() == kStatusLine || lines.back() == "TEST INTPIPE FAILED") break;
    }

    sim.check(lines.size() == std::size(kExpectedLines) + 2, "UART summary line count matches expected output");
    if (!lines.empty()) sim.check(lines[0] == kBootLine, "Boot banner matches");

    for (std::size_t i = 0; i < std::size(kExpectedLines); ++i) {
        if (lines.size() > i + 1) {
            sim.check(lines[i + 1] == kExpectedLines[i],
                      "Summary line " + std::to_string(i + 1) + " matches");
        }
    }

    if (lines.size() >= std::size(kExpectedLines) + 2) {
        sim.check(lines[std::size(kExpectedLines) + 1] == kStatusLine, "Status line matches");
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

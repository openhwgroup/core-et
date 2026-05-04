// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_rf_preview -- Verilator UART summary test.

#include "Vtest_rf_preview.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

using DUT = Vtest_rf_preview;

namespace {

constexpr uint32_t kClksPerBit = 1'152'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko test_rf_preview";
constexpr std::string_view kStatusLine = "TEST RF PREVIEW PASSED";

constexpr std::string_view kExpectedLines[] = {
    "P0 W=12 R=12",
    "P1 W=56 R=12",
    "P2 W=9A R=9A",
    "P3 W=E7 R=12",
    "P4 W=FF R=FF",
    "D0 W=3412 R=12",
    "D1 W=ABCD R=CD",
    "D2 W=ABCD R=34",
    "D3 W=BB00 R=BB",
    "F0 W=00000013 R=00000013",
    "F1 W=33440000 R=33440000",
    "F2 W=C0017788 R=C0017788",
    "F3 W=A5A5F00D R=A5A5F00D",
    "F4 W=00000000 R=00000000",
    "F5 W=00000000 R=00000000",
    "R0 W=2003 R=2003",
    "R1 W=AAAA R=2003",
    "R2 W=55AA R=55AA",
    "C0 W=5102 R=5102",
    "C1 W=5102 R=5102",
    "C2 W=A55A R=A55A",
    "A0 W=00000012 R=00000012",
    "A1 W=00001212 R=00001212",
    "A2 W=9A001212 R=9A001212",
    "T0 W=1234 R=1234",
    "T1 W=1234 R=1234",
    "T2 W=0000 R=0000",
    "T3 W=9A9A R=9A9A",
    "V0 W=1111 R=1111",
    "V1 W=1111 R=1111",
    "V2 W=7777 R=7777",
    "Q0 W=00000021 R=00000021",
    "Q1 W=00AB0021 R=00AB0021",
    "Q2 W=00ABAB21 R=00ABAB21",
    "S0 W=2468 R=2468",
    "S1 W=AAAA R=2468",
    "S2 W=BEEF R=BEEF",
    "W0 W=1357 R=1357",
    "W1 W=AAAA R=1357",
    "W2 W=C0DE R=C0DE",
    "Y0 W=03FF R=03FF",
    "Y1 W=03FF R=03FF",
    "Y2 W=03FF R=03FF",
    "M0 W=40D02001 R=40D02001",
    "M1 W=80A00002 R=80A00002",
    "L0 W=0123456789ABCDEF R=0123456789ABCDEF",
    "L1 W=FEDCBA9876543210 R=FEDCBA9876543210",
    "L2 W=1111222233334444 R=1111222233334444",
    "B0 W=2222333344445555 R=2222333344445555",
    "B1 W=13579BDF2468ACE0 R=13579BDF2468ACE0",
    "B2 W=AAAABBBBCCCCDDDD R=AAAABBBBCCCCDDDD",
    "B3 W=AAAABBBB01020304 R=AAAABBBB01020304",
    "B4 W=5566778801020304 R=5566778801020304",
    "X0 W=0123456789ABCDEF R=0123456789ABCDEF",
    "X1 W=0123456789ABCDEF R=0123456789ABCDEF",
    "Z0 W=3F R=3F",
    "Z1 W=0F R=0F",
    "Z2 W=03 R=03",
    "Z3 W=03 R=03",
    "Z4 W=03 R=03",
    "Z5 W=FF R=FF",
    "Z6 W=03 R=03",
    "Z7 W=FF R=FF",
    "Z8 W=0F R=0F",
    "Z9 W=3F R=3F",
    "ZA W=03 R=03",
    "ZB W=FF R=FF",
    "ZC W=FF R=FF",
    "ZD W=FF R=FF",
    "ZE W=FF R=FF",
};

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
    sim.max_time = 740'000;

    sim.dut->rst_ni = 1;
    sim.tick();
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();

    std::vector<std::string> lines;
    for (std::size_t i = 0; i < std::size(kExpectedLines) + 3; ++i) {
        lines.push_back(recv_line(sim, i));
        if (lines.back() == kStatusLine || lines.back() == "TEST RF PREVIEW FAILED") break;
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

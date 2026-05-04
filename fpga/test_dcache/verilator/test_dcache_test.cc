// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_dcache -- Verilator UART summary test.

#include "Vtest_dcache.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using DUT = Vtest_dcache;

namespace {

constexpr uint32_t kClksPerBit = 10'000'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko test_dcache";
constexpr std::string_view kStatusLine = "TEST DCACHE PASSED";

constexpr std::string_view kExpectedLines[] = {
    "ID R=1 P=0 I=000 B=00 M=1",
    "BP V=1 C=0 T=0 A=0000000000001234 E=001 B=00",
    "PT V=1 M=8 P=0000000000012345 R=1 A=00000000000ABCDE",
    "M0 W=0000000300020024 R=0000000300020024",
    "D0 W=0123456789ABCDEF R=0123456789ABCDEF",
    "L0 W=0123456789ABCDEF R=0123456789ABCDEF",
    "LD V=1 I=1 M=0 R=0 T=0 B=00 A=0000000000000000",
    "HX A=2 B=2 V=11 H=1 Q=0 N=0 C=11 F=00 T=3 K=00 P=11 U=0",
    "HZ R=2 K=0 S=1 Y=1 V=111 N=0",
    "HY W=2222 R=2222",
    "HG V=2 C=2 M=2 D=2",
    "D1 W=0123456789ABCDEF R=0123456789ABCDEF",
    "S0 W=FEDCBA9876543210 R=FEDCBA9876543210",
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

template <std::size_t N>
void check_expected_lines(SimCtrl<DUT>& sim,
                          const std::vector<std::string>& lines,
                          const std::string_view (&expected)[N],
                          std::string_view mode) {
    const std::string prefix(mode);
    sim.check(lines.size() == N + 2, prefix + ": UART summary line count matches expected output");
    if (!lines.empty()) sim.check(lines[0] == kBootLine, prefix + ": boot banner matches");

    for (std::size_t i = 0; i < N; ++i) {
        if (lines.size() > i + 1) {
            sim.check(lines[i + 1] == expected[i],
                      prefix + ": summary line " + std::to_string(i + 1) + " matches");
        }
    }

    if (lines.size() >= N + 2) {
        sim.check(lines[N + 1] == kStatusLine, prefix + ": status line matches");
    }
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 3'000'000;

    sim.dut->rst_ni = 1;
    sim.tick();
    sim.dut->rst_ni = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.tick();

    std::vector<std::string> lines;
    for (std::size_t i = 0; i < 32; ++i) {
        lines.push_back(recv_line(sim, i));
        if (lines.back() == kStatusLine || lines.back() == "TEST DCACHE FAILED") break;
    }

    check_expected_lines(sim, lines, kExpectedLines, "dcache");

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

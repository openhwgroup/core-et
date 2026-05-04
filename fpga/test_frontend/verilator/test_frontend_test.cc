// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_frontend — Verilator test.

#include "Vtest_frontend.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string_view>
#include <string>
#include <vector>

using DUT = Vtest_frontend;

namespace {

constexpr int kClkHz      = 25'000'000;
constexpr int kBaudRate   = 115'200;
constexpr int kClksPerBit = (kClkHz + (kBaudRate / 2)) / kBaudRate;

constexpr std::string_view kBootLine = "Ainekko test_frontend";
constexpr std::string_view kStatusLine = "TEST FRONTEND PASSED";
constexpr std::string_view kTracePrefixes[] = {
    "T0 P=0000000000001000 I=00000013",
    "T1 P=0000000000001020 I=00100093",
    "T2 P=0000000000001040 I=00300193",
    "T3 P=0000000000001060 I=0000A103",
    "T4 P=0000000000001080 I=0020A223",
    "T5 P=00000000000010A0 I=00208463",
    "T6 P=00000000000010C0 I=0080006F",
    "T7 P=00000000000010E0 I=00008067",
};

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

void tick_cycles(SimCtrl<DUT>& sim, int cycles) {
    for (int i = 0; i < cycles; ++i) sim.tick();
}

bool wait_for_start_bit(SimCtrl<DUT>& sim, int max_cycles, const std::string& what) {
    for (int i = 0; i < max_cycles; ++i) {
        if (sim.dut->uart_tx_o == 0) return true;
        sim.tick();
    }
    sim.check(false, what + ": timeout waiting for start bit");
    return false;
}

uint8_t recv_byte(SimCtrl<DUT>& sim, const std::string& what) {
    if (!wait_for_start_bit(sim, 80 * kClksPerBit, what)) return 0;

    tick_cycles(sim, kClksPerBit / 2);
    sim.check(sim.dut->uart_tx_o == 0, what + ": start bit held low");

    uint8_t data = 0;
    for (int bit = 0; bit < 8; ++bit) {
        tick_cycles(sim, kClksPerBit);
        data |= static_cast<uint8_t>(sim.dut->uart_tx_o << bit);
    }

    tick_cycles(sim, kClksPerBit);
    sim.check(sim.dut->uart_tx_o == 1, what + ": stop bit high");
    return data;
}

std::string recv_string(SimCtrl<DUT>& sim, std::size_t len) {
    std::string out;
    out.reserve(len);

    for (std::size_t i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(recv_byte(sim, "uart[" + std::to_string(i) + "]")));
    }
    return out;
}

bool has_status_line(const std::string& text) {
    return text.find("TEST FRONTEND PASSED\r\n") != std::string::npos ||
           text.find("TEST FRONTEND FAILED\r\n") != std::string::npos;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 12'000'000;

    sim.reset();

    std::string got;
    got.reserve(2048);

    for (int i = 0; i < 4096; ++i) {
        if (!wait_for_start_bit(sim, 160 * kClksPerBit, "uart[" + std::to_string(i) + "]")) break;
        got.push_back(static_cast<char>(recv_byte(sim, "uart[" + std::to_string(i) + "]")));
        if (has_status_line(got)) break;
    }

    const auto lines = split_lines(got);
    sim.check(has_status_line(got), "UART trace reaches final status line");
    sim.check(lines.size() == 10, "UART trace line count matches expected output");

    if (lines.size() >= 1) {
        sim.check(lines[0] == kBootLine, "Boot banner matches");
    }

    for (std::size_t i = 0; i < std::size(kTracePrefixes) && (i + 1) < lines.size(); ++i) {
        sim.check(starts_with(lines[i + 1], kTracePrefixes[i]),
                  "Trace line " + std::to_string(i) + " prefix matches");
        sim.check(lines[i + 1].find(" FP=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries thread-PC debug");
        sim.check(lines[i + 1].find(" PU=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries PC-update debug");
        sim.check(lines[i + 1].find(" QS=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries request-snapshot debug");
        sim.check(lines[i + 1].find(" QA=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries request-address debug");
        sim.check(lines[i + 1].find(" RV=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries next-redirect debug");
        sim.check(lines[i + 1].find(" PV=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries pending-redirect debug");
        sim.check(lines[i + 1].find(" RS=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries redirect-pulse debug");
        sim.check(lines[i + 1].find(" SP=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries redirect-pulse-PC debug");
        sim.check(lines[i + 1].find(" GT=") != std::string::npos,
                  "Trace line " + std::to_string(i) + " carries gated-clock debug");
    }

    if (lines.size() >= 10) {
        sim.check(lines[9] == kStatusLine, "Status line matches");
    }

    if (sim.errors) {
        std::printf("Observed UART log (%zu bytes):\n%s\n", got.size(), got.c_str());
    }

    return sim.finish();
}

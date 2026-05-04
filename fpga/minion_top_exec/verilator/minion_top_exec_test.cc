// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_top_exec -- Verilator UART summary test.

#include "Vminion_top_exec.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vminion_top_exec;

namespace {

constexpr uint32_t kClksPerBit = 25'000'000u / 115200u;
constexpr char kExpected[] =
    "Ainekko minion_top_exec\r\n"
    "C Q=3 T=4 N=1\r\n"
    "Q0 V=1 T=0 A=0000000000001000\r\n"
    "Q1 V=1 T=0 A=0000000000001020\r\n"
    "Q2 V=1 T=0 A=0000000000001040\r\n"
    "Q3 V=0 T=0 A=0000000000000000\r\n"
    "T0 V=1 P=0000000000001000 I=00000013 X=0\r\n"
    "T1 V=1 P=0000000000001004 I=00000013 X=0\r\n"
    "T2 V=1 P=0000000000001008 I=00000013 X=0\r\n"
    "T3 V=1 P=000000000000100C I=00000013 X=0\r\n"
    "MINION TOP EXEC TEST PASSED\r\n";

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

std::string recv_log(SimCtrl<DUT>& sim) {
    std::string out;
    for (size_t i = 0; i < sizeof(kExpected) - 1; ++i) {
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

    const std::string got = recv_log(sim);
    if (got != kExpected) {
        sim.check(false, "UART summary matches expected output");
        std::printf("Observed UART log (%zu bytes):\n%s\n", got.size(), got.c_str());
    } else {
        sim.check(true, "UART summary matches expected output");
    }

    return sim.finish();
}

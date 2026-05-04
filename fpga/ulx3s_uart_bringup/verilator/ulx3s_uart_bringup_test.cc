// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ulx3s_uart_bringup — Verilator test.

#include "Vulx3s_uart_bringup.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vulx3s_uart_bringup;

namespace {

constexpr int kClkHz      = 25'000'000;
constexpr int kBaudRate   = 115'200;
constexpr int kClksPerBit = (kClkHz + (kBaudRate / 2)) / kBaudRate;
constexpr char kBootMsg[] = "Ainekko ULX3S UART TEST PASSED\r\n";

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
    if (!wait_for_start_bit(sim, 20 * kClksPerBit, what)) return 0;

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

std::string recv_string(SimCtrl<DUT>& sim, const std::string& tag, std::size_t len) {
    std::string out;
    out.reserve(len);

    for (std::size_t i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(recv_byte(sim, tag + "[" + std::to_string(i) + "]")));
    }
    return out;
}

void send_byte(SimCtrl<DUT>& sim, uint8_t data) {
    sim.dut->uart_rx_i = 0;
    tick_cycles(sim, kClksPerBit);

    for (int bit = 0; bit < 8; ++bit) {
        sim.dut->uart_rx_i = (data >> bit) & 0x1;
        tick_cycles(sim, kClksPerBit);
    }

    sim.dut->uart_rx_i = 1;
    tick_cycles(sim, kClksPerBit);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 600000;

    sim.dut->uart_rx_i = 1;
    sim.reset();

    std::string boot = recv_string(sim, "boot", sizeof(kBootMsg) - 1);
    sim.check(boot == kBootMsg, "boot banner matches");

    const std::string probe = "uart?";
    std::string echo;
    echo.reserve(probe.size());
    for (std::size_t i = 0; i < probe.size(); ++i) {
        send_byte(sim, static_cast<uint8_t>(probe[i]));
        char echoed = static_cast<char>(recv_byte(sim, "echo[" + std::to_string(i) + "]"));
        echo.push_back(echoed);
        sim.check(echoed == probe[i], "echo byte[" + std::to_string(i) + "] matches");
    }
    sim.check(echo == probe, "echoed payload matches");

    return sim.finish();
}

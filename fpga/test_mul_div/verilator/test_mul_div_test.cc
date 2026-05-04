// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// test_mul_div -- Verilator UART summary test.

#include "Vtest_mul_div.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

using DUT = Vtest_mul_div;

namespace {

constexpr uint32_t kClksPerBit = 25'000'000u / 115200u;
constexpr std::string_view kBootLine = "Ainekko test_mul_div";
constexpr std::string_view kStatusLine = "TEST MUL DIV PASSED";

struct ExpectedLine {
  char prefix;
  uint8_t idx;
  uint8_t fn;
  uint8_t dw;
  uint8_t pass;
  uint64_t got;
  uint64_t exp;
  uint8_t dest;
  char aux_tag;
  uint8_t aux;
};

constexpr std::array<ExpectedLine, 22> kExpected = {{
    {'T', 0x00, 0x10, 1, 1, 0x0000000000000006ULL, 0x0000000000000006ULL, 0x01, 0, 0},
    {'T', 0x01, 0x11, 1, 1, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x02, 0, 0},
    {'T', 0x02, 0x1E, 1, 1, 0x121FA00AD77D7422ULL, 0x121FA00AD77D7422ULL, 0x03, 0, 0},
    {'T', 0x03, 0x1C, 1, 1, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x04, 0, 0},
    {'T', 0x04, 0x14, 1, 1, 0x000000000000000EULL, 0x000000000000000EULL, 0x05, 0, 0},
    {'T', 0x05, 0x14, 1, 1, 0x8000000000000000ULL, 0x8000000000000000ULL, 0x06, 0, 0},
    {'T', 0x06, 0x15, 1, 1, 0x0000000012345678ULL, 0x0000000012345678ULL, 0x07, 0, 0},
    {'T', 0x07, 0x15, 1, 1, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x08, 0, 0},
    {'T', 0x08, 0x16, 1, 1, 0x0000000000000002ULL, 0x0000000000000002ULL, 0x09, 0, 0},
    {'T', 0x09, 0x16, 1, 1, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0A, 0, 0},
    {'T', 0x0A, 0x17, 1, 1, 0x000000009ABCDEF0ULL, 0x000000009ABCDEF0ULL, 0x0B, 0, 0},
    {'T', 0x0B, 0x17, 1, 1, 0x0000000000000005ULL, 0x0000000000000005ULL, 0x0C, 0, 0},
    {'T', 0x0C, 0x10, 0, 1, 0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFEULL, 0x0D, 0, 0},
    {'T', 0x0D, 0x11, 0, 1, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x0E, 0, 0},
    {'T', 0x0E, 0x1E, 0, 1, 0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFEULL, 0x0F, 0, 0},
    {'T', 0x0F, 0x1C, 0, 1, 0xFFFFFFFF80000000ULL, 0xFFFFFFFF80000000ULL, 0x10, 0, 0},
    {'T', 0x10, 0x14, 0, 1, 0xFFFFFFFF80000000ULL, 0xFFFFFFFF80000000ULL, 0x11, 0, 0},
    {'T', 0x11, 0x15, 0, 1, 0x00000000000DEADBULL, 0x00000000000DEADBULL, 0x12, 0, 0},
    {'T', 0x12, 0x16, 0, 1, 0x0000000000000002ULL, 0x0000000000000002ULL, 0x13, 0, 0},
    {'T', 0x13, 0x17, 0, 1, 0x0000000000000EEFULL, 0x0000000000000EEFULL, 0x14, 0, 0},
    {'S', 0x14, 0x15, 1, 1, 0x0000000012345678ULL, 0x0000000012345678ULL, 0x55, 'H', 1},
    {'K', 0x15, 0x10, 1, 1, 0x000000000000004DULL, 0x000000000000004DULL, 0x33, 'N', 1},
}};

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

std::vector<std::string> filter_non_debug_lines(const std::vector<std::string>& lines) {
  std::vector<std::string> filtered;
  for (const auto& line : lines) {
    if (!line.empty() && line[0] == 'D') continue;
    filtered.push_back(line);
  }
  return filtered;
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

std::string format_expected(const ExpectedLine& e) {
  char buf[128];
  if (e.aux_tag == 0) {
    std::snprintf(buf, sizeof(buf),
                  "%c%02X F=%02X W=%u P=%u G=%016llX E=%016llX D=%02X",
                  e.prefix, e.idx, e.fn, e.dw, e.pass,
                  static_cast<unsigned long long>(e.got),
                  static_cast<unsigned long long>(e.exp),
                  e.dest);
  } else {
    std::snprintf(buf, sizeof(buf),
                  "%c%02X F=%02X W=%u P=%u G=%016llX E=%016llX D=%02X %c=%u",
                  e.prefix, e.idx, e.fn, e.dw, e.pass,
                  static_cast<unsigned long long>(e.got),
                  static_cast<unsigned long long>(e.exp),
                  e.dest, e.aux_tag, e.aux);
  }
  return std::string(buf);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 30'000'000;

  sim.dut->rst_ni = 1;
  sim.tick();
  sim.dut->rst_ni = 0;
  for (int i = 0; i < 5; ++i) sim.tick();
  sim.dut->rst_ni = 1;
  sim.tick();

  std::vector<std::string> lines;
  for (std::size_t i = 0; i < 64; ++i) {
    lines.push_back(recv_line(sim, i));
    if (lines.back() == kStatusLine || lines.back() == "TEST MUL DIV FAILED") break;
  }

  const auto filtered = filter_non_debug_lines(lines);

  sim.check(filtered.size() == (kExpected.size() + 2), "UART summary line count matches expected output");
  if (!filtered.empty()) sim.check(filtered[0] == kBootLine, "Boot banner matches");

  for (std::size_t i = 0; i < kExpected.size(); ++i) {
    if (filtered.size() > i + 1) {
      sim.check(filtered[i + 1] == format_expected(kExpected[i]),
                "Summary line " + std::to_string(i + 1) + " matches");
    }
  }

  if (filtered.size() == (kExpected.size() + 2)) {
    sim.check(filtered.back() == kStatusLine, "Status line matches");
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

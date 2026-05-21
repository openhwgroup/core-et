// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_coop_tload_tag_table_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_coop_tload_tag_table_tb;

namespace {
constexpr int kReqBits = 40;
constexpr int kReqWords = (2 * kReqBits + 31) / 32;
constexpr int kDataBits = 32;
constexpr int kDataWords = 1;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void clear_words(WData* words, int n) {
  for (int i = 0; i < n; ++i) words[i] = 0;
}

void fill_words(WData* words, int n, uint32_t& seed) {
  for (int i = 0; i < n; ++i) words[i] = xorshift32(seed);
}

template <size_t N>
void sample_words(const WData* words, int nbits, std::array<uint32_t, N>& ones,
                  std::array<uint32_t, N>& zeros) {
  const int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    ones[i] |= words[i] & mask;
    zeros[i] |= (~words[i]) & mask;
  }
}

template <size_t N>
bool full_words(const std::array<uint32_t, N>& ones, const std::array<uint32_t, N>& zeros,
                int nbits) {
  const int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    if ((ones[i] & mask) != mask || (zeros[i] & mask) != mask) return false;
  }
  return true;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("wr_ready", d->orig_wr_ready_o, d->new_wr_ready_o);
  sim.compare("cam_data", d->orig_cam_data_bits_o, d->new_cam_data_bits_o);
}

void tick_compare(CosimCtrl<DUT>& sim, std::array<uint32_t, kReqWords>& req_ones,
                  std::array<uint32_t, kReqWords>& req_zeros, uint8_t& valid_ones,
                  uint8_t& valid_zeros, uint8_t& pop_ones, uint8_t& pop_zeros,
                  uint8_t& tag_ones, uint8_t& tag_zeros) {
  auto* d = sim.dut.get();
  sample_words(d->wr_req_bits_i, 2 * kReqBits, req_ones, req_zeros);
  valid_ones |= d->wr_valid_i & 0x3u;
  valid_zeros |= (~d->wr_valid_i) & 0x3u;
  pop_ones |= d->cam_pop_i & 0x1u;
  pop_zeros |= (~d->cam_pop_i) & 0x1u;
  tag_ones |= d->cam_tag_i;
  tag_zeros |= static_cast<uint8_t>(~d->cam_tag_i) & 0xffu;
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 100000;
  clear_words(sim.dut->wr_req_bits_i, kReqWords);
  sim.dut->wr_valid_i = 0;
  sim.dut->cam_pop_i = 0;
  sim.dut->cam_tag_i = 0;
  sim.reset();
  compare_outputs(sim);

  std::array<uint32_t, kReqWords> req_ones{};
  std::array<uint32_t, kReqWords> req_zeros{};
  uint8_t valid_ones = 0, valid_zeros = 0;
  uint8_t pop_ones = 0, pop_zeros = 0;
  uint8_t tag_ones = 0, tag_zeros = 0;
  uint32_t seed = 0x54414731u;

  for (int cycle = 0; cycle < 6000; ++cycle) {
    fill_words(sim.dut->wr_req_bits_i, kReqWords, seed);
    sim.dut->wr_req_bits_i[kReqWords - 1] &= 0xffffu;
    if ((cycle & 31) == 0) sim.dut->wr_valid_i = 0x3;
    else if ((cycle & 31) == 1) sim.dut->wr_valid_i = 0x0;
    else sim.dut->wr_valid_i = xorshift32(seed) & 0x3u;
    sim.dut->cam_pop_i = ((cycle & 7) == 0) || (xorshift32(seed) & 1u);
    sim.dut->cam_tag_i = (cycle & 15) == 0 ? 0xffu : (xorshift32(seed) & 0xffu);
    tick_compare(sim, req_ones, req_zeros, valid_ones, valid_zeros, pop_ones, pop_zeros,
                 tag_ones, tag_zeros);
  }

  sim.check(full_words(req_ones, req_zeros, 2 * kReqBits),
            "coverage: every wr_req input bit toggled 0/1");
  sim.check(valid_ones == 0x3 && valid_zeros == 0x3,
            "coverage: every wr_valid bit toggled 0/1");
  sim.check(pop_ones == 1 && pop_zeros == 1, "coverage: cam_pop toggled 0/1");
  sim.check(tag_ones == 0xff && tag_zeros == 0xff,
            "coverage: every cam_tag bit toggled 0/1");
  sim.check(sim.comparisons >= 12000, "comparison count >= 12000");
  return sim.finish();
}

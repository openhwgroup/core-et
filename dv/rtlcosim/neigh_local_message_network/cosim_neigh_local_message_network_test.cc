// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_local_message_network_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_local_message_network_tb;

namespace {
constexpr int kValidBits = 24;
constexpr int kInfoBits = 8 * 278;
constexpr int kInfoWords = (kInfoBits + 31) / 32;
constexpr int kDestWords = (8 * 3 * 3 + 31) / 32;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void fill_words(WData* words, int n, uint32_t& seed) {
  for (int i = 0; i < n; ++i) words[i] = xorshift32(seed);
}

void clear_words(WData* words, int n) {
  for (int i = 0; i < n; ++i) words[i] = 0;
}

struct WordCov {
  std::array<uint32_t, 70> ones{};
  std::array<uint32_t, 70> zeros{};
};

void sample_words(const WData* words, int nbits, WordCov& cov) {
  int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    cov.ones[i] |= words[i] & mask;
    cov.zeros[i] |= (~words[i]) & mask;
  }
}

bool full_words(const WordCov& cov, int nbits) {
  int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    if ((cov.ones[i] & mask) != mask || (cov.zeros[i] & mask) != mask) return false;
  }
  return true;
}

void compare_words(CosimCtrl<DUT>& sim, const std::string& name,
                   const WData* orig, const WData* newv, int nwords) {
  for (int i = 0; i < nwords; ++i) sim.compare(name + "[" + std::to_string(i) + "]", orig[i], newv[i]);
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("local_channel_mask", d->orig_local_channel_mask_bits_o, d->new_local_channel_mask_bits_o);
  compare_words(sim, "local_dest_id", d->orig_local_dest_id_bits_o, d->new_local_dest_id_bits_o, kDestWords);
  sim.compare("local_message_ready_out", d->orig_local_message_ready_out_o, d->new_local_message_ready_out_o);
  sim.compare("local_message_valid_in", d->orig_local_message_valid_in_o, d->new_local_message_valid_in_o);
  compare_words(sim, "local_message_info_in", d->orig_local_message_info_in_bits_o,
                d->new_local_message_info_in_bits_o, kInfoWords);
  sim.compare("dbg_local_message_valid_out", d->orig_dbg_local_message_valid_out_o,
              d->new_dbg_local_message_valid_out_o);
}

void tick_compare(CosimCtrl<DUT>& sim, WordCov& info_cov, uint32_t& valid_ones,
                  uint32_t& valid_zeros, uint8_t& ready_ones, uint8_t& ready_zeros) {
  sample_words(sim.dut->local_message_info_out_bits_i, kInfoBits, info_cov);
  valid_ones |= sim.dut->local_message_valid_out_bits_i;
  valid_zeros |= (~sim.dut->local_message_valid_out_bits_i) & ((1u << kValidBits) - 1u);
  ready_ones |= sim.dut->local_message_ready_in_i;
  ready_zeros |= static_cast<uint8_t>(~sim.dut->local_message_ready_in_i) & 0xffu;
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 100000;
  sim.dut->local_message_valid_out_bits_i = 0;
  clear_words(sim.dut->local_message_info_out_bits_i, kInfoWords);
  sim.dut->local_message_ready_in_i = 0xff;
  sim.reset();
  compare_outputs(sim);

  WordCov info_cov;
  uint32_t valid_ones = 0, valid_zeros = 0;
  uint8_t ready_ones = 0, ready_zeros = 0;
  uint32_t seed = 0x6c6d6e01u;

  for (int cycle = 0; cycle < 5000; ++cycle) {
    fill_words(sim.dut->local_message_info_out_bits_i, kInfoWords, seed);
    // Periodic directed all-ones/all-zeros patterns guarantee every channel bit toggles.
    if ((cycle & 31) == 0) sim.dut->local_message_valid_out_bits_i = 0x00ffffffu;
    else if ((cycle & 31) == 1) sim.dut->local_message_valid_out_bits_i = 0;
    else sim.dut->local_message_valid_out_bits_i = xorshift32(seed) & 0x00ffffffu;
    if ((cycle & 15) == 0) sim.dut->local_message_ready_in_i = 0xff;
    else if ((cycle & 15) == 1) sim.dut->local_message_ready_in_i = 0;
    else sim.dut->local_message_ready_in_i = xorshift32(seed) & 0xffu;
    tick_compare(sim, info_cov, valid_ones, valid_zeros, ready_ones, ready_zeros);
  }

  sim.check((valid_ones & 0x00ffffffu) == 0x00ffffffu &&
                (valid_zeros & 0x00ffffffu) == 0x00ffffffu,
            "coverage: every local_message_valid_out bit toggled 0/1");
  sim.check(full_words(info_cov, kInfoBits), "coverage: every local message info input bit toggled 0/1");
  sim.check(ready_ones == 0xff && ready_zeros == 0xff,
            "coverage: every local_message_ready_in bit toggled 0/1");
  sim.check(sim.comparisons >= 350000, "comparison count >= 350000");
  return sim.finish();
}

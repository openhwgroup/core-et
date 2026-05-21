// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_pmu_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_pmu_tb;

namespace {
constexpr int kMinions = 8;
constexpr int kThreads = 2;
constexpr int kXregBits = 64;
constexpr int kPmuMinionCounters = 8;
constexpr int kPmuTotalCounters = 12;
constexpr int kPmuCounterSelBits = 4;
constexpr int kPmuNeighEventCntSelBits = 20;
constexpr int kPmuNeighEventsBits = 25;
constexpr int kReadDataBits = kMinions * kThreads * kXregBits;
constexpr int kReadDataWords = kReadDataBits / 32;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void fill_words(WData* words, int nbits, uint32_t& seed) {
  int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) words[i] = xorshift32(seed);
  if (nbits & 31) words[nwords - 1] &= (uint32_t{1} << (nbits & 31)) - 1u;
}

void clear_words(WData* words, int nbits) {
  for (int i = 0; i < (nbits + 31) / 32; ++i) words[i] = 0;
}

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    int idx = pos + bit;
    uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

struct WordCov {
  std::array<uint32_t, 16> ones{};
  std::array<uint32_t, 16> zeros{};
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

void sample_value(uint64_t value, int nbits, WordCov& cov) {
  for (int i = 0; i < (nbits + 31) / 32; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == ((nbits + 31) / 32) - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    uint32_t word = static_cast<uint32_t>(value >> (32 * i));
    cov.ones[i] |= word & mask;
    cov.zeros[i] |= (~word) & mask;
  }
}

void set_bits_value(QData& word, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    QData mask = QData{1} << (pos + bit);
    if ((value >> bit) & 1ull) word |= mask;
    else word &= ~mask;
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

void compare_outputs(CosimCtrl<DUT>& sim) {
  for (int i = 0; i < kReadDataWords; ++i) {
    sim.compare("pmu_read_data[" + std::to_string(i) + "]",
                sim.dut->orig_pmu_read_data_bits_o[i], sim.dut->new_pmu_read_data_bits_o[i]);
  }
}

void tick_compare(CosimCtrl<DUT>& sim, WordCov& count_cov, WordCov& read_cov,
                  WordCov& write_en_cov, WordCov& write_data_cov, WordCov& neigh_sel_cov,
                  WordCov& neigh_events_cov, bool& dis0, bool& dis1) {
  sample_value(sim.dut->pmu_count_up_bits_i, kMinions * kPmuMinionCounters, count_cov);
  sample_value(sim.dut->pmu_read_sel_bits_i, kMinions * kThreads * kPmuCounterSelBits, read_cov);
  sample_words(sim.dut->pmu_write_en_bits_i, kMinions * kPmuTotalCounters, write_en_cov);
  sample_words(sim.dut->pmu_write_data_bits_i, kMinions * kXregBits, write_data_cov);
  sample_words(sim.dut->pmu_neigh_event_sel_bits_i, kMinions * kPmuNeighEventCntSelBits, neigh_sel_cov);
  sample_words(&sim.dut->pmu_neigh_events_i, kPmuNeighEventsBits + 1, neigh_events_cov);
  if (sim.dut->pmu_disable_clock_i) dis1 = true; else dis0 = true;
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 200000;
  sim.dut->pmu_count_up_bits_i = 0;
  sim.dut->pmu_read_sel_bits_i = 0;
  clear_words(sim.dut->pmu_write_en_bits_i, kMinions * kPmuTotalCounters);
  clear_words(sim.dut->pmu_write_data_bits_i, kMinions * kXregBits);
  clear_words(sim.dut->pmu_neigh_event_sel_bits_i, kMinions * kPmuNeighEventCntSelBits);
  sim.dut->pmu_neigh_events_i = 0;
  sim.dut->pmu_disable_clock_i = 0;
  sim.reset();
  compare_outputs(sim);

  WordCov count_cov, read_cov, write_en_cov, write_data_cov, neigh_sel_cov, neigh_events_cov;
  bool dis0 = false, dis1 = false;
  uint32_t seed = 0x706d7501u;

  for (int cycle = 0; cycle < 5000; ++cycle) {
    sim.dut->pmu_count_up_bits_i = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
    sim.dut->pmu_read_sel_bits_i = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
    fill_words(sim.dut->pmu_write_en_bits_i, kMinions * kPmuTotalCounters, seed);
    fill_words(sim.dut->pmu_write_data_bits_i, kMinions * kXregBits, seed);
    fill_words(sim.dut->pmu_neigh_event_sel_bits_i, kMinions * kPmuNeighEventCntSelBits, seed);
    sim.dut->pmu_neigh_events_i = xorshift32(seed) & 0x03ffffffu;
    sim.dut->pmu_disable_clock_i = ((cycle % 37) == 3);

    // Directed writes/read-selects exercise all counter numbers with single-writer priority cases.
    if ((cycle % 17) == 0) {
      clear_words(sim.dut->pmu_write_en_bits_i, kMinions * kPmuTotalCounters);
      clear_words(sim.dut->pmu_write_data_bits_i, kMinions * kXregBits);
      int minion = (cycle / 17) & 7;
      int counter = (cycle / 17) % kPmuTotalCounters;
      set_bits(sim.dut->pmu_write_en_bits_i, minion * kPmuTotalCounters + counter, 1, 1);
      set_bits(sim.dut->pmu_write_data_bits_i, minion * kXregBits, kXregBits,
               (uint64_t{0x1000} << 32) | static_cast<uint64_t>(cycle));
      for (int m = 0; m < kMinions; ++m) {
        for (int t = 0; t < kThreads; ++t) {
          set_bits_value(sim.dut->pmu_read_sel_bits_i, (m*kThreads + t)*kPmuCounterSelBits,
                         kPmuCounterSelBits, counter);
        }
      }
    }

    tick_compare(sim, count_cov, read_cov, write_en_cov, write_data_cov, neigh_sel_cov,
                 neigh_events_cov, dis0, dis1);
  }

  sim.check(full_words(count_cov, kMinions * kPmuMinionCounters), "coverage: pmu_count_up bits toggled 0/1");
  sim.check(full_words(read_cov, kMinions * kThreads * kPmuCounterSelBits), "coverage: pmu_read_sel bits toggled 0/1");
  sim.check(full_words(write_en_cov, kMinions * kPmuTotalCounters), "coverage: pmu_write_en bits toggled 0/1");
  sim.check(full_words(write_data_cov, kMinions * kXregBits), "coverage: pmu_write_data bits toggled 0/1");
  sim.check(full_words(neigh_sel_cov, kMinions * kPmuNeighEventCntSelBits), "coverage: pmu_neigh_event_sel bits toggled 0/1");
  sim.check(full_words(neigh_events_cov, kPmuNeighEventsBits + 1), "coverage: pmu_neigh_events bits toggled 0/1");
  sim.check(dis0 && dis1, "coverage: pmu_disable_clock toggled 0/1");
  sim.check(sim.comparisons >= 160000, "comparison count >= 160000");
  return sim.finish();
}

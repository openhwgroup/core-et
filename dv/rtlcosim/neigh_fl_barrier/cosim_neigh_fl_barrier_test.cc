// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_fl_barrier_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_fl_barrier_tb;

namespace {
constexpr int kMinPerN = 8;
constexpr int kFlbWidth = 13;
constexpr int kReqDataBits = kMinPerN * kFlbWidth;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    int idx = pos + bit;
    uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

struct BitCov {
  std::array<uint32_t, 4> ones{};
  std::array<uint32_t, 4> zeros{};
};

void sample_bits(const WData* words, int nbits, BitCov& cov) {
  for (int i = 0; i < (nbits + 31) / 32; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == ((nbits + 31) / 32) - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    cov.ones[i] |= words[i] & mask;
    cov.zeros[i] |= (~words[i]) & mask;
  }
}

bool full_bits(const BitCov& cov, int nbits) {
  for (int i = 0; i < (nbits + 31) / 32; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == ((nbits + 31) / 32) - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    if ((cov.ones[i] & mask) != mask || (cov.zeros[i] & mask) != mask) return false;
  }
  return true;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("flb_neigh_core_resp_valid", d->orig_flb_neigh_core_resp_valid_o,
              d->new_flb_neigh_core_resp_valid_o);
  sim.compare("flb_neigh_core_resp_data", d->orig_flb_neigh_core_resp_data_o,
              d->new_flb_neigh_core_resp_data_o);
  sim.compare("flb_neigh_l2_req_valid", d->orig_flb_neigh_l2_req_valid_o,
              d->new_flb_neigh_l2_req_valid_o);
  sim.compare("flb_neigh_l2_req_data", d->orig_flb_neigh_l2_req_data_o,
              d->new_flb_neigh_l2_req_data_o);
}

void tick_compare(CosimCtrl<DUT>& sim, BitCov& data_cov, uint8_t& valid_ones,
                  uint8_t& valid_zeros, bool& resp0, bool& resp1, bool& resp_data0,
                  bool& resp_data1) {
  sample_bits(sim.dut->flb_core_neigh_req_data_bits_i, kReqDataBits, data_cov);
  valid_ones |= sim.dut->flb_core_neigh_req_valid_i;
  valid_zeros |= static_cast<uint8_t>(~sim.dut->flb_core_neigh_req_valid_i) & 0xffu;
  if (sim.dut->flb_l2_neigh_resp_valid_i) resp1 = true; else resp0 = true;
  if (sim.dut->flb_l2_neigh_resp_data_i) resp_data1 = true; else resp_data0 = true;
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 200000;
  clear_words(sim.dut->flb_core_neigh_req_data_bits_i, 4);
  sim.dut->flb_core_neigh_req_valid_i = 0;
  sim.dut->flb_l2_neigh_resp_valid_i = 0;
  sim.dut->flb_l2_neigh_resp_data_i = 0;
  sim.reset();
  compare_outputs(sim);

  BitCov data_cov;
  uint8_t valid_ones = 0, valid_zeros = 0;
  bool resp0 = false, resp1 = false, resp_data0 = false, resp_data1 = false;
  int outstanding = 0;
  uint32_t seed = 0x514231u;

  for (int cycle = 0; cycle < 3000; ++cycle) {
    clear_words(sim.dut->flb_core_neigh_req_data_bits_i, 4);
    uint8_t req_valid = 0;
    for (int m = 0; m < kMinPerN; ++m) {
      uint16_t data = static_cast<uint16_t>(xorshift32(seed) & 0x1fffu);
      set_bits(sim.dut->flb_core_neigh_req_data_bits_i, m * kFlbWidth, kFlbWidth, data);
      if ((xorshift32(seed) & 0xfu) == 0 && outstanding < 7) {
        req_valid |= uint8_t(1u << m);
        outstanding++;
      }
    }
    sim.dut->flb_core_neigh_req_valid_i = req_valid;
    bool do_resp = (outstanding > 0) && ((xorshift32(seed) & 0x3u) == 0);
    sim.dut->flb_l2_neigh_resp_valid_i = do_resp;
    sim.dut->flb_l2_neigh_resp_data_i = (xorshift32(seed) & 1u) != 0;
    if (do_resp) outstanding--;
    tick_compare(sim, data_cov, valid_ones, valid_zeros, resp0, resp1, resp_data0, resp_data1);
  }

  for (int i = 0; i < 16; ++i) {
    sim.dut->flb_core_neigh_req_valid_i = 0;
    sim.dut->flb_l2_neigh_resp_valid_i = outstanding > 0;
    sim.dut->flb_l2_neigh_resp_data_i = (i & 1) != 0;
    if (outstanding > 0) outstanding--;
    tick_compare(sim, data_cov, valid_ones, valid_zeros, resp0, resp1, resp_data0, resp_data1);
  }

  sim.check(valid_ones == 0xff && valid_zeros == 0xff, "coverage: every FLB request-valid bit toggled 0/1");
  sim.check(full_bits(data_cov, kReqDataBits), "coverage: every FLB request-data bit toggled 0/1");
  sim.check(resp0 && resp1, "coverage: FLB L2 response valid toggled 0/1");
  sim.check(resp_data0 && resp_data1, "coverage: FLB L2 response data toggled 0/1");
  sim.check(sim.comparisons >= 12000, "comparison count >= 12000");
  return sim.finish();
}

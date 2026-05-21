// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_coop_tload_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_coop_tload_tb;

namespace {
constexpr int kMinPerN = 8;
constexpr int kMissReqs = 2;
constexpr int kSlots = kMinPerN * kMissReqs;
constexpr int kReqBits = 86;
constexpr int kReqWords = (kSlots * kReqBits + 31) / 32;
constexpr int kNeighReqBits = 328;
constexpr int kNeighReqWords = (kNeighReqBits + 31) / 32;
constexpr int kDbgBits = 112;
constexpr int kDbgWords = (kDbgBits + 31) / 32;
constexpr int kSlvRdyBits = 7;

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

void set_wide_bits(WData* words, int bit, int width, uint64_t value) {
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    const uint32_t mask = uint32_t{1} << (pos & 31);
    if ((value >> i) & 1u) words[pos >> 5] |= mask;
    else words[pos >> 5] &= ~mask;
  }
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

uint32_t coop_data(uint32_t neigh_mask, uint32_t min_mask, uint32_t seq_id, uint32_t coop_id) {
  return (min_mask & 0xffu) | ((seq_id & 0x7u) << 8) |
         ((coop_id & 0x1fu) << 11) | ((neigh_mask & 0xfu) << 16);
}

uint32_t slv_rdy_bits(uint32_t dest, uint32_t coop_id) {
  return ((dest & 0x3u) << 5) | (coop_id & 0x1fu);
}

uint32_t tag(uint32_t coop_id, uint32_t seq_id) {
  return ((coop_id & 0x1fu) << 3) | (seq_id & 0x7u);
}

void clear_inputs(DUT* d) {
  d->neigh_id_i = 0;
  clear_words(d->min_req_bits_i, kReqWords);
  d->min_valid_bits_i = 0;
  d->coop_tload_disable_next_i = 0;
  d->coop_tload_req_ready_i = 1;
  d->slv_rdy_in_data_bits_i = 0;
  d->slv_rdy_in_valid_i = 0;
  d->mst_done_in_coop_id_bits_i = 0;
  d->mst_done_in_valid_i = 0;
  d->tag_table_cam_pop_i = 0;
  d->tag_table_cam_tag_i = 0;
}

void set_req_slot(DUT* d, int slot, uint32_t id, uint64_t addr, uint32_t data,
                  uint32_t size, uint32_t subopcode, uint32_t opcode = 1,
                  uint32_t wdata = 0, uint32_t source = 0, uint32_t qwen = 0) {
  const int base = slot * kReqBits;
  set_wide_bits(d->min_req_bits_i, base + 0, 7, subopcode);
  set_wide_bits(d->min_req_bits_i, base + 7, 4, qwen);
  set_wide_bits(d->min_req_bits_i, base + 11, 3, size);
  set_wide_bits(d->min_req_bits_i, base + 14, 20, data);
  set_wide_bits(d->min_req_bits_i, base + 34, 40, addr);
  set_wide_bits(d->min_req_bits_i, base + 74, 5, opcode);
  set_wide_bits(d->min_req_bits_i, base + 79, 1, wdata);
  set_wide_bits(d->min_req_bits_i, base + 80, 1, source);
  set_wide_bits(d->min_req_bits_i, base + 81, 5, id);
}

void drive_one_req(DUT* d, int slot, uint32_t id, uint32_t addr_sel, uint32_t data,
                   uint32_t size, uint32_t subopcode) {
  clear_words(d->min_req_bits_i, kReqWords);
  d->min_valid_bits_i = uint32_t{1} << slot;
  set_req_slot(d, slot, id, uint64_t{addr_sel & 0x3u} << 6, data, size, subopcode);
}

void compare_words(CosimCtrl<DUT>& sim, const std::string& name, const WData* orig,
                   const WData* newv, int nwords) {
  for (int i = 0; i < nwords; ++i) {
    sim.compare(name + "[" + std::to_string(i) + "]", orig[i], newv[i]);
  }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("min_ready", d->orig_min_ready_bits_o, d->new_min_ready_bits_o);
  compare_words(sim, "req", d->orig_req_bits_o, d->new_req_bits_o, kNeighReqWords);
  sim.compare("req_bid", d->orig_req_bid_o, d->new_req_bid_o);
  sim.compare("slv_rdy_out_data", d->orig_slv_rdy_out_data_bits_o, d->new_slv_rdy_out_data_bits_o);
  sim.compare("slv_rdy_out_valid", d->orig_slv_rdy_out_valid_o, d->new_slv_rdy_out_valid_o);
  sim.compare("mst_done_out_coop_id", d->orig_mst_done_out_coop_id_o, d->new_mst_done_out_coop_id_o);
  sim.compare("mst_done_out_valid", d->orig_mst_done_out_valid_o, d->new_mst_done_out_valid_o);
  sim.compare("cam_data", d->orig_cam_data_bits_o, d->new_cam_data_bits_o);
  compare_words(sim, "dbg", d->orig_dbg_bits_o, d->new_dbg_bits_o, kDbgWords);
}

struct Coverage {
  std::array<uint32_t, kReqWords> req_ones{};
  std::array<uint32_t, kReqWords> req_zeros{};
  uint8_t neigh_ones = 0, neigh_zeros = 0;
  uint32_t valid_ones = 0, valid_zeros = 0;
  uint8_t disable_ones = 0, disable_zeros = 0;
  uint8_t ready_ones = 0, ready_zeros = 0;
  uint32_t slv_data_ones = 0, slv_data_zeros = 0;
  uint8_t slv_valid_ones = 0, slv_valid_zeros = 0;
  uint16_t done_id_ones = 0, done_id_zeros = 0;
  uint8_t done_valid_ones = 0, done_valid_zeros = 0;
  uint8_t cam_pop_ones = 0, cam_pop_zeros = 0;
  uint8_t cam_tag_ones = 0, cam_tag_zeros = 0;
};

void sample_inputs(DUT* d, Coverage& cov) {
  sample_words(d->min_req_bits_i, kSlots * kReqBits, cov.req_ones, cov.req_zeros);
  cov.neigh_ones |= d->neigh_id_i;
  cov.neigh_zeros |= static_cast<uint8_t>(~d->neigh_id_i) & 0x3u;
  cov.valid_ones |= d->min_valid_bits_i;
  cov.valid_zeros |= (~d->min_valid_bits_i) & 0xffffu;
  cov.disable_ones |= d->coop_tload_disable_next_i & 1u;
  cov.disable_zeros |= (~d->coop_tload_disable_next_i) & 1u;
  cov.ready_ones |= d->coop_tload_req_ready_i & 1u;
  cov.ready_zeros |= (~d->coop_tload_req_ready_i) & 1u;
  cov.slv_data_ones |= d->slv_rdy_in_data_bits_i & 0x1fffffu;
  cov.slv_data_zeros |= (~d->slv_rdy_in_data_bits_i) & 0x1fffffu;
  cov.slv_valid_ones |= d->slv_rdy_in_valid_i & 0x7u;
  cov.slv_valid_zeros |= (~d->slv_rdy_in_valid_i) & 0x7u;
  cov.done_id_ones |= d->mst_done_in_coop_id_bits_i & 0x7fffu;
  cov.done_id_zeros |= (~d->mst_done_in_coop_id_bits_i) & 0x7fffu;
  cov.done_valid_ones |= d->mst_done_in_valid_i & 0x7u;
  cov.done_valid_zeros |= (~d->mst_done_in_valid_i) & 0x7u;
  cov.cam_pop_ones |= d->tag_table_cam_pop_i & 1u;
  cov.cam_pop_zeros |= (~d->tag_table_cam_pop_i) & 1u;
  cov.cam_tag_ones |= d->tag_table_cam_tag_i;
  cov.cam_tag_zeros |= static_cast<uint8_t>(~d->tag_table_cam_tag_i) & 0xffu;
}

void tick_compare(CosimCtrl<DUT>& sim, Coverage& cov) {
  sample_inputs(sim.dut.get(), cov);
  sim.tick();
  compare_outputs(sim);
}

bool wait_for_req_bid(CosimCtrl<DUT>& sim, Coverage& cov, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    tick_compare(sim, cov);
    if (sim.dut->new_req_bid_o) return true;
  }
  return false;
}

bool wait_for_slv_valid(CosimCtrl<DUT>& sim, Coverage& cov, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    tick_compare(sim, cov);
    if (sim.dut->new_slv_rdy_out_valid_o) return true;
  }
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 1000000;
  auto* d = sim.dut.get();
  clear_inputs(d);
  sim.reset();
  compare_outputs(sim);

  Coverage cov;

  // Directed slave-neighborhood path.
  d->neigh_id_i = 0;
  drive_one_req(d, 0, 0x12, 1, coop_data(0x2, 0x01, 2, 5), 4, 0x2a);
  tick_compare(sim, cov);
  d->min_valid_bits_i = 0;
  sim.check(wait_for_slv_valid(sim, cov, 12), "directed: slave-ready output observed");
  d->tag_table_cam_tag_i = tag(5, 2);
  d->tag_table_cam_pop_i = 1;
  tick_compare(sim, cov);
  d->tag_table_cam_pop_i = 0;

  // Directed local-master path.
  drive_one_req(d, 0, 0x13, 0, coop_data(0x1, 0x01, 3, 6), 5, 0x31);
  tick_compare(sim, cov);
  d->min_valid_bits_i = 0;
  sim.check(wait_for_req_bid(sim, cov, 12), "directed: local-master request observed");
  d->tag_table_cam_tag_i = tag(6, 3);
  d->tag_table_cam_pop_i = 1;
  tick_compare(sim, cov);
  d->tag_table_cam_pop_i = 0;

  // Directed inter-neighborhood master path after an external ready sideband.
  d->slv_rdy_in_data_bits_i = slv_rdy_bits(0, 7);
  d->slv_rdy_in_valid_i = 0x1;
  tick_compare(sim, cov);
  d->slv_rdy_in_valid_i = 0;
  d->slv_rdy_in_data_bits_i = 0;
  tick_compare(sim, cov);
  tick_compare(sim, cov);

  drive_one_req(d, 0, 0x14, 0, coop_data(0x3, 0x01, 1, 7), 6, 0x12);
  tick_compare(sim, cov);
  d->min_valid_bits_i = 0;
  sim.check(wait_for_req_bid(sim, cov, 16), "directed: inter-neighborhood master request observed");

  // Directed done-credit return path for slave credits.
  d->mst_done_in_coop_id_bits_i = 7 | (8 << 5) | (9 << 10);
  d->mst_done_in_valid_i = 0x7;
  tick_compare(sim, cov);
  d->mst_done_in_valid_i = 0;

  // Random stress with periodic all-ones/all-zeros patterns for input toggle coverage.
  uint32_t seed = 0x434f4f50u;
  for (int cycle = 0; cycle < 10000; ++cycle) {
    if ((cycle & 63) == 0) {
      for (int i = 0; i < kReqWords; ++i) d->min_req_bits_i[i] = 0xffffffffu;
      d->min_valid_bits_i = 0xffffu;
      d->slv_rdy_in_data_bits_i = 0x1fffffu;
      d->slv_rdy_in_valid_i = 0x7;
      d->mst_done_in_coop_id_bits_i = 0x7fffu;
      d->mst_done_in_valid_i = 0x7;
      d->neigh_id_i = 0x3;
      d->coop_tload_disable_next_i = 1;
      d->coop_tload_req_ready_i = 1;
      d->tag_table_cam_pop_i = 1;
      d->tag_table_cam_tag_i = 0xffu;
    } else if ((cycle & 63) == 1) {
      clear_words(d->min_req_bits_i, kReqWords);
      d->min_valid_bits_i = 0;
      d->slv_rdy_in_data_bits_i = 0;
      d->slv_rdy_in_valid_i = 0;
      d->mst_done_in_coop_id_bits_i = 0;
      d->mst_done_in_valid_i = 0;
      d->neigh_id_i = 0;
      d->coop_tload_disable_next_i = 0;
      d->coop_tload_req_ready_i = 0;
      d->tag_table_cam_pop_i = 0;
      d->tag_table_cam_tag_i = 0;
    } else {
      fill_words(d->min_req_bits_i, kReqWords, seed);
      d->min_valid_bits_i = xorshift32(seed) & 0xffffu;
      d->slv_rdy_in_data_bits_i = xorshift32(seed) & 0x1fffffu;
      d->slv_rdy_in_valid_i = xorshift32(seed) & 0x7u;
      d->mst_done_in_coop_id_bits_i = xorshift32(seed) & 0x7fffu;
      d->mst_done_in_valid_i = xorshift32(seed) & 0x7u;
      d->neigh_id_i = xorshift32(seed) & 0x3u;
      d->coop_tload_disable_next_i = xorshift32(seed) & 1u;
      d->coop_tload_req_ready_i = ((cycle & 7) != 0) ? 1 : (xorshift32(seed) & 1u);
      d->tag_table_cam_pop_i = xorshift32(seed) & 1u;
      d->tag_table_cam_tag_i = xorshift32(seed) & 0xffu;
    }
    tick_compare(sim, cov);
  }

  sim.check(full_words(cov.req_ones, cov.req_zeros, kSlots * kReqBits),
            "coverage: every min_coop_miss_req bit toggled 0/1");
  sim.check(cov.neigh_ones == 0x3 && cov.neigh_zeros == 0x3,
            "coverage: neigh_id toggled 0/1");
  sim.check(cov.valid_ones == 0xffff && cov.valid_zeros == 0xffff,
            "coverage: every min_coop_miss_req_valid bit toggled 0/1");
  sim.check(cov.disable_ones == 1 && cov.disable_zeros == 1,
            "coverage: coop_tload_disable_next toggled 0/1");
  sim.check(cov.ready_ones == 1 && cov.ready_zeros == 1,
            "coverage: coop_tload_req_ready toggled 0/1");
  sim.check(cov.slv_data_ones == 0x1fffff && cov.slv_data_zeros == 0x1fffff,
            "coverage: every slave-ready input data bit toggled 0/1");
  sim.check(cov.slv_valid_ones == 0x7 && cov.slv_valid_zeros == 0x7,
            "coverage: every slave-ready input valid bit toggled 0/1");
  sim.check(cov.done_id_ones == 0x7fff && cov.done_id_zeros == 0x7fff,
            "coverage: every master-done input id bit toggled 0/1");
  sim.check(cov.done_valid_ones == 0x7 && cov.done_valid_zeros == 0x7,
            "coverage: every master-done input valid bit toggled 0/1");
  sim.check(cov.cam_pop_ones == 1 && cov.cam_pop_zeros == 1,
            "coverage: tag_table_cam_pop toggled 0/1");
  sim.check(cov.cam_tag_ones == 0xff && cov.cam_tag_zeros == 0xff,
            "coverage: every tag_table_cam_tag bit toggled 0/1");
  sim.check(sim.comparisons >= 200000, "comparison count >= 200000");
  return sim.finish();
}

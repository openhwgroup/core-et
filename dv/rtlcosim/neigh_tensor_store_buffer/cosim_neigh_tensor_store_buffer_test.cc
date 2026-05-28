// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_tensor_store_buffer_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_tensor_store_buffer_tb;

namespace {
constexpr int kMinPerN = 8;
constexpr int kReqBits = 328;
constexpr int kReqWords = (kMinPerN * kReqBits + 31) / 32;
constexpr int kOutReqWords = (kReqBits + 31) / 32;
constexpr int kRspBits = 275;
constexpr int kRspWords = (kMinPerN * kRspBits + 31) / 32;
constexpr int kSubopcodeOff = 0;
constexpr int kQwenOff = 7;
constexpr int kSizeOff = 11;
constexpr int kDataOff = 14;
constexpr int kAddrOff = 270;
constexpr int kOpcodeOff = 310;
constexpr int kWdataOff = 315;
constexpr int kSourceOff = 316;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

void fill_words(WData* words, int nwords, uint32_t& seed) {
  for (int i = 0; i < nwords; ++i) words[i] = xorshift32(seed);
}

void set_bits(WData* words, int bit, int width, uint64_t value) {
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    const uint32_t mask = uint32_t{1} << (pos & 31);
    if ((value >> i) & 1ull) words[pos >> 5] |= mask;
    else words[pos >> 5] &= ~mask;
  }
}

uint64_t get_bits(const WData* words, int bit, int width) {
  uint64_t value = 0;
  for (int i = 0; i < width; ++i) {
    const int pos = bit + i;
    if ((words[pos >> 5] >> (pos & 31)) & 1u) value |= uint64_t{1} << i;
  }
  return value;
}

void set_req(WData* words, int slot, uint32_t subopcode, uint32_t qwen,
             uint32_t size, uint64_t addr, uint32_t seed_data) {
  const int base = slot * kReqBits;
  set_bits(words, base + kSubopcodeOff, 7, subopcode);
  set_bits(words, base + kQwenOff, 4, qwen);
  set_bits(words, base + kSizeOff, 3, size);
  for (int lane = 0; lane < 4; ++lane) {
    set_bits(words, base + kDataOff + lane * 64, 64,
             (uint64_t{seed_data} << 32) | uint32_t(seed_data + lane));
  }
  set_bits(words, base + kAddrOff, 40, addr);
  set_bits(words, base + kOpcodeOff, 5, 2);
  set_bits(words, base + kWdataOff, 1, 1);
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

void compare_words(CosimCtrl<DUT>& sim, const std::string& name, const WData* orig,
                   const WData* newv, int nwords) {
  for (int i = 0; i < nwords; ++i) {
    sim.compare(name + "[" + std::to_string(i) + "]", orig[i], newv[i]);
  }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("evict_ready", d->orig_evict_ready_o, d->new_evict_ready_o);
  sim.compare("evict_out_valid", d->orig_evict_out_valid_o, d->new_evict_out_valid_o);
  compare_words(sim, "evict_out_req", d->orig_evict_out_req_bits_o, d->new_evict_out_req_bits_o,
                kOutReqWords);
  sim.compare("tstore_min_resp_valid", d->orig_tstore_min_resp_valid_o, d->new_tstore_min_resp_valid_o);
  compare_words(sim, "tstore_min_resp", d->orig_tstore_min_resp_bits_o,
                d->new_tstore_min_resp_bits_o, kRspWords);
  sim.compare("dbg", d->orig_dbg_bits_o, d->new_dbg_bits_o);
}

struct Coverage {
  std::array<uint32_t, kReqWords> req_ones{};
  std::array<uint32_t, kReqWords> req_zeros{};
  uint16_t valid_ones = 0, valid_zeros = 0;
  uint8_t out_ready_ones = 0, out_ready_zeros = 0;
  uint8_t disable_ones = 0, disable_zeros = 0;
  uint16_t ack_ones = 0, ack_zeros = 0;
  uint16_t resp_ready_ones = 0, resp_ready_zeros = 0;
};

void sample_inputs(DUT* d, Coverage& cov) {
  sample_words(d->evict_req_bits_i, kMinPerN * kReqBits, cov.req_ones, cov.req_zeros);
  cov.valid_ones |= d->evict_valid_i & 0xffu;
  cov.valid_zeros |= (~d->evict_valid_i) & 0xffu;
  cov.out_ready_ones |= d->evict_out_ready_i & 1u;
  cov.out_ready_zeros |= (~d->evict_out_ready_i) & 1u;
  cov.disable_ones |= d->evict_out_disable_next_i & 1u;
  cov.disable_zeros |= (~d->evict_out_disable_next_i) & 1u;
  cov.ack_ones |= d->tstore_min_ack_i & 0xffu;
  cov.ack_zeros |= (~d->tstore_min_ack_i) & 0xffu;
  cov.resp_ready_ones |= d->tstore_min_resp_ready_i & 0xffu;
  cov.resp_ready_zeros |= (~d->tstore_min_resp_ready_i) & 0xffu;
}

void tick_compare(CosimCtrl<DUT>& sim, Coverage& cov) {
  sample_inputs(sim.dut.get(), cov);
  sim.tick();
  compare_outputs(sim);
}

bool wait_for_valid(CosimCtrl<DUT>& sim, Coverage& cov, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    tick_compare(sim, cov);
    if (sim.dut->new_evict_out_valid_o) return true;
  }
  return false;
}

void clear_inputs(DUT* d) {
  clear_words(d->evict_req_bits_i, kReqWords);
  d->evict_valid_i = 0;
  d->evict_out_ready_i = 0;
  d->evict_out_disable_next_i = 0;
  d->tstore_min_ack_i = 0;
  d->tstore_min_resp_ready_i = 0xff;
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 1500000;
  auto* d = sim.dut.get();
  clear_inputs(d);
  sim.reset();
  compare_outputs(sim);
  Coverage cov;

  // Directed north/south two-way traffic proves arbitration and SourceOffset=4 on the south block.
  set_req(d->evict_req_bits_i, 0, 1, 0x1, 4, 0x1000, 0x10);
  set_req(d->evict_req_bits_i, 1, 1, 0x2, 4, 0x1000, 0x20);
  set_req(d->evict_req_bits_i, 4, 1, 0x1, 4, 0x2000, 0x30);
  set_req(d->evict_req_bits_i, 5, 1, 0x2, 4, 0x2000, 0x40);
  d->evict_valid_i = 0x33;
  d->evict_out_ready_i = 1;
  bool saw_north = false;
  bool saw_south = false;
  for (int cycle = 0; cycle < 80 && !(saw_north && saw_south); ++cycle) {
    tick_compare(sim, cov);
    if (d->new_evict_out_valid_o) {
      const uint32_t source = static_cast<uint32_t>(get_bits(d->new_evict_out_req_bits_o, kSourceOff, 4));
      if (source == 0) saw_north = true;
      if (source == 4) saw_south = true;
    }
    if (d->new_evict_ready_o & 0x03) d->evict_valid_i &= ~0x03u;
    if (d->new_evict_ready_o & 0x30) d->evict_valid_i &= ~0x30u;
  }
  sim.check(saw_north, "directed: top emitted north source 0 request");
  sim.check(saw_south, "directed: top emitted south source-offset 4 request");

  // Directed four-way QWord drives first/second-pass behavior through the top FIFO.
  clear_words(d->evict_req_bits_i, kReqWords);
  for (int m = 0; m < 4; ++m) set_req(d->evict_req_bits_i, m, 2, 1u << m, 4, 0x3000, 0x50 + m);
  d->evict_valid_i = 0x0f;
  d->evict_out_ready_i = 1;
  int fourway_outputs = 0;
  for (int cycle = 0; cycle < 80 && fourway_outputs < 2; ++cycle) {
    tick_compare(sim, cov);
    if (d->new_evict_out_valid_o) fourway_outputs++;
    if (d->new_evict_ready_o & 0x0f) d->evict_valid_i = 0;
  }
  sim.check(fourway_outputs >= 2, "directed: top emitted both passes for 4-way QWord");
  d->tstore_min_ack_i = 0x1;
  tick_compare(sim, cov);
  d->tstore_min_ack_i = 0;
  d->tstore_min_resp_ready_i = 0x0e;
  tick_compare(sim, cov);
  d->tstore_min_resp_ready_i = 0xff;

  // Output-disable path with a held FIFO entry.
  clear_inputs(d);
  sim.reset();
  set_req(d->evict_req_bits_i, 0, 1, 0x1, 4, 0x4000, 0x90);
  set_req(d->evict_req_bits_i, 1, 1, 0x2, 4, 0x4000, 0xa0);
  d->evict_valid_i = 0x03;
  d->evict_out_ready_i = 0;
  sim.check(wait_for_valid(sim, cov, 32), "directed: top FIFO holds a request before disable");
  d->evict_out_disable_next_i = 1;
  tick_compare(sim, cov);
  d->evict_out_disable_next_i = 0;
  tick_compare(sim, cov);
  d->evict_out_ready_i = 1;
  tick_compare(sim, cov);

  // Random stress with periodic all-ones/all-zeros to prove input toggle coverage and FIFO full/empty transitions.
  uint32_t seed = 0x54534254u;
  for (int cycle = 0; cycle < 6000; ++cycle) {
    if ((cycle & 63) == 0) {
      for (int i = 0; i < kReqWords; ++i) d->evict_req_bits_i[i] = 0xffffffffu;
      d->evict_valid_i = 0xff;
      d->evict_out_ready_i = 1;
      d->evict_out_disable_next_i = 1;
      d->tstore_min_ack_i = 0xff;
      d->tstore_min_resp_ready_i = 0xff;
    } else if ((cycle & 63) == 1) {
      clear_words(d->evict_req_bits_i, kReqWords);
      d->evict_valid_i = 0;
      d->evict_out_ready_i = 0;
      d->evict_out_disable_next_i = 0;
      d->tstore_min_ack_i = 0;
      d->tstore_min_resp_ready_i = 0;
    } else {
      fill_words(d->evict_req_bits_i, kReqWords, seed);
      d->evict_valid_i = xorshift32(seed) & 0xffu;
      d->evict_out_ready_i = ((cycle & 7) != 0) ? 1 : (xorshift32(seed) & 1u);
      d->evict_out_disable_next_i = xorshift32(seed) & 1u;
      d->tstore_min_ack_i = xorshift32(seed) & 0xffu;
      d->tstore_min_resp_ready_i = xorshift32(seed) & 0xffu;
    }
    tick_compare(sim, cov);
  }

  sim.check(full_words(cov.req_ones, cov.req_zeros, kMinPerN * kReqBits),
            "coverage: every top evict_req bit toggled 0/1");
  sim.check(cov.valid_ones == 0xff && cov.valid_zeros == 0xff,
            "coverage: every top evict_valid bit toggled 0/1");
  sim.check(cov.out_ready_ones == 1 && cov.out_ready_zeros == 1,
            "coverage: top evict_out_ready toggled 0/1");
  sim.check(cov.disable_ones == 1 && cov.disable_zeros == 1,
            "coverage: top evict_out_disable_next toggled 0/1");
  sim.check(cov.ack_ones == 0xff && cov.ack_zeros == 0xff,
            "coverage: every top tstore_min_ack bit toggled 0/1");
  sim.check(cov.resp_ready_ones == 0xff && cov.resp_ready_zeros == 0xff,
            "coverage: every top tstore_min_resp_ready bit toggled 0/1");
  sim.check(sim.comparisons >= 250000, "comparison count >= 250000");
  return sim.finish();
}

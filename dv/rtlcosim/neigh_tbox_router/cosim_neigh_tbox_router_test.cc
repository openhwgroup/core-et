// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_tbox_router_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_neigh_tbox_router_tb;

namespace {
constexpr int kReqBits = 331;
constexpr int kReqWords = (kReqBits + 31) / 32;
constexpr int kNeighRspBits = 278;
constexpr int kNeighRspWords = (kNeighRspBits + 31) / 32;
constexpr int kRspBits = 534;
constexpr int kRspWords = (kRspBits + 31) / 32;
constexpr int kReqNoDataBits = 75;
constexpr int kReqNoDataWords = (kReqNoDataBits + 31) / 32;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void clear_words(WData* words, int nwords) {
  for (int i = 0; i < nwords; ++i) words[i] = 0;
}

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    int idx = pos + bit;
    uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

void set_u64(WData* words, int pos, uint64_t value) {
  set_bits(words, pos, 32, value & 0xffffffffull);
  set_bits(words, pos + 32, 32, value >> 32);
}

void random_words(WData* words, int nbits, uint32_t& seed) {
  int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) words[i] = xorshift32(seed);
  if (nbits & 31) words[nwords - 1] &= (uint32_t{1} << (nbits & 31)) - 1u;
}

void set_neigh_req(WData* words, uint8_t id, uint8_t source, bool wdata, uint8_t opcode,
                   uint64_t address, uint64_t data0, uint8_t size, uint8_t qwen,
                   uint16_t subopcode) {
  clear_words(words, kReqWords);
  set_bits(words, 0, 10, subopcode);
  set_bits(words, 10, 4, qwen);
  set_bits(words, 14, 3, size);
  set_u64(words, 17, data0);
  set_bits(words, 273, 40, address);
  set_bits(words, 313, 5, opcode);
  set_bits(words, 318, 1, wdata ? 1 : 0);
  set_bits(words, 319, 4, source);
  set_bits(words, 323, 8, id);
}

void set_rsp(WData* words, uint8_t id, uint8_t dest, bool wdata, uint8_t opcode,
             uint64_t data0, uint8_t size, uint8_t qwen) {
  clear_words(words, kRspWords);
  set_bits(words, 0, 4, qwen);
  set_bits(words, 4, 3, size);
  set_u64(words, 7, data0);
  set_bits(words, 519, 2, opcode);
  set_bits(words, 521, 1, wdata ? 1 : 0);
  set_bits(words, 522, 4, dest);
  set_bits(words, 526, 8, id);
}

void set_neigh_req_data_random(WData* words, uint32_t& seed) {
  for (int chunk = 0; chunk < 4; ++chunk) {
    uint64_t value = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
    set_u64(words, 17 + chunk * 64, value);
  }
}

void set_rsp_data_random(WData* words, uint32_t& seed) {
  for (int chunk = 0; chunk < 8; ++chunk) {
    uint64_t value = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
    set_u64(words, 7 + chunk * 64, value);
  }
}

struct WordCov {
  std::array<uint32_t, 17> ones{};
  std::array<uint32_t, 17> zeros{};
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
  sim.compare("min_tbox_req_ready", d->orig_min_tbox_req_ready_o, d->new_min_tbox_req_ready_o);
  compare_words(sim, "tbox_min_rsp_info", d->orig_tbox_min_rsp_info_bits_o,
                d->new_tbox_min_rsp_info_bits_o, kNeighRspWords);
  sim.compare("tbox_min_rsp_valid", d->orig_tbox_min_rsp_valid_o, d->new_tbox_min_rsp_valid_o);
  compare_words(sim, "tbox_sc_req_info", d->orig_tbox_sc_req_info_bits_o,
                d->new_tbox_sc_req_info_bits_o, kReqNoDataWords);
  sim.compare("tbox_sc_req_valid", d->orig_tbox_sc_req_valid_o, d->new_tbox_sc_req_valid_o);
  sim.compare("sc_tbox_rsp_ready", d->orig_sc_tbox_rsp_ready_o, d->new_sc_tbox_rsp_ready_o);
  sim.compare("tbox_neigh_req_ready", d->orig_tbox_neigh_req_ready_o,
              d->new_tbox_neigh_req_ready_o);
  compare_words(sim, "tbox_neigh_rsp_info", d->orig_tbox_neigh_rsp_info_bits_o,
                d->new_tbox_neigh_rsp_info_bits_o, kRspWords);
  sim.compare("tbox_neigh_rsp_valid", d->orig_tbox_neigh_rsp_valid_o,
              d->new_tbox_neigh_rsp_valid_o);
}

void tick_compare(CosimCtrl<DUT>& sim, WordCov& min_req_cov, WordCov& sc_rsp_cov,
                  WordCov& neigh_req_cov, uint32_t& scalar_ones, uint32_t& scalar_zeros) {
  sample_words(sim.dut->min_tbox_req_info_bits_i, kReqBits, min_req_cov);
  sample_words(sim.dut->sc_tbox_rsp_info_bits_i, kRspBits, sc_rsp_cov);
  sample_words(sim.dut->tbox_neigh_req_info_bits_i, kReqBits, neigh_req_cov);
  uint32_t scalars = 0;
  scalars |= sim.dut->min_tbox_req_valid_early_i << 0;
  scalars |= sim.dut->min_tbox_req_valid_i << 1;
  scalars |= sim.dut->tbox_min_rsp_ready_i << 2;
  scalars |= sim.dut->tbox_sc_req_disable_next_i << 3;
  scalars |= sim.dut->tbox_sc_req_ready_i << 4;
  scalars |= sim.dut->sc_tbox_rsp_valid_i << 5;
  scalars |= sim.dut->tbox_neigh_req_valid_i << 6;
  scalars |= sim.dut->tbox_neigh_rsp_ready_i << 7;
  scalar_ones |= scalars;
  scalar_zeros |= (~scalars) & 0xffu;
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 250000;
  clear_words(sim.dut->min_tbox_req_info_bits_i, kReqWords);
  clear_words(sim.dut->sc_tbox_rsp_info_bits_i, kRspWords);
  clear_words(sim.dut->tbox_neigh_req_info_bits_i, kReqWords);
  sim.dut->min_tbox_req_valid_early_i = 0;
  sim.dut->min_tbox_req_valid_i = 0;
  sim.dut->tbox_min_rsp_ready_i = 1;
  sim.dut->tbox_sc_req_disable_next_i = 0;
  sim.dut->tbox_sc_req_ready_i = 1;
  sim.dut->sc_tbox_rsp_valid_i = 0;
  sim.dut->tbox_neigh_req_valid_i = 0;
  sim.dut->tbox_neigh_rsp_ready_i = 1;
  sim.reset();
  compare_outputs(sim);

  WordCov min_req_cov, sc_rsp_cov, neigh_req_cov;
  uint32_t scalar_ones = 0, scalar_zeros = 0;
  uint32_t seed = 0x74626f78u;
  uint64_t msg_addr = (uint64_t{0x5a} << 3) | (uint64_t{0x9} << 12);

  set_neigh_req(sim.dut->tbox_neigh_req_info_bits_i, 0x22, 4, false, 1,
                0x123456789aull, 0, 3, 0x7, 0x12);
  sim.dut->tbox_neigh_req_valid_i = 1;
  tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  sim.dut->tbox_neigh_req_valid_i = 0;
  for (int i = 0; i < 6; ++i) tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);

  set_neigh_req(sim.dut->tbox_neigh_req_info_bits_i, 0x33, 4, true, 4,
                msg_addr, 0xabcdef, 5, 0xf, 0);
  sim.dut->tbox_neigh_req_valid_i = 1;
  tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  sim.dut->tbox_neigh_req_valid_i = 0;
  for (int i = 0; i < 8; ++i) tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);

  set_neigh_req(sim.dut->min_tbox_req_info_bits_i, 0x44, 3, true, 4,
                msg_addr, 0xdeadbeef, 6, 0xf, 0);
  sim.dut->min_tbox_req_valid_early_i = 1;
  tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  sim.dut->min_tbox_req_valid_early_i = 0;
  sim.dut->min_tbox_req_valid_i = 1;
  tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  sim.dut->min_tbox_req_valid_i = 0;
  for (int i = 0; i < 8; ++i) tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);

  set_rsp(sim.dut->sc_tbox_rsp_info_bits_i, 0x55, 2, true, 1, 0xcafef00d, 6, 0xf);
  sim.dut->sc_tbox_rsp_valid_i = 1;
  tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  sim.dut->sc_tbox_rsp_valid_i = 0;
  for (int i = 0; i < 8; ++i) tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);

  bool pending_min_valid = false;
  for (int cycle = 0; cycle < 5000; ++cycle) {
    random_words(sim.dut->min_tbox_req_info_bits_i, kReqBits, seed);
    random_words(sim.dut->sc_tbox_rsp_info_bits_i, kRspBits, seed);
    random_words(sim.dut->tbox_neigh_req_info_bits_i, kReqBits, seed);
    uint8_t opcode = ((cycle & 3) == 0) ? 4 : static_cast<uint8_t>(cycle & 0x1f);
    uint64_t rand_addr = (uint64_t{xorshift32(seed) & 0xffu} << 32) | xorshift32(seed);
    set_neigh_req(sim.dut->tbox_neigh_req_info_bits_i, cycle & 0xff, (cycle >> 3) & 0xf,
                  (cycle & 1) != 0, opcode, rand_addr, xorshift32(seed),
                  cycle & 7, cycle & 0xf, cycle & 0x3ff);
    set_neigh_req_data_random(sim.dut->tbox_neigh_req_info_bits_i, seed);
    uint64_t rand_min_addr = (uint64_t{xorshift32(seed) & 0xffu} << 32) | (msg_addr ^ xorshift32(seed));
    set_neigh_req(sim.dut->min_tbox_req_info_bits_i, (cycle + 0x40) & 0xff, (cycle >> 5) & 0xf,
                  (cycle & 2) != 0, cycle & 0x1f, rand_min_addr, xorshift32(seed),
                  (cycle >> 1) & 7, (cycle >> 2) & 0xf, cycle & 0x3ff);
    set_neigh_req_data_random(sim.dut->min_tbox_req_info_bits_i, seed);
    set_rsp(sim.dut->sc_tbox_rsp_info_bits_i, (cycle + 0x80) & 0xff, cycle & 0xf,
            true, cycle & 3, (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed),
            cycle & 7, cycle & 0xf);
    set_rsp_data_random(sim.dut->sc_tbox_rsp_info_bits_i, seed);

    sim.dut->min_tbox_req_valid_early_i = !pending_min_valid && ((xorshift32(seed) & 0xf) == 0);
    sim.dut->min_tbox_req_valid_i = pending_min_valid;
    pending_min_valid = sim.dut->min_tbox_req_valid_early_i;
    sim.dut->tbox_neigh_req_valid_i = (xorshift32(seed) & 0x7) == 0;
    sim.dut->sc_tbox_rsp_valid_i = (xorshift32(seed) & 0x7) == 1;
    sim.dut->tbox_min_rsp_ready_i = (xorshift32(seed) & 0x3) != 0;
    sim.dut->tbox_sc_req_ready_i = (xorshift32(seed) & 0x3) != 0;
    sim.dut->tbox_neigh_rsp_ready_i = (xorshift32(seed) & 0x3) != 0;
    sim.dut->tbox_sc_req_disable_next_i = (cycle % 97) == 13;
    tick_compare(sim, min_req_cov, sc_rsp_cov, neigh_req_cov, scalar_ones, scalar_zeros);
  }

  sim.check(full_words(min_req_cov, kReqBits), "coverage: min_tbox_req_info bits toggled 0/1");
  sim.check(full_words(sc_rsp_cov, kRspBits), "coverage: sc_tbox_rsp_info bits toggled 0/1");
  sim.check(full_words(neigh_req_cov, kReqBits), "coverage: tbox_neigh_req_info bits toggled 0/1");
  sim.check((scalar_ones & 0xffu) == 0xffu && (scalar_zeros & 0xffu) == 0xffu,
            "coverage: every scalar input toggled 0/1");
  sim.check(sim.comparisons >= 175000, "comparison count >= 175000");
  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_tensorquant_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_tensorquant_tb;

namespace {

constexpr int kCoreBits = 214;
constexpr int kDcacheDataBits = 256;
constexpr int kCoreTensorquantCtrlPos = 1;
constexpr int kCoreTensorquantStartPos = 65;
constexpr int kCoreTensorstoreStartPos = 130;
constexpr int kCoreTensorfmaStartPos = 213;
constexpr int kCorePendingScoreboardPos = 0;

constexpr uint8_t TQ_LAST = 0;
constexpr uint8_t TQ_I32_TO_F32 = 1;
constexpr uint8_t TQ_F32_TO_I32 = 2;
constexpr uint8_t TQ_RELU = 3;
constexpr uint8_t TQ_ADD_ROW = 4;
constexpr uint8_t TQ_ADD_COL = 5;
constexpr uint8_t TQ_MUL_ROW = 6;
constexpr uint8_t TQ_MUL_COL = 7;
constexpr uint8_t TQ_SAT_I8 = 8;
constexpr uint8_t TQ_SAT_U8 = 9;

struct BitCov {
  std::array<uint32_t, 8> ones{};
  std::array<uint32_t, 8> zeros{};
};

struct Coverage {
  BitCov core;
  BitCov data;
  bool tensorfma_pend_0 = false, tensorfma_pend_1 = false;
  bool tensorstore_pend_0 = false, tensorstore_pend_1 = false;
  bool scp_available_0 = false, scp_available_1 = false;
  bool empty_transform = false;
  bool saw_fcvt = false;
  bool saw_relu = false;
  bool saw_add_row = false;
  bool saw_add_col = false;
  bool saw_mul_row = false;
  bool saw_mul_col = false;
  bool saw_sat_i8 = false;
  bool saw_sat_u8 = false;
};

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void set_bits(WData* words, int pos, int width, uint64_t value) {
  for (int bit = 0; bit < width; ++bit) {
    const int idx = pos + bit;
    const uint32_t mask = uint32_t{1} << (idx & 31);
    if ((value >> bit) & 1ull) words[idx >> 5] |= mask;
    else words[idx >> 5] &= ~mask;
  }
}

void clear_core(DUT* dut) {
  for (int i = 0; i < 7; ++i) dut->core_ctrl_bits_i[i] = 0;
}

void fill_core(DUT* dut, uint32_t value) {
  for (int i = 0; i < 7; ++i) dut->core_ctrl_bits_i[i] = value;
}

void fill_data(DUT* dut, uint32_t value) {
  for (int i = 0; i < 8; ++i) dut->dcache_scp_data_i[i] = value;
}

void clear_inputs(DUT* dut) {
  clear_core(dut);
  fill_data(dut, 0);
  dut->tensorfma_pend_i = 0;
  dut->tensorstore_pend_i = 0;
  dut->dcache_scp_available_i = 0;
}

uint64_t trans_list(std::initializer_list<uint8_t> ops) {
  uint64_t trans = 0;
  int pos = 0;
  for (uint8_t op : ops) {
    trans |= uint64_t(op & 0x0fu) << pos;
    pos += 4;
  }
  return trans;
}

uint64_t pack_quant_ctrl(uint8_t start_reg, uint8_t cols, uint8_t rows,
                         uint8_t scp_src, uint64_t trans) {
  return (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(cols & 0x3u) << 55) |
         (uint64_t(rows & 0x0fu) << 51) |
         (uint64_t(scp_src & 0x3fu) << 45) |
         (trans & ((uint64_t{1} << 40) - 1));
}

void drive_quant_start(DUT* dut, uint64_t ctrl, bool scoreboard = false) {
  clear_core(dut);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantCtrlPos, 64, ctrl);
  set_bits(dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, scoreboard);
}

void idle_core(DUT* dut) {
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorstoreStartPos, 1, 0);
}

void sample_wide(const WData* words, int nbits, BitCov& cov) {
  const int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    cov.ones[i] |= words[i] & mask;
    cov.zeros[i] |= (~words[i]) & mask;
  }
}

bool full_wide(const BitCov& cov, int nbits) {
  const int nwords = (nbits + 31) / 32;
  for (int i = 0; i < nwords; ++i) {
    uint32_t mask = 0xffffffffu;
    if (i == nwords - 1 && (nbits & 31)) mask = (uint32_t{1} << (nbits & 31)) - 1u;
    if ((cov.ones[i] & mask) != mask || (cov.zeros[i] & mask) != mask) return false;
  }
  return true;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  sample_wide(dut->core_ctrl_bits_i, kCoreBits, cov.core);
  sample_wide(dut->dcache_scp_data_i, kDcacheDataBits, cov.data);
  if (dut->tensorfma_pend_i) cov.tensorfma_pend_1 = true; else cov.tensorfma_pend_0 = true;
  if (dut->tensorstore_pend_i) cov.tensorstore_pend_1 = true; else cov.tensorstore_pend_0 = true;
  if (dut->dcache_scp_available_i) cov.scp_available_1 = true; else cov.scp_available_0 = true;
}

void compare_words(CosimCtrl<DUT>& sim, const std::string& name,
                   const WData* orig, const WData* newv, int words) {
  for (int i = 0; i < words; ++i) {
    sim.compare(name + "[" + std::to_string(i) + "]", orig[i], newv[i]);
  }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("enabled", sim.dut->orig_enabled_o, sim.dut->new_enabled_o);
  sim.compare("dcache_scp_req", sim.dut->orig_dcache_scp_req_bits_o, sim.dut->new_dcache_scp_req_bits_o);
  sim.compare("quant_en_next", sim.dut->orig_quant_en_next_o, sim.dut->new_quant_en_next_o);
  sim.compare("quant_inst_next", sim.dut->orig_quant_inst_next_o, sim.dut->new_quant_inst_next_o);
  sim.compare("quant_mask_next", sim.dut->orig_quant_mask_next_o, sim.dut->new_quant_mask_next_o);
  sim.compare("quant_tena_en", sim.dut->orig_quant_tena_en_o, sim.dut->new_quant_tena_en_o);
  sim.compare("ex_ren", sim.dut->orig_ex_ren_o, sim.dut->new_ex_ren_o);
  compare_words(sim, "ex_rdata", sim.dut->orig_ex_rdata_o, sim.dut->new_ex_rdata_o, 8);
  sim.compare("vpu_tensorquant_debug", sim.dut->orig_vpu_tensorquant_debug_o, sim.dut->new_vpu_tensorquant_debug_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage& cov) {
  sample_coverage(sim.dut.get(), cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

void run_directed(CosimCtrl<DUT>& sim, Coverage& cov) {
  drive_quant_start(sim.dut.get(), pack_quant_ctrl(0, 0, 0, 0, trans_list({TQ_LAST})));
  tick_and_compare(sim, cov);
  cov.empty_transform = true;
  idle_core(sim.dut.get());

  drive_quant_start(sim.dut.get(), pack_quant_ctrl(2, 1, 0, 4,
      trans_list({TQ_F32_TO_I32, TQ_RELU, TQ_ADD_ROW, TQ_ADD_COL, TQ_MUL_ROW,
                  TQ_MUL_COL, TQ_SAT_I8, TQ_SAT_U8, TQ_LAST})), true);
  sim.dut->tensorfma_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  tick_and_compare(sim, cov);
  idle_core(sim.dut.get());
  set_bits(sim.dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, 0);
  sim.dut->tensorfma_pend_i = 0;
  sim.dut->tensorstore_pend_i = 0;
  for (int i = 0; i < 220; ++i) {
    sim.dut->dcache_scp_available_i = i > 2;
    for (int w = 0; w < 8; ++w) sim.dut->dcache_scp_data_i[w] = 0x22220000u + uint32_t(i * 8 + w);
    tick_and_compare(sim, cov);
    if (sim.dut->new_quant_en_next_o) {
      const uint32_t inst = sim.dut->new_quant_inst_next_o;
      cov.saw_fcvt |= ((inst >> 20) == 0xc00u) || ((inst >> 20) == 0xd00u);
      cov.saw_relu |= ((inst >> 25) == 0x17u);
      cov.saw_add_row |= ((inst >> 25) == 0x03u) && (((inst >> 12) & 7u) == 0u);
      cov.saw_add_col |= ((inst >> 25) == 0x03u) && (((inst >> 12) & 7u) == 0u);
      cov.saw_mul_row |= ((inst >> 25) == 0x08u);
      cov.saw_mul_col |= ((inst >> 25) == 0x08u);
      cov.saw_sat_i8 |= ((inst >> 20) == 0x060u);
      cov.saw_sat_u8 |= ((inst >> 20) == 0x061u);
    }
  }
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(full_wide(cov.core, kCoreBits), "coverage: every core_vpu_ctrl input bit exercised 0/1");
  sim.check(full_wide(cov.data, kDcacheDataBits), "coverage: every dcache_scp_data input bit exercised 0/1");
  sim.check(cov.tensorfma_pend_0 && cov.tensorfma_pend_1, "coverage: tensorfma_pend exercised 0/1");
  sim.check(cov.tensorstore_pend_0 && cov.tensorstore_pend_1, "coverage: tensorstore_pend exercised 0/1");
  sim.check(cov.scp_available_0 && cov.scp_available_1, "coverage: dcache_scp_available exercised 0/1");
  sim.check(cov.empty_transform, "coverage: empty/TQUANT_LAST transform list directed case ran");
  sim.check(cov.saw_fcvt && cov.saw_relu && cov.saw_add_row && cov.saw_add_col &&
                cov.saw_mul_row && cov.saw_mul_col && cov.saw_sat_i8 && cov.saw_sat_u8,
            "coverage: all directed TensorQuant transform classes emitted instructions");
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 70000;

  Coverage cov;
  clear_inputs(sim.dut.get());
  sim.reset();
  compare_outputs(sim);

  fill_core(sim.dut.get(), 0xffffffffu);
  fill_data(sim.dut.get(), 0xffffffffu);
  sim.dut->tensorfma_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  sim.dut->dcache_scp_available_i = 1;
  tick_and_compare(sim, cov);
  clear_inputs(sim.dut.get());
  tick_and_compare(sim, cov);

  run_directed(sim, cov);

  uint32_t seed = 0x9e3779b9u;
  for (int cycle = 0; cycle < 16000; ++cycle) {
    for (int i = 0; i < 7; ++i) sim.dut->core_ctrl_bits_i[i] = xorshift32(seed);
    for (int i = 0; i < 8; ++i) sim.dut->dcache_scp_data_i[i] = xorshift32(seed);
    sim.dut->tensorfma_pend_i = (xorshift32(seed) & 1u) != 0;
    sim.dut->tensorstore_pend_i = (xorshift32(seed) & 1u) != 0;
    sim.dut->dcache_scp_available_i = (xorshift32(seed) & 1u) != 0;
    tick_and_compare(sim, cov);
  }

  check_coverage(sim, cov);
  sim.check(sim.comparisons >= 250000,
            "vpu_tensorquant cosim should generate at least 250000 comparisons");
  return sim.finish();
}

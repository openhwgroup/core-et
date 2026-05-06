// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_ml_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_ml_tb;

namespace {

constexpr int kCoreBits = 214;
constexpr int kDcacheDataBits = 256;
constexpr int kScpRespBits = 7;
constexpr int kReduceCtrlBits = 3;
constexpr int kCoreTensorquantCtrlPos = 1;
constexpr int kCoreTensorquantStartPos = 65;
constexpr int kCoreReduceCtrlPos = 66;
constexpr int kCoreTensorstoreStartPos = 130;
constexpr int kCoreReduceStartPos = 131;
constexpr int kCoreTensorfmaConvBitsPos = 132;
constexpr int kCoreTensorfmaConvReadyPos = 148;
constexpr int kCoreTensorfmaCtrlPos = 149;
constexpr int kCoreTensorfmaStartPos = 213;

constexpr int kTfmaFirstPassPos = 0;
constexpr int kTfmaModePos = 1;
constexpr int kTfmaScpAPos = 4;
constexpr int kTfmaScpBPos = 12;
constexpr int kTfmaToVrfPos = 23;
constexpr int kTfmaStartAPos = 43;
constexpr int kTfmaColsAPos = 47;
constexpr int kTfmaRowsAPos = 51;
constexpr int kTfmaColsBPos = 55;

constexpr uint8_t TQ_LAST = 0;
constexpr uint8_t TQ_RELU = 3;
constexpr uint8_t TQ_ADD_ROW = 4;
constexpr uint8_t TQ_MUL_COL = 7;

struct BitCov {
  std::array<uint32_t, 8> ones{};
  std::array<uint32_t, 8> zeros{};
};

struct Coverage {
  BitCov core;
  BitCov data;
  uint32_t scp_resp_ones = 0;
  uint32_t scp_resp_zeros = 0;
  uint32_t reduce_ctrl_ones = 0;
  uint32_t reduce_ctrl_zeros = 0;
  uint32_t id_inst_ones = 0;
  uint32_t id_inst_zeros = 0;
  bool directed_fma = false;
  bool directed_quant = false;
  bool directed_reduce = false;
  bool directed_tensorstore = false;
  bool directed_simultaneous = false;
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

void clear_core(DUT* dut) { for (int i = 0; i < 7; ++i) dut->core_ctrl_bits_i[i] = 0; }
void fill_core(DUT* dut, uint32_t value) { for (int i = 0; i < 7; ++i) dut->core_ctrl_bits_i[i] = value; }
void fill_data(DUT* dut, uint32_t value) { for (int i = 0; i < 8; ++i) dut->dcache_scp_data_i[i] = value; }

void clear_inputs(DUT* dut) {
  clear_core(dut);
  fill_data(dut, 0);
  dut->dcache_scp_resp_bits_i = 0;
  dut->dcache_reduce_ctrl_bits_i = 0;
  dut->id_inst_i = 0;
}

uint64_t pack_tfma_ctrl(uint8_t mode, uint8_t rows_a, uint8_t cols_a, uint8_t cols_b,
                        uint8_t start_a, uint8_t scp_a, uint8_t scp_b,
                        bool to_vrf = true, bool first_pass = true) {
  return (uint64_t(first_pass) << kTfmaFirstPassPos) |
         (uint64_t(mode & 0x7u) << kTfmaModePos) |
         (uint64_t(scp_a & 0x3fu) << kTfmaScpAPos) |
         (uint64_t(scp_b & 0x3fu) << kTfmaScpBPos) |
         (uint64_t(to_vrf) << kTfmaToVrfPos) |
         (uint64_t(start_a & 0x0fu) << kTfmaStartAPos) |
         (uint64_t(cols_a & 0x0fu) << kTfmaColsAPos) |
         (uint64_t(rows_a & 0x0fu) << kTfmaRowsAPos) |
         (uint64_t(cols_b & 0x3u) << kTfmaColsBPos);
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

uint64_t pack_reduce_ctrl(uint8_t start_reg, uint8_t num_regs, uint8_t op) {
  return (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(op & 0x0fu) << 24) |
         (uint64_t(num_regs & 0x7fu) << 16);
}

uint64_t pack_tensorstore_ctrl(uint8_t src_inc, uint8_t start_reg, uint8_t cols, uint8_t rows) {
  return (uint64_t(src_inc & 0x3u) << 62) |
         (uint64_t(start_reg & 0x1fu) << 57) |
         (uint64_t(cols & 0x3u) << 55) |
         (uint64_t(rows & 0x0fu) << 51);
}

uint8_t pack_reduce_dcache(bool send_reg, bool exec_op, bool nothing) {
  return (uint8_t(send_reg) << 2) | (uint8_t(exec_op) << 1) | uint8_t(nothing);
}

uint8_t pack_scp_resp(bool early, bool fill, bool flush, uint8_t line) {
  return (uint8_t(early) << 6) | (uint8_t(fill) << 5) |
         (uint8_t(flush) << 4) | (line & 0x0fu);
}

void drive_tfma(DUT* dut) {
  clear_core(dut);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaCtrlPos, 64,
           pack_tfma_ctrl(0, 0, 1, 1, 0, 3, 9));
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaConvReadyPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaConvBitsPos, 16, 0xffffu);
}

void drive_tquant(DUT* dut) {
  clear_core(dut);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantCtrlPos, 64,
           pack_quant_ctrl(4, 0, 0, 6, trans_list({TQ_RELU, TQ_ADD_ROW, TQ_MUL_COL, TQ_LAST})));
}

void drive_reduce(DUT* dut, bool tensorstore = false) {
  clear_core(dut);
  if (tensorstore) {
    set_bits(dut->core_ctrl_bits_i, kCoreTensorstoreStartPos, 1, 1);
    set_bits(dut->core_ctrl_bits_i, kCoreReduceCtrlPos, 64, pack_tensorstore_ctrl(1, 7, 1, 1));
  } else {
    set_bits(dut->core_ctrl_bits_i, kCoreReduceStartPos, 1, 1);
    set_bits(dut->core_ctrl_bits_i, kCoreReduceCtrlPos, 64, pack_reduce_ctrl(5, 3, 0));
  }
}

void idle_starts(DUT* dut) {
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorstoreStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreReduceStartPos, 1, 0);
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
  cov.scp_resp_ones |= dut->dcache_scp_resp_bits_i & ((1u << kScpRespBits) - 1u);
  cov.scp_resp_zeros |= (~dut->dcache_scp_resp_bits_i) & ((1u << kScpRespBits) - 1u);
  cov.reduce_ctrl_ones |= dut->dcache_reduce_ctrl_bits_i & ((1u << kReduceCtrlBits) - 1u);
  cov.reduce_ctrl_zeros |= (~dut->dcache_reduce_ctrl_bits_i) & ((1u << kReduceCtrlBits) - 1u);
  cov.id_inst_ones |= dut->id_inst_i;
  cov.id_inst_zeros |= ~dut->id_inst_i;
}

void compare_words(CosimCtrl<DUT>& sim, const std::string& name,
                   const WData* orig, const WData* newv, int words) {
  for (int i = 0; i < words; ++i) {
    sim.compare(name + "[" + std::to_string(i) + "]", orig[i], newv[i]);
  }
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("tfma_enabled", sim.dut->orig_tfma_enabled_o, sim.dut->new_tfma_enabled_o);
  sim.compare("tfma_wrrf_enabled", sim.dut->orig_tfma_wrrf_enabled_o, sim.dut->new_tfma_wrrf_enabled_o);
  sim.compare("tquant_enabled", sim.dut->orig_tquant_enabled_o, sim.dut->new_tquant_enabled_o);
  sim.compare("tstore_reduce_enabled", sim.dut->orig_tstore_reduce_enabled_o, sim.dut->new_tstore_reduce_enabled_o);
  sim.compare("dcache_ctrl", sim.dut->orig_dcache_ctrl_bits_o, sim.dut->new_dcache_ctrl_bits_o);
  sim.compare("reduce_load_ctrl", sim.dut->orig_reduce_load_ctrl_bits_o, sim.dut->new_reduce_load_ctrl_bits_o);
  sim.compare("tensorfma_load_ctrl", sim.dut->orig_tensorfma_load_ctrl_bits_o, sim.dut->new_tensorfma_load_ctrl_bits_o);
  sim.compare("load_ctrl_pre_tena_wen", sim.dut->orig_load_ctrl_pre_tena_wen_o, sim.dut->new_load_ctrl_pre_tena_wen_o);
  sim.compare("load_ctrl_pre_tenb_wen", sim.dut->orig_load_ctrl_pre_tenb_wen_o, sim.dut->new_load_ctrl_pre_tenb_wen_o);
  sim.compare("id_inst_en_next", sim.dut->orig_id_inst_en_next_o, sim.dut->new_id_inst_en_next_o);
  sim.compare("id_inst_next", sim.dut->orig_id_inst_next_o, sim.dut->new_id_inst_next_o);
  sim.compare("id_mask_next", sim.dut->orig_id_mask_next_o, sim.dut->new_id_mask_next_o);
  sim.compare("id_inst_en", sim.dut->orig_id_inst_en_o, sim.dut->new_id_inst_en_o);
  sim.compare("id_inst_fma_en", sim.dut->orig_id_inst_fma_en_o, sim.dut->new_id_inst_fma_en_o);
  sim.compare("id_inst_tena_quant_en", sim.dut->orig_id_inst_tena_quant_en_o, sim.dut->new_id_inst_tena_quant_en_o);
  sim.compare("f2_tentmp_wen_early", sim.dut->orig_f2_tentmp_wen_early_o, sim.dut->new_f2_tentmp_wen_early_o);
  sim.compare("f3_tentmp_wen", sim.dut->orig_f3_tentmp_wen_o, sim.dut->new_f3_tentmp_wen_o);
  sim.compare("f3_tentmp_waddr", sim.dut->orig_f3_tentmp_waddr_o, sim.dut->new_f3_tentmp_waddr_o);
  sim.compare("ex_tentmp_ren", sim.dut->orig_ex_tentmp_ren_o, sim.dut->new_ex_tentmp_ren_o);
  sim.compare("ex_tentmp_raddr", sim.dut->orig_ex_tentmp_raddr_o, sim.dut->new_ex_tentmp_raddr_o);
  sim.compare("ex_tquant_ren", sim.dut->orig_ex_tquant_ren_o, sim.dut->new_ex_tquant_ren_o);
  compare_words(sim, "ex_tquant_rdata", sim.dut->orig_ex_tquant_rdata_o, sim.dut->new_ex_tquant_rdata_o, 8);
  sim.compare("ex_tima_valid", sim.dut->orig_ex_tima_valid_o, sim.dut->new_ex_tima_valid_o);
  sim.compare("ex_tima_mask", sim.dut->orig_ex_tima_mask_o, sim.dut->new_ex_tima_mask_o);
  sim.compare("ex_tima_tena_raddr", sim.dut->orig_ex_tima_tena_raddr_o, sim.dut->new_ex_tima_tena_raddr_o);
  sim.compare("ex_tima_tenb_raddr", sim.dut->orig_ex_tima_tenb_raddr_o, sim.dut->new_ex_tima_tenb_raddr_o);
  sim.compare("ex_tima_last_pass", sim.dut->orig_ex_tima_last_pass_o, sim.dut->new_ex_tima_last_pass_o);
  sim.compare("ex_tima_ren3", sim.dut->orig_ex_tima_ren3_o, sim.dut->new_ex_tima_ren3_o);
  sim.compare("ex_tima_sa", sim.dut->orig_ex_tima_sa_o, sim.dut->new_ex_tima_sa_o);
  sim.compare("ex_tima_sb", sim.dut->orig_ex_tima_sb_o, sim.dut->new_ex_tima_sb_o);
  sim.compare("ex_tena_rdata_mask", sim.dut->orig_ex_tena_rdata_mask_o, sim.dut->new_ex_tena_rdata_mask_o);
  sim.compare("f2_tima_ren3", sim.dut->orig_f2_tima_ren3_o, sim.dut->new_f2_tima_ren3_o);
  sim.compare("f2_tima_tenc_raddr", sim.dut->orig_f2_tima_tenc_raddr_o, sim.dut->new_f2_tima_tenc_raddr_o);
  sim.compare("f2_tima_tenc_wen", sim.dut->orig_f2_tima_tenc_wen_o, sim.dut->new_f2_tima_tenc_wen_o);
  sim.compare("f2_tima_rf_wen", sim.dut->orig_f2_tima_rf_wen_o, sim.dut->new_f2_tima_rf_wen_o);
  sim.compare("f3_tima_tenc_wen", sim.dut->orig_f3_tima_tenc_wen_o, sim.dut->new_f3_tima_tenc_wen_o);
  sim.compare("f3_tima_tenc_waddr", sim.dut->orig_f3_tima_tenc_waddr_o, sim.dut->new_f3_tima_tenc_waddr_o);
  sim.compare("f3_tima_rf_wen", sim.dut->orig_f3_tima_rf_wen_o, sim.dut->new_f3_tima_rf_wen_o);
  sim.compare("io_events_wait_tenb", sim.dut->orig_io_events_wait_tenb_o, sim.dut->new_io_events_wait_tenb_o);
  sim.compare("vpu_tensorquant_debug", sim.dut->orig_vpu_tensorquant_debug_o, sim.dut->new_vpu_tensorquant_debug_o);
  sim.compare("vpu_tensorreduce_debug", sim.dut->orig_vpu_tensorreduce_debug_o, sim.dut->new_vpu_tensorreduce_debug_o);
  sim.compare("vpu_tensorfma_debug", sim.dut->orig_vpu_tensorfma_debug_o, sim.dut->new_vpu_tensorfma_debug_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage& cov) {
  sample_coverage(sim.dut.get(), cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

void run_directed(CosimCtrl<DUT>& sim, Coverage& cov) {
  drive_tfma(sim.dut.get());
  tick_and_compare(sim, cov);
  cov.directed_fma = true;
  idle_starts(sim.dut.get());
  for (int i = 0; i < 80; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, i > 3, i == 41, i & 0x7);
    sim.dut->id_inst_i = 0x11110000u ^ uint32_t(i * 0x01010101u);
    tick_and_compare(sim, cov);
  }

  drive_tquant(sim.dut.get());
  tick_and_compare(sim, cov);
  cov.directed_quant = true;
  idle_starts(sim.dut.get());
  for (int i = 0; i < 120; ++i) {
    for (int w = 0; w < 8; ++w) sim.dut->dcache_scp_data_i[w] = 0x33330000u + uint32_t(i * 8 + w);
    tick_and_compare(sim, cov);
  }

  drive_reduce(sim.dut.get());
  tick_and_compare(sim, cov);
  cov.directed_reduce = true;
  idle_starts(sim.dut.get());
  for (int i = 0; i < 16; ++i) {
    sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache((i & 1) != 0, (i & 2) != 0, (i & 4) != 0);
    tick_and_compare(sim, cov);
  }

  drive_reduce(sim.dut.get(), true);
  tick_and_compare(sim, cov);
  cov.directed_tensorstore = true;
  idle_starts(sim.dut.get());
  for (int i = 0; i < 16; ++i) {
    sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache(false, (i & 1) != 0, (i & 2) != 0);
    tick_and_compare(sim, cov);
  }

  drive_tfma(sim.dut.get());
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorquantCtrlPos, 64,
           pack_quant_ctrl(1, 0, 0, 2, trans_list({TQ_RELU, TQ_LAST})));
  set_bits(sim.dut->core_ctrl_bits_i, kCoreReduceStartPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreReduceCtrlPos, 64, pack_reduce_ctrl(1, 1, 0));
  tick_and_compare(sim, cov);
  cov.directed_simultaneous = true;
  idle_starts(sim.dut.get());
  for (int i = 0; i < 64; ++i) {
    sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache((i & 1) != 0, (i & 2) != 0, false);
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, true, false, i & 0x7);
    tick_and_compare(sim, cov);
  }
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(full_wide(cov.core, kCoreBits), "coverage: every core_vpu_ctrl input bit exercised 0/1");
  sim.check(full_wide(cov.data, kDcacheDataBits), "coverage: every dcache_scp_data input bit exercised 0/1");
  sim.check(cov.scp_resp_ones == ((1u << kScpRespBits) - 1u) &&
                cov.scp_resp_zeros == ((1u << kScpRespBits) - 1u),
            "coverage: every dcache_scp_resp input bit exercised 0/1");
  sim.check(cov.reduce_ctrl_ones == ((1u << kReduceCtrlBits) - 1u) &&
                cov.reduce_ctrl_zeros == ((1u << kReduceCtrlBits) - 1u),
            "coverage: every dcache_reduce_ctrl input bit exercised 0/1");
  sim.check(cov.id_inst_ones == 0xffffffffu && cov.id_inst_zeros == 0xffffffffu,
            "coverage: every id_inst input bit exercised 0/1");
  sim.check(cov.directed_fma && cov.directed_quant && cov.directed_reduce &&
                cov.directed_tensorstore && cov.directed_simultaneous,
            "coverage: ML directed FMA/Quant/Reduce/TensorStore/simultaneous paths ran");
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 60000;

  Coverage cov;
  clear_inputs(sim.dut.get());
  sim.reset();
  compare_outputs(sim);

  fill_core(sim.dut.get(), 0xffffffffu);
  fill_data(sim.dut.get(), 0xffffffffu);
  sim.dut->dcache_scp_resp_bits_i = 0x7f;
  sim.dut->dcache_reduce_ctrl_bits_i = 0x7;
  sim.dut->id_inst_i = 0xffffffffu;
  tick_and_compare(sim, cov);
  clear_inputs(sim.dut.get());
  tick_and_compare(sim, cov);

  run_directed(sim, cov);

  uint32_t seed = 0x6d2b79f5u;
  for (int cycle = 0; cycle < 12000; ++cycle) {
    for (int i = 0; i < 7; ++i) sim.dut->core_ctrl_bits_i[i] = xorshift32(seed);
    for (int i = 0; i < 8; ++i) sim.dut->dcache_scp_data_i[i] = xorshift32(seed);
    sim.dut->dcache_scp_resp_bits_i = xorshift32(seed) & 0x7fu;
    sim.dut->dcache_reduce_ctrl_bits_i = xorshift32(seed) & 0x7u;
    sim.dut->id_inst_i = xorshift32(seed);
    tick_and_compare(sim, cov);
  }

  check_coverage(sim, cov);
  sim.check(sim.comparisons >= 500000,
            "vpu_ml cosim should generate at least 500000 comparisons");
  return sim.finish();
}

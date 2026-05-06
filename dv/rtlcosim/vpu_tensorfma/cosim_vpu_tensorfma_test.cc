// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_tensorfma_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_tensorfma_tb;

namespace {

constexpr int kCoreBits = 214;
constexpr int kScpRespBits = 7;
constexpr int kCoreTensorquantStartPos = 65;
constexpr int kCoreTensorstoreStartPos = 130;
constexpr int kCoreTensorfmaConvBitsPos = 132;
constexpr int kCoreTensorfmaConvReadyPos = 148;
constexpr int kCoreTensorfmaCtrlPos = 149;
constexpr int kCoreTensorfmaStartPos = 213;
constexpr int kCorePendingScoreboardPos = 0;

constexpr int kTfmaFirstPassPos = 0;
constexpr int kTfmaModePos = 1;
constexpr int kTfmaScpAPos = 4;
constexpr int kTfmaScpBPos = 12;
constexpr int kTfmaTenBPos = 20;
constexpr int kTfmaUaPos = 21;
constexpr int kTfmaUbPos = 22;
constexpr int kTfmaToVrfPos = 23;
constexpr int kTfmaStartAPos = 43;
constexpr int kTfmaColsAPos = 47;
constexpr int kTfmaRowsAPos = 51;
constexpr int kTfmaColsBPos = 55;
constexpr int kTfmaIsConvPos = 63;

struct BitCov {
  std::array<uint32_t, 8> ones{};
  std::array<uint32_t, 8> zeros{};
};

struct Coverage {
  BitCov core;
  uint32_t scp_resp_ones = 0;
  uint32_t scp_resp_zeros = 0;
  uint32_t fma_inst_ones = 0;
  uint32_t fma_inst_zeros = 0;
  bool tensorquant_pend_0 = false, tensorquant_pend_1 = false;
  bool tensorstore_pend_0 = false, tensorstore_pend_1 = false;
  bool directed_fp32 = false;
  bool directed_fp16 = false;
  bool directed_int8 = false;
  bool directed_conv = false;
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

void clear_inputs(DUT* dut) {
  clear_core(dut);
  dut->tensorquant_pend_i = 0;
  dut->tensorstore_pend_i = 0;
  dut->dcache_scp_resp_bits_i = 0;
  dut->fma_inst_i = 0;
}

uint64_t pack_tfma_ctrl(uint8_t mode, uint8_t rows_a, uint8_t cols_a, uint8_t cols_b,
                        uint8_t start_a, uint8_t scp_a, uint8_t scp_b,
                        bool ten_b, bool to_vrf, bool first_pass, bool is_conv,
                        bool u_a = false, bool u_b = false) {
  return (uint64_t(first_pass) << kTfmaFirstPassPos) |
         (uint64_t(mode & 0x7u) << kTfmaModePos) |
         (uint64_t(scp_a & 0x3fu) << kTfmaScpAPos) |
         (uint64_t(scp_b & 0x3fu) << kTfmaScpBPos) |
         (uint64_t(ten_b) << kTfmaTenBPos) |
         (uint64_t(u_a) << kTfmaUaPos) |
         (uint64_t(u_b) << kTfmaUbPos) |
         (uint64_t(to_vrf) << kTfmaToVrfPos) |
         (uint64_t(start_a & 0x0fu) << kTfmaStartAPos) |
         (uint64_t(cols_a & 0x0fu) << kTfmaColsAPos) |
         (uint64_t(rows_a & 0x0fu) << kTfmaRowsAPos) |
         (uint64_t(cols_b & 0x03u) << kTfmaColsBPos) |
         (uint64_t(is_conv) << kTfmaIsConvPos);
}

void drive_tfma_start(DUT* dut, uint64_t ctrl, bool conv_ready = true,
                      uint16_t conv_bits = 0xffffu, bool scoreboard = false) {
  clear_core(dut);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 1);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaCtrlPos, 64, ctrl);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaConvReadyPos, 1, conv_ready);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaConvBitsPos, 16, conv_bits);
  set_bits(dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, scoreboard);
}

void idle_core(DUT* dut) {
  set_bits(dut->core_ctrl_bits_i, kCoreTensorfmaStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 0);
  set_bits(dut->core_ctrl_bits_i, kCoreTensorstoreStartPos, 1, 0);
}

uint8_t pack_scp_resp(bool early, bool fill, bool flush, uint8_t line) {
  return (uint8_t(early) << 6) | (uint8_t(fill) << 5) |
         (uint8_t(flush) << 4) | (line & 0x0fu);
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
  cov.scp_resp_ones |= dut->dcache_scp_resp_bits_i & ((1u << kScpRespBits) - 1u);
  cov.scp_resp_zeros |= (~dut->dcache_scp_resp_bits_i) & ((1u << kScpRespBits) - 1u);
  cov.fma_inst_ones |= dut->fma_inst_i;
  cov.fma_inst_zeros |= ~dut->fma_inst_i;
  if (dut->tensorquant_pend_i) cov.tensorquant_pend_1 = true; else cov.tensorquant_pend_0 = true;
  if (dut->tensorstore_pend_i) cov.tensorstore_pend_1 = true; else cov.tensorstore_pend_0 = true;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("enabled", sim.dut->orig_enabled_o, sim.dut->new_enabled_o);
  sim.compare("dcache_ctrl", sim.dut->orig_dcache_ctrl_bits_o, sim.dut->new_dcache_ctrl_bits_o);
  sim.compare("load_ctrl", sim.dut->orig_load_ctrl_bits_o, sim.dut->new_load_ctrl_bits_o);
  sim.compare("load_ctrl_pre_tena_wen", sim.dut->orig_load_ctrl_pre_tena_wen_o, sim.dut->new_load_ctrl_pre_tena_wen_o);
  sim.compare("load_ctrl_pre_tenb_wen", sim.dut->orig_load_ctrl_pre_tenb_wen_o, sim.dut->new_load_ctrl_pre_tenb_wen_o);
  sim.compare("fma_en_next", sim.dut->orig_fma_en_next_o, sim.dut->new_fma_en_next_o);
  sim.compare("fma_en", sim.dut->orig_fma_en_o, sim.dut->new_fma_en_o);
  sim.compare("fma_inst_next", sim.dut->orig_fma_inst_next_o, sim.dut->new_fma_inst_next_o);
  sim.compare("fma_mask_next", sim.dut->orig_fma_mask_next_o, sim.dut->new_fma_mask_next_o);
  sim.compare("ex_tena_rdata_mask", sim.dut->orig_ex_tena_rdata_mask_o, sim.dut->new_ex_tena_rdata_mask_o);
  sim.compare("pending_vpu_rf_write", sim.dut->orig_pending_vpu_rf_write_o, sim.dut->new_pending_vpu_rf_write_o);
  sim.compare("pending_vpu_tenc_vrf_write", sim.dut->orig_pending_vpu_tenc_vrf_write_o, sim.dut->new_pending_vpu_tenc_vrf_write_o);
  sim.compare("f2_tentmp_wen_early", sim.dut->orig_f2_tentmp_wen_early_o, sim.dut->new_f2_tentmp_wen_early_o);
  sim.compare("f3_tentmp_wen", sim.dut->orig_f3_tentmp_wen_o, sim.dut->new_f3_tentmp_wen_o);
  sim.compare("f3_tentmp_waddr", sim.dut->orig_f3_tentmp_waddr_o, sim.dut->new_f3_tentmp_waddr_o);
  sim.compare("ex_tentmp_ren", sim.dut->orig_ex_tentmp_ren_o, sim.dut->new_ex_tentmp_ren_o);
  sim.compare("ex_tentmp_raddr", sim.dut->orig_ex_tentmp_raddr_o, sim.dut->new_ex_tentmp_raddr_o);
  sim.compare("ex_tima_valid", sim.dut->orig_ex_tima_valid_o, sim.dut->new_ex_tima_valid_o);
  sim.compare("ex_tima_mask", sim.dut->orig_ex_tima_mask_o, sim.dut->new_ex_tima_mask_o);
  sim.compare("ex_tima_tena_raddr", sim.dut->orig_ex_tima_tena_raddr_o, sim.dut->new_ex_tima_tena_raddr_o);
  sim.compare("ex_tima_tenb_raddr", sim.dut->orig_ex_tima_tenb_raddr_o, sim.dut->new_ex_tima_tenb_raddr_o);
  sim.compare("ex_tima_last_pass", sim.dut->orig_ex_tima_last_pass_o, sim.dut->new_ex_tima_last_pass_o);
  sim.compare("ex_tima_ren3", sim.dut->orig_ex_tima_ren3_o, sim.dut->new_ex_tima_ren3_o);
  sim.compare("ex_tima_sa", sim.dut->orig_ex_tima_sa_o, sim.dut->new_ex_tima_sa_o);
  sim.compare("ex_tima_sb", sim.dut->orig_ex_tima_sb_o, sim.dut->new_ex_tima_sb_o);
  sim.compare("f2_tima_ren3", sim.dut->orig_f2_tima_ren3_o, sim.dut->new_f2_tima_ren3_o);
  sim.compare("f2_tima_tenc_raddr", sim.dut->orig_f2_tima_tenc_raddr_o, sim.dut->new_f2_tima_tenc_raddr_o);
  sim.compare("f2_tima_tenc_wen", sim.dut->orig_f2_tima_tenc_wen_o, sim.dut->new_f2_tima_tenc_wen_o);
  sim.compare("f2_tima_rf_wen", sim.dut->orig_f2_tima_rf_wen_o, sim.dut->new_f2_tima_rf_wen_o);
  sim.compare("f3_tima_tenc_wen", sim.dut->orig_f3_tima_tenc_wen_o, sim.dut->new_f3_tima_tenc_wen_o);
  sim.compare("f3_tima_tenc_waddr", sim.dut->orig_f3_tima_tenc_waddr_o, sim.dut->new_f3_tima_tenc_waddr_o);
  sim.compare("f3_tima_rf_wen", sim.dut->orig_f3_tima_rf_wen_o, sim.dut->new_f3_tima_rf_wen_o);
  sim.compare("io_events_wait_tenb", sim.dut->orig_io_events_wait_tenb_o, sim.dut->new_io_events_wait_tenb_o);
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
  const uint64_t fp32 = pack_tfma_ctrl(0, 1, 1, 1, 0, 2, 10, false, true, true, false);
  drive_tfma_start(sim.dut.get(), fp32, true, 0xffffu, true);
  sim.dut->tensorquant_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  tick_and_compare(sim, cov);
  cov.directed_fp32 = true;
  idle_core(sim.dut.get());
  sim.dut->tensorquant_pend_i = 0;
  sim.dut->tensorstore_pend_i = 0;
  set_bits(sim.dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, 0);
  for (int i = 0; i < 80; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp((i & 1) != 0, i > 3, i == 41, i & 0x7);
    sim.dut->fma_inst_i = 0x5a5a0000u ^ uint32_t(i * 0x10203u);
    tick_and_compare(sim, cov);
  }

  const uint64_t fp16 = pack_tfma_ctrl(1, 0, 1, 2, 1, 5, 12, false, true, true, false);
  drive_tfma_start(sim.dut.get(), fp16);
  tick_and_compare(sim, cov);
  cov.directed_fp16 = true;
  idle_core(sim.dut.get());
  for (int i = 0; i < 36; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, true, false, i & 0x7);
    tick_and_compare(sim, cov);
  }

  const uint64_t int8 = pack_tfma_ctrl(3, 1, 1, 3, 0, 7, 18, false, true, true, false, true, false);
  drive_tfma_start(sim.dut.get(), int8);
  tick_and_compare(sim, cov);
  cov.directed_int8 = true;
  idle_core(sim.dut.get());
  for (int i = 0; i < 60; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, true, false, i & 0x7);
    tick_and_compare(sim, cov);
  }

  const uint64_t conv = pack_tfma_ctrl(0, 1, 1, 0, 0, 1, 8, false, true, true, true);
  drive_tfma_start(sim.dut.get(), conv, false, 0x0003u);
  tick_and_compare(sim, cov);
  cov.directed_conv = true;
  idle_core(sim.dut.get());
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorfmaConvReadyPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorfmaConvBitsPos, 16, 0x00ffu);
  for (int i = 0; i < 48; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp((i & 1) != 0, true, false, i & 0x7);
    tick_and_compare(sim, cov);
  }
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(full_wide(cov.core, kCoreBits), "coverage: every core_vpu_ctrl input bit exercised 0/1");
  sim.check(cov.scp_resp_ones == ((1u << kScpRespBits) - 1u) &&
                cov.scp_resp_zeros == ((1u << kScpRespBits) - 1u),
            "coverage: every dcache_scp_resp input bit exercised 0/1");
  sim.check(cov.fma_inst_ones == 0xffffffffu && cov.fma_inst_zeros == 0xffffffffu,
            "coverage: every fma_inst input bit exercised 0/1");
  sim.check(cov.tensorquant_pend_0 && cov.tensorquant_pend_1,
            "coverage: tensorquant_pend exercised 0/1");
  sim.check(cov.tensorstore_pend_0 && cov.tensorstore_pend_1,
            "coverage: tensorstore_pend exercised 0/1");
  sim.check(cov.directed_fp32 && cov.directed_fp16 && cov.directed_int8 && cov.directed_conv,
            "coverage: directed FP32/FP16/INT8/convolution TensorFMA starts ran");
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 50000;

  Coverage cov;
  clear_inputs(sim.dut.get());
  sim.reset();
  compare_outputs(sim);

  fill_core(sim.dut.get(), 0xffffffffu);
  sim.dut->tensorquant_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  sim.dut->dcache_scp_resp_bits_i = 0x7f;
  sim.dut->fma_inst_i = 0xffffffffu;
  tick_and_compare(sim, cov);
  clear_inputs(sim.dut.get());
  tick_and_compare(sim, cov);

  run_directed(sim, cov);

  uint32_t seed = 0x5eed1234u;
  for (int cycle = 0; cycle < 16000; ++cycle) {
    for (int i = 0; i < 7; ++i) sim.dut->core_ctrl_bits_i[i] = xorshift32(seed);
    sim.dut->tensorquant_pend_i = (xorshift32(seed) & 1u) != 0;
    sim.dut->tensorstore_pend_i = (xorshift32(seed) & 1u) != 0;
    sim.dut->dcache_scp_resp_bits_i = xorshift32(seed) & 0x7fu;
    sim.dut->fma_inst_i = xorshift32(seed);
    tick_and_compare(sim, cov);
  }

  check_coverage(sim, cov);
  sim.check(sim.comparisons >= 500000,
            "vpu_tensorfma cosim should generate at least 500000 comparisons");
  return sim.finish();
}

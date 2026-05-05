// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_tensorfma_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_tensorfma_tb;

namespace {

constexpr int kCoreTensorquantCtrlPos = 1;
constexpr int kCoreTensorquantStartPos = 65;
constexpr int kCoreReduceCtrlPos = 66;
constexpr int kCoreTensorstoreStartPos = 130;
constexpr int kCoreReduceStartPos = 131;
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

constexpr uint64_t kDcacheTfmaEnabledBit = uint64_t{1} << 2;
constexpr uint64_t kDcacheScpReqBidBit = uint64_t{1} << 6;
constexpr uint64_t kDcacheScpReqReadEnBit = uint64_t{1} << 21;
constexpr uint32_t kDebugReqInt8Bit = 8;
constexpr uint32_t kDebugConvPendingBit = 6;
constexpr uint32_t kDebugPendStickyBit = 22;

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
  uint64_t ctrl = 0;
  ctrl |= uint64_t(first_pass) << kTfmaFirstPassPos;
  ctrl |= uint64_t(mode & 0x7u) << kTfmaModePos;
  ctrl |= uint64_t(scp_a & 0x3fu) << kTfmaScpAPos;
  ctrl |= uint64_t(scp_b & 0x3fu) << kTfmaScpBPos;
  ctrl |= uint64_t(ten_b) << kTfmaTenBPos;
  ctrl |= uint64_t(u_a) << kTfmaUaPos;
  ctrl |= uint64_t(u_b) << kTfmaUbPos;
  ctrl |= uint64_t(to_vrf) << kTfmaToVrfPos;
  ctrl |= uint64_t(start_a & 0x0fu) << kTfmaStartAPos;
  ctrl |= uint64_t(cols_a & 0x0fu) << kTfmaColsAPos;
  ctrl |= uint64_t(rows_a & 0x0fu) << kTfmaRowsAPos;
  ctrl |= uint64_t(cols_b & 0x03u) << kTfmaColsBPos;
  ctrl |= uint64_t(is_conv) << kTfmaIsConvPos;
  return ctrl;
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
  set_bits(dut->core_ctrl_bits_i, kCoreReduceStartPos, 1, 0);
}

uint8_t pack_scp_resp(bool early, bool fill, bool flush, uint8_t line) {
  return (uint8_t(early) << 6) | (uint8_t(fill) << 5) |
         (uint8_t(flush) << 4) | (line & 0x0fu);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 2000;

  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->enabled_o == 0, "reset leaves TensorFMA disabled");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheScpReqReadEnBit) == 0,
            "reset leaves scratchpad read disabled");
  sim.check(sim.dut->fma_en_next_o == 0, "reset clears next FMA enable");
  sim.check(sim.dut->fma_en_o == 0, "reset clears FMA enable");
  sim.check(sim.dut->pending_vpu_rf_write_o == 0, "reset clears pending RF write");
  sim.check(sim.dut->pending_vpu_tenc_vrf_write_o == 0, "reset clears pending TENC/RF write");
  sim.check(sim.dut->io_events_wait_tenb_o == 0, "reset leaves TENB wait low");

  const uint64_t fp32_ctrl = pack_tfma_ctrl(0, 1, 1, 1, 0, 2, 10, false, true, true, false);
  drive_tfma_start(sim.dut.get(), fp32_ctrl, true, 0xffffu, true);
  sim.dut->tensorquant_pend_i = 1;
  sim.dut->tensorstore_pend_i = 1;
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(sim.dut->enabled_o == 1, "TensorFMA start enters enabled state");
  sim.check((sim.dut->vpu_tensorfma_debug_o >> kDebugPendStickyBit) & 1u,
            "blocked start latches dependency wait");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheTfmaEnabledBit) == 0,
            "dependency wait blocks dcache TensorFMA enable");

  sim.dut->tensorquant_pend_i = 0;
  sim.dut->tensorstore_pend_i = 0;
  set_bits(sim.dut->core_ctrl_bits_i, kCorePendingScoreboardPos, 1, 0);
  sim.tick();
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheTfmaEnabledBit) != 0,
            "clearing hazards enables TensorFMA dcache control");

  bool saw_read = false;
  bool saw_fma = false;
  bool saw_tena_pre = false;
  bool saw_tenb_pre = false;
  bool saw_wait_tenb = false;
  for (int i = 0; i < 48; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp((i & 1) != 0, i > 4, (i == 19), i & 0x7);
    sim.tick();
    saw_read |= (sim.dut->dcache_ctrl_bits_o & kDcacheScpReqReadEnBit) != 0;
    saw_fma |= sim.dut->fma_en_next_o || sim.dut->fma_en_o;
    saw_tena_pre |= sim.dut->load_ctrl_pre_tena_wen_o;
    saw_tenb_pre |= sim.dut->load_ctrl_pre_tenb_wen_o;
    saw_wait_tenb |= sim.dut->io_events_wait_tenb_o;
  }
  sim.check(saw_read, "FP32 TensorFMA issues scratchpad reads");
  sim.check(saw_fma, "FP32 TensorFMA emits FMA enables after scratchpad fills");
  sim.check(saw_tena_pre, "FP32 TensorFMA marks TENA load-control writes");
  sim.check(saw_tenb_pre, "FP32 TensorFMA marks TENB load-control writes");
  sim.check(saw_wait_tenb, "FP32 TensorFMA reports TENB wait while B data is missing");

  clear_inputs(sim.dut.get());
  sim.reset();
  const uint64_t fp16_ctrl = pack_tfma_ctrl(1, 0, 1, 2, 1, 5, 12, false, true, true, false);
  drive_tfma_start(sim.dut.get(), fp16_ctrl);
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(sim.dut->enabled_o == 1, "FP16/packed TensorFMA request is accepted");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheScpReqReadEnBit) != 0,
            "FP16/packed TensorFMA starts scratchpad sequencing");

  clear_inputs(sim.dut.get());
  sim.reset();
  const uint64_t int8_ctrl = pack_tfma_ctrl(3, 1, 1, 3, 0, 7, 18, false, true, true, false, true, false);
  drive_tfma_start(sim.dut.get(), int8_ctrl);
  sim.tick();
  idle_core(sim.dut.get());
  sim.check(sim.dut->enabled_o == 1, "INT8 TensorFMA request is accepted");
  sim.check((sim.dut->vpu_tensorfma_debug_o >> kDebugReqInt8Bit) & 1u,
            "INT8 TensorFMA sets the debug INT8 mode bit");
  bool saw_tima = false;
  bool saw_tentmp = false;
  for (int i = 0; i < 40; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, true, false, i & 0x7);
    sim.tick();
    saw_tima |= sim.dut->ex_tima_valid_o;
    saw_tentmp |= sim.dut->f2_tentmp_wen_early_o || sim.dut->f3_tentmp_wen_o;
  }
  sim.check(saw_tima, "INT8 TensorFMA drives TIMA valid");
  sim.check(saw_tentmp, "INT8 TensorFMA exercises tensor-temp write path");

  clear_inputs(sim.dut.get());
  sim.reset();
  const uint64_t conv_ctrl = pack_tfma_ctrl(0, 1, 1, 0, 0, 1, 8, false, true, true, true);
  drive_tfma_start(sim.dut.get(), conv_ctrl, false, 0x0003u);
  sim.tick();
  idle_core(sim.dut.get());
  sim.check((sim.dut->vpu_tensorfma_debug_o >> kDebugConvPendingBit) & 1u,
            "convolution request waits for delayed mask bits");
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorfmaConvReadyPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorfmaConvBitsPos, 16, 0x00ffu);
  sim.tick();
  sim.check(((sim.dut->vpu_tensorfma_debug_o >> kDebugConvPendingBit) & 1u) == 0,
            "convolution mask-ready input clears pending state");

  return sim.finish();
}

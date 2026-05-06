// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_ml_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_ml_tb;

namespace {

constexpr int kCorePendingScoreboardPos = 0;
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

constexpr uint32_t kOpcodeMask = 0x7f;
constexpr uint32_t kVpuOpcode = 0x7b;
constexpr uint64_t kDcacheTfmaEnabledBit = uint64_t{1} << 2;
constexpr uint64_t kDcacheReduceWaitBit = uint64_t{1} << 1;
constexpr uint64_t kDcacheScpReqReadEnBit = uint64_t{1} << 21;
constexpr uint32_t kLoadCtrlWenBit = 9;

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

void clear_data(DUT* dut) {
  for (int i = 0; i < 8; ++i) dut->dcache_scp_data_i[i] = 0;
}

void clear_inputs(DUT* dut) {
  clear_core(dut);
  clear_data(dut);
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

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 2500;

  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->tfma_enabled_o == 0, "reset leaves TensorFMA child disabled");
  sim.check(sim.dut->tquant_enabled_o == 0, "reset leaves TensorQuant child disabled");
  sim.check(sim.dut->tstore_reduce_enabled_o == 0, "reset leaves reduce/store child disabled");
  sim.check(sim.dut->id_inst_en_next_o == 0, "reset clears injected next instruction enable");
  sim.check(sim.dut->id_inst_en_o == 0, "reset clears registered injected instruction enable");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheScpReqReadEnBit) == 0,
            "reset leaves ML dcache scratchpad request idle");

  drive_tfma(sim.dut.get());
  sim.tick();
  idle_starts(sim.dut.get());
  sim.check(sim.dut->tfma_enabled_o == 1, "isolated TensorFMA start enables FMA child");
  sim.check(sim.dut->tfma_wrrf_enabled_o == 1, "TensorFMA start marks RF write pending");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheTfmaEnabledBit) != 0,
            "ML forwards TensorFMA dcache enable");
  bool saw_fma_inst = false;
  bool saw_tfma_load = false;
  for (int i = 0; i < 48; ++i) {
    sim.dut->dcache_scp_resp_bits_i = pack_scp_resp(false, i > 3, false, i & 0x7);
    sim.tick();
    saw_fma_inst |= sim.dut->id_inst_fma_en_o || sim.dut->id_inst_en_next_o;
    saw_tfma_load |= ((sim.dut->tensorfma_load_ctrl_bits_o >> kLoadCtrlWenBit) & 1u) != 0;
  }
  sim.check(saw_fma_inst, "TensorFMA path contributes injected/FMA instruction enable");
  sim.check(saw_tfma_load, "TensorFMA path drives tensorfma_load_ctrl");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_tquant(sim.dut.get());
  sim.tick();
  idle_starts(sim.dut.get());
  sim.check(sim.dut->tquant_enabled_o == 1, "isolated TensorQuant start enables quant child");
  bool saw_quant_inst = false;
  bool saw_quant_tena = false;
  for (int i = 0; i < 80; ++i) {
    for (int w = 0; w < 8; ++w) sim.dut->dcache_scp_data_i[w] = 0x01010000u + uint32_t(i * 8 + w);
    sim.tick();
    saw_quant_inst |= sim.dut->id_inst_en_next_o && !sim.dut->id_inst_fma_en_o;
    saw_quant_tena |= sim.dut->id_inst_tena_quant_en_o || sim.dut->ex_tquant_ren_o;
  }
  sim.check(saw_quant_inst, "TensorQuant path contributes injected instruction enable");
  sim.check(saw_quant_tena, "TensorQuant path drives quant TENA/read controls");
  sim.check((sim.dut->id_inst_next_o & kOpcodeMask) == kVpuOpcode || !sim.dut->id_inst_en_next_o,
            "TensorQuant injected instruction uses the VPU opcode when active");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_reduce(sim.dut.get());
  sim.tick();
  idle_starts(sim.dut.get());
  sim.check(sim.dut->tstore_reduce_enabled_o == 1, "reduce start enables reduce child");
  sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache(true, false, false);
  sim.dut->eval();
  sim.check(sim.dut->id_inst_en_next_o == 1, "reduce send_reg wins the injected instruction mux");
  sim.tick();
  sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache(false, true, false);
  bool saw_reduce_load = false;
  bool saw_reduce_inst_reg = false;
  for (int i = 0; i < 5; ++i) {
    sim.tick();
    saw_reduce_load |= ((sim.dut->reduce_load_ctrl_bits_o >> kLoadCtrlWenBit) & 1u) != 0;
    saw_reduce_inst_reg |= sim.dut->id_inst_en_o || sim.dut->id_inst_en_next_o;
  }
  sim.check(saw_reduce_load, "reduce path drives reduce_load_ctrl write enable on exec_op");
  sim.check(saw_reduce_inst_reg, "registered/next instruction enable captures reduce issue");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_reduce(sim.dut.get(), true);
  sim.tick();
  idle_starts(sim.dut.get());
  sim.check(sim.dut->tstore_reduce_enabled_o == 1, "tensorstore start uses reduce/store child");
  sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache(false, false, true);
  sim.tick();
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheReduceWaitBit) == 0,
            "reduce_wait mux is idle after dcache nothing response");

  clear_inputs(sim.dut.get());
  sim.reset();
  drive_tfma(sim.dut.get());
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorquantStartPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreTensorquantCtrlPos, 64,
           pack_quant_ctrl(1, 0, 0, 2, trans_list({TQ_RELU, TQ_LAST})));
  set_bits(sim.dut->core_ctrl_bits_i, kCoreReduceStartPos, 1, 1);
  set_bits(sim.dut->core_ctrl_bits_i, kCoreReduceCtrlPos, 64, pack_reduce_ctrl(1, 1, 0));
  sim.tick();
  idle_starts(sim.dut.get());
  sim.check(sim.dut->tfma_enabled_o == 1, "simultaneous start keeps TensorFMA active");
  sim.check(sim.dut->tquant_enabled_o == 1, "simultaneous start keeps TensorQuant active");
  sim.check(sim.dut->tstore_reduce_enabled_o == 1, "simultaneous start keeps reduce active");
  sim.dut->dcache_reduce_ctrl_bits_i = pack_reduce_dcache(true, false, false);
  sim.dut->eval();
  sim.check(sim.dut->id_inst_en_next_o == 1, "ML arbitration emits an instruction during simultaneous activity");
  sim.check((sim.dut->dcache_ctrl_bits_o & kDcacheTfmaEnabledBit) != 0,
            "ML dcache mux keeps TensorFMA control visible during simultaneous activity");

  return sim.finish();
}

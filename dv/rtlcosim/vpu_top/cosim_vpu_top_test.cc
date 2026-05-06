// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>
#include <vector>

using DUT = Vcosim_vpu_top_tb;

namespace {

constexpr uint8_t kScenarioIdle        = 0;
constexpr uint8_t kScenarioAllOnes     = 1;
constexpr uint8_t kScenarioShSw        = 2;
constexpr uint8_t kScenarioTxfma       = 3;
constexpr uint8_t kScenarioRomTrans    = 4;
constexpr uint8_t kScenarioTima        = 5;
constexpr uint8_t kScenarioTensorQuant = 6;
constexpr uint8_t kScenarioReduce      = 7;
constexpr uint8_t kScenarioDcache      = 8;
constexpr uint8_t kScenarioKill             = 9;
constexpr uint8_t kScenarioChicken          = 10;
constexpr uint8_t kScenarioThread           = 11;
constexpr uint8_t kScenarioCmdSub           = 12;
constexpr uint8_t kScenarioCompareMinMax    = 13;
constexpr uint8_t kScenarioConvert          = 14;
constexpr uint8_t kScenarioMoveFromInt      = 15;
constexpr uint8_t kScenarioMoveToInt        = 16;
constexpr uint8_t kScenarioLogicMask        = 17;
constexpr uint8_t kScenarioShiftImm         = 18;
constexpr uint8_t kScenarioDtypeF16         = 19;
constexpr uint8_t kScenarioDtypeF11         = 20;
constexpr uint8_t kScenarioDtypeF10         = 21;
constexpr uint8_t kScenarioDtypeUn8         = 22;
constexpr uint8_t kScenarioDtypeSn8         = 23;
constexpr uint8_t kScenarioRoundRne         = 24;
constexpr uint8_t kScenarioRoundRtz         = 25;
constexpr uint8_t kScenarioRoundRdn         = 26;
constexpr uint8_t kScenarioRoundRup         = 27;
constexpr uint8_t kScenarioRoundRmm         = 28;
constexpr uint8_t kScenarioRoundDyn         = 29;
constexpr uint8_t kScenarioHazardWrite      = 30;
constexpr uint8_t kScenarioHazardRead       = 31;
constexpr uint8_t kScenarioInterleaveT0     = 32;
constexpr uint8_t kScenarioInterleaveT1     = 33;
constexpr uint8_t kScenarioKillDcacheTensor = 34;
constexpr uint8_t kScenarioDcacheSideband   = 35;
constexpr uint8_t kScenarioTensorStore      = 36;
constexpr uint8_t kScenarioDebugBanks       = 37;
constexpr uint8_t kScenarioRandom           = 0xff;

constexpr int kProtocolShortAlu        = 0;
constexpr int kProtocolAddSub          = 1;
constexpr int kProtocolMulTima         = 2;
constexpr int kProtocolTxfma           = 3;
constexpr int kProtocolCompareMinMax   = 4;
constexpr int kProtocolConvert         = 5;
constexpr int kProtocolMove            = 6;
constexpr int kProtocolLogicMask       = 7;
constexpr int kProtocolShiftImm        = 8;
constexpr int kProtocolRomTrans        = 9;
constexpr int kProtocolDtypeInt        = 10;
constexpr int kProtocolDtypeF32        = 11;
constexpr int kProtocolDtypeF16        = 12;
constexpr int kProtocolDtypeF11        = 13;
constexpr int kProtocolDtypeF10        = 14;
constexpr int kProtocolDtypeUn8        = 15;
constexpr int kProtocolDtypeSn8        = 16;
constexpr int kProtocolRoundRne        = 17;
constexpr int kProtocolRoundRtz        = 18;
constexpr int kProtocolRoundRdn        = 19;
constexpr int kProtocolRoundRup        = 20;
constexpr int kProtocolRoundRmm        = 21;
constexpr int kProtocolRoundDyn        = 22;
constexpr int kProtocolThread0         = 23;
constexpr int kProtocolThread1         = 24;
constexpr int kProtocolDestSrcOverlap  = 25;
constexpr int kProtocolBackToBackHaz   = 26;
constexpr int kProtocolInterleave      = 27;
constexpr int kProtocolKillEx          = 28;
constexpr int kProtocolKillF2          = 29;
constexpr int kProtocolKillF3          = 30;
constexpr int kProtocolKillF4          = 31;
constexpr int kProtocolDcacheWb        = 32;
constexpr int kProtocolDcacheScpTenb   = 33;
constexpr int kProtocolReduceSend      = 34;
constexpr int kProtocolReduceExec      = 35;
constexpr int kProtocolReduceNothing   = 36;
constexpr int kProtocolMemOverride     = 37;
constexpr int kProtocolDbgMatch        = 38;
constexpr int kProtocolDbgFilter       = 39;
constexpr int kProtocolDbgData0        = 40;
constexpr int kProtocolDbgData1        = 41;
constexpr int kProtocolDbgData2        = 42;
constexpr int kProtocolDbgData3        = 43;
constexpr int kProtocolDbgData4        = 44;
constexpr int kProtocolChickenLaneDis  = 45;
constexpr int kProtocolChickenTimaDis  = 46;
constexpr int kProtocolChickenTransDis = 47;
constexpr int kProtocolTensorFma       = 48;
constexpr int kProtocolTensorReduce    = 49;
constexpr int kProtocolTensorQuant     = 50;
constexpr int kProtocolTensorStore     = 51;
constexpr int kProtocolRealTxfma       = 52;

constexpr uint64_t bit(int index) { return 1ull << index; }

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

template <typename Wide>
void fill_wide(Wide& words, uint32_t value) {
  for (size_t i = 0; i < words.size(); ++i) words[i] = value;
}

template <typename Wide>
void randomize_wide(Wide& words, uint32_t& seed) {
  for (size_t i = 0; i < words.size(); ++i) words[i] = xorshift32(seed);
}

template <typename Wide>
void sample_wide(const Wide& words, std::vector<uint32_t>& ones, std::vector<uint32_t>& zeros) {
  if (ones.empty()) {
    ones.assign(words.size(), 0);
    zeros.assign(words.size(), 0);
  }
  for (size_t i = 0; i < words.size(); ++i) {
    ones[i] |= words[i];
    zeros[i] |= ~words[i];
  }
}

bool full_coverage(const std::vector<uint32_t>& ones, const std::vector<uint32_t>& zeros) {
  if (ones.empty() || ones.size() != zeros.size()) return false;
  for (size_t i = 0; i < ones.size(); ++i) {
    if (ones[i] != 0xffffffffu || zeros[i] != 0xffffffffu) return false;
  }
  return true;
}

struct ProtocolCoverage {
  uint64_t observed = 0;
  bool pending_hazard_write = false;
  bool saw_back_to_back_hazard = false;
  bool saw_alternating_threads = false;
  int last_interleave_thread = -1;

  void sample(uint64_t protocol, uint8_t scenario) {
    observed |= protocol;

    if (scenario == kScenarioHazardWrite && (protocol & bit(kProtocolAddSub))) {
      pending_hazard_write = true;
    } else if (scenario == kScenarioHazardRead) {
      if (pending_hazard_write && (protocol & bit(kProtocolBackToBackHaz))) {
        saw_back_to_back_hazard = true;
      }
    } else {
      pending_hazard_write = false;
    }

    int thread = -1;
    if (scenario == kScenarioInterleaveT0 && (protocol & bit(kProtocolInterleave))) thread = 0;
    if (scenario == kScenarioInterleaveT1 && (protocol & bit(kProtocolInterleave))) thread = 1;
    if (thread >= 0) {
      if (last_interleave_thread >= 0 && last_interleave_thread != thread) {
        saw_alternating_threads = true;
      }
      last_interleave_thread = thread;
    } else if (scenario != kScenarioInterleaveT0 && scenario != kScenarioInterleaveT1) {
      last_interleave_thread = -1;
    }
  }

  bool has(int bit_index) const { return (observed & bit(bit_index)) != 0; }
};

void compare_outputs(CosimCtrl<DUT>& sim) {
  for (size_t i = 0; i < sim.dut->orig_outputs_o.size(); ++i) {
    sim.compare("outputs[" + std::to_string(i) + "]",
                sim.dut->orig_outputs_o[i], sim.dut->new_outputs_o[i]);
  }
  sim.compare("protocol", sim.dut->orig_protocol_o, sim.dut->new_protocol_o);
}

void tick_compare_sample(CosimCtrl<DUT>& sim,
                         std::vector<uint32_t>& input_ones,
                         std::vector<uint32_t>& input_zeros,
                         ProtocolCoverage* protocol_cov) {
  sim.tick();
  sample_wide(sim.dut->applied_inputs_o, input_ones, input_zeros);
  compare_outputs(sim);
  if (protocol_cov != nullptr) {
    protocol_cov->sample(sim.dut->new_protocol_o, sim.dut->scenario_i);
  }
}

void run_scenario(CosimCtrl<DUT>& sim,
                  uint8_t scenario,
                  int cycles,
                  std::vector<uint32_t>& input_ones,
                  std::vector<uint32_t>& input_zeros,
                  ProtocolCoverage* protocol_cov) {
  sim.dut->scenario_i = scenario;
  for (int i = 0; i < cycles; ++i) {
    tick_compare_sample(sim, input_ones, input_zeros, protocol_cov);
  }
}

void check_protocol_coverage(CosimCtrl<DUT>& sim, const ProtocolCoverage& cov) {
  sim.check(cov.has(kProtocolShortAlu), "protocol coverage: short/ALU command observed with short-path control");
  sim.check(cov.has(kProtocolAddSub), "protocol coverage: add/sub command family observed");
  sim.check(cov.has(kProtocolMulTima), "protocol coverage: multiply/TIMA command family observed");
  sim.check(cov.has(kProtocolTxfma), "protocol coverage: TXFMA/FMA command family observed");
  sim.check(cov.has(kProtocolCompareMinMax), "protocol coverage: compare/min/max command family observed");
  sim.check(cov.has(kProtocolConvert), "protocol coverage: conversion command family observed");
  sim.check(cov.has(kProtocolMove), "protocol coverage: move/from-int/to-int command family observed");
  sim.check(cov.has(kProtocolLogicMask), "protocol coverage: logical/mask command family observed");
  sim.check(cov.has(kProtocolShiftImm), "protocol coverage: shift/immediate command family observed");
  sim.check(cov.has(kProtocolRomTrans), "protocol coverage: ROM/transcendental command family observed");

  sim.check(cov.has(kProtocolDtypeInt), "protocol coverage: integer dtype observed");
  sim.check(cov.has(kProtocolDtypeF32), "protocol coverage: F32 dtype observed");
  sim.check(cov.has(kProtocolDtypeF16), "protocol coverage: F16 dtype observed");
  sim.check(cov.has(kProtocolDtypeF11), "protocol coverage: F11 dtype observed");
  sim.check(cov.has(kProtocolDtypeF10), "protocol coverage: F10 dtype observed");
  sim.check(cov.has(kProtocolDtypeUn8), "protocol coverage: unsigned narrow dtype observed");
  sim.check(cov.has(kProtocolDtypeSn8), "protocol coverage: signed narrow dtype observed");

  sim.check(cov.has(kProtocolRoundRne), "protocol coverage: RNE rounding observed");
  sim.check(cov.has(kProtocolRoundRtz), "protocol coverage: RTZ rounding observed");
  sim.check(cov.has(kProtocolRoundRdn), "protocol coverage: RDN rounding observed");
  sim.check(cov.has(kProtocolRoundRup), "protocol coverage: RUP rounding observed");
  sim.check(cov.has(kProtocolRoundRmm), "protocol coverage: RMM rounding observed");
  sim.check(cov.has(kProtocolRoundDyn), "protocol coverage: dynamic/CSR rounding observed");

  sim.check(cov.has(kProtocolThread0), "protocol coverage: thread 0 transaction observed");
  sim.check(cov.has(kProtocolThread1), "protocol coverage: thread 1 transaction observed");
  sim.check(cov.has(kProtocolDestSrcOverlap), "protocol coverage: same destination/source overlap observed");
  sim.check(cov.saw_back_to_back_hazard, "protocol coverage: back-to-back write/read hazard-like sequence observed");
  sim.check(cov.saw_alternating_threads, "protocol coverage: alternating-thread sequence observed");

  sim.check(cov.has(kProtocolKillEx), "protocol coverage: EX kill observed through debug output");
  sim.check(cov.has(kProtocolKillF2), "protocol coverage: F2 kill observed through debug output");
  sim.check(cov.has(kProtocolKillF3), "protocol coverage: F3 kill observed through debug output");
  sim.check(cov.has(kProtocolKillF4), "protocol coverage: F4 kill observed through debug output");

  sim.check(cov.has(kProtocolDcacheWb), "protocol coverage: dcache writeback response sideband observed");
  sim.check(cov.has(kProtocolDcacheScpTenb), "protocol coverage: SCP/TENB fill or credit sideband observed");
  sim.check(cov.has(kProtocolReduceSend), "protocol coverage: reduce send sideband observed");
  sim.check(cov.has(kProtocolReduceExec), "protocol coverage: reduce exec sideband observed");
  sim.check(cov.has(kProtocolReduceNothing), "protocol coverage: reduce nothing sideband observed");
  sim.check(cov.has(kProtocolMemOverride), "protocol coverage: memory-control override sideband observed");

  sim.check(cov.has(kProtocolDbgMatch), "protocol coverage: nonzero debug match observed");
  sim.check(cov.has(kProtocolDbgFilter), "protocol coverage: nonzero debug filter observed");
  sim.check(cov.has(kProtocolDbgData0), "protocol coverage: debug data bank 0 observed");
  sim.check(cov.has(kProtocolDbgData1), "protocol coverage: debug data bank 1 observed");
  sim.check(cov.has(kProtocolDbgData2), "protocol coverage: debug data bank 2 observed");
  sim.check(cov.has(kProtocolDbgData3), "protocol coverage: debug data bank 3 observed");
  sim.check(cov.has(kProtocolDbgData4), "protocol coverage: debug data bank 4 observed");

  sim.check(cov.has(kProtocolChickenLaneDis), "protocol coverage: VPU-lane chicken disable observed");
  sim.check(cov.has(kProtocolChickenTimaDis), "protocol coverage: VPU-TIMA chicken disable observed");
  sim.check(cov.has(kProtocolChickenTransDis), "protocol coverage: VPU-trans chicken disable observed");

  sim.check(cov.has(kProtocolTensorFma), "protocol coverage: tensor-FMA start/datapath observed");
  sim.check(cov.has(kProtocolTensorReduce), "protocol coverage: tensor-reduce start/datapath observed");
  sim.check(cov.has(kProtocolTensorQuant), "protocol coverage: tensor-quant start/datapath observed");
  sim.check(cov.has(kProtocolTensorStore), "protocol coverage: tensor-store start/datapath observed");
  sim.check(cov.has(kProtocolRealTxfma), "protocol coverage: real TXFMA top mode observed");
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 350000;

  std::vector<uint32_t> input_ones;
  std::vector<uint32_t> input_zeros;
  ProtocolCoverage protocol_cov;

  fill_wide(sim.dut->random_inputs_i, 0);
  sim.dut->scenario_i = kScenarioIdle;
  sim.reset();
  sample_wide(sim.dut->applied_inputs_o, input_ones, input_zeros);
  compare_outputs(sim);

  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioAllOnes, 2, input_ones, input_zeros, nullptr);
  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros, &protocol_cov);

  // Directed semantic phases before random activity. The SV wrapper maps these
  // scenario IDs onto the real top-level input structs so the original and new
  // tops see identical no-op, sh/sw, TXFMA, ROM/trans, TIMA/tensor-FMA,
  // tensor-quant, reduce/store, dcache, kill/stall, chicken-bit, and mixed
  // thread-ID transactions.
  run_scenario(sim, kScenarioShSw, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 12, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioTxfma, 32, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRomTrans, 32, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioTima, 40, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioTensorQuant, 40, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioReduce, 40, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDcache, 32, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioKill, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioChicken, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioThread, 32, input_ones, input_zeros, &protocol_cov);

  run_scenario(sim, kScenarioCmdSub, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioCompareMinMax, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioConvert, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioMoveFromInt, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioMoveToInt, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioLogicMask, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioShiftImm, 20, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDtypeF16, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDtypeF11, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDtypeF10, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDtypeUn8, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDtypeSn8, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundRne, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundRtz, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundRdn, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundRup, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundRmm, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioRoundDyn, 16, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioHazardWrite, 8, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioHazardRead, 16, input_ones, input_zeros, &protocol_cov);
  for (int rep = 0; rep < 4; ++rep) {
    run_scenario(sim, kScenarioInterleaveT0, 6, input_ones, input_zeros, &protocol_cov);
    run_scenario(sim, kScenarioInterleaveT1, 6, input_ones, input_zeros, &protocol_cov);
  }
  run_scenario(sim, kScenarioKillDcacheTensor, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDcacheSideband, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioTensorStore, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioDebugBanks, 24, input_ones, input_zeros, &protocol_cov);
  run_scenario(sim, kScenarioIdle, 32, input_ones, input_zeros, &protocol_cov);

  uint32_t seed = 0x7055a117u;
  sim.dut->scenario_i = kScenarioRandom;
  for (int cycle = 0; cycle < 100000; ++cycle) {
    randomize_wide(sim.dut->random_inputs_i, seed);
    tick_compare_sample(sim, input_ones, input_zeros, nullptr);
  }

  run_scenario(sim, kScenarioIdle, 32, input_ones, input_zeros, &protocol_cov);

  sim.check(full_coverage(input_ones, input_zeros),
            "vpu_top applied input bus drives every bit to both 0 and 1");
  check_protocol_coverage(sim, protocol_cov);

  return sim.finish();
}

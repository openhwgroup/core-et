// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_trans_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>
#include <vector>

using DUT = Vvpu_trans_tb;

namespace {

constexpr uint32_t kOpcodePs = 0x7b;
constexpr uint32_t kOpcodeR  = 0x33;
constexpr uint32_t kRne      = 0x0;
constexpr int      kScoreboardWords = 3;  // Verilator lowers the 96-bit packed scoreboard bus to three 32-bit words.

constexpr uint32_t kFrcpPsFunct12     = 0x587;
constexpr uint32_t kFrsqrtPsFunct12   = 0x588;
constexpr uint32_t kFlogPsFunct12     = 0x583;
constexpr uint32_t kFsinPsFunct12     = 0x586;
constexpr uint32_t kFexpPsFunct12     = 0x584;
constexpr uint32_t kFrcpFixRastFunct7 = 0x18;

constexpr uint32_t kTrFma1RcpF7    = 0x00;
constexpr uint32_t kTrFma2RcpF7    = 0x01;
constexpr uint32_t kTrFma1RsqrtF7  = 0x02;
constexpr uint32_t kTrFma2RsqrtF7  = 0x03;
constexpr uint32_t kTrNrF7         = 0x04;
constexpr uint32_t kTrFma1LogF7    = 0x07;
constexpr uint32_t kTrFma2LogF7    = 0x08;
constexpr uint32_t kTrMulLogF7     = 0x09;
constexpr uint32_t kTrTrSinF7      = 0x0a;
constexpr uint32_t kTrRrSinF7      = 0x0b;
constexpr uint32_t kTrP1SinF7      = 0x0c;
constexpr uint32_t kTrP2SinF7      = 0x0d;
constexpr uint32_t kTrP3SinF7      = 0x0e;
constexpr uint32_t kTrRrExpF7      = 0x0f;
constexpr uint32_t kTrFma1ExpF7    = 0x10;
constexpr uint32_t kTrFma2ExpF7    = 0x11;

uint32_t make_ps_inst(uint32_t funct12, uint32_t rd, uint32_t rs1) {
    return ((funct12 & 0xfffu) << 20) | ((rs1 & 0x1fu) << 15) | (kRne << 12) |
           ((rd & 0x1fu) << 7) | kOpcodePs;
}

uint32_t make_fix_rast_inst(uint32_t rd, uint32_t rs2) {
    return ((kFrcpFixRastFunct7 & 0x7fu) << 25) | ((rs2 & 0x1fu) << 20) |
           (kRne << 12) | ((rd & 0x1fu) << 7) | kOpcodePs;
}

uint32_t make_tr_inst(uint32_t funct7, uint32_t rd, uint32_t rs1, uint32_t rs2) {
    return ((funct7 & 0x7fu) << 25) | ((rs2 & 0x1fu) << 20) | ((rs1 & 0x1fu) << 15) |
           (kRne << 12) | ((rd & 0x1fu) << 7) | kOpcodeR;
}

void clear_inputs(DUT* dut) {
    dut->id_valid_i = 0;
    dut->id_core_thread_id_i = 0;
    dut->id_instruction_i = 0;
    dut->ex_valid_i = 0;
    dut->ex_core_thread_id_i = 0;
    dut->ex_instruction_i = 0;
    dut->ex_mask_rf0_i = 0;
    dut->chicken_bit_vputrans_i = 0;
    dut->f6_killed_i = 0;
    dut->f7_killed_i = 0;
    dut->f8_killed_i = 0;
}

bool scoreboard_is_zero(const DUT* dut) {
    for (int i = 0; i < kScoreboardWords; ++i) {
        if (dut->id_trans_scoreboard_packed_o[i] != 0) {
            return false;
        }
    }
    return true;
}

bool scoreboard_has_activity(const DUT* dut) {
    return !scoreboard_is_zero(dut);
}

struct SequenceResult {
    std::vector<uint32_t> inserts;
    bool busy_seen = false;
    bool scoreboard_seen = false;
    bool f6_ctrl_seen = false;
    bool f8_ctrl_seen = false;
    bool f8_fcsr_seen = false;
    bool ex_thread_seen = false;
};

SequenceResult run_sequence(SimCtrl<DUT>& sim, uint32_t start_inst, uint8_t thread_id,
                            uint8_t mask, int cycles) {
    clear_inputs(sim.dut.get());
    sim.dut->ex_valid_i = 1;
    sim.dut->ex_core_thread_id_i = thread_id;
    sim.dut->ex_instruction_i = start_inst;
    sim.dut->ex_mask_rf0_i = mask;
    sim.dut->eval();

    SequenceResult result;
    result.busy_seen |= sim.dut->id_trans_busy_o;
    sim.tick();
    clear_inputs(sim.dut.get());

    for (int i = 0; i < cycles; ++i) {
      sim.dut->eval();
      if (sim.dut->id_insert_en_next_o) {
          result.inserts.push_back(sim.dut->id_insert_inst_next_o);
      }
      result.busy_seen |= sim.dut->id_trans_busy_o;
      result.scoreboard_seen |= scoreboard_has_activity(sim.dut.get());
      result.f6_ctrl_seen |= sim.dut->f6_trans_ctrl_en_o;
      result.f8_ctrl_seen |= sim.dut->f8_trans_ctrl_en_o;
      result.f8_fcsr_seen |= sim.dut->f8_trans_fcsr_en_o;
      result.ex_thread_seen |= sim.dut->ex_trans_thread_id_o == thread_id;
      sim.tick();
    }

    return result;
}

void check_insert_sequence(SimCtrl<DUT>& sim, const SequenceResult& result,
                           const std::vector<uint32_t>& expected,
                           const std::string& name) {
    sim.check(result.inserts.size() == expected.size(),
              name + ": unexpected number of inserted micro-ops");
    const size_t count = result.inserts.size() < expected.size() ? result.inserts.size() : expected.size();
    for (size_t i = 0; i < count; ++i) {
        sim.check(result.inserts[i] == expected[i],
                  name + ": inserted micro-op mismatch at slot " + std::to_string(i));
    }
    sim.check(result.busy_seen, name + ": trans busy should assert");
    sim.check(result.scoreboard_seen, name + ": scoreboard should become non-zero");
    sim.check(result.f6_ctrl_seen, name + ": f6 control pulse should be observed");
    sim.check(result.f8_ctrl_seen, name + ": f8 control pulse should be observed");
    sim.check(result.ex_thread_seen, name + ": ex thread id should be observed");
    sim.check(sim.dut->id_trans_busy_o == 0, name + ": sequencer should drain back to idle");
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    clear_inputs(sim.dut.get());
    sim.reset();
    clear_inputs(sim.dut.get());
    sim.tick();

    const bool enable_extra_trans = sim.dut->enable_extra_trans_o;

    sim.check(sim.dut->id_insert_en_next_o == 0, "reset: no next insert");
    sim.check(sim.dut->id_insert_en_o == 0, "reset: no current insert");
    sim.check(sim.dut->id_trans_busy_o == 0, "reset: not busy");
    sim.check(scoreboard_is_zero(sim.dut.get()), "reset: scoreboard cleared");
    sim.check(sim.dut->state_packed_o == 0, "reset: state cleared");

    {
        const uint32_t start = make_ps_inst(kFrcpPsFunct12, 9, 4);
        const auto result = run_sequence(sim, start, 1, 0xa5, 24);
        check_insert_sequence(sim, result,
                              {make_tr_inst(kTrFma1RcpF7, 9, 0, 4),
                               make_tr_inst(kTrFma2RcpF7, 9, 0, 4)},
                              "FRCP_PS");
        sim.check(result.f8_fcsr_seen, "FRCP_PS: dirty-state FCSR writeback should assert when mask is non-zero");
    }

    {
        const uint32_t start = make_ps_inst(kFlogPsFunct12, 7, 3);
        const auto result = run_sequence(sim, start, 0, 0x3c, 28);
        check_insert_sequence(sim, result,
                              {make_tr_inst(kTrFma1LogF7, 7, 0, 3),
                               make_tr_inst(kTrFma2LogF7, 7, 0, 3),
                               make_tr_inst(kTrMulLogF7, 7, 0, 3)},
                              "FLOG_PS");
        sim.check(result.f8_fcsr_seen, "FLOG_PS: dirty-state FCSR writeback should assert when mask is non-zero");
    }

    {
        const uint32_t start = make_ps_inst(kFexpPsFunct12, 11, 6);
        const auto result = run_sequence(sim, start, 1, 0xf0, 28);
        check_insert_sequence(sim, result,
                              {make_tr_inst(kTrRrExpF7, 11, 0, 11),
                               make_tr_inst(kTrFma1ExpF7, 11, 0, 11),
                               make_tr_inst(kTrFma2ExpF7, 11, 0, 11)},
                              "FEXP_PS");
        sim.check(result.f8_fcsr_seen, "FEXP_PS: dirty-state FCSR writeback should assert");
    }

    {
        const uint32_t start = make_fix_rast_inst(13, 18);
        const auto result = run_sequence(sim, start, 0, 0x5a, 20);
        check_insert_sequence(sim, result,
                              {make_tr_inst(kTrNrF7, 13, 0, 18)},
                              "FRCP_FIX_RAST");
    }

    {
        const uint32_t start = make_ps_inst(kFrsqrtPsFunct12, 5, 2);
        const auto result = run_sequence(sim, start, 1, 0x96, 24);
        if (enable_extra_trans) {
            check_insert_sequence(sim, result,
                                  {make_tr_inst(kTrFma1RsqrtF7, 5, 0, 2),
                                   make_tr_inst(kTrFma2RsqrtF7, 5, 0, 2)},
                                  "FRSQ_PS");
        } else {
            sim.check(result.inserts.empty(), "FRSQ_PS disabled: no micro-ops should be inserted");
            sim.check(!result.busy_seen, "FRSQ_PS disabled: busy should stay low");
        }
    }

    {
        const uint32_t start = make_ps_inst(kFsinPsFunct12, 10, 8);
        const auto result = run_sequence(sim, start, 0, 0xcc, 36);
        if (enable_extra_trans) {
            check_insert_sequence(sim, result,
                                  {make_tr_inst(kTrTrSinF7, 10, 0, 0),
                                   make_tr_inst(kTrRrSinF7, 10, 0, 10),
                                   make_tr_inst(kTrP1SinF7, 10, 10, 10),
                                   make_tr_inst(kTrP2SinF7, 10, 0, 10),
                                   make_tr_inst(kTrP3SinF7, 10, 0, 10)},
                                  "FSIN_PS");
        } else {
            sim.check(result.inserts.empty(), "FSIN_PS disabled: no micro-ops should be inserted");
            sim.check(!result.busy_seen, "FSIN_PS disabled: busy should stay low");
        }
    }

    return sim.finish();
}

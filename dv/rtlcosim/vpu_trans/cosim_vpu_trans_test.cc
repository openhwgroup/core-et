// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_trans_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_trans_tb;

namespace {

constexpr uint32_t kOpcodePs = 0x7b;
constexpr uint32_t kRne      = 0x0;

constexpr uint32_t kFrcpPsFunct12     = 0x587;
constexpr uint32_t kFrsqrtPsFunct12   = 0x588;
constexpr uint32_t kFlogPsFunct12     = 0x583;
constexpr uint32_t kFsinPsFunct12     = 0x586;
constexpr uint32_t kFexpPsFunct12     = 0x584;
constexpr uint32_t kFrcpFixRastFunct7 = 0x18;

struct BoolCoverage {
    bool seen0 = false;
    bool seen1 = false;

    void observe(bool value) {
        if (value) seen1 = true;
        else       seen0 = true;
    }

    bool full() const { return seen0 && seen1; }
};

struct WordCoverage {
    uint32_t or_mask = 0;
    uint32_t and_mask = 0xffffffffu;

    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

struct StimulusCoverage {
    BoolCoverage id_valid;
    BoolCoverage id_thread;
    WordCoverage id_instruction;
    BoolCoverage ex_valid;
    BoolCoverage ex_thread;
    WordCoverage ex_instruction;
    WordCoverage ex_mask_rf0;
    BoolCoverage chicken_bit_vputrans;
    BoolCoverage f6_killed;
    BoolCoverage f7_killed;
    BoolCoverage f8_killed;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint32_t make_ps_inst(uint32_t funct12, uint32_t rd, uint32_t rs1) {
    return ((funct12 & 0xfffu) << 20) | ((rs1 & 0x1fu) << 15) | (kRne << 12) |
           ((rd & 0x1fu) << 7) | kOpcodePs;
}

uint32_t make_fix_rast_inst(uint32_t rd, uint32_t rs2) {
    return ((kFrcpFixRastFunct7 & 0x7fu) << 25) | ((rs2 & 0x1fu) << 20) |
           (kRne << 12) | ((rd & 0x1fu) << 7) | kOpcodePs;
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

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("id_insert_en_next", sim.dut->orig_id_insert_en_next_o, sim.dut->new_id_insert_en_next_o);
    sim.compare("id_insert_inst_next", sim.dut->orig_id_insert_inst_next_o, sim.dut->new_id_insert_inst_next_o);
    sim.compare("id_insert_en", sim.dut->orig_id_insert_en_o, sim.dut->new_id_insert_en_o);
    sim.compare("id_trans_thread_id", sim.dut->orig_id_trans_thread_id_o, sim.dut->new_id_trans_thread_id_o);
    sim.compare("ex_trans_thread_id", sim.dut->orig_ex_trans_thread_id_o, sim.dut->new_ex_trans_thread_id_o);
    sim.compare("f6_trans_ctrl_en", sim.dut->orig_f6_trans_ctrl_en_o, sim.dut->new_f6_trans_ctrl_en_o);
    sim.compare("id_trans_scoreboard_0", sim.dut->orig_id_trans_scoreboard_0_o, sim.dut->new_id_trans_scoreboard_0_o);
    sim.compare("id_trans_scoreboard_1", sim.dut->orig_id_trans_scoreboard_1_o, sim.dut->new_id_trans_scoreboard_1_o);
    sim.compare("id_trans_scoreboard_2", sim.dut->orig_id_trans_scoreboard_2_o, sim.dut->new_id_trans_scoreboard_2_o);
    sim.compare("id_trans_scoreboard_3", sim.dut->orig_id_trans_scoreboard_3_o, sim.dut->new_id_trans_scoreboard_3_o);
    sim.compare("id_trans_scoreboard_4", sim.dut->orig_id_trans_scoreboard_4_o, sim.dut->new_id_trans_scoreboard_4_o);
    sim.compare("id_trans_scoreboard_5", sim.dut->orig_id_trans_scoreboard_5_o, sim.dut->new_id_trans_scoreboard_5_o);
    sim.compare("f6_trans_wmask", sim.dut->orig_f6_trans_wmask_o, sim.dut->new_f6_trans_wmask_o);
    sim.compare("f6_trans_thread_id", sim.dut->orig_f6_trans_thread_id_o, sim.dut->new_f6_trans_thread_id_o);
    sim.compare("f8_trans_ctrl_en", sim.dut->orig_f8_trans_ctrl_en_o, sim.dut->new_f8_trans_ctrl_en_o);
    sim.compare("f8_trans_fcsr_en", sim.dut->orig_f8_trans_fcsr_en_o, sim.dut->new_f8_trans_fcsr_en_o);
    sim.compare("id_trans_busy", sim.dut->orig_id_trans_busy_o, sim.dut->new_id_trans_busy_o);
    sim.compare("state_packed", sim.dut->orig_state_packed_o, sim.dut->new_state_packed_o);
}

void observe_inputs(StimulusCoverage& coverage, const DUT* dut) {
    coverage.id_valid.observe(dut->id_valid_i);
    coverage.id_thread.observe(dut->id_core_thread_id_i);
    coverage.id_instruction.observe(dut->id_instruction_i);
    coverage.ex_valid.observe(dut->ex_valid_i);
    coverage.ex_thread.observe(dut->ex_core_thread_id_i);
    coverage.ex_instruction.observe(dut->ex_instruction_i);
    coverage.ex_mask_rf0.observe(dut->ex_mask_rf0_i);
    coverage.chicken_bit_vputrans.observe(dut->chicken_bit_vputrans_i);
    coverage.f6_killed.observe(dut->f6_killed_i);
    coverage.f7_killed.observe(dut->f7_killed_i);
    coverage.f8_killed.observe(dut->f8_killed_i);
}

void drive_cycle(CosimCtrl<DUT>& sim, StimulusCoverage& coverage,
                 bool id_valid, bool id_thread, uint32_t id_instruction,
                 bool ex_valid, bool ex_thread, uint32_t ex_instruction,
                 uint8_t ex_mask_rf0, bool chicken_bit_vputrans,
                 bool f6_killed, bool f7_killed, bool f8_killed) {
    sim.dut->id_valid_i = id_valid;
    sim.dut->id_core_thread_id_i = id_thread;
    sim.dut->id_instruction_i = id_instruction;
    sim.dut->ex_valid_i = ex_valid;
    sim.dut->ex_core_thread_id_i = ex_thread;
    sim.dut->ex_instruction_i = ex_instruction;
    sim.dut->ex_mask_rf0_i = ex_mask_rf0;
    sim.dut->chicken_bit_vputrans_i = chicken_bit_vputrans;
    sim.dut->f6_killed_i = f6_killed;
    sim.dut->f7_killed_i = f7_killed;
    sim.dut->f8_killed_i = f8_killed;
    observe_inputs(coverage, sim.dut.get());
    sim.tick();
    compare_outputs(sim);
}

uint32_t random_start_instruction(uint32_t& seed, bool allow_extra_trans) {
    switch (xorshift32(seed) % 8u) {
      case 0: return make_ps_inst(kFrcpPsFunct12, xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu);
      case 1: return allow_extra_trans
                    ? make_ps_inst(kFrsqrtPsFunct12, xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu)
                    : xorshift32(seed);
      case 2: return make_ps_inst(kFlogPsFunct12, xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu);
      case 3: return allow_extra_trans
                    ? make_ps_inst(kFsinPsFunct12, xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu)
                    : xorshift32(seed);
      case 4: return make_ps_inst(kFexpPsFunct12, xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu);
      case 5: return make_fix_rast_inst(xorshift32(seed) & 0x1fu, xorshift32(seed) & 0x1fu);
      case 6: return xorshift32(seed);
      default: return 0u;
    }
}

void check_input_coverage(CosimCtrl<DUT>& sim, const StimulusCoverage& coverage) {
    sim.check(coverage.id_valid.full(), "id_valid_i should be driven to both 0 and 1");
    sim.check(coverage.id_thread.full(), "id_core_thread_id_i should be driven to both 0 and 1");
    sim.check(coverage.id_instruction.or_mask == 0xffffffffu, "id_instruction_i should drive every bit high at least once");
    sim.check(coverage.id_instruction.and_mask == 0x00000000u, "id_instruction_i should drive every bit low at least once");
    sim.check(coverage.ex_valid.full(), "ex_valid_i should be driven to both 0 and 1");
    sim.check(coverage.ex_thread.full(), "ex_core_thread_id_i should be driven to both 0 and 1");
    sim.check(coverage.ex_instruction.or_mask == 0xffffffffu, "ex_instruction_i should drive every bit high at least once");
    sim.check(coverage.ex_instruction.and_mask == 0x00000000u, "ex_instruction_i should drive every bit low at least once");
    sim.check((coverage.ex_mask_rf0.or_mask & 0xffu) == 0xffu, "ex_mask_rf0_i should drive every lane bit high");
    sim.check((coverage.ex_mask_rf0.and_mask & 0xffu) == 0x00u, "ex_mask_rf0_i should drive every lane bit low");
    sim.check(coverage.chicken_bit_vputrans.full(), "chicken_bit_vputrans_i should be driven to both 0 and 1");
    sim.check(coverage.f6_killed.full(), "f6_killed_i should be driven to both 0 and 1");
    sim.check(coverage.f7_killed.full(), "f7_killed_i should be driven to both 0 and 1");
    sim.check(coverage.f8_killed.full(), "f8_killed_i should be driven to both 0 and 1");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    StimulusCoverage coverage;

    clear_inputs(sim.dut.get());
    sim.reset();
    clear_inputs(sim.dut.get());
    sim.tick();
    compare_outputs(sim);

    const std::array<uint32_t, 6> directed_ops = {{
        make_ps_inst(kFrcpPsFunct12, 9, 4),
        make_ps_inst(kFrsqrtPsFunct12, 5, 2),
        make_ps_inst(kFlogPsFunct12, 7, 3),
        make_ps_inst(kFsinPsFunct12, 10, 8),
        make_ps_inst(kFexpPsFunct12, 11, 6),
        make_fix_rast_inst(13, 18),
    }};

    for (uint32_t op : directed_ops) {
        drive_cycle(sim, coverage, true, false, op, true, true, op, 0xa5u, false, false, false, false);
        for (int i = 0; i < 8; ++i) {
            drive_cycle(sim, coverage, (i & 1) != 0, (i & 2) != 0, directed_ops[(i + 1) % directed_ops.size()],
                        false, false, 0u, 0u, (i & 4) != 0, false, false, false);
        }
    }

    uint32_t seed = 0x2d7f4ab3u;
    for (int i = 0; i < 50000; ++i) {
        const bool id_valid = (xorshift32(seed) & 0x7u) != 0u;
        const bool ex_valid = (xorshift32(seed) & 0x3u) != 0u;
        const uint32_t id_instruction =
            ((xorshift32(seed) & 0x3u) == 0u) ? random_start_instruction(seed, true) : xorshift32(seed);
        const uint32_t ex_instruction =
            ((xorshift32(seed) & 0x1u) == 0u) ? random_start_instruction(seed, true) : xorshift32(seed);

        drive_cycle(sim, coverage,
                    id_valid,
                    (xorshift32(seed) & 0x1u) != 0u,
                    id_instruction,
                    ex_valid,
                    (xorshift32(seed) & 0x1u) != 0u,
                    ex_instruction,
                    static_cast<uint8_t>(xorshift32(seed)),
                    (xorshift32(seed) & 0x1u) != 0u,
                    (xorshift32(seed) & 0x1u) != 0u,
                    (xorshift32(seed) & 0x1u) != 0u,
                    (xorshift32(seed) & 0x1u) != 0u);
    }

    check_input_coverage(sim, coverage);
    sim.check(sim.comparisons >= 500000, "vpu_trans cosim should generate at least 500000 comparisons");

    return sim.finish();
}

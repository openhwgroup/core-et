// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vdebug_breakpoint_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vdebug_breakpoint_tb;

namespace {

constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kDbgBpLoad = 0;
constexpr uint8_t kDbgBpStore = 1;
constexpr uint8_t kDbgBpTensorLoad = 2;
constexpr uint8_t kDbgBpTensorStore = 3;

uint16_t ctrl(bool action, bool match, bool m, bool s, bool u,
              bool execute, bool store, bool load) {
  return (static_cast<uint16_t>(action) << 7) |
         (static_cast<uint16_t>(match) << 6) |
         (static_cast<uint16_t>(m) << 5) |
         (static_cast<uint16_t>(s) << 4) |
         (static_cast<uint16_t>(u) << 3) |
         (static_cast<uint16_t>(execute) << 2) |
         (static_cast<uint16_t>(store) << 1) |
         static_cast<uint16_t>(load);
}

void clear_inputs(DUT* d) {
  d->control0_i = 0;
  d->control1_i = 0;
  d->in_debug_i = 0;
  d->prv0_i = kPrvM;
  d->prv1_i = kPrvM;
  d->tdata2_0_i = 0;
  d->tdata2_1_i = 0;
  d->address_i = 0;
  d->cmd_i = kDbgBpLoad;
  d->valid_i = 0;
  d->thread_id_i = 0;
}

void check_all(SimCtrl<DUT>& sim, const char* name,
               bool insn_timing, bool insn_xcpt, bool insn_enter, bool insn_update,
               bool data_timing, bool data_xcpt, bool data_enter, bool data_update) {
  sim.dut->eval();
  char msg[192];
#define CHECK_FIELD(field, exp) \
  do { \
    std::snprintf(msg, sizeof(msg), "%s %s got=%u exp=%u", name, #field, sim.dut->field, static_cast<unsigned>(exp)); \
    sim.check(sim.dut->field == (exp), msg); \
  } while (0)
  CHECK_FIELD(insn_timing_o, insn_timing);
  CHECK_FIELD(insn_xcpt_o, insn_xcpt);
  CHECK_FIELD(insn_enter_debug_o, insn_enter);
  CHECK_FIELD(insn_update_o, insn_update);
  CHECK_FIELD(data_timing_o, data_timing);
  CHECK_FIELD(data_xcpt_o, data_xcpt);
  CHECK_FIELD(data_enter_debug_o, data_enter);
  CHECK_FIELD(data_update_o, data_update);
#undef CHECK_FIELD
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset(2);

  clear_inputs(sim.dut.get());
  check_all(sim, "reset_disabled", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, true, false, false);
  sim.dut->tdata2_0_i = 0x000012345000ull;
  sim.dut->address_i = 0x000012345000ull;
  sim.dut->valid_i = 1;
  check_all(sim, "insn_execute_exception", false, true, false, true, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(true, false, true, false, false, true, false, false);
  sim.dut->tdata2_0_i = 0x000012345000ull;
  sim.dut->address_i = 0x000012345000ull;
  sim.dut->valid_i = 1;
  check_all(sim, "insn_enter_debug", false, false, true, true, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, false, false, false);
  sim.dut->tdata2_0_i = 0x1000;
  sim.dut->address_i = 0x1000;
  sim.dut->valid_i = 1;
  check_all(sim, "execute_disabled", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, true, false, false);
  sim.dut->in_debug_i = 1;
  sim.dut->tdata2_0_i = 0x1000;
  sim.dut->address_i = 0x1000;
  sim.dut->valid_i = 1;
  check_all(sim, "debug_mode_blocks_trigger", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, true, false, false);
  sim.dut->prv0_i = kPrvS;
  sim.dut->tdata2_0_i = 0x1000;
  sim.dut->address_i = 0x1000;
  sim.dut->valid_i = 1;
  check_all(sim, "privilege_gate_blocks_s_mode", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, true, true, false, false, true, false, false);
  sim.dut->tdata2_0_i = 0x000000001207ull;
  sim.dut->address_i = 0x00000000120aull;
  sim.dut->valid_i = 1;
  check_all(sim, "insn_mask_match", false, true, false, true, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, true, false, false);
  sim.dut->tdata2_0_i = 0x000000001207ull;
  sim.dut->address_i = 0x00000000120aull;
  sim.dut->valid_i = 1;
  check_all(sim, "insn_exact_mismatch", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, false, false, true);
  sim.dut->tdata2_0_i = 0x4440;
  sim.dut->address_i = 0x4440;
  sim.dut->cmd_i = kDbgBpLoad;
  sim.dut->valid_i = 1;
  check_all(sim, "data_load_exception", false, false, false, false, false, true, false, true);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(true, false, true, false, false, false, true, false);
  sim.dut->tdata2_0_i = 0x5550;
  sim.dut->address_i = 0x5550;
  sim.dut->cmd_i = kDbgBpStore;
  sim.dut->valid_i = 1;
  check_all(sim, "data_store_enter_debug", false, false, false, false, false, false, true, true);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, false, false, true);
  sim.dut->tdata2_0_i = 0x6660;
  sim.dut->address_i = 0x6660;
  sim.dut->cmd_i = kDbgBpTensorLoad;
  sim.dut->valid_i = 1;
  check_all(sim, "data_tensor_load_timing_after", false, false, false, false, true, true, false, true);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, false, false, true);
  sim.dut->tdata2_0_i = 0x7770;
  sim.dut->address_i = 0x7770;
  sim.dut->cmd_i = kDbgBpStore;
  sim.dut->valid_i = 1;
  check_all(sim, "data_command_gate_blocks_store", false, false, false, false, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control1_i = ctrl(false, false, false, true, false, true, false, false);
  sim.dut->prv1_i = kPrvS;
  sim.dut->tdata2_1_i = 0x8880;
  sim.dut->address_i = 0x8880;
  sim.dut->valid_i = 1;
  sim.dut->thread_id_i = 1;
  check_all(sim, "thread1_config_selected", false, true, false, true, false, false, false, false);

  clear_inputs(sim.dut.get());
  sim.dut->control0_i = ctrl(false, false, true, false, false, false, true, false);
  sim.dut->tdata2_0_i = 0x9990;
  sim.dut->address_i = 0x9990;
  sim.dut->cmd_i = kDbgBpTensorStore;
  sim.dut->valid_i = 1;
  check_all(sim, "data_tensor_store_timing_after", false, false, false, false, true, true, false, true);

  return sim.finish();
}

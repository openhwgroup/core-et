// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_top_tb;

namespace {

constexpr uint32_t kEventTimaOps    = 1u << 1;
constexpr uint32_t kEventTxfma32    = 1u << 3;
constexpr uint32_t kEventTransOps   = 1u << 5;
constexpr uint32_t kEventShortOps   = 1u << 6;
constexpr uint32_t kEventTfmaInst   = 1u << 8;
constexpr uint32_t kEventReduceInst = 1u << 9;
constexpr uint32_t kEventTquantInst = 1u << 10;

void drive_idle(DUT* dut) {
  dut->id_core_valid_i = 0;
  dut->id_core_thread_id_i = 0;
  dut->ren_mask_i = 0;
  dut->shsw_i = 0;
  dut->txfma_i = 0;
  dut->rom_i = 0;
  dut->trans_i = 0;
  dut->fromint_i = 0;
  dut->maskop_i = 0;
  dut->wen_i = 0;
  dut->swap12_i = 0;
  dut->tensorfma_start_i = 0;
  dut->tensorquant_start_i = 0;
  dut->reduce_start_i = 0;
  dut->tensorstore_start_i = 0;
  dut->wb_dcache_resp_valid_i = 0;
  dut->dcache_scp_fill_tenb_i = 0;
  dut->dcache_reduce_send_reg_i = 0;
  dut->dcache_reduce_exec_op_i = 0;
  dut->ex_core_kill_i = 0;
  dut->f2_core_kill_i = 0;
  dut->f3_core_kill_i = 0;
  dut->f4_core_kill_i = 0;
  dut->chicken_vpulane_i = 1;
  dut->chicken_vputima_i = 1;
  dut->chicken_vputrans_i = 1;
  dut->ra1_i = 0;
  dut->ra2_i = 0;
  dut->ra3_i = 0;
  dut->rd_i = 0;
  dut->fromint_data_i = 0;
}

void tick_and_accum(SimCtrl<DUT>& sim, uint32_t& events, uint32_t& dbg_match, uint32_t& dbg_filter) {
  sim.tick();
  events |= sim.dut->io_events_o;
  dbg_match |= sim.dut->vpu_dbg_match_o;
  dbg_filter |= sim.dut->vpu_dbg_filter_o[0];
}

void drive_common_decode(DUT* dut) {
  dut->id_core_valid_i = 1;
  dut->id_core_thread_id_i = 1;
  dut->ren_mask_i = 0x7;
  dut->shsw_i = 1;
  dut->txfma_i = 0;
  dut->rom_i = 0;
  dut->trans_i = 0;
  dut->fromint_i = 1;
  dut->maskop_i = 0;
  dut->wen_i = 1;
  dut->swap12_i = 0;
  dut->ra1_i = 3;
  dut->ra2_i = 11;
  dut->ra3_i = 19;
  dut->rd_i = 5;
  dut->fromint_data_i = 0x12345678ULL;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 5000;

  drive_idle(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->id_core_ctrl_tfma_enabled_o == 0, "reset/idle leaves tensor-FMA enable clear");
  sim.check(sim.dut->id_core_ctrl_tquant_enabled_o == 0, "reset/idle leaves tensor-quant enable clear");
  sim.check(sim.dut->id_core_ctrl_reduce_enabled_o == 0, "reset/idle leaves reduce enable clear");
  sim.check(sim.dut->dcache_ctrl_read_en_o == 0, "reset/idle leaves VPU dcache SCP read clear");
  sim.check(sim.dut->dcache_ctrl_tenb_credit_o == 0, "reset/idle leaves TENB credit clear");
  sim.check(sim.dut->io_events_o == 0, "reset/idle leaves VPU event vector clear");
  sim.check(sim.dut->vpu_dbg_match_o == 0, "reset/idle leaves debug match vector clear");

  uint32_t seen_events = 0;
  uint32_t seen_dbg_match = 0;
  uint32_t seen_dbg_filter = 0;

  drive_common_decode(sim.dut.get());
  bool saw_short_valid = false;
  bool saw_short_clock_valid = false;
  for (int i = 0; i < 12; ++i) {
    tick_and_accum(sim, seen_events, seen_dbg_match, seen_dbg_filter);
    saw_short_valid |= sim.dut->ex_sh_sw_valid_o;
    saw_short_clock_valid |= sim.dut->ex_sh_sw_clock_valid_o;
  }
  (void)saw_short_valid;
  sim.check(saw_short_clock_valid, "ordinary decoded sh/sw instruction requests the short-path clock");

  seen_events = 0;
  seen_dbg_match = 0;
  seen_dbg_filter = 0;
  sim.dut->shsw_i = 0;
  sim.dut->txfma_i = 1;
  sim.dut->rom_i = 1;
  sim.dut->trans_i = 1;
  sim.dut->fromint_i = 0;
  bool saw_txfma_valid = false;
  bool saw_txfma_clock_valid = false;
  bool saw_rom_valid = false;
  bool saw_rom_clock_valid = false;
  for (int i = 0; i < 14; ++i) {
    tick_and_accum(sim, seen_events, seen_dbg_match, seen_dbg_filter);
    saw_txfma_valid |= sim.dut->ex_txfma_valid_any_o;
    saw_txfma_clock_valid |= sim.dut->ex_txfma_clock_valid_o;
    saw_rom_valid |= sim.dut->ex_rom_valid_o;
    saw_rom_clock_valid |= sim.dut->ex_rom_clock_valid_o;
  }
  sim.check(saw_txfma_valid, "TXFMA decode reaches lane-visible TXFMA valid");
  sim.check(saw_txfma_clock_valid, "TXFMA decode requests the TXFMA clock");
  sim.check(saw_rom_valid, "ROM/trans decode reaches lane-visible ROM valid");
  sim.check(saw_rom_clock_valid, "ROM/trans decode requests the ROM clock");

  drive_idle(sim.dut.get());
  sim.dut->tensorfma_start_i = 1;
  sim.dut->rd_i = 4;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventTfmaInst) != 0,
            "tensor-FMA start raises the top-level tensor-FMA event");
  sim.check((sim.dut->vpu_dbg_match_o & kEventTfmaInst) != 0,
            "tensor-FMA event is visible on debug match");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->tensorquant_start_i = 1;
  sim.dut->rd_i = 6;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventTquantInst) != 0,
            "tensor-quant start raises the top-level tensor-quant event");
  sim.check((sim.dut->vpu_dbg_filter_o[0] & kEventTquantInst) != 0,
            "tensor-quant event is visible on debug filter");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->reduce_start_i = 1;
  sim.dut->dcache_reduce_send_reg_i = 1;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventReduceInst) != 0,
            "tensor reduce start raises the top-level reduce event");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->tensorfma_start_i = 1;
  sim.dut->dcache_scp_fill_tenb_i = 1;
  bool saw_dcache_tensor_surface = false;
  for (int i = 0; i < 6; ++i) {
    sim.tick();
    saw_dcache_tensor_surface |= sim.dut->dcache_ctrl_tenb_credit_o || sim.dut->dcache_ctrl_tfma_enabled_o;
  }
  sim.check(saw_dcache_tensor_surface,
            "scratchpad/TENB response interacts with the tensor-FMA dcache control surface");

  drive_common_decode(sim.dut.get());
  sim.dut->txfma_i = 0;
  sim.dut->rom_i = 0;
  sim.dut->f2_core_kill_i = 1;
  sim.dut->f3_core_kill_i = 1;
  sim.dut->f4_core_kill_i = 1;
  sim.dut->chicken_vpulane_i = 0;
  sim.dut->chicken_vputima_i = 0;
  sim.dut->chicken_vputrans_i = 0;
  for (int i = 0; i < 4; ++i) sim.tick();
  sim.check((sim.dut->io_events_o & kEventTimaOps) == 0,
            "kill/chicken-bit stress phase keeps the TIMA event clear without a tensor request");

  return sim.finish();
}

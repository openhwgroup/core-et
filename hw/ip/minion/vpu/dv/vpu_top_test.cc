// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vvpu_top_tb;

namespace {

constexpr uint32_t kEventTimaOps    = 1u << 1;
constexpr uint32_t kEventTxfma32    = 1u << 3;
constexpr uint32_t kEventTransOps   = 1u << 5;
constexpr uint32_t kEventShortOps   = 1u << 6;
constexpr uint32_t kEventTfmaInst   = 1u << 8;
constexpr uint32_t kEventReduceInst = 1u << 9;
constexpr uint32_t kEventTquantInst = 1u << 10;

constexpr uint8_t kCmdAdd       = 0x00;
constexpr uint8_t kCmdSub       = 0x01;
constexpr uint8_t kCmdMadd      = 0x07;
constexpr uint8_t kCmdMax       = 0x10;
constexpr uint8_t kCmdCvtF16F32 = 0x3b;
constexpr uint8_t kCmdMvXf      = 0x14;
constexpr uint8_t kCmdMvFx      = 0x16;
constexpr uint8_t kCmdMand      = 0x43;
constexpr uint8_t kCmdSlli      = 0x50;
constexpr uint8_t kCmdCvtF32Un8 = 0x37;
constexpr uint8_t kCmdCvtF32Sn8 = 0x3a;

constexpr uint8_t kDtypeInt = 0;
constexpr uint8_t kDtypeF32 = 1;
constexpr uint8_t kDtypeF16 = 2;
constexpr uint8_t kDtypeF11 = 3;
constexpr uint8_t kDtypeF10 = 4;
constexpr uint8_t kDtypeUn8 = 8;
constexpr uint8_t kDtypeSn8 = 9;

constexpr uint8_t kRmRne = 0;
constexpr uint8_t kRmRtz = 1;
constexpr uint8_t kRmRdn = 2;
constexpr uint8_t kRmRup = 3;
constexpr uint8_t kRmRmm = 4;
constexpr uint8_t kRmDyn = 7;

constexpr uint64_t kDbgKillEx      = 1ull << 51;
constexpr uint64_t kDbgKillF2      = 1ull << 52;
constexpr uint64_t kDbgKillF3      = 1ull << 53;
constexpr uint64_t kDbgKillF4      = 1ull << 54;
constexpr uint64_t kDbgDcacheResp  = 1ull << 57;
constexpr uint32_t kAllDebugBanks  = 0x1f;

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
  dut->cmd_i = kCmdAdd;
  dut->dtype_i = kDtypeF32;
  dut->round_i = 0;
  dut->rm_i = kRmRne;
  dut->fcsr_rm_i = kRmRne;
  dut->cvt_i = 0;
  dut->toint_i = 0;
  dut->tointm_i = 0;
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

void tick_and_accum(SimCtrl<DUT>& sim, uint32_t& events, uint32_t& dbg_match,
                    uint32_t& dbg_filter, uint32_t* dbg_data_mask = nullptr) {
  sim.tick();
  events |= sim.dut->io_events_o;
  dbg_match |= sim.dut->vpu_dbg_match_o;
  dbg_filter |= sim.dut->vpu_dbg_filter_o[0];
  if (dbg_data_mask != nullptr) *dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
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
  dut->cmd_i = kCmdAdd;
  dut->dtype_i = kDtypeF32;
  dut->round_i = 0;
  dut->rm_i = kRmRne;
  dut->fcsr_rm_i = kRmRne;
  dut->cvt_i = 0;
  dut->toint_i = 0;
  dut->tointm_i = 0;
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
  uint32_t seen_dbg_data_mask = 0;

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

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdSub;
  sim.dut->round_i = 1;
  sim.dut->rm_i = kRmRdn;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdSub,
            "sub decode forwards the command to VPU control");
  sim.check(sim.dut->id_effective_rm_o == kRmRdn,
            "encoded RDN rounding reaches VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdMax;
  sim.dut->round_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdMax,
            "compare/min/max decode forwards the command to VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdCvtF16F32;
  sim.dut->dtype_i = kDtypeF16;
  sim.dut->cvt_i = 1;
  sim.dut->round_i = 1;
  sim.dut->rm_i = kRmRmm;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdCvtF16F32,
            "conversion decode forwards the command to VPU control");
  sim.check(sim.dut->id_effective_dtype_o == kDtypeF16,
            "F16 dtype reaches VPU control");
  sim.check(sim.dut->id_effective_rm_o == kRmRmm,
            "encoded RMM rounding reaches VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdMand;
  sim.dut->dtype_i = kDtypeInt;
  sim.dut->shsw_i = 0;
  sim.dut->maskop_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdMand,
            "logical/mask decode forwards the command to VPU control");
  sim.check(sim.dut->id_effective_dtype_o == kDtypeInt,
            "integer dtype reaches VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdSlli;
  sim.dut->dtype_i = kDtypeInt;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdSlli,
            "shift/immediate decode forwards the command to VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdMvXf;
  sim.dut->fromint_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdMvXf,
            "move-from-int decode forwards the command to VPU control");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->cmd_i = kCmdMvFx;
  sim.dut->toint_i = 1;
  sim.dut->tointm_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_cmd_o == kCmdMvFx,
            "move-to-int decode forwards the command to VPU control");
  sim.tick();

  const uint8_t dtype_values[] = {kDtypeF11, kDtypeF10, kDtypeUn8, kDtypeSn8};
  const char* dtype_names[] = {"F11", "F10", "Un8", "Sn8"};
  for (int i = 0; i < 4; ++i) {
    drive_common_decode(sim.dut.get());
    sim.dut->cmd_i = (dtype_values[i] == kDtypeUn8) ? kCmdCvtF32Un8 :
                     ((dtype_values[i] == kDtypeSn8) ? kCmdCvtF32Sn8 : kCmdAdd);
    sim.dut->dtype_i = dtype_values[i];
    sim.dut->cvt_i = (dtype_values[i] == kDtypeUn8) || (dtype_values[i] == kDtypeSn8);
    sim.dut->eval();
    sim.check(sim.dut->id_effective_dtype_o == dtype_values[i],
              std::string(dtype_names[i]) + " dtype reaches VPU control");
    sim.tick();
  }

  const uint8_t rm_values[] = {kRmRne, kRmRtz, kRmRdn, kRmRup, kRmRmm};
  const char* rm_names[] = {"RNE", "RTZ", "RDN", "RUP", "RMM"};
  for (int i = 0; i < 5; ++i) {
    drive_common_decode(sim.dut.get());
    sim.dut->txfma_i = 1;
    sim.dut->round_i = 1;
    sim.dut->rm_i = rm_values[i];
    sim.dut->eval();
    sim.check(sim.dut->id_effective_rm_o == rm_values[i],
              std::string(rm_names[i]) + " rounding reaches VPU control");
    sim.tick();
  }

  drive_common_decode(sim.dut.get());
  sim.dut->txfma_i = 1;
  sim.dut->round_i = 1;
  sim.dut->rm_i = kRmDyn;
  sim.dut->fcsr_rm_i = kRmRup;
  sim.dut->eval();
  sim.check(sim.dut->id_effective_rm_o == kRmRup,
            "dynamic rounding selects the CSR rounding mode");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->id_core_thread_id_i = 0;
  sim.dut->eval();
  const uint8_t thread0_id = sim.dut->id_regfile_thread_id_o;
  sim.tick();
  drive_common_decode(sim.dut.get());
  sim.dut->id_core_thread_id_i = 1;
  sim.dut->eval();
  const uint8_t thread1_id = sim.dut->id_regfile_thread_id_o;
  sim.check(thread0_id != thread1_id,
            "thread ID is reflected in VPU register-file control outputs");
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->rd_i = 13;
  sim.dut->ra1_i = 13;
  sim.dut->eval();
  sim.check((sim.dut->id_effective_cmd_o == kCmdAdd) &&
                ((sim.dut->vpu_dbg_data_nonzero_mask_o & 0x1) != 0),
            "same destination/source overlap is visible in VPU debug data");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->tensorfma_start_i = 1;
  sim.dut->rd_i = 4;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventTfmaInst) != 0,
            "tensor-FMA start raises the top-level tensor-FMA event");
  sim.check((sim.dut->vpu_dbg_match_o & kEventTfmaInst) != 0,
            "tensor-FMA event is visible on debug match");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.check((sim.dut->vpu_dbg_data_nonzero_mask_o & (1u << 3)) != 0,
            "tensor-FMA controls are visible in debug data bank 3");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->tensorquant_start_i = 1;
  sim.dut->rd_i = 6;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventTquantInst) != 0,
            "tensor-quant start raises the top-level tensor-quant event");
  sim.check((sim.dut->vpu_dbg_filter_o[0] & kEventTquantInst) != 0,
            "tensor-quant event is visible on debug filter");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.check((sim.dut->vpu_dbg_data_nonzero_mask_o & (1u << 4)) != 0,
            "tensor-quant controls are visible in debug data bank 4");
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->reduce_start_i = 1;
  sim.dut->dcache_reduce_send_reg_i = 1;
  sim.dut->eval();
  sim.check((sim.dut->io_events_o & kEventReduceInst) != 0,
            "tensor reduce start raises the top-level reduce event");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.check((sim.dut->vpu_dbg_data_nonzero_mask_o & (1u << 4)) != 0,
            "tensor-reduce controls are visible in debug data bank 4");
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
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;

  drive_idle(sim.dut.get());
  sim.dut->wb_dcache_resp_valid_i = 1;
  sim.dut->eval();
  sim.check((sim.dut->vpu_dbg_match_o & kDbgDcacheResp) != 0,
            "dcache writeback response is visible on the debug match output");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->dcache_reduce_exec_op_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->vpu_dbg_data_nonzero_mask_o != 0,
            "dcache reduce exec sideband is visible in debug data");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.tick();

  drive_idle(sim.dut.get());
  sim.dut->tensorstore_start_i = 1;
  sim.dut->rd_i = 8;
  sim.dut->eval();
  sim.check((sim.dut->vpu_dbg_data_nonzero_mask_o & (1u << 3)) != 0,
            "tensor-store start is visible in debug data bank 3");
  seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  sim.tick();

  drive_common_decode(sim.dut.get());
  sim.dut->txfma_i = 0;
  sim.dut->rom_i = 0;
  sim.dut->ex_core_kill_i = 1;
  sim.dut->f2_core_kill_i = 1;
  sim.dut->f3_core_kill_i = 1;
  sim.dut->f4_core_kill_i = 1;
  sim.dut->chicken_vpulane_i = 0;
  sim.dut->chicken_vputima_i = 0;
  sim.dut->chicken_vputrans_i = 0;
  uint64_t kill_dbg_match = 0;
  for (int i = 0; i < 4; ++i) {
    sim.tick();
    kill_dbg_match |= sim.dut->vpu_dbg_match_o;
    seen_dbg_data_mask |= sim.dut->vpu_dbg_data_nonzero_mask_o;
  }
  sim.check((sim.dut->io_events_o & kEventTimaOps) == 0,
            "kill/chicken-bit stress phase keeps the TIMA event clear without a tensor request");
  sim.check((kill_dbg_match & kDbgKillEx) != 0, "EX kill is visible on debug match");
  sim.check((kill_dbg_match & kDbgKillF2) != 0, "F2 kill is visible on debug match");
  sim.check((kill_dbg_match & kDbgKillF3) != 0, "F3 kill is visible on debug match");
  sim.check((kill_dbg_match & kDbgKillF4) != 0, "F4 kill is visible on debug match");
  sim.check((seen_dbg_data_mask & kAllDebugBanks) == kAllDebugBanks,
            "directed VPU top protocol phases exercise every debug data bank");

  return sim.finish();
}

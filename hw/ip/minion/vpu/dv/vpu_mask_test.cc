// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_mask_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vvpu_mask_tb;

namespace {

constexpr uint8_t kCmdMand      = 0x23;
constexpr uint8_t kCmdMovaXM    = 0x28;
constexpr uint8_t kCmdMovaMX    = 0x29;
constexpr uint8_t kCmdMpopc     = 0x2b;
constexpr uint8_t kCmdMpopcz    = 0x2c;
constexpr uint8_t kCmdMpocRast  = 0x68;

void clear_inputs(DUT* dut) {
    dut->ex_in_valid_i = 0;
    dut->ex_inst_valid_i = 0;
    dut->ex_regfile_raddr1_i = 0;
    dut->ex_regfile_raddr2_i = 0;
    dut->ex_cmd_i = 0;
    dut->ex_imm_i = 0;
    dut->ex_fromint_i = 0;
    dut->ex_fromint_data_i = 0;
    dut->ex_thread_id_i = 0;
    dut->ex_ignore_mask_i = 0;
    dut->f2_thread_id_i = 0;
    dut->f2_wen_i = 0;
    dut->f2_core_kill_i = 0;
    dut->f2_waddr_i = 0;
    dut->f2_maskop_i = 0;
    dut->f2_tointm_i = 0;
    dut->f3_thread_id_i = 0;
    dut->f3_regmask_from_txfma_i = 0;
    dut->f3_regmask_from_short_i = 0;
    dut->f3_core_kill_i = 0;
    dut->f3_regmask_wdata_lane_i = 0;
    dut->f4_regmask_from_txfma_i = 0;
    dut->f4_core_kill_i = 0;
    dut->f5_thread_id_i = 0;
    dut->f6_thread_id_i = 0;
    dut->f7_thread_id_i = 0;
    dut->f8_thread_id_i = 0;
    dut->f8_regmask_comp_res_i = 0;
}

void tick_n(SimCtrl<DUT>& sim, int n) {
    for (int i = 0; i < n; ++i) sim.tick();
}

uint64_t entry(uint64_t flat, unsigned idx) {
    return (flat >> (idx * 8)) & 0xffu;
}

uint8_t dest_addr(uint64_t packed, unsigned slot) {
    return static_cast<uint8_t>((packed >> (slot * 4 + 1)) & 0x7u);
}

uint8_t dest_thread(uint64_t packed, unsigned slot) {
    return static_cast<uint8_t>((packed >> (slot * 4)) & 0x1u);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    clear_inputs(sim.dut.get());
    sim.reset();

    sim.dut->ex_ignore_mask_i = 1;
    sim.dut->eval();
    sim.check(sim.dut->ex_mask_rf0_o == 0xff, "ignore-mask path should force all lanes active");
    clear_inputs(sim.dut.get());

    const uint64_t kMaskPattern = 0x0123456789abcdefULL;
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_inst_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMovaMX;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = kMaskPattern;
    sim.tick();
    clear_inputs(sim.dut.get());
    sim.dut->f2_wen_i = 1;
    sim.dut->f2_maskop_i = 1;
    sim.tick();
    clear_inputs(sim.dut.get());
    sim.tick();
    sim.check(sim.dut->mask_rf_thread0_o == kMaskPattern, "MOVA_M_X should write the full mask thread image");
    sim.check(sim.dut->mask_rf_thread1_o == 0, "thread 1 mask RF should remain untouched");

    clear_inputs(sim.dut.get());
    sim.dut->ex_thread_id_i = 0;
    sim.dut->eval();
    sim.check(sim.dut->ex_mask_rf0_o == static_cast<uint32_t>(entry(kMaskPattern, 0)),
              "mask RF0 should reflect the low mask entry after write");

    clear_inputs(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMovaXM;
    sim.tick();
    clear_inputs(sim.dut.get());
    tick_n(sim, 6);
    sim.check(sim.dut->f8_toint_data_o == kMaskPattern, "MOVA_X_M should return the full mask image");

    clear_inputs(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMpopc;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = 0x00000000000000aaULL;
    sim.tick();
    clear_inputs(sim.dut.get());
    tick_n(sim, 6);
    sim.check(sim.dut->f8_toint_data_o == 4, "MPOPC should count set bits in the low lane mask");

    clear_inputs(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMpopcz;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = 0x00000000000000aaULL;
    sim.tick();
    clear_inputs(sim.dut.get());
    tick_n(sim, 6);
    sim.check(sim.dut->f8_toint_data_o == 4, "MPOPCZ should count cleared bits in the low lane mask");

    clear_inputs(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMpocRast;
    sim.dut->ex_imm_i = 3;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = (0x0fULL << 32) | 0xf0ULL;
    sim.tick();
    clear_inputs(sim.dut.get());
    tick_n(sim, 6);
    sim.check(sim.dut->f8_toint_data_o == 8, "MPOPC_RAST should count selected bits across both mask halves");

    clear_inputs(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_inst_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMand;
    sim.tick();
    clear_inputs(sim.dut.get());
    sim.dut->f2_wen_i = 1;
    sim.dut->f2_maskop_i = 1;
    sim.dut->f2_waddr_i = 2;
    sim.tick();
    clear_inputs(sim.dut.get());
    sim.dut->f3_regmask_from_txfma_i = 1;
    sim.tick();
    clear_inputs(sim.dut.get());
    sim.dut->f4_regmask_from_txfma_i = 1;
    sim.tick();
    sim.check(sim.dut->wb_mask_valid_o == 0x1, "mask scoreboard should report the F5 slot on TXFMA path");
    sim.check(dest_addr(sim.dut->wb_mask_dest_packed_o, 0) == 2, "F5 scoreboard slot should carry the mask destination address");
    sim.check(dest_thread(sim.dut->wb_mask_dest_packed_o, 0) == 0, "F5 scoreboard slot should carry thread 0");

    clear_inputs(sim.dut.get());
    sim.tick();
    sim.check(sim.dut->wb_mask_valid_o == 0x2, "mask scoreboard should advance to the F6 slot");

    clear_inputs(sim.dut.get());
    sim.tick();
    sim.check(sim.dut->wb_mask_valid_o == 0x4, "mask scoreboard should advance to the F7 slot");

    clear_inputs(sim.dut.get());
    sim.dut->f8_regmask_comp_res_i = 0x3c;
    sim.tick();
    sim.check(sim.dut->wb_mask_valid_o == 0x8, "mask scoreboard should advance to the F8 slot");

    clear_inputs(sim.dut.get());
    sim.dut->f8_regmask_comp_res_i = 0x3c;
    sim.tick();
    sim.check(entry(sim.dut->mask_rf_thread0_o, 2) == 0x3c,
              "TXFMA compare result should commit into the selected mask entry");

    return sim.finish();
}

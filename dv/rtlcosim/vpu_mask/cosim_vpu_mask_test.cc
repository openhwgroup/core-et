// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_mask_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_vpu_mask_tb;

namespace {

constexpr uint8_t kCmdMand     = 0x23;
constexpr uint8_t kCmdMor      = 0x24;
constexpr uint8_t kCmdMxor     = 0x25;
constexpr uint8_t kCmdMnot     = 0x26;
constexpr uint8_t kCmdMovaXM   = 0x28;
constexpr uint8_t kCmdMovaMX   = 0x29;
constexpr uint8_t kCmdMpopc    = 0x2b;
constexpr uint8_t kCmdMpopcz   = 0x2c;
constexpr uint8_t kCmdMpocRast = 0x68;

struct PortSeen {
    bool saw_zero = false;
    bool saw_nonzero = false;

    void sample(uint64_t value) {
        if (value == 0) {
            saw_zero = true;
        } else {
            saw_nonzero = true;
        }
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void set_defaults(DUT* dut) {
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

void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("ex_mask_rf0", sim.dut->orig_ex_mask_rf0_o, sim.dut->new_ex_mask_rf0_o);
    sim.compare("ex_mask_in1", sim.dut->orig_ex_mask_in1_o, sim.dut->new_ex_mask_in1_o);
    sim.compare("f2_mask_rf0", sim.dut->orig_f2_mask_rf0_o, sim.dut->new_f2_mask_rf0_o);
    sim.compare("f8_toint_data", sim.dut->orig_f8_toint_data_o, sim.dut->new_f8_toint_data_o);
    sim.compare("f8_regmask_store", sim.dut->orig_f8_regmask_store_o, sim.dut->new_f8_regmask_store_o);
    sim.compare("wb_mask_valid", sim.dut->orig_wb_mask_valid_o, sim.dut->new_wb_mask_valid_o);
    sim.compare("wb_mask_dest", sim.dut->orig_wb_mask_dest_packed_o, sim.dut->new_wb_mask_dest_packed_o);
}

void sample_inputs(const DUT* dut,
                   PortSeen& ex_in_valid_seen,
                   PortSeen& ex_inst_valid_seen,
                   PortSeen& ex_raddr1_seen,
                   PortSeen& ex_raddr2_seen,
                   PortSeen& ex_cmd_seen,
                   PortSeen& ex_imm_seen,
                   PortSeen& ex_fromint_seen,
                   PortSeen& ex_fromint_data_seen,
                   PortSeen& ex_thread_seen,
                   PortSeen& ex_ignore_mask_seen,
                   PortSeen& f2_thread_seen,
                   PortSeen& f2_wen_seen,
                   PortSeen& f2_core_kill_seen,
                   PortSeen& f2_waddr_seen,
                   PortSeen& f2_maskop_seen,
                   PortSeen& f2_tointm_seen,
                   PortSeen& f3_thread_seen,
                   PortSeen& f3_from_txfma_seen,
                   PortSeen& f3_from_short_seen,
                   PortSeen& f3_core_kill_seen,
                   PortSeen& f3_lane_seen,
                   PortSeen& f4_from_txfma_seen,
                   PortSeen& f4_core_kill_seen,
                   PortSeen& f5_thread_seen,
                   PortSeen& f6_thread_seen,
                   PortSeen& f7_thread_seen,
                   PortSeen& f8_thread_seen,
                   PortSeen& f8_comp_seen) {
    ex_in_valid_seen.sample(dut->ex_in_valid_i);
    ex_inst_valid_seen.sample(dut->ex_inst_valid_i);
    ex_raddr1_seen.sample(dut->ex_regfile_raddr1_i);
    ex_raddr2_seen.sample(dut->ex_regfile_raddr2_i);
    ex_cmd_seen.sample(dut->ex_cmd_i);
    ex_imm_seen.sample(dut->ex_imm_i);
    ex_fromint_seen.sample(dut->ex_fromint_i);
    ex_fromint_data_seen.sample(dut->ex_fromint_data_i);
    ex_thread_seen.sample(dut->ex_thread_id_i);
    ex_ignore_mask_seen.sample(dut->ex_ignore_mask_i);
    f2_thread_seen.sample(dut->f2_thread_id_i);
    f2_wen_seen.sample(dut->f2_wen_i);
    f2_core_kill_seen.sample(dut->f2_core_kill_i);
    f2_waddr_seen.sample(dut->f2_waddr_i);
    f2_maskop_seen.sample(dut->f2_maskop_i);
    f2_tointm_seen.sample(dut->f2_tointm_i);
    f3_thread_seen.sample(dut->f3_thread_id_i);
    f3_from_txfma_seen.sample(dut->f3_regmask_from_txfma_i);
    f3_from_short_seen.sample(dut->f3_regmask_from_short_i);
    f3_core_kill_seen.sample(dut->f3_core_kill_i);
    f3_lane_seen.sample(dut->f3_regmask_wdata_lane_i);
    f4_from_txfma_seen.sample(dut->f4_regmask_from_txfma_i);
    f4_core_kill_seen.sample(dut->f4_core_kill_i);
    f5_thread_seen.sample(dut->f5_thread_id_i);
    f6_thread_seen.sample(dut->f6_thread_id_i);
    f7_thread_seen.sample(dut->f7_thread_id_i);
    f8_thread_seen.sample(dut->f8_thread_id_i);
    f8_comp_seen.sample(dut->f8_regmask_comp_res_i);
}

void tick_and_compare(CosimCtrl<DUT>& sim,
                      PortSeen& ex_in_valid_seen,
                      PortSeen& ex_inst_valid_seen,
                      PortSeen& ex_raddr1_seen,
                      PortSeen& ex_raddr2_seen,
                      PortSeen& ex_cmd_seen,
                      PortSeen& ex_imm_seen,
                      PortSeen& ex_fromint_seen,
                      PortSeen& ex_fromint_data_seen,
                      PortSeen& ex_thread_seen,
                      PortSeen& ex_ignore_mask_seen,
                      PortSeen& f2_thread_seen,
                      PortSeen& f2_wen_seen,
                      PortSeen& f2_core_kill_seen,
                      PortSeen& f2_waddr_seen,
                      PortSeen& f2_maskop_seen,
                      PortSeen& f2_tointm_seen,
                      PortSeen& f3_thread_seen,
                      PortSeen& f3_from_txfma_seen,
                      PortSeen& f3_from_short_seen,
                      PortSeen& f3_core_kill_seen,
                      PortSeen& f3_lane_seen,
                      PortSeen& f4_from_txfma_seen,
                      PortSeen& f4_core_kill_seen,
                      PortSeen& f5_thread_seen,
                      PortSeen& f6_thread_seen,
                      PortSeen& f7_thread_seen,
                      PortSeen& f8_thread_seen,
                      PortSeen& f8_comp_seen) {
    sample_inputs(sim.dut.get(), ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                  ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                  ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                  f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                  f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                  f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                  f8_comp_seen);
    sim.tick();
    compare_all(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    PortSeen ex_in_valid_seen;
    PortSeen ex_inst_valid_seen;
    PortSeen ex_raddr1_seen;
    PortSeen ex_raddr2_seen;
    PortSeen ex_cmd_seen;
    PortSeen ex_imm_seen;
    PortSeen ex_fromint_seen;
    PortSeen ex_fromint_data_seen;
    PortSeen ex_thread_seen;
    PortSeen ex_ignore_mask_seen;
    PortSeen f2_thread_seen;
    PortSeen f2_wen_seen;
    PortSeen f2_core_kill_seen;
    PortSeen f2_waddr_seen;
    PortSeen f2_maskop_seen;
    PortSeen f2_tointm_seen;
    PortSeen f3_thread_seen;
    PortSeen f3_from_txfma_seen;
    PortSeen f3_from_short_seen;
    PortSeen f3_core_kill_seen;
    PortSeen f3_lane_seen;
    PortSeen f4_from_txfma_seen;
    PortSeen f4_core_kill_seen;
    PortSeen f5_thread_seen;
    PortSeen f6_thread_seen;
    PortSeen f7_thread_seen;
    PortSeen f8_thread_seen;
    PortSeen f8_comp_seen;

    std::array<bool, 9> saw_cmd = {};
    uint32_t seed = 0x5eed1234u;

    set_defaults(sim.dut.get());
    sim.reset(4);

    set_defaults(sim.dut.get());
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->ex_ignore_mask_i = 1;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    // MOVA_M_X full-image write
    set_defaults(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_inst_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMovaMX;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = 0x0123456789abcdefULL;
    saw_cmd[0] = true;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f2_wen_i = 1;
    sim.dut->f2_maskop_i = 1;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    // MOVA_X_M readback
    set_defaults(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMovaXM;
    saw_cmd[1] = true;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    for (int i = 0; i < 6; ++i) {
        set_defaults(sim.dut.get());
        tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                         ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                         ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                         f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                         f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                         f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                         f8_comp_seen);
    }

    // Directed to-int and logical commands
    const std::array<uint8_t, 7> kDirectedCmds = {{
        kCmdMand, kCmdMor, kCmdMxor, kCmdMnot, kCmdMpopc, kCmdMpopcz, kCmdMpocRast
    }};
    for (size_t idx = 0; idx < kDirectedCmds.size(); ++idx) {
        set_defaults(sim.dut.get());
        sim.dut->ex_in_valid_i = 1;
        sim.dut->ex_cmd_i = kDirectedCmds[idx];
        sim.dut->ex_fromint_i = 1;
        sim.dut->ex_fromint_data_i = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
        sim.dut->ex_imm_i = xorshift32(seed) & 0xffu;
        saw_cmd[idx + 2] = true;
        tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                         ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                         ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                         f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                         f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                         f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                         f8_comp_seen);

        for (int flush = 0; flush < 6; ++flush) {
            set_defaults(sim.dut.get());
            tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                             ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                             ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                             f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                             f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                             f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                             f8_comp_seen);
        }
    }

    // Directed TXFMA-style mask scoreboard/writeback walk
    set_defaults(sim.dut.get());
    sim.dut->ex_in_valid_i = 1;
    sim.dut->ex_inst_valid_i = 1;
    sim.dut->ex_cmd_i = kCmdMand;
    sim.dut->ex_fromint_i = 1;
    sim.dut->ex_fromint_data_i = 0x00f0000f00aa0055ULL;
    saw_cmd[2] = true;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f2_wen_i = 1;
    sim.dut->f2_maskop_i = 1;
    sim.dut->f2_waddr_i = 2;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f3_regmask_from_txfma_i = 1;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f4_regmask_from_txfma_i = 1;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f8_regmask_comp_res_i = 0x3cu;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    set_defaults(sim.dut.get());
    sim.dut->f8_regmask_comp_res_i = 0x3cu;
    tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                     ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                     ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                     f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                     f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                     f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                     f8_comp_seen);

    for (int cycle = 0; cycle < 8192; ++cycle) {
        set_defaults(sim.dut.get());

        sim.dut->ex_in_valid_i = (xorshift32(seed) & 0x3u) != 0;
        sim.dut->ex_inst_valid_i = (xorshift32(seed) & 0x1u) != 0;
        sim.dut->ex_regfile_raddr1_i = xorshift32(seed) & 0x7u;
        sim.dut->ex_regfile_raddr2_i = xorshift32(seed) & 0x7u;
        sim.dut->ex_cmd_i = xorshift32(seed) & 0x7fu;
        sim.dut->ex_imm_i = xorshift32(seed) & 0xffu;
        sim.dut->ex_fromint_i = xorshift32(seed) & 0x1u;
        sim.dut->ex_fromint_data_i = (uint64_t{xorshift32(seed)} << 32) | xorshift32(seed);
        sim.dut->ex_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->ex_ignore_mask_i = xorshift32(seed) & 0x1u;

        sim.dut->f2_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f2_wen_i = xorshift32(seed) & 0x1u;
        sim.dut->f2_core_kill_i = xorshift32(seed) & 0x1u;
        sim.dut->f2_waddr_i = xorshift32(seed) & 0x7u;
        sim.dut->f2_maskop_i = xorshift32(seed) & 0x1u;
        sim.dut->f2_tointm_i = xorshift32(seed) & 0x1u;

        sim.dut->f3_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f3_regmask_from_txfma_i = xorshift32(seed) & 0x1u;
        sim.dut->f3_regmask_from_short_i = xorshift32(seed) & 0x1u;
        sim.dut->f3_core_kill_i = xorshift32(seed) & 0x1u;
        sim.dut->f3_regmask_wdata_lane_i = xorshift32(seed) & 0xffu;

        sim.dut->f4_regmask_from_txfma_i = xorshift32(seed) & 0x1u;
        sim.dut->f4_core_kill_i = xorshift32(seed) & 0x1u;

        sim.dut->f5_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f6_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f7_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f8_thread_id_i = xorshift32(seed) & 0x1u;
        sim.dut->f8_regmask_comp_res_i = xorshift32(seed) & 0xffu;

        tick_and_compare(sim, ex_in_valid_seen, ex_inst_valid_seen, ex_raddr1_seen, ex_raddr2_seen,
                         ex_cmd_seen, ex_imm_seen, ex_fromint_seen, ex_fromint_data_seen, ex_thread_seen,
                         ex_ignore_mask_seen, f2_thread_seen, f2_wen_seen, f2_core_kill_seen, f2_waddr_seen,
                         f2_maskop_seen, f2_tointm_seen, f3_thread_seen, f3_from_txfma_seen,
                         f3_from_short_seen, f3_core_kill_seen, f3_lane_seen, f4_from_txfma_seen,
                         f4_core_kill_seen, f5_thread_seen, f6_thread_seen, f7_thread_seen, f8_thread_seen,
                         f8_comp_seen);
    }

    for (size_t i = 0; i < saw_cmd.size(); ++i) {
        sim.check(saw_cmd[i], std::string("directed phase should cover mask command index ") + std::to_string(i));
    }

    sim.check(ex_in_valid_seen.saw_zero && ex_in_valid_seen.saw_nonzero, "ex_in_valid_i should toggle");
    sim.check(ex_inst_valid_seen.saw_zero && ex_inst_valid_seen.saw_nonzero, "ex_inst_valid_i should toggle");
    sim.check(ex_raddr1_seen.saw_zero && ex_raddr1_seen.saw_nonzero, "ex_regfile_raddr1_i should see zero and non-zero values");
    sim.check(ex_raddr2_seen.saw_zero && ex_raddr2_seen.saw_nonzero, "ex_regfile_raddr2_i should see zero and non-zero values");
    sim.check(ex_cmd_seen.saw_zero && ex_cmd_seen.saw_nonzero, "ex_cmd_i should see zero and non-zero values");
    sim.check(ex_imm_seen.saw_zero && ex_imm_seen.saw_nonzero, "ex_imm_i should see zero and non-zero values");
    sim.check(ex_fromint_seen.saw_zero && ex_fromint_seen.saw_nonzero, "ex_fromint_i should toggle");
    sim.check(ex_fromint_data_seen.saw_zero && ex_fromint_data_seen.saw_nonzero, "ex_fromint_data_i should see zero and non-zero values");
    sim.check(ex_thread_seen.saw_zero && ex_thread_seen.saw_nonzero, "ex_thread_id_i should toggle");
    sim.check(ex_ignore_mask_seen.saw_zero && ex_ignore_mask_seen.saw_nonzero, "ex_ignore_mask_i should toggle");
    sim.check(f2_thread_seen.saw_zero && f2_thread_seen.saw_nonzero, "f2_thread_id_i should toggle");
    sim.check(f2_wen_seen.saw_zero && f2_wen_seen.saw_nonzero, "f2_wen_i should toggle");
    sim.check(f2_core_kill_seen.saw_zero && f2_core_kill_seen.saw_nonzero, "f2_core_kill_i should toggle");
    sim.check(f2_waddr_seen.saw_zero && f2_waddr_seen.saw_nonzero, "f2_waddr_i should see zero and non-zero values");
    sim.check(f2_maskop_seen.saw_zero && f2_maskop_seen.saw_nonzero, "f2_maskop_i should toggle");
    sim.check(f2_tointm_seen.saw_zero && f2_tointm_seen.saw_nonzero, "f2_tointm_i should toggle");
    sim.check(f3_thread_seen.saw_zero && f3_thread_seen.saw_nonzero, "f3_thread_id_i should toggle");
    sim.check(f3_from_txfma_seen.saw_zero && f3_from_txfma_seen.saw_nonzero, "f3_regmask_from_txfma_i should toggle");
    sim.check(f3_from_short_seen.saw_zero && f3_from_short_seen.saw_nonzero, "f3_regmask_from_short_i should toggle");
    sim.check(f3_core_kill_seen.saw_zero && f3_core_kill_seen.saw_nonzero, "f3_core_kill_i should toggle");
    sim.check(f3_lane_seen.saw_zero && f3_lane_seen.saw_nonzero, "f3_regmask_wdata_lane_i should see zero and non-zero values");
    sim.check(f4_from_txfma_seen.saw_zero && f4_from_txfma_seen.saw_nonzero, "f4_regmask_from_txfma_i should toggle");
    sim.check(f4_core_kill_seen.saw_zero && f4_core_kill_seen.saw_nonzero, "f4_core_kill_i should toggle");
    sim.check(f5_thread_seen.saw_zero && f5_thread_seen.saw_nonzero, "f5_thread_id_i should toggle");
    sim.check(f6_thread_seen.saw_zero && f6_thread_seen.saw_nonzero, "f6_thread_id_i should toggle");
    sim.check(f7_thread_seen.saw_zero && f7_thread_seen.saw_nonzero, "f7_thread_id_i should toggle");
    sim.check(f8_thread_seen.saw_zero && f8_thread_seen.saw_nonzero, "f8_thread_id_i should toggle");
    sim.check(f8_comp_seen.saw_zero && f8_comp_seen.saw_nonzero, "f8_regmask_comp_res_i should see zero and non-zero values");

    return sim.finish();
}

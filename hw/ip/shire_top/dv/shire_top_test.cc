// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_top_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

namespace {
constexpr uint8_t kShireId = 0x2a;
constexpr uint32_t kNumBanks = 4;
constexpr uint32_t kApbShireIdx = kNumBanks;
constexpr uint32_t kRegShireConfig = 0x001;
constexpr uint32_t kRegPwrNeighNsleepin = 0x045;
constexpr uint32_t kRegPwrNeighIso = 0x046;
constexpr uint32_t kRegRamCfg2 = 0x055;
constexpr uint64_t kConfigEnableAll =
    (uint64_t(3) << 24) | (uint64_t(2) << 22) | (uint64_t(1) << 20) |
    (uint64_t(1) << 17) | (uint64_t(0xf) << 13) | (uint64_t(0xf) << 9) |
    (uint64_t(1) << 8) | uint64_t(kShireId);

uint64_t make_addr_id(uint8_t shire_id, uint32_t cls, uint32_t block,
                      uint32_t bank_or_neigh, uint32_t reg, uint32_t pp = 3) {
    uint64_t addr = 0;
    addr |= 1ull << 32;
    addr |= static_cast<uint64_t>(pp & 0x3) << 30;
    addr |= static_cast<uint64_t>(shire_id) << 22;
    addr |= static_cast<uint64_t>(cls & 0x3) << 20;
    if (cls == 3) {
        addr |= static_cast<uint64_t>(block & 0x7) << 17;
        if (block == 0) addr |= static_cast<uint64_t>(bank_or_neigh & 0xf) << 13;
        addr |= static_cast<uint64_t>(reg & 0x3fff) << 3;
    } else if (cls == 1) {
        addr |= static_cast<uint64_t>(bank_or_neigh & 0xf) << 16;
        addr |= static_cast<uint64_t>(reg & 0x1fff) << 3;
    } else {
        addr |= static_cast<uint64_t>(reg & 0x7f) << 13;
    }
    return addr;
}

uint64_t make_addr(uint32_t cls, uint32_t block, uint32_t bank_or_neigh,
                   uint32_t reg, uint32_t pp = 3) {
    return make_addr_id(kShireId, cls, block, bank_or_neigh, reg, pp);
}

uint64_t shire_reg_addr(uint32_t reg) { return make_addr(3, 2, 0, reg); }

void set_wdata(Vshire_top_tb* dut, uint64_t lo, uint64_t addr = 0) {
    const int lane = static_cast<int>((addr >> 3) & 0x3u);
    for (int i = 0; i < 8; ++i) dut->sys_wdata_stim_i[i] = 0;
    dut->sys_wdata_stim_i[2 * lane] = static_cast<uint32_t>(lo);
    dut->sys_wdata_stim_i[2 * lane + 1] = static_cast<uint32_t>(lo >> 32);
}

void clear_inputs(Vshire_top_tb* dut) {
    dut->rst_cold_ext_ni = 1;
    dut->rst_warm_ext_ni = 1;
    dut->rst_system_ext_ni = 1;
    dut->rst_system_debug_ext_ni = 1;
    dut->rst_noc_ext_ni = 1;
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_ni = 1;
    dut->dft_sram_clk_i = 0;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 0;
    dut->dft_mbist_en_i = 0;
    dut->shire_id_reset_val_i = kShireId;
    dut->shire_phy_id_i = 0x03;
    dut->plic_mtip_i = 0;
    dut->plic_meip_i = 1;
    dut->plic_seip_i = 0;
    dut->noc_err_int_srcs_i = 0;
    dut->status_monitor_bank_sel_stim_i = 0;
    dut->debug_and_or_tree_l2_stim_i = 0;
    dut->sys_axi_ar_valid_stim_i = 0;
    dut->sys_axi_aw_valid_stim_i = 0;
    dut->sys_axi_w_valid_stim_i = 0;
    dut->sys_axi_b_ready_stim_i = 1;
    dut->sys_axi_r_ready_stim_i = 1;
    dut->sys_axi_aw_vcvalid_stim_i = 0;
    dut->sys_axi_w_vcvalid_stim_i = 0;
    dut->sys_id_stim_i = 0;
    dut->sys_addr_stim_i = 0;
    dut->sys_len_stim_i = 0;
    dut->sys_size_stim_i = 3;
    set_wdata(dut, 0);
}

bool wait_for_bvalid(SimCtrl<Vshire_top_tb>& sim, int max_cycles = 240) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sim.dut->sys_axi_b_valid_obs_o) return true;
    }
    return false;
}

void wait_bvalid_low(SimCtrl<Vshire_top_tb>& sim, int max_cycles = 80) {
    for (int i = 0; i < max_cycles; ++i) {
        if (!sim.dut->sys_axi_b_valid_obs_o) return;
        sim.tick();
    }
}

void issue_write(SimCtrl<Vshire_top_tb>& sim, uint64_t addr, uint64_t data, uint32_t id) {
    auto* dut = sim.dut.get();
    wait_bvalid_low(sim);
    dut->sys_id_stim_i = id;
    dut->sys_addr_stim_i = addr;
    dut->sys_len_stim_i = 0;
    dut->sys_size_stim_i = 3;
    set_wdata(dut, data, addr);
    dut->sys_axi_aw_vcvalid_stim_i = 2;
    dut->sys_axi_w_vcvalid_stim_i = 2;
    dut->sys_axi_aw_valid_stim_i = 1;
    dut->sys_axi_w_valid_stim_i = 1;
    for (int i = 0; i < 80; ++i) {
        sim.tick();
        if (dut->sys_axi_aw_ready_obs_o && dut->sys_axi_w_ready_obs_o) break;
    }
    dut->sys_axi_aw_valid_stim_i = 0;
    dut->sys_axi_w_valid_stim_i = 0;
    dut->sys_axi_aw_vcvalid_stim_i = 0;
    dut->sys_axi_w_vcvalid_stim_i = 0;
}

bool issue_write_saw_psel(SimCtrl<Vshire_top_tb>& sim, uint64_t addr, uint64_t data,
                          uint32_t id, uint32_t mask, bool channel_lane) {
    auto* dut = sim.dut.get();
    wait_bvalid_low(sim);
    dut->sys_id_stim_i = id;
    dut->sys_addr_stim_i = addr;
    dut->sys_len_stim_i = 0;
    dut->sys_size_stim_i = 3;
    set_wdata(dut, data, addr);
    dut->sys_axi_aw_vcvalid_stim_i = 2;
    dut->sys_axi_w_vcvalid_stim_i = 2;
    dut->sys_axi_aw_valid_stim_i = 1;
    dut->sys_axi_w_valid_stim_i = 1;
    bool saw_psel = false;
    bool accepted = false;
    for (int i = 0; i < 360; ++i) {
        sim.tick();
        const uint32_t psel = channel_lane ? dut->apb_channel_psel_o : dut->apb_neigh_psel_o;
        saw_psel = saw_psel || ((psel & mask) == mask);
        if (!accepted && dut->sys_axi_aw_ready_obs_o && dut->sys_axi_w_ready_obs_o) {
            accepted = true;
            dut->sys_axi_aw_valid_stim_i = 0;
            dut->sys_axi_w_valid_stim_i = 0;
            dut->sys_axi_aw_vcvalid_stim_i = 0;
            dut->sys_axi_w_vcvalid_stim_i = 0;
        }
        if (accepted && saw_psel && dut->sys_axi_b_valid_obs_o) break;
    }
    dut->sys_axi_aw_valid_stim_i = 0;
    dut->sys_axi_w_valid_stim_i = 0;
    dut->sys_axi_aw_vcvalid_stim_i = 0;
    dut->sys_axi_w_vcvalid_stim_i = 0;
    return saw_psel;
}
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vshire_top_tb> sim(argc, argv);
    auto* dut = sim.dut.get();
    clear_inputs(dut);
    sim.reset(6);
    clear_inputs(dut);
    for (int i = 0; i < 48; ++i) sim.tick();

    sim.check(dut->rst_system_lv_no_o == 1, "low-voltage system reset releases");
    sim.check(dut->rst_system_debug_lv_no_o == 1, "low-voltage debug reset releases");
    sim.check(dut->rst_noc_lv_no_o == 1, "low-voltage NOC reset releases");
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "raw warm-reset fanout to neighborhoods releases");
    sim.check((dut->rst_w_icache_no_o & 0xf) == 0x0,
              "neighborhood ICache warm reset return path is connected in all-stub smoke");
    sim.check((dut->clk_neigh_obs_o & 0xf) == (dut->clk_shire_obs_o ? 0xf : 0x0),
              "contracted neighborhood clocks fan out uniformly in DV smoke mode");
    sim.check((dut->shire_id_o & 0xffu) == kShireId,
              "shire ID reset value reaches neighborhood ID fanout");
    sim.check((dut->tbox_id_o & 0xffu) == 0xe4u,
              "TBOX IDs fan out to neighborhoods");
    sim.check((dut->plic_meip_to_neigh_o & 0xf) == 0xf,
              "MEIP interrupt fans out to all neighborhoods");
    sim.check((dut->plic_seip_to_neigh_o & 0xf) == 0x0,
              "SEIP interrupt low fans out to all neighborhoods");
    sim.check((dut->int_mtip_flat_o & 0xffffffffu) == 0,
              "MTIP is low when plic_mtip_i is low");
    sim.check(dut->dft_hv_flat_o == 0x08u, "DFT struct reaches neighborhood top in normal mode");

    dut->rst_cold_ext_ni = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check((dut->rst_c_shire_no_o & 0xf) == 0, "cold reset asserts neighborhood cold resets");
    sim.check(dut->pwr_ctrl_neigh_nsleepin_o == 0xffffffffu,
              "cold reset forces minion power nsleepin high");
    sim.check(dut->pwr_ctrl_neigh_iso_o == 0,
              "cold reset masks minion isolation low");
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "cold reset does not alter raw warm-reset fanout");
    dut->rst_cold_ext_ni = 1;
    for (int i = 0; i < 48; ++i) sim.tick();

    dut->rst_warm_ext_ni = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0,
              "warm reset drives raw neighborhood fanout low");
    dut->rst_warm_ext_ni = 1;
    for (int i = 0; i < 48; ++i) sim.tick();
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "warm reset fanout releases after warm reset deasserts");

    dut->rst_noc_ext_ni = 0;
    sim.tick();
    sim.check(dut->rst_noc_lv_no_o == 0, "native NOC reset asserts NOC low-voltage path");
    sim.check(dut->rst_system_lv_no_o == 1, "native NOC reset does not collapse into system reset");
    dut->rst_noc_ext_ni = 1;
    for (int i = 0; i < 48; ++i) sim.tick();
    sim.check(dut->rst_noc_lv_no_o == 1, "native NOC reset path releases independently");

    dut->rst_system_ext_ni = 0;
    sim.tick();
    sim.check(dut->rst_system_lv_no_o == 0, "system reset asserts system low-voltage path");
    sim.check(dut->rst_noc_lv_no_o == 1, "system reset does not collapse into NOC reset");
    dut->rst_system_ext_ni = 1;
    for (int i = 0; i < 48; ++i) sim.tick();

    dut->dft_scanmode_i = 1;
    dut->dft_scan_reset_ni = 0;
    sim.tick();
    sim.check(dut->rst_system_lv_no_o == 0, "DFT scan reset bypass asserts system LV reset");
    sim.check(dut->rst_noc_lv_no_o == 0, "DFT scan reset bypass asserts NOC LV reset");
    dut->dft_scan_reset_ni = 1;
    dut->dft_sram_clk_override_i = 1;
    dut->dft_ram_wei_i = 1;
    sim.tick();
    sim.check((dut->dft_hv_flat_o & 0x1fu) == 0x1du,
              "DFT scan/SRAM/WEI fields propagate to neighborhood DFT struct");
    dut->dft_scanmode_i = 0;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_wei_i = 0;
    for (int i = 0; i < 4; ++i) sim.tick();

    dut->noc_err_int_srcs_i = 0x155;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check((dut->noc_all_err_int_srcs_o & 0x3ffu) == 0x155u,
              "NOC error status seam follows native top input");
    sim.check(dut->ioshire_noc_err_int_o == 1,
              "NOC error interrupt asserts for nonzero native sources");
    dut->noc_err_int_srcs_i = 0;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check(dut->ioshire_noc_err_int_o == 0, "NOC error interrupt clears");

    sim.check(dut->sys_axi_aw_ready_obs_o == 1, "SYS AXI AW ready is exposed");
    sim.check(dut->sys_axi_w_ready_obs_o == 1, "SYS AXI W ready is exposed");
    issue_write(sim, shire_reg_addr(kRegShireConfig), kConfigEnableAll, 3);
    sim.check(wait_for_bvalid(sim, 160), "SYS AXI write to shire ESR returns B response");
    sim.check(dut->sys_axi_b_valid_obs_o == 1, "SYS AXI B response remains visible after wait");
    sim.check((dut->tbox_en_o & 0xf) == 0xf,
              "TBOX enable fanout follows shire config write");

    sim.check(issue_write_saw_psel(sim, make_addr(1, 0, 2, 0x010),
                                   0x1111222233334444ull, 4, 1u << 2, false),
              "SYS write selects neighborhood APB lane 2 through top");
    sim.check(issue_write_saw_psel(sim, make_addr(3, 0, 1, 0x020),
                                   0x2222333344445555ull, 5, 1u << 1, true),
              "SYS write selects cache-bank APB lane 1 through top");
    sim.check(issue_write_saw_psel(sim, shire_reg_addr(kRegShireConfig),
                                   kConfigEnableAll, 6, 1u << kApbShireIdx, true),
              "SYS write selects shire ESR APB lane through top");

    const uint64_t ram_cfg2 = (1ull << 13) | (5ull << 10) | (3ull << 7) |
                              (2ull << 5) | (0xaull << 1) | 1ull;
    issue_write(sim, shire_reg_addr(kRegRamCfg2), ram_cfg2, 8);
    sim.check(wait_for_bvalid(sim, 160), "RAM configuration write returns B response");
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check(((dut->ram_cfg_flat_o >> 14) & 1u) == 1,
              "RAM config test_en propagates to top observation seam");
    sim.check(((dut->ram_cfg_flat_o >> 10) & 0xfu) == 0xa,
              "RAM config read margin propagates to top observation seam");
    sim.check(((dut->ram_cfg_flat_o >> 2) & 0x7u) == 5,
              "RAM config write pulse propagates to top observation seam");

    issue_write(sim, shire_reg_addr(kRegPwrNeighNsleepin), 0x11223344u, 9);
    sim.check(wait_for_bvalid(sim, 160), "neighborhood nsleepin write returns B response");
    issue_write(sim, shire_reg_addr(kRegPwrNeighIso), 0x55667788u, 10);
    sim.check(wait_for_bvalid(sim, 160), "neighborhood isolation write returns B response");
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check(dut->pwr_ctrl_neigh_nsleepin_o == 0x11223344u,
              "minion nsleepin control fans out through top");
    sim.check(dut->pwr_ctrl_neigh_iso_o == 0x55667788u,
              "minion isolation control fans out through top");

    sim.check(dut->neigh_sc_req_ready_o != 0,
              "neighborhood/cache ET-Link ready seam is connected");
    sim.check((dut->neigh_sc_rsp_valid_o & 0xf) == 0,
              "neighborhood/cache ET-Link response valid is quiet without traffic");
    sim.check(dut->icache_req_ready_o != 0,
              "ICache SRAM ready fanout is observable at top integration");
    sim.check((dut->icache_resp_valid_o & 0xf) == 0,
              "ICache SRAM response valid is quiet without requests");
    sim.check((dut->flb_l2_neigh_resp_valid_o & 0xf) == 0,
              "FLB response seam is quiet without requests");
    sim.check((dut->uc_to_neigh_fcc_o & 0xffffffffu) == 0,
              "uncached FCC fanout is quiet without uncached traffic");
    sim.check((dut->coop_slv_valid_o & 0xfffu) == 0,
              "cooperative TLoad slave-valid mesh is quiet in all-stub smoke");
    sim.check((dut->coop_done_valid_o & 0xfffu) == 0,
              "cooperative TLoad done-valid mesh is quiet in all-stub smoke");

    return sim.finish();
}

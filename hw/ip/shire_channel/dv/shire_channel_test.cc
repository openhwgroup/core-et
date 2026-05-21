// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_channel_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

namespace {
constexpr uint8_t kApbBank0 = 0;
constexpr uint8_t kApbShire = 4;
constexpr uint8_t kApbRbox = 5;
constexpr uint8_t kApbIcache = 6;
constexpr uint32_t kAddrShireConfig = 0x0c001;
constexpr uint32_t kAddrIpiTrigger = 0x0c012;
constexpr uint32_t kAddrMtimeTarget = 0x0c043;
constexpr uint32_t kAddrShirePowerCtrl = 0x0c044;
constexpr uint32_t kAddrPowerCtrlNeighNsleepin = 0x0c045;
constexpr uint32_t kAddrPowerCtrlNeighIsolation = 0x0c046;
constexpr uint32_t kAddrRamCfg2 = 0x0c055;
constexpr uint32_t kAddrClkGateCtrl = 0x0c062;
constexpr uint64_t kConfigEnableAll =
    (uint64_t(3) << 24) | (uint64_t(2) << 22) | (uint64_t(1) << 20) |
    (uint64_t(1) << 17) | (uint64_t(0xf) << 13) | (uint64_t(0xf) << 9) |
    (uint64_t(1) << 8) | uint64_t(0x2a);

void clear_inputs(Vshire_channel_tb* dut) {
    dut->rst_c_ext_ni = 1;
    dut->rst_w_ext_ni = 1;
    dut->rst_d_ext_ni = 1;
    dut->rst_noc_ext_ni = 1;
    dut->rst_w_icache_ext_ni = 0xf;
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_ni = 1;
    dut->dft_sram_clk_i = 0;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 0;
    dut->dft_mbist_en_i = 0;
    dut->shire_id_reset_val_i = 0x11;
    dut->shire_phy_id_i = 0x03;
    dut->apb_sel_i = 0;
    dut->apb_paddr_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = 0;
    dut->noc_err_int_srcs_i = 0;
    dut->neigh_sc_err_detected_i = 0;
    dut->neigh_sc_err_logged_i = 0;
    dut->flb_neigh_l2_req_valid_i = 0;
    dut->flb_neigh_l2_req_data_i = 0;
    dut->neigh0_req_valid_i = 0;
    dut->neigh0_req_dest_i = 0;
    dut->neigh0_req_addr_i = 0;
    dut->neigh0_req_opcode_i = 1;
    dut->neigh0_req_size_i = 6;
    dut->neigh0_req_qwen_i = 0xf;
    dut->icache_req_valid_i = 0;
    dut->icache_req_write_i = 0;
    dut->icache_req_addr_i = 0;
    dut->icache_resp_ready_i = 0xf;
    dut->coop_slv_valid_i = 0;
    dut->coop_done_id_i = 0;
    dut->coop_done_valid_i = 0;
    dut->esr_icache_prefetch_done_stim_i = 0;
    dut->esr_and_or_tree_l0_flat_stim_i = 0;
    dut->debug_and_or_tree_l2_stim_i = 0;
    dut->bpam_rc_tbox_ack_flat_stim_i = 0;
    dut->esr_pwr_ctrl_glb_nsleepout_stim_i = 0;
    dut->esr_pwr_ctrl_neigh_nsleepout_stim_i = 0;
    dut->esr_pll_busy_stim_i = 0;
    dut->esr_pll_rdata_stim_i = 0;
    dut->esr_pll_rrdy_stim_i = 0;
    dut->esr_pll_lock_stim_i = 1;
    dut->esr_dll_dly_est_sts_flat_stim_i = 0;
    dut->esr_dll_busy_stim_i = 0;
    dut->esr_dll_rdata_stim_i = 0;
    dut->esr_dll_rrdy_stim_i = 0;
    dut->esr_dll_lock_stim_i = 1;
    dut->status_monitor_bank_sel_i = 0;
    dut->neigh_sc_rsp_ready_stim_i = 0xf;
    dut->to_l3_axi_ar_ready_stim_i = 0xf;
    dut->to_l3_axi_aw_ready_stim_i = 0xf;
    dut->to_l3_axi_w_ready_stim_i = 0xf;
    dut->to_l3_axi_b_valid_stim_i = 0;
    dut->to_l3_axi_r_valid_stim_i = 0;
    dut->to_sys_axi_ar_ready_stim_i = 1;
    dut->to_sys_axi_aw_ready_stim_i = 1;
    dut->to_sys_axi_w_ready_stim_i = 1;
    dut->to_sys_axi_b_valid_stim_i = 0;
    dut->to_sys_axi_r_valid_stim_i = 0;
    dut->l3_axi_ar_valid_stim_i = 0;
    dut->l3_axi_aw_valid_stim_i = 0;
    dut->l3_axi_w_valid_stim_i = 0;
    dut->l3_axi_b_ready_stim_i = 0xf;
    dut->l3_axi_r_ready_stim_i = 0xf;
    dut->uc_to_l3_axi_ar_ready_stim_i = 1;
    dut->uc_to_l3_axi_aw_ready_stim_i = 1;
    dut->uc_to_l3_axi_w_ready_stim_i = 1;
    dut->uc_to_l3_axi_b_valid_stim_i = 0;
    dut->uc_to_l3_axi_r_valid_stim_i = 0;
    dut->uc_to_sys_axi_ar_ready_stim_i = 1;
    dut->uc_to_sys_axi_aw_ready_stim_i = 1;
    dut->uc_to_sys_axi_w_ready_stim_i = 1;
    dut->uc_to_sys_axi_b_valid_stim_i = 0;
    dut->uc_to_sys_axi_r_valid_stim_i = 0;
    dut->sys_axi_ar_valid_stim_i = 0;
    dut->sys_axi_aw_valid_stim_i = 0;
    dut->sys_axi_w_valid_stim_i = 0;
    dut->sys_axi_b_ready_stim_i = 1;
    dut->sys_axi_r_ready_stim_i = 1;
    dut->sys_axi_aw_vcvalid_stim_i = 0;
    dut->sys_axi_w_vcvalid_stim_i = 0;
    dut->sbm_write_credit_return_stim_i = 1;
    dut->sbm_sys_axi_ar_ready_stim_i = 1;
    dut->sbm_sys_axi_aw_ready_stim_i = 1;
    dut->sbm_sys_axi_w_ready_stim_i = 1;
    dut->sbm_sys_axi_b_valid_stim_i = 0;
    dut->sbm_sys_axi_r_valid_stim_i = 0;
    dut->axi_stim_i = 0;
}

void apb_idle(Vshire_channel_tb* dut) {
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwrite_i = 0;
}

void apb_write(SimCtrl<Vshire_channel_tb>& sim, uint8_t lane, uint32_t addr, uint64_t data) {
    auto* dut = sim.dut.get();
    dut->apb_sel_i = lane;
    dut->apb_paddr_i = addr;
    dut->apb_pwdata_i = data;
    dut->apb_pwrite_i = 1;
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 0;
    sim.tick();
    dut->apb_penable_i = 1;
    for (int i = 0; i < 40 && !dut->apb_pready_o; ++i) sim.tick();
    sim.check(dut->apb_pready_o, "APB write completed");
    apb_idle(dut);
    sim.tick();
}

uint64_t apb_read(SimCtrl<Vshire_channel_tb>& sim, uint8_t lane, uint32_t addr) {
    auto* dut = sim.dut.get();
    dut->apb_sel_i = lane;
    dut->apb_paddr_i = addr;
    dut->apb_pwdata_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 0;
    sim.tick();
    dut->apb_penable_i = 1;
    for (int i = 0; i < 40 && !dut->apb_pready_o; ++i) sim.tick();
    sim.check(dut->apb_pready_o, "APB read completed");
    uint64_t data = dut->apb_prdata_o;
    apb_idle(dut);
    sim.tick();
    return data;
}
} // namespace

int main(int argc, char** argv) {
    SimCtrl<Vshire_channel_tb> sim(argc, argv);
    auto* dut = sim.dut.get();
    clear_inputs(dut);
    sim.reset(8);

    sim.check(dut->rst_c_shire_no_o == 0, "neighborhood cold resets held while ESR neigh_en=0");
    sim.check(dut->rst_sc_no_o == 0, "cache cold reset held while ESR cache_en=0");
    sim.check(dut->rst_rbox_no_o == 1, "RBOX warm reset released by default rbox_en");
    sim.check((dut->tbox_en_o & 0xf) == 0x5, "default tbox enable reset value observed");
    sim.check((dut->pwr_ctrl_glb_nsleepin_o & 0xf) == 0xf, "global power nsleep reset value observed");
    sim.check((dut->pwr_ctrl_glb_iso_o & 0xf) == 0x0, "global power isolation reset value observed");
    sim.check(dut->pwr_ctrl_neigh_nsleepin_o == 0xffffffffu, "neighborhood power nsleep reset value observed");
    sim.check(dut->pwr_ctrl_neigh_iso_o == 0, "neighborhood power isolation reset value observed");

    apb_write(sim, kApbShire, kAddrShireConfig, kConfigEnableAll);
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check((dut->rst_c_shire_no_o & 0xf) == 0xf, "neighborhood cold resets release after config enable");
    sim.check((dut->rst_w_shire_no_o & 0xf) == 0xf, "neighborhood warm resets release after config enable");
    sim.check(dut->rst_sc_no_o == 1, "cache reset releases after config enable");
    sim.check(dut->rst_rbox_no_o == 1, "RBOX reset remains released after config enable");
    sim.check((dut->tbox_en_o & 0xf) == 0xf, "tbox enables update through ESR config");
    sim.check((dut->tbox_id_flat_o & 0xff) == 0xe4, "tbox IDs update through ESR config");
    sim.check((dut->shire_id_o & 0xff) == 0x2a, "shire_id replicated to neighborhood 0");
    sim.check(((dut->shire_id_o >> 24) & 0xff) == 0x2a, "shire_id replicated to neighborhood 3");
    uint64_t cfg_read = apb_read(sim, kApbShire, kAddrShireConfig);
    sim.check((cfg_read & 0x03ffffffu) == (kConfigEnableAll & 0x03ffffffu), "APB readback of shire config");

    (void)apb_read(sim, kApbBank0, 0);
    sim.check(true, "cache-bank APB fanout lane completes a read");
    (void)apb_read(sim, kApbRbox, 0);
    sim.check(true, "RBOX APB fanout lane completes a read");
    (void)apb_read(sim, kApbIcache, 0);
    sim.check(true, "ICache APB fanout lane completes a read");

    apb_write(sim, kApbShire, kAddrIpiTrigger, 0x55);
    sim.check((dut->esr_ipi_trigger_o & 0xff) == 0x55, "IPI trigger ESR output updates");

    apb_write(sim, kApbShire, kAddrMtimeTarget, 0xa5a5);
    sim.check((dut->esr_mtime_local_target_o & 0xffff) == 0xa5a5, "mtime local target ESR output updates");

    apb_write(sim, kApbShire, kAddrShirePowerCtrl, 0xa5);
    sim.check((dut->pwr_ctrl_glb_nsleepin_o & 0xf) == 0x5, "global power nsleep field updates");
    sim.check((dut->pwr_ctrl_glb_iso_o & 0xf) == 0xa, "global power isolation field updates");
    apb_write(sim, kApbShire, kAddrPowerCtrlNeighNsleepin, 0x13579bdf);
    sim.check(dut->pwr_ctrl_neigh_nsleepin_o == 0x13579bdfu, "neighborhood nsleep power field updates");
    apb_write(sim, kApbShire, kAddrPowerCtrlNeighIsolation, 0x2468ace0);
    sim.check(dut->pwr_ctrl_neigh_iso_o == 0x2468ace0u, "neighborhood isolation power field updates");

    apb_write(sim, kApbShire, kAddrRamCfg2, ~uint64_t{0});
    sim.check(dut->ram_cfg_flat_o != 0, "RAM config fields propagate from shire ESR");

    apb_write(sim, kApbShire, kAddrClkGateCtrl, 0x7ff);
    sim.check((dut->clk_gate_ctrl_flat_o & 0x7ff) != 0, "clock-gate control fields propagate");
    sim.check((dut->clk_gate_ctrl_flat_o & (uint64_t{1} << 5)) == 0,
              "public RBOX clock-gate-disable bit preserves original forced-low behavior");

    dut->noc_err_int_srcs_i = 0x5;
    sim.tick();
    sim.check((dut->noc_all_err_int_srcs_o & 0x5) == 0x5, "NOC error status passes to combiner");
    sim.check(dut->ioshire_noc_err_int_o == 1, "NOC error interrupt asserted");
    dut->noc_err_int_srcs_i = 0;

    dut->esr_icache_prefetch_done_stim_i = 0xa;
    dut->esr_and_or_tree_l0_flat_stim_i = 0x155555555ull;
    dut->debug_and_or_tree_l2_stim_i = 0x155;
    dut->bpam_rc_tbox_ack_flat_stim_i = 0xa5;
    dut->esr_pwr_ctrl_glb_nsleepout_stim_i = 0x5;
    dut->esr_pwr_ctrl_neigh_nsleepout_stim_i = 0x1357;
    dut->esr_pll_busy_stim_i = 1;
    dut->esr_pll_rdata_stim_i = 0x5aa5;
    dut->esr_pll_rrdy_stim_i = 1;
    dut->esr_pll_lock_stim_i = 0;
    dut->esr_dll_dly_est_sts_flat_stim_i = 0x155555555ull;
    dut->esr_dll_busy_stim_i = 1;
    dut->esr_dll_rdata_stim_i = 0xa55a;
    dut->esr_dll_rrdy_stim_i = 1;
    dut->esr_dll_lock_stim_i = 0;
    dut->status_monitor_bank_sel_i = 2;
    sim.tick();
    sim.check(dut->debug_and_or_tree_l2_flat_o != 0,
              "retained debug/status stimulus reaches and-or daisy-chain output");
    clear_inputs(dut);

    dut->neigh_sc_err_detected_i = 0x1;
    sim.tick();
    sim.tick();
    sim.check(dut->ioshire_log_err_int_o == 1, "log-error interrupt aggregates neighborhood error");
    dut->neigh_sc_err_detected_i = 0;

    dut->coop_slv_valid_i = 0x1;
    sim.tick();
    sim.check((dut->coop_slv_valid_o & 0x7) == 0, "coop bus excludes source neighborhood from its own receive set");
    sim.check(((dut->coop_slv_valid_o >> 3) & 0x1) == 1, "coop bus forwards source 0 to neighborhood 1");
    dut->coop_slv_valid_i = 0;

    dut->coop_done_id_i = 0x3210;
    dut->coop_done_valid_i = 0x12;
    sim.tick();
    sim.check(dut->coop_done_valid_o != 0, "coop done valid fanout forwards valid traffic");
    sim.check(dut->coop_done_id_o != 0, "coop done ID fanout forwards nonzero IDs");
    dut->coop_done_id_i = 0;
    dut->coop_done_valid_i = 0;

    dut->icache_req_valid_i = 0x1;
    dut->icache_resp_ready_i = 0xf;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check((dut->icache_req_ready_o & 0x1) != 0, "ICache memory request ready smoke check");
    dut->icache_req_valid_i = 0;

    dut->neigh0_req_valid_i = 1;
    dut->neigh0_req_dest_i = 0x1;
    dut->neigh0_req_addr_i = 0x1000;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check(dut->neigh_sc_req_ready_o != 0, "cache request ready smoke check");
    dut->neigh0_req_valid_i = 0;

    apb_write(sim, kApbShire, kAddrShireConfig, kConfigEnableAll & ~(uint64_t{1} << 17));
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(dut->rst_rbox_no_o == 0, "public RBOX reset asserts immediately when rbox_en is cleared");
    apb_write(sim, kApbShire, kAddrShireConfig, kConfigEnableAll);
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(dut->rst_rbox_no_o == 1, "public RBOX reset releases immediately when rbox_en is restored");

    dut->dft_scanmode_i = 1;
    dut->dft_scan_reset_ni = 0;
    dut->dft_sram_clk_override_i = 1;
    dut->dft_ram_rei_i = 1;
    dut->dft_ram_wei_i = 1;
    dut->dft_mbist_en_i = 1;
    sim.tick();
    sim.check((dut->dft_hv_flat_o & 0x1f) == 0x17,
              "DFT struct propagates scan reset, SRAM override, and RAM inhibit fields");
    sim.check((dut->rst_c_shire_no_o & 0xf) == 0,
              "DFT scan reset bypass asserts reset-sync outputs");
    sim.check(dut->rst_rbox_no_o == 1,
              "public RBOX reset remains raw and ignores DFT scan reset like the original");
    sim.check(dut->bpam_run_control_neigh_flat_o[0] != 0,
              "DFT-backed run-control fanout reaches neighborhood leaves");

    return sim.finish();
}

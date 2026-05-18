// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_channel_wrap_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

namespace {
constexpr uint8_t kShireId = 0x2a;
constexpr uint64_t kConfigEnableAll =
    (uint64_t(3) << 24) | (uint64_t(2) << 22) | (uint64_t(1) << 20) |
    (uint64_t(1) << 17) | (uint64_t(0xf) << 13) | (uint64_t(0xf) << 9) |
    (uint64_t(1) << 8) | uint64_t(kShireId);

uint64_t make_addr_id(uint8_t shire_id, uint32_t cls, uint32_t block, uint32_t bank_or_neigh,
                      uint32_t reg, uint32_t pp = 3) {
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

void set_wdata(Vshire_channel_wrap_tb* dut, uint64_t lo) {
    for (int i = 0; i < 8; ++i) dut->sys_wdata_stim_i[i] = 0;
    dut->sys_wdata_stim_i[0] = static_cast<uint32_t>(lo);
    dut->sys_wdata_stim_i[1] = static_cast<uint32_t>(lo >> 32);
}

void clear_inputs(Vshire_channel_wrap_tb* dut) {
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
    dut->shire_id_reset_val_i = kShireId;
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
    dut->sys_id_stim_i = 0;
    dut->sys_addr_stim_i = 0;
    dut->sys_len_stim_i = 0;
    dut->sys_size_stim_i = 3;
    set_wdata(dut, 0);
}

bool wait_for_bvalid(SimCtrl<Vshire_channel_wrap_tb>& sim, int max_cycles = 200) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sim.dut->sys_axi_b_valid_obs_o) return true;
    }
    return false;
}

bool wait_for_neigh_psel(SimCtrl<Vshire_channel_wrap_tb>& sim, uint32_t mask, int max_cycles = 200) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if ((sim.dut->apb_neigh_psel_o & mask) == mask) return true;
    }
    return false;
}

void issue_write(SimCtrl<Vshire_channel_wrap_tb>& sim, uint64_t addr, uint64_t data, uint32_t id) {
    auto* dut = sim.dut.get();
    dut->sys_id_stim_i = id;
    dut->sys_addr_stim_i = addr;
    dut->sys_len_stim_i = 0;
    dut->sys_size_stim_i = 3;
    set_wdata(dut, data);
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
}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vshire_channel_wrap_tb> sim(argc, argv);
    auto* dut = sim.dut.get();
    clear_inputs(dut);
    sim.reset(6);
    clear_inputs(dut);
    for (int i = 0; i < 40; ++i) sim.tick();

    sim.check(dut->rst_system_lv_no_o == 1, "NOC reset synchronizer releases");
    sim.check(dut->rst_system_debug_lv_no_o == 1, "debug NOC reset synchronizer releases");

    dut->dft_scanmode_i = 1;
    dut->dft_scan_reset_ni = 0;
    sim.tick();
    sim.check(dut->rst_system_lv_no_o == 0, "DFT scan reset drives low-voltage reset low");
    dut->dft_scan_reset_ni = 1;
    sim.tick();
    sim.check(dut->rst_system_lv_no_o == 1, "DFT scan reset bypass releases low-voltage reset");
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_ni = 1;
    sim.tick();

    sim.check((dut->shire_id_o & 0xffu) == kShireId, "shire ID reset value reaches native channel");
    sim.check((dut->pwr_ctrl_glb_nsleepin_o & 0xf) == 0xf, "global power nsleepin follows reset/default mask");
    sim.check((dut->pwr_ctrl_glb_iso_o & 0xf) == 0, "global isolation released after cold reset");
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf, "warm reset fanout released by default");
    sim.check((dut->plic_meip_obs_o & 0xf) == 0xf, "MEIP interrupt fans out to all neighborhoods");
    sim.check((dut->plic_seip_obs_o & 0xf) == 0x0, "SEIP interrupt low fans out to all neighborhoods");

    dut->rst_c_ext_ni = 0;
    for (int i = 0; i < 3; ++i) sim.tick();
    sim.check((dut->rst_c_shire_no_o & 0xf) == 0x0, "cold reset asserts cold shire resets");
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "cold reset does not alter raw warm-reset neighborhood fanout");
    dut->rst_c_ext_ni = 1;
    for (int i = 0; i < 40; ++i) sim.tick();

    dut->dft_ram_rei_i = 1;
    dut->dft_ram_wei_i = 1;
    for (int i = 0; i < 40; ++i) sim.tick();
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "dmactive/ndmreset do not alter raw warm-reset neighborhood fanout");
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check((dut->dmctrl_to_neigh_flat_o & 0x3u) == 0x0,
              "neighborhood DM control clears after dmactive/ndmreset deassert");

    dut->rst_w_ext_ni = 0;
    for (int i = 0; i < 3; ++i) sim.tick();
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0x0,
              "external warm reset drives raw neighborhood fanout low");
    dut->rst_w_ext_ni = 1;
    for (int i = 0; i < 40; ++i) sim.tick();
    sim.check((dut->rst_warm_to_neigh_no_o & 0xf) == 0xf,
              "raw neighborhood warm-reset fanout releases after external warm reset");

    dut->dft_scanmode_i = 1;
    dut->dft_scan_reset_ni = 1;
    dut->dft_sram_clk_override_i = 1;
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 1;
    sim.tick();
    sim.check((dut->dft_hv_flat_o & 0x1fu) == 0x1du,
              "DFT struct fields propagate to retained high-voltage DFT seam");
    dut->dft_scanmode_i = 0;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_wei_i = 0;
    sim.tick();

    sim.check((dut->dft_hv_flat_o & 0x1fu) == 0x08u,
              "DFT struct returns to normal scan-reset-high value after test controls clear");

    dut->noc_err_int_srcs_i = 0x155;
    dut->neigh_sc_err_detected_i = 0xa;
    dut->neigh_sc_err_logged_i = 0x5;
    dut->coop_slv_valid_i = 0x5;
    dut->coop_done_valid_i = 0xa;
    dut->coop_done_id_i = 0x12345;
    dut->flb_neigh_l2_req_valid_i = 0xf;
    dut->flb_neigh_l2_req_data_i = 0x1111222233334444ull;
    dut->icache_req_valid_i = 0xf;
    dut->icache_req_write_i = 0x5;
    dut->icache_req_addr_i = 0x1234;
    dut->icache_resp_ready_i = 0xf;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check((dut->noc_all_err_int_srcs_o & 0x3ffu) == 0x155u, "NOC error sources fan out to status combiner");
    sim.check(dut->ioshire_noc_err_int_o == 1, "NOC error interrupt asserts from nonzero sources");
    sim.check(dut->coop_slv_valid_o != 0, "cooperative TLoad slave-valid fanout toggles");
    sim.check(dut->coop_done_valid_o != 0, "cooperative TLoad done-valid fanout toggles");
    sim.check(dut->flb_l2_neigh_resp_valid_o != 0, "FLB neighborhood response seam toggles under requests");
    sim.check(dut->icache_req_ready_o != 0, "Icache request ready seam is observable");

    dut->sys_id_stim_i = 3;
    dut->sys_addr_stim_i = make_addr_id(0, 3, 2, 0, 0x001);
    set_wdata(dut, kConfigEnableAll);
    dut->sys_axi_aw_vcvalid_stim_i = 2;
    dut->sys_axi_w_vcvalid_stim_i = 2;
    dut->sys_axi_aw_valid_stim_i = 1;
    dut->sys_axi_w_valid_stim_i = 1;
    sim.check(dut->sys_axi_aw_ready_obs_o == 1, "SYS AXI write-address ready is exposed");
    sim.check(dut->sys_axi_w_ready_obs_o == 1, "SYS AXI write-data ready is exposed");
    sim.tick();
    dut->sys_axi_aw_valid_stim_i = 0;
    dut->sys_axi_w_valid_stim_i = 0;
    dut->sys_axi_aw_vcvalid_stim_i = 0;
    dut->sys_axi_w_vcvalid_stim_i = 0;
    sim.check(wait_for_bvalid(sim, 120), "SYS-to-SBM configuration write produces response");
    sim.check(dut->sys_axi_b_valid_obs_o == 1, "SYS-to-SBM configuration write response is visible");

    dut->noc_err_int_srcs_i = 0;
    dut->coop_slv_valid_i = 0;
    dut->coop_done_valid_i = 0;
    dut->flb_neigh_l2_req_valid_i = 0;
    dut->icache_req_valid_i = 0;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.check((dut->noc_all_err_int_srcs_o & 0x3ffu) == 0, "NOC error sources clear when inputs clear");
    sim.check(dut->ioshire_noc_err_int_o == 0, "NOC error interrupt clears when sources clear");

    return sim.finish();
}

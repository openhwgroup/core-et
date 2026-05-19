// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

namespace {
constexpr uint8_t kShireId = 0x2a;
constexpr uint32_t kRegShireConfig = 0x001;
constexpr uint32_t kRegRamCfg2 = 0x055;
constexpr uint64_t kConfigEnableAll =
    (uint64_t(3) << 24) | (uint64_t(2) << 22) | (uint64_t(1) << 20) |
    (uint64_t(1) << 17) | (uint64_t(0xf) << 13) | (uint64_t(0xf) << 9) |
    (uint64_t(1) << 8) | uint64_t(kShireId);

uint32_t xs(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

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

uint64_t shire_reg_addr(uint32_t reg) { return make_addr_id(kShireId, 3, 2, 0, reg); }

void set_wdata(Vcosim_shire_top_tb* d, uint64_t lo, uint64_t addr = 0) {
    const int lane = static_cast<int>((addr >> 3) & 0x3u);
    for (int i = 0; i < 8; ++i) d->sys_wdata_stim_i[i] = 0;
    d->sys_wdata_stim_i[2 * lane] = static_cast<uint32_t>(lo);
    d->sys_wdata_stim_i[2 * lane + 1] = static_cast<uint32_t>(lo >> 32);
}

void clear_inputs(Vcosim_shire_top_tb* d) {
    d->rst_cold_ext_ni = 1;
    d->rst_warm_ext_ni = 1;
    d->rst_system_ext_ni = 1;
    d->rst_system_debug_ext_ni = 1;
    d->dft_scanmode_i = 0;
    d->dft_scan_reset_ni = 1;
    d->dft_sram_clk_i = 0;
    d->dft_sram_clk_override_i = 0;
    d->dft_ram_rei_i = 0;
    d->dft_ram_wei_i = 0;
    d->dft_mbist_en_i = 0;
    d->shire_id_reset_val_i = kShireId;
    d->shire_phy_id_i = 3;
    d->plic_mtip_i = 0;
    d->plic_meip_i = 0;
    d->plic_seip_i = 0;
    d->noc_err_int_srcs_i = 0;
    d->debug_and_or_tree_l2_stim_i = 0;
    d->sys_axi_ar_valid_stim_i = 0;
    d->sys_axi_aw_valid_stim_i = 0;
    d->sys_axi_w_valid_stim_i = 0;
    d->sys_axi_b_ready_stim_i = 1;
    d->sys_axi_r_ready_stim_i = 1;
    d->sys_axi_aw_vcvalid_stim_i = 0;
    d->sys_axi_w_vcvalid_stim_i = 0;
    d->sys_id_stim_i = 0;
    d->sys_addr_stim_i = 0;
    d->sys_len_stim_i = 0;
    d->sys_size_stim_i = 3;
    set_wdata(d, 0);
}

void compare_top(CosimCtrl<Vcosim_shire_top_tb>& sim) {
    auto* d = sim.dut.get();
    // Compare retained top-level observations that the original public top
    // exposed as direct shell wiring and that remain meaningful with the
    // native top's third-party surfaces omitted/held idle.
    sim.compare("rst_c_shire_no", d->orig_rst_c_shire_no_o, d->new_rst_c_shire_no_o);
    sim.compare("rst_w_shire_no", d->orig_rst_w_shire_no_o, d->new_rst_w_shire_no_o);
    sim.compare("rst_d_shire_no", d->orig_rst_d_shire_no_o, d->new_rst_d_shire_no_o);
    sim.compare("rst_warm_to_neigh_no", d->orig_rst_warm_to_neigh_no_o,
                d->new_rst_warm_to_neigh_no_o);
    sim.compare("rst_w_icache_no", d->orig_rst_w_icache_no_o, d->new_rst_w_icache_no_o);
    sim.compare("rst_system_lv_no", d->orig_rst_system_lv_no_o, d->new_rst_system_lv_no_o);
    sim.compare("rst_system_debug_lv_no", d->orig_rst_system_debug_lv_no_o,
                d->new_rst_system_debug_lv_no_o);
    sim.compare("clk_neigh_obs", d->orig_clk_neigh_obs_o, d->new_clk_neigh_obs_o);
    sim.compare("clk_shire_obs", d->orig_clk_shire_obs_o, d->new_clk_shire_obs_o);
    sim.compare("shire_id", d->orig_shire_id_o, d->new_shire_id_o);
    sim.compare("tbox_id", d->orig_tbox_id_o, d->new_tbox_id_o);
    sim.compare("tbox_en", d->orig_tbox_en_o, d->new_tbox_en_o);
    sim.compare("plic_meip_to_neigh", d->orig_plic_meip_to_neigh_o,
                d->new_plic_meip_to_neigh_o);
    sim.compare("plic_seip_to_neigh", d->orig_plic_seip_to_neigh_o,
                d->new_plic_seip_to_neigh_o);
    sim.compare("int_mtip_flat", d->orig_int_mtip_flat_o, d->new_int_mtip_flat_o);
    sim.compare("apb_neigh_psel", d->orig_apb_neigh_psel_o, d->new_apb_neigh_psel_o);
    sim.compare("apb_neigh_pwrite", d->orig_apb_neigh_pwrite_o,
                d->new_apb_neigh_pwrite_o);
    sim.compare("apb_neigh0_paddr", d->orig_apb_neigh0_paddr_o,
                d->new_apb_neigh0_paddr_o);
    sim.compare("apb_neigh0_pwdata", d->orig_apb_neigh0_pwdata_o,
                d->new_apb_neigh0_pwdata_o);
    sim.compare("apb_channel_psel", d->orig_apb_channel_psel_o, d->new_apb_channel_psel_o);
    sim.compare("apb_channel_pwrite", d->orig_apb_channel_pwrite_o,
                d->new_apb_channel_pwrite_o);
    sim.compare("neigh_sc_req_ready", d->orig_neigh_sc_req_ready_o,
                d->new_neigh_sc_req_ready_o);
    sim.compare("neigh_sc_rsp_valid", d->orig_neigh_sc_rsp_valid_o,
                d->new_neigh_sc_rsp_valid_o);
    sim.compare("icache_req_ready", d->orig_icache_req_ready_o, d->new_icache_req_ready_o);
    sim.compare("icache_resp_valid", d->orig_icache_resp_valid_o,
                d->new_icache_resp_valid_o);
    sim.compare("flb_l2_neigh_resp_valid", d->orig_flb_l2_neigh_resp_valid_o,
                d->new_flb_l2_neigh_resp_valid_o);
    sim.compare("flb_l2_neigh_resp_data", d->orig_flb_l2_neigh_resp_data_o,
                d->new_flb_l2_neigh_resp_data_o);
    sim.compare("uc_to_neigh_fcc", d->orig_uc_to_neigh_fcc_o, d->new_uc_to_neigh_fcc_o);
    sim.compare("uc_to_neigh_fcc_target", d->orig_uc_to_neigh_fcc_target_o,
                d->new_uc_to_neigh_fcc_target_o);
    sim.compare("pwr_ctrl_glb_nsleepin", d->orig_pwr_ctrl_glb_nsleepin_o,
                d->new_pwr_ctrl_glb_nsleepin_o);
    sim.compare("pwr_ctrl_glb_iso", d->orig_pwr_ctrl_glb_iso_o,
                d->new_pwr_ctrl_glb_iso_o);
    sim.compare("pwr_ctrl_neigh_nsleepin", d->orig_pwr_ctrl_neigh_nsleepin_o,
                d->new_pwr_ctrl_neigh_nsleepin_o);
    sim.compare("pwr_ctrl_neigh_iso", d->orig_pwr_ctrl_neigh_iso_o,
                d->new_pwr_ctrl_neigh_iso_o);
    sim.compare("ram_cfg", d->orig_ram_cfg_flat_o, d->new_ram_cfg_flat_o);
    sim.compare("dft_hv", d->orig_dft_hv_flat_o, d->new_dft_hv_flat_o);
    sim.compare("debug_and_or_tree_l2", d->orig_debug_and_or_tree_l2_flat_o,
                d->new_debug_and_or_tree_l2_flat_o);
    sim.compare("ioshire_combined_err_int", d->orig_ioshire_combined_err_int_o,
                d->new_ioshire_combined_err_int_o);
    sim.compare("ioshire_noc_err_int", d->orig_ioshire_noc_err_int_o,
                d->new_ioshire_noc_err_int_o);
    sim.compare("noc_all_err_int_srcs", d->orig_noc_all_err_int_srcs_o,
                d->new_noc_all_err_int_srcs_o);
    sim.compare("coop_slv_valid", d->orig_coop_slv_valid_o, d->new_coop_slv_valid_o);
    sim.compare("coop_done_valid", d->orig_coop_done_valid_o, d->new_coop_done_valid_o);
    sim.compare("sys_axi_ar_ready", d->orig_sys_axi_ar_ready_obs_o,
                d->new_sys_axi_ar_ready_obs_o);
    sim.compare("sys_axi_aw_ready", d->orig_sys_axi_aw_ready_obs_o,
                d->new_sys_axi_aw_ready_obs_o);
    sim.compare("sys_axi_w_ready", d->orig_sys_axi_w_ready_obs_o,
                d->new_sys_axi_w_ready_obs_o);
    sim.compare("sys_axi_b_valid", d->orig_sys_axi_b_valid_obs_o,
                d->new_sys_axi_b_valid_obs_o);
    sim.compare("sys_axi_b_id", d->orig_sys_axi_b_id_obs_o, d->new_sys_axi_b_id_obs_o);
    sim.compare("sys_axi_b_resp", d->orig_sys_axi_b_resp_obs_o,
                d->new_sys_axi_b_resp_obs_o);
    sim.compare("sys_axi_r_valid", d->orig_sys_axi_r_valid_obs_o,
                d->new_sys_axi_r_valid_obs_o);
    sim.compare("sys_axi_r_data_lo", d->orig_sys_axi_r_data_lo_obs_o,
                d->new_sys_axi_r_data_lo_obs_o);
    sim.compare("sys_axi_aw_credit", d->orig_sys_axi_aw_credit_obs_o,
                d->new_sys_axi_aw_credit_obs_o);
}

void tick_compare(CosimCtrl<Vcosim_shire_top_tb>& sim) {
    sim.tick();
    compare_top(sim);
}

bool wait_bvalid_low(CosimCtrl<Vcosim_shire_top_tb>& sim, int max_cycles = 80) {
    for (int i = 0; i < max_cycles; ++i) {
        if (!sim.dut->new_sys_axi_b_valid_obs_o && !sim.dut->orig_sys_axi_b_valid_obs_o) return true;
        tick_compare(sim);
    }
    return false;
}

void issue_write(CosimCtrl<Vcosim_shire_top_tb>& sim, uint64_t addr, uint64_t data, uint32_t id) {
    auto* d = sim.dut.get();
    wait_bvalid_low(sim);
    d->sys_id_stim_i = id;
    d->sys_addr_stim_i = addr;
    d->sys_len_stim_i = 0;
    d->sys_size_stim_i = 3;
    set_wdata(d, data, addr);
    d->sys_axi_aw_vcvalid_stim_i = 2;
    d->sys_axi_w_vcvalid_stim_i = 2;
    d->sys_axi_aw_valid_stim_i = 1;
    d->sys_axi_w_valid_stim_i = 1;
    for (int i = 0; i < 80; ++i) {
        tick_compare(sim);
        if (d->new_sys_axi_aw_ready_obs_o && d->new_sys_axi_w_ready_obs_o) break;
    }
    d->sys_axi_aw_valid_stim_i = 0;
    d->sys_axi_w_valid_stim_i = 0;
    d->sys_axi_aw_vcvalid_stim_i = 0;
    d->sys_axi_w_vcvalid_stim_i = 0;
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_top_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset(6);
    clear_inputs(d);

    for (int i = 0; i < 64; ++i) tick_compare(sim);

    d->plic_mtip_i = 1;
    d->plic_meip_i = 1;
    d->plic_seip_i = 1;
    d->noc_err_int_srcs_i = 0x5a;
    d->debug_and_or_tree_l2_stim_i = 0x33;
    d->dft_scanmode_i = 1;
    d->dft_scan_reset_ni = 1;
    d->dft_sram_clk_override_i = 1;
    d->dft_ram_rei_i = 1;
    d->dft_ram_wei_i = 1;
    d->dft_mbist_en_i = 1;
    for (int i = 0; i < 16; ++i) tick_compare(sim);

    d->dft_scanmode_i = 0;
    d->dft_sram_clk_override_i = 0;
    d->dft_ram_rei_i = 0;
    d->dft_ram_wei_i = 0;
    d->dft_mbist_en_i = 0;
    d->plic_mtip_i = 0;
    d->plic_meip_i = 0;
    d->plic_seip_i = 0;
    d->noc_err_int_srcs_i = 0;
    for (int i = 0; i < 64; ++i) tick_compare(sim);

    uint32_t seed = 0x51a7e123u;
    for (int i = 0; i < 12000; ++i) {
        const uint32_t r = xs(seed);
        d->dft_sram_clk_i = (r >> 0) & 1u;
        d->plic_mtip_i = (r >> 1) & 1u;
        d->plic_meip_i = (r >> 2) & 1u;
        d->plic_seip_i = (r >> 3) & 1u;
        d->noc_err_int_srcs_i = (r >> 4) & 0xffu;
        d->debug_and_or_tree_l2_stim_i = (r >> 12) & 0xffu;
        d->sys_axi_b_ready_stim_i = 1;
        d->sys_axi_r_ready_stim_i = 1;
        d->sys_id_stim_i = (r >> 20) & 0x1fu;
        d->sys_addr_stim_i = shire_reg_addr((r >> 8) & 0x7fu);
        d->sys_len_stim_i = 0;
        d->sys_size_stim_i = 3;
        d->sys_axi_ar_valid_stim_i = 0;
        d->sys_axi_aw_valid_stim_i = 0;
        d->sys_axi_w_valid_stim_i = 0;
        d->sys_axi_aw_vcvalid_stim_i = 0;
        d->sys_axi_w_vcvalid_stim_i = 0;
        tick_compare(sim);
    }

    return sim.finish();
}

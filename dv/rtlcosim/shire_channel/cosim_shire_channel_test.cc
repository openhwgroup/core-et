// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_channel_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

namespace {
constexpr uint8_t kApbShire = 4;
constexpr uint32_t kAddrShireConfig = 0x0c001;
constexpr uint32_t kAddrIpiTrigger = 0x0c012;
constexpr uint32_t kAddrIpiRedirect = 0x0c013;
constexpr uint32_t kAddrMtimeTarget = 0x0c043;
constexpr uint32_t kAddrRamCfg2 = 0x0c055;
constexpr uint32_t kAddrClkGateCtrl = 0x0c062;
constexpr uint64_t kConfigEnableAll =
    (uint64_t(3) << 24) | (uint64_t(2) << 22) | (uint64_t(1) << 20) |
    (uint64_t(1) << 17) | (uint64_t(0xf) << 13) | (uint64_t(0xf) << 9) |
    (uint64_t(1) << 8) | uint64_t(0x2a);

uint32_t xs(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

void clear_inputs(Vcosim_shire_channel_tb* d) {
    d->rst_c_ext_ni = 1;
    d->rst_w_ext_ni = 1;
    d->rst_d_ext_ni = 1;
    d->rst_noc_ext_ni = 1;
    d->rst_w_icache_ext_ni = 0xf;
    d->dft_scanmode_i = 0;
    d->dft_scan_reset_ni = 1;
    d->dft_sram_clk_i = 0;
    d->dft_sram_clk_override_i = 0;
    d->dft_ram_rei_i = 0;
    d->dft_ram_wei_i = 0;
    d->dft_mbist_en_i = 0;
    d->shire_id_reset_val_i = 0x11;
    d->shire_phy_id_i = 0x03;
    d->apb_sel_i = 0;
    d->apb_paddr_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwdata_i = 0;
    d->noc_err_int_srcs_i = 0;
    d->neigh_sc_err_detected_i = 0;
    d->neigh_sc_err_logged_i = 0;
    d->flb_neigh_l2_req_valid_i = 0;
    d->flb_neigh_l2_req_data_i = 0;
    d->neigh0_req_valid_i = 0;
    d->neigh0_req_dest_i = 0;
    d->neigh0_req_addr_i = 0;
    d->neigh0_req_opcode_i = 1;
    d->neigh0_req_size_i = 6;
    d->neigh0_req_qwen_i = 0xf;
    d->icache_req_valid_i = 0;
    d->icache_req_write_i = 0;
    d->icache_req_addr_i = 0;
    d->icache_resp_ready_i = 0xf;
    d->coop_slv_valid_i = 0;
    d->coop_done_id_i = 0;
    d->coop_done_valid_i = 0;
}

void compare_retained(CosimCtrl<Vcosim_shire_channel_tb>& sim) {
    auto* d = sim.dut.get();
    sim.compare("rst_c_shire", d->orig_rst_c_shire_no_o, d->new_rst_c_shire_no_o);
    sim.compare("rst_w_shire", d->orig_rst_w_shire_no_o, d->new_rst_w_shire_no_o);
    sim.compare("rst_d_shire", d->orig_rst_d_shire_no_o, d->new_rst_d_shire_no_o);
    sim.compare("rst_c_shire_scs", d->orig_rst_c_shire_scs_no_o,
                d->new_rst_c_shire_scs_no_o);
    sim.compare("rst_sc", d->orig_rst_sc_no_o, d->new_rst_sc_no_o);
    sim.compare("rst_rbox", d->orig_rst_rbox_no_o, d->new_rst_rbox_no_o);
    sim.compare("shire_id", d->orig_shire_id_o, d->new_shire_id_o);
    sim.compare("tbox_id", d->orig_tbox_id_o, d->new_tbox_id_o);
    sim.compare("tbox_en", d->orig_tbox_en_o, d->new_tbox_en_o);
    sim.compare("thread0_en", d->orig_esr_thread0_enable_o, d->new_esr_thread0_enable_o);
    sim.compare("thread1_en", d->orig_esr_thread1_enable_o, d->new_esr_thread1_enable_o);
    for (int i = 0; i < 4; ++i) {
        sim.compare("minion_features_" + std::to_string(i),
                    d->orig_esr_minion_features_o[i], d->new_esr_minion_features_o[i]);
        sim.compare("icache_prefetch_conf_" + std::to_string(i),
                    d->orig_icache_prefetch_conf_o[i], d->new_icache_prefetch_conf_o[i]);
        sim.compare("bpam_run_control_neigh_" + std::to_string(i),
                    d->orig_bpam_run_control_neigh_o[i], d->new_bpam_run_control_neigh_o[i]);
    }
    sim.compare("icache_prefetch_start", d->orig_icache_prefetch_start_o,
                d->new_icache_prefetch_start_o);
    sim.compare("ipi_trigger", d->orig_esr_ipi_trigger_o, d->new_esr_ipi_trigger_o);
    sim.compare("ipi_redirect", d->orig_esr_ipi_redirect_trigger_o,
                d->new_esr_ipi_redirect_trigger_o);
    sim.compare("mtime_target", d->orig_esr_mtime_local_target_o,
                d->new_esr_mtime_local_target_o);
    sim.compare("coop_mode", d->orig_esr_shire_coop_mode_o, d->new_esr_shire_coop_mode_o);
    sim.compare("debug_and_or_tree_l2", d->orig_debug_and_or_tree_l2_o,
                d->new_debug_and_or_tree_l2_o);
    sim.compare("log_err_int", d->orig_ioshire_log_err_int_o, d->new_ioshire_log_err_int_o);
    sim.compare("uc_fcc", d->orig_uc_to_neigh_fcc_o, d->new_uc_to_neigh_fcc_o);
    sim.compare("uc_fcc_target", d->orig_uc_to_neigh_fcc_target_o,
                d->new_uc_to_neigh_fcc_target_o);
    sim.compare("flb_rsp_valid", d->orig_flb_l2_neigh_resp_valid_o,
                d->new_flb_l2_neigh_resp_valid_o);
    sim.compare("flb_rsp_data", d->orig_flb_l2_neigh_resp_data_o,
                d->new_flb_l2_neigh_resp_data_o);
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
    sim.compare("sbm_enable_read", d->orig_sbm_enable_read_o, d->new_sbm_enable_read_o);
    sim.compare("sbm_enable_write", d->orig_sbm_enable_write_o, d->new_sbm_enable_write_o);
    sim.compare("neigh_sc_req_ready", d->orig_neigh_sc_req_ready_o,
                d->new_neigh_sc_req_ready_o);
    sim.compare("neigh_sc_rsp_valid", d->orig_neigh_sc_rsp_valid_o,
                d->new_neigh_sc_rsp_valid_o);
    sim.compare("icache_req_ready", d->orig_icache_req_ready_o, d->new_icache_req_ready_o);
    sim.compare("icache_resp_valid", d->orig_icache_resp_valid_o,
                d->new_icache_resp_valid_o);
    sim.compare("l2hpf_req_valid", d->orig_l2hpf_req_valid_o, d->new_l2hpf_req_valid_o);
    sim.compare("sc_trace_valid", d->orig_sc_trace_valid_o, d->new_sc_trace_valid_o);
    sim.compare("clk_gate_ctrl", d->orig_clk_gate_ctrl_flat_o, d->new_clk_gate_ctrl_flat_o);
    sim.compare("debug_clk_gate_ctrl", d->orig_debug_clk_gate_ctrl_o,
                d->new_debug_clk_gate_ctrl_o);
    sim.compare("neigh_sc_rsp_info", d->orig_neigh_sc_rsp_info_flat_o,
                d->new_neigh_sc_rsp_info_flat_o);
    sim.compare("apb_pready_all", d->orig_apb_pready_all_o, d->new_apb_pready_all_o);
    sim.compare("apb_pslverr_all", d->orig_apb_pslverr_all_o, d->new_apb_pslverr_all_o);
    sim.compare("apb_prdata_all", d->orig_apb_prdata_all_flat_o,
                d->new_apb_prdata_all_flat_o);
    sim.compare("pwr_glb_nsleepin", d->orig_pwr_ctrl_glb_nsleepin_o,
                d->new_pwr_ctrl_glb_nsleepin_o);
    sim.compare("pwr_glb_iso", d->orig_pwr_ctrl_glb_iso_o, d->new_pwr_ctrl_glb_iso_o);
    sim.compare("pwr_neigh_nsleepin", d->orig_pwr_ctrl_neigh_nsleepin_o,
                d->new_pwr_ctrl_neigh_nsleepin_o);
    sim.compare("pwr_neigh_iso", d->orig_pwr_ctrl_neigh_iso_o,
                d->new_pwr_ctrl_neigh_iso_o);
    sim.compare("pll_ctrl", d->orig_pll_ctrl_flat_o, d->new_pll_ctrl_flat_o);
    sim.compare("pll_conf", d->orig_pll_conf_o, d->new_pll_conf_o);
    sim.compare("clk_dly_ctl", d->orig_clk_dly_ctl_flat_o, d->new_clk_dly_ctl_flat_o);
    sim.compare("dll_dly_est_ctl", d->orig_dll_dly_est_ctl_flat_o,
                d->new_dll_dly_est_ctl_flat_o);
    sim.compare("shire_ctrl_clockmux", d->orig_shire_ctrl_clockmux_flat_o,
                d->new_shire_ctrl_clockmux_flat_o);
    sim.compare("dll_ctrl", d->orig_dll_ctrl_flat_o, d->new_dll_ctrl_flat_o);
    sim.compare("dll_conf", d->orig_dll_conf_o, d->new_dll_conf_o);
    sim.compare("ram_cfg", d->orig_ram_cfg_flat_o, d->new_ram_cfg_flat_o);
    sim.compare("to_l3_axi_ar", d->orig_to_l3_axi_ar_flat_o, d->new_to_l3_axi_ar_flat_o);
    sim.compare("to_l3_axi_ar_valid", d->orig_to_l3_axi_ar_valid_o,
                d->new_to_l3_axi_ar_valid_o);
    sim.compare("to_l3_axi_aw", d->orig_to_l3_axi_aw_flat_o, d->new_to_l3_axi_aw_flat_o);
    sim.compare("to_l3_axi_aw_valid", d->orig_to_l3_axi_aw_valid_o,
                d->new_to_l3_axi_aw_valid_o);
    sim.compare("to_l3_axi_w", d->orig_to_l3_axi_w_flat_o, d->new_to_l3_axi_w_flat_o);
    sim.compare("to_l3_axi_w_valid", d->orig_to_l3_axi_w_valid_o,
                d->new_to_l3_axi_w_valid_o);
    sim.compare("to_l3_axi_b_ready", d->orig_to_l3_axi_b_ready_o,
                d->new_to_l3_axi_b_ready_o);
    sim.compare("to_l3_axi_r_ready", d->orig_to_l3_axi_r_ready_o,
                d->new_to_l3_axi_r_ready_o);
    sim.compare("to_sys_axi_ar", d->orig_to_sys_axi_ar_flat_o,
                d->new_to_sys_axi_ar_flat_o);
    sim.compare("to_sys_axi_ar_valid", d->orig_to_sys_axi_ar_valid_o,
                d->new_to_sys_axi_ar_valid_o);
    sim.compare("to_sys_axi_aw", d->orig_to_sys_axi_aw_flat_o,
                d->new_to_sys_axi_aw_flat_o);
    sim.compare("to_sys_axi_aw_valid", d->orig_to_sys_axi_aw_valid_o,
                d->new_to_sys_axi_aw_valid_o);
    sim.compare("to_sys_axi_w", d->orig_to_sys_axi_w_flat_o, d->new_to_sys_axi_w_flat_o);
    sim.compare("to_sys_axi_w_valid", d->orig_to_sys_axi_w_valid_o,
                d->new_to_sys_axi_w_valid_o);
    sim.compare("to_sys_axi_b_ready", d->orig_to_sys_axi_b_ready_o,
                d->new_to_sys_axi_b_ready_o);
    sim.compare("to_sys_axi_r_ready", d->orig_to_sys_axi_r_ready_o,
                d->new_to_sys_axi_r_ready_o);
    sim.compare("l3_axi_ar_ready", d->orig_l3_axi_ar_ready_o, d->new_l3_axi_ar_ready_o);
    sim.compare("l3_axi_aw_ready", d->orig_l3_axi_aw_ready_o, d->new_l3_axi_aw_ready_o);
    sim.compare("l3_axi_w_ready", d->orig_l3_axi_w_ready_o, d->new_l3_axi_w_ready_o);
    sim.compare("l3_axi_b", d->orig_l3_axi_b_flat_o, d->new_l3_axi_b_flat_o);
    sim.compare("l3_axi_b_valid", d->orig_l3_axi_b_valid_o, d->new_l3_axi_b_valid_o);
    sim.compare("l3_axi_r", d->orig_l3_axi_r_flat_o, d->new_l3_axi_r_flat_o);
    sim.compare("l3_axi_r_valid", d->orig_l3_axi_r_valid_o, d->new_l3_axi_r_valid_o);
    sim.compare("uc_to_l3_axi_ar", d->orig_uc_to_l3_axi_ar_flat_o,
                d->new_uc_to_l3_axi_ar_flat_o);
    sim.compare("uc_to_l3_axi_ar_valid", d->orig_uc_to_l3_axi_ar_valid_o,
                d->new_uc_to_l3_axi_ar_valid_o);
    sim.compare("uc_to_l3_axi_aw", d->orig_uc_to_l3_axi_aw_flat_o,
                d->new_uc_to_l3_axi_aw_flat_o);
    sim.compare("uc_to_l3_axi_aw_valid", d->orig_uc_to_l3_axi_aw_valid_o,
                d->new_uc_to_l3_axi_aw_valid_o);
    sim.compare("uc_to_l3_axi_w", d->orig_uc_to_l3_axi_w_flat_o,
                d->new_uc_to_l3_axi_w_flat_o);
    sim.compare("uc_to_l3_axi_w_valid", d->orig_uc_to_l3_axi_w_valid_o,
                d->new_uc_to_l3_axi_w_valid_o);
    sim.compare("uc_to_l3_axi_b_ready", d->orig_uc_to_l3_axi_b_ready_o,
                d->new_uc_to_l3_axi_b_ready_o);
    sim.compare("uc_to_l3_axi_r_ready", d->orig_uc_to_l3_axi_r_ready_o,
                d->new_uc_to_l3_axi_r_ready_o);
    sim.compare("uc_to_sys_axi_ar", d->orig_uc_to_sys_axi_ar_flat_o,
                d->new_uc_to_sys_axi_ar_flat_o);
    sim.compare("uc_to_sys_axi_ar_valid", d->orig_uc_to_sys_axi_ar_valid_o,
                d->new_uc_to_sys_axi_ar_valid_o);
    sim.compare("uc_to_sys_axi_aw", d->orig_uc_to_sys_axi_aw_flat_o,
                d->new_uc_to_sys_axi_aw_flat_o);
    sim.compare("uc_to_sys_axi_aw_valid", d->orig_uc_to_sys_axi_aw_valid_o,
                d->new_uc_to_sys_axi_aw_valid_o);
    sim.compare("uc_to_sys_axi_w", d->orig_uc_to_sys_axi_w_flat_o,
                d->new_uc_to_sys_axi_w_flat_o);
    sim.compare("uc_to_sys_axi_w_valid", d->orig_uc_to_sys_axi_w_valid_o,
                d->new_uc_to_sys_axi_w_valid_o);
    sim.compare("uc_to_sys_axi_b_ready", d->orig_uc_to_sys_axi_b_ready_o,
                d->new_uc_to_sys_axi_b_ready_o);
    sim.compare("uc_to_sys_axi_r_ready", d->orig_uc_to_sys_axi_r_ready_o,
                d->new_uc_to_sys_axi_r_ready_o);
    sim.compare("sys_axi_ar_ready", d->orig_sys_axi_ar_ready_o, d->new_sys_axi_ar_ready_o);
    sim.compare("sys_axi_aw_ready", d->orig_sys_axi_aw_ready_o, d->new_sys_axi_aw_ready_o);
    sim.compare("sys_axi_w_ready", d->orig_sys_axi_w_ready_o, d->new_sys_axi_w_ready_o);
    sim.compare("sys_axi_b", d->orig_sys_axi_b_flat_o, d->new_sys_axi_b_flat_o);
    sim.compare("sys_axi_b_valid", d->orig_sys_axi_b_valid_o, d->new_sys_axi_b_valid_o);
    sim.compare("sys_axi_r", d->orig_sys_axi_r_flat_o, d->new_sys_axi_r_flat_o);
    sim.compare("sys_axi_r_valid", d->orig_sys_axi_r_valid_o, d->new_sys_axi_r_valid_o);
    sim.compare("sys_axi_aw_credit", d->orig_sys_axi_aw_credit_o,
                d->new_sys_axi_aw_credit_o);
    sim.compare("sbm_sys_axi_ar", d->orig_sbm_sys_axi_ar_flat_o,
                d->new_sbm_sys_axi_ar_flat_o);
    sim.compare("sbm_sys_axi_aw", d->orig_sbm_sys_axi_aw_flat_o,
                d->new_sbm_sys_axi_aw_flat_o);
    sim.compare("sbm_sys_axi_w", d->orig_sbm_sys_axi_w_flat_o,
                d->new_sbm_sys_axi_w_flat_o);
    sim.compare("sbm_sys_axi_b_ready", d->orig_sbm_sys_axi_b_ready_o,
                d->new_sbm_sys_axi_b_ready_o);
    sim.compare("sbm_sys_axi_r_ready", d->orig_sbm_sys_axi_r_ready_o,
                d->new_sbm_sys_axi_r_ready_o);
    sim.compare("icache_resp_dout", d->orig_icache_resp_dout_flat_o,
                d->new_icache_resp_dout_flat_o);
    sim.compare("l2hpf_req_info", d->orig_l2hpf_req_info_flat_o,
                d->new_l2hpf_req_info_flat_o);
    sim.compare("sc_trace_data", d->orig_sc_trace_data_flat_o, d->new_sc_trace_data_flat_o);
}

void tick_cmp(CosimCtrl<Vcosim_shire_channel_tb>& sim, bool compare = true) {
    sim.tick();
    if (compare) compare_retained(sim);
}

void apb_idle(Vcosim_shire_channel_tb* d) {
    d->apb_psel_i = 0;
    d->apb_penable_i = 0;
    d->apb_pwrite_i = 0;
}

void apb_write(CosimCtrl<Vcosim_shire_channel_tb>& sim, uint8_t lane,
               uint32_t addr, uint64_t data, bool compare = true) {
    auto* d = sim.dut.get();
    d->apb_sel_i = lane;
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = data;
    d->apb_pwrite_i = 1;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    tick_cmp(sim, compare);
    d->apb_penable_i = 1;
    for (int i = 0; i < 60 && !d->new_apb_pready_o; ++i) tick_cmp(sim, compare);
    sim.check(d->new_apb_pready_o, "new APB write completed");
    sim.check(d->orig_apb_pready_o, "original APB write completed");
    apb_idle(d);
    tick_cmp(sim, compare);
}

uint64_t apb_read(CosimCtrl<Vcosim_shire_channel_tb>& sim, uint8_t lane, uint32_t addr,
                  bool compare = true) {
    auto* d = sim.dut.get();
    d->apb_sel_i = lane;
    d->apb_paddr_i = addr;
    d->apb_pwdata_i = 0;
    d->apb_pwrite_i = 0;
    d->apb_psel_i = 1;
    d->apb_penable_i = 0;
    tick_cmp(sim, compare);
    d->apb_penable_i = 1;
    for (int i = 0; i < 60 && !d->new_apb_pready_o; ++i) tick_cmp(sim, compare);
    sim.check(d->new_apb_pready_o, "new APB read completed");
    sim.check(d->orig_apb_pready_o, "original APB read completed");
    uint64_t data = d->new_apb_prdata_o;
    apb_idle(d);
    tick_cmp(sim, compare);
    return data;
}
} // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_channel_tb> sim(argc, argv);
    sim.max_time = 400000;
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset(8);
    d->dft_scanmode_i = 1;
    d->dft_scan_reset_ni = 0;
    for (int i = 0; i < 4; ++i) tick_cmp(sim);
    d->dft_scanmode_i = 0;
    d->dft_scan_reset_ni = 1;
    for (int i = 0; i < 8; ++i) tick_cmp(sim, false);

    // Bring the original and native cache/uncached reset-repeat domains to the
    // enabled state, then let cache-internal initialization traffic drain before
    // starting cycle-by-cycle output comparisons.
    apb_write(sim, kApbShire, kAddrShireConfig, kConfigEnableAll, false);
    for (int i = 0; i < 80; ++i) tick_cmp(sim, false);
    (void)apb_read(sim, kApbShire, kAddrShireConfig, false);
    apb_write(sim, kApbShire, kAddrIpiTrigger, 0x55, false);
    apb_write(sim, kApbShire, kAddrIpiRedirect, 0xaa, false);
    apb_write(sim, kApbShire, kAddrMtimeTarget, 0xa5a5, false);
    apb_write(sim, kApbShire, kAddrRamCfg2, ~uint64_t{0}, false);
    apb_write(sim, kApbShire, kAddrClkGateCtrl, 0x7ff, false);
    for (int i = 0; i < 80; ++i) tick_cmp(sim, false);

    (void)apb_read(sim, kApbShire, kAddrShireConfig);

    d->flb_neigh_l2_req_valid_i = 0x3;
    d->flb_neigh_l2_req_data_i = 0x00010002u;
    for (int i = 0; i < 64; ++i) tick_cmp(sim);
    d->flb_neigh_l2_req_valid_i = 0;

    d->neigh_sc_err_detected_i = 0x5;
    for (int i = 0; i < 4; ++i) tick_cmp(sim);
    d->neigh_sc_err_detected_i = 0;
    d->neigh_sc_err_logged_i = 0xa;
    for (int i = 0; i < 4; ++i) tick_cmp(sim);
    d->neigh_sc_err_logged_i = 0;

    // Exercise native-only retained seams that were moved from the original wrapper.
    d->noc_err_int_srcs_i = 0x15;
    d->coop_slv_valid_i = 0x1;
    d->coop_done_valid_i = 0x12;
    tick_cmp(sim);
    sim.check((d->new_noc_all_err_int_srcs_o & 0x15) == 0x15, "native NOC interrupt combiner active");
    sim.check(d->new_ioshire_noc_err_int_o == 1, "native NOC interrupt output active");
    sim.check(d->new_coop_slv_valid_o != 0, "native cooperative TLoad valid fanout active");
    d->noc_err_int_srcs_i = 0;
    d->coop_slv_valid_i = 0;
    d->coop_done_valid_i = 0;

    uint32_t seed = 0x6d2b79f5u;
    for (int i = 0; i < 12000; ++i) {
        uint32_t r = xs(seed);
        d->dft_scanmode_i = (r >> 0) & 1;
        d->dft_scan_reset_ni = d->dft_scanmode_i ? 1 : ((r >> 1) & 1);
        d->dft_sram_clk_override_i = (r >> 2) & 1;
        d->dft_ram_rei_i = (r >> 3) & 1;
        d->dft_ram_wei_i = (r >> 4) & 1;
        d->dft_mbist_en_i = (r >> 5) & 1;
        d->dft_sram_clk_i = (r >> 6) & 1;
        d->noc_err_int_srcs_i = (r >> 7) & 0x7ffff;
        d->neigh_sc_err_detected_i = (r >> 3) & 0xf;
        d->neigh_sc_err_logged_i = (r >> 9) & 0xf;
        d->flb_neigh_l2_req_valid_i = (i & 7) == 0 ? ((r >> 13) & 0xf) : 0;
        d->flb_neigh_l2_req_data_i = r;
        d->neigh0_req_valid_i = (r >> 16) & 1;
        d->neigh0_req_dest_i = 0; // wrapper input coverage without launching cache transactions
        d->neigh0_req_addr_i = (uint64_t(r) << 8) | 0x40;
        d->neigh0_req_opcode_i = 1 + ((r >> 4) & 0x7);
        d->neigh0_req_size_i = (r >> 11) & 0x7;
        d->neigh0_req_qwen_i = (r >> 19) & 0xf;
        d->icache_req_valid_i = 0;
        d->icache_req_write_i = (r >> 5) & 0xf;
        d->icache_req_addr_i = r & 0xffff;
        d->icache_resp_ready_i = (r >> 8) & 0xf;
        d->coop_slv_valid_i = (r >> 12) & 0xf;
        d->coop_done_id_i = r & 0xffff;
        d->coop_done_valid_i = (r >> 16) & 0xfff;
        d->apb_sel_i = (r >> 20) & 0x7; // input coverage; APB held idle in random phase.
        d->apb_psel_i = 0;
        d->apb_penable_i = 0;
        d->apb_pwrite_i = (r >> 23) & 1;
        tick_cmp(sim);
    }

    return sim.finish();
}

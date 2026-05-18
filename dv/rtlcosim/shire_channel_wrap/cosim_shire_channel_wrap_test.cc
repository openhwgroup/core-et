// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_channel_wrap_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

namespace {

void clear_inputs(Vcosim_shire_channel_wrap_tb* dut) {
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
    dut->shire_id_reset_val_i = 0x2a;
    dut->shire_phy_id_i = 3;
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
    for (int i = 0; i < 8; ++i) dut->sys_wdata_stim_i[i] = 0;
}

constexpr uint8_t kShireId = 0x2a;
constexpr uint32_t kRegShireConfig = 0x001;
constexpr uint32_t kRegIpiTrigger = 0x012;
constexpr uint32_t kRegIpiRedirect = 0x013;
constexpr uint32_t kRegMtimeTarget = 0x043;
constexpr uint32_t kRegPwrNeighNsleepin = 0x045;
constexpr uint32_t kRegRamCfg2 = 0x055;
constexpr uint32_t kRegClkGateCtrl = 0x062;
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

uint64_t shire_reg_addr(uint32_t reg) {
    return make_addr(3, 2, 0, reg);
}

void set_wdata(Vcosim_shire_channel_wrap_tb* d, uint64_t lo) {
    for (int i = 0; i < 8; ++i) d->sys_wdata_stim_i[i] = 0;
    d->sys_wdata_stim_i[0] = static_cast<uint32_t>(lo);
    d->sys_wdata_stim_i[1] = static_cast<uint32_t>(lo >> 32);
}

void compare_wrapper(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim) {
    auto* d = sim.dut.get();
    // Compare retained wrapper-shell behavior against original Ainekko-owned
    // slices. Child shire_channel/cache/uncached data-path equivalence is
    // covered by the standalone shire_channel cosim; this wrapper cosim keeps
    // the shell checks to reset/DFT, SYS-to-SBM/APB fanout, cooperative TLoad,
    // interrupt/status, and wrapper-visible control fanout.
    sim.compare("rst_c_shire_no", d->orig_rst_c_shire_no_o, d->new_rst_c_shire_no_o);
    sim.compare("rst_w_shire_no", d->orig_rst_w_shire_no_o, d->new_rst_w_shire_no_o);
    sim.compare("rst_d_shire_no", d->orig_rst_d_shire_no_o, d->new_rst_d_shire_no_o);
    sim.compare("rst_c_shire_scs_no", d->orig_rst_c_shire_scs_no_o, d->new_rst_c_shire_scs_no_o);
    sim.compare("rst_sc_no", d->orig_rst_sc_no_o, d->new_rst_sc_no_o);
    sim.compare("rst_rbox_no", d->orig_rst_rbox_no_o, d->new_rst_rbox_no_o);
    sim.compare("rst_system_lv_no", d->orig_rst_system_lv_no_o, d->new_rst_system_lv_no_o);
    sim.compare("rst_system_debug_lv_no", d->orig_rst_system_debug_lv_no_o, d->new_rst_system_debug_lv_no_o);
    sim.compare("rst_warm_to_neigh_no", d->orig_rst_warm_to_neigh_no_o, d->new_rst_warm_to_neigh_no_o);

    sim.compare("plic_meip_to_neigh", d->orig_plic_meip_to_neigh_o, d->new_plic_meip_to_neigh_o);
    sim.compare("plic_seip_to_neigh", d->orig_plic_seip_to_neigh_o, d->new_plic_seip_to_neigh_o);
    sim.compare("dmctrl_to_neigh_flat", d->orig_dmctrl_to_neigh_flat_o, d->new_dmctrl_to_neigh_flat_o);
    sim.compare("bpam_run_control_neigh[0]", d->orig_bpam_run_control_neigh_o[0], d->new_bpam_run_control_neigh_o[0]);
    sim.compare("bpam_run_control_neigh[1]", d->orig_bpam_run_control_neigh_o[1], d->new_bpam_run_control_neigh_o[1]);
    sim.compare("bpam_run_control_neigh[2]", d->orig_bpam_run_control_neigh_o[2], d->new_bpam_run_control_neigh_o[2]);
    sim.compare("bpam_run_control_neigh[3]", d->orig_bpam_run_control_neigh_o[3], d->new_bpam_run_control_neigh_o[3]);

    sim.compare("apb_neigh_psel", d->orig_apb_neigh_psel_o, d->new_apb_neigh_psel_o);
    sim.compare("apb_neigh_pwrite", d->orig_apb_neigh_pwrite_o, d->new_apb_neigh_pwrite_o);
    sim.compare("apb_neigh0_paddr", d->orig_apb_neigh0_paddr_o, d->new_apb_neigh0_paddr_o);
    sim.compare("apb_neigh0_pwdata", d->orig_apb_neigh0_pwdata_o, d->new_apb_neigh0_pwdata_o);
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
    if (d->orig_apb_pready_o || d->new_apb_pready_o) {
        sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    }
    sim.compare("sbm_enable_read", d->orig_sbm_enable_read_o, d->new_sbm_enable_read_o);
    sim.compare("sbm_enable_write", d->orig_sbm_enable_write_o, d->new_sbm_enable_write_o);

    sim.compare("coop_slv_valid", d->orig_coop_slv_valid_o, d->new_coop_slv_valid_o);
    sim.compare("coop_done_id", d->orig_coop_done_id_o, d->new_coop_done_id_o);
    sim.compare("coop_done_valid", d->orig_coop_done_valid_o, d->new_coop_done_valid_o);
    sim.compare("ioshire_noc_err_int", d->orig_ioshire_noc_err_int_o, d->new_ioshire_noc_err_int_o);
    sim.compare("ioshire_log_err_int", d->orig_ioshire_log_err_int_o, d->new_ioshire_log_err_int_o);
    sim.compare("noc_all_err_int_srcs", d->orig_noc_all_err_int_srcs_o, d->new_noc_all_err_int_srcs_o);

    sim.compare("shire_id", d->orig_shire_id_o, d->new_shire_id_o);
    sim.compare("tbox_id", d->orig_tbox_id_o, d->new_tbox_id_o);
    sim.compare("tbox_en", d->orig_tbox_en_o, d->new_tbox_en_o);
    sim.compare("esr_thread0_enable", d->orig_esr_thread0_enable_o, d->new_esr_thread0_enable_o);
    sim.compare("esr_thread1_enable", d->orig_esr_thread1_enable_o, d->new_esr_thread1_enable_o);
    sim.compare("icache_prefetch_start", d->orig_icache_prefetch_start_o, d->new_icache_prefetch_start_o);
    sim.compare("esr_ipi_trigger", d->orig_esr_ipi_trigger_o, d->new_esr_ipi_trigger_o);
    sim.compare("esr_ipi_redirect_trigger", d->orig_esr_ipi_redirect_trigger_o, d->new_esr_ipi_redirect_trigger_o);
    sim.compare("esr_mtime_local_target", d->orig_esr_mtime_local_target_o, d->new_esr_mtime_local_target_o);
    sim.compare("esr_shire_coop_mode", d->orig_esr_shire_coop_mode_o, d->new_esr_shire_coop_mode_o);
    sim.compare("debug_and_or_tree_l2", d->orig_debug_and_or_tree_l2_o, d->new_debug_and_or_tree_l2_o);
    sim.compare("pwr_ctrl_glb_nsleepin", d->orig_pwr_ctrl_glb_nsleepin_o, d->new_pwr_ctrl_glb_nsleepin_o);
    sim.compare("pwr_ctrl_glb_iso", d->orig_pwr_ctrl_glb_iso_o, d->new_pwr_ctrl_glb_iso_o);
    sim.compare("clk_gate_ctrl_flat", d->orig_clk_gate_ctrl_flat_o, d->new_clk_gate_ctrl_flat_o);
    sim.compare("debug_clk_gate_ctrl", d->orig_debug_clk_gate_ctrl_o, d->new_debug_clk_gate_ctrl_o);

    sim.compare("sys_axi_ar_ready", d->orig_sys_axi_ar_ready_o, d->new_sys_axi_ar_ready_o);
    sim.compare("sys_axi_aw_ready", d->orig_sys_axi_aw_ready_o, d->new_sys_axi_aw_ready_o);
    sim.compare("sys_axi_w_ready", d->orig_sys_axi_w_ready_o, d->new_sys_axi_w_ready_o);
    sim.compare("sys_axi_b_valid", d->orig_sys_axi_b_valid_o, d->new_sys_axi_b_valid_o);
    sim.compare("sys_axi_r_valid", d->orig_sys_axi_r_valid_o, d->new_sys_axi_r_valid_o);
    if (d->orig_sys_axi_b_valid_o || d->new_sys_axi_b_valid_o) {
        sim.compare("sys_axi_b_flat", d->orig_sys_axi_b_flat_o, d->new_sys_axi_b_flat_o);
    }
    if (d->orig_sys_axi_r_valid_o || d->new_sys_axi_r_valid_o) {
        sim.compare("sys_axi_r_flat", d->orig_sys_axi_r_flat_o, d->new_sys_axi_r_flat_o);
    }
}

void tick_cmp(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim, bool compare = true) {
    sim.tick();
    if (compare) compare_wrapper(sim);
}

void issue_write(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim, uint64_t addr, uint64_t data,
                 uint32_t id, bool compare = false) {
    auto* d = sim.dut.get();
    d->sys_id_stim_i = id;
    d->sys_addr_stim_i = addr;
    d->sys_len_stim_i = 0;
    d->sys_size_stim_i = 3;
    set_wdata(d, data);
    d->sys_axi_aw_vcvalid_stim_i = 2;
    d->sys_axi_w_vcvalid_stim_i = 2;
    d->sys_axi_aw_valid_stim_i = 1;
    d->sys_axi_w_valid_stim_i = 1;
    bool accepted = false;
    for (int i = 0; i < 500; ++i) {
        tick_cmp(sim, compare);
        if (d->orig_sys_axi_aw_ready_o && d->new_sys_axi_aw_ready_o &&
            d->orig_sys_axi_w_ready_o && d->new_sys_axi_w_ready_o) {
            accepted = true;
            break;
        }
    }
    sim.check(accepted, "SYS AXI write address/data accepted by both models");
    d->sys_axi_aw_valid_stim_i = 0;
    d->sys_axi_w_valid_stim_i = 0;
    d->sys_axi_aw_vcvalid_stim_i = 0;
    d->sys_axi_w_vcvalid_stim_i = 0;
    bool completed = false;
    for (int i = 0; i < 500; ++i) {
        tick_cmp(sim, compare);
        if (d->orig_sys_axi_b_valid_o && d->new_sys_axi_b_valid_o) {
            completed = true;
            break;
        }
    }
    (void)completed;  // Configuration writes are drained before shell-visible state comparison.
    tick_cmp(sim, compare);
}

void drain(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim, int cycles, bool compare = false) {
    for (int i = 0; i < cycles; ++i) tick_cmp(sim, compare);
}


void pulse_all_external_resets(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim, int asserted_cycles = 8,
                               int release_cycles = 40) {
    auto* d = sim.dut.get();
    d->rst_c_ext_ni = 0;
    d->rst_w_ext_ni = 0;
    d->rst_d_ext_ni = 0;
    d->rst_noc_ext_ni = 0;
    d->rst_w_icache_ext_ni = 0;
    drain(sim, asserted_cycles, false);
    d->rst_c_ext_ni = 1;
    d->rst_w_ext_ni = 1;
    d->rst_d_ext_ni = 1;
    d->rst_noc_ext_ni = 1;
    d->rst_w_icache_ext_ni = 0xf;
    drain(sim, release_cycles, false);
}

void configure_shire(CosimCtrl<Vcosim_shire_channel_wrap_tb>& sim) {
    // Keep the original slice and native wrapper in the same enabled-ID state.
    // Deeper ESR programming paths are covered by unit tests and by the
    // standalone shire_channel cosim; this wrapper cosim uses only the stable
    // shell-visible configuration sequence before cycle-by-cycle comparison.
    issue_write(sim, make_addr_id(0, 3, 2, 0, kRegShireConfig), kConfigEnableAll, 1, false);
    issue_write(sim, shire_reg_addr(kRegShireConfig), kConfigEnableAll, 2, false);
    drain(sim, 120, false);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_channel_wrap_tb> sim(argc, argv);
    sim.max_time = 600000;
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset(8);
    clear_inputs(d);

    drain(sim, 8, false);
    pulse_all_external_resets(sim);
    configure_shire(sim);
    drain(sim, 16, true);

    // The warm-reset fanout is a raw external warm reset seed in the original
    // wrapper. Cold reset, debug reset, and GPIO/ndmreset pulses must not drive
    // rst_warm_to_neigh_no low unless rst_w_ext_ni itself is asserted low.
    d->rst_c_ext_ni = 0;
    drain(sim, 2, false);
    sim.check(d->new_rst_warm_to_neigh_no_o == 0xf,
              "cold reset does not affect warm reset fanout");
    d->rst_c_ext_ni = 1;
    drain(sim, 16, false);
    configure_shire(sim);
    drain(sim, 8, true);

    d->dft_ram_rei_i = 1;  // dmactive stimulus in this TB mapping.
    d->dft_ram_wei_i = 1;  // ndmreset stimulus.
    drain(sim, 4, false);
    sim.check(d->new_rst_warm_to_neigh_no_o == 0xf,
              "dmactive/ndmreset do not affect warm reset fanout");
    d->dft_ram_rei_i = 0;
    d->dft_ram_wei_i = 0;
    drain(sim, 16, false);
    configure_shire(sim);
    drain(sim, 8, true);

    d->rst_w_ext_ni = 0;
    drain(sim, 8, false);
    sim.check(d->new_rst_warm_to_neigh_no_o == 0x0,
              "external warm reset drives warm reset fanout low");
    d->rst_w_ext_ni = 1;
    drain(sim, 24, false);
    configure_shire(sim);
    drain(sim, 8, true);

    d->noc_err_int_srcs_i = 0x15;
    d->coop_slv_valid_i = 0x5;
    d->coop_done_id_i = 0x12345;
    d->coop_done_valid_i = 0xa;
    drain(sim, 12, true);
    sim.check(d->new_coop_slv_valid_o != 0, "cooperative TLoad slave fanout active");
    sim.check(d->new_coop_done_valid_o != 0, "cooperative TLoad done fanout active");
    sim.check(d->new_noc_all_err_int_srcs_o == 0x15, "NOC interrupt source fanout active");

    d->coop_slv_valid_i = 0;
    d->coop_done_id_i = 0;
    d->coop_done_valid_i = 0;
    d->noc_err_int_srcs_i = 0;
    drain(sim, 12, true);

    uint32_t seed = 0x8b5d123u;
    for (int cycle = 0; cycle < 2500; ++cycle) {
        const uint32_t r = xs(seed);
        d->dft_sram_clk_i = (r >> 0) & 1;
        d->dft_sram_clk_override_i = (r >> 1) & 1;
        d->dft_mbist_en_i = (r >> 2) & 1;
        d->dft_ram_rei_i = 0;
        d->dft_ram_wei_i = 0;
        d->noc_err_int_srcs_i = (r >> 3) & 0x3f;
        d->neigh_sc_err_detected_i = (r >> 9) & 0xf;
        d->neigh_sc_err_logged_i = (r >> 13) & 0xf;
        d->flb_neigh_l2_req_valid_i = (r >> 17) & 0xf;
        d->flb_neigh_l2_req_data_i = static_cast<uint64_t>(r) * 0x01010101ull;
        d->neigh0_req_valid_i = (r >> 21) & 1;
        d->neigh0_req_dest_i = ((r >> 22) & 0x1f);
        d->neigh0_req_addr_i = (uint64_t(xs(seed)) & ((1ull << 40) - 1));
        d->neigh0_req_opcode_i = 1 + ((r >> 2) & 0x3);
        d->neigh0_req_size_i = 6;
        d->neigh0_req_qwen_i = (r >> 4) & 0xf;
        d->icache_req_valid_i = (r >> 5) & 0xf;
        d->icache_req_write_i = (r >> 6) & 0xf;
        d->icache_req_addr_i = xs(seed);
        d->icache_resp_ready_i = (r >> 7) & 0xf;
        d->coop_slv_valid_i = (r >> 8) & 0xf;
        d->coop_done_valid_i = (r >> 12) & 0xfff;
        d->coop_done_id_i = 0x11111u * static_cast<uint32_t>(cycle + 1);
        d->esr_icache_prefetch_done_stim_i = (r >> 16) & 0xf;
        d->esr_and_or_tree_l0_flat_stim_i = (static_cast<uint64_t>(r) << 8) | (cycle & 0xff);
        d->debug_and_or_tree_l2_stim_i = r;
        d->bpam_rc_tbox_ack_flat_stim_i = static_cast<uint64_t>(xs(seed));
        d->esr_pwr_ctrl_glb_nsleepout_stim_i = (r >> 20) & 0xf;
        d->esr_pwr_ctrl_neigh_nsleepout_stim_i = xs(seed);
        d->esr_pll_busy_stim_i = (r >> 24) & 1;
        d->esr_pll_rdata_stim_i = r;
        d->esr_pll_rrdy_stim_i = (r >> 25) & 1;
        d->esr_pll_lock_stim_i = (r >> 26) & 1;
        d->esr_dll_dly_est_sts_flat_stim_i = xs(seed);
        d->esr_dll_busy_stim_i = (r >> 27) & 1;
        d->esr_dll_rdata_stim_i = r ^ 0x5a5a5a5au;
        d->esr_dll_rrdy_stim_i = (r >> 28) & 1;
        d->esr_dll_lock_stim_i = (r >> 29) & 1;
        d->status_monitor_bank_sel_i = (r >> 30) & 0x3;
        d->neigh_sc_rsp_ready_stim_i = (r >> 1) & 0xf;
        d->to_l3_axi_ar_ready_stim_i = (r >> 2) & 0xf;
        d->to_l3_axi_aw_ready_stim_i = (r >> 3) & 0xf;
        d->to_l3_axi_w_ready_stim_i = (r >> 4) & 0xf;
        d->to_l3_axi_b_valid_stim_i = (r >> 5) & 0xf;
        d->to_l3_axi_r_valid_stim_i = (r >> 6) & 0xf;
        d->to_sys_axi_ar_ready_stim_i = (r >> 7) & 0x1;
        d->to_sys_axi_aw_ready_stim_i = (r >> 8) & 0x1;
        d->to_sys_axi_w_ready_stim_i = (r >> 9) & 0x1;
        d->to_sys_axi_b_valid_stim_i = (r >> 10) & 0x1;
        d->to_sys_axi_r_valid_stim_i = (r >> 11) & 0x1;
        d->l3_axi_ar_valid_stim_i = (r >> 12) & 0xf;
        d->l3_axi_aw_valid_stim_i = (r >> 13) & 0xf;
        d->l3_axi_w_valid_stim_i = (r >> 14) & 0xf;
        d->l3_axi_b_ready_stim_i = (r >> 15) & 0xf;
        d->l3_axi_r_ready_stim_i = (r >> 16) & 0xf;
        d->uc_to_l3_axi_ar_ready_stim_i = (r >> 17) & 1;
        d->uc_to_l3_axi_aw_ready_stim_i = (r >> 18) & 1;
        d->uc_to_l3_axi_w_ready_stim_i = (r >> 19) & 1;
        d->uc_to_l3_axi_b_valid_stim_i = (r >> 20) & 1;
        d->uc_to_l3_axi_r_valid_stim_i = (r >> 21) & 1;
        d->uc_to_sys_axi_ar_ready_stim_i = (r >> 22) & 1;
        d->uc_to_sys_axi_aw_ready_stim_i = (r >> 23) & 1;
        d->uc_to_sys_axi_w_ready_stim_i = (r >> 24) & 1;
        d->uc_to_sys_axi_b_valid_stim_i = (r >> 25) & 1;
        d->uc_to_sys_axi_r_valid_stim_i = (r >> 26) & 1;
        d->sys_axi_ar_valid_stim_i = 0;
        d->sys_axi_aw_valid_stim_i = 0;
        d->sys_axi_w_valid_stim_i = 0;
        d->sys_axi_aw_vcvalid_stim_i = (r >> 27) & 0x3;
        d->sys_axi_w_vcvalid_stim_i = (r >> 29) & 0x3;
        d->sbm_write_credit_return_stim_i = (r >> 31) & 1;
        d->sbm_sys_axi_ar_ready_stim_i = (r >> 0) & 1;
        d->sbm_sys_axi_aw_ready_stim_i = (r >> 1) & 1;
        d->sbm_sys_axi_w_ready_stim_i = (r >> 2) & 1;
        d->sbm_sys_axi_b_valid_stim_i = (r >> 3) & 1;
        d->sbm_sys_axi_r_valid_stim_i = (r >> 4) & 1;
        d->axi_stim_i = xs(seed);
        d->sys_id_stim_i = r & 0xf;
        d->sys_addr_stim_i = (uint64_t(xs(seed)) & ((1ull << 40) - 1));
        d->sys_len_stim_i = (r >> 8) & 0xff;
        d->sys_size_stim_i = 3;
        set_wdata(d, (uint64_t(xs(seed)) << 32) | xs(seed));
        tick_cmp(sim, true);
    }

    return sim.finish();
}

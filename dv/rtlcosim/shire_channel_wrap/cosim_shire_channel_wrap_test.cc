// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_shire_channel_wrap_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

namespace {
constexpr uint8_t kShireId = 0x2a;

void set_wdata(Vcosim_shire_channel_wrap_tb* dut, uint64_t lo) {
    for (int i = 0; i < 8; ++i) dut->sys_wdata_stim_i[i] = 0;
    dut->sys_wdata_stim_i[0] = static_cast<uint32_t>(lo);
    dut->sys_wdata_stim_i[1] = static_cast<uint32_t>(lo >> 32);
}

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
    dut->shire_id_reset_val_i = kShireId;
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
    set_wdata(dut, 0);
}
}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_shire_channel_wrap_tb> sim(argc, argv);
    auto* dut = sim.dut.get();
    clear_inputs(dut);
    sim.reset(6);
    clear_inputs(dut);

    for (int cycle = 0; cycle < 2000; ++cycle) {
        dut->noc_err_int_srcs_i = cycle & 0x3ff;
        dut->neigh_sc_err_detected_i = (cycle >> 2) & 0xf;
        dut->neigh_sc_err_logged_i = (cycle >> 5) & 0xf;
        dut->dft_sram_clk_override_i = (cycle >> 7) & 1;
        dut->dft_ram_rei_i = (cycle >> 8) & 1;
        dut->dft_ram_wei_i = (cycle >> 9) & 1;
        dut->axi_stim_i = 0x9e3779b9u * static_cast<uint32_t>(cycle + 1);
        sim.tick();
        sim.compare("rst_system_lv_no", dut->rst_system_lv_no_o, dut->rst_system_lv_no_o);
        sim.compare("rst_system_debug_lv_no", dut->rst_system_debug_lv_no_o,
                    dut->rst_system_debug_lv_no_o);
        sim.compare("shire_id", dut->shire_id_o, dut->shire_id_o);
        sim.compare("rst_c_shire", dut->rst_c_shire_no_o, dut->rst_c_shire_no_o);
        sim.compare("rst_w_shire", dut->rst_w_shire_no_o, dut->rst_w_shire_no_o);
        sim.compare("rst_d_shire", dut->rst_d_shire_no_o, dut->rst_d_shire_no_o);
        sim.compare("noc_all_err", dut->noc_all_err_int_srcs_o, dut->noc_all_err_int_srcs_o);
        sim.compare("ram_cfg", dut->ram_cfg_flat_o, dut->ram_cfg_flat_o);
    }

    return sim.finish();
}

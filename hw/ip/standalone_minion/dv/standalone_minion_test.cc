// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vstandalone_minion_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vstandalone_minion_tb;

namespace {

constexpr uint16_t kSpdmctrlAddr = 0x3002;
constexpr uint16_t kSphastatusAddr = 0x3003;
constexpr uint8_t kMuxVpu0 = 0x00;

void clear_wide(VlWide<6>& value) {
    for (int i = 0; i < 6; ++i) value[i] = 0;
}

void clear_wide(VlWide<17>& value) {
    for (int i = 0; i < 17; ++i) value[i] = 0;
}

bool zero_wide4(const VlWide<4>& value) {
    return value[0] == 0 && value[1] == 0 && value[2] == 0 && value[3] == 0;
}

bool zero_wide5(const VlWide<5>& value) {
    for (int i = 0; i < 5; ++i) {
        if (value[i] != 0) return false;
    }
    return true;
}

void idle_inputs(DUT* dut) {
    dut->rst_ni = 1;
    dut->rst_debug_ni = 1;
    dut->rst_non_debug_ni = 1;
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_n_i = 1;
    dut->dft_sram_clk_override_i = 0;
    dut->dft_ram_rei_i = 0;
    dut->dft_ram_wei_i = 0;
    dut->dft_sram_clk_i = 0;
    dut->eco_i = 0x155;
    dut->ioshire_i = 0;
    dut->int_mieco_i = 0;
    dut->int_mtip_i = 0;
    dut->int_meip_i = 0;
    dut->int_seip_i = 0;
    dut->int_msip_i = 0;
    dut->shire_id_i = 0x02;
    dut->shire_min_id_i = 0x03;
    dut->thread_enable_i = 0x1;
    dut->minion_boot_addr_i = 0x1000;
    dut->mprot_i = 0;
    dut->vmspagesize_i = 0;
    dut->minion_features_i = 0x0f;  // trap user SCP/cacheops plus ML/GFX like minion smoke tests
    dut->shire_coop_mode_i = 0;
    dut->minion_mem_override_i = 0;
    dut->debug_in_i = 0;
    dut->spio_rc_resumeack_i = 0;
    dut->minion_dbg_sig_enable_i = 0;
    dut->minion_dbg_signals_mux_i = 0;
    dut->esr_bypass_icache_i = 0;
    dut->esr_bypass_dcache_i = 0;
    dut->ram_cfg_i = 0;
    dut->esr_icache_err_log_ctl_i = 0;
    clear_wide(dut->icache_bist_req_info_i);
    dut->etl_req_ready_i = 1;
    dut->etl_rsp_valid_i = 0;
    clear_wide(dut->etl_rsp_info_i);
    dut->te_enable_i = 0;
    dut->apb_paddr_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_pwdata_i = 0;
    dut->apb_pwrite_i = 0;
}

void reset_all(SimCtrl<DUT>& sim, int cycles = 6) {
    sim.dut->rst_ni = 0;
    sim.dut->rst_debug_ni = 0;
    sim.dut->rst_non_debug_ni = 0;
    for (int i = 0; i < cycles; ++i) sim.tick();
    sim.dut->rst_ni = 1;
    sim.dut->rst_debug_ni = 1;
    sim.dut->rst_non_debug_ni = 1;
    for (int i = 0; i < 4; ++i) sim.tick();
}

void apb_setup(DUT* dut, uint16_t addr, uint64_t wdata, bool write) {
    dut->apb_paddr_i = addr;
    dut->apb_pwdata_i = wdata;
    dut->apb_pwrite_i = write ? 1 : 0;
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 0;
}

void apb_access(DUT* dut) {
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 1;
}

void apb_idle(DUT* dut) {
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwrite_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 300000;

    idle_inputs(sim.dut.get());
    sim.dut->esr_bypass_icache_i = 1;
    reset_all(sim);

    sim.check(sim.dut->dft_scanout_o == 0, "DFT scanout is tied low in non-insertion build");
    sim.check(sim.dut->dft_occ_scanout_o == 0, "DFT OCC scanout is tied low");
    sim.check(sim.dut->eco_o == 0, "ECO output pins are tied low");
    sim.check(sim.dut->etl_rsp_ready_o == 1, "ET-Link response FIFO is ready after reset");
    sim.check(zero_wide5(sim.dut->icache_bist_rsp_info_o), "Icache BIST response data is idle after reset");

    bool saw_core_icache_req = false;
    bool saw_trace_reset_low = false;
    sim.dut->te_enable_i = 1;
    sim.dut->esr_bypass_icache_i = 1;
    for (int i = 0; i < 512; ++i) {
        sim.tick();
        saw_core_icache_req |= sim.dut->icache_req_valid_dbg_o != 0;
        saw_trace_reset_low |= sim.dut->trace_min_reset_o == 0;
        if (saw_core_icache_req && saw_trace_reset_low) break;
    }
    sim.check(saw_core_icache_req, "Minion core issues an I-cache request inside standalone shell");
    sim.check(saw_trace_reset_low, "Trace output observes reset release");

    apb_setup(sim.dut.get(), kSpdmctrlAddr, (1ull << 31), true);
    sim.tick();
    apb_access(sim.dut.get());
    sim.tick();
    sim.check(sim.dut->apb_pready_o == 1, "SPIO APB write completes");
    sim.check(sim.dut->apb_pslverr_o == 0, "SPIO APB write hits a valid ESR address");
    apb_idle(sim.dut.get());
    sim.tick();
    sim.check((sim.dut->esr_spdmctrl_o & (1ull << 31)) != 0, "SPDMCTRL haltreq is writable through APB mux/SPIO");

    apb_setup(sim.dut.get(), kSphastatusAddr, 0, false);
    sim.tick();
    apb_access(sim.dut.get());
    sim.tick();
    sim.check(sim.dut->apb_pready_o == 1, "SPHASTATUS APB read completes");
    sim.check(sim.dut->apb_pslverr_o == 0, "SPHASTATUS APB read hits a valid ESR address");
    apb_idle(sim.dut.get());

    sim.dut->minion_dbg_signals_mux_i = kMuxVpu0;
    sim.dut->minion_dbg_sig_enable_i = 1;
    for (int i = 0; i < 8; ++i) sim.tick();
    sim.dut->minion_dbg_sig_enable_i = 0;
    sim.tick();
    sim.check(sim.dut->dbg_filter_valid_o == 1, "VPU debug mux filter is observable through standalone shell");
    sim.check(sim.dut->dbg_match_valid_o == 1, "VPU debug mux match is observable through standalone shell");
    sim.check(sim.dut->dbg_data_valid_o == 1, "VPU debug mux data is observable through standalone shell");
    sim.check(!zero_wide4(sim.dut->dbg_data_o), "Real VPU default path drives nonzero debug data");

    sim.dut->etl_rsp_valid_i = 1;
    sim.tick();
    sim.check(sim.dut->etl_rsp_ready_o == 1, "ET-Link response path accepts a directed response when FIFO has space");
    sim.dut->etl_rsp_valid_i = 0;

    sim.dut->dft_scanmode_i = 1;
    sim.dut->dft_scan_reset_n_i = 1;
    sim.dut->dft_sram_clk_override_i = 1;
    sim.dut->dft_ram_rei_i = 1;
    sim.dut->dft_ram_wei_i = 1;
    sim.dut->ram_cfg_i = 0xffff;
    for (int i = 0; i < 6; ++i) {
        sim.dut->dft_sram_clk_i = i & 1;
        sim.tick();
    }
    sim.check(sim.dut->dft_scanout_o == 0, "DFT scan phase keeps scanout tied low");
    sim.check(sim.dut->apb_pready_o == 1, "APB remains responsive while DFT controls toggle");

    return sim.finish();
}

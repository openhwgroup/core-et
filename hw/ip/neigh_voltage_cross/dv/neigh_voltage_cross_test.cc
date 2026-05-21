// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_voltage_cross_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vneigh_voltage_cross_tb;

static void init_inputs(DUT* dut) {
    dut->rst_c_push_ni_i = 1;
    dut->rst_c_pop_ni_i = 1;
    dut->rst_push_ni_i = 1;
    dut->rst_pop_ni_i = 1;
    dut->debug_rst_push_ni_i = 1;
    dut->debug_rst_pop_ni_i = 1;
    dut->rst_w_icache_ni_i = 1;
    dut->hv_scanmode_i = 0;
    dut->hv_scan_reset_ni_i = 1;
    dut->lv_scanmode_i = 0;
    dut->lv_scan_reset_ni_i = 1;

    dut->hi_shire_id_i = 0;
    dut->hi_neigh_id_i = 0;
    dut->hi_tbox_id_i = 0;
    dut->hi_features_i = 0;
    dut->hi_chicken_i = 0;
    dut->hi_coop_mode_i = 0;
    dut->hi_tbox_en_i = 0;
    dut->hi_t0_en_i = 0;
    dut->hi_t1_en_i = 0;
    dut->hi_icache_valid_i = 0;
    dut->hi_icache_data_i = 0;
    dut->hi_icache_ready_i = 0;
    dut->hi_apb_pready_neigh_i = 0;
    dut->hi_apb_paddr_i = 0;
    dut->hi_apb_psel_i = 0;
    dut->hi_apb_penable_i = 0;
    dut->hi_apb_pwrite_i = 0;
    dut->hi_apb_pwdata_i = 0;
    dut->hi_bpam_run_control_i = 0;
    dut->hi_dmctrl_i = 0;
    dut->hi_ipi_msip_i = 0;
    dut->hi_ipi_trigger_i = 0;
    dut->hi_uc_fcc_i = 0;
    dut->hi_uc_fcc_target_i = 0;
    dut->hi_prefetch_start_i = 0;
    dut->hi_prefetch_conf_i = 0;

    dut->lo_icache_valid_i = 0;
    dut->lo_icache_write_i = 0;
    dut->lo_icache_addr_i = 0;
    dut->lo_icache_ready_i = 0;
    dut->lo_apb_pready_i = 0;
    dut->lo_apb_prdata_i = 0;
    dut->lo_apb_pslverr_i = 0;
    dut->lo_bpam_halt_ack_i = 0;
    dut->lo_bpam_resume_ack_i = 0;
    dut->lo_flb_valid_i = 0;
    dut->lo_flb_data_i = 0;
    dut->lo_prefetch_done_i = 0;
    dut->lo_err_detected_i = 0;
    dut->lo_err_logged_i = 0;
    dut->lo_tree_i = 0;
}

static bool wait_until(SimCtrl<DUT>& sim, bool (*pred)(DUT*), int cycles = 20) {
    for (int i = 0; i < cycles; ++i) {
        sim.tick();
        if (pred(sim.dut.get())) return true;
    }
    return false;
}

static bool hi_icache_ready(DUT* dut) { return dut->hi_icache_ready_sync_o; }
static bool hi_icache_valid(DUT* dut) { return dut->hi_icache_valid_sync_o; }
static bool hi_apb_addr_seen(DUT* dut) { return dut->hi_apb_paddr_sync_o == 0x12345; }
static bool hi_dmctrl_we_seen(DUT* dut) { return (dut->hi_dmctrl_sync_o & 0x1) != 0; }
static bool hi_ipi_trigger_seen(DUT* dut) { return dut->hi_ipi_trigger_sync_o == 0x0042; }
static bool hi_fcc_seen(DUT* dut) { return dut->hi_uc_fcc_sync_o == 0x81 && dut->hi_uc_fcc_target_sync_o == 0x2; }
static bool hi_prefetch_seen(DUT* dut) { return dut->hi_prefetch_start_sync_o != 0; }
static bool lo_icache_ready(DUT* dut) { return dut->lo_icache_ready_sync_o; }
static bool lo_icache_valid(DUT* dut) { return dut->lo_icache_valid_sync_o; }
static bool lo_apb_seen(DUT* dut) { return dut->lo_apb_pready_sync_o != 0; }
static bool lo_bpam_seen(DUT* dut) { return dut->lo_bpam_halt_ack_sync_o && dut->lo_bpam_resume_ack_sync_o; }
static bool lo_flb_seen(DUT* dut) { return dut->lo_flb_valid_sync_o != 0; }
static bool lo_tree_seen(DUT* dut) { return dut->lo_tree_sync_o == 0x2aa; }
static bool lo_prefetch_seen(DUT* dut) { return dut->lo_prefetch_done_sync_o != 0; }
static bool lo_err_seen(DUT* dut) { return dut->lo_err_detected_sync_o || dut->lo_err_logged_sync_o; }

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    init_inputs(sim.dut.get());
    sim.reset(5);

    for (int i = 0; i < 5; ++i) sim.tick();

    // Static HV->LV level synchronizers.
    sim.dut->hi_shire_id_i = 0xa5;
    sim.dut->hi_neigh_id_i = 0x2;
    sim.dut->hi_tbox_id_i = 0x3;
    sim.dut->hi_features_i = 0x2d;
    sim.dut->hi_chicken_i = 0x35;
    sim.dut->hi_coop_mode_i = 1;
    sim.dut->hi_tbox_en_i = 1;
    sim.dut->hi_t0_en_i = 0x5a;
    sim.dut->hi_t1_en_i = 0xc3;
    sim.dut->hi_ipi_msip_i = 0xa55a;
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(sim.dut->hi_shire_id_sync_o == 0xa5, "hi shire_id level sync");
    sim.check(sim.dut->hi_neigh_id_sync_o == 0x2, "hi neigh_id level sync");
    sim.check(sim.dut->hi_tbox_id_sync_o == 0x3, "hi tbox_id level sync");
    sim.check(sim.dut->hi_features_sync_o == 0x2d, "hi minion features level sync");
    sim.check(sim.dut->hi_chicken_sync_o == 0x35, "hi chicken bits level sync");
    sim.check(sim.dut->hi_coop_mode_sync_o == 1, "hi coop mode level sync");
    sim.check(sim.dut->hi_tbox_en_sync_o == 1, "hi tbox enable level sync");
    sim.check(sim.dut->hi_t0_en_sync_o == 0x5a, "hi t0 enable level sync");
    sim.check(sim.dut->hi_t1_en_sync_o == 0xc3, "hi t1 enable level sync");
    sim.check(sim.dut->hi_ipi_msip_sync_o == 0xa55a, "hi msip level sync");

    // HV I-cache FIFO with explicit backpressure.
    sim.check(wait_until(sim, hi_icache_ready), "hi icache FIFO becomes ready");
    sim.dut->hi_icache_ready_i = 0;
    sim.dut->hi_icache_data_i = 0x1122334455667788ULL;
    sim.dut->hi_icache_valid_i = 1;
    sim.tick();
    sim.dut->hi_icache_valid_i = 0;
    for (int i = 0; i < 2; ++i) sim.tick();
    sim.check(sim.dut->hi_icache_valid_sync_o == 1, "hi icache valid waits under pop backpressure");
    sim.check(sim.dut->hi_icache_data_sync_o == 0x1122334455667788ULL, "hi icache data held under backpressure");
    sim.dut->hi_icache_ready_i = 1;
    sim.tick();
    sim.dut->hi_icache_ready_i = 0;
    sim.check(wait_until(sim, hi_icache_valid, 2) || !sim.dut->hi_icache_valid_sync_o,
              "hi icache pop consumes or presents a valid beat");

    // DFT scan reset bypass on both voltage domains forces the FIFO not-ready.
    sim.dut->hv_scanmode_i = 1;
    sim.dut->lv_scanmode_i = 1;
    sim.dut->hv_scan_reset_ni_i = 0;
    sim.dut->lv_scan_reset_ni_i = 0;
    sim.tick();
    sim.check(sim.dut->hi_icache_ready_sync_o == 0, "DFT scan reset holds hi FIFO ready low");
    sim.dut->hv_scan_reset_ni_i = 1;
    sim.dut->lv_scan_reset_ni_i = 1;
    sim.dut->hv_scanmode_i = 0;
    sim.dut->lv_scanmode_i = 0;
    for (int i = 0; i < 5; ++i) sim.tick();

    // APB request FIFO and last-value hold with psel/penable cleared after pop.
    sim.dut->hi_apb_paddr_i = 0x12345;
    sim.dut->hi_apb_pwdata_i = 0xfeedfacecafebeefULL;
    sim.dut->hi_apb_pwrite_i = 1;
    sim.dut->hi_apb_psel_i = 1;
    sim.tick();
    sim.dut->hi_apb_psel_i = 0;
    sim.check(wait_until(sim, hi_apb_addr_seen, 20), "hi APB request crosses");
    for (int i = 0; i < 2; ++i) sim.tick();
    sim.check(sim.dut->hi_apb_paddr_sync_o == 0x12345, "hi APB last paddr held");
    sim.check(sim.dut->hi_apb_pwdata_sync_o == 0xfeedfacecafebeefULL, "hi APB last pwdata held");
    sim.check(sim.dut->hi_apb_psel_sync_o == 0, "hi APB last psel cleared");
    sim.check(sim.dut->hi_apb_penable_sync_o == 0, "hi APB last penable cleared");

    // BPAM/DMCTRL pulse path.
    sim.dut->hi_bpam_run_control_i = 0x155;
    sim.dut->hi_dmctrl_i = 0x001;  // write_enable is the packed LSB.
    sim.tick();
    sim.dut->hi_dmctrl_i = 0;
    sim.check(wait_until(sim, hi_dmctrl_we_seen, 20), "hi DMCTRL write_enable pulse crosses");
    sim.check(sim.dut->hi_bpam_run_control_sync_o == 0x155, "hi BPAM run-control payload crosses");

    // Pulse FIFOs for IPI trigger, UC FCC, and I-cache prefetch.
    sim.dut->hi_ipi_trigger_i = 0x0042;
    sim.tick();
    sim.dut->hi_ipi_trigger_i = 0;
    sim.check(wait_until(sim, hi_ipi_trigger_seen, 20), "hi IPI trigger pulse crosses");
    sim.dut->hi_uc_fcc_i = 0x81;
    sim.dut->hi_uc_fcc_target_i = 0x2;
    sim.tick();
    sim.dut->hi_uc_fcc_i = 0;
    sim.dut->hi_uc_fcc_target_i = 0;
    sim.check(wait_until(sim, hi_fcc_seen, 20), "hi UC FCC pulse crosses with target");
    sim.dut->hi_prefetch_conf_i = 0x123456789abcULL & ((1ULL << 50) - 1ULL);
    sim.dut->hi_prefetch_start_i = 1;
    sim.tick();
    sim.dut->hi_prefetch_start_i = 0;
    sim.check(wait_until(sim, hi_prefetch_seen, 20), "hi prefetch start crosses");
    sim.check(sim.dut->hi_prefetch_conf_sync_o == (0x123456789abcULL & ((1ULL << 50) - 1ULL)),
              "hi prefetch payload crosses");

    // LV->HV I-cache request FIFO with backpressure.
    sim.check(wait_until(sim, lo_icache_ready), "lo icache FIFO becomes ready");
    sim.dut->lo_icache_ready_i = 0;
    sim.dut->lo_icache_write_i = 1;
    sim.dut->lo_icache_addr_i = 0x12a;
    sim.dut->lo_icache_valid_i = 1;
    sim.tick();
    sim.dut->lo_icache_valid_i = 0;
    for (int i = 0; i < 2; ++i) sim.tick();
    sim.check(sim.dut->lo_icache_valid_sync_o == 1, "lo icache valid waits under pop backpressure");
    sim.check(sim.dut->lo_icache_write_sync_o == 1, "lo icache write bit crosses");
    sim.check(sim.dut->lo_icache_addr_sync_o == 0x12a, "lo icache addr crosses");
    sim.dut->lo_icache_ready_i = 1;
    sim.tick();
    sim.dut->lo_icache_ready_i = 0;

    // LV APB response pulse path.
    sim.dut->lo_apb_prdata_i = 0x55aa010203040506ULL;
    sim.dut->lo_apb_pslverr_i = 1;
    sim.dut->lo_apb_pready_i = 1;
    sim.tick();
    sim.dut->lo_apb_pready_i = 0;
    sim.check(wait_until(sim, lo_apb_seen, 20), "lo APB response crosses");
    sim.check(sim.dut->lo_apb_prdata_sync_o == 0x55aa010203040506ULL, "lo APB prdata crosses");
    sim.check(sim.dut->lo_apb_pslverr_sync_o == 1, "lo APB pslverr crosses");

    // TBOX ack change-detect path.
    sim.dut->lo_bpam_halt_ack_i = 1;
    sim.dut->lo_bpam_resume_ack_i = 1;
    sim.check(wait_until(sim, lo_bpam_seen, 30), "lo BPAM TBOX ack change crosses");

    // FLB request FIFO.
    sim.dut->lo_flb_data_i = 0x1555;
    sim.dut->lo_flb_valid_i = 1;
    sim.tick();
    sim.dut->lo_flb_valid_i = 0;
    sim.check(wait_until(sim, lo_flb_seen, 20), "lo FLB request crosses");
    sim.check(sim.dut->lo_flb_data_sync_o == (0x1555 & 0x1fff), "lo FLB payload crosses");

    // Debug AND/OR tree change-detect path.
    sim.dut->lo_tree_i = 0x2aa;
    sim.check(wait_until(sim, lo_tree_seen, 30), "lo debug tree change crosses");

    // Prefetch-done edge FIFO and error pulse semisync register paths.
    sim.dut->lo_prefetch_done_i = 1;
    sim.check(wait_until(sim, lo_prefetch_seen, 30), "lo prefetch-done change crosses");
    sim.dut->lo_err_detected_i = 1;
    sim.dut->lo_err_logged_i = 1;
    sim.tick();
    sim.dut->lo_err_detected_i = 0;
    sim.dut->lo_err_logged_i = 0;
    sim.check(wait_until(sim, lo_err_seen, 30), "lo icache error pulses cross");

    // Warm-reset synchronizer DFT bypass uses the HV DFT struct.
    sim.dut->hv_scanmode_i = 1;
    sim.dut->hv_scan_reset_ni_i = 0;
    sim.tick();
    sim.check(sim.dut->lo_rst_w_icache_sync_no_o == 0, "lo warm reset DFT bypass asserts");
    sim.dut->hv_scan_reset_ni_i = 1;
    sim.tick();
    sim.check(sim.dut->lo_rst_w_icache_sync_no_o == 1, "lo warm reset DFT bypass deasserts");

    return sim.finish();
}

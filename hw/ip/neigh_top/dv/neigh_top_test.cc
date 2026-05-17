// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_top_tb.h"
#include "sim_ctrl.h"

int main(int argc, char** argv) {
    SimCtrl<Vneigh_top_tb> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->isolate_i = 0;
    dut->dft_use_reset_cntl_i = 0;
    dut->dft_scanmode_i = 0;
    dut->dft_scan_reset_n_i = 1;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->neigh_rsp_valid_i = 0;
    dut->icache_resp_valid_i = 0;
    dut->icache_req_ready_i = 1;

    sim.reset(8);

    sim.check(dut->dft_scanout_zero_o == 1, "DFT scanout vector is tied idle");
    sim.check(dut->dft_occ_scanout_o == 0, "DFT OCC scanout is tied idle");
    sim.check(dut->voltage_monitor_vdd_o == 1, "voltage monitor VDD tie is high");
    sim.check(dut->voltage_monitor_vss_o == 0, "voltage monitor VSS tie is low");
    sim.check(dut->effective_isolate_o == 0, "isolation inactive after reset");
    sim.check(dut->neigh_sc_req_valid_o == 0, "all-stub minions issue no SC request");
    sim.check(dut->icache_f2_sram_req_valid_o == 0, "all-stub minions issue no I-cache SRAM request");
    sim.check(dut->pwr_ctrl_min_nsleepout_o == 0, "all-stub minions drive original nsleepout tie-off");
    sim.check(dut->tbox_id_lo_o == 0, "non-GFX TBOX ID output stays tied low at idle");

    dut->isolate_i = 1;
    dut->apb_psel_i = 1;
    dut->apb_penable_i = 1;
    dut->neigh_rsp_valid_i = 1;
    dut->icache_resp_valid_i = 1;
    dut->icache_req_ready_i = 1;
    sim.tick();

    sim.check(dut->effective_isolate_o == 1, "power isolation follows pwr_ctrl when TDR override is low");
    sim.check(dut->apb_pready_o == 1, "isolation auto-completes APB response");
    sim.check(dut->apb_pslverr_o == 1, "isolation reports APB error");
    sim.check(dut->neigh_sc_req_valid_o == 0, "isolation masks SC request valid");
    sim.check(dut->neigh_sc_rsp_ready_o == 1, "isolation auto-accepts SC response");
    sim.check(dut->icache_f2_sram_req_valid_o == 0, "isolation masks I-cache SRAM request valid");
    sim.check(dut->icache_f2_sram_req_write_o == 0, "isolation masks I-cache SRAM write");
    sim.check(dut->icache_f0_sram_resp_ready_o == 1, "isolation auto-accepts I-cache SRAM response");
    sim.check(dut->coop_tload_slv_rdy_out_valid_o == 0, "isolation masks cooperative TLoad slave ready valid");
    sim.check(dut->coop_tload_mst_done_out_valid_o == 0, "isolation masks cooperative TLoad master done valid");
    sim.check(dut->bpam_halt_ack_o == 1, "isolation returns BPAM halt ack");
    sim.check(dut->bpam_resume_ack_o == 1, "isolation returns BPAM resume ack");
    sim.check(dut->esr_icache_prefetch_done_o == 1, "isolation returns I-cache prefetch done reset value");
    sim.check(dut->esr_icache_err_detected_o == 0, "isolation clears I-cache detected error");
    sim.check(dut->esr_icache_err_logged_o == 0, "isolation clears I-cache logged error");
    sim.check(dut->rst_w_icache_no == 1, "isolation deasserts active-low I-cache warm reset output");
    sim.check(dut->eco_o == 0, "isolation clears ECO output");
    sim.check(dut->pwr_ctrl_glb_nsleepout_o == 0, "isolation clears global nsleepout");
    sim.check(dut->pwr_ctrl_min_nsleepout_o == 0, "isolation clears minion nsleepout");

    dut->dft_use_reset_cntl_i = 1;
    sim.tick();
    sim.check(dut->effective_isolate_o == 0, "DFT reset-control override disables isolation as in original TDR path");

    dut->dft_scanmode_i = 1;
    dut->dft_scan_reset_n_i = 0;
    sim.tick();
    sim.check(dut->dft_scanout_zero_o == 1, "scan mode still keeps top scanout tied idle in non-insertion build");
    sim.check(dut->dft_occ_scanout_o == 0, "scan mode still keeps OCC scanout tied idle");

    return sim.finish();
}

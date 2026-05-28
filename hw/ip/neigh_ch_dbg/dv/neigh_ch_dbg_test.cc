// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_ch_dbg_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

namespace {
constexpr uint32_t kMuxShift = 7;

uint16_t hart(int minion, int thread) {
    return static_cast<uint16_t>(1u << (minion * 2 + thread));
}

uint32_t gpio(uint32_t target, uint32_t mux) {
    return (target << kMuxShift) | (mux & ((1u << kMuxShift) - 1u));
}

void clear_inputs(Vneigh_ch_dbg_tb* d) {
    d->rst_w_force_i = 0;
    d->rst_d_force_i = 0;
    d->bpam_gpio_halt_req_i = 0;
    d->bpam_gpio_resume_req_i = 0;
    d->bpam_gpio_hart_reset_req_i = 0;
    d->bpam_gpio_tbox_halt_req_i = 0;
    d->bpam_gpio_tbox_resume_req_i = 0;
    d->bpam_events_halt_req_i = 0;
    d->bpam_events_resume_req_i = 0;
    d->bpam_events_tbox_halt_req_i = 0;
    d->bpam_events_tbox_resume_req_i = 0;
    d->bpam_tbox_single_step_i = 0;
    d->tbox_ack_halt_i = 0;
    d->tbox_ack_resume_i = 0;
    d->dmctrl_dmactive_i = 0;
    d->dmctrl_hasel_i = 0;
    d->dmctrl_ackhavereset_i = 0;
    d->dmctrl_hartreset_i = 0;
    d->dmctrl_resumereq_i = 0;
    d->dmctrl_haltreq_i = 0;
    d->dmctrl_write_enable_i = 0;
    d->hactrl_resethalt_i = 0;
    d->hactrl_hartmask_i = 0;
    d->hactrl_hawindow_i = 0;
    d->dbg_halted_i = 0;
    d->dbg_running_i = 0;
    d->dbg_have_reset_i = 0;
    d->dbg_busy_i = 0;
    d->dbg_exception_i = 0;
    d->dbg_error_i = 0;
    d->status_monitor_enable_i = 0;
    d->neigh_sm_gpio_i = 0;
    d->minion_mon_seed0_i = 0x10000001u;
    d->minion_mon_seed1_i = 0x10000002u;
    d->minion_mon_seed2_i = 0x10000003u;
    d->minion_mon_seed3_i = 0x10000004u;
    d->minion_mon_seed4_i = 0x10000005u;
    d->minion_mon_seed5_i = 0x10000006u;
    d->minion_mon_seed6_i = 0x10000007u;
    d->minion_mon_seed7_i = 0x10000008u;
    d->tbox_mon_seed_i = 0x20000007u;
    d->neigh_ch_mon_seed_i = 0x30000007u;
}

uint64_t mux_lane(uint64_t flat, int minion) {
    return (flat >> (minion * 7)) & 0x7full;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vneigh_ch_dbg_tb> sim(argc, argv);
    sim.max_time = 20000;
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    sim.check(d->debug_halt_o == 0, "debug halt reset value");
    sim.check(d->debug_resume_o == 0, "debug resume reset value");
    sim.check(d->hastatus0_resumeack_o == 0xffff, "resumeack reset high for all harts");
    sim.check(d->reset_w_neigh_o == 0, "warm reset output deasserted after reset");
    sim.check(d->reset_w_per_minion_o == 0, "per-minion warm reset deasserted after reset");

    d->rst_w_force_i = 1;
    sim.tick();
    sim.check(d->reset_w_neigh_o == 1, "warm reset force drives neighborhood reset");
    sim.check(d->reset_w_per_minion_o == 0xff, "warm reset force drives all minion resets");
    d->rst_w_force_i = 0;
    sim.tick();
    sim.check(d->reset_w_neigh_o == 0, "warm reset output releases");

    d->dmctrl_dmactive_i = 1;
    d->dmctrl_hasel_i = 1;
    d->dmctrl_haltreq_i = 1;
    d->hactrl_hawindow_i = hart(1, 0) | hart(3, 1);
    sim.tick();
    sim.check(d->debug_halt_o == (hart(1, 0) | hart(3, 1)), "dmctrl halt selects hawindow harts");
    d->dbg_halted_i = hart(1, 0);
    sim.tick();
    sim.check(d->debug_halt_o == hart(3, 1), "halt request is suppressed for already halted hart");

    d->dmctrl_haltreq_i = 0;
    d->dmctrl_hasel_i = 0;
    d->hactrl_hawindow_i = 0;
    d->hactrl_hartmask_i = hart(2, 0) | hart(2, 1);
    d->hactrl_resethalt_i = 0x00f0;
    d->dmctrl_ackhavereset_i = 1;
    d->dmctrl_write_enable_i = 1;
    d->dbg_have_reset_i = 0x00ff;
    sim.tick();
    sim.check(d->debug_resethalt_o == 0x00f0, "resethalt follows HACTRL when dmactive");
    sim.check(d->debug_ackhavereset_o == (hart(2, 0) | hart(2, 1)), "ackhavereset masks selected harts with have_reset");

    d->dmctrl_hartreset_i = 1;
    sim.tick();
    sim.check(d->reset_w_per_minion_o == (1u << 2), "hartreset only resets selected minion");
    d->bpam_gpio_hart_reset_req_i = 1;
    sim.tick();
    sim.check(d->reset_w_per_minion_o == 0xff, "GPIO hart reset drives every minion reset");
    d->bpam_gpio_hart_reset_req_i = 0;
    d->dmctrl_hartreset_i = 0;

    d->rst_d_force_i = 1;
    sim.tick();
    d->rst_d_force_i = 0;
    sim.tick();
    d->hactrl_hartmask_i = 0;
    d->dmctrl_ackhavereset_i = 0;
    d->dmctrl_write_enable_i = 0;
    d->dbg_halted_i = 0;
    d->dbg_have_reset_i = 0;

    d->dmctrl_dmactive_i = 1;
    d->dmctrl_hasel_i = 1;
    d->hactrl_hawindow_i = hart(0, 0);
    d->dmctrl_resumereq_i = 1;
    d->dmctrl_write_enable_i = 1;
    sim.tick();
    sim.check((d->debug_resume_o & hart(0, 0)) != 0, "dmctrl resumereq starts resume FSM");
    sim.check((d->hastatus0_resumeack_o & hart(0, 0)) == 0, "resume FSM clears resumeack");
    d->dbg_running_i = hart(0, 0);
    sim.tick();
    sim.tick();
    sim.tick();
    sim.check((d->hastatus0_resumeack_o & hart(0, 0)) != 0, "resume FSM recaptures resumeack in wait state");
    sim.check((d->debug_resume_o & hart(0, 0)) == 0, "resume request deasserts after resumeack recapture");
    d->dmctrl_resumereq_i = 0;
    d->dmctrl_write_enable_i = 0;
    d->dbg_running_i = 0;
    sim.tick();

    d->rst_d_force_i = 1;
    sim.tick();
    d->rst_d_force_i = 0;
    sim.tick();
    d->dmctrl_dmactive_i = 1;
    d->dmctrl_hasel_i = 1;
    d->hactrl_hawindow_i = hart(3, 1);
    d->dmctrl_resumereq_i = 1;
    d->dmctrl_write_enable_i = 1;
    d->dbg_running_i = 0;
    sim.tick();
    sim.check((d->debug_resume_o & hart(3, 1)) != 0, "nonzero-minion resume request starts");
    d->dbg_running_i = hart(3, 1);
    sim.tick();
    sim.tick();
    sim.check((d->hastatus0_resumeack_o & hart(3, 1)) == 0,
              "original minion-local running bug is preserved");
    sim.check((d->debug_resume_o & hart(3, 1)) != 0,
              "resume request remains asserted until minion-0 running bit appears");
    d->dbg_running_i = hart(0, 1);
    sim.tick();
    sim.tick();
    sim.tick();
    sim.check((d->hastatus0_resumeack_o & hart(3, 1)) != 0,
              "minion-0 running bit completes nonzero-minion resume FSM");
    d->dmctrl_resumereq_i = 0;
    d->dmctrl_write_enable_i = 0;
    d->dbg_running_i = 0;
    sim.tick();

    d->rst_d_force_i = 1;
    sim.tick();
    d->rst_d_force_i = 0;
    sim.tick();
    d->dmctrl_dmactive_i = 0;
    d->dmctrl_hasel_i = 0;
    d->hactrl_hawindow_i = 0;
    d->bpam_events_resume_req_i = 1;
    sim.tick();
    sim.check(d->debug_resume_o == 0, "BPAM event resume is first captured into a level latch");
    d->bpam_events_resume_req_i = 0;
    sim.tick();
    sim.check(d->debug_resume_o == 0xffff, "BPAM event resume latches to all harts while not running");
    sim.tick();
    sim.check(d->debug_resume_o == 0xffff, "BPAM event resume remains level after input pulse");
    d->dbg_running_i = 0xffff;
    sim.tick();
    sim.tick();
    sim.check(d->debug_resume_o == 0, "running harts clear event resume level");
    d->dbg_running_i = 0;

    d->bpam_events_tbox_halt_req_i = 1;
    sim.tick();
    sim.check(d->tbox_halt_req_o == 1, "TBOX event halt sets latched halt request");
    d->bpam_events_tbox_halt_req_i = 0;
    sim.tick();
    sim.check(d->tbox_halt_req_o == 1, "TBOX halt remains latched until ack");
    d->tbox_ack_halt_i = 1;
    sim.tick();
    sim.tick();
    sim.check(d->tbox_halt_req_o == 0, "TBOX halt ack clears latched halt request");
    d->tbox_ack_halt_i = 0;

    d->bpam_tbox_single_step_i = 1;
    d->bpam_gpio_tbox_resume_req_i = 1;
    sim.tick();
    sim.check(d->tbox_single_step_o == 1, "TBOX single step is forwarded");
    sim.check(d->tbox_resume_req_o == 1, "TBOX GPIO resume rising level sets resume request");
    d->bpam_gpio_tbox_resume_req_i = 0;
    d->tbox_ack_resume_i = 1;
    sim.tick();
    sim.tick();
    sim.check(d->tbox_resume_req_o == 0, "TBOX resume ack clears latched resume request");
    d->tbox_ack_resume_i = 0;
    d->bpam_tbox_single_step_i = 0;

    d->rst_d_force_i = 1;
    sim.tick();
    d->rst_d_force_i = 0;
    sim.tick();
    d->dbg_halted_i = hart(0, 0);
    d->dbg_running_i = hart(0, 1);
    d->dbg_have_reset_i = hart(0, 0) | hart(0, 1);
    d->hactrl_hartmask_i = hart(0, 0) | hart(0, 1);
    sim.tick();
    sim.check(d->and_anyselected_o == 1, "and/or tree anyselected set");
    sim.check(d->and_anyhalted_o == 1 && d->and_allhalted_o == 0, "and/or tree halted reduction");
    sim.check(d->and_anyrunning_o == 1 && d->and_allrunning_o == 0, "and/or tree running reduction");
    sim.check(d->and_anyhavereset_o == 1 && d->and_allhavereset_o == 1, "and/or tree havereset reduction");
    sim.check(d->and_anyresumeack_o == 1 && d->and_allresumeack_o == 1, "and/or tree resumeack reduction");
    d->hactrl_hartmask_i = 0;
    sim.tick();
    sim.check(d->and_anyselected_o == 0 && d->and_anyhalted_o == 0, "and/or tree clears for no selected harts");

    d->status_monitor_enable_i = 1;
    d->minion_mon_seed2_i = 0x1234567du;
    d->neigh_sm_gpio_i = gpio(2, 0x2a);
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(d->minion_dbg_signals_en_o == (1u << 2), "status monitor enables selected north minion");
    sim.check(mux_lane(d->minion_dbg_signals_mux_flat_o, 0) == 0x2a &&
              mux_lane(d->minion_dbg_signals_mux_flat_o, 3) == 0x2a,
              "status monitor north mux fans out to north minions");
    sim.check(d->neigh_sm_data_low_o == 0x1234567du, "status monitor selects north minion data");

    d->minion_mon_seed6_i = 0x2468ace5u;
    d->neigh_sm_gpio_i = gpio(6, 0x55);
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(d->minion_dbg_signals_en_o == (1u << 6), "status monitor enables selected south minion");
    sim.check(mux_lane(d->minion_dbg_signals_mux_flat_o, 4) == 0x55 &&
              mux_lane(d->minion_dbg_signals_mux_flat_o, 7) == 0x55,
              "status monitor south mux fans out to south minions");
    sim.check(d->neigh_sm_filter_low_o == 0x2468ace5u, "status monitor selects south minion filter");

    d->tbox_mon_seed_i = 0xabcdef07u;
    d->neigh_sm_gpio_i = gpio(8, 0x12);
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(d->tbox_dbg_signals_en_o == 1 && d->minion_dbg_signals_en_o == 0,
              "status monitor enables TBOX target only");
    sim.check(d->tbox_dbg_signals_mux_o == 0x12, "status monitor forwards TBOX mux");
    sim.check(d->neigh_sm_match_low_o == 0xabcdef07u, "status monitor selects TBOX match");

    d->neigh_ch_mon_seed_i = 0xfeed0007u;
    d->neigh_sm_gpio_i = gpio(9, 0x33);
    for (int i = 0; i < 4; ++i) sim.tick();
    sim.check(d->neigh_ch_dbg_signals_en_o == 1 && d->tbox_dbg_signals_en_o == 0,
              "status monitor enables neighborhood-channel target");
    sim.check(d->neigh_ch_dbg_signals_mux_o == 0x33, "status monitor forwards channel mux");
    sim.check(d->neigh_sm_data_low_o == 0xfeed0007u, "status monitor selects channel data");

    return sim.finish();
}

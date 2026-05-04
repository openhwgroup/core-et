// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_top.
//
// Tests:
//   1. Reset: status register reads Ready (0x00)
//   2. APB write/read: config ESR write and readback
//   3. Halt/resume: debug run control FSM
//   4. Status register after start with bad config
//   5. reset_ack is always 0
//   6. Independent reset domains: warm vs debug

#include "Vrbox_top_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_top_tb;

// APB helpers
static void apb_setup(DUT* dut, uint16_t addr, bool write, uint64_t wdata) {
    dut->apb_paddr_i   = addr;
    dut->apb_pwrite_i  = write ? 1 : 0;
    dut->apb_psel_i    = 1;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i  = wdata;
}

static void apb_access(DUT* dut) {
    dut->apb_penable_i = 1;
}

static void apb_idle(DUT* dut) {
    dut->apb_paddr_i   = 0;
    dut->apb_pwrite_i  = 0;
    dut->apb_psel_i    = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i  = 0;
}

// Custom dual reset
static void dual_reset(SimCtrl<DUT>& sim, int cycles) {
    sim.dut->rst_d_ni = 0;
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < cycles; i++) sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    sim.tick();
}

// APB write transaction (full 2-phase)
static void apb_write(SimCtrl<DUT>& sim, uint16_t addr, uint64_t data) {
    apb_setup(sim.dut.get(), addr, true, data);
    sim.tick();
    apb_access(sim.dut.get());
    sim.tick();
    apb_idle(sim.dut.get());
    // Wait for ready
    for (int i = 0; i < 5; i++) sim.tick();
}

// APB read transaction (full 2-phase), returns prdata
static uint64_t apb_read(SimCtrl<DUT>& sim, uint16_t addr) {
    apb_setup(sim.dut.get(), addr, false, 0);
    sim.tick();
    apb_access(sim.dut.get());
    sim.tick();
    apb_idle(sim.dut.get());
    // Wait for pready
    for (int i = 0; i < 5; i++) {
        if (sim.dut->apb_pready_o) break;
        sim.tick();
    }
    return sim.dut->apb_prdata_o;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 800000;

    // Initialize
    apb_idle(sim.dut.get());
    sim.dut->rc_halt_req_events_i   = 0;
    sim.dut->rc_resume_req_events_i = 0;
    sim.dut->rc_halt_req_gpio_i     = 0;
    sim.dut->rc_resume_req_gpio_i   = 0;
    sim.dut->sc_req_ready_i         = 1;
    sim.dut->rst_d_ni = 1;
    sim.dut->rst_w_ni = 1;
    dual_reset(sim, 5);

    // -- Test 1: Status = Ready after reset --
    printf("test 1: status = Ready after reset\n");
    // ESR address for status: register 5 -> addr 0x0005
    uint64_t status = apb_read(sim, 0x0005);
    sim.check((status & 0xFF) == 0x00, "status = Ready (0x00)");
    printf("  status = 0x%02llx\n", (unsigned long long)(status & 0xFF));

    // -- Test 2: APB ESR write and readback --
    printf("test 2: ESR config write/read\n");
    // Write config register (addr 0x0000) with enable bit
    apb_write(sim, 0x0000, 0x0000000000000001ull);

    // Read it back
    uint64_t config = apb_read(sim, 0x0000);
    sim.check(config == 0x0000000000000001ull, "config readback = 1 (enable)");
    printf("  config = 0x%016llx\n", (unsigned long long)config);

    // -- Test 3: Halt/resume FSM --
    printf("test 3: halt/resume FSM\n");
    // Assert halt request and hold until ack
    sim.dut->rc_halt_req_events_i = 1;
    {
        bool got_ack = false;
        for (int i = 0; i < 10; i++) {
            sim.tick();
            if (sim.dut->rc_halt_ack_o) { got_ack = true; break; }
        }
        sim.check(got_ack, "halt_ack asserted after halt_req");
    }
    sim.dut->rc_halt_req_events_i = 0;
    // Wait for ack to clear
    for (int i = 0; i < 5; i++) sim.tick();

    // Now resume
    sim.dut->rc_resume_req_events_i = 1;
    {
        bool got_ack = false;
        for (int i = 0; i < 10; i++) {
            sim.tick();
            if (sim.dut->rc_resume_ack_o) { got_ack = true; break; }
        }
        sim.check(got_ack, "resume_ack asserted after resume_req");
    }
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) sim.tick();

    // After acks clear
    sim.check(sim.dut->rc_halt_ack_o == 0, "halt_ack cleared");
    sim.check(sim.dut->rc_resume_ack_o == 0, "resume_ack cleared");

    // -- Test 4: Start with bad config -> error status --
    printf("test 4: start with bad config\n");
    // Write in_buf_pg = 0 (no pages active)
    apb_write(sim, 0x0001, 0x0);
    // Write out_buf_pg = 0
    apb_write(sim, 0x0003, 0x0);
    // Trigger start (register 6, bit 0)
    apb_write(sim, 0x0006, 0x1);

    // Wait and read status
    for (int i = 0; i < 30; i++) sim.tick();
    status = apb_read(sim, 0x0005);
    uint8_t st = status & 0xFF;
    sim.check(st == 0x80 || st == 0x02, "status Error or Finished after bad start");
    printf("  status = 0x%02x\n", st);

    // -- Test 5: reset_ack is always 0 --
    printf("test 5: reset_ack = 0\n");
    sim.check(sim.dut->rc_reset_ack_o == 0, "reset_ack always 0");

    // -- Test 6: Independent reset domains --
    printf("test 6: independent reset domains\n");

    // Halt the RBOX (debug domain)
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 3; i++) sim.tick();

    // Apply warm reset only — should NOT clear halt state
    sim.dut->rst_w_ni = 0;
    for (int i = 0; i < 3; i++) sim.tick();
    sim.dut->rst_w_ni = 1;
    sim.tick();
    sim.tick();
    // Halt ack may have cleared by now, but halted_q should persist
    // (halt_ack is a pulse, so it cleared during reset cycles)
    // We cannot directly observe halted_q, but we can check resume works

    // Resume should still work because halt state is in debug domain
    sim.dut->rc_resume_req_events_i = 1;
    {
        bool got_ack = false;
        for (int i = 0; i < 10; i++) {
            sim.tick();
            if (sim.dut->rc_resume_ack_o) { got_ack = true; break; }
        }
        sim.check(got_ack, "resume_ack after warm-only reset (debug state preserved)");
    }
    sim.dut->rc_resume_req_events_i = 0;
    for (int i = 0; i < 5; i++) sim.tick();

    // Apply debug reset only — should clear halt state
    sim.dut->rc_halt_req_events_i = 1;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.dut->rc_halt_req_events_i = 0;
    for (int i = 0; i < 3; i++) sim.tick();

    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < 3; i++) sim.tick();
    sim.dut->rst_d_ni = 1;
    sim.tick();
    sim.tick();

    // After debug reset, halt/resume acks should be cleared
    sim.check(sim.dut->rc_halt_ack_o == 0, "halt_ack cleared by debug reset");
    sim.check(sim.dut->rc_resume_ack_o == 0, "resume_ack cleared by debug reset");

    return sim.finish();
}

// Co-simulation test: intpipe_csr_file_fl_barrier (new) vs original.
//
// Exercises the full FSM: idle → send_req → wait_req → write_res → idle.
// Tests multiple barrier transactions, back-to-back operations, and
// various rf_wen_ready timing.

#include "Vcosim_fl_barrier_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_fl_barrier_tb;

static uint32_t rng_state = 0xBADCAFE;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("start_ready",         sim.dut->orig_start_ready,         sim.dut->new_start_ready);
    sim.compare("neigh_req_valid",     sim.dut->orig_neigh_req_valid,     sim.dut->new_neigh_req_valid);
    sim.compare("neigh_req_data",      sim.dut->orig_neigh_req_data,      sim.dut->new_neigh_req_data);
    sim.compare("rf_wen_valid",        sim.dut->orig_rf_wen_valid,        sim.dut->new_rf_wen_valid);
    sim.compare("rf_wen_valid_early",  sim.dut->orig_rf_wen_valid_early,  sim.dut->new_rf_wen_valid_early);
    sim.compare("rf_wen_thread_id",    sim.dut->orig_rf_wen_thread_id,    sim.dut->new_rf_wen_thread_id);
    sim.compare("rf_wen_addr",         sim.dut->orig_rf_wen_addr,         sim.dut->new_rf_wen_addr);
    sim.compare("rf_wen_data",         sim.dut->orig_rf_wen_data,         sim.dut->new_rf_wen_data);
    sim.compare("scoreboard_valid",    sim.dut->orig_scoreboard_valid,    sim.dut->new_scoreboard_valid);
    sim.compare("scoreboard_fp",       sim.dut->orig_scoreboard_fp,       sim.dut->new_scoreboard_fp);
    sim.compare("scoreboard_addr",     sim.dut->orig_scoreboard_addr,     sim.dut->new_scoreboard_addr);
    sim.compare("scoreboard_thread_id",sim.dut->orig_scoreboard_thread_id,sim.dut->new_scoreboard_thread_id);
}

static void idle_inputs(DUT* d) {
    d->stim_start_valid      = 0;
    d->stim_start_data       = 0;
    d->stim_start_waddr      = 0;
    d->stim_start_thread_id  = 0;
    d->stim_neigh_resp_valid = 0;
    d->stim_neigh_resp_data  = 0;
    d->stim_rf_wen_ready     = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.reset(5);
    idle_inputs(sim.dut.get());

    // Phase 1: Post-reset idle
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Phase 2: Complete barrier transactions
    for (int txn = 0; txn < 20; txn++) {
        uint16_t data  = xorshift32() & 0x1FFF;
        uint8_t  waddr = xorshift32() & 0x1F;
        uint8_t  tid   = xorshift32() & 1;
        uint8_t  resp  = xorshift32() & 1;

        // Start
        idle_inputs(sim.dut.get());
        sim.dut->stim_start_valid     = 1;
        sim.dut->stim_start_data      = data;
        sim.dut->stim_start_waddr     = waddr;
        sim.dut->stim_start_thread_id = tid;
        sim.tick();
        compare_all(sim);

        // Send_Req → Wait_Req (automatic transition)
        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);

        // Wait some cycles before neigh response
        int wait = xorshift32() % 5;
        for (int w = 0; w < wait; w++) {
            sim.tick();
            compare_all(sim);
        }

        // Neigh response
        sim.dut->stim_neigh_resp_valid = 1;
        sim.dut->stim_neigh_resp_data  = resp;
        sim.tick();
        compare_all(sim);

        sim.dut->stim_neigh_resp_valid = 0;
        sim.tick();
        compare_all(sim);

        // Wait some cycles before rf_wen_ready
        wait = xorshift32() % 3;
        for (int w = 0; w < wait; w++) {
            sim.tick();
            compare_all(sim);
        }

        // RF write ready
        sim.dut->stim_rf_wen_ready = 1;
        sim.tick();
        compare_all(sim);

        sim.dut->stim_rf_wen_ready = 0;
        sim.tick();
        compare_all(sim);

        // Extra idle
        for (int i = 0; i < 2; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 3: Start valid while not idle (should be ignored)
    idle_inputs(sim.dut.get());
    sim.dut->stim_start_valid = 1;
    sim.dut->stim_start_data  = 0x123;
    sim.dut->stim_start_waddr = 0xA;
    sim.tick();
    compare_all(sim);

    // Try to start again while busy
    sim.dut->stim_start_data  = 0x456;
    sim.dut->stim_start_waddr = 0xB;
    sim.tick();
    compare_all(sim);

    idle_inputs(sim.dut.get());
    // Let it complete
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }
    sim.dut->stim_neigh_resp_valid = 1;
    sim.dut->stim_neigh_resp_data  = 1;
    sim.tick(); compare_all(sim);
    sim.dut->stim_neigh_resp_valid = 0;
    sim.tick(); compare_all(sim);
    sim.dut->stim_rf_wen_ready = 1;
    sim.tick(); compare_all(sim);
    sim.dut->stim_rf_wen_ready = 0;
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Phase 4: Random stimulus
    for (int i = 0; i < 5000; i++) {
        uint32_t r = xorshift32();
        sim.dut->stim_start_valid      = (r >> 0) & 1;
        sim.dut->stim_start_data       = (r >> 1) & 0x1FFF;
        sim.dut->stim_start_waddr      = (r >> 14) & 0x1F;
        sim.dut->stim_start_thread_id  = (r >> 19) & 1;
        sim.dut->stim_neigh_resp_valid = (r >> 20) & 1;
        sim.dut->stim_neigh_resp_data  = (r >> 21) & 1;
        sim.dut->stim_rf_wen_ready     = (r >> 22) & 1;
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

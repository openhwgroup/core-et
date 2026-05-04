// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: debug_breakpoint (new) vs (original).
//
// Tests BOTH parameter values (TriggerType=0 insn, TriggerType=1 data)
// in the same testbench. Drives random addresses, configs, privilege
// modes, commands. 50K random combinations per config.

#include "Vcosim_debug_breakpoint_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_debug_breakpoint_tb;

static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    return ((uint64_t)xorshift32() << 32) | xorshift32();
}

// debug_bp_e values (3-bit enum)
enum {
    DBG_BP_LOAD         = 0,
    DBG_BP_STORE        = 1,
    DBG_BP_TENSOR_LOAD  = 2,
    DBG_BP_TENSOR_STORE = 3,
    DBG_BP_AMO          = 4,
    DBG_BP_CO           = 5,
};

// Valid privilege levels
static const uint8_t prv_vals[] = { 0, 1, 3 };  // U, S, M

// minion_bp_ctrl_t packed: {dmode, action, match, m, s, u, execute, store, load} = 9 bits
static uint16_t make_bp_ctrl(bool dmode, bool action, bool match,
                              bool m, bool s, bool u,
                              bool execute, bool store, bool load) {
    return ((uint16_t)dmode   << 8) |
           ((uint16_t)action  << 7) |
           ((uint16_t)match   << 6) |
           ((uint16_t)m       << 5) |
           ((uint16_t)s       << 4) |
           ((uint16_t)u       << 3) |
           ((uint16_t)execute << 2) |
           ((uint16_t)store   << 1) |
           ((uint16_t)load    << 0);
}

static void compare_all(CosimCtrl<DUT>& sim, const char* desc) {
    auto* d = sim.dut.get();
    d->eval();

    // Insn breakpoint (TriggerType=0)
    sim.compare(std::string(desc) + "_insn_timing",
                (uint64_t)d->orig_insn_timing_o,
                (uint64_t)d->new_insn_timing_o);
    sim.compare(std::string(desc) + "_insn_xcpt",
                (uint64_t)d->orig_insn_xcpt_o,
                (uint64_t)d->new_insn_xcpt_o);
    sim.compare(std::string(desc) + "_insn_enter_debug",
                (uint64_t)d->orig_insn_enter_debug_o,
                (uint64_t)d->new_insn_enter_debug_o);
    sim.compare(std::string(desc) + "_insn_update",
                (uint64_t)d->orig_insn_update_o,
                (uint64_t)d->new_insn_update_o);

    // Data breakpoint (TriggerType=1)
    sim.compare(std::string(desc) + "_data_timing",
                (uint64_t)d->orig_data_timing_o,
                (uint64_t)d->new_data_timing_o);
    sim.compare(std::string(desc) + "_data_xcpt",
                (uint64_t)d->orig_data_xcpt_o,
                (uint64_t)d->new_data_xcpt_o);
    sim.compare(std::string(desc) + "_data_enter_debug",
                (uint64_t)d->orig_data_enter_debug_o,
                (uint64_t)d->new_data_enter_debug_o);
    sim.compare(std::string(desc) + "_data_update",
                (uint64_t)d->orig_data_update_o,
                (uint64_t)d->new_data_update_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    auto* d = sim.dut.get();
    d->rst_ni = 1;

    // ── Phase 1: Targeted tests ──
    printf("phase 1: targeted tests\n");

    // All zeros — disabled
    d->stim_control = 0;
    d->stim_in_debug = 0;
    d->stim_prv = 0;
    for (int w = 0; w < 3; w++) d->stim_tdata2[w] = 0;
    d->stim_address = 0;
    d->stim_cmd = 0;
    d->stim_valid = 0;
    d->stim_thread_id = 0;
    compare_all(sim, "all_zero");

    // Insn breakpoint: execute enabled, valid, M-mode, address match
    // control: execute=1, m=1, action=0 (exception)
    {
        uint16_t ctrl0 = make_bp_ctrl(0, 0, 0, 1, 0, 0, 1, 0, 0);
        uint16_t ctrl1 = 0;
        d->stim_control = ((uint32_t)ctrl1 << 9) | ctrl0;
    }
    d->stim_prv = (3 << 0) | (0 << 2);  // thread0=M, thread1=U
    {
        // tdata2: thread0 = 0x1234, thread1 = 0
        uint32_t tdata_words[3] = {};
        tdata_words[0] = 0x00001234;
        tdata_words[1] = 0x00000000;
        tdata_words[2] = 0x00000000;
        for (int w = 0; w < 3; w++)
            d->stim_tdata2[w] = tdata_words[w];
    }
    d->stim_address = 0x1234;  // sign-extended match
    d->stim_valid = 1;
    d->stim_thread_id = 0;
    compare_all(sim, "insn_match_m_mode");

    // Data breakpoint: load enabled, valid, M-mode, address match
    {
        uint16_t ctrl0 = make_bp_ctrl(0, 0, 0, 1, 0, 0, 0, 0, 1);  // load=1
        d->stim_control = ctrl0;
    }
    d->stim_cmd = DBG_BP_LOAD;
    compare_all(sim, "data_load_match");

    // Store command with store enabled
    {
        uint16_t ctrl0 = make_bp_ctrl(0, 0, 0, 1, 0, 0, 0, 1, 0);  // store=1
        d->stim_control = ctrl0;
    }
    d->stim_cmd = DBG_BP_STORE;
    compare_all(sim, "data_store_match");

    // Action=1 — enter debug instead of exception
    {
        uint16_t ctrl0 = make_bp_ctrl(0, 1, 0, 1, 0, 0, 1, 0, 0);  // action=1
        d->stim_control = ctrl0;
    }
    d->stim_cmd = 0;
    compare_all(sim, "action_debug");

    // Address mismatch
    d->stim_address = 0x5678;
    compare_all(sim, "addr_mismatch");

    // In debug mode — disabled
    d->stim_address = 0x1234;
    d->stim_in_debug = 0x01;
    compare_all(sim, "in_debug_disabled");
    d->stim_in_debug = 0;

    // Privilege mismatch (S-mode, but only M enabled)
    d->stim_prv = (1 << 0);  // thread0=S
    compare_all(sim, "prv_mismatch");

    // Match mode (NAPOT masking)
    d->stim_prv = (3 << 0);  // thread0=M
    {
        uint16_t ctrl0 = make_bp_ctrl(0, 0, 1, 1, 0, 0, 1, 0, 0);  // match=1
        d->stim_control = ctrl0;
    }
    compare_all(sim, "match_napot");

    // Thread 1 test
    d->stim_thread_id = 1;
    {
        uint16_t ctrl1 = make_bp_ctrl(0, 0, 0, 0, 1, 0, 1, 0, 0);  // S-mode enabled
        d->stim_control = ((uint32_t)ctrl1 << 9);
    }
    d->stim_prv = (1 << 2);  // thread1=S
    {
        // tdata2: thread1 = 0x1234
        uint32_t tdata_words[3] = {};
        tdata_words[1] = 0x00001234;
        tdata_words[2] = 0x00000000;
        for (int w = 0; w < 3; w++)
            d->stim_tdata2[w] = tdata_words[w];
    }
    compare_all(sim, "thread1_s_mode");

    // ── Phase 2: Random sweep (50K) ──
    printf("phase 2: random sweep (50000 combinations)\n");
    int phase2_mm = 0;

    for (int i = 0; i < 50000; i++) {
        // Random control (18 bits: 2 x 9-bit minion_bp_ctrl_t)
        d->stim_control = xorshift32() & 0x3FFFF;

        d->stim_in_debug = xorshift32() & 0x3;

        // Random privilege (bias towards valid values)
        uint32_t r = xorshift32();
        uint8_t prv0 = prv_vals[r % 3];
        uint8_t prv1 = prv_vals[(r >> 4) % 3];
        d->stim_prv = prv0 | (prv1 << 2);

        // Random tdata2 (96 bits = 3 x 32-bit words)
        for (int w = 0; w < 3; w++)
            d->stim_tdata2[w] = xorshift32();

        // Random address (49 bits)
        uint64_t addr = xorshift64() & ((1ULL << 49) - 1);
        d->stim_address = addr;

        // Random cmd (valid range 0..5)
        d->stim_cmd = xorshift32() % 6;

        d->stim_valid = xorshift32() & 1;
        d->stim_thread_id = xorshift32() & 1;

        d->eval();

        int mm_before = sim.mismatches;

        // Compare insn outputs
        sim.compare("rand_insn_timing",
                    (uint64_t)d->orig_insn_timing_o,
                    (uint64_t)d->new_insn_timing_o);
        sim.compare("rand_insn_xcpt",
                    (uint64_t)d->orig_insn_xcpt_o,
                    (uint64_t)d->new_insn_xcpt_o);
        sim.compare("rand_insn_enter_debug",
                    (uint64_t)d->orig_insn_enter_debug_o,
                    (uint64_t)d->new_insn_enter_debug_o);
        sim.compare("rand_insn_update",
                    (uint64_t)d->orig_insn_update_o,
                    (uint64_t)d->new_insn_update_o);

        // Compare data outputs
        sim.compare("rand_data_timing",
                    (uint64_t)d->orig_data_timing_o,
                    (uint64_t)d->new_data_timing_o);
        sim.compare("rand_data_xcpt",
                    (uint64_t)d->orig_data_xcpt_o,
                    (uint64_t)d->new_data_xcpt_o);
        sim.compare("rand_data_enter_debug",
                    (uint64_t)d->orig_data_enter_debug_o,
                    (uint64_t)d->new_data_enter_debug_o);
        sim.compare("rand_data_update",
                    (uint64_t)d->orig_data_update_o,
                    (uint64_t)d->new_data_update_o);

        if (sim.mismatches > mm_before) {
            phase2_mm++;
            if (phase2_mm <= 20) {
                printf("  iter=%d ctrl=0x%05x dbg=%d prv=0x%x addr=0x%013llx "
                       "cmd=%d valid=%d tid=%d\n",
                       i, d->stim_control, d->stim_in_debug, d->stim_prv,
                       (unsigned long long)d->stim_address,
                       d->stim_cmd, d->stim_valid, d->stim_thread_id);
            }
        }
    }

    if (phase2_mm > 20)
        printf("  ... and %d more mismatches (truncated)\n", phase2_mm - 20);

    return sim.finish();
}

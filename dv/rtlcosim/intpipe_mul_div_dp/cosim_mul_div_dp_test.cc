// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_mul_div_dp (new) vs original.
//
// Latch-based dual-phase datapath. Drives all control inputs randomly,
// drives operands randomly, and compares all 6 outputs every cycle for
// 20K random cycles.

#include "Vcosim_mul_div_dp_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_mul_div_dp_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    uint64_t hi = xorshift32();
    uint64_t lo = xorshift32();
    return (hi << 32) | lo;
}

// Compare all 6 outputs between new and original.
static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("md_resp_data_2p",   (uint64_t)sim.dut->orig_md_resp_data_2p,
                                     (uint64_t)sim.dut->new_md_resp_data_2p);
    sim.compare("md_mplr_2d",       (uint8_t)sim.dut->orig_md_mplr_2d,
                                     (uint8_t)sim.dut->new_md_mplr_2d);
    sim.compare("md_rem_sgn_1d",    (uint8_t)sim.dut->orig_md_rem_sgn_1d,
                                     (uint8_t)sim.dut->new_md_rem_sgn_1d);
    sim.compare("md_zero_rem_1d",   (uint8_t)sim.dut->orig_md_zero_rem_1d,
                                     (uint8_t)sim.dut->new_md_zero_rem_1d);
    sim.compare("md_nxt_quot_1q",   (uint8_t)sim.dut->orig_md_nxt_quot_1q,
                                     (uint8_t)sim.dut->new_md_nxt_quot_1q);
    sim.compare("md_div_by_zero_1p",(uint8_t)sim.dut->orig_md_div_by_zero_1p,
                                     (uint8_t)sim.dut->new_md_div_by_zero_1p);
}

// Randomize all control inputs.
static void randomize_inputs(DUT* dut) {
    dut->stim_ex_op1_2p                  = xorshift64();
    dut->stim_ex_op2_2p                  = xorshift64();
    dut->stim_op1_sgnd_2p                = xorshift32() & 1;
    dut->stim_op2_sgnd_2p                = xorshift32() & 1;
    dut->stim_ex_dw_2p                   = xorshift32() & 1;

    dut->stim_mdctl_besl_0_1p            = xorshift32() & 0x7;
    dut->stim_mdctl_besl_1_1p            = xorshift32() & 0x7;
    dut->stim_mdctl_besl_2_1p            = xorshift32() & 0x7;
    dut->stim_mdctl_besl_3_1p            = xorshift32() & 0x7;
    dut->stim_mdctl_start_op_2p          = xorshift32() & 1;
    dut->stim_mdctl_fnl_pp_1p            = xorshift32() & 1;
    dut->stim_mdctl_dw_1q                = xorshift32() & 1;
    dut->stim_mdctl_dw_2q                = xorshift32() & 1;
    dut->stim_mdctl_acc_v1_sl_1d         = xorshift32() & 0x3;
    dut->stim_mdctl_acc_v2_sl_1d         = xorshift32() & 0x3;
    dut->stim_mdctl_add_ain_sl_1p        = xorshift32() & 1;
    dut->stim_mdctl_add_bin_sl_1p        = xorshift32() & 0x3;
    dut->stim_mdctl_se_zero_1p           = xorshift32() & 1;
    dut->stim_mdctl_fnl_mul_cyc_1p       = xorshift32() & 1;
    dut->stim_mdctl_start_div_2p         = xorshift32() & 1;
    dut->stim_mdctl_fc_zr_acc_1p         = xorshift32() & 1;
    dut->stim_mdctl_dvsr_sgn_1p          = xorshift32() & 1;
    dut->stim_mdctl_div_cin_1p           = xorshift32() & 1;
    dut->stim_mdctl_resp_sl_2p           = xorshift32() & 0x3;

    dut->stim_mdctl_clken_mpcnd_1d       = xorshift32() & 1;
    dut->stim_mdctl_clken_mul_ph1_2d     = xorshift32() & 1;
    dut->stim_mdctl_clken_muldvsr_ph2_1d = xorshift32() & 1;
    dut->stim_mdctl_clken_acc_v1_2p      = xorshift32() & 1;
    dut->stim_mdctl_clken_acc_v1_1p      = xorshift32() & 1;
    dut->stim_mdctl_clken_acc_v2_2p      = xorshift32() & 1;
    dut->stim_mdctl_clken_acc_v2_1p      = xorshift32() & 1;
}

// Drive idle / zeroed inputs.
static void drive_idle(DUT* dut) {
    dut->stim_ex_op1_2p                  = 0;
    dut->stim_ex_op2_2p                  = 0;
    dut->stim_op1_sgnd_2p                = 0;
    dut->stim_op2_sgnd_2p                = 0;
    dut->stim_ex_dw_2p                   = 0;

    dut->stim_mdctl_besl_0_1p            = 0x7; // all-ones = inactive
    dut->stim_mdctl_besl_1_1p            = 0x7;
    dut->stim_mdctl_besl_2_1p            = 0x7;
    dut->stim_mdctl_besl_3_1p            = 0x7;
    dut->stim_mdctl_start_op_2p          = 0;
    dut->stim_mdctl_fnl_pp_1p            = 0;
    dut->stim_mdctl_dw_1q                = 0;
    dut->stim_mdctl_dw_2q                = 0;
    dut->stim_mdctl_acc_v1_sl_1d         = 0;
    dut->stim_mdctl_acc_v2_sl_1d         = 0;
    dut->stim_mdctl_add_ain_sl_1p        = 0;
    dut->stim_mdctl_add_bin_sl_1p        = 0;
    dut->stim_mdctl_se_zero_1p           = 0;
    dut->stim_mdctl_fnl_mul_cyc_1p       = 0;
    dut->stim_mdctl_start_div_2p         = 0;
    dut->stim_mdctl_fc_zr_acc_1p         = 0;
    dut->stim_mdctl_dvsr_sgn_1p          = 0;
    dut->stim_mdctl_div_cin_1p           = 0;
    dut->stim_mdctl_resp_sl_2p           = 0;

    dut->stim_mdctl_clken_mpcnd_1d       = 0;
    dut->stim_mdctl_clken_mul_ph1_2d     = 0;
    dut->stim_mdctl_clken_muldvsr_ph2_1d = 0;
    dut->stim_mdctl_clken_acc_v1_2p      = 0;
    dut->stim_mdctl_clken_acc_v1_1p      = 0;
    dut->stim_mdctl_clken_acc_v2_2p      = 0;
    dut->stim_mdctl_clken_acc_v2_1p      = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    // ================================================================
    // Phase 1: Hold idle for several cycles to let both modules settle
    // ================================================================
    printf("phase 1: idle settle\n");
    drive_idle(sim.dut.get());
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_outputs(sim);
    }

    // ================================================================
    // Phase 2: 20K random cycles
    // ================================================================
    printf("phase 2: 20K random cycles\n");
    for (int i = 0; i < 20000; i++) {
        randomize_inputs(sim.dut.get());
        sim.tick();
        compare_outputs(sim);

        if (sim.mismatches > 100) {
            printf("  Too many mismatches, stopping at cycle %d\n", i);
            break;
        }
    }

    // ================================================================
    // Phase 3: Drain with idle
    // ================================================================
    printf("phase 3: drain\n");
    for (int i = 0; i < 20; i++) {
        drive_idle(sim.dut.get());
        sim.tick();
        compare_outputs(sim);
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: intpipe_mul_div_ctl (new) vs original.
//
// Latch-based dual-phase controller. Drives req interface and dp
// feedback signals randomly and compares all outputs every cycle for
// 20K random cycles.

#include "Vcosim_mul_div_ctl_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_mul_div_ctl_tb;

// Simple xorshift32 PRNG for reproducible results.
static uint32_t rng_state = 0xDEADBEEF;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

// Compare all outputs between new and original.
static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("resp_dest",               (uint8_t)sim.dut->orig_resp_dest,
                                            (uint8_t)sim.dut->new_resp_dest);
    sim.compare("req_ready",               (uint8_t)sim.dut->orig_req_ready,
                                            (uint8_t)sim.dut->new_req_ready);
    sim.compare("resp_valid",              (uint8_t)sim.dut->orig_resp_valid,
                                            (uint8_t)sim.dut->new_resp_valid);
    sim.compare("resp_valid_early",        (uint8_t)sim.dut->orig_resp_valid_early,
                                            (uint8_t)sim.dut->new_resp_valid_early);
    sim.compare("op1_sgnd_2p",             (uint8_t)sim.dut->orig_op1_sgnd_2p,
                                            (uint8_t)sim.dut->new_op1_sgnd_2p);
    sim.compare("op2_sgnd_2p",             (uint8_t)sim.dut->orig_op2_sgnd_2p,
                                            (uint8_t)sim.dut->new_op2_sgnd_2p);
    sim.compare("mdctl_besl_0_1p",         (uint8_t)sim.dut->orig_mdctl_besl_0_1p,
                                            (uint8_t)sim.dut->new_mdctl_besl_0_1p);
    sim.compare("mdctl_besl_1_1p",         (uint8_t)sim.dut->orig_mdctl_besl_1_1p,
                                            (uint8_t)sim.dut->new_mdctl_besl_1_1p);
    sim.compare("mdctl_besl_2_1p",         (uint8_t)sim.dut->orig_mdctl_besl_2_1p,
                                            (uint8_t)sim.dut->new_mdctl_besl_2_1p);
    sim.compare("mdctl_besl_3_1p",         (uint8_t)sim.dut->orig_mdctl_besl_3_1p,
                                            (uint8_t)sim.dut->new_mdctl_besl_3_1p);
    sim.compare("mdctl_start_op_2p",       (uint8_t)sim.dut->orig_mdctl_start_op_2p,
                                            (uint8_t)sim.dut->new_mdctl_start_op_2p);
    sim.compare("mdctl_fnl_pp_1p",         (uint8_t)sim.dut->orig_mdctl_fnl_pp_1p,
                                            (uint8_t)sim.dut->new_mdctl_fnl_pp_1p);
    sim.compare("mdctl_dw_1q",             (uint8_t)sim.dut->orig_mdctl_dw_1q,
                                            (uint8_t)sim.dut->new_mdctl_dw_1q);
    sim.compare("mdctl_dw_2q",             (uint8_t)sim.dut->orig_mdctl_dw_2q,
                                            (uint8_t)sim.dut->new_mdctl_dw_2q);
    sim.compare("mdctl_acc_v1_sl_1d",      (uint8_t)sim.dut->orig_mdctl_acc_v1_sl_1d,
                                            (uint8_t)sim.dut->new_mdctl_acc_v1_sl_1d);
    sim.compare("mdctl_acc_v2_sl_1d",      (uint8_t)sim.dut->orig_mdctl_acc_v2_sl_1d,
                                            (uint8_t)sim.dut->new_mdctl_acc_v2_sl_1d);
    sim.compare("mdctl_add_ain_sl_1p",     (uint8_t)sim.dut->orig_mdctl_add_ain_sl_1p,
                                            (uint8_t)sim.dut->new_mdctl_add_ain_sl_1p);
    sim.compare("mdctl_add_bin_sl_1p",     (uint8_t)sim.dut->orig_mdctl_add_bin_sl_1p,
                                            (uint8_t)sim.dut->new_mdctl_add_bin_sl_1p);
    sim.compare("mdctl_se_zero_1p",        (uint8_t)sim.dut->orig_mdctl_se_zero_1p,
                                            (uint8_t)sim.dut->new_mdctl_se_zero_1p);
    sim.compare("mdctl_fnl_mul_cyc_1p",    (uint8_t)sim.dut->orig_mdctl_fnl_mul_cyc_1p,
                                            (uint8_t)sim.dut->new_mdctl_fnl_mul_cyc_1p);
    sim.compare("mdctl_start_div_2p",      (uint8_t)sim.dut->orig_mdctl_start_div_2p,
                                            (uint8_t)sim.dut->new_mdctl_start_div_2p);
    sim.compare("mdctl_fc_zr_acc_1p",      (uint8_t)sim.dut->orig_mdctl_fc_zr_acc_1p,
                                            (uint8_t)sim.dut->new_mdctl_fc_zr_acc_1p);
    sim.compare("mdctl_dvsr_sgn_1p",       (uint8_t)sim.dut->orig_mdctl_dvsr_sgn_1p,
                                            (uint8_t)sim.dut->new_mdctl_dvsr_sgn_1p);
    sim.compare("mdctl_div_cin_1p",        (uint8_t)sim.dut->orig_mdctl_div_cin_1p,
                                            (uint8_t)sim.dut->new_mdctl_div_cin_1p);
    sim.compare("mdctl_resp_sl_2p",        (uint8_t)sim.dut->orig_mdctl_resp_sl_2p,
                                            (uint8_t)sim.dut->new_mdctl_resp_sl_2p);
    sim.compare("mdctl_clken_mpcnd_1d",    (uint8_t)sim.dut->orig_mdctl_clken_mpcnd_1d,
                                            (uint8_t)sim.dut->new_mdctl_clken_mpcnd_1d);
    sim.compare("mdctl_clken_mul_ph1_2d",  (uint8_t)sim.dut->orig_mdctl_clken_mul_ph1_2d,
                                            (uint8_t)sim.dut->new_mdctl_clken_mul_ph1_2d);
    sim.compare("mdctl_clken_muldvsr_ph2_1d", (uint8_t)sim.dut->orig_mdctl_clken_muldvsr_ph2_1d,
                                               (uint8_t)sim.dut->new_mdctl_clken_muldvsr_ph2_1d);
    sim.compare("mdctl_clken_acc_v1_2p",   (uint8_t)sim.dut->orig_mdctl_clken_acc_v1_2p,
                                            (uint8_t)sim.dut->new_mdctl_clken_acc_v1_2p);
    sim.compare("mdctl_clken_acc_v1_1p",   (uint8_t)sim.dut->orig_mdctl_clken_acc_v1_1p,
                                            (uint8_t)sim.dut->new_mdctl_clken_acc_v1_1p);
    sim.compare("mdctl_clken_acc_v2_2p",   (uint8_t)sim.dut->orig_mdctl_clken_acc_v2_2p,
                                            (uint8_t)sim.dut->new_mdctl_clken_acc_v2_2p);
    sim.compare("mdctl_clken_acc_v2_1p",   (uint8_t)sim.dut->orig_mdctl_clken_acc_v2_1p,
                                            (uint8_t)sim.dut->new_mdctl_clken_acc_v2_1p);
}

// Drive idle inputs.
static void drive_idle(DUT* dut) {
    dut->stim_req_fn             = 0;
    dut->stim_req_fn_early       = 0;
    dut->stim_req_valid_early    = 0;
    dut->stim_resp_ready         = 1;
    dut->stim_req_dest           = 0;
    dut->stim_kill               = 0;
    dut->stim_ex_dw_2p           = 0;
    dut->stim_ex_op1_msb_2p      = 0;
    dut->stim_ex_op2_msb_2p      = 0;
    dut->stim_md_mplr_2d         = 0;
    dut->stim_md_rem_sgn_1d      = 0;
    dut->stim_md_zero_rem_1d     = 0;
    dut->stim_md_nxt_quot_1q     = 0;
    dut->stim_md_div_by_zero_1p  = 0;
}

// Randomize all inputs.
static void randomize_inputs(DUT* dut) {
    dut->stim_req_fn             = xorshift32() & 0x1F;
    dut->stim_req_fn_early       = xorshift32() & 0x1F;
    dut->stim_req_valid_early    = xorshift32() & 1;
    dut->stim_resp_ready         = xorshift32() & 1;
    dut->stim_req_dest           = xorshift32() & 0x7F;
    dut->stim_kill               = xorshift32() & 1;
    dut->stim_ex_dw_2p           = xorshift32() & 1;
    dut->stim_ex_op1_msb_2p      = xorshift32() & 1;
    dut->stim_ex_op2_msb_2p      = xorshift32() & 1;
    dut->stim_md_mplr_2d         = xorshift32() & 0xFF;
    dut->stim_md_rem_sgn_1d      = xorshift32() & 1;
    dut->stim_md_zero_rem_1d     = xorshift32() & 1;
    dut->stim_md_nxt_quot_1q     = xorshift32() & 1;
    dut->stim_md_div_by_zero_1p  = xorshift32() & 1;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;

    // ================================================================
    // Reset
    // ================================================================
    drive_idle(sim.dut.get());
    sim.reset(10);

    // Let things settle after reset
    printf("phase 1: post-reset settle\n");
    for (int i = 0; i < 10; i++) {
        drive_idle(sim.dut.get());
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

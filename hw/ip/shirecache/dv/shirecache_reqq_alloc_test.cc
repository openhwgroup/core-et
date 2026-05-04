// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_bank_reqq_alloc unit test (IS_L2=1).
// Tests bank_ready gating, alloc flow, and AD pipeline.
// Opcode mapping correctness validated by RTL cosim.

#include "Vshirecache_reqq_alloc_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstring>

using DUT = Vshirecache_reqq_alloc_tb;

static void clear_inputs(DUT* d) {
    d->bank_valid_i = 0;
    d->bank_opcode_i = 0;
    d->bank_subopcode_i = 0;
    d->bank_address_i = 0;
    d->bank_qwen_i = 0;
    d->bank_size_i = 6;
    d->bank_write_is_partial_i = 0;
    d->bank_port_id_i = 0;
    d->bank_source_i = 0;
    d->bank_trans_id_i = 0;
    d->bank_qos_i = 0;
    d->bank_id_i = 0;
    d->bank_wdata_i = 0;
    d->dataq_bank_ready_i = 1;
    d->pipe_idx_cop_req_valid_i = 0;
    d->pipe_idx_cop_l2_busy_i = 0;
    d->pipe_idx_cop_l3_busy_i = 0;
    d->pipe_idx_cop_aborted_i = 0;
    d->mbist_on_i = 0;
    d->esr_wr_reqq_entries_i = 0;
    memset(&d->bank_data_i, 0, sizeof(d->bank_data_i));
    memset(&d->bank_ben_i, 0, sizeof(d->bank_ben_i));
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    // Let clock gate settle
    for (int i = 0; i < 3; i++) sim.tick();

    // ── Test 1: bank_ready gating ───────────────────────
    printf("test 1: bank_ready gating\n");
    {
        // With dataq ready and no stalls, bank_ready should be 1
        // Need bank_valid to enable the clock gate
        d->dataq_bank_ready_i = 1;
        d->bank_valid_i = 1;
        d->bank_opcode_i = 0;
        d->bank_address_i = 0;
        for (int i = 0; i < 3; i++) sim.tick();
        sim.check(d->bank_ready_o == 1, "bank_ready=1 when idle");
        d->bank_valid_i = 0;

        // dataq not ready → bank_ready=0
        d->dataq_bank_ready_i = 0;
        sim.tick();
        sim.check(d->bank_ready_o == 0, "bank_ready=0 when dataq not ready");
        d->dataq_bank_ready_i = 1;
        sim.tick();
    }

    // ── Test 2: idx_cop stall blocks bank ───────────────
    printf("test 2: idx_cop stall\n");
    {
        d->pipe_idx_cop_l2_busy_i = 1;
        for (int i = 0; i < 3; i++) sim.tick(); // mbist_on_r registered
        sim.check(d->bank_ready_o == 0, "bank_ready=0 during idx_cop_l2_busy");
        d->pipe_idx_cop_l2_busy_i = 0;
        for (int i = 0; i < 3; i++) sim.tick();
    }

    // ── Test 3: mbist_on stall ──────────────────────────
    printf("test 3: mbist_on stall\n");
    {
        d->mbist_on_i = 1;
        d->bank_valid_i = 1; // enable clock gate
        d->dataq_bank_ready_i = 1;
        for (int i = 0; i < 3; i++) sim.tick(); // mbist_on_r is registered
        sim.check(d->bank_ready_o == 0, "bank_ready=0 during mbist_on");
        d->mbist_on_i = 0;
        d->bank_valid_i = 0;
        for (int i = 0; i < 3; i++) sim.tick();
    }

    // ── Test 4: Basic alloc ─────────────────────────────
    printf("test 4: basic alloc\n");
    {
        clear_inputs(d);
        d->dataq_bank_ready_i = 1;
        d->bank_valid_i = 1;
        d->bank_opcode_i = 0; // ReqRead
        d->bank_address_i = 0x1000;
        sim.tick();
        sim.check(d->alloc_valid_ag_o == 1, "alloc_valid_ag on Read");

        // AD pipeline: valid_ad appears next cycle (clock gate may delay)
        clear_inputs(d);
        // Force clock on by keeping bank_valid high briefly
        d->bank_valid_i = 1; d->dataq_bank_ready_i = 1;
        sim.tick();
        d->bank_valid_i = 0;
        sim.check(d->alloc_valid_ad_o == 1, "alloc_valid_ad 1 cycle after AG");
        sim.tick();
        sim.check(d->alloc_valid_ad_o == 0, "alloc_valid_ad clears");
    }

    // ── Test 5: Sequential allocs produce different IDs ──
    printf("test 5: sequential alloc IDs\n");
    {
        int seen_ids[4] = {};
        for (int i = 0; i < 4; i++) {
            clear_inputs(d);
            d->dataq_bank_ready_i = 1;
            d->bank_valid_i = 1;
            d->bank_opcode_i = 0; // ReqRead
            d->bank_address_i = 0x2000 + (i * 0x100);
            sim.tick();
            sim.check(d->alloc_valid_ag_o == 1, "alloc valid");
            seen_ids[i] = d->reqq_alloc_id_ag_o;
            clear_inputs(d);
            sim.tick(); // let AD pass
            sim.tick();
        }
        // Verify all IDs are distinct
        bool all_distinct = true;
        for (int i = 0; i < 4; i++)
            for (int j = i+1; j < 4; j++)
                if (seen_ids[i] == seen_ids[j]) all_distinct = false;
        sim.check(all_distinct, "4 sequential alloc IDs are distinct");
    }

    // ── Test 6: reqq_full ───────────────────────────────
    printf("test 6: reqq_full\n");
    {
        // With all entries free (TB ties reqq_entry_valid=0), reqq_full=0
        sim.check(d->reqq_full_o == 0, "reqq_full=0 with all free");
    }

    // ── Test 7: err_valid on unknown opcode ─────────────
    printf("test 7: error on unknown opcode\n");
    {
        clear_inputs(d);
        d->dataq_bank_ready_i = 1;
        d->bank_valid_i = 1;
        d->bank_opcode_i = 16; // ReqAtomicRsp — illegal for L2, maps to OrigErr
        d->bank_address_i = 0x3000;
        sim.tick();
        sim.check(d->alloc_valid_ag_o == 1, "alloc fires for unknown opcode");
        sim.check(d->alloc_err_valid_o == 1, "err_valid on unknown opcode");
        clear_inputs(d);
        sim.tick(); sim.tick();
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank vs shirecache_bank
//
// Both modules are full bank containers with all sub-modules.
// This C++ driver:
//   1. Resets both modules
//   2. Waits for idx_cop_sm post-reset ALL_INV to complete
//   3. Sends L2 read/write requests and compares key outputs
//   4. Verifies idle behavior (no spurious outputs)
//   5. Tests back-to-back requests
//
// NOTE: The original bank has an internal ESR block that initializes with
// default values.  The new bank takes esr_ctl as input.  We drive esr_ctl
// to match the defaults so behavior should be identical after reset.
//
// Some outputs may NOT match due to known architectural differences:
//   - trans_id masking: original masks trans_id under EVL_SIMULATION
//     (but cosim is compiled with -DET_ASSERT_OFF, defaults apply)
//   - ESR status registers: original builds them internally, new outputs them
//   - Trace/perfmon use different free-running vs gated clock
//
// We focus on the PRIMARY external interface signals.

#include "Vcosim_bank_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include "verilated.h"

using DUT = Vcosim_bank_tb;

// ── etlink opcodes ──
static constexpr uint8_t OP_RD         = 0x04;  // Read
static constexpr uint8_t OP_WR         = 0x05;  // Write

// ── Size encoding ──
static constexpr uint8_t SIZE_DW       = 3;  // 64b
static constexpr uint8_t SIZE_LINE     = 6;  // 512b

// ── Clear all bank inputs ──
static void clear_inputs(DUT* dut) {
    dut->bank_req_valid_i                    = 0;
    dut->bri_port_id_i                       = 0;
    dut->bri_trans_id_i                      = 0;
    dut->bri_opcode_i                        = 0;
    dut->bri_subopcode_i                     = 0;
    dut->bri_address_i                       = 0;
    dut->bri_size_i                          = 0;
    dut->bri_qwen_i                          = 0;
    for (int i = 0; i < 16; i++) dut->bri_data_i[i] = 0;
    dut->bri_wdata_i                         = 0;
    dut->bri_id_i                            = 0;
    dut->bri_source_i                        = 0;

    // Response credits: always ready
    dut->bank_rsp_ready_i                    = 0xF;
    dut->bank_rsp_2_credits_i                = 0;

    // Mesh backpressure: always ready
    dut->to_l3_mesh_master_bank_req_ready_i  = 1;
    dut->to_l3_mesh_master_bank_rsp_valid_i  = 0;
    dut->to_sys_mesh_master_bank_req_ready_i = 1;
    dut->to_sys_mesh_master_bank_rsp_valid_i = 0;

    // L3 slave: idle
    dut->l3_mesh_slave_bank_clk_en_i         = 0;
    dut->l3_mesh_slave_bank_req_valid_i      = 0;
    dut->l3_mesh_slave_bank_rsp_ready_i      = 1;
}

// ── Drive a bank request (L2 read/write) ──
struct BankReq {
    uint8_t  port_id;
    uint8_t  opcode;
    uint8_t  subopcode;
    uint64_t address;
    uint8_t  size;
    uint8_t  qwen;
    uint8_t  wdata;
    uint8_t  id;
    uint8_t  source;
};

static BankReq make_read_req(uint8_t port_id, uint64_t address) {
    BankReq r = {};
    r.port_id   = port_id;
    r.opcode    = OP_RD;
    r.address   = address;
    r.size      = SIZE_LINE;
    r.qwen      = 0xF;
    return r;
}

static BankReq make_write_req(uint8_t port_id, uint64_t address,
                               uint8_t wdata = 1, uint8_t qwen = 0xF) {
    BankReq r = {};
    r.port_id   = port_id;
    r.opcode    = OP_WR;
    r.address   = address;
    r.size      = SIZE_LINE;
    r.qwen      = qwen;
    r.wdata     = wdata;
    return r;
}

static void drive_req(DUT* dut, const BankReq& r) {
    dut->bank_req_valid_i   = 1;
    dut->bri_port_id_i      = r.port_id;
    dut->bri_trans_id_i     = 0;
    dut->bri_opcode_i       = r.opcode;
    dut->bri_subopcode_i    = r.subopcode;
    dut->bri_address_i      = r.address;
    dut->bri_size_i         = r.size;
    dut->bri_qwen_i         = r.qwen;
    dut->bri_wdata_i        = r.wdata;
    dut->bri_id_i           = r.id;
    dut->bri_source_i       = r.source;
}

// Build an L2-aligned address
// L2 address layout: [tag | set | sub_bank | bank | line_offset]
//   line_offset = 6 bits (64B line)
//   bank = 2 bits
//   sub_bank = 2 bits
//   set = 9 bits
//   tag = remaining
static uint64_t make_l2_addr(uint16_t set, uint8_t sub_bank, uint8_t bank,
                              uint32_t tag) {
    uint64_t addr = 0;
    addr |= ((uint64_t)bank & 0x3) << 6;
    addr |= ((uint64_t)sub_bank & 0x3) << 8;
    addr |= ((uint64_t)set & 0x1FF) << 10;
    addr |= ((uint64_t)tag & 0xFFFFF) << 19;
    return addr;
}

// ── Compare key outputs each cycle ──
// NOTE: We only compare signals that should match between old and new.
// ESR-derived internal state may differ because the original uses its
// internal ESR block with defaults, while the new uses our driven esr_ctl.
static void compare_primary(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("bank_req_ready",
                (uint32_t)d->orig_bank_req_ready_o,
                (uint32_t)d->new_bank_req_ready_o);

    sim.compare("bank_rsp_valid",
                (uint32_t)d->orig_bank_rsp_valid_o,
                (uint32_t)d->new_bank_rsp_valid_o);

    if (d->orig_bank_rsp_valid_o || d->new_bank_rsp_valid_o) {
        bool match = (memcmp(d->orig_bank_rsp_info_o.data(),
                             d->new_bank_rsp_info_o.data(),
                             sizeof(d->orig_bank_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] bank_rsp_info: differs (words:",
                   (unsigned long)sim.sim_time);
            int nw = sizeof(d->orig_bank_rsp_info_o) / 4;
            for (int i = 0; i < nw; i++) {
                if (d->orig_bank_rsp_info_o.data()[i] !=
                    d->new_bank_rsp_info_o.data()[i])
                    printf(" [%d] o=0x%08x n=0x%08x", i,
                           d->orig_bank_rsp_info_o.data()[i],
                           d->new_bank_rsp_info_o.data()[i]);
            }
            printf(")\n");
        } else {
            sim.comparisons++;
        }
    }

    sim.compare("to_l3_req_valid",
                (uint32_t)d->orig_to_l3_req_valid_o,
                (uint32_t)d->new_to_l3_req_valid_o);

    sim.compare("to_l3_clk_en",
                (uint32_t)d->orig_to_l3_clk_en_o,
                (uint32_t)d->new_to_l3_clk_en_o);

    sim.compare("to_sys_req_valid",
                (uint32_t)d->orig_to_sys_req_valid_o,
                (uint32_t)d->new_to_sys_req_valid_o);

    sim.compare("to_sys_clk_en",
                (uint32_t)d->orig_to_sys_clk_en_o,
                (uint32_t)d->new_to_sys_clk_en_o);

    sim.compare("l3s_req_ready",
                (uint32_t)d->orig_l3s_req_ready_o,
                (uint32_t)d->new_l3s_req_ready_o);

    sim.compare("err_detected",
                (uint32_t)d->orig_err_detected_o,
                (uint32_t)d->new_err_detected_o);

    sim.compare("err_logged",
                (uint32_t)d->orig_err_logged_o,
                (uint32_t)d->new_err_logged_o);
}

// ── Wait for idx_cop_sm post-reset ALL_INV to complete in both modules ──
// After cold reset, the idx_cop_sm does a full invalidation sweep of all
// sets and sub-banks.  This takes ~4000+ cycles.  During this time we
// don't compare outputs since timing may legitimately differ.
static bool wait_for_quiescence(CosimCtrl<DUT>& sim, int max_cycles = 16000) {
    auto* dut = sim.dut.get();
    bool settled = false;
    int settle_count = 0;

    for (int i = 0; i < max_cycles; i++) {
        clear_inputs(dut);
        sim.tick();

        // Both modules should eventually stop driving req_valid to mesh
        bool new_quiet = !dut->new_to_l3_req_valid_o &&
                         !dut->new_to_sys_req_valid_o &&
                         dut->new_bank_req_ready_o;
        bool orig_quiet = !dut->orig_to_l3_req_valid_o &&
                          !dut->orig_to_sys_req_valid_o &&
                          dut->orig_bank_req_ready_o;

        if (new_quiet && orig_quiet) {
            settle_count++;
            if (settle_count >= 20) {
                printf("  Post-reset quiescence after %d cycles\n", i + 1);
                settled = true;
                break;
            }
        } else {
            settle_count = 0;
        }
    }
    return settled;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();
    sim.max_time = 800000;

    // ── Setup ESR defaults ──
    // Match the original ESR block reset defaults.
    // ram_delay defaults to 2 in the original.
    dut->esr_sc_ram_delay_i    = 2;
    // Set mask/tag mask default to all-ones (full 9-bit set space)
    dut->esr_sc_l2_set_base_i  = 0;
    dut->esr_sc_l2_set_size_i  = 0;
    dut->esr_sc_l2_set_mask_i  = 0x1FF;
    dut->esr_sc_l2_tag_mask_i  = 0x1FF;
    dut->esr_sc_l3_set_base_i  = 0;
    dut->esr_sc_l3_set_size_i  = 0;
    dut->esr_sc_l3_set_mask_i  = 0x1FF;
    dut->esr_sc_l3_tag_mask_i  = 0x1FF;
    dut->my_bank_id_i          = 0;
    dut->esr_shire_id_i        = 0;

    clear_inputs(dut);

    // ════════════════════════════════════════════════════════
    // Phase 1: Reset
    // ════════════════════════════════════════════════════════
    printf("Phase 1: Reset\n");
    sim.reset(5);

    // ════════════════════════════════════════════════════════
    // Phase 2: Wait for idx_cop_sm ALL_INV to complete
    // ════════════════════════════════════════════════════════
    printf("Phase 2: Wait for post-reset quiescence\n");
    if (!wait_for_quiescence(sim)) {
        printf("  ERROR: modules did not reach quiescence within timeout\n");
        // Continue anyway — some comparisons may still work
    }

    // ════════════════════════════════════════════════════════
    // Phase 3: Compare idle behavior
    // ════════════════════════════════════════════════════════
    printf("Phase 3: Idle comparison (%d cycles)\n", 100);
    for (int i = 0; i < 100; i++) {
        clear_inputs(dut);
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 4: L2 read request
    // ════════════════════════════════════════════════════════
    printf("Phase 4: L2 read request\n");
    {
        uint64_t addr = make_l2_addr(/*set=*/1, /*sub_bank=*/0, /*bank=*/0,
                                      /*tag=*/0x100);
        BankReq req = make_read_req(/*port_id=*/0, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);

        // Clear request and let the pipeline process
        clear_inputs(dut);
        for (int i = 0; i < 40; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 5: Multiple L2 read requests to different sets
    // ════════════════════════════════════════════════════════
    printf("Phase 5: Multiple L2 read requests\n");
    for (int r = 0; r < 4; r++) {
        uint64_t addr = make_l2_addr(/*set=*/10 + r, /*sub_bank=*/r & 3,
                                      /*bank=*/0, /*tag=*/0x200 + r);
        BankReq req = make_read_req(/*port_id=*/r & 3, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
    }

    // Let the pipeline drain
    for (int i = 0; i < 50; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 6: Back-to-back requests (pipeline stress)
    // ════════════════════════════════════════════════════════
    printf("Phase 6: Back-to-back requests\n");
    for (int r = 0; r < 8; r++) {
        uint64_t addr = make_l2_addr(/*set=*/50 + r, /*sub_bank=*/r & 3,
                                      /*bank=*/0, /*tag=*/0x300 + r);
        BankReq req = make_read_req(/*port_id=*/0, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
    }

    // Drain
    clear_inputs(dut);
    for (int i = 0; i < 100; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 7: Re-read same addresses (should hit in cache)
    // ════════════════════════════════════════════════════════
    printf("Phase 7: Cache-hit reads\n");
    for (int r = 0; r < 4; r++) {
        uint64_t addr = make_l2_addr(/*set=*/50 + r, /*sub_bank=*/r & 3,
                                      /*bank=*/0, /*tag=*/0x300 + r);
        BankReq req = make_read_req(/*port_id=*/0, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        for (int i = 0; i < 20; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 8: Reads to different sub-banks (verify sub-bank wiring)
    // ════════════════════════════════════════════════════════
    printf("Phase 8: Reads to all sub-banks\n");
    for (int sb = 0; sb < 4; sb++) {
        uint64_t addr = make_l2_addr(/*set=*/100, /*sub_bank=*/sb, /*bank=*/0,
                                      /*tag=*/0x400 + sb);
        BankReq req = make_read_req(/*port_id=*/sb & 3, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        // Let each request complete
        for (int i = 0; i < 15; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 9: Reads from all 4 ports
    // ════════════════════════════════════════════════════════
    printf("Phase 9: Reads from all ports\n");
    for (int p = 0; p < 4; p++) {
        uint64_t addr = make_l2_addr(/*set=*/200 + p, /*sub_bank=*/0,
                                      /*bank=*/0, /*tag=*/0x500 + p);
        BankReq req = make_read_req(/*port_id=*/p, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
    }
    // Drain
    for (int i = 0; i < 60; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 10: Idle before write phases
    // ════════════════════════════════════════════════════════
    printf("Phase 10: Idle before write phases (%d cycles)\n", 50);
    clear_inputs(dut);
    for (int i = 0; i < 50; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 11: L2 Write requests
    // ════════════════════════════════════════════════════════
    printf("Phase 11: L2 write requests\n");
    for (int w = 0; w < 8; w++) {
        uint64_t addr = make_l2_addr(/*set=*/300 + w, /*sub_bank=*/w & 3,
                                      /*bank=*/0, /*tag=*/0x600 + w);
        BankReq req = make_write_req(/*port_id=*/w & 3, addr,
                                      /*wdata=*/1, /*qwen=*/0xF);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        // Drain pipeline between writes
        for (int i = 0; i < 30; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 12: Mixed read/write workload
    // ════════════════════════════════════════════════════════
    printf("Phase 12: Mixed read/write workload\n");
    for (int m = 0; m < 16; m++) {
        uint64_t addr = make_l2_addr(/*set=*/350 + m, /*sub_bank=*/m & 3,
                                      /*bank=*/0, /*tag=*/0x700 + m);
        BankReq req;
        if (m & 1) {
            req = make_write_req(/*port_id=*/m & 3, addr);
        } else {
            req = make_read_req(/*port_id=*/m & 3, addr);
        }
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        // Drain every 4 requests
        if ((m & 3) == 3) {
            for (int i = 0; i < 40; i++) {
                sim.tick();
                compare_primary(sim);
            }
        }
    }
    // Final drain for phase 12
    clear_inputs(dut);
    for (int i = 0; i < 60; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 13: Backpressure testing
    // ════════════════════════════════════════════════════════
    printf("Phase 13: Backpressure testing\n");
    {
        // Send a request
        uint64_t addr = make_l2_addr(/*set=*/400, /*sub_bank=*/0, /*bank=*/0,
                                      /*tag=*/0x800);
        BankReq req = make_read_req(/*port_id=*/0, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);

        // Deassert rsp_ready for 20 cycles (backpressure)
        dut->bank_rsp_ready_i = 0x0;
        for (int i = 0; i < 20; i++) {
            sim.tick();
            compare_primary(sim);
        }

        // Re-assert ready and let response through
        dut->bank_rsp_ready_i = 0xF;
        for (int i = 0; i < 40; i++) {
            sim.tick();
            compare_primary(sim);
        }

        // Second test: backpressure during write
        addr = make_l2_addr(/*set=*/401, /*sub_bank=*/1, /*bank=*/0,
                             /*tag=*/0x801);
        req = make_write_req(/*port_id=*/1, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);

        dut->bank_rsp_ready_i = 0x0;
        for (int i = 0; i < 15; i++) {
            sim.tick();
            compare_primary(sim);
        }
        dut->bank_rsp_ready_i = 0xF;
        for (int i = 0; i < 40; i++) {
            sim.tick();
            compare_primary(sim);
        }

        // Third test: partial ready (only some ports)
        addr = make_l2_addr(/*set=*/402, /*sub_bank=*/2, /*bank=*/0,
                             /*tag=*/0x802);
        req = make_read_req(/*port_id=*/2, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);

        // Only port 0 ready, port 2 blocked
        dut->bank_rsp_ready_i = 0x1;
        for (int i = 0; i < 15; i++) {
            sim.tick();
            compare_primary(sim);
        }
        dut->bank_rsp_ready_i = 0xF;
        for (int i = 0; i < 40; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 14: Rapid-fire requests (fill request queue)
    // ════════════════════════════════════════════════════════
    printf("Phase 14: Rapid-fire requests (16 back-to-back)\n");
    for (int r = 0; r < 16; r++) {
        uint64_t addr = make_l2_addr(/*set=*/420 + r, /*sub_bank=*/r & 3,
                                      /*bank=*/0, /*tag=*/0x900 + r);
        BankReq req = make_read_req(/*port_id=*/r & 3, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
    }
    // Drain all 16 requests
    clear_inputs(dut);
    for (int i = 0; i < 200; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // Second burst: 16 writes back-to-back
    printf("Phase 14b: Rapid-fire writes (16 back-to-back)\n");
    for (int r = 0; r < 16; r++) {
        uint64_t addr = make_l2_addr(/*set=*/440 + r, /*sub_bank=*/r & 3,
                                      /*bank=*/0, /*tag=*/0xA00 + r);
        BankReq req = make_write_req(/*port_id=*/r & 3, addr);
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
    }
    clear_inputs(dut);
    for (int i = 0; i < 200; i++) {
        sim.tick();
        compare_primary(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 15: Write then read same address (read-after-write)
    // ════════════════════════════════════════════════════════
    printf("Phase 15: Read-after-write (same address)\n");
    for (int t = 0; t < 4; t++) {
        uint64_t addr = make_l2_addr(/*set=*/460 + t, /*sub_bank=*/t & 3,
                                      /*bank=*/0, /*tag=*/0xB00 + t);

        // Write
        BankReq wreq = make_write_req(/*port_id=*/t & 3, addr);
        drive_req(dut, wreq);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        // Let write complete
        for (int i = 0; i < 30; i++) {
            sim.tick();
            compare_primary(sim);
        }

        // Read same address
        BankReq rreq = make_read_req(/*port_id=*/t & 3, addr);
        drive_req(dut, rreq);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        // Let read complete
        for (int i = 0; i < 30; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 16: Different ID/source combinations
    // ════════════════════════════════════════════════════════
    printf("Phase 16: ID/source variation\n");
    for (int n = 0; n < 8; n++) {
        uint64_t addr = make_l2_addr(/*set=*/470 + n, /*sub_bank=*/n & 3,
                                      /*bank=*/0, /*tag=*/0xC00 + n);
        BankReq req = make_read_req(/*port_id=*/n & 3, addr);
        req.id     = n & 0xF;
        req.source = (n * 3) & 0xF;
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        for (int i = 0; i < 25; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }
    // Also test writes with varied id/source
    for (int n = 0; n < 8; n++) {
        uint64_t addr = make_l2_addr(/*set=*/480 + n, /*sub_bank=*/n & 3,
                                      /*bank=*/0, /*tag=*/0xD00 + n);
        BankReq req = make_write_req(/*port_id=*/n & 3, addr);
        req.id     = (n + 5) & 0xF;
        req.source = (n * 7) & 0xF;
        drive_req(dut, req);
        sim.tick();
        compare_primary(sim);
        clear_inputs(dut);
        for (int i = 0; i < 25; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 17: Extended random stimulus
    // ════════════════════════════════════════════════════════
    printf("Phase 17: Extended random stimulus (500 cycles)\n");
    {
        // Deterministic seed for reproducibility
        unsigned int seed = 0xDEAD0042;
        auto rng = [&seed]() -> uint32_t {
            seed = seed * 1103515245 + 12345;
            return (seed >> 16) & 0x7FFF;
        };

        for (int cyc = 0; cyc < 500; cyc++) {
            uint32_t r = rng();

            // 40% chance of driving a new request
            if ((r % 100) < 40) {
                uint16_t set     = rng() & 0x1FF;
                uint8_t  sub_bank = rng() & 0x3;
                uint32_t tag     = rng() & 0xFFFFF;
                uint8_t  port_id = rng() & 0x3;
                uint64_t addr = make_l2_addr(set, sub_bank, /*bank=*/0, tag);

                BankReq req;
                if (rng() & 1) {
                    req = make_read_req(port_id, addr);
                } else {
                    req = make_write_req(port_id, addr);
                }
                req.id     = rng() & 0xF;
                req.source = rng() & 0xF;
                drive_req(dut, req);
            } else {
                clear_inputs(dut);
            }

            // Random backpressure: 15% chance of deasserting rsp_ready
            if ((rng() % 100) < 15) {
                dut->bank_rsp_ready_i = rng() & 0xF;
            } else {
                dut->bank_rsp_ready_i = 0xF;
            }

            sim.tick();
            compare_primary(sim);
        }

        // Drain with full ready
        clear_inputs(dut);
        dut->bank_rsp_ready_i = 0xF;
        for (int i = 0; i < 200; i++) {
            sim.tick();
            compare_primary(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // Phase 18: Final idle (verify no spurious activity)
    // ════════════════════════════════════════════════════════
    printf("Phase 18: Final idle (%d cycles)\n", 100);
    clear_inputs(dut);
    dut->bank_rsp_ready_i = 0xF;
    for (int i = 0; i < 100; i++) {
        sim.tick();
        compare_primary(sim);
    }

    printf("\nTotal simulation cycles: %lu\n", (unsigned long)(sim.sim_time / 2));
    return sim.finish();
}

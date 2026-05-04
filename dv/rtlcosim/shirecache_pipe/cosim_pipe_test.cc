// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_pipe vs shirecache_pipe
//
// Both modules are self-contained with internal RAMs and sub-modules
// (sub_bank, sub_bank_mem x4, cbuf, rbuf, idx_cop_sm).
// This C++ driver sends cache-line operations through the pipeline
// and compares key outputs cycle-by-cycle.

#include "Vcosim_pipe_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "verilated.h"

using DUT = Vcosim_pipe_tb;

// ── Address-space encoding (address_space_e) ──
static constexpr uint8_t ADDR_L2  = 0;
static constexpr uint8_t ADDR_L3  = 1;
static constexpr uint8_t ADDR_SCP = 2;
static constexpr uint8_t ADDR_IDX = 3;

// ── Reqq opcodes (reqq_opcode_e) ──
static constexpr uint8_t OP_L2_READ           = 0x00;
static constexpr uint8_t OP_L2_WRITE          = 0x01;
static constexpr uint8_t OP_L2_WRITE_PARTIAL  = 0x02;
static constexpr uint8_t OP_L2_FILL           = 0x0d;
static constexpr uint8_t OP_L2_SCRUB          = 0x0e;
static constexpr uint8_t OP_L3_READ           = 0x10;
static constexpr uint8_t OP_L3_FILL           = 0x1b;
static constexpr uint8_t OP_SCP_READ          = 0x1d;
static constexpr uint8_t OP_SCP_WRITE         = 0x1e;
static constexpr uint8_t OP_SCP_FILL          = 0x21;
static constexpr uint8_t OP_IDX_ALL_INV       = 0x28;
static constexpr uint8_t OP_IDX_L2_INV        = 0x29;

// ── Size encoding (etlink size_e) ──
static constexpr uint8_t SIZE_DW   = 3;  // 64b
static constexpr uint8_t SIZE_LINE = 6;  // 512b

// Constants
static constexpr int SUB_BANKS = 4;
static constexpr int LINE_QW   = 4;

// ── Clear all pipe inputs ──
static void clear_pipe(DUT* dut) {
    dut->pipe_start_i          = 0;
    dut->pr_trans_id_i         = 0;
    dut->pr_reqq_id_i          = 0;
    dut->pr_opcode_i           = 0;
    dut->pr_address_i          = 0;
    dut->pr_address_space_i    = 0;
    dut->pr_zero_data_i        = 0;
    dut->pr_qwen_i             = 0;
    dut->pr_size_i             = 0;
    dut->pr_sub_bank_id_i      = 0;
    dut->dataq_rsp_trans_id_i  = 0;
    dut->dataq_rsp_reqq_id_i   = 0;
    for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0;
    dut->dataq_rsp_ben_i       = 0;
    dut->dataq_rsp_err_i       = 0;
    dut->rbuf_clear_mem_valid_hot_i = 0;
    dut->pipe_rbuf_rd_req_valid_i   = 0;
}

// ── Drive a pipe request ──
struct PipeReq {
    uint8_t  opcode;
    uint64_t address;
    uint8_t  address_space;
    uint8_t  sub_bank_id;
    uint8_t  qwen;
    uint64_t trans_id;
    uint8_t  reqq_id;
    uint8_t  size;
    bool     zero_data;
};

static PipeReq make_req(uint8_t opcode, uint8_t addr_space,
                        uint64_t address, uint8_t sub_bank_id) {
    PipeReq r = {};
    r.opcode        = opcode;
    r.address       = address;
    r.address_space = addr_space;
    r.sub_bank_id   = sub_bank_id;
    r.qwen          = 0xF;  // all QWs
    r.trans_id      = 0;
    r.reqq_id       = 0;
    r.size          = SIZE_LINE;
    return r;
}

static void drive_req(DUT* dut, const PipeReq& r) {
    dut->pipe_start_i        = 1;
    dut->pr_trans_id_i       = r.trans_id;
    dut->pr_reqq_id_i        = r.reqq_id;
    dut->pr_opcode_i         = r.opcode;
    dut->pr_address_i        = r.address;
    dut->pr_address_space_i  = r.address_space;
    dut->pr_zero_data_i      = r.zero_data;
    dut->pr_qwen_i           = r.qwen;
    dut->pr_size_i           = r.size;
    dut->pr_sub_bank_id_i    = r.sub_bank_id;
}

// Build an L2-aligned address from set/bank/sub_bank
// L2 address layout: [tag | set | sub_bank | bank | line_offset]
//   line_offset = 6 bits (64B line)
//   bank = 2 bits
//   sub_bank = 2 bits
//   set = 9 bits (SetIdSize)
//   tag = remaining
static uint64_t make_l2_addr(uint16_t set, uint8_t sub_bank, uint8_t bank, uint32_t tag) {
    uint64_t addr = 0;
    addr |= ((uint64_t)bank & 0x3) << 6;
    addr |= ((uint64_t)sub_bank & 0x3) << 8;
    addr |= ((uint64_t)set & 0x1FF) << 10;
    addr |= ((uint64_t)tag & 0xFFFFF) << 19;
    return addr;
}

// Build an SCP address
// SCP shares the same sub_bank/bank layout as L2 for the lower bits
// but the way is encoded in the sub_bank field (ScpWayLsb = SubBankLsb = 8)
static uint64_t make_scp_addr(uint16_t set, uint8_t sub_bank, uint8_t bank) {
    uint64_t addr = 0;
    addr |= ((uint64_t)bank & 0x3) << 6;
    addr |= ((uint64_t)sub_bank & 0x3) << 8;
    addr |= ((uint64_t)set & 0x1FF) << 10;
    // SCP region check: addr[ScpRegionLsb +: ScpRegionIdSize] must match ScpRegionId
    addr |= ((uint64_t)0x001) << 31;  // ScpRegionId = 9'h001 at bit 31
    return addr;
}

// ── Compare all key outputs ──
static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("pipe_active",
                (uint32_t)d->orig_pipe_active_o,
                (uint32_t)d->new_pipe_active_o);

    sim.compare("pipe_busy_sub_bank",
                (uint32_t)d->orig_pipe_busy_sub_bank_o,
                (uint32_t)d->new_pipe_busy_sub_bank_o);

    sim.compare("pipe_squash",
                (uint32_t)d->orig_pipe_squash_o,
                (uint32_t)d->new_pipe_squash_o);

    sim.compare("tag_rsp_valid",
                (uint32_t)d->orig_tag_rsp_valid_o,
                (uint32_t)d->new_tag_rsp_valid_o);
    if (d->orig_tag_rsp_valid_o || d->new_tag_rsp_valid_o) {
        bool match = (memcmp(d->orig_tag_rsp_info_o.data(),
                             d->new_tag_rsp_info_o.data(),
                             sizeof(d->orig_tag_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] tag_rsp_info: differs (words:", (unsigned long)sim.sim_time);
            int nw = sizeof(d->orig_tag_rsp_info_o) / 4;
            for (int i = 0; i < nw; i++) {
                if (d->orig_tag_rsp_info_o.data()[i] != d->new_tag_rsp_info_o.data()[i])
                    printf(" [%d] o=0x%08x n=0x%08x", i, d->orig_tag_rsp_info_o.data()[i], d->new_tag_rsp_info_o.data()[i]);
            }
            printf(")\n");
        } else {
            sim.comparisons++;
        }
    }

    sim.compare("data_rsp_pre_valid",
                (uint32_t)d->orig_data_rsp_pre_valid_o,
                (uint32_t)d->new_data_rsp_pre_valid_o);
    if (d->orig_data_rsp_pre_valid_o || d->new_data_rsp_pre_valid_o) {
        sim.compare("data_rsp_pre_info",
                    d->orig_data_rsp_pre_info_o,
                    d->new_data_rsp_pre_info_o);
    }

    sim.compare("data_rsp_valid",
                (uint32_t)d->orig_data_rsp_valid_o,
                (uint32_t)d->new_data_rsp_valid_o);
    if (d->orig_data_rsp_valid_o || d->new_data_rsp_valid_o) {
        bool match = (memcmp(d->orig_data_rsp_info_o.data(),
                             d->new_data_rsp_info_o.data(),
                             sizeof(d->orig_data_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] data_rsp_info: differs (words:", (unsigned long)sim.sim_time);
            int nw = sizeof(d->orig_data_rsp_info_o) / 4;
            for (int i = 0; i < nw; i++) {
                if (d->orig_data_rsp_info_o.data()[i] != d->new_data_rsp_info_o.data()[i])
                    printf(" [%d] o=0x%08x n=0x%08x", i, d->orig_data_rsp_info_o.data()[i], d->new_data_rsp_info_o.data()[i]);
            }
            printf(")\n");
        } else {
            sim.comparisons++;
        }
    }

    sim.compare("cbuf_rsp_valid",
                (uint32_t)d->orig_cbuf_rsp_valid_o,
                (uint32_t)d->new_cbuf_rsp_valid_o);
    if (d->orig_cbuf_rsp_valid_o || d->new_cbuf_rsp_valid_o) {
        bool match = (memcmp(d->orig_cbuf_rsp_info_o.data(),
                             d->new_cbuf_rsp_info_o.data(),
                             sizeof(d->orig_cbuf_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] cbuf_rsp_info: differs\n",
                   (unsigned long)sim.sim_time);
        } else {
            sim.comparisons++;
        }
    }

    sim.compare("idx_cop_aborted",
                (uint32_t)d->orig_idx_cop_aborted_o,
                (uint32_t)d->new_idx_cop_aborted_o);
    sim.compare("idx_cop_l2_busy",
                (uint32_t)d->orig_idx_cop_l2_busy_o,
                (uint32_t)d->new_idx_cop_l2_busy_o);
    sim.compare("idx_cop_l3_busy",
                (uint32_t)d->orig_idx_cop_l3_busy_o,
                (uint32_t)d->new_idx_cop_l3_busy_o);
    sim.compare("idx_cop_req_valid",
                (uint32_t)d->orig_idx_cop_req_valid_o,
                (uint32_t)d->new_idx_cop_req_valid_o);
    if (d->orig_idx_cop_req_valid_o || d->new_idx_cop_req_valid_o) {
        bool match = (memcmp(d->orig_idx_cop_req_info_o.data(),
                             d->new_idx_cop_req_info_o.data(),
                             sizeof(d->orig_idx_cop_req_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] idx_cop_req_info: differs\n",
                   (unsigned long)sim.sim_time);
        } else {
            sim.comparisons++;
        }
    }
    sim.compare("idx_cop_is_active",
                (uint32_t)d->orig_idx_cop_is_active_o,
                (uint32_t)d->new_idx_cop_is_active_o);

    sim.compare("dataq_rd_req_valid",
                (uint32_t)d->orig_dataq_rd_req_valid_o,
                (uint32_t)d->new_dataq_rd_req_valid_o);

    sim.compare("rbuf_rd_req_ready",
                (uint32_t)d->orig_rbuf_rd_req_ready_o,
                (uint32_t)d->new_rbuf_rd_req_ready_o);
}

// ── Wait for idx_cop_sm post-reset ALL_INV to complete ──
// After reset, idx_cop_sm does automatic ALL_INV (~256 cycle counter +
// 512 sets x 4 sub-banks). Wait for idx_cop_is_active to deassert AND
// for pipe_busy/pipe_active to drain on both modules.
static void wait_for_idx_cop_idle(CosimCtrl<DUT>& sim, int max_cycles = 16000) {
    auto* dut = sim.dut.get();
    bool idx_reported = false;
    for (int i = 0; i < max_cycles; i++) {
        clear_pipe(dut);
        sim.tick();
        // Don't compare during ALL_INV — pipeline timing may legitimately differ
        // during the massive invalidation sweep. Just wait for quiescence.
        if (!idx_reported &&
            !dut->new_idx_cop_is_active_o && !dut->orig_idx_cop_is_active_o) {
            printf("  idx_cop_sm idle after %d cycles\n", i + 1);
            idx_reported = true;
        }
        if (idx_reported &&
            !dut->new_pipe_active_o && !dut->orig_pipe_active_o &&
            dut->new_pipe_busy_sub_bank_o == 0 &&
            dut->orig_pipe_busy_sub_bank_o == 0) {
            printf("  pipeline fully drained after %d cycles\n", i + 1);
            return;
        }
    }
    printf("  WARNING: pipeline still active after %d cycles\n", max_cycles);
}

// ── Idle drain — let pipeline flush ──
static void drain_pipe(CosimCtrl<DUT>& sim, int cycles = 30) {
    auto* dut = sim.dut.get();
    for (int i = 0; i < cycles; i++) {
        clear_pipe(dut);
        sim.tick();
        compare_all(sim);
    }
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000000;  // 200M — enough for post-reset ALL_INV + tests
    auto* dut = sim.dut.get();

    // ── Default ESR config ──
    dut->esr_sc_ram_delay_i                          = 2;
    dut->esr_sc_l2_set_base_i                        = 0;
    dut->esr_sc_l2_set_size_i                        = 256;
    dut->esr_sc_l2_set_mask_i                        = 0x1FF;
    dut->esr_sc_l2_tag_mask_i                        = 0x1FF;
    dut->esr_sc_l3_set_base_i                        = 0;
    dut->esr_sc_l3_set_size_i                        = 128;
    dut->esr_sc_l3_set_mask_i                        = 0x1FF;
    dut->esr_sc_l3_tag_mask_i                        = 0x1FF;
    dut->esr_sc_scp_set_base_i                       = 0;
    dut->esr_sc_scp_set_size_i                       = 64;
    dut->esr_sc_scp_set_mask_i                       = 0x1FF;
    dut->esr_sc_scp_tag_mask_i                       = 0x1FF;
    dut->esr_sc_l2_rbuf_enable_i                     = 0;
    dut->esr_sc_scp_rbuf_enable_i                    = 0;
    dut->esr_sc_zero_state_enable_i                  = 1;
    dut->esr_sc_all_shire_aliasing_i                 = 0;
    dut->esr_sc_two_shire_aliasing_use_shire_lsb_i   = 0;
    dut->esr_sc_cbuf_enable_i                        = 0;
    dut->esr_sc_cbuf_entry_enable_i                  = 0;
    dut->esr_sc_allow_only_1_req_per_sub_bank_i      = 0;
    dut->esr_sc_allow_only_1_req_per_bank_i          = 0;
    dut->my_bank_id_i                                = 1;
    dut->my_l3_virtual_shire_id_i                    = 0;
    dut->my_scp_virtual_shire_id_i                   = 0;
    dut->pipe_idx_cop_req_ready_i                    = 1;  // always accept idx_cop requests
    dut->pipe_rbuf_rd_rsp_ready_i                    = 1;
    clear_pipe(dut);

    sim.reset();

    // ════════════════════════════════════════════════════════
    // Phase 0: Wait for post-reset ALL_INV to complete
    // ════════════════════════════════════════════════════════
    printf("cosim phase 0: waiting for post-reset ALL_INV\n");
    wait_for_idx_cop_idle(sim);
    drain_pipe(sim, 10);
    printf("  phase 0 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 1: Single L2 read miss
    // ════════════════════════════════════════════════════════
    printf("cosim phase 1: L2 read (miss into empty tags)\n");
    {
        PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 1 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 2: L2 fill + L2 read hit
    // ════════════════════════════════════════════════════════
    printf("cosim phase 2: L2 fill then L2 read hit\n");
    {
        // Fill at set 0, sub_bank 0, tag 0x100
        PipeReq r = make_req(OP_L2_FILL, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id  = 1;
        r.trans_id = 0x10;
        // Provide fill data through dataq rsp
        dut->dataq_rsp_trans_id_i = 0x10;
        dut->dataq_rsp_reqq_id_i  = 1;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0xDEAD0000 + i;
        dut->dataq_rsp_ben_i      = 0xFFFFFFFFFFFFFFFFULL;
        dut->dataq_rsp_err_i      = 0;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        // Now read — should hit
        r = make_req(OP_L2_READ, ADDR_L2,
                     make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id  = 2;
        r.trans_id = 0x20;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 2 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 3: L2 write
    // ════════════════════════════════════════════════════════
    printf("cosim phase 3: L2 write\n");
    {
        PipeReq r = make_req(OP_L2_WRITE, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id  = 3;
        r.trans_id = 0x30;
        dut->dataq_rsp_trans_id_i = 0x30;
        dut->dataq_rsp_reqq_id_i  = 3;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0xBEEF0000 + i;
        dut->dataq_rsp_ben_i      = 0xFFFFFFFFFFFFFFFFULL;
        dut->dataq_rsp_err_i      = 0;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 3 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 4: L3 read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 4: L3 read\n");
    {
        // With zero swizzle config, all swizzle selectors pick rel_addr[0]
        // = address[6]. To get sub_bank_id=0, we need address[6]=0.
        // bank field is bits [7:6] so bank=0 ensures addr[6]=0.
        PipeReq r = make_req(OP_L3_READ, ADDR_L3,
                             make_l2_addr(10, 0, 0, 0x200), 0);
        r.reqq_id  = 4;
        r.trans_id = 0x40;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 4 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 5: Multiple back-to-back reads (pipeline stress)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 5: back-to-back reads\n");
    {
        for (int i = 0; i < 4; i++) {
            PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                                 make_l2_addr(i + 1, 0, 1, 0x100 + i), 0);
            r.reqq_id  = 5 + i;
            r.trans_id = 0x50 + i;
            drive_req(dut, r);
            sim.tick(); compare_all(sim);
            clear_pipe(dut);
        }
        drain_pipe(sim, 30);
    }
    printf("  phase 5 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 6: Different sub-banks
    // ════════════════════════════════════════════════════════
    printf("cosim phase 6: different sub-banks\n");
    {
        for (uint8_t sb = 0; sb < 4; sb++) {
            PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                                 make_l2_addr(20, sb, 1, 0x300), sb);
            r.reqq_id  = 10 + sb;
            r.trans_id = 0x60 + sb;
            drive_req(dut, r);
            sim.tick(); compare_all(sim);
            clear_pipe(dut);
        }
        drain_pipe(sim, 30);
    }
    printf("  phase 6 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 7: Squash (SCP read — out of range triggers squash)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 7: SCP read (squash test)\n");
    {
        PipeReq r = make_req(OP_SCP_READ, ADDR_SCP,
                             make_scp_addr(5, 0, 1), 0);
        r.reqq_id  = 20;
        r.trans_id = 0x70;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 7 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 8: Idx operations (AllInv, L2Inv)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 8: idx operations\n");
    {
        // AllInv
        PipeReq r = make_req(OP_IDX_ALL_INV, ADDR_IDX, 0, 0);
        r.reqq_id  = 30;
        r.trans_id = 0x80;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);

        // Wait for idx_cop_sm to process it
        wait_for_idx_cop_idle(sim);
        drain_pipe(sim, 10);

        // L2Inv
        r = make_req(OP_IDX_L2_INV, ADDR_IDX, 0, 0);
        r.reqq_id  = 31;
        r.trans_id = 0x81;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);

        wait_for_idx_cop_idle(sim);
        drain_pipe(sim, 10);
    }
    printf("  phase 8 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 9: ram_delay=3 test
    // ════════════════════════════════════════════════════════
    printf("cosim phase 9: ram_delay=3\n");
    {
        dut->esr_sc_ram_delay_i = 3;
        drain_pipe(sim, 5);

        PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                             make_l2_addr(50, 0, 1, 0x400), 0);
        r.reqq_id  = 40;
        r.trans_id = 0x90;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 30);

        // Restore ram_delay
        dut->esr_sc_ram_delay_i = 2;
    }
    printf("  phase 9 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 10: L3 fill + L3 read hit
    // ════════════════════════════════════════════════════════
    printf("cosim phase 10: L3 fill then L3 read hit\n");
    {
        PipeReq r = make_req(OP_L3_FILL, ADDR_L3,
                             make_l2_addr(10, 0, 0, 0x200), 0);
        r.reqq_id = 50; r.trans_id = 0xA0;
        dut->dataq_rsp_trans_id_i = 0xA0;
        dut->dataq_rsp_reqq_id_i = 50;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0xCA000000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        r = make_req(OP_L3_READ, ADDR_L3,
                     make_l2_addr(10, 0, 0, 0x200), 0);
        r.reqq_id = 51; r.trans_id = 0xA1;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 10 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 11: SCP write + SCP read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 11: SCP write + read\n");
    {
        PipeReq w = make_req(OP_SCP_WRITE, ADDR_SCP,
                             make_scp_addr(5, 0, 1), 0);
        w.reqq_id = 52; w.trans_id = 0xB0;
        dut->dataq_rsp_trans_id_i = 0xB0;
        dut->dataq_rsp_reqq_id_i = 52;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0xFACE0000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, w);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        PipeReq r = make_req(OP_SCP_READ, ADDR_SCP,
                             make_scp_addr(5, 0, 1), 0);
        r.reqq_id = 53; r.trans_id = 0xB1;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 11 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 12: L2 write-partial
    // ════════════════════════════════════════════════════════
    printf("cosim phase 12: L2 write-partial\n");
    {
        PipeReq r = make_req(OP_L2_WRITE_PARTIAL, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id = 54; r.trans_id = 0xC0;
        r.qwen = 0x5;  // QW0 + QW2 only
        r.size = SIZE_DW;
        dut->dataq_rsp_trans_id_i = 0xC0;
        dut->dataq_rsp_reqq_id_i = 54;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x11110000 + i;
        dut->dataq_rsp_ben_i = 0x00FF00FF00FF00FFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 12 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 13: Fill 4 ways in same set (LRU saturation)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 13: fill 4 ways same set\n");
    for (int way = 0; way < 4; way++) {
        PipeReq r = make_req(OP_L2_FILL, ADDR_L2,
                             make_l2_addr(80, 0, 1, 0xD00 + way), 0);
        r.reqq_id = 60 + way; r.trans_id = 0xD0 + way;
        dut->dataq_rsp_trans_id_i = 0xD0 + way;
        dut->dataq_rsp_reqq_id_i = 60 + way;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x22220000 + way * 0x100 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 20);
    }
    // 5th fill → LRU eviction
    {
        PipeReq r = make_req(OP_L2_FILL, ADDR_L2,
                             make_l2_addr(80, 0, 1, 0xD10), 0);
        r.reqq_id = 0; r.trans_id = 0xD8;
        dut->dataq_rsp_trans_id_i = 0xD8;
        dut->dataq_rsp_reqq_id_i = 0;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x33330000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 13 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 14: Back-to-back fills on different sub-banks (pipeline interleave)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 14: interleaved sub-bank fills\n");
    for (int sb = 0; sb < 4; sb++) {
        PipeReq r = make_req(OP_L2_FILL, ADDR_L2,
                             make_l2_addr(90 + sb, sb, 1, 0xE00 + sb), sb);
        r.reqq_id = sb; r.trans_id = 0xE0 + sb;
        dut->dataq_rsp_trans_id_i = 0xE0 + sb;
        dut->dataq_rsp_reqq_id_i = sb;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x44440000 + sb * 0x100 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
    }
    drain_pipe(sim, 30);
    printf("  phase 14 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 15: ram_delay=4
    // ════════════════════════════════════════════════════════
    printf("cosim phase 15: ram_delay=4\n");
    {
        dut->esr_sc_ram_delay_i = 4;
        drain_pipe(sim, 5);

        PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id = 0; r.trans_id = 0xF0;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 35);

        // Fill + read at delay=4
        r = make_req(OP_L2_FILL, ADDR_L2,
                     make_l2_addr(95, 0, 1, 0xF00), 0);
        r.reqq_id = 1; r.trans_id = 0xF1;
        dut->dataq_rsp_trans_id_i = 0xF1;
        dut->dataq_rsp_reqq_id_i = 1;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x55550000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 35);

        r = make_req(OP_L2_READ, ADDR_L2,
                     make_l2_addr(95, 0, 1, 0xF00), 0);
        r.reqq_id = 2; r.trans_id = 0xF2;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 35);

        dut->esr_sc_ram_delay_i = 2;
        drain_pipe(sim, 5);
    }
    printf("  phase 15 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 16: L2 scrub
    // ════════════════════════════════════════════════════════
    printf("cosim phase 16: L2 scrub\n");
    {
        PipeReq r = make_req(OP_L2_SCRUB, ADDR_L2,
                             make_l2_addr(0, 0, 1, 0x100), 0);
        r.reqq_id = 0; r.trans_id = 0x100;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 16 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 17: cbuf_enable on — fill then write (should generate cbuf rsp)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 17: cbuf enabled\n");
    {
        dut->esr_sc_cbuf_enable_i = 1;
        dut->esr_sc_cbuf_entry_enable_i = 0xFFFFFFFF;
        drain_pipe(sim, 5);

        // Fill
        PipeReq fill = make_req(OP_L2_FILL, ADDR_L2,
                                make_l2_addr(100, 0, 1, 0x1100), 0);
        fill.reqq_id = 0; fill.trans_id = 0x110;
        dut->dataq_rsp_trans_id_i = 0x110;
        dut->dataq_rsp_reqq_id_i = 0;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x66660000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        // Write (should interact with cbuf)
        PipeReq wr = make_req(OP_L2_WRITE, ADDR_L2,
                              make_l2_addr(100, 0, 1, 0x1100), 0);
        wr.reqq_id = 1; wr.trans_id = 0x111;
        dut->dataq_rsp_trans_id_i = 0x111;
        dut->dataq_rsp_reqq_id_i = 1;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x77770000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, wr);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        dut->esr_sc_cbuf_enable_i = 0;
        dut->esr_sc_cbuf_entry_enable_i = 0;
    }
    printf("  phase 17 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 18: rbuf_enable on — L2 fill then read (should install in rbuf)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 18: L2 rbuf enabled\n");
    {
        dut->esr_sc_l2_rbuf_enable_i = 1;
        drain_pipe(sim, 5);

        PipeReq fill = make_req(OP_L2_FILL, ADDR_L2,
                                make_l2_addr(110, 0, 1, 0x1200), 0);
        fill.reqq_id = 0; fill.trans_id = 0x120;
        dut->dataq_rsp_trans_id_i = 0x120;
        dut->dataq_rsp_reqq_id_i = 0;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x88880000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        PipeReq rd = make_req(OP_L2_READ, ADDR_L2,
                              make_l2_addr(110, 0, 1, 0x1200), 0);
        rd.reqq_id = 1; rd.trans_id = 0x121;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        dut->esr_sc_l2_rbuf_enable_i = 0;
    }
    printf("  phase 18 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 19: SCP fill + SCP read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 19: SCP fill + read\n");
    {
        PipeReq fill = make_req(OP_SCP_FILL, ADDR_SCP,
                                make_scp_addr(10, 0, 1), 0);
        fill.reqq_id = 0; fill.trans_id = 0x130;
        dut->dataq_rsp_trans_id_i = 0x130;
        dut->dataq_rsp_reqq_id_i = 0;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0x99990000 + i;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);

        PipeReq rd = make_req(OP_SCP_READ, ADDR_SCP,
                              make_scp_addr(10, 0, 1), 0);
        rd.reqq_id = 1; rd.trans_id = 0x131;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
    }
    printf("  phase 19 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 20: Mixed traffic — 16 ops with varying sets, sub-banks, opcodes
    // ════════════════════════════════════════════════════════
    printf("cosim phase 20: mixed traffic\n");
    for (int iter = 0; iter < 16; iter++) {
        int set = (iter * 7) % 128;
        int sb = iter % 4;
        int tag = 0x1400 + iter;
        uint8_t op = (iter % 4 == 0) ? OP_L2_READ :
                     (iter % 4 == 1) ? OP_L2_FILL :
                     (iter % 4 == 2) ? OP_L2_WRITE : OP_L2_READ;
        PipeReq r = make_req(op, ADDR_L2,
                             make_l2_addr(set, sb, 1, tag), sb);
        r.reqq_id = iter & 0x3F;
        r.trans_id = 0x140 + iter;
        if (op == OP_L2_FILL || op == OP_L2_WRITE) {
            dut->dataq_rsp_trans_id_i = 0x140 + iter;
            dut->dataq_rsp_reqq_id_i = iter & 0x3F;
            for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0xAA000000 + iter * 0x10000 + i;
            dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        }
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        sim.tick(); compare_all(sim);
    }
    drain_pipe(sim, 40);
    printf("  phase 20 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 21: ram_delay=3 back-to-back stress
    // ════════════════════════════════════════════════════════
    printf("cosim phase 21: ram_delay=3 back-to-back\n");
    {
        dut->esr_sc_ram_delay_i = 3;
        drain_pipe(sim, 5);
        for (int i = 0; i < 6; i++) {
            PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                                 make_l2_addr(120 + i, 0, 1, 0x1500 + i), 0);
            r.reqq_id = i; r.trans_id = 0x150 + i;
            drive_req(dut, r);
            sim.tick(); compare_all(sim);
            clear_pipe(dut);
        }
        drain_pipe(sim, 35);
        dut->esr_sc_ram_delay_i = 2;
        drain_pipe(sim, 5);
    }
    printf("  phase 21 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 22: Different bank_id
    // ════════════════════════════════════════════════════════
    printf("cosim phase 22: different bank_id\n");
    {
        dut->my_bank_id_i = 3;
        PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                             make_l2_addr(0, 0, 3, 0x100), 0);
        r.reqq_id = 0; r.trans_id = 0x160;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
        dut->my_bank_id_i = 1;
    }
    printf("  phase 22 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 23: zero_state_enable off
    // ════════════════════════════════════════════════════════
    printf("cosim phase 23: zero_state off\n");
    {
        dut->esr_sc_zero_state_enable_i = 0;
        PipeReq r = make_req(OP_L2_FILL, ADDR_L2,
                             make_l2_addr(130, 0, 1, 0x1700), 0);
        r.reqq_id = 0; r.trans_id = 0x170;
        r.zero_data = true;
        dut->dataq_rsp_trans_id_i = 0x170;
        dut->dataq_rsp_reqq_id_i = 0;
        for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0;
        dut->dataq_rsp_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        drain_pipe(sim, 25);
        dut->esr_sc_zero_state_enable_i = 1;
    }
    printf("  phase 23 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 24: allow_only_1_req_per_sub_bank
    // ════════════════════════════════════════════════════════
    printf("cosim phase 24: allow_only_1_req_per_sub_bank\n");
    {
        dut->esr_sc_allow_only_1_req_per_sub_bank_i = 1;
        drain_pipe(sim, 5);
        for (int i = 0; i < 4; i++) {
            PipeReq r = make_req(OP_L2_READ, ADDR_L2,
                                 make_l2_addr(140 + i, 0, 1, 0x1800 + i), 0);
            r.reqq_id = i; r.trans_id = 0x180 + i;
            drive_req(dut, r);
            sim.tick(); compare_all(sim);
            clear_pipe(dut);
        }
        drain_pipe(sim, 30);
        dut->esr_sc_allow_only_1_req_per_sub_bank_i = 0;
    }
    printf("  phase 24 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 25: Final idle drain
    // ════════════════════════════════════════════════════════
    printf("cosim phase 25: final idle drain\n");
    drain_pipe(sim, 50);
    printf("  phase 25 done\n");

    return sim.finish();
}

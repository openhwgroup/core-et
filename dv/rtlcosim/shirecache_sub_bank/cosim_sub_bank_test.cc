// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_pipe_sub_bank vs shirecache_pipe_sub_bank
//
// Both modules share behavioral RAM models in the SV wrapper.
// This C++ driver sends cache-line operations through the pipeline
// and compares key outputs cycle-by-cycle.

#include "Vcosim_sub_bank_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "verilated.h"

using DUT = Vcosim_sub_bank_tb;

// ── Address-space encoding (address_space_e) ──
static constexpr uint8_t ADDR_L2  = 0;
static constexpr uint8_t ADDR_L3  = 1;
static constexpr uint8_t ADDR_SCP = 2;
static constexpr uint8_t ADDR_IDX = 3;
static constexpr uint8_t ADDR_MSG = 4;

// ── Reqq opcodes (reqq_opcode_e) ──
static constexpr uint8_t OP_L2_READ           = 0x00;
static constexpr uint8_t OP_L2_WRITE          = 0x01;
static constexpr uint8_t OP_L2_WRITE_PARTIAL  = 0x02;
static constexpr uint8_t OP_L2_WRITE_PARTIAL2 = 0x03;
static constexpr uint8_t OP_L2_FLUSH          = 0x04;
static constexpr uint8_t OP_L2_FLUSH_WDATA    = 0x05;
static constexpr uint8_t OP_L2_EVICT          = 0x06;
static constexpr uint8_t OP_L2_EVICT_WDATA    = 0x07;
static constexpr uint8_t OP_L2_PREFETCH       = 0x08;
static constexpr uint8_t OP_L2_WRITE_AROUND   = 0x09;
static constexpr uint8_t OP_L2_ATOMIC         = 0x0a;
static constexpr uint8_t OP_L2_ATOMIC2ND      = 0x0c;
static constexpr uint8_t OP_L2_FILL           = 0x0d;
static constexpr uint8_t OP_L2_SCRUB          = 0x0e;
static constexpr uint8_t OP_L2_MSG_SEND_DATA  = 0x0f;
static constexpr uint8_t OP_L3_READ           = 0x10;
static constexpr uint8_t OP_L3_WRITE          = 0x11;
static constexpr uint8_t OP_L3_FILL           = 0x1b;
static constexpr uint8_t OP_SCP_READ          = 0x1d;
static constexpr uint8_t OP_SCP_WRITE         = 0x1e;
static constexpr uint8_t OP_SCP_FILL          = 0x21;
static constexpr uint8_t OP_SCP_ZERO          = 0x23;
static constexpr uint8_t OP_IDX_ALL_INV       = 0x28;
static constexpr uint8_t OP_IDX_L2_INV        = 0x29;
static constexpr uint8_t OP_IDX_WRITE         = 0x27;
static constexpr uint8_t OP_IDX_READ          = 0x26;

// ── Size encoding (etlink size_e) ──
static constexpr uint8_t SIZE_DW   = 3;  // 64b
static constexpr uint8_t SIZE_LINE = 6;  // 512b

// Constants
static constexpr int SUB_BANKS = 4;
static constexpr int LINE_QW   = 4;

// ── Clear all pipe inputs ──
static void clear_pipe(DUT* dut) {
    dut->pipe_valid_tap_i    = 0;
    dut->pi_pipe_set_i       = 0;
    dut->pi_pipe_sub_bank_i  = 0;
    dut->pi_pipe_idx_address_space_i = 0;
    dut->pi_pipe_dec_err_i   = 0;
    dut->pi_pipe_dec_err_type_i = 0;
    dut->pi_pipe_scp_read_i  = 0;
    dut->pi_pipe_bubble_i    = 0;
    dut->pi_idx_write_i      = 0;
    dut->pi_tag_hit_i        = 0;
    dut->pi_tag_ram_hit_way_i = 0;
    dut->pi_tag_sbe_i        = 0;
    dut->pi_victim_i         = 0;
    dut->pi_data_rd_req_i    = 0;
    dut->pi_data_wr_req_i    = 0;
    dut->pi_data_wr_zero_i   = 0;
    dut->pi_data_wr_qwen_i   = 0;
    dut->pi_dataq_rd_req_en_i = 0;
    dut->pi_dataq_rd_req_partial_i = 0;
    dut->pi_dataq_sel_qwen_i = 0;
    dut->pi_merge_wr_data_in_rd_rsp_i = 0;
    dut->pi_merge_rd_data_in_wr_req_i = 0;
    dut->pi_data_rsp_req_i   = 0;
    dut->pi_data_rsp_zero_i  = 0;
    dut->pi_data_rsp_qwen_i  = 0;
    dut->pr_trans_id_i       = 0;
    dut->pr_reqq_id_i        = 0;
    dut->pr_opcode_i         = 0;
    dut->pr_address_i        = 0;
    dut->pr_address_space_i  = 0;
    dut->pr_zero_data_i      = 0;
    dut->pr_qwen_i           = 0;
    dut->pr_size_i           = 0;
    dut->pr_sub_bank_id_i    = 0;
    dut->dataq_rsp_trans_id_i = 0;
    dut->dataq_rsp_reqq_id_i  = 0;
    for (int i = 0; i < 16; i++) dut->dataq_rsp_data_i[i] = 0;
    dut->dataq_rsp_ben_i      = 0;
    dut->dataq_rsp_err_i      = 0;
    dut->esr_wr_en_sbe_dbe_counts_i = 0;
    dut->esr_wr_data_i       = 0;
}

// ── Drive a pipe request ──
// The pipe_info_tap fields that matter at _tap stage are:
//   pipe_set, pipe_sub_bank, pipe_idx_address_space, pipe_dec_err, pipe_scp_read
// The rest of pipe_info_tap fields are set from _tc stage logic — the sub_bank
// module itself sets them internally.  For cosim, we only drive the _tap-stage
// fields the module reads at input; the others are zero.
//
// This simplified driver sets the bare minimum for a read/write/fill.
struct PipeReq {
    uint8_t  opcode;
    uint64_t address;
    uint8_t  address_space;
    uint16_t set;
    uint8_t  sub_bank_id;      // which sub_bank (0..3)
    uint8_t  sub_bank_mask;    // one-hot
    uint8_t  qwen;             // QW enable
    uint64_t trans_id;
    uint8_t  reqq_id;
    uint8_t  size;
    bool     zero_data;

    // pipe_info fields set at _tap
    bool     pipe_dec_err;
    bool     pipe_scp_read;
    bool     pipe_bubble;
    bool     idx_write;
    // tag compare results (normally from tc, but fed at tap here for cosim simplicity)
    bool     tag_hit;
    uint8_t  tag_ram_hit_way;
    bool     tag_sbe;
    bool     victim;
    // data control
    bool     data_rd_req;
    bool     data_wr_req;
    bool     data_wr_zero;
    uint8_t  data_wr_qwen;
    bool     dataq_rd_req_en;
    bool     dataq_rd_req_partial;
    uint8_t  dataq_sel_qwen;
    bool     merge_wr_data_in_rd_rsp;
    bool     merge_rd_data_in_wr_req;
    bool     data_rsp_req;
    bool     data_rsp_zero;
    uint8_t  data_rsp_qwen;
};

static PipeReq make_simple_req(uint8_t opcode, uint8_t addr_space,
                               uint64_t address, uint16_t set,
                               uint8_t sub_bank_id) {
    PipeReq r = {};
    r.opcode        = opcode;
    r.address       = address;
    r.address_space = addr_space;
    r.set           = set;
    r.sub_bank_id   = sub_bank_id;
    r.sub_bank_mask = 1u << sub_bank_id;
    r.qwen          = 0xF;  // all QWs
    r.trans_id      = 0;
    r.reqq_id       = 0;
    r.size          = SIZE_LINE;
    return r;
}

static void drive_req(DUT* dut, const PipeReq& r) {
    dut->pipe_valid_tap_i = 1;
    // pipe_info_tap fields
    dut->pi_pipe_set_i              = r.set;
    dut->pi_pipe_sub_bank_i         = r.sub_bank_mask;
    dut->pi_pipe_idx_address_space_i = r.address_space;
    dut->pi_pipe_dec_err_i          = r.pipe_dec_err;
    dut->pi_pipe_dec_err_type_i     = 0;
    dut->pi_pipe_scp_read_i         = r.pipe_scp_read;
    dut->pi_pipe_bubble_i           = r.pipe_bubble;
    dut->pi_idx_write_i             = r.idx_write;
    dut->pi_tag_hit_i               = r.tag_hit;
    dut->pi_tag_ram_hit_way_i       = r.tag_ram_hit_way;
    dut->pi_tag_sbe_i               = r.tag_sbe;
    dut->pi_victim_i                = r.victim;
    dut->pi_data_rd_req_i           = r.data_rd_req;
    dut->pi_data_wr_req_i           = r.data_wr_req;
    dut->pi_data_wr_zero_i          = r.data_wr_zero;
    dut->pi_data_wr_qwen_i          = r.data_wr_qwen;
    dut->pi_dataq_rd_req_en_i       = r.dataq_rd_req_en;
    dut->pi_dataq_rd_req_partial_i  = r.dataq_rd_req_partial;
    dut->pi_dataq_sel_qwen_i        = r.dataq_sel_qwen;
    dut->pi_merge_wr_data_in_rd_rsp_i = r.merge_wr_data_in_rd_rsp;
    dut->pi_merge_rd_data_in_wr_req_i = r.merge_rd_data_in_wr_req;
    dut->pi_data_rsp_req_i          = r.data_rsp_req;
    dut->pi_data_rsp_zero_i         = r.data_rsp_zero;
    dut->pi_data_rsp_qwen_i         = r.data_rsp_qwen;

    // pipe_req_info_tap fields
    dut->pr_trans_id_i      = r.trans_id;
    dut->pr_reqq_id_i       = r.reqq_id;
    dut->pr_opcode_i        = r.opcode;
    dut->pr_address_i       = r.address;
    dut->pr_address_space_i = r.address_space;
    dut->pr_zero_data_i     = r.zero_data;
    dut->pr_qwen_i          = r.qwen;
    dut->pr_size_i          = r.size;
    dut->pr_sub_bank_id_i   = r.sub_bank_id;
}

// ── Compare all key outputs ──
static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("squash_tap",
                (uint32_t)d->orig_pipe_squash_tap_o,
                (uint32_t)d->new_pipe_squash_tap_o);

    sim.compare("tag_rsp_valid",
                (uint32_t)d->orig_tag_rsp_valid_o,
                (uint32_t)d->new_tag_rsp_valid_o);
    // Compare full tag_rsp_info as packed bits (VlWide<6>)
    if (d->orig_tag_rsp_valid_o || d->new_tag_rsp_valid_o) {
        bool match = (memcmp(d->orig_tag_rsp_info_o.data(),
                             d->new_tag_rsp_info_o.data(),
                             sizeof(d->orig_tag_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] tag_rsp_info: differs\n",
                   (unsigned long)sim.sim_time);
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
    // Compare data_rsp_info when valid (VlWide<19>)
    if (d->orig_data_rsp_valid_o || d->new_data_rsp_valid_o) {
        bool match = (memcmp(d->orig_data_rsp_info_o.data(),
                             d->new_data_rsp_info_o.data(),
                             sizeof(d->orig_data_rsp_info_o)) == 0);
        if (!match) {
            sim.comparisons++;
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] data_rsp_info: differs\n",
                   (unsigned long)sim.sim_time);
        } else {
            sim.comparisons++;
        }
    }

    sim.compare("dataq_rd_req_valid",
                (uint32_t)d->orig_dataq_rd_req_valid_o,
                (uint32_t)d->new_dataq_rd_req_valid_o);

    sim.compare("cbuf_inv_req_valid",
                (uint32_t)d->orig_cbuf_invalidate_req_valid_o,
                (uint32_t)d->new_cbuf_invalidate_req_valid_o);
    sim.compare("cbuf_req_valid",
                (uint32_t)d->orig_cbuf_req_valid_o,
                (uint32_t)d->new_cbuf_req_valid_o);

    sim.compare("rbuf_inv_req_valid",
                (uint32_t)d->orig_rbuf_invalidate_req_valid_o,
                (uint32_t)d->new_rbuf_invalidate_req_valid_o);
    sim.compare("rbuf_install_req_valid",
                (uint32_t)d->orig_rbuf_install_req_valid_o,
                (uint32_t)d->new_rbuf_install_req_valid_o);
    sim.compare("rbuf_evict_req_valid",
                (uint32_t)d->orig_rbuf_evict_req_valid_o,
                (uint32_t)d->new_rbuf_evict_req_valid_o);
    sim.compare("rbuf_data_req_valid",
                (uint32_t)d->orig_rbuf_data_req_valid_o,
                (uint32_t)d->new_rbuf_data_req_valid_o);

    // RAM request enables (important for cycle-accurate behavior)
    sim.compare("ts_ram_rd_en",
                (uint32_t)d->orig_ts_ram_rd_en_o,
                (uint32_t)d->new_ts_ram_rd_en_o);
    sim.compare("ts_ram_wr_en",
                (uint32_t)d->orig_ts_ram_wr_en_o,
                (uint32_t)d->new_ts_ram_wr_en_o);
    sim.compare("tag_ram_rd_en",
                (uint32_t)d->orig_tag_ram_rd_en_o,
                (uint32_t)d->new_tag_ram_rd_en_o);
    sim.compare("tag_ram_wr_en",
                (uint32_t)d->orig_tag_ram_wr_en_o,
                (uint32_t)d->new_tag_ram_wr_en_o);
    sim.compare("data_ram_rd_en",
                (uint32_t)d->orig_data_ram_rd_en_o,
                (uint32_t)d->new_data_ram_rd_en_o);
    sim.compare("data_ram_wr_en",
                (uint32_t)d->orig_data_ram_wr_en_o,
                (uint32_t)d->new_data_ram_wr_en_o);
    sim.compare("data_ram_amo_wr_en",
                (uint32_t)d->orig_data_ram_amo_wr_en_o,
                (uint32_t)d->new_data_ram_amo_wr_en_o);
}

// Build an L2-aligned address from set/bank/sub_bank
// L2 address layout: [tag | set | sub_bank | bank | line_offset]
//   line_offset = 6 bits (64B line)
//   bank = 2 bits
//   sub_bank = 2 bits
//   set = 9 bits (SetIdSize)
//   tag = remaining
static uint64_t make_l2_addr(uint16_t set, uint8_t sub_bank, uint8_t bank, uint32_t tag) {
    // BankLsb = LineOffsetIdSize = 6
    // SubBankLsb = BankLsb + BankIdSize = 6 + 2 = 8
    // SetLsb = SubBankLsb + SubBankIdSize = 8 + 2 = 10
    uint64_t addr = 0;
    addr |= ((uint64_t)bank & 0x3) << 6;
    addr |= ((uint64_t)sub_bank & 0x3) << 8;
    addr |= ((uint64_t)set & 0x1FF) << 10;
    addr |= ((uint64_t)tag & 0xFFFFF) << 19;
    return addr;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000000;
    auto* dut = sim.dut.get();

    // ── Default ESR config ──
    dut->esr_sc_ram_delay_i = 2;
    dut->esr_sc_l2_set_base_i  = 0;
    dut->esr_sc_l2_set_size_i  = 256;  // reasonable L2 allocation
    dut->esr_sc_l2_set_mask_i  = 0x1FF;
    dut->esr_sc_l2_tag_mask_i  = 0x1FF;
    dut->esr_sc_l3_set_base_i  = 0;
    dut->esr_sc_l3_set_size_i  = 128;
    dut->esr_sc_l3_set_mask_i  = 0x1FF;
    dut->esr_sc_l3_tag_mask_i  = 0x1FF;
    dut->esr_sc_scp_set_base_i = 0;
    dut->esr_sc_scp_set_size_i = 64;
    dut->esr_sc_scp_set_mask_i = 0x1FF;
    dut->esr_sc_scp_tag_mask_i = 0x1FF;
    dut->esr_sc_l2_rbuf_enable_i = 0;
    dut->esr_sc_scp_rbuf_enable_i = 0;
    dut->esr_sc_zero_state_enable_i = 1;
    dut->esr_sc_all_shire_aliasing_i = 0;
    dut->esr_sc_two_shire_aliasing_use_shire_lsb_i = 0;
    dut->my_bank_id_i = 1;
    dut->my_l3_virtual_shire_id_i = 0;
    clear_pipe(dut);

    sim.reset();

    // After reset, run a few idle cycles to let the modules settle
    for (int i = 0; i < 20; i++) {
        clear_pipe(dut);
        sim.tick();
        compare_all(sim);
    }

    // ════════════════════════════════════════════════════════
    // Phase 1: Simple L2 read (miss, tag-state empty)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 1: L2 read (miss into empty tag-state)\n");
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        // Let pipeline drain
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 1 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 2: L2 fill (installs tag and data)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 2: L2 fill\n");
    {
        PipeReq r = make_simple_req(OP_L2_FILL, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        r.data_wr_req  = true;
        r.data_wr_qwen = 0xF;
        r.dataq_rd_req_en = true;
        r.dataq_sel_qwen = 0xF;
        r.merge_wr_data_in_rd_rsp = false;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 2 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 3: L2 read hit (after fill)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 3: L2 read (should hit if fill installed tag)\n");
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        r.data_rd_req  = true;
        r.data_rsp_req = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 3 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 4: Multiple L2 reads to different sets
    // ════════════════════════════════════════════════════════
    printf("cosim phase 4: multiple L2 reads (different sets, same sub-bank)\n");
    for (int s = 1; s <= 4; s++) {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(s, 0, 1, 0x200+s), s, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        // 1 idle cycle between reads
        sim.tick(); compare_all(sim);
    }
    // Drain pipeline
    for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 4 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 5: L2 reads to different sub-banks
    // ════════════════════════════════════════════════════════
    printf("cosim phase 5: L2 reads to different sub-banks\n");
    for (int sb = 0; sb < SUB_BANKS; sb++) {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(10, sb, 1, 0x300+sb), 10, sb);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        sim.tick(); compare_all(sim);
    }
    for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 5 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 6: L2 write (miss, creates victim)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 6: L2 write (miss)\n");
    {
        PipeReq r = make_simple_req(OP_L2_WRITE, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x400), 0, 0);
        r.data_wr_req = true;
        r.data_wr_qwen = 0xF;
        r.dataq_rd_req_en = true;
        r.dataq_sel_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 6 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 7: L2 evict
    // ════════════════════════════════════════════════════════
    printf("cosim phase 7: L2 evict\n");
    {
        PipeReq r = make_simple_req(OP_L2_EVICT, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x400), 0, 0);
        r.victim = true;
        r.data_rd_req = true;
        r.data_rsp_req = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 7 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 8: L2 flush
    // ════════════════════════════════════════════════════════
    printf("cosim phase 8: L2 flush\n");
    {
        PipeReq r = make_simple_req(OP_L2_FLUSH, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        r.victim = true;
        r.data_rd_req = true;
        r.data_rsp_req = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 8 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 9: IdxAllInv (squashes tag reads)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 9: IdxAllInv\n");
    {
        PipeReq r = make_simple_req(OP_IDX_ALL_INV, ADDR_IDX,
                                    0, 0, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 9 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 10: IdxL2Inv (walks sets)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 10: IdxL2Inv\n");
    for (int s = 0; s < 4; s++) {
        PipeReq r = make_simple_req(OP_IDX_L2_INV, ADDR_IDX,
                                    make_l2_addr(s, 0, 1, 0), s, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        sim.tick(); compare_all(sim);
    }
    for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 10 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 11: Back-to-back L2 reads (pipeline stress)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 11: back-to-back L2 reads\n");
    for (int i = 0; i < 8; i++) {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(20+i, 0, 1, 0x500+i), 20+i, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
    }
    clear_pipe(dut);
    for (int i = 0; i < 30; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 11 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 12: L2 fill then read-hit sequence
    // ════════════════════════════════════════════════════════
    printf("cosim phase 12: fill+read-hit sequence\n");
    for (int s = 0; s < 4; s++) {
        // Fill
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(50+s, 0, 1, 0x600+s), 50+s, 0);
        fill.data_wr_req    = true;
        fill.data_wr_qwen   = 0xF;
        fill.dataq_rd_req_en = true;
        fill.dataq_sel_qwen = 0xF;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);

        // Let fill complete
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }

        // Read hit
        PipeReq rd = make_simple_req(OP_L2_READ, ADDR_L2,
                                     make_l2_addr(50+s, 0, 1, 0x600+s), 50+s, 0);
        rd.data_rd_req  = true;
        rd.data_rsp_req = true;
        rd.data_rsp_qwen = 0xF;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 12 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 13: L2 write-around
    // ════════════════════════════════════════════════════════
    printf("cosim phase 13: L2 write-around\n");
    {
        PipeReq r = make_simple_req(OP_L2_WRITE_AROUND, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x700), 0, 0);
        r.data_wr_req  = true;
        r.data_wr_qwen = 0xF;
        r.dataq_rd_req_en = true;
        r.dataq_sel_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 13 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 14: SCP zero + SCP read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 14: SCP zero + SCP read\n");
    {
        PipeReq r = make_simple_req(OP_SCP_ZERO, ADDR_SCP,
                                    0, 0, 0);
        r.pipe_scp_read = false;
        r.data_wr_req   = true;
        r.data_wr_zero  = true;
        r.data_wr_qwen  = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    {
        PipeReq r = make_simple_req(OP_SCP_READ, ADDR_SCP,
                                    0, 0, 0);
        r.pipe_scp_read = true;
        r.data_rd_req  = true;
        r.data_rsp_req = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 14 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 15: L2 prefetch
    // ════════════════════════════════════════════════════════
    printf("cosim phase 15: L2 prefetch\n");
    {
        PipeReq r = make_simple_req(OP_L2_PREFETCH, ADDR_L2,
                                    make_l2_addr(80, 0, 1, 0x800), 80, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 15 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 16: L2 scrub (tag rewrite for error correction)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 16: L2 scrub\n");
    {
        PipeReq r = make_simple_req(OP_L2_SCRUB, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 16 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 17: ram_delay=3 test
    // ════════════════════════════════════════════════════════
    printf("cosim phase 17: ram_delay=3 L2 read\n");
    dut->esr_sc_ram_delay_i = 3;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 20; i++) { sim.tick(); compare_all(sim); }
    }
    dut->esr_sc_ram_delay_i = 2;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    printf("  phase 17 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 18: Bubble (pipe_bubble set)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 18: bubble\n");
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        r.pipe_bubble = true;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 18 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 19: dec_err (decoder error)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 19: dec_err\n");
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        r.pipe_dec_err = true;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 19 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 20: Long idle drain
    // ════════════════════════════════════════════════════════
    printf("cosim phase 20: idle drain\n");
    for (int i = 0; i < 30; i++) {
        clear_pipe(dut);
        sim.tick();
        compare_all(sim);
    }
    printf("  phase 20 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 21: L3 read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 21: L3 read\n");
    {
        // L3 address layout differs (shire_id in different position)
        // but we keep it simple for cosim — just pick a reasonable address
        PipeReq r = make_simple_req(OP_L3_READ, ADDR_L3,
                                    make_l2_addr(5, 0, 1, 0x900), 5, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 21 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 22: L3 fill then L3 read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 22: L3 fill + read\n");
    {
        PipeReq fill = make_simple_req(OP_L3_FILL, ADDR_L3,
                                       make_l2_addr(5, 0, 1, 0x900), 5, 0);
        fill.data_wr_req    = true;
        fill.data_wr_qwen   = 0xF;
        fill.dataq_rd_req_en = true;
        fill.dataq_sel_qwen = 0xF;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }

        PipeReq rd = make_simple_req(OP_L3_READ, ADDR_L3,
                                     make_l2_addr(5, 0, 1, 0x900), 5, 0);
        rd.data_rd_req  = true;
        rd.data_rsp_req = true;
        rd.data_rsp_qwen = 0xF;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 22 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 23: Multiple requests back-to-back with different
    //           sub-banks and sets (mixed traffic)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 23: mixed traffic\n");
    for (int iter = 0; iter < 16; iter++) {
        int set  = (iter * 7) % 128;
        int sb   = iter % SUB_BANKS;
        int tag  = 0xA00 + iter;
        uint8_t op = (iter % 3 == 0) ? OP_L2_READ :
                     (iter % 3 == 1) ? OP_L2_FILL : OP_L2_WRITE;
        PipeReq r = make_simple_req(op, ADDR_L2,
                                    make_l2_addr(set, sb, 1, tag), set, sb);
        if (op == OP_L2_FILL || op == OP_L2_WRITE) {
            r.data_wr_req    = true;
            r.data_wr_qwen   = 0xF;
            r.dataq_rd_req_en = true;
            r.dataq_sel_qwen = 0xF;
        }
        if (op == OP_L2_READ) {
            r.data_rd_req  = true;
            r.data_rsp_req = true;
            r.data_rsp_qwen = 0xF;
        }
        r.reqq_id  = iter & 0x3F;
        r.trans_id = iter;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        // 1 idle cycle
        clear_pipe(dut);
        sim.tick(); compare_all(sim);
    }
    for (int i = 0; i < 30; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 23 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 24: SCP write + SCP read
    // ════════════════════════════════════════════════════════
    printf("cosim phase 24: SCP write + read\n");
    {
        PipeReq w = make_simple_req(OP_SCP_WRITE, ADDR_SCP,
                                    0, 0, 0);
        w.pipe_scp_read  = false;
        w.data_wr_req    = true;
        w.data_wr_qwen   = 0xF;
        w.dataq_rd_req_en = true;
        w.dataq_sel_qwen = 0xF;
        drive_req(dut, w);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    {
        PipeReq r = make_simple_req(OP_SCP_READ, ADDR_SCP,
                                    0, 0, 0);
        r.pipe_scp_read = true;
        r.data_rd_req   = true;
        r.data_rsp_req  = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 24 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 25: IdxWrite (dbg write squash — tag reads skipped)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 25: IdxWrite\n");
    {
        PipeReq r = make_simple_req(OP_IDX_WRITE, ADDR_IDX,
                                    0, 0, 0);
        r.idx_write = true;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 25 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 26: ram_delay=4 test
    // ════════════════════════════════════════════════════════
    printf("cosim phase 26: ram_delay=4\n");
    dut->esr_sc_ram_delay_i = 4;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(0, 0, 1, 0x100), 0, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 25; i++) { sim.tick(); compare_all(sim); }
    }
    // Fill + read at delay=4
    {
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(90, 0, 1, 0xB00), 90, 0);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 25; i++) { sim.tick(); compare_all(sim); }

        PipeReq rd = make_simple_req(OP_L2_READ, ADDR_L2,
                                     make_l2_addr(90, 0, 1, 0xB00), 90, 0);
        rd.data_rd_req = true; rd.data_rsp_req = true; rd.data_rsp_qwen = 0xF;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 25; i++) { sim.tick(); compare_all(sim); }
    }
    dut->esr_sc_ram_delay_i = 2;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    printf("  phase 26 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 27: Fill 4 ways in same set (LRU saturation)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 27: fill 4 ways in same set (LRU saturation)\n");
    for (int way = 0; way < 4; way++) {
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(100, 0, 1, 0xC00+way), 100, 0);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        fill.trans_id = 100 + way;
        fill.reqq_id = way;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    // Now fill a 5th tag → should trigger LRU eviction/victim
    {
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(100, 0, 1, 0xC10), 100, 0);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        fill.trans_id = 200;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 27 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 28: Tag forward substitution — write then read same set
    // ════════════════════════════════════════════════════════
    printf("cosim phase 28: tag forward substitution (write then read same set)\n");
    {
        // Fill to set 110
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(110, 0, 1, 0xD00), 110, 0);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);

        // Immediately read same set — forward substitution in _tn pipeline
        PipeReq rd = make_simple_req(OP_L2_READ, ADDR_L2,
                                     make_l2_addr(110, 0, 1, 0xD00), 110, 0);
        rd.data_rd_req = true; rd.data_rsp_req = true; rd.data_rsp_qwen = 0xF;
        drive_req(dut, rd);
        sim.tick(); compare_all(sim);

        // Another read to same set (1 cycle gap)
        clear_pipe(dut);
        sim.tick(); compare_all(sim);

        PipeReq rd2 = make_simple_req(OP_L2_READ, ADDR_L2,
                                      make_l2_addr(110, 0, 1, 0xD00), 110, 0);
        rd2.data_rd_req = true; rd2.data_rsp_req = true; rd2.data_rsp_qwen = 0xF;
        drive_req(dut, rd2);
        sim.tick(); compare_all(sim);

        clear_pipe(dut);
        for (int i = 0; i < 20; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 28 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 29: Back-to-back fills on same set (write pipeline stress)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 29: back-to-back fills same set\n");
    for (int i = 0; i < 4; i++) {
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(120, i % 4, 1, 0xE00+i), 120, i % 4);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        fill.trans_id = 300 + i;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
    }
    clear_pipe(dut);
    for (int i = 0; i < 30; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 29 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 30: Partial QW writes (partial qwen patterns)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 30: partial QW writes\n");
    {
        // Fill with only QW0
        PipeReq r = make_simple_req(OP_L2_FILL, ADDR_L2,
                                    make_l2_addr(130, 0, 1, 0xF00), 130, 0);
        r.data_wr_req = true; r.data_wr_qwen = 0x1; // only QW0
        r.dataq_rd_req_en = true; r.dataq_sel_qwen = 0x1;
        r.qwen = 0x1;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    {
        // Write partial QW2+QW3
        PipeReq r = make_simple_req(OP_L2_WRITE, ADDR_L2,
                                    make_l2_addr(130, 0, 1, 0xF00), 130, 0);
        r.data_wr_req = true; r.data_wr_qwen = 0xC; // QW2+QW3
        r.dataq_rd_req_en = true; r.dataq_sel_qwen = 0xC;
        r.qwen = 0xC;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    {
        // Read back — should see partial data
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(130, 0, 1, 0xF00), 130, 0);
        r.data_rd_req = true; r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 30 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 31: IdxRead
    // ════════════════════════════════════════════════════════
    printf("cosim phase 31: IdxRead\n");
    {
        PipeReq r = make_simple_req(OP_IDX_READ, ADDR_IDX,
                                    make_l2_addr(0, 0, 1, 0), 0, 0);
        r.data_rd_req = true;
        r.data_rsp_req = true;
        r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 31 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 32: L3 with different bank_id
    // ════════════════════════════════════════════════════════
    printf("cosim phase 32: L3 read with bank_id=3\n");
    dut->my_bank_id_i = 3;
    {
        PipeReq r = make_simple_req(OP_L3_READ, ADDR_L3,
                                    make_l2_addr(5, 0, 3, 0x900), 5, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    dut->my_bank_id_i = 1;
    printf("  phase 32 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 33: Mixed traffic with different sub-banks (pipeline interleave)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 33: interleaved sub-bank traffic\n");
    for (int iter = 0; iter < 16; iter++) {
        int sb = iter % 4;
        int set = 200 + iter;
        uint8_t op = (iter % 4 == 0) ? OP_L2_FILL :
                     (iter % 4 == 1) ? OP_L2_READ :
                     (iter % 4 == 2) ? OP_L2_WRITE : OP_L2_EVICT;
        PipeReq r = make_simple_req(op, ADDR_L2,
                                    make_l2_addr(set, sb, 1, 0x1000+iter), set, sb);
        if (op == OP_L2_FILL || op == OP_L2_WRITE) {
            r.data_wr_req = true; r.data_wr_qwen = 0xF;
            r.dataq_rd_req_en = true; r.dataq_sel_qwen = 0xF;
        }
        if (op == OP_L2_READ) {
            r.data_rd_req = true; r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        }
        if (op == OP_L2_EVICT) {
            r.victim = true;
            r.data_rd_req = true; r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        }
        r.trans_id = 400 + iter;
        r.reqq_id = iter & 0x3F;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
    }
    clear_pipe(dut);
    for (int i = 0; i < 30; i++) { sim.tick(); compare_all(sim); }
    printf("  phase 33 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 34: zero_state_enable toggling
    // ════════════════════════════════════════════════════════
    printf("cosim phase 34: zero_state_enable off\n");
    dut->esr_sc_zero_state_enable_i = 0;
    {
        PipeReq fill = make_simple_req(OP_L2_FILL, ADDR_L2,
                                       make_l2_addr(140, 0, 1, 0x1100), 140, 0);
        fill.data_wr_req = true; fill.data_wr_qwen = 0xF; fill.data_wr_zero = true;
        fill.dataq_rd_req_en = true; fill.dataq_sel_qwen = 0xF;
        fill.zero_data = true;
        drive_req(dut, fill);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    dut->esr_sc_zero_state_enable_i = 1;
    printf("  phase 34 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 35: rbuf_enable test
    // ════════════════════════════════════════════════════════
    printf("cosim phase 35: L2 rbuf_enable on\n");
    dut->esr_sc_l2_rbuf_enable_i = 1;
    {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(150, 0, 1, 0x1200), 150, 0);
        r.data_rd_req = true; r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    dut->esr_sc_l2_rbuf_enable_i = 0;
    printf("  phase 35 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 36: SCP rbuf_enable test
    // ════════════════════════════════════════════════════════
    printf("cosim phase 36: SCP rbuf_enable on\n");
    dut->esr_sc_scp_rbuf_enable_i = 1;
    {
        PipeReq r = make_simple_req(OP_SCP_READ, ADDR_SCP,
                                    0, 0, 0);
        r.pipe_scp_read = true;
        r.data_rd_req = true; r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    dut->esr_sc_scp_rbuf_enable_i = 0;
    printf("  phase 36 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 37: L2 write-partial (partial line write)
    // ════════════════════════════════════════════════════════
    printf("cosim phase 37: L2 write-partial\n");
    {
        PipeReq r = make_simple_req(OP_L2_WRITE_PARTIAL, ADDR_L2,
                                    make_l2_addr(160, 0, 1, 0x1300), 160, 0);
        r.data_wr_req = true; r.data_wr_qwen = 0x5; // QW0 + QW2
        r.dataq_rd_req_en = true; r.dataq_rd_req_partial = true;
        r.dataq_sel_qwen = 0x5;
        r.qwen = 0x5;
        r.size = SIZE_DW;
        r.merge_rd_data_in_wr_req = true;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 37 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 38: L2 fill with merge_wr_data_in_rd_rsp
    // ════════════════════════════════════════════════════════
    printf("cosim phase 38: fill with data merge\n");
    {
        PipeReq r = make_simple_req(OP_L2_FILL, ADDR_L2,
                                    make_l2_addr(170, 0, 1, 0x1400), 170, 0);
        r.data_wr_req = true; r.data_wr_qwen = 0xF;
        r.dataq_rd_req_en = true; r.dataq_sel_qwen = 0xF;
        r.data_rd_req = true;
        r.data_rsp_req = true; r.data_rsp_qwen = 0xF;
        r.merge_wr_data_in_rd_rsp = true;
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
        clear_pipe(dut);
        for (int i = 0; i < 15; i++) { sim.tick(); compare_all(sim); }
    }
    printf("  phase 38 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 39: ram_delay=3 with back-to-back traffic
    // ════════════════════════════════════════════════════════
    printf("cosim phase 39: ram_delay=3 back-to-back\n");
    dut->esr_sc_ram_delay_i = 3;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    for (int iter = 0; iter < 8; iter++) {
        PipeReq r = make_simple_req(OP_L2_READ, ADDR_L2,
                                    make_l2_addr(180+iter, 0, 1, 0x1500+iter), 180+iter, 0);
        drive_req(dut, r);
        sim.tick(); compare_all(sim);
    }
    clear_pipe(dut);
    for (int i = 0; i < 30; i++) { sim.tick(); compare_all(sim); }
    dut->esr_sc_ram_delay_i = 2;
    for (int i = 0; i < 5; i++) { clear_pipe(dut); sim.tick(); compare_all(sim); }
    printf("  phase 39 done\n");

    // ════════════════════════════════════════════════════════
    // Phase 40: Final idle drain
    // ════════════════════════════════════════════════════════
    printf("cosim phase 40: final idle drain\n");
    for (int i = 0; i < 50; i++) {
        clear_pipe(dut);
        sim.tick();
        compare_all(sim);
    }
    printf("  phase 40 done\n");

    return sim.finish();
}

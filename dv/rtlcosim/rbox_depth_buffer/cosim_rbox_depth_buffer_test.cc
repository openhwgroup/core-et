// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_depth_buffer vs new rbox_depth_buffer.

#include "Vcosim_rbox_depth_buffer_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vcosim_rbox_depth_buffer_tb;

// ── VlWide helpers ──────────────────────────────────────

// Compare two VlWide<N> arrays word-by-word.
template <std::size_t N>
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                          const VlWide<N>& orig, const VlWide<N>& newv) {
    for (std::size_t w = 0; w < N; w++) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s[%zu]", name, w);
        sim.compare(buf, orig[w], newv[w]);
    }
}

// Set all words of a VlWide to zero.
template <std::size_t N>
static void zero_wide(VlWide<N>& v) {
    for (std::size_t w = 0; w < N; w++) v[w] = 0;
}

// ── Output comparison ───────────────────────────────────

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("db_sc_req_valid", d->orig_db_sc_req_valid_o, d->new_db_sc_req_valid_o);
    sim.compare("db_fetch_ptr",    d->orig_db_fetch_ptr_o,    d->new_db_fetch_ptr_o);
    sim.compare("db_fetch_done",   d->orig_db_fetch_done_o,   d->new_db_fetch_done_o);
    sim.compare("db_read_data_valid", d->orig_db_read_data_valid_o, d->new_db_read_data_valid_o);
    sim.compare("db_write_done",   d->orig_db_write_done_o,   d->new_db_write_done_o);
    sim.compare("db_flush_done",   d->orig_db_flush_done_o,   d->new_db_flush_done_o);

    // Compare ET-Link request (584 bits = VlWide<19>) when valid
    if (d->orig_db_sc_req_valid_o || d->new_db_sc_req_valid_o) {
        compare_wide(sim, "db_sc_req", d->orig_db_sc_req_o, d->new_db_sc_req_o);
    }

    // Compare read data (512 bits = VlWide<16>) when valid
    if (d->orig_db_read_data_valid_o || d->new_db_read_data_valid_o) {
        compare_wide(sim, "db_read_data", d->orig_db_read_data_o, d->new_db_read_data_o);
    }

    // Compare debug read data (512 bits) — always compare
    compare_wide(sim, "dbg_db_read_data", d->orig_dbg_db_read_data_o, d->new_dbg_db_read_data_o);

    // Compare debug metadata. The original port is 38 bits, the new is 43 bits.
    // The wrapper zero-extends the original to 43 bits. Compare the lower 38 bits
    // (word 0 fully, word 1 low 6 bits).
    sim.compare("dbg_db_meta[0]",
                d->orig_dbg_db_meta_read_data_o & 0xFFFFFFFF,
                d->new_dbg_db_meta_read_data_o  & 0xFFFFFFFF);
    sim.compare("dbg_db_meta[1]",
                (d->orig_dbg_db_meta_read_data_o >> 32) & 0x3F,
                (d->new_dbg_db_meta_read_data_o  >> 32) & 0x3F);
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// ── Build an ET-Link response (534-bit rsp_t) ────────────
//
// rsp_t layout (packed, MSB-first):
//   id[7:0]  dest[3:0]  wdata  opcode[1:0]  data[511:0]  size[2:0]  qwen[3:0]
//
// Total = 8+4+1+2+512+3+4 = 534 bits
//
// In VlWide<17> (544 bits), bit 0 is LSB of qwen.

static void build_rsp(VlWide<17>& rsp, uint8_t id, uint8_t dest, bool wdata,
                       uint8_t opcode, const VlWide<16>* data, uint8_t size,
                       uint8_t qwen) {
    // Start clean
    for (int w = 0; w < 17; w++) rsp[w] = 0;

    // Bit layout from LSB:
    //  [3:0]   = qwen
    //  [6:4]   = size
    //  [518:7] = data (512 bits)
    //  [520:519] = opcode (2 bits)
    //  [521]     = wdata
    //  [525:522] = dest
    //  [533:526] = id

    // qwen (bits 3:0)
    rsp[0] = (uint32_t)(qwen & 0xF);

    // size (bits 6:4)
    rsp[0] |= (uint32_t)(size & 0x7) << 4;

    // data (bits 518:7) — 512 bits starting at bit 7
    // We need to shift the 512-bit data left by 7 bits into rsp.
    if (data) {
        uint32_t carry = 0;
        for (int w = 0; w < 16; w++) {
            uint64_t val = ((uint64_t)(*data)[w] << 7) | carry;
            rsp[w] |= (uint32_t)(val & 0xFFFFFFFF);
            carry = (uint32_t)(val >> 32);
        }
        rsp[16] |= carry;
    }

    // opcode (bits 520:519)
    {
        // bit 519 is in word 16 bit 7, bit 520 is word 16 bit 8
        uint32_t opc_shift = (uint32_t)(opcode & 0x3) << (519 - 512);
        rsp[16] |= opc_shift;
    }

    // wdata (bit 521)
    if (wdata) rsp[16] |= (1u << (521 - 512));

    // dest (bits 525:522)
    rsp[16] |= (uint32_t)(dest & 0xF) << (522 - 512);

    // id (bits 533:526)
    rsp[16] |= (uint32_t)(id & 0x3F) << (526 - 512);  // low 6 bits
    // bits 533:532 -> word 16 bits 21:20? No, 533-512=21, 532-512=20
    // id has 8 bits at [533:526]. 526-512=14, 533-512=21
    // Already covered: id shifted by 14 bits into word 16.
    // But we only put (id & 0x3F) << 14 above. We need full 8 bits:
    rsp[16] &= ~((uint32_t)0xFF << 14);  // clear
    rsp[16] |= (uint32_t)(id & 0xFF) << 14;
}

// Build an AckData response (fill): opcode=1 (RspAckData), wdata=1
static void build_fill_rsp(VlWide<17>& rsp, uint8_t entry_id,
                            const VlWide<16>& data) {
    // ID bit 7 = 1 for depth buffer source
    build_rsp(rsp, entry_id | 0x80, 0, true, 1 /*RspAckData*/, &data, 6 /*SizeLine*/, 0xF);
}

// Build an Ack response (spill ack): opcode=0 (RspAck), wdata=0
static void build_spill_ack_rsp(VlWide<17>& rsp, uint8_t entry_id) {
    build_rsp(rsp, entry_id | 0x80, 0, false, 0 /*RspAck*/, nullptr, 6 /*SizeLine*/, 0);
}

// ── Initialize all inputs ───────────────────────────────

static void init_inputs(DUT* d) {
    d->db_sc_req_ready_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    d->sc_db_rsp_valid_i = 0;
    d->db_fetch_address_i = 0;
    d->db_fetch_quad_i = 0;
    d->db_fetch_valid_i = 0;
    d->db_read_ptr_i = 0;
    d->db_read_valid_i = 0;
    d->db_write_ptr_i = 0;
    for (int w = 0; w < 16; w++) d->db_write_data_i[w] = 0;
    d->db_write_valid_i = 0;
    d->db_dealloc_ptr_i = 0;
    d->db_dealloc_quad_i = 0;
    d->db_dealloc_valid_i = 0;
    d->db_flush_i = 0;
    d->dbg_ram_ptr_i = 0;
    d->dbg_db_read_valid_i = 0;
    for (int w = 0; w < 16; w++) d->dbg_db_write_data_i[w] = 0;
    d->dbg_db_write_valid_i = 0;
    d->dbg_db_meta_write_data_i = 0;
    d->dbg_db_meta_write_valid_i = 0;
}

static void clear_stimulus(DUT* d) {
    d->db_sc_req_ready_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    d->sc_db_rsp_valid_i = 0;
    d->db_fetch_valid_i = 0;
    d->db_read_valid_i = 0;
    d->db_write_valid_i = 0;
    d->db_dealloc_valid_i = 0;
    d->db_flush_i = 0;
    d->dbg_db_read_valid_i = 0;
    d->dbg_db_write_valid_i = 0;
    d->dbg_db_meta_write_valid_i = 0;
}

// ── Main test ───────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;  // need more cycles for depth buffer tests

    init_inputs(sim.dut.get());
    sim.reset(10);
    compare_outputs(sim);

    auto* d = sim.dut.get();

    // ── Test 1: Reset → verify all outputs idle ─────────
    printf("cosim test 1: reset idle\n");
    d->db_sc_req_ready_i = 1;
    tick_and_compare(sim);
    tick_and_compare(sim);
    clear_stimulus(d);
    tick_and_compare(sim);

    // ── Test 2: Fetch miss → generates ET-Link read ─────
    printf("cosim test 2: fetch miss → read request\n");
    // Address 0x1000 (tag = addr[39:6] = 0x40), bank = addr[7:6] = 0
    d->db_sc_req_ready_i = 1;
    d->db_fetch_address_i = 0x1000;
    d->db_fetch_quad_i = 1;
    d->db_fetch_valid_i = 1;
    tick_and_compare(sim);

    // The request should appear: check that req_valid is nonzero
    d->db_fetch_valid_i = 0;
    tick_and_compare(sim);

    // Let the request be accepted
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 3; i++) tick_and_compare(sim);

    // ── Test 3: Fetch hit (same address) → no new request ─
    printf("cosim test 3: fetch hit → no new request\n");
    d->db_fetch_address_i = 0x1000;
    d->db_fetch_quad_i = 2;
    d->db_fetch_valid_i = 1;
    tick_and_compare(sim);
    d->db_fetch_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_and_compare(sim);

    // ── Test 6: Respond to read request → entry valid ───
    printf("cosim test 6: fill response → entry becomes valid\n");
    // Send AckData response with id = fetch_ptr | 0x80
    {
        VlWide<16> fill_data;
        for (int w = 0; w < 16; w++) fill_data[w] = 0xA0000000 + w;
        VlWide<17> rsp;
        build_fill_rsp(rsp, d->new_db_fetch_ptr_o, fill_data);
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
        d->sc_db_rsp_valid_i = 1;
    }
    tick_and_compare(sim);
    d->sc_db_rsp_valid_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // ── Test 4: Write → marks entry dirty ───────────────
    printf("cosim test 4: write → marks dirty\n");
    d->db_write_ptr_i = d->new_db_fetch_ptr_o;
    for (int w = 0; w < 16; w++) d->db_write_data_i[w] = 0xBB000000 + w;
    d->db_write_valid_i = 1;
    tick_and_compare(sim);
    d->db_write_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_and_compare(sim);

    // ── Read back the data ──────────────────────────────
    printf("cosim test: read back written data\n");
    d->db_read_ptr_i = d->new_db_fetch_ptr_o;
    d->db_read_valid_i = 1;
    tick_and_compare(sim);
    d->db_read_valid_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // ── Test 5: Fetch with eviction → write-back + read ─
    printf("cosim test 5: fetch eviction → spill then fill\n");
    // Fill ALL 64 entries first, so the LRU victim is dirty and valid.
    // We already have entry 0 filled. Fill entries 1..63 by fetching different addresses.
    // Address format: tag = addr[39:6]. Use addr = (i+1) * 64 to get unique tags.
    d->db_sc_req_ready_i = 1;
    for (int i = 1; i < 64; i++) {
        // Fetch unique address
        d->db_fetch_address_i = (uint64_t)(i + 1) * 64;
        d->db_fetch_quad_i = (i & 0x7F);
        d->db_fetch_valid_i = 1;
        tick_and_compare(sim);
        d->db_fetch_valid_i = 0;
        tick_and_compare(sim);

        // Accept any pending request
        d->db_sc_req_ready_i = 1;
        tick_and_compare(sim);

        // Send fill response
        {
            VlWide<16> fill_data;
            for (int w = 0; w < 16; w++) fill_data[w] = 0xC0000000 + i * 0x100 + w;
            VlWide<17> rsp;
            build_fill_rsp(rsp, d->new_db_fetch_ptr_o, fill_data);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
        }
        tick_and_compare(sim);
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        tick_and_compare(sim);

        // Write to mark dirty
        d->db_write_ptr_i = d->new_db_fetch_ptr_o;
        for (int w = 0; w < 16; w++) d->db_write_data_i[w] = 0xDD000000 + i * 0x100 + w;
        d->db_write_valid_i = 1;
        tick_and_compare(sim);
        d->db_write_valid_i = 0;
        tick_and_compare(sim);

        // Dealloc to clear reserved
        d->db_dealloc_ptr_i = d->new_db_fetch_ptr_o;
        d->db_dealloc_quad_i = (i & 0x7F);
        d->db_dealloc_valid_i = 1;
        tick_and_compare(sim);
        d->db_dealloc_valid_i = 0;
        tick_and_compare(sim);
    }

    // Also dealloc entry 0 (from earlier tests)
    d->db_dealloc_ptr_i = 0;
    d->db_dealloc_quad_i = 2;  // last quad assigned to entry 0 was 2 (test 3)
    d->db_dealloc_valid_i = 1;
    tick_and_compare(sim);
    d->db_dealloc_valid_i = 0;
    tick_and_compare(sim);

    // Now fetch a new address — should evict the LRU (which is dirty+valid)
    d->db_fetch_address_i = 0x80000;  // new unique tag
    d->db_fetch_quad_i = 5;
    d->db_fetch_valid_i = 1;
    d->db_sc_req_ready_i = 1;
    tick_and_compare(sim);
    d->db_fetch_valid_i = 0;

    // Wait for spill write request, then fill read request
    for (int i = 0; i < 5; i++) {
        d->db_sc_req_ready_i = 1;
        tick_and_compare(sim);
    }

    // ── Test 7: Respond to spill ack ────────────────────
    printf("cosim test 7: spill ack\n");
    {
        // Get the entry index from the request id (low 7 bits of id)
        // Just send a generic spill ack for the entry we evicted
        VlWide<17> rsp;
        // We don't know exactly which entry, but the spill request's id has it.
        // Use entry that was LRU. After filling 0..63 in order, entry 0 is LRU.
        build_spill_ack_rsp(rsp, d->new_db_fetch_ptr_o);
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
        d->sc_db_rsp_valid_i = 1;
    }
    tick_and_compare(sim);
    d->sc_db_rsp_valid_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    for (int i = 0; i < 3; i++) tick_and_compare(sim);

    // Send fill response
    {
        VlWide<16> fill_data;
        for (int w = 0; w < 16; w++) fill_data[w] = 0xEE000000 + w;
        VlWide<17> rsp;
        build_fill_rsp(rsp, d->new_db_fetch_ptr_o, fill_data);
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
        d->sc_db_rsp_valid_i = 1;
    }
    tick_and_compare(sim);
    d->sc_db_rsp_valid_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    for (int i = 0; i < 3; i++) tick_and_compare(sim);

    // ── Test 9: Dealloc → clears reserved ───────────────
    printf("cosim test 9: dealloc\n");
    d->db_dealloc_ptr_i = d->new_db_fetch_ptr_o;
    d->db_dealloc_quad_i = 5;
    d->db_dealloc_valid_i = 1;
    tick_and_compare(sim);
    d->db_dealloc_valid_i = 0;
    tick_and_compare(sim);

    // ── Test 8: Flush → iterates dirty entries ──────────
    printf("cosim test 8: flush\n");
    d->db_flush_i = 1;
    d->db_sc_req_ready_i = 1;

    // Flush needs to iterate all 64 entries. For each dirty+valid entry,
    // it generates a spill. We need to accept requests and send acks.
    int flush_cycles = 0;
    int acks_sent = 0;
    while (!d->new_db_flush_done_o && flush_cycles < 2000) {
        tick_and_compare(sim);
        flush_cycles++;
        d->db_sc_req_ready_i = 1;

        // If there's a pending write request, send ack
        if (d->new_db_sc_req_valid_o) {
            // Extract id from the request to send matching ack.
            // req_t id is at bits [583:576]. In VlWide<19>:
            // word 18 bits [7:0] = id (583-576=7, 576/32=18, 576%32=0)
            uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
            VlWide<17> rsp;
            build_spill_ack_rsp(rsp, req_id & 0x7F);  // keep id but clear bit 7
            // Actually we need bit 7 set — the ack check uses id[7]
            build_spill_ack_rsp(rsp, req_id);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
            acks_sent++;
        } else {
            d->sc_db_rsp_valid_i = 0;
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        }
    }
    d->sc_db_rsp_valid_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    d->db_flush_i = 0;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);
    printf("  flush completed in %d cycles, %d acks sent\n", flush_cycles, acks_sent);

    // ── Test 10: Debug read/write ───────────────────────
    printf("cosim test 10: debug read/write\n");
    // Debug write to entry 0
    d->dbg_ram_ptr_i = 0;
    for (int w = 0; w < 16; w++) d->dbg_db_write_data_i[w] = 0xFF000000 + w;
    d->dbg_db_write_valid_i = 1;
    tick_and_compare(sim);
    d->dbg_db_write_valid_i = 0;
    tick_and_compare(sim);

    // Debug read from entry 0
    d->dbg_ram_ptr_i = 0;
    d->dbg_db_read_valid_i = 1;
    tick_and_compare(sim);
    d->dbg_db_read_valid_i = 0;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // Debug metadata write
    d->dbg_ram_ptr_i = 3;
    d->dbg_db_meta_write_data_i = 0x12345;
    d->dbg_db_meta_write_valid_i = 1;
    tick_and_compare(sim);
    d->dbg_db_meta_write_valid_i = 0;
    tick_and_compare(sim);

    // Debug metadata read
    d->dbg_ram_ptr_i = 3;
    tick_and_compare(sim);

    // ── Test 11: Random stimulus (200+ cycles) ──────────
    printf("cosim test 11: random stimulus (300 cycles)\n");
    uint32_t seed = 0xDEADBEEF;
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 300; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

        clear_stimulus(d);
        d->db_sc_req_ready_i = (seed & 0x3) != 0;  // ready 75% of time

        uint8_t action = (seed >> 4) & 0xF;

        if (action < 4) {
            // Fetch with random address
            d->db_fetch_address_i = (uint64_t)((seed >> 8) & 0xFFFFC0);
            d->db_fetch_quad_i = (seed >> 24) & 0x7F;
            d->db_fetch_valid_i = 1;
        } else if (action < 6) {
            // Read
            d->db_read_ptr_i = (seed >> 8) & 0x3F;
            d->db_read_valid_i = 1;
        } else if (action < 8) {
            // Write
            d->db_write_ptr_i = (seed >> 8) & 0x3F;
            for (int w = 0; w < 16; w++) d->db_write_data_i[w] = seed + w;
            d->db_write_valid_i = 1;
        } else if (action < 10) {
            // Dealloc
            d->db_dealloc_ptr_i = (seed >> 8) & 0x3F;
            d->db_dealloc_quad_i = (seed >> 16) & 0x7F;
            d->db_dealloc_valid_i = 1;
        } else if (action < 12) {
            // Send fill response
            VlWide<16> fill_data;
            for (int w = 0; w < 16; w++) fill_data[w] = seed + w * 0x111;
            VlWide<17> rsp;
            build_fill_rsp(rsp, (seed >> 8) & 0x3F, fill_data);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
        } else if (action < 14) {
            // Send spill ack
            VlWide<17> rsp;
            build_spill_ack_rsp(rsp, (seed >> 8) & 0x3F);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
        } else {
            // Debug read or write
            d->dbg_ram_ptr_i = (seed >> 8) & 0x3F;
            if (seed & 0x100) {
                d->dbg_db_read_valid_i = 1;
            } else {
                for (int w = 0; w < 16; w++) d->dbg_db_write_data_i[w] = seed + w;
                d->dbg_db_write_valid_i = 1;
            }
        }

        tick_and_compare(sim);
    }

    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    return sim.finish();
}

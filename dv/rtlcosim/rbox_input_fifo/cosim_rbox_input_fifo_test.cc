// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_input_fifo vs new rbox_input_fifo.

#include "Vcosim_rbox_input_fifo_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_rbox_input_fifo_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("alloc_ptr",    sim.dut->orig_alloc_ptr_o,    sim.dut->new_alloc_ptr_o);
    sim.compare("push_done",    sim.dut->orig_push_done_o,    sim.dut->new_push_done_o);
    sim.compare("out_valid",    sim.dut->orig_out_valid_o,    sim.dut->new_out_valid_o);
    sim.compare("pop_done",     sim.dut->orig_pop_done_o,     sim.dut->new_pop_done_o);
    sim.compare("full",         sim.dut->orig_full_o,         sim.dut->new_full_o);
    sim.compare("empty",        sim.dut->orig_empty_o,        sim.dut->new_empty_o);
    sim.compare("out_pck_type", sim.dut->orig_out_pck_type_o, sim.dut->new_out_pck_type_o);
    if (sim.dut->orig_out_valid_o || sim.dut->new_out_valid_o) {
        // Compare data word-by-word (512 bits = 16 x 32-bit words)
        for (int w = 0; w < 16; w++) {
            sim.compare("out_data", sim.dut->orig_out_data_o[w], sim.dut->new_out_data_o[w]);
        }
    }
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// Build a 512-bit packet with inpck_type in the low 3 bits and a pattern.
static void make_packet(DUT* dut, uint8_t pck_type, uint32_t pattern) {
    for (int w = 0; w < 16; w++)
        dut->in_data_i[w] = pattern + w;
    // Set packet type in low 3 bits
    dut->in_data_i[0] = (dut->in_data_i[0] & ~0x7) | (pck_type & 0x7);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    // Initialize
    sim.dut->alloc_i     = 0;
    for (int w = 0; w < 16; w++) sim.dut->in_data_i[w] = 0;
    sim.dut->push_ptr_i  = 0;
    sim.dut->push_i      = 0;
    sim.dut->pop_i       = 0;
    sim.dut->buffer_end_i = 0;
    sim.reset(10);
    compare_outputs(sim);

    // ── Test 1: Empty after reset ───────────────────────
    printf("cosim test 1: empty after reset\n");
    tick_and_compare(sim);

    // ── Test 2: Alloc, push, read, pop (1-word packet) ──
    printf("cosim test 2: alloc + push + read + pop (1-word pkt)\n");
    // Alloc one entry
    sim.dut->alloc_i = 1;
    tick_and_compare(sim);
    uint8_t alloc_ptr = sim.dut->new_alloc_ptr_o;
    sim.dut->alloc_i = 0;
    tick_and_compare(sim);

    // Push data (type=7 = END_OF_BUFFER, 1-word packet)
    make_packet(sim.dut.get(), 7, 0xDEAD0000);
    sim.dut->push_i = 1;
    sim.dut->push_ptr_i = alloc_ptr;
    tick_and_compare(sim);
    sim.dut->push_i = 0;
    tick_and_compare(sim);

    // Wait for data to appear at output
    for (int i = 0; i < 3; i++)
        tick_and_compare(sim);

    // Pop
    sim.dut->pop_i = 1;
    tick_and_compare(sim);
    sim.dut->pop_i = 0;
    tick_and_compare(sim);

    // ── Test 3: Multiple alloc/push/pop cycles ──────────
    printf("cosim test 3: multiple packets\n");
    for (int pkt = 0; pkt < 10; pkt++) {
        // Alloc
        sim.dut->alloc_i = 1;
        tick_and_compare(sim);
        alloc_ptr = sim.dut->new_alloc_ptr_o;
        sim.dut->alloc_i = 0;

        // Push (type 3 = FULLY_COVERED_TILE, 1-word packet)
        make_packet(sim.dut.get(), 3, 0x10000 * pkt);
        sim.dut->push_i = 1;
        sim.dut->push_ptr_i = alloc_ptr;
        tick_and_compare(sim);
        sim.dut->push_i = 0;

        // Wait for valid
        for (int w = 0; w < 3; w++)
            tick_and_compare(sim);

        // Pop
        if (sim.dut->new_out_valid_o) {
            sim.dut->pop_i = 1;
            tick_and_compare(sim);
            sim.dut->pop_i = 0;
        }
        tick_and_compare(sim);
    }

    // ── Test 4: Fill multiple entries, then drain ────────
    printf("cosim test 4: fill + drain\n");
    for (int i = 0; i < 8; i++) {
        sim.dut->alloc_i = 1;
        tick_and_compare(sim);
        alloc_ptr = sim.dut->new_alloc_ptr_o;
        sim.dut->alloc_i = 0;

        make_packet(sim.dut.get(), 7, 0xAA000000 + i * 0x100);
        sim.dut->push_i = 1;
        sim.dut->push_ptr_i = alloc_ptr;
        tick_and_compare(sim);
        sim.dut->push_i = 0;
        tick_and_compare(sim);
    }

    // Drain
    for (int i = 0; i < 20; i++) {
        if (sim.dut->new_out_valid_o) {
            sim.dut->pop_i = 1;
            tick_and_compare(sim);
            sim.dut->pop_i = 0;
        }
        tick_and_compare(sim);
    }

    // ── Test 5: Packed word stream (odd read pointer) ────
    // Simulates the real input buffer layout:
    //   Line 0: RBOX_STATE (type=5, inpck_sz=2)  → alloc+push at addr 0
    //   Line 1: {TRI[255:0], FSH_STATE}           → alloc+push at addr 1
    //   Line 2: {END_BUF, TRI[511:256]}           → alloc+push at addr 2
    // Then: pop sz=2 (RBOX_STATE), pop sz=1 (FSH_STATE) → read_ptr=3 (odd)
    // → FIFO reconstructs TRI from SRAM1[1] + SRAM0[2] with byte swap
    // → verify full 512-bit data matches expected.
    printf("cosim test 5: packed word stream (odd read ptr)\n");
    sim.reset(10);
    compare_outputs(sim);

    {
        // Build 3 cache lines with distinctive patterns
        VlWide<16> line0, line1, line2;

        // Line 0: RBOX_STATE (type=5), pattern 0xA0xxxxxx
        for (int w = 0; w < 16; w++) line0[w] = 0xA0000000 + w;
        line0[0] = (line0[0] & ~0x7) | 5;  // type=5 (RBOX_STATE, inpck_sz=2)

        // Line 1: lower=FSH_STATE (type=6), upper=TRI lower half (type=0)
        for (int w = 0; w < 8; w++) line1[w] = 0xB0000000 + w;      // FSH_STATE
        line1[0] = (line1[0] & ~0x7) | 6;                             // type=6 (FSH_STATE, sz=1)
        for (int w = 0; w < 8; w++) line1[w + 8] = 0xC0000000 + w;  // TRI lower
        line1[8] = (line1[8] & ~0x7) | 0;                             // type=0 (TRI, sz=2)

        // Line 2: lower=TRI upper half, upper=END_OF_BUFFER (type=7)
        for (int w = 0; w < 8; w++) line2[w] = 0xC0000008 + w;      // TRI upper
        for (int w = 0; w < 8; w++) line2[w + 8] = 0xD0000000 + w;  // END_OF_BUFFER
        line2[8] = (line2[8] & ~0x7) | 7;                             // type=7 (END_OF_BUFFER, sz=1)

        // Alloc + push all 3 lines (with gaps to avoid push/read conflicts)
        for (int i = 0; i < 3; i++) {
            sim.dut->alloc_i = 1;
            tick_and_compare(sim);
            uint8_t ptr = sim.dut->new_alloc_ptr_o;
            sim.dut->alloc_i = 0;
            tick_and_compare(sim);

            VlWide<16>& data = (i == 0) ? line0 : (i == 1) ? line1 : line2;
            for (int w = 0; w < 16; w++) sim.dut->in_data_i[w] = data[w];
            sim.dut->push_i = 1;
            sim.dut->push_ptr_i = ptr;
            tick_and_compare(sim);
            sim.dut->push_i = 0;
            tick_and_compare(sim);
            tick_and_compare(sim);  // extra cycle for SRAM read to settle
        }

        sim.dut->buffer_end_i = 1;  // all lines received
        tick_and_compare(sim);

        // Wait for first packet (RBOX_STATE) to be valid
        for (int i = 0; i < 5; i++) tick_and_compare(sim);

        // Pop RBOX_STATE (sz=2, advances read_ptr by 2)
        sim.dut->pop_i = 1;
        tick_and_compare(sim);
        sim.dut->pop_i = 0;
        for (int i = 0; i < 3; i++) tick_and_compare(sim);

        // Pop FSH_STATE (sz=1, advances read_ptr by 1 → now odd)
        sim.dut->pop_i = 1;
        tick_and_compare(sim);
        sim.dut->pop_i = 0;
        for (int i = 0; i < 3; i++) tick_and_compare(sim);

        // Now read_ptr is odd (3).  FIFO should present the TRI packet
        // reconstructed from SRAM1[1] + SRAM0[2].
        // Expected: out_data[255:0] = line1 upper half = TRI lower
        //           out_data[511:256] = line2 lower half = TRI upper
        // → full 512-bit TRI packet with type=0

        // Pop TRI (sz=2, advances by 2 → read_ptr=5, odd)
        if (sim.dut->new_out_valid_o) {
            sim.dut->pop_i = 1;
            tick_and_compare(sim);
            sim.dut->pop_i = 0;
        }
        for (int i = 0; i < 3; i++) tick_and_compare(sim);

        // Pop END_OF_BUFFER (sz=1) — at read_ptr=5 with buffer_end=1
        if (sim.dut->new_out_valid_o) {
            sim.dut->pop_i = 1;
            tick_and_compare(sim);
            sim.dut->pop_i = 0;
        }
        for (int i = 0; i < 3; i++) tick_and_compare(sim);

        sim.dut->buffer_end_i = 0;
    }

    // ── Test 6: Random stimulus (500 cycles) ────────────
    printf("cosim test 6: random stimulus (500 cycles)\n");
    uint32_t seed = 0xBEEFCAFE;
    int fill = 0;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

        // Random alloc
        int want_alloc = (seed & 0xF) < 6 && fill < 30;
        sim.dut->alloc_i = want_alloc;

        // Random push
        int want_push = (seed & 0xF0) < 0x80;
        if (want_push) {
            make_packet(sim.dut.get(), seed & 0x7, seed);
            sim.dut->push_i = 1;
            sim.dut->push_ptr_i = (seed >> 16) & 0x1F;
        } else {
            sim.dut->push_i = 0;
        }

        // Random pop
        sim.dut->pop_i = ((seed >> 8) & 0xF) < 5;

        sim.dut->buffer_end_i = ((seed >> 12) & 0xFF) == 0;

        tick_and_compare(sim);

        if (want_alloc && !sim.dut->new_full_o) fill++;
        if (sim.dut->new_pop_done_o) fill--;
    }
    sim.dut->alloc_i = 0;
    sim.dut->push_i = 0;
    sim.dut->pop_i = 0;
    sim.dut->buffer_end_i = 0;
    for (int i = 0; i < 5; i++)
        tick_and_compare(sim);

    return sim.finish();
}

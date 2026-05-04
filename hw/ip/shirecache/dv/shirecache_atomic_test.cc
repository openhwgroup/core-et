// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_atomic — Verilator test via SV wrapper.

#include "Vshirecache_atomic_tb.h"
#include "sim_ctrl.h"
#include <cstring>

using DUT = Vshirecache_atomic_tb;

// AMO opcodes
static constexpr uint8_t AMO_SWAP      = 0;
static constexpr uint8_t AMO_ADD       = 1;
static constexpr uint8_t AMO_XOR       = 2;
static constexpr uint8_t AMO_AND       = 3;
static constexpr uint8_t AMO_OR        = 4;
static constexpr uint8_t AMO_MIN       = 5;
static constexpr uint8_t AMO_MAX       = 6;
static constexpr uint8_t AMO_MINU      = 7;
static constexpr uint8_t AMO_MAXU      = 8;
static constexpr uint8_t AMO_COMP_EXCH = 11;

// reqq_opcode_e
static constexpr uint8_t OP_L2_ATOMIC        = 0x0b;
static constexpr uint8_t OP_L2_WRITE_PARTIAL = 0x02;

// Pack atomic operands: [255:0]=data, [262:256]=conf, [268:263]=offset
// conf = {dest(1), size(1), sc_vec(1), opcode(4)} packed MSB-first
static void pack_operands(uint32_t* w, uint8_t amo_op, bool sc_vec, bool size64,
                          uint32_t op_lo, uint32_t op_hi, uint8_t offset) {
    memset(w, 0, 16 * sizeof(uint32_t)); // 512 bits
    // [255:0] = operand data
    w[0] = op_lo;
    w[1] = op_hi;
    // conf at bit 256 = word 8
    uint8_t conf = (amo_op & 0xF) | ((sc_vec ? 1 : 0) << 4) |
                   ((size64 ? 1 : 0) << 5);
    w[8] = conf;
    // offset at bit 263 = word 8, bit 7
    w[8] |= ((uint32_t)(offset & 0x3F)) << 7;
}

// Run a sc32 atomic op and return the result from word 0
static uint32_t run_sc32(SimCtrl<DUT>& sim, uint8_t amo_op,
                          uint32_t operand, uint32_t rd_data) {
    auto* dut = sim.dut.get();
    // pipe_data: put rd_data in word 0
    memset(dut->pipe_data_i.data(), 0, 64);
    dut->pipe_data_i[0] = rd_data;
    // Setup info
    dut->tag_hit_i = 1;
    dut->victim_i = 0;
    dut->pipe_set_i = 0;
    dut->hit_way_i = 0;
    dut->data_rsp_zero_i = 0;
    dut->opcode_i = OP_L2_ATOMIC;
    dut->qwen_i = 0xF;
    dut->address_i = 0;
    // Operands: sc32, offset=0
    pack_operands(dut->atomic_operands_i.data(), amo_op, false, false,
                  operand, 0, 0);
    dut->partial_wr_data_ben_i = 0;
    dut->pipe_valid_i = 1;
    sim.tick();              // posedge: d0 → d1
    dut->pipe_valid_i = 0;
    sim.tick();              // posedge: d1 → d2 (result available)
    return dut->result_wr_data_o[0];
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;
    auto* dut = sim.dut.get();

    dut->pipe_valid_i = 0;
    sim.reset();

    // ── Test 1: Swap ────────────────────────────────────
    printf("test 1: swap (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_SWAP, 0xDEADBEEF, 0x12345678);
        sim.check(r == 0xDEADBEEF, "swap result");
    }

    // ── Test 2: Add ─────────────────────────────────────
    printf("test 2: add (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_ADD, 10, 20);
        sim.check(r == 30, "add result");
    }

    // ── Test 3: XOR ─────────────────────────────────────
    printf("test 3: xor (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_XOR, 0xFF00FF00, 0x0F0F0F0F);
        sim.check(r == 0xF00FF00F, "xor result");
    }

    // ── Test 4: AND ─────────────────────────────────────
    printf("test 4: and (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_AND, 0xFF00FF00, 0x0F0F0F0F);
        sim.check(r == 0x0F000F00, "and result");
    }

    // ── Test 5: OR ──────────────────────────────────────
    printf("test 5: or (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_OR, 0xFF00FF00, 0x0F0F0F0F);
        sim.check(r == 0xFF0FFF0F, "or result");
    }

    // ── Test 6: Signed min ──────────────────────────────
    printf("test 6: signed min (sc32)\n");
    {
        // -1 (0xFFFFFFFF) < 1 signed → min = -1
        uint32_t r = run_sc32(sim, AMO_MIN, 0xFFFFFFFF, 1);
        sim.check(r == 0xFFFFFFFF, "smin(-1,1)=-1");
    }

    // ── Test 7: Signed max ──────────────────────────────
    printf("test 7: signed max (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_MAX, 0xFFFFFFFF, 1);
        sim.check(r == 1, "smax(-1,1)=1");
    }

    // ── Test 8: Unsigned min ────────────────────────────
    printf("test 8: unsigned min (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_MINU, 0xFFFFFFFF, 1);
        sim.check(r == 1, "umin(0xFFFFFFFF,1)=1");
    }

    // ── Test 9: Unsigned max ────────────────────────────
    printf("test 9: unsigned max (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_MAXU, 0xFFFFFFFF, 1);
        sim.check(r == 0xFFFFFFFF, "umax(0xFFFFFFFF,1)=0xFFFFFFFF");
    }

    // ── Test 10: No output when invalid ─────────────────
    printf("test 10: no output when pipe_valid=0\n");
    {
        dut->pipe_valid_i = 0;
        sim.tick();
        sim.tick();
        sim.tick();
        sim.check(dut->result_wr_en_o == 0, "wr_en=0 when idle");
    }

    // ── Test 11: Partial write ──────────────────────────
    printf("test 11: partial write\n");
    {
        memset(dut->pipe_data_i.data(), 0, 64);
        dut->pipe_data_i[0] = 0x0000FF00;
        dut->tag_hit_i = 1;
        dut->victim_i = 0;
        dut->data_rsp_zero_i = 0;
        dut->opcode_i = OP_L2_WRITE_PARTIAL;
        dut->qwen_i = 0xF;
        dut->address_i = 0;
        memset(dut->atomic_operands_i.data(), 0, 64);
        dut->atomic_operands_i[0] = 0x000000AA;
        dut->partial_wr_data_ben_i = 0x01;
        dut->pipe_valid_i = 1;
        sim.tick();              // d0 → d1
        dut->pipe_valid_i = 0;
        sim.tick();              // d1 → d2 (result available)
        sim.check(dut->result_wr_en_o == 1, "partial wr_en");
        sim.check(dut->result_wr_data_o[0] == 0x0000FFAA,
                  "partial: byte0=AA, byte1=FF");
    }

    // ── Test 12: Add with overflow ──────────────────────
    printf("test 12: add with overflow (sc32)\n");
    {
        uint32_t r = run_sc32(sim, AMO_ADD, 0xFFFFFFFF, 1);
        sim.check(r == 0, "add overflow wraps");
    }

    return sim.finish();
}

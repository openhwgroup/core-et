// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_pipe_atomic vs shirecache_pipe_atomic

#include "Vcosim_atomic_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstring>

using DUT = Vcosim_atomic_tb;

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
static constexpr uint8_t AMO_MINPS     = 9;
static constexpr uint8_t AMO_MAXPS     = 10;
static constexpr uint8_t AMO_COMP_EXCH = 11;

// Opcodes
static constexpr uint8_t OP_L2_ATOMIC        = 0x0b;
static constexpr uint8_t OP_L3_ATOMIC        = 0x19;
static constexpr uint8_t OP_SCP_ATOMIC       = 0x20;
static constexpr uint8_t OP_L2_WRITE_PARTIAL = 0x02;

// Pack conf + offset into atomic_operands at bit 256+
static void pack_operands(uint32_t* w, uint8_t amo_op, bool sc_vec, bool size64,
                          const uint32_t* operand_data, unsigned operand_words,
                          uint8_t offset) {
    memset(w, 0, 16 * sizeof(uint32_t));
    for (unsigned i = 0; i < operand_words && i < 8; i++)
        w[i] = operand_data[i];
    uint8_t conf = (amo_op & 0xF) | ((sc_vec ? 1 : 0) << 4) | ((size64 ? 1 : 0) << 5);
    w[8] = conf | (((uint32_t)offset & 0x3F) << 7);
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    sim.compare("wr_en",   (uint8_t)dut->orig_wr_en_o,   (uint8_t)dut->new_wr_en_o);
    if (dut->orig_wr_en_o || dut->new_wr_en_o) {
        sim.compare("wr_addr", dut->orig_wr_addr_o, dut->new_wr_addr_o);
        sim.compare("wr_qwen", dut->orig_wr_qwen_o, dut->new_wr_qwen_o);
        for (int i = 0; i < 16; i++)
            sim.compare("wr_data_w" + std::to_string(i),
                        dut->orig_wr_data_o[i], dut->new_wr_data_o[i]);
    }
    // Combinational data_rsp
    for (int i = 0; i < 16; i++)
        sim.compare("data_rsp_w" + std::to_string(i),
                    dut->orig_data_rsp_o[i], dut->new_data_rsp_o[i]);
}

static void run_one(CosimCtrl<DUT>& sim, uint8_t opcode, uint8_t amo_op,
                    bool sc_vec, bool size64, const uint32_t* rd_data,
                    const uint32_t* operand, unsigned op_words,
                    uint64_t ben, uint8_t offset, uint8_t qwen) {
    auto* dut = sim.dut.get();
    for (int i = 0; i < 16; i++) dut->pipe_data_i[i] = rd_data[i];
    dut->tag_hit_i = 1;
    dut->victim_i = 0;
    dut->pipe_set_i = 0;
    dut->hit_way_i = 0;
    dut->data_rsp_zero_i = 0;
    dut->opcode_i = opcode;
    dut->qwen_i = qwen;
    dut->address_i = offset; // address LSBs = offset
    pack_operands(dut->atomic_operands_i.data(), amo_op, sc_vec, size64,
                  operand, op_words, offset);
    dut->partial_wr_data_ben_i = ben;
    dut->pipe_valid_i = 1;

    // Combinational output compare
    dut->eval();
    compare_outputs(sim);

    sim.tick(); // d0→d1
    compare_outputs(sim);

    dut->pipe_valid_i = 0;
    sim.tick(); // d1→d2
    compare_outputs(sim);

    sim.tick(); // drain
    compare_outputs(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* dut = sim.dut.get();

    dut->pipe_valid_i = 0;
    sim.reset();

    uint32_t rd_data[16] = {0};
    uint32_t operand[8] = {0};

    // ── Phase 1: sc32 operations ────────────────────────
    printf("cosim phase 1: sc32 operations\n");
    uint8_t ops[] = {AMO_SWAP, AMO_ADD, AMO_XOR, AMO_AND, AMO_OR,
                     AMO_MIN, AMO_MAX, AMO_MINU, AMO_MAXU,
                     AMO_MINPS, AMO_MAXPS, AMO_COMP_EXCH};
    for (auto op : ops) {
        for (uint32_t a = 0; a < 4; a++) {
            for (uint32_t b = 0; b < 4; b++) {
                uint32_t vals_a[] = {0, 1, 0x7FFFFFFF, 0x80000000};
                uint32_t vals_b[] = {0, 1, 0xFFFFFFFF, 0x7FC00000}; // includes NaN
                memset(rd_data, 0, sizeof(rd_data));
                rd_data[0] = vals_a[a];
                memset(operand, 0, sizeof(operand));
                operand[0] = vals_b[b];
                run_one(sim, OP_L2_ATOMIC, op, false, false,
                        rd_data, operand, 1, 0, 0, 0xF);
            }
        }
    }

    // ── Phase 2: sc64 operations ────────────────────────
    printf("cosim phase 2: sc64 operations\n");
    for (auto op : ops) {
        memset(rd_data, 0, sizeof(rd_data));
        rd_data[0] = 0x12345678;
        rd_data[1] = 0xDEADBEEF;
        memset(operand, 0, sizeof(operand));
        operand[0] = 0xCAFEBABE;
        operand[1] = 0x00000001;
        run_one(sim, OP_L2_ATOMIC, op, false, true,
                rd_data, operand, 2, 0, 0, 0xF);
    }

    // ── Phase 3: SIMD operations ────────────────────────
    printf("cosim phase 3: SIMD operations\n");
    for (auto op : ops) {
        // Fill all 8 SIMD lanes
        for (int i = 0; i < 8; i++) rd_data[i] = 100 + i;
        for (int i = 0; i < 8; i++) operand[i] = 200 + i;
        run_one(sim, OP_L2_ATOMIC, op, true, false,
                rd_data, operand, 8, 0, 0, 0xF);
    }

    // ── Phase 4: Partial write ──────────────────────────
    printf("cosim phase 4: partial write\n");
    for (int pat = 0; pat < 8; pat++) {
        memset(rd_data, 0xAA, sizeof(rd_data));
        uint32_t wr[16] = {0};
        memset(wr, 0x55, sizeof(wr));
        uint64_t ben = ((uint64_t)1 << pat) | ((uint64_t)1 << (pat + 8));
        run_one(sim, OP_L2_WRITE_PARTIAL, 0, false, false,
                rd_data, wr, 16, ben, 0, 0xF);
    }

    // ── Phase 5: Different address offsets ──────────────
    printf("cosim phase 5: address offsets\n");
    for (uint8_t off = 0; off < 64; off += 4) {
        memset(rd_data, 0, sizeof(rd_data));
        for (int i = 0; i < 16; i++) {
            rd_data[i] = static_cast<uint32_t>(i) * 0x11111111u;
        }
        memset(operand, 0, sizeof(operand));
        operand[0] = 0xDEADBEEF;
        run_one(sim, OP_L2_ATOMIC, AMO_SWAP, false, false,
                rd_data, operand, 1, 0, off, 0xF);
    }

    return sim.finish();
}

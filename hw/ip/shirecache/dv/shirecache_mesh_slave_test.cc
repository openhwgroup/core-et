// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_mesh_slave.
// NumBanks=4, NumPorts=1.  Same clock for both domains.

#include "Vshirecache_mesh_slave_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vshirecache_mesh_slave_tb;

// Opcode values from etlink_pkg
static constexpr uint8_t ReqRead  = 1;
static constexpr uint8_t ReqWrite = 0;

// L3SwizzleBitSelSize = 4, each field 4 bits.
// Default swizzle: bank from addr bits [7:6] (rel_addr[1:0])
// bank_sel_b0 = 0, bank_sel_b1 = 1, bank_sel_b2 = 2
// Everything else = 0.
// Packed as l3_swizzle_ctl_t (MSB first):
//   all_shire_aliasing(1), two_shire_aliasing_use_shire_lsb(1),
//   sub_bank_sel_b2(4), sub_bank_sel_b1(4), sub_bank_sel_b0(4),
//   bank_sel_b2(4), bank_sel_b1(4), bank_sel_b0(4),
//   shire_sel_b5..b0 (6*4=24)
// Total = 1 + 1 + 12*4 = 50 bits.
// bank_sel_b0=0, bank_sel_b1=1, bank_sel_b2=2, rest=0
// Packed MSB→LSB: [49]=all_shire=0, [48]=two_shire=0,
//   sub_bank_sel_b2..b0 [47:36]=0,
//   bank_sel_b2[35:32]=2, bank_sel_b1[31:28]=1, bank_sel_b0[27:24]=0,
//   shire_sel_b5..b0 [23:0]=0
static uint64_t default_swizzle() {
    uint64_t v = 0;
    // bank_sel_b2 = 2 at bits [35:32]
    v |= (uint64_t)2 << 32;
    // bank_sel_b1 = 1 at bits [31:28]
    v |= (uint64_t)1 << 28;
    // bank_sel_b0 = 0 at bits [27:24]
    return v;
}

static void clear_inputs(DUT* d) {
    d->axi_ar_valid_i = 0;
    d->axi_ar_id_i    = 0;
    d->axi_ar_addr_i  = 0;
    d->axi_ar_size_i  = 6;  // 64B line
    d->axi_ar_qos_i   = 0;

    d->axi_aw_valid_i = 0;
    d->axi_aw_id_i    = 0;
    d->axi_aw_addr_i  = 0;
    d->axi_aw_size_i  = 6;
    d->axi_aw_qos_i   = 0;
    d->axi_aw_user_i  = 0;

    d->axi_w_valid_i  = 0;
    d->axi_w_strb_i   = 0;

    d->axi_r_ready_i  = 1;
    d->axi_b_ready_i  = 1;

    d->bank_req_ready_i = 0xF;

    d->bank_rsp_valid_i = 0;
    d->b0_rsp_id_i      = 0;
    d->b0_rsp_source_i  = 0;
    d->b0_rsp_port_id_i = 0;
    d->b0_rsp_err_i     = 0;
    d->b0_rsp_wdata_i   = 0;

    d->swizzle_ctl_i    = default_swizzle();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();
    sim.max_time = 500000;
    clear_inputs(dut);
    sim.reset(10);

    // ── Phase 1: AXI AR request → bank req ──────────────────
    printf("phase 1: AXI AR read request\n");
    clear_inputs(dut);

    // Build AR with: id[18:9]=source=0x15, id[8:0]=master_id=0x42
    // Address that maps to bank 0: addr[7:6]=0 → bank_id=0
    uint32_t source_val = 0x15;
    uint32_t master_id  = 0x42;
    uint32_t slave_id   = (source_val << 9) | master_id;
    uint64_t addr_bank0 = 0x0000001000ULL;  // bits[7:6]=0 → bank 0

    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = slave_id;
    dut->axi_ar_addr_i  = addr_bank0;
    dut->axi_ar_size_i  = 6;
    dut->axi_ar_qos_i   = 3;

    // Hold bank_req_ready low so we can observe the valid
    dut->bank_req_ready_i = 0;

    // Push AR and wait for it to propagate through CDC + internal FIFOs
    bool b0_valid = false;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        if (dut->bank_req_valid_o & 1) { b0_valid = true; break; }
    }
    dut->axi_ar_valid_i = 0;

    sim.check(b0_valid, "bank0 req valid after AR");
    if (b0_valid) {
        sim.check(dut->b0_req_opcode_o == ReqRead, "AR opcode is ReqRead");
        sim.check(dut->b0_req_id_o == (master_id & 0x1FF),
                  "AR req id matches master_id LSBs");
        sim.check(dut->b0_req_source_o == (source_val & 0x3FF),
                  "AR req source matches slave_id MSBs");
        sim.check(dut->b0_req_wdata_o == 0, "AR req wdata=0 (read)");
    }

    // Drain by accepting the request
    dut->bank_req_ready_i = 0xF;
    for (int i = 0; i < 10; i++) sim.tick();

    // ── Phase 2: AXI AW+W write request → bank req ─────────
    printf("phase 2: AXI AW+W write request\n");
    clear_inputs(dut);

    uint32_t aw_opcode = ReqWrite;  // ReqWrite=0
    dut->bank_req_ready_i = 0;  // hold so we can observe
    dut->axi_aw_valid_i = 1;
    dut->axi_aw_id_i    = slave_id;
    dut->axi_aw_addr_i  = addr_bank0;
    dut->axi_aw_size_i  = 6;
    dut->axi_aw_qos_i   = 2;
    dut->axi_aw_user_i  = aw_opcode;

    dut->axi_w_valid_i  = 1;
    // strb: first 16 bytes partially set → qwen[0]=1, ben_en=1
    dut->axi_w_strb_i   = 0x00FF;  // low 8 bytes of qw0

    bool aw_b0_valid = false;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        if (dut->bank_req_valid_o & 1) { aw_b0_valid = true; break; }
    }
    dut->axi_aw_valid_i = 0;
    dut->axi_w_valid_i  = 0;

    sim.check(aw_b0_valid, "bank0 req valid after AW+W");
    if (aw_b0_valid) {
        sim.check(dut->b0_req_opcode_o == ReqWrite, "AW opcode from USER");
        sim.check(dut->b0_req_wdata_o == 1, "AW req wdata=1 (write)");
        sim.check((dut->b0_req_qwen_o & 0x1) == 1, "qwen[0]=1 (some strb in qw0)");
        sim.check(dut->b0_req_ben_en_o == 1,
                  "ben_en=1 (partial strb in qw0: 8/16 bytes)");
    }

    for (int i = 0; i < 10; i++) sim.tick();

    // ── Phase 3: R response (wdata=1) ───────────────────────
    printf("phase 3: R response (wdata=1)\n");
    clear_inputs(dut);
    dut->axi_r_ready_i = 0;  // hold R so we can observe

    dut->bank_rsp_valid_i = 1;  // bank 0
    dut->b0_rsp_id_i      = 0x23;
    dut->b0_rsp_source_i  = 0x07;
    dut->b0_rsp_port_id_i = 0;
    dut->b0_rsp_err_i     = 0;
    dut->b0_rsp_wdata_i   = 1;  // R response

    // Wait for response to propagate through CDC FIFOs
    bool r_valid = false;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        if (dut->axi_r_valid_o) { r_valid = true; break; }
    }
    dut->bank_rsp_valid_i = 0;

    sim.check(r_valid, "AXI R valid after rsp push");
    if (dut->axi_r_valid_o) {
        // R.ID = {source, id} = {0x07, 0x23} → {10'h07, 9'h23}
        uint32_t expected_r_id = (0x07 << 9) | 0x23;
        sim.check(dut->axi_r_id_o == expected_r_id, "R.ID = {source, id}");
        sim.check(dut->axi_r_resp_o == 0, "R.RESP = OKAY (no err)");
        sim.check(dut->axi_r_last_o == 1, "R.LAST = 1");
    }

    for (int i = 0; i < 10; i++) sim.tick();

    // ── Phase 4: B response (wdata=0) ───────────────────────
    printf("phase 4: B response (wdata=0)\n");
    clear_inputs(dut);
    dut->axi_b_ready_i = 0;  // hold B so we can observe

    dut->bank_rsp_valid_i = 1;  // bank 0
    dut->b0_rsp_id_i      = 0x44;
    dut->b0_rsp_source_i  = 0x0A;
    dut->b0_rsp_port_id_i = 0;
    dut->b0_rsp_err_i     = 1;  // error
    dut->b0_rsp_wdata_i   = 0;  // B response

    bool b_valid = false;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        if (dut->axi_b_valid_o) { b_valid = true; break; }
    }
    dut->bank_rsp_valid_i = 0;

    sim.check(b_valid, "AXI B valid after rsp push");
    if (dut->axi_b_valid_o) {
        uint32_t expected_b_id = (0x0A << 9) | 0x44;
        sim.check(dut->axi_b_id_o == expected_b_id, "B.ID = {source, id}");
        sim.check(dut->axi_b_resp_o == 2, "B.RESP = SLVERR (err=1)");
    }

    for (int i = 0; i < 10; i++) sim.tick();

    // ── Phase 5: Backpressure ───────────────────────────────
    printf("phase 5: backpressure\n");
    clear_inputs(dut);

    // Fill the request path by blocking bank req_ready
    dut->bank_req_ready_i = 0;  // no bank accepts

    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = 0x100;
    dut->axi_ar_addr_i  = addr_bank0;

    for (int i = 0; i < 40; i++) sim.tick();

    // After enough ticks with blocked bank, the async FIFO should fill
    // and AR ready should eventually drop
    bool ar_was_blocked = false;
    for (int i = 0; i < 60; i++) {
        sim.tick();
        if (dut->axi_ar_ready_o == 0) {
            ar_was_blocked = true;
            break;
        }
    }
    sim.check(ar_was_blocked, "AR ready drops under backpressure");

    // Release backpressure
    dut->bank_req_ready_i = 0xF;
    dut->axi_ar_valid_i = 0;
    for (int i = 0; i < 30; i++) sim.tick();

    // ── Phase 6: mesh_slave_bank_clk_en ─────────────────────
    printf("phase 6: bank_clk_en\n");
    clear_inputs(dut);

    // Ensure clk_en is low when idle
    for (int i = 0; i < 10; i++) sim.tick();
    sim.check(dut->bank_clk_en_o == 0, "clk_en=0 when idle");

    // Send a request that targets bank 0 and check clk_en goes high
    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = 0x55;
    dut->axi_ar_addr_i  = addr_bank0;  // bank 0

    // Wait for request to propagate into the bank req path
    bool clk_en_seen = false;
    for (int i = 0; i < 40; i++) {
        sim.tick();
        if (dut->bank_clk_en_o & 1) {
            clk_en_seen = true;
            break;
        }
    }
    sim.check(clk_en_seen, "clk_en[0] goes high when request arrives");

    dut->axi_ar_valid_i = 0;
    for (int i = 0; i < 20; i++) sim.tick();

    // ── Phase 7: Bank routing ───────────────────────────────
    printf("phase 7: bank routing via swizzle\n");
    clear_inputs(dut);
    dut->bank_req_ready_i = 0;  // hold to observe

    // Address targeting bank 1: addr[7:6] = 01 → bits 7:6 = 0x40
    uint64_t addr_bank1 = 0x40;  // rel_addr bit 0 = 1 → bank_id[0]=1 → bank 1

    dut->axi_ar_valid_i = 1;
    dut->axi_ar_id_i    = 0x77;
    dut->axi_ar_addr_i  = addr_bank1;

    bool bank1_valid = false;
    for (int i = 0; i < 30; i++) {
        sim.tick();
        if (dut->bank_req_valid_o & 2) { bank1_valid = true; break; }
    }
    dut->axi_ar_valid_i = 0;
    sim.check(bank1_valid, "request routed to bank 1 via swizzle");

    for (int i = 0; i < 10; i++) sim.tick();

    return sim.finish();
}

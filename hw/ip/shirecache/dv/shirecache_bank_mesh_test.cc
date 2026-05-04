// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_mesh — both ToL3 and ToSys configurations.

#include "Vshirecache_bank_mesh_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vshirecache_bank_mesh_tb;

// Opcodes
static constexpr uint8_t MeshRead       = 0x30;
static constexpr uint8_t MeshWrite      = 0x31;
static constexpr uint8_t MeshFlush      = 0x32;
static constexpr uint8_t MeshFlushToMem = 0x33;
static constexpr uint8_t MeshEvict      = 0x34;
static constexpr uint8_t MeshEvictToMem = 0x35;
static constexpr uint8_t MeshPrefetch   = 0x36;
static constexpr uint8_t MeshVictim     = 0x37;
static constexpr uint8_t MeshAtomicRsp  = 0x38;
static constexpr uint8_t EtReqRead      = 1;
static constexpr uint8_t EtReqWrite     = 0;
static constexpr uint8_t EtReqFlush     = 8;
static constexpr uint8_t EtReqFlushToMem= 9;
static constexpr uint8_t EtReqEvict     = 10;
static constexpr uint8_t EtReqEvictToMem= 11;
static constexpr uint8_t EtReqPrefetch  = 15;
static constexpr uint8_t EtReqAtomicRsp = 16;
static constexpr uint8_t OrigRead       = 0x00;
static constexpr uint8_t OrigScpRead    = 0x07;
static constexpr uint8_t OrigCopScpFill = 0x18;
static constexpr uint8_t ReqqSrcL2      = 0;
static constexpr uint8_t ReqqSrcL3      = 1;
static constexpr uint8_t AddressL2      = 0;
static constexpr uint8_t SizeLine       = 6;

#define CHK(sim, msg, val, exp) do { \
    auto _v = (val); auto _e = (exp); \
    (sim).check(_v == _e, std::string(msg) + \
        (_v != _e ? " (got=" + std::to_string(_v) + " exp=" + std::to_string(_e) + ")" : "")); \
} while(0)

static void clear(DUT* d) {
    d->mesh_master_bank_clk_en_i = 1;
    d->my_bank_id_i = 0;
    d->reqq_valid_i = 0; d->dataq_valid_i = 0; d->pipe_valid_i = 0;
    d->mesh_req_ready_i = 1;
    d->mesh_rsp_valid_i = 0; d->mesh_rsp_id_i = 0; d->mesh_rsp_err_i = 0;
    d->rsp_ready_reqq_i = 1; d->rsp_ready_dataq_i = 1;
    d->rsp_ready_rspmux_l2_i = 1; d->rsp_ready_l3_slave_i = 1;
    d->esr_l2_bypass_i = 0; d->esr_l3_bypass_i = 0; d->esr_remote_l3_enable_i = 0;
    d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    d->pipe_mod_disable_bypass_i = 0; d->pipe_mod_redirect_i = 0;
    d->pipe_mod_dataq_id_i = 0; d->pipe_reqq_id_i = 0;
    d->reqq_reqq_id_i = 0; d->reqq_dataq_id_i = 0;
    d->reqq_with_data_i = 0; d->reqq_rd_ben_i = 0;
    d->dataq_reqq_id_i = 0; d->dataq_dataq_id_i = 0; d->dataq_with_data_i = 0;
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_reqq_source0_i = ReqqSrcL2; d->rs_address_space0_i = AddressL2;
    d->rs_size0_i = SizeLine; d->rs_qos0_i = 0; d->rs_qwen0_i = 0xF;
    d->rs_address0_i = 0; d->rs_dataq_id0_i = 0;
    d->rs_idx1_i = 1; d->rs_opcode1_i = MeshRead; d->rs_orig_opcode1_i = OrigRead;
    d->rs_reqq_source1_i = ReqqSrcL2; d->rs_address_space1_i = AddressL2;
    d->rs_size1_i = SizeLine; d->rs_qos1_i = 0; d->rs_qwen1_i = 0xF;
    d->rs_address1_i = 0; d->rs_dataq_id1_i = 0;
}

// Send a reqq request and wait for it to appear at mesh output.
// Returns true if req_valid appeared within max_wait ticks.
static bool send_reqq_and_wait(SimCtrl<DUT>& sim, DUT* d, int max_wait = 6) {
    d->reqq_valid_i = 1;
    sim.tick();
    d->reqq_valid_i = 0;
    for (int i = 0; i < max_wait; i++) {
        sim.tick();
        if (d->l3_req_valid_o) return true;
    }
    return false;
}

// Push a response into the rsp FIFO and wait for valid to reqq.
// Note: if all destinations are ready, the response may be consumed in a single tick.
static bool push_rsp_and_wait(SimCtrl<DUT>& sim, DUT* d, int max_wait = 6) {
    d->mesh_rsp_valid_i = 1;
    sim.tick();
    d->mesh_rsp_valid_i = 0;
    // Check immediately after push — response may be consumed in one tick
    if (d->l3_rsp_valid_reqq_o) return true;
    for (int i = 0; i < max_wait; i++) {
        sim.tick();
        if (d->l3_rsp_valid_reqq_o) return true;
    }
    return false;
}

// Drain all pending transactions
static void drain(SimCtrl<DUT>& sim, int n = 10) {
    for (int i = 0; i < n; i++) sim.tick();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset(5);
    drain(sim, 3);

    // Phase 1: Basic reqq request (MeshRead)
    printf("Phase 1: basic reqq MeshRead\n");
    clear(d);
    d->rs_idx0_i = 5; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->reqq_reqq_id_i = 5;
    CHK(sim, "req appears", (int)send_reqq_and_wait(sim, d), 1);
    CHK(sim, "opcode=Read", (int)d->l3_req_opcode_o, (int)EtReqRead);
    CHK(sim, "id=5", (int)d->l3_req_id_o, 5);
    CHK(sim, "wdata=0", (int)d->l3_req_wdata_o, 0);
    CHK(sim, "size=line", (int)d->l3_req_size_o, (int)SizeLine);
    drain(sim);

    // Phase 2: Opcode mapping
    printf("Phase 2: opcode mapping\n");
    struct { uint8_t mesh_op; uint8_t et_op; const char* name; } opcodes[] = {
        {MeshRead,       EtReqRead,       "Read"},
        {MeshWrite,      EtReqWrite,      "Write"},
        {MeshFlush,      EtReqFlush,      "Flush"},
        {MeshFlushToMem, EtReqFlushToMem, "FlushToMem"},
        {MeshEvict,      EtReqEvict,      "Evict"},
        {MeshEvictToMem, EtReqEvictToMem, "EvictToMem"},
        {MeshPrefetch,   EtReqPrefetch,   "Prefetch"},
        {MeshVictim,     EtReqWrite,      "Victim->Write"},
    };
    for (auto& op : opcodes) {
        clear(d);
        d->rs_idx0_i = 0; d->rs_opcode0_i = op.mesh_op; d->rs_orig_opcode0_i = OrigRead;
        d->reqq_reqq_id_i = 0;
        send_reqq_and_wait(sim, d);
        char msg[64]; snprintf(msg, sizeof(msg), "opcode %s", op.name);
        CHK(sim, msg, (int)d->l3_req_opcode_o, (int)op.et_op);
        drain(sim);
    }

    // Phase 3: Request priority
    printf("Phase 3: request priority\n");
    clear(d);
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshRead;
    d->rs_idx1_i = 1; d->rs_opcode1_i = MeshWrite;
    d->reqq_valid_i = 1; d->reqq_reqq_id_i = 0;
    d->dataq_valid_i = 1; d->dataq_reqq_id_i = 1; d->dataq_with_data_i = 1;
    d->pipe_valid_i = 1; d->pipe_reqq_id_i = 0;
    d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    sim.tick();
    CHK(sim, "reqq wins", (int)d->l3_reqq_ready_o, 1);
    CHK(sim, "dataq no grant", (int)d->l3_dataq_ready_o, 0);
    CHK(sim, "victim no grant", (int)d->l3_bypassed_victim_o, 0);
    d->reqq_valid_i = 0;
    sim.tick();
    CHK(sim, "dataq wins next", (int)d->l3_dataq_ready_o, 1);
    d->dataq_valid_i = 0;
    // Victim may need to wait for pipeline not stalled
    for (int i = 0; i < 4; i++) {
        sim.tick();
        if (d->l3_bypassed_victim_o) break;
    }
    CHK(sim, "victim gets through", (int)d->l3_bypassed_victim_o, 1);
    d->pipe_valid_i = 0;
    drain(sim);

    // Phase 4: QOS
    printf("Phase 4: QOS\n");
    clear(d);
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshRead; d->rs_qos0_i = 0;
    d->reqq_reqq_id_i = 0;
    send_reqq_and_wait(sim, d);
    CHK(sim, "qos low", (int)d->l3_req_qos_o, 0);
    drain(sim);
    d->rs_qos0_i = 1;
    d->reqq_reqq_id_i = 0;
    send_reqq_and_wait(sim, d);
    CHK(sim, "qos high", (int)d->l3_req_qos_o, 1);
    drain(sim);

    // Phase 5: Source
    printf("Phase 5: source=bank_id\n");
    clear(d);
    d->my_bank_id_i = 2;
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshRead;
    d->reqq_reqq_id_i = 0;
    send_reqq_and_wait(sim, d);
    CHK(sim, "source=2", (int)d->l3_req_source_o, 2);
    drain(sim);

    // Phase 6: Response — reqq only (Write opcode, not Read → no dataq)
    printf("Phase 6: rsp reqq only\n");
    clear(d);
    d->rs_idx0_i = 3; d->rs_opcode0_i = MeshWrite; d->rs_orig_opcode0_i = OrigRead;
    d->mesh_rsp_id_i = 3;
    CHK(sim, "rsp appears", (int)push_rsp_and_wait(sim, d), 1);
    CHK(sim, "reqq valid", (int)d->l3_rsp_valid_reqq_o, 1);
    CHK(sim, "dataq=0", (int)d->l3_rsp_valid_dataq_o, 0);
    CHK(sim, "rspmux=0", (int)d->l3_rsp_valid_rspmux_l2_o, 0);
    CHK(sim, "l3slv=0", (int)d->l3_rsp_valid_l3_slave_o, 0);
    drain(sim);

    // Phase 7: Response — reqq + dataq (MeshRead, OrigRead)
    // Hold dataq ready low so response stays until we can observe both
    printf("Phase 7: rsp reqq+dataq\n");
    clear(d);
    d->rsp_ready_dataq_i = 0; // hold dataq
    d->rs_idx0_i = 7; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_reqq_source0_i = ReqqSrcL2; d->rs_address_space0_i = AddressL2;
    d->mesh_rsp_id_i = 7;
    push_rsp_and_wait(sim, d);
    // After reqq sent, dataq should become valid
    for (int i = 0; i < 4; i++) {
        sim.tick();
        if (d->l3_rsp_valid_dataq_o) break;
    }
    CHK(sim, "dataq valid", (int)d->l3_rsp_valid_dataq_o, 1);
    d->rsp_ready_dataq_i = 1;
    drain(sim);

    // Phase 8: rsp reqq + rspmux_l2
    printf("Phase 8: rsp reqq+rspmux_l2\n");
    clear(d);
    d->rsp_ready_rspmux_l2_i = 0; // hold rspmux
    d->rs_idx0_i = 10; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_reqq_source0_i = ReqqSrcL2;
    d->mesh_rsp_id_i = 10;
    push_rsp_and_wait(sim, d);
    for (int i = 0; i < 4; i++) { sim.tick(); if (d->l3_rsp_valid_rspmux_l2_o) break; }
    CHK(sim, "rspmux_l2 valid", (int)d->l3_rsp_valid_rspmux_l2_o, 1);
    d->rsp_ready_rspmux_l2_i = 1;
    drain(sim);

    // Phase 9: rsp reqq + l3_slave
    printf("Phase 9: rsp reqq+l3_slave\n");
    clear(d);
    d->rsp_ready_l3_slave_i = 0; // hold l3_slave
    d->rs_idx0_i = 11; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_reqq_source0_i = ReqqSrcL3;
    d->mesh_rsp_id_i = 11;
    push_rsp_and_wait(sim, d);
    for (int i = 0; i < 4; i++) { sim.tick(); if (d->l3_rsp_valid_l3_slave_o) break; }
    CHK(sim, "l3_slave valid", (int)d->l3_rsp_valid_l3_slave_o, 1);
    d->rsp_ready_l3_slave_i = 1;
    drain(sim);

    // Phase 10: Error delay
    printf("Phase 10: error delay\n");
    clear(d);
    d->rs_idx0_i = 12; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_reqq_source0_i = ReqqSrcL2;
    d->rsp_ready_reqq_i = 0; // hold reqq
    d->mesh_rsp_id_i = 12; d->mesh_rsp_err_i = 1;
    push_rsp_and_wait(sim, d);
    // reqq valid but not ready → rspmux should be delayed
    CHK(sim, "rspmux delayed (err)", (int)d->l3_rsp_valid_rspmux_l2_o, 0);
    d->rsp_ready_reqq_i = 1;
    for (int i = 0; i < 4; i++) { sim.tick(); if (d->l3_rsp_valid_rspmux_l2_o) break; }
    CHK(sim, "rspmux after reqq", (int)d->l3_rsp_valid_rspmux_l2_o, 1);
    drain(sim);

    // Phase 11: Victim bypass
    printf("Phase 11: victim bypass\n");
    clear(d);
    d->pipe_valid_i = 1; d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    sim.tick();
    CHK(sim, "l3 bypass", (int)d->l3_bypassed_victim_o, 1);
    d->pipe_mod_which_mesh_i = 1; // ToSys
    sim.tick();
    CHK(sim, "l3 no bypass (ToSys)", (int)d->l3_bypassed_victim_o, 0);
    CHK(sim, "sys bypass (ToSys)", (int)d->sys_bypassed_victim_o, 1);
    d->pipe_valid_i = 0;
    drain(sim);

    // Phase 12: Bypass disabled
    printf("Phase 12: bypass disabled\n");
    clear(d);
    d->pipe_valid_i = 1; d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    d->pipe_mod_disable_bypass_i = 1;
    sim.tick();
    CHK(sim, "bypass disabled", (int)d->l3_bypassed_victim_o, 0);
    d->pipe_valid_i = 0;
    drain(sim, 3);

    // Phase 13: Non-victim redirect
    printf("Phase 13: redirect\n");
    clear(d);
    d->pipe_valid_i = 1; d->pipe_rsp_type_i = 0; // NormalRsp
    d->pipe_mod_which_mesh_i = 0; d->pipe_mod_redirect_i = 1;
    sim.tick();
    CHK(sim, "redirect", (int)d->l3_bypassed_victim_o, 1);
    d->pipe_valid_i = 0;
    drain(sim);

    // Phase 14: CopScpFill wdata forced off
    printf("Phase 14: CopScpFill\n");
    clear(d);
    d->rs_idx0_i = 20; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigCopScpFill;
    d->reqq_reqq_id_i = 20; d->reqq_with_data_i = 1;
    send_reqq_and_wait(sim, d);
    CHK(sim, "wdata=0", (int)d->l3_req_wdata_o, 0);
    drain(sim);

    // Phase 15: ben_en
    printf("Phase 15: ben_en\n");
    clear(d);
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshWrite; d->rs_orig_opcode0_i = OrigRead;
    d->reqq_reqq_id_i = 0; d->reqq_with_data_i = 1; d->reqq_rd_ben_i = 1;
    send_reqq_and_wait(sim, d);
    CHK(sim, "ben_en=1", (int)d->l3_req_ben_en_o, 1);
    drain(sim);

    // Phase 16: L2 bypass skips dataq
    printf("Phase 16: L2 bypass\n");
    clear(d);
    d->esr_l2_bypass_i = 1;
    d->rs_idx0_i = 15; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigRead;
    d->rs_address_space0_i = AddressL2;
    d->mesh_rsp_id_i = 15;
    push_rsp_and_wait(sim, d);
    sim.tick(); sim.tick();
    CHK(sim, "dataq skipped", (int)d->l3_rsp_valid_dataq_o, 0);
    drain(sim);

    // Phase 17: ScpRead → rspmux
    printf("Phase 17: ScpRead\n");
    clear(d);
    d->rsp_ready_rspmux_l2_i = 0; // hold rspmux so we can observe
    d->rs_idx0_i = 16; d->rs_opcode0_i = MeshRead; d->rs_orig_opcode0_i = OrigScpRead;
    d->rs_reqq_source0_i = ReqqSrcL2;
    d->mesh_rsp_id_i = 16;
    push_rsp_and_wait(sim, d);
    for (int i = 0; i < 4; i++) { sim.tick(); if (d->l3_rsp_valid_rspmux_l2_o) break; }
    CHK(sim, "dataq=0", (int)d->l3_rsp_valid_dataq_o, 0);
    CHK(sim, "rspmux=1", (int)d->l3_rsp_valid_rspmux_l2_o, 1);
    d->rsp_ready_rspmux_l2_i = 1;
    drain(sim);

    // Phase 18: Backpressure
    printf("Phase 18: backpressure\n");
    clear(d);
    d->mesh_req_ready_i = 0;
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshRead;
    for (int i = 0; i < 5; i++) {
        d->reqq_valid_i = 1; d->reqq_reqq_id_i = i;
        sim.tick();
    }
    CHK(sim, "stalled", (int)d->l3_reqq_ready_o, 0);
    d->mesh_req_ready_i = 1; d->reqq_valid_i = 0;
    drain(sim);

    // Phase 19: ToSys MeshAtomicRsp
    printf("Phase 19: ToSys AtomicRsp\n");
    clear(d);
    d->rs_idx0_i = 0; d->rs_opcode0_i = MeshAtomicRsp; d->rs_size0_i = 3;
    d->reqq_reqq_id_i = 0; d->reqq_with_data_i = 1;
    d->reqq_valid_i = 1;
    sim.tick(); d->reqq_valid_i = 0;
    for (int i = 0; i < 6; i++) { sim.tick(); if (d->sys_req_valid_o) break; }
    CHK(sim, "sys req valid", (int)d->sys_req_valid_o, 1);
    CHK(sim, "opcode=AtomicRsp", (int)d->sys_req_opcode_o, (int)EtReqAtomicRsp);
    CHK(sim, "qos=2", (int)d->sys_req_qos_o, 2);
    CHK(sim, "size=3", (int)d->sys_req_size_o, 3);
    drain(sim);

    return sim.finish();
}

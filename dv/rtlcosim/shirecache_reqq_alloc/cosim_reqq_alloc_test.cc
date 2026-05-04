// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_reqq_alloc vs shirecache_bank_reqq_alloc
// Thorough test covering all L2 opcode mappings, error cases, idx cop,
// paired allocation, dependency computation, and stress.

#include "Vcosim_reqq_alloc_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using DUT = Vcosim_reqq_alloc_tb;

// ═══════════════════════════════════════════════════════
// ET-Link request opcodes (etlink_pkg::req_opcode_e)
// ═══════════════════════════════════════════════════════
static constexpr int REQ_WRITE        = 0;
static constexpr int REQ_READ         = 1;
static constexpr int REQ_WRITE_AROUND = 2;
static constexpr int REQ_READ_COOP    = 3;
static constexpr int REQ_MSG_SEND     = 4;
static constexpr int REQ_ATOMIC       = 5;
static constexpr int REQ_FLUSH        = 8;
static constexpr int REQ_FLUSH_TO_MEM = 9;
static constexpr int REQ_EVICT        = 10;
static constexpr int REQ_EVICT_TO_MEM = 11;
static constexpr int REQ_LOCK         = 12;
static constexpr int REQ_UNLOCK       = 13;
static constexpr int REQ_SCP_FILL     = 14;
static constexpr int REQ_PREFETCH     = 15;
static constexpr int REQ_ATOMIC_RSP   = 16;

// ═══════════════════════════════════════════════════════
// reqq_orig_opcode_e
// ═══════════════════════════════════════════════════════
static constexpr int ORIG_READ             = 0x00;
static constexpr int ORIG_WRITE            = 0x01;
static constexpr int ORIG_WRITE_AROUND     = 0x02;
static constexpr int ORIG_WRITE_PARTIAL    = 0x03;
static constexpr int ORIG_READ_COOP        = 0x04;
static constexpr int ORIG_MSG_SEND_DATA    = 0x05;
static constexpr int ORIG_ATOMIC           = 0x06;
static constexpr int ORIG_SCP_READ         = 0x07;
static constexpr int ORIG_SCP_WRITE        = 0x08;
static constexpr int ORIG_SCP_WRITE_PART   = 0x09;
static constexpr int ORIG_SCP_READ_COOP    = 0x0a;
static constexpr int ORIG_IDX              = 0x0b;
static constexpr int ORIG_SYNC             = 0x0c;
static constexpr int ORIG_READ_FORWARD     = 0x0d;
static constexpr int ORIG_WRITE_FORWARD    = 0x0e;
static constexpr int ORIG_WRITE_FWD_PART   = 0x0f;
static constexpr int ORIG_COP_LOCK         = 0x10;
static constexpr int ORIG_COP_UNLOCK       = 0x11;
static constexpr int ORIG_COP_UNLOCK_INV   = 0x12;
static constexpr int ORIG_COP_FLUSH        = 0x13;
static constexpr int ORIG_COP_EVICT        = 0x14;
static constexpr int ORIG_COP_FLUSH_WDATA  = 0x15;
static constexpr int ORIG_COP_EVICT_WDATA  = 0x16;
static constexpr int ORIG_COP_PREFETCH     = 0x17;
static constexpr int ORIG_COP_SCP_FILL     = 0x18;
static constexpr int ORIG_ATOMIC2          = 0x19;
static constexpr int ORIG_WRITE_AROUND2    = 0x1a;
static constexpr int ORIG_WRITE_PARTIAL2   = 0x1b;
static constexpr int ORIG_ERR              = 0x1c;

// address_space_e
static constexpr int ADDR_L2   = 0;
static constexpr int ADDR_L3   = 1;
static constexpr int ADDR_SCP  = 2;
static constexpr int ADDR_IDX  = 3;
static constexpr int ADDR_MSG  = 4;

// reqq_source_e
static constexpr int SRC_L2    = 0;
static constexpr int SRC_L3    = 1;
static constexpr int SRC_FSM   = 2;

// reqq_opcode_e for idx cop
static constexpr int OP_IDX_READ      = 0x26;
static constexpr int OP_IDX_WRITE     = 0x27;
static constexpr int OP_IDX_ALL_INV   = 0x28;
static constexpr int OP_IDX_L2_INV    = 0x29;
static constexpr int OP_IDX_L2_FLUSH  = 0x2a;
static constexpr int OP_IDX_L2_EVICT  = 0x2b;
static constexpr int OP_IDX_L3_INV    = 0x2c;
static constexpr int OP_IDX_L3_FLUSH  = 0x2d;
static constexpr int OP_IDX_L3_EVICT  = 0x2e;
static constexpr int OP_SYNC          = 0x2f;
static constexpr int OP_L2_EVICT      = 0x09;  // Coalescing buffer evict
static constexpr int OP_SCP_ZERO      = 0x23;
static constexpr int OP_REQQ_INITIAL  = 0x3d;

// err_rsp_e
static constexpr int ERR_NONE                 = 0;
static constexpr int ERR_REMOTE_SCP_DISABLED  = 0x11;
static constexpr int ERR_L2_BYPASS_ATOMIC     = 0x12;
static constexpr int ERR_UNSUPPORTED          = 0x14;
static constexpr int ERR_ILLEGAL_PORT         = 0x15;

// SCP address region constants
// SCP region: PA[39:31] == 9'h001
// SCP shire:  PA[30:23]
static constexpr uint64_t SCP_REGION_BIT = (1ULL << 31);
static constexpr uint64_t SCP_LOCAL_SHIRE = (0xFFULL << 23);  // local_shire_id = all 1s

// ═══════════════════════════════════════════════════════
// Helpers
// ═══════════════════════════════════════════════════════

static void clear_inputs(DUT* d) {
    d->bank_valid_i = 0;
    d->bank_opcode_i = 0;
    d->bank_subopcode_i = 0;
    d->bank_address_i = 0;
    memset(&d->bank_data_i, 0, sizeof(d->bank_data_i));
    d->bank_qwen_i = 0;
    d->bank_size_i = 0;
    d->bank_port_id_i = 0;
    d->bank_source_i = 0;
    d->bank_trans_id_i = 0;
    d->bank_write_is_partial_i = 0;
    d->bank_qos_i = 0;
    memset(&d->bank_ben_i, 0, sizeof(d->bank_ben_i));
    d->bank_id_i = 0;
    d->bank_wdata_i = 0;
    d->dataq_bank_ready_i = 1;
    d->pipe_idx_cop_req_valid_i = 0;
    d->pipe_idx_cop_req_opcode_i = 0;
    d->pipe_idx_cop_req_address_i = 0;
    d->pipe_idx_cop_req_trans_id_i = 0;
    d->pipe_idx_cop_req_sync_flush_victims_i = 0;
    d->pipe_idx_cop_l2_busy_i = 0;
    d->pipe_idx_cop_l3_busy_i = 0;
    d->pipe_idx_cop_aborted_i = 0;
    d->mbist_on_i = 0;
    d->esr_wr_reqq_entries_i = 0;
    d->reqq_entry_valid_i = 0;
    d->reqq_allowed_entries_i = ~0ULL;  // all entries allowed
    d->pipe_tag_rsp_valid_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_victim_address_i = 0;
    d->pipe_tag_rsp_victim_address_space_i = 0;
    d->pipe_tag_rsp_reqq_id_i = 0;
    d->esr_sc_l2_bypass_i = 0;
    d->esr_sc_remote_scp_enable_i = 0;
}

static void compare_all(CosimCtrl<DUT>& sim, const char* label = nullptr) {
    auto* d = sim.dut.get();

    auto tag = [&](const char* name) -> std::string {
        if (label) return std::string(label) + "." + name;
        return name;
    };

    sim.compare(tag("bank_ready").c_str(),       d->orig_bank_ready_o,       d->new_bank_ready_o);
    sim.compare(tag("reqq_full").c_str(),         d->orig_reqq_full_o,        d->new_reqq_full_o);
    sim.compare(tag("alloc_valid_ag").c_str(),    d->orig_alloc_valid_ag_o,   d->new_alloc_valid_ag_o);
    sim.compare(tag("alloc_valid_ad").c_str(),    d->orig_alloc_valid_ad_o,   d->new_alloc_valid_ad_o);
    sim.compare(tag("alloc_1hot_ag").c_str(),     d->orig_alloc_1hot_ag_o,    d->new_alloc_1hot_ag_o);
    sim.compare(tag("alloc_1hot_ad").c_str(),     d->orig_alloc_1hot_ad_o,    d->new_alloc_1hot_ad_o);
    sim.compare(tag("reqq_alloc_id_ag").c_str(),  d->orig_reqq_alloc_id_ag_o, d->new_reqq_alloc_id_ag_o);
    sim.compare(tag("idx_cop_req_ready").c_str(), d->orig_pipe_idx_cop_req_ready_o, d->new_pipe_idx_cop_req_ready_o);
    sim.compare(tag("alloc_match_tag_victim").c_str(), d->orig_alloc_match_tag_victim_ag_o, d->new_alloc_match_tag_victim_ag_o);

    // Only compare alloc_info fields when alloc is valid
    if (d->orig_alloc_valid_ag_o || d->new_alloc_valid_ag_o) {
        sim.compare(tag("orig_opcode").c_str(),       d->orig_alloc_orig_opcode_o,       d->new_alloc_orig_opcode_o);
        sim.compare(tag("opcode").c_str(),            d->orig_alloc_opcode_o,            d->new_alloc_opcode_o);
        sim.compare(tag("address_space").c_str(),     d->orig_alloc_address_space_o,     d->new_alloc_address_space_o);
        sim.compare(tag("err_valid").c_str(),         d->orig_alloc_err_valid_o,         d->new_alloc_err_valid_o);
        sim.compare(tag("err_type").c_str(),          d->orig_alloc_err_type_o,          d->new_alloc_err_type_o);
        sim.compare(tag("dep_valid").c_str(),         d->orig_alloc_dep_valid_o,         d->new_alloc_dep_valid_o);
        sim.compare(tag("dep_id").c_str(),            d->orig_alloc_dep_id_o,            d->new_alloc_dep_id_o);
        sim.compare(tag("reqq_source").c_str(),       d->orig_alloc_reqq_source_o,       d->new_alloc_reqq_source_o);
        sim.compare(tag("address").c_str(),           d->orig_alloc_address_o,           d->new_alloc_address_o);
        sim.compare(tag("qwen").c_str(),              d->orig_alloc_qwen_o,              d->new_alloc_qwen_o);
        sim.compare(tag("size").c_str(),              d->orig_alloc_size_o,              d->new_alloc_size_o);
        sim.compare(tag("dep_non_meshll_head").c_str(), d->orig_alloc_dep_non_meshll_head_o, d->new_alloc_dep_non_meshll_head_o);
        sim.compare(tag("err_imprecise").c_str(),     d->orig_alloc_err_imprecise_o,     d->new_alloc_err_imprecise_o);
        sim.compare(tag("paired_id").c_str(),         d->orig_alloc_paired_id_o,         d->new_alloc_paired_id_o);
        sim.compare(tag("1hot_paired_ag").c_str(),    d->orig_alloc_1hot_paired_ag_o,    d->new_alloc_1hot_paired_ag_o);
        sim.compare(tag("paired_orig_opcode").c_str(), d->orig_alloc_paired_orig_opcode_o, d->new_alloc_paired_orig_opcode_o);
    }
}

// Send a bank request for one cycle, compare, then clear and compare AD.
static void bank_req(CosimCtrl<DUT>& sim, const char* label,
                     int opcode, uint64_t address = 0x1000,
                     int subopcode = 0, int write_is_partial = 0,
                     int port_id = 0, int source = 0,
                     uint32_t data_low = 0, int qwen = 0xF) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->bank_valid_i = 1;
    d->bank_opcode_i = opcode;
    d->bank_subopcode_i = subopcode;
    d->bank_address_i = address;
    d->bank_data_i[0] = data_low;
    d->bank_qwen_i = qwen;
    d->bank_size_i = 6;  // SizeLine
    d->bank_port_id_i = port_id;
    d->bank_source_i = source;
    d->bank_write_is_partial_i = write_is_partial;
    d->reqq_allowed_entries_i = ~0ULL;
    sim.tick();
    compare_all(sim, label);

    // Deassert for AD cycle
    clear_inputs(d);
    d->bank_valid_i = 0;
    d->reqq_allowed_entries_i = ~0ULL;
    sim.tick();
    compare_all(sim, label);
}

// Send an idx cop request for one cycle, compare.
static void idx_req(CosimCtrl<DUT>& sim, const char* label,
                    int opcode, uint64_t address = 0x2000,
                    int sync_flush_victims = 0) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->pipe_idx_cop_req_valid_i = 1;
    d->pipe_idx_cop_req_opcode_i = opcode;
    d->pipe_idx_cop_req_address_i = address;
    d->pipe_idx_cop_l2_busy_i = 1;  // Typically busy when idx cop is active
    d->pipe_idx_cop_req_sync_flush_victims_i = sync_flush_victims;
    d->reqq_allowed_entries_i = ~0ULL;
    sim.tick();
    compare_all(sim, label);

    // Deassert
    clear_inputs(d);
    d->reqq_allowed_entries_i = ~0ULL;
    sim.tick();
    compare_all(sim, label);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    sim.reset();
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->reqq_allowed_entries_i = ~0ULL;

    // A few settle cycles
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim, "settle"); }

    printf("──── L2 Opcode Mapping ────\n");

    // 1. ReqRead -> OrigRead (non-SCP, non-forward)
    bank_req(sim, "01_ReqRead", REQ_READ);

    // 2. ReqRead + subopcode[0]=1 -> OrigReadForward
    bank_req(sim, "02_ReqReadFwd", REQ_READ, 0x1000, /*sub*/1);

    // 3. ReqRead + SCP address -> OrigScpRead
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;
        bank_req(sim, "03_ReqReadScp", REQ_READ, scp_local);
    }

    // 4. ReqReadCoop -> OrigReadCoop
    bank_req(sim, "04_ReqReadCoop", REQ_READ_COOP, 0x1000, 0, 0, 0, 0, 0x10000);

    // 5. ReqReadCoop + SCP -> OrigScpReadCoop
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;
        bank_req(sim, "05_ReqReadCoopScp", REQ_READ_COOP, scp_local, 0, 0, 0, 0, 0x10000);
    }

    // 6. ReqWrite -> OrigWrite
    bank_req(sim, "06_ReqWrite", REQ_WRITE);

    // 7. ReqWrite + write_is_partial -> OrigWritePartial
    bank_req(sim, "07_ReqWritePartial", REQ_WRITE, 0x1000, 0, 1);

    // 8. ReqWrite + subopcode[0]=1 -> OrigWriteForward
    bank_req(sim, "08_ReqWriteFwd", REQ_WRITE, 0x1000, 1);

    // 9. ReqWrite + subopcode[0]=1 + write_is_partial -> OrigWriteForwardPartial
    bank_req(sim, "09_ReqWriteFwdPart", REQ_WRITE, 0x1000, 1, 1);

    // 10. ReqWrite + SCP -> OrigScpWrite
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;
        bank_req(sim, "10_ReqWriteScp", REQ_WRITE, scp_local);
    }

    // 11. ReqWrite + SCP + write_is_partial -> OrigScpWritePartial
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;
        bank_req(sim, "11_ReqWriteScpPart", REQ_WRITE, scp_local, 0, 1);
    }

    // 12. ReqWriteAround -> OrigWriteAround
    bank_req(sim, "12_ReqWriteAround", REQ_WRITE_AROUND);

    // 13. ReqAtomic -> OrigAtomic
    bank_req(sim, "13_ReqAtomic", REQ_ATOMIC);

    // 14. ReqLock -> OrigCopLock
    bank_req(sim, "14_ReqLock", REQ_LOCK);

    // 15. ReqUnlock + data[3]=0 -> OrigCopUnlockInv
    bank_req(sim, "15_ReqUnlockInv", REQ_UNLOCK, 0x1000, 0, 0, 0, 0, 0x0);

    // 16. ReqUnlock + data[3]=1 -> OrigCopUnlock
    bank_req(sim, "16_ReqUnlock", REQ_UNLOCK, 0x1000, 0, 0, 0, 0, 0x8);

    // 17. ReqFlush -> OrigCopFlush
    bank_req(sim, "17_ReqFlush", REQ_FLUSH);

    // 18. ReqEvict -> OrigCopEvict
    bank_req(sim, "18_ReqEvict", REQ_EVICT);

    // 19. ReqPrefetch -> OrigCopPrefetch
    bank_req(sim, "19_ReqPrefetch", REQ_PREFETCH);

    // 20. ReqScpFill -> OrigCopScpFill (local SCP, source address in data)
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;
        // data field: SCP region in data for scpFill_source_is_scp check
        bank_req(sim, "20_ReqScpFill", REQ_SCP_FILL, scp_local, 0, 0, 0, 0,
                 (1 << 31));  // data bit 31 -> SCP region in data
    }

    // 21. ReqMsgSendData -> OrigMsgSendData (port < 5)
    bank_req(sim, "21_ReqMsgSend", REQ_MSG_SEND, 0x10000);  // port_id = 1

    // 22. Unknown opcode -> OrigErr
    bank_req(sim, "22_UnknownOp", REQ_FLUSH_TO_MEM);  // FlushToMem not supported for L2
    bank_req(sim, "22b_UnknownOp2", REQ_EVICT_TO_MEM);
    bank_req(sim, "22c_UnknownOp3", REQ_ATOMIC_RSP);

    printf("──── Paired Entry Allocation ────\n");

    // 23. Atomic -> paired_valid -> Atomic2 allocated
    // When paired is available, alloc_1hot_paired_ag will be set
    bank_req(sim, "23_AtomicPaired", REQ_ATOMIC, 0x3000);

    // 24. WriteAround -> paired_valid -> WriteAround2
    bank_req(sim, "24_WriteAroundPaired", REQ_WRITE_AROUND, 0x3040);

    // 25. WritePartial -> paired_valid -> WritePartial2
    bank_req(sim, "25_WritePartialPaired", REQ_WRITE, 0x3080, 0, 1);

    printf("──── Error Detection ────\n");

    // 27. OrigErr (unknown opcode) -> err_valid
    bank_req(sim, "27_ErrOrigErr", REQ_FLUSH_TO_MEM, 0x4000);

    // 28. SCP remote + remote_scp not enabled -> err_valid
    // Create a remote SCP address: SCP region bit set, shire_id != 0xFF and != shire_config.shire_id(=0)
    {
        uint64_t scp_remote = SCP_REGION_BIT | (0x42ULL << 23) | 0x40;
        // esr_sc_remote_scp_enable = 0 (default)
        bank_req(sim, "28_ErrRemoteScp", REQ_READ, scp_remote);
    }

    // 29. L2 Atomic + L2 bypass -> err_valid
    {
        clear_inputs(d);
        d->bank_valid_i = 1;
        d->bank_opcode_i = REQ_ATOMIC;
        d->bank_address_i = 0x5000;
        d->bank_qwen_i = 0xF;
        d->bank_size_i = 6;
        d->esr_sc_l2_bypass_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "29_ErrL2BypassAtomic");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "29_ErrL2BypassAtomic_AD");
    }

    // 30. MsgSendData to illegal port -> err_valid
    // port_id in address[19:16] >= 5
    bank_req(sim, "30_ErrMsgIllegalPort", REQ_MSG_SEND, 0xF0000);  // port = 15

    printf("──── CacheOp FSM Idx Opcodes ────\n");

    // 31. IdxRead -> OrigIdx
    idx_req(sim, "31_IdxRead", OP_IDX_READ);

    // IdxWrite -> OrigIdx
    idx_req(sim, "31b_IdxWrite", OP_IDX_WRITE);

    // IdxAllInv -> OrigIdx
    idx_req(sim, "31c_IdxAllInv", OP_IDX_ALL_INV);

    // IdxL2Inv -> OrigIdx
    idx_req(sim, "31d_IdxL2Inv", OP_IDX_L2_INV);

    // IdxL2Flush -> OrigIdx
    idx_req(sim, "31e_IdxL2Flush", OP_IDX_L2_FLUSH);

    // IdxL2Evict -> OrigIdx
    idx_req(sim, "31f_IdxL2Evict", OP_IDX_L2_EVICT);

    // IdxL3Inv -> OrigIdx
    idx_req(sim, "31g_IdxL3Inv", OP_IDX_L3_INV);

    // IdxL3Flush -> OrigIdx
    idx_req(sim, "31h_IdxL3Flush", OP_IDX_L3_FLUSH);

    // IdxL3Evict -> OrigIdx
    idx_req(sim, "31i_IdxL3Evict", OP_IDX_L3_EVICT);

    // L2Evict (cbuf evict) -> OrigIdx, address_space = AddressL2
    idx_req(sim, "31j_L2Evict", OP_L2_EVICT);

    // ScpZero -> OrigIdx, address_space = AddressScp
    idx_req(sim, "31k_ScpZero", OP_SCP_ZERO);

    // 32. Sync -> OrigSync
    idx_req(sim, "32_Sync", OP_SYNC);

    // 33. Idx stall: pipe_idx_cop_l2_busy blocks bank_ready
    {
        clear_inputs(d);
        d->pipe_idx_cop_l2_busy_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "33_IdxStall");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
    }

    printf("──── Control Signals ────\n");

    // 34. reqq_full when all entries valid
    {
        clear_inputs(d);
        d->reqq_entry_valid_i = ~0ULL;  // all 64 valid
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "34_ReqqFull");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
    }

    // 35. bank_ready depends on dataq_bank_ready
    {
        clear_inputs(d);
        d->dataq_bank_ready_i = 0;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "35_NoDatqReady");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
    }

    // 36. alloc_valid_ag when bank_valid & bank_ready
    bank_req(sim, "36_AllocOnValid", REQ_READ, 0x6000);

    // 37. AD pipeline: alloc_valid_ad follows alloc_valid_ag by 1 cycle
    // (already covered by the bank_req helper which checks both AG and AD)

    printf("──── Dependency (no matching entries with zeroed state) ────\n");

    // 38. No dependency (no matching entries) -> dep_valid=0
    bank_req(sim, "38_NoDep", REQ_READ, 0x7000);

    // 39. Victim match: tag_rsp_valid + victim -> dep_valid
    // (Cannot test with zeroed reqq_state; tested via victim match below)

    // 40. Victim match (tag_rsp_valid + victim) -> dep_valid
    {
        clear_inputs(d);
        d->bank_valid_i = 1;
        d->bank_opcode_i = REQ_READ;
        d->bank_address_i = 0x8000;
        d->bank_qwen_i = 0xF;
        d->bank_size_i = 6;
        d->pipe_tag_rsp_valid_i = 1;
        d->pipe_tag_rsp_victim_i = 1;
        d->pipe_tag_rsp_victim_address_i = 0x8000;
        d->pipe_tag_rsp_victim_address_space_i = ADDR_L2;
        d->pipe_tag_rsp_reqq_id_i = 5;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "40_VictimMatch");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "40_VictimMatch_AD");
    }

    printf("──── Stress: Sequential Allocs ────\n");

    // 41. Sequential allocs: 10 different opcodes in sequence
    {
        int opcodes[] = {REQ_READ, REQ_WRITE, REQ_WRITE_AROUND, REQ_ATOMIC,
                         REQ_LOCK, REQ_FLUSH, REQ_EVICT, REQ_PREFETCH,
                         REQ_READ_COOP, REQ_READ};
        for (int i = 0; i < 10; i++) {
            char label[64];
            snprintf(label, sizeof(label), "41_SeqAlloc_%d", i);
            bank_req(sim, label, opcodes[i], 0xA000 + i * 0x40, 0, 0, 0, 0,
                     (opcodes[i] == REQ_READ_COOP) ? 0x10000 : 0);
        }
    }

    printf("──── Stress: Random Stimulus ────\n");

    // 42. Random stimulus: 500 cycles
    {
        uint32_t seed = 0xDEAD;
        auto xrand = [&]() -> uint32_t {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            return seed;
        };

        for (int cycle = 0; cycle < 500; cycle++) {
            clear_inputs(d);
            d->reqq_allowed_entries_i = ~0ULL;
            uint32_t r = xrand();

            // Randomly present bank request
            if (r & 1) {
                d->bank_valid_i = 1;
                d->bank_opcode_i = xrand() % 16;
                d->bank_subopcode_i = xrand() & 0x7F;
                d->bank_address_i = xrand() & 0xFFFFFF;
                d->bank_data_i[0] = xrand();
                d->bank_qwen_i = xrand() & 0xF;
                d->bank_size_i = 6;
                d->bank_port_id_i = xrand() % 5;
                d->bank_source_i = xrand() & 0xF;
                d->bank_write_is_partial_i = xrand() & 1;
            }

            // Randomly present idx cop request
            if ((r >> 1) & 1) {
                d->pipe_idx_cop_req_valid_i = 1;
                d->pipe_idx_cop_req_opcode_i = OP_IDX_READ + (xrand() % 10);
                d->pipe_idx_cop_req_address_i = xrand() & 0xFFFFFF;
                d->pipe_idx_cop_l2_busy_i = 1;
            }

            // Randomly toggle controls
            d->dataq_bank_ready_i = (r >> 2) & 1;
            d->mbist_on_i = (r >> 3) & 1 ? 0 : 0;  // keep mbist off mostly
            d->pipe_idx_cop_l2_busy_i = d->pipe_idx_cop_req_valid_i;

            sim.tick();
            compare_all(sim);
        }
    }

    printf("──── Additional Idx CacheOp Stress ────\n");

    // Mix idx cop and bank requests
    for (int i = 0; i < 20; i++) {
        if (i % 3 == 0) {
            idx_req(sim, "stress_idx", OP_IDX_L2_FLUSH + (i % 3), 0xC000 + i * 0x40);
        } else {
            bank_req(sim, "stress_bank", REQ_READ + (i % 6), 0xD000 + i * 0x40);
        }
    }

    printf("──── mbist_on Stall ────\n");
    {
        clear_inputs(d);
        d->mbist_on_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "mbist_cycle1");
        sim.tick();
        compare_all(sim, "mbist_cycle2");
        d->mbist_on_i = 0;
        sim.tick();
        compare_all(sim, "mbist_release1");
        sim.tick();
        compare_all(sim, "mbist_release2");
    }

    printf("──── reqq_allowed_entries restricts allocation ────\n");
    {
        clear_inputs(d);
        d->reqq_allowed_entries_i = 0;  // no entries allowed
        d->bank_valid_i = 1;
        d->bank_opcode_i = REQ_READ;
        d->bank_address_i = 0xE000;
        d->bank_qwen_i = 0xF;
        d->bank_size_i = 6;
        sim.tick();
        compare_all(sim, "no_allowed_entries");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
    }

    printf("──── pipe_idx_cop_aborted ────\n");
    {
        clear_inputs(d);
        d->pipe_idx_cop_l2_busy_i = 1;
        d->pipe_idx_cop_aborted_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "aborted_no_stall");
        // With aborted, bank should NOT be stalled
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
    }

    printf("──── SCP address + remote_scp_enable ────\n");
    {
        // Remote SCP with remote_scp_enable = 1 -> no error
        uint64_t scp_remote = SCP_REGION_BIT | (0x42ULL << 23) | 0x40;
        clear_inputs(d);
        d->bank_valid_i = 1;
        d->bank_opcode_i = REQ_READ;
        d->bank_address_i = scp_remote;
        d->bank_qwen_i = 0xF;
        d->bank_size_i = 6;
        d->esr_sc_remote_scp_enable_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "scp_remote_enabled");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "scp_remote_enabled_AD");
    }

    printf("──── SCP illegal opcodes ────\n");
    {
        uint64_t scp_local = SCP_REGION_BIT | SCP_LOCAL_SHIRE | 0x40;

        // Atomic to SCP -> error
        bank_req(sim, "scp_atomic_err", REQ_ATOMIC, scp_local);

        // Flush to SCP -> error
        bank_req(sim, "scp_flush_err", REQ_FLUSH, scp_local);

        // Lock to SCP -> error
        bank_req(sim, "scp_lock_err", REQ_LOCK, scp_local);

        // Unlock to SCP -> error
        bank_req(sim, "scp_unlock_err", REQ_UNLOCK, scp_local);

        // Prefetch to SCP -> error
        bank_req(sim, "scp_prefetch_err", REQ_PREFETCH, scp_local);
    }

    printf("──── esr_wr_reqq_entries (arbiter reeval) ────\n");
    {
        clear_inputs(d);
        d->esr_wr_reqq_entries_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "esr_wr_reqq");
        d->esr_wr_reqq_entries_i = 0;
        sim.tick();
        compare_all(sim, "esr_wr_reqq_off");
    }

    printf("──── Sync with valid FSM entries (waiting) ────\n");
    // With no valid FSM entries (state all zero), sync goes through immediately.
    // This tests the pass-through case.
    idx_req(sim, "sync_no_wait", OP_SYNC);

    // Sync with flush_victims
    {
        clear_inputs(d);
        d->pipe_idx_cop_req_valid_i = 1;
        d->pipe_idx_cop_req_opcode_i = OP_SYNC;
        d->pipe_idx_cop_req_sync_flush_victims_i = 1;
        d->pipe_idx_cop_l2_busy_i = 1;
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "sync_flush_victims");
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "sync_flush_victims_AD");
    }

    printf("──── Multiple opcodes with different qwen ────\n");
    {
        // Write with partial qwen
        bank_req(sim, "write_qwen_partial", REQ_WRITE, 0xF000, 0, 0, 0, 0, 0, 0x5);

        // WriteAround with partial qwen
        bank_req(sim, "wa_qwen_partial", REQ_WRITE_AROUND, 0xF040, 0, 0, 0, 0, 0, 0xA);

        // MsgSendData with partial qwen, legal port
        bank_req(sim, "msg_qwen", REQ_MSG_SEND, 0x20000, 0, 0, 0, 0, 0, 0x3);

        // Atomic with partial qwen
        bank_req(sim, "atomic_qwen", REQ_ATOMIC, 0xF080, 0, 0, 0, 0, 0, 0x1);
    }

    printf("──── Final idle cycles ────\n");
    for (int i = 0; i < 5; i++) {
        clear_inputs(d);
        d->reqq_allowed_entries_i = ~0ULL;
        sim.tick();
        compare_all(sim, "final_idle");
    }

    return sim.finish();
}

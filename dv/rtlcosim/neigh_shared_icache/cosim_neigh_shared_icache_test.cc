// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_shared_icache_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>
#include <string>

using DUT = Vcosim_neigh_shared_icache_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kRspAckData = 1;
constexpr uint64_t kAddrMask49 = (1ull << 49) - 1ull;

struct ToggleSeen {
    bool zero = false;
    bool one = false;
    void sample(bool v) {
        zero |= !v;
        one |= v;
    }
    bool ok() const { return zero && one; }
};

struct ToggleCoverage {
    ToggleSeen prefetch_start;
    ToggleSeen err_rsp_enable;
    ToggleSeen bypass;
    ToggleSeen coop;
    ToggleSeen req_valid;
    ToggleSeen req_thread;
    ToggleSeen flush;
    ToggleSeen l2_disable;
    ToggleSeen l2_req_ready;
    ToggleSeen l2_resp_valid;
    ToggleSeen tlb_invalidate;
    ToggleSeen ptw_req_ready;
    ToggleSeen ptw_resp_valid;
    ToggleSeen sram_req_ready;
    ToggleSeen sram_resp_valid;
    ToggleSeen apb_pwrite;
    ToggleSeen apb_psel;
    ToggleSeen apb_penable;

    void sample(const DUT* dut) {
        prefetch_start.sample(dut->prefetch_start_i != 0);
        err_rsp_enable.sample(dut->err_rsp_enable_i != 0);
        bypass.sample(dut->bypass_icache_i != 0);
        coop.sample(dut->coop_mode_i != 0);
        req_valid.sample(dut->req_valid_i != 0);
        req_thread.sample(dut->req_thread_id_i != 0);
        flush.sample(dut->flush_data_i != 0);
        l2_disable.sample(dut->l2_miss_req_disable_next_i != 0);
        l2_req_ready.sample(dut->l2_miss_req_ready_i != 0);
        l2_resp_valid.sample(dut->l2_miss_resp_valid_i != 0);
        tlb_invalidate.sample(dut->tlb_invalidate_i != 0);
        ptw_req_ready.sample(dut->ptw_req_ready_i != 0);
        ptw_resp_valid.sample(dut->ptw_resp_valid_i != 0);
        sram_req_ready.sample(dut->sram_req_ready_i != 0);
        sram_resp_valid.sample(dut->sram_resp_valid_i != 0);
        apb_pwrite.sample(dut->apb_pwrite_i != 0);
        apb_psel.sample(dut->apb_psel_i != 0);
        apb_penable.sample(dut->apb_penable_i != 0);
    }

    void check(CosimCtrl<DUT>& sim) const {
        sim.check(prefetch_start.ok(), "prefetch_start_i toggled 0/1");
        sim.check(err_rsp_enable.ok(), "err_rsp_enable_i toggled 0/1");
        sim.check(bypass.ok(), "bypass_icache_i toggled 0/1");
        sim.check(coop.ok(), "coop_mode_i toggled 0/1");
        sim.check(req_valid.ok(), "req_valid_i toggled 0/1");
        sim.check(req_thread.ok(), "req_thread_id_i toggled 0/1");
        sim.check(flush.ok(), "flush_data_i toggled 0/1");
        sim.check(l2_disable.ok(), "l2_miss_req_disable_next_i toggled 0/1");
        sim.check(l2_req_ready.ok(), "l2_miss_req_ready_i toggled 0/1");
        sim.check(l2_resp_valid.ok(), "l2_miss_resp_valid_i toggled 0/1");
        sim.check(tlb_invalidate.ok(), "tlb_invalidate_i toggled 0/1");
        sim.check(ptw_req_ready.ok(), "ptw_req_ready_i toggled 0/1");
        sim.check(ptw_resp_valid.ok(), "ptw_resp_valid_i toggled 0/1");
        sim.check(sram_req_ready.ok(), "sram_req_ready_i toggled 0/1");
        sim.check(sram_resp_valid.ok(), "sram_resp_valid_i toggled 0/1");
        sim.check(apb_pwrite.ok(), "apb_pwrite_i toggled 0/1");
        sim.check(apb_psel.ok(), "apb_psel_i toggled 0/1");
        sim.check(apb_penable.ok(), "apb_penable_i toggled 0/1");
    }
};

void clear_inputs(DUT* dut) {
    dut->prefetch_prv_i = kPrvS;
    dut->prefetch_start_addr_i = 0;
    dut->prefetch_num_lines_i = 0;
    dut->prefetch_start_i = 0;
    dut->err_rsp_enable_i = 0;
    dut->err_interrupt_enable_i = 0;
    dut->mprot_i = 0;
    dut->vmspagesize_i = 0;
    dut->bypass_icache_i = 1;
    dut->coop_mode_i = 0;
    dut->req_valid_i = 0;
    dut->req_thread_id_i = 0;
    dut->req_addr0_i = 0;
    dut->req_addr1_i = 0;
    dut->req_addr2_i = 0;
    dut->req_addr3_i = 0;
    dut->req_addr4_i = 0;
    dut->req_addr5_i = 0;
    dut->req_addr6_i = 0;
    dut->req_addr7_i = 0;
    dut->req_prv_i = kPrvS;
    dut->req_mprv_i = 0;
    dut->req_mpp_i = kPrvM;
    dut->req_sum_i = 0;
    dut->req_mxr_i = 0;
    dut->req_debug_i = 0;
    dut->flush_data_i = 0;
    dut->l2_miss_req_disable_next_i = 0;
    dut->l2_miss_req_ready_i = 1;
    dut->l2_miss_resp_valid_i = 0;
    dut->l2_miss_resp_opcode_i = 0;
    dut->l2_resp_qw0_i = 0;
    dut->l2_resp_qw1_i = 0;
    dut->l2_resp_qw2_i = 0;
    dut->l2_resp_qw3_i = 0;
    dut->l2_resp_qw4_i = 0;
    dut->l2_resp_qw5_i = 0;
    dut->l2_resp_qw6_i = 0;
    dut->l2_resp_qw7_i = 0;
    dut->satp_mode_pack_i = 0;
    dut->satp_ppn_seed_i = 0;
    dut->matp_mode_pack_i = 0;
    dut->matp_ppn_seed_i = 0;
    dut->tlb_invalidate_i = 0;
    dut->ptw_req_ready_i = 0x3;
    dut->ptw_resp_valid_i = 0;
    dut->ptw_resp_flags_i = 0;
    dut->ptw0_resp_ppn_i = 0;
    dut->ptw1_resp_ppn_i = 0;
    dut->sram_req_ready_i = 1;
    dut->sram_resp_valid_i = 0;
    dut->sram_resp_qw0_i = 0;
    dut->sram_resp_qw1_i = 0;
    dut->sram_resp_qw2_i = 0;
    dut->sram_resp_qw3_i = 0;
    dut->sram_resp_qw4_i = 0;
    dut->sram_resp_qw5_i = 0;
    dut->sram_resp_qw6_i = 0;
    dut->sram_resp_qw7_i = 0;
    dut->apb_paddr_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("prefetch_done", d->orig_prefetch_done_o, d->new_prefetch_done_o);
    sim.compare("err_log_sbe", d->orig_err_log_sbe_o, d->new_err_log_sbe_o);
    sim.compare("err_log_dbe", d->orig_err_log_dbe_o, d->new_err_log_dbe_o);
    sim.compare("err_log_way", d->orig_err_log_way_o, d->new_err_log_way_o);
    sim.compare("err_log_set", d->orig_err_log_set_o, d->new_err_log_set_o);
    sim.compare("err_log_sbe_bits", d->orig_err_log_sbe_bits_o, d->new_err_log_sbe_bits_o);
    sim.compare("err_log_dbe_bits", d->orig_err_log_dbe_bits_o, d->new_err_log_dbe_bits_o);
    sim.compare("err_log_addr", d->orig_err_log_addr_o, d->new_err_log_addr_o);
    sim.compare("req_ready", d->orig_req_ready_o, d->new_req_ready_o);
    sim.compare("resp_valid", d->orig_resp_valid_o, d->new_resp_valid_o);
    sim.compare("resp_miss", d->orig_resp_miss_o, d->new_resp_miss_o);
    sim.compare("resp0_data_0", d->orig_resp0_data_0_o, d->new_resp0_data_0_o);
    sim.compare("resp0_data_1", d->orig_resp0_data_1_o, d->new_resp0_data_1_o);
    sim.compare("resp0_data_2", d->orig_resp0_data_2_o, d->new_resp0_data_2_o);
    sim.compare("resp0_data_3", d->orig_resp0_data_3_o, d->new_resp0_data_3_o);
    sim.compare("resp0_flags", d->orig_resp0_flags_o, d->new_resp0_flags_o);
    sim.compare("resp1_data_0", d->orig_resp1_data_0_o, d->new_resp1_data_0_o);
    sim.compare("resp1_data_1", d->orig_resp1_data_1_o, d->new_resp1_data_1_o);
    sim.compare("resp1_data_2", d->orig_resp1_data_2_o, d->new_resp1_data_2_o);
    sim.compare("resp1_data_3", d->orig_resp1_data_3_o, d->new_resp1_data_3_o);
    sim.compare("resp1_flags", d->orig_resp1_flags_o, d->new_resp1_flags_o);
    sim.compare("fill_done", d->orig_fill_done_o, d->new_fill_done_o);
    sim.compare("l2_req_valid", d->orig_l2_miss_req_valid_o, d->new_l2_miss_req_valid_o);
    sim.compare("l2_req_id", d->orig_l2_miss_req_id_o, d->new_l2_miss_req_id_o);
    sim.compare("l2_req_source", d->orig_l2_miss_req_source_o, d->new_l2_miss_req_source_o);
    sim.compare("l2_req_wdata", d->orig_l2_miss_req_wdata_o, d->new_l2_miss_req_wdata_o);
    sim.compare("l2_req_opcode", d->orig_l2_miss_req_opcode_o, d->new_l2_miss_req_opcode_o);
    sim.compare("l2_req_addr", d->orig_l2_miss_req_addr_o, d->new_l2_miss_req_addr_o);
    sim.compare("l2_req_size", d->orig_l2_miss_req_size_o, d->new_l2_miss_req_size_o);
    sim.compare("l2_req_qwen", d->orig_l2_miss_req_qwen_o, d->new_l2_miss_req_qwen_o);
    sim.compare("l2_req_subopcode", d->orig_l2_miss_req_subopcode_o, d->new_l2_miss_req_subopcode_o);
    sim.compare("l2_resp_ready", d->orig_l2_miss_resp_ready_o, d->new_l2_miss_resp_ready_o);
    sim.compare("ptw0_mode", d->orig_ptw0_req_satp_mode_o, d->new_ptw0_req_satp_mode_o);
    sim.compare("ptw0_ppn", d->orig_ptw0_req_satp_ppn_o, d->new_ptw0_req_satp_ppn_o);
    sim.compare("ptw0_prv", d->orig_ptw0_req_prv_o, d->new_ptw0_req_prv_o);
    sim.compare("ptw0_addr", d->orig_ptw0_req_addr_o, d->new_ptw0_req_addr_o);
    sim.compare("ptw0_valid", d->orig_ptw0_req_valid_o, d->new_ptw0_req_valid_o);
    sim.compare("ptw0_inv", d->orig_ptw0_invalidate_o, d->new_ptw0_invalidate_o);
    sim.compare("ptw1_mode", d->orig_ptw1_req_satp_mode_o, d->new_ptw1_req_satp_mode_o);
    sim.compare("ptw1_ppn", d->orig_ptw1_req_satp_ppn_o, d->new_ptw1_req_satp_ppn_o);
    sim.compare("ptw1_prv", d->orig_ptw1_req_prv_o, d->new_ptw1_req_prv_o);
    sim.compare("ptw1_addr", d->orig_ptw1_req_addr_o, d->new_ptw1_req_addr_o);
    sim.compare("ptw1_valid", d->orig_ptw1_req_valid_o, d->new_ptw1_req_valid_o);
    sim.compare("ptw1_inv", d->orig_ptw1_invalidate_o, d->new_ptw1_invalidate_o);
    sim.compare("sram_req_write", d->orig_sram_req_write_o, d->new_sram_req_write_o);
    sim.compare("sram_req_addr", d->orig_sram_req_addr_o, d->new_sram_req_addr_o);
    sim.compare("sram_req_valid", d->orig_sram_req_valid_o, d->new_sram_req_valid_o);
    sim.compare("sram_resp_ready", d->orig_sram_resp_ready_o, d->new_sram_resp_ready_o);
    sim.compare("apb_pready", d->orig_apb_pready_o, d->new_apb_pready_o);
    sim.compare("apb_prdata", d->orig_apb_prdata_o, d->new_apb_prdata_o);
    sim.compare("apb_pslverr", d->orig_apb_pslverr_o, d->new_apb_pslverr_o);
    sim.compare("dbg_chunk0", d->orig_dbg_chunk0_o, d->new_dbg_chunk0_o);
    sim.compare("dbg_chunk1", d->orig_dbg_chunk1_o, d->new_dbg_chunk1_o);
    sim.compare("dbg_chunk2", d->orig_dbg_chunk2_o, d->new_dbg_chunk2_o);
    sim.compare("dbg_chunk3", d->orig_dbg_chunk3_o, d->new_dbg_chunk3_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, ToggleCoverage& toggles) {
    toggles.sample(sim.dut.get());
    sim.tick();
    compare_outputs(sim);
}

void set_req_addr(DUT* dut, int minion, uint64_t addr) {
    addr &= kAddrMask49;
    switch (minion) {
        case 0: dut->req_addr0_i = addr; break;
        case 1: dut->req_addr1_i = addr; break;
        case 2: dut->req_addr2_i = addr; break;
        case 3: dut->req_addr3_i = addr; break;
        case 4: dut->req_addr4_i = addr; break;
        case 5: dut->req_addr5_i = addr; break;
        case 6: dut->req_addr6_i = addr; break;
        case 7: dut->req_addr7_i = addr; break;
        default: break;
    }
}

void set_sram_words(DUT* dut, uint64_t base) {
    dut->sram_resp_qw0_i = base ^ 0x0001020304050607ull;
    dut->sram_resp_qw1_i = base ^ 0x1011121314151617ull;
    dut->sram_resp_qw2_i = base ^ 0x2021222324252627ull;
    dut->sram_resp_qw3_i = base ^ 0x3031323334353637ull;
    dut->sram_resp_qw4_i = base ^ 0x4041424344454647ull;
    dut->sram_resp_qw5_i = base ^ 0x5051525354555657ull;
    dut->sram_resp_qw6_i = base ^ 0x6061626364656667ull;
    dut->sram_resp_qw7_i = base ^ 0x7071727374757677ull;
}

void set_l2_words(DUT* dut, uint64_t base) {
    dut->l2_resp_qw0_i = base ^ 0x1111111111111111ull;
    dut->l2_resp_qw1_i = base ^ 0x2222222222222222ull;
    dut->l2_resp_qw2_i = base ^ 0x3333333333333333ull;
    dut->l2_resp_qw3_i = base ^ 0x4444444444444444ull;
    dut->l2_resp_qw4_i = base ^ 0x5555555555555555ull;
    dut->l2_resp_qw5_i = base ^ 0x6666666666666666ull;
    dut->l2_resp_qw6_i = base ^ 0x7777777777777777ull;
    dut->l2_resp_qw7_i = base ^ 0x8888888888888888ull;
}

void drive_single_request(CosimCtrl<DUT>& sim, ToggleCoverage& toggles, int minion, int thread, uint64_t addr) {
    clear_inputs(sim.dut.get());
    sim.dut->bypass_icache_i = 1;
    sim.dut->req_valid_i = static_cast<uint8_t>(1u << minion);
    sim.dut->req_thread_id_i = thread ? static_cast<uint8_t>(1u << minion) : 0;
    set_req_addr(sim.dut.get(), minion, addr);
    tick_and_compare(sim, toggles);
    sim.dut->req_valid_i = 0;
    for (int i = 0; i < 8; ++i) tick_and_compare(sim, toggles);
}

void drive_miss_fill(CosimCtrl<DUT>& sim, ToggleCoverage& toggles, int minion, uint64_t addr) {
    clear_inputs(sim.dut.get());
    sim.dut->bypass_icache_i = 0;
    sim.dut->req_valid_i = static_cast<uint8_t>(1u << minion);
    set_req_addr(sim.dut.get(), minion, addr);
    sim.dut->sram_req_ready_i = 1;
    sim.dut->l2_miss_req_ready_i = 1;
    for (int i = 0; i < 12; ++i) tick_and_compare(sim, toggles);
    sim.dut->req_valid_i = 0;
    set_sram_words(sim.dut.get(), addr);
    set_l2_words(sim.dut.get(), addr);
    for (int i = 0; i < 24; ++i) {
        sim.dut->l2_miss_resp_valid_i = (i == 2 || i == 3);
        sim.dut->l2_miss_resp_opcode_i = kRspAckData;
        sim.dut->sram_resp_valid_i = (i >= 4 && i <= 6);
        tick_and_compare(sim, toggles);
    }
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    ToggleCoverage toggles;
    clear_inputs(sim.dut.get());
    sim.reset(3);
    clear_inputs(sim.dut.get());
    compare_outputs(sim);

    // Directed request staging/arbitration: all minions and both threads.
    for (int minion = 0; minion < 8; ++minion) {
        drive_single_request(sim, toggles, minion, 0, 0x1000ull + (static_cast<uint64_t>(minion) << 12));
        drive_single_request(sim, toggles, minion, 1, 0x2000ull + (static_cast<uint64_t>(minion) << 12));
    }

    // Simultaneous contenders in each requestor group exercise delayed LRU grants.
    clear_inputs(sim.dut.get());
    sim.dut->bypass_icache_i = 1;
    sim.dut->req_valid_i = 0x0f;
    sim.dut->req_thread_id_i = 0x05;
    for (int m = 0; m < 4; ++m) set_req_addr(sim.dut.get(), m, 0x4000ull + (static_cast<uint64_t>(m) << 8));
    for (int i = 0; i < 24; ++i) tick_and_compare(sim, toggles);
    clear_inputs(sim.dut.get());
    sim.dut->bypass_icache_i = 1;
    sim.dut->req_valid_i = 0xf0;
    sim.dut->req_thread_id_i = 0xa0;
    for (int m = 4; m < 8; ++m) set_req_addr(sim.dut.get(), m, 0x8000ull + (static_cast<uint64_t>(m) << 8));
    for (int i = 0; i < 24; ++i) tick_and_compare(sim, toggles);

    // Flush, APB, prefetch and ESR controls.
    clear_inputs(sim.dut.get());
    for (int i = 0; i < 32; ++i) {
        sim.dut->flush_data_i = (i & 1) ? 0xa5 : 0;
        sim.dut->prefetch_start_i = (i == 3);
        sim.dut->prefetch_num_lines_i = 4;
        sim.dut->prefetch_start_addr_i = 0x1200 + i;
        sim.dut->err_rsp_enable_i = (i & 2) != 0;
        sim.dut->err_interrupt_enable_i = i & 7;
        sim.dut->coop_mode_i = (i & 4) != 0;
        sim.dut->apb_psel_i = (i % 3) == 0;
        sim.dut->apb_penable_i = (i % 3) == 1;
        sim.dut->apb_pwrite_i = (i & 1) != 0;
        sim.dut->apb_paddr_i = i;
        sim.dut->apb_pwdata_i = 0x1000 + i;
        tick_and_compare(sim, toggles);
    }

    // Miss/fill and PTW-oriented traffic.
    drive_miss_fill(sim, toggles, 0, 0x0000000000010000ull);
    drive_miss_fill(sim, toggles, 5, 0x0000000000020000ull);

    clear_inputs(sim.dut.get());
    sim.dut->bypass_icache_i = 0;
    sim.dut->satp_mode_pack_i = 0x99999999u;
    sim.dut->matp_mode_pack_i = 0x88888888u;
    sim.dut->satp_ppn_seed_i = 0x1234500ull;
    sim.dut->matp_ppn_seed_i = 0x2234500ull;
    sim.dut->req_prv_i = kPrvS;
    sim.dut->req_valid_i = 0x11;
    set_req_addr(sim.dut.get(), 0, 0x0000004000001000ull);
    set_req_addr(sim.dut.get(), 4, 0x0000004000002000ull);
    for (int i = 0; i < 80; ++i) {
        sim.dut->ptw_req_ready_i = (i & 1) ? 0x3 : 0x1;
        sim.dut->ptw_resp_valid_i = (i == 20 || i == 21 || i == 44) ? 0x3 : 0;
        sim.dut->ptw_resp_flags_i = 0x0000c0ffu ^ static_cast<uint32_t>(i * 0x101u);
        sim.dut->ptw0_resp_ppn_i = 0x345600ull + i;
        sim.dut->ptw1_resp_ppn_i = 0x456700ull + i;
        sim.dut->tlb_invalidate_i = (i == 50) ? 0xff : 0;
        tick_and_compare(sim, toggles);
    }

    // Constrained random phase. Keep clocks/resets legal while toggling every practical data/control input.
    std::mt19937_64 rng(0x51a7ed1caceull);
    for (int cycle = 0; cycle < 5000; ++cycle) {
        uint64_t r0 = rng();
        uint64_t r1 = rng();
        uint64_t r2 = rng();
        sim.dut->prefetch_prv_i = r0 & 0x3;
        sim.dut->prefetch_start_addr_i = (r0 >> 6) & ((1ull << 42) - 1ull);
        sim.dut->prefetch_num_lines_i = (r0 >> 12) & 0x3f;
        sim.dut->prefetch_start_i = (cycle % 257) == 3;
        sim.dut->err_rsp_enable_i = (r0 >> 20) & 1;
        sim.dut->err_interrupt_enable_i = (r0 >> 21) & 0x7;
        sim.dut->mprot_i = (r0 >> 24) & 0xff;
        sim.dut->vmspagesize_i = (r0 >> 33) & 0x3;
        sim.dut->bypass_icache_i = (cycle % 5) != 0;
        sim.dut->coop_mode_i = (r0 >> 35) & 1;
        sim.dut->req_valid_i = static_cast<uint8_t>((cycle % 4 == 0) ? (r0 & 0xff) : 0);
        sim.dut->req_thread_id_i = static_cast<uint8_t>((r0 >> 8) & 0xff);
        sim.dut->req_prv_i = (r0 >> 16) & 0x3;
        sim.dut->req_mprv_i = (r0 >> 18) & 1;
        sim.dut->req_mpp_i = (r0 >> 19) & 0x3;
        sim.dut->req_sum_i = (r0 >> 21) & 1;
        sim.dut->req_mxr_i = (r0 >> 22) & 1;
        sim.dut->req_debug_i = (r0 >> 23) & 1;
        set_req_addr(sim.dut.get(), 0, r0);
        set_req_addr(sim.dut.get(), 1, r1);
        set_req_addr(sim.dut.get(), 2, r2);
        set_req_addr(sim.dut.get(), 3, rng());
        set_req_addr(sim.dut.get(), 4, rng());
        set_req_addr(sim.dut.get(), 5, rng());
        set_req_addr(sim.dut.get(), 6, rng());
        set_req_addr(sim.dut.get(), 7, rng());
        sim.dut->flush_data_i = static_cast<uint8_t>((cycle % 31 == 0) ? (r1 & 0xff) : 0);
        sim.dut->l2_miss_req_disable_next_i = (r1 >> 8) & 1;
        sim.dut->l2_miss_req_ready_i = (cycle % 7) != 0;
        sim.dut->l2_miss_resp_valid_i = (cycle % 23) == 0;
        sim.dut->l2_miss_resp_opcode_i = (cycle % 97 == 0) ? 3 : kRspAckData;
        set_l2_words(sim.dut.get(), r1);
        sim.dut->satp_mode_pack_i = (cycle % 11 == 0) ? 0x99999999u : 0;
        sim.dut->satp_ppn_seed_i = r1;
        sim.dut->matp_mode_pack_i = (cycle % 13 == 0) ? 0x88888888u : 0;
        sim.dut->matp_ppn_seed_i = r2;
        sim.dut->tlb_invalidate_i = static_cast<uint8_t>((cycle % 127 == 0) ? (r2 & 0xff) : 0);
        sim.dut->ptw_req_ready_i = static_cast<uint8_t>((r2 >> 8) & 0x3);
        sim.dut->ptw_resp_valid_i = static_cast<uint8_t>((cycle % 41 == 0) ? ((r2 >> 10) & 0x3) : 0);
        sim.dut->ptw_resp_flags_i = static_cast<uint32_t>(r2 >> 16);
        sim.dut->ptw0_resp_ppn_i = rng();
        sim.dut->ptw1_resp_ppn_i = rng();
        sim.dut->sram_req_ready_i = (cycle % 9) != 0;
        sim.dut->sram_resp_valid_i = (cycle % 17) == 0;
        set_sram_words(sim.dut.get(), rng());
        sim.dut->apb_psel_i = (cycle % 19) == 0;
        sim.dut->apb_penable_i = (cycle % 19) == 1;
        sim.dut->apb_pwrite_i = (r2 >> 20) & 1;
        sim.dut->apb_paddr_i = r2 & 0x3ff;
        sim.dut->apb_pwdata_i = static_cast<uint32_t>(r2 >> 32);
        tick_and_compare(sim, toggles);
    }

    toggles.check(sim);
    return sim.finish();
}

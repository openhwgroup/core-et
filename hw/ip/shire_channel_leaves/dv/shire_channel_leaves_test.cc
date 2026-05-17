// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_channel_leaves_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

namespace {

uint16_t dmctrl_bits(bool dmactive, bool ndmreset, bool clresethaltreq,
                     bool setresethaltreq, bool hasel, bool ackhavereset,
                     bool hartreset, bool resumereq, bool haltreq,
                     bool write_enable) {
    return (uint16_t(dmactive) << 9) | (uint16_t(ndmreset) << 8) |
           (uint16_t(clresethaltreq) << 7) | (uint16_t(setresethaltreq) << 6) |
           (uint16_t(hasel) << 5) | (uint16_t(ackhavereset) << 4) |
           (uint16_t(hartreset) << 3) | (uint16_t(resumereq) << 2) |
           (uint16_t(haltreq) << 1) | uint16_t(write_enable);
}

uint32_t bpam_bits() {
    // gpio: ndmreset, halt, resume, hart_reset, tbox_halt, tbox_resume,
    //       rbox_halt, rbox_resume, rbox_reset
    // events: halt, resume, tbox_halt, tbox_resume, rbox_halt, rbox_resume
    return (1u << 14) | (1u << 12) | (1u << 11) | (1u << 9) |
           (1u << 8) | (1u << 6) | (1u << 5) | (1u << 3) | (1u << 0);
}

uint16_t l0_bits(bool selected, bool unavailable, bool allhavereset,
                 bool anyhavereset, bool allresumeack, bool anyresumeack,
                 bool allrunning, bool anyrunning, bool allhalted,
                 bool anyhalted) {
    return (uint16_t(selected) << 9) | (uint16_t(unavailable) << 8) |
           (uint16_t(allhavereset) << 7) | (uint16_t(anyhavereset) << 6) |
           (uint16_t(allresumeack) << 5) | (uint16_t(anyresumeack) << 4) |
           (uint16_t(allrunning) << 3) | (uint16_t(anyrunning) << 2) |
           (uint16_t(allhalted) << 1) | uint16_t(anyhalted);
}

uint16_t l2_bits(bool unavailable, bool allhavereset, bool anyhavereset,
                 bool allresumeack, bool anyresumeack, bool allrunning,
                 bool anyrunning, bool allhalted, bool anyhalted2,
                 bool anyhalted1, bool anyhalted0) {
    return (uint16_t(unavailable) << 10) | (uint16_t(allhavereset) << 9) |
           (uint16_t(anyhavereset) << 8) | (uint16_t(allresumeack) << 7) |
           (uint16_t(anyresumeack) << 6) | (uint16_t(allrunning) << 5) |
           (uint16_t(anyrunning) << 4) | (uint16_t(allhalted) << 3) |
           (uint16_t(anyhalted2) << 2) | (uint16_t(anyhalted1) << 1) |
           uint16_t(anyhalted0);
}

uint32_t coop_rdy_bits(uint32_t dest, uint32_t id) {
    return ((dest & 0x3u) << 5) | (id & 0x1fu);
}

void set_wdata_reg(WData* words, int reg_idx, uint16_t value) {
    const int bit = reg_idx * 16;
    const int word = bit / 32;
    const int shift = bit % 32;
    words[word] &= ~(0xffffu << shift);
    words[word] |= uint32_t(value) << shift;
}

uint64_t wide_get(const WData* words, int bit, int width) {
    uint64_t value = 0;
    for (int i = 0; i < width; ++i) {
        if ((words[(bit + i) / 32] >> ((bit + i) & 31)) & 1u) value |= uint64_t{1} << i;
    }
    return value;
}

void clear_wide(WData* words, int n) {
    for (int i = 0; i < n; ++i) words[i] = 0;
}

uint32_t pll_cfg_bits(bool pclk1, bool pclk0, bool lock_reset_disable,
                      uint32_t reg_num, uint32_t reg_first, bool write,
                      bool run, bool enable, bool reset) {
    return (uint32_t(pclk1) << 16) | (uint32_t(pclk0) << 15) |
           (uint32_t(lock_reset_disable) << 14) | ((reg_num & 0xfu) << 10) |
           ((reg_first & 0x3fu) << 4) | (uint32_t(write) << 3) |
           (uint32_t(run) << 2) | (uint32_t(enable) << 1) | uint32_t(reset);
}

uint32_t dll_cfg_bits(bool dll_enable, uint32_t pclk_sel, bool lock_reset_disable,
                      uint32_t reg_num, uint32_t reg_first, bool write,
                      bool run, bool enable, bool reset) {
    return (uint32_t(dll_enable) << 13) | ((pclk_sel & 0x3u) << 11) |
           (uint32_t(lock_reset_disable) << 10) | ((reg_num & 0x7u) << 7) |
           ((reg_first & 0x7u) << 4) | (uint32_t(write) << 3) |
           (uint32_t(run) << 2) | (uint32_t(enable) << 1) | uint32_t(reset);
}

void clear_inputs(Vshire_channel_leaves_tb* d) {
    d->dmctrl_bits_i = 0;
    d->bpam_bits_i = 0;
    d->tbox_single_step_i = 0;
    d->rbox_single_step_i = 0;
    d->shire_id_i = 0;
    d->and_or_l2_bits_i = 0;
    d->and_or_l0_bits_i = 0;
    d->coop_slv_rdy_data_i = 0;
    d->coop_slv_rdy_valid_i = 0;
    d->coop_mst_done_id_i = 0;
    d->coop_mst_done_valid_i = 0;
    d->noc_err_int_srcs_i = 0;
    d->xll_enable_i = 0;
    d->xll_reset_i = 0;
    d->xll_run_i = 0;
    d->xll_write_i = 0;
    d->xll_reg_first_i = 0;
    d->xll_reg_num_i = 0;
    d->xll_pclk_sel_i = 0;
    clear_wide(d->xll_wdata_i, 8);
    d->xll_prdata_i = 0;
    d->xll_pready_i = 1;
    d->dft_bits_i = 0;
    d->pll_lock_i = 0;
    d->dll_lock_i = 0;
    d->pll_gate_disable_i = 0;
    d->pll_debug_gate_disable_i = 0;
    d->pll_auto_cfg_bits_i = 0;
    d->dll_auto_cfg_bits_i = 0;
    d->pll_clk_sel_bits_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<Vshire_channel_leaves_tb> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    sim.check(d->dmctrl_bits_o == 0, "dmctrl reset output is zero");
    sim.check(d->ioshire_noc_err_int_o == 0, "noc interrupt reset output is zero");

    const uint16_t dm_a = dmctrl_bits(1, 1, 0, 1, 0, 1, 0, 1, 1, 1);
    d->dmctrl_bits_i = dm_a;
    bool saw_we = false;
    uint16_t captured = 0;
    for (int i = 0; i < 32; ++i) {
        sim.tick();
        if (d->dmctrl_bits_o & 1u) {
            saw_we = true;
            captured = d->dmctrl_bits_o;
        }
    }
    sim.check(saw_we, "dmctrl write-enable pulse observed after sync/delay");
    sim.check((captured & 0x3feu) == (dm_a & 0x3feu), "dmctrl captures payload on toggle");

    d->dmctrl_bits_i = dmctrl_bits(0, 0, 1, 0, 1, 0, 1, 0, 0, 0);
    saw_we = false;
    for (int i = 0; i < 32; ++i) {
        sim.tick();
        saw_we = saw_we || ((d->dmctrl_bits_o & 1u) != 0);
    }
    sim.check(saw_we, "dmctrl pulses on write-enable falling edge as in original XOR detector");

    d->bpam_bits_i = bpam_bits();
    d->tbox_single_step_i = 0b1010;
    d->rbox_single_step_i = 1;
    sim.tick();
    for (int n = 0; n < 4; ++n) {
        const uint64_t neigh = (d->bpam_neigh_bits_o >> (n * 11)) & 0x7ffu;
        const uint64_t expected = ((uint64_t(1) << 10) | (uint64_t(1) << 8) |
                                   (uint64_t(1) << 7) | (uint64_t(1) << 5) |
                                   (uint64_t(1) << 4) | (uint64_t(1) << 2) |
                                   (((0b1010u >> n) & 1u)));
        sim.check(neigh == expected, "bpam neighborhood " + std::to_string(n) + " mapping");
    }
    sim.check(d->bpam_rbox_bits_o == 0b101011, "bpam rbox mapping");

    const uint64_t l0 = uint64_t(l0_bits(1, 0, 1, 1, 1, 0, 1, 0, 1, 1)) |
                        (uint64_t(l0_bits(1, 1, 1, 0, 1, 1, 1, 1, 1, 0)) << 10) |
                        (uint64_t(l0_bits(1, 0, 0, 1, 1, 0, 0, 0, 0, 1)) << 20) |
                        (uint64_t(l0_bits(1, 0, 1, 0, 0, 1, 1, 1, 1, 0)) << 30);
    d->and_or_l0_bits_i = l0;
    d->and_or_l2_bits_i = l2_bits(0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0);
    d->shire_id_i = 8;
    sim.tick();
    sim.check((d->and_or_l1_bits_o & 0x3u) == 0x3u, "and/or local anyhalted halves");
    sim.check(((d->and_or_l1_bits_o >> 9) & 1u) == 1u, "and/or local anyunavailable");
    sim.check(((d->and_or_l1_bits_o >> 3) & 1u) == 1u, "and/or local anyrunning");
    sim.check((d->and_or_l2_bits_o & 1u) == 1u, "and/or l2 group0 daisy update");
    d->shire_id_i = 24;
    sim.tick();
    sim.check(((d->and_or_l2_bits_o >> 1) & 1u) == 1u, "and/or l2 group1 daisy update");
    d->shire_id_i = 40;
    sim.tick();
    sim.check(((d->and_or_l2_bits_o >> 2) & 1u) == 1u, "and/or l2 group2 daisy update");

    uint32_t coop_data = 0;
    uint32_t coop_ids = 0;
    for (int n = 0; n < 4; ++n) {
        coop_data |= coop_rdy_bits(n, 3 + n) << (n * 7);
        coop_ids |= (10 + n) << (n * 5);
    }
    d->coop_slv_rdy_data_i = coop_data;
    d->coop_slv_rdy_valid_i = 0b1101;
    d->coop_mst_done_id_i = coop_ids;
    d->coop_mst_done_valid_i = 0b101'011'110'001;
    sim.tick();
    sim.check(wide_get(d->coop_slv_rdy_data_o, 0, 7) == coop_rdy_bits(1, 4),
              "coop neigh0 sees neigh1 ready on slot0");
    sim.check(wide_get(d->coop_slv_rdy_data_o, 3 * 7, 7) == coop_rdy_bits(0, 3),
              "coop neigh1 sees neigh0 ready on slot0");
    sim.check(((d->coop_mst_done_id_o >> (4 * 5)) & 0x1fu) == 12,
              "coop done-id crossbar preserves source id");

    d->noc_err_int_srcs_i = 0;
    sim.tick();
    sim.check(d->ioshire_noc_err_int_o == 0, "noc interrupt remains low for no source");
    d->noc_err_int_srcs_i = 0x4002;
    sim.tick();
    sim.check(d->noc_all_err_int_srcs_o == 0x4002, "noc status pass-through");
    sim.check(d->ioshire_noc_err_int_o == 1, "noc interrupt registered OR");

    d->xll_enable_i = 1;
    d->xll_write_i = 1;
    d->xll_reg_first_i = 3;
    d->xll_reg_num_i = 1;
    d->xll_pclk_sel_i = 0;
    clear_wide(d->xll_wdata_i, 8);
    set_wdata_reg(d->xll_wdata_i, 0, 0x1111);
    set_wdata_reg(d->xll_wdata_i, 1, 0x2222);
    d->xll_run_i = 1;
    sim.tick();
    d->xll_run_i = 0;
    bool saw_addr3 = false;
    bool saw_addr4 = false;
    for (int i = 0; i < 120; ++i) {
        sim.tick();
        if (d->xll_psel_o && d->xll_pwrite_o && d->xll_paddr_o == 3 && d->xll_pwdata_o == 0x1111) {
            saw_addr3 = true;
        }
        if (d->xll_psel_o && d->xll_pwrite_o && d->xll_paddr_o == 4 && d->xll_pwdata_o == 0x2222) {
            saw_addr4 = true;
        }
    }
    sim.check(saw_addr3, "xll writes first register");
    sim.check(saw_addr4, "xll writes last register when reg_num is one");
    sim.check(d->xll_busy_o == 0, "xll write sequence eventually idle");

    d->xll_write_i = 0;
    d->xll_reg_first_i = 5;
    d->xll_reg_num_i = 0;
    d->xll_prdata_i = 0xabcd;
    d->xll_run_i = 1;
    sim.tick();
    d->xll_run_i = 0;
    bool saw_rrdy = false;
    for (int i = 0; i < 80; ++i) {
        sim.tick();
        saw_rrdy = saw_rrdy || d->xll_rrdy_o;
    }
    sim.check(saw_rrdy, "xll read raises rrdy");
    sim.check(d->xll_rdata_o == 0xabcd, "xll read captures prdata");

    d->pll_lock_i = 1;
    d->dll_lock_i = 1;
    d->pll_auto_cfg_bits_i = pll_cfg_bits(0, 0, 0, 0, 0, false, false, true, false);
    d->dll_auto_cfg_bits_i = dll_cfg_bits(true, 0, false, 0, 0, false, false, true, false);
    d->pll_clk_sel_bits_i = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.check(d->pll_lock_detect_o == 1, "pll lock synchronized to system clock");
    sim.check(d->dll_lock_detect_o == 1, "dll lock synchronized to system clock");
    sim.check(d->pll_busy_o == 0 && d->dll_busy_o == 0, "pll wrapper control idle without run pulse");

    sim.check(d->icache_apb_pready_o == 0, "icache_mems APB idle ready is low");
    sim.check(d->icache_apb_pslverr_o == 0, "icache_mems APB idle error is low");

    return sim.finish();
}

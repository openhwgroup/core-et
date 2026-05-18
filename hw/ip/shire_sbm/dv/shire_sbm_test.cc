// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_sbm_tb.h"
#include "sim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vshire_sbm_tb;

static constexpr uint32_t kNumIf = 11;
static constexpr uint8_t kShireId = 0x2a;

static uint64_t make_addr(uint32_t cls, uint32_t block, uint32_t bank_or_neigh,
                          uint32_t reg, uint32_t pp = 3) {
    uint64_t addr = 0;
    addr |= 1ull << 32;
    addr |= static_cast<uint64_t>(pp & 0x3) << 30;
    addr |= static_cast<uint64_t>(kShireId) << 22;
    addr |= static_cast<uint64_t>(cls & 0x3) << 20;
    if (cls == 3) {
        addr |= static_cast<uint64_t>(block & 0x7) << 17;
        if (block == 0) addr |= static_cast<uint64_t>(bank_or_neigh & 0xf) << 13;
        addr |= static_cast<uint64_t>(reg & 0x3fff) << 3;
    } else if (cls == 1) {
        addr |= static_cast<uint64_t>(bank_or_neigh & 0xf) << 16;
        addr |= static_cast<uint64_t>(reg & 0x1fff) << 3;
    } else {
        addr |= static_cast<uint64_t>(reg & 0x7f) << 13;
    }
    return addr;
}

static void defaults(DUT* dut) {
    dut->rst_neigh_ni = 0xf;
    dut->rst_rbox_ni = 1;
    dut->rst_shire_cache_ni = 1;
    dut->shire_virtual_id_i = kShireId;
    dut->ar_valid_i = 0;
    dut->aw_valid_i = 0;
    dut->ar_id_i = 0;
    dut->aw_id_i = 0;
    dut->ar_addr_i = 0;
    dut->aw_addr_i = 0;
    dut->ar_len_i = 0;
    dut->aw_len_i = 0;
    dut->wdata_i = 0;
    dut->b_ready_i = 1;
    dut->r_ready_i = 1;
    dut->apb_pready_i = (1u << kNumIf) - 1u;
    dut->apb_pslverr_i = 0;
    dut->apb_prdata_i = 0x1122334455667788ull;
    dut->obs_index_i = 0;
}

static bool wait_for_rvalid(SimCtrl<DUT>& sim, int max_cycles = 80) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sim.dut->r_valid_o) return true;
    }
    return false;
}

static bool wait_for_bvalid(SimCtrl<DUT>& sim, int max_cycles = 100) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (sim.dut->b_valid_o) return true;
    }
    return false;
}

static bool wait_for_psel(SimCtrl<DUT>& sim, uint32_t mask, int max_cycles = 60) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if ((sim.dut->apb_psel_o & mask) == mask) return true;
    }
    return false;
}

static bool wait_for_axi_ready(SimCtrl<DUT>& sim, bool want_write, int max_cycles = 80) {
    for (int i = 0; i < max_cycles; ++i) {
        sim.tick();
        if (want_write) {
            if (sim.dut->aw_ready_o && sim.dut->w_ready_o) return true;
        } else {
            if (sim.dut->ar_ready_o) return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* dut = sim.dut.get();
    defaults(dut);
    sim.reset(5);
    defaults(dut);
    sim.tick();

    sim.check(dut->ar_ready_o == 1, "AR ready after reset");
    sim.check(dut->aw_ready_o == 1, "AW ready after reset");
    sim.check(dut->w_ready_o == 1, "W ready after reset");

    // Directed read to shire-level ESR lane (index 8).
    const uint64_t shire_addr = make_addr(3, 2, 0, 0x24);
    dut->ar_valid_i = 1;
    dut->ar_id_i = 0x55;
    dut->ar_addr_i = shire_addr;
    dut->apb_prdata_i = 0xa5a55a5acc33cc33ull;
    dut->obs_index_i = 8;
    sim.tick();
    dut->ar_valid_i = 0;
    sim.check(wait_for_psel(sim, 1u << 8), "shire ESR read reaches APB lane 8");
    sim.check(dut->obs_paddr_o == ((3u << 14) | 0x24u), "shire ESR paddr decode");
    sim.check((dut->apb_pwrite_o & (1u << 8)) == 0, "shire ESR read pwrite=0");
    sim.check(wait_for_rvalid(sim), "shire ESR read returns RVALID");
    sim.check(dut->r_id_o == 0x55, "shire ESR read response ID");
    sim.check(dut->r_resp_o == 0, "shire ESR read response OKAY");
    sim.check(dut->r_last_o == 1, "shire ESR read response LAST");
    sim.check(dut->r_data_lo_o == 0xa5a55a5acc33cc33ull, "shire ESR read response data");
    sim.tick();

    // Directed write to bank 2 (lane 6).
    const uint64_t bank2_addr = make_addr(3, 0, 2, 0x15);
    dut->aw_valid_i = 1;
    dut->aw_id_i = 0x123;
    dut->aw_addr_i = bank2_addr;
    dut->wdata_i = 0xdeadbeefcafef00dull;
    dut->obs_index_i = 6;
    sim.tick();
    dut->aw_valid_i = 0;
    sim.check(wait_for_psel(sim, 1u << 6), "bank write reaches APB lane 6");
    sim.check((dut->apb_pwrite_o & (1u << 6)) != 0, "bank write pwrite=1");
    sim.check(dut->obs_paddr_o == ((3u << 10) | 0x15u), "bank write paddr decode");
    sim.check(dut->obs_pwdata_o == 0xdeadbeefcafef00dull, "bank write data forwarded");
    sim.check(wait_for_bvalid(sim), "bank write returns BVALID");
    sim.check(dut->b_id_o == 0x123, "bank write response ID");
    sim.check(dut->b_resp_o == 0, "bank write response OKAY");
    sim.tick();

    // Broadcast write to all neighborhoods.
    const uint64_t bcast_neigh_addr = make_addr(1, 0, 0xf, 0x40);
    dut->aw_valid_i = 1;
    dut->aw_id_i = 0x77;
    dut->aw_addr_i = bcast_neigh_addr;
    dut->wdata_i = 0x0102030405060708ull;
    sim.tick();
    dut->aw_valid_i = 0;
    sim.check(wait_for_psel(sim, 0x00f), "neighborhood broadcast drives lanes 0..3");
    sim.check(wait_for_bvalid(sim), "neighborhood broadcast returns a single BVALID");
    sim.check(dut->b_id_o == 0x77, "neighborhood broadcast response ID");
    sim.check(dut->b_resp_o == 0, "neighborhood broadcast response OKAY");
    sim.tick();

    // Invalid read: wrong shire ID should return SLVERR with zero data.
    sim.check(wait_for_axi_ready(sim, false), "AR ready before invalid read");
    uint64_t bad_addr = shire_addr ^ (1ull << 22);
    dut->ar_valid_i = 1;
    dut->ar_id_i = 0x19;
    dut->ar_addr_i = bad_addr;
    sim.tick();
    dut->ar_valid_i = 0;
    sim.check(dut->r_valid_o == 1, "invalid read returns RVALID");
    sim.check(dut->r_id_o == 0x19, "invalid read response ID");
    sim.check(dut->r_resp_o == 2, "invalid read response SLVERR");
    sim.check(dut->r_data_lo_o == 0, "invalid read response data zero");
    sim.tick();

    // Reset-gated RBOX read returns a decode error and never issues APB.
    sim.check(wait_for_axi_ready(sim, false), "AR ready before RBOX reset-gated read");
    dut->rst_rbox_ni = 0;
    dut->ar_valid_i = 1;
    dut->ar_id_i = 0x1a;
    dut->ar_addr_i = make_addr(3, 1, 0, 0x12);
    dut->obs_index_i = 9;
    sim.tick();
    dut->ar_valid_i = 0;
    dut->rst_rbox_ni = 1;
    sim.check(dut->r_valid_o == 1, "RBOX reset-gated read returns RVALID");
    sim.check(dut->r_id_o == 0x1a, "RBOX reset-gated read response ID");
    sim.check(dut->r_resp_o == 2, "RBOX reset-gated read response SLVERR");
    sim.check(dut->r_data_lo_o == 0, "RBOX reset-gated read response data zero");
    sim.check(dut->apb_psel_o == 0, "RBOX reset-gated read does not issue APB");
    sim.tick();

    // Reset-gated shire-cache bank write returns a decode error and never issues APB.
    sim.check(wait_for_axi_ready(sim, true), "AW ready before shire-cache reset-gated write");
    dut->rst_shire_cache_ni = 0;
    dut->aw_valid_i = 1;
    dut->aw_id_i = 0x1b;
    dut->aw_addr_i = make_addr(3, 0, 1, 0x35);
    dut->wdata_i = 0x0badc0de12345678ull;
    dut->obs_index_i = 5;
    sim.tick();
    dut->aw_valid_i = 0;
    dut->rst_shire_cache_ni = 1;
    sim.check(dut->b_valid_o == 1, "shire-cache reset-gated write returns BVALID");
    sim.check(dut->b_id_o == 0x1b, "shire-cache reset-gated write response ID");
    sim.check(dut->b_resp_o == 2, "shire-cache reset-gated write response SLVERR");
    sim.check(dut->apb_psel_o == 0, "shire-cache reset-gated write does not issue APB");
    sim.tick();

    // Nonzero ARLEN is unsupported and returns a read decode error.
    sim.check(wait_for_axi_ready(sim, false), "AR ready before nonzero ARLEN read");
    dut->ar_valid_i = 1;
    dut->ar_id_i = 0x1c;
    dut->ar_addr_i = shire_addr;
    dut->ar_len_i = 2;
    sim.tick();
    dut->ar_valid_i = 0;
    dut->ar_len_i = 0;
    sim.check(dut->r_valid_o == 1, "nonzero ARLEN read returns RVALID");
    sim.check(dut->r_id_o == 0x1c, "nonzero ARLEN read response ID");
    sim.check(dut->r_resp_o == 2, "nonzero ARLEN read response SLVERR");
    sim.check(dut->r_data_lo_o == 0, "nonzero ARLEN read response data zero");
    sim.check(dut->apb_psel_o == 0, "nonzero ARLEN read does not issue APB");
    sim.tick();

    // Nonzero AWLEN is unsupported and returns a write decode error.
    sim.check(wait_for_axi_ready(sim, true), "AW ready before nonzero AWLEN write");
    dut->aw_valid_i = 1;
    dut->aw_id_i = 0x1d;
    dut->aw_addr_i = bank2_addr;
    dut->aw_len_i = 4;
    dut->wdata_i = 0x123456789abcdef0ull;
    sim.tick();
    dut->aw_valid_i = 0;
    dut->aw_len_i = 0;
    sim.check(dut->b_valid_o == 1, "nonzero AWLEN write returns BVALID");
    sim.check(dut->b_id_o == 0x1d, "nonzero AWLEN write response ID");
    sim.check(dut->b_resp_o == 2, "nonzero AWLEN write response SLVERR");
    sim.check(dut->apb_psel_o == 0, "nonzero AWLEN write does not issue APB");
    sim.tick();

    // Reset while APB response is held off clears in-flight state.
    defaults(dut);
    dut->apb_pready_i = 0;
    dut->ar_valid_i = 1;
    dut->ar_id_i = 0x33;
    dut->ar_addr_i = shire_addr;
    sim.tick();
    dut->ar_valid_i = 0;
    for (int i = 0; i < 4; ++i) sim.tick();
    dut->rst_ni = 0;
    sim.tick();
    sim.tick();
    dut->rst_ni = 1;
    defaults(dut);
    sim.tick();
    sim.check(dut->ar_ready_o == 1, "ready restored after in-flight reset");
    sim.check(dut->apb_psel_o == 0, "APB psel cleared after in-flight reset");

    return sim.finish();
}

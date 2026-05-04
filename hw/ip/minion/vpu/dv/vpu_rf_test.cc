// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_rf_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vvpu_rf_tb;

namespace {

constexpr int kEntries = 64;
constexpr int kRandomCycles = 2048;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

struct RefModel {
    bool use_mmi = false;
    std::array<uint32_t, kEntries> rf{};
    std::array<uint8_t, 3> rd_addr_q{};
    std::array<uint32_t, 2> wr_data_lat{};

    static uint8_t pack_addr(uint8_t thread_id, uint8_t freg_addr) {
        return static_cast<uint8_t>(((thread_id & 0x1u) << 5) | (freg_addr & 0x1fu));
    }

    void low(const DUT* dut) {
        if (!use_mmi) {
            if (dut->wr_en_early_i & 0x1u) wr_data_lat[0] = dut->wr_data_0_i;
            if (dut->wr_en_early_i & 0x2u) wr_data_lat[1] = dut->wr_data_1_i;
        }
    }

    void high(const DUT* dut) {
        const uint8_t wr_addr0 = pack_addr(dut->wr_thread_id_0_i, dut->wr_addr_0_i);
        const uint8_t wr_addr1 = pack_addr(dut->wr_thread_id_1_i, dut->wr_addr_1_i);
        const bool wr0 = (dut->wr_en_i & 0x1u) && (dut->wr_mask_i & 0x1u);
        const bool wr1 = (dut->wr_en_i & 0x2u) && (dut->wr_mask_i & 0x2u);

        if (use_mmi) {
            if (wr0) rf[wr_addr0] = dut->wr_data_0_i;
            if (wr1) rf[wr_addr1] = dut->wr_data_1_i;
        } else {
            if (wr1) rf[wr_addr1] = wr_data_lat[1];
            if (wr0) rf[wr_addr0] = wr_data_lat[0];
        }

        if (dut->rd_en_i & 0x1u) rd_addr_q[0] = pack_addr(dut->rd_thread_id_0_i, dut->rd_addr_0_i);
        if (dut->rd_en_i & 0x2u) rd_addr_q[1] = pack_addr(dut->rd_thread_id_1_i, dut->rd_addr_1_i);
        if (dut->rd_en_i & 0x4u) rd_addr_q[2] = pack_addr(dut->rd_thread_id_2_i, dut->rd_addr_2_i);
    }

    uint32_t rd_data(int port) const {
        return rf[rd_addr_q[port]];
    }
};

struct Sim {
    DUT* dut;
    RefModel ref;
    uint64_t sim_time = 0;
    int errors = 0;
    int checks = 0;
    bool compare_reads = false;

    Sim(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        dut = new DUT;
    }

    ~Sim() { delete dut; }

    void compare(const std::string& phase) {
        if (!compare_reads) return;
        check(dut->rd_data_0_o == ref.rd_data(0), phase + " rd_data_0");
        check(dut->rd_data_1_o == ref.rd_data(1), phase + " rd_data_1");
        check(dut->rd_data_2_o == ref.rd_data(2), phase + " rd_data_2");
    }

    void low(bool do_compare = true) {
        dut->clk_i = 0;
        dut->eval();
        ref.low(dut);
        if (do_compare) compare("low");
        sim_time++;
    }

    void high(bool do_compare = true) {
        dut->clk_i = 1;
        dut->eval();
        ref.high(dut);
        if (do_compare) compare("high");
        sim_time++;
    }

    void tick(bool do_compare = true) {
        low(do_compare);
        high(do_compare);
    }

    void check(bool cond, const std::string& msg) {
        checks++;
        if (!cond) {
            std::printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
            errors++;
        }
    }

    void drive_idle() {
        dut->rd_en_i = 0;
        dut->rd_thread_id_0_i = 0;
        dut->rd_thread_id_1_i = 0;
        dut->rd_thread_id_2_i = 0;
        dut->rd_addr_0_i = 0;
        dut->rd_addr_1_i = 0;
        dut->rd_addr_2_i = 0;
        dut->wr_en_i = 0;
        dut->wr_en_early_i = 0;
        dut->wr_thread_id_0_i = 0;
        dut->wr_thread_id_1_i = 0;
        dut->wr_mask_i = 0;
        dut->wr_addr_0_i = 0;
        dut->wr_addr_1_i = 0;
        dut->wr_data_0_i = 0;
        dut->wr_data_1_i = 0;
    }

    int finish() {
        std::printf("\n%d/%d checks passed\n", checks - errors, checks);
        if (errors) {
            std::printf("TEST FAILED: %d error(s)\n", errors);
            return 1;
        }
        std::printf("TEST PASSED\n");
        return 0;
    }
};

void load_read_addrs(Sim& sim, uint8_t a0, uint8_t a1, uint8_t a2) {
    sim.dut->rd_en_i = 0x7;
    sim.dut->rd_thread_id_0_i = (a0 >> 5) & 0x1u;
    sim.dut->rd_thread_id_1_i = (a1 >> 5) & 0x1u;
    sim.dut->rd_thread_id_2_i = (a2 >> 5) & 0x1u;
    sim.dut->rd_addr_0_i = a0 & 0x1fu;
    sim.dut->rd_addr_1_i = a1 & 0x1fu;
    sim.dut->rd_addr_2_i = a2 & 0x1fu;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_mask_i = 0;
    sim.tick(false);
}

void write_pair_cycle(Sim& sim,
                      uint8_t addr0, uint32_t data0, bool wr0, bool early0, bool mask0,
                      uint8_t addr1, uint32_t data1, bool wr1, bool early1, bool mask1,
                      bool compare = true) {
    sim.dut->wr_en_i = (wr0 ? 0x1u : 0x0u) | (wr1 ? 0x2u : 0x0u);
    sim.dut->wr_en_early_i = (early0 ? 0x1u : 0x0u) | (early1 ? 0x2u : 0x0u);
    sim.dut->wr_mask_i = (mask0 ? 0x1u : 0x0u) | (mask1 ? 0x2u : 0x0u);
    sim.dut->wr_thread_id_0_i = (addr0 >> 5) & 0x1u;
    sim.dut->wr_thread_id_1_i = (addr1 >> 5) & 0x1u;
    sim.dut->wr_addr_0_i = addr0 & 0x1fu;
    sim.dut->wr_addr_1_i = addr1 & 0x1fu;
    sim.dut->wr_data_0_i = data0;
    sim.dut->wr_data_1_i = data1;
    sim.low(compare);
    sim.high(compare);
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_mask_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    sim.drive_idle();
    sim.dut->clk_i = 1;
    sim.dut->eval();
    sim.ref.use_mmi = sim.dut->use_mmi_o;

    for (int i = 0; i < kEntries; i += 2) {
        const uint8_t addr0 = static_cast<uint8_t>(i);
        const uint8_t addr1 = static_cast<uint8_t>(i + 1);
        const uint32_t data0 = 0x10000000u + static_cast<uint32_t>(i);
        const uint32_t data1 = 0x20000000u + static_cast<uint32_t>(i + 1);
        write_pair_cycle(sim, addr0, data0, true, true, true, addr1, data1, true, true, true, false);
    }

    load_read_addrs(sim, 0, 1, 2);
    sim.compare_reads = true;
    sim.compare("post-init");

    for (int base = 0; base < kEntries; base += 3) {
        load_read_addrs(sim,
                        static_cast<uint8_t>(base % kEntries),
                        static_cast<uint8_t>((base + 1) % kEntries),
                        static_cast<uint8_t>((base + 2) % kEntries));
        sim.compare("read-sweep");
    }

    load_read_addrs(sim, 5, 5, 5);
    write_pair_cycle(sim, 5, 0xdeadbeefu, true, true, false, 11, 0x12345678u, false, false, false);
    sim.check(sim.dut->rd_data_0_o == sim.ref.rd_data(0), "masked port 0 write blocked");

    load_read_addrs(sim, 7, 7, 7);
    write_pair_cycle(sim, 7, 0xaaaa0001u, true, true, true, 7, 0xbbbb0002u, true, true, true);
    sim.check(sim.dut->rd_data_0_o == (sim.ref.use_mmi ? 0xbbbb0002u : 0xaaaa0001u),
              "same-address collision matches configured path");

    load_read_addrs(sim, 9, 9, 9);
    sim.dut->wr_en_i = 0x1;
    sim.dut->wr_en_early_i = 0x1;
    sim.dut->wr_mask_i = 0x1;
    sim.dut->wr_thread_id_0_i = 0;
    sim.dut->wr_addr_0_i = 9;
    sim.dut->wr_data_0_i = 0x44444444u;
    sim.low();
    sim.dut->wr_data_0_i = 0x55555555u;
    sim.high();
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_mask_i = 0;
    sim.check(sim.dut->rd_data_0_o == (sim.ref.use_mmi ? 0x55555555u : 0x44444444u),
              "write data sampling matches configured path");

    load_read_addrs(sim, 10, 10, 10);
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0x1;
    sim.dut->wr_mask_i = 0x1;
    sim.dut->wr_thread_id_0_i = 0;
    sim.dut->wr_addr_0_i = 10;
    sim.dut->wr_data_0_i = 0x66666666u;
    sim.tick();
    sim.dut->wr_en_i = 0x1;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_mask_i = 0x1;
    sim.dut->wr_data_0_i = 0x77777777u;
    sim.tick();
    sim.dut->wr_en_i = 0;
    sim.dut->wr_en_early_i = 0;
    sim.dut->wr_mask_i = 0;
    sim.check(sim.dut->rd_data_0_o == (sim.ref.use_mmi ? 0x77777777u : 0x66666666u),
              "early-write handling matches configured path");

    uint32_t seed = sim.ref.use_mmi ? 0x4d4d0001u : 0x4c4c0001u;
    for (int i = 0; i < kRandomCycles; ++i) {
        sim.dut->rd_en_i = static_cast<uint8_t>(xorshift32(seed) & 0x7u);
        sim.dut->rd_thread_id_0_i = xorshift32(seed) & 0x1u;
        sim.dut->rd_thread_id_1_i = xorshift32(seed) & 0x1u;
        sim.dut->rd_thread_id_2_i = xorshift32(seed) & 0x1u;
        sim.dut->rd_addr_0_i = xorshift32(seed) & 0x1fu;
        sim.dut->rd_addr_1_i = xorshift32(seed) & 0x1fu;
        sim.dut->rd_addr_2_i = xorshift32(seed) & 0x1fu;
        sim.dut->wr_en_i = static_cast<uint8_t>(xorshift32(seed) & 0x3u);
        sim.dut->wr_en_early_i = static_cast<uint8_t>(xorshift32(seed) & 0x3u);
        sim.dut->wr_thread_id_0_i = xorshift32(seed) & 0x1u;
        sim.dut->wr_thread_id_1_i = xorshift32(seed) & 0x1u;
        sim.dut->wr_mask_i = static_cast<uint8_t>(xorshift32(seed) & 0x3u);
        sim.dut->wr_addr_0_i = xorshift32(seed) & 0x1fu;
        sim.dut->wr_addr_1_i = xorshift32(seed) & 0x1fu;
        sim.dut->wr_data_0_i = xorshift32(seed);
        sim.dut->wr_data_1_i = xorshift32(seed);
        sim.low();
        if (xorshift32(seed) & 0x1u) sim.dut->wr_data_0_i = xorshift32(seed);
        if (xorshift32(seed) & 0x1u) sim.dut->wr_data_1_i = xorshift32(seed);
        sim.high();
    }

    return sim.finish();
}

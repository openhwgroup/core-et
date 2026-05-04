// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_rf_mmi_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <string>

using DUT = Vcosim_vpu_rf_mmi_tb;

namespace {

constexpr int kEntries = 64;
constexpr int kRandomCycles = 8192;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

struct Sim {
    DUT* dut;
    uint64_t sim_time = 0;
    int errors = 0;
    int mismatches = 0;
    int comparisons = 0;
    bool compare_reads = false;

    Sim(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        dut = new DUT;
    }

    ~Sim() { delete dut; }

    void compare(const std::string& phase) {
        if (!compare_reads) return;
        compare_one("rd_data_0", phase, dut->orig_rd_data_0_o, dut->new_rd_data_0_o);
        compare_one("rd_data_1", phase, dut->orig_rd_data_1_o, dut->new_rd_data_1_o);
        compare_one("rd_data_2", phase, dut->orig_rd_data_2_o, dut->new_rd_data_2_o);
    }

    void compare_one(const char* name, const std::string& phase, uint32_t orig, uint32_t repl) {
        comparisons++;
        if (orig != repl) {
          mismatches++;
          std::printf("  MISMATCH [t=%lu] %s %s: orig=0x%08x new=0x%08x\n",
                      (unsigned long)sim_time, phase.c_str(), name, orig, repl);
        }
    }

    void low(bool do_compare = true) {
        dut->clk_i = 0;
        dut->eval();
        if (do_compare) compare("low");
        sim_time++;
    }

    void high(bool do_compare = true) {
        dut->clk_i = 1;
        dut->eval();
        if (do_compare) compare("high");
        sim_time++;
    }

    void tick(bool do_compare = true) {
        low(do_compare);
        high(do_compare);
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
        std::printf("\n%d comparisons, %d mismatches\n", comparisons, mismatches);
        if (errors || mismatches) {
            std::printf("COSIM FAILED: %d mismatch(es), %d error(s)\n", mismatches, errors);
            return 1;
        }
        std::printf("COSIM PASSED\n");
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
                      bool do_compare = true) {
    sim.dut->wr_en_i = (wr0 ? 0x1u : 0x0u) | (wr1 ? 0x2u : 0x0u);
    sim.dut->wr_en_early_i = (early0 ? 0x1u : 0x0u) | (early1 ? 0x2u : 0x0u);
    sim.dut->wr_mask_i = (mask0 ? 0x1u : 0x0u) | (mask1 ? 0x2u : 0x0u);
    sim.dut->wr_thread_id_0_i = (addr0 >> 5) & 0x1u;
    sim.dut->wr_thread_id_1_i = (addr1 >> 5) & 0x1u;
    sim.dut->wr_addr_0_i = addr0 & 0x1fu;
    sim.dut->wr_addr_1_i = addr1 & 0x1fu;
    sim.dut->wr_data_0_i = data0;
    sim.dut->wr_data_1_i = data1;
    sim.low(do_compare);
    sim.high(do_compare);
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

    for (int i = 0; i < kEntries; i += 2) {
        write_pair_cycle(sim,
                         static_cast<uint8_t>(i),     0x30000000u + static_cast<uint32_t>(i),     true, true, true,
                         static_cast<uint8_t>(i + 1), 0x40000000u + static_cast<uint32_t>(i + 1), true, true, true,
                         false);
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

    load_read_addrs(sim, 7, 7, 7);
    write_pair_cycle(sim, 7, 0xaaaa0001u, true, true, true, 7, 0xbbbb0002u, true, true, true);

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

    uint32_t seed = 0x56505502u;
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

    if (sim.compare_reads) {
        sim.errors += !(sim.comparisons >= 25000);
        if (sim.comparisons < 25000) {
            std::printf("  FAIL: vpu_rf_mmi cosim should exceed 25k comparisons\n");
        }
    }
    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncacheable_flb_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <string>

static uint32_t xorshift32(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

template <typename T, typename U>
static void compare_words(CosimCtrl<Vcosim_uncacheable_flb_tb>& sim,
                          const char* name, const T& orig, const U& nw, int n) {
    for (int i = 0; i < n; ++i) {
        sim.compare(std::string(name) + "[" + std::to_string(i) + "]", orig[i], nw[i]);
    }
}

static void compare_all(CosimCtrl<Vcosim_uncacheable_flb_tb>& sim) {
    compare_words(sim, "esr_barrier_values", sim.dut->orig_esr_barrier_values_bits_o,
                  sim.dut->new_esr_barrier_values_bits_o, 8);
    sim.compare("flb_resp_valid", sim.dut->orig_flb_l2_neigh_resp_valid_o,
                sim.dut->new_flb_l2_neigh_resp_valid_o);
    sim.compare("flb_resp_data", sim.dut->orig_flb_l2_neigh_resp_data_o,
                sim.dut->new_flb_l2_neigh_resp_data_o);
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncacheable_flb_tb> sim(argc, argv);
    sim.dut->esr_enables_bits_i = 0;
    sim.dut->esr_wdata_i = 0;
    sim.dut->flb_neigh_l2_req_valid_i = 0;
    sim.dut->flb_neigh_l2_req_data_bits_i = 0;
    sim.reset();
    compare_all(sim);

    // Initialize several barrier limits through ESR writes.
    for (int b = 0; b < 4; ++b) {
        sim.dut->esr_enables_bits_i = 1u << b;
        sim.dut->esr_wdata_i = 2 + b;
        sim.tick();
        compare_all(sim);
    }
    sim.dut->esr_enables_bits_i = 0;

    uint32_t seed = 0xf1b123u;
    for (int cyc = 0; cyc < 4096; ++cyc) {
        uint32_t r = (cyc < 1024) ? static_cast<uint32_t>(cyc) : xorshift32(seed);
        sim.dut->flb_neigh_l2_req_valid_i = r & 0xfu;
        // Four packed barrier requests: low bits carry limit and barrier-number in the
        // same packed layout used by both original and translated structs.
        uint64_t data = 0;
        for (int n = 0; n < 4; ++n) {
            uint64_t req = ((r >> (n * 3)) & 0xffu);
            if ((cyc & 63) < 8) req = (uint64_t)(2 + ((cyc + n) & 3));
            data |= req << (n * 16);
        }
        sim.dut->flb_neigh_l2_req_data_bits_i = data;
        // Interleave ESR writes with requests to cover the clock gate and update masks.
        if ((cyc & 127) == 17) {
            sim.dut->esr_enables_bits_i = 1u << ((cyc >> 7) & 3);
            sim.dut->esr_wdata_i = (cyc >> 2) & 0xffu;
        } else {
            sim.dut->esr_enables_bits_i = 0;
            sim.dut->esr_wdata_i = xorshift32(seed);
        }
        sim.tick();
        compare_all(sim);
    }

    // Drain pending FIFO contents.
    sim.dut->esr_enables_bits_i = 0;
    sim.dut->flb_neigh_l2_req_valid_i = 0;
    for (int i = 0; i < 64; ++i) {
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

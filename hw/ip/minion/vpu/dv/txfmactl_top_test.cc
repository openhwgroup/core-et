// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


#include "Vtxfmactl_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vtxfmactl_top_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int input_words(SimCtrl<DUT>& sim) {
    return static_cast<int>(sizeof(sim.dut->inputs_i) / sizeof(sim.dut->inputs_i[0]));
}

int output_words(SimCtrl<DUT>& sim) {
    return static_cast<int>(sizeof(sim.dut->outputs_o) / sizeof(sim.dut->outputs_o[0]));
}

std::vector<uint32_t> get_outputs(SimCtrl<DUT>& sim) {
    const int words = output_words(sim);
    std::vector<uint32_t> out(words);
    for (int i = 0; i < words; ++i) out[i] = sim.dut->outputs_o[i];
    return out;
}

void drive_words(SimCtrl<DUT>& sim, const std::vector<uint32_t>& words) {
    const int n = input_words(sim);
    for (int i = 0; i < n; ++i) sim.dut->inputs_i[i] = (i < static_cast<int>(words.size())) ? words[i] : 0u;
}

std::vector<uint32_t> filled_words(SimCtrl<DUT>& sim, uint32_t value) {
    return std::vector<uint32_t>(input_words(sim), value);
}

std::vector<uint32_t> random_words(SimCtrl<DUT>& sim, uint32_t seed) {
    std::vector<uint32_t> words(input_words(sim));
    uint32_t rng = seed;
    for (uint32_t& word : words) word = xorshift32(rng);
    return words;
}

void check_words(SimCtrl<DUT>& sim, const std::vector<uint32_t>& actual,
                 const std::vector<uint32_t>& expected, const std::string& label) {
    sim.check(actual.size() == expected.size(), label + " word count");
    const size_t n = actual.size() < expected.size() ? actual.size() : expected.size();
    for (size_t i = 0; i < n; ++i) {
        char msg[160];
        std::snprintf(msg, sizeof(msg), "%s output word %zu expected 0x%08x got 0x%08x",
                      label.c_str(), i, expected[i], actual[i]);
        sim.check(actual[i] == expected[i], msg);
    }
}

// Packed-output golden vectors for all-zero, all-one, and deterministic random stimuli.
const std::vector<std::vector<uint32_t>> kExpected = {
        {0x00640000u, 0x00000000u, 0x00100000u, 0x06000000u, 0x02d01819u, 0x18000000u, 0x5000000cu, 0x00000004u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
        {0x0065d5d1u, 0x17000000u, 0x00100000u, 0x44000000u, 0x00100019u, 0x18000000u, 0x1000000cu, 0x00000204u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
        {0x0065d5d1u, 0x37000000u, 0x00104000u, 0x44000000u, 0x00100019u, 0x18000000u, 0x5000000cu, 0x00000004u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
        {0x0065c541u, 0x75320000u, 0x00104000u, 0x45000000u, 0x00100019u, 0x18000000u, 0x1000000cu, 0x00000204u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
        {0x0065d5d1u, 0xf7000000u, 0x00100000u, 0x44000000u, 0x00100019u, 0x18000000u, 0x5000000cu, 0x00000004u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
        {0x0001d5d1u, 0xf7c09301u, 0x00100801u, 0x04000000u, 0x00100019u, 0x18000000u, 0x1000000cu, 0x00000204u, 0x08000000u, 0x02804600u, 0x00000000u, 0x00000000u},
    };

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    sim.dut->rst_ni = 0;
    drive_words(sim, filled_words(sim, 0u));
    sim.tick();
    sim.dut->rst_ni = 1;
    Verilated::assertOn(true);

    const std::vector<std::vector<uint32_t>> stimuli = {
        filled_words(sim, 0u),
        filled_words(sim, 0xffff'ffffu),
        random_words(sim, 0x1234'5678u),
        random_words(sim, 0x8765'4321u),
        random_words(sim, 0xdead'beefu),
        random_words(sim, 0x0bad'f00du),
    };

    std::vector<std::vector<uint32_t>> observed;
    for (size_t i = 0; i < stimuli.size(); ++i) {
        drive_words(sim, stimuli[i]);
        sim.tick();
        observed.push_back(get_outputs(sim));
    }

    bool dump = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "+dump-golden") dump = true;
    }
    if (dump) {
        std::printf("GOLDEN txfmactl_top BEGIN\n");
        for (const auto& vec : observed) {
            std::printf("{");
            for (size_t j = 0; j < vec.size(); ++j) {
                std::printf("0x%08xu%s", vec[j], (j + 1 == vec.size()) ? "" : ", ");
            }
            std::printf("},\n");
        }
        std::printf("GOLDEN txfmactl_top END\n");
    }

    sim.check(kExpected.size() == observed.size(), "golden vector count");
    if (kExpected.size() == observed.size()) {
        for (size_t i = 0; i < observed.size(); ++i) {
            check_words(sim, observed[i], kExpected[i], "directed vector " + std::to_string(i));
        }
    }
    return sim.finish();
}

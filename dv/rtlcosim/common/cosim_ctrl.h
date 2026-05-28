// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// cosim_ctrl.h — Co-simulation harness.
//
// Extends SimCtrl with helpers for comparing original vs. new module outputs.
// The SV testbench exposes both modules' outputs; C++ checks them each cycle.
//
// Usage:
//   #include "Vcosim_fifo_sram_tb.h"
//   #include "cosim_ctrl.h"
//   int main(int argc, char** argv) {
//     CosimCtrl<Vcosim_fifo_sram_tb> sim(argc, argv);
//     sim.reset();
//     // drive inputs, call sim.tick()
//     // use sim.compare("signal", orig_val, new_val) each cycle
//     return sim.finish();
//   }

#pragma once

#include "sim_ctrl.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

template <typename DUT>
class CosimCtrl : public SimCtrl<DUT> {
public:
    int mismatches = 0;
    int comparisons = 0;
    bool verbose = false;

    CosimCtrl(int argc, char** argv) : SimCtrl<DUT>(argc, argv) {
        // Check for verbose flag.
        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "+verbose") verbose = true;
        }
    }

    // Compare two scalar values. Logs on mismatch.
    template <typename T>
    void compare(const std::string& name, T orig, T newv) {
        comparisons++;
        if (orig != newv) {
            mismatches++;
            printf("  MISMATCH [t=%lu] %s: orig=0x%llx new=0x%llx\n",
                   (unsigned long)this->sim_time, name.c_str(),
                   (unsigned long long)(uint64_t)orig,
                   (unsigned long long)(uint64_t)newv);
        } else if (verbose) {
            printf("  ok [t=%lu] %s = 0x%llx\n",
                   (unsigned long)this->sim_time, name.c_str(),
                   (unsigned long long)(uint64_t)orig);
        }
    }

    // Compare a Verilator wide packed vector. This preserves all bits for
    // flattened RTL structs and arrays wider than 64 bits.
    template <std::size_t NWords>
    void compare(const std::string& name,
                 const VlWide<NWords>& orig,
                 const VlWide<NWords>& newv) {
        comparisons++;
        if (orig != newv) {
            mismatches++;
            const std::string orig_s = VL_TO_STRING(orig);
            const std::string new_s = VL_TO_STRING(newv);
            printf("  MISMATCH [t=%lu] %s: orig=0x%s new=0x%s\n",
                   (unsigned long)this->sim_time, name.c_str(),
                   orig_s.c_str(), new_s.c_str());
        } else if (verbose) {
            const std::string val_s = VL_TO_STRING(orig);
            printf("  ok [t=%lu] %s = 0x%s\n",
                   (unsigned long)this->sim_time, name.c_str(), val_s.c_str());
        }
    }

    int finish() {
#if VM_COVERAGE
        VerilatedCov::write();
#endif
        printf("\n%d comparisons, %d mismatches\n", comparisons, mismatches);
        if (mismatches) {
            printf("COSIM FAILED: %d mismatch(es)\n", mismatches);
            return 1;
        }
        // Also report any check() failures from base class.
        if (this->errors) {
            printf("COSIM FAILED: %d error(s)\n", this->errors);
            return 1;
        }
        printf("COSIM PASSED\n");
        return 0;
    }
};

// Co-simulation test: intpipe_csr_pmu_read_interface (new) vs original.
//
// Exercises the PMU counter read interface: address decoding for
// MHPMCOUNTER3..8 and HPMCOUNTER3..8, per-thread read delay propagation,
// ready generation, concurrent thread reads, and random stimulus.

#include "Vcosim_pmu_read_interface_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_pmu_read_interface_tb;

static uint32_t rng_state = 0xBADCAFE;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("io_rw_mem_ready",  sim.dut->orig_io_rw_mem_ready,  sim.dut->new_io_rw_mem_ready);
    sim.compare("pmu_read_sel_0",   sim.dut->orig_pmu_read_sel_0,   sim.dut->new_pmu_read_sel_0);
    sim.compare("pmu_read_sel_1",   sim.dut->orig_pmu_read_sel_1,   sim.dut->new_pmu_read_sel_1);
    sim.compare("pmu_read_en_wb",   sim.dut->orig_pmu_read_en_wb,   sim.dut->new_pmu_read_en_wb);
}

static void idle_inputs(DUT* d) {
    d->stim_io_rw_mem_valid     = 0;
    d->stim_io_rw_mem_thread_id = 0;
    d->stim_io_rw_mem_addr      = 0;
}

// CSR address constants (matching the enum in intpipe_csr_pkg / soc.vh)
static const uint16_t MHPMCOUNTER3 = 0xB03;
static const uint16_t MHPMCOUNTER8 = 0xB08;
static const uint16_t HPMCOUNTER3  = 0xC03;
static const uint16_t HPMCOUNTER8  = 0xC08;

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.reset(5);
    idle_inputs(sim.dut.get());

    // Phase 1: Post-reset idle — outputs should be zero / idle
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Phase 2: Targeted reads to each valid address, both threads
    // MHPMCOUNTER3..MHPMCOUNTER8 (0xB03..0xB08)
    for (uint16_t addr = MHPMCOUNTER3; addr <= MHPMCOUNTER8; addr++) {
        for (int tid = 0; tid < 2; tid++) {
            idle_inputs(sim.dut.get());
            sim.dut->stim_io_rw_mem_valid     = 1;
            sim.dut->stim_io_rw_mem_thread_id = tid;
            sim.dut->stim_io_rw_mem_addr      = addr;
            sim.tick();
            compare_all(sim);

            // Let the delay pipeline drain (PmuCountersReadDelay=3 + margin)
            idle_inputs(sim.dut.get());
            for (int w = 0; w < 6; w++) {
                sim.tick();
                compare_all(sim);
            }
        }
    }

    // HPMCOUNTER3..HPMCOUNTER8 (0xC03..0xC08)
    for (uint16_t addr = HPMCOUNTER3; addr <= HPMCOUNTER8; addr++) {
        for (int tid = 0; tid < 2; tid++) {
            idle_inputs(sim.dut.get());
            sim.dut->stim_io_rw_mem_valid     = 1;
            sim.dut->stim_io_rw_mem_thread_id = tid;
            sim.dut->stim_io_rw_mem_addr      = addr;
            sim.tick();
            compare_all(sim);

            idle_inputs(sim.dut.get());
            for (int w = 0; w < 6; w++) {
                sim.tick();
                compare_all(sim);
            }
        }
    }

    // Phase 3: Test read delay propagation
    // Issue a valid read, then verify ready comes after PmuCountersReadDelay=3 cycles
    for (int tid = 0; tid < 2; tid++) {
        // First, idle long enough to clear any state
        idle_inputs(sim.dut.get());
        for (int i = 0; i < 10; i++) {
            sim.tick();
            compare_all(sim);
        }

        // Issue read to a new counter (different from current sel to force delay)
        sim.dut->stim_io_rw_mem_valid     = 1;
        sim.dut->stim_io_rw_mem_thread_id = tid;
        sim.dut->stim_io_rw_mem_addr      = MHPMCOUNTER3 + (tid ? 2 : 0);
        sim.tick();
        compare_all(sim);

        // Watch the delay propagation
        idle_inputs(sim.dut.get());
        for (int i = 0; i < 8; i++) {
            sim.tick();
            compare_all(sim);
        }

        // Now read same counter again (should be ready immediately via current_sel path)
        sim.dut->stim_io_rw_mem_valid     = 1;
        sim.dut->stim_io_rw_mem_thread_id = tid;
        sim.dut->stim_io_rw_mem_addr      = MHPMCOUNTER3 + (tid ? 2 : 0);
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        for (int i = 0; i < 4; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 4: Concurrent thread reads — alternate between threads
    for (int round = 0; round < 20; round++) {
        uint32_t r = xorshift32();
        int tid   = round & 1;
        uint16_t base = (r & 1) ? HPMCOUNTER3 : MHPMCOUNTER3;
        uint16_t off  = (r >> 1) % 6;  // 0..5 for counter3..counter8

        idle_inputs(sim.dut.get());
        sim.dut->stim_io_rw_mem_valid     = 1;
        sim.dut->stim_io_rw_mem_thread_id = tid;
        sim.dut->stim_io_rw_mem_addr      = base + off;
        sim.tick();
        compare_all(sim);

        // Drain some cycles
        idle_inputs(sim.dut.get());
        int drain = 1 + (xorshift32() % 5);
        for (int w = 0; w < drain; w++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Test out-of-range addresses — should not trigger any valid read
    {
        uint16_t oob_addrs[] = {0x000, 0x100, 0xB02, 0xB09, 0xC02, 0xC09, 0xFFF};
        for (auto addr : oob_addrs) {
            idle_inputs(sim.dut.get());
            sim.dut->stim_io_rw_mem_valid = 1;
            sim.dut->stim_io_rw_mem_addr  = addr;
            sim.dut->stim_io_rw_mem_thread_id = xorshift32() & 1;
            sim.tick();
            compare_all(sim);

            idle_inputs(sim.dut.get());
            for (int w = 0; w < 4; w++) {
                sim.tick();
                compare_all(sim);
            }
        }
    }

    // Phase 5: Random stimulus — ~5000 cycles
    for (int i = 0; i < 5000; i++) {
        uint32_t r = xorshift32();
        sim.dut->stim_io_rw_mem_valid     = (r >> 0) & 1;
        sim.dut->stim_io_rw_mem_thread_id = (r >> 1) & 1;
        // Mix valid PMU addresses with random addresses
        if ((r >> 2) & 1) {
            // Pick a valid PMU address
            uint32_t r2 = xorshift32();
            uint16_t base = (r2 & 1) ? HPMCOUNTER3 : MHPMCOUNTER3;
            uint16_t off  = (r2 >> 1) % 6;
            sim.dut->stim_io_rw_mem_addr = base + off;
        } else {
            // Random 12-bit address
            sim.dut->stim_io_rw_mem_addr = (r >> 3) & 0xFFF;
        }
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

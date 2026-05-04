// Co-simulation test: intpipe_csr_file_conv (new) vs original.
//
// Exercises convolution boundary computation:
// Phase 1: post-reset idle
// Phase 2: trigger conv computations with various size/ctrl values
// Phase 3: test pending behavior (tensorload_wen while computing)
// Phase 4: random stimulus ~5000 cycles

#include "Vcosim_conv_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_conv_tb;

static uint32_t rng_state = 0xDEAD1234;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    return ((uint64_t)xorshift32() << 32) | xorshift32();
}

static void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("can_accept_new_write", sim.dut->orig_can_accept_new_write, sim.dut->new_can_accept_new_write);
    sim.compare("conv_save_mask",       sim.dut->orig_conv_save_mask,       sim.dut->new_conv_save_mask);
    sim.compare("conv_bits_ready",      sim.dut->orig_conv_bits_ready,      sim.dut->new_conv_bits_ready);
    sim.compare("conv_bits",            sim.dut->orig_conv_bits,            sim.dut->new_conv_bits);
}

static void idle_inputs(DUT* d) {
    d->stim_tensorconv_size_data = 0;
    d->stim_tensorconv_ctrl_data = 0;
    d->stim_tensorconv_size_wen  = 0;
    d->stim_tensorconv_ctrl_wen  = 0;
    d->stim_tensorload_wen       = 0;
    d->stim_tensorfma_wen        = 0;
    d->stim_cacheop_wen          = 0;
}

// Build a tensorconv_size packed struct as a 64-bit value.
// Layout (MSB to LSB):
//   row_step_offset[7:0], padding2[7:0], row_size[15:0],
//   col_step_offset[7:0], padding1[7:0], col_size[15:0]
static uint64_t make_size(uint8_t row_step, uint16_t row_size,
                          uint8_t col_step, uint16_t col_size) {
    uint64_t v = 0;
    v |= (uint64_t)row_step << 56;
    v |= (uint64_t)0       << 48; // padding2
    v |= (uint64_t)row_size << 32;
    v |= (uint64_t)col_step << 24;
    v |= (uint64_t)0        << 16; // padding1
    v |= (uint64_t)col_size;
    return v;
}

// Build a tensorconv_control packed struct as a 48-bit value.
// Layout (MSB to LSB):
//   row_pos[15:0], padding1[15:0], col_pos[15:0]
static uint64_t make_ctrl(uint16_t row_pos, uint16_t col_pos) {
    uint64_t v = 0;
    v |= (uint64_t)row_pos << 32;
    v |= (uint64_t)0       << 16; // padding1
    v |= (uint64_t)col_pos;
    return v;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.reset(5);
    idle_inputs(sim.dut.get());

    // Phase 1: Post-reset idle — should show compute_done=1, no pending
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Phase 2: Trigger conv computations with various size/ctrl values
    // Each computation takes 16 cycles (conv_idx 0..15)
    for (int txn = 0; txn < 15; txn++) {
        uint16_t col_size = (xorshift32() % 64) + 1;
        uint16_t row_size = (xorshift32() % 64) + 1;
        uint8_t  col_step = xorshift32() & 0xFF;
        uint8_t  row_step = xorshift32() & 0xFF;
        int16_t  col_pos  = (int16_t)(xorshift32() & 0xFFFF);
        int16_t  row_pos  = (int16_t)(xorshift32() & 0xFFFF);

        // Set size data and trigger via size_wen
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(row_step, row_size, col_step, col_size);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl((uint16_t)row_pos, (uint16_t)col_pos);
        sim.dut->stim_tensorconv_size_wen  = 1;
        sim.tick();
        compare_all(sim);

        // Let computation run for 20 cycles (16 needed + margin)
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(row_step, row_size, col_step, col_size);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl((uint16_t)row_pos, (uint16_t)col_pos);
        for (int c = 0; c < 20; c++) {
            sim.tick();
            compare_all(sim);
        }

        // Verify idle after computation
        for (int i = 0; i < 3; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 2b: Trigger via ctrl_wen
    for (int txn = 0; txn < 5; txn++) {
        uint16_t col_size = (xorshift32() % 32) + 1;
        uint16_t row_size = (xorshift32() % 32) + 1;
        uint8_t  col_step = xorshift32() & 0x0F;
        uint8_t  row_step = xorshift32() & 0x0F;
        int16_t  col_pos  = (int16_t)(xorshift32() & 0x1F);
        int16_t  row_pos  = (int16_t)(xorshift32() & 0x1F);

        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(row_step, row_size, col_step, col_size);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl((uint16_t)row_pos, (uint16_t)col_pos);
        sim.dut->stim_tensorconv_ctrl_wen  = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(row_step, row_size, col_step, col_size);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl((uint16_t)row_pos, (uint16_t)col_pos);
        for (int c = 0; c < 20; c++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 3: Test pending behavior — tensorload_wen while computing
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(2, 10, 3, 12);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(1, 1);
        sim.dut->stim_tensorconv_size_wen  = 1;
        sim.tick();
        compare_all(sim);

        // While computing, assert tensorload_wen to create pending
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(2, 10, 3, 12);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(1, 1);
        sim.dut->stim_tensorload_wen = 1;
        sim.tick();
        compare_all(sim);

        // Pending should block new writes
        sim.dut->stim_tensorload_wen = 0;
        for (int c = 0; c < 20; c++) {
            sim.tick();
            compare_all(sim);
        }

        // After compute_done, pending clears
        for (int i = 0; i < 5; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 3b: tensorfma_wen while computing
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 8, 1, 8);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(0, 0);
        sim.dut->stim_tensorconv_ctrl_wen  = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 8, 1, 8);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(0, 0);
        sim.dut->stim_tensorfma_wen = 1;
        sim.tick();
        compare_all(sim);

        sim.dut->stim_tensorfma_wen = 0;
        for (int c = 0; c < 25; c++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 3c: cacheop_wen while computing
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 4, 1, 4);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(0, 0);
        sim.dut->stim_tensorconv_size_wen  = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 4, 1, 4);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(0, 0);
        sim.dut->stim_cacheop_wen = 1;
        sim.tick();
        compare_all(sim);

        sim.dut->stim_cacheop_wen = 0;
        for (int c = 0; c < 25; c++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 3d: Try to re-trigger while pending (should be blocked)
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 16, 1, 16);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(2, 2);
        sim.dut->stim_tensorconv_size_wen  = 1;
        sim.tick();
        compare_all(sim);

        // Create pending
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 16, 1, 16);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(2, 2);
        sim.dut->stim_tensorload_wen = 1;
        sim.tick();
        compare_all(sim);

        // Let computation finish
        sim.dut->stim_tensorload_wen = 0;
        for (int c = 0; c < 20; c++) {
            sim.tick();
            compare_all(sim);
        }

        // Now pending should be set; try to trigger (should be blocked)
        sim.dut->stim_tensorconv_size_wen = 1;
        sim.tick();
        compare_all(sim);

        // Pending clears after compute_done
        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 16, 1, 16);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(2, 2);
        for (int c = 0; c < 5; c++) {
            sim.tick();
            compare_all(sim);
        }

        // Now should be able to trigger again
        sim.dut->stim_tensorconv_size_wen = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.dut->stim_tensorconv_size_data = make_size(1, 16, 1, 16);
        sim.dut->stim_tensorconv_ctrl_data = make_ctrl(2, 2);
        for (int c = 0; c < 25; c++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 4: Random stimulus ~5000 cycles
    for (int i = 0; i < 5000; i++) {
        uint32_t r1 = xorshift32();
        uint32_t r2 = xorshift32();

        sim.dut->stim_tensorconv_size_data = xorshift64();
        sim.dut->stim_tensorconv_ctrl_data = xorshift64() & 0xFFFFFFFFFFFFULL;
        sim.dut->stim_tensorconv_size_wen  = (r1 >> 0) & 1;
        sim.dut->stim_tensorconv_ctrl_wen  = (r1 >> 1) & 1;
        sim.dut->stim_tensorload_wen       = (r1 >> 2) & 1;
        sim.dut->stim_tensorfma_wen        = (r1 >> 3) & 1;
        sim.dut->stim_cacheop_wen          = (r1 >> 4) & 1;
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

// Co-simulation test: intpipe_csr_replay (new) vs original.
//
// Exercises replay conditions, stall clearing, tensor wait replays,
// interrupt/redirect clearing, and random stimulus.

#include "Vcosim_replay_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_replay_tb;

static uint32_t rng_state = 0xDEADBEEF;
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
    sim.compare("io_replay",       sim.dut->orig_io_replay,       sim.dut->new_io_replay);
    sim.compare("io_replay_stall", sim.dut->orig_io_replay_stall, sim.dut->new_io_replay_stall);
}

static void idle_inputs(DUT* d) {
    d->stim_wb_wdata                      = 0;
    d->stim_wb_thread_id                  = 0;
    d->stim_io_interrupt                  = 0;
    d->stim_io_redirect                   = 0;
    d->stim_reg_co_evict_va_en            = 0;
    d->stim_reg_co_flush_va_en            = 0;
    d->stim_reg_co_lock_va_en_qual        = 0;
    d->stim_reg_co_unlock_va_en_qual      = 0;
    d->stim_reg_co_lock_sw_en             = 0;
    d->stim_reg_co_unlock_sw_en           = 0;
    d->stim_reg_co_prefetch_va_en         = 0;
    d->stim_reg_co_evict_sw_en            = 0;
    d->stim_reg_co_flush_sw_en            = 0;
    d->stim_reg_tensor_load_l2scp_en      = 0;
    d->stim_reg_flb_en                    = 0;
    d->stim_reg_tensor_load_en            = 0;
    d->stim_reg_tensor_fma_en             = 0;
    d->stim_reg_tensor_quant_en           = 0;
    d->stim_reg_tensor_reduce_en          = 0;
    d->stim_reg_tensor_store_en           = 0;
    d->stim_reg_tensor_conv_ctrl_en       = 0;
    d->stim_reg_tensor_conv_size_en       = 0;
    d->stim_reg_fcc_en                    = 0;
    d->stim_reg_tensor_wait_en            = 0;
    d->stim_reg_tex_send_en               = 0;
    d->stim_reg_tensor_mask_read_invalid  = 0;
    d->stim_pmu_read_en_wb                = 0;
    d->stim_pmu_read_mem_ready            = 0x3; // all ready
    d->stim_dcache_ctrl_flat              = 0x7FFFFF; // all bits high = all ready/complete
    d->stim_fl_barrier_ready              = 1;
    d->stim_tensor_fma_replay_condition   = 0;
    d->stim_tensor_quant_replay_condition = 0;
    d->stim_vpu_treduce_enabled           = 0;
    d->stim_tenb_tl_pend_tfma_en          = 0;
    d->stim_tenb_restart_process          = 0;
    d->stim_tload_is_for_tenb             = 0;
    d->stim_conv_can_accept_new_write     = 0x3;
    d->stim_fcc_ready                     = 0xF; // all ready
    d->stim_wb_fcc_addr                   = 0;
    d->stim_msg_port_pget_block           = 0;
    d->stim_msg_port_stall_flat           = 0;
    d->stim_reg_tensor_mask_ready         = 0x3;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000; // generous limit for many cycles
    sim.reset(5);
    idle_inputs(sim.dut.get());

    // ── Phase 1: Post-reset idle ─────────────────────────
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 2: Individual replay conditions ────────────
    // For each enable + blocking condition, verify io_replay fires.
    // After each test, clear with idle + enough cycles.

    auto test_replay = [&](auto set_fn) {
        idle_inputs(sim.dut.get());
        sim.tick(); compare_all(sim); // clean cycle
        set_fn(sim.dut.get());
        sim.tick(); compare_all(sim); // replay should fire
        idle_inputs(sim.dut.get());
        // let stall propagate and clear
        for (int i = 0; i < 5; i++) {
            sim.tick(); compare_all(sim);
        }
    };

    // CACHE_OP: evict_va with cache_op_ready=0
    test_replay([](DUT* d) {
        d->stim_reg_co_evict_va_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 22); // clear cache_op_ready (MSB of struct)
    });

    // CACHE_OP: flush_va
    test_replay([](DUT* d) {
        d->stim_reg_co_flush_va_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 22);
    });

    // CACHE_OP: lock_sw
    test_replay([](DUT* d) {
        d->stim_reg_co_lock_sw_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 22);
    });

    // CACHE_OP: prefetch_va
    test_replay([](DUT* d) {
        d->stim_reg_co_prefetch_va_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 22);
    });

    // CACHE_OP: tensor_load_l2scp
    test_replay([](DUT* d) {
        d->stim_reg_tensor_load_l2scp_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 22);
    });

    // FLB: fl_barrier not ready
    test_replay([](DUT* d) {
        d->stim_reg_flb_en = 1;
        d->stim_fl_barrier_ready = 0;
    });

    // TL0: tensor_load, tl_ready_0=0, not for tenb
    test_replay([](DUT* d) {
        d->stim_reg_tensor_load_en = 1;
        d->stim_tload_is_for_tenb = 0;
        d->stim_dcache_ctrl_flat &= ~(1u << 12); // clear tl_ready_0
    });

    // TL1: tensor_load, tl_ready_1=0, for tenb
    test_replay([](DUT* d) {
        d->stim_reg_tensor_load_en = 1;
        d->stim_tload_is_for_tenb = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 11); // clear tl_ready_1
    });

    // TFMA: tensor_fma_replay_condition
    test_replay([](DUT* d) {
        d->stim_reg_tensor_fma_en = 1;
        d->stim_tensor_fma_replay_condition = 1;
    });

    // TQUANT
    test_replay([](DUT* d) {
        d->stim_reg_tensor_quant_en = 1;
        d->stim_tensor_quant_replay_condition = 1;
    });

    // TREDUCE: reduce_ready=0
    test_replay([](DUT* d) {
        d->stim_reg_tensor_reduce_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 8); // clear reduce_ready
    });

    // TS: ts_ready=0
    test_replay([](DUT* d) {
        d->stim_reg_tensor_store_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 10); // clear ts_ready
    });

    // CONV: conv_ctrl_en, can't accept
    test_replay([](DUT* d) {
        d->stim_reg_tensor_conv_ctrl_en = 1;
        d->stim_conv_can_accept_new_write = 0; // neither thread can accept
    });

    // FCC0: fcc_en, not ready, wdata bit0=0
    test_replay([](DUT* d) {
        d->stim_reg_fcc_en = 1;
        d->stim_fcc_ready = 0;
        d->stim_wb_fcc_addr = 0;
        d->stim_wb_wdata = 0; // bit0 = 0
    });

    // FCC1: fcc_en, not ready, wdata bit0=1
    test_replay([](DUT* d) {
        d->stim_reg_fcc_en = 1;
        d->stim_fcc_ready = 0;
        d->stim_wb_fcc_addr = 0;
        d->stim_wb_wdata = 1; // bit0 = 1
    });

    // TMASK
    test_replay([](DUT* d) {
        d->stim_reg_tensor_mask_read_invalid = 1;
    });

    // PMU_READ: pmu not ready
    test_replay([](DUT* d) {
        d->stim_pmu_read_en_wb = 1;
        d->stim_pmu_read_mem_ready = 0;
    });

    // TEX: tex_send_ready=0
    test_replay([](DUT* d) {
        d->stim_reg_tex_send_en = 1;
        d->stim_dcache_ctrl_flat &= ~(1u << 7); // clear tex_send_ready
    });

    // PGET: msg_port_pget_block
    test_replay([](DUT* d) {
        d->stim_msg_port_pget_block = 1;
    });

    // ── Phase 3: Clear conditions ────────────────────────
    // Set a stall, then assert the clear condition.

    // Set CACHE_OP stall then clear with cache_op_busy_changed
    idle_inputs(sim.dut.get());
    sim.dut->stim_reg_co_evict_va_en = 1;
    sim.dut->stim_dcache_ctrl_flat &= ~(1u << 22); // cache_op_ready=0
    sim.tick(); compare_all(sim);
    // Now idle but keep cache_op_ready=0 to maintain stall
    idle_inputs(sim.dut.get());
    sim.dut->stim_dcache_ctrl_flat &= ~(1u << 22);
    sim.tick(); compare_all(sim);
    // Clear: set cache_op_busy_changed for both threads
    sim.dut->stim_dcache_ctrl_flat |= (3u << 20); // cache_op_busy_changed[1:0]
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Set FLB stall then clear with fl_barrier_ready
    idle_inputs(sim.dut.get());
    sim.dut->stim_reg_flb_en = 1;
    sim.dut->stim_fl_barrier_ready = 0;
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    sim.dut->stim_fl_barrier_ready = 0; // keep stalled
    sim.tick(); compare_all(sim);
    sim.dut->stim_fl_barrier_ready = 1; // clear
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Clear by interrupt
    idle_inputs(sim.dut.get());
    sim.dut->stim_reg_tensor_fma_en = 1;
    sim.dut->stim_tensor_fma_replay_condition = 1;
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    sim.dut->stim_tensor_fma_replay_condition = 1; // keep condition to prevent clear
    sim.tick(); compare_all(sim);
    sim.dut->stim_io_interrupt = 0x3; // interrupt both threads
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Clear by redirect
    idle_inputs(sim.dut.get());
    sim.dut->stim_reg_tensor_quant_en = 1;
    sim.dut->stim_tensor_quant_replay_condition = 1;
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    sim.dut->stim_tensor_quant_replay_condition = 1;
    sim.tick(); compare_all(sim);
    sim.dut->stim_io_redirect = 0x3;
    sim.tick(); compare_all(sim);
    idle_inputs(sim.dut.get());
    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 4: Tensor wait replay conditions ───────────
    // Test each tensor_wait type (wb_wdata[3:0] selects which)

    for (int wait_type = 0; wait_type <= 10; wait_type++) {
        idle_inputs(sim.dut.get());
        sim.dut->stim_reg_tensor_wait_en = 0x3;
        sim.dut->stim_wb_wdata = wait_type;
        sim.dut->stim_wb_thread_id = 0; // thread 0 for thread-specific waits

        // Set the blocking condition for each wait type
        switch (wait_type) {
            case 0: // TW_TL0: !tl_complete_0[0], thread==0
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 9); // clear tl_complete_0[0]
                break;
            case 1: // TW_TL1: !tl_complete_0[1], thread==0
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 9); // clear tl_complete_0[1] (bit 9 is [1])
                // Actually need to be more careful. tl_complete_0 is 2 bits.
                // Let me clear the correct bit.
                sim.dut->stim_dcache_ctrl_flat = 0x7FFFFF & ~(1u << 9); // clear bit 9 = tl_complete_0[1]
                break;
            case 2: // TW_PREFETCH0: !cache_op_complete_l2[thread][0]
            case 4: // same condition
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 14); // cache_op_complete_l2[0][0]
                break;
            case 3: // TW_PREFETCH1: !cache_op_complete_l2[thread][1]
            case 5:
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 15); // cache_op_complete_l2[0][1]
                break;
            case 6: // TW_CACHE_OP: not all complete
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 14); // clear one l2 complete bit
                break;
            case 7: // TW_TENSOR_FMA: tensor_fma_replay_condition, thread==0
                sim.dut->stim_tensor_fma_replay_condition = 1;
                break;
            case 8: // TW_TENSOR_STORE: !ts_ready, thread==0
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 10);
                break;
            case 9: // TW_TENSOR_REDUCE: !reduce_ready, thread==0
                sim.dut->stim_dcache_ctrl_flat &= ~(1u << 8);
                break;
            case 10: // TW_TENSOR_QUANT: tensor_quant_replay_condition, thread==0
                sim.dut->stim_tensor_quant_replay_condition = 1;
                break;
        }

        sim.tick(); compare_all(sim);
        idle_inputs(sim.dut.get());
        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }
    }

    // ── Phase 5: Random stimulus ─────────────────────────
    for (int i = 0; i < 5000; i++) {
        uint32_t r1 = xorshift32();
        uint32_t r2 = xorshift32();
        uint32_t r3 = xorshift32();

        DUT* d = sim.dut.get();
        d->stim_wb_wdata                      = xorshift64();
        d->stim_wb_thread_id                  = r1 & 1;
        d->stim_io_interrupt                  = (r1 >> 1) & 3;
        d->stim_io_redirect                   = (r1 >> 3) & 3;
        d->stim_reg_co_evict_va_en            = (r1 >> 5) & 1;
        d->stim_reg_co_flush_va_en            = (r1 >> 6) & 1;
        d->stim_reg_co_lock_va_en_qual        = (r1 >> 7) & 1;
        d->stim_reg_co_unlock_va_en_qual      = (r1 >> 8) & 1;
        d->stim_reg_co_lock_sw_en             = (r1 >> 9) & 1;
        d->stim_reg_co_unlock_sw_en           = (r1 >> 10) & 1;
        d->stim_reg_co_prefetch_va_en         = (r1 >> 11) & 1;
        d->stim_reg_co_evict_sw_en            = (r1 >> 12) & 1;
        d->stim_reg_co_flush_sw_en            = (r1 >> 13) & 1;
        d->stim_reg_tensor_load_l2scp_en      = (r1 >> 14) & 1;
        d->stim_reg_flb_en                    = (r1 >> 15) & 1;
        d->stim_reg_tensor_load_en            = (r1 >> 16) & 1;
        d->stim_reg_tensor_fma_en             = (r1 >> 17) & 1;
        d->stim_reg_tensor_quant_en           = (r1 >> 18) & 1;
        d->stim_reg_tensor_reduce_en          = (r1 >> 19) & 1;
        d->stim_reg_tensor_store_en           = (r1 >> 20) & 1;
        d->stim_reg_tensor_conv_ctrl_en       = (r1 >> 21) & 3;
        d->stim_reg_tensor_conv_size_en       = (r1 >> 23) & 3;
        d->stim_reg_fcc_en                    = (r1 >> 25) & 3;
        d->stim_reg_tensor_wait_en            = (r1 >> 27) & 3;
        d->stim_reg_tex_send_en               = (r1 >> 29) & 1;
        d->stim_reg_tensor_mask_read_invalid  = (r1 >> 30) & 1;
        d->stim_pmu_read_en_wb                = (r1 >> 31) & 1;

        d->stim_pmu_read_mem_ready            = r2 & 3;
        d->stim_dcache_ctrl_flat              = (r2 >> 2) & 0x7FFFFF; // 23 bits
        d->stim_fl_barrier_ready              = (r2 >> 25) & 1;
        d->stim_tensor_fma_replay_condition   = (r2 >> 26) & 1;
        d->stim_tensor_quant_replay_condition = (r2 >> 27) & 1;
        d->stim_vpu_treduce_enabled           = (r2 >> 28) & 1;
        d->stim_tenb_tl_pend_tfma_en          = (r2 >> 29) & 1;
        d->stim_tenb_restart_process          = (r2 >> 30) & 1;
        d->stim_tload_is_for_tenb             = (r2 >> 31) & 1;

        d->stim_conv_can_accept_new_write     = r3 & 3;
        d->stim_fcc_ready                     = (r3 >> 2) & 0xF;
        d->stim_wb_fcc_addr                   = (r3 >> 6) & 3;
        d->stim_msg_port_pget_block           = (r3 >> 8) & 1;
        d->stim_msg_port_stall_flat           = (r3 >> 9) & 0xFF;
        d->stim_reg_tensor_mask_ready         = (r3 >> 17) & 3;

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

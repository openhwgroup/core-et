/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_compute.h"
#include "test_compute_pass0.h"
#include "test_compute_pass1.h"
#include "test_compute_pass2.h"
#include "utils.h"
#include "test_filter_data.h"
#include "test_act_data.h"

volatile uint64_t l2_scp_initialized = 0;

// Initializes the L2 scp
static inline void init_l2_scp(uint32_t shire_id, uint32_t minion_id)
{
    // Each minion writes 1024 CL, to write a total of 2MBytes of L2Scp
    uint64_t l2_scp_addr = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (minion_id * 1024 * 64);
    uint64_t offset = (minion_id & 0x3) * 0x100; //256

    __asm__ __volatile__ (
        // Init FP registers
        // Each minion stores 1024 cachelines. First 8 write activation type of data, other filter
        "li       x31, 8\n"
        "blt      %[minion_id], x31, load_act\n"
        "addi     x31, %[filter_data], 0\n"
        "j        do_loads\n"
        "load_act:\n"
        "addi     x31, %[act_data], 0\n"
        "do_loads:\n"
        "addi     t0, x31, 0x400\n"  // t0 is limit
        "add      x31, x31, %[offset]\n"
        "flw.ps   f0,  0x0 (x31)\n"
        "flw.ps   f1,  0x20(x31)\n"
        "flw.ps   f2,  0x40(x31)\n"
        "flw.ps   f3,  0x60(x31)\n"
        "flw.ps   f4,  0x80(x31)\n"
        "flw.ps   f5,  0xA0(x31)\n"
        "flw.ps   f6,  0xC0(x31)\n"
        "flw.ps   f7,  0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset0\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset0:\n"
        "flw.ps   f8,  0x0 (x31)\n"
        "flw.ps   f9,  0x20(x31)\n"
        "flw.ps   f10, 0x40(x31)\n"
        "flw.ps   f11, 0x60(x31)\n"
        "flw.ps   f12, 0x80(x31)\n"
        "flw.ps   f13, 0xA0(x31)\n"
        "flw.ps   f14, 0xC0(x31)\n"
        "flw.ps   f15, 0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset1\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset1:\n"
        "flw.ps   f16, 0x0 (x31)\n"
        "flw.ps   f17, 0x20(x31)\n"
        "flw.ps   f18, 0x40(x31)\n"
        "flw.ps   f19, 0x60(x31)\n"
        "flw.ps   f20, 0x80(x31)\n"
        "flw.ps   f21, 0xA0(x31)\n"
        "flw.ps   f22, 0xC0(x31)\n"
        "flw.ps   f23, 0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset2\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset2:\n"
        "flw.ps   f24, 0x0 (x31)\n"
        "flw.ps   f25, 0x20(x31)\n"
        "flw.ps   f26, 0x40(x31)\n"
        "flw.ps   f27, 0x60(x31)\n"
        "flw.ps   f28, 0x80(x31)\n"
        "flw.ps   f29, 0xA0(x31)\n"
        "flw.ps   f30, 0xC0(x31)\n"
        "flw.ps   f31, 0xE0(x31)\n"

        // do 64 tstores of 16 cachelines
        "addi     t0, zero, 64\n"
        "li       t1, (3 << 55) + (15 << 51)\n"
        "add      t1, t1, %[l2_scp_addr] \n"
        "li       x31, 0x40\n"
        "loop:\n"
        "csrw     0x87F, t1\n"
        "addi     t1, t1, 64 * 16\n" // Advance 1024 bytes
        "addi     t0, t0, -1\n"
        "bne      t0, zero, loop\n"
        // Twait for tstores at the end
        "csrwi    0x830, 8\n"
      :
      :
        [l2_scp_addr] "r" (l2_scp_addr),
        [filter_data] "r" (filter_data),
        [act_data] "r" (act_data),
        [minion_id] "r" (minion_id),
        [offset] "r" (offset)
      : "t0", "t1", "t2", "x31"
    );

    #ifdef UPF
      if (shire_id == 0) {
      // Each minion writes 1024 CL, to write a total of 2MBytes of L2Scp
        l2_scp_addr = (uint64_t volatile) 0x0080000000UL + (2 << 23) + (minion_id * 1024 * 64);
    
        __asm__ __volatile__ (
            // do 64 tstores of 16 cachelines
            "addi     t0, zero, 64\n"
            "li       t1, (3 << 55) + (15 << 51)\n"
            "add      t1, t1, %[l2_scp_addr] \n"
            "li       x31, 0x40\n"
            "loop2:\n"
            "csrw     0x87F, t1\n"
            "addi     t1, t1, 64 * 16\n" // Advance 1024 bytes
            "addi     t0, t0, -1\n"
            "bne      t0, zero, loop2\n"
            // Twait for tstores at the end
            "csrwi    0x830, 8\n"
          :
          :
            [l2_scp_addr] "r" (l2_scp_addr),
            [filter_data] "r" (filter_data),
            [act_data] "r" (act_data),
            [minion_id] "r" (minion_id),
            [offset] "r" (offset)
          : "t0", "t1", "t2", "x31"
        );

      }
    #endif

    // Initialize just once for multi-run cases
    if((shire_id == 0) && (minion_id == 0))
    {
        l2_scp_initialized = 1;
        // Evicts from L1/L2
        FENCE;
        evict_va(0, to_L3, (uint64_t) &l2_scp_initialized, 0, 0, 0, 0);
        WAIT_CACHEOPS;
    }
}

void test_compute(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ ("mov.m.x m0, zero, 0xff /*255*/  \n");

    // Initialize L2 Scp
    if(l2_scp_initialized == 0)
    {
        // Evicts from L1/L2
        FENCE;
        evict_va(0, to_L3, (uint64_t) &l2_scp_initialized, 0, 0, 0, 0);
        WAIT_CACHEOPS;

        init_l2_scp(shire_id, minion_id);
    }

    // Global barrier (harts, flb to use, source shire id, fcc to sync minions)
    global_barrier_starter(32, 0, shire_id, 0);

    // Waits for credit to come back
    fcc(FCC_0);

    // Initial credit to wake up the threads to let them know L2 scp init is done
    // Acts as if sync minions sent it to FCC1
    if (minion_id == 0) {
        fcc_send(shire_id,
                 HELPER_ACTIVATION_THREADS
               | HELPER_WEIGHTS_THREADS
               | HELPER_DRAIN_THREADS
               | HELPER_CODE_THREADS
               | HELPER_DDR_THREADS,
                 THREAD_1,
                 FCC_1);
    }
    // Sends initial credits to helper activation threads (done by minion1..5)
    // Up to 5 iterations ahead. The way credits are returned is before tload is
    // finished, so we can't give 6 credits
    else if(minion_id < 6)
        fcc_send(shire_id, HELPER_ACTIVATION_THREADS, THREAD_1, FCC_0);

    __asm__ __volatile__ (
        "test_compute_start_point:\n"
        "csrwi 0x89f, 0x1a\n"      
        ".global test_compute_start_point\n"
    );

    // 3 passes
    test_compute_pass0(shire_id, minion_id);
    // WARNING: pass1 is commented inside the function, because GCC complaints otherwise...
    test_compute_pass1(shire_id, minion_id);
#ifndef SINGLE_PASS
    test_compute_pass2(shire_id, minion_id);
#endif

    __asm__ __volatile__ (
        "csrwi 0x89f, 0x1b\n"
        "test_compute_end_point:\n"
        ".global test_compute_end_point\n"
    );
}



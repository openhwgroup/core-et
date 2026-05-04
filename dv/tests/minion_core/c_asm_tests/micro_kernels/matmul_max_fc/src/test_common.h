/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

// Global
#include <inttypes.h>

// Firmware
//#include "fw_ecalls.h"
//Local
#include "utils.h"
#include "cacheops.h"

// L2 SCP regions (32,768 entries):
//  - 4096 entries for activation prefetch (4 buffers per minion x 16 cachelines per minion x 8 minions x 7 iterations)
//  - 4096 entries store the input activation (8 batch x 56 rows x 56 cols x 256 feats / 32 shires)
//  - 1024 entries store the result (8 batch x 56 rows x 56 cols x 64 feats / 32 shires)
//  - All other entries for weight prefetcher

#define L2_SCP_ACTIVATION_PREF_OFFSET   0
#define L2_SCP_ACTIVATION_PREF_SIZE     4096
#define L2_SCP_ACTIVATION_SOURCE_OFFSET (L2_SCP_ACTIVATION_PREF_OFFSET+L2_SCP_ACTIVATION_PREF_SIZE)
#define L2_SCP_ACTIVATION_SOURCE_SIZE   4096
#define L2_SCP_RESULT_OFFSET            (L2_SCP_ACTIVATION_SOURCE_OFFSET+L2_SCP_ACTIVATION_SOURCE_SIZE)
#define L2_SCP_RESULT_SIZE              1024
#define L2_SCP_WEIGHT_PREF_OFFSET       (L2_SCP_RESULT_OFFSET+L2_SCP_RESULT_SIZE)
#define L2_SCP_WEIGHT_SIZE              2048
#define L2_SCP_WEIGHT_MEM_OFFSET        (L2_SCP_WEIGHT_PREF_OFFSET+L2_SCP_WEIGHT_SIZE)

// Memory map:

#define MEM_WEIGHTS 0x8080000000ULL
#define MEM_SCALE   0x8090000000ULL
#define MEM_OFFSET  0x80A0000000ULL

// Helper thread masks
#define COMPUTE_THREADS           0xFFFFFFFFULL
#define HELPER_ACTIVATION_THREADS 0x0000FFFFULL
#define HELPER_WEIGHTS_THREADS    0x00FF0000ULL
#define HELPER_DRAIN_THREADS      0x0F000000ULL
#define HELPER_CODE_THREADS       0x10000000ULL
#define HELPER_DDR_THREADS        0x20000000ULL

// Some defines
#define THREAD_0 0
#define THREAD_1 1
#define FCC_0    0
#define FCC_1    1

// Global functions

// This function sends one FCC to a sync minion when the last hart gets
// to the barrier
static inline void global_barrier_starter(
                            uint64_t num_harts, // Harts doing barrier in the source shires
                            uint64_t flb_num,   // FLB to be used in the source shire for the barrier
                            uint64_t shire_id,  // Source shire id
                            uint64_t fcc)       // Which FCC to send the shire ready signal
{
    volatile uint64_t * sync_minion_addr = (uint64_t * ) (
                                           (1ULL << 32)          // ESR
                                         + (32ULL << 22)         // Going to master shire
                                         + (0x1AULL << 17)       // Shire other ESRs
                                         + 0xC0ULL               // FCC ESRs
                                         + ((shire_id & 1) * 16) // Which thread is going to
                                         + (fcc * 8));           // FCC destination
    uint64_t sync_minion_data = 1ULL << ((shire_id / 2) + 16); // Send FCC to according sync minion
    uint64_t flb_result = flb(flb_num, num_harts - 1);
    if(flb_result == 1)
        * sync_minion_addr = sync_minion_data;
}

// This function wiats for one FCC to a sync minion when the last hart gets
// to the barrier
static inline void global_barrier_receiver(
                            uint64_t fcc_wait,         // Which FCC to wait for
                            uint64_t flb_num,          // FLB to be used in the source shire for the barrier
                            uint64_t minion_id,        // Sync minion id
                            uint64_t thread_id,        // Sync thread id
                            uint64_t thread_dest,      // Thread of the FCC dest
                            uint64_t fcc_dest,         // FCC for dest
                            uint64_t minion_mask_dest) // Mask of minions in dest shire to receive FCC
{
    // Waits for its associated shire FCC and do FLB
    fcc(fcc_wait);
    uint64_t flb_result = flb(flb_num, N_SHIRES_COMPUTE - 1);

    // If last wake up other sync minions
    if(flb_result == 1)
    {
        // Send to FCC1
        fcc_send(32, 0xFFFF0000ULL, 0, 1);
        fcc_send(32, 0xFFFF0000ULL, 1, 1);
    }

    // Waits for FCC that last sync minion got FCC
    fcc(1);

    // Sends FCC to destination
    fcc_send(((minion_id - 16) * 2) + thread_id, minion_mask_dest, thread_dest, fcc_dest);
}

#ifdef MINSHIRE_GATESIM_ENABLE
inline __attribute__((always_inline)) void initFRegs_0() {
 
  __asm__ __volatile__
    (
     "fbci.ps  f0,0\n"
     "fbci.ps  f1,0\n"
     "fbci.ps  f2,0\n"
     "fbci.ps  f3,0\n"
     "fbci.ps  f4,0\n"
     "fbci.ps  f5,0\n"
     "fbci.ps  f6,0\n"
     "fbci.ps  f7,0\n"
     "fbci.ps  f8,0\n"
     "fbci.ps  f9,0\n"
     "fbci.ps f10,0\n"
     "fbci.ps f11,0\n"
     "fbci.ps f12,0\n"
     "fbci.ps f13,0\n"
     "fbci.ps f14,0\n"
     "fbci.ps f15,0\n"
     "fbci.ps f16,0\n"
     "fbci.ps f17,0\n"
     "fbci.ps f18,0\n"
     "fbci.ps f19,0\n"
     "fbci.ps f20,0\n"
     "fbci.ps f21,0\n"
     "fbci.ps f22,0\n"
     "fbci.ps f23,0\n"
     "fbci.ps f24,0\n"
     "fbci.ps f25,0\n"
     "fbci.ps f26,0\n"
     "fbci.ps f27,0\n"
     "fbci.ps f28,0\n"
     "fbci.ps f29,0\n"
     "fbci.ps f30,0\n"
     "fbci.ps f31,0\n"
     :
     : 
     : "memory"
    );
}

inline __attribute__((always_inline)) void setM0MaskFF() {
  __asm__ __volatile__
    (
     "mov.m.x m0, zero, 0xff\n"
     :
     : 
     : "memory"
    );
}

inline __attribute__((always_inline)) void tensorStore(long conf, long stride) {
    register long s asm("x31") = stride;
    __asm__ __volatile__
        (
         "  csrw 0x87F,  %[conf]\n"
         :
         : [conf] "r" (conf), "r" (s)
         : "memory"
        );
}

#endif 




#if 0
static inline void ecall_cb_drain(uint64_t params, uint64_t hart_mask)
{
    __asm__ __volatile__
    (
        "mv   a0, %[ecall_param0]\n"
        "mv   a1, %[ecall_param1]\n"
        "mv   a2, %[ecall_param2]\n"
        "mv   a3, %[ecall_param3]\n"
        "ecall\n"
      :
      : [ecall_param0] "r" (FW_COMPUTE_SCODE_ECALL_CB_DRAIN),
        [ecall_param1] "r" (params),
        [ecall_param2] "r" (hart_mask),
        [ecall_param3] "r" (0)
      : "a0", "a1", "a2", "a3"
    );
}
#endif

#endif


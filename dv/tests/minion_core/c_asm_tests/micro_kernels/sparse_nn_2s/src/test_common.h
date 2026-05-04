/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

// Global
#include <inttypes.h>
#include <utils.h>

// Memory map:

// Defines
#define FCC_0 0
#define FCC_1 1

// Global functions

// Executes an FLB instruction on a barrier and number harts minus 1
#if 0
static inline uint64_t flb(int barrier_num, uint8_t match)
{
    uint64_t ret;
    uint64_t flb_arg = (match << 5) | (barrier_num & 0x1F);

    __asm__ __volatile__ (
        "csrrw  %0, 0x820, %1\n"
        : "=r"(ret)
        : "r"(flb_arg)
        : "memory"
    );

    return ret;
}
#endif

/*// Executes a blocking FCC on a specific reg
static inline void fcc(uint64_t fcc_reg)
{
    __asm__ __volatile__ (
        "csrw   0x821, %0\n"
        : : "r"(fcc_reg)
        : "memory"
    );
}

// Sends an FCC credit
static inline void fcc_send(uint64_t shire, uint64_t minion_mask, uint64_t thread, uint64_t fcc_num)
{
    volatile uint64_t * fcc_addr = (uint64_t * ) (
                                   (1ULL << 32)    // ESR
                                 + (shire << 22)   // Dest shire
                                 + (0x1AULL << 17) // Shire other ESRs
                                 + 0xC0ULL         // FCC ESRs
                                 + (thread * 16)   // Which thread is going to
                                 + (fcc_num * 8)); // FCC destination
    * fcc_addr = minion_mask;
}*/

// This function sends one FCC to a sync minion when the last hart gets
// to the barrier
static inline void global_barrier_starter(
                            uint64_t num_harts, // Harts doing barrier in the source shires
                            uint64_t flb_num,   // FLB to be used in the source shire for the barrier
                            uint64_t shire_id,  // Source shire id
                            uint64_t fcc)       // Which FCC to send the shire ready signal
{
    uint64_t * sync_minion_addr = (uint64_t * ) (
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
    uint64_t flb_result = flb(flb_num, SIM_SHIRES - 1);

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

#endif

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

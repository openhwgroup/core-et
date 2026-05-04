/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



#ifndef _SLT_BARRIER_
#define _SLT_BARRIER_
#include "slt_ifc.h"
#include "cacheops.h"

// define a value different to 0 to avoid allocating the global variable in the .bss section
#define RESET_VAL 1

__attribute__((aligned(64))) uint64_t amo_barrier_count[64/sizeof(uint64_t)] = {RESET_VAL};

uint64_t amo_incr() {
    uint64_t ret;
    uint64_t incr = 1;
    uint64_t addr = (uint64_t) &amo_barrier_count;
    __asm__ __volatile__ (
        "amoaddg.d %[ret], %[incr], (%[addr])\n"
        : [ret] "=r" (ret)
        : [incr] "r" (incr), [addr] "r" (addr)
        :
   );
   return ret-RESET_VAL;
}

void amo_reset() {
    uint64_t addr = (uint64_t) &amo_barrier_count;
    uint64_t reset_val = RESET_VAL;
    __asm__ __volatile__ (
        "amoswapg.d x0, %[reset_val], (%[addr])\n"
        :
        : [addr] "r" (addr), [reset_val] "r" (reset_val)
        :
   );
}

void amo_wait() {
    uint64_t ret;
    uint64_t addr = (uint64_t) &amo_barrier_count;
    uint64_t delay = get_minion_id();
    do {
        // Delay using NOPs
        for (uint64_t i = 0; i < delay; i++) {
           NOP;
        }
        __asm__ __volatile__ (
            "amoorg.d %[ret], x0, (%[addr])\n"
            : [ret] "=r" (ret)
            : [addr] "r" (addr)
            :
        );
    } while (ret != RESET_VAL);
}

uint64_t fast_local_barrier(int barrier_num, uint8_t match) {
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

void wait_fcc() {
    __asm__ __volatile__(
        "csrrwi x0, fcc, 0\n"
    );
}

inline void send_even_local_fcc(uint64_t minion_mask) {
    volatile uint64_t* fcc_0 = (uint64_t*) 0x803400C0;
    *fcc_0 = minion_mask;
}

inline void send_odd_local_fcc(uint64_t minion_mask) {
    volatile uint64_t* fcc_1 = (uint64_t*) 0x803400D0;
    *fcc_1 = minion_mask;
}

void send_local_fcc() {
    uint64_t minion_mask = get_minion_mask();
    send_even_local_fcc(minion_mask);
    send_odd_local_fcc(minion_mask);
}

void send_remote_fcc(uint64_t shire_id) {
    shire_id = shire_id & 0x0FF;
    uint64_t minion_mask = get_minion_mask();
    volatile uint64_t* fcc_0 = (uint64_t*) (0x803400C0 | shire_id << 22);
    *fcc_0 = minion_mask;

    volatile uint64_t* fcc_1 = (uint64_t*) (0x803400C0 | shire_id << 22);
    *fcc_1 = minion_mask;
}

void minion_evict_l1(uint64_t dst) {
   evict_sw(0, dst, 0, 0, 15, 0);
   evict_sw(0, dst, 1, 0, 15, 0);
   evict_sw(0, dst, 2, 0, 15, 0);
   evict_sw(0, dst, 3, 0, 15, 0);
   WAIT_CACHEOPS;
   FENCE;
}

void wait_sc_cop(volatile uint64_t* addr) {
   const uint64_t SC_COP_SM_ACTIVE = 8;
   uint64_t cop_sm_state;
   uint64_t mask;
   do {
      __asm__ __volatile__(
         "ld %[cop_sm_state], 0(%[addr])\n"
         "srli %[cop_sm_state], %[cop_sm_state], 24\n"
         "li %[mask], 255\n"
         "and %[cop_sm_state], %[cop_sm_state], %[mask]\n"
         : [cop_sm_state] "=r" (cop_sm_state),
           [mask] "=r" (mask)
         : [addr] "r" (addr)
         :
      );
   } while (cop_sm_state == SC_COP_SM_ACTIVE);
}

void sc_cop(uint64_t sc_cop_enc) {
    uint64_t sid_mask = 255 << 22;
    volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_BCAST = (uint64_t*) (0x80F1E030 + sid_mask);

    // Trigger a Shire Cache cache op
    *ESR_SC_IDX_COP_SM_CTL_BCAST = sc_cop_enc;
    FENCE;

    // Wait for the cacheop to be done
    volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B0 = (uint64_t*) (0x80F00030 + sid_mask);
    volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B1 = (uint64_t*) (0x80F02030 + sid_mask);
    volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B2 = (uint64_t*) (0x80F04030 + sid_mask);
    volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B3 = (uint64_t*) (0x80F06030 + sid_mask);
    wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B0);
    wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B1);
    wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B2);
    wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B3);
}

void sc_evict_l2(uint64_t evict_dst) {
    uint64_t sc_cop_enc;
    if (evict_dst == 2) sc_cop_enc = 0x301;
    else                sc_cop_enc = 0x601;
    sc_cop(sc_cop_enc);
}

void sc_cb_inv() {
    sc_cop(0xA01);
}

void slt_barrier(uint64_t evict) {
    /*  evict:
     *  0 -> No evicts. AKA do not change cache states
     *  1 -> Evict to L2
     *  2 -> Evict to L3
     *  3 -> Evict to MEM
     */
    if (evict > 0) {
        // All HARTs evict to their local L1 to the L2
        minion_evict_l1(1);
    }

    uint32_t active_harts_per_shire = get_number_active_harts_per_shire();
    uint64_t last = fast_local_barrier(3, active_harts_per_shire-1);
    if (last == 1) {
        // Coalescing Buffer
        sc_cb_inv();
        if (evict > 1) {
            // Only the last HART to get into the FLB triggers the evict from L2 to L3/MEM
            sc_evict_l2(evict);
        }
        uint64_t count = amo_incr() + 1;
        uint64_t players = get_number_active_shires();
        if (count == players) {
            amo_reset();
        }
        else {
            amo_wait();
        }
        send_local_fcc();
    }
    wait_fcc();
}

void slt_soc_fcc_barrier_with_delay(uint64_t delay) {
    uint32_t active_harts_per_shire = get_number_active_harts_per_shire();
    uint64_t last = fast_local_barrier(3, active_harts_per_shire-1);
    if (last == 1) {
        uint64_t count = amo_incr() + 1;
        uint64_t players = get_number_active_shires();
        if (count == players) {
            for (uint64_t i = 0; i < delay; i++) {
               NOP;
            }
            amo_reset();
        }
        else {
            amo_wait();
        }
        send_local_fcc();
    }
    wait_fcc();
}

#endif

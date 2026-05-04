/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "minion.h"
#include "test_barrier.h"

uint64_t get_local_mprot_addr() {
    uint64_t nid = get_neigh_id();
    uint64_t local_mprot_addr = 0x80D00020 + (nid << 16);
    return local_mprot_addr;
}

uint64_t get_broadcast_mprot_addr() {
    uint64_t broadcast_mprot_addr = 0x80DF0020;
    return broadcast_mprot_addr;
}

uint64_t read_mprot() {
    volatile uint64_t* local_mprot_addr = (volatile uint64_t*) get_local_mprot_addr();
    uint64_t mprot_config = *local_mprot_addr;
    FENCE;
    return mprot_config;
}

void write_mprot(uint64_t mprot_config) {
    volatile uint64_t* broadcast_mprot_addr = (volatile uint64_t*) get_broadcast_mprot_addr();
    *broadcast_mprot_addr = mprot_config;
    FENCE;
}

void set_dram_32gb() {
    // All HARTs evict their local L1 to the L2
    minion_evict_l1(1);

    const uint32_t active_harts_per_shire = 64;
    const uint32_t active_shires = 34;

    uint64_t last = fast_local_barrier(3, active_harts_per_shire-1);
    if (last == 1) {
        // Coalescing Buffer
        sc_cb_inv();
        // Only the last HART to get into the FLB triggers the evict from L2 to L3/MEM
        sc_evict_l2(2);

        // Write to the 
        uint64_t curr_mprot_config = read_mprot();
        uint64_t new_mprot_config = curr_mprot_config | (0b11 << 4); // Set DRAM to 32GB
        write_mprot(new_mprot_config);
        
        uint64_t count = amo_incr();
        uint64_t players = active_shires;
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

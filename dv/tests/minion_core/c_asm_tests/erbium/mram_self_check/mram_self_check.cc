/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

uint64_t addr_read(const uint64_t addr) {
    uint64_t rd_val = 0xdead0baddead0bad;
    rd_val = *(volatile uint64_t *)addr;
    return rd_val;
}

void addr_write(const uint64_t addr, const uint64_t val) {
    *(volatile uint64_t *)addr = val;
}

void evict_dc() {
    FENCE;
    // each hart evicts half the Minion's dcache (h0 ways 0,1; h1 ways 2,3)
    uint64_t tid = get_thread_id();
    for (uint64_t set = 0; set < 16; set++) {
        constexpr uint64_t dest_l2 = 1;
        //constexpr uint64_t dest_l3 = 2;
        for (uint64_t way = (2*tid); way < (2*tid)+2; way++) {
            //       use_tmask  dest     way  set  num_lines  warl
            //       ---------  -------- ---- ---- ---------- ----
            evict_sw(0,         dest_l2, way, set, 0,         0);
        }
    }
    WAIT_CACHEOPS;
}

#ifdef FROM_MRAM
// short sanity check when executing from MRAM
constexpr uint64_t mram_bootrom_size = 0x400;
constexpr uint64_t sram_placeholder_size = 0x800;
constexpr uint64_t mram_start_addr = (
    0x40000000 + mram_bootrom_size + sram_placeholder_size);
constexpr uint64_t range = (1024); // 1K per hart, 16K total

#else

constexpr uint64_t mram_start_addr = 0x40000000;
// optionally short, else entire MRAM (takes hours in RTL sim)
#ifdef SHORT
constexpr uint64_t range = (1024); // 1K per hart, 16K total
#else
constexpr uint64_t range = (1024*1024); // 1M per hart, 16M total
#endif

#endif

static_assert((range % 8) == 0, "range must be multiple of 8 (sd/ld)");

int main() {
    C_TEST_START;

    uint64_t hid = get_shire_hart_id();
    uint64_t start = (hid * range);
    uint64_t end = (start + range);

    for (uint64_t i = start; i < end; i+=8) {
        uint64_t idx = (mram_start_addr + i);
        uint64_t hash = splitmix64(idx);
        addr_write(idx, hash);
    }

    evict_dc();

    size_t cnt = 200;
    while (cnt--) NOP
    for (uint64_t i = start; i < end; i+=8) {
        uint64_t idx = (mram_start_addr + i);
        uint64_t hash = splitmix64(idx);
        uint64_t val = addr_read(idx);
        if (hash != val) C_TEST_FAIL;
    }

    C_TEST_PASS;
    return 0;
}

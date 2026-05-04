/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"

// Test assumes vm off
void assert_vm_off()
{
    uint64_t matp, satp;
    asm volatile ("csrr %[reg], matp\n" : [reg] "=r" (matp)::);
    asm volatile ("csrr %[reg], satp\n" : [reg] "=r" (satp)::);

    if (matp != 0 || satp != 0) C_TEST_FAIL;
}

uint8_t cache_line_t0[L1_CL_SIZE] __attribute__((aligned(64)));
uint8_t cache_line_t1[L1_CL_SIZE] __attribute__((aligned(64)));

inline void populate_cacheline(uint8_t * cacheline)
{
    uint64_t* dwptr = reinterpret_cast<uint64_t*>(cacheline);
    constexpr auto dwords = L1_CL_SIZE/sizeof(uint64_t);
    #pragma GCC unroll dwords
    for (uint64_t i = 0; i < dwords; ++i)
        dwptr[i] = 0xDEADBEAFCAFEBAB0 + i;
}

inline uint64_t get_cacheline()
{
    return reinterpret_cast<uint64_t>(get_thread_id() ? cache_line_t1 : cache_line_t0);
}

// Assumes phyisical addresses
inline uint64_t get_set(uint64_t addr)
{
    return (addr >> L1_CL_SIZE_LOG2) & ((1 << L1_SETS_LOG2)-1);
}

template<enum cop_dest start_level, enum cop_dest end_level, typename Func>
inline void test_op(Func&& f)
{
    static_assert(start_level <= end_level, "Levels range must be from core to memory");
    auto * cache_line = get_thread_id() ?  cache_line_t1 : cache_line_t0;
    for (int start = start_level; start <= end_level; ++start)
    {
        enum cop_dest lvl = static_cast<enum cop_dest>(start);
        populate_cacheline(cache_line);
        FENCE
        f(lvl);
        WAIT_CACHEOPS;
        FENCE;
    }
}


template<int way>
inline void test_evict_sw(cop_dest level)
{
    evict_sw(0, level, way, get_set(get_cacheline()), 0);
};

template<int way>
inline void test_flush_sw(cop_dest level)
{
    flush_sw(0, level, way, get_set(get_cacheline()), 0);
};


int main()
{
    setup_mtrap_vector();
    assert_vm_off();

    C_TEST_START;

    constexpr auto test_evict_va = [](cop_dest level) { evict_va (0, level, get_cacheline(), 0); };
    constexpr auto test_flush_va = [](cop_dest level) { flush_va (0, level, get_cacheline(), 0); };
    constexpr auto test_prefetch_va = [](cop_dest level) { prefetch_va (0, level, get_cacheline(), 0); };

    // Test ops
    test_op<to_L3, to_Mem> (test_evict_sw<0>);
    test_op<to_L3, to_Mem> (test_evict_sw<1>);
    test_op<to_L3, to_Mem> (test_evict_sw<2>);
    test_op<to_L3, to_Mem> (test_evict_sw<3>);
    test_op<to_L3, to_Mem> (test_flush_sw<0>);
    test_op<to_L3, to_Mem> (test_flush_sw<1>);
    test_op<to_L3, to_Mem> (test_flush_sw<2>);
    test_op<to_L3, to_Mem> (test_flush_sw<3>);

    test_op<to_L3, to_Mem> (test_evict_va);
    test_op<to_L3, to_Mem> (test_flush_va);

    test_op<to_L2, to_Mem> (test_prefetch_va);

    // Test waits that affects co_l2 
    WAIT_PREFETCH_0;
    WAIT_PREFETCH_1;
    WAIT_CACHEOPS;
    

    C_TEST_PASS
}

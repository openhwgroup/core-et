
#pragma once

#include <utility>
#include <cstdint>

#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"
#include "log.h"

constexpr uint64_t START_VALID_TREE_DEPTH = 0;
constexpr uint64_t END_VALID_TREE_DEPTH = 2;
constexpr uint64_t START_VALID_PARTNER_ID = 0;
constexpr uint64_t END_VALID_PARTNER_ID = 7;

constexpr uint64_t START_INVALID_TREE_DEPTH = END_VALID_TREE_DEPTH+1;
constexpr uint64_t END_INVALID_TREE_DEPTH = (1 << 4)-1; // 0xF
constexpr uint64_t START_INVALID_PARTNER_ID = END_VALID_PARTNER_ID+1;
constexpr uint64_t END_INVALID_PARTNER_ID = (1 << 13)-1; // 0x1FFF

typedef struct {
    reduce_transform_t operation;
    uint64_t start_reg;
    uint64_t num_regs;
    uint64_t range_val;
} args_t;

template <uint64_t START, uint64_t END, typename Func, auto  func_args, auto checker>
inline void test_range(Func&& f)
{
    args_t args;
    for (uint64_t v = START; v <= END; ++v)
    {
        func_args(args);
        args.range_val = v;
        f(args);
        checker();
    }
}


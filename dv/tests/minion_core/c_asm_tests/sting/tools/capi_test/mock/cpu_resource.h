/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <random>

#include "types.h"

struct CpuResource {
    using rand_engine = std::mt19937_64;

    unum_t id;
    rand_engine rand;
    rand_engine rand_glbl;
};

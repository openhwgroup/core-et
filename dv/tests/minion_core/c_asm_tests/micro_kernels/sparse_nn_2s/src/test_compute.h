/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_COMPUTE_H_
#define _TEST_COMPUTE_H_

// Global
#include <inttypes.h>

// Defines
#define ACTIVE_SHIRES         30
#define RESULTS_PER_SHIRE     64
#define RESULTS_PER_MINION    2
#define DATA_TENSOR_ROW_ELEMS 64
#define DATA_TENSOR_ROW_SIZE  128
#define OUT_TENSOR_ROW_ELEMS  64
#define OUT_TENSOR_ROW_SIZE   256

// Entry point
void test_compute(uint32_t shire_id, uint32_t minion_id);

#endif


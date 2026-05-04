/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// 34shires*64 threads * 1line==uint64_t*8=64B
typedef struct result_struct
{
    uint64_t eot_pattern;
    uint64_t kernel_status[7];
} test_result;

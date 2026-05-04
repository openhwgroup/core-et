/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define SEED               42

#define TENSOR_WAIT_LOAD_0 0
#define TENSOR_WAIT_FMA    7

#define TENSOR_FMA16A32_OP 0b001

#define CACHE_LINE_SIZE    64

#define TENSOR_MAX_A_ROWS  16
#define TENSOR_MAX_A_COLS  32
#define TENSOR_MAX_B_ROWS  TENSOR_MAX_A_COLS
#define TENSOR_MAX_B_COLS  16

#define TENSOR_A_SIZE      (2 * TENSOR_MAX_A_ROWS * TENSOR_MAX_A_COLS)
#define TENSOR_B_SIZE      (2 * TENSOR_MAX_B_ROWS * TENSOR_MAX_B_COLS)

#define TENSOR_A_NUM_LINES (TENSOR_A_SIZE / CACHE_LINE_SIZE)
#define TENSOR_B_NUM_LINES (TENSOR_B_SIZE / CACHE_LINE_SIZE)

inline uint32_t rand(void)
{
    static uint32_t cnt = SEED;
    uint32_t a = cnt++;
    a = ((a >> 16) ^ a) * 0x45d9f3b;
    a = ((a >> 16) ^ a) * 0x45d9f3b;
    a = ((a >> 16) ^ a);
    return a;
}

inline uint64_t gen_rand_fp16(void)
{
    uint16_t exp;
    // Get exponent different than all 0s and all 1s
    do {
        exp = rand() & 0x1F;
    } while (exp == 0 || exp == 0x1F);

    return ((rand() & 1) << 15) | (exp << 10) | (rand() & 0x3FF);
}

static uint16_t A[TENSOR_MAX_A_ROWS][TENSOR_MAX_A_COLS] __attribute__((aligned(CACHE_LINE_SIZE)));
static uint16_t B[TENSOR_MAX_B_ROWS][TENSOR_MAX_B_COLS] __attribute__((aligned(CACHE_LINE_SIZE)));

int main()
{
    uint64_t hart_id = getHartId();

    if (hart_id == 0) {
        setM0MaskFF();
        initFRegs();

        // Enable SCP
        mcache_control(1,0,0);
        fence();               // VERIF-3295 (PRM-8)
        mcache_control(1,1,0);

        // Generate "random" data
        for (uint16_t i = 0; i < TENSOR_MAX_A_ROWS; i++)
            for (uint16_t j = 0; j < TENSOR_MAX_A_COLS; j++)
                A[i][j] = gen_rand_fp16();

        for (uint16_t i = 0; i < TENSOR_MAX_B_ROWS; i++)
            for (uint16_t j = 0; j < TENSOR_MAX_B_COLS; j++)
                B[i][j] = gen_rand_fp16();

        // Evict matrices to the L2
        evict_va(0, to_L2, (uint64_t)A, TENSOR_A_NUM_LINES - 1, CACHE_LINE_SIZE);
        evict_va(0, to_L2, (uint64_t)B, TENSOR_B_NUM_LINES - 1, CACHE_LINE_SIZE);

        // Load matrices into the scratchpad
        //      use_tmask  use_coop     dst_start     transformation  use_tenb   addr     offset      num_lines            stride       id
        tensor_load(0,         0,           0,              0,            0,  (uint64_t)A,   0,   TENSOR_A_NUM_LINES-1, CACHE_LINE_SIZE,  0);
        //      use_tmask  use_coop     dst_start     transformation  use_tenb   addr     offset      num_lines            stride       id
        tensor_load(0,         0,  TENSOR_A_NUM_LINES,      0,            0,  (uint64_t)B,   0,   TENSOR_B_NUM_LINES-1, CACHE_LINE_SIZE,  0);

        // Wait until the matrices are loaded
        tensorWait(TENSOR_WAIT_LOAD_0);

        // PTPX Power marker start
        __asm__ __volatile__ (
          "csrwi 0x89f, 0x1a\n"
        );

        // Start the TensorFMA16A32
        tensor_fma(
        //  use_tmask     b_num_col             a_num_rows          a_num_cols,     offset  tenc_loc
                0,    TENSOR_MAX_B_COLS-1, TENSOR_MAX_A_ROWS-1, TENSOR_MAX_A_COLS-1,   0,       0,
        //  tenb_unsigned tena_unsigned  tenb_loc      scp_loc_b      scp_loc_a        opcode      first_pass
                0,             0,            0,    TENSOR_A_NUM_LINES,     0,     TENSOR_FMA16A32_OP,   1
        );

        // PTPX Power marker end
        __asm__ __volatile__ (
          "csrwi 0x89f, 0x1b\n"
        );

        // Wait until the FMA has finished
        tensorWait(TENSOR_WAIT_FMA);
        
        // End marker
        C_TEST_PASS;
    }

    return 0;
}

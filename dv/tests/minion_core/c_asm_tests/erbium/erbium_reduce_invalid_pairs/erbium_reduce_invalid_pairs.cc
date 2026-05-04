/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"

void test_reduce_invalid();
void test_reduce_valid();


int main()
{
    setup_mtrap_vector();

    C_TEST_START;

    // Tensor reduce are only valid for thread 0
    if (get_thread_id() != 0) C_TEST_PASS;

    // Test invalid combination in thread 0
    if (get_hart_id() == 0) {
        test_reduce_invalid();
    }

    // Test valid combinations in 8 thread 0 harts
    test_reduce_valid();

    C_TEST_PASS
}

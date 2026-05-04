/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"

extern "C" void graphics_instruction_test();


int main ()
{
    C_TEST_START;

    uint64_t hart_id = get_hart_id();
    if (hart_id != 0) C_TEST_PASS;

    // Enable graphics instructions
    volatile uint64_t * minion_features = reinterpret_cast<uint64_t*>(0x80f40000);
    *minion_features = 0;
    FENCE;
        
    //Run tests
    graphics_instruction_test();
    
    C_TEST_PASS;
}

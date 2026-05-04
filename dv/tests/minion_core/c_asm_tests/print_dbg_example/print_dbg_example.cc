/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "print_dbg.h"

int main()
{
    C_TEST_START;

    print_dbg("This is regular print.");

    int t = -3;
    print_dbg("This is decimal print: %d", t);
    print_dbg("This is hexadecimal print: %x", t);
    print_dbg("Thats all folks");

    C_TEST_PASS;
    return 0;
}

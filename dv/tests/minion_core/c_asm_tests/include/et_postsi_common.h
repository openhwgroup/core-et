/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __ET_POSTSI_COMMON_H__
#define __ET_POSTSI_COMMON_H__

#include <inttypes.h>

#ifdef POSTSI_UST
#define C_TEST_PASS_HEADER  \
    __asm__ __volatile__ (  \
         "fence\n" \
         "lui x31, 0x1FEED\n" \
         "csrwi validation2, 1\n" \
         "call postsi_ust_test_end\n" \
         : : : "x31");
#endif 

void c_test_pass()
{
#ifndef POSTSI_UST
    C_TEST_PASS;
#else
    static int count = 0;
    
    if (count == 0) {
        C_TEST_PASS;
        count++;
    }
    else {
        C_TEST_PASS_HEADER;
    }
#endif
}

#endif // ! __ET_POSTSI_COMMON_H__


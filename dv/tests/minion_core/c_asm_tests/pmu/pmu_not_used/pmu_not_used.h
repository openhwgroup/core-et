/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/*
hpmcounter9 to hpmcounter31

mhpmcounter9 to mhpmcounter31

mhpmevent9 to mhpmevent31
*/

#include "macros.h"
#include "et_test_common.h"
    
__attribute__((always_inline))
inline uint64_t read_hpmcounter(uint32_t counter_id) {
    uint64_t counter_value = 0;

    switch (counter_id) {
        case 9:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter9\n" : [res] "=r" (counter_value) );
            break;
        case 10:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter10\n" : [res] "=r" (counter_value) );
            break;
        case 11:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter11\n" : [res] "=r" (counter_value) );
            break;
        case 12:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter12\n" : [res] "=r" (counter_value) );
            break;
        case 13:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter13\n" : [res] "=r" (counter_value) );
            break;
        case 14:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter14\n" : [res] "=r" (counter_value) );
            break;
        case 15:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter15\n" : [res] "=r" (counter_value) );
            break;
        case 16:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter16\n" : [res] "=r" (counter_value) );
            break;
        case 17:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter17\n" : [res] "=r" (counter_value) );
            break;
        case 18:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter18\n" : [res] "=r" (counter_value) );
            break;
        case 19:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter19\n" : [res] "=r" (counter_value) );
            break;
        case 20:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter20\n" : [res] "=r" (counter_value) );
            break;
        case 21:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter21\n" : [res] "=r" (counter_value) );
            break;
        case 22:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter22\n" : [res] "=r" (counter_value) );
            break;
        case 23:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter23\n" : [res] "=r" (counter_value) );
            break;
        case 24:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter24\n" : [res] "=r" (counter_value) );
            break;
        case 25:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter25\n" : [res] "=r" (counter_value) );
            break;
        case 26:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter26\n" : [res] "=r" (counter_value) );
            break;
        case 27:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter27\n" : [res] "=r" (counter_value) );
            break;
        case 28:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter28\n" : [res] "=r" (counter_value) );
            break;
        case 29:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter29\n" : [res] "=r" (counter_value) );
            break;
        case 30:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter30\n" : [res] "=r" (counter_value) );
            break;
        case 31:
            __asm__ __volatile__ ( "csrr %[res], hpmcounter31\n" : [res] "=r" (counter_value) );
            break;
        default:
            C_TEST_FAIL
    }

    return counter_value;
}

__attribute__((always_inline))
inline uint64_t read_mhpmcounter(uint32_t counter_id) {
    uint64_t counter_value = 0;

    switch (counter_id) {
        case 9:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter9\n" : [res] "=r" (counter_value) );
            break;
        case 10:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter10\n" : [res] "=r" (counter_value) );
            break;
        case 11:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter11\n" : [res] "=r" (counter_value) );
            break;
        case 12:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter12\n" : [res] "=r" (counter_value) );
            break;
        case 13:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter13\n" : [res] "=r" (counter_value) );
            break;
        case 14:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter14\n" : [res] "=r" (counter_value) );
            break;
        case 15:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter15\n" : [res] "=r" (counter_value) );
            break;
        case 16:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter16\n" : [res] "=r" (counter_value) );
            break;
        case 17:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter17\n" : [res] "=r" (counter_value) );
            break;
        case 18:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter18\n" : [res] "=r" (counter_value) );
            break;
        case 19:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter19\n" : [res] "=r" (counter_value) );
            break;
        case 20:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter20\n" : [res] "=r" (counter_value) );
            break;
        case 21:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter21\n" : [res] "=r" (counter_value) );
            break;
        case 22:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter22\n" : [res] "=r" (counter_value) );
            break;
        case 23:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter23\n" : [res] "=r" (counter_value) );
            break;
        case 24:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter24\n" : [res] "=r" (counter_value) );
            break;
        case 25:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter25\n" : [res] "=r" (counter_value) );
            break;
        case 26:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter26\n" : [res] "=r" (counter_value) );
            break;
        case 27:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter27\n" : [res] "=r" (counter_value) );
            break;
        case 28:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter28\n" : [res] "=r" (counter_value) );
            break;
        case 29:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter29\n" : [res] "=r" (counter_value) );
            break;
        case 30:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter30\n" : [res] "=r" (counter_value) );
            break;
        case 31:
            __asm__ __volatile__ ( "csrr %[res], mhpmcounter31\n" : [res] "=r" (counter_value) );
            break;
        default:
            C_TEST_FAIL
    }

    return counter_value;
}

__attribute__((always_inline))
inline uint64_t read_mhpmevent(uint32_t counter_id) {
    uint64_t counter_value = 0;

    switch (counter_id) {
        case 9:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent9\n" : [res] "=r" (counter_value) );
            break;
        case 10:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent10\n" : [res] "=r" (counter_value) );
            break;
        case 11:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent11\n" : [res] "=r" (counter_value) );
            break;
        case 12:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent12\n" : [res] "=r" (counter_value) );
            break;
        case 13:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent13\n" : [res] "=r" (counter_value) );
            break;
        case 14:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent14\n" : [res] "=r" (counter_value) );
            break;
        case 15:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent15\n" : [res] "=r" (counter_value) );
            break;
        case 16:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent16\n" : [res] "=r" (counter_value) );
            break;
        case 17:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent17\n" : [res] "=r" (counter_value) );
            break;
        case 18:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent18\n" : [res] "=r" (counter_value) );
            break;
        case 19:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent19\n" : [res] "=r" (counter_value) );
            break;
        case 20:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent20\n" : [res] "=r" (counter_value) );
            break;
        case 21:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent21\n" : [res] "=r" (counter_value) );
            break;
        case 22:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent22\n" : [res] "=r" (counter_value) );
            break;
        case 23:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent23\n" : [res] "=r" (counter_value) );
            break;
        case 24:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent24\n" : [res] "=r" (counter_value) );
            break;
        case 25:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent25\n" : [res] "=r" (counter_value) );
            break;
        case 26:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent26\n" : [res] "=r" (counter_value) );
            break;
        case 27:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent27\n" : [res] "=r" (counter_value) );
            break;
        case 28:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent28\n" : [res] "=r" (counter_value) );
            break;
        case 29:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent29\n" : [res] "=r" (counter_value) );
            break;
        case 30:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent30\n" : [res] "=r" (counter_value) );
            break;
        case 31:
            __asm__ __volatile__ ( "csrr %[res], mhpmevent31\n" : [res] "=r" (counter_value) );
            break;
        default:
            C_TEST_FAIL
    }

    return counter_value;
}
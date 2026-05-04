/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "et_test_common.h"
#include "cacheops.h"
#include "common.h"

volatile static uint32_t operands[32] __attribute__ ((aligned (32))) __attribute__((section(".operands_array"))) = {
        0xff800000,//-Inf
        0xc1a4cccd, //-20.6
        0x806ce3ee, // -1.0E-38-0
        0x80000000, // -zero
        0x0,  // zero
        0x006ce3ee, //1.0E-38
        0x40000000, //2
        0x7f800000, // +Inf
        0x7f800002, // signaling NaN 
        0xff800002, // -signaling NaN 
        0x7fc00001, //NANQ
        0xffc00001, //-NANQ
        0x1edcba98,
        0x12345678,
        0x12345678,
        0x1edcba98,
        0x69abcdef,
        0x38765432,
        0x7543654a,
        0x3c342985,
        0x15634579,
        0x78967365,
        0x88888888,
        0x88888888,
        0x77777777,
        0x77777777,
        0x44444444,
        0x44444444,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98};

volatile static uint32_t result[32]  __attribute__ ((aligned (32))) __attribute__((section(".result_array"))) = {
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98,
        0xfedcba98
    };


volatile static uint32_t address[32]  __attribute__ ((aligned (16))) __attribute__((section(".address_array"))) = {
        0x0,
        0x4, 
        0x8, 
        0xc, 
        0x10,  
        0x14, 
        0x18, 
        0x1c, 
    };

int main()
{
// Init marker
   C_TEST_START;

    volatile uint64_t *esr_poll_value;
    volatile uint64_t *esr_write;
    
    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) ULTRASOC_TEST_ESR_AD; 
    while ( *esr_poll_value != 0xFE );

    // Sending request to ESRs
    esr_write = (volatile uint64_t*) ULTRASOC_TEST_ESR_1_AD;
    *esr_write = 0xEE;

    // Wait for the debugger to write this ESR
    while ( *esr_poll_value != 0xAA );

    C_TEST_START;
    __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "addi t0, %[result], 0\n"

         "famoaddg.pi f0,f1(t0)\n"

         "flw.ps f0, 0(%[operands])\n"
         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
    );
   // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}

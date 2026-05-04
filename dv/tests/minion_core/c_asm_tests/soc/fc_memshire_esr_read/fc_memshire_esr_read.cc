/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"

#define MINSHIRE0_NEIGH0_IPI_REDIRECT_PC 0x80100040
#define MEMSHIRE0_MEM_CTRL_ESR 0x80C00000

#ifndef ITERATIONS
  #define ITERATIONS 10
#endif

inline __attribute__((naked)) void mtrap_vector(void)
{
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vector%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         "csrw mepc, s3\n\t"
         "mret\n\t"
         :::
   );
}

inline void setup_mtrap_vector(void)
{
    void (*mtvec_ptr)(void) = &mtrap_vector;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}


int main()
{
    // Init marker
    C_TEST_START;

    setup_mtrap_vector();

    // Do something to avoid issues due to double de-assertion of cold reset
    __asm__ __volatile__ (
        "addi a1, zero, 13\n"
        "addi a2, zero, 1\n"
	    "li a3, %[LOOP_END]\n"
        "addi a4, zero, 0\n"
        "loop:\n"
        "div  a1, a1, a2\n"
        "add  a4, a1, a1\n"
        "add  a3, a3, -1\n"
        "bne  a3, zero, loop\n"            
	: 
    : [LOOP_END] "i" (ITERATIONS)
    : "a1", "a2", "a3", "a4"
    );

    volatile uint64_t *esr_poll_addr;
    volatile uint64_t *esr_read_addr;
    volatile uint64_t esr_read_value;

    // We only want MinShire 0, Minion 0 and Thread 0
    if ( get_shire_id() == 0 and get_hart_id() == 0) 
    {
        // Wait for the debugger to write this ESR to unblock the minions
        esr_poll_addr = (volatile uint64_t*) MINSHIRE0_NEIGH0_IPI_REDIRECT_PC; 
        while ( *esr_poll_addr != 0xFE );

        // Perform a read request that will be captured by the debug modules
        esr_read_addr = (volatile uint64_t*) MEMSHIRE0_MEM_CTRL_ESR;
        esr_read_value = *esr_read_addr;
        esr_read_value = esr_read_value -1;
    }

    // Finish minion test (it will perform a WFI)
   C_TEST_PASS;
   return 0;
}




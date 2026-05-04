/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 10
#include "macros.h"
#include <inttypes.h>

#define BUF_SIZE (1 << 20)
#define BUF_SLOT BUF_SIZE/256

#define IPI_THREAD0    0x803400C0ULL
#define IPI_THREAD1    0x803400D0ULL
#define ACTIVE_T1_MASK 0xFFFFFFFFFFFFFFFFULL // Mask of minions that have thread 1 active
#define ACTIVE_T0_M1   63 // Minions that have thread 0 active minus 1

uint64_t ipi_to_o_t_addr1 = IPI_THREAD1;
uint64_t ipi_to_o_t_addr0 = IPI_THREAD0;
uint64_t ipi_to_o_t_mask  = ACTIVE_T1_MASK;
uint64_t barrier_o_t      = ACTIVE_T0_M1 << 3;

int main()
{
    C_TEST_START;
    __asm__ __volatile__ (
        // Sends an IPI to the other thread
        // Uses a barrier to send an IPI from last thread

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done
        "li a1,  0x80340000\n"
        "li a2, 0x20\n"
        "loop: addi a2, a2, -1\n"
        "add a3, a2, %[barrier_o_t]\n"
        "add a4, a1, a2\n"
        "sd zero, 0(a4)\n"
        "csrrw  x31, 0x820, a3\n"
        "beq    x31, x0, skip_ipi_ot_helper_0\n"
        "sd     %[ipi_to_o_t_mask], 0(%[ipi_to_o_t_addr0])\n"
        "sd     %[ipi_to_o_t_mask], 0(%[ipi_to_o_t_addr1])\n"
        "skip_ipi_ot_helper_0:\n"
        // Waits for the other thread to send the prefetch signal
        "csrrw x31, 0x821, x0\n"
        "bne x0, a2, loop\n"
            :
        [barrier_o_t] "+r" (barrier_o_t)
            :
        [ipi_to_o_t_addr0] "r" (ipi_to_o_t_addr0),
        [ipi_to_o_t_addr1] "r" (ipi_to_o_t_addr1),
        [ipi_to_o_t_mask] "r" (ipi_to_o_t_mask)
            : "a4", "a5", "a2", "a3"
    );

    C_TEST_PASS;
    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();

void machine_handler() __attribute__((interrupt("machine"), aligned(4)));

#define MRET_TO_SMODE_END()                                      \
    do {                                                         \
        void *target;                                            \
        asm volatile (                                           \
            "auipc %0, 0\n\t"                                    \
            "addi %0, %0, 40\n\t"                                \
            "li t1, 0x1800\n\t"     /* reg holds large value */  \
            "csrc mstatus, t1\n\t"  /* clear using reg */        \
            "li t0, 0x800\n\t"     /* MPP=01, MIE=1 */           \
            "csrs mstatus, t0\n\t"                               \
            "csrw mepc, %0\n\t"                                  \
            "mret\n\t"                                           \
            "nop;nop;nop;nop\n\t"                                \
            : "=r"(target) : : "t0", "t1", "memory"              \
        );                                                       \
    } while(0)

    #define MRET_TO_UMODE_END()                                  \
    do {                                                         \
        void *target;                                            \
        asm volatile (                                           \
            "auipc %0, 0\n\t"                                    \
            "addi %0, %0, 32\n\t"                                \
            "li t1, 0x1800\n\t"     /* reg holds large value */  \
            "csrc mstatus, t1\n\t"  /* clear using reg */        \
            "csrw mepc, %0\n\t"                                  \
            "mret\n\t"                                           \
            "nop;nop;nop;nop\n\t"                                \
            : "=r"(target) : :"t1", "memory"                     \
        );                                                       \
    } while(0)
 

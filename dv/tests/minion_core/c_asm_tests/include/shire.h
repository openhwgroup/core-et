/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef SHIRE_H
#define SHIRE_H

// Auxiliar functions
inline unsigned int get_hart_id() __attribute__((always_inline));
inline unsigned int get_shire_id() __attribute__((always_inline));
inline unsigned int get_neigh_id() __attribute__((always_inline));
inline unsigned int get_minion_id() __attribute__((always_inline));
inline unsigned int get_thread_id() __attribute__((always_inline));

// Function that returns shire id
unsigned int get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], hartid\n" // u-mode hartid shadow
        : [ret] "=r" (ret)
    );
    return ret;
}

// Function that returns shire id
unsigned int get_shire_id()
{
    return get_hart_id() >> 6;
}

unsigned int get_neigh_id()
{
    return (get_hart_id() >> 4) & 3;
}

// Function that returns minion id
unsigned int get_minion_id()
{
    return get_hart_id() >> 1;
}

// functions to deal with CSR regs
unsigned int get_thread_id()
{
    return get_hart_id() & 1;
}

#endif // ! SHIRE_H


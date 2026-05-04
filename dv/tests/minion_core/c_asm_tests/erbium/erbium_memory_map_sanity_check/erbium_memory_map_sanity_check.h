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

enum class region_type {
    MRAM,
    BOOTROM,
    SRAM,
    SYSTEMREGS,
    CPUREGS,
    OTP,
    IO,
    HYPERBUS,
    MRAMREGS
};

typedef struct {
    const char *name;
    region_type type;
    uint64_t start_address;
    uint64_t byte_size;
    bool allow_read;
    bool allow_write;
    bool allow_exec;
    bool allow_amo;
    bool allow_tensor;
    bool allow_cacheop;
    bool allow_m_mode;
    bool allow_s_mode;
    bool allow_u_mode;
} mem_region_t;

// static mem_region_t regions[] = {
//     // name          type,                   start_addr.    size.       read.  write. exec.  amo.   tens.  cache   m.     s.    u
//     { "MRAM",        region_type::MRAM,      0x40050000,    0x0010000,  true,  true,  true,  true,  true,  true,  true,  true,  true },
//     { "Boot ROM",    region_type::BOOTROM,   0x200a000,     0x2000,     true,  false, true,  false, false, false, true,  true,  true },/* cacheops are not allowed while the sheet says they are*/
//     { "SRAM",        region_type::SRAM,      0x200e000,     0x800,      true,  true,  true,  false, true,  true,  true,  true,  true },
//     { "CPU Regs",    region_type::CPUREGS,   0x80000000,    0x80000000, true,  true,  false, false, false, false, true,  true,  true },
//     { "OTP",         region_type::OTP,       0x41000000,    0x1000,     true,  false, false, false, false, false, true,  true,  true },/* what does bus error interrupts translate to??*/
//     { "IO",          region_type::IO,        0x2002000,     0x1000,     true,  true,  false, false, false, false, true,  true,  false},
//     { "Hyperbus",    region_type::HYPERBUS,  0x2003000,     0x1000,     true,  true,  false, false, false, false, true,  false, false},
//     { "System Regs", region_type::SYSTEMREGS,0x2000000,     0x1000,     true,  true,  false, false, false, false, true,  true,  false},
//     { "MRAM Regs",   region_type::MRAMREGS,  0x2001000,     0x1000,     true,  true,  false, false, false, false, true,  false, false}
// };

// static unsigned regions_count = sizeof(regions) / sizeof(regions[0]);

void machine_handler() __attribute__((interrupt("machine"), aligned(4096)));

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
 
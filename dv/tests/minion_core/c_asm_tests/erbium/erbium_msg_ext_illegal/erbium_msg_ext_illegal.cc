/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"

constexpr uint64_t CAUSE_ACCESS_FAULT = 7;
constexpr uint64_t CAUSE_BUS_ERROR = 23 | (1ULL << 63);
constexpr uint64_t NUM_PORTS = 4;

// Keep 1 line spacing between variables to avoid aliasing
// This should be solved with thread-local storage at some point...
bool got_xcpt[NUM_HARTS_PER_SHIRE][L1_CL_SIZE];
// Convinent setter/getter for got_xcpt
inline void set_got_xcpt(bool x) { got_xcpt[get_hart_id()][0] = x; }
inline bool get_got_xcpt() { return got_xcpt[get_hart_id()][0]; }


inline uint64_t msg_addr_old_memmap(uint64_t hid, uint64_t pid)
{
    hid = hid & 0xFF;
    uint64_t addr =
        1ULL << 32  |
        (hid << 12) |
        (1   << 11) |
        (pid << 6 );
    return addr;
}

inline uint64_t msg_addr_new_memmap(uint64_t hid, uint64_t pid)
{
    hid = hid & 0xFF;
    uint64_t addr =
        1ULL << 31  |
        (hid << 12) |
        (1   << 11) |
        (pid << 6 );
    return addr;
}

template<typename Func>
void msg_send(uint64_t hid, uint64_t pid, Func && gen_addr)
{
    volatile uint64_t *ptr = reinterpret_cast<volatile uint64_t*>(gen_addr(hid, pid));

    // This block will generate a store and wait for the response.
    // The expected behaviors is to get an xcpt/bus error interrupt.
    // The handler will set go_xcpt to mask that the trap took place
    set_got_xcpt(false);
    asm volatile (
        "sd x0, %[addr]\n"
        "fence\n"
        "nop\n"
        :
        : [addr] "m" (*ptr)
        : );

    if (!get_got_xcpt())
        C_TEST_FAIL;
}

// TODO: ANT: Change aligment when changes in mtvec required aligment propagate to emu
extern "C" void machine_handler() __attribute__((interrupt("machine"), aligned(4096)));
void machine_handler()
{
    uint64_t mcause, mepc;
    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    asm volatile ("csrr %0, mepc"   : "=r"(mepc));
    if (mcause != CAUSE_ACCESS_FAULT &&
        mcause != CAUSE_BUS_ERROR)
        C_TEST_FAIL;
    set_got_xcpt(true);
    asm volatile ("csrw mip, x0" :::);
    auto next_pc = mcause == CAUSE_BUS_ERROR ? mepc : mepc + 4;
    asm volatile ("csrw mepc, %0" :: "r"(next_pc));
}

void setup_trap_vector()
{
    __asm__ __volatile__
    (
     "la t0, machine_handler\n"
     "csrw mtvec, t0\n"
     "li t0, (1 << 23)\n"
     "csrs mie, t0\n"       // Enable bus error interrupts
     "csrsi mstatus, 8\n"  // Enable interrupts globaly
     : : : "t0"
    );
}

int main()
{
    C_TEST_START;

    setup_trap_vector();

    for (uint64_t h = 0; h < NUM_HARTS_PER_SHIRE; ++h)
    {
        #pragma GCC unroll NUM_PORTS
        for (uint64_t p = 0; p < NUM_PORTS; ++p)
        {
            msg_send(h, p, msg_addr_new_memmap);
            msg_send(h, p, msg_addr_old_memmap);
        }
    }

    C_TEST_PASS;
}

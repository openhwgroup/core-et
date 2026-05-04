/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "esr.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "rtlmin_6562.h"

void set_pass_trap_vector()
{
    //configure trap vector
    uint64_t mie = 1 << 3; // Set MSIE
    uint64_t mstatus = 1 << 3; // Enable MIE interrupts
    __asm__ __volatile__
        (
            "la t0, test_pass_vector\n"
            "csrw mtvec, t0\n"
            "csrs mie, %[mie]\n"
            "csrs mstatus, %[mstatus]\n"
            :
            : [mie] "r" (mie), [mstatus] "r" (mstatus)
            : "t0"
        );

}

void run_as_stuck_minion_int()
{
    constexpr uint8_t NO_TMASK = 0;
    constexpr uint8_t NUM_LINES = 1;

    uint8_t shire_id = get_shire_id();
    volatile uint64_t *fcc_addr = (volatile uint64_t*) ESR_SHIRE(shire_id, FCC_CREDINC_0);

    minion_cache_line_t cache_line;
    uint64_t addr = reinterpret_cast<uint64_t>(&cache_line);

    set_pass_trap_vector();

    // Evict the line first to prevent hitting the cache
    evict_va(NO_TMASK, to_L3, addr, NUM_LINES);
    WAIT_CACHEOPS

    // Delay the response for addresses
    et_delay_response(addr, MEM_REQ_DELAY);

    *fcc_addr = HELPER_HART_FCC_MASK;

    // Be careful, this registers may colide with the ones in trap handler
    __asm__ volatile (
        ".align 5\n"
        "fence\n"
        "addi t1, x0, 1\n"
        "ld t2, %[addr]\n"
        "add t2, t2, t1\n"
        :
        : [addr] "m" (cache_line)
        : "t1", "t2"
    );


}

void run_as_stuck_minion_fp()
{
    constexpr uint8_t NO_TMASK = 0;
    constexpr uint8_t NUM_LINES = 1;

    uint8_t shire_id = get_shire_id();
    volatile uint64_t *fcc_addr = (volatile uint64_t*) ESR_SHIRE(shire_id, FCC_CREDINC_0);

    minion_cache_line_t cache_line;
    uint64_t addr = reinterpret_cast<uint64_t>(&cache_line);

    set_pass_trap_vector();

    // Evict the line first to prevent hitting the cache
    evict_va(NO_TMASK, to_L3, addr, NUM_LINES);
    WAIT_CACHEOPS

    // Delay the response for addresses
    et_delay_response(addr, MEM_REQ_DELAY);

    *fcc_addr = HELPER_HART_FCC_MASK;

    // Be careful, this registers may colide with the ones in trap handler
    __asm__ volatile (
        ".align 5\n"
        "fence\n"
        "mov.m.x m0, x0, 0x1\n"
        "li t1, 1\n"
        "fcvt.s.w f1, t1\n"
        "flw f2, %[addr]\n"
        "fadd.s f2, f2, f1\n"
        :
        : [addr] "m" (cache_line)
        : "t1", "f1", "f2"
    );
}

void run_as_stuck_minion_csr()
{
    uint8_t shire_id = get_shire_id();
    volatile uint64_t *fcc_addr = (volatile uint64_t*) ESR_SHIRE(shire_id, FCC_CREDINC_0);

    set_pass_trap_vector();

    *fcc_addr = HELPER_HART_FCC_MASK;

    wait_for_credit(FCC_REG);
}


// Wait until the other minion is ready and
// trigger the interrupt
void run_as_helper()
{
    for (int32_t i = 0; i < NUM_HARTS_STUCK; ++i)
        wait_for_credit(FCC_REG);

    uint8_t shire_id = get_shire_id();
    volatile uint64_t* ipi_trigger = (volatile uint64_t*) ESR_SHIRE(shire_id, IPI_TRIGGER);
    *ipi_trigger = IPI_MASK;
}

int main() {

    uint64_t hart_id = get_hart_id();

    C_TEST_START;

    setup_mtrap_vector();

    switch(hart_id)
    {
        case HELPER_HART:  run_as_helper(); break;
        case FP_HART:      run_as_stuck_minion_fp(); break;
        case INT_HART:     run_as_stuck_minion_int(); break;
        case CSR_HART:     run_as_stuck_minion_csr(); break;
        default: break;
    }

    C_TEST_PASS;

}

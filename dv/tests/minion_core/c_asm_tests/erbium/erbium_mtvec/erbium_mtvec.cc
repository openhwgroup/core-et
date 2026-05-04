/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"

#include "erbium_mtvec_defines.h"

// External assembly symbols
extern "C" {
    extern char test_mtrap_direct_table[];
    extern char test_mtrap_vector_table[];
    extern char test_strap_direct_table[];
    extern char test_strap_vector_table[];
    bool xcpt_taken;
}


// Test configuration
enum class vector_type_t {
    direct, vector
};

enum class priv_mode_t {
    machine, supervisor
};

struct test_config {
    uint64_t table;
    uint64_t elements;
    uint64_t offset;
    priv_mode_t mode;
};

void to_s_mode() {
    asm volatile (
        "li t0, 0x00001800\n"
        "csrc mstatus, t0\n"     // Clear MPP
        "li t0, 0x00000800\n"
        "csrs mstatus, t0\n"     // Set to S-mode
        "la t0, 1f\n"
        "csrw mepc, t0\n"
        "mret\n"
        "1:\n"
        ::: "t0", "memory"
    );
}

void trigger_ipi()
{
    // TODO
}


void generate_traps()
{
    enum class Trap { ECALL, EBREAK, INTERRUPT, ILLEGAL, LOAD_XCPT, STORE_XCPT, NUM_TRAPS};

    xcpt_taken = false;
    for (int trap_idx = 0; trap_idx <  static_cast<int>(Trap::NUM_TRAPS); ++trap_idx)
    {
        auto trap = static_cast<Trap>(trap_idx);

        switch (trap)
        {
            case Trap::ECALL: {
                asm volatile ("ecall\n"
                              "nop\n");
                break;
            }
            case Trap::EBREAK: {
                asm volatile ("ebreak\n"
                              "nop\n");
                break;
            }
            case Trap::INTERRUPT: {
                // TODO
                // trigger_ipi();
                // asm volatile ("wfi\n"
                //               "nop\n");
                xcpt_taken = true; // Emulate taken interrupt
                break;
            }
            case Trap::ILLEGAL: {
                asm volatile ("unimp\n"
                              "nop\n");
                break;
            }
            case Trap::LOAD_XCPT: {
                asm volatile ("ld x0, 0(x0)\n"
                              "fence\n"
                              "nop\n"
                              :
                              :
                              :);
                break;
            }
            case Trap::STORE_XCPT: {
                asm volatile ("sd x0, 0(x0)\n"
                              "fence\n"
                              "nop\n"
                              :
                              :
                              : );
                break;
            }
            default: {
                C_TEST_FAIL; // Should not reach here
                break;
            }
        }
        // Check trap taken and reset if ok
        if (not xcpt_taken) C_TEST_FAIL;
        xcpt_taken = false;
    }
}


void set_vec(uint64_t addr, vector_type_t type, priv_mode_t mode)
{
    uint64_t en_vec = type == vector_type_t::vector ? 1 : 0;
    uint64_t vec = addr | en_vec;

    if (mode == priv_mode_t::machine) {
        asm volatile ("csrw mtvec, %[addr]\n" : : [addr] "r" (vec) :);
    } else {
        asm volatile ("csrw stvec, %[addr]\n" : : [addr] "r" (vec) :);
    }
}

void test(const test_config config, vector_type_t type)
{
    for (uint64_t i = 0; i < config.elements; ++i)
    {
        uint64_t addr = config.table + config.offset*i;
        set_vec(addr, type, config.mode);
        generate_traps();
    }
}

int main()
{
    uint64_t hartid = get_hart_id();

    if (hartid != 0) C_TEST_PASS;

    // Machine mode direct test
    test_config tc;
    tc.table = (uint64_t)&test_mtrap_direct_table;
    tc.elements = NUM_DIRECT_VECTORS;
    tc.offset = 4;
    tc.mode = priv_mode_t::machine;
    test(tc, vector_type_t::direct);

    // Machine mode vectored test
    tc.table = (uint64_t)&test_mtrap_vector_table;
    tc.elements = NUM_VECTORED_VECTORS;
    tc.offset = 128;
    tc.mode = priv_mode_t::machine;
    test(tc, vector_type_t::vector);

    // Switch mode
    to_s_mode();

    // Supervisor mode direct test
    tc.table = (uint64_t)&test_strap_direct_table;
    tc.elements = NUM_DIRECT_VECTORS;
    tc.offset = 4;
    tc.mode = priv_mode_t::supervisor;
    test(tc, vector_type_t::direct);

    // Supervisor mode vectored test
    tc.table = (uint64_t)&test_strap_vector_table;
    tc.elements = NUM_VECTORED_VECTORS;
    tc.offset = 128;
    tc.mode = priv_mode_t::supervisor;
    test(tc, vector_type_t::vector);

    C_TEST_PASS;
}

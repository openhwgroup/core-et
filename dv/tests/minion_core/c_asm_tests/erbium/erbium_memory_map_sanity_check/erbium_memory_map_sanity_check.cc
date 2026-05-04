/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"
#include "erbium_memory_map_sanity_check.h"
#include <string.h>
#include "tensors.h"
#include <string.h>
#include "cacheops.h"
// Test assumes vm off

volatile uint64_t error_count = 0;
volatile uint64_t illegal_trap_on_tensor_count = 0;
volatile uint64_t illegal_trap_on_cacheop_count = 0;
volatile uint64_t trap_address = 0;

void incr_error_count(void) {
        // asm volatile ("ecall;nop");
        error_count++;
        // C_TEST_FAIL;
}

void c_test_final_fail(void) {
        asm volatile ("ecall;nop");
        C_TEST_FAIL;
}

void assert_vm_off()
{
    uint64_t matp, satp;
    asm volatile ("csrr %[reg], matp\n" : [reg] "=r" (matp)::);
    asm volatile ("csrr %[reg], satp\n" : [reg] "=r" (satp)::);

    if (matp != 0 || satp != 0) C_TEST_FAIL;
}

// extern  mem_region_t regions[];
// extern  unsigned regions_count;
static uint64_t test_exec_return_pc;

typedef struct {
    uint64_t mcause;
    uint64_t mtval;
    uint64_t mepc;
    uint64_t mstatus;
} trap_info_t;

volatile trap_info_t g_last_trap = {0, 0, 0, 0};


inline void clear_last_trap(void)
{
    g_last_trap.mcause = 0;
    g_last_trap.mtval  = 0;
    g_last_trap.mepc   = 0;
}

inline bool trap_happened(void)
{
    return g_last_trap.mcause != 0;
}

void setup_trap_vector()
{
    __asm__ __volatile__ 
    (
     "la t0, machine_handler\n"
    //  "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
      //enable interrupt
     "li t0, 1\n"
     "slli t0, t0, 23\n"
     "csrs mie, t0\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
    );
}


void machine_handler()
{
    uint64_t mcause, mtval, mepc, mstatus;
    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    asm volatile ("csrr %0, mtval"  : "=r"(mtval));
    asm volatile ("csrr %0, mepc"   : "=r"(mepc));
    asm volatile ("csrr %0, mstatus": "=r"(mstatus));

    g_last_trap.mcause  = mcause;
    g_last_trap.mtval   = mtval;
    g_last_trap.mepc    = mepc;
    g_last_trap.mstatus = mstatus;
    // Skip offending instruction (assumes 4‑byte instruction)

    if (mcause == 8 || mcause == 9 || mcause == 11) {  // U/S ecall
        mstatus = (mstatus & ~0x1800UL) | (3UL << 11);  //  MPP = M-mode
        
        asm volatile (
            "csrw mstatus, %0\n\t"
            ::"r"(mstatus)
        );
        mepc += 4;
    }
    else if (mcause == 5 || mcause == 7) {  // load store access
        mepc += 4;
    }    
    else if (mcause == 1) {  // instruction access
        mepc = test_exec_return_pc;  
    }
    else if (mcause == 0x8000000000000017) {
        // mepc += 4;
        asm volatile (
        //enable interrupt
        "li t0, 1\n"
        "slli t0, t0, 23\n"
        "csrc mip, t0\n"
        );
    }
    //TODO confirm cases for illegal
    else if (mcause == 2) {  // illegal instr
        mepc += 4;
    } 
    else {
        C_TEST_FAIL;
    }
    

    asm volatile ("csrw mepc, %0" :: "r"(mepc));
}


bool test_read64(uint64_t addr)
{
    volatile uint64_t *p = (volatile uint64_t *)addr;
    clear_last_trap();
    uint64_t val = 0;
    // Potentially faulting load
    val = *p;
    (void)val;
    return !trap_happened();  // true => access OK
}

bool test_write64(uint64_t addr, uint64_t data)
{
    volatile uint64_t *p = (volatile uint64_t *)addr;
    clear_last_trap();
    *p = data;                // potentially faulting store
    return !trap_happened();  // true => access OK
}

bool test_read32(uint64_t addr)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    clear_last_trap();
    uint32_t val = 0;
    // Potentially faulting load
    val = *p;
    (void)val;
    return !trap_happened();  // true => access OK
}

bool test_write32(uint64_t addr, uint64_t data)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    clear_last_trap();
    *p = data;                // potentially faulting store
    return !trap_happened();  // true => access OK
}

uint64_t custom_atomic_fetch_add_64(volatile uint64_t *addr, uint64_t inc)
{
    uint64_t old;

    clear_last_trap();
    __asm__ __volatile__ (
        "fence\n\t"                        // full barrier before
        "amoaddl.w %0, %2, (%1)\n\t"       // old = *addr; *addr += inc
        "fence\n\t"                        // full barrier after (if you want seq_cst)
        : "=&r"(old)                       // %0 = rd (old value)
        : "r"(addr),                       // %1 = rs1 (address)
          "r"(inc)                         // %2 = rs2 (increment)
        : "memory"
    );

    return old;
}

bool test_executable(uint64_t addr, bool flush_supported)
{
    // Write NOP + RET sequence. Make sure to flush the line from D$.
    if (!test_write32(addr, 0x00008067)) return false;  // jalr zero, ra, 0
    FENCE;
    //       use_tmask  dst  addr  num_lines  stride  id  warl
    //       ---------  ---  ----  ---------  ------  --  ----
    if (flush_supported) {
        flush_va(    false,   1, addr,         0,      0,  0,    0);
        FENCE;
        WAIT_CACHEOPS;

        clear_last_trap();
    }
    test_exec_return_pc = (uint64_t)__extension__ (&&inst_access_trap_return);

    // Simple jump - let trap handler return to here
    asm volatile ("jalr ra, %0, 0" : : "r"(addr) : "memory");

inst_access_trap_return:
    // Continues here after mret (whether success or trap)
    return !trap_happened();
}

bool test_add64_atomic(uint64_t addr, uint64_t increment/*, uint64_t expected_result*/)
{
    uint64_t prev;
    
    clear_last_trap();
    
    prev = custom_atomic_fetch_add_64((volatile uint64_t*)addr, increment);

    // expected_result = prev;
    prev+=0;
    
    return !trap_happened()/* && (prev + increment == expected_result)*/;
}


static inline void clear_tensor_error(void)
{
    // Write 0 to tensor_error CSR (0x808)
    asm volatile ("csrw 0x808, zero" ::: "memory");
}

static inline uint64_t read_tensor_error(void)
{
    uint64_t v;
    asm volatile ("csrr %0, 0x808" : "=r"(v));
    return v;
}

bool test_tensor_op(uint64_t addr)
{
    // Clear previous tensor error state
    clear_tensor_error();
    clear_last_trap();
    uint64_t stride = 1;
    uint64_t id = 0;

    uint64_t csr_enc = ((addr & 0xFFFFFFFFFFC0ULL));
    uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

    __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         "fence\n"
         :
         : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
         : "x31"
    );
    WAIT_TENSOR_LOAD_0;

    // 3. Read tensor_error CSR and check bit 7
    uint64_t terr = read_tensor_error();
    bool error = (terr >> 7) & 0x1;

    if (trap_happened()) {
        illegal_trap_on_tensor_count++;
        trap_address = addr;
    }
    // Return true on success (no error bit set)
    return !error && !trap_happened();
}

bool test_cache_op(uint64_t addr)
{
    // Clear previous tensor error state
    clear_tensor_error();
    clear_last_trap();
    uint64_t stride = 1;
    uint64_t id = 0;
    uint64_t dest = 0x3;

    uint64_t csr_enc = dest << 58 |
                        ((addr & 0xFFFFFFFFFFC0ULL));
    uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

    __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x8bf, %[csr_enc]\n"
         "fence\n"
         :
         : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
         : "x31"
    );
    WAIT_CACHEOPS;


    // Read tensor_error CSR and check bit 7
    uint64_t terr = read_tensor_error();
    bool error = (terr >> 7) & 0x1;

    if (trap_happened()) {
        illegal_trap_on_cacheop_count++;
        trap_address = addr;
    }
    // Return true on success (no error bit set)
    return !error && !trap_happened(); // TODO where in spec?? for some region like CSR reg it gives illegal instruction, for some it sets the tensor error bit
}

typedef enum {
    MODE_U = 0,
    MODE_S = 1,
    MODE_D = 2,
    MODE_M = 3
} priv_mode_t;

// For now: test only M‑mode attributes (allow_m_mode + allow_read/allow_write).
void test_region_rw_mmode(uint64_t test_addr, bool is_32_bit, bool is_64_bit, priv_mode_t mode, bool allow_read, bool allow_write, bool allow_exec, bool allow_amo, bool allow_tensor, bool allow_cacheop, bool allow_m_mode, bool allow_s_mode, bool allow_u_mode)
{
    bool allow_priv_mode = (mode == MODE_M && allow_m_mode ) || (mode == MODE_S && allow_s_mode ) || (mode == MODE_U && allow_u_mode);
    if (mode == MODE_U) {
        MRET_TO_UMODE_END();
    }
    else if (mode == MODE_S) {
        MRET_TO_SMODE_END();
    } else {
    }

        // READ32
        bool ok_read32 = test_read32(test_addr);
        bool allow_read_priv = allow_read && allow_priv_mode && is_32_bit;
        if ((allow_read_priv != ok_read32)) {
            incr_error_count();
            trap_address = test_addr;
        }

        // READ64
        bool ok_read64 = test_read64(test_addr);
        bool allow_read_priv64 = allow_read && allow_priv_mode && is_64_bit;
        if ((allow_read_priv64  != ok_read64)) {
            incr_error_count();
            trap_address = test_addr;
        }

        // WRITE
        bool ok_write32 = test_write32(test_addr, 0xffffffff/*0xA5A5A5A5*/);
        bool allow_write_priv = allow_write && allow_priv_mode && is_32_bit;
        if ((allow_write_priv != ok_write32)) {
            incr_error_count();
            trap_address = test_addr;
        }

        // WRITE64
        bool ok_write64 = test_write64(test_addr, 0xffffffff/*0xA5A5A5A5*/);
        bool allow_write_priv64 = allow_write && allow_priv_mode && is_64_bit;
        if ((allow_write_priv64 != ok_write64)) {
            incr_error_count();
            trap_address = test_addr;
        }

        // ATOMIC
        bool ok_amo = test_add64_atomic(test_addr, 0x0/*, *(volatile uint64_t*)test_addr*/);
        bool allow_amo_priv = allow_amo && allow_priv_mode;
        if ((allow_amo_priv != ok_amo)) {
            incr_error_count();
            trap_address = test_addr;
        }

        // TODO: somehow the memory region is getting overwritten and modifying the excuting instructions, debug later
        // EXECUTE
        // if (allow_write) {
        //     if (!(test_addr >= 0x2008000 && test_addr < 0x2008000+0x2000)) { //TODO: skipping bootrom
        //         bool ok_execute = test_executable(test_addr, allow_cacheop);
        //         bool allow_exec_priv = allow_exec && allow_priv_mode;
        //         if ((allow_exec_priv != ok_execute)) {
        //         incr_error_count();
        //         trap_address = test_addr;
        //         }
        //     }
        // }
        allow_exec=!allow_exec;
        
        // TENSOR
            bool ok_tensor = test_tensor_op(test_addr);
            bool allow_tensor_priv = allow_tensor && allow_priv_mode;
            if ((allow_tensor_priv != ok_tensor)) {
            incr_error_count();
            trap_address = test_addr;
            }
        // CACHEOP
            bool ok_cacheop = test_cache_op(test_addr);
            bool allow_cacheop_priv = allow_cacheop && allow_priv_mode;
            if ((allow_cacheop_priv != ok_cacheop)) {
            incr_error_count();
            trap_address = test_addr;
            }

    // }
    if (mode == MODE_U || mode == MODE_S) {
        //ECALL to return to M mode via handler
        asm volatile ("ecall;nop");
    }
}

int main()
{
    assert_vm_off();

    setup_trap_vector();

    C_TEST_START;

    uint64_t thread_id = get_thread_id();

    if (thread_id != 0) {
        C_TEST_PASS;
    }

    setup_cache_scp();
    // mcache_control(1,1,1);

        for (int mode = PRV_U; mode <= PRV_M; mode++) {
            if (mode == PRV_D) continue;   
            //MRAM
            test_region_rw_mmode(0x40050000/*MRAM*/, 1/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 1 /*exec*/, 1 /*amo*/, 1 /*tens*/, 1 /*cacheop*/, 1 /*M*/, 1 /*S*/, 1 /*U*/);

            //BOOTROM
            test_region_rw_mmode(0x2008000/*BOOTROM*/,  1/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 0/*write*/, 1 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*check TODO*/ /*cacheop*/, 1 /*M*/, 1 /*S*/, 1 /*U*/); //TODO https://github.com/nekkoai/roadmap/issues/270

            //SRAM
            test_region_rw_mmode(0x200c000/*SRAM*/,  1/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 1 /*exec*/, 0 /*amo*/, 1 /*tens*/, 1 /*cacheop*/, 1 /*M*/, 1 /*S*/, 1 /*U*/);

            //CPU Regs
            //CPU regs S-shire shire_coop_mode
            test_region_rw_mmode(0x80740300/**/,  0/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

            //CPU regs U-shire icache u prefetch
            test_region_rw_mmode(0x803402f8/**/,  0/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 1 /*U*/);
 
            // credit 0
            test_region_rw_mmode(0x803400c0/**/,  0/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 1 /*U*/);

            // M shire minion feature cpu regs
            test_region_rw_mmode(0x80f40000/**/,  0/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode), 1 /*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 0 /*S*/, 0 /*U*/);
            // end CPU Regs

            //MRAM regs // the first register is arbiter_mode_reg register
            //issue
            test_region_rw_mmode(0x2001000,  0/*is_32_bit*/, 1/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 0 /*S*/, 0 /*U*/);

            //System regs // mailbox0 register at offset 0x68
            test_region_rw_mmode(0x2000068,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

            //I2C regs // wdata register at offset 0x20
            //issues
            // illegal instruction when executing cache_flush va in M mode
            test_region_rw_mmode(0x2002020,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

            //QSPI regs // dll register at offset 0x20
            //issues
            test_region_rw_mmode(0x2003020,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

            //UART regs // Tx register at offset 0x8
            test_region_rw_mmode(0x2004008,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

            //NIC config registers // bus config at offset 0x0
            test_region_rw_mmode(0xfe000000,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 0 /*S*/, 0 /*U*/);

            //PLIC config registers // source 0 prio at offset 0x4
            test_region_rw_mmode(0xc0000004,  1/*is_32_bit*/, 0/*is_64_bit*/, priv_mode_t(mode),  1/*read*/, 1/*write*/, 0 /*exec*/, 0 /*amo*/, 0 /*tens*/, 0 /*cacheop*/, 1 /*M*/, 1 /*S*/, 0 /*U*/);

        }

        if (error_count > 1) {
            c_test_final_fail();
        }

        C_TEST_PASS;
}


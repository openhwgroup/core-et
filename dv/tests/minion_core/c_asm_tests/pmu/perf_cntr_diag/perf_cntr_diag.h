/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#include <cstdint>

#define PMU_MHPMCOUNTER3 0xB03
#define PMU_MHPMCOUNTER4 0xB04
#define PMU_MHPMCOUNTER5 0xB05
#define PMU_MHPMCOUNTER6 0xB06
#define PMU_MHPMCOUNTER7 0xB07
#define PMU_MHPMCOUNTER8 0xB08

#define PMU_MHPMEVENT3 0x323
#define PMU_MHPMEVENT4 0x324
#define PMU_MHPMEVENT5 0x325
#define PMU_MHPMEVENT6 0x326
#define PMU_MHPMEVENT7 0x327
#define PMU_MHPMEVENT8 0x328

enum PmuNeighEvent {
      pmu_neigh_event_NOCOUNT         = 0,  // No count
      pmu_neigh_event_MIN_ET_REQ      = 1,  // Minion sends ET Link REQ
      pmu_neigh_event_MIN_ET_RSP      = 2,  // Minion receives ET Link RSP
      pmu_neigh_event_COOP_LD_REQ     = 3,  // Coop Load REQ sent
      pmu_neigh_event_IN_COOP_LD_REQ  = 4,  // Inter-Neigh Coop Load REQ sent
      pmu_neigh_event_COOP_LD_RSP     = 5,  // Coop Load RSP received
      pmu_neigh_event_COOP_ST_REQ     = 6,  // Coop Store REQ sent
      pmu_neigh_event_COOP_ST_RSP     = 7,  // Coop Store RSP received

      pmu_neigh_event_MIN_ICACHE_REQ  = 8,  // Minion sneds REQ to ICache
      pmu_neigh_event_MIN_ICACHE_RSP  = 9,  // Minion receives RSP from ICache
      pmu_neigh_event_MIN_PTW_REQ     = 10, // Minion sends REQ to PTW
      pmu_neigh_event_MIN_PTW_RSP     = 11, // Minion receives RSP from PTW
      pmu_neigh_event_FLN_MSG         = 12, // Message sent between Minions through FLN
      pmu_neigh_event_ICACHE_ET_REQ   = 13, // ICache sends ET Link REQ
      pmu_neigh_event_ICACHE_ET_RSP   = 14, // ICache receives ET Link RSP
      pmu_neigh_event_ICACHE_L1_REQ   = 15, // ICache sends REQ to L1 SRAM
      pmu_neigh_event_ICACHE_L1_RSP   = 16, // ICache receives RSP from L1 SRAM
      pmu_neigh_event_PTW_ET_REQ      = 17, // PTW sends ET Link REQ
      pmu_neigh_event_PTW_ET_RSP      = 18, // PTW receives ET Link RSP

      pmu_neigh_event_ET_REQ_FIFO     = 21, // ET Link REQ pushed into the intermediate FIFO
      pmu_neigh_event_ET_REQ_UC_FIFO  = 22, // ET Link REQ pushed into any of the BANK/UC FIFOs
      pmu_neigh_event_ET_REQ_SC_UC    = 23  // ET Link RSP received from the SC/UC input
};


enum PmuMinEvent {
      pmu_event_NOCOUNT          = 0,  // No count
      pmu_event_MCYCLES          = 1,  // Mcycles 
      pmu_event_RETIRED_INST0    = 2,  // Retired Inst TH0
      pmu_event_RETIRED_INST1    = 3,  // Retired Inst TH1
      pmu_event_BRANCHES0        = 4,  // Branches taken TH0
      pmu_event_BRANCHES1        = 5,  // Branches taken TH1

      pmu_event_DCACHE_ACCESS0   = 6,  // Dcache Accesses TH0
      pmu_event_DCACHE_ACCESS1   = 7,  // Dcache Accesses TH1
      pmu_event_DCACHE_MISSES0   = 8,  // Dcache Misses TH0
      pmu_event_DCACHE_MISSES1   = 9,  // Dcache Misses TH1
      pmu_event_L2_MISS_REQ      = 10, // L2 Miss requests sent
      pmu_event_L2_MISS_REQ_REJ  = 11, // L2 Miss requests rejected
      pmu_event_L2_EVICT_REQ     = 12, // L2 Evict requests sent
      pmu_event_L2_EVICT_REQ_REJ = 13, // L2 Evict requests rejected
      pmu_event_TL_INST          = 14, // Tensor Load Instruction
      pmu_event_TL_OPS           = 15, // Tensor Load Operation
      pmu_event_TS_INST          = 16, // Tensor Store Instruction
      pmu_event_TS_OPS           = 17, // Tensor Store Operation

      pmu_event_TFMA_WAIT_TENB   = 18, // Tensor FMA waiting TENB
      pmu_event_TIMA_OPS         = 19, // TIMA Operation
      pmu_event_TXFMA_OPS16      = 20, // TXFMA Operation 16 bits
      pmu_event_TXFMA_OPS32      = 21, // TXFMA Operation 32 bits
      pmu_event_TXFMA_INT_OPS    = 22, // TXFMA Integer Operation
      pmu_event_TRANS_OPS        = 23, // TRANS Operation
      pmu_event_SHORT_OPS        = 24, // PI or uops from a TFMA32 Operation
      pmu_event_MASK_OPS         = 25, // Mask instructions
      pmu_event_TFMA_INST        = 26, // TFMA Instruction
      pmu_event_TREDUCE_INST     = 27, // Tensor Reduce instruction
      pmu_event_TQUANT_INST      = 28  // Tensor Quant instruction
};

// Interface with a PMU Counter
// Uses a single CSR read/write for configure, read, and write
template <int CounterId>
struct PmuCounter {
    static_assert(CounterId >= 3 && CounterId <= 8, "Invalid PMU Counter");

    // Calculate CSRs
    static constexpr int event_csr   = 0x320 + CounterId; // MHPMEVENT
    static constexpr int counter_csr = 0xB00 + CounterId; // MHPMCOUNTER

    // Configure the counter with an event
    // e.g.: PmuCounter<3>::configure(PmuMinEvent::MCYCLES);
    static ALWAYS_INLINE void configure(int event_id) {
        //const int event_id = static_cast<int>(event);
        __asm__ __volatile__("csrw %[csr], %[event]\n" ::[event] "r"(event_id), [csr] "i"(event_csr) :);
    }

    // Read the current value of the counter
    // e.g.: uint64_t cycles = PmuCounter<3>:read();
    static ALWAYS_INLINE uint64_t read() {
        uint64_t value;
        __asm__ __volatile__("csrr %[value], %[csr]\n" : [value] "=r"(value) : [csr] "i"(counter_csr) :);
        return value;
    }

    // Write a value to the counter
    // e.g.: PmuCounter<3>::write(0);
    static ALWAYS_INLINE void write(uint64_t value) {
        __asm__ __volatile__("csrw %[csr], %[value]\n" ::[csr] "i"(counter_csr), [value] "r"(value) :);
    }

    // Sets counter to zero
    static ALWAYS_INLINE void reset() {
        configure(pmu_event_NOCOUNT);
        write(0);
    }

    // Read the counter twice and return the higher value
    // Otherwise we may get the wrong value if we read the counter as it overflows
    // See Xavi's comment in RTLMIN-213
    static ALWAYS_INLINE uint64_t read_twice() {
        const uint64_t first = read();
        const uint64_t second = read();
        return first > second ? first : second;
    }
};

#ifndef NUM_MINIONS
#define NUM_MINIONS 1
#endif

#ifndef THREAD_MASK
#define THREAD_MASK 0x3
#endif
constexpr auto NUM_THREADS = THREAD_MASK == 0x1 or THREAD_MASK == 0x2 ? 1 : 2;

#define DEST_L2 1

uint32_t barrier_cnt __attribute__((section(".data"))) = 0; 

void pmu_init_fp_regs(uint64_t ptr) {
    init_fp_regs((void*) ptr);
    // Evict L1 lines
    for (unsigned way = 0; way < 4; ++way) {
       //       use_tmask      dst  way  set  num_lines
       evict_sw(    false, DEST_L2, way,   0,        15);
    }
}


void wait_amo(uint32_t * ptr, uint32_t target) {

    // Inc shared counter
    uint32_t last_value;
    
    asm volatile (
        "li t0, 1\n"
        "amoaddg.w %[val], t0, %[ptr]\n"
        : [val] "=r" (last_value)
        : [ptr] "r" (ptr)
        : "memory", "t0"
    );

    if (last_value == target-1)
    {
        // Reset the barrier and finish
        asm volatile (
            "amoaddg.w x0, x0, %[ptr]\n"
            : 
            : [ptr] "r" (ptr)
            : "memory"
            );
        FENCE;
    }
    else
    {
        last_value = 1; // Set non 0 value
        do
        {
            // Pull value - if 0, everything is set
            asm volatile (
                "amoorg.w %[val], x0, %[ptr]\n"
                : [val] "=r" (last_value)
                : [ptr] "r" (ptr)
                : "memory", "t0"
                ); 
        FENCE;            
        } while (last_value != 0);
    }
}

void wait_for_everyone()
{
    give_credit_local(-1, -1, 0); // thread 0
 
    // wait for all other harts to be done consuming the shared counter, plus the credit we just sent to ourselves
    for (uint32_t j = 0; j < NUM_MINIONS; ++j) {
       wait_for_credit(0);
       wait_for_credit(0);
    
    }
}

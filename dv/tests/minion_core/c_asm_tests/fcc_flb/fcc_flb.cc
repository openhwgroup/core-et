/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Check README
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "esr_defines.h"

////////////////////////////////////////////////////////////////
// Parameters
////////////////////////////////////////////////////////////////
#if defined(OVER_PARAMS)
   #ifndef MINION_MASK
       #warning "OVER_PARAMS set but no MINION_MASK provided. Assuming MINION_MASK=0x1"
       #define MINION_MASK 0x1
   #endif
   #ifndef THREAD_MASK
       #warning "OVER_PARAMS set but no THREAD_MASK provided. Assuming THREAD_MASK=0x1"
       #define THREAD_MASK 0x1
   #endif
   #ifndef SHIRE_MASK
       #warning "OVER_PARAMS set but no SHIRE_MASK provided. Assuming SHIRE_MASK=0x1"
       #define SHIRE_MASK  0x1
   #endif
#else
   // Setup default parameters to avoid compilation error
   #define MINION_MASK 0x0
   #define THREAD_MASK 0x0
   #define SHIRE_MASK  0x0
#endif

////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////

#ifndef VERBOSE
   #define VERBOSE 0
#endif

#define LOG(...) \
    do {                                        \
    if (VERBOSE) {                              \
        et_printf_long(__VA_ARGS__);             \
    }                                           \
    } while(0);

////////////////////////////////////////////////////////////////
// Global constants
////////////////////////////////////////////////////////////////
constexpr uint64_t NUM_ITER       = 64;
constexpr uint64_t FCC_COUNTER_ID = 0;
constexpr bool     EVICT_TO_L3    = false;

////////////////
// Types
////////////////
typedef struct {
    uint64_t TOTAL_ENABLED_HARTS;
    uint32_t THREAD0_ENABLED;
    uint32_t THREAD1_ENABLED;
    uint32_t NUM_THREAD0;
    uint32_t NUM_THREAD1;
} shire_config_data_t;

typedef union {
    shire_config_data_t data;
    minion_cache_line_t _min_size;
} shire_config_data_u;

////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////
shire_config_data_u shire_config_data [NUM_SHIRES] __attribute__((aligned(4096)));

const uint8_t rand_bytes[] =
  {
   0x05,0x38,0xeb,0xde,0xbc,0x02,0x56,0x82,0x41,0xf1,0xeb,0x49,0xe6,0x78,0xfb,0xe0,0xb5,0xaf,0xb7,0x07,0xde,0x90,0x16,0xf9,0x2f,0x28,0x08,
   0x27,0xc7,0xb3,0x80,0x7f,0xe2,0x17,0xfb,0x38,0xe3,0x8e,0x5f,0xf8,0xbb,0xbd,0xb8,0xe7,0x40,0xa8,0xfe,0x0d,0x27,0xaa,0x5a,0x27,0xe9,0x2b,
   0xb9,0xe7,0x5f,0xdd,0x8d,0xb9,0xd7,0xed,0x2d,0x79,0xce,0xd2,0xe0,0x32,0xf9,0xfb,0x48,0x61,0x39,0x4a,0x43,0xe0,0xdd,0x3f,0xb7,0xbc,0x20,
   0x32,0xa8,0x99,0x67,0xb7,0xf4,0x0f,0x01,0x79,0x7b,0x2c,0x96,0x73,0x0f,0xe5,0xd7,0x02,0xb2,0xe2,0x67,0x5a,0x5c,0xae,0x75,0x47,0xe2,0x61,
   0xab,0x24,0x88,0x9e,0x69,0x2a,0x64,0x9a,0x5f,0x98,0xcf,0x41,0x3e,0xcc,0x55,0xc2,0x2b,0x4a,0x87,0x88,0x53,0xbe,0x58,0x0b,0xcf,0x71,0xf9,
   0x04,0x16,0xf5,0x27,0xc4,0x56,0x95,0xbf,0x88,0x13,0xa7,0xa9,0xce,0xfd,0x0c,0xe5,0x6c,0x62,0x34,0xed,0x18,0x82,0x6a,0xf0,0x40,0x4a,0xe2,
   0x79,0x04,0x13,0x29,0x91,0xfe,0x8f,0x6a,0xd1,0x41,0xc7,0x72,0x15,0xea,0xf6,0x82,0x1a,0xcf,0xb2,0x71,0x17,0xb9,0x16,0x29,0xdd,0xda,0xb6,
   0x89,0x68,0x51,0x20,0x08,0x33,0xdf,0x73,0xc0,0x58,0x21,0x6d,0xbc,0xb7,0xdb,0x06,0x12,0x61,0x18,0x26,0x44,0xc0,0x5b,0xee,0xdb,0x4a,0x66,
   0xb6,0x60,0x9f,0x43,0xf0,0x91,0x17,0x1e,0x4a,0x67,0xd0,0xc0,0xcc,0xbb,0x9c,0x7b,0xb8,0xcf,0x5a,0x63,0x4b,0xef,0x07,0x6e,0xc1,0x3d,0xdd,
   0xd3,0x07,0x8f,0xf0,0x99,0xcb,0x17,0x1e,0x2b,0x32,0x5b,0x8c,0x74,0x1f,0x08,0x07,0x4e,0xb4,0x30,0xc5,0xf4,0x4e,0xd6,0x35,0x5c,0x30,0x13,
   0xa1,0x90,0x47,0x01,0x60,0x8b,0x5e,0x33,0xcd,0x44,0xc1,0x32,0x3f,0xe9,0x34,0x7f,0x89,0x73,0x47,0xe3,0x7c,0xf3,0xe5,0x05,0xc5,0xc7,0xcb,
   0x5e,0x57,0x60,0x92,0xdc,0x47,0x68,0xa2,0xe5,0x8e,0xff,0x59,0x06,0xa6,0x01,0x05,0xb9,0xcc,0xaa,0x93,0x8d,0x24,0x18,0x4e,0x75,0x17,0x3d,
   0xf4,0x92,0xb8,0x26,0xfb,0x0d,0xa2,0x52,0xbd,0x73,0xc9,0xa6,0xfa,0x54,0x0d,0x46,0x53,0x7c,0x36,0x9a,0x3d,0x3f,0x7e,0xfb,0xa1,0x90,0x34,
   0x7e,0x52,0x7a,0x75,0xa6,0x74,0x30,0x40,0x1b,0xd9,0x74,0x67,0xa0,0x6b,0x69,0x52,0x8d,0x0e,0x7a,0x36,0x4f,0xe7,0x0f,0x8d,0x49,0xe7,0x2f,
   0xb0,0x16,0xf5,0x90,0xd8,0xd2,0xfb,0xe4,0xde,0xd2,0x28,0x64,0x92,0x80,0x5a,0x98,0x69,0x49,0x56,0x7a,0x86,0x22,0xf5,0x04,0x96,0x4c,0xf3,
   0x65,0xda,0xd3,0x4a,0xa1,0x60,0x50,0x78,0x02,0xaa,0x98,0x0b,0xa1,0xb9,0x35,0x77,0x71,0xa6,0x55,0xcc,0x0d,0xf8,0xa9,0xee,0x22,0x57,0xf4,
   0x78,0x0f,0x5a,0xec,0xb5,0xf5,0x51,0x0b,0x1e,0x56,0x54,0x1c,0x62,0xe3,0x99,0x36,0x8a,0xae,0xe6,0xb4,0x13,0x60,0x11,0x0e,0x99,0xb5,0x9d,
   0xa6,0xa4,0x92,0xc9,0xb4,0x93,0x5c,0xd8,0x72,0xe5,0x96,0x03,0x3f,0x7e,0x4c,0x81,0xc0,0x45,0xa0,0x5d,0x4f,0xbb,0xdc,0x0f,0x59,0x16,0x96,
   0x52,0x9e,0xa5,0x2f,0x95,0xe8,0x12,0x87,0x72,0xdc,0xdc,0x0a,0xfb,0x98,0x58,0x44,0xa3,0x38,0x8f,0x77,0xff,0xa0,0xfc,0xb7,0x2b,0x65
};

////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////
inline bool flb(uint64_t flb_counter, uint64_t cmp_value)
{
  uint64_t ret = 0;
  uint64_t flb_value = flb_counter | ((cmp_value-1) << 5);

  __asm__ volatile ("csrrw %[RET], 0x820, %[CMP]\n"
		    : [RET] "=r" (ret)
		    : [CMP] "r" (flb_value)
		    :
		    );
  return bool(ret & 0x1);
}

void inc_fcc0_th0(uint32_t thread0_mask)
{
  volatile uint64_t* trigger0 = (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_0);
  *trigger0 = thread0_mask;
}

void inc_fcc0_th1(uint32_t thread1_mask)
{
  volatile uint64_t* trigger1 = (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_2);
  *trigger1 = thread1_mask;
}

uint8_t get_rand_byte(unsigned &ptr) {
  ptr%=sizeof(rand_bytes);
  return rand_bytes[ptr++];
}


/*
 * All the harts participate on a FLB and then waits for a credit
 * The last hart, unlocks all the harts
 */
void test0()
{
   uint64_t hid = get_hart_id();
   uint64_t sid = get_shire_id();
   uint64_t flb_id = 0;
   unsigned ptr = hid;
   uint64_t FLB_HARTS = shire_config_data[sid].data.TOTAL_ENABLED_HARTS;

   LOG("TEST0");
   for (uint64_t i = 0; i < NUM_ITER; ++i)
   {
     bool is_last = flb(flb_id, FLB_HARTS);
     if (is_last)
     {
       inc_fcc0_th0(shire_config_data[sid].data.THREAD0_ENABLED);
       inc_fcc0_th1(shire_config_data[sid].data.THREAD1_ENABLED);
     }
     wait_for_credit(FCC_COUNTER_ID);

     delay( get_rand_byte(ptr) );
   }

   // There should not be pending credits
   uint64_t pending_credits = get_credit_count_nb(FCC_COUNTER_ID);
   if (pending_credits)
     C_TEST_FAIL;

}

/*
 * Thread 0/1 use its own FLB so Thread 0/1 unlocks itself
 */
void test1()
{
   uint64_t hid = get_hart_id();
   uint64_t sid = get_shire_id();
   uint64_t tid = hid & 0x1;
   uint64_t flb_id = tid;
   unsigned ptr = hid;
   uint64_t FLB_HARTS = tid == 0 ? shire_config_data[sid].data.NUM_THREAD0 :
                                   shire_config_data[sid].data.NUM_THREAD1;

   LOG("TEST1");
   for (uint64_t i = 0; i < NUM_ITER; ++i)
   {
     bool is_last = flb(flb_id, FLB_HARTS);
     if (is_last)
     {
       const uint32_t thread_mask0 = shire_config_data[sid].data.THREAD0_ENABLED;
       const uint32_t thread_mask1 = shire_config_data[sid].data.THREAD1_ENABLED;

       if (tid == 0) { inc_fcc0_th0(thread_mask0); }
       else          { inc_fcc0_th1(thread_mask1); }
     }
     wait_for_credit(FCC_COUNTER_ID);
     delay( get_rand_byte(ptr) );
   }

   // There should not be pending credits
   uint64_t pending_credits = get_credit_count_nb(FCC_COUNTER_ID);
   if (pending_credits)
     C_TEST_FAIL;
}

/*
 * Compute thread mask and active harts for for mask provided at build-time
 */
void compute_over_mask(uint64_t & active_harts, uint32_t & thread0_enable_mask, uint32_t & thread1_enable_mask,
                       uint32_t & num_thread0,  uint32_t & num_thread1)
{
    #ifndef OVER_PARAMS
    // Safe check to avoid calling this function without setting parameters.
    LOG("ERROR: Calling 'compute_over_mask' without defining OVER_PARMS\n");
    C_TEST_FAIL;
    #endif

    uint64_t param_shire_mask  = SHIRE_MASK;
    uint64_t param_minion_mask = MINION_MASK;
    bool     thread0_enable    = (THREAD_MASK & 0x1) != 0;
    bool     thread1_enable    = (THREAD_MASK & 0x2) != 0;

    const uint64_t MASK_SIZE = NUM_HARTS_PER_SHIRE;

    // Arguments my not be 0. Setting to 0
    active_harts        = 0;
    thread0_enable_mask = 0;
    thread1_enable_mask = 0;
    num_thread0         = 0;
    num_thread1         = 0;

    // Skip the mask compute if the SHIRE_MASK[SID] = 0
    // Parameters were already cleared
    uint64_t sid = get_shire_id();
    bool enabled_shire = (param_shire_mask >> sid) & 0x1;
    if (not enabled_shire)
        return;


    // Compute masks
    for (uint64_t hart = 0; hart < MASK_SIZE; ++hart)
    {
        bool is_thread0 = hart%2 == 0;
        bool is_thread1 = !is_thread0;
        uint32_t thread_mask_index = hart/2;

        // Skip mask setup if hart is not enabled
        if ((!thread0_enable && is_thread0 ) ||
            (!thread1_enable && is_thread1))
            continue;

        // Check MINION_MASK and setup the masks to store
        uint64_t current_mask;
        bool     hart_enabled;
        current_mask = (param_minion_mask >> hart) & 0x1;
        current_mask = current_mask << thread_mask_index;
        hart_enabled = current_mask != 0;
        if (is_thread0) thread0_enable_mask |= current_mask;
        else            thread1_enable_mask |= current_mask;

        active_harts+=hart_enabled;
        num_thread0+=hart_enabled && is_thread0;
        num_thread1+=hart_enabled && is_thread1;
    }
}

/* Get active harts and store it on gloval storage per shire to reuse it
 * in the test
*/
int setup()
{
    volatile uint64_t* thread0_disable_esr = (volatile uint64_t*) ESR_SHIRE(0xFF, THREAD0_DISABLE);
    volatile uint64_t* thread1_disable_esr = (volatile uint64_t*) ESR_SHIRE(0xFF, THREAD1_DISABLE);

    // Pull masks
    const uint64_t thread0_disable_mask = *thread0_disable_esr;
    const uint64_t thread1_disable_mask = *thread1_disable_esr;
    const uint32_t thread0_enable_mask  = ~thread0_disable_mask;
    const uint32_t thread1_enable_mask  = ~thread1_disable_mask;
          uint32_t thread0_test_mask    = thread0_enable_mask;
          uint32_t thread1_test_mask    = thread1_enable_mask;

    // Data to calculate
    uint64_t lowest_index   = 63;
    uint64_t active_harts   = 0;
    uint32_t active_thread0 = 0;
    uint32_t active_thread1 = 0;

    for (uint64_t hart = 0; hart < NUM_HARTS_PER_SHIRE; ++hart)
    {
        // Intercate each mask on each iteration. It0: th0, It1: th1, It2: th0,...
        // Get the enabled harts and compute enabled hart with lowest ID and the
        // total number of harts enabled
        uint64_t current_mask = (hart%2 == 0) ? thread0_test_mask : thread1_test_mask;
        bool hart_enabled = ((current_mask >> (hart/2)) & 0x1);
        active_harts   += hart_enabled;
        active_thread0 += hart_enabled && (hart%2 == 0);
        active_thread1 += hart_enabled && (hart%2 == 1);
        lowest_index    = (hart_enabled && lowest_index > hart) ? hart : lowest_index;
    }



    // Only the lowest ID hart updates the info and evict the data
    const uint64_t shid = get_shire_hart_id();
    const uint64_t sid  = get_shire_id();
    const uint64_t tid = get_thread_id();
    const uint64_t mid = get_minion_id();

    if (shid == lowest_index)
    {
        LOG("FCC_FLB: S%u:M%u:T%u: SHIRE_TOTAL_ENABLED_HARTS: %llu\n", sid,mid,tid, active_harts );
        LOG("FCC_FLB: S%u:M%u:T%u: SHIRE_THREAD0_ENABLED: %x\n",       sid,mid,tid, thread0_test_mask );
        LOG("FCC_FLB: S%u:M%u:T%u: SHIRE_THREAD1_ENABLED: %x\n",       sid,mid,tid, thread1_test_mask );
        LOG("FCC_FLB: S%u:M%u:T%u: SHIRE_NUM_THREAD0: %llu\n",         sid,mid,tid, active_thread0 );
        LOG("FCC_FLB: S%u:M%u:T%u: SHIRE_NUM_THREAD1: %llu\n",         sid,mid,tid, active_thread1 );

        // Compute parametrized masks if necessary
#ifdef OVER_PARAMS
        compute_over_mask(active_harts, thread0_test_mask, thread1_test_mask,
                          active_thread0, active_thread1);
#endif

        LOG("FCC_FLB: S%u:M%u:T%u: TOTAL_ENABLED_HARTS: %llu\n", sid,mid,tid, active_harts );
        LOG("FCC_FLB: S%u:M%u:T%u: THREAD0_ENABLED: %x\n",       sid,mid,tid, thread0_test_mask );
        LOG("FCC_FLB: S%u:M%u:T%u: THREAD1_ENABLED: %x\n"  ,     sid,mid,tid, thread1_test_mask );
        LOG("FCC_FLB: S%u:M%u:T%u: NUM_THREAD0: %llu\n",         sid,mid,tid, active_thread0 );
        LOG("FCC_FLB: S%u:M%u:T%u: NUM_THREAD1: %llu\n",         sid,mid,tid, active_thread1 );

        // Validate configuretion
        // Overriten thread mask may not be possible with the current shire
        // THREAD_DISABLE ESR configuration
        uint32_t requested_disabled_thread0 = (thread0_test_mask ^ thread0_enable_mask) & thread0_test_mask;
        uint32_t requested_disabled_thread1 = (thread1_test_mask ^ thread1_enable_mask) & thread1_test_mask;
        bool     requested_mask_have_disabled_harts = requested_disabled_thread0 != 0 || requested_disabled_thread1 != 0;

        if (requested_mask_have_disabled_harts)
        {
            for (uint32_t hart = 0; hart < NUM_HARTS_PER_SHIRE; ++hart)
            {
                uint32_t thread_mask = (hart%2 == 0) ? requested_disabled_thread0 : requested_disabled_thread1;
                uint32_t hart_mask_index = hart/2;
                bool     hart_disabled = (thread_mask >> hart_mask_index) & 0x1;
                if (hart_disabled)
                {
                    uint64_t hart_disabled_id = sid*NUM_HARTS_PER_SHIRE + hart;
                    LOG("ERROR: Requested hart %llu but it's disabled\n", hart_disabled_id);
                }
            }
                C_TEST_FAIL;
        }

        // Store the configuration
        shire_config_data[sid].data.TOTAL_ENABLED_HARTS = active_harts;
        shire_config_data[sid].data.THREAD0_ENABLED = thread0_test_mask;
        shire_config_data[sid].data.THREAD1_ENABLED = thread1_test_mask;
        shire_config_data[sid].data.NUM_THREAD0 = active_thread0;
        shire_config_data[sid].data.NUM_THREAD1 = active_thread1;
        // Make test configuration visible to all tests
        FENCE;

        const enum cop_dest ev_dest = EVICT_TO_L3 ? to_L3 : to_L2;
        evict_va(0, ev_dest, (uint64_t)&shire_config_data[sid], 1);
        WAIT_CACHEOPS;

        // Get up other harts
        // Always get up all the harts regardless whichs are participating in the test
        // They are expected to reach the next part of the function and discard by them
        // self is they are not part of the test
        inc_fcc0_th0(thread0_enable_mask);
        inc_fcc0_th1(thread1_enable_mask);
        wait_for_credit(FCC_COUNTER_ID);

    } else {
        wait_for_credit(FCC_COUNTER_ID);
    }


    // Double check that we don't have pending credits before starting
    // the tests
    uint64_t pending_credits = get_credit_count_nb(FCC_COUNTER_ID);
    if (pending_credits)
        C_TEST_FAIL;

    // Harts that are not required for the test, should stop here

    shire_config_data_t tests_config = shire_config_data[sid].data;
    uint64_t hart_mask = tid == 0 ? tests_config.THREAD0_ENABLED : tests_config.THREAD1_ENABLED;
    uint64_t hart_mask_index = shid/2;
    bool hart_skip = ((hart_mask >> hart_mask_index) & 0x1) == 0; // If not present in mask, skip

    if (hart_skip)
    {
        LOG("FCC_FLB: Skipping hart %llu\n", get_hart_id() );
        LOG("FCC_FLB: hart_mask 0x%llx hart_mask_index %llu\n", hart_mask, hart_mask_index);
        return 0;
    }
    return 1;
}

int main()
{

   // Init marker
   C_TEST_START;

   // Configure tests parameters and
   if (setup()) {
       test0();
       test1();
   }

   C_TEST_PASS;
   return 0;
}

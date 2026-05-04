/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series stores and loads in a walking 1s pattern
//

#include "mbox_intr.h"
#include "print_dbg.h"
#include "cacheops.h"
#include "bringup.h"
#include "tensors.h"
#include "slt_barrier.h"

#ifdef FMAX_KERNEL_TEST
#include "../fmax_power/fmax_power_loads_and_stores/fmax_power_loads_and_stores.h"
#endif

#ifndef ETDV_SHIRE_MASK
  #define ETDV_SHIRE_MASK 0x1
#endif


#ifndef ETDV_SHIRE_HARTS_MASK
  #define ETDV_SHIRE_HARTS_MASK 0x1
#endif

inline void amo_write(uint64_t *addr, uint64_t write_data) {
   __asm__ __volatile__(
      "amoswapg.d x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
}

inline uint64_t amo_cmpswp(uint64_t *addr, uint64_t write_val, uint64_t cmp_val) {
   uint64_t old_data;
   __asm__ __volatile__(
      "add x31, x0, %[cmp_val]\n"
      "amocmpswapg.d %[old_data], %[write_val], (%[addr])\n"
      : [old_data] "=r" (old_data)
      : [addr] "r" (addr), [write_val] "r" (write_val), [cmp_val] "r" (cmp_val)
      : "x31"
   );
   return old_data;
}
test_result min_result[34*64]__attribute__((aligned(64))) = {0,0};
uint64_t lock[64/sizeof(uint64_t)] __attribute__((aligned(64))) = {0}; // full cache line
uint64_t eotlock[64/sizeof(uint64_t)] __attribute__((aligned(64))) = {0}; // full cache line
#define CMPSWP_LOCK amo_cmpswp(&lock[0], 1, 0)
#define FREE_LOCK amo_write(&lock[0], 0)
#define CMPSWP_LOCK_EOT amo_cmpswp(&eotlock[0], 1, 0)
#define FREE_LOCK_EOT amo_write(&eotlock[0], 0)

inline void SetLock(uint8_t print) {
    if (print) {
        while (CMPSWP_LOCK == 1); // spin until the lock seems free
    } else {
        while (CMPSWP_LOCK_EOT == 1); // spin until the lock seems free
    }
}

inline void UnsetLock(uint8_t print) {
    if (print) {
        FREE_LOCK;
    } else {
        FREE_LOCK_EOT;
    }
}

template<typename... Args>
int my_printf(char* fmt, Args... args) {
   unsigned int hid;
   __asm__ __volatile__ (
       "csrr %[hid], hartid\n"
     : [hid] "=r" (hid)
     :
     :
   );
   SetLock(0x1);
   print_dbg("[H%d] ", hid);
   print_dbg(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock(0x1);
   return 0;
}


void init_scratchpads(uint64_t hart_id) {
    if (hart_id == 0) {
        //my_printf("hart id %x\n", hart_id);
        // Broadcast and init the scp to zero
        // Can only broadcast to shires 0-31
        write_esr_new(PP_MACHINE, 32, REGION_OTHER, 0x0, 0x6, 0x00000901, 0xf);
        write_esr_new(PP_MACHINE, 33, REGION_OTHER, 0x0, 0x6, 0x00000901, 0xf);
        broadcast_req (PP_MACHINE, REGION_OTHER, 0x3c06, 0xffffffff, 0x00000901);
        // disable shire channel debug clock logic
        //write_esr_new(PP_MESSAGES, 32, REGION_OTHER, 0x2, 0x3ff4, 0x1, 0x0);
        //write_esr_new(PP_MESSAGES, 33, REGION_OTHER, 0x2, 0x3ff4, 0x1, 0x0);
        //broadcast_req (PP_MESSAGES, REGION_OTHER, 0xbff4, 0xffffffff, 0x1);
        int a = 0x100000;
        while(a-- != 0);
    }
}
void synchronize_dv_harts(uint64_t hart_id) {
    // All harts wait for the hart0 to enable them
    //uint64_t fcc_count;
    if (hart_id == 0) {
        // Broadcast to give credits to all the harts
        // Can only broadcast to shires 0-31
        write_esr_new(PP_USER, 32, REGION_OTHER, 0x2, 0x18, 0xffffffffffffffff, 0);
        write_esr_new(PP_USER, 32, REGION_OTHER, 0x2, 0x1a, 0xffffffffffffffff, 0);
        write_esr_new(PP_USER, 33, REGION_OTHER, 0x2, 0x18, 0xffffffffffffffff, 0);
        write_esr_new(PP_USER, 33, REGION_OTHER, 0x2, 0x1a, 0xffffffffffffffff, 0);
        broadcast_req (PP_USER, REGION_OTHER, 0x8018, 0xffffffff, 0xffffffffffffffff);
        broadcast_req (PP_USER, REGION_OTHER, 0x801a, 0xffffffff, 0xffffffffffffffff);
        // hart 0 also got a credit because of broadcast
    }
    asm volatile ("csrwi         0x821, 0");
/*
    __asm__ __volatile__ (
        "csrr  %0, fccnb\n"
        : "=r"(fcc_count)
    );
    while ((fcc_count & 0xffff) != 0)  {
        asm volatile ("csrwi         0x821, 0"); 
        __asm__ __volatile__ (
            "csrr  %0, fccnb\n"
            : "=r"(fcc_count)
            );
*/
}

uint64_t kernel_blast_mem (uint64_t hart_id, uint32_t loop_count) {
    uint64_t *my_addr;
    uint64_t read_data;
    const uint8_t index = 6;
    uint64_t my_data[index] = { 
        0xffffffffffffffff,
        0x0000000000000000,
        0xa5a5a5a5a5a5a5a5,
        0x5a5a5a5a5a5a5a5a,
        0xf0f0f0f0f0f0f0f0,
        0x0f0f0f0f0f0f0f0f,
    };

  #ifndef DDR_DENSITY
   #define DDR_DENSITY 2
  #endif
  #if DDR_DENSITY == 0
    // FIXME. should be 2GB and fix the base address for my_addr below
    const uint64_t window = 0x1e0000;
    #pragma message "Running blast_mem with 4GB DDR window"
  #elif DDR_DENSITY == 1
    const uint64_t window = 0x1e0000;
    #pragma message "Running blast_mem with 8GB DDR window"
  #elif DDR_DENSITY == 2
    const uint64_t window = 0x1e0000*3;
    #pragma message "Running blast_mem with 16GB DDR window"
  #elif DDR_DENSITY == 3
    const uint64_t window = 0x1e0000*7;
    #pragma message "Running blast_mem with 32GB DDR window"
  #endif

  #ifndef DISABLE_SCP_CHECK
    const uint64_t scp_window = 0xa000; // 2.5MB SCP / 64harts
    // target the next shire. shire 33 will target shire 0
    uint64_t target_shire_id  = ((((hart_id>>6) & 0x3F) + 1) % 34) & 0x7f;
  #endif
    for (uint32_t i = 0; i < loop_count; i++) {
        for (uint8_t jj = 0; jj < index;  jj++) {
            
            // DDR Writes
            my_addr = ( uint64_t * )(0x8100000000 + (hart_id * window));
            for (uint32_t ii = 0; ii < (window/8); ii++) {
		//my_printf ("addr %x", &my_addr[ii]);
                my_addr[ii] = my_data[jj];
            }
            
           #ifndef DISABLE_SCP_CHECK
            // Scratchpad writes
            my_addr = ( uint64_t * )(( 1UL << 31 ) + ( target_shire_id << 23 ) + ((hart_id % 64) * scp_window));
            for (uint32_t ii = 0; ii < (scp_window/8); ii++) {
                //my_printf ("addr %x", &my_addr[ii]);
                my_addr[ii] = my_data[jj];
            }
	   #endif

            // DDR Reads
            my_addr = ( uint64_t * )(0x8100000000 + (hart_id * window));
            for (uint32_t ii = 0; ii < (window/8); ii++) {
                read_data = my_addr[ii];
                //my_printf ("raddr %x d %x", &my_addr[ii], read_data);
                if (read_data != my_data[jj]) {
                    my_printf ("iter %d, pattern %d radd %x d %x hart_id %x expected %x", i, jj, &my_addr[ii], read_data, hart_id, my_data[jj]);
                    my_printf ("read %x", my_addr[ii]);
                    return 0x01;
                }
            } 

           #ifndef DISABLE_SCP_CHECK
            // Scratchpad reads
            my_addr = ( uint64_t * )(( 1UL << 31 ) + ( target_shire_id << 23 ) + ((hart_id % 64) * scp_window));
            for (uint32_t ii = 0; ii < (scp_window/8); ii++) {
                read_data = my_addr[ii];
                //my_printf ("raddr %x d %x", &my_addr[ii], read_data);
                if (read_data != my_data[jj]) {
                    my_printf ("scpadd %x d %x hart_id %x expected %x", &my_addr[ii], read_data, hart_id, my_data[jj]);
                    my_printf ("scpread %x", my_addr[ii]);
                    return 0x02;
                }
            }
           #endif
        }
    }
#ifdef ENABLE_REFRESH_CHECK
    delay(30000000000); // 30 second delay for 1000MHz
    // Read and compare again to test that refresh rate is good
    // DDR Reads
    my_addr = ( uint64_t * )(0x8100000000 + (hart_id * window));
    for (uint32_t ii = 0; ii < (window/8); ii++) {
        read_data = my_addr[ii];
        ////my_printf ("raddr %x d %x", &my_addr[ii], read_data);
        if (read_data != my_data[index-1]) {
            my_printf ("radd %x d %x hart_id %x expected %x", &my_addr[ii], read_data, hart_id, my_data[index-1]);
            my_printf ("read %x", my_addr[ii]);
            return 0x03;
        }
    } 
#endif
    return 0x0;
}

void min_done_notify_sp(uint64_t status_code, uint64_t hid, uint16_t loop_count) {
    uint64_t mmin_baseAddr = R_PU_TRG_MMIN_BASEADDR;
    uint64_t *mbox_addr = ( uint64_t * )( R_PU_MBOX_MM_SP_BASEADDR );
    PTR_Mbox_intr mbox_int_regs = ( PTR_Mbox_intr ) mmin_baseAddr;

    // Write pass/fail code of hart into L3
    FENCE;
    uint64_t eot_pattern;
    uint64_t eot_pattern_addr   = (uint64_t) &min_result[hid].eot_pattern;
    if (status_code == 0) {
        eot_pattern = 0x1feed000UL + (uint64_t)loop_count;
    } else {
        eot_pattern = 0x50bad000UL + (uint64_t)loop_count;
    }
    __asm__ __volatile__ (
        "amoswapg.d x0,  %[eot_pattern], (%[eot_pattern_addr])\n"
        :
        : [eot_pattern]        "r" (eot_pattern),
          [eot_pattern_addr]   "r" (eot_pattern_addr)
    );

    uint64_t kernel_status_addr = (uint64_t) &min_result[hid].kernel_status[0];
    __asm__ __volatile__ (
        "amoswapg.d x0, %[status_code], (%[kernel_status_addr])\n"
        :
        : [status_code]        "r" (status_code),
          [kernel_status_addr] "r" (kernel_status_addr)
    );
    FENCE;

    // Barrier to wait all the Minions and let the last Minion print to the Minion UART
    uint32_t active_harts_per_shire = get_number_active_harts_per_shire();
    uint64_t last = fast_local_barrier(3, active_harts_per_shire-1);
    if (last == 1) {
        uint64_t count = amo_incr() + 1;
        uint64_t players = get_number_active_shires();
        if (count == players) {
            amo_reset();
            // At this point all the harts must be done. Inform SP through mailbox interrupt
            my_printf ("Hart %d - Test Done, sending mbox interrupt", hid);
            mbox_addr[0] = (uint64_t) (&min_result[0]);
            __asm__ __volatile__ ("fence iorw, iorw");
            mbox_int_regs->ipi_trigger_set = 0xf;
        }
        else {
            amo_wait();
        }
        send_local_fcc();
    }
    wait_fcc();
}

void wait_for_sp() {
    // Get credit from SP
    asm volatile ("csrwi         0x821, 0");
}

#ifdef FMAX_KERNEL_TEST
#include "../power_vpu/kernel.h"
#include "../shire_cache/power/kernel.h"
#include "../noc/post_si/power/kernel.h"
#include "../power_vpu/tima/kernel.h"
#include "../power_vpu/bypass/kernel.h"
#include "../fmax/fmax_noc_amo/kernel.h"
#include "../fmax/fmax_noc_esr/kernel.h"
#include "../fmax/sc_noc_power_virus/kernel.h"
#endif

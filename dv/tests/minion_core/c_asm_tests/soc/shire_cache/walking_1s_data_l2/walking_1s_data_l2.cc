/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series stores and loads in a walking 1s pattern
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040

//#define SIZE 4096
//#define ALIGN_DATA 4096 
//#define ALIGN_INDEX 16
//
//static uint8_t buffer[SIZE]  __attribute__ ((aligned (ALIGN_DATA)));


// Floating point register usage
// f1 golden
// f2 read back
// f3 compare result (1 if equal)

inline __attribute__((always_inline)) void __do_write(uint64_t my_addr, uint64_t my_data) {
    __asm__ __volatile__
        (

         "sbl      %[my_data], (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

inline __attribute__((always_inline)) void __next_golden(uint64_t ps_mask, uint32_t golden_int) {
    __asm__ __volatile__
        (
         // clear golden
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f1, zero\n"
         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f1, %[golden_int]\n"

         :
         : [ps_mask] "r" (ps_mask), [golden_int] "r" (golden_int)
         :
        );
}

inline __attribute__((always_inline)) void __do_read(uint64_t my_addr) {
    __asm__ __volatile__
        (

         "mov.m.x m0, zero, 0xff\n"
         "flwl.ps f2, (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr)
         :  "memory"
        );
}

inline __attribute__((always_inline)) uint64_t __do_compare() {
    uint64_t ret;

    __asm__ __volatile__
        (

         "mov.m.x m0, zero, 0xff\n"
         "feq.pi f3, f1, f2\n"
         "fsetm.pi m0, f3\n"
         "maskpopc %[ret], m0\n"

         : [ret] "=r" (ret)
         :
         :
        );
    return ret; 
}


// Preload random data (~1M)
extern unsigned char _binary_blob_bin_start;

__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:          \n\t"
      "       .incbin     \"data.raw\"  \n\t"
      "       .align      4             \n\t"
      );


int main()
{
    volatile uint64_t *esr_poll_value;
    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );
 
    // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x3F);
   uint64_t base_addr;

   if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();

      // scp address
      //base_addr = 
      //   (1U << 31U) + // Scratchpad
      //   (shire_id << 23) + // Shire_id
      //   (minion_id << 19) + // minion
      //   (hart_id << 18) + // hart
      //   (0 << 11) + // index
      //   (0 << 9) + // sub_bank
      //   (0 << 6);  // bank

      // l2 address
      base_addr = 
         (1UL << 39) + // Main memory
         ((shire_id+1) << 32) + // Shire_id
         (0 << 31) + // Scratchpad
         (minion_id << 19) + // minion
         (hart_id << 18) + // hart
         (0 << 11) + // index
         (0 << 9) + // sub_bank
         (0 << 6);  // bank

      //base_addr = (uint64_t) buffer;
      
      et_printf_long("TEST: Base_addr = 0x%lx\n", base_addr);

      // Get the line into shire cache
      // Lock and zero the L1 line.  Line is now modified.
      lock_va(0, base_addr); 
      // Unlock and evict it to do a full line write to L2. 
      unlock_va(0, base_addr); 
      evict_va(0, to_L2, base_addr); 

      uint64_t ii;
      // For all bits in a cache line
      for (ii = 0; ii < 512; ++ii) {
         uint64_t byte_addr;
         uint64_t hline_addr;
         uint64_t data;
         uint64_t compare;

         byte_addr = base_addr + (ii/8);
         hline_addr = base_addr + ((ii/256) * 256/8);
         data = 1 << (ii%8);

         // write a byte
         // update the golden fr
         // read back to read fr
         // compare read fr to golden fr
         __do_write(byte_addr, data);
         __next_golden(1<<((ii/32)%8), 1<<(ii%32));
         __do_read(hline_addr);
         compare = __do_compare();
         if (compare != 8) {
            et_printf_long("TEST: Error: Failed compare for address 0x%lx\n", byte_addr);
            pass = false;
         }
         //} else {
            //et_printf_long("TEST: Compare = at address 0x%lx passed\n",compare, byte_addr);
         //}

         // repeat walking the zero (inverted zero).
         // write a byte
         // update the golden fr
         // read back to read fr
         // compare read fr to golden fr
         data = ~data; 
         __do_write(byte_addr, data);
         __next_golden(1<<((ii/32)%8),(data&0xff)<<((ii/8)%4)*8);
         __do_read(hline_addr);
         compare = __do_compare();
         if (compare != 8) {
            et_printf_long("TEST: Error: Failed compare for address 0x%lx\n", byte_addr);
            pass = false;
         }
         //} else {
            //et_printf_long("TEST: Compare = at address 0x%lx passed\n",compare, byte_addr);
         //}

         // write zero to clear it out
         __do_write(byte_addr, 0);
      }
   }


   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


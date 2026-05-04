/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "esr.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define ESR_FLB_REG_00         0x80340100ULL

inline void __attribute__((always_inline)) perform_flb_accesses(volatile uint32_t flb_counter_start, volatile uint32_t flb_counter_end, volatile uint32_t barrier_start, volatile uint32_t barrier_end) {
  for (uint32_t i = flb_counter_start; i < flb_counter_end; i++) {
    volatile uint64_t * FLB_REG_ADDR = (uint64_t *) (uint64_t(ESR_FLB_REG_00 + (i<<3)));
    for (uint32_t j = barrier_start; j < barrier_end; j++) {
      *FLB_REG_ADDR = 0;
      asm("fence\n");
      volatile uint64_t flb_data = (uint64_t(j << 5) | uint64_t(i));
      // perform as many flb writes as the counter max
      for (uint32_t z = 0; z < j; z++) {
	__asm__ __volatile__ (
			      // perform the barrior operation
			      "csrrw x4, 0x820, %[flb_data]\n"
			      "bnez x4, fail_end\n"
			      :
			      : [flb_data] "r" (flb_data)
			      : "x4"
			      );
      }
      __asm__ __volatile__ (
			    // perform the barrior operation
			    "csrrw x4, 0x820, %[flb_data]\n"
			    "beqz x4, fail_end\n"
			    :
			    : [flb_data] "r" (flb_data)
			    : "x4"
			    );
      
    }
  }
  return;
}
	
int main()
{
    uint64_t hid;

    volatile uint64_t *esr_poll_value;

    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );

    // Get hart ID
    hid = get_hart_id();

    C_TEST_START; 
    
    if (hid == 0) {
      //                   flb_counter start, flb_counter_end, barrier_start, barrier_end
      perform_flb_accesses(0, 8, 0, 2);
    }
    else if (hid == 16){
      //                   flb_counter start, flb_counter_end, barrier_start, barrier_end
      perform_flb_accesses(8, 16, 0, 2);
    }
    else if (hid == 32){
      //                   flb_counter start, flb_counter_end, barrier_start, barrier_end
      perform_flb_accesses(16, 24, 0, 2);
    }
    else if (hid == 48){
      //                   flb_counter start, flb_counter_end, barrier_start, barrier_end
      perform_flb_accesses(24, 32, 0, 2);
    } 

    C_TEST_PASS;
    asm("end_loop: j end_loop\n");

    asm("fail_end:\n");
    C_TEST_FAIL;

    return 0;
}
      



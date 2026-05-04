/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"

// simple test checking that invalid operations in tensor fma configuration produce an illegal instruction exception
#ifdef __cplusplus
extern "C"
#endif
void   __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler() {
  // just increment mpec 
  __asm__ __volatile__
    (
     "csrr t0, mepc\n"
     "addi t0, t0, 4\n"
     "csrw mepc, t0\n"
     : : : "t0" );
}


int main()
{
    // Init marker
   C_TEST_START;

     // set trap vector
  __asm__ __volatile__  ("csrw mtvec, %[vect]\n" : : [vect] "r" (trap_handler));


   uint16_t invalid_opcodes = 0xF4; // 11110100 => valid opcodes are 0,1,3
   // Only thread 0 executes this
   if (get_thread_id() == 0) {

     for ( int opcode = 0 ; opcode < 8; opcode++) {
       if (invalid_opcodes  & 1) {
         //         use_tmask  b_num_col  a_num_rows  a_num_cols  offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b  scp_loc_a  opcode  first pass
         //         ---------  ---------  ----------  ----------  ------  -------- -------------  -------------  -------- ---------  ---------  ------  ----------
         tensor_fma(false,     3,         7,          7,          0,         0,         0,              0,           0,       7,         0,      opcode,      true);
         WAIT_TENSOR_FMA;
       }
       invalid_opcodes >>=1;
     }
   }

   C_TEST_PASS;
   return 0;
}

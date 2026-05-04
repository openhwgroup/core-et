/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"
#include <inttypes.h>
#include "common.h"

#define N_LINES_RW  16ULL
#define N_TIMES 4ULL
#define N_BLOCKS 4ULL

int main()
{
    volatile char __attribute__((aligned(4096))) ptr[4096];
    long long int scp = 0x80003000;
    long long int dest = 0x8000000F;                                  

    volatile uint64_t hart_id   = getHartId();
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    

    volatile uint64_t shire_id_offset;
    volatile long     init_value;
    volatile uint64_t start_destination;
    volatile uint64_t conf_base;

   // volatile uint64_t shire_id_remote = 32 - shire_id;
   // volatile uint64_t shire_id_offset;

    long long int scp_addr = scp + (shire_id*N_TIMES*N_LINES_RW*N_BYTES_CL);

    // Init marker
    C_TEST_START;

    setup_cache_scp();

    setM0MaskFF();

    //if( ( (hart_id & 1) == 0) & (shire_id == 0) ) {
    if(  (hart_id & 1) == 0 ) {

      init_value = (shire_id << 24) + (1ULL << 16);
      initFRegs_2(init_value);

      shire_id_offset   = 3;
      start_destination = ((2U)<<30U)  + (shire_id << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore( conf , N_BYTES_CL);
        start_destination+=N_LINES_RW*N_BYTES_CL;
      } 

    // Test
    __asm__ __volatile__ (
        // Init and set all cachelines to modified
        "addi a1, %[ptr], 0\n"
        "addi a2, zero, 0\n"
        "addi a3, zero, 1024\n"
        "loop_sw:\n"
        "sw   a2, 0(a1)\n"          // Store
        "addi a2, a2, 1\n"          // Inc value
        "addi a1, a1, 4\n"          // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_sw\n"

        "csrwi 0x830, 6\n"

       // Prefetch VA
        "li   a1, 0\n"  
        "add  a1, a1, %[scp]\n"
       // "add  a2, zero, %[dest]\n"
        "addi a3, zero, 16\n"
        "loop_prefetch:\n"
        "csrw 0x81f, a1\n"
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_prefetch\n"

        "csrwi 0x830, 6\n"

        // Lock VA
        "li   a1, 0\n"        // Lock VA
        "add  a1, a1, %[scp_addr]\n"
        "addi a3, zero, 16\n"
        "loop_lock:\n"
        "csrw 0x8df, a1\n"          // Lock S/W
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_lock\n"

        "csrwi 0x830, 6\n"

        // Does some operations over scratchpad
        "add  a1, zero, %[scp_addr]\n"
        "add  a2, zero, 0xbe\n"
        "addi a3, zero, 16\n"
        "loop_oper_locked:\n"
        "lw   a2, 0(a1)\n"
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_oper_locked\n"

        // UnLock VA
        "li   a1, 0\n"        // UnLock VA
        "add  a1, a1, %[scp_addr]\n"
        "addi a3, zero, 16\n"
        "loop_unlock:\n"
        "csrw 0x8ff, a1\n"          // UnLock S/W
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_unlock\n"

        "csrwi 0x830, 6\n"

        // Does some operations over scratchpad
        "add  a1, zero, %[scp_addr]\n"
        "add  a2, zero, 0xbe\n"
        "addi a3, zero, 16\n"
        "loop_oper_unlocked:\n"
        "sw   a2, 0(a1)\n"
        "lw   a2, 0(a1)\n"
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_oper_unlocked\n"

        // Read all cachelines again
        "addi a1, %[ptr], 0\n"
        "addi a3, zero, 1024\n"
        "loop_lw:\n"
        "lw   a2, 0(a1)\n"          // Load
        "addi a1, a1, 4\n"          // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_lw\n"
      : 
      : [ptr] "r" (ptr), [scp] "r" (scp), [dest] "r" (dest), [scp_addr] "r" (scp_addr)
      : "a1", "a2", "a3", "a4"
    );
   
    }
    // End marker
    C_TEST_PASS;
    return 0;
}

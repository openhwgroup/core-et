/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include "macros.h"
#include "minion.h"

#define STUB_SHIRE 0xFFFFFFFFFFFFFFFEULL

int main()
{
    uint64_t hart_id   = get_hart_id();
    uint64_t thread_id = hart_id & 0x1;
    uint64_t minion_id = (hart_id >> 1) & 0x1F;
    uint64_t shire_id  = hart_id >> 6;

    C_TEST_START;

    if ((thread_id != 0) || (minion_id != 0) ) //all threads of shire 32 execut wfi, except th0
    {
       C_TEST_PASS;
    } else {

      if (minion_id==0) {  //exec by ANY Shire, Min 0, Thread 0

        // * 10 consecutive reads to ESR add 0->9
        for (int iter = 0; iter < 10; iter++) {
          __asm__ __volatile__ (
                  "csrr a5, mhartid\n"    //Get mhartid CSR
                  "srli a5, a5, 6\n"      //Get rid of neigh_id[1:0],min_id[2:0],hart_id. Hold shire_id[7:0]
                  "slli a5, a5, 22\n"     //Move shire_id to bit 22
                  "li a3, 0x80340100\n" //Load Addr to ESR space, reg addr=0x020,
                  "add a2, a5, a3\n"      //Add 'ESR space' addr + shire_id(bit 22 LSB)
                  "slli a5, %[iter], 3\n" //Move ESR id to bits 3    
                  "add a2, a2, a5\n"      //Add ESR id to address
                  "ld zero, 0(a2)\n"      //Load content of ESR reg addr
                  :
                  : [iter] "r" (iter)
                  : "a5", "a2", "a3"
          );
        }
      }

      __asm__ __volatile__ (
      "li a0, 0x00000901\n"
      "li a1, 0\n"
      "slli a1, a1, 22\n"

      "li a2, 0x80F1E030\n"
      "add a2, a2, a1\n"
      "sd a0, 0(a2)\n"
      "fence iorw, iorw\n" 
      );

      if (shire_id==0 && minion_id==0) {  //exec by Shire 0, Min 0, Thread 0

        // * 10 consecutive writes to SCRATCHPAD addr 0->9 (current SHIRE)
        for (int iter = 0; iter < 10; iter++) {
          __asm__ __volatile__ (
                  "csrr a5, mhartid\n"      //Get mhartid CSR
                  "srli a5, a5, 6\n"        //Get rid of neigh_id[1:0],min_id[2:0],hart_id. Hold shire_id[7:0]
                  "slli a5, a5, 23\n"       //Move shire_id to bit 23(scratchpad)
                  "li a3, 0x0080000000\n"   //Load Addr to SCRATCHPAD space, index 0,
                  "add a2, a5, a3\n"        //Add 'ESR space' addr + shire_id(bit 23 LSB)
                  "slli a5, %[iter], 3\n"   //Move ESR id to bits 3
                  "add a2, a2, a5\n"        //Add ESR id to address
                  "add a4, zero, %[iter]\n" //Prepare value to write
                  "sd a4, 0(a2)\n"          //Store value TO SCRATCHPAD index(iterated +1)
                  :
                  : [iter] "r" (iter)
                  : "a4", "a5", "a2", "a3"
          );
        }

        // * 10 consecutive reads to SCRATCHPAD addr 0->9 (SHIRE ID 8)
        for (int iter = 0; iter < 10; iter++) {
          __asm__ __volatile__ (
                  "li a5, 0x8\n"            //Load SHIREID=8
                  "slli a5, a5, 23\n"       //Move shire_id to bit 23(scratchpad)
                  "li a3, 0x0080000000\n"   //Load Addr to SCRATCHPAD space, index 0,
                  "add a2, a5, a3\n"        //Add 'ESR space' addr + shire_id(bit 23 LSB)
                  "slli a5, %[iter], 3\n"   //Move ESR id to bits 3
                  "add a2, a2, a5\n"        //Add ESR id to address
                  "ld zero, 0(a2)\n"        //Load value FROM SCRATCHPAD index(iterated +1)
                  :
                  : [iter] "r" (iter)
                  : "a5", "a2", "a3"
          );
        }

      } else if (shire_id==8 && minion_id==0) { //exec by Shire 8, Min 0, Thread 0

        // * 10 consecutive writes to SCRATCHPAD addr 0->9 (current SHIRE)
        for (int iter = 0; iter < 10; iter++) {
          __asm__ __volatile__ (
                  "csrr a5, mhartid\n"      //Get mhartid CSR
                  "srli a5, a5, 6\n"        //Get rid of neigh_id[1:0],min_id[2:0],hart_id. Hold shire_id[7:0]
                  "slli a5, a5, 23\n"       //Move shire_id to bit 23(scratchpad)
                  "li a3, 0x0080000000\n"   //Load Addr to SCRATCHPAD space, index 0,
                  "add a2, a5, a3\n"        //Add 'ESR space' addr + shire_id(bit 23 LSB)
                  "slli a5, %[iter], 3\n"   //Move ESR id to bits 3
                  "add a2, a2, a5\n"        //Add ESR id to address
                  "add a4, zero, %[iter]\n" //Prepare value to write
                  "sd a4, 0(a2)\n"          //Store value TO SCRATCHPAD index(iterated +1)
                  :
                  : [iter] "r" (iter)
                  : "a4", "a5", "a2", "a3"
          );
        }

        // * 10 consecutive reads to SCRATCHPAD addr 0->9 (SHIRE ID 0)
        for (int iter = 0; iter < 10; iter++) {
          __asm__ __volatile__ (
                  "li a5, 0x0\n"            //Load SHIREID=0
                  "slli a5, a5, 23\n"       //Move shire_id to bit 23(scratchpad)
                  "li a3, 0x0080000000\n"   //Load Addr to SCRATCHPAD space, index 0,
                  "add a2, a5, a3\n"        //Add 'ESR space' addr + shire_id(bit 23 LSB)
                  "slli a5, %[iter], 3\n"   //Move ESR it to bits 3
                  "add a2, a2, a5\n"        //Add ESR id to address
                  "ld zero, 0(a2)\n"        //Load value FROM SCRATCHPAD index(iterated +1)
                  :
                  : [iter] "r" (iter)
                  : "a5", "a2", "a3"
          );
        }


      }

      C_TEST_PASS;

    } // if-else, all threads of shire 32 execut wfi, except th0

    return 0;
}

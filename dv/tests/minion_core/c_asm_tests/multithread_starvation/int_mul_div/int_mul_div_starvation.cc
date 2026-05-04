/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#define BLOB_SIZE 1048576 // 1M
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
   ".section .data\n\t"
   "_binary_blob_bin_start:        \n\t"
   "      .align     12         \n\t"
   "      .incbin    \"data.raw\"  \n\t"
);

uint64_t* snp_get_fcc_addr(int threadid, uint32_t counter_num) {

    if (threadid == 0) {
        if (counter_num == 0) {
            return (uint64_t*) 0x803400C0;
        }
        else {
            return (uint64_t*) 0x803400C8;
        }
    }
    else {
        if (counter_num == 0) {
            return (uint64_t*) 0x803400D0;
        }
        else {
            return (uint64_t*) 0x803400D8;
        }
    }
}

void inc_fcc(uint32_t minion_id, uint32_t recv_thread, uint32_t fcc_count_id)
{
  uint32_t thread_mask = 1 << minion_id;
  volatile uint64_t* trigger = (volatile uint64_t*) snp_get_fcc_addr(recv_thread, fcc_count_id);;
  *trigger = thread_mask;
}

void consumer(int fcc_count_id) {
   // Prepare register to contain same values as RTLMIN-5842
   __asm__ (
      "la x26, _binary_blob_bin_start;"
      "li x14, 0xffff;"
      "sd x14, 1312(x26);"
      "fence;"
      "li  x7, 0x810815f048;"
      "li x14, 0;"
      "li x25, 0;"
   );
   
   if (fcc_count_id == 0) {
       __asm__ ("li x11, 0x0000ffff;");
   }
   else {
       __asm__ ("li x11, 0xffff0000;");
   }
   
   // Sequence causing the starvation
   __asm__ volatile (
      "1:"
      "lhu   x30, 1312(x26);"
      "divu  x13,    x7, x14;"
      "rem   x31,   x30, x25;"
      "divu  x13,    x7, x14;"
      "rem   x31,   x30, x25;"
      "csrrs  x5, fccnb, x0;"
      "and    x5,    x5, x11;"
      "beq    x0,    x5, 1b;"
   );
}

void producer(int minion_id, int consumer_thread, int fcc_count_id) {

   delay(1000);

   // Prepare register to contain same values as RTLMIN-5842
   __asm__ (
      "li x25, 0x1;"
      "li x14, 0x0;"
   );

   // Sequence causing the starvation
   __asm__ volatile (
      "mulh x30,x25,x14;"
   );
   inc_fcc(minion_id, consumer_thread, fcc_count_id);
}

int main() {

   // Init marker
   C_TEST_START;
   uint64_t tid;
   uint64_t mid;

   // Get minion and thread id
   tid = get_thread_id();
   mid = get_minion_id();

   if (tid == 1) {
      consumer(0);
   }
   else {
      //       minion_id, consumer_thread, fcc_count_id
      producer(      mid,               1,            0);
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

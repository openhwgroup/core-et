/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "tensors.h"

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
   // Prepare register to contain same values as RTLMIN-X
   __asm__ (
      "li x28, 0x40596580;"
      "li x13, 0;"
   );
   
   if (fcc_count_id == 0) {
       __asm__ ("li x12, 0x0000ffff;");
   }
   else {
       __asm__ ("li x12, 0xffff0000;");
   }

   __asm__ volatile ("sb  x13,-1649(x28);");
   __asm__ volatile ("sw  x13,-1785(x28);");

   __asm__ volatile ("sb  x13,-1521(x28);");
   __asm__ volatile ("sw  x13,-1657(x28);");

   __asm__ volatile ("sb  x13,-1393(x28);");
   __asm__ volatile ("sw  x13,-1529(x28);");

   __asm__ volatile ("sb  x13,-1265(x28);");
   __asm__ volatile ("sw  x13,-1401(x28);");

   // Sequence causing the starvation
   __asm__ volatile (
      "1:"
      "sb  x13,-1777(x28);"
      "sw  x13,-1913(x28);"
      "sb  x13,-1649(x28);"
      "sw  x13,-1785(x28);"
      "sw  x13,64(x28);"
      "sb  x13,128(x28);"
      "csrrs  x7,fccnb,x0;"
      "sb  x13,0(x28);"
      "sw  x13,0(x28);"
      "sb  x13,-1009(x28);"
      "sw  x13,-1657(x28);"
      "sw  x13,-1529(x28);"
      "and x7,x7,x12;"
      "sw  x13,192(x28);"
      "sb  x13,256(x28);"
      "sb  x13,-1265(x28);"
      "sw  x13,-1401(x28);"
      "sb  x13,-1137(x28);"
      "sw  x13,-1273(x28);"
      "beq x0,x7,1b;"
   );
}

      //"sb  x13,-1777(x28);"
      //"sb  x13,-1649(x28);"
      //"sb  x13,-1393(x28);"
      //"sb  x13,-1265(x28);"
      //"sb  x13,-1137(x28);"
      //"sb  x13,-1009(x28);"

      //"sw  x13,-1913(x28);"
      //"sw  x13,-1785(x28);"
      //"sw  x13,-1657(x28);"
      //"sw  x13,-1529(x28);"
      //"sw  x13,-1401(x28);"
      //"sw  x13,-1273(x28);"

void producer(int minion_id, int consumer_thread, int fcc_count_id) {

   delay(1000);

   // Prepare register to contain same values as RTLMIN-X
   __asm__ (
      "li x5, 0xa4a8000080002e64;"
   );

   // Sequence causing the starvation
   __asm__ volatile (
      "csrrw  x6, tensor_store,x5;"
      "csrrwi x0, tensor_wait, 0x08;"
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
      setup_cache_scp();
      //       minion_id, consumer_thread, fcc_count_id
      producer(      mid,               1,            0);
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

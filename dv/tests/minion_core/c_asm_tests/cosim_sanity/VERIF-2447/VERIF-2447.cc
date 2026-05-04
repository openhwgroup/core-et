/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"
#include "fcc.h"

#define TEST_FCC_COUNTER 0
#define TEST_FCC_COUNTER_FORCE 1
#define REDUCE_NUM_REGS 32
#define FLB0 0x820
#define ESR_FLB_REG_00 0x80340100ULL

void write_csr_flb(uint64_t csr_enc){
   __asm__ __volatile__ (
      "csrw 0x820, %[csr_enc]\n"
      :
      : [csr_enc] "r" (csr_enc)
      :
      );
}

void check_flb(){
   // [ 4:0] : 0 -> Barrier 0
   // [12:5] : 2 -> Number of Minions participating in the barrier -1
   uint64_t flb_val = 0x40;
   write_csr_flb(flb_val);
   FENCE;
}

uint64_t long_latency() {
   volatile uint64_t tmp = 0xdeadbeefdeadc0de;
   for (int i = 0; i < 100; i++) {
      uint64_t tmp1;
      if (i%2) {
          tmp1 = (tmp << 1) ^ tmp;
      }
      else {
          tmp1 = (tmp >> 1) ^ tmp;
      }
      tmp = tmp1/tmp;
      tmp = tmp1*tmp;
   }
   return tmp;
}

void do_h0(){

   long_latency();
   long_latency();

   //                  start_reg,         num_reg, partnerID
   tensor_reduce_send(         0, REDUCE_NUM_REGS,         1);

}

void do_h1(){
   wait_for_credit(TEST_FCC_COUNTER);
}

void do_h2(){
   //                  start_reg, Operation,         num_reg, partnerID
   tensor_reduce_recv(         0,      FGET, REDUCE_NUM_REGS,         0);
}

void do_h3(){
   long_latency();
   //                 minion_id, thread_id, credit_counter_num
   give_credit_local(         0,         1,   TEST_FCC_COUNTER);
}

int main()
{

   uint64_t mid, tid, hid;
   C_TEST_START;


   // Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();
   hid = (mid << 1) + tid;

   if (hid == 0) do_h0();
   else if (hid == 1) do_h1();
   else if (hid == 2) do_h2();
   else if (hid == 3) do_h3();

   WAIT_TENSOR_REDUCE;

   C_TEST_PASS;

   return 0;
}

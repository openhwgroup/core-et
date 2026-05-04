/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"

/*
 * This test stall the threads for debuging propouses
 * Thread 0 starts a TL COOP (since we only have one minion this should never end) and then it does a TensorWait
 * Thread 1 does a PGET (blocking) on PORT0
 */

volatile uint64_t global_array[4]; // global array just to avoid unused variable
volatile uint64_t TL_load_region[64];

__attribute__((section(".pass_handler"))) void return_handler() {
  C_TEST_PASS;
}


int main()
{
   C_TEST_START;

   int tid = get_thread_id();
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();
   
   if (mid > 0) {
     C_TEST_PASS;
   }

   if (tid == 0) {
     enable_shire_coop(sid);     
     setup_cache_scp();
     FENCE
     uint64_t neigh_mask = 0xff;
     uint64_t minion_mask = 0xff;
     uint64_t coop_id = 0;
     tensor_coop(neigh_mask, minion_mask, coop_id);
     et_tensor_load_conf_t tl_conf;
     tl_conf.use_tmask = 0;
     tl_conf.use_coop = 1;
     tl_conf.use_tenb = 0;
     tl_conf.dst_start = 0;
     tl_conf.transformation = 0;
     tl_conf.rd_l2scp = 0;
     tl_conf.addr = reinterpret_cast<uint64_t>(&TL_load_region[0]);
     tl_conf.offset = 0;
     tl_conf.num_lines = 4;
     tl_conf.stride = 0;
     tl_conf.id = 0;
     et_tensor_load(&tl_conf);
     __asm__ volatile ("csrwi gsc_progress, 1"); // Hart reached expected point
     WAIT_TENSOR_LOAD_0
       
   } else {
     char buff[64];
     constexpr unsigned port = 0;
     setup_rx_port(port, true, 2, 1, buff, 0, 0);
     __asm__ volatile ("csrwi gsc_progress, 1"); // Hart reached expected point
     uint32_t offset = read_port_status(port);
     global_array[offset] = 0xDEADBEAF; // avoid unused variable
     
   }

   C_TEST_FAIL;
   return 0;
   
}

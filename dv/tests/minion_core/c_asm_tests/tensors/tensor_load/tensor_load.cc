/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"
#include "tensor_load.h"

#ifndef LOOP_SIZE
    #define LOOP_SIZE 7
#endif

#ifdef SELF_CHECK
   #include "tl_selfchecking.cpp"
#endif

#define BLOB_SIZE 1048576 // 1M
#define CL_BLOB_SIZE (BLOB_SIZE/sizeof(cl_data_t))
// Tensor Load accesses multiple addresses. To avoid acessing beyond the BLOB
// reduce the size of the BLOB to the half
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

int main() {

   // Init marker
   C_TEST_START;
   uint64_t tid;
  #ifndef POSTSI_PARAM
   uint64_t mid;
  #endif
   et_tensor_load_conf_t  load_conf_matrix;
   #ifndef SELF_CHECK
   et_tensor_load_conf_t  load_conf_matrix_b;
   #endif

   #ifdef SELF_CHECK
   uint64_t selfcheck_tmask = 0;
   uint8_t pre_data[L1SCP_SIZE_BYTES] __attribute__((aligned(64)));
   uint8_t post_data[L1SCP_SIZE_BYTES]  __attribute__((aligned(64)));
   #endif

   // Get minion ID and thread id
   tid = get_thread_id();
  #ifndef POSTSI_PARAM
   mid = get_minion_id();
  #endif

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   // Only thread 0 executes this
   if (tid == 0) {
      volatile cl_data_t *buffer = &_binary_blob_bin_start;
      setup_cache_scp();

      for (int i = 0; i < LOOP_SIZE; i++) {
        #ifdef POSTSI_PARAM
         // assume in POSTSI_PARAM the test is always run with ONE minion (to keep within 64KB raw_data size); 
         uint64_t ptr = (uint64_t)(void*)buffer[(i)%TL_BLOB_SIZE].data[0];
        #else
         uint64_t ptr = (uint64_t)(void*)buffer[(mid*mid*i)%TL_BLOB_SIZE].data[0];
        #endif
         do_tensor_load_common(&load_conf_matrix, (void*) ptr, false);
         uint64_t load_op = i%7;

         if (i > 6) {
             load_conf_matrix.use_tmask = et_get_rand_word(0,1) ? true : false;
             if (load_conf_matrix.use_tmask) {
                 uint64_t tmask = et_get_rand_word();
                 tensor_mask(0x0, tmask);
                 #ifdef SELF_CHECK
                 selfcheck_tmask = tmask;
                 #endif
             }
         }

         #ifdef SELF_CHECK
         dump_whole_l1scp((uint64_t*) pre_data);
         #endif

         switch (load_op) {
            case 0:
               do_tensor_load(&load_conf_matrix);
               break;
            case 1:
               load_conf_matrix.use_tmask = 0;
               do_tensor_load(&load_conf_matrix);
               WAIT_TENSOR_LOAD_0;

               #ifndef SELF_CHECK
               do_tensor_load_common(&load_conf_matrix_b, (void*) ptr, false);
               load_conf_matrix_b.num_lines = (load_conf_matrix_b.num_lines & 0xC) - 1;
               setup_tensor_load_B(&load_conf_matrix_b);

               do_tensorFMA(&load_conf_matrix, &load_conf_matrix_b);
               WAIT_TENSOR_FMA;

               tensor_error_check();
               #endif
               break;
            case 2:
               do_tensor_load_interleave8(&load_conf_matrix);
               break;
            case 3:
               do_tensor_load_interleave16(&load_conf_matrix);
               break;
            case 4:
               do_tensor_load_transpose8(&load_conf_matrix);
               break;
            case 5:
               do_tensor_load_transpose16(&load_conf_matrix);
               break;
            case 6:
               do_tensor_load_transpose32(&load_conf_matrix);
               break;
            default:
               do_tensor_load(&load_conf_matrix);
         }
         FENCE;
         WAIT_TENSOR_LOAD_0;
         tensor_error_check();
         #ifdef SELF_CHECK
         dump_whole_l1scp((uint64_t*) post_data);
         uint64_t ret = tl_selfchecking(&load_conf_matrix, selfcheck_tmask, (uint64_t*) pre_data, (uint64_t*) post_data);
         if (ret != PASS) C_TEST_FAIL;
         #endif
      }
      do_tensor_load_illegal_config_0(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
      do_tensor_load_illegal_config_1(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

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
#define NEIGH0_IPI_REDIRECT_PC 0x80100040
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

volatile uint64_t ptr __attribute__((section(".address_array")));
 
int main() {

   // Init marker
   C_TEST_START;


   volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );

   uint64_t mid, tid;
   et_tensor_load_conf_t  load_conf_matrix, load_conf_matrix_b;

   // Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   // Only thread 0 executes this
   if (tid == 0) {
      volatile cl_data_t *buffer = &_binary_blob_bin_start;
      
      setup_cache_scp();

      for (int i = 0; i < LOOP_SIZE; i++) {
         ptr = (uint64_t)(void*)buffer[(mid*mid*i)%TL_BLOB_SIZE].data[0];
         do_tensor_load_common(&load_conf_matrix, (void*) ptr, false);
         uint64_t load_op = i%7;

         if (i > 6) {
             load_conf_matrix.use_tmask = et_get_rand_word(0,1) ? true : false;
             if (load_conf_matrix.use_tmask) {
                 uint64_t tmask = et_get_rand_word();
                 tensor_mask(0x0, tmask);
             }
         }

         switch (load_op) {
            case 0:
               do_tensor_load(&load_conf_matrix);
               break;
            case 1:
               load_conf_matrix.use_tmask = 0;
               do_tensor_load(&load_conf_matrix);
               WAIT_TENSOR_LOAD_0;

               do_tensor_load_common(&load_conf_matrix_b, (void*) ptr, false);
               load_conf_matrix_b.num_lines = (load_conf_matrix_b.num_lines & 0xC) - 1;
               setup_tensor_load_B(&load_conf_matrix_b);

               do_tensorFMA(&load_conf_matrix, &load_conf_matrix_b);
               WAIT_TENSOR_FMA;

               tensor_error_check();
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

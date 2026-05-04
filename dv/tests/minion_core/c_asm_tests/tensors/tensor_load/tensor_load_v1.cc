/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"
#include "tensor_load.h"
#include "cacheops.h"


#define SIZE 32
#define ALIGN_DATA  32
#define ALIGN_INDEX 16


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


static uint32_t x31_operand[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0x040,
   0x080,
   0x0c0,
   0x100,
   0x140,
   0x180,
   0x1c0,
   0x200,
   0x240,
   0x280,
   0x2c0,
   0x300,
   0x340,
   0x380,
   0x3c0,
   0x400,
   0x440,
   0x480,
   0x4c0,
   0x500,
   0x540,
   0x580,
   0x5c0,
   0x600,
   0x640,
   0x680,
   0x6c0,
   0x700,
   0x740,
   0x780,
   0x7c0,
   0x800
};




int main() {

   // Init marker
   C_TEST_START;
   uint64_t mid, tid;
   et_tensor_load_conf_t  load_conf_matrix;

//   #ifndef SELF_CHECK
//   et_tensor_load_conf_t  load_conf_matrix_b;
//   #endif

   #ifdef SELF_CHECK
   uint64_t selfcheck_tmask = 0;
   uint8_t pre_data[L1SCP_SIZE_BYTES] __attribute__((aligned(64)));
   uint8_t post_data[L1SCP_SIZE_BYTES]  __attribute__((aligned(64)));
   #endif

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
         uint64_t ptr = (uint64_t)(void*)buffer[(mid*mid*i)%TL_BLOB_SIZE].data[0];
         do_tensor_load_common(&load_conf_matrix, (void*) ptr, false);
         uint64_t load_op = i%7;

         uint64_t csr_enc; 
         uint32_t *my_x31_operand = (x31_operand + i);   // increment with i

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
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
               //do_tensor_load(&load_conf_matrix);
               // try to test (implicit operand) "x31" dependency:
               load_conf_matrix.use_tenb = 0;  // avoid TenB in this test (irrelavent); 
               csr_enc  = (((uint64_t)load_conf_matrix.use_tmask & 1) << 63) |
                          (((uint64_t)load_conf_matrix.use_coop & 1)  << 62) |
                          ((          load_conf_matrix.transformation & 0x7) << 59)    |
                          ((          load_conf_matrix.dst_start & 0x3F) << 53)        |
                          (((uint64_t)load_conf_matrix.use_tenb & 0x1) << 52)          |
                          ((          load_conf_matrix.addr & 0xFFFFFFFFFFC0ULL))      |
                          ((          load_conf_matrix.offset & 0x3) << 4)             |
                          ((          load_conf_matrix.num_lines & 0xF));

               // special: x31 will be implicit operand used by Tensor_Load: 
               __asm__ __volatile__ (
                    //"amoorl.w x31, x0, (%[my_x31_operand])\n" // Read mem value with AMOOR (directly from L2. bypassing L1)  
                    "lw x31, 0(%[my_x31_operand])\n"    // note: better to make sure the [my_x31_operand] data is NOT in L1  
                    :
                    : [my_x31_operand] "r" (my_x31_operand)
                    : "x31", "memory"
               );
               __asm__ __volatile__ (
                    "csrw 0x83f, %[csr_enc]\n"
                    :
                    : [csr_enc] "r" (csr_enc)
                    : 
               );
               break;

          /************
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
          ************/ 
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

         // better to make sure the [my_x31_operand] data is NOT in L1: evict it (to L2 or L3):
         FENCE;
         evict_va(false, 0x2, (uint64_t)(void*)my_x31_operand, 0, 0, 0, 0); 
         WAIT_CACHEOPS;
         FENCE;

      } // for (int i = 0; i < LOOP_SIZE; i++) 

     /*****
      do_tensor_load_illegal_config_0(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
      do_tensor_load_illegal_config_1(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
     *****/
   } // if (tid == 0) {

   // End marker
   C_TEST_PASS;
   return 0;
}

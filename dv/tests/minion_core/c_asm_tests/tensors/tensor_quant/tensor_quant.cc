/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      8             \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );

int main()
{

   uint64_t mid, tid;

   volatile cl_data_t *buffer = &_binary_blob_bin_start;

   //1.0 Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Init marker
   C_TEST_START;

   if (tid == 0)
   {

      // Enable scratchpad
      scp(0,1);

      FENCE;
      tensor_mask (0x0, 0x0);	

      //          use_tmask  use_coop  transformation  dst_start  scp_sel   addr                   offset  num_lines  stride   id
      //          ---------  --------  --------------  ---------  --------  --------------------   ------  ---------  ------   --
      tensor_load(false,     false,    0,              0,         0,        (uint64_t)(void*)buffer[mid].data[0],  0,      7,        64,      0 );

      WAIT_TENSOR_LOAD_0;

      //                  use_coop  addr                                   num_lines  stride   id
      //                  --------  -------------------------------------  ---------  ------   --
      //tensor_load_setup_b(false,    (uint64_t)(void*)buffer[mid].data[0],  7,         64,      0);

      //          start_reg  col row scp_loc  transf9  transf8 transf7 transf6 transf5 transf4 transf3 transf2 transf1 transf0
      //         ----------  --- --- -------  -------  ------- ------- ------- ------- ------- ------- ------- ------- -------
      tensor_quant(	  0,   0, 0,      0,       0,        0,     0,      0,      0,      0,      0,      0,      0,       0);


      //          use_tmask  use_coop  transformation  dst_start  scp_sel   addr                   offset  num_lines  stride   id
      //          ---------  --------  --------------  ---------  --------  --------------------   ------  ---------  ------   --
      //tensor_load(false,     false,    0,              0,         0,        (uint64_t)(void*)buffer[mid].data[0]+256,  0,      7,        64,      0 );

      WAIT_TENSOR_FMA;
      //WAIT_TENSOR_LOAD_1;

      C_TEST_PASS;
    } 
    else 
    {
      C_TEST_PASS;
    }

   return 0;
}

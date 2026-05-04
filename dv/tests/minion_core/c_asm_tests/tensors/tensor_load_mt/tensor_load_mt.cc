/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Directed self-checking test that exercices the basic functionality
//               of the tensor load
//
//    Each hart issues a series for stores to warm-up the L1 cache, then evicts everything
//    from the L1 and reconfigures the L1 as scratchpad.
//
//    Then it tries to fill up the L1 scratchpad with data using tensor loads and reads back the evicted
//    data before the reconfiguration of the L1 cache. Tensor load address is always unique amongst all
//    harts (based on the minion/thread id)
//
// Note:
//       - Machine trap handler is configured in this test in such way that the test would branch to DEAD, should
//        an exception occur.
//       - Tensor data are being preloaded at @_binary_blob_bin_start
// FUTURE:
//       Randomize
//          - L1 or TENB destination
//          - Ensure @_binary_blob_bin_start is aligned at 32b
//          - Randomize number of lines being in the tensor_load

// (CYE / Charles) changes for #ifdef POSTSI_UST:
//  0) note: L1D$ size is 16 * 4 * 64(byte) = 4KB;
//  1) reduce the data size / memory allocation to fit within 64KB "data.raw";
//  2) assume the test runs in one minion (default "neigh" build);
//


#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"
#include "et_postsi_common.h"

// Preload random data (~1M)
extern unsigned char _binary_blob_bin_start[];

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .incbin     \"data.raw\"  \n\t"
        "       .align      4             \n\t"
    );



int main()
{

  #ifndef POSTSI_UST
   uint64_t mid;
  #endif
   uint64_t tid;
   uint8_t  scp_sel = 0;

   // Read/Write buffers 256K apart
  #ifdef POSTSI_UST
    volatile uint64_t data;      // CYE: better to make it 64-bit (same as read_buffer/write_buffer) 
  #else
    volatile uint32_t data;
  #endif

    volatile uint64_t * read_buffer  = (uint64_t *) &_binary_blob_bin_start;

  #ifdef POSTSI_UST   // CYE: assume test runs in only 1 minion, reduce to 4KB; or choose 32KB if runs in 8 minions; 
    volatile uint64_t * write_buffer = (uint64_t *) &_binary_blob_bin_start + 0x200;   //  0x200 * 0x8(byte) = 0x1000 (bytes) = 4KB
  #else
    volatile uint64_t * write_buffer = (uint64_t *) &_binary_blob_bin_start + 0x4000;  // 0x4000 * 0x8(byte) = 0x20000 (bytes) = 128KB
  #endif

    // volatile uint64_t scp_en = 0;

   //1.0 Get minion ID and thread id
  #ifndef POSTSI_UST
   mid = get_minion_id();
  #endif
   tid = get_thread_id();

   // Randomize scp_sel destination of the load (L1 or TenB scratchpad)
   //scp_sel = (uint8_t) et_get_rand_word(0, 1);

   // Setup machine trap vector
   setup_mtrap_vector();

   // Init marker
   C_TEST_START;

   // Allocate space for each thread/minion: 
   #ifdef POSTSI_UST 
     // CYE: actually each minion only needs 4KB (assuming only thread 0) because L1D$ size is 64*64=4KB; 
     //read_buffer  = read_buffer  + 0x200*mid;      // 0x200 * 0x8(byte) = 4KB (to each minion, thread 0)
     //write_buffer = write_buffer + 0x200*mid;      // 0x200 * 0x8(byte) = 4KB (to each minion, thread 0)
     // CYE: assume that in POSTSI_UST this test always runs one minion active; so to simplify: 
     read_buffer  = read_buffer; 
     write_buffer = write_buffer; 
   #else 
     read_buffer  = read_buffer  + 0x400*mid + tid*0x400;      // 0x400 * 0x8(byte) = 8KB (to each thread)
     write_buffer = write_buffer + 0x400*mid + tid*0x400;      // 0x400 * 0x8(byte) = 8KB (to each thread)
   #endif

   //1.0 T0 initializes cache lines and then Evicts them to L2
   if (tid == 0)
   {
      //1.1 Warm up L1 cache
      //Write some data to the L1 before turning on the scratchpad
    #ifdef POSTSI_UST
      for (int i=0; i<512; i++)   // CYE: actually only need i<512 (bcoz 512*8(byte) = 4KB = L1D$ size)
    #else
      for (int i=0; i<1024; i++) 
    #endif
      {
         *(write_buffer+i) = (i+1) + ((tid+1)+i);
      }

      //1.2 Evict L1 lines to L2 (dst=0x01) (16L/set x 4ways)
      for (unsigned way = 0; way < 4; ++way) {
         evict_sw(false, 0x1, way, 0, 15);
      }

      //2.0 Enable scratchpad control register (locks 48/64 cache lines as scratchpad)
	   scp(0,1);
      // FENCE is needed here due to RTLMIN-1641, as when we configure the cache as scratchpad the tensor_load is not blocked in the frontend
      // hence, it starts before the scratchpad is enabled
      //FENCE;

      //2.1 Issue tensor load (16 lines) (fill 1/4 scratchpad)
      //FUTURE fill the scratchpad with multiple loads with a different starting offset
      //          use_tmask  use_coop  transformation  dst_start  scp_sel   addr                         offset  num_lines  stride       id
      //          ---------  --------  --------------  ---------  --------  --------------------         ------  ---------  ------       --
      tensor_load(false,     false,    0,              0,         scp_sel,  (uint64_t)(void*)read_buffer,           0,      7,        64,      0 );
      //tensor_load(false,     false,    0,              8,         scp_sel,  (uint64_t)(void*)read_buffer+8*64,      0,      7,        64,      1 );
      //tensor_load(false,     false,    0,              16,        scp_sel,  (uint64_t)(void*)read_buffer+16*64,     0,      7,        64,      1 );
      //tensor_load(false,     false,    0,              23,        scp_sel,  (uint64_t)(void*)read_buffer+23*64,     0,      7,        64,      0 );


      //tensor_load(false,     false,    0,            0,         !scp_sel,  (uint64_t)(void*)read_buffer+num_lines*64,     0,      7,        64,      0 );

      //2.2 Sanity check
    #ifdef POSTSI_UST
      for (int i=0; i<512; i++)   // CYE: assume "data" size is 64-bit ...
    #else
      for (int i=0; i<1024; i++)
    #endif
      {
         data = *(write_buffer+i);
         if (data != (i+1)+ ((tid+1)+i))
            C_TEST_FAIL;
      }

      //2.2 Wait for the tensor load to finish (the actual TB doesn't wait for the actual L2 requests
      WAIT_TENSOR_LOAD_0;
      WAIT_TENSOR_LOAD_1;


      c_test_pass();
    }
    else
    {
        c_test_pass();
    }

   return 0;
}

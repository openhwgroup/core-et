/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "fcc.h"
#include "et_test_common.h"
#include "cacheops.h"


#ifndef LOOP_SIZE
    #define LOOP_SIZE 4
#endif

// Preload random data (~1M)
#define BLOB_SIZE 1048576 // 1M
#define CL_BLOB_SIZE (BLOB_SIZE/sizeof(cl_data_t))
// Tensor Load accesses multiple addresses. To avoid acessing beyond the BLOB
// reduce the size of the BLOB to the half
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

uint64_t* SCPline_to_addr(uint64_t scp_line, bool is_local_scp, uint64_t sid, uint64_t scp_format_mode) {
   uint64_t scp_shire;
   uint64_t* addr;

   scp_shire = (is_local_scp == true) ? 0xFFFL : sid;

   // PRM-19 - 2.3 The Scratchpad Region (2G to 4G)
   // Bit 30 defines the SCP Format Mode
   scp_format_mode = scp_format_mode & 0x1UL;
   scp_shire = scp_shire & (0x1UL << 30);
   scp_shire = scp_shire | (scp_format_mode << 30);

   addr = (uint64_t*)(0b000000001UL          << 31 | // Fixed for SCP region
                     (scp_shire & 0x0FFUL)   << 23 | //  8 bits
                     (scp_line  & 0x0FFFFUL) <<  6); // 16 bits

   return addr;
}

void self_check(uint64_t* pre, uint64_t* post, uint64_t nelem) {
   for (uint64_t i = 0; i < nelem; i++) {
      if (pre[i] != post[i]) {
         C_TEST_FAIL;
      }
   }
}

inline uint64_t __attribute__((always_inline)) et_get_l2_scp_size (uint64_t sid)
{
   volatile uint64_t pp = 0b11UL;
   volatile uint64_t addr = 0x005;
   volatile uint64_t banks = 0b1111;

   volatile uint64_t* esr_addr = (uint64_t *)(
                                 (((uint64_t)(               0b00000001UL)) << 32) |
                                 (((uint64_t)(                pp & 0b11UL)) << 30) |
                                 (((uint64_t)(        sid & 0b011111111UL)) << 22) |
                                 (((uint64_t)(                    0b011UL)) << 20) |
                                 (((uint64_t)(                    0b010UL)) << 17) |
                                 (((uint64_t)(             banks & 0b1111)) << 13) |
                                 (((uint64_t)( addr & 0b011111111111111UL)) <<  3) |
                                 (((uint64_t)(                           0b000UL))));
   uint64_t esr_value = *esr_addr;
   // Shire Cache Specification
   // ESR Controlling the SCP configuration has the following fields:
   //
   // | set_base_rsvd |  set_base  | set_size_rsvd |  set_size  | set_mask_rsvd |  set_mask  | tag_mask_rsvd | tag_mask  |
   // --------------------------------------------------------------------------------------------------------------------
   // | 63:60 (4)     | 59:48 (12) | 47:45 (3)     | 44:32 (13) | 31:28 (4)     | 27:16 (12) | 15:12 (4)     | 11:0 (12) |
   // --------------------------------------------------------------------------------------------------------------------

   return ((esr_value >> 32) & (2^(13-1)));
}

void set_test_case(int i, et_tensor_load_l2scp_conf_t* conf, bool* is_local_scp/*, uint64_t sid*/) {
   // conf->dst_start: Starting SCP Line, L2 SCP Cache line (0..(2^17)-1)
   // is_local_scp:    Controls mem addr bits[30:23]. Specifies which shire cache you want to access
   // l2scp_num_lines: Number of lines of the L2 Scratchpad
   uint64_t l2scp_num_lines = 0xA000; //FUTURE Retrieve the value from the ESR (0x280 sets Shire-Cache spec)
   //uint64_t l2scp_num_lines = et_get_l2_scp_size(sid);
   switch(i) {
      case 0:
         conf->dst_start = 0;     //Load starting from SCP Line 0
         *is_local_scp   = false; //Addr bits[30:23] will be set to the corresponding shire_id
         break;
      case 1:
         conf->dst_start = 16;     //Load starting from SCP Line 0
         *is_local_scp   = true;  //Addr bits[30:23] will be set to 0xFFFF, equal to your local shire SCP (Shire Cache Spec)
         break;
      case 2:
         conf->dst_start = 32;
         *is_local_scp   = false; //Addr bits[30:23] will be set to the corresponding shire_id
         break;
      case 3:
         conf->dst_start = (1 << 17)-1; //Set all the bits to 1
         *is_local_scp   = true;  //Addr bits[30:23] will be set to 0xFFFF, equal to your local shire SCP (Shire Cache Spec)
         break;
      default: //Random values to increase coverage
         conf->dst_start = et_get_rand_word(0,((1 << 17)-1));
         *is_local_scp   = et_get_rand_word(0,1) ? true : false;
         conf->num_lines = et_get_rand_word(0,15);
         conf->use_tmask = et_get_rand_word(0,1) ? true : false;
         if (conf->use_tmask) {
             uint64_t tmask = et_get_rand_word();
             tensor_mask(0x0, tmask);
         }
         break;
   }
   // Shire Cache expects the address to not exceed the number of SCP lines
   conf->dst_start = conf->dst_start % (l2scp_num_lines - conf->num_lines);
}

int main() {
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t sid, tid;
  #ifndef POSTSI_PARAM
   uint64_t nid, mid;
  #endif
   uint64_t nelem;
   uint64_t* scp_addr;
   bool is_local_scp;

   // Init marker
   C_TEST_START;

   et_tensor_load_l2scp_conf_t l2scp_conf;

   l2scp_conf.use_tmask = 0;                               // xs[   63]: Not used currently
   l2scp_conf.num_lines = 15;                              // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)
   l2scp_conf.stride    = 64;                              // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
   l2scp_conf.id        = 0;                               // x31[0]: Tensor Operation ID
   l2scp_conf.dst_start = 0;                               // xs[62:48] & xs[5:4]: TO: L2 SCP Cache line (0.(2^17)-1)

   // Get minion ID and thread id
   tid = get_thread_id();
   sid = get_shire_id();
  #ifndef POSTSI_PARAM
   nid = get_neigh_id();
   mid = get_minion_id();
  #endif

   // Setup machine trap vector
   // Normally we do not expect any interrupt
   setup_mtrap_vector();

   // Only thread 0 executes this
  #ifdef POSTSI_PARAM   // allow to run on any different Minion (but only one Minion active)     
   if (tid == 0) { 
  #else
   if ((tid == 0) && (nid == 0)) {
  #endif
      for (int i = 0; i < LOOP_SIZE; i++) {
       #ifdef POSTSI_UST
         // postsi_UST flow: raw_data size reduce to 64KB, so try to keep "ptr" within the range with for() loop 48 iterations;
         uint64_t ptr = (uint64_t)(void*)buffer[(i)%TL_BLOB_SIZE].data[0];
       #else
         uint64_t ptr = (uint64_t)(void*)buffer[(i*i)%TL_BLOB_SIZE].data[0];
       #endif

         l2scp_conf.addr      = (uint64_t) ptr;                  // xs[47: 6]: FROM: Full Virtual Address

         // Set up the parameters for test case
         set_test_case(i, &l2scp_conf, &is_local_scp/*, sid*/);
         // Only one guy does the load into the Shire Cache SCP
        #ifdef POSTSI_PARAM    // allow to run on any different Minion (but only one Minion active)
         if (1) {
        #else
         if ((nid == 0) && (mid == 0)) {
        #endif
            #ifdef POSTSI_UST
              // Tensor_Load_L2SCP fetch lines from L3$ or Mem/DDR (above L2$) into L2SCP.
              // Currently in this test after PostSi UST flow, the (Data) cachelines are mostly in L2$.
              // So, we need to Flush the Data cachelines (16 lines) from L2$ to L3$.  
              // flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl) 
              flush_va(0, 2, ptr, 15, 64, 0, 0);
              WAIT_CACHEOPS; 
            #endif
         
            et_tensor_load_l2scp (&l2scp_conf);
            WAIT_TENSOR_LOAD_L2_0;
            FENCE;
            tensor_error_check();
            // Set everyone's credits to 1
            give_credit_local(-1, -1, 1);
         }
         // Wait for the credits to be 1
         wait_for_credit(1);
         if (i < 4) {
             scp_addr = SCPline_to_addr(l2scp_conf.dst_start, is_local_scp, sid, 0);
             nelem = (((l2scp_conf.num_lines+1)*64)/8); // 15 lines, 64 bytes per line, 8 bytes per uint64_t
             self_check((uint64_t*)ptr, scp_addr, nelem);
         }
      }
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __VPU_PV_TIMA__
#define __VPU_PV_TIMA__ 1

#include "scp_content.h"
#include "tensors.h"

#define L1SCP_SIZE_BYTES 1024*3
#define L1SCP_LINE_SIZE_BYTES 64
#define L1SCP_NUM_LINES 48
#define PASS 0
#define FAIL 1

int compare_l1scp(uint64_t* l1scp_a, uint64_t* l1scp_b) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint64_t);
   uint64_t (*cmp_l1scp_a)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) l1scp_a;
   uint64_t (*cmp_l1scp_b)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) l1scp_b;

   for (int i = 0; i < L1SCP_NUM_LINES; i++) {
      for (int j = 0; j < elems_per_scp_line; j++) {
         if (cmp_l1scp_a[i][j] != cmp_l1scp_b[i][j]) return FAIL;
      }
   }

   return PASS;
}

uint64_t selfcheck() {
   WAIT_TENSOR_FMA;
   uint8_t post_data[L1SCP_SIZE_BYTES] __attribute__((aligned(64)));
   dump_whole_l1scp((uint64_t*) post_data);
   return compare_l1scp((uint64_t*)SCP_CONTENT, (uint64_t*)post_data);
}

void set_mregs(uint64_t mval) {
   __asm__ __volatile__ (
      "mova.m.x %[mval]"
      :
      : [mval] "r" (mval)
      :
   );
}

void load_l1_scp(uint64_t* ptr, uint64_t start, uint64_t num_lines) {
   et_tensor_load_conf_t  load_conf_matrix;

   // Configure X31
   load_conf_matrix.stride         = 64;                     // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
   load_conf_matrix.id             = 0;                      // x31[0]: Tensor Operation ID

   // Configure XS
   load_conf_matrix.use_tmask      = false;                  // xs[   63]: Not used currently
   load_conf_matrix.use_coop       = 0;                      // xs[   62]: 1: cooperation, 0: no cooperation
   load_conf_matrix.transformation = 0x0UL;                  // xs[61:59]: Tensor Load Op dependant
   load_conf_matrix.dst_start      = start;                  // xs[58:53]: TO: SCP Cache line (0..47)
   load_conf_matrix.use_tenb       = 0;                      // xs[   52]: Tensor Load Op dependant
   //                                                           xs[51:48]: WARL(0)
   load_conf_matrix.addr           = (uint64_t) ptr;         // xs[47: 6]: FROM: Full Virtual Address
   load_conf_matrix.offset         = 0;                      // xs[ 5: 4]: WARL(0)
   load_conf_matrix.num_lines      = num_lines-1;            // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)

   et_tensor_load (&load_conf_matrix);
   if (load_conf_matrix.id == 0) {
      WAIT_TENSOR_LOAD_0;
   }
   else {
      WAIT_TENSOR_LOAD_1;
   }
}

void setup_l1_scp() {
   setup_cache_scp();
   float (*l1_scp_content)[FLOAT_PER_SCP_LINE] = (float (*)[FLOAT_PER_SCP_LINE]) SCP_CONTENT;
   load_l1_scp((uint64_t*)l1_scp_content[ 0],  0, 16);
   load_l1_scp((uint64_t*)l1_scp_content[16], 16, 16);
   load_l1_scp((uint64_t*)l1_scp_content[32], 32, 16);
}

void do_tima(bool tenc, bool firstpass, uint64_t tenaoff, uint64_t scp_off) {
   uint64_t tencloc = tenc ? 0 : 1;
   //         use_tmask  b_num_col  a_num_rows  a_num_cols    offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b    scp_loc_a  opcode  first pass
   //         ---------  ---------  ----------  ----------    ------  -------- -------------  -------------  -------- ---------    ---------  ------  ----------
   tensor_fma(   false,       0x3,        0xf,        0xf,  tenaoff,  tencloc,            1,             1,        0,  scp_off,  scp_off+16,      3,  firstpass);
}

uint64_t vpu_tima_l1scp_selfchk(int loop_size) {
   uint64_t res = 0;
   uint64_t mask_tima_gold_ref = 0xffff;
   set_mregs(mask_tima_gold_ref);

   setup_l1_scp();
   
   for (int i = 0; i < loop_size; i++) {
      do_tima( true,  true,  0, 0);
      for (int j = 0; j < 256; j+=3) {
         do_tima( true, false,  0, j);
         do_tima( true, false,  1, j);
         do_tima( true, false,  2, j);
         do_tima( true, false,  3, j);
         do_tima( true, false,  4, j);
         do_tima( true, false,  5, j);
         do_tima( true, false,  6, j);
         do_tima( true, false,  7, j);
         do_tima( true, false,  8, j);
         do_tima( true, false,  9, j);
         do_tima( true, false, 10, j);
         do_tima( true, false, 11, j);
         do_tima( true, false, 12, j);
         do_tima( true, false, 13, j);
         do_tima( true, false, 14, j);
         do_tima( true, false, 15, j);
      }
      do_tima(false, false, 15, 0);
   
      res = selfcheck();
      if (res == FAIL) {
         return res;
      }
   }
   return res;
}

#endif

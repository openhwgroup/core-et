/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "tensors.h"

#include "scp_content.h"

uint32_t gold_ref[4*8*16] = {0x0009d11e, 0x00089271, 0x000aa083, 0x0008a23e, 0x000b0ea8, 0x0009bf74, 0x000adf0e, 0x0008d627, 0x0005dad3, 0x0009116a, 0x0007b3da, 0x00097828, 0x00096dbe, 0x000cde06, 0x000b7533, 0x000af26f, 0x000cd8dd, 0x000b12fc, 0x000bd7aa, 0x0008edcc, 0x000c2b6a, 0x000b2c62, 0x000b227b, 0x0006a7c8, 0x00040c2a, 0x000b39fa, 0x000690af, 0x000b5bd8, 0x0008a3cf, 0x0010451c, 0x000f6601, 0x000d0ab3, 0x000810c0, 0x0007b95d, 0x000aa3e2, 0x00086cda, 0x0009fccd, 0x00095052, 0x000a76d9, 0x00090dc9, 0x000733a4, 0x00097107, 0x0008bf11, 0x0008f51f, 0x0009d759, 0x000a8716, 0x0008876e, 0x00098e63, 0x00096c56, 0x00095bf8, 0x0008a6ca, 0x000883aa, 0x000881bb, 0x0008dd51, 0x000839fb, 0x0007878e, 0x00081179, 0x00093912, 0x00081f56, 0x00090b7b, 0x0008055d, 0x000924b2, 0x00096f61, 0x0008e15a, 0x000c91cc, 0x000c6687, 0x000c9d1f, 0x0008b232, 0x000a64de, 0x000b9624, 0x0009b3c2, 0x00040415, 0x00053f53, 0x000e7fdc, 0x000765c6, 0x000c4c26, 0x000807ca, 0x000e3932, 0x000d4509, 0x000be88b, 0x000a99a2, 0x000b35de, 0x0008b4e8, 0x00088182, 0x00078579, 0x00094f83, 0x0006e879, 0x00052bb4, 0x00085b35, 0x000b466a, 0x0007f522, 0x000a0a81, 0x0006e8af, 0x0008bb8a, 0x0009e131, 0x0008bc7c, 0x000c658f, 0x000b826c, 0x000878cc, 0x0008d016, 0x00094bb5, 0x00099b7b, 0x0007ecf2, 0x0006045e, 0x000694ab, 0x0009b16c, 0x0006a831, 0x000a333b, 0x0006cd98, 0x000bd8ce, 0x000d8a0e, 0x000a8f29, 0x000962eb, 0x0009254f, 0x00061fa6, 0x00088cde, 0x00077875, 0x0007b356, 0x0006c5b4, 0x000835e7, 0x0009187a, 0x0006cd4d, 0x0007bd6a, 0x0007ee09, 0x00070b7c, 0x0007510a, 0x00095c8d, 0x0007da5e};

void load_fp(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f16,   0(%[ptr1])\n"
      "flq2 f17,  32(%[ptr1])\n"
      "flq2 f18,  64(%[ptr1])\n"
      "flq2 f19,  96(%[ptr1])\n"
      "flq2 f20, 128(%[ptr1])\n"
      "flq2 f21, 160(%[ptr1])\n"
      "flq2 f22, 192(%[ptr1])\n"
      "flq2 f23, 224(%[ptr1])\n"
      "flq2 f24, 256(%[ptr1])\n"
      "flq2 f25, 288(%[ptr1])\n"
      "flq2 f26, 320(%[ptr1])\n"
      "flq2 f27, 352(%[ptr1])\n"
      "flq2 f28, 384(%[ptr1])\n"
      "flq2 f29, 416(%[ptr1])\n"
      "flq2 f30, 448(%[ptr1])\n"
      "flq2 f31, 480(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void selfcheck(uint64_t mask_gold_ref) {
   WAIT_TENSOR_FMA;
   __asm__ __volatile__ (
      "feqm.ps m1, f0, f16\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f1, f17\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f2, f18\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f3, f19\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f4, f20\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f5, f21\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f6, f22\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f7, f23\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f8, f24\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f9, f25\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f10, f26\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f11, f27\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f12, f28\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f13, f29\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f14, f30\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f15, f31\n"
      "maskand m0, m0, m1\n"
   );

   uint64_t mask_regs;
   __asm__ __volatile__ (
      "mova.x.m %[mask_regs]"
      : [mask_regs] "=r" (mask_regs)
      :
      :
   );

   if (mask_regs != mask_gold_ref) {
      C_TEST_FAIL;
   }
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

void do_tima(bool tenc, bool firstpass, uint64_t tenaoff) {
   uint64_t tencloc = tenc ? 0 : 1;
   //         use_tmask  b_num_col  a_num_rows  a_num_cols    offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b  scp_loc_a  opcode  first pass
   //         ---------  ---------  ----------  ----------    ------  -------- -------------  -------------  -------- ---------  ---------  ------  ----------
   tensor_fma(   false,         3,          7,          7,  tenaoff,  tencloc,            0,             0,        0,        0,        16,      3,  firstpass);
}

#define LOOP_SIZE 100*1024*1024

int main() {

   uint64_t tid = get_thread_id();
   if (tid == 0) {
      uint64_t mask_gold_ref = 0xffff;
      set_mregs(mask_gold_ref);

      setup_l1_scp();

      load_fp((uint64_t*) gold_ref);

      for (int i = 0; i < LOOP_SIZE; i++) {
         do_tima( true,  true,  0);
         do_tima( true, false,  1);
         do_tima( true, false,  2);
         do_tima( true, false,  3);
         do_tima( true, false,  4);
         do_tima( true, false,  5);
         do_tima( true, false,  6);
         do_tima( true, false,  7);
         do_tima( true, false,  8);
         do_tima( true, false,  9);
         do_tima( true, false, 10);
         do_tima( true, false, 11);
         do_tima( true, false, 12);
         do_tima( true, false, 13);
         do_tima( true, false, 14);
         do_tima(false, false, 15);

         selfcheck(mask_gold_ref);
      }
   }
   C_TEST_PASS;

   return 0;
}

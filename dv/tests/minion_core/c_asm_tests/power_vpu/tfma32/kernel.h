/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __POWER_VPU_TFMA32__
#define __POWER_VPU_TFMA32__ 1

#include "scp_content.h"

uint32_t tima_gold_ref[32*8] = {0x4cb15215, 0x4ca2b7fe, 0x4caaf730, 0x4cba15f4, 0x4ca4271a, 0x4cc2e26f, 0x4ca071fb, 0x4ca24591, 0x4c9c2fb1, 0x4cb2a4d6, 0x4c948bc7, 0x4c8ffd62, 0x4ca926b0, 0x4c95fc50, 0x4ca7543a, 0x4c981687, 0x4cb3d6dd, 0x4ca4a187, 0x4cacf94d, 0x4cbc291d, 0x4ca61af6, 0x4cc5925c, 0x4ca26667, 0x4ca49281, 0x4c9de0d0, 0x4cb47800, 0x4c96a806, 0x4c918e8b, 0x4cab2de2, 0x4c9813da, 0x4ca990cf, 0x4c9a0577, 0x4cb152e4, 0x4ca2de02, 0x4cab1ae5, 0x4cba0b06, 0x4ca405de, 0x4cc27659, 0x4ca06afc, 0x4ca217cb, 0x4c9c3b3b, 0x4cb2454d, 0x4c94a7bb, 0x4c8f5962, 0x4ca90a2f, 0x4c967fc0, 0x4ca79e4d, 0x4c9850a3, 0x4cb12ec3, 0x4ca3018e, 0x4cab1e8b, 0x4cba5800, 0x4ca47e77, 0x4cc2c371, 0x4ca0a0ef, 0x4ca2305e, 0x4c9c7105, 0x4cb287e5, 0x4c9527b5, 0x4c8f7f80, 0x4ca94b1f, 0x4c96820d, 0x4ca76967, 0x4c986e3b, 0x4cb349df, 0x4ca4e234, 0x4cacaef6, 0x4cbc0e74, 0x4ca60c71, 0x4cc4ea91, 0x4ca25a6b, 0x4ca4264d, 0x4c9da5d1, 0x4cb43cda, 0x4c963ff3, 0x4c910e68, 0x4cab1a86, 0x4c97ed06, 0x4ca925f8, 0x4c998d79, 0x4cb1ec59, 0x4ca35a3e, 0x4cabdcca, 0x4cbb5a8c, 0x4ca56274, 0x4cc3c4f2, 0x4ca154a9, 0x4ca2be17, 0x4c9cfb67, 0x4cb39ccf, 0x4c9522f3, 0x4c902de4, 0x4caa14ea, 0x4c96e8b0, 0x4ca82617, 0x4c9941e4, 0x4cb20152, 0x4ca3c8a7, 0x4cabeff5, 0x4cbb5f2e, 0x4ca533ef, 0x4cc3a4da, 0x4ca15cd3, 0x4ca322cf, 0x4c9d3fbe, 0x4cb397a4, 0x4c95be27, 0x4c905895, 0x4caa1afb, 0x4c9739e6, 0x4ca85039, 0x4c9933e7, 0x4cb2fc9f, 0x4ca4d659, 0x4cacc57d, 0x4cbc16ae, 0x4ca62d17, 0x4cc5126c, 0x4ca2a76b, 0x4ca41308, 0x4c9d9c2e, 0x4cb4600f, 0x4c96715d, 0x4c9135c0, 0x4cab2920, 0x4c98006e, 0x4ca8fb97, 0x4c99fee7, 0x4cb23ca1, 0x4ca37cf1, 0x4cac3e1e, 0x4cbb10ab, 0x4ca581da, 0x4cc42b6b, 0x4ca183e8, 0x4ca33fb4, 0x4c9d0f66, 0x4cb3708c, 0x4c95e8fc, 0x4c904a80, 0x4caa619f, 0x4c9727c5, 0x4ca83775, 0x4c993e12, 0x4cb01f45, 0x4ca19062, 0x4caa019e, 0x4cb916ad, 0x4ca336bb, 0x4cc1f36f, 0x4c9fd1fd, 0x4ca186e2, 0x4c9b2d47, 0x4cb19046, 0x4c93dbb6, 0x4c8f05a5, 0x4ca837ee, 0x4c954114, 0x4ca6860c, 0x4c9763cb, 0x4cb2ae60, 0x4ca4cc22, 0x4caccd1c, 0x4cbc39f9, 0x4ca5fcd1, 0x4cc4e1e8, 0x4ca28bf3, 0x4ca40029, 0x4c9dc4ae, 0x4cb452c8, 0x4c96660e, 0x4c912744, 0x4cab2bac, 0x4c97c000, 0x4ca90a5b, 0x4c99d52a, 0x4cb13956, 0x4ca256b1, 0x4cab4030, 0x4cba1cb7, 0x4ca457fa, 0x4cc34639, 0x4ca0c5a3, 0x4ca2bba3, 0x4c9c4569, 0x4cb28c33, 0x4c946b31, 0x4c8ffe64, 0x4ca96ff3, 0x4c961ba6, 0x4ca7976c, 0x4c97dd66, 0x4cb14cc8, 0x4ca27b39, 0x4caae7e5, 0x4cba190f, 0x4ca429a5, 0x4cc2b5ce, 0x4ca09265, 0x4ca249d3, 0x4c9c1c9c, 0x4cb26d59, 0x4c949933, 0x4c8fa20d, 0x4ca93a75, 0x4c961ce9, 0x4ca786b4, 0x4c97fc60, 0x4cb42d13, 0x4ca5aae9, 0x4cae32df, 0x4cbda2f8, 0x4ca72f17, 0x4cc67cfd, 0x4ca3da36, 0x4ca573ec, 0x4c9f1c92, 0x4cb60b1e, 0x4c9779bb, 0x4c927f3c, 0x4cac610c, 0x4c98d0d8, 0x4caa6565, 0x4c9acea2, 0x4cb2ec3a, 0x4ca381b7, 0x4cac9f90, 0x4cbb698a, 0x4ca582ea, 0x4cc4a336, 0x4ca1fd41, 0x4ca3e902, 0x4c9d5811, 0x4cb41492, 0x4c955b13, 0x4c912c04, 0x4caa6273, 0x4c975a74, 0x4ca8f416, 0x4c9956eb, 0x4cb3866b, 0x4ca42230, 0x4cacd956, 0x4cbbafc5, 0x4ca5ef3a, 0x4cc5537f, 0x4ca24cf6, 0x4ca43eff, 0x4c9d7a0a, 0x4cb41883, 0x4c96351c, 0x4c915298, 0x4caac08d, 0x4c97c6d4, 0x4ca959cf, 0x4c99a09f};

void check(uint32_t *mem, uint32_t* gold) {
   __asm__ __volatile__ (
      "flq2 f0, 0(%[mem])\n"
      "flq2 f4, 0(%[gold])\n"
      "flq2 f1, 32(%[mem])\n"
      "flq2 f5, 32(%[gold])\n"
      "flq2 f2, 64(%[mem])\n"
      "flq2 f6, 64(%[gold])\n"
      "flq2 f3, 96(%[mem])\n"
      "flq2 f7, 96(%[gold])\n"
      "feqm.ps m1, f0, f4\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f1, f5\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f2, f6\n"
      "maskand m0, m0, m1\n"
      "feqm.ps m1, f3, f7\n"
      "maskand m0, m0, m1\n"
      :
      : [mem]  "r" (mem), [gold] "r" (gold)
      :
   );
}

void vpu_tima_pv_dump_vpurf(uint32_t* mem) {
   __asm__ __volatile__ (
      "fsq2  f0,   0(%[mem])\n"
      "fsq2  f1,  32(%[mem])\n"
      "fsq2  f2,  64(%[mem])\n"
      "fsq2  f3,  96(%[mem])\n"
      "fsq2  f4, 128(%[mem])\n"
      "fsq2  f5, 160(%[mem])\n"
      "fsq2  f6, 192(%[mem])\n"
      "fsq2  f7, 224(%[mem])\n"
      "fsq2  f8, 256(%[mem])\n"
      "fsq2  f9, 288(%[mem])\n"
      "fsq2 f10, 320(%[mem])\n"
      "fsq2 f11, 352(%[mem])\n"
      "fsq2 f12, 384(%[mem])\n"
      "fsq2 f13, 416(%[mem])\n"
      "fsq2 f14, 448(%[mem])\n"
      "fsq2 f15, 480(%[mem])\n"
      "fsq2 f16, 512(%[mem])\n"
      "fsq2 f17, 544(%[mem])\n"
      "fsq2 f18, 576(%[mem])\n"
      "fsq2 f19, 608(%[mem])\n"
      "fsq2 f20, 640(%[mem])\n"
      "fsq2 f21, 672(%[mem])\n"
      "fsq2 f22, 704(%[mem])\n"
      "fsq2 f23, 736(%[mem])\n"
      "fsq2 f24, 768(%[mem])\n"
      "fsq2 f25, 800(%[mem])\n"
      "fsq2 f26, 832(%[mem])\n"
      "fsq2 f27, 864(%[mem])\n"
      "fsq2 f28, 896(%[mem])\n"
      "fsq2 f29, 928(%[mem])\n"
      "fsq2 f30, 960(%[mem])\n"
      "fsq2 f31, 992(%[mem])\n"
      :
      : [mem]  "r" (mem)
      :
   );
}

uint64_t selfcheck(uint64_t mask_tima_gold_ref) {
   WAIT_TENSOR_FMA;
   uint64_t mask_regs;

   uint32_t vpurf[32*8];
   vpu_tima_pv_dump_vpurf(vpurf);

   uint32_t* mem = vpurf;
   uint32_t* gold = tima_gold_ref;

   for (int i = 0;i < 32; i+=4) {
      check(mem, gold);
      __asm__ __volatile__ (
         "mova.x.m %[mask_regs]"
         : [mask_regs] "=r" (mask_regs)
         :
         :
      );

      if (mask_regs != mask_tima_gold_ref) {
         return 1;
      }

      mem += (8*4);
      gold += (8*4);
   }
   return 0;
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

void do_tfma32(bool tenc, bool firstpass, uint64_t tenaoff, uint64_t scp_off) {
   uint64_t tencloc = tenc ? 0 : 1;
   //         use_tmask  b_num_col  a_num_rows  a_num_cols    offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b    scp_loc_a  opcode  first pass
   //         ---------  ---------  ----------  ----------    ------  -------- -------------  -------------  -------- ---------    ---------  ------  ----------
   tensor_fma(   false,       0x3,        0xf,        0xf,  tenaoff,  tencloc,            1,             1,        0,  scp_off,  scp_off+16,      0,  firstpass);
}

uint64_t vpu_tfma32_power_virus(int loop_size) {
   uint64_t res = 0;
   uint64_t mask_tima_gold_ref = 0xffff;
   set_mregs(mask_tima_gold_ref);

   setup_l1_scp();
   
   for (int i = 0; i < loop_size; i++) {
      do_tfma32( true,  true,  0, 0);
      for (int j = 0; j < 256; j+=3) {
         do_tfma32( true, false,  0, j);
         do_tfma32( true, false,  1, j);
         do_tfma32( true, false,  2, j);
         do_tfma32( true, false,  3, j);
         do_tfma32( true, false,  4, j);
         do_tfma32( true, false,  5, j);
         do_tfma32( true, false,  6, j);
         do_tfma32( true, false,  7, j);
         do_tfma32( true, false,  8, j);
         do_tfma32( true, false,  9, j);
         do_tfma32( true, false, 10, j);
         do_tfma32( true, false, 11, j);
         do_tfma32( true, false, 12, j);
         do_tfma32( true, false, 13, j);
         do_tfma32( true, false, 14, j);
         do_tfma32( true, false, 15, j);
      }
      do_tfma32(false, false, 15, 0);
   
      res = selfcheck(mask_tima_gold_ref);
      if (res == 1) {
         return res;
      }
   }
   return res;
}

#endif

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __POWER_VPU_TIMA__
#define __POWER_VPU_TIMA__ 1

#include "scp_content.h"

uint32_t tima_gold_ref[32*8] = {0x510cc1eb, 0x4c55cc36, 0x4a618f70, 0x4f960752, 0x48f345ed, 0x50412f93, 0x4b3c0d8b, 0x4f72a744, 0x4dc9bb30, 0x4fc699e2, 0x4ad0178a, 0x480aec21, 0x4e486e7b, 0x4a19f974, 0x558b305d, 0x4b8fcea7, 0x51d7ce59, 0x4d27e20f, 0x4af95446, 0x50451533, 0x49a1592a, 0x5103e5cb, 0x4c03bdd4, 0x504218f4, 0x4e71074a, 0x508095fb, 0x4b8f3067, 0x48b4d9d6, 0x4eecde16, 0x4ad428b6, 0x5661f1be, 0x4c52a9da, 0x51bfa472, 0x4d06cdf8, 0x4acee032, 0x502d837c, 0x4976f3f3, 0x50d954f4, 0x4bcd0ac4, 0x500b9b45, 0x4e5b5559, 0x50692899, 0x4b6bf84f, 0x4891f9b1, 0x4ec10cae, 0x4abd1586, 0x56318783, 0x4c1263ff, 0x516f1264, 0x4cba5a28, 0x4aa62e82, 0x4ff7d8d7, 0x495c3974, 0x508d9529, 0x4b8df883, 0x4fcd4055, 0x4e37b8d3, 0x5033f58e, 0x4b32453d, 0x486ddc2f, 0x4ea4637e, 0x4a6e303b, 0x55f5a111, 0x4beff5e2, 0x51f6858e, 0x4d4a7a52, 0x4afd1943, 0x50707655, 0x49c2e4f7, 0x5112eec0, 0x4c10aabd, 0x504db2cc, 0x4e9ff7c3, 0x509834c0, 0x4b9529f3, 0x48d0e7da, 0x4f14c88b, 0x4ae86402, 0x5684159c, 0x4c5ff4bc, 0x51cb4f41, 0x4d1a9ce1, 0x4ad743eb, 0x50480651, 0x49a5531a, 0x50f1e336, 0x4bf3a0a6, 0x5018083f, 0x4e768245, 0x5078491b, 0x4b7e3cde, 0x48ab42d6, 0x4ee46bde, 0x4ab41b32, 0x5646bec9, 0x4c2be8d7, 0x51a78c19, 0x4cf6db63, 0x4abf68e3, 0x5014a147, 0x497663f8, 0x50cd37d5, 0x4bc084b0, 0x5000257b, 0x4e622296, 0x505a14c2, 0x4b609e06, 0x489159af, 0x4ea6e34f, 0x4a86aaf6, 0x56223d9a, 0x4c0464ff, 0x5210c613, 0x4d47561e, 0x4b1796c4, 0x508346e9, 0x49b84a78, 0x514dc2e5, 0x4c2a27ef, 0x5065926b, 0x4eafa1c9, 0x50afefc6, 0x4ba81301, 0x48d9a0a1, 0x4f0b5838, 0x4ae75569, 0x568ac475, 0x4c60df43, 0x51ea0dd8, 0x4d111d6c, 0x4afb286e, 0x50515a52, 0x499c84ab, 0x51189215, 0x4bf6eac5, 0x5030253a, 0x4e8ede06, 0x508b7b9f, 0x4b82d497, 0x48a841af, 0x4ee5792d, 0x4ab08a58, 0x5650fc28, 0x4c2327af, 0x51250994, 0x4c577743, 0x4a468a5a, 0x4f8f2b20, 0x48eecdf0, 0x504fa67a, 0x4b488466, 0x4f7e647e, 0x4ded5b61, 0x4feadaa7, 0x4ada3cc3, 0x48202ec7, 0x4e3d0857, 0x4a03b506, 0x5595b43e, 0x4b89909e, 0x51ead4f4, 0x4d03b888, 0x4ae80e4e, 0x505c0355, 0x499a97e0, 0x510daaed, 0x4c0369e6, 0x504d2a2f, 0x4e936077, 0x5094449a, 0x4b7d179d, 0x48cdf340, 0x4ee89127, 0x4aad9605, 0x5645b08e, 0x4c2f6c65, 0x52098d67, 0x4d1deab6, 0x4b1a9b0c, 0x50747088, 0x49b2d290, 0x5125b237, 0x4c0db875, 0x506cb5eb, 0x4eb9ebf8, 0x50d2965e, 0x4b90aa67, 0x48f1f18b, 0x4f2311f2, 0x4ae1dfe2, 0x5670ed2c, 0x4c4df0bb, 0x5119db54, 0x4c507a59, 0x4a56e8f7, 0x4faa6cf3, 0x48f46263, 0x5037a5c9, 0x4b451794, 0x4f92931b, 0x4df1b739, 0x4fece914, 0x4adcbede, 0x4839c995, 0x4e49a62d, 0x4a1c6e63, 0x5597e738, 0x4ba2f41f, 0x52070614, 0x4d2df095, 0x4b0a3e41, 0x50816500, 0x49d9941f, 0x510fd445, 0x4c37c298, 0x507ee292, 0x4eb6e09a, 0x50bf545b, 0x4ba397d0, 0x48fce695, 0x4f099a2c, 0x4acda5cf, 0x5667d842, 0x4c45ff87, 0x51d95ebe, 0x4d0e78ee, 0x4ae02850, 0x504b72f0, 0x49aba6e9, 0x50dad712, 0x4bfd2c78, 0x503916d1, 0x4e900efe, 0x5098b3f7, 0x4b798ed2, 0x48c64319, 0x4ed42fab, 0x4aa55aa1, 0x56353d0e, 0x4c20b54c, 0x510bc86b, 0x4c71ca58, 0x4a30aa1f, 0x4f92c41e, 0x48fe91cf, 0x502e0f7e, 0x4b5263bb, 0x4f7aaf60, 0x4de31645, 0x4fe6f71c, 0x4ac509fb, 0x481f0399, 0x4e390315, 0x49fc6979, 0x55784fba, 0x4b94baf1};

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

void do_tima(bool tenc, bool firstpass, uint64_t tenaoff, uint64_t scp_off) {
   uint64_t tencloc = tenc ? 0 : 1;
   //         use_tmask  b_num_col  a_num_rows  a_num_cols    offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b    scp_loc_a  opcode  first pass
   //         ---------  ---------  ----------  ----------    ------  -------- -------------  -------------  -------- ---------    ---------  ------  ----------
   tensor_fma(   false,       0x3,        0xf,        0xf,  tenaoff,  tencloc,            1,             1,        0,  scp_off,  scp_off+16,      3,  firstpass);
}

uint64_t vpu_tima_power_virus(int loop_size) {
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
   
      res = selfcheck(mask_tima_gold_ref);
      if (res == 1) {
         return res;
      }
   }
   setup_cache_shared();
   return res;
}

#endif

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

//      uint32_t buffer[32][8];

uint32_t buffer[32*8] = {0x510cc1eb, 0x4c55cc36, 0x4a618f70, 0x4f960752, 0x48f345ed, 0x50412f93, 0x4b3c0d8b, 0x4f72a744, 0x4dc9bb30, 0x4fc699e2, 0x4ad0178a, 0x480aec21, 0x4e486e7b, 0x4a19f974, 0x558b305d, 0x4b8fcea7, 0x51d7ce59, 0x4d27e20f, 0x4af95446, 0x50451533, 0x49a1592a, 0x5103e5cb, 0x4c03bdd4, 0x504218f4, 0x4e71074a, 0x508095fb, 0x4b8f3067, 0x48b4d9d6, 0x4eecde16, 0x4ad428b6, 0x5661f1be, 0x4c52a9da, 0x51bfa472, 0x4d06cdf8, 0x4acee032, 0x502d837c, 0x4976f3f3, 0x50d954f4, 0x4bcd0ac4, 0x500b9b45, 0x4e5b5559, 0x50692899, 0x4b6bf84f, 0x4891f9b1, 0x4ec10cae, 0x4abd1586, 0x56318783, 0x4c1263ff, 0x516f1264, 0x4cba5a28, 0x4aa62e82, 0x4ff7d8d7, 0x495c3974, 0x508d9529, 0x4b8df883, 0x4fcd4055, 0x4e37b8d3, 0x5033f58e, 0x4b32453d, 0x486ddc2f, 0x4ea4637e, 0x4a6e303b, 0x55f5a111, 0x4beff5e2, 0x51f6858e, 0x4d4a7a52, 0x4afd1943, 0x50707655, 0x49c2e4f7, 0x5112eec0, 0x4c10aabd, 0x504db2cc, 0x4e9ff7c3, 0x509834c0, 0x4b9529f3, 0x48d0e7da, 0x4f14c88b, 0x4ae86402, 0x5684159c, 0x4c5ff4bc, 0x51cb4f41, 0x4d1a9ce1, 0x4ad743eb, 0x50480651, 0x49a5531a, 0x50f1e336, 0x4bf3a0a6, 0x5018083f, 0x4e768245, 0x5078491b, 0x4b7e3cde, 0x48ab42d6, 0x4ee46bde, 0x4ab41b32, 0x5646bec9, 0x4c2be8d7, 0x51a78c19, 0x4cf6db63, 0x4abf68e3, 0x5014a147, 0x497663f8, 0x50cd37d5, 0x4bc084b0, 0x5000257b, 0x4e622296, 0x505a14c2, 0x4b609e06, 0x489159af, 0x4ea6e34f, 0x4a86aaf6, 0x56223d9a, 0x4c0464ff, 0x5210c613, 0x4d47561e, 0x4b1796c4, 0x508346e9, 0x49b84a78, 0x514dc2e5, 0x4c2a27ef, 0x5065926b, 0x4eafa1c9, 0x50afefc6, 0x4ba81301, 0x48d9a0a1, 0x4f0b5838, 0x4ae75569, 0x568ac475, 0x4c60df43, 0x51ea0dd8, 0x4d111d6c, 0x4afb286e, 0x50515a52, 0x499c84ab, 0x51189215, 0x4bf6eac5, 0x5030253a, 0x4e8ede06, 0x508b7b9f, 0x4b82d497, 0x48a841af, 0x4ee5792d, 0x4ab08a58, 0x5650fc28, 0x4c2327af, 0x51250994, 0x4c577743, 0x4a468a5a, 0x4f8f2b20, 0x48eecdf0, 0x504fa67a, 0x4b488466, 0x4f7e647e, 0x4ded5b61, 0x4feadaa7, 0x4ada3cc3, 0x48202ec7, 0x4e3d0857, 0x4a03b506, 0x5595b43e, 0x4b89909e, 0x51ead4f4, 0x4d03b888, 0x4ae80e4e, 0x505c0355, 0x499a97e0, 0x510daaed, 0x4c0369e6, 0x504d2a2f, 0x4e936077, 0x5094449a, 0x4b7d179d, 0x48cdf340, 0x4ee89127, 0x4aad9605, 0x5645b08e, 0x4c2f6c65, 0x52098d67, 0x4d1deab6, 0x4b1a9b0c, 0x50747088, 0x49b2d290, 0x5125b237, 0x4c0db875, 0x506cb5eb, 0x4eb9ebf8, 0x50d2965e, 0x4b90aa67, 0x48f1f18b, 0x4f2311f2, 0x4ae1dfe2, 0x5670ed2c, 0x4c4df0bb, 0x5119db54, 0x4c507a59, 0x4a56e8f7, 0x4faa6cf3, 0x48f46263, 0x5037a5c9, 0x4b451794, 0x4f92931b, 0x4df1b739, 0x4fece914, 0x4adcbede, 0x4839c995, 0x4e49a62d, 0x4a1c6e63, 0x5597e738, 0x4ba2f41f, 0x52070614, 0x4d2df095, 0x4b0a3e41, 0x50816500, 0x49d9941f, 0x510fd445, 0x4c37c298, 0x507ee292, 0x4eb6e09a, 0x50bf545b, 0x4ba397d0, 0x48fce695, 0x4f099a2c, 0x4acda5cf, 0x5667d842, 0x4c45ff87, 0x51d95ebe, 0x4d0e78ee, 0x4ae02850, 0x504b72f0, 0x49aba6e9, 0x50dad712, 0x4bfd2c78, 0x503916d1, 0x4e900efe, 0x5098b3f7, 0x4b798ed2, 0x48c64319, 0x4ed42fab, 0x4aa55aa1, 0x56353d0e, 0x4c20b54c, 0x510bc86b, 0x4c71ca58, 0x4a30aa1f, 0x4f92c41e, 0x48fe91cf, 0x502e0f7e, 0x4b5263bb, 0x4f7aaf60, 0x4de31645, 0x4fe6f71c, 0x4ac509fb, 0x481f0399, 0x4e390315, 0x49fc6979, 0x55784fba, 0x4b94baf1};

int main() {

   // Init marker
   C_TEST_START;

   __asm__ __volatile__(
      "mov.m.x m0, x0, 0xff\n"

   "fmadd.ps f0, f0, f0, f0\n"
//   "fmadd.ps f1, f1, f1, f1\n"
//   "fmadd.ps f2, f2, f2, f2\n"
//   "fmadd.ps f3, f3, f3, f3\n"
//   "fmadd.ps f4, f4, f4, f4\n"
//   "fmadd.ps f5, f5, f5, f5\n"
//   "fmadd.ps f6, f6, f6, f6\n"
//   "fmadd.ps f7, f7, f7, f7\n"
//   "fmadd.ps f8, f8, f8, f8\n"
//   "fmadd.ps f9, f9, f9, f9\n"
//   "fmadd.ps f10, f10, f10, f10\n"
//   "fmadd.ps f11, f11, f11, f11\n"
//   "fmadd.ps f12, f12, f12, f12\n"
//   "fmadd.ps f13, f13, f13, f13\n"
//   "fmadd.ps f14, f14, f14, f14\n"
//   "fmadd.ps f15, f15, f15, f15\n"
//   "fmadd.ps f16, f16, f16, f16\n"
//   "fmadd.ps f17, f17, f17, f17\n"
//   "fmadd.ps f18, f18, f18, f18\n"
//   "fmadd.ps f19, f19, f19, f19\n"
//   "fmadd.ps f20, f20, f20, f20\n"
//   "fmadd.ps f21, f21, f21, f21\n"
//   "fmadd.ps f22, f22, f22, f22\n"
//   "fmadd.ps f23, f23, f23, f23\n"
//   "fmadd.ps f24, f24, f24, f24\n"
//   "fmadd.ps f25, f25, f25, f25\n"
//   "fmadd.ps f26, f26, f26, f26\n"
//   "fmadd.ps f27, f27, f27, f27\n"
//   "fmadd.ps f28, f28, f28, f28\n"
//   "fmadd.ps f29, f29, f29, f29\n"
//   "fmadd.ps f30, f30, f30, f30\n"
//   "fmadd.ps f31, f31, f31, f31\n"

        "fcvt.s.w f0,  x0\n"
        "fcvt.s.w f1,  x0\n"
        "fcvt.s.w f2,  x0\n"
        "fcvt.s.w f3,  x0\n"
        "fcvt.s.w f4,  x0\n"
        "fcvt.s.w f5,  x0\n"
        "fcvt.s.w f6,  x0\n"
        "fcvt.s.w f7,  x0\n"
        "fcvt.s.w f8,  x0\n"
        "fcvt.s.w f9,  x0\n"
        "fcvt.s.w f10,  x0\n"
        "fcvt.s.w f11,  x0\n"
        "fcvt.s.w f12,  x0\n"
        "fcvt.s.w f13,  x0\n"
        "fcvt.s.w f14,  x0\n"
        "fcvt.s.w f15,  x0\n"
        "fcvt.s.w f16,  x0\n"
        "fcvt.s.w f17,  x0\n"
        "fcvt.s.w f18,  x0\n"
        "fcvt.s.w f19,  x0\n"
        "fcvt.s.w f20,  x0\n"
        "fcvt.s.w f21,  x0\n"
        "fcvt.s.w f22,  x0\n"
        "fcvt.s.w f23,  x0\n"
        "fcvt.s.w f24,  x0\n"
        "fcvt.s.w f25,  x0\n"
        "fcvt.s.w f26,  x0\n"
        "fcvt.s.w f27,  x0\n"
        "fcvt.s.w f28,  x0\n"
        "fcvt.s.w f29,  x0\n"
        "fcvt.s.w f30,  x0\n"
        "fcvt.s.w f31,  x0\n"

   : : [buffer] "r" (buffer)
     
   );

for (int i = 0; i < 1024*1024*102; i++) {
   __asm__ __volatile__ ("add x0, x0, x0\n");
}

   __asm__ __volatile__(

          "add x31, x0, %[buffer]\n"
        "flw.ps   f0,  0x0 (x31)\n"
        "flw.ps   f1,  0x20(x31)\n"
        "flw.ps   f2,  0x40(x31)\n"
        "flw.ps   f3,  0x60(x31)\n"
        "flw.ps   f4,  0x80(x31)\n"
        "flw.ps   f5,  0xA0(x31)\n"
        "flw.ps   f6,  0xC0(x31)\n"
        "flw.ps   f7,  0xE0(x31)\n"
        "flw.ps   f8,  0x0 (x31)\n"
        "flw.ps   f9,  0x20(x31)\n"
        "flw.ps   f10, 0x40(x31)\n"
        "flw.ps   f11, 0x60(x31)\n"
        "flw.ps   f12, 0x80(x31)\n"
        "flw.ps   f13, 0xA0(x31)\n"
        "flw.ps   f14, 0xC0(x31)\n"
        "flw.ps   f15, 0xE0(x31)\n"
        "flw.ps   f16, 0x0 (x31)\n"
        "flw.ps   f17, 0x20(x31)\n"
        "flw.ps   f18, 0x40(x31)\n"
        "flw.ps   f19, 0x60(x31)\n"
        "flw.ps   f20, 0x80(x31)\n"
        "flw.ps   f21, 0xA0(x31)\n"
        "flw.ps   f22, 0xC0(x31)\n" 
        "flw.ps   f23, 0xE0(x31)\n"
        "flw.ps   f24, 0x0 (x31)\n"            
        "flw.ps   f25, 0x20(x31)\n"
        "flw.ps   f26, 0x40(x31)\n"
        "flw.ps   f27, 0x60(x31)\n"
        "flw.ps   f28, 0x80(x31)\n"
        "flw.ps   f29, 0xA0(x31)\n"
        "flw.ps   f30, 0xC0(x31)\n"
        "flw.ps   f31, 0xE0(x31)\n"

                          
   : : [buffer] "r" (buffer)
     
   );

   // End marker
   C_TEST_PASS;
   return 0;
}

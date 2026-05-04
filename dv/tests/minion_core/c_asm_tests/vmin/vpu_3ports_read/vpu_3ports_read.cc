/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

//#include "../power_vpu/kernel.h"

inline void vpu_fmaddps_load_f0(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f0, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

inline void vpu_fmaddps_load_f31(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f31, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

inline void vpu_fmaddps_set_m0_m1(uint64_t m0_m1) {
   __asm__ __volatile__ (
      "mova.m.x %[m0_m1]"
      :
      : [m0_m1] "r" (m0_m1)
      :
   );
}

int main() {

   /*
   // Init marker
   C_TEST_START;
   */
   uint64_t mask_gold_ref = 0xffffffffff;
   vpu_fmaddps_set_m0_m1(mask_gold_ref);

   int tid = get_thread_id();
   if (tid == 0) {
       volatile float fp_src[8] = {4.6791792323853665, 9.888869407288553, 10.593939116934417,  4.723527639728084, 8.172980173709412,  7.583268477192551, 10.276484180990414, 10.500393800612335};
       volatile uint32_t fp_gold_ref[8] = {0x41d49757, 0x42d75b72, 0x42f5a6a5, 0x41d8482b, 0x4295f0f1, 0x42822db0, 0x42e7c3e7, 0x42f18470};
       vpu_fmaddps_load_f0((uint64_t*)fp_src);
       vpu_fmaddps_load_f31((uint64_t*)fp_gold_ref);
   }
   else {
       volatile float fp_src[8] = {6.495942425355921,  8.993313656846523, 3.489153871988233, 0.4085908879364616,  9.488701311543025,  7.649850424644063, 1.2659858272029276,  5.040690601992794};
       volatile uint32_t fp_gold_ref[8] = {0x4242c5da, 0x42b3befb, 0x417a9d13, 0x3f13566b, 0x42c70c5d, 0x42845712, 0x403798e1, 0x41f39811};
       vpu_fmaddps_load_f0((uint64_t*)fp_src);
       vpu_fmaddps_load_f31((uint64_t*)fp_gold_ref);
   }

   for (int i = 0; i < 1024*1024*1024; i++) {
      __asm__ __volatile__ (
         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"

         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"

         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"

         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"

         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"

         "fmadd.ps  f1, f0, f0, f0\n"
         "feqm.ps m2, f1, f31\n"
         "maskand m1, m1, m2\n"
      );
   }

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

   C_TEST_PASS;
   return 0;
}

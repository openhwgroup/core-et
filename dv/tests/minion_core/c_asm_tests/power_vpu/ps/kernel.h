/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __POWER_VPU_PS__
#define __POWER_VPU_PS__ 1

void vpu_pv_set_m0_m1(uint64_t m0_m1) {
   __asm__ __volatile__ (
      "mova.m.x %[m0_m1]"
      :
      : [m0_m1] "r" (m0_m1)
      :
   );
}

void vpu_pv_load_f0(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f0, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f1(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f1, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f2(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f2, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f3(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f3, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f28(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f28, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f29(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f29, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f30(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f30, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_load_f31(uint64_t *mem) {
   __asm__ __volatile__ (
      "flq2 f31, 0(%[ptr1])\n"
      :
      : [ptr1]  "r" (mem)
      :
   );
}

void vpu_pv_setup0() {
   volatile float fp0[8] = {6.495942425355921,  8.993313656846523, 3.489153871988233, 0.4085908879364616,  9.488701311543025,  7.649850424644063, 1.2659858272029276,  5.040690601992794};
   volatile float fp1[8] = {10.26807481337694, 7.1361025671387335, 5.970622520608017,  9.955459741339057, 3.3585531873670718, 2.1418013771720448,  4.443178235056731,  6.169331426859764};
   volatile float fp2[8] = {6.404517107008146,   3.81819055067076,  7.78785072434012, 2.4909725987724687,  8.728038495741082,  9.271151154019305, 2.7606945395838385, 2.5334389601906646};
   vpu_pv_load_f1((uint64_t*)fp0);
   vpu_pv_load_f2((uint64_t*)fp1);
   vpu_pv_load_f3((uint64_t*)fp2);

   volatile uint32_t gold_ref_0[8] = {0x429235ef, 0x4287fda5, 0x41e4f651, 0x40d1e0ba, 0x422262a8, 0x41cd3eb1, 0x41062bcf, 0x42068647};
   vpu_pv_load_f0((uint64_t*)gold_ref_0);

   /****************************************************************************************************************************************************************************/

   volatile float fp3[8] = {7.922846243030589,  7.951783617278872, 3.243987856108815, 1.350838403538006,  8.241438680694662, 10.914914528770467,  5.791815839279895, 10.108810951451593};
   volatile float fp4[8] = {10.16780962875947, 10.239534242656244, 9.262630921398772, 6.565292248236178,  6.343653061007674,  1.486750803655164,  8.075994433003018, 0.9059698201370306};
   volatile float fp5[8] = {9.138845711481778, 0.7045719518527581, 6.060770415748755,  3.22500927980329, 0.8981216713184665,  5.694721782774539, 3.5959052499031863, 0.2034496693650486};
   vpu_pv_load_f28((uint64_t*)fp3);
   vpu_pv_load_f29((uint64_t*)fp4);
   vpu_pv_load_f30((uint64_t*)fp5);

   volatile uint32_t gold_ref_1[8] = {0x42b364c8, 0x42a44118, 0x42106f3d, 0x41417fa0, 0x4254b73f, 0x41af613d, 0x42497b79, 0x4115c9a3};
   vpu_pv_load_f31((uint64_t*)gold_ref_1);
}

void vpu_pv_setup1() {

   volatile float fp0[8] = {4.6791792323853665, 9.888869407288553, 10.593939116934417,  4.723527639728084, 8.172980173709412,  7.583268477192551, 10.276484180990414, 10.500393800612335};
   volatile float fp1[8] = { 8.931139730835381, 1.936338189980582,  7.482601769653853, 1.8465752640695512, 8.562900887812571, 2.9489933180766528,  2.448864893948106, 3.8508894419560056};
   volatile float fp2[8] = { 5.066726120811531,  7.82523896480587, 2.7674846424987836,  4.842661976272451, 6.856733998319489, 1.8871823747583134, 1.1202174623623495, 10.093619453591886};
   vpu_pv_load_f1((uint64_t*)fp0);
   vpu_pv_load_f2((uint64_t*)fp1);
   vpu_pv_load_f3((uint64_t*)fp2);

   volatile uint32_t gold_ref_0[8] = {0x423b6db4, 0x41d7c998, 0x42a4134f, 0x41590a49, 0x4299aeab, 0x41c20064, 0x41d2499b, 0x424a1e2f};
   vpu_pv_load_f0((uint64_t*)gold_ref_0);

   /****************************************************************************************************************************************************************************/

   volatile float fp3[8] = {4.4532868607962115, 10.226065457550645,  9.460105598030646,   7.634876748568018, 1.6861839078110483, 8.633154098329157, 9.314858633110235,  9.868010930202294};
   volatile float fp4[8] = { 7.090934398908858,  6.069672240892571, 3.9631940361052824,   9.806098546334006,  2.738036760257621, 9.978815212812913, 5.622762376973514, 2.3221180997895283};
   volatile float fp5[8] = { 7.842280163181539,  3.043255919591418, 10.370986446525636, 0.16043322097567136, 10.885759929197516, 7.863890418895694,  4.62426062109188, 0.5684462449289374};
   vpu_pv_load_f28((uint64_t*)fp3);
   vpu_pv_load_f29((uint64_t*)fp4);
   vpu_pv_load_f30((uint64_t*)fp5);

   volatile uint32_t gold_ref_1[8] = {0x421dae54, 0x42823968, 0x423f73f1, 0x42960ebe, 0x41780aa0, 0x42bc066b, 0x4263ff7b, 0x41bbdd74};
   vpu_pv_load_f31((uint64_t*)gold_ref_1);
}

uint64_t check(uint64_t mask_gold_ref) {
   __asm__ __volatile__ (
      "feqm.ps m2, f0, f4\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f5\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f6\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f7\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f8\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f9\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f10\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f11\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f12\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f13\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f14\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f15\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f16\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f17\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f18\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f19\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f20\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f21\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f22\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f23\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f24\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f25\n"
      "maskand m3, m3, m4\n"
      
      "feqm.ps m2, f0, f26\n"
      "maskand m1, m1, m2\n"
      
      "feqm.ps m4, f31, f27\n"
      "maskand m3, m3, m4\n"
   );

   uint64_t mask_regs;
   __asm__ __volatile__ (
      "mova.x.m %[mask_regs]" : [mask_regs] "=r" (mask_regs) : : 
   );

   if (mask_regs != mask_gold_ref) return 0x1;
   return 0x0;

}

uint64_t vpu_ps_power_virus(int loop_size, int inner_loop_size, bool skip_check) {
   uint64_t mask_gold_ref = 0xffffffffff;
   vpu_pv_set_m0_m1(mask_gold_ref);

   uint64_t tid = get_thread_id();
   if (tid == 0) {
      vpu_pv_setup0();
   }
   else {
      vpu_pv_setup1();
   }

   for (int i = 0; i < loop_size; i++) {

      for (int j = 0; j < inner_loop_size; j++) {
         __asm__ __volatile__ (
            "fmadd.ps   f4, f1, f2, f3\n"
            "fmadd.ps   f5, f28, f29, f30\n"
            "fmadd.ps   f6, f1, f2, f3\n"
            "fmadd.ps   f7, f28, f29, f30\n"
            "fmadd.ps   f8, f1, f2, f3\n"
            "fmadd.ps   f9, f28, f29, f30\n"
            "fmadd.ps  f10, f1, f2, f3\n"
            "fmadd.ps  f11, f28, f29, f30\n"
            "fmadd.ps  f12, f1, f2, f3\n"
            "fmadd.ps  f13, f28, f29, f30\n"
            "fmadd.ps  f14, f1, f2, f3\n"
            "fmadd.ps  f15, f28, f29, f30\n"
            "fmadd.ps  f16, f1, f2, f3\n"
            "fmadd.ps  f17, f28, f29, f30\n"
            "fmadd.ps  f18, f1, f2, f3\n"
            "fmadd.ps  f19, f28, f29, f30\n"
            "fmadd.ps  f20, f1, f2, f3\n"
            "fmadd.ps  f21, f28, f29, f30\n"
            "fmadd.ps  f22, f1, f2, f3\n"
            "fmadd.ps  f23, f28, f29, f30\n"
            "fmadd.ps  f24, f1, f2, f3\n"
            "fmadd.ps  f25, f28, f29, f30\n"
            "fmadd.ps  f26, f1, f2, f3\n"
            "fmadd.ps  f27, f28, f29, f30\n"
         );
      }

      if (!skip_check) {
         uint64_t res = check(mask_gold_ref);
         if (res) return res;
      }
   }

   // if checks were skipped in the hot loop, run once at the end for sanity
   if (skip_check) {
      uint64_t res = check(mask_gold_ref);
      if (res) return res;
   }
   return 0x0;
}
#endif

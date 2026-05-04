/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#define DEST_L2 1

#define VPU_RF_SWITCH(CMP_FUNCT, FSRC, EXPECTED, OBS_ADDR) \
         switch (FSRC) { \
            case 0: \
               CMP_FUNCT( f0, EXPECTED, OBS_ADDR) \
            case 1: \
               CMP_FUNCT( f1, EXPECTED, OBS_ADDR) \
            case 2: \
               CMP_FUNCT( f2, EXPECTED, OBS_ADDR) \
            case 3: \
               CMP_FUNCT( f3, EXPECTED, OBS_ADDR) \
            case 4: \
               CMP_FUNCT( f4, EXPECTED, OBS_ADDR) \
            case 5: \
               CMP_FUNCT( f5, EXPECTED, OBS_ADDR) \
            case 6: \
               CMP_FUNCT( f6, EXPECTED, OBS_ADDR) \
            case 7: \
               CMP_FUNCT( f7, EXPECTED, OBS_ADDR) \
            case 8: \
               CMP_FUNCT( f8, EXPECTED, OBS_ADDR) \
            case 9: \
               CMP_FUNCT( f9, EXPECTED, OBS_ADDR) \
            case 10: \
               CMP_FUNCT(f10, EXPECTED, OBS_ADDR) \
            case 11: \
               CMP_FUNCT(f11, EXPECTED, OBS_ADDR) \
            case 12: \
               CMP_FUNCT(f12, EXPECTED, OBS_ADDR) \
            case 13: \
               CMP_FUNCT(f13, EXPECTED, OBS_ADDR) \
            case 14: \
               CMP_FUNCT(f14, EXPECTED, OBS_ADDR) \
            case 15: \
               CMP_FUNCT(f15, EXPECTED, OBS_ADDR) \
            case 16: \
               CMP_FUNCT(f16, EXPECTED, OBS_ADDR) \
            case 17: \
               CMP_FUNCT(f17, EXPECTED, OBS_ADDR) \
            case 18: \
               CMP_FUNCT(f18, EXPECTED, OBS_ADDR) \
            case 19: \
               CMP_FUNCT(f19, EXPECTED, OBS_ADDR) \
            case 20: \
               CMP_FUNCT(f20, EXPECTED, OBS_ADDR) \
            case 21: \
               CMP_FUNCT(f21, EXPECTED, OBS_ADDR) \
            case 22: \
               CMP_FUNCT(f22, EXPECTED, OBS_ADDR) \
            case 23: \
               CMP_FUNCT(f23, EXPECTED, OBS_ADDR) \
            case 24: \
               CMP_FUNCT(f24, EXPECTED, OBS_ADDR) \
            case 25: \
               CMP_FUNCT(f25, EXPECTED, OBS_ADDR) \
            case 26: \
               CMP_FUNCT(f26, EXPECTED, OBS_ADDR) \
            case 27: \
               CMP_FUNCT(f27, EXPECTED, OBS_ADDR) \
            case 28: \
               CMP_FUNCT(f28, EXPECTED, OBS_ADDR) \
            case 29: \
               CMP_FUNCT(f29, EXPECTED, OBS_ADDR) \
            case 30: \
               CMP_FUNCT(f30, EXPECTED, OBS_ADDR) \
            case 31: \
               CMP_FUNCT(f31, EXPECTED, OBS_ADDR) \
         }

#define COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, LANE) \
         __asm__ volatile("fmvz.x.ps %[IREG]," #FSRC ", " #LANE : [IREG] "=r" (EXPECTED) : : ); \
         if (OBS_ADDR[LANE] != EXPECTED) return 1; \
         /*       use_tmask,     dst,                      addr, num_lines, stride, id, warl */ \
         evict_va(        0, DEST_L2, (uint64_t) &OBS_ADDR[LANE],         0,      0,  0,    0);  \
         WAIT_CACHEOPS

#define COMPARE_16B(FSRC, EXPECTED, OBS_ADDR) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 0) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 1) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 2) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 3) \
         break;

#define COMPARE_32B(FSRC, EXPECTED, OBS_ADDR) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 0) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 1) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 2) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 3) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 4) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 5) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 6) \
         COMPARE_LANE(FSRC, EXPECTED, OBS_ADDR, 7) \
         break;

int selfcheck_ts_16bytes(const uint64_t fsrc, const uint64_t VA) {
   uint32_t  expected;
   uint32_t *addr = (uint32_t*) VA;
   VPU_RF_SWITCH(COMPARE_16B, fsrc, expected, addr)
   return 0;
}

int selfcheck_ts_32bytes(const uint64_t fsrc, const uint64_t VA) {
   uint32_t  expected;
   uint32_t *addr = (uint32_t*) VA;
   VPU_RF_SWITCH(COMPARE_32B, fsrc, expected, addr)
   return 0;
}

void selfcheck_ts_compare(const uint64_t addr, const int freg, const int bytes, const int fstep) {
   int fail = 0;
   if (bytes == 16) {
      fail = selfcheck_ts_16bytes(freg, addr);
   }
   else if (bytes == 32) {
      fail = selfcheck_ts_32bytes(freg, addr);
   }
   else if (bytes == 64) {
      fail = selfcheck_ts_32bytes(freg, addr);
      if (!fail) {
         fail = selfcheck_ts_32bytes(freg+fstep, addr+32);
      }
   }
   if (fail) C_TEST_FAIL
}

void wait_for_everyone() {
   give_credit_local(-1, -1, 0);

   // wait for all other harts to be done consuming the shared counter, plus the credit we just sent to ourselves
   for (uint32_t j = 0; j < NUM_MINIONS; ++j) {
      wait_for_credit(0);
   }
}

//                     xs[63:62],       xs[61:57],      xs[56:55],     xs[54:51],    xs[47:4],       x31[47:4]
void selfcheck_ts(uint64_t fstep, uint64_t fstart, uint64_t bytes, uint64_t rows, uint64_t VA, uint64_t stride) {
   WAIT_TENSOR_STORE
   wait_for_everyone();

   fstep  = fstep + 1;
   fstart = fstart;
   bytes  = (bytes + 1) * 16;
   rows   = rows + 1;
   stride = stride & 0xFFFFFFFFFF0UL;
   VA     = VA & (0xFFFFFFFFFFFFFFF0);

   int mult = (bytes == 64) ? 2 : 1;
   for (uint64_t i = 0; i < rows; i++) {
      const uint64_t reg = (fstart + mult*i*fstep) % 32;
      uint64_t va_sc = VA + (i*stride);
      selfcheck_ts_compare(va_sc, reg, bytes, fstep);
   }

   wait_for_everyone();
}

void post_init_fp_evict(void *addr) {
   /*       use_tmask,     dst,            addr, num_lines, stride, id, warl */
   evict_va(        0, DEST_L2, (uint64_t) addr,        15,     64,  0,    0);
   WAIT_CACHEOPS
   wait_for_everyone();
}

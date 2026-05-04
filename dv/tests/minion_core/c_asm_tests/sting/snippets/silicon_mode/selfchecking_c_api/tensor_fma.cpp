
#include "tensor_fma.h"

bool tfma32_skip(float32_t a, float32_t b) {
   const bool skip_a = (a.v == 0);
   const bool skip_b = (b.v == 0);
   return (skip_a || skip_b);
}

bool tfma16_skip(float16_t a1, float16_t b1, float16_t a2, float16_t b2) {
   const bool skip_a = (a1.v == 0) && (a2.v == 0);
   const bool skip_b = (b1.v == 0) && (b2.v == 0);
   return (skip_a || skip_b);
}

bool tima8_skip(uint8_t a1, uint8_t b1, uint8_t a2, uint8_t b2, uint8_t a3, uint8_t b3, uint8_t a4, uint8_t b4){
   const bool skip_a = (a1 == 0) && (a2 == 0) && (a3 == 0) && (a4 == 0);
   const bool skip_b = (b1 == 0) && (b2 == 0) && (b3 == 0) && (b4 == 0);
   return (skip_a || skip_b);
}

int compare_tenc(uint64_t hid, uint64_t* tenc_a, uint64_t* tenc_b) {
   const unsigned int SIZE = LANES_PER_VPU_REG / (sizeof(uint64_t)/BYTES_PER_LANE);
   uint64_t (*cmp_tenc_a)[SIZE] = (uint64_t (*)[SIZE]) tenc_a;
   uint64_t (*cmp_tenc_b)[SIZE] = (uint64_t (*)[SIZE]) tenc_b;
   for (int i = 0; i < VPURF_NUM_REGS; i++) {
      for (int j = 0; j < SIZE; j++) {
         if (cmp_tenc_a[i][j] != cmp_tenc_b[i][j]) {
            et_printf("[dbg] H%d error: tenc mismatch\n", hid);
            et_printf("[dbg] H%d   tenc_a(chk) : tenc%u[%u/%u] = 0x%lx (addr: %lx)\n", hid, i, j / 2, j / 2 + 1, cmp_tenc_a[i][j], &cmp_tenc_a[i][j]);
            et_printf("[dbg] H%d   tenc_b(dut) : tenc%u[%u/%u] = 0x%lx (addr: %lx)\n", hid, i, j / 2, j / 2 + 1, cmp_tenc_b[i][j], &cmp_tenc_b[i][j]);
            C_TEST_FAIL
            return NOT_PASS;
         }
      }
   }
   return PASS;
}

void do_tima8a32(uint64_t hid, uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf, uint64_t tl_tenb_csr, uint64_t* tenc_ptr) {
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint8_t);
   uint8_t (*sec_l1scp)[elems_per_scp_line] = (uint8_t (*)[elems_per_scp_line]) pre_l1scp;

   const uint64_t usemsk  = (sm_get_bits(csr_enc, 63, 63) == 1);
   const uint64_t bcols   = (sm_get_bits(csr_enc, 56, 55) + 1) * 4;
   const uint64_t arows   =  sm_get_bits(csr_enc, 54, 51) + 1;
   const uint64_t acols   = (sm_get_bits(csr_enc, 50, 47) + 1) * 4;
   const uint64_t offset  =  sm_get_bits(csr_enc, 46, 43) * 4; // first column inside A row
   const uint64_t tenc2rf = (sm_get_bits(csr_enc, 23, 23) == 1);
   const uint64_t bsgnd   = (sm_get_bits(csr_enc, 22, 22) == 0);
   const uint64_t asgnd   = (sm_get_bits(csr_enc, 21, 21) == 0);
   const uint64_t l1scpb  = (sm_get_bits(csr_enc, 20, 20) == 0);
   const uint64_t bstart  =  sm_get_bits(csr_enc, 17, 12); // start of B data in L1 scratchpad
   const uint64_t astart  =  sm_get_bits(csr_enc,  9,  4); // start of A data in L1 scratchpad
   const uint64_t first_pass = sm_get_bits(csr_enc, 0, 0);

   // TensorLoad TenB info
   const uint64_t vaddrB  = sm_get_bits(tl_tenb_csr, 47, 6) * 64;
   const uint64_t countB  = sm_get_bits(tl_tenb_csr,  3, 0) + 1;
   const uint64_t strideB = sm_get_bits(x31, 47, 6) * 64;

   et_printf("[dbg] H%d Start TensorIMA8A32 with tm: %d, aoffset: %d, first_pass: %d, bcols: %d, acols: %d, arows: %d, "
             "ub: %d, ua: %d, tenc2rf: %d, tenb: %d, bstart: %d, astart: %d\n",
             hid, usemsk, offset, first_pass, bcols, acols, arows, !bsgnd, !asgnd, tenc2rf, !l1scpb, bstart, astart);
   et_printf("[dbg] H%d tensor_fma = 0x%lx\n", hid, csr_enc);
   et_printf("[dbg] H%d tensor_mask : 0x%lx\n", hid, tensor_mask);

   // VPU RF
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   int32_t (*tenc)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) tenc_ptr;

   for (int k = 0; k < acols; k += 4) {
      const int pass = k / 4;
      uint8_t* tmp = l1scpb ? sec_l1scp[(bstart+(k/4))%48] : (uint8_t *) (vaddrB + ((k/4)*strideB));
      const bool write_vpurf = tenc2rf && (k+4 == acols);
      const bool first_iter = (k == 0);
      const bool first_iter_first_pass = first_iter && first_pass;

      for (int i = 0; i < arows; i++) {
         if (usemsk && (sm_get_bits(tensor_mask, i, i) == 0)) {
            if (write_vpurf) {
               for (int j = 0; j < bcols; ++j) {
                  vpurf[i*2+j/8][j%8] = (first_iter_first_pass) ? 0 : tenc[i*2+j/8][j%8];
                  et_printf("[dbg] H%d TensorIMA8A32(%d) f%u[%u] = 0x%x\n", hid, pass, i*2+j/8, j%8, vpurf[i*2+j/8][j%8]);
               }
            }
            else if (first_iter_first_pass) {
               for (int j = 0; j < bcols; ++j) {
                  tenc[i*2+j/8][j%8] = 0;
                  et_printf("[dbg] H%d TensorIMA8A32(%d) TenC%u[%u] = 0x%x\n", hid, pass, i*2+j/8, j%8, tenc[i*2+j/8][j%8]);
               }
            }
            continue;
         }

         const uint8_t ta1 = sec_l1scp[(astart+i)%L1SCP_NUM_LINES][(offset+k+0)%L1SCP_LINE_SIZE_BYTES];
         const uint8_t ta2 = sec_l1scp[(astart+i)%L1SCP_NUM_LINES][(offset+k+1)%L1SCP_LINE_SIZE_BYTES];
         const uint8_t ta3 = sec_l1scp[(astart+i)%L1SCP_NUM_LINES][(offset+k+2)%L1SCP_LINE_SIZE_BYTES];
         const uint8_t ta4 = sec_l1scp[(astart+i)%L1SCP_NUM_LINES][(offset+k+3)%L1SCP_LINE_SIZE_BYTES];
         const int32_t a1 = asgnd ? sext(ta1) : zext(ta1);
         const int32_t a2 = asgnd ? sext(ta2) : zext(ta2);
         const int32_t a3 = asgnd ? sext(ta3) : zext(ta3);
         const int32_t a4 = asgnd ? sext(ta4) : zext(ta4);

         for (int j = 0; j < bcols; j++) {
            const int32_t c0 = tenc[i*2+j/8][j%8];

            const uint32_t tb1 = tmp[j*4+0];
            const uint32_t tb2 = tmp[j*4+1];
            const uint32_t tb3 = tmp[j*4+2];
            const uint32_t tb4 = tmp[j*4+3];
            const int32_t b1 = bsgnd ? sext(tb1) : zext(tb1);
            const int32_t b2 = bsgnd ? sext(tb2) : zext(tb2);
            const int32_t b3 = bsgnd ? sext(tb3) : zext(tb3);
            const int32_t b4 = bsgnd ? sext(tb4) : zext(tb4);

            int32_t c = c0;
            if (first_iter_first_pass) c = a1*b1 + a2*b2 + a3*b3 + a4*b4;
            else if (!tima8_skip(a1, b1, a2, b2, a3, b3, a4, b4)) c = c + a1*b1 + a2*b2 + a3*b3 + a4*b4;
            if (write_vpurf) {
               vpurf[i*2+j/8][j%8] = c;
               et_printf("[dbg] H%d TensorIMA8A32(%d) f%u[%u]: 0x%x = 0x%x + (0x%x * 0x%x) + (0x%x * 0x%x) + (0x%x * 0x%x) + (0x%x * 0x%x)\n", hid, pass, i*2+j/8,
                         j%8, c, c0, (uint8_t)a1, (uint8_t)b1, (uint8_t)a2, (uint8_t)b2, (uint8_t)a3, (uint8_t)b3, (uint8_t)a4, (uint8_t)b4);
            }
            else {
               tenc[i*2+j/8][j%8] = c;
               et_printf("[dbg] H%d TensorIMA8A32(%d) f%u[%u]: 0x%x = 0x%x + (0x%x * 0x%x) + (0x%x * 0x%x) + (0x%x * 0x%x) + (0x%x * 0x%x)\n", hid, pass, i*2+j/8,
                         j%8, c, c0, (uint8_t)a1, (uint8_t)b1, (uint8_t)a2, (uint8_t)b2, (uint8_t)a3, (uint8_t)b3, (uint8_t)a4, (uint8_t)b4);
            }
         }
      }
   }
}

void do_fma32(uint64_t hid, uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf, uint64_t tl_tenb_csr) {
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(float);
   float (*sec_l1scp)[elems_per_scp_line] = (float (*)[elems_per_scp_line]) pre_l1scp;

   const uint64_t usemsk = (sm_get_bits(csr_enc, 63, 63) == 1);
   const uint64_t bcols  = (sm_get_bits(csr_enc, 56, 55) + 1) * 4;
   const uint64_t arows  =  sm_get_bits(csr_enc, 54, 51) + 1;
   const uint64_t acols  =  sm_get_bits(csr_enc, 50, 47) + 1;
   const uint64_t offset =  sm_get_bits(csr_enc, 46, 43); // first column inside A row
   const uint64_t l1scpb = (sm_get_bits(csr_enc, 20, 20) == 0);
   const uint64_t bstart =  sm_get_bits(csr_enc, 17, 12); // start of B data in L1 scratchpad
   const uint64_t astart =  sm_get_bits(csr_enc,  9,  4); // start of A data in L1 scratchpad
   const uint64_t ignore_prev_val = sm_get_bits(csr_enc, 0, 0);

   // TensorLoad TenB info
   const uint64_t vaddrB  = sm_get_bits(tl_tenb_csr, 47, 6) * 64;
   const uint64_t countB  = sm_get_bits(tl_tenb_csr,  3, 0) + 1;
   const uint64_t strideB = sm_get_bits(x31, 47, 6) * 64;

   et_printf("[dbg] H%d Start TensorFMA32 with tm: %d, aoffset: %d, first_pass: %d, bcols: %d, acols: %d, arows: %d, tenb: %d, bstart: %d, astart: %d\n",
              hid, usemsk, offset, ignore_prev_val, bcols, acols, arows, !l1scpb, bstart, astart);
   et_printf("[dbg] H%d tensor_fma = 0x%lx\n", hid, csr_enc);
   et_printf("[dbg] H%d tensor_mask : 0x%lx\n", hid, tensor_mask);

   // VPU RF
   float (*vpurf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) pre_vpurf;

   int k = 0;
   if (ignore_prev_val) {
      // If first pass, the first iteration must be a FMUL.S instead of FMADD.S
      float* tmp = l1scpb ? sec_l1scp[(bstart+k)%48] : (float*) (vaddrB + k*strideB);
      for (int i = 0; i < arows; i++) {
         if (usemsk && (sm_get_bits(tensor_mask, i, i) == 0)) {
            // Even if the mask is not set, first-pass sets the register to 0
            for (int j = 0; j < bcols; j++) {
               vpurf[i*2+j/8][j%8] = 0;
               et_printf("[dbg] H%d TensorFMA32(%d) f%u[%u] = 0x%x\n", hid, k, i*2+j/8, j%8, *(unsigned int*)&vpurf[i*2+j/8][j%8]);
            }
            continue;
         }
         const float a = sec_l1scp[(astart+i)%48][(offset+k)%16];
         for (int j = 0; j < bcols; j++ ) {
            const float b  = tmp[j];
            float c;
            FMUL_S(c, a, b)
            vpurf[i*2+j/8][j%8] = c;
            et_printf("[dbg] H%d TensorFMA32(%d) f%u[%u]: 0x%x = 0x%x * 0x%x\n", hid, k, i*2+j/8, j%8, *(unsigned int*)&c, *(unsigned int*)&a, *(unsigned int*)&b);
         }
      }
      k++;
   }

   while (k < acols) {
      float* tmp = l1scpb ? sec_l1scp[(bstart+k)%48] : (float*) (vaddrB + k*strideB);
      for (int i = 0; i < arows; i++) {
         if (usemsk && (sm_get_bits(tensor_mask, i, i) == 0)) {
            continue;
         }
         const float a = sec_l1scp[(astart+i)%48][(offset+k)%16];
         for (int j = 0; j < bcols; j++ ) {
            const float c0 = vpurf[i*2+j/8][j%8];
            const float b  = tmp[j];
            if (!tfma32_skip(*(float32_t*)&a, *(float32_t*)&b)) {
               float c;
               FMADD_S(c, c0, a, b)
               vpurf[i*2+j/8][j%8] = c;
               et_printf("[dbg] H%d TensorFMA32(%d) f%u[%u]: 0x%x = 0x%x + 0x%x * 0x%x\n", hid, k, i*2+j/8, j%8, *(unsigned int*)&c, *(unsigned int*)&c0, *(unsigned int*)&a, *(unsigned int*)&b);
            }
         }
      }
      k++;
   }
}

void do_fma16a32(uint64_t hid, uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf,
                 uint64_t tl_tenb_csr) {
   // Define local data types
   static constexpr auto f16_per_line = L1SCP_LINE_SIZE_BYTES / sizeof(float16_t);
   using line_t = float16_t[f16_per_line];
   using freg_t = float32_t[LANES_PER_VPU_REG];

   static_assert(sizeof(line_t) == L1SCP_LINE_SIZE_BYTES);
   static_assert(sizeof(freg_t) == 4 * LANES_PER_VPU_REG);

   // Helper functions
   const auto xs = [csr_enc](int hi, int lo) { return sm_get_bits(csr_enc, hi, lo); };
   const auto tenb = [tl_tenb_csr](int hi, int lo) { return sm_get_bits(tl_tenb_csr, hi, lo); };
   const auto tmask = [tensor_mask](int i) { return tensor_mask & (1 << i); };

   // Parse CSR encoding
   const bool usemsk     = (xs(63, 63) == 1);
   const int  bcols      = (xs(56, 55) + 1) * 4;
   const int  arows      =  xs(54, 51) + 1;
   const int  acols      = (xs(50, 47) + 1) * 2;
   const int  offset     =  xs(46, 43) * 2; // first column inside A row
   const bool l1scpb     = (xs(20, 20) == 0);
   const int  bstart     =  xs(17, 12); // start of B data in L1 scratchpad
   const int  astart     =  xs( 9,  4); // start of A data in L1 scratchpad
   const bool first_pass =  xs( 0,  0);

   // Paired TensorLoadSetupB
   const uint64_t vaddrB  = tenb(47, 6) * 64;
   const int countB  = tenb(3, 0) + 1;
   const int strideB = sm_get_bits(x31, 47, 6) * 64;

   softfloat_roundingMode = 1; // == rtz

   et_printf("[dbg] H%d   tensor_fma = 0x%lx\n", hid, csr_enc);
   et_printf("[dbg] H%d Execute TensorFMA16A32 with tm: %d, aoffset: %d, first_pass: %d, bcols: %d, acols: %d, arows: %d, "
             "tenb: %d, bstart: %d, astart: %d\n",
             hid, usemsk, offset, first_pass, bcols, acols, arows, !l1scpb, bstart, astart);
   et_printf("[dbg] H%d   tensor_mask : 0x%lx\n", hid, tensor_mask);

   // Cast to correct data types
   freg_t* vpurf = (freg_t*)pre_vpurf;
   const line_t* l1scp = (line_t*)pre_l1scp;

   if (first_pass) {
      for (int i = 0; i < arows; ++i) {
         for (int j = 0; j < bcols; ++j) {
            const int freg = i * 2 + j / 8;
            const int lane = j % 8;
            et_printf("[dbg] H%d TensorFMA16A32(0) f%u[%u] = 0x0\n", hid, freg, lane);
            vpurf[freg][lane].v = 0;
         }
      }
   }

   for (int k = 0; k < acols; k += 2) {
      const int pass = k / 2;
      const int line_b = (bstart + pass) % L1SCP_NUM_LINES;
      const uintptr_t mem = vaddrB + pass * strideB;
      const float16_t* tmp = l1scpb ? l1scp[line_b] : (const float16_t*)mem;

      for (int i = 0; i < arows; ++i) {
         if (usemsk && !tmask(i)) continue;

         const int line_a = (astart + i) % L1SCP_NUM_LINES;
         const float16_t a1 = l1scp[line_a][(offset + k + 0) % f16_per_line];
         const float16_t a2 = l1scp[line_a][(offset + k + 1) % f16_per_line];

         for (int j = 0; j < bcols; ++j) {
            const float16_t b1 = tmp[j * 2 + 0];
            const float16_t b2 = tmp[j * 2 + 1];

            const int freg = i * 2 + j / 8;
            const int lane = j % 8;

            if (k == 0 && first_pass) {
               const float32_t c = f1632_mulAdd2(a1, b1, a2, b2);
               et_printf("[dbg] H%d TensorFMA16A32(%d) f%u[%u]: 0x%x = (0x%x * 0x%x) + (0x%x * 0x%x)\n", hid, pass, freg, lane,
                         c.v, a1.v, b1.v, a2.v, b2.v);
               vpurf[freg][lane] = c;
            }
            else {
               if (!tfma16_skip(a1, b1, a2, b2)){
                  const float32_t c0 = vpurf[freg][lane];
                  const float32_t c = f1632_mulAdd3(a1, b1, a2, b2, c0);
                  et_printf("[dbg] H%d TensorFMA16A32(%d) f%u[%u]: 0x%x = 0x%x + (0x%x * 0x%x) + (0x%x * 0x%x)\n", hid, pass, freg,
                            lane, c.v, c0.v, a1.v, b1.v, a2.v, b2.v);
                  vpurf[freg][lane] = c;
               }
            }
         }
      }
   }
}

int execute_tfma(uint64_t hid, uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf, uint64_t tl_tenb_csr, uint64_t* tenc_ptr) {
   const uint64_t config = sm_get_bits(csr_enc, 3, 1);
   const uint64_t tenb = sm_get_bits(csr_enc, 20, 20);
   if (tenb == 1) {
      const uint64_t tfma_bits = sm_get_bits(csr_enc, 50, 47);
      const uint64_t tl_bits = sm_get_bits(tl_tenb_csr, 3, 0);
      if (tfma_bits != tl_bits) return SKIP;
   }
   switch (config) {
      case TFMA32:
         do_fma32(hid, csr_enc, x31, tensor_mask, pre_l1scp, pre_vpurf, tl_tenb_csr);
         return COMPARE;
      case TFMA16A32:
         do_fma16a32(hid, csr_enc, x31, tensor_mask, pre_l1scp, pre_vpurf, tl_tenb_csr);
         return COMPARE;
      case TIMA8A32:
         do_tima8a32(hid, csr_enc, x31, tensor_mask, pre_l1scp, pre_vpurf, tl_tenb_csr, tenc_ptr);
         return COMPARE;
      default:
         return SKIP;
   }
}

int tfma_selfchecking(tensor_data* snp_info) {
   uint64_t csr_enc, x31_val, tensor_mask, tl_tenb_csr;
   uint64_t *pre_data;
   uint64_t *pre_vpurf;
   uint64_t *post_data;
   uint64_t *pre_tenc;
   uint64_t *post_tenc;
   uint64_t minion_id;
   uint64_t hart_id;

   csr_enc     = snp_info->csr_enc;
   tensor_mask = snp_info->tensor_mask;
   x31_val     = snp_info->x31_val;
   pre_data    = (uint64_t *) snp_info->pre_data_ptr;  //L1SCP
   pre_vpurf   = (uint64_t *) snp_info->pre_vpurf_ptr; //VPU RF before tensor op
   post_data   = (uint64_t *) snp_info->post_data_ptr; //VPU RF after tensor op
   pre_tenc    = (uint64_t *) snp_info->pre_tenc_ptr;  //VPU TenC before tensor op
   post_tenc   = (uint64_t *) snp_info->post_tenc_ptr; //VPU TenC after tensor op
   tl_tenb_csr = snp_info->tl_tenb_csr;
   minion_id   = snp_info->minion_id;
   hart_id   = minion_id << 1;

   int action = execute_tfma(hart_id, csr_enc, x31_val, tensor_mask, pre_data, pre_vpurf, tl_tenb_csr, pre_tenc);
   if (action == COMPARE) {
      int vpu_rf = compare_vpurf(hart_id, pre_vpurf, post_data);
      if (vpu_rf == PASS) {
         return compare_tenc(hart_id, pre_tenc, post_tenc);
      }
      else return NOT_PASS;
   }
   else return PASS;
}

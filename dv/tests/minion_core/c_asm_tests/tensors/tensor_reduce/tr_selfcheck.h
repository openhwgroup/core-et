/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define pwrtwo(x) (1 << (x))
#define LANES_PER_VPU_REG 8
#define VPURF_NUM_REGS 32
#define BYTES_PER_LANE 4

#define PASS 0
#define FAIL 1

#define DEST_L1  0
#define DEST_L2  1
#define DEST_L3  2
#define DEST_MEM 3

#define TR_SND   0b00
#define TR_RCV   0b01
#define TR_BCAST 0b10
#define TR_AUTO  0b11

#define BLOB_SIZE 1048576
#define TR_BLOB_SIZE (BLOB_SIZE/2)
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

void wait_for_everyone() {
   give_credit_local(-1, -1, 0);

   // wait for all other harts to be done consuming the shared counter, plus the credit we just sent to ourselves
   for (uint32_t j = 0; j < NUM_MINIONS; ++j) {
      wait_for_credit(0);
   }
}

void evict_snd_data(uint64_t* snd_rf_ptr, uint64_t fstart, uint64_t fnum) {
   uint64_t i;
   float * freg;
   if (fnum > 0) {
      float (*snd_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) snd_rf_ptr;
      for (i = 0; i < fnum; i++) {
         freg = snd_rf[(fstart+i)%VPURF_NUM_REGS];
         //       use_tmask,     dst,                addr, num_lines, stride, id, warl)
         evict_va(        0, DEST_L2, (uint64_t) &freg[0],        15,     64,  0,    0);
      }
      uint64_t last_evict_ba = (uint64_t) &freg[0];
      if ((last_evict_ba & 0x3f) > 32) {
         //       use_tmask,     dst,                 addr, num_lines, stride, id, warl)
         evict_va(        0, DEST_L2, (last_evict_ba + 32),        15,     64,  0,    0);
      }
      WAIT_CACHEOPS
   }
}

uint64_t get_ptr(uint64_t mid) {
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t idx = (mid*mid) % (TR_BLOB_SIZE/sizeof(cl_data_t));
   uint64_t ptr = (uint64_t)(void*)buffer[idx].data[0];
   return ptr;
}

void dump_vpu_rf(uint32_t* addr) {
   __asm__ __volatile__ (
      "fsq2  f0,   0(%[base_addr])\n"
      "fsq2  f1,  32(%[base_addr])\n"
      "fsq2  f2,  64(%[base_addr])\n"
      "fsq2  f3,  96(%[base_addr])\n"
      "fsq2  f4, 128(%[base_addr])\n"
      "fsq2  f5, 160(%[base_addr])\n"
      "fsq2  f6, 192(%[base_addr])\n"
      "fsq2  f7, 224(%[base_addr])\n"
      "fsq2  f8, 256(%[base_addr])\n"
      "fsq2  f9, 288(%[base_addr])\n"
      "fsq2 f10, 320(%[base_addr])\n"
      "fsq2 f11, 352(%[base_addr])\n"
      "fsq2 f12, 384(%[base_addr])\n"
      "fsq2 f13, 416(%[base_addr])\n"
      "fsq2 f14, 448(%[base_addr])\n"
      "fsq2 f15, 480(%[base_addr])\n"
      "fsq2 f16, 512(%[base_addr])\n"
      "fsq2 f17, 544(%[base_addr])\n"
      "fsq2 f18, 576(%[base_addr])\n"
      "fsq2 f19, 608(%[base_addr])\n"
      "fsq2 f20, 640(%[base_addr])\n"
      "fsq2 f21, 672(%[base_addr])\n"
      "fsq2 f22, 704(%[base_addr])\n"
      "fsq2 f23, 736(%[base_addr])\n"
      "fsq2 f24, 768(%[base_addr])\n"
      "fsq2 f25, 800(%[base_addr])\n"
      "fsq2 f26, 832(%[base_addr])\n"
      "fsq2 f27, 864(%[base_addr])\n"
      "fsq2 f28, 896(%[base_addr])\n"
      "fsq2 f29, 928(%[base_addr])\n"
      "fsq2 f30, 960(%[base_addr])\n"
      "fsq2 f31, 992(%[base_addr])\n"
      :
      : [base_addr] "r" (addr)
      :
   );
   const int vpu_rf_size = 1024; // Bytes
   const int cache_line_size = 64; // Bytes
   const int num_lines = vpu_rf_size/cache_line_size;
   // Flush 16 lines
   //       use_tmask,     dst,            addr,   num_lines, stride, id, warl)
   flush_va(        0, DEST_L2, (uint64_t) addr, num_lines-1,     64,  0,    0);
   if ((((uint64_t) addr) & 0x3f) != 0) {
      // Flush last line if it is not aligned to 64Bytes
      //       use_tmask,     dst,                  addr,   num_lines, stride, id, warl)
      flush_va(        0, DEST_L2, ((uint64_t) addr)+992,           0,     64,  0,    0);
   }
   WAIT_CACHEOPS
}

int compare_vpurf(uint64_t* vpurf_a, uint64_t* vpurf_b) {
   const int SIZE = LANES_PER_VPU_REG / (sizeof(uint64_t)/BYTES_PER_LANE);
   uint64_t (*cmp_vpurf_a)[SIZE] = (uint64_t (*)[SIZE]) vpurf_a;
   uint64_t (*cmp_vpurf_b)[SIZE] = (uint64_t (*)[SIZE]) vpurf_b;
   for (int i = 0; i < VPURF_NUM_REGS; i++) {
      for (int j = 0; j < SIZE; j++) {
         if (cmp_vpurf_a[i][j] != cmp_vpurf_b[i][j]) {
            C_TEST_FAIL
            return FAIL;
         }
      }
   }
   return PASS;
}

inline float tr_fmax(float a, float b) {
   float ret;
   __asm__ volatile ( "fmax.s %[rd], %[rs1], %[rs2]\n" \
                      :  [rd] "=f" (ret)  \
                      : [rs1]  "f" (a),   \
                        [rs2]  "f" (b)    \
                      : /* No clobbers */ \
                    );
   return ret;
}

inline float tr_fmin(float a, float b) {
   float ret;
   __asm__ volatile ( "fmin.s %[rd], %[rs1], %[rs2]\n" \
                      :  [rd] "=f" (ret)  \
                      : [rs1]  "f" (a),   \
                        [rs2]  "f" (b)    \
                      : /* No clobbers */ \
                    );
   return ret;
}

//                                                                             xs[61:57],   xs[27:24],     xs[22:16]
void run_tensor_reduce_fp_op(uint64_t* snd_rf_ptr, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {
   float (*snd_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) snd_rf_ptr;
   float (*rcv_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) rcv_rf_ptr; // rcv_data that it is safe to modify

   for (uint64_t i = 0; i < fnum; i++) {
      float* freg = snd_rf[(fstart+i)%VPURF_NUM_REGS];
      uint64_t k = (fstart + i) % VPURF_NUM_REGS;
      for (uint64_t j = 0; j < LANES_PER_VPU_REG; j++) {
         switch (op) {
            case FADD:
               rcv_rf[k][j] = freg[j] + rcv_rf[k][j];
               break;
            case FMAX:
               rcv_rf[k][j] = tr_fmax(rcv_rf[k][j], freg[j]);
               break;
            case FMIN:
               rcv_rf[k][j] = tr_fmin(rcv_rf[k][j], freg[j]);
               break;
            case FGET:
               rcv_rf[k][j] = freg[j];
               break;
         }
      }
   }
}

//                                                                              xs[61:57],   xs[27:24],     xs[22:16]
void run_tensor_reduce_int_op(uint64_t* snd_rf_ptr, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {
   int32_t (*snd_rf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) snd_rf_ptr;
   int32_t (*rcv_rf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) rcv_rf_ptr; // rcv_data that it is safe to modify

   for (uint64_t i = 0; i < fnum; i++) {
      int32_t* freg = snd_rf[(fstart+i)%VPURF_NUM_REGS];
      uint64_t k = (fstart + i) % VPURF_NUM_REGS;
      for (uint64_t j = 0; j < LANES_PER_VPU_REG; j++) {
         switch (op) {
            case IADD:
               rcv_rf[k][j] = freg[j] + rcv_rf[k][j];
               break;
            case IMAX:
               rcv_rf[k][j] = (freg[j] > rcv_rf[k][j]) ? freg[j] : rcv_rf[k][j];
               break;
            case IMIN:
               rcv_rf[k][j] = (freg[j] < rcv_rf[k][j]) ? freg[j] : rcv_rf[k][j];
               break;
         }
      }
   }
}


//                                                                                                       xs[61:57],   xs[27:24],     xs[22:16]
int run_tensor_reduce_model_base(uint64_t* rtl_rf_ptr, uint64_t* snd_rf_ptr, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {

   // Create a copy of the Receiver VPU RF content to not override the original values
   // VPU Lanes granularity are 32 bits, but to speedup the copy, we cast it to 64 bits
   const int j_size = (LANES_PER_VPU_REG * BYTES_PER_LANE) / sizeof(uint64_t);
   uint64_t scratch[VPURF_NUM_REGS][j_size];
   uint64_t (*tmp_ptr)[j_size] = (uint64_t (*)[j_size]) rcv_rf_ptr;
   for (int i = 0; i < VPURF_NUM_REGS; i++) {
      for (int j = 0; j < j_size; j++) {
         scratch[i][j] = tmp_ptr[i][j];
      }
   }

   if ((op == FMAX) || (op == FMIN) || (op == FADD) || (op == FGET)) {
      // Result is stored in 'scratch'
      run_tensor_reduce_fp_op(snd_rf_ptr, (uint64_t*) scratch, fstart, op, fnum);
      evict_snd_data(snd_rf_ptr, fstart, fnum);
   }
   else if ((op == IMAX) || (op == IMIN) || (op == IADD)) {
      // Result is stored in 'scratch'
      run_tensor_reduce_int_op(snd_rf_ptr, (uint64_t*) scratch, fstart, op, fnum);
      evict_snd_data(snd_rf_ptr, fstart, fnum);
   }

   float (*rtl_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) rtl_rf_ptr;
   float (*rcv_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) scratch;
   return compare_vpurf((uint64_t*) &rtl_rf[0][0], (uint64_t*) &rcv_rf[0][0]);
}

//                                                                    xs[61:57],   xs[27:24],     xs[22:16]
int run_tensor_reduce_model(uint64_t snd_mid, uint64_t rcv_mid, uint64_t fstart, uint64_t op, uint64_t fnum) {
   uint64_t* snd_rf_ptr = (uint64_t*) get_ptr(snd_mid);
   uint64_t* rcv_rf_ptr = (uint64_t*) get_ptr(rcv_mid);

   // Dump out the Tensor Reduce data
   uint32_t rtl_rf[32][8]; //1024 Bytes
   dump_vpu_rf(&rtl_rf[0][0]);

   return run_tensor_reduce_model_base((uint64_t*) &rtl_rf[0][0], snd_rf_ptr, rcv_rf_ptr, fstart, op, fnum);
}

//                                        xs[61:57],   xs[27:24],     xs[22:16],        xs[6:3]
int selfcheck_tr_auto(uint64_t mid, uint64_t fstart, uint64_t op, uint64_t fnum, uint64_t depth) {
   uint64_t snd_mid, rcv_mid;
   int status = PASS;
   int eval = mid % pwrtwo(depth+1);
   if (eval == 0) {
      // this ET-Minion is a receiver
      snd_mid = mid + pwrtwo(depth);
      rcv_mid = mid;
      status = run_tensor_reduce_model(snd_mid, rcv_mid, fstart, op, fnum);
   }
   else {
      // Make sure the Sender data did not change before and after the reduce
      uint32_t rtl_rf[32][8]; //1024 Bytes
      dump_vpu_rf(&rtl_rf[0][0]);
      status = compare_vpurf((uint64_t*) &rtl_rf[0][0], (uint64_t*) get_ptr(mid));
   }
   return status;
}

//                                         xs[61:57],   xs[27:24],     xs[22:16],        xs[6:3]
int selfcheck_tr_bcast(uint64_t mid, uint64_t fstart, uint64_t op, uint64_t fnum, uint64_t depth) {
   uint64_t snd_mid, rcv_mid;
   int status = PASS;
   int eval = mid % pwrtwo(depth+1);
   if (eval == pwrtwo(depth)) {
      // this ET-Minion is a receiver
      snd_mid = mid - pwrtwo(depth);
      rcv_mid = mid;
      status = run_tensor_reduce_model(snd_mid, rcv_mid, fstart, op, fnum);
   }
   else {
      // Make sure the Sender data did not change before and after the reduce
      uint32_t rtl_rf[32][8]; //1024 Bytes
      dump_vpu_rf(&rtl_rf[0][0]);
      status = compare_vpurf((uint64_t*) &rtl_rf[0][0], (uint64_t*) get_ptr(mid));
   }
   return status;
}

//                                                  xs[61:57],          xs[27:24],        xs[22:16],                 xs[15:3],         xs[1:0]
int self_check_tensor_reduce(uint64_t mid, uint64_t start_reg, uint64_t operation, uint64_t num_reg, uint64_t partnerID_depth, uint64_t action) {
   int status = PASS;
   WAIT_TENSOR_REDUCE;
   if (action == TR_BCAST) {
      status = selfcheck_tr_bcast(mid, start_reg, operation, num_reg, partnerID_depth);
   }
   else if (action == TR_AUTO) {
      status = selfcheck_tr_auto(mid, start_reg, operation, num_reg, partnerID_depth);
   }
   else if (action == TR_RCV) {
      status = run_tensor_reduce_model(partnerID_depth, mid, start_reg, operation, num_reg);
   }
   else { // TR_SND
      // Dump out the Tensor Reduce data
      uint32_t rtl_rf[32][8]; //1024 Bytes
      dump_vpu_rf(&rtl_rf[0][0]);
      status = compare_vpurf((uint64_t*) &rtl_rf[0][0], (uint64_t*) get_ptr(mid));
   }
   return status;
}

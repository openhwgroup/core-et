/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#define L1SCP_NUM_LINES 48
#define L1SCP_LINE_SIZE_BYTES 64
#define VPURF_NUM_REGS 32
#define LANES_PER_VPU_REG 8
#define BYTES_PER_LANE 4

#define SKIP 12
#define COMPARE 27

#define PASS 0
#define NOT_PASS 1


inline uint64_t do_read_x31() {
   uint64_t x31_bak;
   __asm__ __volatile__ (
      "add %[X31_BAK], x31, x0\n"
      : [X31_BAK] "=r" (x31_bak)
      : /* no source reg */
   );
   return x31_bak;
}

inline void do_write_x31(uint64_t x31_bak) {
   __asm__ __volatile__ (
      "add x31, %[X31_BAK], x0\n"
      : /* no destination reg */
      : [X31_BAK] "r" (x31_bak)
   );
}

void dump_l1_scp(uint64_t* l1scp_bak) {
   WAIT_TENSOR_LOAD_0
   WAIT_TENSOR_LOAD_1

   uint64_t csr_enc = 0x0079000000000000 + ((uint64_t) l1scp_bak);
   uint64_t x31_bak = do_read_x31(); // Backup X31 value

   do_write_x31(0x40);

   // SCP Lines [0..15]
   __asm__ __volatile__ ( "csrrw x0, tensor_store, %[CSR_ENC]\n" : : [CSR_ENC] "r" (csr_enc));

   // SCP Lines [16..31]
   csr_enc += (0x1000000000000000 + (1024));
   __asm__ __volatile__ ( "csrrw x0, tensor_store, %[CSR_ENC]\n" : : [CSR_ENC] "r" (csr_enc));

   // SCP Lines [32..47]
   csr_enc += (0x1000000000000000 + (1024));
   __asm__ __volatile__ ( "csrrw x0, tensor_store, %[CSR_ENC]\n" : : [CSR_ENC] "r" (csr_enc));

   do_write_x31(x31_bak); // Restore X31 value

   WAIT_TENSOR_STORE
}

void dump_vpu_rf(uint64_t* vpu_rf_bak) {

   uint64_t addr = (uint64_t) vpu_rf_bak;

   WAIT_TENSOR_FMA
   WAIT_TENSOR_STORE
   WAIT_TENSOR_REDUCE
   WAIT_TENSOR_QUANT

   __asm__ __volatile__ (
      "fsq2  f0,   0(%[ADDR])\n"
      "fsq2  f1,  32(%[ADDR])\n"
      "fsq2  f2,  64(%[ADDR])\n"
      "fsq2  f3,  96(%[ADDR])\n"
      "fsq2  f4, 128(%[ADDR])\n"
      "fsq2  f5, 160(%[ADDR])\n"
      "fsq2  f6, 192(%[ADDR])\n"
      "fsq2  f7, 224(%[ADDR])\n"
      "fsq2  f8, 256(%[ADDR])\n"
      "fsq2  f9, 288(%[ADDR])\n"
      "fsq2 f10, 320(%[ADDR])\n"
      "fsq2 f11, 352(%[ADDR])\n"
      "fsq2 f12, 384(%[ADDR])\n"
      "fsq2 f13, 416(%[ADDR])\n"
      "fsq2 f14, 448(%[ADDR])\n"
      "fsq2 f15, 480(%[ADDR])\n"
      "fsq2 f16, 512(%[ADDR])\n"
      "fsq2 f17, 544(%[ADDR])\n"
      "fsq2 f18, 576(%[ADDR])\n"
      "fsq2 f19, 608(%[ADDR])\n"
      "fsq2 f20, 640(%[ADDR])\n"
      "fsq2 f21, 672(%[ADDR])\n"
      "fsq2 f22, 704(%[ADDR])\n"
      "fsq2 f23, 736(%[ADDR])\n"
      "fsq2 f24, 768(%[ADDR])\n"
      "fsq2 f25, 800(%[ADDR])\n"
      "fsq2 f26, 832(%[ADDR])\n"
      "fsq2 f27, 864(%[ADDR])\n"
      "fsq2 f28, 896(%[ADDR])\n"
      "fsq2 f29, 928(%[ADDR])\n"
      "fsq2 f30, 960(%[ADDR])\n"
      "fsq2 f31, 992(%[ADDR])\n"
      : /* no dest reg */
      : [ADDR] "r" (addr)
   );
}


int compare_vpurf(uint64_t* vpurf_a, uint64_t* vpurf_b) {
   const int SIZE = LANES_PER_VPU_REG / (sizeof(uint64_t)/BYTES_PER_LANE);
   uint64_t (*cmp_vpurf_a)[SIZE] = (uint64_t (*)[SIZE]) vpurf_a;
   uint64_t (*cmp_vpurf_b)[SIZE] = (uint64_t (*)[SIZE]) vpurf_b;
   for (int i = 0; i < VPURF_NUM_REGS; i++) {
      for (int j = 0; j < SIZE; j++) {
         if (cmp_vpurf_a[i][j] != cmp_vpurf_b[i][j]) {
            C_TEST_FAIL
            return NOT_PASS;
         }
      }
   }
   return PASS;
}

inline uint_fast64_t sm_get_bits(uint64_t value, int head, int tail) {
   // we want to extract k-bits
   uint_fast64_t k = (head - tail) + 1;
   uint_fast64_t mask = 1;
   mask = mask << k;
   mask = mask - 1;
   uint_fast64_t sh_val = value;
   sh_val = sh_val  >> tail;
   return (mask & sh_val);
}

uint64_t* get_aligned_addr(uint64_t* na_addr) {
   uint64_t ret = (uint64_t) na_addr;
   ret += 64;
   ret = ret & 0xffffffffffffffc0;
   return (uint64_t*) ret;
}


#include "tensor_quant.h"

void do_int32tofloat32(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   int32_t (*src_vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   float   (*dst_vpurf)[LANES_PER_VPU_REG] = (float   (*)[LANES_PER_VPU_REG]) pre_vpurf;

   const auto int32tofloat32 = [](int32_t a) -> float {
      float c;
      __asm__ __volatile__ (
         "fcvt.s.w %[fd], %[rs1], dyn\n"
         : [fd]  "=f" (c)
         : [rs1] "r"  (a)
         :
      );
      return c;
   };

   #define TRANS_OP(A) int32tofloat32((A))
   TQUANT_LOOP(hid, rows, cols, fstart, dst_vpurf, src_vpurf, TRANS_OP)
   #undef TRANS_OP
}

void do_float32toint32(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   float   (*src_vpurf)[LANES_PER_VPU_REG] = (float   (*)[LANES_PER_VPU_REG]) pre_vpurf;
   int32_t (*dst_vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;

   const auto float32toint32 = [](float a) -> int32_t {
      int32_t c;
      __asm__ __volatile__ (
         "fcvt.w.s %[rd], %[fs1], dyn\n"
         : [rd]  "=r" (c)
         : [fs1] "f"  (a)
         :
      );
      return c;
   };

   #define TRANS_OP(A) float32toint32((A))
   TQUANT_LOOP(hid, rows, cols, fstart, dst_vpurf, src_vpurf, TRANS_OP)
   #undef TRANS_OP
}

void do_relu(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   #define TRANS_OP(A) ((A) > 0) ? (A) : 0;
   TQUANT_LOOP(hid, rows, cols, fstart, vpurf, vpurf, TRANS_OP)
   #undef TRANS_OP
}

void do_int32_add_row(uint64_t hid, uint64_t *pre_vpurf, uint64_t *pre_l1scp, uint64_t rows, uint64_t cols, uint64_t fstart, uint64_t &line) {
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(int32_t);
   int32_t (*l1scp)[elems_per_scp_line] = (int32_t (*)[elems_per_scp_line]) pre_l1scp;
   #define TRANS_OP(A, B) (A) + (B)
   TQUANT_ROWS_LOOP(hid, rows, cols, fstart, line, vpurf, l1scp, TRANS_OP)
   #undef TRANS_OP
   line = (line + 1) % 48;
}

void do_int32_add_col(uint64_t hid, uint64_t *pre_vpurf, uint64_t *pre_l1scp, uint64_t rows, uint64_t cols, uint64_t fstart, uint64_t &line) {
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(int32_t);
   int32_t (*l1scp)[elems_per_scp_line] = (int32_t (*)[elems_per_scp_line]) pre_l1scp;
   #define TRANS_OP(A, B) (A) + (B)
   TQUANT_COLS_LOOP(hid, rows, cols, fstart, line, vpurf, l1scp, TRANS_OP)
   #undef TRANS_OP
   line = (line + 1) % 48;
}

void do_fp32_mul_row(uint64_t hid, uint64_t *pre_vpurf, uint64_t *pre_l1scp, uint64_t rows, uint64_t cols, uint64_t fstart, uint64_t &line) {
   float (*vpurf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) pre_vpurf;
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(float);
   float (*l1scp)[elems_per_scp_line] = (float (*)[elems_per_scp_line]) pre_l1scp;
   #define TRANS_OP(A, B) (A) * (B)
   TQUANT_ROWS_LOOP(hid, rows, cols, fstart, line, vpurf, l1scp, TRANS_OP)
   #undef TRANS_OP
   line = (line + 1) % 48;
}

void do_fp32_mul_col(uint64_t hid, uint64_t *pre_vpurf, uint64_t *pre_l1scp, uint64_t rows, uint64_t cols, uint64_t fstart, uint64_t &line) {
   float (*vpurf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) pre_vpurf;
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(float);
   float (*l1scp)[elems_per_scp_line] = (float (*)[elems_per_scp_line]) pre_l1scp;
   #define TRANS_OP(A, B) (A) * (B)
   TQUANT_COLS_LOOP(hid, rows, cols, fstart, line, vpurf, l1scp, TRANS_OP)
   #undef TRANS_OP
   line = (line + 1) % 48;
}

void do_satint8(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   #define TRANS_OP(A) (int32_t) ((MAX(-128, MIN(127, A))) & 0xFF)
   TQUANT_LOOP(hid, rows, cols, fstart, vpurf, vpurf, TRANS_OP)
   #undef TRANS_OP
}

void do_satuint8(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   int32_t (*vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   #define TRANS_OP(A) (uint32_t) ((MAX(0, MIN(255, A))) & 0xFF)
   TQUANT_LOOP(hid, rows, cols, fstart, vpurf, vpurf, TRANS_OP)
   #undef TRANS_OP
}

void do_pack128b(uint64_t hid, uint64_t *pre_vpurf, uint64_t rows, uint64_t cols, uint64_t fstart) {
   int32_t (*src_vpurf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) pre_vpurf;
   const uint64_t pack128b_size = LANES_PER_VPU_REG*(BYTES_PER_LANE/sizeof(uint8_t));
   uint8_t (*dst_vpurf)[pack128b_size] = (uint8_t (*)[pack128b_size]) pre_vpurf;

   for (int i = 0; i < rows; i++) {
      const uint64_t dst_reg = (fstart + i*2) % 32;
      for (int j = 0; j < cols; j+=8) {
         uint32_t reg = (fstart + (i*2) + (j/8)) % 32;
         uint32_t nlanes = (j+8) > cols ? cols - j : 8;
         /*print_vpurf_fs1(hid, (uint64_t*)pre_vpurf, dst_reg);*/
         for (int lane = 0; lane < nlanes; lane++) {
            dst_vpurf[dst_reg][j + lane] = src_vpurf[reg][lane];
         }
         /*print_vpurf_fd(hid, (uint64_t*)pre_vpurf, dst_reg);*/
      }
   }
}

void do_tquant(uint64_t csr_enc, uint64_t hid, uint64_t *pre_l1scp, uint64_t *pre_vpurf) {
   uint64_t fstart =  sm_get_bits(csr_enc, 61, 57);
   uint64_t cols   = (sm_get_bits(csr_enc, 56, 55) + 1) * 4;
   uint64_t rows   =  sm_get_bits(csr_enc, 54, 51) + 1;
   uint64_t line   =  sm_get_bits(csr_enc, 50, 45) % 48;

   int k = 0;
   bool last = 0;
   while ((k < 10) && !last) {
      unsigned int trans = sm_get_bits(csr_enc, ((4*k)+3), (4*k));
      switch (trans) {
         case LAST:
            last = 1;
            break;
         case INT32_TO_FP32:
            et_printf("[dbg] H%d Transformation %d: INT32_TO_FP32\n", hid, k);
            do_int32tofloat32(hid, pre_vpurf, rows, cols, fstart);
            break;
         case FP32_TO_INT32:
            et_printf("[dbg] H%d Transformation %d: FP32_TO_INT32\n", hid, k);
            do_float32toint32(hid, pre_vpurf, rows, cols, fstart);
            break;
         case RELU:
            et_printf("[dbg] H%d Transformation %d: RELU\n", hid, k);
            do_relu(hid, pre_vpurf, rows, cols, fstart);
            break;
         case INT32_ADD_ROW:
            et_printf("[dbg] H%d Transformation %d: INT32_ADD_ROW\n", hid, k);
            do_int32_add_row(hid, pre_vpurf, pre_l1scp, rows, cols, fstart, line);
            break;
         case INT32_ADD_COL:
            et_printf("[dbg] H%d Transformation %d: INT32_ADD_COL\n", hid, k);
            do_int32_add_col(hid, pre_vpurf, pre_l1scp, rows, cols, fstart, line);
            break;
         case FP32_MUL_ROW:
            et_printf("[dbg] H%d Transformation %d: INT32_MUL_ROW\n", hid, k);
            do_fp32_mul_row(hid, pre_vpurf, pre_l1scp, rows, cols, fstart, line);
            break;
         case FP32_MUL_COL:
            et_printf("[dbg] H%d Transformation %d: INT32_MUL_COL\n", hid, k);
            do_fp32_mul_col(hid, pre_vpurf, pre_l1scp, rows, cols, fstart, line);
            break;
         case SATINT8:
            et_printf("[dbg] H%d Transformation %d: SATINT8\n", hid, k);
            do_satint8(hid, pre_vpurf, rows, cols, fstart);
            break;
         case SATUINT8:
            et_printf("[dbg] H%d Transformation %d: SATUINT8\n", hid, k);
            do_satuint8(hid, pre_vpurf, rows, cols, fstart);
            break;
         case PACK_128B:
            et_printf("[dbg] H%d Transformation %d: PACK_128B\n", hid, k);
            do_pack128b(hid, pre_vpurf, rows, cols, fstart);
            break;
      }
      k++;
   }

}

int tquant_selfchecking(tensor_data* snp_info) {
   uint64_t csr_enc;
   uint64_t *pre_data;
   uint64_t *pre_vpurf;
   uint64_t *post_data;
   uint64_t minion_id;
   uint64_t hart_id;

   csr_enc = snp_info->csr_enc;
   pre_data = (uint64_t*) snp_info->pre_data_ptr;
   pre_vpurf = (uint64_t*) snp_info->pre_vpurf_ptr;
   post_data = (uint64_t*) snp_info->post_data_ptr;

   minion_id = snp_info->minion_id;
   hart_id = minion_id << 1;

   et_printf("[dbg] H%d csr_enc: 0x%lx\n", hart_id, csr_enc);
   et_printf("[dbg] H%d @l1_scp: 0x%lx\n", hart_id, (uint64_t) pre_data);
   et_printf("[dbg] H%d @pre_vpurf: 0x%lx\n", hart_id, (uint64_t) pre_vpurf);
   et_printf("[dbg] H%d @post_vpurf: 0x%lx\n", hart_id, (uint64_t) post_data);

   do_tquant(csr_enc, hart_id, pre_data, pre_vpurf);

   return compare_vpurf(hart_id, pre_vpurf, post_data);
}

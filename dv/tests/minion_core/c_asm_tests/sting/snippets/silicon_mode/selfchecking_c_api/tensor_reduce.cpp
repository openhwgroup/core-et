
#include "tensor_reduce.h"

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

void do_tr_fp_op(uint64_t* snd_rf_ptr, uint64_t snd_fstart, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {
   float (*snd_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) snd_rf_ptr;
   float (*rcv_rf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) rcv_rf_ptr; // rcv_data that it is safe to modify

   for (uint64_t i = 0; i < fnum; i++) {
      float* freg = snd_rf[(snd_fstart+i)%VPURF_NUM_REGS];
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

void do_tr_int_op(uint64_t* snd_rf_ptr, uint64_t snd_fstart, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {
   int32_t (*snd_rf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) snd_rf_ptr;
   int32_t (*rcv_rf)[LANES_PER_VPU_REG] = (int32_t (*)[LANES_PER_VPU_REG]) rcv_rf_ptr; // rcv_data that it is safe to modify

   for (uint64_t i = 0; i < fnum; i++) {
      int32_t* freg = snd_rf[(snd_fstart+i)%VPURF_NUM_REGS];
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

void do_tr_model(uint64_t* snd_rf_ptr, uint64_t snd_fstart, uint64_t* rcv_rf_ptr, uint64_t fstart, uint64_t op, uint64_t fnum) {
   if ((op == FMAX) || (op == FMIN) || (op == FADD) || (op == FGET)) {
      do_tr_fp_op(snd_rf_ptr, snd_fstart, rcv_rf_ptr, fstart, op, fnum);
   }
   else if ((op == IMAX) || (op == IMIN) || (op == IADD)) {
      do_tr_int_op(snd_rf_ptr, snd_fstart, rcv_rf_ptr, fstart, op, fnum);
   }
}

void do_tr_auto(uint64_t mid, uint64_t* partner_vpurf, uint64_t partner_fstart, uint64_t* pre_vpurf, uint64_t fstart, uint64_t op, uint64_t fnum, uint64_t depth) {
   int eval = mid % pwrtwo(depth+1);
   if (eval == 0) {
      // this ET-Minion is a receiver
      do_tr_model(partner_vpurf, partner_fstart, pre_vpurf, fstart, op, fnum);
   }
}

void do_tr_bcast(uint64_t mid, uint64_t* partner_vpurf, uint64_t partner_fstart, uint64_t* pre_vpurf, uint64_t fstart, uint64_t op, uint64_t fnum, uint64_t depth) {
   int eval = mid % pwrtwo(depth+1);
   if (eval == pwrtwo(depth)) {
      // this ET-Minion is a receiver
      do_tr_model(partner_vpurf, partner_fstart, pre_vpurf, fstart, op, fnum);
   }
   // Senders do not modify the data so we will check that the VPU RF content before and after the reduce is the same
}

int execute_tensor_reduce(uint64_t mid, uint64_t csr_enc, uint64_t partner_enc, uint64_t* partner_vpurf, uint64_t* pre_vpurf) {
   uint64_t fstart = sm_get_bits(csr_enc, 61, 57);
   uint64_t op     = sm_get_bits(csr_enc, 27, 24);
   uint64_t fnum   = sm_get_bits(csr_enc, 22, 16);
   uint64_t action = sm_get_bits(csr_enc,  1,  0);
   uint64_t depth;

   uint64_t partner_fstart = sm_get_bits(partner_enc, 61, 57);

   switch (action) {
      case TR_BCAST:
         depth = sm_get_bits(csr_enc, 6, 3);
         do_tr_bcast(mid, partner_vpurf, partner_fstart, pre_vpurf, fstart, op, fnum, depth);
         return COMPARE;

      case TR_AUTO:
         depth = sm_get_bits(csr_enc, 6, 3);
         do_tr_auto(mid, partner_vpurf, partner_fstart, pre_vpurf, fstart, op, fnum, depth);
         return COMPARE;

      case TR_RCV:
         do_tr_model(partner_vpurf, partner_fstart, pre_vpurf, fstart, op, fnum);
         return COMPARE;

      case TR_SND:
         return COMPARE;

      default:
         return SKIP;
   }
}

int tr_selfchecking(tensor_data* snp_info) {

   tensor_reduce_data* tr_info = (tensor_reduce_data*) snp_info;
   tensor_reduce_data* prtnr_tr_info = (tensor_reduce_data*) tr_info->prtnr_reduce_data;

   uint64_t csr_enc, partner_enc;
   uint64_t *partner_data;
   uint64_t *pre_vpurf;
   uint64_t *post_vpurf;
   uint64_t minion_id;


   csr_enc      = tr_info->csr_enc;
   partner_enc  = prtnr_tr_info->csr_enc;
   partner_data = (uint64_t *) prtnr_tr_info->pre_vpurf_ptr;
   pre_vpurf    = (uint64_t *) tr_info->pre_vpurf_ptr;
   post_vpurf   = (uint64_t *) tr_info->post_vpurf_ptr;
   minion_id    = tr_info->minion_id;


   int action = execute_tensor_reduce(minion_id, csr_enc, partner_enc, partner_data, pre_vpurf);
   if (action == COMPARE) compare_vpurf(minion_id*2, pre_vpurf, post_vpurf);

   return PASS;
}

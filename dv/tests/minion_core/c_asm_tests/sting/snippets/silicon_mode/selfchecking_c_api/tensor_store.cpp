
#include "tensor_store.h"

int selfcheck_ts_16bytes(uint64_t hid, const uint32_t* fsrc, const uint64_t VA) {
   const uint16_t* vpu_val = (uint16_t*) fsrc;
   const uint16_t* mem_val = (uint16_t*) VA;
   if (vpu_val[0] != mem_val[0]) {
      et_printf("[dbg] H%d Self-Check failed. VPU is 0x%lx but MEM[0x%lx] is 0x%lx\n",
                 hid, vpu_val[0], VA, mem_val[0]);
      return NOT_PASS;
   }
   else return PASS;
}

int selfcheck_ts_32bytes(uint64_t hid, const uint32_t* fsrc, const uint64_t VA) {
   const uint32_t* vpu_val = fsrc;
   const uint32_t* mem_val = (uint32_t*) VA;
   if (vpu_val[0] != mem_val[0]) {
      et_printf("[dbg] H%d Self-Check failed. VPU is 0x%lx but MEM[0x%lx] is 0x%lx\n",
                 hid, vpu_val[0], VA, mem_val[0]);
      return NOT_PASS;
   }
   else return PASS;
}

int selfcheck_ts_compare(uint64_t hid, uint64_t addr, uint64_t* vpurf_ptr, uint64_t freg, const int bytes, const uint64_t srcinc) {
   // VPU RF
   uint32_t (*vpurf)[LANES_PER_VPU_REG] = (uint32_t (*)[LANES_PER_VPU_REG]) vpurf_ptr;

   int fail = 0;
   if (bytes == 16) {
      addr = addr & 0xfffffffffffffff0;
#ifdef DEBUG
      print_vpurf_fs1(hid, vpurf_ptr, freg);
      print_mem_w(hid, addr);
#endif
      fail = selfcheck_ts_16bytes(hid, vpurf[freg], addr);
   }
   else if (bytes == 32) {
      addr = addr & 0xffffffffffffffe0;
#ifdef DEBUG
      print_vpurf_fs1(hid, vpurf_ptr, freg);
      print_mem_w(hid, addr);
      print_mem_w(hid, addr+16);
#endif
      fail = selfcheck_ts_32bytes(hid, vpurf[freg], addr);
   }
   else if (bytes == 64) {
      addr = addr & 0xffffffffffffffc0;
#ifdef DEBUG
      print_vpurf_fs1(hid, vpurf_ptr, freg);
      print_mem_w(hid, addr);
      print_mem_w(hid, addr+16);
#endif
      fail = selfcheck_ts_32bytes(hid, vpurf[freg], addr);
      if (!fail) {
         addr = addr + 32,
         freg = (freg+srcinc)%32;
#ifdef DEBUG
         print_vpurf_fs1(hid, vpurf_ptr, freg);
         print_mem_w(hid, addr);
         print_mem_w(hid, addr+16);
#endif
         fail = selfcheck_ts_32bytes(hid, vpurf[freg], addr);
      }
   }
   if (fail) return NOT_PASS;
   else      return PASS;
}

int do_tstore(uint64_t csr_enc, uint64_t x31, uint64_t hart_id, uint64_t* pre_vpurf, uint64_t* post_data, uint64_t hid) {
   uint64_t fstep  =  sm_get_bits(csr_enc, 63, 62) + 1;
   uint64_t fstart =  sm_get_bits(csr_enc, 61, 57);
   uint64_t bytes  = (sm_get_bits(csr_enc, 56, 55) + 1) * 16;
   uint64_t rows   =  sm_get_bits(csr_enc, 54, 51) + 1;
   uint64_t VA     =  sm_get_bits(csr_enc, 47,  4) * 16;
   uint64_t stride =  sm_get_bits(x31, 47,  4) * 16;
   uint64_t coop   =  sm_get_bits(csr_enc, 50, 49);

   et_printf("[dbg] H%d Start TensorStore with addr: 000000%lx, stride: 00000000000000%lx, regstart: %d, rows: %d, cols: %d, srcinc: %d, coop: %d\n",
                    hid,                                    VA,                    stride,       fstart,      rows, bytes/16,      fstep,     coop+1);

   // If the CTS combination is invalid, do not check and return PASS
   if (coop == 0) {
      if (bytes == 48) return PASS;
   }
   else if (coop == 1) {
      if ((bytes == 48) || (bytes == 64)) return PASS;
   }
   else if (coop == 2) {
      return PASS;
   }
   else if (coop == 3) {
      if (bytes != 16) return PASS;
   }

   // Make sure the VA is the same one as the expected one
   if ((VA & 0xffffffffffffffc0) != (((uint64_t) post_data) & 0xFFFFFFFFFFFFFFC0)) {
      return NOT_PASS;
   }

   int ret;
   int mult = (bytes == 64) ? 2 : 1;
   for (uint64_t i = 0; i < rows; i++) {
      const uint64_t reg = (fstart + mult*i*fstep) % 32;
      uint64_t va_sc = VA + (i*stride);
      ret = selfcheck_ts_compare(hid, va_sc, pre_vpurf, reg, bytes, fstep);
      if (ret != PASS) break;
   }
   return ret;
}

int ts_selfchecking(tensor_data* snp_info) {
   uint64_t csr_enc, x31_val;
   uint64_t *pre_vpurf;
   uint64_t *post_data;

   uint64_t minion_id;
   uint64_t hart_id;

   csr_enc = snp_info->csr_enc;
   x31_val = snp_info->x31_val;
   pre_vpurf = (uint64_t*) snp_info->pre_vpurf_ptr;
   post_data = (uint64_t*) snp_info->post_data_ptr;

   minion_id = snp_info->minion_id;
   hart_id = minion_id << 1;

   return do_tstore(csr_enc, x31_val, hart_id, pre_vpurf, post_data, hart_id);
}

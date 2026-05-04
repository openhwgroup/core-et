
#include <assert.h>
#include <iostream>
#include <vector>
#include "snp_sequence.h"
#include "snp_seq_tload.h"

#define TLOAD_TENA  0
#define TLOAD_TENB  0
#define TLOAD_INT8  1
#define TLOAD_INT16 2
#define TLOAD_TRA8  5
#define TLOAD_TRA16 6
#define TLOAD_TRA32 7

#define NOT_TENB 0
#define IS_TENB  1

////////////////////////////////////////////////////////////////////////////
//
//  Tensor Load (Any Variant)
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
unum_t get_tload_ten_waits(CpuResource * cpu_resource, unum_t tl_csr_val) {
   unum_t ret = 0;
   unum_t config = snp_get_bits(cpu_resource, tl_csr_val, 61, 59);
   unum_t tenb   = snp_get_bits(cpu_resource, tl_csr_val, 52, 52);
   unum_t valid_config = config == TLOAD_TENA || config == TLOAD_INT8  || config == TLOAD_INT16 || \
                         config == TLOAD_TRA8 || config == TLOAD_TRA16 || config == TLOAD_TRA32;
   if (valid_config && !tenb) {
      ret = ret | WRITES_L1SCP | READS_FROM_MEM;
   }
   if (tenb) {
      ret = ret | READS_FROM_MEM;
      // WAIT_FOR_TFMA is required to avoid a known issue/conflict between a TFMA (non-tenb)
      // from an older sequence, and a TL tenb of the current sequence
      ret = ret | WAIT_FOR_TFMA;
   }
   return ret;
}
unum_t get_mem_size_tload_tena_tenb(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t mem_size = 0;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count-1;
   mem_size = i*stride + 64;
   return mem_size;
}
unum_t get_mem_size_tload_int8(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t offset = snp_get_bits(cpu_resource, tl_csr_val, 5, 4) * 16;
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count - 1;
   unum_t r = 3;

   unum_t VA = offset;
   unum_t mem_size = (VA + (i * 4 + r) * stride) + 16;
   return mem_size;
}
unum_t get_mem_size_tload_int16(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t r = 1;
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count - 1;
   unum_t offset = snp_get_bits(cpu_resource, tl_csr_val, 5, 5) * 32;
   unum_t VA = offset;
   unum_t mem_size = (VA + (i * 2 + r) * stride) + 32;
   return mem_size;
}
unum_t get_mem_size_tload_tra8(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count - 1;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t r = 63;
   unum_t offset = snp_get_bits(cpu_resource, tl_csr_val, 5, 4) * 16;
   unum_t VA = offset;
   unum_t mem_size = (VA + r*stride + i) + 1;
   return mem_size;
}
unum_t get_mem_size_tload_tra16(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count - 1;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t r = 31;
   unum_t offset = snp_get_bits(cpu_resource, tl_csr_val, 5, 5) * 32;
   unum_t VA = offset;
   unum_t mem_size = (VA + r*stride + i*2) + 2;
   return mem_size;
}
unum_t get_mem_size_tload_tra32(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
   unum_t i = count - 1;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t r = 15;
   unum_t VA = 0;
   unum_t mem_size = (VA + r*stride + i*4) + 4;
   return mem_size;
}
unum_t get_mem_size_tload(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t x31_val) {
   unum_t mem_size = 0;
   unum_t tenb = snp_get_bits(cpu_resource, tl_csr_val, 52, 52);
   if (tenb == 1) {
      mem_size = get_mem_size_tload_tena_tenb(cpu_resource, tl_csr_val, x31_val);
   }
   else {
      unum_t config = snp_get_bits(cpu_resource, tl_csr_val, 61, 59);
      switch (config) {
         case TLOAD_TENA:
            mem_size = get_mem_size_tload_tena_tenb(cpu_resource, tl_csr_val, x31_val);
            break;
         case TLOAD_INT8:
            mem_size = get_mem_size_tload_int8(cpu_resource, tl_csr_val, x31_val);
            break;
         case TLOAD_INT16:
            mem_size = get_mem_size_tload_int16(cpu_resource, tl_csr_val, x31_val);
            break;
         case TLOAD_TRA8:
            mem_size = get_mem_size_tload_tra8(cpu_resource, tl_csr_val, x31_val);
            break;
         case TLOAD_TRA16:
            mem_size = get_mem_size_tload_tra16(cpu_resource, tl_csr_val, x31_val);
            break;
         case TLOAD_TRA32:
            mem_size = get_mem_size_tload_tra32(cpu_resource, tl_csr_val, x31_val);
            break;
         default:
            mem_size = 0;
            break;
      }
   }
   // To avoid sharing the same cache line between different operations
   // We align the memory size to 64 bytes
   mem_size = ((mem_size%64) != 0) ? (mem_size & 0xffffffffffffffc0) + 64 : mem_size;
   return mem_size;
}
void selfcheck_boundaries_tload(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t config = snp_get_bits(cpu_resource, tl_csr_val, 61, 59);
   unum_t tenb   = snp_get_bits(cpu_resource, tl_csr_val, 52, 52);

   unum_t valid_config = config == TLOAD_TENA || config == TLOAD_INT8  || config == TLOAD_INT16 || \
                         config == TLOAD_TRA8 || config == TLOAD_TRA16 || config == TLOAD_TRA32;
   if (valid_config && !tenb) {
      unum_t dst   = snp_get_bits(cpu_resource, tl_csr_val, 58,53);
      unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
      for (int i = 0; i < count; i++) {
         // FUTURE: We currently ignore the mask and assume that all lines will be written
         unum_t scp_line = (dst + i) % 48;
         assert(scp_head <= scp_line && scp_line <= scp_tail);
      }
   }
}
void selfcheck_set_tload(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val, unum_t selfcheck, unum_t set) {
   // Make sure that at least one of the two options are set
   assert(selfcheck || set);

   unum_t config = snp_get_bits(cpu_resource, tl_csr_val, 61, 59);
   unum_t tenb   = snp_get_bits(cpu_resource, tl_csr_val, 52, 52);

   unum_t valid_config = config == TLOAD_TENA || config == TLOAD_INT8  || config == TLOAD_INT16 || \
                         config == TLOAD_TRA8 || config == TLOAD_TRA16 || config == TLOAD_TRA32;
   if (valid_config && !tenb) {
      unum_t dst   = snp_get_bits(cpu_resource, tl_csr_val, 58,53);
      unum_t count = snp_get_bits(cpu_resource, tl_csr_val, 3, 0) + 1;
      for (int i = 0; i < count; i++) {
         // FUTURE: We currently ignore the mask and assume that all lines will be written
         if (selfcheck) assert(l1_scp[(dst + i) % 48] != L1SCP_SHARED);
         if (set) l1_scp[(dst + i) % 48] = L1SCP_MODIFIED;
      }

   }
}
void set_tload_l1scp(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val) {
   selfcheck_set_tload(cpu_resource, l1_scp, tl_csr_val, 0, 1);
}
void selfcheck_tload(CpuResource * cpu_resource, unum_t *l1_scp, unum_t tl_csr_val) {
   selfcheck_set_tload(cpu_resource, l1_scp, tl_csr_val, 1, 0);
}
unum_t constrain_tload_write_lines(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t ret = tl_csr_val;
   unum_t config = snp_get_bits(cpu_resource, tl_csr_val, 61, 59);
   unum_t tenb   = snp_get_bits(cpu_resource, tl_csr_val, 52, 52);
   unum_t valid_config = config == TLOAD_TENA || config == TLOAD_INT8  || config == TLOAD_INT16 || \
                         config == TLOAD_TRA8 || config == TLOAD_TRA16 || config == TLOAD_TRA32;
   if (valid_config && !tenb) {
      unum_t start_scp_line = snp_urandrange(cpu_resource, scp_head, scp_tail+1);
      unum_t num_scp_lines  = scp_tail - start_scp_line + 1;
      num_scp_lines         = et_min(num_scp_lines, 16);
      num_scp_lines         = snp_urandrange(cpu_resource, 0, num_scp_lines);

      ret = snp_set_bits(cpu_resource, ret, 58, 53, start_scp_line);
      ret = snp_set_bits(cpu_resource, ret,  3,  0, num_scp_lines);
      selfcheck_boundaries_tload(cpu_resource, ret, scp_head, scp_tail);
   }
   return ret;
}
unum_t snp_seq_tload_base(CpuResource * cpu_resource, unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t *l1_scp, unum_t tenb) {
   data_region scp_region;
   unum_t tl_csr_val;
   if (tenb) tl_csr_val = snp_get_tload_tenb_wrapper(cpu_resource);
   else {
      tl_csr_val = snp_get_tload_wrapper(cpu_resource);
      scp_region = tl_get_empty_l1scp_range(cpu_resource, l1_scp, ten_seq, idx, num_ops);
   }
   tl_csr_val = constrain_tload_write_lines(cpu_resource, tl_csr_val, scp_region.head, scp_region.tail);
   selfcheck_set_tload(cpu_resource, l1_scp, tl_csr_val, 1, 1);
   return tl_csr_val;
}
unum_t snp_seq_tload_wrapper(CpuResource * cpu_resource) {
   unum_t l1_scp[L1SCP_SIZE];
   init_l1scp(l1_scp);
   unum_t stub[] = {TLOAD_OP};
   return snp_seq_tload_base(cpu_resource, stub, 0, 1, l1_scp, NOT_TENB);
}

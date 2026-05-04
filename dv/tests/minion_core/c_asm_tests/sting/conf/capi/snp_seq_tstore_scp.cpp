
#include <assert.h>
#include <iostream>
#include <vector>
#include "snp_sequence.h"
#include "snp_seq_tstore.h"

////////////////////////////////////////////////////////////////////////////
//
//  Tensor Store From SCP
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
unum_t get_ts_scp_ten_waits(CpuResource * cpu_resource, unum_t tstore_csr_val) {
   // Tensor Store From SCP does not read from VPU RF, but due to resource collision,
   // a Tensor Store From SCP must also wait as a normal TensorStore
   return READS_L1SCP | READS_VPU_RF | WRITES_TO_MEM;
}
unum_t get_mem_size_ts_scp(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t x31_val) {
   unum_t rows = snp_get_bits(cpu_resource, ts_scp_csr_val, 54, 51) + 1;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   unum_t mem_size = 0;
   mem_size = (rows-1)*stride + 64;
   // To avoid sharing the same cache line between different operations
   // We align the memory size to 64 bytes
   mem_size = ((mem_size%64) != 0) ? (mem_size & 0xffffffffffffffc0) + 64 : mem_size;
   // As the memory address could not be aligned to 64 bytes, we add 64 extra bytes just in case
   mem_size = mem_size + 64;
   return mem_size;
}
unum_t selfcheck_set_ts_scp(CpuResource * cpu_resource, unum_t *l1_scp, unum_t ts_scp_csr_val, unum_t selfcheck, unum_t set) {
   unum_t step = snp_get_bits(cpu_resource, ts_scp_csr_val, 63, 62) + 1;
   unum_t src = snp_get_bits(cpu_resource, ts_scp_csr_val, 61, 56);
   unum_t rows = snp_get_bits(cpu_resource, ts_scp_csr_val, 54, 51) + 1;

   for (int i = 0; i < rows; i++) {
      unum_t scp_line = (src + (i*step))%48;
      if (selfcheck) assert(l1_scp[scp_line] != L1SCP_MODIFIED);
      if (set) l1_scp[scp_line] = L1SCP_SHARED;
   }
}
void selfcheck_boundaries_ts_scp(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t head, unum_t tail) {
   unum_t step = snp_get_bits(cpu_resource, ts_scp_csr_val, 63, 62) + 1;
   unum_t src = snp_get_bits(cpu_resource, ts_scp_csr_val, 61, 56);
   unum_t rows = snp_get_bits(cpu_resource, ts_scp_csr_val, 54, 51) + 1;

   for (int i = 0; i < rows; i++) {
      unum_t scp_line = (src + (i*step))%48;
      assert((head <= scp_line) && (scp_line <= tail));
   }
}
unum_t constrain_ts_scp_read(CpuResource * cpu_resource, unum_t ts_scp_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t src = snp_urandrange(cpu_resource, scp_head, scp_tail+1);
   unum_t max_rows = (scp_tail-src)%17;
   unum_t rows = snp_urandrange(cpu_resource, 1, max_rows+1);

   if (rows > 1) {
      unum_t max_step = ((scp_tail - src)/(rows-1))%5;
      unum_t step = snp_urandrange(cpu_resource, 1, max_step+1);
      assert(step > 0);
      ts_scp_csr_val = snp_set_bits(cpu_resource, ts_scp_csr_val, 63, 62, step - 1);
   }
   assert(rows > 0);
   ts_scp_csr_val = snp_set_bits(cpu_resource, ts_scp_csr_val, 61, 56, src);
   ts_scp_csr_val = snp_set_bits(cpu_resource, ts_scp_csr_val, 54, 51, rows - 1);
   selfcheck_boundaries_ts_scp(cpu_resource, ts_scp_csr_val, scp_head, scp_tail);
   return ts_scp_csr_val;
}
unum_t snp_seq_ts_scp_base(CpuResource * cpu_resource, unum_t *l1_scp) {
   unum_t ts_scp_csr_val = snp_get_tstore_scp_wrapper(cpu_resource);

   data_region scp_region = get_empty_l1scp_range(cpu_resource, l1_scp, L1SCP_SHARED);
   ts_scp_csr_val = constrain_ts_scp_read(cpu_resource, ts_scp_csr_val, scp_region.head, scp_region.tail);

   // Self Check there is no potential L1SCP collisions and set the lines in the L1SCP model
   selfcheck_set_ts_scp(cpu_resource, l1_scp, ts_scp_csr_val, 1, 1);
   return ts_scp_csr_val;
}

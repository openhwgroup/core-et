
#include <assert.h>
#include <iostream>
#include <vector>
#include "snp_sequence.h"
#include "snp_tl_l2.h"
#include "snp_seq_tload_l2.h"

////////////////////////////////////////////////////////////////////////////
//
//  Tensor Load L2
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
unum_t get_tload_l2_ten_waits(CpuResource * cpu_resource, unum_t tl_l2_csr_val) {
   return 0;
}
unum_t get_mem_size_tload_l2(CpuResource * cpu_resource, unum_t tl_l2_csr_val, unum_t x31_val) {
   unum_t count = snp_get_bits(cpu_resource, tl_l2_csr_val, 3, 0) + 1;
   unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 6) * 64;
   return count*stride + 64;
}
void selfcheck_boundaries_tload_l2(CpuResource * cpu_resource, unum_t tl_l2_csr_val) {
   unum_t dflt_l2scp_size = 0x280000; // Bytes
   unum_t l2scp_line_size = 64; // Bytes
   unum_t dflt_l2scp_lines = dflt_l2scp_size / l2scp_line_size;

   unum_t count  = snp_get_bits(cpu_resource, tl_l2_csr_val, 3, 0) + 1; 
   unum_t head_h = snp_get_bits(cpu_resource, tl_l2_csr_val, 62, 48);
   unum_t head_l = snp_get_bits(cpu_resource, tl_l2_csr_val, 5, 4);
   unum_t head = head_h * 4 + head_l;
   unum_t tail = head + (count - 1);

   assert(head >= 0);
   assert(tail <= dflt_l2scp_lines); 
}
unum_t snp_seq_tload_l2_base(CpuResource * cpu_resource) {
   unum_t tl_l2scp_csr_base = snp_randnum(cpu_resource);
   unum_t tl_l2scp_csr_val = snp_set_valid_num_scp_lines(cpu_resource, tl_l2scp_csr_base);
   tl_l2scp_csr_val = tl_l2scp_csr_val & 0xffff00000000003f;
   selfcheck_boundaries_tload_l2(cpu_resource, tl_l2scp_csr_val);
   return tl_l2scp_csr_val;
}

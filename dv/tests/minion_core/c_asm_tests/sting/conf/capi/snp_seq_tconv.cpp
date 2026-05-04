
#include <assert.h>
#include <iostream>
#include <vector>
#include "snp_sequence.h"
#include "snp_seq_tconv.h"
#include "snp_tconv.h"

////////////////////////////////////////////////////////////////////////////
//
//  Tensor Convolution
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
unum_t get_tconv_size_ten_waits(CpuResource * cpu_resource, unum_t tconv_csr_val) {
   return 0;
}
unum_t get_tconv_ctrl_ten_waits(CpuResource * cpu_resource, unum_t tconv_csr_val) {
   return 0;
}
unum_t get_mem_size_tconv_size(CpuResource * cpu_resource, unum_t tconv_csr_val, unum_t x31_val) {
   return 0;
}
unum_t get_mem_size_tconv_ctrl(CpuResource * cpu_resource, unum_t tconv_csr_val, unum_t x31_val) {
   return 0;
}
unum_t snp_seq_tconv_size_base(CpuResource * cpu_resource) {
   return snp_get_tensor_conv_size(cpu_resource);
}
unum_t snp_paired_tconv_ctrl_base(CpuResource * cpu_resource, unum_t tconv_size) {
   return snp_get_tensor_conv_ctrl(cpu_resource, tconv_size);
}
unum_t snp_seq_tconv_ctrl_base(CpuResource * cpu_resource) {
   unum_t rand_conv_size = snp_seq_tconv_size_base(cpu_resource);
   return snp_paired_tconv_ctrl_base(cpu_resource, rand_conv_size);
}

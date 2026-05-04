
#include "snp_tconv.h"
#include "snp_common.h"

// IS_GLBL_RAND and NOT_GLBL_RAND are defined in snp_common.h

unum_t __get_tensor_conv_size(CpuResource * cpu_resource, const unum_t is_glbl) {
   unum_t ret;
   unum_t rand_bias = et_urandrange(cpu_resource, is_glbl, 0, 27);
   if (rand_bias > 0) {
      unum_t rdiv, cdiv;
      unum_t warl = snp_randnum(cpu_resource);

      unum_t nrow = et_urandrange(cpu_resource, is_glbl, 0, 65536); // Random number of 16 bits
      unum_t ncol = et_urandrange(cpu_resource, is_glbl, 0, 65536); // Random number of 16 bits

      unum_t srow_bias = et_urandrange(cpu_resource, is_glbl, 0, 4);
      if (srow_bias == 0)      rdiv = nrow >> 1;
      else if (srow_bias <= 2) rdiv = nrow >> 2;
      else                     rdiv = nrow >> 3;
      unum_t srow = et_urandrange(cpu_resource, is_glbl, rdiv/2, rdiv+1) & 0x7fff;

      unum_t scol_bias = et_urandrange(cpu_resource, is_glbl, 0, 4);
      if (scol_bias == 0)      cdiv = ncol >> 1;
      else if (scol_bias <= 2) cdiv = ncol >> 2;
      else                     cdiv = ncol >> 3;
      unum_t scol = et_urandrange(cpu_resource, is_glbl, cdiv/2, cdiv+1) & 0x7fff;

      ret = snp_set_bits(cpu_resource, warl, 63, 56, srow);
      ret = snp_set_bits(cpu_resource,  ret, 47, 32, nrow);
      ret = snp_set_bits(cpu_resource,  ret, 31, 24, scol);
      ret = snp_set_bits(cpu_resource,  ret, 15,  0, ncol);
   }
   else {
      ret = et_randnum(cpu_resource, is_glbl);
   }

   return ret;
}

unum_t __get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size, const unum_t is_glbl) {
   unum_t ret;
   unum_t rand_bias = et_urandrange(cpu_resource, is_glbl, 0, 27);
   if (rand_bias > 0) {
      unum_t warl = snp_randnum(cpu_resource);

      unum_t srow = snp_get_bits(cpu_resource, tensor_conv_size, 63, 56);
      unum_t scol = snp_get_bits(cpu_resource, tensor_conv_size, 31, 24);

      unum_t row_start = et_urandrange(cpu_resource, is_glbl, 0, srow+1) & 0x7fff;
      unum_t col_start = et_urandrange(cpu_resource, is_glbl, 0, scol+1) & 0x7fff;

      ret = snp_set_bits(cpu_resource, warl, 47, 32, row_start);
      ret = snp_set_bits(cpu_resource,  ret, 15,  0, col_start);
   }
   else {
      ret = et_randnum(cpu_resource, is_glbl);
   }
   return ret;
}

unum_t glbl_snp_get_tensor_conv_size(CpuResource * cpu_resource) {
   return __get_tensor_conv_size(cpu_resource, IS_GLBL_RAND);
}

unum_t snp_get_tensor_conv_size(CpuResource * cpu_resource) {
   return __get_tensor_conv_size(cpu_resource, NOT_GLBL_RAND);
}

unum_t glbl_snp_get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size) {
   return __get_tensor_conv_ctrl(cpu_resource, tensor_conv_size, IS_GLBL_RAND);
}

unum_t snp_get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size) {
   return __get_tensor_conv_ctrl(cpu_resource, tensor_conv_size, NOT_GLBL_RAND);
}

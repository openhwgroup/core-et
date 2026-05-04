
#include "snp_common.h"
#include "snp_tfma.h"
#include "snp_sequence.h"

unum_t snp_tfma_from_scp(CpuResource * cpu_resource, unum_t value) {
    value = snp_set_bits(cpu_resource, value, 20, 20, 0);
    return value;
}

unum_t snp_tfma_from_tenb(CpuResource * cpu_resource, unum_t value) {
    value = snp_set_bits(cpu_resource, value, 20, 20, 1);
    return value;
}

unum_t snp_tfma_set_ima8a32(CpuResource * cpu_resource, unum_t csr_value) {
    csr_value = snp_set_bits(cpu_resource, csr_value,  3,  1, 0b011);
    csr_value = snp_set_bits(cpu_resource, csr_value, 63, 63,     0);
    return csr_value;
}

unum_t snp_set_valid_tfma_op(CpuResource * cpu_resource, unum_t rand_val, unum_t tenb) {
    if (tenb == 0) {
        rand_val = snp_tfma_from_scp(cpu_resource, rand_val);
    }
    else {
        rand_val = snp_tfma_from_tenb(cpu_resource, rand_val);
    }
    unum_t tfma_op = (snp_get_bits(cpu_resource, rand_val, 3, 1))%3;
    switch (tfma_op) {
        case 0:
            tfma_op = 0b000; // TensorFMA32
            break;
        case 1:
            tfma_op = 0b001; // TensorFMA16A32
            break;
        case 2:
            tfma_op = 0b011; // TensorIMA8A32
            break;
    }
    rand_val = snp_set_bits(cpu_resource, rand_val, 3, 1, tfma_op);
    return rand_val;
}

unum_t snp_set_ilegal_tfma_op(CpuResource * cpu_resource, unum_t rand_val, unum_t tenb) {
    if (tenb == 0) {
        rand_val = snp_tfma_from_scp(cpu_resource, rand_val);
    }
    else {
        rand_val = snp_tfma_from_tenb(cpu_resource, rand_val);
    }
    unum_t tfma_op = (snp_get_bits(cpu_resource, rand_val, 3, 1))%5;
    switch (tfma_op) {
        case 0:
            tfma_op = 0b010;
            break;
        case 1:
            tfma_op = 0b100;
            break;
        case 2:
            tfma_op = 0b101;
            break;
        case 3:
            tfma_op = 0b110;
            break;
        case 4:
            tfma_op = 0b111;
            break;
    }
    rand_val = snp_set_bits(cpu_resource, rand_val, 3, 1, tfma_op);
    return rand_val;
}

unum_t snp_tima8a32_tenc_clear_setup(CpuResource * cpu_resource, unum_t old_a_rows, unum_t csr_value) {
   unum_t tfma_op = snp_get_bits(cpu_resource, csr_value, 3, 1);
   if (tfma_op == 0b011) {
      // If it is a TIMA8A32 operation
      unum_t clear_tenc = snp_get_bits(cpu_resource, csr_value, 0, 0);
      if (clear_tenc == 0) {
         // If the clear tenc bit was not set
         unum_t a_rows = snp_get_bits(cpu_resource, csr_value, 54, 51);
         if (a_rows > old_a_rows) {
            // We must clean the tenc to avoid errors
            snp_set_bits(cpu_resource, csr_value, 0, 0, 1);
         }
      }
   }
   return csr_value;
}

unum_t snp_tima8a32_update_tenc_clear(CpuResource * cpu_resource, unum_t old_a_rows, unum_t csr_value) {
   unum_t tfma_op = snp_get_bits(cpu_resource, csr_value, 3, 1);
   unum_t next_a_rows = old_a_rows;
   if (tfma_op == 0b011) {
      // If it is a TIMA8A32 operation
      unum_t clear_tenc = snp_get_bits(cpu_resource, csr_value, 0, 0);
      if (clear_tenc == 1) {
         // If the clear tenc bit was set
         unum_t a_rows = snp_get_bits(cpu_resource, csr_value, 54, 51);
         if (a_rows > old_a_rows) {
            next_a_rows = a_rows;
         }
      }
   }
   return next_a_rows;
}

unum_t snp_get_tfma_wrapper(CpuResource * cpu_resource) {
   unum_t tfma_rand = snp_randnum(cpu_resource);
   unum_t tfma_csr_val = snp_set_valid_tfma_op(cpu_resource, tfma_rand, 0);
   return tfma_csr_val;
}

unum_t snp_get_tfma_tenb_wrapper(CpuResource * cpu_resource) {
   unum_t tfma_rand = snp_randnum(cpu_resource);
   unum_t tfma_csr_val = snp_set_valid_tfma_op(cpu_resource, tfma_rand, 1);
   return tfma_csr_val;
}

unum_t snp_get_tfma_pre_ten_waits(CpuResource * cpu_resource, unum_t tfma_enc) {
   csr_op tfma;
   tfma.op_type = TFMA_OP;
   tfma.csr_val = tfma_enc;
   return get_tensor_waits_from_op(cpu_resource, tfma);
}

//                                                          xs[23],      xs[20],             xs[0]
unum_t glbl_snp_sm_tfma_wrapper(CpuResource * cpu_resource, unum_t tenc, unum_t tenb, unum_t first_pass, unum_t tl_tenb_csr_val) {
   unum_t tfma_value = snp_get_tfma_wrapper(cpu_resource);
   if (tenc != 0) {
      // xs[23] = Location of result (0: C, 1: floating-point registers)
      tfma_value = snp_set_bits(cpu_resource, tfma_value, 23, 23, 1);
   }
   if (tenb != 0) {
      // xs[20] = Location of matrix B (0=L1 scratchpad, 1=memory)
      tfma_value = snp_set_bits(cpu_resource, tfma_value, 20, 20, 1);

      // Pair TL with TFMA
      unum_t tl_bits = snp_get_bits(cpu_resource, tl_tenb_csr_val, 3, 0);
      // In order to test tensor_error[6], we will force a wrong configuration
      unum_t wrong_pairing_bias = snp_urandrange_glbl(cpu_resource, 0, 27);
      if (wrong_pairing_bias == 0) {
         tl_bits = ~tl_bits;
      }
      tfma_value = snp_set_bits(cpu_resource, tfma_value, 50, 47, tl_bits);
   }
   if (first_pass != 0) {
      // xs[0] = If set to 1 then the source floating-point registers are cleared before starting the operation.
      tfma_value = snp_set_bits(cpu_resource, tfma_value,  0,  0, 1);
   }
   return tfma_value;
}

unum_t glbl_snp_get_sm_tfma(CpuResource * cpu_resource) {
   unum_t tenc_bias = snp_urandrange_glbl(cpu_resource, 0, 3);
   unum_t tenc      = (tenc_bias == 0) ? 1 : 0;

   unum_t first_pass_bias = snp_urandrange_glbl(cpu_resource, 0, 12);
   unum_t first_pass      = (first_pass_bias == 0) ? 1 : 0;

   return glbl_snp_sm_tfma_wrapper(cpu_resource, tenc, 0, first_pass, 0);
}

unum_t glbl_snp_get_sm_tenb_tfma_wrapper(CpuResource * cpu_resource, unum_t tl_tenb_csr_val) {
   unum_t tenc_bias = snp_urandrange_glbl(cpu_resource, 0, 3);
   unum_t tenc      = (tenc_bias == 0) ? 1 : 0;

   unum_t first_pass_bias = snp_urandrange_glbl(cpu_resource, 0, 12);
   unum_t first_pass      = (first_pass_bias == 0) ? 1 : 0;

   return glbl_snp_sm_tfma_wrapper(cpu_resource, tenc, 1, first_pass, tl_tenb_csr_val);
}

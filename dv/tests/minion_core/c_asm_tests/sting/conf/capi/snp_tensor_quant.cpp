
#include "snp_common.h"
#include "snp_tensor_quant.h"

unum_t snp_set_ntrans_tquant(CpuResource * cpu_resource, unum_t ntrans, unum_t csr_val, unum_t trans_val) {

    if (ntrans < 10) {
        unum_t bias   = snp_urandrange(cpu_resource, 0, 10);
        unum_t is_sat = (trans_val == 8) || (trans_val == 9);
        if (is_sat && (bias != 0)) {
            trans_val = snp_urandrange(cpu_resource, 0, 11);
            return snp_set_ntrans_tquant(cpu_resource, ntrans, csr_val, trans_val);
        }
        else {
            unum_t lb = ntrans*4;
            unum_t hb = lb + 3;
            return snp_set_bits(cpu_resource, csr_val, hb, lb, trans_val);
        }
    }
    else {
        ntrans = snp_urandrange(cpu_resource, 0, 10);
        return snp_set_ntrans_tquant(cpu_resource, ntrans, csr_val, trans_val);
    }
}

unum_t snp_set_valid_tquant_op(CpuResource * cpu_resource, unum_t rand_val) {

    unum_t n_trans = snp_urandrange(cpu_resource, 0, 11);
    unum_t i; 
    for (i = 0; i < 10; i++) {
        unum_t rand_trans_val;
        if (i < n_trans) {
            rand_trans_val = snp_urandrange(cpu_resource, 0, 11);
            rand_val = snp_set_ntrans_tquant(cpu_resource, i, rand_val, rand_trans_val);
        }
        else if (i == n_trans){
            rand_trans_val = 0;
            rand_val = snp_set_ntrans_tquant(cpu_resource, i, rand_val, rand_trans_val);
        }
    }
    return rand_val;
}

unum_t snp_get_tquant_wrapper(CpuResource * cpu_resource) {
   unum_t csr_val_rand = snp_randnum(cpu_resource);
   unum_t tquant_csr_val = snp_set_valid_tquant_op(cpu_resource, csr_val_rand);
   return tquant_csr_val;
}

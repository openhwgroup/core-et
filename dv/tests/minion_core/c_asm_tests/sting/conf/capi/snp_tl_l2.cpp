
#include "snp_common.h"
#include "et_soc.h"
#include "snp_tl_l2.h"
#include <assert.h>

#define SC_SCP_SIZE 0x280000
#define SC_NUM_SCP_LINES (SC_SCP_SIZE/64)

unum_t snp_set_valid_num_scp_lines(CpuResource * cpu_resource, unum_t csr_value) {
    unum_t num_lines = snp_get_bits(cpu_resource, csr_value, 3, 0);

    unum_t l2_scp_start_line = snp_urandrange(cpu_resource, 0, SC_NUM_SCP_LINES - num_lines);

    unum_t hb = snp_get_bits(cpu_resource, l2_scp_start_line, 16, 2);
    unum_t lb = snp_get_bits(cpu_resource, l2_scp_start_line,  1, 0);

    csr_value = snp_set_bits(cpu_resource, csr_value, 62, 48, hb);
    csr_value = snp_set_bits(cpu_resource, csr_value,  5,  4, lb);

    return csr_value;
}

unum_t snp_constrain_l2_lines(CpuResource * cpu_resource, unum_t csr_value) {

    unum_t local_active_minions = snp_get_local_active_harts(cpu_resource);
    unum_t l2scp_per_hart_size = SC_NUM_SCP_LINES/local_active_minions;
    unum_t local_minion_idx = get_idx_from_active_local_shire_minions(cpu_resource);

    unum_t head = local_minion_idx * l2scp_per_hart_size;
    unum_t tail = head + l2scp_per_hart_size; // non-inclusive tail

    unum_t num_lines = snp_get_bits(cpu_resource, csr_value, 3, 0);
    num_lines = (num_lines >= l2scp_per_hart_size) ? l2scp_per_hart_size - 1 : num_lines;

    assert(num_lines < l2scp_per_hart_size);

    unum_t l2_scp_start_line = snp_urandrange(cpu_resource, head, tail - num_lines);

    assert((l2_scp_start_line + num_lines) < SC_NUM_SCP_LINES);

    unum_t hb = snp_get_bits(cpu_resource, l2_scp_start_line, 16, 2);
    unum_t lb = snp_get_bits(cpu_resource, l2_scp_start_line,  1, 0);

    csr_value = snp_set_bits(cpu_resource, csr_value, 62, 48, hb);
    csr_value = snp_set_bits(cpu_resource, csr_value,  5,  4, lb);

    return csr_value;
}

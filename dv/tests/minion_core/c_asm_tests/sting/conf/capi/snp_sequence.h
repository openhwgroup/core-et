/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SEQ_H_
#define _ET_SEQ_H_

#include <set>
#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"

#define BITS_PER_OP 4
#define OP_BITS_MASK 15
#define TENSOR_OPS_SIZE 11
#define TFMA_OP       0
#define TLOAD_OP      1
#define TFMA_TENB_OP  2
#define TLOAD_TENB_OP 3
#define TQUANT_OP     4
#define TSTORE_OP     5
#define TREDUCE_OP    6
#define TSTORE_SCP_OP 7
#define TCONV_SIZE_OP 8
#define TCONV_CTRL_OP 9
#define TLOAD_L2_OP   10

#define L1SCP_SIZE 48
#define L1SCP_NOT_USED 0
#define L1SCP_SHARED   1
#define L1SCP_MODIFIED 2

#define VPU_RF_SIZE 32
#define VPU_RF_NOT_USED 10
#define VPU_RF_SHARED   11
#define VPU_RF_MODIFIED 12
#define VPU_RF_EXCLUSIVE 13

#define WRITES_L1SCP 1
#define READS_L1SCP 2
#define WRITES_VPU_RF 4
#define READS_VPU_RF 8
#define WRITES_TO_MEM 16
#define READS_FROM_MEM 32
#define WAIT_FOR_TFMA 64

#define TEN_WAIT_TL_0    0
#define TEN_WAIT_TL_1    1
#define TEN_WAIT_TL_L2_0 2
#define TEN_WAIT_TL_L2_1 3
#define TEN_WAIT_TFMA    7
#define TEN_WAIT_TSTORE  8
#define TEN_WAIT_TREDUCE 9
#define TEN_WAIT_TQUANT  10

#define encode_ten_wait(a) \
   ({ __typeof__ (a) _a = (a); \
      1 << _a; })


#define et_min(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct {
   int head;
   int tail;
} data_region;

typedef struct {
   unum_t op_type;
   unum_t csr_val;
} csr_op;

void print_ten_seq(CpuResource * cpu_resource, unum_t *ops, unum_t num_ops);
void init_data_struct(unum_t *data_struct, const unum_t data_struct_size, const unum_t not_used_type);
void init_l1scp(unum_t *l1_scp);
void init_vpu_rf(unum_t *vpu_rf);
void print_vpu_rf(unum_t *vpu_rf, char* msg);
unum_t init_l1scp_ptr(CpuResource * cpu_resource);
unum_t init_vpu_rf_ptr(CpuResource * cpu_resource);
unum_t init_tensor_sequence(CpuResource * cpu_resource, unum_t num_ops);
std::vector<unum_t> set_tensor_enc(CpuResource * cpu_resource);
unum_t free_l1scp_ptr(CpuResource * cpu_resource, unum_t l1scp_ptr);
unum_t free_vpu_rf_ptr(CpuResource * cpu_resource, unum_t vpu_rf_ptr);
unum_t free_tensor_sequence(CpuResource * cpu_resource, unum_t ten_seq_ptr);
unum_t get_clean_tenb_tfma(CpuResource * cpu_resource, unum_t l1scp_ptr, unum_t vpu_rf_ptr, unum_t ten_seq_ptr, unum_t num_ops);
unum_t get_clean_tenb_twait(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops);
unum_t glbl_get_csr_val_from_op(CpuResource * cpu_resource, unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t* l1_scp, unum_t* vpu_rf);
void set_empty_data_ranges(std::vector<data_region> &free_regions, unum_t *data_struct, const unum_t data_struct_size, const unum_t not_used_type, unum_t op_type, unum_t assert_size);
data_region get_empty_l1scp_range(CpuResource * cpu_resource, unum_t *l1_scp, unum_t op_type);
void regions_pre_booking(CpuResource * cpu_resource, const unum_t *ten_seq, unum_t num_ops, unum_t target_op, unum_t not_used_op, unum_t *data_reg, unum_t data_reg_size, const std::set<unum_t>& ops_set);
void vpu_rf_pre_booking(CpuResource * cpu_resource, const unum_t *ten_seq, unum_t num_ops, unum_t * vpu_rf);
void l1scp_pre_booking(CpuResource * cpu_resource, const unum_t *ten_seq, unum_t num_ops, unum_t * l1_scp);
std::vector<data_region> get_fair_empty_regions(CpuResource * cpu_resource, unum_t *data_reg, unum_t data_reg_size, unum_t data_reg_not_used, const unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t region_op, const std::set<unum_t>& read_ops, const std::set<unum_t> &write_ops);
data_region tl_get_empty_l1scp_range(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *ten_seq, unum_t idx, unum_t num_ops);
data_region get_empty_vpu_rf_range(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t op_type, unum_t *ten_seq, unum_t idx, unum_t num_ops);
data_region get_empty_vpu_tfma_range(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t op_type, unum_t *ten_seq, unum_t idx, unum_t num_ops);
data_region get_empty_vpu_reduce_range(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t num_regs, unum_t op_type);
void do_fisher_yates(CpuResource * cpu_resource, csr_op* ten_ops_seq, unum_t seq_size, unum_t* ops_idx);
void glbl_do_fisher_yates(CpuResource * cpu_resource, unum_t seq_size, unum_t* ops_idx);
void pair_tconv_size_ctrl(CpuResource * cpu_resource, csr_op * ten_seq, unum_t num_ops);
unum_t is_tfma_tena_executed(CpuResource * cpu_resource, csr_op * ten_seq, unum_t tfma_idx);
unum_t is_tfma_tenb_paired(CpuResource * cpu_resource, csr_op * ten_seq, unum_t tfma_idx);
unum_t is_tfmaa_tlb_dependency(CpuResource * cpu_resource, csr_op * ten_seq, unum_t start, unum_t num_ops);
unum_t get_tstore_scp_dependency(CpuResource * cpu_resource, csr_op * ten_seq, unum_t start, unum_t num_ops);
unum_t get_tfma_tquant_tsscp_dependency(CpuResource * cpu_resource, csr_op * ten_seq, unum_t start, unum_t num_ops);
unum_t get_tfma_dependency(CpuResource * cpu_resource, csr_op * ten_seq, unum_t start, unum_t num_ops);
unum_t get_tquant_dependency(CpuResource * cpu_resource, csr_op * ten_seq, unum_t start, unum_t num_ops);
unum_t get_tenwait_value_from_idx(CpuResource * cpu_resource, unum_t idx, unum_t ten_seq_ptr, unum_t num_ops);
unum_t get_glbl_rand_op(CpuResource * cpu_resource, std::vector<unum_t> &ten_seq_enc);
unum_t get_rand_op_excl(CpuResource * cpu_resource, unum_t excl_op, std::vector<unum_t> &ten_seq_enc);
void avoid_rtlmin_6040(CpuResource * cpu_resource, unum_t *ops, unum_t num_ops, std::vector<unum_t> &ten_seq_enc);
void limit_num_treduces(CpuResource * cpu_resource, unum_t *ops, unum_t num_ops, std::vector<unum_t> &ten_seq_enc);
unum_t order_ops_by_type(unum_t it_start, unum_t op_type, unum_t *ops, unum_t num_ops);
void order_ops_by_priority(unum_t *ops, unum_t num_ops);
void selfcheck_decode(CpuResource * cpu_resource, unum_t *ops, unum_t num_ops, unum_t encoded_seq);
unum_t encode_tensor_ops_sequence(CpuResource * cpu_resource, unum_t *ops, unum_t num_ops);
unum_t decode_tensor_ops_sequence(CpuResource * cpu_resource, unum_t encoded_seq, unum_t op_pos);
void glbl_set_rand_sequence(CpuResource * cpu_resource, unum_t* ops, unum_t num_ops, std::vector<unum_t> &ten_seq_enc);
void glbl_set_unique_sequence(CpuResource * cpu_resource, unum_t* ops, unum_t num_ops, std::vector<unum_t> &ten_seq_enc);
unum_t glbl_snp_set_tensor_ops_sequence(CpuResource * cpu_resource, unum_t l1scp_ptr, unum_t vpu_rf_ptr, unum_t ten_seq_ptr, unum_t num_ops, unum_t rand_init);
unum_t get_csr_op_type_from_idx(CpuResource * cpu_resource, unum_t idx, unum_t ten_seq_ptr);
unum_t get_csr_op_value_from_idx(CpuResource * cpu_resource, unum_t idx, unum_t ten_seq_ptr);
unum_t get_tensor_waits_from_op(CpuResource * cpu_resource, csr_op operation);
unum_t get_pre_tensor_wait(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops, unum_t tfma_clean_tenb_csr);
unum_t get_post_tensor_wait(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops, unum_t tfma_clean_tenb_csr, unum_t x31_val);
unum_t get_mem_offset_from_idx(CpuResource * cpu_resource, unum_t idx, unum_t ten_seq_ptr, unum_t x31_val);
unum_t selfcheck_mem_occupation(CpuResource * cpu_resource, unum_t mem_size, unum_t ten_seq_ptr, unum_t num_ops, unum_t x31_val);
unum_t uses_tensor_mask(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops);
unum_t uses_x31_from_idx(CpuResource * cpu_resource, unum_t idx, unum_t ten_seq_ptr, unum_t num_ops);
unum_t uses_x31(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops);
unum_t snp_get_tenb_pre_ten_waits(CpuResource * cpu_resource, unum_t tl_enc, unum_t tfma_enc);
unum_t snp_get_tl_l2_pre_ten_waits(CpuResource * cpu_resource, unum_t tl_l2_enc);
unum_t snp_ow_unpr_fregs(CpuResource * cpu_resource, unum_t ten_seq_ptr, unum_t num_ops, unum_t vpu_rf_ptr);

#endif

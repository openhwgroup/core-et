/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MANUAL_DPI_IMPL_H_
#define _MANUAL_DPI_IMPL_H_

extern "C" {
void inst_retired(uint64_t cycle, uint32_t minion_id, bool thread_id, uint64_t pc, int inst_bits, bool delayed_write,
                  bool load_miss, bool wbx, int wbx_addr, uint64_t wbx_data, bool wbf,
                  int wbf_addr, /*svBitVecVal * wbf_data_,*/
                  bool wbm, bool wbmall, int wbm_addr, uint64_t wbm_data, bool store, int store_size,
                  uint64_t store_addr, svBitVecVal* store_data_, int store_mask, bool delayed_store, int store_age_id,
                  bool scatter, svBitVecVal* prev_scatter_addr_, svBitVecVal* prev_scatter_data_, bool gsc32,
                  uint64_t gsc32_indices, bool tex_send, bool tex_rcv, bool tensor_load, bool tenb,
                  uint32_t tensor_load_num_lines, bool tensor_fma, bool tensor_quant, bool reduce, bool tensor_store,
                  bool flb_valid, uint32_t flb_idx, uint32_t flb_range, bool wflags, unsigned trans_id);

void inst_exception(uint64_t cycle, unsigned minion_id, bool thread_id, uint64_t pc, int inst_bits, int wbf_addr,
                    bool is_store, bool is_atomic, bool is_gsc, uint64_t scatter_addr_, uint32_t scatter_data_,
                    svBitVecVal* prev_scatter_addr_, svBitVecVal* prev_scatter_data_, uint32_t store_mask,
                    uint32_t gsc_size, int gsc_progress, uint64_t mstatus, uint64_t cause, uint64_t tval, uint64_t epc,
                    uint64_t mip, uint64_t prv);

void esr_write(uint64_t addr, uint64_t data);
void icache_prefetch_done(uint32_t shire_id);

// tensor load
void tensorload_mon(uint64_t cycle_, uint32_t minion_id, bool tl_new_op, uint32_t tl_new_op_sel,
                    uint32_t tl_new_op_transform, uint32_t tl_new_op_num_lines, svBitVecVal* tl_busy_rising_,
                    svBitVecVal* tl_busy_falling_, svBitVecVal* tl_req_id_idx_, int32_t tl_resp_module,
                    int32_t tl_resp_id_idx, int32_t tl_resp_entry, svBitVecVal* tl_resp_data_, bool tl_trans_load_valid,
                    int32_t tl_trans_entry, int32_t tl_trans_banks, svBitVecVal* tl_trans_load_data_,
                    bool tl_tenb_flush);
}

#endif

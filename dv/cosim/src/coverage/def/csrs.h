/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Cache control
COV(et_cache_control, mcache_control)
COV(et_cache_control, evict_sw)
COV(et_cache_control, flush_sw)
COV(et_cache_control, lock_sw)
COV(et_cache_control, unlock_sw)
COV(et_cache_control, ucache_control)
COV(et_cache_control, prefetch_va)
COV(et_cache_control, evict_va)
COV(et_cache_control, flush_va)
COV(et_cache_control, lock_va)
COV(et_cache_control, unlock_va)

// Tensors
COV(et_tensor_coop, tensor_coop)
COV(et_tensor_error, tensor_error)
COV(et_tensor_wait, tensor_wait)

COV(et_tensor_mask, tensor_mask)
COV(et_tensor_mask, tensor_conv_size)
COV(et_tensor_mask, tensor_conv_ctrl)

COV(et_tensor_vpu, tensor_fma)
COV(et_tensor_vpu, tensor_quant)
COV(et_tensor_reduce, tensor_reduce)

COV(et_tensor_load, tensor_load)
COV(et_tensor_load, tensor_load_l2)
COV(et_tensor_store, tensor_store)

// Others
COV(et_fcc, fcc)
COV(et_fcc, fccnb)
COV(et_flb, flb)

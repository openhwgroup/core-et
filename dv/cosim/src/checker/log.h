/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _LOG_H
#define _LOG_H

#include <cstdint>
#include <algorithm>
#include <array>
#include <bitset>

#include "emu_defines.h"
#include "processor.h"
#include "state.h"

#include "support/multi_dim_array.h"
#include "state_change.h"

// Notify changes to state by an instruction
void notify_pc_update(const bemu::Hart& cpu, uint64_t new_pc);
void notify_xreg_write(const bemu::Hart& cpu, uint8_t xdst, uint64_t xval);
void notify_freg_write(const bemu::Hart& cpu, uint8_t fdst, const bemu::mreg_t&, const bemu::freg_t& fval);
void notify_mreg_write(const bemu::Hart& cpu, uint8_t mdst, const bemu::mreg_t& mval);
void notify_fflags_write(const bemu::Hart& cpu, uint64_t new_fflags);
void notify_mem_write(const bemu::Hart& cpu, bool modify, int size, uint64_t vaddr, uint64_t paddr, uint64_t val);
void notify_mem_read(const bemu::Hart& cpu, bool access, int size, uint64_t vaddr, uint64_t paddr);
void notify_mem_read_write(const bemu::Hart& cpu, bool modify, int size, uint64_t vaddr, uint64_t paddr, uint64_t val);
void notify_trap(const bemu::Hart& cpu, uint64_t mstatus, uint64_t cause, uint64_t tval, uint64_t epc);
void notify_gsc_progress(const bemu::Hart& cpu, uint64_t gsc_progress);

// We don't care if the write is late or not
inline void notify_xreg_late_write(const bemu::Hart& cpu, uint8_t xdst, uint64_t xval)
{
    notify_xreg_write(cpu, xdst, xval);
}

// Vector loads are same as vpu writes
inline void notify_freg_load(const bemu::Hart& cpu, uint8_t fdst, const bemu::mreg_t& mask, const bemu::freg_t& fval)
{
    notify_freg_write(cpu, fdst, mask, fval);
}

// Int-to-float moves
inline void notify_freg_intmv(const bemu::Hart& cpu, uint8_t fdst, const bemu::mreg_t& mask, const bemu::freg_t& fval)
{
    notify_freg_write(cpu, fdst, mask, fval);
}

// Freg reads - not tracked
inline void notify_freg_read(const bemu::Hart&, uint8_t) {}

// tensor_error
void notify_tensor_error_value(const bemu::Hart& cpu, uint16_t val);

// TensorLoad
void notify_tensor_load(const bemu::Hart& cpu, uint8_t trans, uint8_t tenb, uint8_t first, uint64_t conv_mask);
void notify_tensor_load_scp_write(const bemu::Hart& cpu, uint8_t line, const uint64_t* data);

// TensorFMA
void notify_tensor_fma_new_pass(const bemu::Hart& cpu);
void notify_tensor_fma_write(const bemu::Hart& cpu, uint8_t pass, bool vpu, uint8_t dst, int elem, uint32_t value);

// TensorQuant
void notify_tensor_quant_new_transform(const bemu::Hart& cpu, bool skip_write = false);
void notify_tensor_quant_write(const bemu::Hart& cpu, uint8_t trans, uint8_t freg, const bemu::mreg_t& mask,
                               const bemu::freg_t& fval);

// TensorReduce
void notify_tensor_reduce(const bemu::Hart& cpu, bool, uint8_t first, uint8_t count);
void notify_tensor_reduce_write(const bemu::Hart& cpu, uint8_t freg, const bemu::freg_t& fval);

// TensorStore
void notify_tensor_store(const bemu::Hart& cpu, bool scp, uint8_t rows, uint8_t cols, uint8_t coop);
void notify_tensor_store_write(const bemu::Hart& cpu, uint64_t addr, uint32_t data);
void notify_tensor_store_error(const bemu::Hart& cpu, uint16_t error);

// Support for log state
void setlogstate(inst_state_change* log_info_);
void clearlogstate();

#endif // _LOG_H

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "cache.h"
#include "log.h"

inst_state_change* log_info = NULL;

void clearlogstate()
{
    if (!log_info) return;
    log_info->pc_mod = false;
    log_info->int_reg_mod = false;
    log_info->fp_reg_mod = false;
    log_info->fflags_mod = false;
    log_info->tensor_mod = false;
    log_info->trap_mod = false;
    log_info->gsc_progress_mod = false;
    log_info->m_reg_mod.reset();
    log_info->mem_mod.reset();
    log_info->mem_read.reset();
    for (unsigned pos = 0; pos < bemu::VLENW; ++pos) {
        log_info->mem_size[pos] = 0;
    }
    log_info->fflags_value = 0;
    log_info->tl_scp_count = 0;
    log_info->tensorfma_passes = 0;
    log_info->tensorquant_trans = 0;
    log_info->tensorreduce_count = 0;
    log_info->tensorstore_count = 0;
    log_info->tensorstore_error = 0;
    log_info->tensor_error = 0;
}

void setlogstate(inst_state_change* log_info_)
{
    log_info = log_info_;
}

void notify_pc_update(const bemu::Hart&, uint64_t new_pc)
{
    assert(~new_pc & 1);
    if (!log_info) return;
    log_info->pc_mod = true;
    log_info->pc = new_pc;
}

void notify_xreg_write(const bemu::Hart&, uint8_t xdst, uint64_t xval)
{
    assert(xdst < bemu::NXREGS);
    if (!log_info) return;
    log_info->int_reg_mod = true;
    log_info->int_reg_rd = xdst;
    log_info->int_reg_data = xval;
}

void notify_freg_write(const bemu::Hart&, uint8_t fdst, const bemu::mreg_t&, const bemu::freg_t& fval)
{
    assert(fdst < bemu::NFREGS);
    if (!log_info) return;
    log_info->fp_reg_mod = true;
    log_info->fp_reg_rd = fdst;
    for (size_t i = 0; i < bemu::VLEND; i++) log_info->fp_reg_data[i] = fval.u64[i];
}

void notify_mreg_write(const bemu::Hart&, uint8_t mdst, const bemu::mreg_t& mval)
{
    assert(mdst < bemu::NMREGS);
    if (!log_info) return;
    log_info->m_reg_mod[mdst] = true;
    for (size_t i = 0; i < bemu::MLEN; i++) log_info->m_reg_data[mdst][i] = mval[i];
}

void notify_mem_write(const bemu::Hart&, bool modify, int size, uint64_t vaddr, uint64_t paddr, uint64_t val)
{
    if (!log_info) return;
    for (unsigned pos = 0; pos < bemu::VLENW; ++pos) {
        if (!log_info->mem_size[pos]) {
            log_info->mem_mod[pos] = modify;
            log_info->mem_size[pos] = size;
            log_info->mem_vaddr[pos] = vaddr;
            log_info->mem_paddr[pos] = paddr;
            log_info->mem_data[pos] = val;
            return;
        }
    }
    assert(0);
}

void notify_mem_read(const bemu::Hart&, bool access, int size, uint64_t vaddr, uint64_t paddr)
{
    if (!log_info) return;
    for (unsigned pos = 0; pos < bemu::VLENW; ++pos) {
        if (!log_info->mem_size[pos]) {
            log_info->mem_read[pos] = access;
            log_info->mem_size[pos] = size;
            log_info->mem_vaddr[pos] = vaddr;
            log_info->mem_paddr[pos] = paddr;
            return;
        }
    }
    assert(0);
}

void notify_mem_read_write(const bemu::Hart&, bool modify, int size, uint64_t vaddr, uint64_t paddr, uint64_t val)
{
    if (!log_info) return;
    for (unsigned pos = 0; pos < bemu::VLENW; ++pos) {
        if (!log_info->mem_size[pos]) {
            log_info->mem_mod[pos] = modify;
            log_info->mem_read[pos] = modify;
            log_info->mem_size[pos] = size;
            log_info->mem_vaddr[pos] = vaddr;
            log_info->mem_paddr[pos] = paddr;
            log_info->mem_data[pos] = val;
            return;
        }
    }
    assert(0);
}

void notify_fflags_write(const bemu::Hart&, uint64_t new_flags)
{
    if (!log_info) return;
    log_info->fflags_mod = true;
    log_info->fflags_value |= new_flags;
}

void notify_tensor_error_value(const bemu::Hart&, uint16_t val)
{
    if (!log_info) return;
    log_info->tensor_error = val;
}

void notify_tensor_load(const bemu::Hart&, uint8_t trans, uint8_t tenb, uint8_t first, uint64_t conv_mask)
{
    assert(trans < 8);
    assert(conv_mask <= 0xFFFF);
    if (!log_info) return;
    log_info->tensor_mod = true;
    log_info->tl_tenb = tenb;
    log_info->tl_scp_first = first;
    log_info->tl_scp_count = 0;
    switch (trans) {
    case 1:
    case 2:
        log_info->tl_transform = 2;
        break;
    case 5:
    case 6:
    case 7:
        log_info->tl_transform = 1;
        break;
    default:
        log_info->tl_transform = 0;
        break;
    }
    log_info->tl_conv_skip = ~conv_mask & 0xFFFF;
}

void notify_tensor_load_scp_write(const bemu::Hart&, uint8_t row, const uint64_t* data)
{
    assert(row < 16);
    if (!log_info) return;
    if (log_info->tl_scp_count <= row) log_info->tl_scp_count = row + 1;
    for (int j = 0; j < (L1D_LINE_SIZE / 4); j += 2) {
        log_info->tensordata[0][2 * row + (j + 0) / bemu::VLENW][(j + 0) % bemu::VLENW] = data[j / 2] & 0xffffffff;
        log_info->tensordata[0][2 * row + (j + 1) / bemu::VLENW][(j + 1) % bemu::VLENW] = (data[j / 2] >> 32)
                                                                                          & 0xffffffff;
    }
}

void notify_tensor_fma_new_pass(const bemu::Hart&)
{
    if (!log_info) return;
    uint8_t pass = log_info->tensorfma_passes++;
    assert(pass < TFMA_MAX_ACOLS);
    log_info->tensorfma_mod[pass] = 0;
    for (unsigned i = 0; i < bemu::NFREGS; ++i) {
        log_info->tensorfma_mask[pass][i].reset();
    }
}

void notify_tensor_fma_write(const bemu::Hart&, uint8_t pass, bool, uint8_t dst, int elem, uint32_t value)
{
    assert(pass < TFMA_MAX_ACOLS);
    assert(dst < bemu::NFREGS);
    assert(unsigned(elem) < bemu::VLENW);
    if (!log_info) return;
    log_info->tensorfma_mask[pass][dst][elem] = true;
    log_info->tensorfma_mod[pass] |= 1u << dst;
    log_info->tensordata[pass][dst][elem] = value;
}

void notify_tensor_quant_new_transform(const bemu::Hart&, bool skip_write)
{
    if (!log_info) return;
    uint8_t trans = log_info->tensorquant_trans++;
    assert(trans < TQUANT_MAX_TRANS);
    log_info->tensorquant_skip_write[trans] = skip_write;
    log_info->tensorquant_mod[trans] = 0;
    for (unsigned i = 0; i < bemu::NFREGS; ++i) {
        log_info->tensorquant_mask[trans][i].reset();
    }
}

void notify_tensor_quant_write(const bemu::Hart&, uint8_t trans, uint8_t freg, const bemu::mreg_t& mask,
                               const bemu::freg_t& fval)
{
    assert(trans < TQUANT_MAX_TRANS);
    assert(freg < bemu::NFREGS);
    if (!log_info) return;
    for (unsigned elem = 0; elem < bemu::MLEN; ++elem) {
        if (!mask[elem]) continue;
        log_info->tensorquant_mask[trans][freg][elem] = true;
        log_info->tensorquant_mod[trans] |= 1u << freg;
        log_info->tensordata[trans][freg][elem] = fval.u32[elem];
    }
}

void notify_tensor_reduce(const bemu::Hart&, bool receiver, uint8_t first, uint8_t count)
{
    assert(first < bemu::NFREGS);
    assert(count <= 127);
    if (!receiver || !log_info) return;
    log_info->tensorreduce_first = first;
    log_info->tensorreduce_count = count;
}

void notify_tensor_reduce_write(const bemu::Hart&, uint8_t freg, const bemu::freg_t& fval)
{
    assert(freg < bemu::NFREGS);
    if (!log_info) return;
    for (unsigned elem = 0; elem < bemu::VLENW; ++elem) {
        log_info->tensordata[0][freg][elem] = fval.u32[elem];
    }
}

void notify_tensor_store(const bemu::Hart&, bool scp, uint8_t rows, uint8_t cols, uint8_t coop)
{
    assert(rows <= bemu::NFREGS / 2);
    assert(cols <= 4);
    assert(coop <= 4);

    if (!log_info) return;
    log_info->tensorstore_scp = scp;
    log_info->tensorstore_count = rows;
    log_info->tensorstore_bytes = cols * 16;
    log_info->tensorstore_coop = coop;
    log_info->tensorstore_idx = 0;
    log_info->tensorstore_error = 0;
}

void notify_tensor_store_write(const bemu::Hart&, uint64_t addr, uint32_t data)
{
    if (!log_info) return;
    log_info->tensorstore_write_addr[log_info->tensorstore_idx] = addr;
    log_info->tensorstore_write_data[log_info->tensorstore_idx++] = data;
}

void notify_tensor_store_error(const bemu::Hart&, uint16_t error)
{
    if (!log_info) return;
    log_info->tensorstore_error = error;
}

void notify_trap(const bemu::Hart&, uint64_t mstatus, uint64_t cause, uint64_t tval, uint64_t epc)
{
    if (!log_info) return;
    log_info->trap_mod = true;
    log_info->mstatus = mstatus;
    log_info->cause = cause;
    log_info->tval = tval;
    log_info->epc = epc;
}

void notify_gsc_progress(const bemu::Hart&, uint64_t gsc_progress)
{
    if (!log_info) return;
    log_info->gsc_progress_mod = true;
    log_info->gsc_progress = gsc_progress;
}

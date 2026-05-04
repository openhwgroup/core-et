/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <iostream>

#include "cosim.h"
#include "events/cosim_evt.h"
#include "math.h"
#include "dispatcher.h"

#if 0
#include "emu.h"
#include "emu_gio.h"
#include "vlog_defines.h"
#endif

#include "minion_monitor.h"

/*
 * This file intercepts RTL DPI calls
 * and "converts" them into CoSIM calls
 */

DPI_IMPORT_NOCONTEXT(void, tbox_new_req, (unsigned int shire_id, unsigned int hart_id_dest, unsigned int port_id),
                     cosim, tbox_new_req, (shire_id, hart_id_dest, port_id));

DPI_IMPORT_NOCONTEXT(void, minion_new_msg,
                     (unsigned int shire_id, unsigned int shire_dest, unsigned int hart_id_src,
                      unsigned int hart_id_dest, unsigned int port_id, bool to_localnet),
                     cosim, minion_new_msg, (shire_id, shire_dest, hart_id_src, hart_id_dest, port_id, to_localnet));

DPI_IMPORT_NOCONTEXT(void, sc_local_new_msg,
                     (unsigned int shire_id, unsigned int neigh_id, unsigned int minion_src, unsigned int n_bank),
                     cosim, sc_local_new_msg, (shire_id, neigh_id, minion_src, n_bank));

DPI_IMPORT_NOCONTEXT(void, sc_local_new_msg_rbox,
                     (unsigned int shire_id, unsigned int rbox_local_id, unsigned int n_bank, unsigned int thread_dest,
                      unsigned int port_id, unsigned int* data, unsigned int oob),
                     cosim, sc_local_new_msg_rbox, (shire_id, rbox_local_id, n_bank, thread_dest, port_id, data, oob));

DPI_IMPORT_NOCONTEXT(void, sc_to_neigh_new_msg,
                     (unsigned int shire_id, unsigned int neigh_dest, unsigned int origin, bool is_uc), cosim,
                     sc_to_neigh_new_msg, (shire_id, neigh_dest, origin, is_uc));

DPI_IMPORT_NOCONTEXT(void, uc_new_msg_remote_req, (unsigned int shire_id_dest, unsigned int shire_id_origin), cosim,
                     uc_new_msg_remote_req, (shire_id_dest, shire_id_origin));

DPI_IMPORT_NOCONTEXT(void, uc_new_msg_req,
                     (unsigned int shire_id, unsigned int shire_dest, unsigned int neigh_id, unsigned int minion_src),
                     cosim, uc_new_msg_req, (shire_id, shire_dest, neigh_id, minion_src));

DPI_IMPORT_NOCONTEXT(void, neigh_localnet_msg,
                     (unsigned int shire_id, unsigned int neigh_id, unsigned int minion_dest, unsigned int minion_src),
                     cosim, neigh_localnet_msg, (shire_id, neigh_id, minion_dest, minion_src));

DPI_IMPORT_NOCONTEXT(void, neigh_fill_ff_msg,
                     (unsigned int shire_id, unsigned int neigh_id, bool is_localnet, bool is_tbox,
                      unsigned int minion_rcv, unsigned int thread_id),
                     cosim, neigh_fill_ff_msg, (shire_id, neigh_id, is_localnet, is_tbox, minion_rcv, thread_id));

////////////////////////////////////////////////////////////////
// DPI Functions
///////////////////////////////////////////////////////////////

void inst_retired(uint64_t cycle, uint32_t minion_id, bool thread_id, uint64_t pc, int inst_bits, bool delayed_write,
                  bool load_miss, bool wbx, int wbx_addr, uint64_t wbx_data, bool wbf,
                  int wbf_addr, /*svBitVecVal * wbf_data_,*/
                  bool wbm, bool wbmall, int wbm_addr, uint64_t wbm_data, bool store, int store_size,
                  uint64_t store_addr, svBitVecVal* store_data_, int store_mask, bool delayed_store, int store_age_id,
                  bool scatter, svBitVecVal* prev_scatter_addr_, svBitVecVal* prev_scatter_data_, bool gsc32,
                  uint64_t gsc32_indices, bool tex_send, bool tex_rcv, bool tensor_load, bool tenb,
                  uint32_t tensor_load_num_lines, bool tensor_fma, bool tensor_quant, bool reduce, bool tensor_store,
                  bool flb_valid, uint32_t flb_idx, uint32_t flb_range, bool wflags, unsigned trans_id, uint32_t prv)
{
    //uint64_t store_data[bemu::VLEND];
    //uint64_t scatter_addr[bemu::VLENW];
    //uint32_t scatter_data[bemu::VLENW];

    cosim_evt::inst_retire_t evt{};
    evt.cycle = cycle;
    evt.minion_id = minion_id;
    evt.thread_id = thread_id;
    evt.pc = pc;
    evt.inst_bits = inst_bits;
    evt.delayed_write = delayed_write;
    evt.load_miss = load_miss;
    evt.wbx = wbx;
    evt.wbx_rd = wbx_addr;
    evt.wbx_data = wbx_data;
    evt.wbf = wbf;
    evt.wbf_rd = wbf_addr;
    evt.wbm = wbm;
    evt.wbmall = wbmall;
    evt.wbm_rd = wbm_addr;
    evt.wbm_data = wbm_data;
    evt.store = store;
    evt.store_size = store_size;
    evt.store_addr = store_addr;
    evt.store_mask = store_mask;
    evt.delayed_store = delayed_store;
    evt.store_age_id = store_age_id;
    evt.scatter = scatter;
    evt.prv = prv;

    if (store) {
        for (unsigned int i = 0; i < bemu::VLEND; ++i) {
            evt.store_data[i] = dpi2cpp(store_data_, i * 64, 64);
        }

        for (unsigned int i = 0; i < bemu::VLENW - 1; ++i) {
            evt.scatter_addr[i] = dpi2cpp(prev_scatter_addr_, 64 * i, 64);
            evt.scatter_data[i] = dpi2cpp(prev_scatter_data_, 32 * i, 32);
        }
        evt.scatter_addr[bemu::VLENW - 1] = store_addr;
        evt.scatter_data[bemu::VLENW - 1] = dpi2cpp(store_data_, 0, 32);

        if (gsc32) {
            uint64_t base_addr = store_addr;

            for (unsigned int i = 0; i < bemu::VLENW; ++i) {
                evt.scatter_data[i] = dpi2cpp(store_data_, 32 * i, 32);
                switch (store_size) {
                case 1:
                    evt.scatter_addr[i]
                        = (base_addr & ~0x1f) | ((((gsc32_indices >> (5 * i)) & 0x1f) + (base_addr & 0x1f)) & 0x1f);
                    break;
                case 2:
                    evt.scatter_addr[i] = (base_addr & ~0x1f)
                                          | (((((gsc32_indices >> (4 * i)) & 0xf) << 1) + (base_addr & 0x1e)) & 0x1f);
                    break;
                case 4:
                    evt.scatter_addr[i] = (base_addr & ~0x1f)
                                          | (((((gsc32_indices >> (3 * i)) & 0x7) << 2) + (base_addr & 0x1c)) & 0x1f);
                    break;
                }
            }
            evt.scatter = true;
        }
    }

    evt.tex_send = tex_send;
    evt.tex_rcv = tex_rcv;
    evt.tensor_load = tensor_load;
    evt.tenb = tenb;
    evt.tensor_load_num_lines = tensor_load_num_lines;
    evt.tensor_fma = tensor_fma;
    evt.tensor_quant = tensor_quant;
    evt.reduce = reduce;
    evt.tensor_store = tensor_store;
    evt.flb_valid = flb_valid;
    evt.flb_idx = flb_idx;
    evt.flb_range = flb_range;
    evt.wflags = wflags;
    evt.trans_id = trans_id;

    dispatcher::instance()->dispatch(&evt);
}

void inst_exception(uint64_t cycle, unsigned minion_id, bool thread_id, uint64_t pc, int inst_bits, int wbf_addr,
                    bool is_store, bool is_atomic, bool is_gsc, uint64_t scatter_addr_, uint32_t scatter_data_,
                    svBitVecVal* prev_scatter_addr_, svBitVecVal* prev_scatter_data_, uint32_t store_mask,
                    uint32_t gsc_size, int gsc_progress, uint64_t mstatus, uint64_t cause, uint64_t tval, uint64_t epc,
                    uint64_t mip, uint32_t prv)
{
    cosim_evt::inst_exception_t evt{};
    evt.cycle = cycle;
    evt.minion_id = minion_id;
    evt.thread_id = thread_id;
    evt.pc = pc;
    evt.inst_bits = inst_bits;
    evt.wbf_rd = wbf_addr;
    evt.is_store = is_store;
    evt.is_atomic = is_atomic;
    evt.is_gsc = is_gsc;

    for (unsigned int i = 0; i < bemu::VLENW - 1; ++i) {
        evt.scatter_addr[i] = dpi2cpp(prev_scatter_addr_, 64 * i, 64);
        evt.scatter_data[i] = dpi2cpp(prev_scatter_data_, 32 * i, 32);
    }
    evt.scatter_addr[bemu::VLENW - 1] = scatter_addr_;
    evt.scatter_data[bemu::VLENW - 1] = scatter_data_;

    evt.store_mask = store_mask;
    evt.gsc_size = gsc_size;
    evt.gsc_progress = gsc_progress;
    evt.mstatus = mstatus;
    evt.cause = cause;
    evt.tval = tval;
    evt.epc = epc;
    evt.mip = mip;
    evt.prv = prv;

    dispatcher::instance()->dispatch(&evt);
}

// ESR Write due to backdoor access into RTL
void esr_write(uint64_t address, uint64_t data)
{
    cosim_evt::esr_write_t evt;
    evt.addr = address;
    evt.data = data;
    try {
        dispatcher::instance()->dispatch(&evt);
    } catch (...) {
        // TODO: ANT do propper logging
        std::cout << "WARN: " << "ESR[" << std::hex << "0x" << evt.addr << "]=" << "0x" << evt.data << " xcpt in DPI" << std::dec << std::endl;
    }
}

void icache_prefetch_done(uint32_t shire_id)
{
    cosim_evt::icache_prefetch_done_t evt;
    evt.shire_id = shire_id;
    dispatcher::instance()->dispatch(&evt);
}

////////////////////////////////////////////////////////////////
// tensor load DPI
////////////////////////////////////////////////////////////////
void tensorload_mon(uint64_t cycle, uint32_t minion_id, bool tl_new_op, uint32_t tl_new_op_sel,
                    uint32_t tl_new_op_transform, uint32_t tl_new_op_num_lines, svBitVecVal* tl_busy_rising_,
                    svBitVecVal* tl_busy_falling_, svBitVecVal* tl_req_id_idx_, int32_t tl_resp_module,
                    int32_t tl_resp_id_idx, int32_t tl_resp_entry, svBitVecVal* tl_resp_data_, bool tl_trans_load_valid,
                    int32_t tl_trans_entry, int32_t tl_trans_banks, svBitVecVal* tl_trans_load_data_,
                    bool tl_tenb_flush)

{

    bool tl_busy_rising[2], tl_busy_falling[2];

    // busy rising
    for (int i = 0; i < 2; i++) {
        tl_busy_rising[i] = dpi2cpp(tl_busy_rising_, i * 1, 1);
        if (tl_busy_rising[i] == 1) {
            cosim_evt::tl_mon_process_busy_t evt;
            evt.cycle = cycle;
            evt.minion_id = minion_id;
            evt.tl_module = i;
            evt.state = 1;
            dispatcher::instance()->dispatch(&evt);
        }
    }

    // process l2 reqs
    for (int i = 0; i < 2; i++) {
        cosim_evt::tl_mon_process_req_t evt;
        evt.cycle = cycle;
        evt.minion_id = minion_id;
        evt.tl_module = i;
        evt.req_id_idx = dpi2cpp(tl_req_id_idx_, i * 32, 32); // tl_req_id_idx[i] : 0-3
        if (evt.req_id_idx != -1) {
            dispatcher::instance()->dispatch(&evt);
        }
    }

    // process l2 resp - tl_resp_id_idx : 0-3
    if (tl_resp_module != -1) {
        cosim_evt::tl_mon_process_resp_t evt;
        evt.cycle = cycle;
        evt.minion_id = minion_id;
        evt.tl_module = tl_resp_module;
        evt.resp_id_idx = tl_resp_id_idx;
        evt.entry = tl_resp_entry;
        for (int i = 0; i < 4; i++) {
            evt.data[i] = dpi2cpp(tl_resp_data_, i * 64, 64);
        }
        dispatcher::instance()->dispatch(&evt);
    }

    // process tranform data xfer
    if (tl_trans_load_valid) {
        cosim_evt::tl_mon_process_trans_data_xfer_t evt;
        evt.cycle = cycle;
        evt.minion_id = minion_id;
        evt.entry = tl_trans_entry;
        evt.banks = tl_trans_banks;
        for (int i = 0; i < 4; i++) {
            evt.data[i] = dpi2cpp(tl_trans_load_data_, i * 64, 64);
        }
        dispatcher::instance()->dispatch(&evt);
    }

    // process tenb_flush
    if (tl_tenb_flush) {
        cosim_evt::tl_mon_process_tenb_flush_t evt;
        evt.cycle = cycle;
        evt.minion_id = minion_id;
        dispatcher::instance()->dispatch(&evt);
    }

    // new op
    if (tl_new_op) {
        cosim_evt::tl_mon_op_start_t evt;
        evt.cycle = cycle;
        evt.minion_id = minion_id;
        evt.tl_module = tl_new_op_sel;
        evt.transform = tl_new_op_transform;
        evt.num_lines = tl_new_op_num_lines;
        dispatcher::instance()->dispatch(&evt);
    }

    // busy falling
    for (int i = 0; i < 2; i++) {
        tl_busy_falling[i] = dpi2cpp(tl_busy_falling_, i * 1, 1);
        if (tl_busy_falling[i] == 1) {
            cosim_evt::tl_mon_process_busy_t evt;
            evt.cycle = cycle;
            evt.minion_id = minion_id;
            evt.tl_module = i;
            evt.state = 2;
            dispatcher::instance()->dispatch(&evt);
        }
    }
}

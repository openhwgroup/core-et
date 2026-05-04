/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _COSIM_INSN_H_
#define _COSIM_INSN_H_

#include <list>
#include <vector>
#include <deque>

#include "cache.h"
#include "checker/log.h"

#include "support/hart_id.h"
#include "support/insn_desc.h"

struct cosim_pending_msg {
    uint64_t shire_org;    // shire id (0..32)
    uint64_t shire_dest;   // shire id (0..32)
    uint32_t hart_id_src;  // sender minion id
    uint32_t hart_id_dest; // shire_id ,bits in address [19:12]
    uint32_t port_id;      // bits in address [5:4]
    bool is_tbox;
    bool is_rbox;
};

// Struct that holds the information of a retired tensor load instruction
struct cosim_tensor_load_entry {
    uint32_t banks;
    int entry;
    uint64_t data[L1D_LINE_SIZE / 8];
};

// Struct that holds the information of a retired instruction or msgs that is
// pending to be checked against the EMU
struct cosim_insn {
    enum class type {
        INSTRUCTION,
        EXCEPTION,
        INTERRUPT,
        PENDING_MSG, // not a instruction, is a pending message from self/other minion
        IPI
    };

    insn_desc desc;
    class hart_id hart;

    cosim_insn::type type_id;       // Type of events (instruction/interrupt/msg/ipi...)
    base_state_change state_change; // State changes of the instruction
    bool delayed_write;             // If the instruction has a delayed write
    bool delayed_store;             // If the instruction has a delayed store due a miss
    bool load_miss;                 // Load Miss instruction

    bool tensor_fma;   // Tensor FMA
    bool tensor_quant; // Tensor Quant

    // Tensor reduce
    bool reduce; // Reduce
    unsigned reduce_partner;
    bool reduce_partner_done;
    uint8_t reduce_action;

    // Tensor store
    bool tensor_store;

    // Fast Local Barrier
    bool is_flb;
    int flb_return_val;    // FLB: Return value from UC::FLB block
    int flb_min_to_wakeup; // FLB: Minion Master in Barrier Idx to wake up
    uint8_t flb_dest;      // FLB: Dest register update
    int flb_idx;           // FLB: Barrier ID
    int flb_match;         // FLB: Barrier Match

    uint32_t store_age_id; // Age ID of the delayed store
    uint8_t delayed_addr;  // Addr of the register of the delayed write
    bool pending_fcsr;     // If instruction flags didn't arribe
    bool trans;            // Its a trans instruction
    unsigned trans_status; // Trans instruction type

    // Messages
    cosim_pending_msg rmt_msg_info; // msg info

    // IPI
    uint64_t ipi_pc; // IPI info

    // Tensor load
    bool tensor_load;
    bool tenb;
    unsigned tensor_load_num_lines;
    unsigned tensor_load_op_id;
    bool tensor_load_done;
    bool tensor_load_tenb_flush;
    unsigned tensor_load_num_hword;
    std::list<cosim_tensor_load_entry>* tensor_load_entry_list = NULL;

    inline bool is_ready() const
    {
        return !(delayed_write || delayed_store || tensor_load || tensor_fma || tensor_quant || reduce || pending_fcsr
                 || tensor_store);
    }
};

// iterator type - for cosim::pending_queue[]
typedef std::deque<cosim_insn>::iterator cosim_insn_queue_it_t;

// Struct that stores statistics info
struct cosim_minion_stat {
    uint32_t num_instr;
    uint32_t num_instr_thread[2];
    uint32_t num_ipi;
    uint32_t num_fma;
    uint32_t num_fma_transactions;
    uint32_t num_tensor_load[2];
    uint32_t num_tensor_load_transactions[2];
    uint32_t num_tensor_store;
    uint32_t num_tensor_store_transactions;
    uint32_t num_tensor_reduce;
    uint32_t num_tensor_reduce_transactions;
    uint32_t num_tensor_quant;
    uint32_t num_tensor_quant_transactions;
    uint32_t num_cache_op_prefetch; // not implemented yet
};

// Struct that stores tensor load monitor infos
struct cosim_tl_op {
    uint32_t op_id; // operation id
    uint32_t state;
    uint32_t req_state[4];
    bool is_transform;
    uint32_t num_req;
    uint32_t num_resp;
    uint32_t num_data_xfer;
    bool tenb_flush;
    uint32_t dw_i;
    insn_desc desc;
};

#endif

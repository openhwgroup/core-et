/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

// Base state changes
struct base_state_change {
    // Custom bitmasks
    using vl_mask = std::bitset<bemu::VLENW>;
    using mreg_mask = std::bitset<bemu::NMREGS>;

    // Template short-hands
    template <typename T>
    using per_lane = std::array<T, bemu::VLENW>;
    template <typename T>
    using per_mreg = std::array<T, bemu::NMREGS>;
    // Buffer for freg change
    using freg_buf = std::array<uint64_t, bemu::VLEND>;

    uint64_t cycle;     // Cycle SnapShot when event happened
    uint64_t pc;        // PC of instruction being executed
    uint32_t inst_bits; // Bits of instruction being executed

    // Was any state modified?
    bool pc_mod;      // Is a jump instruction
    bool int_reg_mod; // If an integer register was modified by instruction
    bool fp_reg_mod;  // If a float register was modified by instruction
    bool fflags_mod;  // Fcsr flags were updated by this instruction
    bool tensor_mod;  // Tensor load produced new values
    bool trap_mod;    // The instruction trapped
    bool gsc_progress_mod;
    mreg_mask m_reg_mod; // If a mask register was modified by instruction
    vl_mask mem_mod;     // If a memory position was updated
    vl_mask mem_read;    // If a memory position was read

    // Integer modifications
    int int_reg_rd;        // The destination integer register
    uint64_t int_reg_data; // Value written to the integer register

    // FP modifications
    int fp_reg_rd;        // The destination floating register
    freg_buf fp_reg_data; // Value written to the floating register

    // Memory modifications
    per_lane<int> mem_size;       // Size of the memory update
    per_lane<uint64_t> mem_vaddr; // Virtual address being accessed
    per_lane<uint64_t> mem_paddr; // Physical address being accessed
    per_lane<uint64_t> mem_data;  // New memory contents

    // Others
    uint64_t fflags_value;             // Fcsr flags new value
    per_mreg<bemu::mreg_t> m_reg_data; // Value written to the mask register

    // Trap CSR change
    uint64_t mstatus;      // mstatus
    uint64_t cause;        // [m|s]cause
    uint64_t mip;          // mip
    uint64_t tval;         // [m|s]tval
    uint64_t epc;          // [m|s]epc
    uint64_t prv;          // prv
    bool bus_error;        // BUS Error was set
    uint64_t bus_err_addr; // BUS Error address
    // uint64_t tvec;        // vec
    uint64_t gsc_progress; // in case that we trap during a gather/scatter we need to save gsc_progress csr to check it
};

// Extends base_state_change with extra fields (mostly for tensor ops)
struct inst_state_change : public base_state_change {
    using tquant_mask = std::bitset<TQUANT_MAX_TRANS>;
    template <size_t N>
    using freg_mask = std::array<uint32_t, N>;
    template <size_t N>
    using freg_lane_mask = multi_dim_array<vl_mask, N, bemu::NFREGS>;

    static constexpr auto tdata_width = TQUANT_MAX_TRANS > TFMA_MAX_ACOLS ? TQUANT_MAX_TRANS : TFMA_MAX_ACOLS;
    using tdata_buf = multi_dim_array<uint32_t, tdata_width, bemu::NFREGS, bemu::VLENW>;

    using tstore_buf = std::array<uint64_t, bemu::NFREGS* bemu::VLENW>;

    // TensorLoad state change
    // FUTURE Review the following types
    uint8_t tl_transform; // 0: No transform, 1: Interleave 2: Transpose (Can this be an enum?)
    uint8_t tl_tenb;      // tensorLoadSetupB (Can this be a bool?)
    uint8_t tl_scp_first;
    uint8_t tl_scp_count;
    uint16_t tl_conv_skip;

    // TensorFMA state change
    uint8_t tensorfma_passes;                      // Number of TFMA passes
    freg_mask<TFMA_MAX_ACOLS> tensorfma_mod;       // Mask which freg has been modified
    freg_lane_mask<TFMA_MAX_ACOLS> tensorfma_mask; // Mask which lane in a freg has been modified

    // TensorQuant state change
    uint8_t tensorquant_trans;                         // Number of TQUANT transformations
    tquant_mask tensorquant_skip_write;                // Mask whether TQUANT skips write
    freg_mask<TQUANT_MAX_TRANS> tensorquant_mod;       // Mask which freg has been modified
    freg_lane_mask<TQUANT_MAX_TRANS> tensorquant_mask; // Mask which lane in a freg has been modified

    // TensorReduce state change
    uint8_t tensorreduce_first;
    uint8_t tensorreduce_count;

    // TensorLoad, TensorFMA, TensorQuant, and TensorReduce values
    tdata_buf tensordata; // Shared buffer between tensor ops

    // Tensor store addresses and data
    tstore_buf tensorstore_write_addr;
    tstore_buf tensorstore_write_data;
    uint32_t tensorstore_idx;

    // TensorStore state change
    bool tensorstore_scp;
    uint8_t tensorstore_count;
    uint8_t tensorstore_bytes;
    uint8_t tensorstore_coop;
    uint16_t tensorstore_error;

    // tensor_error change
    uint16_t tensor_error;

    // Copy fields from base
    inst_state_change& operator=(const base_state_change& rhs)
    {
        base_state_change::operator=(rhs);
        return *this;
    }
};

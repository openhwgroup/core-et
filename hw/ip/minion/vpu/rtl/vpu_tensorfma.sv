// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off LATCH */  // Imported original TensorFMA control intentionally retains combinational state in this block; preserve the original latchy behavior here.
/* verilator lint_off UNOPTFLAT */  // TensorFMA shares preserved dcache/VPU control feedback that Verilator over-flattens into false loops.
/* verilator lint_off WIDTHEXPAND */  // Original tensor-FMA helper arithmetic mixes packed field widths and macro-sized constants.
/* verilator lint_off UNUSEDSIGNAL */  // Several packed control/debug fields are intentionally carried through this imported block without every bit being consumed.
module vpu_tensorfma
(
    // System signals
    input  logic                                        clock,
    input  logic                                        reset,
    // Control port from core to start new TensorFMA
    input  core_vpu_ctrl                                f0_core_ctrl,
    output logic                                        enabled,
    // Dependencies with other units
    input  logic                                        tensorquant_pend,
    input  logic                                        tensorstore_pend,
    // Requests to read data to dcache
    input  dcache_vpu_scp_resp                          dcache_scp_resp,
    output vpu_dcache_ctrl                              dcache_ctrl,
    // Signals going to VPU/Load control
    output vpu_ml_load_ctrl                             load_ctrl,
    output logic                                        load_ctrl_pre_tena_wen,
    output logic                                        load_ctrl_pre_tenb_wen,
    output logic                                        fma_en_next,
    output logic                                        fma_en,
    output logic [INST_SIZE-1:0]                          fma_inst_next,
    output logic [VPU_LANE_NUM-1:0]                    fma_mask_next,
    input  logic [INST_SIZE-1:0]                          fma_inst,
    output logic                                        ex_tena_rdata_mask,
    // Sync with Core
    output logic                                        pending_vpu_rf_write,
    // Sync with Core TensorQuant/TensorReduce
    output logic                                        pending_vpu_tenc_vrf_write,
    // TensorTMP control
    output logic                                        f2_tentmp_wen_early,
    output logic                                        f3_tentmp_wen,
    output logic [VPU_TENTMP_ADDR_W-1:0]                     f3_tentmp_waddr,
    output logic                                        ex_tentmp_ren,
    output logic [VPU_TENTMP_ADDR_W-1:0]                     ex_tentmp_raddr,
    // Control straight to TIMA (bypasses VPU CTRL)
    output logic                                        ex_tima_valid,
    output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_mask,
    output logic [VPU_TENA_ADDR_W-1:0]                       ex_tima_tena_raddr,
    output logic [VPU_TENB_ADDR_W-1:0]                       ex_tima_tenb_raddr,
    output logic                                        ex_tima_last_pass,
    output logic                                        ex_tima_ren3,
    output logic                                        ex_tima_sa,
    output logic                                        ex_tima_sb,
    output logic                                        f2_tima_ren3,
    output logic [VPU_TENC_ADDR_MSB:1]                       f2_tima_tenc_raddr,
    output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_tenc_wen,
    output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_rf_wen,
    output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_tenc_wen,
    output logic [VPU_TENC_ADDR_MSB:1]                       f3_tima_tenc_waddr,
    output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_rf_wen,
    // Performance counters
    output logic                                        io_events_wait_tenb,
    // debug
    output logic [24:0]                                 vpu_tensorfma_debug
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TensorFMA
// This instruction computes an A x B = C matrix multiply operation. A is
// 16 x 16, B is 16 x 16 and C is 16 x 16 (for FP32 operation). The instruction
// gets the A and B data from the scratchpad and accumulates the result in the
// FP registers (C matrix).
// It is expected that all the data has already been prefetched when the
// operation starts. It sends read commands straight to the LRAM arrays in the
// dcache and gets the data from there. It doesn't go through aligners or
// other logic to minimize power.
// In parallel and in sync, this unit generates FMA operations to correctly
// execute the operation.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Request control and state
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

logic [$clog2(VPU_TENA_SIZE):0]     dc_read_a_req_sent;           // Count of request for A sent
logic [(VPU_TENB_SIZE/2)-1:0][1:0]  dc_read_b_req_sent;           // If a request for B has been sent
logic [TENSOR_MASK_BITS-1:0]        conv_bits;                    // Convolution bits (which loads are done and which are skipped)
logic [2:0]                          fma_exec_cd;                  // Count down from last exec op to clear enabled bit
logic [2:0]                          fma_exec_cd_next;
logic                                enabled_next;                 // If the ML is enabled and performing an TensorFMA
logic                                start;                        // Starting a new request
logic                                working;                      // Doing work
logic                                dc_read_doing_a;              // Doing an A dcache request
logic                                dc_read_doing_b;              // Doing an B dcache request
logic                                s0_fma_en;                    // FMA enable 2 cycles earlier than execution
logic                                first_pass;                   // Doing a first pass operation, so do MUL instead of FMA
logic                                first_pass_next;              // Computes the next first pass value
logic                                first_pass_clear;             // Clear the first pass bit
logic                                conv_bits_up;                 // Update the convolution bits
logic                                conv_bits_pending;            // Pending to receive the convolution bits
logic                                conv_bits_pending_next;
logic [$clog2(VPU_TENB_SIZE/2)-1:0] fma_exec_b_rd_ptr_next;
logic                                done_sticky;                  // The TensorFMA issuing is done
logic                                req_int8;                     // INT8 request
logic                                scoreboard_pend_sticky;       // Waits for previous Scoreboards to finish writes to VPU RF before starting TensorFMA
logic                                scoreboard_pend_sticky_next;  //
logic                                tquant_pend_sticky;           // Waits for previous TensorQuant to finish writes to VPU RF before starting TensorFMA
logic                                tquant_pend_sticky_next;      //
logic                                tstore_pend_sticky;           // Waits for previous TensorStore to finish writes/reads to VPU RF before starting TensorFMA
logic                                tstore_pend_sticky_next;      //
logic [VPU_TENTMP_SIZE-1:0]         tensortmp_rf_written_next;    // Tensor tmp rf written marker
logic [VPU_TENTMP_SIZE-1:0]         tensortmp_rf_written;         // Tensor tmp rf written marker
logic [4:0]                          tensorfma_ctrl_cols_start_a;  // Number of cols+start of A matrix to be computed
logic                                pending_vpu_rf_write_next;    // Pending VPU RF write next
logic                                pending_vpu_tenc_vrf_write_next; // Pending TENC/VPU RF write next
logic [TXFMA_CLOCK_EN_DELAY-1:0]    txfma_clock_valid_delay;      // TXFMA clock valid delay
logic                                txfma_clock_valid_delay_en;   // TXFMA clock valid enable
logic [TXFMA_CLOCK_EN_DELAY-1:0]    txfma_clock_valid_delay_next; // TXFMA clock valid delay next
logic                                clock_en_txfma;               // TXFMA clock enable
logic [TIMA_CLOCK_EN_DELAY-1:0]     tima_clock_valid_delay;       // TIMA clock valid delay
logic                                tima_clock_valid_delay_en;    // TIMA clock valid enable
logic [TIMA_CLOCK_EN_DELAY-1:0]     tima_clock_valid_delay_next;  // TIMA clock valid delay next
logic                                clock_en_tima;                // TIMA clock enable

logic                                enabled_int;                  // Internal version of the "enabled" signals
logic [TFMA_ENABLED_FLUSH_BITS-1:0] enabled_flush;                // Delay to allow flushing the pipeline when leaving the enabled state
logic [TFMA_ENABLED_FLUSH_BITS-1:0] enabled_flush_next;
logic                                enabled_flush_up;


always_ff @(posedge clock) begin
    if (reset) begin
        txfma_clock_valid_delay <= {TXFMA_CLOCK_EN_DELAY{1'b0}};
        tima_clock_valid_delay <= {TIMA_CLOCK_EN_DELAY{1'b0}};
    end else begin
        if (txfma_clock_valid_delay_en) begin
            txfma_clock_valid_delay <= txfma_clock_valid_delay_next;
        end
        if (tima_clock_valid_delay_en) begin
            tima_clock_valid_delay <= tima_clock_valid_delay_next;
        end
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        enabled_int <= 1'b0;
        fma_exec_cd <= 3'b0;
        scoreboard_pend_sticky <= 1'b0;
        tquant_pend_sticky <= 1'b0;
        tstore_pend_sticky <= 1'b0;
    end else if (working) begin
        enabled_int <= enabled_next;
        fma_exec_cd <= fma_exec_cd_next;
        scoreboard_pend_sticky <= scoreboard_pend_sticky_next;
        tquant_pend_sticky <= tquant_pend_sticky_next;
        tstore_pend_sticky <= tstore_pend_sticky_next;
    end
end

always_ff @(posedge clock) begin
    if (working) begin
        first_pass <= first_pass_next;
    end
    if (conv_bits_up) begin
        conv_bits <= f0_core_ctrl.tensorfma_conv_bits;
    end
    conv_bits_pending <= conv_bits_pending_next;
end

always_ff @(posedge clock) begin
    if (reset) begin
        pending_vpu_rf_write <= 1'b0;
        pending_vpu_tenc_vrf_write <= 1'b0;
    end else begin
        pending_vpu_rf_write <= pending_vpu_rf_write_next;
        pending_vpu_tenc_vrf_write <= pending_vpu_tenc_vrf_write_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        enabled_flush <= {TFMA_ENABLED_FLUSH_BITS{1'b0}};
    end else if (enabled_flush_up) begin
        enabled_flush <= enabled_flush_next;
    end
end

// Pipeline flush: indicate that TFMA is enabled for longer
always_comb
begin
 enabled = enabled_int | enabled_flush[TFMA_ENABLED_FLUSH_BITS-1];

 enabled_flush_next = {enabled_flush[TFMA_ENABLED_FLUSH_BITS-2:0],enabled_int};
 enabled_flush_up   = enabled_int | (|enabled_flush);
end

  //clock en
always_comb
begin
    //txfma
  txfma_clock_valid_delay_en   = (enabled_int & ~req_int8) | txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-1];
  txfma_clock_valid_delay_next = (enabled_int & ~req_int8) ? {TXFMA_CLOCK_EN_DELAY{1'b1}}
                               : {txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-2:0],(enabled_int & ~req_int8)};
  clock_en_txfma = (enabled_int & ~req_int8) | (txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-1]);

    //tima
  tima_clock_valid_delay_en   = (enabled_int & req_int8) | tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-1];
  tima_clock_valid_delay_next = (enabled_int & req_int8) ? {TIMA_CLOCK_EN_DELAY{1'b1}}
                               : {tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-2:0],(enabled_int & req_int8)};
  clock_en_tima = (enabled_int & req_int8) | (tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-1]);
end


always_comb
begin
    // Keep values
    enabled_next         = enabled_int;
    start                = 1'b0;
    first_pass_next      = first_pass;

    // New request
    if(f0_core_ctrl.tensorfma_start)
    begin
        enabled_next    = 1'b1;
        start           = 1'b1;
        first_pass_next = f0_core_ctrl.tensorfma_ctrl.first_pass;
    end

    // Done when FMAs finish
    if(enabled_int && done_sticky && !s0_fma_en && !fma_en_next && !(|fma_exec_cd))
      enabled_next = 1'b0;

      //working signal used to clock gates FF's
    working = start || enabled_int || reset || clock_en_txfma || clock_en_tima;

    // Decrements count down unless a new exec op is sent
    fma_exec_cd_next = fma_exec_cd - 3'b1;
    if(fma_exec_cd > 3'b000)
        fma_exec_cd_next = 3'b000;
    if(fma_en)
        fma_exec_cd_next = 3'b100;

    // Clears the first pass bit
    if(first_pass_clear)
        first_pass_next = 1'b0;

    // Updates the convolution bits. If upon a new request they are ready,
    // they are captured. Otherwise waits until they are ready
    conv_bits_up = 1'b0;
    if(start && f0_core_ctrl.tensorfma_conv_bits_ready)
        conv_bits_up = 1'b1;

    // Updates the pending convolution bits signal
    conv_bits_pending_next = conv_bits_pending;
    if(start)
    begin
        // If new tensor load and bits are ready, they are not pending
        if(f0_core_ctrl.tensorfma_conv_bits_ready)
            conv_bits_pending_next = 1'b0;
        // If new tensor loads and bits are not ready, they are pending to be
        // ready
        else
            conv_bits_pending_next = 1'b1;
    end

    // If bits are pending and they get ready, update contents and remove
    // pending bit
    if(conv_bits_pending && f0_core_ctrl.tensorfma_conv_bits_ready)
    begin
        conv_bits_pending_next = 1'b0;
        conv_bits_up           = 1'b1;
    end

      // This signal is used by the intpipe to stall FP instructions targeting VPU RF
      // while there is an ongoing tensor FMA writing to VPU RF
    pending_vpu_rf_write_next = enabled_next || (working && (!req_int8 || f0_core_ctrl.tensorfma_ctrl.to_vrf));
    pending_vpu_tenc_vrf_write_next = working;

    // Computes sticky bit for tensorfma write pending
    scoreboard_pend_sticky_next = scoreboard_pend_sticky;
    tquant_pend_sticky_next     = tquant_pend_sticky;
    tstore_pend_sticky_next     = tstore_pend_sticky;
    // Get if pending RF write/read dependency
    if(start)
    begin
        scoreboard_pend_sticky_next = f0_core_ctrl.pending_scoreboard_t0;
        tquant_pend_sticky_next     = tensorquant_pend;
        tstore_pend_sticky_next     = tensorstore_pend;
    end
    else
    begin
        // Clear sticky bit when write done
        if(enabled_int && scoreboard_pend_sticky && !f0_core_ctrl.pending_scoreboard_t0)
            scoreboard_pend_sticky_next = 1'b0;
        // Clear sticky bit when write done
        if((enabled_int && tquant_pend_sticky && !tensorquant_pend) || f0_core_ctrl.tensorquant_start)
            tquant_pend_sticky_next = 1'b0;
        // Clear sticky bit when write done
        if((enabled_int && tstore_pend_sticky && !tensorstore_pend) || f0_core_ctrl.tensorstore_start)
            tstore_pend_sticky_next = 1'b0;
    end
end


always_comb tensorfma_ctrl_cols_start_a = (f0_core_ctrl.tensorfma_ctrl.start_a + f0_core_ctrl.tensorfma_ctrl.cols_a);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Dcache request control
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

logic [$clog2(DCACHE_WAYS*DCACHE_SETS)+5:0] dc_read_addr_flat;           // LRAM dcache flat addr
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)+5:0] dc_read_addr_mod;            // LRAM dcache modified addr
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dc_read_scp_dest;            // Scratchpad destination index
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dc_read_scp_a_addr;          // Scratchpad start a addr
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dc_read_scp_b_addr;          // Scratchpad start b addr
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dc_read_scp_a_addr_next;     // Scratchpad start a addr next
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dc_read_scp_b_addr_next;     // Scratchpad start b addr next
logic [DCACHE_SET_IDX_WIDTH-1:0]                     dc_read_scp_dest_set;        // Scratchpad destination set
logic [DCACHE_WAY_IDX_WIDTH-1:0]                     dc_read_scp_dest_way;        // Scratchpad destination way
logic [5:0]                                   dc_read_a_col_adj;           // Adjusted based on FP size
logic [3:0]                                   dc_read_a_col;               // Which A col we are about to request
logic [3:0]                                   dc_read_a_row;               // Which A row we are about to request
logic [3:0]                                   dc_read_b_row;               // Which B row we are about to request
logic                                         dc_read_b_col;               // Which B col we are about to request
logic                                         dc_read_req_a;               // Requested an A data
logic                                         dc_read_req_b;               // Requested a B data
logic                                         dc_read_a_done;              // All A requests are done
logic                                         dc_read_b_done;              // All B requests are done
logic [VPU_TENC_ADDR_MSB:1]                        s1_tima_tenc_raddr;          // TENC read addr in S1
logic                                         dc_read_acol_even_arow_even; // Even A col - Even A row
logic                                         dc_read_acol_odd_arow_even;  // Even A col - Odd A row
logic                                         dc_read_start_a_is_odd;      // dc read start address is odd
logic                                         dc_read_start_a_is_odd_next; // dc read start address is odd next
logic                                         s0_tima_last_pass;           // tima last pass


////////////////////////////////////////////////////////////////////////////////
// Decodes mode of operation
////////////////////////////////////////////////////////////////////////////////

logic req_fp32;      // FP32
logic req_fp16afp32; // FP16 mul with FP32 add

always_comb
begin
    req_fp32      = (f0_core_ctrl.tensorfma_ctrl.mode == 3'b000);
    req_fp16afp32 = (f0_core_ctrl.tensorfma_ctrl.mode == 3'b001);
    req_int8      = (f0_core_ctrl.tensorfma_ctrl.mode == 3'b011);
end

////////////////////////////////////////////////////////////////////////////////
// Generates DCache request
////////////////////////////////////////////////////////////////////////////////

logic dc_read_a_skip;     // Skip requests to A because data is already in ten TMP
logic dc_read_tentmp_wen; // Write to TMP RF

always_comb
begin
      //acol-arow
    dc_read_acol_even_arow_even = ~dc_read_a_col[0] && ~dc_read_a_row[0];
    dc_read_acol_odd_arow_even  = dc_read_a_col[0] && ~dc_read_a_row[0];


    // A requests have priority over B requests. If there's a bit not set then
    // we are doing an A request
    dc_read_doing_a = (dc_read_a_req_sent < VPU_TENA_SIZE) && !dc_read_a_done && ~(req_int8 && tensortmp_rf_written[dc_read_a_row[3:1]] && dc_read_acol_even_arow_even);
    dc_read_doing_b = |(~dc_read_b_req_sent[fma_exec_b_rd_ptr_next]) && !dc_read_b_done && !dc_read_doing_a && !f0_core_ctrl.tensorfma_ctrl.ten_b;

    // Computes the Scratchpad position (at byte level) that we need to read
    dc_read_a_col_adj = { dc_read_a_col, 2'b0 };

    dc_read_addr_flat = dc_read_doing_a ? { dc_read_scp_a_addr, dc_read_a_col_adj }   + { dc_read_a_row, 6'b0 }
                                        : { dc_read_scp_b_addr, dc_read_b_col, 5'b0 } + { dc_read_b_row, 6'b0 };

    dc_read_scp_dest     = dc_read_addr_flat[6+:$clog2(DCACHE_WAYS*DCACHE_SETS)];
    dc_read_scp_dest_set = get_set_from_scp_dest(dc_read_scp_dest);
    dc_read_scp_dest_way = get_way_from_scp_dest(dc_read_scp_dest);
    dc_read_addr_mod     = {dc_read_scp_dest_way, dc_read_scp_dest_set, dc_read_addr_flat[5:0]};

    // For passes where we already saved the data in TenTMP, don't read
    dc_read_a_skip = req_int8 && dc_read_acol_odd_arow_even && !dc_read_start_a_is_odd;
    // Send a request if enabled and there's a pending request to send
    dcache_ctrl.scp_req.read_en = enabled_int && ((dc_read_doing_a && ~dc_read_a_skip) || dc_read_doing_b);

    // In order to have available slots to SCP in INT8 mode:
    // FSM does 64b accesses in even a col passes for the even a rows
    // We save up to 8 32bit values in TENA which are later read in following
    // a col pass, freeing the SCP port
    // This way dcache is available for regular instructions and also other
    // FSMs that need SCP port
    dcache_ctrl.scp_req.size64  = req_int8                      // TIMA
                                && dc_read_doing_a              // A access
                                && dc_read_acol_even_arow_even; // Even A col - Even A row

    dcache_ctrl.scp_req.size256 = ~dc_read_doing_a; // Full load for B


    dcache_ctrl.scp_req.addr    = dc_read_addr_mod[DCACHE_LRAM_ADDR_SIZE-1:0];
    dcache_ctrl.scp_req.way     = dc_read_addr_mod[DCACHE_LRAM_ADDR_SIZE +: $clog2(DCACHE_WAYS)];
    // In order to prevent having instructions replaying in the dcache because
    // they have conflicts with SCP reads (high prio), there's a signal to
    // indicate the SCP will read in two cycles, so bidding for other units
    // will be lost.
    dcache_ctrl.scp_req.bid     = ~enabled ? 1'b0
                                : req_int8 ? dcache_ctrl.scp_req.read_en // For INT8, using the scp_read_en works fine
                                :            dc_read_req_a;

    // Going to write to TMP RF
    dc_read_tentmp_wen = dcache_ctrl.scp_req.read_en && dcache_ctrl.scp_req.size64;
end

always_comb
begin
    // Compute request signals
    dc_read_req_a = (dcache_ctrl.scp_req.read_en || (enabled_int && dc_read_a_skip)) && dc_read_doing_a;
    dc_read_req_b = dcache_ctrl.scp_req.read_en && !dc_read_doing_a;
end

////////////////////////////////////////////////////////////////////////////////
// Keeps track of pending request to dcache to prefetch
////////////////////////////////////////////////////////////////////////////////

logic [$clog2(VPU_TENA_SIZE):0]    dc_read_a_req_sent_next;     // Count of request for A sent
logic [(VPU_TENB_SIZE/2)-1:0][1:0] dc_read_b_req_sent_next;     // If a request for B has been sent
logic                               dc_read_b_req_sent_up;       // Update dc_read_b_req_sent
logic [3:0]                         fma_exec_a_row;              // Which A row block it is executing
logic [3:0]                         fma_exec_a_col;              // Which A col block it is executing
logic [1:0]                         dc_read_b_wr_ptr;            // Write pointer to the VRF temp values for B reads
logic [1:0]                         dc_read_b_wr_ptr_next;
logic [3:0]                         fma_exec_a_col_req_sent_ptr;
logic                               fma_exec_b_col;              // Which B col block it is executing
logic                               fma_exec_last_b_col;         // Doing last B col pass
logic                               dc_read_a_wr_ptr;            // Write pointer to the VRF temp values for A reads
logic                               dc_read_a_wr_ptr_next;
logic                               tenb_fill_pass;              // Which TENB pass we are doing
logic                               tenb_credit_s0;              // TENB credit released
logic                               pend_sticky_wait;            // Have to wait for previous TensorOps to finish


always_ff @(posedge clock) begin
    if (reset) begin
        dc_read_a_req_sent <= 'b0;
    end else if (working) begin
        dc_read_a_req_sent <= dc_read_a_req_sent_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        dc_read_b_req_sent <= {VPU_TENB_SIZE/2{2'b0}};
    end else if (dc_read_b_req_sent_up) begin
        dc_read_b_req_sent <= dc_read_b_req_sent_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        tenb_fill_pass <= 1'b0;
    end else if (dcache_scp_resp.fill_is_tenb) begin
        tenb_fill_pass <= ~tenb_fill_pass;
    end
end

always_ff @(posedge clock) begin
    if (working) begin
        dc_read_a_wr_ptr <= dc_read_a_wr_ptr_next;
        dc_read_b_wr_ptr <= dc_read_b_wr_ptr_next;
        dc_read_scp_a_addr <= dc_read_scp_a_addr_next;
        dc_read_scp_b_addr <= dc_read_scp_b_addr_next;
    end
end


always_comb
begin
    dc_read_a_req_sent_next = dc_read_a_req_sent;
    dc_read_b_req_sent_next = dc_read_b_req_sent;

    // Computes if we are in last B col
    fma_exec_last_b_col = (fma_exec_b_col == 1'b1) || req_int8;

    // Sent an A request, set the prefetch bit
    if(dc_read_req_a)
        dc_read_a_req_sent_next = dc_read_a_req_sent_next + 'b1;

    // Clear an A request when sending an FMA and is the last col block of
    // a B row
    if(s0_fma_en && fma_exec_last_b_col)
        dc_read_a_req_sent_next = dc_read_a_req_sent_next - 'b1;

    // Sent a B request, set the prefetch bit
    if(dc_read_req_b)
        dc_read_b_req_sent_next[dc_read_b_row[$clog2(VPU_TENB_SIZE/2)-1:0]][dc_read_b_col] = 1'b1;

    // TensorLoad wrote to B
    if(dcache_scp_resp.fill_is_tenb)
        dc_read_b_req_sent_next[dcache_scp_resp.tenb_line[$clog2(VPU_TENB_SIZE/2)-1:0]][tenb_fill_pass] = 1'b1;

    // TensolLoad B flush
    if (dcache_scp_resp.tenb_flush)
        dc_read_b_req_sent_next = {VPU_TENB_SIZE/2{2'b0}};

    // Update dc_read_b_req_sent
    dc_read_b_req_sent_up = working || dcache_scp_resp.fill_is_tenb || dcache_scp_resp.tenb_flush;

    // Clear a B request when sending an FMA and is the last A row
    tenb_credit_s0 = 1'b0;
    if(s0_fma_en && (fma_exec_a_row == f0_core_ctrl.tensorfma_ctrl.rows_a))
    begin
        fma_exec_a_col_req_sent_ptr = fma_exec_a_col-f0_core_ctrl.tensorfma_ctrl.start_a;
        dc_read_b_req_sent_next[fma_exec_a_col_req_sent_ptr[$clog2(VPU_TENB_SIZE/2)-1:0]][fma_exec_b_col] = 1'b0;
        // Clear both lines in INT8 mode
        if(req_int8)
            dc_read_b_req_sent_next[fma_exec_a_col_req_sent_ptr[$clog2(VPU_TENB_SIZE/2)-1:0]][1'b1] = 1'b0;

        tenb_credit_s0 = f0_core_ctrl.tensorfma_ctrl.ten_b && (req_int8 || fma_exec_b_col);
    end

    // Have to wait due pending sticky (previous) tensorops
    // For TIMA ops that don't write to VPU RF they can go ahead as they don't
    // have dependencies with other tensorops
    pend_sticky_wait = (scoreboard_pend_sticky || tquant_pend_sticky || tstore_pend_sticky) && !(req_int8 && !f0_core_ctrl.tensorfma_ctrl.to_vrf);

    // Only send the TFMA enable bit to dcache when no longer blocked by
    // sticky dependencies
    dcache_ctrl.tfma_enabled = enabled_int && !pend_sticky_wait;
    dcache_ctrl.tfma_rf_write = (working && req_int8 && f0_core_ctrl.tensorfma_ctrl.to_vrf && !pend_sticky_wait);

    // Computes write pointers to the VRF temp registers
    dc_read_a_wr_ptr_next = dc_read_a_wr_ptr;
    if(start)
        dc_read_a_wr_ptr_next = 1'b0;
    else if(dc_read_req_a)
        dc_read_a_wr_ptr_next = ~dc_read_a_wr_ptr;

    dc_read_b_wr_ptr_next = dc_read_b_wr_ptr;
    if(start)
        dc_read_b_wr_ptr_next = 2'b0;
    // Increment write pointer when done with the B row
    else if(dc_read_req_b && (dc_read_b_col == 1'b1))
        dc_read_b_wr_ptr_next = dc_read_b_wr_ptr + 2'b1;

    // Scratchpad start address wrap
    dc_read_scp_a_addr_next = f0_core_ctrl.tensorfma_ctrl.scp_a;
    dc_read_scp_b_addr_next = f0_core_ctrl.tensorfma_ctrl.scp_b;
    if (f0_core_ctrl.tensorfma_ctrl.scp_a >= DCACHE_TL_SCP_MAX_IDX_SCP_ADDR)
      dc_read_scp_a_addr_next = f0_core_ctrl.tensorfma_ctrl.scp_a - DCACHE_TL_SCP_MAX_IDX_SCP_ADDR;
    if (f0_core_ctrl.tensorfma_ctrl.scp_b >= DCACHE_TL_SCP_MAX_IDX_SCP_ADDR)
      dc_read_scp_b_addr_next = f0_core_ctrl.tensorfma_ctrl.scp_b - DCACHE_TL_SCP_MAX_IDX_SCP_ADDR;
end

////////////////////////////////////////////////////////////////////////////////
// Keeps track of what Dcache position to request for A matrix
////////////////////////////////////////////////////////////////////////////////

logic [3:0] dc_read_a_col_next;  // Which A col we are about to request
logic [3:0] dc_read_a_row_next;  // Which A row we are about to request
logic       dc_read_a_done_next; // A has done all the requests

always_ff @(posedge clock) begin
    if (reset) begin
        dc_read_a_col <= 4'b0;
        dc_read_a_row <= 4'b0;
        dc_read_a_done <= 1'b1;
        dc_read_start_a_is_odd <= 1'b0;
    end else if (working) begin
        dc_read_a_col <= dc_read_a_col_next;
        dc_read_a_row <= dc_read_a_row_next;
        dc_read_a_done <= dc_read_a_done_next;
        dc_read_start_a_is_odd <= dc_read_start_a_is_odd_next;
    end
end


always_comb
begin
    dc_read_a_col_next  = dc_read_a_col;
    dc_read_a_row_next  = dc_read_a_row;
    dc_read_a_done_next = dc_read_a_done;
    dc_read_start_a_is_odd_next = dc_read_start_a_is_odd;

    if(start) begin
      dc_read_a_col_next  = f0_core_ctrl.tensorfma_ctrl.start_a;
      dc_read_a_done_next = 1'b0;
      dc_read_start_a_is_odd_next = f0_core_ctrl.tensorfma_ctrl.start_a[0];
    end


    // Update in case of A request sent
    if(dc_read_req_a)
    begin
        // Move to next row
        dc_read_a_row_next = dc_read_a_row + 4'b1;
        // If done the last row, move to next col
        if(dc_read_a_row == f0_core_ctrl.tensorfma_ctrl.rows_a)
        begin
            dc_read_a_col_next = dc_read_a_col + 4'b1;
            dc_read_start_a_is_odd_next = 1'b0;
            dc_read_a_row_next = 4'b0;

            if(dc_read_a_col == tensorfma_ctrl_cols_start_a[3:0])
                dc_read_a_done_next = 1'b1;
        end
    end
end

////////////////////////////////////////////////////////////////////////////////
// Keeps track of what Dcache position to request for B matrix
////////////////////////////////////////////////////////////////////////////////

logic [3:0] dc_read_b_row_next;  // Which B row we are about to request
logic       dc_read_b_col_next;  // Which B col we are about to request
logic       dc_read_b_done_next; // B has done all the requests

always_ff @(posedge clock) begin
    if (reset) begin
        dc_read_b_col <= 1'b0;
        dc_read_b_row <= 4'b0;
        dc_read_b_done <= 1'b1;
    end else if (working) begin
        dc_read_b_col <= dc_read_b_col_next;
        dc_read_b_row <= dc_read_b_row_next;
        dc_read_b_done <= dc_read_b_done_next;
    end
end

always_comb
begin
    dc_read_b_col_next  = dc_read_b_col;
    dc_read_b_row_next  = dc_read_b_row;
    dc_read_b_done_next = dc_read_b_done;

    if(start) begin
      dc_read_b_done_next = 1'b0;
      dc_read_b_row_next =  4'b0;
    end

    // Update in case of B request sent
    if(dc_read_req_b)
    begin
        // Move to next row
        dc_read_b_col_next = dc_read_b_col + 1'b1;
        // If done the last col, move to next row
        if(dc_read_b_col == 1'b1)
        begin
            dc_read_b_row_next = dc_read_b_row + 4'b1;
            if(dc_read_b_row == f0_core_ctrl.tensorfma_ctrl.cols_a)
                dc_read_b_done_next = 1'b1;
        end
    end
end

////////////////////////////////////////////////////////////////////////////////
// Generates the write signals to the RF. Need to add two cycles to align the
// control with the data.
////////////////////////////////////////////////////////////////////////////////

vpu_ml_load_ctrl load_ctrl_pre0;
vpu_ml_load_ctrl load_ctrl_pre1;

always_ff @(posedge clock) begin
    if (working) begin
        load_ctrl_pre1 <= load_ctrl_pre0;
        load_ctrl <= load_ctrl_pre1;
    end
end

always_comb
begin
    // Computes the write control values
    load_ctrl_pre0.wen           = dcache_ctrl.scp_req.read_en;
    load_ctrl_pre0.thread_id     = 1'b1;                                                         // Always going to thread1
    load_ctrl_pre0.broadcast_sel = dc_read_addr_flat[DCACHE_ADDR_BCAST_RANGE_MSB:DCACHE_ADDR_BCAST_RANGE_LSB];                  // Broadcast sel
    load_ctrl_pre0.waddr         = dc_read_doing_a ? { 4'b0,  dc_read_a_wr_ptr                }  // A requests
                                 :                   { 2'b01, dc_read_b_wr_ptr, dc_read_b_col }; // B requests
end

// pre wen
assign load_ctrl_pre_tena_wen  = load_ctrl_pre1.wen & ~load_ctrl_pre1.waddr[3];
assign load_ctrl_pre_tenb_wen  = load_ctrl_pre1.wen &  load_ctrl_pre1.waddr[3];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// FMA generation control
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

logic [$clog2(VPU_REGFILE_NUM)-1:0] fma_exec_accum_reg;     // Read pointer for the accum register (and destination)
logic [$clog2(VPU_REGFILE_NUM)-1:0] fma_exec_mat_a_reg;     // Read pointer for the A matrix register
logic [$clog2(VPU_REGFILE_NUM)-1:0] fma_exec_mat_b_reg;     // Read pointer for the B matrix register
logic [INST_SIZE-1:0]                  fma_inst_s0;            // FMA inst computed 2 cycles earlier than execution
logic [3:0]                          fma_exec_a_row_next;    // Which A row block it is executing
logic [3:0]                          fma_exec_a_col_next;    // Which A col block it is executing
logic                                fma_exec_b_col_next;    // Which B col block it is executing
logic                                fma_exec_a_rd_ptr;      // Read pointer to the VRF temp values for A reads
logic                                fma_exec_a_rd_ptr_next;
logic [$clog2(VPU_TENB_SIZE/2)-1:0] fma_exec_b_rd_ptr;      // Read pointer to the VRF temp values for B reads
logic                                fma_accum_collision;    // There's an accumulation collision because the register was written less than 5 cycles before
logic                                s0_fma_en_conv;         // FMA enable in S0 stage after convolution CSR check
logic                                conv_skip;              // Step must be skipped due convolution CSR
logic                                done_sticky_next;       // The TensorFMA issuing is done
logic [VPU_LANE_NUM-1:0]            s0_fma_mask;
logic                                fma_exec_start_a_is_odd_next;
logic                                fma_exec_start_a_is_odd;
logic                                fma_exec_hazard_wait_next; // Wait execution due a hazard
logic                                fma_exec_hazard_wait;      // Wait execution due a hazard



always_ff @(posedge clock) begin
    if (reset) begin
        fma_exec_b_col <= 1'b0;
        fma_exec_a_row <= 4'b0;
        fma_exec_a_col <= 4'b0;
        fma_mask_next <= '0;
        fma_exec_start_a_is_odd <= 1'b0;
        done_sticky <= 1'b0;
    end else if (working) begin
        fma_exec_b_col <= fma_exec_b_col_next;
        fma_exec_a_row <= fma_exec_a_row_next;
        fma_exec_a_col <= fma_exec_a_col_next;
        fma_mask_next <= s0_fma_mask;
        fma_exec_start_a_is_odd <= fma_exec_start_a_is_odd_next;
        done_sticky <= done_sticky_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        fma_en_next <= 1'b0;
        fma_en <= 1'b0;
    end else if (working && ~req_int8) begin
        fma_en_next <= s0_fma_en_conv && ~req_int8 && (req_fp32 || req_fp16afp32);
        fma_en <= fma_en_next;
    end
end

always_ff @(posedge clock) begin
    if (working) begin
        fma_exec_a_rd_ptr <= fma_exec_a_rd_ptr_next;
        fma_exec_b_rd_ptr <= fma_exec_b_rd_ptr_next;
    end
    if (working && ~req_int8) begin
        fma_inst_next <= fma_inst_s0;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        fma_exec_hazard_wait <= 1'b0;
    end else if (working && req_int8) begin
        fma_exec_hazard_wait <= fma_exec_hazard_wait_next;
    end
end


always_comb
begin
    // Based on the current position generates the instruction.
    // Only generate instruction if we have A and B data available, otherwise
    // stalls the execution
    s0_fma_en = enabled_int
              && !pend_sticky_wait
              && (|dc_read_a_req_sent)
              && dc_read_b_req_sent[fma_exec_b_rd_ptr][fma_exec_b_col]
              && (dc_read_b_req_sent[fma_exec_b_rd_ptr][1'b1] || ~req_int8) // Check that both B data is present for INT8 case
              && !fma_accum_collision
              && (!f0_core_ctrl.tensorfma_ctrl.is_conv || !conv_bits_pending)
              && !fma_exec_hazard_wait;

    // Doesn't send the instruction if using conv CSR and bit is not set
    // Exception is for first pass, that even in case of convolution skip must
    // write a 0
    conv_skip      = !conv_bits_pending && f0_core_ctrl.tensorfma_ctrl.is_conv && (!conv_bits[fma_exec_a_row]);
    s0_fma_en_conv = s0_fma_en && (!conv_skip || first_pass || s0_tima_last_pass);


      //all lanes enabled by default
    s0_fma_mask = {VPU_LANE_NUM{1'b1}};

    if (req_int8) begin
      //f0_core_ctrl.tensorfma_ctrl.cols_b==00: s0_fma_mask=0x03
      //f0_core_ctrl.tensorfma_ctrl.cols_b==01: s0_fma_mask=0x0f
      //f0_core_ctrl.tensorfma_ctrl.cols_b==10: s0_fma_mask=0x3f
      //f0_core_ctrl.tensorfma_ctrl.cols_b==11: s0_fma_mask=0xff
      s0_fma_mask[3:2] &= {2{~(f0_core_ctrl.tensorfma_ctrl.cols_b==0)}};
      s0_fma_mask[5:4] &= {2{f0_core_ctrl.tensorfma_ctrl.cols_b[1]}};
      s0_fma_mask[7:6] &= {2{(f0_core_ctrl.tensorfma_ctrl.cols_b==3)}};
    end
      //fp16
    else begin
      //masking out vpu lanes out of the B matrix colums
        //f0_core_ctrl.tensorfma_ctrl.cols_b==00: s0_fma_mask=0x0f (!fma_exec_b_col), s0_fma_mask=0x00 (fma_exec_b_col)
        //f0_core_ctrl.tensorfma_ctrl.cols_b==01: s0_fma_mask=0xff (!fma_exec_b_col), s0_fma_mask=0x00 (fma_exec_b_col)
        //f0_core_ctrl.tensorfma_ctrl.cols_b==10: s0_fma_mask=0xff (!fma_exec_b_col), s0_fma_mask=0x0f (fma_exec_b_col)
        //f0_core_ctrl.tensorfma_ctrl.cols_b==11: s0_fma_mask=0xff (!fma_exec_b_col), s0_fma_mask=0xff (fma_exec_b_col)
      s0_fma_mask[3:0] &= {4{~((f0_core_ctrl.tensorfma_ctrl.cols_b<2)&fma_exec_b_col)}};
      s0_fma_mask[7:4] &= {4{~(((f0_core_ctrl.tensorfma_ctrl.cols_b==0)&!fma_exec_b_col) ||
                               ((f0_core_ctrl.tensorfma_ctrl.cols_b<3)&fma_exec_b_col))}};
    end


    fma_exec_accum_reg = { fma_exec_a_row, fma_exec_b_col };
    fma_exec_mat_a_reg = { 4'b0, fma_exec_a_rd_ptr                  };
    fma_exec_mat_b_reg = { 2'b01, fma_exec_b_rd_ptr, fma_exec_b_col };

    // Generates opcode depending on precission and Mul/FMA pass
    // Convolution skips for the 1st pass must write a zero (done through
    // XOR_PI)
    fma_inst_s0 =  conv_skip                   ? FXOR_PI
                :  first_pass && req_fp16afp32 ? TMUL_PH_PS
                : ~first_pass && req_fp16afp32 ? TFMA_PH_PS
                :  first_pass && req_fp32      ? FMUL_PS
                :                                FMADD_PS;

    if(!conv_skip)
    begin
        //FP1632 ops only support RTZ rm
      if (req_fp16afp32)
        fma_inst_s0[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = RTZ;
        //FP32 ops uses DYN rm
      else
        fma_inst_s0[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB] = VPU_RM_DYN;
    end

    fma_inst_s0[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = fma_exec_accum_reg;
    fma_inst_s0[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = conv_skip ? fma_exec_accum_reg : fma_exec_mat_a_reg;
    fma_inst_s0[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = conv_skip ? fma_exec_accum_reg : fma_exec_mat_b_reg;

    // Only FMA cases update the src3
    if(~first_pass) fma_inst_s0[VPU_INST_REN3_RA_SEL_MSB:VPU_INST_REN3_RA_SEL_LSB] = fma_exec_accum_reg;
end

always_comb
begin
    first_pass_clear    = 1'b0;
    done_sticky_next    = done_sticky;
    fma_exec_b_col_next = fma_exec_b_col;
    fma_exec_a_row_next = fma_exec_a_row;
    fma_exec_a_col_next = fma_exec_a_col;
    fma_exec_start_a_is_odd_next = fma_exec_start_a_is_odd;

    // When issue a new FMA, move the pointers
    if(s0_fma_en)
    begin
        // First go to next B col block (for INT8, we do all B cols at once)
        fma_exec_b_col_next = req_int8 ? fma_exec_b_col : fma_exec_b_col + 1'b1;
        // If last B col block, go to next A row
        if(fma_exec_last_b_col)
        begin
            fma_exec_a_row_next = fma_exec_a_row + 4'b1;

             // If last A row, go to next A col
            if(fma_exec_a_row == f0_core_ctrl.tensorfma_ctrl.rows_a)
            begin
                fma_exec_a_row_next = 4'b0;
                first_pass_clear    = 1'b1; // Clear the first pass bit when changing to next A col
                fma_exec_a_col_next = fma_exec_a_col + 4'b0001;
                done_sticky_next    = (fma_exec_a_col == tensorfma_ctrl_cols_start_a[3:0]);
                fma_exec_start_a_is_odd_next = 1'b0;
            end
        end
    end

    // Clear done bit upon start
    if(start)
        done_sticky_next = 1'b0;

    // Computes write pointers to the VRF temp registers
    fma_exec_a_rd_ptr_next = fma_exec_a_rd_ptr;
    if(start) begin
      fma_exec_a_rd_ptr_next       = 1'b0;
      fma_exec_a_col_next          = f0_core_ctrl.tensorfma_ctrl.start_a;
      fma_exec_start_a_is_odd_next = f0_core_ctrl.tensorfma_ctrl.start_a[0];
    end
    else if(s0_fma_en && fma_exec_last_b_col)
      fma_exec_a_rd_ptr_next = ~fma_exec_a_rd_ptr;

    fma_exec_b_rd_ptr_next = fma_exec_b_rd_ptr;
    if(start)
        fma_exec_b_rd_ptr_next = 'b0;
    // Doing last A row and last B col, move to next B row
    else if(s0_fma_en && (fma_exec_a_row == f0_core_ctrl.tensorfma_ctrl.rows_a) && fma_exec_last_b_col)
        fma_exec_b_rd_ptr_next = fma_exec_b_rd_ptr + 'b1;

    //Computes the next hazard wait
    fma_exec_hazard_wait_next = fma_exec_hazard_wait;

    if (fma_exec_hazard_wait)
      fma_exec_hazard_wait_next = 1'b0;
    else if (s0_fma_en && f0_core_ctrl.tensorfma_ctrl.rows_a==0)
      fma_exec_hazard_wait_next = 1'b1;
end

////////////////////////////////////////////////////////////////////////////////
// Collision control
// As FMA latency is 5 cycles, tensorFMA can't issue an FMA with same accum
// than any of the FMAs issued in the last 4 cycles
////////////////////////////////////////////////////////////////////////////////

logic [4:0] fma_accum_3_cycle; // Accum for the 3 cycle ahead FMA
logic [4:0] fma_accum_4_cycle; // Accum for the 4 cycle ahead FMA
logic [4:0] fma_accum_5_cycle; // Accum for the 5 cycle ahead FMA
logic [4:0] fma_accum_6_cycle; // Accum for the 6 cycle ahead FMA
logic       fma_valid_3_cycle; // Accum for the 3 cycle ahead FMA
logic       fma_valid_4_cycle; // Accum for the 4 cycle ahead FMA
logic       fma_valid_5_cycle; // Accum for the 5 cycle ahead FMA
logic       fma_valid_6_cycle; // Accum for the 6 cycle ahead FMA

always_ff @(posedge clock) begin
    if (reset) begin
        fma_valid_3_cycle <= 1'b0;
        fma_valid_4_cycle <= 1'b0;
        fma_valid_5_cycle <= 1'b0;
        fma_valid_6_cycle <= 1'b0;
    end else if (working) begin
        fma_valid_3_cycle <= fma_en;
        fma_valid_4_cycle <= fma_valid_3_cycle;
        fma_valid_5_cycle <= fma_valid_4_cycle;
        fma_valid_6_cycle <= fma_valid_5_cycle;
    end
end

always_ff @(posedge clock) begin
    if (fma_en) begin
        fma_accum_3_cycle <= fma_inst[11:7];
    end
    if (fma_valid_3_cycle) begin
        fma_accum_4_cycle <= fma_accum_3_cycle;
    end
    if (fma_valid_4_cycle) begin
        fma_accum_5_cycle <= fma_accum_4_cycle;
    end
    if (fma_valid_5_cycle) begin
        fma_accum_6_cycle <= fma_accum_5_cycle;
    end
end

always_comb
begin
    fma_accum_collision = 1'b0;
    // Collision with FMA
    if(fma_en_next       && (fma_exec_accum_reg == fma_inst_next[11:7])) fma_accum_collision = 1'b1; // 1 cycle distance
    if(fma_en            && (fma_exec_accum_reg == fma_inst[11:7]))      fma_accum_collision = 1'b1; // 2 cycle distance
    if(fma_valid_3_cycle && (fma_exec_accum_reg == fma_accum_3_cycle))   fma_accum_collision = 1'b1; // 3 cycle distance
    if(fma_valid_4_cycle && (fma_exec_accum_reg == fma_accum_4_cycle))   fma_accum_collision = 1'b1; // 4 cycle distance
    if(fma_valid_5_cycle && (fma_exec_accum_reg == fma_accum_5_cycle))   fma_accum_collision = 1'b1; // 4 cycle distance
    if(fma_valid_6_cycle && (fma_exec_accum_reg == fma_accum_6_cycle))   fma_accum_collision = 1'b1; // 4 cycle distance
end

////////////////////////////////////////////////////////////////////////////////
// Deallocation of TENB entries. Need to delay it 3 cycles to prevent writing
// new value before old value is read for last time
////////////////////////////////////////////////////////////////////////////////

logic [$clog2(VPU_TENB_SIZE/2)-1:0] tenb_credit_entry_s1;      // TENB entry being released
logic [$clog2(VPU_TENB_SIZE/2)-1:0] tenb_credit_entry_s2;      // TENB entry being released
logic [$clog2(VPU_TENB_SIZE/2)-1:0] tenb_credit_entry_s3;      // TENB entry being released
logic                                tenb_credit_s1;            // TENB entry is released
logic                                tenb_credit_s2;            // TENB entry is released
logic                                tenb_credit_s3;            // TENB entry is released
logic [3:0]                          fma_exec_a_col_no_start;   // Which A col-start block it is executing

assign fma_exec_a_col_no_start = (fma_exec_a_col-f0_core_ctrl.tensorfma_ctrl.start_a);


always_ff @(posedge clock) begin
    tenb_credit_s1 <= tenb_credit_s0;
    tenb_credit_s2 <= tenb_credit_s1;
    tenb_credit_s3 <= tenb_credit_s2;
    if (tenb_credit_s0) begin
        tenb_credit_entry_s1 <= fma_exec_a_col_no_start[$clog2(VPU_TENB_SIZE/2)-1:0];
    end
    if (tenb_credit_s1) begin
        tenb_credit_entry_s2 <= tenb_credit_entry_s1;
    end
    if (tenb_credit_s2) begin
        tenb_credit_entry_s3 <= tenb_credit_entry_s2;
    end
end

// Doing this in an always comb and not straight in FF because verilator
// chokes up on this
always_comb
begin
    dcache_ctrl.tenb_credit       = tenb_credit_s3;
    dcache_ctrl.tenb_credit_entry = tenb_credit_entry_s3;
    dcache_ctrl.reduce_wait       = 1'b0; // VPU ML will use the reduce_wait coming from the vpu_tensorreduce block
end

////////////////////////////////////////////////////////////////////////////////
// TIMA specific control signals
// In order to allow TensorQuantize and regular operations run while TIMA is
// working, its control is completely detached from the pipeline
// Need to delay the control 2 cycles to wait for the data to be available in
// TENA registers
////////////////////////////////////////////////////////////////////////////////

logic tima_clk_en; // Clock enable for TIMA specific control registers

assign tima_clk_en = working && req_int8;

////////////////////////////////////////////////////////////////////////////////
// S0 stage
////////////////////////////////////////////////////////////////////////////////

always_comb s0_tima_last_pass = req_int8 && (fma_exec_a_col == tensorfma_ctrl_cols_start_a[3:0]) && f0_core_ctrl.tensorfma_ctrl.to_vrf;

////////////////////////////////////////////////////////////////////////////////
// S1 stage
////////////////////////////////////////////////////////////////////////////////

logic                  s1_tima_valid;         // Valid TIMA operation in S1
logic [VPU_TENA_ADDR_W-1:0] s1_tima_tena_raddr;    // TENA read pointer in S1
logic [VPU_TENB_ADDR_W-1:0] s1_tima_tenb_raddr;    // TENB read pointer in S1
logic                  s1_tima_last_pass;     // Last pass in S1
logic                  s1_tima_ren3;          // Doing an FMA in S1
logic                  s1_tena_rf_rdata_mask; // TENA read data mask

always_ff @(posedge clock) begin
    if (reset) begin
        s1_tima_valid <= 1'b0;
    end else if (tima_clk_en) begin
        s1_tima_valid <= s0_fma_en_conv && req_int8;
    end
    if (tima_clk_en) begin
        s1_tima_tena_raddr <= fma_exec_a_rd_ptr;
        s1_tima_tenb_raddr <= { fma_exec_b_rd_ptr, 1'b0 };
        s1_tima_last_pass <= s0_tima_last_pass;
        s1_tima_ren3 <= ~first_pass;
        s1_tima_tenc_raddr <= fma_exec_a_row;
    end
    if (working) begin
        s1_tena_rf_rdata_mask <= ~conv_skip;
    end
end


////////////////////////////////////////////////////////////////////////////////
// EX stage
////////////////////////////////////////////////////////////////////////////////

logic [VPU_LANE_NUM-1:0]                    ex_fma_mask;        // ex fma mask
logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_tenc_wen;   // ex tima tenc wen
logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_rf_wen;     // ex tima rf wen
logic [VPU_TENC_ADDR_MSB:1]                       ex_tima_tenc_raddr; // ex tima tenc raddr

always_ff @(posedge clock) begin
    if (reset) begin
        ex_tima_valid <= 1'b0;
        ex_fma_mask <= '0;
    end else if (tima_clk_en) begin
        ex_tima_valid <= s1_tima_valid;
        ex_fma_mask <= fma_mask_next;
    end
    if (tima_clk_en) begin
        ex_tima_tenb_raddr <= s1_tima_tenb_raddr;
        ex_tima_tena_raddr <= s1_tima_tena_raddr;
        ex_tima_last_pass <= s1_tima_last_pass;
        ex_tima_ren3 <= s1_tima_ren3;
        ex_tima_tenc_raddr <= s1_tima_tenc_raddr;
    end
    if (working) begin
        ex_tena_rdata_mask <= s1_tena_rf_rdata_mask;
    end
end


  //tfma ima8 ops maps tensor raws as two consecutive vpu regs: f0+f1, f2+f3, f4+f5, f6+f7
  //TenC is 256b, TenC0 := f0, TenC1 := f1, etc.
  //if tensor has  4 raws, it is going to map as TenC0[3:0], TenC2[3:0]
  //if tensor has  8 raws, it is going to map as TenC0[7:0], TenC2[7:0]
  //if tensor has 12 raws, it is going to map as TenC0[7:0], TenC2[7:0], TenC1[3:0]
  //if tensor has 16 raws, it is going to map as TenC0[7:0], TenC2[7:0], TenC1[7:0]
always_comb
begin
  for(int l = 0; l < VPU_LANE_NUM; l++) begin
   for(int t = 0; t < VPU_NUM_TIMA; t++) begin
     ex_tima_mask[l][t]     = ex_fma_mask[(l>>1)+(t<<2)];
     ex_tima_tenc_wen[l][t] = ex_fma_mask[(l>>1)+(t<<2)] & ex_tima_valid && ~ex_tima_last_pass;
     ex_tima_rf_wen[l][t]   = ex_fma_mask[(l>>1)+(t<<2)] & ex_tima_valid && ex_tima_last_pass;
    end
  end
end


////////////////////////////////////////////////////////////////////////////////
// F2 stage
////////////////////////////////////////////////////////////////////////////////


always_ff @(posedge clock) begin
    if (reset) begin
        f2_tima_tenc_wen <= '0;
        f2_tima_rf_wen <= '0;
        f2_tima_ren3 <= 1'b0;
    end else if (tima_clk_en) begin
        f2_tima_tenc_wen <= ex_tima_tenc_wen;
        f2_tima_rf_wen <= ex_tima_rf_wen;
        f2_tima_ren3 <= ex_tima_ren3;
    end
    if (f0_core_ctrl.tensorfma_start) begin
        ex_tima_sa <= !f0_core_ctrl.tensorfma_ctrl.u_a;
        ex_tima_sb <= !f0_core_ctrl.tensorfma_ctrl.u_b;
    end
    if (tima_clk_en) begin
        f2_tima_tenc_raddr <= ex_tima_tenc_raddr;
    end
end

////////////////////////////////////////////////////////////////////////////////
// F2 stage
////////////////////////////////////////////////////////////////////////////////

always_ff @(posedge clock) begin
    if (reset) begin
        f3_tima_tenc_wen <= '0;
        f3_tima_rf_wen <= '0;
    end else if (tima_clk_en) begin
        f3_tima_tenc_wen <= f2_tima_tenc_wen;
        f3_tima_rf_wen <= f2_tima_rf_wen;
    end
    if (tima_clk_en) begin
        f3_tima_tenc_waddr <= f2_tima_tenc_raddr;
    end
end

////////////////////////////////////////////////////////////////////////////////
// TensorTMP control
////////////////////////////////////////////////////////////////////////////////

logic [VPU_TENTMP_ADDR_W-1:0] f2_tentmp_waddr;
logic                    s1_tentmp_acol_odd; // A column step is odd
logic                    ex_tentmp_acol_odd; // A column step is odd

always_ff @(posedge clock) begin
    if (reset) begin
        f2_tentmp_wen_early <= 1'b0;
        f3_tentmp_wen <= 1'b0;
    end else begin
        if (working && req_int8) begin
            f2_tentmp_wen_early <= dc_read_tentmp_wen;
            f3_tentmp_wen <= f2_tentmp_wen_early;
            tensortmp_rf_written <= tensortmp_rf_written_next;
            s1_tentmp_acol_odd <= fma_exec_a_col[0] & !fma_exec_start_a_is_odd;
            ex_tentmp_acol_odd <= s1_tentmp_acol_odd;
        end
    end
    if (dc_read_tentmp_wen) begin
        f2_tentmp_waddr <= dc_read_a_row[3:1];
    end
    if (f2_tentmp_wen_early) begin
        f3_tentmp_waddr <= f2_tentmp_waddr;
    end
end

always_comb
begin
    ex_tentmp_ren   = req_int8 && ex_tentmp_acol_odd && ~ex_tima_tenc_raddr[1];
    ex_tentmp_raddr = ex_tima_tenc_raddr[4:2] & {$bits(ex_tentmp_raddr){req_int8 && ex_tentmp_acol_odd}};
end

////////////////////////////////////////////////////////////////////////////////
// Keeps track of tensor tmp write/read to request next a col odd
////////////////////////////////////////////////////////////////////////////////

always_comb
begin
  tensortmp_rf_written_next = tensortmp_rf_written;

  if(start) begin
    for(integer i = 0; i < VPU_TENTMP_SIZE; i++)
      tensortmp_rf_written_next[i] = 1'b0;
  end

    //Write to TMP RF
  if(dc_read_tentmp_wen)
    tensortmp_rf_written_next[dc_read_a_row[3:1]] = 1'b1;

    //Read from TMP RF
  if ((ex_tima_valid & ex_tentmp_acol_odd) || (!first_pass & conv_skip & ~dc_read_a_row[0]))
    tensortmp_rf_written_next[ex_tima_tenc_raddr[4:2]] = 1'b0;

end


////////////////////////////////////////////////////////////////////////////////
// Performance counters
////////////////////////////////////////////////////////////////////////////////
assign io_events_wait_tenb = enabled_int
              && (|dc_read_a_req_sent)
              && ~dc_read_b_req_sent[fma_exec_b_rd_ptr][fma_exec_b_col]
              && (~dc_read_b_req_sent[fma_exec_b_rd_ptr][1'b1] || ~req_int8) // Check that both B data is present for INT8 case
              && !fma_accum_collision
              && (!f0_core_ctrl.tensorfma_ctrl.is_conv || !conv_bits_pending);


////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////

assign vpu_tensorfma_debug = {enabled_int,          //1
                              working,              //1
                              pend_sticky_wait,     //1
                              dc_read_a_req_sent,   //2
                              dc_read_b_req_sent,   //8
                              fma_exec_b_rd_ptr,    //2
                              fma_exec_b_col,       //1
                              req_int8,             //1
                              fma_accum_collision,  //1
                              conv_bits_pending,    //1,
                              tenb_credit_entry_s1, //2
                              tenb_credit_entry_s2, //2
                              tenb_credit_entry_s3  //2
                             };


endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on LATCH */

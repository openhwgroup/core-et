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

/* verilator lint_off WIDTHEXPAND */  // Imported original TensorQuant arithmetic relies on packed-field widths and constant-size promotion.
/* verilator lint_off UNOPTFLAT */  // TensorQuant request/hazard bookkeeping participates in preserved cross-unit feedback that Verilator flattens into false loops.
/* verilator lint_off UNUSEDSIGNAL */  // Packed control fields intentionally expose more bits than this block consumes in the current build.
module vpu_tensorquant
(
    // System signals
    input  logic                                         clock,
    input  logic                                         reset,
    // Control port from core to start new TensorQuant
    input  core_vpu_ctrl                                 f0_core_ctrl,
    output logic                                         enabled,
    // Dependencies with other units
    input  logic                                         tensorfma_pend,
    input  logic                                         tensorstore_pend,
    // Scratchpad port
    input  logic                                         dcache_scp_available,
    output vpu_dcache_scp_req                            dcache_scp_req,
    input  logic [DCACHE_DATA_SIZE-1:0]                    dcache_scp_data,
    // Signals going to VPU pipeline
    output logic                                         quant_en_next,
    output logic [INST_SIZE-1:0]                           quant_inst_next,
    output logic [VPU_LANE_NUM-1:0]                     quant_mask_next,
    output logic                                         quant_tena_en,
    // TensorQuant RF Control
    output logic                                         ex_ren,
    output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] ex_rdata,
    //debug
    output logic [10:0]                                  vpu_tensorquant_debug
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TensorQuant
// This instruction execute a sequence of operations specified by the
// programmer.
// The programmer specifies the number of A rows and B cols (128b chunks) of
// the tensor. It also specifies where the first data of the tensor is stored
// in the RF and register stride of the storage.
// The FSM walks through the different operations that the user specified and
// executes them for all the TensorValues.
// The operations are sent to the VPU pipeline.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

tensorquant_control tensorquant_ctrl;            // TensorQuant control bits
tensorquant_control tensorquant_ctrl_next;
logic [2:0]         exec_hazard_wait_next;       // Wait execution due a hazard
logic               tensorquant_ctrl_en;         // Update tensorquant control bits
logic               enabled_next;                // FSM is enabled next cycle
logic               clock_en;                    // Global clock enable
logic               exec_last_op_trans;          // Last op of a transformation
logic               new_req;                     // New request
logic               scoreboard_pend_sticky;      // Waits for previous Scoreboard to finish writes to VPU RF before starting TensorQuant
logic               scoreboard_pend_sticky_next; //
logic               tfma_pend_sticky;            // Waits for previous TensorFMA to finish writes to VPU RF before starting TensorQuant
logic               tfma_pend_sticky_next;       //
logic               tstore_pend_sticky;          // Waits for previous TensorStore to finish writes/reads to VPU RF before starting TensorQuant
logic               tstore_pend_sticky_next;     //
logic               quant_tena_scp_en_next;      // Enable next cycle the usage of TENA Scratchpad register
logic               tensorquant_last_next;
logic               tensorquant_last;
logic               scp_read_pending;            // Read pending from the scratchpad
logic               scp_read_pending_next;



always_ff @(posedge clock) begin
    if (reset) begin
        enabled <= 1'b0;
        tensorquant_last <= 1'b0;
        scoreboard_pend_sticky <= 1'b0;
        tfma_pend_sticky <= 1'b0;
        tstore_pend_sticky <= 1'b0;
    end else if (clock_en) begin
        enabled <= enabled_next;
        tensorquant_last <= tensorquant_last_next;
        scoreboard_pend_sticky <= scoreboard_pend_sticky_next;
        tfma_pend_sticky <= tfma_pend_sticky_next;
        tstore_pend_sticky <= tstore_pend_sticky_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        tensorquant_ctrl <= '0;
    end else if (tensorquant_ctrl_en) begin
        tensorquant_ctrl <= tensorquant_ctrl_next;
    end
end

always_comb
begin
      //clock enable
    clock_en = enabled || f0_core_ctrl.tensorquant_start || tensorquant_last || scp_read_pending;

    // Enabled
    enabled_next = enabled;
    // New TensorQuant with at least one transform
    new_req = f0_core_ctrl.tensorquant_start && (f0_core_ctrl.tensorquant_ctrl.trans[3:0] != 4'b0000);

      //last tensor quant
    tensorquant_last_next = (tensorquant_ctrl[3:0] == TQUANT_LAST) && (exec_hazard_wait_next == 3'b000) && enabled;

    // Computes next tensor quant control
    tensorquant_ctrl_next = tensorquant_ctrl;
    tensorquant_ctrl_en   =  f0_core_ctrl.tensorquant_start // New quant
                          || exec_last_op_trans;            // Moving to next transform

    if(f0_core_ctrl.tensorquant_start)
        tensorquant_ctrl_next = f0_core_ctrl.tensorquant_ctrl;
    // Shift the transforms
    else
    begin
        for(integer i = 0; i < 9; i++)
            tensorquant_ctrl_next.trans[i*4+:4] = tensorquant_ctrl.trans[4+i*4+:4];
        // Shift in Last to guarantee logic always finishes
        tensorquant_ctrl_next.trans[36+:4] = TQUANT_LAST;

        // Done last op of a transformation that reads from Scp
        // Read next Scp entry
        if(quant_tena_scp_en_next)
            tensorquant_ctrl_next.scp_src = tensorquant_ctrl.scp_src + 6'b1;
    end

    // Scratchpad destination wrap
    if (tensorquant_ctrl_next.scp_src >= DCACHE_TL_SCP_MAX_IDX_SCP_ADDR)
      tensorquant_ctrl_next.scp_src = tensorquant_ctrl_next.scp_src - DCACHE_TL_SCP_MAX_IDX_SCP_ADDR;

    // If next transform is a LAST, FSM is done (waits for hazard wait to
    // drain)
    if((tensorquant_ctrl[3:0] == TQUANT_LAST) && (exec_hazard_wait_next == 3'b000))
        enabled_next = 1'b0;
    // Upon new request, enable the FSM
    if(new_req)
        enabled_next = 1'b1;

    // Computes sticky bit for tensorfma write pending
    scoreboard_pend_sticky_next = scoreboard_pend_sticky;
    tfma_pend_sticky_next       = tfma_pend_sticky;
    tstore_pend_sticky_next     = tstore_pend_sticky;
    // Get if pending RF write
    if(new_req)
    begin
        scoreboard_pend_sticky_next = f0_core_ctrl.pending_scoreboard_t0;
        tfma_pend_sticky_next       = tensorfma_pend;
        tstore_pend_sticky_next     = tensorstore_pend;
    end
    else
    begin
        // Clear sticky bit when write done
        if(enabled && scoreboard_pend_sticky && !f0_core_ctrl.pending_scoreboard_t0)
            scoreboard_pend_sticky_next = 1'b0;
        // Clear sticky bit when write done
        if((enabled && tfma_pend_sticky && !tensorfma_pend) || f0_core_ctrl.tensorfma_start)
            tfma_pend_sticky_next = 1'b0;
        // Clear sticky bit when write done
        if((enabled && tstore_pend_sticky && !tensorstore_pend) || f0_core_ctrl.tensorstore_start)
            tstore_pend_sticky_next = 1'b0;
    end
end

//

////////////////////////////////////////////////////////////////////////////////
// Stores one cacheline of the SCP with data to be consumed by one
// transformation. It reads one cacheline ahead of time to provide the values
// as soon as needed. It always reads the data in 256b chunks and all the
// muxing is done internally.
////////////////////////////////////////////////////////////////////////////////

logic [1:0][DCACHE_DATA_SIZE-1:0]               scp_data;                  // Data loaded from SCP
logic [1:0]                                   scp_data_en_s0;            // Enable scratchpad data S0 stage
logic [1:0]                                   scp_data_en_s1;            // Enable scratchpad data S1 stage
logic [1:0]                                   scp_data_en_s2;            // Enable scratchpad data S2 stage
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)+5:0] dcache_scp_raddr;          // LRAM dcache addr
logic [$clog2(DCACHE_WAYS*DCACHE_SETS)-1:0] dcache_scp_dest;           // Scratchpad destination index
logic [DCACHE_SET_IDX_WIDTH-1:0]                     dcache_scp_dest_set;       // Scratchpad destination set
logic [DCACHE_WAY_IDX_WIDTH-1:0]                     dcache_scp_dest_way;       // Scratchpad destination way
logic [1:0]                                   scp_read_set_pending;      // Delayed set pending (needed to prevent updating RF with scratchpad data too early with multipass)
logic [1:0]                                   scp_read_set_pending_next;
logic                                         scp_read_buf;              // Which half of the scratchpad line to read
logic                                         scp_read_buf_next;
logic [3:0]                                   exec_curr_transform;       // Current transform operation
logic                                         exec_b_col;                // B col working on (pre ID)
logic [3:0]                                   exec_a_row;                // Which A row working on (pre ID)
logic                                         id_b_col;                  // B col working on (ID)
logic [3:0]                                   id_a_row;                  // Which A row working on (ID)
logic                                         ex_b_col;                  // B col working on (EX)
logic [3:0]                                   ex_a_row;                  // Which A row working on (EX)
logic                                         ex_rdata_zero;             // Force data to be zero
logic                                         ex_rdata_col;              // Force data to be read col-wise

always_ff @(posedge clock) begin
    if (reset) begin
        scp_data_en_s1 <= 2'b0;
        scp_data_en_s2 <= 2'b0;
        scp_read_pending <= 1'b0;
        scp_read_set_pending <= 2'b0;
    end else if (clock_en) begin
        scp_data_en_s1 <= scp_data_en_s0;
        scp_data_en_s2 <= scp_data_en_s1;
        scp_read_pending <= scp_read_pending_next;
        scp_read_set_pending <= scp_read_set_pending_next;
    end
end

always_ff @(posedge clock) begin
    if (clock_en) begin
        scp_read_buf <= scp_read_buf_next;
        id_b_col <= exec_b_col;
        id_a_row <= exec_a_row;
        ex_b_col <= id_b_col;
        ex_a_row <= id_a_row;
    end
end

genvar gen_it;

generate for(gen_it = 0; gen_it < 2; gen_it++)
begin
    always_ff @(posedge clock) begin
        if (scp_data_en_s2[gen_it]) begin
            scp_data[gen_it] <= dcache_scp_data;
        end
    end
end
endgenerate

always_comb
begin
    // Send SCP read request

    //L1 scratchpad cache line of first vector is stored betweem 0 and 47
    dcache_scp_dest = tensorquant_ctrl.scp_src;

    dcache_scp_dest_set    = get_set_from_scp_dest(dcache_scp_dest);
    dcache_scp_dest_way    = get_way_from_scp_dest(dcache_scp_dest);

    dcache_scp_raddr       = { dcache_scp_dest_way, dcache_scp_dest_set, scp_read_buf, 5'b0 };

    dcache_scp_req.read_en = scp_read_pending;
    dcache_scp_req.addr    = dcache_scp_raddr[DCACHE_LRAM_ADDR_SIZE-1:0];
    dcache_scp_req.way     = dcache_scp_raddr[DCACHE_LRAM_ADDR_SIZE +: $clog2(DCACHE_WAYS)];
    dcache_scp_req.size64  = 1'b0;
    dcache_scp_req.size256 = 1'b1;
    dcache_scp_req.bid     = 1'b0;

    // Read pending
    scp_read_pending_next = scp_read_pending;
    scp_read_buf_next     = scp_read_buf;
    scp_data_en_s0        = 2'b0;
    // Always read speculatively a scratchpad entry at start to hide the
    // latency
    if(new_req)
    begin
        scp_read_pending_next = 1'b1;
        scp_read_buf_next     = 1'b0;
    end
    // Request sent
    else if(dcache_scp_req.read_en && dcache_scp_available)
    begin
        scp_read_pending_next = ~scp_read_buf; // If read second half, remove the pending
        scp_read_buf_next     = ~scp_read_buf; // Invert half
        // Set the entry as read to update contents 2 cycle later
        for(integer i = 0; i < 2; i++)
            scp_data_en_s0[i] = dcache_scp_req.read_en && (scp_read_buf == i);
    end

    // Done last op of a transformation that reads from Scp
    // Read next Scp entry
    if(exec_last_op_trans && quant_tena_scp_en_next && (exec_hazard_wait_next == 3'b000)
       || (scp_read_set_pending == 2'b01))
        scp_read_pending_next = 1'b1;

    // In order to prevent updating the RF with scratchpad contents too early
    // for multipass cases, the read pending bit is set a cycle later
    scp_read_set_pending_next = scp_read_set_pending - 2'b01;
    if(scp_read_set_pending == 2'b00)
        scp_read_set_pending_next = 2'b00;
    if(exec_last_op_trans && quant_tena_scp_en_next && (exec_hazard_wait_next != 3'b000))
        scp_read_set_pending_next = 2'b10;
end

// Muxes the data read from SCP
always_comb
begin
    for(integer i = 0; i < VPU_LANE_NUM; i++)
    begin
        // Force zero (needed for ReLU)
        if(ex_rdata_zero)
            ex_rdata[i] = 32'b0;
        // Col-wise cases
        else if(ex_rdata_col)
            ex_rdata[i] = scp_data[ex_a_row / VPU_LANE_NUM][(ex_a_row % VPU_LANE_NUM) * 32 +: 32];
        // Row-wise cases
        else
            ex_rdata[i] = scp_data[ex_b_col][i * 32 +: 32];
    end
end

////////////////////////////////////////////////////////////////////////////////
// Executes transformations specified in the tensorquant instruction
////////////////////////////////////////////////////////////////////////////////

logic [4:0] exec_dest_reg;                   // Destination register
logic [3:0] exec_a_row_next;                 // Which A row working on
logic [2:0] exec_hazard_wait;                // Wait execution due a hazard
logic       exec_b_col_next;                 // B col working on
logic       exec_adv;                        // Advance exec status
logic       exec_is_txfma;                   // Instruction to be executed needs TXFMA
logic       exec_is_shsw;                    // Instruction to be executed needs SH-SW
logic       prev_is_txfma;                   // previous instruction to be executed needs TXFMA
logic       prev_is_shsw;                    // previous instruction to be executed needs SH-SW
logic       exec_hazard_txfma_to_short;      // Going from TXFMA to short with TXFMA lanes sleeping or small matrix config requires a bubble cycle
logic       exec_hazard_txfma_to_txfma;      // Going from TXFMA to TXFMA with small matrix config requires a bubble cycle
logic       quant_tena_scp_en;               // Enable the usage of TENA Scratchpad register
logic       quant_tena_zero_en;              // Force to read zero in TENA
logic       quant_tena_zero_en_next;
logic       quant_rdata_col;                 // Read data from scratchpad in a col-wise fashion
logic       quant_rdata_col_next;

always_ff @(posedge clock) begin
    if (reset) begin
        quant_tena_scp_en <= 1'b0;
        quant_tena_zero_en <= 1'b0;
        quant_rdata_col <= 1'b0;
        ex_rdata_zero <= 1'b0;
        ex_rdata_col <= 1'b0;
        exec_hazard_wait <= 3'b000;
        prev_is_txfma <= 1'b0;
        prev_is_shsw <= 1'b0;
    end else if (clock_en) begin
        quant_tena_scp_en <= quant_tena_scp_en_next;
        quant_tena_zero_en <= quant_tena_zero_en_next;
        quant_rdata_col <= quant_rdata_col_next;
        ex_rdata_zero <= quant_tena_zero_en;
        ex_rdata_col <= quant_rdata_col;
        exec_hazard_wait <= exec_hazard_wait_next;
        prev_is_txfma <= exec_is_txfma;
        prev_is_shsw <= exec_is_shsw;
    end
end

always_ff @(posedge clock) begin
    if (exec_adv) begin
        exec_a_row <= exec_a_row_next;
        exec_b_col <= exec_b_col_next;
    end
end

always_ff @(posedge clock) begin
    if (reset) begin
        ex_ren <= 1'b0;
    end else begin
        ex_ren <= quant_tena_en;
    end
end

always_comb
begin
    // Enable reading from TENA when reading Scp data or forcing zero
    quant_tena_en = (quant_tena_scp_en || quant_tena_zero_en) && enabled;

    exec_curr_transform = tensorquant_ctrl.trans[3:0];

    // Enabled and not waiting for sticky pending bit from tensorfma or
    // tensorstore
    quant_en_next           = enabled && !scoreboard_pend_sticky && !tfma_pend_sticky && !tstore_pend_sticky && (exec_hazard_wait == 3'b000) && (exec_curr_transform != TQUANT_LAST);
    quant_tena_scp_en_next  = 1'b0;
    quant_tena_zero_en_next = 1'b0;
    quant_rdata_col_next    = exec_curr_transform[0];


      //all lanes enabled by default
    quant_mask_next         = {VPU_LANE_NUM{1'b1}};

      //masking out vpu lanes out of the B matrix colums
    quant_mask_next[3:0] &= {4{~(( tensorquant_ctrl.cols<2)&exec_b_col)}};
    quant_mask_next[7:4] &= {4{~((( tensorquant_ctrl.cols==0)&!exec_b_col) || (( tensorquant_ctrl.cols<3)&exec_b_col))}};

    exec_dest_reg = { exec_a_row, exec_b_col } + tensorquant_ctrl.start_reg;
    exec_is_txfma = (exec_curr_transform == TQUANT_INT32_TO_FP32_CVT) || (exec_curr_transform == TQUANT_FP32_TO_INT32_CVT) ||
                    (exec_curr_transform == TQUANT_FP32_MUL_ROW) || (exec_curr_transform == TQUANT_FP32_MUL_COL);

    exec_is_shsw = (exec_curr_transform == TQUANT_RELU) || (exec_curr_transform == TQUANT_INT32_ADD_ROW) ||
                   (exec_curr_transform == TQUANT_INT32_ADD_COL) || (exec_curr_transform == TQUANT_SAT_INT8) ||
                   (exec_curr_transform == TQUANT_SAT_UINT8) || (exec_curr_transform == TQUANT_PACK_128B);


    // Mark as potential problem for next cycle if in this cycle issuing last
    // multipass cycle. It means it is a TXFMA operation and lanes 2..7 are
    // sleeping. The problem is that with multipass the writes happen a cycle
    // later. So sending TXFMA (which goes to multipass) followed by a short
    // (which doesn't do multipass) would collide with the write port.
    // Therefore, a bubble must be put when going from TXFMA multipass to
    // short.
    exec_hazard_txfma_to_short = (exec_hazard_wait == 3'b001);

    // A bubble mis also required when going from TXFMA to any unit due to the
    // configured matrix is shorter than the TXFMA pipeline
    exec_hazard_txfma_to_txfma = (exec_hazard_wait != 3'b000);


    // FP32 to INT32 convert
    if(exec_curr_transform == TQUANT_INT32_TO_FP32_CVT)
    begin
        quant_inst_next                        = FCVT_PS_PW;
        quant_inst_next[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB]      = 3'b111; // Dynamic round mode
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_txfma;
    end
    // INT32 to FP32 convert
    else if(exec_curr_transform == TQUANT_FP32_TO_INT32_CVT)
    begin
        quant_inst_next                        = FCVT_PW_PS;
        quant_inst_next[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB]      = 3'b111; // Dynamic round mode
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_txfma;
    end
    else if(exec_curr_transform == TQUANT_RELU)
    begin
        quant_inst_next                        = FMAX_PI;
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_inst_next[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_short;
        quant_tena_zero_en_next                = quant_en_next;
    end
    // INT32 Add row-wise/col-wise
    else if((exec_curr_transform == TQUANT_INT32_ADD_ROW) || (exec_curr_transform == TQUANT_INT32_ADD_COL))
    begin
        quant_inst_next                        = FADD_PI;
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_inst_next[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !scp_read_pending && !exec_hazard_txfma_to_txfma; // Can execute if data already read from SCP
        quant_tena_scp_en_next                 = quant_en_next;
    end
    // FP32 Mul row-wise/col-wise
    else if((exec_curr_transform == TQUANT_FP32_MUL_ROW) || (exec_curr_transform == TQUANT_FP32_MUL_COL))
    begin
        quant_inst_next                        = FMUL_PS;
        quant_inst_next[VPU_INST_RM_SEL_MSB:VPU_INST_RM_SEL_LSB]      = 3'b111;        // Dynamic round mode
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_inst_next[VPU_INST_REN2_RA_SEL_MSB:VPU_INST_REN2_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !scp_read_pending && !exec_hazard_txfma_to_txfma; // Can execute if data already read from SCP
        quant_tena_scp_en_next                 = quant_en_next;
    end
    // Saturation to INT8
    else if(exec_curr_transform == TQUANT_SAT_INT8)
    begin
        quant_inst_next                        = FSAT8_PI;
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_short;
    end
    // Saturation to UINT8
    else if(exec_curr_transform == TQUANT_SAT_UINT8)
    begin
        quant_inst_next                        = FSATU8_PI;
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = exec_dest_reg;
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_short;
    end
    // Pack to 128bits
    // Special case where same register is written twice (for cols >= 3 which
    // means 512bit). f0 and f1 pack write to f0. Also, as there a 2 cycle dependency
    // the execution is not done in the same order as others
    else
    begin
        quant_inst_next                        = FPACKREPB_PI;
        quant_inst_next[VPU_INST_RD_SEL_MSB:VPU_INST_RD_SEL_LSB]      = { exec_dest_reg[4:1], (tensorquant_ctrl.start_reg[0] & ~exec_b_col) } - {4'b0,(tensorquant_ctrl.start_reg[0] & exec_b_col)};
        quant_inst_next[VPU_INST_REN1_RA_SEL_MSB:VPU_INST_REN1_RA_SEL_LSB] = exec_dest_reg;
        quant_en_next                          = quant_en_next && !exec_hazard_txfma_to_short;
        quant_mask_next                        = exec_b_col ? {4'b0,(tensorquant_ctrl.cols==3),(tensorquant_ctrl.cols>=2),2'b0}  // Odd entries to 3 and 4
                                               :              {6'b0,(tensorquant_ctrl.cols>=1),1'b1};                            // Even entries write to 2 low lanes
    end


    // Keep previous values in case of hazard
    if(exec_hazard_wait != 3'b000)
    begin
        quant_tena_scp_en_next  = quant_tena_scp_en;
        quant_tena_zero_en_next = quant_tena_zero_en;
    end

    // Update the position in the tensor
    exec_a_row_next    = exec_a_row + 4'b1;
    exec_b_col_next    = exec_b_col;
    exec_last_op_trans = (exec_a_row == tensorquant_ctrl.rows) && enabled && quant_en_next;

    // Doing first even registers and then odd registers (so execute per cols).
    // This helps keeping the code more regular (Pack to 128b requires this order)
    if(tensorquant_ctrl.cols >= 2'b10)
    begin
        if(exec_a_row == tensorquant_ctrl.rows)
        begin
            exec_a_row_next = 4'b0;
            exec_b_col_next = 1'b1;
        end
        exec_last_op_trans = (exec_a_row == tensorquant_ctrl.rows) && exec_b_col && enabled && quant_en_next;
    end

    if(exec_last_op_trans || f0_core_ctrl.tensorquant_start)
    begin
        exec_a_row_next = 4'b0;
        exec_b_col_next = 1'b0;
    end

      // Computes the next hazard wait
    exec_hazard_wait_next = exec_hazard_wait;

    // If didn't get to end, continue decrementing
    if(exec_hazard_wait != 3'b000) begin
      exec_hazard_wait_next = exec_hazard_wait_next - 3'b001;
    end
    // required hazard wait if previous operation is a txfma and:
    // - current op is configured with number of rows shorter than txfma pipeline(7)
    // - current op is the last one and can have operand dependences with next instruction
    else if ((quant_en_next && prev_is_txfma && exec_last_op_trans && ((tensorquant_ctrl_next.rows<3'b111) || new_req)) ||
             ((prev_is_txfma || prev_is_shsw) && (tensorquant_ctrl[3:0] == TQUANT_LAST))) begin
      exec_hazard_wait_next = 3'b111;
    end
    // required hazard wait if previous operation is a sh-sw and current op is configured with number of rows shorter than sh-sw pipeline(2)
    else if ((((quant_en_next && exec_last_op_trans) || new_req) && (tensorquant_ctrl_next.rows<3'b010))) begin
      exec_hazard_wait_next = 3'b100;
    end

    // Clock enable
    exec_adv = quant_en_next || new_req;
end


////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////
assign vpu_tensorquant_debug = {enabled,
                                scoreboard_pend_sticky,
                                tfma_pend_sticky,
                                tstore_pend_sticky,
                                exec_hazard_wait[1:0],
                                exec_curr_transform,
                                dcache_scp_available
                               };


endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on WIDTHEXPAND */

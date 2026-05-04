// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_tensorreduce (
  // System signals
  input  logic                      clock,
  input  logic                      reset,
  // Control port from core to start new TensorReduce
  input  logic                      reduce_start,
  input  logic                      tensorstore_start,
  input  logic                      tensorfma_start,
  input  logic                      tensorquant_start,
  input  reduce_tensorstore_control reduce_ctrl,
  output logic                      enabled,
  // Dependencies with other units
  input  logic                      scoreboard_pend,
  input  logic                      tensorfma_pend,
  input  logic                      tensorquant_pend,
  output logic                      reduce_wait,
  // Control coming from Dcache
  input  dcache_vpu_reduce_ctrl     dcache_reduce_ctrl,
  // Instructions stuffed in the VPU pipeline
  output vpu_ml_load_ctrl           load_ctrl,
  output logic                      reduce_inst_en_next,
  output logic [`INST_RANGE]        reduce_inst_next,
  // debug
  output logic [24:0]               vpu_tensorreduce_debug
);

////////////////////////////////////////////////////////////////////////////////
// Reduce
// This instruction reduces the contents of the VPU RF between two minions.
// The instruction can specify the starting RF entry, the number of RF
// entries, the reduction operation (float/int, add/sub/max/min) and it also
// specifies the pairing minion.
// One minion behaves as sender and another as receiver. Once the receiver
// minion receives the reduce instruction, it sends a message to the sender
// minion saying that it is ready to accept packets.
// The sender minion once sees that the other minion is ready to receive, it
// will start sending the contents of the requested VPU RF lines.
// The receiver will get the contents, write it to the buffer array, and then
// ship it to the VPU which will do the reduce op and write the final results.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// New request and pending count
////////////////////////////////////////////////////////////////////////////////

  logic [6:0] reg_pending;                 // Number of pending registers to be processed
  logic [6:0] reg_pending_next;
  logic [4:0] cur_reg;                     // Current reg
  logic [4:0] cur_reg_next;
  logic [3:0] reduce_op;                   // Current reduce op
  logic [3:0] reduce_op_next;
  logic [2:0] exec_op_cd;                  // Count down from last exec op to clear enabled bit
  logic [2:0] exec_op_cd_next;
  logic [1:0] cols_per_row;                // Cols per row for tensor store
  logic       new_req;                     // New request coming in
  logic       reg_up;                      // Register pending update
  logic       send_reg;                    // Send a new register request from dcache reduce unit
  logic       exec_op;                     // Execute the reduce operation
  logic       exec_op_s0;                  // Execute the reduce operation (next cycle)
  logic       enabled_int;                 // Internal enable to send oeprations
  logic       scoreboard_pend_sticky;      // Waits for previous TensorFMA to finish writes to VPU RF before starting TensorReduce
  logic       scoreboard_pend_sticky_next; //
  logic       tfma_pend_sticky;            // Waits for previous TensorFMA to finish writes to VPU RF before starting TensorReduce
  logic       tfma_pend_sticky_next;       //
  logic       tquant_pend_sticky;          // Waits for previous TensorQuant to finish writes/reads to VPU RF before starting TensorReduce
  logic       tquant_pend_sticky_next;     //
  logic       clock_en;                    // General clock enable
  logic       is_reduce;                   // Tensor operation is reduce


  //         CLK    RST    EN        DOUT                    DIN                          DEF
  `RST_EN_FF(clock, reset, reg_up,   reg_pending,            reg_pending_next,            7'b0)
  `RST_EN_FF(clock, reset, enabled,  exec_op_cd,             exec_op_cd_next,             3'b0)
  `EN_FF    (clock,        reg_up,   cur_reg,                cur_reg_next)
  `EN_FF    (clock,        new_req,  reduce_op,              reduce_op_next)
  `EN_FF    (clock,        new_req,  is_reduce,              reduce_start)
  `EN_FF    (clock,        new_req,  cols_per_row,           reduce_ctrl.tensorstore.cols)
  `RST_EN_FF(clock, reset, clock_en, scoreboard_pend_sticky, scoreboard_pend_sticky_next, 1'b0)
  `RST_EN_FF(clock, reset, clock_en, tfma_pend_sticky,       tfma_pend_sticky_next,       1'b0)
  `RST_EN_FF(clock, reset, clock_en, tquant_pend_sticky,     tquant_pend_sticky_next,     1'b0)
  `FF       (clock,                  send_reg,               dcache_reduce_ctrl.send_reg)
  `FF       (clock,                  exec_op_s0,             dcache_reduce_ctrl.exec_op)
  `FF       (clock,                  exec_op,                exec_op_s0)

  always_comb begin
    // Generates enabled signal
    enabled_int = (reg_pending != 7'b0);
    // We need to prevent new VPU instructions coming in until we know that
    // the last op result can be consumed with the bypass. We use a count down
    // from last execution op to guarantee that
    enabled = enabled_int || (|exec_op_cd);

    new_req = reduce_start || tensorstore_start;
    reg_up = new_req
          || (send_reg && enabled_int)
          || (exec_op && enabled_int);

    // Enable clocks when working or starting a new request
    clock_en = enabled || new_req;

    // Selects between reduce op and src_inc for tensor store
    reduce_op_next = reduce_ctrl.reduce.op;
    if (tensorstore_start)
      reduce_op_next = {2'b0, reduce_ctrl.tensorstore.src_inc};

    reg_pending_next = reg_pending;
    // Computes next register pending
    if (is_reduce) begin
      reg_pending_next = reg_pending - 7'b1;
      cur_reg_next     = cur_reg + 6'b1;
    end else begin
      // Decrement cols
      reg_pending_next[2:0] = reg_pending[2:0] - 3'b1;
      cur_reg_next          = cur_reg + reduce_op + 6'b1;

      // Col done, move to next row
      if (reg_pending_next[2:0] == 3'b000) begin
        reg_pending_next[2:0] = cols_per_row[1] + 3'b1;
        reg_pending_next[6:3] = reg_pending[6:3] - 4'b1;
        // Saturate at 0
        if (reg_pending[6:3] == 4'b000)
          reg_pending_next = 7'b0;
      end
    end

    if (new_req && reduce_start) begin
      reg_pending_next = reduce_ctrl.reduce.num_regs;
      cur_reg_next     = reduce_ctrl.reduce.start_reg;
    end else if (new_req && tensorstore_start) begin
      reg_pending_next[2:0] = reduce_ctrl.tensorstore.cols[1] + 3'b1;
      reg_pending_next[6:3] = reduce_ctrl.tensorstore.rows;
      cur_reg_next          = reduce_ctrl.tensorstore.start_reg;
    end

    // Clear upon nothing action
    if (dcache_reduce_ctrl.nothing) begin
      reg_pending_next = 7'b0;
      reg_up           = 1'b1;
    end

    // Decrements count down unless a new exec op is sent
    exec_op_cd_next = exec_op_cd - 3'b1;
    if (exec_op_cd == 3'b000)
      exec_op_cd_next = 3'b000;
    if (exec_op)
      exec_op_cd_next = 3'b100;

    // Computes sticky bit for tensorfma write pending
    scoreboard_pend_sticky_next = scoreboard_pend_sticky;
    tfma_pend_sticky_next       = tfma_pend_sticky;
    tquant_pend_sticky_next     = tquant_pend_sticky;
    // Get if pending RF write
    if (new_req && (reg_pending_next != 7'b0)) begin
      scoreboard_pend_sticky_next = scoreboard_pend;
      tfma_pend_sticky_next       = tensorfma_pend || tensorfma_start;
      tquant_pend_sticky_next     = tensorquant_pend || tensorquant_start;
    end else begin
      // Clear sticky bit when write done
      if (enabled && scoreboard_pend_sticky && !scoreboard_pend)
        scoreboard_pend_sticky_next = 1'b0;
      // Clear sticky bit when write done
      if ((enabled && tfma_pend_sticky && !tensorfma_pend) || tensorfma_start)
        tfma_pend_sticky_next = 1'b0;
      // Clear sticky bit when write done
      if ((enabled && tquant_pend_sticky && !tensorquant_pend) || tensorquant_start)
        tquant_pend_sticky_next = 1'b0;
    end
    // As VPU is a slave, send a bit to dcache to stall reduce/store until
    // sticky bits are resolved
    reduce_wait = scoreboard_pend_sticky || tfma_pend_sticky || tquant_pend_sticky;
  end

////////////////////////////////////////////////////////////////////////////////
// Generates either:
//   - a fake store in order to deal with send register operations
//   - a reduce operation to deal with the receive register
////////////////////////////////////////////////////////////////////////////////

  logic [4:0] cur_reg_next_mux; // Selects the next current reg taking into account the write enable
  always_comb begin
    cur_reg_next_mux = reg_up ? cur_reg_next : cur_reg;

    // Send operations have to read data from VRF and ship it to dcache
    // A store is stuffed in the pipeline in order to do so
    // Execute operations send an FADD_PS, ...
    reduce_inst_en_next = (dcache_reduce_ctrl.send_reg || (exec_op_s0 && (reduce_op != 8))) && enabled_int;

    // Send case
    reduce_inst_next        = `FSW_PS;
    reduce_inst_next[11:7]  = cur_reg_next_mux; // Don't care, same as exec case
    reduce_inst_next[19:15] = cur_reg_next_mux; // Don't care, sane as exec case
    reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    // Exec case
    if (exec_op_s0 && (reduce_op == 4'b0000)) begin
      reduce_inst_next        = `FADD_PS;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[14:12] = 3'b111;           // Dynamic rounding
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b0010)) begin
      reduce_inst_next        = `FMAX_PS;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b0011)) begin
      reduce_inst_next        = `FMIN_PS;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b0100)) begin
      reduce_inst_next        = `FADD_PI;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b0110)) begin
      reduce_inst_next        = `FMAX_PI;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b0111)) begin
      reduce_inst_next        = `FMIN_PI;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end else if (exec_op_s0 && (reduce_op == 4'b1000)) begin
      reduce_inst_next        = `FCMOVM_PS;
      reduce_inst_next[11:7]  = cur_reg_next_mux; // Destination is current reg
      reduce_inst_next[19:15] = cur_reg_next_mux; // Src2 is current reg
      reduce_inst_next[24:20] = cur_reg_next_mux; // Src2 is current reg
    end

    // Need to write to current reg without doing broadcast for
    // receiver case
    load_ctrl.wen           = exec_op;
    load_ctrl.thread_id     = 1'b0;    // Tensor reduce only supported at thread 0
    load_ctrl.broadcast_sel = 2'b00;
    load_ctrl.waddr         = cur_reg;
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Debug
  ////////////////////////////////////////////////////////////////////////////////
  assign vpu_tensorreduce_debug = {reg_pending,
                                   exec_op_cd,
                                   cur_reg,
                                   reduce_op,
                                   is_reduce,
                                   cols_per_row,
                                   scoreboard_pend_sticky,
                                   tfma_pend_sticky,
                                   tquant_pend_sticky
                                  };

endmodule


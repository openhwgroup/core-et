// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

typedef struct packed {
  logic [`VPU_FCMD_SZ-1:0] cmd;
  logic  txfma;
  logic  shsw;
  logic  rom;
  logic  scatter;
  logic  cvt;
  logic  trans;
  logic  gcvt;
  logic  fromintf;
  logic  tointm;
  logic  tointf;
  logic  maskop;
  logic  ldst;
  logic  m0ren;
  logic  mallren;
  logic  mren1;
  logic  mren2;
  logic  wen;
  logic  ren1;
  logic  ren2;
  logic  ren3;
  logic  rend;
  logic  swap12;
  logic  swap23;
  logic  swap13;
  logic  vector;
  logic  [`VPU_CTRL_SIGS_DTYPE_SZ-1:0] dtype;
  logic  fromint;
  logic  toint;
  logic  fma;
  logic  fms;
  logic  add;
  logic  sub;
  logic  mul;
  logic  round;
  logic  wflags;
} vpu_ctrl_sigs_t;

// VPU output struct 
typedef struct packed {
  logic [`VPU_DATA_S_SZ-1:0] data;
  logic [`VPU_FLAGS_SZ-1:0]  exc;
} vpu_output_t;

typedef struct packed {
  logic            use_prev_sigs;  
  vpu_ctrl_sigs_t  sigs;
  logic [19:0]     imm;
  logic [2:0]      rm;
  logic [1:0]      typ;
  logic [31:0]     in1;
  logic [31:0]     in2;
  logic [31:0]     in3;
} vpu_input_t;

// Core to VPU control signals
typedef struct packed {
    logic       tensorfma_start; // Start an TensorFMA instruction
    logic [7:0] tensorfma_scp_a; // Where in the scratchpad the matrix A is stored
    logic [7:0] tensorfma_scp_b; // Where in the scratchped the matrix B is stored
} vpu_core_ctrl;

// VPU control signals from ML
typedef struct packed {
    logic                                      wen;           // Write new load data
    logic                                      thread_id;     // Which thread the write is going to
    logic [`VPU_ML_LOAD_CTRL_CHUNK_IDX_SZ-1:0] broadcast_sel; // Select which chunk to broadcast
    logic [`VPU_ML_LOAD_CTRL_WADDR_SZ-1:0]     waddr;         // RF write address for the loads
} vpu_ml_load_ctrl;

// VPU control signals fot trans unit

// TransDiv writeback
typedef struct packed {
  logic thread_id;
  logic vector;
  logic [`VPU_LANE_NUM-1:0]           wmask;
  logic [`FREG_ADDR_RANGE]            waddr;
} vpu_trans_div_info_type_t;

typedef struct packed {
    logic txfma_in0;
    logic txfma_in1;
    logic txfma_in2;
    logic shsw_in1;
    logic shsw_in2;
} vpu_bypass_force_ctrl;



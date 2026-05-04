// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

typedef enum logic [2:0] 
{
   csr_cmd_N = 0,
   csr_cmd_W = 1,
   csr_cmd_S = 2,
   csr_cmd_C = 3,
   csr_cmd_I = 4,
   csr_cmd_R = 5
} csr_cmd_t;


typedef enum logic [2:0] 
{
   msg_port_32  = 3'd2,
   msg_port_64  = 3'd3,
   msg_port_128 = 3'd4,
   msg_port_256 = 3'd5
`ifdef SUPPORT_512_MSGS              
 , msg_port_512 = 3'd6
`endif
} core_msg_port_logsize_t;


typedef struct packed
{
   logic                                   umode;
   core_msg_port_logsize_t                 logsize;
   logic [`CORE_MSG_PORT_MAX_MSGS_SZ-1:0]  max_msgs;
   logic [`DCACHE_SET_RANGE]               scp_set;
   logic [`DCACHE_WAY_RANGE]               scp_way;
   logic                                   enable_oob;
} core_msg_port_conf_t;


`ifdef SUPPORT_512_MSGS              
// returns nr passes of 256 bits - 1
// DCache supports up to 256 bits transfer in a single cycle
function automatic [1:0] msg_port_fill_passes_cnt;
   input     core_msg_port_logsize_t size;
   begin
      case (size) 
        msg_port_32, msg_port_64, msg_port_128, msg_port_256:
          msg_port_fill_passes_cnt = 0;
        msg_port_512:
          msg_port_fill_passes_cnt = 1;
        default:
          msg_port_fill_passes_cnt = 0;
      endcase // case (size)
   end
endfunction
`endif

typedef enum logic [4:0]
 {
  CSR_REPLAY_CACHE_OP = 0, 
  CSR_REPLAY_FLB,
  CSR_REPLAY_TL0,
  CSR_REPLAY_TL1,
  CSR_REPLAY_TFMA,
  CSR_REPLAY_TQUANT,
  CSR_REPLAY_TREDUCE,
  CSR_REPLAY_TS,
  CSR_REPLAY_CONV,
  CSR_REPLAY_FCC0,
  CSR_REPLAY_FCC1,
  CSR_REPLAY_TMASK,
  CSR_REPLAY_PMU_READ,
  CSR_REPLAY_TW_TL0,
  CSR_REPLAY_TW_TL1,
  CSR_REPLAY_TW_PREFETCH0,
  CSR_REPLAY_TW_PREFETCH1,
  CSR_REPLAY_TW_CACHE_OP,
  CSR_REPLAY_TW_TENSOR_FMA,
  CSR_REPLAY_TW_TENSOR_STORE,
  CSR_REPLAY_TW_TENSOR_REDUCE,
  CSR_REPLAY_TW_TENSOR_QUANT,
  CSR_REPLAY_SZ
} csr_replay_t;


typedef struct packed {
   logic [`DCACHE_TL_ERROR_BITS-3:0] tload;
   logic                             fcc;
   logic                             scp_unset;
   logic                             co;
   logic                             tenb;
   logic                             mem_fault;
   logic                             tstore;
   logic                             treduce;
} terror_t0_t;

             
typedef struct packed {
   logic                             fcc;
   logic                             scp_unset;
   logic                             co;
   logic                             mem_fault;
} terror_t1_t;


 

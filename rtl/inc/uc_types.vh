// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

typedef logic[`BARRIER_REGISTERS-1:0][`CSR_FL_BARRIER_LIMIT_RANGE] uc_esr_barrier_values_t;

typedef logic[`UC_FCC_REGISTERS-1:0] uc_esr_fcc_enable_sigs;

typedef logic[`BARRIER_REGISTERS-1:0] uc_esr_flb_enable_sigs;

typedef struct packed
{
    uc_esr_flb_enable_sigs barriers;
    uc_esr_fcc_enable_sigs fcc;
} uc_esr_enable_sigs;

typedef struct packed
{
    uc_esr_barrier_values_t barriers;
} uc_esr_values_t;

//typedef uc_reqq_data_t  logic [`UC_REQ_DATA_SIZE-1:0];

typedef struct packed
{
    logic                                             resp;   // if 1 the minion needs the ACK
    logic   [`ET_LINK_ID_SIZE-1:0]                      id;
    logic   [`UC_REQ_DATA_SIZE-1:0]                   data;
    et_link_size_t                                    size;
    et_link_req_opcode_t                            opcode;
    logic   [`PA_SIZE-1:0]                         address;
    logic   [`ET_LINK_SOURCE_SIZE-1:0]              source;
    logic   [`SC_PORT_ID_SIZE-1:0]                 port_id;
    logic   [`DV_TRANS_ID_SIZE-1:0]               trans_id;
    logic                                           use_l3;
    logic                                            bcast;
} uc_reqq_entry_t;

typedef struct packed   // Entry is valid if any of the entries is valid
{
  logic wait_ack;       // waits an ACK from tosys/tol3 interfaces
  logic wait_from_sys;  // waits a rsp from from_sys interface (atomics)
} uc_reqq_valid_t;

typedef struct packed
{
    logic   [`SC_MESH_MASTER_AXI_DATA_SIZE-1:0]    data;
    logic   [`PA_SIZE-1:0]                      address;
    et_link_size_t                                 size;
    et_link_req_opcode_t                         opcode;
    logic   [`SC_MESH_MASTER_AXI_ID_SIZE-1:0]    axi_id; // id that has to be returned on the response
    logic                                         l3_en;       
} uc_noc_master_req_t;

typedef struct packed
{
    logic   [`PA_SIZE-1:0]                      address;
    et_link_size_t                                 size;
    et_link_req_opcode_t                         opcode;
    logic   [`SC_MESH_MASTER_AXI_ID_SIZE-1:0]    axi_id; // id that has to be returned on the response
    logic                                         l3_en;       
} uc_noc_master_req_nodata_t;


typedef struct packed {
  logic [`SC_PORT_ID_SIZE-1:0]           port_id;
  logic [`ET_LINK_SOURCE_SIZE-1:0]        source;
  logic [`ESR_BROADCAST_DATA_SIZE-1:0]      data;
} uc_esr_broadcast_t;

typedef struct packed
{
  logic [`SC_MESH_SLAVE_AXI_ID_SIZE-1:0]    id;
  axi_resp_t                              resp;
} uc_sb_axi_rsp_t;

typedef struct packed
{
  logic [`ET_LINK_ID_SIZE-1:0]           id;
  et_link_size_t                       size;
  logic [`ET_LINK_DEST_SIZE-1:0]       dest;
  et_link_rsp_opcode_t               opcode;
  logic [`SC_PORT_ID_SIZE-1:0]      port_id;
  logic [`DV_TRANS_ID_SIZE-1:0]    trans_id;
  // qwen, wdata are constants
} uc_decode_rsp_t;


///////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////

function automatic bit uc_is_broadcast;
  input [`PA_SIZE-1:0] addr;
  
  uc_is_broadcast = addr inside { `BROADCAST_ADDR_UM,
				  `BROADCAST_ADDR_SM,
				  `BROADCAST_ADDR_DM,
				  `BROADCAST_ADDR_MM };				  
endfunction  

function automatic bit uc_is_broadcast_esr;
  input [`PA_SIZE-1:0] addr;
  
  uc_is_broadcast_esr = addr inside { `BROADCAST_ADDR_ESR };
				  
endfunction  

function automatic bit uc_is_cacheop;
  input et_link_req_opcode_t opcode;
  
  uc_is_cacheop = opcode inside { ET_LINK_REQ_Flush,
				  ET_LINK_REQ_FlushToMem,
				  ET_LINK_REQ_Evict,
				  ET_LINK_REQ_EvictToMem,
				  ET_LINK_REQ_Prefetch } ;
endfunction

function automatic bit uc_is_atomic;
  input et_link_req_opcode_t opcode;
  
  uc_is_atomic = opcode inside { ET_LINK_REQ_Atomic } ;

endfunction 

function automatic bit uc_is_msg;
  input et_link_req_opcode_t opcode;
  
  uc_is_msg = opcode inside { ET_LINK_REQ_MsgSendData } ;

endfunction // uc_is_msg

function automatic bit uc_is_scp;
  input logic[`SC_MESH_MASTER_AXI_ADDR_SIZE-1:0] addr;

  uc_is_scp = (addr[`AD_SCP_REGION_RANGE] == `AD_SCP_REGION);

endfunction // uc_is_scp

function automatic bit uc_is_write;
  input et_link_req_opcode_t opcode;
  
  uc_is_write = opcode inside { ET_LINK_REQ_Write } ;

endfunction


function automatic logic[`ET_LINK_QWEN_SIZE-1:0] uc_qwen;
  input et_link_size_t               size;
  input logic [`UC_ALIGN_BITS_RANGE] addr_lsb;

  logic [`ET_LINK_QWEN_SIZE-1:0] qwen;
  logic [1:0] offset;

  case (size)
    ET_LINK_Byte, ET_LINK_HWord, ET_LINK_Word,
    ET_LINK_DWord, ET_LINK_QWord: begin
      offset = addr_lsb[5:4];
      qwen = 4'b0001 << offset;
    end
    ET_LINK_HLine: begin
      offset = addr_lsb[5:4];
      offset = offset > 2 ?  2 : offset; //clip it
      qwen = 4'b0011 << offset;
    end
    default : qwen = 4'b1111;

  endcase // case (size)

  uc_qwen = qwen;
  
endfunction

function automatic logic[`ET_LINK_QWEN_SIZE-1:0] uc_qwen_unalign;
  input et_link_size_t          size;

  logic [`ET_LINK_QWEN_SIZE-1:0] qwen;

  case (size)
    ET_LINK_Byte, ET_LINK_HWord, ET_LINK_Word,
    ET_LINK_DWord, ET_LINK_QWord: begin
      qwen = 4'b0001;
    end
    ET_LINK_HLine: begin
      qwen = 4'b0011;
    end
    default : qwen = 4'b1111;
  endcase // case (size)

  uc_qwen_unalign = qwen;
  
endfunction // uc_qwen_unalign

function automatic bit uc_slv_is_msg;
  input logic[`SC_MESH_MASTER_AXI_ADDR_SIZE-1:0] addr;
  logic hart_msg;
  logic reduce_msg;

  hart_msg = addr[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION &&
	     addr[`AD_ESR_PROT_BITS_RANGE] == `AD_ESR_PROT_BITS_UMODE &&
	     addr[`AD_ESR_AGENT_TYPE_RANGE] == `AD_ESR_AGENT_TYPE_MINION && 
	     addr[`AD_ESR_PORT_MSG_START+:`AD_ESR_PORT_MSG_SIZE] == (`AD_ESR_PORT_MSG_COMMON | (addr[`AD_ESR_PORT_ID_RANGE] << `AD_ESR_PORT_ID_START));

  reduce_msg = addr[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION &&
	       addr[`AD_ESR_PROT_BITS_RANGE] == `AD_ESR_PROT_BITS_MSG &&
	       addr[`AD_ESR_AGENT_TYPE_RANGE] == `AD_ESR_AGENT_TYPE_MINION && 
	       addr[`AD_ESR_PORT_MSG_START+:`AD_ESR_PORT_MSG_SIZE] inside {`AD_ESR_PORT_MSG_REDUCE_DATA, `AD_ESR_PORT_MSG_REDUCE_READY};


  uc_slv_is_msg = hart_msg || reduce_msg;

endfunction // uc_slv_is_msg


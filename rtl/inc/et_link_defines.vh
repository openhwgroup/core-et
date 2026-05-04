// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _ET_LINK_DEFINES_
`define _ET_LINK_DEFINES_

////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

`define ET_LINK_ID_SIZE         8
`define ET_LINK_SOURCE_SIZE     $bits(et_link_neigh_source_t)
`define ET_LINK_DEST_SIZE       `ET_LINK_SOURCE_SIZE
`define ET_LINE_DATA_SIZE       512
`define ET_LINK_QWEN_SIZE       4
`define ET_LINK_SUBOPCODE_SIZE  7

`define ET_CORE_MISS_ID_SIZE  $bits(et_link_core_miss_id_t)
`define ET_CORE_MISS_ID_RANGE `ET_CORE_MISS_ID_SIZE-1:0
`define ET_CORE_MISS_ID_COOP  ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0

// Messages
`define ET_MSG_ID_SIZE       8

`define ET_MSG_VALID_BIT     2
`define ET_MSG_VALID_MASK    (`ET_MSG_ID_SIZE)'(`ET_MSG_ID_SIZE'b1<<`ET_MSG_VALID_BIT)
`define ET_MSG_PORT_ID_SIZE  2
`define ET_MSG_PORT_ID_START 3
`define ET_MSG_PORT_ID_MASK  (`ET_MSG_ID_SIZE)'(((`ET_MSG_ID_SIZE'b1<<`ET_MSG_PORT_ID_SIZE)-`ET_MSG_ID_SIZE'b1)<<`ET_MSG_PORT_ID_START)

// Atomics
`define ET_ATOMIC_OPCODE_SIZE  4
`define ET_ATOMIC_OPCODE_START 0
`define ET_ATOMIC_VECTOR_SIZE  1
`define ET_ATOMIC_VECTOR_START (`ET_ATOMIC_OPCODE_START+`ET_ATOMIC_OPCODE_SIZE)
`define ET_ATOMIC_SIZE_SIZE    1
`define ET_ATOMIC_SIZE_START   (`ET_ATOMIC_VECTOR_START+`ET_ATOMIC_VECTOR_SIZE)
`define ET_ATOMIC_DEST_SIZE    1
`define ET_ATOMIC_DEST_START   (`ET_ATOMIC_SIZE_START+`ET_ATOMIC_SIZE_SIZE)
`define ET_ATOMIC_DEST_LOCAL   1'b0
`define ET_ATOMIC_DEST_GLOBAL  1'b1
`define ET_ATOMIC_CONFIG_SIZE  (`ET_ATOMIC_OPCODE_SIZE+`ET_ATOMIC_VECTOR_SIZE+`ET_ATOMIC_SIZE_SIZE+`ET_ATOMIC_DEST_SIZE)

`endif // _ET_LINK_DEFINES_

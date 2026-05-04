// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _IOSHIRE_TILELINK_DEFINES_
`define _IOSHIRE_TILELINK_DEFINES_

`define M_PutFullData     3'd0
`define M_PutPartialData  3'd1
`define M_ArithmeticData  3'd2
`define M_LogicalData     3'd3
`define M_Get             3'd4
`define M_Hint            3'd5
`define M_AcquireBlock    3'd6
`define M_AcquirePerm     3'd7
`define M_Probe           3'd6
`define M_AccessAck       3'd0
`define M_AccessAckData   3'd1
`define M_HintAck         3'd2
`define M_ProbeAck        3'd4
`define M_ProbeAckData    3'd5
`define M_Release         3'd6
`define M_ReleaseData     3'd7
`define M_Grant           3'd4
`define M_GrantData       3'd5
`define M_ReleaseAck      3'd6
`define M_GrantAck        3'd0


`define M_A_OPCODE_WIDTH     3                  // TL2 fixed
`define M_A_PARAM_WIDTH      3                  // TL2 fixed
`define M_A_SIZE_WIDTH       2                  // application specific; for 8-byte data, size should be 3; hence, 2 bits;
`define M_A_SOURCE_WIDTH     9                  // application specific; // TBR: NOT SURE WHERE 9 IS FROM. CHECK!
`define M_A_ADDRESS_WIDTH   28                  // application specific; // TBR: I think this is incorrect for BoomV3. Need to verify;
`define M_A_MASK_WIDTH       8                  // application specific; // TBR: For BoomV2, it makes sense; for Maxion, it may not. Need to consider NOC. 
`define M_A_DATA_WIDTH       (8*`M_A_MASK_WIDTH)     // TL2 fixed
`define M_A_MASK_WIDTH_32    4                       // used in PLIC
`define M_A_DATA_WIDTH_32    (8*`M_A_MASK_WIDTH_32)  // used in PLIC

`define M_D_OPCODE_WIDTH     3                  // TL2 fixed
`define M_D_PARAM_WIDTH      2                  // TL2 fixed
`define M_D_SIZE_WIDTH       `M_A_SIZE_WIDTH    // application specific; Same as Channel A;
`define M_D_SOURCE_WIDTH     `M_A_SOURCE_WIDTH  // application specific; Same as Channel A;
`define M_D_SINK_WIDTH       `M_D_SOURCE_WIDTH  // application specific; Same as source;
`define M_D_ADDRESS_WIDTH    `M_A_ADDRESS_WIDTH // application specific; Same as Channel A;
`define M_D_MASK_WIDTH       `M_A_MASK_WIDTH    // application specific; Same as Channel A;
`define M_D_DATA_WIDTH       (8*`M_D_MASK_WIDTH)     // TL2 fixed
`define M_D_MASK_WIDTH_32    `M_A_MASK_WIDTH_32      // used in PLIC
`define M_D_DATA_WIDTH_32    (8*`M_D_MASK_WIDTH_32)  // used in PLIC

`define M_A_ADDRESS_LSB      $clog2(`M_A_MASK_WIDTH)
`define M_D_ADDRESS_LSB      `M_A_ADDRESS_LSB

`endif // _IOSHIRE_TILELINK_DEFINES_



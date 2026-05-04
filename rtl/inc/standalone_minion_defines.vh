// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _STDALMIN_DEFINES_
`define _STDALMIN_DEFINES_

////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

`define STDALMIN_NUM_AGENTS 4

// APB
`define STDALMIN_APB_ADDR_WIDTH             (`SPIO_DBG_BPAM_APB_ADDR_WIDTH - 5)
`define STDALMIN_SM_MUX_WIDTH               `NEIGH_DEBUG_SM_MUX_WIDTH

`endif // _STDALMIN_DEFINES_

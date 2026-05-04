// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _IOSHIRE_PLIC_DEFINES_
`define _IOSHIRE_PLIC_DEFINES_

// ioshire_plic.vh

//// The following are placed in TLPLIC as localparam s.
/*
// PLICConsts
`define M_maxDevices                  1023
`define M_maxHarts                    15872
`define M_priorityBase                'h0
`define M_pendingBase                 'h1000
`define M_enableBase                  'h2000
`define M_hartBase                    'h200000

`define M_claimOffset                 4
`define M_priorityBytes               4

`define M_enableOffset(i)             i * ((maxDevices+7)/8)
`define M_hartOffset(i)               i * 'h1000
`define M_enableBase(i)               M_enableOffset(i) + M_enableBase
`define M_hartBase(i)                 M_hartOffset(i) + M_hartBase

`define M_size                        M_hartBase(M_maxHarts)
//
//  require(M_hartBase >= M_enableBase(maxHarts))
//
*/




// PLICParams

// We no longer use full addresses in the PLICs.  We just pass in the address
// offset.  The APB will route the request to the PLIC only if the request's
// base address matches the base address of the region assigned to the PLIC.
//`define M_baseAddress                 32'h0C00_0000   // TBR: double-check on memory map

// The address region assigned to the PLIC is 32M in size.  We are assuming that
// the region always starts on a 32M boundary.
`define M_PLIC_ADDR_WIDTH            25


// Number of priority levels supported by the PLIC.  Does not include the 0 level
// (interrupt disabled).
`define M_maxPriorities               7               // Must be a positive integer


//// intStages: Int = 0)
//// {
////   require (maxPriorities >= 0)
////   def address = AddressSet(baseAddress, PLICConsts.size-1)
//// }

`endif // _IOSHIRE_PLIC_DEFINES_

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//
// Global defines for various DFT aspects of the design.
// 

`ifndef _DFT_DEFINES_VH_
`define _DFT_DEFINES_VH_

// Define the encoding of the 4-bit dft__bc_cntl[7:4]->dft__test_mode[3:0] field of dft__bc_cntl bus

// dft__test_mode[3:0] encoding
// The current map (as of 18 June 2018) for the dft__test_mode[3:0] bus in ATPG mode is (see corresponding RTL below):

// TestMode                   Encoding          Notes/Corresponding HUB signal (if any)
// Mission mode               0000
// Bypass_single_chain        0001              dft__byp_single
// Bypass_regular             0010              dft__byp_multi  [bypass multi-chain mode]  FIXED_DFT: confirmed with Vinay that all flops are on these chains
// Bypass_macrotest           0011              dft__byp_macro  
// Bypass_extest              0100              dft__byp_ext
// Compression_intest         0101              dft__cmp_in
// Compression_extest         0110              dft__cmp_ex      *Note: For SOC A0, this mode is not used or supported
// Compression_extest_m02     0111              dft__cmp_ex_m02  *Note: For SOC A0, this mode is not used or supported (aka dft__cmp_ex_ddr, see note below]
//
//                                              *Note: Possible encodings added 9 aug 2019, primarily for HM interface
// DFT_TM_RESERVED_8          1000
// DFT_TM_RESERVED_9          1001              
// DFT_TM_RESERVED_A          1010              
// DFT_TM_RESERVED_B          1011
// DFT_TM_RESERVED_C          1100
// DFT_TM_RESERVED_D          1101
// DFT_TM_RESERVED_E          1110
// DFT_TM_RESERVED_F          1111
//
// --------START: ADDITIONAL NOTES-------
//
// // Mode 02 of dft__cmp_ex  (special case to deal with a) ddr_chip <-> memshire_x_top boundary; b)tbd)
// [FIXED_DFT:?Compression_extest_ddr?]     0111              dft__cmp_ex_ddr  [moot for SOC A0, not implemented or supported]
//
// Reserved-B                 1000-1111         [FIXED_DFT: reserve bank B for some other major function]
//
// --------END:   ADDITIONAL NOTES-------

// FIXED_DFT:  dft_bc_cntl_map.v qualifes signals with scan_mode [was: Generate a macro for DFT_SCAN_MODE, and use the same to qualify the ATPG related signals]
// FIXed_DFT:  dft_bc_cntl_map.v qualifes signals for each mode  [was: Corralary/use model is to retarget the dft__bc_cntl[] bus for other purposes when not in DFT_SCAN_MODE]

`define DFT_TM_MISSION          4'b0000
`define DFT_TM_BYP_SINGLE       4'b0001
`define DFT_TM_BYP_MULTI        4'b0010
`define DFT_TM_BYP_MACRO        4'b0011
`define DFT_TM_BYP_EXTEST       4'b0100
`define DFT_TM_CMP_INTEST       4'b0101
`define DFT_TM_CMP_EXTEST       4'b0110
`define DFT_TM_CMP_EXTEST_M02   4'b0111

`define DFT_TM_RESERVED_8       4'b1000
`define DFT_TM_RESERVED_9       4'b1001
`define DFT_TM_RESERVED_A       4'b1010
`define DFT_TM_RESERVED_B       4'b1011
`define DFT_TM_RESERVED_C       4'b1100
`define DFT_TM_RESERVED_D       4'b1101
`define DFT_TM_RESERVED_E       4'b1110
`define DFT_TM_RESERVED_F       4'b1111

//------------------------------------------
// 3rd party test mode Aux input control bus
//------------------------------------------
// Define the bit positions of the N-bit dft__bc_cntl_ip[X:Y]->dft__ip_aux_in[A:B] fields of dft__bc_cntl bus
// 10 aug 2019: this was initially hardcoded as dft__bc_cntl_ip[31:12] -> dft__ip_aux_in[19:0]
//              No change on widths, just need to make the code more portable.
//              Note: legnths of these ranges must be the same
`define DFT_IP_AUX_IN_W        20
`define DFT_BC_CNTL_IP_RANGE   31:12
`define DFT_IP_AUX_IN_RANGE    19:0


//------------------------------------------
// MBIST test mode Aux input control bus
//------------------------------------------
// Define the bit positions of the N-bit dft__bc_cntl_ip[X:Y]->dft__mb_aux_in[A:B] fields of dft__bc_cntl bus
// 10 aug 2019: Mimic the structure used for the 3rd party bus.
//              The dft_hub structures will include a full bit-wise TDR override of these bits, per Zoran's request.
//
// Allocation of bits (no overlap allowed between shires, since we may be runing MBIST concurrently in each shire)
//
//
`define DFT_MB_AUX_IN_W        20
`define DFT_BC_CNTL_MB_RANGE   31:12
`define DFT_MB_AUX_IN_RANGE    19:0

// Flags to control which version of the code is used for components with more that one implementation
//       available during deveoplment.  A down-select to a single implementation must be made
//       prior to DFT RTL freeze

// if USE_GATE_VERSION_EW_SCR is defined, the dft_hub_scr_we.vp module will use ed.toy's handcoded gate
//    view for the east/west scan chain router block.  If USE_GATE_VERSION_EW_SCR is not defined,
//    use Albert's RTL version (as of 14 aug 2019, too many lint issues in RTL version for it to be deployed).
`define  USE_GATE_VERSION_EW_SCR   1


`define ET_BIST_INSERTION       0   // set this to 1 during and after bist insertion
// `define ET_USE_BIST_RTL

`ifndef ET_USE_BIST_RTL
   `define VIRAGE_IGNORE_RESET
  `endif

`define DFT_OCC_SC_IDX 43

`endif


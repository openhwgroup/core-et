// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//
// Synopsys SRAM config
//

// Struct for config pins of single ported Synopsys SRAMs
typedef struct packed {
    logic [2:0]                     bc;
    logic                           test_rnm;
    logic                           test1;
    logic [2:0]                     wpulse;
    logic [2:0]                     wa;
    logic [1:0]                     ra;
    logic [3:0]                     rm;
    logic                           rme;
} ioshire_sram1p_cfg_t;

// Struct for config pins of dual ported Synopsys SRAMs
typedef struct packed {
    logic                           test_rnma;
    logic                           test1a;
    logic                           test1b;
    logic [2:0]                     wpulse;
    logic [2:0]                     wa;
    logic [1:0]                     ra;
    logic [3:0]                     rma;
    logic [3:0]                     rmb;
    logic                           rmea;
    logic                           rmeb;
} ioshire_sram2p_cfg_t;

// Struct for config pins of Synopsys ROM
typedef struct packed {
    logic                           test1;
    logic [3:0]                     rm;
    logic                           rme;
} ioshire_rom_cfg_t;

// Struct-of-structs for SPIO and PU TDR 
// Includes SRAM/ROM configs
// based on compiler type.  All SRAMs/ROMs from the same
// compiler will use the same configs.
// Also includes pins for ramdump and extra DFT scandump/ramdump
// control pins
typedef struct packed {
    logic [`IOSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH-1:0]   dft__scan_dump_cntl;
    logic                                           dft__ram_mei;
    logic                                           dft__ram_wei;
    ioshire_rom_cfg_t                               asdvd;
    ioshire_sram1p_cfg_t                            sadcl;
    ioshire_sram1p_cfg_t                            sadul;
    ioshire_sram2p_cfg_t                            sadrl;
} spio_tdr_t;

typedef struct packed {
    logic [`IOSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH-1:0]   dft__scan_dump_cntl;
    logic                                           dft__ram_mei;
    logic                                           dft__ram_wei;
    ioshire_sram1p_cfg_t                            sadcl;
} pu_tdr_t;

// JTAG interface for TDR
typedef sc_jtag_ram_cfg_t           ios_jtag_tdr_cfg_t;

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _SC_DEFINES_
`define _SC_DEFINES_

// **************************************************************************
// MACROS
// **************************************************************************

// --------------------------------------------------------------------------
// Shire cache revision ids
// --------------------------------------------------------------------------
// NOTE: shire cache revision id (32-bit) = b3.b2.b1.b0
`define SC_B3_REVISION_ID            0
`define SC_B2_REVISION_ID            0
`define SC_B1_REVISION_ID            0
`define SC_B0_REVISION_ID            1

// --------------------------------------------------------------------------
// soc related defines
// --------------------------------------------------------------------------
`define SC_PORTS                     (`NUM_NEIGH+`RBOX_PER_SHIRE) // neighborhoods + rbox
`define SC_PORT_ID_SIZE              $clog2(`SC_PORTS)

`define SC_L3_MASTER_PORTS       4   // NOTE: for minion shire master/slave are the same, for maxion shire, master/slave are different
`define SC_L3_SLAVE_PORTS        4   // NOTE: for minion shire master/slave are the same, for maxion shire, master/slave are different
`define SC_SYS_PORTS             1

// these define are used to save away l3_slave_port id to be used on l3_slave responses.
`define SC_MAX_L3_SLAVE_PORTS           4
`define SC_MAX_L3_SLAVE_PORT_ID_SIZE    $clog2(`SC_MAX_L3_SLAVE_PORTS)


// NOTE: Total number of scp shire ids = 256 because
//       it has to represent shires 0-255
//       
//  noc_id           shire
// ---------    ------------------
//   0 -  31 =>  minion shire
//  32       =>  master minion shire
// 232 - 239 =>  mem shires
// 240 - 247 =>  reserved
// 248 - 251 =>  maxion shires
// 252 - 254 =>  io shire
// 255       =>  self 
`define SC_SCP_SHIRES                `NUM_SHIRE_IDS
`define SC_SCP_SHIRE_ID_SIZE         $clog2(`SC_SCP_SHIRES)             
                                    
// NOTE: L3_SHIRES must be >= 2
`define SC_L3_SHIRES                 32
`define SC_L3_SHIRE_ID_SIZE          $clog2(`SC_L3_SHIRES)

`define SC_MEM_SHIRES                8
`define SC_MEM_SHIRE_ID_SIZE         $clog2(`SC_MEM_SHIRES)

`define SC_NEIGH_COOP_MASK_SIZE      `NUM_NEIGH
`define SC_NEIGH_COOP_MASK_START     16 
`define SC_NEIGH_COOP_MASK_RANGE     `SC_NEIGH_COOP_MASK_START +: `SC_NEIGH_COOP_MASK_SIZE

`define SC_START_LEVEL_RANGE         4:3
`define SC_DEST_LEVEL_RANGE          6:5
`define SC_UNLOCKINV_RANGE           3

// --------------------------------------------------------------------------
// reqq related defines
// --------------------------------------------------------------------------

`define SC_REQQ_DEPTH                64 // Number of reqq entries
`define SC_REQQ_ID_SIZE              6  // Always 6 even when there are fewer than 64 reqq entries

`define SC_REQQ_TAG_ID_SIZE          `MAX(`ET_LINK_ID_SIZE,`SC_MESH_MASTER_AXI_ID_SIZE)
`define SC_REQQ_SOURCE_SIZE          `MAX(`ET_LINK_SOURCE_SIZE,`MESH_SOURCE_BRIDGE_ID_SIZE)

// Number of QOS bits actually in use.
`define SC_REQQ_QOS_SIZE             1

// Holds paired_id, cache_level_start, cache_level_dest, read_coop_dest + scp_is_remote
// Depending upon the request type.
`define SC_REQQ_INFO_SIZE            `MAX(`SC_REQQ_ID_SIZE, (`SC_NEIGH_COOP_MASK_SIZE+1))

`define SC_CGATE_DIS_SIZE            8
`define SC_L3_YIELD_SIZE             5

// --------------------------------------------------------------------------
// pipeline and cache related defines
// --------------------------------------------------------------------------
`define SC_CACHE_SIZE_IN_MB                  4
`define SC_CACHE_SIZE                        (`SC_CACHE_SIZE_IN_MB*1024*1024)
                                             
`define SC_LINE_DATA_SIZE                    512
`define SC_BYTE_DATA_SIZE                    8
`define SC_HW_DATA_SIZE                      16
`define SC_SW_DATA_SIZE                      32
`define SC_DW_DATA_SIZE                      64
`define SC_QW_DATA_SIZE                      128
`define SC_LINE_BYTE_SIZE                    (`SC_LINE_DATA_SIZE/`SC_BYTE_DATA_SIZE)
`define SC_LINE_SW_SIZE                      (`SC_LINE_DATA_SIZE/`SC_SW_DATA_SIZE)
`define SC_LINE_DW_SIZE                      (`SC_LINE_DATA_SIZE/`SC_DW_DATA_SIZE)
`define SC_LINE_QW_SIZE                      (`SC_LINE_DATA_SIZE/`SC_QW_DATA_SIZE)

`define SC_LINE_OFFSET_ID_SIZE               $clog2(`SC_LINE_BYTE_SIZE)
`define SC_LINE_SW_ID_SIZE                   $clog2(`SC_LINE_SW_SIZE)
`define SC_LINE_DW_ID_SIZE                   $clog2(`SC_LINE_DW_SIZE)
`define SC_LINE_QW_ID_SIZE                   $clog2(`SC_LINE_QW_SIZE)

`define SC_BYTE_BEN                          `ZX(`SC_LINE_BYTE_SIZE, 1'b1)
`define SC_HW_BEN                            `ZX(`SC_LINE_BYTE_SIZE, {(`SC_HW_DATA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}})
`define SC_SW_BEN                            `ZX(`SC_LINE_BYTE_SIZE, {(`SC_SW_DATA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}})
`define SC_DW_BEN                            `ZX(`SC_LINE_BYTE_SIZE, {(`SC_DW_DATA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}})
`define SC_QW_BEN                            `ZX(`SC_LINE_BYTE_SIZE, {(`SC_QW_DATA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}})
`define SC_HL_BEN                            `ZX(`SC_LINE_BYTE_SIZE, {((`SC_QW_DATA_SIZE*2)/`SC_BYTE_DATA_SIZE){1'b1}})
`define SC_LINE_BEN                          {(`SC_LINE_DATA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}}
`define SC_SCPFILL_BEN                       `ZX(`SC_LINE_BYTE_SIZE, {(`PA_SIZE/`SC_BYTE_DATA_SIZE){1'b1}})

`define SC_LINE_SEL_ADDR_RANGE               (`PA_SIZE - 1) : `SC_LINE_OFFSET_ID_SIZE // Bits of the addr that are above line size

`define SC_BANKS                             4  // Number of banks in the shire cache
`define SC_BANK_ID_SIZE                      $clog2(`SC_BANKS)
`define SC_BANK_LSB                          `SC_LINE_OFFSET_ID_SIZE
`define SC_BANK_MSB                          (`SC_BANK_LSB + `SC_BANK_ID_SIZE - 1)
`define SC_BANK_SEL_ADDR_RANGE               `SC_BANK_LSB +: `SC_BANK_ID_SIZE // Bits of the addr that define the bank where it goes
                                             
`define SC_SUB_BANKS                         4  // Number of sub_banks per bank
`define SC_SUB_BANK_ID_SIZE                  $clog2(`SC_SUB_BANKS)
`define SC_SUB_BANK_LSB                      (`SC_LINE_OFFSET_ID_SIZE + `SC_BANK_ID_SIZE)
`define SC_SUB_BANK_MSB                      (`SC_SUB_BANK_LSB + `SC_SUB_BANK_ID_SIZE - 1)
`define SC_SUB_BANK_SEL_ADDR_RANGE           `SC_SUB_BANK_LSB +: `SC_SUB_BANK_ID_SIZE // Bits of the addr that define the sub_bank where it goes
                                             
`define SC_PIPE_SEL_ADDR_RANGE               (`PA_SIZE - 1) : `SC_SUB_BANK_LSB
`define SC_REQQ_SEL_ADDR_RANGE               (`PA_SIZE - 1) : `SC_BANK_LSB
                                             
`define SC_LINES_PER_SUB_BANK                (`SC_CACHE_SIZE/`SC_LINE_BYTE_SIZE/`SC_BANKS/`SC_SUB_BANKS)
                                             
                                             
`define SC_WAYS                              4  // Number of cache ways in each sub_bank
`define SC_WAY_ID_SIZE                       $clog2(`SC_WAYS)
                                             
`define SC_SETS_PER_SUB_BANK                 (`SC_LINES_PER_SUB_BANK/`SC_WAYS)
`define SC_SET_ID_SIZE                       $clog2(`SC_SETS_PER_SUB_BANK)
`define SC_SET_ALLOC_SIZE                    (`SC_SET_ID_SIZE + 1)
                                             
// tag ram                                   
`define SC_TAG_MAX_SIZE                      23  // allows for down to 512k L2 allocation within shire cache
                                                 // NOTE: Increasing MAX_TAG_SIZE allows for reduced L2 buffer allocation
                                                 //    23 => L2_BUF = 512k (current minimum allocation per gen1 spec)
                                                 //    24 => L2_BUF = 256k
                                                 //    25 => L2_BUF = 128k
                                                 //    26 => L2_BUF =  64k - do not exceed 26 as that would require a larger ecc size
                                             
// L2 Specific set/tag information           
`define SC_SET_LSB                           (`SC_LINE_OFFSET_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)
`define SC_SET_MSB                           (`SC_SET_LSB + `SC_SET_ID_SIZE - 1)
`define SC_SET_SEL_ADDR_RANGE                `SC_SET_LSB +: `SC_SET_ID_SIZE
                                             
`define SC_TAG_SET_SEL_ADDR_SIZE             (`PA_SIZE - `SC_SET_LSB)
`define SC_TAG_SET_SEL_ADDR_RANGE            `SC_SET_LSB +: `SC_TAG_SET_SEL_ADDR_SIZE
                                             
                                             
`define SC_TAG_OF_TAG_SET_LSB                (`SC_TAG_SET_SEL_ADDR_SIZE - `SC_TAG_MAX_SIZE) // removing unused bits of the "tag_set" when determining the tag_max bits for the tag_ram
`define SC_TAG_LSB                           (`SC_SET_LSB + `SC_TAG_OF_TAG_SET_LSB)
`define SC_TAG_ID_SIZE                       `SC_TAG_MAX_SIZE
`define SC_TAG_OF_TAG_SET_SEL_ADDR_RANGE     `SC_TAG_OF_TAG_SET_LSB +: `SC_TAG_ID_SIZE
                                             
// L3 Specific set/tag information
`define SC_L3_SHIRE_LSB                      `SC_LINE_OFFSET_ID_SIZE
`define SC_L3_SHIRE_MSB                      (`SC_L3_SHIRE_LSB + `SC_L3_SHIRE_ID_SIZE - 1)
`define SC_L3_SHIRE_SEL_ADDR_RANGE           `SC_L3_SHIRE_LSB +: `SC_L3_SHIRE_ID_SIZE
                                             
`define SC_L3_BANK_LSB                       (`SC_LINE_OFFSET_ID_SIZE + `SC_L3_SHIRE_ID_SIZE)
`define SC_L3_BANK_MSB                       (`SC_L3_BANK_LSB + `SC_BANK_ID_SIZE - 1)
`define SC_L3_BANK_SEL_ADDR_RANGE            `SC_L3_BANK_LSB +: `SC_BANK_ID_SIZE // Bits of the addr that define the bank where it goes
                                             
`define SC_L3_SUB_BANK_LSB                   (`SC_LINE_OFFSET_ID_SIZE + `SC_L3_SHIRE_ID_SIZE + `SC_BANK_ID_SIZE)
`define SC_L3_SUB_BANK_MSB                   (`SC_L3_SUB_BANK_LSB + `SC_SUB_BANK_ID_SIZE - 1)
`define SC_L3_SUB_BANK_SEL_ADDR_RANGE        `SC_L3_SUB_BANK_LSB +: `SC_SUB_BANK_ID_SIZE // Bits of the addr that define the sub_bank where it goes
                                             
`define SC_L3_PIPE_SEL_ADDR_RANGE            (`PA_SIZE - 1) : `SC_L3_SUB_BANK_LSB
`define SC_L3_REQQ_SEL_ADDR_RANGE            `SC_L3_PIPE_SEL_ADDR_RANGE
                                             
`define SC_L3_SET_LSB                        (`SC_LINE_OFFSET_ID_SIZE + `SC_L3_SHIRE_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)
`define SC_L3_SET_MSB                        (`SC_L3_SET_LSB + `SC_SET_ID_SIZE - 1)
`define SC_L3_SET_SEL_ADDR_RANGE             `SC_L3_SET_LSB +: `SC_SET_ID_SIZE
                                             
`define SC_L3_TAG_SET_SEL_ADDR_SIZE          (`PA_SIZE - `SC_L3_SET_LSB)
`define SC_L3_TAG_SET_SEL_ADDR_RANGE         `SC_L3_SET_LSB +: `SC_L3_TAG_SET_SEL_ADDR_SIZE

// removing unused bits of the "tag_set" when determining the tag_max bits for the tag_ram
// NOTE: SC_TAG_MAX can be larger than the L3_TAG_SET_SEL_ADDR_SIZE, in that case just use the TAG_SET_SEL_ADDR_RANGE
`define SC_L3_TAG_OF_TAG_SET_LSB             (( `SC_TAG_MAX_SIZE > `SC_L3_TAG_SET_SEL_ADDR_SIZE) ? 0              : (`SC_L3_TAG_SET_SEL_ADDR_SIZE-`SC_TAG_MAX_SIZE))
`define SC_L3_TAG_LSB                        (`SC_L3_SET_LSB + `SC_L3_TAG_OF_TAG_SET_LSB)
`define SC_L3_TAG_ID_SIZE                    (( `SC_TAG_MAX_SIZE > `SC_L3_TAG_SET_SEL_ADDR_SIZE) ? `SC_L3_TAG_SET_SEL_ADDR_SIZE : `SC_TAG_MAX_SIZE)
`define SC_L3_TAG_OF_TAG_SET_SEL_ADDR_RANGE  `SC_L3_TAG_OF_TAG_SET_LSB +: `SC_L3_TAG_ID_SIZE


// L3 swizzle bit sel size (max size for each field) 
`define SC_BANKS_MAX                         8  // Maximum number of banks in the shire cache (NOTE: all of these _MAX defines are used for L3 shire swizzle)
`define SC_BANK_ID_SIZE_MAX                  $clog2(`SC_BANKS_MAX)
`define SC_SUB_BANKS_MAX                     8  // Maximum number of sub_banks in the shire cache (NOTE: all of these _MAX defines are used for L3 shire swizzle)
`define SC_SUB_BANK_ID_SIZE_MAX              $clog2(`SC_BANKS_MAX)
`define SC_L3_SHIRES_MAX                     64 // Maximum number of L3 shires in the shire cache (NOTE: all of these _MAX defines are used for L3 shire swizzle)
`define SC_L3_SHIRE_ID_SIZE_MAX              $clog2(`SC_L3_SHIRES_MAX)
`define SC_L3_SWIZZLE_BIT_SEL_SIZE           $clog2(`SC_BANK_ID_SIZE_MAX + `SC_SUB_BANK_ID_SIZE_MAX + `SC_L3_SHIRE_ID_SIZE_MAX) // determines sel_width for ESR
`define SC_L3_SWIZZLE_SEL_SIZE               (1 << `SC_L3_SWIZZLE_BIT_SEL_SIZE)

// L3 swizzle sel_addr_range
`define SC_L3_SWIZZLE_ID_SIZE                (`SC_L3_SHIRE_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)
`define SC_L3_SWIZZLE_ID_LSB                 `SC_LINE_OFFSET_ID_SIZE
`define SC_L3_SWIZZLE_SEL_ADDR_RANGE         `SC_L3_SWIZZLE_ID_LSB +: `SC_L3_SWIZZLE_ID_SIZE
                                             
                                             
// SCP Specific set/tag information
`define SC_SCP_WAY_LSB                       (`SC_LINE_OFFSET_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)
`define SC_SCP_WAY_MSB                       (`SC_SCP_WAY_LSB + `SC_WAY_ID_SIZE - 1)
`define SC_SCP_WAY_SEL_ADDR_RANGE            `SC_SCP_WAY_LSB +: `SC_WAY_ID_SIZE
                                             
`define SC_SCP_SET_LSB                       (`SC_SCP_WAY_LSB + `SC_WAY_ID_SIZE)
`define SC_SCP_SET_MSB                       (`SC_SCP_SET_LSB + `SC_SET_ID_SIZE - 1)
`define SC_SCP_SET_SEL_ADDR_RANGE            `SC_SCP_SET_LSB +: `SC_SET_ID_SIZE

// Index based cache ops
`define SC_IDX_WAY_LSB                       `SC_SCP_WAY_LSB
`define SC_IDX_WAY_MSB                       `SC_SCP_WAY_MSB
`define SC_IDX_WAY_SEL_ADDR_RANGE            `SC_SCP_WAY_SEL_ADDR_RANGE
                                             
`define SC_IDX_SET_LSB                       `SC_SCP_SET_LSB
`define SC_IDX_SET_MSB                       `SC_SCP_SET_MSB
`define SC_IDX_SET_SEL_ADDR_RANGE            `SC_SCP_SET_SEL_ADDR_RANGE

// tag state ram
`define SC_TAG_STATE_ECC_SIZE                7   // supports tag state size up to 57-bits
`define SC_TAG_STATE_LRU_SIZE                5
`define SC_TAG_STATE_FLAGS_SIZE              7   // valid, locked, zero, qwen[3:0]
`define SC_TAG_STATE_DATA_SIZE               (`SC_WAYS*`SC_TAG_STATE_FLAGS_SIZE + `SC_TAG_STATE_LRU_SIZE)
                                             
`define SC_TAG_STATE_RAM_ADDR_SIZE           `SC_SET_ID_SIZE
`define SC_TAG_STATE_RAM_DATA_SIZE           (`SC_TAG_STATE_DATA_SIZE + `SC_TAG_STATE_ECC_SIZE)
`define SC_TAG_STATE_RAM_DEPTH               `SC_SETS_PER_SUB_BANK
                                             
// tag ram                                   
`define SC_TAG_ECC_SIZE                      6   // supports tag size up to 26-bits
`define SC_TAG_DATA_SIZE                     `SC_TAG_MAX_SIZE
`define SC_TAGS_DATA_SIZE                    (`SC_WAYS*`SC_TAG_DATA_SIZE)
                                             
`define SC_TAG_RAM_ADDR_SIZE                 `SC_SET_ID_SIZE
`define SC_TAG_RAM_DATA_SIZE                 (`SC_WAYS*(`SC_TAG_DATA_SIZE + `SC_TAG_ECC_SIZE))
`define SC_TAG_RAM_DEPTH                     `SC_SETS_PER_SUB_BANK
                                             
// data ram                                  
`define SC_DW_ECC_SIZE                       8
`define SC_LINE_ECC_SIZE                     (`SC_LINE_DW_SIZE * `SC_DW_ECC_SIZE)
                                             
`define SC_DATA_RAM_ADDR_SIZE                (`SC_WAY_ID_SIZE + `SC_SET_ID_SIZE)
`define SC_DATA_RAM_DATA_SIZE                (`SC_LINE_DATA_SIZE + `SC_LINE_ECC_SIZE)
`define SC_DATA_RAM_DEPTH                    `SC_LINES_PER_SUB_BANK
                                             
`define SC_RAM_DELAY_SIZE                    3 // has to support representing ram_delay = 2,3, and 4 ('b100 -> 3 bits)
                                             
// Rams                                      
`define SC_RAM_DELAY_DEFAULT                 2  // default esr_sc_ram_delay
`ifdef EVL_SIMULATION
   `define ET_INIT_RAMS
`endif

`define ET_SCP_L2_REQQ_OPCODES SC_Scp_Read, SC_Scp_Write, SC_Scp_Fill
`define ET_SCP_L2_REQQ_ORIG_OPCODES SC_Reqq_ScpRead, SC_Reqq_ScpWrite


// Pipe rbuf (read Buffer) defines
// NOTE: minimum RBUF_SIZE = 6 (ram_delay=2)
//                         = 8 (ram_delay=3) // since ram_delay_3 timing is adjusted to match ram_delay_4 timing of tag_rsp and prevent requiring a RBUF_SIZE > 8, otherwise would not have to push out the ram_delay_3 tag_rsp
//                         = 8 (ram_delay=4)
`define SC_RBUF_SIZE                         8
`define SC_RBUF_ID_SIZE                      $clog2(`SC_RBUF_SIZE)
`define SC_RBUF_FIFO_SIZE                    4

// depth from _tc to _dc (install) for max ram_delay pipe
`define SC_RBUF_PENDING_INSTALL_SIZE         8


// Pipe cbuf (coalesce Buffer) defines
`define SC_CBUF_SIZE                         32
`define SC_CBUF_ID_SIZE                      $clog2(`SC_CBUF_SIZE)


// Pipe Atomic
`define SC_ATOMIC_SIMD_SIZE                 8      // Number of elements in SIMD operation
`define SC_ATOMIC_DATA_SIZE                 (`SC_ATOMIC_SIMD_SIZE*`SC_SW_DATA_SIZE)
`define SC_ATOMIC_DATA_ID_SIZE              $clog2(`SC_LINE_DATA_SIZE/`SC_ATOMIC_DATA_SIZE)
`define SC_ATOMIC_DATA_SEL_ADDR_RANGE       (`SC_LINE_OFFSET_ID_SIZE-1) : (`SC_LINE_OFFSET_ID_SIZE-`SC_ATOMIC_DATA_ID_SIZE)
`define SC_ATOMIC_DW_SEL_ADDR_RANGE         (`SC_LINE_OFFSET_ID_SIZE-1) : (`SC_LINE_OFFSET_ID_SIZE-`SC_LINE_DW_ID_SIZE)
`define SC_ATOMIC_SW_SEL_ADDR_RANGE         (`SC_LINE_OFFSET_ID_SIZE-1) : (`SC_LINE_OFFSET_ID_SIZE-`SC_LINE_SW_ID_SIZE)

`define SC_ATOMIC_SC64_ADDR_RANGE           (`SC_LINE_OFFSET_ID_SIZE-`SC_ATOMIC_DATA_ID_SIZE-1) : (`SC_LINE_OFFSET_ID_SIZE-`SC_LINE_DW_ID_SIZE)
`define SC_ATOMIC_SC32_ADDR_RANGE           (`SC_LINE_OFFSET_ID_SIZE-`SC_ATOMIC_DATA_ID_SIZE-1) : (`SC_LINE_OFFSET_ID_SIZE-`SC_LINE_SW_ID_SIZE)

// data field sizes in qw2 (shared between all atomics)
`define SC_ATOMIC_CONF_SIZE                 7  // NOTE: This must match the size of the sc_pipe_amo_conf_req_t struct!
`define SC_ATOMIC_CONF_LSB                  `SC_ATOMIC_DATA_SIZE
`define SC_ATOMIC_CONF_MSB                  (`SC_ATOMIC_CONF_LSB + `SC_ATOMIC_CONF_SIZE - 1)
`define SC_ATOMIC_CONF_SEL_RANGE            `SC_ATOMIC_CONF_LSB +: `SC_ATOMIC_CONF_SIZE
`define SC_ATOMIC_ADDR_OFFSET_SIZE        `SC_LINE_OFFSET_ID_SIZE
`define SC_ATOMIC_ADDR_OFFSET_LSB           (`SC_ATOMIC_DATA_SIZE + `SC_ATOMIC_CONF_SIZE)
`define SC_ATOMIC_ADDR_OFFSET_MSB           (`SC_ATOMIC_ADDR_OFFSET_LSB + `SC_ATOMIC_ADDR_OFFSET_SIZE - 1)
`define SC_ATOMIC_ADDR_OFFSET_SEL_RANGE     `SC_ATOMIC_ADDR_OFFSET_LSB +: `SC_ATOMIC_ADDR_OFFSET_SIZE



// Pipe idx_cop state machine

// idx_cop dbg_read fields
`define SC_IDX_COP_DWORD_LSB                 0
`define SC_IDX_COP_DWORD_MSB                 (`SC_IDX_COP_DWORD_LSB + `SC_LINE_DW_ID_SIZE - 1)
`define SC_IDX_COP_DWORD_SEL_ADDR_RANGE      `SC_IDX_COP_DWORD_LSB +: `SC_LINE_DW_ID_SIZE

`define SC_IDX_COP_WAY_LSB                   8
`define SC_IDX_COP_WAY_MSB                   (`SC_IDX_COP_WAY_LSB + `SC_WAY_ID_SIZE - 1)
`define SC_IDX_COP_WAY_SEL_ADDR_RANGE        `SC_IDX_COP_WAY_LSB +: `SC_WAY_ID_SIZE

`define SC_IDX_COP_SET_LSB                   16
`define SC_IDX_COP_SET_MSB                   (`SC_IDX_COP_SET_LSB + `SC_SET_ID_SIZE - 1)
`define SC_IDX_COP_SET_SEL_ADDR_RANGE        `SC_IDX_COP_SET_LSB +: `SC_SET_ID_SIZE

`define SC_IDX_COP_SUB_BANK_LSB              32
`define SC_IDX_COP_SUB_BANK_MSB              (`SC_IDX_COP_SUB_BANK_LSB + `SC_SUB_BANK_ID_SIZE - 1)
`define SC_IDX_COP_SUB_BANK_SEL_ADDR_RANGE   `SC_IDX_COP_SUB_BANK_LSB +: `SC_SUB_BANK_ID_SIZE

// Remote Scratchpad defines
`define SC_SCP_SHIRE_LSB                     23
`define SC_SCP_SHIRE_MSB                     (`SC_SCP_SHIRE_LSB + `SC_SCP_SHIRE_ID_SIZE - 1)
`define SC_SCP_SHIRE_SEL_ADDR_RANGE          `SC_SCP_SHIRE_LSB +: `SC_SCP_SHIRE_ID_SIZE

// for out of range checks need to check max set bits of address range not just the number of SET_ID bits for the build configuration
`define SC_SCP_SET_MAX_LSB                   `SC_SCP_SET_LSB
`define SC_SCP_SET_MAX_MSB                   (`SC_SCP_SHIRE_LSB - 1)
`define SC_SCP_SET_ID_MAX_SIZE               (`SC_SCP_SET_MAX_MSB - `SC_SCP_SET_MAX_LSB + 1)
`define SC_SCP_SET_MAX_SEL_ADDR_RANGE        `SC_SCP_SET_MAX_LSB +: `SC_SCP_SET_ID_MAX_SIZE

`define SC_SCP_REGION_ID                     'h001
`define SC_SCP_REGION_ID_SIZE                9   // [39:31]
`define SC_SCP_REGION_LSB                    31
`define SC_SCP_REGION_MSB                    (`SC_SCP_REGION_LSB + `SC_SCP_REGION_ID_SIZE - 1)
`define SC_SCP_REGION_SEL_ADDR_RANGE         `SC_SCP_REGION_LSB +: `SC_SCP_REGION_ID_SIZE


// BIST related defines
// zorans 07/13/2019 moved to dft_defines.vh for entire chip
//`define ET_BIST_INSERTION                    0   // set this to 1 during and after bist insertion
`define SC_MBIST_LOGICAL_MEMS                4
`define SC_MBIST_SEL_SIZE                    $clog2(`SC_MBIST_LOGICAL_MEMS + 1) // +1 for reserved code

`define SC_MBS_ADDR_SIZE                     (`SC_TAG_STATE_RAM_ADDR_SIZE + `SC_SUB_BANK_ID_SIZE)
`define SC_MBT_ADDR_SIZE                     (`SC_TAG_RAM_ADDR_SIZE       + `SC_SUB_BANK_ID_SIZE)
`define SC_MBD_ADDR_SIZE                     (`SC_DATA_RAM_ADDR_SIZE      + `SC_SUB_BANK_ID_SIZE + `SC_LINE_QW_ID_SIZE)
`define SC_MBQ_ADDR_SIZE                     (`SC_REQQ_ID_SIZE            + `SC_LINE_QW_ID_SIZE)
`define SC_MBB_ADDR_SIZE                     (`SC_REQQ_ID_SIZE            + 0)
`define SC_MBI_ADDR_SIZE                     (`ICACHE_MEM_ADWIDTH         + `NUM_NEIGH_L + $clog2(`ICACHE_NR_MEMS))


`define SC_MBS_DATA_SIZE                     `SC_TAG_STATE_RAM_DATA_SIZE
`define SC_MBT_DATA_SIZE                     `SC_TAG_RAM_DATA_SIZE
`define SC_MBD_DATA_SIZE                     (`SC_DATA_RAM_DATA_SIZE/`SC_LINE_QW_SIZE)
`define SC_MBQ_DATA_SIZE                     (`SC_DATA_RAM_DATA_SIZE/`SC_LINE_QW_SIZE)
`define SC_MBB_DATA_SIZE                     (`SC_LINE_BYTE_SIZE + `SC_DW_ECC_SIZE)
`define SC_MBI_DATA_SIZE                     `ICACHE_MEM_WIDTH

`define SC_MBIST_ADDR_SIZE                   `SC_MBD_ADDR_SIZE    // max addr width across all logical memories
`define SC_MBIST_DATA_SIZE                   `SC_MBD_DATA_SIZE    // max data width across all logical memories

`define SC_MBS_LOGICAL_RAM_DEPTH              (`SC_SETS_PER_SUB_BANK * `SC_SUB_BANKS)
`define SC_MBT_LOGICAL_RAM_DEPTH              (`SC_SETS_PER_SUB_BANK * `SC_SUB_BANKS)
`define SC_MBD_LOGICAL_RAM_DEPTH              (`SC_SETS_PER_SUB_BANK * `SC_SUB_BANKS * `SC_WAYS * `SC_LINE_QW_SIZE)
`define SC_MBQ_LOGICAL_RAM_DEPTH              (`SC_REQQ_DEPTH * `SC_LINE_QW_SIZE)
`define SC_MBB_LOGICAL_RAM_DEPTH              (`SC_REQQ_DEPTH * 1)
`define SC_MBI_LOGICAL_RAM_DEPTH              (`ICACHE_MEM_WORDS * `NUM_NEIGH * `ICACHE_NR_MEMS)
 
`define SC_MBS_RAM_DELAY                     `SC_RAM_DELAY_DEFAULT
`define SC_MBT_RAM_DELAY                     `SC_RAM_DELAY_DEFAULT
`define SC_MBD_RAM_DELAY                     `SC_RAM_DELAY_DEFAULT
`define SC_MBQ_RAM_DELAY                     1
`define SC_MBB_RAM_DELAY                     1
`define SC_MBI_RAM_DELAY                     2

`define SC_MBS_READ_DELAY                    4
`define SC_MBT_READ_DELAY                    4
`define SC_MBD_READ_DELAY                    5
`define SC_MBQ_READ_DELAY                    3
`define SC_MBB_READ_DELAY                    3
`define SC_MBI_READ_DELAY                    5

// End of pipeline and cache defines
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// ESR, Interrupt, Err log, trace, interrupt and esr status defines
// --------------------------------------------------------------------------

`define SC_ESR_SET_BASE_SIZE         12
`define SC_ESR_SET_SIZE_SIZE         13
`define SC_SBE_COUNT_SIZE            8  // NOTE: must be <= 8
`define SC_DBE_COUNT_SIZE            3  // NOTE: must be <= 8
`define SC_ESR_STATUS_SIZE           `SC_DW_DATA_SIZE

`define SC_ERR_INT_SIZE              8

`define SC_ERR_LOG_ECC_SBE           4'h0
`define SC_ERR_LOG_ECC_DBE           4'h1
`define SC_ERR_LOG_ECC_SAT           4'h2
`define SC_ERR_LOG_DECODE            4'h3
`define SC_ERR_LOG_PERFMON_SAT       4'h4

// Trace
`define SC_TRACE_EN_SIZE             12
`define SC_TRACE_EN_ALLOC_L2         0
`define SC_TRACE_EN_ALLOC_L3         1
`define SC_TRACE_EN_TC               2
`define SC_TRACE_EN_TC_REQQ_ID       3
`define SC_TRACE_EN_TC_VICTIM        4
`define SC_TRACE_EN_RBUF             5
`define SC_TRACE_EN_MESH             6
`define SC_TRACE_EN_REQQ             7
`define SC_TRACE_EN_FILL             11

// --------------------------------------------------------------------------
// MsgSendData information
// --------------------------------------------------------------------------
// The memory map has MsgSendData neigh destination port id in address bit 19:16
`define SC_MSG_NEIGH_ID_SIZE         `AD_ESR_NEIGH_ID_SIZE
`define SC_MSG_NEIGH_ID_START        `AD_ESR_NEIGH_ID_START
`define SC_MSG_NEIGH_ID_ADDR_RANGE   `SC_MSG_NEIGH_ID_START +: `SC_MSG_NEIGH_ID_SIZE

// The memory map has MsgSendData ID in address bit 10:3
`define SC_MSG_ID_SIZE               `AD_ESR_MESSAGE_ID_SIZE
`define SC_MSG_ID_START              `AD_ESR_MESSAGE_ID_START
`define SC_MSG_ID_ADDR_RANGE         `SC_MSG_ID_START +: `SC_MSG_ID_SIZE

// The memory map has MsgSendData Source in address bit 15:12
`define SC_MSG_SOURCE_SIZE           `AD_ESR_NEIGH_HART_ID_SIZE
`define SC_MSG_SOURCE_START          `AD_ESR_NEIGH_HART_ID_START
`define SC_MSG_SOURCE_ADDR_RANGE     `SC_MSG_SOURCE_START +: `SC_MSG_SOURCE_SIZE

// esr_shire_config_t is auto generated and need to make sure the define here is consistent with it.
`define SC_SHIRE_CONFIG_NUM_NEIGH   4
`define SC_SHIRE_CONFIG_NEIGH_SIZE  2
`define SC_SHIRE_CONFIG_NEIGH_RANGE 0 +: `SC_SHIRE_CONFIG_NEIGH_SIZE

// Defines for L2/L3 <> Mesh connections
// --------------------------------------------------------------------------
`define SC_MESH_MASTER_SOURCE_SIZE   $clog2(`SC_BANKS)    // Banks only, UC is dedicated port on mesh

// Defines for L3 Atomics or Scp Atomics additional defines from shared atomic defines
// --------------------------------------------------------------------------
`define SC_L3_ATOMIC_CONFIG_RANGE          `SC_ATOMIC_CONF_SEL_RANGE
`define SC_L3_ATOMIC_ADDR_LOWER_RANGE      `SC_ATOMIC_ADDR_OFFSET_SEL_RANGE
// Atomic Shire ID is fixed at 8 bits regardless of number of shires configured
`define SC_L3_ATOMIC_SHIRE_ID_SIZE         8
`define SC_L3_ATOMIC_SHIRE_ID_START        (`SC_ATOMIC_ADDR_OFFSET_LSB + `SC_ATOMIC_ADDR_OFFSET_SIZE)
`define SC_L3_ATOMIC_SHIRE_ID_RANGE        `SC_L3_ATOMIC_SHIRE_ID_START +: `SC_L3_ATOMIC_SHIRE_ID_SIZE
`define SC_L3_ATOMIC_TRANS_ID_SIZE         5
`define SC_L3_ATOMIC_TRANS_ID_START        (`SC_L3_ATOMIC_SHIRE_ID_START + `SC_L3_ATOMIC_SHIRE_ID_SIZE)
`define SC_L3_ATOMIC_TRANS_ID_RANGE        `SC_L3_ATOMIC_TRANS_ID_START +: `SC_L3_ATOMIC_TRANS_ID_SIZE
`define SC_L2_ATOMIC_DATA_MASK             `ZX(`SC_LINE_DATA_SIZE, {(`SC_QW_DATA_SIZE*2){1'b1}})

// NoC spec section 3.5.2 specifies the address for the atomic response:
`define SC_L3_ATOMIC_RSP_ADDR_OFFSET       `PA_SIZE'h01803E0000
`define SC_L3_ATOMIC_RSP_SHIRE_ID_SHIFT    22

// Defines for Performance Monitor
// --------------------------------------------------------------------------
`define SC_PERFMON_INCR_SIZE               7
`define SC_PERFMON_EV_QUAL_SIZE            3
`define SC_PERFMON_RSRC_REQ_SIZE           5
`define SC_PERFMON_RSRC_RSP_SIZE           `SC_PERFMON_INCR_SIZE

`define SC_PERFMON_EV_RBUF                 0
`define SC_PERFMON_EV_MSG                  1
`define SC_PERFMON_EV_TAG                  2

`define SC_PERFMON_RSRC_L2_REQQ            0
`define SC_PERFMON_RSRC_L3_BUSY            1
`define SC_PERFMON_RSRC_ANY_TO_L3          2
`define SC_PERFMON_RSRC_L2_TO_L3           3
`define SC_PERFMON_RSRC_L3_TO_SYS          4

`define SC_PERFMON_RSRC_L3_REQQ            0
`define SC_PERFMON_RSRC_L2_BUSY            1
`define SC_PERFMON_RSRC_ANY_TO_SYS         2
`define SC_PERFMON_RSRC_L2_TO_SYS          3
`define SC_PERFMON_RSRC_CBUF               4

// Clk Gate Disable defines
// --------------------------------------------------------------------------
`define SC_CLK_GATE_DISABLE_BANK           0
`define SC_CLK_GATE_DISABLE_REQQ_ALLOC     1
`define SC_CLK_GATE_DISABLE_REQQ_ENTRIES   2
`define SC_CLK_GATE_DISABLE_MESH_MASTER    3

// DV defines
// --------------------------------------------------------------------------
`define DV_TRANS_ID_SIZE  40  


`endif // _SC_DEFINES_


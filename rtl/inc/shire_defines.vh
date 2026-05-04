// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _SHIRE_DEFINES_
`define _SHIRE_DEFINES_

// Global defines

`define NUM_MINION_SHIRES   34

`define NUM_SHIRE_IDS    256
`define NUM_SHIRE_IDS_L  $clog2(`NUM_SHIRE_IDS)
`define NUM_SHIRE_IDS_R  (`NUM_SHIRE_IDS_L-1):0
`define LOCAL_SHIRE_ID   (`NUM_SHIRE_IDS_L)'('hFF)

`define NUM_SHIRES   64 // ceiling log 2 of number (Total number of instantiated shires)
`define NUM_SHIRES_L $clog2(`NUM_SHIRES)
`define NUM_SHIRES_R (`NUM_SHIRES_L-1):0

`define NUM_NEIGH    4 // Total number of neighbourhoods in a shire
`define NUM_NEIGH_L  $clog2(`NUM_NEIGH)
`define NUM_NEIGH_R  (`NUM_NEIGH_L-1):0

`define MIN_PER_N    8 // Minions per neighbourhood
`define MIN_PER_N_L  $clog2(`MIN_PER_N)
`define MIN_PER_N_R  (`MIN_PER_N_L-1):0
`define MIN_PER_S    (`MIN_PER_N * `NUM_NEIGH)
`define MIN_PER_S_L  $clog2(`MIN_PER_S)
`define MIN_PER_S_R  (`MIN_PER_S_L-1):0

`define IO_SHIRE_ID  254
`define P_SHIRE_ID   253

// nr threads
`define CORE_NR_THREADS   2
`define CORE_NR_THREADS_L $clog2(`CORE_NR_THREADS)
`define CORE_NR_THREADS_R (`CORE_NR_THREADS_L-1):0
`define THREADS_PER_N     (`MIN_PER_N * `CORE_NR_THREADS)
`define THREADS_PER_S     (`NUM_NEIGH * `THREADS_PER_N)

// minion and hart ids
`define MAX_MINION_ID        (`NUM_SHIRE_IDS*`NUM_NEIGH*`MIN_PER_N)
`define MAX_MINION_ID_L      $clog2(`MAX_MINION_ID)
`define MAX_MINION_ID_R      (`MAX_MINION_ID_L-1):0
`define MIN_ID_TO_NEIGH_ID_R `MIN_PER_N_L+:`NUM_NEIGH_L

`define MAX_HART_ID   (`MAX_MINION_ID*`CORE_NR_THREADS)
`define MAX_HART_ID_L $clog2(`MAX_HART_ID)
`define MAX_HART_ID_R (`MAX_HART_ID_L-1):0

// tboxes
`define NEIGH_PER_TBOX         1
//`define NEIGH_PER_TBOX_L       $clog2(`NEIGH_PER_TBOX)
//`define NEIGH_PER_TBOX_R       (`NEIGH_PER_TBOX_L-1):0
`define MIN_PER_TBOX           (`NEIGH_PER_TBOX*`MIN_PER_N)
`define MIN_PER_TBOX_L         $clog2(`MIN_PER_TBOX)
`define MIN_THREADS_PER_TBOX   (`MIN_PER_TBOX*`CORE_NR_THREADS)
`define MIN_THREADS_PER_TBOX_L $clog2(`MIN_THREADS_PER_TBOX)
`define TBOX_PER_SHIRE         (`NUM_NEIGH/`NEIGH_PER_TBOX)
`define TBOX_PER_SHIRE_L       $clog2(`TBOX_PER_SHIRE)
`define TBOX_PER_SHIRE_R       (`TBOX_PER_SHIRE_L-1):0

// icaches
`define MIN_PER_ICACHE   `MIN_PER_N
`define ICACHE_PER_N     (`MIN_PER_N / `MIN_PER_ICACHE)
`define ICACHE_PER_SHIRE (`ICACHE_PER_N * `NUM_NEIGH)

// ptws
`define MIN_PER_PTW 4
`define PTW_PER_N   (`MIN_PER_N / `MIN_PER_PTW)
`define PTW_PER_N_L $clog2(`PTW_PER_N)
`define PTW_PER_N_R (`PTW_PER_N_L-1):0

// rboxes
`define RBOX_PER_SHIRE 1

//voltage crossing
`define SHIRE_LO_VOLTAGE_FIFO_SIZE 3
`define SHIRE_HI_VOLTAGE_FIFO_SIZE 3

`define ESR_FILTER_IPI_RESET_VAL 64'hFFFF_FFFF_FFFF_FFFF

//clock gate control
`define ESR_SHIRE_CLK_GATE_CTRL_RESET_VAL 11'h000

// NEIGHBORHOOD
// ------------
`define NEIGH_NUM_REQ_AGENTS  (`MIN_PER_N+`ICACHE_PER_N+`PTW_PER_N+3)
`define NEIGH_NUM_RESP_AGENTS (`MIN_PER_N+`ICACHE_PER_N+`PTW_PER_N+1)
`define NEIGH_AGENT_MINION    0
`define NEIGH_AGENT_ICACHE    `MIN_PER_N
`define NEIGH_AGENT_PTW       (`MIN_PER_N+`ICACHE_PER_N)
`define NEIGH_AGENT_TBOX      (`MIN_PER_N+`ICACHE_PER_N+`PTW_PER_N)
`define NEIGH_AGENT_TLOAD     (`MIN_PER_N+`ICACHE_PER_N+`PTW_PER_N+1)
`define NEIGH_AGENT_TSTORE    (`MIN_PER_N+`ICACHE_PER_N+`PTW_PER_N+2)

// Neighborhood data size
`define NEIGH_DATA_SIZE          256
`define NEIGH_DATA_RANGE         (`NEIGH_DATA_SIZE-1):0
`define NEIGH_BLOCK_PASSES       (512/`NEIGH_DATA_SIZE)            // Number of passes per evict/fill
`define NEIGH_BLOCK_PASSES_RANGE ($clog2(`NEIGH_BLOCK_PASSES)-1):0 // Range to count number of passes

// Minion TBOX request FIFO
`define NEIGH_MIN_TBOX_REQ_FIFO_SIZE 2
// TBOX neighborhood request FIFO
`define TBOX_NEIGH_REQ_FIFO_SIZE     2
// TBOX L2 reply FIFO
`define NEIGH_SC_TBOX_RSP_FIFO_SIZE  2

// Intermediate FIFO for miss requests
`define NEIGH_INT_FIFO_SIZE       2
`define NEIGH_INT_FIFO_SIZE_L     $clog2(`NEIGH_INT_FIFO_SIZE)
`define NEIGH_INT_FIFO_SIZE_RANGE (NEIGH_INT_FIFO_SIZE_L-1):0

// Per-bank FIFOs for miss requests
`define NEIGH_BANK_FIFO_SIZE 3

// Fill FIFO
`define NEIGH_FILL_FIFO_SIZE  4
`define NEIGH_FILL_FIFO_RANGE ($clog2(`NEIGH_FILL_FIFO_SIZE)-1):0

// Fast local message network
`define NEIGH_MAX_LOCAL_CONNECT 3

// Force destination FIFOs
`define NEIGH_FORCE_DEST_FIFO_BITS  2
`define NEIGH_FORCE_DEST_FIFO_BANKS `NEIGH_FORCE_DEST_FIFO_BITS'd0
`define NEIGH_FORCE_DEST_FIFO_UC    `NEIGH_FORCE_DEST_FIFO_BITS'd1
`define NEIGH_FORCE_DEST_FIFO_TBOX  `NEIGH_FORCE_DEST_FIFO_BITS'd2

`define NEIGH_FORCE_AGENT_BITS      2
`define NEIGH_FORCE_AGENT_DCACHE    `NEIGH_FORCE_AGENT_BITS'd0
`define NEIGH_FORCE_AGENT_ICACHE    `NEIGH_FORCE_AGENT_BITS'd1
`define NEIGH_FORCE_AGENT_TBOX      `NEIGH_FORCE_AGENT_BITS'd2
`define NEIGH_FORCE_AGENT_PTW       `NEIGH_FORCE_AGENT_BITS'd3

// Cooperative miss requests
// -------------------------
`define SHIRE_COOP_MISS_REQS   2 // TLoads to TenB and Scp
`define SHIRE_COOP_NUM_IDS     32
`define SHIRE_COOP_RDY_CREDS   2'd3
`define SHIRE_COOP_RDY_CREDS_L $clog2(`SHIRE_COOP_RDY_CREDS)
`define SHIRE_COOP_RDY_CREDS_R (`SHIRE_COOP_RDY_CREDS_L-1):0

// Data fields for cooperative requests
`define SHIRE_COOP_MIN_MASK_START   0
`define SHIRE_COOP_MIN_MASK_SIZE    `MIN_PER_N
`define SHIRE_COOP_SEQ_ID_START     `SHIRE_COOP_MIN_MASK_SIZE
`define SHIRE_COOP_SEQ_ID_SIZE      3
`define SHIRE_COOP_SEQ_ID_RANGE     (`SHIRE_COOP_SEQ_ID_SIZE-1):0
`define SHIRE_COOP_ID_START         (`SHIRE_COOP_SEQ_ID_START+`SHIRE_COOP_SEQ_ID_SIZE)
`define SHIRE_COOP_ID_SIZE          $clog2(`SHIRE_COOP_NUM_IDS)
`define SHIRE_COOP_ID_RANGE         (`SHIRE_COOP_ID_SIZE-1):0
`define SHIRE_COOP_LOAD_ID_START    `SHIRE_COOP_SEQ_ID_START
`define SHIRE_COOP_LOAD_ID_SIZE     (`SHIRE_COOP_ID_SIZE+`SHIRE_COOP_SEQ_ID_SIZE)
`define SHIRE_COOP_LOAD_ID_RANGE    (`SHIRE_COOP_LOAD_ID_SIZE-1):0
`define SHIRE_COOP_NEIGH_MASK_START (`SHIRE_COOP_LOAD_ID_START+`SHIRE_COOP_LOAD_ID_SIZE)
`define SHIRE_COOP_NEIGH_MASK_SIZE  `NUM_NEIGH

`define SHIRE_COOP_DATA_SIZE        (`SHIRE_COOP_NEIGH_MASK_SIZE+`SHIRE_COOP_LOAD_ID_SIZE+`SHIRE_COOP_MIN_MASK_SIZE)
`define SHIRE_COOP_DATA_RANGE       (`SHIRE_COOP_DATA_SIZE-1):0

// Address bits to compute master neigh
`define SHIRE_COOP_MST_NEIGH_ADDR_START 6
`define SHIRE_COOP_MST_NEIGH_ADDR_SIZE  `NUM_NEIGH_L
`define SHIRE_COOP_MST_NEIGH_ADDR_RANGE `SHIRE_COOP_MST_NEIGH_ADDR_START +: `SHIRE_COOP_MST_NEIGH_ADDR_SIZE

// Tag table
`define SHIRE_MAX_COOP           8
`define SHIRE_COOP_MISS_ID_SIZE  $clog2(`SHIRE_MAX_COOP)
`define SHIRE_COOP_MISS_ID_RANGE (`SHIRE_COOP_MISS_ID_SIZE-1):0

`define SHIRE_COOP_TAG_TABLE_DEPTH 32 // (`MIN_PER_N*`SHIRE_MAX_COOP)
`define SHIRE_COOP_TAG_TABLE_SIZE  $clog2(`SHIRE_COOP_TAG_TABLE_DEPTH)
`define SHIRE_COOP_TAG_TABLE_RANGE (`SHIRE_COOP_TAG_TABLE_SIZE-1):0

// Address range
// -------------
`define VA_SIZE  48
`define VA_MSB   (`VA_SIZE-1)
`define VA_RANGE `VA_MSB:0
`define VA_RANGE_CA  `VA_MSB:6

`define VA39_SIZE  39
`define VA39_MSB   (`VA39_SIZE-1)
`define VA39_RANGE `VA39_MSB:0
`define VA39_RANGE_CA  `VA39_MSB:6

`define PA_SIZE  40
`define PA_MSB   (`PA_SIZE-1)
`define PA_RANGE `PA_MSB:0
`define PA_RANGE_CA `PA_MSB:6
`define VA_NO_PA_SIZE  (`VA_SIZE-`PA_SIZE)
`define VA_NO_PA_RANGE `VA_MSB:`PA_SIZE

// MEMORY MAP
// ----------
// SystemReg region
// ----------
`define AD_SYSTEMREG_REGION_START     12
`define AD_SYSTEMREG_REGION_SIZE      (`PA_SIZE-`AD_SYSTEMREG_REGION_START)
`define AD_SYSTEMREG_REGION_RANGE     `PA_MSB:`AD_SYSTEMREG_REGION_START
`define AD_SYSTEMREG_REGION           (`AD_SYSTEMREG_REGION_SIZE)'('h02000)

// MramReg region
// ----------
`define AD_MRAMREG_REGION_START       12
`define AD_MRAMREG_REGION_SIZE        (`PA_SIZE-`AD_MRAMREG_REGION_START)
`define AD_MRAMREG_REGION_RANGE       `PA_MSB:`AD_MRAMREG_REGION_START
`define AD_MRAMREG_REGION             (`AD_MRAMREG_REGION_SIZE)'('h02001)

// IO region
// ----------
`define AD_IO1_REGION_START           13
`define AD_IO1_REGION_SIZE            (`PA_SIZE-`AD_IO1_REGION_START)
`define AD_IO1_REGION_RANGE           `PA_MSB:`AD_IO1_REGION_START
`define AD_IO1_REGION                 (`AD_IO1_REGION_SIZE)'('h01001)

`define AD_IO2_REGION_START           12
`define AD_IO2_REGION_SIZE            (`PA_SIZE-`AD_IO2_REGION_START)
`define AD_IO2_REGION_RANGE           `PA_MSB:`AD_IO2_REGION_START
`define AD_IO2_REGION                 (`AD_IO2_REGION_SIZE)'('h02004)

// xSPI region
// ----------
`define AD_XSPI_REGION_START          12
`define AD_XSPI_REGION_SIZE           (`PA_SIZE-`AD_XSPI_REGION_START)
`define AD_XSPI_REGION_RANGE          `PA_MSB:`AD_XSPI_REGION_START
`define AD_XSPI_REGION                (`AD_XSPI_REGION_SIZE)'('h0200F)

// BootROM region
// ----------
`define AD_BOOTROM_REGION_START       13
`define AD_BOOTROM_REGION_SIZE        (`PA_SIZE-`AD_BOOTROM_REGION_START)
`define AD_BOOTROM_REGION_RANGE       `PA_MSB:`AD_BOOTROM_REGION_START
`define AD_BOOTROM_REGION             (`AD_BOOTROM_REGION_SIZE)'('h1004)

// SRAM region
// ----------
`define AD_SRAM_REGION_START          12
`define AD_SRAM_REGION_SIZE           (`PA_SIZE-`AD_SRAM_REGION_START)
`define AD_SRAM_REGION_RANGE          `PA_MSB:`AD_SRAM_REGION_START
`define AD_SRAM_REGION                (`AD_SRAM_REGION_SIZE)'('h0200C)

// NIC config region
`define AD_NIC_REGION_START           24
`define AD_NIC_REGION_SIZE            (`PA_SIZE-`AD_NIC_REGION_START)
`define AD_NIC_REGION_RANGE           `PA_MSB:`AD_NIC_REGION_START
`define AD_NIC_REGION                 (`AD_NIC_REGION_SIZE)'('hFE)

// DDR region (MRAM)
// ----------
`define AD_DDR_REGION_START          24
`define AD_DDR_REGION_SIZE           (`PA_SIZE-`AD_DDR_REGION_START)
`define AD_DDR_REGION_RANGE          `PA_MSB:`AD_DDR_REGION_START
`define AD_DDR_REGION                (`AD_DDR_REGION_SIZE)'('h40)

`define AD_MRAM_SUBREGION_SIZE       12
`define AD_MRAM_SUBREGION_RANGE      ((`AD_DDR_REGION_START-1) -: `AD_MRAM_SUBREGION_SIZE)

// OTP region
// ----------
`define AD_OTP_REGION_START          14
`define AD_OTP_REGION_SIZE           (`PA_SIZE-`AD_OTP_REGION_START)
`define AD_OTP_REGION_RANGE          `PA_MSB:`AD_OTP_REGION_START
`define AD_OTP_REGION                (`AD_OTP_REGION_SIZE)'('h1FFFF)

// ESR region
// ----------
`define AD_ESR_REGION_START          24
`define AD_ESR_REGION_SIZE           (`PA_SIZE-`AD_ESR_REGION_START)
`define AD_ESR_REGION_RANGE          `PA_MSB:`AD_ESR_REGION_START
`define AD_ESR_REGION                (`AD_ESR_REGION_SIZE)'('h80)

// PLIC region
// ----------
`define AD_PLIC_REGION_START          24
`define AD_PLIC_REGION_SIZE           (`PA_SIZE-`AD_PLIC_REGION_START)
`define AD_PLIC_REGION_RANGE          `PA_MSB:`AD_PLIC_REGION_START
`define AD_PLIC_REGION                (`AD_PLIC_REGION_SIZE)'('hA0)

// Address fields in the ESR region
// Agent type: Minion
`define AD_ESR_PORT_MSG_START        0
`define AD_ESR_PORT_MSG_SIZE         12
`define AD_ESR_PORT_MSG_COMMON       `AD_ESR_PORT_MSG_SIZE'h800
`define AD_ESR_PORT_MSG_REDUCE_READY `AD_ESR_PORT_MSG_SIZE'he00
`define AD_ESR_PORT_MSG_REDUCE_DATA  `AD_ESR_PORT_MSG_SIZE'he20
`define AD_ESR_PORT_MSG_TBOX_REPLY   `AD_ESR_PORT_MSG_SIZE'hc00
`define AD_ESR_PORT_MSG_TBOX_PULL    `AD_ESR_PORT_MSG_SIZE'he40
`define AD_ESR_MESSAGE_ID_START      3
`define AD_ESR_MESSAGE_ID_SIZE       8
`define AD_ESR_MESSAGE_ID_RANGE      `AD_ESR_MESSAGE_ID_START +: `AD_ESR_MESSAGE_ID_SIZE
`define AD_ESR_VALID_BIT_START       (`AD_ESR_MESSAGE_ID_START+`ET_MSG_VALID_BIT)
`define AD_ESR_VALID_BIT_SIZE        1
`define AD_ESR_PORT_ID_START         (`AD_ESR_MESSAGE_ID_START+`ET_MSG_PORT_ID_START)
`define AD_ESR_PORT_ID_SIZE          `ET_MSG_PORT_ID_SIZE
`define AD_ESR_PORT_ID_RANGE         `AD_ESR_PORT_ID_START +: `AD_ESR_PORT_ID_SIZE
`define AD_ESR_TH_ID_START           (`AD_ESR_PORT_MSG_START+`AD_ESR_PORT_MSG_SIZE)
`define AD_ESR_TH_ID_SIZE            1
`define AD_ESR_NEIGH_MIN_ID_START    (`AD_ESR_TH_ID_START+`AD_ESR_TH_ID_SIZE)
`define AD_ESR_NEIGH_MIN_ID_SIZE     3
`define AD_ESR_NEIGH_HART_ID_START   `AD_ESR_TH_ID_START
`define AD_ESR_NEIGH_HART_ID_SIZE    (`AD_ESR_NEIGH_MIN_ID_SIZE+`AD_ESR_TH_ID_SIZE)
`define AD_ESR_NEIGH_HART_ID_RANGE   `AD_ESR_NEIGH_HART_ID_START +: `AD_ESR_NEIGH_HART_ID_SIZE
`define AD_ESR_NEIGH_ID_START        (`AD_ESR_NEIGH_MIN_ID_START+`AD_ESR_NEIGH_MIN_ID_SIZE)
`define AD_ESR_NEIGH_ID_SIZE         4
`define AD_ESR_NEIGH_ID_RANGE        `AD_ESR_NEIGH_ID_START +: `AD_ESR_NEIGH_ID_SIZE
`define AD_ESR_SHIRE_MIN_ID_START    `AD_ESR_NEIGH_MIN_ID_START
`define AD_ESR_SHIRE_MIN_ID_SIZE     (`AD_ESR_NEIGH_ID_SIZE+`AD_ESR_NEIGH_MIN_ID_SIZE)
`define AD_ESR_SHIRE_MIN_ID_RANGE    `AD_ESR_SHIRE_MIN_ID_START +: `AD_ESR_SHIRE_MIN_ID_SIZE
`define AD_ESR_SHIRE_HART_ID_START   `AD_ESR_TH_ID_START
`define AD_ESR_SHIRE_HART_ID_SIZE    (`AD_ESR_SHIRE_MIN_ID_SIZE+`AD_ESR_TH_ID_SIZE)
`define AD_ESR_SHIRE_HART_ID_RANGE   `AD_ESR_SHIRE_HART_ID_START +: `AD_ESR_SHIRE_HART_ID_SIZE
// Agent type: TBOX
`define AD_ESR_TBOX_COMM_START       7
`define AD_ESR_TBOX_COMM_SIZE        13
`define AD_ESR_TBOX_COMM_REQ         `AD_ESR_TBOX_COMM_SIZE'h200
`define AD_ESR_TBOX_COMM_DATA        `AD_ESR_TBOX_COMM_SIZE'h208
// Common to all agent types
`define AD_ESR_AGENT_TYPE_START      20
`define AD_ESR_AGENT_TYPE_SIZE       2
`define AD_ESR_AGENT_TYPE_RANGE      `AD_ESR_AGENT_TYPE_START +: `AD_ESR_AGENT_TYPE_SIZE
`define AD_ESR_AGENT_TYPE_MINION     `AD_ESR_AGENT_TYPE_SIZE'd0
`define AD_ESR_AGENT_TYPE_TBOX       `AD_ESR_AGENT_TYPE_SIZE'd2
`define AD_ESR_AGENT_TYPE_SHIRE      `AD_ESR_AGENT_TYPE_SIZE'd3
`define AD_ESR_PROT_BITS_START       (`AD_ESR_AGENT_TYPE_START+`AD_ESR_AGENT_TYPE_SIZE)
`define AD_ESR_PROT_BITS_SIZE        2
`define AD_ESR_PROT_BITS_RANGE       `AD_ESR_PROT_BITS_START+:`AD_ESR_PROT_BITS_SIZE
`define AD_ESR_PROT_BITS_MSG         `AD_ESR_PROT_BITS_SIZE'd2
`define AD_ESR_PROT_BITS_MMODE       `AD_ESR_PROT_BITS_SIZE'd3
`define AD_ESR_PROT_BITS_UMODE       `AD_ESR_PROT_BITS_SIZE'd0
`define AD_ESR_SHIRE_ID_START        (`AD_ESR_PROT_BITS_START+`AD_ESR_PROT_BITS_SIZE)
`define AD_ESR_SHIRE_ID_SIZE         7
`define AD_ESR_SHIRE_ID_RANGE        `AD_ESR_SHIRE_ID_START+:`AD_ESR_SHIRE_ID_SIZE

// SCP region
// ----------
`define AD_SCP_REGION_START          32
`define AD_SCP_REGION_SIZE           (`PA_SIZE-`AD_SCP_REGION_START)
`define AD_SCP_REGION_RANGE          `PA_MSB:`AD_SCP_REGION_START
`define AD_SCP_REGION                (`AD_SCP_REGION_SIZE)'('b1)

// Address fields in the SCP region
// Format 0 between minions and neighborhood
`define AD_SCP_F0_SHIRE_ID_START     23
`define AD_SCP_F0_SHIRE_ID_SIZE      7
`define AD_SCP_F0_SHIRE_ID_RANGE     `AD_SCP_F0_SHIRE_ID_START+:`AD_SCP_F0_SHIRE_ID_SIZE
`define AD_SCP_F0_SHIRE_ID_MSB       (`AD_SCP_F0_SHIRE_ID_START+`AD_SCP_F0_SHIRE_ID_SIZE-1)
// Format 1 between minions and neighborhood
`define AD_SCP_F1_OFFSET_START0      0
`define AD_SCP_F1_OFFSET_SIZE0       6
`define AD_SCP_F1_OFFSET_RANGE0      `AD_SCP_F1_OFFSET_START0+:`AD_SCP_F1_OFFSET_SIZE0
`define AD_SCP_F1_SHIRE_ID_START0    (`AD_SCP_F1_OFFSET_START0+`AD_SCP_F1_OFFSET_SIZE0)
`define AD_SCP_F1_SHIRE_ID_SIZE0     5
`define AD_SCP_F1_SHIRE_ID_RANGE0    `AD_SCP_F1_SHIRE_ID_START0+:`AD_SCP_F1_SHIRE_ID_SIZE0
`define AD_SCP_F1_OFFSET_START1      (`AD_SCP_F1_SHIRE_ID_START0+`AD_SCP_F1_SHIRE_ID_SIZE0)
`define AD_SCP_F1_OFFSET_SIZE1       17
`define AD_SCP_F1_OFFSET_RANGE1      `AD_SCP_F1_OFFSET_START1+:`AD_SCP_F1_OFFSET_SIZE1
`define AD_SCP_F1_SHIRE_ID_START1    (`AD_SCP_F1_OFFSET_START1+`AD_SCP_F1_OFFSET_SIZE1)
`define AD_SCP_F1_SHIRE_ID_SIZE1     2
`define AD_SCP_F1_SHIRE_ID_RANGE1    `AD_SCP_F1_SHIRE_ID_START1+:`AD_SCP_F1_SHIRE_ID_SIZE1
`define AD_SCP_F1_SHIRE_ID_MSB       (`AD_SCP_F1_SHIRE_ID_START1+`AD_SCP_F1_SHIRE_ID_SIZE1-1)
// Common to both formats
`define AD_SCP_FORMAT_START          30
`define AD_SCP_FORMAT_SIZE           1
`define AD_SCP_FORMAT_RANGE          `AD_SCP_FORMAT_START+:`AD_SCP_FORMAT_SIZE
`define AD_SCP_FORMAT_0              `AD_SCP_FORMAT_SIZE'b0
`define AD_SCP_FORMAT_1              `AD_SCP_FORMAT_SIZE'b1
// Common L2 Format
`define AD_SCP_FL2_OFFSET_START      0
`define AD_SCP_FL2_OFFSET_SIZE       23
`define AD_SCP_FL2_OFFSET_RANGE      `AD_SCP_FL2_OFFSET_START+:`AD_SCP_FL2_OFFSET_SIZE
`define AD_SCP_FL2_SHIRE_ID_START    (`AD_SCP_FL2_OFFSET_START+`AD_SCP_FL2_OFFSET_SIZE)
`define AD_SCP_FL2_SHIRE_ID_SIZE     8
`define AD_SCP_FL2_SHIRE_ID_RANGE    `AD_SCP_FL2_SHIRE_ID_START+:`AD_SCP_FL2_SHIRE_ID_SIZE
`define AD_SCP_FL2_SHIRE_ID_MSB      (`AD_SCP_FL2_SHIRE_ID_START+`AD_SCP_FL2_SHIRE_ID_SIZE-1)


// L2 writeAround size
`define CORE_L2_BLOCK_SIZE             128                                                            // Bits done per pass
`define CORE_L2_BLOCK_RANGE            (`CORE_L2_BLOCK_SIZE-1):0                                      // Range
`define CORE_L2_BLOCK_PASSES           (512/`CORE_L2_BLOCK_SIZE)                                      // Number of passes per writeAround
`define CORE_L2_BLOCK_PASSES_RANGE     ($clog2(`CORE_L2_BLOCK_PASSES)-1):0                            // Range to count number of passes
`define CORE_L2_BLOCK_ADDR_OFFSET_BITS $clog2(`CORE_L2_BLOCK_SIZE/8)                                  // Addr bits that are within a block
`define CORE_L2_BLOCK_PASS_NUM_RANGE   `CORE_L2_BLOCK_ADDR_OFFSET_BITS+:$clog2(`CORE_L2_BLOCK_PASSES) // Bits where the pass number is from a 5:0 range

// L2 fill/evict size
`define CORE_L2_BLOCK_EXT_SIZE             256                                                                    // Bits done per pass
`define CORE_L2_BLOCK_EXT_RANGE            (`CORE_L2_BLOCK_EXT_SIZE-1):0                                          // Range
`define CORE_L2_BLOCK_EXT_PASSES           (512/`CORE_L2_BLOCK_EXT_SIZE)                                          // Number of passes per evict/fill
`define CORE_L2_BLOCK_EXT_PASSES_RANGE     ($clog2(`CORE_L2_BLOCK_EXT_PASSES)-1):0                                // Range to count number of passes
`define CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS $clog2(`CORE_L2_BLOCK_EXT_SIZE/8)                                      // Addr bits that are within a block
`define CORE_L2_BLOCK_EXT_PASS_NUM_RANGE   `CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS+:$clog2(`CORE_L2_BLOCK_EXT_PASSES) // Bits where the pass number is from a 5:0 range

// APB interfaces
// --------------
`define SBM_APB_INTERFACES (`NUM_NEIGH + `SC_BANKS + 2) //neigh, banks,shire esr, rbox 
`define SHIRE_APB_INTERFACES (`SBM_APB_INTERFACES + 1) //+shire icache mem
`define SHIRE_CHANNEL_APB_INTERFACES  (`SC_BANKS + 3) //banks, shire esr, rbox , icache

// UNCACHED
// --------
`define UC_DATA_SIZE  256
`define UC_DATA_RANGE (`UC_DATA_SIZE-1):0


// MTIME  
// ----------------------------------------
`define MTIME_TICK_PRESCALER 100
`define MTIME_TICK_CNT_R ($clog2(`MTIME_TICK_PRESCALER)-1):0

`define FCC_PER_MIN   2
`define FCC_PER_MIN_R ($clog2(`FCC_PER_MIN)-1):0

// DEBUG pipeline stages Neight to virtual shire_channel logic
`define SHIRE_DEBUG_PIPELINE_STAGES 2

// NOC interrupts
`define NOC_INT_NUM 19
`define NOC_DBG_INT_NUM 2

// NOC scan_mode
`define NOC_SCAN_MODE_NUM 1

// PLL and DLL interface parameters
// ----------------------------------------
`define SHIRE_PLL_NUM_CLKREF        3
`define SHIRE_PLL_NUM_CLKPLL        4
`define SHIRE_PLL_ADDR_BITS         6
`define SHIRE_PLL_DATA_BITS         16
`define SHIRE_PLL_NUM_DATA_BYTES    2
`define SHIRE_PLL_NUM_SCAN_CHAINS   40
`define SHIRE_PLL_NUM_SCAN_CLOCKS   1
`define SHIRE_PLL_CLOCK_DIVIDE_BITS 4
`define SHIRE_PLL_CLOCK_REGPTR_BITS 6
`define SHIRE_PLL_CLOCK_REGNUM_BITS 4
`define SHIRE_PLL_NUM_TDR_CHAINS    2
`define SHIRE_PLL_NUM_TDR_SEL_BITS  2
`define SHIRE_PLL_ESR_CTRL_BITS     $bits(esr_pll_auto_config_t)
`define SHIRE_PLL_ESR_CONF_BITS     256  // 16 registers of 16 bits
`define SHIRE_PLL_ESR_READ_BITS     (`SHIRE_PLL_DATA_BITS+2)
`define SHIRE_PLL_LOCK_DELAY_BITS   5

`define SHIRE_DLL_ADDR_BITS         3
`define SHIRE_DLL_DATA_BITS         16
`define SHIRE_DLL_NUM_DATA_BYTES    2
`define SHIRE_DLL_NUM_SCAN_CHAINS   10
`define SHIRE_DLL_CLOCK_DIVIDE_BITS 4
`define SHIRE_DLL_CLOCK_REGPTR_BITS 3
`define SHIRE_DLL_CLOCK_REGNUM_BITS 3
`define SHIRE_DLL_NUM_TDR_CHAINS    2
`define SHIRE_DLL_NUM_TDR_SEL_BITS  2
`define SHIRE_DLL_ESR_CTRL_BITS     $bits(esr_dll_auto_config_t)
`define SHIRE_DLL_ESR_CONF_BITS     128   // 8 registers of 16 bits
`define SHIRE_DLL_ESR_READ_BITS     (`SHIRE_DLL_DATA_BITS+2)
`define SHIRE_DLL_LOCK_DELAY_BITS   5

// Memory override bits
`define SHIRE_MINION_MEM_OVERRIDE_BITS 3
`define SHIRE_MINION_MEM_OVERRIDE_SEL  2
`define SHIRE_MINION_MEM_OVERRIDE_WE   1
`define SHIRE_MINION_MEM_OVERRIDE_RE   0

// DFT
// ----------------------------------------
`define ET_DFT_INSERTION                          0  // set this to 1 during and after dft insertion
`define SHIRE_DFT_MODE_WIDTH                        4  // dft test mode width
`define SHIRE_DFT_BC_CTL_WIDTH                     `SOCTOP_DFT_BC_CTL_WIDTH  // dft broadcast control bits to all shires
`define SHIRE_DFT_CNTL_WIDTH                       10  // dft spare control bits
`define SHIRE_DFT_SCAN_DUMP_CNTL_WIDTH              8  // extra scan dump control bits in addition to wei/rei
                                                 
`define SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH   `SOCTOP_DFT_SI_SO_TOP_WIDTH  // common shire e2w, w2e scan_in/scan_out widths
`define SHIRE_DFT_SCANIN_E2W_MEMSHIRE_WIDTH        `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANIN_W2E_MEMSHIRE_WIDTH        `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANIN_E2W_SHIRE_TOP_WIDTH       `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANIN_W2E_SHIRE_TOP_WIDTH       `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANIN_NEIGH_WIDTH               12
`define SHIRE_DFT_SCANIN_NOC_WIDTH                 43
`define SHIRE_DFT_SCANIN_SCW_WIDTH                 16 // shire channel wrap
`define SHIRE_DFT_SCANIN_TBOX_WIDTH                8
`define SHIRE_DFT_SCANIN_MINION_WIDTH              8  
`define SHIRE_DFT_SCANIN_SA_MINION_WIDTH           12 // standalone minion   
`define SHIRE_DFT_SCANIN_SHIRE_CACHE_BANK_WIDTH    230

`define SHIRE_DFT_SCANOUT_E2W_MEMSHIRE_WIDTH       `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANOUT_W2E_MEMSHIRE_WIDTH       `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANOUT_E2W_SHIRE_TOP_WIDTH      `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANOUT_W2E_SHIRE_TOP_WIDTH      `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define SHIRE_DFT_SCANOUT_NEIGH_WIDTH               12
`define SHIRE_DFT_SCANOUT_NOC_WIDTH                 43
`define SHIRE_DFT_SCANOUT_SCW_WIDTH                 16
`define SHIRE_DFT_SCANOUT_TBOX_WIDTH                8
`define SHIRE_DFT_SCANOUT_MINION_WIDTH              8  
`define SHIRE_DFT_SCANOUT_SA_MINION_WIDTH           12   
`define SHIRE_DFT_SCANOUT_SHIRE_CACHE_BANK_WIDTH    `SHIRE_DFT_SCANIN_SHIRE_CACHE_BANK_WIDTH   

// ECO port width defines
`define ET_ECO_SHIRE_TOP_INPUT_WIDTH               10
`define ET_ECO_SHIRE_TOP_OUTPUT_WIDTH              10
`define ET_ECO_NEIGH_INPUT_WIDTH                   10
`define ET_ECO_NEIGH_OUTPUT_WIDTH                  10
`define ET_ECO_SCW_INPUT_WIDTH                     10
`define ET_ECO_SCW_OUTPUT_WIDTH                    10
`define ET_ECO_TBOX_INPUT_WIDTH                    10
`define ET_ECO_TBOX_OUTPUT_WIDTH                   10
`define ET_ECO_MINION_INPUT_WIDTH                  10
`define ET_ECO_MINION_OUTPUT_WIDTH                 10
`define ET_ECO_SA_MINION_INPUT_WIDTH               10
`define ET_ECO_SA_MINION_OUTPUT_WIDTH              10
`define ET_ECO_SHIRE_CACHE_BANK_INPUT_WIDTH        10
`define ET_ECO_SHIRE_CACHE_BANK_OUTPUT_WIDTH       10

// SMS minion shire specific 
`define SHIRE_SCAN_ROWS                             6  // number of soc_top rows for scan routing
`define SHIRE_SCAN_COLS                             6  // number of soc_top rows for scan routing
`define SHIRE_SMS_WS_CHAINS                         6  // number of chains from dft_control_unit
`define SHIRE_SMS_FB_CHAINS                         5  // number of fb chains in soc_top routing


`endif // _SHIRE_DEFINES_

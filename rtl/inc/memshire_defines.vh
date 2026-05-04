// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// memshire_defines.vh
//
// defines only

`ifndef _MEMSHIRE_DEFINES_
`define _MEMSHIRE_DEFINES_

// --------------------------------------------------------------------------
// Memshire revision ids
// --------------------------------------------------------------------------
// NOTE: memshire revision id (32-bit) = b3.b2.b1.b0
`define MEMSHIRE_B3_REVISION_ID            0
`define MEMSHIRE_B2_REVISION_ID            0
`define MEMSHIRE_B1_REVISION_ID            0
`define MEMSHIRE_B0_REVISION_ID            1

// Memory address
`define MEMSHIRE_SEL_ADDR_RANGE      8:6
`define MEMSHIRE_MCTL_SEL_ADDR_RANGE 9
`define MEMSHIRE_AXI_SLAVE_CONVERTED_ID_SIZE (`SC_MESH_SLAVE_AXI_ID_SIZE -2)
`define MEM_MASTER_AXI_ID_SIZE     (`MEMSHIRE_AXI_SLAVE_CONVERTED_ID_SIZE  + 3)
`define MEM_MASTER_AXI_ADDR_SIZE   (`SC_MESH_MASTER_AXI_ADDR_SIZE-5)

// Netspeed virtual channel AXI  - VSI = Virtual Slave Interface
`define MEM_MESH_SLAVE_VSI_DEPTH 3


// Definition for DDR controller signals

`define MEM_MASTER_AXI_REGION_SIZE     4
`define DDR_XAQ_WCOUNT_SIZE            3
`define DDR_SELF_REF_STAT_SIZE         2
`define DDR_DBG_DFI_IE_SIZE            3
`define DDR_CRD_CNT_SIZE               7
`define DDR_BANKS_SIZE                 3
`define DRR_HIF_ADDR_MAX_SIZE          36
`define DDR_DRAM_DATA_SIZE             16

`define DDR_ATPG_LU_SIZE               6
`define DDR_ATPG_SEL_SIZE              6

`define DDR_NUM_SCAN_CHAINS            46
`define DDR_BP_A_SIZE                  24
`define DDR_BP_D_SIZE                  48
`define DDR_NUM_ANIBS                  6
`define DDR_NUM_DBYTES                 4
`define DDR_NUM_MASTERS                2
`define DDR_NUM_PORTS                  2
`define DDR_TEST_MODE_SIZE             2
`define DDR_SMS_ALGO_SEL_SIZE          2
`define DDR_SMS_RING_BYPASS_SIZE       7
`define DDR_DFI_ERR_SIZE               4
`define DDR_DFI_CS_SIZE                2
`define DDR_NUM_TOP_SCAN_CHAINS ( (30*`DDR_NUM_DBYTES) + (8*`DDR_NUM_ANIBS) + 2 )

// Note: the memory controller has a 32 bit APB address bus, but only uses 26 of those bits
`define MEM_DDRC_APB_A_WIDTH    26

//
// the AxADDR bit on the memshire smb input that determines a memshire esr
// request (1) vs. a memory controller request (0)
//
`define MEM_AXI_ESR_SEL_BIT   32

// The APB paddr bit that determines a memshire esr request (1) vs. a mem ctrl
// req (0). This is placed right after the last mem controller address bit
`define MEM_APB_MESR_SEL1_BIT   `MEM_DDRC_APB_A_WIDTH
`define MEM_APB_MESR_SEL2_BIT   9

// THe APB paddr bit that determines a PLL request
`define MEM_APB_PLL_SEL_BIT     24

// memshire address and data width for apb bus that is shared between the esr and mem controller
`define MEM_REG_APB_A_WIDTH   32
`define MEM_REG_APB_D_WIDTH   64

// BPAM address and data width
`define MEMSHIRE_BPAM_APB_ADDR_WIDTH `MEM_REG_APB_A_WIDTH
`define MEMSHIRE_BPAM_APB_DATA_WIDTH `MEM_REG_APB_D_WIDTH

// address and data width for pll apb bus 
`define MEM_PLL_APB_A_WIDTH   6
`define MEM_PLL_APB_D_WIDTH   16

// memory controller apb data width
`define MEM_DDRC_APB_D_WIDTH    32

// memshire id width
`define MEMSHIRE_ID_SIZE  8

// size of writes counter, one per priority - large enough for AW request fifo + uMCTL input fifo X 2 + pipeline + B uMCTL output fifo X 2 = ~12
`define MEMSHIRE_WRITE_CNT_SIZE 4

// auto config width (dv only)
`define MS_AUTO_CONFIG_WIDTH 16

`define MEMSHIRE_AUTO_INIT_CONFIG_ECC_BIT     0
`define MEMSHIRE_AUTO_INIT_CONFIG_PLL_REAL_EN 3
`define MEMSHIRE_AUTO_INIT_CONFIG_FREQ_BITS   14:13 
`define MEMSHIRE_AUTO_INIT_CONFIG_FREQ_933    2'b01
`define MEMSHIRE_AUTO_INIT_CONFIG_FREQ_800    2'b10
`define MEMSHIRE_AUTO_INIT_CONFIG_AUTO_PRECHARGE 4
`define MEMSHIRE_AUTO_INIT_CONFIG_PWR_RUN        5
`define MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_BITS  2:1
`define MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_4GB   2'b01
`define MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_8GB   2'b10
`define MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_16GB  2'b00
`define MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_32GB  2'b11

// the size of the padding required on the memshire APB address bus for ESR
// and the mem controller apb bus
`define MEM_ESR_APB_A_PAD_WIDTH (`MEM_DDRC_APB_A_WIDTH - `ESR_MEMSHIRE_APB_AD_PP_WIDTH)    
`define MEM_DDRC_APB_A_PAD_WIDTH  (`MEM_REG_APB_A_WIDTH - `MEM_DDRC_APB_A_WIDTH - 1)

// mem shire specific ESR defines
`define MEMSHIRE_ESR_STATUS_SIZE       64
`define MEMSHIRE_ADDR_BIT_SEL_SIZE     6
`define MEMSHIRE_ATOMIC_STATE_SIZE     4
`define MEMSHIRE_ADDR_MASK_SIZE        12
`define MEMSHIRE_MC_EN_SIZE            2
`define MEMSHIRE_DDRC_SCRUB_SIZE       36
`define MEM_MRR_DATA_WIDTH             16
`define MEMSHIRE_INT_ENABLE_SIZE       16
`define MEMSHIRE_DEBUG_MASK_SIZE       64
`define MEMSHIRE_ESR_SCRATCH_SIZE      64
`define MEMSHIRE_DDRC_PERF_OP_SIZE     24

// clock gater disable : a value of 0 means by default the clock is enabled
`define ESR_MS_CLK_GATE_CTL_RESET_VAL    1'b0

// Manual ESRs
`define MEMSHIRE_ESR_ATOMIC_SM_START_LSB  0
`define MEMSHIRE_DESR_U0_MRR_VALID_BIT    0
`define MEMSHIRE_DESR_U1_MRR_VALID_BIT    1

`define MEMSHIRE_DESR_U1_CSYSACK_HI_BIT   16
`define MEMSHIRE_DESR_U1_CACTIVE_HI_BIT   15

`define MEMSHIRE_DESR_U0_CSYSACK_HI_BIT   13
`define MEMSHIRE_DESR_U0_CACTIVE_HI_BIT   12

`define MEMSHIRE_DESR_U1_CSYSACK_LO_BIT   10
`define MEMSHIRE_DESR_U1_CACTIVE_LO_BIT    9

`define MEMSHIRE_DESR_U0_CSYSACK_LO_BIT    7
`define MEMSHIRE_DESR_U0_CACTIVE_LO_BIT    6

`define MEMSHIRE_DESR_U1_CSYSACK_DDRC_BIT  4
`define MEMSHIRE_DESR_U1_CACTIVE_DDRC_BIT  3

`define MEMSHIRE_DESR_U0_CSYSACK_DDRC_BIT  1
`define MEMSHIRE_DESR_U0_CACTIVE_DDRC_BIT  0

// width of perf ops
`define  MEMSHIRE_PERF_OP_SIGS_WIDTH   40
`define  MEMSHIRE_PERF_OPS_WIDTH       18
`define  MEMSHIRE_SM_DATA_TRACE_WIDTH 191

// status monitor defines
`define MEMSHIRE_DEBUG_SM_U1_RAQ_WCOUNT_1_BIT     (126-64)
`define MEMSHIRE_DEBUG_SM_U1_WAQ_WCOUNT_1_BIT     (125-64)
`define MEMSHIRE_DEBUG_SM_U1_RAQ_WCOUNT_0_BIT     (124-64)
`define MEMSHIRE_DEBUG_SM_U1_WAQ_WCOUNT_0_BIT     (123-64)
`define MEMSHIRE_DEBUG_SM_U1_WRECC_CREDIT_CNT_BIT (122-64)
`define MEMSHIRE_DEBUG_SM_U1_WR_CREDIT_CNT_BIT    (121-64)
`define MEMSHIRE_DEBUG_SM_U1_HPR_CREDIT_CNT_BIT   (120-64)
`define MEMSHIRE_DEBUG_SM_U1_LPR_CREDIT_CNT_BIT   (119-64)
`define MEMSHIRE_DEBUG_SM_U0_RAQ_WCOUNT_1_BIT     (118-64)
`define MEMSHIRE_DEBUG_SM_U0_WAQ_WCOUNT_1_BIT     (117-64)
`define MEMSHIRE_DEBUG_SM_U0_RAQ_WCOUNT_0_BIT     (116-64)
`define MEMSHIRE_DEBUG_SM_U0_WAQ_WCOUNT_0_BIT     (115-64)
`define MEMSHIRE_DEBUG_SM_U0_WRECC_CREDIT_CNT_BIT (114-64)
`define MEMSHIRE_DEBUG_SM_U0_WR_CREDIT_CNT_BIT    (113-64)
`define MEMSHIRE_DEBUG_SM_U0_HPR_CREDIT_CNT_BIT   (112-64)
`define MEMSHIRE_DEBUG_SM_U0_LPR_CREDIT_CNT_BIT   (111-64)

`define MEMSHIRE_DEBUG_SM_U1_RAQ_WCOUNT_1_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U1_WAQ_WCOUNT_1_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U1_RAQ_WCOUNT_0_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U1_WAQ_WCOUNT_0_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U1_WRECC_CREDIT_CNT_WIDTH 7
`define MEMSHIRE_DEBUG_SM_U1_WR_CREDIT_CNT_WIDTH    7
`define MEMSHIRE_DEBUG_SM_U1_HPR_CREDIT_CNT_WIDTH   7
`define MEMSHIRE_DEBUG_SM_U1_LPR_CREDIT_CNT_WIDTH   7
`define MEMSHIRE_DEBUG_SM_U0_RAQ_WCOUNT_1_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U0_WAQ_WCOUNT_1_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U0_RAQ_WCOUNT_0_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U0_WAQ_WCOUNT_0_WIDTH     3
`define MEMSHIRE_DEBUG_SM_U0_WRECC_CREDIT_CNT_WIDTH 7
`define MEMSHIRE_DEBUG_SM_U0_WR_CREDIT_CNT_WIDTH    7
`define MEMSHIRE_DEBUG_SM_U0_HPR_CREDIT_CNT_WIDTH   7
`define MEMSHIRE_DEBUG_SM_U0_LPR_CREDIT_CNT_WIDTH   7

`define MEMSHIRE_MEM_DEBUG_SIGS_PC_WIDTH   22
`define MEMSHIRE_MEM_DEBUG_SIGS_1_WIDTH   (53+`DDR_BANKS_SIZE +`DDR_BANKS_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_CRD_CNT_SIZE +`DDR_DBG_DFI_IE_SIZE +`DDR_DBG_DFI_IE_SIZE +`DDR_DFI_CS_SIZE +`DDR_DFI_CS_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE +`DDR_XAQ_WCOUNT_SIZE)
`define MEMSHIRE_MEM_DEBUG_SIGS_WIDTH    (`MEMSHIRE_PERF_OP_SIGS_WIDTH + `MEMSHIRE_MEM_DEBUG_SIGS_1_WIDTH + `MEMSHIRE_MEM_DEBUG_SIGS_PC_WIDTH)

// interrupt 
`define MEMSHIRE_INT_MASKABLE_SIZE     16
`define MEMSHIRE_NOC_ERR_WIDTH         13
`define MEMSHIRE_INT_STATUS_PAD       (32 - `MEMSHIRE_INT_MASKABLE_SIZE)
`define MEMSHIRE_INT_STATUS_SIZE      (32 + `MEMSHIRE_NOC_ERR_WIDTH)

`define MEMSHIRE_SET_DFI0_ERR_BIT      2
`define MEMSHIRE_CLR_DFI0_ERR_BIT      1
`define MEMSHIRE_SET_DFI1_ERR_BIT     10
`define MEMSHIRE_CLR_DFI1_ERR_BIT      9
`define MEMSHIRE_ESR_INT_STATUS_SIZE  16
`define MEMSHIRE_DFI0_ERR_INFO_BITS   7:4
`define MEMSHIRE_DFI1_ERR_INFO_BITS   15:12

// Perfmon
`define MEMSHIRE_PERFMON_QUAL_MCOP_BANK_BITS              27:20
`define MEMSHIRE_PERFMON_QUAL_MCOP_ECC_BITS               19:17
`define MEMSHIRE_PERFMON_QUAL_MCOP_MC1_SEL_BIT            16
`define MEMSHIRE_PERFMON_QUAL_MCOP_MC0_SEL_BIT            15
`define MEMSHIRE_PERFMON_QUAL_MCOP_CMD_BITS               14:0

`define MEMSHIRE_PERFMON_QUAL_MCDATA_MC1_SEL_BIT          3
`define MEMSHIRE_PERFMON_QUAL_MCDATA_MC0_SEL_BIT          2
`define MEMSHIRE_PERFMON_QUAL_MCDATA_WR_BIT               1
`define MEMSHIRE_PERFMON_QUAL_MCDATA_RD_BIT               0

`define MEMSHIRE_PERFMON_QUAL_MCMISC_WRCOMB_BIT           7
`define MEMSHIRE_PERFMON_QUAL_MCMISC_IEBLK_BIT            6
`define MEMSHIRE_PERFMON_QUAL_MCMISC_WAR_BIT              5
`define MEMSHIRE_PERFMON_QUAL_MCMISC_RAW_BIT              4
`define MEMSHIRE_PERFMON_QUAL_MCMISC_WAW_BIT              3
`define MEMSHIRE_PERFMON_QUAL_MCMISC_RDWR_BIT             2
`define MEMSHIRE_PERFMON_QUAL_MCMISC_MC1_SEL_BIT          1
`define MEMSHIRE_PERFMON_QUAL_MCMISC_MC0_SEL_BIT          0

`define MEMSHIRE_PERFMON_QUAL_MESHCNT_ADDR_BASE_BITS      63:38
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_ADDR_MASK_SIZE_BITS 37:33
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_ADDR_BIT            32

`define MEMSHIRE_PERFMON_QUAL_MESHCNT_SRC_EXACT_BITS      28:20
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_SRC_EXACT_EN_BIT    31

`define MEMSHIRE_PERFMON_QUAL_MESHCNT_SRC_MINSHIRE_BIT    8
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_SRC_PSHIRE_BIT      7
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_SRC_IOSHIRE_BIT     6
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_QOSHI_BIT           5
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_QOSLO_BIT           4
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_MC1_BIT             3
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_MC0_BIT             2
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_WR_BIT              1
`define MEMSHIRE_PERFMON_QUAL_MESHCNT_RD_BIT              0

`define MEMSHIRE_PERFMON_QUAL_MESHOUT_WR_BIT              1
`define MEMSHIRE_PERFMON_QUAL_MESHOUT_RD_BIT              0

`define MEMSHIRE_PERFMON_QUAL2_MIN_BITS                   11:0
`define MEMSHIRE_PERFMON_QUAL2_MAX_BITS                   27:16

`define MEMSHIRE_PERFMON_MODE_OP_BITS                     7:6
`define MEMSHIRE_PERFMON_MODE_TYPE_BITS                   5:0
   
// corresponds to noc v2.2
`define MEMSHIRE_PERFMON_AXID_SOURCE_BITS      18:10      
`define MEMSHIRE_PERFMON_AXID_SOURCE_IOSHIRE   9'h003     
`define MEMSHIRE_PERFMON_AXID_SOURCE_PSHIRE    9'h00d     
`define MEMSHIRE_PERFORM_AXID_SOURCE_SIZE      9

// Atomics
// modify shire_cache atomic model - memshire atomic response width is only half as much
`define MEMSHIRE_ATOMIC_ADDR_OFFSET_SIZE        (`SC_LINE_OFFSET_ID_SIZE-1)
`define MEMSHIRE_ATOMIC_ADDR_OFFSET_SEL_RANGE   `SC_ATOMIC_ADDR_OFFSET_LSB +: `MEMSHIRE_ATOMIC_ADDR_OFFSET_SIZE


// PLL controls for DV - default to RTL (except in simulation)
`ifdef ET_SIMULATION
   `ifndef DV_MEMSHIRE_USE_PLL_BEH
      `define DV_MEMSHIRE_USE_PLL_BEH 1
   `endif
   `ifndef DV_MEMSHIRE_PLL_FAST_START
      `define DV_MEMSHIRE_PLL_FAST_START 1
   `endif
`else
   `ifndef DV_MEMSHIRE_USE_PLL_BEH
      `define DV_MEMSHIRE_USE_PLL_BEH 0
   `endif
   `ifndef DV_MEMSHIRE_PLL_FAST_START
      `define DV_MEMSHIRE_PLL_FAST_START 0
   `endif
`endif

// DFT(note): bootlegged from shire_defines.vh, will need to clean up a bit later
// DFT
// ----------------------------------------
//????(ejt) Review this with Vinat`define ET_DFT_INSERTION                          0  // set this to 1 during and after dft insertion

// memshire_dft_hub() defines
`define MEMSHIRE_DFT_BC_CTL_WIDTH                     `SOCTOP_DFT_BC_CTL_WIDTH  // dft broadcast control bits to all shires
`define MEMSHIRE_DFT_SCANIN_SCANOUT_TOP_WIDTH         44                     // width of scan_in/out buses flowing [memshire_x_top(memshire_dft_hub)] <--> soc_top

// (ejt) review with Vinay, does he need these??
`define MEMSHIRE_DFT_MODE_WIDTH                        4  // dft test mode width
`define MEMSHIRE_DFT_CNTL_WIDTH                       10  // dft spare control bits
`define MEMSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH              8  // extra scan dump control bits in addition to wei/rei
                                                 

// ECO port width defines
`define ET_ECO_MEMSHIRE_INPUT_WIDTH                16
`define ET_ECO_MEMSHIRE_OUTPUT_WIDTH               16


`endif //  `ifndef _MEMSHIRE_DEFINES_


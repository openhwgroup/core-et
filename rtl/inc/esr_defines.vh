// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _ESR_DEFINES_VH_
`define _ESR_DEFINES_VH_

// defines for address widths (not including PP bits)

`define ESR_NEIGH_APB_AD_WIDTH       13
`define ESR_TBOX_APB_AD_WIDTH        13
`define ESR_CACHE_BANK_APB_AD_WIDTH  10
`define ESR_SHIRE_OTHER_APB_AD_WIDTH 14
`define ESR_IOSHIRE_RVTIME_APB_AD_WIDTH 14
`define ESR_RBOX_APB_AD_WIDTH        14
`define ESR_MEMSHIRE_APB_AD_WIDTH    10
`define ESR_DDRC_APB_AD_WIDTH        10
`define ESR_SPIO_APB_AD_WIDTH         2
`define ESR_IOSHIRE_APB_AD_WIDTH      3

// defines for address widths (including PP bits)

`define ESR_NEIGH_APB_AD_PP_WIDTH       15
`define ESR_TBOX_APB_AD_PP_WIDTH        15
`define ESR_CACHE_BANK_APB_AD_PP_WIDTH  12
`define ESR_SHIRE_OTHER_APB_AD_PP_WIDTH 16
`define ESR_RBOX_APB_AD_PP_WIDTH        16
`define ESR_NEIGH_MIN_APB_AD_PP_WIDTH   17
`define ESR_MEMSHIRE_APB_AD_PP_WIDTH    12
`define ESR_DDRC_APB_AD_PP_WIDTH        12
`define ESR_SPIO_APB_AD_PP_WIDTH         4
`define ESR_IOSHIRE_APB_AD_PP_WIDTH      5

`define ESR_APB_AD_PP_WIDTH `MAX(`ESR_NEIGH_APB_AD_PP_WIDTH, \
                                `MAX(`ESR_CACHE_BANK_APB_AD_PP_WIDTH,  \
                                    `MAX(`ESR_TBOX_APB_AD_PP_WIDTH, \
                                        `MAX(`ESR_SHIRE_OTHER_APB_AD_PP_WIDTH, \
                                            `MAX(`ESR_RBOX_APB_AD_PP_WIDTH, \
                                                 `ESR_NEIGH_MIN_APB_AD_PP_WIDTH) ) ) ) )

`define ESR_APB_D_WIDTH 64
`define ESR_APB_AD_WIDTH `MAX( `ESR_NEIGH_APB_AD_WIDTH, \
                            `MAX( `ESR_CACHE_BANK_APB_AD_WIDTH,  \
                                `MAX(`ESR_TBOX_APB_AD_WIDTH, \
                                    `MAX(`ESR_SHIRE_OTHER_APB_AD_WIDTH, \
                                        `ESR_RBOX_APB_AD_WIDTH ) ) ) )

`define ESR_MINION_FEATURE_SZ $bits(esr_minion_features_t)
`ifdef GFX_ENABLED
`define ESR_MINION_FEATURE_RESET_VAL '0
`else
`define ESR_MINION_FEATURE_RESET_VAL `ZX(`ESR_MINION_FEATURE_SZ,1'b1)
`endif



// shire_cache_bank reset vals

// `define ESR_SHIRE_CONFIG_CONST_RESET_VAL '{tbox3_id: 2'h3, tbox2_id: 2'h2, tbox1_id: 2'h1, tbox0_id: 2'h0, rbox_en: 1'h1, tbox_en: 4'h5, neigh_en: 4'h0, cache_en: 1'h0, shire_id: 8'h0, default: '0}
`define ESR_SHIRE_CONFIG_CONST_RESET_VAL 26'h392a000
`define ESR_SHIRE_CONFIG_RESET_VAL     (`ESR_SHIRE_CONFIG_CONST_RESET_VAL | shire_id_reset_val)

// Tbox enabled
`define ESR_TEXTURE_CTL_RESET_VAL   16'h0005

`define ESR_SC_REQQ_CTL_RESET_VAL             '{esr_sc_cbuf_enable: '1, esr_sc_remote_l3_enable: '1, esr_sc_remote_scp_enable: '1, esr_sc_num_l3_reqq_entries: NUM_L3_REQQ_ENTRIES_DEFAULT[`SC_REQQ_ID_SIZE-1:0], default: '0}
`define ESR_SC_PIPE_CTL_RESET_VAL             '{esr_sc_idx_cop_sm_ctl_user_en: '1, esr_sc_ram_delay: RAM_DELAY_DEFAULT[`SC_RAM_DELAY_SIZE-1:0], esr_sc_l2_rbuf_enable: '1, esr_sc_scp_rbuf_enable: '1, esr_sc_zero_state_enable: '1, esr_sc_cbuf_entry_enable: '1, default: '0}
`define ESR_SC_L3_SHIRE_SWIZZLE_CTL_RESET_VAL '{                                              \
   esr_sc_sub_bank_sel_b2 : ESR_SWZ_SUB_BANK_SEL_B2_DEFAULT[`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_sub_bank_sel_b1 : ESR_SWZ_SUB_BANK_SEL_B1_DEFAULT[`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_sub_bank_sel_b0 : ESR_SWZ_SUB_BANK_SEL_B0_DEFAULT[`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_bank_sel_b2     : ESR_SWZ_BANK_SEL_B2_DEFAULT    [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_bank_sel_b1     : ESR_SWZ_BANK_SEL_B1_DEFAULT    [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_bank_sel_b0     : ESR_SWZ_BANK_SEL_B0_DEFAULT    [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b5    : ESR_SWZ_SHIRE_SEL_B5_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b4    : ESR_SWZ_SHIRE_SEL_B4_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b3    : ESR_SWZ_SHIRE_SEL_B3_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b2    : ESR_SWZ_SHIRE_SEL_B2_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b1    : ESR_SWZ_SHIRE_SEL_B1_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   esr_sc_shire_sel_b0    : ESR_SWZ_SHIRE_SEL_B0_DEFAULT   [`SC_L3_SWIZZLE_BIT_SEL_SIZE-1:0], \
   default: '0}
`define ESR_SC_L2_CACHE_CTL_RESET_VAL          '{                   \
   set_base: ESR_SC_L2_SET_BASE_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   set_size: ESR_SC_L2_SET_SIZE_DEFAULT[`SC_ESR_SET_SIZE_SIZE-1:0], \
   set_mask: ESR_SC_L2_SET_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   tag_mask: ESR_SC_L2_TAG_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   default: '0}                          
`define ESR_SC_L3_CACHE_CTL_RESET_VAL          '{                   \
   set_base: ESR_SC_L3_SET_BASE_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   set_size: ESR_SC_L3_SET_SIZE_DEFAULT[`SC_ESR_SET_SIZE_SIZE-1:0], \
   set_mask: ESR_SC_L3_SET_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   tag_mask: ESR_SC_L3_TAG_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   default: '0}
`define ESR_SC_SCP_CACHE_CTL_RESET_VAL         '{                    \
   set_base: ESR_SC_SCP_SET_BASE_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   set_size: ESR_SC_SCP_SET_SIZE_DEFAULT[`SC_ESR_SET_SIZE_SIZE-1:0], \
   set_mask: ESR_SC_SCP_SET_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
   tag_mask: ESR_SC_SCP_TAG_MASK_DEFAULT[`SC_ESR_SET_BASE_SIZE-1:0], \
      default: '0}

`define ESR_SC_ERR_LOG_CTL_RESET_VAL          '{esr_sc_err_interrupt_enable: 8'hFE, esr_sc_err_rsp_enable: 1'b1, default: '0}
`define ESR_SC_TRACE_CTL_RESET_VAL            '{default: '0}
`define ESR_SHIRE_CACHE_RAM_CFG1_RESET_VAL     '{ \
    sc_mbt_wa      : 3'b111 ,                     \
    sc_mbt_ra      : 2'b01  ,                     \
    sc_mbt_rm      : 4'b0000,                     \
    sc_mbt_rme     : 1'b0,                        \
    sc_mbs_wa      : 3'b110 ,                     \
    sc_mbs_ra      : 2'b10  ,                     \
    sc_mbs_rm      : 4'b0000,                     \
    sc_mbs_rme     : 1'b0,                        \
    default : '0}
`define ESR_SHIRE_CACHE_RAM_CFG2_RESET_VAL     '{ \
    sc_mbd_wa      : 3'b111 ,                     \
    sc_mbd_ra      : 2'b01  ,                     \
    sc_mbd_rm      : 4'b0000,                     \
    sc_mbd_rme     : 1'b0,                        \
    default : '0}
`define ESR_SHIRE_CACHE_RAM_CFG3_RESET_VAL     '{ \
    sc_mbb_wa      : 3'b110 ,                     \
    sc_mbb_ra      : 2'b01  ,                     \
    sc_mbb_rm      : 4'b0001,                     \
    sc_mbb_rme     : 1'b1,                        \
    sc_mbq_wa      : 3'b110 ,                     \
    sc_mbq_ra      : 2'b01  ,                     \
    sc_mbq_rm      : 4'b0001,                     \
    sc_mbq_rme     : 1'b1,                        \
    default : '0}
`define ESR_SHIRE_CACHE_RAM_CFG4_RESET_VAL     '{ \
    sc_mbr_1p_wa   : 3'b110 ,                     \
    sc_mbr_1p_ra   : 2'b10  ,                     \
    sc_mbr_1p_rm   : 4'b0000,                     \
    sc_mbr_1p_rme  : 1'b0,                        \
    sc_mbr_2p_wa   : 3'b110 ,                     \
    sc_mbr_2p_ra   : 2'b01  ,                     \
    sc_mbr_2p_rm   : 4'b0001,                     \
    sc_mbr_2p_rme  : 1'b1,                        \
    sc_mbi_wa      : 3'b111 ,                     \
    sc_mbi_ra      : 2'b01  ,                     \
    sc_mbi_rm      : 4'b0000,                     \
    sc_mbi_rme     : 1'b0,                        \
    default : '0}

// ECO registers
`define ESR_SCB_ECO_WIDTH   8
`define ESR_SCW_ECO_WIDTH   8


// mem shire reset vals
`define ESR_MS_MEM_CTL_RESET_VAL '{                               \
   esr_ms_mc_en                 : `MEMSHIRE_MC_EN_SIZE'd3,        \
   esr_ms_addr_mask             : `MEMSHIRE_ADDR_MASK_SIZE'h07f,  \
   esr_ms_addr_debug_mc_bit_sel : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd6, \
   esr_ms_addr_mc_bit_sel       : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd9, \
   esr_ms_addr_remove_bit_sel3  : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd9, \
   esr_ms_addr_remove_bit_sel2  : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd8, \
   esr_ms_addr_remove_bit_sel1  : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd7, \
   esr_ms_addr_remove_bit_sel0  : `MEMSHIRE_ADDR_BIT_SEL_SIZE'd6, \
   default: '0}

`define ESR_DDRC_MAIN_CTL_RESET_VAL        '{esr_ddrc_write_auto_precharge_lo : 1'b0, esr_ddrc_write_auto_precharge_hi : 1'b0, esr_ddrc_read_auto_precharge_lo  : 1'b0, esr_ddrc_read_auto_precharge_hi  : 1'b0,  default: '0}
`define ESR_DDRC_CLOCK_CTL_RESET_VAL       '{esr_ddrc_u0_csysreq_lo : 1'b1, esr_ddrc_u0_csysreq_hi : 1'b1, esr_ddrc_u1_csysreq_lo : 1'b1, esr_ddrc_u1_csysreq_hi : 1'b1, esr_ddrc_u1_csysreq_ddrc : 1'b1, esr_ddrc_u0_csysreq_ddrc : 1'b1, default: '0}
`define ESR_DDRC_RESET_CTL_RESET_VAL       '{esr_ddrc_subsystem_reset : 1'b1, esr_ddrc_apb_reset : 1'b1, esr_ddrc_pub_reset : 1'b1, esr_ddrc_power_ok : 1'b0, esr_ddrc_pll_clk_sel : 1'b0, default: '0}
`define ESR_DDRC_INT_CRITICAL_EN_RESET_VAL `MEMSHIRE_INT_ENABLE_SIZE'h0
`define ESR_DDRC_INT_NORMAL_EN_RESET_VAL   `MEMSHIRE_INT_ENABLE_SIZE'h0
`define ESR_DDRC_TRACE_CTL_RESET_VAL       '{esr_ddrc_trace_en : 1'b0, esr_ddrc_perf_op_sigs_mask : `MEMSHIRE_DDRC_PERF_OP_SIZE'h0, default: '0}
  
// default minion boot address
`define ESR_NEIGH_MINION_BOOT_RESET_VAL `VA_SIZE'h02008000

// Icache error logging
// By default, disable SBE interrupts and enable DBE and ECC error counter overflow interrupts 
// Level 2 error responses are disabled by default
`define ESR_ICACHE_ERR_LOG_CTL_RESET_VAL '{err_interrupt_enable: 3'b110, default: '0}
`define ESR_ICACHE_SBE_CNT_SIZE          8
`define ESR_ICACHE_DBE_CNT_SIZE          3

// shire_coop_mode
`define ESR_SHIRE_COOP_MODE_RESET_VAL 1'b0

// clint timer
`define ESR_SHIRE_MTIME_RESET_VAL 64'h0
`define ESR_SHIRE_MTIMECMP_RESET_VAL {64{1'b1}}
`define ESR_SHIRE_TIME_CONFIG_RESET_VAL 8'h14
`define ESR_SHIRE_MTIME_LOCAL_TARGET_RESET_VAL {`THREADS_PER_N{1'b1}}

// DEBUG
`define HACTRL_RESETHALT_SIZE (`MIN_PER_N*`CORE_NR_THREADS)
`define HACTRL_RESETHALT_START 32
`define HACTRL_RESETHALT_RANGE (`HACTRL_RESETHALT_START+:`HACTRL_RESETHALT_SIZE)
`define ESR_DMCTRL_RESET_VAL '0


// MPROT
// -----

`define MPROT_RESET_VAL '0

// IO ACCESS MODE
`define MPROT_IO_ACCESS_MODE_U 2'h0
`define MPROT_IO_ACCESS_MODE_S 2'h1
`define MPROT_IO_ACCESS_MODE_F 2'h2 // Access always forbidden
`define MPROT_IO_ACCESS_MODE_M 2'h3

// DRAM SIZE
`define MPROT_DRAM_SIZE_8G  2'h0
`define MPROT_DRAM_SIZE_16G 2'h1
`define MPROT_DRAM_SIZE_24G 2'h2
`define MPROT_DRAM_SIZE_32G 2'h3

// OTHERS
`define THREAD0_DISABLE_RESET_VAL 8'hfe

`endif



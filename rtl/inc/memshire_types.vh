// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// memshire_types.vh
//
// enums
// structs
 
// ESR structures
// --------------------------------------------------------------------------

// Struct for memshire esr ctl
typedef struct packed {
   logic [`MEMSHIRE_MC_EN_SIZE-1:0]         esr_ms_mc_en;
   logic [`MEMSHIRE_ADDR_MASK_SIZE -1:0]    esr_ms_addr_mask;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_debug_mc_bit_sel;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_mc_bit_sel;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_remove_bit_sel3;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_remove_bit_sel2;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_remove_bit_sel1;
   logic [`MEMSHIRE_ADDR_BIT_SEL_SIZE -1:0] esr_ms_addr_remove_bit_sel0;
   logic                                    esr_ms_clk_disable_debug;

   // manual esr control
   logic                                    wr_en_ms_atomic_sm_ctl;
   logic [`SC_DW_DATA_SIZE-1:0]             wr_data;
} memshire_esr_ctl_t;

// Struct for ddrc esr ctl
typedef struct packed {

   logic                                    esr_ddrc_pll_clk_sel;
   logic                                    esr_ddrc_power_ok;
   logic                                    esr_ddrc_pub_reset;
   logic                                    esr_ddrc_apb_reset;
   logic                                    esr_ddrc_subsystem_reset;

   logic                                    esr_ddrc_u1_csysreq_hi;
   logic                                    esr_ddrc_u1_csysreq_lo;
   logic                                    esr_ddrc_u0_csysreq_hi;
   logic                                    esr_ddrc_u0_csysreq_lo;
   logic                                    esr_ddrc_u1_csysreq_ddrc;
   logic                                    esr_ddrc_u0_csysreq_ddrc;

   logic                                    esr_ddrc_read_auto_precharge_hi;
   logic                                    esr_ddrc_read_auto_precharge_lo;
   logic                                    esr_ddrc_write_auto_precharge_hi;
   logic                                    esr_ddrc_write_auto_precharge_lo;

   logic [`MEMSHIRE_DDRC_SCRUB_SIZE-1:0]    esr_ddrc_sbr_address_start_mask;
   logic [`MEMSHIRE_DDRC_SCRUB_SIZE-1:0]    esr_ddrc_sbr_address_range_mask;

   logic [`MEMSHIRE_INT_ENABLE_SIZE-1:0]    esr_ddrc_int_critical_en;
   logic [`MEMSHIRE_INT_ENABLE_SIZE-1:0]    esr_ddrc_int_normal_en;

   logic [`MEMSHIRE_DEBUG_MASK_SIZE-1:0]    esr_ddrc_debug_sigs_mask0;
   logic [`MEMSHIRE_DEBUG_MASK_SIZE-1:0]    esr_ddrc_debug_sigs_mask1;
   logic [`MEMSHIRE_ESR_SCRATCH_SIZE-1:0]   esr_ddrc_scratch;

   logic [`MEMSHIRE_DDRC_PERF_OP_SIZE-1:0]  esr_ddrc_perf_op_sigs_mask;
   logic                                    esr_ddrc_trace_en;

   // manual esr control
   logic                                    wr_en_ddrc_mrr_status;
   logic                                    wr_en_ddrc_err_int_log;
   logic                                    wr_en_perfmon_ctl_status; 
   logic                                    wr_en_perfmon_cyc_cntr;   
   logic                                    wr_en_perfmon_p0_cntr;    
   logic                                    wr_en_perfmon_p1_cntr;    
   logic                                    wr_en_perfmon_p0_qual;    
   logic                                    wr_en_perfmon_p1_qual;    
   logic                                    wr_en_perfmon_p0_qual2;    
   logic                                    wr_en_perfmon_p1_qual2;    
   logic [`SC_DW_DATA_SIZE-1:0]             wr_data;
} ddrc_esr_ctl_t;



// Struct for memshire esr status
typedef struct packed {
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] atomic_sm;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] mem_revision_id;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] mem_status;
} memshire_esr_status_t;


// Struct for ddrc esr status
typedef struct packed {
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_clock_ctl;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_u0_mrr_data;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_u1_mrr_data;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_mrr_status;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_int_status;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] ddrc_err_int_log;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_ctl_status;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_cyc_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_qual;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_qual;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_qual2;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_qual2;
} ddrc_esr_status_t;

// Struct for perfmon esr status
typedef struct packed {
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_ctl_status;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_cyc_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_cntr;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_qual;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_qual;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p0_qual2;
   logic [`MEMSHIRE_ESR_STATUS_SIZE-1:0] perfmon_p1_qual2;
} ms_perfmon_status_t;



// functions

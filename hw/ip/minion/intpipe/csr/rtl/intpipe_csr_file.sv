// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_file — Main CSR register file for the integer pipeline.
//
// Mechanically translated from etcore-soc intpipe_csr_file.v.
// All signal names, timing, and architecture preserved exactly.

module intpipe_csr_file
  import minion_pkg::*;
  import intpipe_csr_pkg::*;
  #(
    parameter bit VpuEn = 1'b1
  )
  (
   // System signals
   input logic                                        clk_i,
   input logic                                        rst_ni,    // active-low, was reset_c
   input logic                                        rst_w_ni,  // active-low, was reset_w
   input logic                                        rst_d_ni,  // active-low, was reset_d
   /* verilator lint_off UNUSEDSIGNAL */  // The preserved CSR-file interface keeps DFT/test and wide packed inputs even though only the original functional subset is consumed here.
   input logic                                        test_en,
    // DFT signals
   input  logic                                       dft__reset_byp,     //Reset byp
   input  logic                                       dft__reset,         //DFT mode reset
    // Interrupts and thread id
   input logic [NrThreads-1:0]                        int_mieco,
   input logic [NrThreads-1:0]                        int_mtip,
   input logic [NrThreads-1:0]                        int_msip,
   input logic [NrThreads-1:0]                        int_meip,
   input logic [NrThreads-1:0]                        int_seip,
`ifdef CSR_U_INTERRUPTS
   input logic [NrThreads-1:0]                        int_ueip,
`endif
   input logic [(NrThreads*FccPerMin)-1:0]            int_fcc,
   input logic [$clog2(256)-1:0]                      shire_id,       // NUM_SHIRE_IDS=256
   input logic [$clog2(32)-1:0]                       shire_min_id,   // MIN_PER_S=32
   input logic                                        wb_thread_id,
   input logic                                        mem_thread_id,
    // FrontEnd, Dcache and VPU control
   output logic [NrThreads-1:0]                       fe_ctrl_stall,
   output                                             core_dcache_ctrl dcache_ctrl,
   input                                              dcache_core_ctrl dcache_ctrl_resp,
   output                                             core_vpu_ctrl vpu_ctrl,
   input  logic                                       pending_scoreboard_t0,
   input  logic                                       vpu_tfma_enabled,
   input  logic                                       vpu_tfma_tenb_working,
   input  logic                                       vpu_tquant_enabled,
   input  logic                                       vpu_treduce_enabled,
   output logic                                       icache_invalidate,
   output logic                                       tlb_invalidate,

    // Read/Write port
   input logic [CsrRegAddrSize-1:0]                   io_rw_mem_addr,
   input csr_cmd_t                                    io_rw_wb_cmd,
   input csr_cmd_t                                    io_rw_mem_cmd,
   input logic [2:0]                                  io_rw_wb_inst, //bits 22:20 of the instruction, to be used to decode system insn
   output logic [XregSize-1:0]                        io_rw_wb_rdata,
   input logic [XregSize-1:0]                         io_rw_mem_wdata,
   input logic [XregSize-1:0]                         io_rw_wb_wdata,
   input logic [XregAddrSize-1:0]                     io_rw_wb_waddr,
   input logic [XregSize-1:0]                         wb_x31_reg,

    // control signals to intpipe
   output logic [NrThreads-1:0]                       io_csr_stall,
   output logic                                       io_csr_xcpt,
   output logic                                       io_eret,
   output logic                                       io_replay,
   output logic [NrThreads-1:0]                       io_resume,
   output logic [NrThreads-1:0]                       io_singleStep,
   output logic                                       io_excl_mode,             // minion in exclusive mode
   output logic                                       io_excl_mode_sel,         // thread selection for exclusive mode
   output logic                                       io_excl_mode_transition,  // transitioning in or out of exclusive mode

    // Status
   output intpipe_status_t [NrThreads-1:0]            io_status,
   output logic [NrThreads-1:0][1:0]                  io_prv,
   output logic [NrThreads-1:0]                       io_debug,
    // SATP
   output csr_satp_t                                  io_satp,
   output logic                                       io_satp_en,
   output csr_matp_t                                  io_matp,
   output logic                                       io_matp_en,
    // L2 fills (used for SEND)
   input logic                                        l2_resp_valid,
   input logic                                        l2_resp_ready,
   input                                              et_link_minion_rsp_info_t l2_resp,
    // Evec
   output logic [PcSizeExt-1:0]                       io_evec,
   output logic [PcSizeExt-1:0]                       io_evec_resume,
    // Events
   input logic [CsrNrEventsExt-1:0]                   io_events_ext,
    // Exception and retire from core
   input logic                                        io_exception,
   input logic                                        io_retire,
   input                                              csr_cause_t io_cause,
   input logic [PcSizeExt-1:0]                        io_pc,
   input logic [VaSizeExt-1:0]                        io_badaddr,
   input logic [InstSize-1:0]                         io_inst_bits,
   input logic                                        io_inst_rvc,
   input logic [NrThreads-1:0]                        io_bad_redirect,
   input logic [NrThreads-1:0]                        io_redirect,
    // VPU
   output logic [2:0]                                 io_fcsr_rm_thread0,
   output logic [2:0]                                 io_fcsr_rm_thread1,
   input logic                                        io_fcsr_flags_valid,
   input logic [5:0]                                  io_fcsr_flags_bits,
   input logic                                        io_fcsr_thread_id,
    // Interrupts
   output logic [NrThreads-1:0]                       io_interrupt,
   output logic [NrThreads-1:0][4:0]                  io_interrupt_cause,
    // bus error flag
   input logic [NrThreads-1:0]                        dcache_bus_error,
   input logic [NrThreads-1:0][PaSize-1:0]            dcache_bus_error_addr,
   `ifdef DCACHE_REPORT_PC
   input logic [NrThreads-1:0][PcSizeExt-1:0]         dcache_bus_error_pc,
   `endif

    // Breakpoint and debug
   output minion_bp_ctrl_t                            [NrThreads-1:0] bp_control,
   output logic [NrThreads-1:0][VaSize-1:0]           bp_address,
   input  logic [NrThreads-1:0]                       debug_timing,
   output logic [NrThreads-1:0]                       update_ddata0,
   output logic [63:0]                                ddata0_wdata,
   input logic                                        bp_progress_save,
   input logic [NrThreads-1:0][63:0]                  read_ddata0,
   input logic [NrThreads-1:0]                        halt,
   input logic [NrThreads-1:0]                        resume,
   input logic [NrThreads-1:0]                        ex_progbuf,
   output logic [NrThreads-1:0]                       busy,
   output logic [NrThreads-1:0]                       debug_out_exception,
   output logic                                       xcpt_traps_to_debug,

    // Fast Local Barrier request interface with the neigh
   output logic                                       flb_neigh_req_valid,
   output logic [CsrFlBarrierSize-1:0]                flb_neigh_req_data,
    // Fast Local Barrier response interface with the neigh
   input logic                                        flb_neigh_resp_valid,
   input logic                                        flb_neigh_resp_data,
    // Fast Local Barrier interface with the RF write
   input logic                                        flb_rf_wen_ready,
   output logic                                       flb_rf_wen_valid,
   output logic                                       flb_rf_wen_valid_early,
   output logic                                       flb_rf_wen_thread_id,
   output logic [XregAddrSize-1:0]                    flb_rf_wen_addr,
   output logic [XregSize-1:0]                        flb_rf_wen_data,
    // Fast Local Barrier interface with the INT scoreboard
   output logic                                       flb_scoreboard_valid,
   output                                             minion_reg_dest_t flb_scoreboard_addr,

   output logic [1:0]                                 TE_prv,
   output logic                                       TE_exception,
   output logic [VaSizeExt-1:0]                       TE_tval,
   output csr_cause_t                                 TE_wb_reg_cause_ie,
   input logic                                        te_thread_sel,
   // m code instruction match
   output minst_mask_t [NrThreads-1:0]                io_minstmask,
   output logic [NrThreads-1:0] [InstSize-1:0]        io_minstmatch,
   // ESR configuration
   input  logic                                       esr_shire_coop_mode,
   input esr_minion_features_t                        esr_features,
   /* verilator lint_on UNUSEDSIGNAL */
   // Tensor load error bits
   input logic [DcacheTlErrorBits-3:0]                tensor_load_err_flags,
   // Cache Ops error bits
   input logic [DcacheErrFlagRange:0][NrThreads-1:0]  cache_ops_err_flags,
   // Tensor reduce error bits
   input logic                                        tensor_reduce_err_flags,
   // Progress stored by the Dcache when found a TLB miss
   output logic [NrThreads-1:0][CoreGscCntBits-1:0]   gsc_progress,
   // PMU signals
   output logic [PmuMinionCountersNum-1:0]                             pmu_count_up,
   input  logic [NrThreads-1:0][XregSize-1:0]                         pmu_read_data,
   output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]            pmu_read_sel,
   output logic [PmuTotalCountersNum-1:0]                              pmu_write_en,
   output logic [XregSize-1:0]                                        pmu_write_data,
   output logic [PmuNeighEventCntSelBits-1:0]                         pmu_neigh_event_sel,
   // Debug signals
   input dcache_debug_sigs                                            dcache_debug_signals
);

   ////////////////////////////////////////////////////////////////////////////////
   // SIGNAL DECLARATIONS
   ////////////////////////////////////////////////////////////////////////////////
   logic clock_wb;
   logic clock_dec;

   // Internal clocks/resets using original naming for the body logic.
   // The original uses active-high resets; create aliases.
   logic clock;
   logic reset_c;
   /* verilator lint_off SYNCASYNCNET */  // Translated CSR logic intentionally uses reset_w in both async-sensitive flops and synchronous gating/alias paths.
   logic reset_w;
   /* verilator lint_on SYNCASYNCNET */
   logic reset_d;

   assign clock   = clk_i;
   assign reset_c = !rst_ni;
   assign reset_w = !rst_w_ni;
   assign reset_d = !rst_d_ni;

   // Latch-compatible reset (originally from rst_repeat DFT cell)
   logic reset_w_reg;
   assign reset_w_reg = reset_w;

   // Reset value constants for auto-generated registers
   localparam csr_mstatus_t MstatusResetVal = '{ mpp: PrvM, fs: 2'b00, sxl: 2'b10, uxl: 2'b10, default: '0};
   localparam csr_dcsr_t    DcsrResetVal    = '{ prv: 2'b11, xdebugver: 4'h4, default: '0};
   localparam csr_tdata1_t  Tdata1ResetVal  = '{ ttype: 4'h2, maskmax: 6'h6, default: '0};

   // cpu access and privilege control
   logic                                            io_rw_mem_valid_qual;
   logic [1:0]                                      addr_priv_mem;
   logic                                            cpu_mem_ren;
   logic                                            cpu_mem_wen;
   logic                                            system_insn_mem;
   logic                                            mem_read_only;
   logic                                            mem_priv_ok;
   logic                                            mem_wen;
   logic                                            mem_wen_p2;


   logic                                            m_mode_wb;
   logic                                            wb_read_only;
   logic                                            wb_priv_ok;
   logic                                            cpu_wb_ren;
   logic                                            cpu_wb_wen;
   logic [63:0]                                     wb_mask_data;
   logic [63:0]                                     wb_wdata;
   logic [63:0]                                     wb_wdata_1p;
   /* verilator lint_off UNUSEDSIGNAL */  // Tensor-FMA CSR decode keeps the full packed control word, though only the architectural subset is read locally.
   tensorfma_control                                wb_wdata_as_tfma;
   /* verilator lint_on UNUSEDSIGNAL */
   logic                                            do_system_insn_wb;
   logic                                            system_insn_wb;
   logic                                            insn_call;
   logic                                            insn_break;
   logic                                            insn_ret;
   logic                                            insn_ret_qual;
   logic                                            insn_wfi;
   logic                                            insn_dret;

   logic                                            wb_system_insn_illegal;
   logic                                            mem_trap_virtual_memory;
   logic                                            wb_allow_wfi;
   logic                                            wb_allow_sret;


   logic [NrThreads-1:0][1:0]                       prv;
   logic [NrThreads-1:0][1:0]                       prv_next;

   //for status
   csr_mstatus_t [NrThreads-1:0]                    read_mstatus;
   csr_mstatus_t [NrThreads-1:0]                    read_sstatus;

   // to return read_hartid
   logic [63:0]                                     read_hartid;

   // for wfi
   logic [NrThreads-1:0]                            reg_wfi;
   logic [NrThreads-1:0]                            reg_wfi_next;
   logic [NrThreads-1:0]                            io_csr_stall_next;
   logic [NrThreads-1:0]                            fe_replay_stall;

   // for decoding
   logic                                            wb_addr_valid;
   logic                                            mem_addr_valid;
   logic [XregSize-1:0]                             io_rw_wb_rdata_next;

   // to return correct flags
   csr_fflags_t [NrThreads-1:0] read_fflags;
   csr_fcsr_t [NrThreads-1:0] read_fcsr;

   // for hardware performance monitor
   logic [CsrNrEvents-1:0]                          events;
   logic                                            cycle_up;
   logic [CsrLastCoreHpm:CsrFirstHpm][NrThreads-1:0] hpm_up;
   logic [NrThreads-1:0]                            instret_up;
   logic                                            counter_en_next;
   logic [CsrMaxNrCounters-1:0]                     counteren;
   logic                                            counter_xcpt, counter_xcpt_next;

   // for debug
   logic [NrThreads-1:0]                            reg_debug, reg_debug_next;
   logic                                            mem_debug_only_reg;
   csr_tdata1_t [NrThreads-1:0]                     read_tdata1;
   logic [NrThreads-1:0]                            single_stepped, single_stepped_next;
   logic                                            trapToDebug;
   logic                                            causeIsDebugInt;
   logic                                            causeIsDebugTrigger;
   logic                                            causeIsDebugBreak;
   logic [NrThreads-1:0]                            resume_thread;
   logic [NrThreads-1:0]                            reg_busy, reg_busy_next;

   // for msg ports
   logic                                            msg_port_wr_en_reg;   // delayed wr_en for dcache control
   logic                                            msg_port_wr_en_reg_next;
   logic [XregSize-1:0]                             msg_port_rdata_common;
   logic [NrThreads-1:0][XregSize-1:0]              msg_port_rdata;
   logic [DcacheWayBits-1:0]                        msg_port_scp_way_reg; // delayed scp_way for dcache control
   logic [DcacheWayBits-1:0]                        msg_port_scp_way_reg_next;
   logic [DcacheLramAddrBits-1:0]                   msg_port_wr_addr_reg; // delayed wr_addr offset for dcache control
   logic [DcacheLramAddrBits-1:0]                   msg_port_wr_addr_reg_next;
   logic                                            l2_resp_msg_valid;
   logic                                            msg_port_xcpt;
   logic                                            msg_port_pget_block; //pulse so the instruction to replay the csrr

   logic [NrThreads-1:0][NrMsgPorts-1:0]            msg_port_stall;
   logic [XregSize-1:0]                             portctl_rdata_common;
   logic [NrThreads-1:0][XregSize-1:0]              portctl_rdata;


   //for conv
   logic [NrThreads-1:0][15:0]                      conv_bits;
   logic [NrThreads-1:0]                            conv_can_accept_new_write;
   logic [NrThreads-1:0]                            conv_save_mask;
   logic [NrThreads-1:0]                            conv_bits_ready;
   logic [NrThreads-1:0]                            conv_bits_ready_z1;
   logic [NrThreads-1:0]                            reg_tensor_mask_ready;
   logic                                            reg_tensor_mask_read_invalid;
   logic                                            reg_tensor_mask_read_invalid_next;

   // for interrupts
   csr_mip_t [NrThreads-1:0]                        read_mip;
   csr_mip_t [NrThreads-1:0]                        m_interrupts;

   csr_sip_t [NrThreads-1:0]                        read_sip;
   csr_sie_t [NrThreads-1:0]                        read_sie;
   csr_sip_t [NrThreads-1:0]                        sip_wdata;
   csr_sie_t [NrThreads-1:0]                        sie_wdata;
   csr_mip_t [NrThreads-1:0]                        s_interrupts;

`ifdef CSR_U_INTERRUPTS
   csr_uip_t [NrThreads-1:0]                        read_uip;
   csr_uie_t [NrThreads-1:0]                        read_uie;
   csr_mip_t [NrThreads-1:0]                        u_interrupts;
`endif

   csr_mip_t [NrThreads-1:0]                        pending_interrupts;
   csr_mip_t [NrThreads-1:0]                        all_interrupts;

   logic [NrThreads-1:0][4:0]                       interrupt_cause;

   // for tensor error
   terror_t0_t reg_tensor_error_t0, reg_tensor_error_t0_next; //tensor errors for thread0
   terror_t1_t reg_tensor_error_t1, reg_tensor_error_t1_next; //tensor errors for thread0
   logic [NrThreads-1:0] [TensorErrorSz-1:0]        reg_tensor_error;

   logic                                            tenb_cfg_err;
   logic                                            tenb_cfg_err_next;
   /* verilator lint_off UNOPTFLAT */  // These tensor/cache-op helpers sit in preserved CSR replay and clock-gated control cones that flatten into false loops during full-core elaboration.
   logic                                            tenb_restart_process;
   logic                                            tenb_restart_ongoing;
   logic                                            tenb_restart_ongoing_next;
   logic                                            tload_is_for_tenb;
   logic                                            tload_to_tenb_pend_sticky_error;
   logic                                            tensor_store_cfg_err;
   logic                                            tensor_store_cfg_err_next;
   /* verilator lint_off UNUSEDSIGNAL */  // Tensor-store CSR decode keeps the full packed control word, though only the architectural subset is read locally.
   tensorstore_control                              tensor_store_wb_data;
   /* verilator lint_on UNUSEDSIGNAL */
   logic                                            tensor_store_replay_condition;
   logic                                            tenb_tl_pend_sticky;
   logic                                            tenb_tl_pend_sticky_next;
   logic [3:0]                                      tenb_tl_numlines;
   logic                                            tenb_tl_numlines_up;
   logic                                            tenb_tl_restart;
   logic                                            tenb_tl_restart_delayed;
   logic                                            tenb_tl_pend_restart;
   logic                                            tenb_tl_pend_restart_next;
   logic                                            tenb_tl_pend_tfma_en;
   logic                                            tenb_tl_pend_tfma_en_next;
   logic                                            tenb_cancel_scp_off;
   logic                                            tload_ready_for_start;
   logic                                            scpbit_unset_err_flag;
   logic                                            scpbit_unset_err_flag_tensor_store;
   logic                                            scpbit_unset_err_flag_tensor_load;
   logic                                            scpbit_unset_err_flag_tensor_fma;
   logic                                            scpbit_unset_err_flag_tensor_quant;

   // scratchpad/cache control
   logic [CsrUcacheCtrlSz-1:0]                      reg_ucache_control;
   /* verilator lint_off UNUSEDSIGNAL */  // The preserved ucache control register width includes low bits this translated block never reads back.
   logic [CsrUcacheCtrlSz-1:0]                      reg_ucache_control_u;
   /* verilator lint_on UNUSEDSIGNAL */
   logic [CsrMcacheCtrlSz-1:0]                      reg_mcache_control;

   // for exceptions
   logic [NrThreads-1:0]                            fp_csr;
   logic                                            delegate;
   csr_tvec_t                                       tvec;
   csr_pc_t                                         tvec_pc;
   csr_pc_t                                         epc;
   logic                                            xcpt_write_badaddr;
   logic                                            xcpt_write_insn_bits;
   logic                                            xcpt_write_addr_from_wdata;
   logic                                            exception;
   csr_cause_t                                      cause;
   logic                                            trap_on_ml_or_thread1;
   logic                                            bad_tensor_rm;

   logic                                            reg_co_lock_va_en_qual;
   logic                                            reg_co_unlock_va_en_qual;
   logic                                            usr_cache_op_xcpt;
   logic                                            mem_feature_xcpt;
   logic                                            wb_feature_xcpt;
   logic                                            usr_shadow_xcpt;
   logic                                            usr_shadow_xcpt_next;
   logic                                            tensor_fma_xcpt;
   logic                                            tensor_fma_replay_condition;
   logic                                            tensor_quant_replay_condition;
   logic                                            tensor_reduce_xcpt;
   logic                                            tensor_quant_xcpt;
   /* verilator lint_on UNOPTFLAT */

   // for cooperative tensor stores or loads if disabled
   logic                                            reg_tensor_store_en_qual;
   logic                                            reg_tensor_load_en_qual;
   logic                                            reg_tensor_load_en_clean;
   logic                                            reg_tensor_fma_en_qual;
   logic                                            reg_tensor_reduce_en_qual;
   logic                                            reg_tensor_quant_en_qual;
   logic                                            coop_tensor_xcpt;
   logic                                            coop_tensor_store_xcpt;
   logic                                            coop_tensor_load_xcpt;

   // for fl barrier
   logic                                            fl_barrier_ready; // Fast local barrier unit is ready

   // to get port information from "csr_msgs" module
   logic                                           req_port_info_thread_id;
   logic [$clog2(NrMsgPorts)-1:0]                  req_port_info_port_id;
   logic                                           req_port_info_enabled;
   /* verilator lint_off UNUSEDSIGNAL */  // Message-port replies retain the full packed struct; only the queried fields are consumed in this block.
   core_msg_port_conf_t                            req_port_info_reply;
   /* verilator lint_on UNUSEDSIGNAL */

   //portheads/nb enable bits
   logic                                     porthead_rd_en_any;
   logic                                     portheadnb_rd_en_any;

   //performance counters
   logic [XregSize-1:0]                              reg_mcycle;
   logic [NrThreads-1:0][XregSize-1:0]              reg_minstret;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter3;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter4;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter5;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter6;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter7;
   logic [NrThreads-1:0][XregSize-1:0]              reg_mhpmcounter8;

   // signals for fcc counter
   logic [(NrThreads*FccPerMin)-1:0]                 fcc_ready;
   logic [(NrThreads*FccPerMin)-1:0]                 fcc_overflow;
   logic [$clog2(NrThreads*FccPerMin)-1:0]           wb_fcc_addr;                          // Which FCC we are accessing
   logic [(NrThreads*FccPerMin)-1:0][15:0]           reg_fcc_counter, reg_fcc_counter_next; // FCC counters
   logic [(NrThreads*FccPerMin)-1:0]                 reg_fcc_dec;                           // Per FCC counter decrement
   logic                                             reg_fcc_clk_en;                        // Enable clocks of FCC registers
   logic [FccPerMin-1:0][XregSize-1:0]               read_nb_fcc;

   logic tensor_fma_wen;      // delayed tensorFMA write to send updated reg values
   logic tensor_quant_wen;    // delayed tensorQuant write to send updated reg values
   logic tensor_fma_wen_next;
   logic tensor_quant_wen_next;

   // Save threads that are writing to different CSRs to provide the right conv_bits to each operation
   logic tensor_load_thread_id;
   logic tensor_fma_thread_id;
   logic cache_op_thread_id, cache_op_thread_id_en;


   ////////////////////////////////////////////////////////////////////////////////
   // local clock gatings
   ////////////////////////////////////////////////////////////////////////////////
   logic any_reset;
   assign any_reset = reset_c | reset_w | reset_d;

   prim_clk_gate clk_gate_wb  (.clk_i(clock), .en_i( (io_rw_wb_cmd != CsrCmdN && !io_replay) || any_reset),   .dft_i('0), .clk_o(clock_wb));
   prim_clk_gate clk_gate_dec (.clk_i(clock), .en_i( (io_rw_mem_valid_qual || cpu_wb_wen) || reset_c),         .dft_i('0), .clk_o(clock_dec));

   ////////////////////////////////////////////////////////////////////////////////
   // CSR STATEMENTS AUTO GENERATED (signal declarations, tasks, FF macros)
   ////////////////////////////////////////////////////////////////////////////////
/* verilator lint_off UNOPTFLAT */  // The autogenerated CSR _pre/_next/latch network preserves the original phased update protocol; Verilator reports false integrated loops when it flattens these cleaned register views in full-core elaboration.
/* verilator lint_off SYNCASYNCNET */  // The autogenerated CSR enable/latch network intentionally mixes async-reset flops with synchronous compare/gating on the same translated signals.
`include "intpipe_csr_file_auto_declarations.svh"
`include "intpipe_csr_file_auto_decode_seq.svh"
/* verilator lint_off COMBDLY */  // Auto-generated CSR flops preserve the original latch/clock structure.
`include "intpipe_csr_file_auto_write_seq.svh"
/* verilator lint_on COMBDLY */
/* verilator lint_off UNUSEDSIGNAL */  // Auto-generated CSR conversion helpers cast full packed words and intentionally discard reserved bits.
`include "intpipe_csr_file_auto_type_conversions.svh"
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on SYNCASYNCNET */
/* verilator lint_on UNOPTFLAT */

   ////////////////////////////////////////////////////////////////////////////////
   // TYPE CONVERSIONS: reg_X = func(reg_X_pre)
   //
   // The _pre registers hold the raw flop output.  The cleaned reg_X signals
   // are produced by running each through a write_*_non_reserved function that
   // zeroes reserved fields, fixes hard-wired values (xdebugver, sxl, uxl,
   // ttype, maskmax), etc.  For identity-cast types the _pre is just recast.
   ////////////////////////////////////////////////////////////////////////////////

   /* verilator lint_off UNOPTFLAT */  // Cleaned CSR register views are fed from generated _pre flops and reused by read/decode helpers; full-core flattening reports false cycles through the staged CSR/dcache replay cone.
   // ── NON_SHARED (per-thread) typed registers ──
   always_comb reg_fcsr[0]            = write_fcsr_non_reserved(reg_fcsr_pre[0]);
   always_comb reg_fcsr[1]            = write_fcsr_non_reserved(reg_fcsr_pre[1]);

   always_comb reg_tensor_conv_size[0] = tensorconv_size'(reg_tensor_conv_size_pre[0]);
   always_comb reg_tensor_conv_size[1] = tensorconv_size'(reg_tensor_conv_size_pre[1]);

   always_comb reg_tensor_conv_ctrl[0] = tensorconv_control'(reg_tensor_conv_ctrl_pre[0]);
   always_comb reg_tensor_conv_ctrl[1] = tensorconv_control'(reg_tensor_conv_ctrl_pre[1]);

   always_comb reg_stvec[0]           = write_tvec_non_reserved(reg_stvec_pre[0]);
   always_comb reg_stvec[1]           = write_tvec_non_reserved(reg_stvec_pre[1]);

   always_comb reg_sepc[0]            = write_pc_non_reserved(reg_sepc_pre[0]);
   always_comb reg_sepc[1]            = write_pc_non_reserved(reg_sepc_pre[1]);

   always_comb reg_scause[0]          = write_cause_non_reserved(reg_scause_pre[0]);
   always_comb reg_scause[1]          = write_cause_non_reserved(reg_scause_pre[1]);

   always_comb reg_mstatus[0]         = write_mstatus_non_reserved(reg_mstatus_pre[0]);
   always_comb reg_mstatus[1]         = write_mstatus_non_reserved(reg_mstatus_pre[1]);

   always_comb reg_medeleg[0]         = write_medeleg_non_reserved(reg_medeleg_pre[0]);
   always_comb reg_medeleg[1]         = write_medeleg_non_reserved(reg_medeleg_pre[1]);

   always_comb reg_mideleg[0]         = write_mideleg_non_reserved(reg_mideleg_pre[0]);
   always_comb reg_mideleg[1]         = write_mideleg_non_reserved(reg_mideleg_pre[1]);

   always_comb reg_mie[0]             = write_mie_non_reserved(reg_mie_pre[0]);
   always_comb reg_mie[1]             = write_mie_non_reserved(reg_mie_pre[1]);

   always_comb reg_mtvec[0]           = write_tvec_non_reserved(reg_mtvec_pre[0]);
   always_comb reg_mtvec[1]           = write_tvec_non_reserved(reg_mtvec_pre[1]);

   always_comb reg_mepc[0]            = write_pc_non_reserved(reg_mepc_pre[0]);
   always_comb reg_mepc[1]            = write_pc_non_reserved(reg_mepc_pre[1]);

   always_comb reg_mcause[0]          = write_cause_non_reserved(reg_mcause_pre[0]);
   always_comb reg_mcause[1]          = write_cause_non_reserved(reg_mcause_pre[1]);

   always_comb reg_mip[0]             = write_mip_non_reserved(reg_mip_pre[0]);
   always_comb reg_mip[1]             = write_mip_non_reserved(reg_mip_pre[1]);

   always_comb reg_tdata1[0]          = write_tdata1_non_reserved(reg_tdata1_pre[0]);
   always_comb reg_tdata1[1]          = write_tdata1_non_reserved(reg_tdata1_pre[1]);

   always_comb reg_dcsr[0]            = write_dcsr_non_reserved(reg_dcsr_pre[0]);
   always_comb reg_dcsr[1]            = write_dcsr_non_reserved(reg_dcsr_pre[1]);

   always_comb reg_dpc[0]             = write_pc_non_reserved(reg_dpc_pre[0]);
   always_comb reg_dpc[1]             = write_pc_non_reserved(reg_dpc_pre[1]);

   always_comb reg_minstmask[0]       = minst_mask_t'(reg_minstmask_pre[0]);
   always_comb reg_minstmask[1]       = minst_mask_t'(reg_minstmask_pre[1]);

   // ── SHARED (single-instance) typed registers ──
   always_comb reg_tensor_fma         = tensorfma_control'(reg_tensor_fma_pre);
   always_comb reg_tensor_cooperation = tensorcoop_control'(reg_tensor_cooperation_pre);
   always_comb reg_tensor_quant       = tensorquant_control'(reg_tensor_quant_pre);
   always_comb reg_satp               = write_satp_non_reserved(reg_satp_pre);
   always_comb reg_matp               = write_matp_non_reserved(reg_matp_pre);
   always_comb reg_menable_shadows    = menable_shadows_t'(reg_menable_shadows_pre);
   /* verilator lint_on UNOPTFLAT */

   ////////////////////////////////////////////////////////////////////////////////
   // CSR ADDRESS DECODING + READ
   ////////////////////////////////////////////////////////////////////////////////
   always_comb begin
      // note: using always_comb rather than always@* , so that the signals used in the
      // functions (and not tasks) are taken into account for the sensitivity list

      // set default values
      mem_addr_valid = wb_addr_valid;
      io_rw_wb_rdata_next = io_rw_wb_rdata;
      mem_feature_xcpt = 1'b0;

`include "intpipe_csr_file_auto_decode_default.svh"

      // decode (case (io_rw_mem_addr) with en=io_rw_mem_valid_qual)
      if(io_rw_mem_valid_qual) begin
         case (csr_ad_t'(io_rw_mem_addr))
`include "intpipe_csr_file_auto_decode_cases.svh"
           default:
             mem_addr_valid = 1'b0;
         endcase
      end

      // check if some port is being read
      porthead_rd_en_any = csr_ad_t'(io_rw_mem_addr[CsrRegAddrSize-1:0]) inside {csr_ad_PORTHEAD0, csr_ad_PORTHEAD1, csr_ad_PORTHEAD2, csr_ad_PORTHEAD3} && io_rw_mem_valid_qual;
      portheadnb_rd_en_any =  csr_ad_t'(io_rw_mem_addr[CsrRegAddrSize-1:0]) inside {csr_ad_PORTHEADNB0, csr_ad_PORTHEADNB1, csr_ad_PORTHEADNB2, csr_ad_PORTHEADNB3} && io_rw_mem_valid_qual;

      // do not update tdata1 if dmode is set and not in debug mode
      for ( int i = 0; i < NrThreads; i++) begin
         if (!reg_debug[i] && reg_tdata1[i].dmode)
           reg_tdata1_en_next[i] = 1'b0;
      end

      // get when tensor mask is read ( in case we have to replay)
      reg_tensor_mask_read_invalid_next = csr_ad_t'(io_rw_mem_addr) == csr_ad_TENSOR_MASK && io_rw_mem_valid_qual &&
                                          !reg_tensor_mask_ready[mem_thread_id];

   end // always_comb

   // `FF(clock, io_rw_wb_rdata, io_rw_wb_rdata_next)
   always_ff @(posedge clock)
     io_rw_wb_rdata <= io_rw_wb_rdata_next;

   // `FF(clock, wb_addr_valid, mem_addr_valid)
   always_ff @(posedge clock)
     wb_addr_valid <= mem_addr_valid;

   // `RST_FF(clock, reset_w, reg_tensor_mask_read_invalid, reg_tensor_mask_read_invalid_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) reg_tensor_mask_read_invalid <= 1'b0;
     else         reg_tensor_mask_read_invalid <= reg_tensor_mask_read_invalid_next;

   // return values for FCSR flags
   // FP flags are updated in vpu F8 => which can correspond to MEM if
   // the next instruction is a csr read of the flags => just get next value instead)
   always_comb begin
      read_fcsr = reg_fcsr_next;
      for ( int i = 0; i < NrThreads; i++) begin
         read_fflags[i] = read_fcsr_as_fflags(read_fcsr[i]);
      end
   end

  ////////////////////////////////////////////////////////////////////////////////
   // WRITE CSR REGISTERS
   ////////////////////////////////////////////////////////////////////////////////

   // latch wb data
   // Preview the write qualifier before previewing the write payload.
   prim_write_preview_en #(
      .Width (1)
   ) u_mem_wen_p2 (
      .clk_i (clock),
      .en_i  (1'b1),
      .d_i   (mem_wen),
      .q_o   (mem_wen_p2)
   );

   // Enabled write-preview capture for CSR write data.
   prim_write_preview_en #(
      .Width (64)
   ) u_wb_wdata_1p (
      .clk_i (clock),
      .en_i  (mem_wen_p2),
      .d_i   (wb_wdata),
      .q_o   (wb_wdata_1p)
   );

   // combinational part (generates reg_XXX_next).. sequential part inside intpipe_csr_auto.svh
   always_comb begin
      // note: using always_comb rather than always@* , so that the signals used in the
      // functions (and not tasks) are taken into account for the sensitivity list
      reg_debug_next = reg_debug;

      // by default, keep the same value
`include "intpipe_csr_file_auto_write_default.svh"

      // other causes of update (less prioritary than writes)
      if( io_fcsr_flags_valid && |io_fcsr_flags_bits == 1'b1 ) begin
        reg_fcsr_next[io_fcsr_thread_id].flags      |= io_fcsr_flags_bits[4:0];
        reg_fcsr_next[io_fcsr_thread_id].eflags     |= io_fcsr_flags_bits[5];
      end

      // save tensor mask when ready
      for ( int i = 0 ; i < NrThreads; i++)
        if (conv_save_mask[i]) reg_tensor_mask_next[i] = conv_bits[i];

      // save GSC progress when indicated from Dcache
      for ( int i = 0 ; i < NrThreads; i++)
        if (dcache_ctrl_resp.gsc_progress_save[i]) reg_gsc_progress_next[i] = dcache_ctrl_resp.gsc_progress;
      if (bp_progress_save) reg_gsc_progress_next[mem_thread_id] = dcache_ctrl_resp.gsc_progress;

      // writes from core
`include "intpipe_csr_file_auto_write_comb.svh"

      // do not update satp if value is not legal or it is not allowed
      if (!satp_valid(wb_wdata)) reg_satp_latch_clk_en = 1'b0;

      // do not update matp if value is not legal
      if (!matp_valid(reg_matp.lock, csr_matp_t'(wb_wdata)) ) reg_matp_latch_clk_en = 1'b0;

      // Keep values for read-only fields
      for ( int i = 0 ; i < NrThreads; i++) begin
        reg_dcsr_next[i].cause = reg_dcsr[i].cause;
        reg_dcsr_next[i].nmip = reg_dcsr[i].nmip;
      end

      // put only legal values in fields that are WA-RL
      for ( int i = 0 ; i < NrThreads; i++) begin
         reg_mstatus_next[i].mpp = legalizePrivilege(reg_mstatus_next[i].mpp);
         reg_mstatus_next[i].fs = legalizeMstatusFS(reg_mstatus_next[i].fs);
         reg_dcsr_next[i].prv = legalizePrivilege(reg_dcsr_next[i].prv);
      end

     // tdataX registers managment
     for (int i = 0; i < NrThreads; ++i) begin

       if (!reg_debug[i]) begin
         // Don't let M-mode change dmode (Keep the value)
         reg_tdata1_next[i].dmode = reg_tdata1[i].dmode;
         // Don't let M-mode enable action (Keep the value)
         reg_tdata1_next[i].action = reg_tdata1[i].action;
       end

       // Turn down action if tdata1.dmode goes down
       if (!reg_tdata1_next[i].dmode)
         reg_tdata1_next[i].action = 1'b0;

       // Don't let write to tdata2 if dmode is enabled unless we are in debug mode
       if (!reg_debug[i] && reg_tdata1[i].dmode)
         reg_tdata2_latch_clk_en[i] = 1'b0;
      end

      // other causes of update (more prioritary than writes)

      // assignments that take precedence over writes (or RO fields)
      for ( int i = 0; i < NrThreads; i++) begin
         reg_mstatus_next[i].xs = '0;
         reg_mip_next[i].mbad_red |= io_bad_redirect[i];
         reg_mip_next[i].bus_err |= dcache_bus_error[i];
         if (dcache_bus_error[i]) reg_mbusaddr_next[i] = dcache_bus_error_addr[i];
         `ifdef DCACHE_REPORT_PC
         if (dcache_bus_error[i]) reg_mbuspc_next[i] = dcache_bus_error_pc[i];
         `endif
      end

      // Set debug busy signal
      reg_busy_next = reg_busy;
      for (int i = 0; i < NrThreads; ++i) begin
         if (reset_w || ((insn_break || insn_dret || exception) && i[0] == wb_thread_id))
           reg_busy_next[i] = 1'b0;
         else if (ex_progbuf[i])
           reg_busy_next[i] = 1'b1;
      end


      // update registers upon exception
      if (exception) begin
         if (trapToDebug) begin
            reg_dpc_next[wb_thread_id] = copyPC(io_pc);
            reg_debug_next[wb_thread_id] = 1'b1;
            reg_dcsr_next[wb_thread_id].cause = single_stepped[wb_thread_id] ?  3'h4 :
                                                causeIsDebugInt? 3'h3 :
                                                causeIsDebugTrigger? 3'h2 :
                                                3'h1; //ebreak
            reg_dcsr_next[wb_thread_id].prv = prv[wb_thread_id];
         end
         else
           if (!reg_debug[wb_thread_id] && !trapToDebug) begin
             if (delegate) begin

                reg_scause_next[wb_thread_id] = cause;
                reg_sepc_next[wb_thread_id] = copyPC(io_pc);
                if (xcpt_write_badaddr) reg_stval_next[wb_thread_id] = io_badaddr;
                else if (xcpt_write_addr_from_wdata) reg_stval_next[wb_thread_id] = extend_VA(io_rw_wb_wdata);
                else if (xcpt_write_insn_bits) reg_stval_next[wb_thread_id] = io_inst_rvc ? '0 :
                                                                 {{(VaSizeExt-InstSize){1'b0}}, io_inst_bits};
                else reg_stval_next[wb_thread_id] = '0;

                // spie = sie, spp = prv, sie=0
                reg_mstatus_next[wb_thread_id].spie = reg_mstatus[wb_thread_id].sie;
                reg_mstatus_next[wb_thread_id].spp = prv[wb_thread_id][0];
                reg_mstatus_next[wb_thread_id].sie = 1'b0;

             end
             else begin
                reg_mcause_next[wb_thread_id] = cause;
                reg_mepc_next[wb_thread_id] = copyPC(io_pc);
                if (xcpt_write_badaddr) reg_mtval_next[wb_thread_id] = io_badaddr;
                else if (xcpt_write_addr_from_wdata) reg_mtval_next[wb_thread_id] = extend_VA(io_rw_wb_wdata);
                else if (xcpt_write_insn_bits)  reg_mtval_next[wb_thread_id] = io_inst_rvc ? '0 :
                                                                  {{(VaSizeExt-InstSize){1'b0}}, io_inst_bits};
                else reg_mtval_next[wb_thread_id] = '0;

                // mpie = mie, mpp=prv, mie=0
                reg_mstatus_next[wb_thread_id].mpie =  reg_mstatus[wb_thread_id].mie;
                reg_mstatus_next[wb_thread_id].mpp = prv[wb_thread_id];
                reg_mstatus_next[wb_thread_id].mie = 1'b0;
             end // else: !if(delegate)
           end
      end // if (exception)


      // Exit debug mode on resume
      for(int th = 0; th < NrThreads; ++th) begin
        if (resume_thread[th])
          reg_debug_next[th] = 1'b0;
      end

      if (insn_ret_qual) begin
         // in S mode
         if (!m_mode_wb)  begin
            // restore sie from spie
            reg_mstatus_next[wb_thread_id].sie = reg_mstatus[wb_thread_id].spie;
            reg_mstatus_next[wb_thread_id].spie = 1'b1;
            reg_mstatus_next[wb_thread_id].spp = 1'b0;
         end
         // in M mode
         else begin
            reg_mstatus_next[wb_thread_id].mie = reg_mstatus[wb_thread_id].mpie;
            reg_mstatus_next[wb_thread_id].mpie = 1'b1;
            reg_mstatus_next[wb_thread_id].mpp = '0;
         end // else: !if(!m_mode_wb)

      end // if (insn_ret)

   end // always_comb


   ////////////////////////////////////////////////////////////////////////////////
   // Run control and debug
   ////////////////////////////////////////////////////////////////////////////////

    // `FF(clock, reg_busy, reg_busy_next)
    always_ff @(posedge clock)
      reg_busy <= reg_busy_next;

    assign busy = reg_busy;
    assign debug_out_exception = {reg_debug[1] & wb_thread_id & exception & !insn_break,
                                  reg_debug[0] & !wb_thread_id & exception & !insn_break};


   ////////////////////////////////////////////////////////////////////////////////
   // cpu privilege access control
   ////////////////////////////////////////////////////////////////////////////////

   // mem stage
   always_comb begin
      io_rw_mem_valid_qual = io_rw_mem_cmd != CsrCmdN &&
                             !(io_csr_xcpt && wb_thread_id == mem_thread_id);
      addr_priv_mem = io_rw_mem_addr[9:8];
      system_insn_mem = (io_rw_mem_cmd == CsrCmdI);
      cpu_mem_ren = io_rw_mem_cmd != CsrCmdN && !system_insn_mem;
      cpu_mem_wen = cpu_mem_ren && io_rw_mem_cmd != CsrCmdR;
      mem_read_only =  &io_rw_mem_addr[11:10];
      mem_debug_only_reg = (io_rw_mem_addr[CsrRegAddrSize-1:4] == CsrDebugOnlyAdMsb); // register that can only be accessed in debug mode
      mem_priv_ok =  prv[mem_thread_id] >= addr_priv_mem && !mem_debug_only_reg || reg_debug[mem_thread_id];

      mem_wen = cpu_mem_wen && mem_priv_ok && !mem_read_only;
   end

   // wb stage
   csr_mip_t mip_mask;
   always_comb begin
      mip_mask = '0;
      wb_mask_data = io_rw_wb_rdata;

      // data to mask with in set/clear operations
      // will typically be io_rw_wb_rdata, except for reading MIP, where SEIP doesn't have to be ORed with external interrupt
      if (reg_mip_en[wb_thread_id]) begin
         mip_mask = '{seip: 1'b1,
`ifdef CSR_U_INTERRUPTS
                      ueip___zero: 1'b1,
`endif
                      default: '0};
         wb_mask_data =  (io_rw_wb_rdata & ~mip_mask) | (reg_mip[wb_thread_id] & mip_mask);
      end

      wb_wdata =  ( ( (io_rw_wb_cmd == CsrCmdS || io_rw_wb_cmd == CsrCmdC) ? wb_mask_data : '0) | (io_rw_wb_cmd != CsrCmdC ?  io_rw_wb_wdata : '0) ) & ~(io_rw_wb_cmd == CsrCmdC ?  io_rw_wb_wdata : '0);
      wb_wdata_as_tfma = wb_wdata;

      do_system_insn_wb = wb_priv_ok && system_insn_wb;

      // systems instruction decoding
      {insn_call , insn_break, insn_ret, insn_wfi} = '0;
      if (do_system_insn_wb) begin
         casez (io_rw_wb_inst[2:0])
           3'b000: insn_call  = 1'b1;
           3'b001: insn_break = 1'b1;
           3'b01?: insn_ret   = 1'b1;
           3'b1??: insn_wfi   = 1'b1;
         endcase
         // in reality the X's in the casex should be 0... but any other combination should not reach here (intpipe decoder would say illegal insn)
      end // if (do_system_insn_wb)


      // additional privilege checking for system instructions
      mem_trap_virtual_memory = prv[mem_thread_id] < PrvM && reg_mstatus[mem_thread_id].tvm; //this signal is used as  a trap condition in the spreadsheet / macros for accesses to satp reg
      wb_allow_wfi = prv[wb_thread_id]  > PrvS || !reg_mstatus[wb_thread_id].tw;
      wb_allow_sret = prv[wb_thread_id]  > PrvS || !reg_mstatus[wb_thread_id].tsr;

      wb_system_insn_illegal =  system_insn_wb && !wb_priv_ok ||
                                insn_wfi && !wb_allow_wfi ||
                                insn_ret && !wb_allow_sret;
      insn_ret_qual = insn_ret && wb_allow_sret;
      insn_dret = insn_ret && io_inst_bits[31:20] == 12'h7b2;
   end



   // `EN_FF(clock, io_rw_mem_valid_qual, wb_read_only, mem_read_only)
   always_ff @(posedge clock)
     if (io_rw_mem_valid_qual) wb_read_only <= mem_read_only;

   // `EN_FF(clock, io_rw_mem_valid_qual, wb_priv_ok, mem_priv_ok)
   always_ff @(posedge clock)
     if (io_rw_mem_valid_qual) wb_priv_ok <= mem_priv_ok;

   // `EN_FF(clock, io_rw_mem_valid_qual, m_mode_wb, addr_priv_mem[1])
   always_ff @(posedge clock)
     if (io_rw_mem_valid_qual) m_mode_wb <= addr_priv_mem[1];

   // `RST_FF(clock, reset_w, system_insn_wb, system_insn_mem, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) system_insn_wb <= 1'b0;
     else         system_insn_wb <= system_insn_mem;

   // `RST_FF(clock, reset_w, cpu_wb_wen, cpu_mem_wen, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) cpu_wb_wen <= 1'b0;
     else         cpu_wb_wen <= cpu_mem_wen;

   // `RST_FF(clock, reset_w, cpu_wb_ren, cpu_mem_ren, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) cpu_wb_ren <= 1'b0;
     else         cpu_wb_ren <= cpu_mem_ren;


   // updating privilege mode
   always_comb begin
     prv_next = prv;

     if (exception) begin
       if (delegate) prv_next[wb_thread_id] = PrvS;
       else prv_next[wb_thread_id] = PrvM;
     end

     if (insn_ret_qual) begin
       //restore priviliege from spp or mpp
       if (!m_mode_wb) prv_next[wb_thread_id] = legalizePrivilege({1'b0, reg_mstatus[wb_thread_id].spp});
       else prv_next[wb_thread_id] = reg_mstatus[wb_thread_id].mpp;
     end

     for(int i = 0; i < NrThreads; ++i)
       if(resume_thread[i]) // resume_thread is onehot
         // Restore prv when returning from debug
         prv_next[i] = legalizePrivilege(reg_dcsr[i].prv);

   end

   // `RST_FF(clock, reset_w, prv, prv_next, {PrvM, PrvM})
   always_ff @(posedge clock)
     if (reset_w) prv <= {PrvM, PrvM};
     else         prv <= prv_next;



   // mstatus and sstatus return value
   always_comb begin
      for ( int i = 0 ; i< NrThreads; i++) begin
         // determine read value for mstatus
         read_mstatus[i] = reg_mstatus[i];
         // set the RO fields of mstatus
         read_mstatus[i].sd = &reg_mstatus[i].fs || &reg_mstatus[i].xs; // if either xs or fs are dirty (2'b11)

         // and read value for sstatus
         read_sstatus[i] = csr_mstatus_t'(read_mstatus_as_sstatus(read_mstatus[i]));
      end
   end


   ////////////////////////////////////////////////////////////////////////////////
   // debug mode statements
   ////////////////////////////////////////////////////////////////////////////////
   // `RST_FF(clock, reset_w, reg_debug, reg_debug_next, '0)
   always_ff @(posedge clock)
     if (reset_w) reg_debug <= '0;
     else         reg_debug <= reg_debug_next;

   // trigger configuration for breakpoint unit
   always_comb begin
      bp_address = reg_tdata2;
      for ( int i = 0 ; i< NrThreads; i++)
         bp_control[i] ='{ dmode: reg_tdata1[i].dmode,
                           action: reg_tdata1[i].action,
                           match: reg_tdata1[i].match,
                           m: reg_tdata1[i].m,
                           s: reg_tdata1[i].s,
                           u: reg_tdata1[i].u,
                           execute: reg_tdata1[i].execute,
                           load: reg_tdata1[i].load,
                           store:reg_tdata1[i].store};
   end

   // tdata1 return value
   always_comb begin
      read_tdata1 = reg_tdata1;
      for ( int i = 0; i< NrThreads;i++)
        read_tdata1[i].timing = debug_timing[i];
   end


   // Resume procedure
  logic [NrThreads-1:0] dret_resume;
  logic [NrThreads-1:0] external_resume;

   always_comb begin
      for(int th = 0; th < NrThreads; ++th) begin
         dret_resume[th]     = reg_debug[th] && insn_ret && wb_thread_id == th[0];
         external_resume[th] = reg_debug[th] && resume[th] && !reg_busy[th];

         // if both threads with external resume, take thread0
         if(th > 0)
           external_resume[th] &= !external_resume[0];

         // only one thread to be resumed at a time... if a thread is doing a dret, ignore external resume
         resume_thread[th] = dret_resume[th] || external_resume[th] && !dret_resume[~th[0]];
      end
   end // always_comb


   ////////////////////////////////////////////////////////////////////////////////
   // exclusive mode control
   ////////////////////////////////////////////////////////////////////////////////
   assign io_excl_mode = reg_excl_mode;
   // thread selection => the first that sets the CSR reg to 1
   // `RST_EN_FF(clock_wb, reset_w, reg_excl_mode_en && !reg_excl_mode, io_excl_mode_sel, wb_thread_id, 1'b0)
   always_ff @(posedge clock_wb)
     if (reset_w) io_excl_mode_sel <= 1'b0;
     else if (reg_excl_mode_en && !reg_excl_mode) io_excl_mode_sel <= wb_thread_id;

   // pulse when going into or out of exclusive mode
   assign io_excl_mode_transition = reg_excl_mode_en && (reg_excl_mode != wb_wdata[0]);

   ////////////////////////////////////////////////////////////////////////////////
   // hardware performance counters
   ////////////////////////////////////////////////////////////////////////////////
   logic [CsrLastCoreHpm:CsrFirstHpm][NrThreads-1:0] [CsrNrEventsL-1:0] event_sel;
   logic [PmuMinionCountersNum-1:0]                                       pmu_count_up_next;
   logic                                                                  pmu_count_up_en;
   logic                                                                  pmu_count_up_en_next;

   // Generation of internal events and selection of internal and neigh events
   always_comb begin
      // determine when counters need to go up (note that counters are stopped when in debug mode)
      event_sel = {reg_mhpmevent6, reg_mhpmevent5, reg_mhpmevent4, reg_mhpmevent3};

      for (int t = 0; t < NrThreads; t++) begin
         instret_up[t] = io_retire && wb_thread_id == t[0] && !reg_debug[t];
         for ( int i = CsrFirstHpm; i <=CsrLastCoreHpm; i++)
           hpm_up[i][t] = events[event_sel[i][t]] && !reg_debug[t];
      end

      cycle_up = ! (|reg_debug); // stop cycle counter if one of the threads is in debug


      // Send count up signals to PMU unit
      pmu_count_up_next = hpm_up;

      // Event selection for neigh counters. Use registers for both threads
      pmu_neigh_event_sel = {reg_mhpmevent8[1][PmuNeighEventsSelBits-1:0],reg_mhpmevent8[0][PmuNeighEventsSelBits-1:0],
                             reg_mhpmevent7[1][PmuNeighEventsSelBits-1:0],reg_mhpmevent7[0][PmuNeighEventsSelBits-1:0]};
   end

   assign events = {{(CsrNrEvents - CsrNrEventsExt - NrThreads - 2){1'b0}}, io_events_ext, instret_up, cycle_up, 1'b0}; //ev0 = no events

   // Write interface to performance registers
   logic [PmuTotalCountersNum-1:0]  pmu_reg_write;

   // both mcycle and instret are not implemented and will always return 0
   assign reg_mcycle = '0;
   assign reg_minstret = '0;

   logic [PmuTotalCountersNum-1:0]               pmu_write_en_next;
   logic [XregSize-1:0]                          pmu_write_data_next;

   always_comb begin

      pmu_reg_write = {reg_mhpmcounter8_en,reg_mhpmcounter7_en,reg_mhpmcounter6_en,
                       reg_mhpmcounter5_en,reg_mhpmcounter4_en,reg_mhpmcounter3_en};
      if ( |pmu_reg_write != 0 || |pmu_write_en)
        pmu_write_en_next   = pmu_reg_write;
      else if (cpu_wb_ren && counter_xcpt)
        pmu_write_en_next = {PmuTotalCountersNum{1'b0}};
      else
        pmu_write_en_next   = pmu_write_en;

      pmu_write_data_next = wb_wdata;
   end

   // Register pmu counters write signals.
   // Count Up is updated only when some event is selected, making sure that pmu_count_up remains in zero
   // once all the select registers go to zero
   assign pmu_count_up_en_next = |event_sel;

   // `RST_FF(clock, reset_w, pmu_count_up_en, pmu_count_up_en_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) pmu_count_up_en <= 1'b0;
     else         pmu_count_up_en <= pmu_count_up_en_next;

   // `RST_EN_FF(clock, reset_w, pmu_count_up_en, pmu_count_up, pmu_count_up_next, PmuMinionCountersNum'b0)
   always_ff @(posedge clock)
     if (reset_w) pmu_count_up <= PmuMinionCountersNum'(0);
     else if (pmu_count_up_en) pmu_count_up <= pmu_count_up_next;

   // `EN_FF(clock, |pmu_write_en_next, pmu_write_data, pmu_write_data_next)
   always_ff @(posedge clock)
     if (|pmu_write_en_next) pmu_write_data <= pmu_write_data_next;

   // `RST_FF(clock, reset_w, pmu_write_en, pmu_write_en_next, '0)
   always_ff @(posedge clock)
     if (reset_w) pmu_write_en <= '0;
     else         pmu_write_en <= pmu_write_en_next;

   // Read interface to performance counters.
   logic [NrThreads-1:0] pmu_read_mem_ready;
   logic                  pmu_read_en_wb;

   intpipe_csr_pmu_read_interface
   intpipe_csr_pmu_read_interface_inst
   (
      .rst_ni              ( rst_w_ni            ),
      .clk_i               ( clock               ),
      .io_rw_mem_valid     ( io_rw_mem_valid_qual ),
      .io_rw_mem_thread_id ( mem_thread_id        ),
      .io_rw_mem_addr      ( io_rw_mem_addr       ),
      .io_rw_mem_ready     ( pmu_read_mem_ready   ),
      .pmu_read_sel        ( pmu_read_sel         ),
      .pmu_read_en_wb      ( pmu_read_en_wb       )
   );

   // Single register is read based on the selection bits in pmu_read_sel
   // Read selection is propagated through N pipeline stages
   assign reg_mhpmcounter3 = pmu_read_data;
   assign reg_mhpmcounter4 = pmu_read_data;
   assign reg_mhpmcounter5 = pmu_read_data;
   assign reg_mhpmcounter6 = pmu_read_data;
   assign reg_mhpmcounter7 = pmu_read_data;
   assign reg_mhpmcounter8 = pmu_read_data;

   ////////////////////////////////////////////////////////////////////////////////
   // interrupts
   ////////////////////////////////////////////////////////////////////////////////
   always_comb begin
      // return value for xIP registers

      for ( int i = 0 ; i < NrThreads; i++) begin
         read_mip[i] = reg_mip[i];
         read_mip[i].mieco = int_mieco[i];
         read_mip[i].mtip  = int_mtip[i];
         read_mip[i].msip  = int_msip[i];
         read_mip[i].meip  = int_meip[i];

         // external interrupt bits for S and U, Ored with bit for external interrupt controller
         read_mip[i].seip |= int_seip[i];
`ifdef CSR_U_INTERRUPTS
         read_mip[i].ueip___zero |= int_ueip[i];
`endif
         // restricted views of mip and mie for u and s modes
         read_sip[i] = read_mip_as_sip(read_mip[i]) & reg_mideleg[i];
         read_sie[i] = read_mie_as_sie(reg_mie[i] ) & reg_mideleg[i];
`ifdef CSR_U_INTERRUPTS
         read_uip[i] = read_mip_as_uip(read_mip[i]) & reg_sideleg[i] & reg_mideleg[i];
         read_uie[i] = read_mie_as_uie(reg_mie[i] ) & reg_sideleg[i] & reg_mideleg[i];
`endif

         // when writing SIP, only SSIP is writeable ( also UEIP and USIP if they were implemented) and only if delegating
         sip_wdata[i] = get_sip_wdata(wb_wdata[$bits(csr_sip_t)-1:0], reg_mip[i], reg_mideleg[i]);
         // same for sie, only writeable if delegating
         sie_wdata[i] = get_sie_wdata(wb_wdata[$bits(csr_sie_t)-1:0], reg_mie[i], reg_mideleg[i]);
      end

      // pending interrupts
      pending_interrupts = read_mip & reg_mie;
      for ( int i = 0 ; i < NrThreads; i++) begin

         // interrupts in m mode: if priv < M or if mstatus.mie and prv=M, and not delegating
         m_interrupts[i] = !reg_debug[i] && ( prv[i] < PrvM || prv[i] == PrvM && reg_mstatus[i].mie) ?
                           pending_interrupts[i] &  ~reg_mideleg[i] : '0;

         // for s mode... if priv < S or if mstatus.SIE and prv=S and not delegating
         s_interrupts[i] = !reg_debug[i] && (prv[i] < PrvS || prv[i] == PrvS && reg_mstatus[i].sie) ?
                           pending_interrupts[i] & reg_mideleg[i]/* & ~reg_sideleg[i] */: '0;

`ifdef CSR_U_INTERRUPTS
         // and u mode..
         u_interrupts[i] = ! reg_debug[i] && prv[i] == PrvS && reg_mstatus[i].uie ?
                          pending_interrupts[i] & reg_mideleg[i] & reg_sideleg[i] : '0;
`endif
      end // for ( int i = 0 ; i < NrThreads; i++)

      // set interrupt cause
      all_interrupts = m_interrupts | s_interrupts/* |u_interrupts*/;

      for ( int i = 0 ; i < NrThreads; i++)
        interrupt_cause[i] = selectInterrupt(all_interrupts[i]);

   end // always_comb

   always_comb begin
      for ( int i = 0; i < NrThreads; i++) begin
         // if there are any interrupts and not in single step mode (because stepie is fixed to 0)
         // and if single stepped, to enter again in debug
         io_interrupt[i] =  ( (|all_interrupts[i]) && !io_singleStep[i] && !reg_excl_mode) || single_stepped[i];
         io_interrupt_cause[i] = single_stepped[i] ? XcptDebug : interrupt_cause[i];

         // ignore interrupts and enter debug mode (treated as an interrupt) if halt is requested
         if (halt[i] && !reg_debug[i]) begin
            io_interrupt[i] = 1'b1;
            io_interrupt_cause[i] = XcptDebug;
         end

      end // for ( int i = 0; i < NrThreads; i++)

   end // always_comb

   /* verilator lint_off UNUSEDSIGNAL */  // Interrupt and CSR helper functions intentionally accept full packed structs and inspect only architecturally defined bits.
   function automatic logic [4:0] selectInterrupt;
      // returns interrupt cause code
      input  csr_mip_t in;
      begin
         //in decreasing prio order: external, software, timer
         if      (in.meip) selectInterrupt = 5'd11; //Machine external interrupt (MEIP)
         else if (in.seip) selectInterrupt = 5'd9;  //Supervisor external interrupt (SEIP)
`ifdef CSR_U_INTERRUPTS
         else if (in.ueip___zero) selectInterrupt = 5'd8; //User external interrupt (UEIP)
`endif
         else if (in.msip) selectInterrupt = 5'd3;  //Machine software interrupt (MSIP)
         else if (in.ssip) selectInterrupt = 5'd1;  //Supervisor software interrupt (SSIP)
`ifdef CSR_U_INTERRUPTS
         else if (in.usip___zero) selectInterrupt = 5'd0; //User software interrupt (USIP)
`endif
         else if (in.mtip) selectInterrupt = 5'd7;  //Machine timer interrupt (MTIP)
         else if (in.stip) selectInterrupt = 5'd5;  //Supervisor timer interrupt (STIP)
`ifdef CSR_U_INTERRUPTS
         else if (in.utip___zero) selectInterrupt = 5'd4; //User timer interrupt (UTIP)
`endif
         else if (in.mbad_red) selectInterrupt = 5'd16; // bad trigger redirect (MBAD_RED)
         else if (in.mieco)    selectInterrupt = 5'd19; // Icache ECC error count overflow (MIECO)
         else if (in.bus_err)  selectInterrupt = 5'd23; // data bus error (BUS_ERROR)
         else selectInterrupt = {5{1'bx}}; //don't care
      end

   endfunction // selectInterrupt
   /* verilator lint_on UNUSEDSIGNAL */


   ////////////////////////////////////////////////////////////////////////////////
   // exceptions
   ////////////////////////////////////////////////////////////////////////////////

   // generate exception if reading a counter without sufficient privilege
   always_comb begin
      counter_en_next = (io_rw_mem_addr >= CsrRegAddrSize'(CsrFirstCounter) && io_rw_mem_addr< CsrRegAddrSize'(CsrFirstCounter + CsrMaxNrCounters));
      counteren = reg_mcounteren[mem_thread_id];
      if ( !counter_en_next || io_prv[mem_thread_id] == PrvM)
        counter_xcpt_next = '0;
      else begin
         if (prv[mem_thread_id] == PrvU) counteren &= reg_scounteren[mem_thread_id];
         counter_xcpt_next = ! counteren[io_rw_mem_addr[CsrMaxNrCountersL-1:0]];
      end
   end

   // accessing features that have been disabled by ESR
   always_comb begin

      usr_cache_op_xcpt = (reg_co_lock_va_en | reg_co_unlock_va_en) && esr_features.disable_lock_unlock;

      reg_co_lock_va_en_qual   = reg_co_lock_va_en   && !usr_cache_op_xcpt;
      reg_co_unlock_va_en_qual = reg_co_unlock_va_en && !usr_cache_op_xcpt;

   end

   // trap cooperative tensor stores or loads if disabled
   always_comb begin
      // enable tensor stores unless coop is enabled or if they are not cooperative
      reg_tensor_store_en_qual = reg_tensor_store_en && (esr_shire_coop_mode || wb_wdata[50:49] == 2'b00 || tensor_store_wb_data.scp);
      reg_tensor_load_en_qual  = reg_tensor_load_en  && (esr_shire_coop_mode || wb_wdata[62]==1'b0);
      coop_tensor_store_xcpt = ( reg_tensor_store_en_qual != reg_tensor_store_en);
      coop_tensor_load_xcpt = ( reg_tensor_load_en_qual  != reg_tensor_load_en);
      coop_tensor_xcpt = coop_tensor_store_xcpt || coop_tensor_load_xcpt;
      reg_tensor_load_en_clean = reg_tensor_load_en_qual && !scpbit_unset_err_flag_tensor_load && !coop_tensor_load_xcpt;

   end

   // several CSRs are not allowed in thread1
   assign trap_on_ml_or_thread1 = esr_features.trap_on_ml || mem_thread_id == 1'b1;
   assign bad_tensor_rm = invalid_rm(reg_fcsr[0].rm) && !(fp_csr[0] && wb_thread_id == 1'b0) ||
                          invalid_rm(reg_fcsr_next[0].rm) && (fp_csr[0] && wb_thread_id == 1'b0);


   // accessing a floating point register
   assign fp_csr = reg_fcsr_en | reg_fflags_en | reg_frm_en;

   // accessing a shadow register that is disabled
   assign usr_shadow_xcpt_next =  (!reg_menable_shadows.hartid && csr_ad_t'(io_rw_mem_addr) == csr_ad_HARTID) &&
                                  prv[mem_thread_id] != PrvM && io_rw_mem_cmd != CsrCmdN;

   // check for invalid operation in tensor fma
   always_comb begin
      if (reg_tensor_fma_en) begin
         reg_tensor_fma_en_qual = tensor_fma_valid_operation(wb_wdata[3:1]) &&
                                  (!tensor_fma32(wb_wdata[3:1]) || !bad_tensor_rm);
         tensor_fma_xcpt  = !reg_tensor_fma_en_qual;
      end
      else begin
         reg_tensor_fma_en_qual = 1'b0;
         tensor_fma_xcpt  = 1'b0;
      end
   end

   // check for invalid operation in tensor reduce
   // Option 2  (best in timing)
   always_comb begin
      if (reg_tensor_reduce_en) begin
         if (wb_wdata[1:0] != DcacheReduceSend && wb_wdata[27:24]==4'b0000) begin
            reg_tensor_reduce_en_qual = !bad_tensor_rm ;
            tensor_reduce_xcpt  = !reg_tensor_reduce_en_qual;
         end else begin
            reg_tensor_reduce_en_qual = 1'b1 ;
            tensor_reduce_xcpt  = 1'b0;
         end
      end
      else begin
         reg_tensor_reduce_en_qual = 1'b0;
         tensor_reduce_xcpt  = 1'b0;
      end
   end

   // check for invalid operation in tensor quant
   always_comb begin
      if (reg_tensor_quant_en) begin
         // Opt 2: Best timming : check rounding config always! (tquant_is_fp is not used)
         tensor_quant_xcpt = bad_tensor_rm;
         reg_tensor_quant_en_qual = !tensor_quant_xcpt;
      end
      else begin
         reg_tensor_quant_en_qual = 1'b0;
         tensor_quant_xcpt  = 1'b0;
      end
   end

   // flop xcpts from mem to wb
   logic en_mem_xcpts;
   assign en_mem_xcpts = (usr_shadow_xcpt ^usr_shadow_xcpt_next) |
                         (counter_xcpt ^ counter_xcpt_next)      |
                         (mem_feature_xcpt ^ wb_feature_xcpt);

   // `RST_EN_FF(clock, reset_w, en_mem_xcpts, usr_shadow_xcpt, usr_shadow_xcpt_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w)          usr_shadow_xcpt <= 1'b0;
     else if (en_mem_xcpts) usr_shadow_xcpt <= usr_shadow_xcpt_next;

   // `RST_EN_FF(clock, reset_w, en_mem_xcpts, counter_xcpt, counter_xcpt_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w)          counter_xcpt <= 1'b0;
     else if (en_mem_xcpts) counter_xcpt <= counter_xcpt_next;

   // `RST_EN_FF(clock, reset_w, en_mem_xcpts, wb_feature_xcpt, mem_feature_xcpt, 1'b0)
   always_ff @(posedge clock)
     if (reset_w)          wb_feature_xcpt <= 1'b0;
     else if (en_mem_xcpts) wb_feature_xcpt <= mem_feature_xcpt;

   // exception control
   always_comb begin
      cause = '0; // by default set to 0 (includes reserved fields)

      // exceptions generated by CSR file
      io_csr_xcpt = cpu_wb_wen && wb_read_only        ||                                       // writing a RO CSR
                    cpu_wb_ren && ( !wb_priv_ok       ||                                       // accessing without privileges
                                    !wb_addr_valid    ||                                       // accessing non existing CSR
                                    counter_xcpt      ||                                       // counter exception
                                    fp_csr[wb_thread_id] && reg_mstatus[wb_thread_id].fs == '0 // accessing FP CSR and FP=of
                                    )                 ||
                    wb_system_insn_illegal            ||                                       // xcpt when doing sys instruction
                    msg_port_xcpt                     ||                                       // xcpt from msg port
                    wb_feature_xcpt                   ||                                       // trying to access disabled feature
                    usr_cache_op_xcpt                 ||                                       // disable user level cache op
                    coop_tensor_xcpt                  ||                                       // forbidden coop tensor load or store
                    usr_shadow_xcpt                   ||                                       // trying to access disabled shadow registers in u-mode
                    tensor_fma_xcpt                   ||                                       // invalid op in tensor fma
                    tensor_reduce_xcpt                ||                                       // invalid op in tensor reduce
                    tensor_quant_xcpt                 ||                                       // invalid op in tensor quant
                    insn_call  || insn_break;



      if (!io_csr_xcpt) begin
         cause.cause = io_cause.cause;
         cause.interrupt_x = io_cause.interrupt_x;
      end
      else if ( insn_call)
         cause.cause = prv[wb_thread_id] == PrvU ? XcptUserEcall :
                       prv[wb_thread_id] == PrvS ? XcptSupervisorEcall :
                       XcptMachineEcall;
      else if ( insn_break)
        cause.cause = XcptBreakpoint;
      else cause.cause = XcptIllegalInstruction;

      causeIsDebugInt = cause.interrupt_x && xcpt_cause_e'(cause.cause) == XcptDebug;
      causeIsDebugTrigger = !cause.interrupt_x && xcpt_cause_e'(cause.cause) == XcptDebug;
      causeIsDebugBreak = !cause.interrupt_x && insn_break && ( prv[wb_thread_id] == PrvU && reg_dcsr[wb_thread_id].ebreaku ||
                                                        prv[wb_thread_id] == PrvS && reg_dcsr[wb_thread_id].ebreaks ||
                                                        prv[wb_thread_id] == PrvM && reg_dcsr[wb_thread_id].ebreakm );


      xcpt_write_badaddr = !cause.interrupt_x && (xcpt_cause_e'(cause.cause) == XcptMisalignedLoad  ||
                                                  xcpt_cause_e'(cause.cause) == XcptMisalignedStore ||
                                                  /* xcpt_cause_e'(cause.cause) == XcptMisalignedFetch ||*/  //commented out => we never have this exception, compressed extension always on
                                                  xcpt_cause_e'(cause.cause) == XcptLoadPageFault  ||
                                                  xcpt_cause_e'(cause.cause) == XcptStorePageFault ||
                                                  xcpt_cause_e'(cause.cause) == XcptLoadAccess      ||
                                                  xcpt_cause_e'(cause.cause) == XcptStoreAccess);

      xcpt_write_insn_bits = !cause.interrupt_x && (xcpt_cause_e'(cause.cause) == XcptIllegalInstruction ||
                                                    xcpt_cause_e'(cause.cause) == XcptMcodeInstruction );

      xcpt_write_addr_from_wdata = !cause.interrupt_x && (xcpt_cause_e'(cause.cause) == XcptBreakpoint       ||
                                                          xcpt_cause_e'(cause.cause) == XcptFetchPageFault ||
                                                          xcpt_cause_e'(cause.cause) == XcptFetchAccess );

      delegate =  prv[wb_thread_id] < PrvM &&
                  (cause.interrupt_x ? reg_mideleg[wb_thread_id][{1'b0, cause.cause}] :
                                       reg_medeleg[wb_thread_id][cause.cause]);

      // select s or m trap vectors depending on if we are delegating
      tvec = delegate ? reg_stvec[wb_thread_id] :  reg_mtvec[wb_thread_id];
      tvec_pc = csr_pc_t'(tvec);
      tvec_pc[1:0] = 2'b00; // 2 LSB of PC are always 0... in tvec they are used for mode


      // if in vectored mode, add interupt cause times 4
      // base is expected to be aligned to 4k (12 lsb are 0), so we can substitute the bits directly instead of adding
      if (tvec.mode && cause.interrupt_x)
        tvec_pc[2+:5]=cause.cause;

      exception = io_exception || io_csr_xcpt;

      trapToDebug = exception && !reg_debug[wb_thread_id] && (single_stepped[wb_thread_id] || causeIsDebugInt || causeIsDebugTrigger || causeIsDebugBreak);

      xcpt_traps_to_debug = trapToDebug;

      // select return PC after mret/sret instructions
      epc = !m_mode_wb ? reg_sepc[wb_thread_id] : reg_mepc[wb_thread_id];

      // and select PC sent to core ( either trap vector or return PC or debug pc)

      io_evec = insn_ret_qual  ? epc  : tvec_pc;
      io_evec = (trapToDebug || (reg_debug[wb_thread_id] && insn_break)) ? '0 : io_evec; //Avoid X's propagation on debug mode. Debug doesn't use the PC but it needs the fe req

      // Note that both threads won't resume at the same time (there is an assert for that)
      io_evec_resume = resume_thread[0] ? reg_dpc[0] : reg_dpc[1];

   end

   ////////////////////////////////////////////////////////////////////////////////
   // Computes if the instruction needs to be replayed due a structure hazard
   ////////////////////////////////////////////////////////////////////////////////
   intpipe_csr_replay #(
     .VpuEn(VpuEn)
   ) csr_replay (
     .clk_i             ( clock               ),
     .rst_ni            ( rst_w_ni            ),
     .wb_wdata          ( wb_wdata            ),
     .wb_thread_id      ( wb_thread_id        ),
     .io_interrupt      ( io_interrupt        ),
     .io_redirect       ( io_redirect         ),
     .io_replay         ( io_replay           ),
     .io_replay_stall   ( fe_replay_stall     ),
     // Remaining ports connected via .*-equivalent explicit connections
     // The replay module uses .* in the original, so we need to list all ports.
     // NOTE: This submodule has many more ports connected via .*. The auto-include
     // file provides the signals. For now, connect the explicitly named ones;
     // the rest are connected by matching signal names.
     .*
   );


   ////////////////////////////////////////////////////////////////////////////////
   // Fast Credit Counter
   ////////////////////////////////////////////////////////////////////////////////

   // `RST_EN_FF(clock, reset_w, reg_fcc_clk_en, reg_fcc_counter, reg_fcc_counter_next, {NrThreads*FccPerMin{16'b0}})
   always_ff @(posedge clock)
     if (reset_w) reg_fcc_counter <= '0;
     else if (reg_fcc_clk_en) reg_fcc_counter <= reg_fcc_counter_next;

   // Computes next value for the fast credit counter registers
   always_comb
   begin
      reg_fcc_clk_en = 1'b0;
      fcc_overflow = '0;
      for(integer i = 0; i < (NrThreads*FccPerMin); i++)
        begin
           reg_fcc_counter_next[i] = reg_fcc_counter[i];
           if(reg_fcc_dec[i])
             begin
                reg_fcc_counter_next[i] = reg_fcc_counter_next[i] - 16'b1;
                reg_fcc_clk_en          = 1'b1;
             end
           if(int_fcc[i])
             begin
                reg_fcc_counter_next[i] = reg_fcc_counter_next[i] + 16'b1;
                reg_fcc_clk_en          = 1'b1;
             end
           if(reg_fcc_counter_next[i] == 0 && reg_fcc_counter[i] == 16'hffff) begin //Overflow
              fcc_overflow[i] = 1'b1;
           end
        end
   end

   // Outputs of the Fast Credit Counter
   always_comb
   begin
      // Updates ready signals
      for(integer i = 0; i < (NrThreads*FccPerMin); i++)
         fcc_ready[i] = (reg_fcc_counter[i] != 16'b0);

      // FCC accessed in WB stage
      wb_fcc_addr = { wb_thread_id, wb_wdata[$clog2(FccPerMin)-1:0] };

      // Compute decrement
      reg_fcc_dec               = {NrThreads*FccPerMin{1'b0}};
      reg_fcc_dec[wb_fcc_addr] = reg_fcc_en[wb_thread_id] && fcc_ready[wb_fcc_addr];

   end // always_comb

   // read of counters for current thread
   always_comb begin
      read_nb_fcc = '0;  // only 32 LSB have actual data... bits up to 63 are 0 when read
      for ( int t = 0 ; t < NrThreads; t++ )
        for ( int i = 0 ; i < FccPerMin; i++)
          read_nb_fcc[t][i*16+:16] = reg_fcc_counter[t*FccPerMin + i];
   end

   ////////////////////////////////////////////////////////////////////////////////
   // Convolution CSR
   ////////////////////////////////////////////////////////////////////////////////
   genvar gen_it;
   generate for (gen_it = 0 ; gen_it < NrThreads; gen_it ++) begin: CONV
      intpipe_csr_file_conv conv_thread
          (
           // System signals
           .clk_i                ( clock                                                ),
           .rst_ni               ( rst_w_ni                                             ),
           // CSR information
           .tensorconv_size_data ( reg_tensor_conv_size[gen_it]                         ),
           .tensorconv_ctrl_data ( reg_tensor_conv_ctrl[gen_it]                         ),
           // CSR write enable information to know when to start computing
           .tensorconv_size_wen  ( reg_tensor_conv_size_en[gen_it]                      ),
           .tensorconv_ctrl_wen  ( reg_tensor_conv_ctrl_en[gen_it]                      ),
           .tensorload_wen       ( reg_tensor_load_en_qual                              ),
           .tensorfma_wen        ( reg_tensor_fma_en_qual                               ),
           .cacheop_wen          ( dcache_ctrl.cache_op_valid &&  wb_thread_id == gen_it),
           // Ready signal to CSR file to accept a new write
           .can_accept_new_write ( conv_can_accept_new_write[gen_it]                    ),
           // Ready signal to consumers for the computed skip bits
           .conv_save_mask       ( conv_save_mask[gen_it]                               ),
           .conv_bits_ready      ( conv_bits_ready[gen_it]                              ),
           .conv_bits            ( conv_bits[gen_it]                                    )
           );
   end // block: CONV
   endgenerate


   ////////////////////////////////////////////////////////////////////////////////
   // message ports
   ////////////////////////////////////////////////////////////////////////////////
   intpipe_csr_msgs csr_msgs
  (
   .clk_i(clock),
   .clk_wr_i(clock_wb),
   .rst_ni(rst_w_ni),
   .mpp(read_mstatus[wb_thread_id].mpp),
   .cpu_mem_porthead_en_any(porthead_rd_en_any),
   .cpu_mem_portheadnb_en_any(portheadnb_rd_en_any),
   .cpu_mem_sen({reg_smsg_port3_en_next, reg_smsg_port2_en_next, reg_smsg_port1_en_next, reg_smsg_port0_en_next} ),
   .mem_addr(io_rw_mem_addr[$clog2(NrMsgPorts)-1:0]),
   .mem_cmd(io_rw_mem_cmd),
   .mem_valid_qual(io_rw_mem_valid_qual),
   .mem_thread_id(mem_thread_id),
   .mem_wdata(io_rw_mem_wdata[3:0]),
   .mem_rdata_next(msg_port_rdata_common),
   .cpu_wb_sen({reg_smsg_port3_en, reg_smsg_port2_en, reg_smsg_port1_en, reg_smsg_port0_en} ),
   .wb_thread_id(wb_thread_id),
   .wb_wdata(wb_wdata),
   .msg_port_xcpt(msg_port_xcpt),
   .msg_port_stall(msg_port_stall),
   .l2_resp_valid(l2_resp_valid),
   .l2_resp_ready(l2_resp_ready),
   .l2_resp(l2_resp),
   .msg_port_wr_en_reg_next(msg_port_wr_en_reg_next),
   .msg_port_scp_way_reg_next(msg_port_scp_way_reg_next),
   .msg_port_wr_addr_reg_next(msg_port_wr_addr_reg_next),
   .l2_resp_msg_valid(l2_resp_msg_valid),
   .req_port_info_thread_id(req_port_info_thread_id),
   .req_port_info_port_id(req_port_info_port_id),
   .req_port_info_enabled(req_port_info_enabled),
   .req_port_info_reply(req_port_info_reply),
   .portctl(portctl_rdata_common),
   .msg_port_pget_block(msg_port_pget_block)
   );

   assign msg_port_rdata = { msg_port_rdata_common, msg_port_rdata_common };

   //portctl data is stored inside msgs module
   assign portctl_rdata = { portctl_rdata_common, portctl_rdata_common };

   ////////////////////////////////////////////////////////////////////////////////
   // Fast Local Barrier
   ////////////////////////////////////////////////////////////////////////////////

   intpipe_csr_file_fl_barrier
   fl_barrier
   (
       // System signals
       .clk_i              ( clock                           ),
       .rst_ni             ( rst_w_ni                        ),
       // Trigger a new barrier
       .start_ready        ( fl_barrier_ready                ),
       .start_valid        ( reg_flb_en                      ),
       .start_data         ( wb_wdata[CsrFlBarrierSize-1:0]  ),
       .start_waddr        ( io_rw_wb_waddr                  ),
       .start_thread_id    ( wb_thread_id                    ),
       // Request interface with the neigh
       .neigh_req_valid    ( flb_neigh_req_valid             ),
       .neigh_req_data     ( flb_neigh_req_data              ),
       // Response interface with the neigh
       .neigh_resp_valid   ( flb_neigh_resp_valid            ),
       .neigh_resp_data    ( flb_neigh_resp_data             ),
       // Interface with the RF write
       .rf_wen_ready       ( flb_rf_wen_ready                ),
       .rf_wen_valid       ( flb_rf_wen_valid                ),
       .rf_wen_valid_early ( flb_rf_wen_valid_early          ),
       .rf_wen_thread_id   ( flb_rf_wen_thread_id            ),
       .rf_wen_addr        ( flb_rf_wen_addr                 ),
       .rf_wen_data        ( flb_rf_wen_data                 ),
       // Interface with the INT scoreboard
       .scoreboard_valid   ( flb_scoreboard_valid            ),
       .scoreboard_addr    ( flb_scoreboard_addr             )
   );


   ////////////////////////////////////////////////////////////////////////////////
   // Updates the WFI status
   ////////////////////////////////////////////////////////////////////////////////

   always_comb
     begin
        reg_wfi_next = reg_wfi;
        io_csr_stall_next = io_csr_stall;

        for(integer i = 0; i < NrThreads; i++)
          begin
             // Stop stalling when we receive an interrupt/IPI
             if (|pending_interrupts[i] ||
                 // when about to enter on debug mode
                 (trapToDebug && (wb_thread_id == i[0])) ||
                 // or in exclusive mode
                 reg_excl_mode && io_excl_mode_sel == i[0] ||
                 // or in debug mode
                 reg_debug[i] ||
                 // or received a redirect
                 io_redirect[i]
                 )  begin
                reg_wfi_next[i] = 1'b0;
                io_csr_stall_next[i] = 1'b0; // stop stalling if we were
             end
             else if ((insn_wfi && wb_allow_wfi || reg_stall_en) && (i[0] == wb_thread_id)) begin
                reg_wfi_next[i] = 1'b1; // If doing a WFI / stall, put the thread to sleep
                io_csr_stall_next[i] = reg_stall_en;
             end

          end // for (integer i = 0; i < NrThreads; i++)

     end // always_comb


   // `RST_FF(clock, reset_w, reg_wfi, reg_wfi_next, '0)
   always_ff @(posedge clock)
     if (reset_w) reg_wfi <= '0;
     else         reg_wfi <= reg_wfi_next;

   // `RST_FF(clock, reset_w, io_csr_stall, io_csr_stall_next, '0)
   always_ff @(posedge clock)
     if (reset_w) io_csr_stall <= '0;
     else         io_csr_stall <= io_csr_stall_next;

   ////////////////////////////////////////////////////////////////////////////////
   // Tensor Error Register
   ////////////////////////////////////////////////////////////////////////////////

   // Condition to make the TFMA instruction get an io_replay (see intpipe_csr_replay)
   assign tensor_fma_replay_condition = vpu_tfma_enabled || tensor_fma_wen;

   // Condition to make the TQUANT instruction get and io_replay (see intpipe_csr_replay)
   assign tensor_quant_replay_condition = vpu_tquant_enabled || tensor_quant_wen;

   // Condition to make the TS instruction get an io_replay (see intpipe_csr_replay)
   assign tensor_store_replay_condition = !dcache_ctrl_resp.ts_ready  || vpu_treduce_enabled;

   // Type of tensor load: this is defined in bit 52 when writing the register
   assign tload_is_for_tenb = wb_wdata[52];

   // Capture configuration errors
   assign tenb_cfg_err_next =  reg_tensor_fma_en_qual && !tensor_fma_replay_condition &&
                              (  !tenb_tl_pend_sticky &&  wb_wdata_as_tfma.ten_b ||  // tenb is set, and no TensorLoad TENB in progress
                                  tenb_tl_pend_sticky && !wb_wdata_as_tfma.ten_b ||  // tenb is not set, and a TensorLoad TENB is in progress
                                  tenb_tl_pend_sticky &&  wb_wdata_as_tfma.ten_b &&  // tenb is set and a TensorLoad TENB is in progress and
                                 (tenb_tl_numlines != wb_wdata_as_tfma.cols_a)       // num lines in TensorLoad does not matchthe A columns in TensorFMA
                              );

   assign tload_to_tenb_pend_sticky_error = tload_is_for_tenb && tenb_tl_pend_sticky;

   assign tenb_tl_numlines_up = dcache_ctrl.tensorload_start && tload_is_for_tenb;
   assign tenb_tl_restart     = dcache_ctrl.tensorload_restart_b;

   // SCP bit not set and requesting a tensor operation that requires it
   assign scpbit_unset_err_flag_tensor_store = (reg_tensor_store_en_qual && tensor_store_wb_data.scp) && (reg_ucache_control[1:0] != DcacheMode_Scratchpad);
   assign scpbit_unset_err_flag_tensor_load  = reg_tensor_load_en_qual                                && (reg_ucache_control[1:0] != DcacheMode_Scratchpad);
   assign scpbit_unset_err_flag_tensor_fma   = reg_tensor_fma_en_qual                                 && (reg_ucache_control[1:0] != DcacheMode_Scratchpad);
   assign scpbit_unset_err_flag_tensor_quant = tensor_quant_wen && tquant_scp(reg_tensor_quant.trans) && (reg_ucache_control[1:0] != DcacheMode_Scratchpad);
   assign scpbit_unset_err_flag = scpbit_unset_err_flag_tensor_store | scpbit_unset_err_flag_tensor_load | scpbit_unset_err_flag_tensor_fma | scpbit_unset_err_flag_tensor_quant;

   assign tenb_cancel_scp_off = tenb_tl_pend_sticky & (reg_ucache_control[1:0] != DcacheMode_Scratchpad);

   // Flag to remember that a TenB operation has been started but no paired TFMA started yet
   always_comb
   begin
      tenb_tl_pend_sticky_next = tenb_tl_pend_sticky;
      if(tenb_tl_numlines_up)
          tenb_tl_pend_sticky_next = 1'b1;
      if(tensor_fma_wen_next | tenb_tl_restart)
          tenb_tl_pend_sticky_next = 1'b0;
   end

   // Flag to remember that a TFMA has been paired with TenB but that TFMA has not yet started processing
   always_comb
   begin
      tenb_tl_pend_tfma_en_next = tenb_tl_pend_tfma_en;
      if (tenb_tl_numlines_up)
          tenb_tl_pend_tfma_en_next = 1'b1;
      else if ((vpu_tfma_tenb_working & !tenb_tl_pend_sticky) | tenb_tl_restart)
          tenb_tl_pend_tfma_en_next = 1'b0;
   end

   // Flag to remember that a TenB operation has been replayed but TFMA was still working
   always_comb
   begin
      tenb_tl_pend_restart_next = tenb_tl_pend_restart;
      if (reg_tensor_load_en_clean && tload_to_tenb_pend_sticky_error && tensor_fma_replay_condition)
          tenb_tl_pend_restart_next = 1'b1;
      if (!tensor_fma_replay_condition)
          tenb_tl_pend_restart_next = 1'b0;
   end

   // Flag to remember that TenB is processing a replay request
   always_comb
   begin
      tenb_restart_ongoing_next = tenb_restart_ongoing;
      if (tenb_tl_restart)
          tenb_restart_ongoing_next = 1'b1;
      if (dcache_ctrl_resp.tl_ready_1)
          tenb_restart_ongoing_next = 1'b0;
   end

   assign tenb_tl_restart_delayed = tenb_tl_pend_restart & !tensor_fma_replay_condition;
   assign tenb_restart_process = tenb_tl_restart | tenb_restart_ongoing;

   assign tensor_store_wb_data = wb_wdata;
   assign tensor_store_cfg_err_next = reg_tensor_store_en_qual && !tensor_store_wb_data.scp && !tensor_store_replay_condition &&
                                      (    (tensor_store_wb_data.cols == 2'd2)                                         ||
                                          ((tensor_store_wb_data.coop == 2'd1) && (tensor_store_wb_data.cols == 2'd3)) ||
                                           (tensor_store_wb_data.coop == 2'd2)                                         ||
                                          ((tensor_store_wb_data.coop == 2'd3) && (tensor_store_wb_data.cols != 2'd0))
                                      );

   // `EN_FF(clock_wb, tenb_tl_numlines_up, tenb_tl_numlines, wb_wdata[3:0])
   always_ff @(posedge clock_wb)
     if (tenb_tl_numlines_up) tenb_tl_numlines <= wb_wdata[3:0];

   // `RST_FF(clock, reset_w, tenb_tl_pend_sticky, tenb_tl_pend_sticky_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tenb_tl_pend_sticky <= 1'b0;
     else         tenb_tl_pend_sticky <= tenb_tl_pend_sticky_next;

   // `RST_FF(clock, reset_w, tenb_tl_pend_restart, tenb_tl_pend_restart_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tenb_tl_pend_restart <= 1'b0;
     else         tenb_tl_pend_restart <= tenb_tl_pend_restart_next;

   // `RST_FF(clock, reset_w, tenb_tl_pend_tfma_en, tenb_tl_pend_tfma_en_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tenb_tl_pend_tfma_en <= 1'b0;
     else         tenb_tl_pend_tfma_en <= tenb_tl_pend_tfma_en_next;

   // `RST_FF(clock, reset_w, tenb_restart_ongoing, tenb_restart_ongoing_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tenb_restart_ongoing <= 1'b0;
     else         tenb_restart_ongoing <= tenb_restart_ongoing_next;

   // `RST_FF(clock, reset_w, tenb_cfg_err, tenb_cfg_err_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tenb_cfg_err <= 1'b0;
     else         tenb_cfg_err <= tenb_cfg_err_next;

   // `RST_FF(clock, reset_w, tensor_store_cfg_err, tensor_store_cfg_err_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tensor_store_cfg_err <= 1'b0;
     else         tensor_store_cfg_err <= tensor_store_cfg_err_next;

   // `RST_FF(clock, reset_w, reg_tensor_error_t0, reg_tensor_error_t0_next, '0)
   always_ff @(posedge clock)
     if (reset_w) reg_tensor_error_t0 <= '0;
     else         reg_tensor_error_t0 <= reg_tensor_error_t0_next;

   // `RST_FF(clock, reset_w, reg_tensor_error_t1, reg_tensor_error_t1_next, '0)
   always_ff @(posedge clock)
     if (reset_w) reg_tensor_error_t1 <= '0;
     else         reg_tensor_error_t1 <= reg_tensor_error_t1_next;

// Toggle error bits
   always_comb begin
      //////////////////////////////
      // tensor error for thread0 //
      //////////////////////////////
      reg_tensor_error_t0_next = reg_tensor_error_t0;

      if (reg_tensor_error_en[0])
         reg_tensor_error_t0_next = '{ tload:     wb_wdata[DcacheTlErrorBits-3:0],
                                       fcc:       wb_wdata[3],
                                       scp_unset: wb_wdata[DcacheTxErrorScpbtUnset],
                                       co:        wb_wdata[DcacheCoErrorLockVa],
                                       tenb:      wb_wdata[6],
                                       mem_fault: wb_wdata[7],
                                       tstore:    wb_wdata[8],
                                       treduce:   wb_wdata[9]
                                       };
      // set bits
      reg_tensor_error_t0_next.tload     |= tensor_load_err_flags;
      reg_tensor_error_t0_next.fcc       |= |fcc_overflow[FccPerMin-1:0];
      reg_tensor_error_t0_next.scp_unset |= scpbit_unset_err_flag && wb_thread_id == 1'b0;
      reg_tensor_error_t0_next.co        |= cache_ops_err_flags[DcacheErrFlagCoLock][0];
      reg_tensor_error_t0_next.tenb      |= tenb_cfg_err;
      reg_tensor_error_t0_next.mem_fault |= cache_ops_err_flags[DcacheErrFlagMemFault][0];
      reg_tensor_error_t0_next.tstore    |= tensor_store_cfg_err;
      reg_tensor_error_t0_next.treduce   |= tensor_reduce_err_flags;

      //////////////////////////////
      // tensor error for thread1 //
      //////////////////////////////
      reg_tensor_error_t1_next = reg_tensor_error_t1;
      if (reg_tensor_error_en[1])
        reg_tensor_error_t1_next = '{ fcc:       wb_wdata[3],
                                      scp_unset: wb_wdata[DcacheTxErrorScpbtUnset],
                                      co:        wb_wdata[DcacheCoErrorLockVa],
                                      mem_fault: wb_wdata[7]
                                      };

      reg_tensor_error_t1_next.fcc       |= |fcc_overflow[NrThreads*FccPerMin-1:FccPerMin];
      reg_tensor_error_t1_next.scp_unset |= scpbit_unset_err_flag && wb_thread_id == 1'b1;
      reg_tensor_error_t1_next.co        |= cache_ops_err_flags[DcacheErrFlagCoLock][1];
      reg_tensor_error_t1_next.mem_fault |= cache_ops_err_flags[DcacheErrFlagMemFault][1];


      ////////////////////////////////////
      // return values for tensor error //
      ////////////////////////////////////
      reg_tensor_error = '0; // unset bits => zeros

      reg_tensor_error[0][DcacheTlErrorBits-3:0]    = reg_tensor_error_t0.tload;
      reg_tensor_error[0][3]                         = reg_tensor_error_t0.fcc;
      reg_tensor_error[0][DcacheTxErrorScpbtUnset]   = reg_tensor_error_t0.scp_unset;
      reg_tensor_error[0][DcacheCoErrorLockVa]       = reg_tensor_error_t0.co;
      reg_tensor_error[0][6]                         = reg_tensor_error_t0.tenb;
      reg_tensor_error[0][7]                         = reg_tensor_error_t0.mem_fault;
      reg_tensor_error[0][8]                         = reg_tensor_error_t0.tstore;
      reg_tensor_error[0][9]                         = reg_tensor_error_t0.treduce;

      reg_tensor_error[1][3]                         = reg_tensor_error_t1.fcc;
      reg_tensor_error[1][DcacheTxErrorScpbtUnset]   = reg_tensor_error_t1.scp_unset;
      reg_tensor_error[1][DcacheCoErrorLockVa]       = reg_tensor_error_t1.co;
      reg_tensor_error[1][7]                         = reg_tensor_error_t1.mem_fault;

   end


   ////////////////////////////////////////////////////////////////////////////////
   // Cache Operations and Scratchpad Control
   ////////////////////////////////////////////////////////////////////////////////

   logic [CsrUcacheCtrlSz-1:0] ucache_control_data;
   logic [CsrMcacheCtrlSz-1:0] mcache_control_data;
   logic                        u_scpenable_bit;

   always_comb
   begin
      if (reg_ucache_control_en)
      begin
        mcache_control_data = {u_scpenable_bit,reg_mcache_control[0]};
      end else
      begin
        mcache_control_data = (reg_mcache_control == DcacheMode_Shared)     ? (wb_wdata[CsrMcacheCtrlSz-1:0] != DcacheMode_Split   ? DcacheMode_Shared     : wb_wdata[CsrMcacheCtrlSz-1:0]) :
                              (reg_mcache_control == DcacheMode_Split)      ? (wb_wdata[CsrMcacheCtrlSz-1:0] == DcacheMode_Invalid ? DcacheMode_Split      : wb_wdata[CsrMcacheCtrlSz-1:0]) :
                              (reg_mcache_control == DcacheMode_Scratchpad) ? (wb_wdata[CsrMcacheCtrlSz-1:0] == DcacheMode_Invalid ? DcacheMode_Scratchpad : wb_wdata[CsrMcacheCtrlSz-1:0]) :
                                                                              DcacheMode_Shared;
      end
   end

   // User mode shadows D1Split and scpEnable bits from m CSR. D1Split is read only, ScpEnable is only writable when D1Split is set and only from thread0
   assign u_scpenable_bit = reg_mcache_control[0] && wb_thread_id == 1'b0 ? wb_wdata[1] : reg_mcache_control[1];
   assign ucache_control_data = {wb_wdata[6+:DcacheCoL2MaxOpsBits],   // CacheOp_Max     [10:6]
                                 1'b0,                                // WARL(0)            [5]
                                 wb_wdata[2+:DcacheCoL2ReprateBits],  // CacheOp_RepRate  [4:2]
                                 u_scpenable_bit,                     // SCPEnable          [1]
                                 reg_mcache_control[0]};              // D1Split            [0]

   // Update registers
   logic mcache_control_up;
   logic ucache_control_up;
   assign mcache_control_up = reg_mcache_control_en | reg_ucache_control_en;
   assign ucache_control_up = reg_ucache_control_en;

   // `RST_EN_FF(clock_wb, reset_w, ucache_control_up, reg_ucache_control_u, ucache_control_data, CsrUcacheCtrlResetVal)
   always_ff @(posedge clock_wb)
     if (reset_w) reg_ucache_control_u <= CsrUcacheCtrlResetVal[CsrUcacheCtrlSz-1:0];
     else if (ucache_control_up) reg_ucache_control_u <= ucache_control_data;

   // `RST_EN_FF(clock_wb, reset_w, mcache_control_up, reg_mcache_control, mcache_control_data, CsrMcacheCtrlSz'b0)
   always_ff @(posedge clock_wb)
     if (reset_w) reg_mcache_control <= CsrMcacheCtrlSz'(0);
     else if (mcache_control_up) reg_mcache_control <= mcache_control_data;

   // ucache_control register is the combination of both registers when reading
   assign reg_ucache_control = {reg_ucache_control_u[CsrUcacheCtrlSz-1:2],reg_mcache_control};

   ////////////////////////////////////////////////////////////////////////////////
   // Operations started through CSR writes
   ////////////////////////////////////////////////////////////////////////////////

   assign cache_op_thread_id_en = reg_co_evict_va_en      || reg_co_flush_va_en       ||
                                  reg_co_lock_va_en_qual  || reg_co_unlock_va_en_qual ||
                                  reg_co_prefetch_va_en   || reg_co_evict_sw_en       ||
                                  reg_co_flush_sw_en      || reg_tensor_load_l2scp_en ||
                                  reg_co_lock_sw_en       || reg_co_unlock_sw_en;

   // `RST_FF(clock, reset_w, tensor_fma_wen, tensor_fma_wen_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tensor_fma_wen <= 1'b0;
     else         tensor_fma_wen <= tensor_fma_wen_next;

   // `RST_FF(clock, reset_w, tensor_quant_wen, tensor_quant_wen_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w) tensor_quant_wen <= 1'b0;
     else         tensor_quant_wen <= tensor_quant_wen_next;

   // `EN_FF(clock, l2_resp_msg_valid, msg_port_wr_addr_reg, msg_port_wr_addr_reg_next)
   always_ff @(posedge clock)
     if (l2_resp_msg_valid) msg_port_wr_addr_reg <= msg_port_wr_addr_reg_next;

   // `EN_FF(clock, l2_resp_msg_valid, msg_port_scp_way_reg, msg_port_scp_way_reg_next)
   always_ff @(posedge clock)
     if (l2_resp_msg_valid) msg_port_scp_way_reg <= msg_port_scp_way_reg_next;

   // `RST_EN_FF(clock, reset_w, l2_resp_msg_valid, msg_port_wr_en_reg, msg_port_wr_en_reg_next, 1'b0)
   always_ff @(posedge clock)
     if (reset_w)               msg_port_wr_en_reg <= 1'b0;
     else if (l2_resp_msg_valid) msg_port_wr_en_reg <= msg_port_wr_en_reg_next;

   // `EN_FF(clock, cache_op_thread_id_en, cache_op_thread_id, wb_thread_id)
   always_ff @(posedge clock)
     if (cache_op_thread_id_en) cache_op_thread_id <= wb_thread_id;

   // no need to save thread id for tensor_X operations => it is always thread0
   assign tensor_load_thread_id = 1'b0;
   assign tensor_fma_thread_id = 1'b0;


   // Delay one cycle the "ready" signal for tensor mask as conv_bits may update it one cycle later

   // `FF(clock, conv_bits_ready_z1, conv_bits_ready)
   always_ff @(posedge clock)
     conv_bits_ready_z1 <= conv_bits_ready;

   assign reg_tensor_mask_ready = conv_bits_ready & conv_bits_ready_z1;

   // Tensor Load can accept a new start
   assign tload_ready_for_start = (    (dcache_ctrl_resp.tl_ready_0 && !tload_is_for_tenb)
                                    || (dcache_ctrl_resp.tl_ready_1 &&  tload_is_for_tenb && !tenb_tl_pend_tfma_en)
                                  );

   // Sends control bits to units
   always_comb
     begin
        // Front End
       fe_ctrl_stall = ~reg_debug & (reg_wfi | fe_replay_stall);

       // FE should not be stalled when we are halting
       for(int i = 0; i < NrThreads; ++i)
               if(halt[i])
                 fe_ctrl_stall = '0;

       // Dcache prefetch (tensorLoad)
       dcache_ctrl.tensorload_start          = VpuEn && reg_tensor_load_en_clean && !tload_to_tenb_pend_sticky_error
                                               && tload_ready_for_start;
       dcache_ctrl.tensorload_restart_b      = VpuEn && ((reg_tensor_load_en_clean && tload_to_tenb_pend_sticky_error &&
                                                          !tensor_fma_replay_condition) ||
                                                         tenb_tl_restart_delayed || tenb_cfg_err || tenb_cancel_scp_off);
       dcache_ctrl.tensorload_stride         = VpuEn ? wb_x31_reg[VaSize-1:0] : '0;
       dcache_ctrl.tensorload_ctrl.use_tmask = VpuEn && (tload_is_for_tenb ? 1'b0 : wb_wdata[63]);
       dcache_ctrl.tensorload_ctrl.use_coop  = VpuEn && wb_wdata[62];
       dcache_ctrl.tensorload_ctrl.transform = VpuEn ? tensorload_transform_e'(wb_wdata[61:59]) : TensorTransNone;
       dcache_ctrl.tensorload_ctrl.scp_dest  = VpuEn ? wb_wdata[58:53] : '0;
       dcache_ctrl.tensorload_ctrl.tl_sel    = VpuEn && tload_is_for_tenb;
       dcache_ctrl.tensorload_ctrl.addr      = VpuEn ? wb_wdata[VaSize-1:6] : '0;
       dcache_ctrl.tensorload_ctrl.offset    = VpuEn ? tensorload_offset'(wb_wdata[5:4]) : tensorload_offset'(2'b0);
       dcache_ctrl.tensorload_ctrl.num_lines = VpuEn ? wb_wdata[3:0] : '0;
       dcache_ctrl.tensorcoop_ctrl           = VpuEn ? reg_tensor_cooperation : '0;
       dcache_ctrl.tensorload_tmask_ready    = VpuEn && reg_tensor_mask_ready[tensor_load_thread_id];
       dcache_ctrl.tensorload_tmask_bits     = VpuEn ? reg_tensor_mask[tensor_load_thread_id] : '0;

       // CacheOp
       dcache_ctrl.cache_op_valid           = !io_csr_xcpt &&
                                              ( reg_co_evict_va_en     || reg_co_flush_va_en       ||
                                                reg_co_lock_va_en_qual || reg_co_unlock_va_en_qual ||
                                                reg_co_prefetch_va_en  || reg_co_evict_sw_en       ||
                                                reg_co_flush_sw_en     || (VpuEn && reg_tensor_load_l2scp_en) ||
                                                reg_co_lock_sw_en      || reg_co_unlock_sw_en);
       dcache_ctrl.cache_op_req.use_tmask   = reg_co_lock_sw_en        ? 1'b0 :
                                              reg_co_unlock_sw_en      ? 1'b0 :
                                                                         wb_wdata[63];
       dcache_ctrl.cache_op_req.cmd         = reg_co_evict_va_en       ? cache_op_Evict_VA  :
                                              reg_co_flush_va_en       ? cache_op_Flush_VA  :
                                              reg_co_lock_va_en        ? cache_op_Lock_VA   :
                                              reg_co_unlock_va_en      ? cache_op_Unlock_VA :
                                              reg_co_prefetch_va_en    ? cache_op_Pref      :
                                              reg_co_evict_sw_en       ? cache_op_Evict_SW  :
                                              (VpuEn && reg_tensor_load_l2scp_en) ? cache_op_Pref      :
                                              reg_co_flush_sw_en       ? cache_op_Flush_SW  :
                                              reg_co_lock_sw_en        ? cache_op_Lock_SW   :
                                                                         cache_op_Unlock_SW;
       dcache_ctrl.cache_op_req.dest_level  = (VpuEn && reg_tensor_load_l2scp_en) ? cache_op_L2 : CacheOpLevel_t'(wb_wdata[59:58]);
       dcache_ctrl.cache_op_req.start_level = cache_op_L1;
       dcache_ctrl.cache_op_req.way         = reg_co_evict_sw_en ? wb_wdata[7:6]  :
                                              reg_co_flush_sw_en ? wb_wdata[7:6]  :
                                                                   wb_wdata[56:55];
       dcache_ctrl.cache_op_req.addr        = reg_co_evict_sw_en ? {{(VaSize-10){1'b0}},{wb_wdata[17:14],6'b0}} :
                                              reg_co_flush_sw_en ? {{(VaSize-10){1'b0}},{wb_wdata[17:14],6'b0}} :
                                                                     wb_wdata[VaSize-1:0];
       dcache_ctrl.cache_op_req.count       = reg_co_lock_sw_en ?   4'b0 :
                                              reg_co_unlock_sw_en ? 4'b0 :
                                                                    wb_wdata[3:0];
       dcache_ctrl.cache_op_req.l2_scp_dest = {wb_wdata[62:48],wb_wdata[5:4]};
       dcache_ctrl.cache_op_req.l2_scp_fill = VpuEn && reg_tensor_load_l2scp_en;
       dcache_ctrl.cache_op_req.stride      = wb_x31_reg[VaSize-1:0];
       dcache_ctrl.cache_op_req.thread_id   = wb_thread_id;
       dcache_ctrl.cache_op_req.tmask_ready = reg_tensor_mask_ready[cache_op_thread_id];
       dcache_ctrl.cache_op_req.tmask_bits  = reg_tensor_mask[cache_op_thread_id];

       dcache_ctrl.cache_op_req_next.cmd        = reg_co_evict_va_en_next       ? cache_op_Evict_VA  :
                                                  reg_co_flush_va_en_next       ? cache_op_Flush_VA  :
                                                  reg_co_lock_va_en_next        ? cache_op_Lock_VA   :
                                                  reg_co_unlock_va_en_next      ? cache_op_Unlock_VA :
                                                  reg_co_prefetch_va_en_next    ? cache_op_Pref      :
                                                  reg_co_evict_sw_en_next       ? cache_op_Evict_SW  :
                                                  (VpuEn && reg_tensor_load_l2scp_en_next) ? cache_op_Pref      :
                                                  reg_co_flush_sw_en_next       ? cache_op_Flush_SW  :
                                                  reg_co_lock_sw_en_next        ? cache_op_Lock_SW   :
                                                                                  cache_op_Unlock_SW;
       dcache_ctrl.cache_op_req_next.dest_level   = (VpuEn && reg_tensor_load_l2scp_en_next) ? cache_op_L2 :  CacheOpLevel_t'(io_rw_mem_cmd == CsrCmdC? 2'b00 : io_rw_mem_wdata[59:58]); // if cmd is clear => 0 (because register returns 0 when reading)
       dcache_ctrl.cache_op_req_next.start_level  = cache_op_L1;

       // Other cache control bits
       dcache_ctrl.cache_ctrl               = reg_ucache_control;
       dcache_ctrl.thread_id                = wb_thread_id;

       // Dcache Reduce and Tensor Store
       dcache_ctrl.reduce_start            = VpuEn && reg_tensor_reduce_en_qual && dcache_ctrl_resp.reduce_ready && !vpu_treduce_enabled;
       dcache_ctrl.tensorstore_start       = VpuEn && reg_tensor_store_en_qual && dcache_ctrl_resp.ts_ready && !vpu_treduce_enabled &&
                                             !tensor_store_cfg_err_next && !coop_tensor_xcpt && !scpbit_unset_err_flag_tensor_store;
       dcache_ctrl.tensorstore_stride      = VpuEn ? wb_x31_reg[VaSize-1:0] : '0;
       dcache_ctrl.reduce_tensorstore_ctrl = VpuEn ? wb_wdata : '0;

       // set write destination way and address in d$ data array ( which is set * 64B + wr_ptr<<logsize)
       // For timing purposes, as address/way/wr_en is registered here, Dcache must stall the l2_resp by one cycle
       dcache_ctrl.msg_port_wr_addr = msg_port_wr_addr_reg;
       dcache_ctrl.msg_port_wr_way  = msg_port_scp_way_reg;
       dcache_ctrl.msg_port_wr_en   = msg_port_wr_en_reg;

       // TexSend (consults the port info database to get set and way)
       req_port_info_thread_id = wb_thread_id;
       req_port_info_port_id   = wb_wdata[$clog2(NrMsgPorts)-1:0];

       dcache_ctrl.tex_send_valid          = VpuEn && reg_tex_send_en && dcache_ctrl_resp.tex_send_ready;
       dcache_ctrl.tex_send_ctrl.port_id   = VpuEn ? wb_wdata[$clog2(NrMsgPorts)-1:0] : '0;
       dcache_ctrl.tex_send_ctrl.size      = VpuEn ? wb_wdata[6:4] : '0;
       dcache_ctrl.tex_send_ctrl.way       = VpuEn ? req_port_info_reply.scp_way : '0;
       dcache_ctrl.tex_send_ctrl.set       = VpuEn ? req_port_info_reply.scp_set : '0;
       dcache_ctrl.tex_send_ctrl.port_en   = VpuEn && req_port_info_enabled;
       dcache_ctrl.tex_send_ctrl.thread_id = VpuEn ? wb_thread_id : '0;


       // VPU TensorFMA
       tensor_fma_wen_next                = reg_tensor_fma_en_qual && !tensor_fma_replay_condition && !tenb_restart_process
                                            && !tenb_cfg_err_next && !scpbit_unset_err_flag_tensor_fma;
       vpu_ctrl.tensorfma_start           = tensor_fma_wen;
       vpu_ctrl.tensorfma_ctrl            = reg_tensor_fma;
       vpu_ctrl.tensorfma_conv_bits_ready = reg_tensor_mask_ready[tensor_fma_thread_id];
       vpu_ctrl.tensorfma_conv_bits       = reg_tensor_mask[tensor_fma_thread_id];

       // VPU Reduce
       vpu_ctrl.reduce_start       = dcache_ctrl.reduce_start;
       vpu_ctrl.reduce_ctrl        = dcache_ctrl.reduce_tensorstore_ctrl;

       // VPU TensorQuant
       tensor_quant_wen_next       = reg_tensor_quant_en_qual && !tensor_quant_replay_condition;
       vpu_ctrl.tensorquant_start  = tensor_quant_wen && !scpbit_unset_err_flag_tensor_quant;
       vpu_ctrl.tensorquant_ctrl   = reg_tensor_quant;

       // VPU TensorStore
       vpu_ctrl.tensorstore_start  = dcache_ctrl.tensorstore_start && !dcache_ctrl.reduce_tensorstore_ctrl.tensorstore_scp.scp;

       vpu_ctrl.pending_scoreboard_t0 = pending_scoreboard_t0;
     end // always_comb



   ////////////////////////////////////////////////////////////////////////////////
   // determine hartid (if multithreading feature disabled, return even ids only)
   ////////////////////////////////////////////////////////////////////////////////
   assign read_hartid = {{(64-$bits(shire_id)-$bits(shire_min_id)-1){1'b0}}, shire_id, shire_min_id, mem_thread_id};

   ////////////////////////////////////////////////////////////////////////////////
   // assigning misc outputs to intpipe
   ////////////////////////////////////////////////////////////////////////////////

   always_comb begin
      io_fcsr_rm_thread0 = reg_fcsr[0].rm;
      io_fcsr_rm_thread1 = reg_fcsr[1].rm;

      io_status = mstatus_to_intpipe_status(reg_mstatus);
      io_prv = prv;
      io_debug = reg_debug;

      // refined privilege mode if in debug
      for ( int i = 0; i < NrThreads; i++) begin
         if (reg_debug[i]) begin
            io_prv[i] = PrvM;
            if (!reg_dcsr[i].mprven ) io_status[i].mprv = 1'b0;
         end
      end

      io_satp = csr_satp_t'(reg_satp);
      io_matp = csr_matp_t'(reg_matp);
      io_satp_en = reg_satp_en;
      io_matp_en = reg_matp_en;
      io_eret = insn_ret_qual;
      io_resume = resume_thread;

      for ( int i = 0; i < NrThreads; i++)
         io_singleStep[i] = reg_dcsr[i].step & !reg_debug[i];

      io_minstmask  = reg_minstmask;
      io_minstmatch = reg_minstmatch;

      icache_invalidate = reg_cacheInvalidate_en && wb_wdata[0];
      tlb_invalidate = reg_cacheInvalidate_en && wb_wdata[1];


      update_ddata0 = reg_ddata0_en;
      ddata0_wdata   = wb_wdata;

      gsc_progress = reg_gsc_progress;

   end // always_comb



   always_comb begin
      for ( int i = 0; i< NrThreads; i++) begin
         if ( !io_singleStep[i]) single_stepped_next[i] = 1'b0;
         else if (( io_retire || exception) && i[0] == wb_thread_id) single_stepped_next[i] =  1'b1;
         else single_stepped_next[i] = single_stepped[i];
      end
   end

   // `FF(clock, single_stepped, single_stepped_next)
   always_ff @(posedge clock)
     single_stepped <= single_stepped_next;

   ////////////////////////////////////////////////////////////////////////////////
   // helper functions
   ////////////////////////////////////////////////////////////////////////////////
   function automatic [1:0] legalizePrivilege;
      input [1:0] priv;
      begin
         legalizePrivilege = priv == PrvH ? PrvU : priv;
      end
   endfunction // legalizePrivilege

   function automatic [1:0] legalizeMstatusFS;
      input [1:0] fs;
      begin
         // only valid values are 2'b00 and 2'b11 (OFF and DIRTY)
         // 2'b10 and 2'b01 (INITIAL and CLEAN) will transform into DIRTY
         legalizeMstatusFS = {|fs, |fs};
      end
   endfunction // legalizePrivilege

   /* verilator lint_off UNUSEDSIGNAL */  // Packed CSR helpers below preserve full input widths while reading only the fields required by the original logic.
   function  automatic csr_pc_t copyPC;
      // simply sets unused bits to 0
      input csr_pc_t in;
      begin
         copyPC = '0;
         copyPC.base = in.base;
         copyPC.sign_ext = in.sign_ext;
      end
   endfunction

   function automatic logic satp_valid;
      input [63:0] data;
      begin
         satp_valid = ((data[CsrSatpPpnSz+CsrSatpAsidSz+12 +: CsrSatpModeSz] == CsrSatpModeBare) ||
                       (data[CsrSatpPpnSz+CsrSatpAsidSz+12 +: CsrSatpModeSz] == CsrSatpModeSv39) ||
                       (data[CsrSatpPpnSz+CsrSatpAsidSz+12 +: CsrSatpModeSz] == CsrSatpModeSv48));
      end
   endfunction // legalize_satp

   function automatic logic matp_valid;
      input lock;
      input csr_matp_t newval;
      begin
         // matp.mode has same encoding as satp.mode
         matp_valid = (lock == 0) && ((newval.mode == CsrSatpModeBare) ||
                                      (newval.mode == CsrSatpModeSv39) ||
                                      (newval.mode == CsrSatpModeSv48));
      end
   endfunction
   /* verilator lint_on UNUSEDSIGNAL */

   function automatic logic tensor_fma_valid_operation;
      input [2:0] t;
      begin
         case (t)
           3'b000:  tensor_fma_valid_operation = 1'b1;
           3'b001:  tensor_fma_valid_operation = 1'b1;
           3'b011:  tensor_fma_valid_operation = 1'b1;
           default: tensor_fma_valid_operation = 1'b0;
         endcase
      end
   endfunction

   function automatic logic tensor_fma32;
      input [2:0] t;
      begin
         case (t)
           3'b000:  tensor_fma32 = 1'b1;
           default: tensor_fma32 = 1'b0;
         endcase // case (t)
      end
   endfunction // tensor_fma32

   function automatic logic invalid_rm;
      input [2:0] rm;
      begin
         case (rm)
           3'h5: invalid_rm = 1'b1;
           3'h6: invalid_rm = 1'b1;
           3'h7: invalid_rm = 1'b1;
           default: invalid_rm = 1'b0;
         endcase // case (rm)
      end
   endfunction // invalid_rm

   /* Function not used - so commenting out
   function automatic logic tquant_is_fp;
      input [TquantTransBits-1:0]         trans_cfg;
      logic                               last_found;
      logic [TquantBitsPerTransform-1:0]  trans;

      begin

        tquant_is_fp = 1'b0;
        last_found = 1'b0;

        for (int i = 0; i < TquantNumCfgTransforms; i++)
        begin
          if (last_found == 1'b0)
          begin
            trans = trans_cfg[i*TquantBitsPerTransform+:TquantBitsPerTransform];
            if (trans == TquantLast)
              last_found = 1'b1;
            else
            begin
                if ( (trans == TquantFp32ToInt32Cvt) ||
                       (trans == TquantInt32ToFp32Cvt) ||
                       (trans == TquantFp32MulRow)      ||
                       (trans == TquantFp32MulCol)       )
                begin
                    tquant_is_fp  = 1'b1;
                    last_found = 1'b1;
                end
            end
          end
        end

      end
   endfunction
   */

   // copies only needed fields into new struct
   function automatic intpipe_status_t [NrThreads-1:0] mstatus_to_intpipe_status;
      input csr_mstatus_t [NrThreads-1:0] in;
      intpipe_status_t [NrThreads-1:0] out;
      begin
         for ( int i = 0; i < NrThreads; i++)
           out[i] = '{ mprv: in[i].mprv,
                       mpp: in[i].mpp,
                       sum: in[i].sum,
                       mxr: in[i].mxr,
                       fs:  in[i].fs != 2'b00
                       };
         return out;
      end
   endfunction

   //writing SIP => destination is reg_mip, only ssip is writeable if delegable
   /* verilator lint_off UNUSEDSIGNAL */  // SIP/SIE write helpers keep full packed CSR arguments while updating only delegable architectural bits.
   function automatic csr_sip_t get_sip_wdata;
      input [$bits(csr_sip_t)-1:0] d;
      input csr_mip_t mip;
      input csr_mideleg_t mideleg;
      csr_sip_t dd;
      csr_sip_t ret;
      begin
         dd = csr_sip_t'(d);
         ret = csr_sip_t'(mip);
         if (mideleg.ssip)
           ret.ssip = dd.ssip;
         return ret;
      end
   endfunction


   function automatic csr_sie_t get_sie_wdata;
      input [$bits(csr_sie_t)-1:0] d;
      input csr_mie_t mie;
      input csr_mideleg_t mideleg;
      csr_sie_t dd;
      csr_sie_t ret;
      begin
         dd = csr_sie_t'(d);
         ret = csr_sie_t'(mie);
         if (mideleg.ssip) ret.ssie = dd.ssie;
         if (mideleg.stip) ret.stie = dd.stie;
         if (mideleg.seip) ret.seie = dd.seie;
         return ret;
      end
   endfunction
   /* verilator lint_on UNUSEDSIGNAL */

   function automatic logic tquant_scp;
      input [TquantTransBits-1:0]         trans_cfg;
      logic                               last_found;
      logic [TquantBitsPerTransform-1:0]  trans;

      begin
        tquant_scp = 1'b0;
        last_found = 1'b0;

        // Look for any transformation that requires SCP
        // If the transformation is marked as "last" skip the remaining cases
        for (int i = 0; i < TquantNumCfgTransforms; i++)
        begin
          if (last_found == 1'b0)
          begin
            trans = trans_cfg[i*TquantBitsPerTransform+:TquantBitsPerTransform];
            if (trans == TquantLast)
              last_found = 1'b1;
            else
            begin
              if ((trans == TquantInt32AddRow) || (trans == TquantInt32AddCol) ||
                  (trans == TquantFp32MulRow)  || (trans == TquantFp32MulCol))
                tquant_scp = 1'b1;
            end
          end
        end
      end
   endfunction

   // extend_VA function (was in minion_types.vh)
   function automatic logic [VaSizeExt-1:0] extend_VA;
      input logic [XregSize-1:0] in;
      begin
         extend_VA[VaSize-1:0] = in[VaSize-1:0];
         // if bits 48..63 are all the same as bit 47, then sign-extend
         if (&in[63:VaSize-1] || ~|in[63:VaSize-1])
           extend_VA[VaSizeExt-1] = extend_VA[VaSize-1];  // extra bit is sign extension
         else
           extend_VA[VaSizeExt-1] = !extend_VA[VaSize-1]; // extra bit is negating
      end
   endfunction

   ////////////////////////////////////////////////////////////////////////////////
   // assertions
   ////////////////////////////////////////////////////////////////////////////////

   assign TE_prv   = prv[te_thread_sel];
   assign TE_exception = exception && wb_thread_id == te_thread_sel;
   assign TE_tval = (delegate) ? reg_stval_next[te_thread_sel][VaSizeExt-1:0] : reg_mtval_next[te_thread_sel][VaSizeExt-1:0];
   assign TE_wb_reg_cause_ie = cause;


   wire intpipe_csr_file_unused_ok;
   assign intpipe_csr_file_unused_ok = &{
      wb_wdata_as_tfma.padding0,
      wb_wdata_as_tfma.padding1
   };

endmodule // intpipe_csr_file

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_top — Integer pipeline top-level module.
//
// Mechanical translation of intpipe_top.v (etcore-soc).
// Same signals, same conditions, same timing.

module intpipe_top
  import minion_pkg::*;
  import intpipe_csr_pkg::*;
  #(
    parameter bit VpuEn = 1'b1
  )
  (
   // System signals
   input logic                                          clk_i,

   input logic                                          rst_ni,       // active-low, was reset_c
   input logic                                          rst_w_ni,     // active-low, was reset_w
   input logic                                          rst_d_ni,     // active-low, was reset_d
   input logic                                          te_enable, // ultrasoc trace enc enabled
    // DFT signals
   input  logic                                         dft__reset_byp,     //Reset byp
   input  logic                                         dft__reset,         //DFT mode reset
   // Interrupts
   input                                                minion_interrupts interrupts,
   // Thread id
   input logic [$clog2(256)-1:0]                        shire_id,        // NUM_SHIRE_IDS=256
   input logic [$clog2(32)-1:0]                         shire_min_id,    // MIN_PER_S=32
   // Chicken bits
   input  logic                                         chicken_bit_intpipe,
   // Request to the front end
   output logic [NrThreads-1:0]                         id_fe_req_valid,
   output                                               minion_fe_req [NrThreads-1:0] id_fe_req,
   output logic [NrThreads-1:0]                         id_fe_stall,
   // Front end response
   output logic                                         id_fe_resp_ready,
   input logic                                          id_fe_resp_valid,
   input logic                                          id_fe_resp_thread_id,
   input                                                frontend_core_resp id_fe_resp,
   // ICache flush
   output logic                                         icache_invalidate,
   // FrontEnd, Dcache and VPU control
   output                                               core_dcache_ctrl dcache_ctrl,
   input                                                dcache_core_ctrl dcache_ctrl_resp,
   output                                               core_vpu_ctrl vpu_ctrl,
   // Request to the dcache
   output logic                                         id_dcache_alloc_rq_pre,
   output logic                                         ex_dcache_alloc_rq_val,
   output logic                                         id_dcache_gsc,
   input logic                                          id_dcache_ready,
   output logic                                         ex_dcache_req_valid,
   output                                               minion_dcache_req ex_dcache_req,
   output logic                                         ex_dcache_gsc,
   output logic                                         tag_dcache_kill,
   output logic [VpuLaneNum*VpuDataSSz-1:0]             tag_dcache_store_data,
   output logic                                         mem_dcache_kill,
   output logic [XregSize-1:0]                          wb_dcache_x31,
   // Dcache response
   input logic                                          mem_dcache_resp_int_valid,
   input logic                                          wb_dcache_resp_valid,
   input                                                dcache_minion_resp wb_dcache_resp,
   input                                                dcache_core_bp_t tag_dcache_bp, // data from dcache for breakpoint unit
   input                                                tag_dcache_bp_valid,
   // inpipe decode control coming from frontend
   input                                                minion_control_t id_ctrl,
   // Dcache control
   input logic [NrThreads-1:0]                          id_dcache_ordered,
   input logic                                          id_dcache_idle,
   input                                                dcache_scoreboard_t id_dcache_scoreboard,
   input logic [NrThreads-1:0]                          id_dcache_sb_int_dealloc, // pulse when a INT entry is deallocated
   input logic [NrThreads-1:0]                          id_dcache_sb_fp_dealloc, // pulse when a FP entry is deallocated
   input                                                dcache_tag_xcpt tag_dcache_xcpt,
   input logic                                          tag_dcache_replay_next,
   input logic                                          mem_dcache_flush_pipeline,
   output logic                                         wb_dcache_invalidate_lr,
   output logic                                         wb_dcache_fp_toint, // FP to int instruction in WB
   input logic [NrThreads-1:0]                          dcache_bus_error,   // pulse when there has been an et-link error
   input logic [NrThreads-1:0][PaSize-1:0]              dcache_bus_error_addr,
   `ifdef DCACHE_REPORT_PC
   input logic [NrThreads-1:0][PcSizeExt-1:0]          dcache_bus_error_pc,
   `endif
   // TLB/PTW control
   output                                               minion_satp_info satp_info,
   output                                               minion_satp_info matp_info,
   output                                               satp_info_en,
   output                                               matp_info_en,
   output                                               vm_status_t [NrThreads-1:0] vm_status,
   output logic                                         tlb_invalidate,
   // L2 fills (used for RBOX)
   input logic                                          l2_resp_valid,
   input logic                                          l2_resp_ready,
   input                                                et_link_minion_rsp_info_t l2_resp,
   // Control signals going to VPU
   output                                               minion_vpu_id_req id_vpu_req,
   output                                               minion_vpu_ex_req ex_vpu_req,
   output logic                                         tag_vpu_kill,
   output logic                                         mem_vpu_kill,
   output logic                                         wb_vpu_kill,
   // Dcache response to VPU
   output logic                                         wb_vpu_dcache_resp_valid,
   output                                               dcache_vpu_resp wb_vpu_dcache_resp,
   // thread1 enable and reset
   input logic                                          thread0_enable,
   input logic                                          thread1_enable,
   // VPU control signals coming from frontend
   /* verilator lint_off UNUSEDSIGNAL */  // The preserved VPU ID-control structs carry fields consumed by sibling stages; this block only reads the subset used by intpipe.
   input                                                vpu_id_ctrl_t id_frontend_vpu_ctrl,
   // Control signals coming from VPU
   input                                                vpu_id_ctrl_t id_vpu_ctrl,
   /* verilator lint_on UNUSEDSIGNAL */
   input                                                vpu_minion_ex_ctrl ex_vpu_ctrl,
   input                                                vpu_minion_tag_ctrl tag_vpu_ctrl,
   input                                                vpu_minion_mem_ctrl mem_vpu_ctrl,
   input                                                vpu_minion_wb_ctrl wb_vpu_ctrl,
   // Control signals from VPU to pair with TenB operations in Dcache
   input                                                vpu_tfma_tenb_working,
   // Fast Local Barrier request interface with the neigh
   output logic                                         flb_neigh_req_valid,
   output logic [CsrFlBarrierSize-1:0]                  flb_neigh_req_data,
   // Fast Local Barrier response interface with the neigh
   input logic                                          flb_neigh_resp_valid,
   input logic                                          flb_neigh_resp_data,

   output logic [InstSize-1:0]                          TE_wb_reg_inst,
   output csr_cause_t                                   TE_wb_reg_cause_ie,
   output logic                                         TE_wb_reg_context,
   output logic [VaSizeExt-1:0]                         TE_tval,
   output logic [1:0]                                   TE_prv,
   output logic                                         TE_exception,
   input logic                                          te_thread_sel,

   // interface with debug APB slave
   output logic [NrThreads-1:0]                         update_ddata0,
   output logic [63:0]                                  ddata0_wdata,
   input  [NrThreads-1:0][63:0]                         read_ddata0,
   input  [NrThreads-1:0]                               debug_ex_program_buffer,
   input  logic [NrThreads-1:0]                         halt,
   input  logic [NrThreads-1:0]                         resume,
   output logic [NrThreads-1:0]                         in_debug_mode,
   output logic [NrThreads-1:0]                         debug_busy,
   output logic [NrThreads-1:0]                         debug_exception,
   output intpipe_dbg_monitor_t                         debug_monitor_out,
   // Monitor signals for netlist debug
   output logic                                         wb_valid,
   output logic                                         wb_thread_id,
   output logic [PcSizeExt-1:0]                         wb_reg_pc,
   // ESR configuration
   input logic                                          esr_shire_coop_mode,
   input                                                esr_minion_features_t esr_features,
   // Tensor load error bits
   input logic [DcacheTlErrorBits-3:0]                  tensor_load_err_flags,
   // Cache Ops error bits
   input logic [DcacheErrFlagRange:0][NrThreads-1:0]    cache_ops_err_flags,
   // Tensor reduce error bits
   input logic                                          tensor_reduce_err_flags,
   // PMU signals
   output logic [PmuMinionCountersNum-1:0]                              pmu_count_up,
   input  logic [NrThreads-1:0][XregSize-1:0]                          pmu_read_data,
   output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]             pmu_read_sel,
   output logic [PmuTotalCountersNum-1:0]                               pmu_write_en,
   output logic [XregSize-1:0]                                          pmu_write_data,
   output logic [PmuNeighEventCntSelBits-1:0]                           pmu_neigh_event_sel,
   // Events for performance counters from Dcache and VPU
   input logic [CsrNrEventsDcache+CsrNrEventsVpu-1:0]  io_events_dcache_vpu,
   input                                                dcache_debug_sigs dcache_debug_signals
   );

  // Internal clocks/resets using original naming for the body logic.
  logic clock;
  logic reset_c;
  logic reset_w;
  logic reset_d;
  assign clock   = clk_i;
  assign reset_c = !rst_ni;
  assign reset_w = !rst_w_ni;
  assign reset_d = !rst_d_ni;

  localparam int IdCtrlStallCntSz    = 3;

  // Instruction pattern defines for trans starvation casex
  /* verilator lint_off VARHIDDEN */  // Local masks intentionally mirror VPU aliases for starvation decode.
  localparam logic [31:0] FRCP_PS       = 32'b010110000111?????000?????1111011;
  localparam logic [31:0] FRSQ_PS       = 32'b010110001000?????000?????1111011;
  localparam logic [31:0] FLOG_PS       = 32'b010110000011?????000?????1111011;
  localparam logic [31:0] FEXP_PS       = 32'b010110000100?????000?????1111011;
  localparam logic [31:0] FSIN_PS       = 32'b010110000110?????000?????1111011;
  localparam logic [31:0] FRCP_FIX_RAST = 32'b0011000??????????000?????1111011;
  /* verilator lint_on VARHIDDEN */

  // Inst bits FF masks
  localparam logic [InstSize-1:0] ExInstBitsFFMask   = InstSize'(32'hf80);
  localparam logic [InstSize-1:0] TagInstBitsFFMask  = InstSize'(32'hf80);
  localparam logic [InstSize-1:0] MemInstBitsFFMask  = InstSize'(32'hf80);
  localparam logic [InstSize-1:0] WbInstBitsFFMask   = InstSize'(32'hf80);
  localparam logic [InstSize-1:0] ExInstBitsFFBrMask  = InstSize'(32'hffffff80);
  localparam logic [InstSize-1:0] TagInstBitsFFBrMask = InstSize'(32'hffffff80);
  localparam logic [InstSize-1:0] MemInstBitsFFBrMask = InstSize'(32'h0);
  localparam logic [InstSize-1:0] WbInstBitsFFBrMask  = InstSize'(32'h0);

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // ID Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

   logic [NrThreads-1:0][CoreGscCntBits-1:0] id_reg_gsc_cnt_tid;                          // Which gather/scatter element ID has to start working on, per thread
   logic [CoreGscCntBits-1:0]                id_reg_gsc_cnt;                              // Which gather/scatter element ID has to start working on
   logic [NrThreads-1:0]                     id_reg_fence, id_reg_fence_next;             // Fence: wait dcache to be ordered
   logic                                     id_thread_id;                                // Thread ID in ID stage

   //      CLK    RST      DOUT                DIN                      DEF
   always_ff @(posedge clock) if (reset_w) id_reg_fence <= NrThreads'(0); else id_reg_fence <= id_reg_fence_next;


   ////////////////////////////////////////////////////////////////////////////////
   // Gets the information from the front end response.
   ////////////////////////////////////////////////////////////////////////////////

   logic [PcSizeExt-1:0]                     id_pc;          // PC of the instruction in ID stage
   logic [InstSize-1:0]                      id_inst_bits;   // Instruction from front end
   logic [XregAddrSize-1:0]                  id_raddr3_tmp;  // Read address for 3rd source before G/S mux
   logic [XregAddrSize-1:0]                  id_raddr3_sc;   // Read address for 3rd source for G/S instructions
   logic [XregAddrSize-1:0]                  id_raddr2;      // Read address for 2nd source
   logic [XregAddrSize-1:0]                  id_raddr1;      // Read address for 1st source
   logic [XregAddrSize-1:0]                  id_waddr;       // Write address
   /* verilator lint_off UNOPTFLAT */  // Frontend/intpipe backpressure keeps this ID valid path in a preserved cross-module feedback cone that Verilator flattens spuriously.
   logic                                     id_valid;       // Valid instruction in ID stage
   /* verilator lint_on UNOPTFLAT */
   logic                                     id_pf0;         // Page fault in the instruction fetch
   logic                                     id_pf1;         // Page fault in the instruction fetch
   logic                                     id_af0;         // Access fault in the instruction fetch
   logic                                     id_af1;         // Access fault in the instruction fetch
   logic                                     id_inst_replay; // Instruction has replay bit set
   logic                                     id_inst_rvc;    // RiscV Compressed
   logic                                     id_fetch_bus_error;             // fetch bus error async xcpt pending
   logic                                     id_fetch_ecc_error;             // fetch ecc error async xcpt pending

   // Gets data coming from front end
   /* verilator lint_off UNOPTFLAT */  // ID-stage kill/valid qualification is acyclic per field, but Verilator reports a false cycle through the integrated hazard cone.
   always_comb
     begin
        id_valid       = id_fe_resp_valid;
        id_thread_id   = id_fe_resp_thread_id;
        id_pc          = id_fe_resp.pc;
        id_inst_bits   = id_fe_resp.inst_bits;
        id_pf0         = id_fe_resp.pf0;
        id_pf1         = id_fe_resp.pf1;
        id_af0         = id_fe_resp.af0;
        id_af1         = id_fe_resp.af1;
        id_inst_replay = id_fe_resp.replay;
        id_inst_rvc    = id_fe_resp.rvc;
        id_waddr       = id_fe_resp.inst_bits[11:7];
        id_raddr1      = id_fe_resp.inst_bits[19:15];
        id_raddr2      = id_fe_resp.inst_bits[24:20];
        id_raddr3_tmp  = id_fe_resp.inst_bits[31:27];
        id_raddr3_sc   = id_fe_resp.inst_bits[11:7];
        id_fetch_bus_error = id_fe_resp.bus_error;
        id_fetch_ecc_error = id_fe_resp.ecc_error;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // This module decodes the instruction and generates control information.
   ////////////////////////////////////////////////////////////////////////////////

   logic [NrThreads-1:0]           id_dcache_busy;    // Dcache is busy
   logic [NrThreads-1:0]           id_csr_inflight;   // There's a CSR that is in-flight in pipeline
   logic                           id_fence_new;      // Set fence state in core due a new instruction in ID stage
   logic                           id_do_fence;       // Instruction requires fencing the DCache
   /* verilator lint_off UNOPTFLAT */  // Hazard/replay qualification spans multiple mutually-exclusive control paths that Verilator flattens into a false cycle.
   logic                           id_valid_qual;     // Valid bit for ID stage after kill
   /* verilator lint_on UNOPTFLAT */
   logic                           id_uses_alu;       // set to 1 if alu (and not mul/div) is to be used
   logic                           id_uses_rs1_all;
   logic                           id_uses_rs1_lsb;
   logic                           id_uses_rs2;
   logic                           id_csr_en;         // Enable access to CSR
   logic                           ex_reg_valid;      // Instruction in EX stage
   logic                           gsc_reg_valid;      // Instruction in GSC stage
   logic                           ex_thread_id;      // Thread ID in EX stage
   logic                           ex_csr_en;         // EX is doing a CSR access
   logic                           ex_csr_flush;
   logic                           tag_reg_valid;     // Instruction in TAG stage
   logic                           tag_thread_id;     // Thread ID in TAG stage
   logic                           tag_csr_en;        // TAG is doing a CSR access
   logic                           tag_csr_flush;
   logic                           tag_dcache_bp_triggered;
   logic                           mem_reg_valid;     // Instruction in MEM stage
   logic                           mem_thread_id;     // Thread ID in MEM stage
   logic                           mem_dcache_bp_triggered;
   logic                           mem_dcache_bp_triggered_next;
   logic                           mem_csr_en;        // MEM is doing a CSR access
   logic                           mem_csr_flush;
   logic                           wb_reg_valid;      // Valid instruction in WB stage
   logic                           wb_csr_en;         // WB is doing a CSR access
   logic                           wb_csr_flush;
   minst_mask_t [NrThreads-1:0]    minstmask_reg;     // Machine-mode instruction mask register
   logic [NrThreads-1:0] [31:0]   minstmatch_reg;    // Machine-mode instruction match register
   logic                           minst_match;       // Instruction match mcode configuration
   logic                           csr_excl_mode;                  // minion in exclusive mode
   logic                           csr_excl_mode_sel;              // thread selection for exclusive mode
   /* verilator lint_off UNOPTFLAT */  // Exclusive-mode transition is part of the same false-cycle cone as WB redirect/hazard control.
   logic                           csr_excl_mode_transition;       // transitioning in or out of exclusive mode
   /* verilator lint_on UNOPTFLAT */
   logic                           csr_excl_mode_stall_dcache;
   logic                           csr_excl_mode_stall_dcache_next;
   logic                           csr_excl_mode_stall;
   logic [NrThreads-1:0]           csr_fe_stall;
   logic                           id_xcpt_pf;      // Page fault exception in instruction fetch
   logic                           id_xcpt_af;      // Access fault exception in instruction fetch
   logic                           id_xcpt;                 // Exception in the instruction
   logic                           id_xcpt_ignore_opcode; // xcpt where opcode has to be ignored (e.g. AF from frontend, and FE delivering dummy data)

   //check whether current instruction matches a configured pattern, so it is going
   //to be executed in mcode
   assign minst_match = ((id_inst_bits ^ minstmatch_reg[id_thread_id]) & minstmask_reg[id_thread_id].mask) == 0 &&
                        minstmask_reg[id_thread_id].enable;

   assign id_xcpt_ignore_opcode = id_xcpt_af || id_xcpt_pf || id_fetch_bus_error || id_fetch_ecc_error  || id_ctrl.mcode || minst_match;

   // Computes the fence
   always_comb
     begin

        // Enter in fence mode when the instruction is a fence or it is an atomic
        // instruction with acquire bit set. This affects following instructions.
        id_fence_new = id_ctrl.fence && !id_xcpt_ignore_opcode;


        for(integer i = 0; i < NrThreads; i++)
          begin
             // Computes if there is any CSR in-flight that might trigger a mem
             // operation
             id_csr_inflight[i] = (ex_reg_valid  && ex_csr_en  && (ex_thread_id == i[0])) || // EX instruction is a CSR (it might trigger operations in Dcache)
                        (tag_reg_valid && tag_csr_en && (tag_thread_id == i[0])) ||          // TAG instruction is a CSR (it might trigger operations in Dcache)
                        (mem_reg_valid && mem_csr_en && (mem_thread_id == i[0])) ||          // MEM instruction is a CSR (it might trigger operations in Dcache)
                        (wb_reg_valid  && wb_csr_en  && (wb_thread_id == i[0]));             // WB instruction is a CSR (it might trigger operations in Dcache)

             // Compute if dcache is busy
             id_dcache_busy[i] = !id_dcache_ordered[i]   ||                      // Dcache has a transaction for the thread
                                 (ex_dcache_req_valid && (ex_thread_id == i[0])) || // A transaction in EX going to dcache
                                  id_csr_inflight[i];                            // CSR might trigger a dcache operation

             // Set fence state next cycle if there's a new fence or there's a previous
             // one and still dcache is busy
             id_reg_fence_next[i] = (id_fence_new && id_valid_qual && (i[0] == id_thread_id)) || // New fence
                                    (id_reg_fence[i] && id_dcache_busy[i]);                   // In fence mode and still busy

          end // for (integer i = 0; i < NrThreads; i++)

        // Instruction can't move forward
        // We are in fence mode and it is a memory instruction or a CSR access (ex: TensorFMA wants TensorLoad to be done)
        id_do_fence = id_dcache_busy[id_thread_id] && id_reg_fence[id_thread_id] && (id_ctrl.mem || id_csr_en || id_ctrl.fence) && !id_xcpt_ignore_opcode;
     end

   // Stalls for exclusive mode
   // will stall if:
   //    - entering excl mode and dcache is busy (signal csr_excl_mode_stall_dcache)
   //    - about to start a gsc but an instruction in the pipeline might set the excl mode (in order not to stop the gsc in the middle => progress csr and so on)
   //    - transitioning
   logic           ex_csr_excl_mode;         // EX has a CSR write to excl mode => used to prevent gsc of the other thread in ID
   logic           tag_csr_excl_mode;        // TAG has a CSR write to excl mode => used to prevent gsc of the other thread in ID
   logic           mem_csr_excl_mode;        // MEM has a CSR write to excl mode => used to prevent gsc of the other thread in ID

   always_ff @(posedge clock) if (reset_w) csr_excl_mode_stall_dcache <= 1'b0; else csr_excl_mode_stall_dcache <= csr_excl_mode_stall_dcache_next;
   always_comb begin
      csr_excl_mode_stall_dcache_next = csr_excl_mode_transition ? 1'b1:
                                      csr_excl_mode_stall_dcache ? !id_dcache_idle : csr_excl_mode_stall_dcache;

      csr_excl_mode_stall = csr_excl_mode_stall_dcache ||                      // waiting for the dcache not to be busy
                            id_ctrl.gsc && (                                   // do not launch a gsc if about to enter excl mode
                                     ex_csr_excl_mode && ex_reg_valid    ||    //  (not checking thread, because it's always the other thread)
                                     tag_csr_excl_mode && tag_reg_valid  ||
                                     mem_csr_excl_mode && mem_reg_valid) ||
                            csr_excl_mode_transition;                          // about to enter or exit excl mode
   end

   ////////////////////////////////////////////////////////////////////////////////
   // This module stores the integer register file contents
   ////////////////////////////////////////////////////////////////////////////////

   logic [1:0][XregSize-1:0]      id_data_rf;      // Read data from RF
   logic [1:0][XregAddrSize-1:0]  id_raddr;        // Read address for integer RF
   logic [1:0]                    id_ren;          // Read enable for integer RF
   logic [XregAddrSize-1:0]       wb_rf_waddr;     // Write address to RF
   logic [XregSize-1:0]           wb_rf_wdata;     // Write data to RF
   logic                          wb_rf_wen_early; // 1 cycle early version of the write enable to clock gate the data write latch
   logic                          wb_rf_wen;       // Write enable to RF
   logic                          wb_rf_thread_id; // Thread ID writing to RF
   logic [NrThreads-1:0][XregSize-1:0]   wb_x31_reg;  // X31 reg value in WB stage for each thread
   intpipe_rf
     rf
       (
        // System signals
        .clk_i        ( clock           ),
        // Read port
        .rd_en        ( id_ren          ),
        .rd_thread_id ( id_thread_id    ),
        .rd_addr      ( id_raddr        ),
        .rd_data      ( id_data_rf      ),
        .wb_x31_reg   ( wb_x31_reg      ),
        // Write ports
        .wr_en        ( wb_rf_wen       ),
        .wr_en_early  ( wb_rf_wen_early ),
        .wr_thread_id ( wb_rf_thread_id ),
        .wr_addr      ( wb_rf_waddr     ),
        .wr_data      ( wb_rf_wdata     )
        );

   always_comb
     begin
        // RF access
        id_ren    = { id_ctrl.rxs2, id_ctrl.rxs1 };
        id_raddr  = { id_raddr2, id_raddr1 };
     end

   // Computes the bypasses for the sources
   minion_control_t          ex_ctrl;       // Control bits for instruction in EX stage
   minion_control_t          tag_ctrl;      // Control bits for instruction in TAG stage
   minion_control_t          mem_ctrl;      // Control bits for instruction in MEM stage
   logic [1:0][XregSize-1:0] id_reg_data;   // Register data for the sources of ID instruction
   logic [XregSize-1:0]      ex_alu_out;    // ALU result data in EX stage
   logic [XregSize-1:0]      tag_int_wdata; // Write data of TAG stage to MEM
   logic [XregSize-1:0]      tag_reg_wdata; // ALU result data in TAG stage
   logic [XregSize-1:0]      mem_reg_wdata; // Result data to WB
   logic [XregSize-1:0]      wb_reg_wdata;  // Result data
   logic [XregAddrSize-1:0]  ex_waddr;      // Write address for EX instruction
   logic [XregAddrSize-1:0]  tag_waddr;     // Write address for TAG instruction
   logic [XregAddrSize-1:0]  mem_waddr;     // Write address for MEM instruction
   logic [1:0]               id_raddr_zero; // Read addr is zero, so kill bypass
   logic                     wb_dcache_wen; // Write enable due a dcache value
   logic                     wb_wen;        // WB is writing
   logic [XregAddrSize-1:0]  wb_waddr;      // Write address for WB instruction
   /* verilator lint_off UNUSEDSIGNAL */  // WB control and page-table structs keep their full packed layouts, though only the architectural subset is consumed here.
   minion_control_t wb_ctrl;                 // Control signals for instruction in WB
   /* verilator lint_on UNUSEDSIGNAL */

   always_comb
     begin
        // For all the sources
        for(integer i = 0; i < 2; i++)
          begin
             id_raddr_zero[i] = (id_raddr[i] == 5'b0);
             // Data coming from EX stage
             if(!id_raddr_zero[i] && (ex_thread_id == id_thread_id) && ex_reg_valid && ex_ctrl.wxd && (ex_waddr == id_raddr[i]))
               id_reg_data[i] = ex_alu_out;
             // Data coming from TAG stage alu
             else if(!id_raddr_zero[i] && (tag_thread_id == id_thread_id) && tag_reg_valid && tag_ctrl.wxd && (tag_waddr == id_raddr[i]))
               id_reg_data[i] = tag_reg_wdata; //used to be tag_int_wdata => but only need reg_wdata ( int_data may contain branch target... but no bypass needed if branch)
             // Data coming from MEM stage alu
             else if(!id_raddr_zero[i] && (mem_thread_id == id_thread_id) && mem_reg_valid && mem_ctrl.wxd && (mem_waddr == id_raddr[i]))
               id_reg_data[i] = mem_reg_wdata;

             // Note: for WB, the condition used to be wb_rf_wen and wb_rf_waddr and data wb_rf_data
             // since we can only bypass from either the dcache or regular instructions (not from fp2int, flb, csr...) we only handle these 2 conditions
             // the other cases will be stopped by scoreboard (because the corresponding valid bit for the scoreboards is deasserted the cycle after write for these)
             // data coming from dcache
             else if(!id_raddr_zero[i] && (wb_dcache_resp.dest.thread_id == id_thread_id) && ( wb_dcache_wen) && (wb_dcache_resp.dest.addr == id_raddr[i]))
               id_reg_data[i] = wb_dcache_resp.data[XregSize-1:0];
             // Data coming from WB stage alu
             else if(!id_raddr_zero[i] && (wb_thread_id == id_thread_id) && wb_wen && (wb_waddr == id_raddr[i]))
               id_reg_data[i] = wb_reg_wdata;
             // Data coming from RF
             else
               id_reg_data[i] = id_data_rf[i];
          end
     end

   // Register X31 for Dcache in WB stage
   assign wb_dcache_x31 = wb_x31_reg[wb_thread_id];

   ////////////////////////////////////////////////////////////////////////////////
   // Gather/Scatter replication
   ////////////////////////////////////////////////////////////////////////////////

   logic                       id_ctrl_kill;   // Kill the instruction in ID stage
   /* verilator lint_off UNOPTFLAT */  // Gather/scatter keepalive depends on staged EX progress plus dcache readiness; Verilator flattens that handshake into a false cycle on the helper bit.
   logic                       id_keepgsc;     // Keep Gather/Scatter FSM working
   /* verilator lint_on UNOPTFLAT */
   logic                       ex_gscing;      // EX working on a g/s element that is not the last one
   logic                       id_gscing_one_lane; // starting gsc in last lane (according to progress)
   logic                       ex_ctrl_kill;   // Kill the instruction in EX
   /* verilator lint_off UNOPTFLAT */  // WB redirect selection participates in the same false-cycle cone reported by Verilator.
   logic                       wb_take_pc;     // Take the PC from WB stage
   /* verilator lint_on UNOPTFLAT */
   logic                       ex_gsc_busy;
   logic                       gsc_reg_gscing; // Dealing with a G/S operation
   logic [NrThreads-1:0][CoreGscCntBits-1:0] gsc_progress; // Progress stored by the Dcache when found a TLB miss


   assign id_keepgsc = ex_gscing && !ex_ctrl_kill && id_dcache_ready;
   assign ex_gsc_busy = (ex_gscing || gsc_reg_gscing) && !ex_ctrl_kill;

   assign id_reg_gsc_cnt_tid = gsc_progress;
   assign id_reg_gsc_cnt = id_reg_gsc_cnt_tid[id_thread_id];

   ////////////////////////////////////////////////////////////////////////////////
   // CSR module with all the status registers of the core
   ////////////////////////////////////////////////////////////////////////////////

   minion_bp_ctrl_t[NrThreads-1:0]           bp_control;                     // Breakpoint control struct
   logic [NrThreads-1:0][4:0]                id_csr_interrupt_cause;         // CSR interrupt cause (xcpt_cause_e is 5 bits)
   logic [4:0]                               wb_reg_cause;                   // Cause of the exception (xcpt_cause_e)
   logic                                     wb_reg_xcpt_interrupt;
   csr_cause_t                               wb_reg_cause_ie;                // cause with extra bit saying if it is exception or interrupt
   csr_ctrl_e                                id_csr;                         // CSR access
   csr_ctrl_e                                mem_csr_rw_cmd;                 // CSR command sent from MEM
   /* verilator lint_off UNOPTFLAT */  // WB CSR command/exception remain in the original CSR replay loop that is broken by pipeline flops, but flattened as cyclic by Verilator.
   csr_ctrl_e                                wb_csr_rw_cmd;                  // CSR command sent from WB
   /* verilator lint_on UNOPTFLAT */
   intpipe_status_t [NrThreads-1:0]          status;                         // Hart's current operating state
   logic [NrThreads-1:0][1:0]               prv;                            // hart's current priv mode
   logic [NrThreads-1:0]                    debug;                          // hart's current debug status
   minion_reg_dest_t                         wb_flb_scoreboard_addr;         // Fast local barrier is blocking a register in the scoreboard
   /* verilator lint_off UNUSEDSIGNAL */  // SATP/MATP carry reserved bits that are preserved for interface fidelity but not read by intpipe.
   csr_satp_t                                satp;                           // Supervisor Address Translation and Protection Register
   csr_matp_t                                matp;                           // Machine Address Translation and Protection Register
   /* verilator lint_on UNUSEDSIGNAL */
   logic [XregSize-1:0]                      wb_csr_rdata;                   // Read data out of CSR file
   logic [XregSize-1:0]                      wb_flb_wen_data;                // RF write data for fast local barrier
   logic [VaSizeExt-1:0]                     wb_csr_badaddr;                 // Addr sent to CSR
   logic [PcSizeExt-1:0]                     id_csr_evec;                    // Exception vector from CSR
   logic [PcSizeExt-1:0]                     id_csr_evec_resume;             // Exception vector when exiting debug mode
   logic [InstSize-1:0]                      mem_reg_inst;                   // Bits of the instruction
   logic [InstSize-1:0]                      wb_reg_inst;                    // Instruction bits of the instruction
   logic                                     wb_reg_rvc;                     // instruction is compressed (set mtval/stval to 0 on xcpt)
   logic [XregAddrSize-1:0]                  wb_flb_wen_addr;                // RF write address for fast local barrier
   logic [NrThreads-1:0]                     id_csr_stall;                   // waiting CSR stall register
   logic [NrThreads-1:0]                     id_redirect_allowed;            // trigger redirect allowed
   logic [NrThreads-1:0]                     id_bad_redirect;                // trigger redirect received when not allowed
   logic [NrThreads-1:0]                     id_take_redirect;               // trigger redirect received when allowed
   logic [NrThreads-1:0][VaSize-1:0]         bp_address;                     // Breakpoint address
   logic [NrThreads-1:0]                     debug_timing;                   // timing field
   logic [NrThreads-1:0]                     debug_timing_next;
   csr_ad_t                                  id_csr_addr;                    // Access address for the CSR
   logic [2:0]                               fcsr_rm_thread0;                // Rounding mode thread0
   logic [2:0]                               fcsr_rm_thread1;                // Rounding mode thread1
   logic [NrThreads-1:0]                     id_single_step;                 // Single step mode
   logic                                     id_system_insn;                 // System instruction
   logic                                     id_csr_ren;                     // Read enable to CSR
   logic                                     id_csr_wr_safe;                 // ID write is safe so no flush needed
   logic                                     id_csr_fflags;                  // ID will read/access floating point flags
   logic                                     id_csr_shared;                  // shared CSRs between threads, that need a bubble to be read correctly
   logic                                     id_csr_excl_mode;               // ID write to exclusive mode
   logic                                     id_csr_vpu;                     // ID write causes changes to VPU RF
   logic                                     id_csr_vpu_qual;                // ID write causes changes to VPU RF qualified with id valid
   logic                                     id_csr_flush;                   // Flush the pipeline due a CSR access
   logic                                     id_csr_flush_stall;
   logic [NrThreads-1:0]                     id_csr_interrupt;               // Interrupt being generated in ID stage
   logic                                     mem_valid_qual;                 // Valid instruction in MEM after kill
   /* verilator lint_off UNOPTFLAT */  // WB CSR command/exception remain in the original CSR replay loop that is broken by pipeline flops, but flattened as cyclic by Verilator.
   logic                                     wb_csr_xcpt;                    // CSR had an exception in WB stage
   /* verilator lint_on UNOPTFLAT */
   logic                                     wb_csr_eret;                    // CSR generated an ERET
   logic [NrThreads-1:0]                     wb_csr_resume;                  // CSR generated an ERET
   logic                                     wb_reg_xcpt;                    // Instruction has an exception
   logic                                     wb_csr_replay;                  // WB instruction needs to be replayed due a problem in CSR unit
   logic                                     wb_flb_scoreboard_valid;        // Fast local barrier is blocking a register in the scoreboard
   logic                                     wb_flb_wen_ready;               // RF write ready for fast local barrier
   logic                                     wb_flb_wen_valid;               // RF write valid for fast local barrier
   logic                                     wb_flb_wen_valid_early;         // RF write valid for fast local barrier
   logic                                     wb_flb_wen_thread_id;           // RF write thread for fast local barrier
   logic                                     id_pending_scoreboard_t0;       // There's a pending write in the scoreboard for thread 0

   logic [CsrNrEventsExt-1:0]               io_events_ext;                  // Events external to intpipe_csr
   logic [NrThreads-1:0]                     tbranch_up;                     // Branches taken

   logic                                     xcpt_traps_to_debug;            // Exception traps to debug mode
   logic                                     id_bp_xcpt;                     // Exception due a fetch breakpoint



   // Ints are registered to achieve timing constraints
   logic [NrThreads-1:0]                     intpipe_int_mieco ;
   logic [NrThreads-1:0]                     intpipe_int_mtip  ;
   logic [NrThreads-1:0]                     intpipe_int_msip  ;
   logic [NrThreads-1:0]                     intpipe_int_meip  ;
   logic [NrThreads-1:0]                     intpipe_int_seip  ;
`ifdef CSR_U_INTERRUPTS
   logic [NrThreads-1:0]                     intpipe_int_ueip  ;
`endif
   logic [(NrThreads*FccPerMin)-1:0]         intpipe_int_fcc   ;
   logic [NrThreads-1:0]                     ipi_with_pc;   // Registers to store IPI request info


   //
   //     CLK    DOUT               DIN
   always_ff @(posedge clock) intpipe_int_mieco <= interrupts.mieco;
   always_ff @(posedge clock) intpipe_int_mtip  <= interrupts.mtip;
   always_ff @(posedge clock) intpipe_int_msip  <= interrupts.msip;
   always_ff @(posedge clock) intpipe_int_meip  <= interrupts.meip;
   always_ff @(posedge clock) intpipe_int_seip  <= interrupts.seip;
`ifdef CSR_U_INTERRUPTS
   always_ff @(posedge clock) intpipe_int_ueip  <= interrupts.ueip;
`endif
   always_ff @(posedge clock) intpipe_int_fcc   <= interrupts.fcc;

   // Debug in/out signals
   logic [NrThreads-1:0]                     busy;
   logic [NrThreads-1:0]                     debug_out_exception;
   logic                                     mem_ctrl_kill_no_tlb_miss; // MEM instruction is killed, except for tlb miss

   intpipe_csr_file #(
     .VpuEn(VpuEn)
   ) csr_file (
        // System signals
        .clk_i                      ( clock                          ),
        .rst_ni                     ( !reset_c                       ),
        .rst_w_ni                   ( !reset_w                       ),
        .rst_d_ni                   ( !reset_d                       ),
        // DFT signals
        .test_en                    ( 1'b0                           ), //DFT_SCAN_INSERT
        .dft__reset_byp             ( dft__reset_byp                 ),
        .dft__reset                 ( dft__reset                     ),
        // Interrupts and minion id
        .int_mieco                  ( intpipe_int_mieco              ),
        .int_mtip                   ( intpipe_int_mtip               ),
        .int_msip                   ( intpipe_int_msip               ),
        .int_meip                   ( intpipe_int_meip               ),
        .int_seip                   ( intpipe_int_seip               ),
`ifdef CSR_U_INTERRUPTS
        .int_ueip                   ( intpipe_int_ueip               ),
`endif
        .int_fcc                    ( intpipe_int_fcc                ),
        .shire_id                   ( shire_id                       ),
        .shire_min_id               ( shire_min_id                   ),
        .wb_thread_id               ( wb_thread_id                   ),
        .mem_thread_id              ( mem_thread_id                  ),
        // FrontEnd, Dcache and VPU control
        .fe_ctrl_stall              ( csr_fe_stall                   ),
        .dcache_ctrl                ( dcache_ctrl                    ),
        .dcache_ctrl_resp           ( dcache_ctrl_resp               ),
        .vpu_ctrl                   ( vpu_ctrl                       ),
        .pending_scoreboard_t0      ( id_pending_scoreboard_t0       ),
        .vpu_tfma_enabled           ( id_vpu_ctrl.tfma_enabled
                                      || vpu_ctrl.tensorfma_start    ),
        .vpu_tfma_tenb_working      ( vpu_tfma_tenb_working          ),
        .vpu_tquant_enabled         ( id_vpu_ctrl.tquant_enabled
                                      || vpu_ctrl.tensorquant_start  ),
        .vpu_treduce_enabled        ( id_vpu_ctrl.reduce_enabled     ),
        .tlb_invalidate             ( tlb_invalidate                 ),
        .icache_invalidate          ( icache_invalidate              ),
        // Read/Write port
        .io_rw_mem_addr             ( mem_reg_inst[31:20]            ),
        .io_rw_mem_cmd              ( csr_cmd_t'(mem_csr_rw_cmd)     ),
        .io_rw_wb_inst              ( wb_reg_inst[22:20]             ),
        .io_rw_wb_cmd               ( csr_cmd_t'(wb_csr_rw_cmd)      ),
        .io_rw_wb_rdata             ( wb_csr_rdata                   ),
        .io_rw_mem_wdata            ( mem_reg_wdata                  ),
        .io_rw_wb_wdata             ( wb_reg_wdata                   ),
        .wb_x31_reg                 ( wb_x31_reg[wb_thread_id]       ),
        .io_rw_wb_waddr             ( wb_waddr                       ),
        // Stall, exception, eret and single step
        .io_csr_stall               ( id_csr_stall                   ),
        .io_csr_xcpt                ( wb_csr_xcpt                    ),
        .io_eret                    ( wb_csr_eret                    ),
        .io_resume                  ( wb_csr_resume                  ),
        .io_replay                  ( wb_csr_replay                  ),
        .io_singleStep              ( id_single_step                 ),
        .io_excl_mode               ( csr_excl_mode                  ),
        .io_excl_mode_sel           ( csr_excl_mode_sel              ),
        .io_excl_mode_transition    ( csr_excl_mode_transition       ),
        // Status
        .io_status                  ( status                         ),
        .io_prv                     ( prv                            ),
        .io_debug                   ( debug                          ),
        // SATP
        .io_satp                    ( satp                           ),
        .io_satp_en                 ( satp_info_en                   ),
        .io_matp                    ( matp                           ),
        .io_matp_en                 ( matp_info_en                   ),
        // L2 fills (used for RBOX)
        .l2_resp_valid              ( l2_resp_valid                  ),
        .l2_resp_ready              ( l2_resp_ready                  ),
        .l2_resp                    ( l2_resp                        ),
        // Evec
        .io_evec                    ( id_csr_evec                    ),
        .io_evec_resume             ( id_csr_evec_resume             ),
        // events for counters
        .io_events_ext              ( io_events_ext                  ),
        // Exception and retire from core
        .io_exception               ( wb_reg_xcpt                    ),
        .io_retire                  ( wb_valid                       ),
        .io_cause                   ( wb_reg_cause_ie                ),
        .io_pc                      ( wb_reg_pc                      ),
        .io_badaddr                 ( wb_csr_badaddr                 ),
        .io_inst_bits               ( wb_reg_inst                    ),
        .io_inst_rvc                ( wb_reg_rvc                     ),
        .io_bad_redirect            ( id_bad_redirect                ),
        .io_redirect                ( id_take_redirect               ),
        // VPU
        .io_fcsr_rm_thread0         ( fcsr_rm_thread0                ),
        .io_fcsr_rm_thread1         ( fcsr_rm_thread1                ),
        .io_fcsr_flags_valid        ( wb_vpu_ctrl.fcsr_flags_valid   ),
        .io_fcsr_flags_bits         ( wb_vpu_ctrl.fcsr_flags         ),
        .io_fcsr_thread_id          ( wb_vpu_ctrl.thread_id          ),
        // Interrupts
        .io_interrupt               ( id_csr_interrupt               ),
        .io_interrupt_cause         ( id_csr_interrupt_cause         ),
        // bus errors
        .dcache_bus_error           ( dcache_bus_error               ),
        .dcache_bus_error_addr      ( dcache_bus_error_addr          ),
        `ifdef DCACHE_REPORT_PC
        .dcache_bus_error_pc        ( dcache_bus_error_pc            ),
        `endif
        // Breakpoint
        .bp_control                 ( bp_control                     ),
        .bp_address                 ( bp_address                     ),
        .debug_timing               ( debug_timing                   ),
        .update_ddata0              ( update_ddata0                  ),
        .ddata0_wdata               ( ddata0_wdata                   ),
        .bp_progress_save           ( mem_dcache_bp_triggered && mem_ctrl.gsc ),
        .read_ddata0                ( read_ddata0                    ),
        .halt                       ( halt                           ),
        .xcpt_traps_to_debug        ( xcpt_traps_to_debug            ),
        .resume                     ( resume                         ),
        .busy                       ( busy                           ),
        .ex_progbuf                 ( debug_ex_program_buffer        ),
        .debug_out_exception        ( debug_out_exception            ),
        // Fast Local Barrier request interface with the neigh
        .flb_neigh_req_valid        ( flb_neigh_req_valid            ),
        .flb_neigh_req_data         ( flb_neigh_req_data             ),
        // Fast Local Barrier response interface with the neigh
        .flb_neigh_resp_valid       ( flb_neigh_resp_valid           ),
        .flb_neigh_resp_data        ( flb_neigh_resp_data            ),
        // Fast Local Barrier interface with the RF write
        .flb_rf_wen_ready           ( wb_flb_wen_ready               ),
        .flb_rf_wen_valid           ( wb_flb_wen_valid               ),
        .flb_rf_wen_valid_early     ( wb_flb_wen_valid_early         ),
        .flb_rf_wen_thread_id       ( wb_flb_wen_thread_id           ),
        .flb_rf_wen_addr            ( wb_flb_wen_addr                ),
        .flb_rf_wen_data            ( wb_flb_wen_data                ),
        // Fast Local Barrier interface with the INT scoreboard
        .flb_scoreboard_valid       ( wb_flb_scoreboard_valid        ),
        .flb_scoreboard_addr        ( wb_flb_scoreboard_addr         ),
        .TE_prv                     ( TE_prv                         ),
        .TE_exception               ( TE_exception                   ),
        .TE_tval                    ( TE_tval                        ),
        .TE_wb_reg_cause_ie         ( TE_wb_reg_cause_ie             ),
        .te_thread_sel              ( te_thread_sel                  ),
        // m code instruction match
        .io_minstmask               ( minstmask_reg                  ),
        .io_minstmatch              ( minstmatch_reg                 ),
        // ESR configuration
        .esr_shire_coop_mode        ( esr_shire_coop_mode            ),
        .esr_features               ( esr_features                   ),
        // Error bits
        .tensor_load_err_flags      ( tensor_load_err_flags          ),
        .cache_ops_err_flags        ( cache_ops_err_flags            ),
        .tensor_reduce_err_flags    ( tensor_reduce_err_flags        ),
        // Gather/Scatter progress
        .gsc_progress               ( gsc_progress                   ),
        // PMU performance counters
        .pmu_count_up               ( pmu_count_up                   ),
        .pmu_read_data              ( pmu_read_data                  ),
        .pmu_read_sel               ( pmu_read_sel                   ),
        .pmu_write_en               ( pmu_write_en                   ),
        .pmu_write_data             ( pmu_write_data                 ),
        .pmu_neigh_event_sel        ( pmu_neigh_event_sel            ),
        // debug
        .dcache_debug_signals       ( dcache_debug_signals           )
        );


   // Combine events
   assign io_events_ext = {io_events_dcache_vpu,tbranch_up};

   always_comb
     begin
        // ID CSR info
        id_csr_en          = (id_ctrl.csr != CsrN);
        id_system_insn     = (id_ctrl.csr == CsrI);
        id_csr_ren         = ((id_ctrl.csr == CsrS) || (id_ctrl.csr == CsrC)) && (id_raddr1 == 5'b0);
        id_csr             = id_csr_ren ? CsrR : id_ctrl.csr;
        id_csr_addr        = csr_ad_t'(id_inst_bits[31:20]);
        id_csr_fflags      = id_csr_addr == csr_ad_FCSR || id_csr_addr == csr_ad_FFLAGS;
        id_csr_shared      = id_csr_addr == csr_ad_UCACHE_CONTROL || id_csr_addr == csr_ad_SATP ||
                             id_csr_addr == csr_ad_MATP           || id_csr_addr == csr_ad_MENABLE_SHADOWS ||
                             id_csr_addr == csr_ad_MCACHE_CONTROL;

        // List of CSR writes that are safe and can be done with the pipeline not
        // empty
        id_csr_wr_safe =
`ifdef DCACHE_CO_CSR_OLD_IMPLEMENTATION
                         (id_csr_addr == csr_ad_CO_PREFETCH_VA)     || // csr_ad_CO_PREFETCH_VA is former csr_ad_USR_CACHE_OP. This code is be removed ...
`else
                         (id_csr_addr == csr_ad_CO_EVICT_VA)        || // CacheOps
                         (id_csr_addr == csr_ad_CO_FLUSH_VA)        ||
                         (id_csr_addr == csr_ad_CO_LOCK_VA)         ||
                         (id_csr_addr == csr_ad_CO_UNLOCK_VA)       ||
                         (id_csr_addr == csr_ad_CO_PREFETCH_VA)     ||
`endif
                         (id_csr_addr == csr_ad_TENSOR_LOAD)        || // TensorLoad control
                         (id_csr_addr == csr_ad_TENSOR_FMA)         || // TensorFMA
                         (id_csr_addr == csr_ad_TENSOR_QUANT)       || // TensorQuant
                         (id_csr_addr == csr_ad_TENSOR_REDUCE)      || // Reduce
                         (id_csr_addr == csr_ad_TENSOR_STORE)       || // Tensor Store
                         (id_csr_addr == csr_ad_TENSOR_CONV_CTRL)   || // Convolution control
                         (id_csr_addr == csr_ad_TENSOR_WAIT)        || // Tensor wait
                         (id_csr_addr == csr_ad_TENSOR_COOPERATION) ||
                         (id_csr_addr == csr_ad_FLB)                || // Fast local barrier
                         (id_csr_addr == csr_ad_FCC);                  // Fast credit counter

        // List of CSR writes that modify VPU RF and have to stall following
        // instructions in case they read VPU registers
        id_csr_vpu      = id_csr_en && !id_csr_ren && ((id_csr_addr == csr_ad_TENSOR_REDUCE) || (id_csr_addr == csr_ad_TENSOR_FMA) || (id_csr_addr == csr_ad_TENSOR_QUANT) || (id_csr_addr == csr_ad_TENSOR_STORE));
        id_csr_vpu_qual = id_valid_qual && id_csr_vpu;

        // detect future write to exclusive mode => prevent gsc in other threads
        id_csr_excl_mode = id_csr_en && !id_csr_ren && id_csr_addr == csr_ad_EXCL_MODE;
        id_csr_flush = id_system_insn || (id_csr_en && !(id_csr_ren || id_csr_wr_safe)); // Flush for CSR writes that are not safe


        id_csr_flush_stall = ex_reg_valid && ex_thread_id == id_thread_id && ex_csr_flush    ||
                             tag_reg_valid && tag_thread_id == id_thread_id && tag_csr_flush ||
                             mem_reg_valid && mem_thread_id == id_thread_id && mem_csr_flush ||
                             wb_reg_valid && wb_thread_id == id_thread_id && wb_csr_flush;

        id_uses_alu = (!id_ctrl.alu_fn[4] || id_xcpt_pf || id_xcpt_af || id_bp_xcpt) && id_valid_qual; // if MSB of alu_dw either the mul/div is used, or nothing at all
        id_uses_rs1_all = id_valid_qual && id_ctrl.div;
        id_uses_rs1_lsb = id_valid_qual && ((id_ctrl.mem && dcache_cmd_is_gsc32(id_ctrl.mem_cmd)) || id_ctrl.div);
        id_uses_rs2 = id_valid_qual && (id_ctrl.div  || id_ctrl.mem && id_ctrl.rxs2);
     end

   // SATP and MAPT info
   assign {satp_info.mode, satp_info.ppn} = {satp.mode, satp.ppn};
   assign {matp_info.mode, matp_info.ppn} = {matp.mode, matp.ppn};

   // Useful status bits for VM
   assign vm_status = { compose_vm_status ( status[1],
                                            debug[1],
                                            prv[1]),
                        compose_vm_status ( status[0],
                                            debug[0],
                                            prv[0])
                        };

   /* verilator lint_off UNUSEDSIGNAL */  // VM-status composition intentionally reads only the privilege-relevant subset of the packed intpipe status struct.
   function automatic vm_status_t compose_vm_status;
      input intpipe_status_t st;
      input deb;
      input [1:0] pr;
      vm_status_t ret;
      begin
         ret = '{ prv: pr,
                  mprv: st.mprv,
                  mpp: st.mpp,
                  sum: st.sum,
                  mxr: st.mxr,
                  debug: deb };
         return ret;
      end
   endfunction
   /* verilator lint_on UNUSEDSIGNAL */

   ////////////////////////////////////////////////////////////////////////////////
   // Breakpoint unit for fetch
   ////////////////////////////////////////////////////////////////////////////////
   logic id_bp_timing;
   logic id_bp_enter_debug;
   logic id_bp_update_timing;

   debug_breakpoint #(.TriggerType(0)) fetch_bp
     (
      .control_ip(bp_control),
      .in_debug_ip(debug),
      .prv_ip(prv),
      .tdata2_ip(bp_address),
      .address_ip(id_pc),
      .cmd_ip( debug_bp_e'(3'h0) ),
      .valid_ip(id_valid_qual),
      .thread_id_ip(id_thread_id),
      .timing_op(id_bp_timing),
      .xcpt_op(id_bp_xcpt),
      .enter_debug_op(id_bp_enter_debug),
      .update_op(id_bp_update_timing)
      );


   ////////////////////////////////////////////////////////////////////////////////
   // id exceptions
   ////////////////////////////////////////////////////////////////////////////////
   // Checks if there's an exception and generates the cause
   logic [4:0] id_xcpt_cause; // Exception cause (xcpt_cause_e)
   logic id_illegal_insn;         // Illegal instruction detected by decode
   logic id_split_fetch_fault;    // fetch access or page fault exception.. in 2nd part of the word

   always_comb
     begin
        // Exception in instruction fetch if there's any page fault bit set from ibuf
        id_xcpt_pf = id_pf0 || id_pf1;
        id_xcpt_af = id_af0 || id_af1;
        id_split_fetch_fault = id_af1     ? !id_af0 :
                               id_bp_xcpt ? 1'b0    :  // because breakpoint is more prioritary than pf
                               id_pf1     ? !id_pf0 :
                               1'b0;

        // Illegal instruction
        id_illegal_insn = !id_ctrl.legal ||
                          id_ctrl.fp && !status[id_thread_id].fs || // if mstatus.fs=OFF, illegal ins if attempting to use fp
                          id_ctrl.gfx && esr_features.trap_on_gfx;      // gfx instruction not allowed

        // Prioritizes the exceptions
        // By default we set the lowest prio exception case
        id_xcpt = 1'b1;
        id_xcpt_cause = XcptMcodeInstruction;

        if(id_csr_interrupt[id_thread_id])           // First CSR interrupt
          id_xcpt_cause = id_csr_interrupt_cause[id_thread_id];
        else if(id_bp_enter_debug)                        // Then breakpoint debug
          id_xcpt_cause = XcptDebug;
        else if(id_bp_xcpt)                               // Then breakpoint exception
          id_xcpt_cause = XcptBreakpoint;
        else if(id_xcpt_af & id_valid)                    // Then instruction access fault
          id_xcpt_cause = XcptFetchAccess;
        else if(id_xcpt_pf & id_valid)                    // Then instruction page fault
          id_xcpt_cause = XcptFetchPageFault;
        else if (id_fetch_bus_error & id_valid)
          id_xcpt_cause = XcptFetchBusError;
        else if (id_fetch_ecc_error & id_valid)
          id_xcpt_cause = XcptFetchEccError;
        else if(id_illegal_insn & id_valid)               // Then illegal instruction
          id_xcpt_cause = XcptIllegalInstruction;
        else if((id_ctrl.mcode | minst_match) & id_valid) // If not M-code instruction, then there's no exception
          id_xcpt_cause = XcptMcodeInstruction;
        else
          id_xcpt = 1'b0;
     end


   ////////////////////////////////////////////////////////////////////////////////
   // Stall for RAW/WAW hazards on CSRs, LB/LH, and mul/div in TAG stage
   ////////////////////////////////////////////////////////////////////////////////

   // Computes list of potential hazards for float, int and mask
   logic [3:0][XregAddrSize-1:0]  id_fp_hazard_addr;    // What is the float reg with the potential hazard
   logic [2:0][XregAddrSize-1:0]  id_int_hazard_addr;   // What is the integer reg with the potential hazard
   logic [VpuRegmaskNum-1:0]      id_mask_raddr_hazard; // List of mask entries with hazards
   logic [VpuRegmaskNum-1:0]      id_mask_waddr_hazard; // List of mask entries with hazards
   logic [3:0]                    id_fp_hazard_en;      // Is there a potential hazard for float reg
   logic [2:0]                    id_int_hazard_en;     // Is there a potential hazard for integer reg
   logic [XregAddrSize-1:0]       id_raddr3;            // Addr for 3rd source

   logic [3:0]                    intpipe_trans_busy;   // signal is non zero if there's the end of a trans unit (when flushing) in parallel to the intpipe (bit0=ex, bit1=tag...)
   logic [3:0]                    intpipe_trans_busy_next; // it is required to stall CSR access => the trans unit can change flags, FS dirty state... and the CSR can clear them

   always_comb
     begin
        // Gets the enable and addrs for the integer hazards
        id_int_hazard_en = { id_ctrl.rxs1 && (id_raddr1 != 5'b0),
                             id_ctrl.rxs2 && (id_raddr2 != 5'b0),
                             id_ctrl.wxd  && (id_waddr  != 5'b0) };
        id_int_hazard_addr = { id_raddr1, id_raddr2, id_waddr };

        // Gets the enable and addrs for the float hazards
        id_raddr3         = id_frontend_vpu_ctrl.rend ? id_raddr3_sc : id_raddr3_tmp; // If it is a GS src3 comes from different position
        id_fp_hazard_en   = { id_frontend_vpu_ctrl.ren1, id_frontend_vpu_ctrl.ren2, id_frontend_vpu_ctrl.ren3, id_frontend_vpu_ctrl.wen };
        id_fp_hazard_addr = { id_raddr1, id_raddr2, id_raddr3, id_waddr };
        // One hot of mask hazards
        id_mask_raddr_hazard  = {VpuRegmaskNum{id_frontend_vpu_ctrl.mallren && id_ctrl.fp}};                    // All read set the hazard to all entries
        id_mask_raddr_hazard |= {{(VpuRegmaskNum-1){1'b0}}, id_frontend_vpu_ctrl.m0ren && id_ctrl.fp};                   // M0 read for regular instructions
        id_mask_raddr_hazard |= {{(VpuRegmaskNum-1){1'b0}}, id_frontend_vpu_ctrl.mren1 && id_ctrl.fp} << id_raddr1[2:0]; // Read port 1
        id_mask_raddr_hazard |= {{(VpuRegmaskNum-1){1'b0}}, id_frontend_vpu_ctrl.mren2 && id_ctrl.fp} << id_raddr2[2:0]; // Read port 2
        id_mask_waddr_hazard = {VpuRegmaskNum{id_ctrl.wmad}};      // All write set the hazard to all entries
        id_mask_waddr_hazard |= {{(VpuRegmaskNum-1){1'b0}}, id_ctrl.wmd} << id_waddr[2:0]; // Write port

        if (id_vpu_ctrl.trans_busy)
          intpipe_trans_busy_next = 4'b1111;
        else
          intpipe_trans_busy_next = {intpipe_trans_busy[2:0], 1'b0};
     end
   always_ff @(posedge clock) if (reset_w) intpipe_trans_busy <= '0; else if (id_vpu_ctrl.trans_busy | intpipe_trans_busy[3]) intpipe_trans_busy <= intpipe_trans_busy_next;

   // EX to ID hazards
   logic id_cannot_bypass_ex;    // ID stage cannot bypass from EX
   logic id_int_data_hazard_ex;  // There's an integer data hazard between EX and ID stage
   logic id_fp_data_hazard_ex;   // There's a float data hazard between EX and ID stage
   logic id_mask_data_hazard_ex; // There's a mask data hazard between EX and ID stage
   logic id_hazard_ex;           // There's a hazard from EX to ID

   always_comb
     begin
        // Computes if EX can bypass to ID
        id_cannot_bypass_ex = ex_csr_en ||                     // CSR instructions
                              ex_ctrl.jalr || ex_ctrl.mem ||   // JALR and MEM
                              ex_ctrl.div || ex_ctrl.fp;       // DIV and FP

        // Checks if there's an integer hazard issue
        id_int_data_hazard_ex = 1'b0;
        for(integer i = 0; i < 3; i++)
          id_int_data_hazard_ex |= id_int_hazard_en[i] && (id_int_hazard_addr[i] == ex_waddr);
        id_int_data_hazard_ex &= ex_ctrl.wxd;

        // Checks if there's a float hazard issue
        id_fp_data_hazard_ex = 1'b0;
        // WAW hazard (only if ID inst is a load, which might be faster, or EX inst is a load, which might be slower)
        id_fp_data_hazard_ex |= id_fp_hazard_en[0] && (id_fp_hazard_addr[0] == ex_waddr) && (id_ctrl.mem || ex_ctrl.mem);
        // RAW hazards
        for(integer i = 1; i <= 3; i++)
          id_fp_data_hazard_ex |= id_fp_hazard_en[i] && (id_fp_hazard_addr[i] == ex_waddr);
        id_fp_data_hazard_ex &= ex_ctrl.wfd && id_ctrl.fp;

        // Checks if there's a mask hazard issue
        id_mask_data_hazard_ex = |((( {{(VpuRegmaskNum-1){1'b0}}, ex_ctrl.wmd} << ex_waddr[2:0]) | {VpuRegmaskNum{ex_ctrl.wmad}}) & id_mask_raddr_hazard);
        // Can't write mask if there's a mask read all to X
        id_mask_data_hazard_ex |= ex_ctrl.rma && (id_ctrl.wmd || id_ctrl.wmad);
        //Can't write mask if there's a mask comp
        id_mask_data_hazard_ex |= |((( {{(VpuRegmaskNum-1){1'b0}}, ex_ctrl.wmd & ex_vpu_ctrl.tointm} << ex_waddr[2:0])) & id_mask_waddr_hazard);
        // EX to ID result
        id_hazard_ex = (ex_reg_valid || gsc_reg_valid) && (id_int_data_hazard_ex && id_cannot_bypass_ex || id_fp_data_hazard_ex || id_mask_data_hazard_ex) && (id_thread_id == ex_thread_id);
     end

   // TAG to ID hazards
   logic id_cannot_bypass_tag;    // ID stage cannot bypass from TAG
   logic id_int_data_hazard_tag;  // There's an integer data hazard between TAG and ID stage
   logic id_fp_data_hazard_tag;   // There's a float data hazard between TAG and ID stage
   logic id_hazard_tag;           // There's a hazard from TAG to ID
   logic id_mask_data_hazard_tag; // There's a mask data hazard between TAG and ID stage

   always_comb
     begin
        // Computes if TAG can bypass to ID
        id_cannot_bypass_tag = (tag_ctrl.csr != CsrN) // CSR instructions
          || tag_ctrl.mem                      // Dcache bypass
                               || tag_ctrl.div || tag_ctrl.fp;      // Div or FP

        // Checks if there's an integer hazard issue
        id_int_data_hazard_tag = 1'b0;
        for(integer i = 0; i < 3; i++)
          id_int_data_hazard_tag |= id_int_hazard_en[i] && (id_int_hazard_addr[i] == tag_waddr);
        id_int_data_hazard_tag &= tag_ctrl.wxd;

        // Checks if there's a float hazard issue
        id_fp_data_hazard_tag = 1'b0;
        // WAW hazard (only if ID inst is a load, which might be faster, or TAG inst is a load, which might be slower)
        id_fp_data_hazard_tag |= id_fp_hazard_en[0] && (id_fp_hazard_addr[0] == tag_waddr) && (id_ctrl.mem || tag_ctrl.mem);
        // RAW hazards (only if WB inst is load, FMA, which can write after WB)
        for(integer i = 1; i <= 3; i++)
          id_fp_data_hazard_tag |= id_fp_hazard_en[i] && (id_fp_hazard_addr[i] == tag_waddr) && (tag_ctrl.mem || tag_vpu_ctrl.fma);
        id_fp_data_hazard_tag &= tag_ctrl.wfd && id_ctrl.fp;

        // Checks if there's a mask hazard issue
        id_mask_data_hazard_tag = |((( {{(VpuRegmaskNum-1){1'b0}}, tag_ctrl.wmd} << tag_waddr[2:0]) | {VpuRegmaskNum{tag_ctrl.wmad}}) & id_mask_raddr_hazard);
        // Only RAW hazard if EX inst is a comparison instruction writing to mask
        // Can't write mask if there's a mask read all to X
        id_mask_data_hazard_tag |= tag_ctrl.rma && (id_ctrl.wmd || id_ctrl.wmad);
        //Can't write mask if there's a comparison instruction
        id_mask_data_hazard_tag |= |((( {{(VpuRegmaskNum-1){1'b0}}, tag_ctrl.wmd & tag_vpu_ctrl.tointm} << tag_waddr[2:0])) & id_mask_waddr_hazard);

        // TAG to ID result
        id_hazard_tag = tag_reg_valid && ((id_int_data_hazard_tag && id_cannot_bypass_tag) || id_fp_data_hazard_tag || id_mask_data_hazard_tag) && (id_thread_id == tag_thread_id);
     end

   // MEM to ID hazards
   logic id_cannot_bypass_mem;    // ID stage cannot bypass from MEM
   logic id_int_data_hazard_mem;  // There's an integer data hazard between MEM and ID stage
   logic id_fp_data_hazard_mem;   // There's a float data hazard between MEM and ID stage
   logic id_hazard_mem;           // There's a hazard from MEM to ID
   logic id_mask_data_hazard_mem; // There's a mask data hazard between MEM and ID stage

   always_comb
     begin
        // Computes if MEM can bypass to ID
        id_cannot_bypass_mem = (mem_ctrl.csr != CsrN)  || // CSR instructions
                               mem_ctrl.mem ||                       // Dcache bypass
                               mem_ctrl.div || mem_ctrl.fp;          // Div or FP

        // Checks if there's an integer hazard issue
        id_int_data_hazard_mem = 1'b0;
        for(integer i = 0; i < 3; i++)
          id_int_data_hazard_mem |= id_int_hazard_en[i] && (id_int_hazard_addr[i] == mem_waddr);
        id_int_data_hazard_mem &= mem_ctrl.wxd;

        // Checks if there's a float hazard issue
        id_fp_data_hazard_mem = 1'b0;
        // WAW hazard (only if ID inst is a load, which might be faster, or MEM inst is a load, which might be slower)
        id_fp_data_hazard_mem |= id_fp_hazard_en[0] && (id_fp_hazard_addr[0] == mem_waddr) && (id_ctrl.mem || mem_ctrl.mem);
        // RAW hazards (only if WB inst is load, FMA which can write after WB)
        for(integer i = 1; i <= 3; i++)
          id_fp_data_hazard_mem |= id_fp_hazard_en[i] && (id_fp_hazard_addr[i] == mem_waddr) && (mem_ctrl.mem || mem_vpu_ctrl.fma);
        id_fp_data_hazard_mem &= mem_ctrl.wfd && id_ctrl.fp;

        // Checks if there's a mask hazard issue
        id_mask_data_hazard_mem = |((( {{(VpuRegmaskNum-1){1'b0}}, mem_ctrl.wmd} << mem_waddr[2:0]) | {VpuRegmaskNum{mem_ctrl.wmad}}) & id_mask_raddr_hazard);
        // Can't write mask if there's a mask read all to X
        id_mask_data_hazard_mem |= mem_ctrl.rma && (id_ctrl.wmd || id_ctrl.wmad);
        //Can't write mask if there's a comparison instruction
        id_mask_data_hazard_mem |= |((( {{(VpuRegmaskNum-1){1'b0}}, mem_ctrl.wmd & mem_vpu_ctrl.tointm} << mem_waddr[2:0])) & id_mask_waddr_hazard);

        // MEM to ID result
        id_hazard_mem = mem_reg_valid && ((id_int_data_hazard_mem && id_cannot_bypass_mem) || id_fp_data_hazard_mem || id_mask_data_hazard_mem) && (id_thread_id == mem_thread_id);
     end

   // WB to ID hazards
   logic          id_cannot_bypass_wb;    // ID stage cannot bypass from WB
   logic          id_int_data_hazard_wb;  // There's an integer data hazard between WB and ID stage
   logic          id_fp_data_hazard_wb;   // There's a float data hazard between WB and ID stage
   logic          id_hazard_wb;           // There's a hazard from WB to ID
   logic          id_mask_data_hazard_wb; // There's a mask data hazard between WB and ID stage

   always_comb
     begin
        // Computes if MEM can bypass to ID
        id_cannot_bypass_wb = (wb_ctrl.csr != CsrN) || // CSR instructions
                              wb_ctrl.div || wb_ctrl.fp;       // Div or FP

        // Checks if there's an integer hazard issue
        id_int_data_hazard_wb = 1'b0;
        for(integer i = 0; i < 3; i++)
          id_int_data_hazard_wb |= id_int_hazard_en[i] && (id_int_hazard_addr[i] == wb_waddr);
        id_int_data_hazard_wb &= wb_ctrl.wxd;

        // Checks if there's a float hazard issue
        id_fp_data_hazard_wb = 1'b0;
        // WAW hazard (only if ID inst is a load, which might be faster, or WB inst is load, which might be slower)
        id_fp_data_hazard_wb |= id_fp_hazard_en[0] && (id_fp_hazard_addr[0] == wb_waddr) && (id_ctrl.mem || wb_vpu_ctrl.fma || wb_ctrl.mem);
        // RAW hazards (only if WB inst is load which can write after WB)
        for(integer i = 1; i <= 3; i++)
          id_fp_data_hazard_wb |= id_fp_hazard_en[i] && (id_fp_hazard_addr[i] == wb_waddr) && wb_vpu_ctrl.fma;
        id_fp_data_hazard_wb &= wb_ctrl.wfd && id_ctrl.fp;

        // Checks if there's a mask hazard issue
        id_mask_data_hazard_wb = |((( {{(VpuRegmaskNum-1){1'b0}}, wb_ctrl.wmd} << wb_waddr[2:0]) | {VpuRegmaskNum{wb_ctrl.wmad}}) & id_mask_raddr_hazard);
        // Can't write mask if there's a mask read all to X
        id_mask_data_hazard_wb |= wb_ctrl.rma && (id_ctrl.wmd || id_ctrl.wmad);
        //Can't write mask if there's a comparison instruction
        id_mask_data_hazard_wb |= |((( {{(VpuRegmaskNum-1){1'b0}}, wb_ctrl.wmd & wb_vpu_ctrl.tointm} << wb_waddr[2:0])) & id_mask_waddr_hazard);

        // WB to ID result
        id_hazard_wb = wb_reg_valid && ((id_int_data_hazard_wb && id_cannot_bypass_wb) || id_fp_data_hazard_wb || id_mask_data_hazard_wb) && (id_thread_id == wb_thread_id);
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Integer scoreboard that stores which integer registers have long latency
   // results pending to be written
   ////////////////////////////////////////////////////////////////////////////////

   minion_reg_dest_t wb_div_resp_dest;       // Destination in WB stage from div unit
   logic [2:0]       id_int_sboard_bits;     // Scoreboard bits for the 3 registers of int
   logic [2:0]       id_int_sboard_dc_div;   // Scoreboard bits for the 3 registers of int
   logic             id_int_sboard_x31;      // Scoreboard bit for register X31
   logic             id_inst_requires_x31;   // Instructions requires implicit usage of X31
   logic             id_int_sboard_hazard;   // There's an integer hazard with the scoreboard
   logic             ex_div_ready;           // Div unit is ready to accept new requests
   logic             wb_div_wen;             // Write enable due a divide

   intpipe_int_scoreboard
     #(
       .ReadPorts        ( 3                       )
       )
   scoreboard_int
     (
      // Read port
      .rd_thread_id      ( id_thread_id            ),
      .rd_addr           ( id_int_hazard_addr      ),
      .rd_data           ( id_int_sboard_bits      ),
      .rd_data_dcache_div( id_int_sboard_dc_div    ),
      .rd_x31            ( id_int_sboard_x31       ),
      // Scoreboard from different units
      .dcache_scoreboard ( id_dcache_scoreboard    ),
      .int_div_valid     ( !ex_div_ready           ),
      .int_div_dest      ( wb_div_resp_dest        ),
      .int_flb_valid     ( wb_flb_scoreboard_valid ),
      .int_flb_dest      ( wb_flb_scoreboard_addr  ),
      .vpu_scoreboard    ( id_vpu_ctrl.scoreboard  )
      );

   assign id_inst_requires_x31 = id_ctrl.x31;
   assign id_int_sboard_hazard = |(id_int_sboard_bits & id_int_hazard_en) || (id_int_sboard_x31 && id_inst_requires_x31);

   ////////////////////////////////////////////////////////////////////////////////
     // Float scoreboard that stores which float registers have long latency
   // results pending to be written
   ////////////////////////////////////////////////////////////////////////////////

   logic [3:0]     id_fp_sboard_bits;        // Scoreboard bits for FP registers
   logic [3:0]     id_fp_sboard_bits_dcache; // Scoreboard bits for FP registers
   logic           id_stall_vpu;             // Stall due VPU scoreboard
   logic           ex_csr_vpu;               // EX has a CSR write that will change VPU contents
   logic           tag_csr_vpu;              // TAG has a CSR write that will change VPU content
   logic           mem_csr_vpu;              // MEM has a CSR write that will change VPU content
   logic           wb_csr_vpu;               // WB has a CSR write that will change VPU content
   logic           wb_pre_valid;             // Valid instruction in WB stage before clearing due G/S (needed in minion monitor!!)

   intpipe_fp_scoreboard
     #(
       .ReadPorts        ( 4                       )
       )
   scoreboard_fp
     (
      // Read port
      .rd_thread_id      ( id_thread_id             ),
      .rd_addr           ( id_fp_hazard_addr        ),
      .rd_data           ( id_fp_sboard_bits        ),
      .rd_data_dcache    ( id_fp_sboard_bits_dcache ),
      // Scoreboard from different units
      .dcache_scoreboard ( id_dcache_scoreboard     ),
      .vpu_scoreboard    ( id_vpu_ctrl.scoreboard   )
      );

   always_comb
     begin
        id_pending_scoreboard_t0 = 1'b0;
        // Gets for outstanding writes in dcache
        for(integer i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++)
          id_pending_scoreboard_t0 |= id_dcache_scoreboard.valid[i] && id_dcache_scoreboard.dest[i].fp && !id_dcache_scoreboard.dest[i].thread_id;

        // VPU outstanding operations
        for(integer i = 0; i < VpuTransScoreboardSize; i++)
          id_pending_scoreboard_t0 |= id_vpu_ctrl.scoreboard.fp_valid[i]; //Both threads have to be checked, as the tensor instructions interfere with the trans sequencer
        for(integer i = VpuTransScoreboardSize; i < VpuFpScoreboardSize; i++)
            id_pending_scoreboard_t0 |= id_vpu_ctrl.scoreboard.fp_valid[i] && !id_vpu_ctrl.scoreboard.fp_dest[i].thread_id; //Both threads have to be checked, as the tensor instructions interfere with the trans sequencer
        // Added to detect pending instructions wihile trans is inserting
        // without adding an extra scoreboard slot
        id_pending_scoreboard_t0 |=id_vpu_ctrl.id_trans_insert;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Float scoreboard that stores which mask registers have long latency results
   // pending to be written
   ////////////////////////////////////////////////////////////////////////////////

   logic [VpuRegmaskNum-1:0] id_mask_sboard_raddr_bits;     // Scoreboard bits for mask registers
   logic [VpuRegmaskNum-1:0] id_mask_sboard_waddr_bits;     // Scoreboard bits for mask registers

   intpipe_mask_scoreboard
     scoreboard_mask
       (
        // Read port
        .rd_thread_id      ( id_thread_id              ),
        .rd_addr           ( id_mask_raddr_hazard      ),
        .wd_addr           ( id_mask_waddr_hazard      ),
        .rd_data           ( id_mask_sboard_raddr_bits ),
        .wd_data           ( id_mask_sboard_waddr_bits ),
        // Scoreboard from vpu unit
        .vpu_scoreboard    ( id_vpu_ctrl.scoreboard    )
        );

   always_comb
     begin
        // Computes ID stall due FP
        // Checks for scoreboard bits, FCSR and ML module.
        id_stall_vpu  = |(id_fp_sboard_bits & id_fp_hazard_en);
        id_stall_vpu |= |(id_mask_sboard_raddr_bits);
        id_stall_vpu |= |(id_mask_sboard_waddr_bits);
        id_stall_vpu |= id_vpu_ctrl.tfma_wrrf_enabled || vpu_ctrl.tensorfma_start;  // TensorFMA  (only for ones that use the VPU RF)
        id_stall_vpu |= (id_vpu_ctrl.tquant_enabled || vpu_ctrl.tensorquant_start); // TensorQuant
        id_stall_vpu |= id_vpu_ctrl.reduce_enabled;                                 // TensorReduce/TensorStore
        id_stall_vpu |= ex_csr_vpu || tag_csr_vpu || mem_csr_vpu || wb_csr_vpu;     // Tensor* going to VPU
     end

   logic id_vpu_ctrl_mread;

   //id vpu instructions using vpu masks
   assign id_vpu_ctrl_mread = id_frontend_vpu_ctrl.m0ren || id_frontend_vpu_ctrl.mallren || id_frontend_vpu_ctrl.mren1 || id_frontend_vpu_ctrl.mren2;

   ////////////////////////////////////////////////////////////////////////////////
   // Final stall and valid generation
   ////////////////////////////////////////////////////////////////////////////////

   logic id_div_req;                  // Request to div unit in next cycle
   logic id_take_pc;                  // Redirecting to a new PC
   logic id_ctrl_stall;               // ID is stalled
   logic ex_div_req;                  // Request to div unit this cycle and previous
   logic ex_ctrl_fp_toint;            // FP to int instruction in EX
   logic tag_ctrl_fp_toint;           // FP to int instruction in TAG
   logic mem_ctrl_fp_toint;           // FP to int instruction in MEM
   logic wb_div_resp_ready;           // Div unit can write results
   logic wb_div_resp_valid;           // Div unit has new results
   logic intpipe_is_gscing;           // The pipeline is processing a gather/scatter instruction

   logic id_ctrl_stall_hazard;
   logic id_ctrl_stall_hazard_vpu;
   logic id_ctrl_stall_div;
   logic id_ctrl_stall_div_dist; // stall because both threads wanted to use the mul/div... once it becomes available, assign fairly the div to threads
   logic id_ctrl_stall_trans;
   /* verilator lint_off UNOPTFLAT */  // The trans-starvation helper feeds the preserved counter next-state cone through a real flop, but Verilator reports a false flattened cycle at full minion_top hierarchy.
   logic id_ctrl_stall_trans_starve;
   /* verilator lint_on UNOPTFLAT */
   logic id_ctrl_stall_dcache;
   logic id_ctrl_stall_csr;
   logic id_stall_fp2int;
   logic id_stall_gsc_progress;
   logic [NrThreads-1:0] muldiv_needed;
   logic                  next_muldiv_thread;

   always_comb begin
      // Computes all the reasons why an instruction needs to stay in ID
      id_ctrl_stall_hazard      = id_hazard_ex || id_hazard_tag || id_hazard_mem || id_hazard_wb;                            // Hazards from next stages
      id_ctrl_stall_div         = id_ctrl.div && (!ex_div_ready || (wb_div_resp_valid && !wb_div_resp_ready) || ex_div_req); // Don't issue Div if unit is not ready
      id_ctrl_stall_div_dist    = id_ctrl.div && muldiv_needed[!id_thread_id] && id_thread_id != next_muldiv_thread;         // the other thread has been waiting for longer for the muldiv
      id_ctrl_stall_hazard_vpu  =  id_ctrl.fp && (id_stall_vpu ||                                                             // VPU hazards for regular VPU operations
                                   id_vpu_ctrl_mread && wb_vpu_ctrl.mova_mx);                                                 // stall because of masks
      id_ctrl_stall_trans       = id_vpu_ctrl.id_trans_insert && id_ctrl.fp  ||                                                     // If trans is going to use the WB slot, stall FP instructions that are not loads (have a dedicated WB port) or trascendentals (they write later)
                                  id_vpu_ctrl.trans_busy && id_ctrl.fp && id_ctrl.gsc;                                       // If trans is going to use the WB slot, stall FP instructions that are not loads (have a dedicated WB port) or trascendentals (they write later)
      id_ctrl_stall_dcache      = !id_dcache_ready && id_ctrl.mem;                                                           // Replay queue in Dcache is full

      id_ctrl_stall_csr         = (id_csr_en && !id_csr_wr_safe &&                                                           // Wait for pipe to drain before writing CSR (some CSRs are safe though)
                                   ( ex_reg_valid  && (ex_thread_id == id_thread_id)  ||
                                     tag_reg_valid && (tag_thread_id == id_thread_id) ||
                                     mem_reg_valid && (mem_thread_id == id_thread_id) ||
                                     wb_reg_valid  && (wb_thread_id == id_thread_id)  ||
                                     id_vpu_ctrl.trans_busy  || intpipe_trans_busy[3] ||
                                     id_csr_fflags && id_vpu_ctrl.fflags_stall)) ||
                                  id_csr_flush_stall ||                                                                      // wait for CSR insn to go through
                                  id_csr_shared && ex_reg_valid && ex_csr_en;                                                // add bubble for shared CSRs that are not RO

      id_stall_fp2int           = id_ctrl.wxd && (!id_ctrl.fp && ex_reg_valid && ex_ctrl_fp_toint ||                         // FP to int instruction
                                                  tag_reg_valid && tag_ctrl_fp_toint ||
                                                  !id_ctrl.fp && mem_reg_valid && mem_ctrl_fp_toint ||
                                                  wb_dcache_fp_toint);
      id_stall_gsc_progress     = intpipe_is_gscing && (gsc_progress[id_thread_id] != CoreGscCntBits'(0)) && id_ctrl.gsc;


      id_ctrl_stall = id_ctrl_stall_hazard       ||
                      id_ctrl_stall_hazard_vpu   ||
                      id_int_sboard_hazard       || // Long latency dependency
                      id_ctrl_stall_div          ||
                      id_ctrl_stall_div_dist     ||
                      id_ctrl_stall_trans        ||
                      id_ctrl_stall_trans_starve ||
                      id_do_fence                || // Instruction needs to fence
                      id_ctrl_stall_dcache       ||
                      id_ctrl_stall_csr          ||
                      csr_fe_stall[id_thread_id] || // this one will kill instructions just after WFIs
                      ex_gsc_busy                || // EX/GSC is busy working on several elements of a G/S
                      id_stall_fp2int            ||
                      id_stall_gsc_progress;

      // Computes kill and valid to EX
      /* verilator lint_off UNOPTFLAT */  // ID valid/kill folds through the preserved hazard cone and trips a false flattened cycle in full-core lint.
      id_ctrl_kill     = id_inst_replay || id_take_pc || id_ctrl_stall || id_csr_interrupt[id_thread_id] || csr_excl_mode && csr_excl_mode_sel != id_thread_id || csr_excl_mode_stall;
      id_valid_qual    = id_valid && !id_ctrl_kill;
      id_fe_resp_ready = id_valid_qual;
      /* verilator lint_on UNOPTFLAT */
   end


   // avoid thread starvation because of the trans unit being busy
   logic [NrThreads-1:0][IdCtrlStallCntSz-1:0]   id_ctrl_stall_trans_cnt;
   logic [NrThreads-1:0][IdCtrlStallCntSz-1:0]   id_ctrl_stall_trans_cnt_next;
   logic [NrThreads-1:0]                          id_ctrl_stall_trans_cnt_maxed;


   always_comb begin
      for ( int i = 0 ; i < NrThreads; i++)
        id_ctrl_stall_trans_cnt_maxed[i] = &id_ctrl_stall_trans_cnt[i];

      //default value
      id_ctrl_stall_trans_cnt_next = id_ctrl_stall_trans_cnt;

      // reset both counters if in exclusive mode
      // or if one of the threads is disabled
      if ( csr_excl_mode || !thread0_enable || !thread1_enable)
        id_ctrl_stall_trans_cnt_next = '0;
      // instruction issued or trans no longer busy clear the counter or thread sleeping
      else if (id_valid_qual || !id_vpu_ctrl.trans_busy && !intpipe_trans_busy[3])
        id_ctrl_stall_trans_cnt_next[id_thread_id] = '0;
      // new stall because of trans (and counter not already all 1's)
      else if (!id_ctrl_stall_trans_cnt_maxed[id_thread_id] &&
               (id_ctrl_stall_trans
              ||(id_csr_en && !id_csr_wr_safe && (id_vpu_ctrl.trans_busy  || intpipe_trans_busy[3])) //Unsafe CSR write
              ||(id_vpu_ctrl.tfma_enabled && id_pending_scoreboard_t0))) //Tensor stalled
        id_ctrl_stall_trans_cnt_next[id_thread_id] = id_ctrl_stall_trans_cnt[id_thread_id] + 1'b1;

      // reset counter if trans longer needed (jumping (e.g. thread asleep or jumping)
      for ( int i = 0 ; i < NrThreads; i++)
        if (id_fe_stall[i] || id_fe_req_valid[i])
          id_ctrl_stall_trans_cnt_next[i] = '0;

      // if trans instruction in id, and other thread has not issued and instruction because trans
      // was in used, do not issue the instruction in order to avoid starving the other thread
      // Note: casex with don't-care patterns; using wildcard equality in SV
      if      ((id_inst_bits & 32'b111111111111_00000_111_00000_1111111) == (FRCP_PS       & 32'b111111111111_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else if ((id_inst_bits & 32'b111111111111_00000_111_00000_1111111) == (FRSQ_PS       & 32'b111111111111_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else if ((id_inst_bits & 32'b111111111111_00000_111_00000_1111111) == (FLOG_PS       & 32'b111111111111_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else if ((id_inst_bits & 32'b111111111111_00000_111_00000_1111111) == (FEXP_PS       & 32'b111111111111_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else if ((id_inst_bits & 32'b111111111111_00000_111_00000_1111111) == (FSIN_PS       & 32'b111111111111_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else if ((id_inst_bits & 32'b1111111_00000_00000_111_00000_1111111) == (FRCP_FIX_RAST & 32'b1111111_00000_00000_111_00000_1111111))
        id_ctrl_stall_trans_starve = id_ctrl_stall_trans_cnt_maxed[!id_thread_id];
      else
        id_ctrl_stall_trans_starve = 1'b0;

   end
   always_ff @(posedge clock) if (reset_w) id_ctrl_stall_trans_cnt <= '0; else id_ctrl_stall_trans_cnt <= id_ctrl_stall_trans_cnt_next;



   ////////////////////////////////////////////////////////////////////////////////
   // Request to front end for next fetch
   ////////////////////////////////////////////////////////////////////////////////

   logic [PcSize-2:0]           ipi_pc_reg;
   logic [PcSize-1:0]           ipi_pc;
   logic [PcSizeExt-1:0]        tag_npc;       // Next PC computed in TAG stage
   logic                        tag_take_pc;   // TAG stage is changed the PC flow
   logic                        wb_reg_replay; // Replay instruction in WB
   logic                        wb_xcpt;       // Exception in the instruction

   //     CLK    EN                       DOUT         DIN
   always_ff @(posedge clock) ipi_with_pc <= interrupts.ipi_with_pc;
   always_ff @(posedge clock) if (|interrupts.ipi_with_pc) ipi_pc_reg <= interrupts.ipi_pc;
   assign  ipi_pc = {ipi_pc_reg, 1'b0}; // add LSB that is fixed to 0

   always_comb begin
      for ( int i = 0; i < NrThreads; i++) begin
         id_redirect_allowed[i] = (prv[i] == PrvU) && id_csr_stall[i];
      end
      id_bad_redirect = ipi_with_pc & ~id_redirect_allowed;
      id_take_redirect = ipi_with_pc & id_redirect_allowed;
   end


   // stalls to FE
   // if an instruction is not accepted, tell FE not to send more data for this thread
   // until the cause for stall has changed
   logic [NrThreads-1:0]                     fence_fe_stall, fence_fe_stall_next;
   logic                                     new_id_fp_stall_dcache;
   logic [NrThreads-1:0]                     fp_sboard_fe_stall, fp_sboard_fe_stall_next;
   logic [NrThreads-1:0]                     int_sboard_fe_stall, int_sboard_fe_stall_next;
   logic                                     div_kill;             // Send a kill to the instruction in div
   logic                                     ex_div_ready_prev;
   logic [NrThreads-1:0] muldiv_needed_next;

   always_ff @(posedge clock) if (reset_w) fence_fe_stall <= NrThreads'(0); else fence_fe_stall <= fence_fe_stall_next;
   always_ff @(posedge clock) if (reset_w) fp_sboard_fe_stall <= NrThreads'(0); else fp_sboard_fe_stall <= fp_sboard_fe_stall_next;
   always_ff @(posedge clock) if (reset_w) int_sboard_fe_stall <= NrThreads'(0); else int_sboard_fe_stall <= int_sboard_fe_stall_next;
   always_ff @(posedge clock) if (reset_w) muldiv_needed <= NrThreads'(0); else muldiv_needed <= muldiv_needed_next;
   always_ff @(posedge clock) ex_div_ready_prev <= ex_div_ready;
   always_ff @(posedge clock) if (reset_w) next_muldiv_thread <= 1'b0; else if (ex_div_req) next_muldiv_thread <= !ex_thread_id;

   always_comb begin
      muldiv_needed_next = muldiv_needed & ~id_fe_stall & ~id_fe_req_valid;

      // reset if in exclusive mode or if one of the threads is disabled
      if ( csr_excl_mode || !thread0_enable || !thread1_enable) muldiv_needed_next = '0;
      else if ( id_valid && (id_ctrl_stall_div || id_ctrl_stall_div_dist)) muldiv_needed_next[id_thread_id] = 1'b1;
      else if (id_valid_qual) muldiv_needed_next[id_thread_id] = 1'b0;

      // a fence stalls => wait for dcache not to be busy
      fence_fe_stall_next = fence_fe_stall;  //keep value by default
      if ( id_do_fence && id_valid ) fence_fe_stall_next[id_thread_id] = 1'b1; // set to 1 on the first fence
      fence_fe_stall_next &= id_dcache_busy & ~id_fe_req_valid;    // clear stall

      int_sboard_fe_stall_next = int_sboard_fe_stall;
      // stall because of int scoreboard from dcache or div
      if ( (|(id_int_sboard_dc_div & id_int_hazard_en) && id_valid) )
        int_sboard_fe_stall_next[id_thread_id] = 1'b1; // set to 1 on the first stall
      // retry if div finished or int sb from dcache changed
      int_sboard_fe_stall_next &= ~id_dcache_sb_int_dealloc;
      if ( ex_div_ready && !ex_div_ready_prev) int_sboard_fe_stall_next = '0;


      // stall because of dcache fp sboard => wait until there is a change in the dcache sb
      fp_sboard_fe_stall_next = fp_sboard_fe_stall;
      new_id_fp_stall_dcache = |(id_fp_sboard_bits_dcache & id_fp_hazard_en) && !((id_reg_gsc_cnt != 'b0) && id_ctrl.gsc);
      if (new_id_fp_stall_dcache && id_valid) fp_sboard_fe_stall_next[id_thread_id] = 1'b1;
      fp_sboard_fe_stall_next &= ~id_dcache_sb_fp_dealloc;
   end



   // interface to FE
   always_comb
     begin
        for(integer i = 0; i < NrThreads; i++)
          begin
             id_fe_req_valid[i] = tag_take_pc && (tag_thread_id == i[0]) ||
                        wb_take_pc  && (wb_thread_id  == i[0]) ||
                        wb_csr_resume[i] ||
                        id_take_redirect[i];

             id_fe_req[i].pc = (wb_csr_resume[i])                                         ?   id_csr_evec_resume // Resume from dbg mode
                               : (wb_xcpt || wb_csr_eret) && (wb_thread_id  == i[0])      ?   id_csr_evec // Exception or [m|s]ret
                               : (wb_reg_replay || wb_csr_replay) && (wb_thread_id == i[0]) ? wb_reg_pc   // Replay instruction in WB
                               : tag_take_pc && (tag_thread_id == i[0])                    ?   tag_npc     // Branch misprediction
                               : {{(PcSizeExt-PcSize){ipi_pc[PcSize-1]}}, ipi_pc};      // PC of the IPI (sign-extended)

             // It is speculative if there's any instruction in the pipeline
             id_fe_req[i].speculative = ex_reg_valid  && (ex_thread_id  == i[0]) ||
                                        tag_reg_valid && (tag_thread_id == i[0]) ||
                                        mem_reg_valid && (mem_thread_id == i[0]) ||
                                        wb_reg_valid  && (wb_thread_id  == i[0]);

             id_fe_req[i].debug_info = '{ resume: wb_csr_resume[i],
                                         halt: xcpt_traps_to_debug && wb_thread_id == i[0],
                                         default: '0};


             id_fe_stall[i] = csr_fe_stall[i] || csr_excl_mode && csr_excl_mode_sel != i[0] || fence_fe_stall[i] ||
                              int_sboard_fe_stall[i] || fp_sboard_fe_stall[i];
          end

        id_take_pc = id_fe_req_valid[id_thread_id];

     end

   ////////////////////////////////////////////////////////////////////////////////
   // Sends down information to EX stage, VPU and DCACHE
   ////////////////////////////////////////////////////////////////////////////////

   minion_control_t id_ctrl_to_ex;          // Control signals sent to EX
   logic            id_rvc;                 // If the instruction in ID is riscV compressed
   logic            id_flush_pipe;          // Send to EX that pipeline must be flushed
   logic            id_replay;              // Replay instruction in ID
   logic            id_xcpt_interrupt;      // Exception interrupt in ID
   logic            id_inst_en;             // Enable sampling of inst and PC to EX
   logic            id_dcache_alloc_rq_val; // Validated allocation request to DCache, to be propagated to EX/S0
   logic [CoreGscCntBits-1:0] ex_reg_gsc_cnt_next;


   always_comb
     begin
        id_ctrl_to_ex     = id_ctrl;
        id_ctrl_to_ex.csr = id_csr;
        id_rvc            = id_inst_rvc;

        // if instruction doesn't use alu, do not update alu configuration
        if ( !id_uses_alu && !id_ctrl.div ) begin
           id_ctrl_to_ex.alu_dw = ex_ctrl.alu_dw;
           id_ctrl_to_ex.alu_fn = ex_ctrl.alu_fn;
        end

        // PC+2
        if (id_xcpt_af || id_xcpt_pf || id_bp_xcpt) begin
           id_ctrl_to_ex.alu_fn   = AluAdd;
           id_ctrl_to_ex.alu_dw   = Dw64;
           id_ctrl_to_ex.sel_alu1 = A1Pc;
           // if split fault, add +2
           id_ctrl_to_ex.sel_alu2 = id_split_fetch_fault? A2Size : A2Zero;
           id_rvc                 = id_split_fetch_fault ? 1'b1 : 1'b0;
        end

        // do not launch any dcache requests if there is an exception
        if (id_xcpt) begin
           id_ctrl_to_ex.mem = 1'b0;
           id_ctrl_to_ex.gsc = 1'b0;
        end

        id_flush_pipe     = id_single_step[id_thread_id];      // Flush pipe
        id_replay         = !id_take_pc && id_valid && id_inst_replay;         // Instruction need to be replayed
        id_xcpt_interrupt = !id_take_pc && id_valid && id_csr_interrupt[id_thread_id]  && !id_csr_flush_stall && !ex_gsc_busy;       // Dealing with an exception interrupt

        // Enable sampling of ID inst and PC to ex
        id_inst_en = id_valid_qual || id_xcpt_interrupt || (id_inst_replay && id_valid);

        // Sends instruction to VPU as well
        id_vpu_req.valid     = id_valid_qual && id_ctrl.fp;
        id_vpu_req.thread_id = id_thread_id;
        id_vpu_req.fcsr_rm   = id_valid_qual && id_ctrl.fp && id_thread_id  ? fcsr_rm_thread1 : fcsr_rm_thread0;
        id_vpu_req.inst_bits = id_inst_bits;

        // Pre-allocates dcache replay queue for mem instructions in ID
        // and confirms the resource allocation for EX stage
        id_dcache_alloc_rq_pre = (id_valid && id_ctrl.mem) || id_keepgsc;
        id_dcache_alloc_rq_val = (id_valid_qual && id_ctrl.mem) || id_keepgsc;
        id_dcache_gsc = id_ctrl.gsc || id_keepgsc;

        // Early Multiply / Divide  request
        id_div_req = id_valid_qual && id_ctrl_to_ex.div;

     end


   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // EX Stage
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

   logic [1:0][XregSize-1:0]       ex_reg_data;           // Data coming from RF and bypasses
   logic [XregSize-1:0]            vpu_ex_reg_data;       // Data coming from RF and bypasses.. going to the VPU
   logic [4:0]                     ex_reg_cause;          // Cause of the exception (xcpt_cause_e)
   logic [PcSizeExt-1:0]           ex_reg_pc;             // PC of the instruction
   logic [InstSize-1:0]            ex_reg_inst;           // Bits of the instruction
   logic [CoreGscCntBits-1:0]      ex_reg_gsc_cnt;        // Gather/Scatter element count
   logic                           ex_gscing_one_lane;    // G/S started in the last lane (according to progress CSR)
   logic                           ex_gscing_one_lane_next; // G/S started in the last lane (according to progress CSR)
   logic                           ex_reg_xcpt_interrupt; // We are doing an interrupt
   logic                           ex_reg_rvc;            // Is a RiscV compressed instruction
   logic                           ex_reg_xcpt;           // There's an exception
   logic                           ex_reg_flush_pipe;     // Flush the pipeline
   logic                           ex_reg_replay;         // Replay the instruction
   logic                           ex_pc_valid;           // Instruction valid in EX
   logic                           ex_pc_valid_to_gsc;    // Instruction valid in EX going to GSC
   logic                           ex_uses_alu;
   // Logic for GSC stage => we are delaying the ex_vpu_ctrl.gsc_fs signal for timing purposes (1 FF is already in the vpu)
   localparam GSC_FS_DELAY = 2; // delay introduced in ex_vpu_ctrl.gsc_fs ( GSC_FS_DELAY -1 FF introduced in the intpipe). min value is 1, because that is what comes from the vpuo
   logic [GSC_FS_DELAY-1:0]        ex_gsc_fs_valid;
   logic [GSC_FS_DELAY-1:0]        ex_gsc_fs_valid_next;
   logic [GSC_FS_DELAY-1:0][31:0]  vpu_gsc_fs_delayed; // note: delay-2 because there is 1 already FF in the VPU
   logic [GSC_FS_DELAY-1:1][31:0]  vpu_gsc_fs_delayed_reg;



   always_ff @(posedge clock) if (reset_w) ex_reg_valid <= 1'b0; else ex_reg_valid <= id_valid_qual || id_keepgsc;
   always_ff @(posedge clock) if (reset_w) ex_reg_gsc_cnt <= CoreGscCntBits'(VpuLaneNum-1); else ex_reg_gsc_cnt <= ex_reg_gsc_cnt_next;
   always_ff @(posedge clock) if (reset_w) ex_gscing_one_lane <= 1'b0; else ex_gscing_one_lane <= ex_gscing_one_lane_next;
   always_ff @(posedge clock) if (reset_w) ex_ctrl_fp_toint <= 1'b0; else ex_ctrl_fp_toint <= id_ctrl_to_ex.wxd && id_ctrl_to_ex.fp;

   always_ff @(posedge clock) if (id_valid_qual || id_xcpt_interrupt) ex_ctrl <= id_ctrl_to_ex;
   always_ff @(posedge clock) if (id_valid_qual) ex_reg_rvc <= id_rvc;
   always_ff @(posedge clock) if (id_valid_qual) ex_reg_flush_pipe <= id_flush_pipe;
   always_ff @(posedge clock) if (id_uses_rs1_all) ex_reg_data[0][XregSize-1:CoreGsc32IdxVSize] <= id_reg_data[0][XregSize-1:CoreGsc32IdxVSize];
   always_ff @(posedge clock) if (id_uses_rs1_lsb) ex_reg_data[0][CoreGsc32IdxVSize-1:0] <= id_reg_data[0][CoreGsc32IdxVSize-1:0];
   always_ff @(posedge clock) if (id_valid_qual && id_ctrl.rxs1 && id_frontend_vpu_ctrl.fromint && id_ctrl.fp) vpu_ex_reg_data <= id_reg_data[0];
   always_ff @(posedge clock) if (id_uses_rs2) ex_reg_data[1] <= id_reg_data[1];
   always_ff @(posedge clock) if (id_xcpt) ex_reg_cause <= id_xcpt_cause;
   always_ff @(posedge clock) if (id_inst_en) ex_reg_pc <= id_pc;
   always_ff @(posedge clock) if (id_inst_en) ex_uses_alu <= id_uses_alu;
   always_ff @(posedge clock) if (id_inst_en) ex_thread_id <= id_thread_id;
   always_ff @(posedge clock) if (id_inst_en) ex_csr_flush <= id_csr_flush;

   always_ff @(posedge clock) ex_gsc_fs_valid <= ex_gsc_fs_valid_next;
   always_ff @(posedge clock) ex_reg_xcpt_interrupt <= id_xcpt_interrupt && !id_take_pc;
   always_ff @(posedge clock) ex_reg_xcpt <= id_valid_qual && id_xcpt;
   always_ff @(posedge clock) ex_reg_replay <= id_replay;
   always_ff @(posedge clock) ex_csr_vpu <= id_csr_vpu_qual;
   always_ff @(posedge clock) ex_csr_excl_mode <= id_csr_excl_mode;
   always_ff @(posedge clock) ex_dcache_alloc_rq_val <= id_dcache_alloc_rq_val;

   // for instruction bits, flop only bits that are going to be used in next stages for powersaving
   intpipe_inst_bits_stage
     # ( .Mask    (ExInstBitsFFMask    ),
         .BrMask  (ExInstBitsFFBrMask  )
         )
   ex_reg_inst_ff
     (
      .clk_i(clock),
      .enable_i(id_inst_en),
      .is_csr_i(id_csr_en),
      .is_br_jal_i(id_ctrl.br || id_ctrl.jal),
      .te_enable_i(te_enable),
      .xcpt_i( id_valid_qual && (id_xcpt || id_ctrl.fp) ), // adding id_ctrl.fp because if if insn FP,
      .in_i(id_inst_bits),                                 // it can have an exception for illegal
      .out_o(ex_reg_inst)                                  // rounding mode in the EX stage
      );

   logic [4:0] ex_xcpt_cause; // Cause of the exception in EX (xcpt_cause_e)
   logic                          ex_xcpt;       // Exception in EX
   logic                          tag_replay;    // Instruction needs to be replayed
   logic                          ex_valid_qual; // EX valid bit after kill to TAG stage
   logic                          ex_valid_qual_to_gsc; // EX valid bit after kill to GSC stage
   logic                          ex_replay;

   always_comb
     begin
        // Decodes some info
        ex_waddr  = ex_reg_inst[11:7];
        ex_csr_en = (ex_ctrl.csr != CsrN);


        ex_replay = ex_reg_replay || csr_excl_mode_transition && ex_reg_valid;

        // Detects new exception cases in EX (default case is illegal instruction
        // in FP
        ex_xcpt       = 1'b1;
        ex_xcpt_cause = XcptIllegalInstruction;
        if(ex_reg_xcpt_interrupt || ex_reg_xcpt)
          ex_xcpt_cause = ex_reg_cause;
        else if(!(ex_ctrl.fp && ex_vpu_ctrl.illegal_rm && ex_reg_valid))
          ex_xcpt = 1'b0;

        // Kill
        ex_ctrl_kill = id_fe_req_valid[ex_thread_id] || ex_replay ||
                       (tag_replay || tag_dcache_bp_triggered) && (tag_thread_id == ex_thread_id) ||
                       mem_dcache_flush_pipeline && mem_thread_id == ex_thread_id;

        ex_valid_qual = ex_reg_valid && !ex_ctrl_kill && !ex_ctrl.gsc;
        ex_valid_qual_to_gsc = ex_reg_valid && !ex_ctrl_kill && ex_ctrl.gsc && ex_gsc_fs_valid[0];
     end

   always_comb
     begin
        // Valid for EX stage for replays and interrupts
        ex_pc_valid = (ex_reg_valid || ex_replay || ex_reg_xcpt_interrupt) && !ex_ctrl.gsc;
        ex_pc_valid_to_gsc = (ex_reg_valid || ex_replay || ex_reg_xcpt_interrupt) && ex_ctrl.gsc && !(tag_dcache_bp_triggered && (tag_thread_id == ex_thread_id));

     end

   ////////////////////////////////////////////////////////////////////////////////
   // Unit that generates the immediate for the second source of the instruction
   ////////////////////////////////////////////////////////////////////////////////

   logic [XregSize-1:0] id_imm; // Immediate for second source

   intpipe_imm
     imm_src2
       (
        // Instruction and control
        .inst_bits ( id_inst_bits    ),
        .sel_imm   ( id_ctrl.sel_imm ),
        // Output
        .imm       ( id_imm          )
        );

   ////////////////////////////////////////////////////////////////////////////////
         // ALU that does the integer divide and mul
   ////////////////////////////////////////////////////////////////////////////////

   minion_reg_dest_t  ex_div_dest;               // Destination in EX stage to div unit
   logic [XregSize-1:0] wb_div_resp_data;          // Div unit result data
   logic               tag_kill_common;           // Kill instruction in TAG
   logic               wb_div_resp_valid_early;   // Next cycle Div will try to write
   logic               tag_div_req;
   logic               mem_div_req;
   logic               tag_ctrl_kill;             // Kill the instruction in TAG stage
   /* verilator lint_off UNOPTFLAT */  // MEM kill participates in the preserved intpipe<->dcache replay/flush loop that Verilator misidentifies as cyclic after flattening.
   logic               mem_ctrl_kill;             // MEM instruction is killed
   /* verilator lint_on UNOPTFLAT */

   intpipe_mul_div_top
     mul_div
       (
        // System signals
        .clk_i               ( clock                   ),
        .rst_ni              ( !reset_w                ),
        .dft_i               ( '0                      ),
        // Request port
        .req_ready           ( ex_div_ready            ),
        .req_valid           ( ex_div_req              ),
        .req_valid_early     ( id_div_req              ),
        .req_fn_early        ( id_ctrl.alu_fn          ),
        .req_fn              ( ex_ctrl.alu_fn          ),
        .req_dw              ( ex_ctrl.alu_dw          ),
        .req_in1             ( ex_reg_data[0]          ),
        .req_in2             ( ex_reg_data[1]          ),
        .req_dest            ( ex_div_dest             ),
        .kill                ( div_kill                ),
        .chicken_bit_mul_div ( chicken_bit_intpipe     ),
        // Response port
        .resp_ready          ( wb_div_resp_ready       ),
        .resp_valid          ( wb_div_resp_valid       ),
        .resp_valid_early    ( wb_div_resp_valid_early ),
        .resp_data           ( wb_div_resp_data        ),
        .resp_dest           ( wb_div_resp_dest        )
        );



   always_ff @(posedge clock) if (reset_w) tag_div_req <= 1'b0; else tag_div_req <= ex_div_req;
   always_ff @(posedge clock) if (reset_w) mem_div_req <= 1'b0; else mem_div_req <= tag_div_req && !div_kill;
   always_comb
     begin
        // New request coming
        ex_div_req            = ex_reg_valid  && ex_ctrl.div;
        ex_div_dest.fp        = 1'b0;
        ex_div_dest.addr      = ex_waddr;
        ex_div_dest.thread_id = ex_thread_id;

        // Kill when we have a kill in TAG stage and we issued a div instruction
        // in previous cycle
        div_kill = tag_div_req && (!tag_reg_valid || tag_ctrl_kill) || mem_div_req && mem_ctrl_kill;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Gather/Scatter FSM
   ////////////////////////////////////////////////////////////////////////////////

   always_comb
     begin
        // Computes the G/S count for next cycle
        ex_reg_gsc_cnt_next = id_keepgsc && ex_gsc_fs_valid[0]   ? ex_reg_gsc_cnt + CoreGscCntBits'(1) // If doing G/S, dcache not blocked and instruction not killed, move to next element
                              : id_valid_qual                    ? id_reg_gsc_cnt                         // Get ID counter if a new instruction is coming
                              : ex_ctrl_kill                     ? CoreGscCntBits'(VpuLaneNum-1)          // Set counter to last element in case it is killed, so we stop setting ex_gscing
                              :                                    ex_reg_gsc_cnt;                        // Keep value otherwise (next gather element is stalled in ID)

        // We are doing G/S while we are not done with all the elements
        ex_gscing = ex_ctrl.gsc & (ex_reg_gsc_cnt != CoreGscCntBits'(VpuLaneNum-1) || ex_gscing_one_lane) & !ex_ctrl_kill;

        // G/S starts in the last lane
        id_gscing_one_lane = id_valid_qual && (id_reg_gsc_cnt == CoreGscCntBits'(VpuLaneNum-1)) && id_ctrl.gsc;

        // keep ex_gscing_one_lane high if id_dcache_ready is 0
        ex_gscing_one_lane_next = id_gscing_one_lane ? 1'b1 : id_dcache_ready || id_valid_qual ? 1'b0 : ex_gscing_one_lane;

     end

   ////////////////////////////////////////////////////////////////////////////////
   // Sends down information to TAG stage and VPU
   ////////////////////////////////////////////////////////////////////////////////
   logic [CoreGscCntBits-1:0]      gsc_reg_gsc_cnt;    // Gather/Scatter element count
   logic                           ex_rs2_en; // Enables data sampling from second source for mem operations

   always_comb
     begin
        // Computes some enables and info
        ex_rs2_en = ex_ctrl.rxs2 && ex_ctrl.mem && ex_ctrl.mem_cmd!=DcCmdXrd && !ex_ctrl.fp;

        // Sends control signals to VPU
        ex_vpu_req.thread_id    = ex_thread_id;
        ex_vpu_req.kill         = ex_ctrl_kill;
        ex_vpu_req.fromint_data = vpu_ex_reg_data;
        ex_vpu_req.gscing       = ex_gscing;
        ex_vpu_req.gsc_src      = ex_reg_gsc_cnt;
     end


   ////////////////////////////////////////////////////////////////////////////////
   // GSC Stage
   ////////////////////////////////////////////////////////////////////////////////

   // This is an additional stage exclusivily used in gather/scatter instructions
   // to ease timing of gather/scatter index coming from VPU
   // For other instructions, the logic in this stage is used in EX stage

   logic [DmemReqPsMaskSz-1:0]  gsc_reg_ps_mask;    // PS mask for dcache access
   logic                        gsc_pc_valid;       // Instruction valid in GSC
   logic                        gsc_reg_flush_pipe; // Flush the pipeline

   //      CLK    RST      EN                     DOUT                 DIN                      DEF
   always_ff @(posedge clock) if (reset_w) gsc_reg_valid <= 1'b0; else gsc_reg_valid <= ex_valid_qual_to_gsc;
   always_ff @(posedge clock) if (reset_w) gsc_reg_gsc_cnt <= CoreGscCntBits'(0); else gsc_reg_gsc_cnt <= ex_reg_gsc_cnt;
   always_ff @(posedge clock) if (ex_valid_qual_to_gsc) gsc_reg_ps_mask <= ex_vpu_ctrl.ps_mask;
   always_ff @(posedge clock) if (reset_w) gsc_reg_gscing <= 1'b0; else gsc_reg_gscing <= ex_gscing;
   always_ff @(posedge clock) gsc_pc_valid <= ex_pc_valid_to_gsc;
   always_ff @(posedge clock) gsc_reg_flush_pipe <= ex_reg_flush_pipe;


   // delay ex_vpu_ctrl.gsc_fs signal ( add GSC_FS_DELAY - 1 FFs)
   always_comb begin
      ex_gsc_fs_valid_next =   id_valid_qual ? ({{(GSC_FS_DELAY-1){1'b0}}, 1'b1} << (GSC_FS_DELAY-1)) :
                               ex_ctrl.gsc && id_keepgsc ?   ({{(GSC_FS_DELAY-1){1'b0}}, 1'b1} << (GSC_FS_DELAY-1)) | (ex_gsc_fs_valid >> 1'b1):
                               ex_gsc_fs_valid;
   end
   genvar gen_it;
   generate
      for ( gen_it = 1; gen_it < GSC_FS_DELAY; gen_it ++) begin: DELAY_GSC_FS
         logic [31:0] vpu_gsc_fs_delayed_next;
         assign vpu_gsc_fs_delayed_next = vpu_gsc_fs_delayed[gen_it-1];
         always_ff @(posedge clock) if (ex_reg_valid && !ex_ctrl_kill && ex_ctrl.gsc) vpu_gsc_fs_delayed_reg[gen_it] <= vpu_gsc_fs_delayed_next;
      end
   endgenerate
   always_comb vpu_gsc_fs_delayed = {vpu_gsc_fs_delayed_reg,ex_vpu_ctrl.gsc_fs};

   logic gsc_valid_qual; // GSC valid bit after kill to TAG stage

   assign gsc_valid_qual = gsc_reg_valid && !ex_ctrl_kill;

   ////////////////////////////////////////////////////////////////////////////////
   // ALU that does the basic integer operations
   ////////////////////////////////////////////////////////////////////////////////

   logic [XregSize-1:0] ex_op1;         // Data for the first operand
   logic [XregSize-1:0] ex_op2;         // Data for the second operand
   logic [XregSize-1:0] id_op1;         // Data for the first operand
   logic [XregSize-1:0] id_op2;         // Data for the second operand
   logic [XregSize-1:0] ex_alu_add_out; // Result of ALU adder

   // Selects the alu operands
   always_comb begin
      id_op1='0; //default value
      if ( ex_gsc_busy )  // next iteration of gather/scatter (instruction in id is not issued)
        id_op1 = {{(XregSize-32){vpu_gsc_fs_delayed[GSC_FS_DELAY-2][31]}}, vpu_gsc_fs_delayed[GSC_FS_DELAY-2]};
      else begin
         case (id_ctrl_to_ex.sel_alu1)
           A1Rs1: id_op1 = id_reg_data[0];
           A1Pc:  id_op1 = {{(XregSize-PcSizeExt){id_pc[PcSizeExt-1]}}, id_pc};
           A1Fs1: id_op1 = {{(XregSize-32){vpu_gsc_fs_delayed[GSC_FS_DELAY-2][31]}}, vpu_gsc_fs_delayed[GSC_FS_DELAY-2]};
           default:          id_op1 = '0;
         endcase // case (id_ctrl.sel_alu1)
      end

      id_op2 = '0; // default value
      case (id_ctrl_to_ex.sel_alu2)
        A2Rs2:   id_op2 = id_reg_data[1];
        A2Imm:   id_op2 = id_imm;
        A2Size:  id_op2 =  id_rvc ? XregSize'(2) : XregSize'(4);
        default:            id_op2 = '0;
      endcase // case (id_ctrl.sel_alu2)
   end // always_comb

   always_ff @(posedge clock) if (id_uses_alu && id_valid_qual || ex_gsc_busy) ex_op1 <= id_op1;
   always_ff @(posedge clock) if (id_uses_alu && id_valid_qual) ex_op2 <= id_op2;

   intpipe_alu
     alu
       (
        // Control
        .dw        ( ex_ctrl.alu_dw ),
        .fn        ( ex_ctrl.alu_fn ),
        // Data input
        .in1       ( ex_op1         ),
        .in2       ( ex_op2         ),
        // Output
        .out       ( ex_alu_out     ),
        .adder_out ( ex_alu_add_out )
        );

   ////////////////////////////////////////////////////////////////////////////////
     // Sends down information to Dcache
   ////////////////////////////////////////////////////////////////////////////////
   logic ex_dcache_addr_msb_err;

   always_comb
     begin

        ex_dcache_addr_msb_err = |ex_alu_add_out[XregSize-1:VaSize-1] == 1'b0 ||
                                 &ex_alu_add_out[XregSize-1:VaSize-1] == 1'b1 ? 1'b0 : 1'b1;
        // Request to Dcache
        ex_dcache_req_valid          = ex_reg_valid && ex_ctrl.mem && !ex_ctrl.gsc
                                       || gsc_reg_valid;
        ex_dcache_req.dest.fp        = ex_ctrl.fp;
        ex_dcache_req.dest.addr      = ex_waddr;
        ex_dcache_req.dest.thread_id = ex_thread_id;
        ex_dcache_req.cmd            = ex_ctrl.mem_cmd;
        ex_dcache_req.typ            = ex_ctrl.mem_type;
        ex_dcache_req.gsc_cnt        = gsc_reg_gsc_cnt;
        ex_dcache_req.phys           = 1'b0;
        ex_dcache_req.addr           = ex_alu_add_out[VaSize-1:0];
        ex_dcache_req.addr_msb_err   = ex_dcache_addr_msb_err;
        ex_dcache_req.ps_mask        = gsc_reg_valid ? gsc_reg_ps_mask : ex_vpu_ctrl.ps_mask;
        ex_dcache_req.gsc32_idx      = ex_reg_data[0][CoreGsc32IdxVSize-1:0];
        ex_dcache_req.mem_global     = ex_ctrl.mem_g;
        `ifdef DCACHE_REPORT_PC
        ex_dcache_req.pc             = ex_reg_pc;
        `endif

        ex_dcache_gsc = ex_valid_qual_to_gsc;
     end

   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // TAG Stage
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

   logic [XregSize-1:0]         tag_reg_rs2;            // Store data/Atomic operation source
   logic [4:0]                  tag_reg_cause;          // Cause of the exception
   logic [PcSizeExt-1:0]        tag_reg_pc;             // PC of instruction in TAG stage
   logic [InstSize-1:0]         tag_reg_inst;           // Bits of the instruction
   logic                        tag_reg_xcpt_interrupt; // It is an interrupt
   logic                        tag_reg_rvc;            // RiscV compressed instruction
   logic                        tag_reg_xcpt;           // An exception happened to the instruction
   logic                        tag_reg_replay;         // It is a replay
   logic                        tag_reg_flush_pipe;     // Requires a flush
   logic                        tag_reg_gscing;         // Dealing with a G/S operation

   //      CLK    RST      EN                           DOUT                    DIN                              DEF
   always_ff @(posedge clock) if (reset_w) tag_reg_valid <= 1'b0; else tag_reg_valid <= ex_valid_qual || gsc_valid_qual;
   always_ff @(posedge clock) if (ex_xcpt) tag_reg_cause <= ex_xcpt_cause;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_ctrl <= ex_ctrl;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_reg_rvc <= ex_reg_rvc;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_reg_flush_pipe <= gsc_pc_valid ? gsc_reg_flush_pipe : ex_reg_flush_pipe;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_reg_pc <= ex_reg_pc;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_thread_id <= ex_thread_id;
   always_ff @(posedge clock) if (ex_pc_valid || gsc_pc_valid) tag_csr_flush <= ex_csr_flush;
   always_ff @(posedge clock) if ((ex_pc_valid || gsc_pc_valid) && ex_uses_alu) tag_reg_wdata <= ex_alu_out;
   always_ff @(posedge clock) if (ex_rs2_en) tag_reg_rs2 <= ex_reg_data[1];
   always_ff @(posedge clock) tag_reg_xcpt_interrupt <= !id_fe_req_valid[ex_thread_id] && ex_reg_xcpt_interrupt;
   always_ff @(posedge clock) tag_reg_xcpt <= ex_xcpt && ex_valid_qual;
   always_ff @(posedge clock) tag_reg_replay <= ex_replay && !id_take_pc;
   always_ff @(posedge clock) tag_reg_gscing <= gsc_reg_gscing;
   always_ff @(posedge clock) if (reset_w) tag_ctrl_fp_toint <= 1'b0; else tag_ctrl_fp_toint <= ex_ctrl.wxd && ex_ctrl.fp;
   always_ff @(posedge clock) tag_csr_vpu <= ex_valid_qual && ex_csr_vpu;
   always_ff @(posedge clock) tag_csr_excl_mode <= ex_csr_excl_mode;

   // for instruction bits, flop only bits that are going to be used in next stages for powersaving
   intpipe_inst_bits_stage
     # ( .Mask    (TagInstBitsFFMask    ),
         .BrMask  (TagInstBitsFFBrMask  )
         )
   tag_reg_inst_ff
     (
      .clk_i(clock),
      .enable_i(ex_pc_valid || gsc_pc_valid),
      .is_csr_i(ex_csr_en),
      .is_br_jal_i ( ex_ctrl.br || ex_ctrl.jal),
      .te_enable_i(te_enable),
      .xcpt_i(ex_xcpt),
      .in_i(ex_reg_inst),
      .out_o(tag_reg_inst)
      );


   ////////////////////////////////////////////////////////////////////////////////
   // Branch mispredict logic
   ////////////////////////////////////////////////////////////////////////////////

   logic [PcSizeExt-1:0]       tag_br_target;      // Target of the branch
   logic [PcSizeExt-1:0]       tag_jal_imm;        // Immediate offset to the PC for JAL instructions
   logic [PcSizeExt-1:0]       tag_br_imm;         // Immediate offset to the PC for Branch instructions
   logic                       tag_br_taken;       // The branch is taken
   logic                       tag_misprediction;  // There was a mispredict
   logic                       tag_npc_misaligned; // The next PC is misaligned (raise an exception)

   always_comb
     begin
        // The branch taken comes from the ALU result
        tag_br_taken = tag_reg_wdata[0];
        tbranch_up = NrThreads'(0);
        tbranch_up[tag_thread_id] = tag_br_taken && tag_ctrl.br;

        // Gets the JAL immediate
        tag_jal_imm = { {PcSizeExt-20{tag_reg_inst[31]}}, tag_reg_inst[19:12], tag_reg_inst[20], tag_reg_inst[30:21], 1'b0 };
        // Gets the Branch immediate
        tag_br_imm  = { {PcSizeExt-12{tag_reg_inst[31]}}, tag_reg_inst[7], tag_reg_inst[30:25], tag_reg_inst[11:8], 1'b0 };

        // Adds to the PC the offset
        tag_br_target = tag_reg_pc +
                        ((tag_ctrl.br && tag_br_taken) ? tag_br_imm  : // Branch taken
                         tag_ctrl.jal                 ? tag_jal_imm  : // JAL
                         tag_reg_rvc                  ? PcSizeExt'(2) : // +2 for 16b instructions
                         PcSizeExt'(4));                               // +4 for 32b instructions

        // Computes the next PC
        tag_npc = tag_ctrl.jalr ? extend_VA(tag_reg_wdata) :
                                  tag_br_target;
        tag_npc[0] = 1'b0; // Clears bit[0]

        // Misprediction over taken branches
        tag_misprediction = ((tag_ctrl.br && tag_br_taken) ||           // Conditional taken branch
                            tag_ctrl.jal ||                             // Jump
                            tag_ctrl.jalr) &&                           // Jump indirect
                            !(tag_reg_xcpt || tag_reg_xcpt_interrupt);  // and not trailing an exception => this is just in case
                                                                        // the instruction bits are invalid (e.g. ecc error) and just
                                                                        // happen to be a jump


        // The PC is misaligned if we don't have Compressed enable and not word aligned
        tag_npc_misaligned = 1'b0; //!status.isa[2] && tag_npc[1]; => Commented out, reg misa is RO... so isa[2] is always 1

        // Take new PC when there's an instruction and there's a misprediction or
        // forced flush
        tag_take_pc = tag_reg_valid && (tag_misprediction || tag_reg_flush_pipe);

        // Computes final write to RF
        tag_int_wdata = (!tag_reg_xcpt && (tag_ctrl.jalr ^ tag_npc_misaligned)) ? {{(XregSize-PcSizeExt){tag_br_target[PcSizeExt-1]}}, tag_br_target} : // JALR need to write the next PC in the dest reg
                                                                                  tag_reg_wdata;

        // Gets some info
        tag_waddr  = tag_reg_inst[11:7];
        tag_csr_en = (tag_ctrl.csr != CsrN);
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Breakpoint unit for data
   ////////////////////////////////////////////////////////////////////////////////
   logic                dcache_bp_xcpt;
   logic                dcache_bp_enter_debug;
   logic                dcache_bp_timing;

   debug_breakpoint #(.TriggerType(1)) dcache_bp
     (
      .control_ip(bp_control),
      .in_debug_ip(debug),
      .prv_ip(prv),
      .tdata2_ip(bp_address),
      .address_ip(tag_dcache_bp.address),
      .cmd_ip(tag_dcache_bp.cmd),
      .valid_ip(tag_dcache_bp_valid),
      .thread_id_ip(tag_dcache_bp.thread_id),
      .timing_op(dcache_bp_timing),
      .xcpt_op(dcache_bp_xcpt),
      .enter_debug_op(dcache_bp_enter_debug),
      .update_op(tag_dcache_bp_triggered)
      );

   ////////////////////////////////////////////////////////////////////////////////
   // Breakpoint and exception handling
   ////////////////////////////////////////////////////////////////////////////////

   logic [4:0] tag_new_cause;       // Cause of new exception found in TAG (xcpt_cause_e)
   logic [4:0] tag_xcpt_cause;      // Final exception cause (xcpt_cause_e)
   logic                tag_breakpoint; // Breakpoint
   logic                tag_debug_breakpoint;
   logic                tag_new_xcpt;   // New exception detected in TAG
   logic                tag_xcpt;       // Final exception for TAG instruction

   always_comb
     begin
        // Computes if there's a breakpoint related to TAG operation
        tag_breakpoint = dcache_bp_xcpt;
        // Computes if there's a debug breakpoint related to TAG operation
        tag_debug_breakpoint = dcache_bp_enter_debug;

        // Computes the exceptions detected in TAG stage (default is misaligned fetch)
        tag_new_xcpt  = 1'b1;
        tag_new_cause = XcptMisalignedFetch;

        if(tag_debug_breakpoint)
          tag_new_cause = XcptDebug;
        else if(tag_breakpoint)
          tag_new_cause = XcptBreakpoint;

        else if(tag_ctrl.mem && tag_dcache_xcpt.mf_ld)
          tag_new_cause = XcptMisalignedLoad;
        else if(tag_ctrl.mem && tag_dcache_xcpt.mf_st)
          tag_new_cause = XcptMisalignedStore;
        // Contrary to what the PRM says we should keep the "Access fault" before
        // the "Page fault", because misaligned AMOs may generate both AF and PF
        // and we need to report the AF only in this case (all other instructions
        // generate either AF or PF but not both).
        else if(tag_ctrl.mem && tag_dcache_xcpt.af_ld)
          tag_new_cause = XcptLoadAccess;
        else if(tag_ctrl.mem && tag_dcache_xcpt.af_st)
          tag_new_cause = XcptStoreAccess;
        else if(tag_ctrl.mem && tag_dcache_xcpt.ps_ld)
          tag_new_cause = 5'h10; // LOAD_PAGE_SPLIT_FAULT
        else if(tag_ctrl.mem && tag_dcache_xcpt.ps_st)
          tag_new_cause = 5'h11; // STORE_PAGE_SPLIT_FAULT
        else if(tag_ctrl.mem && tag_dcache_xcpt.pf_ld)
          tag_new_cause = XcptLoadPageFault;
        else if(tag_ctrl.mem && tag_dcache_xcpt.pf_st)
          tag_new_cause = XcptStorePageFault;

        else if(!tag_npc_misaligned)
          tag_new_xcpt = 1'b0;

        // Final exception computation
        tag_xcpt       = tag_reg_xcpt_interrupt || tag_reg_xcpt || (tag_reg_valid && tag_new_xcpt);
        tag_xcpt_cause = (tag_reg_xcpt_interrupt || tag_reg_xcpt) ? tag_reg_cause
                         :                                            tag_new_cause;
     end



   always_comb begin
      debug_timing_next = debug_timing;

      if (id_bp_update_timing)
        debug_timing_next[id_thread_id] = id_bp_timing;

      if (tag_dcache_bp_triggered) // note that dcache condition is more prioritary than the one from fetch
        debug_timing_next[tag_thread_id] = dcache_bp_timing;


   end
   always_ff @(posedge clock) if (reset_w) debug_timing <= '0; else debug_timing <= debug_timing_next;

   ////////////////////////////////////////////////////////////////////////////////
        // Instruction kill
   ////////////////////////////////////////////////////////////////////////////////

   logic tag_killed_by_dcache; // Instruction killed by dcache
   logic tag_valid_qual;       // Valid instruction in TAG stage after kill

   always_comb
     begin
        // DCache kills TAG instruction because it has higher priority on the RF
        // write port (only happens for replays)
        tag_killed_by_dcache    = tag_reg_valid && tag_ctrl.wxd && tag_dcache_replay_next;

        // Replay instruction when replay set in prior stages or instruction is
        // killed by Dcache or VPU
        tag_replay  = tag_reg_replay || tag_killed_by_dcache || csr_excl_mode_transition && tag_reg_valid;

        // Signal sent to units to kill the instruction
        tag_kill_common = tag_killed_by_dcache ||
                          wb_take_pc && (wb_thread_id == tag_thread_id) ||
                          wb_csr_resume[tag_thread_id] ||
                          tag_reg_xcpt ||
                          mem_dcache_flush_pipeline && mem_thread_id == tag_thread_id  ||
                          csr_excl_mode_transition;

        // No instruction going to WB
        tag_ctrl_kill  = tag_kill_common || tag_xcpt;
        tag_valid_qual = tag_reg_valid && !tag_ctrl_kill;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Sends down information to MEM stage and VPU
   ////////////////////////////////////////////////////////////////////////////////

   logic [XregSize-1:0] tag_final_wdata; // Final result data to WB
   logic                tag_pc_valid;    // Instruction valid in TAG
   logic                tag_thread_disabled;
   logic                tag_uses_wdata;
   always_comb
     begin
        tag_pc_valid = tag_reg_valid || tag_replay || tag_reg_xcpt_interrupt;
        tag_final_wdata = tag_int_wdata;
        // overwrite LSB in case there is a breakpoint exception (keeping MSB to save a few muxes)
        tag_final_wdata[VaSizeExt-1:0] = dcache_bp_xcpt ? tag_dcache_bp.address : tag_int_wdata[VaSizeExt-1:0];

        tag_thread_disabled = tag_thread_id && !thread1_enable || !tag_thread_id && !thread0_enable;
        tag_uses_wdata = tag_ctrl.wxd && !tag_ctrl.alu_fn[4]   || // used alu, and writes into int RF (includes alu ops, jal, csrs..)
                         tag_csr_en || // tag goes to the CSR (either regs or special system instructions)
                         tag_reg_xcpt || // or an exception (because fetch page and access faults use them)
                         tag_breakpoint; // or data breakpoint xcpt
        // Sends kill to VPU
        tag_vpu_kill = !tag_reg_valid || tag_kill_common || tag_thread_disabled;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Sends down information to DCache
   ////////////////////////////////////////////////////////////////////////////////

   logic [VpuDataSSz-1:0] vpu_scatter_store_data; // Extra flip flop for scatter data coming from VPU

   always_ff @(posedge clock) if (gsc_reg_valid && dcache_cmd_is_write(ex_ctrl.mem_cmd)) vpu_scatter_store_data <= tag_vpu_ctrl.scatter_data[VpuDataSSz-1:0];

   always_comb
     begin
        // Data and kill to DCache. Send FP bits if no integer mem instruction, so
        // reduce operations can send the VRF store data to the dcache
        // Scatter data is delayed an additional cycle to synchronize with GSC stage

        // send vpu data by default
        tag_dcache_store_data = tag_ctrl.gsc ? {{(CoreVpuStoreDataSz-VpuDataSSz){1'b0}}, vpu_scatter_store_data} : tag_vpu_ctrl.store_data;

        // if we have to send int data, just set bits [63:0]... other bits will still be from the vpu, but won't be used (and we can save muxing)
        if (! (tag_ctrl.fp || !(tag_reg_valid && tag_ctrl.mem))) tag_dcache_store_data[XregSize-1:0] =  tag_reg_rs2;



        tag_dcache_kill = !tag_reg_valid || tag_kill_common || tag_dcache_bp_triggered || tag_thread_disabled;
     end

   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // MEM Stage
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

   logic [4:0]                  mem_reg_cause;   // Exception bits in MEM (xcpt_cause_e)
   logic                        mem_reg_xcpt_interrupt;
   logic [PcSizeExt-1:0]        mem_reg_pc;      // PC of instruction in MEM stage
   logic                        mem_uses_wdata;
   logic                        mem_reg_xcpt;    // Exception in the MEM instruction
   logic                        mem_reg_replay;  // Instruction in MEM must be replayed
   logic                        mem_reg_gscing;  // Instruction working on non-final element of a G/S
   logic                        mem_replay;      // Final signal indicating that instruction in MEM needs to be replayed
   logic                        mem_reg_rvc;     // this comes from a compressed instruction

   //      CLK    RST      EN            DOUT                   DIN                  DEF
   always_ff @(posedge clock) if (reset_w) mem_reg_valid <= 1'b0; else mem_reg_valid <= tag_valid_qual;
   always_ff @(posedge clock) if (reset_w) mem_dcache_bp_triggered <= '0; else mem_dcache_bp_triggered <= mem_dcache_bp_triggered_next;
   always_ff @(posedge clock) if (tag_xcpt) mem_reg_cause <= tag_xcpt_cause;
   always_ff @(posedge clock) if (tag_pc_valid) mem_ctrl <= tag_ctrl;
   always_ff @(posedge clock) if (tag_pc_valid) mem_reg_rvc <= tag_reg_rvc;
   always_ff @(posedge clock) if (tag_pc_valid) mem_reg_pc <= tag_reg_pc;
   always_ff @(posedge clock) if (tag_pc_valid) mem_uses_wdata <= tag_uses_wdata;
   always_ff @(posedge clock) if (tag_pc_valid) mem_csr_flush <= tag_csr_flush;
   always_ff @(posedge clock) if (reset_w) mem_thread_id <= 1'b0; else if (tag_pc_valid) mem_thread_id <= tag_thread_id;
   always_ff @(posedge clock) if (tag_pc_valid && tag_uses_wdata) mem_reg_wdata <= tag_final_wdata;
   always_ff @(posedge clock) mem_reg_xcpt <= tag_xcpt && !(wb_take_pc && (wb_thread_id == tag_thread_id) || wb_csr_resume[tag_thread_id]);
   always_ff @(posedge clock) if (tag_xcpt) mem_reg_xcpt_interrupt <= tag_reg_xcpt_interrupt;
   always_ff @(posedge clock) mem_reg_replay <= tag_replay && !(wb_take_pc && (wb_thread_id == tag_thread_id) || wb_csr_resume[tag_thread_id]);
   always_ff @(posedge clock) mem_reg_gscing <= tag_reg_gscing;
   always_ff @(posedge clock) if (reset_w) mem_ctrl_fp_toint <= 1'b0; else mem_ctrl_fp_toint <= tag_ctrl.wxd && tag_ctrl.fp;
   always_ff @(posedge clock) mem_csr_vpu <= tag_valid_qual && tag_csr_vpu;
   always_ff @(posedge clock) mem_csr_excl_mode <= tag_csr_excl_mode;

   // only update (progress CSR) if there is a breakpoint (tag_dcache_bp_triggered) and it is for a different
   // thread ( if it is for the same thread,  it means there have been 2 consecutive breakpoints for the
   // same thread (e.g. gather scatter, consecutive loads... ) => only the first one will be taken
   assign mem_dcache_bp_triggered_next = tag_dcache_bp_triggered && ( !mem_dcache_bp_triggered || mem_thread_id != tag_thread_id);


   // for instruction bits, flop only bits that are going to be used in next stages for powersaving
   intpipe_inst_bits_stage
     # ( .Mask    (MemInstBitsFFMask    ),
         .BrMask  (MemInstBitsFFBrMask  )
         )
   mem_reg_inst_ff
    (
     .clk_i(clock),
     .enable_i(tag_pc_valid),
     .is_csr_i(tag_csr_en),
     .is_br_jal_i(1'b0), // not used
     .te_enable_i(te_enable),
     .xcpt_i(tag_xcpt),
     .in_i(tag_reg_inst),
                    .out_o(mem_reg_inst)
     );

   ////////////////////////////////////////////////////////////////////////////////
   // Sends down information to WB stage and VPU
   ////////////////////////////////////////////////////////////////////////////////

   logic                       mem_pc_valid;   // Instruction valid in MEM
   logic                       mem_rfen_early; // Instruction in MEM will write integer RF next cycle

   always_comb
     begin
        // Replay instruction because of previous stages or because of Dcache flushing pipeline
        mem_replay = mem_reg_replay | mem_dcache_flush_pipeline || csr_excl_mode_transition && mem_reg_valid;

        // Propagete PC to next stage
        mem_pc_valid = mem_reg_valid || mem_replay || mem_reg_xcpt;

        // WB PC changes kill the instruction, and if in thread1 and thread 1 has been disabled
        mem_ctrl_kill_no_tlb_miss  = wb_take_pc && (wb_thread_id == mem_thread_id) ||
                                     wb_csr_resume[mem_thread_id] ||
                                     mem_thread_id && !thread1_enable ||
                                     !mem_thread_id && !thread0_enable ||
                                     csr_excl_mode_transition;

        mem_ctrl_kill = mem_ctrl_kill_no_tlb_miss || mem_dcache_flush_pipeline;

        mem_valid_qual = mem_reg_valid && !mem_ctrl_kill;

        // Sends kill to VPU and Dcache
        mem_vpu_kill    = mem_ctrl_kill;
        mem_dcache_kill = mem_ctrl_kill;

        // Gets some info
        mem_waddr  = mem_reg_inst[11:7];
        mem_csr_en = (mem_ctrl.csr != CsrN);

        // Write early for regular integer
        mem_rfen_early = mem_reg_valid && mem_ctrl.wxd && !mem_ctrl.fp;

        // CSR command
        mem_csr_rw_cmd  = mem_valid_qual ? mem_ctrl.csr : CsrN;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Exception handling
   ////////////////////////////////////////////////////////////////////////////////

   logic [4:0] mem_xcpt_cause; // Final exception cause (xcpt_cause_e)
   logic       mem_xcpt;       // Final exception for MEM instruction

   always_comb
     begin
        // Final exception computation
        mem_xcpt       = mem_reg_xcpt;
        mem_xcpt_cause = mem_reg_cause;
     end

   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // WB Stage
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

   logic wb_reg_gscing; // Doing a G/S operation


   always_ff @(posedge clock) if (reset_w) wb_reg_valid <= 1'b0; else wb_reg_valid <= mem_valid_qual;
   always_ff @(posedge clock) if (reset_w) wb_dcache_fp_toint <= 1'b0; else wb_dcache_fp_toint <= mem_ctrl.wxd && mem_ctrl.fp && mem_valid_qual;

   always_ff @(posedge clock) if (mem_xcpt) wb_reg_cause <= mem_xcpt_cause;
   always_ff @(posedge clock) if (mem_xcpt) wb_reg_xcpt_interrupt <= mem_reg_xcpt_interrupt;
   always_ff @(posedge clock) if (mem_pc_valid) wb_ctrl <= mem_ctrl;
   always_ff @(posedge clock) if (mem_pc_valid) wb_reg_pc <= mem_reg_pc;
   always_ff @(posedge clock) if (mem_pc_valid) wb_csr_flush <= mem_csr_flush;
   always_ff @(posedge clock) if (mem_pc_valid) wb_reg_rvc <= mem_reg_rvc;
   always_ff @(posedge clock) if (mem_pc_valid) wb_thread_id <= mem_thread_id;
   always_ff @(posedge clock) if (mem_uses_wdata && mem_pc_valid) wb_reg_wdata <= mem_reg_wdata;

   always_ff @(posedge clock) wb_reg_xcpt <= mem_xcpt && !(wb_take_pc && (wb_thread_id == mem_thread_id) || wb_csr_resume[mem_thread_id]);
   always_ff @(posedge clock) wb_reg_replay <= mem_replay && !(wb_take_pc && (wb_thread_id == mem_thread_id) || wb_csr_resume[mem_thread_id]);
   always_ff @(posedge clock) wb_reg_gscing <= mem_reg_gscing;
   always_ff @(posedge clock) wb_csr_vpu <= mem_valid_qual && mem_csr_vpu;



   // for instruction bits, flop only bits that are going to be used in next stages for powersaving
   intpipe_inst_bits_stage
     # ( .Mask    (WbInstBitsFFMask    ),
         .BrMask  (WbInstBitsFFBrMask  )
         )
   wb_reg_inst_ff
         (
          .clk_i(clock),
          .enable_i(mem_pc_valid),
          .is_csr_i(mem_csr_en),
          .is_br_jal_i(1'b0), // not used
          .te_enable_i(te_enable),
          .xcpt_i(mem_xcpt),
          .in_i(mem_reg_inst),
          .out_o(wb_reg_inst)
          );

   // indicate that pipeline is still doing a gather/scatter operation
   assign intpipe_is_gscing = wb_reg_gscing;

   ////////////////////////////////////////////////////////////////////////////////
   // Valid, replays and exceptions
   ////////////////////////////////////////////////////////////////////////////////

   always_comb
     begin
        // Sends kill to VPU
        wb_vpu_kill = 1'b0;

        // New exception if CSR detects it
        wb_xcpt = wb_reg_xcpt || wb_csr_xcpt;

        // Valids
        wb_pre_valid = wb_reg_valid && !wb_reg_replay && !wb_xcpt && !wb_csr_replay;
        wb_valid     = wb_pre_valid && !wb_reg_gscing;

        // Take new PC upon replay, exception or eret
        wb_take_pc = wb_reg_replay || wb_xcpt || wb_csr_eret || wb_csr_replay;

        // CSR command
        wb_csr_badaddr = wb_dcache_resp.addr;
        wb_csr_rw_cmd  = wb_reg_valid ? wb_ctrl.csr : CsrN;
        wb_csr_en      = (wb_ctrl.csr != CsrN);

        // Invalidate load reserve to Dcache in case of exception
        wb_dcache_invalidate_lr = wb_xcpt;

        wb_reg_cause_ie = '{cause: wb_reg_cause, interrupt_x: wb_reg_xcpt_interrupt, default: '0};

     end

   ////////////////////////////////////////////////////////////////////////////////
   // Muxes the different results to the int RF write port
   ////////////////////////////////////////////////////////////////////////////////

   /* verilator lint_off UNUSEDSIGNAL */  // The preserved RF destination struct carries an FP tag bit that the integer writeback mux does not inspect.
   minion_reg_dest_t wb_fp_toint_resp_dest;  // Destination in WB stage from vpu unit
   /* verilator lint_on UNUSEDSIGNAL */
   logic             wb_fp_toint_wen;        // Write enable due a fp to int
   logic             wb_fp_toint_resp_valid; // vpu resp valid

   always_comb
     begin
        // Ready when the write port is not used by regular instructions neither
        // dcache
        /* verilator lint_off UNOPTFLAT */  // WB/FLB port-ready arbitration closes through preserved dcache/CSR feedback, but the actual cycle is cut by stage registers.
        wb_div_resp_ready = !(wb_dcache_wen || wb_wen || wb_fp_toint_wen);
        wb_flb_wen_ready  = !(wb_dcache_wen || wb_div_wen || wb_wen || wb_fp_toint_wen);
        /* verilator lint_on UNOPTFLAT */
     end

   always_comb
     begin
        // FP to int resp is ready at the last stage
        wb_fp_toint_resp_valid = id_vpu_ctrl.scoreboard.toint_valid[VpuTointScoreboardSize-1];
        wb_fp_toint_resp_dest  = id_vpu_ctrl.scoreboard.toint_dest[VpuTointScoreboardSize-1];

        // Write enable
        wb_dcache_wen    = wb_dcache_resp_valid && !wb_dcache_resp.dest.fp;          // Dcache
        wb_div_wen       = wb_div_resp_valid && wb_div_resp_ready;                   // Div
        wb_wen           = wb_valid && wb_ctrl.wxd && !wb_ctrl.fp && !wb_ctrl.mem && !wb_ctrl.div;  // Regular instruction
        wb_fp_toint_wen  = wb_fp_toint_resp_valid;                                   // fp to int from VPU
        wb_rf_wen        = wb_dcache_wen || wb_div_wen || wb_wen || wb_fp_toint_wen  // Final write enable to RF
                           || wb_flb_wen_valid;

        // Muxes the write addr
        wb_waddr = wb_reg_inst[11:7];
        wb_rf_waddr = wb_dcache_wen   ? wb_dcache_resp.dest.addr   // Data written by Dcache
                      : wb_fp_toint_wen ? wb_fp_toint_resp_dest.addr // Data written by vpu
                      : wb_div_wen      ? wb_div_resp_dest.addr      // Data written by Div/Mul unit
                      : wb_wen          ? wb_waddr                   // Data written by instruction in WB
                      :                   wb_flb_wen_addr;           // Data written by FastLocal Barrier

        // Muxes the data from the different sources
        wb_rf_wdata = wb_dcache_wen                                ? wb_dcache_resp.data[XregSize-1:0] // Data coming from Dcache
                      : wb_fp_toint_wen                              ? wb_vpu_ctrl.toint_data           // Data coming from fp to int
                      : wb_div_wen                                   ? wb_div_resp_data                 // Data coming from Div/Mul unit
                      : (wb_wen && (wb_ctrl.csr != CsrN))           ? wb_csr_rdata                     // Data coming from CSR
                      : wb_wen                                       ? wb_reg_wdata                     // Data coming from Int ALU pipeline
                      :                                                wb_flb_wen_data;                 // Data coming from FastLocal Barrier

        // Muxes the thread id
        wb_rf_thread_id = wb_dcache_wen   ? wb_dcache_resp.dest.thread_id   // Data coming from Dcache
                          : wb_fp_toint_wen ? wb_fp_toint_resp_dest.thread_id // Data coming from vpu
                          : wb_div_wen      ? wb_div_resp_dest.thread_id      // Data coming from Div/Mul unit
                          : wb_wen          ? wb_thread_id                    // Data coming from Int ALU pipeline
                          :                   wb_flb_wen_thread_id;           // Data coming from FastLocal Barrier
     end


   ////////////////////////////////////////////////////////////////////////////////
   // Gets the load data coming from the DCache
   ////////////////////////////////////////////////////////////////////////////////

   always_comb
     begin
        // Forwards DCache response to VPU
        wb_vpu_dcache_resp_valid     = wb_dcache_resp_valid && wb_dcache_resp.dest.fp;
        wb_vpu_dcache_resp.data      = wb_dcache_resp.data;
        wb_vpu_dcache_resp.typ       = wb_dcache_resp.typ;
        wb_vpu_dcache_resp.gdst      = wb_dcache_resp.gsc_cnt;
        wb_vpu_dcache_resp.addr      = wb_dcache_resp.dest.addr;
        wb_vpu_dcache_resp.ps_mask   = wb_dcache_resp.ps_mask;
        wb_vpu_dcache_resp.thread_id = wb_dcache_resp.dest.thread_id;
     end

   ////////////////////////////////////////////////////////////////////////////////
   // Generates the WB enable signal to the integer RF a cycle earlier
   ////////////////////////////////////////////////////////////////////////////////

   always_comb
     begin
        wb_rf_wen_early = mem_rfen_early                                                   // Integer instruction write
                          || mem_dcache_resp_int_valid                                        // Integer load write
                          || wb_div_resp_valid_early                                          // Divide/Mul write request
                          || id_vpu_ctrl.scoreboard.toint_valid[VpuTointScoreboardSize-2] // FP to INT write
                          || wb_flb_wen_valid_early;                                          // Fast Local Barrier
     end

   assign TE_wb_reg_inst = wb_reg_inst;
   assign TE_wb_reg_context = 1'b0;

   ////////////////////////////////////////////////////////////////////////////////
   //  Debug signals
   ////////////////////////////////////////////////////////////////////////////////



   always_comb begin

     in_debug_mode = debug;
     debug_busy = busy;
     debug_exception = debug_out_exception;


     debug_monitor_out.id_thread_id = id_thread_id;
     debug_monitor_out.id_pc = id_pc;
     debug_monitor_out.id_valid = id_valid;
     debug_monitor_out.id_valid_qual = id_valid_qual;
     debug_monitor_out.id_inst_replay = id_inst_replay;
     debug_monitor_out.id_take_pc = id_take_pc;
     debug_monitor_out.id_csr_interrupt = id_csr_interrupt[id_thread_id];
     debug_monitor_out.excl_mode_stall = (csr_excl_mode && (csr_excl_mode_sel != id_thread_id)) || csr_excl_mode_stall;
     debug_monitor_out.ex_valid_qual = ex_valid_qual;
     debug_monitor_out.tag_valid_qual = tag_valid_qual;
     debug_monitor_out.mem_valid_qual = mem_valid_qual;
     debug_monitor_out.wb_valid = wb_valid;
     debug_monitor_out.gsc_reg_gsc_cnt = gsc_reg_gsc_cnt;
     debug_monitor_out.tag_take_pc = tag_take_pc;
     debug_monitor_out.ipi_with_pc = ipi_with_pc;
     debug_monitor_out.wb_take_pc = wb_take_pc;
     debug_monitor_out.id_xcpt = id_xcpt;
     debug_monitor_out.wb_xcpt = wb_xcpt;
     debug_monitor_out.id_stall_fp2int = id_stall_fp2int;
     debug_monitor_out.ex_gsc_busy = ex_gsc_busy;
     debug_monitor_out.csr_fe_stall = csr_fe_stall[id_thread_id];
     debug_monitor_out.id_ctrl_stall_csr = id_ctrl_stall_csr;
     debug_monitor_out.id_ctrl_stall_dcache = id_ctrl_stall_dcache;
     debug_monitor_out.id_do_fence = id_do_fence;
     debug_monitor_out.id_ctrl_stall_trans = id_ctrl_stall_trans;
     debug_monitor_out.id_ctrl_stall_div = id_ctrl_stall_div;
     debug_monitor_out.id_int_sboard_hazard = id_int_sboard_hazard;
     debug_monitor_out.id_ctrl_stall_hazard_vpu = id_ctrl_stall_hazard_vpu;
     debug_monitor_out.id_ctrl_stall_hazard = id_ctrl_stall_hazard;
   end // always_comb

////////////////////////////////////////////////////////////////////////////
// Asserts
////////////////////////////////////////////////////////////////////////////

// synopsys translate_off

// X propagation on intpipe

/* verilator lint_off UNUSEDSIGNAL */  // These X-propagation masks exist only for translate-off debug checking and are intentionally unobserved in lint-only builds.
minion_control_t id_ctrl_to_ex_masked;
minion_control_t ex_ctrl_masked;
minion_control_t tag_ctrl_masked;
minion_control_t mem_ctrl_masked;
logic ex_reg_rvc_masked;
logic tag_reg_rvc_masked;
logic mem_reg_rvc_masked;

always_comb
begin
    id_ctrl_to_ex_masked          = id_ctrl_to_ex;
    id_ctrl_to_ex_masked.alu_fn   = id_uses_alu                                                 ? id_ctrl_to_ex.alu_fn   : alu_func_e'(0);
    id_ctrl_to_ex_masked.sel_alu2 = id_uses_alu                                                 ? id_ctrl_to_ex.sel_alu2 : a2_ctrl_e'(0);
    id_ctrl_to_ex_masked.sel_alu1 = id_uses_alu                                                 ? id_ctrl_to_ex.sel_alu1 : a1_ctrl_e'(0);
    id_ctrl_to_ex_masked.sel_imm  = id_uses_alu && (id_ctrl_to_ex.sel_alu2 == A2Imm) ? id_ctrl_to_ex.sel_imm  : imm_ctrl_e'(0);
    id_ctrl_to_ex_masked.alu_dw   = id_uses_alu && !id_ctrl_to_ex.br                            ? id_ctrl_to_ex.alu_dw   : dw_ctrl_e'(0);
    id_ctrl_to_ex_masked.mem_cmd  = id_ctrl_to_ex.mem                                           ? id_ctrl_to_ex.mem_cmd  : dcache_cmd_e'(0);
    id_ctrl_to_ex_masked.mem_type = id_ctrl_to_ex.mem                                           ? id_ctrl_to_ex.mem_type : dcache_type_e'(0);
    ex_ctrl_masked                = ex_ctrl;
    ex_ctrl_masked.sel_alu2       = ex_uses_alu                                                 ? ex_ctrl.sel_alu2 : a2_ctrl_e'(0);
    ex_ctrl_masked.sel_alu1       = ex_uses_alu                                                 ? ex_ctrl.sel_alu1 : a1_ctrl_e'(0);
    ex_ctrl_masked.sel_imm        = ex_uses_alu && (ex_ctrl.sel_alu2 == A2Imm)       ? ex_ctrl.sel_imm  : imm_ctrl_e'(0);
    ex_ctrl_masked.alu_dw         = ex_uses_alu && !ex_ctrl.br                                  ? ex_ctrl.alu_dw   : dw_ctrl_e'(0);
    ex_ctrl_masked.alu_fn         = ex_uses_alu                                                 ? ex_ctrl.alu_fn   : alu_func_e'(0);
    ex_ctrl_masked.mem_cmd        = ex_ctrl.mem                                                 ? ex_ctrl.mem_cmd  : dcache_cmd_e'(0);
    ex_ctrl_masked.mem_type       = ex_ctrl.mem                                                 ? ex_ctrl.mem_type : dcache_type_e'(0);
    ex_reg_rvc_masked = ex_reg_rvc && !ex_reg_xcpt_interrupt;
    tag_ctrl_masked               = tag_ctrl;
    tag_ctrl_masked.sel_alu2      = a2_ctrl_e'(0);                                           // Don't care (unused)
    tag_ctrl_masked.sel_alu1      = a1_ctrl_e'(0);                                           // Don't care (unused)
    tag_ctrl_masked.sel_imm       = imm_ctrl_e'(0);                                          // Don't care (unused)
    tag_ctrl_masked.alu_dw        = dw_ctrl_e'(0);                                           // Don't care (unused)
    tag_ctrl_masked.alu_fn        = tag_ctrl.wxd ? tag_ctrl.alu_fn : alu_func_e'(0);
    tag_ctrl_masked.mem_cmd       = tag_ctrl.mem ? tag_ctrl.mem_cmd  : dcache_cmd_e'(0);
    tag_ctrl_masked.mem_type      = tag_ctrl.mem ? tag_ctrl.mem_type : dcache_type_e'(0);
    tag_reg_rvc_masked = tag_reg_rvc && !tag_reg_xcpt_interrupt;
    mem_ctrl_masked               = mem_ctrl;
    mem_ctrl_masked.sel_alu2      = a2_ctrl_e'(0);                                           // Don't care (unused)
    mem_ctrl_masked.sel_alu1      = a1_ctrl_e'(0);                                           // Don't care (unused)
    mem_ctrl_masked.sel_imm       = imm_ctrl_e'(0);                                          // Don't care (unused)
    mem_ctrl_masked.alu_dw        = dw_ctrl_e'(0);                                           // Don't care (unused)
    mem_ctrl_masked.alu_fn        = alu_func_e'(0);                                          // Don't care (unused)
    mem_ctrl_masked.mem_cmd       = mem_ctrl.mem ? mem_ctrl.mem_cmd  : dcache_cmd_e'(0);
    mem_ctrl_masked.mem_type      = mem_ctrl.mem ? mem_ctrl.mem_type : dcache_type_e'(0);
    mem_reg_rvc_masked = mem_reg_rvc && !(mem_reg_xcpt_interrupt && mem_reg_xcpt);
end
/* verilator lint_on UNUSEDSIGNAL */

// synopsys translate_on

endmodule

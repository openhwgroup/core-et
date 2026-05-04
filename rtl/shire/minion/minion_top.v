// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module minion_top
`ifdef ET_SIMULATION #(
  parameter int SHIRE_ID  = 0,
  parameter int NEIGH_ID  = 0,
  parameter int MINION_ID = 0
) 
`endif (
  // System signals
  input  logic                                          clock,
  input  logic                                          reset,
  input  logic                                          reset_debug,
  input  logic                                          reset_non_debug,

  // DFT signals
  input  logic [`SHIRE_DFT_SCANIN_MINION_WIDTH-1:0]     dft__scanin,        //Scan In
  output logic [`SHIRE_DFT_SCANOUT_MINION_WIDTH-1:0]    dft__scanout,       //Scan Out
  input  logic                                          dft__scan_mode,     //Scan Mode
  input  logic                                          dft__reset_byp,     //Reset byp
  input  logic                                          dft__scan_enable,   //Scan Enable
  input  logic [`SHIRE_DFT_MODE_WIDTH-1:0]              dft__test_mode,     //Compression selection
  input  logic                                          dft__reset,         //DFT mode reset
  input  logic                                          dft__clock_gate_en, //For ICG TE connection
  input  logic [`SHIRE_DFT_CNTL_WIDTH-1:0]              dft__cntl,          //spare bus
  input  logic                                          dft__occ_scanin,    //OCC scan input
  output logic                                          dft__occ_scanout,   //OCC scan output
  input  logic                                          dft__occ_reset,     //OCC Reset
  input  logic                                          dft__occ_testmode,  //OCC testmode
  input  logic                                          dft__scan_ate_clk,  //ATE shift clock
  input  logic                                          dft__occ_bypass,    //OCC bypass

  // ECO ports
  input  logic [`ET_ECO_MINION_INPUT_WIDTH-1:0]         eco_i,
  output logic [`ET_ECO_MINION_OUTPUT_WIDTH-1:0]        eco_o,

  // Is this in the IOShire
  input  logic                                          ioshire,

  // Power control
  input  logic                                          nsleepin,
  input  logic                                          iso_enable,
  output logic                                          nsleepout,
  // Clock gate Ctrl override
  input  `toVector(minion_chicken_bits_t)               chicken_bits,
  // DCache request to L2 evict
  input  logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]         l2_dcache_evict_req_ready,
  output logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]         l2_dcache_evict_req_valid,
  output `toVector(et_link_minion_evict_req_info_t)     l2_dcache_evict_req,
  // DCache request to L2 miss
  input  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]          l2_dcache_miss_req_ready,
  output logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]          l2_dcache_miss_req_valid,
  output `toVector(et_link_minion_miss_req_info_t)      l2_dcache_miss_req,
  // L2 response to DCache
  output logic                                          l2_dcache_resp_ready,
  input  logic                                          l2_dcache_resp_valid,
  input  `toVector(et_link_minion_rsp_info_t)           l2_dcache_resp,
  // ICache requests
  input  logic                                          icache_req_ready,
  output logic                                          icache_req_valid,
  output `toVector(frontend_icache_req)                 icache_req,
  // ICache response
  input  logic                                          icache_resp_valid,
  input  logic                                          icache_resp_miss,
  input  `toVector(icache_frontend_resp)                icache_resp,
  input  logic                                          icache_fill_done,
  // ICache control
  output logic                                          icache_flush_data,
  // TLB/PTW control
  output minion_satp_info                               satp_info,
  output minion_satp_info                               matp_info,
  output logic                                          tlb_invalidate,
  // PTW request
  output `toVector(minion_ptw_req)                      dc_ptw_req_data,
  output logic                                          dc_ptw_req_valid,
  input  logic                                          dc_ptw_req_ready,
  // PTW response
  input  `toVector(minion_ptw_pte)                      ptw_dc_resp_data,
  input  logic                                          ptw_dc_resp_valid,
  // Interrupts
  input  `toVector(minion_interrupts)                   interrupts,
  // Thread ID
  input  logic [`NUM_SHIRE_IDS_R]                       shire_id,
  input  logic [`MIN_PER_S_R]                           shire_min_id,
  // Reset PC
  input  logic [`CORE_NR_THREADS-1:0]                   enabled,
  input  logic [`VA_RANGE]                              reset_vector,
  // PMA control
  input  `toVector(esr_mprot_t)                         mprot,
  // Minimum page size ESR
  input  `toVector(tlb_entry_type)                      vmspagesize,
  // Fast Local Barrier request interface with the neigh
  output logic                                          flb_neigh_req_valid,
  output logic [`CSR_FL_BARRIER_RANGE]                  flb_neigh_req_data,
  // Fast Local Barrier response interface with the neigh
  input  logic                                          flb_neigh_resp_valid,
  input  logic                                          flb_neigh_resp_data,
  // Trace Encoder signals
  input  logic                                          te_thread_sel,
  output `toVector(TraceEncoder_signals)                traceEncoder,
  input  logic                                          te_enable,

  // BPAM signals to minion interface
  // APBM bus channel
  input  logic [`MIN_DM_APB_ADDR_WIDTH - 1:0]           apb_paddr,
  input  logic                                          apb_penable,
  output logic [`ESR_APB_D_WIDTH - 1:0]                 apb_prdata,
  output logic                                          apb_pready,
  input  logic                                          apb_psel,
  output logic                                          apb_pslverr,
  input  logic [`ESR_APB_D_WIDTH - 1:0]                 apb_pwdata,
  input  logic                                          apb_pwrite,

  // Debug signals
  input  `toVector(minion_debug_in_t)                   debug_in,
  output `toVector(minion_debug_out_t)                  debug_out,
  output `toVector(neigh_sm_dbg_monitor_t)              minion_dbg_signals,
  input  logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]          minion_dbg_signals_mux,
  input  logic                                          minion_dbg_sig_enable,

  // ESR configuration
  input  logic [`ESR_MINION_FEATURE_SZ-1:0]             esr_features,
  input  logic                                          esr_bypass_dcache,
  input  logic                                          esr_shire_coop_mode,
  // Memory control override
  input  logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]    esr_minion_mem_override,
  // PMU signals
  output logic [`PMU_MINION_COUNTERS_RANGE]                       pmu_count_up,
  input  logic [`CORE_NR_THREADS-1:0][`XREG_RANGE]                pmu_read_data,
  output logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE] pmu_read_sel,
  output logic [`PMU_TOTAL_COUNTERS_RANGE]                        pmu_write_en,
  output logic [`XREG_RANGE]                                      pmu_write_data,
  output logic [`PMU_NEIGH_EVENT_CNT_SEL_RANGE]                   pmu_neigh_event_sel
);

////////////////////////////////////////////////////////////////////////////////
// Delays the resets (debug, non_debug too).
////////////////////////////////////////////////////////////////////////////////

  logic  reset_c_ff;
  logic  reset_d_ff;
  logic  reset_w_ff;

  rst_repeat rst_repeat_reset_ff           (.dft__reset_byp(dft__reset_byp), .dft__reset(dft__reset), .clock(clock), .reset_in(reset),           .reset_out(reset_c_ff));
  rst_repeat rst_repeat_reset_debug_ff     (.dft__reset_byp(dft__reset_byp), .dft__reset(dft__reset), .clock(clock), .reset_in(reset_debug),     .reset_out(reset_d_ff));
  rst_repeat rst_repeat_reset_non_debug_ff (.dft__reset_byp(dft__reset_byp), .dft__reset(dft__reset), .clock(clock), .reset_in(reset_non_debug), .reset_out(reset_w_ff));


////////////////////////////////////////////////////////////////////////////////
// Power control
////////////////////////////////////////////////////////////////////////////////

  // Only for simulation purposes bypass nsleep signal. In real device, UPF will
  // take care of these signals. Delay is arbitrary
  assign nsleepout = nsleepin;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Core module. It contains the frontend, the integer pipe and the dcache
// modules. It interfaces with the neighborhood and the VPU.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  minion_vpu_id_req          id_core_vpu_req;
  vpu_ctrl_sigs_t            id_vpu_decoder_sigs;
  minion_vpu_ex_req          ex_core_vpu_req;
  logic                      tag_core_vpu_kill;
  logic                      mem_core_vpu_kill;
  logic                      wb_core_vpu_kill;
  core_vpu_ctrl              f0_core_vpu_ctrl;
  logic                      wb_dcache_vpu_resp_valid;
  dcache_vpu_resp            wb_dcache_vpu_resp;
  vpu_dcache_ctrl            vpu_dcache_ctl;
  dcache_vpu_scp_resp        dcache_vpu_scp_res;
  logic [`DCACHE_DATA_RANGE] dcache_vpu_scp_data;
  logic [`DCACHE_DATA_RANGE] dcache_vpu_tenb_data;
  dcache_vpu_reduce_ctrl     vpu_reduce_ctrl;
  vpu_minion_id_ctrl         id_vpu_core_ctrl;
  vpu_minion_ex_ctrl         ex_vpu_core_ctrl;
  vpu_minion_tag_ctrl        f2_vpu_core_ctrl;
  vpu_minion_mem_ctrl        f3_vpu_core_ctrl;
  vpu_minion_wb_ctrl         wb_vpu_core_ctrl;
  logic [`CORE_NR_THREADS-1:0] enabled_reg;

  logic [`CSR_NR_EVENTS_VPU-1:0]            io_events_vpu;
  logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]      vpu_dbg_match;
  logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]     vpu_dbg_filter;
  logic [4:0] [`NEIGH_DEBUG_DATA_WIDTH-1:0] vpu_dbg_data;

  `toVector(minion_debug_in_t) debug_in_reg;

  // Signals for Debug Status Monitor
  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl0;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl0;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl0;

  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_tl1;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_tl1;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_tl1;

  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_ts;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_ts;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_ts;

  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_co;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_co;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_co;

  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_col2;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_col2;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_col2;

  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_MATCH_BITS-1:0]  sm_match_debug_signals_mh;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_debug_signals_mh;
  logic [`DCACHE_MH_FILE_SIZE-1:0][`DCACHE_DBG_SM_DATA_BITS-1:0]   sm_data_debug_signals_mh;

  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0]                            sm_match_debug_signals_dc;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0]                           sm_filter_debug_signals_dc;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0]                             sm_data_debug_signals_dc;

  intpipe_dbg_monitor_t                  intpipe_debug_monitor_out;
  intpipe_dbg_match_t                    intpipe_dbg_match;

  minion_chicken_bits_t                  chicken_bits_clock_gate_disable;
  logic                                  dcache_clock_gate_disable;
  logic                                  intpipe_clock_gate_disable;
  logic                                  frontend_clock_gate_disable;
  logic                                  vpulane_clock_gate_disable;
  logic                                  vputima_clock_gate_disable;
  logic                                  vputrans_clock_gate_disable;

  `RST_XFF(clock, reset_c_ff, chicken_bits_clock_gate_disable, chicken_bits, '0)
  assign dcache_clock_gate_disable   = chicken_bits_clock_gate_disable.min_dcache_clock_gate_disable;
  assign intpipe_clock_gate_disable  = chicken_bits_clock_gate_disable.min_intpipe_clock_gate_disable;
  assign frontend_clock_gate_disable = chicken_bits_clock_gate_disable.min_frontend_clock_gate_disable;
  assign vpulane_clock_gate_disable  = chicken_bits_clock_gate_disable.min_vpulane_clock_gate_disable;
  assign vputima_clock_gate_disable  = chicken_bits_clock_gate_disable.min_vputima_clock_gate_disable;
  assign vputrans_clock_gate_disable = chicken_bits_clock_gate_disable.min_vputrans_clock_gate_disable;

  always_comb begin
    intpipe_dbg_match.id_csr_interrupt   = intpipe_debug_monitor_out.id_csr_interrupt;
    intpipe_dbg_match.excl_mode_stall    = intpipe_debug_monitor_out.excl_mode_stall;
    intpipe_dbg_match.ipi_with_pc        = intpipe_debug_monitor_out.ipi_with_pc;
    intpipe_dbg_match.id_xcpt            = intpipe_debug_monitor_out.id_xcpt;
    intpipe_dbg_match.wb_xcpt            = intpipe_debug_monitor_out.wb_xcpt;
    intpipe_dbg_match.csr_fe_stall       = intpipe_debug_monitor_out.csr_fe_stall;
    intpipe_dbg_match.id_do_fence        = intpipe_debug_monitor_out.id_do_fence;
  end

  // Power Gating

  neigh_sm_dbg_monitor_t                minion_dbg_signals_next;
  logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0] minion_dbg_signals_mux_ff;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_next_0, minion_dbg_signals_next_1;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_0, minion_dbg_signals_1;

  //     CLK    EN                     DOUT                         DIN
  `EN_FF(clock, minion_dbg_sig_enable, minion_dbg_signals_0,        minion_dbg_signals_next_0)
  `EN_FF(clock, minion_dbg_sig_enable, minion_dbg_signals_1,        minion_dbg_signals_next_1)
  `EN_FF(clock, minion_dbg_sig_enable, minion_dbg_signals,          minion_dbg_signals_next)

  `EN_FF(clock, minion_dbg_sig_enable, minion_dbg_signals_mux_ff,   minion_dbg_signals_mux)

  //debug signals mux
  //minion_dbg_signals_mux_ff[6:2] : Module select
  //minion_dbg_signals_mux_ff[1:0] : Data select inside the module

  always_comb begin
    case (minion_dbg_signals_mux_ff[6:2])
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h0: begin //VPU0
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = vpu_dbg_match;
        minion_dbg_signals_next_0.filter_signals = vpu_dbg_filter;
        minion_dbg_signals_next_0.data_signals   = vpu_dbg_data[minion_dbg_signals_mux_ff[1:0]];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h1: begin //VPU1
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = vpu_dbg_match;
        minion_dbg_signals_next_0.filter_signals = vpu_dbg_filter;
        minion_dbg_signals_next_0.data_signals   = vpu_dbg_data[4];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h2: begin //DCACHE TL0
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = sm_match_debug_signals_tl0;
        minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_tl0;
        minion_dbg_signals_next_0.data_signals   = sm_data_debug_signals_tl0[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h3: begin //DCACHE TL1
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = sm_match_debug_signals_tl1;
        minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_tl1;
        minion_dbg_signals_next_0.data_signals   = sm_data_debug_signals_tl1[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h4: begin //DCACHE TS
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = sm_match_debug_signals_ts;
        minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_ts;
        minion_dbg_signals_next_0.data_signals   = sm_data_debug_signals_ts[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h5: begin //DCACHE C0
        minion_dbg_signals_next_0.filter_valid   = 1'b1;
        minion_dbg_signals_next_0.match_valid    = 1'b1;
        minion_dbg_signals_next_0.data_valid     = 1'b1;
        minion_dbg_signals_next_0.match_signals  = sm_match_debug_signals_co;
        minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_co;
        minion_dbg_signals_next_0.data_signals   = sm_data_debug_signals_co[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      default: minion_dbg_signals_next_0 = '0;
    endcase
  end

  always_comb begin
    case (minion_dbg_signals_mux_ff[6:2])
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h6: begin //DCACHE COL2
        minion_dbg_signals_next_1.filter_valid   = 1'b1;
        minion_dbg_signals_next_1.match_valid    = 1'b1;
        minion_dbg_signals_next_1.data_valid     = 1'b1;
        minion_dbg_signals_next_1.match_signals  = sm_match_debug_signals_col2;
        minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_col2;
        minion_dbg_signals_next_1.data_signals   = sm_data_debug_signals_col2[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h7: begin //DCACHE MH0
        minion_dbg_signals_next_1.filter_valid   = 1'b1;
        minion_dbg_signals_next_1.match_valid    = 1'b1;
        minion_dbg_signals_next_1.data_valid     = 1'b1;
        minion_dbg_signals_next_1.match_signals  = sm_match_debug_signals_mh[0];
        minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_mh[0];
        minion_dbg_signals_next_1.data_signals   = sm_data_debug_signals_mh[0][`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h8: begin //DCACHE MH1
        minion_dbg_signals_next_1.filter_valid   = 1'b1;
        minion_dbg_signals_next_1.match_valid    = 1'b1;
        minion_dbg_signals_next_1.data_valid     = 1'b1;
        minion_dbg_signals_next_1.match_signals  = sm_match_debug_signals_mh[1];
        minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_mh[1];
        minion_dbg_signals_next_1.data_signals   = sm_data_debug_signals_mh[1][`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'h9: begin //DCACHE DC
        minion_dbg_signals_next_1.filter_valid   = 1'b1;
        minion_dbg_signals_next_1.match_valid    = 1'b1;
        minion_dbg_signals_next_1.data_valid     = 1'b1;
        minion_dbg_signals_next_1.match_signals  = sm_match_debug_signals_dc;
        minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_dc;
        minion_dbg_signals_next_1.data_signals   = sm_data_debug_signals_dc[`DCACHE_DBG_SM_DATA_GROUP(minion_dbg_signals_mux_ff[1:0])];
      end
      `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'ha: begin //INTPIPE
        minion_dbg_signals_next_1.filter_valid   = 1'b1;
        minion_dbg_signals_next_1.match_valid    = 1'b1;
        minion_dbg_signals_next_1.data_valid     = 1'b1;
        minion_dbg_signals_next_1.filter_signals = `ZX(`NEIGH_DEBUG_FILTER_WIDTH, intpipe_debug_monitor_out);
        minion_dbg_signals_next_1.match_signals  = `ZX(`NEIGH_DEBUG_MATCH_WIDTH, intpipe_dbg_match);
        minion_dbg_signals_next_1.data_signals   = `ZX(`NEIGH_DEBUG_DATA_WIDTH, intpipe_debug_monitor_out);
      end
      default: minion_dbg_signals_next_1 = '0;
    endcase
  end

  always_comb begin
    if (minion_dbg_signals_mux_ff[5:2] > 4'h5) begin
      minion_dbg_signals_next = minion_dbg_signals_1;
    end else begin
      minion_dbg_signals_next = minion_dbg_signals_0;
    end
  end

  // group signals with low toggle rate, and do xor self gating
  `RST_XFF(clock, reset_c_ff, debug_in_reg, debug_in, '0)
  `RST_XFF(clock, reset_c_ff, enabled_reg,  enabled,  '0)

  // Memory override control
  logic [`MINION_MEM_CTRL_BITS-1:0] mem_ctrl_override;
  always_comb begin
    mem_ctrl_override = {dft__cntl[9], dft__cntl[9]};
    if (esr_minion_mem_override[`SHIRE_MINION_MEM_OVERRIDE_SEL]) begin
      mem_ctrl_override[`MINION_MEM_CTRL_WE] |= esr_minion_mem_override[`SHIRE_MINION_MEM_OVERRIDE_WE];
      mem_ctrl_override[`MINION_MEM_CTRL_RE] |= esr_minion_mem_override[`SHIRE_MINION_MEM_OVERRIDE_RE];
    end
  end

  // core instance
  core_top core (
    // System signals
    .clock                         ( clock                                      ),
    .reset_c                       ( reset_c_ff                                 ),
    .reset_w                       ( reset_w_ff                                 ),
    .reset_d                       ( reset_d_ff                                 ),
    // DFT signals
    .dft__reset_byp                ( dft__reset_byp                             ),
    .dft__reset                    ( dft__reset                                 ),
    // Is this in the IOShire
    .ioshire                       ( ioshire                                    ),
    // Chicken_bits
    .chicken_bit_dcache            ( dcache_clock_gate_disable                  ),
    .chicken_bit_intpipe           ( intpipe_clock_gate_disable                 ),
    .chicken_bit_frontend          ( frontend_clock_gate_disable                ),
    // DCache request to L2 evict
    .l2_dcache_evict_req_ready     ( l2_dcache_evict_req_ready                  ),
    .l2_dcache_evict_req_valid     ( l2_dcache_evict_req_valid                  ),
    .l2_dcache_evict_req           ( l2_dcache_evict_req                        ),
    // DCache request to L2 miss
    .l2_dcache_miss_req_ready      ( l2_dcache_miss_req_ready                   ),
    .l2_dcache_miss_req_valid      ( l2_dcache_miss_req_valid                   ),
    .l2_dcache_miss_req            ( l2_dcache_miss_req                         ),
    // L2 response to DCache
    .l2_dcache_resp_ready          ( l2_dcache_resp_ready                       ),
    .l2_dcache_resp_valid          ( l2_dcache_resp_valid                       ),
    .l2_dcache_resp                ( et_link_minion_rsp_info_t'(l2_dcache_resp) ),
    // ICache requests
    .icache_req_ready              ( icache_req_ready                           ),
    .icache_req_valid              ( icache_req_valid                           ),
    .icache_req                    ( icache_req                                 ),
    // ICache response
    .icache_resp_valid             ( icache_resp_valid                          ),
    .icache_resp_miss              ( icache_resp_miss                           ),
    .icache_resp                   ( icache_frontend_resp'(icache_resp)         ),
    .icache_fill_done              ( icache_fill_done                           ),
    // ICache control
    .icache_flush_data             ( icache_flush_data                          ),
    // TLB/PTW control
    .satp_info                     ( satp_info                                  ),
    .matp_info                     ( matp_info                                  ),
    .tlb_invalidate                ( tlb_invalidate                             ),
    // PTW request
    .dc_ptw_req_data               ( dc_ptw_req_data                            ),
    .dc_ptw_req_valid              ( dc_ptw_req_valid                           ),
    .dc_ptw_req_ready              ( dc_ptw_req_ready                           ),
    // PTW response
    .ptw_dc_resp_data              ( minion_ptw_pte'(ptw_dc_resp_data)          ),
    .ptw_dc_resp_valid             ( ptw_dc_resp_valid                          ),
    // Interrupts
    .interrupts                    ( minion_interrupts'(interrupts)             ),
    // Thread ID
    .shire_id                      ( shire_id                                   ),
    .shire_min_id                  ( shire_min_id                               ),
    // Reset PC
    .enabled                       ( enabled_reg                                ),
    .reset_vector                  ( reset_vector                               ),
    // PMA control
    .mprot                         ( esr_mprot_t'(mprot)                        ),
    // Minimum page size ESR
    .vmspagesize                   ( tlb_entry_type'(vmspagesize)               ),
    // Control signals going to VPU
    .id_vpu_req                    ( id_core_vpu_req                            ),
    .id_vpu_decoder_sigs           ( id_vpu_decoder_sigs                        ),
    .ex_vpu_req                    ( ex_core_vpu_req                            ),
    .tag_vpu_kill                  ( tag_core_vpu_kill                          ),
    .mem_vpu_kill                  ( mem_core_vpu_kill                          ),
    .wb_vpu_kill                   ( wb_core_vpu_kill                           ),
    .f0_vpu_ctrl                   ( f0_core_vpu_ctrl                           ),
    // WB Dcache response to VPU
    .wb_dcache_vpu_resp_valid      ( wb_dcache_vpu_resp_valid                   ),
    .wb_dcache_vpu_resp            ( wb_dcache_vpu_resp                         ),
    // DCache SRAM direct port for VPU
    .vpu_dcache_ctl                ( vpu_dcache_ctl                             ),
    .dcache_vpu_scp_res            ( dcache_vpu_scp_res                         ),
    .dcache_vpu_scp_data           ( dcache_vpu_scp_data                        ),
    .dcache_vpu_tenb_data          ( dcache_vpu_tenb_data                       ),
    // VPU reduce control
    .vpu_reduce_ctrl               ( vpu_reduce_ctrl                            ),
    // Control signals from VPU
    .id_vpu_ctrl                   ( id_vpu_core_ctrl                           ),
    .ex_vpu_ctrl                   ( ex_vpu_core_ctrl                           ),
    .f2_vpu_ctrl                   ( f2_vpu_core_ctrl                           ),
    .f3_vpu_ctrl                   ( f3_vpu_core_ctrl                           ),
    .wb_vpu_ctrl                   ( wb_vpu_core_ctrl                           ),
    // Fast Local Barrier request interface with the neigh
    .flb_neigh_req_valid           ( flb_neigh_req_valid                        ),
    .flb_neigh_req_data            ( flb_neigh_req_data                         ),
    // Fast Local Barrier response interface with the neigh
    .flb_neigh_resp_valid          ( flb_neigh_resp_valid                       ),
    .flb_neigh_resp_data           ( flb_neigh_resp_data                        ),
    //Trace Encoder signals
    .te_thread_sel                 ( te_thread_sel                              ),
    .traceEncoder                  ( traceEncoder                               ),
    .te_enable                     ( te_enable                                  ),
    // ESR configuration
    .esr_features                  ( esr_features                               ),
    .esr_bypass_dcache             ( esr_bypass_dcache                          ),
    .esr_shire_coop_mode           ( esr_shire_coop_mode                        ),
    // Memory control override
    .mem_ctrl_override             ( mem_ctrl_override                          ),
    // debug control
    .debug_in                      ( minion_debug_in_t'(debug_in_reg)           ),
    .debug_out                     ( debug_out                                  ),
    // apb slave signals from debug BPAM
    .apb_paddr                     ( apb_paddr                                  ),
    .apb_pwrite                    ( apb_pwrite                                 ),
    .apb_psel                      ( apb_psel                                   ),
    .apb_penable                   ( apb_penable                                ),
    .apb_pwdata                    ( apb_pwdata                                 ),
    .apb_pready                    ( apb_pready                                 ),
    .apb_prdata                    ( apb_prdata                                 ),
    .apb_pslverr                   ( apb_pslverr                                ),
    // PMU performance counters
    .pmu_count_up                  ( pmu_count_up                               ),
    .pmu_read_data                 ( pmu_read_data                              ),
    .pmu_read_sel                  ( pmu_read_sel                               ),
    .pmu_write_en                  ( pmu_write_en                               ),
    .pmu_write_data                ( pmu_write_data                             ),
    .pmu_neigh_event_sel           ( pmu_neigh_event_sel                        ),
    // Events for performance counters
    .io_events_vpu                 ( io_events_vpu                              ),
    // Signals for Debug Status Monitor
    .sm_match_debug_signals_tl0    ( sm_match_debug_signals_tl0                 ),
    .sm_filter_debug_signals_tl0   ( sm_filter_debug_signals_tl0                ),
    .sm_data_debug_signals_tl0     ( sm_data_debug_signals_tl0                  ),
    .sm_match_debug_signals_tl1    ( sm_match_debug_signals_tl1                 ),
    .sm_filter_debug_signals_tl1   ( sm_filter_debug_signals_tl1                ),
    .sm_data_debug_signals_tl1     ( sm_data_debug_signals_tl1                  ),
    .sm_match_debug_signals_ts     ( sm_match_debug_signals_ts                  ),
    .sm_filter_debug_signals_ts    ( sm_filter_debug_signals_ts                 ),
    .sm_data_debug_signals_ts      ( sm_data_debug_signals_ts                   ),
    .sm_match_debug_signals_co     ( sm_match_debug_signals_co                  ),
    .sm_filter_debug_signals_co    ( sm_filter_debug_signals_co                 ),
    .sm_data_debug_signals_co      ( sm_data_debug_signals_co                   ),
    .sm_match_debug_signals_col2   ( sm_match_debug_signals_col2                ),
    .sm_filter_debug_signals_col2  ( sm_filter_debug_signals_col2               ),
    .sm_data_debug_signals_col2    ( sm_data_debug_signals_col2                 ),
    .sm_match_debug_signals_mh     ( sm_match_debug_signals_mh                  ),
    .sm_filter_debug_signals_mh    ( sm_filter_debug_signals_mh                 ),
    .sm_data_debug_signals_mh      ( sm_data_debug_signals_mh                   ),
    .sm_match_debug_signals_dc     ( sm_match_debug_signals_dc                  ),
    .sm_filter_debug_signals_dc    ( sm_filter_debug_signals_dc                 ),
    .sm_data_debug_signals_dc      ( sm_data_debug_signals_dc                   ),
    .intpipe_debug_monitor_out     ( intpipe_debug_monitor_out                  )
  );


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// VPU module. This unit executes all the scalar floating precision
// instructions as well as all the packed instructions. It contains the RF and
// executing lanes. The core sends the instructions to the VPU, it executes
// them and sends any issues or stalls back to the core.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


  vpu_top vpu (
    // System signals
    .clock                  ( clock                       ),
    .reset                  ( reset_w_ff                  ),
    // Control signals coming from core
    .id_core_req            ( id_core_vpu_req             ),
    .id_vpu_decoder_sigs    ( id_vpu_decoder_sigs         ),
    .ex_core_req            ( ex_core_vpu_req             ),
    .f2_core_kill           ( tag_core_vpu_kill           ),
    .f3_core_kill           ( mem_core_vpu_kill           ),
    .f4_core_kill           ( wb_core_vpu_kill            ),
    .f0_core_ctrl           ( f0_core_vpu_ctrl            ),
    // Chicken_bits
    .chicken_bit_vpulane    ( vpulane_clock_gate_disable  ),
    .chicken_bit_vputima    ( vputima_clock_gate_disable  ),
    .chicken_bit_vputrans   ( vputrans_clock_gate_disable ),
    // Memory control override
    .mem_ctrl_override      ( mem_ctrl_override           ),
    // Dcache response
    .wb_dcache_resp_valid   ( wb_dcache_vpu_resp_valid    ),
    .wb_dcache_resp         ( wb_dcache_vpu_resp          ),
    // DCache SRAM direct port
    .dcache_ctrl            ( vpu_dcache_ctl              ),
    .dcache_scp_resp        ( dcache_vpu_scp_res          ),
    .dcache_scp_data        ( dcache_vpu_scp_data         ),
    .dcache_tenb_data       ( dcache_vpu_tenb_data        ),
    // Dcache reduce control
    .dcache_reduce_ctrl     ( vpu_reduce_ctrl             ),
    // Control signals going to core
    .id_core_ctrl           ( id_vpu_core_ctrl            ),
    .ex_core_ctrl           ( ex_vpu_core_ctrl            ),
    .f2_core_ctrl           ( f2_vpu_core_ctrl            ),
    .f3_core_ctrl           ( f3_vpu_core_ctrl            ),
    .wb_core_ctrl           ( wb_vpu_core_ctrl            ),
    // Events for performance counters
    .io_events              ( io_events_vpu               ),
    // Signals going to debug monitor
    .vpu_dbg_match          ( vpu_dbg_match               ),
    .vpu_dbg_filter         ( vpu_dbg_filter              ),
    .vpu_dbg_data           ( vpu_dbg_data                )
  ); //vpu_top


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Monitor signals that are flopped to meet timing. To be removed eventually.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


  et_eco_ports #(
    .INPUT_WIDTH  ( `ET_ECO_MINION_INPUT_WIDTH  ),
    .OUTPUT_WIDTH ( `ET_ECO_MINION_OUTPUT_WIDTH )
  ) eco_ports (
    .eco_o ( eco_o ),
    .eco_i ( eco_i )
  );


  // dft lint
  generate
    if (`ET_DFT_INSERTION == 0) begin : no_dft_insertion
      assign dft__scanout = '0;
      assign dft__occ_scanout = 1'b0;
    end
  endgenerate
  wire dft_unused_ok = &{
  `ifndef ET_ASCENT_LINT
    1'b0,
  `endif
    dft__scanin,
    dft__scan_mode,
    dft__scan_enable,
    dft__test_mode,
    dft__clock_gate_en,
    dft__cntl,
    dft__occ_scanin,
    dft__occ_reset,
    dft__occ_testmode,
    dft__scan_ate_clk,
    dft__occ_bypass
  };

  `ifdef ET_SIMULATION
    `include "dv/minion_common/dv_minion_top.vh"
  `endif // ET_SIMULATION

endmodule

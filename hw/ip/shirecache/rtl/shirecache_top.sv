// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache top-level — instantiates banks, crossbars, and mesh interfaces.
//
// Container of B banks of shirecache_bank, plus:
// - req_xbar from N neighborhoods to B banks
// - rsp_xbar from B banks to N neighborhoods
// - to_l3  mesh master from B banks to NumL3MasterPorts AXI ports
// - to_sys mesh master from B banks to NumSysPorts AXI ports
// - l3     mesh slave  from NumL3SlavePorts AXI ports to B banks
// - UC response relay FIFO
// - Reset synchronizers (prim_rst_sync)
//
// The original has a stub variant with shirecache_l3_to_sys_bridge for
// standalone pipe mode. That bridge is not instantiated here — in our
// reimplementation the full cache mode is always used with all 4 banks.
//
// Replaces: shire_cache (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_top
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumPorts          = Ports,       // neighborhoods + rbox
  parameter int unsigned NumBanks          = Banks,       // always 4
  parameter int unsigned NumL3MasterPorts  = L3MasterPorts,
  parameter int unsigned NumL3SlavePorts   = L3SlavePorts,
  parameter int unsigned NumSysPorts       = SysPorts,
  parameter int unsigned NumUc             = 1,
  parameter int unsigned NumBankReqqEntries = ReqqDepth,
  parameter int unsigned SetsPerSubBankParam = SetsPerSubBank,
  parameter bit          UseNocClkGating   = 1,
  parameter bit          IcacheMemsImplemented = 1'b1,
  parameter bit          L2hpfImplemented      = 1'b0
) (
  // ── Clocks and resets ────────────────────────────────
  input  logic                                             clk_i,         // cache clock
  input  logic                                             clk_free_i,    // free-running monitor clock (perfmon, trace)
  input  logic                                             rst_cold_ni,   // cold reset (active-low)
  input  logic                                             rst_ni,        // warm reset (active-low)
  input  logic                                             rst_debug_ni,  // debug reset (active-low)
  input  logic                                             noc_clk_i,     // mesh/NOC clock
  input  logic                                             noc_rst_ni,    // mesh/NOC reset (active-low)

  // ── DFT ──────────────────────────────────────────────
  input  dft_pkg::dft_t                                    dft_i,
  input  logic                                             dft_sram_clk_i,  // SRAM DFT clock override source

  // ── RAM config ───────────────────────────────────────
  input  ram_cfg_pkg::ram_cfg_t                            ram_cfg_i,

  // ── Neighborhood miss request bus ────────────────────
  // neigh_sc_req_ready[port][bank+uc]
  output logic [NumPorts-1:0][NumBanks+NumUc-1:0]          neigh_sc_req_ready_o,
  input  logic [NumPorts-1:0][NumBanks+NumUc-1:0]          neigh_sc_req_valid_i,
  input  etlink_pkg::req_t [NumPorts-1:0]                  neigh_sc_req_info_i,

  // ── Neighborhood miss response bus ───────────────────
  input  logic [NumPorts-1:0]                              neigh_sc_rsp_ready_i,
  output logic [NumPorts-1:0]                              neigh_sc_rsp_valid_o,
  output etlink_pkg::rsp_t [NumPorts-1:0]                  neigh_sc_rsp_info_o,

  // ── UC request/response (to uncached block in shire_channel) ──
  input  logic                                             neigh_uc_req_ready_i,
  output logic                                             neigh_uc_req_valid_o,
  output xbar_req_t                                        neigh_uc_req_info_o,

  output logic [NumPorts-1:0]                              neigh_uc_rsp_ready_o,
  input  logic [NumPorts-1:0]                              neigh_uc_rsp_valid_i,
  input  xbar_rsp_t                                        neigh_uc_rsp_info_i,

  // ── Per-bank ESR control/status ──────────────────────
  input  bank_esr_ctl_t [NumBanks-1:0]                     bank_esr_ctl_i,
  output bank_esr_status_t [NumBanks-1:0]                  bank_esr_status_o,

  // ── ESR outputs to UC block ──────────────────────────
  output logic                                             esr_sc_remote_l3_enable_o,
  output logic                                             esr_sc_remote_scp_enable_o,

  // ── Shire config from ESR ────────────────────────────
  input  esr_shire_config_t                                esr_shire_config_i,

  // ── Error indicators ─────────────────────────────────
  output logic [NumBanks-1:0]                              sc_bank_err_detected_o,
  output logic [NumBanks-1:0]                              sc_bank_err_logged_o,

  // ── L2HPF (Hardware Prefetcher) interface ────────────
  output logic                [NumBanks-1:0]               sc_neigh_l2hpf_req_valid_o,
  output neigh_l2hpf_req_t   [NumBanks-1:0]               sc_neigh_l2hpf_req_info_o,

  // ── Trace ────────────────────────────────────────────
  input  logic [BankIdSize-1:0]                            status_monitor_bank_sel_i,
  output trace_packet_t                                    sc_trace_data_o,
  output logic                                             sc_trace_valid_o,

  // ── to_l3 AXI master ports ──────────────────────────
  input  logic                      [NumL3MasterPorts-1:0] to_l3_axi_ar_ready_i,
  output logic                      [NumL3MasterPorts-1:0] to_l3_axi_ar_valid_o,
  output sc_master_ar_t             [NumL3MasterPorts-1:0] to_l3_axi_ar_o,
  input  logic                      [NumL3MasterPorts-1:0] to_l3_axi_aw_ready_i,
  output logic                      [NumL3MasterPorts-1:0] to_l3_axi_aw_valid_o,
  output sc_master_aw_t             [NumL3MasterPorts-1:0] to_l3_axi_aw_o,
  input  logic                      [NumL3MasterPorts-1:0] to_l3_axi_w_ready_i,
  output logic                      [NumL3MasterPorts-1:0] to_l3_axi_w_valid_o,
  output sc_master_w_t              [NumL3MasterPorts-1:0] to_l3_axi_w_o,
  output logic                      [NumL3MasterPorts-1:0] to_l3_axi_r_ready_o,
  input  logic                      [NumL3MasterPorts-1:0] to_l3_axi_r_valid_i,
  input  sc_master_r_t              [NumL3MasterPorts-1:0] to_l3_axi_r_i,
  output logic                      [NumL3MasterPorts-1:0] to_l3_axi_b_ready_o,
  input  logic                      [NumL3MasterPorts-1:0] to_l3_axi_b_valid_i,
  input  sc_master_b_t              [NumL3MasterPorts-1:0] to_l3_axi_b_i,

  // ── to_sys AXI master ports ─────────────────────────
  input  logic                      [NumSysPorts-1:0]      to_sys_axi_ar_ready_i,
  output logic                      [NumSysPorts-1:0]      to_sys_axi_ar_valid_o,
  output sc_master_ar_t             [NumSysPorts-1:0]      to_sys_axi_ar_o,
  input  logic                      [NumSysPorts-1:0]      to_sys_axi_aw_ready_i,
  output logic                      [NumSysPorts-1:0]      to_sys_axi_aw_valid_o,
  output sc_master_aw_t             [NumSysPorts-1:0]      to_sys_axi_aw_o,
  input  logic                      [NumSysPorts-1:0]      to_sys_axi_w_ready_i,
  output logic                      [NumSysPorts-1:0]      to_sys_axi_w_valid_o,
  output sc_master_w_t              [NumSysPorts-1:0]      to_sys_axi_w_o,
  output logic                      [NumSysPorts-1:0]      to_sys_axi_r_ready_o,
  input  logic                      [NumSysPorts-1:0]      to_sys_axi_r_valid_i,
  input  sc_master_r_t              [NumSysPorts-1:0]      to_sys_axi_r_i,
  output logic                      [NumSysPorts-1:0]      to_sys_axi_b_ready_o,
  input  logic                      [NumSysPorts-1:0]      to_sys_axi_b_valid_i,
  input  sc_master_b_t              [NumSysPorts-1:0]      to_sys_axi_b_i,

  // ── l3_slave AXI ports ──────────────────────────────
  output logic                      [NumL3SlavePorts-1:0]  l3_axi_ar_ready_o,
  input  logic                      [NumL3SlavePorts-1:0]  l3_axi_ar_valid_i,
  input  sc_slave_ar_t              [NumL3SlavePorts-1:0]  l3_axi_ar_i,
  output logic                      [NumL3SlavePorts-1:0]  l3_axi_aw_ready_o,
  input  logic                      [NumL3SlavePorts-1:0]  l3_axi_aw_valid_i,
  input  sc_slave_aw_t              [NumL3SlavePorts-1:0]  l3_axi_aw_i,
  output logic                      [NumL3SlavePorts-1:0]  l3_axi_w_ready_o,
  input  logic                      [NumL3SlavePorts-1:0]  l3_axi_w_valid_i,
  input  sc_master_w_t              [NumL3SlavePorts-1:0]  l3_axi_w_i,
  input  logic                      [NumL3SlavePorts-1:0]  l3_axi_r_ready_i,
  output logic                      [NumL3SlavePorts-1:0]  l3_axi_r_valid_o,
  output sc_slave_r_t               [NumL3SlavePorts-1:0]  l3_axi_r_o,
  input  logic                      [NumL3SlavePorts-1:0]  l3_axi_b_ready_i,
  output logic                      [NumL3SlavePorts-1:0]  l3_axi_b_valid_o,
  output sc_slave_b_t               [NumL3SlavePorts-1:0]  l3_axi_b_o,

  // ── Icache BIST ──────────────────────────────────────
  output icache_bist_req_t                                 icache_bist_req_info_o,
  input  icache_bist_rsp_t                                 icache_bist_rsp_info_i
);

  // ════════════════════════════════════════════════════════════
  // Reset synchronizers
  // ════════════════════════════════════════════════════════════
  //
  // The original has three resets: cold, warm, debug, each run through
  // rst_repeat (our prim_rst_sync). All are active-low in our design.

  logic rst_cold_n, rst_warm_n, rst_debug_n;

  prim_rst_sync u_rst_sync_cold (
    .clk_i  (clk_i),
    .rst_ni (rst_cold_ni),
    .dft_i  (dft_i),
    .rst_no (rst_cold_n)
  );

  prim_rst_sync u_rst_sync_warm (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_warm_n)
  );

  prim_rst_sync u_rst_sync_debug (
    .clk_i  (clk_i),
    .rst_ni (rst_debug_ni),
    .dft_i  (dft_i),
    .rst_no (rst_debug_n)
  );

  // ════════════════════════════════════════════════════════════
  // Request crossbar
  // ════════════════════════════════════════════════════════════
  //
  // Routes request from NumPorts sources (neighborhoods + rbox) to
  // NumBanks+NumUc destinations (cache banks + UC block).

  localparam int unsigned XbarReqW = $bits(xbar_req_t);
  localparam int unsigned XbarRspW = $bits(xbar_rsp_t);

  // Wrap et-link request with port_id and trans_id for the crossbar
  xbar_req_t [NumPorts-1:0] neigh_sc_xbar_req_info;
  xbar_rsp_t [NumPorts-1:0] neigh_sc_xbar_rsp_info;

  // After req xbar:
  logic [NumBanks+NumUc-1:0]                       bank_req_ready;
  logic [NumBanks+NumUc-1:0]                       bank_req_valid;
  xbar_req_t [NumBanks+NumUc-1:0]                  bank_req_info;
  // Raw DataWidth-bit wires for xbar data ports
  logic [NumPorts-1:0][XbarReqW-1:0]               xbar_req_src_data;
  logic [NumBanks+NumUc-1:0][XbarReqW-1:0]         xbar_req_dest_data;

  // Before rsp xbar:
  logic [NumBanks+NumUc-1:0][NumPorts-1:0]         bank_rsp_ready;
  logic [NumBanks+NumUc-1:0][NumPorts-1:0]         bank_rsp_2_credits;
  logic [NumBanks+NumUc-1:0][NumPorts-1:0]         bank_rsp_valid;
  xbar_rsp_t [NumBanks+NumUc-1:0]                  bank_rsp_info;
  logic [NumBanks+NumUc-1:0][XbarRspW-1:0]         xbar_rsp_src_data;
  logic [NumPorts-1:0][XbarRspW-1:0]               xbar_rsp_dest_data;

  // For incoming requests: mark destination bank and add port_id/trans_id
  always_comb begin
    for (int ii = 0; ii < NumPorts; ii++) begin
      neigh_sc_xbar_req_info[ii].port_id  = ii[PortIdSize-1:0];
      neigh_sc_xbar_req_info[ii].trans_id = '0;
      neigh_sc_xbar_req_info[ii].req_info = neigh_sc_req_info_i[ii];
      xbar_req_src_data[ii] = neigh_sc_xbar_req_info[ii];
    end
  end

  // Request crossbar: NumPorts → NumBanks+NumUc
  // SrcArb all 1 (arbitration found in the neighborhood)
  // AddOutputRelay: {1'b1 for UC, 1'b0 for banks} — output relay FIFO on UC only
  // NumSrcMask: all neighborhoods enabled (rbox depends on config, enable all)
  shirecache_xbar #(
    .NumSrc         (NumPorts),
    .NumSrcMask     ({NumPorts{1'b1}}),
    .SrcArb         ({NumPorts{1'b1}}),
    .NumDest        (NumBanks + NumUc),
    .AddOutputRelay ({1'b1, {NumBanks{1'b0}}}),
    .DataWidth      (XbarReqW)
  ) u_req_xbar (
    .clk_i          (clk_i),
    .rst_ni         (rst_warm_n),
    .src_ready_o    (neigh_sc_req_ready_o),
    .src_2_credits_o(neigh_sc_req_2_credits_unused),
    .src_valid_i    (neigh_sc_req_valid_i),
    .src_data_i     (xbar_req_src_data),
    .dest_ready_i   (bank_req_ready),
    .dest_valid_o   (bank_req_valid),
    .dest_data_o    (xbar_req_dest_data)
  );

  logic [NumPorts-1:0][NumBanks+NumUc-1:0] neigh_sc_req_2_credits_unused;

  // Convert xbar output raw data back to typed structs
  always_comb begin
    for (int ii = 0; ii < NumBanks + NumUc; ii++) begin
      bank_req_info[ii] = xbar_req_t'(xbar_req_dest_data[ii]);
    end
  end

  // Special req slave: UC (MSB of bank_req)
  assign neigh_uc_req_valid_o         = bank_req_valid[NumBanks];
  assign neigh_uc_req_info_o          = bank_req_info[NumBanks + NumUc - 1];
  assign bank_req_ready[NumBanks]     = neigh_uc_req_ready_i;

  // ════════════════════════════════════════════════════════════
  // Response crossbar
  // ════════════════════════════════════════════════════════════
  //
  // Routes responses from NumBanks+NumUc sources to NumPorts destinations.

  // Strip off port_id and trans_id from the response before sending out
  always_comb begin
    for (int ii = 0; ii < NumPorts; ii++) begin
      neigh_sc_rsp_info_o[ii] = neigh_sc_xbar_rsp_info[ii].rsp_info;
    end
  end

  // Convert typed structs to raw data for xbar
  always_comb begin
    for (int ii = 0; ii < NumBanks + NumUc; ii++) begin
      xbar_rsp_src_data[ii] = bank_rsp_info[ii];
    end
  end

  shirecache_xbar #(
    .NumSrc         (NumUc + NumBanks),
    .SrcArb         ({{NumUc{1'b0}}, {NumBanks{1'b1}}}),
    .NumDest        (NumPorts),
    .AddOutputRelay ({NumPorts{1'b1}}),
    .DataWidth      (XbarRspW)
  ) u_rsp_xbar (
    .clk_i          (clk_i),
    .rst_ni         (rst_warm_n),
    .src_ready_o    (bank_rsp_ready),
    .src_2_credits_o(bank_rsp_2_credits),
    .src_valid_i    (bank_rsp_valid),
    .src_data_i     (xbar_rsp_src_data),
    .dest_ready_i   (neigh_sc_rsp_ready_i),
    .dest_valid_o   (neigh_sc_rsp_valid_o),
    .dest_data_o    (xbar_rsp_dest_data)
  );

  // Convert xbar output raw data back to typed structs
  always_comb begin
    for (int ii = 0; ii < NumPorts; ii++) begin
      neigh_sc_xbar_rsp_info[ii] = xbar_rsp_t'(xbar_rsp_dest_data[ii]);
    end
  end

  // ════════════════════════════════════════════════════════════
  // Special response master: UC
  // ════════════════════════════════════════════════════════════
  //
  // UC response relay FIFO for timing closure at shire_cache level.

  logic [NumPorts-1:0] neigh_uc_rsp_ready_buf;
  logic [NumPorts-1:0] neigh_uc_rsp_valid_buf;
  xbar_rsp_t           neigh_uc_rsp_info_buf;

  logic [NumPorts-1:0] neigh_uc_rsp_ready_tmp;
  logic [NumPorts-1:0] neigh_uc_rsp_valid_tmp;

  assign neigh_uc_rsp_ready_o  = neigh_uc_rsp_ready_tmp;
  assign neigh_uc_rsp_valid_tmp = neigh_uc_rsp_valid_i;

  // UC rsp FIFO
  localparam int unsigned UcRspFifoW = NumPorts + XbarRspW;
  logic [NumPorts-1:0]       neigh_uc_rsp_valid_pre;
  logic                      neigh_uc_rsp_fifo_full;
  logic                      neigh_uc_rsp_fifo_pop;
  logic                      neigh_uc_rsp_fifo_valid;
  logic [UcRspFifoW-1:0]    uc_rsp_fifo_wdata;
  logic [UcRspFifoW-1:0]    uc_rsp_fifo_rdata;
  /* verilator lint_off UNUSEDSIGNAL */
  logic [2:0]                neigh_uc_rsp_fifo_level_unused;
  /* verilator lint_on UNUSEDSIGNAL */

  assign uc_rsp_fifo_wdata = {neigh_uc_rsp_valid_tmp, neigh_uc_rsp_info_i};

  prim_fifo_reg #(
    .Width (UcRspFifoW),
    .Depth (2)
  ) u_neigh_uc_rsp_fifo (
    .clk_i  (clk_i),
    .rst_ni (rst_warm_n),
    .push_i (|neigh_uc_rsp_valid_tmp),
    .wdata_i(uc_rsp_fifo_wdata),
    .full_o (neigh_uc_rsp_fifo_full),
    .level_o(neigh_uc_rsp_fifo_level_unused),
    .pop_i  (neigh_uc_rsp_fifo_pop),
    .valid_o(neigh_uc_rsp_fifo_valid),
    .rdata_o(uc_rsp_fifo_rdata)
  );

  assign {neigh_uc_rsp_valid_pre, neigh_uc_rsp_info_buf} = uc_rsp_fifo_rdata;
  assign neigh_uc_rsp_ready_tmp = neigh_uc_rsp_valid_tmp
                                & {NumPorts{~neigh_uc_rsp_fifo_full}};
  assign neigh_uc_rsp_valid_buf = neigh_uc_rsp_valid_pre
                                & {NumPorts{neigh_uc_rsp_fifo_valid}};
  assign neigh_uc_rsp_fifo_pop  = |neigh_uc_rsp_ready_buf;

  // Wire UC response into rsp crossbar
  assign neigh_uc_rsp_ready_buf         = bank_rsp_ready[NumBanks];
  assign bank_rsp_valid[NumBanks]       = neigh_uc_rsp_valid_buf;

  always_comb begin
    bank_rsp_info[NumBanks]             = neigh_uc_rsp_info_buf;
    bank_rsp_info[NumBanks].src_is_uc   = 1'b1;
  end

  // ════════════════════════════════════════════════════════════
  // Bank-to-mesh interconnect wires
  // ════════════════════════════════════════════════════════════

  // to_l3: banks → mesh master → L3 AXI
  logic                [NumBanks-1:0]            to_l3_mesh_master_bank_clk_en;
  logic                [NumBanks-1:0]            to_l3_mesh_master_bank_req_ready;
  logic                [NumBanks-1:0]            to_l3_mesh_master_bank_req_valid;
  mesh_master_req_t    [NumBanks-1:0]            to_l3_mesh_master_bank_req_info;
  logic                [NumBanks-1:0]            to_l3_mesh_master_bank_rsp_ready;
  logic                [NumBanks-1:0]            to_l3_mesh_master_bank_rsp_valid;
  mesh_master_rsp_t    [NumBanks-1:0]            to_l3_mesh_master_bank_rsp_info;

  // to_sys: banks → mesh master → sys AXI
  logic                [NumBanks-1:0]            to_sys_mesh_master_bank_clk_en;
  logic                [NumBanks-1:0]            to_sys_mesh_master_bank_req_ready;
  logic                [NumBanks-1:0]            to_sys_mesh_master_bank_req_valid;
  mesh_master_req_t    [NumBanks-1:0]            to_sys_mesh_master_bank_req_info;
  logic                [NumBanks-1:0]            to_sys_mesh_master_bank_rsp_ready;
  logic                [NumBanks-1:0]            to_sys_mesh_master_bank_rsp_valid;
  mesh_master_rsp_t    [NumBanks-1:0]            to_sys_mesh_master_bank_rsp_info;

  // l3_mesh_slave: L3 AXI → mesh slave → banks
  logic                [NumBanks-1:0]            l3_mesh_slave_bank_clk_en;
  logic                [NumBanks-1:0]            l3_mesh_slave_bank_req_ready;
  logic                [NumBanks-1:0]            l3_mesh_slave_bank_req_valid;
  mesh_slave_req_t     [NumBanks-1:0]            l3_mesh_slave_bank_req_info;
  logic                [NumBanks-1:0]            l3_mesh_slave_bank_rsp_ready;
  logic                [NumBanks-1:0]            l3_mesh_slave_bank_rsp_valid;
  mesh_slave_rsp_t     [NumBanks-1:0]            l3_mesh_slave_bank_rsp_info;

  // Per-bank trace
  trace_packet_t       [NumBanks-1:0]            bank_trace_data;
  logic                [NumBanks-1:0]            bank_trace_valid;

  // Per-bank ESR outputs
  logic                [NumBanks-1:0]            int_esr_sc_remote_l3_enable;
  logic                [NumBanks-1:0]            int_esr_sc_remote_scp_enable;
  l3_swizzle_ctl_t     [NumBanks-1:0]            int_esr_sc_l3_shire_swizzle_ctl;
  l3_swizzle_ctl_t                               esr_sc_l3_shire_swizzle_ctl;

  // ESRs to UC/l3_slave (only bank0 drives ESRs — closest physical bank)
  assign esr_sc_remote_l3_enable_o      = int_esr_sc_remote_l3_enable[0];
  assign esr_sc_remote_scp_enable_o     = int_esr_sc_remote_scp_enable[0];
  assign esr_sc_l3_shire_swizzle_ctl    = int_esr_sc_l3_shire_swizzle_ctl[0];

  // Per-bank icache BIST — only bank[NumBanks-1] drives icache BIST
  icache_bist_req_t    [NumBanks-1:0]            int_icache_bist_req_info;
  icache_bist_rsp_t    [NumBanks-1:0]            int_icache_bist_rsp_info;

  assign icache_bist_req_info_o = int_icache_bist_req_info[NumBanks-1];

  // Tie off unused bank BIST and ESR outputs
  logic int_unused_ok;
  always_comb begin
    int_unused_ok = '0;
    for (int ii = 0; ii < NumBanks; ii++) begin
      if (ii == NumBanks - 1) begin
        int_icache_bist_rsp_info[ii] = icache_bist_rsp_info_i;
      end else begin
        int_unused_ok &= (&int_icache_bist_req_info[ii]);
        int_icache_bist_rsp_info[ii] = '0;
      end
    end
    for (int ii = 1; ii < NumBanks; ii++) begin
      int_unused_ok &= int_esr_sc_remote_l3_enable[ii];
      int_unused_ok &= int_esr_sc_remote_scp_enable[ii];
      int_unused_ok &= (&int_esr_sc_l3_shire_swizzle_ctl[ii]);
    end
  end

  // ════════════════════════════════════════════════════════════
  // Cache banks
  // ════════════════════════════════════════════════════════════

  for (genvar g = 0; g < NumBanks; g++) begin : gen_banks
    localparam bit MbiImplemented = (g == NumBanks - 1) ? IcacheMemsImplemented : 1'b0;
    logic [MbistDataSize-1:0] bank_mbist_rdata_unused;

    /* verilator lint_off PINCONNECTEMPTY */  // The top wrapper intentionally leaves the bank ECO spare outputs unconsumed, matching the original et_eco_ports usage.
    shirecache_bank #(
      .NumPorts         (NumPorts),
      .NumEntries       (NumBankReqqEntries),
      .SetsPerSubBankP  (SetsPerSubBankParam),
      .L2HpfImplemented (L2hpfImplemented)
    ) u_bank (
      .clk_i                              (clk_i),
      .clk_free_i                         (clk_free_i),
      .rst_ni                             (rst_warm_n),
      .rst_c_ni                           (rst_cold_n),
      .rst_d_ni                           (rst_debug_n),
      .dft_i                              (dft_i),
      .dft_sram_clk_i                     (dft_sram_clk_i),
      .dft_mbist_en_i                     (1'b0),    // TODO: wire DFT MBIST enable
      .eco_i                              ('0),
      .eco_o                              (),
      .ram_cfg_i                          (ram_cfg_i),
      .mbi_implemented_const_i            (MbiImplemented),
      .my_bank_id_i                       (g[BankIdSize-1:0]),

      // MBIST shared interface (pre-insertion defaults)
      .mbist_en_i                         (1'b0),
      .mbist_sel_i                        ('0),
      .mbist_rd_en_i                      (1'b0),
      .mbist_wr_en_i                      (1'b0),
      .mbist_addr_i                       ('0),
      .mbist_wdata_i                      ('0),
      .mbist_rdata_o                      (bank_mbist_rdata_unused),

      // Xbar request
      .bank_req_ready_o                   (bank_req_ready[g]),
      .bank_req_valid_i                   (bank_req_valid[g]),
      .bank_req_info_i                    (bank_req_info[g]),

      // Xbar response
      .bank_rsp_valid_o                   (bank_rsp_valid[g]),
      .bank_rsp_info_o                    (bank_rsp_info[g]),
      .bank_rsp_ready_i                   (bank_rsp_ready[g]),
      .bank_rsp_2_credits_i              (bank_rsp_2_credits[g]),

      // to_l3 mesh master
      .to_l3_mesh_master_bank_clk_en_o    (to_l3_mesh_master_bank_clk_en[g]),
      .to_l3_mesh_master_bank_req_ready_i (to_l3_mesh_master_bank_req_ready[g]),
      .to_l3_mesh_master_bank_req_valid_o (to_l3_mesh_master_bank_req_valid[g]),
      .to_l3_mesh_master_bank_req_info_o  (to_l3_mesh_master_bank_req_info[g]),
      .to_l3_mesh_master_bank_rsp_ready_o (to_l3_mesh_master_bank_rsp_ready[g]),
      .to_l3_mesh_master_bank_rsp_valid_i (to_l3_mesh_master_bank_rsp_valid[g]),
      .to_l3_mesh_master_bank_rsp_info_i  (to_l3_mesh_master_bank_rsp_info[g]),

      // to_sys mesh master
      .to_sys_mesh_master_bank_clk_en_o   (to_sys_mesh_master_bank_clk_en[g]),
      .to_sys_mesh_master_bank_req_ready_i(to_sys_mesh_master_bank_req_ready[g]),
      .to_sys_mesh_master_bank_req_valid_o(to_sys_mesh_master_bank_req_valid[g]),
      .to_sys_mesh_master_bank_req_info_o (to_sys_mesh_master_bank_req_info[g]),
      .to_sys_mesh_master_bank_rsp_ready_o(to_sys_mesh_master_bank_rsp_ready[g]),
      .to_sys_mesh_master_bank_rsp_valid_i(to_sys_mesh_master_bank_rsp_valid[g]),
      .to_sys_mesh_master_bank_rsp_info_i (to_sys_mesh_master_bank_rsp_info[g]),

      // l3 mesh slave
      .l3_mesh_slave_bank_clk_en_i        (l3_mesh_slave_bank_clk_en[g]),
      .l3_mesh_slave_bank_req_ready_o     (l3_mesh_slave_bank_req_ready[g]),
      .l3_mesh_slave_bank_req_valid_i     (l3_mesh_slave_bank_req_valid[g]),
      .l3_mesh_slave_bank_req_info_i      (l3_mesh_slave_bank_req_info[g]),
      .l3_mesh_slave_bank_rsp_ready_i     (l3_mesh_slave_bank_rsp_ready[g]),
      .l3_mesh_slave_bank_rsp_valid_o     (l3_mesh_slave_bank_rsp_valid[g]),
      .l3_mesh_slave_bank_rsp_info_o      (l3_mesh_slave_bank_rsp_info[g]),

      // Icache BIST
      .icache_bist_req_info_o             (int_icache_bist_req_info[g]),
      .icache_bist_rsp_info_i             (int_icache_bist_rsp_info[g]),

      // ESR control/status
      .esr_ctl_i                          (bank_esr_ctl_i[g]),
      .esr_status_o                       (bank_esr_status_o[g]),
      .esr_shire_config_i                 (esr_shire_config_i),
      .err_detected_o                     (sc_bank_err_detected_o[g]),
      .err_logged_o                       (sc_bank_err_logged_o[g]),

      // Trace
      .bank_trace_data_o                  (bank_trace_data[g]),
      .bank_trace_valid_o                 (bank_trace_valid[g]),

      // L2HPF
      .l2hpf_req_valid_o                  (sc_neigh_l2hpf_req_valid_o[g]),
      .l2hpf_req_info_o                   (sc_neigh_l2hpf_req_info_o[g]),

      // ESR forwarded outputs
      .esr_sc_remote_l3_enable_o          (int_esr_sc_remote_l3_enable[g]),
      .esr_sc_remote_scp_enable_o         (int_esr_sc_remote_scp_enable[g]),
      .esr_sc_l3_shire_swizzle_ctl_o      (int_esr_sc_l3_shire_swizzle_ctl[g])
    );
    /* verilator lint_on PINCONNECTEMPTY */
  end

  // ════════════════════════════════════════════════════════════
  // Trace multiplexing
  // ════════════════════════════════════════════════════════════
  //
  // Select one bank's trace data to send to status monitor.

  logic [BankIdSize-1:0] status_monitor_bank_sel_q;

  always_ff @(posedge clk_i or negedge rst_debug_n) begin
    if (!rst_debug_n) status_monitor_bank_sel_q <= '0;
    else if (status_monitor_bank_sel_q != status_monitor_bank_sel_i)
      status_monitor_bank_sel_q <= status_monitor_bank_sel_i;
  end

  trace_packet_t sc_trace_data_pre;
  logic          sc_trace_valid_pre;

  assign sc_trace_data_pre  = bank_trace_data[status_monitor_bank_sel_q];
  assign sc_trace_valid_pre = bank_trace_valid[status_monitor_bank_sel_q];

  always_ff @(posedge clk_i or negedge rst_debug_n) begin
    if (!rst_debug_n) sc_trace_valid_o <= 1'b0;
    else if (sc_trace_valid_o | sc_trace_valid_pre)
      sc_trace_valid_o <= sc_trace_valid_pre;
  end

  always_ff @(posedge clk_i) begin
    if (sc_trace_valid_pre) sc_trace_data_o <= sc_trace_data_pre;
  end

  // ════════════════════════════════════════════════════════════
  // to_l3 mesh master
  // ════════════════════════════════════════════════════════════

  shirecache_mesh_master #(
    .IsToL3            (1),
    .NumBanks        (NumBanks),
    .NumPorts        (NumL3MasterPorts),
    .UseNocClkGating (UseNocClkGating)
  ) u_to_l3_mesh_master (
    .clk_i                          (clk_i),
    .rst_ni                         (rst_warm_n),
    .noc_clk_i                      (noc_clk_i),
    .noc_rst_ni                     (noc_rst_ni),
    .dft_i                          (dft_i),
    .esr_sc_l3_shire_swizzle_ctl_i  (esr_sc_l3_shire_swizzle_ctl),
    .mesh_master_bank_clk_en_i      (to_l3_mesh_master_bank_clk_en),
    .mesh_master_bank_req_ready_o   (to_l3_mesh_master_bank_req_ready),
    .mesh_master_bank_req_valid_i   (to_l3_mesh_master_bank_req_valid),
    .mesh_master_bank_req_info_i    (to_l3_mesh_master_bank_req_info),
    .mesh_master_bank_rsp_ready_i   (to_l3_mesh_master_bank_rsp_ready),
    .mesh_master_bank_rsp_valid_o   (to_l3_mesh_master_bank_rsp_valid),
    .mesh_master_bank_rsp_info_o    (to_l3_mesh_master_bank_rsp_info),
    .axi_ar_ready_i                 (to_l3_axi_ar_ready_i),
    .axi_ar_valid_o                 (to_l3_axi_ar_valid_o),
    .axi_ar_o                       (to_l3_axi_ar_o),
    .axi_aw_ready_i                 (to_l3_axi_aw_ready_i),
    .axi_aw_valid_o                 (to_l3_axi_aw_valid_o),
    .axi_aw_o                       (to_l3_axi_aw_o),
    .axi_w_ready_i                  (to_l3_axi_w_ready_i),
    .axi_w_valid_o                  (to_l3_axi_w_valid_o),
    .axi_w_o                        (to_l3_axi_w_o),
    .axi_r_ready_o                  (to_l3_axi_r_ready_o),
    .axi_r_valid_i                  (to_l3_axi_r_valid_i),
    .axi_r_i                        (to_l3_axi_r_i),
    .axi_b_ready_o                  (to_l3_axi_b_ready_o),
    .axi_b_valid_i                  (to_l3_axi_b_valid_i),
    .axi_b_i                        (to_l3_axi_b_i)
  );

  // ════════════════════════════════════════════════════════════
  // to_sys mesh master
  // ════════════════════════════════════════════════════════════

  shirecache_mesh_master #(
    .IsToL3            (0),
    .NumBanks        (NumBanks),
    .NumPorts        (NumSysPorts),
    .UseNocClkGating (UseNocClkGating)
  ) u_to_sys_mesh_master (
    .clk_i                          (clk_i),
    .rst_ni                         (rst_warm_n),
    .noc_clk_i                      (noc_clk_i),
    .noc_rst_ni                     (noc_rst_ni),
    .dft_i                          (dft_i),
    .esr_sc_l3_shire_swizzle_ctl_i  (esr_sc_l3_shire_swizzle_ctl),
    .mesh_master_bank_clk_en_i      (to_sys_mesh_master_bank_clk_en),
    .mesh_master_bank_req_ready_o   (to_sys_mesh_master_bank_req_ready),
    .mesh_master_bank_req_valid_i   (to_sys_mesh_master_bank_req_valid),
    .mesh_master_bank_req_info_i    (to_sys_mesh_master_bank_req_info),
    .mesh_master_bank_rsp_ready_i   (to_sys_mesh_master_bank_rsp_ready),
    .mesh_master_bank_rsp_valid_o   (to_sys_mesh_master_bank_rsp_valid),
    .mesh_master_bank_rsp_info_o    (to_sys_mesh_master_bank_rsp_info),
    .axi_ar_ready_i                 (to_sys_axi_ar_ready_i),
    .axi_ar_valid_o                 (to_sys_axi_ar_valid_o),
    .axi_ar_o                       (to_sys_axi_ar_o),
    .axi_aw_ready_i                 (to_sys_axi_aw_ready_i),
    .axi_aw_valid_o                 (to_sys_axi_aw_valid_o),
    .axi_aw_o                       (to_sys_axi_aw_o),
    .axi_w_ready_i                  (to_sys_axi_w_ready_i),
    .axi_w_valid_o                  (to_sys_axi_w_valid_o),
    .axi_w_o                        (to_sys_axi_w_o),
    .axi_r_ready_o                  (to_sys_axi_r_ready_o),
    .axi_r_valid_i                  (to_sys_axi_r_valid_i),
    .axi_r_i                        (to_sys_axi_r_i),
    .axi_b_ready_o                  (to_sys_axi_b_ready_o),
    .axi_b_valid_i                  (to_sys_axi_b_valid_i),
    .axi_b_i                        (to_sys_axi_b_i)
  );

  // ════════════════════════════════════════════════════════════
  // l3 mesh slave
  // ════════════════════════════════════════════════════════════

  shirecache_mesh_slave #(
    .NumBanks (NumBanks),
    .NumPorts (NumL3SlavePorts)
  ) u_l3_mesh_slave (
    .clk_i                          (clk_i),
    .rst_ni                         (rst_warm_n),
    .noc_clk_i                      (noc_clk_i),
    .noc_rst_ni                     (noc_rst_ni),
    .dft_i                          (dft_i),
    .esr_sc_l3_shire_swizzle_ctl_i  (esr_sc_l3_shire_swizzle_ctl),
    .mesh_slave_bank_clk_en_o       (l3_mesh_slave_bank_clk_en),
    .mesh_slave_bank_req_ready_i    (l3_mesh_slave_bank_req_ready),
    .mesh_slave_bank_req_valid_o    (l3_mesh_slave_bank_req_valid),
    .mesh_slave_bank_req_info_o     (l3_mesh_slave_bank_req_info),
    .mesh_slave_bank_rsp_ready_o    (l3_mesh_slave_bank_rsp_ready),
    .mesh_slave_bank_rsp_valid_i    (l3_mesh_slave_bank_rsp_valid),
    .mesh_slave_bank_rsp_info_i     (l3_mesh_slave_bank_rsp_info),
    .axi_ar_ready_o                 (l3_axi_ar_ready_o),
    .axi_ar_valid_i                 (l3_axi_ar_valid_i),
    .axi_ar_i                       (l3_axi_ar_i),
    .axi_aw_ready_o                 (l3_axi_aw_ready_o),
    .axi_aw_valid_i                 (l3_axi_aw_valid_i),
    .axi_aw_i                       (l3_axi_aw_i),
    .axi_w_ready_o                  (l3_axi_w_ready_o),
    .axi_w_valid_i                  (l3_axi_w_valid_i),
    .axi_w_i                        (l3_axi_w_i),
    .axi_r_ready_i                  (l3_axi_r_ready_i),
    .axi_r_valid_o                  (l3_axi_r_valid_o),
    .axi_r_o                        (l3_axi_r_o),
    .axi_b_ready_i                  (l3_axi_b_ready_i),
    .axi_b_valid_o                  (l3_axi_b_valid_o),
    .axi_b_o                        (l3_axi_b_o)
  );

  // ════════════════════════════════════════════════════════════
  // Unused signals
  // ════════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    neigh_sc_req_2_credits_unused,
    neigh_sc_xbar_rsp_info,            // src_is_uc field not used at top
    neigh_uc_rsp_info_buf.src_is_uc,
    bank_rsp_2_credits[NumBanks],      // UC does not need second xbar fifo credit
    int_unused_ok,
    rst_cold_n,                        // cold reset only used by banks
    unused_ok};

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNUSEDPARAM */

endmodule : shirecache_top

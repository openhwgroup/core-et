// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ESR package.
//
// Shared ET System Register types and address constants used by the
// standalone-shell ESR blocks.

package esr_pkg;
  /* verilator lint_off UNUSEDPARAM */  // Package exports additional ESR constants for modules still being ported.

  localparam int unsigned EsrApbDataWidth        = 64;
  localparam int unsigned EsrSpioApbAddrWidth    = 2;
  localparam int unsigned EsrSpioApbPpAddrWidth  = EsrSpioApbAddrWidth + 2;
  localparam int unsigned EsrNeighApbAddrWidth   = 13;
  localparam int unsigned EsrNeighApbPpAddrWidth = EsrNeighApbAddrWidth + 2;
  localparam int unsigned EsrNeighThreads        = 16;
  localparam int unsigned EsrIcacheSbeCntSize    = 8;
  localparam int unsigned EsrIcacheDbeCntSize    = 3;

  localparam logic [47:0] EsrNeighMinionBootResetVal = 48'h8000001000;
  localparam logic [15:0] EsrTextureCtlResetVal      = 16'h0005;
  localparam logic [3:0]  EsrIcacheErrLogCtlResetVal = 4'h6;

  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrMinionBoot =
      {2'd3, 13'h003};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrMprot =
      {2'd3, 13'h004};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrVmspagesize =
      {2'd3, 13'h007};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrIpiRedirectPc =
      {2'd0, 13'h008};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrHactrl =
      {2'd2, 13'h1ff0};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrHastatus0 =
      {2'd2, 13'h1ff1};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrHastatus1 =
      {2'd2, 13'h1ff2};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrAndOrTreeL0 =
      {2'd2, 13'h1ff3};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrPmuCtrl =
      {2'd3, 13'h00d};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrNeighChicken =
      {2'd3, 13'h00e};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrIcacheErrLogCtl =
      {2'd3, 13'h00f};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrIcacheErrLogInfo =
      {2'd3, 13'h010};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrIcacheErrLogAddress =
      {2'd3, 13'h011};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrIcacheSbeDbeCounts =
      {2'd3, 13'h012};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrTextureControl =
      {2'd0, 13'h1000};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrTextureStatus =
      {2'd0, 13'h1001};
  localparam logic [EsrNeighApbPpAddrWidth-1:0] EsrNeighAddrTextureImageTablePtr =
      {2'd0, 13'h1002};

  typedef struct packed {
    logic anyselected;
    logic anyunavailable;
    logic allhavereset;
    logic anyhavereset;
    logic allresumeack;
    logic anyresumeack;
    logic allrunning;
    logic anyrunning;
    logic allhalted;
    logic anyhalted;
  } esr_and_or_tree_l0_t;

  typedef struct packed {
    logic [15:0] resethalt;
    logic [15:0] hartmask;
    logic [15:0] hawindow;
  } esr_hactrl_t;

  typedef struct packed {
    logic [15:0] havereset;
    logic [15:0] resumeack;
    logic [15:0] running;
    logic [15:0] halted;
  } esr_hastatus0_t;

  typedef struct packed {
    logic [15:0] error;
    logic [15:0] exception;
    logic [15:0] busy;
  } esr_hastatus1_t;

  typedef struct packed {
    logic [46:0] address_msb;
    logic        reserved0;
  } esr_ipi_pc_t;

  typedef struct packed {
    logic       ctl_enable;
    logic       ctl_init;
    logic       ctl_start;
    logic [7:0] ctl_txn;
  } esr_dll_dly_est_ctl_t;

  typedef struct packed {
    logic [7:0] sts_errn;
    logic       sts_done;
  } esr_dll_dly_est_sts_t;

  typedef struct packed {
    logic       enable_secure_memory;
    logic [1:0] dram_size;
    logic       disable_osbox_access;
    logic       disable_pcie_access;
    logic [1:0] io_access_mode;
  } esr_mprot_t;

  typedef struct packed {
    logic [1:0] agent_forced;
    logic       force_all_agents;
    logic [1:0] dest_fifo;
    logic       force_dest_fifo;
    logic       bypass_dcache;
    logic       bypass_icache;
  } esr_neigh_chicken_t;

  typedef struct packed {
    logic disable_clock;
  } esr_pmu_ctrl_t;

  typedef struct packed {
    logic [6:0] reserved;
    logic       tb_only_lod0;
    logic       tb_single_tbpull;
    logic       tb_disable_mipmap;
    logic       tb_disable_aniso;
    logic       tb_single_texel;
    logic       tb_single_l2req;
    logic       tb_ignore_cg;
    logic       tb_force_cg;
    logic       tb_en;
  } esr_tex_ctl_t;

  typedef struct packed {
    logic       tb_en;
    logic       tb_idle;
    logic       tb_texsend_fifo_full;
    logic [4:0] tb_ibuff_oc;
    logic [2:0] tb_inf_tbpull_req;
    logic       tb_obuff_full;
    logic [3:0] reserved;
  } esr_tex_status_t;

  typedef struct packed {
    logic dmactive;
    logic ndmreset;
    logic clresethaltreq;
    logic setresethaltreq;
    logic hasel;
    logic ackhavereset;
    logic hartreset;
    logic resumereq;
    logic haltreq;
    logic write_enable;
  } esr_ms_dmctrl_t;

  localparam esr_mprot_t EsrMprotResetVal = '{
    dram_size: 2'h1,
    io_access_mode: 2'h3,
    default: '0
  };

  typedef struct packed {
    logic [31:0] reserved2;
    logic        haltreq;
    logic        resumereq;
    logic        hartreset;
    logic        ackhavereset;
    logic [23:0] reserved1;
    logic        setresethaltreq;
    logic        clrresethaltreq;
    logic        ndmreset;
    logic        reserved0;
  } esr_spdmctrl_t;

  typedef struct packed {
    logic [56:0] reserved;
    logic        error;
    logic        exception;
    logic        busy;
    logic        havereset;
    logic        resumeack;
    logic        running;
    logic        halted;
  } esr_sphastatus_t;

  localparam logic [EsrSpioApbPpAddrWidth-1:0] EsrSpioAddrSpdmctrl   = 4'hA;
  localparam logic [EsrSpioApbPpAddrWidth-1:0] EsrSpioAddrSphastatus = 4'hB;

  /* verilator lint_on UNUSEDPARAM */
endpackage

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_top_pwrstub (
  input logic                                   pwr_ctrl_glb_isolate,

  input  logic                                  coop_tload_slv_rdy_out_valid__pwrstub_in   ,
  output logic                                  coop_tload_slv_rdy_out_valid__pwrstub_out  ,
  input  logic [(`NUM_NEIGH-1)-1:0]             coop_tload_mst_done_out_valid__pwrstub_in  ,
  output logic [(`NUM_NEIGH-1)-1:0]             coop_tload_mst_done_out_valid__pwrstub_out ,

  input  APB_to_neigh_t                         APB_ESR_req__pwrstub_in                    ,
  output APB_to_neigh_t                         APB_ESR_req__pwrstub_out                   ,
  input  APB_from_neigh_t                       APB_ESR_rsp__pwrstub_in                    ,
  output APB_from_neigh_t                       APB_ESR_rsp__pwrstub_out                   ,

  input  logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_ready__pwrstub_in             ,
  output logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_ready__pwrstub_out            ,
  output logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_valid__pwrstub_out            ,
  input  logic [(`SC_BANKS+1)-1:0]              neigh_sc_req_valid__pwrstub_in             ,

  input  logic                                  neigh_sc_rsp_ready__pwrstub_in             ,
  input  logic                                  neigh_sc_rsp_valid__pwrstub_in             ,
  output logic                                  neigh_sc_rsp_ready__pwrstub_out            ,
  output logic                                  neigh_sc_rsp_valid__pwrstub_out            ,

  input  logic                                  esr_icache_prefetch_done__pwrstub_in       ,
  input  logic                                  esr_icache_err_detected__pwrstub_in        ,
  input  logic                                  esr_icache_err_logged__pwrstub_in          ,
  output logic                                  esr_icache_prefetch_done__pwrstub_out      ,
  output logic                                  esr_icache_err_detected__pwrstub_out       ,
  output logic                                  esr_icache_err_logged__pwrstub_out         ,

  input  logic                                  icache_f0_sram_resp_valid__pwrstub_in      ,
  input  logic                                  icache_f0_sram_resp_ready__pwrstub_in      ,
  output logic                                  icache_f0_sram_resp_valid__pwrstub_out     ,
  output logic                                  icache_f0_sram_resp_ready__pwrstub_out     ,

  input  esr_and_or_tree_L0_t                   esr_and_or_tree_L0__pwrstub_in             ,
  output esr_and_or_tree_L0_t                   esr_and_or_tree_L0__pwrstub_out            ,

  input  logic                                  flb_neigh_l2_req_valid__pwrstub_in         ,
  input  logic                                  flb_l2_neigh_resp_valid__pwrstub_in        ,
  output logic                                  flb_neigh_l2_req_valid__pwrstub_out        ,
  output logic                                  flb_l2_neigh_resp_valid__pwrstub_out       ,

  input  logic                                  icache_f2_sram_req_write__pwrstub_in       ,
  input  logic                                  icache_f2_sram_req_valid__pwrstub_in       ,
  input  logic                                  icache_f2_sram_req_ready__pwrstub_in       ,
  output logic                                  icache_f2_sram_req_write__pwrstub_out      ,
  output logic                                  icache_f2_sram_req_valid__pwrstub_out      ,
  output logic                                  icache_f2_sram_req_ready__pwrstub_out      ,

  input  bpam_rc_tbox_ack_t                     bpam_rc_tbox_ack_hi__pwrstub_in            ,
  output bpam_rc_tbox_ack_t                     bpam_rc_tbox_ack_hi__pwrstub_out           ,

  input  logic                                  reset_w_icache__pwrstub_in                 ,
  output logic                                  reset_w_icache__pwrstub_out                ,

  input  logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0] eco_o__pwrstub_in                          ,
  output logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0] eco_o__pwrstub_out                         ,

  input  logic                                  pwr_ctrl_glb_nsleepout_in                  ,
  input  logic [`MIN_PER_N-1:0]                 pwr_ctrl_min_nsleepout_in                  ,
  output logic                                  pwr_ctrl_glb_nsleepout_out                 ,
  output logic [`MIN_PER_N-1:0]                 pwr_ctrl_min_nsleepout_out

  );

  // Logical stubbing of the neigh_top interfaces

  // Logical stubbing of the neigh_top interfaces connected to the voltage
  // domain boundaires (where isolation cells are placed) in order to:
  // - automatically accept erroneus accesses to the powered off neigh logic
  // - disable toggling of logic from neigh_top to voltage domain crossing
  // boundaries.

  // COOP
  assign coop_tload_slv_rdy_out_valid__pwrstub_out  = (pwr_ctrl_glb_isolate)? 1'b0                  
      : coop_tload_slv_rdy_out_valid__pwrstub_in ;
  assign coop_tload_mst_done_out_valid__pwrstub_out = (pwr_ctrl_glb_isolate)? {(`NUM_NEIGH-1){1'b0}} 
      : coop_tload_mst_done_out_valid__pwrstub_in;
  // APB
  assign APB_ESR_req__pwrstub_out.apb_paddr         = APB_ESR_req__pwrstub_in.apb_paddr ;
  assign APB_ESR_req__pwrstub_out.apb_penable       = (pwr_ctrl_glb_isolate)? 1'b0 : APB_ESR_req__pwrstub_in.apb_penable ;
  assign APB_ESR_req__pwrstub_out.apb_psel          = (pwr_ctrl_glb_isolate)? 1'b0 : APB_ESR_req__pwrstub_in.apb_psel ;
  assign APB_ESR_req__pwrstub_out.apb_pwdata        = APB_ESR_req__pwrstub_in.apb_pwdata ;
  assign APB_ESR_req__pwrstub_out.apb_pwrite        = APB_ESR_req__pwrstub_in.apb_pwrite ;
  assign APB_ESR_rsp__pwrstub_out.apb_prdata        = APB_ESR_rsp__pwrstub_in.apb_prdata ;
  assign APB_ESR_rsp__pwrstub_out.apb_pready        = (pwr_ctrl_glb_isolate)? 1'b1 : APB_ESR_rsp__pwrstub_in.apb_pready  ; // doesn't hang the apb bus
  assign APB_ESR_rsp__pwrstub_out.apb_pslverr       = (pwr_ctrl_glb_isolate)? 1'b1 : APB_ESR_rsp__pwrstub_in.apb_pslverr ; // notify error
  // et-link neigh_sc L2 Req/Rsp bus
  assign neigh_sc_req_ready__pwrstub_out            = (pwr_ctrl_glb_isolate)? {(`SC_BANKS+1){1'b0}} : neigh_sc_req_ready__pwrstub_in ;
  assign neigh_sc_req_valid__pwrstub_out            = (pwr_ctrl_glb_isolate)? {(`SC_BANKS+1){1'b0}} : neigh_sc_req_valid__pwrstub_in ;
  assign neigh_sc_rsp_ready__pwrstub_out            = (pwr_ctrl_glb_isolate)? 1'b1 : neigh_sc_rsp_ready__pwrstub_in ; // Automatically accepts messages
  assign neigh_sc_rsp_valid__pwrstub_out            = (pwr_ctrl_glb_isolate)? 1'b0 : neigh_sc_rsp_valid__pwrstub_in ; // Do nothing with these messages
  // neigh_hi_voltage_cross
  // ESR icache prefetch
  assign esr_icache_prefetch_done__pwrstub_out      = (pwr_ctrl_glb_isolate)? 1'b1 : esr_icache_prefetch_done__pwrstub_in ; // Reset value at low voltage domain
  assign esr_icache_err_detected__pwrstub_out       = (pwr_ctrl_glb_isolate)? 1'b0 : esr_icache_err_detected__pwrstub_in ;
  assign esr_icache_err_logged__pwrstub_out         = (pwr_ctrl_glb_isolate)? 1'b0 : esr_icache_err_logged__pwrstub_in ;
  // Response from L1 SRAM blocks
  assign icache_f0_sram_resp_valid__pwrstub_out     = (pwr_ctrl_glb_isolate)? 1'b0 : icache_f0_sram_resp_valid__pwrstub_in  ;
  assign icache_f0_sram_resp_ready__pwrstub_out     = (pwr_ctrl_glb_isolate)? 1'b1 : icache_f0_sram_resp_ready__pwrstub_in  ; // Automatically accepts
  // neigh_lo_voltage_cross
  assign esr_and_or_tree_L0__pwrstub_out.anyselected   = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyselected;
  assign esr_and_or_tree_L0__pwrstub_out.anyunavailable= (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyunavailable;
  assign esr_and_or_tree_L0__pwrstub_out.allhavereset  = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.allhavereset;
  assign esr_and_or_tree_L0__pwrstub_out.anyhavereset  = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyhavereset;
  assign esr_and_or_tree_L0__pwrstub_out.allresumeack  = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.allresumeack;
  assign esr_and_or_tree_L0__pwrstub_out.anyresumeack  = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyresumeack;
  assign esr_and_or_tree_L0__pwrstub_out.allrunning    = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.allrunning;
  assign esr_and_or_tree_L0__pwrstub_out.anyrunning    = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyrunning;
  assign esr_and_or_tree_L0__pwrstub_out.allhalted     = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.allhalted;
  assign esr_and_or_tree_L0__pwrstub_out.anyhalted     = (pwr_ctrl_glb_isolate)? 1'b0 : esr_and_or_tree_L0__pwrstub_in.anyhalted;
  // FLB req/rsp if between neigh and L2
  assign flb_neigh_l2_req_valid__pwrstub_out    = (pwr_ctrl_glb_isolate)? 1'b0 : flb_neigh_l2_req_valid__pwrstub_in ;
  assign flb_l2_neigh_resp_valid__pwrstub_out   = (pwr_ctrl_glb_isolate)? 1'b0 : flb_l2_neigh_resp_valid__pwrstub_in  ;
  assign icache_f2_sram_req_write__pwrstub_out  = (pwr_ctrl_glb_isolate)? 1'b0 : icache_f2_sram_req_write__pwrstub_in ;
  assign icache_f2_sram_req_valid__pwrstub_out  = (pwr_ctrl_glb_isolate)? 1'b0 : icache_f2_sram_req_valid__pwrstub_in ;
  assign icache_f2_sram_req_ready__pwrstub_out  = (pwr_ctrl_glb_isolate)? 1'b0 : icache_f2_sram_req_ready__pwrstub_in  ;
  // Bpam ack
  assign bpam_rc_tbox_ack_hi__pwrstub_out.halt_ack   = (pwr_ctrl_glb_isolate)? 1'b1 : bpam_rc_tbox_ack_hi__pwrstub_in.halt_ack   ;
  assign bpam_rc_tbox_ack_hi__pwrstub_out.resume_ack = (pwr_ctrl_glb_isolate)? 1'b1 : bpam_rc_tbox_ack_hi__pwrstub_in.resume_ack ;
  // DLL feedback shire clock hiv
  // assign dll_feedback_shire = (pwr_ctrl_glb_isolate)? 1'b0 : dll_feedback_shire__pwrstub ;
  // ECO
  assign eco_o__pwrstub_out = (pwr_ctrl_glb_isolate)? {`ET_ECO_NEIGH_OUTPUT_WIDTH{1'b0}} : eco_o__pwrstub_in ;
  // reset_w_icache
  assign reset_w_icache__pwrstub_out = (pwr_ctrl_glb_isolate)? 1'b0 : reset_w_icache__pwrstub_in ;

  // power control nsleep_out
  assign pwr_ctrl_glb_nsleepout_out = (pwr_ctrl_glb_isolate)? 1'b0               : pwr_ctrl_glb_nsleepout_in ;
  assign pwr_ctrl_min_nsleepout_out = (pwr_ctrl_glb_isolate)? {`MIN_PER_N{1'b0}} : pwr_ctrl_min_nsleepout_in ;

endmodule


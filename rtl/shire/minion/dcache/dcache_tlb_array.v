// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_tlb_array (
  // System signals
  input  logic            clock,
  input  logic            reset,
  // ESRs
  input  tlb_entry_type   esr_vmspagesize,
  // Request to the TLB
  input  dcache_tlb_req   req_data,
  input  logic            req_valid,
  // Response with the physical bits
  output dcache_tlb_resp  resp_data,
  // TLB/PTW control
  input  minion_satp_info satp_info,
  input  minion_satp_info matp_info,
  input                   satp_info_en,
  input                   matp_info_en,
  input  logic            tlb_invalidate,
  // PTW request
  output minion_ptw_req   ptw_req_data,
  output logic            ptw_req_valid,
  input  logic            ptw_req_ready,
  // PTW response
  input  minion_ptw_pte   ptw_resp_data,
  input  logic            ptw_resp_valid,
  // VM enabled
  output logic            vm_enabled
);

  // INTERNAL DECLARATIONS
  minion_satp_info satp_info_reg;
  minion_satp_info matp_info_reg;

  // Flop pseudo-static configuration from minion
  // (m|s)atp_info_en come one cycle earlier that satp_info
  logic satp_info_en_del;
  logic matp_info_en_del;
  logic en_del;
  assign en_del = satp_info_en_del | satp_info_en | matp_info_en_del | matp_info_en;

  `RST_EN_FF(clock, reset, en_del, satp_info_en_del, satp_info_en, 1'b0)
  `RST_EN_FF(clock, reset, en_del, matp_info_en_del, matp_info_en, 1'b0)

  //       CLK    RST    DOUT           DIN        DEF
  `RST_EN_FF (clock, reset, satp_info_en_del, satp_info_reg, satp_info, '0)
  `RST_EN_FF (clock, reset, matp_info_en_del, matp_info_reg, matp_info, '0)

  // Connect request and response
  tlb_req tlb_req_data;
  tlb_resp tlb_resp_data;

  assign tlb_req_data.status      = req_data.status;
  assign tlb_req_data.vpn         = req_data.vpn;
  assign tlb_req_data.passthrough = req_data.passthrough;
  assign tlb_req_data.instruction = 1'b0;
  assign tlb_req_data.store       = req_data.store;
  assign tlb_req_data.msb_err     = req_data.msb_err;

  assign resp_data.fill_pending = tlb_resp_data.fill_pending;
  assign resp_data.miss         = tlb_resp_data.miss;
  assign resp_data.ppn          = tlb_resp_data.ppn;
  assign resp_data.xcpt_ld      = tlb_resp_data.xcpt_ld;
  assign resp_data.xcpt_st      = tlb_resp_data.xcpt_st;
  assign resp_data.access_fault = tlb_resp_data.access_fault;

  // Instantiate TLB
  logic ptw_invalidate_unused_ok;
  tlb_top #(
    .ENTRIES    ( `DCACHE_TLB_ENTRIES ),
    .NR_MINIONS ( 1                   )
  ) tlb_top (
    // System signals
    .clock          ( clock               ),
    .reset          ( reset               ),
    // ESRs
    .vmspagesize    ( esr_vmspagesize     ),
    .coop_mode      ( 1'b0                ),
    // Request to the TLB
    .req_min_id     ( '0                  ),
    .req_data       ( tlb_req_data        ),
    .req_valid      ( req_valid           ),
    // Response with the physical bits
    .resp_data      ( tlb_resp_data       ),
    // TLB/PTW control
    .satp_info      ( satp_info_reg       ),
    .matp_info      ( matp_info_reg       ),
    .tlb_invalidate ( tlb_invalidate      ),
    // PTW request
    .ptw_req_data   ( ptw_req_data        ),
    .ptw_req_valid  ( ptw_req_valid       ),
    .ptw_req_ready  ( ptw_req_ready       ),
    .ptw_invalidate ( ptw_invalidate_unused_ok ),
    // PTW response
    .ptw_resp_data  ( ptw_resp_data       ),
    .ptw_resp_valid ( ptw_resp_valid      ),
    // VM enabled flag
    .vm_enabled     ( vm_enabled          )
  );

endmodule


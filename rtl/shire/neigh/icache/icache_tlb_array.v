// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_tlb_array #(
  parameter ENTRIES      = 8,
  parameter NR_MINIONS   = 1,
  parameter NR_MINIONS_L = (NR_MINIONS == 1) ? 1 : $clog2(NR_MINIONS)
) (
  // System signals
  input  logic                             clock,
  input  logic                             reset,
  // ESRs
  input  tlb_entry_type                    esr_vmspagesize,
  input  logic                             esr_shire_coop_mode,
  // Request to the TLB
  input  logic [NR_MINIONS_L-1:0]          req_min_id,
  input  icache_tlb_req                    req_data,
  input  logic                             req_valid,
  // Response with the physical bits
  output icache_tlb_resp                   resp_data,
  // TLB/PTW control
  input  minion_satp_info [NR_MINIONS-1:0] satp_info,
  input  minion_satp_info [NR_MINIONS-1:0] matp_info,
  input  logic [NR_MINIONS-1:0]            tlb_invalidate,
  // PTW request
  output minion_ptw_req                    ptw_req_data,
  output logic                             ptw_req_valid,
  input  logic                             ptw_req_ready,
  output logic                             ptw_invalidate,
  // PTW response
  input  logic                             ptw_resp_valid,
  input  minion_ptw_pte                    ptw_resp_data
  );

  // Register for ESR
  tlb_entry_type                esr_vmspagesize_reg;

  // INTERNAL DECLARATIONS
  minion_satp_info [NR_MINIONS-1:0] satp_info_reg;
  minion_satp_info [NR_MINIONS-1:0] matp_info_reg;
  logic                             vm_enabled;

  // Flop pseudo-static configuration from minions

  // CLK    RST    DOUT           DIN        DEF
  `RST_FF (clock, reset, satp_info_reg, satp_info, '0)
  `RST_FF (clock, reset, matp_info_reg, matp_info, '0)

  // Connect request and response
  tlb_req  tlb_req_data;
  tlb_resp tlb_resp_data;

  assign tlb_req_data.status      = req_data.status;
  assign tlb_req_data.vpn         = req_data.vpn;
  assign tlb_req_data.passthrough = req_data.passthrough;
  assign tlb_req_data.instruction = 1'b1;
  assign tlb_req_data.store       = 1'b0;
  assign tlb_req_data.msb_err     = req_data.msb_err;

  assign resp_data.fill_pending = tlb_resp_data.fill_pending;
  assign resp_data.miss         = tlb_resp_data.miss;
  assign resp_data.ppn          = tlb_resp_data.ppn;
  assign resp_data.xcpt_if      = tlb_resp_data.xcpt_if;
  assign resp_data.access_fault = tlb_resp_data.access_fault;


  // Register for ESR
  `RST_EN_FF (clock, reset, |(esr_vmspagesize_reg^esr_vmspagesize),          esr_vmspagesize_reg,    esr_vmspagesize,          tlb_entry_type'(0))

  // Instantiate TLB
  tlb_top #(
    .ENTRIES        ( ENTRIES             ),
    .NR_MINIONS     ( NR_MINIONS          )
  ) tlb_top (
    // System signals
    .clock          ( clock               ),
    .reset          ( reset               ),
    // ESRs
    .vmspagesize    ( esr_vmspagesize_reg ),
    .coop_mode      ( esr_shire_coop_mode ),
    // Request to the TLB
    .req_min_id     ( req_min_id          ),
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
    .ptw_invalidate ( ptw_invalidate      ),
    // PTW response
    .ptw_resp_data  ( ptw_resp_data       ),
    .ptw_resp_valid ( ptw_resp_valid      ),
    // VM enabled
    .vm_enabled     ( vm_enabled          )
  );

  // Lint
  wire unused_ok = &{
    `ifndef ET_ASCENT_LINT
     1'b0,
     `endif
     vm_enabled
  };

endmodule


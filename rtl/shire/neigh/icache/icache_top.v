// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_top #(
  parameter NR_MINIONS       = 8,
  parameter NR_REQUESTORS    = 2,
  parameter NR_MIN_PER_REQ   = NR_MINIONS/NR_REQUESTORS,
  parameter NR_MIN_PER_REQ_L = (NR_MIN_PER_REQ == 1) ? 1 : $clog2(NR_MIN_PER_REQ)
) (
  // System signals
  input  logic                                           clock,
  input  logic                                           reset,
  // ESRs
  input  icache_prefetch_conf_t                          esr_prefetch_conf,
  input  logic                                           esr_prefetch_start,
  output logic                                           esr_prefetch_done,
  input  esr_icache_err_log_ctl_t                        esr_err_log_ctl,
  output logic                                           esr_err_log_sbe,
  output logic                                           esr_err_log_dbe,
  output icache_err_log_info_t                           esr_err_log_info,
  input  esr_mprot_t                                     esr_mprot,
  input  tlb_entry_type                                  esr_vmspagesize,
  input  logic                                           esr_bypass_icache,
  input  logic                                           esr_shire_coop_mode,
  // Request port
  output logic [NR_REQUESTORS-1:0]                       f0_req_ready,
  input  logic [NR_REQUESTORS-1:0]                       f0_req_valid,
  input  frontend_icache_req [NR_REQUESTORS-1:0]         f0_req,
  input  logic [NR_REQUESTORS-1:0][NR_MIN_PER_REQ_L-1:0] f0_req_min_id,
  // Response
  output logic [NR_REQUESTORS-1:0]                       f4_resp_valid,
  output logic [NR_REQUESTORS-1:0]                       f4_resp_miss,
  output icache_frontend_resp [NR_REQUESTORS-1:0]        f4_resp,
  output logic [NR_REQUESTORS-1:0]                       f5_resp_fill_done,
  // Flush control
  input  logic                                           f0_flush_data,
  // Request to L2 port
  input  logic                                           f0_l2_miss_req_disable_next,
  input  logic                                           f0_l2_miss_req_ready,
  output logic                                           f0_l2_miss_req_valid,
  output et_link_nodata_req_info_t                       f0_l2_miss_req,
  // Fill port
  output logic                                           f0_l2_miss_resp_ready,
  input  logic                                           f0_l2_miss_resp_valid,
  input  et_link_rsp_info_t                              f0_l2_miss_resp,
  // TLB/PTW control
  input  minion_satp_info [NR_MINIONS-1:0]               satp_info,
  input  minion_satp_info [NR_MINIONS-1:0]               matp_info,
  input  logic [NR_MINIONS-1:0]                          tlb_invalidate,
  // PTW request
  output minion_ptw_req [NR_REQUESTORS-1:0]              ptw_req_data,
  output logic [NR_REQUESTORS-1:0]                       ptw_req_valid,
  input  logic [NR_REQUESTORS-1:0]                       ptw_req_ready,
  output logic [NR_REQUESTORS-1:0]                       ptw_invalidate,
  // PTW response
  input  logic [NR_REQUESTORS-1:0]                       ptw_resp_valid,
  input  minion_ptw_pte [NR_REQUESTORS-1:0]              ptw_resp_data,
  // Request to L1 SRAM blocks
  output logic                                           f2_sram_req_write,
  output logic [`ICACHE_SRAM_ADD_WIDTH-1:0]              f2_sram_req_addr,
  output logic                                           f2_sram_req_valid,
  input  logic                                           f2_sram_req_ready,
  // Response from L1 SRAM blocks
  input  logic [`ICACHE_SRAM_DATA_WIDTH-1:0]             f0_sram_resp_dout,
  input  logic                                           f0_sram_resp_valid,
  output logic                                           f0_sram_resp_ready,
  // APB signals from BPAM for debug
  input  logic [`ICACHE_NEIGH_APB_ADDR_WIDTH-1:0]        apb_paddr,
  input  logic                                           apb_pwrite,
  input  logic                                           apb_psel,
  input  logic                                           apb_penable,
  input  logic [`bpam_shire_apb_data_width-1:0]          apb_pwdata,
  output logic                                           apb_pready,
  output logic [`bpam_shire_apb_data_width-1:0]          apb_prdata,
  output logic                                           apb_pslverr,
  // Output debug signals going to Status Monitor
  output icache_dbg_sm_t                                 dbg_sm_signals
  );

  localparam NR_REQUESTORS_L  = (NR_REQUESTORS == 1) ? 1 : $clog2(NR_REQUESTORS);

  // INTERNAL DECLARATIONS
  logic                                                        f0_l1_miss_req_ready;
  logic                                                        f0_l1_miss_req_ready_next;
  logic                                                        f0_l1_miss_req_valid;
  logic                                                        f0_l1_miss_req_valid_next;
  logic [`PA_RANGE]                                            f0_l1_miss_req_addr;
  logic [`PA_RANGE]                                            f0_l1_miss_req_addr_next;
  logic [NR_REQUESTORS_L-1:0]                                  f0_l1_miss_req_idx;
  logic [NR_REQUESTORS_L-1:0]                                  f0_l1_miss_req_idx_next;
  logic                                                        f0_l1_miss_resp_early_valid;
  logic                                                        f0_l1_miss_resp_valid;
  logic [`ICACHE_BLOCK_BITS-1:0]                               f0_l1_miss_resp_data;
  logic                                                        f0_l1_miss_resp_ecc_err;
  logic                                                        f0_l1_miss_resp_ecc_err_next;
  logic                                                        f0_l1_miss_resp_l2_err;
  logic                                                        f0_data_array_resp_valid;
  logic [`ICACHE_BLOCK_BITS-1:0]                               f0_data_array_resp_data;
  logic [`ICACHE_ECC_BLOCKS-1:0]                               f0_data_array_resp_sbe_per_block;
  logic [`ICACHE_ECC_BLOCKS-1:0]                               f0_data_array_resp_dbe_per_block;
  logic                                                        f0_da_sbe;
  logic                                                        f0_da_dbe;
  logic                                                        f0_l2_err;
  logic [`PA_RANGE]                                            f1_paddr;
  logic [`ICACHE_WAY_RANGE]                                    f1_read_way; // Encoded way for the read in F1
  logic [`ICACHE_WAY_RANGE]                                    f1_write_way; // Encoded way for the write in F1
  logic [`ICACHE_SET_RANGE]                                    f1_set;
  logic                                                        f1_hit; // Tag hit in F1
  logic                                                        f1_miss; // Tag miss in F1
  logic [`ICACHE_WAYS-1:0]                                     f1_replace_way; // Victim selected in F0
  logic                                                        f1_lru_update_en; // Update the LRU of the set
  logic [`ICACHE_WAYS-1:0]                                     f1_accessed_way; // Last way accessed
  et_link_rsp_opcode_t                                         f0_l2_miss_resp_opcode_reg;
  logic                                                        f0_l2_miss_resp_valid_reg;
  logic [`ICACHE_DBG_TAG_ADDR_WIDTH-1:0]                       tag_apb_paddr;
  logic                                                        tag_apb_pwrite;
  logic                                                        tag_apb_psel;
  logic                                                        tag_apb_penable;
  logic [`bpam_shire_apb_data_width-1:0]                       tag_apb_pwdata;
  logic                                                        tag_apb_pready;
  logic [`bpam_shire_apb_data_width-1:0]                       tag_apb_prdata;
  logic                                                        tag_apb_pslverr;
  logic [NR_REQUESTORS-1:0][`ICACHE_DBG_UCACHE_ADDR_WIDTH-1:0] ucache_apb_paddr;
  logic [NR_REQUESTORS-1:0]                                    ucache_apb_pwrite;
  logic [NR_REQUESTORS-1:0]                                    ucache_apb_psel;
  logic [NR_REQUESTORS-1:0]                                    ucache_apb_penable;
  logic [NR_REQUESTORS-1:0][`bpam_shire_apb_data_width-1:0]    ucache_apb_pwdata;
  logic [NR_REQUESTORS-1:0]                                    ucache_apb_pready;
  logic [NR_REQUESTORS-1:0][`bpam_shire_apb_data_width-1:0]    ucache_apb_prdata;
  logic [NR_REQUESTORS-1:0]                                    ucache_apb_pslverr;
  icache_dbg_sm_t [NR_REQUESTORS-1:0]                          ucache_dbg_sm_signals;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F0 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  icache_miss_state                                 f0_miss_state, f0_miss_state_next; // Miss FSM state
  logic [`PA_RANGE]                                 f0_paddr; // Address for the miss request
  logic [`ICACHE_WAY_RANGE]                         f0_read_way; // Encoded way for the read in F0
  logic [`ICACHE_WAY_RANGE]                         f0_write_way; // Encoded way for the write in F0
  logic                                             f0_l2_miss_resp_err;
  logic                                             f0_l2_miss_resp_done;
  logic                                             f0_l2_miss_resp_done_next;
  logic                                             f0_l1_resp_done;
  logic                                             f0_l1_resp_done_next;
  icache_prefetch_conf_t [NR_REQUESTORS-1:0]        ucache_prefetch_conf;
  logic [NR_REQUESTORS-1:0]                         ucache_prefetch_start;
  logic [NR_REQUESTORS-1:0]                         ucache_prefetch_done;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_ready;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_valid;
  logic [NR_REQUESTORS-1:0][`PA_RANGE]              ucache_l1_miss_req_addr;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_ff_available;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_ff_available_next;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_ff_in_accepted;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_req_ff_out_accepted;
  logic [NR_REQUESTORS-1:0][`PA_RANGE]              ucache_l1_miss_req_addr_ff;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_resp_early_valid;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_resp_valid;
  logic [NR_REQUESTORS-1:0][`ICACHE_BLOCK_BITS-1:0] ucache_l1_miss_resp_data;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_resp_ecc_err;
  logic [NR_REQUESTORS-1:0]                         ucache_l1_miss_resp_l2_err;
  logic                                             esr_err_log_sbe_next;
  logic                                             esr_err_log_dbe_next;
  icache_err_log_info_t                             esr_err_log_info_next;


  // CLK    RST    EN                                           DOUT                     DIN                        DEF
  `RST_FF(clock, reset,                                              f0_miss_state,           f0_miss_state_next,        icache_miss_state_Ready)
  `EN_FF (clock,        f1_miss | f1_hit,                            f0_paddr,                f1_paddr)
  `EN_FF (clock,        f1_hit,                                      f0_read_way,             f1_read_way)
  `EN_FF (clock,        f1_miss,                                     f0_write_way,            f1_write_way)
  `RST_FF(clock, reset,                                              f0_l2_miss_resp_done,    f0_l2_miss_resp_done_next, 1'b0)
  `RST_FF(clock, reset,                                              f0_l1_resp_done,         f0_l1_resp_done_next,      1'b0)
  `EN_FF (clock,        f0_data_array_resp_valid,                    f0_l1_miss_resp_data,    f0_data_array_resp_data)
  `EN_FF (clock,        f0_data_array_resp_valid,                    f0_l1_miss_resp_ecc_err, f0_l1_miss_resp_ecc_err_next)
  `EN_FF (clock,        f0_l2_miss_resp_valid_reg,                   f0_l2_err,               f0_l2_miss_resp_err)
  `RST_FF(clock, reset,                                              esr_err_log_sbe,         esr_err_log_sbe_next,      1'b0)
  `RST_FF(clock, reset,                                              esr_err_log_dbe,         esr_err_log_dbe_next,      1'b0)
  `EN_FF (clock,        esr_err_log_sbe_next | esr_err_log_dbe_next, esr_err_log_info,        esr_err_log_info_next)

  // //////////////////////////////////////////////////////////////////////////////
  // Instantiate L0 micro caches
  // //////////////////////////////////////////////////////////////////////////////

  // Connect prefetch only to ucache 0
  assign esr_prefetch_done = ucache_prefetch_done[0];

  for (genvar i = 0; i < NR_REQUESTORS; i++) begin : UCACHE
    // Connect prefetch only to ucache 0
    assign ucache_prefetch_conf[i]  = i == 0 ? esr_prefetch_conf  : '0;
    assign ucache_prefetch_start[i] = i == 0 ? esr_prefetch_start : 1'b0;

    icache_micro_cache #(
      .ID                          ( i                                                  ),
      .NR_MINIONS                  ( NR_MIN_PER_REQ                                     )
    ) micro_cache (
      // System signals
      .clock                       ( clock                                              ),
      .reset                       ( reset                                              ),
      // ESRs
      .esr_prefetch_conf           ( ucache_prefetch_conf[i]                            ),
      .esr_prefetch_start          ( ucache_prefetch_start[i]                           ),
      .esr_prefetch_done           ( ucache_prefetch_done[i]                            ),
      .esr_mprot                   ( esr_mprot                                          ),
      .esr_vmspagesize             ( esr_vmspagesize                                    ),
      .esr_bypass_icache           ( esr_bypass_icache                                  ),
      .esr_shire_coop_mode         ( esr_shire_coop_mode                                ),
      // Request port
      .f0_req_ready                ( f0_req_ready[i]                                    ),
      .f0_req_valid                ( f0_req_valid[i]                                    ),
      .f0_req                      ( f0_req[i]                                          ),
      .f0_req_min_id               ( f0_req_min_id[i]                                   ),
      // Response
      .f4_resp_valid               ( f4_resp_valid[i]                                   ),
      .f4_resp_miss                ( f4_resp_miss[i]                                    ),
      .f4_resp                     ( f4_resp[i]                                         ),
      .f5_resp_fill_done           ( f5_resp_fill_done[i]                               ),
      // Flush control
      .f0_flush_data               ( f0_flush_data                                      ),
      // Request to L1 tag array
      .f0_l1_miss_req_ready        ( ucache_l1_miss_req_ready[i]                        ),
      .f0_l1_miss_req_valid        ( ucache_l1_miss_req_valid[i]                        ),
      .f0_l1_miss_req_addr         ( ucache_l1_miss_req_addr[i]                         ),
      // Response from L1
      .f0_l1_miss_resp_early_valid ( ucache_l1_miss_resp_early_valid[i]                 ),
      .f0_l1_miss_resp_valid       ( ucache_l1_miss_resp_valid[i]                       ),
      .f0_l1_miss_resp_data        ( ucache_l1_miss_resp_data[i]                        ),
      .f0_l1_miss_resp_ecc_err     ( ucache_l1_miss_resp_ecc_err[i]                     ),
      .f0_l1_miss_resp_l2_err      ( ucache_l1_miss_resp_l2_err[i]                      ),
      // TLB/PTW control
      .satp_info                   ( satp_info[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ]      ),
      .matp_info                   ( matp_info[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ]      ),
      .tlb_invalidate              ( tlb_invalidate[i*NR_MIN_PER_REQ +: NR_MIN_PER_REQ] ),
      // PTW request
      .ptw_req_data                ( ptw_req_data[i]                                    ),
      .ptw_req_valid               ( ptw_req_valid[i]                                   ),
      .ptw_req_ready               ( ptw_req_ready[i]                                   ),
      .ptw_invalidate              ( ptw_invalidate[i]                                  ),
      // PTW response
      .ptw_resp_valid              ( ptw_resp_valid[i]                                  ),
      .ptw_resp_data               ( ptw_resp_data[i]                                   ),
      // APB access
      .apb_paddr                   ( ucache_apb_paddr[i]                                ),
      .apb_pwrite                  ( ucache_apb_pwrite[i]                               ),
      .apb_psel                    ( ucache_apb_psel[i]                                 ),
      .apb_penable                 ( ucache_apb_penable[i]                              ),
      .apb_pwdata                  ( ucache_apb_pwdata[i]                               ),
      .apb_pready                  ( ucache_apb_pready[i]                               ),
      .apb_prdata                  ( ucache_apb_prdata[i]                               ),
      .apb_pslverr                 ( ucache_apb_pslverr[i]                              ),
      // Output debug signals going to Status Monitor
      .dbg_sm_signals              ( ucache_dbg_sm_signals[i]                           )
    );

    // Store requests to L1
    assign ucache_l1_miss_req_ready[i] = ucache_l1_miss_req_ff_available[i];

    // CLK    RST    EN                                    DOUT                                DIN                                      DEF
    `RST_FF(clock, reset,                                       ucache_l1_miss_req_ff_available[i], ucache_l1_miss_req_ff_available_next[i], 1'b1)
    `EN_FF (clock,        ucache_l1_miss_req_ff_in_accepted[i], ucache_l1_miss_req_addr_ff[i],      ucache_l1_miss_req_addr[i])

    assign ucache_l1_miss_req_ff_in_accepted[i] = ucache_l1_miss_req_valid[i] & ucache_l1_miss_req_ff_available[i];

    always_comb begin
      // If FF still available
      if (ucache_l1_miss_req_ff_available[i])
        // Not available anymore when input is valid
        ucache_l1_miss_req_ff_available_next[i] = ~ucache_l1_miss_req_valid[i];
      // Not available, wait for arbiter grant
      else
        // Grant received, FF available again
        ucache_l1_miss_req_ff_available_next[i] = ucache_l1_miss_req_ff_out_accepted[i];
    end
  end


  // //////////////////////////////////////////////////////////////////////////////
  // Round-robin arbiter between the ucache L1 requests
  // //////////////////////////////////////////////////////////////////////////////

  generate if (NR_REQUESTORS > 1) begin
    logic [NR_REQUESTORS-1:0] ucache_l1_miss_req_bid;
    logic [NR_REQUESTORS-1:0] ucache_l1_miss_req_grant;

    assign ucache_l1_miss_req_bid             = ~ucache_l1_miss_req_ff_available;
    assign ucache_l1_miss_req_ff_out_accepted = f0_l1_miss_req_ready ? ucache_l1_miss_req_grant : '0;

    arb_rr_data #(
      .NUM_REQS ( NR_REQUESTORS              ),
      .WIDTH    ( `PA_SIZE                   )
    ) ucache_l1_miss_req_arb (
      // System signals
      .clock    ( clock                      ),
      .reset    ( reset                      ),
      // Bidding
      .reqs     ( ucache_l1_miss_req_bid     ),
      .data_in  ( ucache_l1_miss_req_addr_ff ),
      .pop      ( f0_l1_miss_req_ready       ),
      // Grant
      .grants   ( ucache_l1_miss_req_grant   ),
      .data_out ( f0_l1_miss_req_addr_next   )
    );

    always_comb begin
      f0_l1_miss_req_idx_next = '0;

      for (integer i = 0; i < NR_REQUESTORS; i++) begin
        if (ucache_l1_miss_req_grant[i])
        f0_l1_miss_req_idx_next |= i[NR_REQUESTORS_L-1:0];
      end
    end

    assign f0_l1_miss_req_valid_next = |ucache_l1_miss_req_bid & f0_l1_miss_req_ready;
  end else begin
    assign ucache_l1_miss_req_ff_out_accepted = f0_l1_miss_req_ready;

    assign f0_l1_miss_req_addr_next  = ucache_l1_miss_req_addr_ff;
    assign f0_l1_miss_req_idx_next   = 1'b0;
    assign f0_l1_miss_req_valid_next = ~ucache_l1_miss_req_ff_available & f0_l1_miss_req_ready;
  end
  endgenerate

  // //////////////////////////////////////////////////////////////////////////////
  // Sequence requests through the pipeline
  // //////////////////////////////////////////////////////////////////////////////

  // CLK    RST    EN                                                DOUT                  DIN                        DEF
  `RST_FF(clock, reset,                                                   f0_l1_miss_req_ready, f0_l1_miss_req_ready_next, 1'b1)
  `RST_FF(clock, reset,                                                   f0_l1_miss_req_valid, f0_l1_miss_req_valid_next, 1'b0)
  `EN_FF (clock,        f0_l1_miss_req_ready & f0_l1_miss_req_valid_next, f0_l1_miss_req_addr,  f0_l1_miss_req_addr_next)
  `EN_FF (clock,        f0_l1_miss_req_ready & f0_l1_miss_req_valid_next, f0_l1_miss_req_idx,   f0_l1_miss_req_idx_next)

  always_comb begin
    if (f0_l1_miss_req_ready)
      // Stop accepting new requests when a requests enters the pipeline
      f0_l1_miss_req_ready_next = ~f0_l1_miss_req_valid_next;
    else
      // Once response from L1 is received, accept next request
      f0_l1_miss_req_ready_next = f0_l1_miss_resp_valid;
  end

  // //////////////////////////////////////////////////////////////////////////////
  // FSM to handle a miss
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    f0_miss_state_next = f0_miss_state;

    // Ready state
    if(f0_miss_state == icache_miss_state_Ready) begin
      // In case of miss move to request
      if (f1_miss)
      f0_miss_state_next = icache_miss_state_Request;
    end
    // Sending a request
    else if(f0_miss_state == icache_miss_state_Request) begin
      // Request gets accepted
      if (f0_l2_miss_req_ready)
      f0_miss_state_next = icache_miss_state_Fill_Wait;
    end
    // Waiting for the fill to come back
    else begin // if (f0_miss_state == icache_miss_state_Fill_Wait)
      if (f0_l1_miss_resp_valid)
      f0_miss_state_next = icache_miss_state_Ready;
    end
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Miss request to L2
  // //////////////////////////////////////////////////////////////////////////////
  logic f0_l2_miss_req_valid_next;
  logic f0_l2_miss_req_acc;

  // CLK    RST    DOUT                  DIN                        DEF
  `RST_FF (clock, reset, f0_l2_miss_req_valid, f0_l2_miss_req_valid_next, 1'b0)

  always_comb begin
    // Sends the miss request to L2
    f0_l2_miss_req_valid_next = (f0_miss_state_next == icache_miss_state_Request)
    && !f0_l2_miss_req_disable_next;
    f0_l2_miss_req.id         = '0; // Unused
    f0_l2_miss_req.source     = '0; // Unused (assigned at neighborhood channel)
    f0_l2_miss_req.wdata      = 1'b0; // Read access
    f0_l2_miss_req.opcode     = ET_LINK_REQ_Read; // Read data
    f0_l2_miss_req.subopcode  = `ET_LINK_SUBOPCODE_SIZE'(ET_LINK_Read_L2);
    f0_l2_miss_req.address    = { f0_paddr[`PA_MSB:`ICACHE_BLOCK_ADDR_SIZE], (`ICACHE_BLOCK_ADDR_SIZE)'(0) };
    f0_l2_miss_req.size       = ET_LINK_Line; // Read full line
    f0_l2_miss_req.qwen       = 4'b0000;
  end

  assign f0_l2_miss_req_acc = f0_l2_miss_req_valid && f0_l2_miss_req_ready;

  // //////////////////////////////////////////////////////////////////////////////
  // Fill from L2 and response from L1
  // //////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN                     DOUT                        DIN                    DEF
  `EN_FF (clock,        f0_l2_miss_resp_valid, f0_l2_miss_resp_opcode_reg, f0_l2_miss_resp.opcode)
  `RST_FF(clock, reset,                        f0_l2_miss_resp_valid_reg,  f0_l2_miss_resp_valid, 1'b0)

  assign f0_l2_miss_resp_err = f0_l2_miss_resp_opcode_reg == ET_LINK_RSP_Err;

  // Always accepting fills from L2
  assign f0_l2_miss_resp_ready = 1'b1;

  // Fill to micro cache is done when response from L1 SRAM blocks is received
  // (both on an L1 hit and on an L1 miss, in which both L1 SRAM blocks and
  // L0 micro cache are written)
  // On a L1 miss, also wait for fill response from L2 to synchronize
  always_comb begin
    f0_l2_miss_resp_done_next = f0_l2_miss_resp_done;
    f0_l1_resp_done_next = f0_l1_resp_done;

    if (f0_l2_miss_resp_valid_reg)
      f0_l2_miss_resp_done_next = 1'b1;
    else if (f0_l1_miss_resp_valid)
      f0_l2_miss_resp_done_next = 1'b0;

    if (f0_data_array_resp_valid)
      f0_l1_resp_done_next = 1'b1;
    else if (f0_l1_miss_resp_valid)
      f0_l1_resp_done_next = 1'b0;
  end

  // Wait for response from L1 SRAM blocks and, on L1 miss, wait for fill response from L2
  assign f0_l1_miss_resp_early_valid = f0_l1_resp_done_next && ((f0_miss_state_next != icache_miss_state_Fill_Wait) || f0_l2_miss_resp_done_next);
  assign f0_l1_miss_resp_valid       = f0_l1_resp_done      && ((f0_miss_state      != icache_miss_state_Fill_Wait) || f0_l2_miss_resp_done);

  // Check SBE and DBE in data array response
  assign f0_da_sbe = |f0_data_array_resp_sbe_per_block & f0_data_array_resp_valid & (f0_miss_state != icache_miss_state_Fill_Wait);
  assign f0_da_dbe = |f0_data_array_resp_dbe_per_block & f0_data_array_resp_valid & (f0_miss_state != icache_miss_state_Fill_Wait);

  // ECC errors are notified down to the core if the error is enabled
  // and Level 2 error responses are also enabled
  assign f0_l1_miss_resp_ecc_err_next =
      (f0_da_sbe & esr_err_log_ctl.err_interrupt_enable[icache_err_code_SBE] |
      f0_da_dbe & esr_err_log_ctl.err_interrupt_enable[icache_err_code_DBE]);
      // & esr_err_log_ctl.err_rsp_enable; // IGA: err_rsp_enable flag removed,
      // errors are always notified to the core

  // L2 errors are notified down to the core only when it corresponds to the
  // current response
  assign f0_l1_miss_resp_l2_err = f0_l2_err & f0_l2_miss_resp_done;

  // Log error information into ESRs
  assign esr_err_log_sbe_next                = f0_da_sbe;
  assign esr_err_log_dbe_next                = f0_da_dbe;
  assign esr_err_log_info_next.way           = f0_read_way;
  assign esr_err_log_info_next.set           = f0_paddr[`ICACHE_SET_ADDR_RANGE];
  assign esr_err_log_info_next.dbe_per_block = f0_data_array_resp_dbe_per_block;
  assign esr_err_log_info_next.sbe_per_block = f0_data_array_resp_sbe_per_block;
  assign esr_err_log_info_next.address       = f0_paddr[`PA_RANGE_CA];

  // Connect response to every micro cache
  always_comb begin
    for (integer i = 0; i < NR_REQUESTORS; i++) begin
      ucache_l1_miss_resp_early_valid[i] = f0_l1_miss_resp_early_valid & (f0_l1_miss_req_idx == i[NR_REQUESTORS_L-1:0]);
      ucache_l1_miss_resp_valid[i]       = f0_l1_miss_resp_valid & (f0_l1_miss_req_idx == i[NR_REQUESTORS_L-1:0]);
      ucache_l1_miss_resp_data[i]        = f0_l1_miss_resp_data;
      ucache_l1_miss_resp_ecc_err[i]     = f0_l1_miss_resp_ecc_err;
      ucache_l1_miss_resp_l2_err[i]      = f0_l1_miss_resp_l2_err;
    end
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Get some info of the micro cache request
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_SET_RANGE] f0_set; 
  logic [`ICACHE_SET_RANGE] f0_set_next; //Flop is moved inside the lru_state_array to match latency
  logic                     f0_set_valid;

  assign f0_set = f0_l1_miss_req_addr[`ICACHE_SET_ADDR_RANGE];
  assign f0_set_next  = f0_l1_miss_req_addr_next[`ICACHE_SET_ADDR_RANGE];
  assign f0_set_valid = f0_l1_miss_req_ready & f0_l1_miss_req_valid_next;
  // //////////////////////////////////////////////////////////////////////////////
  // APB access
  // //////////////////////////////////////////////////////////////////////////////

  assign tag_apb_paddr      = apb_paddr[`ICACHE_DBG_TAG_ADDR_WIDTH-1:0];
  assign tag_apb_pwrite     = apb_pwrite;
  assign tag_apb_psel       = apb_psel    && (apb_paddr[`ICACHE_DBG_ADDR_REGION_RANGE] == `ICACHE_DBG_TAG_ADDR_REGION);
  assign tag_apb_penable    = apb_penable && (apb_paddr[`ICACHE_DBG_ADDR_REGION_RANGE] == `ICACHE_DBG_TAG_ADDR_REGION);
  assign tag_apb_pwdata     = apb_pwdata;
  always_comb begin
    for (integer i = 0; i < NR_REQUESTORS; i++) begin
      ucache_apb_paddr[i]   = apb_paddr[`ICACHE_DBG_UCACHE_ADDR_WIDTH-1:0];
      ucache_apb_pwrite[i]  = apb_pwrite;
      ucache_apb_psel[i]    = apb_psel    && (apb_paddr[`ICACHE_DBG_ADDR_REGION_RANGE] == `ICACHE_DBG_UCACHE_ADDR_REGION)
                                          && (apb_paddr[`ICACHE_DBG_UCACHE_ADDR_RANGE] == i[`ICACHE_DBG_UCACHE_ADDR_SIZE-1:0]);
      ucache_apb_penable[i] = apb_penable && (apb_paddr[`ICACHE_DBG_ADDR_REGION_RANGE] == `ICACHE_DBG_UCACHE_ADDR_REGION)
                                          && (apb_paddr[`ICACHE_DBG_UCACHE_ADDR_RANGE] == i[`ICACHE_DBG_UCACHE_ADDR_SIZE-1:0]);
      ucache_apb_pwdata[i]  = apb_pwdata;
    end
  end

  // Combine ready, read data and error signals
  always_comb begin
    apb_pready  = tag_apb_pready;
    apb_prdata  = tag_apb_prdata & {`bpam_shire_apb_data_width{tag_apb_pready}};
    apb_pslverr = tag_apb_pslverr;

    for (integer i = 0; i < NR_REQUESTORS; i++) begin
      apb_pready  |= ucache_apb_pready[i];
      apb_prdata  |= ucache_apb_prdata[i] & {`bpam_shire_apb_data_width{ucache_apb_pready[i]}};
      apb_pslverr |= ucache_apb_pslverr[i];
    end
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Tags of the cachelines in the L1 SRAMs. The look up starts at F0 and
  // the hits for all the ways of the index are returned in F1
  // //////////////////////////////////////////////////////////////////////////////
  logic                    f1_tag_array_miss;
  logic [`ICACHE_WAYS-1:0] f1_tag_hit; // Hit per way
  icache_tag_write_req     f0_tag_write_req; // Write request to the tags
  logic                    f0_tag_write_valid; // Write valid to the tags

  // Indicate miss tag array so that it invalidates victim
  // (except if icache is bypassed)
  assign f1_tag_array_miss = f1_miss & ~esr_bypass_icache;

  icache_tag_array tag_array (
    // System signals
    .clock                ( clock                    ),
    .reset                ( reset                    ),
    // Read request
    .f0_read_valid        ( f0_l1_miss_req_valid     ),
    .f0_read_paddr        ( f0_l1_miss_req_addr      ), 
    .f0_read_paddr_next   ( f0_l1_miss_req_addr_next ), //moving flop inside the tag array
    .f0_read_paddr_en     ( f0_set_valid             ),
    .f1_read_tag_hit      ( f1_tag_hit               ),
    // Write request
    .f0_write_valid       ( f0_tag_write_valid       ),
    .f0_write_req         ( f0_tag_write_req         ),
    // Miss indication
    .f1_miss              ( f1_tag_array_miss        ),
    .f1_miss_idx          ( f1_set                   ),
    .f1_miss_way          ( f1_write_way             ),
    .f0_miss_state        ( f0_miss_state            ),
    // Flush control
    .f0_flush_data        ( f0_flush_data            ),
    // APB access
    .apb_paddr            ( tag_apb_paddr            ),
    .apb_pwrite           ( tag_apb_pwrite           ),
    .apb_psel             ( tag_apb_psel             ),
    .apb_penable          ( tag_apb_penable          ),
    .apb_pwdata           ( tag_apb_pwdata           ),
    .apb_pready           ( tag_apb_pready           ),
    .apb_prdata           ( tag_apb_prdata           ),
    .apb_pslverr          ( tag_apb_pslverr          )
  );

  // Write request
  // L1 tags are updated on L2 fill responses (new entry is stored)
  // or on data array DBE (entry is invalidated)
  assign f0_tag_write_valid       = (f0_l2_miss_resp_valid_reg & ~f0_l2_miss_resp_err & ~esr_bypass_icache) | f0_da_dbe;
  assign f0_tag_write_req.valid   = ~f0_da_dbe;
  assign f0_tag_write_req.idx     = f0_paddr[`ICACHE_SET_ADDR_RANGE];
  assign f0_tag_write_req.way     = f0_da_dbe ? f0_read_way : f0_write_way;
  assign f0_tag_write_req.data    = f0_paddr[`ICACHE_PA_TAG_RANGE];

  // //////////////////////////////////////////////////////////////////////////////
  // LRU selector
  // Stores the LRU for the ways within each set.
  // Selects a victim (a way to replace) for the current set.
  // It is accessed in F0 and updated in F1.
  // The update is done once per valid request if there is either a miss or a hit
  // NOTE: If RTLMIN-5127 is implemented, then a similar solution to that
  // implemented in RTLMIN-5378 for the microcache is needed here. I.e.
  // the update must be also done when the miss response is effectively
  // cached, so that other hit accesses that happen meanwhile do not make
  // the newly cached line to be already old. That would need a new
  // update port on icache_lru_array that is asynchronous to the current
  // one.
  // A different approach would be to store the victim to be replaced
  // while the miss is outstanding and OR it with f1_accessed_way for any
  // update that happens in the meanwhile.
  // Also, we may stall any access going to the same set while the miss is
  // outstanding
  // //////////////////////////////////////////////////////////////////////////////

  icache_lru_array lru_array (
    // System signals
    .clock                ( clock                  ),
    .reset                ( reset                  ),
    // Victim read port
    .f0_valid             ( f0_l1_miss_req_valid   ),
    .f0_access_set        ( f0_set                 ),
    .f0_access_set_next   ( f0_set_next            ),
    .f0_set_valid         ( f0_set_valid           ),
    .f1_victim            ( f1_replace_way         ),
    // Update port
    .f1_update_en         ( f1_lru_update_en       ),
    .f1_update_set        ( f1_set                 ),
    .f1_update_access_way ( f1_accessed_way        )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic f1_valid; // Request valid in F1

  // CLK    RST    EN                    DOUT      DIN                   DEF
  `EN_FF (clock,        f0_l1_miss_req_valid, f1_paddr, f0_l1_miss_req_addr)
  `RST_FF(clock, reset,                       f1_valid, f0_l1_miss_req_valid, 1'b0)

  assign f1_set = f1_paddr[`ICACHE_SET_ADDR_RANGE];

  // //////////////////////////////////////////////////////////////////////////////
  // Tag match
  // //////////////////////////////////////////////////////////////////////////////
  logic f1_any_tag_hit; // There was any hit in the tags in F1

  assign f1_any_tag_hit = |f1_tag_hit & ~esr_bypass_icache;

  assign f1_hit  = f1_any_tag_hit && f1_valid;
  assign f1_miss = !f1_any_tag_hit && f1_valid
      && (f0_miss_state == icache_miss_state_Ready); // Miss is accepted when miss FSM is ready

  // //////////////////////////////////////////////////////////////////////////////
  // LRU update
  // //////////////////////////////////////////////////////////////////////////////

  // Sends the LRU update to the LRU array
  assign f1_accessed_way = f1_any_tag_hit ? f1_tag_hit : f1_replace_way;

  // The update is done once per valid request if there is either a miss or a hit
  assign f1_lru_update_en = (f1_hit | f1_miss) & ~esr_bypass_icache;

  // //////////////////////////////////////////////////////////////////////////////
  // Module that accesses the data of the cachelines cached in L1 SRAMs.
  // //////////////////////////////////////////////////////////////////////////////
  icache_data_array data_array (
    // System signals
    .clock                      ( clock                            ),
    .reset                      ( reset                            ),
    // Read port request
    .f1_read_req_valid          ( f1_hit                           ),
    .f1_read_req_way            ( f1_read_way                      ),
    .f1_read_req_set            ( f1_set                           ),
    // Read port response
    .f0_read_resp_valid         ( f0_data_array_resp_valid         ),
    .f0_read_resp_data          ( f0_data_array_resp_data          ),
    .f0_read_resp_sbe_per_block ( f0_data_array_resp_sbe_per_block ),
    .f0_read_resp_dbe_per_block ( f0_data_array_resp_dbe_per_block ),
    // Write port request
    // Notify that a L2 miss request was issued
    // so a write from L2 will happen
    .f1_write_req_valid         ( f0_l2_miss_req_acc               ),
    .f1_write_req_way           ( f0_write_way                     ),
    .f1_write_req_set           ( f0_paddr[`ICACHE_SET_ADDR_RANGE] ),
    // Read request to L1 SRAM blocks
    .f2_mem_req_write           ( f2_sram_req_write                ),
    .f2_mem_req_addr            ( f2_sram_req_addr                 ),
    .f2_mem_req_valid           ( f2_sram_req_valid                ),
    .f2_mem_req_ready           ( f2_sram_req_ready                ),
    // Read response from L1 SRAM blocks
    .f0_mem_resp_dout           ( f0_sram_resp_dout                ),
    .f0_mem_resp_valid          ( f0_sram_resp_valid               ),
    .f0_mem_resp_ready          ( f0_sram_resp_ready               )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Encoded ways
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    f1_read_way = '0;
    f1_write_way = '0;
    for(integer i = 0; i < `ICACHE_WAYS; i++) begin
      if (f1_tag_hit[i])
        f1_read_way |= i[`ICACHE_WAY_RANGE];

      if (f1_replace_way[i])
        f1_write_way |= i[`ICACHE_WAY_RANGE];
    end
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // Take signals from micro cache
    dbg_sm_signals = '0;

    for (integer i = 0; i < NR_REQUESTORS; i++)
      dbg_sm_signals |= ucache_dbg_sm_signals[i];

    // Connect icache signals
    dbg_sm_signals.f1_tag_hit           = f1_tag_hit;
    dbg_sm_signals.f0_miss_state        = f0_miss_state;
    dbg_sm_signals.f0_l1_miss_req_valid = f0_l1_miss_req_valid;
    dbg_sm_signals.f0_l1_miss_req_addr  = f0_l1_miss_req_addr[`PA_RANGE_CA];
  end

endmodule

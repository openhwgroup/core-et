// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_mems
(
    // System signals
    input  logic                                               clock,
    input  logic [`NUM_NEIGH-1:0]                              reset,
    input  logic                                               dft__sram_clock,
    input  logic                                               dft__clk_override,

    // Neighborhood - Icache L1 SRAM interface
    input  logic [`NUM_NEIGH-1:0]                              icache_req_write,
    input  logic [`NUM_NEIGH-1:0][`ICACHE_SRAM_ADD_WIDTH-1:0]  icache_req_addr,
    input  logic [`NUM_NEIGH-1:0]                              icache_req_valid,
    output logic [`NUM_NEIGH-1:0]                              icache_req_ready,
    output logic [`NUM_NEIGH-1:0][`ICACHE_SRAM_DATA_WIDTH-1:0] icache_resp_dout,
    output logic [`NUM_NEIGH-1:0]                              icache_resp_valid,
    input  logic [`NUM_NEIGH-1:0]                              icache_resp_ready,
    // L2 - Neighborhood response
    input  et_link_rsp_info_t [`NUM_NEIGH-1:0]                 neigh_sc_rsp_info,
    input  logic [`NUM_NEIGH-1:0]                              neigh_sc_rsp_valid,
    input  logic [`NUM_NEIGH-1:0]                              neigh_sc_rsp_ready,
    // ESRs
    input  esr_shire_cache_ram_cfg_t                           esr_shire_cache_ram_cfg,
    // BIST                                                              
    input  sc_icache_bist_req_t                                bist_req_info,
    output sc_icache_bist_rsp_t                                bist_rsp_info,
    // APB signals from BPAM for debug 
    input  logic [`ICACHE_MEMS_APB_ADDR_WIDTH-1:0]             apb_paddr,
    input  logic                                               apb_pwrite,
    input  logic                                               apb_psel,
    input  logic                                               apb_penable,
    input  logic [`bpam_shire_apb_data_width-1:0]              apb_pwdata,
    output logic                                               apb_pready,
    output logic [`bpam_shire_apb_data_width-1:0]              apb_prdata,
    output logic                                               apb_pslverr
);

// INTERNAL DECLARATIONS
sc_icache_bist_rsp_t int_bist_rsp_info [`NUM_NEIGH-1:0];
logic [`NUM_NEIGH-1:0][`ICACHE_DATA_APB_ADDR_WIDTH-1:0] int_apb_paddr;
logic [`NUM_NEIGH-1:0]                                  int_apb_pwrite;
logic [`NUM_NEIGH-1:0]                                  int_apb_psel;
logic [`NUM_NEIGH-1:0]                                  int_apb_penable;
logic [`NUM_NEIGH-1:0][`bpam_shire_apb_data_width-1:0]  int_apb_pwdata;
logic [`NUM_NEIGH-1:0]                                  int_apb_pready;
logic [`NUM_NEIGH-1:0][`bpam_shire_apb_data_width-1:0]  int_apb_prdata;
logic [`NUM_NEIGH-1:0]                                  int_apb_pslverr;

genvar n;
generate for(n = 0; n < `NUM_NEIGH; n++)
begin : ICACHE_SRAM
    icache_data_ram_wrap
    #(
        .ID                 ( n                     )
    )
    data_ram_wrap
    (
        // System signals
        .clock                   ( clock                   ),
        .reset                   ( reset[n]                ),
        .dft__sram_clock         ( dft__sram_clock         ),
        .dft__clk_override       ( dft__clk_override       ),
        // Neighborhood - Icache L1 SRAM interface         
        .icache_req_write        ( icache_req_write[n]     ),
        .icache_req_addr         ( icache_req_addr[n]      ),
        .icache_req_valid        ( icache_req_valid[n]     ),
        .icache_req_ready        ( icache_req_ready[n]     ),
        .icache_resp_dout        ( icache_resp_dout[n]     ),
        .icache_resp_valid       ( icache_resp_valid[n]    ),
        .icache_resp_ready       ( icache_resp_ready[n]    ),
        // L2 - Neighborhood response                      
        .neigh_sc_rsp_info       ( neigh_sc_rsp_info[n]    ),
        .neigh_sc_rsp_valid      ( neigh_sc_rsp_valid[n]   ),
        .neigh_sc_rsp_ready      ( neigh_sc_rsp_ready[n]   ),
        // ESRs
        .esr_shire_cache_ram_cfg ( esr_shire_cache_ram_cfg ),
        // BIST
        .bist_req_info           ( bist_req_info           ),
        .bist_rsp_info           ( int_bist_rsp_info[n]    ),
        // APB signals from BPAM for debug                 
        .apb_paddr               ( int_apb_paddr[n]        ),
        .apb_pwrite              ( int_apb_pwrite[n]       ),
        .apb_psel                ( int_apb_psel[n]         ),
        .apb_penable             ( int_apb_penable[n]      ),
        .apb_pwdata              ( int_apb_pwdata[n]       ),
        .apb_pready              ( int_apb_pready[n]       ),
        .apb_prdata              ( int_apb_prdata[n]       ),
        .apb_pslverr             ( int_apb_pslverr[n]      )
    );

    // APB access
    assign int_apb_paddr[n]   = apb_paddr[`ICACHE_DATA_APB_ADDR_WIDTH-1:0];
    assign int_apb_pwrite[n]  = apb_pwrite;
    assign int_apb_psel[n]    = apb_psel & (apb_paddr[(`ICACHE_MEMS_APB_ADDR_WIDTH-1) -: `ICACHE_MEMS_APB_NEIGH_WIDTH] == n[`NUM_NEIGH_R]);
    assign int_apb_penable[n] = apb_penable & (apb_paddr[(`ICACHE_MEMS_APB_ADDR_WIDTH-1) -: `ICACHE_MEMS_APB_NEIGH_WIDTH] == n[`NUM_NEIGH_R]);
    assign int_apb_pwdata[n]  = apb_pwdata;

    wire apb_apddr_msb_unused_ok;
    assign apb_apddr_msb_unused_ok = &{
        `ifndef ET_ASCENT_LINT
            1'b0,
        `endif
            apb_paddr[`ICACHE_MEMS_APB_ADDR_WIDTH-1:`ICACHE_DATA_APB_ADDR_WIDTH] };
end
endgenerate

// OR all bst_rsp_info's together from icache_data_ram_wrap modules
always_comb begin
   bist_rsp_info = '0;
   for (int unsigned ii=0; ii<`NUM_NEIGH; ii++) begin
      bist_rsp_info |= int_bist_rsp_info[ii];
   end
end

////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////

// Combine ready, read data and error signals
assign apb_pready = |int_apb_pready;

always_comb
begin
    apb_prdata = '0;
    for (integer neigh_it = 0; neigh_it < `NUM_NEIGH; neigh_it++)
        apb_prdata |= int_apb_prdata[neigh_it] & {`bpam_shire_apb_data_width{int_apb_pready[neigh_it]}};
end

assign apb_pslverr = |int_apb_pslverr;


endmodule

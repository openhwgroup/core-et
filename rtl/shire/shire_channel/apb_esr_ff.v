// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


/*
 * This module is just a wrapper arround the generic apb_ff
 */
module apb_esr_ff
 #( parameter NUM_INTERFACES = `SBM_APB_INTERFACES)
(
   input  logic clock,
   input  logic reset,
   input  APB_to_all_ESRs_t [NUM_INTERFACES-1:0] APB_ESR_req_in,
   output APB_to_all_ESRs_t [NUM_INTERFACES-1:0] APB_ESR_req_out,
   input  APB_from_all_ESRs_t [NUM_INTERFACES-1:0] APB_ESR_rsp_in,
   output APB_from_all_ESRs_t [NUM_INTERFACES-1:0] APB_ESR_rsp_out   
);

localparam ADDR_WIDTH = $bits(APB_ESR_req_in[0].apb_paddr);
localparam DATA_WIDTH = $bits(APB_ESR_req_in[0].apb_pwdata);

// APB request in
logic   [NUM_INTERFACES-1:0]                    apb_psel_in;
logic   [NUM_INTERFACES-1:0]                    apb_penable_in;
logic   [NUM_INTERFACES-1:0]                    apb_pwrite_in;
logic   [NUM_INTERFACES-1:0][DATA_WIDTH-1:0]    apb_pwdata_in;
logic   [NUM_INTERFACES-1:0][ADDR_WIDTH-1:0]    apb_paddr_in;
// APB request out (after FF)
logic   [NUM_INTERFACES-1:0]                    apb_psel_out;
logic   [NUM_INTERFACES-1:0]                    apb_penable_out;
logic   [NUM_INTERFACES-1:0]                    apb_pwrite_out;
logic   [NUM_INTERFACES-1:0][DATA_WIDTH-1:0]    apb_pwdata_out;
logic   [NUM_INTERFACES-1:0][ADDR_WIDTH-1:0]    apb_paddr_out ;
// APB response in
logic   [NUM_INTERFACES-1:0]                    apb_pready_in;
logic   [NUM_INTERFACES-1:0]                    apb_pslverr_in;
logic   [NUM_INTERFACES-1:0][DATA_WIDTH-1:0]    apb_prdata_in;
// APB response out (after FF)
logic   [NUM_INTERFACES-1:0]                    apb_pready_out;
logic   [NUM_INTERFACES-1:0]                    apb_pslverr_out;
logic   [NUM_INTERFACES-1:0][DATA_WIDTH-1:0]    apb_prdata_out;

generate for(genvar i = 0; i < NUM_INTERFACES; ++i)
begin: DRIV_APB_IN   
  // REQ
  assign apb_psel_in[i] = APB_ESR_req_in[i].apb_psel;
  assign apb_penable_in[i] = APB_ESR_req_in[i].apb_penable;
  assign apb_pwrite_in[i] = APB_ESR_req_in[i].apb_pwrite;
  assign apb_paddr_in[i] = APB_ESR_req_in[i].apb_paddr;
  assign apb_pwdata_in[i] = APB_ESR_req_in[i].apb_pwdata;
  // RSP
  assign APB_ESR_rsp_out[i].apb_pready = apb_pready_out[i];
  assign APB_ESR_rsp_out[i].apb_pslverr = apb_pslverr_out[i];
  assign APB_ESR_rsp_out[i].apb_prdata = apb_prdata_out[i];
  
end  
endgenerate

generate for(genvar i = 0; i < NUM_INTERFACES; ++i)
begin: DRIV_APB_OUT
  // REQ
  assign APB_ESR_req_out[i].apb_psel = apb_psel_out[i];
  assign APB_ESR_req_out[i].apb_penable = apb_penable_out[i];
  assign APB_ESR_req_out[i].apb_pwrite = apb_pwrite_out[i];
  assign APB_ESR_req_out[i].apb_paddr = apb_paddr_out[i];
  assign APB_ESR_req_out[i].apb_pwdata = apb_pwdata_out[i];
  // RSP
  assign apb_pready_in[i] = APB_ESR_rsp_in[i].apb_pready;
  assign apb_pslverr_in[i] = APB_ESR_rsp_in[i].apb_pslverr;
  assign apb_prdata_in[i] = APB_ESR_rsp_in[i].apb_prdata;
end  
endgenerate


apb_ff #( .NUM_INTERFACES(NUM_INTERFACES),
	  .ADDR_WIDTH(ADDR_WIDTH),
	  .DATA_WIDTH(DATA_WIDTH))
ff 
( 
  .*
);


endmodule

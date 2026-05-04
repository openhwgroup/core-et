// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


//------------------------------------------------------------------------------
// Generic FF stage for APB protocol
//------------------------------------------------------------------------------

module apb_ff
#(
   parameter ADDR_WIDTH,
   parameter DATA_WIDTH = 64
)
(
   // System signals
   input  logic                  clock,
   input  logic                  reset,
   // Input slave side
   input  logic                  apb_in_psel,
   input  logic                  apb_in_penable,
   input  logic [ADDR_WIDTH-1:0] apb_in_paddr,
   input  logic                  apb_in_pwrite,
   input  logic [DATA_WIDTH-1:0] apb_in_pwdata,
   output logic                  apb_in_pready,
   output logic [DATA_WIDTH-1:0] apb_in_prdata,
   output logic                  apb_in_pslverr,
   // Output master side
   output logic                  apb_out_psel,
   output logic                  apb_out_penable,
   output logic [ADDR_WIDTH-1:0] apb_out_paddr,
   output logic                  apb_out_pwrite,
   output logic [DATA_WIDTH-1:0] apb_out_pwdata,
   input  logic                  apb_out_pready,
   input  logic [DATA_WIDTH-1:0] apb_out_prdata,
   input  logic                  apb_out_pslverr
);

//------------------------------------------------------------------------------
// INTERNAL DECLARATIONS
//------------------------------------------------------------------------------
logic apb_in_setup;
logic apb_out_psel_next;
logic apb_out_penable_next;
logic apb_out_access;
logic apb_in_pready_next;
   
// Input is in Setup stage
assign apb_in_setup = apb_in_psel & ~apb_in_penable;

always_comb
begin
   apb_out_psel_next    = apb_out_psel;
   apb_out_penable_next = apb_out_penable;

   if (!apb_out_psel)
      apb_out_psel_next = apb_in_setup;
   else if (apb_out_access && apb_out_pready)
      apb_out_psel_next = 1'b0;

   if (!apb_out_penable)
      apb_out_penable_next = apb_in_penable;
   else if (apb_out_pready)
      apb_out_penable_next = 1'b0;
end

//         CLK    RST    EN                                DOUT             DIN                   DEF
`RST_FF   (clock, reset,                                   apb_out_psel,    apb_out_psel_next,    1'b0)
`RST_EN_FF(clock, reset, apb_out_psel,                     apb_out_penable, apb_out_penable_next, 1'b0)
`EN_FF    (clock,        apb_in_setup,                     apb_out_paddr,   apb_in_paddr)
`EN_FF    (clock,        apb_in_setup,                     apb_out_pwrite,  apb_in_pwrite)
`EN_FF    (clock,        apb_in_setup && apb_in_pwrite,    apb_out_pwdata,  apb_in_pwdata)
`RST_EN_FF(clock, reset, apb_out_access || apb_in_pready,  apb_in_pready,   apb_in_pready_next,   1'b0)
`EN_FF    (clock,        apb_out_access && apb_out_pready, apb_in_prdata,   apb_out_prdata)
`RST_EN_FF(clock, reset, apb_out_access && apb_out_pready, apb_in_pslverr,  apb_out_pslverr,      1'b0)

// Output is in Access stage
assign apb_out_access = apb_out_psel & apb_out_penable;

// Control apb_in_pready
// Propagate the ready_out to ready_in in when access (apb_out_acces)
// But when a ready is visible force a 0 so next access doesn't
// see an old ready value
assign apb_in_pready_next = apb_out_access && !apb_in_pready ? apb_out_pready :
                            !apb_out_access && apb_in_pready  ? 1'b0          : 
                            apb_in_pready;
   
   
endmodule

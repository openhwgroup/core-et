// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"

// This module implements a multiplexer for multiple handshake signals for ET-Link interfaces
// Interface to bus is of type B (only one valid active, multiple ready see spec)
module mux_etl_type_b
#(
    parameter WIDTH = 32,
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // Handshake ports on client side
    input  logic [NUM_CLIENTS-1:0]           valid_in,
    output logic [NUM_CLIENTS-1:0]           ready_in,
    // Handshake ports on bus side
    output logic [NUM_CLIENTS-1:0]           valid_bus,
    input  logic [NUM_CLIENTS-1:0]           ready_bus,
    // Data ports
    input logic [NUM_CLIENTS-1:0][WIDTH-1:0] data_in,
    output logic [WIDTH-1:0]                 data_bus
);

// Grants the bus to the interface that has both, valid and ready active
logic  [NUM_CLIENTS_LOG-1:0] select;
logic                        handshake;

always_comb
begin
  select = {NUM_CLIENTS_LOG{1'b0}};
  valid_bus = {NUM_CLIENTS_LOG{1'b0}};
  ready_in = {NUM_CLIENTS_LOG{1'b0}};
  handshake  = 1'b0;
  for (integer i=0; i<NUM_CLIENTS; i++)
  begin
    if (!handshake)
    begin
      valid_bus[i] = valid_in[i] & ready_bus[i];
      handshake = valid_in[i] & ready_bus[i];
      ready_in[i] = ready_bus[i] & valid_in[i];
      select = i[NUM_CLIENTS_LOG-1:0];
    end    
  end
end  

// Data muxed to bus
assign data_bus = data_in[select];

endmodule


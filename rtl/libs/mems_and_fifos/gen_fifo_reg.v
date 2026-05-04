// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"


// This module is a generic fifo with registered rdata, valid and full signals
// DEPTH includes the output register stage

module gen_fifo_reg #(parameter WIDTH=8, DEPTH=4, parameter gating_type_t GATING_TYPE = gating_type_default) (
   input  logic             clock,
   input  logic             reset,
   input  logic             push,
   input  logic [WIDTH-1:0] wdata,
   output logic             full,
   output logic [DEPTH:0]   level,
   input  logic             pop,
   output logic             valid,
   output logic [WIDTH-1:0] rdata
);

   genvar gen_it;
   generate
      if (DEPTH == 1) begin : single_entry_fifo
         // NOTE: this instantiation does not break the pop chain!
         assign full = valid && !pop;
         wire valid_en = !full;
         `RST_EN_FF(clock, reset, valid_en, valid, push, '0)
         wire rdata_en = push && !full;
         `EN_FF    (clock, rdata_en, rdata, wdata)
         assign level = {valid, !valid};
      end
      else begin : two_or_more_entry_fifo
         
         // level (for full/valid flags)
         wire wdata_taken = (push && !full);
         wire rdata_taken = (pop  && valid);
         wire level_en = wdata_taken ^ rdata_taken;
         wire [DEPTH:0] level_next = wdata_taken ? {level[DEPTH-1:0], 1'b0} : {1'b0, level[DEPTH:1]};
         `RST_EN_FF(clock, reset, level_en, level, level_next, `ZX((DEPTH+1), 1'b1))
         wire    mem_valid = !(|level[1:0]); // if not (empty or only one entry that is in output flop)
         assign  full  = level[DEPTH];
         assign  valid = !level[0];
         
         localparam MEM_DEPTH=DEPTH-1;
         localparam PTR_W = (MEM_DEPTH==1) ? 1 : $clog2(MEM_DEPTH);
         wire inc_wptr = push & (mem_valid | (valid && !pop)) && !full; // pushing into mem -> if (mem_valid already or output valid and not getting popped) and not full
         wire inc_rptr = (pop & mem_valid); // loading output stage with mem entry
         logic [PTR_W-1:0] wptr;
         logic [PTR_W-1:0] rptr;
         wire [PTR_W-1:0] wptr_next = (wptr==(MEM_DEPTH-1)) ? {PTR_W {1'b0}} : wptr + 1'b1;
         wire [PTR_W-1:0] rptr_next = (rptr==(MEM_DEPTH-1)) ? {PTR_W {1'b0}} : rptr + 1'b1;

         `RST_EN_FF(clock, reset, inc_wptr, wptr, wptr_next, '0)
         `RST_EN_FF(clock, reset, inc_rptr, rptr, rptr_next, '0)
         
         // mem
         logic [WIDTH-1:0] mem [MEM_DEPTH-1:0];
         for (gen_it = 0; gen_it < MEM_DEPTH; gen_it++) begin : gen_mem
            en_ff_gated_struct #(.WIDTH($bits(wdata)), .GATING_TYPE(GATING_TYPE)) mem_inst (clock, inc_wptr & (wptr==gen_it), mem[gen_it], wdata);
         end
         
         
         // rdata
         wire             rdata_en = ((push && !valid) || (push && pop && !mem_valid)) || (inc_rptr);
         logic [WIDTH-1:0] rdata_next;
         always_comb begin 
            if ((push && !valid) || (push && pop && !mem_valid)) begin
               // pushing first entry or
               // output stage getting popped and mem_empty so push directly into output stage
               rdata_next = wdata;
            end
            else if (inc_rptr) begin
               // output stage getting popped, load output stage with next mem_entry
               rdata_next = mem[rptr];
            end
            else begin
               rdata_next = rdata;
            end
         end
         en_ff_gated_struct #(.WIDTH($bits(rdata)), .GATING_TYPE(GATING_TYPE)) rdata_inst (clock, rdata_en, rdata, rdata_next);

      end // two_or_more_entry_fifo
      
   endgenerate

`ifdef ET_COVERAGE
   covergroup gen_fifo_reg_cg @(posedge clock);
      full: coverpoint full{
         bins valid = {1'b1};
      }
      push_when_full: coverpoint 1'b1 iff (push && full){
         bins valid = {1'b1};
      }
      pop_when_empty: coverpoint 1'b1 iff (pop && ~valid){
         bins valid = {1'b1};
      }
   endgroup // gen_fifo_reg_cg

   gen_fifo_reg_cg gen_fifo_reg_cover = new();
`endif


endmodule



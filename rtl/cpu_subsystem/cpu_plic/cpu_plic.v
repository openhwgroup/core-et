// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// This is a wrapper over
`include "soc.vh"

module cpu_plic
   (
    input                             clock,
    input                             reset,
    // APB interface
    input APB_to_plic_t               plic_apb_req,
    output APB_from_plic_t            plic_apb_rsp,
    
    // Interrupts sources
    input  logic [`PLIC_NEXTINT_SOURCES-1:0] interrupt_req_from_src,
    output logic [`THREADS_PER_N-1:0]       plic_meip,
    output logic [`THREADS_PER_N-1:0]       plic_seip
    );

   localparam WORD_SIZE = 2;
   localparam PLIC_SOURCES_WITH_DEFAULT = `PLIC_NEXTINT_SOURCES+1;

   wire resetn = !reset;

   // ET PLIC is ported from Rocket and talks TileLink
   // Conversion from APB to TL here
   logic tl_req_valid;
   logic tl_req_ready;
   logic [`M_A_OPCODE_WIDTH-1:0] tl_req_opcode;
   logic [`M_A_SIZE_WIDTH-1:0]   tl_req_size;
   logic [`M_A_SOURCE_WIDTH-1:0] tl_req_source;
   logic [`M_PLIC_ADDR_WIDTH-1:0] tl_req_addr;
   logic [`M_A_MASK_WIDTH_32-1:0] tl_req_mask;
   logic [`M_A_DATA_WIDTH_32-1:0] tl_req_data;

   logic                          tl_resp_ready;
   logic [`M_A_DATA_WIDTH_32-1:0] tl_resp_data;

   always_comb begin
      plic_apb_rsp.apb_pready    = tl_req_ready;        // APB host always takes response without any delay
      plic_apb_rsp.apb_pslverr   = 1'b0;                // Assumtion: This block is connected to the same reset as the whole cpu subsystem
      plic_apb_rsp.apb_prdata     = tl_resp_data;
   end

   // APB mux crops 2-lsb bits on the address, since it's asume to be 4-bytes aligned
   // ios_plic assumes complete address so we are adding them back.
   wire [`M_PLIC_ADDR_WIDTH-1:0] plic_apb_req_addr = {plic_apb_req.apb_paddr, 2'b0};
   
   assign tl_req_valid  = plic_apb_req.apb_psel && !plic_apb_req.apb_penable; // TODO:ANT: Review this... copied from ios_plic but looks wrong
   assign tl_req_opcode = plic_apb_req.apb_pwrite ? `M_PutFullData : `M_Get;   // always access full data width because APB does not support partial access
   assign tl_req_size   = WORD_SIZE;                                      // fixed size, 4-byte wide, hence value 2;
   assign tl_req_source = '0;                                             // apb does not care about source id, set it to 0;
   assign tl_req_addr   = plic_apb_req_addr;                              // Our PLIC will decode only offset bits within allocated address region
   assign tl_req_mask   = '1;                                             // Use full mask always
   assign tl_req_data   = plic_apb_req.apb_pwdata;

   assign tl_resp_ready = 1'b1;

   // Interrups mapping here
   logic [PLIC_SOURCES_WITH_DEFAULT-1:0] interrupt_req_from_src_int;
   
   always_comb begin
      interrupt_req_from_src_int[0] = 1'b0;
      interrupt_req_from_src_int[`PLIC_NEXTINT_SOURCES:1] = interrupt_req_from_src;
   end

   // *eip trigger mapping
   logic [`PLIC_NEXTINT_TARGETS-1:0] interrupt_req_to_target;
   always_comb begin
      // Modes are contigous to harts
      // context 0 and 1 are hart 0 mmode and smode
      // context 2 and 3 are hart 1 mmode and smode      
      foreach(interrupt_req_to_target[i]) begin
         if (i%2 == 0)
           plic_meip[i/2] = interrupt_req_to_target[i];
         else
           plic_seip[i/2] = interrupt_req_to_target[i];
      end
   end

   // Instance
   ios_plic
   #(
     .maxPriorities    ( 0 ), // No priorities
     .N_EXTINT         (PLIC_SOURCES_WITH_DEFAULT), // Default one is always unused
     .N_EXTINT_TARGETS (`PLIC_NEXTINT_TARGETS)
   )
   plic
   (
    .clock                    ( clock           ),
    .resetn                   ( resetn          ),
    .a_ready                  ( tl_req_ready    ),
    .a_valid                  ( tl_req_valid    ),
    .a_opcode                 ( tl_req_opcode   ),
    .a_size                   ( tl_req_size     ),                        
    .a_source                 ( tl_req_source   ), 
    .a_address                ( tl_req_addr     ),     
    .a_mask                   ( tl_req_mask     ),    
    .a_data                   ( tl_req_data     ),
                              
    // Response               
    .d_ready                  ( tl_resp_ready   ),        
    // ri lint_check_off OPEN_OUTPUT
    .d_valid                  (),                          // APB host determines response data valid based on pready, i.e., when request is taken
    .d_opcode                 (),                          // APB host does not care about d_opcode because it knows what it expects
    .d_size                   (),                          // APB host does not care about d_size because the size is fixed
    .d_source                 (),                          // APB host does not care about d_souce because it is the only source
    // ri lint_check_on  OPEN_OUTPUT
    .d_data                   ( tl_resp_data    ),

    .interrupt_req_from_src  ( interrupt_req_from_src_int  ),
    .interrupt_req_to_target ( interrupt_req_to_target     )
    );




endmodule

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


`include "soc.vh"

// Non-compiant Generic AXI4 to APB3 module
// - Only 1 beat supported
// - No support for lock
// - Ignores CACHE, PROT, QOS, REGION, USER
// - Expect low performance - Not trying to make it fast, just simple
//    - The bridge process 1 request and response at each time

module axi2apb_bridge
  #(
    // AXI parameters
     parameter int ID_WIDTH = 20,
     parameter int AXLEN_WIDTH = 8,
     parameter int AXSIZE_WIDTH = 3,
     parameter int AXBURST_WIDTH = 2,
     parameter int AXCACHE_WIDTH = 4,
     parameter int AXPROT_WIDTH = 3,
     parameter int AXQOS_WIDTH = 4,
     parameter int ADATA_WIDTH = 512,
     parameter int WSTRB_WIDTH = ADATA_WIDTH / 8,
     parameter int BRESP_WIDTH = 2,
     parameter int RRESP_WIDTH = 2,
    // APB parameters (shared with AXI)
     parameter int ADDR_WIDTH = 30,
    // APB parameters
     parameter int APB_DATA_WIDTH = 64,
    // Others
     localparam    ADATA_BYTES = WSTRB_WIDTH,
     localparam    ADATA_BYTES_ADDR = $clog2(ADATA_BYTES)
    
    )(
      // System
       input logic  clock,
       input logic  reset,
      // AW channel
       input logic                       aw_valid,
       input logic [ID_WIDTH-1:0]        aw_id,
       input logic [ADDR_WIDTH-1:0]      aw_addr,
       input logic [AXLEN_WIDTH-1:0]     aw_len,
       input logic [AXSIZE_WIDTH-1:0]    aw_size,
       input logic [AXBURST_WIDTH-1:0]   aw_burst,
       input logic                       aw_lock,
       input logic [AXCACHE_WIDTH-1:0]   aw_cache,
       input logic [AXPROT_WIDTH-1:0]    aw_prot,
       input logic [AXQOS_WIDTH-1:0]     aw_qos,
       output logic                      aw_ready,

      // W channel
       input logic                       w_valid,
       input logic [ADATA_WIDTH-1:0]     w_data,
       input logic [WSTRB_WIDTH-1:0]     w_strb,
       input logic                       w_last,
       output logic                      w_ready,

      // B channel
       output logic                      b_valid,
       output logic [ID_WIDTH-1:0]       b_id,
       output logic [BRESP_WIDTH-1:0]    b_resp,
       input logic                       b_ready,

      // AR channel
       input logic                       ar_valid,
       input logic [ID_WIDTH-1:0]        ar_id,
       input logic [ADDR_WIDTH-1:0]      ar_addr,
       input logic [AXLEN_WIDTH-1:0]     ar_len,
       input logic [AXSIZE_WIDTH-1:0]    ar_size,
       input logic [AXBURST_WIDTH-1:0]   ar_burst,
       input logic                       ar_lock,
       input logic [AXCACHE_WIDTH-1:0]   ar_cache,
       input logic [AXPROT_WIDTH-1:0]    ar_prot,
       input logic [AXQOS_WIDTH-1:0]     ar_qos,
       output logic                      ar_ready,

      // R channel
       output logic                      r_valid,
       output logic [ID_WIDTH-1:0]       r_id,
       output logic [ADATA_WIDTH-1:0]    r_data,
       output logic [RRESP_WIDTH-1:0]    r_resp,
       output logic                      r_last,
       input logic                       r_ready,

      // APB output
       output logic                      apb_psel,
       output logic                      apb_penable,
       output logic [ADDR_WIDTH-1:0]     apb_paddr,
       output logic                      apb_pwrite,
       output logic [APB_DATA_WIDTH-1:0] apb_pwdata,
       input logic                       apb_pready,
       input logic [APB_DATA_WIDTH-1:0]  apb_prdata,
       input logic                       apb_pslverr
      );

   // Constants and types
   localparam MASTER_CHANNELS = 2;
   localparam AXI_AW_IDX = 0;
   localparam AXI_AR_IDX = 1;
   localparam OK = 2'b00;
   localparam EXOKAY = 2'b01;
   localparam SLVERR = 2'b10;
   localparam DECERR = 2'b11;

   typedef struct packed {
      logic [ID_WIDTH-1:0]      a_id;
      logic [ADDR_WIDTH-1:0]    a_addr;
      logic [AXSIZE_WIDTH-1:0]  a_size;
      logic [ADATA_WIDTH-1:0]   w_data;
      logic [WSTRB_WIDTH-1:0]   w_strb;
      logic                     a_write;
   } axi_req_t;

   typedef struct packed {
      logic [ID_WIDTH-1:0]          id;
      logic [ADATA_BYTES_ADDR-1:0]  addr;
      logic [APB_DATA_WIDTH-1:0]    data;
      logic                         write;
      logic                         error;
   } axi_resp_t;
   
   typedef enum logic [1:0]
   {
    IDLE,
    SETUP,
    ACCESS
   } axi2apb_apb_states_t;
   
   // Global signals
   logic      apb_fsm_busy;
   logic pending_resp, pending_resp_next;

   ////////////////////////////////////////////////////////////////
   
   // Select a channel (AW, AR) to acces the APB FSM
   logic [MASTER_CHANNELS-1:0] arb_grants;
   logic [MASTER_CHANNELS-1:0] arb_reqs;
   logic                       arb_pop;

   arb_rr 
   #(.NUM_REQS(MASTER_CHANNELS) )
   arb
   ( 
      .clock    ( clock      ),
      .reset    ( reset      ),
      .grants   ( arb_grants ),
      .reqs     ( arb_reqs   ),
      .pop      ( arb_pop    )
   );

   wire any_granted = |arb_grants;
   always_comb begin
      arb_reqs = '0;
      arb_reqs[AXI_AW_IDX] = aw_valid;
      arb_reqs[AXI_AR_IDX] = ar_valid;
   end
   assign arb_pop = !apb_fsm_busy && !pending_resp;
   assign aw_ready = !apb_fsm_busy && !pending_resp && arb_grants[AXI_AW_IDX];
   assign w_ready  = !apb_fsm_busy && !pending_resp && arb_grants[AXI_AW_IDX];   
   assign ar_ready = !apb_fsm_busy && !pending_resp && arb_grants[AXI_AR_IDX];   
     
   // Capture APB signals
   axi_req_t req, req_next;
  `RST_EN_FF(clock, reset, any_granted && arb_pop, req, req_next, '0)
   
   always_comb begin
      req_next = req;
      if (arb_reqs[AXI_AW_IDX]) begin
         req_next.a_id = aw_id;
         req_next.a_addr = aw_addr;
         req_next.a_size = aw_size;
         req_next.w_data = w_data;
         req_next.w_strb = w_strb;
         req_next.a_write = 1'b1;
      end else begin
         req_next.a_id = ar_id;
         req_next.a_addr = ar_addr;
         req_next.a_size = ar_size;
         req_next.w_data = w_data;
         req_next.w_strb = w_strb;
         req_next.a_write = 1'b0;
      end
   end
   

   // APB FSM
   axi2apb_apb_states_t state, state_next;

   `RST_FF(clock, reset, state, state_next, IDLE)

   assign apb_fsm_busy = state != IDLE;

   always_comb begin
      state_next = state;
      
      case (state)
        IDLE: begin
           if (|arb_grants && arb_pop) state_next = SETUP;
        end
        SETUP: begin
           state_next = ACCESS;
        end
        ACCESS: begin
           if (apb_pready) begin
              //state_next = requests_pending ? ACCESS : IDLE;
              state_next = IDLE; // ANT: The over case will never happen since we can't buffer responses // TODO: Add fifos
           end
        end
        default: begin
           // do nothing
        end
      endcase
   end // always_comb

   // Drive APB signals
   assign apb_psel = state inside {SETUP, ACCESS};
   assign apb_penable = state == ACCESS;
   wire [ADATA_WIDTH-1:0] req_data = req.w_data >> 8*req.a_addr[ADATA_BYTES_ADDR-1:0];
   assign apb_pwdata = req_data[APB_DATA_WIDTH-1:0];
   assign apb_pwrite = req.a_write;
   assign apb_paddr  = req.a_addr;

   // Save APB response
   wire pending_resp_en = state == ACCESS && apb_pready;
   
   
   axi_resp_t resp_data, resp_data_next;
   `EN_FF(clock,  pending_resp_en, resp_data, resp_data_next)
   `RST_FF(clock, reset,  pending_resp, pending_resp_next, 1'b0)

   always_comb begin
      resp_data_next.id = req.a_id;
      resp_data_next.addr = req.a_addr[ADATA_BYTES_ADDR-1:0];
      resp_data_next.data = apb_prdata;
      resp_data_next.write = req.a_write;
      resp_data_next.error = apb_pslverr;
   end

   always_comb begin
      pending_resp_next = pending_resp;
      if (pending_resp_en) 
        pending_resp_next = 1'b1;
      else if (r_valid && r_ready || b_valid && b_ready)
        pending_resp_next = 1'b0;
   end
   

   // Send response to AXI
   // // Drive ports
   always_comb begin
      b_id = resp_data.id;
      r_id = resp_data.id;
      
      b_resp = resp_data.error ? SLVERR : OK;
      r_resp = resp_data.error ? SLVERR : OK;      

      r_data = '0;
      r_data = resp_data.data << 8*resp_data.addr[ADATA_BYTES_ADDR-1:0];

      r_last = 1'b1;
   end
   
   assign b_valid = pending_resp && resp_data.write;
   assign r_valid = pending_resp && !resp_data.write;   
   
   
endmodule
   

      
`include "soc.vh"
  
module erbium_ip_stub
  import erbium_pkg::*;
(
 // System signals
 input                                  clock,
 input                                  reset,

 // CPU Subsystem AXI master
 // Read address channel
 input logic [`CPU_AXI_ID_WIDTH-1:0]    axi_ARID,
 input logic [`CPU_AXI_ADDR_WIDTH-1:0]  axi_ARADDR,
 input logic [`CPU_AXI_LEN_WIDTH-1:0]   axi_ARLEN,
 input logic [`CPU_AXI_SIZE_WIDTH-1:0]  axi_ARSIZE,
 input logic [`CPU_AXI_BURST_WIDTH-1:0] axi_ARBURST,
 input logic                            axi_ARLOCK,
 input logic [`CPU_AXI_CACHE_WIDTH-1:0] axi_ARCACHE,
 input logic [`CPU_AXI_PROT_WIDTH-1:0]  axi_ARPROT,
 input logic                            axi_ARVALID,
 output logic                           axi_ARREADY,

 // Read data channel
 output logic [`CPU_AXI_ID_WIDTH-1:0]   axi_RID,
 output logic [`CPU_AXI_DATA_WIDTH-1:0] axi_RDATA,
 output logic [`CPU_AXI_RESP_WIDTH-1:0] axi_RRESP,
 output logic                           axi_RLAST,
 output logic                           axi_RVALID,
 input logic                            axi_RREADY,

 // Write address channel
 input logic [`CPU_AXI_ID_WIDTH-1:0]    axi_AWID,
 input logic [`CPU_AXI_ADDR_WIDTH-1:0]  axi_AWADDR,
 input logic [`CPU_AXI_LEN_WIDTH-1:0]   axi_AWLEN,
 input logic [`CPU_AXI_SIZE_WIDTH-1:0]  axi_AWSIZE,
 input logic [`CPU_AXI_BURST_WIDTH-1:0] axi_AWBURST,
 input logic                            axi_AWLOCK,
 input logic [`CPU_AXI_CACHE_WIDTH-1:0] axi_AWCACHE,
 input logic [`CPU_AXI_PROT_WIDTH-1:0]  axi_AWPROT,
 input logic                            axi_AWVALID,
 output logic                           axi_AWREADY,

 // Write data channel
 input logic [`CPU_AXI_DATA_WIDTH-1:0]  axi_WDATA,
 input logic [`CPU_AXI_STRB_WIDTH-1:0]  axi_WSTRB,
 input logic                            axi_WLAST,
 input logic                            axi_WVALID,
 output logic                           axi_WREADY,

 // Write response channel
 output logic [`CPU_AXI_ID_WIDTH-1:0]   axi_BID,
 output logic [`CPU_AXI_RESP_WIDTH-1:0] axi_BRESP,
 output logic                           axi_BVALID,
 input logic                            axi_BREADY,

 // CPU subsystem APB slave
 output logic                           apb_psel,
 output logic                           apb_penable,
 output logic                           apb_pwrite,
 output logic [`CPU_APB_ADDR_WIDTH-1:0] apb_paddr,
 output logic [`CPU_APB_DATA_WIDTH-1:0] apb_pwdata,
 input logic [`CPU_APB_DATA_WIDTH-1:0]  apb_prdata,
 input logic                            apb_pready,
 input logic                            apb_pslverr
 );

   // Global signals
   wire resetn = !reset;

   // Mram AXI
// ------------------------------------------------------------
// Low‑priority AW channel
// ------------------------------------------------------------
   localparam              ID_WIDTH =  `CPU_AXI_ID_WIDTH;
   localparam              ADDR_WIDTH = `CPU_AXI_ADDR_WIDTH;
   localparam              AXLEN_WIDTH = `CPU_AXI_LEN_WIDTH;
   localparam              AXSIZE_WIDTH = `CPU_AXI_SIZE_WIDTH;
   localparam              AXBURST_WIDTH = `CPU_AXI_BURST_WIDTH;
   localparam              AXCACHE_WIDTH = `CPU_AXI_CACHE_WIDTH;
   localparam              AXPROT_WIDTH = `CPU_AXI_PROT_WIDTH;
   localparam              DATA_WIDTH = `CPU_AXI_DATA_WIDTH;
   localparam              WSTRB_WIDTH = `CPU_AXI_STRB_WIDTH;
   localparam              APB_DATA_WIDTH = 64;

   localparam              NSLAVES = 2, NCHANNELS = 2;
   localparam              BCHN = 0, RCHN = 1;
   localparam              MRAM = 0, APB_SLV = 1;

   
logic                      mram_aw_valid;
logic                      mram_aw_ready;

// ------------------------------------------------------------
// Low‑priority W channel
// ------------------------------------------------------------
logic                      mram_w_valid;
logic                      mram_w_ready;

// ------------------------------------------------------------
// Low‑priority B channel
// ------------------------------------------------------------
logic                      mram_b_valid;
logic [ID_WIDTH-1:0]       mram_b_id;
axi_resp_t                 mram_b_resp;
logic                      mram_b_ready;

// ------------------------------------------------------------
// Low‑priority AR channel
// ------------------------------------------------------------
logic                      mram_ar_valid;
logic                      mram_ar_ready;

// ------------------------------------------------------------
// Low‑priority R channel
// ------------------------------------------------------------
logic                      mram_r_valid;
logic [ID_WIDTH-1:0]       mram_r_id;
logic [DATA_WIDTH-1:0]     mram_r_data;
axi_resp_t                 mram_r_resp;
logic                      mram_r_last;
logic                      mram_r_ready;

////////////////////////////////////////////////////////////////
// CPU subsystem
////////////////////////////////////////////////////////////////
logic                       cpu_sub_aw_valid;
logic                       cpu_sub_aw_ready;
logic                       cpu_sub_ar_valid;
logic                       cpu_sub_ar_ready;
   
logic                       cpu_sub_w_valid;
logic                       cpu_sub_w_ready;

logic                       cpu_sub_b_valid;
logic [ID_WIDTH-1:0]        cpu_sub_b_id;
axi_resp_t                  cpu_sub_b_resp;
logic                       cpu_sub_b_ready;

logic                       cpu_sub_r_valid;
logic [ID_WIDTH-1:0]        cpu_sub_r_id;
logic [DATA_WIDTH-1:0]      cpu_sub_r_data;
axi_resp_t                  cpu_sub_r_resp;
logic                       cpu_sub_r_last;
logic                       cpu_sub_r_ready;

   logic w_target_q[$];
   logic will_push_w, will_pop_w;
   logic aw_target_is_mram, w_target_is_mram;
   
   // Routing CPU AXI to MRAM or AXI
   assign aw_target_is_mram = !addr_is_esr(axi_AWADDR);
   always_ff @(negedge clock) // VCS don't allow dyamic calls (q.size())  outside procedures and ignore them from the sensistivity list
     w_target_is_mram = w_target_q.size() > 0 ? w_target_q[0] : aw_target_is_mram;
   assign cpu_sub_aw_valid = axi_AWVALID && !aw_target_is_mram;
   assign cpu_sub_w_valid  = axi_WVALID  && !aw_target_is_mram;
   assign cpu_sub_ar_valid = axi_ARVALID && addr_is_esr(axi_ARADDR);
   assign mram_aw_valid    = axi_AWVALID && aw_target_is_mram;
   assign mram_w_valid     = axi_WVALID  && aw_target_is_mram;
   assign mram_ar_valid    = axi_ARVALID && !addr_is_esr(axi_ARADDR);
   always_comb begin
      axi_AWREADY = 1'b0;
      axi_WREADY = 1'b0;      
      axi_ARREADY = 1'b0;
      if (axi_AWVALID) axi_AWREADY = addr_is_esr(axi_AWADDR) ?  cpu_sub_aw_ready : mram_aw_ready;
      if (axi_WVALID)  axi_WREADY  = !w_target_is_mram       ?  cpu_sub_w_ready  : mram_w_ready;      
      if (axi_ARVALID) axi_ARREADY = addr_is_esr(axi_ARADDR) ?  cpu_sub_ar_ready : mram_ar_ready;
   end

   assign will_push_w = axi_AWVALID && axi_AWREADY;
   assign will_pop_w = axi_WVALID && axi_WREADY && axi_WLAST;
   
   always_ff @(posedge clock or posedge reset) begin
        if (reset) begin
            w_target_q = {};
        end else begin
            case ({will_push_w, will_pop_w})
                2'b10: w_target_q.push_back(aw_target_is_mram);
                2'b01: if (w_target_q.size() > 0) void'(w_target_q.pop_front());
                2'b11: begin
                    if (w_target_q.size() > 0) begin
                        void'(w_target_q.pop_front());
                        w_target_q.push_back(aw_target_is_mram);
                    end
                end
            endcase
        end
    end


   
   // Mux MRAM and Bridge to CPU
   logic [NSLAVES-1:0][NCHANNELS-1:0] grant, bid;
   logic [NCHANNELS-1:0]              stall;
   
   assign axi_BID     = grant[BCHN][MRAM] ? mram_b_id     : cpu_sub_b_id;
   assign axi_BRESP   = grant[BCHN][MRAM] ? mram_b_resp   : cpu_sub_b_resp;
   assign axi_BVALID  = grant[BCHN][MRAM] ? mram_b_valid  : cpu_sub_b_valid;
   assign axi_RID     = grant[RCHN][MRAM] ? mram_r_id     : cpu_sub_r_id;
   assign axi_RDATA   = grant[RCHN][MRAM] ? mram_r_data   : cpu_sub_r_data;
   assign axi_RRESP   = grant[RCHN][MRAM] ? mram_r_resp   : cpu_sub_r_resp;
   assign axi_RLAST   = grant[RCHN][MRAM] ? mram_r_last   : cpu_sub_r_last;
   assign axi_RVALID  = grant[RCHN][MRAM] ? mram_r_valid  : cpu_sub_r_valid;

   assign stall[BCHN] = !axi_BREADY;
   assign stall[RCHN] = !axi_RREADY;

   assign bid[BCHN][MRAM]    = mram_b_valid;
   assign bid[BCHN][APB_SLV] = cpu_sub_b_valid;   
   assign bid[RCHN][MRAM]    = mram_r_valid;
   assign bid[RCHN][APB_SLV] = cpu_sub_r_valid;   

   assign mram_r_ready    = grant[RCHN][MRAM]    && axi_RREADY;
   assign cpu_sub_r_ready = grant[RCHN][APB_SLV] && axi_RREADY;   
   assign mram_b_ready    = grant[BCHN][MRAM]    && axi_BREADY;
   assign cpu_sub_b_ready = grant[BCHN][APB_SLV] && axi_BREADY;   
   
   // Arbiters
   arb_lru_grant #(.NUM_CLIENTS(NSLAVES)) lru_bchn (.clock(clock),.reset(reset),.bid(bid[BCHN]),
                                                    .stall(stall[BCHN]), .grant(grant[BCHN]));
   arb_lru_grant #(.NUM_CLIENTS(NSLAVES)) lru_rchn (.clock(clock),.reset(reset),.bid(bid[RCHN]),
                                                    .stall(stall[RCHN]), .grant(grant[RCHN]));


   // Instances
   erbium_memory
     #(
       .MEMORY_TYPE   (erbium_pkg::MRAM),
       .ID_WIDTH      (ID_WIDTH),
       .AXLEN_WIDTH   (AXLEN_WIDTH),
       .AXSIZE_WIDTH  (AXSIZE_WIDTH),
       .AXBURST_WIDTH (AXBURST_WIDTH),
       .AXCACHE_WIDTH (AXCACHE_WIDTH),
       .AXPROT_WIDTH  (AXPROT_WIDTH),
       .ADDR_WIDTH    (ADDR_WIDTH),
       .MEM_SIZE (1 << 24)
       ) 
   u_mram_slave_stub 
     (
      .clock          (clock),
      .resetn         (resetn),
      
      // AW channel (low priority)
    .aw_valid_lo    (mram_aw_valid),
    .aw_id_lo       (axi_AWID),
    .aw_addr_lo     (axi_AWADDR),
    .aw_len_lo      (axi_AWLEN),
    .aw_size_lo     (axi_AWSIZE),
    .aw_burst_lo    (axi_AWBURST),
    .aw_lock_lo     (axi_AWLOCK),
    .aw_cache_lo    (axi_AWCACHE),
    .aw_prot_lo     (axi_AWPROT),
    .aw_qos_lo      ('0),
    .aw_ready_lo    (mram_aw_ready),

    // W channel (low priority)
    .w_valid_lo     (mram_w_valid),
    .w_data_lo      (axi_WDATA),
    .w_strb_lo      (axi_WSTRB),
    .w_last_lo      (axi_WLAST),
    .w_ready_lo     (mram_w_ready),

    // B channel (low priority)
    .b_valid_lo     (mram_b_valid),
    .b_id_lo        (mram_b_id),
    .b_resp_lo      (mram_b_resp),
    .b_ready_lo     (mram_b_ready),

    // AR channel (low priority)
    .ar_valid_lo    (mram_ar_valid),
    .ar_id_lo       (axi_ARID),
    .ar_addr_lo     (axi_ARADDR),
    .ar_len_lo      (axi_ARLEN),
    .ar_size_lo     (axi_ARSIZE),
    .ar_burst_lo    (axi_ARBURST),
    .ar_lock_lo     (axi_ARLOCK),
    .ar_cache_lo    (axi_ARCACHE),
    .ar_prot_lo     (axi_ARPROT),
    .ar_qos_lo      ('0),
    .ar_ready_lo    (mram_ar_ready),

    // R channel (low priority)
    .r_valid_lo     (mram_r_valid),
    .r_id_lo        (mram_r_id),
    .r_data_lo      (mram_r_data),
    .r_resp_lo      (mram_r_resp),
    .r_last_lo      (mram_r_last),
    .r_ready_lo     (mram_r_ready)
);

   axi2apb_bridge
   #(
     .ID_WIDTH      (ID_WIDTH),
     .AXLEN_WIDTH   (AXLEN_WIDTH),
     .AXSIZE_WIDTH  (AXSIZE_WIDTH),
     .AXBURST_WIDTH (AXBURST_WIDTH),
     .AXCACHE_WIDTH (AXCACHE_WIDTH),
     .AXPROT_WIDTH  (AXPROT_WIDTH),
     .ADDR_WIDTH    (ADDR_WIDTH),
     .BRESP_WIDTH   (`CPU_AXI_RESP_WIDTH),
     .RRESP_WIDTH   (`CPU_AXI_RESP_WIDTH)
     )
     u_axi2apb_bridge
   (
      .clock          (clock),
      .reset          (reset),
    // AW channel
    .aw_valid     (cpu_sub_aw_valid),
    .aw_id        (axi_AWID),
    .aw_addr      (axi_AWADDR),
    .aw_len       (axi_AWLEN),
    .aw_size      (axi_AWSIZE),
    .aw_burst     (axi_AWBURST),
    .aw_lock      (axi_AWLOCK),
    .aw_cache     (axi_AWCACHE),
    .aw_prot      (axi_AWPROT),
    .aw_qos       ('0),
    .aw_ready     (cpu_sub_aw_ready),

    // W channel
    .w_valid      (cpu_sub_w_valid),
    .w_data       (axi_WDATA),
    .w_strb       (axi_WSTRB),
    .w_last       (axi_WLAST),
    .w_ready      (cpu_sub_w_ready),

    // B channel
    .b_valid      (cpu_sub_b_valid),
    .b_id         (cpu_sub_b_id),
    .b_resp       (cpu_sub_b_resp),
    .b_ready      (cpu_sub_b_ready),

    // AR channel
    .ar_valid     (cpu_sub_ar_valid),
    .ar_id        (axi_ARID),
    .ar_addr      (axi_ARADDR),
    .ar_len       (axi_ARLEN),
    .ar_size      (axi_ARSIZE),
    .ar_burst     (axi_ARBURST),
    .ar_lock      (axi_ARLOCK),
    .ar_cache     (axi_ARCACHE),
    .ar_prot      (axi_ARPROT),
    .ar_qos       ('0),
    .ar_ready     (cpu_sub_ar_ready),

    // R channel
    .r_valid      (cpu_sub_r_valid),
    .r_id         (cpu_sub_r_id),
    .r_data       (cpu_sub_r_data),
    .r_resp       (cpu_sub_r_resp),
    .r_last       (cpu_sub_r_last),
    .r_ready      (cpu_sub_r_ready),

    // APB interface
    .apb_psel     (apb_psel),
    .apb_penable  (apb_penable),
    .apb_paddr    (apb_paddr),
    .apb_pwrite   (apb_pwrite),
    .apb_pwdata   (apb_pwdata),
    .apb_pready   (apb_pready),
    .apb_prdata   (apb_prdata),
    .apb_pslverr  (apb_pslverr)
);

   // TODO: Use PMA function
function automatic logic addr_is_esr (input [31:0] paddr);
   return paddr[31] == 1'b1;
endfunction

endmodule
   

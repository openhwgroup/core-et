// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------
// CPU Subsystem APB multiplexer
//------------------------------------------------------------------------------

`include "soc.vh"

module cpu_apb_mux (
  // System signals
  input  logic                  clock,
  input  logic                  reset,
  // APB connection with the CPU subsystem
  input  APB_to_cpu_t           cpu_apb_req,
  output APB_from_cpu_t         cpu_apb_rsp,
  // APB connection with L1 ICache Data RAM
  output APB_to_cpu_icache_t    l1_icache_apb_req,
  input  APB_from_cpu_icache_t  l1_icache_apb_rsp,
  // APB connection with the Neighborhood
  output APB_to_neigh_t         neigh_apb_req,
  input  APB_from_neigh_t       neigh_apb_rsp,
  // APB connection with ESR
  output APB_to_cpu_reg_t       cpu_reg_apb_req,
  input  APB_from_cpu_reg_t     cpu_reg_apb_rsp,
  // APB connection with PLIC
  output APB_to_plic_t          plic_apb_req,
  input  APB_from_plic_t        plic_apb_rsp
);

//------------------------------------------------------------------------------
// INTERNAL DECLARATIONS
//------------------------------------------------------------------------------
// Internal APB from CPU subsystem
logic                           apb_psel;
logic                           apb_penable;
logic [`CPU_APB_ADDR_WIDTH-1:0] apb_paddr; 
logic [`CPU_APB_ADDR_WIDTH-4:0] apb_paddr64; 
logic [`CPU_APB_ADDR_WIDTH-3:0] apb_paddr32; 
logic                           apb_pwrite;
logic [`ESR_APB_D_WIDTH-1:0]    apb_pwdata;
logic                           apb_pready;
logic [`ESR_APB_D_WIDTH-1:0]    apb_prdata;
logic                           apb_pslverr;
logic [1:0]                     apb_pp_bits;
   

//------------------------------------------------------------------------------
// Flop interface with CPU subsystem
//------------------------------------------------------------------------------
apb_ff #(
  .ADDR_WIDTH(`CPU_APB_ADDR_WIDTH),
  .DATA_WIDTH(`ESR_APB_D_WIDTH)
) cpu_apb_ff (
  // System signals
  .clock          (clock),
  .reset          (reset),
  // Input slave side
  .apb_in_psel    (cpu_apb_req.apb_psel),
  .apb_in_penable (cpu_apb_req.apb_penable),
  .apb_in_paddr   (cpu_apb_req.apb_paddr),
  .apb_in_pwrite  (cpu_apb_req.apb_pwrite),
  .apb_in_pwdata  (cpu_apb_req.apb_pwdata),
  .apb_in_pready  (cpu_apb_rsp.apb_pready),
  .apb_in_prdata  (cpu_apb_rsp.apb_prdata),
  .apb_in_pslverr (cpu_apb_rsp.apb_pslverr),
  // Output master side
  .apb_out_psel   (apb_psel),
  .apb_out_penable(apb_penable),
  .apb_out_paddr  (apb_paddr),
  .apb_out_pwrite (apb_pwrite),
  .apb_out_pwdata (apb_pwdata),
  .apb_out_pready (apb_pready),
  .apb_out_prdata (apb_prdata),
  .apb_out_pslverr(apb_pslverr)
);

assign apb_paddr64 = apb_paddr[`CPU_APB_ADDR_WIDTH-1:3];
assign apb_paddr32 = apb_paddr[`CPU_APB_ADDR_WIDTH-1:2];
assign apb_pp_bits = apb_paddr[`CPU_APB_ADDR_PP_RANGE];
   
//------------------------------------------------------------------------------
// Multiplex access to different modules
//------------------------------------------------------------------------------
always_comb
begin
  apb_pready  = '0;
  apb_prdata  = '0;
  apb_pslverr = '0;
  
  // Initialize request for neigh
  neigh_apb_req.apb_psel    = '0;
  neigh_apb_req.apb_penable = '0;
  neigh_apb_req.apb_paddr   = '0;
  neigh_apb_req.apb_pwrite  = apb_pwrite;
  neigh_apb_req.apb_pwdata  = apb_pwdata;

  // Initialize request for CPU Registers
  cpu_reg_apb_req.apb_psel    = '0;
  cpu_reg_apb_req.apb_penable = '0;
  cpu_reg_apb_req.apb_paddr   = '0;
  cpu_reg_apb_req.apb_pwrite  = apb_pwrite;
  cpu_reg_apb_req.apb_pwdata  = apb_pwdata;

  // Initialize request for L1 ICache
  l1_icache_apb_req.apb_psel    = '0;
  l1_icache_apb_req.apb_penable = '0;
  l1_icache_apb_req.apb_paddr   = '0;
  l1_icache_apb_req.apb_pwrite  = apb_pwrite;
  l1_icache_apb_req.apb_pwdata  = apb_pwdata;
  
  // Initialize request for PLIC
  plic_apb_req.apb_psel    = '0;
  plic_apb_req.apb_penable = '0;
  plic_apb_req.apb_paddr   = '0;
  plic_apb_req.apb_pwrite  = apb_pwrite;
  plic_apb_req.apb_pwdata  = apb_pwdata[`PLIC_DATA_WIDTH-1:0];

  // Decode destination and re-arrange address format 
  casez (apb_paddr[`CPU_APB_ADDR_DEST_RANGE])

    // Neighborhood / Minion ESR
    `CPU_APB_ADDR_HART_ESR,
    `CPU_APB_ADDR_NEIGH_ESR : begin
      neigh_apb_req.apb_psel    = apb_psel;
      neigh_apb_req.apb_penable = apb_penable;
      neigh_apb_req.apb_paddr   = {1'b0, apb_paddr64[`CPU_APB_ADDR_MIN_OR_NEIGH_BIT], 
                                         apb_pp_bits,
                                         apb_paddr64[`NEIGH_DM_APB_ADDR_MIN_RANGE],
                                         apb_paddr64[`NEIGH_DM_APB_ADDR_THREAD_RANGE],
                                         apb_paddr64[`NEIGH_DM_APB_ADDR_REG_RANGE]};

      apb_pready  = neigh_apb_rsp.apb_pready;
      apb_prdata  = neigh_apb_rsp.apb_prdata;
      apb_pslverr = neigh_apb_rsp.apb_pslverr;
    end

   // Neighborhood / Minion Cache Debug
    `CPU_APB_ADDR_MINION_DCACHE,
    `CPU_APB_ADDR_NEIGH_ICACHE : begin
      neigh_apb_req.apb_psel    = apb_psel;
      neigh_apb_req.apb_penable = apb_penable;
      neigh_apb_req.apb_paddr   = {1'b1, apb_paddr64[`NEIGH_CH_DM_APB_ADDR_WIDTH-1:0]};

      apb_pready  = neigh_apb_rsp.apb_pready;
      apb_prdata  = neigh_apb_rsp.apb_prdata;
      apb_pslverr = neigh_apb_rsp.apb_pslverr;
    end

    // CPU Registers
    `CPU_APB_ADDR_CPU_REG : begin
      cpu_reg_apb_req.apb_psel    = apb_psel;
      cpu_reg_apb_req.apb_penable = apb_penable;
      cpu_reg_apb_req.apb_paddr   = {apb_pp_bits,
                                     apb_paddr64[`ESR_SHIRE_OTHER_APB_AD_PP_WIDTH-3:0]};

      apb_pready  = cpu_reg_apb_rsp.apb_pready;
      apb_prdata  = cpu_reg_apb_rsp.apb_prdata;
      apb_pslverr = cpu_reg_apb_rsp.apb_pslverr;
    end

    // L1 ICache Debug
    `CPU_APB_ADDR_L1_ICACHE : begin
      l1_icache_apb_req.apb_psel    = apb_psel;
      l1_icache_apb_req.apb_penable = apb_penable;
      l1_icache_apb_req.apb_paddr   = apb_paddr64[`ICACHE_DATA_APB_ADDR_WIDTH-1:0];
      apb_pready  = l1_icache_apb_rsp.apb_pready;
      apb_prdata  = l1_icache_apb_rsp.apb_prdata;
      apb_pslverr = l1_icache_apb_rsp.apb_pslverr;
    end

    // PLIC
    `CPU_APB_ADDR_PLIC : begin
      plic_apb_req.apb_psel    = apb_psel;
      plic_apb_req.apb_penable = apb_penable;
      plic_apb_req.apb_paddr   = apb_paddr32[`PLIC_APB_ADDR_WIDTH-1:0];
      apb_pready  = plic_apb_rsp.apb_pready;
      apb_pslverr = plic_apb_rsp.apb_pslverr;

      // Align to 4-bytes data into a 8-bytes bus
      if (apb_paddr32[0]) begin
        apb_prdata  = {plic_apb_rsp.apb_prdata, {`PLIC_DATA_WIDTH{1'b0}}};
        plic_apb_req.apb_pwdata  = apb_pwdata[`CPU_APB_DATA_WIDTH-1:`PLIC_DATA_WIDTH];
      end else begin
        apb_prdata  = `ZX(`CPU_APB_DATA_WIDTH, plic_apb_rsp.apb_prdata);
        plic_apb_req.apb_pwdata  = apb_pwdata[`PLIC_DATA_WIDTH-1:0];
      end
    end

    // No destination matched
    default : begin
      apb_pready  = apb_psel & apb_penable;
      apb_prdata  = '0;
      apb_pslverr = apb_psel & apb_penable;
    end
  endcase

end


endmodule

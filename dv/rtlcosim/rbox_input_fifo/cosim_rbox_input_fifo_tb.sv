// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_input_fifo vs new rbox_input_fifo.

`include "soc.vh"

module cosim_rbox_input_fifo_tb (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic                                  alloc_i,
  input  logic [511:0]                          in_data_i,
  input  logic [$clog2(`RBOX_IN_FIFO_SZ)-1:0]  push_ptr_i,
  input  logic                                  push_i,
  input  logic                                  pop_i,
  input  logic                                  buffer_end_i,

  // New module outputs
  output logic [$clog2(`RBOX_IN_FIFO_SZ)-1:0]  new_alloc_ptr_o,
  output logic                                  new_push_done_o,
  output logic [511:0]                          new_out_data_o,
  output logic [2:0]                            new_out_pck_type_o,
  output logic                                  new_out_valid_o,
  output logic                                  new_pop_done_o,
  output logic                                  new_full_o,
  output logic                                  new_empty_o,

  // Original module outputs
  output logic [$clog2(`RBOX_IN_FIFO_SZ)-1:0]  orig_alloc_ptr_o,
  output logic                                  orig_push_done_o,
  output logic [511:0]                          orig_out_data_o,
  output logic [2:0]                            orig_out_pck_type_o,
  output logic                                  orig_out_valid_o,
  output logic                                  orig_pop_done_o,
  output logic                                  orig_full_o,
  output logic                                  orig_empty_o
);

  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  // Adapt reset
  logic reset;
  assign reset = ~rst_ni;

  // DFT/config = disabled (all zeros)
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // New module output types
  inpck_type_e new_pck_type;
  assign new_out_pck_type_o = new_pck_type;

  // Original output type
  rbox_inpck_type_t orig_pck_type;
  assign orig_out_pck_type_o = orig_pck_type;

  // ── New module ────────────────────────────────────────

  rbox_input_fifo u_new (
    .clk_i        (clk_i),
    .rst_ni       (rst_ni),
    .ram_cfg_i    (ram_cfg_t'('0)),
    .dft_i        (dft_t'('0)),
    .alloc_ptr_o  (new_alloc_ptr_o),
    .alloc_i      (alloc_i),
    .in_data_i    (in_data_i),
    .push_ptr_i   (push_ptr_i),
    .push_i       (push_i),
    .push_done_o  (new_push_done_o),
    .out_data_o   (new_out_data_o),
    .out_pck_type_o(new_pck_type),
    .out_valid_o  (new_out_valid_o),
    .pop_i        (pop_i),
    .pop_done_o   (new_pop_done_o),
    .buffer_end_i (buffer_end_i),
    .full_o       (new_full_o),
    .empty_o      (new_empty_o)
  );

  // ── Original module ───────────────────────────────────

  rbox_input_fifo_orig u_orig (
    .clock                   (clk_i),
    .reset                   (reset),
    .esr_shire_cache_ram_cfg (orig_ram_cfg),
    .alloc_ptr               (orig_alloc_ptr_o),
    .alloc                   (alloc_i),
    .in_data                 (in_data_i),
    .push_ptr                (push_ptr_i),
    .push                    (push_i),
    .push_done               (orig_push_done_o),
    .out_data                (orig_out_data_o),
    .out_pck_type            (orig_pck_type),
    .out_valid               (orig_out_valid_o),
    .pop                     (pop_i),
    .pop_done                (orig_pop_done_o),
    .buffer_end              (buffer_end_i),
    .full                    (orig_full_o),
    .empty                   (orig_empty_o)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Renamed copy of original rbox_input_fifo
// ════════════════════════════════════════════════════════════

module rbox_input_fifo_orig
(
    input  logic                      clock,
    input  logic                      reset,
    input  esr_shire_cache_ram_cfg_t  esr_shire_cache_ram_cfg,

    output logic [`RBOX_FIFO_PTR_RNG] alloc_ptr,
    input                             alloc,

    input  logic [511:0]              in_data,
    input  logic [`RBOX_FIFO_PTR_RNG] push_ptr,
    input  logic                      push,
    output logic                      push_done,

    output logic [511:0]              out_data,
    output rbox_inpck_type_t          out_pck_type,
    output logic                      out_valid,
    input  logic                      pop,
    output logic                      pop_done,

    input  logic                      buffer_end,

    output logic                      full,
    output logic                      empty
);

logic [1:0]                         sram_en;
logic [1:0]                         write_en;
logic [1:0][`RBOX_IN_FIFO_ADDR_RNG] address;
logic [1:0][`RBOX_IN_FIFO_ADDR_RNG] address_pop;
logic [1:0][255:0]                  out_data_sram;
logic [511:0]                       out_data_nxt;
logic [511:0]                       out_data_ff;
logic                               out_valid_sram;
logic                               out_valid_sram_nxt;
logic                               out_valid_ff_nxt;
logic                               out_valid_ff;
logic                               out_pck_ready;
rbox_inpck_type_t                   out_pck_type_nxt;
rbox_inpck_type_t                   out_pck_type_sram;
rbox_inpck_type_t                   out_pck_type_ff;
logic [1:0]                         out_pck_sz_pop;
logic [1:0]                         dealloc;
logic                               load_data;
logic [1:0][`RBOX_IN_FIFO_ENTR_RNG] in_use_ff;
logic [1:0][`RBOX_IN_FIFO_ENTR_RNG] in_use_nxt;
logic [1:0][`RBOX_IN_FIFO_ENTR_RNG] ready_ff;
logic [1:0][`RBOX_IN_FIFO_ENTR_RNG] ready_nxt;
logic [`RBOX_IN_FIFO_PCKT_RNG]      read_ptr_ff;
logic [`RBOX_IN_FIFO_PCKT_RNG]      read_ptr_nxt;
logic [`RBOX_IN_FIFO_PCKT_RNG]      read_ptr_sram_ff;
logic [`RBOX_IN_FIFO_PCKT_RNG]      read_ptr_sram_nxt;
logic [`RBOX_IN_FIFO_ADDR_RNG]      write_ptr_ff;
logic [`RBOX_IN_FIFO_ADDR_RNG]      write_ptr_nxt;
logic                               update_read_ptr;
logic                               update_read_ptr_sram;
logic                               update_write_ptr;
logic                               update_in_use;
logic                               update_ready;

`RST_EN_FF(clock, reset, update_in_use,        in_use_ff,        in_use_nxt,        0)
`RST_EN_FF(clock, reset, update_ready,         ready_ff,         ready_nxt,         0)
`RST_EN_FF(clock, reset, update_read_ptr,      read_ptr_ff,      read_ptr_nxt,      0)
`RST_EN_FF(clock, reset, update_read_ptr_sram, read_ptr_sram_ff, read_ptr_sram_nxt, 0)
`RST_EN_FF(clock, reset, update_write_ptr,     write_ptr_ff,     write_ptr_nxt,     0)

`RST_FF(clock, reset, out_valid_ff,   out_valid_ff_nxt,   0)
`RST_FF(clock, reset, out_valid_sram, out_valid_sram_nxt, 0)

`EN_FF(clock, load_data, out_data_ff,     out_data_nxt    )
`EN_FF(clock, load_data, out_pck_type_ff, out_pck_type_nxt)

always_comb
begin
    full  =  in_use_ff[0][write_ptr_ff];
    empty = ~|in_use_ff;
    in_use_nxt = in_use_ff;
    ready_nxt  = ready_ff;
    read_ptr_nxt  = read_ptr_ff;
    write_ptr_nxt = write_ptr_ff;
    pop_done             = 1'b0;
    push_done            = 1'b0;
    update_in_use        = 1'b0;
    update_ready         = 1'b0;
    update_read_ptr      = 1'b0;
    update_read_ptr_sram = 1'b0;
    update_write_ptr     = 1'b0;
    load_data            = 1'b0;
    sram_en     = 0;
    write_en    = 0;
    address     = 0;
    address_pop = 0;
    dealloc     = 0;
    out_data_nxt       = out_data_ff;
    out_valid_sram_nxt = 0;
    out_valid_ff_nxt   = (!pop & out_valid_ff) | out_valid_sram;
    alloc_ptr = `ZX($bits(alloc_ptr), write_ptr_ff);
    if (out_valid_sram) begin
        load_data = 1;
        out_data_nxt[255:  0] = read_ptr_sram_ff[0] ? out_data_sram[1] : out_data_sram[0];
        out_data_nxt[511:256] = read_ptr_sram_ff[0] ? out_data_sram[0] : out_data_sram[1];
    end
    if (alloc & ~full) begin
        in_use_nxt[0][write_ptr_ff] = 1'b1;
        in_use_nxt[1][write_ptr_ff] = 1'b1;
        write_ptr_nxt++;
        update_write_ptr = 1;
        update_in_use    = 1;
    end
    out_pck_type_sram = rbox_inpck_type_t'(out_data_nxt[$bits(rbox_inpck_type_t)-1:0]);
    out_pck_sz_pop = rbox_inpck_sz_f(out_valid_sram ? out_pck_type_sram : out_pck_type_ff);
    address[0] = read_ptr_ff[0] ? (read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG] + 1) : read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG];
    address[1] = read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG];
    read_ptr_sram_nxt = read_ptr_ff;
    out_pck_ready =   ready_ff[0][address[0]] && ready_ff[1][address[1]]
                   || buffer_end && read_ptr_ff[0] && ready_ff[1][address[1]];
    if ((read_ptr_sram_ff != read_ptr_ff) && out_pck_ready) begin
        sram_en[0] = 1;
        sram_en[1] = 1;
        out_valid_sram_nxt = 1;
        update_read_ptr_sram = 1;
    end
    if (pop && ~empty && (read_ptr_ff == read_ptr_sram_ff)) begin
        address_pop[0] = read_ptr_ff[0] ? (read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG] + 1) : read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG];
        address_pop[1] = read_ptr_ff[`RBOX_IN_FIFO_LINE_RNG];
        if (    ready_ff[0][address_pop[0]]
            && (ready_ff[1][address_pop[1]] || !out_pck_sz_pop[1])) begin
            dealloc[0] = (~read_ptr_ff[0] |  read_ptr_ff[0] & out_pck_sz_pop[1]);
            dealloc[1] = ( read_ptr_ff[0] | ~read_ptr_ff[0] & out_pck_sz_pop[1]);
            in_use_nxt[0][address_pop[0]] = in_use_ff[0][address_pop[0]] & ~dealloc[0];
            in_use_nxt[1][address_pop[1]] = in_use_ff[1][address_pop[1]] & ~dealloc[1];
            ready_nxt[0][address_pop[0]]  = ready_ff[0][address_pop[0]]  & ~dealloc[0];
            ready_nxt[1][address_pop[1]]  = ready_ff[1][address_pop[1]]  & ~dealloc[1];
            read_ptr_nxt = out_pck_sz_pop[1] ? read_ptr_ff + 2 : read_ptr_ff + 1;
            update_in_use   = 1;
            update_ready    = 1;
            update_read_ptr = 1;
            pop_done  = 1;
        end
    end
    if (push && !out_valid_sram_nxt) begin
        address[0] = push_ptr;
        address[1] = push_ptr;
        sram_en[0] = 1;
        sram_en[1] = 1;
        write_en[0] = 1;
        write_en[1] = 1;
        ready_nxt[0][push_ptr] = 1;
        ready_nxt[1][push_ptr] = 1;
        update_ready    = 1;
        push_done = 1;
        if ({push_ptr, 1'b0} == read_ptr_ff) begin
            out_data_nxt         = in_data;
            out_valid_ff_nxt     = 1;
            update_read_ptr_sram = 1;
            load_data            = 1;
        end
    end
    out_pck_type_nxt = rbox_inpck_type_t'(out_data_nxt[$bits(rbox_inpck_type_t)-1:0]);
    out_valid    = out_valid_ff;
    out_data     = out_data_ff;
    out_pck_type = out_pck_type_ff;
end

gen_mem1p
#(.WIDTH (256), .WORDS (32))
fifo_data_hi (
   .clock   (clock           ),
   .address (address[1]      ),
   .en      (sram_en[1]  && !esr_shire_cache_ram_cfg.dft__ram_rei ),
   .we      (write_en[1] && !esr_shire_cache_ram_cfg.dft__ram_wei ),
   .din     (in_data[511:256]),
   .dout    (out_data_sram[1])
);

gen_mem1p
#(.WIDTH (256), .WORDS (32))
fifo_data_lo (
   .clock   (clock           ),
   .address (address[0]      ),
   .en      (sram_en[0]  && !esr_shire_cache_ram_cfg.dft__ram_rei ),
   .we      (write_en[0] && !esr_shire_cache_ram_cfg.dft__ram_wei ),
   .din     (in_data[255:0]  ),
   .dout    (out_data_sram[0])
);

wire unused_ok = &{1'b0, esr_shire_cache_ram_cfg, esr_shire_cache_ram_cfg};

endmodule

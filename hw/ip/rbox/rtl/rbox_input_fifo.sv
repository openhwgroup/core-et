// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX input FIFO.
//
// 32-entry FIFO storing 512-bit input packets. Each entry is split across
// two 256-bit SRAMs (hi/lo). Packets can be 1 or 2 entries depending on
// type. External logic allocates entries via alloc/alloc_ptr, pushes data,
// and pops complete packets.
//
// Uses prim_ram_1p for the backing SRAMs.

module rbox_input_fifo
  import rbox_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i,

  output logic [FifoPtrAddrSz-1:0]   alloc_ptr_o,
  input  logic                       alloc_i,

  input  logic [511:0]               in_data_i,
  input  logic [FifoPtrAddrSz-1:0]   push_ptr_i,
  input  logic                       push_i,
  output logic                       push_done_o,

  output logic [511:0]               out_data_o,
  output inpck_type_e                out_pck_type_o,
  output logic                       out_valid_o,
  input  logic                       pop_i,
  output logic                       pop_done_o,

  input  logic                       buffer_end_i,

  output logic                       full_o,
  output logic                       empty_o
);

  localparam int unsigned FifoSz     = InFifoSz;       // 32
  localparam int unsigned FifoAddrSz = InFifoAddrSz;   // 5
  localparam int unsigned FifoPcktSz = InFifoPcktSz;    // 6

  // SRAM interface
  logic [1:0]                     sram_en;
  logic [1:0]                     write_en;
  logic [1:0][FifoAddrSz-1:0]    address;
  logic [1:0][FifoAddrSz-1:0]    address_pop;
  logic [1:0][255:0]              out_data_sram;
  logic [511:0]                   out_data_d;
  logic [511:0]                   out_data_q;
  logic                           out_valid_sram;
  logic                           out_valid_sram_d;
  logic                           out_valid_d;
  logic                           out_valid_q;
  logic                           out_pck_ready;
  inpck_type_e                    out_pck_type_d;
  inpck_type_e                    out_pck_type_sram;
  inpck_type_e                    out_pck_type_q;
  /* verilator lint_off UNUSEDSIGNAL */ // out_pck_sz_pop[0] not used — only [1] checked for 2-entry packets
  logic [1:0]                     out_pck_sz_pop;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [1:0]                     dealloc;

  logic                           load_data;
  logic [1:0][FifoSz-1:0]        in_use_q;
  logic [1:0][FifoSz-1:0]        in_use_d;
  logic [1:0][FifoSz-1:0]        ready_q;
  logic [1:0][FifoSz-1:0]        ready_d;
  logic [FifoPcktSz-1:0]         read_ptr_q;
  logic [FifoPcktSz-1:0]         read_ptr_d;
  logic [FifoPcktSz-1:0]         read_ptr_sram_q;
  logic [FifoPcktSz-1:0]         read_ptr_sram_d;
  logic [FifoAddrSz-1:0]         write_ptr_q;
  logic [FifoAddrSz-1:0]         write_ptr_d;
  logic                           update_read_ptr;
  logic                           update_read_ptr_sram;
  logic                           update_write_ptr;
  logic                           update_in_use;
  logic                           update_ready;

  // Sequential logic
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      in_use_q        <= '0;
      ready_q         <= '0;
      read_ptr_q      <= '0;
      read_ptr_sram_q <= '0;
      write_ptr_q     <= '0;
      out_valid_q     <= 1'b0;
      out_valid_sram  <= 1'b0;
    end else begin
      if (update_in_use)        in_use_q        <= in_use_d;
      if (update_ready)         ready_q         <= ready_d;
      if (update_read_ptr)      read_ptr_q      <= read_ptr_d;
      if (update_read_ptr_sram) read_ptr_sram_q <= read_ptr_sram_d;
      if (update_write_ptr)     write_ptr_q     <= write_ptr_d;
      out_valid_q    <= out_valid_d;
      out_valid_sram <= out_valid_sram_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (load_data) begin
      out_data_q     <= out_data_d;
      out_pck_type_q <= out_pck_type_d;
    end
  end

  // Combinational logic
  /* verilator lint_off UNUSEDSIGNAL */ // Not all ram_cfg/dft bits used in behavioral model

  always_comb begin
    full_o  =  in_use_q[0][write_ptr_q];
    empty_o = ~|in_use_q;

    in_use_d = in_use_q;
    ready_d  = ready_q;

    read_ptr_d  = read_ptr_q;
    write_ptr_d = write_ptr_q;

    pop_done_o           = 1'b0;
    push_done_o          = 1'b0;
    update_in_use        = 1'b0;
    update_ready         = 1'b0;
    update_read_ptr      = 1'b0;
    update_read_ptr_sram = 1'b0;
    update_write_ptr     = 1'b0;
    load_data            = 1'b0;

    sram_en     = '0;
    write_en    = '0;
    address     = '0;
    address_pop = '0;
    dealloc     = '0;

    out_data_d         = out_data_q;
    out_valid_sram_d   = 1'b0;
    out_valid_d        = (~pop_i & out_valid_q) | out_valid_sram;

    alloc_ptr_o = FifoPtrAddrSz'(write_ptr_q);

    if (out_valid_sram) begin
      load_data = 1'b1;
      out_data_d[255:0]   = read_ptr_sram_q[0] ? out_data_sram[1] : out_data_sram[0];
      out_data_d[511:256]  = read_ptr_sram_q[0] ? out_data_sram[0] : out_data_sram[1];
    end

    if (alloc_i & ~full_o) begin
      in_use_d[0][write_ptr_q] = 1'b1;
      in_use_d[1][write_ptr_q] = 1'b1;

      write_ptr_d = write_ptr_q + FifoAddrSz'(1);

      update_write_ptr = 1'b1;
      update_in_use    = 1'b1;
    end

    out_pck_type_sram = inpck_type_e'(out_data_d[$bits(inpck_type_e)-1:0]);
    out_pck_sz_pop = inpck_sz(out_valid_sram ? out_pck_type_sram : out_pck_type_q);

    address[0] = read_ptr_q[0] ? (read_ptr_q[FifoPcktSz-1:1] + FifoAddrSz'(1)) : read_ptr_q[FifoPcktSz-1:1];
    address[1] = read_ptr_q[FifoPcktSz-1:1];

    read_ptr_sram_d = read_ptr_q;

    out_pck_ready =
                    // The next two 32-byte blocks are valid
                      ready_q[0][address[0]] && ready_q[1][address[1]]
                    // End of buffer and a single 32-byte block is valid
                   || buffer_end_i && read_ptr_q[0] && ready_q[1][address[1]];

    if ((read_ptr_sram_q != read_ptr_q) && out_pck_ready) begin
      sram_en[0] = 1'b1;
      sram_en[1] = 1'b1;

      out_valid_sram_d = 1'b1;

      update_read_ptr_sram = 1'b1;
    end

    if (pop_i && ~empty_o && (read_ptr_q == read_ptr_sram_q)) begin
      address_pop[0] = read_ptr_q[0] ? (read_ptr_q[FifoPcktSz-1:1] + FifoAddrSz'(1)) : read_ptr_q[FifoPcktSz-1:1];
      address_pop[1] = read_ptr_q[FifoPcktSz-1:1];

      if (    ready_q[0][address_pop[0]]
          && (ready_q[1][address_pop[1]] || !out_pck_sz_pop[1]))
      begin
        dealloc[0] = (~read_ptr_q[0] |  read_ptr_q[0] & out_pck_sz_pop[1]);
        dealloc[1] = ( read_ptr_q[0] | ~read_ptr_q[0] & out_pck_sz_pop[1]);

        in_use_d[0][address_pop[0]] = in_use_q[0][address_pop[0]] & ~dealloc[0];
        in_use_d[1][address_pop[1]] = in_use_q[1][address_pop[1]] & ~dealloc[1];

        ready_d[0][address_pop[0]]  = ready_q[0][address_pop[0]]  & ~dealloc[0];
        ready_d[1][address_pop[1]]  = ready_q[1][address_pop[1]]  & ~dealloc[1];

        read_ptr_d = out_pck_sz_pop[1] ? read_ptr_q + FifoPcktSz'(2) : read_ptr_q + FifoPcktSz'(1);

        update_in_use   = 1'b1;
        update_ready    = 1'b1;
        update_read_ptr = 1'b1;

        pop_done_o = 1'b1;
      end
    end

    if (push_i && !out_valid_sram_d) begin
      address[0] = push_ptr_i;
      address[1] = push_ptr_i;

      sram_en[0]  = 1'b1;
      sram_en[1]  = 1'b1;

      write_en[0] = 1'b1;
      write_en[1] = 1'b1;

      ready_d[0][push_ptr_i] = 1'b1;
      ready_d[1][push_ptr_i] = 1'b1;

      update_ready = 1'b1;

      push_done_o = 1'b1;

      if ({push_ptr_i, 1'b0} == read_ptr_q) begin
        out_data_d           = in_data_i;
        out_valid_d          = 1'b1;
        update_read_ptr_sram = 1'b1;
        load_data            = 1'b1;
      end
    end

    out_pck_type_d = inpck_type_e'(out_data_d[$bits(inpck_type_e)-1:0]);

    out_valid_o    = out_valid_q;
    out_data_o     = out_data_q;
    out_pck_type_o = out_pck_type_q;
  end

  /* verilator lint_on UNUSEDSIGNAL */

  // ── SRAM instances ──────────────────────────────────────

  /* verilator lint_off PINCONNECTEMPTY */ // alert_o intentionally unconnected

  prim_ram_1p #(
    .Width (256),
    .Depth (FifoSz)
  ) u_fifo_data_hi (
    .clk_i   (clk_i),
    .rst_ni  (rst_ni),
    .req_i   (sram_en[1] && !dft_i.ram_rei),
    .we_i    (write_en[1] && !dft_i.ram_wei),
    .addr_i  (address[1]),
    .wdata_i (in_data_i[511:256]),
    .rdata_o (out_data_sram[1]),
    .cfg_i   (ram_cfg_i),
    .dft_i   (dft_i),
    .alert_o ()
  );

  prim_ram_1p #(
    .Width (256),
    .Depth (FifoSz)
  ) u_fifo_data_lo (
    .clk_i   (clk_i),
    .rst_ni  (rst_ni),
    .req_i   (sram_en[0] && !dft_i.ram_rei),
    .we_i    (write_en[0] && !dft_i.ram_wei),
    .addr_i  (address[0]),
    .wdata_i (in_data_i[255:0]),
    .rdata_o (out_data_sram[0]),
    .cfg_i   (ram_cfg_i),
    .dft_i   (dft_i),
    .alert_o ()
  );

  /* verilator lint_on PINCONNECTEMPTY */

endmodule

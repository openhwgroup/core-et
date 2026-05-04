// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module minion_debug_apb_slv (
  //////////////////////////////////////////////////////////////
  // System signals
  //////////////////////////////////////////////////////////////
  input  logic                                  clock,
  input  logic                                  reset,

  //////////////////////////////////////////////////////////////
  // APB slave interface
  //////////////////////////////////////////////////////////////
  // apb signals from master
  input  logic [`MIN_REG_APB_ADDR_WIDTH-1:0]    apb_paddr,
  input  logic                                  apb_pwrite,
  input  logic                                  apb_psel,
  input  logic                                  apb_penable,
  input  logic [`MINION_DEBUG_APB_D_WIDTH-1:0]  apb_pwdata,

  // apb signals to master
  output logic                                  apb_pready,
  output logic [`MINION_DEBUG_APB_D_WIDTH-1:0]  apb_prdata,
  output logic                                  apb_pslverr,

  //////////////////////////////////////////////////////////////
  // interface from core CSR
  //////////////////////////////////////////////////////////////
  input  logic [63:0]                           csr_wdata_ip,
  input  logic [`CORE_NR_THREADS-1:0]           update_ddata0_ip,
  output logic [`CORE_NR_THREADS-1:0][63:0]     read_ddata0_op,
  //////////////////////////////////////////////////////////////
  // interface to frontend
  //////////////////////////////////////////////////////////////
  output logic [`MINION_DEBUG_APB_D_WIDTH-1:0]  ffb_wdata_op,
  output logic [`MINION_DEBUG_FFB_WORDS-1:0]    ffb_en_op,
  output logic                                  ffb_thread_sel_op,

  //////////////////////////////////////////////////////////////
  // interface to intpipe
  //////////////////////////////////////////////////////////////
  output logic [`CORE_NR_THREADS-1:0]           ex_program_buffer,
  input  logic [`CORE_NR_THREADS-1:0]           in_debug_mode
);

  ////////////////////////////////////////////////////////////////////////////////
  // SIGNAL declarations
  ////////////////////////////////////////////////////////////////////////////////

  logic                                         thread_sel;
  logic [`MINION_DEBUG_APB_AD_WIDTH-2:0]        reg_sel;
  logic [`AD_ESR_PROT_BITS_SIZE-1:0]            pp;

  logic                                         s0_execute;
  logic                                         s0_en;
  logic                                         s0_addr_valid;
  logic [31:0]                                  s0_rdata;
  logic [`MINION_DEBUG_APB_REGS_SEL_RANGE]      s0_reg_en;
  logic                                         s0_pp_valid;

  logic                                         s1_execute;
  logic                                         s1_en;
  logic                                         s1_addr_valid;
  logic [31:0]                                  s1_rdata;
  logic [`MINION_DEBUG_APB_REGS_SEL_RANGE]      s1_reg_en;
  logic                                         s1_pp_valid;

  logic                                         s2_execute;

  logic [`CORE_NR_THREADS-1:0][63:0]            reg_ddata0;
  logic [`CORE_NR_THREADS-1:0][63:0]            reg_ddata0_next;

  logic                                         ffb_en_lo; // Enable instrucction buffer slot 0 (0-31) and slot 1 (0-63)
  logic [1:0]                                   ffb_en_hi; // Enable instruction buffer slot 2 (64-95) or slot 3 (96-127)


  // thread and register selection:
  //assign {reg_sel,thread_sel} = apb_paddr;
  assign {pp, thread_sel, reg_sel} = apb_paddr;

  // s0: setup phase (address decoding + read)
  always_comb begin
    s0_en = apb_psel && !apb_penable;
    s0_addr_valid = 1'b1;
    s0_rdata = '0;
    s0_execute = '0;
    s0_pp_valid = '0;

    s0_reg_en = '0;

    if (s0_en) begin
      s0_pp_valid = pp inside {`AD_ESR_PROT_BITS_MSG}; // Common for all current registers

      case (minion_debug_apb_regs_t'(reg_sel))
        minion_debug_nxdata0: begin
          s0_reg_en[int'(minion_debug_nxdata0)] = apb_pwrite;
          s0_rdata = reg_ddata0[thread_sel][31:0];
        end
        minion_debug_nxdata1: begin
          s0_reg_en[int'(minion_debug_nxdata1)] = apb_pwrite;
          s0_rdata = reg_ddata0[thread_sel][63:32];
        end
        minion_debug_axdata0: begin
          s0_reg_en[int'(minion_debug_axdata0)] = apb_pwrite;
          s0_rdata = reg_ddata0[thread_sel][31:0];
          s0_execute = apb_pwrite;
        end
        minion_debug_axdata1: begin
          s0_reg_en[int'(minion_debug_axdata1)] = apb_pwrite;
          s0_rdata = reg_ddata0[thread_sel][63:32];
          s0_execute = apb_pwrite;
        end
        minion_debug_abscmd,
        minion_debug_axprogbuf0,
        minion_debug_axprogbuf1: begin
          s0_reg_en[minion_debug_apb_regs_t'(reg_sel)] = apb_pwrite;
          s0_execute = apb_pwrite;
        end
        minion_debug_nxprogbuf0, minion_debug_nxprogbuf1: begin
          s0_reg_en[minion_debug_apb_regs_t'(reg_sel)] = apb_pwrite;
        end

        default: begin
          s0_addr_valid = 1'b0;
          s0_reg_en[int'(minion_debug_invalid)] = 1'b1;
        end
      endcase // case ( reg_sel )
    end // if ( s0_en )

  end

  // s1: access phase
  always_comb begin
    s1_en = apb_psel && apb_penable && apb_pready;

    // only dscratch is writeable.. setting previous value as default
    reg_ddata0_next = reg_ddata0;

    // udpates from apb
    if (!apb_pslverr) begin
      if (s1_reg_en[int'(minion_debug_nxdata0)] || s1_reg_en[int'(minion_debug_axdata0)]) begin
        reg_ddata0_next[thread_sel][31:0] = apb_pwdata[31:0];
      end
      if (s1_reg_en[int'(minion_debug_nxdata1)] || s1_reg_en[int'(minion_debug_axdata1)]) begin
        reg_ddata0_next[thread_sel][63:32] = apb_pwdata[31:0];
      end
    end

    // udpate from core
    for (int i = 0; i < `CORE_NR_THREADS; i++) begin
      if (update_ddata0_ip[i]) begin
        reg_ddata0_next[i] = csr_wdata_ip;
      end
    end

  end // always_comb

  `EN_FF(clock, s0_en && !apb_pwrite, s1_rdata, s0_rdata)
  `RST_EN_FF(clock, reset, s0_en || s1_en, s1_reg_en, s0_reg_en, '0)
  `RST_EN_FF(clock, reset, s0_en, s1_addr_valid, s0_addr_valid, '0)
  `RST_EN_FF(clock, reset, s0_en, s1_pp_valid, s0_pp_valid, '0)
  `FF(clock, reg_ddata0, reg_ddata0_next)
  `FF(clock, s1_execute, s0_execute)
  `FF(clock, s2_execute, s1_execute) // delay to raise the pready when the hart is set as busy

  // assign APB slave outputs
  assign apb_prdata = {32'h0, s1_rdata};
  assign apb_pslverr = !s1_addr_valid || !s1_pp_valid || !in_debug_mode[thread_sel];
  // Wait until the pb is written or the execution signal is send so the DM can detect the busy signal
  assign apb_pready = apb_psel && apb_penable && (
    s1_reg_en[int'(minion_debug_invalid)]  || // Error
    s1_addr_valid && !apb_pwrite || // read req
    s1_addr_valid && apb_pwrite && !s1_execute || //  write but doesn't execute
    s2_execute); // write does execute (delay to update the busy signal)

  // and connect output ports to frontend and CSR
  always_comb begin
    ffb_en_lo = s1_reg_en[int'(minion_debug_abscmd)];
    ffb_en_hi = {s1_reg_en[int'(minion_debug_axprogbuf1)] | s1_reg_en[int'(minion_debug_nxprogbuf1)],
                 s1_reg_en[int'(minion_debug_axprogbuf0)] | s1_reg_en[int'(minion_debug_nxprogbuf0)]};
    ffb_en_op = {ffb_en_hi, ffb_en_lo, ffb_en_lo} & {$bits(ffb_en_op){!apb_pslverr}};
    ffb_thread_sel_op = thread_sel;

    ffb_wdata_op = apb_pwdata;

    read_ddata0_op = reg_ddata0;

    ex_program_buffer = '0;
    ex_program_buffer[thread_sel] = s1_execute && !apb_pslverr;
  end


endmodule

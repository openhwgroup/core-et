// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_pmu_read_interface — PMU counter read interface.
//
// Manages per-thread PMU counter read selections with a configurable
// read delay propagation mechanism.
//
// Replaces: intpipe_csr_pmu_read_interface (etcore-soc)

module intpipe_csr_pmu_read_interface
  import minion_pkg::*;
  import intpipe_csr_pkg::*;
(
  input  logic                                                   clk_i,
  input  logic                                                   rst_ni,

  input  logic                                                   io_rw_mem_valid,
  input  logic                                                   io_rw_mem_thread_id,
  input  logic [CsrRegAddrSize-1:0]                              io_rw_mem_addr,
  output logic [NrThreads-1:0]                                   io_rw_mem_ready,

  output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]        pmu_read_sel,
  output logic                                                   pmu_read_en_wb
);

  logic [PmuCountersSelectBits-2:0]                        pmu_read_addr, pmu_read_addr_um, pmu_read_addr_mm;
  logic                                                    pmu_read_valid, pmu_read_valid_um, pmu_read_valid_mm;
  logic [NrThreads-1:0]                                    pmu_read_valid_th;
  logic [NrThreads-1:0][PmuCountersSelectBits-1:0]         pmu_read_sel_next;
  logic [NrThreads-1:0]                                    pmu_read_current_sel;
  logic [NrThreads-1:0][PmuCountersReadDelay-1:0]          pmu_read_prop;
  logic [NrThreads-1:0][PmuCountersReadDelay-1:0]          pmu_read_prop_next;

  logic [NrThreads-1:0]                                    pmu_read_ready;
  logic [NrThreads-1:0]                                    pmu_read_ready_next;
  logic [NrThreads-1:0]                                    pmu_read_ongoing;

  always_comb begin
    pmu_read_ongoing = NrThreads'(0);
    for (integer i = 0; i < NrThreads; i++) begin
      pmu_read_ongoing[i] |= (|pmu_read_prop[i]);
    end

    // Valid access if within the performance counters range
    pmu_read_valid_mm = io_rw_mem_valid && ((io_rw_mem_addr >= CsrRegAddrSize'(csr_ad_MHPMCOUNTER3)) && (io_rw_mem_addr <= CsrRegAddrSize'(csr_ad_MHPMCOUNTER8)));
    pmu_read_valid_um = io_rw_mem_valid && ((io_rw_mem_addr >= CsrRegAddrSize'(csr_ad_HPMCOUNTER3))  && (io_rw_mem_addr <= CsrRegAddrSize'(csr_ad_HPMCOUNTER8)));
    pmu_read_valid    = pmu_read_valid_mm || pmu_read_valid_um;

    // Obtain register selection offset
    pmu_read_addr_mm = $bits(pmu_read_addr)'(io_rw_mem_addr - csr_ad_MHPMCOUNTER3);
    pmu_read_addr_um = $bits(pmu_read_addr)'(io_rw_mem_addr - csr_ad_HPMCOUNTER3);
    pmu_read_addr    = (pmu_read_valid_mm) ? pmu_read_addr_mm : pmu_read_addr_um;

    // Per thread valid read
    for (integer i = 0; i < NrThreads; i++) begin
      pmu_read_valid_th[i] = pmu_read_valid && (io_rw_mem_thread_id == i[0])
                                            && !(|(pmu_read_ongoing & ~(NrThreads'(1) << i)));
      pmu_read_current_sel[i] = pmu_read_valid && (io_rw_mem_thread_id == i[0]) && (pmu_read_sel[i] == {pmu_read_addr, io_rw_mem_thread_id});
      pmu_read_sel_next[i]    = {pmu_read_addr, io_rw_mem_thread_id};
    end

    // Propagate read and generate a ready signal for every thread
    for (integer i = 0; i < NrThreads; i++) begin
      pmu_read_ready_next[i] = pmu_read_ready[i];
      if (pmu_read_prop[i][PmuCountersReadDelay-1])
        pmu_read_ready_next[i] = 1'b1;
      else if (pmu_read_ready[i] && pmu_read_current_sel[i])
        pmu_read_ready_next[i] = 1'b1;
      else if (pmu_read_valid_th[i] && !pmu_read_current_sel[i])
        pmu_read_ready_next[i] = 1'b0;

      pmu_read_prop_next[i] = {pmu_read_prop[i][PmuCountersReadDelay-2:0], pmu_read_valid_th[i] & ~pmu_read_ready_next[i]};
    end
  end

  // Per-thread pmu_read_sel registers
  for (genvar ii = 0; ii < NrThreads; ii++) begin : gen_pmu_sel
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni)
        pmu_read_sel[ii] <= '0;
      else if (pmu_read_valid_th[ii])
        pmu_read_sel[ii] <= pmu_read_sel_next[ii];
    end
  end

  // pmu_read_prop — self-XOR gated (matches RST_XFF)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      pmu_read_prop <= '0;
    else
      pmu_read_prop <= pmu_read_prop_next;
  end

  // pmu_read_ready
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      pmu_read_ready <= '0;
    else
      pmu_read_ready <= pmu_read_ready_next;
  end

  // pmu_read_en_wb
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      pmu_read_en_wb <= 1'b0;
    else
      pmu_read_en_wb <= |pmu_read_valid_th;
  end

  assign io_rw_mem_ready = pmu_read_ready;

endmodule

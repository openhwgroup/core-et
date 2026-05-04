// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module intpipe_csr_pmu_read_interface
(
  input logic                                                     reset,
  input logic                                                     clock,

 // CSR Interface
  input logic                                                     io_rw_mem_valid,
  input logic                                                     io_rw_mem_thread_id,
  input logic [`CSR_REG_NR_SZ-1:0]                                io_rw_mem_addr,
  output logic [`CORE_NR_THREADS-1:0]                             io_rw_mem_ready,
  // Interface to PMU
  output logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE] pmu_read_sel,
  output logic                                                    pmu_read_en_wb
  
);

  logic [`PMU_COUNTERS_SELECT_BITS-2:0]                        pmu_read_addr, pmu_read_addr_um, pmu_read_addr_mm;
  logic                                                        pmu_read_valid, pmu_read_valid_um, pmu_read_valid_mm;
  logic [`CORE_NR_THREADS-1:0]                                 pmu_read_valid_th;
  logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_BITS-1:0]  pmu_read_sel_next;
  logic [`CORE_NR_THREADS-1:0]                                 pmu_read_current_sel;
  logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_READ_DELAY-1:0]   pmu_read_prop;
  logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_READ_DELAY-1:0]   pmu_read_prop_next;
  
  logic [`CORE_NR_THREADS-1:0]                                 pmu_read_ready;
  logic [`CORE_NR_THREADS-1:0]                                 pmu_read_ready_next;

  always_comb
  begin

    // Valid access if within the performance counters range
    pmu_read_valid_mm   =  io_rw_mem_valid && ((io_rw_mem_addr>=`CSR_REG_NR_SZ'(csr_ad_MHPMCOUNTER3)) && (io_rw_mem_addr<=`CSR_REG_NR_SZ'(csr_ad_MHPMCOUNTER8)));
    pmu_read_valid_um   =  io_rw_mem_valid && ((io_rw_mem_addr>=`CSR_REG_NR_SZ'(csr_ad_HPMCOUNTER3))  && (io_rw_mem_addr<=`CSR_REG_NR_SZ'(csr_ad_HPMCOUNTER8)));
    pmu_read_valid      =  pmu_read_valid_mm || pmu_read_valid_um;

    // Obtain register selection offset 
    pmu_read_addr_mm    = $bits(pmu_read_addr)'(io_rw_mem_addr - csr_ad_MHPMCOUNTER3);
    pmu_read_addr_um    = $bits(pmu_read_addr)'(io_rw_mem_addr - csr_ad_HPMCOUNTER3);
    pmu_read_addr       = (pmu_read_valid_mm) ? pmu_read_addr_mm : pmu_read_addr_um;
        
    // Per thread valid read. Must take into accout if an ongoing read happens for any other thread
    for (integer i=0; i<`CORE_NR_THREADS; i++)
    begin 
       pmu_read_valid_th[i] = pmu_read_valid && (io_rw_mem_thread_id == i);

      // reading to current sel
      pmu_read_current_sel[i] = pmu_read_valid && (io_rw_mem_thread_id == i) && (pmu_read_sel[i] == {pmu_read_addr,io_rw_mem_thread_id});

      // Address that will be used for reading
      pmu_read_sel_next[i]    = {pmu_read_addr,io_rw_mem_thread_id};
    end

    // Propagate read and generate a ready signal for every thread
    for (integer i=0; i<`CORE_NR_THREADS; i++)
    begin

      pmu_read_ready_next[i] = pmu_read_ready[i];
      if ( pmu_read_prop[i][`PMU_COUNTERS_READ_DELAY-1] )
        pmu_read_ready_next[i] = 1'b1;
      else if (pmu_read_ready[i] && pmu_read_current_sel[i])
        pmu_read_ready_next[i] = 1'b1;
      else if (pmu_read_valid_th[i] && !pmu_read_current_sel[i])
        pmu_read_ready_next[i] = 1'b0;

      pmu_read_prop_next[i]  = {pmu_read_prop[i][`PMU_COUNTERS_READ_DELAY-2:0], pmu_read_valid_th[i] & ~pmu_read_ready_next[i] };
    end
  end

  genvar ii;
  generate
    for(ii=0; ii<`CORE_NR_THREADS; ii++)
    begin
      //         CLK    RST    EN                     DOUT              DIN                    DEF
      `RST_EN_FF(clock, reset, pmu_read_valid_th[ii], pmu_read_sel[ii], pmu_read_sel_next[ii], {`PMU_COUNTERS_SELECT_BITS{1'b0}})
    end
  endgenerate

  //        CLK    RST    DOUT            DIN                  DEF
  `RST_XFF (clock, reset, pmu_read_prop,  pmu_read_prop_next,  '0                 )
  `RST_FF  (clock, reset, pmu_read_ready, pmu_read_ready_next, `CORE_NR_THREADS'b0)
  `RST_FF  (clock, reset, pmu_read_en_wb, |pmu_read_valid_th,  1'b0               )
  
  assign io_rw_mem_ready = pmu_read_ready;

endmodule

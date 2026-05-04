// Co-simulation testbench: intpipe_csr_pmu_read_interface (new) vs original.

`include "soc.vh"

module cosim_pmu_read_interface_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus
  input  logic        stim_io_rw_mem_valid,
  input  logic        stim_io_rw_mem_thread_id,
  input  logic [11:0] stim_io_rw_mem_addr,

  // New outputs
  output logic [1:0]  new_io_rw_mem_ready,
  output logic [3:0]  new_pmu_read_sel_0,
  output logic [3:0]  new_pmu_read_sel_1,
  output logic        new_pmu_read_en_wb,

  // Original outputs
  output logic [1:0]  orig_io_rw_mem_ready,
  output logic [3:0]  orig_pmu_read_sel_0,
  output logic [3:0]  orig_pmu_read_sel_1,
  output logic        orig_pmu_read_en_wb
);

  import minion_pkg::*;
  import intpipe_csr_pkg::*;

  // -- New module --
  logic [NrThreads-1:0][PmuCountersSelectBits-1:0] new_pmu_read_sel;

  intpipe_csr_pmu_read_interface u_new (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .io_rw_mem_valid     (stim_io_rw_mem_valid),
    .io_rw_mem_thread_id (stim_io_rw_mem_thread_id),
    .io_rw_mem_addr      (stim_io_rw_mem_addr),
    .io_rw_mem_ready     (new_io_rw_mem_ready),
    .pmu_read_sel        (new_pmu_read_sel),
    .pmu_read_en_wb      (new_pmu_read_en_wb)
  );
  assign new_pmu_read_sel_0 = new_pmu_read_sel[0];
  assign new_pmu_read_sel_1 = new_pmu_read_sel[1];

  // -- Original module --
  logic [`CORE_NR_THREADS-1:0][`PMU_COUNTERS_SELECT_RANGE] orig_pmu_read_sel;

  intpipe_csr_pmu_read_interface_orig u_orig (
    .clock               (clk_i),
    .reset               (~rst_ni),
    .io_rw_mem_valid     (stim_io_rw_mem_valid),
    .io_rw_mem_thread_id (stim_io_rw_mem_thread_id),
    .io_rw_mem_addr      (stim_io_rw_mem_addr),
    .io_rw_mem_ready     (orig_io_rw_mem_ready),
    .pmu_read_sel        (orig_pmu_read_sel),
    .pmu_read_en_wb      (orig_pmu_read_en_wb)
  );
  assign orig_pmu_read_sel_0 = orig_pmu_read_sel[0];
  assign orig_pmu_read_sel_1 = orig_pmu_read_sel[1];

endmodule

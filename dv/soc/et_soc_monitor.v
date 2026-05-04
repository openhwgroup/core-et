`include "rtl/inc/soc.vh"

`ifdef EVL_SIMULATION
   `ifndef ET_TESTEND_DPI
      `ifdef ENABLE_BEMU
         `ifndef DV_IOSHIRE_NOC_ONLY
            `define ET_TESTEND_DPI 1
         `endif // ifndef DV_IOSHIRE_NOC_ONLY
      `endif // ifdef ENABLE_BEMU
   `endif // ifndef ET_TESTEND_DPI
`endif // ifdef EVL_SIMULATION

`ifdef ET_TESTEND_DPI
   `ifndef ET_SYSTEM_TESTEND
      `define ET_SYSTEM_TESTEND 1
   `endif // ifndef ET_SYSTEM_TESTEND
`endif // ifdef ET_TESTEND_DPI

`ifdef ZEBU
   `ifndef ET_INCLUDE_TESTEND_TRIGGERS
      `define ET_INCLUDE_TESTEND_TRIGGERS 1
   `endif // ifndef ET_INCLUDE_TESTEND_TRIGGERS
`endif // ifdef ZEBU

module et_soc_monitor #(parameter int    NUM_MINIONS         = (`NUM_MINION_SHIRES * `NUM_NEIGH * `MIN_PER_N),
                        parameter [33:0] DV_MINSHIRE_GATESIM = {34{1'b0}},
                        parameter bit    DV_IOSHIRE_GATESIM  = 1'b0) (
      input  wire                   reset,
      input  wire                   clock,
      input  wire [NUM_MINIONS-1:0] minion_mask,
      input  wire                   io_minion_mask
   );

   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
      import evl_base_pkg::*;
   `endif // ifdef EVL_SIMULATION

   `ifdef ET_SYSTEM_TESTEND
      //
      // DPI calls
      //
      import "DPI-C" context function void test_end_system_enable(input int unsigned count);
      import "DPI-C" context function void test_end_system_done(input int unsigned fail, input int unsigned timeout, input longint cycle);
   `endif // `ifdef ET_SYSTEM_TESTEND


   //-----------------------------------------------------------------------------------------------
   //
   // Local Parameters
   //
   // MAX_THREAD:           This is the number of threads per core - 1.
   // MAX_SHIRE:            This is the number of Minion shires.  Note that arrays declared using
   //                       this parameter will typically have MAX_SHIRE + 1 elements (i.e., the
   //                       most significant element will be MAX_SHIRE, not MAX_SHIRE - 1).  This is
   //                       done to accommodate the IO shire's Minion.
   // MAX_MINION_PER_SHIRE: This is the number of Minions per Minion shire.
   // IO_SHIRE_GATESIM:     This parameter should be set to 1 for IO shire gate simulations.
   // MINION_GATESIM:       Bits in this parameter should be set to 1 for instances of Minion shire
   //                       gates.
   // MAX_SHIRE_MASK:       This is either MAX_SHIRE + 2 or 39, whichever is greater.
   // MAX_MINION_MASK:      This is either MAX_MINION_PER_SHIRE or 32, whichever is greater.
   //
   //-----------------------------------------------------------------------------------------------
   localparam int    MAX_THREAD           = (`CORE_NR_THREADS - 1);
   localparam int    MAX_SHIRE            = (NUM_MINIONS == 1)          ? 1          :
                                            (NUM_MINIONS == 8)          ? 1          : (`NUM_MINION_SHIRES);
   localparam int    MAX_MINION_PER_SHIRE = (NUM_MINIONS == 1)          ? 1          :
                                            (NUM_MINIONS == 8)          ? `MIN_PER_N : (`NUM_NEIGH * `MIN_PER_N);
   localparam bit    IOSHIRE_GATESIM      = (NUM_MINIONS == 1)          ? 1'b0       : DV_IOSHIRE_GATESIM;
   localparam [33:0] MINION_GATESIM       = (NUM_MINIONS == 1)          ? {34{1'b0}} : DV_MINSHIRE_GATESIM;
   localparam int    MAX_SHIRE_MASK       = (MAX_SHIRE < 37)            ? 39         : MAX_SHIRE + 2;
   localparam int    MAX_MINION_MASK      = (MAX_MINION_PER_SHIRE < 32) ? 32         : MAX_MINION_PER_SHIRE;


   //-----------------------------------------------------------------------------------------------
   //
   // Configuration Variables
   //
   // The following variables can be set by Zebu or via `et_value_plusargs calls.
   //
   //-----------------------------------------------------------------------------------------------
   reg                        ignore_io_minion;
   reg                        wfi_ends_test;
   reg                        ecall_ends_test;
   reg                        csr_stall_ends_test;
   reg  [MAX_SHIRE:0]         csr_stall_one_ends_test;
   reg  [7:0]                 csr_stall_done_count;
   reg  [31:0]                thread_timeout_cycles;
   reg  [MAX_SHIRE:0]         shire_is_rtl;
   reg  [MAX_SHIRE_MASK:0]    shire_config;
   reg                        shire_config_set;
   reg  [MAX_MINION_MASK-1:0] minion_disable = {(MAX_MINION_MASK){1'b0}};
   reg  [MAX_THREAD:0]        thread_disable = {(MAX_THREAD + 1){1'b0}};


   //-----------------------------------------------------------------------------------------------
   //
   // Status Variable
   //
   // The following variable can be watched by Zebu or internal code.
   //
   //-----------------------------------------------------------------------------------------------
   localparam int DONE_BIT    = 0;
   localparam int FAIL_BIT    = 1;
   localparam int TIMEOUT_BIT = 2;
   localparam int STATUS_MSB  = 2;

   bit [STATUS_MSB:0] sim_complete = {(STATUS_MSB + 1){1'b0}};


   //-----------------------------------------------------------------------------------------------
   //
   // Internal Wires - Remotely Set/Watched
   //
   // The following wires are remotely set by testend_monitor instances.
   //
   //-----------------------------------------------------------------------------------------------
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_valid;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_done;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_fail;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_timeout;
   wire [NUM_MINIONS:0]               hart_isolate;
   wire [NUM_MINIONS:0]               hart_reset_done;


   //-----------------------------------------------------------------------------------------------
   //
   // Internal Wires/Variables - Locally Set
   //
   // The following wires/variables are locally set by code within this module.
   //
   //-----------------------------------------------------------------------------------------------
   bit                                force_reset_n;
   bit  [63:0]                        cycle;
   reg  [`CSR_REG_NR_SZ-1:0]          mon_csr_wb_addr;
   reg                                any_active_r;
   reg  [STATUS_MSB:0]                sim_complete_r1;
   reg  [STATUS_MSB:0]                sim_complete_r2;
   wire [STATUS_MSB:0]                sim_complete_w;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_mask;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_valid_lcl;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_done_n_lcl;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_fail_lcl;
   wire [NUM_MINIONS:0][MAX_THREAD:0] hart_timeout_lcl;
   wire [NUM_MINIONS:0]               hart_isolate_lcl;
   wire [NUM_MINIONS:0]               hart_reset_done_lcl;
   wire [MAX_SHIRE_MASK:0]            shire_mask;
   wire [MAX_SHIRE:0]                 shire_inactive_lcl;
   wire                               io_minion_mask_lcl;
   wire                               all_active;
   wire                               any_active;
   wire                               any_fail;
   wire                               any_timeout;
   wire                               all_done;


   //-----------------------------------------------------------------------------------------------
   //
   // Trigger Wires
   //
   // The following wires are used solely for trigger mechanisms in the testend_monitor.
   //
   //-----------------------------------------------------------------------------------------------
   bit                                   clear_triggers;
   bit                                   instr_trigger;
   bit                                   pc_trigger;
   `ifdef ET_INCLUDE_TESTEND_TRIGGERS
      wire [NUM_MINIONS:0][MAX_THREAD:0] hart_instr_trigger;
      wire [NUM_MINIONS:0][MAX_THREAD:0] hart_pc_trigger;
      bit  [NUM_MINIONS:0][MAX_THREAD:0] hart_instr_trigger_r;
      bit  [NUM_MINIONS:0][MAX_THREAD:0] hart_pc_trigger_r;
   `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS


   //-----------------------------------------------------------------------------------------------
   //
   // Assign HART masks based on build information.  The incoming minion_mask and io_minion_mask
   // variables determine which Minions were built.
   //
   //-----------------------------------------------------------------------------------------------
   genvar ii, jj;
   generate
      for (ii = 0; ii < NUM_MINIONS; ii++) begin :g_hart_wires
         localparam int shire_select = (ii / MAX_MINION_PER_SHIRE);
         wire           minion_ignore_n;

         assign minion_ignore_n                  = (~minion_mask[ii]) & shire_is_rtl[shire_select];
         assign hart_mask[ii][MAX_THREAD:0]      = hart_valid[ii] & {(MAX_THREAD + 1){(~hart_isolate[ii]) & minion_ignore_n}};
         assign hart_valid_lcl[ii][MAX_THREAD:0] = hart_valid[ii] & {(MAX_THREAD + 1){minion_ignore_n}};
         assign hart_isolate_lcl[ii]             = minion_ignore_n & hart_isolate[ii];
         assign hart_reset_done_lcl[ii]          = minion_ignore_n & (hart_reset_done[ii] | hart_isolate[ii]);
      end
   endgenerate

   generate
      for (ii = 0; ii < MAX_SHIRE; ii++) begin :g_shire_wires
         wire [MAX_MINION_PER_SHIRE-1:0] minion_active;
         wire [MAX_MINION_PER_SHIRE-1:0] minion_ignore;

         for (jj = 0; jj < MAX_MINION_PER_SHIRE; jj++) begin :g_minion_wires
            assign minion_active[jj] = hart_reset_done_lcl[(ii * MAX_MINION_PER_SHIRE) + jj];
            assign minion_ignore[jj] = minion_mask[(ii * MAX_MINION_PER_SHIRE) + jj];
         end
         assign shire_mask[ii]         = (minion_ignore == {MAX_MINION_PER_SHIRE{1'b1}}) ? 1'b0 : shire_is_rtl[ii];
         assign shire_inactive_lcl[ii] = ~(|(minion_active)) & shire_mask[ii] & shire_config[ii];
      end
   endgenerate

   assign io_minion_mask_lcl                        = io_minion_mask | ignore_io_minion;
   assign hart_mask[NUM_MINIONS][MAX_THREAD:0]      = hart_valid[NUM_MINIONS] & {(MAX_THREAD + 1){~io_minion_mask_lcl}};
   assign hart_valid_lcl[NUM_MINIONS][MAX_THREAD:0] = hart_valid[NUM_MINIONS] & {(MAX_THREAD + 1){~io_minion_mask_lcl}};
   assign hart_isolate_lcl[NUM_MINIONS]             = 1'b0;
   assign hart_reset_done_lcl[NUM_MINIONS]          = ~io_minion_mask_lcl & hart_reset_done[NUM_MINIONS];
   assign shire_mask[MAX_SHIRE]                     = ~io_minion_mask_lcl;
   assign shire_mask[MAX_SHIRE_MASK:MAX_SHIRE+1]    = {(MAX_SHIRE_MASK-MAX_SHIRE){1'b0}};
   assign shire_inactive_lcl[MAX_SHIRE]             = ~io_minion_mask_lcl & (~hart_reset_done[NUM_MINIONS]);


   //-----------------------------------------------------------------------------------------------
   //
   // Assign termination information for all active HARTs.
   //
   //-----------------------------------------------------------------------------------------------
   assign hart_timeout_lcl = hart_mask & hart_timeout;
   assign hart_fail_lcl    = hart_mask & hart_fail;
   assign hart_done_n_lcl  = hart_mask & ~hart_done;

   assign all_active       = ~(|(shire_inactive_lcl)) & any_active;
   assign any_active       =   |(hart_mask);
   assign any_timeout      =   |(hart_timeout_lcl) & any_active;
   assign any_fail         =   |(hart_fail_lcl)    & any_active;
   assign all_done         = ~(|(hart_done_n_lcl)) & all_active;
   assign sim_complete_w   = { any_timeout, any_fail, (all_done | any_fail) };

   `ifdef EVL_SIMULATION
      //
      // The following assigns are included solely to eliminate VCS lint warnings.
      //
      assign hart_valid      = {((NUM_MINIONS+1) * (MAX_THREAD+1)){1'bz}};
      assign hart_done       = {((NUM_MINIONS+1) * (MAX_THREAD+1)){1'bz}};
      assign hart_fail       = {((NUM_MINIONS+1) * (MAX_THREAD+1)){1'bz}};
      assign hart_timeout    = {((NUM_MINIONS+1) * (MAX_THREAD+1)){1'bz}};
      assign hart_isolate    = {(NUM_MINIONS+1){1'bz}};
      assign hart_reset_done = {(NUM_MINIONS+1){1'bz}};
   `endif // ifdef EVL_SIMULATION


   //-----------------------------------------------------------------------------------------------
   //
   // Set up initial values for variables that may be used by testend_monitor instances.
   //
   //-----------------------------------------------------------------------------------------------
   `ifndef ZEBU
      `ifdef ET_TESTEND_DPI
         initial begin
            cycle           = {64{1'b0}};
            force_reset_n   = 1'b0;
            mon_csr_wb_addr = {`CSR_REG_NR_SZ{1'b0}};
            any_active_r    = 1'b0;
            sim_complete_r1 = {(STATUS_MSB + 1){1'b0}};
            sim_complete_r2 = {(STATUS_MSB + 1){1'b0}};
            if (`et_value_plusargs("WFI_ENDS_TEST=%0d", wfi_ends_test) == 0) begin
               wfi_ends_test = 1'b1;
            end
            if (`et_value_plusargs("ECALL_ENDS_TEST=%0d", ecall_ends_test) == 0) begin
               ecall_ends_test = 1'b1;
            end
            if (`et_value_plusargs("CSR_STALL_DONE_COUNT=%d", csr_stall_done_count) != 0) begin
               csr_stall_ends_test = 1'b1;
               if (`et_value_plusargs("CSR_STALL_ONCE=0x%x", csr_stall_one_ends_test) == 0) begin
                  csr_stall_one_ends_test = {(MAX_SHIRE+1){1'b0}};
               end
            end
            else begin
               csr_stall_ends_test     = 1'b0;
               csr_stall_done_count    = 8'h01;
               csr_stall_one_ends_test = {(MAX_SHIRE+1){1'b0}};
            end
            if (`et_value_plusargs("thread_timeout=%d", thread_timeout_cycles) == 0) begin
               thread_timeout_cycles = 60000;
            end

            //
            // If +SHIRE_MASK has been specified, use it.  If not, assume shire 0 is available.
            //
            shire_config_set = 1'b1;
            shire_is_rtl     = ~{ IOSHIRE_GATESIM, MINION_GATESIM[MAX_SHIRE-1:0] };
            if (`et_value_plusargs("SHIRE_MASK=0x%x", shire_config) == 0) begin
               shire_config     = { {(MAX_SHIRE_MASK-1){1'b0}}, 1'b1 };
               shire_config_set = 1'b0;
               ignore_io_minion = 1'b1;
            end
            else begin
               ignore_io_minion = ~(shire_config[MAX_SHIRE:MAX_SHIRE] & shire_is_rtl[MAX_SHIRE:MAX_SHIRE]);
            end

            `ifdef EVL_SIMULATION
               #0 `evl_log(UVM_LOW, VMOD_GLOBAL, "SOC(RTL)", $sformatf("Minion shire mask is 0x%x (RTL shires: 0x%x ... IO shire minion will%0s be ignored for completion status)", shire_config[MAX_SHIRE:0], shire_is_rtl, ((io_minion_mask_lcl == 1'b1) ? "" : " not")))
            `endif // ifdef EVL_SIMULATION

            #0 wait (sim_complete_r2 !== {(STATUS_MSB + 1){1'b0}});
            if (sim_complete_r2[FAIL_BIT:FAIL_BIT] === 1'b0) begin
               `ifdef EVL_SIMULATION
                  #0 `evl_log(UVM_LOW, VMOD_GLOBAL, "SOC(RTL)", $sformatf("all HARTs have completed the test"))
               `else // ifdef EVL_SIMULATION
                  #0 $display("SOC(RTL): all HARTs have completed the test");
               `endif // ifdef EVL_SIMULATION
            end
         end
      `else // ifdef ET_TESTEND_DPI
         initial begin
            cycle                   = {64{1'b0}};
            force_reset_n           = 1'b0;
            mon_csr_wb_addr         = {`CSR_REG_NR_SZ{1'b0}};
            any_active_r            = 1'b0;
            sim_complete_r1         = {(STATUS_MSB + 1){1'b0}};
            sim_complete_r2         = {(STATUS_MSB + 1){1'b0}};
            ignore_io_minion        = 1'b1;
            wfi_ends_test           = 1'b1;
            ecall_ends_test         = 1'b1;
            csr_stall_ends_test     = 1'b0;
            csr_stall_one_ends_test = {(MAX_SHIRE+1){1'b0}};
            csr_stall_done_count    = 8'h00;
            thread_timeout_cycles   = 60000;
            shire_is_rtl            = {(MAX_SHIRE+1){1'b1}};
            shire_config            = shire_mask;
            shire_config_set        = 1'b0;
         end
      `endif // !ifdef ET_TESTEND_DPI
   `endif // !ifdef ZEBU


   //-----------------------------------------------------------------------------------------------
   //
   // Capture completion information for all active HARTs.
   //
   //-----------------------------------------------------------------------------------------------
   bit [2:0] zebu_reset_n;
   bit       zebu_reset;

   wire unused_ok = &{ zebu_reset_n, zebu_reset };

   initial begin
      zebu_reset   = 1'b1;
      zebu_reset_n = 3'b000;
   end

   always @(posedge clock) begin
      if (zebu_reset_n == 3'b111) begin
         zebu_reset <= 1'b0;
      end
      else begin
         zebu_reset   <= 1'b1;
         zebu_reset_n <= zebu_reset_n + 3'b001;
      end
   end

   `ifdef ZEBU
      initial begin
         force_reset_n           = 1'b0;
         any_active_r            = 1'b0;
         sim_complete_r1         = {(STATUS_MSB + 1){1'b0}};
         sim_complete_r2         = {(STATUS_MSB + 1){1'b0}};
         sim_complete            = {(STATUS_MSB + 1){1'b0}};
         ignore_io_minion        = 1'b1;
         wfi_ends_test           = 1'b0;
         ecall_ends_test         = 1'b0;
         csr_stall_ends_test     = 1'b0;
         csr_stall_one_ends_test = {(MAX_SHIRE+1){1'b0}};
         csr_stall_done_count    = 8'h00;
         thread_timeout_cycles   = 1000000;
         shire_is_rtl            = {(MAX_SHIRE+1){1'b1}};
         shire_config            = shire_mask;
         shire_config_set        = 1'b0;
         minion_disable          = {(MAX_MINION_MASK){1'b0}};
         thread_disable          = {(MAX_THREAD + 1){1'b0}};
         cycle                   = 64'h0000000000000001;
      end

      always @(posedge clock) begin
         if ((reset == 1'b1) || (zebu_reset == 1'b1)) begin
            force_reset_n           <= 1'b1;
            any_active_r            <= 1'b0;
            sim_complete_r1         <= {(STATUS_MSB + 1){1'b0}};
            sim_complete_r2         <= {(STATUS_MSB + 1){1'b0}};
            sim_complete            <= {(STATUS_MSB + 1){1'b0}};
            ignore_io_minion        <= 1'b1;
            wfi_ends_test           <= 1'b0;
            ecall_ends_test         <= 1'b0;
            csr_stall_ends_test     <= 1'b0;
            csr_stall_one_ends_test <= {(MAX_SHIRE+1){1'b0}};
            csr_stall_done_count    <= 8'h00;
            thread_timeout_cycles   <= 1000000;
            shire_is_rtl            <= {(MAX_SHIRE+1){1'b1}};
            shire_config            <= (zebu_reset == 1'b1) ? shire_mask : shire_config;
            shire_config_set        <= 1'b0;
            minion_disable          <= minion_disable;
            thread_disable          <= thread_disable;
            clear_triggers          <= (cycle == 64'h0000000000000000) ? 1'b1 : 1'b0;
            instr_trigger           <= 1'b0;
            pc_trigger              <= 1'b0;
            `ifdef ET_INCLUDE_TESTEND_TRIGGERS
               hart_instr_trigger_r <= {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
               hart_pc_trigger_r    <= {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
            `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS
         end
         else begin
            `ifdef ET_SYSTEM_TESTEND
               if ((any_active == 1'b1) && (any_active_r == 1'b0)) begin
                  test_end_system_enable(1);
               end
               if ((sim_complete_r1 !== {(STATUS_MSB + 1){1'b0}}) && (sim_complete_r2 === {(STATUS_MSB + 1){1'b0}})) begin
                  test_end_system_done(int'(sim_complete_r1[FAIL_BIT:FAIL_BIT]), int'(sim_complete_r1[TIMEOUT_BIT:TIMEOUT_BIT]), cycle);
               end
            `endif // ifdef ET_SYSTEM_TESTEND
            any_active_r    <= any_active | any_active_r;
            sim_complete    <= sim_complete_r2;
            sim_complete_r2 <= sim_complete_r1;
            sim_complete_r1 <= sim_complete_w | sim_complete_r1;

            `ifdef ET_INCLUDE_TESTEND_TRIGGERS
               if (hart_instr_trigger_r != (hart_instr_trigger & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] })) begin
                  hart_instr_trigger_r <= hart_instr_trigger & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] };
                  instr_trigger        <= |{(~hart_instr_trigger_r & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] } & hart_instr_trigger)};
               end
               if (hart_pc_trigger_r != (hart_pc_trigger & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] })) begin
                  hart_pc_trigger_r <= hart_pc_trigger & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] };
                  pc_trigger        <= |{(~hart_pc_trigger_r & { {(MAX_THREAD + 1){~io_minion_mask}}, hart_mask[NUM_MINIONS-1:0] } & hart_pc_trigger)};
               end
            `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS

            //
            // The following code fragment is used simply to force Zebu to implement flip-flops.
            //
            if (cycle[63:62] == 2'b11) begin
               ignore_io_minion        <= ~ignore_io_minion;
               wfi_ends_test           <= ~wfi_ends_test;
               ecall_ends_test         <= ~ecall_ends_test;
               csr_stall_ends_test     <= ~csr_stall_ends_test;
               csr_stall_one_ends_test <= ~csr_stall_one_ends_test;
               csr_stall_done_count    <= ~csr_stall_done_count;
               thread_timeout_cycles   <= ~thread_timeout_cycles;
               shire_is_rtl            <= ~shire_is_rtl;
               shire_config            <= cycle[MAX_SHIRE_MASK:0] | shire_config;
               shire_config_set        <= ~shire_config_set;
               minion_disable          <= cycle[MAX_MINION_MASK-1:0] | minion_disable;
               thread_disable          <= cycle[MAX_THREAD:0] | thread_disable;
            end
            else begin
               ignore_io_minion        <= ignore_io_minion;
               wfi_ends_test           <= wfi_ends_test;
               ecall_ends_test         <= ecall_ends_test;
               csr_stall_ends_test     <= csr_stall_ends_test;
               csr_stall_one_ends_test <= csr_stall_one_ends_test;
               csr_stall_done_count    <= csr_stall_done_count;
               thread_timeout_cycles   <= thread_timeout_cycles;
               shire_is_rtl            <= shire_is_rtl;
               shire_config            <= shire_config & shire_mask;
               shire_config_set        <= shire_config_set;
               minion_disable          <= minion_disable;
               thread_disable          <= thread_disable;
            end
         end

         //
         // The following code fragment is used simply to force Zebu to implement flip-flops.
         //
         if (cycle[63:62] == 2'b11) begin
            cycle <= cycle | { {(64-MAX_SHIRE_MASK){1'b0}}, shire_config } |
                             { {(63-MAX_THREAD){1'b0}}, thread_disable } |
                             { {(63-MAX_MINION_MASK){1'b0}}, minion_disable };
         end
         else begin
            if (cycle[63:63] == 1'b1) begin
               cycle <= 64'h0000000000000000;
            end
            else begin
               cycle <= cycle + 64'h0000000000000001;
            end
         end
      end
   `else // `ifdef ZEBU
      always @(posedge clock) begin
         if ((reset !== 1'b0) || (force_reset_n !== 1'b1)) begin
            any_active_r    <= 1'b0;
            sim_complete_r1 <= {(STATUS_MSB + 1){1'b0}};
            sim_complete_r2 <= {(STATUS_MSB + 1){1'b0}};
            sim_complete    <= {(STATUS_MSB + 1){1'b0}};
            clear_triggers  <= (cycle == 64'h0000000000000000) ? 1'b1 : 1'b0;
            instr_trigger   <= 1'b0;
            pc_trigger      <= 1'b0;
            if ((shire_config_set == 1'b0) && (shire_mask[MAX_SHIRE:0] !== {(MAX_SHIRE+1){1'b0}})) begin
               shire_config <= shire_mask;
            end
            if (reset === 1'b1) begin
               force_reset_n <= 1'b1;
            end
            `ifdef ET_INCLUDE_TESTEND_TRIGGERS
               hart_instr_trigger_r <= {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
               hart_pc_trigger_r    <= {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
            `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS
         end
         else begin
            `ifdef ET_SYSTEM_TESTEND
               if ((any_active == 1'b1) && (any_active_r == 1'b0)) begin
                  test_end_system_enable(1);
               end
               if ((sim_complete_r1 !== {(STATUS_MSB + 1){1'b0}}) && (sim_complete_r2 === {(STATUS_MSB + 1){1'b0}})) begin
                  test_end_system_done(int'(sim_complete_r1[FAIL_BIT:FAIL_BIT]), int'(sim_complete_r1[TIMEOUT_BIT:TIMEOUT_BIT]), cycle);
               end
            `endif // ifdef ET_SYSTEM_TESTEND
            any_active_r    <= any_active | any_active_r;
            sim_complete    <= sim_complete_r2;
            sim_complete_r2 <= sim_complete_r1;
            sim_complete_r1 <= sim_complete_w | sim_complete_r1;
         end
         cycle          <= cycle + 64'h0000000000000001;
         minion_disable <= {(MAX_MINION_MASK){1'b0}};
         thread_disable <= {(MAX_THREAD + 1){1'b0}};
      end
   `endif // !ifdef ZEBU


   //-----------------------------------------------------------------------------------------------
   //
   // Perform DPI calls if they are enabled.
   //
   //-----------------------------------------------------------------------------------------------
   `ifndef ZEBU
      `ifdef ET_TESTEND_DPI
         //
         // DPI calls
         //
         import "DPI-C" context function void test_end_monitor_power(input int unsigned id, input int unsigned power_off);
         import "DPI-C" context function void test_end_monitor_done(input int unsigned id, input int unsigned thread_id, input longint cycle);
         import "DPI-C" context function void test_end_monitor_fail(input int unsigned id, input int unsigned thread_id, input longint cycle);
         import "DPI-C" context function void test_end_monitor_timeout(input int unsigned id, input int unsigned thread_id, input longint cycle);
         import "DPI-C" context function void test_end_monitor_update_enabled(input int unsigned id, input int unsigned enabled);

         reg  [NUM_MINIONS:0]               hart_isolate_r = {(NUM_MINIONS+1){1'b0}};
         reg  [NUM_MINIONS:0][MAX_THREAD:0] hart_valid_r   = {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
         reg  [NUM_MINIONS:0][MAX_THREAD:0] hart_done_r    = {((NUM_MINIONS+1)*(MAX_THREAD+1)){1'b0}};
         wire [NUM_MINIONS:0][MAX_THREAD:0] hart_done_w;

         assign hart_done_w = hart_mask & hart_done;

         always @(posedge clock) begin
            if ((reset !== 1'b0) || (force_reset_n !== 1'b1)) begin
               hart_isolate_r <= {(NUM_MINIONS + 1){1'b0}};
               hart_valid_r   <= {((NUM_MINIONS + 1) * (MAX_THREAD + 1)){1'b0}};
               hart_done_r    <= {((NUM_MINIONS + 1) * (MAX_THREAD + 1)){1'b0}};
            end
            else begin
               if (hart_valid_r !== hart_valid_lcl) begin
                  for (int ii = 0; ii <= NUM_MINIONS; ii++) begin
                     if (hart_valid_r[ii] !== hart_valid_lcl[ii]) begin
                        test_end_monitor_update_enabled(ii, int'(hart_valid_lcl[ii]));
                     end
                  end
                  hart_valid_r <= hart_valid_lcl;
               end
               if (hart_isolate_r !== hart_isolate_lcl) begin
                  for (int ii = 0; ii <= NUM_MINIONS; ii++) begin
                     if (hart_isolate_r[ii] !== hart_isolate_lcl[ii]) begin
                        test_end_monitor_power(ii, int'(hart_isolate_lcl[ii]));
                     end
                  end
                  hart_isolate_r <= hart_isolate_lcl;
               end
               if (hart_done_r !== hart_done_w) begin
                  for (int ii = 0; ii <= NUM_MINIONS; ii++) begin
                     for (int jj = 0; jj <= MAX_THREAD; jj++) begin
                        if (hart_done_r[ii][jj] !== hart_done_w[ii][jj]) begin
                           if (hart_timeout[ii][jj] === 1'b1) begin
                              `ifdef EVL_SIMULATION
                                 evl_global_attrs::cosim_end(-1, ii, jj, 1'b1);
                              `endif // ifdef EVL_SIMULATION
                              test_end_monitor_timeout(ii, jj, cycle);
                           end
                           else if (hart_fail[ii][jj] !== 1'b0) begin
                              `ifdef EVL_SIMULATION
                                 evl_global_attrs::cosim_end(1, ii, jj, 1'b1);
                              `endif // ifdef EVL_SIMULATION
                              test_end_monitor_fail(ii, jj, cycle);
                           end
                        end
                     end
                  end
                  for (int ii = 0; ii <= NUM_MINIONS; ii++) begin
                     for (int jj = 0; jj <= MAX_THREAD; jj++) begin
                        if (hart_done_r[ii][jj] !== hart_done_w[ii][jj]) begin
                           if ((hart_fail[ii][jj] === 1'b0) && (hart_done_w[ii][jj] === 1'b1)) begin
                              test_end_monitor_done(ii, jj, cycle);
                           end
                        end
                     end
                  end
                  hart_done_r <= hart_done_w;
               end

               //
               // Wait for a change in monitored signals before continuing.  This will prevent
               // unnecessary comparisons.  Note that this is not a synthesizable construct for
               // Zebu.
               //
               @(reset or hart_valid_lcl or hart_isolate_lcl or hart_done_w);
               #0;
            end
         end
      `endif // ifdef ET_TESTEND_DPI
   `endif // ifdef ZEBU
endmodule

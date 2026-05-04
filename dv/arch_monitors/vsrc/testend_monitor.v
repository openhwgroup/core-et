`include "soc_defines.vh"
`include "monitor_defines.vh"

 `ifndef SOC_MONITOR
 `define SOC_MONITOR soc_monitor
`endif

`ifndef evl_sim_complete
   `define evl_sim_complete 1'b0
`endif // ifndef evl_sim_complete

`ifdef EVL_SIMULATION
   `ifndef ET_TESTEND_DPI
      `ifdef ENABLE_BEMU
         `define ET_TESTEND_DPI 1
      `endif // ifdef ENABLE_BEMU
   `endif // ifndef ET_TESTEND_DPI
`endif // ifdef EVL_SIMULATION

`ifdef ZEBU
   `ifndef ET_INCLUDE_TESTEND_TRIGGERS
      `define ET_INCLUDE_TESTEND_TRIGGERS 1
   `endif // ifndef ET_INCLUDE_TESTEND_TRIGGERS
`endif // ifdef ZEBU

`ifdef ET_INCLUDE_TESTEND_TRIGGERS
   `ifndef ET_USE_SHIRE_MONITOR_PIPE
      `define ET_USE_SHIRE_MONITOR_PIPE 1
   `endif // ifndef ET_USE_SHIRE_MONITOR_PIPE
`endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS

module testend_monitor #(parameter int SHIRE_ID    = 0,
                         parameter int NEIGH_ID    = 0,
                         parameter int MINION_ID   = 0,
                         parameter bit PREVENT_DPI = 1'b0)
(
    input                         reset,
    input                         iso_enable,
    input                         clock,
    input  [`NUM_SHIRE_IDS_R]     shire_id,
    input  [`MIN_PER_S_R]         shire_min_id,
    input  [`CORE_NR_THREADS-1:0] enabled,
    output [63:0]                 soc_cycle
);

   localparam int FIXED_ONE       = 32'h00000001;
   localparam int MAX_MINION      = (`NUM_NEIGH * `MIN_PER_N);
   localparam int GLOBAL_ID       = (SHIRE_ID > `NUM_MINION_SHIRES) ? (`NUM_MINION_SHIRES * `NUM_NEIGH * `MIN_PER_N) :
                                                                      (((SHIRE_ID * `NUM_NEIGH) + NEIGH_ID) * `MIN_PER_N) + MINION_ID;
   localparam int GLOBAL_SHIRE_ID = (SHIRE_ID > `NUM_MINION_SHIRES) ? (`NUM_MINION_SHIRES) : SHIRE_ID;

   //
   // 1-hot encoding for instructions
   //
   localparam int INST_ECALL        = 0;
   localparam int INST_WFI          = 1;
   localparam int INST_CSR_STALL    = 2;
   localparam int INST_BOOT_START   = 3;
   localparam int INST_BOOT_END     = 4;
   localparam int INST_CONFIG_START = 5;
   localparam int INST_CONFIG_END   = 6;
   localparam int INST_MAIN_START   = 7;
   localparam int INST_MAIN_END     = 8;
   localparam int INST_PASS         = 9;
   localparam int INST_FAIL         = 10;
   localparam int INST_ONE_HOT_MSB  = 10;

   bit                         force_reset     = 1'b1;
   bit  [`CSR_REG_NR_SZ-1:0]   mon_csr_wb_addr = {(`CSR_REG_NR_SZ){1'b0}};
   bit                         isolate         = 1'b0;
   bit                         reset_done      = 1'b0;
   wire                        reset_int;             // Internal reset (used to tolerate initial clock Xs)
   wire [INST_ONE_HOT_MSB:0]   inst_decode;           // One-hot decode of a graduating instruction
   wire                        inst_is_ecall;         // Graduating instruction is an ECALL
   wire                        inst_is_wfi;           // Graduating instruction is a WFI
   wire                        inst_is_csr_stall;     // Graduating instruction is a CSR stall
   wire                        inst_is_boot_start;    // Graduating instruction is a start-of-boot
   wire                        inst_is_boot_end;      // Graduating instruction is an end-of-boot
   wire                        inst_is_config_start;  // Graduating instruction is a start-of-configure phase
   wire                        inst_is_config_end;    // Graduating instruction is an end-of-configure phase
   wire                        inst_is_main_start;    // Graduating instruction is a start-of-main test phase
   wire                        inst_is_main_end;      // Graduating instruction is an end-of-main test phase
   wire                        inst_is_pass;          // Graduating instruction is a pass
   wire                        inst_is_fail;          // Graduating instruction is a fail
   wire                        inst_is_done;
   wire                        wb_valid;
   wire                        wb_thread_id;
   wire [`PC_RANGE_EXT]        wb_pc;
   wire                        soc_wfi_ends_test;
   wire                        soc_ecall_ends_test;
   wire                        soc_csr_stall_ends_test;
   wire [7:0]                  soc_csr_stall_done_count;
   wire                        soc_csr_stall_one_ends_test;
   wire [31:0]                 soc_thread_timeout_cycles;
   wire                        soc_minion_enable;
   wire [`CORE_NR_THREADS-1:0] soc_thread_enable;

   //
   // Save instruction bits inside the monitor (the actual ones might be clock gated and not all
   // bits are propagated through the pipe)
   //
   reg  [`INST_SIZE-1:0]       ex_inst;
   reg  [`INST_SIZE-1:0]       tag_inst;
   reg  [`INST_SIZE-1:0]       mem_inst;
   reg  [`INST_SIZE-1:0]       wb_inst;
   reg  [`CORE_NR_THREADS-1:0] thread_valid;      // Per-thread information passed to soc_monitor when a graduation first occurs
   reg  [`CORE_NR_THREADS-1:0] thread_done;       // Per-thread information passed to soc_monitor when a diag passes
   reg  [`CORE_NR_THREADS-1:0] thread_fail;       // Per-thread information passed to soc_monitor when a diag fails
   reg  [`CORE_NR_THREADS-1:0] thread_timeout;    // Per-thread information passed to soc_monitor when a diag fails with a timeout
   reg  [`CORE_NR_THREADS-1:0] thread_in_wfi;     // Per-thread information indicating it is waiting for an interrupt
   reg  [`CORE_NR_THREADS-1:0] thread_csr_stall;  // Per-thread information indicating the CSR stall count has been reached
   reg  [`CORE_NR_THREADS-1:0] thread_enabled = {(`CORE_NR_THREADS){1'b0}};


   //
   // Local Assignments
   //
   assign reset_int   = reset | force_reset;
   assign inst_decode = (reset_int == 1'b1)        ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b0 })                      :
                        (wb_valid != 1'b1)         ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b0 })                      :
                        (wb_inst == 32'h0000_0073) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_ECALL)        :
                        (wb_inst == 32'h1050_0073) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_WFI)          :
                        (wb_inst == 32'h8220_1073) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_CSR_STALL)    :
                        (wb_inst == 32'h7f00_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_BOOT_START)   :
                        (wb_inst == 32'h7f10_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_BOOT_END)     :
                        (wb_inst == 32'h7f20_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_CONFIG_START) :
                        (wb_inst == 32'h7f30_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_CONFIG_END)   :
                        (wb_inst == 32'h7f40_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_MAIN_START)   :
                        (wb_inst == 32'h7f50_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_MAIN_END)     :
                        (wb_inst == 32'h7fe0_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_PASS)         :
                        (wb_inst == 32'h7ff0_2013) ? ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b1 } << INST_FAIL)         :
                                                     ({ {(INST_ONE_HOT_MSB - 1){1'b0}}, 1'b0 });

   assign wb_valid             = `MINION_CORE.wb_valid;
   assign wb_thread_id         = `MINION_CORE.wb_thread_id;
   assign wb_pc                = `MINION_CORE.wb_reg_pc;
   assign inst_is_ecall        = inst_decode[INST_ECALL:INST_ECALL];
   assign inst_is_wfi          = inst_decode[INST_WFI:INST_WFI];
   assign inst_is_csr_stall    = inst_decode[INST_CSR_STALL:INST_CSR_STALL];
   assign inst_is_boot_start   = inst_decode[INST_BOOT_START:INST_BOOT_START];
   assign inst_is_boot_end     = inst_decode[INST_BOOT_END:INST_BOOT_END];
   assign inst_is_config_start = inst_decode[INST_CONFIG_START:INST_CONFIG_START];
   assign inst_is_config_end   = inst_decode[INST_CONFIG_END:INST_CONFIG_END];
   assign inst_is_main_start   = inst_decode[INST_MAIN_START:INST_MAIN_START];
   assign inst_is_main_end     = inst_decode[INST_MAIN_END:INST_MAIN_END];
   assign inst_is_pass         = inst_decode[INST_PASS:INST_PASS];
   assign inst_is_fail         = inst_decode[INST_FAIL:INST_FAIL];
   assign inst_is_done         = inst_is_pass |
                                 (inst_is_wfi   & soc_wfi_ends_test) |
                                 (inst_is_ecall & soc_ecall_ends_test);

   `ifdef EVL_SIMULATION
      //
      // Control the internal isolate variable.  The input iso_enable will eventually go to X when
      // power for this Minion is turned off.
      //
      always begin
         #0 wait (iso_enable === 1'b1);
         isolate <= 1'b1;
         #0 wait (iso_enable === 1'b0);
         isolate <= 1'b0;
      end
   `endif // ifdef EVL_SIMULATION


   `ifdef ET_INCLUDE_TESTEND_TRIGGERS
      //
      // Instruction Triggering
      //
      bit  [`CORE_NR_THREADS-1:0] instr_trigger_match = {(`CORE_NR_THREADS){1'b0}};
      bit  [`INST_SIZE:0]         instr_trigger       = {(`INST_SIZE+1){1'b0}};
      bit  [`CORE_NR_THREADS-1:0] pc_trigger_match    = {(`CORE_NR_THREADS){1'b0}};
      bit  [`PC_SIZE_EXT:0]       pc_trigger          = {(`PC_SIZE_EXT+1){1'b0}};

      wire                        soc_clear_triggers;

      always @(posedge clock) begin
         if (soc_clear_triggers == 1'b1) begin
            instr_trigger_match <= {(`CORE_NR_THREADS){1'b0}};
            instr_trigger       <= {(`INST_SIZE+1){1'b0}};
            pc_trigger_match    <= {(`CORE_NR_THREADS){1'b0}};
            pc_trigger          <= {(`PC_SIZE_EXT+1){1'b0}};
         end
         else if ((isolate === 1'b0) && (reset_int === 1'b0) && (wb_valid === 1'b1)) begin
            //
            // If there is a PC trigger match, assert a trigger match and clear the trigger.
            //
            if (pc_trigger == { 1'b1, wb_pc }) begin
               pc_trigger_match <= FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id;
               pc_trigger       <= pc_trigger ^ {(`PC_SIZE_EXT){1'b1}};
            end
            else begin
               pc_trigger_match <= {(`CORE_NR_THREADS){1'b0}};
               pc_trigger       <= pc_trigger;
            end
            //
            //
            // If there is an instruction trigger match, assert a trigger match and clear the
            // trigger.
            //
            if (instr_trigger == { 1'b1, wb_inst }) begin
               instr_trigger_match <= FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id;
               instr_trigger       <= instr_trigger ^ {(`INST_SIZE){1'b1}};
            end
            else begin
               instr_trigger_match <= {(`CORE_NR_THREADS){1'b0}};
               instr_trigger       <= instr_trigger;
            end
         end
         else begin
            instr_trigger_match <= {(`CORE_NR_THREADS){1'b0}};
            instr_trigger       <= instr_trigger;
            pc_trigger_match    <= {(`CORE_NR_THREADS){1'b0}};
            pc_trigger          <= pc_trigger;
         end
      end
   `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS


   `ifdef ET_USE_SHIRE_MONITOR_PIPE

      //
      // Let the shire monitor pipeline know if this Minion's threads are enabled/have passed/have failed.
      //
      assign shire_monitor_pipe.hart_valid[GLOBAL_ID][`CORE_NR_THREADS-1:0]   = thread_valid;
      assign shire_monitor_pipe.hart_done[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = thread_done | thread_fail | thread_timeout | thread_csr_stall;
      assign shire_monitor_pipe.hart_fail[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = thread_fail | thread_timeout;
      assign shire_monitor_pipe.hart_timeout[GLOBAL_ID][`CORE_NR_THREADS-1:0] = thread_timeout;
      assign shire_monitor_pipe.hart_isolate[GLOBAL_ID]                       = isolate;
      assign shire_monitor_pipe.hart_reset_done[GLOBAL_ID]                    = reset_done;

      assign soc_wfi_ends_test           = shire_monitor_pipe.wfi_ends_test;
      assign soc_ecall_ends_test         = shire_monitor_pipe.ecall_ends_test;
      assign soc_csr_stall_ends_test     = shire_monitor_pipe.csr_stall_ends_test;
      assign soc_csr_stall_one_ends_test = shire_monitor_pipe.csr_stall_one_ends_test;
      assign soc_csr_stall_done_count    = shire_monitor_pipe.csr_stall_done_count;
      assign soc_thread_timeout_cycles   = shire_monitor_pipe.thread_timeout_cycles;
      assign soc_cycle                   = shire_monitor_pipe.cycle;
      assign soc_minion_enable           = (GLOBAL_SHIRE_ID >= `NUM_MINION_SHIRES) ? ~shire_monitor_pipe.minion_disable[0] :
                                                                                     ~shire_monitor_pipe.minion_disable[GLOBAL_ID[4:0]];
      assign soc_thread_enable           = ~shire_monitor_pipe.thread_disable & {(`CORE_NR_THREADS){soc_minion_enable}};

      `ifdef ET_INCLUDE_TESTEND_TRIGGERS
         assign shire_monitor_pipe.hart_instr_trigger[GLOBAL_ID][`CORE_NR_THREADS-1:0] = instr_trigger_match;
         assign shire_monitor_pipe.hart_pc_trigger[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = pc_trigger_match;

         assign soc_clear_triggers = shire_monitor_pipe.clear_triggers;
      `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS

   `else // ifdef ET_USE_SHIRE_MONITOR_PIPE

      //
      // Let the SOC monitor know if this Minion's threads are enabled/have passed/have failed.
      //
      assign `SOC_MONITOR.hart_valid[GLOBAL_ID][`CORE_NR_THREADS-1:0]   = thread_valid;
      assign `SOC_MONITOR.hart_done[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = thread_done | thread_fail | thread_timeout | thread_csr_stall;
      assign `SOC_MONITOR.hart_fail[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = thread_fail | thread_timeout;
      assign `SOC_MONITOR.hart_timeout[GLOBAL_ID][`CORE_NR_THREADS-1:0] = thread_timeout;
      assign `SOC_MONITOR.hart_isolate[GLOBAL_ID]                       = isolate;
      assign `SOC_MONITOR.hart_reset_done[GLOBAL_ID]                    = reset_done;

      assign soc_wfi_ends_test           = `SOC_MONITOR.wfi_ends_test;
      assign soc_ecall_ends_test         = `SOC_MONITOR.ecall_ends_test;
      assign soc_csr_stall_ends_test     = `SOC_MONITOR.csr_stall_ends_test;
      assign soc_csr_stall_one_ends_test = `SOC_MONITOR.csr_stall_one_ends_test[GLOBAL_SHIRE_ID];
      assign soc_csr_stall_done_count    = `SOC_MONITOR.csr_stall_done_count;
      assign soc_thread_timeout_cycles   = `SOC_MONITOR.thread_timeout_cycles;
      assign soc_cycle                   = `SOC_MONITOR.cycle;
      assign soc_minion_enable           = ~`SOC_MONITOR.minion_disable[GLOBAL_ID[4:0]];
      assign soc_thread_enable           = ~`SOC_MONITOR.thread_disable & {(`CORE_NR_THREADS){soc_minion_enable}};

      `ifdef ET_INCLUDE_TESTEND_TRIGGERS
         assign `SOC_MONITOR.hart_instr_trigger[GLOBAL_ID][`CORE_NR_THREADS-1:0] = instr_trigger_match;
         assign `SOC_MONITOR.hart_pc_trigger[GLOBAL_ID][`CORE_NR_THREADS-1:0]    = pc_trigger_match;

         assign soc_clear_triggers = `SOC_MONITOR.clear_triggers;
      `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS

   `endif // ifdef ET_USE_SHIRE_MONITOR_PIPE


   //
   // Instruction Retirement
   //
   always @(posedge clock) begin
      if ((reset == 1'b1) && (force_reset == 1'b1)) begin
         //
         // This looks goofy, but it prevents a VCS lint warning of unused wires.  The result of
         // this will always be 0.
         //
         force_reset <= &{ inst_is_ecall,
                           inst_is_wfi,
                           inst_is_csr_stall,
                           inst_is_boot_start,
                           inst_is_boot_end,
                           inst_is_config_start,
                           inst_is_config_end,
                           inst_is_main_start,
                           inst_is_main_end,
                           inst_is_pass,
                           inst_is_fail,
                           iso_enable,
                           shire_id,
                           shire_min_id,
                           ~force_reset };
      end
      if (isolate !== 1'b0) begin
         ex_inst       <= {(`INST_SIZE){1'b0}};
         tag_inst      <= {(`INST_SIZE){1'b0}};
         mem_inst      <= {(`INST_SIZE){1'b0}};
         wb_inst       <= {(`INST_SIZE){1'b0}};
         thread_valid  <= {(`CORE_NR_THREADS){1'b0}};
         thread_done   <= {(`CORE_NR_THREADS){1'b0}};
         thread_fail   <= {(`CORE_NR_THREADS){1'b0}};
         thread_in_wfi <= {(`CORE_NR_THREADS){1'b0}};
         reset_done    <= reset_done;
      end
      else if (reset_int !== 1'b0) begin
         reset_done    <= 1'b0;
         ex_inst       <= {(`INST_SIZE){1'b0}};
         tag_inst      <= {(`INST_SIZE){1'b0}};
         mem_inst      <= {(`INST_SIZE){1'b0}};
         wb_inst       <= {(`INST_SIZE){1'b0}};
         thread_valid  <= {(`CORE_NR_THREADS){1'b0}};
         thread_done   <= {(`CORE_NR_THREADS){1'b0}};
         thread_fail   <= {(`CORE_NR_THREADS){1'b0}};
         thread_in_wfi <= {(`CORE_NR_THREADS){1'b0}};
      end
      else begin
         reset_done <= 1'b1;
         if (wb_valid !== 1'b1) begin
            for (int ii = 0; ii < `CORE_NR_THREADS; ii++) begin
               if ((thread_enabled[ii] === 1'b0) || (enabled[ii] === 1'b0)) begin
                  thread_valid[ii] <= 1'b0;
               end
               else if ((thread_enabled[ii] === 1'b1) && (enabled[ii] === 1'b1)) begin
                  thread_valid[ii] <= 1'b1;
               end
            end
         end
         else begin
            thread_valid <= thread_enabled & (thread_valid | enabled | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id));
            if (inst_is_fail == 1'b1) begin
               thread_fail <= thread_fail | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id);
            end
            if (inst_is_done == 1'b1) begin
               thread_done <= thread_done | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id);
            end
            if ((`MINION_CORE.csr_file.io_rw_wb_cmd == csr_cmd_W) && (mon_csr_wb_addr == `REG_VALIDATION0)) begin
               if (`MINION_CORE.csr_file.io_rw_wb_wdata[31:0] == `EVL_THREAD_PASS) begin
                  thread_done <= thread_done | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id);
               end
               else if (`MINION_CORE.csr_file.io_rw_wb_wdata[31:0] == `EVL_THREAD_FAIL) begin
                  thread_fail <= thread_fail | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id);
               end
            end
            if (inst_is_wfi == 1'b1) begin
               thread_in_wfi <= thread_in_wfi | (FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id);
            end
            else begin
               thread_in_wfi <= thread_in_wfi & (~(FIXED_ONE[(`CORE_NR_THREADS-1):0] << wb_thread_id));
            end
            `ifdef EVL_SIMULATION
               `evl_instr_trigger(wb_pc, wb_inst, (GLOBAL_ID << 1) | wb_thread_id)
            `endif // ifdef EVL_SIMULATION
         end

         //
         // Pipeline the instruction.
         //
         if (`MINION_CORE.mem_pc_valid == 1'b1) begin
            wb_inst  <= mem_inst;
         end
         if (`MINION_CORE.tag_pc_valid == 1'b1) begin
            mem_inst <= tag_inst;
         end
         if ((`MINION_CORE.ex_pc_valid | `MINION_CORE.gsc_pc_valid) == 1'b1) begin
            tag_inst <= ex_inst;
         end
         if (`MINION_CORE.id_inst_en == 1'b1) begin
            ex_inst  <= `MINION_CORE.id_inst_bits;
         end
         mon_csr_wb_addr <= `MINION_CORE.csr_file.io_rw_mem_addr;
      end
      thread_enabled <= soc_thread_enable;
   end


   //
   // Per-thread Timeout and CSR Stall
   //
   genvar thread;
   generate
      for (thread = 0; thread < `CORE_NR_THREADS; thread++) begin :timeout
         reg  [31:0] timeout_count;
         reg  [7:0]  csr_stall_count;
         wire        thread_fcc_wait;
         wire        thread_excl_mode;
         wire        thread_inactive;

         assign thread_fcc_wait  = (`MINION_CSR.csr_replay.csr_replay_stall[thread][CSR_REPLAY_FCC1:CSR_REPLAY_FCC0] == 0) ? 1'b0 : 1'b1;
         assign thread_excl_mode = ((`MINION_CORE.csr_excl_mode == 1'b1) && (`MINION_CORE.csr_excl_mode_sel != thread)) ? 1'b1 : 1'b0;
         assign thread_inactive  = ((~thread_enabled[thread]            |       // Thread is completely disabled
                                     ~minion.enabled[thread]            |       // Thread is disabled
                                     thread_in_wfi[thread]              |       // Waiting for an interrupt
                                     `MINION_CORE.id_csr_stall[thread]  |       // CSR stalled
                                     `MINION_CORE.in_debug_mode[thread] |       // In debug mode
                                     thread_fcc_wait                    |       // Waiting for an FCC credit
                                     thread_excl_mode) === 1'b1) ? 1'b1 : 1'b0; // Other thread is in exclusive mode

         always @(posedge clock) begin
            if ((reset_int | isolate) == 1'b1) begin
               thread_timeout[thread]   <= 1'b0;
               thread_csr_stall[thread] <= 1'b0;
               timeout_count            <= {32{1'b0}};
               csr_stall_count          <= {8{1'b0}};
            end
            else begin
               if ((wb_valid === 1'b1) && (wb_thread_id === thread)) begin
                  timeout_count <= {32{1'b0}};
                  if ((inst_is_csr_stall === 1'b1) && (soc_csr_stall_ends_test === 1'b1)) begin
                     if (((csr_stall_count + 8'h01) === soc_csr_stall_done_count) || (soc_csr_stall_one_ends_test === 1'b1)) begin
                        thread_csr_stall[thread] <= 1'b1;
                     end
                     else begin
                        csr_stall_count <= csr_stall_count + 8'h01;
                     end
                  end
               end
               else if (thread_inactive === 1'b1) begin
                  timeout_count <= {32{1'b0}};
               end
               else if ((thread_timeout[thread] === 1'b0) && (thread_done[thread] === 1'b0)) begin
                  if ((timeout_count >= soc_thread_timeout_cycles) && (soc_thread_timeout_cycles != 32'h00000000)) begin
                     thread_timeout[thread] <= 1'b1;
                  end
                  else begin
                     timeout_count <= timeout_count + 32'h00000001;
                  end
               end
            end
         end
      end
   endgenerate


   //
   // The following code currently will not be included in a Zebu compile.
   //
   `ifdef ET_TESTEND_DPI
      generate
         if (PREVENT_DPI == 1'b0) begin :testend_monitor
            //
            // DPI calls
            //
            import "DPI-C" context function void test_end_monitor_update_wfi(input int unsigned id, input int unsigned wfi);

            //
            // Pass stall nformation to C code and pass wait-for-interrupt information to C code.
            //
            bit [`CORE_NR_THREADS-1:0][`CORE_NR_MSG_PORTS-1:0] msg_port_stall;
            bit [`CORE_NR_THREADS-1:0]                         last_wfi;

            always @(posedge clock) begin
               if ((reset !== 1'b0) || (force_reset !== 1'b0)) begin
                  msg_port_stall <= {(`CORE_NR_THREADS * `CORE_NR_MSG_PORTS){1'b0}};
                  last_wfi       <= {(`CORE_NR_THREADS){1'b0}};
               end
               else if (reset_int === 1'b0) begin
                  if (thread_in_wfi !== last_wfi) begin
                     test_end_monitor_update_wfi(GLOBAL_ID, thread_in_wfi);
                     last_wfi <= thread_in_wfi;
                  end
               end
            end
         end // if (PREVENT_DPI == 1'b0) ...
      endgenerate
   `endif // ifdef ET_TESTEND_DPI

endmodule

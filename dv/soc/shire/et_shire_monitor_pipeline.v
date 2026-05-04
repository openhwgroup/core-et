`include "rtl/inc/soc.vh"

`ifdef ZEBU
   `ifndef ET_USE_SHIRE_MONITOR_PIPE
      `define ET_USE_SHIRE_MONITOR_PIPE 1
   `endif // ifndef ET_USE_SHIRE_MONITOR_PIPE

   `ifndef ET_INCLUDE_TESTEND_TRIGGERS
      `define ET_INCLUDE_TESTEND_TRIGGERS 1
   `endif // ifndef ET_INCLUDE_TESTEND_TRIGGERS
`endif // ifdef ZEBU

module et_shire_monitor_pipeline #(parameter int SHIRE_ID    = 0,
                                   parameter int NUM_MINIONS = (`NUM_NEIGH * `MIN_PER_N)) (
      input wire clock
   );

   localparam int MAX_THREAD           = (`CORE_NR_THREADS - 1);
   localparam int MAX_MINION_PER_SHIRE = (NUM_MINIONS == 1)              ? 1                    : (`NUM_NEIGH * `MIN_PER_N);
   localparam int GLOBAL_SHIRE_ID      = (SHIRE_ID > `NUM_MINION_SHIRES) ? (`NUM_MINION_SHIRES) : SHIRE_ID;
   localparam int MAX_MINION_MASK      = (MAX_MINION_PER_SHIRE < 32)     ? 32                   : MAX_MINION_PER_SHIRE;
   localparam int SELECT_LSB           = (GLOBAL_SHIRE_ID * (`NUM_NEIGH * `MIN_PER_N));
   localparam int SELECT_MSB           = (GLOBAL_SHIRE_ID == (`NUM_MINION_SHIRES)) ? SELECT_LSB : (SELECT_LSB + NUM_MINIONS - 1);


   wire                                       reset;
   wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_valid;
   wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_done;
   wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_fail;
   wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_timeout;
   wire [SELECT_MSB:SELECT_LSB]               hart_isolate;
   wire [SELECT_MSB:SELECT_LSB]               hart_reset_done;

   reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_valid_r;
   reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_done_r;
   reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_fail_r;
   reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_timeout_r;
   reg  [SELECT_MSB:SELECT_LSB]               hart_isolate_r;
   reg  [SELECT_MSB:SELECT_LSB]               hart_reset_done_r;

   reg                                        wfi_ends_test;
   reg                                        ecall_ends_test;
   reg                                        csr_stall_ends_test;
   reg                                        csr_stall_one_ends_test;
   reg  [7:0]                                 csr_stall_done_count;
   reg  [31:0]                                thread_timeout_cycles;
   reg  [MAX_MINION_MASK-1:0]                 minion_disable = {(MAX_MINION_MASK){1'b0}};
   reg  [MAX_THREAD:0]                        thread_disable = {(MAX_THREAD + 1){1'b0}};
   reg  [63:0]                                cycle;

   assign reset = soc_monitor.zebu_reset;

   assign soc_monitor.hart_valid[SELECT_MSB:SELECT_LSB]      = hart_valid_r;
   assign soc_monitor.hart_done[SELECT_MSB:SELECT_LSB]       = hart_done_r;
   assign soc_monitor.hart_fail[SELECT_MSB:SELECT_LSB]       = hart_fail_r;
   assign soc_monitor.hart_timeout[SELECT_MSB:SELECT_LSB]    = hart_timeout_r;
   assign soc_monitor.hart_isolate[SELECT_MSB:SELECT_LSB]    = hart_isolate_r;
   assign soc_monitor.hart_reset_done[SELECT_MSB:SELECT_LSB] = hart_reset_done_r;

   always @(posedge clock) begin
      hart_valid_r            <= hart_valid;
      hart_done_r             <= hart_done;
      hart_fail_r             <= hart_fail;
      hart_timeout_r          <= hart_timeout;
      hart_isolate_r          <= hart_isolate;
      hart_reset_done_r       <= hart_reset_done;
      wfi_ends_test           <= soc_monitor.wfi_ends_test;
      ecall_ends_test         <= soc_monitor.ecall_ends_test;
      csr_stall_ends_test     <= soc_monitor.csr_stall_ends_test;
      csr_stall_one_ends_test <= soc_monitor.csr_stall_one_ends_test[GLOBAL_SHIRE_ID];
      csr_stall_done_count    <= soc_monitor.csr_stall_done_count;
      thread_timeout_cycles   <= soc_monitor.thread_timeout_cycles;
      minion_disable          <= soc_monitor.minion_disable;
      thread_disable          <= soc_monitor.thread_disable;
      cycle                   <= soc_monitor.cycle;
   end

   `ifdef ET_INCLUDE_TESTEND_TRIGGERS
      wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_instr_trigger;
      wire [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_pc_trigger;

      reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_instr_trigger_r;
      reg  [SELECT_MSB:SELECT_LSB][MAX_THREAD:0] hart_pc_trigger_r;
      reg                                        clear_triggers;

      assign soc_monitor.hart_instr_trigger[SELECT_MSB:SELECT_LSB] = hart_instr_trigger_r;
      assign soc_monitor.hart_pc_trigger[SELECT_MSB:SELECT_LSB]    = hart_pc_trigger_r;

      always @(posedge clock) begin
         hart_instr_trigger_r <= hart_instr_trigger;
         hart_pc_trigger_r    <= hart_pc_trigger;
         clear_triggers       <= soc_monitor.clear_triggers;
      end
   `endif // ifdef ET_INCLUDE_TESTEND_TRIGGERS

endmodule

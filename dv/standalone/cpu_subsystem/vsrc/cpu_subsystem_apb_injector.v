
`include "soc.vh"

module cpu_subsystem_apb_injector
  #(
    parameter int APB_ADDR_WIDTH = 32,
    parameter int APB_DATA_WIDTH = 64
    )
  (
   input logic         clock,
   input logic         reset,
   output logic        enabled,
   output logic        apb_psel,
   output logic        apb_penable,
   output logic        apb_pwrite,
   output logic [APB_ADDR_WIDTH-1:0] apb_paddr,
   output logic [APB_DATA_WIDTH-1:0] apb_pwdata,
   input logic [APB_DATA_WIDTH-1:0]  apb_prdata,
   input logic         apb_pready,
   input logic        apb_pslverr
   );
   `ifdef ENABLE_BEMU
  import "DPI-C" function void esr_write(input longint unsigned paddr, input longint unsigned data);
   `endif

  logic [`MIN_PER_S-1:0] thread0_disable;
  logic [`MIN_PER_S-1:0] thread1_disable;
  logic [`MIN_PER_S-1:0] minion_mask;
  logic [`MIN_PER_S-1:0] minion_enable;
  logic [`CORE_NR_THREADS-1:0] thread_mask;
  logic [`CORE_NR_THREADS-1:0] thread_enable;
  logic [APB_DATA_WIDTH-1:0]  reg_data;
  logic [`VA_SIZE-1:0] start_pc;
  logic [`VA_SIZE-1:0] minion_boot;

   wire [APB_ADDR_WIDTH-1:0]   thread0_disable_addr = 32'h80f40240;
   wire [APB_ADDR_WIDTH-1:0]   thread1_disable_addr = 32'h80f40010;
   wire [APB_ADDR_WIDTH-1:0]   minion_boot_addr     = 32'h80d00018;

   initial begin
      thread0_disable = '0;
      thread1_disable = '0;
      minion_boot     = '0;
      if (`et_value_plusargs("THREAD_MASK=0x%x", thread_enable) != 0) begin
         thread_mask = thread_enable[`CORE_NR_THREADS-1:0];
      end
      else begin
         thread_mask = {{`CORE_NR_THREADS-1{1'b0}}, 1'b1};
      end
      if (`et_value_plusargs("MINION_MASK=0x%x", minion_enable) != 0) begin
         minion_mask = minion_enable;
      end
      else begin
         minion_mask = {{`MIN_PER_S-1{1'b0}}, 1'b1};
      end
      for(int i = 0; i < `THREADS_PER_N; ++i) begin
        if (i%2 == 0) begin
           thread0_disable[i/2] = !(minion_mask[i/2] && thread_mask[i%2]);
        end else begin
          thread1_disable[i/2] = !(minion_mask[i/2] && thread_mask[i%2]);
        end
      end
      if (`et_value_plusargs("START_PC=%x", start_pc) != 0) begin
        minion_boot = start_pc;
      end
      else begin
        minion_boot = `ESR_NEIGH_MINION_BOOT_RESET_VAL;
      end
      enabled = 0;
      wait(reset == 1);
      wait(reset == 0);
      repeat (3) @(posedge clock); // Wait a few cycles to reset=0 propagates in subsystem
      enabled = 1;
      reg_data = `ZX(APB_DATA_WIDTH, minion_boot);
      apb_write(minion_boot_addr, reg_data);
`ifdef ENABLE_BEMU
      esr_write (minion_boot_addr, reg_data );
`endif
      reg_data = `ZX(APB_DATA_WIDTH, thread0_disable);
      apb_write(thread0_disable_addr, reg_data);
`ifdef ENABLE_BEMU
      esr_write (thread0_disable_addr, reg_data );
`endif
      reg_data = `ZX(APB_DATA_WIDTH, thread1_disable);
      apb_write(thread1_disable_addr, reg_data);
`ifdef ENABLE_BEMU
      esr_write (thread1_disable_addr, reg_data );
`endif
      @(posedge clock);

      // APB sequence file processing
      begin
        integer                    apb_seq_en;
        string                     apb_seq_file;
        integer                    seq_fd;
        string                     line;
        string                     cmd;
        logic [APB_ADDR_WIDTH-1:0] seq_addr;
        logic [APB_DATA_WIDTH-1:0] seq_val;
        logic [APB_DATA_WIDTH-1:0] seq_mask;
        logic [APB_DATA_WIDTH-1:0] seq_comp;
        logic [APB_DATA_WIDTH-1:0] seq_rdata;
        integer                    poll_count;
        int                        scan_ret;

        apb_seq_en = 0;
        if (`et_value_plusargs("APB_SEQ_EN=%d", apb_seq_en) != 0 && apb_seq_en == 1) begin

          if (`et_value_plusargs("APB_SEQ_FILE=%s", apb_seq_file) == 0) begin
            $fatal(1, "[APB_INJECTOR] APB_SEQ_EN=1 but APB_SEQ_FILE plusarg not provided");
          end

          seq_fd = $fopen(apb_seq_file, "r");
          if (seq_fd == 0) begin
            $fatal(1, "[APB_INJECTOR] Could not open APB_SEQ_FILE: %s", apb_seq_file);
          end

          while (!$feof(seq_fd)) begin
            if ($fgets(line, seq_fd) == 0) disable fork; // end of file

            // Skip blank lines and comments
            if (line.len() == 0) continue;
            if (line.substr(0, 0) == "#") continue;
            if (line.substr(0, 0) == "\n") continue;               

            // Try WRITE ADDR VALUE
            scan_ret = $sscanf(line, "%s %h %h", cmd, seq_addr, seq_val);
            if (scan_ret == 3 && cmd == "WRITE") begin
              apb_write(seq_addr, seq_val);
             `ifdef ENABLE_BEMU
              esr_write (seq_addr, seq_val);
             `endif
              continue;
            end

            // Try POLL ADDR MASK COMP
            scan_ret = $sscanf(line, "%s %h %h %h", cmd, seq_addr, seq_mask, seq_comp);
            if (scan_ret == 4 && cmd == "POLL") begin
              poll_count = 0;
              do begin
                apb_read(seq_addr, seq_rdata);
                if (poll_count >= 1000) begin
                  $fatal(1, "[APB_INJECTOR] POLL timeout after 1000 iterations: ADDR=0x%h MASK=0x%h COMP=0x%h last_read=0x%h",
                         seq_addr, seq_mask, seq_comp, seq_rdata);
                end
                poll_count++;
              end while ((seq_rdata & seq_mask) != seq_comp);
              continue;
            end

            // Try READ ADDR
            scan_ret = $sscanf(line, "%s %h", cmd, seq_addr);
            if (scan_ret == 2 && cmd == "READ") begin
              apb_read(seq_addr, seq_rdata);
              $display("[APB_INJECTOR] READ ADDR=0x%h DATA=0x%h", seq_addr, seq_rdata);
              continue;
            end

            // Unknown line — strip trailing newline for cleaner warning
            $warning("[APB_INJECTOR] Unrecognized line in APB_SEQ_FILE: %s", line);
          end

          $fclose(seq_fd);
        end
      end

      enabled = 0;
   end

  // ---------------------------------------------------------------------------
  // APB Write Task
  // ---------------------------------------------------------------------------
  task apb_write;
  input [APB_ADDR_WIDTH -1:0] addr;
  input [APB_DATA_WIDTH-1:0] data;
  integer injector_log_file;
  begin
    injector_log_file = $fopen("cpu_subsystem_apb_injector.log", "a");
    apb_paddr   = addr;
    apb_pwrite  = 1'b1;
    apb_psel    = 1'b1;
    apb_penable = 1'b0;
    apb_pwdata  = data;
    @(posedge clock);
    apb_penable = 1'b1;
    @(negedge clock);
    while (apb_pready==1'b0 || apb_pslverr==1'b1) begin
      @(posedge clock);
      apb_penable = 1'b1;
    end
    apb_paddr   = {APB_ADDR_WIDTH{1'b0}};
    apb_pwrite  = 1'b0;
    apb_psel    = 1'b0;
    apb_penable = 1'b0;
    apb_pwdata  = {APB_DATA_WIDTH{1'b0}};
    $fwrite(injector_log_file,"{ A:0x%h, D:0x%h }\n", addr, data);
    $fclose(injector_log_file);
    @(posedge clock);
  end
  endtask

  // ---------------------------------------------------------------------------
  // APB Read Task
  // ---------------------------------------------------------------------------
  task apb_read;
  input  [APB_ADDR_WIDTH-1:0] addr;
  output [APB_DATA_WIDTH-1:0] data;
  integer injector_log_file;
  begin
    injector_log_file = $fopen("cpu_subsystem_apb_injector.log", "a");
    apb_paddr   = addr;
    apb_pwrite  = 1'b0;
    apb_psel    = 1'b1;
    apb_penable = 1'b0;
    apb_pwdata  = {APB_DATA_WIDTH{1'b0}};
    @(posedge clock);
    apb_penable = 1'b1;
    @(negedge clock);
    while (apb_pready==1'b0 || apb_pslverr==1'b1) begin
      @(posedge clock);
      apb_penable = 1'b1;
    end
    data        = apb_prdata;
    apb_paddr   = {APB_ADDR_WIDTH{1'b0}};
    apb_pwrite  = 1'b0;
    apb_psel    = 1'b0;
    apb_penable = 1'b0;
    $fwrite(injector_log_file,"{ A:0x%h, D:0x%h }\n", addr, data);
    $fclose(injector_log_file);
    @(posedge clock);
  end
  endtask

endmodule

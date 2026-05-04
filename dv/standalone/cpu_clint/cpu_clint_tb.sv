`define CLK_HALF_PERIOD 0.5 // timescale units, usually ns
`define CLK_EXT_HALF_PERIOD 2.5

`include "soc.vh"

module clint_tb ( );

logic clk = 1;
logic clk_ext = 1;
always #(`CLK_HALF_PERIOD) clk = ~clk;
always #(`CLK_EXT_HALF_PERIOD) clk_ext = ~clk_ext;

logic mtime_clk;
//assign mtime_clk = clk_ext;
assign mtime_clk = clk;

localparam unsigned N_HARTS = 16;
localparam unsigned TIMEOUT_CYCLES = 100_000;
int cycle_count;

logic reset = 0;
logic in, out;
cpu_clint_timer_regs_we_t timer_regs_we;
cpu_clint_timer_regs_t timer_regs_write;
cpu_clint_timer_regs_t timer_regs_read;
logic [N_HARTS-1:0] mtip;

ipi_esr_regs_we_t ipi_regs_we;
ipi_esr_regs_t ipi_regs_write;
ipi_esr_regs_t ipi_regs_read;
logic [N_HARTS-1:0] msip;

task write_mtime;
    input logic [63:0] wdata;
    begin
        $display("    WRITE %0t mtime: %0d (0x%0h)", $time, wdata, wdata);
        timer_regs_write.mtime = wdata;
        timer_regs_we.mtime = 1'b1;
        @(posedge mtime_clk);
        timer_regs_we.mtime = 1'b0;
    end
endtask

task write_mtimecmp;
    input logic [63:0] wdata;
    begin
        $display("    WRITE %0t mtimecmp: %0d (0x%0h)", $time, wdata, wdata);
        timer_regs_write.mtimecmp = wdata;
        timer_regs_we.mtimecmp = 1'b1;
        @(posedge mtime_clk);
        timer_regs_we.mtimecmp = 1'b0;
    end
endtask

task write_mtime_local_target;
    input logic [63:0] wdata;
    begin
        $display("    WRITE %0t mtime_local_target: %0d (0x%0h) (b %4b'%4b'%4b'%4b)",
            $time, wdata, wdata, wdata[15-:4], wdata[11-:4], wdata[7-:4], wdata[3-:4]);
        timer_regs_write.mtime_local_target = wdata;
        timer_regs_we.mtime_local_target = 1'b1;
        @(posedge mtime_clk);
        timer_regs_we.mtime_local_target = 1'b0;
    end
endtask

task write_time_config;
    input logic [63:0] wdata;
    begin
        $display("    WRITE %0t time_config: %0d (0x%0h)", $time, wdata, wdata);
        timer_regs_write.time_config = wdata;
        timer_regs_we.time_config = 1'b1;
        @(posedge mtime_clk);
        timer_regs_we.time_config = 1'b0;
    end
endtask

initial begin
    // set %t:
    // - scaled in ns (-9),
    // - with 0 precision digits
    // - with the " ns" string
    // - taking up a total of 12 characters, including the string
    $timeformat(-9, 0, " ns", 12);
end

int unsigned wclks = 30;

initial begin
    $display("\nstarting tb");
    timer_regs_we = 'h0;
    timer_regs_write = 'h0;
    ipi_regs_we = 'h0;
    ipi_regs_write = 'h0;
    // reset
    reset = 1;
    repeat(12) @(posedge clk);
    reset = 0;
    repeat(100) @(posedge clk);

    begin: TIMER
        $display("TIMER: starting test %0t", $time);
        repeat(5) @(posedge mtime_clk);

        write_mtime('h0);
        repeat(5) @(posedge mtime_clk);

        //----------------------------------------------------------------------
        // simple single hart iterrupt
        write_mtime_local_target(1'd1);
        repeat(5) @(posedge mtime_clk);
        write_mtimecmp('d120);
        repeat(10) @(posedge mtime_clk);

        while (mtip[0] !== 1'b1) @(posedge mtime_clk);
        $display("TIMER: mtip fired 1st at %0t", $time);
        for (int h = 1; h < N_HARTS; h++) begin
            if (mtip[h]) $error("TIMER: inactive mtip %0d fired at %0t", h, $time);
        end
        repeat(10) @(posedge mtime_clk);

        //----------------------------------------------------------------------
        // simple single hart iterrupt - inc mtimecmp to clear old interrupt
        write_mtimecmp('d320);
        repeat(wclks) @(posedge mtime_clk);

        while (mtip[0] !== 1'b1) @(posedge mtime_clk);
        $display("TIMER: mtip fired 2nd at %0t", $time);
        for (int h = 1; h < N_HARTS; h++) begin
            if (mtip[h]) $error("TIMER: inactive mtip %0d fired at %0t", h, $time);
        end

        //----------------------------------------------------------------------
        // multiple harts
        write_mtimecmp('d520);
        repeat(wclks) @(posedge mtime_clk);

        write_mtime_local_target((('h1 << 4) | ('h1 << 2) | ('h1 << 0)));
        @(posedge mtime_clk);

        while ((mtip[0] && mtip[2] && mtip[4]) !== 1'b1) @(posedge mtime_clk);
        $display("TIMER: mtip 0 2 4 fired at %0t", $time);
        for (int h = 0; h < N_HARTS; h++) begin
            if (h == 0 || h == 2 || h == 4) continue;
            if (mtip[h]) $error("TIMER: inactive mtip %0d fired at %0t", h, $time);
        end

        //----------------------------------------------------------------------
        // back to single hart and increase thr so it takes longer for mtime inc
        write_mtimecmp('d720);
        repeat(wclks) @(posedge mtime_clk);

        write_mtime_local_target(1'd1); // back to single hart
        @(posedge mtime_clk);

        write_time_config('h1a); // slower
        @(posedge mtime_clk);

        while (mtip[0] !== 1'b1) @(posedge mtime_clk);
        $display("TIMER: (slow) mtip fired at %0t", $time);
        for (int h = 1; h < N_HARTS; h++) begin
            if (mtip[h]) $error("TIMER: inactive mtip %0d fired at %0t", h, $time);
        end

        //----------------------------------------------------------------------
        // single hart, default thr, using cpu_subsystem (fast) clk
        write_mtimecmp('d920);
        repeat(wclks) @(posedge mtime_clk);

        write_time_config('h14 | (1'b1 << 7)); // back to default thr, fast clk
        @(posedge mtime_clk);

        while (mtip[0] !== 1'b1) @(posedge mtime_clk);
        $display("TIMER: (cpu clock) mtip fired at %0t", $time);
        for (int h = 1; h < N_HARTS; h++) begin
            if (mtip[h]) $error("TIMER: inactive mtip %0d fired at %0t", h, $time);
        end

        //----------------------------------------------------------------------
        write_mtimecmp('d2000);
        $display("TIMER: finished test %0t", $time);
    end

    repeat(100) @(posedge clk);
    $display("finished tb at %0t\n", $time);
    $finish;
end

cpu_clint dut_clint (
    .clock_ext (clk_ext),
    //.clock_ext (clk),
    .clock (clk),
    .reset (reset),
    // timer
    .timer_regs_we (timer_regs_we),
    .timer_regs_write (timer_regs_write),
    .timer_regs_read (timer_regs_read),
    .mtip (mtip),
    // ipi
    .esr_ipi_trigger (16'h0),
    .msip (msip)
);

initial begin
    cycle_count = 0;
    wait (reset == 1);
    @(posedge clk);
    while(1) begin
        cycle_count = (cycle_count + 1);
        if (cycle_count == TIMEOUT_CYCLES) begin
            $error("timeout reached");
            $finish;
        end
        @(posedge clk);
    end
end

endmodule

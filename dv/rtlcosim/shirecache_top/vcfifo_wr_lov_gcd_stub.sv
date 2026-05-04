// Simulation-only stub for vcfifo_wr_lov_gcd.
// Matches the original's port interface but uses simple behavioral logic.
// No ASIC dependencies (no level shifters, no vcfifo_mem, etc.)

/* verilator lint_off UNUSEDSIGNAL */
module vcfifo_wr_lov_gcd #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_PTR_SYNC_STAGES = 2,
   RD_PTR_SYNC_STAGES = 2,
   FF_MEM_IMPL = 1
) (
   input  logic                  clock_push,
   input  logic                  reset_push,
   input  logic                  clock_pop,
   input  logic                  reset_pop,

   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,

   output logic                  valid,
   input  logic                  pop,
   output logic [ELEM_SIZE-1:0]  pop_data,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);

   localparam PTR_W = $clog2(NUM_ELEMS);
   localparam GRAY_W = PTR_W + 1;
   localparam MEM_DEPTH = 2**PTR_W;

   // Memory
   logic [ELEM_SIZE-1:0] mem [MEM_DEPTH];

   // Binary/gray code conversion
   function automatic logic [GRAY_W-1:0] bin2gray(input logic [GRAY_W-1:0] bin);
     return bin ^ (bin >> 1);
   endfunction

   function automatic logic [GRAY_W-1:0] gray2bin(input logic [GRAY_W-1:0] gray);
     logic [GRAY_W-1:0] bin;
     bin[GRAY_W-1] = gray[GRAY_W-1];
     for (int i = GRAY_W-2; i >= 0; i--)
       bin[i] = bin[i+1] ^ gray[i];
     return bin;
   endfunction

   // Write domain
   logic [GRAY_W-1:0] wptr_bin_q, wptr_gray_q;
   logic [GRAY_W-1:0] rptr_gray_sync;
   logic               full;

   always_ff @(posedge clock_push or posedge reset_push) begin
     if (reset_push) begin
       wptr_bin_q  <= '0;
       wptr_gray_q <= '0;
     end else if (push & ~full) begin
       wptr_bin_q  <= wptr_bin_q + GRAY_W'(1);
       wptr_gray_q <= bin2gray(wptr_bin_q + GRAY_W'(1));
     end
   end

   always_ff @(posedge clock_push) begin
     if (push & ~full)
       mem[wptr_bin_q[PTR_W-1:0]] <= push_data;
   end

   logic [GRAY_W-1:0] rptr_bin_sync;
   logic [GRAY_W-1:0] occupancy_wr;
   assign rptr_bin_sync = gray2bin(rptr_gray_sync);
   assign occupancy_wr  = wptr_bin_q - rptr_bin_sync;
   assign full          = (occupancy_wr >= GRAY_W'(NUM_ELEMS));
   assign ready         = ~full;

   // Read domain
   logic [GRAY_W-1:0] rptr_bin_q, rptr_gray_q;
   logic [GRAY_W-1:0] wptr_gray_sync;
   logic               empty;

   always_ff @(posedge clock_pop or posedge reset_pop) begin
     if (reset_pop) begin
       rptr_bin_q  <= '0;
       rptr_gray_q <= '0;
     end else if (pop & ~empty) begin
       rptr_bin_q  <= rptr_bin_q + GRAY_W'(1);
       rptr_gray_q <= bin2gray(rptr_bin_q + GRAY_W'(1));
     end
   end

   assign pop_data = mem[rptr_bin_q[PTR_W-1:0]];
   assign empty    = (rptr_gray_q == wptr_gray_sync);
   assign valid    = ~empty;

   // CDC synchronizers (write gray -> read domain)
   logic [GRAY_W-1:0] wptr_sync [WR_PTR_SYNC_STAGES];
   always_ff @(posedge clock_pop or posedge reset_pop) begin
     if (reset_pop)
       for (int i = 0; i < WR_PTR_SYNC_STAGES; i++) wptr_sync[i] <= '0;
     else begin
       wptr_sync[0] <= wptr_gray_q;
       for (int i = 1; i < WR_PTR_SYNC_STAGES; i++) wptr_sync[i] <= wptr_sync[i-1];
     end
   end
   assign wptr_gray_sync = wptr_sync[WR_PTR_SYNC_STAGES-1];

   // CDC synchronizers (read gray -> write domain)
   logic [GRAY_W-1:0] rptr_sync [RD_PTR_SYNC_STAGES];
   always_ff @(posedge clock_push or posedge reset_push) begin
     if (reset_push)
       for (int i = 0; i < RD_PTR_SYNC_STAGES; i++) rptr_sync[i] <= '0;
     else begin
       rptr_sync[0] <= rptr_gray_q;
       for (int i = 1; i < RD_PTR_SYNC_STAGES; i++) rptr_sync[i] <= rptr_sync[i-1];
     end
   end
   assign rptr_gray_sync = rptr_sync[RD_PTR_SYNC_STAGES-1];

   logic unused_ok;
   assign unused_ok = &{1'b0, dft__reset_byp_push, dft__reset_push, dft__reset_byp_pop, dft__reset_pop};

endmodule
/* verilator lint_on UNUSEDSIGNAL */

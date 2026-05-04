`include "soc.vh"

`include "uevent_minion.vh"

//==================================================================================================
//
// Minion Pending DPI Event FIFO
//
// This module is used to collect pending DPI events for each of the instantiated Minions.  The
// basic per-neighborhood structure is as follows:
//
//   minion_event_fifo #0                            minion_event_fifo #7
//   +-----------------------------------------+     +-----------------------------------------+
//   | retire_event_fifo     vpu_event_fifo    |     | retire_event_fifo     vpu_event_fifo    |
//   | +---------------+     +---------------+ |     | +---------------+     +---------------+ |
//   | |               |     |               | |     | |               |     |               | |
//   | |  |         |  |     |  |         |  | |     | |  |         |  |     |  |         |  | |
//   | |  |---------|  |     |  |---------|  | |     | |  |---------|  |     |  |---------|  | |
//   | |  |---------|  | ... |  |---------|  | |     | |  |---------|  | ... |  |---------|  | |
//   | |  |---------|  |     |  |---------|  | |     | |  |---------|  |     |  |---------|  | |
//   | |  +---------+  |     |  +---------+  | |     | |  +---------+  |     |  +---------+  | |
//   | |       |       |     |       |       | |     | |       |       |     |       |       | |
//   | +-------|-------+     +-------|-------+ |     | +-------|-------+     +-------|-------+ |
//   |         |                     |         |     |         |                     |         |
//   |         v                     v         | ... |         v                     v         |
//   | +-------------------------------------+ |     | +-------------------------------------+ |
//   |  \          dpi_event_select         /  |     |  \          dpi_event_select         /  |
//   |   +---------------------------------+   |     |   +---------------------------------+   |
//   |                    |                    |     |                    |                    |
//   |                    |                    |     |                    |                    |
//   |               |    v    |               |     |               |    v    |               |
//   |               |---------|               |     |               |---------|               |
//   | fast clock -->|---------|               |     | fast clock -->|---------|               |
//   |               |---------|               |     |               |---------|               |
//   |               +---------+               |     |               +---------+               |
//   |                    |                    |     |                    |                    |
//   +--------------------|--------------------+     +--------------------|--------------------+
//                        |                                               |
//   neigh_event_fifo     |                                               |
//   +--------------------|-----------------------------------------------|--------------------+
//   |                    |                                               |                    |
//   |                    v                                               v                    |
//   | +-------------------------------------------------------------------------------------+ |
//   |  \                                  dpi_event_select                                 /  |
//   |   +---------------------------------------------------------------------------------+   |
//   |                                            |                                            |
//   |                                            v                                            |
//   |                         fast clock --> DPI call                                         |
//   |                                                                                         |
//   +-----------------------------------------------------------------------------------------+
//
// Note that the FIFOs that capture raw CPU events (in retire_event_fifo and vpu_event_fifo, for
// example) run off of the appropriate CPU clock, but all other FIFOs run with the fastest clock in
// the system.  This means that the raw CPU event FIFOs can empty at a faster rate than they fill
// (although because emptying these queues is time-sliced, they can still overflow).
//
// Output data type: uevent_minion_packet
//
module minion_event_fifo #(parameter int SHIRE_ID       = 0,
                           parameter int MINION_ID      = 0,
                           parameter int HART_ID_SIZE   = `NUM_SHIRE_IDS_L + `MIN_PER_S_L,
                           parameter int SOC_CYCLE_SIZE = 64) (
      input wire                      reset,
      input wire                      clock,
      input wire [SOC_CYCLE_SIZE-1:0] soc_cycle,
      input wire [`MIN_PER_S_L-1:0]   shire_min_id
   );

   localparam int EVENT_COUNT  = 2;
   localparam int FIFO_DEPTH   = 32;
   localparam int SHIRE_SELECT = (SHIRE_ID > 33) ? 34 : SHIRE_ID;

   // Public interface used by neigh_event_fifo
   wire                 minion_event_empty;
   wor                  minion_event_pop;
   uevent_minion_packet minion_event_data_out;

   // Capture values from MINION_CORE
   wire                      rd_clk       = top.clk_max;
   wire                      wb_valid     = `MINION_CORE.wb_valid;
   wire                      wb_xcpt      = `MINION_CORE.wb_xcpt;
   wire [`CORE_NR_THREADS_R] wb_thread_id = `MINION_CORE.wb_thread_id;
   wire [`XREG_RANGE]        wb_reg_pc    = `SX(`XREG_SIZE, `MINION_CORE.wb_reg_pc);
   wire [HART_ID_SIZE-1:0]   hart_id;
   bit  [31:0]               hart_id_r;
   bit                       enable;
   // Retires
   wire                      id_inst_en   = `MINION_CORE.id_inst_en;
   wire                      ex_pc_valid  = `MINION_CORE.ex_pc_valid | `MINION_CORE.gsc_pc_valid;
   wire                      tag_pc_valid = `MINION_CORE.tag_pc_valid;
   wire                      mem_pc_valid = `MINION_CORE.mem_pc_valid;
   wire [`INST_RANGE]        id_inst_bits = `MINION_CORE.id_inst_bits;
   reg  [`INST_RANGE]        ex_reg_inst;
   reg  [`INST_RANGE]        tag_reg_inst;
   reg  [`INST_RANGE]        mem_reg_inst;
   reg  [`INST_RANGE]        wb_reg_inst;
   // Exceptions & Interrupts
   reg  [`INST_RANGE]        ex_reg_inst_xcpt;
   reg  [`INST_RANGE]        tag_reg_inst_xcpt;
   reg  [`INST_RANGE]        mem_reg_inst_xcpt;
   reg  [`INST_RANGE]        wb_reg_inst_xcpt;
   wire                      id_pc_xcpt        = `MINION_CORE.ex_reg_inst_ff.enable  | `MINION_CORE.ex_reg_inst_ff.xcpt;
   wire                      ex_pc_xcpt        = `MINION_CORE.tag_reg_inst_ff.enable | `MINION_CORE.tag_reg_inst_ff.xcpt;
   wire                      tag_pc_xcpt       = `MINION_CORE.mem_reg_inst_ff.enable | `MINION_CORE.mem_reg_inst_ff.xcpt;
   wire                      mem_pc_xcpt       = `MINION_CORE.wb_reg_inst_ff.enable  | `MINION_CORE.wb_reg_inst_ff.xcpt;
   wire [`INST_RANGE]        id_inst_bits_xcpt = `MINION_CORE.ex_reg_inst_ff.in;

   // IOs for the event selector
   wire                 [EVENT_COUNT-1:0] fifo_valid;
   uevent_minion_packet [EVENT_COUNT-1:0] select_data_in;
   wire                 [EVENT_COUNT-1:0] fifo_pop;

   assign minion_event_pop = 1'b0;
   assign hart_id          = hart_id_r[HART_ID_SIZE-1:0];

   // Flop instruction bits from ID
   // (In RTL some of these bits are dropped)
   always_ff @(posedge clock) begin
      hart_id_r <= { SHIRE_SELECT[31-`MIN_PER_S_L:0], shire_min_id };
      enable    <= ~(top.m_shire_disable[SHIRE_SELECT] | top.no_arch_mon[SHIRE_SELECT] | top.m_minion_ev_disable[MINION_ID]);
      if (mem_pc_valid === 1'b1) begin
         wb_reg_inst <= mem_reg_inst;
      end
      if (tag_pc_valid === 1'b1) begin
         mem_reg_inst <= tag_reg_inst;
      end
      if (ex_pc_valid === 1'b1) begin
         tag_reg_inst <= ex_reg_inst;
      end
      if (id_inst_en === 1'b1) begin
         ex_reg_inst <= id_inst_bits;
      end
      if (mem_pc_xcpt === 1'b1) begin
         wb_reg_inst_xcpt <= mem_reg_inst_xcpt;
      end
      if (tag_pc_xcpt === 1'b1) begin
         mem_reg_inst_xcpt <= tag_reg_inst_xcpt;
      end
      if (ex_pc_xcpt === 1'b1) begin
         tag_reg_inst_xcpt <= ex_reg_inst_xcpt;
      end
      if (id_pc_xcpt === 1'b1) begin
         ex_reg_inst_xcpt <= id_inst_bits_xcpt;
      end
   end


   //===============================================================================================
   //
   // Retire Event FIFO
   //
   bit                 retire_enable;
   wire                retire_empty;
   wire                retire_pop;
   uevent_retire_event retire_data_out;

   assign fifo_valid[0]                 = ~retire_empty;
   assign select_data_in[0].event_type  = `UEVENT_TYPE_RETIRE;
   assign select_data_in[0].data.retire = retire_data_out;
   assign retire_pop                    = fifo_pop[0];

   always_ff @(posedge clock) begin
      retire_enable <= top.m_enable_event_fifos[(`UEVENT_TYPE_RETIRE - 1)] & enable;
   end

   retire_event_fifo #(.HART_ID_SIZE(HART_ID_SIZE)) retire_event_fifo (
         .reset        (reset),
         .clock        (clock),
         .enable       (retire_enable),
         .soc_cycle    (soc_cycle),
         .rd_clk       (rd_clk),
         .pop          (retire_pop),
         .empty        (retire_empty),
         .data_out     (retire_data_out),
         .hart_id      (hart_id),
         .wb_thread_id (wb_thread_id),
         .wb_valid     (wb_valid),
         .wb_reg_pc    (wb_reg_pc),
         .wb_reg_inst  (wb_reg_inst)
      );

   //===============================================================================================
   //
   // Exception & Interrupt Event FIFO
   //
   bit                    exception_enable;
   wire                   exception_empty;
   wire                   exception_pop;
   uevent_exception_event exception_data_out;

   assign fifo_valid[1]                    = ~exception_empty;
   assign select_data_in[1].event_type     = `UEVENT_TYPE_EXCEPTION;
   assign select_data_in[1].data.exception = exception_data_out;
   assign exception_pop                    = fifo_pop[1];

   always_ff @(posedge clock) begin
      exception_enable <= top.m_enable_event_fifos[(`UEVENT_TYPE_EXCEPTION - 1)] & enable;
   end

   exception_event_fifo #(.HART_ID_SIZE(HART_ID_SIZE)) exception_event_fifo (
         .reset        (reset),
         .clock        (clock),
         .enable       (exception_enable),
         .soc_cycle    (soc_cycle),
         .rd_clk       (rd_clk),
         .pop          (exception_pop),
         .empty        (exception_empty),
         .data_out     (exception_data_out),
         .hart_id      (hart_id),
         .wb_thread_id (wb_thread_id),
         .wb_xcpt      (wb_xcpt),
         .wb_reg_pc    (wb_reg_pc),
         .wb_reg_inst  (wb_reg_inst_xcpt)
      );



   //===============================================================================================
   //
   // Minion Mux and FIFO
   //
   wire                    minion_fifo_full;
   wire                    select_valid_out;
   uevent_minion_packet    select_data_out;
   uevent_minion_packet    fifo_data_out;

   assign minion_event_data_out = fifo_data_out;

   // The event selector (mux)
   dpi_event_select #(.EVENT_COUNT(EVENT_COUNT),
                      .DATA_MSB(`UEVENT_MINION_PACKET_MSB)) dpi_event_select (
         .reset     (reset),
         .clk       (rd_clk),
         .full_in   (minion_fifo_full),
         .valid_in  (fifo_valid),
         .data_in   (select_data_in),
         .pop_out   (fifo_pop),
         .valid_out (select_valid_out),
         .data_out  (select_data_out)
      );

   dpi_event_capture #(.DEPTH               (FIFO_DEPTH),
                       .DATA_MSB            (`UEVENT_MINION_PACKET_MSB),
                       .HIGH_WATERMARK_8THS (8),
                       .LOW_WATERMARK_8THS  (7)) dpi_event_capture (
         .reset           (reset),
         .wr_clk          (rd_clk),
         .rd_clk          (rd_clk),
         .enable          (1'b1),
         .valid_in        (select_valid_out),
         .data_in         (select_data_out),
         .pop_in          (minion_event_pop),
         .data_out        (fifo_data_out),
         .empty_out       (minion_event_empty),
         .nearly_full_out (minion_fifo_full),
         .full_out        ()
      );

endmodule

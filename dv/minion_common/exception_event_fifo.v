`include "soc.vh"

`include "uevent_minion.vh"

// ====================
// exception_event_fifo
// ====================
// Captures event of type uevent_exception_event.
// Exceptions are triggered on wb_xcpt.
// Output data type: uevent_exception_event

module exception_event_fifo #(parameter int SOC_CYCLE_SIZE = 64,
                              parameter int HART_ID_SIZE   = `NUM_SHIRE_IDS_L + `MIN_PER_S_L) (
      // The following IOs are necessary for all event FIFOs.
      input  wire                      reset,
      input  wire                      clock,
      input  wire                      enable,
      input  wire [SOC_CYCLE_SIZE-1:0] soc_cycle,
      input  wire                      rd_clk,
      input  wire                      pop,
      output wire                      empty,
      output uevent_exception_event    data_out, // output event
      // The following IOs are specific to this event FIFO.
      input  wire [HART_ID_SIZE-1:0]   hart_id,
      input  wire                      wb_xcpt,
      input  wire [`CORE_NR_THREADS_R] wb_thread_id,
      input  wire [`XREG_RANGE]        wb_reg_pc,
      input  wire [`INST_RANGE]        wb_reg_inst
   );

   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
      import evl_base_pkg::*;

      string abstract_name;
   `endif // ifdef EVL_SIMULATION

   localparam int FIFO_DEPTH = 64;

   // Once the underlying FIFO is nearly full, there is one additional entry available ... after
   // that, information is lost.
   reg [1:0] last_capture;

   // IOs for the underlying FIFO.
   uevent_exception_event fifo_data;
   uevent_exception_event fifo_data_out;
   reg                    fifo_valid;
   wire                   nearly_full;

   assign data_out = fifo_data_out;

   always_ff @(posedge clock) begin
      if ((reset !== 1'b0) || (enable !== 1'b1)) begin
         last_capture <= 2'b00;
         fifo_valid   <= 1'b0;
         fifo_data    <= {`UEVENT_EXCEPTION_EVENT_SIZE{1'b0}};
      end
      else if (wb_xcpt === 1'b1) begin
         fifo_data.cycle <= soc_cycle;
         fifo_data.hart  <= { hart_id, wb_thread_id };
         if (nearly_full === 1'b1) begin
            fifo_valid   <= ~last_capture[1];
            last_capture <= (last_capture >= 2'b01) ? 2'b11 : (last_capture + 2'b01);
            if (last_capture[0:0] == 1'b1) begin
               fifo_data.pc   <= {64{1'b1}};
               fifo_data.inst <= {32{1'b1}};
            end
            else begin
               fifo_data.pc   <= wb_reg_pc;
               fifo_data.inst <= wb_reg_inst;
            end
         end
         else begin // not nearly_full
            fifo_valid     <= 1'b1;
            fifo_data.pc   <= wb_reg_pc;
            fifo_data.inst <= wb_reg_inst;
            last_capture   <= 2'b00; // clear last_capture
         end
      end
      else begin // not wb_xcpt
         fifo_valid <= 1'b0;
      end
   end

   // This implements the actual FIFO.
   dpi_event_capture #(.DEPTH               (FIFO_DEPTH),
                       .DATA_MSB            (`UEVENT_EXCEPTION_EVENT_MSB),
                       .HIGH_WATERMARK_8THS (8),
                       .LOW_WATERMARK_8THS  (7)) dpi_event_capture (
         .reset           (reset),
         .wr_clk          (clock),
         .rd_clk          (rd_clk),
         .enable          (enable),
         .valid_in        (fifo_valid),
         .data_in         (fifo_data),
         .pop_in          (pop),
         .data_out        (fifo_data_out),
         .empty_out       (empty),
         .nearly_full_out (nearly_full),
         .full_out        ()
      );

endmodule

`include "soc.vh"

`include "uevent_neigh.vh"

//==================================================================================================
//
// Service Processor Pending DPI Event FIFO
//
// This module is used to collect pending DPI events from the service processor.  The basic mimics
// neighborhood event captures and is structured as follows:
//
//   minion_event_fifo
//   +-----------------------------------------+
//   | retire_event_fifo     vpu_event_fifo    |
//   | +---------------+     +---------------+ |
//   | |               |     |               | |
//   | |  |         |  |     |  |         |  | |
//   | |  |---------|  |     |  |---------|  | |
//   | |  |---------|  | ... |  |---------|  | |
//   | |  |---------|  |     |  |---------|  | |
//   | |  +---------+  |     |  +---------+  | |
//   | |       |       |     |       |       | |
//   | +-------|-------+     +-------|-------+ |
//   |         |                     |         |
//   |         v                     v         |
//   | +-------------------------------------+ |
//   |  \          dpi_event_select         /  |
//   |   +---------------------------------+   |
//   |                    |                    |
//   |                    |                    |
//   |               |    v    |               |
//   |               |---------|               |
//   | fast clock -->|---------|               |
//   |               |---------|               |
//   |               +---------+               |
//   |                    |                    |
//   +--------------------|--------------------+
//                        |
//                        |
//                        v
//            fast clock --> DPI call
//
//==================================================================================================
// Note that this module could also contain a FIFO, but it is unnecessary because it can perform a
// DPI call on every cycle.  Also note that the FIFOs that capture raw CPU events (in
// retire_event_fifo and vpu_event_fifo, for example) run off of the appropriate CPU clock, but all
// other FIFOs run with the fastest clock in the system.  This means that the raw CPU event FIFOs
// can empty at a faster rate than they fill (although because emptying these queues is time-sliced,
// they can still overflow).
//
// Output data type: uevent_neigh_packet
// Local data type:  uevent_neigh_packet
//
module sp_neigh_event_fifo #(parameter int SHIRE_ID = 254) (
      input wire reset
   );

   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
      import evl_base_pkg::*;
   `endif // ifdef EVL_SIMULATION

   localparam int EVENT_COUNT  = 1;
   localparam int FIFO_DEPTH   = 1;
   localparam int SHIRE_SELECT = (SHIRE_ID > 33) ? 34 : SHIRE_ID;

   wire                                  rd_clk = top.clk_max;
   wire [EVENT_COUNT-1:0]                minion_event_valid;
   uevent_neigh_packet [EVENT_COUNT-1:0] minion_event_data;
   wire [EVENT_COUNT-1:0]                minion_event_pop;
   wire                                  neigh_event_empty;
   uevent_neigh_packet                   neigh_event_data_out;
   bit                                   enable;

   always_ff @(posedge rd_clk) begin
      if ((reset !== 1'b0) || (enable !== 1'b1)) begin
      end
      else if (neigh_event_empty === 1'b0) begin
         uevent_send_neigh_packet(neigh_event_data_out);
      end
      enable <= ~top.m_shire_disable[SHIRE_SELECT];
   end


   //===============================================================================================
   //
   // Minion Event FIFO Connections
   //
   assign minion_event_valid[0]            = ~minion_event_fifo.minion_event_empty;
   assign minion_event_data[0].event_type  = minion_event_fifo.minion_event_data_out.event_type;
   assign minion_event_data[0].data.minion = minion_event_fifo.minion_event_data_out.data;

   assign minion_event_fifo.minion_event_pop = minion_event_pop[0];


   //===============================================================================================
   //
   // Neighborhood Mux (and FIFO, if used)
   //
   wire                neigh_fifo_full;
   wire                select_valid_out;
   uevent_neigh_packet select_data_out;


   dpi_event_select #(
      .EVENT_COUNT(EVENT_COUNT),
      .DATA_MSB(`UEVENT_NEIGH_PACKET_MSB))
      dpi_event_select (
         .reset     (reset),
         .clk       (rd_clk),
         .full_in   (neigh_fifo_full),
         .valid_in  (minion_event_valid),
         .data_in   (minion_event_data),
         .pop_out   (minion_event_pop),
         .valid_out (select_valid_out),
         .data_out  (select_data_out)
      );


   generate
      if (FIFO_DEPTH <= 1) begin : fifo

         assign neigh_fifo_full      = 1'b0;
         assign neigh_event_empty    = ~select_valid_out;
         assign neigh_event_data_out = select_data_out;

      end
      else begin : fifo

         reg neigh_event_pop;

         always_ff @(posedge rd_clk) begin
            if ((reset !== 1'b0) || (enable !== 1'b1)) begin
               neigh_event_pop <= 1'b0;
            end
            else begin
               neigh_event_pop <= ~neigh_event_empty;
            end
         end

         dpi_event_capture #(
            .DEPTH(FIFO_DEPTH),
            .DATA_MSB(`UEVENT_NEIGH_PACKET_MSB),
            .HIGH_WATERMARK_8THS(8),
            .LOW_WATERMARK_8THS(7))
            dpi_event_capture (
               .reset           (reset),
               .wr_clk          (rd_clk),
               .rd_clk          (rd_clk),
               .enable          (enable),
               .valid_in        (select_valid_out),
               .data_in         (select_data_out),
               .pop_in          (neigh_event_pop),
               .data_out        (neigh_event_data_out),
               .empty_out       (neigh_event_empty),
               .nearly_full_out (neigh_fifo_full),
               .full_out        ()
            );

      end
   endgenerate

endmodule

`include "soc.vh"

`include "uevent_neigh.vh"

//==================================================================================================
//
// Neighborhood Pending DPI Event FIFO
//
// This module is used to collect pending DPI events from each of the instantiated Minions.  The
// basic structure is as follows:
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
// Note that this module could also contain a FIFO, but it is unnecessary because it can
// perform a DPI call on every cycle.  Also note that the FIFOs that capture raw CPU events (in
// retire_event_fifo and vpu_event_fifo, for example) run off of the appropriate CPU clock, but all
// other FIFOs run with the fastest clock in the system.  This means that the raw CPU event FIFOs
// can empty at a faster rate than they fill (although because emptying these queues is time-sliced,
// they can still overflow).
//
// Output data type: uevent_neigh_packet
// Local data type:  uevent_neigh_packet
module neigh_event_fifo #(parameter int              SHIRE_ID        = 0,
                          parameter int              NEIGH_ID        = 0,
                          parameter [`MIN_PER_N-1:0] DISABLE_MINIONS = {`MIN_PER_N{1'b0}},
                          parameter [`MIN_PER_N-1:0] STUB_MINIONS    = {`MIN_PER_N{1'b0}}) (
      input wire                        reset_c_shire,
      input wire [`NUM_SHIRE_IDS_L-1:0] shire_id,
      input wire [`NUM_NEIGH_L-1:0]     neigh_id
   );

   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
      import evl_base_pkg::*;
   `endif // ifdef EVL_SIMULATION

   localparam [`MIN_PER_N-1:0] MINION_IGNORE = (DISABLE_MINIONS | STUB_MINIONS);
   localparam int              EVENT_COUNT   = `MIN_PER_N;
   localparam int              FIFO_DEPTH    = 1;
   localparam int              SHIRE_SELECT  = (SHIRE_ID > 33) ? 34 : SHIRE_ID;

   wire                                  rd_clk = top.clk_max;
   wire [EVENT_COUNT-1:0]                minion_event_valid;
   uevent_neigh_packet [EVENT_COUNT-1:0] minion_event_data;
   wire [EVENT_COUNT-1:0]                minion_event_pop;
   wire                                  neigh_event_empty;
   uevent_neigh_packet                   neigh_event_data_out;
   bit                                   enable;

   always_ff @(posedge rd_clk) begin
      if ((reset_c_shire !== 1'b0) || (enable !== 1'b1)) begin
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
   genvar gen_it;

   generate
      for (gen_it = 0; gen_it < `MIN_PER_N; gen_it++) begin : g_connect
         if (MINION_IGNORE[gen_it] == 1'b0) begin : g_connect
            assign minion_event_valid[gen_it] = ~neigh_top.gen_minions[gen_it].min_inst.minion.minion_event_fifo.minion_event_empty;
            assign minion_event_data[gen_it].event_type =
               neigh_top.gen_minions[gen_it].min_inst.minion.minion_event_fifo.minion_event_data_out.event_type;
            assign minion_event_data[gen_it].data.minion =
               neigh_top.gen_minions[gen_it].min_inst.minion.minion_event_fifo.minion_event_data_out.data;

            assign neigh_top.gen_minions[gen_it].min_inst.minion.minion_event_fifo.minion_event_pop = minion_event_pop[gen_it];
         end
         else begin : g_connect
            assign minion_event_valid[gen_it] = 1'b0;
            assign minion_event_data[gen_it]  = {`UEVENT_NEIGH_PACKET_SIZE{1'b0}};
         end
      end
   endgenerate


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
         .reset     (reset_c_shire),
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
            if ((reset_c_shire !== 1'b0) || (enable !== 1'b1)) begin
               neigh_event_pop <= 1'b0;
            end
            else begin
               neigh_event_pop <= ~neigh_event_empty;
            end
         end

         dpi_event_capture #(
            .DEPTH(FIFO_DEPTH),
            .DATA_MSB(`UEVENT_NEIGH_PACKET_MSB),
            .HIGH_WATERMARK_8THS(7),
            .LOW_WATERMARK_8THS(6))
            dpi_event_capture (
               .reset           (reset_c_shire),
               .wr_clk          (rd_clk),
               .rd_clk          (rd_clk),
               .enable          (enable),
               .valid_in        (select_valid_out),
               .data_in         (select_data_out),
               .pop_in          (neigh_event_pop),
               .data_out        (neigh_event_data_out),
               .empty_out       (neigh_event_empty),
               .nearly_full_out (),
               .full_out        (neigh_fifo_full)
            );

      end
   endgenerate

   wire unused_ok = &{ 1'b0,
                       shire_id,
                       neigh_id,
                       1'b0 };

endmodule

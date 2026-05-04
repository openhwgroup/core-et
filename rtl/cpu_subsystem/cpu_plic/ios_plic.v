// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// ios_plic.v
// Derived from freechips.rocketchip.devices.tilelink.TLPLIC in file
//   rocket-chip/src/main/scala/devices/tilelink/Plic.scala
//
// This module is a Platform Level Interrupt Controller.
//
// There are N_EXTINT interrupt_req_from_src. The number must be 2^n-1.
//
// The interrupt_req_from_src has a priority order. The smaller the index,
// the higher the priority. There is a null-interrupt added.
// The null-interrupt will never be asserted.
//
// Each interrupt has a priority that can is programmable.
//
// Each interrupt_req_to_target output goes to an interrupt target, which is a hart.
// Each hart has a programmable threshold. It also has an enable bit for each
// of the interrupt source.
//
// When an interrupt input is asserted, multiple of no interrupt request
// output may be asserted. Each hart choose the winning interrupt input based
// on the following criteria:
//
//   1. The interrupt input which is asserted wins over the ones that are not
//      not asserted.
//   2. When both interrupt_req_from_src are asserted, the higher priority interrupt wins
//      over the lower priority interrupt.
//   3. When both asserted interrupt_req_from_src have the same priority, the interrupt
//      with lower index wins.
//   4. If the winning interrupt has the priority greater than (or equal-to?)
//      the threshold, the corresponding interrupt_req_to_target output for the hart is
//      asserted.
//
// PLIC occupies the address space in the memory map in the range:
//
//   ET-SOC:
//       40'h_00_2000_0000 to
//       40'h_00_23FF_FFFF
//
//   Rocket-chip:
//       28'h_C00_0000 to
//       28'h_FFF_FFFF
//

`include "soc.vh"

module ios_plic
  #(
    parameter maxPriorities = 7,
    parameter N_EXTINT = 2,
    parameter N_EXTINT_TARGETS = 2
    )
   (
    input logic                           clock,
    input logic                           resetn,

    // No a_param because this is TL-UL where a_param is always reserved, must
    // be 0.
    output logic                          a_ready,
    input                                 a_valid,
    input [`M_A_OPCODE_WIDTH-1:0]         a_opcode,
    input [`M_A_SIZE_WIDTH-1:0]           a_size,
    input [`M_A_SOURCE_WIDTH-1:0]         a_source,
    input [`M_PLIC_ADDR_WIDTH-1:0]        a_address,
    input [`M_A_MASK_WIDTH_32-1:0]        a_mask, // ri lint_check_waive INPUT_NOT_READ
    input [`M_A_DATA_WIDTH_32-1:0]        a_data,

    // (a) No d_param because this is TL-UL where d_param is always reserved, must
    // be 0.
    //
    // (b) No d_sink because this is TL-UL where d_sink is always ignored. It can be
    // any value. So, it will be optimized away anyway.
    //
    // (c) No d_error because (1) this module does not flag any error, and (2) the
    // TL2 spec states that the handling of the assertion of d_error is
    // implementation dependent. This implementation does not handle d_error so
    // there is no need to flag an error.
    input                                 d_ready,
    output logic                          d_valid,
    output logic [`M_D_OPCODE_WIDTH-1:0]  d_opcode,
    output logic [`M_D_SIZE_WIDTH-1:0]    d_size,
    output logic [`M_D_SOURCE_WIDTH-1:0]  d_source,
    output logic [`M_D_DATA_WIDTH_32-1:0] d_data,

    input [N_EXTINT-1:0]                  interrupt_req_from_src,
    output [N_EXTINT_TARGETS-1:0]         interrupt_req_to_target
    );

   // The localparam s are derived from object PLICConsts.
   // localparam maxDevices       = 1023;     // 'h3FF
   // localparam maxHarts         = 15872;    // 'h3E00
   // localparam priorityBase     = 'h0;
   localparam pendingBase      = 'h1000;
   localparam enableBase       = 'h2000;
   localparam hartBase         = 'h20_0000;

   // localparam claimOffset      = 4;
   // localparam priorityBytes    = 4;


   // N_EXTINT       - # of interrupt sources including default interrupt 0
   // nDevices       - # of interrupt sources excluding default interrupt 0
   // DEV_ID_WIDTH   - # of bits needed to represent the device/interrupt ID
   // nDeviceRegs    - # of registers needed to hold all devices where:
   //                  1. we devote 1 bit/device
   //                  2. each reg has a maximum of 32 bits
   // nDeviceRegBits - # of total bits accross all device registers (i.e. closest
   //                  multiple of 32 that is >= N_EXTINT)
   localparam nDevices         = N_EXTINT-1;
   localparam nPriorities      = (nDevices > maxPriorities) ? maxPriorities : nDevices;
   localparam nHarts           = N_EXTINT_TARGETS;
   localparam PRI_WIDTH        = $clog2(nPriorities+1);
   localparam DEV_ID_WIDTH     = $clog2(nDevices+1);
   localparam PRIORITY_WIDTH   = nPriorities > 0 ? PRI_WIDTH : 1;
   localparam nDeviceRegs      = (((nDevices+1)%`M_A_DATA_WIDTH_32) > 0) ? ((nDevices+1)/`M_A_DATA_WIDTH_32)+1 :
              ((nDevices+1)/`M_A_DATA_WIDTH_32);
   localparam nDeviceRegBits   = nDeviceRegs * `M_A_DATA_WIDTH_32;

   // Used by the binary search code to find the highest priority interrupt.
   // The number of inputs to the search must be a power of 2.
   localparam nDevicesPow2  = (2 ** DEV_ID_WIDTH) - 1;


   logic [nDevices:0] pending;
   logic [nDeviceRegBits-1:0] pending_regs;

   // enables is a register which holds the per target enable bits.  The bits
   //  beyond nDevices are not used.  They are there to pad enables width to be
   //  a multiple of 32.  We wanted the width to be a multpile of 32 to make
   //  the verilog code a little simpler.  One implication of this simplification
   //  is that the bits beyond nDevices can get written.  However they are never
   //  accessed.  See enabled_regs.
   //
   // enables_regs is a copy of enables except that the unused bits beyond nDevices
   //  are zeroed out.  enabled_regs is what is used when we need to read the enable
   //  bits
   logic [nDeviceRegBits-1:0] enables[nHarts-1:0];
   logic [nDeviceRegBits-1:0] enables_regs[nHarts-1:0];

   logic [DEV_ID_WIDTH-1:0]   maxDevs[nHarts-1:0];
   logic [DEV_ID_WIDTH-1:0]   maxDev[nHarts-1:0];
   logic [PRIORITY_WIDTH:0]   maxPri[nHarts-1:0];
   logic [PRIORITY_WIDTH:0]   maxPri_f[nHarts-1:0];
   logic                      irq[nHarts-1:0];            // ri lint_check_waive ONE_BIT_MEM_WIDTH

   logic [nDevices:0]         claimedDevs;
   logic [nDevices:0]         io_plic_complete;

   logic [`M_PLIC_ADDR_WIDTH-1:0] a_address_i;
   logic [`M_A_DATA_WIDTH_32-1:0] a_data_i;
   logic                          read;
   logic                          write;

   logic [PRIORITY_WIDTH-1:0]     d_data_priority;
   logic [PRIORITY_WIDTH-1:0]     d_data_threshold;
   logic [`M_A_DATA_WIDTH_32-1:0] d_data_enables;
   logic [`M_A_DATA_WIDTH_32-1:0] d_data_pending;
   logic [DEV_ID_WIDTH-1:0]       d_data_maxDevs;
   logic [PRIORITY_WIDTH-1:0]     pri[nDevices:0];
   logic [nHarts-1:0]             harts;
   logic [PRIORITY_WIDTH-1:0]     threshold[nHarts-1:0];


   

   genvar                         i,j,k;


   // pending has one bit per interrupt source.
   // pending_regs has one bit per interrupt source rounded up to the closest multiple
   //  of 32.  It represents the set of pending registers.  The extra padding bits are
   //  set to 0.
   generate
      for (i = 0; i < N_EXTINT; i++) begin : gen_pending_regs
         assign pending_regs[i] = pending[i];
      end

      for (i = N_EXTINT; i < nDeviceRegBits; i++) begin : gen_pending_regs_pad
         assign pending_regs[i] = '0;
      end
   endgenerate



   // Each hart has one set of enable bits.
   // Each enables vector has one bit per interrupt source.
   // Each enables_regs has one bit per interrupt source rounded up to the closest multiple
   //  of 32.  It represents the set of interrupt enable registers.  The extra padding bits
   //  are set to 0.
   generate
      for (i = 0; i < nHarts; i++) begin : gen_enables_regs_hart
         for (j = 0; j < N_EXTINT; j++) begin : gen_enables_regs
            assign enables_regs[i][j] = enables[i][j];
         end

         for (j = N_EXTINT; j < nDeviceRegBits; j++) begin : gen_enables_regs_pad
            assign enables_regs[i][j] = '0;
         end
      end
   endgenerate


   // An interrupt is in flight if its is source is sending an interrupt request
   // and a target has claimed it.  The interrupt remains in flight until the
   // interrupt target signals that it has been completed.
   reg [nDevices:0] inFlight;

   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
         inFlight <= 'b0;
      end
      else begin
         inFlight <= ~io_plic_complete & (((interrupt_req_from_src | pending ) & claimedDevs) | inFlight);
      end
   end

   // A interrupt request is considered valid (i.e. will set the corresponding
   // interrupt pending bit) if no earlier instance of that insterrupt is still
   // in flight (i.e. has yet to be completed).
   wire [nDevices:0]                interrupt_valid;
   assign interrupt_valid = interrupt_req_from_src & ~inFlight;


   // Logic for managing TL-UL interface to registers.
   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
         a_ready <= 1'b0;
      end else begin
         a_ready <= a_valid;
      end
   end

   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
         a_address_i <= '0;
         a_data_i    <= '0;
         d_source    <= '0;
         d_size      <= '0;
         read        <= '0;
         write       <= '0;
      end
      else begin
         if (a_valid) begin
            a_address_i <= a_address;
            a_data_i    <= a_data;
            d_source    <= a_source;
            d_size      <= a_size;

            read        <= (a_opcode == `M_Get);
            write       <= (a_opcode == `M_PutFullData);
         end
         else begin
            read        <= '0;
            write       <= '0;
         end // else: !if(a_valid)
      end // else: !if(!resetn)
   end


   // Write to the per-source priority registers
   generate

      if (nPriorities > 0) begin : gen_priority_gt0

         // Source 0 is special. It is read-only, reserved.
         assign pri[0] = {PRIORITY_WIDTH{1'b0}};

         for (i = 1; i <= nDevices; i++) begin : gen_pri
            always @(posedge clock or negedge resetn) begin
               if (!resetn) begin
                  pri[i] <= '0;
               end
               else begin
                  if (write && (a_address_i == (i << 2))) begin
                     pri[i] <= a_data_i[PRIORITY_WIDTH-1:0];
                  end
               end
            end
         end

      end else begin : gen_priority_0
         // nPriorities == 0
         assign pri[0] = '0;
         for (i = 1; i <= nDevices; i++) begin : gen_pri0
            assign pri[i] =  '1;
         end
      end

   endgenerate

   // Write to the per-target threshold registers
   generate
      if (nPriorities > 0) begin : gen_threshold_gt0

         for (i = 0; i < nHarts; i++) begin : gen_thr
            wire addr_match = a_address_i == (hartBase + (i << 12));

            always @(posedge clock or negedge resetn) begin
               if (!resetn) begin
                  threshold[i] <= '0;
               end
               else if (write && addr_match) begin
                  threshold[i] <= a_data_i[PRIORITY_WIDTH-1:0];
               end
            end
         end

      end else begin : gen_threshold_0
         for (i = 0; i < nHarts; i++) begin : gen_thr0
            assign threshold[i] = '0;
         end
      end

   endgenerate

   // Write to the per-target enable registers
   generate
      for (i = 0; i < nHarts; i++) begin : gen_en_tgt
         for (j = 0; j < nDevices+1; j = j+`M_A_DATA_WIDTH_32) begin : gen_en_reg
            wire addr_match = (a_address_i == (enableBase + (i << 7) + (j >> 3)));

            always @(posedge clock or negedge resetn) begin
               if (!resetn) begin
                  enables[i][j+:`M_A_DATA_WIDTH_32] <= 'b0;
               end
               else if (write && addr_match) begin
                  enables[i][j +: `M_A_DATA_WIDTH_32] <= a_data_i;
               end // else: !if(!resetn)
            end // always @ (posedge clock or negedge resetn)
         end // block: gen_en_reg
      end // block: gen_en_tgt
   endgenerate


   // Determine if there was a write to the per-target maxDevs register.
   wire [nHarts-1:0]                max_devs_wen;

   generate
      for (i = 0; i < nHarts; i++) begin : gen_wr_max_devs
         wire addr_match = (a_address_i == (hartBase + (i << 12) + 'h4));
         assign max_devs_wen[i] = write && addr_match;
      end
   endgenerate

   // A write to any of the maxDevs registers triggers the completion of an interrupt.
   // The interrupt completed is specified by the write data.
   wire                            max_devs_write;
   assign max_devs_write = |max_devs_wen;

   generate
      for (i = 0; i <= nDevices; i++) begin : gen_compl
         wire intr_match;

         assign intr_match          = (a_data_i == i);
         assign io_plic_complete[i] = max_devs_write && intr_match;
      end
   endgenerate

   // An interrupt's pending bit is set when an interrupt request arrives for it
   // and no earlier instances of that interrupt are still in flight.  It remains
   // pending until an interrupt target has claimed it.
   always @(posedge clock or negedge resetn) begin
      if (!resetn)
        pending <= 'b0;
      else
        pending <= ~claimedDevs & (interrupt_valid | pending);
   end

   generate
      for (i = 0; i < nHarts; i++) begin : gen_harts

         logic [PRIORITY_WIDTH:0]  p_en_pri[nDevicesPow2:0];
         logic [nDevices:0]        en;

         assign en = enables_regs[i][nDevices:0];

         // First, generate maxPri and MaxDev.
         //
         // Generate vector p_en_pri. There are nDevices+1 elements in the array.
         // The number of elements must be a power of 2 because a binary search
         // will be done on it later.
         for (j = 0; j <= nDevicesPow2; j++) begin : gen_p_en_pri
            if (j == 0)             assign p_en_pri[j] = {1'b1, {PRIORITY_WIDTH {1'b0}}};
            else if (j <= nDevices) assign p_en_pri[j] = {(pending[j] && en[j]), pri[j]};
            else                    assign p_en_pri[j] = {1'b0, {PRIORITY_WIDTH {1'b0}}};
         end // for (genvar j ...)

         // Generate maxPri for each hart
         //
         // Do a binary search for the largest value of p_en_pri.
         // It will be mapped to a set of muxes with DEV_ID_WIDTH number of
         // levels.
         // TBD POW2
         logic [PRIORITY_WIDTH:0]  max_pri[DEV_ID_WIDTH:0][nDevicesPow2:0];
         logic [DEV_ID_WIDTH-1:0]  max_dev[DEV_ID_WIDTH:0][nDevicesPow2:0];

         // Assign p_en_pri to the inputs of the first-level muxes.
         for (j = 0; j <= nDevicesPow2; j++) begin : gen_max_pri_0
            assign max_pri[0][j] = p_en_pri[j];
         end

         // Assign original IDs to the first-level max_dev.
         for (j = 0; j <= nDevicesPow2; j++) begin : gen_max_dev_0
            assign max_dev[0][j] = j;
         end

         for (j = 0; j < DEV_ID_WIDTH; j++) begin : gen_max_pri_dev
            for (k = 0; k < 2 ** (DEV_ID_WIDTH-j-1); k++) begin : gen_max_pri
               assign {max_pri[j+1][k], max_dev[j+1][k]} =
                                                          (max_pri[j][k*2] >= max_pri[j][k*2+1]) ? {max_pri[j][k*2],   max_dev[j][k*2]} :
                                                          {max_pri[j][k*2+1], max_dev[j][k*2+1]};
            end // for (genvar k ...)
            // For the unused array elements, assign them to 0 so that the
            // synthesizer will optimize them away, and without raising a flag.
            for (k = 2 ** (DEV_ID_WIDTH-j-1); k <= nDevicesPow2; k++) begin : gen_max_pri_dev_null
               assign {max_pri[j+1][k], max_dev[j+1][k]} = { {(DEV_ID_WIDTH+1){1'b0}}, {(DEV_ID_WIDTH){1'b0}} };
            end // for (genvar k ...)
         end // for (genvar j ...)

         assign maxPri[i] = max_pri[DEV_ID_WIDTH][0];
         assign maxDev[i] = max_dev[DEV_ID_WIDTH][0];

         always @(posedge clock) begin
            maxPri_f[i] <= maxPri[i];
         end

         always @(posedge clock) begin
            maxDevs[i] <= maxDev[i];
         end

         assign irq[i] = (maxPri_f[i] > {1'b1, threshold[i]});

         always @(posedge clock) begin
            harts[i] <= irq[i];
         end

      end // for (genvar i ...)
   endgenerate

   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
         d_valid <= 1'b0;
      end else begin
         if (~d_valid) begin
            if (a_valid) begin
               d_valid <= 1'b1;
            end
         end else if (d_ready) begin
            d_valid <= 1'b0;
         end
      end
   end

   assign d_opcode = d_valid ? (write ? `M_AccessAck : `M_AccessAckData)
     : 'h0;


   always_comb begin
      if (d_valid && read) begin
         // Read the per-source priority registers
         d_data_priority = 'h0;

         for (int m = 1; m <= nDevices; m++) begin : gen_rd_pri
            if (a_address_i == (m << 2)) begin
               d_data_priority = d_data_priority | pri[m];
            end
         end

         // Read the per-target threshold registers
         d_data_threshold = 'h0;

         for (int m = 0; m < nHarts; m++) begin : gen_rd_thr
            if (a_address_i == (hartBase + (m << 12))) begin
               d_data_threshold = d_data_threshold | threshold[m];
            end
         end

         // Read the per-target enable registers
         d_data_enables = 'h0;

         for (int m = 0; m < nHarts; m++) begin : gen_rd_en_tgt
            for (int n = 0; n < nDevices+1; n = n +`M_A_DATA_WIDTH_32) begin : gen_rd_en_src
               if (a_address_i == (enableBase + (m << 7) + (n >> 3))) begin
                  d_data_enables = d_data_enables | enables_regs[m][n +: `M_A_DATA_WIDTH_32];
               end
            end // for n
         end // for m

         // Read the per-source pending register
         d_data_pending = 'h0;

         for (int m = 0; m < nDevices+1; m = m +`M_A_DATA_WIDTH_32) begin : gen_rd_pending
            if (a_address_i == (pendingBase + (m >> 3))) begin
               d_data_pending = d_data_pending | pending_regs[m +: `M_A_DATA_WIDTH_32];
            end
         end // for m

         // Read the per-target maxDevs register
         d_data_maxDevs = 'h0;

         for (int m = 0; m < nHarts; m++) begin : gen_rd_maxDevs
            if (a_address_i == (hartBase + (m << 12) + 'h4)) begin
               d_data_maxDevs = d_data_maxDevs | maxDevs[m];
            end
         end // block: gen_rd_maxDevs


         for (int m = 0; m <= nDevices; m++) begin : gen_claim
            claimedDevs[m] = d_data_maxDevs == m;
         end

         d_data = {{(`M_A_DATA_WIDTH_32-PRIORITY_WIDTH){1'b0}}, d_data_priority} |
                  {{(`M_A_DATA_WIDTH_32-PRIORITY_WIDTH){1'b0}}, d_data_threshold} |
                  d_data_enables |
                  d_data_pending |
                  {{(`M_A_DATA_WIDTH_32-DEV_ID_WIDTH){1'b0}}, d_data_maxDevs};

      end else begin
         // Need this assignment to prevent the inference of a latch.  We want this
         // signal to be combinational logic (or else it will be x coming out of reset
         // due to the absence of a reset block).
         claimedDevs      = 'h0;

         // Don't think I need these assignments but they might prevent synthesis from
         // inferring unnecessary latches.
         d_data           = 'h0;
         d_data_priority  = 'h0;
         d_data_threshold = 'h0;
         d_data_enables   = 'h0;
         d_data_pending   = 'h0;
         d_data_maxDevs   = 'h0;
      end
   end

   assign interrupt_req_to_target = harts;

endmodule // ios_plic

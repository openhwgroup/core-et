// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Common et_cdc_sync synchronizer
// 
//     Active high asynchronous (set/clr - based on RST_VAL) SYNC_STAGES-synchronizer
//     NOTE: even though the reset causes an async set/clr, the reset signal is 
//           expected to be externally synchronized to "clock" prior to being
//           applied to this module
//     
//     If not needing reset flops within this module, just tie reset low
//     this means that junk data will come out of the synchronizer for SYNC_STAGES clocks
//     before the output is valid
//     
//     To simulate with actual standard cell gates use this define: ET_LIBS_USE_STD_CELLS
//     
//     When simulating with the behavioral model (i.e. not std cells), there is behavioral 
//     jitter modeling within this module.  To simulate with jitter, use 
//     the plusarg: et_cdc_sync_jitter_enable=1
// 

`include "libs_defines.vh"
`include "macros.vh"
`include "shire_assert.vh"

module et_cdc_sync #(parameter
   WIDTH       = 1,   // width of data
   SYNC_STAGES = 2,   // number of sync stages
   RESET_VAL   = 0    // reset value: 0 = reset cdc flops to zero, 1 = reset cdc flops to one
) (
   input  logic              clock,
   input  logic              reset,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);


`ifdef ET_LIBS_USE_STD_CELLS
   et_cdc_sync_grp_cells #(.WIDTH(WIDTH), .SYNC_STAGES(SYNC_STAGES), .RESET_VAL(RESET_VAL)) ET_GROUP_CELLS_et_cdc_sync_grp_cells (.clock (clock), .reset (reset), .din (din), .dout (dout));
`else
   // simple behavioral model
   logic [WIDTH-1:0] et_cdc_sync_meta_flops [SYNC_STAGES-1:0];
   always @(posedge clock or posedge reset) begin
      if (reset) begin
         for (int unsigned ii=0; ii<SYNC_STAGES; ii++) begin
            et_cdc_sync_meta_flops[ii] <= (RESET_VAL == 1) ? '1 : '0;
         end
      end 
      else begin
         for (int unsigned ii=0; ii<SYNC_STAGES; ii++) begin
            if (ii == 0) begin
               et_cdc_sync_meta_flops[0] <= din;
            end
            else begin
               et_cdc_sync_meta_flops[ii] <= et_cdc_sync_meta_flops[ii-1];
            end
         end
      end
   end

   // select dout between "simple behavioral" and "jitter"
   `ifdef EVL_SIMULATION
      bit               dout_jitter_sel;
      logic [WIDTH-1:0] dout_jitter;
   `else
      wire              dout_jitter_sel = 1'b0;
      wire [WIDTH-1:0]  dout_jitter = {WIDTH {1'b0}};
   `endif
   assign dout = dout_jitter_sel ? dout_jitter : et_cdc_sync_meta_flops[SYNC_STAGES-1];


   `ifdef EVL_SIMULATION
      // jitter modeling (to enable use plusarg: +et_cdc_sync_jitter_enable=1)
      // 
      // maintain din_prev, din_cur for selection in below jitter always block
      // need to select between the current and the last data change not between
      // the current and the capture data change because the write clock
      // could be much faster than the read clock (i.e. sync clock)
   
      string       inst_name;
      int unsigned inst_checksum;
      bit          inst_checksum_val;
      bit          nominal_late;
      int          value;
      initial begin
         if (`et_value_plusargs("et_cdc_sync_jitter_enable=%d", value) == 0) begin
            value = 0;
         end
         dout_jitter_sel = (value == 1) ? 1'b1 : 1'b0;
         #1;
         // determine nominal_late first before applying instance based randomization
         // this is required for array of instance based instantiations
         randcase
            1: nominal_late = 0; // jitter between nominal and nominal_minus1
            1: nominal_late = 1; // jitter between nominal and nominal_late
         endcase
         // $write("%m nominal_late=%0d\n", nominal_late); 
         inst_name = $sformatf("%m");
         for (int unsigned ii=0; ii<inst_name.len(); ii++) begin
            // $write("%m ii=%0d char=%s 0x%0x\n", ii, inst_name.getc(ii), inst_name.getc(ii)); 
            inst_checksum += 1000*inst_name.getc(ii);
         end
         inst_checksum_val <= 1;
      end
   
   
      // din_changed, din_cur, din_prev
      // -----------------------------------------------------------------------
      bit               din_init;
      bit               din_changed;
      logic [WIDTH-1:0] din_cur;
      logic [WIDTH-1:0] din_prev;
      always @ (din) begin
         if (!din_init) begin
            // set all din_* to din
            din_init     = 1'b1; 
            din_cur     <= din;
            din_prev    <= din;
            din_changed <= dout_jitter_sel;
            // $write("%0t 0. din_init:  din_cur=0x%08x din_prev=0x%08x\n", $realtime(), din, din);
         end
         else begin
            din_cur     <= din;
            din_prev    <= (reset) ? din : din_cur;
            din_changed <= dout_jitter_sel;
            // $write("%0t 1. din_check: din_cur=0x%08x din_prev=0x%08x\n", $realtime(), din, din_cur);
         end
      end
   
      // bit               rc_leg;
      bit               srandom_init;
      logic [WIDTH-1:0] jitter_d1;
      logic [WIDTH-1:0] jitter_d2;
      logic [WIDTH-1:0] jitter_d2_sel;
      logic [WIDTH-1:0] jitter_d3_sel;
      logic [WIDTH-1:0] jitter_d4_sel;
      logic [WIDTH-1:0] jitter_d5_sel;
      always @(posedge clock, posedge reset) begin
         if (dout_jitter_sel) begin                                 
            if (inst_checksum_val && !srandom_init) begin
               // set instance based random seed
               srandom_init = 1'b1;
               // $write("%m $srandom(0x%0x)\n", inst_checksum); 
               process::self.srandom(inst_checksum);
            end
            if (reset) begin
                 jitter_d1     <= (RESET_VAL == 1) ? '1 : '0;
                 jitter_d2     <= (RESET_VAL == 1) ? '1 : '0;
                 jitter_d3_sel <= (RESET_VAL == 1) ? '1 : '0;
                 jitter_d4_sel <= (RESET_VAL == 1) ? '1 : '0;
                 jitter_d5_sel <= (RESET_VAL == 1) ? '1 : '0;
            end
            else begin
               // $write("%0t 2. seq_check: din_cur=0x%08x din_prev=0x%08x\n", $realtime(), din_cur, din_prev);
               if (din_changed) begin  
                  din_changed = 1'b0; // clear with blocking assignment so above logic can reset din_changed if din changes on same clock cycle
                  // update jitter_d1 with mix of din_non_blocking
                  for (int unsigned ii=0; ii<WIDTH; ii++) begin
                     randcase 
                        1: begin 
                           jitter_d1[ii] <=  din_cur[ii];  // jitter_d1 takes cur
                           // rc_leg = 0; 
                        end 
                        1: begin 
                           jitter_d1[ii] <=  din_prev[ii]; // jitter_d1 takes prev
                           // rc_leg = 1; 
                        end
                     endcase
                  end
                  // $write("(inst_name=%s, 0x%0x) => rcl=%0d r=0x%0x u=0x%0x\n", inst_name, inst_checksum, rc_leg, $random(), $urandom());
               end
               else begin 
                  // update all bits of data in jitter_d1 to din_cur to get all bits to have the data that has been static on the input of the synchronizer
                  jitter_d1 <=  din_cur; 
               end
               // pipeline jitter_d1 for nominal_late
               jitter_d2 <= jitter_d1; 
               // pipeline along jitter_d2_sel for SYNC_STAGES>2
               jitter_d3_sel <= jitter_d2_sel;
               jitter_d4_sel <= jitter_d3_sel;
               jitter_d5_sel <= jitter_d4_sel;
            end
         end
      end
   
      // select jitter output between nominal_early => (nominal-1:nominal) -> jitter_d1 (jitter between 1 to 2 cycles of delay)
      //                      and     nomial_late   => (nominal:nominal+1) -> jitter_d2 (jitter between 2 to 3 cycles of delay)
      assign jitter_d2_sel = (nominal_late) ? jitter_d2 : jitter_d1;
   
      assign dout_jitter = 
         (SYNC_STAGES == 2) ? jitter_d2_sel :
         (SYNC_STAGES == 3) ? jitter_d3_sel :
         (SYNC_STAGES == 4) ? jitter_d4_sel :
         (SYNC_STAGES == 5) ? jitter_d5_sel : 'x; 
   
   
   `endif // EVL_SIMULATION

`endif  // ET_LIBS_USE_STD_CELLS


// Per Snps data, SYNC_STAGES==2 for all PD >= 585mV.  
// Therefore lock this module to use only the value of 2.
// and verify with assertion at falling edge of reset
`ifndef ET_ASSERT_OFF
   logic [1:0] check;
   initial begin
      check = '1;
   end
   wire en_check = |check;
   `EN_FF(clock, en_check, check, (check << 1))
   `ASSERT_ALWAYS_MSG(ERROR_ET_CDC_SYNC_sync_stages_check, clock, check[0], en_check |-> (en_check && (SYNC_STAGES == 2)), $sformatf("*** ERROR *** et_cdc_sync should be set to 2 sync_stages: SYNC_STAGES=%0d\n", $sampled(SYNC_STAGES)) )
`endif

endmodule


`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_cdc_sync_grp_cells #(parameter
   WIDTH       = 1,   // width of data
   SYNC_STAGES = 2,   // number of sync stages
   RESET_VAL   = 0    // reset value: 0 = reset cdc flops to zero, 1 = reset cdc flops to one
) (
   input  logic              clock,
   input  logic              reset,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

   // standard cell gate implementation
   //   recommended std cell list:
   //     plain 2-1 mux:                              HDBULT11_MUX2_MM_2 
   //     glitch-less clock mux:                      HDBULT11_MUX2_CK_2
   //     inverter:                                   HDBULT11_INV_1
   //     posedge, active low async clear scan flop:  HDBULT08_FSDPRBQ_CBV2Y2_1
   //     posedge, active low async set   scan flop:  HDBULT08_FSDPSBQ_V2Y2_1
   // 
   generate
      if (1) begin : ET_DONT_TOUCH_et_cdc_sync_wrap
         // std cells require active low async clear/set so need to invert the incoming active high reset that is already assumed to be synchronous to "clock" domain
         logic reset_n;
         HDBULT11_INV_1 reset_inv (.X(reset_n), .A(reset));
         
         logic [WIDTH-1:0] din_d2;
         if (RESET_VAL==0) begin :rst_low
            HDBULT08_SYNC2RBMSFQ_Y4_2 et_cdc_sync_meta_flops [WIDTH-1:0] (.Q(din_d2), .CK(clock), .D(din), .RD(reset_n), .SI (1'b0), .SE (1'b0));
         end                                                                                                            
         else begin :rst_high                                                                                           
            HDBULT08_SYNC2SBMSFQ_Y4_2 et_cdc_sync_meta_flops [WIDTH-1:0] (.Q(din_d2), .CK(clock), .D(din), .SD(reset_n), .SI (1'b0), .SE (1'b0));
         end
         assign dout = din_d2;
      end
   endgenerate
   wire param_unused = &{1'b0, &SYNC_STAGES};
      
endmodule
`endif  // ET_LIBS_USE_STD_CELLS



`include "soc.vh"

module esr_monitor (
 // System input signals
   input logic                                          clock, // Global
   input logic                                          reset_c, // Global
   input logic [`NUM_SHIRE_IDS_R] 		        int_shire_id // Shire ID for this module
);

`define SHIRE_ESR shire_channel.esrs

//////////////////////////////////////////////////
// DPI to update BEMU once ICache Prefetch is done
//////////////////////////////////////////////////
    import "DPI-C" context function void icache_prefetch_done (input int unsigned shire_id);

logic prefetch_done;
logic prefetch_on_going;
logic prefetch_on_going_next;

     always_comb
      begin
  	   prefetch_on_going_next = !`SHIRE_ESR.icache_prefetch_all_idle;
      end

    `RST_FF(clock, reset_c, prefetch_on_going, prefetch_on_going_next, '0)
    
    assign prefetch_done = prefetch_on_going & ~prefetch_on_going_next;


    always_ff @(posedge clock) begin
        if (prefetch_done) begin
     	   icache_prefetch_done(int_shire_id); 
        end
    end // always_ff @ (negedge clock)

endmodule // esr_monitor

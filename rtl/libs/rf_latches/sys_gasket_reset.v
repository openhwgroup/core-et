// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

module sys_gasket_reset (
  input  logic clock           ,
  input  logic reset_warm_in   ,
  input  logic gpio_ndmreset_in,
  input  logic ndmreset_in     ,
  input  logic dmactive_in     ,
  input  logic reset_cold_in   ,
  input  logic dft__reset_byp  ,
  input  logic dft__reset      ,
  output logic reset_w         ,
  output logic reset_c         ,
  output logic reset_d            
);
parameter RST_RESYNC = 1 ; // 0:input resets pass through "rst_repeat", 1: input resets pass through "sync_rst" 

// INTERNAL PARAMETERS
localparam DEBUG_RST_PULSE_CNT_SZ = $clog2(`DEBUG_RST_PULSE_CYCLES);  
localparam DEBUG_RST_PULSE_CYCLES = `DEBUG_RST_PULSE_CYCLES-1;  

  wire reset_warm_in_sync    ;
  wire gpio_ndmreset_in_sync ; 
  wire gpio_ndmreset_active  ; 
  wire ndmreset_in_sync      ; 
  wire ndmreset_active       ; 
  wire dmactive_in_sync      ;
  wire reset_cold_in_sync    ;
  wire reset_d_counter_rst   ;

  logic dm_reset_pulse;
  logic dm_reset_pulse_dft;

  generate 
  if (RST_RESYNC==1'b1) begin : IN_RST_SYNCHRONIZERS 
    sync_rst 
    sync_rst_reset_warm_in( 
      .dft__reset_byp( dft__reset_byp     ),
      .dft__reset    ( dft__reset         ),
      .clock         ( clock              ),
      .reset_in      ( reset_warm_in      ),
      .reset_out     ( reset_warm_in_sync )
    );

    sync_rst 
    sync_rst_ndmreset_in( 
      .dft__reset_byp( dft__reset_byp   ),
      .dft__reset    ( dft__reset       ),
      .clock         ( clock            ),
      .reset_in      ( ndmreset_in      ),
      .reset_out     ( ndmreset_in_sync )
    );

    sync_rst 
    sync_rst_gpio_ndmreset_in( 
      .dft__reset_byp( dft__reset_byp        ),
      .dft__reset    ( dft__reset            ),
      .clock         ( clock                 ),
      .reset_in      ( gpio_ndmreset_in      ),
      .reset_out     ( gpio_ndmreset_in_sync )
    );

    sync_rst 
    sync_rst_dmactive_in( 
      .dft__reset_byp ( dft__reset_byp   ),
      .dft__reset     ( dft__reset       ),
      .clock          ( clock            ),
      .reset_in       ( dmactive_in      ),
      .reset_out      ( dmactive_in_sync )
    );
    sync_rst 
    sync_rst_reset_cold_in( 
      .dft__reset_byp ( dft__reset_byp     ),
      .dft__reset     ( dft__reset         ),
      .clock          ( clock              ),
      .reset_in       ( reset_cold_in      ),
      .reset_out      ( reset_cold_in_sync )
    );
  end else begin : IN_RST_REPEATERS
    rst_repeat 
    rst_repeat_reset_warm_in( 
      .dft__reset_byp( dft__reset_byp     ),
      .dft__reset    ( dft__reset         ),
      .clock         ( clock              ),
      .reset_in      ( reset_warm_in      ),
      .reset_out     ( reset_warm_in_sync )
    );

    rst_repeat 
    rst_repeat_ndmreset_in( 
      .dft__reset_byp( dft__reset_byp   ),
      .dft__reset    ( dft__reset       ),
      .clock         ( clock            ),
      .reset_in      ( ndmreset_in      ),
      .reset_out     ( ndmreset_in_sync )
    );

    rst_repeat 
    rst_repeat_gpio_ndmreset_in( 
      .dft__reset_byp( dft__reset_byp        ),
      .dft__reset    ( dft__reset            ),
      .clock         ( clock                 ),
      .reset_in      ( gpio_ndmreset_in      ),
      .reset_out     ( gpio_ndmreset_in_sync )
    );

    rst_repeat 
    rst_repeat_dmactive_in( 
      .dft__reset_byp ( dft__reset_byp   ),
      .dft__reset     ( dft__reset       ),
      .clock          ( clock            ),
      .reset_in       ( dmactive_in      ),
      .reset_out      ( dmactive_in_sync )
    );
    rst_repeat 
    rst_repeat_reset_cold_in( 
      .dft__reset_byp ( dft__reset_byp     ),
      .dft__reset     ( dft__reset         ),
      .clock          ( clock              ),
      .reset_in       ( reset_cold_in      ),
      .reset_out      ( reset_cold_in_sync )
    );
  end
  endgenerate

  assign ndmreset_active      = ndmreset_in_sync & dmactive_in_sync;
  assign gpio_ndmreset_active = gpio_ndmreset_in_sync; 

  assign reset_d_counter_rst = dmactive_in_sync | reset_cold_in_sync ;   
  logic [DEBUG_RST_PULSE_CNT_SZ-1:0] dmreset_pulse_counter;
  always_ff @ (posedge clock or posedge reset_d_counter_rst) 
  begin
    if    (reset_d_counter_rst)
      dmreset_pulse_counter <= '0;
    else  if (dmreset_pulse_counter < DEBUG_RST_PULSE_CYCLES ) 
      dmreset_pulse_counter <= dmreset_pulse_counter+1;
  end

  logic dm_reset_pulse_reg;
  always_ff @ (posedge clock or posedge reset_d_counter_rst) 
  begin
    if    (reset_d_counter_rst)
      dm_reset_pulse_reg <= 1'b1;
    else  if (dmreset_pulse_counter >= DEBUG_RST_PULSE_CYCLES ) 
      dm_reset_pulse_reg <= 1'b0;
  end

  assign  dm_reset_pulse = ~dmactive_in_sync & dm_reset_pulse_reg ;
  et_clk_mux2 dm_reset_pulse_mux (.x(dm_reset_pulse_dft), .d0(dm_reset_pulse), .d1(dft__reset), .s(dft__reset_byp));

  // Outputs
  assign  reset_w = ndmreset_active      |
                    reset_warm_in_sync   | 
                    reset_cold_in_sync   |
                    gpio_ndmreset_active ;

  assign  reset_d = dm_reset_pulse_dft | reset_cold_in_sync   ;

  assign  reset_c = reset_cold_in_sync ;

endmodule

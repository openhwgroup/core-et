// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


module signals_cdc 
#(
    parameter   NUM_ELEMS               = 10,
    parameter   SIGNALS_MASTER_WIDTH    = 1,
    parameter   SIGNALS_SLAVE_WIDTH     = 1,
    parameter   ENABLE_MASTER_CDC       = 1,
    parameter   ENABLE_SLAVE_CDC        = 1  
)
(
    input   logic                               clk_master,
    input   logic                               reset_master,
    input   logic                               clk_slave,
    input   logic                               reset_slave,

    // APB interface with clk_master
    input   logic   [SIGNALS_MASTER_WIDTH-1:0]  signals_master_ip, 
    output  logic   [SIGNALS_MASTER_WIDTH-1:0]  signals_slave_op,

    // APB interface with clk_slave
    input   logic   [SIGNALS_SLAVE_WIDTH-1:0]   signals_slave_ip,  
    output  logic   [SIGNALS_SLAVE_WIDTH-1:0]   signals_master_op,


    input  logic                                dft__reset_byp_master,
    input  logic                                dft__reset_master,
    input  logic                                dft__reset_byp_slave,
    input  logic                                dft__reset_slave
 );

///////////////////////////////////////////
// MASTER APB (clk_master) TO SLAVE APB (clk_slave)
///////////////////////////////////////////

genvar gen_master_signals;
generate
    for ( gen_master_signals = 0; gen_master_signals < ENABLE_MASTER_CDC; ++gen_master_signals) begin :MASTER_SIGNALS
    
        logic master_ip_vcfifo_valid;
        logic master_ip_vcfifo_push;
        logic   [SIGNALS_MASTER_WIDTH-1:0]  signals_master_ip_ff;
        logic   [SIGNALS_MASTER_WIDTH-1:0]  signals_slave_op_last;
        logic   [SIGNALS_MASTER_WIDTH-1:0]  signals_slave_op_last_ff;

        //        CLK         RST           DOUT                  DINA               DEF
        `RST_FF ( clk_master, reset_master, signals_master_ip_ff, signals_master_ip, '0 )

        //           CLK         RST           EN,                     DOUT                      DINA                  DEF
        `RST_EN_FF ( clk_slave,  reset_slave,  master_ip_vcfifo_valid, signals_slave_op_last_ff, signals_slave_op_last, '0)
        always_comb
        begin
            signals_slave_op = signals_slave_op_last_ff;
            if (master_ip_vcfifo_valid) begin
                signals_slave_op = signals_slave_op_last;
            end
        end

        assign master_ip_vcfifo_push = ( signals_master_ip != signals_master_ip_ff );

        fifo_gcd
        #(
            .ELEM_SIZE ( $bits( signals_master_ip )),
            .NUM_ELEMS ( NUM_ELEMS )
        )
        master_signals_vcfifo_wr_master
        (
            .clock_push          ( clk_master                       ),
            .reset_push          ( reset_master                     ),
            .clock_pop           ( clk_slave                        ),
            .reset_pop           ( reset_slave                      ),
            .push                ( master_ip_vcfifo_push            ),
            .push_data           ( signals_master_ip                ),
            .pop_data            ( signals_slave_op_last            ),
            .ready               (                                  ),
            .pop                 ( master_ip_vcfifo_valid           ),
            .valid               ( master_ip_vcfifo_valid           ),
            .dft__reset_byp_push ( dft__reset_byp_master            ),
            .dft__reset_push     ( dft__reset_master                ),
            .dft__reset_byp_pop  ( dft__reset_byp_slave             ),
            .dft__reset_pop      ( dft__reset_slave                 )
        );
    end
endgenerate

///////////////////////////////////////////
// SLAVE APB (clk_slave) TO MASTER APB (clk_master)
///////////////////////////////////////////

genvar gen_slave_signals;
generate
    for ( gen_slave_signals = 0; gen_slave_signals < ENABLE_SLAVE_CDC; ++gen_slave_signals) begin :SLAVE_SIGNALS

        logic slave_ip_vcfifo_valid;
        logic slave_ip_vcfifo_push;
        logic   [SIGNALS_SLAVE_WIDTH-1:0]   signals_slave_ip_ff;
        logic   [SIGNALS_SLAVE_WIDTH-1:0]   signals_master_op_last;

        //        CLK        RST          DOUT                 DINA              DEF
        `RST_FF ( clk_slave, reset_slave, signals_slave_ip_ff, signals_slave_ip, '0 )

        //           CLK        RST          EN,                    DOUT               DINA                    DEF
        `RST_EN_FF ( clk_slave, reset_slave, slave_ip_vcfifo_valid, signals_master_op, signals_master_op_last, '0)


        assign slave_ip_vcfifo_push = ( signals_slave_ip != signals_slave_ip_ff ); 
        // ---------------------------------------------------------------------------
        fifo_gcd
        #(
           .ELEM_SIZE ( $bits( signals_slave_ip )),
           .NUM_ELEMS ( NUM_ELEMS )
        )
        slave_signals_vcfifo_wr_slave
        (
          .clock_push          ( clk_slave                ),
          .reset_push          ( reset_slave              ),
          .clock_pop           ( clk_master               ),
          .reset_pop           ( reset_master             ),
          .push                ( slave_ip_vcfifo_push     ),
          .push_data           ( signals_slave_ip         ),
          .pop_data            ( signals_master_op_last   ),
          .ready               (                          ),
          .pop                 ( slave_ip_vcfifo_valid    ),
          .valid               ( slave_ip_vcfifo_valid    ),
          .dft__reset_byp_push ( dft__reset_byp_slave     ),
          .dft__reset_push     ( dft__reset_slave         ),
          .dft__reset_byp_pop  ( dft__reset_byp_master    ),
          .dft__reset_pop      ( dft__reset_master        )
        );
    end
endgenerate

endmodule

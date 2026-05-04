// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module bpam2minions # ( parameter AD_IN_WIDTH=19,
  parameter AD_OUT_WIDTH = 10,
  parameter DATA_WIDTH = 32,
  parameter NR_MINIONS = `MIN_PER_N
) (
  input                                   clock,
  input                                   reset,
  //////////////////////////////////////////////////////////////
  // signals to / from bpam
  //////////////////////////////////////////////////////////////
  input [AD_IN_WIDTH-1:0]                 req_apb_paddr,
  input                                   req_apb_pwrite, 
  input                                   req_apb_psel, 
  input                                   req_apb_penable,
  input [DATA_WIDTH-1:0]                  req_apb_pwdata,
   
  output logic                            rsp_apb_pready,
  output logic [DATA_WIDTH-1:0]           rsp_apb_prdata,
  output logic                            rsp_apb_pslverr,
   
   //////////////////////////////////////////////////////////////
   // signals to / from minions
   //////////////////////////////////////////////////////////////
   output logic [AD_OUT_WIDTH-1:0]         minion_apb_paddr,
   output logic                            minion_apb_pwrite, 
   output logic [NR_MINIONS-1:0]           minion_apb_psel, 
   output logic                            minion_apb_penable,
   output logic [DATA_WIDTH-1:0]           minion_apb_pwdata,
   

   input [NR_MINIONS-1:0]                  minion_apb_pready,
   input [NR_MINIONS-1:0] [DATA_WIDTH-1:0] minion_apb_prdata,
   input [NR_MINIONS-1:0]                  minion_apb_pslverr
   );
   
   logic                                    req_start;
   logic [$clog2(NR_MINIONS)-1:0]           slave_sel;
   logic [AD_OUT_WIDTH-1:0]                 minion_apb_enc_addr;

   logic [$clog2(NR_MINIONS)-1:0]           minion_sel;
   logic [NR_MINIONS-1:0]                   minion_apb_psel_next;
   logic                                    minion_done;
   logic                                    minion_start;
   
   
// sbm/bpam side
always_comb begin
  req_start           = req_apb_psel && !req_apb_penable;
  slave_sel           = req_apb_paddr[`NEIGH_DM_APB_ADDR_MIN_RANGE];
  minion_apb_enc_addr = { req_apb_paddr[`NEIGH_DM_APB_ADDR_DEST_RANGE],
                        req_apb_paddr[`NEIGH_DM_APB_ADDR_PP_RANGE],
                        req_apb_paddr[`NEIGH_DM_APB_ADDR_THREAD_RANGE],
			            req_apb_paddr[`NEIGH_DM_APB_ADDR_REG_RANGE]
                        };
end

//          CLK     RST     EN                        DOUT             DIN                  DEF
`RST_EN_FF( clock,  reset,  req_start || minion_done, minion_apb_psel, minion_apb_psel_next, '0)

logic rsp_apb_pready_aux;
logic minion_penable_aux;

assign rsp_apb_pready_aux = ( minion_done ) ? 1'b1 : 
                            ( rsp_apb_pready ) ? 1'b0 : 
                             rsp_apb_pready;

assign minion_penable_aux = ( minion_start ) ? 1'b1 : 
                            ( minion_done  ) ? 1'b0 : 
                            minion_apb_penable;

//      CLK    RST    DOUT                DIN                  DEF
`RST_FF(clock, reset, rsp_apb_pready,     rsp_apb_pready_aux , 1'b0 )
`RST_FF(clock, reset, minion_start,       req_start,           1'b0 )
`RST_FF(clock, reset, minion_apb_penable, minion_penable_aux , 1'b0 )


//      CLK   EN           DOUT               DIN                  
`EN_FF(clock, minion_done, rsp_apb_prdata,    minion_apb_prdata[minion_sel] )
`EN_FF(clock, minion_done, rsp_apb_pslverr,   minion_apb_pslverr[minion_sel])
`EN_FF(clock, req_start,   minion_apb_paddr,  minion_apb_enc_addr           )
`EN_FF(clock, req_start,   minion_apb_pwrite, req_apb_pwrite                )
`EN_FF(clock, req_start,   minion_apb_pwdata, req_apb_pwdata                )

// minion side
always_comb begin
  minion_sel                      = req_apb_paddr[`NEIGH_DM_APB_ADDR_MIN_RANGE];
  minion_done                     = minion_apb_penable && minion_apb_pready[minion_sel];
  minion_apb_psel_next            = '0;
  minion_apb_psel_next[slave_sel] = !minion_done;
end



endmodule // apb2apb_bridge

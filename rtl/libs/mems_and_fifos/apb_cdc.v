// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


module apb_cdc 
#(
    parameter   NUM_ELEMS           = 10,
    parameter   apb_addr_width      = `SPIO_APB_MUX_ADDR_WIDTH,
    parameter   apb_data_width      = `SPIO_APB_MUX_DATA_WIDTH
)
(
    input   logic                           clk_master,
    input   logic                           reset_master,
    input   logic                           clk_slave,
    input   logic                           reset_slave,

    // APB interface with clk_master
    input   logic   [apb_addr_width - 1:0]  master_apb_paddr,        
    input   logic                           master_apb_penable,      
    input   logic                           master_apb_psel,         
    input   logic   [apb_data_width - 1:0]  master_apb_pwdata,      
    input   logic                           master_apb_pwrite,   
    output  logic                           master_apb_pslverr,      
    output  logic   [apb_data_width - 1:0]  master_apb_prdata,       
    output  logic                           master_apb_pready,       

    // APB interface with clk_slave
    output  logic   [apb_addr_width - 1:0]  slave_apb_paddr,        
    output  logic                           slave_apb_penable,     
    output  logic                           slave_apb_psel,         
    output  logic   [apb_data_width - 1:0]  slave_apb_pwdata,      
    output  logic                           slave_apb_pwrite,
    input   logic                           slave_apb_pslverr,      
    input   logic   [apb_data_width - 1:0]  slave_apb_prdata,       
    input   logic                           slave_apb_pready,

    input  logic                            dft__reset_byp_master,
    input  logic                            dft__reset_master,
    input  logic                            dft__reset_byp_slave,
    input  logic                            dft__reset_slave
 );

typedef struct packed {
   logic    [apb_addr_width - 1:0]  apb_paddr   ;
   logic                            apb_penable ;
   logic                            apb_psel    ;
   logic    [apb_data_width - 1:0]  apb_pwdata  ;
   logic                            apb_pwrite  ;
} apb_request;

typedef struct packed {
   logic                            apb_pslverr ;                                     
   logic    [apb_data_width - 1:0]  apb_prdata  ;                                     
   logic                            apb_pready  ;
} apb_response;

///////////////////////////////////////////
// MASTER APB (clk_master) TO SLAVE APB (clk_slave)
///////////////////////////////////////////
apb_response     apb_esr_rsp_sync        ; // connected to the output ports of clk_master

logic apb_esr_req_vcfifo_valid;
apb_request     apb_esr_req             ; // connected to the input ports of clk_master
apb_request     apb_esr_req_sync        ; // connected to the output ports of clk_slave
apb_request     apb_esr_req_pop         ;
apb_request     apb_esr_req_last        ;
apb_request     apb_esr_req_last_next   ;

logic apb_esr_req_vcfifo_push;

always_comb
begin
    apb_esr_req.apb_paddr   = master_apb_paddr      ; 
    apb_esr_req.apb_penable = master_apb_penable    ;
    apb_esr_req.apb_psel    = master_apb_psel       ;
    apb_esr_req.apb_pwdata  = master_apb_pwdata     ;
    apb_esr_req.apb_pwrite  = master_apb_pwrite     ;

    slave_apb_paddr   = apb_esr_req_sync.apb_paddr  ; 
    slave_apb_penable = apb_esr_req_sync.apb_penable;
    slave_apb_psel    = apb_esr_req_sync.apb_psel   ;
    slave_apb_pwdata  = apb_esr_req_sync.apb_pwdata ;
    slave_apb_pwrite  = apb_esr_req_sync.apb_pwrite ;
end

// Push logic for the master
// ---------------------------------------------------------------------
neigh_hv_logic_apb_req
neigh_hv_logic_apb_req_pd_master
(
  .reset_push              ( reset_master                   ),
  .clock_push              ( clk_master                     ),
  .apb_pready_neigh        ( apb_esr_rsp_sync.apb_pready    ),
  .apb_esr_req_apb_psel    ( apb_esr_req.apb_psel           ),
  .apb_esr_req_apb_penable ( apb_esr_req.apb_penable        ),
  .apb_esr_req_vcfifo_push ( apb_esr_req_vcfifo_push        )
);

// ---------------------------------------------------------------------
fifo_gcd
#(
    .ELEM_SIZE ( $bits( apb_esr_req )),
    .NUM_ELEMS ( NUM_ELEMS )
)
apb_esr_req_vcfifo_wr_master
(
    .clock_push          ( clk_master                       ),
    .reset_push          ( reset_master                     ),
    .clock_pop           ( clk_slave                        ),
    .reset_pop           ( reset_slave                      ),
    .push                ( apb_esr_req_vcfifo_push          ),
    .push_data           ( apb_esr_req                      ),
    .pop_data            ( apb_esr_req_pop                  ),
    .ready               (                                  ),
    .pop                 ( apb_esr_req_vcfifo_valid         ),
    .valid               ( apb_esr_req_vcfifo_valid         ),
    .dft__reset_byp_push ( dft__reset_byp_master            ),
    .dft__reset_push     ( dft__reset_master                ),
    .dft__reset_byp_pop  ( dft__reset_byp_slave             ),
    .dft__reset_pop      ( dft__reset_slave                 )
);

// Pop logic for the slave
// ---------------------------------------------------------------------
always_comb begin
  apb_esr_req_last_next = apb_esr_req_last;

  if (apb_esr_req_vcfifo_valid) 
  begin
    apb_esr_req_sync = apb_esr_req_pop;
    apb_esr_req_last_next                = apb_esr_req_pop;
    apb_esr_req_last_next.apb_penable    = 1'b0;
    apb_esr_req_last_next.apb_psel       = 1'b0;
  end
  else 
  begin
    apb_esr_req_sync = apb_esr_req_last;
  end
end

//       CLK          RST          DOUT               DIN
`RST_FF (clk_slave  , reset_slave , apb_esr_req_last , apb_esr_req_last_next , '0)

///////////////////////////////////////////
// SLAVE APB (clk_slave) TO MASTER APB (clk_master)
///////////////////////////////////////////

apb_response     apb_esr_rsp             ; // connected to the input ports of clk_slave
apb_response     apb_esr_rsp_pop         ;

always_comb
begin
    apb_esr_rsp.apb_pslverr = slave_apb_pslverr ; 
    apb_esr_rsp.apb_prdata  = slave_apb_prdata  ;
    apb_esr_rsp.apb_pready  = slave_apb_pready  ;

    master_apb_pslverr  = apb_esr_rsp_sync.apb_pslverr  ; 
    master_apb_prdata   = apb_esr_rsp_sync.apb_prdata   ;
    master_apb_pready   = apb_esr_rsp_sync.apb_pready   ;
end

logic apb_esr_rsp_vcfifo_valid;
logic apb_esr_rsp_vcfifo_push;

// Push logic for the slave
// ---------------------------------------------------------------------------
assign apb_esr_rsp_vcfifo_push = apb_esr_rsp.apb_pready;

// ---------------------------------------------------------------------------
fifo_gcd
#(
   .ELEM_SIZE ( $bits( apb_esr_rsp )),
   .NUM_ELEMS ( NUM_ELEMS )
)
apb_esr_rsp_vcfifo_wr_slave
(
  .clock_push          ( clk_slave                ),
  .reset_push          ( reset_slave              ),
  .clock_pop           ( clk_master               ),
  .reset_pop           ( reset_master             ),
  .push                ( apb_esr_rsp_vcfifo_push  ),
  .push_data           ( apb_esr_rsp              ),
  .pop_data            ( apb_esr_rsp_pop          ),
  .ready               (                          ),
  .pop                 ( apb_esr_rsp_vcfifo_valid ),
  .valid               ( apb_esr_rsp_vcfifo_valid ),
  .dft__reset_byp_push ( dft__reset_byp_slave     ),
  .dft__reset_push     ( dft__reset_slave         ),
  .dft__reset_byp_pop  ( dft__reset_byp_master    ),
  .dft__reset_pop      ( dft__reset_master        )
);

// Pop logic for master
// ---------------------------------------------------------------------------
always_comb 
begin
    apb_esr_rsp_sync = '0;

    if (apb_esr_rsp_vcfifo_valid) begin
      apb_esr_rsp_sync = apb_esr_rsp_pop;
    end   
end 

endmodule
